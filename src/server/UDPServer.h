/*
 * UDPServer.h
 *
 *  Created on: Aug 18, 2017
 *      Author: demo
 */

#ifndef SERVER_UDPSERVER_H_
#define SERVER_UDPSERVER_H_
#include "../threads/MThread.h"
#include "../sockets/UDPSocket.h"
#include "../sockets/TCPMessengerProtocol.h"
#include "../utils/ServerIO.h"
#include "../sockets/Types.h"
#include "../tcp/ClientTCP.h"
#include "../tcp/Socket.h"
#include "Broker.h"
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;
class UDPServer : public MThread  {
public:
	Socket* tcpSock;
	UDPSocket * serverSocket;
	vector<Broker*> brokers;
	map<string,string> usersToAddress;
	map<string,string> addressToUsers;
	bool isUserExists(string name);
	bool isRunning;
	bool login(string user, string pass);
	bool registerUser(string user, string pass);
	void setEstablishConnectionToBroker(string ipAndPort);
	bool isPlayerInBroker(string ipAndPortPlayer);
	void disconnectClients();
	string getRandomPlayer(string requester);

	void sendMsgToGameServer(int command, string msg);
private :
	bool containsBroker(Broker * broker);
	void disconnectBroker(string ipAndPort);
	int random(int start, int end);
public:
	UDPServer(int port);
	virtual ~UDPServer();
	/**
		 * Run a command
	*/
	void run();

	void handleClientRequests(int command, string msg);
	void stopServer();
};

#endif /* SERVER_UDPSERVER_H_ */
