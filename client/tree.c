//创建博弈链形树
#include "tree.h"
int cnt;
int chd,bro;
int ab[DEEP] ;//alpha beta
int tmp_ab[DEEP + 1];//层最大最小值
int vdata;//第一层最大权值的data
int data_flag;//改变data的标志位
int cmp_a[32][6] = {0,1,1,1,1,1,
				    1,1,1,1,1,0,
				    2,1,1,1,1,1,
					1,1,1,1,1,2,
					0,1,1,1,1,0,
					0,1,1,1,0,0,
					0,0,1,1,1,0,
					0,1,1,0,1,0,
					0,1,0,1,1,0,
					1,1,1,1,0,0,
					0,0,1,1,1,1,
					1,1,1,1,0,2,
					2,0,1,1,1,1,
					0,1,1,1,1,2,
					2,1,1,1,1,0,
					0,1,1,0,1,1,
					2,1,1,0,1,1,
					1,1,0,1,1,0,
					1,1,0,1,1,2,
					0,1,0,1,1,1,
					2,1,0,1,1,1,
					1,0,1,1,1,0,
					1,0,1,1,1,2,
					0,1,1,1,0,1,
					2,1,1,1,0,1,
					1,1,1,0,1,0,
					1,1,1,0,1,2,
					0,0,1,1,0,0,
					0,0,1,0,1,0,
					0,1,0,1,0,0,
					0,0,0,1,0,0,
					0,0,1,0,0,0 };
int cmp_b[32][6] = {
					0,2,2,2,2,2,
					2,2,2,2,2,0,
					1,2,2,2,2,2,
					2,2,2,2,2,1,
					0,2,2,2,2,0,
					0,2,2,2,0,0,
					0,0,2,2,2,0,
					0,2,2,0,2,0,
					0,2,0,2,2,0,
					2,2,2,2,0,0,
					0,0,2,2,2,2,
					2,2,2,2,0,1,
					1,0,2,2,2,2,
					0,2,2,2,2,1,
					1,2,2,2,2,0,
					0,2,2,0,2,2,
					1,2,2,0,2,2,
					2,2,0,2,2,0,
					2,2,0,2,2,1,
					0,2,0,2,2,2,
					1,2,0,2,2,2,
					2,0,2,2,2,0,
					2,0,2,2,2,1,
					0,2,2,2,0,2,
					1,2,2,2,0,2,
					2,2,2,0,2,0,
					2,2,2,0,2,1,
					0,0,2,2,0,0,
					0,0,2,0,2,0,
					0,2,0,2,0,0,
					0,0,0,2,0,0,
					0,0,2,0,0,0 };
int score[32]={50000,50000,50000,50000,4320,720,720,720,720,720,720,720,
				720,720,720,720,720,720,720,720,720,720,720,720,720,720,
				720,120,120,120,20,20};
ushort a[DEEP][W],b[DEEP][W];//创建点的父节点棋面值
//树的初始化函数
void tree_init(int tmp[][W]){
	int i = 0,j = 0;
	int tmp_pad[W][W];
	for(i = 0;i<W;i++){
		for(j = 0;j < W;j++){
			tmp_pad[i][j] = tmp[i][j];
			ab[i] = 1000000;//初始化
		}
	}
	int frw = 0;
	tree_creat_child(tmp_pad,&frw,1);
}
//树的有界创建—子节点
void tree_creat_child(int tmp_pad[][W],int *pw,int num){
	//如果深度到4 子节点创建结束
	if(num > DEEP){
		//深度到达-返回

		return ;
	}
	//如果父已决输赢 子节点创建结束
	if(check_lose(tmp_pad) != 0){
		int tt = 0;
		tt = node_weight(tmp_pad);
		*pw = tt;
		if(ab[num - 1] == 1000000){
			/**************************************************
			  此处“或该层同父分支已完，应赋值” 有疑问
			  *************************************************/
			//若父层ab无值，则为第一次且该层同父分支已完，应赋值
			tmp_ab[num - 1] = ab[num - 1] = tt;
		}
		//否则，应该该层的父节点不是长子，则应比较父层的tmp_ab
		else{
			if((num - 1)% 2){//奇数层取最大
				if(tmp_ab[num - 1] < tt){
					ab[num - 1] = tmp_ab[num - 1] = tt;
					if(num == 2){
						data_flag = 1;
					}	
				}
			}
			else{
				if(tmp_ab[num - 1] > tt){
					ab[num - 1] = tmp_ab[num - 1] = tt;
				}
			}
		}

		return ;
	}
	int data = 0;
	//先找可落子点，一般情况下不会没有，若真没有则return
	while((data < WIDTH)&&(!(check_here(tmp_pad,data)))){
		data++;
	}
	if(data < WIDTH){
		//创建子点
		//创建本节点
		cnt++;
		chd++;
		//传入父节 点棋局
		int row = 0,col = 0;
		int tmp_pp[W][W];
		for(row = 0;row < W;row++){
			for(col = 0;col < W;col++){
				tmp_pp[row][col] = tmp_pad[row][col];
			}
		}
		if(num % 2){
			trans_data(tmp_pp,data,1);
		}
		else{
			trans_data(tmp_pp,data,2);
		}
		//最末层
		if(num == DEEP){
			//肯定是长子点,因为其他都是sybling创建
			tmp_ab[num] = node_weight(tmp_pp);
			
		}
		else{
			ab[num] = 1000000;
		}
		if(1 == num){
			vdata = data;
		}
		int frw = 0;
		//试图创建子节点
		tree_creat_child(tmp_pp,&frw,num+1);
		if(1 == data_flag){
			vdata = data;
			data_flag = 0;
		}
		//试图创建弟节点
		tree_creat_sybling(tmp_pad,pw,frw,num,data+1);
	}
	else{
		/*********************************
		  预留：棋盘铺满情况该如何
		  *******************************/
		return ;
	}

}
//树的有界创建—弟节点
void tree_creat_sybling(int tmp_pad[][W],int *pw,int w,int num,int data){
	//剪枝
	//如果满足剪枝条件，则弟节点创建结束
	int cut_flag = 0;
	if(ab[num-1] != 1000000){
			if(num % 2){
				if(w > ab[num-1]){
					cut_flag = 1;
				}
			}
			else{
				if(w < ab[num-1]){
					cut_flag = 1;
				}
			}
	}
	if(cut_flag){
		//宽度到达-指向父节点-返回
		//最小弟节点指向父节点
		*pw = tmp_ab[num];
#if 0
		if(ab[num - 1] == 32000){
			//若父层ab无值，则为第一次或该层同父分支已完，应赋值
			tmp_ab[num - 1] = ab[num - 1] = tmp_ab[num];
			//同理，如果该层返回，则应是该层同父分支已完，应清值
			ab[num] = 32000;
		}
		//否则，应该该层的父节点不是长子，则应比较父层的tmp_ab
		else{
#endif
			if((num - 1) % 2){//奇数层取最大
				if(tmp_ab[num - 1] < tmp_ab[num]){
					ab[num - 1] = tmp_ab[num - 1] = tmp_ab[num];
					if(2 == num){
						data_flag = 1;
					}
				}	
			}
			else{
				if(tmp_ab[num - 1] > tmp_ab[num]){
					ab[num - 1] = tmp_ab[num - 1] = tmp_ab[num];
				}
			}
		//}
		return ;
	}

	//先找可落子点
	while((data < WIDTH)&&(!(check_here(tmp_pad,data)))){
		data++;
	}
	//如果宽度到达 弟节点创建结束
	if(data >= WIDTH){
		//宽度到达-指向父节点-返回
		//最小弟节点指向父节点
		if(ab[num - 1] == 1000000){
			//若父层ab无值，则为第一次以及本层同父兄弟已完，应赋值
			tmp_ab[num - 1] = ab[num - 1] = tmp_ab[num];
			//同理，如果该层返回，则应是该层同父分支已完，应清值
			ab[num] = 1000000;
		}
		//否则，应该该层的父节点不是长子，则应比较父层的tmp_ab
		else{
			if((num - 1) % 2){//奇数层取最大
				if(tmp_ab[num - 1] < tmp_ab[num]){
					ab[num - 1] = tmp_ab[num - 1] = tmp_ab[num];
					if(2 == num){
						data_flag = 1;
					}
				}	
			}
			else{
				if(tmp_ab[num - 1] > tmp_ab[num]){
					ab[num - 1] = tmp_ab[num - 1] = tmp_ab[num];
				}
			}
		}
		return ;
	}
	//创建弟点
	//创建本节点
	cnt++;
	bro++;
	//传入父节 点棋局
	int row = 0,col = 0;
	int tmp_pp[W][W];
	for(row = 0;row < W;row++){
		for(col = 0;col < W;col++){
			tmp_pp[row][col] = tmp_pad[row][col];
		}
	}
	if(num % 2){
		trans_data(tmp_pp,data,1);
	}
	else{
		trans_data(tmp_pp,data,2);
	}
	if(num == DEEP){//若是最末层
		int tt = node_weight(tmp_pp);

		if(!(num % 2)){//若是min层 偶数取最小
			if(tt < tmp_ab[num]){
				tmp_ab[num] = tt;
			}
		}
		else{
			if(tt > tmp_ab[num]){
				tmp_ab[num] = tt;
			}
		}
		//试图创建弟节点
		tree_creat_sybling(tmp_pad,pw,tt,num,data+1);
	}
	else{

		//作为第一点创建一个分支
		int frw = 0;
		//递归调用创建弟节点
		//试图创建子节点
		tree_creat_child(tmp_pp,&frw,num+1);
		if(1 == data_flag){
			vdata = data;
			data_flag = 0;
		}
		//试图创建弟节点
		tree_creat_sybling(tmp_pad,pw,frw,num,data+1);
	}
}
int check_lose(int tmp_pad[][W]){
	//行判断
	ushort i = 0;
	ushort u = 31;
	ushort l = 1;
	ushort j = 0;
	for(i = 0;i < W;i++){
		for(j = 0;j < W-4;j++){
			if(tmp_pad[i][j]==1\
		    && tmp_pad[i][j+1]==1 \
			&& tmp_pad[i][j+2]==1\
		    && tmp_pad[i][j+3]==1 \
			&& tmp_pad[i][j+4]==1){
				return 1;
			}
			if(tmp_pad[i][j]==2 \
			&& tmp_pad[i][j+1]==2 \
			&& tmp_pad[i][j+2]==2\
			&& tmp_pad[i][j+3]==2 \
			&& tmp_pad[i][j+4]==2){
				return -1;
			}
		}
	}
	//列判断
	for(i = 0;i < W-4;i++){
		for(j = 0;j < W;j++){
			if(tmp_pad[i][j]==1 \
			&& tmp_pad[i+1][j]==1 \
			&& tmp_pad[i+2][j]==1\
			&& tmp_pad[i+3][j]==1 \
			&& tmp_pad[i+4][j]==1){
				return 1;
			}
			if(tmp_pad[i][j]==2 \
			&& tmp_pad[i+1][j]==2 \
			&& tmp_pad[i+2][j]==2\
			&& tmp_pad[i+3][j]==2 \
			&& tmp_pad[i+4][j]==2){
				return -1;
			}
		}
	}
	//左斜
	for(i = 4;i < W;i++){
		for(j = 0;j < W-4;j++){
			if(tmp_pad[i][j]==1 \
			&& tmp_pad[i-1][j+1]==1 \
			&& tmp_pad[i-2][j+2]==1 \
			&& tmp_pad[i-3][j+3]==1\
			&& tmp_pad[i-4][j+4]==1){
				return 1;
			}
			if(tmp_pad[i][j]==2 \
			&& tmp_pad[i-1][j+1]==2 \
			&& tmp_pad[i-2][j+2]==2 \
			&& tmp_pad[i-3][j+3]==2\
			&& tmp_pad[i-4][j+4]==2){
				return -1;
			}
		}
	}
	//右斜
	for(i = 4;i < W;i++){
		for(j = 4;j < W;j++){
			if(tmp_pad[i][j]==1 \
			&& tmp_pad[i-1][j-1]==1 \
			&& tmp_pad[i-2][j-2]==1 \
			&& tmp_pad[i-3][j-3]==1\
			&& tmp_pad[i-4][j-4]==1){
				return 1;
			}
			if(tmp_pad[i][j]==2 \
			&& tmp_pad[i-1][j-1]==2 \
			&& tmp_pad[i-2][j-2]==2 \
			&& tmp_pad[i-3][j-3]==2\
			&& tmp_pad[i-4][j-4]==2){
				return -1;
			}
		}
	}
	return 0;
}
int node_weight(int tmp_pad[][W]){
	//行判断
	ushort i = 0;
	ushort j = 0;
	ushort k = 0;
	int max = 0,min = 0;
	//定义max和min的权值计算数组
	//aa为max  bb为min
	for(i = 0;i < W;i++){
		for(j = 0;j < W-5;j++){
			for(k = 0;k < 32;k++){
				if(tmp_pad[i][j]==cmp_a[k][0] \
				&& tmp_pad[i][j+1]==cmp_a[k][1] \
			    && tmp_pad[i][j+2]==cmp_a[k][2] \
				&& tmp_pad[i][j+3]==cmp_a[k][3] \
				&& tmp_pad[i][j+4]==cmp_a[k][4] \
				&& tmp_pad[i][j+5]==cmp_a[k][5]){
					max += score[k];
					continue;
				}
				if(tmp_pad[i][j]==cmp_b[k][0] \
				&& tmp_pad[i][j+1]==cmp_b[k][1]\
				&& tmp_pad[i][j+2]==cmp_b[k][2] \
				&& tmp_pad[i][j+3]==cmp_b[k][3]\
			    && tmp_pad[i][j+4]==cmp_b[k][4] \
				&& tmp_pad[i][j+5]==cmp_b[k][5]){
					min += score[k];
					continue;
				}
			}
		}
	}
	//列判断
	for(i = 0;i < W-5;i++){
		for(j = 0;j < W;j++){
			for(k = 0;k < 32;k++){
				if(tmp_pad[i][j]==cmp_a[k][0]\
				&& tmp_pad[i+1][j]==cmp_a[k][1]\
				&& tmp_pad[i+2][j]==cmp_a[k][2]\
				&& tmp_pad[i+3][j]==cmp_a[k][3]\
				&& tmp_pad[i+4][j]==cmp_a[k][4]\
				&& tmp_pad[i+5][j]==cmp_a[k][5]){
					max += score[k];
					continue;
				}
				if(tmp_pad[i][j]==cmp_b[k][0]\
				&& tmp_pad[i+1][j]==cmp_b[k][1]\
				&& tmp_pad[i+2][j]==cmp_b[k][2]\
				&& tmp_pad[i+3][j]==cmp_b[k][3]\
				&& tmp_pad[i+4][j]==cmp_b[k][4]\
				&& tmp_pad[i+5][j]==cmp_b[k][5]){
					min += score[k];
					continue;
				}
			}
		}
	}
	//左斜
	for(i = 5;i < W;i++){
		for(j = 0;j < W-5;j++){
			for(k = 0;k < 32;k++){
				if(tmp_pad[i][j]==cmp_a[k][0]\
		    	&& tmp_pad[i-1][j+1]==cmp_a[k][1]\
		   		&& tmp_pad[i-2][j+2]==cmp_a[k][2]\
		   		&& tmp_pad[i-3][j+3]==cmp_a[k][3]\
		    	&& tmp_pad[i-4][j+4]==cmp_a[k][4]\
				&& tmp_pad[i-5][j+5]==cmp_a[k][5]){
					max += score[k];
					continue;
				}
				if(tmp_pad[i][j]==cmp_b[k][0]\
		    	&& tmp_pad[i-1][j+1]==cmp_b[k][1]\
		   		&& tmp_pad[i-2][j+2]==cmp_b[k][2]\
		   		&& tmp_pad[i-3][j+3]==cmp_b[k][3]\
		    	&& tmp_pad[i-4][j+4]==cmp_b[k][4]\
				&& tmp_pad[i-5][j+5]==cmp_b[k][5]){
					min += score[k];
					continue;
				}
			}
		}
	}
	//右斜
	for(i = 5;i < W;i++){
		for(j = 5;j < W;j++){
			for(k = 0;k < 32;k++){
				if(tmp_pad[i][j]==cmp_a[k][0]\
		    	&& tmp_pad[i-1][j-1]==cmp_a[k][1]\
		   		&& tmp_pad[i-2][j-2]==cmp_a[k][2]\
		   		&& tmp_pad[i-3][j-3]==cmp_a[k][3]\
		    	&& tmp_pad[i-4][j-4]==cmp_a[k][4]\
				&& tmp_pad[i-5][j-5]==cmp_a[k][5]){
					max += score[k];
					continue;
				}
				if(tmp_pad[i][j]==cmp_b[k][0]\
		    	&& tmp_pad[i-1][j-1]==cmp_b[k][1]\
		   		&& tmp_pad[i-2][j-2]==cmp_b[k][2]\
		   		&& tmp_pad[i-3][j-3]==cmp_b[k][3]\
		    	&& tmp_pad[i-4][j-4]==cmp_b[k][4]\
				&& tmp_pad[i-5][j-5]==cmp_b[k][5]){
					min += score[k];
					continue;
				}
			}
		}
	}
	
	return max - min;
}
void trans_data(int tmp_pad[][W],ushort data,int num){
	ushort i = data / W;
	ushort j = data % W;
	tmp_pad[i][j] = num;
}
int check_here(int tmp_pad[][W],int data){
	ushort i = data / W;	
	ushort j = data % W;
	if(tmp_pad[i][j]){
		return 0;
	}
	return 1;
}
int get_data(int pad[][W]){

	//树的初始化
	tree_init(pad);
	//创建要调用creat_child
	printf("树创建完成\n");
	printf("共创建了%d个节点\n",cnt);	
	printf("共创建了%d个子节点\n",chd);	
	printf("共创建了%d个兄节点\n",bro);	

	return vdata;
}
