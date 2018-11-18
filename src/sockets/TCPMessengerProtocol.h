#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346
#define TCP_SERVER_DB_PORT 5774
#define UDP_GAME_SERVER_PORT 13459
#define UDP_GAME_SERVER_IP	"127.0.0.1"

#define CLOSE_SESSION_WITH_PEER 	1
#define OPEN_SESSION_WITH_PEER 		2 // DATA: user
#define DISCONNECT_FROM_SERVER_REQ	3
#define SEND_MSG_TO_PEER			4
#define SESSION_REFUSED				5
#define SESSION_ESTABLISHED			6
#define LOGIN_REQ					7 // DATA: user password
#define REGISTER_REQ				8 // DATA: user password
#define USER_ALREADY_EXISTS_RES 	9
#define LOGIN_FAILED				10
#define OPEN_ROOM_REQ				11 // DATA: room
#define CLOSE_ROOM_REQ				12
#define JOIN_ROOM_REQ				13 // DATA: room
#define LEAVE_ROOM_REQ				14
#define NEW_MESSAGE_DST_RES			15 // DATA: ip1:port1:name1,ip2:port2:name2,...
#define USER_JOIN_ROOM_RES			16
#define USER_LEAVE_ROOM_RES			17
#define ALREADY_CONNECTED_RES		18
#define USER_BUSY_RES				19
#define ROOM_ALREADY_EXISTS			20
#define SUCCESSFULLY_LEFT_ROOM		21
#define CLOSE_ROOM_RES				22
#define SUCCESSFULLY_JOIN_ROOM		23 // DATA: room
#define PRINT_ROOMS_USERS_REQ		24
#define PRINT_ROOMS_USERS_RES		25 // DATA: name1,name2,...
#define PRIMT_ROOMS_NAMES_REQ		26
#define PRIMT_ROOMS_NAMES_RES		27 // DATA: room1,room2,...
#define PRINT_CONNECT_USERS_REQ		28
#define PRINT_CONNECT_USERS_RES		29 // DATA: name1,name2,...
#define SHUT_DOWN_RES				30
#define SUCCESSFULLY_LOGIN_RES		31
#define PORT_INIT_RES				32 // DATA: port
#define PORT_INIT_REQ				33
#define PORT_INIT_FAILED			34
#define IN_DISPATCHER				35
#define IN_EMPTY_ROOM				36
#define PRINT_USERS_REQ				37
#define PRINT_USERS_RES				38 // DATA: name1,name2,...
#define ROOM_NOT_EXISTS				39
#define DISCONNECT_CLIENTS			40
#define LOGIN_SUCCESS				41
#define SEND_OTHER_PLAYER_DETAILS				42
#define HAND_SHAKE					43
#define REGISTER_FAILED	44
#define REGISTER_SUCCESS 45
#define SESSION_REQ_PLAY_WITH_USER 46
#define CHALLENGE_ACCEPTED	47
#define WAIT_FOR_OPPONENT_ADDR	48
#define ACCEPT_ALWAYS	49
#define REJECT_ALWAYS	50
#define OPPONENT_ADDR 51
#define PRINT_ALL_USERS_RES	52
#define REJECT_REQ_SUCCESS 53
#define REJECT_REQ_FAILURE 54
#define ACCEPT_REQ_SUCCESS 55
#define ACCEPT_REQ_FAILURE 56
#define DISCONNECT_FROM_CLIENT 57
#define DISCONNECT_FROM_BROKER 58
#define EXIT_GAME 59
#define GET_ALL_USERS 60
#define GET_ALL_USERS_RES 61
#define GET_ALL_CONNECTED_USERS 62
#define GET_ALL_CONNECTED_USERS_RES 63
#define STATUS_ACCEPT_ALL 64
#define STATUS_ACCEPT_ALL_RES 65
#define STATUS_REJECT_ALL 66
#define STATUS_REJECT_ALL_RES 67
#define PLAY_WITH_RANDOM_PLAYER 68
#define PLAY_WITH_RANDOM_PLAYER_RES 69
#define GET_REJECTERS_PLAYERS 70
#define GET_REJECTED_PLAYERS_RES 71
#define CANCEL_REQUEST 72
#define GAME_YOU_PLAY 73
#define GAME_YOU_WAIT 74
#define GAME_YOU_WIN 75
#define GAME_YOU_LOSE 76
#define GAME_DRAW 77
#define GAME_CREATE 78
#define GAME_MAKE_MOVE 79
#define GAME_REMOVE 80
#define GAME_IS_OVER 81
#define GAME_STOP 82
#define REPORT_SCORE 83
#define GET_SCORES 84


#define WIN 1
#define LOSE -1
#define DRAW 0


#define ROOM						1
#define DISPATCHER					2
#define BROKER						3
#define AUTH						4
#define WAIT_FOR_PLAY				5
#define DISCONNECTED				0
#define DEAD						6
#define INT_BYTE_SIZE	4
#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"
#define USER_IP "127.0.0.1"
#define MESSENGER_SERVER_IP "127.0.0.1"
#define GAME_SERVER_IP "127.0.0.1"
#define MESSENGER_SERVER_PORT 1327
#define GAME_SERVER_PORT 4098
#endif /* TCPMESSENGERPROTOCOL_H_ */
