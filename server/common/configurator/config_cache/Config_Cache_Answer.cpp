/*
 * Answer_Config.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Answer.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "answer/Answer_Config.h"
#include "answer/Answer_Def.h"
#include "fighter/Fighter_Def.h"

const uint32_t Config_Cache_Answer::get_common_cfg_by_key(uint32_t key) {
	UInt_UInt_Map::const_iterator it = common_cfg.find(key);
	if(it != common_cfg.end()) {
		return it->second;
	}
	return 0;
}

const Answer_Question_Config_Map &Config_Cache_Answer::get_question(void) {
	return question;
}

const Answer_Rank_Reward_Config_List &Config_Cache_Answer::get_rank_reward_list(void) {
	return rank_reward;
}

const Answer_Question_Config* Config_Cache_Answer::get_question_byid(uint32_t id) {
	return get_map_second_pointer_by_key(id, question);
}

const Answer_Integral_Reward_Config* Config_Cache_Answer::get_integral_reward_by_level(uint32_t level) {
	return get_map_second_pointer_by_key(level, integral_reward);
}

const double Config_Cache_Answer::get_modulus_by_level(uint32_t level) {
	return get_value_by_key(level, level_modulus);
}

const double Config_Cache_Answer::get_modulus_item_by_level(uint32_t level) {
	return get_value_by_key(level, level_item_modulus);
}

void Config_Cache_Answer::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Answer");

	refresh_common_config_cache();//first
	refresh_question_config_cache();
	refresh_integral_reward_config_cache();
	refresh_rank_reward_config_cache();
	refresh_level_modulus_config_cache();

}

void Config_Cache_Answer::refresh_question_config_cache(void) {
	question.clear();
	const uint32_t question_num = get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_QUESTION_NUM);
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["answer"]["question"];
	if (config_json == Json::Value::null || config_json.size() < question_num  || question_num == 0) {
		LOG_ABORT("题库数量少于活动问题数");
	}
	Answer_Question_Config base_set;
	for (uint i = 0; i < config_json.size(); ++i) {
		base_set.reset();
		const Json::Value& json = config_json[i];
		base_set.id = json["id"].asInt();
		if(json["answer_a"].isInt()) {
			int a = json["answer_a"].asInt();
			std::stringstream str;
			str << a;
			base_set.answer_a = str.str();
		} else {
			base_set.answer_a = json["answer_a"].asString();
		}
		if(json["answer_b"].isInt()) {
			int b = json["answer_b"].asInt();
			std::stringstream str;
			str << b;
			base_set.answer_b = str.str();
		} else {
			base_set.answer_b = json["answer_b"].asString();
		}
		base_set.true_answer = json["true_answer"].asInt();
		base_set.next_id = json["next_id"].asInt();
		base_set.integral = json["integral"].asInt();
		set_map_second_value_by_key(base_set.id, question, base_set);
	}
}

void Config_Cache_Answer::refresh_common_config_cache(void) {
	common_cfg.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["answer"]["common_cfg"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		LOG_ABORT("not have answer common_cfg.json");
	}
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		uint32_t key = json["key"].asInt();
		uint32_t value = json["value"].asInt();
		common_cfg[key] = value;
	}
}

void Config_Cache_Answer::refresh_integral_reward_config_cache(void) {
	integral_reward.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["answer"]["integral_reward"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		LOG_ABORT("not have answer integral_reward.json");
	}
	Answer_Integral_Reward_Config set;
	for (uint i = 0; i < config_json.size(); ++i) {
		set.reset();
		const Json::Value& json = config_json[i];
		set.level = json["level"].asInt();
		Answer_Integral_Reward_Config_Map::iterator it = integral_reward.find(set.level);
		if(it == integral_reward.end()) {
			refresh_answer_reward_config_cache(set.reward, json);
			set_map_second_value_by_key(set.level, integral_reward, set);
		} else {
			refresh_answer_reward_config_cache(it->second.reward, json);
		}
	}
}

void Config_Cache_Answer::refresh_answer_reward_config_cache(Answer_Reward_Config &reward, const Json::Value& json) {
	uint32_t item = json["item_id"].asInt();
	uint32_t value = json["num"].asInt();
	if(item == PT_EXP_CURRENT) {
		reward.exp += value;
	} else {
		if(item > 999999) {
			reward.item.push_back(Answer_Reward_Item(item,value));
		} else {
			reward.money.push_back(Answer_Reward_Money(item,value));
		}
	}
}

void Config_Cache_Answer::refresh_rank_reward_config_cache(void) {
	rank_reward.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["answer"]["rank_reward"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		LOG_ABORT("not have answer rank_reward.json");
	}
	Answer_Rank_Reward_Config set;
	for (uint i = 0; i < config_json.size(); ++i) {
		set.reset();
		const Json::Value& json = config_json[i];
		set.start_rank = json["start_rank"].asInt();
		set.end_rank = json["end_rank"].asInt();
		for(uint j = 0; j < json["reward"].size(); ++j){
			const Json::Value& re_json =  json["reward"][j];
			uint32_t item = re_json[0u].asInt();
			uint32_t value = re_json[1u].asInt();
			if(item == PT_EXP_CURRENT) {
				set.reward.exp += value;
			} else {
				if(item > 999999) {
					set.reward.item.push_back(Answer_Reward_Item(item,value));
				} else {
					set.reward.money.push_back(Answer_Reward_Money(item,value));
				}
			}
		}
		rank_reward.push_back(set);
	}
}

void Config_Cache_Answer::refresh_level_modulus_config_cache(void) {
	level_modulus.clear();level_item_modulus.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["answer"]["level_modulus"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		LOG_ABORT("not have answer level_modulus.json");
	}
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		uint32_t level = json["level"].asInt();
		double rank = json["rank"].asDouble();
		double rank_item = json["rank_item"].asDouble();
		level_modulus[level] = rank;
		level_item_modulus[level] = rank_item;
	}
}
