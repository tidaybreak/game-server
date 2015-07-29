/*
 * Logic_Timer_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 *      定时器消息
 */

#ifndef LOGIC_TIMER_REQUEST_H_
#define LOGIC_TIMER_REQUEST_H_

#include "Game_Typedef.h"

class Logic_Monitor;
class Logic_Player;
struct Logic_Player_Detail;

class Logic_Timer_Request {
public:
	Logic_Timer_Request();
	virtual ~Logic_Timer_Request();

	Logic_Monitor *monitor(void);

	int monitor_tick(void);

	int reboot_server(Block_Buffer &buf);

	int player_trigger_first(int32_t type);

	int player_trigger_daily_zero(void);

	int player_trigger_daily_six(void);

	int player_trigger_daily_twenty(void);

	int player_trigger_weekly_zero(void);

private:
	Logic_Monitor *monitor_;
};

typedef Singleton<Logic_Timer_Request> Logic_Timer_Request_Single;
#define LOGIC_TIMER_REQUEST    (Logic_Timer_Request_Single::instance())

inline Logic_Monitor *Logic_Timer_Request::monitor(void) {
	return monitor_;
}

#endif /* LOGIC_TIMER_REQUEST_H_ */
