/*
 * Robot_Client_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Robot_Client_Request.h"
#include "Robot_Monitor.h"
#include "Pool_Manager.h"
#include "Robot_Player.h"


Robot_Client_Request::Robot_Client_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = ROBOT_MONITOR;
}

Robot_Client_Request::~Robot_Client_Request() {
	// TODO Auto-generated destructor stub
}

int Robot_Client_Request::process_client_block(Robot_Player *player, int cid, int msg_id, Block_Buffer &buf) {
	switch (msg_id) {
	case 10400005: {
		process_transmit(player, buf);
		break;
	}
	default : {
		MSG_USER("error msg_id:%d, role:%ld, player cid:%d, cid:%d", msg_id, player->role_id(), player->cid(), cid);
		break;
	}
	}

	return 0;
}

int Robot_Client_Request::process_player_login(int cid, Block_Buffer &buf) {

	return 0;
}

int Robot_Client_Request::process_transmit(Robot_Player *player, Block_Buffer &buf) {
	return 0;
}
