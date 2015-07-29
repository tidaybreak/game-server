/*
 * Chat_Teamer.cpp
 *
 *  Created on: 2014年6月4日
 *      Author: Linqiyou
 */

#include "Chat_Teamer.h"
#include "Chat_Team_Manager.h"

Chat_Teamer::Chat_Teamer(void) {
	reset();
}

Chat_Teamer::~Chat_Teamer(void) {
}

void Chat_Teamer::reset(void) {
	this->teamer_detail_.reset();
}

//int Chat_Teamer::syn_team_info(Block_Buffer &buf) {
//	CHAT_TEAM_MANAGER->add_team(buf);
//	return 0;
//}

int Chat_Teamer::syn_teamer_info(Block_Buffer &buf) {
	this->teamer_detail_.deserialize(buf);
	return 0;
}

int64_t Chat_Teamer::get_team_id() {
	return this->teamer_detail_.team_id;
}
