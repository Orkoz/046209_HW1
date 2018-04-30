#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <signal.h>
#include<iostream>
#include <string>
#include <list>
#include <time.h>
#include "job.h"
#include "commands.h"

using namespace std;



#endif

void catch_ctrl_z(int sig_id);
void catch_ctrl_c(int sig_id);
int send_signal(pid_t pid, int sig_id);
string get_sig_name(int sig_id);


