/*
 * Register_Inner_Message.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: ti
 */

#include "Gate_Message_Unit.h"
#include "Gate_Server_Request.h"

using namespace Server_Enum;

int Gate_Message_Unit::register_inner_message(void) {
	register_inner_process(LOGIC_GATE_LOGIN_READY, SCT_CALLBACK_2(Gate_Server_Request::logic_login_ready, GATE_SERVER_REQUEST));
	register_inner_process(LOGIC_GATE_LOGOUT_READY, SCT_CALLBACK_2(Gate_Server_Request::logic_logout_ready, GATE_SERVER_REQUEST));
	register_inner_process(INNER_GATE_ASK_LOGIC_TRAN, SCT_CALLBACK_2(Gate_Server_Request::logic_transmit_finish, GATE_SERVER_REQUEST));
	register_inner_process(SCENE_GATE_BATTLE_STATUS, SCT_CALLBACK_2(Gate_Server_Request::scene_battle_update, GATE_SERVER_REQUEST));
	register_inner_process(INNER_INNER_ASK_GATE_TRAN, SCT_CALLBACK_2(Gate_Server_Request::logic_inner_transmit, GATE_SERVER_REQUEST));
	register_inner_process(LOGIC_SYNC_LOGIN_SESSION, SCT_CALLBACK_2(Gate_Server_Request::logic_session, GATE_SERVER_REQUEST));


	register_inner_process(CHAT_GATE_BROADCAST_BUFF, SCT_CALLBACK_2(Gate_Server_Request::caht_gate_broadcast_buff, GATE_SERVER_REQUEST));

	return 0;
}

