/*************************************************************************
* function name:          TCP
* The Input:
* The output:             initialize socket
* The Function operation: creating sockets
*************************************************************************/

#include "TCP.h"



TCP::TCP() {

	// Initialize socket :
	this->sock = socket(AF_INET, SOCK_STREAM, 0);

	if (this->sock < 0) {
		perror ("(Server) - Error in Creating Socket");
	}
	this->csocket = sock;

}

/*************************************************************************
* function name:          sendInfo
* The Input:			  message to be send
* The output:
* The Function operation: send data to sockets
*************************************************************************/

void TCP::sendInfo(char *message) {

	int size = strlen(message);
	int sent_bytes = send(this->csocket, message, size, 0);

	if (sent_bytes < 0) {
		char error[] = "(TCP) - Error in Sending data to Socket";
		perror(error);
	}
}

/*************************************************************************
* function name:          reciveInfo
* The Input:			  buffer to be receiving the data
* The output:			  the message
* The Function operation: send data to sockets
*************************************************************************/

char* TCP::reciveInfo(char* buffer) {
	char buffer1[4096] = "";
	int read_bytes = recv(this->csocket, buffer1, sizeof(buffer1), 0);
	if (read_bytes == 0) {
		char error[] = "(TCP) -  Connection is closed !";
		perror(error);
	}
	else if (read_bytes < 0) {
		char error[] = "(TCP) - Error in reading bytes !";
		perror(error);
	}
	strcpy(buffer,buffer1);
	return buffer;
}

/*************************************************************************
* function name:          setNewSocket
* The Input:			  socket to be changed to
* The output:			  None.
* The Function operation:
*************************************************************************/

void TCP::setNewSocket(int s) {
	this->csocket = s;
}

TCP::~TCP() {
	close(this->csocket);
}


