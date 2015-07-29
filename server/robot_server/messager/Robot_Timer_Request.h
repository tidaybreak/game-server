/*
 * Robot_Timer_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 *      定时器消息
 */

#ifndef ROBOT_TIMER_REQUEST_H_
#define ROBOT_TIMER_REQUEST_H_

#include "Game_Typedef.h"

class Robot_Monitor;
class Robot_Player;
struct Logic_Player_Detail;

class Robot_Timer_Request {
public:
	Robot_Timer_Request();
	virtual ~Robot_Timer_Request();

	Robot_Monitor *monitor(void);

	int login_robot_timeout(Block_Buffer &buf);

	int monitor_tick(void);

private:
	Robot_Monitor *monitor_;
};

typedef Singleton<Robot_Timer_Request> Robot_Timer_Request_Single;
#define ROBOT_TIMER_REQUEST    (Robot_Timer_Request_Single::instance())

inline Robot_Monitor *Robot_Timer_Request::monitor(void) {
	return monitor_;
}

#endif /* Robot_TIMER_REQUEST_H_ */
