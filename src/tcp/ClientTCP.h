/*
 * ClientTCP.h
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#ifndef TCP_CLIENTTCP_H_
#define TCP_CLIENTTCP_H_

#include "TCP.h"

class ClientTCP : public TCP {

public:
	ClientTCP();
	ClientTCP(int port, char *ip);
	virtual ~ClientTCP();


};

#endif /* TCP_CLIENTTCP_H_ */
