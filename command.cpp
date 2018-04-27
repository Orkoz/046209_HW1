//		commands.cpp
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString, bool background_flag)
{
	char* cmd;
	char* args[MAX_ARG+3]; // 3 for complicated command 'csh -f -c'];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
   	args[0] = cmd;
	for (i=1; i<MAX_ARG+3; i++) // 3 for complicated command 'csh -f -c'];
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") )
	{

	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{

	}

	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{

	}
	/*************************************************/

	else if (!strcmp(cmd, "jobs"))
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{

	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, background_flag);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG+3], char* cmdString, void* jobs, bool background_flag)// 3 for complicated command 'csh -f -c'];
{
	int pID;
    	switch(pID = fork())
	{
    		case -1:
					// Add your code here (error)

					/*
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
			        // Add your code here (execute an external command)

					// enter to job list
					

			default:
                	// Add your code here

					if(background_flag) // if background processe
					{
						// add to father job list TODO
					}
					else
					{
						waitpid(pID, &state, WUNTRACED) // WUNTRACED for stopped processe
					}
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: edit executes complicated command to executes like normal command
// Parameters: command string
// Returns: same command string if not complicated or 'csh -f -c' addtion in the beggining of a complicated command
//**************************************************************************************
char* ExeComp(char* lineSize)
{
	
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here
		char ExtCmd[MAX_LINE_SIZE+10]; // 10 for complicated command 'csh -f -c'];
		strcpy (ExtCmd, "csh -f -c ");
		strcat (ExtCmd, &lineSize);

    	return ExtCmd;
	}
	return lineSize;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
bool BgCmd(char* lineSize)
{
	bool background_flag = FALSE;
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		// Add your code here (execute a in the background)
		background_flag = TRUE;
	}
	return background_flag;
}



