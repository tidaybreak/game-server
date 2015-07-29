/*
 * Dungeon_Config.cpp
 *
 *  Created on: 2014-3-20
 *      Author: DonaldShaw
 */

#include "Config_Cache_Dungeon.h"
#include "pack/Pack_Struct.h"
#include "Configurator.h"
#include "Json_Utility.h"


void Config_Cache_Dungeon::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Dungeon");
	refresh_chapter_award_config();
	refresh_elite_copy_config();
	refresh_war_base_config();
	refresh_war_streak_kill_award_config();
	refresh_war_kill_num_award_config();
	refresh_war_mine_award_config();
	refresh_war_balance_config();
	refresh_war_random_coord_config();
	refresh_single_copy_vip_config();
	refresh_wipe_base();
	refresh_wipe_award();
	refresh_team_award();
}

void Config_Cache_Dungeon::refresh_single_copy_vip_config(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["copy_vip"];
	Single_Copy_Vip_Config cache;
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("vip_level")) {
			cache.vip_level = config_json["vip_level"].asInt();
		}
		if (config_json.isMember("rate")) {
			cache.rate = config_json["rate"].asInt();
		}
		set_map_second_value_by_key(cache.vip_level, single_copy_vip_config_map_, cache);
	}
}

void Config_Cache_Dungeon::refresh_chapter_award_config(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["copy"];
	Chapter_Award_Config cache;
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("chapter")) {
			cache.chapter = config_json["chapter"].asInt();
		}
		if (config_json.isMember("finish_scene")) {
			cache.finish_scene = config_json["finish_scene"].asInt();
		}

		if(config_json.isMember("drops")){
			Json_Utility::assign_int_int_vec(config_json["drops"], cache.award_vec, 1);
		}
		set_map_second_value_by_key(cache.chapter, single_dungeon_chapter_award_map_, cache);
	}
}

void Config_Cache_Dungeon::refresh_elite_copy_config(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["elite_copy"];
	elite_copy_config_.reset();
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		if (config_json.isMember("cost")){
			elite_copy_config_.reset_cost = config_json["cost"].asInt();
		}
		if (config_json.isMember("hard_lv")){
			elite_copy_config_.hor_lv_limit = config_json["hard_lv"].asInt();
		}
		if (config_json.isMember("vip_lv")){
			elite_copy_config_.reset_vip_lv = config_json["vip_lv"].asInt();
		}
	}
}

void Config_Cache_Dungeon::refresh_war_base_config(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["war_base"];
	war_base_config_.reset();
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		if (config_json.isMember("initial_score")){
			war_base_config_.initial_score = config_json["initial_score"].asInt();
		}
		if (config_json.isMember("fb_merit")){
			war_base_config_.fb_merit = config_json["fb_merit"].asInt();
		}
		if (config_json.isMember("fb_score_pct")){
			war_base_config_.fb_score_pct = config_json["fb_score_pct"].asDouble();
		}
		if (config_json.isMember("fb_score")){
			war_base_config_.fb_score = config_json["fb_score"].asInt();
		}
		if (config_json.isMember("bk_score_pct")){
			war_base_config_.bk_score_pct = config_json["bk_score_pct"].asDouble();
		}
		if (config_json.isMember("assists_merit")){
			war_base_config_.assists_merit = config_json["assists_merit"].asDouble();
		}
		if (config_json.isMember("heal_cost_score")){
			war_base_config_.heal_cost_score = config_json["heal_cost_score"].asDouble();
		}
		if (config_json.isMember("guard_score")){
			war_base_config_.guard_score = config_json["guard_score"].asDouble();
		}
		if (config_json.isMember("guard_merit")){
			war_base_config_.guard_merit = config_json["guard_merit"].asDouble();
		}
		if (config_json.isMember("officer_score")){
			war_base_config_.officer_score = config_json["officer_score"].asDouble();
		}
		if (config_json.isMember("officer_merit")){
			war_base_config_.officer_merit = config_json["officer_merit"].asDouble();
		}
		if (config_json.isMember("level_limit")){
			war_base_config_.level_limit = config_json["level_limit"].asDouble();
		}
		if (config_json.isMember("ready_time")){
			war_base_config_.ready_time = config_json["ready_time"].asDouble();
		}
		if (config_json.isMember("open_time")){
			Json_Utility::assign_int_int_vec(config_json["open_time"], war_base_config_.open_time);
		}
	}
}

void Config_Cache_Dungeon::refresh_war_streak_kill_award_config(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["war_streak_kill_award"];
	War_Streak_Kill_Award_Config cache;
	War_Streak_Kill_Award_Config default_cache;
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("streak_kill_num")) {
			cache.streak_kill_num = config_json["streak_kill_num"].asInt();
		}
		if (config_json.isMember("merit")) {
			cache.merit = config_json["merit"].asInt();
		}
		if (config_json.isMember("continuity_merit")) {
			cache.continuity_merit = config_json["continuity_merit"].asInt();
		}
		if (config_json.isMember("reputation")) {
			cache.rep = config_json["reputation"].asInt();
		}
		if (config_json.isMember("continuity_reputation")) {
			cache.continuity_rep = config_json["continuity_reputation"].asInt();
		}
		if(config_json.isMember("score_pct")){
			cache.score_pct = config_json["score_pct"].asDouble();
		}
		if (config_json.isMember("score")) {
			cache.score = config_json["score"].asInt();
		}
		if(cache.streak_kill_num >= default_cache.streak_kill_num){
			default_cache = cache;
		}
		set_map_second_value_by_key(cache.streak_kill_num, war_streak_kill_award_map_, cache);
	}
	default_cache.streak_kill_num = -1;
	set_map_second_value_by_key(default_cache.streak_kill_num, war_streak_kill_award_map_, default_cache);
}

void Config_Cache_Dungeon::refresh_war_kill_num_award_config(void){
	war_kill_num_award_map_.clear();
	War_Kill_Num_Award_Config cache;
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["reputation"];
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("total_kill_num")) {
			cache.kill_num = config_json["total_kill_num"].asInt();
		}
		if (config_json.isMember("award")) {
			Json_Utility::assign_id_int_int_vec(config_json["award"], cache.award);
		}
		set_map_second_value_by_key(cache.kill_num, war_kill_num_award_map_, cache);
	}
}
void Config_Cache_Dungeon::refresh_war_mine_award_config(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["war_mine_award"];
	war_mine_award_config_.reset();
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		int value;
		value = 0;
		if (config_json.isMember("nor_pieces")){
			value = config_json["nor_pieces"].asInt();
		}
		war_mine_award_config_.nor_pieces.push_back(value);
		value = 0;
		if (config_json.isMember("nor_merit")){
			value = config_json["nor_merit"].asInt();
		}
		war_mine_award_config_.nor_merit.push_back(value);
		value = 0;
		if (config_json.isMember("nor_score")){
			value = config_json["nor_score"].asInt();
		}
		war_mine_award_config_.nor_score.push_back(value);
		value = 0;
		if (config_json.isMember("reputation")){
			value = config_json["reputation"].asInt();
		}
		war_mine_award_config_.nor_rep.push_back(value);
		value = 0;
		if (config_json.isMember("nor_rate")){
			value = config_json["nor_rate"].asDouble()*1000;
		}
		war_mine_award_config_.nor_rate.push_back(value);

		value = 0;
		if (config_json.isMember("spe_pieces")){
			value = config_json["spe_pieces"].asInt();
		}
		war_mine_award_config_.spe_pieces.push_back(value);
		value = 0;
		if (config_json.isMember("spe_merit")){
			value = config_json["spe_merit"].asInt();
		}
		war_mine_award_config_.spe_merit.push_back(value);
		value = 0;
		if (config_json.isMember("spe_score")){
			value = config_json["spe_score"].asInt();
		}
		war_mine_award_config_.spe_score.push_back(value);
		value = 0;
		if (config_json.isMember("spe_reputation")){
			value = config_json["spe_reputation"].asInt();
		}
		war_mine_award_config_.spe_rep.push_back(value);
		value = 0;
		if (config_json.isMember("spe_rate")){
			value = config_json["spe_rate"].asDouble()*1000;
		}
		war_mine_award_config_.spe_rate.push_back(value);
	}
}

void Config_Cache_Dungeon::refresh_war_balance_config(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["battlefieldConfig"];
	war_balance_config_.reset();

	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		int type = 0;
		std::vector<Id_Int_Int> items;
		if (config_json.isMember("type")){
			type = config_json["type"].asInt();
		}
		if(config_json.isMember("items")){
			if(config_json["items"].isArray()){
				int array_size = config_json["items"].size();
				for (int i = 0; i < array_size; ++i) {
					Id_Int_Int item;
					item.id = config_json["items"][i][0u].asInt();
					item.val1 = config_json["items"][i][1u].asInt();
					item.val2 = config_json["items"][i][2u].asInt();
					items.push_back(item);
				}
			}
		}
		war_balance_config_.rank_merit.insert(std::make_pair(type, items));
	}
}

void Config_Cache_Dungeon::refresh_war_random_coord_config(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["war_random_coord"];
	war_random_coord_config_.reset();
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		int type = 0;
		Coord coord;
		if (config_json.isMember("type")){
			type = config_json["type"].asInt();
		}
		if (config_json.isMember("coord")){
			Json_Utility::assign_coord(config_json["coord"], coord);
			switch(type){
				case War_Random_Coord_Config::LIGHT_REVIVE_BALL_COORD:{
					war_random_coord_config_.light_revive_ball.push_back(coord);
					break;
				}
				case War_Random_Coord_Config::DARK_REVIVE_BALL_COORD:{
					war_random_coord_config_.dark_revive_ball.push_back(coord);
					break;
				}
				case War_Random_Coord_Config::LIGHT_MINE_COORD:{
					war_random_coord_config_.light_mine.push_back(coord);
					break;
				}
				case War_Random_Coord_Config::DARK_MINE_COORD:{
					war_random_coord_config_.dark_mine.push_back(coord);
					break;
				}
				default:{
					break;
				}
			}
		}
	}
}

void Config_Cache_Dungeon::refresh_wipe_base(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["dungeon"]["wipe_base"];
	wipe_base_.reset();
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		if (config_json.isMember("single_time")){
			wipe_base_.single_time = config_json["single_time"].asInt();
		}
		if (config_json.isMember("single_cost")){
			wipe_base_.single_cost = config_json["single_cost"].asInt();
		}
		if (config_json.isMember("single_level_limit")){
			wipe_base_.single_level_limit = config_json["single_level_limit"].asInt();
		}
		if (config_json.isMember("elite_time")){
			wipe_base_.elite_time = config_json["elite_time"].asInt();
		}
		if (config_json.isMember("elite_cost")){
			wipe_base_.elite_cost = config_json["elite_cost"].asInt();
		}
		if (config_json.isMember("elite_level_limit")){
			wipe_base_.elite_level_limit = config_json["elite_level_limit"].asInt();
		}
		if (config_json.isMember("nor_elite_wipe_id")){
			wipe_base_.nor_elite_wipe_id = config_json["nor_elite_wipe_id"].asInt();
		}
		if (config_json.isMember("hor_elite_wipe_id")){
			wipe_base_.hor_elite_wipe_id = config_json["hor_elite_wipe_id"].asInt();
		}
		if (config_json.isMember("single_wipe_no_cd_lv")){
			wipe_base_.single_wipe_no_cd_lv = config_json["single_wipe_no_cd_lv"].asInt();
		}
		if (config_json.isMember("elite_cost_item")){
			wipe_base_.elite_cost_item = config_json["elite_cost_item"].asInt();
		}
		if (config_json.isMember("elite_cost_money")){
			wipe_base_.elite_cost_money = config_json["elite_cost_money"].asInt();
		}
		if (config_json.isMember("elite_cost_type")){
			wipe_base_.elite_cost_type = config_json["elite_cost_type"].asInt();
		}
		if (config_json.isMember("elite_base_num")){
			wipe_base_.elite_base_num = config_json["elite_base_num"].asInt();
		}
		if (config_json.isMember("elite_cost_num")){
			wipe_base_.elite_cost_num = config_json["elite_cost_num"].asInt();
		}
		if (config_json.isMember("control")){
			wipe_base_.single_ver = config_json["control"].asInt();
		}
		if (config_json.isMember("elite_control")){
			wipe_base_.elite_ver = config_json["elite_control"].asInt();
		}
		if (config_json.isMember("lord_control")){
			wipe_base_.lord_temple_ver = config_json["lord_control"].asInt();
		}
		if (config_json.isMember("hor_open_id")){
			wipe_base_.lord_open_id = config_json["hor_open_id"].asInt();
		}
	}
}
void Config_Cache_Dungeon::refresh_wipe_award(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["dungeon"]["wipe_award"];
	Wipe_Award_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it != info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("scene_id")) {
			cache.scene_id = config_json["scene_id"].asInt();
		}
		if (config_json.isMember("monsters")) {
			Json_Utility::assign_int_int_vec(config_json["monsters"], cache.monsters, 1);
		}
		if (config_json.isMember("box")) {
			Json_Utility::assign_int_int_vec(config_json["box"], cache.box, 1);
		}
		if (config_json.isMember("vip_box")) {
			Json_Utility::assign_int_int_vec(config_json["vip_box"], cache.vip_box, 1);
		}
		// first reward
		if (config_json.isMember("first_reward")) {
			for (Json::Value::iterator iter = config_json["first_reward"].begin();
					iter != config_json["first_reward"].end(); ++iter) {
				if((*iter).isArray() && (*iter).size() == 2){
					Int_Int cfg_item;
					cfg_item.val_2 = (*iter)[1u].asInt();;
					if((*iter)[0u].isArray()){
						if((*iter)[0u].size() != 3){
							LOG_ABORT("wipe_award error(first_reward): %d", cache.scene_id);
						}
						cfg_item.val_1 = (*iter)[0u][0u].asInt();
						cache.first_reward1.push_back(cfg_item);
						cfg_item.val_1 = (*iter)[0u][1u].asInt();
						cache.first_reward2.push_back(cfg_item);
						cfg_item.val_1 = (*iter)[0u][2u].asInt();
						cache.first_reward3.push_back(cfg_item);
					}else{
						cfg_item.val_1 = (*iter)[0u].asInt();
						cache.first_reward1.push_back(cfg_item);
						cache.first_reward2.push_back(cfg_item);
						cache.first_reward3.push_back(cfg_item);
					}
				}else{
					LOG_ABORT("wipe_award error(first_reward): %d", cache.scene_id);
				}
			}
		}

		//copy reward
		if (config_json.isMember("copy_reward")) {
			for (Json::Value::iterator iter = config_json["copy_reward"].begin();
					iter != config_json["copy_reward"].end(); ++iter) {
				if((*iter).isArray() && (*iter).size() == 2){
					Int_Int cfg_item;
					cfg_item.val_2 = (*iter)[1u].asInt();;
					if((*iter)[0u].isArray()){
						if((*iter)[0u].size() != 3){
							LOG_ABORT("wipe_award error(copy_reward): %d", cache.scene_id);
						}
						cfg_item.val_1 = (*iter)[0u][0u].asInt();
						cache.copy_reward1.push_back(cfg_item);
						cfg_item.val_1 = (*iter)[0u][1u].asInt();
						cache.copy_reward2.push_back(cfg_item);
						cfg_item.val_1 = (*iter)[0u][2u].asInt();
						cache.copy_reward3.push_back(cfg_item);
					}else{
						cfg_item.val_1 = (*iter)[0u].asInt();
						cache.copy_reward1.push_back(cfg_item);
						cache.copy_reward2.push_back(cfg_item);
						cache.copy_reward3.push_back(cfg_item);
					}
				}else{
					LOG_ABORT("wipe_award error(copy_reward): %d", cache.scene_id);
				}
			}
		}
		// card reward
		if (config_json.isMember("change_reward")) {
			for (Json::Value::iterator iter = config_json["change_reward"].begin();
					iter != config_json["change_reward"].end(); ++iter) {
				if((*iter).isArray() && (*iter).size() == 3){
					Single_Card_Config card_info1;
					Single_Card_Config card_info2;
					Single_Card_Config card_info3;
					// id
					if((*iter)[0u].isArray()){
						if((*iter)[0u].size() != 3){
							LOG_ABORT("wipe_award error(change_reward): %d", cache.scene_id);
						}
						card_info1.id = (*iter)[0u][0u].asInt();
						card_info2.id = (*iter)[0u][1u].asInt();
						card_info3.id = (*iter)[0u][2u].asInt();
					}else{
						int id = (*iter)[0u].asInt();
						card_info1.id = id;
						card_info2.id = id;
						card_info3.id = id;
					}
					// amount
					int amount = (*iter)[1u].asInt();
					card_info1.amount = amount;
					card_info2.amount = amount;
					card_info3.amount = amount;
					// rate
					int rate = (*iter)[2u].asInt();
					card_info1.rate = rate;
					card_info2.rate = rate;
					card_info3.rate = rate;
					cache.card_reward1.push_back(card_info1);
					cache.card_reward2.push_back(card_info2);
					cache.card_reward3.push_back(card_info3);
				}else{
					LOG_ABORT("wipe_award error(change_reward): %d", cache.scene_id);
				}
			}
		}

		set_map_second_value_by_key(cache.scene_id, wipe_award_map_, cache);
	}
}

void Config_Cache_Dungeon::refresh_team_award(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["dungeon"]["team_award"];
	Team_Award_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it != info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("scene_id")) {
			cache.scene_id = config_json["scene_id"].asInt();
		}

		if (config_json.isMember("exp")) {
			cache.exp = config_json["exp"].asInt();
		}
		if (config_json.isMember("gold")) {
			cache.copper = config_json["gold"].asInt();
		}
		if (config_json.isMember("soul")) {
			cache.soul = config_json["soul"].asInt();
		}
		if (config_json.isMember("friendship")) {
			cache.friendship = config_json["friendship"].asInt();
		}

		if (config_json.isMember("drop_type")) {
			cache.drop_type = config_json["drop_type"].asInt();
		}
		if (config_json.isMember("mult_drop_type")) {
			cache.mult_drop_type = config_json["mult_drop_type"].asInt();
		}
		if (config_json.isMember("drop_mult_coe")) {
			cache.drop_mult_coe = config_json["drop_mult_coe"].asDouble();
		}
		if (config_json.isMember("drop_lv_limit")) {
			cache.drop_lv_limit = config_json["drop_lv_limit"].asInt();
		}
		if (config_json.isMember("drop_lv_coe")) {
			cache.drop_lv_coe = config_json["drop_lv_coe"].asDouble();
		}
		if (config_json.isMember("res_mult_coe")) {
			cache.res_mult_coe = config_json["res_mult_coe"].asDouble();
		}
		if (config_json.isMember("res_lv_limit")) {
			cache.res_lv_limit = config_json["res_lv_limit"].asInt();
		}
		if (config_json.isMember("res_lv_coe")) {
			cache.res_lv_coe = config_json["res_lv_coe"].asInt();
		}
		if (config_json.isMember("drops")) {
			Json_Utility::assign_int_vec(config_json["drops"], cache.drops);
		}
		set_map_second_value_by_key(cache.scene_id, team_award_map_, cache);
	}
}

const Chapter_Award_Config* Config_Cache_Dungeon::find_chapter_award_config(int chapter){
	Chapter_Award_Map::iterator it = single_dungeon_chapter_award_map_.find(chapter);
	if(it != single_dungeon_chapter_award_map_.end()){
		return &(it->second);
	}
	return NULL;
}

int Config_Cache_Dungeon::get_single_dungeon_chapter_count(std::vector<int>& chapter_vec){
	Chapter_Award_Map::iterator it;
	for(it = single_dungeon_chapter_award_map_.begin();
			it != single_dungeon_chapter_award_map_.end(); ++it){
		chapter_vec.push_back(it->second.chapter);
	}
	return chapter_vec.size();
}

int Config_Cache_Dungeon::check_single_dungeon_chapter_finish(int scene_id){
	Chapter_Award_Map::iterator it;
	for(it = single_dungeon_chapter_award_map_.begin();
			it != single_dungeon_chapter_award_map_.end(); ++it){
		if(it->second.finish_scene == scene_id){
			return it->second.chapter;
		}
	}
	return 0;
}

int Config_Cache_Dungeon::get_single_copy_vip_rate(int level){
	Single_Copy_Vip_Config_Map::iterator it = single_copy_vip_config_map_.find(level);
	if( it != single_copy_vip_config_map_.end()){
		return it->second.rate;
	}
	return 0;
}

int Config_Cache_Dungeon::get_elite_reset_vip_lv(void){
	return elite_copy_config_.reset_vip_lv;
}

int Config_Cache_Dungeon::get_elite_reset_cost(void){
	return elite_copy_config_.reset_cost;
}

int Config_Cache_Dungeon::get_elite_hor_lv_limit(void){
	return elite_copy_config_.hor_lv_limit;
}

const War_Base_Config* Config_Cache_Dungeon::war_base_config(void){
	return &war_base_config_;
}

const War_Balance_Config* Config_Cache_Dungeon::war_balance_config(void){
	return &war_balance_config_;
}

const War_Random_Coord_Config* Config_Cache_Dungeon::war_random_coord_config(void){
	return &war_random_coord_config_;
}

void Config_Cache_Dungeon::get_kill_num_award(const int kill_num, int& rep){
	return;
}

void Config_Cache_Dungeon::get_streak_kill_award(int num, int end_num, int& merit, int& end_merit, int& rep, int& end_rep, double& score_pct, int& score){
	War_Streak_Kill_Award_Map::iterator it;
	it = war_streak_kill_award_map_.find(num);
	if(it != war_streak_kill_award_map_.end()){
		end_merit = it->second.continuity_merit;
		end_rep = it->second.continuity_rep;
		score_pct = it->second.score_pct;
		score = it->second.score;
	}else{
		it = war_streak_kill_award_map_.find(-1);
		if(it != war_streak_kill_award_map_.end()){
			end_merit = it->second.continuity_merit;
			end_rep = it->second.continuity_rep;
			score_pct = it->second.score_pct;
			score = it->second.score;
		}
	}

	it = war_streak_kill_award_map_.find(end_num);
	if(it != war_streak_kill_award_map_.end()){
		merit = it->second.merit;
		rep = it->second.rep;
		score_pct = it->second.score_pct;
		score = it->second.score;
	}else{
		it = war_streak_kill_award_map_.find(-1);
		if(it != war_streak_kill_award_map_.end()){
			merit = it->second.merit;
			rep = it->second.rep;
			score_pct = it->second.score_pct;
			score = it->second.score;
		}
	}
}

void Config_Cache_Dungeon::get_war_mine_award(int mine_type, int& pieces, int& merit, int& score, int& rep){
	int rand_rate = random()%1000;
	int total_rate = 0;
	if(mine_type == 1){
		int count = war_mine_award_config_.nor_rate.size();
		for(int i=0; i<count; ++i){
			total_rate += war_mine_award_config_.nor_rate[i];
			if(total_rate > rand_rate){
				pieces = war_mine_award_config_.nor_pieces[i];
				merit = war_mine_award_config_.nor_merit[i];
				score = war_mine_award_config_.nor_score[i];
				rep = war_mine_award_config_.nor_rep[i];
				return;
			}
		}
	}else{
		int count = war_mine_award_config_.spe_rate.size();
		for(int i=0; i<count; ++i){
			total_rate += war_mine_award_config_.spe_rate[i];
			if(total_rate > rand_rate){
				pieces = war_mine_award_config_.spe_pieces[i];
				merit = war_mine_award_config_.spe_merit[i];
				score = war_mine_award_config_.spe_score[i];
				rep = war_mine_award_config_.spe_rep[i];
				return;
			}
		}
	}

}

int  Config_Cache_Dungeon::get_war_level_limit(void){
	return war_base_config_.level_limit;
}

int Config_Cache_Dungeon::get_war_win_merit(void){
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(0);
	if(it != war_balance_config_.rank_merit.end()){
		int count = it->second.size();
		for(int i = 0; i < count; ++i){
			if(it->second[i].id == 503003){
				return it->second[i].val1;
			}
		}
	}
	return 0;
}

int Config_Cache_Dungeon::get_war_win_piece(void){
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(0);
	if(it != war_balance_config_.rank_merit.end()){
		int count = it->second.size();
		for(int i = 0; i < count; ++i){
			if(it->second[i].id == 20705401){
				return it->second[i].val1;
			}
		}
	}
	return 0;
}

int Config_Cache_Dungeon::get_war_win_coin(void){
	int coin_count = 0;
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(0);
	if(it != war_balance_config_.rank_merit.end()){
		int count = it->second.size();
		for(int i = 0; i < count; ++i){
			if(it->second[i].id == 21002401){
				coin_count += it->second[i].val1;
			}
		}
	}
	return coin_count;
}

void Config_Cache_Dungeon::get_war_win_prop(std::vector<Id_Int_Int>& props){
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(0);
	if(it != war_balance_config_.rank_merit.end()){
		std::vector<Id_Int_Int>::iterator s_it;
		for(s_it = it->second.begin(); s_it != it->second.end(); ++s_it){
			if((*s_it).id == 503003){
				continue;
			}else if((*s_it).val1 == 21002401){
				continue;
			}else if((*s_it).val1 == 20705401){
				continue;
			}
			else{
				props.push_back(*s_it);
			}
		}
	}
	return;
}

void Config_Cache_Dungeon::get_war_lose_prop(std::vector<Id_Int_Int>& props){
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(-1);
	if(it != war_balance_config_.rank_merit.end()){
		std::vector<Id_Int_Int>::iterator s_it;
		for(s_it = it->second.begin(); s_it != it->second.end(); ++s_it){
			if((*s_it).id == 503003){
				continue;
			}else if((*s_it).val1 == 21002401){
				continue;
			}else if((*s_it).val1 == 20705401){
				continue;
			}else{
				props.push_back(*s_it);
			}
		}
	}
	return;
}

void Config_Cache_Dungeon::get_war_result_ex_reward(const int result, const int kill_num, const int coe, std::vector<Item_Detail>& awards){
	if(coe <= 0){
		return;
	}
	int index = 0;
	if(result != 1){
		index = -1;
	}
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(index);
	if(it != war_balance_config_.rank_merit.end()){
		int count = it->second.size();
		for(int i = 0; i < count; ++i){
			Item_Detail item;
			item.id_ = it->second[i].id;
			item.amount_ = it->second[i].val1;
			item.bind_ = it->second[i].val2;
			awards.push_back(item);
		}
	}
	//kill award
	int award_index = -1;
	for(War_Kill_Num_Award_Map::iterator it = war_kill_num_award_map_.begin();
			it != war_kill_num_award_map_.end(); ++it){
		if(kill_num >= it->second.kill_num){
			if(it->second.kill_num > award_index){
				award_index = it->second.kill_num;
			}
		}
	}

	boost::unordered_map<int, int> tmp_items;
	War_Kill_Num_Award_Map::iterator award_it = war_kill_num_award_map_.find(award_index);
	if(award_it != war_kill_num_award_map_.end()){
		int count = award_it->second.award.size();
		for(int i = 0; i < count; ++i){
			tmp_items[award_it->second.award[i].id] += award_it->second.award[i].val1;
		}
	}

	for(boost::unordered_map<int, int>::iterator it = tmp_items.begin(); it != tmp_items.end(); ++it){
		Item_Detail item;
		item.id_ = it->first;
		item.amount_ = it->second;
		item.bind_ = 2;
		awards.push_back(item);
	}
}

int Config_Cache_Dungeon::get_war_lose_merit(void){
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(-1);
	if(it != war_balance_config_.rank_merit.end()){
		int count = it->second.size();
		for(int i = 0; i < count; ++i){
			if(it->second[i].id == 503003){
				return it->second[i].val1;
			}
		}
	}
	return 0;
}

int Config_Cache_Dungeon::get_war_lose_piece(void){
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(-1);
	if(it != war_balance_config_.rank_merit.end()){
		int count = it->second.size();
		for(int i = 0; i < count; ++i){
			if(it->second[i].id == 20705401){
				return it->second[i].val1;
			}
		}
	}
	return 0;
}

int Config_Cache_Dungeon::get_war_lose_coin(void){
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(0);
	if(it != war_balance_config_.rank_merit.end()){
		int count = it->second.size();
		for(int i = 0; i < count; ++i){
			if(it->second[i].id == 21002401){
				return it->second[i].val1;
			}
		}
	}
	return 0;
}

int Config_Cache_Dungeon::get_war_over_rank_merit(int rank, std::vector<Id_Int_Int>& props){
	int merit = 0;
	std::map<int, std::vector<Id_Int_Int>>::iterator it = war_balance_config_.rank_merit.find(rank);
	if(it != war_balance_config_.rank_merit.end()){
		std::vector<Id_Int_Int>::iterator s_it;
		for(s_it = it->second.begin(); s_it != it->second.end(); ++s_it){
			if((*s_it).id == 503003){
				merit += (*s_it).val1;
			}else if((*s_it).val1 == 21002401){
				continue;
			}
			else{
				props.push_back(*s_it);
			}
		}
	}
	return merit;
}


std::vector<Int_Int> Config_Cache_Dungeon::get_war_open_time(void){
	return war_base_config_.open_time;
}

const Wipe_Award_Config* Config_Cache_Dungeon::find_wipe_award_config(const int scene_id){
	Wipe_Award_Config_Map::const_iterator it = wipe_award_map_.find(scene_id);
	if( it != wipe_award_map_.end()){
		return &(it->second);
	}
	return NULL;
}


const Team_Award_Config* Config_Cache_Dungeon::find_team_award_config(const int scene_id){
	Team_Award_Config_Map::const_iterator it = team_award_map_.find(scene_id);
	if( it != team_award_map_.end()){
		return &(it->second);
	}
	return NULL;
}

const Wipe_Base_Config* Config_Cache_Dungeon::wipe_base_config(void){
	return &wipe_base_;
}

int Config_Cache_Dungeon::get_wipe_out_lv_limit(const int wipe_type){
	if(wipe_type == 1){
		return wipe_base_.single_level_limit;
	}else{
		return wipe_base_.elite_level_limit;
	}
	return 0;
}

int Config_Cache_Dungeon::get_single_wipe_no_cd_lv(void){
	return wipe_base_.single_wipe_no_cd_lv;
}

int Config_Cache_Dungeon::get_wipe_out_cost_time(const int wipe_type){
	if(wipe_type == 1){
		return wipe_base_.single_time;
	}else{
		return wipe_base_.elite_time;
	}
	return 0;
}

int Config_Cache_Dungeon::get_wipe_out_cost_gold(const int wipe_type){
	if(wipe_type == 1){
		return wipe_base_.single_cost;
	}else{
		return wipe_base_.elite_cost;
	}
	return 0;
}

bool Config_Cache_Dungeon::is_nor_elite_wipe_out_activate(const int scene_id){
	if(wipe_base_.nor_elite_wipe_id == 0 ||
			scene_id == wipe_base_.nor_elite_wipe_id){
		return true;
	}
	return false;
}

bool Config_Cache_Dungeon::is_hor_elite_wipe_out_activate(const int scene_id){
	if(wipe_base_.hor_elite_wipe_id == 0 ||
			scene_id == wipe_base_.hor_elite_wipe_id){
		return true;
	}
	return false;
}

int Config_Cache_Dungeon::get_elite_wipe_out_activate_id(const int diff){
	if(diff == 1){
		return wipe_base_.nor_elite_wipe_id;
	}else{
		return wipe_base_.hor_elite_wipe_id;
	}
}

int Config_Cache_Dungeon::get_elite_add_num_prop_id(void){
	return wipe_base_.elite_cost_item;
}

int Config_Cache_Dungeon::get_elite_add_num_cost(void){
	return wipe_base_.elite_cost_money;
}

int Config_Cache_Dungeon::Config_Cache_Dungeon::get_elite_ch_num(void){
	return wipe_base_.elite_base_num;
}

int Config_Cache_Dungeon::Config_Cache_Dungeon::get_elite_add_num(void){
	return wipe_base_.elite_cost_num;
}

int Config_Cache_Dungeon::get_elite_add_num_type(void){
	return wipe_base_.elite_cost_type;
}

int Config_Cache_Dungeon::get_single_ver(void){
	return wipe_base_.single_ver;
}

int Config_Cache_Dungeon::get_elite_ver(void){
	return wipe_base_.elite_ver;
}

int Config_Cache_Dungeon::get_lord_temple_ver(void){
	return wipe_base_.lord_temple_ver;
}

int Config_Cache_Dungeon::get_lord_open_id(void){
	return wipe_base_.lord_open_id;
}
