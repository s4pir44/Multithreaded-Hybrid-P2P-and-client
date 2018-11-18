/*
 * TCPClient.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#include "TCPClient.h"

TCPClient::TCPClient(string ip, int port) {
	// TODO Auto-generated constructor stub
	this->isRuning = true;
	this->client = new TCPSocket(ip, port);

	this->start();
}

TCPClient::~TCPClient() {
	if (this->client != NULL)
		delete(this->client);
}

void TCPClient::run()
{
	this->client->listenAndAccept();
	while(this->isRuning)
	{
		char * message = new char[2000];
		while(this->client->recv(message, 2000) > 0)
		{
			cout<<"Message in tcp client is : "<<message<<endl;
		}
	}
}
