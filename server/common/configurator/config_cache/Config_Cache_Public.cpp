/*
 * Config_Cache_Public.cpp
 *
 *  Created on: May 30, 2014
 *      Author: gaojianshan
 */


#include "Config_Cache_Public.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Lib_Log.h"
#include "ranking/Ranking_Manager.h"
#include "fighter/Fighter_Def.h"

const Guide_Config_Map& Config_Cache_Public::guide_config(void) const {
	return guide_config_map;
}

const Sys_Cfg& Config_Cache_Public::sys_config() const {
	return sys_cfg;
}

const Guide_Base_Config* Config_Cache_Public::find_guide_config(uint16_t id) const {
	return get_map_second_pointer_by_key(id, guide_config_map);
}

const Forenotice_Cfg_Lvl* Config_Cache_Public::find_forenotice_lvl(const int forenotice_lvl) const {
	Forenotice_Cfg_Lvl_Map::const_iterator it = forenotice_cfg_lvl_map_.find(forenotice_lvl);
	if (it != forenotice_cfg_lvl_map_.end()) {
		return &it->second;
	}
	return NULL;
}

const Forenotice_Cfg_Lvl_Map& Config_Cache_Public::forenotice_cfg_lvl_map(void) {
	return forenotice_cfg_lvl_map_;
}

const std::map<int, int>& Config_Cache_Public::cfg_exploit_lvl_map(void) {
	return exploit_lvl_map_;
}

const Guide_Finish_Cfg_Map& Config_Cache_Public::get_guide_finish_cfg_map() {
	return guide_finish_cfg_map_;
}

const Notice_Cfg_Map& Config_Cache_Public::get_notice_cfg_map() {
	return notice_cfg_map_;
}
const int Config_Cache_Public::get_world_level_add_exp_inner_module(const int gap) const {
	Int_Int_Map::const_iterator it = world_level_map_.find(gap);
	if (it != world_level_map_.end()) {
		return it->second;
	}
	return 0;
}
const int Config_Cache_Public::get_world_level_add_exp(const int level, const int world_level) const {
//	MSG_USER("level:(%d), world_level_role_level_limit_(%d), world_level(%d), world_level_effect_limit(%d)", level, world_level_role_level_limit_, world_level, world_level_effect_limit_);
	if (level <= world_level_role_level_limit_ || world_level < world_level_effect_limit_) {
		return 0;
	}
	return get_world_level_add_exp_inner_module(world_level - level);
}

const Questionnaire_Reward_Config* Config_Cache_Public::get_questionnaire_reward_config_byqid(const uint8_t qid) const {
	return get_map_second_pointer_by_key(qid, questionnaire_conf);
}

const Questionnaire_Config_Map& Config_Cache_Public::get_questionnaire_config(void) const {
	return questionnaire_conf;
}

const Invite_Reward_Config &Config_Cache_Public::get_invite_one_reward_config(void) const {
	return invite_ones_reward;
}

const Invite_Reward_Config &Config_Cache_Public::get_invite_daily_reward_config(void) const {
	return invite_daily_reward;
}

const Nobility_Config *Config_Cache_Public::find_title_nobility_cfg(const int32_t exploit_lvl) const {
	Nobility_Config_Map::const_iterator it = nobility_config_map_.find(exploit_lvl);
	if (it != nobility_config_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

int Config_Cache_Public::world_level_index(void) {
	return world_level_index_;
}

int Config_Cache_Public::get_public_default_value(const int key){
	Int_Int_Map::iterator it =  public_default_cfg_map_.find(key);
	if(it != public_default_cfg_map_.end()){
		return it->second;
	}
	return 0;
}

const bool Config_Cache_Public::has_old_player(const std::string &account) const {
	return old_player_account.find(account) != old_player_account.end();
}

const Challenge_Cfg& Config_Cache_Public::get_challenge_cfg() {
	return challenge_cfg_;
}

const Challenge_Area_Cfg* Config_Cache_Public::find_challenge_area_cfg(uint32_t scene_id) {
	Challenge_Area_Cfg_Map::const_iterator find_it = challenge_cfg_.challenge_area_cfg_map.find(scene_id);
	if(find_it == challenge_cfg_.challenge_area_cfg_map.end()) {
		return 0;
	}
	return &(find_it->second);
}

void Config_Cache_Public::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Public:refresh_config_cache");
	refresh_guide_config_cache();
	refresh_forenotice_cfg();
	refresh_exploit_cfg();
	refresh_world_level();
	refresh_old_player_account();
	refresh_invite_conf_cache();
	refresh_config_nobility();
}

void Config_Cache_Public::refresh_guide_config_cache() {
	const Json::Value& guide_json_list = CONFIG_INSTANCE->config_json()["guide"]["guide"];
	if (guide_json_list == Json::Value::null || guide_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Public:guide");
	}

	Guide_Base_Config base_set;
	for (uint i = 0; i < guide_json_list.size(); ++i) {
		base_set.reset();
		const Json::Value& guide_json = guide_json_list[i];
		base_set.guide_id = (uint16_t)guide_json["guideId"].asInt();
		base_set.max_num = guide_json["max_num"].asInt();
		set_map_second_value_by_key(base_set.guide_id, guide_config_map, base_set);
	}
}

void Config_Cache_Public::refresh_sys_config_cache() {
	const Json::Value& sys_cfg_json_list = CONFIG_INSTANCE->config_json()["syscfg"]["syscfg"];
	if (sys_cfg_json_list == Json::Value::null || sys_cfg_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Public:syscfg");
	}

	for (uint i = 0; i < sys_cfg_json_list.size(); ++i) {
		const Json::Value& sys_cfg_json = sys_cfg_json_list[i];
		sys_cfg.is_total_open = (uint8_t)sys_cfg_json["is_total_open"].asInt();
		sys_cfg.is_open_music = (uint8_t)sys_cfg_json["is_open_music"].asInt();
		sys_cfg.music_num = (uint8_t)sys_cfg_json["music_num"].asInt();
		sys_cfg.is_open_voice = (uint8_t)sys_cfg_json["is_open_voice"].asInt();
		sys_cfg.voice_num = (uint8_t)sys_cfg_json["voice_num"].asInt();
		sys_cfg.is_hide_role_model = (uint8_t)sys_cfg_json["is_hide_role_model"].asInt();
		sys_cfg.is_hide_hero_model = (uint8_t)sys_cfg_json["is_hide_hero_model"].asInt();
		sys_cfg.is_hide_title = (uint8_t)sys_cfg_json["is_hide_title"].asInt();
		sys_cfg.min_player_count = (uint32_t)sys_cfg_json["min_player_count"].asInt();
		sys_cfg.max_player_count = (uint32_t)sys_cfg_json["max_player_count"].asInt();
		sys_cfg.default_player_count = (uint32_t)sys_cfg_json["default_player_count"].asInt();

		break;
	}
}

void Config_Cache_Public::refresh_notice_cfg_cache() {
	const Json::Value& notice_json_list = CONFIG_INSTANCE->config_json()["forenotice"]["notice"];
	if (notice_json_list == Json::Value::null || notice_json_list.size() == 0) {
		return;
		LOG_ABORT("Config_Cache_Public:refresh_notice_cfg_cache");
	}

	notice_cfg_map_.clear();
	for (uint i = 0; i < notice_json_list.size(); ++i) {
		const Json::Value& notice_json = notice_json_list[i];
		Notice_Cfg notice_cfg;
		notice_cfg.id = (uint32_t)notice_json["id"].asUInt();
		notice_cfg.type = (uint8_t)notice_json["type"].asInt();
		notice_cfg.task_id = (uint32_t)notice_json["taskId"].asUInt();
		notice_cfg.task_status = (uint8_t)notice_json["taskStatus"].asUInt();
		notice_cfg.role_level = (uint32_t)notice_json["level"].asUInt();
		notice_cfg_map_.insert(std::make_pair(notice_cfg.id, notice_cfg));
	}
}

void Config_Cache_Public::refresh_challenge_cfg_cache() {
	//base
	{
		const Json::Value& challenge_cfg_json_list = CONFIG_INSTANCE->config_json()["challenge"]["base"];
		if (challenge_cfg_json_list == Json::Value::null || challenge_cfg_json_list.size() == 0) {
			LOG_ABORT("Config_Cache_Public:refresh_challenge_cfg_cache");
		}

		for (uint i = 0; i < challenge_cfg_json_list.size(); ++i) {
			const Json::Value& challenge_cfg_json = challenge_cfg_json_list[i];
			challenge_cfg_.ganger_devote = (uint32_t)challenge_cfg_json["ganger_devote"].asUInt();
			break;
		}
	}

	//scene
	{
		challenge_cfg_.challenge_area_cfg_map.clear();
		const Json::Value& challenge_cfg_json_list = CONFIG_INSTANCE->config_json()["challenge"]["scene"];
		if (challenge_cfg_json_list == Json::Value::null || challenge_cfg_json_list.size() == 0) {
			LOG_ABORT("Config_Cache_Public:refresh_challenge_cfg_cache");
		}

		for (uint i = 0; i < challenge_cfg_json_list.size(); ++i) {
			const Json::Value& challenge_cfg_json = challenge_cfg_json_list[i];
			Challenge_Area_Cfg challenge_area_cfg;
			uint32_t scene_id = (uint32_t)challenge_cfg_json["scene_id"].asUInt();
			challenge_area_cfg.left_border = (uint32_t)challenge_cfg_json["left_border"].asUInt();
			challenge_area_cfg.right_border = (uint32_t)challenge_cfg_json["right_border"].asUInt();
			challenge_area_cfg.up_border = (uint32_t)challenge_cfg_json["up_border"].asUInt();
			challenge_area_cfg.down_border = (uint32_t)challenge_cfg_json["down_border"].asUInt();
			challenge_cfg_.challenge_area_cfg_map.insert(std::make_pair(scene_id, challenge_area_cfg));
		}
	}
}

void Config_Cache_Public::refresh_invite_conf_cache(void) {
	{
		invite_ones_reward.reset();
		const Json::Value& json_list = CONFIG_INSTANCE->config_json()["public"]["invite_ones_reward"];
		if (json_list != Json::Value::null && json_list.size() > 0) {
			for(uint j = 0; j < json_list.size(); ++j){
				const Json::Value& re_json =  json_list[j];
				uint32_t item = re_json["item_id"].asInt();
				uint32_t value = re_json["num"].asInt();
				uint32_t bind = re_json["bind"].asInt();
				if(item == PT_EXP_CURRENT) {
					invite_ones_reward.exp += value;
				} else {
					Invite_Item_Config conf_item;
					conf_item.item_id = item;
					conf_item.num = value;
					conf_item.bind = bind;
					if(item > 999999) {
						invite_ones_reward.reward_item.push_back(conf_item);
					} else {
						invite_ones_reward.reward_money.push_back(conf_item);
					}
				}
			}
		}
	}
	{
		invite_daily_reward.reset();
		const Json::Value& json_list = CONFIG_INSTANCE->config_json()["public"]["invite_daily_reward"];
		if (json_list != Json::Value::null && json_list.size() > 0) {
			for(uint j = 0; j < json_list.size(); ++j){
				const Json::Value& re_json =  json_list[j];
				uint32_t item = re_json["item_id"].asInt();
				uint32_t value = re_json["num"].asInt();
				uint32_t bind = re_json["bind"].asInt();
				if(item == PT_EXP_CURRENT) {
					invite_daily_reward.exp += value;
				} else {
					Invite_Item_Config conf_item;
					conf_item.item_id = item;
					conf_item.num = value;
					conf_item.bind = bind;
					if(item > 999999) {
						invite_daily_reward.reward_item.push_back(conf_item);
					} else {
						invite_daily_reward.reward_money.push_back(conf_item);
					}
				}
			}
		}
	}
}

void Config_Cache_Public::refresh_questionnaire_conf_cache(void) {
	questionnaire_conf.clear();
	const Json::Value& json_list = CONFIG_INSTANCE->config_json()["questionnaire"]["questionCfg"];
	if (json_list == Json::Value::null || json_list.size() == 0) {
		return;
	}
	Questionnaire_Reward_Config reward;
	for (uint i = 0; i < json_list.size(); ++i) {
		const Json::Value& json = json_list[i];

		reward.reset();
		uint8_t qid = (uint8_t)json["id"].asInt();

		for(uint j = 0; j < json["rewards"].size(); ++j){
			const Json::Value& re_json =  json["rewards"][j];
			uint32_t item = re_json[0u].asInt();
			uint32_t value = re_json[1u].asInt();
			if(item == PT_EXP_CURRENT) {
				reward.exp += value;
			} else {
				if(item > 999999) {
					reward.reward_item[item] += value;
				} else {
					reward.reward_money[item] += value;
				}
			}
		}
		set_map_second_value_by_key(qid, questionnaire_conf,reward);
	}
}

void Config_Cache_Public::refresh_forenotice_cfg(void) {
	/*
	 * forenotice/lvl_gift.json
	 */
	Forenotice_Cfg_Lvl cfg_row_info;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["forenotice"]["lvl_gift"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.level = (*it_json)["level"].asInt();
		ABORT_INT_ZERO(cfg_row_info.level);
		Json_Utility::assign_item_vec((*it_json)["gifts"], cfg_row_info.gifts);
		cfg_row_info.copper = (*it_json)["copper"].asInt();
		cfg_row_info.soul = (*it_json)["soul"].asInt();
		cfg_row_info.bind_diamond = (*it_json)["bind_diamond"].asInt();
		cfg_row_info.dragon_soul = (*it_json)["dragon_soul"].asInt();

		set_map_second_value_by_key(cfg_row_info.level, forenotice_cfg_lvl_map_, cfg_row_info);
	}
}

void Config_Cache_Public::refresh_exploit_cfg(void) {
	/*
	 * exploit/exploit_lvl.json
	 */
	const Json::Value &json = CONFIG_INSTANCE->config_json()["exploit"]["exploit_lvl"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		int exploit_lvl =  (*it_json)["exploit_lvl"].asInt();
		int exploit_val =  (*it_json)["exploit_val"].asInt();
		ABORT_INT_ZERO(exploit_lvl);
		set_map_second_value_by_key(exploit_lvl, exploit_lvl_map_, exploit_val);
	}
}
void Config_Cache_Public::refresh_world_level(void) {
	public_default_cfg_map_.clear();
	world_level_map_.clear();
	world_level_role_level_limit_ = 0;
	world_level_effect_limit_ = 0;
	world_level_index_ = 0;
	int gap_max = 200;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["world_level"]["world_level"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		int gap =  (*it_json)["gap"].asInt();
		int exp =  (*it_json)["exp"].asInt();
		if (gap > gap_max) {
			gap_max = gap;
		}
		set_map_second_value_by_key(gap, world_level_map_, exp);
	}

	Int_Vec lvl_small;
	for (int i = gap_max - 1; i > 0; --i) {
		if (0 != world_level_map_.count(i)) {
			int exp = get_world_level_add_exp_inner_module(i);
			for (Int_Vec::const_iterator it = lvl_small.begin(); it != lvl_small.end(); ++it) {
				set_map_second_value_by_key(*it, world_level_map_, exp);
			}
			lvl_small.clear();
			continue;
		}
		lvl_small.push_back(i);
	}

	const Json::Value &buff_cfg = CONFIG_INSTANCE->config_json()["public"]["public"];
	for (Json::Value::const_iterator cfg_it = buff_cfg.begin();
					cfg_it != buff_cfg.end(); ++cfg_it) {
		int type = (*cfg_it)["type"].asInt();
		switch(type) {
		case 2:
			world_level_role_level_limit_ = (*cfg_it)["val"].asInt();
			break;
		case 3:
			world_level_effect_limit_ = (*cfg_it)["val"].asInt();
			break;
		case 11: case 12: case 13: case 14: case 15:
			phy_power_practice_conf_map_[type] = (*cfg_it)["val"].asInt();
			break;
		case 16: case 17: case 18:
			wing_public_cfg_map_[type] = (*cfg_it)["val"].asInt();
			break;
		case 20:
			world_level_index_ = (*cfg_it)["val"].asInt();
			break;
		case 25: {
			Int_Int adore_cost_1;
			Json_Utility::assign_int_int((*cfg_it)["val"], adore_cost_1);
			arena_adore_data.adore_cost_1.push_back(adore_cost_1);
			break;
		}
		case 26:{
			Int_Int adore_item_1;
			Json_Utility::assign_int_int((*cfg_it)["val"], adore_item_1);
			arena_adore_data.adore_item_1.push_back(adore_item_1);
			break;
		}
		case 27:{
			Int_Int adore_cost_2;
			Json_Utility::assign_int_int((*cfg_it)["val"], adore_cost_2);
			arena_adore_data.adore_cost_2.push_back(adore_cost_2);
			break;
		}
		case 28: {
			Int_Int adore_item_2;
			Json_Utility::assign_int_int((*cfg_it)["val"], adore_item_2);
			arena_adore_data.adore_item_2.push_back(adore_item_2);
			break;
		}
		case 29:{
			Int_Int adore_cost_3;
			Json_Utility::assign_int_int((*cfg_it)["val"], adore_cost_3);
			arena_adore_data.adore_cost_3.push_back(adore_cost_3);
			break;
		}
		case 30:{
			Int_Int adore_item_3;
			Json_Utility::assign_int_int((*cfg_it)["val"], adore_item_3);
			arena_adore_data.adore_item_3.push_back(adore_item_3);
			break;
		}
		case 31:
			arena_adore_data.adore_times = (*cfg_it)["val"].asInt();
			break;
		default:
			if((*cfg_it)["val"].isInt()){
				public_default_cfg_map_[type] = (*cfg_it)["val"].asInt();
			}
			break;
		}
	}
	LOG_USER("world_level_role_level_limit_(%d), world_level_effect_limit_(%d)", world_level_role_level_limit_, world_level_effect_limit_);
}

void Config_Cache_Public::refresh_guide_finish_cfg() {
	const Json::Value& guide_finish_cfg_json_list = CONFIG_INSTANCE->config_json()["safe_guide"]["guide_finish"];
	if (guide_finish_cfg_json_list == Json::Value::null || guide_finish_cfg_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Public:refresh_guide_finish_cfg");
	}

	Guide_Finish_Cfg cfg;
	for (uint i = 0; i < guide_finish_cfg_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& guide_finish_cfg_json = guide_finish_cfg_json_list[i];
		uint16_t type = (uint16_t)guide_finish_cfg_json["type"].asInt();
		cfg.guide_id = (uint16_t)guide_finish_cfg_json["guide_id"].asInt();

		if(guide_finish_cfg_json["param1"] != Json::Value::null) {
			if(guide_finish_cfg_json["param1"].isArray()) {
				Json_Utility::assign_set_list(guide_finish_cfg_json["param1"], cfg.param1_list);
			}
			else {
				cfg.param1 = (uint64_t)guide_finish_cfg_json["param1"].asUInt();
			}
		}

		cfg.param2 = (uint64_t)guide_finish_cfg_json["param2"].asUInt();

		guide_finish_cfg_map_.insert(std::make_pair(type, cfg));
	}
}

void Config_Cache_Public::refresh_old_player_account(void) {
	old_player_account.clear();
	const Json::Value& json = CONFIG_INSTANCE->config_json()["public"]["old_player_account"];
	if (json == Json::Value::null || json.size() == 0) {
		return;
	}

	for (uint i = 0; i < json.size(); ++i) {
		Json::Value jval = json[i]["account_str"];
		if(jval == Json::Value::null) {
			jval = json[i]["account"];
		}
		if(jval.isString()) {
			std::string account = jval.asString();
			old_player_account.insert(account);
		}
	}
}

void Config_Cache_Public::refresh_config_nobility(void) {
	nobility_config_map_.clear();
	Nobility_Config info;
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["exploit"]["exploit_lvl"];
	for (Json::Value::const_iterator be_it = config_json.begin();be_it != config_json.end(); ++be_it) {
		info.reset();
		info.exploit_lvl = (*be_it)["exploit_lvl"].asInt();
		info.exploit_val = (*be_it)["exploit_val"].asInt();
		info.broadcast_id = (*be_it)["broadcast_id"].asInt();
		info.name = (*be_it)["name"].asString();
		set_map_second_value_by_key(info.exploit_lvl, nobility_config_map_, info);
	}
}

const int Config_Cache_Public::get_phy_power_every_practice(void) const {
	Int_Int_Map::const_iterator find_it = phy_power_practice_conf_map_.find(11);
	if (find_it == phy_power_practice_conf_map_.end()) return 0;
	return find_it->second;
}
const int Config_Cache_Public::get_practice_gap_seconds(void) const {
	Int_Int_Map::const_iterator find_it = phy_power_practice_conf_map_.find(12);
	if (find_it == phy_power_practice_conf_map_.end()) return 0;
	return find_it->second;
}
const int Config_Cache_Public::get_max_practice_times(void) const {
	Int_Int_Map::const_iterator find_it = phy_power_practice_conf_map_.find(13);
	if (find_it == phy_power_practice_conf_map_.end()) return 0;
	return find_it->second;
}
const int Config_Cache_Public::speed_up_practice_every_min_cost(void) const {
	Int_Int_Map::const_iterator find_it = phy_power_practice_conf_map_.find(14);
	if (find_it == phy_power_practice_conf_map_.end()) return 0;
	return find_it->second;
}

const int Config_Cache_Public::get_practice_scene_id(void) const {
	Int_Int_Map::const_iterator find_it = phy_power_practice_conf_map_.find(15);
	if (find_it == phy_power_practice_conf_map_.end()) return 0;
	return find_it->second;
}

// 翅膀体验及返回配置
const int Config_Cache_Public::get_wing_experience_lv(void) const {
	Int_Int_Map::const_iterator find_it = wing_public_cfg_map_.find(16);
	if (find_it == wing_public_cfg_map_.end()) return 0;
	return find_it->second;
}

const int Config_Cache_Public::get_wing_experience_mins(void) const {
	Int_Int_Map::const_iterator find_it = wing_public_cfg_map_.find(17);
	if (find_it == wing_public_cfg_map_.end()) return 0;
	return find_it->second;
}

const int Config_Cache_Public::get_wing_experience_ret_reward_time(void) const {
	Int_Int_Map::const_iterator find_it = wing_public_cfg_map_.find(18);
	if (find_it == wing_public_cfg_map_.end()) return 0;
	return find_it->second;
}

const Arena_Adore_Public &Config_Cache_Public::get_arena_adore_data(void) const {
	return arena_adore_data;
}
