/*
 * Score.cpp
 *
 *  Created on: Aug 26, 2017
 *      Author: demo
 */

#include "Score.h"

Score::Score(string name) {
	// TODO Auto-generated constructor stub
	this->name = name;
	this->draws = 0;
	this->wins = 0;
	this->loses = 0;
}

void Score::addWin(int count){
	this->wins += count;
}

void Score::addLost(int count)
{
	this->loses+=count;
}

void Score::addDraw(int count)
{
	this->draws += count;
}
Score::~Score() {
	// TODO Auto-generated destructor stub
}

