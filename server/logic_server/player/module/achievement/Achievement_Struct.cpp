/*
 * Achievement_Struct.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: Linqiyou
 */

#include "Achievement_Struct.h"
#include "DB_Operator.h"


Achievement_Detail::Achievement_Detail(void) {
	reset();
}
Achievement_Detail::~Achievement_Detail(void) {}
void Achievement_Detail::reset(void) {
	role_id = 0;
	achievement_count = 0;
	achievement_info_map.clear();
	complete_ids.clear();
	suit_purple_ids.clear();
	suit_gold_ids.clear();
	rune_lvl_his = 0;
	unban_lvl_his = 0;
	master_lvl_his = 0;
	force_lvl_his = 0;
	war_kill_nums_his = 0;
	arena_win_nums_his = 0;
	gang_war_win_nums_his = 0;
//	first_lvl_role_id_his.clear();
//	first_lvl_ck_his.clear();
//	first_lvl_zs_his.clear();
//	first_lvl_yss_his.clear();
	kill_monster_nums_his = 0;
	team_fb_complete_nums_his.clear();
	horse_get_his.clear();
	secret_shop_buy_nums_his = 0;
	dragon_wale_rob_times_his = 0;
	dragon_wale_step_foot_times_his = 0;
	dragon_wale_play_dragon_times_his = 0;
	hero_awake_lvl_his = 0;
	task_complete_nums_his = 0;
	dragon_wale_gold_mine_lvl_his = 0;
	dragon_wale_moon_well_lvl_his = 0;
	dragon_wale_totem_lvl_his = 0;
	dragon_wale_drive_thief_times_his = 0;
	war_light_win_nums_his = 0;
	war_dark_win_nums_his = 0;
	task_limit_time_his = -1;
	dragon_wale_keep_level_his = 0;
	kill_boss_his.clear();
	rune_stone_lvl_his = 0;
	arena_lvl_his = 0;
	first_boss_against = 0;
	complete_suit.clear();
}

int Achievement_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int64(achievement_count);
	buffer.write_uint16(achievement_info_map.size());
	for (Achievement_Info_Map::const_iterator it = achievement_info_map.begin(); it != achievement_info_map.end(); ++it) {
		buffer.write_int32(it->second.id);
		buffer.write_int8(it->second.state_count);
		buffer.write_int8(it->second.state_award);
		buffer.write_int32(it->second.data);
	}
	buffer.write_uint16(complete_ids.size());
	for (Int_Hash_Set::const_iterator it = complete_ids.begin(); it != complete_ids.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_uint16(suit_purple_ids.size());
	for (Int_Hash_Set::const_iterator it = suit_purple_ids.begin(); it != suit_purple_ids.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_uint16(suit_gold_ids.size());
	for (Int_Hash_Set::const_iterator it = suit_gold_ids.begin(); it != suit_gold_ids.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_int32(rune_lvl_his);
	buffer.write_int32(unban_lvl_his);
	buffer.write_int32(master_lvl_his);
	buffer.write_int32(force_lvl_his);

	buffer.write_int32(war_kill_nums_his);
	buffer.write_int32(arena_win_nums_his);
	buffer.write_int32(gang_war_win_nums_his);
//	buffer.write_uint16(first_lvl_role_id_his.size());
//	for (Int_Uid::const_iterator it = first_lvl_role_id_his.begin(); it != first_lvl_role_id_his.end(); ++it) {
//		buffer.write_int32(it->first);
//		buffer.write_int64(it->second);
//	}
//	buffer.write_uint16(first_lvl_ck_his.size());
//	for (Int_Uid::const_iterator it = first_lvl_ck_his.begin(); it != first_lvl_ck_his.end(); ++it) {
//		buffer.write_int32(it->first);
//		buffer.write_int64(it->second);
//	}
//	buffer.write_uint16(first_lvl_zs_his.size());
//	for (Int_Uid::const_iterator it = first_lvl_zs_his.begin(); it != first_lvl_zs_his.end(); ++it) {
//		buffer.write_int32(it->first);
//		buffer.write_int64(it->second);
//	}
//	buffer.write_uint16(first_lvl_yss_his.size());
//	for (Int_Uid::const_iterator it = first_lvl_yss_his.begin(); it != first_lvl_yss_his.end(); ++it) {
//		buffer.write_int32(it->first);
//		buffer.write_int64(it->second);
//	}
	buffer.write_int32(kill_monster_nums_his);
	buffer.write_uint16(team_fb_complete_nums_his.size());
	for (Int_Hash_Set::const_iterator it = team_fb_complete_nums_his.begin(); it != team_fb_complete_nums_his.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_uint16(horse_get_his.size());
	for (Int_Hash_Set::const_iterator it = horse_get_his.begin(); it != horse_get_his.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_int32(secret_shop_buy_nums_his);
	buffer.write_int32(dragon_wale_rob_times_his);
	buffer.write_int32(dragon_wale_step_foot_times_his);
	buffer.write_int32(dragon_wale_play_dragon_times_his);
	buffer.write_int32(hero_awake_lvl_his);
	buffer.write_int32(task_complete_nums_his);
	buffer.write_int32(dragon_wale_gold_mine_lvl_his);
	buffer.write_int32(dragon_wale_moon_well_lvl_his);
	buffer.write_int32(dragon_wale_totem_lvl_his);
	buffer.write_int32(dragon_wale_drive_thief_times_his);
	buffer.write_int32(war_light_win_nums_his);
	buffer.write_int32(war_dark_win_nums_his);
	buffer.write_int32(task_limit_time_his);
	buffer.write_uint16(kill_boss_his.size());
	for (Int_Hash_Set::const_iterator it = kill_boss_his.begin(); it != kill_boss_his.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_int32(dragon_wale_keep_level_his);
	buffer.write_int32(rune_stone_lvl_his);
	buffer.write_int32(arena_lvl_his);
	buffer.write_int32(first_boss_against);
	buffer.write_uint16(complete_suit.size());
	for (Int_Hash_Set::const_iterator it = complete_suit.begin(); it != complete_suit.end(); ++it) {
		buffer.write_int32(*it);
	}

	return 0;
}
int Achievement_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_int64(achievement_count);
	uint16_t len = 0;
	buffer.read_uint16(len);
	achievement_info_map.clear();
	Achievement_Info info;
	for (uint16_t i = 0; i < len; ++i) {
		info.reset();
		buffer.read_int32(info.id);
		buffer.read_int8(info.state_count);
		buffer.read_int8(info.state_award);
		buffer.read_int32(info.data);
		achievement_info_map.insert(std::make_pair(info.id, info));
	}
	buffer.read_uint16(len);
	complete_ids.clear();
	int32_t complete_id = 0;
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(complete_id);
		complete_ids.insert(complete_id);
	}
	buffer.read_uint16(len);
	suit_purple_ids.clear();
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(complete_id);
		suit_purple_ids.insert(complete_id);
	}
	suit_gold_ids.clear();
	buffer.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(complete_id);
		suit_gold_ids.insert(complete_id);
	}
	buffer.read_int32(rune_lvl_his);
	buffer.read_int32(unban_lvl_his);
	buffer.read_int32(master_lvl_his);
	buffer.read_int32(force_lvl_his);

	buffer.read_int32(war_kill_nums_his);
	buffer.read_int32(arena_win_nums_his);
	buffer.read_int32(gang_war_win_nums_his);
//	role_id_t uid = 0;
//	first_lvl_role_id_his.clear();
//	for (uint16_t i = 0; i < len; ++i) {
//		buffer.read_int32(complete_id);
//		buffer.read_int64(uid);
//		first_lvl_role_id_his.insert(std::make_pair(complete_id, uid));
//	}
//	first_lvl_ck_his.clear();
//	for (uint16_t i = 0; i < len; ++i) {
//		buffer.read_int32(complete_id);
//		buffer.read_int64(uid);
//		first_lvl_ck_his.insert(std::make_pair(complete_id, uid));
//	}
//	first_lvl_zs_his.clear();
//	for (uint16_t i = 0; i < len; ++i) {
//		buffer.read_int32(complete_id);
//		buffer.read_int64(uid);
//		first_lvl_zs_his.insert(std::make_pair(complete_id, uid));
//	}
//	first_lvl_yss_his.clear();
//	for (uint16_t i = 0; i < len; ++i) {
//		buffer.read_int32(complete_id);
//		buffer.read_int64(uid);
//		first_lvl_yss_his.insert(std::make_pair(complete_id, uid));
//	}
	buffer.read_int32(kill_monster_nums_his);
	team_fb_complete_nums_his.clear();
	buffer.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(complete_id);
		team_fb_complete_nums_his.insert(complete_id);
	}
	horse_get_his.clear();
	buffer.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(complete_id);
		horse_get_his.insert(complete_id);
	}
	buffer.read_int32(secret_shop_buy_nums_his);
	buffer.read_int32(dragon_wale_rob_times_his);
	buffer.read_int32(dragon_wale_step_foot_times_his);
	buffer.read_int32(dragon_wale_play_dragon_times_his);
	buffer.read_int32(hero_awake_lvl_his);
	buffer.read_int32(task_complete_nums_his);
	buffer.read_int32(dragon_wale_gold_mine_lvl_his);
	buffer.read_int32(dragon_wale_moon_well_lvl_his);
	buffer.read_int32(dragon_wale_totem_lvl_his);
	buffer.read_int32(dragon_wale_drive_thief_times_his);
	buffer.read_int32(war_light_win_nums_his);
	buffer.read_int32(war_dark_win_nums_his);
	buffer.read_int32(task_limit_time_his);
	kill_boss_his.clear();
	buffer.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(complete_id);
		kill_boss_his.insert(complete_id);
	}
	buffer.read_int32(dragon_wale_keep_level_his);
	buffer.read_int32(rune_stone_lvl_his);
	buffer.read_int32(arena_lvl_his);
	buffer.read_int32(first_boss_against);
	complete_suit.clear();
	buffer.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(complete_id);
		complete_suit.insert(complete_id);
	}
	return 0;
}
int Achievement_Detail::load(void) {
	return MONGO_OPERATOR->load_achievement_detail(*this);
}
int Achievement_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_achievement_detail(*this);
	else
		return 0;
}
