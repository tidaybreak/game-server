/*
 * Scene_War_Manager.cpp
 *
 *  Created on: 2014年7月3日
 *      Author: root
 */

#include "Scene_War_Manager.h"
#include "war_scene/War_Scene_Manager.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Scene.h"
#include "fighter/Fighter_Def.h"


Scene_War_Manager::Scene_War_Manager(void){
	int zone_count = MAX_LEVEL/WAR_SCENE_ZONE_STAGE;
	for(int i=0; i <= zone_count; ++i){
		Honour_Zone zone;
		zone.id = i;
		honour_zone_map_.insert(std::make_pair(i, zone));
	}

}

Scene_War_Manager::~Scene_War_Manager(void){

}

int Scene_War_Manager::req_enter_war_scene(Monitor_Unique_ID& logic_muid, role_id_t role_id , int level, int force, role_id_t gang_id,
		int remain_income_num, int income_rate, int total_merit, int remain_time, const int act_time, std::string server_name){
	//check

	//
	Honour_Player honour_player_info;
	honour_player_info.role_id = role_id;
	honour_player_info.level = level;
	honour_player_info.force = force;
	honour_player_info.gang_id = gang_id;
	honour_player_info.remain_income_num = remain_income_num;
	honour_player_info.income_rate = income_rate;
	honour_player_info.total_merit = total_merit;
	honour_player_info.remain_time = remain_time;
	honour_player_info.act_time = act_time;
	honour_player_info.server_name = server_name;
	honour_player_map_.insert(std::make_pair(honour_player_info.role_id, honour_player_info));
	Honour_Player* h_player = get_honour_player(honour_player_info.role_id);
	if(!h_player){
		return -1;
	}
	if(join_honour_field(h_player) != 0){
		return -1;
	}
	//
	Coord coord;
	const Scene_Config* scene_config = CONFIG_CACHE_SCENE->scene_config_cache(80207001);
	if(scene_config){
		Scene_Config::Transmit_Coord_Map::const_iterator it;
		it = scene_config->transmit_coord.find(h_player->league+1);
		if(it != scene_config->transmit_coord.end()){
			coord = it->second.coord;
		}
	}
	// send to logic
	Block_Buffer buf;
	buf.make_message(INNER_WAR_SCENE_ENTER);
	buf.write_int64(h_player->role_id);
	buf.write_int64(h_player->honor_field_id);
	buf.write_int32(h_player->league+1);
	coord.serialize(buf);
	buf.finish_message();

	SCENE_MONITOR->send_to_logic(logic_muid.uuid, buf);
	return 0;
}

int Scene_War_Manager::req_leave_war_scene(role_id_t role_id){
	Honour_Player* h_player = this->get_honour_player(role_id);
	if(!h_player){
		return -1;
	}
	Honour_Field* h_field = this->get_honour_field(h_player->honor_field_id);
	if(!h_field){
		return -1;
	}
	Honour_Zone* h_zone = this->get_honour_zone(h_field->zone_id);
	if(!h_zone){
		return -1;
	}
	h_zone->del_player(h_player);
	this->del_honour_player(role_id);
	return 0;
}

int Scene_War_Manager::honour_field_finish(role_id_t field_id){
	Honour_Field* h_field = this->get_honour_field(field_id);
	if(!h_field){
		return -1;
	}
	Honour_Zone* h_zone = this->get_honour_zone(h_field->zone_id);
	if(!h_zone){
		return -1;
	}
	boost::unordered_set<role_id_t>::iterator it;
	for(it = h_field->player_set.begin(); it !=  h_field->player_set.end(); ++it){
		this->del_honour_player(*(it));
	}
	h_zone->del_field(h_field);
	this->del_honour_field(h_field->id);
	return 0;

}

int Scene_War_Manager::join_honour_field(Honour_Player* h_player){
	int zone_id = get_zone_id_from_level(h_player->level);
	Honour_Zone* h_zone = get_honour_zone(zone_id);
	if(!h_zone){
		return -1;
	}
	Honour_Field* h_field = h_zone->get_best_honour_field(h_player);
	if(h_field){
		h_player->honor_field_id = h_field->id;
		h_player->league = h_field->enter_league;
		h_zone->add_player(h_player);
		return 0;
	}else{
		// 区间内不足构成一个战场，向当前战场数最多的一个区间方向，自动偏移一个区间段
		//（30-34段人数不够，人数最多的为50-54的，则30-34段的人自动匹配至35-39段）
		int largest_zone_id = 0;
		int largest_num = 0;
		this->get_largest_num_honour_zone(largest_zone_id, largest_num);
		int target_zone = 0;
		if(largest_zone_id == 0 || largest_num <= h_zone->zone_player_num){
			target_zone = h_zone->id;
		}else{
			int tmp_target_zone = 0;
			if(h_zone->id > largest_zone_id){
				tmp_target_zone = h_zone->id - 0;
			}else{
				tmp_target_zone = h_zone->id + 0;
			}
			Honour_Zone* zone = get_honour_zone(tmp_target_zone);
			if(zone){
				Honour_Field* field = zone->get_best_honour_field(h_player);
				if(field){
					h_player->honor_field_id = field->id;
					h_player->league = field->enter_league;
					target_zone = zone->id;
				}
			}
			if(target_zone == 0){
				target_zone = h_zone->id;
			}
		}
		h_zone = this->get_honour_zone(target_zone);
		if(!h_zone){
			return -1;
		}
		if(h_player->honor_field_id == 0){
			h_field = this->genarate_honour_field();
			if(!h_field){
				return -1;
			}
			h_field->remain_time = h_player->remain_time;
			h_field->act_time = h_player->act_time;
			h_player->honor_field_id = h_field->id;
			h_zone->add_field(h_field);
		}
		h_zone->add_player(h_player);
		return 0;
	}
	return 0;
}

Honour_Field* Scene_War_Manager::genarate_honour_field(void){
	Honour_Field h_field;
	h_field.id = WAR_SCENE_MANAGER->genarate_war_scene_order();
	honour_field_map_.insert(std::make_pair(h_field.id, h_field));
	return this->get_honour_field(h_field.id);
}

Honour_Player* Scene_War_Manager::get_honour_player(role_id_t role_id){
	Honour_Player_Map::iterator it = honour_player_map_.find(role_id);
	if( it != honour_player_map_.end() ){
		return &(it->second);
	}
	return NULL;
}

Honour_Field* Scene_War_Manager::get_honour_field(role_id_t field_id){
	Honour_Field_Map::iterator it = honour_field_map_.find(field_id);
	if( it != honour_field_map_.end() ){
		return &(it->second);
	}
	return NULL;
}

Honour_Zone* Scene_War_Manager::get_honour_zone(int zone_id){
	Honour_Zone_Map::iterator it = honour_zone_map_.find(zone_id);
	if(it != honour_zone_map_.end()){
		return &(it->second);
	}
	return NULL;
}

void Scene_War_Manager::del_honour_player(role_id_t role_id){
	honour_player_map_.erase(role_id);
}

void Scene_War_Manager::del_honour_field(role_id_t field_id){
	honour_field_map_.erase(field_id);
}

int Scene_War_Manager::get_zone_id_from_level(int level){
	int zone_id = level/WAR_SCENE_ZONE_STAGE;
	return zone_id;
}

int Scene_War_Manager::get_largest_num_honour_zone(int& zone_id, int& player_num){
	zone_id = 0;
	player_num = 0;
	Honour_Zone_Map::iterator it;
	for(it = honour_zone_map_.begin(); it != honour_zone_map_.end(); ++it){
		if(it->second.zone_player_num > player_num){
			zone_id = it->second.id;
			player_num = it->second.zone_player_num;
		}
	}
	return 0;
}
