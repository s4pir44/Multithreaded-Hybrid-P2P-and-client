/*
 * IGame.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef GAMEINFRA_IGAME_H_
#define GAMEINFRA_IGAME_H_

#include "IMOVE.h"
#include "IPlayer.h"
#include "IViewer.h"

class IGame {
public:
	virtual string getGameID()=0;
	virtual void runGame()=0;
	virtual IPlayer* getNextPlayersTurn()=0;
	virtual IPlayer* getCurrentPlayer()=0;
	virtual bool isWinner(IPlayer *player)=0;
	virtual bool isGameOver()=0;
	virtual void notifyUsers(int command) = 0;
	virtual bool play(IPlayer* p)=0;
	virtual void saveGame()=0;
	virtual void stopGame()=0;
	virtual ~IGame(){};
};




#endif /* GAMEINFRA_IGAME_H_ */
