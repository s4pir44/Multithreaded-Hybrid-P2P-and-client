/*
 * Socket.h
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_
#include<iostream>
using namespace std;
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<string.h>


#define ANY_ADDR_IP 1
#define INT_IP 2

class Socket {
private:
	void setStructAny(int port, char * ip); // set the sin any struct
	void setStructInt(int port, char * ip); // set the sin in struct
protected:
	struct sockaddr_in sin_in;
	struct sockaddr_in sin_any;
	int sock;
	void changePort(int port);
public:
	Socket();
	virtual ~Socket();
	void setStruct(int port, char *ip, int val);

	virtual char* reciveInfo(char* buffer)=0; //  receive
	virtual void sendInfo(char *message)=0; // send
};

#endif /* TCP_SOCKET_H_ */
