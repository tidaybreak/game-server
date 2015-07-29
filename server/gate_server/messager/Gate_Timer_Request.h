/*
 * Gate_Timer_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 *      定时器消息
 */

#ifndef GATE_TIMER_REQUEST_H_
#define GATE_TIMER_REQUEST_H_

#include "Game_Typedef.h"

class Gate_Monitor;
class Gate_Player;
struct Logic_Player_Detail;

class Gate_Timer_Request {
public:
	Gate_Timer_Request();
	virtual ~Gate_Timer_Request();

	Gate_Monitor *monitor(void);

	int login_gate_timeout(Block_Buffer &buf);

	int monitor_tick(void);

private:
	Gate_Monitor *monitor_;
};

typedef Singleton<Gate_Timer_Request> Gate_Timer_Request_Single;
#define GATE_TIMER_REQUEST    (Gate_Timer_Request_Single::instance())

inline Gate_Monitor *Gate_Timer_Request::monitor(void) {
	return monitor_;
}

#endif /* Gate_TIMER_REQUEST_H_ */
