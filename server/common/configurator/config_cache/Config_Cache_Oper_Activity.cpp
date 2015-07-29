/*
 * Config_Cache_Oper_Activity.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Oper_Activity.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "fighter/Fighter_Def.h"

const Oper_Activity_Project_Config_Map &Config_Cache_Oper_Activity::get_project_map(void) const {
	return project_map;
}
const Oper_Activity_Project_Config *Config_Cache_Oper_Activity::get_project_config_by_id(const uint32_t id) const {
	return get_map_second_pointer_by_key(id, project_map);
}

const UInt_Set &Config_Cache_Oper_Activity::get_has_effect_id(void) const {
	return has_effcet_id;
}

const UInt_Set *Config_Cache_Oper_Activity::get_type_actidset(const uint32_t type) const {
	return get_map_second_pointer_by_key(type, type_actidset);
}

void Config_Cache_Oper_Activity::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Oper_Activity");
	refresh_activity_config_cache();

}

void Config_Cache_Oper_Activity::refresh_activity_config_cache(void) {
	project_map.clear();
	has_effcet_id.clear();
	type_actidset.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["oper_activity"]["activity"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		return;
	}
	Oper_Activity_Project_Config proj_conf;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		proj_conf.reset();
		proj_conf.act_id = json["act_id"].asInt();
		proj_conf.mail_id = json["mail_id"].asInt();
		proj_conf.not_show = json["not_show"].asInt();

		refresh_activity_detail_config_cache(json["detail"], proj_conf.item_line, proj_conf.act_id);
		refresh_activity_extra_config_cache(json["extra"], proj_conf.append_effect, proj_conf.act_id);

		set_map_second_value_by_key(proj_conf.act_id, project_map, proj_conf);
		if(!proj_conf.append_effect.empty()) {
			has_effcet_id.insert(proj_conf.act_id);
		}
	}
}

void Config_Cache_Oper_Activity::refresh_activity_detail_config_cache(const Json::Value& config_json, Oper_Activity_Item_Line_Config_Map &item_line, const uint32_t act_id) {
	Oper_Activity_Item_Line_Config value;
	for(uint i = 0; i < config_json.size(); ++i){
		const Json::Value& json =  config_json[i];
		value.reset();
		value.rec_id =  json["rec_id"].asInt();
		value.type =  json["type"].asInt();
		value.num =  json["num"].asInt();

		refresh_activity_detail_config_pram_ext_all_type(json["parm"], value);

		value.refresh =  json["refresh"].asInt();
		value.ditto =  (json["ditto"].isInt() && json["ditto"].asInt())?true:false;
		refresh_activity_detail_reward_config_cache(json["reward"], value.reward_info);
		set_map_second_value_by_key(value.rec_id, item_line, value);
		if(type_actidset[value.type].find(act_id) == type_actidset[value.type].end()) {
			type_actidset[value.type].insert(act_id);
		}
	}
}

void Config_Cache_Oper_Activity::refresh_activity_extra_config_cache(const Json::Value& config_json, Oper_Activity_Append_Effect_Config_Map &effect_map, const uint32_t act_id) {
	Oper_Activity_Append_Effect_Config value;
	for(uint i = 0; i < config_json.size(); ++i){
		const Json::Value& json =  config_json[i];
		value.reset();
		value.ae_id =  json["ae_id"].asInt();
		value.type =  json["type"].asInt();

		switch(value.type) {
			case OPER_ACTIVITY_APPEND_BUFF_EFFECT: {
				refresh_activity_extra_buff_effect_config_cache(json, value.prop_map);
				break;
			}
		}

		set_map_second_value_by_key(value.ae_id, effect_map, value);

	}
}

void Config_Cache_Oper_Activity::refresh_activity_extra_buff_effect_config_cache(const Json::Value& config_json, Effect_Argv_Prop_Map &prop_map) {
	const Json::Value& json =  config_json["v1"];
	if(json.isArray()) {
		Effect_Argv_Prop prop;
		for(uint i = 0; i < json.size(); ++i) {
			const Json::Value& bjson = json[i];
			if(!bjson.isArray() || bjson.size() <= 0) continue;
			prop.reset();
			int32_t status_id =  bjson[0u].asInt();
			if(bjson.size() >= 2) prop.effect_value = bjson[1u].asDouble();
			if(bjson.size() >= 3) prop.i_effect_val1 = bjson[2u].asInt();
			if(bjson.size() >= 4) prop.i_effect_val2 = bjson[3u].asInt();
			if(bjson.size() >= 5) prop.i_effect_val3 = bjson[4u].asInt();
			if(bjson.size() >= 6) prop.d_effect_val1 = bjson[5u].asDouble();
			if(bjson.size() >= 7) prop.d_effect_val2 = bjson[6u].asDouble();
			if(bjson.size() >= 8) prop.d_effect_val3 = bjson[7u].asDouble();
			if(bjson.size() >= 9) prop.d_effect_val4 = bjson[8u].asDouble();
			Effect_Argv_Prop_Map::iterator it = prop_map.find(status_id);
			if(it == prop_map.end()) {
				Effect_Argv_Prop_Vec prop_vec;
				prop_vec.clear();
				if(prop.i_effect_val1) {
					prop_vec.push_back(prop);
				}
				prop_map.insert(std::make_pair(status_id, prop_vec));
			} else {
				it->second.push_back(prop);
			}
		}
	}
}

void Config_Cache_Oper_Activity::refresh_activity_detail_reward_config_cache(const Json::Value& config_json, Oper_Activity_Reward_Info_Config &reward) {
	if(config_json.isArray()) {
		for(uint i = 0; i < config_json.size(); ++i) {
			const Json::Value& json = config_json[i];

			if(!json.isArray()) continue;

			uint32_t item = json[0u].asInt();
			uint32_t value = json[1u].asInt();
			if(item == PT_EXP_CURRENT) {
				reward.exp += value;
			} else {
				if(item > 999999) {
					reward.item.push_back(Oper_Activity_Item_Config(item,value));
				} else {
					reward.money.push_back(Oper_Activity_Money_Config(item,value));
				}
			}
		}
	}
}

void Config_Cache_Oper_Activity::refresh_activity_detail_config_pram_ext_all_type(const Json::Value& config_json, Oper_Activity_Item_Line_Config &value) {
	if(config_json.isInt()) {
		refresh_activity_detail_config_pram_ext_int(config_json, value.parm);
	} else if(config_json.isArray()) {
		if(!refresh_activity_detail_config_pram_ext_array(config_json, value.parm_ex_arr)){
			refresh_activity_detail_config_pram_ext2_array(config_json, value.parm_ex_2arr);
		}
	}
}

void Config_Cache_Oper_Activity::refresh_activity_detail_config_pram_ext2_array(const Json::Value& config_json, UInt_2_Vec &value) {
	for(uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		if(json.isArray()) {
			UInt_Vec val;
			refresh_activity_detail_config_pram_ext_array(json, val);
			value.push_back(val);
		}
	}
}

bool Config_Cache_Oper_Activity::refresh_activity_detail_config_pram_ext_array(const Json::Value& config_json, UInt_Vec &value) {
	for(uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		if(!json.isInt()) return false;
		value.push_back(json.asInt());
	}
	return true;
}

void Config_Cache_Oper_Activity::refresh_activity_detail_config_pram_ext_int(const Json::Value& config_json, uint32_t &value) {
	value = config_json.asInt();
}
