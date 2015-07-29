/*
 * Robot_Timer_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Robot_Timer_Request.h"
#include "Robot_Monitor.h"
#include "Config_Cache.h"
#include "Game_Manager.h"
#include "DB_Manager.h"
#include "Pool_Manager.h"
#include "Robot_Player.h"
#include "Utility_Func.h"
#include "Robot_Player.h"

Robot_Timer_Request::Robot_Timer_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = ROBOT_MONITOR;
}

Robot_Timer_Request::~Robot_Timer_Request() {
	// TODO Auto-generated destructor stub
}

int Robot_Timer_Request::login_robot_timeout(Block_Buffer &buf) {
	role_id_t role_id = 0;
	buf.read_int64(role_id);

	Robot_Player *player = monitor()->find_player_by_role_id(role_id);
	if (!player) {
		MSG_DEBUG("can not find player, maybe logout, role:%ld", role_id);
		return 0;
	}

	if (! player->cid()) {

		player->notice_logic_sign_out(0);

		MSG_DEBUG("login robot timeout, role:%ld", role_id);
	} else {
		//MSG_DEBUG("");
	}

	return 0;
}

int Robot_Timer_Request::monitor_tick(void) {

	return 0;
}
