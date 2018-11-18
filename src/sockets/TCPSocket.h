/*
 * TCPSocket.h
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#ifndef SOCKETS_TCPSOCKET_H_
#define SOCKETS_TCPSOCKET_H_
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

class TCPSocket {
	struct sockaddr_in serverAddr;
		struct sockaddr_in peerAddr;

public:
	int connected_sock;
	TCPSocket(int connected_sock, struct sockaddr_in serverAddr,
				struct sockaddr_in peerAddr);

	/**
	 * Constructor to create a TCP server socket.
	 */
	TCPSocket(int port);

	/**
	 * Constructor to create a TCP client socket
	 * and connect it to the remote peer in the given ip and port.
	 */
	TCPSocket(string peerIp, int port);

	/**
	 * Perform listen and accept on server socket.
	 * this function returns a new TCPSocket object to communicate over the incoming session.
	 */
	TCPSocket* listenAndAccept();

	/**
	 * Reads from socket into the given buffer up to the buffer given length.
	 * return the number of bytes read
	 */
	int recv(char* buffer, int length);

	/**
	 * Sends the given buffer to the socket
	 */
	int send(const char* msg, int len);

	/**
	 * Closes the socket and free all resources
	 */
	void cclose();

	/**
	 * Returns the address of the connected peer
	 */
	string fromAddr();

	/**
	 * Gets the destination ip and port
	 */
	string destIpAndPort();

};

#endif /* SOCKETS_TCPSOCKET_H_ */
