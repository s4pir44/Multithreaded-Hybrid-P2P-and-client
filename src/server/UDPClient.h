/*
 * UDPClient.h
 *
 *  Created on: Aug 18, 2017
 *      Author: demo
 */

#ifndef SERVER_UDPCLIENT_H_
#define SERVER_UDPCLIENT_H_
#include "../threads/MThread.h"
#include "../sockets/UDPSocket.h"
#include "../sockets/Strings.h"
#include "../sockets/TCPMessengerProtocol.h"
#include "../utils/ServerIO.h"
#include "../sockets/Types.h"
using namespace std;
#include "SingleClient.h"
#include "../gamexo/XOBoardViewer.h"
#include "../gamexo/GameProtocol.h"
#include "../Menus.h"
#include<vector>
#include<map>
#include <list>
class UDPClient: public MThread {
public:
	SingleClient * clientDetails;
	UDPSocket * clientSocket;
	bool isRunning;
	int status;
	bool isMyturn;
	bool waitForResponse;
	string username;
	string userRequest;
	bool acceptAll;
	bool rejectAll;
	bool waitForReplyFromPlayer;
	XOBoardViewer *board;
	int* getTable(string table, string delimiter);
	void displayBaordGame();
	bool isStepValid(int i, int j);
	bool sendMessageToServer(int command, string msg);
public:
	UDPClient(int port);
	virtual ~UDPClient();
	void run();
	void SetOtherPlayer(SingleClient* otherPlayer);
	void handleServerRequestsAndResponses(int command, string msg);
	void stopServer();
	bool sendMessage(string msg, string ip, int port);
	bool sendMessage2PeerPartner(string msg);
private:
	void HandlePeerRequest(const string& msg);
	void printAcceptRejectInstructions();
	void PrintUsers(const string& msg);
};

#endif /* SERVER_UDPCLIENT_H_ */
