#ifndef  __KEY_H_
#define __KEY_H_

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
extern struct termios oldt;
void restore_terminal_settings(void);
void disable_terminal_return(void);

#endif //__KEY_H_

