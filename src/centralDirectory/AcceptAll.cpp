/*
 * AcceptAll.cpp
 *
 *  Created on: Aug 21, 2017
 *      Author: demo
 */

#include "AcceptAll.h"
vector<string>* AcceptAll::users = NULL; // clearing map

/**
 * Caches the file system calls
 */
vector< string>* AcceptAll::getAll() {

	if (users == NULL) {
		load();
	}
	return users;
}

/**
 * Loads the text files to the map
 */
bool AcceptAll::load() {
	vector<string> * newUsers = new vector<string>();

	ifstream userfiles;
	userfiles.open("acceptAll");
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
bool AcceptAll::isUserExists(string name) {
	getAll();
	bool b = std::find(users->begin(), users->end(), name) != users->end();
	return b;
}
/**
 * Create a new user
 */
bool AcceptAll::AddNewUser(string name) {
	getAll();


	if (isUserExists(name)) {
		return false;
	}

	ofstream users;

	users.open("acceptAll", std::ios_base::app);

	if (AcceptAll::getAll()->size() > 0)
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

string AcceptAll::getAcceptUsers() {
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


