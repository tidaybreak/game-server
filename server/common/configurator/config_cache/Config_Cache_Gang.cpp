/*
 * ConfigCacheGang.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: caijianwei
 */

#include "Config_Cache_Gang.h"
#include "Configurator.h"
#include "Json_Utility.h"

void Config_Cache_Gang::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Gang");

	refresh_gang_boss();
	refresh_limit();
	refresh_base();
	refresh_power();
	refresh_event();
	refresh_skill();
	refresh_bless_buf();
	refresh_role_bless();
	refresh_draw_card();
	refresh_shop_items();
	refresh_gang_war();
}

const Gang_Config& Config_Cache_Gang::limit() const {
	return gang_config_limit_;
}

void Config_Cache_Gang::refresh_gang_boss(void){
	bool is_find = false;
	gang_boss_open_lv_ = 0;
	const Json::Value& json_gang = CONFIG_INSTANCE->config_json()["guild"]["guild_activity"];
	for (Json::Value::iterator it = json_gang.begin(); it != json_gang.end(); ++it) {
		const Json::Value &config_json = (*it);
		int action = 0;
		int lv = 0;
		if (config_json.isMember("action")){
			action = config_json["action"].asInt();
		}
		if (config_json.isMember("level")){
			lv = config_json["level"].asInt();
		}
		if(action == 3){
			is_find = true;
			gang_boss_open_lv_ = lv;
			break;
		}
	}
	if(!is_find){
		gang_boss_open_lv_ = 99999;
	}
}
void Config_Cache_Gang::refresh_limit() {
	const Json::Value& json_gang = CONFIG_INSTANCE->config_json()["guild"]["guild_limit"];
	for (Json::Value::iterator iter_limit = json_gang.begin();
			iter_limit != json_gang.end(); ++iter_limit) {
		gang_config_limit_.change_chief = (*iter_limit)["guild_change_chief"].asUInt();
		gang_config_limit_.cost_friendset = (*iter_limit)["guild_cost_friendset"].asUInt();
		gang_config_limit_.cost_notice = (*iter_limit)["guild_cost_notice"].asUInt();
		gang_config_limit_.create_cost = (*iter_limit)["guild_create_cost"].asUInt();
		gang_config_limit_.create_cost_copper = (*iter_limit)["create_cost_copper"].asUInt();
		gang_config_limit_.diam_donate = (*iter_limit)["guild_cost_Diamond"].asUInt();
		gang_config_limit_.diam_donate_plus = (*iter_limit)["guild_cost_Diamond_add"].asUInt();
		gang_config_limit_.gold_donate = (*iter_limit)["guild_cost_money"].asUInt();
		gang_config_limit_.gold_donate_plus = (*iter_limit)["guild_cost_money_add"].asUInt();
		gang_config_limit_.limit_level = (*iter_limit)["guild_create_level"].asInt();
		gang_config_limit_.upgrade_cd_clear_rate = (*iter_limit)["upgrade_cd_clear_rate"].asInt();
		gang_config_limit_.diam_upgrade_gift = (*iter_limit)["diam_upgrade_gift"].asInt();
		gang_config_limit_.group_mail_cost = (*iter_limit)["group_mail_cost"].asInt();
		gang_config_limit_.min_donate_copper = (*iter_limit)["min_donate_copper"].asInt();
		gang_config_limit_.min_donate_diam = (*iter_limit)["min_donate_diam"].asInt();
		gang_config_limit_.max_gang_lvl = (*iter_limit)["max_gang_lvl"].asInt();
		gang_config_limit_.copper_to_contrib = (*iter_limit)["copper_to_contrib"].asDouble();
		gang_config_limit_.diam_to_contrib = (*iter_limit)["diam_to_contrib"].asDouble();

		Json_Utility::assign_time_value((*iter_limit)["guild_join_cd"], gang_config_limit_.join_cd);
		Json_Utility::assign_time_value((*iter_limit)["guild_cd_friendset"], gang_config_limit_.cd_friendset);
		Json_Utility::assign_time_value((*iter_limit)["guild_cd_notice"], gang_config_limit_.cd_notice);
		Json_Utility::assign_time_value((*iter_limit)["guild_change_noticecd"], gang_config_limit_.change_noticecd);

		gang_config_limit_.cd_seize_power.set((*iter_limit)["cd_seize_power"].asInt(), 0);
	}
}

const Gang_Config_Level& Config_Cache_Gang::base(uint32_t level) {
	return gang_config_base_.get_level( level );
}

void Config_Cache_Gang::refresh_base() {
	const Json::Value& json_gang = CONFIG_INSTANCE->config_json()["guild"]["guild_base"];
	Gang_Config_Level config_level;
	for (Json::Value::iterator iter_limit = json_gang.begin();
			iter_limit != json_gang.end(); ++iter_limit) {
		config_level.reset();

		config_level.level = (*iter_limit)["Guild_level"].asUInt();
		config_level.population = (*iter_limit)["guild_Population"].asUInt();
		config_level.cost = (*iter_limit)["guild_cost"].asUInt();
		config_level.money = (*iter_limit)["guild_money"].asUInt();
		config_level.donate_gain = (*iter_limit)["guild_gain_cost"].asUInt();
		config_level.upgrade_cd.set((*iter_limit)["guild_cd"].asInt(), 0);
		config_level.tower_upgrade_cd.set((*iter_limit)["tower_upgrade_cd"].asInt(), 0);
		config_level.altar_upgrade_cd.set((*iter_limit)["altar_upgrade_cd"].asInt(), 0);
		config_level.shop_upgrade_cd.set((*iter_limit)["shop_upgrade_cd"].asInt(), 0);
		Json_Utility::assign_int_vec((*iter_limit)["draw_box"], config_level.draw_box);
		config_level.daily_draw_times = (*iter_limit)["daily_draw_times"].asInt();
		config_level.draw_cost_contrib = (*iter_limit)["draw_cost_contrib"].asInt();
		config_level.tower_up_cost_contrib = (*iter_limit)["tower_up_cost_contrib"].asInt();
		config_level.altar_up_cost_contrib = (*iter_limit)["altar_up_cost_contrib"].asInt();
		config_level.shop_upgrade_contrib = (*iter_limit)["shop_upgrade_contrib"].asInt();
		Json_Utility::assign_item_vec((*iter_limit)["upgrade_gift"], config_level.upgrade_gift);

		gang_config_base_.add_level(config_level);
	}
}

const Gang_Config_Authority& Config_Cache_Gang::power(int8_t headship) {
	return gang_config_power_.get_authority(headship);
}

void Config_Cache_Gang::refresh_power() {
	const Json::Value& json_gang = CONFIG_INSTANCE->config_json()["guild"]["guild_power"];
	Gang_Config_Authority config_authority;
	for (Json::Value::iterator iter_limit = json_gang.begin();
			iter_limit != json_gang.end(); ++iter_limit) {
		config_authority.headship = (*iter_limit)["guild_menbertype"].asUInt();
		config_authority.name = (*iter_limit)["guild_menber_name"].asString();
		config_authority.max_count = (*iter_limit)["guild_number_menber"].asUInt();
		Json_Utility::assign_int_vec((*iter_limit)["guild_power_Initialization"], config_authority.default_auth);
		Json_Utility::assign_int_vec((*iter_limit)["guild_power_given"], config_authority.can_granted);
		gang_config_power_.add_authority(config_authority);
	}
}

void Config_Cache_Gang::refresh_event(void) {
	Gang_Cfg_Event event_config;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["guild"]["guild_events"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		event_config.reset();

		event_config.event_id = (*it_json)["event_id"].asInt();
		ABORT_INT_ZERO(event_config.event_id);
		event_config.event_name = (*it_json)["event_name"].asString();
		event_config.event_content = (*it_json)["event_content"].asString();

		set_map_second_value_by_key(event_config.event_id, event_cfg_map_, event_config);
	}
}

const Gang_Cfg_Event *Config_Cache_Gang::find_event(const int event_id) const {
	return get_map_second_pointer_by_key(event_id, event_cfg_map_);
}

void Config_Cache_Gang::refresh_skill(void) {
	Gang_Cfg_Skill_Id cfg_row_info;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["guild"]["guild_skill_id"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.skill_id = (*it_json)["skill_id"].asInt();
		ABORT_INT_ZERO(cfg_row_info.skill_id);
		cfg_row_info.skill_lvl = (*it_json)["skill_lvl"].asInt();
		cfg_row_info.skill_type = (*it_json)["skill_type"].asInt();
		cfg_row_info.need_player_lvl = (*it_json)["need_player_lvl"].asInt();
		cfg_row_info.need_tower_lvl = (*it_json)["need_tower_lvl"].asInt();
		cfg_row_info.research_cost_fund = (*it_json)["research_cost_fund"].asInt();
		cfg_row_info.learn_cost_fund = (*it_json)["learn_cost_fund"].asInt();
		cfg_row_info.prop = (*it_json)["prop"].asInt();
		cfg_row_info.prop_val = (*it_json)["prop_val"].asDouble();

		set_map_second_value_by_key(cfg_row_info.skill_id, skill_id_map_, cfg_row_info);
	}
}

const Gang_Cfg_Skill_Id *Config_Cache_Gang::find_skill(const int skill_id) const {
	return get_map_second_pointer_by_key(skill_id, skill_id_map_);
}

const Gang_Cfg_Skill_Id_Map *Config_Cache_Gang::skill_map(void) const {
	return &skill_id_map_;
}

void Config_Cache_Gang::refresh_bless_buf(void) {
	Gang_Cfg_Bless_Buf_Id cfg_row_info;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["guild"]["bless_buf_id"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.bless_buf_id = (*it_json)["bless_buf_id"].asInt();
		ABORT_INT_ZERO(cfg_row_info.bless_buf_id);
		cfg_row_info.need_gold = (*it_json)["need_gold"].asInt();

		const Json::Value &json_effect = (*it_json)["effect"];
		if (Json::Value::null == json_effect || !json_effect.isArray() || json_effect.size() != 3) {
			abort();
		}
		if (!json_effect[1u].isArray() || !json_effect[2u].isArray() ||
				json_effect[1u].size() != 1 || json_effect[2u].size() != 2) {
			abort();
		}
		cfg_row_info.effect_id = json_effect[0u].asInt();
		Json_Utility::assign_time_value(json_effect[1u][0u], cfg_row_info.effect_time);
		cfg_row_info.effect_prop = json_effect[2u][0u].asInt();
		cfg_row_info.effect_val = json_effect[2u][1u].asDouble();

		set_map_second_value_by_key(cfg_row_info.bless_buf_id, bless_buf_id_map_, cfg_row_info);
	}
}

const Gang_Cfg_Bless_Buf_Id *Config_Cache_Gang::find_bless_buf(const int bless_buf_id) const {
	return get_map_second_pointer_by_key(bless_buf_id, bless_buf_id_map_);
}

const Gang_Cfg_Bless_Buf_Id_Map& Config_Cache_Gang::bless_buf_id_map(void) const {
	return bless_buf_id_map_;
}
std::string Config_Cache_Gang::team_defend_boss_name(void) const {
	return team_defend_boss_name_;
}

void Config_Cache_Gang::refresh_role_bless(void) {
	Gang_Cfg_Role_Bless cfg_row_info;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["guild"]["role_bless"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.role_level = (*it_json)["role_level"].asInt();
		ABORT_INT_ZERO(cfg_row_info.role_level);
		cfg_row_info.bless_buf_1 = (*it_json)["bless_buf_1"].asInt();
		cfg_row_info.bless_buf_2 = (*it_json)["bless_buf_2"].asInt();
		cfg_row_info.bless_buf_3 = (*it_json)["bless_buf_3"].asInt();
		cfg_row_info.need_gold = (*it_json)["need_gold"].asInt();

		set_map_second_value_by_key(cfg_row_info.role_level, role_bless_map_, cfg_row_info);
	}
}

const Gang_Cfg_Role_Bless *Config_Cache_Gang::find_role_bless(const int role_lvl) const {
	return get_map_second_pointer_by_key(role_lvl, role_bless_map_);
}

void Config_Cache_Gang::refresh_draw_card(void) {
	Gang_Cfg_Draw_Card cfg_row_info;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["guild"]["draw_box"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.draw_card_id = (*it_json)["draw_card_id"].asInt();
		ABORT_INT_ZERO(cfg_row_info.draw_card_id);
		cfg_row_info.item_id = (*it_json)["item_id"].asInt();
		cfg_row_info.item_amount = (*it_json)["item_amount"].asInt();
		cfg_row_info.rate = (*it_json)["rate"].asInt();
		cfg_row_info.is_rare = (*it_json)["is_rare"].asBool();

		set_map_second_value_by_key(cfg_row_info.draw_card_id, draw_card_map_, cfg_row_info);
	}
}

void Config_Cache_Gang::refresh_shop_items(void) {
	Gang_Cfg_Shop_Item cfg_row_info;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["guild"]["shop_items"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.item_id = (*it_json)["item_id"].asInt();
		ABORT_INT_ZERO(cfg_row_info.item_id);
		cfg_row_info.cost_contrib = (*it_json)["cost_contrib"].asInt();
		cfg_row_info.need_shop_lvl = (*it_json)["need_shop_lvl"].asInt();

		set_map_second_value_by_key(cfg_row_info.item_id, shop_item_map_, cfg_row_info);
	}
}

void Config_Cache_Gang::refresh_gang_war(void) {
	{
		gang_war_rank_cfg_.clear();
		Gang_War_Rank_Detail_Cfg cfg;
		const Json::Value &json = CONFIG_INSTANCE->config_json()["gang"]["gang_war"];
		for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
			cfg.reset();
			cfg.index = (*it_json)["rank"].asInt();
//			cfg.fund = (*it_json)["fund"].asInt();
			if (!(*it_json)["list"].isNull() && (*it_json)["list"].isArray()) {
				Json_Utility::assign_item_vec((*it_json)["list"], cfg.list);
			}
			if (cfg.list.size() != 4) {
//				LOG_ABORT("gang_war_award win size not 4: %d", cfg.list.size());
			}
			if (!(*it_json)["self_list"].isNull() && (*it_json)["self_list"].isArray()) {
				Json_Utility::assign_item_vec((*it_json)["self_list"], cfg.self_list);
			}
			if (!(*it_json)["lose_list"].isNull() && (*it_json)["lose_list"].isArray()) {
				Json_Utility::assign_item_vec((*it_json)["lose_list"], cfg.lose_list);
			}
			if (!(*it_json)["lose_self_list"].isNull() && (*it_json)["lose_self_list"].isArray()) {
				Json_Utility::assign_item_vec((*it_json)["lose_self_list"], cfg.lose_self_list);
			}
//			cfg.win_self_contri = (*it_json)["win_self_contri"].asInt();
//			cfg.win_self_mercit = (*it_json)["win_self_mercit"].asInt();

			set_map_second_value_by_key(cfg.index, gang_war_rank_cfg_, cfg);
		}
	}
	{
		const Json::Value &json = CONFIG_INSTANCE->config_json()["gang"]["setting"];
		for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
			gang_war_setting_cfg_.contribute = (*it_json)["contribute"].asInt();
			gang_war_setting_cfg_.mercit = (*it_json)["mercit"].asInt();
			gang_war_setting_cfg_.finish_time = (*it_json)["finish_time"].asInt();
			gang_war_setting_cfg_.ready_time = (*it_json)["ready_time"].asInt();
			gang_war_setting_cfg_.tower_change_time = (*it_json)["tower_change_time"].asInt();
			gang_war_setting_cfg_.tower_score = (*it_json)["tower_score"].asInt();
			gang_war_setting_cfg_.tower_score_center = (*it_json)["tower_score_center"].asInt();
			gang_war_setting_cfg_.tower_score_tick = (*it_json)["tower_score_tick"].asInt();
			gang_war_setting_cfg_.win_score = (*it_json)["win_score"].asInt();
			gang_war_setting_cfg_.skill_cd = (*it_json)["skill_cd"].asInt();
			gang_war_setting_cfg_.revive_time = (*it_json)["revive_time"].asInt();
			gang_war_setting_cfg_.level_limit = (*it_json)["level_limit"].asInt();
			gang_war_setting_cfg_.box_limit = (*it_json)["box_limit"].asInt();
			gang_war_setting_cfg_.empty_score = (*it_json)["empty_score"].asInt();
			gang_war_setting_cfg_.kill_score = (*it_json)["kill_score"].asInt();
			gang_war_setting_cfg_.hold_score = (*it_json)["hold_score"].asInt();
		}
	}
	{
		const Json::Value &json = CONFIG_INSTANCE->config_json()["gang"]["score"];
		int max_gap = 0;
		for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
			int time = (*it_json)["time"].asInt();
			int win_score = (*it_json)["win"].asInt();
			int lose_score = (*it_json)["lose"].asInt();
			gang_war_setting_cfg_.win_scores.insert(std::make_pair(time, win_score));
			gang_war_setting_cfg_.lose_scores.insert(std::make_pair(time, lose_score));
			if (max_gap < time) {
				max_gap = time;
			}
		}
		Int_Vec undefined_gap;
		for (int i = 0; i <= max_gap; ++i) {
			Int_Int_Map::iterator it = gang_war_setting_cfg_.win_scores.find(i);
			if (it != gang_war_setting_cfg_.win_scores.end()) {
				int lose_score = 0;
				Int_Int_Map::iterator it_ls = gang_war_setting_cfg_.lose_scores.find(i);
				if (it_ls != gang_war_setting_cfg_.lose_scores.end()) {
					lose_score = it_ls->second;
				}
				for (Int_Vec::const_iterator it_ug = undefined_gap.begin();
						it_ug != undefined_gap.end(); ++it_ug) {
					gang_war_setting_cfg_.win_scores.insert(std::make_pair(*it_ug, it->second));
					gang_war_setting_cfg_.lose_scores.insert(std::make_pair(*it_ug, lose_score));
				}
				undefined_gap.clear();
				continue;
			}
			undefined_gap.push_back(i);
		}
	}
	{
		gang_war_rank_award_cfg_.clear();
		Gang_War_Rank_Award_Detail_Cfg cfg;
		const Json::Value &json = CONFIG_INSTANCE->config_json()["gang"]["rank_award"];
		for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
			cfg.reset();
			cfg.rank = (*it_json)["rank"].asInt();
			cfg.stage = (*it_json)["stage"].asInt();
			if (!(*it_json)["list"].isNull() && (*it_json)["list"].isArray()) {
				Json_Utility::assign_item_vec((*it_json)["list"], cfg.list);
			}
			if (!(*it_json)["all_list"].isNull() && (*it_json)["all_list"].isArray()) {
				Json_Utility::assign_item_vec((*it_json)["all_list"], cfg.all_list);
			}

			set_map_second_value_by_key(cfg.rank, gang_war_rank_award_cfg_, cfg);
		}
	}
	{
		weekday_string_.clear();
		team_defend_boss_name_.clear();
		const Json::Value &json = CONFIG_INSTANCE->config_json()["public"]["public"];
		for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
			int key = (*it_json)["type"].asInt();
			if (key == 4) {
				if ((*it_json)["val"].isArray() && !(*it_json)["val"].isNull()) {
					uint16_t len = (*it_json)["val"].size();
					for (uint16_t i = 0; i < len; ++i) {
						std::string str;
						str = (*it_json)["val"][i].asString();
						set_map_second_value_by_key(i+1, weekday_string_, str);
					}
				}
			} else if (key == 6) {
				std::string str;
				str = (*it_json)["val"].asString();
				set_map_second_value_by_key(8, weekday_string_, str);
			} else if (key == 7) {
				if ((*it_json)["val"].isArray() && !(*it_json)["val"].isNull()) {
					uint16_t len = (*it_json)["val"].size();
					for (uint16_t i = 0; i < len; ++i) {
						std::string str;
						str = (*it_json)["val"][i].asString();
						set_map_second_value_by_key(i+1, part_string_, str);
					}
				}
			} else if (key == 8) {
				team_defend_boss_name_ = (*it_json)["val"].asString();
			}
		}
	}
}

const Gang_Cfg_Draw_Card *Config_Cache_Gang::find_draw_card(const int draw_card_id) const {
	return get_map_second_pointer_by_key(draw_card_id, draw_card_map_);
}

const Gang_Cfg_Shop_Item *Config_Cache_Gang::find_shop_item(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, shop_item_map_);
}

const Gang_War_Setting_Cfg &Config_Cache_Gang::gang_war_setting_cfg(void) const {
	return gang_war_setting_cfg_;
}
const Gang_War_Rank_Detail_Cfg *Config_Cache_Gang::find_gang_war_rank(const int8_t index) const {
	return get_map_second_pointer_by_key(index, gang_war_rank_cfg_);
//	Gang_War_Rank_Cfg::iterator it = gang_war_rank_cfg_.find(index);
//	if (it != gang_war_rank_cfg_.end()) {
//		return &(it->second);
//	}
//	return NULL;
}
const Gang_War_Rank_Award_Detail_Cfg *Config_Cache_Gang::find_gang_war_rank_award(const int8_t rank) const {
	return get_map_second_pointer_by_key(rank, gang_war_rank_award_cfg_);
}
const std::string *Config_Cache_Gang::find_weekday_string(const int weekday) const{
	return get_map_second_pointer_by_key(weekday, weekday_string_);
}
const std::string *Config_Cache_Gang::find_part_string(const int part) const{
	return get_map_second_pointer_by_key(part, part_string_);
}

int Config_Cache_Gang::get_open_gang_boss_lv(void){
	return gang_boss_open_lv_;
}
