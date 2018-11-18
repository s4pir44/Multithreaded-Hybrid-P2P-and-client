/*
 * IHandler.h
 *
 *  Created on: Aug 24, 2017
 *      Author: demo
 */

#ifndef TCP_IHANDLER_H_
#define TCP_IHANDLER_H_
#include<string>
using namespace std;

#include "GameServer.h"

class GameServer;

class IGameHandler {
public:
	virtual ~IGameHandler(){};
	virtual void handleMessages(GameServer *gameServer, int serverCommand, string serverMsg) = 0;
};

#endif /* TCP_IHANDLER_H_ */
