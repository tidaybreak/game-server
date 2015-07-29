/*
 * Chat_Client_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Chat_Client_Request.h"
#include "Chat_Monitor.h"
#include "Game_Manager.h"
#include "DB_Manager.h"
#include "Pool_Manager.h"
#include "Chat_Player.h"
#include "Utility_Func.h"
#include "Chat_Player.h"
#include "Err_Code.h"
#include "Msg_Chat_Struct.h"
#include "Msg_Test_Struct.h"

Chat_Client_Request::Chat_Client_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = CHAT_MONITOR;
}

Chat_Client_Request::~Chat_Client_Request() {
	// TODO Auto-generated destructor stub
}


int Chat_Client_Request::process_10300000(Chat_Player& player, Block_Buffer& buf){
	MSG_10300000 msg;
	JUDGE_MSG_ERROR_RETURN(msg)
	return player.req_chat( msg.chat_type, msg.system_type, msg.dest_id, msg.dest_name, msg.content );
}

int Chat_Client_Request::process_10300001(Chat_Player& player, Block_Buffer& buf){
	MSG_10300001 msg;
	JUDGE_MSG_ERROR_RETURN(msg)
	return player.req_history_private_chat_record(msg.role_id);
}

int Chat_Client_Request::process_10300002(Chat_Player& player, Block_Buffer& buf){
	MSG_10300002 msg;
	JUDGE_MSG_ERROR_RETURN(msg)
	return player.req_history_gang_chat_record();
}

int Chat_Client_Request::process_10399999(Chat_Player &player, Block_Buffer &buf) {
	MSG_10399999 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.test_command(msg.command);
}

int Chat_Client_Request::process_10300003(Chat_Player &player, Block_Buffer &buf) {
	MSG_10300003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.send_input_status(msg.role_id,msg.status);
}
