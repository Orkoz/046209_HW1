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
// Description: interprets and executes built-in commands
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
			if(chdir(args[1])) // path not correct
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
			list<string>::iterator its;
			for (its = history.begin(); its != history.end(); its++)
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
		else if(!args[2])
		{
			cerr << "smash error: > kill job – job does not exist" << endl;
			return 1;
		}
		else
		{
			strtok(args[1], "-");
			args[1] = strtok(NULL, "-");
			if(!kill_raper(args[2], args[1]))// sending signal fail TODO kill_raper func
			{
				cerr << "smash error: > kill job – cannot send signal" << endl;
				return 1;				
			}
			else
			{
				return 0;
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
		if(num_arg != 0)
		{
			illegal_cmd = true;
		}
		else
		{
			unsigned int smash_pid = getpid();
			cout << "smash pid is " << smash_pid << endl;
			return 0;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
		if(num_arg > 1)
		{
			illegal_cmd = true;
		}
		else if ( jobs.empty )
		{
			cerr << "smash error: > fg - jobs list is empty" << endl;
			return 1;
		}
		else if ( num_arg == 0) // no command number - fg for last background cmd
		{
			L_Fg_Cmd = jobs.back;
			jobs.pop_back;
			cout << L_Fg_Cmd.name << endl;
			if(L_Fg_Cmd.stopped)
			{
				L_Fg_Cmd.stopped = false;
				rep_kill(L_Fg_Cmd.pid, SIGCONT); // TODO rep_kill func and signal SIGCONT
			}
			waitpid(L_Fg_Cmd.pid, &state, WUNTRACED) // WUNTRACED for stopped processe
			L_Fg_Cmd = NULL; // TODO if work
			return 0;	
		}
		else // ( num_arg == 1) fg for job command number = args[1]
		{
			int cmd_num = atoi(args[1]);
			if(cmd_num > jobs.size)
			{
				cerr << "smash error: > fg - args[1] is exceed the jobs list size" << endl;
				return 1;
			}
			else
			{
				list<job>::iterator its = jobs.begin();
				std::advance(its, cmd_num-1);
				L_Fg_Cmd = *its;
				jobs.erase(its);
				if(L_Fg_Cmd.stopped)
				{
					L_Fg_Cmd.stopped = false;
					rep_kill(L_Fg_Cmd.pid, SIGCONT); // TODO rep_kill func and signal SIGCONT
				}
				cout << L_Fg_Cmd.name << endl;
				waitpid(L_Fg_Cmd.pid, &state, WUNTRACED) // WUNTRACED for stopped processe
				return 0;	
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{
		if(num_arg > 1)
		{
			illegal_cmd = true;
		}
		else if ( jobs.empty )
		{
			cerr << "smash error: > bg - jobs list is empty" << endl;
			return 1;
		}	
		else if ( num_arg == 0) // no command number - bg for last background cmd
		{
			list<job>::iterator its;
			for (its = jobs.end(); its != jobs.begin(); its--)
			}
				if(*its.stopped)
				{
					job L_Cmd_stopped = *its;
					break;
				}
				cerr << "smash error: > bg - all jobs list is running" << endl;
				return 1;
			}
			cout << L_Cmd_stopped.name << endl;
			L_Cmd_stopped.stopped = false;
			rep_kill(L_Cmd_stopped.pid, sigACTION); // TODO rep_kill func and signal sigACTION
			return 0;
		}	
		else // ( num_arg == 1) bg for job command number = args[1]
		{
			int cmd_num = atoi(args[1]);
			if(cmd_num > jobs.size)
			{
				cerr << "smash error: > bg - args[1] is exceed the jobs list size" << endl;
				return 1;
			}
			else
			{
				list<job>::iterator its = jobs.begin();
				std::advance(its, cmd_num-1);
				if(*its.stopped)
				{
					cout << *its.name << endl;
					*its.stopped = false;
					rep_kill(*its.pid, sigACTION); // TODO rep_kill func and signal sigACTION
					return 0;	
				}
				else
				{
					cerr << "smash error: > bg - job is allredy running" << endl;
					return 1;				
				}
			}	
		}	
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if(num_arg > 1)
		{
			illegal_cmd = true;
		}	
		else if ( num_arg == 0) // regular quit
		{
			exit(1);
		}
		else // ( num_arg == 1) quit kill
		{
			clock_t start_time;
			clock_t end_time;
			bool success;
			int status;
			list<job>::iterator its;
			for (its = jobs.begin(); its != jobs.end(); its++)
			{
				success = false;
				rep_kill(*its.pid, SIGTERM);  // TODO rep_kill func and signal SIGTERM
				waitpid(*its.pid, &state, WUNTRACED);
				start_time = clock();
				end_time = 5000 + start_time; // 5 sec wait
				while(clock() <= end_time)
				{
					if (WIFSIGNALED(status)) // if killing success
					{
						success = true;
						cout << "Done." << endl;
						break;
					}
				}
				if (!success)
				{
					rep_kill(*its.pid, SIGKILL);  // TODO rep_kill func and signal SIGKILL
					cout << "Done." << endl; // TODO SIGKILL mast success
				}
			}
			its = jobs.begin()
			while (its)
			{
				jobs.erase(its);
				its++
			}
			exit(1);
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "mv"))
	{
		if(num_arg != 2)
		{
			illegal_cmd = true;
		}	
		else if ( )
		{
			if(rename( args[1] , args[2] ))
			{
				cerr << "smash error: > mv - is failed" << endl;
				return 1;
			}
			cout << args[1] << " has been renamed to " << args[2] << endl;
			return 0;
		}

	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, background_flag);
	 	return 0;
	}
	/*************************************************/
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



