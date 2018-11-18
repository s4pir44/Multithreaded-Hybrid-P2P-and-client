/*
 * MainTester.cpp
 *
 *  Created on: Aug 24, 2017
 *      Author: demo
 */

#include "MainTester.h"
template<class T>
bool MainTester::assertFalse(T arg1, T arg2) {
	return arg1 != arg2;
}

template<class T>
bool MainTester::assertTrue(T arg1, T arg2) {
	return arg1 == arg2;
}
void MainTester::deleteFile(string filename)
{
	ifstream ifile(filename.c_str());
	if (ifile)
	{
		try {
			 remove(filename.c_str());
		}
		catch(const char * e) {
			perror("Failed to delete users file");
		}
	}
}
void MainTester::ResetInitialStateDB()
{
	deleteFile("users");
	deleteFile("acceptAll");
	deleteFile("rejectAll");

	Users::registerUsers("a","a");
	Users::registerUsers("b","b");
	Users::registerUsers("c","c");
	Users::registerUsers("d","d");

	AcceptAll::AddNewUser("a");


}
void MainTester::runAllTests()
{
	UnitTest * gameTest = new UnitTest("Game Flow Tester");
	gameTest->AddTest("Game",gameRequests );

	UnitTest * tcpTest = new UnitTest("TCP_DB_SERVER_TEST");
	// save as temp files
	// a,b,c,d register in users, and a in accept all
	tcpTest->AddTest("NOT IMPELEMENTED YET game Requests", gameRequests);
	tcpTest->AddTest("accept Always", acceptAlways);
	tcpTest->AddTest("get users", getUsers);
	tcpTest->AddTest("login", login);
	tcpTest->AddTest("register", registerUsers);
	tcpTest->AddTest("reject Always", rejectAlways);

	gameTest->RunTest("Game");
	//tcpTest->RunAllTests(true);

	ResetInitialStateDB();
}
#include "../server/XOHandler.h"
bool MainTester::gameRequests()
{
	GameServer* gs = new GameServer(13457);
	sleep(3);
	UDPClientGame *c1 = new UDPClientGame(NULL);
	UDPClientGame *c2 = new UDPClientGame(NULL);
	c1->inDebugMode = true;
	c2->inDebugMode = true;
	c1->connect("c 4097");
	c2->connect("c 6899");
	//ip1:port1:user1 ip2:port2:user2
	c1->client->clientSocket->sendTo(ServerIO::CreateMessage(GAME_CREATE,
			"127.0.0.1:4097:a 127.0.0.1:6899:b"), "127.0.0.1", 13457);
	c1->client->username = "c1";
	c2->client->username = "c2";

	c1->client->userRequest = "c2";
	c2->client->userRequest = "c1";


	while(true)
	{
		if (c2->client->isMyturn)
		{
			cout<<"choose_move <i,j>";
			cout<<" You are c2"<<endl;
			string command;
			getline(cin, command);
			cout<<command<<" has been chosen! c2"<<endl;
			if(Strings::split(command,",").size() != 2)
			{
				cout<<"wrong input"<<endl;
				continue;
			}
			c2->client->clientSocket->sendTo(ServerIO::CreateMessage(GAME_MAKE_MOVE,
					command), "127.0.0.1", 13457);
			cout<<"c2 sent the command "<<command<<endl;
			c2->client->isMyturn = false;
		}
		else
			if (c1->client->isMyturn)
			{
			cout<<"choose_move <i,j> ";
			cout<<"You are c1"<<endl;
			string command;
			getline(cin, command);
			if(Strings::split(command,",").size() != 2)
			{
				cout<<"wrong input"<<endl;
				continue;
			}
			c1->client->clientSocket->sendTo(ServerIO::CreateMessage(GAME_MAKE_MOVE,
					command), "127.0.0.1", 13457);
			c1->client->isMyturn = false;
			cout<<"c1 sent the command "<<command<<endl;
		}

	}

	gs->waitForThread();

	gs->stopServer();
	c1->disconnect();
	c2->disconnect();
	delete(gs);
	delete(c1);
	delete(c2);
	return true;
}
bool MainTester::rejectAlways()
{
	char ip[] = "127.0.0.1";
	int port = 4099;
	char buffer[100];
	bool b = false;
	cout<<"Reject start"<<endl;
	ResetInitialStateDB();

	ITCPServerHandler * handler = new ITCPServerHandler();
	ServerTCP * server = new ServerTCP(port,ip, handler);
	sleep(3);
	Socket *client = new ClientTCP(port, ip);

	// [Accept Always Status Users]
	string acceptRequest = ServerIO::CreateMessage(STATUS_REJECT_ALL,"b");
	client->sendInfo((char*)acceptRequest.c_str());
	client->reciveInfo(buffer);
	string msg = ServerIO::ReadMessage(buffer);
	b = MainTester::assertTrue<string>(msg, "false"); // only 4 users attached

	// [Accept Always Status Users]
	acceptRequest = ServerIO::CreateMessage(REJECT_ALWAYS,"b");
	client->sendInfo((char*)acceptRequest.c_str());
	client->reciveInfo(buffer);
	msg = ServerIO::ReadMessage(buffer);
	b = MainTester::assertTrue<string>(msg, "true"); // only 4 users attached

	string registerRequest = ServerIO::CreateMessage(STATUS_REJECT_ALL,"b");
	client->sendInfo((char*)registerRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	b = b | MainTester::assertTrue<string>(msg, "true");
	cout<<"Reject end"<<endl;
	server->stopServer();
	delete(server);
	delete(client);
	delete(handler);
	return b;
}

void MainTester::dismissAll(ITCPServerHandler * handler, ServerTCP *server, Socket *client)
{
	server->stopServer();
	delete(server);
	delete(client);
	delete(handler);
}
bool MainTester::acceptAlways()
{
	// ACCEPT_ALWAYS
	char ip[] = "127.0.0.1";
	int port = 5678;
	char buffer[100];
	bool b = false;
	cout<<"Accept start"<<endl;
	ResetInitialStateDB();

	ITCPServerHandler * handler = new ITCPServerHandler(); // deleted already
	ServerTCP * server = new ServerTCP(port,ip, handler);
	sleep(3);
	Socket *client = new ClientTCP(port, ip);
	//Timer * t = new Timer(0.1);
	//t->startTimer();
	// [Accept Always Status Users]
	string acceptRequest = ServerIO::CreateMessage(STATUS_ACCEPT_ALL,"a");
	client->sendInfo((char*)acceptRequest.c_str());
	client->reciveInfo(buffer);
	string msg = ServerIO::ReadMessage(buffer);
	b = MainTester::assertTrue<string>(msg, "true"); // only 4 users attached

	// [Accept Always Status Users]
	acceptRequest = ServerIO::CreateMessage(STATUS_ACCEPT_ALL,"b");
	client->sendInfo((char*)acceptRequest.c_str());
	client->reciveInfo(buffer);
	msg = ServerIO::ReadMessage(buffer);
	b = MainTester::assertTrue<string>(msg, "false"); // only 4 users attached

	string registerRequest = ServerIO::CreateMessage(ACCEPT_ALWAYS,"b"); // user correct
	client->sendInfo((char*)registerRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	b = b | MainTester::assertTrue<string>(msg, "true");

	// [Accept Always Status Users]
	acceptRequest = ServerIO::CreateMessage(STATUS_ACCEPT_ALL,"b");
	client->sendInfo((char*)acceptRequest.c_str());
	client->reciveInfo(buffer);
	msg = ServerIO::ReadMessage(buffer);
	b = MainTester::assertTrue<string>(msg, "true"); // only 4 users attached

	registerRequest = ServerIO::CreateMessage(ACCEPT_ALWAYS,"b"); // user correct
	client->sendInfo((char*)registerRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	//t->stopTimer();
	b = b | MainTester::assertTrue<string>(msg, "false"); // already in !

	//cout<<"it took : "<<ServerIO::FromFloatToString(t->getTotalRunTime(),2)<<" ms"<<endl;
	server->stopServer();

	cout<<"Accept end"<<endl;
	delete(server);
	delete(client);
	delete(handler);
	//delete(t);
	return b;
}

bool MainTester::getUsers()
{
	// abcd
	char ip[] = "127.0.0.1";
	int port = 13306;
	char buffer[100];
	bool b = false;
	cout<<"get users"<<endl;
	ResetInitialStateDB();

	ITCPServerHandler * handler = new ITCPServerHandler();
	ServerTCP * server = new ServerTCP(port,ip, handler);
	sleep(3);
	Socket *client = new ClientTCP(port, ip);

	Timer * t = new Timer(0.1);
	t->startTimer();

	// [Get Users]
	string getUsersRequest = ServerIO::CreateMessage(GET_ALL_USERS,""); // wrong password
	client->sendInfo((char*)getUsersRequest.c_str());
	client->reciveInfo(buffer);
	string msg = ServerIO::ReadMessage(buffer);
	b = MainTester::assertTrue<string>(msg, "a,b,c,d"); // only 4 users attached

	string registerRequest = ServerIO::CreateMessage(REGISTER_REQ,"p a"); // user correct
	client->sendInfo((char*)registerRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	b = b | MainTester::assertTrue<string>(msg, "a,b,c,d,p");

	t->stopTimer();
	cout<<"it took : "<<ServerIO::FromFloatToString(t->getTotalRunTime(),2)<<" ms"<<endl;

	cout<<"get users end"<<endl;
	server->stopServer();
	delete(server);
	delete(client);
	delete(handler);
	return b;

}
bool MainTester::registerUsers()
{
	char ip[] = "127.0.0.1";
	int port = 13308;
	char buffer[100];
	bool b = false;
	cout<<"register"<<endl;
	ResetInitialStateDB();

	ITCPServerHandler * handler = new ITCPServerHandler();
	ServerTCP * server = new ServerTCP(port,ip, handler);
	sleep(3);
	Socket *client = new ClientTCP(port, ip);

	Timer * t = new Timer(0.01);
	t->startTimer();

	// [Register]
	string registerRequest = ServerIO::CreateMessage(REGISTER_REQ,"a b"); // wrong password
	client->sendInfo((char*)registerRequest.c_str());
	client->reciveInfo(buffer);
	string msg = ServerIO::ReadMessage(buffer);
	b = MainTester::assertTrue<string>(msg, "false"); // user already exists!

	registerRequest = ServerIO::CreateMessage(REGISTER_REQ,"q a"); // user correct
	client->sendInfo((char*)registerRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	b = b | MainTester::assertTrue<string>(msg, "true");

	string loginRequest = ServerIO::CreateMessage(LOGIN_REQ,"q a"); // user exists and able to login but not register
	client->sendInfo((char*)loginRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	b = b | MainTester::assertTrue<string>(msg, "true");

	registerRequest = ServerIO::CreateMessage(REGISTER_REQ,"q a"); // user now exists
	client->sendInfo((char*)registerRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	b = b | MainTester::assertTrue<string>(msg, "false");

	t->stopTimer();
	cout<<"it took : "<<ServerIO::FromFloatToString(t->getTotalRunTime(),2)<<" ms"<<endl;
	server->stopServer();

	cout<<"register end"<<endl;

	delete(server);
	delete(client);
	delete(handler);
	return b;
}
bool MainTester::login()
{
	char ip[] = "127.0.0.1";
	int port = 13309;
	char buffer[100];
	bool b = false;
	cout<<"login"<<endl;
	ResetInitialStateDB();

	ITCPServerHandler * handler = new ITCPServerHandler();
	ServerTCP * server = new ServerTCP(port,ip, handler);
	sleep(3);
	Socket *client = new ClientTCP(port, ip);

	// [Login]
	string loginRequest = ServerIO::CreateMessage(LOGIN_REQ,"a b"); // wrong password
	client->sendInfo((char*)loginRequest.c_str());
	client->reciveInfo(buffer);
	string msg = ServerIO::ReadMessage(buffer);
	b = MainTester::assertTrue<string>(msg, "false");

	loginRequest = ServerIO::CreateMessage(LOGIN_REQ,"a a"); // user correct
	client->sendInfo((char*)loginRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	b = b | MainTester::assertTrue<string>(msg, "true");

	loginRequest = ServerIO::CreateMessage(LOGIN_REQ,"q a"); // user does not exists
	client->sendInfo((char*)loginRequest.c_str());
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	client->reciveInfo(buffer);
	b = b | MainTester::assertTrue<string>(msg, "false");

	server->stopServer();
	cout<<"login end"<<endl;
	delete(server);
	delete(client);
	delete(handler);
	return b;
}
