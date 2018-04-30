/*
 * job.cpp
 *
 *  Created on: Apr 26, 2018
 *      Author: os
 */

#include <job.h>

job::job(string name, int pid, bool stopped):name_(name),pid_(pid),stopped_(stopped){time(&time_);}
job::job(const job &src_job){
	name_ = src_job.getName();

}
string job::getName(){return name_;}
int job::getPID(){return pid_;}
int job::getTime(){return time_;}
bool job::isStopped(){return stopped_;}
void job::stopJob(){stopped_ = true;}
void job::continueJob(){stopped_ = false;}
string job::printJob(){
	cout << name_ << " " << pid_ << " " << time << " sec" << endl;
}
