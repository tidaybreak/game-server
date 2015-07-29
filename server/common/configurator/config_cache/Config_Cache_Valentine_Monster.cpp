/*
 * Config_Cache_Valentine_Monster.cpp
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */

#include "Cache_Base.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_Valentine_Monster.h"


void Config_Cache_Valentine_Monster::refresh_config_cache(void){
	PERF_MON("Config_Cache_Valentine_Monster");
	refresh_monster_cfg();
}

void Config_Cache_Valentine_Monster::refresh_monster_cfg(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["activities_special"]["monster"];
	cfg_monster_map_.clear();
	Valentine_Monster_Config cache;
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("scene_key")){
			cache.scene_key = config_json["scene_key"].asInt();
		}
		if (config_json.isMember("type")){
			cache.type = config_json["type"].asInt();
		}
		if (config_json.isMember("id")){
			cache.id = config_json["id"].asInt();
		}
		if (config_json.isMember("monster_amount")){
			cache.monster_amount = config_json["monster_amount"].asInt();
		}
		if (config_json.isMember("refresh_time")){
			cache.refresh_time = config_json["refresh_time"].asInt();
		}
		if (config_json.isMember("fight_time")){
			cache.fight_time = config_json["fight_time"].asInt();
		}
		if (config_json.isMember("birth_radius")){
			cache.birth_radius = config_json["birth_radius"].asInt();
		}

		if (config_json.isMember("birth_coord")){
			Json_Utility::assign_coord_list(config_json["birth_coord"], cache.birth_coord);
		}
		if (config_json.isMember("boss_reward")){
			Json_Utility::assign_int_int_vec(config_json["boss_reward"], cache.boss_reward, 1);
		}
		//
		Vlt_Monster_Scene_Map::iterator it = cfg_monster_map_.find(cache.scene_key);
		if(it != cfg_monster_map_.end()){
			it->second[cache.id] = cache;
		}else{
			Vlt_Monster_Point_Map point_map;
			point_map[cache.id] = cache;
			cfg_monster_map_[cache.scene_key] = point_map;
		}
	}
}

Vlt_Monster_Point_Map* Config_Cache_Valentine_Monster::get_monster_point_map(const int scene_id){
	Vlt_Monster_Scene_Map::iterator it = cfg_monster_map_.find(scene_id);
	if(it != cfg_monster_map_.end()){
		return &(it->second);
	}
	return NULL;
}
