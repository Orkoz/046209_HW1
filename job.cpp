/*
 * job.cpp
 *
 *  Created on: Apr 26, 2018
 *      Author: os
 */

#include <job.h>

job::job(string name, int pid, int time, bool stopped=false):name_(name),pid_(pid),time_(time),stopped_(stopped){}
string job::getName(){return name_;}
int job::getPID(){return pid_;}
int job::getTime(){return time_;}
bool job::isStopped(){return stopped_;}
void job::stopJob(){stopped_ = true;}
void job::continueJob(){stopped_ = false;}
