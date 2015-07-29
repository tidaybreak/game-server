/*
 * Chat_Gang_Struct.cpp
 *
 *  Created on: May 8, 2014
 *      Author: caijianwei
 */

#include "Chat_Gang_Struct.h"
#include "Msg_Struct.h"

Chat_Gang_Info::Chat_Gang_Info() {
	reset();
}

void Chat_Gang_Info::reset() {
	gang_id = 0;
	members.clear();
}

Gang_Chat_Record::Gang_Chat_Record() {
	reset();
}

void Gang_Chat_Record::reset() {
	is_change = false;
	chat_record_vec.clear();
}
