/*
 * Chat_Timer_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Chat_Timer_Request.h"
#include "Chat_Monitor.h"
#include "Pool_Manager.h"
#include "Chat_Player.h"
#include "Utility_Func.h"
#include "gang/Chat_Gang_Manager.h"
#include "gm_ann/GM_Ann_Manager.h"
#include "chater/Chater_Manager.h"

Chat_Timer_Request::Chat_Timer_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = CHAT_MONITOR;
}

Chat_Timer_Request::~Chat_Timer_Request() {
	// TODO Auto-generated destructor stub
}

int Chat_Timer_Request::monitor_tick() {
	Time_Value now = Time_Value::gettimeofday();
	CHAT_GANG_MANAGER->save();
	GM_ANN_MANAGER->tick(now);
	CHATER_MANAGER->tick(now);
	return 0;
}

int Chat_Timer_Request::player_trigger_daily_zero() {
	CHAT_MONITOR->player_trigger_daily_zero();
	return 0;
}
