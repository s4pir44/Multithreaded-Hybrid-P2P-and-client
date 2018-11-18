/*
 * Users.h
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#ifndef CENTRALDIRECTORY_USERS_H_
#define CENTRALDIRECTORY_USERS_H_
#include <map>
#include <iostream>
#include <strings.h>
#include <vector>
#include <fstream>
#include "../sockets/Strings.h"
using namespace std;


class Users {
private:
	static map<string, string>* userToPassword;
	/**
	 * Caches the file system calls
	 */
	static map<string, string>* getAll();
	static string has_key;
public:

	/**
	 * Loads the text files to the map
	 */
	static bool load();

	/**
	 * Check if user exists
	 */
	static bool isUserExists(string name);

	/**
	 * Create a new user
	 */
	static bool registerUsers(string name, string password);

	/**
	 * Check if there is a match with a username and password
	 */
	static bool login(string name, string password);

	/**
	 * Get all the users registered
	 */
	static vector<string> getAllNames();

	/**
	 * Get all the users registered in a textual representation
	 */
	static string getAllUsers();

};

#endif /* CENTRALDIRECTORY_USERS_H_ */
