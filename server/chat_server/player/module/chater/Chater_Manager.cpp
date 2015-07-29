/*
 * Chater_Manager.cpp
 *
 *  Created on: 2014-06-19
 *      Author: xiaoliang
 */

#include "Chater_Manager.h"
#include "Msg_Struct.h"
#include "gang/Chat_Gang_Manager.h"
#include "Chat_Monitor.h"
#include "Chat_Player.h"
#include "Msg_Active_Struct.h"
#include "Err_Code.h"
#include "DB_Manager.h"
#include "announcement/Announcement_Struct.h"
#include "Chat_Player.h"
#include <openssl/md5.h>
#include "Config_Cache_Chat.h"
#include "http_client/Http_Manager.h"
#include "http_client/Http_Struct.h"
#include "Config_Cache.h"

Chater_Manager::Chater_Manager() {
	reset();
}

Chater_Manager::~Chater_Manager() { }

void Chater_Manager::reset() {
}

void Chater_Manager::tick(const Time_Value &now) {
	if(now.sec() >= chat_info.last_tick_time.sec()) {
		uint32_t gag_num = CONFIG_CACHE_CHAT->get_common_cfg_by_key(CHAT_CHECK_COMM_GAP_NUM);
		chat_info.contents_num.clear();
		for(String_Vec::iterator it = chat_info.contets.begin(); it != chat_info.contets.end(); ++it) {
			++chat_info.contents_num[*it];
		}
		for(String_Int_Map::iterator it = chat_info.contents_num.begin(); it != chat_info.contents_num.end(); ++it) {
			int num = it->second;
			if((uint32_t)num >= CONFIG_CACHE_CHAT->get_common_cfg_by_key(CHAT_CHECK_COMM_GAP_COUNT) && chat_info.ban_contents.find(it->first) == chat_info.ban_contents.end()) {
				MSG_DEBUG("chat manager ban str:%s", it->first.c_str());
				chat_info.ban_contents.insert(it->first);
			}
		}
		for(RoleID_UInt_Vec_Map::iterator it = chat_info.role_id_gap_num.begin(); it != chat_info.role_id_gap_num.end(); ++it) {
			if(gag_num > 0 && it->second.size() >= gag_num
					&& chat_info.ban_role_id.find(it->first) == chat_info.ban_role_id.end()) {
				MSG_DEBUG("chat manager ban role_id:%ld", it->first);
				chat_info.ban_role_id.insert(it->first);
			}
		}
		chat_info.last_tick_time.sec(now.sec()+Time_Value::ONE_MINUTE_IN_SECS);
	}
}

int Chater_Manager::send_announce(int8_t type, int32_t ann_id, int64_t gang_id, const std::vector<role_id_t>& recievers, const std::vector<Chat_Content>& contents) {
	if(recievers.empty()) {
		switch(type) {
			case ANNOUNCE_DISTRICT_GANG: {
				return announce_in_gang(gang_id, ann_id, contents);
				break;
			}
			default: {
				return announce_in_broadcast(type, ann_id, contents);
			}
		}
	} else {
		return announce_in_role_vec(recievers, contents, type, ann_id);
	}
	return 0;
}

int Chater_Manager::player_trigger_daily_zero(void) {

	return 0;
}

int Chater_Manager::announce_in_role_vec(const std::vector<role_id_t>& recievers, const std::vector<Chat_Content>& contents, uint8_t type, int32_t ann_id) {
	if(recievers.empty()) {
		return -1;
	}
	MSG_83000000 msg;
	msg.chat_type = type;
	msg.system_type = ann_id;
	msg.content = contents;

	for( std::vector<role_id_t>::const_iterator iter = recievers.begin(); iter != recievers.end(); ++iter ) {
		Chat_Player* player = CHAT_MONITOR->find_chat_player_by_role_id( *iter );
		if ( player ) {
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
	return 0;
}


int Chater_Manager::announce_in_gang(int64_t gang_id, int ann_id, const std::vector<Chat_Content>& contents) {
	MSG_83000000 msg;
	msg.chat_type = ANNOUNCE_DISTRICT_GANG;
	msg.system_type = ann_id;
	msg.content = contents;
	if(gang_id) {
		Chat_Gang_Info* gang_info = CHAT_GANG_MANAGER->gang_info_by_id( gang_id );
		if ( !gang_info ) return ERROR_GANG_NOT_EXIST;

		for ( Role_Hash_Set::iterator iter = gang_info->members.begin(); iter != gang_info->members.end(); ++iter) {
			Chat_Player *player = CHAT_MONITOR->find_chat_player_by_role_id( (*iter) );
			if (player) {
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}
		}
	} else {
		const Chat_Gang_Manager::Gang_ID_Info_Map &gang_map = CHAT_GANG_MANAGER->get_gang_map();
		for(Chat_Gang_Manager::Gang_ID_Info_Map::const_iterator it = gang_map.begin(); it != gang_map.end(); ++it) {
			for ( Role_Hash_Set::const_iterator iter = it->second.members.begin(); iter != it->second.members.end(); ++iter) {
				Chat_Player *player = CHAT_MONITOR->find_chat_player_by_role_id( (*iter) );
				if (player) {
					OBJ_SEND_TO_CLIENT(msg, (*player));
				}
			}
		}
	}
	return 0;
}


int Chater_Manager::announce_in_broadcast(int8_t type, int32_t ann_id, const std::vector<Chat_Content>& contents) {

	MSG_83000000 msg;
	msg.chat_type = type;
	msg.system_type = ann_id;
	msg.content = contents;

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	CHAT_MONITOR->send_to_all_players(buf);
	return 0;
}

void Chater_Manager::switch_verification_chat(void) {
	MSG_DEBUG("switch_verification_chat");
	bool status = !chat_info.verification;
	chat_info.reset();
	chat_info.verification = status;
}

void Chater_Manager::not_verification_chat(void) {
	MSG_DEBUG("not_verification_chat");
	chat_info.reset();
	chat_info.verification = false;
}

void Chater_Manager::send_chat_contents_to_pl(Chat_Player *player, int64_t target,  std::string &target_name, const std::vector<Chat_Content>& contents) {
	Http_Info info;
	info.url = CONFIG_CACHE->server_maintainer_cache().chat_monitor_url;
	if(info.url.empty()) {
		return;
	}
	info.uri["game_key"]="tod";//游戏英文代号（与平台一致）
	{
//		std::stringstream stream;
//		stream << CONFIG_CACHE->server_flag();
		info.uri["server_id"]=CONFIG_CACHE->server_maintainer_cache().pt_server_id;//服ID
	}
	{
		std::stringstream stream;
		stream << Time_Value::gettimeofday().sec();
		info.uri["time"]=stream.str();//聊天时间，Unix时间戳，单位秒
	}
	info.uri["login_account"]=player->base_detail().account_info.account_name;//37平台账号
//	info.uri["actor"]=player->base_detail().role_name;//角色名
//	{
//		std::stringstream stream;
//		stream << player->base_detail().role_id;
//		info.uri["actor_id"]=stream.str();//角色ID（可选）
//	}
	if(target) {
		Chat_Player *tplayer = player->monitor()->find_chat_player_by_role_id(target);
		if (tplayer && !tplayer->is_in_black_list(player->role_id())) {
			info.uri["to_login_account"]=tplayer->base_detail().account_info.account_name;// 发送给谁，平台账号
			info.uri["to_actor"]=tplayer->role_name();// 发送给谁，角色名
		} else {
			info.uri["to_login_account"]="";// todo 发送给谁，平台账号
			info.uri["to_actor"]=target_name;// 发送给谁，角色名
		}
	} else {
		info.uri["to_login_account"]="";//  发送给谁，平台账号
		info.uri["to_actor"]="";// 发送给谁，角色名
	}

    {
	    get_chat_contents_to_string(contents, info.uri["content"]);//聊天内容
    }
    info.uri["ip"]=player->base_detail().ip;//玩家IP
    {
		//签名验证，参数相连接。md5后的结果小写
		// key为接口密钥，由平台技术发放
		// md5($key.$server_id.$login_account.$actor.$actor_id.$content.$time)
    	std::string key = CONFIG_CACHE->server_maintainer_cache().chat_monitor_key; //得到KEY
    	std::stringstream stream;
    	stream << key << info.uri["server_id"] << info.uri["login_account"]/* << info.uri["actor"] << info.uri["actor_id"]*/ << info.uri["content"] << info.uri["time"];
    	md5_string(stream.str(), info.uri["sign"]);
    }
	info.op_type = HTTP_OP_CHAT_MONITOR;
	info.type = HTTP_GET;
	HTTP_MANAGER->push_data_block(info);
}

bool Chater_Manager::check_chat_contents_interface(Chat_Player *player, const std::vector<Chat_Content>& contents) {
	if(!player) { return false; }
	//检测开关
	if(!chat_info.verification) { return true; }
	std::string str_chat;// md5_str = "";
	get_chat_contents_to_string(contents, str_chat);
	//判断字符大小
	const uint32_t len = CONFIG_CACHE_CHAT->get_common_cfg_by_key(CHAT_CHECK_COMM_CHR_LEN);
	if(len && str_chat.size() < len) {
		return true;
	}
	//检测白名单
	if(verification_chat_white_list(str_chat)) {
		return true;
	}
	//检测黑名单
	if(verification_chat_black_list(str_chat)) {
		return false;
	}
//	md5_string(str_chat, md5_str);
	//过滤聊天检测
	if(!verification_chat(player->role_id(), str_chat)) {
		return false;
	}
	//记录聊天内容=>次数//记录聊天间隔
	record_chat_contents_text_num(player->role_id(), str_chat);
	return true;
}

void Chater_Manager::record_chat_contents_text_num(role_id_t role_id, const std::string & str) {
	Time_Value now = Time_Value::gettimeofday();

	chat_info.contets.push_back(str);
	if(chat_info.contets.size() > (uint32_t)CONFIG_CACHE_CHAT->get_common_cfg_by_key(CHAT_CHECK_COMM_CONTENTS_LEN)) {
		chat_info.contets.erase(chat_info.contets.begin());
	}
	Role_Id_Time_Map::iterator iter = chat_info.role_id_last_chat_time.find(role_id);
	if(iter != chat_info.role_id_last_chat_time.end()) {
		int slep = now.sec() - iter->second.sec();
		uint32_t slep_conf = CONFIG_CACHE_CHAT->get_common_cfg_by_key(CHAT_CHECK_COMM_GAP_TIME);
		if(slep >= 0 && slep_conf > 0 && (uint32_t)slep <= slep_conf) {
			chat_info.role_id_gap_num[role_id].push_back(now.sec());
			for(UInt_Vec::iterator it = chat_info.role_id_gap_num[role_id].begin(); it != chat_info.role_id_gap_num[role_id].end(); ) {
				if(*it + CONFIG_CACHE_CHAT->get_common_cfg_by_key(CHAT_CHECK_COMM_GAP_TIME) < now.sec()) {
					it = chat_info.role_id_gap_num[role_id].erase(it);
				} else {
					break;
				}
			}
		}
	}
	chat_info.role_id_last_chat_time[role_id] = now;
}

void Chater_Manager::get_chat_contents_to_string(const std::vector<Chat_Content>& contents, std::string &str_res) {
	std::stringstream stream;
	for(std::vector<Chat_Content>::const_iterator iter = contents.begin(); iter != contents.end(); ++iter) {
		if(iter->type == 0) {
			stream << iter->text;
		}
	}
	str_res = stream.str();
}

void Chater_Manager::md5_string(const std::string &str_src, std::string &str_res) {
	const unsigned char *result_md5 = MD5((const unsigned char *) str_src.c_str(), str_src.length(), 0);
	char char_str_res[50];
	memset(char_str_res, 0, sizeof(char_str_res));
	for (uint i = 0; i < 16; i++) {
		snprintf(&char_str_res[i * 2], sizeof(char_str_res), "%.2x", result_md5[i]);
	}
	str_res.assign(char_str_res);
//	str_res = (*result_md5);
	return;
}

bool Chater_Manager::verification_chat(const role_id_t role_id, const std::string & str) {
	if(chat_info.ban_contents.find(str) != chat_info.ban_contents.end()) {
		return false;
	}
	if(chat_info.ban_role_id.find(role_id) != chat_info.ban_role_id.end()) {
		return false;
	}
	return true;
}

bool Chater_Manager::verification_chat_white_list(const std::string & str) {
	const String_Set &list = CONFIG_CACHE_CHAT->get_white_list();
	for(String_Set::const_iterator it = list.begin(); it != list.end(); ++it) {
		if(str.find(*it) != std::string::npos) {
			return true;
		}
	}
	return false;
}

bool Chater_Manager::verification_chat_black_list(const std::string & str) {
	const String_Set &list = CONFIG_CACHE_CHAT->get_black_list();
	for(String_Set::const_iterator it = list.begin(); it != list.end(); ++it) {
		if(str.find(*it) != std::string::npos) {
			return true;
		}
	}
	return false;
}
