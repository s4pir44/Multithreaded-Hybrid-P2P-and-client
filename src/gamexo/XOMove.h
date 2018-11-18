/*
 * XOMove.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef GAMEXO_XOMOVE_H_
#define GAMEXO_XOMOVE_H_

#include "../gameInfra/IMOVE.h"

class XO_Move : public IMOVE {

public:
	int row;
	int col;
	int sign;
public:
	XO_Move(int i, int j);
	virtual ~XO_Move();
};

#endif /* GAMEXO_XOMOVE_H_ */
