/*
 * Menus.cpp
 *
 *  Created on: Aug 21, 2017
 *      Author: demo
 */

#include "Menus.h"

void Menus::PrintMenu(int menu)
{
	cout<<"Please Follow the instructions: type"<<endl;
	switch(menu)
	{
		case 1:
		{
			cout<<"c <port> - connect to the server with the given port"<<endl;
			break;
		}
		case 2:
		{
			cout<<"login <user> <password> - login with the user and password"<<endl;
			cout<<"register <user> <password> - register the new user with password"<<endl;
			break;
		}
		case 3:
		{
			cout<<"scores - see game scores so far"<<endl;
			cout<<"lu - see user list from the server"<<endl;
			cout<<"lcu - see the connected user list"<<endl;
			cout<<"r <user> - request to play with a specific user"<<endl;
			cout<<"rp - request to play with a random player"<<endl;
			break;
		}
		case 4:
		{
			cout<<"Wait for peer address from server..."<<endl;
			break;
		}
		case 5:
		{
			cout<<"s <message> - send a message to the user"<<endl;
			//cout<<"move i j - choose you next target at the game"<<endl;
			//cout<<"exit game - exit the game"<<endl;
			break;
		}
		case 6:
		{
			cout<<"s <message> - send a message to the user"<<endl;
			cout<<"m <i,j> - choose you next move target at the game (start with 1,1 until 3,3)"<<endl;
		}

	}
	cout<<"x - quit the application"<<endl;
}
