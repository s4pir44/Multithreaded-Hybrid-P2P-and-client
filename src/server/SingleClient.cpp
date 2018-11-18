/*
 * SingleClient.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#include "SingleClient.h"
#include <iostream>
SingleClient::SingleClient(string ip, int port) {
	// TODO Auto-generated constructor stub
	this->username = "";
	this->ip = ip;
	this->port = port;
}

SingleClient::~SingleClient() {
	// TODO Auto-generated destructor stub
}

void SingleClient::toString() {
	cout<<"("<<ip<<" , "<<port<<")";
}
