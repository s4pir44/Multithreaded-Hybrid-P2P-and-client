/*
 * ServerIO.h
 *
 *  Created on: Aug 18, 2017
 *      Author: demo
 */

#ifndef UTILS_SERVERIO_H_
#define UTILS_SERVERIO_H_
#define INT_BYTE_LENGTH 4

#include <strings.h>
#include<string.h>
#include <vector>
#include "../sockets/UDPSocket.h"
#include "../sockets/Strings.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include <algorithm> // find in vector

class ServerIO {
public:
		static string FromFloatToString(float number, int percision);

		static string FormatVector(vector<string> users, string delimiter);

		static bool isInVector(string name, vector<string> users);

		static string CreateMessage(int command, string content);

		static string FromIntToString(int number, int numberStringLength);

		static int FromStringToInt(string str);

		static int ReadCommand(string msg);

		static string ReadMessage(string msg);
		/**
		 * read command from peer
		 */
		static int readCommandFromPeer(UDPSocket* peer);

		/**
		 * read data from peer
		 */
		static string readDataFromPeer(UDPSocket* peer);

		/**
		* send command to peer
				 */
		static void sendLoginOrRegisterCommandToPeer(UDPSocket* peer, string ip, int serverPort, int command);
		/**
		 * send command to peer
		 */
		static void sendCommandToPeer(UDPSocket* peer, int command);

		/**
		 * send data to peer
		 */
		static void sendDataToPeer(UDPSocket* peer, string msg);



};


#endif /* UTILS_SERVERIO_H_ */
