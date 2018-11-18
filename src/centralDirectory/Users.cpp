/*
 * Users.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#include "Users.h"
string hash_key = "thisismyhashedkey";

map<string, string>* Users::userToPassword = NULL; // clearing map

/**
 * Caches the file system calls
 */
map<string, string>* Users::getAll() {
	if (userToPassword == NULL) {
		load();
	}
	return userToPassword;
}

/**
 * Loads the text files to the map
 */
bool Users::load() {
	map<string, string>* newUserToPassword = new map<string, string>();

	ifstream users;
	users.open("users");

	string password;
	string name;
	string usernameAndPassword;

	while (users) {
		getline(users, usernameAndPassword); // user and password are seperated by $ username$encryptedpassowrd
		if (usernameAndPassword == "") {
			continue;
		}
		vector<string> usernamePassword = Strings::split(usernameAndPassword, "$");

		name = usernamePassword.at(0);
		password = usernameAndPassword.substr(name.size() + 1); // +1 because of $ sign

		(*newUserToPassword)[name] = password;
	}
	// close users file
	users.close();

	map<string, string>* oldUserToPassword = userToPassword;
	userToPassword = newUserToPassword;

	if (oldUserToPassword != NULL) {
		delete oldUserToPassword;
	}

	return true;
}

/**
 * Check if user exists
 */
bool Users::isUserExists(string name) {
	map<string, string>* all = getAll();
	return all->find(name) != all->end();
}

/**
 * Create a new user
 */
bool Users::registerUsers(string name, string password) {
	if (isUserExists(name)) {
		return false;
	}

	ofstream users;

	users.open("users", std::ios_base::app);

	char * pass = (char*) password.c_str();
	int len = password.size();
	char * key = (char*) hash_key.c_str();
	int keyLength = hash_key.size();
	for(int i = 0; i < len; i++) {
		pass[i] ^= key[i % keyLength];
	}
	password = pass;
	string user = name + "$" + password;
	if (Users::getAll()->size() > 0)
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

/**
 * Check if there is a match with a username and password
 */
bool Users::login(string name, string password) {
	if (!isUserExists(name)) {
		return false;
	}

	char * pass = (char*) password.c_str();
	int len = password.size();
	char * key = (char*) hash_key.c_str();
	int keyLength = hash_key.size();
	for(int i = 0; i < len; i++) {
		pass[i] ^= key[i % keyLength];
	}
	string Decryptedpassword = pass;
	return (Decryptedpassword == userToPassword->find(name)->second);
}

/**
 * Get all the users registered
 */
vector<string> Users::getAllNames() {
	vector<string> names;

	for (std::map<string, string>::const_iterator it = getAll()->begin();
			it != getAll()->end(); it++) {
		string key = it->first;
		names.push_back(key);
	}

	return names;
}

/**
 * Get all the users registered in a textual representation
 */
string Users::getAllUsers() {
	string result = "";

	vector<string> names = getAllNames();

	if (names.size() == 0) {
		return "There is no users.";
	}

	for (unsigned int i = 0; i < names.size(); i++) {
		if (result != "") {
			result += ",";
		}

		result += names[i];
	}

	return result;
}
