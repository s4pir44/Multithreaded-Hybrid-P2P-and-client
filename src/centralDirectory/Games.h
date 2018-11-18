/*
 * Games.h
 *
 *  Created on: Aug 26, 2017
 *      Author: demo
 */

#ifndef CENTRALDIRECTORY_GAMES_H_
#define CENTRALDIRECTORY_GAMES_H_
#include <map>
#include <iostream>
#include <strings.h>
#include <vector>
#include <fstream>
#include "../sockets/Strings.h"
#include "../sockets/TCPMessengerProtocol.h"
#include "../utils/ServerIO.h"
using namespace std;
#include "Score.h"

class Games {
	static map<string, Score*>* scores;
	static map<string, Score*>* getAll();
public:
	static bool load();
	static bool addScore(string name, int type); // and saves score to db
	static bool addUser(string name);
	static bool isUserExists(string name);
	static bool saveScores();
	static string getScores();
};

#endif /* CENTRALDIRECTORY_GAMES_H_ */
