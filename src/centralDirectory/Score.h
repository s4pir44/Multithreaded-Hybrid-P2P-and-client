/*
 * Score.h
 *
 *  Created on: Aug 26, 2017
 *      Author: demo
 */

#ifndef CENTRALDIRECTORY_SCORE_H_
#define CENTRALDIRECTORY_SCORE_H_
using namespace std;
#include<string>
class Score {
public:
	int draws;
	int wins;
	int loses;
	string name;
public:
	Score(string name);
	virtual ~Score();

	void addWin(int count); // count because of loader
	void addLost(int count);
	void addDraw(int count);
};

#endif /* CENTRALDIRECTORY_SCORE_H_ */
