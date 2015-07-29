/*
 * Chat_Control_Manager.cpp
 *
 *  Created on: 2014-06-19
 *      Author: xiaoliang
 */

#include <chat_control/Chat_Control_Manager.h>
#include "Chat_Monitor.h"
#include "Chat_Player.h"
#include "DB_Operator.h"

Chat_Control_Manager::Chat_Control_Manager() {
	reset();
}

Chat_Control_Manager::~Chat_Control_Manager() { }

void Chat_Control_Manager::reset() {
}

void Chat_Control_Manager::tick(const Time_Value &now) {

}

void Chat_Control_Manager::start(void) {
	MONGO_OPERATOR->load_chat_control_info(control_info);
}

bool Chat_Control_Manager::is_gag(Chat_Player *player) {
	if(!player) {
		return true;
	}
	Time_Value now = Time_Value::gettimeofday();
	{//验证是否封禁帐号
		String_Time_Map::iterator ait = control_info.account_gag.find(player->base_detail().account_info.account_name);
		if(ait != control_info.account_gag.end()) {
			if(ait->second.sec() == 0 || now.sec() <= ait->second.sec()) {
				return true;
			} else {
				control_info.account_gag.erase(ait);
			}
		}
	}
	{//验证角色ID
		Role_Id_Time_Map::iterator ait = control_info.role_id_gag.find(player->role_id());
		if(ait != control_info.role_id_gag.end()) {
			if(ait->second.sec() == 0 || now.sec() <= ait->second.sec()) {
				return true;
			} else {
				control_info.role_id_gag.erase(ait);
			}
		}
	}
	return false;
}


void Chat_Control_Manager::set_bag_account(std::string &account, uint32_t time) {
	String_Time_Map::iterator it = control_info.account_gag.find(account);
	if(it != control_info.account_gag.end()) {
		it->second.sec(time);
	} else {
		control_info.account_gag.insert(std::make_pair(account, Time_Value(time)));
	}
}

void Chat_Control_Manager::set_role_id_account(role_id_t &role_id, uint32_t time) {
	Role_Id_Time_Map::iterator it = control_info.role_id_gag.find(role_id);
	if(it != control_info.role_id_gag.end()) {
		it->second.sec(time);
	} else {
		control_info.role_id_gag.insert(std::make_pair(role_id, Time_Value(time)));
	}
}

void Chat_Control_Manager::unset_bag_account(std::string &account) {
	String_Time_Map::iterator it = control_info.account_gag.find(account);
	if(it != control_info.account_gag.end()) {
		 control_info.account_gag.erase(it);
	}
}

void Chat_Control_Manager::unset_role_id_account(role_id_t &role_id) {
	Role_Id_Time_Map::iterator it = control_info.role_id_gag.find(role_id);
	if(it != control_info.role_id_gag.end()) {
		control_info.role_id_gag.erase(it);
	}
}
