/*
 * TCP.h
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#ifndef TCP_TCP_H_
#define TCP_TCP_H_


#include "Socket.h"

class TCP : public Socket {
private:
	int csocket;
protected:
	void setNewSocket(int sock);
public:
	TCP();
	virtual ~TCP();

	char* reciveInfo(char* buffer);
	void sendInfo(char *message);
};

#endif /* TCP_TCP_H_ */
