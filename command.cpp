//		commands.cpp
//********************************************
#include "commands.h"

char pwd[MAX_PATH_SIZE];
char pwd_pre[MAX_PATH_SIZE];
list<string> history;
	
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string, background command flag
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString, bool background_flag)
{
	char* cmd;
	char* args[MAX_ARG+3]; // 3 for complicated command 'csh -f -c'];
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
	AddToHistory(lineSize)

	/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{
		if(num_arg != 0)
		{
			illegal_cmd = TRUE;
		}
		else if (getcwd(pwd, MAX_LINE_SIZE)==NULL)
		{
			cerr << endl; // print error messge TODO
			return 1;
		}
		else
		{
			cout << pwd << endl;
			return 0;
		}
	
	}
	/*************************************************/
	if (!strcmp(cmd, "cd") )
	{
		if(num_arg != 1)
		{
			illegal_cmd = TRUE;
		}
		
		else if ( !strcmp(args[1],"-") )
		{
			if (!pwd_pre || chdir(pwd_pre)) // if pwd_pre==NULL or path not found
			{
				cerr << "smash error: > " << args[1] << " - path not found" << endl;
				return 1;
			}
			char temp[MAX_PATH_SIZE];
			temp = pwd;
			pwd = pwd_pre;
			pwd_pre = temp;
			cout << pwd << endl;
			return 0;
		}
		else 
		{	
			if (chdir(args[1]) // path not coorect 
			{
				cerr << "smash error: > " << args[1] << " - path not found " << endl;
				return 1;
			}
			pwd_pre = pwd;
			pwd = args[1];
			return 0;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
		if(num_arg != 0)
		{
			illegal_cmd = TRUE;
		}
		else if(!history.empty)
		{
			list<string>::iterator its;
			for (its = history.begin(); its != history.end(); its++)
			{
				cout << *its << endl;
			}
			return 0;
		}
		else
		{
			return 0;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs"))
	{
		if(num_arg != 0)
		{
			illegal_cmd = TRUE;
		}
		else if(!jobs.empty)
		{
			int i;
			for (i=0; i < jobs.size; i++)
			{
				cout << [i] << " " << jobs[i].PrintJob << endl; // TODO PrintJob func
			}
			return 0;
		}
		else
		{
			return 0;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		if(num_arg != 2)
		{
			illegal_cmd = TRUE;
		}
		else if()
		{
			
		}
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
		cerr << "smash error: > " << lineSize << endl;
		return 1;
	}
    return 0;
}

//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string, jobs list, bool background_flag
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG+3], char* cmdString, void* jobs, bool background_flag)// 3 for complicated command 'csh -f -c'];
{
	int pID;
    	switch(pID = fork())
	{
    		case -1:
					// Add your code here (error)
					cerr << endl; // print ereor TODO
					
        	case 0 :
                	// Child Process
               		setpgrp();
			        // Add your code here (execute an external command)
					execvp(args[0],args) 
					cerr << endl; // how print ereor TODO
					// sanding kill signal TODO
					
			default:
                	// Add your code here

					if(background_flag) // if command is in background, insert the command to jobs
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
		strcpy (ExtCmd, "csh -f -c "); // TODO check return?
		strcat (ExtCmd, &lineSize); // TODO check return?

    	return ExtCmd;
	}
	return lineSize;
}
//**************************************************************************************
// function name: BgCmd
// Description: check if the command are background command
// Parameters: command string
// Returns: background_flag: TRUE if it's background command or FALSE otherwize
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
//**************************************************************************************
// function name: AddToHistory
// Description: add commend to history list. if it's allredy full (50) delete the first one first
// Parameters: command string
// Returns: void
//**************************************************************************************
void AddToHistory(char* lineSize)
	{
		if(history.size == MAX_HISTORY_SIZE)
		{
			history.pop_front;
		}
		history.push_back(lineSize);	
	}



