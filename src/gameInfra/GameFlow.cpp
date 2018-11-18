/*
 * GameFlow.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#include "GameFlow.h"
#include<iostream>
using namespace std;
GameFlow::GameFlow() {
	this->viewer = NULL;
	this->turn = 0; // first player begins
	isGameRunning = true;

}

void GameFlow::setParams(IViewer * view, vector<IPlayer*> players)
{
	this->viewer = viewer;
	this->players = players;
}

IPlayer* GameFlow::getCurrentPlayer()
{
	return this->players.at(this->turn);
}

void GameFlow::runGame()
{
	try{
		bool winner = false;
		while(!this->isGameOver())
		{
			IPlayer *p = this->getCurrentPlayer();
			this->play(p);

			// still running?
			if (this->isWinner(p))
			{
				winner = true;
				this->notifyUsers(WINNER_ANNONCMENT);
				break;
			}
			if (!isGameRunning)
			{
				cout<<"game was aborted"<<endl;
				this->notifyUsers(GAME_STOP); // sending the table to users to view
				break;
			}
			this->notifyUsers(WHOS_TURN_NOW); // sending the table to users to view
			// play with next player
			p = this->getNextPlayersTurn();
		}
		if (!winner)
		{
			this->notifyUsers(DRAW_ANNOUNCMENT);
		}

		this->saveGame();
	}catch(const char * e){
		cout<<e<<endl;
	}

}


IPlayer* GameFlow::getNextPlayersTurn()
{
	int numOfPlayers = this->players.size();
	int t = turn % numOfPlayers;
	turn++;
	if (turn == numOfPlayers) turn = 0; // re-init
	IPlayer *p = this->players.at(t);

	return p;
}

GameFlow::~GameFlow() {
	if (this->viewer != NULL)
		delete(this->viewer);
	for(unsigned int i = 0; i < this->players.size(); i++)
				delete(this->players.at(i));
}

