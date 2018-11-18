/*
 * XOHandler.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef SERVER_XOHANDLER_H_
#define SERVER_XOHANDLER_H_
#include<string>
using namespace std;
#include "IGameHandler.h"
#include "GameServer.h"
#include "../sockets/TCPMessengerProtocol.h"
#include "../sockets/Strings.h"
#include "../utils/ServerIO.h"
#include<vector>
#include "../gamexo/XOMove.h"


class XO_Handler {
	//XO_Move* readMove(string userMove);
public:
	XO_Handler();
	virtual ~XO_Handler();

	//void handleMessages(GameServer *gameServer, int serverCommand, string serverMsg);
};
class GameServer;
#endif /* SERVER_XOHANDLER_H_ */
