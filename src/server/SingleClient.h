/*
 * SingleClient.h
 *
 *  Created on: Aug 19, 2017
 *      Author: demo
 */

#ifndef SERVER_SINGLECLIENT_H_
#define SERVER_SINGLECLIENT_H_
#include<string>
using namespace std;



class SingleClient {
public:
	string ip;
	int port;
	string username;

public:
	SingleClient(string ip, int port);
	virtual ~SingleClient();

	void toString();
};

#endif /* SERVER_SINGLECLIENT_H_ */
