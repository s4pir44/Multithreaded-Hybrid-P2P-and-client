/*
 * UDPClient.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: demo
 */

#include "UDPClient.h"
#include "../sockets/Strings.h"
#include <limits>
UDPClient::UDPClient(int port) {
	// TODO Auto-generated constructor stub
	this->isRunning = true;
	this->clientSocket = new UDPSocket(port);
	this->clientDetails = NULL;
	this->status = AUTH; // has port, now in auth
	this->waitForResponse = false;
	this->username = "";
	this->userRequest = "";
	this->acceptAll = false;
	this->rejectAll = false;
	this->waitForReplyFromPlayer = false;

	this->board = new XOBoardViewer(3);
	this->isMyturn = false;

	this->start();
}
void UDPClient::stopServer() {
	cout<<"[Client is disconnecting]"<<endl;
	this->status = DISCONNECTED;
	this->isRunning = false;
	this->clientSocket->cclose();
	this->waitForThread();
	if (this->clientSocket != NULL)
	{
		try {
			delete(this->clientSocket);
			this->clientSocket = NULL;
			cout<<"disconnect was successful"<<endl;
		}
		catch(const char * e) {
			cout<<"Can't delete clientSocket socket"<<endl;
		}

	}


	// TODO send disconnect request to broker according to status if im in broker as well as to server!!
}
void UDPClient::run() {
	if (clientSocket != NULL) {
				//cout << "[client is running]" << endl;

				while(this->isRunning)
				{
					char message[2000];
					while (clientSocket->recv(message, 2000) > 0)
					{
						string smsg = message;
						this->handleServerRequestsAndResponses(ServerIO::ReadCommand(smsg), ServerIO::ReadMessage(smsg));
					} // end of while (clientSocket->recv(message, 2000) > 0)
				} // end of while(this->isRunning)
			} // end of if (clientSocket != NULL)
			cout << "[Client Finish run function : worker - finished]" << endl;
}
void UDPClient::SetOtherPlayer(SingleClient* otherPlayer) {
	this->clientDetails = otherPlayer;
}
UDPClient::~UDPClient() {
	if (this->clientSocket != NULL)
	{
		delete(this->clientSocket);
	}
	if (this->clientDetails != NULL)
	{
		delete(this->clientDetails);
	}

}

bool UDPClient::sendMessage(string msg, string ip, int port)
{
	if (this->clientSocket != NULL)
	{
		return this->clientSocket->sendTo(msg,ip,port) > 0;
	}
	return false;
}
void UDPClient::printAcceptRejectInstructions() {
	cout<<"a - accept"<<endl;
	cout<<"r - reject"<<endl;
	cout<<"aa - accept automatically every user"<<endl;
	cout<<"rr - reject automatically every user"<<endl;
}

void UDPClient::HandlePeerRequest(const string& msg)
{
	if (this->status == DISPATCHER) // can respond only if the user is connected
	{
		cout<<endl;

		cout << "user " << msg << " wants to play with you." << endl;
		this->userRequest = msg; // opponent player

		if (!this->acceptAll && !this->rejectAll)
		{
			this->printAcceptRejectInstructions();
		}
		else if (this->acceptAll)
		{
			this->clientSocket->reply(ServerIO::CreateMessage(CHALLENGE_ACCEPTED, this->userRequest));
			cout<<"Press Enter to Match a Game "<<endl;
			this->status = WAIT_FOR_OPPONENT_ADDR;
		}
		else if (this->rejectAll)
		{
			cout<<"You rejected the play since you set Reject always..."<<endl;
			cout<<"Press Enter to Continue "<<endl;
			this->clientSocket->reply(ServerIO::CreateMessage(SESSION_REFUSED, this->userRequest));
		}


	} else {
		// refused user is not connected or in a game
		this->clientSocket->reply(
				ServerIO::CreateMessage(SESSION_REFUSED,
						"user not connected or play with someone else"));
	}
}
bool UDPClient::sendMessage2PeerPartner(string msg) {
	if (this->status != BROKER)
	{
		return false;
	}
	if (clientDetails == NULL || clientDetails->ip == "" || clientDetails->port <= 0)
	{
		cout<<"Failed to send message since client has not received client's address";
		return false;
	}
	try {
		string command = ServerIO::CreateMessage(NEW_MESSAGE_DST_RES, msg);
		this->clientSocket->sendTo(command ,clientDetails->ip, clientDetails->port );
		return true;
	}
	catch(const char *e) {
		perror(e);
	}

	return false;
}

void UDPClient::PrintUsers(const string& msg) {

	vector<string> users = Strings::split(msg, ",");
	cout << "users are: " <<users.size()<< endl;
	for (unsigned int i = 0; i < users.size(); i++) {
		cout <<(i+1)<<". " <<users.at(i)<<endl;
	}

}
int* UDPClient::getTable(string table, string delimiter)
{
	int *xoBoard = new int[this->board->size*this->board->size];
	vector<string> vTable = Strings::split(table, delimiter);
	for(unsigned int i = 0; i < vTable.size(); i++)
	{
		xoBoard[i] = ServerIO::FromStringToInt(vTable.at(i));
	}

	return xoBoard;
}
void UDPClient::displayBaordGame()
{
	if (this->board != NULL)
	{
		this->board->display();
	}

}


bool UDPClient::isStepValid(int i, int j)
{
	if (this->isMyturn)
	{
		if (this->board == NULL)
		{
			cout<<"board is NULL"<<endl;
			return false;
		} else
		{
			// check cell is empty
			if (this->board->table[i * this->board->size + j] != NO_VALUE)
			{
				cout<<"cell has value"<<endl;
				return false;
			}
			else{
				return true;
			}
		}
	}
	else {
		return false;
	}
}
bool UDPClient::sendMessageToServer(int command, string msg)
{
	try
	{
		return this->sendMessage(ServerIO::CreateMessage(command, msg),MESSENGER_SERVER_IP ,MESSENGER_SERVER_PORT);
	}
	catch(const char * e)
	{
		perror(e);
		return false;
	}
}

void UDPClient::handleServerRequestsAndResponses(int command, string msg)
{
	switch(command)
	{
	case SUCCESSFULLY_LOGIN_RES:
	{
		this->status = DISPATCHER; // in server connected list
		this->username = Strings::split(msg," ").at(0);
		cout<<msg<<endl;
		// get accept all or reject all status
		try {
				// get accept all/ reject all status
				this->clientSocket->reply(ServerIO::CreateMessage(STATUS_ACCEPT_ALL, ""));
				this->clientSocket->reply(ServerIO::CreateMessage(STATUS_REJECT_ALL, ""));
			}
			catch(const char * e) {

			}
		break;
	}
	case LOGIN_FAILED:
	{
		this->status = AUTH;
		cout<<"Failed to login "<<msg<<endl;
		break;
	}
	case USER_ALREADY_EXISTS_RES:
	{
		// user is in dispatcher mode : LOGIN FAILURE
		cout<<"Failed to login - user already logged to the server"<<endl;
		break;
	}
	case REGISTER_FAILED:
	{
		this->status = AUTH; // same status
		cout<<"Failed to register - User name is taken or server db is down"<<endl;
		break;
	}
	case REGISTER_SUCCESS:
	{
		this->status = AUTH; // same status
		cout<<msg<<endl; // still need AUTH (to login) !
		break;
	}
	case SESSION_REQ_PLAY_WITH_USER: // only if im in dispatch status !!
	{
		// when other user request to play with me
		HandlePeerRequest(msg);
		break;
	}
	case STATUS_ACCEPT_ALL_RES: {
		if (msg == "1" || msg == "true")
		{
			this->acceptAll = true;
		}
		break;
	}
	case STATUS_REJECT_ALL_RES:
	{
		if (msg == "1" || msg == "true")
		{
			this->rejectAll = true;
		}
		break;
	}
	case SESSION_REFUSED:
	{
		this->waitForReplyFromPlayer = false;
		this->status = DISPATCHER;
		if (msg == "one of the users is no longer connected" || msg == "You can't play with your self")
		{
			//refused by server
			cout<<msg<<endl;
		} else
		{
			// refused by user
			cout<<" The user '"<<msg<<"' refused to play with you. "<<endl;
			cout<<"Press Enter To Continue..."<<endl;
		}

		break;
	}
	case CANCEL_REQUEST:
	{
		this->status = DISPATCHER;
		cout<<"'"<<msg<<"' is no longer connected to server : Press Enter to continue"<<endl;
		this->waitForResponse = true;
		break;
	}
	case CHALLENGE_ACCEPTED:
	{
		this->waitForReplyFromPlayer = false;
		this->status = WAIT_FOR_OPPONENT_ADDR;
		cout<<"challenge accepted"<<endl;
		break;
	}
	case OPPONENT_ADDR:
	{
		// both players accept it from server
		this->status = BROKER;
		string cMsg = msg;
		vector<string>commadAndMessage2 = Strings::split(cMsg, ":");
		string playerIP = commadAndMessage2.at(0);
		int portPlayer = ServerIO::FromStringToInt(commadAndMessage2.at(1));
		this->clientDetails = new SingleClient(playerIP, portPlayer);
		//print for debug
		this->clientDetails->toString();
		// when server accepts it he knows that the broker is active
		this->clientSocket->reply(ServerIO::CreateMessage(SESSION_ESTABLISHED, "Got Address"));
		break;
	}
	case NEW_MESSAGE_DST_RES: // From Client
	{
		cout<<this->userRequest<<" : "<<msg<<endl; // print message
		sleep(2);
		break;
	}
	case GET_ALL_CONNECTED_USERS_RES:
	{
		cout<<"connected ";
		PrintUsers(msg);
		break;
	}
	case GET_ALL_USERS_RES:
	{
		cout<<"all ";
		PrintUsers(msg);
		break;
	}
	case REJECT_REQ_SUCCESS:
	{
		this->rejectAll = true;
		cout<<"Reject automatically player always was successful"<<endl;
		this->status = DISPATCHER;
		break;
	}
	case REJECT_REQ_FAILURE:
	{
		cout<<"Reject automatically player always Failed"<<endl;
		this->status = DISPATCHER;
		break;
	}
	case ACCEPT_REQ_SUCCESS:
	{
		this->acceptAll = true;
		cout<<"Accept automatically player always was successful"<<endl;
		break;
	}
	case ACCEPT_REQ_FAILURE:
	{
		cout<<"Accept automatically player always Failed"<<endl;
		break;
	}
	case DISCONNECT_CLIENTS:
	{
		this->stopServer();
		cout<<"Server disconnected you..."<<endl;
		break;
	}
	case EXIT_GAME: // From Broker ! or Game
	{
		this->status = DISPATCHER;
		cout<<"other user exited the game: Press Enter to continue"<<endl;
		break;
	}
	case PLAY_WITH_RANDOM_PLAYER_RES:
	{
		vector<string> response = Strings::split(msg, " ");
		if (response.at(0) == "true")
		{
			this->userRequest = response.back().at(0);
			cout<<"Press Enter"<<endl;
		}
		else
		{
			sleep(2);
		}
		cout<<msg.substr(response.at(0).length() + 1)<<endl; // cut the true or false..

		break;
	}

	// Games
	case GAME_YOU_WAIT:
	{
		// turn flag wait on
		this->isMyturn = false;
		this->board->setTable(this->getTable(ServerIO::ReadMessage(msg),","));
		this->displayBaordGame();
		cout<<"'"<<this->username<<"' please wait for '"<<this->userRequest<<"' move."<<endl;
		break;
	}
	case GAME_YOU_PLAY:
	{
		// turn flag wait on
		this->isMyturn = true;
		this->board->setTable(this->getTable(ServerIO::ReadMessage(msg),","));
		this->displayBaordGame();
		cout<<"'"<<this->username<<"' its your turn. please make a move."<<endl;
		Menus::PrintMenu(6);
		break;
	}
	case GAME_YOU_WIN:
	{
		// turn flag wait on

		this->board->setTable(this->getTable(msg,","));
		this->displayBaordGame();
		this->isMyturn = false;
		this->status = DISPATCHER;
		this->sendMessageToServer(GAME_IS_OVER,"WINNER"); // for keeping scores
		cout<<"'"<<this->username<<"' you WON ! Press Enter To Continue"<<endl;

		break;
	}
	case GAME_YOU_LOSE:
	{
		this->isMyturn = false;
		// turn flag wait on
		this->board->setTable(this->getTable(msg,","));
		this->displayBaordGame();
		this->status = DISPATCHER;

		this->sendMessageToServer(GAME_IS_OVER,"LOSSER");
		cout<<"'"<<this->username<<"' you lost...Press Enter To Continue"<<endl;
		break;
	}
	case GAME_STOP:
	{
		this->isMyturn = false;
		cout<<"*********************Game has been Cancelled****************"<<endl;
		break;
	}
	case GAME_DRAW:
	{
		this->isMyturn = false;
		this->board->setTable(this->getTable(msg,","));
		this->displayBaordGame();
		this->status = DISPATCHER;
		this->sendMessageToServer(GAME_IS_OVER,"DRAW");
		// turn flag wait on
		cout<<"Game is over in a draw "<<endl;
		break;
	}
	case GET_SCORES:
	{
		map<int, vector<string> > *m = new map<int, vector<string> >();
		vector<string> stringsR = Strings::split(msg, " ");
		for(unsigned int i = 0; i < stringsR.size(); i++)
		{
			string user = stringsR.at(i);
			vector<string> nameScore = Strings::split(user, "=");
			string scores = nameScore.at(1);
			vector<string> scoresArray = Strings::split(scores,"&");

			int wins = ServerIO::FromStringToInt(scoresArray.at(0));
			(*m)[wins].push_back(nameScore.at(0));

			//cout<<nameScore.at(0)<<" Wins : "<<scoresArray.at(0) <<" Loses : "<<scoresArray.at(1)<<" Draws : "<<scoresArray.at(2)<<endl;
		}
		cout<<"[Sorted]"<<endl;
		std::list<string> ordered;
		int index = 1;
		for (std::map<int, vector<string> >::const_iterator it = m->begin();
					it != m->end(); it++)
		{
			for(unsigned int i = 0; i < it->second.size(); i++)
			{
				string players = it->second.at(i)+" with "+ServerIO::FromIntToString(it->first,0)+" wins ";
				ordered.push_front(players);
			}

		}
		for (std::list<string>::iterator it = ordered.begin(); it != ordered.end(); it++)
		    	cout <<ServerIO::FromIntToString(index++,0)<<". "<< *it << endl;
		cout<<"Press Enter to continue"<<endl;
		delete(m);
		//cout<<"Message ["<<msg<<"]"<<endl;
		break;
	}
	} // end of switch
	this->waitForResponse = true;
}
