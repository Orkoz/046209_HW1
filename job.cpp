/*
 * job.cpp
 * A model of a single job (process).
 * Defines functionד to manage a single job: write (stopJob,continueJob)
 * 		and read (getName,getPID,getTime,isStopped) its values.
 *
 */

#include "job.h"

//**************************************************************************************
// function name: job
// Description: empty constructor
// Parameters: None
// Returns: job object
//**************************************************************************************
job::job():name_(""),pid_(0),stopped_(false){time(&time_);}

//**************************************************************************************
// function name: job
// Description: constructor
// Parameters: name: the name of the job.
//             pid: job process pid.
//             stopped: the state of the job (process) - is it running or not.
// Returns: job object
//**************************************************************************************
job::job(string name, int pid, bool stopped):name_(name),pid_(pid),stopped_(stopped){time(&time_);}

//**************************************************************************************
// function name: job
// Description: copy constructor
// Parameters: src_job: the job that will be copied
// Returns: job object
//**************************************************************************************
job::job(const job &src_job){
	name_ = src_job.name_;
	pid_ = src_job.pid_;
	stopped_ = src_job.stopped_;
	time(&time_);
}

//**************************************************************************************
// function name: getName
// Description: return the job name
// Parameters: none.
// Returns: (string) job name
//**************************************************************************************
string job::getName(){return name_;}

//**************************************************************************************
// function name: getPID
// Description: return the job (process) pid
// Parameters: none.
// Returns: (int) job (process) pid
//**************************************************************************************
int job::getPID(){return pid_;}

//**************************************************************************************
// function name: getTime
// Description: return the time that passed since the job was inserted to jobs list
// Parameters: none.
// Returns: (double) the time that passed since the job was inserted to jobs list
//**************************************************************************************
double job::getTime(){return difftime(time(NULL),time_);}

//**************************************************************************************
// function name: isStopped
// Description: return if the job (process) is running or not
// Parameters: none.
// Returns: (boll) 1- the process is stopped, 0- the process is running.
//**************************************************************************************
bool job::isStopped(){return stopped_;}

//**************************************************************************************
// function name: stopJob
// Description: stops the job (process) from running.
// Parameters: none.
// Returns: none.
//**************************************************************************************
void job::stopJob(){stopped_ = true;}

//**************************************************************************************
// function name: continueJob
// Description: resuming the job (process) run.
// Parameters: none.
// Returns: none.
//**************************************************************************************
void job::continueJob(){stopped_ = false;}

