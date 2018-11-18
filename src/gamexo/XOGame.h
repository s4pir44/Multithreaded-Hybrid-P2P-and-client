/*
 * XOGame.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef GAMEXO_XOGAME_H_
#define GAMEXO_XOGAME_H_
#include<iostream>
using namespace std;
#include "../gameInfra/GameFlow.h"
#include "../sockets/UDPSocket.h"
#include "../threads/MThread.h"
#include "../sockets/Strings.h"
#include "../sockets/TCPMessengerProtocol.h"
#include "../utils/ServerIO.h"
#include "XOBoardViewer.h"
#include "XOPlayer.h"
#include "XOMove.h"
#include <vector>

class XOGame :public GameFlow, public MThread {


	UDPSocket * socketToSendFrom;
	bool isRunning;
	bool waitForMove;
	bool isUserWinner(int sign);
	void sendMessage(string ipAndPort, string msg);
	string fromTableToString(int *table, int size);
public:
	string user1;
	string user2;
	string ipAndPort1;
	string ipAndPort2;
	void switchturns(XOPlayer * p1, XOPlayer *p2);
	void setMove(string ipAndPort, XO_Move *move);
	string getGameID(); // unique key for the game..
	void saveGame();
	bool isWinner(IPlayer *player);
	bool isGameOver();
	void notifyUsers(int command);
	bool play(IPlayer* player);
	void stopGame();
	XOGame(UDPSocket * game, string ipAndPortUser1, string user1, string IpAndPortUser2, string user2);
	virtual ~XOGame();

	void run();
};

#endif /* GAMEXO_XOGAME_H_ */
