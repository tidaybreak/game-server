/*
 * Chat_Client_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef CHAT_CLIENT_REQUEST_H_
#define CHAT_CLIENT_REQUEST_H_


#include "Game_Typedef.h"

class Chat_Monitor;
class Chat_Player;

class Chat_Client_Request {
public:
	Chat_Client_Request();
	virtual ~Chat_Client_Request();

	Chat_Monitor *monitor(void);
public:
	//local chat
	int process_10300000(Chat_Player& player, Block_Buffer& buf);
	int process_10300001(Chat_Player& player, Block_Buffer& buf);
	int process_10300002(Chat_Player& player, Block_Buffer& buf);

	int process_10399999(Chat_Player& player, Block_Buffer& buf);
	int process_10300003(Chat_Player& player, Block_Buffer& buf);
private:
	Chat_Monitor *monitor_;
};

typedef Singleton<Chat_Client_Request> Chat_Client_Request_Single;
#define CHAT_CLIENT_REQUEST    (Chat_Client_Request_Single::instance())

inline Chat_Monitor *Chat_Client_Request::monitor(void) {
	return monitor_;
}

#endif /* CHAT_CLIENT_REQUEST_H_ */
