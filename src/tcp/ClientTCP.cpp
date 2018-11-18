/*************************************************************************
* function name:          ClientTCP
* The Input:              Makes a Conncention to a socket via TCP protocol
* The output:             The correct error message and exiting the system.
*************************************************************************/

#include "ClientTCP.h"

ClientTCP::ClientTCP() {

}

/*************************************************************************
* function name:          ClientTCP
* The Input:              port and ip needed for connection socket
* The output:             The correct error message and exiting the system.
* The Function operation: conncet to a socket.
*************************************************************************/

ClientTCP::ClientTCP(int port, char*ip) {
	this->setStruct(port, ip,INT_IP);
	if (connect(sock, (struct sockaddr *) &this->sin_in, sizeof(sin_in)) < 0) {
		char error[] = "(Client - TCP) - Error in Connecting to Socket";
		perror(error);
	}
}

ClientTCP::~ClientTCP() {

}


