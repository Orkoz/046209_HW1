/*
 * commands.cpp
 * The model handles the command executions and the management of jobs (process).
 * "ExeCmd" interprets and executes built-in commands and "ExeExternal" executes external command
 *
 */

#include "commands.h"


static list<job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of string)
static char pwd[MAX_PATH_SIZE];
static char pwd_pre[MAX_PATH_SIZE];
static list<string> history;
static job L_Fg_Cmd;
static bool fgExcites = false;

//********************************************
// function name: ExeCmd
// Description: interprets and executes built-in commands
// Parameters: pointer to jobs, command string, background command flag
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString, bool background_flag)
{
	char* cmd;
	char* args[MAX_ARG+3]; // 3 for complicated command 'csh -f -c'];
	const char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    	cmd = strtok(cmdString, delimiters);
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
//	RemoveFinishJob();

	/*************************************************/
	if (!strcmp(cmd, "pwd"))
	{
		if(num_arg != 0)
		{
			illegal_cmd = true;
		}
		else if (getcwd(pwd, MAX_LINE_SIZE)==NULL)
		{
			cerr << "smash error: > pwd - pwd not found" << endl; // print error message TODO
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
			if (pwd_pre==NULL || chdir(pwd_pre)) // if pwd_pre==NULL or change dir not success
			{
				cerr << "smash error: > " << args[1] << " - path not found" << endl;
				return 1;
			}
			char temp[MAX_PATH_SIZE];
			strcpy(temp,pwd);
			strcpy(pwd,pwd_pre);
			strcpy(pwd_pre,temp);
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
			strcpy(pwd_pre,pwd);
			strcpy(pwd,args[1]);
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
		else if(!history.empty())
		{
			list<string>::iterator its;
			for (its = history.begin(); its != history.end(); its++)
			{
				cout << *its;
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
		else if(!jobs.empty())
		{
			list<job>::iterator its;
			int i = 0;
			for (its = jobs.begin(); its != jobs.end(); its++)
			{
				cout << "[" << i << "] " << its->getName() << " " << its->getPID() << " " << its->getTime() << " sec";
				if(its->isStopped())
					cout << " (Stopped)" << endl;
				else
					cout << endl;
				i++;
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
			//strtok(args[1], "-");
			//args[1] = strtok(NULL, "-");
			if (args[1])
			if(args[1]==NULL || send_signal(atoi(args[2]), atoi(args[1])))
			{
				cerr << "smash error: > kill job – cannot send signal" << endl;
				return 1;				
			}
			return 0;
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
		else if ( jobs.empty() )
		{
			cerr << "smash error: > fg - jobs list is empty" << endl;
			return 1;
		}
		else if ( num_arg == 0) // no command number - fg for last background cmd
		{
			L_Fg_Cmd = jobs.back();
			jobs.pop_back();
		}
		else // ( num_arg == 1) fg for job command number = args[1]
		{
			unsigned int cmd_num = atoi(args[1]);
			if(cmd_num > jobs.size())
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
			}
		}
		if(!illegal_cmd)
		{
			cout << L_Fg_Cmd.getName() << endl;
			if(L_Fg_Cmd.isStopped())
			{
				if(send_signal(L_Fg_Cmd.getPID() , SIGCONT))
				{
					cerr << "smash error: > fg – cannot send signal" << endl;
					return 1;
				}
				L_Fg_Cmd.continueJob();
				send_signal(L_Fg_Cmd.getPID() , SIGCONT);
			}
			fgExcites = true;
			waitpid(L_Fg_Cmd.getPID(), NULL, WUNTRACED); // WUNTRACED for stopped process
			fgExcites = false;
			return 0;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{
		if(num_arg > 1)
		{
			illegal_cmd = true;
		}
		else if ( jobs.empty() )
		{
			cerr << "smash error: > bg - jobs list is empty" << endl;
			return 1;
		}	
		else if ( num_arg == 0) // no command number - bg for last background cmd
		{
			list<job>::iterator its;
			for(its = jobs.end(); its != jobs.begin(); its--)
			{
				if(its->isStopped())
				{
					if(send_signal(its->getPID(), SIGCONT))
					{
						cerr << "smash error: > bg – cannot send signal" << endl;
						return 1;
					}
					cout << its->getName() << endl;
					its->continueJob();
					return 0;
					break;
				}
				cerr << "smash error: > bg - all jobs list is running" << endl;
				return 1;
			}
		}	
		else // ( num_arg == 1) bg for job command number = args[1]
		{
			unsigned int cmd_num = atoi(args[1]);
			if(cmd_num > jobs.size())
			{
				cerr << "smash error: > bg - args[1] is exceed the jobs list size" << endl;
				return 1;
			}
			else
			{
				list<job>::iterator its = jobs.begin();
				std::advance(its, cmd_num-1);
				if(its->isStopped())
				{
					if(send_signal(its->getPID(), SIGCONT))
					{
						cerr << "smash error: > bg – cannot send signal" << endl;
						return 1;
					}
					cout << its->getName() << endl;
					its->continueJob();
					return 0;	
				}
				else
				{
					cerr << "smash error: > bg - job is already running" << endl;
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
				send_signal(its->getPID(), SIGTERM);
				waitpid(its->getPID(), &status, WNOHANG);
				start_time = clock()/CLOCKS_PER_SEC;
				end_time = 5 + start_time; // 5 sec wait
				while((clock()/CLOCKS_PER_SEC) <= end_time) // TODO check
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
					send_signal(its->getPID() ,SIGKILL); // SIGKILL mast success
					cout << "Done." << endl;
				}
				jobs.erase(its);
				if (jobs.size() == 0 || its == jobs.end())
					break;

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
		else if(rename( args[1] , args[2] ))
		{
			cerr << "smash error: > mv - is failed" << endl;
			return 1;
		}
		else
		{
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
		cerr << "smash error: > " << lineSize;
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
void ExeExternal(char* args[MAX_ARG+3], char* cmdString, bool background_flag)// 3 for complicated command 'csh -f -c'];
{
	int pID;
    	switch(pID = fork())
	{
    		case -1:
					cerr << endl; // print ereor TODO
					break;
					
        	case 0 :
                	// Child Process
               		setpgrp();
					execvp(args[0],args);
					cerr << "smash error: > execute - failed" << endl;
					send_signal(getpid(), SIGTERM);
					break;
					
			default:
					if(background_flag) // if command is in background, insert the command to jobs
					{
						job new_job(args[0], pID, false);
						jobs.push_back(new_job);
					}
					else
					{
						L_Fg_Cmd = job(args[0], pID, false);
						fgExcites = true;
						waitpid(pID, NULL, WUNTRACED); // WUNTRACED for stopped process
						fgExcites = false;

					}
					break;
	}
}

//**************************************************************************************
// function name: ExeComp
// Description: check if the command are complicated command
// Parameters: command string
// Returns: true if it's a complicated Command or false otherwise
//**************************************************************************************
bool ExeComp(char* lineSize)
{
	
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
       	return true;
	}
	return false;
}

//**************************************************************************************
// function name: BgCmd
// Description: check if the command are background command
// Parameters: command string
// Returns: background_flag: true if it's background command or false otherwise
//**************************************************************************************
bool BgCmd(char* lineSize)
{
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		return true;
	}
	return false;
}

//**************************************************************************************
// function name: AddToHistory
// Description: add commend to history list. if it's already full (50) delete the first one first
// Parameters: command string
// Returns: void
//**************************************************************************************
void AddToHistory(char* lineSize)
{
	if(history.size() == MAX_HISTORY_SIZE)
	{
		history.pop_front();
	}
	history.push_back(lineSize);
}

//**************************************************************************************
// function name: stop_job
// Description: stop the run of the foreground (if exists) job and add it to the jobs list.
// Parameters: none.
// Returns: void
//**************************************************************************************
void stop_job(){
	if (fgExcites){
		job new_job = job(L_Fg_Cmd);
		if(send_signal(new_job.getPID(), SIGTSTP))
		{
			cerr << "smash error: > Ctrl-Z - cannot send signal" << endl;
			return;
		}
		new_job.stopJob();
		jobs.push_back(new_job);
	}
}

//**************************************************************************************
// function name: kill_job
// Description: terminates the foreground job (if exists).
// Parameters: none.
// Returns: void
//**************************************************************************************
void kill_job(){
	if (fgExcites){
		if(send_signal(L_Fg_Cmd.getPID(), SIGINT))
		{
			cerr << "smash error: > Ctrl-Z - cannot send signal" << endl;
			return;
		}
		fgExcites = false;
	}
}

//**************************************************************************************
// function name: RemoveFinishJob
// Description: Remove finish job from job list
// Parameters: No arguments
// Returns: void
//**************************************************************************************
void RemoveFinishJob()
{
	list<job>::iterator its;
	for (its = jobs.begin(); its != jobs.end(); its++)
	{
		if (waitpid(its->getPID(), NULL, WNOHANG) != 0)
			jobs.erase(its);
		if (jobs.size() == 0 || its == jobs.end())
			break;
	}
}



