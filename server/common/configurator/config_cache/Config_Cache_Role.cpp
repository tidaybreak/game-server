/*
 * Config_Cache_Role.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 */

#include "Config_Cache_Role.h"
#include "Configurator.h"
#include "Utility_Func.h"
#include "Json_Utility.h"
#include "Record_Client.h"
#include "fighter/Fighter_Def.h"
#include "Config_Cache_Scene.h"


void Player_Name_Depot::reset(void) {
	family_name_depot.clear();
	m_name_depot.clear();
	f_name_depot.clear();
}

void Player_Init_Cache::reset(void) {
	birth_scene = 0;
	birth_coord.reset();

	safe_scene = 0;
	safe_coord.reset();

	level = 0;
	default_skill.clear();

	initial_attribute.clear();
	prop_conversion.clear();
	first_single_id = 0;
	first_nor_elite_id = 0;
	first_hor_elite_id = 0;
}

const int PLAYER_PROPS[] = {PT_POWER, PT_AGILITY, PT_DEXTERITY, PT_ARMOR, PT_STAMINA,
			PT_PHY_ATTACK, PT_SPELL_ATTACK, PT_PHY_DEFENSE, PT_SPELL_DEFENSE, PT_HIT,
			PT_AVOID, PT_BLOOD_MAX, PT_CRIT, PT_CRIT_DEF, PT_SKILL_POINT,
			PT_BLOOD_POOL_MAX, PT_BLOOD_POOL_CURRENT
};

const int PLAYER_INIT_PROPS[] = {
		PT_MORALE_CAPS, PT_STRENGTH_MAX, PT_CRIT_HURT, PT_CURE_RATE, PT_BE_CURE_RATE, PT_STATUS_HIT_RATE, PT_STATUS_DEF_RATE, PT_SING_RATE, PT_SPEED
};

const int CONVERSION_PROPS[] = {
			PT_PHY_ATTACK, PT_SPELL_ATTACK, PT_HIT, PT_CRIT, PT_AVOID, PT_CRIT_DEF, PT_PHY_DEFENSE, PT_SPELL_DEFENSE, PT_BLOOD_MAX
};


Config_Cache_Role::Config_Cache_Role(void) {
}

Config_Cache_Role::~Config_Cache_Role(void) {}

void Config_Cache_Role::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Role");

	refresh_error_tip_cache();
	refresh_fight_prop_cache();
	refresh_level_fighter_prop();
	refresh_player_init_cache();
	refresh_career_tips();

}

void Config_Cache_Role::refresh_player_init_cache(void) {
	player_init_cache_.reset();

	{
		// 创号初始化
		const Json::Value &json_initial_role = CONFIG_INSTANCE->config_json()["player"]["initial_role"][0u];
		if (json_initial_role == Json::Value::null) LOG_ABORT("");

		player_init_cache_.birth_scene = json_initial_role["birth_scene"].asInt();
		Json_Utility::assign_coord(json_initial_role["birth_coord"], player_init_cache_.birth_coord);

		{
			const Scene_Config *scene = CONFIG_CACHE_SCENE->scene_config_cache(player_init_cache_.birth_scene);
			if (!scene) LOG_ABORT("scene config not exist:%d", player_init_cache_.birth_scene);
			int map_value = CONFIG_CACHE_SCENE->get_map_value(scene->map_id, player_init_cache_.birth_coord);
			if (map_value <= 0) {
				LOG_ABORT("scene id:%d,  x:%d, y:%d", player_init_cache_.birth_scene, player_init_cache_.birth_coord.x, player_init_cache_.birth_coord.y);
			}
		}

		player_init_cache_.safe_scene = json_initial_role["safe_scene"].asInt();
		Json_Utility::assign_coord(json_initial_role["safe_coord"], player_init_cache_.safe_coord);

		{
			const Scene_Config *scene = CONFIG_CACHE_SCENE->scene_config_cache(player_init_cache_.safe_scene);
			if (!scene) LOG_ABORT("scene config not exist:%d", player_init_cache_.safe_scene);
			int map_value = CONFIG_CACHE_SCENE->get_map_value(scene->map_id, player_init_cache_.safe_coord);
			if (map_value <= 0) {
				LOG_ABORT("scene id:%d, x:%d, y:%d", player_init_cache_.safe_scene, player_init_cache_.safe_coord.x, player_init_cache_.safe_coord.y);
			}
		}

		player_init_cache_.level = json_initial_role["level"].asInt();

		int career = 1;
		if (json_initial_role["skill"].size() != 3) LOG_ABORT("");
		Player_Init_Cache::Default_Skill_Map default_skill_map;
		Int_Int default_skills;
		for (Json::Value::const_iterator it = json_initial_role["skill"].begin();
				it != json_initial_role["skill"].end(); ++it, ++career) {
			if ((*it).size() != 3) LOG_ABORT("");
			default_skill_map.clear();
			int talent = 1;
			for (Json::Value::const_iterator d_it = (*it).begin(); d_it != (*it).end(); ++d_it, ++talent) {
				if ((*d_it).size() != 2) LOG_ABORT("");
				default_skills.reset();
				default_skills.val_1 = (*d_it)[0u].asInt();
				default_skills.val_2 = (*d_it)[1].asInt();
				default_skill_map[talent] = default_skills;
			}
			player_init_cache_.default_skill[career] = default_skill_map;
		}
		// 首个副本id
		player_init_cache_.first_single_id = json_initial_role["first_single_id"].asInt();
		player_init_cache_.first_nor_elite_id = json_initial_role["first_nor_elite_id"].asInt();
		player_init_cache_.first_hor_elite_id = json_initial_role["first_hor_elite_id"].asInt();
	}

	{	// 登录:一二级转换数据
		const Json::Value &fighter_conversion = CONFIG_INSTANCE->config_json()["fighter"]["conversion"];
		if (fighter_conversion == Json::Value::null) LOG_ABORT("");
		for (Json::Value::const_iterator it = fighter_conversion.begin(); it != fighter_conversion.end(); ++it) {
			int career = (*it)["career"].asInt();
			ABORT_INT_ZERO(career);

			Prop_Value_Vec prop_value_vec;
			for (size_t i = 0; i < ARRAY_SIZE(CONVERSION_PROPS); ++i) {
				Int_Double prop_value;
				prop_value.id = CONVERSION_PROPS[i];

				std::ostringstream prop_stream;
				prop_stream << prop_value.id;

				if ((*it)[prop_stream.str()] != Json::Value::null) {
					prop_value.value = (*it)[prop_stream.str()].asDouble();
					prop_value_vec.push_back(prop_value);
				}
			}
			player_init_cache_.prop_conversion.insert(std::make_pair(career, prop_value_vec));
		}
	}

	{	// 登录:初始属性
		const Json::Value &initial_attribute = CONFIG_INSTANCE->config_json()["fighter"]["initial_attribute"];
		if (initial_attribute == Json::Value::null) LOG_ABORT("");
		for (Json::Value::const_iterator it = initial_attribute.begin(); it != initial_attribute.end(); ++it) {
			for (size_t i = 0; i < ARRAY_SIZE(PLAYER_INIT_PROPS); ++i) {
				Int_Double prop_value;
				prop_value.id = PLAYER_INIT_PROPS[i];

				std::ostringstream prop_stream;
				prop_stream << prop_value.id;

				if ((*it)[prop_stream.str()] != Json::Value::null) {
					prop_value.value = (*it)[prop_stream.str()].asDouble();
					player_init_cache_.initial_attribute.push_back(prop_value);
				}
			}
		}
	}
}

void Config_Cache_Role::refresh_player_name_depot(void) {
	player_name_depot_.reset();

	const Json::Value name_depot_json = CONFIG_INSTANCE->config_json()["player"]["name_depot"];
	if (name_depot_json == Json::Value::null) LOG_ABORT("");
	std::string male_name = "";
	std::string female_name = "";
	for (Json::Value::const_iterator it = name_depot_json.begin(); it != name_depot_json.end(); ++it) {
		if (!(*it)["family_name"].isNull() && (*it)["family_name"].isString()) {
			player_name_depot_.family_name_depot.push_back((*it)["family_name"].asString());
		}
		if (!(*it)["male_name"].isNull() && (*it)["male_name"].isString()) {
			player_name_depot_.m_name_depot.push_back((*it)["male_name"].asString());
		}
		if (!(*it)["female_name"].isNull() && (*it)["female_name"].isString()) {
			player_name_depot_.f_name_depot.push_back((*it)["female_name"].asString());
		}
	}
}

void Config_Cache_Role::refresh_level_fighter_prop(void) {
	const Json::Value property_base_json = CONFIG_INSTANCE->config_json()["fighter"]["property_base"];
	if (property_base_json == Json::Value::null) LOG_ABORT("");

	for (Json::Value::const_iterator it = property_base_json.begin(); it != property_base_json.end(); ++it) {
		int level = (*it)["level"].asInt();
		ABORT_INT_ZERO(level);

		Prop_Value_Vec prop_value_vec;

		for (size_t i = 0; i < ARRAY_SIZE(PLAYER_PROPS); ++i) {
			Int_Double prop_value;
			prop_value.id = PLAYER_PROPS[i];

			std::ostringstream prop_stream;
			prop_stream << prop_value.id;

			if ((*it)[prop_stream.str()] != Json::Value::null) {
				prop_value.value = (*it)[prop_stream.str()].asDouble();
				prop_value_vec.push_back(prop_value);
			}
		}

		set_map_second_value_by_key(level, level_prop_map_, prop_value_vec);


		std::ostringstream s_p_stream;
		s_p_stream << PT_SKILL_POINT;
		if ((*it)[s_p_stream.str()] != Json::Value::null) {
			int skill_points_ = (*it)[s_p_stream.str()].asInt();
			set_map_second_value_by_key(level, level_up_add_skill_points_, skill_points_);
		}

		int64_t exp_need = (*it)["exp_need"].asInt();
		if (exp_need <= 0.0)
			LOG_ABORT("exp error level:%d", level);
		set_map_second_value_by_key(level, level_up_need_exp_map_, exp_need);

		double ref_force = (*it)["Standard_BR"].asDouble();
		set_map_second_value_by_key(level, ref_force_map_, ref_force);
	}

}

void Config_Cache_Role::refresh_error_tip_cache(void) {
	const Json::Value &error_json = CONFIG_INSTANCE->config_json()["error"]["error"]["content"];

	for (Json::Value::const_iterator it = error_json.begin(); it != error_json.end(); it++) {
		int error_id = atoi((*it)[0u].asString().c_str());
		std::string content = (*it)[1u].asString();
		set_map_second_value_by_key(error_id, error_tip_map_, content);
	}
}

void Config_Cache_Role::refresh_fight_prop_cache(void) {
	fight_prop_set_.clear();
	const Json::Value &property_tips = CONFIG_INSTANCE->config_json()["player"]["property_tips"];
	for (Json::Value::const_iterator it = property_tips.begin(); it != property_tips.end(); it++) {
		int prop_id = (*it)["proerty_id"].asInt();
		if (!prop_id || !(*it)["fight_prop"].asInt()) {
			continue;
		}
		fight_prop_set_.insert(prop_id);
	}

	refresh_m_race_fight_props();
}

void Config_Cache_Role::refresh_m_race_fight_props(void) {
	m_race_fight_props_.clear();
	const Json::Value &property_tips = CONFIG_INSTANCE->config_json()["player"]["property_tips"];
	for (Json::Value::const_iterator it = property_tips.begin(); it != property_tips.end(); it++) {
		int prop_id = (*it)["proerty_id"].asInt();
		if (!prop_id || !(*it)["m_race_fight_prop"].asInt()) {
			continue;
		}
		m_race_fight_props_.insert(prop_id);
	}
}

void Config_Cache_Role::refresh_career_tips(void) {
	career_map.clear();
	const Json::Value &career_tips = CONFIG_INSTANCE->config_json()["player"]["career_tips"];
	for (Json::Value::const_iterator it = career_tips.begin(); it != career_tips.end(); it++) {
		uint8_t id = (*it)["career_id"].asUInt();
		std::string name = (*it)["career_name"].asString();
		career_map.insert(std::make_pair(id,name));
	}
}

int Config_Cache_Role::get_elite_first_id(const int diff){
	if(diff == 1){
		return player_init_cache_.first_nor_elite_id;
	}else{
		return player_init_cache_.first_hor_elite_id;
	}
}

int Config_Cache_Role::get_single_first_id(void){
	return player_init_cache_.first_single_id;
}

const Player_Init_Cache &Config_Cache_Role::player_init_cache(void) const {
	return player_init_cache_;
}


const Prop_Value_Vec *Config_Cache_Role::level_fighter_prop_cache(int level) const {
	return get_map_second_pointer_by_key(level, level_prop_map_);
}

const int64_t Config_Cache_Role::level_up_need_exp_map(int level) const {
	return get_value_by_key(level, level_up_need_exp_map_);
}

const int Config_Cache_Role::level_up_add_skill_points(int level) const {
	return get_value_by_key(level, level_up_add_skill_points_);
}

const std::string Config_Cache_Role::error_tip_map(int error_id) const {
	Int_String_Map::const_iterator it = error_tip_map_.find(error_id);
	if (it != error_tip_map_.end()) {
		return it->second;
	}
	return "";
}

const Int_Set &Config_Cache_Role::fight_prop_set(void) const {
	return fight_prop_set_;
}

const Int_Set &Config_Cache_Role::m_race_fight_props(void) const {
	return m_race_fight_props_;
}

const std::vector<std::string>& Config_Cache_Role::get_family_name_vec(void) const {
	return player_name_depot_.family_name_depot;
}

const std::vector<std::string>& Config_Cache_Role::get_male_name_vec(void) const {
	return player_name_depot_.m_name_depot;
}

const std::vector<std::string>& Config_Cache_Role::get_female_name_vec(void) const {
	return player_name_depot_.f_name_depot;
}

const double Config_Cache_Role::get_ref_force(int lv) const {
	return get_value_by_key(lv, ref_force_map_);
}

std::string Config_Cache_Role::get_career_name(const uint8_t id) const {
	std::string name("");
	Career_Map::const_iterator iter = career_map.find(id);
	if ( iter != career_map.end())
		name = iter->second;
	return name;
}

