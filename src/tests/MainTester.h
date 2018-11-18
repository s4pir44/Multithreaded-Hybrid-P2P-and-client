/*
 * MainTester.h
 *
 *  Created on: Aug 24, 2017
 *      Author: demo
 */

#ifndef TESTS_MAINTESTER_H_
#define TESTS_MAINTESTER_H_
#include <iostream>
using namespace std;
#include <unistd.h>
#include<csignal>
#include "../utils/ServerIO.h"
#include "../server/UDPServer.h"
#include "../server/UDPClient.h"
#include "../sockets/TCPServer.h"
#include "../sockets/TCPClient.h"
#include "../sockets/TCPMessengerProtocol.h"
#include "../centralDirectory/Users.h"
#include "../tcp/ClientTCP.h"
#include "../tcp/ServerTCP.h"
#include "../client/UDPClientGame.h"

#include "../server/GameServer.h"

#include <fstream>
#include "../unittests/UnitTest.h"
#include "../unittests/Assert.h"
#include "../timers/Timer.h"
#include "../tcp/IHandler.h"
#include "../tcp/ITCPServerHandler.h"
#include <sstream>
class MainTester {

	static void deleteFile(string filename);

	// DB TCP Server
	/* Users */
	static bool login();
	static bool registerUsers();
	static bool getUsers();
	/* Accept Always */
	static bool acceptAlways();
	/* Reject Always */
	static bool rejectAlways();
	/* Game */
	static bool gameRequests();

	static void dismissAll(ITCPServerHandler * handler, ServerTCP *server, Socket *client);

	static bool gameTester();
public:
	static void runAllTests();
	template<class T>
	static bool assertTrue(T arg1, T arg2);

	template<class T>
	static bool assertFalse(T arg1, T arg2);

	static void ResetInitialStateDB(); // state of current db


};

#endif /* TESTS_MAINTESTER_H_ */
