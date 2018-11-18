/*
 * GameServer.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#include "GameServer.h"

GameServer::GameServer(int port) {
	this->serverSocket = new UDPSocket(port);
	this->isRunning = true;

	this->start();
}

GameServer::~GameServer() {
	this->stopServer();
}
XOGame* GameServer::getGameByID(string gameID) {

	for(unsigned int i = 0; i < this->games.size(); i++ )
	{
		if (gameID == this->games.at(i)->getGameID())
		{
			return this->games.at(i);
		}
	}
	return NULL;
}

void GameServer::createGame(string ipAndPortUser1, string user1, string ipAndPortUser2, string user2)
{
	XOGame * newGame = new XOGame(this->serverSocket, ipAndPortUser1,user1,ipAndPortUser2,user2);
	// check if game exist
	if (this->getGameByID(newGame->getGameID()))
	{
		// wrong !! game exists
		return;
	}
	this->games.push_back(newGame);
	this->addressesToUsers[ipAndPortUser1] = user1;
	this->addressesToUsers[ipAndPortUser2] = user2;
	this->addressedToGames[ipAndPortUser1] = newGame->getGameID();
	this->addressedToGames[ipAndPortUser2] = newGame->getGameID();

	newGame->start(); // run in a new thread
}

void GameServer::run()
{
	while (this->isRunning) {
		char message[2000];
		while (serverSocket->recv(message, 2000) > 0)
		{
			string smsg = message;
			cout<<"["<<ServerIO::FromIntToString(this->serverSocket->getPort(), 0)<<"]";
			this->handleMessages(ServerIO::ReadCommand(smsg), ServerIO::ReadMessage(smsg));
		}
	}
}
#include <algorithm>
bool GameServer::removeGame(string gameID)
{
	XOGame *game = this->getGameByID(gameID);
	if(game != NULL)
	{
		game->stopGame();
		game->waitForThread();
		cout<<"Finished!"<<endl;
		map<string,string>::iterator it,it2,it3,it4;

		it  = this->addressedToGames.find (game->ipAndPort1);
		it3  = this->addressedToGames.find (game->ipAndPort2);
		it2 = this->addressesToUsers.find (game->ipAndPort1);
		it4 = this->addressesToUsers.find (game->ipAndPort2);

		this->addressedToGames.erase (it); // remove first client
		this->addressesToUsers.erase (it2);
		this->addressedToGames.erase (it3); // remove first client
		this->addressesToUsers.erase (it4);



		this->games.erase(std::remove(this->games.begin(), this->games.end(), game), this->games.end());
		cout<<"Removing game between "<<game->user1<<" and "<<game->user2;
		return true;
	}
	else
	{
		// game does not exists
		return false;
	}
}

void GameServer::handleMessages(int serverCommand, string serverMsg)
{
	/*
	 * #define  78
#define  79
#define  80
	 * */
	switch(serverCommand)
	{
		case GAME_CREATE:
		{
			// from server : ip1:port1:user1 ip2:port2:user2
			vector<string> players = Strings::split(serverMsg, " ");
			vector<string> player1 = Strings::split(players.at(0),":");
			vector<string> player2= Strings::split(players.at(1),":");
			this->createGame(player1.at(0) +":"+ player1.at(1), player1.at(2),
					player2.at(0)+":"+player2.at(1), player2.at(2));
			break;
		}
		case GAME_MAKE_MOVE:
		{
			//from client
			//cout<<"[Game_Move] "<<serverMsg<<endl;
			vector<string> moves = Strings::split(serverMsg,",");
			XO_Move* playersMove = new XO_Move(ServerIO::FromStringToInt(moves.at(0)),ServerIO::FromStringToInt(moves.at(1)));
			if (!this->setMove(playersMove))
			{
				cout<<"Wrong Step!!!!"<<endl;
				// TODO: bad play move !
			}
			break;
		}
		case GAME_REMOVE:
		{
			cout<<"[Game Server][Remove Game]";
			// from server
			if (!this->removeGame(serverMsg))
			{
				// TODO: handle
				cout<<"[Failed]"<<endl;
			} else {
				cout<<"[Success]"<<endl;
			}
			break;
		}
	}
}

bool GameServer::setMove(XO_Move * move)
{
	string userIpandPort = this->serverSocket->destIpAndPort();
	string gameID = this->addressedToGames[userIpandPort];
	XOGame *game = this->getGameByID(gameID);
	if (game != NULL)
	{
		XOPlayer * p = (XOPlayer*)game->getCurrentPlayer();
		move->sign = p->getSign();
		game->setMove(this->addressesToUsers[userIpandPort], move);
		return true;
	}
	else
	{
		// exception !! reply => game does not exists
		return false;
	}
}
void GameServer::stopServer()
{
	this->isRunning = false;
	//this->disconnectClients(); TODO : DISCONNECT_USERS
	this->serverSocket->cclose();
	this->waitForThread();
	if (this->serverSocket != NULL)
	{
		try {
				delete(this->serverSocket);
				this->serverSocket = NULL;
			}
			catch(const char * e) {
				cout<<"Can't delete game server socket"<<endl;
			}
	}
	cout<<"[Game Server]"<<endl;
	for(unsigned int i = 0; i<this->games.size(); i++ )
	{
		cout<<"Stopping Game. "<<(i+1);
		this->games.at(i)->stopGame();
		this->games.at(i)->waitForThread();
		delete(this->games.at(i));
		cout<<" - stopped."<<endl;
	}

	cout<<"[GameServer] server is down"<<endl;
}
