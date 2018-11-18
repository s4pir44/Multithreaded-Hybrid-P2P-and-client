/*
 * UDPClientGame.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: demo
 */

#include "UDPClientGame.h"

// Client Command Options
#define OPEN_CLIENT "c"
#define LOGIN "login"
#define REGISTER "register"
#define QUIT "x"
#define CONNECTED_USERS "lcu"
#define ALL_USERS "lu"
#define PLAY_WITH_USER "r"
#define SEND_MESSAGE_TO_OPPONENT "s"
#define RANDOM_PLAYER_REQUEST "rp"
#define UNKNOWN_DATA "IGNORE_ME"
#define OTHER_PLAYER_REJECTED_YOU "reject"
#define OTHER_PLAYER_ACCEPTED_YOU "accept"
#define ENTER "\n"
#define MAKE_MOVE "m"
#define SEE_SCORE "scores"
UDPClientGame::UDPClientGame(Timer * timer) {
	this->client = NULL;
	this->isRunning = true;
	this->currentConversation = "unknown";
	this->user = "";
	inDebugMode = false;
	//int signal, void(*handler)(int), float secondsToSleep, float stopAfter);
	//this->timer = new Timer(SIGUSR1,Handler,0.1,20 );
	this->timer = timer;
	// run client to listen
	this->start();
}

UDPClientGame::~UDPClientGame() {
	if (this->client != NULL)
	{
		delete(this->client);
	}
	if (this->timer != NULL)
		delete(this->timer);
}

void UDPClientGame::timerHandler(int signal)
{
	if (this->client->waitForReplyFromPlayer)
	{
		this->client->waitForResponse = true;
		// passed over 20 seconds quit wait state & send Cancel to server
		this->client->waitForReplyFromPlayer = false;
		cout<<"20 seconds passed : response was not sent. Cancel request"<<endl;

		// CANCEL REQUEST!
	}
	cout<<"Reached end goal ! "<<endl;
}

bool UDPClientGame::isConnected() {
	return this->client != NULL && this->client->status != DISCONNECTED;
}

bool UDPClientGame::isLoggedIn()
{
	return this->isConnected() && this->client->status != AUTH;
}

bool UDPClientGame::disconnect()
{
	this->isRunning = false;

	if (!isLoggedIn())
	{
		if (this->client != NULL)
		{
			this->client->stopServer(); // delete it
			cout<<"disconnecting client socket (udp) ";
		}
		cout<<"user is not connected"<<endl;
		return false;
	}
	bool result = false;
	if (this->client->status != BROKER)
	{
		// working in game server
		result =  this->sendMessageToMessengerServer(DISCONNECT_FROM_CLIENT, this->user);
	}
	else
	{
		result =  this->sendMessageToMessengerServer(DISCONNECT_FROM_CLIENT, this->user);
		result =  this->sendMessageToGameServer(DISCONNECT_FROM_CLIENT, this->user);
	}
	this->client->stopServer();
	this->client->waitForThread();
	if (this->client != NULL)
	{
		delete(this->client);
	}

	return result;

}

// only if loggedin => client main will ask if loggedin then : assume looged in
bool UDPClientGame::getAllUsers()
{
	try
	{
		return this->sendMessageToMessengerServer(PRINT_USERS_REQ, "All"); // all users
		return true;
	}
	catch(const char * e)
	{
		perror(e);
		return false;
	}
}

bool UDPClientGame::getAllConectedUsers()
{
	try
		{
			return this->sendMessageToMessengerServer(PRINT_USERS_REQ, "Connected"); // all users
			return true;
		}
		catch(const char * e)
		{
			perror(e);
			return false;
		}
}

bool UDPClientGame::sendMessageToMessengerServer(int command, string msg)
{
	try
	{
		return this->client->sendMessage(ServerIO::CreateMessage(command, msg),MESSENGER_SERVER_IP ,MESSENGER_SERVER_PORT);
	}
	catch(const char * e)
	{
		perror(e);
		return false;
	}
}

bool UDPClientGame::sendMessageToGameServer(int command, string msg)
{
	try
	{
		return this->client->sendMessage(ServerIO::CreateMessage(command, msg),UDP_GAME_SERVER_IP ,UDP_GAME_SERVER_PORT);
	}
	catch(const char * e)
	{
		perror(e);
		return false;
	}
}
bool UDPClientGame::checkNumberOfCorrectArguments(string input, unsigned int argNum)
{
	return Strings::split(input, " ").size() == argNum;
}
bool UDPClientGame::isNumberInRange(string input, unsigned int start, unsigned int end)
{
	if (input.find_first_not_of("0123456789") == string::npos)
	{
		// is a number
		unsigned int i = ServerIO::FromStringToInt(input);
		return start <= i && i <= end;
	}
	else {
		return false;
	}
}
void UDPClientGame::PrintInstructionByStatus(int status) {
	// print instructions
	switch (status)
	{
		case DISCONNECTED:
		{
			Menus::PrintMenu(1);
			break;
		}
		case AUTH:
		{
			Menus::PrintMenu(2);
			break;
		}
		case DISPATCHER:
		{
			Menus::PrintMenu(3);
			break;
		}
		case WAIT_FOR_OPPONENT_ADDR:
		{
			Menus::PrintMenu(4);
			break;
		}
		case BROKER:
		{
			if (this->client->isMyturn)
				Menus::PrintMenu(6); // + m move
			else
				Menus::PrintMenu(5);
			break;
		}
	}
	if (this->isLoggedIn())
	{
		cout<<"You are "<<this->client->username<<endl;
	}

}


string UDPClientGame::getCommand(string com)
{
	if (this->isInDispatch())
	{
		string answer = com;
		if (answer == "a") {
			this->client->clientSocket->reply(ServerIO::CreateMessage(CHALLENGE_ACCEPTED, this->client->userRequest));
			this->client->status = WAIT_FOR_OPPONENT_ADDR;
			return OTHER_PLAYER_ACCEPTED_YOU;
		} else if (answer == "aa") {
			//CHALLENGE_ACCEPTED : server takes the ip and port...
			this->client->clientSocket->reply(ServerIO::CreateMessage(CHALLENGE_ACCEPTED, this->client->userRequest));
			this->client->clientSocket->reply(ServerIO::CreateMessage(ACCEPT_ALWAYS, this->client->userRequest));
			this->client->status = WAIT_FOR_OPPONENT_ADDR;
			return OTHER_PLAYER_ACCEPTED_YOU;
		} else if (answer == "r") {
			this->client->clientSocket->reply(ServerIO::CreateMessage(SESSION_REFUSED, this->client->userRequest));
			return OTHER_PLAYER_REJECTED_YOU;
		} else if (answer == "rr") {
			this->client->clientSocket->reply(ServerIO::CreateMessage(SESSION_REFUSED, this->client->userRequest));
			this->client->clientSocket->reply(ServerIO::CreateMessage(REJECT_ALWAYS, this->client->userRequest));
			return OTHER_PLAYER_REJECTED_YOU;
		}
	}
	return Strings::split(com," ").at(0);
}

string UDPClientGame::getData(string msg)
{
	string com = this->getCommand(msg);
	unsigned int fromHere = com.size() + 1; // +1 because of space
	if(fromHere >= msg.size())
	{
		return UNKNOWN_DATA;
	}
	return msg.substr(fromHere, msg.size());
}

bool UDPClientGame::isAuth()
{
	return this->isConnected() && this->client->status == AUTH;
}

bool UDPClientGame::isInDispatch() {
	return this->isConnected() && this->client->status == DISPATCHER;
}

bool UDPClientGame::isInBroker()
{
	return this->isConnected() && this->client->status == BROKER;
}

bool UDPClientGame::isWaitingForOppAddr()
{
	return this->isConnected() && this->client->status == WAIT_FOR_OPPONENT_ADDR;
}

void UDPClientGame::connect(const string& command) {
	if (!this->isConnected()) {
		int port = ServerIO::FromStringToInt(this->getData(command));
		this->client = new UDPClient(port);
		this->client->waitForResponse = true;
	} else {
		perror("you are already connected");
	}
}

void UDPClientGame::run() {

	this->isRunning = true;
	bool shouldPrint = true;
	while(this->isRunning)
	{
		while(!inDebugMode)
		{
			if (this->client == NULL)
				{
					// open session
					shouldPrint = true;
				}
				else
				{
					shouldPrint = this->client->waitForResponse;
				}
				if (shouldPrint)
				{
					// print instructions
					if (this->client != NULL)
					{
						PrintInstructionByStatus(this->client->status);
					}
					else
					{
						PrintInstructionByStatus(DISCONNECTED);
					}
					string command;
					getline(cin, command);
					string com = this->getCommand(command);
					if (com == OPEN_CLIENT)
					{
						string portNum = this->getData(command);
						if(this->checkNumberOfCorrectArguments(portNum, 1)
								&& this->isNumberInRange(portNum, 1024,65536))
						{
							connect(command);
						}
						else {
							perror("Invalid input, please try again.");
						}

					}
					else if (com == LOGIN)
					{
						if (isAuth())
						{
							string userAndPass = this->getData(command);
							if (this->checkNumberOfCorrectArguments(userAndPass, 2))
							{
								this->client->waitForResponse = false;
								this->sendMessageToMessengerServer(LOGIN_REQ, userAndPass);
							}
							else{
								perror("Invalid input, please try again.");
							}

						}
						else
						{
							perror("you are already logged");
						}
					}
					else if (com == REGISTER)
					{
						if (isAuth())  {
							string userAndPass = this->getData(command);
							if (this->checkNumberOfCorrectArguments(userAndPass, 2))
							{
								this->client->waitForResponse = false;
								this->sendMessageToMessengerServer(REGISTER_REQ, userAndPass);
							}
							else{
								perror("Invalid input, please try again.");
							}
						} else {
							perror("you are already registered");
						}

					} else if (com == PLAY_WITH_USER)
					{
						if (this->isInDispatch())
						{
							string playWith = this->getData(command);

							if (this->checkNumberOfCorrectArguments(playWith, 1))
							{	this->client->waitForResponse = false;
								if (UNKNOWN_DATA == playWith) {
									continue;
								}

								this->client->userRequest = playWith;
								this->sendMessageToMessengerServer(SESSION_REQ_PLAY_WITH_USER, playWith);
								this->client->waitForReplyFromPlayer = true;
								if (this->timer != NULL)
									this->timer->startTimer();
								cout<<"Wait for response..."<<endl;
							}
							else{
								perror("Invalid input, please try again.");
							}

						} else {
							perror("you are already playing or must login first");
						}
					}
					else if (com == RANDOM_PLAYER_REQUEST) {
						if (this->isInDispatch())
						{
							this->client->waitForResponse = false;
							this->sendMessageToMessengerServer(PLAY_WITH_RANDOM_PLAYER, this->user);
							cout<<"Wait for response..."<<endl;
						}

					}
					else if (com == QUIT) {
						cout<<"disconnecting..."<<endl;
						this->disconnect();
						break;
					} else if (com == CONNECTED_USERS) {
						if (this->isInDispatch())
						{
							this->client->waitForResponse = false;
							this->sendMessageToMessengerServer(GET_ALL_CONNECTED_USERS, "");
							sleep(2);
						} else {
							perror("you must be logged in in order to make this action");
						}
					} else if (com == ALL_USERS) {
						if (this->isInDispatch())
						{
							this->client->waitForResponse = false;
							this->sendMessageToMessengerServer(GET_ALL_USERS, "");
							sleep(2);
						} else {
							perror("you must be logged in in order to make this action");
						}

					}
					else if (com == OTHER_PLAYER_REJECTED_YOU) {
						// handled
						cout<<"you refused to play with other player"<<endl;
					} else if (com == OTHER_PLAYER_ACCEPTED_YOU){
						//handled
						cout<<"you accepted to play with other player"<<endl;
					} else if (com == SEND_MESSAGE_TO_OPPONENT) {
						if (this->isInBroker())
						{
							string msg = this->getData(command);
							this->client->sendMessage2PeerPartner(msg);
						}
						else
						{
							perror("you must be playing in order to make this action");
						}
					}
					else if (com == MAKE_MOVE)
					{
						string realMsg = this->getData(command);

						if (Strings::split(realMsg,",").size() == 2)
						{
							vector<string> moves = Strings::split(realMsg, ",");

							if (this->isNumberInRange(moves.at(0), 1,3)
									&& this->isNumberInRange(moves.at(1), 1,3)) {
								int i = ServerIO::FromStringToInt(moves.at(0)) - 1;
								int j = ServerIO::FromStringToInt(moves.at(1)) - 1;
								if (this->client->isStepValid(i,j))
								{
									string make_move = ServerIO::FromIntToString(i,0) + "," + ServerIO::FromIntToString(j,0);
									this->sendMessageToGameServer(GAME_MAKE_MOVE,make_move);
								}
								else{
									cout<<"Step is invalid please type again..."<<endl;
									Menus::PrintMenu(6);
								}
							}
							else {
								perror("Invalid move state, moves must be (1<=i<=3, 1<=j<=3). Press enter to continue");
							}

						} else {
							perror("invalid input, type again");
						}
					}
					else if (com == SEE_SCORE)
					{
						this->sendMessageToMessengerServer(GET_SCORES,"");
					}
					else if (ENTER) {
						// ignored...
					}
					else
					{
						cout<<"unknown command"<<endl;
					}
				} // end if
				else {
					sleep(0.2);
				}
		}
	}
	cout<<"Goodbye"<<endl;
}




