/*
 * Timer.cpp
 *
 *  Created on: Aug 24, 2017
 *      Author: demo
 */

#include "Timer.h"
#define INIT_STATE -1


Timer::Timer(float secondsToSleep) {
	this->sig = INIT_STATE;
	this->currentTime = 0;
	this->endTraget = INIT_STATE;
	this->isRunning = false;
	this->minTimeToSleep = secondsToSleep;
	this->stopAfterInSeconds = INIT_STATE;
	this->handler = NULL;

}

Timer::Timer(int signal, void(*handler)(int), float secondsToSleep, float stopAfter)
{
	this->sig = signal;
	this->currentTime = 0;
	this->endTraget = stopAfter;
	this->isRunning = false;
	this->minTimeToSleep = secondsToSleep;
	this->stopAfterInSeconds = INIT_STATE;
	this->handler = handler;
	this->signSignal();
}

bool Timer::startTimer()
{
	if (this->isRunning)
	{
		// already running
		return false;
	}
	this->currentTime = 0;
	this->isRunning = true;
	this->start();
	return true;
}
void Timer::signSignal()
{
	if (this->sig != INIT_STATE && this->handler != NULL)
	{
		signal(this->sig, this->handler);
	}
}
bool Timer::stopTimer()
{
	if (!this->isRunning)
	{
		// already not running
		return false;
	}
	this->isRunning = false;
	//cout<<"Stpping...true "<<this->currentTime<<endl;
	return true;
}
float Timer::getTotalRunTime()
{
	return this->currentTime;
}

void Timer::run()
{
	while(this->isRunning)
	{
		if (this->sig != INIT_STATE)
		{
			// stopper mode
			if (this->currentTime >= this->endTraget)
			{
				try {
					//cout<<"[Stopper : ] = "<<this->currentTime<<endl;
					raise(this->sig);
				}
				catch(const char * e)
				{
					perror(e);
				}
				this->stopTimer();
				break;
			}
		}
		usleep(1000000 * this->minTimeToSleep);
		this->currentTime += this->minTimeToSleep;
	}
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

