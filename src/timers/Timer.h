/*
 * Timer.h
 *
 *  Created on: Aug 24, 2017
 *      Author: demo
 */

#ifndef TIMERS_TIMER_H_
#define TIMERS_TIMER_H_
#include<string>
#include<iostream>
using namespace std;
#include "../threads/MThread.h"
#include<csignal>
#include<stdio.h>
#include <unistd.h>
#include "ITimerHandler.h"
class Timer : public MThread {
private:
	int sig;
	int stopAfterInSeconds;
	float minTimeToSleep;
	float endTraget;
	bool isRunning;
	float currentTime;
	void signSignal();
	void(*handler)(int);
public:
	// Stop and raise event after a certain time : Stopper
	Timer(int signal, void(*handler)(int), float secondsToSleep, float stopAfter);
	// timer : timer
	Timer(float secondsToSleep);
	virtual ~Timer();

	bool startTimer();
	bool stopTimer();
	float getTotalRunTime();
	void run();
};

#endif /* TIMERS_TIMER_H_ */
