// signals.cpp
// contains signal handler functions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */

// Ctrl-Z sigmal handler
void catch_ctrl_z(int sig_num){
	stop_job();
}




