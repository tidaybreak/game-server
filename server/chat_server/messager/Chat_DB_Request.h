/*
 * Chat_DB_Request.h
 *
 *  Created on: 2014-05-20
 *      Author: xiaoliang
 */

#ifndef CHAT_DB_REQUEST_H_
#define CHAT_DB_REQUEST_H_

#include "Game_Typedef.h"

class Chat_Monitor;
struct Chat_Player_Detail;

class Chat_DB_Request {
public:
	Chat_DB_Request();
	virtual ~Chat_DB_Request();

	Chat_Monitor *monitor(void);

	int process_load_offline_private_chat_buff(Block_Buffer &buf);
	int process_load_private_chat_buff(Block_Buffer &buf);

private:
	Chat_Monitor *monitor_;
};

typedef Singleton<Chat_DB_Request> Chat_DB_Request_Single;
#define CHAT_DB_REQUEST    (Chat_DB_Request_Single::instance())

inline Chat_Monitor *Chat_DB_Request::monitor(void) {
	return monitor_;
}

#endif /* CHAT_DB_REQUEST_H_ */
