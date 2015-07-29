/*
 * Dragon_Vale_Struct.h
 *
 *  Created on: 2014年7月11日
 *      Author: gaojianshan
 */

#include "Dragon_Vale_Struct.h"
#include "DB_Operator.h"
#include "Config_Cache_Dragon_Vale.h"

int DragonBuildingInfo::serialize(Block_Buffer &buffer) const {
	buffer.write_uint32(id);
	buffer.write_uint8(type);
	buffer.write_uint16(level);
	buffer.write_uint8(status);
	buffer.write_uint32(production);
	buffer.write_bool(is_gain);
	buffer.write_uint64((uint64_t)begin_upgrade_time.sec());
	buffer.write_uint64((uint64_t)last_output_production_time.sec());
	buffer.write_uint8(order);
	buffer.write_uint8(consume_type);
	return 0;
}

int DragonBuildingInfo::deserialize(Block_Buffer &buffer) {
	buffer.read_uint32(id);
	buffer.read_uint8(type);
	buffer.read_uint16(level);
	buffer.read_uint8(status);
	buffer.read_uint32(production);
	buffer.read_bool(is_gain);

	uint64_t  temp_begin_upgrade_time_sec = 0;
	uint64_t temp_last_output_production_time_sec = 0;
	buffer.read_uint64(temp_begin_upgrade_time_sec);
	buffer.read_uint64(temp_last_output_production_time_sec);

	begin_upgrade_time.sec((long)temp_begin_upgrade_time_sec);
	last_output_production_time.sec((long)temp_last_output_production_time_sec);

	buffer.read_uint8(order);
	buffer.read_uint8(consume_type);
	return 0;
}

int DragonInfo::serialize(Block_Buffer &buffer) const {
	buffer.write_uint8(progress);
	buffer.write_uint8(feeling);
	buffer.write_uint32(power);
	buffer.write_uint32(armor);
	buffer.write_uint32(stamina);
	buffer.write_uint32(power_level);
	buffer.write_uint32(armor_level);
	buffer.write_uint32(stamina_level);
	buffer.write_uint32(hero_power);
	buffer.write_uint32(hero_armor);
	buffer.write_uint32(hero_stamina);
	buffer.write_uint32(hero_power_level);
	buffer.write_uint32(hero_armor_level);
	buffer.write_uint32(hero_stamina_level);
	buffer.write_bool(is_play_dragon);
	buffer.write_uint32(energy);
	buffer.write_uint32(crit);
	buffer.write_uint32(prevent_crit);
	buffer.write_uint32(crit_level);
	buffer.write_uint32(prevent_crit_level);
	buffer.write_uint32(hero_crit);
	buffer.write_uint32(hero_prevent_crit);
	buffer.write_uint32(hero_crit_level);
	buffer.write_uint32(hero_prevent_crit_level);
	return 0;
}

int DragonInfo::deserialize(Block_Buffer &buffer) {
	buffer.read_uint8(progress);
	buffer.read_uint8(feeling);
	buffer.read_uint32(power);
	buffer.read_uint32(armor);
	buffer.read_uint32(stamina);
	buffer.read_uint32(power_level);
	buffer.read_uint32(armor_level);
	buffer.read_uint32(stamina_level);
	buffer.read_uint32(hero_power);
	buffer.read_uint32(hero_armor);
	buffer.read_uint32(hero_stamina);
	buffer.read_uint32(hero_power_level);
	buffer.read_uint32(hero_armor_level);
	buffer.read_uint32(hero_stamina_level);
	buffer.read_bool(is_play_dragon);
	buffer.read_uint32(energy);
	buffer.read_uint32(crit);
	buffer.read_uint32(prevent_crit);
	buffer.read_uint32(crit_level);
	buffer.read_uint32(prevent_crit_level);
	buffer.read_uint32(hero_crit);
	buffer.read_uint32(hero_prevent_crit);
	buffer.read_uint32(hero_crit_level);
	buffer.read_uint32(hero_prevent_crit_level);
	return 0;
}

void Dragon_Vale_Detail::reset(void) {
	role_id = 0;
	target_player_id = 0;
	is_rob_enter_dragon_vale = false;
	uuid = 0;
	is_gold_mine_max_storage = false;
	is_moon_well_max_storage = false;
	is_totem_max_storage = false;
	build_gold_mine_num = 0;
	build_moon_well_num = 0;
	build_totem_num = 0;
	build_queue_num = 0;
	all_open_build_queue_num = 0;
	dragon_building_info_map.clear();

	cancel_create_building_set.clear();

	is_dragon_born = false;
	dragon_info.reset();

	is_vip_down_to_zero = false;
	matched_num = 1;
	thief_id_quality_map.clear();

	star_num = 0;
	award_foot_num = 0;
	is_special = 0;
	foot_info_map.clear();
	foot_id_set.clear();
	is_yesterday_login = false;
	dragon_heart_num = 0;

	notice_board_list.clear();

	is_rob_protect = false;
	is_first_enter_after_robbed = false;
	is_in_robbed_dragon_vale = false;

	is_dragon_vale_open = false;
	left_time = 0;
	play_dragon_num = 0;
	drive_thief_num = 0;

	to_uuid = 0;
	to_role_id = 0;
	to_server_name = "";
	to_role_name = "";
	coin = 0;
}

Dragon_Vale_Detail::Dragon_Vale_Detail(void) {
	reset();
}

Dragon_Vale_Detail::~Dragon_Vale_Detail() {
}

int Dragon_Vale_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int64(target_player_id);
	buffer.write_bool(is_rob_enter_dragon_vale);
	buffer.write_int64(uuid);
	buffer.write_bool(is_gold_mine_max_storage);
	buffer.write_bool(is_moon_well_max_storage);
	buffer.write_bool(is_totem_max_storage);
	buffer.write_uint8(build_gold_mine_num);
	buffer.write_uint8(build_moon_well_num);
	buffer.write_uint8(build_totem_num);
	buffer.write_uint8(build_queue_num);
	buffer.write_uint8(all_open_build_queue_num);

	uint8_t size = dragon_building_info_map.size();
	buffer.write_uint8(size);

	for(DragonBuildingInfoMap::const_iterator it = dragon_building_info_map.begin();
			it != dragon_building_info_map.end();
			++it) {
		it->second.serialize(buffer);
	}

	buffer.write_bool(is_dragon_born);
	buffer.write_uint64((uint64_t)begin_born_time.sec());

	dragon_info.serialize(buffer);

	buffer.write_uint64((uint64_t)begin_play_dragon_interval_time.sec());
	buffer.write_uint64((uint64_t)begin_down_feeling_time.sec());

	buffer.write_bool(is_vip_down_to_zero);
	buffer.write_uint32(matched_num);

	size = thief_id_quality_map.size();
	buffer.write_uint8(size);
	for(Thief_Id_Quality_Map::const_iterator it = thief_id_quality_map.begin(); it != thief_id_quality_map.end(); ++it) {
		buffer.write_int64(it->first);
		buffer.write_int64(it->second);
	}

	buffer.write_uint64((uint64_t)begin_refresh_thief_time.sec());

	buffer.write_uint8(star_num);
	buffer.write_uint8(award_foot_num);
	buffer.write_bool(is_special);

	size = foot_info_map.size();
	buffer.write_uint8(size);
	for(Foot_Info_Map::const_iterator it = foot_info_map.begin(); it != foot_info_map.end(); ++it) {
		buffer.write_int64(it->first);
		buffer.write_uint8(it->second.sex);
		buffer.write_string(it->second.name);
	}

	uint32_t size_32 = foot_id_set.size();
	buffer.write_uint32(size_32);
	for(Foot_Id_Set::const_iterator it = foot_id_set.begin(); it != foot_id_set.end(); ++it) {
		buffer.write_int64(*it);
	}

	buffer.write_bool(is_yesterday_login);
	buffer.write_uint8(dragon_heart_num);

	size = notice_board_list.size();
	buffer.write_uint8(size);
	for(Notice_Board_List::const_iterator it = notice_board_list.begin();
			it != notice_board_list.end();
			++it) {
		it->serialize(buffer);
	}

	buffer.write_bool(is_rob_protect);
	buffer.write_uint64((uint64_t)begin_rob_protect_time.sec());

	buffer.write_bool(is_first_enter_after_robbed);

	buffer.write_bool(is_in_robbed_dragon_vale);
	buffer.write_uint64((uint64_t)begin_in_robbed_dragon_vale_time.sec());

	size = cancel_create_building_set.size();
	buffer.write_uint8(size);
	for(Cancel_Create_Building_Set::const_iterator it = cancel_create_building_set.begin();
			it != cancel_create_building_set.end();
			++it) {
		buffer.write_uint8(*it);
	}

	buffer.write_bool(is_dragon_vale_open);

	buffer.write_uint64((uint64_t)begin_no_enter_time.sec());
	buffer.write_uint32(left_time);

	buffer.write_uint32(play_dragon_num);
	buffer.write_uint32(drive_thief_num);

	return 0;
}

int Dragon_Vale_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_int64(target_player_id);
	buffer.read_bool(is_rob_enter_dragon_vale);
	buffer.read_int64(uuid);
	buffer.read_bool(is_gold_mine_max_storage);
	buffer.read_bool(is_moon_well_max_storage);
	buffer.read_bool(is_totem_max_storage);
	buffer.read_uint8(build_gold_mine_num);
	buffer.read_uint8(build_moon_well_num);
	buffer.read_uint8(build_totem_num);
	buffer.read_uint8(build_queue_num);
	buffer.read_uint8(all_open_build_queue_num);

	uint8_t size = 0;
	buffer.read_uint8(size);

	dragon_building_info_map.clear();
	uint64_t key = 0;

	DragonBuildingInfo building_info;
	for(uint8_t i = 0; i < size; ++i) {
		building_info.reset();
		building_info.deserialize(buffer);
		key = create_key_of_Dragon_Building_Info(building_info.id, building_info.type);
		dragon_building_info_map.insert(std::make_pair(key, building_info));
	}

	buffer.read_bool(is_dragon_born);

	uint64_t temp_begin_born_time_sec = 0;
	buffer.read_uint64(temp_begin_born_time_sec);
	begin_born_time.sec((long)temp_begin_born_time_sec);

	dragon_info.deserialize(buffer);

	uint64_t temp_begin_play_dragon_interval_time_sec = 0;
	buffer.read_uint64(temp_begin_play_dragon_interval_time_sec);
	begin_play_dragon_interval_time.sec((long)temp_begin_play_dragon_interval_time_sec);

	uint64_t temp_begin_down_feeling_time_sec = 0;
	buffer.read_uint64(temp_begin_down_feeling_time_sec);
	begin_down_feeling_time.sec((long)temp_begin_down_feeling_time_sec);

	buffer.read_bool(is_vip_down_to_zero);
	buffer.read_uint32(matched_num);

	buffer.read_uint8(size);
	for(uint8_t i = 0; i < size; ++i) {
		int64_t key = 0;
		int64_t value = 0;
		buffer.read_int64(key);
		buffer.read_int64(value);
		thief_id_quality_map.insert(std::make_pair(key, value));
	}

	uint64_t temp_begin_refresh_thief_time = 0;
	buffer.read_uint64(temp_begin_refresh_thief_time);
	begin_refresh_thief_time.sec((long)temp_begin_refresh_thief_time);

	buffer.read_uint8(star_num);
	buffer.read_uint8(award_foot_num);
	buffer.read_bool(is_special);

	buffer.read_uint8(size);
	for(uint8_t i = 0; i < size; ++i) {
		int64_t key = 0;
		buffer.read_int64(key);
		Foot_Info foot_info;
		foot_info.reset();
		buffer.read_uint8(foot_info.sex);
		buffer.read_string(foot_info.name);
		foot_info_map.insert(std::make_pair(key, foot_info));
	}

	uint32_t size_32 = 0;
	buffer.read_uint32(size_32);
	for(uint32_t i = 0; i < size_32; ++i) {
		int64_t role_id = 0;
		buffer.read_int64(role_id);
		foot_id_set.insert(role_id);
	}

	buffer.read_bool(is_yesterday_login);
	buffer.read_uint8(dragon_heart_num);

	buffer.read_uint8(size);
	Notice_Board notice_board;
	for(uint8_t i = 0; i < size; ++i) {
		notice_board.reset();
		notice_board.deserialize(buffer);
		notice_board_list.push_back(notice_board);
	}

	buffer.read_bool(is_rob_protect);

	uint64_t temp_begin_rob_protect_time = 0;
	buffer.read_uint64(temp_begin_rob_protect_time);
	begin_rob_protect_time.sec((long)temp_begin_rob_protect_time);

	buffer.read_bool(is_first_enter_after_robbed);

	buffer.read_bool(is_in_robbed_dragon_vale);
	uint64_t temp_begin_in_robbed_dragon_vale_time = 0;
	buffer.read_uint64(temp_begin_in_robbed_dragon_vale_time);
	begin_in_robbed_dragon_vale_time.sec((long)temp_begin_in_robbed_dragon_vale_time);

	buffer.read_uint8(size);
	for(uint8_t i = 0; i < size; ++i) {
		uint8_t order = 0;
		buffer.read_uint8(order);
		cancel_create_building_set.insert(order);
	}

	buffer.read_bool(is_dragon_vale_open);

	uint64_t temp_begin_no_enter_time = 0;
	buffer.read_uint64(temp_begin_no_enter_time);
	begin_no_enter_time.sec((long)temp_begin_no_enter_time);

	buffer.read_uint32(left_time);
	buffer.read_uint32(play_dragon_num);
	buffer.read_uint32(drive_thief_num);

	return 0;
}

int Dragon_Vale_Detail::load(void) {
	return MONGO_OPERATOR->load_dragon_vale_detail(*this);
}

int Dragon_Vale_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_dragon_vale_detail(*this);
	return 0;
}

void Dragon_Vale_Detail::get_building_max_storage(uint32_t &gold_mine_max_storage, uint32_t &moon_weel_max_storage,
			uint32_t &totem_max_storage, uint32_t &gold_mine_production, uint32_t &moon_weel_production,
			uint32_t &totem_production, bool is_rob /*= false*/) {
	const Building_Level_Cfg *build_level_cfg = 0;

	for(DragonBuildingInfoMap::iterator it = this->dragon_building_info_map.begin();
			it != this->dragon_building_info_map.end();
			++it) {
		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, it->second.level);
		if(!build_level_cfg) {
			continue;
		}

		if(BUILDING_GOLD_MINE == it->second.type) {
			gold_mine_max_storage += build_level_cfg->max_storage;
			if(!is_rob || (is_rob && it->second.status != BUILDING_UPGRADE_DOING)) {
				gold_mine_production += it->second.production;
			}

			continue;
		}

		if(BUILDING_MOON_WELL == it->second.type) {
			moon_weel_max_storage += build_level_cfg->max_storage;
			if(!is_rob || (is_rob && it->second.status != BUILDING_UPGRADE_DOING)) {
				moon_weel_production += it->second.production;
			}

			continue;
		}

		if(BUILDING_TOTEM == it->second.type) {
			totem_max_storage += build_level_cfg->max_storage;
			if(!is_rob || (is_rob && it->second.status != BUILDING_UPGRADE_DOING)) {
				totem_production += it->second.production;
			}

			continue;
		}
	}
}

bool Dragon_Vale_Detail::is_has_building_upgrading() {
	for(DragonBuildingInfoMap::const_iterator it = dragon_building_info_map.begin();
			it != dragon_building_info_map.end();
			++it) {
		if(it->second.status == BUILDING_UPGRADE_DOING) {
			return true;
		}
	}

	return false;
}

void Dragon_Vale_Detail::check_and_change_notice_board_num() {
	while(notice_board_list.size() >= 50) {
		notice_board_list.pop_front();
	}
}

bool Dragon_Vale_Detail::is_has_building_finish() {
	for(DragonBuildingInfoMap::const_iterator it = dragon_building_info_map.begin();
			it != dragon_building_info_map.end();
			++it) {
		if(it->second.level > 0) {
			return true;
		}
	}

	return false;
}

