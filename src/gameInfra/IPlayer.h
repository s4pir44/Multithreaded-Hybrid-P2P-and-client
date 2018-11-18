/*
 * IPlayer.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef GAMEINFRA_IPLAYER_H_
#define GAMEINFRA_IPLAYER_H_
#include "IMOVE.h"
#include<iostream>
using namespace std;
class IPlayer {
	IMOVE *move;
public:
	string name;
public:
	IPlayer();
	virtual IMOVE* getMove();
	virtual void setMove(IMOVE *move);
	virtual ~IPlayer();
};

#endif /* GAMEINFRA_IPLAYER_H_ */


