/*************************************************************************
* function name:          Socket
* The Input:
* The output:             setting struct
* The Function operation: creating sockets
*************************************************************************/

#include "Socket.h"

Socket::Socket() {
	this->sock = 0;
}

Socket::~Socket() {
	close(this->sock);
}

/*************************************************************************
* function name:          setStruct
* The Input:			  port number, ip address, val = sin_in/sin_any
* The output:             setting struct
* The Function operation: creating sockets
*************************************************************************/

void Socket::setStruct(int port, char *ip, int val) {
	if (val == ANY_ADDR_IP) {
		this->setStructAny(port, ip);
		return;
	}
	this->setStructInt(port, ip);
}

/*************************************************************************
* function name:          setStructAny
* The Input:			  port number, ip address
* The output:             setting struct
* The Function operation: initializing struct
*************************************************************************/

void Socket::setStructAny(int port, char *ip) {
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	this->sin_any = sin;
}

/*************************************************************************
* function name:          setStructInt
* The Input:			  port number, ip address
* The output:             setting struct
* The Function operation: initializing struct
*************************************************************************/

void Socket::setStructInt(int port, char *ip) {
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip);
	sin.sin_port = htons(port);
	this->sin_in = sin;
}
/*************************************************************************
* function name:          changePort
* The Input:			  port number
* The output:             changes the listening port to new port
* The Function operation: initializing struct
*************************************************************************/
void Socket::changePort(int port) {
	this->sin_in.sin_port = htons(port);
}
