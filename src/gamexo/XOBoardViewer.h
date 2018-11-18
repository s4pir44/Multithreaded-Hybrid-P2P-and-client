/*
 * XOBoardViewer.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef GAMEXO_XOBOARDVIEWER_H_
#define GAMEXO_XOBOARDVIEWER_H_
#include <iostream>
using namespace std;
#include "../gameInfra/IViewer.h"
#include "GameProtocol.h"
class XOBoardViewer :public IViewer {
public:
	int *table;

public:
	unsigned int size;
	XOBoardViewer(unsigned int size);
	virtual ~XOBoardViewer();
	void display();
	void setTable(int * table);
};

#endif /* GAMEXO_XOBOARDVIEWER_H_ */
