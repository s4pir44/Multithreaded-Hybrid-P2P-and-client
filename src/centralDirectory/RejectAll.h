/*
 * RejectAll.h
 *
 *  Created on: Aug 21, 2017
 *      Author: demo
 */

#ifndef CENTRALDIRECTORY_REJECTALL_H_
#define CENTRALDIRECTORY_REJECTALL_H_
#include<string>
#include "../sockets/Strings.h"
#include<vector>
#include<algorithm>
#include <strings.h>
#include <fstream>
using namespace std;

class RejectAll {
		static vector<string> * users;
		static vector<string> * getAll();
		/**
		 * Loads the text files to the map
		 */
		static bool load();
public:
		/**
			 * Check if user exists
			 */
			static bool isUserExists(string name);

			static bool AddNewUser(string name);

			static string getRejectedUsers();
};

#endif /* CENTRALDIRECTORY_REJECTALL_H_ */
