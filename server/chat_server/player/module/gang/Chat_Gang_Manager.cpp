/*
 * Chat_Gang_Manager.cpp
 *
 *  Created on: May 8, 2014
 *      Author: caijianwei
 */

#include "Chat_Gang_Manager.h"
#include "Msg_Struct.h"
#include "DB_Operator.h"

Chat_Gang_Manager::Chat_Gang_Manager() {
	reset();
}

Chat_Gang_Manager::~Chat_Gang_Manager() { }

void Chat_Gang_Manager::start() {
	MONGO_OPERATOR->load_gang_chat_record(gand_chat_record_map);//初始化公会聊天记录
}

void Chat_Gang_Manager::save() {
	MONGO_OPERATOR->save_gang_chat_record(gand_chat_record_map);
}

void Chat_Gang_Manager::add_ganger(int64_t gang_id, role_id_t role_id) {
	if ( gang_id == 0 ) return ;
	Chat_Gang_Info* gang_info = gang_info_by_id( gang_id );
	if ( gang_info ){
		gang_info->members.insert( role_id );
	} else {
		Chat_Gang_Info new_info;
		new_info.gang_id = gang_id;
		new_info.members.insert( role_id );
		gang_id_map.insert( std::make_pair(gang_id, new_info));
	}
}

void Chat_Gang_Manager::remove_ganger(int64_t gang_id, role_id_t role_id) {
	Chat_Gang_Info* gang_info = gang_info_by_id( gang_id );
	if ( gang_info ) {
		gang_info->members.erase( role_id );
	}
}

Chat_Gang_Info* Chat_Gang_Manager::gang_info_by_id(int64_t gang_id) {
	Gang_ID_Info_Map::iterator iter = gang_id_map.find(gang_id);
	if ( iter != gang_id_map.end() ) {
		return &(iter->second);
	}
	return 0;
}

const Chat_Gang_Manager::Gang_ID_Info_Map& Chat_Gang_Manager::get_gang_map(void) {
	return gang_id_map;
}

void Chat_Gang_Manager::save_chat_record(gang_id_t gang_id, Chat_Record& chat_record) {
	Gang_ID_Chat_Record_Map::iterator iter = gand_chat_record_map.find(gang_id);
	if(iter != gand_chat_record_map.end()) {
		while(iter->second.chat_record_vec.size() >= 100){
			iter->second.chat_record_vec.erase(iter->second.chat_record_vec.begin());
		}
		iter->second.chat_record_vec.push_back(chat_record);
		iter->second.is_change = true;
	} else {
		Gang_Chat_Record gcr;
		gcr.reset();
		gcr.chat_record_vec.push_back(chat_record);
		gcr.is_change = true;
		gand_chat_record_map.insert(std::make_pair(gang_id,gcr));
	}
}

std::vector<Chat_Record>* Chat_Gang_Manager::get_chat_record_by_id(gang_id_t gang_id) {
	Gang_ID_Chat_Record_Map::iterator iter = gand_chat_record_map.find(gang_id);
	if(iter != gand_chat_record_map.end()) {
		return &iter->second.chat_record_vec;
	} else {
		return 0;
	}
}

void Chat_Gang_Manager::reset() {
	gang_id_map.clear();
	gand_chat_record_map.clear();
}
