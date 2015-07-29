/*
 * Config_Cache_Arena.cpp
 *
 *  Created on: 2014年5月23日
 *      Author: root
 */

#include "Config_Cache_Arena.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include <iostream>
#include <fstream>

void Config_Cache_Arena::refresh_config_cache(void){
	refresh_arena_award();
	refresh_arena_season_award();
	refresh_arena_base();
	refresh_arena_levelfix();
	refresh_arena_setting();
	refresh_arena_season();
	refresh_arena_force_level();
}

void Config_Cache_Arena::refresh_arena_award(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["arena"]["arena_award"];
	Arena_Award_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("award_grouping")){
			cache.rank_begin = config_json["award_grouping"][0u].asInt();
			cache.rank_end = config_json["award_grouping"][1u].asInt();
		}
		if (config_json.isMember("rank_award_soul")){
			cache.rank_award_soul = config_json["rank_award_soul"].asInt();
		}
		if (config_json.isMember("rank_award_gold")){
			cache.rank_award_gold = config_json["rank_award_gold"].asInt();
		}
		if (config_json.isMember("rank_award_exploit")){
					cache.rank_award_exploit = config_json["rank_award_exploit"].asInt();
		}
		if (config_json.isMember("rank_award_item")){

			Json_Utility::assign_int_int_vec(config_json["rank_award_item"], cache.rank_award_item_vec);
		}
		set_map_second_value_by_key(cache.rank_begin, arena_award_map_, cache);
	}
}

void Config_Cache_Arena::refresh_arena_season_award(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["arena"]["arena_award_season"];
	Arena_Season_Award_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("rank_award_soul")){
			cache.rank_award_soul = config_json["rank_award_soul"].asInt();
		}
		if (config_json.isMember("rank_award_gold")){
			cache.rank_award_gold = config_json["rank_award_gold"].asInt();
		}
		if (config_json.isMember("rank_award_item")){

			Json_Utility::assign_int_int_vec(config_json["rank_award_item"], cache.rank_award_item_vec);
		}

		int season_rank = 0;
		int rank_award = 0;
		if (config_json.isMember("rank_award")){
			rank_award = config_json["rank_award"].asInt();
		}
		if (config_json.isMember("season_rank")){
			season_rank = config_json["season_rank"].asInt();
		}
		if(rank_award == 0){
			set_map_second_value_by_key(season_rank, arena_season_group_award_map_, cache);
		}else{
			set_map_second_value_by_key(rank_award, arena_season_rank_award_map_, cache);
		}
	}
}


void Config_Cache_Arena::refresh_arena_base(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["arena"]["arena_base"];
	Arena_Base_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("arena_group")){
			cache.arena_group = config_json["arena_group"].asInt();
		}
		if (config_json.isMember("arena_part")){
			cache.arena_part = config_json["arena_part"].asInt();
		}
		if (config_json.isMember("arena_award")){
			cache.arena_award = config_json["arena_award"].asInt();
		}
		if (config_json.isMember("arena_punish")){
			cache.arena_punish = config_json["arena_punish"].asInt();
		}

		if (config_json.isMember("arena_award_hard")){
			cache.arena_award_hard = config_json["arena_award_hard"].asInt();
		}
		if (config_json.isMember("arena_punish_hard")){
			cache.arena_punish_hard = config_json["arena_punish_hard"].asInt();
		}
		if (config_json.isMember("arena_award_gold")){
			cache.arena_award_gold = config_json["arena_award_gold"].asInt();
		}
		if (config_json.isMember("arena_punish_gold")){
			cache.arena_punish_gold = config_json["arena_punish_gold"].asInt();
		}

		if (config_json.isMember("winer_award")){
			cache.winer_award = config_json["winer_award"].asInt();
		}
		if (config_json.isMember("loser_award")){
			cache.loser_award = config_json["loser_award"].asInt();
		}
		if (config_json.isMember("winer_award_item")){
			Json_Utility::assign_id_int_int_vec(config_json["winer_award_item"],
					cache.winer_award_item_vec);
		}
		if (config_json.isMember("loser_award_item")){
			Json_Utility::assign_id_int_int_vec(config_json["loser_award_item"],
					cache.loser_award_item_vec);
		}

		if (config_json.isMember("winer_award_item_hard")){
			Json_Utility::assign_id_int_int_vec(config_json["winer_award_item_hard"],
					cache.winer_award_item_hard);
		}
		if (config_json.isMember("loser_award_item_hard")){
			Json_Utility::assign_id_int_int_vec(config_json["loser_award_item_hard"],
					cache.loser_award_item_hard);
		}
		if (config_json.isMember("winer_award_item_gold")){
			Json_Utility::assign_id_int_int_vec(config_json["winer_award_item_gold"],
					cache.winer_award_item_gold);
		}
		if (config_json.isMember("loser_award_item_gold")){
			Json_Utility::assign_id_int_int_vec(config_json["loser_award_item_gold"],
					cache.loser_award_item_gold);
		}

		if (config_json.isMember("arena_serve")){
			cache.arena_serve = config_json["arena_serve"].asInt();
		}
		if (config_json.isMember("arena_promote")){
			cache.arena_promote = config_json["arena_promote"].asInt();
		}
		if (config_json.isMember("arena_promote_award")){
			cache.arena_promote_award = config_json["arena_promote_award"].asInt();
		}
		if (config_json.isMember("arena_promote_punish")){
			cache.arena_promote_punish = config_json["arena_promote_punish"].asInt();
		}
		if (config_json.isMember("arena_promote_awarditem")){

			Json_Utility::assign_id_int_int_vec(config_json["arena_promote_awarditem"],
					cache.arena_promote_awarditem_vec);
		}
		if (config_json.isMember("arena_name")){
			cache.name = config_json["arena_name"].asString();
		}
		if (config_json.isMember("bronze_refer_force")){
			cache.bronze_refer_force = config_json["bronze_refer_force"].asInt();
		}
		int id = cache.arena_group*10+cache.arena_part;
		set_map_second_value_by_key(id, arena_base_map_, cache);
	}
}

void Config_Cache_Arena::refresh_arena_levelfix(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["arena"]["arena_levelfix"];
	Arena_Levelfix_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("level")){
			cache.level = config_json["level"].asInt();
		}
		if (config_json.isMember("fix_level")){
			cache.fix_level = config_json["fix_level"].asDouble();
		}
		set_map_second_value_by_key(cache.level, arena_levelfix_map_, cache);
	}
}

void Config_Cache_Arena::refresh_arena_setting(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["arena"]["arena_setting"];
	Arena_Setting_Config&cache = this->arena_setting_;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("athletics_number")){
			cache.athletics_number = config_json["athletics_number"].asInt();
		}
		if (config_json.isMember("athletics_buy_limit")){
			cache.athletics_buy_limit = config_json["athletics_buy_limit"].asInt();
		}
		if (config_json.isMember("athletics_initial_cost")){
			cache.athletics_initial_cost = config_json["athletics_initial_cost"].asInt();
		}
		if (config_json.isMember("athletics_cost_increase")){
			cache.athletics_cost_increase = config_json["athletics_cost_increase"].asInt();
		}
		if (config_json.isMember("cd")){
			cache.cd = config_json["cd"].asInt();
		}
		if (config_json.isMember("min_cd")){
			cache.min_cd = config_json["min_cd"].asInt();
		}
		if (config_json.isMember("reset_cd_cost")){
			cache.reset_cd_cost = config_json["reset_cd_cost"].asInt();
		}
		if (config_json.isMember("no_cd_vip_lv")){
			cache.no_cd_vip_lv = config_json["no_cd_vip_lv"].asInt();
		}
		if (config_json.isMember("fight_map_id")){
			cache.fight_map_id = config_json["fight_map_id"].asInt();
		}
		if (config_json.isMember("arena_open_lv")){
			cache.arena_open_lv = config_json["arena_open_lv"].asInt();
		}
		if(config_json.isMember("athletics_buy_limit_vip_lv")){
			cache.pay_time_vip_lv = config_json["athletics_buy_limit_vip_lv"].asInt();
		}
		if (config_json.isMember("refresh_match_cost")){
			cache.refresh_match_cost = config_json["refresh_match_cost"].asInt();
		}
		if (config_json.isMember("refresh_match_time")){
			cache.refresh_match_time = config_json["refresh_match_time"].asInt();
		}
		if (config_json.isMember("mail_title")){
			cache.mail_title = config_json["mail_title"].asString();
		}
		if (config_json.isMember("mail_sender")){
			cache.mail_sender = config_json["mail_sender"].asString();
		}
		if (config_json.isMember("mail_content")){
			cache.mail_content = config_json["mail_content"].asString();
		}

		if (config_json.isMember("text_copper")){
			cache.text_copper = config_json["text_copper"].asString();
		}
		if (config_json.isMember("text_soul")){
			cache.text_soul = config_json["text_soul"].asString();
		}
		if (config_json.isMember("text_exploit")){
			cache.text_exploit = config_json["text_exploit"].asString();
		}
//		if (config_json.isMember("adore_cost_1")){
//
//			Json_Utility::assign_int_int_vec(config_json["adore_cost_1"], cache.adore_cost_1);
//		}
//		if (config_json.isMember("adore_cost_2")){
//
//			Json_Utility::assign_int_int_vec(config_json["adore_cost_2"], cache.adore_cost_2);
//		}
//		if (config_json.isMember("adore_cost_3")){
//
//			Json_Utility::assign_int_int_vec(config_json["adore_cost_3"], cache.adore_cost_3);
//		}
//		if (config_json.isMember("adore_item_1")){
//
//			Json_Utility::assign_int_int_vec(config_json["adore_item_1"], cache.adore_item_1);
//		}
//		if (config_json.isMember("adore_item_2")){
//
//			Json_Utility::assign_int_int_vec(config_json["adore_item_2"], cache.adore_item_2);
//		}
//		if (config_json.isMember("adore_item_3")){
//
//			Json_Utility::assign_int_int_vec(config_json["adore_item_3"], cache.adore_item_3);
//		}
//		cache.adore_times = config_json["adore_times"].asInt();

	}
}

void Config_Cache_Arena::refresh_arena_season(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["arena"]["arena_season"];
	Arena_Season_Config &cache = this->arena_season_;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("season_status")){
			cache.season_status = config_json["season_status"].asInt();
		}
		if (config_json.isMember("begin_time")){
			Id_Int_Int id_int_int;
			Json_Utility::assign_id_int_int(config_json["begin_time"], id_int_int);
			Date_Time data_time(id_int_int.val2, id_int_int.val1, id_int_int.id);
			cache.begin_time.sec(data_time.time_sec());
		}
		if (config_json.isMember("finish_time")){
			Id_Int_Int id_int_int;
			Json_Utility::assign_id_int_int(config_json["finish_time"], id_int_int);
			Date_Time data_time(id_int_int.val2, id_int_int.val1, id_int_int.id);
			cache.finish_time.sec(data_time.time_sec());
		}
		if (config_json.isMember("ref_time")){
			cache.ref_time.sec(config_json["ref_time"].asInt());
		}
		if (config_json.isMember("ref_time1")){
			cache.ref_time1.sec(config_json["ref_time1"].asInt());
		}
		if (config_json.isMember("ref_time2")){
			cache.ref_time2.sec(config_json["ref_time2"].asInt());
		}
	}
}

void Config_Cache_Arena::refresh_arena_force_level(void){
	force_level_map_.clear();
	max_force_level_ = 0;
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["arena"]["battle_level"];
	Int_Int cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		int level_key = 0;
		if (config_json.isMember("battle_level")){
			level_key = config_json["battle_level"].asInt();
		}
		if (config_json.isMember("battle_low")){
			cache.val_1 = config_json["battle_low"].asInt();
		}
		if (config_json.isMember("battle_high")){
			cache.val_2 = config_json["battle_high"].asInt();
		}
		if(level_key > max_force_level_){
			max_force_level_ = level_key;
		}
		force_level_map_.insert(std::make_pair(level_key, cache));
	}
}

Arena_Award_Config* Config_Cache_Arena::get_arena_award_conifg(int id){

	Arena_Award_Config_Map::iterator it;
	for(it = arena_award_map_.begin(); it != arena_award_map_.end(); ++it){
		if(id >= it->second.rank_begin && id <= it->second.rank_end){
			return &(it->second);
		}
	}
	return NULL;
}

Arena_Season_Award_Config* Config_Cache_Arena::get_arena_season_rank_award_conifg(int id){
	Arena_Season_Award_Config_Map::iterator it =  arena_season_rank_award_map_.find(id);
	if(it != arena_season_rank_award_map_.end()){
		return &(it->second);
	}
	return NULL;
}
Arena_Season_Award_Config* Config_Cache_Arena::get_arena_season_group_award_conifg(int id){
	Arena_Season_Award_Config_Map::iterator it =  arena_season_group_award_map_.find(id);
	if(it != arena_season_group_award_map_.end()){
		return &(it->second);
	}
	return NULL;
}

Arena_Base_Config* Config_Cache_Arena::get_arena_base_conifg(int id){
	Arena_Base_Config_Map::iterator it = arena_base_map_.find(id);
	if(it != arena_base_map_.end()){
		return &(it->second);
	}
	return NULL;
}

Arena_Setting_Config* Config_Cache_Arena::get_arena_setting_conifg(void){
	return &arena_setting_;
}

Arena_Levelfix_Config* Config_Cache_Arena::get_arena_levelfix_conifg(int id){
	Arena_Levelfix_Config_Map::iterator it = arena_levelfix_map_.find(id);
	if(it != arena_levelfix_map_.end()){
		return &(it->second);
	}
	return NULL;
}

double Config_Cache_Arena::get_arena_levelfix(int id){
	Arena_Levelfix_Config_Map::iterator it = arena_levelfix_map_.find(id);
	if(it != arena_levelfix_map_.end()){
		return it->second.fix_level;
	}
	return 1;
}

Arena_Season_Config* Config_Cache_Arena::arena_season_config(void){
	return &arena_season_;
}

void Config_Cache_Arena::save_arena_season_to_json(void){
	//Json::Value& root =
	//			CONFIG_INSTANCE->config_json()["arena"]["arena_season"];
	Json::Value root;
	root[0u]["season_status"] = Json::Value(arena_season_.season_status);
	int value;
	Date_Time b_data_time(arena_season_.begin_time);
	value = b_data_time.year();
	root[0u]["begin_time"][0u] = Json::Value(value);
	value = b_data_time.month();
	root[0u]["begin_time"][1u] = Json::Value(value);
	value = b_data_time.day();
	root[0u]["begin_time"][2u] = Json::Value(value);

	Date_Time f_data_time(arena_season_.finish_time);
	value = f_data_time.year();
	root[0u]["finish_time"][0u] = Json::Value(value);
	value = f_data_time.month();
	root[0u]["finish_time"][1u] = Json::Value(value);
	value = f_data_time.day();
	root[0u]["finish_time"][2u] = Json::Value(value);

	value = arena_season_.ref_time.sec();
	root[0u]["ref_time"] = Json::Value(value);
	value = arena_season_.ref_time1.sec();
	root[0u]["ref_time1"] = Json::Value(value);
	value = arena_season_.ref_time2.sec();
	root[0u]["ref_time2"] = Json::Value(value);

	Json::StyledWriter writer;
	std::string output = writer.write(root);
	std::ofstream file("./config/arena/arena_season.json");
	file<<output;
}

int Config_Cache_Arena::get_arena_cd(void){
	return arena_setting_.cd;
}

int Config_Cache_Arena::get_arena_min_cd(void){
	return arena_setting_.min_cd;
}

int Config_Cache_Arena::get_arena_cd_cost(void){
	return arena_setting_.reset_cd_cost;
}

int Config_Cache_Arena::get_arena_no_cd_vip_lv(void){
	return arena_setting_.no_cd_vip_lv;
}

int Config_Cache_Arena::get_arena_fight_map_id(void){
	return arena_setting_.fight_map_id;
}

int Config_Cache_Arena::get_bronze_refer_force(const int group, const int seg){
	int id = group*10+seg;
	Arena_Base_Config* cfg = get_arena_base_conifg(id);
	if(cfg){
		return cfg->bronze_refer_force;
	}
	return 0;
}

int Config_Cache_Arena::get_arena_open_lv(void){
	return this->arena_setting_.arena_open_lv;
}

int Config_Cache_Arena::get_arena_pay_time_vip_lv(void){
	return this->arena_setting_.pay_time_vip_lv;
}

int Config_Cache_Arena::get_refresh_match_cost(void){
	return this->arena_setting_.refresh_match_cost;
}

int Config_Cache_Arena::get_refresh_match_time(void){
	return this->arena_setting_.refresh_match_time;
}

std::string Config_Cache_Arena::get_part_name(const int group, const int seg){
	int id = group*10 + seg;
	Arena_Base_Config* cfg = get_arena_base_conifg(id);
	if(cfg){
		return cfg->name;
	}
	return "";
}

std::string Config_Cache_Arena::get_mail_title(void){
	return arena_setting_.mail_title;
}

std::string Config_Cache_Arena::get_mail_sender(void){
	return arena_setting_.mail_sender;
}

std::string Config_Cache_Arena::get_mail_content(void){
	return arena_setting_.mail_content;
}

std::string Config_Cache_Arena::get_text_copper(void){
	return arena_setting_.text_copper;
}

std::string Config_Cache_Arena::get_text_soul(void){
	return arena_setting_.text_soul;
}

std::string Config_Cache_Arena::get_text_exploit(void){
	return arena_setting_.text_exploit;
}
int Config_Cache_Arena::get_arena_force_level(int force){
	Arena_Force_Level_Map::const_iterator it;
	for(it = force_level_map_.begin(); it != force_level_map_.end(); ++it){
		if(force >= it->second.val_1 && force <= it->second.val_2){
			return it->first;
		}
	}
	return max_force_level_;
}
int Config_Cache_Arena::get_arena_max_force_lv(void){
	return max_force_level_;
}
