/*
 * TCPClient.h
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#ifndef SOCKETS_TCPCLIENT_H_
#define SOCKETS_TCPCLIENT_H_
#include "TCPSocket.h"
#include "../threads/MThread.h"

class TCPClient : public MThread {
bool isRuning;


public:
TCPSocket * client;
	TCPClient(string ip, int port);
	virtual ~TCPClient();

	void run();


};

#endif /* SOCKETS_TCPCLIENT_H_ */
