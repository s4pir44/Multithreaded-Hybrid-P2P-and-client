/*
 * UDPSocket2.h
 *
 *  Created on: Aug 17, 2017
 *      Author: demo
 */

#ifndef SOCKETT_UDPSOCKET_H_
#define SOCKETT_UDPSOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class UDPSocket {
	struct sockaddr_in s_in;
	struct sockaddr_in from;
	unsigned int fsize;
public :
	int socket_fd;
public:
	/**
	 * Ctor to create a UDP socket by port
	 */
	UDPSocket(int port = -1);

	/**
	 * Try to read a message in a specific length
	 */
	int recv(char* buffer, int length);

	/**
	 * Send a message to specific ip and port
	 */
	int sendTo(string msg, string ip, int port);

	/**
	 * Replay a message to the last sender.
	 */
	int reply(string msg);

	/*
	 * Close the UDP socket
	 */
	void cclose();

	/**
	 * Gets the sender address
	 */
	string fromAddr();

	/**
	 * Gets the port
	 */
	int getPort();

	/**
	 * Gets the destination ip and port.
	 */
	string destIpAndPort();
	int getDestPort();
};

#endif /* UDPSOCKET_H_ */
