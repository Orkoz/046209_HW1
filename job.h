/*
 * job.h
 *
 *  Created on: Apr 26, 2018
 *      Author: os
 */

#ifndef JOB_H_
#define JOB_H_
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <list>
#include <time.h>

using namespace std;

class job {
public:

	job(string name, int pid, bool stopped);
	job(const job &src_job);
	string getName();
	int getPID();
	int getTime();
	bool isStopped();
	void stopJob();
	void continueJob();



private:
	string name_;
    int pid_;
    time_t time_;
    bool stopped_;
};


#endif /* JOB_H_ */
