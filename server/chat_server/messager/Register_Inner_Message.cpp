/*
 * Register_Inner_Message.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: ti
 */

#include "Chat_Message_Unit.h"
#include "Msg_Inner_Enum.h"
#include "Chat_Server_Request.h"
#include "Msg_Php_Enum.h"


using namespace Server_Enum;

int Chat_Message_Unit::register_inner_message(void) {
	register_server_process(LOGIC_CHAT_LOGIN_READY, SCT_CALLBACK_4(Chat_Server_Request::logic_login_ready, CHAT_SERVER_REQUEST));
	register_server_process(LOGIC_CHAT_LOGOUT_READY, SCT_CALLBACK_4(Chat_Server_Request::logic_logout_ready, CHAT_SERVER_REQUEST));
	register_server_process(20100015, SCT_CALLBACK_4(Chat_Server_Request::process_20100015, CHAT_SERVER_REQUEST));
	register_server_process(INNER_SYNC_PLAYER_NAME, SCT_CALLBACK_4(Chat_Server_Request::process_20100028, CHAT_SERVER_REQUEST));
	register_server_process(INNER_SYNC_PLAYER_GENDER, SCT_CALLBACK_4(Chat_Server_Request::process_20100029, CHAT_SERVER_REQUEST));

	register_server_process(INNER_SYNC_GANG_INFO, SCT_CALLBACK_4(Chat_Server_Request::process_20170000, CHAT_SERVER_REQUEST));
	register_server_process(INNER_SYNC_BLACK_LIST, SCT_CALLBACK_4(Chat_Server_Request::process_20160000, CHAT_SERVER_REQUEST));
	register_server_process(INNER_ANNOUNCE_SYSTEM, SCT_CALLBACK_4(Chat_Server_Request::process_20500000, CHAT_SERVER_REQUEST));
	register_server_process(LOGIC_CHAT_CLIENT_READY_NOW, SCT_CALLBACK_4(Chat_Server_Request::process_20010085, CHAT_SERVER_REQUEST));
	register_server_process(INNER_CHAT_SYNC_TEAMER_INFO, SCT_CALLBACK_4(Chat_Server_Request::process_20100301, CHAT_SERVER_REQUEST));
	register_server_process(INNER_CHAT_SYNC_TEAM_INFO, SCT_CALLBACK_4(Chat_Server_Request::process_20100302, CHAT_SERVER_REQUEST));

	register_server_process(LOGIC_CHAT_CHAT_BUGLE, SCT_CALLBACK_4(Chat_Server_Request::process_20100099, CHAT_SERVER_REQUEST));

	register_server_process(LOGIC_CHAT_BROADCAST_BUFF, SCT_CALLBACK_4(Chat_Server_Request::process_20100098, CHAT_SERVER_REQUEST));
	register_server_process(INNER_SEND_VIP_INFO_TO_CHAT, SCT_CALLBACK_4(Chat_Server_Request::process_20200024, CHAT_SERVER_REQUEST));
	register_server_process(INNER_SEND_LEVEL_INFO_TO_CHAT, SCT_CALLBACK_4(Chat_Server_Request::process_20200025, CHAT_SERVER_REQUEST));

	register_server_process(PHP_SEND_GM_ANNOUNCE, SCT_CALLBACK_4(Chat_Server_Request::process_30100008, CHAT_SERVER_REQUEST));
	register_server_process(GAG_ACCOUNT_ROLE, SCT_CALLBACK_4(Chat_Server_Request::process_30100005, CHAT_SERVER_REQUEST));

	return 0;
}
