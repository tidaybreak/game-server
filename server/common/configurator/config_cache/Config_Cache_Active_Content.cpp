/*
 * Active_Config_Content.cpp
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 */

#include "Config_Cache_Active_Content.h"
#include "active/Active_Content_Config.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "fighter/Fighter_Def.h"

//const Limit_Buy_Conf_Map* Config_Cache_Active_Content::get_limit_buy_conf_map(void) const {
//	return &limit_buy_conf_map_;
//}

const Vip_Club_Conf_Map* Config_Cache_Active_Content::get_vip_club_conf_map(void) const {
	return &vip_club_conf_map_;
}

const Vip_Club_Config* Config_Cache_Active_Content::get_vip_club_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, vip_club_conf_map_);
}

const Level_Ace_Conf_Map* Config_Cache_Active_Content::get_level_ace_conf_map(void) const {
	return &level_ace_conf_map_;
}

const Level_Ace_Config* Config_Cache_Active_Content::get_level_ace_conf_by_level(uint32_t level) const {
	return get_map_second_pointer_by_key(level, level_ace_conf_map_);
}

const Sg_Pass_Conf_Map* Config_Cache_Active_Content::get_sg_pass_conf_map(void) const {
	return &sg_pass_conf_map_;
}

const Sg_Pass_Config* Config_Cache_Active_Content::get_sg_pass_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, sg_pass_conf_map_);
}

const Mount_Levelup_Conf_Map* Config_Cache_Active_Content::get_mount_pass_conf_map(void) const {
	return &mount_pass_conf_map_;
}
const Mount_Levelup_Config* Config_Cache_Active_Content::get_mount_pass_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, mount_pass_conf_map_);
}

const Wing_Levelup_Conf_Map* Config_Cache_Active_Content::get_wing_pass_conf_map(void) const {
	return &wing_pass_conf_map_;
}
const Wing_Levelup_Config* Config_Cache_Active_Content::get_wing_pass_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, wing_pass_conf_map_);
}

const Suit_Collect_Conf_Map* Config_Cache_Active_Content::get_suit_collect_conf_map(uint32_t career) {
	for(Suit_Collect_Conf_Map::iterator it = suit_collect_conf_map.begin(); it != suit_collect_conf_map.end(); ++it) {
		UInt_Set_Map::iterator iter = it->second.career_suit_collect_item.find(career);
		if(iter != it->second.career_suit_collect_item.end()) {
			it->second.suit_collect_item = iter->second;
		} else {
			it->second.suit_collect_item.clear();
		}
	}
	return &suit_collect_conf_map;
}

const Suit_Collect_Config* Config_Cache_Active_Content::get_suit_collect_conf_by_level(uint32_t level, uint32_t career) {
	for(Suit_Collect_Conf_Map::iterator it = suit_collect_conf_map.begin(); it != suit_collect_conf_map.end(); ++it) {
		UInt_Set_Map::iterator iter = it->second.career_suit_collect_item.find(career);
		if(iter != it->second.career_suit_collect_item.end()) {
			it->second.suit_collect_item = iter->second;
		} else {
			it->second.suit_collect_item.clear();
		}
	}
	return get_map_second_pointer_by_key(level, suit_collect_conf_map);
}

const Suit_Collect_Two_Conf_Map* Config_Cache_Active_Content::get_suit_collect_two_conf_map(uint32_t career) {
	for(Suit_Collect_Two_Conf_Map::iterator it = suit_collect_two_conf_map.begin(); it != suit_collect_two_conf_map.end(); ++it) {
		UInt_Set_Map::iterator iter = it->second.career_suit_collect_item.find(career);
		if(iter != it->second.career_suit_collect_item.end()) {
			it->second.suit_collect_item = iter->second;
		} else {
			it->second.suit_collect_item.clear();
		}
	}
	return &suit_collect_two_conf_map;
}

const Suit_Collect_Two_Config* Config_Cache_Active_Content::get_suit_collect_two_conf_by_level(uint32_t level, uint32_t career) {
	for(Suit_Collect_Two_Conf_Map::iterator it = suit_collect_two_conf_map.begin(); it != suit_collect_two_conf_map.end(); ++it) {
		UInt_Set_Map::iterator iter = it->second.career_suit_collect_item.find(career);
		if(iter != it->second.career_suit_collect_item.end()) {
			it->second.suit_collect_item = iter->second;
		} else {
			it->second.suit_collect_item.clear();
		}
	}
	return get_map_second_pointer_by_key(level, suit_collect_two_conf_map);
}

const Guild_Level_Conf_Map* Config_Cache_Active_Content::get_guild_level_conf_map(void) const {
	return &guild_level_conf_map_;
}

const Guild_Level_Config* Config_Cache_Active_Content::get_guild_level_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, guild_level_conf_map_);
}

const Gem_Levelup_Conf_Map* Config_Cache_Active_Content::get_gem_levelup_conf_map(void) const {
	return &gem_levelup_conf_map;
}

const Gem_Levelup_Config* Config_Cache_Active_Content::get_gem_levelup_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, gem_levelup_conf_map);
}

const Wing_Levelup_Conf_Map* Config_Cache_Active_Content::get_wing_levelup_conf_map(void) const {
	return &wing_levelup_conf_map;
}

const Wing_Levelup_Config* Config_Cache_Active_Content::get_wing_levelup_conf_by_level(uint32_t level) const {
	return get_map_second_pointer_by_key(level, wing_levelup_conf_map);
}

const Mount_Levelup_Conf_Map* Config_Cache_Active_Content::get_mount_levelup_conf_map(void) const {
	return &mount_levelup_conf_map;
}

const Mount_Levelup_Config* Config_Cache_Active_Content::get_mount_levelup_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, mount_levelup_conf_map);
}

const Arena_Dan_Conf_Map* Config_Cache_Active_Content::get_arena_dan_conf_map(void) const {
	return &arena_dan_conf_map;
}

const Arena_Dan_Config* Config_Cache_Active_Content::get_arena_dan_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, arena_dan_conf_map);
}

const World_Boss_Conf_Map* Config_Cache_Active_Content::get_world_boss_conf_map(void) const {
	return &world_boss_conf_map;
}

const World_Boss_Config* Config_Cache_Active_Content::get_world_boss_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, world_boss_conf_map);
}

const Heroes_Door_Conf_Map* Config_Cache_Active_Content::get_heroes_door_conf_map(void) const {
	return &heroes_door_conf_map;
}

const Heroes_Door_Config* Config_Cache_Active_Content::get_heroes_door_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, heroes_door_conf_map);
}

const Team_Arena_Conf_Map* Config_Cache_Active_Content::get_team_arena_conf_map(void) const {
	return &team_arena_conf_map;
}

const Team_Arena_Config* Config_Cache_Active_Content::get_team_arena_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, team_arena_conf_map);
}

const Warer_Conf_Map* Config_Cache_Active_Content::get_warer_conf_map(void) const {
	return &warer_conf_map;
}

const Warer_Config* Config_Cache_Active_Content::get_warer_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, warer_conf_map);
}

const Reward_Task_Conf_Map* Config_Cache_Active_Content::get_reward_task_conf_map(void) const {
	return &reward_task_conf_map;
}

const Active_Content_Reward_Task_Config* Config_Cache_Active_Content::get_reward_task_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, reward_task_conf_map);
}

const Sg_Pass_Conf_Map* Config_Cache_Active_Content::get_sg_fb_conf_map(void) const {
	return &sg_fb_conf_map;
}

const Sg_Pass_Config* Config_Cache_Active_Content::get_sg_fb_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, sg_fb_conf_map);
}

const Fm_X_Ci_Conf_Map* Config_Cache_Active_Content::get_fm_x_ci_conf_map(void) const {
	return &fm_x_ci_conf_map;
}

const Fm_X_Ci_Config* Config_Cache_Active_Content::get_fm_x_ci_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, fm_x_ci_conf_map);
}

const X_X_Quality_Heroer_Map* Config_Cache_Active_Content::get_x_x_quality_heroer_conf_map(void) const {
	return &x_x_quality_heroer_map;
}

const X_X_Quality_Heroer_Config* Config_Cache_Active_Content::get_x_x_quality_heroer_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, x_x_quality_heroer_map);
}

const Mount_Lvup_X_Conf_Map* Config_Cache_Active_Content::get_mount_lvup_x_conf_map(void) const {
	return &mount_lvup_x_conf_map;
}

const Mount_Lvup_X_Config* Config_Cache_Active_Content::get_mount_lvup_x_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, mount_lvup_x_conf_map);
}

const Fusion_X_Elite_Card_Conf_Map* Config_Cache_Active_Content::get_fusion_x_elite_card_conf_map(void) const {
	return &fusion_x_elite_card_conf_map;
}

const Fusion_X_Elite_Card_Config* Config_Cache_Active_Content::get_fusion_x_elite_card_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, fusion_x_elite_card_conf_map);
}

const Alchemy_Gift_Conf_Map* Config_Cache_Active_Content::get_alchemy_gift_conf_map(void) const {
	return &alchemy_gift_conf_map;
}

const Alchemy_Gift_Config* Config_Cache_Active_Content::get_alchemy_gift_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, alchemy_gift_conf_map);
}

const Activity_Gift_Conf_Map* Config_Cache_Active_Content::get_activity_gift_conf_map(void) const {
	return &activity_gift_conf_map;
}

const Activity_Gift_Config* Config_Cache_Active_Content::get_activity_gift_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, activity_gift_conf_map);
}

const Active_Ring_Task_Conf_Map* Config_Cache_Active_Content::get_ring_task_conf_map(void) const {
	return &ring_task_conf_map;
}

const Active_Ring_Task_Config* Config_Cache_Active_Content::get_ring_task_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, ring_task_conf_map);
}

const Dragon_Prey_Conf_Map* Config_Cache_Active_Content::get_dragon_prey_conf_map(void) const {
	return &dragon_prey_conf_map;
}

const Dragon_Prey_Config* Config_Cache_Active_Content::get_dragon_prey_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, dragon_prey_conf_map);
}

const Rank_Count_Config* Config_Cache_Active_Content::get_level_rank_count_conf_by_rank(uint32_t rank) const {
	UInt_UInt_Map::const_iterator riit = level_rank_id_map.find(rank);
	if(riit != level_rank_id_map.end()) {
		return get_map_second_pointer_by_key(riit->second, level_rank_conf_map_);
	}
	return 0;
}

const Rank_Count_Config* Config_Cache_Active_Content::get_arena_rank_count_conf_by_rank(uint32_t rank) const {
	UInt_UInt_Map::const_iterator riit = arena_rank_id_map.find(rank);
	if(riit != arena_rank_id_map.end()) {
		return get_map_second_pointer_by_key(riit->second, arena_rank_conf_map_);
	}
	return 0;
}

const Rank_Count_Config* Config_Cache_Active_Content::get_force_rank_count_conf_by_rank(uint32_t rank) const {
	UInt_UInt_Map::const_iterator riit = force_rank_id_map.find(rank);
	if(riit != force_rank_id_map.end()) {
		return get_map_second_pointer_by_key(riit->second, force_rank_conf_map_);
	}
	return 0;
}

const Rank_Count_Config* Config_Cache_Active_Content::get_sg_rank_count_conf_by_rank(uint32_t rank) const {
	UInt_UInt_Map::const_iterator riit = sg_rank_id_map.find(rank);
	if(riit != sg_rank_id_map.end()) {
		return get_map_second_pointer_by_key(riit->second, sg_rank_conf_map_);
	}
	return 0;
}

const Rank_Count_Config* Config_Cache_Active_Content::get_mount_rank_count_conf_by_rank(uint32_t rank) const {
	UInt_UInt_Map::const_iterator riit = mount_rank_id_map.find(rank);
	if(riit != mount_rank_id_map.end()) {
		return get_map_second_pointer_by_key(riit->second, mount_rank_conf_map_);
	}
	return 0;
}

const Rank_Count_Config* Config_Cache_Active_Content::get_wing_rank_count_conf_by_rank(uint32_t rank) const {
	UInt_UInt_Map::const_iterator riit = mount_rank_id_map.find(rank);
	if(riit != mount_rank_id_map.end()) {
		return get_map_second_pointer_by_key(riit->second, wing_rank_conf_map_);
	}
	return 0;
}

//const Limit_Buy_Config* Config_Cache_Active_Content::get_limit_buy_conf_by_id(uint32_t id) const {
//	return get_map_second_pointer_by_key(id, limit_buy_conf_map_);
//}

void Config_Cache_Active_Content::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Active_Content");
//	refresh_limit_buy_config_cache();
	refresh_vip_club_config_cache();
	refresh_level_ace_config_cache();
	refresh_suit_collect_config_cache();
	refresh_guild_level_config_cache();
	refresh_level_rank_config_cache();
	refresh_arena_rank_config_cache();
	refresh_force_rank_config_cache();
	refresh_sg_rank_config_cache();
	refresh_mount_rank_config_cache();
	refresh_wing_rank_config_cache();
	refresh_sg_pass_config_cache();
	refresh_wing_pass_config_cache();
	refresh_mount_pass_config_cache();
	refresh_gem_levelup_config_cache();
	refresh_wing_levelup_config_cache();
	refresh_mount_levelup_config_cache();
	refresh_arena_dan_config_cache();

	refresh_world_boss_config_cache();
	refresh_suit_collect_two_config_cache();
	refresh_heroes_door_config_cache();
	refresh_team_arena_config_cache();
	refresh_warer_config_cache();
	refresh_reward_task_config_cache();
	refresh_sg_fb_config_cache();

	refresh_fm_x_ci_config_cache();
	refresh_x_x_quality_heroe_config_cache();

	refresh_act_add_num_config_cache();
	refresh_mount_lvup_x_config_cache();
	refresh_funsion_x_elite_card_config_cache();
	refresh_alchemy_gift_config_cache();

	refresh_activity_gift_config_cache();
	refresh_ring_task_config_cache();
	refresh_dragon_prey_config_cache();

}

void Config_Cache_Active_Content::refresh_vip_club_config_cache(void) {
	vip_club_conf_map_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["vip_club"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: vip_club.json");
	}

	Vip_Club_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.vip_level = json["vip_level"].asInt();
		value.needs_num = json["needs_num"].asInt();

		value.reward.clear();
		const Json::Value& item_conf = json["reward"];
		if(item_conf != Json::Value::null && item_conf.size()) {

			Vip_Club_Item_Config item_value;
			for (uint j = 0; j < item_conf.size(); ++j) {
				const Json::Value& item_json = item_conf[j];
				item_value.reset();

				item_value.reward_id = item_json["reward_id"].asInt();
				item_value.item_id = item_json["item_id"].asInt();
				item_value.limit_vip_level = item_json["limit_vip_level"].asInt();
				item_value.num = item_json["num"].asInt();

				set_map_second_value_by_key(item_value.reward_id, value.reward, item_value);
			}
		}

		set_map_second_value_by_key(value.id, vip_club_conf_map_, value);
	}
}

void Config_Cache_Active_Content::refresh_level_ace_config_cache(void) {
	level_ace_conf_map_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["level_ace"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: level_ace.json");
	}
	Level_Ace_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.level = json["level"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.level, level_ace_conf_map_, value);
	}
}

void Config_Cache_Active_Content::refresh_sg_pass_config_cache(void) {
	sg_pass_conf_map_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["sg_pass"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
		//LOG_ABORT("not have Active content config: sg_pass.json");
	}
	Sg_Pass_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.sg_id = json["sg_id"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, sg_pass_conf_map_, value);
	}
}

void Config_Cache_Active_Content::refresh_mount_pass_config_cache(void) {
	mount_pass_conf_map_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["mount_pass"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
		//LOG_ABORT("not have Active content config: mount_pass.json");
	}
	Mount_Levelup_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.mount_id = json["mount_id"].asInt();
		value.level = json["level"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, mount_pass_conf_map_, value);
	}
}

void Config_Cache_Active_Content::refresh_wing_pass_config_cache(void) {
	wing_pass_conf_map_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["wing_pass"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
		//LOG_ABORT("not have Active content config: wing_pass.json");
	}
	Wing_Levelup_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		uint32_t id = json["id"].asInt();
		value.level = json["level"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(id, wing_pass_conf_map_, value);
	}
}

void Config_Cache_Active_Content::refresh_suit_collect_config_cache(void) {
	suit_collect_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["suit_collect"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: suit_collect.json");
	}
	Suit_Collect_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.level = json["level"].asInt();

		value.suit_collect_item.clear();
		value.career_suit_collect_item.clear();
		const Json::Value& item_conf = json["item"];
		if(item_conf != Json::Value::null && item_conf.size()) {
			for (uint j = 0; j < item_conf.size(); ++j) {
				uint32_t item_id = item_conf[j]["item_id"].asInt();
				uint32_t career = item_conf[j]["career"].asInt();
				UInt_Set_Map::iterator it = value.career_suit_collect_item.find(career);
				if(it != value.career_suit_collect_item.end()) {
					it->second.insert(item_id);
				} else {
					UInt_Set us;
					us.clear();
					us.insert(item_id);
					value.career_suit_collect_item.insert(std::make_pair(career, us));
				}
			}
		}

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.level, suit_collect_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_guild_level_config_cache(void) {
	guild_level_conf_map_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["guild_level"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: guild_level.json");
	}
	Guild_Level_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.level = json["level"].asInt();
		value.type = json["type"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, guild_level_conf_map_, value);
	}
}

void Config_Cache_Active_Content::refresh_gem_levelup_config_cache(void) {
	gem_levelup_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["gem_levelup"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: gem_levelup.json");
	}
	Gem_Levelup_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.level = json["level"].asInt();
		value.count = json["count"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, gem_levelup_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_wing_levelup_config_cache(void) {
	wing_levelup_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["wing_levelup"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: wing_levelup.json");
	}
	Wing_Levelup_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.level = json["level"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.level, wing_levelup_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_arena_dan_config_cache(void) {
	arena_dan_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["arena_dan"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		//LOG_ABORT("not have Active content config: mount_levelup.json");
		return;
	}
	Arena_Dan_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.group = json["group"].asInt();
		value.seg = json["seg"].asInt();
		value.time = json["time"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, arena_dan_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_mount_levelup_config_cache(void) {
	mount_levelup_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["mount_levelup"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: mount_levelup.json");
	}
	Mount_Levelup_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.mount_id = json["mount_id"].asInt();
		value.level = json["level"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, mount_levelup_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_level_rank_config_cache(void) {
	level_rank_conf_map_.clear();
	level_rank_id_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["level_rank"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: level_rank.json");
	}
	Rank_Count_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.start = json["start"].asInt();
		value.end = json["end"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, level_rank_conf_map_, value);

		for(uint32_t tmp = value.start; tmp <= value.end; ++tmp) {
			set_map_second_value_by_key(tmp, level_rank_id_map, value.id);
		}
	}
}

void Config_Cache_Active_Content::refresh_arena_rank_config_cache(void) {
	arena_rank_conf_map_.clear();
	arena_rank_id_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["arena_rank"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: arena_rank.json");
	}
	Rank_Count_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.start = json["start"].asInt();
		value.end = json["end"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, arena_rank_conf_map_, value);

		for(uint32_t tmp = value.start; tmp <= value.end; ++tmp) {
			set_map_second_value_by_key(tmp, arena_rank_id_map, value.id);
		}
	}
}

void Config_Cache_Active_Content::refresh_force_rank_config_cache(void) {
	force_rank_conf_map_.clear();
	force_rank_id_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["force_rank"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: force_rank.json");
	}
	Rank_Count_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.start = json["start"].asInt();
		value.end = json["end"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, force_rank_conf_map_, value);

		for(uint32_t tmp = value.start; tmp <= value.end; ++tmp) {
			set_map_second_value_by_key(tmp, force_rank_id_map, value.id);
		}
	}
}

void Config_Cache_Active_Content::refresh_sg_rank_config_cache(void) {
	sg_rank_conf_map_.clear();
	sg_rank_id_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["sg_rank"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: sg_rank.json");
	}
	Rank_Count_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.start = json["start"].asInt();
		value.end = json["end"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, sg_rank_conf_map_, value);

		for(uint32_t tmp = value.start; tmp <= value.end; ++tmp) {
			set_map_second_value_by_key(tmp, sg_rank_id_map, value.id);
		}
	}
}

void Config_Cache_Active_Content::refresh_mount_rank_config_cache(void) {
	mount_rank_conf_map_.clear();
	mount_rank_id_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["mount_rank"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: mount_rank.json");
	}
	Rank_Count_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.start = json["start"].asInt();
		value.end = json["end"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, mount_rank_conf_map_, value);

		for(uint32_t tmp = value.start; tmp <= value.end; ++tmp) {
			set_map_second_value_by_key(tmp, mount_rank_id_map, value.id);
		}
	}
}

void Config_Cache_Active_Content::refresh_wing_rank_config_cache(void) {
	wing_rank_conf_map_.clear();
	wing_rank_id_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["wing_rank"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: wing_rank.json");
	}
	Rank_Count_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.start = json["start"].asInt();
		value.end = json["end"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, wing_rank_conf_map_, value);

		for(uint32_t tmp = value.start; tmp <= value.end; ++tmp) {
			set_map_second_value_by_key(tmp, wing_rank_id_map, value.id);
		}
	}
}

void Config_Cache_Active_Content::refresh_active_reward_info_config(const Json::Value& item_conf,Active_Reward_Info_Config &info) {
	info.reset();
	if(item_conf != Json::Value::null && item_conf.size()) {
		for (uint i = 0; i < item_conf.size(); ++i) {
			const Json::Value& reward_item_json = item_conf[i];

			uint32_t item_id = reward_item_json["item_id"].asInt();
			uint32_t value_ = reward_item_json["num"].asInt();
			uint32_t career = reward_item_json["career"].asInt();

			if(item_id == PT_EXP_CURRENT) {
				info.exp += value_;
				info.career = career;
			} else {
				uint8_t type = reward_item_json["type"].asInt();
				switch(type) {
					case 1:{
						Active_Item_Config item;
						item.reset();
						item.item_id = item_id;
						item.num = value_;
						item.career = career;
						info.item.push_back(item);
						break;
					}
					case 2:{
						Active_Money_Config money;
						money.reset();
						money.money_id = item_id;
						money.num = value_;
						money.career = career;
						info.money.push_back(money);
						break;
					}
				}
			}

		}
	}
}

//void Config_Cache_Active_Content::refresh_limit_buy_config_cache(void) {
//	limit_buy_conf_map_.clear();
//	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["limit_buy"];
//	if (config_json == Json::Value::null || config_json.size() == 0) {
//		LOG_ABORT("not have Active content config: limit_buy.json");
//	}
//
//	Limit_Buy_Config value;
//	for (uint i = 0; i < config_json.size(); ++i) {
//		const Json::Value& json = config_json[i];
//		value.reset();
//
//		value.id = json["id"].asInt();
//		value.item_id = json["item_id"].asInt();
//		value.price = json["price"].asInt();
//		value.pro_price = json["pro_price"].asInt();
//		value.start_time = json["start_time"].asInt();
//		value.continue_time = json["continue_time"].asInt();
//		value.num = json["num"].asInt();
//
//		set_map_second_value_by_key(value.id, limit_buy_conf_map_, value);
//	}
//}

void Config_Cache_Active_Content::refresh_world_boss_config_cache(void){
	world_boss_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["world_boss"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: world_boss.json");
		//return;
	}
	World_Boss_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.type = json["type"].asInt();
		value.val = json["val"].asInt();
		value.sval = json["sval"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, world_boss_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_suit_collect_two_config_cache(void){
	suit_collect_two_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["suit_collect_two"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: suit_collect_two.json");
		//return;
	}
	Suit_Collect_Two_Config value;

	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.act_id = json["act_id"].asInt();

		value.suit_collect_item.clear();
		value.career_suit_collect_item.clear();
		const Json::Value& item_conf = json["item"];
		if(item_conf != Json::Value::null && item_conf.size()) {
			for (uint j = 0; j < item_conf.size(); ++j) {
				uint32_t item_id = item_conf[j]["item_id"].asInt();
				uint32_t career = item_conf[j]["career"].asInt();
				UInt_Set_Map::iterator it = value.career_suit_collect_item.find(career);
				if(it != value.career_suit_collect_item.end()) {
					it->second.insert(item_id);
				} else {
					UInt_Set us;
					us.clear();
					us.insert(item_id);
					value.career_suit_collect_item.insert(std::make_pair(career, us));
				}
			}
		}

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, suit_collect_two_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_heroes_door_config_cache(void){
	heroes_door_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["heroes_door"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: heroes_door.json");
		//return;
	}
	Heroes_Door_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.type = json["type"].asInt();
		value.level = json["level"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, heroes_door_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_team_arena_config_cache(void){
	team_arena_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["team_arena"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: team_arena.json");
		//return;
	}
	Team_Arena_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, team_arena_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_warer_config_cache(void){
	warer_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["warer"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: warer.json");
		//return;
	}
	Warer_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, warer_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_reward_task_config_cache(void) {
	reward_task_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["reward_task"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: reward_task.json");
		//return;
	}
	Active_Content_Reward_Task_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, reward_task_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_sg_fb_config_cache(void) {
	sg_fb_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["sg_fb"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active content config: sg_fb.json");
		//return;
	}
	Sg_Pass_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.sg_id = json["sg_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, sg_fb_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_fm_x_ci_config_cache(void) {
	fm_x_ci_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["enchanting_x_num"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		//LOG_ABORT("not have Active content config: enchanting_x_num.json");
		return;
	}
	Fm_X_Ci_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, fm_x_ci_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_x_x_quality_heroe_config_cache(void) {
	x_x_quality_heroer_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["x_x_quality_heroer"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		//LOG_ABORT("not have Active content config: enchanting_x_num.json");
		return;
	}
	X_X_Quality_Heroer_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.type = json["type"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, x_x_quality_heroer_map, value);
	}
}

void Config_Cache_Active_Content::refresh_act_add_num_config_cache(void) {
	act_add_num_conf.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["act_add_num"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		//LOG_ABORT("not have Active content config: sg_fb.json");
		return;
	}
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		uint32_t act_id = json["act_id"].asInt();
		uint32_t day = json["day"].asInt();
		uint32_t num = json["num"].asInt();

		act_add_num_conf[act_id][day] = num;
	}
}

void Config_Cache_Active_Content::refresh_mount_lvup_x_config_cache(void) {
	mount_lvup_x_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["mount_lvup_x"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		//LOG_ABORT("not have Active content config: enchanting_x_num.json");
		return;
	}
	Mount_Lvup_X_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, mount_lvup_x_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_funsion_x_elite_card_config_cache(void) {
	fusion_x_elite_card_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["fusion_x_elite_card"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		//LOG_ABORT("not have Active content config: enchanting_x_num.json");
		return;
	}
	Fusion_X_Elite_Card_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, fusion_x_elite_card_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_alchemy_gift_config_cache(void) {
	alchemy_gift_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["alchemy_gift"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		//LOG_ABORT("not have Active content config: enchanting_x_num.json");
		return;
	}
	Alchemy_Gift_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, alchemy_gift_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_activity_gift_config_cache(void) {
	activity_gift_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["activity_gift"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
	}
	Activity_Gift_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, activity_gift_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_ring_task_config_cache(void) {
	ring_task_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["ring_task"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
	}
	Active_Ring_Task_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();
		value.recharge = json["recharge"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, ring_task_conf_map, value);
	}
}

void Config_Cache_Active_Content::refresh_dragon_prey_config_cache(void) {
	dragon_prey_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["active_content"]["dragon_prey"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
	}
	Dragon_Prey_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.num = json["num"].asInt();
		value.act_id = json["act_id"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward_info);

		set_map_second_value_by_key(value.id, dragon_prey_conf_map, value);
	}
}

uint32_t Config_Cache_Active_Content::get_act_add_num_config_by_act_id_day(uint32_t act_id, uint32_t day) {
	if(act_add_num_conf.find(act_id) != act_add_num_conf.end()) {
		if(act_add_num_conf[act_id].find(day) != act_add_num_conf[act_id].end()) {
			return act_add_num_conf[act_id][day];
		}
	}
	return 0;
}
