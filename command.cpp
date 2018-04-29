//		commands.cpp
//********************************************
#include "commands.h"

static list<job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of string)
static char pwd[MAX_PATH_SIZE];
static char pwd_pre[MAX_PATH_SIZE];
static list<string> history;
static job fg_job;
	
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string, background command flag
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(string lineSize, string cmdString, bool background_flag)
{
	string cmd;
	string args[MAX_ARG+3]; // 3 for complicated command 'csh -f -c'];
	string delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
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
	AddToHistory(lineSize);

	/*************************************************/
	if (!strcmp(cmd, "pwd"))
	{
		if(num_arg != 0)
		{
			illegal_cmd = true;
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
	else if (!strcmp(cmd, "cd") )
	{
		if(num_arg != 1)
		{
			illegal_cmd = true;
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
			if (chdir(args[1])) // path not coorect
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
			illegal_cmd = true;
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
			illegal_cmd = true;
		}
		else if(!jobs.empty)
		{
			int i;
			for (i=0; i < jobs.size; i++)
			{
				cout << '[' << i << ']' << " " << jobs[i].printJob << endl; // TODO PrintJob func
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
			illegal_cmd = true;
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
	if (illegal_cmd == true)
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
void ExeExternal(string args[MAX_ARG+3], string cmdString, bool background_flag)// 3 for complicated command 'csh -f -c'];
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
					execvp(args[0],args);
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
						fg_job = new job(args[0], pID, false);
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
string ExeComp(string lineSize)
{
	
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here
		char ExtCmd[MAX_LINE_SIZE+10]; // 10 for complicated command 'csh -f -c'];
		strcpy (ExtCmd, "csh -f -c "); // TODO check return?
		strct (ExtCmd, &lineSize); // TODO check return?

    	return ExtCmd;
	}
	return lineSize;
}
//**************************************************************************************
// function name: BgCmd
// Description: check if the command are background command
// Parameters: command string
// Returns: background_flag: true if it's background command or false otherwize
//**************************************************************************************
bool BgCmd(string lineSize)
{
	bool background_flag = false;
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		// Add your code here (execute a in the background)
		background_flag = true;
	}
	return background_flag;

}
//**************************************************************************************
// function name: AddToHistory
// Description: add commend to history list. if it's allredy full (50) delete the first one first
// Parameters: command string
// Returns: void
//**************************************************************************************
void AddToHistory(string lineSize)
	{
		if(history.size == MAX_HISTORY_SIZE)
		{
			history.pop_front;
		}
		history.push_back(lineSize);	
	}

void stop_job(){
	job new_job(fg_job);
	new_job.suspended = true;
	new_job.sus_time = time(NULL);
	jobs.push_back(new_job);
	L_Fg_Cmd.pid = 0;
	GPID = -1;
	printf("\n");
}



