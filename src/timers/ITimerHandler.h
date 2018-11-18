/*
 * ITimerHandler.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef TIMERS_ITIMERHANDLER_H_
#define TIMERS_ITIMERHANDLER_H_
#include<string>
using namespace std;


class ITimerHandler {
public:
	ITimerHandler(){};
	virtual ~ITimerHandler(){};

	virtual void handleMessages(int sig) = 0;
};



#endif /* TIMERS_ITIMERHANDLER_H_ */
