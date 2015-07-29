/*
 * Team_Config.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: Linqiyou
 */

#include "Config_Cache_Team.h"
#include "Configurator.h"
#include "Json_Utility.h"


const Team_Config &Config_Cache_Team::team_config(void) const {
	return team_config_;
}

const int* Config_Cache_Team::find_scene(int8_t type) const {
	return get_map_second_pointer_by_key(Team_Scene_Config::Scene_Type(type), team_config_.scene.data);
}

Arena_Multi_Detail *Config_Cache_Team::arena_multi_cfg_win(const int32_t &level) {
	Level_ArenaMultiDetail_Map::iterator it = this->arena_multi_cfg_win_.find(level);
	if (it != this->arena_multi_cfg_win_.end()) {
		return &it->second;
	}
	return NULL;
}
Arena_Multi_Detail *Config_Cache_Team::arena_multi_cfg_fail(const int32_t &level) {
	Level_ArenaMultiDetail_Map::iterator it = this->arena_multi_cfg_fail_.find(level);
	if (it != this->arena_multi_cfg_fail_.end()) {
		return &it->second;
	}
	return NULL;
}
const Arena_Multi_Checkout_Detail *Config_Cache_Team::arena_multi_cfg_checkout(const int32_t &score) {
	for (Index_ArenaMultiCheckout_Map::iterator it = arena_multi_cfg_checkout_.begin(); it != arena_multi_cfg_checkout_.end(); ++it) {
		if (score >= it->second.rank.low && score <= it->second.rank.high) {
			return &it->second;
		}
	}
	return NULL;
}

const Arena_Multi_Checkout_Detail *Config_Cache_Team::arena_multi_cfg_level(const int32_t &level) {
	return get_map_second_pointer_by_key(level, arena_multi_cfg_checkout_);
}

int Config_Cache_Team::arena_multi_level(const int32_t &score) {
	for (Index_ArenaMultiCheckout_Map::iterator it = arena_multi_cfg_checkout_.begin(); it != arena_multi_cfg_checkout_.end(); ++it) {
		if (score >= it->second.rank.low && score <= it->second.rank.high) {
			return it->second.index;
		}
	}
	return 0;
}
const Team_Robot_Config_Map &Config_Cache_Team::team_robot_cfg(void) {
	return team_robot_cfg_;
}
const Team_Defend_Cfg_Detail *Config_Cache_Team::find_team_defend(const int scene_id) const {
	return get_map_second_pointer_by_key(scene_id, team_defend_cfg_);
}

int Config_Cache_Team::get_correct_by_gap_and_victory(const int gap, const int victory) const {
	for (Team_Arena_Correct_Vec::const_iterator it = team_arena_correct_cfg_.begin();
			it != team_arena_correct_cfg_.end(); ++it) {
		if (gap >= it->gap.low && gap <= it->gap.high) {
			if (victory > 0) {
				return it->correct.low;
			} else {
				return it->correct.high;
			}
		}
	}
	return 100;
}

void Config_Cache_Team::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Team");

	// task_setting ======================================
	{
		const Json::Value& team_setting_json = CONFIG_INSTANCE->config_json()["team"]["team_setting"];
		if (!team_setting_json.isNull() && team_setting_json.isArray() && team_setting_json.size() > 0) {
			// task_type
			if (team_setting_json[0u]["max_player"] != Json::Value::null)
				team_config_.max_player = team_setting_json[0u]["max_player"].asInt();
			team_config_.profile_num_fb = team_setting_json[0u]["profile_num"].asInt();
			team_config_.profile_num_sports = team_setting_json[0u]["profile_num_sports"].asInt();
			team_config_.profile_num_hero_dream = team_setting_json[0u]["profile_num_hero_dream"].asInt();
			team_config_.profile_num_defend = team_setting_json[0u]["profile_num_defend"].asInt();
	//		team_config_.words = team_setting_json["words"].asString();
			team_config_.offline_limit = Time_Value::zero;
			team_config_.offline_limit.sec(team_setting_json[0u]["offline_limit"].asInt());
			team_config_.robot_task = team_setting_json[0u]["robot_task"].asInt();
			if (team_config_.robot_task == 0) {
				MSG_USER("robot_task empty!");
			}
			team_config_.friendship_timelimit = team_setting_json[0u]["friendship"].asInt();
			if (!team_setting_json[0u]["world_invite_cost"].isNull() && team_setting_json[0u]["world_invite_cost"].isArray()) {
				Json_Utility::assign_item_vec(team_setting_json[0u]["world_invite_cost"], team_config_.world_invite_cost);
			}
		}

		const Json::Value& team_json = CONFIG_INSTANCE->config_json()["team"]["team"];
		for (Json::Value::iterator it = team_json.begin();
					it != team_json.end(); ++it) {
			Team_Scene_Config::Scene_Type key = (Team_Scene_Config::Scene_Type)((*it)["type"].asInt());//程序从0开始
			int value = (*it)["scene_id"].asInt();
			set_map_second_value_by_key(key, team_config_.scene.data, value);
		}
	}
	// task_setting ======================================

	{
		arena_multi_cfg_win_.clear();
		arena_multi_cfg_fail_.clear();
		const Json::Value& json = CONFIG_INSTANCE->config_json()["arena_multi"]["arena"];
		Arena_Multi_Detail detail;
		for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
			detail.reset();
			detail.level_ = (*it)["tier"].asInt();
			detail.victory_ = (*it)["victory"].asInt();
			detail.friendship_ = (*it)["friendly"].asInt();
			detail.integrate_ = (*it)["credit"].asInt();
			detail.exp_ = (*it)["exp"].asInt();
			detail.gold_ = (*it)["gold"].asInt();
			const Json::Value &item_json = (*it)["item"];
			if (!item_json.isNull() && item_json.isArray()) {
				Json_Utility::assign_item_vec(item_json, detail.item_vec_);
			}
			detail.merit_ = (*it)["merit"].asInt();
			if (detail.victory_) {
				set_map_second_value_by_key(detail.level_, arena_multi_cfg_win_, detail);
			} else {
				set_map_second_value_by_key(detail.level_, arena_multi_cfg_fail_, detail);
			}
		}
	}

	{
		arena_multi_cfg_checkout_.clear();
		const Json::Value& json = CONFIG_INSTANCE->config_json()["arena_multi"]["tier"];
		Arena_Multi_Checkout_Detail detail;
		for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
			detail.reset();
			detail.index = (*it)["tier"].asInt();
			const Json::Value &item_json = (*it)["item"];
			Json_Utility::assign_item_vec(item_json, detail.item_vec);
			if ((*it)["tier_credit"] != Json::Value::null && (*it)["tier_credit"].isArray()
					&& (*it)["tier_credit"].size() > 0) {
				if ((*it)["tier_credit"].size() == 2) {
					detail.rank.low = (*it)["tier_credit"][0u].asInt();
					detail.rank.high = (*it)["tier_credit"][1u].asInt() == -1 ? INT_MAX : (*it)["tier_credit"][1u].asInt();
				} else {
					LOG_ABORT("error Arena_Multi:%d, integral_num", detail.index);
				}
				set_map_second_value_by_key(detail.index, arena_multi_cfg_checkout_, detail);
			}
		}
	}

	{
		team_arena_correct_cfg_.clear();
		const Json::Value& json = CONFIG_INSTANCE->config_json()["arena_multi"]["correct"];
		Team_Arena_Correct detail;
		for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
			detail.reset();
			if ((*it)["gap"] != Json::Value::null && (*it)["gap"].isArray()
					&& (*it)["gap"].size() > 0) {
				if ((*it)["gap"].size() == 2) {
					detail.gap.low = (*it)["gap"][0u].asInt() == -999999 ? INT_MIN : (*it)["gap"][0u].asInt();
					detail.gap.high = (*it)["gap"][1u].asInt() == 999999 ? INT_MAX : (*it)["gap"][1u].asInt();
				} else {
					LOG_ABORT("error Arena_Multi Correct: gap not 2!");
				}
				if (((*it)["correct"] != Json::Value::null && (*it)["correct"].isArray()
					&& (*it)["correct"].size() == 2)) {
					detail.correct.low = (*it)["correct"][0u].asInt();
					detail.correct.high = (*it)["correct"][1u].asInt();
				} else {
					LOG_ABORT("error Arena_Multi Correct: correct not 2!");
				}
				team_arena_correct_cfg_.push_back(detail);
			}
		}
	}

	{
		team_robot_cfg_.clear();
		const Json::Value& json = CONFIG_INSTANCE->config_json()["team"]["robot"];
		// task_type
		Team_Robot_Config detail;
		for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
			detail.reset();
			detail.id = (*it)["id"].asInt();
			detail.name = (*it)["name"].asString();
			detail.career = (*it)["career"].asInt();
			detail.lvl = (*it)["lvl"].asInt();

			Json_Utility::assign_set_list((*it)["equip"], detail.equip);
			Json_Utility::assign_set_list((*it)["skill"], detail.skill);

			detail.hero_id = (*it)["hero_id"].asInt();
			detail.hero_name = (*it)["hero_name"].asString();
			detail.hero_fight = (*it)["hero_fight"].asInt();
			detail.hero_lvl = (*it)["hero_lvl"].asInt();
			detail.hero_awake_lvl = (*it)["hero_awake_lvl"].asInt();

			Json_Utility::assign_set_list((*it)["hero_equip"], detail.hero_equip);
			Json_Utility::assign_set_list((*it)["hero_skill"], detail.hero_skill);

			set_map_second_value_by_key(detail.id, team_robot_cfg_, detail);
		}
	}

	{
		team_defend_cfg_.clear();
		const Json::Value& json = CONFIG_INSTANCE->config_json()["team"]["defend"];
		// task_type
		Team_Defend_Cfg_Detail detail;
		for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
			detail.reset();
			detail.scene_id = (*it)["sceneId"].asInt();
			Json_Utility::assign_coord_list((*it)["birth_coord"], detail.birth_coord);
			Json_Utility::assign_set_list((*it)["name_list"], detail.monster_ids);

			detail.boss_id = (*it)["boss_name"].asInt();
			detail.wave = (*it)["wave"].asInt();
			detail.init_time = (*it)["init_time"].asInt();
			detail.award_time = (*it)["award_time"].asInt();
			detail.wave_time = (*it)["wave_time"].asInt();
			detail.first_wave_time = (*it)["first_wave_time"].asInt();

			set_map_second_value_by_key(detail.scene_id, team_defend_cfg_, detail);
		}
	}
}
