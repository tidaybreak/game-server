/*
 * Skill_Config.cpp
 *
 *  Created on: 2014-4-2
 *      Author: donald shaw
 */

#include "Config_Cache_Fighter.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "json/json.h"

const int EFFECT_PROPS[] = {
		PT_POWER, PT_AGILITY, PT_DEXTERITY, PT_ARMOR, PT_STAMINA, PT_PHY_ATTACK, PT_SPELL_ATTACK,
		PT_PHY_DEFENSE, PT_SPELL_DEFENSE, PT_HIT, PT_AVOID,	PT_IGNORE_DEF, PT_BLOOD_MAX, PT_CURE,
		PT_CURE_RATE, PT_SING_RATE, PT_CRIT, PT_CRIT_DEF, PT_INIT_MORALE, PT_MORALE_CAPS, PT_CRIT_HURT, PT_CRIT_HURT_SUB,
		PT_HIT_RATE_ADDITION, PT_AVOID_RATE_ADDITION, PT_CRIT_RATE_ADDITION, PT_CRIT_DEF_RATE_ADDITION,
		PT_SPEED, PT_BLOOD_CURRENT, PT_MORALE_CURRENT, PT_STRENGTH_CURRENT, PT_STRENGTH_MAX, PT_BLOOD_POOL_CURRENT,
		PT_BLOOD_POOL_MAX, PT_STATUS_HIT_RATE, PT_STATUS_DEF_RATE, PT_BLOOD_STEAL, PT_PHY_HURT_FIX,
		PT_PHY_HURT_RATE, PT_SPELL_HURT_FIX, PT_SPELL_HURT_RATE, PT_HURT_FIX, PT_HURT_RATE, PT_BE_HURT_FIX,
		PT_BE_HURT_RATE, PT_PK_HURT_ADD, PT_PK_HURT_SUB, PT_PHY_HURT_RATE_SUB, PT_SPELL_HURT_RATE_SUB,
		PT_HUMAN_ADD, PT_ELF_ADD, PT_ORC_ADD, PT_DRAGON_ADD, PT_UNDEAD_ADD, PT_DEMON_ADD, PT_HUMAN_SUB, PT_ELF_SUB,
		PT_ORC_SUB, PT_DRAGON_SUB, PT_UNDEAD_SUB, PT_DEMON_SUB, PT_SUMMON_ADD, PT_SUMMON_SUB, PT_BE_CURE_RATE,
		PT_EFFECT_RATE, PT_SKILL_HURT_RATE, PT_IGNORE_DEF_RATE,
};

const Skill_Config *Config_Cache_Fighter::skill_config_cache(int skill_id) const {
	return get_map_second_pointer_by_key(skill_id, skill_cache_map_);
}

const Skill_Detail *Config_Cache_Fighter::skill_detail_cache(int skill_id, int level) const {
	const Skill_Config *skill_config = get_map_second_pointer_by_key(skill_id, skill_cache_map_);
	if (skill_config) {
		return get_map_second_pointer_by_key(level, skill_config->skill_level_map);
	}
	return 0;
}

const Skill_Cache_Map& Config_Cache_Fighter::skill_cache_map(void)const {
	return skill_cache_map_;
}

const Skill_Talent_Map* Config_Cache_Fighter::skill_talent_cache(int career) const {
	return get_map_second_pointer_by_key(career, skill_talent_cache_map_);
}

const int Config_Cache_Fighter::fight_misc(const std::string &str) {
	String_Int_Map::const_iterator find_it = fighter_misc_.find(str);
	if (find_it == fighter_misc_.end()) return 0;
	return find_it->second;
}

const Skill_Talent* Config_Cache_Fighter::skill_talent(int career, int id) const {
	const Skill_Talent_Map* skill_talents = skill_talent_cache(career);
	if (skill_talents) {
		return get_map_second_pointer_by_key(id, *skill_talents);
	}
	return 0;
}
const Skill_Talent_Cache_Map& Config_Cache_Fighter::skill_talent_cache_map(void) const {
	return skill_talent_cache_map_;
}

void Config_Cache_Fighter::get_skill_ai_config(int skill_id, std::vector<Skill_AI_Config*>& ai_config_vec){
	Skill_AI_Config_Map::iterator it;
	for(it = skill_ai_config_map_.begin(); it != skill_ai_config_map_.end(); ++it){
		if(it->second.skill_id == skill_id){
			ai_config_vec.push_back(&it->second);
		}
	}
}

void Config_Cache_Fighter::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Fighter");

	const Json::Value &skill_base_json 		= CONFIG_INSTANCE->config_json()["fighter"]["skill_base"];
	const Json::Value &skill_detail_json 	= CONFIG_INSTANCE->config_json()["fighter"]["skill_detail"];
	const Json::Value &skill_effect_json 	= CONFIG_INSTANCE->config_json()["fighter"]["skill_effect"];
	const Json::Value &skill_correct_json 	= CONFIG_INSTANCE->config_json()["fighter"]["effect_correct"];

	const Json::Value &skill_ai_json = CONFIG_INSTANCE->config_json()["fighter"]["skill_ai"];

	Skill_AI_Config skill_ai_config;
	for (Json::Value::const_iterator it = skill_ai_json.begin(); it != skill_ai_json.end(); it++) {
		skill_ai_config.reset();
		skill_ai_config.id 					= (*it)["id"].asInt();
		skill_ai_config.skill_id 			= (*it)["skill_id"].asInt();
		skill_ai_config.priority_add     	= (*it)["skill_priority_add"].asInt();
		if(!(*it)["skill_ai_strike"].isNull()){
			Json_Utility::assign_id_int_int_vec((*it)["skill_ai_strike"], skill_ai_config.strike_vec);
		}
		ABORT_INT_ZERO(skill_ai_config.skill_id);
		if(skill_ai_config.strike_vec.empty() == false){
			set_map_second_value_by_key(skill_ai_config.id, skill_ai_config_map_, skill_ai_config);
		}
	}

	Skill_Config skill_config;
	for (Json::Value::const_iterator it = skill_base_json.begin(); it != skill_base_json.end(); it++) {
		skill_config.reset();
		skill_config.skill_id 				= (*it)["skill_id"].asInt();
		skill_config.skill_career 			= (*it)["skill_career"].asInt();
		skill_config.skill_series 			= (*it)["skill_series"].asInt();
		skill_config.skill_type 			= (*it)["skill_type"].asInt();
		skill_config.skill_affiliation 		= (*it)["skill_affiliation"].asInt();
		skill_config.skill_attribute 		= (*it)["skill_attribute"].asInt();
		skill_config.skill_conflict			= (*it)["skill_Conflicttype"].asInt();
		skill_config.target_conflict 		= (*it)["target_conflict"].asInt();
		skill_config.ignore_enemy_conflict	= (*it)["ignore_enemy_conflict"].asInt();
		skill_config.skill_sort				= (*it)["skill_accord_tyle"].asInt();
		skill_config.skill_priority	= (*it)["skill_priority"].asInt();
		ABORT_INT_ZERO(skill_config.skill_id);

		set_map_second_value_by_key(skill_config.skill_id, skill_cache_map_, skill_config);
	}

	Be_Skill_Effect_Map be_skill_effect_map;
	read_be_skill_config(be_skill_effect_map);

	Skill_Detail skill_detail;
	for (Json::Value::const_iterator it = skill_detail_json.begin(); it != skill_detail_json.end(); it++) {
		skill_detail.reset();
		skill_detail.skill_id 					= (*it)["skill_id"].asInt();
		skill_detail.skill_level 			= (*it)["skill_level"].asInt();
		skill_detail.skill_act 				= (*it)["skill_act"].asInt();
		skill_detail.yl_cost 					= (*it)["yl_cost"].asInt();
		skill_detail.goods_cost			= (*it)["goods_cost"][0u].asInt();
		skill_detail.goods_cost_num 	= (*it)["goods_cost"][1].asInt();
		skill_detail.lv_lim 					= (*it)["lv_lim"].asInt();
		skill_detail.intimacy_lv			= (*it)["intimacy_level"].asInt();
		skill_detail.wake_lv	 				= (*it)["wake_limit"].asInt();
		skill_detail.force_factor			= (*it)["force_factor"].asDouble();
//		skill_detail.mutex_cond 			= (*it)["mutex_cond"].asInt();
		Json_Utility::assign_time_value((*it)["hurt_time"], skill_detail.hurt_time);

		read_pre_skill((*it)["pre_skill"], skill_detail.pre_skill);
		Json_Utility::assign_int_vec((*it)["mutex_cond"], skill_detail.mutex_cond_vec);

		Skill_Cost fight_cost;
		for (Json::Value::const_iterator cost_it = (*it)["skill_cost"].begin();
				cost_it != (*it)["skill_cost"].end(); ++cost_it) {
			fight_cost.reset();
			fight_cost.type 				= (*cost_it)[0u].asInt();
			fight_cost.value 				= (*cost_it)[1].asDouble();
			fight_cost.coster				= (*cost_it)[2].asInt();
			skill_detail.fight_cost_vec.push_back(fight_cost);
		}

		Skill_Cd skill_cd;
		for (Json::Value::const_iterator cd_it = (*it)["skill_cd"].begin();
				cd_it != (*it)["skill_cd"].end(); ++cd_it) {
			skill_cd.reset();
			skill_cd.type 					= (*cd_it)[0u].asInt();
			skill_cd.millisecond 			= (*cd_it)[1].asInt();
			skill_detail.skill_cd_vec.push_back(skill_cd);
		}

		for (Json::Value::const_iterator cd_it = (*it)["pre_skill_cd"].begin();
				cd_it != (*it)["pre_skill_cd"].end(); ++cd_it) {
			skill_cd.reset();
			skill_cd.type 					= (*cd_it)[0u].asInt();
			skill_cd.millisecond 			= (*cd_it)[1].asInt();
			skill_detail.pre_cd_vec.push_back(skill_cd);
		}

		Json_Utility::assign_time_value((*it)["sing_time"], skill_detail.sing_time);

		int step = 1;
		Skill_Effect effect;
		for (Json::Value::const_iterator effect_it = (*it)["effect_id"].begin(); effect_it != (*it)["effect_id"].end(); ++effect_it, ++step) {
			effect.effect_id = (*effect_it).asInt();
			skill_detail.step_effect.insert(std::make_pair(step, effect));
		}
//		if (skill_detail.step_effect.empty()) {
//			LOG_USER("skill id:%d level:%d effect empty", skill_detail.skill_id, skill_detail.skill_level);
//		}

		// be skill effect
		//if ((*it)["be_skill_effect"].isNull() || !(*it)["be_skill_effect"].isArray()) continue;
		for (Json::Value::const_iterator be_skill_it = (*it)["be_skill_effect"].begin();
				be_skill_it != (*it)["be_skill_effect"].end(); ++be_skill_it) {
			int be_skill_effect_id = (*be_skill_it).asInt();
			if (0 == be_skill_effect_id) continue;
			Be_Skill_Effect_Map::iterator be_find_it = be_skill_effect_map.find(be_skill_effect_id);
			if (be_find_it == be_skill_effect_map.end()) continue;
			skill_detail.be_skill_effect_map[be_skill_effect_id] = be_find_it->second;
		}

		ABORT_INT_ZERO(skill_detail.skill_id);
		ABORT_INT_ZERO(skill_detail.skill_level);
		Skill_Cache_Map::iterator find_it = skill_cache_map_.find(skill_detail.skill_id);
		if (find_it != skill_cache_map_.end()) {
			if (find_it->second.skill_level_map.find(skill_detail.skill_level) != find_it->second.skill_level_map.end())
				LOG_ABORT("cannot find skill detail, skill_id=%d, skill_lv=%d", skill_detail.skill_id, skill_detail.skill_level);

			set_map_second_value_by_key(skill_detail.skill_level, find_it->second.skill_level_map, skill_detail);
		}
	}

	Skill_Effect skill_effect;
	Skill_Effect_Map skill_effect_map;
	for (Json::Value::const_iterator it = skill_effect_json.begin(); it != skill_effect_json.end(); it++) {
		skill_effect.reset();
		skill_effect.effect_id = (*it)["effect_id"].asInt();
		skill_effect.effect_type = (*it)["effect_type"].asInt();
		skill_effect.effectgroup_id = (*it)["effectgroup_id"].asInt();
		skill_effect.effect_series_id = (*it)["group_Probability"].asInt();
		//skill_effect.ignoring_defense = (*it)["ignoring_defense"].asDouble();
		skill_effect.step_fix_type = (*it)["fix_type"].asInt();
		skill_effect.effect_rate = (*it)["effect_Probability"].asDouble();

		if (!(*it)["angry_damage"].isNull()) {
			skill_effect.morale_damage.id = (*it)["angry_damage"][0u].asInt();
			skill_effect.morale_damage.value = (*it)["angry_damage"][1].asDouble();
		}

		skill_effect.target_sellect_info.select_type 		= (*it)["select_type"].asInt();
		skill_effect.target_sellect_info.target_relation 	= (*it)["target_relation"].asInt();
		Json_Utility::assign_set_list((*it)["target_type"], skill_effect.target_sellect_info.target_type);
		skill_effect.target_sellect_info.prior_line 		= (*it)["prior_line"].asInt();
		skill_effect.target_sellect_info.prior_row			= (*it)["prior_row"].asInt();
		skill_effect.target_sellect_info.prior_type 		= (*it)["prior_type"].asInt();
		skill_effect.target_sellect_info.target_amount		= (*it)["target_amount"].asInt();

		Json_Utility::assign_time_value((*it)["continue_time"], skill_effect.continue_time);

		if ((*it)["cure_hp"].isArray() && (*it)["cure_hp"].size()==2) {
			skill_effect.hp_cure.id = (*it)["cure_hp"][0u].asInt();
			skill_effect.hp_cure.value = (*it)["cure_hp"][1].asDouble();
		}

		//读取skill_val1
		if (!(*it)["skill_val1"].isNull() &&
				(*it)["skill_val1"].isArray()) {
			skill_effect.val1.id = (*it)["skill_val1"][0u].asInt();
			if (!(*it)["skill_val1"][1].isNull() &&
					(*it)["skill_val1"][1].isArray()) {
				Json_Utility::assign_int_vec((*it)["skill_val1"][1], skill_effect.val1.data1);
			}
			for (uint16_t j = 2; j < (*it)["skill_val1"].size(); ++j) {
				if (!(*it)["skill_val1"][j].isNull() &&
						(*it)["skill_val1"][j].isArray()) {
					Skill_Effect_Val_Inner inner;
					int id = (*it)["skill_val1"][j][0u].asInt();
					double value = (*it)["skill_val1"][j][1].asDouble();
					get_attr_change_data(id, inner, value);
					if (0 == inner.val1) { //伤害链
						inner.val1 = id;
						inner.val4 = value;
					}
					if (3 == (*it)["skill_val1"][j].size()) {
						double value2 = (*it)["skill_val1"][j][2].asDouble();
						inner.val7 = value2;//目前使用作治疗的下限值
					}
					skill_effect.val1.inner_vec.push_back(inner);
				}
			}
 		}

		//读取skill_val2
		Fix_Call_Monster fix_call_monster;
		if (skill_effect.effect_type == 10006) {
			if (!(*it)["skill_val2"].isNull() && (*it)["skill_val1"].isArray()) {
				if ((*it)["skill_val2"].size() > 1) {
					skill_effect.call_monster.monster_type_id = (*it)["skill_val2"][0u].asInt();
					skill_effect.call_monster.idx = (*it)["skill_val2"][1].asInt();
				}
				if ((*it)["skill_val2"].size() > 2 && (*it)["skill_val2"][2].isArray()) {
					for (Json::Value::const_iterator fix_it = (*it)["skill_val2"][2].begin();
							fix_it != (*it)["skill_val2"][2].end(); ++fix_it) {
						fix_call_monster.reset();
						fix_call_monster.type = (*fix_it)[0u].asInt();
						fix_call_monster.cfg_percent = (*fix_it)[1].asDouble();
						skill_effect.call_monster.fix_monster_vec.push_back(fix_call_monster);
					}
				}
			}
		}

		Transfer_Call_Monster t_call_monster;
		if (!(*it)["skill_val4"].isNull() && (*it)["skill_val4"].isArray()) {
			for (Json::Value::const_iterator c_it = (*it)["skill_val4"].begin();
					c_it != (*it)["skill_val4"].end(); ++c_it) {
				t_call_monster.reset();
				t_call_monster.monster_type_id = (*c_it)[0u].asInt();
				t_call_monster.idx = (*c_it)[1].asInt();
				t_call_monster.fix_blood_percent = (*c_it)[2].asDouble();
				skill_effect.transfer_call_monster_vec.push_back(t_call_monster);
			}
		}

		// read fix_steps values
		get_step_fix_config((*it)["fix_steps"], skill_effect.skill_step_fix_vec);

		if (skill_effect_map.find(skill_effect.effect_id) != skill_effect_map.end())
			LOG_ABORT("cannot find skill effect, effect_id=%d", skill_effect.effect_id);
		skill_effect_map.insert(std::make_pair(skill_effect.effect_id, skill_effect));
	}


	Skill_Correct_Map correct_map;
	correct_map.clear();
	Attr_Correct_Vec attr_correct_vec;
	Skill_Effect_Correct effect_correct;
	Attr_Correct attr_correct;
	for (Json::Value::const_iterator c_it = skill_correct_json.begin();
			c_it != skill_correct_json.end(); ++c_it) {
		attr_correct_vec.clear();

		for (size_t i = 0; i < ARRAY_SIZE(EFFECT_PROPS); ++i) {
			std::ostringstream prop_stream;
			prop_stream << EFFECT_PROPS[i];
			if ((*c_it)[prop_stream.str()] != Json::Value::null ) {
				attr_correct.reset();
				if ((*c_it)[prop_stream.str()].isArray()) {
					if ((*c_it)[prop_stream.str()].size() == 1) {
						attr_correct.type = EFFECT_PROPS[i];
						attr_correct.fixed_value = (*c_it)[prop_stream.str()][0u].asDouble();
						attr_correct_vec.push_back(attr_correct);
					} else if ((*c_it)[prop_stream.str()].size() == 2) {
						attr_correct.type = EFFECT_PROPS[i];
						attr_correct.fixed_value = (*c_it)[prop_stream.str()][0u].asDouble();
						attr_correct.percent_value = (*c_it)[prop_stream.str()][1].asDouble();
						attr_correct_vec.push_back(attr_correct);
					} else if ((*c_it)[prop_stream.str()].size() == 3) {
						attr_correct.type = EFFECT_PROPS[i];
						attr_correct.fixed_value = (*c_it)[prop_stream.str()][0u].asDouble();
						attr_correct.percent_value = (*c_it)[prop_stream.str()][1].asDouble();
						attr_correct.mask_value = (*c_it)[prop_stream.str()][2].asDouble();
						attr_correct_vec.push_back(attr_correct);
					}
				}
			}
		}
		if (attr_correct_vec.size() != 0u) {
			effect_correct.reset();
			effect_correct.effect_id = (*c_it)["id"].asInt();
			effect_correct.attr_crr_vec = attr_correct_vec;
			correct_map.insert(std::make_pair(effect_correct.effect_id, effect_correct));
		}
	}

	for (Skill_Cache_Map::iterator skill_it = skill_cache_map_.begin(); skill_it != skill_cache_map_.end(); ++skill_it) {
		for (Skill_Detail_Map::iterator detail_it = skill_it->second.skill_level_map.begin(); detail_it != skill_it->second.skill_level_map.end(); ++detail_it) {
			for (Skill_Effect_Map::iterator effect_it = detail_it->second.step_effect.begin(); effect_it != detail_it->second.step_effect.end(); ++effect_it) {
				// 战斗效果配置
				Skill_Effect_Map::iterator find_it = skill_effect_map.find(effect_it->second.effect_id);
				if (find_it == skill_effect_map.end())
					LOG_ABORT("effect error skill:%d level:%d effect:%d", skill_it->second.skill_id, detail_it->second.skill_level, effect_it->second.effect_id);

				effect_it->second = find_it->second;
				// 战斗修正配置
				int id = effect_it->second.effect_id;
				Skill_Correct_Map::iterator c_find_it = correct_map.find(id);//effect_it->second.effect_id);
				if (c_find_it != correct_map.end()) {
					effect_it->second.effect_correct = c_find_it->second;
				}
			}
		}
	}

	refresh_talent_config_cache();
	refresh_phy_power_config();
	refresh_fight_misc();
}

void Config_Cache_Fighter::get_attr_change_data(int id, Skill_Effect_Val_Inner & inner, double value) {
	const Json::Value &attr_config = CONFIG_INSTANCE->config_json()["fighter"]["Attribute_number"];
	for (Json::Value::const_iterator it = attr_config.begin();
			it != attr_config.end(); ++it) {
		if ((*it)["Attribute_number_id"].asInt() != id) continue;
		inner.val1 = (*it)["attr_id"].asInt();
		if ((*it)["add"].asInt() == 1) {
			inner.val4 = value;
			return;
		} else if((*it)["percent"].asInt() == 1) {
			inner.val5 = value;
			return;
		} else if((*it)["cover"].asInt() == 1) {
			inner.val6 = value;
			return;
		}
	}
}

void Config_Cache_Fighter::refresh_talent_config_cache(void) {
	skill_talent_cache_map_.clear();
	Skill_Talent talent;
	const Json::Value talent_base = CONFIG_INSTANCE->config_json()["fighter"]["talent_base"];
	for (Json::Value::const_iterator it = talent_base.begin();
			it != talent_base.end(); ++it) {
		talent.reset();
		talent.talent_id = (*it)["talent_num"].asInt();
		talent.career = (*it)["talent_career"].asInt();
		talent.lv_cond = (*it)["talent_level"].asInt();
		talent.task_cond = (*it)["talent_task"].asInt();
		if (!(*it)["talent_item"].empty()){
			talent.goods_id = (*it)["talent_item"][0u].asInt();
			talent.goods_cost = (*it)["talent_item"][1].asInt();
		}

		//insert config
		Skill_Talent_Cache_Map::iterator map_it = skill_talent_cache_map_.find(talent.career);
		if (map_it == skill_talent_cache_map_.end()) {
			Skill_Talent_Map talent_map;
			talent_map[talent.talent_id] = talent;
			skill_talent_cache_map_[talent.career] = talent_map;
		} else {
			Skill_Talent_Map::iterator tln_it = map_it->second.find(talent.talent_id);
			if (tln_it == map_it->second.end()) {
				map_it->second.insert(std::make_pair(talent.talent_id, talent));
			}
		}
	}
}

void Config_Cache_Fighter::read_be_skill_config(Be_Skill_Effect_Map& be_skill_effect_map) {
	const Json::Value be_skill_config = CONFIG_INSTANCE->config_json()["fighter"]["be_skill_effect"];
	Be_Skill_Effect ef;
	for (Json::Value::const_iterator it = be_skill_config.begin();
			it != be_skill_config.end(); ++it) {
		ef.reset();
		ef.be_skill_effect_id = (*it)["be_skill_effect"].asInt();
		ef.trigger_type = (*it)["trigger_type"].asInt();
		ef.trigger_cd_time = (*it)["trigger_cd"].asInt();
		ef.max_trigger_times = (*it)["trigger_num"].asInt();
		ef.trigger_threshold_val = (*it)["trigger_val"].asInt();

		//ef.be_condition = (*it)["be_condition"].asInt();
		ef.be_fix_type = (*it)["be_fix_type"].asDouble();
		//ef.add_moral = (*it)["be_addpower"].asDouble();
		if (!(*it)["be_addpower"].isNull()) {
			for (Json::Value::const_iterator m_it = (*it)["be_addpower"].begin();
					m_it != (*it)["be_addpower"].end(); ++m_it) {
				int type = (*m_it)[0u].asInt();
				double value = (*m_it)[1].asDouble();
				ef.add_moral_map[type] = value;
			}
		}

		if (0 == ef.be_skill_effect_id) continue;

		if ((*it)["be_skill_property"].isArray()) {
			for (Json::Value::const_iterator p_it = (*it)["be_skill_property"].begin();
					p_it != (*it)["be_skill_property"].end(); ++p_it) {
				Be_Skill_Attr attr;
				attr.id = (*p_it)[0u].asInt();
				attr.fix = (*p_it)[1].asDouble();
				attr.percnet = (*p_it)[2].asDouble();
				ef.attr_vec.push_back(attr);
			}
		}

		if ((*it)["trigger_skill"].isArray()) {
			for (Json::Value::const_iterator t_it = (*it)["trigger_skill"].begin();
					t_it != (*it)["trigger_skill"].end(); ++t_it) {
				ef.trigger_skill.push_back((*t_it).asInt());
			}
		}

		if ((*it)["opening_skill"].isArray()) {
			for (Json::Value::const_iterator s_it = (*it)["opening_skill"].begin();
					s_it != (*it)["opening_skill"].end(); ++s_it) {
				ef.open_skills.push_back((*s_it).asInt());
			}
		}

		if ((*it)["be_effectid"].isArray()) {
			for (Json::Value::const_iterator s_it = (*it)["be_effectid"].begin(); s_it != (*it)["be_effectid"].end(); ++s_it) {
				ef.effect_vec.push_back((*s_it).asInt());
			}
		}

		get_step_fix_config((*it)["be_fix_steps"], ef.be_fix_steps);

		//读取be_buff----the same with active skill effect skill_val1
		if (!(*it)["be_buff"].isNull() &&
				(*it)["be_buff"].isArray()) {
			ef.val1.id = (*it)["be_buff"][0u].asInt();
			Skill_Effect_Val_Inner inner;
			if (!(*it)["be_buff"][1].isNull() &&
					(*it)["be_buff"][1].isArray()) {
				Json_Utility::assign_int_vec((*it)["be_buff"][1], ef.val1.data1);
			}
			if (!(*it)["be_buff"][2].isNull() &&
					(*it)["be_buff"][2].isArray()) {
				int id = (*it)["be_buff"][2][0u].asInt();
				double value = (*it)["be_buff"][2][1].asDouble();
				get_attr_change_data(id, inner, value);
				if (0 == inner.val1) { //伤害链
					inner.val1 = id;
					inner.val4 = value;
				}
				if (3 == (*it)["be_buff"][2].size()) {
					double value2 = (*it)["be_buff"][2][2].asDouble();
					inner.val7 = value2;//目前使用作治疗的下限值
				}
				ef.val1.inner_vec.push_back(inner);
			}
 		}

		if ((*it)["be_fix_bufftime"].isArray()) {
			const Json::Value fix_bufftime = (*it)["be_fix_bufftime"];
			for (Json::Value::const_iterator it = fix_bufftime.begin();
					it != fix_bufftime.end(); ++it) {
				int id = (*it)[0u].asInt();
				int time = (*it)[1].asInt();
				ef.fix_buff_time[id] = time;
			}
		}

		if ((*it)["be_fix_cd"].isArray()) {
			const Json::Value fix_bufftime = (*it)["be_fix_cd"];
			for (Json::Value::const_iterator it = fix_bufftime.begin();
					it != fix_bufftime.end(); ++it) {
				int id = (*it)[0u].asInt();
				int time = (*it)[1].asInt();
				ef.fix_skill_time[id] = time;
			}
		}

		if ((*it)["fix_anger"].isArray()) {
			for (Json::Value::const_iterator iter = (*it)["fix_anger"].begin();
					iter != (*it)["fix_anger"].end(); ++iter) {
				if (!(*iter).isArray() || (*iter).size() != 2)
					LOG_ABORT("fix_angger config error");
				ef.fix_morale_cost_map[(*iter)[0u].asInt()] = (*iter)[1].asDouble();
			}
		}

		be_skill_effect_map[ef.be_skill_effect_id] = ef;
	}
}

void Config_Cache_Fighter::get_step_fix_config(const Json::Value & cfg, Skill_Step_Fix_Vec & vec) {
	vec.clear();
	if (cfg.isNull() || !cfg.isArray()) return;
	Skill_Step_Fix step_fix;
	Skill_Effect_Val_Inner inner;
	for (Json::Value::const_iterator it = cfg.begin(); it != cfg.end(); ++it) {
		step_fix.reset();
		step_fix.value = (*it)[0u].asDouble();
		step_fix.id = (*it)[1].asInt();
		switch (step_fix.id) {
		case 1: case 2:
			step_fix.val1 = (*it)[2].asInt();
			break;
		case 3: case 4: case 5:
			step_fix.val2 = (*it)[2].asDouble();
			break;
		default:
			if (step_fix.id < 1000) break;
			double value = (*it)[2].asDouble();
			get_attr_change_data(step_fix.id, inner, value);
			step_fix.id = inner.val1;
			step_fix.val2 = inner.val4;
			step_fix.val3 = inner.val5;
			inner.reset();
			break;
		}
		vec.push_back(step_fix);
	}
}

void Config_Cache_Fighter::refresh_phy_power_config(void){
	Phy_Power_Cost_Config cost_info;
	const Json::Value& cost_json = CONFIG_INSTANCE->config_json()["power"]["power_cost"];
	for (Json::Value::const_iterator it = cost_json.begin();
			it != cost_json.end(); ++it) {
		cost_info.reset();
		cost_info.cost = (*it)["cost"].asInt();
		cost_info.number = (*it)["number"].asInt();
		set_map_second_value_by_key(cost_info.number, phy_power_cost_map_, cost_info);
	}

	Phy_Power_Lv_Config lv_info;
	const Json::Value& lv_json = CONFIG_INSTANCE->config_json()["power"]["power_lv"];
	for (Json::Value::const_iterator it = lv_json.begin();
			it != lv_json.end(); ++it) {
		lv_info.reset();
		lv_info.level = (*it)["level"].asInt();
		lv_info.number = (*it)["number"].asInt();
		set_map_second_value_by_key(lv_info.level, phy_power_lv_map_, lv_info);
	}
}

int Config_Cache_Fighter::get_phy_power_buy_cost(int number){
	Phy_Power_Cost_Config_Map::iterator it = phy_power_cost_map_.find(number);
	if(it != phy_power_cost_map_.end()){
		return it->second.cost;
	}
	return 100;
}

int Config_Cache_Fighter::get_phy_power_max_buy_times(int vip_level){
	Phy_Power_Lv_Config_Map::iterator it = phy_power_lv_map_.find(vip_level);
	if(it != phy_power_lv_map_.end()){
		return it->second.number;
	}
	return 3;
}

void Config_Cache_Fighter::read_pre_skill(const Json::Value &cfg, Int_Int_Vec &pre_skill) {
	pre_skill.clear();
	if (cfg.isNull() || !cfg.isArray()) return;
	Int_Int value;
	for (Json::Value::const_iterator it = cfg.begin(); it != cfg.end(); ++it) {
		value.reset();
		value.val_1 = (*it)[0u].asInt();
		value.val_2 = (*it)[1].asInt();
		pre_skill.push_back(value);
	}
}

void Config_Cache_Fighter::refresh_fight_misc(void) {
	const Json::Value& misc_json = CONFIG_INSTANCE->config_json()["fighter"]["fight_common"];
	for (Json::Value::const_iterator it = misc_json.begin(); it != misc_json.end(); ++it) {
		if ((*it)["isServer"].asInt() != 1) continue;
		this->fighter_misc_[(*it)["key"].asString()] = (*it)["value"].asInt();
	}
}
