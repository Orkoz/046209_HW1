#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <job.h>
#include "signals.h"
#include "commands.h"
#include <list>
using std::list;

#endif

void catch_ctrl_z(int sig_num);


