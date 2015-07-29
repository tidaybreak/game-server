/*
 * Task_Config.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: Linqiyou
 */

#include "Config_Cache_Task.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "task/Task_Struct.h"
#include "Config_Cache_Reward_Task.h"
#include "Fighter_Detail.h"
#include "task/Task_Def.h"
#include "task/Task_Struct.h"


const Task_Config &Config_Cache_Task::get_task_config(void) const {
	return task_config_;
}

const Task_Mission_Detail_Config *Config_Cache_Task::find_task(int task_id) const {
	return get_map_second_pointer_by_key(task_id, task_config_.missions);
}

/*const Task_Config::IntSet *Config_Cache_Task::npc_task_list(int npc_id) const {
	return get_map_second_pointer_by_key(npc_id, task_config_.npc_task_map);
}*/


const Day_Week_Task_Config& Config_Cache_Task::get_day_task_config() const {
	return day_week_task_config_;
}

const Day_Group_Set* Config_Cache_Task::find_day_group_set(uint8_t time_hour) const {
	Day_Group_Map::const_iterator it = day_week_task_config_.day_group_map.find(time_hour);
	if(it != day_week_task_config_.day_group_map.end()) {
		return &(it->second);
	}
	return 0;
}

const Week_Group_Set* Config_Cache_Task::find_week_group_set(uint8_t wday, uint8_t time_hour) const {
	uint16_t key = create_key_of_week_group_map(wday, time_hour);
	Week_Group_Map::const_iterator it = day_week_task_config_.week_group_map.find(key);
	if(it != day_week_task_config_.week_group_map.end()) {
		return &(it->second);
	}
	return 0;
}

const Day_Week_Task_Award_Cfg* Config_Cache_Task::find_day_award_cfg(uint32_t role_level) {
	return get_map_second_pointer_by_key(role_level, day_week_task_config_.day_task_award_cfg_map);
}

void Config_Cache_Task::add_day_task_award_cfg_to_map(uint32_t role_level, const Day_Week_Task_Award_Cfg &cfg) {
	set_map_second_value_by_key(role_level, day_week_task_config_.day_task_award_cfg_map, cfg);
}

const Day_Week_Task_Award_Cfg* Config_Cache_Task::find_week_award_cfg(uint32_t role_level) {
	return get_map_second_pointer_by_key(role_level, day_week_task_config_.week_task_award_cfg_map);
}

void Config_Cache_Task::add_week_task_award_cfg_to_map(uint32_t role_level, const Day_Week_Task_Award_Cfg &cfg) {
	set_map_second_value_by_key(role_level, day_week_task_config_.week_task_award_cfg_map, cfg);
}

Day_Group_Set* Config_Cache_Task::find_day_group_set_no_const(uint8_t time_hour) {
	Day_Group_Map::iterator it = day_week_task_config_.day_group_map.find(time_hour);
	if(it != day_week_task_config_.day_group_map.end()) {
		return &(it->second);
	}
	return 0;
}

Week_Group_Set* Config_Cache_Task::find_week_group_set_no_const(uint8_t wday, uint8_t time_hour) {
	uint16_t key = create_key_of_week_group_map(wday, time_hour);
	Week_Group_Map::iterator it = day_week_task_config_.week_group_map.find(key);
	if(it != day_week_task_config_.week_group_map.end()) {
		return &(it->second);
	}
	return 0;
}

const Safe_Guide_Config_Map& Config_Cache_Task::get_safe_guide_config_map() {
	return safe_guide_config_map_;
}

void Config_Cache_Task::add_safe_guide_config_to_map(uint64_t safe_id, const Safe_Guide_Config &cfg) {
	safe_guide_config_map_.insert(std::make_pair(safe_id, cfg));
}

const Ring_Task_All_Cfg& Config_Cache_Task::get_ring_task_all_cfg() const  {
	return ring_task_all_cfg_;
}

void Config_Cache_Task::refresh_day_task_config() {
	const Json::Value& day_task_group_json_list = CONFIG_INSTANCE->config_json()["task"]["day"];
	if (day_task_group_json_list == Json::Value::null || day_task_group_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_day_task_config");
	}

	day_week_task_config_.day_group_map.clear();

	for (uint i = 0; i < day_task_group_json_list.size(); ++i) {
		const Json::Value& day_task_group_json = day_task_group_json_list[i];
		uint8_t time = (uint8_t)day_task_group_json["time"].asUInt();
		uint32_t group = (uint32_t)day_task_group_json["group"].asUInt();
		Day_Group_Set *day_group_set_ptr = find_day_group_set_no_const(time);
		if(!day_group_set_ptr) {
			Day_Group_Set day_group_set;
			day_group_set.insert(group);
			day_week_task_config_.day_group_map.insert(std::make_pair(time, day_group_set));
		}
		else {
			day_group_set_ptr->insert(group);
		}
	}
}

void Config_Cache_Task::refresh_week_task_config() {
	const Json::Value& week_task_group_json_list = CONFIG_INSTANCE->config_json()["task"]["week"];
	if (week_task_group_json_list == Json::Value::null || week_task_group_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_week_task_config");
	}

	day_week_task_config_.week_group_map.clear();

	for (uint i = 0; i < week_task_group_json_list.size(); ++i) {
		const Json::Value& week_task_group_json = week_task_group_json_list[i];
		uint8_t wday = (uint8_t)week_task_group_json["wday"].asUInt();
		uint8_t time = (uint8_t)week_task_group_json["time"].asUInt();
		uint32_t group = (uint32_t)week_task_group_json["group"].asUInt();
		uint16_t key = create_key_of_week_group_map(wday, time);
		Week_Group_Set *week_group_set_ptr = find_week_group_set_no_const(wday, time);
		if(!week_group_set_ptr) {
			Week_Group_Set week_group_set;
			week_group_set.insert(group);
			day_week_task_config_.week_group_map.insert(std::make_pair(key, week_group_set));
		}
		else {
			week_group_set_ptr->insert(group);
		}
	}
}

void Config_Cache_Task::refresh_day_task_award_config() {
	const Json::Value& day_task_award_json_list = CONFIG_INSTANCE->config_json()["task"]["day_award"];
	if (day_task_award_json_list == Json::Value::null || day_task_award_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_day_week_task_award_config");
	}
	day_week_task_config_.day_task_award_cfg_map.clear();
	for (uint i = 0; i < day_task_award_json_list.size(); ++i) {
		const Json::Value& day_task_award_json = day_task_award_json_list[i];
		Day_Week_Task_Award_Cfg cfg;
		uint32_t role_level = (uint32_t)day_task_award_json["role_level"].asUInt();
		cfg.coin = (uint32_t)day_task_award_json["coin"].asUInt();
		cfg.exp = (uint32_t)day_task_award_json["exp"].asUInt();
		cfg.dragon_soul = (uint32_t)day_task_award_json["dragon_soul"].asUInt();
		cfg.soul = (uint32_t)day_task_award_json["soul"].asUInt();

		cfg.item1_id = (uint32_t)day_task_award_json["item1_id"].asUInt();
		cfg.item1_num = (uint32_t)day_task_award_json["item1_num"].asUInt();
		cfg.is_item1_bind = (day_task_award_json["item1_bind"].asInt() == 1) ? true : false;

		cfg.item2_id = (uint32_t)day_task_award_json["item2_id"].asUInt();
		cfg.item2_num = (uint32_t)day_task_award_json["item2_num"].asUInt();
		cfg.is_item2_bind = (day_task_award_json["item2_bind"].asInt() == 1) ? true : false;

		cfg.item3_id = (uint32_t)day_task_award_json["item3_id"].asUInt();
		cfg.item3_num = (uint32_t)day_task_award_json["item3_num"].asUInt();
		cfg.is_item3_bind = (day_task_award_json["item3_bind"].asInt() == 1) ? true : false;

		cfg.item4_id = (uint32_t)day_task_award_json["item4_id"].asUInt();
		cfg.item4_num = (uint32_t)day_task_award_json["item4_num"].asUInt();
		cfg.is_item4_bind = (day_task_award_json["item4_bind"].asInt() == 1) ? true : false;

		add_day_task_award_cfg_to_map(role_level, cfg);
	}
}

void Config_Cache_Task::refresh_week_task_award_config() {
	const Json::Value& week_task_award_json_list = CONFIG_INSTANCE->config_json()["task"]["week_award"];
	if (week_task_award_json_list == Json::Value::null || week_task_award_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_week_task_award_config");
	}
	day_week_task_config_.week_task_award_cfg_map.clear();
	for (uint i = 0; i < week_task_award_json_list.size(); ++i) {
		const Json::Value& week_task_award_json = week_task_award_json_list[i];
		Day_Week_Task_Award_Cfg cfg;
		uint32_t role_level = (uint32_t)week_task_award_json["role_level"].asUInt();
		cfg.coin = (uint32_t)week_task_award_json["coin"].asUInt();
		cfg.exp = (uint32_t)week_task_award_json["exp"].asUInt();
		cfg.dragon_soul = (uint32_t)week_task_award_json["dragon_soul"].asUInt();
		cfg.soul = (uint32_t)week_task_award_json["soul"].asUInt();

		cfg.item1_id = (uint32_t)week_task_award_json["item1_id"].asUInt();
		cfg.item1_num = (uint32_t)week_task_award_json["item1_num"].asUInt();
		cfg.is_item1_bind = (week_task_award_json["item1_bind"].asInt() == 1) ? true : false;

		cfg.item2_id = (uint32_t)week_task_award_json["item2_id"].asUInt();
		cfg.item2_num = (uint32_t)week_task_award_json["item2_num"].asUInt();
		cfg.is_item2_bind = (week_task_award_json["item2_bind"].asInt() == 1) ? true : false;

		cfg.item3_id = (uint32_t)week_task_award_json["item3_id"].asUInt();
		cfg.item3_num = (uint32_t)week_task_award_json["item3_num"].asUInt();
		cfg.is_item3_bind = (week_task_award_json["item3_bind"].asInt() == 1) ? true : false;

		cfg.item4_id = (uint32_t)week_task_award_json["item4_id"].asUInt();
		cfg.item4_num = (uint32_t)week_task_award_json["item4_num"].asUInt();
		cfg.is_item4_bind = (week_task_award_json["item4_bind"].asInt() == 1) ? true : false;

		add_week_task_award_cfg_to_map(role_level, cfg);
	}
}

void Config_Cache_Task::refresh_safe_guide_config() {
	const Json::Value& safe_guide_json_list = CONFIG_INSTANCE->config_json()["safe_guide"]["safe_guide"];
	if (safe_guide_json_list == Json::Value::null || safe_guide_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_safe_guide_config");
	}

	safe_guide_config_map_.clear();

	for (uint i = 0; i < safe_guide_json_list.size(); ++i) {
		const Json::Value& safe_guide_json = safe_guide_json_list[i];
		Safe_Guide_Config cfg;
		cfg.safe_type = (uint16_t)safe_guide_json["safe_type"].asUInt();
		cfg.safe_id = (uint64_t)safe_guide_json["safe_id"].asUInt();
		cfg.safe_level = (uint32_t)safe_guide_json["safe_level"].asUInt();
		cfg.param1 = (uint32_t)safe_guide_json["param1"].asUInt();
		cfg.safe_task = (uint32_t)safe_guide_json["task_id"].asUInt();
		add_safe_guide_config_to_map(cfg.safe_id, cfg);
	}
}

void Config_Cache_Task::refresh_ring_task_base_cfg() {
	const Json::Value& ring_task_base_json_list = CONFIG_INSTANCE->config_json()["task"]["ring_task"]["base"];
	if (ring_task_base_json_list == Json::Value::null || ring_task_base_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_ring_task_base_cfg");
	}

	ring_task_all_cfg_.ring_task_base_cfg.reset();

	for (uint i = 0; i < ring_task_base_json_list.size(); ++i) {
		const Json::Value& ring_task_base_json = ring_task_base_json_list[i];
		ring_task_all_cfg_.ring_task_base_cfg.friend_ship = (uint32_t)ring_task_base_json["friend_ship"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.diamond = (uint32_t)ring_task_base_json["diamond"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.max_ring_num = (uint32_t)ring_task_base_json["max_ring_num"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.open_level = (uint32_t)ring_task_base_json["open_level"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.npc_talk_chance = (uint8_t)ring_task_base_json["npc_talk_chance"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.collect_chance = (uint8_t)ring_task_base_json["collect_chance"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.limit_time_kill_chance = (uint8_t)ring_task_base_json["limit_time_kill_chance"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.kill_monster_chance = (uint8_t)ring_task_base_json["kill_monster_chance"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.commit_consume_item_chance = (uint8_t)ring_task_base_json["commit_consume_item_chance"].asUInt();
		ring_task_all_cfg_.ring_task_base_cfg.dragon_vale_op_chance = (uint8_t)ring_task_base_json["dragon_vale_op_chance"].asUInt();
		break;
	}
}

void Config_Cache_Task::refresh_ring_task_ring_award_cfg() {
	const Json::Value& ring_award_json_list = CONFIG_INSTANCE->config_json()["task"]["ring_task"]["ring_award"];
	if (ring_award_json_list == Json::Value::null || ring_award_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_ring_task_ring_award_cfg");
	}

	ring_task_all_cfg_.ring_task_award_cfg_map.clear();

	for (uint i = 0; i < ring_award_json_list.size(); ++i) {
		const Json::Value& ring_award_json = ring_award_json_list[i];
		Ring_Task_Award_Cfg ring_task_award_cfg;
		ring_task_award_cfg.ring_num = (uint32_t)ring_award_json["ring_num"].asUInt();
		ring_task_award_cfg.exp = (uint32_t)ring_award_json["exp"].asUInt();
		ring_task_award_cfg.coin = (uint32_t)ring_award_json["coin"].asUInt();
		ring_task_award_cfg.soul = (uint32_t)ring_award_json["soul"].asUInt();

		ring_task_award_cfg.item1_id = (uint32_t)ring_award_json["item1_id"].asUInt();
		ring_task_award_cfg.item1_num = (uint32_t)ring_award_json["item1_num"].asUInt();
		ring_task_award_cfg.is_item1_bind = (ring_award_json["item1_bind"].asUInt() == 1) ? true : false;

		ring_task_award_cfg.item2_id = (uint32_t)ring_award_json["item2_id"].asUInt();
		ring_task_award_cfg.item2_num = (uint32_t)ring_award_json["item2_num"].asUInt();
		ring_task_award_cfg.is_item2_bind = (ring_award_json["item2_bind"].asUInt() == 1) ? true : false;

		ring_task_award_cfg.item3_id = (uint32_t)ring_award_json["item3_id"].asUInt();
		ring_task_award_cfg.item3_num = (uint32_t)ring_award_json["item3_num"].asUInt();
		ring_task_award_cfg.is_item3_bind = (ring_award_json["item3_bind"].asUInt() == 1) ? true : false;

		ring_task_award_cfg.item4_id = (uint32_t)ring_award_json["item4_id"].asUInt();
		ring_task_award_cfg.item4_num = (uint32_t)ring_award_json["item4_num"].asUInt();
		ring_task_award_cfg.is_item4_bind = (ring_award_json["item4_bind"].asUInt() == 1) ? true : false;

		ring_task_all_cfg_.ring_task_award_cfg_map.insert(std::make_pair(ring_task_award_cfg.ring_num, ring_task_award_cfg));
	}
}

void Config_Cache_Task::refresh_ring_task_player_level_award_cfg() {
	const Json::Value& player_level_award_json_list = CONFIG_INSTANCE->config_json()["task"]["ring_task"]["player_level_award"];
	if (player_level_award_json_list == Json::Value::null || player_level_award_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_ring_task_player_level_award_cfg");
	}

	ring_task_all_cfg_.player_level_award_cfg_map.clear();

	for (uint i = 0; i < player_level_award_json_list.size(); ++i) {
		const Json::Value& player_level_award_json = player_level_award_json_list[i];
		Player_Level_Award_Cfg player_level_award_cfg;
		player_level_award_cfg.player_level = (uint32_t)player_level_award_json["player_level"].asUInt();
		player_level_award_cfg.exp_percent = (uint32_t)player_level_award_json["exp_percent"].asUInt();
		player_level_award_cfg.coin_percent = (uint32_t)player_level_award_json["coin_percent"].asUInt();
		player_level_award_cfg.soul_percent = (uint32_t)player_level_award_json["soul_percent"].asUInt();
		player_level_award_cfg.item1_percent = (uint32_t)player_level_award_json["item1_percent"].asUInt();
		player_level_award_cfg.item2_percent = (uint32_t)player_level_award_json["item2_percent"].asUInt();
		player_level_award_cfg.item3_percent = (uint32_t)player_level_award_json["item3_percent"].asUInt();
		player_level_award_cfg.item4_percent = (uint32_t)player_level_award_json["item4_percent"].asUInt();

		ring_task_all_cfg_.player_level_award_cfg_map.insert(std::make_pair(player_level_award_cfg.player_level, player_level_award_cfg));
	}
}

void Config_Cache_Task::refresh_ring_task_ring_num_type_cfg() {
	const Json::Value& ring_num_type_json_list = CONFIG_INSTANCE->config_json()["task"]["ring_task"]["ring_type"];
	if (ring_num_type_json_list == Json::Value::null || ring_num_type_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_ring_task_ring_num_type_cfg");
	}

	ring_task_all_cfg_.ring_num_type_cfg_map.clear();

	for (uint i = 0; i < ring_num_type_json_list.size(); ++i) {
		const Json::Value& ring_num_type_json = ring_num_type_json_list[i];
		uint32_t ring_num = (uint32_t)ring_num_type_json["ring_num"].asUInt();
		Op_Ring_Type op_ring_type;
		op_ring_type.op = (uint8_t)ring_num_type_json["op_type"].asUInt();
		op_ring_type.ring_type = (uint8_t)ring_num_type_json["ring_type"].asUInt();
		if(op_ring_type.op != 1 && op_ring_type.op != 2) {
			LOG_USER("环任务操作类型非法，找策划, 环数=%d, 操作类型=%d", ring_num, op_ring_type.op);
			LOG_ABORT("环任务配置错误，找策划");
		}

		if(op_ring_type.ring_type != RING_TASK_TYPE_NPC_TALK
				&& op_ring_type.ring_type != RING_TASK_TYPE_COLLECT
				&& op_ring_type.ring_type != RING_TASK_TYPE_LIMIT_TIME_KILL_BOSS
				&& op_ring_type.ring_type != RING_TASK_TYPE_KILL_MONSTER
				&& op_ring_type.ring_type != RING_TASK_TYPE_COMMIT_CONSUME_ITEM
				&& op_ring_type.ring_type != RING_TASK_TYPE_DRAGON_VALE_OP) {
			LOG_USER("环任务类型非法，找策划, 环数=%d, 环类型=%d", ring_num, op_ring_type.ring_type);
			LOG_ABORT("环任务配置错误，找策划");
		}

		ring_task_all_cfg_.ring_num_type_cfg_map.insert(std::make_pair(ring_num, op_ring_type));
	}
}

void Config_Cache_Task::refresh_ring_monster() {
	const Json::Value& ring_monster_json_list = CONFIG_INSTANCE->config_json()["task"]["ring_task"]["ring_monster"];
	if (ring_monster_json_list == Json::Value::null || ring_monster_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_ring_monster");
	}

	ring_task_all_cfg_.ring_monster_cfg_map.clear();

	for (uint i = 0; i < ring_monster_json_list.size(); ++i) {
		const Json::Value& ring_monster_json = ring_monster_json_list[i];
		uint32_t ring_num = (uint32_t)ring_monster_json["ring_num"].asUInt();
		Ring_Monster_Cfg ring_monster_cfg;
		ring_monster_cfg.physical_attack = (uint32_t)ring_monster_json["physical_attack"].asUInt();
		ring_monster_cfg.magic_attack = (uint32_t)ring_monster_json["magic_attack"].asUInt();
		ring_monster_cfg.physical_defence = (uint32_t)ring_monster_json["physical_defence"].asUInt();
		ring_monster_cfg.magic_defence = (uint32_t)ring_monster_json["magic_defence"].asUInt();
		ring_monster_cfg.hit_rate = (uint32_t)ring_monster_json["hit_rate"].asUInt();
		ring_monster_cfg.avoid = (uint32_t)ring_monster_json["avoid"].asUInt();
		ring_monster_cfg.blood_max = (uint32_t)ring_monster_json["blood_max"].asUInt();
		ring_task_all_cfg_.ring_monster_cfg_map.insert(std::make_pair(ring_num, ring_monster_cfg));
	}
}

void Config_Cache_Task::refresh_level_monster() {
	const Json::Value& level_monster_json_list = CONFIG_INSTANCE->config_json()["task"]["ring_task"]["level_monster"];
	if (level_monster_json_list == Json::Value::null || level_monster_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Task: refresh_level_monster");
	}

	ring_task_all_cfg_.level_monster_cfg_map.clear();

	for (uint i = 0; i < level_monster_json_list.size(); ++i) {
		const Json::Value& ring_monster_json = level_monster_json_list[i];
		uint32_t level = (uint32_t)ring_monster_json["ring_num"].asUInt();
		Level_Monster_Cfg level_monster_cfg;
		level_monster_cfg.physical_attack = (uint32_t)ring_monster_json["physical_attack"].asUInt();
		level_monster_cfg.magic_attack = (uint32_t)ring_monster_json["magic_attack"].asUInt();
		level_monster_cfg.physical_defence = (uint32_t)ring_monster_json["physical_defence"].asUInt();
		level_monster_cfg.magic_defence = (uint32_t)ring_monster_json["magic_defence"].asUInt();
		level_monster_cfg.hit_rate = (uint32_t)ring_monster_json["hit_rate"].asUInt();
		level_monster_cfg.avoid = (uint32_t)ring_monster_json["avoid"].asUInt();
		level_monster_cfg.blood_max = (uint32_t)ring_monster_json["blood_max"].asUInt();
		ring_task_all_cfg_.level_monster_cfg_map.insert(std::make_pair(level, level_monster_cfg));
	}
}

void Config_Cache_Task::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Task");

	refresh_day_task_config();
	refresh_week_task_config();
	refresh_day_task_award_config();
	refresh_week_task_award_config();
	refresh_ring_task_base_cfg();
	refresh_ring_task_ring_award_cfg();
	refresh_ring_task_player_level_award_cfg();
	refresh_ring_task_ring_num_type_cfg();
//	refresh_ring_monster();
//	refresh_level_monster();

	//热更前，先清空
	RewardTokenSet &reward_token_set = CONFIG_CACHE_REWARD_TASK->get_config_no_const().reward_token_set;
	reward_token_set.clear();

	for(Task_Mission_Config::iterator it = task_config_.missions.begin(); it != task_config_.missions.end(); ++it) {
		it->second.reset();
	}

	task_config_.missions.clear();

/*
	// task_setting ======================================
	{
		const Json::Value& task_setting_json = CONFIG_INSTANCE->config_json()["task"]["task_setting"];
		// task_type
		task_config_.task_setting.task_type.main = task_setting_json["task_type"]["main"].asInt();
		task_config_.task_setting.task_type.branch = task_setting_json["task_type"]["branch"].asInt();
		task_config_.task_setting.task_type.routine = task_setting_json["task_type"]["routine"].asInt();
		task_config_.task_setting.task_type.league = task_setting_json["task_type"]["league"].asInt();

		// cond_type
		task_config_.task_setting.cond_type.kill = task_setting_json["cond_type"]["kill"].asInt();
		task_config_.task_setting.cond_type.fb = task_setting_json["cond_type"]["fb"].asInt();
		task_config_.task_setting.cond_type.talk = task_setting_json["cond_type"]["talk"].asInt();
		task_config_.task_setting.cond_type.arrive = task_setting_json["cond_type"]["arrive"].asInt();
		task_config_.task_setting.cond_type.lvl_up = task_setting_json["cond_type"]["lvl_up"].asInt();
		task_config_.task_setting.cond_type.package = task_setting_json["cond_type"]["package"].asInt();
		task_config_.task_setting.cond_type.kill_group = task_setting_json["cond_type"]["kill_group"].asInt();
		task_config_.task_setting.cond_type.collect = task_setting_json["cond_type"]["collect"].asInt();
		task_config_.task_setting.cond_type.recharge = task_setting_json["cond_type"]["recharge"].asInt();
		task_config_.task_setting.cond_type.attend = task_setting_json["cond_type"]["attend"].asInt();

		Json_Utility::assign_int_vec(task_setting_json["cond_enum"], task_config_.task_setting.cond_enum);
		Json_Utility::assign_str_vec(task_setting_json["daily_reset"], task_config_.task_setting.daily_reset);
	}
	// task_setting ======================================
*/

	// mission ========================================
	{
//		task_config_.task_ids.clear();
		Int_Vec id_vec;
		id_vec.clear();
		const Json::Value& mission_json = CONFIG_INSTANCE->config_json()["task"]["mission"];
		for (Json::Value::iterator it = mission_json.begin();
					it != mission_json.end(); ++it) {
			int key = (*it)["taskid"].asInt();
			if (key < TaskInfo::TASK_START || key > TaskInfo::TASK_END) {
				id_vec.push_back(key);
			} else {
				Task_Mission_Detail_Config task_miss;
				task_miss.award.reset();

				task_miss.task_id = key;
				task_miss.task_type = key/100000;
				if (task_miss.task_type > TaskInfo::TASK_GTYPE_END) {
					LOG_ABORT("error task %d", key);
				}
				task_miss.isNew = (*it)["isNew"].asInt();
				task_miss.progress = (*it)["progress"].asInt();
				task_miss.quality = (*it)["quality"].asInt();
				task_miss.giveuptask = (*it)["giveuptask"].asInt();
				task_miss.precondition = (*it)["precondition"].asInt();
				task_miss.postcondition = (*it)["postcondition"].asInt();
				task_miss.postcondition2 = (*it)["postcondition2"].asInt();
				task_miss.postcondition3 = (*it)["postcondition3"].asInt();
				task_miss.recruid_hero = (*it)["recruid_hero"].asBool();
				task_miss.chapter = (*it)["chapter"].asString();
				task_miss.is_limit_time = (*it)["is_limit_time"].asInt() ? true : false;
				task_miss.limit_time = (*it)["limit_time"].asInt();
				task_miss.is_fast_complete = (*it)["is_fast_complete"].asInt() ? true : false;
				task_miss.money_type = (*it)["moneyType"].asInt();
				task_miss.amount = (*it)["amount"].asInt();
				task_miss.item_id = (*it)["itemId"].asInt();
				task_miss.item_num = (*it)["itemNum"].asInt();
				task_miss.hero_op = (*it)["hero_op"].asUInt();
				task_miss.hero_id = (*it)["hero_id"].asUInt();
				task_miss.icon_hero_id = (*it)["icon_hero_id"].asUInt();
				task_miss.show_or_close_icon = (*it)["is_show_icon"].asUInt();
				task_miss.gift_level = (*it)["gift_level"].asUInt();
				task_miss.day_group = (*it)["day_group"].asUInt();
				task_miss.week_group = (*it)["week_group"].asUInt();
				task_miss.horse_id = (*it)["horse_id"].asUInt();
				task_miss.is_magic_wing = (*it)["is_magic_wing"].asUInt() ? true : false;
				bool is_magic_wing_try = (*it)["is_magic_wing_try"].asUInt() ? true : false;
				if(is_magic_wing_try) {
					task_config_.magic_wing_try_id = (uint32_t)task_miss.task_id;
				}

				task_miss.vip_accept_level = (*it)["vip_accept_level"].asUInt();
				task_miss.exp_times = (*it)["exp_times"].asUInt();
				task_miss.coin_times = (*it)["coin_times"].asUInt();
				task_miss.dragon_soul_times = (*it)["dragon_soul_times"].asUInt();
				task_miss.soul_times = (*it)["soul_times"].asUInt();
				task_miss.item1_times = (*it)["item1_times"].asUInt();
				task_miss.item2_times = (*it)["item2_times"].asUInt();
				task_miss.item3_times = (*it)["item3_times"].asUInt();
				task_miss.item4_times = (*it)["item4_times"].asUInt();
				task_miss.ring_type = (*it)["ring_type"].asUInt();
				task_miss.min_ring_num = (*it)["min_ring_num"].asUInt();
				task_miss.max_ring_num = (*it)["max_ring_num"].asUInt();
				task_miss.scene_id = (*it)["scene_id"].asUInt();
				task_miss.npc_op = (uint8_t)(*it)["npc_op"].asUInt();
				task_miss.npc_scene_id = (uint32_t)(*it)["npc_scene_id"].asUInt();
				task_miss.npc_id = (uint32_t)(*it)["npc_id"].asUInt();
				task_miss.talent_id = (uint32_t)(*it)["talent_id"].asUInt();
				task_miss.job = (uint8_t)(*it)["job"].asUInt();
				task_miss.task_version = (uint32_t)(*it)["task_version"].asUInt();
				task_miss.is_show_in_task_list = ((*it)["is_show_in_task_list"].asUInt() == 0) ? true : false;

				if(task_miss.task_type == TaskInfo::TASK_GTYPE_RING_TASK) {
					if(task_miss.ring_type != RING_TASK_TYPE_NPC_TALK
							&& task_miss.ring_type != RING_TASK_TYPE_COLLECT
							&& task_miss.ring_type != RING_TASK_TYPE_LIMIT_TIME_KILL_BOSS
							&& task_miss.ring_type != RING_TASK_TYPE_KILL_MONSTER
							&& task_miss.ring_type != RING_TASK_TYPE_COMMIT_CONSUME_ITEM
							&& task_miss.ring_type != RING_TASK_TYPE_DRAGON_VALE_OP) {
						LOG_USER("环任务类型非法，找策划, 任务id=%d, 环类型=%d", task_miss.task_id, task_miss.ring_type);
						LOG_ABORT("环任务配置错误，找策划");
					}
				}

				// award cfg
				/*1，道具
				2，装备
				3，经验
				4，金币
				5，英灵
				6，声望
				7，龙魂
				8，钻石
				9，绑定钻石
				10，特殊物品
				11，hero*/
				if(!(*it)["award"].empty())
				{
					Task_Mission_Award_Detail_Cfg item;
					const Json::Value &award_json = (*it)["award"];
					for (Json::Value::iterator it_award = award_json.begin();
							it_award != award_json.end(); ++it_award) {
						item.reset();
						item.type = (*it_award)["type"].asInt();
						item.val1 = (*it_award)["val1"].asInt();
						item.val2 = (*it_award)["val2"].asInt();
						item.val3 = (*it_award)["val3"].asInt();
						item.val4 = (*it_award)["val4"].asInt();
						item.val5 = (*it_award)["val5"].asInt();
						item.job = (*it_award)["job"].asInt();
						switch(item.type) {
						case 1:
						case 2:
						case 14:
							if (item.val1) {
								//2为绑定
								if (item.val3 == 2) {
									item.val3 = 2;
								}
								else {
									item.val3 = 1;
								}
								task_miss.award.item_list.push_back(item);
							}
							break;
						case 3:
						case 4:
						case 5:
						case 6:
						case 7:
						case 8:
						case 9:
						case 10:
						case 11:
						case 12:
						case 13:
						case 15:
						case 16:
							task_miss.award.other_list.push_back(item);
							break;
						default:
							break;
						}
					}
				}
				// award cfg

				// accept cfg
				{
					const Json::Value &accept_json = (*it)["accept"];
					task_miss.accept.is_empty = true;
					for (Json::Value::iterator it_snd = accept_json.begin();
							it_snd != accept_json.end(); ++it_snd) {
						task_miss.accept.is_empty = false;
						task_miss.accept.talk = (*it_snd)["talk"].asInt();
//						if (task_miss.accept.talk > 0) {
//							task_config_.npc_task_map[task_miss.accept.talk].insert(key);
//						}
						task_miss.accept.camp = (*it_snd)["camp"].asInt();
						task_miss.accept.career = (*it_snd)["career"].asInt();
						task_miss.accept.vip_level = (*it_snd)["vipLevel"].asInt();
						task_miss.accept.type = (*it_snd)["type"].asInt();
						task_miss.accept.level = (*it_snd)["level"].asInt();
						task_miss.accept.level_max = (*it_snd)["levelMax"].asInt();
						task_miss.accept.team = (*it_snd)["team"].asInt();
						task_miss.accept.guild = (*it_snd)["guild"].asInt();
						task_miss.accept.accept_times = (*it_snd)["times"].asInt();
						task_miss.accept.accept_times_all = (*it_snd)["sumTimes"].asInt();
						int receive_type = (*it_snd)["receiveType"].asInt();
						if(receive_type == 0) {
							receive_type = 1;
						}
						task_miss.accept.auto_accept = receive_type;
						task_miss.accept.receive.type = (*it_snd)["gainType"].asInt();
						task_miss.accept.receive.val1 = (*it_snd)["val1"].asInt();
						task_miss.accept.receive.val2 = (*it_snd)["val2"].asInt();
						task_miss.accept.receive.val3 = 2;
						task_miss.accept.paytype.type = (*it_snd)["paytype"].asInt();
						task_miss.accept.paytype.val1 = (*it_snd)["val3"].asInt();
						task_miss.accept.paytype.val2 = (*it_snd)["val4"].asInt();

						task_miss.accept.exp = (*it_snd)["exp"].asInt();
						task_miss.accept.pet = (*it_snd)["pet"].asInt();
						task_miss.accept.buf_op = (uint8_t)(*it_snd)["buf_op"].asInt();
						task_miss.accept.buff = (*it_snd)["buffId"].asInt();
						task_miss.accept.buf_ms = (*it_snd)["buf_ms"].asInt();
						task_miss.accept.fly = (*it_snd)["fly"].asInt();
						task_miss.accept.polt_id = (*it_snd)["poltId"].asInt();
						task_miss.accept.action.polt_id = (*it_snd)["actionGroup"][0u].asInt();
						task_miss.accept.action.action_id = (*it_snd)["actionGroup"][1u].asInt();
						task_miss.accept.skill_id = (*it_snd)["skillId"].asInt();
						task_miss.accept.full_prop_num = (*it_snd)["full_prop_num"].asInt();
                        task_miss.accept.val1 = (*it_snd)["val1"].asUInt();
                        task_miss.accept.val2 = (*it_snd)["val2"].asUInt();
                        task_miss.accept.val3 = (*it_snd)["val3"].asUInt();
                        task_miss.accept.val4 = (*it_snd)["val4"].asUInt();

                        if(task_miss.accept.val1 && task_miss.accept.val2) {
                        	RewardTokenSet &reward_token_set = CONFIG_CACHE_REWARD_TASK->get_config_no_const().reward_token_set;
                        	if(reward_token_set.find(task_miss.accept.val1) == reward_token_set.end()) {
                        		reward_token_set.insert(task_miss.accept.val1);
                        	}
                        }
					}
				}
				// accept cfg

				// execute cfg
				if((*it)["execute"].isArray())
				{
					const Json::Value &exec_json = (*it)["execute"];
					Task_Mission_Exec_Detail_Cfg exec;
					int size = exec_json.size();
					for (int i = 0; i < size; ++i) {
						exec.reset();
						exec.type = exec_json[i]["type"].asInt();
						if (exec_json[i]["id"] != Json::Value::null) {
							if (exec_json[i]["id"].isArray()) {
								Json_Utility::assign_set_list(exec_json[i]["id"], exec.id_list);
							} else {
								exec.id = exec_json[i]["id"].asInt();
							}
						}
						exec.npc_id = exec_json[i]["param1"].asInt();
//						if (exec.npc_id > 0) {
//							task_config_.npc_task_map[exec.npc_id].insert(key);
//						}
						exec.sceneid = exec_json[i]["sceneid"].asInt();
						exec.target_x = exec_json[i]["target_x"].asInt();
						exec.target_y = exec_json[i]["target_y"].asInt();
						exec.amount = exec_json[i]["amount"].asInt();
						exec.action.polt_id = exec_json[i]["actionGroup"][0u].asInt();
						exec.action.action_id = exec_json[i]["actionGroup"][1u].asInt();

						if (exec_json[i]["mosterpoint"] != Json::Value::null && exec_json[i]["mosterpoint"].isArray()) {
							const Json::Value& monster_point_array_json = exec_json[i]["mosterpoint"];
							for(uint j = 0; j < monster_point_array_json.size(); ++j){
								const Json::Value& re_json = monster_point_array_json[j];
								if(monster_point_array_json[j] != Json::Value::null && monster_point_array_json[j].isArray()) {
									Scene_Monster_Point scene_monster_point;
									scene_monster_point.scene_id = re_json[0u].asUInt();
									scene_monster_point.monster_point = re_json[1u].asUInt();
									exec.scene_monster_point_set.insert(scene_monster_point);
								}
							}
						}

						exec.type = exec_json[i]["type"].asInt();
						exec.param1 = exec_json[i]["param1"].asUInt();
						exec.param2 = exec_json[i]["param2"].asUInt();
						exec.talk = exec_json[i]["talk"].asUInt();
						task_miss.execute.push_back(exec);
					}
				}
				// execute cfg

				// fialed cfg
				if(!(*it)["failed"].empty())
				{
					const Json::Value &failed_json = (*it)["failed"];
					Task_Mission_Exec_Detail_Cfg exec;
					int size = failed_json.size();
					for (int i = 0; i < size; ++i) {
						task_miss.failed.duration = failed_json[i]["duration"].asInt();
						task_miss.failed.item_id = failed_json[i]["item_id"].asInt();
						task_miss.failed.level_max = failed_json[i]["level_max"].asInt();
						task_miss.failed.state = failed_json[i]["state"].asInt();
						task_miss.failed.hero = failed_json[i]["hero"].asInt();
					}
				}
				// fialed cfg

				// complete cfg
				task_miss.complete.is_empty = true;
				if(!(*it)["complete"].empty())
				{
					const Json::Value &complete_json = (*it)["complete"];
					Task_Mission_Exec_Detail_Cfg exec;
					int size = complete_json.size();
					for (int i = 0; i < size; ++i) {
						task_miss.complete.is_empty = false;
						task_miss.complete.npc_id = complete_json[i]["talk"].asInt();
						task_miss.complete.paytype.type = complete_json[i]["paytype"].asInt();
						task_miss.complete.paytype.val1 = complete_json[i]["val1"].asInt();
						task_miss.complete.paytype.val2 = complete_json[i]["val2"].asInt();
						task_miss.complete.fashion1 = complete_json[i]["fashion1"].asInt();
						task_miss.complete.fashion2 = complete_json[i]["fashion2"].asInt();
					}
				}
				// complete cfg

				set_map_second_value_by_key(key, task_config_.missions, task_miss);
//				task_config_.task_ids.insert(key);
			}
		}
	}
	// mission ========================================
}

void Config_Cache_Task::check_ring_task_cfg() {
	//key:level  value:ring_num
	typedef boost::unordered_multimap<uint32_t, uint32_t> Error_Level_Ring_Num_Map;
	Error_Level_Ring_Num_Map error_level_ring_num_map;

	typedef Int_Int Ring_Num_Ring_Type;
	typedef boost::unordered_map<uint32_t, Ring_Num_Ring_Type> Error_Level_Ring_Num_Ring_Type_Map;
	Error_Level_Ring_Num_Ring_Type_Map error_ring_type_map;

	for(uint32_t level = 1; level <= MAX_LEVEL; ++level) {
		if(level < ring_task_all_cfg_.ring_task_base_cfg.open_level) {
			continue;
		}

		for(uint32_t ring_num = 1; ring_num <= ring_task_all_cfg_.ring_task_base_cfg.max_ring_num; ++ring_num) {
			bool is_find_task = false;
			bool is_npc_talk_exist = false;
			bool is_collect_exist = false;
			bool is_limit_time_kill_boss_eixst = false;
			bool is_kill_monster_exist = false;
			bool is_commit_consume_item_exist = false;
			bool is_dragon_vale_op_exist = false;

			for(Task_Mission_Config::const_iterator it = task_config_.missions.begin(); it != task_config_.missions.end(); ++it) {
				if(it->second.task_type  != TaskInfo::TASK_GTYPE_RING_TASK) {
					continue;
				}

				if(it->second.task_type == TaskInfo::TASK_GTYPE_RING_TASK) {
					if(it->second.ring_type != RING_TASK_TYPE_NPC_TALK
							&& it->second.ring_type != RING_TASK_TYPE_COLLECT
							&& it->second.ring_type != RING_TASK_TYPE_LIMIT_TIME_KILL_BOSS
							&& it->second.ring_type != RING_TASK_TYPE_KILL_MONSTER
							&& it->second.ring_type != RING_TASK_TYPE_COMMIT_CONSUME_ITEM
							&& it->second.ring_type != RING_TASK_TYPE_DRAGON_VALE_OP) {
						LOG_ABORT("环任务配置错误:环类型不合法, 任务id=%d, 环类型=%d", it->second.task_id, it->second.ring_type);
					}
				}

				if(it->second.accept.level != 0 && it->second.accept.level > (int)level) {
					continue;
				}

				if(it->second.accept.level_max != 0 && it->second.accept.level_max < (int)level) {
					continue;
				}

				if(it->second.min_ring_num != 0 && it->second.min_ring_num > ring_num) {
					continue;
				}

				if(it->second.max_ring_num != 0 && it->second.max_ring_num < ring_num) {
					continue;
				}

				if(it->second.task_type == TaskInfo::TASK_GTYPE_RING_TASK && it->second.ring_type == RING_TASK_TYPE_NPC_TALK) {
					is_npc_talk_exist = true;
				}

				if(it->second.task_type == TaskInfo::TASK_GTYPE_RING_TASK && it->second.ring_type == RING_TASK_TYPE_COLLECT) {
					is_collect_exist = true;
				}

				if(it->second.task_type == TaskInfo::TASK_GTYPE_RING_TASK && it->second.ring_type == RING_TASK_TYPE_LIMIT_TIME_KILL_BOSS) {
					is_limit_time_kill_boss_eixst = true;
				}

				if(it->second.task_type == TaskInfo::TASK_GTYPE_RING_TASK && it->second.ring_type == RING_TASK_TYPE_KILL_MONSTER) {
					is_kill_monster_exist = true;
				}

				if(it->second.task_type == TaskInfo::TASK_GTYPE_RING_TASK && it->second.ring_type == RING_TASK_TYPE_COMMIT_CONSUME_ITEM) {
					is_commit_consume_item_exist =  true;
				}

				if(it->second.task_type == TaskInfo::TASK_GTYPE_RING_TASK && it->second.ring_type == RING_TASK_TYPE_DRAGON_VALE_OP) {
					is_dragon_vale_op_exist = true;
				}

				is_find_task = true;
			}//end for

			if(!is_find_task) {
				error_level_ring_num_map.insert(std::make_pair(level, ring_num));
			}

			if(!is_npc_talk_exist) {
				Ring_Num_Ring_Type ring_num_ring_type;
				ring_num_ring_type.val_1 = (int32_t)ring_num;
				ring_num_ring_type.val_2 = (int32_t)RING_TASK_TYPE_NPC_TALK;
				error_ring_type_map.insert(std::make_pair(level, ring_num_ring_type));
			}

			if(!is_collect_exist) {
				Ring_Num_Ring_Type ring_num_ring_type;
				ring_num_ring_type.val_1 = (int32_t)ring_num;
				ring_num_ring_type.val_2 = (int32_t)RING_TASK_TYPE_COLLECT;
				error_ring_type_map.insert(std::make_pair(level, ring_num_ring_type));
			}

			if(!is_limit_time_kill_boss_eixst) {
				Ring_Num_Ring_Type ring_num_ring_type;
				ring_num_ring_type.val_1 = (int32_t)ring_num;
				ring_num_ring_type.val_2 = (int32_t)RING_TASK_TYPE_LIMIT_TIME_KILL_BOSS;
				error_ring_type_map.insert(std::make_pair(level, ring_num_ring_type));
			}

			if(!is_kill_monster_exist) {
				Ring_Num_Ring_Type ring_num_ring_type;
				ring_num_ring_type.val_1 = (int32_t)ring_num;
				ring_num_ring_type.val_2 = (int32_t)RING_TASK_TYPE_KILL_MONSTER;
				error_ring_type_map.insert(std::make_pair(level, ring_num_ring_type));
			}

			if(!is_commit_consume_item_exist) {
				Ring_Num_Ring_Type ring_num_ring_type;
				ring_num_ring_type.val_1 = (int32_t)ring_num;
				ring_num_ring_type.val_2 = (int32_t)RING_TASK_TYPE_COMMIT_CONSUME_ITEM;
				error_ring_type_map.insert(std::make_pair(level, ring_num_ring_type));
			}

			if(!is_dragon_vale_op_exist) {
				Ring_Num_Ring_Type ring_num_ring_type;
				ring_num_ring_type.val_1 = (int32_t)ring_num;
				ring_num_ring_type.val_2 = (int32_t)RING_TASK_TYPE_DRAGON_VALE_OP;
				error_ring_type_map.insert(std::make_pair(level, ring_num_ring_type));
			}

		}//end for
	}//end for

	if(!error_level_ring_num_map.empty()) {
		for(Error_Level_Ring_Num_Map::const_iterator it = error_level_ring_num_map.begin(); it != error_level_ring_num_map.end(); ++it) {
			LOG_USER("环任务配置错误:没有环任务, 等级=%ld, 环数=%ld", it->first, it->second);
		}

		LOG_ABORT("环任务配置错误,找策划");
	}

	if(!error_ring_type_map.empty()) {
		for(Error_Level_Ring_Num_Ring_Type_Map::const_iterator it = error_ring_type_map.begin(); it != error_ring_type_map.end(); ++it) {
			LOG_USER("环任务配置错误:没有环任务, 等级=%ld, 环数=%d, 环类型=%d", it->first, it->second.val_1, it->second.val_2);
		}
		LOG_ABORT("环任务配置错误,找策划");
	}
}

void Config_Cache_Task::check_reward_task_cfg() {
	typedef Int_Int Quality_NUM;
	typedef boost::unordered_multimap<int, Quality_NUM> Level_Quality_Num_Map;
	Level_Quality_Num_Map level_quality_num_map;
	level_quality_num_map.clear();

	const Reward_Task_Config &reward_task_cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	for(int level = 1; level <= MAX_LEVEL; ++level) {
		if(level < reward_task_cfg.open_level) {
			continue;
		}

		for(int quality = TASK_QUALITY_WHITE; quality < TASK_QUALITY_END; ++quality) {
			uint32_t num = 0;

			for(Task_Mission_Config::const_iterator it = task_config_.missions.begin(); it != task_config_.missions.end(); ++it) {
				if(it->second.task_type  != TaskInfo::TASK_GTYPE_REWARD_TASK) {
					continue;
				}

				if(it->second.quality != quality) {
					continue;
				}

				if(it->second.accept.level && level < it->second.accept.level) {
					continue;
				}

				if(it->second.accept.level_max && level > it->second.accept.level_max) {
					continue;
				}

				++num;
			}//end for

			if(num < 4) {
				Quality_NUM quality_num;
				quality_num.val_1 = quality;
				quality_num.val_2 = num;
				level_quality_num_map.insert(std::make_pair(level, quality_num));
			}
		}//end for
	}//end for

	if(!level_quality_num_map.empty()) {
		for(Level_Quality_Num_Map::const_iterator it = level_quality_num_map.begin(); it != level_quality_num_map.end(); ++it) {
			LOG_USER("悬赏任务配置错误： 可刷新悬赏任务数量不足4个, 等级=%d, 品质=%d, 数量=%d", it->first, it->second.val_1, it->second.val_2);
		}
		LOG_ABORT("悬赏任务配置错误，找策划");
	}

	level_quality_num_map.clear();
}

void Config_Cache_Task::check_task_cfg() {
	check_ring_task_cfg();
	check_reward_task_cfg();
}
