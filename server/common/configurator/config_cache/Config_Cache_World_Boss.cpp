/*
 * Config_Cache_World_Boss.cpp
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#include "Cache_Base.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_World_Boss.h"
#include "Config_Cache_Fighter.h"

void Config_Cache_World_Boss::refresh_config_cache(void){
	PERF_MON("Config_Cache_World_Boss");
	refresh_base();
	refresh_level();
	refresh_reward();

	refresh_base_gang();
	refresh_level_gang();
	refresh_reward_gang();
}

void Config_Cache_World_Boss::refresh_base(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["worldboss"]["world_boss_base"];
	cfg_base_.reset();
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		if (config_json.isMember("power_up_num")){
			Json_Utility::assign_int_vec(config_json["power_up_num"], cfg_base_.power_up_num);
		}
		if (config_json.isMember("power_up_cost")){
			Json_Utility::assign_int_vec(config_json["power_up_cost"], cfg_base_.power_up_cost);
		}
		if (config_json.isMember("reborn_num")){
			Json_Utility::assign_int_vec(config_json["reborn_num"], cfg_base_.reborn_num);
		}
		if (config_json.isMember("reborn_cost")){
			Json_Utility::assign_int_vec(config_json["reborn_cost"], cfg_base_.reborn_cost);
		}
		if (config_json.isMember("gold_bless")){
			Json_Utility::assign_int_vec(config_json["gold_bless"], cfg_base_.gold_bless);
		}
		if (config_json.isMember("gold_bless_effect")){
			Json_Utility::assign_int_vec(config_json["gold_bless_effect"], cfg_base_.gold_bless_effect);
		}

		if (config_json.isMember("common_reborn_cost")){
			cfg_base_.common_reborn_cost = config_json["common_reborn_cost"].asInt();
		}
		if (config_json.isMember("common_reborn_num")){
			cfg_base_.common_reborn_num = config_json["common_reborn_num"].asInt();
		}
		if (config_json.isMember("leader_hurt")){
			cfg_base_.leader_hurt = config_json["leader_hurt"].asInt();
		}
		if (config_json.isMember("reborn_time")){
			cfg_base_.reborn_time = config_json["reborn_time"].asInt();
		}
		if (config_json.isMember("reborn_time_fix_max")){
			cfg_base_.reborn_time_fix_max = config_json["reborn_time_fix_max"].asInt();
		}
		if (config_json.isMember("reborn_time_fix")){
			cfg_base_.reborn_time_fix = config_json["reborn_time_fix"].asInt();
		}

		if (config_json.isMember("powerup_costtype")){
			cfg_base_.powerup_costtype = config_json["powerup_costtype"].asInt();
		}
		if (config_json.isMember("reborn_costtype")){
			cfg_base_.reborn_costtype = config_json["reborn_costtype"].asInt();
		}
		if (config_json.isMember("gold_bless_costtype")){
			cfg_base_.gold_bless_costtype = config_json["gold_bless_costtype"].asInt();
		}
		if (config_json.isMember("boss_open_tiem")){
			cfg_base_.boss_open_tiem = config_json["boss_open_tiem"].asInt();
		}
		if(config_json.isMember("kill_time")){
			cfg_base_.rapid_kill_time = config_json["kill_time"].asInt();
		}
		//
		cfg_base_.gold_bless_num = cfg_base_.gold_bless.size();
		if(config_json.isMember("power_up_buff")){
			int count = 0;
			if(config_json["power_up_buff"].size() < 2){
				LOG_ABORT("world_boss_base error(power_up_buff)");
			}
			for (Json::Value::iterator iter = config_json["power_up_buff"].begin();
					iter != config_json["power_up_buff"].end(); ++iter) {
				if(count == 0){// id
					cfg_base_.power_up_buff.status_id = (*iter).asInt();
					cfg_base_.power_up_buff.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), cfg_base_.power_up_buff.disappear_gap);
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
						cfg_base_.power_up_buff.data.push_back(prop);
					}
				}
				++count;
			}
		}

		if(config_json.isMember("leader_buff")){
			int count = 0;
			if(config_json["leader_buff"].size() < 2){
				LOG_ABORT("world_boss_base error(leader_buff)");
			}
			for (Json::Value::iterator iter = config_json["leader_buff"].begin();
					iter != config_json["leader_buff"].end(); ++iter) {
				if(count == 0){// id
					cfg_base_.leader_buff.status_id = (*iter).asInt();
					cfg_base_.leader_buff.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), cfg_base_.leader_buff.disappear_gap);
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
						cfg_base_.leader_buff.data.push_back(prop);
					}
				}
				++count;
			}
		}
		//
		if(config_json.isMember("boss_weak_buff1")){
			int count = 0;
			if(config_json["boss_weak_buff1"].size() < 2){
				LOG_ABORT("world_boss_base error(boss_weak_buff1)");
			}
			for (Json::Value::iterator iter = config_json["boss_weak_buff1"].begin();
					iter != config_json["boss_weak_buff1"].end(); ++iter) {
				if(count == 0){// id
					cfg_base_.boss_weak_buff1.status_id = (*iter).asInt();
					cfg_base_.boss_weak_buff1.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), cfg_base_.boss_weak_buff1.disappear_gap);
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
						cfg_base_.boss_weak_buff1.data.push_back(prop);
					}
				}
				++count;
			}
		}
		//
		if(config_json.isMember("boss_weak_buff2")){
			int count = 0;
			if(config_json["boss_weak_buff2"].size() < 2){
				LOG_ABORT("world_boss_base error(boss_weak_buff2)");
			}
			for (Json::Value::iterator iter = config_json["boss_weak_buff2"].begin();
					iter != config_json["boss_weak_buff2"].end(); ++iter) {
				if(count == 0){// id
					cfg_base_.boss_weak_buff2.status_id = (*iter).asInt();
					cfg_base_.boss_weak_buff2.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), cfg_base_.boss_weak_buff2.disappear_gap);
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
						cfg_base_.boss_weak_buff2.data.push_back(prop);
					}
				}
				++count;
			}
		}
		if (config_json.isMember("boss_weak_hp")){
			Json_Utility::assign_int_vec(config_json["boss_weak_hp"], cfg_base_.boss_weak_hp);
		}
		if (config_json.isMember("open_time")){
			Json_Utility::assign_int_int_vec(config_json["open_time"], cfg_base_.open_time);
		}
	}
}

void Config_Cache_World_Boss::refresh_level(void){
	int max_lv = 0;
	const Json::Value &config = CONFIG_INSTANCE->config_json()["worldboss"]["world_boss_level"];
	World_Boss_Level_Config cache;
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("boss_level")){
			cache.boss_level = config_json["boss_level"].asInt();
		}
		if (config_json.isMember("boss_kill")){
			cache.boss_kill = config_json["boss_kill"].asInt();
		}
		if (config_json.isMember("monster_id")){
			cache.monster_id = config_json["monster_id"].asInt();
		}
		if (config_json.isMember("rank_reward_money")){
			cache.rank_reward_money = config_json["rank_reward_money"].asInt();
		}
		if (config_json.isMember("hurt_reward")){
			cache.hurt_reward = config_json["hurt_reward"].asInt();
		}

		if (config_json.isMember("drop_box")){
			Json_Utility::assign_int_vec(config_json["drop_box"], cache.drop_box);
		}
		if (config_json.isMember("low_hero_item")){
			Json_Utility::assign_int_int_vec(config_json["low_hero_item"], cache.low_hero_item);
		}
		if (config_json.isMember("mid_hero_item")){
			Json_Utility::assign_int_int_vec(config_json["mid_hero_item"], cache.mid_hero_item);
		}
		if (config_json.isMember("high_hero_item")){
			Json_Utility::assign_int_int_vec(config_json["high_hero_item"], cache.high_hero_item);
		}
		if (config_json.isMember("first_blood_reward")){
			Json_Utility::assign_int_int_vec(config_json["first_blood_reward"], cache.first_blood_reward);
		}
		if (config_json.isMember("first_kill_reward")){
			Json_Utility::assign_int_int_vec(config_json["first_kill_reward"], cache.first_kill_reward);
		}
		if (config_json.isMember("low_hero_fix")){
			cache.low_hero_fix = config_json["low_hero_fix"].asInt();
		}
		if (config_json.isMember("mid_hero_fix")){
			cache.mid_hero_fix = config_json["mid_hero_fix"].asInt();
		}
		if (config_json.isMember("high_hero_fix")){
			cache.high_hero_fix = config_json["high_hero_fix"].asInt();
		}
		set_map_second_value_by_key(cache.boss_level, cfg_level_map_, cache);
		if(max_lv < cache.boss_level){
			max_lv = cache.boss_level;
		}
	}
	set_map_second_value_by_key(10000, cfg_level_map_, cache);
	cfg_base_.boss_max_lv = max_lv;
}

void Config_Cache_World_Boss::refresh_reward(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["worldboss"]["world_boss_reward"];
	cfg_reward_map_.clear();
	int reward_id = 0;
	World_Boss_Reward_Config cache;
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		reward_id += 1;
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("level_group")){
			Json_Utility::assign_int_int(config_json["level_group"], cache.level_group);
		}
		if (config_json.isMember("ranb_high")){
			cache.ranb_high = config_json["ranb_high"].asInt();
		}
		if (config_json.isMember("rank_low")){
			cache.rank_low = config_json["rank_low"].asInt();
		}

		if (config_json.isMember("reward_rank_base_money")){
			Json_Utility::assign_int_int_vec(config_json["reward_rank_base_money"], cache.reward_rank_base_money);
		}
		if (config_json.isMember("reward_rank_base_item")){
			Json_Utility::assign_int_int_vec(config_json["reward_rank_base_item"], cache.reward_rank_base_item);
		}
		set_map_second_value_by_key(reward_id, cfg_reward_map_, cache);
	}
}


void Config_Cache_World_Boss::refresh_base_gang(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["worldboss"]["guild_boss_base"];
	gang_cfg_base_.reset();
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		const Json::Value &config_json = (*it);
		if (config_json.isMember("power_up_num")){
			Json_Utility::assign_int_vec(config_json["power_up_num"], gang_cfg_base_.power_up_num);
		}
		if (config_json.isMember("power_up_cost")){
			Json_Utility::assign_int_vec(config_json["power_up_cost"], gang_cfg_base_.power_up_cost);
		}
		if (config_json.isMember("reborn_num")){
			Json_Utility::assign_int_vec(config_json["reborn_num"], gang_cfg_base_.reborn_num);
		}
		if (config_json.isMember("reborn_cost")){
			Json_Utility::assign_int_vec(config_json["reborn_cost"], gang_cfg_base_.reborn_cost);
		}
		if (config_json.isMember("gold_bless")){
			Json_Utility::assign_int_vec(config_json["gold_bless"], gang_cfg_base_.gold_bless);
		}
		if (config_json.isMember("gold_bless_effect")){
			Json_Utility::assign_int_vec(config_json["gold_bless_effect"], gang_cfg_base_.gold_bless_effect);
		}

		if (config_json.isMember("common_reborn_cost")){
			gang_cfg_base_.common_reborn_cost = config_json["common_reborn_cost"].asInt();
		}
		if (config_json.isMember("common_reborn_num")){
			gang_cfg_base_.common_reborn_num = config_json["common_reborn_num"].asInt();
		}
		if (config_json.isMember("leader_hurt")){
			gang_cfg_base_.leader_hurt = config_json["leader_hurt"].asInt();
		}
		if (config_json.isMember("reborn_time")){
			gang_cfg_base_.reborn_time = config_json["reborn_time"].asInt();
		}
		if (config_json.isMember("reborn_time_fix_max")){
			gang_cfg_base_.reborn_time_fix_max = config_json["reborn_time_fix_max"].asInt();
		}
		if (config_json.isMember("reborn_time_fix")){
			gang_cfg_base_.reborn_time_fix = config_json["reborn_time_fix"].asInt();
		}

		if (config_json.isMember("powerup_costtype")){
			gang_cfg_base_.powerup_costtype = config_json["powerup_costtype"].asInt();
		}
		if (config_json.isMember("reborn_costtype")){
			gang_cfg_base_.reborn_costtype = config_json["reborn_costtype"].asInt();
		}
		if (config_json.isMember("gold_bless_costtype")){
			gang_cfg_base_.gold_bless_costtype = config_json["gold_bless_costtype"].asInt();
		}
		if (config_json.isMember("boss_open_tiem")){
			gang_cfg_base_.boss_open_tiem = config_json["boss_open_tiem"].asInt();
		}
		if(config_json.isMember("kill_time")){
			gang_cfg_base_.rapid_kill_time = config_json["kill_time"].asInt();
		}
		//
		gang_cfg_base_.gold_bless_num = gang_cfg_base_.gold_bless.size();
		if(config_json.isMember("power_up_buff")){
			int count = 0;
			if(config_json["power_up_buff"].size() < 2){
				LOG_ABORT("world_boss_base error(power_up_buff)");
			}
			for (Json::Value::iterator iter = config_json["power_up_buff"].begin();
					iter != config_json["power_up_buff"].end(); ++iter) {
				if(count == 0){// id
					gang_cfg_base_.power_up_buff.status_id = (*iter).asInt();
					gang_cfg_base_.power_up_buff.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), gang_cfg_base_.power_up_buff.disappear_gap);
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
						gang_cfg_base_.power_up_buff.data.push_back(prop);
					}
				}
				++count;
			}
		}

		if(config_json.isMember("leader_buff")){
			int count = 0;
			if(config_json["leader_buff"].size() < 2){
				LOG_ABORT("world_boss_base error(leader_buff)");
			}
			for (Json::Value::iterator iter = config_json["leader_buff"].begin();
					iter != config_json["leader_buff"].end(); ++iter) {
				if(count == 0){// id
					gang_cfg_base_.leader_buff.status_id = (*iter).asInt();
					gang_cfg_base_.leader_buff.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), gang_cfg_base_.leader_buff.disappear_gap);
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
						gang_cfg_base_.leader_buff.data.push_back(prop);
					}
				}
				++count;
			}
		}
		//
		if(config_json.isMember("boss_weak_buff1")){
			int count = 0;
			if(config_json["boss_weak_buff1"].size() < 2){
				LOG_ABORT("world_boss_base error(boss_weak_buff1)");
			}
			for (Json::Value::iterator iter = config_json["boss_weak_buff1"].begin();
					iter != config_json["boss_weak_buff1"].end(); ++iter) {
				if(count == 0){// id
					gang_cfg_base_.boss_weak_buff1.status_id = (*iter).asInt();
					gang_cfg_base_.boss_weak_buff1.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), gang_cfg_base_.boss_weak_buff1.disappear_gap);
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
						gang_cfg_base_.boss_weak_buff1.data.push_back(prop);
					}
				}
				++count;
			}
		}
		//
		if(config_json.isMember("boss_weak_buff2")){
			int count = 0;
			if(config_json["boss_weak_buff2"].size() < 2){
				LOG_ABORT("world_boss_base error(boss_weak_buff2)");
			}
			for (Json::Value::iterator iter = config_json["boss_weak_buff2"].begin();
					iter != config_json["boss_weak_buff2"].end(); ++iter) {
				if(count == 0){// id
					gang_cfg_base_.boss_weak_buff2.status_id = (*iter).asInt();
					gang_cfg_base_.boss_weak_buff2.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), gang_cfg_base_.boss_weak_buff2.disappear_gap);
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
						gang_cfg_base_.boss_weak_buff2.data.push_back(prop);
					}
				}
				++count;
			}
		}
		if (config_json.isMember("boss_weak_hp")){
			Json_Utility::assign_int_vec(config_json["boss_weak_hp"], gang_cfg_base_.boss_weak_hp);
		}
		if (config_json.isMember("open_time")){
			Json_Utility::assign_int_int_vec(config_json["open_time"], gang_cfg_base_.open_time);
		}
	}
}

void Config_Cache_World_Boss::refresh_level_gang(void){
	int max_lv = 0;
	const Json::Value &config = CONFIG_INSTANCE->config_json()["worldboss"]["guild_boss_level"];
	World_Boss_Level_Config cache;
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("boss_level")){
			cache.boss_level = config_json["boss_level"].asInt();
		}
		if (config_json.isMember("boss_kill")){
			cache.boss_kill = config_json["boss_kill"].asInt();
		}
		if (config_json.isMember("monster_id")){
			cache.monster_id = config_json["monster_id"].asInt();
		}
		if (config_json.isMember("rank_reward_money")){
			cache.rank_reward_money = config_json["rank_reward_money"].asInt();
		}
		if (config_json.isMember("hurt_reward")){
			cache.hurt_reward = config_json["hurt_reward"].asInt();
		}

		if (config_json.isMember("drop_box")){
			Json_Utility::assign_int_vec(config_json["drop_box"], cache.drop_box);
		}
		if (config_json.isMember("low_hero_item")){
			Json_Utility::assign_int_int_vec(config_json["low_hero_item"], cache.low_hero_item);
		}
		if (config_json.isMember("mid_hero_item")){
			Json_Utility::assign_int_int_vec(config_json["mid_hero_item"], cache.mid_hero_item);
		}
		if (config_json.isMember("high_hero_item")){
			Json_Utility::assign_int_int_vec(config_json["high_hero_item"], cache.high_hero_item);
		}
		if (config_json.isMember("first_blood_reward")){
			Json_Utility::assign_int_int_vec(config_json["first_blood_reward"], cache.first_blood_reward);
		}
		if (config_json.isMember("first_kill_reward")){
			Json_Utility::assign_int_int_vec(config_json["first_kill_reward"], cache.first_kill_reward);
		}
		if (config_json.isMember("low_hero_fix")){
			cache.low_hero_fix = config_json["low_hero_fix"].asInt();
		}
		if (config_json.isMember("mid_hero_fix")){
			cache.mid_hero_fix = config_json["mid_hero_fix"].asInt();
		}
		if (config_json.isMember("high_hero_fix")){
			cache.high_hero_fix = config_json["high_hero_fix"].asInt();
		}
		set_map_second_value_by_key(cache.boss_level, gang_cfg_level_map_, cache);
		if(max_lv < cache.boss_level){
			max_lv = cache.boss_level;
		}
	}
	set_map_second_value_by_key(10000, gang_cfg_level_map_, cache);
	gang_cfg_base_.boss_max_lv = max_lv;
}

void Config_Cache_World_Boss::refresh_reward_gang(void){
	const Json::Value &config = CONFIG_INSTANCE->config_json()["worldboss"]["guild_boss_reward"];
	gang_cfg_reward_map_.clear();
	int reward_id = 0;
	World_Boss_Reward_Config cache;
	for (Json::Value::iterator it = config.begin();it != config.end(); ++it){
		reward_id += 1;
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("level_group")){
			Json_Utility::assign_int_int(config_json["level_group"], cache.level_group);
		}
		if (config_json.isMember("ranb_high")){
			cache.ranb_high = config_json["ranb_high"].asInt();
		}
		if (config_json.isMember("rank_low")){
			cache.rank_low = config_json["rank_low"].asInt();
		}

		if (config_json.isMember("reward_rank_base_money")){
			Json_Utility::assign_int_int_vec(config_json["reward_rank_base_money"], cache.reward_rank_base_money);
		}
		if (config_json.isMember("reward_rank_base_item")){
			Json_Utility::assign_int_int_vec(config_json["reward_rank_base_item"], cache.reward_rank_base_item);
		}
		set_map_second_value_by_key(reward_id, gang_cfg_reward_map_, cache);
	}
}

const World_Boss_Level_Config* Config_Cache_World_Boss::get_level_config_from_lv(int lv, const int8_t icon_id/* = 16*/){
	World_Boss_Level_Config_Map* tg_cfg;
	switch(icon_id){
		case 16:{
			tg_cfg = &cfg_level_map_;
			break;
		}
		case 19:{
			tg_cfg = &gang_cfg_level_map_;
			break;
		}
		default:{
			tg_cfg = &cfg_level_map_;
			break;
		}
	}

	World_Boss_Level_Config_Map::const_iterator it = tg_cfg->find(lv);
	if(it != tg_cfg->end()){
		return &(it->second);
	}
	it = tg_cfg->find(10000);
	if(it != tg_cfg->end()){
		return &(it->second);
	}
	return NULL;
}

int Config_Cache_World_Boss::get_boss_lv_from_kill_num(const int kill_num, const int8_t icon_id){
	World_Boss_Level_Config_Map* tg_cfg;
	switch(icon_id){
		case 16:{
			tg_cfg = &cfg_level_map_;
			break;
		}
		case 19:{
			tg_cfg = &gang_cfg_level_map_;
			break;
		}
		default:{
			tg_cfg = &cfg_level_map_;
			break;
		}
	}
	int lv = 1;
	World_Boss_Level_Config_Map::const_iterator it;
	for(it = tg_cfg->begin(); it != tg_cfg->end(); ++it){
		if(kill_num >= it->second.boss_kill){
			if(lv < it->second.boss_level){
				lv = it->second.boss_level;
			}
		}
	}
	return lv;
}

int Config_Cache_World_Boss::get_boss_kill_num_from_lv(const int lv, const int8_t icon_id){
	World_Boss_Level_Config_Map* tg_cfg;
	switch(icon_id){
		case 16:{
			tg_cfg = &cfg_level_map_;
			break;
		}
		case 19:{
			tg_cfg = &gang_cfg_level_map_;
			break;
		}
		default:{
			tg_cfg = &cfg_level_map_;
			break;
		}
	}
	int max_lv = 0;
	int max_lv_kill_num = 0;
	int kill_num = 0;
	World_Boss_Level_Config_Map::const_iterator it;
	for(it = tg_cfg->begin(); it != tg_cfg->end(); ++it){
		if(lv == it->second.boss_level){
			kill_num = it->second.boss_kill;
			return kill_num;
		}
		if(max_lv < it->second.boss_level){
			max_lv = it->second.boss_level;
			max_lv_kill_num = it->second.boss_kill;
		}
	}
	return max_lv_kill_num;
}

int Config_Cache_World_Boss::get_boss_rapid_kill_time(const int8_t icon_id){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 60;
	}
	return tg_cfg->rapid_kill_time;
}

int Config_Cache_World_Boss::get_boss_max_lv(const int8_t icon_id){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 1;
	}
	return tg_cfg->boss_max_lv;
}

const World_Boss_Reward_Config* Config_Cache_World_Boss::get_reward_config(int boss_lv, int rank, const int8_t icon_id/* = 16*/){
	World_Boss_Reward_Config_Map* tg_cfg;
	switch(icon_id){
		case 16:{
			tg_cfg = &cfg_reward_map_;
			break;
		}
		case 19:{
			tg_cfg = &gang_cfg_reward_map_;
			break;
		}
		default:{
			tg_cfg = &cfg_reward_map_;
			break;
		}
	}
	World_Boss_Reward_Config_Map::const_iterator it;
	for(it = tg_cfg->begin(); it != tg_cfg->end(); ++it){
		if(boss_lv >= it->second.level_group.val_1 && boss_lv <= it->second.level_group.val_2){
			if(rank >= it->second.ranb_high && rank <= it->second.rank_low){
				return &(it->second);
			}
		}
	}
	return NULL;
}

World_Boss_Base_Config* Config_Cache_World_Boss::get_base_config(const int8_t icon_id/* = 16*/){
	switch(icon_id){
		case 16:{
			return &cfg_base_;
			break;
		}
		case 19:{
			return &gang_cfg_base_;
			break;
		}
		default:{
			return &cfg_base_;
			break;
		}
	}
	return &cfg_base_;
}

int Config_Cache_World_Boss::cal_active_time(const int cur_time, int& valid_status, int& valid_time, int& valid_last_time, bool& next_day){
	int count = cfg_base_.open_time.size();
	if(count <= 0){
		return -1;
	}
	for(int i = 0; i < count; ++i){
		int cfg_time = cfg_base_.open_time[i].val_1;
		int b_time  = cfg_time/100*Time_Value::ONE_HOUR_IN_SECS+cfg_time%100*Time_Value::ONE_MINUTE_IN_SECS;
		int f_time = b_time + cfg_base_.open_time[i].val_2;
		if(cur_time  >= f_time){
			continue;
		}
		next_day = false;
		if(cur_time >= b_time){
			valid_status = 3;
		}else if(cur_time >= (b_time - 5*60)){
			valid_status = 2;
		}else{
			valid_status = 1;
		}
		valid_time = b_time;
		valid_last_time = cfg_base_.open_time[i].val_2;
		return 0;
	}
	next_day = true;
	valid_status = 1;
	int cfg_time = cfg_base_.open_time[0].val_1;
	valid_time  = cfg_time/100*Time_Value::ONE_HOUR_IN_SECS+cfg_time%100*Time_Value::ONE_MINUTE_IN_SECS;
	valid_last_time = cfg_base_.open_time[0].val_2;
	return 0;
}

int Config_Cache_World_Boss::get_ability0_num(const int vip_lv, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->common_reborn_num;
}

int Config_Cache_World_Boss::get_ability1_num(const int vip_lv, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	int count = tg_cfg->reborn_num.size();
	if(vip_lv < 0 || count <= 0){
		return 1;
	}
	if(vip_lv >= count){
		return tg_cfg->reborn_num[count-1];
	}
	return tg_cfg->reborn_num[vip_lv];
}

int Config_Cache_World_Boss::get_ability2_num(const int vip_lv, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	int count = tg_cfg->power_up_num.size();
	if(vip_lv < 0 || count <= 0){
		return 1;
	}
	if(vip_lv >= count){
		return tg_cfg->power_up_num[count-1];
	}
	return tg_cfg->power_up_num[vip_lv];
}


int Config_Cache_World_Boss::get_ability3_num(const int vip_lv, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->gold_bless_num;
}

int Config_Cache_World_Boss::get_ability0_cost(const int use_num, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->common_reborn_cost;
}

int Config_Cache_World_Boss::get_ability1_cost(const int use_num, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	int count = tg_cfg->reborn_cost.size();
	if(use_num < 0 || count <= 0){
		return 0;
	}
	if(use_num >= count){
		return tg_cfg->reborn_cost[count-1];
	}
	return tg_cfg->reborn_cost[use_num];
}

int Config_Cache_World_Boss::get_ability2_cost(const int use_num, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	int count = tg_cfg->power_up_cost.size();
	if(use_num < 0 || count <= 0){
		return 0;
	}
	if(use_num >= count){
		return tg_cfg->power_up_cost[count-1];
	}
	return tg_cfg->power_up_cost[use_num];
}

int Config_Cache_World_Boss::get_ability3_cost(const int use_num, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	int count = tg_cfg->gold_bless.size();
	if(use_num < 0 || count <= 0){
		return 0;
	}
	if(use_num >= count){
		return tg_cfg->gold_bless[count-1];
	}
	return tg_cfg->gold_bless[use_num];
}

int Config_Cache_World_Boss::get_ability1_cost_type(const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->reborn_costtype;
}

int Config_Cache_World_Boss::get_ability2_cost_type(const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->powerup_costtype;
}

int Config_Cache_World_Boss::get_ability3_cost_type(const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->gold_bless_costtype;
}

int Config_Cache_World_Boss::achieve_1_coe(const int boss_lv, const int8_t icon_id/* = 16*/){
	const World_Boss_Level_Config* cfg_lv = get_level_config_from_lv(boss_lv, icon_id);
	if(cfg_lv){
		return cfg_lv->low_hero_fix;
	}
	return 0;

}

int Config_Cache_World_Boss::achieve_2_coe(const int boss_lv, const int8_t icon_id/* = 16*/){
	const World_Boss_Level_Config* cfg_lv = get_level_config_from_lv(boss_lv, icon_id);
	if(cfg_lv){
		return cfg_lv->mid_hero_fix;
	}
	return 0;
}

int Config_Cache_World_Boss::achieve_3_coe(const int boss_lv, const int8_t icon_id/* = 16*/){
	const World_Boss_Level_Config* cfg_lv = get_level_config_from_lv(boss_lv, icon_id);
	if(cfg_lv){
		return cfg_lv->high_hero_fix;
	}
	return 0;
}

int Config_Cache_World_Boss::get_ability3_coe(const int use_num, const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	int count = tg_cfg->gold_bless_effect.size();
	if(use_num < 0 || count <= 0){
		return 0;
	}
	if(use_num >= count){
		return tg_cfg->gold_bless_effect[count-1];
	}
	return tg_cfg->gold_bless_effect[use_num];
}

int Config_Cache_World_Boss::get_revive_time(const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->reborn_time;
}

double Config_Cache_World_Boss::leader_hurt_coe(const int8_t icon_id/* = 16*/){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->leader_hurt/100.0f;
}

int Config_Cache_World_Boss::get_hurt_reward_coe(const int boss_cfg_lv, const int8_t icon_id/* = 16*/){
	World_Boss_Level_Config_Map* tg_cfg;
	switch(icon_id){
		case 16:{
			tg_cfg = &cfg_level_map_;
			break;
		}
		case 19:{
			tg_cfg = &gang_cfg_level_map_;
			break;
		}
		default:{
			tg_cfg = &cfg_level_map_;
			break;
		}
	}
	World_Boss_Level_Config_Map::iterator it = tg_cfg->find(boss_cfg_lv);
	if(it != tg_cfg->end()){
		return it->second.hurt_reward;
	}
	return 1;
}

bool Config_Cache_World_Boss::get_boss_weak_check(const int8_t icon_id){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return false;
	}
	if(tg_cfg->boss_weak_hp.empty()){
		return false;
	}
	return true;
}

int Config_Cache_World_Boss::get_boss_weak_1(const int8_t icon_id){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 800;
	}
	if(tg_cfg->boss_weak_hp.size() < 1){
		return 800;
	}
	return tg_cfg->boss_weak_hp[0];
}

int Config_Cache_World_Boss::get_boss_weak_2(const int8_t icon_id){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 600;
	}
	if(tg_cfg->boss_weak_hp.size() < 2){
		return 600;
	}
	return tg_cfg->boss_weak_hp[1];
}

int Config_Cache_World_Boss::get_boss_weak_3(const int8_t icon_id){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 400;
	}
	if(tg_cfg->boss_weak_hp.size() < 3){
		return 400;
	}
	return tg_cfg->boss_weak_hp[2];
}

int Config_Cache_World_Boss::get_enter_lv_limit(const int8_t icon_id){
	World_Boss_Base_Config* tg_cfg = get_base_config(icon_id);
	if(!tg_cfg){
		return 0;
	}
	return tg_cfg->boss_open_tiem;
}
