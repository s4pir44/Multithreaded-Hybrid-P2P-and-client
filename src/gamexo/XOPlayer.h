/*
 * XOPlayer.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef GAMEXO_XOPLAYER_H_
#define GAMEXO_XOPLAYER_H_

#include "../gameInfra/IPlayer.h"
#include "../gameInfra/IMOVE.h"
#include<string>
using namespace std;

class XOPlayer :public IPlayer {
	int sign;
	string ipandport;
	string user;
public:
	XOPlayer(int sign, string ipandport, string user);
	virtual ~XOPlayer();
	int getSign();
	string getIpAndPort();
	string getUser();
};

#endif /* GAMEXO_XOPLAYER_H_ */
