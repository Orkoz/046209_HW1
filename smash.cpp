/*	smash.cpp
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <list>
#include "job.h"
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

using namespace std;

char lineSize[MAX_LINE_SIZE];

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE+10]; // 10 for complicated command 'csh -f -c';
    bool background_flag;

	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */

    struct sigaction act;
    act.sa_handler = &catch_ctrl_z;
    sigaction(SIGTSTP,&act,NULL);

	/************************************/
	// Init globals

    while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);
		cmdString = ExeComp(lineSize); // edit a complicated Command for easy execute if needed
		cmdString[strlen(cmdString)-1]='\0';
	 	background_flag = BgCmd(lineSize); // check if it's background command			
		if (background_flag) // if it's background command change '&' to '\0'
		{
			cmdString[strlen(cmdString)-2] = '\0';
		}
		
		// all commands
		ExeCmd(lineSize, cmdString, background_flag);

		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}





