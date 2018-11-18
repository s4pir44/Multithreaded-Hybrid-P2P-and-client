/*
 * XOPlayer.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#include "XOPlayer.h"

XOPlayer::XOPlayer(int sign, string ipAndPort, string user) {
	this->sign = sign;
	this->ipandport = ipAndPort;
	this->user = user;

	this->name = user;
}
string XOPlayer::getUser()
{
	return this->user;
}
string XOPlayer::getIpAndPort()
{
	return this->ipandport;
}
int XOPlayer::getSign()
{
	return this->sign;
}
XOPlayer::~XOPlayer() {

}

