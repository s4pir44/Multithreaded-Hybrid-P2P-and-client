/*
 * IPlayer.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#include "IPlayer.h"

IPlayer::IPlayer() {
	this->move = NULL;

}
void IPlayer::setMove(IMOVE * m)
{
	this->move = m;
}
IMOVE * IPlayer::getMove()
{
	return this->move;
}
IPlayer::~IPlayer() {
	if (this->move != NULL)
		delete(this->move);
}

