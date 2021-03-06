/*************************************************
  **文 件 名：链式树型结构
  **描    述：基于父子兄弟关系的树型顺序链
  **作    者：xuyunhe
  **创始时间：2017-1-8
  **修 改 人：xuyunhe
  **修改时间：2017-1-12
  **修改内容：修改树销毁程序(用循环）
  **修改时间：2017-1-12
  **修改内容：销毁程序完成，尝试树型打印
  			  之前建弟节点错将幺子的弟指针与父相连，改正后销毁出错
  **修改时间：2017-1-12
  **修改内容：前问题解决，树型打印初步定型，优化打印界面
  **修改时间：2017-1-12
  **修改内容：打印优化放弃，应用于棋盘研究删枝与加层函数
  				1.删枝前先查找保留枝的头节点，保留的总是第2层的某个点
				2.加层总是一次加两层
  **修改时间：2017-1-13
  **修改内容：加棋局判断及权值 测试博弈树创建与权值正确性
  **修改时间：2017-1-14
  **修改内容：发现问题，层及宽过多时程序段错误，查出动态分配出错
  			分析认为内存太大超出空间，无法完成动态分配；
			暂搁置该问题，写剪枝函数，希望能避免该问题
  **修改内容：剪枝函数完成，暂时缓解了上问题，但在4层10宽时已接近临界点
  **修改内容：发现剪枝函数写错，修改权值打印，棋局赋初值，尝试重写剪枝
  **修改内容：边创建节点边计算权值功能似乎完成
  **修改内容：尝试剪枝算法
  **修改时间：2017-1-15
  **修改内容：尝试验证剪枝算法
  				
  **链式原则：	1.长子先死，长子长孙的最后一代长子决定
					其一系的存在
				2.变态杀法，凡是有后代的先捆绑，看着
					后代死光后再将其杀死
				3.亲近原则，父与长子亲近，兄与弟亲近
					幺子与父亲近
*************************************************/
//创建博弈链形树
#ifndef __TREE_H_
#define __TREE_H_

#include <stdio.h>
#include <stdlib.h>
#define     DEEP   2  //深度
#define      W     15   //棋盘大小 W * W
#define     WIDTH  W*W	//宽度
#define ushort  unsigned short

extern int cnt;//测试：统计创建节点数
extern int chd;//测试：统计调用创建子节点数
extern int bro;//测试：统计调用创建弟节点数
extern int rls;//测试：统计释放节点数
extern int ab[DEEP];
extern int tmp_ab[DEEP+1];
extern int vdata;
extern int data_flag;
extern int cmp_a[32][6];
extern int cmp_b[32][6];
extern int score[32];
extern ushort a[DEEP][W],b[DEEP][W];

/*****************************
  功能：机器计算
  参数：棋盘全局变量
  返回值：落子点 
*****************************/
int get_data(int pad[][W]);

/*****************************
  功能：在创建树时用于判断是否可落子
  参数：父节点的方块地址；要创建节点的data
  返回值：0  不可落子
  		  1   可落子
*****************************/
int check_here(int tmp_pad[][W],int );

/*****************************
  功能：向节点内a，b传棋局
  参数：圆圈里数组地址；要传入的点位
  返回值：void
*****************************/
void trans_data(int tmp_pad[][W],ushort data,int num);

//树的初始化函数
/*****************************
  功能：创建第一个圆圈节点
  参数：头方块的地址
  返回值：void
*****************************/
void tree_init(int (*)[W]);

/*****************************
  功能：创建后代一系列子节点
  参数：自己的父节点位置；
  		自己圆圈内的子方块地址；
		要创建的子的层数
  返回值：void
*****************************/
void tree_creat_child(int tmp_pad[][W],int *,int);

//树的有界创建—弟节点
/*****************************
  功能：创建自己同层弟节点
  参数：自己的父节点的位置；
  		自己圆圈内的弟方块地址；
		自己圆圈内的父方块地址（用于万一自己是幺子）；
		自己的层数；
		要创建的点的宽度（即棋盘位置）；
  返回值：void
*****************************/
void tree_creat_sybling(int tmp_pad[][W],int *,int,int,int);


// 判断棋局结果
/*****************************
  功能：
  参数：棋局数组地址正方
  		棋局数组地址反方
  返回值：-1  输
  		   0  无输赢
		   1  赢
*****************************/
int check_lose(int tmp_pad[][W]);

// 估算权值
/*****************************
  功能：
  参数：棋局数组地址正方
  		棋局数组地址反方
  返回值：权值
*****************************/
int node_weight(int tmp_pad[][W]);

#endif  // __TREE_H_

