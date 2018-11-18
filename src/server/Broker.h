/*
 * Broker.h
 *
 *  Created on: Aug 20, 2017
 *      Author: demo
 */

#ifndef SERVER_BROKER_H_
#define SERVER_BROKER_H_
#include <string>
#include "../utils/ServerIO.h"
using namespace std;

class Broker {
public:
	string gameID();
	string ipInitPlayer;
	string ipOtherPlayer;
	int initPlayerPort;
	int otherPlayerPort;
	bool player1ConfirmConnection;
	bool player2ConfirmConnection;
	string player1;
	string player2;
	string toString(string ip, int port);
public:
	Broker(string ipInitPlayer, int initPlayerPort, string ipOtherPlayer, int otherPlayerPort,string player1,string player2);
	virtual ~Broker();

	bool isActive();
	bool isEqual(Broker * other);
	bool isInBroker(string ipAndPort);
	string getSecondPlayer(string ipAndAddress);
};

#endif /* SERVER_BROKER_H_ */
