/*
 * UDPServer.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: demo
 */

#include "UDPServer.h"
#include "../sockets/Strings.h"
UDPServer::UDPServer(int port) {
	this->serverSocket = new UDPSocket(port);
	this->isRunning = true;

	this->tcpSock = new ClientTCP(TCP_SERVER_DB_PORT, (char *)"127.0.0.1");
	this->start();
}

UDPServer::~UDPServer() {
	if (this->serverSocket != NULL)
		delete(this->serverSocket);
	if(this->tcpSock != NULL)
		delete(this->tcpSock);

}

void UDPServer::stopServer() {
	cout<<"Stopping Server..."<<endl;
	this->isRunning = false;
	this->disconnectClients();
	this->serverSocket->cclose();
	this->waitForThread();
	if (this->serverSocket != NULL)
	{
		try {
				delete(this->serverSocket);
				this->serverSocket = NULL;
			}
			catch(const char * e) {
				cout<<"Can't delete server socket"<<endl;
			}
	}
	cout<<"[Server] server is down"<<endl;

}


void UDPServer::disconnectClients() {
	cout<<"[Server - Disconnect clients] : ";
	if (this->usersToAddress.empty()) {
		cout<<" No users Attached to server"<<endl;
	}
	else
	{
		cout<<this->usersToAddress.size()<<endl;
		 for (std::map<string,string>::iterator it=this->usersToAddress.begin(); it!=usersToAddress.end(); ++it)
		 {
			 cout<<"Disconnect "<<it->first<<" from address "<<it->second<<endl;
			 vector<string> ipAndPort = Strings::split(it->second, ":");
			 string ip = ipAndPort.at(0);
			 int portClient = ServerIO::FromStringToInt(ipAndPort.at(1));
			 this->serverSocket->sendTo(ServerIO::CreateMessage(DISCONNECT_CLIENTS, "Server is shutting down"),ip,portClient);
		 }
	}


	 usersToAddress.clear();
	 this->addressToUsers.clear();
}

/**
 * Run a command
 */
void UDPServer::run() {


	while (this->isRunning) {
		char message[2000];
		while (serverSocket->recv(message, 2000) > 0)
		{
			string smsg = message;
			cout<<"["<<ServerIO::FromIntToString(this->serverSocket->getPort(), 0)<<"]"<<"[Server -";
			this->handleClientRequests(ServerIO::ReadCommand(smsg), ServerIO::ReadMessage(smsg));
		}
	}
}
bool UDPServer::isUserExists(string name)
{
	return this->usersToAddress.find(name) != this->usersToAddress.end();
}

int UDPServer::random(int LOW, int HIGH)
{
	/*
	Declare variable to hold seconds on clock.
	*/
	time_t seconds;
	/*
	Get value from system clock and
	place in seconds variable.
	*/
	time(&seconds);
	/*
	Convert seconds to a unsigned
	integer.
	*/
	srand((unsigned int) seconds);

	return (rand() % (HIGH - LOW + 1) + LOW);
}


bool UDPServer::registerUser(string user, string pass)
{
	/*	TODO: Add tcp client server request
		 *
		 * 1. check if tcpClient has an open socket if no try to coonect
		 * 2. move the same message in the tcp ServerIO::CreateMessage(LOGIN_REQ,user + " " + pass);
		 * 3. this is syncronious task so you must wait for a response after 20 seconds if not response break from the loop
		 *
		 * right now locally
	* */
	try {

		bool success = true;
		if (isUserExists(user))
		{
			success = false;
			this->serverSocket->reply(ServerIO::CreateMessage(REGISTER_FAILED, user + " already exits"));
		}
		else
		{
			char buffer[50];
			string registerRequest = ServerIO::CreateMessage(REGISTER_REQ,user + " " + pass); // wrong password
			this->tcpSock->sendInfo((char*)registerRequest.c_str());
			this->tcpSock->reciveInfo(buffer);
			success = ServerIO::ReadCommand(string(buffer)) == REGISTER_SUCCESS;
		}

		if (success)
		{
			this->serverSocket->reply(ServerIO::CreateMessage(REGISTER_SUCCESS, user + " Success to register"));
		}
		else
		{
			this->serverSocket->reply(ServerIO::CreateMessage(REGISTER_FAILED, user + " already exits"));
		}

		return true;
	}
	catch(const char * e) {
		perror(e);
		return false;
	}

}
bool UDPServer::login(string user, string pass) {
	/*
	 *
	 * 1. check if tcpClient has an open socket if no try to coonect
	 * 2. move the same message in the tcp ServerIO::CreateMessage(LOGIN_REQ,user + " " + pass);
	 * 3. this is syncronious task so you must wait for a response after 20 seconds if not response break from the loop
	 *
	 * right now locally
	 * */

	if (isUserExists(user))
	{
		this->serverSocket->reply(ServerIO::CreateMessage(USER_ALREADY_EXISTS_RES, user + "is already logged in"));
		return false;
	}
	string loginRequest = ServerIO::CreateMessage(LOGIN_REQ,user + " " + pass);
	this->tcpSock->sendInfo((char*)loginRequest.c_str());
	char buffer[50];
	this->tcpSock->reciveInfo(buffer);
	if (SUCCESSFULLY_LOGIN_RES == ServerIO::ReadCommand(string(buffer)))
	{
		this->usersToAddress[user] = this->serverSocket->destIpAndPort(); // add name,ip_and_port
		this->addressToUsers[this->serverSocket->destIpAndPort()] = user; // add name,ip_and_port
		this->serverSocket->reply(ServerIO::CreateMessage(SUCCESSFULLY_LOGIN_RES, user + " - is currently logged in to messenger server"));
		return true;
	}
	else
	{
		this->serverSocket->reply(ServerIO::CreateMessage(LOGIN_FAILED, user + " : user or password wrong. "));
		return false;
	}

}
#define NO_USERS_AVAIABLE ""
string UDPServer::getRandomPlayer(string requester)
{
	/* Rules
	 * 		1. user can not play with himself
	 * 		2. user can only play with connected user
	 * 		3. user can not play with connected user that is playing other game
	 * 		4. user can not play with a player that is in rejected all situation */
	string user = NO_USERS_AVAIABLE;
	vector<string> possiblePartners;
	string rejectedList = ServerIO::CreateMessage(GET_REJECTERS_PLAYERS,"");
	this->tcpSock->sendInfo((char*)rejectedList.c_str());
	char buffer[50];
	this->tcpSock->reciveInfo(buffer);
	vector<string> rejectedUsers = Strings::split(string(buffer), ",");

	for (std::map<string,string>::iterator it=this->usersToAddress.begin(); it!=usersToAddress.end(); ++it)
	{
		if (it->first == requester) continue;
		// if the user is connected to server and not playing with anyone else
		if (this->isUserExists(it->first)
				&& !this->isPlayerInBroker(it->second)
				&& !ServerIO::isInVector(it->first,rejectedUsers))
		{
			possiblePartners.push_back(it->first);
		}
	}

	if (!possiblePartners.empty())
	{
		int rand = this->random(0, possiblePartners.size());
		if ((unsigned int) rand == possiblePartners.size())
		{
			rand--; // avoid out of range
		}
		return possiblePartners.at(rand);
	}

	return user;
}
void UDPServer::sendMsgToGameServer(int command, string msg)
{
	this->serverSocket->sendTo(ServerIO::CreateMessage(command, msg),UDP_GAME_SERVER_IP,UDP_GAME_SERVER_PORT );
}
void UDPServer::handleClientRequests(int command,string msg)
{
	switch(command)
	{
		case LOGIN_REQ:
		{
			cout<<" Login] From :["<<this->serverSocket->destIpAndPort()<<" - ";
			vector<string> userAndPassword = Strings::split(msg, " ");
			try {
				string name = userAndPassword.at(0);
				cout<<name<<"]";
				string password = userAndPassword.at(1);
				if (login(name, password)) {
					cout<<" [Success]"<<endl;
				} else {
					cout<<" [Failed]"<<endl;
				}
			}
			catch(const char * e)
			{
				this->serverSocket->reply(ServerIO::CreateMessage(LOGIN_FAILED, "failed logged in"));
			}
			break;
		}
		case REGISTER_REQ:
		{
			cout<<" Register] From :["<<this->serverSocket->destIpAndPort()<<"]"<<endl;
			vector<string> userAndPassword = Strings::split(msg, " ");
			try {
				string name = userAndPassword.at(0);
				string password = userAndPassword.at(1);
				this->registerUser(name, password);
			}
			catch(const char * e)
			{
				this->serverSocket->reply(ServerIO::CreateMessage(REGISTER_FAILED, "failed register"));
			}
			break;
		}
		case SESSION_REQ_PLAY_WITH_USER: // accepts SESSION_REQ_PLAY_WITH_USER$size of other user$other user
		{
			// assume user is allowed to make this action : that is not in other broker!!! TODO:
			// msg is the 2nd user
			cout<<" Chat Request] From :["<<this->serverSocket->destIpAndPort()<<" - " << this->addressToUsers[this->serverSocket->destIpAndPort()]<<"]";
			if (this->addressToUsers[this->serverSocket->destIpAndPort()] == msg) {
				cout<<" To : "<<msg<<" Failed since "<<msg<<" user can't play with himself "<<endl;
				this->serverSocket->reply(ServerIO::CreateMessage(SESSION_REFUSED, "You can't play with your self"));
			}
			else if (!this->isUserExists(msg)
					|| !this->isUserExists(this->addressToUsers[this->serverSocket->destIpAndPort()]))
			{
				cout<<" To : "<<msg<<" Failed since "<<msg<<" is not connected to server"<<endl;
				this->serverSocket->reply(ServerIO::CreateMessage(SESSION_REFUSED, "one of the users is no longer connected"));
			}
			else
			{
				// Find 2nd user and request him
				try {
					string ipAndPortString = this->usersToAddress[msg];
					if (isPlayerInBroker(ipAndPortString)) {
						this->serverSocket->reply(ServerIO::CreateMessage(SESSION_REFUSED, "user is playing.."));
						return ;
					}
					vector<string> ipAndPort = Strings::split(ipAndPortString, ":");
					string otherIP = ipAndPort.at(0);
					int otherPort = ServerIO::FromStringToInt(ipAndPort.at(1));
					string initPlayer = this->addressToUsers[this->serverSocket->destIpAndPort()];
					cout<<" To : ["<<msg<<"]"<<endl;
					this->serverSocket->sendTo(ServerIO::CreateMessage(SESSION_REQ_PLAY_WITH_USER,initPlayer),otherIP, otherPort);
				}
				catch(const char * e) {
					perror(e);
				}
			}
			break;
		}
		case PLAY_WITH_RANDOM_PLAYER:
		{
			cout<<" Chat Request Random] From :["<<this->serverSocket->destIpAndPort()<<" - " << this->addressToUsers[this->serverSocket->destIpAndPort()]<<"]";
			string returnedMsg = "false No User Was Found To Play With";
			string randomPlayerName = this->getRandomPlayer(this->addressToUsers[this->serverSocket->destIpAndPort()]);
			if (randomPlayerName == NO_USERS_AVAIABLE)
			{
				this->serverSocket->reply(ServerIO::CreateMessage(PLAY_WITH_RANDOM_PLAYER_RES, returnedMsg));
			}
			else
			{
				// user was found
				string ipAndPortPartner = this->usersToAddress[randomPlayerName];
				vector<string> ipAndPort = Strings::split(ipAndPortPartner, ":");
				string otherIP = ipAndPort.at(0);
				int otherPort = ServerIO::FromStringToInt(ipAndPort.at(1));
				string initPlayer = this->addressToUsers[this->serverSocket->destIpAndPort()];
				cout<<" To : ["<<randomPlayerName<<"]"<<endl;
				this->serverSocket->sendTo(ServerIO::CreateMessage(SESSION_REQ_PLAY_WITH_USER,initPlayer),otherIP, otherPort);
				// ask
				returnedMsg ="true user request was sent to " + randomPlayerName;
				this->serverSocket->reply(ServerIO::CreateMessage(PLAY_WITH_RANDOM_PLAYER_RES, returnedMsg));
			}
			break;
		}
		case CHALLENGE_ACCEPTED: // 2nd client approve game
		{

			string otherPlayerResponse = this->addressToUsers[this->serverSocket->destIpAndPort()];
			if (!this->isUserExists(msg))
			{
				// 2nd user press x button
				cout<<" CHALLENGE_ACCEPTED - Failure]"<<" Game between : '"<<msg<<"' and '"<<otherPlayerResponse<<"'"<<"[user = "<<msg<<"] is not connected"<<endl;
				this->serverSocket->reply(ServerIO::CreateMessage(CANCEL_REQUEST, msg));
			}
			else
			{
				string ipAndPortString = this->usersToAddress[msg];
				vector<string> ipAndPort = Strings::split(ipAndPortString, ":");
				string otherIP = ipAndPort.at(0);
				int otherPort = ServerIO::FromStringToInt(ipAndPort.at(1));
				this->serverSocket->reply(ServerIO::CreateMessage(OPPONENT_ADDR, ipAndPortString));
				this->serverSocket->sendTo(ServerIO::CreateMessage(OPPONENT_ADDR,
						this->serverSocket->destIpAndPort()),otherIP,otherPort);
				vector<string> ipAndPortSecondPlayer = Strings::split(this->serverSocket->destIpAndPort(), ":");
				// create Broker ! send OPPONENT_ADDR, each other address

				cout<<" CHALLENGE_ACCEPTED]"<<" Game between : '"<<msg<<"' and '"<<otherPlayerResponse<<"'"<<endl;

				Broker * broker = new Broker(otherIP,
						otherPort,
						ipAndPortSecondPlayer.at(0),
						ServerIO::FromStringToInt(ipAndPortSecondPlayer.at(1)),
						msg,
						otherPlayerResponse);
				this->brokers.push_back(broker);
				string playersmsg = this->usersToAddress[msg] + ":" + msg + " " + this->usersToAddress[otherPlayerResponse] + ":"+otherPlayerResponse;
				this->sendMsgToGameServer(GAME_CREATE,playersmsg);
			}

			break;
		}
		case SESSION_REFUSED:
		{
			string refusingPlayer = this->addressToUsers[this->serverSocket->destIpAndPort()];
			string ipAndPortString = this->usersToAddress[msg];
			vector<string> ipAndPort = Strings::split(ipAndPortString, ":");
			string otherIP = ipAndPort.at(0);
			int otherPort = ServerIO::FromStringToInt(ipAndPort.at(1));
			cout<<"player '"<<refusingPlayer<<"' Refused to play with '"<<msg<<"' ";
			cout<<ipAndPortString<<"["<<msg<<"]"<<endl;
			this->serverSocket->sendTo(ServerIO::CreateMessage(SESSION_REFUSED, refusingPlayer), otherIP,otherPort);
			break;
		}
		case SESSION_ESTABLISHED :
		{
			cout<<" SESSION_ESTABLISHED ["<<this->serverSocket->destIpAndPort()<<"]"<<endl;
			this->setEstablishConnectionToBroker(this->serverSocket->destIpAndPort());
			break;
		}
		case DISCONNECT_FROM_CLIENT:
		{
			try {
				string ip = this->serverSocket->destIpAndPort();
				this->disconnectBroker(ip);
				string user = this->addressToUsers[ip];
				//int p = ServerIO::FromStringToInt(Strings::split(ip, "").at(1));
				//this->serverSocket->sendTo(ServerIO::CreateMessage(DISCONNECT_CLIENTS, "server shut down"), this->serverSocket->fromAddr(),p);
				map<string,string>::iterator it = this->usersToAddress.find(user);
				this->usersToAddress.erase(it);
				map<string,string>::iterator it2 = this->addressToUsers.find(ip);
				this->addressToUsers.erase(it2);
				this->disconnectBroker(this->serverSocket->destIpAndPort());
				cout<<" Disconnect] server disconnected the client :" <<user<<endl;
			}
			catch(const char * e) {
				cout<<"FALLS HERE"<<endl;
				perror(e);
			}
			break;
		}
		case DISCONNECT_FROM_BROKER:
		{

			break;
		}
		case ACCEPT_ALWAYS:
		{
			string name = this->addressToUsers[this->serverSocket->destIpAndPort()];
			string registerRequest = ServerIO::CreateMessage(ACCEPT_ALWAYS,name);
			this->tcpSock->sendInfo((char*)registerRequest.c_str());
			char buffer[50];
			this->tcpSock->reciveInfo(buffer);
			// msg is the user name
			if (ACCEPT_REQ_SUCCESS == ServerIO::ReadCommand(string(buffer)))
			{
				this->serverSocket->reply(ServerIO::CreateMessage(ACCEPT_REQ_SUCCESS, ""));
			}
			else
			{
				this->serverSocket->reply(ServerIO::CreateMessage(ACCEPT_REQ_FAILURE, ""));
			}
			break;
		}
		case REJECT_ALWAYS:
		{
			// [Accept Always Status Users]
			string name = this->addressToUsers[this->serverSocket->destIpAndPort()];
			string registerRequest = ServerIO::CreateMessage(REJECT_ALWAYS,name);
			this->tcpSock->sendInfo((char*)registerRequest.c_str());
			char buffer[50];
			this->tcpSock->reciveInfo(buffer);
			// msg is the user name
			if (REJECT_REQ_SUCCESS == ServerIO::ReadCommand(string(buffer)))
			{
				this->serverSocket->reply(ServerIO::CreateMessage(REJECT_REQ_SUCCESS, ""));
			}
			else
			{
				this->serverSocket->reply(ServerIO::CreateMessage(REJECT_REQ_FAILURE, ""));
			}
			break;
		}
		case STATUS_ACCEPT_ALL:
		{
			string name  = this->addressToUsers[this->serverSocket->destIpAndPort()];
			string acceptRequest = ServerIO::CreateMessage(STATUS_ACCEPT_ALL,name);
			char buffer[50];
			this->tcpSock->sendInfo((char*)acceptRequest.c_str());
			this->tcpSock->reciveInfo(buffer);
			this->serverSocket->reply(ServerIO::CreateMessage(STATUS_ACCEPT_ALL_RES, ServerIO::ReadMessage(buffer)));
			break;
		}
		case STATUS_REJECT_ALL:
		{
			string name  = this->addressToUsers[this->serverSocket->destIpAndPort()];
			string acceptRequest = ServerIO::CreateMessage(STATUS_REJECT_ALL,name);
			this->tcpSock->sendInfo((char*)acceptRequest.c_str());
			char buffer[50];
			this->tcpSock->reciveInfo(buffer);
			this->serverSocket->reply(ServerIO::CreateMessage(STATUS_REJECT_ALL_RES, ServerIO::ReadMessage(string(buffer))));
			break;
		}
		case GET_ALL_USERS:
		{
			char buffer[50];
			string getUsersRequest = ServerIO::CreateMessage(GET_ALL_USERS,""); // wrong password
			this->tcpSock->sendInfo((char*)getUsersRequest.c_str());
			this->tcpSock->reciveInfo(buffer);
			string users = ServerIO::ReadMessage(buffer);
			this->serverSocket->reply(ServerIO::CreateMessage(GET_ALL_USERS_RES, users));
			break;
		}
		case GET_ALL_CONNECTED_USERS:
		{
			vector<string> users;
			for (std::map<string,string>::iterator it=this->usersToAddress.begin(); it!=usersToAddress.end(); ++it)
			{
				users.push_back(it->first);
			}

			this->serverSocket->reply(ServerIO::CreateMessage(GET_ALL_CONNECTED_USERS_RES, ServerIO::FormatVector(users, ",")));
			break;
		}
		case GAME_IS_OVER:
		{
			this->disconnectBroker(this->serverSocket->destIpAndPort());
			string name = this->addressToUsers[this->serverSocket->destIpAndPort()]; //
			string what = msg == "WINNER" ? " WIN" : msg == "LOSSER" ? " LOSE" : " DRAW";
			this->tcpSock->sendInfo((char*)ServerIO::CreateMessage(REPORT_SCORE, name + what).c_str());
			char buff[50];
			this->tcpSock->reciveInfo(buff); // wait for respnse
			cout<<"[Server add new Score ] : "<<name<<" Status = "<<string(buff)<<endl;
			// Add Score to Table where msg == 'WINNER/LOSSER/DRAW' : TCP
			break;
		}
		case GET_SCORES:
		{
			this->tcpSock->sendInfo((char *)ServerIO::CreateMessage(GET_SCORES, "").c_str());
			char buffer[2000];
			this->tcpSock->reciveInfo(buffer);
			this->serverSocket->reply(string(buffer));
			break;
		}
	}
}
void UDPServer::setEstablishConnectionToBroker(string ipAndPort)
{
	vector<Broker*> allBrokers = this->brokers;
	for(unsigned int i = 0 ; i < allBrokers.size(); i++) {
		Broker * broker = allBrokers.at(i);
		if (broker->isInBroker(ipAndPort))
		{
			string name = this->addressToUsers[ipAndPort];
			if (broker->player1 == name
					|| broker->toString(broker->ipInitPlayer, broker->initPlayerPort) == name)
			{
				broker->player1ConfirmConnection = true;
			}
			else if (broker->player2 == name
					|| broker->toString(broker->ipOtherPlayer, broker->otherPlayerPort) == name)
			{
				broker->player2ConfirmConnection = true;
			}
			else
			{
				perror("ip and port are in broker but are not recognizable");
			}
		}
	}
}

bool UDPServer::isPlayerInBroker(string ipAndPortPlayer)
{
	vector<Broker*> allBrokers = this->brokers;
	for(unsigned int i = 0 ; i < allBrokers.size(); i++)
	{
		Broker *broker = allBrokers.at(i);
		if (broker->isInBroker(ipAndPortPlayer))
		{
			return true;
		}
	}
	return false;
}


void UDPServer::disconnectBroker(string ipAndPort)
{
	cout<<" disconnect Brokers] : "<<endl;
	vector<Broker*> allBrokers = this->brokers;
	int index = -1;
	for(unsigned int i = 0 ; i < allBrokers.size(); i++) {
		Broker *broker = allBrokers.at(i);
		if (broker->isInBroker(ipAndPort))
		{
			cout<<"[Broker Found At IP_PORT] : "<<ipAndPort<<endl;
			vector<string> ipAndPortSecond = Strings::split(broker->getSecondPlayer(ipAndPort), ":");
			string otherIP = ipAndPortSecond.at(0);
			int otherPort = ServerIO::FromStringToInt(ipAndPortSecond.at(1));
			this->serverSocket->sendTo(ServerIO::CreateMessage(EXIT_GAME, "quit game"),otherIP,otherPort);


			this->sendMsgToGameServer(GAME_REMOVE, broker->gameID());
			index = i;
			break;
		}
	}
	if (index > -1) {
		cout<<"[Broker Before erased broker] Current broker size before :"<<(this->brokers.size())<<endl;
		this->brokers.erase(this->brokers.begin() + index); // remove the index element
		cout<<"[Broker After erased broker] Current broker size after :"<<(this->brokers.size())<<endl;
	}
	cout<<"[";
}
bool UDPServer::containsBroker(Broker * broker) {
	vector<Broker*> allBrokers = this->brokers;
	for(unsigned int i = 0 ; i < allBrokers.size(); i++) {
		if (broker->isEqual(allBrokers.at(i)))
		{
			return true;
		}
	}
	return false;
}
