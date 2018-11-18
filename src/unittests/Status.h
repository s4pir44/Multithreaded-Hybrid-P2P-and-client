/*
 * Status.h
 *
 *  Created on: Aug 23, 2017
 *      Author: demo
 */

#ifndef UNITTESTS_STATUS_H_
#define UNITTESTS_STATUS_H_
#include <vector>
#include<string>
using namespace std;

class Status {
public:
	vector<string> failedTests;
public:
	Status() {}
	virtual ~Status(){}

	bool passedAllTests() {
		return failedTests.size() == 0;
	}
};

#endif /* UNITTESTS_STATUS_H_ */
