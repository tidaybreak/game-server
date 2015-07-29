/*
 * Chat_DB_Request.cpp
 *
 *  Created on: 2014-05-20
 *      Author: xiaoliang
 */

#include "Chat_DB_Request.h"
#include "Chat_Client_Request.h"
#include "Chat_Monitor.h"
#include "Pool_Manager.h"
#include "Chat_Player.h"
#include "Msg_Active_Struct.h"
#include "gang/Chat_Gang_Manager.h"
#include "chater/Chater.h"
#include "Msg_Chat_Struct.h"

Chat_DB_Request::Chat_DB_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = CHAT_MONITOR;
}

Chat_DB_Request::~Chat_DB_Request() {
	// TODO Auto-generated destructor stub
}

int Chat_DB_Request::process_load_offline_private_chat_buff(Block_Buffer &buf) {
	role_id_t role_id = 0;
	int64_t time_sec = 0;
	uint8_t size = 0;
	buf.read_int64( role_id );
	buf.read_int64( time_sec );
	Time_Value time_val = Time_Value(time_sec);
	buf.read_uint8(size);
	std::vector<Chat_Record> chat_record_vec;
	Chat_Record chat_record;
	for(;size>0; size--) {
		chat_record.reset();
		chat_record.deserialize(buf);
		chat_record_vec.push_back(chat_record);
	}
	Chat_Player *player = CHAT_MONITOR->find_chat_player_by_role_id(role_id);

	if (player) {
		MSG_83000001 msg;
		//过滤黑名单
		for ( std::vector<Chat_Record>::iterator iter = chat_record_vec.begin(); iter != chat_record_vec.end(); iter++) {
			std::vector<Chat_Content>::iterator cciter = iter->content.begin();
			if(cciter != iter->content.end() && !player->is_in_black_list(cciter->role_Info.role_id)) {
				msg.chat_record.push_back(*iter);
			}
		}

		//int i = 0;//公会离线消息,只读取30条^C
		std::vector<Chat_Record> *gang_chat_record_vec = CHAT_GANG_MANAGER->get_chat_record_by_id(player->base_detail().gang_id);
		if(gang_chat_record_vec){
			for ( std::vector<Chat_Record>::iterator iter = gang_chat_record_vec->begin(); iter != gang_chat_record_vec->end(); iter++) {
				if(iter->send_time >= player->base_detail().last_sign_out_time.sec()) {
					msg.chat_record.push_back(*iter);
				}
			}
		}
		if(msg.chat_record.size()){
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
	return 0;
}

int Chat_DB_Request::process_load_private_chat_buff(Block_Buffer &buf) {
	role_id_t role_id = 0;
	int64_t target = 0;
	uint8_t size = 0;
	buf.read_int64( role_id );
	buf.read_int64( target );
	buf.read_uint8(size);
	std::vector<Chat_Record> chat_record_vec;
	Chat_Record chat_record;
	for(;size>0; size--) {
		chat_record.reset();
		chat_record.deserialize(buf);
		chat_record_vec.push_back(chat_record);
	}
	Chat_Player *player = CHAT_MONITOR->find_chat_player_by_role_id(role_id);

	if (player) {
		//过滤黑名单
		for ( std::vector<Chat_Record>::iterator iter = chat_record_vec.begin(); iter != chat_record_vec.end(); ) {
			std::vector<Chat_Content>::iterator cciter = iter->content.begin();
			if(cciter == iter->content.end() || player->is_in_black_list(cciter->role_Info.role_id)) {
				iter = chat_record_vec.erase(iter);
			} else {
				iter++;
			}
		}

		player->save_or_update_target_chat_record_map(target, chat_record_vec);

		if(chat_record_vec.size()){
			MSG_50300001 msg;
			msg.chat_record = chat_record_vec;
			msg.role_id = target;
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
	return 0;
}
