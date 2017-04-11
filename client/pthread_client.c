#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "show.h"
#include "tree.h"
#include "key.h"
typedef struct{
	int house_number;
	int pad[15][15];
	char owner_name[20];
	char visitor_name[20];
	int people_counting;
	int cnt;
}House;

int main(int argc,char *argv[]){
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	int i,byte;
	char char_send[100] = {0};
    

    //人机程序
    char if_robot = 0;
	printf("是否进入人机对战（y/n）？：");
	scanf("%c",&if_robot);
	scanf("%*[^\n]");
	scanf("%*c");
    if('y'==if_robot){
		disable_terminal_return();
        printf("电脑正在加入对局，请您准备落子\n");
        sleep(1);

	House house = {0};
    house.cnt = 1;
    while(house.cnt < 225){
        int data = 0;
        int row,col;
        if(house.cnt % 2){
            data = show_up(house.pad);
        }
        else{
            system("clear");
            print_a(house.pad);
            printf("\n    电脑正在思考中...\n");
            data = get_data(house.pad);
        }
        row = data / 15;
        col = data % 15;

        if(house.cnt % 2){
            house.pad[row][col] = 2;
        }
        else{
            house.pad[row][col] = 1;
        }
        if(check_lose(house.pad) == 1){
            system("clear");
            print_a(house.pad);
            printf("\n   哈哈哈，电脑赢了～\n");
            break;
        }
        else if(check_lose(house.pad) == -1){
            system("clear");
            print_a(house.pad);
            printf("\n   恭喜你赢了\n");
            break;
        }
        house.cnt++;
    }
    if(check_lose(house.pad) == 0){
        system("clear");
        print_a(house.pad);
        printf("\n  和棋！\n");
    }


        restore_terminal_settings();
        return 0;
    }
    


	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("sock");
		exit(1);
	}
	bzero(&address,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi("9734"));
	inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);
	len = sizeof(address);

	if((result = connect(sockfd,(struct sockaddr *)&address,len)) == -1){
		perror("connect");
		exit(EXIT_FAILURE);
	}

	printf("connect succeed...\n");
	
	//创建用户名字
	char my_name[20] = {0};
	printf("请输入您的游戏中称号:");
	scanf("%s",my_name);
	scanf("%*[^\n]");
	scanf("%*c");
	//发送用户名字给服务器
	if((byte = send(sockfd,my_name,20,0)) == -1){
		perror("send_name");
		exit(EXIT_FAILURE);
	}
	int my_id;//记录自己的身份1房主 2访客 3
	//游戏选项操作 //循环接收房间信息 休眠等待输入 刷新

	House house;
	char option = 0;
	while(option != 'y' && option != 'n'){
		//从服务器接收房间信息
		if((byte = recv(sockfd,&house,sizeof(House),0)) == -1){
			perror("recv_house");
			exit(EXIT_FAILURE);
		}
		system("clear");
		//打印房间信息菜单
		if(house.people_counting == 0){
			printf("当前无可用游戏房间\n是否创建(y/n)（输入q手动刷新）:");
		}
		else{
			printf("房间号:%d\n",house.house_number);
			printf("当前状态:");
			if(house.people_counting == 1){
				printf("可加入游戏\n是否加入(y/n)（输入q手动刷新）:");
			}
			else if(house.people_counting >= 2){
				printf("可观战\n是否观战(y/n)(输入q手动刷新）:");
			}
		}
		option = getchar();//获取玩家输入的菜单操作
		scanf("%*[^\n]");
		scanf("%*c");

		//将菜单操作发送给服务器
		if((byte = send(sockfd,&option,1,0)) == -1){
			perror("send_option");
			exit(EXIT_FAILURE);
		}
	}
#if 0
	//将菜单操作发送给服务器
	if((byte = send(sockfd,&option,1,0)) == -1){
		perror("send_option");
		exit(EXIT_FAILURE);
	}
#endif
	printf("正在进入游戏，请稍等...\n");

	//从服务器接收自己的身份id
	if((byte = recv(sockfd,&my_id,sizeof(int),0)) == -1){
		perror("recv_my_id");
		exit(EXIT_FAILURE);
	}	
	
	//等待从服务器接收房间信息
	if((byte = recv(sockfd,&house,sizeof(House),0)) == -1){
		perror("recv_wait_house");
		exit(EXIT_FAILURE);
	}	
	
	if(my_id == 1){
		printf("%s已进入房间\n游戏即将开始...\n",house.visitor_name);
	}
	else if(my_id == 2){
		printf("已进入房间\n房主%s\n游戏即将开始...\n",house.owner_name);
	}
	else{
		printf("已进入房间\n即将开始观战...\n");
	}
	
	//进入游戏操作
	if(my_id > 2){//观众
		while(1){
			//等待从服务器接收房间信息
			if((byte = recv(sockfd,&house,sizeof(House),0)) == -1){
				perror("recv_play");
				exit(EXIT_FAILURE);
			}
			system("clear");
			print_a(house.pad);
			if(((house.cnt % 2)+1)==1){
				printf("\n  %s正在落子...\n",house.owner_name);
			}			
			else{
				printf("\n  %s正在落子...\n",house.visitor_name);
			}
			//将房间信息原样发送给服务器
			if((byte = send(sockfd,&house,sizeof(House),0)) == -1){
				perror("send_play");
				exit(EXIT_FAILURE);
			}
		}
	}
	else{//玩家
		disable_terminal_return();
		while(1){
			system("clear");
			print_a(house.pad);
			if(((house.cnt % 2)+1)==1){
				printf("\n  %s正在落子...\n",house.owner_name);
			}			
			else{
				printf("\n  %s正在落子...\n",house.visitor_name);
			}
			//等待从服务器接收房间信息
			if((byte = recv(sockfd,&house,sizeof(House),0)) == -1){
				perror("recv_play");
				exit(EXIT_FAILURE);
			}			
			//此处落子等操作
			int data = 0;
			int row,col;
			data = show_up(house.pad);
			row = data/15;
			col = data%15;
			house.pad[row][col] = my_id;

			house.cnt++;	
			//将修改后的房间信息发送给服务器
			if((byte = send(sockfd,&house,sizeof(House),0)) == -1){
				perror("send_play");
				exit(EXIT_FAILURE);
			}
		
		}
	}
	restore_terminal_settings();
	for(;;){
		scanf("%s",char_send);//输入发送数据
		fflush(stdin);//清除输入缓存
		if(strcmp(char_send,"exit") == 0){
			//如果输入exit 跳出循环
			if((byte = send(sockfd,char_send,100,0)) == -1){
				perror("send");
				exit(EXIT_FAILURE);
			}
			break;
		}
		if((byte = send(sockfd,char_send,100,0)) == -1){
			perror("send");
			exit(EXIT_FAILURE);
		}
	}
	close(sockfd);
	exit(0);
	
}
