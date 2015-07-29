/*
 * Gate_Timer_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Gate_Timer_Request.h"
#include "Gate_Monitor.h"
#include "Config_Cache.h"
#include "Game_Manager.h"
#include "DB_Manager.h"
#include "Pool_Manager.h"
#include "Gate_Player.h"
#include "Utility_Func.h"
#include "Gate_Player.h"

Gate_Timer_Request::Gate_Timer_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = GATE_MONITOR;
}

Gate_Timer_Request::~Gate_Timer_Request() {
	// TODO Auto-generated destructor stub
}

int Gate_Timer_Request::login_gate_timeout(Block_Buffer &buf) {
	role_id_t role_id = 0;
	buf.read_int64(role_id);

	Gate_Player *player = monitor()->find_player_by_role_id(role_id);
	if (!player) {
		MSG_DEBUG("can not find player, maybe logout, role:%ld", role_id);
		return 0;
	}

	if (! player->cid()) {
		// 可能正在下线,如网关断了
		if (player->recv_pace_off()) {
			MSG_DEBUG("sign out ing maybe gate link drop, role:%ld", role_id);
			return 0;
		}

		player->notice_logic_sign_out(10000119);

		MSG_DEBUG("login gate timeout, role:%ld", role_id);
	} else {
		//MSG_DEBUG("");
	}

	return 0;
}

int Gate_Timer_Request::monitor_tick(void) {

	return 0;
}
