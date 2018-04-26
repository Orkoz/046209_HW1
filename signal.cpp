// signals.cpp
// contains signal handler functions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include <list>
using std::list;
extern list<job> jobs;
extern job latest_fg;
 job* jobs;


// Ctrl-Z sigmal handler
void catch_ctrl_z(int sig_num){

}




