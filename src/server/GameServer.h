/*
 * GameServer.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef SERVER_GAMESERVER_H_
#define SERVER_GAMESERVER_H_
#include "../threads/MThread.h"
#include "../sockets/UDPSocket.h"
#include "../sockets/TCPMessengerProtocol.h"
#include "../utils/ServerIO.h"
#include "../sockets/Types.h"
#include "Broker.h"
#include "../gamexo/XOGame.h"
#include "IGameHandler.h"
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;



class GameServer :public MThread {
	map<string,string>addressesToUsers;
	map<string,string>addressedToGames;

	vector<XOGame*>games;
	//IGameHandler *handler;
	UDPSocket * serverSocket;
	bool isRunning;
	void handleMessages(int serverCommand, string serverMsg);
	XOGame* getGameByID(string gameID);
public:
	//GameServer(int port, IGameHandler * handler);
	GameServer(int port);
	virtual ~GameServer();

	void createGame(string ipAndPortUser1, string user1, string IpAndPortUser2, string user2);
	bool setMove(XO_Move * move);

	bool removeGame(string gameID); // the main server disconnect clients

	void run();
	void stopServer();
};

#endif /* SERVER_GAMESERVER_H_ */
