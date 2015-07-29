/*
 * Register_Client_Message.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: ti
 */
#include "Logic_Client_Request.h"
#include "Chat_Message_Unit.h"
#include "Chat_Client_Request.h"
#include "Msg_Chat_Enum.h"
#include "Msg_Test_Enum.h"

int Chat_Message_Unit::register_client_message(void) {
	this->register_client_process(REQ_CHAT_TEST_COMMAND, SCT_CALLBACK_2(Chat_Client_Request::process_10399999, CHAT_CLIENT_REQUEST));

	this->register_client_process(REQ_LOCAL_CHAT, SCT_CALLBACK_2(Chat_Client_Request::process_10300000, CHAT_CLIENT_REQUEST));
	this->register_client_process(REQ_HISTORY_CHAT, SCT_CALLBACK_2(Chat_Client_Request::process_10300001, CHAT_CLIENT_REQUEST));
	this->register_client_process(REQ_HISTORY_GANG_CHAT, SCT_CALLBACK_2(Chat_Client_Request::process_10300002, CHAT_CLIENT_REQUEST));
	this->register_client_process(REQ_INPUT_STATUS, SCT_CALLBACK_2(Chat_Client_Request::process_10300003, CHAT_CLIENT_REQUEST));

	return 0;
}

