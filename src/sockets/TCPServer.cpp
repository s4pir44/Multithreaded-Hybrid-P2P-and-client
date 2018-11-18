/*
 * TCPServer.cpp
 *
 *  Created on: Aug 21, 2017
 *      Author: demo
 */

#include "TCPServer.h"

TCPServer::TCPServer(int port) {
	// TODO Auto-generated constructor stub
	this->isRuning = true;
	this->server = new TCPSocket(port);

	this->start(); // listen to server Messenger
}

TCPServer::~TCPServer() {
	this->isRuning = false;
	this->waitForThread();
	if (this->server != NULL) {
		this->server->cclose();
		delete(this->server);
	}
}

void TCPServer::run() {
	bool isConnectedToMessenger = false;
	while(this->isRuning) {
		while(!isConnectedToMessenger) {
			this->server->listenAndAccept();
			isConnectedToMessenger = true;
		}
		char * buff = new char[2000];
		while(this->server->recv(buff, 2000) > 0) {
			cout<<"received "<<buff<<endl;
		}

	}
}
