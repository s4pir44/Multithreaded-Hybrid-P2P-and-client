/*
 * XOMove.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#include "XOMove.h"

XO_Move::XO_Move(int r, int c) {
	this->row = r;
	this->col = c;
	this->sign = -1;

}

XO_Move::~XO_Move() {
	// TODO Auto-generated destructor stub
}

