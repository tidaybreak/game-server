/*
 * Chat_Timer_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 *      定时器消息
 */

#ifndef CHAT_TIMER_REQUEST_H_
#define CHAT_TIMER_REQUEST_H_

#include "Game_Typedef.h"

class Chat_Monitor;
class Chat_Player;

class Chat_Timer_Request {
public:
	Chat_Timer_Request();
	virtual ~Chat_Timer_Request();

	int monitor_tick(void);

	Chat_Monitor *monitor(void);

	int player_trigger_daily_zero(void);

private:
	Chat_Monitor *monitor_;
};

typedef Singleton<Chat_Timer_Request> Chat_Timer_Request_Single;
#define CHAT_TIMER_REQUEST    (Chat_Timer_Request_Single::instance())

inline Chat_Monitor *Chat_Timer_Request::monitor(void) {
	return monitor_;
}

#endif /* CHAT_TIMER_REQUEST_H_ */
