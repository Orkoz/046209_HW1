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

char lineSize[MAX_LINE_SIZE+10]; // 10 for complicated command 'csh -f -c';

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE+10]; // 10 for complicated command 'csh -f -c';;
    bool background_flag;

	//signal declarations
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c

    /*struct sigaction act_c;
    sigset_t sigmask_c;
	sigfillset(&sigmask_c);
	act_c.sa_mask = sigmask_c;
	act_c.sa_handler = &catch_ctrl_c;
	act_c.sa_flags = 0;
	*/


    struct sigaction act_z;
	sigset_t sigmask_z;
	sigfillset(&sigmask_z);
	act_z.sa_mask = sigmask_z;
	act_z.sa_flags = 0;
	act_z.sa_handler = &catch_ctrl_z;

	if (sigaction(SIGTSTP, &act_z, NULL) == -1)
		cerr << "signal handler Ctrl-Z failed" << endl;
	/*
	if (sigaction(SIGINT, &act_c, NULL) == -1)
			cerr << "signal handler Ctrl-C failed" << endl;
			*/

    while (1)
    	{
	 	printf("smash > ");
		if(fgets(lineSize, MAX_LINE_SIZE, stdin))
		{
			if(ExeComp(lineSize)) // if complicated Command edit a "csh -f -c "
			{
				const char* temp = lineSize;
				strcpy (cmdString, "csh -f -c ");
				strcat (cmdString, temp);
			}
			else
			{
				strcpy(cmdString, lineSize);
			}

			cmdString[strlen(cmdString)-1]='\0';
			background_flag = BgCmd(lineSize); // check if it's background command
			if (background_flag) // if it's background command change '&' to '\0'
			{
				cmdString[strlen(cmdString)-2] = '\0';
			}

			// all commands
			ExeCmd(lineSize, cmdString, background_flag);
		}
		else
		{
			cout << "smash error: > fgets" << endl;
		}

		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}





