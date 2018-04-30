
#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string>
#include <list>
#include "job.h"
#include "signals.h"


using namespace std;

#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_PATH_SIZE 250
#define MAX_HISTORY_SIZE 50


int ExeCmd(char* lineSize, char* cmdString, bool background_flag);
void ExeExternal(char* args[MAX_ARG+3], char* cmdString, bool background_flag);
char* ExeComp(char* lineSize, char* cmdString);
int BgCmd(char* lineSize, void* jobs);
void AddToHistory(char* lineSize);
void stop_job();
void kill_job();
void RemoveFinishJob();

#endif

