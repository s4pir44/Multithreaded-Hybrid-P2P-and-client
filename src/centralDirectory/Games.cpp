/*
 * Games.cpp
 *
 *  Created on: Aug 26, 2017
 *      Author: demo
 */

#include "Games.h"

#define FILE_NAME "scores"

map<string, Score*> * Games::scores = NULL; // clearing map
/**
 * Caches the file system calls
 */
map<string, Score*>* Games::getAll() {
	if (scores == NULL) {
		load();
	}
	return scores;
}
string Games::getScores()
{
	 vector<string> userScores;
	 for (std::map<string, Score*>::const_iterator it = getAll()->begin();
			 			it != getAll()->end(); it++)
	 {
			 string wins = ServerIO::FromIntToString(it->second->wins,0);
			 string losts = ServerIO::FromIntToString(it->second->loses,0);
			 string draws = ServerIO::FromIntToString(it->second->draws,0);
			 string saveAs = it->first + "="+wins+"&"+losts+"&"+draws;
			 userScores.push_back(saveAs);
	}

	 return ServerIO::FormatVector(userScores," ");
}
bool Games::load()
{
	map<string, Score*>* newScores = new map<string, Score*>();

	ifstream scoreFile;
	scoreFile.open(FILE_NAME);

	string name;
	string wins;
	string losts;
	string draws;
	string fullLine;
	while (scoreFile) {
		getline(scoreFile, fullLine); // user=wins&losts&draws
		if (fullLine == "") { // ignore empty lines
			continue;
		}
		vector<string> scoresString = Strings::split(fullLine, "=");
		name = scoresString.at(0);
		vector<string> scoresStringR = Strings::split(fullLine, "&");

		wins = scoresStringR.at(0);
		losts = scoresStringR.at(1);
		draws = scoresStringR.at(2);
		Score * score = new Score(name);
		score->addWin(ServerIO::FromStringToInt(wins));
		score->addLost(ServerIO::FromStringToInt(losts));
		score->addDraw(ServerIO::FromStringToInt(draws));

		(*newScores)[name] = score;
	}
// close users file
	scoreFile.close();

	map<string, Score*>* oldRes = scores;
	scores = newScores;

	if (oldRes != NULL) {
		delete oldRes;
	}

	return true;

}
bool Games::isUserExists(string name) {
	map<string, Score*>* all = getAll();
	return all->find(name) != all->end();
}

bool Games::saveScores()
{
	 remove(FILE_NAME);
	 ofstream scoresFile;

	 scoresFile.open(FILE_NAME, std::ios_base::app);
	 for (std::map<string, Score*>::const_iterator it = getAll()->begin();
	 			it != getAll()->end(); it++) {
		 string wins = ServerIO::FromIntToString(it->second->wins,0);
		 string losts = ServerIO::FromIntToString(it->second->loses,0);
		 string draws = ServerIO::FromIntToString(it->second->draws,0);
		 string saveAs = it->first + "="+wins+"&"+losts+"&"+draws;
		 scoresFile << saveAs<<endl;
	 }
	 scoresFile.close();
	 return true;
}


bool Games::addUser(string name)
{
		if (isUserExists(name)) {
			return false;
		}
		string user = name + "=0&0&0";

		ofstream users;

		users.open(FILE_NAME, std::ios_base::app);

		if (Games::getAll()->size() > 0)
		{
			users << endl << user;
		}
		else
		{
			users << user;
		}

		users.close();

		load();
		return true;
}

bool Games::addScore(string name, int type)
{
	getAll();

	if (!isUserExists(name))
	{
		addUser(name);
	}

	bool res = false;
	if (type == DRAW)
	{
		res = true;
		(*scores)[name]->addDraw(1);
	}
	else if (type == WIN)
	{
		res = true;
		(*scores)[name]->addWin(1);
	}
	else if (type == LOSE)
	{
		res = true;
		(*scores)[name]->addLost(1);
	}
	if (res)
	{
		saveScores();
	}

	return res;
}

