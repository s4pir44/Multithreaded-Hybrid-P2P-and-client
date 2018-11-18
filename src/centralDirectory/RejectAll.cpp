/*
 * RejectAll.cpp
 *
 *  Created on: Aug 21, 2017
 *      Author: demo
 */

#include "RejectAll.h"
#define REJECT_FILE "rejectAll"
vector<string>* RejectAll::users = NULL; // clearing map

/**
 * Caches the file system calls
 */
vector< string>* RejectAll::getAll() {
	if (users == NULL) {
		load();
	}
	return users;
}

/**
 * Loads the text files to the map
 */
bool RejectAll::load() {
	vector<string> * newUsers = new vector<string>();

	ifstream userfiles;
	userfiles.open(REJECT_FILE);
	string name;
	while (userfiles)
	{
		getline(userfiles, name);
		if (name == "") {
			continue;
		}

		newUsers->push_back(name);

	}
	// close users userFiles
	userfiles.close();

	vector<string>* oldUsers = users;
	users = newUsers;
	if (oldUsers != NULL)
	{
		delete(oldUsers);
	}

	return true;
}

/**
 * Check if user exists
 */
bool RejectAll::isUserExists(string name) {
	getAll();
	return std::find(users->begin(), users->end(), name) != users->end();
}
/**
 * Create a new user
 */
bool RejectAll::AddNewUser(string name) {
	getAll();
	if (isUserExists(name)) {
		return false;
	}

	ofstream users;

	users.open(REJECT_FILE, std::ios_base::app);

	if (RejectAll::getAll()->size() > 0)
	{
		users << endl << name;
	}
	else
	{
		users << name;
	}

	users.close();

	load();
	return true;
}

string RejectAll::getRejectedUsers() {
	getAll();
	string result = "";
	vector<string> *users =  getAll();
	if (users->size() == 0) {
		return "";
	}

	for (unsigned int i = 0; i < users->size(); i++) {
		if (result != "") {
			result += ",";
		}

		result += users->at(i);
	}

	return result;
}



