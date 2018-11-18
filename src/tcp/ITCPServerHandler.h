/*
 * ITCPServerHandler.h
 *
 *  Created on: Aug 24, 2017
 *      Author: demo
 */

#ifndef TCP_ITCPSERVERHANDLER_H_
#define TCP_ITCPSERVERHANDLER_H_

#include<string>
#include "Socket.h"
using namespace std;
#include "IHandler.h"
#include "../sockets/TCPMessengerProtocol.h"
#include<vector>
#include "../sockets/Strings.h"
#include "../centralDirectory/Users.h"
#include "../centralDirectory/Games.h"
#include "../centralDirectory/AcceptAll.h"
#include "../centralDirectory/RejectAll.h"
#include "../utils/ServerIO.h"
class ITCPServerHandler :public IHandler {

public:
	ITCPServerHandler();
	virtual ~ITCPServerHandler();
	void handleMessages(Socket * socket, int serverCommand, string serverMsg);
};

#endif /* TCP_ITCPSERVERHANDLER_H_ */
