/*
 * Config_Cache_LFSkill.cpp
 *
 *  Created on: 2015-03-11
 *      Author: zhoujin
 */

#include "Config_Cache_LFSkill.h"
#include "Configurator.h"
#include "Json_Utility.h"

void Config_Cache_LFSkill::refresh_config_cache(void) {
	refresh_life_skill();
	refresh_refine();
	refresh_card();
	refresh_smelt();
	refresh_smelt_product();
	refresh_other_config();
	refresh_gather_config();
	refresh_gather_other_config();

}

void Config_Cache_LFSkill::refresh_life_skill(void) {

	const Json::Value& skill_json = CONFIG_INSTANCE->config_json()["life_skill"]["life_skill"];
	for (Json::Value::iterator it = skill_json.begin();
				it != skill_json.end(); ++it) {
		LFSkill_Base base_config;
		base_config.reset();
		base_config.skill_id = (*it)["skill_id"].asInt();
		if ( (*it).isMember("next_skill"))
			base_config.next_id = (*it)["next_skill"].asInt();
		if ( (*it).isMember("skill_cost"))
			base_config.skill_cost = (*it)["skill_cost"].asInt();

		set_map_second_value_by_key(base_config.skill_id, lfskill_base_map, base_config);
	}
}

void Config_Cache_LFSkill::refresh_refine(void) {

	const Json::Value& skill_json = CONFIG_INSTANCE->config_json()["life_skill"]["refine"];
	for (Json::Value::iterator it = skill_json.begin();
				it != skill_json.end(); ++it) {
		int key = (*it)["skill_id"].asInt();
		if ( key /1000 != 1 )
			continue;
		LFSkill_Base_Map::iterator iter = lfskill_base_map.find(key);
		if ( iter == lfskill_base_map.end() )
			continue;
		iter->second.collect_unit.base_success = (*it)["base_success"].asDouble() /1000;
		iter->second.collect_unit.refine_sucess = (*it)["refine_sucess"].asDouble() /1000;
		if ( (*it).isMember("skilled_gold"))
		    iter->second.collect_unit.cast_gold_once = (*it)["skilled_gold"].asUInt();
		if ( (*it).isMember("cast_gold"))
		    iter->second.collect_unit.refine_cost = (*it)["cast_gold"].asInt();
		if ( (*it).isMember("skilled_max"))
		    iter->second.collect_unit.skilled_max = (*it)["skilled_max"].asInt();
		if ( (*it).isMember("skilled_once"))
		    iter->second.collect_unit.skilled_once = (*it)["skilled_once"].asInt();
	}
}

void Config_Cache_LFSkill::refresh_card(void) {

	const Json::Value& skill_json = CONFIG_INSTANCE->config_json()["life_skill"]["card"];
	for (Json::Value::iterator it = skill_json.begin();
				it != skill_json.end(); ++it) {
		int key = (*it)["skill_id"].asInt();
		if ( key /1000 != 2 )
			continue;
		LFSkill_Base_Map::iterator iter = lfskill_base_map.find(key);
		if ( iter == lfskill_base_map.end() )
			continue;
		iter->second.card_unit.base_success = (*it)["change_base"].asDouble()/1000;
		iter->second.card_unit.refine_sucess = (*it)["change_refine"].asDouble()/1000;
		if ( (*it).isMember("cast_gold"))
		    iter->second.card_unit.make_cost = (*it)["cast_gold"].asInt();
		if ( (*it).isMember("energy_cast"))
		    iter->second.card_unit.energy_cost = (*it)["energy_cast"].asInt();
		if ( (*it).isMember("skilled_max"))
		    iter->second.card_unit.skilled_max = (*it)["skilled_max"].asInt();
		if ( (*it).isMember("skilled_once"))
		    iter->second.card_unit.skilled_once = (*it)["skilled_once"].asInt();
		if ( (*it).isMember("skilled_gold"))
		    iter->second.card_unit.cast_gold_once = (*it)["skilled_gold"].asInt();
		if ( (*it).isMember("card_id")) {
			Json::Value card_id = (*it)["card_id"];
			if ( card_id.size() >0 ) {
				for(uint8_t i = 0;i < card_id.size();++i) {
					if ( card_id[i].size() != 2 )
						continue;
					Int_Int card_pro;
					card_pro.val_1 = card_id[i][0u].asInt();
					card_pro.val_2 = card_id[i][1u].asInt();
					iter->second.card_unit.card_vec.push_back(card_pro);
				}
			}
		}
		if ( (*it).isMember("change_id")) {
			Json::Value change_id = (*it)["change_id"];
			if ( change_id.size() >0 ) {
				for(uint8_t i = 0;i < change_id.size();++i) {
					if ( change_id[i].size() != 2 )
						continue;
					Int_Int change_pro;
					change_pro.val_1 = change_id[i][0u].asInt();
					change_pro.val_2 = change_id[i][1u].asInt();
					iter->second.card_unit.change_vec.push_back(change_pro);
				}
			}
		}
		if ( (*it).isMember("material")) {
			Json::Value material = (*it)["material"];
			if ( material.size() >0 ) {
				for(uint8_t i = 0;i < material.size();++i) {
					if ( material[i].size() != 2 )
						continue;
					Int_Int mer_num;
					mer_num.val_1 = material[i][0u].asInt();
					mer_num.val_2 = material[i][1u].asInt();
					iter->second.card_unit.meterial_vec.push_back(mer_num);
				}
			}
		}
	}
}

void Config_Cache_LFSkill::refresh_smelt(void) {

	const Json::Value& skill_json = CONFIG_INSTANCE->config_json()["life_skill"]["smelt"];
	for (Json::Value::iterator it = skill_json.begin();
				it != skill_json.end(); ++it) {
		int key = (*it)["skill_id"].asInt();
		if ( key/1000 != 3)
			continue;
		LFSkill_Base_Map::iterator iter = lfskill_base_map.find(key);
		if ( iter == lfskill_base_map.end() )
			continue;
		//iter->second.smelt_unit.skill_id = key;
		if ( (*it).isMember("skilled_max"))
		    iter->second.smelt_unit.skilled_max = (*it)["skilled_max"].asInt();
		if ( (*it).isMember("skilled_once"))
		    iter->second.smelt_unit.skilled_once = (*it)["skilled_once"].asInt();
		if ( (*it).isMember("skilled_gold"))
		    iter->second.smelt_unit.cast_gold_once = (*it)["skilled_gold"].asInt();
	}
}

void Config_Cache_LFSkill::refresh_smelt_product(void) {
	const Json::Value& skill_json = CONFIG_INSTANCE->config_json()["life_skill"]["smelt_product"];
	for (Json::Value::iterator it = skill_json.begin();
				it != skill_json.end(); ++it) {
		Smelt_Tab tab_config;
		tab_config.reset();
		tab_config.id = (*it)["smelt_tab"].asInt();
		tab_config.base_success = (*it)["extra_base"].asDouble() / 1000;
		tab_config.energy_cost = (*it)["energy_cast"].asInt();
		tab_config.item_id = (*it)["item_id"].asUInt();
		tab_config.smelt_cost = (*it)["cast_gold"].asInt();
		if ( (*it).isMember("material")) {
			Json::Value material = (*it)["material"];
			if ( material.size() >0 ) {
				for(uint8_t i = 0;i < material.size();++i) {
					if ( material[i].size() != 2 )
						continue;
					Int_Int mer_num;
					mer_num.val_1 = material[i][0u].asInt();
					mer_num.val_2 = material[i][1u].asInt();
					tab_config.meterial_vec.push_back(mer_num);
				}
			}
		}
		tab_config.skill_id = (*it)["skill_id"].asInt();
		set_map_second_value_by_key(tab_config.id, tab_map, tab_config);
	}
}

void Config_Cache_LFSkill::refresh_other_config(void) {
	const Json::Value& other_json = CONFIG_INSTANCE->config_json()["life_skill"]["other"];

	Json::Value::iterator it = other_json.begin();
	if (it != other_json.end()) {
		com_config.level = (*it)["open"].asInt();
        if ( (*it).isMember("card_change_up") && (*it)["card_change_up"].size() >0) {
        	for(uint8_t i = 0;i < (*it)["card_change_up"].size();++i) {
        		com_config.card_probability.push_back((*it)["card_change_up"][i].asDouble() / 1000);
        	}
        }
        if ( (*it).isMember("smelt_extra_lv") && (*it)["smelt_extra_lv"].size() >0) {
            for(uint8_t i = 0;i < (*it)["smelt_extra_lv"].size();++i) {
                com_config.smelt_lv_probability.push_back((*it)["smelt_extra_lv"][i].asDouble() / 1000);
            }
        }
        if ( (*it).isMember("smelt_extra_up") && (*it)["smelt_extra_up"].size() >0) {
            for(uint8_t i = 0;i < (*it)["smelt_extra_up"].size();++i) {
                com_config.smelt_probability.push_back((*it)["smelt_extra_up"][i].asDouble() / 1000);
            }
        }
	}
}

void Config_Cache_LFSkill::refresh_gather_config(void) {
	const Json::Value& gather_json = CONFIG_INSTANCE->config_json()["life_skill"]["gather"];
	for (Json::Value::iterator it = gather_json.begin();
					it != gather_json.end(); ++it) {
		Gather_Con_Unit unit;
		unit.id = (*it)["monster_id"].asInt();
		unit.energy_cost = (*it)["energy_cast"].asUInt();
		unit.gather_num = (*it)["num"].asUInt();
		unit.skill_id = (*it)["skill"].asInt();
		if ( (*it).isMember("award") && (*it)["award"].isArray() ) {
			for (uint16_t i = 0;i < (*it)["award"].size();++i) {
				unit.award_vec.push_back((*it)["award"][i].asInt());
			}
		}
		if ( (*it).isMember("chance") && (*it)["chance"].isArray() ) {
			for (uint16_t i = 0;i < (*it)["chance"].size();++i) {
				unit.chance_vec.push_back((*it)["chance"][i].asInt());
			}
		}
		if ( unit.award_vec.size() != unit.chance_vec.size() )
			LOG_ABORT("monster id %d award size is not same of chance size", unit.id);
		set_map_second_value_by_key(unit.id, gather_cof_map, unit);
	}
}

void Config_Cache_LFSkill::refresh_gather_other_config(void) {
	const Json::Value& gather_change_json = CONFIG_INSTANCE->config_json()["life_skill"]["gather_chance"];
	for (Json::Value::iterator it = gather_change_json.begin();
					it != gather_change_json.end(); ++it) {
		Gather_Change_Cof unit;
		unit.level = (*it )["level"].asInt();
		if ( (*it).isMember("gather_lv") && (*it)["gather_lv"].isArray() ) {
			for (uint16_t i = 0;i < (*it)["gather_lv"].size();++i) {
				unit.gather_lv_chance.push_back((*it)["gather_lv"][i].asInt());
			}
		}
		set_map_second_value_by_key(unit.level, gather_change_map, unit);
	}
}

const LFSkill_Base *Config_Cache_LFSkill::lfskill_base_config(const int skill_id) const {
	LFSkill_Base_Map::const_iterator iter = lfskill_base_map.find(skill_id);
	if ( iter == lfskill_base_map.end() )
		return NULL;
	return &iter->second;
}

//Collect_Unit *Config_Cache_LFSkill::collect_unit_config(const int skill_id) const {
//	Collect_Unit_Map::iterator iter = collect_map.find(skill_id);
//	if ( iter == collect_map.end() )
//		return NULL;
//	return &iter->second;
//}
//
//Card_Make_Unit *Config_Cache_LFSkill::card_unit_config(const int skill_id) const {
//	Card_Unit_Map::iterator iter = card_map.find(skill_id);
//	if ( iter == card_map.end())
//		return NULL;
//	return &iter->second;
//}
//
//Smelt_Unie *Config_Cache_LFSkill::smelt_config(const int skill_id) const {
//	Smelt_Unit_Map::iterator iter = smelt_map.find(skill_id);
//	if ( iter == smelt_map.end() )
//		return NULL;
//	return &iter->second;
//
//}

const Smelt_Tab *Config_Cache_LFSkill::smelt_tab_config(const int id) const {

	Smelt_Tab_Map::const_iterator iter = tab_map.find(id);
	if ( iter == tab_map.end() )
		return NULL;
	return &iter->second;
}

const Life_Skill_Comm &Config_Cache_LFSkill::common_config() const {
	return com_config;
}

const Gather_Con_Unit *Config_Cache_LFSkill::gather_config(const int id) const {
	Gather_Cof_Map::const_iterator iter = gather_cof_map.find(id);
	if ( iter == gather_cof_map.end())
		return NULL;
	return &iter->second;
}

const Gather_Change_Cof *Config_Cache_LFSkill::gather_change_config(const int level) const {
	Gather_Change_Map::const_iterator iter = gather_change_map.find(level);
	if ( iter == gather_change_map.end() )
		return NULL;
	return &iter->second;

}
