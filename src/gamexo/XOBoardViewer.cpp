/*
 * XOBoardViewer.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: demo
 */

#include "XOBoardViewer.h"

XOBoardViewer::XOBoardViewer(unsigned int size) {
	int totalSize = size * size;
	this->size = size;
	this->table = new int[totalSize]; // 3X3
	for(int i = 0; i< totalSize;i++)
		this->table[i] = NO_VALUE; // No Value Attched

}
void XOBoardViewer::setTable(int *table)
{
	// Memento?
	this->table = table;
}
void XOBoardViewer::display()
{
	cout<<endl;
	cout<<" ___________________________"<<endl;
	cout<<"|__________XO_Board_________|"<<endl;
	for(unsigned int i = 0; i < this->size; i++)
	{
		int row = i * size;

		for(unsigned int j = 0; j < this->size; j++)
		{

			cout<<"|__";
			switch(table[row+j])
			{
				case NO_VALUE:
				{
					cout<<" * ";
					break;
				}
				case X:
				{
					cout<<" X ";
					break;
				}
				case O:
				{
					cout<<" O ";
					break;
				}
			}
			cout<<"__| ";

		}
		cout<<endl;

	}
}
XOBoardViewer::~XOBoardViewer() {
	// TODO Auto-generated destructor stub
}

