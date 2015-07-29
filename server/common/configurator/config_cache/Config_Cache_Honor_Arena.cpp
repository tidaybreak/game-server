/*
 * Config_Cache_Honor_Arena.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#include "Cache_Base.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_Honor_Arena.h"
#include "Config_Cache_Fighter.h"

bool Honor_Arena_Streak_Show_Greater(Int_Int value1, Int_Int value2){
	return value1.val_1 < value2.val_1;
}

void Config_Cache_Honor_Arena::refresh_config_cache(void){
	PERF_MON("Config_Cache_Honor_Arena");
	refresh_base();
	refresh_add();
	refresh_show();
	refresh_rank();
}

void Config_Cache_Honor_Arena::refresh_base(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["arena_multi"]["pk_base"];
	cfg_base_.reset();
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);

		if (config_json.isMember("base_score")){
			cfg_base_.base_score = config_json["base_score"].asInt();
		}
		if (config_json.isMember("succ_score")){
			cfg_base_.succ_score = config_json["succ_score"].asInt();
		}
		if (config_json.isMember("succ_merit")){
			cfg_base_.succ_merit = config_json["succ_merit"].asInt();
		}
		if (config_json.isMember("succ_gold")){
			cfg_base_.succ_gold = config_json["succ_gold"].asInt();
		}
		if (config_json.isMember("fail_score")){
			cfg_base_.fail_score = config_json["fail_score"].asInt();
		}
		if (config_json.isMember("fail_merit")){
			cfg_base_.fail_merit = config_json["fail_merit"].asInt();
		}
		if (config_json.isMember("fail_gold")){
			cfg_base_.fail_gold = config_json["fail_gold"].asInt();
		}

		if (config_json.isMember("score_match")){
			Json_Utility::assign_int_vec(config_json["score_match"], cfg_base_.score_match);
		}

		if(config_json.isMember("buff")){
			int count = 0;
			if(config_json["buff"].size() < 2){
				LOG_ABORT("gold npc error(buff)");
			}
			for (Json::Value::iterator iter = config_json["buff"].begin();
					iter != config_json["buff"].end(); ++iter) {
				if(count == 0){// id
					cfg_base_.buff_argv.status_id = (*iter).asInt();
					cfg_base_.buff_argv.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), cfg_base_.buff_argv.disappear_gap);
					}
				}else{// attr[530321,20], []
					if(!(*iter).isNull() && (*iter).isArray()){
						Skill_Effect_Val_Inner inner;
						int id = (*iter)[0u].asInt();
						double value = (*iter)[1u].asDouble();
						CONFIG_CACHE_FIGHTER->get_attr_change_data(id, inner, value);
						if (0 == inner.val1) { //伤害链\经验加成
							inner.val1 = id;
							inner.val4 = value;
						}
						if (3 == (*iter).size()) {
							double value2 = (*iter)[2u].asDouble();
							inner.val7 = value2;//目前使用作治疗的下限值
						}

						Effect_Argv_Prop prop;
						prop.i_effect_val1 = inner.val1;
						prop.d_effect_val1 = inner.val4;
						prop.d_effect_val2 = inner.val5;
						prop.d_effect_val3 = inner.val6;
						prop.d_effect_val4 = inner.val7;
						cfg_base_.buff_argv.data.push_back(prop);
					}
				}
				++count;
			}
		}
	}
}

void Config_Cache_Honor_Arena::refresh_add(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["arena_multi"]["pk_continuity"];
	Honor_Arena_Add_Config cache;

	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);

		cache.reset();

		if (config_json.isMember("continuity_succ")){
			cache.continuity_succ = config_json["continuity_succ"].asInt();
		}
		if (config_json.isMember("continuity_score")){
			cache.continuity_score = config_json["continuity_score"].asInt();
		}
		if (config_json.isMember("continuity_merit")){
			cache.continuity_merit = config_json["continuity_merit"].asInt();
		}
		if (config_json.isMember("kill_continuity_score")){
			cache.kill_continuity_score = config_json["kill_continuity_score"].asInt();
		}
		if (config_json.isMember("kill_continuity_merit")){
			cache.kill_continuity_merit = config_json["kill_continuity_merit"].asInt();
		}

		set_map_second_value_by_key(cache.continuity_succ, cfg_add_map_, cache);
	}
}

void Config_Cache_Honor_Arena::refresh_show(void){
	cfg_streak_show_.clear();
	const Json::Value &config = CONFIG_INSTANCE->config_json()["arena_multi"]["pk_show"];
	Honor_Arena_Show_Config cache;

	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);

		cache.reset();

		if (config_json.isMember("continuity")){
			cache.continuity = config_json["continuity"].asInt();
		}

		if (config_json.isMember("continuity_award")){
			Json_Utility::assign_int_int_vec(config_json["continuity_award"], cache.continuity_award);
		}

		set_map_second_value_by_key(cache.continuity, cfg_show_map_, cache);
		if(cache.continuity != 1000 && cache.continuity != 1001){
			Int_Int streak_value;
			streak_value.val_1 = cache.continuity;
			streak_value.val_2 = 0;
			cfg_streak_show_.push_back(streak_value);
		}

	}
	std::sort(cfg_streak_show_.begin(), cfg_streak_show_.end(), Honor_Arena_Streak_Show_Greater);
}

void Config_Cache_Honor_Arena::refresh_rank(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["arena_multi"]["pk_rank"];
	Honor_Arena_Rank_Config cache;

	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);

		cache.reset();

		if (config_json.isMember("rank")){
			if(config_json["rank"].size() == 2){
				cache.rank.val_1 = config_json["rank"][0u].asInt();
				cache.rank.val_2 = config_json["rank"][1u].asInt();
			}else{
				LOG_ABORT("honor arena pk_rank config error(rank)");
			}

		}

		if (config_json.isMember("award")){
			Json_Utility::assign_int_int_vec(config_json["award"], cache.award);
		}

		set_map_second_value_by_key(cache.rank.val_1, cfg_rank_map_, cache);
	}
}

const Honor_Arena_Show_Config* Config_Cache_Honor_Arena::get_show_config(const int type){
	Honor_Arena_Show_Config_Map::iterator it =  cfg_show_map_.find(type);
	if(it != cfg_show_map_.end()){
		return &(it->second);
	}
	return NULL;
}

const Honor_Arena_Add_Config* Config_Cache_Honor_Arena::get_add_config(const int type){
	Honor_Arena_Add_Config_Map::iterator it =  cfg_add_map_.find(type);
	if(it != cfg_add_map_.end()){
		return &(it->second);
	}
	return NULL;
}

const Honor_Arena_Base_Config* Config_Cache_Honor_Arena::get_base_config(void){
	return &cfg_base_;
}

void Config_Cache_Honor_Arena::get_fight_lose_reward(int& score, int& copper, int& merit){
	score = cfg_base_.fail_score;
	copper = cfg_base_.fail_gold;
	merit = cfg_base_.fail_merit;
}

void Config_Cache_Honor_Arena::get_fight_win_reward(int& score, int& copper, int& merit){
	score = cfg_base_.succ_score;
	copper = cfg_base_.succ_gold;
	merit = cfg_base_.succ_merit;
}

void Config_Cache_Honor_Arena::get_streak_win_reward(const int num, int& score, int& merit){
	Honor_Arena_Add_Config_Map::iterator it = cfg_add_map_.find(num);
	if(it != cfg_add_map_.end()){
		score += it->second.continuity_score;
		merit += it->second.continuity_merit;
	}
}

void Config_Cache_Honor_Arena::get_finish_streak_win_reward(const int num, int& score, int& merit){
	Honor_Arena_Add_Config_Map::iterator it = cfg_add_map_.find(num);
	if(it != cfg_add_map_.end()){
		score += it->second.continuity_score;
		merit += it->second.continuity_merit;
	}
}

Int_Int_Vec& Config_Cache_Honor_Arena::get_streak_show(void){
	return cfg_streak_show_;
}

Int_Int_Vec Config_Cache_Honor_Arena::get_rank_reward(const int rank){
	Honor_Arena_Rank_Config_Map::iterator it;
	for(it = cfg_rank_map_.begin(); it != cfg_rank_map_.end(); ++it){
		if(rank >= it->second.rank.val_1 && rank <= it->second.rank.val_2){
			return it->second.award;
		}
	}
	Int_Int_Vec vec;
	vec.clear();
	return vec;
}

int Config_Cache_Honor_Arena::get_match_score_spread(const int match_lv){
	int count = cfg_base_.score_match.size();
	if(count <= 0 || match_lv < 0){
		return 100;
	}
	if(match_lv >= count){
		return cfg_base_.score_match[count-1];
	}
	return cfg_base_.score_match[match_lv];
}

int Config_Cache_Honor_Arena::get_enter_lv_limit(void){
	return 0;
}

int Config_Cache_Honor_Arena::get_base_score(void){
	return cfg_base_.base_score;
}


