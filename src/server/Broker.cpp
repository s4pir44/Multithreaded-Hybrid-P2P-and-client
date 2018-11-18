/*
 * Broker.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: demo
 */

#include "Broker.h"

Broker::Broker(string ipInitPlayer, int initPlayerPort, string ipOtherPlayer, int otherPlayerPort, string p1, string p2) {
	this->ipInitPlayer = ipInitPlayer;
	this->initPlayerPort = initPlayerPort;
	this->ipOtherPlayer = ipOtherPlayer;
	this->otherPlayerPort = otherPlayerPort;

	this->player1 = p1;
	this->player2 = p2;

	this->player1ConfirmConnection = false;
	this->player2ConfirmConnection = false;

}
bool Broker::isEqual(Broker * other) {
	return this->ipInitPlayer == other->ipInitPlayer &&
			this->initPlayerPort == other->initPlayerPort &&
			this->ipOtherPlayer == other->ipOtherPlayer &&
			this->otherPlayerPort == other->otherPlayerPort;
}
string Broker::getSecondPlayer(string ipAndAddress){
	if (this->toString(this->ipInitPlayer, this->initPlayerPort) == ipAndAddress) {
		return this->toString(this->ipOtherPlayer, this->otherPlayerPort);
	} else {
		return this->toString(this->ipInitPlayer, this->initPlayerPort);
	}
}

string Broker::gameID()
{
	return this->player1 + this->player2;
}

bool Broker::isActive()
{
	return this->player1ConfirmConnection && this->player2ConfirmConnection;
}
string Broker::toString(string ip, int port) {
	return ip + ":" + ServerIO::FromIntToString(port,0);
}
bool Broker::isInBroker(string ipAndPort)
{
	return ipAndPort == this->toString(this->ipInitPlayer , this->initPlayerPort)
			|| ipAndPort == this->toString(this->ipOtherPlayer , this->otherPlayerPort) ;
}
Broker::~Broker() {
	// TODO Auto-generated destructor stub
}

