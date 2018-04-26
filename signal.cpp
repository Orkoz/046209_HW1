// signals.cpp
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

// Ctrl-Z sigmal handler
void catch_ctrl_z(int sig_num){
	this.stop_job();
}




