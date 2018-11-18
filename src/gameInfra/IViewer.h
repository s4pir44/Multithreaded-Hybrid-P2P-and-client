/*
 * IViewer.h
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#ifndef GAMEINFRA_IVIEWER_H_
#define GAMEINFRA_IVIEWER_H_

class IViewer {
public:
	virtual void display()=0;
	virtual ~IViewer(){};
};


#endif /* GAMEINFRA_IVIEWER_H_ */
