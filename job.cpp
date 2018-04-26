/*
 * job.cpp
 *
 *  Created on: Apr 26, 2018
 *      Author: os
 */

#include <job.h>

job::job(string name, int pid, int time):name_(name),pid_(pid),time_(time){}
string job::getName(){return name_;}
int job::getPID(){return pid_;}
int job::getTime(){return time_;}
