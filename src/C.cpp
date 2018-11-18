//============================================================================
// Name        : C.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include <unistd.h>
#include<csignal>
#include "utils/ServerIO.h"
#include "server/UDPServer.h"
#include "server/UDPClient.h"
#include "sockets/TCPServer.h"
#include "sockets/TCPClient.h"
#include "sockets/TCPMessengerProtocol.h"
#include "centralDirectory/Users.h"
#include "tcp/ClientTCP.h"
#include "tcp/ServerTCP.h"
#include "client/UDPClientGame.h"
#include <fstream>
#include "unittests/UnitTest.h"
#include "unittests/Assert.h"
#include "timers/Timer.h"
#include "tcp/IHandler.h"
#include "tcp/ITCPServerHandler.h"
#include <sstream> // stringstream
#include "tests/MainTester.h"
#include "server/GameServer.h"

void MakeTest(){
		UDPServer * server = new UDPServer(1327);
		UDPClient * client = new UDPClient(4097);
		UDPClient * client2 = new UDPClient(6899);


		string login1 = ServerIO::CreateMessage(LOGIN_REQ, "amir 1234");
		string login2 = ServerIO::CreateMessage(LOGIN_REQ, "dana 1A2E3r4");

		client->clientSocket->sendTo(login1, "127.0.0.1",server->serverSocket->getPort());
		client2->clientSocket->sendTo(login2, "127.0.0.1",server->serverSocket->getPort());

		sleep(60);
		client->stopServer();
		server->stopServer();
}
void TestUsers() {
	Users::load();
	bool b = Users::registerUsers("amir","A123$");
	cout<<"registered success = "<<b<<endl;
	cout<<"isUserExists success = "<<Users::isUserExists("amir")<<endl;
	cout<<"login Should be false success = "<<Users::login("amir","A123")<<endl;
	cout<<"login Should be true success = "<<Users::login("amir","A123$")<<endl;
	cout<<Users::getAllUsers()<<endl;
	string s;
	cin>>s;
}
bool TestServerTcp()
{
	char ip[] = "127.0.0.1";
	ITCPServerHandler * handler = new ITCPServerHandler();
	Socket * server = new ServerTCP(13304,ip, handler);
	sleep(3);
	Socket *client = new ClientTCP(13304, ip);

	string s = ServerIO::CreateMessage(LOGIN_REQ,"a b");
	client->sendInfo((char*)s.c_str());
	char buffer[4096];
	client->reciveInfo(buffer);
	string msg = ServerIO::ReadMessage(buffer);
	cout<<endl<<"[Client Got]"<<msg<<endl;

	delete(client);
	delete(server);
	sleep(60);
	return true;
}
#include <iostream>
#include <cstdlib>
#include <time.h>
const int LOW = 1;
const int HIGH = 6;
void F87()
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

	cout<<rand() % (HIGH - LOW + 1) + LOW<<endl;
}
bool TestTcpClients() {

	char ip[] = "127.0.0.1";




	Socket *server = new ServerTCP(13308, ip, NULL);

	Socket *client = new ClientTCP(13308, ip);
	//Socket *client = new ClientUDP(PORT_SERVER_UDP, ip);
	//char message[] = "Do you wanna build a snow man?";
	string s = ServerIO::CreateMessage(1,"Do you wanna build a snow man?");
	client->sendInfo((char*)s.c_str());

	char buffer[4096];
	server->reciveInfo(buffer);
	cout<<buffer<<endl;
	delete(server);
	delete(client);
	return Assert::assertTrue<string>("Do you wanna build a snow man?", buffer);


	//TCPServer *server = new TCPServer(13301);
	//TCPSocket *client = new TCPSocket("127.0.0.1", 13301);

	//server->server->send("hello", 5);
	//char * buff = new char[2000];
	//client->recv(buff,2000);
	//cout<<buff;
	//ClientTCP * c1 = new ClientTCP(14409, (char*) "127.0.0.1");
//	ClientTCP * c2 = new ClientTCP(13302, (char*) "127.0.0.1");
//
//	char * buff = new char[2000];
//	c1->sendInfo((char*)"attempt");
//	c2->reciveInfo(buff);
//	cout<<"c2 got : "<<buff<<endl;
//	client1->client->send(buff, 7);
//	client1->client->send("hi", 2);



}
/*
 * Handles termination signals for cleanups
 * */
UDPClient * globalClient = NULL;
void handleSignal(int signal) {
	try {
		if (globalClient != NULL) {
			globalClient->stopServer();
		}
	} catch(int e) {
		perror("Failed to handle signal");
	}
	// kill with correct exit code
	exit(signal);
}
void printInstructions(int status)
{
	switch(status) {
	case DISCONNECTED:
		cout<<"c <port> - connect to the server with the given port"<<endl;
		break;
	case AUTH:
		cout<<"login <user> <password> - login with the user and password"<<endl;
		cout<<"register <user> <password> - register the new user with password"<<endl;
		break;

	case WAIT_FOR_PLAY:
		cout<<"lu - print the user list from the server"<<endl;
		cout<<"lcu - print the connected user list"<<endl;
		cout<<"r <user> - request to play with a specific user"<<endl;
		cout<<"rr <user> - request to play with a random user"<<endl;
		break;
	case BROKER:
		cout<<"s <message> - send a message to the user"<<endl;
		break;
	// TODO : Game scores, Games Moves!!
	}
	if (status != DISCONNECTED)
	{
		cout<<"accept all - accept all user requests automatically"<<endl;
		cout<<"reject all - reject all user requests automatically"<<endl;
		cout<<"cs - disconnect the open session(user) / exit game"<<endl;
		cout<<"d - disconnect the server"<<endl;
	}


	cout<<"x - close the application"<<endl;
}
void TestInstruction(){
	printInstructions(DISCONNECTED);
		sleep(2);
		cout<<""<<endl;
		cout<<""<<endl;
		printInstructions(AUTH);
		sleep(2);
		cout<<""<<endl;
		cout<<""<<endl;
		printInstructions(WAIT_FOR_PLAY);
		sleep(2);
		cout<<""<<endl;
			cout<<""<<endl;
		printInstructions(BROKER);
		sleep(2);
		cout<<""<<endl;
		cout<<""<<endl;
}

void TestClient()
{
	UDPServer * server = new UDPServer(MESSENGER_SERVER_PORT);
	UDPClientGame * client = new UDPClientGame(NULL);

	client->waitForThread();
	server->stopServer();
	cout<<"finished"<<endl;
}
#define MESSENGER_SERVER 1
#define MESSENGER_FILE_NAME "msgServerTempFile.txt"
bool fexists(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
}
bool IsServerUp(int server)
{
	if (server == MESSENGER_SERVER)
	{
		return fexists(MESSENGER_FILE_NAME);
	} else {
		return false;
	}

}
void DeleteServerFile(const char * filename)
{
	 if( remove( filename ) != 0 )
	    perror( "Error deleting file" );
}
void CreateFile(const char * filename)
{
	ofstream outputFile;
	outputFile.open(filename);
	outputFile.close();
}
template<class T>
bool Assert::assertFalse(T arg1, T arg2) {
	return arg1 != arg2;
}

template<class T>
bool Assert::assertTrue(T arg1, T arg2) {
	return arg1 == arg2;
}


bool isXY(int x, int y, int z){
	return Assert::assertTrue<int>(x+y,z);
}
bool TT() {
	return isXY(1,2,3);
}
bool TT2() {
	return isXY(1,2,4);
}
bool Timer1()
{
	Timer * t = new Timer(0.1 );
	t->startTimer();
	sleep(3);
	t->stopTimer();
	string s = ServerIO::FromFloatToString(t->getTotalRunTime(),1);
	bool b0 = Assert::assertTrue<string>(s, "2.9");
	bool b1 = Assert::assertTrue<string>(s, "3.0");
	bool b2 =  Assert::assertTrue<string>(s, "3.1");
	delete(t);
	return b0 || b1 || b2;
}
void handler(int signal)
{
	cout<<"Reached end goal ! "<<endl;
}
bool Stopper()
{
	Timer * t = new Timer(SIGUSR1,handler,0.1,3 );
	t->startTimer();
	t->waitForThread();
	string s = ServerIO::FromFloatToString(t->getTotalRunTime(),1);
	bool b1 = Assert::assertTrue<string>(s, "3.0");
	bool b2 =  Assert::assertTrue<string>(s, "3.1");
	delete(t);
	return b1 || b2;
}

bool TcpChcek()
{
	TCPSocket *server = new TCPSocket(4098);
	TCPSocket *client = new TCPSocket("127.0.0.1", 4098);
	TCPSocket *client2 = new TCPSocket("127.0.0.1", 4098);
	bool b0 = Assert::assertFalse<string>(client->destIpAndPort(), "127.0.0.1:4098");
	b0 = b0 && Assert::assertFalse<string>(client2->destIpAndPort(), "127.0.0.1:4098");
	client->send("hello",5);
	char * buff = new char[2000];
	server->recv(buff, 2000);
	b0 = b0 && Assert::assertFalse<string>(buff, "hello");
	server->send("hi",2);
	char * b1 = new char[100];
	char * b2 = new char[100];
	client->recv(b1, 2000);
	client2->recv(b2, 2000);
	b0 = b0 && Assert::assertFalse<string>(b1, "hi");
	b0 = b0 && Assert::assertFalse<string>(b2, "hi");
	delete(server);
	delete(client);
	return b0;
}

UDPClientGame * gameClient = NULL;
void handleSignalForClient(int sig)
{
	if (gameClient != NULL)
		gameClient->timerHandler(sig);
	cout<<"Call game client function"<<endl;
}

int main() {

	//MainTester::runAllTests();

	//while(true);
	//TestTcpClients();
	//TestServerTcp();
//
//	UnitTest * unit = new UnitTest("test1");
//
//	unit->AddTest("isZY", TT);
//	unit->AddTest("isZYFail", TT2);
//
//	//unit->RunAllTests(true);
//
	//UnitTest * unit2 = new UnitTest("test timer");
//
	//unit2->AddTest("Stopper", Stopper);
//	//unit2->AddTest("Timer",Timer1);
	//unit2->AddTest("TCP",TestTcpClients);
	//unit2->RunAllTests(true);
//	cout<<"Done..."<<endl;



	// Register SIGTERM,SIGABRT, SIGINT signals
//	signal(SIGTERM,handleSignal);
//	signal(SIGABRT,handleSignal);
//	signal(SIGINT,handleSignal);
//	bool isRunning = true;
//	printInstructions(DISCONNECTED);
//	while(isRunning)
//	{
//		string command;
//		cin >> command;
//
//		if (command == "x")
//		{
//			if (globalClient != NULL)
//			{
//				if (DISCONNECTED != globalClient->status &&  globalClient->status != AUTH)
//				{
//					globalClient->stopServer();
//				}
//			}
//			isRunning = false;
//		}
//	}
//	cout<<"GoodBye!"<<endl;
	//MakeTest();
	//TestUsers();
	//TestTcpClients();
	string s;
	cout<<"Choose who to run"<<endl;
	cout<<"To run Messenger Server type 's'"<<endl;
	cout<<"To run Messenger Client type 'c'"<<endl;
	string command;
	getline(cin, s);
	cin.clear();
	if(s == "s")
	{
		if (IsServerUp(MESSENGER_SERVER))
		{
			perror("Server is already up");
		}
		else
		{
			CreateFile(MESSENGER_FILE_NAME);
			char ip[] = "127.0.0.1";
			MainTester::ResetInitialStateDB();
			// DB TCP SERVER
			ITCPServerHandler * handler = new ITCPServerHandler();
			ServerTCP * servertcpDB = new ServerTCP(TCP_SERVER_DB_PORT,ip, handler);
			cout<<"Server TCP DB is on Air"<<endl;
			GameServer* gs = new GameServer(UDP_GAME_SERVER_PORT);
			cout<<"Please wait 3 seconds before running any client 3 2 1"<<endl;
			UDPServer * server = new UDPServer(MESSENGER_SERVER_PORT);
			sleep(3);
			cout<<"Server is running - "<<MESSENGER_SERVER<<endl;
			cout<<"To Stop server press 'quit'"<<endl;
			string ans;
			cin>>ans;
			while(ans != "quit")
			{
				cout<<"To Stop server press 'quit'"<<endl;
				cin>>ans;
			}
			servertcpDB->stopServer();
			server->stopServer();
			gs->stopServer();
			cout<<"[Server] deleting file"<<endl;
			DeleteServerFile(MESSENGER_FILE_NAME);
			cout<<"[Server is Down]"<<endl;

			delete(servertcpDB);
			delete(server);
			delete(gs);
		}
	}
	else if (s == "c")
	{
		Timer * t = new Timer(SIGUSR1, handleSignalForClient, 0.1, 20);

		UDPClientGame * client = new UDPClientGame(t);

		gameClient = client;

		client->waitForThread();
		cout<<"[Client is Down]"<<endl;
		delete(client);

	}
	else {
		cout<<"Wrong action goodbye"<<endl;
	}
	cout<<"finished"<<endl;
	return 0;
}
