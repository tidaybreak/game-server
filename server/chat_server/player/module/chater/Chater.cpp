/*
 * Chater.cpp
 *
 *  Created on: May 8, 2014
 *      Author: caijianwei
 */

#include "Chater.h"
#include "Err_Code.h"
#include "Chat_Player.h"
#include "Chat_Monitor.h"
#include "Msg_Active_Struct.h"
#include "module/gang/Chat_Gang_Manager.h"
#include "DB_Manager.h"
#include "DB_Operator.h"
#include "Msg_Chat_Struct.h"
#include "DB_Def.h"
#include "team/Chat_Team_Manager.h"
#include "announcement/Announcement_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Chater_Manager.h"
#include "chat_control/Chat_Control_Manager.h"

Chater::Chater() { reset(); }

Chater::~Chater() {}

void Chater::reset() {
	last_world_chat_time = Time_Value::zero;
	target_chat_record_map.clear();
	send_world_chat_to_logic = false;
}

void Chater::module_init(void) {
	send_world_chat_to_logic = false;
}

int Chater::client_ready_now(void) {
	target_chat_record_map.clear();

	Block_Buffer buf;
	buf.make_message(MONGO_READ_CHAT_RECORD);
	buf.write_int32(READ_OFFLINE);
	buf.write_int64(player_self()->role_id());
	buf.write_int64(player_self()->base_detail().last_sign_out_time.sec());
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);

	return 0;
}

int Chater::req_chat(int8_t district, int8_t type, int64_t target, std::string &target_name, std::vector<Chat_Content>& contents) {
	if(!player_self()->base_detail().role_type) {//普通玩家需要验证一系列聊天内容
		if(player_self()->level() <  10) {
			return ERROR_CLIENT_PARAM;
		}
		//gag
		if(CHATER_CONTROL_MANAGER->is_gag(player_self())) {
			return 0;
		}
		//检测聊天
		if(contents.empty()) {
			return ERROR_CLIENT_PARAM;
		}
		if(contents[0].type != 2) {
			return ERROR_CLIENT_PARAM;
		}
		if(contents[0].role_Info.role_name != player_self()->role_name()
			|| contents[0].role_Info.vip_level != player_self()->vip()) {
			return ERROR_CLIENT_PARAM;
		}
		// check frequency
		if (district == chat_world && chat_too_fast() ) {
			MSG_83000002 msg;
			THIS_SEND_TO_CLIENT(msg);
			return 0;
		}
		if(!CHATER_MANAGER->check_chat_contents_interface(player_self(), contents)) {
			return 0;
		}
		//将游戏内玩家聊天信息发送给平台
		CHATER_MANAGER->send_chat_contents_to_pl(player_self(), target, target_name, contents);
	}
	int chat_res = 0;
	switch( district ) {
		case chat_world: {
			chat_res = chat_in_world(contents);
			last_world_chat_time = Time_Value::gettimeofday();
			break;
		}
		case chat_horn:{
			chat_res = chat_in_horn(contents);
			break;
		}
		case chat_guild:{
			chat_res = chat_in_gang(contents);
			break;
		}
		case chat_system:{ break; }
		case chat_team:{
			chat_res = this->chat_in_team(contents);
			break;
		}
		case chat_player:{
			chat_res = chat_in_private(target, contents);
			break;
		}
	}
	return chat_res;
}

int Chater::req_history_private_chat_record(role_id_t role_id) {
	Role_Id_Chat_Record_Map::iterator iter = target_chat_record_map.find(role_id);
	if(iter != target_chat_record_map.end() && iter->second.size()) {
			MSG_50300001 msg;
			msg.role_id = role_id;
			msg.chat_record = iter->second;
			THIS_SEND_TO_CLIENT(msg);
	} else {
		Block_Buffer buf;
		buf.make_message(MONGO_READ_CHAT_RECORD);
		buf.write_int32(READ_HISTORY);
		buf.write_int64(player_self()->role_id());
		buf.write_int64(role_id);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, 0);
	}

	return 0;
}

int Chater::req_history_gang_chat_record() {
	std::vector<Chat_Record> chat_record_vec;
	Chat_Player *player = player_self();
	if (player) {
		std::vector<Chat_Record> *gang_chat_record_vec = CHAT_GANG_MANAGER->get_chat_record_by_id(player->base_detail().gang_id);
		if(gang_chat_record_vec){
			MSG_50300001 msg;
			msg.role_id = 0;
			msg.chat_record = *gang_chat_record_vec;
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
	return 0;
}

void Chater::save_or_update_target_chat_record_map(role_id_t target, std::vector<Chat_Record>& chat_record_vec) {
	Role_Id_Chat_Record_Map::iterator iter = target_chat_record_map.find(target);
	if(iter != target_chat_record_map.end()) {
		iter->second = chat_record_vec;
	} else {
		target_chat_record_map.insert(std::make_pair(target, chat_record_vec));
	}
}

int Chater::chat_in_world(const std::vector<Chat_Content>& contents) {
	{//聊天服通知逻辑服进行了一次世界聊天
		if(!send_world_chat_to_logic) {
			send_world_chat_to_logic = true;
			MSG_20200021 msg;
			msg.reset();
			THIS_SEND_TO_LOGIC(msg);
		}
	}
	return broadcast_contents(chat_world, 0, contents);
}

int Chater::chat_in_bugle(const std::vector<Chat_Content>& contents) {
	return chat_in_horn(contents);
}

int Chater::chat_in_horn(const std::vector<Chat_Content>& contents) {
	// check frequency
//	if ( chat_too_fast() ) return ERROR_CHAT_TIME_LIMIT;
	return broadcast_contents(chat_horn, 0, contents);
}


int Chater::chat_in_gang(const std::vector<Chat_Content>& contents) {
	// check frequency
//	if ( chat_too_fast() ) return ERROR_CHAT_TIME_LIMIT;
	Chat_Player* self = player_self();
	if ( !self ) return ERROR_INNER_SERVER;
	int64_t gang_id = self->base_detail().gang_id;
	if ( gang_id == 0 ) return ERROR_GANG_NOT_IN_GANG;
	Chat_Gang_Info* gang_info = CHAT_GANG_MANAGER->gang_info_by_id( gang_id );
	if ( !gang_info ) return ERROR_GANG_NOT_EXIST;

	//save_to gang manager
	Chat_Record chat_record;
	chat_record.reset();
	chat_record.chat_type = chat_guild;
	chat_record.send_time = Time_Value::gettimeofday().sec();
	chat_record.content = contents;
	CHAT_GANG_MANAGER->save_chat_record(gang_id, chat_record);
	//end save to gang manager

	MSG_83000000 msg;
	msg.chat_type = chat_guild;
	msg.system_type = 0;
	msg.role_type = player_self()->base_detail().role_type;
	msg.content = contents;
	for ( Role_Hash_Set::iterator iter = gang_info->members.begin(); iter != gang_info->members.end(); ++iter) {
		Chat_Player *player = self->monitor()->find_chat_player_by_role_id( (*iter) );
		if (player) {
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
	return 0;
}

int Chater::chat_in_private(int64_t target,	const std::vector<Chat_Content>& contents) {
	Chat_Player* self = player_self();
	if ( !self ) return ERROR_INNER_SERVER;

	//start save to mongodb
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_CHAT_RECORD);
	buf.write_int64(self->role_id());
	buf.write_int64(target);
	buf.write_uint8(chat_player);
	buf.write_int8(contents.size());
	for(std::vector<Chat_Content>::const_iterator it = contents.begin(); it != contents.end(); it++) {
		it->serialize(buf);
	}
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	//end save to mongodb

	Chat_Player *player = self->monitor()->find_chat_player_by_role_id(target);
	if (player && !player->is_in_black_list(self->role_id())) {
		MSG_83000000 msg;
		msg.chat_type = chat_player;
		msg.system_type = 0;
		msg.role_type = player_self()->base_detail().role_type;
		msg.content = contents;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}

	return 0;
}

int Chater::chat_in_team(const std::vector<Chat_Content>& contents) {
	int64_t time_id = player_self()->get_team_id();
	Team *team = CHAT_TEAM_MANAGER->get_team(time_id);
	if(team) {
		MSG_83000000 msg;
		msg.chat_type = chat_team;
		msg.system_type = 0;
		msg.role_type = player_self()->base_detail().role_type;
		msg.content = contents;
		Role_Hash_Set members;
		team->team_all_member_set(members);
		for(Role_Hash_Set::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
			Chat_Player *player = player_self()->monitor()->find_chat_player_by_role_id( (*iter) );
			if (player) {
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}
		}
	}
	return 0;
}



int Chater::broadcast_contents(int8_t district, int32_t type, const std::vector<Chat_Content>& contents) {
	MSG_83000000 msg;
	msg.chat_type = district;
	msg.system_type = type;
	msg.role_type = player_self()->base_detail().role_type;
	msg.content = contents;

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	player_self()->monitor()->send_to_all_players(buf);
	return 0;
}

bool Chater::chat_too_fast() {
	bool test = (last_world_chat_time+Chat_CD_in_World) > Time_Value::gettimeofday();
	return test;
}


int Chater::send_input_status(role_id_t target, int8_t status) {
	Chat_Player *player = player_self()->monitor()->find_chat_player_by_role_id(target);
	if (player) {
		MSG_50300003 msg;
		msg.role_id = this->player_self()->role_id();
		msg.status = status;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}
	return 0;
}
