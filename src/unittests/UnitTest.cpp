/*
 * UnitTest.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: demo
 */

#include "UnitTest.h"

UnitTest::UnitTest(string unitname) {
	// TODO Auto-generated constructor stub
	this->unitTestName = unitname;
	this->longestname = 0;
}

UnitTest::~UnitTest() {
	// TODO Auto-generated destructor stub
}
bool UnitTest::isTestExists(string testname)
{
	map<string,bool>::iterator it;
	it = results.find(testname);
	return it != results.end();
}
bool UnitTest::AddTest(string testname, callback_function func)
{
	if(this->isTestExists(testname))
	{
		cout<<testname<<" already exists"<<endl;
		return false;
	}
	if ( this->longestname < (int) testname.size())
	{
		this->longestname = testname.size();
	}
	tests[testname] = func;
	results[testname] = false; // initial value
	return true;
}
bool UnitTest::RunTest(string testname)
{
	if (!this->isTestExists(testname))
	{
		cout<<testname;
		perror(" : Test does not exists");
		return false;
	}
	return this->tests[testname]();
}

#include <unistd.h>
Status* UnitTest::RunAllTests(bool printToScreen)
{
	Status * status = new Status();
	map<string,callback_function>::iterator it;
	int i = 1;
	cout<<"RunAllTests"<<endl;
	for (it=this->tests.begin(); it!=this->tests.end(); ++it)
	{
		cout<<"Running Test "<<(i++)<<". '"<<it->first<<"'";
		this->printSpace(this->longestname - it->first.size());
		cout<<endl;
		try {
			this->results[it->first] = this->RunTest(it->first);
			sleep(1);
		}
		catch(const char * e){
			perror(e);
		}
		if (!this->results[it->first])
		{
			status->failedTests.push_back(it->first);
		}
		if(printToScreen)
		{
			cout<<endl<<endl;
			if (this->results[it->first])
			{
				cout<<"************** ["<<it->first<<" - Passed ] **************"<<endl;
			}
			else
			{
				cout<<"************** ["<<it->first<<" - Failed ] **************"<<endl;
			}
		}
		cout<<endl;
		sleep(1);
	} // end for loop
	cout<<endl<<endl;
	if (status->passedAllTests())
	{
		cout<<"************** [Passed] ************** "<<endl<<endl;
	}
	else {
		int len = this->tests.size() - status->failedTests.size();
		cout<<"**************  [Failed] **************  in : "<<len<<" tests out of :"<<this->tests.size()<<endl<<endl;

	}
	return status;
}
void UnitTest::printSpace(int len)
{
	while(len > 0)
	{
		cout<<" ";
		len--;
	}
}
