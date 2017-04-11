#include "key.h"

struct termios oldt;
void restore_terminal_settings(void){
	tcsetattr(0,TCSANOW,&oldt);	
}
void disable_terminal_return(void){
	struct termios newt;
	tcgetattr(0,&oldt);
	newt = oldt;
	newt.c_lflag &=~(ICANON|ECHO);
	newt.c_cc[VTIME] = 5;
	newt.c_cc[VMIN] = 0;
	tcsetattr(0,TCSANOW,&newt);
	atexit(restore_terminal_settings);
}
