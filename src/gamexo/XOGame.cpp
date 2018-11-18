/*
 * XOGame.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#include "XOGame.h"

XOGame::XOGame(UDPSocket * game, string ipAndPortUser1, string user1, string ipAndPortUser2, string user2) {
	this->ipAndPort1 = ipAndPortUser1;
	this->user1 = user1;
	this->ipAndPort2 = ipAndPortUser2;
	this->user2 = user2;
	this->socketToSendFrom = game;
	vector<IPlayer*>players;
	XOPlayer *p1 = new XOPlayer(X, ipAndPortUser1, user1);
	XOPlayer *p2 = new XOPlayer(O,ipAndPortUser2,user2);

	players.push_back(p1);
	players.push_back(p2);

	this->isRunning = true;
	this->waitForMove = true;

	this->players = players;
	this->viewer = new XOBoardViewer(3);
}

void XOGame::setMove(string user, XO_Move *move)
{
	IPlayer * p = this->getCurrentPlayer();
	if (p->name == user)
	{
		p->setMove(move);
		this->waitForMove = false;
	}
}
bool XOGame::play(IPlayer * p)
{
	while(isRunning)
	{
		while(!this->waitForMove)
		{
			XO_Move * move = (XO_Move*) p->getMove();
			XOBoardViewer * view = (XOBoardViewer*)this->viewer;
			if (move != NULL)
			{
				int r = move->row * ((XOBoardViewer*)this->viewer)->size;
				int c = move->col;
				int* table = view->table;
				table[r+c] = move->sign;
				view->setTable(table);
				this->waitForMove = true;
				return true;
			}
			else
			{
				// wrong move
			}

		}
	}
	return true;
}
void XOGame::stopGame()
{
	this->waitForMove = true;
	this->isRunning = false;

	// diconnect clients TODO:: Send DISCONNECTION MESSAGE
}

void XOGame::switchturns(XOPlayer * p, XOPlayer *p2)
{
	XOBoardViewer * board = (XOBoardViewer*) this->viewer;
	// player 1 just played : player 2 is next
	string youWait = ServerIO::CreateMessage(GAME_YOU_WAIT, this->fromTableToString(board->table,board->size));
	string youPlay = ServerIO::CreateMessage(GAME_YOU_PLAY, this->fromTableToString(board->table,board->size));
	this->sendMessage(p->getIpAndPort(), ServerIO::CreateMessage(GAME_YOU_WAIT, youWait));
	this->sendMessage(p2->getIpAndPort(), ServerIO::CreateMessage(GAME_YOU_PLAY, youPlay));
}
void XOGame::sendMessage(string ipAndPort, string msg)
{
	vector<string> ipAndPortParts = Strings::split(ipAndPort, ":");
	string ip = ipAndPortParts.at(0);
	int port = ServerIO::FromStringToInt(ipAndPortParts.at(1));

	this->socketToSendFrom->sendTo(msg, ip, port);
}
void XOGame::run()
{
	this->notifyUsers(INIT_RUN_STATE);
	this->runGame();
}
string XOGame::fromTableToString(int *table, int size)
{
	vector<string> tString;
	for(int i = 0; i < size; i++)
	{
		int row = size * i;
		for(int j = 0; j < size; j++)
		{
			tString.push_back(ServerIO::FromIntToString(table[row+j], 0));
		}
	}
	return ServerIO::FormatVector(tString,",");
}
void XOGame::notifyUsers(int command)
{
	if (command == GAME_STOP)
	{

		string stopGameMsg = ServerIO::CreateMessage(GAME_STOP, "");
		for(unsigned int i = 0; i < this->players.size(); i++)
		{
			XOPlayer * p = (XOPlayer *) this->players.at((i));
			this->sendMessage(p->getIpAndPort(),stopGameMsg);
		}
	}
	if (command == INIT_RUN_STATE)
	{
		this->switchturns((XOPlayer *) this->players.at(1),(XOPlayer *) this->getCurrentPlayer());
	}
	// TODO : Send view to clients by udp
	if (command == WHOS_TURN_NOW)
	{
		XOPlayer * p = (XOPlayer *) this->getCurrentPlayer();
		if(this->players.at(0)->name == p->name)
		{
			// player 1 just played : player 2 is next
			this->switchturns(p, (XOPlayer *) this->players.at(1));
		}
		else
		{
			// player 2 played, now its player 1 turn
			this->switchturns(p, (XOPlayer *) this->players.at(0));
		}
	}
	else if (command == WINNER_ANNONCMENT )
	{
		XOPlayer * winner = (XOPlayer*) this->getCurrentPlayer(); // winner!
		XOPlayer * other;
		// winner is player 2
		XOBoardViewer * board = (XOBoardViewer*) this->viewer;
		this->sendMessage(winner->getIpAndPort(), ServerIO::CreateMessage(GAME_YOU_WIN, this->fromTableToString(board->table,board->size)));
		if(this->players.at(0)->name == winner->name)
		{
			// winner is player 1
			other = (XOPlayer*)this->players.at(1);
			this->sendMessage(other->getIpAndPort(), ServerIO::CreateMessage(GAME_YOU_LOSE, this->fromTableToString(board->table,board->size)));
		}
		else
		{
			other = (XOPlayer*)this->players.at(0);
			this->sendMessage(other->getIpAndPort(), ServerIO::CreateMessage(GAME_YOU_LOSE, this->fromTableToString(board->table,board->size)));
		}

	}
	else if (command == DRAW_ANNOUNCMENT)
	{
		XOPlayer * p1 = (XOPlayer*) this->players.at(0);
		XOPlayer * p2 = (XOPlayer*) this->players.at(1);
		XOBoardViewer * board = (XOBoardViewer*) this->viewer;
		this->sendMessage(p1->getIpAndPort(), ServerIO::CreateMessage(GAME_DRAW, this->fromTableToString(board->table,board->size)));
		this->sendMessage(p2->getIpAndPort(), ServerIO::CreateMessage(GAME_DRAW, this->fromTableToString(board->table,board->size)));
	}
}


bool XOGame::isGameOver()
{
	if (!this->isRunning)
	{
		return true;
	}
	XOBoardViewer * board = (XOBoardViewer *) this->viewer;
	int total = board->size * board->size;
	for(int i = 0; i<total; i++)
		if (board->table[i] == NO_VALUE) return false;
	return true;
}
bool XOGame::isUserWinner(int sign)
{

	XOBoardViewer * board = (XOBoardViewer *) this->viewer;
	bool rightDiag = true;
	// check all rows
	for(unsigned int i = 0; i < board->size; i++)
	{
		unsigned int row = i * board->size;
		unsigned int j;
		for(j = 0; j < board->size; j++)
		{
			if (board->table[row + j] != sign)
			{
				break;
			}
		}
		if (j == board->size)
		{
			// winner
			return true;
		}

		// check right diagonal
		if (board->table[(board->size + 1) * i] != sign)
		{
			rightDiag = false;
			break;
		}
	}
	if (rightDiag)
	{
		return true;
	}

	//check left diag
	int jInd = board->size - 1;
	unsigned int index = 0;
	int row = 0;
	for(jInd = board->size - 1; jInd >= 0 ; jInd--)
	{
		row = board->size * index;
		if (board->table[row + jInd] != sign)
		{
			break;
		}

		index++;
	}
	if (jInd == -1)
	{
		return true;
	}
	for(unsigned int i = 0; i < board->size; i++)
	{
		int j;
		int row = board->size  * i;
		for(j = board->size - 1; j >= 0 ; j--)
		{
			if (board->table[row + j] != sign)
			{
				break;
			}
			i++;
		}
		if (j == -1)
		{
			return true;
		}
	}

	// check all cols
	for(unsigned int j = 0; j < board->size; j++)
	{
		unsigned int i;
		for(i = 0; i < board->size; i++)
		{
			unsigned int row = i * board->size;
			if (board->table[j + row] != sign)
			{
				break;
			}
		}
		if (i == board->size)
		{
			// winner
			return true;
		}
	}


	return false;
}
bool XOGame::isWinner(IPlayer *player)
{
	XO_Move * move = (XO_Move*) player->getMove();
	if (move == NULL)
	{
		this->isGameRunning = false;
		return false;
	}
	return this->isUserWinner(move->sign);
}
void XOGame::saveGame()
{

}
string XOGame::getGameID()
{
	XOPlayer * p1 = (XOPlayer*) this->players.at(0);
	XOPlayer * p2 = (XOPlayer*)this->players.at(1);

	return p1->getUser()+p2->getUser();
}
XOGame::~XOGame() {
	// TODO Auto-generated destructor stub
}

