/*
 * job.h
 *
 *  Created on: Apr 26, 2018
 *      Author: os
 */

#ifndef JOB_H_
#define JOB_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>

using namespace std;

class job {
public:

	job(string name, int pid, int time,  bool stopped);
	string getName();
	int getPID();
	int getTime();
	bool job::isStopped();
	void stopJob();
	void continueJob();



private:
	string name_;
    int pid_;
    int time_;
    bool stopped_;
};


#endif /* JOB_H_ */
