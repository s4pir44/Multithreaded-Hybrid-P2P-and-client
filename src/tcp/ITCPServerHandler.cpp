/*
 * ITCPServerHandler.cpp
 *
 *  Created on: Aug 24, 2017
 *      Author: demo
 */

#include "ITCPServerHandler.h"

ITCPServerHandler::ITCPServerHandler()
{

}
ITCPServerHandler::~ITCPServerHandler()
{

}
void ITCPServerHandler::handleMessages(Socket * socket, int serverCommand, string serverMsg)
{
	cout<<"[Start: DataBase TCP Server]"<<endl;
	switch(serverCommand)
	{
		case LOGIN_REQ:
		{
			cout<<"[Login Request] ";
			vector<string> userAndPassword = Strings::split(serverMsg, " ");
			try {
				string name = userAndPassword.at(0);
				string password = userAndPassword.at(1);
				cout<<"[user = "<<name<<", pass = "<<password<<"]";
				if (Users::login(name, password)) {
					cout<<" [True]"<<endl;
					//return ServerIO::CreateMessage(SUCCESSFULLY_LOGIN_RES, "true");
					socket->sendInfo((char*)(ServerIO::CreateMessage(SUCCESSFULLY_LOGIN_RES, "true")).c_str());
				} else {
					//return ServerIO::CreateMessage(LOGIN_FAILED, "false");
					socket->sendInfo((char*)(ServerIO::CreateMessage(LOGIN_FAILED, "false")).c_str());
					cout<<" [False]"<<endl;
				}
			}
			catch(const char * e)
			{
				socket->sendInfo((char*)(ServerIO::CreateMessage(LOGIN_FAILED, "false")).c_str());
			}
			break;
		}
		case REGISTER_REQ:
		{
			cout<<"[Register Request] ";
			vector<string> userAndPassword = Strings::split(serverMsg, " ");
			try {
				string name = userAndPassword.at(0);
				string password = userAndPassword.at(1);
				cout<<"[user = "<<name<<", pass = "<<password<<"]";
				if (Users::registerUsers(name, password)) {
					cout<<" [True]"<<endl;
					socket->sendInfo((char*)(ServerIO::CreateMessage(REGISTER_SUCCESS, "true")).c_str());
				} else {
					socket->sendInfo((char*)(ServerIO::CreateMessage(REGISTER_FAILED, "false")).c_str());
					cout<<" [False]"<<endl;
				}
			}
			catch(const char * e)
			{
				socket->sendInfo((char*)(ServerIO::CreateMessage(REGISTER_FAILED, "false")).c_str());
			}
			break;
		}
		case GET_ALL_USERS:
		{
			cout<<"[Get All Users Request] ";
			socket->sendInfo((char*)(ServerIO::CreateMessage(GET_ALL_USERS, Users::getAllUsers())).c_str());
			cout<<"[True]"<<endl;
			break;
		}
		case ACCEPT_ALWAYS:
		{
			cout<<"[Accept Always Request]";
			if (AcceptAll::AddNewUser(serverMsg))
			{
				socket->sendInfo((char*)(ServerIO::CreateMessage(ACCEPT_REQ_SUCCESS, "true")).c_str());
				cout<<" [True]"<<endl;
			}
			else {
				socket->sendInfo((char*)(ServerIO::CreateMessage(ACCEPT_REQ_FAILURE, "false")).c_str());
				cout<<" [False]"<<endl;
			}
			break;
		}
		case STATUS_ACCEPT_ALL:
		{
			cout<<"[Accept Always Status Request]";
			string res = AcceptAll::isUserExists(serverMsg) ? "true":"false";
			socket->sendInfo((char*)(ServerIO::CreateMessage(STATUS_ACCEPT_ALL, res)).c_str());
			cout<<"["<<res<<"]"<<" [True]"<<endl;
			break;
		}
		case REJECT_ALWAYS:
		{
			cout<<"[Reject Always Request]";
			if (RejectAll::AddNewUser(serverMsg))
			{
				socket->sendInfo((char*)(ServerIO::CreateMessage(REJECT_REQ_SUCCESS, "true")).c_str());
				cout<<" [True]"<<endl;
			}
			else {
				socket->sendInfo((char*)(ServerIO::CreateMessage(REJECT_REQ_FAILURE, "false")).c_str());
				cout<<" [False]"<<endl;
			}
			break;
		}
		case STATUS_REJECT_ALL:
		{
			cout<<"[Reject Always Status Request]";
			string res = RejectAll::isUserExists(serverMsg) ? "true":"false";
			socket->sendInfo((char*)(ServerIO::CreateMessage(STATUS_REJECT_ALL, res)).c_str());
			cout<<"["<<res<<"]"<<" [True]"<<endl;
			break;
		}
		case GET_REJECTERS_PLAYERS:
		{
			cout<<"[Reject Always Users Request]";
			socket->sendInfo((char*)(ServerIO::CreateMessage(GET_REJECTED_PLAYERS_RES, RejectAll::getRejectedUsers())).c_str());
			break;
		}
		case REPORT_SCORE:
		{
			// user WIN/LOSE/DRAW
			vector<string> userAndType = Strings::split(serverMsg, " ");
			string t = userAndType.at(1);
			int type = t == "WIN"? WIN : t == "LOSE"?LOSE:DRAW;

			bool success = Games::addScore(userAndType.at(0), type);
			socket->sendInfo((char*)(ServerIO::CreateMessage(REPORT_SCORE, success ? "true" : "false")).c_str());
			break;
		}
		case GET_SCORES:
		{
			socket->sendInfo((char*)(ServerIO::CreateMessage(GET_SCORES, Games::getScores())).c_str());
			break;
		}
	}
	cout<<"[END  : DataBase TCP Server]"<<endl;
	//return "";

}
