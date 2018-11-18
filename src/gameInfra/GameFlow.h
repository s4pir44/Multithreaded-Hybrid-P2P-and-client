/*
 * GameFlow.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef GAMEINFRA_GAMEFLOW_H_
#define GAMEINFRA_GAMEFLOW_H_
#include <iostream>
using namespace std;
#include <strings.h>
#include <vector>
#include "IGame.h"
#include "IViewer.h"
#include "IPlayer.h"
#include "IMOVE.h"



#define WHOS_TURN_NOW 2565
#define WINNER_ANNONCMENT 2566
#define DRAW_ANNOUNCMENT 2567
#define INIT_RUN_STATE 2568
#define GAME_STOP 82
class GameFlow :public IGame {
	int turn;
public:
	bool isGameRunning;
	IViewer *viewer;
	vector<IPlayer*> players;
	virtual string getGameID()=0;
	void runGame();
	IPlayer* getNextPlayersTurn();
	IPlayer* getCurrentPlayer();
	virtual bool isWinner(IPlayer *player)=0;
	virtual bool isGameOver()=0;
	virtual void notifyUsers(int command) = 0;
	virtual bool play(IPlayer* p)=0;
	virtual void saveGame()=0;
	virtual void stopGame()=0;
	void setParams(IViewer * view, vector<IPlayer*> players);
	GameFlow();
	virtual ~GameFlow();
};

#endif /* GAMEINFRA_GAMEFLOW_H_ */
