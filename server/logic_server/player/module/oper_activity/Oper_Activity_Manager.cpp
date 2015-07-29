/*
 * Oper_Activity_Manager.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include <oper_activity/Oper_Activity_Manager.h>
#include "Config_Cache_Oper_Activity.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"

Oper_Activity_Manager::Oper_Activity_Manager() {
	// TODO Auto-generated constructor stub
	reset();
}

Oper_Activity_Manager::~Oper_Activity_Manager() {
	// TODO Auto-generated destructor stub
}

void Oper_Activity_Manager::reset(void) {

}

int Oper_Activity_Manager::start(void) {

	return 0;
}

void Oper_Activity_Manager::tick(Time_Value &now) {

}

void Oper_Activity_Manager::open(const Time_Value &start, const Time_Value &end) {
	act_start = start;
	act_end = end;
}

void Oper_Activity_Manager::close(const Time_Value &end) {
	act_end = end;
}

void Oper_Activity_Manager::open_item(const uint32_t act_id, const Time_Value &start, const Time_Value &end) {
	start_time[act_id] = start;
	end_time[act_id] = end;
	try_append_effect(act_id);
}

void Oper_Activity_Manager::close_item(const uint32_t act_id, const Time_Value &end) {
	end_time[act_id] = end;
	try_close_append_effect(act_id);
}

const Time_Value &Oper_Activity_Manager::get_item_start_time(const uint32_t act_id) {
	return 	start_time[act_id];
}

const Time_Value &Oper_Activity_Manager::get_item_end_time(const uint32_t act_id) {
	return 	end_time[act_id];
}

bool Oper_Activity_Manager::item_is_open(const uint32_t act_id) {
	Time_Value now = Time_Value::gettimeofday();
	if(act_start.sec() <= now.sec() && now.sec() < act_end.sec()) {
		if(start_time[act_id].sec() <= now.sec() && now.sec() < end_time[act_id].sec()) {
			return true;
		}
	}
	return false;
}

void Oper_Activity_Manager::try_close_append_effect(const uint32_t act_id) {
	if(!item_is_open(act_id)) return;
	const Oper_Activity_Project_Config *conf = CONFIG_CACHE_OPER_ACTIVITY->get_project_config_by_id(act_id);
	if(conf) {
		if(!conf->append_effect.empty()) {
			const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
			for(Oper_Activity_Append_Effect_Config_Map::const_iterator ae_iter = conf->append_effect.begin(); ae_iter != conf->append_effect.end(); ++ae_iter) {//遍历特殊效果
				switch(ae_iter->second.type) {
					case OPER_ACTIVITY_APPEND_BUFF_EFFECT: {
						for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {//先给在线玩家加上
							it->second->oper_activity_close_append_effect_buff_type(*conf, ae_iter->second);
						}
						break;
					}
				}
			}
		}
	}
}

void Oper_Activity_Manager::try_append_effect(const uint32_t act_id) {
	if(!item_is_open(act_id)) return;
	const Oper_Activity_Project_Config *conf = CONFIG_CACHE_OPER_ACTIVITY->get_project_config_by_id(act_id);
	if(conf) {
		if(!conf->append_effect.empty()) {
			const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
			for(Oper_Activity_Append_Effect_Config_Map::const_iterator ae_iter = conf->append_effect.begin(); ae_iter != conf->append_effect.end(); ++ae_iter) {//遍历特殊效果
				for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {//先给在线玩家加上
					it->second->oper_activity_append_effect(*conf, ae_iter->second);
				}
			}
		}
	}
}
