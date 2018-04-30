//		commands.cpp
//********************************************
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
	RemoveFinishJob();

	/*************************************************/
	if (!strcmp(cmd, "pwd"))
	{
		if(num_arg != 0)
		{
			illegal_cmd = true;
		}
		else if (getcwd(pwd, MAX_LINE_SIZE)==NULL)
		{
			cerr << endl; // print error message TODO
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
			list<job>::iterator its;
			int i = 0;
			for (its = jobs.begin(); its != jobs.end(); its++)
			{
				cout << "[" << i << "] " << its->printJob() << endl;
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
			strtok(args[1], "-");
			args[1] = strtok(NULL, "-");
			if(!send_signal(args[2], args[1]))// sending signal fail TODO kill_raper func
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
		else if ( jobs.empty )
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
			}
		}
		if(!illegal_cmd)
		{
			cout << L_Fg_Cmd.name_ << endl;
			if(L_Fg_Cmd.stopped_)
			{
				L_Fg_Cmd.stopped_ = false;
				send_signal(L_Fg_Cmd.pid_, SIGCONT);
			}
			fgExcites = true;
			waitpid(L_Fg_Cmd.pid_, NULL, WUNTRACED); // WUNTRACED for stopped process
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
		else if ( jobs.empty )
		{
			cerr << "smash error: > bg - jobs list is empty" << endl;
			return 1;
		}	
		else if ( num_arg == 0) // no command number - bg for last background cmd
		{
			list<job>::iterator its;
			for(its = jobs.end(); its != jobs.begin(); its--)
			{
				if(*its->isStopped())
				{
					cout << its->getName() << endl;
//					rep_kill(*its->getPID, sigACTION); // TODO rep_kill func and signal sigACTION
					*its->isStopped() = false;
					return 0;
					break;
				}
				cerr << "smash error: > bg - all jobs list is running" << endl;
				return 1;
			}
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
				if(*its->isStopped())
				{
					cout << its->getName() << endl; // TODO change to char*
//					rep_kill(*its->getPID(), sigACTION); // TODO rep_kill func and signal sigACTION
					*its->isStopped() = false;
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
//				rep_kill(*its->getPID(), SIGTERM);  // TODO rep_kill func and signal SIGTERM
				waitpid(its->getPID(), &status, WUNTRACED);
				start_time = clock();
				end_time = 5000 + start_time; // 5 sec wait
				while(clock() <= end_time) // beasy wait?? TODO
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
//					rep_kill(*its->getPID() ,SIGKILL);  // TODO rep_kill func and signal SIGKILL
					cout << "Done." << endl; // TODO SIGKILL mast success
				}
			}
			its = jobs.begin();
			while (its)
			{
				jobs.erase(its);
				its++;
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
void ExeExternal(char* args[MAX_ARG+3], char* cmdString, bool background_flag)// 3 for complicated command 'csh -f -c'];
{
	int pID;
    	switch(pID = fork())
	{
    		case -1:
					cerr << endl; // print ereor TODO
					
        	case 0 :
                	// Child Process
               		setpgrp();
					execvp(args[0],args);
					cerr << endl; // how print ereor TODO
					// sanding kill signal TODO
					
			default:
					if(background_flag) // if command is in background, insert the command to jobs
					{
						job new_job = job(args[0], pID, false);
						new_job.stopped_ = true;
						new_job.time_ = time(NULL);
						jobs.push_back(new_job);
					}
					else
					{
						L_Fg_Cmd = job(args[0], pID, false);
						fgExcites = true;
						waitpid(pID, NULL, WUNTRACED); // WUNTRACED for stopped processe
						fgExcites = false;

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
		const char* temp =  &lineSize;
		strcpy (ExtCmd, "csh -f -c "); // TODO check return?
		strcat (ExtCmd, temp); // TODO check return?

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
bool BgCmd(char* lineSize)
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
void AddToHistory(char* lineSize)
	{
		if(history.size == MAX_HISTORY_SIZE)
		{
			history.pop_front;
		}
		history.push_back(lineSize);	
	}

void stop_job(){
	if (fgExcites){
		job new_job = job(L_Fg_Cmd);
		send_signal(new_job.getPID(), SIGTSTP);
		new_job.stopped_ = true;
		new_job.time_ = time(NULL);
		jobs.push_back(new_job);
	}
}

void kill_job(){
	if (fgExcites){
		send_signal(L_Fg_Cmd.getPID(), SIGINT);
	}
}

void RemoveFinishJob()
{
	int status;
	list<job>::iterator its;
	for (its = jobs.begin(); its != jobs.end(); its++)
	{
		waitpid(its->getPID(), &status, WUNTRACED);
		if (WIFSIGNALED(status) || (WIFEXITED(status))) // if killing success
		{
			jobs.erase(its);
		}
		if (jobs.size() == 0 || its == jobs.end()) //job list is empty after deletion / deleted last element
			break;
	}
}



