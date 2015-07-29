/*
 * Config_Cache_Achievement.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: Linqiyou
 */

#include "Config_Cache_Achievement.h"
#include "Configurator.h"
#include "Json_Utility.h"

Achievement_Cfg_Detail_Map &Config_Cache_Achievement::achievement_cfg(void) {
	return this->achievement_cfg_detail_map_;
}

const Achievement_Cfg_Detail *Config_Cache_Achievement::find_achievement_cfg(const int32_t id) const {
	Achievement_Cfg_Detail_Map::const_iterator it = achievement_cfg_detail_map_.find(id);
	if (it != achievement_cfg_detail_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

Achievement_Cfg_Setting &Config_Cache_Achievement::achievement_cfg_setting(void) {
	return this->achievement_cfg_setting_;
}
const Achievement_Cfg_Medal_Detail *Config_Cache_Achievement::find_achievement_medal(const int32_t id) const{
	Achievement_Cfg_Medal_Map::const_iterator it = achievement_cfg_medal_map_.find(id);
	if (it != achievement_cfg_medal_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

void Config_Cache_Achievement::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Achievement");

	achievement_cfg_detail_map_.clear();
	Achievement_Cfg_Detail info;
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["achievement"]["achievement"];
	for (Json::Value::const_iterator be_it = config_json.begin();
			be_it != config_json.end(); ++be_it) {
		info.reset();
		info.id = (*be_it)["id"].asInt();
		info.name = (*be_it)["name"].asString();
		info.num = (*be_it)["num"].asInt();
		if (!(*be_it)["award"].isNull() && (*be_it)["award"].isArray()) {
			Json_Utility::assign_item_vec((*be_it)["award"], info.award);
		}
		info.type = (*be_it)["type"].asInt();
		info.cond = (*be_it)["cond"].asInt();
		info.cond_ext = (*be_it)["cond_ext"].asInt();
//		info.cond_ext1 = (*be_it)["cond_ext1"].isArray();
		info.broadcast = (*be_it)["broadcast"].asInt();
		info.des = (*be_it)["des"].asString();

		set_map_second_value_by_key(info.id, achievement_cfg_detail_map_, info);
	}

	achievement_cfg_setting_.reset();
	const Json::Value& setting_json = CONFIG_INSTANCE->config_json()["achievement"]["setting"];
	for (Json::Value::const_iterator be_it = setting_json.begin();
			be_it != setting_json.end(); ++be_it) {
		if (!(*be_it)["prop"].isNull() && (*be_it)["prop"].isArray()) {
			for (Json::Value::const_iterator armor_it = (*be_it)["prop"].begin();
					armor_it != (*be_it)["prop"].end(); ++armor_it) {
				if (!(*armor_it).isNull() && (*armor_it).isArray()) {
					achievement_cfg_setting_.prop.push_back(Property((*armor_it)[0u].asInt(), (*armor_it)[1u].asInt()));
				}
			}
		}
//		achievement_cfg_setting_.armor = (*be_it)["armor"].asInt();
		achievement_cfg_setting_.bind_diamond = (*be_it)["bind_diamond"].asInt();
		break;
	}

	{
		achievement_cfg_medal_map_.clear();
		Achievement_Cfg_Medal_Detail info;
		const Json::Value& config_json = CONFIG_INSTANCE->config_json()["achievement"]["medal_setting"];
		for (Json::Value::const_iterator be_it = config_json.begin();
				be_it != config_json.end(); ++be_it) {
			info.reset();
			info.id = (*be_it)["id"].asInt();
			info.type = (*be_it)["type"].asInt();
			info.num = (*be_it)["num"].asInt();
			info.profile = (*be_it)["profile"].asInt();

			set_map_second_value_by_key(info.id, achievement_cfg_medal_map_, info);
		}
	}
}
