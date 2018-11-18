/******************************************************************************
* Student Name:     Daniel Fisher
* Exercise Name:    Ex2
* File description: ServerTCP extends Tcp class
* Functionality :   This Class is responsible of binding listening and accepting
* 					connections.
********************************************************************************/

#ifndef SERVERTCP_H_
#define SERVERTCP_H_
#include "TCP.h"
#include "../threads/MThread.h"
#include "../utils/ServerIO.h"
#include "IHandler.h"

class ServerTCP :public TCP,MThread {
private:
	IHandler * callback;
	bool isRunning;
	void acceptSock();
public:
	ServerTCP();
	ServerTCP(int port, char *ip, IHandler * callback);
	virtual ~ServerTCP();
	void run();

	void stopServer();
};



#endif /* SERVERTCP_H_ */
