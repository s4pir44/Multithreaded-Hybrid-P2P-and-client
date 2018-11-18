/*
 * UDPClientGame.h
 *
 *  Created on: Aug 20, 2017
 *      Author: demo
 */

#ifndef CLIENT_UDPCLIENTGAME_H_
#define CLIENT_UDPCLIENTGAME_H_
#include "../threads/MThread.h"
#include "../sockets/UDPSocket.h"
#include "../sockets/TCPMessengerProtocol.h"
#include "../utils/ServerIO.h"
#include "../sockets/Types.h"
#include "../sockets/Strings.h"
#include "../server/UDPClient.h"
#include "../timers/Timer.h"
#include "../Menus.h"
#include <vector>
#include<string>
using namespace std;
#include<csignal>

class UDPClientGame : public MThread {
	string user;

	Timer * timer;

	string currentConversation;


	bool isAuth();
	bool isInDispatch();
	bool isInBroker();
	bool isWaitingForOppAddr();

	void PrintInstructionByStatus(int status);
	string getCommand(string com);
	string getData(string msg);
	bool isRunning;

	bool checkNumberOfCorrectArguments(string input, unsigned int argNum);
	bool isNumberInRange(string input, unsigned int start, unsigned int end);

public:
	bool inDebugMode;
	UDPClient * client;
	UDPClientGame(Timer * timer);
	virtual ~UDPClientGame();
	void timerHandler(int signal);
	bool sendMessageToMessengerServer(int command, string msg);
	bool sendMessageToGameServer(int command, string msg);
	bool isLoggedIn();
	bool isConnected();
	bool disconnect();
	bool getAllUsers();
	bool getAllConectedUsers();

	void connect(const string& command); // open port

	void run();
};

#endif /* CLIENT_UDPCLIENTGAME_H_ */
