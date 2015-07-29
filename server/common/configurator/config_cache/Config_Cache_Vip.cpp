/*
 * Vip_Config.cpp
 *
 *  Created on: 2014-07-15
 *      Author: "xiaoliang"
 */

#include "Config_Cache_Vip.h"
#include "Configurator.h"
#include "Json_Utility.h"

const World_Tree_Config *Config_Cache_Vip::get_world_tree_config(uint32_t id) {
	return get_map_second_pointer_by_key(id, world_tree_conf_map_);
}

const Grow_Get_Config *Config_Cache_Vip::get_grow_get_config(uint32_t vip_level) {
	return get_map_second_pointer_by_key(vip_level, grow_get_conf_map_);
}

const Vip_Open_Config *Config_Cache_Vip::get_vip_open_config(uint32_t id) {
	return get_map_second_pointer_by_key(id, vip_open_conf_map_);
}

const Turntable_Item_Config *Config_Cache_Vip::get_turntable_item_config(uint32_t id) {
	return get_map_second_pointer_by_key(id, turntable_item_conf_map_);
}

const Turntable_Cost_Config *Config_Cache_Vip::get_turntable_cost_config(uint8_t num) {
	return get_map_second_pointer_by_key(num, turntable_cost_conf_map_);
}

const UInt_Vec *Config_Cache_Vip::get_vip_addition_config(uint32_t type) {
	return get_map_second_pointer_by_key(type, vip_addition_map_);
}

const Vip_Daily_Item_Config_Vec *Config_Cache_Vip::get_vip_daily_config(uint32_t vip_level) {
	return get_map_second_pointer_by_key(vip_level, vip_daily_item_conf_vec_map);
}

Vip_Open_Conf_Map &Config_Cache_Vip::vip_open_conf_map(void) {
	return vip_open_conf_map_;
}
Grow_Get_Conf_Map &Config_Cache_Vip::grow_get_conf_map(void) {
	return grow_get_conf_map_;
}
World_Tree_Conf_Map &Config_Cache_Vip::world_tree_conf_map(void) {
	return world_tree_conf_map_;
}
Turntable_Item_Conf_Map &Config_Cache_Vip::turntable_item_conf_map(void) {
	return turntable_item_conf_map_;
}
Turntable_Config_Vec &Config_Cache_Vip::turntable_conf_vec(void) {
	return turntable_conf_vec_;
}

const Turntable_Item_Config_Vec *Config_Cache_Vip::turntable_item_conf_vec_by_type(uint8_t type) {
	return get_map_second_pointer_by_key(type, turntable_type_item_conf_map_);
}

void Config_Cache_Vip::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Vip");
	refresh_vip_open_config_cache();
	refresh_grow_get_config_cache();
	refresh_world_tree_config_cache();
	refresh_turntable_config_cache();
	refresh_turntable_item_config_cache();
	refresh_turntable_cost_config_cache();
	refresh_vip_addition_config_cache();
	refresh_vip_daily_item_config_cache();
}

void Config_Cache_Vip::refresh_vip_open_config_cache(void) {
	vip_open_conf_map_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["vip"]["vip_open"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Vip_Open_Config vip_open;
	for (uint i = 0; i < config.size(); ++i) {
		vip_open.reset();
		const Json::Value& json = config[i];
		vip_open.id = json["id"].asInt(); //ID
		vip_open.price = json["price"].asInt(); //价格
		vip_open.init_level = json["init_level"].asInt(); //初始等级
		vip_open.days = json["days"].asInt(); //开通天数

		set_map_second_value_by_key(vip_open.id, vip_open_conf_map_, vip_open);
	}
}

void Config_Cache_Vip::refresh_grow_get_config_cache(void) {
	grow_get_conf_map_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["vip"]["grow_get"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Grow_Get_Config value;
	for (uint i = 0; i < config.size(); ++i) {
		value.reset();
		const Json::Value& json = config[i];
		value.vip_level = json["vip_level"].asInt(); //VIP等级
		value.get_val = json["get_val"].asInt(); //每天获得的成长值
		value.lost_val = json["lost_val"].asInt(); //过期丢失成长值
		value.need_val = json["need_val"].asInt(); //升级所需成长值
		value.cost_ratio = json["cost_ratio"].asInt(); //消费成长值汇率
		value.pri_num = json["pri_num"].asInt(); //特权项数

		set_map_second_value_by_key(value.vip_level, grow_get_conf_map_, value);
	}
}

void Config_Cache_Vip::refresh_world_tree_config_cache(void) {
	world_tree_conf_map_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["vip"]["world_tree"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	World_Tree_Config value;
	for (uint i = 0; i < config.size(); ++i) {
		value.reset();
		const Json::Value& json = config[i];
		value.id = json["id"].asInt(); //ID

		value.addition.clear();
		const Json::Value& addition_json = json["addition"];
		if(addition_json != Json::Value::null && addition_json.size()) {
			for (uint i = 0; i < addition_json.size(); ++i) {
				value.addition.push_back(addition_json[i].asInt());
			}
		}

		set_map_second_value_by_key(value.id, world_tree_conf_map_, value);
	}
}

void Config_Cache_Vip::refresh_turntable_config_cache(void) {
	turntable_conf_vec_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["vip"]["turntable"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Turntable_Config value;
	for (uint i = 0; i < config.size(); ++i) {
		value.reset();
		const Json::Value& json = config[i];
		value.id = json["id"].asInt(); //ID

		value.item_list.clear();
		const Json::Value& list_json = json["list"];
		if(list_json != Json::Value::null && list_json.size()) {
			for (uint i = 0; i < list_json.size(); ++i) {
				value.item_list.insert(list_json[i].asInt());
			}
		}

		value.list_size = value.item_list.size();//物品列表大小
		value.max_open = json["max_open"].asInt();//最大获许数
		value.rate = json["rate"].asInt(); //权重

		turntable_conf_vec_.push_back(value);
	}
}

void Config_Cache_Vip::refresh_turntable_cost_config_cache(void) {
	turntable_cost_conf_map_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["vip"]["turntable_cost"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Turntable_Cost_Config value;
	for (uint i = 0; i < config.size(); ++i) {
		value.reset();
		const Json::Value& json = config[i];
		value.num = json["num"].asInt(); //数量
		value.cost = json["cost"].asInt(); //
		set_map_second_value_by_key(value.num, turntable_cost_conf_map_, value);
	}
}

void Config_Cache_Vip::refresh_turntable_item_config_cache(void) {
	turntable_item_conf_map_.clear();
	turntable_type_item_conf_map_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["vip"]["turntable_item"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Turntable_Item_Config value;
	for (uint i = 0; i < config.size(); ++i) {
		value.reset();
		const Json::Value& json = config[i];
		value.id = json["id"].asInt(); //ID
		value.item_id = json["item_id"].asInt(); //物品id
		value.num = json["num"].asInt(); //数量
		value.rate = json["rate"].asInt(); //权重
		value.ann = json["ann"].asInt(); //是否发送广播
		value.type.clear();
		const Json::Value& type_json = json["type"];
		if(type_json != Json::Value::null && type_json.size()) {
			for (uint i = 0; i < type_json.size(); ++i) {
				value.type.insert(type_json[i].asInt());
			}
		}
		//生成中间配置
		for(UInt8_Set::iterator it = value.type.begin(); it != value.type.end(); ++it) {
			refresh_turntable_type_tiem_conf_map_(*it, value);
		}

		set_map_second_value_by_key(value.id, turntable_item_conf_map_, value);
	}
}

void Config_Cache_Vip::refresh_turntable_type_tiem_conf_map_(uint8_t type, Turntable_Item_Config &item_config) {
	Turntable_Type_Item_Conf_Map::iterator it = turntable_type_item_conf_map_.find(type);
	if(it != turntable_type_item_conf_map_.end()) {
		it->second.push_back(item_config);
	} else {
		Turntable_Item_Config_Vec vec;
		vec.clear();
		vec.push_back(item_config);
		turntable_type_item_conf_map_.insert(std::make_pair(type, vec));
	}
}


void Config_Cache_Vip::refresh_vip_addition_config_cache(void) {
	vip_addition_map_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["vip"]["vipPrivilege"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	for (uint i = 0; i < config.size(); ++i) {
		const Json::Value& json = config[i];
		UInt_Vec addition_vec;
		addition_vec.clear();
		for (uint j = 0; j < json["addition"].size(); ++j) {
			const Json::Value& addition_json = json["addition"][j];
			addition_vec.push_back(addition_json.asInt());
		}
		set_map_second_value_by_key(json["itemId"].asInt(), vip_addition_map_, addition_vec);
	}
}


void Config_Cache_Vip::refresh_vip_daily_item_config_cache(void) {
	vip_daily_item_conf_vec_map.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["vip"]["daily"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Vip_Daily_Item_Config item;
	for (uint i = 0; i < config.size(); ++i) {
		const Json::Value& json = config[i];
		item.reset();
		item.vip_level = json["vip_level"].asInt();

		for (uint j = 0; j < json["item_id"].size(); ++j) {
			const Json::Value& addition_json = json["item_id"][j];
			item.item_id = addition_json[0u].asInt();
			item.num = addition_json[1u].asInt();
			item.bind = 2;
			vip_daily_item_conf_vec_map[item.vip_level].push_back(item);
		}
	}
}

