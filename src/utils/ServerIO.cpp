/*
 * ServerIO.cpp
 *
 * Handles the reading and writing to and from the server.
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "ServerIO.h"
#include <cstdlib>
#include <stdio.h>
#include<sstream>
#include <stdlib.h>
#include <math.h>

#include <cstring>
#include <string>
#include "../sockets/TCPMessengerProtocol.h"
/*
 * Read command from peer
 */
int ServerIO::readCommandFromPeer(UDPSocket* peer) {
	int result;

	// Read a command from socket
	peer->recv((char*) &result, INT_BYTE_LENGTH);

	return ntohl(result);
}

/*
 * Read data from peer
 */
string ServerIO::readDataFromPeer(UDPSocket* peer) {
	// Read a string from socket



	int length = readCommandFromPeer(peer);

	// Receive the incoming message
	char buffer[length];
	memset((void*) buffer, 0, length);

	int rc = peer->recv(buffer, length);

	if (rc < 0) {
		perror("Error receiving socket");
		return "";
	}

	string str = buffer;

	return str.substr(0, length);
}
/*
 * Send Login command to peer
 */
void ServerIO::sendLoginOrRegisterCommandToPeer(UDPSocket* peer, string ip, int serverport, int command) {
	// Send command to socket

	stringstream ss;
	ss << command;
	std::string str = ss.str();
	int i = str.length();
	while (i< INT_BYTE_LENGTH)
	{
		str = "0" + str;
		i++;
	}
	peer->sendTo(str.c_str(), ip, serverport);
}

/*
 * Send command to peer
 */
void ServerIO::sendCommandToPeer(UDPSocket* peer, int command) {
	// Send command to socket
	if (command != LOGIN_REQ)
	{
			stringstream ss;
			ss << command;
			std::string str = ss.str();
			int i = str.length();
			while (i< INT_BYTE_LENGTH)
			{
				str = "0" + str;
				i++;
			}
		peer->reply(str.c_str()); // port already saved
	}
	//peer->send((char*) &msgLen, INT_BYTE_LENGTH);
}

/**
 * Send data to peer
 */
void ServerIO::sendDataToPeer(UDPSocket* peer, string msg) {
	// Send the message back to the client
	int msgLen = htonl(msg.length());
	// Check reply!!!
	peer->reply((char*) &msgLen);
	peer->reply(msg);
}
int ServerIO::FromStringToInt(string str)
{
	char * b = new char [str.length()+1];
	std::strcpy (b, str.c_str());
	int len = str.length()-1;
	int start = 10;
	int total = 0;
	bool negative = false;
	int j = 0;
	for(int i = len; i >= 0 ; i--) {
		int dig = 0;
		if (b[i] == '-')
		{
			negative = true;
		}
		else if (b[i] == '1')
		{
			dig = 1;
		} else if (b[i] == '2') {
			dig = 2;
		} else if (b[i] == '3') {
			dig = 3;
		}else if (b[i] == '4') {
			dig = 4;
		}else if (b[i] == '5') {
			dig = 5;
		}else if (b[i] == '6') {
			dig = 6;
		}else if (b[i] == '7') {
			dig = 7;
		}else if (b[i] == '8') {
			dig = 8;
		}else if (b[i] == '9') {
			dig = 9;
		}
		total += pow(start,j++) * dig;

	}

	return negative ? (-1)*total : total;
}

bool ServerIO::isInVector(string name, vector<string> users)
{
	return std::find(users.begin(), users.end(), name) != users.end();
}
string ServerIO::FormatVector(vector<string> users, string delimiter)
{
	string smsg= "";
	unsigned int i = 0;
	for(i = 0; i < users.size() - 1; i++) {
		smsg += users.at(i) + delimiter;
	}
	if (i >= 0)
	{
		smsg += users.at(i);
	}
	return smsg;
}
string ServerIO::ReadMessage(string message)
{
	string smsg = message;
	string msg = smsg.substr(INT_BYTE_SIZE+1) ; // +1 because of $ sign
	try {

		int readBytes = ServerIO::FromStringToInt(msg.substr(0,INT_BYTE_SIZE)); // size of message
		msg = msg.substr(INT_BYTE_SIZE+1, readBytes); // +1 because of $ sign
	} catch(const char * e) {
		perror("here");
	}


	return msg;
}

int ServerIO::ReadCommand(string message)
{
	string smsg = message;
	vector<string> commadAndMessage = Strings::split(smsg, "$");
	return ServerIO::FromStringToInt(commadAndMessage.at(0));
}

string ServerIO::FromFloatToString(float num,int percision)
{
	stringstream stream;
	stream << fixed << setprecision(percision) << num;
	return stream.str();
}

string ServerIO::FromIntToString(int number, int numberStringLength) {
	stringstream ss;
	ss << number;
	std::string str = ss.str();
	int i = str.length();
	while (i < numberStringLength)
	{
		str = "0" + str;
		i++;
	}

	return str;
}


string ServerIO::CreateMessage(int command, string content)
{
	string commandMsg = ServerIO::FromIntToString(command, INT_BYTE_SIZE);
	int leng = content.size();
	return commandMsg + "$" +ServerIO::FromIntToString(leng,4) +"$"+content;
}
