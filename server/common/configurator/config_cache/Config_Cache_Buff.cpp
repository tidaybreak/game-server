/*
 * Skill_Config.cpp
 *
 *  Created on: 2014-4-2
 *      Author: donald shaw
 */

#include "Config_Cache_Buff.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "status/Status_Def.h"
#include "Config_Cache_Fighter.h"


const Btl_Buff_Map &Config_Cache_Buff::btl_buff_cache(void) const {
	return btl_buff_map_;
}

const Btl_Buff_Cfg *Config_Cache_Buff::btl_buff(int buff_id) const {
	return get_map_second_pointer_by_key(buff_id, btl_buff_map_);
}

const Btl_Buff_Effect *Config_Cache_Buff::btl_effect(int buff_id, int effect_id) const {
	const Btl_Buff_Cfg *buff = this->get_map_second_pointer_by_key(buff_id, btl_buff_map_);
	if (NULL != buff) {
		for (Buff_Effect_Vec::const_iterator it = buff->effect_map.begin();
				it != buff->effect_map.end(); ++it) {
			if ((*it).buff_effect_id == effect_id) {
				return &(*it);
			}
		}
//		Buff_Effect_Map::const_iterator it = buff->effect_map.find(effect_id);
//		if (it != buff->effect_map.end()) {
//			return &(it->second);
//		}
	}
	return NULL;
}

const Btl_Buff_Effect *Config_Cache_Buff::btl_effect(int effect_id) const {
	return get_map_second_pointer_by_key(effect_id, effect_map_);
}
const Attribute_Number *Config_Cache_Buff::attribute_number(const int attribute_number_id) const {
	Attribute_Number_Map::const_iterator it = attribute_number_map_.find(attribute_number_id);
	if (it != attribute_number_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

const Int_Hash_Set *Config_Cache_Buff::btl_scene_visible(const int scene_id) const {
	return get_map_second_pointer_by_key(scene_id, scene_visible_);
}
const Status_Effect_Argv &Config_Cache_Buff::special_level_buff(void) const {
	return special_level_buff_;
}

void Config_Cache_Buff::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Buff");
	scene_visible_.clear();

	const Json::Value &buff_eff_json = CONFIG_INSTANCE->config_json()["fighter"]["buff_effect"];
	Int_Set status_ids_trigger_buff;// 有触发buff的效果

	Btl_Buff_Effect buff_eff;
	for (Json::Value::const_iterator be_it = buff_eff_json.begin();
			be_it != buff_eff_json.end(); ++be_it) {
		buff_eff.reset();
		buff_eff.buff_effect_id 			= (*be_it)["buff_effectid"].asInt();
		buff_eff.buff_effect_type 			= (*be_it)["buff_effecttype"].asInt();
		buff_eff.buff_hit 					= (*be_it)["buff_hit"].asInt();

		if (!(*be_it)["buff_delay"].isNull() &&
				(*be_it)["buff_delay"].isArray()) {
			Json_Utility::assign_int_vec((*be_it)["buff_delay"], buff_eff.buff_delay);
		}
		if (!(*be_it)["buff_heartbeattype"].isNull() &&
				(*be_it)["buff_heartbeattype"].isArray()) {
			Json_Utility::assign_int_vec((*be_it)["buff_heartbeattype"], buff_eff.beat_type_vec);//buff_heartbeattype,buff_heartbeatgap对应上
		}
		if (buff_eff.buff_delay.size() != buff_eff.beat_type_vec.size()) {
			LOG_ABORT("buff_effect_id: %d, buff_delay.size != beat_type_vec.size", buff_eff.buff_effect_id);
		}
		uint32_t i = 0;
		for (Int_Vec::const_iterator it = buff_eff.beat_type_vec.begin();
				it != buff_eff.beat_type_vec.end(); ++it) {
//			buff_eff.beat_type_set.insert(*it);
			if (*it == EFFECT_TYPE_TIME) {
				buff_eff.delay_time_gap = buff_eff.buff_delay[i];
			} else if (*it == EFFECT_TYPE_SHOT) {
				buff_eff.delay_action_gap = buff_eff.buff_delay[i];
			} else if (*it == EFFECT_TYPE_PASSIVE) {
				buff_eff.delay_passive_gap = buff_eff.buff_delay[i];
			}
			++i;
		}

		buff_eff.buff_direct				= (*be_it)["buff_direct"].asInt();

		if (!(*be_it)["buff_heartbeatgap"].isNull() &&
				(*be_it)["buff_heartbeatgap"].isArray()) {
			Json_Utility::assign_int_vec((*be_it)["buff_heartbeatgap"], buff_eff.beat_gap_vec);
		}
		if (buff_eff.beat_gap_vec.size() != buff_eff.beat_type_vec.size()) {
			LOG_ABORT("buff_effect_id: %d, beat_gap_vec.size != beat_type_vec.size", buff_eff.buff_effect_id);
		}
		i = 0;
		for (Int_Vec::const_iterator it = buff_eff.beat_type_vec.begin();
				it != buff_eff.beat_type_vec.end(); ++it) {
			buff_eff.beat_type_set.insert(*it);
			if (*it == EFFECT_TYPE_TIME) {
				buff_eff.time_gap = buff_eff.beat_gap_vec[i];
			} else if (*it == EFFECT_TYPE_SHOT) {
				buff_eff.action_gap = buff_eff.beat_gap_vec[i];
			} else if (*it == EFFECT_TYPE_PASSIVE) {
				buff_eff.passive_gap = buff_eff.beat_gap_vec[i];
			}
			++i;
		}

		buff_eff.buff_overlay				= (*be_it)["buff_effect_Overlay"].asBool();
		buff_eff.buff_trigger_skill			= (*be_it)["buff_triggertypeskill"].asInt();
		buff_eff.buff_chance				= (*be_it)["buff_chance"].asDouble() / 1000;
		if (0 == buff_eff.buff_chance) {
			buff_eff.buff_chance = 1;
		}
		if (Json::Value::null != (*be_it)["buff_triggerbuff"] && (*be_it)["buff_triggerbuff"].isArray()) {
			buff_eff.trigger_buff.status_id = (*be_it)["buff_triggerbuff"][0u].asInt();
			buff_eff.trigger_buff.status_from = TYPE_SOURCE_BUFF;
			//读取skill_val1
			Skill_Effect_Val_Inner inner;
			if (!(*be_it)["buff_triggerbuff"][1].isNull() &&
					(*be_it)["buff_triggerbuff"][1].isArray()) {
				Json_Utility::assign_int_vec((*be_it)["buff_triggerbuff"][1], buff_eff.trigger_buff.disappear_gap);
			}
			if (!(*be_it)["buff_triggerbuff"][2].isNull() &&
					(*be_it)["buff_triggerbuff"][2].isArray()) {
				int id = (*be_it)["buff_triggerbuff"][2][0u].asInt();
				double value = (*be_it)["buff_triggerbuff"][2][1].asDouble();
				CONFIG_CACHE_FIGHTER->get_attr_change_data(id, inner, value);
				if (0 == inner.val1) { //伤害链\经验加成
					inner.val1 = id;
					inner.val4 = value;
				}
				if (3 == (*be_it)["buff_triggerbuff"][2].size()) {
					double value2 = (*be_it)["buff_triggerbuff"][2][2].asDouble();
					inner.val7 = value2;//目前使用作治疗的下限值
				}

				Effect_Argv_Prop prop;
				prop.i_effect_val1 = inner.val1;
				prop.d_effect_val1 = inner.val4;
				prop.d_effect_val2 = inner.val5;
				prop.d_effect_val3 = inner.val6;
				prop.d_effect_val4 = inner.val7;
				buff_eff.trigger_buff.data.push_back(prop);
			}
		}
		buff_eff.val1						= (*be_it)["buff_val1"].asInt();
		buff_eff.val2						= (*be_it)["buff_val2"].asInt();
		buff_eff.val3						= (*be_it)["buff_val3"].asInt();
		buff_eff.val4						= (*be_it)["buff_val4"].asInt();
		buff_eff.val5						= (*be_it)["buff_val5"].asInt();
		set_map_second_value_by_key(buff_eff.buff_effect_id, effect_map_, buff_eff);
	}

	const Json::Value &buff_cfg = CONFIG_INSTANCE->config_json()["fighter"]["btl_buff"];
	Btl_Buff_Cfg cfg;
	for (Json::Value::const_iterator cfg_it = buff_cfg.begin();
			cfg_it != buff_cfg.end(); ++cfg_it) {
		cfg.reset();
		cfg.buff_id 		= (*cfg_it)["buff_id"].asInt();
		cfg.buff_charactor	= (*cfg_it)["buff_character"].asInt();
		if (!(*cfg_it)["buff_disappear"].isNull() &&
				(*cfg_it)["buff_disappear"].isArray()) {
			Json_Utility::assign_int_vec((*cfg_it)["buff_disappear"], cfg.buff_disappear_vec);
			Json_Utility::assign_set_list((*cfg_it)["buff_disappear"], cfg.buff_disappear);
		}
		if (!(*cfg_it)["buff_spdisappear"].isNull() &&
				(*cfg_it)["buff_spdisappear"].isArray()) {
			Json_Utility::assign_set_list((*cfg_it)["buff_spdisappear"], cfg.buff_spdisappear);
		}
		cfg.overlay	 		= (*cfg_it)["buff_Overlay"].asInt();
		cfg.is_dispel		= (*cfg_it)["buff_dispel"].asBool();
		cfg.is_pure			= (*cfg_it)["buff_purification"].asBool();
		cfg.is_stoic		= (*cfg_it)["buff_Stoic"].asBool();
		cfg.is_fight_buff		= (*cfg_it)["buff_battle_decide"].asBool();
		if (!(*cfg_it)["buff_scene"].isNull() && (*cfg_it)["buff_scene"].isArray()) {
			Json_Utility::assign_int_vec((*cfg_it)["buff_scene"], cfg.buff_scene);
		}
		if (!(*cfg_it)["scene_visible"].isNull() && (*cfg_it)["scene_visible"].isArray()) {
			Json_Utility::assign_int_vec((*cfg_it)["scene_visible"], cfg.scene_visible);
			for (Int_Vec::const_iterator it_sv = cfg.scene_visible.begin(); it_sv != cfg.scene_visible.end(); ++it_sv) {
				scene_visible_[*it_sv].insert(cfg.buff_id);
			}
		}
		for (Json::Value::const_iterator ef_it = (*cfg_it)["buff_effectid"].begin();
				ef_it != (*cfg_it)["buff_effectid"].end(); ++ef_it) {
			int e_id = (*ef_it).asInt();
			const Btl_Buff_Effect* it = this->btl_effect(e_id);
			if (NULL == it) {
				LOG_ABORT("ERROR buff id:%d effcte_id: %d not exist!!;", cfg.buff_id, e_id);
				continue;
			}
			if (!it->buff_overlay) {
				cfg.is_effect_un_overlay = true;
			}
			cfg.effect_map.push_back(*it);
		}

		set_map_second_value_by_key(cfg.buff_id, btl_buff_map_, cfg);
	}

	{
		this->attribute_number_map_.clear();
		const Json::Value &buff_cfg = CONFIG_INSTANCE->config_json()["fighter"]["Attribute_number"];
		Attribute_Number cfg;
		for (Json::Value::const_iterator cfg_it = buff_cfg.begin();
				cfg_it != buff_cfg.end(); ++cfg_it) {
			cfg.reset();
			cfg.attribute_number_id		= (*cfg_it)["Attribute_number_id"].asInt();
			cfg.attr_id		= (*cfg_it)["attr_id"].asInt();
			cfg.add			= (*cfg_it)["add"].asBool();
			cfg.percent		= (*cfg_it)["percent"].asBool();
			cfg.cover		= (*cfg_it)["cover"].asBool();

			set_map_second_value_by_key(cfg.attribute_number_id, attribute_number_map_, cfg);
		}
	}

	{
		special_level_buff_.reset();
		const Json::Value &buff_cfg = CONFIG_INSTANCE->config_json()["public"]["public"];
		for (Json::Value::const_iterator cfg_it = buff_cfg.begin();
						cfg_it != buff_cfg.end(); ++cfg_it) {
			int type = (*cfg_it)["type"].asInt();
			if (type == 1) {
				special_level_buff_.status_id = (*cfg_it)["val"][0u].asInt();
				special_level_buff_.status_from = TYPE_SOURCE_BUFF;
				//读取skill_val1
				Skill_Effect_Val_Inner inner;
				if (!(*cfg_it)["val"][1].isNull() &&
						(*cfg_it)["val"][1].isArray()) {
					Json_Utility::assign_int_vec((*cfg_it)["val"][1], special_level_buff_.disappear_gap);
				}
				if (!(*cfg_it)["val"][2].isNull() &&
						(*cfg_it)["val"][2].isArray()) {
					int id = (*cfg_it)["val"][2][0u].asInt();
					double value = (*cfg_it)["val"][2][1].asDouble();
					CONFIG_CACHE_FIGHTER->get_attr_change_data(id, inner, value);
					if (0 == inner.val1) { //伤害链\经验加成
						inner.val1 = id;
						inner.val4 = value;
					}
					if (3 == (*cfg_it)["val"][2].size()) {
						double value2 = (*cfg_it)["val"][2][2].asDouble();
						inner.val7 = value2;//目前使用作治疗的下限值
					}

					Effect_Argv_Prop prop;
					prop.i_effect_val1 = inner.val1;
					prop.d_effect_val1 = inner.val4;
					prop.d_effect_val2 = inner.val5;
					prop.d_effect_val3 = inner.val6;
					prop.d_effect_val4 = inner.val7;
					special_level_buff_.data.push_back(prop);
				}
			}
		}
	}

	return ;
}
