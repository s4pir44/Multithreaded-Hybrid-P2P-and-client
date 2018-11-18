/*
 * TCPServer.h
 *
 *  Created on: Aug 21, 2017
 *      Author: demo
 */

#ifndef SOCKETS_TCPSERVER_H_
#define SOCKETS_TCPSERVER_H_
#include "TCPSocket.h"
#include "../threads/MThread.h"
class TCPServer :public MThread {
public:
	TCPSocket * server;
	bool isRuning;
public:
	TCPServer(int port);
	virtual ~TCPServer();

	void run(); // listen to connection
};

#endif /* SOCKETS_TCPSERVER_H_ */
