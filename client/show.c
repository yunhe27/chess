#include "show.h"
int move(char c,int *loc){
	switch(c){
		case 'w':
			if(*loc >= 15){
				*loc -= 15;
			}
			else {
				;
				//*loc = *loc + 225 - 15;
			}
			break;
		case 's':
			if(*loc <= 209){
				*loc += 15;
			}
			else{
				;
				//*loc = *loc + 15 - 225;
			}
			break;
		case 'a':
			if(*loc % 15  != 0){
				*loc -= 1;
			}
			else{
				;
				//*loc += 14;
			}
			break;
		case 'd':
			if(*loc % 15 != 14){
				*loc += 1;
			}
			else{
				;
				//*loc -= 14;
			}
			break;
		default:
			break;
	}
}
void print_a(int a[][15]){
	int i = 0,j = 0;
	printf("                                              \n");
	for(i = 0;i < 15;i++){
		printf("  ");
		for(j = 0;j < 15;j++){
			if(a[i][j] == 0){
				printf("\033[47;30m十\033[0m");
			}
			else if(a[i][j] == 1){
				printf("\033[47;31m〇\033[0m");
			} 
			else if(a[i][j] == 2){
				printf("\033[47;36m〇\033[0m");
			} 
		}
		printf("\n");
	}
}
void print_b(int a[][15],int row,int col){
	int i = 0,j = 0;
	printf("                                              \n");
	for(i = 0;i < 15;i++){
		printf("  ");
		for(j = 0;j < 15;j++){
			if(a[i][j] == 0){
				if(row == i && col == j){
					printf("\033[40;37m十\033[0m");
				}
				else{
					printf("\033[47;30m十\033[0m");
				}
			}
			else if(a[i][j] == 1){
				if(row == i && col == j){
					printf("\033[41;37m十\033[0m");
				}
				else{
					printf("\033[47;31m〇\033[0m");
				}
			} 
			else if(a[i][j] == 2){
				if(row == i && col == j){
					printf("\033[46;37m十\033[0m");
				}
				else{
					printf("\033[47;36m〇\033[0m");
				}
			} 
		}
		printf("\n");
	}
}
int show_up(int a[][15]){
	int loc = 112,row = 0,col = 0;
	char cc = 0;
	while(1){
		while((cc = getchar())!= '\n'){
			system("clear");
			print_a(a);
			printf("\n  请落子\n");
			move(cc,&loc);
			row = loc / 15;
			col = loc % 15;
			system("clear");
			print_b(a,row,col);
			printf("\n  请落子\n");
			//move(c,%loc);
		}
		row = loc / 15;
		col = loc % 15;
		if(a[row][col] == 0){
			break;
		}
		else{
			printf("\n  此处已有棋子，请重新落棋.\n");
			sleep(1);
		}
	}

	return loc;
}
