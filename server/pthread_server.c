#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
	int house_number;
	int pad[15][15];
	char owner_name[20];
	char visitor_name[20];
	int people_counting;
	int cnt;
}House;

//定义房间
House house;
pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;//定义互斥锁
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;//定义条件变量

//线程调用函数
void *rec_data(void *fd);

int main(int argc,char *argv[]){
	int server_sockfd;
	int *client_sockfd;//利用指针变量与线程中分配堆存储可重复利用该变量名
	int server_len,client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	struct sockaddr_in tempaddr;
	socklen_t templen;
	//IPV4 TCP
	server_sockfd = socket(AF_INET,SOCK_STREAM,0);//创建套接字

	//初始化server_address结构体
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
	server_len = sizeof(server_address);

	//绑定套接字
	bind(server_sockfd,(struct sockaddr *)&server_address,server_len);
	templen = sizeof(struct sockaddr);
	//启动监听	
	listen(server_sockfd,30);
	printf("server waiting for connect\n");

	while(1){
		pthread_t thread;//创建用以区别不同客户端的子线程的参数
		client_sockfd = (int *)malloc(sizeof(int));
		client_len = sizeof(client_address);
		*client_sockfd = accept(server_sockfd,\
			(struct sockaddr *)&client_address,(socklen_t *)&client_len);
		if(-1 == *client_sockfd){
			perror("accept");
			continue;
		}

		//将客户端ip地址打印出来
		char ip[128] = {0};
		printf("%s登录服务器...\n",\
				inet_ntop(AF_INET,&client_address.sin_addr,ip,128));

		if(pthread_create(&thread,NULL,rec_data,client_sockfd) != 0){
		//创建子线程
			perror("pthread_create");
			break;
		}
	}
	shutdown(*client_sockfd,2);
	shutdown(server_sockfd,2);
}
void *rec_data(void *fd){
	//定义用户名临时存储区
	char name[20];
	//建立读写房间全局变量的指针
	House *p_house = &house;
	//线程与之通信的客户端id
	int id = 0;

	int client_sockfd;
	int i,byte;
	char char_recv[100];//存放数据
	client_sockfd = *((int *)fd);

	//第一步获取玩家姓名
	if((byte = recv(client_sockfd,name,20,0)) == -1){
		perror("recv_name");
		exit(EXIT_FAILURE);
	}
	//第二步获取到玩家姓名后发送房间状态及读取菜单选项（循环）
	char option = 0;
	while(option != 'y' && option != 'n'){
		//此处待补充mutex锁
//		pthread_mutex_lock(&clifd_mutex);
		if((byte = send(client_sockfd,&house,sizeof(House),0)) == -1){
			perror("send_house");
			exit(EXIT_FAILURE);
		}
//		pthread_mutex_unlock(&clifd_mutex);
		//接收用户菜单指令
		if((byte = recv(client_sockfd,&option,1,0)) == -1){
			perror("recv_option");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
	//对菜单选项做出反应
	//此处待补充mutex锁
	if('y' == option){
		pthread_mutex_lock(&clifd_mutex);
		if(house.people_counting == 0){
			strcpy(house.owner_name,name);
		}
		else if(house.people_counting == 1){
			strcpy(house.visitor_name,name);
		}
		house.people_counting++;
		//将用户id发送给客户端
		if((byte = send(client_sockfd,&(house.people_counting),sizeof(int),\
						0)) == -1){
			perror("send_id");
			exit(EXIT_FAILURE);
		}

		id = house.people_counting;

		pthread_mutex_unlock(&clifd_mutex);
	}
	//此处待补充循环读取房间状态，用mutex锁
	while(1){
//		pthread_mutex_lock(&clifd_mutex);
		if(house.people_counting <= 1){
			continue;
		}
		else{
			//将房间信息发送给客户端，准备开始游戏
			if((byte = send(client_sockfd,&house,sizeof(House),0)) == -1){
				perror("send_house_end");
				exit(EXIT_FAILURE);
			}
			break;
		}
//		pthread_mutex_unlock(&clifd_mutex);
	}
	//游戏开始
	if(id > 2){//观众
		House house2;
		int pre_cnt = 0;
		while(1){
//			pthread_mutex_lock(&clifd_mutex);
			while(house.cnt == pre_cnt){
//				pthread_cond_wait(&clifd_cond,&clifd_mutex);
			}
			//1.发送现在房间信息给客户端
			if((byte = send(client_sockfd,&house,sizeof(House),0)) == -1){
				perror("send_play");
				exit(EXIT_FAILURE);
			}
			//2.从客户端接收修改后的房间信息
			if((byte = recv(client_sockfd,&house2,sizeof(House),0)) == -1){
				perror("recv_play");
				exit(EXIT_FAILURE);
			}
			pre_cnt = house.cnt;
//			pthread_mutex_unlock(&clifd_mutex);
//			pthread_cond_broadcast(&clifd_cond);
		}
	
	}
	else{
		while(1){
			pthread_mutex_lock(&clifd_mutex);
			while(((house.cnt % 2) + 1)!=id){
				pthread_cond_wait(&clifd_cond,&clifd_mutex);
			}
			//以下为线程的操作
			//1.发送现在房间信息给客户端
			if((byte = send(client_sockfd,&house,sizeof(House),0)) == -1){
				perror("send_play");
				exit(EXIT_FAILURE);
			}
			//2.从客户端接收修改后的房间信息
			if((byte = recv(client_sockfd,&house,sizeof(House),0)) == -1){
				perror("recv_play");
				exit(EXIT_FAILURE);
			}
			pthread_mutex_unlock(&clifd_mutex);
			pthread_cond_broadcast(&clifd_cond);
		}	
	}
	for(;;){
		if((byte = recv(client_sockfd,char_recv,100,0)) == -1){
			perror("recv");
			exit(EXIT_FAILURE);
		}
		if(strcmp(char_recv,"exit") == 0)//接收到exit 跳出循环
			break;
		printf("receive from client if %s\n",char_recv);
		//打印收到的数据
	}
	free(fd);
	close(client_sockfd);
	pthread_exit(NULL);
}
