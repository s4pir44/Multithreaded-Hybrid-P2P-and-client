/*
 * IHandler.h
 *
 *  Created on: Aug 24, 2017
 *      Author: demo
 */

#ifndef TCP_IHANDLER_H_
#define TCP_IHANDLER_H_
#include<string>
using namespace std;
#include "Socket.h"

class IHandler {
public:
	IHandler();
	virtual ~IHandler();

	virtual void handleMessages(Socket * socket, int serverCommand, string serverMsg) = 0;
};

#endif /* TCP_IHANDLER_H_ */
