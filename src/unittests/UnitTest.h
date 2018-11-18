/*
 * UnitTest.h
 *
 *  Created on: Aug 23, 2017
 *      Author: demo
 */

#ifndef UNITTESTS_UNITTEST_H_
#define UNITTESTS_UNITTEST_H_
#include <vector>
#include<string>
using namespace std;
#include<iostream>
#include "Status.h"
#include<map>
#include "Status.h"
#include <stdio.h>
typedef bool (*callback_function)(void); // type for conciseness
class UnitTest {
	int longestname;
	string unitTestName;
	map<string, callback_function> tests;
	map<string, bool> results;
	bool isTestExists(string testname);
	void printSpace(int len);
public:

	UnitTest(string unitTestName);
	virtual ~UnitTest();
	bool AddTest(string testname, callback_function func);
	bool RunTest(string testname);
	Status* RunAllTests(bool printToScreen);

};

#endif /* UNITTESTS_UNITTEST_H_ */
