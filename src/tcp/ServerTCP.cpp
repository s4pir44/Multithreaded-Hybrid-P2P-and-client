/*************************************************************************
* function name:          ServerTCP
* The Input:              Creates server features : bind listen and accept
*************************************************************************/

#include "ServerTCP.h"

ServerTCP::ServerTCP() {
	isRunning = false;
	this->callback = NULL;
}

/*************************************************************************
* function name:          ServerTCP
* The Input:              port and ip needed for connection socket
* The output:             Makes bind listen and accept
* The Function operation: connects to a socket.
*************************************************************************/

ServerTCP::ServerTCP(int port, char *ip, IHandler * handler) {
	this->callback = handler;
	isRunning = false;
	this->setStruct(port, ip,ANY_ADDR_IP);

	if (bind(this->sock,
			(struct sockaddr *) &this->sin_any, sizeof(this->sin_any)) < 0) {
		char error[] = "(Server - TCP) - Error in Creating Socket";
		perror(error);
	}
	// listen to socket :
	unsigned int back_log = 5;
	if (listen(this->sock, back_log) < 0) {
		char error[] = "(Server - TCP) - Error in Listening Socket";
		perror(error);
	}

	this->start();

}

void ServerTCP::run() {
	// ready for accept socket :
	this->acceptSock();
	this->isRunning = true;
	while(isRunning)
	{
		try {
			cout<<"[Tcp client - Started] "<<endl<<endl;
			char message[2000];
			while(this->reciveInfo(message) > 0)
			{
				//cout<<"[Start Request]"<<endl;
				string msg = message;
				if (this->callback != NULL && !msg.empty())
				{
					//cout<<"[Start Handle]"<<endl<<endl;
					try {
						this->callback->handleMessages(this, ServerIO::ReadCommand(msg), ServerIO::ReadMessage(msg));
					}
					catch(const char * e)
					{
						perror(e);
					}
					//cout<<endl<<"[Stop  Handle] "<<endl;
				}
				//cout<<"[End   Request]";

			}
			cout<<"[Tcp client - Stopped] "<<endl;
		}
		catch(const char * e){perror(e);}

	}
}

/*************************************************************************
* function name:          acceptSock
* The Input:
* The output:             acceptSock
* The Function operation: accept to a socket.
*************************************************************************/

void ServerTCP::acceptSock() {
	struct sockaddr_in client_sin;
	unsigned int addr_len = sizeof(client_sin);
	int newSocket = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
	if (newSocket < 0) {
		char error[] = "(Server - TCP) - Error in Accepting Socket";
		perror(error);
	}
	this->setNewSocket(newSocket);

}
void ServerTCP::stopServer()
{
	this->isRunning = false;
}
ServerTCP::~ServerTCP() {

}


