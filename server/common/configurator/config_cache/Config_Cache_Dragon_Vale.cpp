/*
* Config_Cache_Dragon_Vale_Building.cpp
*
*  Created on: 2014年7月10日
*      Author: gaojianshan
*/

#include "Config_Cache_Dragon_Vale.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "pack/Pack_Def.h"

const Dragon_Building_Cfg& Config_Cache_Dragon_Vale::get_buidling_config() const {
	return dragon_building_cfg;
}

const Building_Level_Cfg* Config_Cache_Dragon_Vale::find_Building_Level_Cfg(uint8_t building_type,
				  uint16_t building_level) {
	uint32_t key = create_key_of_Building_Level_Cfg_Map(building_type, building_level);
	return get_map_second_pointer_by_key(key, dragon_building_cfg.building_level_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Building_Level_Cfg_to_map(uint8_t building_type,
									uint16_t building_level,
									const Building_Level_Cfg &cfg) {
	uint32_t key = create_key_of_Building_Level_Cfg_Map(building_type, building_level);
	set_map_second_value_by_key(key, dragon_building_cfg.building_level_cfg_map, cfg);
}

const Role_Level_Cfg* Config_Cache_Dragon_Vale::find_Building_Role_Level_Cfg(uint32_t player_level,
													   uint8_t building_type) {
	uint64_t key = create_key_of_Role_Level_Cfg_Map(player_level, building_type);
	return get_map_second_pointer_by_key(key, dragon_building_cfg.role_level_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Building_Role_Level_Cfg_to_map(uint32_t player_level,
										uint8_t building_type,
										const Role_Level_Cfg &cfg) {
	uint64_t key = create_key_of_Role_Level_Cfg_Map(player_level, building_type);
	set_map_second_value_by_key(key, dragon_building_cfg.role_level_cfg_map, cfg);
}

const Vip_Level_Cfg* Config_Cache_Dragon_Vale::find_Building_Vip_Level_Cfg(uint32_t vip_level) {
	return get_map_second_pointer_by_key(vip_level, dragon_building_cfg.vip_level_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Building_Vip_Level_Cfg_to_map(uint32_t vip_level,
										const Vip_Level_Cfg &cfg) {
	set_map_second_value_by_key(vip_level, dragon_building_cfg.vip_level_cfg_map, cfg);
}

const Building_Queue_Cfg* Config_Cache_Dragon_Vale::find_Building_Queue_Cfg(uint8_t building_order) {
	return get_map_second_pointer_by_key(building_order, dragon_building_cfg.building_queue_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Building_Queue_Cfg_to_map(uint8_t building_order,
										 const Building_Queue_Cfg &cfg) {
	set_map_second_value_by_key(building_order, dragon_building_cfg.building_queue_cfg_map, cfg);
}

const Quick_Upgrade_Cfg* Config_Cache_Dragon_Vale::find_Building_Quick_Upgrade_Cfg(uint32_t upgrade_left_time) {
	return get_map_second_pointer_by_key(upgrade_left_time, dragon_building_cfg.quick_upgrade_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Building_Quick_Upgrade_Cfg_to_map(uint32_t upgrade_left_time, const Quick_Upgrade_Cfg  &cfg) {
	set_map_second_value_by_key(upgrade_left_time, dragon_building_cfg.quick_upgrade_cfg_map, cfg);
}

const Create_Building_Cfg* Config_Cache_Dragon_Vale::find_Building_Create_Cfg(uint8_t order) {
	return get_map_second_pointer_by_key(order, dragon_building_cfg.create_building_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Building_Create_Cfg_to_map(uint8_t order,
												const Create_Building_Cfg &cfg) {
	set_map_second_value_by_key(order, dragon_building_cfg.create_building_cfg_map, cfg);
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Building_Level_Cfg() {
	const Json::Value& building_level_json_list = CONFIG_INSTANCE->config_json()["dragon"]["building"]["building_level"];
	if (building_level_json_list == Json::Value::null || building_level_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Building_Level_Cfg");
	}

	Building_Level_Cfg cfg;
	for (uint i = 0; i < building_level_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& building_level_json = building_level_json_list[i];
		cfg.type = (uint8_t)building_level_json["type"].asUInt();
		cfg.level = (uint16_t)building_level_json["level"].asUInt();
		cfg.production_per_min = (uint32_t)building_level_json["production_per_min"].asUInt();
		cfg.min_gain_limit = (uint32_t)building_level_json["min_gain_limit"].asUInt();
		cfg.max_storage = (uint32_t)building_level_json["max_storage"].asUInt();
		cfg.upgrade_need_hour = (uint32_t)building_level_json["upgrade_need_hour"].asUInt();
		cfg.upgrade_need_min = (uint32_t)building_level_json["upgrade_need_min"].asUInt();
		cfg.upgrade_need_second = (uint32_t)building_level_json["upgrade_need_second"].asUInt();
		cfg.upgrade_money_type = (uint8_t)building_level_json["upgrade_money_type"].asUInt();
		cfg.upgrade_money_num = (uint32_t)building_level_json["upgrade_money_num"].asUInt();
		cfg.cancel_upgrade_return_money_num = (uint32_t)building_level_json["cancel_upgrade_return_money_num"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Building_Level_Cfg_to_map(cfg.type, cfg.level, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Building_Role_Level_Cfg() {
	const Json::Value& role_level_json_list = CONFIG_INSTANCE->config_json()["dragon"]["building"]["role_level"];
	if (role_level_json_list == Json::Value::null || role_level_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Role_Level_Cfg");
	}

	Role_Level_Cfg cfg;
	for (uint i = 0; i < role_level_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& role_level_json = role_level_json_list[i];
		cfg.player_level = (uint32_t)role_level_json["player_level"].asUInt();
		cfg.type = (uint8_t)role_level_json["type"].asUInt();
		cfg.max_building_level = (uint16_t)role_level_json["max_building_level"].asUInt();
		cfg.max_building_num = (uint16_t)role_level_json["max_building_num"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Building_Role_Level_Cfg_to_map(cfg.player_level, cfg.type, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Building_Vip_Level_Cfg() {
	const Json::Value& vip_level_json_list = CONFIG_INSTANCE->config_json()["dragon"]["building"]["vip_level"];
	if (vip_level_json_list == Json::Value::null || vip_level_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Vip_Level_Cfg");
	}

	Vip_Level_Cfg cfg;
	for (uint i = 0; i < vip_level_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& vip_level_json = vip_level_json_list[i];
		cfg.vip_level = (uint32_t)vip_level_json["vip_level"].asUInt();
		cfg.max_queue_num = (uint8_t)vip_level_json["max_queue_num"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Building_Vip_Level_Cfg_to_map(cfg.vip_level, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Building_Queue_Cfg() {
	const Json::Value& building_queue_json_list = CONFIG_INSTANCE->config_json()["dragon"]["building"]["building_queue"];
	if (building_queue_json_list == Json::Value::null || building_queue_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Building_Queue_Cfg");
	}

	Building_Queue_Cfg cfg;
	for (uint i = 0; i < building_queue_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& building_queue_json = building_queue_json_list[i];
		cfg.building_order = (uint8_t)building_queue_json["building_order"].asUInt();
		cfg.open_queue_money_type = (uint8_t)building_queue_json["open_queue_money_type"].asUInt();
		cfg.open_queue_money_num = (uint32_t)building_queue_json["open_queue_money_num"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Building_Queue_Cfg_to_map(cfg.building_order, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Building_Quick_Upgrade_Cfg() {
	const Json::Value& quick_upgrade_json_list = CONFIG_INSTANCE->config_json()["dragon"]["building"]["quick_upgrade"];
	if (quick_upgrade_json_list == Json::Value::null || quick_upgrade_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Quick_Upgrade_Cfg");
	}

	Quick_Upgrade_Cfg cfg;
	for (uint i = 0; i < quick_upgrade_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& quick_upgrade_json = quick_upgrade_json_list[i];
		cfg.upgrade_left_time = (uint32_t)quick_upgrade_json["upgrade_left_time"].asUInt();
		cfg.quick_upgrade_money_type = (uint8_t)quick_upgrade_json["quick_upgrade_money_type"].asUInt();
		cfg.quick_upgrade_money_ratio = (uint32_t)quick_upgrade_json["quick_upgrade_money_ratio"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Building_Quick_Upgrade_Cfg_to_map(cfg.upgrade_left_time, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Building_Create_Cfg() {
	const Json::Value& create_building_json_list = CONFIG_INSTANCE->config_json()["dragon"]["building"]["create_building"];
	if (create_building_json_list == Json::Value::null || create_building_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Building_Create_Cfg");
	}

	Create_Building_Cfg cfg;
	for (uint i = 0; i < create_building_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& create_building_json = create_building_json_list[i];
		cfg.order = (uint8_t)create_building_json["order"].asUInt();
		cfg.create_need_hour = (uint32_t)create_building_json["create_need_hour"].asUInt();
		cfg.create_need_min = (uint32_t)create_building_json["create_need_min"].asUInt();
		cfg.create_need_second = (uint32_t)create_building_json["create_need_second"].asUInt();
		cfg.create_money_type = (uint8_t)create_building_json["create_money_type"].asUInt();
		cfg.create_money_num = (uint32_t)create_building_json["create_money_num"].asUInt();
		cfg.cancel_create_return_money_num = (uint32_t)create_building_json["cancel_create_return_money_num"].asUInt();
		cfg.create_money_type2 = (uint8_t)create_building_json["create_money_type2"].asUInt();
		cfg.create_money_num2 = (uint32_t)create_building_json["create_money_num2"].asUInt();
		cfg.cancel_create_return_2 = (uint32_t)create_building_json["cancel_create_return_2"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Building_Create_Cfg_to_map(cfg.order, cfg);
	}
}

const Dragon_All_Cfg& Config_Cache_Dragon_Vale::get_dragon_config() const {
	return dragon_all_cfg;
}

const Dragon_Base_Cfg& Config_Cache_Dragon_Vale::find_Dragon_Base_Cfg() {
	return dragon_all_cfg.dragon_base_cfg;
}

const Dragon_Role_Level_Cfg* Config_Cache_Dragon_Vale::find_Dragon_Role_Level_Cfg(
										uint32_t player_level,
										uint8_t dragon_keep_type) {
	uint64_t key = create_key_of_Dragon_Role_Level_Cfg_Map(player_level, dragon_keep_type);
	return get_map_second_pointer_by_key(key, dragon_all_cfg.dragon_role_level_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Dragon_Role_Level_Cfg_to_map(uint32_t player_level, uint8_t dragon_keep_type,
				const Dragon_Role_Level_Cfg &cfg) {
	uint64_t key = create_key_of_Dragon_Role_Level_Cfg_Map(player_level, dragon_keep_type);
	set_map_second_value_by_key(key, dragon_all_cfg.dragon_role_level_cfg_map, cfg);
}

const Dragon_Keep_Cfg* Config_Cache_Dragon_Vale::find_Dragon_Keep_Cfg(uint8_t dragon_keep_type, uint32_t dragon_level) {
	uint64_t key = create_key_of_Dragon_Keep_Cfg_Map(dragon_keep_type, dragon_level);
	return get_map_second_pointer_by_key(key, dragon_all_cfg.dragon_keep_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Dragon_Keep_Cfg_to_map(uint8_t dragon_keep_type, uint32_t dragon_level,
		const Dragon_Keep_Cfg &cfg) {
	uint64_t key = create_key_of_Dragon_Keep_Cfg_Map(dragon_keep_type, dragon_level);
	set_map_second_value_by_key(key, dragon_all_cfg.dragon_keep_cfg_map, cfg);
}

const Dragon_Feeling_Cfg* Config_Cache_Dragon_Vale::find_Dragon_Feeling_Cfg(uint8_t dragon_feeling) {
	return get_map_second_pointer_by_key(dragon_feeling, dragon_all_cfg.dragon_feeling_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Dragon_Feeling_Cfg_to_map(uint8_t dragon_feeling, const Dragon_Feeling_Cfg &cfg) {
	set_map_second_value_by_key(dragon_feeling, dragon_all_cfg.dragon_feeling_cfg_map, cfg);
}

const Dragon_Evolution_Cfg* Config_Cache_Dragon_Vale::find_Dragon_Evolution_Cfg(uint8_t progress) {
	return get_map_second_pointer_by_key(progress, dragon_all_cfg.dragon_evolution_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Dragon_Evolution_Cfg_to_map(uint8_t progress, const Dragon_Evolution_Cfg &cfg) {
	set_map_second_value_by_key(progress, dragon_all_cfg.dragon_evolution_cfg_map, cfg);
}

const Dragon_Play_Dragon_Cfg* Config_Cache_Dragon_Vale::find_Dragon_Play_Dragon_Cfg(uint32_t player_level) {
	return get_map_second_pointer_by_key(player_level, dragon_all_cfg.dragon_play_dragon_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Dragon_Play_Dragon_Cfg_to_map(uint32_t player_level,
									const Dragon_Play_Dragon_Cfg &cfg) {
	set_map_second_value_by_key(player_level, dragon_all_cfg.dragon_play_dragon_cfg_map, cfg);
}

const Play_Dragon_Get_Heart_Cfg* Config_Cache_Dragon_Vale::find_Dragon_Play_Dragon_Get_Heart_Cfg(uint16_t heart_num) {
	return get_map_second_pointer_by_key(heart_num, dragon_all_cfg.dragon_play_dragon_get_heart_cfg_map);
}

void Config_Cache_Dragon_Vale::add_Dragon_Play_Dragon_Get_Heart_Cfg_to_map(uint16_t heart_num,
									const Play_Dragon_Get_Heart_Cfg &cfg) {
	set_map_second_value_by_key(heart_num, dragon_all_cfg.dragon_play_dragon_get_heart_cfg_map, cfg);
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Dragon_Base_Cfg() {
	const Json::Value& base_cfg_json_list = CONFIG_INSTANCE->config_json()["dragon"]["dragon"]["base_cfg"];
	if (base_cfg_json_list == Json::Value::null || base_cfg_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Dragon_Base_Cfg");
	}

	dragon_all_cfg.dragon_base_cfg.reset();

	for (uint i = 0; i < base_cfg_json_list.size(); ++i) {
		const Json::Value& base_cfg_json = base_cfg_json_list[i];
		dragon_all_cfg.dragon_base_cfg.dragon_born_time = (uint32_t)base_cfg_json["dragon_born_time"].asUInt();
		dragon_all_cfg.dragon_base_cfg.max_feeling = (uint8_t)base_cfg_json["max_feeling"].asUInt();
		dragon_all_cfg.dragon_base_cfg.down_feeling_time = (uint32_t)base_cfg_json["down_feeling_time"].asUInt();
		dragon_all_cfg.dragon_base_cfg.money_type = (uint8_t)base_cfg_json["money_type"].asUInt();
		dragon_all_cfg.dragon_base_cfg.money_num = (uint32_t)base_cfg_json["money_num"].asUInt();
		dragon_all_cfg.dragon_base_cfg.play_dragon_time = (uint32_t)base_cfg_json["play_dragon_time"].asUInt();
		dragon_all_cfg.dragon_base_cfg.energy = (uint32_t)base_cfg_json["energy"].asUInt();
		dragon_all_cfg.dragon_base_cfg.chance = (uint8_t)base_cfg_json["chance"].asUInt();
		dragon_all_cfg.dragon_base_cfg.max_evolution = (uint8_t)base_cfg_json["max_evolution"].asUInt();
		dragon_all_cfg.dragon_base_cfg.max_batch_evolution_num = (uint16_t)base_cfg_json["batch_evolution_num"].asUInt();
		dragon_all_cfg.dragon_base_cfg.evolution_open_level = (uint16_t)base_cfg_json["evolveOpenLevel"].asUInt();
		dragon_all_cfg.dragon_base_cfg.max_play_dragon_award_num = (uint32_t)base_cfg_json["play_dragon_max_award_num"].asUInt();
		dragon_all_cfg.dragon_base_cfg.dragon_born_level = (uint16_t)base_cfg_json["dragon_born_level"].asUInt();
		dragon_all_cfg.dragon_base_cfg.dragon_vale_op_type = (uint32_t)base_cfg_json["dragon_vale_op_type"].asUInt();
		if(dragon_all_cfg.dragon_base_cfg.dragon_vale_op_type != Pack::COPPER
				&& dragon_all_cfg.dragon_base_cfg.dragon_vale_op_type != Pack::DRAGON_SOUL
				&& dragon_all_cfg.dragon_base_cfg.dragon_vale_op_type != Pack::SOULS
				&& dragon_all_cfg.dragon_base_cfg.dragon_vale_op_type != Pack::FRIENDSHIP) {
			LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Dragon_Base_Cfg ==> 龙谷奖励类型非法");
		}

		break;
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Dragon_Role_Level_Cfg() {
	const Json::Value& dragon_role_level_json_list = CONFIG_INSTANCE->config_json()["dragon"]["dragon"]["role_level"];
	if (dragon_role_level_json_list == Json::Value::null || dragon_role_level_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Dragon_Role_Level_Cfg");
	}

	Dragon_Role_Level_Cfg cfg;
	for (uint i = 0; i < dragon_role_level_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& dragon_role_level_json = dragon_role_level_json_list[i];
		cfg.player_level = (uint32_t)dragon_role_level_json["player_level"].asUInt();
		cfg.type = (uint8_t)dragon_role_level_json["type"].asUInt();
		cfg.dragon_max_level = (uint32_t)dragon_role_level_json["dragon_max_level"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Dragon_Role_Level_Cfg_to_map(cfg.player_level, cfg.type, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Dragon_Keep_Cfg() {
	const Json::Value& dragon_keep_cfg_json_list = CONFIG_INSTANCE->config_json()["dragon"]["dragon"]["keep_dragon"];
	if (dragon_keep_cfg_json_list == Json::Value::null || dragon_keep_cfg_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Dragon_Keep_Cfg");
	}

	Dragon_Keep_Cfg cfg;
	for (uint i = 0; i < dragon_keep_cfg_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& dragon_keep_cfg_json = dragon_keep_cfg_json_list[i];
		cfg.type = (uint8_t)dragon_keep_cfg_json["type"].asUInt();
		cfg.dragon_level = (uint32_t)dragon_keep_cfg_json["dragon_level"].asUInt();
		cfg.coin_num = (uint32_t)dragon_keep_cfg_json["coin_num"].asUInt();
		cfg.dragon_soul_num = (uint32_t)dragon_keep_cfg_json["dragon_soul_num"].asUInt();
		cfg.add_property = (float)dragon_keep_cfg_json["add_property"].asDouble();
		cfg.success_chance = (uint8_t)dragon_keep_cfg_json["success_chance"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Dragon_Keep_Cfg_to_map(cfg.type, cfg.dragon_level, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Dragon_Feeling_Cfg() {
	const Json::Value& dragon_feeling_cfg_json_list = CONFIG_INSTANCE->config_json()["dragon"]["dragon"]["feeling"];
	if (dragon_feeling_cfg_json_list == Json::Value::null || dragon_feeling_cfg_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Dragon_Feeling_Cfg");
	}

	Dragon_Feeling_Cfg cfg;
	for (uint i = 0; i < dragon_feeling_cfg_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& dragon_feeling_cfg_json = dragon_feeling_cfg_json_list[i];
		cfg.dragon_feeling = (uint8_t)dragon_feeling_cfg_json["dragon_feeling"].asUInt();
		cfg.chance = (uint8_t)dragon_feeling_cfg_json["chance"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Dragon_Feeling_Cfg_to_map(cfg.dragon_feeling, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Dragon_Evolution_Cfg() {
	const Json::Value& dragon_evolution_cfg_json_list = CONFIG_INSTANCE->config_json()["dragon"]["dragon"]["evolution"];
	if (dragon_evolution_cfg_json_list == Json::Value::null || dragon_evolution_cfg_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Dragon_Evolution_Cfg");
	}

	Dragon_Evolution_Cfg cfg;
	for (uint i = 0; i < dragon_evolution_cfg_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& dragon_evolution_cfg_json = dragon_evolution_cfg_json_list[i];
		cfg.progress = (uint8_t)dragon_evolution_cfg_json["progre"].asUInt();
		cfg.total_energy = (uint32_t)dragon_evolution_cfg_json["total_energy"].asUInt();
		cfg.base_power = (uint32_t)dragon_evolution_cfg_json["power"].asUInt();
		cfg.base_armor = (uint32_t)dragon_evolution_cfg_json["armor"].asUInt();
		cfg.base_stamina = (uint32_t)dragon_evolution_cfg_json["tamina"].asUInt();
		cfg.base_crit = (uint32_t)dragon_evolution_cfg_json["crit"].asUInt();
		cfg.base_prevent_crit = (uint32_t)dragon_evolution_cfg_json["prevent_crit"].asUInt();
		cfg.item_id = (uint32_t)dragon_evolution_cfg_json["item_id"].asUInt();
		cfg.item_num = (uint32_t)dragon_evolution_cfg_json["item_num"].asUInt();
		cfg.money_type = (uint8_t)dragon_evolution_cfg_json["money_type"].asUInt();
		cfg.money_num = (uint32_t)dragon_evolution_cfg_json["money_num"].asUInt();
		cfg.dragon_name = dragon_evolution_cfg_json["dragon_name"].asString();

		CONFIG_CACHE_DRAGON_VALE->add_Dragon_Evolution_Cfg_to_map(cfg.progress, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Dragon_Play_Dragon_Cfg() {
	const Json::Value& dragon_play_dragon_json_list = CONFIG_INSTANCE->config_json()["dragon"]["dragon"]["play_dragon"];
	if (dragon_play_dragon_json_list == Json::Value::null || dragon_play_dragon_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Dragon_Play_Dragon_Cfg");
	}

	Dragon_Play_Dragon_Cfg cfg;
	for (uint i = 0; i < dragon_play_dragon_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& dragon_play_dragon_json = dragon_play_dragon_json_list[i];
		cfg.player_level = (uint32_t)dragon_play_dragon_json["player_level"].asUInt();
		cfg.play_dragon_award = (uint32_t)dragon_play_dragon_json["play_dragon_award"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Dragon_Play_Dragon_Cfg_to_map(cfg.player_level, cfg);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Dragon_Play_Dragon_Get_Heart_Cfg() {
	const Json::Value& dragon_play_dragon_json_list = CONFIG_INSTANCE->config_json()["dragon"]["dragon"]["play_get_heart"];
	if (dragon_play_dragon_json_list == Json::Value::null || dragon_play_dragon_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Dragon_Play_Dragon_Get_Heart_Cfg");
	}

	Play_Dragon_Get_Heart_Cfg cfg;
	for (uint i = 0; i < dragon_play_dragon_json_list.size(); ++i) {
		cfg.reset();
		const Json::Value& play_dragon_get_heart_json = dragon_play_dragon_json_list[i];
		uint16_t heart_num = (uint16_t)play_dragon_get_heart_json["num"].asUInt();
		cfg.chance = (uint8_t)play_dragon_get_heart_json["chance"].asUInt();
		cfg.item_id = (uint32_t)play_dragon_get_heart_json["item_id"].asUInt();

		CONFIG_CACHE_DRAGON_VALE->add_Dragon_Play_Dragon_Get_Heart_Cfg_to_map(heart_num, cfg);
	}
}

const Dragon_Vale_Main_Cfg&  Config_Cache_Dragon_Vale::get_main_config() const {
	return dragon_vale_main_cfg;
}

const Main_Base_Cfg& Config_Cache_Dragon_Vale::find_main_base_cfg() {
	return dragon_vale_main_cfg.main_base_cfg;
}

const uint8_t* Config_Cache_Dragon_Vale::find_Main_Star_Up_Spped(uint8_t star) {
	return get_map_second_pointer_by_key(star, dragon_vale_main_cfg.main_star_up_speed_map);
}

void Config_Cache_Dragon_Vale::add_Main_Star_Up_Speed_to_map(uint8_t star, uint8_t percent) {
	set_map_second_value_by_key(star, dragon_vale_main_cfg.main_star_up_speed_map, percent);
}

const uint32_t* Config_Cache_Dragon_Vale::find_main_match_level(uint32_t begin_level) {
	return get_map_second_pointer_by_key(begin_level, dragon_vale_main_cfg.main_match_level_map);
}

void Config_Cache_Dragon_Vale::add_Main_Match_Level_to_map(uint32_t begin_level, uint32_t end_level) {
	set_map_second_value_by_key(begin_level, dragon_vale_main_cfg.main_match_level_map, end_level);
}

const uint32_t* Config_Cache_Dragon_Vale::find_main_foot_award(uint32_t player_level) {
	return get_map_second_pointer_by_key(player_level, dragon_vale_main_cfg.main_foot_award_map);
}

void Config_Cache_Dragon_Vale::add_Main_Foot_Award_to_map(uint32_t player_level, uint32_t dragon_soul_award) {
	set_map_second_value_by_key(player_level, dragon_vale_main_cfg.main_foot_award_map, dragon_soul_award);
}

const uint32_t* Config_Cache_Dragon_Vale::find_main_thief_award(uint32_t player_level, uint8_t thief_quality) {
	uint64_t key = create_key_of_Main_Thief_Award(player_level, thief_quality);
	return get_map_second_pointer_by_key(key, dragon_vale_main_cfg.main_thief_award_map);
}

const uint32_t* Config_Cache_Dragon_Vale::find_main_match_coin(uint32_t match_num) {
	return get_map_second_pointer_by_key(match_num, dragon_vale_main_cfg.main_match_coin_map);
}

void Config_Cache_Dragon_Vale::add_main_thief_award_to_map(uint32_t player_level,
						uint8_t thief_quality, uint32_t award) {
	uint64_t key = create_key_of_Main_Thief_Award(player_level, thief_quality);
	set_map_second_value_by_key(key, dragon_vale_main_cfg.main_thief_award_map, award);
}

void Config_Cache_Dragon_Vale::add_main_match_coin_to_map(uint32_t match_num, uint32_t coin) {
	set_map_second_value_by_key(match_num, dragon_vale_main_cfg.main_match_coin_map, coin);
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Main_Base_Cfg() {
	const Json::Value& base_cfg_json_list = CONFIG_INSTANCE->config_json()["dragon"]["main"]["base_cfg"];
	if (base_cfg_json_list == Json::Value::null || base_cfg_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Main_Base_Cfg");
	}

	dragon_vale_main_cfg.main_base_cfg.reset();

	for (uint i = 0; i < base_cfg_json_list.size(); ++i) {
		const Json::Value& main_base_json = base_cfg_json_list[i];
		dragon_vale_main_cfg.main_base_cfg.step_foot_max_num_per_day = (uint8_t)main_base_json["step_foot_max_num_per_day"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.normal_player_chance = (uint8_t)main_base_json["normal_player_chance"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.special_player_chance = (uint8_t)main_base_json["special_player_chance"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.whole_server_special_chance = (uint8_t)main_base_json["whole_server_special_chance"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.rob_time_hour = (uint32_t)main_base_json["rob_time_hour"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.rob_time_minute = (uint32_t)main_base_json["rob_time_minute"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.rob_percent = (uint8_t)main_base_json["rob_percent"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.rob_down_time = (uint32_t)main_base_json["rob_down_time"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.border_match_num_of_max_match_coin
		   	   	   	   	   = (uint32_t)main_base_json["border_match_num_of_max_match_coin"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.refresh_thief_time = (uint32_t)main_base_json["refresh_thief_time"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.refresh_thief_chance = (uint8_t)main_base_json["refresh_thief_chance"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.max_thief_num = (uint8_t)main_base_json["max_thief_num"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.thief_down_production = (uint8_t)main_base_json["thief_down_production"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.dragon_vale_scene_id = (uint32_t)main_base_json["dragon_vale_scene_id"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.battle_map_id = (uint32_t)main_base_json["battle_map_id"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.dragon_heart_item_id = (uint32_t)main_base_json["dragon_heart_item_id"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.max_dragon_heart_num = (uint32_t)main_base_json["max_dragon_heart_num"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.rob_level = (uint32_t)main_base_json["match_level"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.drive_thief_max_num = (uint32_t)main_base_json["drive_thief_max_num"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.white_chance = (uint8_t)main_base_json["white_chance"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.blue_chance = (uint8_t)main_base_json["blue_chance"].asUInt();
		dragon_vale_main_cfg.main_base_cfg.orange_chance = (uint8_t)main_base_json["orange_chance"].asUInt();
		break;
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Main_Star_Up_Speed() {
	const Json::Value& main_star_up_speed_json_list = CONFIG_INSTANCE->config_json()["dragon"]["main"]["star_up_speed"];
	if (main_star_up_speed_json_list == Json::Value::null || main_star_up_speed_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Main_Star_Up_Speed");
	}

	for (uint i = 0; i < main_star_up_speed_json_list.size(); ++i) {
		const Json::Value& main_star_up_json = main_star_up_speed_json_list[i];
		uint8_t star = (uint8_t)main_star_up_json["star"].asUInt();
		uint8_t percent = (uint8_t)main_star_up_json["percent"].asUInt();
		add_Main_Star_Up_Speed_to_map(star, percent);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Main_Match_Level() {
	const Json::Value& main_match_level_json_list = CONFIG_INSTANCE->config_json()["dragon"]["main"]["match"];
	if (main_match_level_json_list == Json::Value::null || main_match_level_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Main_Match_Level");
	}

	for (uint i = 0; i < main_match_level_json_list.size(); ++i) {
		const Json::Value& main_match_level_json = main_match_level_json_list[i];
		uint32_t begin_level = (uint32_t)main_match_level_json["begin_level"].asUInt();
		uint32_t end_level = (uint32_t)main_match_level_json["end_level"].asUInt();
		add_Main_Match_Level_to_map(begin_level, end_level);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Main_Foot_Award() {
	const Json::Value& main_foot_award_json_list = CONFIG_INSTANCE->config_json()["dragon"]["main"]["foot_award"];
	if (main_foot_award_json_list == Json::Value::null || main_foot_award_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Main_Foot_Award");
	}

	for (uint i = 0; i < main_foot_award_json_list.size(); ++i) {
		const Json::Value& main_star_up_json = main_foot_award_json_list[i];
		uint32_t player_level = (uint32_t)main_star_up_json["player_level"].asUInt();
		uint32_t dragon_soul_award = (uint32_t)main_star_up_json["dragon_soul_award"].asUInt();
		add_Main_Foot_Award_to_map(player_level, dragon_soul_award);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Main_Thief_Award() {
	const Json::Value& main_thief_award_json_list = CONFIG_INSTANCE->config_json()["dragon"]["main"]["thief_award"];
	if (main_thief_award_json_list == Json::Value::null || main_thief_award_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Main_Thief_Award");
	}

	for (uint i = 0; i < main_thief_award_json_list.size(); ++i) {
		const Json::Value& main_thief_award_json = main_thief_award_json_list[i];
		uint32_t player_level = (uint32_t)main_thief_award_json["player_level"].asUInt();
		uint8_t quality = (uint8_t)main_thief_award_json["quality"].asUInt();
		uint32_t award = (uint32_t)main_thief_award_json["award"].asUInt();
		add_main_thief_award_to_map(player_level, quality, award);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Main_Thief_Id() {
	const Json::Value& main_thief_id_json_list = CONFIG_INSTANCE->config_json()["dragon"]["main"]["thief_id"];
	if (main_thief_id_json_list == Json::Value::null || main_thief_id_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Main_Thief_Id");
	}

	for (uint i = 0; i < main_thief_id_json_list.size(); ++i) {
		const Json::Value& main_thief_id_json = main_thief_id_json_list[i];
		uint32_t id = (uint32_t)main_thief_id_json["id"].asUInt();
		dragon_vale_main_cfg.main_thief_id_set.insert(id);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache_Main_Match_Coin() {
	const Json::Value& main_foot_award_json_list = CONFIG_INSTANCE->config_json()["dragon"]["main"]["match_coin"];
	if (main_foot_award_json_list == Json::Value::null || main_foot_award_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Dragon_Vale: refresh_config_cache_Main_Match_Coin");
	}

	for (uint i = 0; i < main_foot_award_json_list.size(); ++i) {
		const Json::Value& main_star_up_json = main_foot_award_json_list[i];
		uint32_t match_num = (uint32_t)main_star_up_json["match_num"].asUInt();
		uint32_t coin = (uint32_t)main_star_up_json["coin"].asUInt();
		add_main_match_coin_to_map(match_num, coin);
	}
}

void Config_Cache_Dragon_Vale::refresh_config_cache() {
	PERF_MON("Config_Cache_Dragon_Vale");
	refresh_config_cache_Building_Level_Cfg();
	refresh_config_cache_Building_Role_Level_Cfg();
	refresh_config_cache_Building_Vip_Level_Cfg();
	refresh_config_cache_Building_Queue_Cfg();
	refresh_config_cache_Building_Quick_Upgrade_Cfg();
	refresh_config_cache_Building_Create_Cfg();
	refresh_config_cache_Dragon_Base_Cfg();
	refresh_config_cache_Dragon_Role_Level_Cfg();
	refresh_config_cache_Dragon_Keep_Cfg();
	refresh_config_cache_Dragon_Feeling_Cfg();
	refresh_config_cache_Dragon_Evolution_Cfg();
	refresh_config_cache_Dragon_Play_Dragon_Cfg();
	refresh_config_cache_Dragon_Play_Dragon_Get_Heart_Cfg();
	refresh_config_cache_Main_Base_Cfg();
	refresh_config_cache_Main_Star_Up_Speed();
	refresh_config_cache_Main_Match_Level();
	refresh_config_cache_Main_Foot_Award();
	refresh_config_cache_Main_Thief_Award();
	refresh_config_cache_Main_Thief_Id();
	refresh_config_cache_Main_Match_Coin();
}
