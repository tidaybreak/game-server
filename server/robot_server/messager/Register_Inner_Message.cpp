/*
 * Register_Inner_Message.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: ti
 */

#include "Robot_Message_Unit.h"
#include "Robot_Server_Request.h"
#include "Head_Enum.h"


using namespace Server_Enum;

int Robot_Message_Unit::register_inner_message(void) {
	register_inner_process(SERVER_ERROR_MSG_TO_CLIENT, SCT_CALLBACK_2(Robot_Server_Request::process_80000001, ROBOT_SERVER_REQUEST));

	register_inner_process(RES_FETCH_ROLE_LIST, SCT_CALLBACK_2(Robot_Server_Request::process_50100000, ROBOT_SERVER_REQUEST));
	register_inner_process(RES_CREATE_ROLE, SCT_CALLBACK_2(Robot_Server_Request::process_50100005, ROBOT_SERVER_REQUEST));
	register_inner_process(RES_LOGIN_GATE, SCT_CALLBACK_2(Robot_Server_Request::process_50400001, ROBOT_SERVER_REQUEST));

	register_inner_process(ACTIVE_PLAYER_LOGIN, SCT_CALLBACK_2(Robot_Server_Request::process_81000000, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_ROLE_BASE_INFO, SCT_CALLBACK_2(Robot_Server_Request::process_81000100, ROBOT_SERVER_REQUEST));
	//register_inner_process(ACTIVE_PLAYER_APPEAR, SCT_CALLBACK_2(Robot_Server_Request::process_81000001, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_NPC_APPEAR, SCT_CALLBACK_2(Robot_Server_Request::process_81000005, ROBOT_SERVER_REQUEST));
	register_inner_process(FCM_LOGIN_TIME_TIP, SCT_CALLBACK_2(Robot_Server_Request::process_82200101, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_SCENE_TRANSFER, SCT_CALLBACK_2(Robot_Server_Request::process_81000030, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_RESET_COORD, SCT_CALLBACK_2(Robot_Server_Request::process_81000015, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_LINK_CLOSE, SCT_CALLBACK_2(Robot_Server_Request::process_80000005, ROBOT_SERVER_REQUEST));

	register_inner_process(RES_CLIENT_MOVE, SCT_CALLBACK_2(Robot_Server_Request::process_50200102, ROBOT_SERVER_REQUEST));
	register_inner_process(RES_ROLE_INFO, SCT_CALLBACK_2(Robot_Server_Request::process_50100006, ROBOT_SERVER_REQUEST));


	//register_inner_process(ACTIVE_MOVER_MOVE, SCT_CALLBACK_2(Robot_Server_Request::process_81000020, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_TRANSMIT_POINT, SCT_CALLBACK_2(Robot_Server_Request::process_81000040, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_MOVER_DISAPPEAR, SCT_CALLBACK_2(Robot_Server_Request::process_81000010, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_CLIENT_FIND_PATH, SCT_CALLBACK_2(Robot_Server_Request::process_81000041, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_NEW_HAND_DISABLE, SCT_CALLBACK_2(Robot_Server_Request::process_80100500, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_ROLE_INFO, SCT_CALLBACK_2(Robot_Server_Request::process_81000101, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_ICON_LIST, SCT_CALLBACK_2(Robot_Server_Request::process_80100400, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_LOGIC_ROLE_BASE_INFO, SCT_CALLBACK_2(Robot_Server_Request::process_81000099, ROBOT_SERVER_REQUEST));


	register_inner_process(RES_FETCH_TASK_LIST, SCT_CALLBACK_2(Robot_Server_Request::process_50110000, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_TASK_LIST_UPDATE, SCT_CALLBACK_2(Robot_Server_Request::process_81110000, ROBOT_SERVER_REQUEST));

	register_inner_process(RES_PASTED_SGL_DUNGEON, SCT_CALLBACK_2(Robot_Server_Request::process_50100200, ROBOT_SERVER_REQUEST));

	register_inner_process(ACTIVE_NPC_TRIGGER_SPOT, SCT_CALLBACK_2(Robot_Server_Request::process_81000201, ROBOT_SERVER_REQUEST));

	register_inner_process(ACTIVE_INTO_BATTLE, SCT_CALLBACK_2(Robot_Server_Request::process_81000050, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_BATTLE_EXIT, SCT_CALLBACK_2(Robot_Server_Request::process_81000055, ROBOT_SERVER_REQUEST));
	register_inner_process(ACTIVE_FIGHTER_PROP_CHANGE, SCT_CALLBACK_2(Robot_Server_Request::process_81000060, ROBOT_SERVER_REQUEST));

	register_inner_process(RES_SCENE_TRANSFER, SCT_CALLBACK_2(Robot_Server_Request::process_50400005, ROBOT_SERVER_REQUEST));
	//register_inner_process(ACTIVE_SCENE_TRANSFER, SCT_CALLBACK_2(Robot_Server_Request::process_81000030, ROBOT_SERVER_REQUEST));

	return 0;
}

