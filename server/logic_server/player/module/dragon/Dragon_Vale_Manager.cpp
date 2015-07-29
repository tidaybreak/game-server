/*
 * Dragon_Vale_Manager.cpp
 *
 *  Created on: 2014年7月26日
 *      Author: gaojianshan
 */
#include <stdlib.h>
#include <vector>
#include "Dragon_Vale_Manager.h"
#include "Logic_Monitor.h"
#include "Logic_Player_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Dragon_Vale_Struct.h"
#include "Err_Code.h"
#include "Config_Cache.h"
#include "Logic_Player.h"
#include "heroer/Logic_Heroer.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "Config_Cache_Dragon_Vale.h"
#include "Game_Typedef.h"
#include "pool_manager/Pool_Manager.h"
#include "DB_Def.h"
#include "Msg_Inner_Enum.h"
#include "Err_Code.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "Msg_Dragon_Vale_Enum.h"
#include "Time_Value.h"
#include "Config_Cache_NPC.h"
#include "helper/Helper_Def.h"
#include "gang/Gang_Manager.h"
#include "Config_Cache_Scene.h"
#include "scene/Scene_Def.h"
#include <sstream>


void Dragon_Vale_Manager::reset() {
	dragon_vale_role_id_map.clear();
	logic_rob_production_map.clear();
	battle_role_id_set.clear();
}

void Dragon_Vale_Manager::tick(const Time_Value &now) {
	Logic_Player *player = 0;
	DB_View::ID_Player_View_Map::iterator find_it = LOGIC_MONITOR->db_view()->id_player_view_map.end();

	for(Dragon_Vale_Role_Id_Map::iterator it = dragon_vale_role_id_map.begin();
			it != dragon_vale_role_id_map.end(); ++it) {
		player = LOGIC_MONITOR->find_logic_player_by_role_id(it->first);
		if(player) {
			continue;
		}

		find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->first);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			continue;
		}

		if(!find_it->second) {
			continue;
		}

		process_offline_tick(*(find_it->second), now);
	}
}

int Dragon_Vale_Manager::match_level(uint32_t level, uint32_t &begin_level, uint32_t &end_level) {
	begin_level = 0;
	end_level = 0;
	const Main_Match_Level_Map &main_match_level_map = CONFIG_CACHE_DRAGON_VALE->get_main_config().main_match_level_map;
	for(Main_Match_Level_Map::const_iterator it = main_match_level_map.begin();
			it != main_match_level_map.end();
			++it) {
		if(level >= it->first && level <= it->second) {
			begin_level = it->first;
			end_level = it->second;
		}
	}

	if(!begin_level && !end_level) {
		return -1;//no match level
	}
	return 0;
}

int Dragon_Vale_Manager::process_rob_match(int64_t uuid, int64_t role_id, uint32_t level, uint32_t coin,
					std::string server_name, std::string role_name, uint32_t last_coin) {
	DB_View *db_view = LOGIC_MONITOR->db_view();
	if(!db_view) {
		MSG_USER("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
		MSG_USER("DRAGON_VALE_ROB: db_view is null");
		MSG_USER("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

		//error notice
		{
			MSG_82300027 msg_82300027;
			msg_82300027.error_id = ERROR_ROB_MATCH_FAIL;
			msg_82300027.coin = last_coin;
			Block_Buffer buf;
			buf.make_message(ACTIVE_ROB_MATCH_FAIL);
			msg_82300027.serialize(buf);
			buf.finish_message();

			Block_Buffer scene_buf;
			scene_buf.make_message(INNER_DRAGON_VALE_ERROR_NOTICE);
			scene_buf.write_int64(role_id);
			scene_buf.copy(&buf);
			scene_buf.finish_message();

			const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, scene_buf);
		}

		//back coin message
		{
			MSG_20600020 msg_20600020;
			msg_20600020.uuid = uuid;
			msg_20600020.role_id = role_id;
			msg_20600020.coin = last_coin;
			msg_20600020.type = 1;
			Block_Buffer buf;
			buf.make_message(INNER_ROB_MATCH_FAIL_BACK_COIN);
			msg_20600020.serialize(buf);
			buf.finish_message();

			const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		}

		return ERROR_INNER_SERVER;
	}

	std::vector<role_id_t> role_id_vector;
	std::list<role_id_t> lower_list;
	std::list<role_id_t> taller_list;
	role_id_vector.clear();
	lower_list.clear();
	taller_list.clear();

	const Main_Base_Cfg&& main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	uint32_t temp = 0;
	uint32_t gold_mine_production = 0;
	uint32_t moon_weel_production = 0;
	uint32_t totem_production = 0;
	uint32_t rob_gold_mine = 0;
	uint32_t rob_moon_well = 0;
	uint32_t rob_totem = 0;
	uint32_t begin_level = 0;
	uint32_t end_level = 0;

	for(DB_View::ID_Player_View_Map::const_iterator it = db_view->id_player_view_map.begin();
			it != db_view->id_player_view_map.end();
			++it) {
		gold_mine_production = 0;
		moon_weel_production = 0;
		totem_production = 0;
		rob_gold_mine = 0;
		rob_moon_well = 0;
		rob_totem = 0;
		begin_level = 0;
		end_level = 0;

		if(!it->second) {
			continue;
		}

		if(it->second->role_id == role_id) {
			continue;
		}

		if(it->second->dragon_vale_view.is_rob_protect) {
			continue;
		}

		if(match_level(level, begin_level, end_level) != 0) {
			break;
		}

		it->second->dragon_vale_view.get_building_max_storage(temp, temp,
				temp, gold_mine_production, moon_weel_production,
				totem_production, true);

		rob_gold_mine  = (uint32_t)(gold_mine_production * main_base_cfg.rob_percent  * 1.0 / 100);
		rob_moon_well = (uint32_t)(moon_weel_production * main_base_cfg.rob_percent * 1.0 / 100);
		rob_totem = (uint32_t)(totem_production * main_base_cfg.rob_percent * 1.0 / 100);

		if(0 == rob_gold_mine && 0 == rob_moon_well && 0 == rob_totem) {
			continue;
		}

		if(it->second->level < (int)begin_level || it->second->level > (int)end_level) {
			if(it->second->level < (int)begin_level) {
				lower_list.push_back(it->first);
			}
			else {
				taller_list.push_back(it->first);
			}

			continue;
		}

		role_id_vector.push_back(it->first);
	}//end for


	if(role_id_vector.empty() && !lower_list.empty()) {
		for(std::list<role_id_t>::const_iterator lower_it = lower_list.begin(); lower_it != lower_list.end(); ++lower_it) {
			role_id_vector.push_back(*lower_it);
		}
	}

	if(role_id_vector.empty() && !taller_list.empty()) {
		for(std::list<role_id_t>::const_iterator taller_it = taller_list.begin(); taller_it != taller_list.end(); ++taller_it) {
			role_id_vector.push_back(*taller_it);
		}
	}

	if(role_id_vector.empty()) {
		MSG_DEBUG("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		MSG_DEBUG("DRAGON_VALE_ROB: role_id_vector is empty");
		MSG_DEBUG("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		lower_list.clear();
		taller_list.clear();

		//error notice
		{
			MSG_82300027 msg_82300027;
			msg_82300027.error_id = ERROR_ROB_MATCH_FAIL;
			msg_82300027.coin = last_coin;
			Block_Buffer buf;
			buf.make_message(ACTIVE_ROB_MATCH_FAIL);
			msg_82300027.serialize(buf);
			buf.finish_message();

			Block_Buffer scene_buf;
			scene_buf.make_message(INNER_DRAGON_VALE_ERROR_NOTICE);
			scene_buf.write_int64(role_id);
			scene_buf.copy(&buf);
			scene_buf.finish_message();

			const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, scene_buf);
		}

		//back coin message
		{
			MSG_20600020 msg_20600020;
			msg_20600020.uuid = uuid;
			msg_20600020.role_id = role_id;
			msg_20600020.coin = last_coin;
			msg_20600020.type = 1;
			Block_Buffer buf;
			buf.make_message(INNER_ROB_MATCH_FAIL_BACK_COIN);
			msg_20600020.serialize(buf);
			buf.finish_message();

			const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		}

		return 0;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint32_t index = random() % role_id_vector.size();
	role_id_t player_id = role_id_vector[index];

	MSG_20600002 msg;
	msg.to_uuid = uuid;
	msg.to_role_id = role_id;
	msg.to_server_name = server_name;
	msg.to_role_name = role_name;
	msg.uuid = CONFIG_CACHE->server_flag();
	msg.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	msg.role_id = player_id;

	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(player_id);
	if(!player) {
		Block_Buffer buf;
		buf.make_message(MONGO_LOAD_DRAGON_VALE_ROB_MATCH);
		buf.write_int64(uuid);
		buf.write_int64(role_id);
		buf.write_string(server_name);
		buf.write_string(role_name);
		buf.write_int64(player_id);
		buf.write_uint32(coin);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, 0);

		role_id_vector.clear();
		lower_list.clear();
		taller_list.clear();
		return 0;
	}

	Rob_Match_Member role;
	role.name = player->role_name();
	role.level = player->level();
	role.career = (uint8_t)player->career();
	role.gender = (uint8_t)player->gender();
	msg.fight = player->fighter_detail().force;

	if(player->heroer_detail().fighter_index) {
		Heroer_Detail::Hero_Map::const_iterator it_hero =
				player->heroer_detail().hero_map.find(player->heroer_detail().fighter_index);
		if(it_hero !=  player->heroer_detail().hero_map.end()) {
			Rob_Match_Member hero;
			hero.name = it_hero->second.hero_name;
			hero.level = it_hero->second.level;
			hero.hero_id = it_hero->second.hero_id;
			int hero_index = player->heroer_get_outer_hero_formation();
			if(hero_index == 5) {
				msg.match_member_list.push_back(hero);
				msg.match_member_list.push_back(role);
			} else {
				msg.match_member_list.push_back(role);
				msg.match_member_list.push_back(hero);
			}
			msg.fight += it_hero->second.hero_fighter_detail.force;
		} else {
			msg.match_member_list.push_back(role);
		}
	} else {
		msg.match_member_list.push_back(role);
	}

	player->get_building_max_storage(temp, temp,
			temp, gold_mine_production, moon_weel_production,
			totem_production, true);

	msg.rob_gold_mine  = (uint32_t)(gold_mine_production * main_base_cfg.rob_percent  * 1.0 / 100);
	msg.rob_moon_well = (uint32_t)(moon_weel_production * main_base_cfg.rob_percent * 1.0 / 100);
	msg.rob_totem = (uint32_t)(totem_production * main_base_cfg.rob_percent * 1.0 / 100);
	msg.coin = coin;

	Block_Buffer buf;
	buf.make_message(INNER_RES_ROB_MATCH);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	role_id_vector.clear();
	lower_list.clear();
	taller_list.clear();
	return 0;
}

void Dragon_Vale_Manager::process_rob_match(Logic_Player_Detail* logic_player_detail) {
	if(!logic_player_detail) {
		return;
	}

	if(logic_player_detail->status != Logic_Player_Detail::SUCCESS_LOADED) {


		POOL_MANAGER->push_player_data_pool(logic_player_detail, 15);
		return;
	}

	MSG_20600002 msg;
	msg.to_uuid = logic_player_detail->dragon_vale_detail.to_uuid;
	msg.to_role_id = logic_player_detail->dragon_vale_detail.to_role_id;
	msg.to_server_name = logic_player_detail->dragon_vale_detail.to_server_name;
	msg.to_role_name = logic_player_detail->dragon_vale_detail.to_role_name;
	msg.uuid = CONFIG_CACHE->server_flag();
	msg.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	msg.role_id = logic_player_detail->base_detail.role_id;

	Rob_Match_Member role;
	role.name = logic_player_detail->base_detail.role_name;
	role.level = logic_player_detail->base_detail.level;
	role.career = logic_player_detail->base_detail.career;
	role.gender = logic_player_detail->base_detail.gender;
	msg.fight = logic_player_detail->fighter_detail.force;

	if(logic_player_detail->heroer_detail.fighter_index) {
		Heroer_Detail::Hero_Map::const_iterator it_hero =
				logic_player_detail->heroer_detail.hero_map.find(logic_player_detail->heroer_detail.fighter_index);
		if(it_hero != logic_player_detail->heroer_detail.hero_map.end()) {
			Rob_Match_Member hero;
			hero.name = it_hero->second.hero_name;
			hero.level = it_hero->second.level;
		    hero.hero_id = it_hero->second.hero_id;
			if(it_hero->second.formation == 5) {
				msg.match_member_list.push_back(hero);
				msg.match_member_list.push_back(role);
			} else {
				msg.match_member_list.push_back(role);
				msg.match_member_list.push_back(hero);
			}
			msg.fight += it_hero->second.hero_fighter_detail.force;
		} else {
			msg.match_member_list.push_back(role);
		}

	} else {
		msg.match_member_list.push_back(role);
	}//end if

	uint32_t gold_mine_max_storage = 0;
	uint32_t moon_weel_max_storage = 0;
    uint32_t totem_max_storage = 0;
	uint32_t gold_mine_production = 0;
	uint32_t moon_weel_production = 0;
	uint32_t totem_production = 0;

	logic_player_detail->dragon_vale_detail.get_building_max_storage(gold_mine_max_storage, moon_weel_max_storage,
								totem_max_storage, gold_mine_production,
								moon_weel_production, totem_production, true);

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	msg.rob_gold_mine  = (uint32_t)(gold_mine_production * main_base_cfg.rob_percent  * 1.0 / 100);
	msg.rob_moon_well = (uint32_t)(moon_weel_production * main_base_cfg.rob_percent * 1.0 / 100);
	msg.rob_totem = (uint32_t)(totem_production * main_base_cfg.rob_percent * 1.0 / 100);
	msg.coin = logic_player_detail->dragon_vale_detail.coin;

	Block_Buffer buf;
	buf.make_message(INNER_RES_ROB_MATCH);
    msg.serialize(buf);
    buf.finish_message();
    LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

    POOL_MANAGER->push_player_data_pool(logic_player_detail, 16);
}

int Dragon_Vale_Manager::process_rob_fight_load_player_data(int64_t to_uuid, int64_t to_role_id,
										std::string to_server_name, std::string to_role_name,
										int64_t role_id) {
	Block_Buffer buf;
	buf.make_message(MONGO_LOAD_DRAGON_VALE_ROB_FIGHT);
	buf.write_int64(to_uuid);
	buf.write_int64(to_role_id);
	buf.write_string(to_server_name);
	buf.write_string(to_role_name);
	buf.write_int64(role_id);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

void Dragon_Vale_Manager::process_rob_fight_load_player_data(Logic_Player_Detail* logic_player_detail) {
	if(!logic_player_detail) {
		return;
	}

	if(logic_player_detail->status != Logic_Player_Detail::SUCCESS_LOADED) {
		POOL_MANAGER->push_player_data_pool(logic_player_detail, 13);
		return;
	}

	Logic_Player *player = POOL_MANAGER->pop_logic_player();
	if (! player) {
		MSG_USER("logic_player_pool_.pop() return 0.");
		return;
	}
	player->load_base_detail(logic_player_detail->base_detail);
	player->Logic_Fighter::load_detail(logic_player_detail->fighter_detail);
	player->Logic_Heroer::load_detail(logic_player_detail->heroer_detail);
	player->Dragon_Vale::load_detail(logic_player_detail->dragon_vale_detail);

	Block_Buffer buf;
	buf.make_message(INNER_RES_DRAGON_VALE_ROB_FIGHT_LOAD);
	int64_t uuid = CONFIG_CACHE->server_flag();
	std::string server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	buf.write_int64(logic_player_detail->dragon_vale_detail.to_uuid);
	buf.write_int64(logic_player_detail->dragon_vale_detail.to_role_id);
	buf.write_string(logic_player_detail->dragon_vale_detail.to_server_name);
	buf.write_string(logic_player_detail->dragon_vale_detail.to_role_name);
	buf.write_int64(uuid);
	buf.write_int64(logic_player_detail->base_detail.role_id);
	buf.write_string(server_name);
	buf.write_string(logic_player_detail->base_detail.role_name);

	int8_t value;
	value = player->career();
	buf.write_int8(value);

	value = player->gender();
	buf.write_int8(value);

	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(logic_player_detail->base_detail.role_id);
	if(find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		buf.write_uint32(find_it->second->avatar_vec.size());
		for(Int_Vec::const_iterator it = find_it->second->avatar_vec.begin(); it != find_it->second->avatar_vec.end(); ++it) {
			buf.write_int32(*it);
		}
	}

	// 人物战斗信息
	player->fighter_detail().serialize(buf);

	{
		// 战斗英雄信息
		bool has_fight_hero = false;

		if(logic_player_detail->heroer_detail.fighter_index) {
			Heroer_Detail::Hero_Map::const_iterator it_hero =
					logic_player_detail->heroer_detail.hero_map.find(logic_player_detail->heroer_detail.fighter_index);
			if(it_hero != logic_player_detail->heroer_detail.hero_map.end()) {
				has_fight_hero = true;
				buf.write_int8(1);
				player->refresh_heroer_fight_detail();
				player->heroer_detail().fight_hero_base_.serialize(buf);
				player->heroer_skill_refresh(it_hero->second.hero_index, false, false, false);
				it_hero->second.hero_fighter_detail.serialize(buf);
			}
		}

		if (!has_fight_hero) {
			buf.write_int8(0);
		}
	}

	buf.finish_message();

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
    LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	POOL_MANAGER->push_logic_player(player);
	POOL_MANAGER->push_player_data_pool(logic_player_detail, 14);
}

void Dragon_Vale_Manager::porocss_rob_enter_dragon_vale(Logic_Player &player) {
	if(!player.dragon_vale_detail().is_rob_enter_dragon_vale || !player.dragon_vale_detail().uuid) {
		MSG_USER("rob enter dragon vale error, player_id=%ld, target_player_id=%ld, "
				       "is_rob_dragon_vale=%d, uuid=%ld", player.role_id(), player.dragon_vale_detail().target_player_id,
				       player.dragon_vale_detail().is_rob_enter_dragon_vale ? 1 :0,
				       player.dragon_vale_detail().uuid);
		return;
	}

	Time_Value now = Time_Value::gettimeofday();
	if(!player.dragon_vale_detail().is_in_robbed_dragon_vale) {
		player.dragon_vale_detail_no_const().is_in_robbed_dragon_vale = true;
		player.dragon_vale_detail_no_const().begin_in_robbed_dragon_vale_time.set(now);
		player.save_dragon_vale_tick();
		player.sync_dragon_vale_to_db_view();
	}

	MSG_20600007 msg_20600007;
	msg_20600007.to_uuid = player.monitor_link().logic_muid.uuid;
	msg_20600007.to_role_id = player.role_id();
	msg_20600007.uuid = player.dragon_vale_detail().uuid;
	msg_20600007.role_id = player.dragon_vale_detail().target_player_id;

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	long total_time = player.dragon_vale_detail().begin_in_robbed_dragon_vale_time.sec()
			 	 	 + main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;
	msg_20600007.limit_time = (uint8_t)((total_time > now.sec()) ? total_time - now.sec() : 0);

	Block_Buffer buf;
	buf.make_message(INNNER_ROB_ENTER_DRAGON_VALE);
	msg_20600007.serialize(buf);
    buf.finish_message();
    LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
}

void Dragon_Vale_Manager::process_rob_enter_dragon_vale(int64_t to_uuid, int64_t to_role_id, int64_t role_id, uint8_t limit_time) {
	process_enter_dragon_vale(role_id, to_role_id);

	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		process_rob_enter_offline_player_dragon_vale(to_uuid, to_role_id, role_id, limit_time);
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	Time_Value now = Time_Value::gettimeofday();

	MSG_20600008 msg_20600008;
	msg_20600008.to_uuid = to_uuid;
	msg_20600008.to_role_id = to_role_id;
	msg_20600008.limit_time = limit_time;
	msg_20600008.uuid = player->monitor_link().logic_muid.uuid;

	DragonBuildingInfoMap temp;
	temp.clear();
	temp.insert(player->dragon_vale_detail().dragon_building_info_map.begin(),
				player->dragon_vale_detail().dragon_building_info_map.end());

	Building_Info building;
	for(DragonBuildingInfoMap::iterator it = temp.begin(); it != temp.end(); ++it) {
		building.reset();
		building.id = it->second.id;
		building.type = it->second.type;
		building.level = it->second.level;
		building.status = it->second.status;
		building.production = it->second.production;
		uint32_t rob_production = (uint32_t)(ceil(it->second.production * main_base_cfg.rob_percent * 1.0 / 100));

		if(it->second.status == BUILDING_UPGRADE_DOING) {
			building.is_gain = 0;
		}
		else {
			building.is_gain = rob_production > 0 ? 1 : 0;
		}

		building.order = it->second.order;
		building.consume_type = it->second.consume_type;
		msg_20600008.builing_info_list.push_back(building);
	}
	temp.clear();

	msg_20600008.head_info.role_id = player->role_id();
	msg_20600008.head_info.role_name = player->role_name();
	msg_20600008.head_info.star_num = player->dragon_vale_detail().star_num;
	msg_20600008.head_info.head_photo_id = player->friend_detail().head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(player->dragon_vale_detail().star_num);
	if(!star_add_speed) {
		msg_20600008.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", player->dragon_vale_detail().star_num);
	}
	else {
		msg_20600008.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_20600008.head_info.thief_sub_speed =
		(uint16_t)(player->dragon_vale_detail().thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = player->dragon_vale_detail().foot_info_map.begin();
			it != player->dragon_vale_detail().foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_20600008.head_info.foot_list.push_back(msg_foot_info);
	}

	msg_20600008.is_dragon_born = player->dragon_vale_detail().is_dragon_born ? 1 : 0;

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = player->dragon_vale_detail_no_const().begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;
	msg_20600008.born_time = (uint32_t)((total_time > now.sec()) ? (total_time - now.sec()) : 0);
	msg_20600008.progress = player->dragon_vale_detail_no_const().dragon_info.progress;
	msg_20600008.is_paly_dragon = 0;  //掠夺者进入被掠夺者的跨服龙谷，不让其逗龙，只能收获资源

	uint32_t gold_mine_max_storage = 0;
	uint32_t moon_weel_max_storage = 0;
	uint32_t totem_max_storage = 0;
	uint32_t gold_mine_production = 0;
	uint32_t moon_weel_production = 0;
	uint32_t totem_production = 0;

	player->get_building_max_storage(gold_mine_max_storage, moon_weel_max_storage,
									 totem_max_storage, gold_mine_production,
									 moon_weel_production, totem_production, true);
	msg_20600008.total_gold_mine = (uint32_t)(gold_mine_production * main_base_cfg.rob_percent / 100.0);
	msg_20600008.total_moon_weel = (uint32_t)(moon_weel_production * main_base_cfg.rob_percent / 100.0);
	msg_20600008.total_totem = (uint32_t)(totem_production * main_base_cfg.rob_percent / 100.0);

	Block_Buffer buf;
	buf.make_message(INNNER_ROB_ENTER_DRAGON_VALE_RES);
	msg_20600008.serialize(buf);
    buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
}

void Dragon_Vale_Manager::process_rob_enter_offline_player_dragon_vale(int64_t to_uuid, int64_t to_role_id,
															int64_t role_id, uint8_t limit_time) {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();
	process_offline_tick(*(find_it->second), now);

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	MSG_20600008 msg_20600008;
	msg_20600008.to_uuid = to_uuid;
	msg_20600008.to_role_id = to_role_id;
	msg_20600008.limit_time = limit_time;
	msg_20600008.uuid = CONFIG_CACHE->server_flag();

	DragonBuildingInfoMap temp;
	temp.clear();
	temp.insert(find_it->second->dragon_vale_view.dragon_building_info_map.begin(),
			find_it->second->dragon_vale_view.dragon_building_info_map.end());

	Building_Info building;
	for(DragonBuildingInfoMap::iterator it = temp.begin(); it != temp.end(); ++it) {
		building.reset();
		building.id = it->second.id;
		building.type = it->second.type;
		building.level = it->second.level;
		building.status = it->second.status;
		building.production = it->second.production;
		uint32_t rob_production = (uint32_t)(ceil(it->second.production * main_base_cfg.rob_percent * 1.0 / 100));

		if(it->second.status == BUILDING_UPGRADE_DOING) {
			building.is_gain = 0;
		}
		else {
			building.is_gain = rob_production > 0 ? 1 : 0;
		}

		building.order = it->second.order;
		building.consume_type = it->second.consume_type;
		msg_20600008.builing_info_list.push_back(building);
	}
	temp.clear();

	msg_20600008.head_info.role_id =  find_it->second->role_id;
	msg_20600008.head_info.role_name =  find_it->second->role_name;
	msg_20600008.head_info.star_num = find_it->second->dragon_vale_view.star_num;
	msg_20600008.head_info.head_photo_id = find_it->second->friend_.head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(find_it->second->dragon_vale_view.star_num);
	if(!star_add_speed) {
		msg_20600008.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", find_it->second->dragon_vale_view.star_num);
	}
	else {
		msg_20600008.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_20600008.head_info.thief_sub_speed =
		(uint16_t)(find_it->second->dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = find_it->second->dragon_vale_view.foot_info_map.begin();
			it != find_it->second->dragon_vale_view.foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_20600008.head_info.foot_list.push_back(msg_foot_info);
	}

	msg_20600008.is_dragon_born = find_it->second->dragon_vale_view.is_dragon_born ? 1 : 0;

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = find_it->second->dragon_vale_view.begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;
	msg_20600008.born_time = (uint32_t)((total_time > now.sec()) ? (total_time - now.sec()) : 0);
	msg_20600008.progress = find_it->second->dragon_vale_view.dragon_info.progress;
	msg_20600008.is_paly_dragon = 0;  //掠夺者进入被掠夺者的跨服龙谷，不让其逗龙，只能收获资源

	uint32_t gold_mine_max_storage = 0;
	uint32_t moon_weel_max_storage = 0;
	uint32_t totem_max_storage = 0;
	uint32_t gold_mine_production = 0;
	uint32_t moon_weel_production = 0;
	uint32_t totem_production = 0;

	find_it->second->dragon_vale_view.get_building_max_storage(gold_mine_max_storage, moon_weel_max_storage,
									 totem_max_storage, gold_mine_production,
									 moon_weel_production, totem_production, true);
	msg_20600008.total_gold_mine = (uint32_t)(gold_mine_production * main_base_cfg.rob_percent / 100.0);
	msg_20600008.total_moon_weel = (uint32_t)(moon_weel_production * main_base_cfg.rob_percent / 100.0);
	msg_20600008.total_totem = (uint32_t)(totem_production * main_base_cfg.rob_percent / 100.0);

	Block_Buffer buf;
	buf.make_message(INNNER_ROB_ENTER_DRAGON_VALE_RES);
	msg_20600008.serialize(buf);
    buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
}

void Dragon_Vale_Manager::process_gain_rob_production(int64_t to_uuid, int64_t to_role_id,
								std::string to_server_name,std::string to_role_name,
								int64_t uuid, int64_t role_id,
								uint32_t building_id, uint8_t building_type) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		process_gain_rob_offline_player_production(to_uuid, to_role_id, to_server_name, to_role_name,
										uuid, role_id, building_id, building_type);
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	uint64_t key = create_key_of_Dragon_Building_Info(building_id, building_type);
	DragonBuildingInfoMap::iterator it = player->dragon_vale_detail_no_const().dragon_building_info_map.find(key);
	if(it == player->dragon_vale_detail_no_const().dragon_building_info_map.end()) {
		MSG_20600010 msg_20600010;
		msg_20600010.error_id = ERROR_BUILDING_NOT_EXIST;
		msg_20600010.to_uuid = to_uuid;
		msg_20600010.to_role_id = to_role_id;
		msg_20600010.uuid = uuid;
		msg_20600010.role_id = role_id;

		Block_Buffer buf;
		buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
		msg_20600010.serialize(buf);
	    buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}

	const Building_Level_Cfg *build_level_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(building_type, it->second.level);
	if(!build_level_cfg) {
		MSG_20600010 msg_20600010;
		msg_20600010.error_id = ERROR_BUILDING_LEVEL_CFG_NOT_EXIST;
		msg_20600010.to_uuid = to_uuid;
		msg_20600010.to_role_id = to_role_id;
		msg_20600010.uuid = uuid;
		msg_20600010.role_id = role_id;

		Block_Buffer buf;
		buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
		msg_20600010.serialize(buf);
	    buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}

	if(BUILDING_UPGRADE_DOING == it->second.status) {
		MSG_20600010 msg_20600010;
		msg_20600010.error_id = ERROR_BUDING_UPGRADING_NO_ROB;
		msg_20600010.to_uuid = to_uuid;
		msg_20600010.to_role_id = to_role_id;
		msg_20600010.uuid = uuid;
		msg_20600010.role_id = role_id;

		Block_Buffer buf;
		buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
		msg_20600010.serialize(buf);
	    buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}

	if(it->second.production == 0) {
		MSG_20600010 msg_20600010;
		msg_20600010.error_id = ERROR_NO_PRODUCTION_NO_ROB;
		msg_20600010.to_uuid = to_uuid;
		msg_20600010.to_role_id = to_role_id;
		msg_20600010.uuid = uuid;
		msg_20600010.role_id = role_id;

		Block_Buffer buf;
		buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
		msg_20600010.serialize(buf);
	    buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}

	Logic_Rob_Production_Map::iterator rob_map_it = logic_rob_production_map.begin();
	if(rob_map_it == logic_rob_production_map.end()) {
		Logic_Rob_Production logic_rob_production;
		logic_rob_production.role_id = role_id;
		logic_rob_production.id = it->second.id;
		logic_rob_production.type = it->second.type;
		Logic_Rob_Production_Set logic_set;
		logic_set.insert(logic_rob_production);
		logic_rob_production_map.insert(std::make_pair(to_role_id, logic_set));
	}
	else {
		bool is_find = false;
		for(Logic_Rob_Production_Set::const_iterator rob_set_it = rob_map_it->second.begin();
				rob_set_it != rob_map_it->second.end();
				++rob_set_it) {
			if(rob_set_it->role_id == role_id && rob_set_it->id == it->second.id && rob_set_it->type == it->second.type) {
				is_find = true;
				break;
			}
		}

		if(!is_find) {
			Logic_Rob_Production rob_production;
			rob_production.role_id = role_id;
			rob_production.id = it->second.id;
			rob_production.type = it->second.type;
			rob_map_it->second.insert(rob_production);
		}
		else {
			MSG_20600010 msg_20600010;
			msg_20600010.error_id = ERROR_NO_REPEAT_ROB_SAME_BUILDING;
			msg_20600010.to_uuid = to_uuid;
			msg_20600010.to_role_id = to_role_id;
			msg_20600010.uuid = uuid;
			msg_20600010.role_id = role_id;

			Block_Buffer buf;
			buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
			msg_20600010.serialize(buf);
		    buf.finish_message();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
			return;
		}
	}//end else

	uint32_t rob_production = (uint32_t)(ceil(it->second.production * main_base_cfg.rob_percent * 1.0 / 100));

	//重置上一次建筑资源产出时间
	Time_Value now = Time_Value::gettimeofday();
	it->second.last_output_production_time.set(now);

	MSG_20600010 msg_20600010;
	msg_20600010.to_uuid = to_uuid;
	msg_20600010.to_role_id = to_role_id;
	msg_20600010.to_server_name = to_server_name;
	msg_20600010.to_role_name = to_role_name;
	msg_20600010.uuid = uuid;
	msg_20600010.role_id = role_id;
	msg_20600010.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	msg_20600010.role_name = player->role_name();
	msg_20600010.gain_production = rob_production;

	it->second.production =
			(it->second.production >=  rob_production) ? (it->second.production - rob_production) : 0;
	it->second.is_gain = it->second.production >= build_level_cfg->min_gain_limit;

	if(BUILDING_GOLD_MINE == it->second.type) {
		player->dragon_vale_detail_no_const().is_gold_mine_max_storage = false;
	}

	if(BUILDING_MOON_WELL == it->second.type) {
		player->dragon_vale_detail_no_const().is_moon_well_max_storage = false;
	}

	if(BUILDING_TOTEM == it->second.type) {
		player->dragon_vale_detail_no_const().is_totem_max_storage = false;
	}

	msg_20600010.building_info.id = it->second.id;
	msg_20600010.building_info.type = it->second.type;
	msg_20600010.building_info.level = it->second.level;
	msg_20600010.building_info.status = it->second.status;
	msg_20600010.building_info.production = it->second.production;
	msg_20600010.building_info.is_gain = 0; //note:(1)只能掠夺收获一次  (2)掠夺时，龙谷主人不能进龙谷，不能收获
	msg_20600010.building_info.order = it->second.order;
	msg_20600010.building_info.consume_type = it->second.consume_type;

	Block_Buffer buf;
	buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
	msg_20600010.serialize(buf);
    buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	//boradcast to dragon vale scene
	MSG_50180018  msg_50180018;
	msg_50180018.to_uuid = msg_20600010.to_uuid;
	msg_50180018.to_role_id = msg_20600010.to_role_id;
	msg_50180018.uuid = msg_20600010.uuid;
	msg_50180018.role_id = msg_20600010.role_id;
	msg_50180018.gain_production = msg_20600010.gain_production;
	msg_50180018.building_info.id = msg_20600010.building_info.id;
	msg_50180018.building_info.type = msg_20600010.building_info.type;
	msg_50180018.building_info.level = msg_20600010.building_info.level;
	msg_50180018.building_info.status = msg_20600010.building_info.status;
	msg_50180018.building_info.production = msg_20600010.building_info.production;
	msg_50180018.building_info.is_gain = msg_20600010.building_info.is_gain;
	msg_50180018.building_info.left_time = msg_20600010.building_info.left_time;
	msg_50180018.building_info.order = msg_20600010.building_info.order;

	Block_Buffer buf_50180018;
	buf_50180018.make_message(msg_50180018.msg_id);
	msg_50180018.serialize(buf_50180018);
	buf_50180018.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player->role_id());
	broad_cast_buf.copy(&buf_50180018);
	broad_cast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	((Dragon_Vale*)player)->save_dragon_vale_tick();
	player->sync_dragon_vale_to_db_view();
}

void Dragon_Vale_Manager::process_gain_rob_offline_player_production(int64_t to_uuid, int64_t to_role_id,
								std::string to_server_name, std::string to_role_name,
								int64_t uuid, int64_t role_id, uint32_t building_id, uint8_t building_type) {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	uint64_t key = create_key_of_Dragon_Building_Info(building_id, building_type);
	DragonBuildingInfoMap::iterator it = find_it->second->dragon_vale_view.dragon_building_info_map.find(key);
	if(it == find_it->second->dragon_vale_view.dragon_building_info_map.end()) {
		MSG_20600010 msg_20600010;
		msg_20600010.error_id = ERROR_BUILDING_NOT_EXIST;
		msg_20600010.to_uuid = to_uuid;
		msg_20600010.to_role_id = to_role_id;
		msg_20600010.uuid = uuid;
		msg_20600010.role_id = role_id;

		Block_Buffer buf;
		buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
		msg_20600010.serialize(buf);
	    buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}

	const Building_Level_Cfg *build_level_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(building_type, it->second.level);
	if(!build_level_cfg) {
		MSG_20600010 msg_20600010;
		msg_20600010.error_id = ERROR_BUILDING_LEVEL_CFG_NOT_EXIST;
		msg_20600010.to_uuid = to_uuid;
		msg_20600010.to_role_id = to_role_id;
		msg_20600010.uuid = uuid;
		msg_20600010.role_id = role_id;

		Block_Buffer buf;
		buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
		msg_20600010.serialize(buf);
	    buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}

	if(BUILDING_UPGRADE_DOING == it->second.status) {
		MSG_20600010 msg_20600010;
		msg_20600010.error_id = ERROR_UPGRADING_NO_GAIN_PRODUCTION;
		msg_20600010.to_uuid = to_uuid;
		msg_20600010.to_role_id = to_role_id;
		msg_20600010.uuid = uuid;
		msg_20600010.role_id = role_id;

		Block_Buffer buf;
		buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
		msg_20600010.serialize(buf);
	    buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}

	if(it->second.production == 0) {
		MSG_20600010 msg_20600010;
		msg_20600010.error_id = ERROR_ROB_BUILDING_NO_PRODUCTION;
		msg_20600010.to_uuid = to_uuid;
		msg_20600010.to_role_id = to_role_id;
		msg_20600010.uuid = uuid;
		msg_20600010.role_id = role_id;

		Block_Buffer buf;
		buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
		msg_20600010.serialize(buf);
	    buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}

	Logic_Rob_Production_Map::iterator rob_map_it = logic_rob_production_map.begin();
	if(rob_map_it == logic_rob_production_map.end()) {
		Logic_Rob_Production logic_rob_production;
		logic_rob_production.role_id = role_id;
		logic_rob_production.id = it->second.id;
		logic_rob_production.type = it->second.type;
		Logic_Rob_Production_Set logic_set;
		logic_set.insert(logic_rob_production);
		logic_rob_production_map.insert(std::make_pair(to_role_id, logic_set));
	}
	else {
		bool is_find = false;
		for(Logic_Rob_Production_Set::const_iterator rob_set_it = rob_map_it->second.begin();
				rob_set_it != rob_map_it->second.end();
				++rob_set_it) {
			if(rob_set_it->role_id == role_id && rob_set_it->id == it->second.id && rob_set_it->type == it->second.type) {
				is_find = true;
				break;
			}
		}

		if(!is_find) {
			Logic_Rob_Production rob_production;
			rob_production.role_id = role_id;
			rob_production.id = it->second.id;
			rob_production.type = it->second.type;
			rob_map_it->second.insert(rob_production);
		}
		else {
			MSG_20600010 msg_20600010;
			msg_20600010.error_id = ERROR_NO_REPEAT_ROB_SAME_BUILDING;
			msg_20600010.to_uuid = to_uuid;
			msg_20600010.to_role_id = to_role_id;
			msg_20600010.uuid = uuid;
			msg_20600010.role_id = role_id;

			Block_Buffer buf;
			buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
			msg_20600010.serialize(buf);
		    buf.finish_message();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
			return;
		}
	}//end else

	uint32_t rob_production = (uint32_t)(ceil(it->second.production * main_base_cfg.rob_percent  * 1.0 / 100));

	//重置上一次建筑资源产出时间
	Time_Value now = Time_Value::gettimeofday();
	it->second.last_output_production_time.set(now);

	MSG_20600010 msg_20600010;
	msg_20600010.to_uuid = to_uuid;
	msg_20600010.to_role_id = to_role_id;
	msg_20600010.to_server_name = to_server_name;
	msg_20600010.to_role_name = to_role_name;
	msg_20600010.uuid = uuid;
	msg_20600010.role_id = role_id;
	msg_20600010.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	msg_20600010.role_name = find_it->second->role_name;
	msg_20600010.gain_production = rob_production;

	it->second.production =
			(it->second.production >=  rob_production) ? (it->second.production - rob_production) : 0;
	it->second.is_gain = it->second.production >= build_level_cfg->min_gain_limit;

	if(BUILDING_GOLD_MINE == it->second.type) {
		find_it->second->dragon_vale_view.is_gold_mine_max_storage = false;
	}

	if(BUILDING_MOON_WELL == it->second.type) {
		find_it->second->dragon_vale_view.is_moon_well_max_storage = false;
	}

	if(BUILDING_TOTEM == it->second.type) {
		find_it->second->dragon_vale_view.is_totem_max_storage = false;
	}

	msg_20600010.building_info.id = it->second.id;
	msg_20600010.building_info.type = it->second.type;
	msg_20600010.building_info.level = it->second.level;
	msg_20600010.building_info.status = it->second.status;
	msg_20600010.building_info.production = it->second.production;
	msg_20600010.building_info.is_gain = 0; //note:(1)只能掠夺收获一次  (2)掠夺时，龙谷主人不能进龙谷，不能收获
	msg_20600010.building_info.order = it->second.order;
	msg_20600010.building_info.consume_type = it->second.consume_type;

	Block_Buffer buf;
	buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
	msg_20600010.serialize(buf);
    buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	//boradcast to dragon vale scene
	MSG_50180018  msg_50180018;
	msg_50180018.to_uuid = msg_20600010.to_uuid;
	msg_50180018.to_role_id = msg_20600010.to_role_id;
	msg_50180018.uuid = msg_20600010.uuid;
	msg_50180018.role_id = msg_20600010.role_id;
	msg_50180018.gain_production = msg_20600010.gain_production;
	msg_50180018.building_info.id = msg_20600010.building_info.id;
	msg_50180018.building_info.type = msg_20600010.building_info.type;
	msg_50180018.building_info.level = msg_20600010.building_info.level;
	msg_50180018.building_info.status = msg_20600010.building_info.status;
	msg_50180018.building_info.production = msg_20600010.building_info.production;
	msg_50180018.building_info.is_gain = msg_20600010.building_info.is_gain;
	msg_50180018.building_info.left_time = msg_20600010.building_info.left_time;
	msg_50180018.building_info.order = msg_20600010.building_info.order;

	Block_Buffer buf_50180018;
	buf_50180018.make_message(msg_50180018.msg_id);
	msg_50180018.serialize(buf_50180018);
	buf_50180018.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(find_it->second->role_id);
	broad_cast_buf.copy(&buf_50180018);
	broad_cast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	find_it->second->dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);
}

void Dragon_Vale_Manager::process_gain_rob_production_res(const MSG_20600010 &msg) {
	if(msg.error_id != 0) {
		MSG_80000001 msg_80000001;
		msg_80000001.error_id = msg.error_id;
		msg_80000001.error_source = REQ_GAIN_ROB_PRODUCTION_TO_LOGIC;
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(msg.to_role_id);
		if(player) {
			OBJ_SEND_TO_CLIENT(msg_80000001, (*player));
		}
		return;
	}

	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(msg.to_role_id);
	if(!player) {
		MSG_USER("player is null, role_id=%ld", msg.to_role_id);
		return;
	}

	//收获资源获得货币
	uint8_t money_type = player->get_production_type(msg.building_info.type);
	player->gain_money_in_dragon_vale(money_type,
					msg.gain_production,
					Pack::MONEY_GAIN_PRODUCTION_IN_DRAGON_VALE);
}

void Dragon_Vale_Manager::process_enter_dragon_vale(int64_t creater, int64_t role_id) {
	Dragon_Vale_Role_Id_Map::iterator map_it = dragon_vale_role_id_map.find(creater);
	if(map_it == dragon_vale_role_id_map.end()) {
		Enter_Role_Id_Set enter_set;
		enter_set.insert(role_id);
		dragon_vale_role_id_map.insert(std::make_pair(creater, enter_set));

		process_add_monster_to_scene_first_enter(creater);
	}
	else {
		map_it->second.insert(role_id);
		process_add_monster_to_scene_first_enter(creater);
	}
}

void Dragon_Vale_Manager::process_exit_dragon_vale(int64_t creater, int64_t role_id) {
	Dragon_Vale_Role_Id_Map::iterator map_it = dragon_vale_role_id_map.find(creater);
	if(map_it == dragon_vale_role_id_map.end()) {
		return;
	}

	map_it->second.erase(role_id);
	if(map_it->second.empty()) {
		dragon_vale_role_id_map.erase(creater);
	}
}

void Dragon_Vale_Manager::process_offline_tick(Player_DB_View &player_db_view, const Time_Value &now) {
	listen_offline_building_output_production_by_one_minute(player_db_view, now);
	listen_offline_building_finish(player_db_view, now);
	listen_offline_dragon_born_finish(player_db_view, now);
	listen_offline_play_dragon_interval_time(player_db_view, now);
	listen_offline_dragon_feeling_down_time(player_db_view, now);
	listen_offline_dragon_rob_protect_time(player_db_view, now);
	listen_offline_refresh_monster(player_db_view, now);
}

void Dragon_Vale_Manager::listen_offline_building_output_production_by_one_minute(Player_DB_View &player_db_view,
													const Time_Value &now) {
	MSG_82300001 msg;
	const Building_Level_Cfg *build_level_cfg = 0;
	long total_time = 0;

	uint32_t production_per_min = 0;
	uint32_t add_speed = 0;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(player_db_view.dragon_vale_view.star_num);
	if(!star_add_speed) {
		add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", player_db_view.dragon_vale_view.star_num);
	}
	else {
		add_speed = (uint16_t)((*star_add_speed));
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	uint32_t sub_speed =
				(uint32_t)(player_db_view.dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);

	for(DragonBuildingInfoMap::iterator it = player_db_view.dragon_vale_view.dragon_building_info_map.begin();
			it != player_db_view.dragon_vale_view.dragon_building_info_map.end();
			++it) {
		//升级状态，不产出
		if(BUILDING_UPGRADE_DOING == it->second.status) {
			continue;
		}

		//等级为0，说明还未建筑完成，不能产出
		if(0 == it->second.level) {
			continue;
		}

		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, it->second.level);
		if(!build_level_cfg) {
			continue;
		}

		if(it->second.production >= build_level_cfg->max_storage) {
			continue;
		}

		total_time = it->second.last_output_production_time.sec() + 1 * Time_Value::ONE_MINUTE_IN_SECS;
		if(total_time > now.sec()) {
			continue;
		}

		it->second.last_output_production_time.set(now);

		production_per_min = (uint32_t)((100 + add_speed - sub_speed) * build_level_cfg->production_per_min / 100.0);

		it->second.production += production_per_min;
		if(it->second.production > build_level_cfg->max_storage) {
			it->second.production = build_level_cfg->max_storage;
		}

		it->second.is_gain = (it->second.production >= build_level_cfg->min_gain_limit);

		Building_Info msg_building_info;
		msg_building_info.id = it->second.id;
		msg_building_info.type = it->second.type;
		msg_building_info.level = it->second.level;
		msg_building_info.status = it->second.status;
		msg_building_info.production = it->second.production;
		msg_building_info.production_per_minute = production_per_min;
		msg_building_info.is_gain = it->second.is_gain ? 1 : 0;
		msg_building_info.order = it->second.order;
		msg_building_info.consume_type = it->second.consume_type;

		msg.building_info_list.push_back(msg_building_info);
	}//end for

	if(!msg.building_info_list.empty()) {
		//在龙谷里面才会发消息
//		if(dragon_vale_detail_->target_player_id != 0) {
//			THIS_SEND_TO_CLIENT(msg);
//		}

		//boradcast to dragon vale scene
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(player_db_view.role_id);
		broad_cast_buf.copy(&buf);
		broad_cast_buf.finish_message();
	    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

		uint32_t gold_mine_max_storage = 0;
		uint32_t moon_weel_max_storage = 0;
	    uint32_t totem_max_storage = 0;
	    uint32_t new_gold_mine_production = 0;
	    uint32_t new_moon_weel_production = 0;
	    uint32_t new_totem_production = 0;

	    //在龙谷里面、外面都会发的消息，客户端弹窗用
	    MSG_82300008 max_msg;
	    player_db_view.dragon_vale_view.get_building_max_storage(gold_mine_max_storage, moon_weel_max_storage,
				totem_max_storage, new_gold_mine_production,
				new_moon_weel_production, new_totem_production, false);
		if(!player_db_view.dragon_vale_view.is_gold_mine_max_storage && gold_mine_max_storage != 0
				&& new_gold_mine_production != 0 && new_gold_mine_production >= gold_mine_max_storage) {
			max_msg.is_gold_mine = 1;
			player_db_view.dragon_vale_view.is_gold_mine_max_storage = true;
		}
		if(!player_db_view.dragon_vale_view.is_moon_well_max_storage && moon_weel_max_storage != 0
				&& new_moon_weel_production != 0 && new_moon_weel_production >= moon_weel_max_storage) {
			max_msg.is_moon_weel = 1;
			player_db_view.dragon_vale_view.is_moon_well_max_storage = true;
		}
		if(!player_db_view.dragon_vale_view.is_totem_max_storage && totem_max_storage != 0
				&& new_totem_production != 0 && new_totem_production >= totem_max_storage) {
			max_msg.is_totem = 1;
			player_db_view.dragon_vale_view.is_totem_max_storage = true;
		}

		if(max_msg.is_gold_mine || max_msg.is_moon_weel || max_msg.is_totem) {
			Logic_Player::offline_dragon_vale_building_notice(player_db_view.role_id,
								BUILDING_NOTICE_OUTPUT_PRODUCTION_MAX,
								max_msg.is_gold_mine,
								max_msg.is_moon_weel,
								max_msg.is_totem);
		}

		Block_Buffer db_buf;
		db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
		player_db_view.dragon_vale_view.serialize(db_buf);
		db_buf.finish_message();
		DB_MANAGER->push_data_block(db_buf, 0);
	}
}

void Dragon_Vale_Manager::listen_offline_building_finish(Player_DB_View &player_db_view, const Time_Value &now) {
	MSG_82300002 msg;
	const Building_Level_Cfg *build_level_cfg = 0;
	long total_time = 0;

	//建筑是否建造、升级完成的标志
	uint8_t gold_mine_flag = 0;
	uint8_t moon_well_flag = 0;
	uint8_t totem_flag = 0;

	for(DragonBuildingInfoMap::iterator it = player_db_view.dragon_vale_view.dragon_building_info_map.begin();
				it != player_db_view.dragon_vale_view.dragon_building_info_map.end();
				++it) {
		//不是升级状态，不处理
		if(BUILDING_UPGRADE_DOING != it->second.status) {
			continue;
		}

		uint16_t new_level = it->second.level + 1;
		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, new_level);
		if(!build_level_cfg) {
			continue;
		}

		if(0 == it->second.level) {
			const Create_Building_Cfg *create_building_cfg =
							CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
			if(!create_building_cfg) {
				continue;
			}
			total_time = it->second.begin_upgrade_time.sec()
					+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
					+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
					+ (long)(create_building_cfg->create_need_second);
		}
		else {
			total_time = it->second.begin_upgrade_time.sec()
					+ build_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
					+ build_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
					+ build_level_cfg->upgrade_need_second;
		}

		if(total_time > now.sec()) {
			continue;
		}

		it->second.level += 1;
		it->second.status = BUILDING_UPGRADE_FINISH;
		//建造或者升级完成，将上一次资源产出时间设置为当前时间(因为建造或者升级中，不能产出资源)
		it->second.last_output_production_time.set(now);
		player_db_view.dragon_vale_view.build_queue_num += 1;

		if(1 == it->second.level && player_db_view.dragon_vale_view.dragon_building_info_map.size() == 1) {
			player_db_view.dragon_vale_view.begin_refresh_thief_time.set(now);
		}

		if(BUILDING_GOLD_MINE == it->second.type) {
			if(1 == it->second.level){
				gold_mine_flag = 1;
			}
//			else {
//				gold_mine_flag = 2;
//			}
		}

		if(BUILDING_MOON_WELL == it->second.type) {
			if(1 == it->second.level) {
				moon_well_flag = 1;
			}
//			else {
//				moon_well_flag = 2;
//			}
		}

		if(BUILDING_TOTEM == it->second.type) {
			if(1 == it->second.level) {
				totem_flag = 1;
			}
//			else {
//				totem_flag = 2;
//			}
		}

		Building_Info msg_building_info;
		msg_building_info.id = it->second.id;
		msg_building_info.type = it->second.type;
		msg_building_info.level = it->second.level;
		msg_building_info.status = it->second.status;
		msg_building_info.production = it->second.production;
		msg_building_info.is_gain = it->second.is_gain ? 1 : 0;
		msg_building_info.order = it->second.order;
		msg_building_info.consume_type = it->second.consume_type;

		msg.building_info_list.push_back(msg_building_info);
	}//end for

	if(!msg.building_info_list.empty()) {
		msg.role_id = player_db_view.role_id;
		const Role_Level_Cfg *role_level_cfg = 0;
		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_db_view.level, BUILDING_GOLD_MINE);
		if(role_level_cfg) {
			msg.is_build_gold_mine =
					role_level_cfg->max_building_num >  player_db_view.dragon_vale_view.build_gold_mine_num ? 1 : 0;
		}

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_db_view.level, BUILDING_MOON_WELL);
		if(role_level_cfg) {
			msg.is_build_moon_well = role_level_cfg->max_building_num > player_db_view.dragon_vale_view.build_moon_well_num ? 1 : 0;
		}

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_db_view.level, BUILDING_TOTEM);
		if(role_level_cfg) {
			msg.is_build_totem = role_level_cfg->max_building_num > player_db_view.dragon_vale_view.build_totem_num ? 1 : 0;
		}

		msg.build_queue_num = player_db_view.dragon_vale_view.build_queue_num;

		//boradcast to dragon vale scene
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(player_db_view.role_id);
		broad_cast_buf.copy(&buf);
		broad_cast_buf.finish_message();
	    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

		Block_Buffer db_buf;
		db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
		player_db_view.dragon_vale_view.serialize(db_buf);
		db_buf.finish_message();
		DB_MANAGER->push_data_block(db_buf, 0);

		//在龙谷里面、外面都会发的消息，客户端弹窗用
		if((gold_mine_flag == 1) || (moon_well_flag == 1) || (totem_flag == 1)) {
			Logic_Player::offline_dragon_vale_building_notice(player_db_view.role_id,
								BUILDING_NOTICE_UPGRADE_FINISH,
								gold_mine_flag,
								moon_well_flag,
								totem_flag);
		}

		if(player_db_view.dragon_vale_view.is_vip_down_to_zero
				&& !player_db_view.dragon_vale_view.is_has_building_upgrading()) {
			const Vip_Level_Cfg *vip_normal_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Vip_Level_Cfg(0);
			if(!vip_normal_cfg) {
				MSG_USER("vip_level_cfg not exist, vip_level=0");
				return;
			}

			player_db_view.dragon_vale_view.all_open_build_queue_num = vip_normal_cfg->max_queue_num;
			player_db_view.dragon_vale_view.build_queue_num = vip_normal_cfg->max_queue_num;

//			if(player_db_view.role_id == player_db_view.dragon_vale_view.target_player_id) {
//				MSG_82300022 msg;
//				msg.all_open_build_queue_num = player_db_view.dragon_vale_view.all_open_build_queue_num;
//				msg.build_queue_num = player_db_view.dragon_vale_view.build_queue_num;
//				THIS_SEND_TO_CLIENT(msg);
//			}

			Block_Buffer db_buf;
			db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
			player_db_view.dragon_vale_view.serialize(db_buf);
			db_buf.finish_message();
			DB_MANAGER->push_data_block(db_buf, 0);
		}
	}//end if
}

void Dragon_Vale_Manager::listen_offline_dragon_born_finish(Player_DB_View &player_db_view, const Time_Value &now) {
	//未开始孵化，不处理
	if(!player_db_view.dragon_vale_view.is_dragon_born) {
		return;
	}

	//龙蛋已孵化为龙，不处理
	if(player_db_view.dragon_vale_view.dragon_info.progress > 0) {
		return;
	}

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = player_db_view.dragon_vale_view.begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;

	//已开始孵化，但是，孵化未完成，不处理
	if(total_time > now.sec()) {
		return;
	}

	//孵化完成，进化程度变为1
	player_db_view.dragon_vale_view.dragon_info.progress = 1;

	//孵化完成，可以逗龙   注意：龙蛋孵化完成，马上就可以逗龙，不能重置逗龙冷却时间
	player_db_view.dragon_vale_view.dragon_info.is_play_dragon = true;

	//龙蛋孵化为龙，龙的心情值初始为最大值 - 1
	player_db_view.dragon_vale_view.dragon_info.feeling = dragon_base_cfg.max_feeling  - 1;

	//龙蛋孵化为龙，初始化龙的等级为0、 初始化属性
	player_db_view.dragon_vale_view.dragon_info.power_level = 0;
	player_db_view.dragon_vale_view.dragon_info.armor_level = 0;
	player_db_view.dragon_vale_view.dragon_info.stamina_level = 0;
	player_db_view.dragon_vale_view.dragon_info.hero_power_level = 0;
	player_db_view.dragon_vale_view.dragon_info.hero_armor_level = 0;
	player_db_view.dragon_vale_view.dragon_info.hero_stamina_level = 0;
	player_db_view.dragon_vale_view.dragon_info.crit_level = 0;
	player_db_view.dragon_vale_view.dragon_info.prevent_crit_level = 0;
	player_db_view.dragon_vale_view.dragon_info.hero_crit_level = 0;
	player_db_view.dragon_vale_view.dragon_info.hero_prevent_crit_level = 0;

	const Dragon_Evolution_Cfg* dragon_evolution_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(player_db_view.dragon_vale_view.dragon_info.progress);
	if(dragon_evolution_cfg) {
		double temp_property = 0.0;
		const Dragon_Keep_Cfg* dragon_keep_cfg = 0;
		//初始化主角斗力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_POWER,
								player_db_view.dragon_vale_view.dragon_info.power_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.power = (uint32_t)temp_property;
		}

		//初始化主角护甲属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_ARMOR,
							player_db_view.dragon_vale_view.dragon_info.armor_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.armor = (uint32_t)temp_property;
		}

		//初始化主角耐力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_STAMINA,
				player_db_view.dragon_vale_view.dragon_info.stamina_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.stamina = (uint32_t)temp_property;
		}

		//初始化英雄斗力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_POWER,
								player_db_view.dragon_vale_view.dragon_info.hero_power_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.hero_power = (uint32_t)temp_property;
		}

		//初始化英雄护甲属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_ARMOR,
							player_db_view.dragon_vale_view.dragon_info.hero_armor_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.hero_armor = (uint32_t)temp_property;
		}

		//初始化英雄耐力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_STAMINA,
				player_db_view.dragon_vale_view.dragon_info.hero_stamina_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.hero_stamina = (uint32_t)temp_property;
		}

		//初始化主角暴击属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_CRIT,
				player_db_view.dragon_vale_view.dragon_info.crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.crit = (uint32_t)temp_property;
		}

		//初始化主角主角抗暴属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_PREVENT_CRIT,
				player_db_view.dragon_vale_view.dragon_info.prevent_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.prevent_crit = (uint32_t)temp_property;
		}

		//初始化英雄暴击属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_CRIT,
				player_db_view.dragon_vale_view.dragon_info.hero_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.hero_crit = (uint32_t)temp_property;
		}

		//初始化英雄抗暴属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_PREVENT_CRIT,
				player_db_view.dragon_vale_view.dragon_info.hero_prevent_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			player_db_view.dragon_vale_view.dragon_info.hero_prevent_crit = (uint32_t)temp_property;
		}
	}

	//重置龙心情值下降的开始时间
	player_db_view.dragon_vale_view.begin_down_feeling_time.set(now);

	//在龙谷里面才会发消息
//	if(player_db_view.dragon_vale_view.target_player_id != 0) {
//		MSG_82300004 msg;
//		msg.progress = player_db_view.dragon_vale_view.dragon_info.progress;
//		msg.is_play_dragon = player_db_view.dragon_vale_view.dragon_info.is_play_dragon ? 1 : 0;
//		THIS_SEND_TO_CLIENT(msg);
//	}

	//boradcast to dragon vale scene
	MSG_82300004 msg;
	msg.progress = player_db_view.dragon_vale_view.dragon_info.progress;
	msg.is_play_dragon = player_db_view.dragon_vale_view.dragon_info.is_play_dragon ? 1 : 0;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_db_view.role_id);
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);


	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	player_db_view.dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);
}

void Dragon_Vale_Manager::listen_offline_play_dragon_interval_time(Player_DB_View &player_db_view, const Time_Value &now) {
	//未开始孵化或者孵化未完成，不处理
	if(!player_db_view.dragon_vale_view.is_dragon_born || player_db_view.dragon_vale_view.dragon_info.progress < 1) {
		return;
	}

	//当前可以逗龙，不需要处理冷却时间
	if(player_db_view.dragon_vale_view.dragon_info.is_play_dragon) {
		return;
	}

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();

	//龙的心情值满时，不可以逗龙,不处理逗龙冷却
	if(dragon_base_cfg.max_feeling <= player_db_view.dragon_vale_view.dragon_info.feeling) {
		return;
	}

	long total_time = player_db_view.dragon_vale_view.begin_play_dragon_interval_time.sec()
							+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;
	//冷却时间未到，不处理
	if(total_time > now.sec()) {
		return;
	}

	player_db_view.dragon_vale_view.dragon_info.is_play_dragon = true;

	//TODO broadcast to scene
	//在龙谷里面才会发消息
//	if(player_db_view.dragon_vale_view.target_player_id != 0) {
//		MSG_82300007 msg;
//		msg.role_id = player_db_view.role_id;
//		msg.is_play_dragon = player_db_view.dragon_vale_view.dragon_info.is_play_dragon ?  1 : 0;
//		THIS_SEND_TO_CLIENT(msg);
//	}

	MSG_82300007 msg;
	msg.role_id = player_db_view.role_id;
	msg.is_play_dragon = player_db_view.dragon_vale_view.dragon_info.is_play_dragon ?  1 : 0;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_db_view.role_id);
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	player_db_view.dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);
}

void Dragon_Vale_Manager::listen_offline_dragon_feeling_down_time(Player_DB_View &player_db_view, const Time_Value &now) {
	//未开始孵化或者孵化未完成，不处理
	if(!player_db_view.dragon_vale_view.is_dragon_born || player_db_view.dragon_vale_view.dragon_info.progress < 1) {
		return;
	}

	//龙的心情值已下降到0，不处理
	if(0 == player_db_view.dragon_vale_view.dragon_info.feeling) {
		return;
	}

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();

	long total_time = player_db_view.dragon_vale_view.begin_down_feeling_time.sec()
						+ dragon_base_cfg.down_feeling_time * Time_Value::ONE_MINUTE_IN_SECS;
	//时间未到,不处理
	if(total_time > now.sec()) {
		return;
	}

	//龙的心情值减1，最低为0
	player_db_view.dragon_vale_view.dragon_info.feeling =
			(player_db_view.dragon_vale_view.dragon_info.feeling > 0) ?player_db_view.dragon_vale_view.dragon_info.feeling - 1 : 0;

	//重置龙心情值下降的开始时间
	player_db_view.dragon_vale_view.begin_down_feeling_time.set(now);

	//TODO boraocast
	//在龙谷里面才会发消息
//	if(player_db_view.dragon_vale_view.target_player_id != 0) {
//		MSG_82300005 msg;
//		msg.type = 1;
//		msg.feeling = player_db_view.dragon_vale_view.dragon_info.feeling;
//		THIS_SEND_TO_CLIENT(msg);
//	}

	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	player_db_view.dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);
}

void Dragon_Vale_Manager::listen_offline_dragon_rob_protect_time(Player_DB_View &player_db_view, const Time_Value &now) {
	if(!player_db_view.dragon_vale_view.is_rob_protect) {
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	long total_time = player_db_view.dragon_vale_view.begin_rob_protect_time.sec()
			          + main_base_cfg.rob_time_hour * Time_Value::ONE_HOUR_IN_SECS
			          + main_base_cfg.rob_time_minute * Time_Value::ONE_MINUTE_IN_SECS;
	if(total_time > now.sec()) {
		return;
	}

	player_db_view.dragon_vale_view.is_rob_protect = false;

	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	player_db_view.dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);
}

void Dragon_Vale_Manager::listen_offline_refresh_monster(Player_DB_View &player_db_view, const Time_Value &now) {
	if(player_db_view.dragon_vale_view.dragon_building_info_map.empty()) {
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	if(player_db_view.dragon_vale_view.thief_id_quality_map.size() >= main_base_cfg.max_thief_num) {
		return;
	}

	long total_time = player_db_view.dragon_vale_view.begin_refresh_thief_time.sec()
					+ main_base_cfg.refresh_thief_time * Time_Value::ONE_MINUTE_IN_SECS;
	if(total_time > now.sec()) {
		return;
	}

	bool is_has_building_finish = player_db_view.dragon_vale_view.is_has_building_finish();
	if(!is_has_building_finish) {
		return;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint8_t chance = random() % 100;
	if(chance > main_base_cfg.refresh_thief_chance) {
		return;
	}

	MSG_20600015 msg;
	msg.type = 1;
	msg.role_id = player_db_view.role_id;

	const Main_Thief_Id_Set &main_thief_id_set = CONFIG_CACHE_DRAGON_VALE->get_main_config().main_thief_id_set;
	std::vector<uint32_t> temp;
	for(Main_Thief_Id_Set::const_iterator it = main_thief_id_set.begin(); it != main_thief_id_set.end(); ++it) {
		if(player_db_view.dragon_vale_view.thief_id_quality_map.find((int64_t)*it)
				!= player_db_view.dragon_vale_view.thief_id_quality_map.end()) {
			continue;
		}
		temp.push_back(*it);
	}

	if(temp.empty()) {
		return;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint32_t index = random()%temp.size();
	uint32_t id = temp[index];
	msg.id = id;
	uint8_t  quality = DRAGON_VALE_MANAGER->get_dragon_vale_refresh_monster_quality();
	msg.quality = quality;

	player_db_view.dragon_vale_view.begin_refresh_thief_time.set(now);
	bool flag = player_db_view.dragon_vale_view.thief_id_quality_map.insert(std::make_pair(id, quality)).second;
	if(!flag) {
		return;
	}

	Block_Buffer buf;
	buf.make_message(INNER_DRAGON_VALE_MONSTER);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	player_db_view.dragon_vale_view.check_and_change_notice_board_num();
	if(player_db_view.dragon_vale_view.notice_board_list.size() < 50) {
		Notice_Board notice_board;
		Time_Value now = Time_Value::gettimeofday();
		notice_board.type = NOTICE_BOARD_THIEF_APPEAR;
		notice_board.time = now.sec();

		const Monster_Section_Config *monster_section_cfg =
				CONFIG_CACHE_NPC->get_monster_section_config(main_base_cfg.dragon_vale_scene_id);
		if(monster_section_cfg) {
			Monster_Point_Config_Map::const_iterator it = monster_section_cfg->point_map.find(id);
			if(it != monster_section_cfg->point_map.end()) {
				const std::vector<Int_Int> &npc_vec = it->second.npc_vec;
				if(!npc_vec.empty()) {
					uint32_t monster_id = npc_vec.begin()->val_1;
					const NPC_Config_Cache *npc_config_cache =
							CONFIG_CACHE_NPC->find_npc_config_cache(monster_id);
					if(npc_config_cache) {
						notice_board.host = npc_config_cache->name;
						notice_board.param1 = quality;
						player_db_view.dragon_vale_view.notice_board_list.push_back(notice_board);
					}
				}
			}
		}
	}// end if

	MSG_82300030 msg_82300030;
	msg_82300030.head_info.role_id = player_db_view.role_id;
	msg_82300030.head_info.role_name = player_db_view.role_name;
	msg_82300030.head_info.star_num = player_db_view.dragon_vale_view.star_num;
	msg_82300030.head_info.head_photo_id = player_db_view.friend_.head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(player_db_view.dragon_vale_view.star_num);
	if(!star_add_speed) {
		msg_82300030.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", player_db_view.dragon_vale_view.star_num);
	}
	else {
		msg_82300030.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_82300030.head_info.thief_sub_speed =
		(uint16_t)(player_db_view.dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = player_db_view.dragon_vale_view.foot_info_map.begin();
			it != player_db_view.dragon_vale_view.foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_82300030.head_info.foot_list.push_back(msg_foot_info);
	}

	Block_Buffer buf_82300030;
	buf_82300030.make_message(msg_82300030.msg_id);
	msg_82300030.serialize(buf_82300030);
	buf_82300030.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_db_view.role_id);
	broad_cast_buf.copy(&buf_82300030);
	broad_cast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	player_db_view.dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);

	refresh_dragon_vale_op(player_db_view.role_id);
}

void Dragon_Vale_Manager::process_drive_thief(int64_t player_id, int64_t role_id, int64_t id, uint8_t quality) {
	Logic_Player *target = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!target) {
		process_drive_offline_dragon_vale_thief(player_id, role_id, id, quality);
		return;
	}

	if(target->dragon_vale_detail().thief_id_quality_map.find(id)
			== target->dragon_vale_detail().thief_id_quality_map.end()) {
		return;
	}

	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(player_id);
	if(!player) {
		return;
	}

	const uint32_t *thief_award =  CONFIG_CACHE_DRAGON_VALE->find_main_thief_award(player->level(), quality);
	if(!thief_award) {
		return;
	}

	++player->dragon_vale_detail_no_const().drive_thief_num;

	MSG_50200154 msg;
	msg.quality = quality;

	if(player->dragon_vale_detail().drive_thief_num <=
			CONFIG_CACHE_DRAGON_VALE->get_main_config().main_base_cfg.drive_thief_max_num) {
		const Dragon_Base_Cfg &dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->get_dragon_config().dragon_base_cfg;
		Money_Sub_Type money_sub_type = Pack::MONEY_SUB_TYPE_END;
		Money_Gain_Type money_gain_type = MONEY_ADD_MAX;
		switch(dragon_base_cfg.dragon_vale_op_type) {
		case Pack::COPPER:
			money_sub_type = Pack::COPPER_ONLY;
			money_gain_type = Pack::MONEY_ADD_DRIVTE_THIEF_GET_COIN;
			break;

		case Pack::DRAGON_SOUL:
			money_sub_type = Pack::DRAGON_SOUL_ONLY;
			money_gain_type = Pack::MONEY_ADD_DRIVTE_THIEF_GET_DRAGON_SOUL;
			break;

		case Pack::SOULS:
			money_sub_type = Pack::SOULS_ONLY;
			money_gain_type = Pack::MONEY_ADD_SOUL_BY_DRIVTE_THIEF;
			break;

		case Pack::FRIENDSHIP:
			money_sub_type = Pack::FRIENDSHIP_ONLY;
			money_gain_type = Pack::MONEY_ADD_FRIENDSHIP_BY_DRIVTE_THIEF;
			break;
		}

		player->gain_money_in_dragon_vale(money_sub_type,
						*thief_award,
						money_gain_type);
		msg.award = *thief_award;
	}
	else {
		msg.award = 0;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	if(player_id != role_id) {
		target->dragon_vale_detail_no_const().check_and_change_notice_board_num();
		if(target->dragon_vale_detail_no_const().notice_board_list.size() < 50) {
			Notice_Board notice_board;
			Time_Value now = Time_Value::gettimeofday();
			notice_board.type = NOTICE_BOARD_DRIVE_THIEF;
			notice_board.time = now.sec();
			notice_board.host = player->role_name();
			notice_board.dragon_soul = *thief_award;
			notice_board.param1 = quality;

			const Monster_Section_Config *monster_section_cfg =
					CONFIG_CACHE_NPC->get_monster_section_config(main_base_cfg.dragon_vale_scene_id);
			if(monster_section_cfg) {
				Monster_Point_Config_Map::const_iterator it = monster_section_cfg->point_map.find(id);
				if(it != monster_section_cfg->point_map.end()) {
					const std::vector<Int_Int> &npc_vec = it->second.npc_vec;
					if(!npc_vec.empty()) {
						uint32_t monster_id = npc_vec.begin()->val_1;
						const NPC_Config_Cache *npc_config_cache =
								CONFIG_CACHE_NPC->find_npc_config_cache(monster_id);
						if(npc_config_cache) {
							msg.name = npc_config_cache->name;
							notice_board.target = npc_config_cache->name;
							target->dragon_vale_detail_no_const().notice_board_list.push_back(notice_board);
						}
					}
				}
			}
		}
	}//end if

	OBJ_SEND_TO_CLIENT(msg, (*player));

	target->dragon_vale_detail_no_const().thief_id_quality_map.erase(id);
	if(target->dragon_vale_detail_no_const().thief_id_quality_map.size() >= (uint8_t)(main_base_cfg.max_thief_num - 1)) {
		target->dragon_vale_detail_no_const().begin_refresh_thief_time.set(Time_Value::gettimeofday());
	}

	MSG_82300030 msg_82300030;
	msg_82300030.head_info.role_id = target->role_id();
	msg_82300030.head_info.role_name = target->role_name();
	msg_82300030.head_info.star_num = target->dragon_vale_detail().star_num;
	msg_82300030.head_info.head_photo_id = target->friend_detail().head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(target->dragon_vale_detail().star_num);
	if(!star_add_speed) {
		msg_82300030.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", target->dragon_vale_detail().star_num);
	}
	else {
		msg_82300030.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_82300030.head_info.thief_sub_speed =
		(uint16_t)(target->dragon_vale_detail().thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = target->dragon_vale_detail().foot_info_map.begin();
			it != target->dragon_vale_detail().foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_82300030.head_info.foot_list.push_back(msg_foot_info);
	}

	Block_Buffer buf_82300030;
	buf_82300030.make_message(msg_82300030.msg_id);
	msg_82300030.serialize(buf_82300030);
	buf_82300030.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(target->role_id());
	broad_cast_buf.copy(&buf_82300030);
	broad_cast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	((Dragon_Vale*)target)->save_dragon_vale_tick();
	target->sync_dragon_vale_to_db_view();

	player->sync_dragon_vale_to_db_view();
	player->save_dragon_vale_tick();

	player->achievement_listen_dragon_vale_drive_thief_times();
	player->daily_helper_listen(DAILY_FULFILL_LISTEN_DRAGON_DRIVE_THIEF);//日常助手监听龙谷驱逐

	refresh_dragon_vale_op(role_id);
}

void Dragon_Vale_Manager::process_drive_offline_dragon_vale_thief(int64_t player_id, int64_t role_id,
								int64_t id, uint8_t quality) {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	if(find_it->second->dragon_vale_view.thief_id_quality_map.find(id)
			== find_it->second->dragon_vale_view.thief_id_quality_map.end()) {
		return;
	}

	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(player_id);
	if(!player) {
		return;
	}

	const uint32_t *thief_award =  CONFIG_CACHE_DRAGON_VALE->find_main_thief_award(player->level(), quality);
	if(!thief_award) {
		return;
	}

	++player->dragon_vale_detail_no_const().drive_thief_num;

	MSG_50200154 msg;
	msg.quality = quality;

	if(player->dragon_vale_detail().drive_thief_num <=
			CONFIG_CACHE_DRAGON_VALE->get_main_config().main_base_cfg.drive_thief_max_num) {
		const Dragon_Base_Cfg &dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->get_dragon_config().dragon_base_cfg;
		Money_Sub_Type money_sub_type = Pack::MONEY_SUB_TYPE_END;
		Money_Gain_Type money_gain_type = MONEY_ADD_MAX;
		switch(dragon_base_cfg.dragon_vale_op_type) {
		case Pack::COPPER:
			money_sub_type = Pack::COPPER_ONLY;
			money_gain_type = Pack::MONEY_ADD_DRIVTE_THIEF_GET_COIN;
			break;

		case Pack::DRAGON_SOUL:
			money_sub_type = Pack::DRAGON_SOUL_ONLY;
			money_gain_type = Pack::MONEY_ADD_DRIVTE_THIEF_GET_DRAGON_SOUL;
			break;

		case Pack::SOULS:
			money_sub_type = Pack::SOULS_ONLY;
			money_gain_type = Pack::MONEY_ADD_SOUL_BY_DRIVTE_THIEF;
			break;

		case Pack::FRIENDSHIP:
			money_sub_type = Pack::FRIENDSHIP_ONLY;
			money_gain_type = Pack::MONEY_ADD_FRIENDSHIP_BY_DRIVTE_THIEF;
			break;
		}

		player->gain_money_in_dragon_vale(money_sub_type,
						*thief_award,
						money_gain_type);
		msg.award = *thief_award;
	}
	else {
		msg.award = 0;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	if(player_id != role_id) {
		find_it->second->dragon_vale_view.check_and_change_notice_board_num();
		if(find_it->second->dragon_vale_view.notice_board_list.size() < 50) {
			Notice_Board notice_board;
			Time_Value now = Time_Value::gettimeofday();
			notice_board.type = NOTICE_BOARD_DRIVE_THIEF;
			notice_board.time = now.sec();
			notice_board.host = player->role_name();
			notice_board.dragon_soul = *thief_award;
			notice_board.param1 = quality;

			const Monster_Section_Config *monster_section_cfg =
					CONFIG_CACHE_NPC->get_monster_section_config(main_base_cfg.dragon_vale_scene_id);
			if(monster_section_cfg) {
				Monster_Point_Config_Map::const_iterator it = monster_section_cfg->point_map.find(id);
				if(it != monster_section_cfg->point_map.end()) {
					const std::vector<Int_Int> &npc_vec = it->second.npc_vec;
					if(!npc_vec.empty()) {
						uint32_t monster_id = npc_vec.begin()->val_1;
						const NPC_Config_Cache *npc_config_cache =
								CONFIG_CACHE_NPC->find_npc_config_cache(monster_id);
						if(npc_config_cache) {
							msg.name = npc_config_cache->name;
							notice_board.target = npc_config_cache->name;
							find_it->second->dragon_vale_view.notice_board_list.push_back(notice_board);
						}
					}
				}
			}
		}
	}//end if

	OBJ_SEND_TO_CLIENT(msg, (*player));

	find_it->second->dragon_vale_view.thief_id_quality_map.erase(id);
	if(find_it->second->dragon_vale_view.thief_id_quality_map.size() >= (uint8_t)(main_base_cfg.max_thief_num - 1)) {
		find_it->second->dragon_vale_view.begin_refresh_thief_time.set(Time_Value::gettimeofday());
	}

	MSG_82300030 msg_82300030;
	msg_82300030.head_info.role_id = find_it->second->role_id;
	msg_82300030.head_info.role_name = find_it->second->role_name;
	msg_82300030.head_info.star_num = find_it->second->dragon_vale_view.star_num;
	msg_82300030.head_info.head_photo_id = find_it->second->friend_.head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(find_it->second->dragon_vale_view.star_num);
	if(!star_add_speed) {
		msg_82300030.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", find_it->second->dragon_vale_view.star_num);
	}
	else {
		msg_82300030.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_82300030.head_info.thief_sub_speed =
		(uint16_t)(find_it->second->dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = find_it->second->dragon_vale_view.foot_info_map.begin();
			it != find_it->second->dragon_vale_view.foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_82300030.head_info.foot_list.push_back(msg_foot_info);
	}

	Block_Buffer buf_82300030;
	buf_82300030.make_message(msg_82300030.msg_id);
	msg_82300030.serialize(buf_82300030);
	buf_82300030.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(find_it->second->role_id);
	broad_cast_buf.copy(&buf_82300030);
	broad_cast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	find_it->second->dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);

	player->sync_dragon_vale_to_db_view();
	player->save_dragon_vale_tick();

	player->achievement_listen_dragon_vale_drive_thief_times();
	player->daily_helper_listen(DAILY_FULFILL_LISTEN_DRAGON_DRIVE_THIEF);//日常助手监听龙谷驱逐

	refresh_dragon_vale_op(role_id);
}

void Dragon_Vale_Manager::process_add_monster_to_scene_first_enter(int64_t creater) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(creater);
	if(!player) {
		process_offline_add_monster_to_scene_first_enter(creater);
		return;
	}

	if(player->dragon_vale_detail().thief_id_quality_map.empty()) {
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	uint8_t i = 0;
	for(Thief_Id_Quality_Map::const_iterator it = player->dragon_vale_detail().thief_id_quality_map.begin();
			it != player->dragon_vale_detail().thief_id_quality_map.end();
			++it) {
		if(i >= main_base_cfg.max_thief_num) {
			break;
		}

		MSG_20600015 msg;
		msg.type = 1;
		msg.role_id = creater;
		msg.id = it->first;
		msg.quality = it->second;
		Block_Buffer buf;
		buf.make_message(INNER_DRAGON_VALE_MONSTER);
	    msg.serialize(buf);
	    buf.finish_message();
	    LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	    ++i;
	}
}

void Dragon_Vale_Manager::process_offline_add_monster_to_scene_first_enter(int64_t creater) {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(creater);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	if(find_it->second->dragon_vale_view.thief_id_quality_map.empty()) {
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	uint8_t i = 0;

	for(Thief_Id_Quality_Map::const_iterator it = find_it->second->dragon_vale_view.thief_id_quality_map.begin();
			it != find_it->second->dragon_vale_view.thief_id_quality_map.end();
			++it) {
		if(i >= main_base_cfg.max_thief_num) {
			break;
		}
		MSG_20600015 msg;
		msg.type = 1;
		msg.role_id = creater;
		msg.id = it->first;
		msg.quality = it->second;
		Block_Buffer buf;
		buf.make_message(INNER_DRAGON_VALE_MONSTER);
	    msg.serialize(buf);
	    buf.finish_message();
	    LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	    ++i;
	}
}

void Dragon_Vale_Manager::process_refresh_special() {
	uint8_t chance = 100;
	Logic_Player *player = 0;
	Time_Value now = Time_Value::gettimeofday();

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	for(DB_View::ID_Player_View_Map::iterator it = LOGIC_MONITOR->db_view()->id_player_view_map.begin();
			it != LOGIC_MONITOR->db_view()->id_player_view_map.end();
			++it) {
		if(!it->second) {
			continue;
		}

		srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
		chance = random() % 100;

		player = LOGIC_MONITOR->find_logic_player_by_role_id(it->first);
		if(player) {
			bool is_special_before = player->dragon_vale_detail_no_const().is_special;
			player->dragon_vale_detail_no_const().is_special = false;
			if(player->dragon_vale_detail_no_const().is_yesterday_login) {
				if(chance < main_base_cfg.whole_server_special_chance) {
					player->dragon_vale_detail_no_const().is_special = true;
					MSG_82300028 msg;
				    OBJ_SEND_TO_CLIENT(msg, (*player));
				}
				else {
					player->dragon_vale_detail_no_const().is_special = false;
				}
			}

			if(is_special_before != player->dragon_vale_detail_no_const().is_special) {
				player->sync_special_to_db_view();
				save_special_to_db(player->role_id(), player->dragon_vale_detail_no_const().is_special);
				player->hook_dragon_vale_special_change();
			}
			continue;
		}

		bool is_special_before = it->second->dragon_vale_view.is_special;
		it->second->dragon_vale_view.is_special = false;
		if(it->second->dragon_vale_view.is_yesterday_login) {
			if(chance < main_base_cfg.whole_server_special_chance) {
				it->second->dragon_vale_view.is_special = true;
				Logic_Player::offline_dragon_vale_special_notice(it->second->role_id, true);
			}
			else {
				it->second->dragon_vale_view.is_special = false;
			}
		}

		if(is_special_before != it->second->dragon_vale_view.is_special) {
			save_special_to_db(it->second->role_id, it->second->dragon_vale_view.is_special);
			it->second->hook_dragon_vale_special_change();
		}
	}
}

void Dragon_Vale_Manager::save_special_to_db(int64_t role_id, bool is_special) {
	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_IS_SPECIAL);
	db_buf.write_int64(role_id);
	db_buf.write_bool(is_special);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);
}

void Dragon_Vale_Manager::process_save_notice_board_drive_robber(int64_t role_id, std::string rob_name) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			return;
		}

		if(!find_it->second) {
			return;
		}

		find_it->second->dragon_vale_view.check_and_change_notice_board_num();
		if(find_it->second->dragon_vale_view.notice_board_list.size() < 50) {
			Notice_Board notice_board;
			Time_Value now = Time_Value::gettimeofday();
			notice_board.type = NOTICE_BOARD_ROB;
			notice_board.time = now.sec();
			notice_board.host = rob_name;
			notice_board.param1 = 1;
			find_it->second->dragon_vale_view.notice_board_list.push_back(notice_board);
		}

		Block_Buffer db_buf;
		db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
		find_it->second->dragon_vale_view.serialize(db_buf);
		db_buf.finish_message();
		DB_MANAGER->push_data_block(db_buf, 0);

		return;
	}

	player->dragon_vale_detail_no_const().check_and_change_notice_board_num();
	if(player->dragon_vale_detail_no_const().notice_board_list.size() < 50) {
		Notice_Board notice_board;
		Time_Value now = Time_Value::gettimeofday();
		notice_board.type = NOTICE_BOARD_ROB;
		notice_board.time = now.sec();
		notice_board.host = rob_name;
		notice_board.param1 = 1;
		player->dragon_vale_detail_no_const().notice_board_list.push_back(notice_board);
	}

	((Dragon_Vale*)player)->save_dragon_vale_tick();
	player->sync_dragon_vale_to_db_view();
}

void Dragon_Vale_Manager::process_save_notice_board_rob_production(int64_t role_id, std::string rob_name,
						uint32_t gold_mine, uint32_t moon_well, uint32_t totem) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			return;
		}

		if(!find_it->second) {
			return;
		}

		find_it->second->dragon_vale_view.check_and_change_notice_board_num();
		if(find_it->second->dragon_vale_view.notice_board_list.size() < 50) {
			Notice_Board notice_board;
			Time_Value now = Time_Value::gettimeofday();
			notice_board.type = NOTICE_BOARD_ROB;
			notice_board.time = now.sec();
			notice_board.host = rob_name;
			notice_board.param1 = 2;
			notice_board.coin  = gold_mine;
			notice_board.soul = moon_well;
			notice_board.dragon_soul = totem;
			find_it->second->dragon_vale_view.notice_board_list.push_back(notice_board);
		}

		Block_Buffer db_buf;
		db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
		find_it->second->dragon_vale_view.serialize(db_buf);
		db_buf.finish_message();
		DB_MANAGER->push_data_block(db_buf, 0);

		return;
	}

	player->dragon_vale_detail_no_const().check_and_change_notice_board_num();
	if(player->dragon_vale_detail_no_const().notice_board_list.size() < 50) {
		Notice_Board notice_board;
		Time_Value now = Time_Value::gettimeofday();
		notice_board.type = NOTICE_BOARD_ROB;
		notice_board.time = now.sec();
		notice_board.host = rob_name;
		notice_board.param1 = 2;
		notice_board.coin  = gold_mine;
		notice_board.soul = moon_well;
		notice_board.dragon_soul = totem;
		player->dragon_vale_detail_no_const().notice_board_list.push_back(notice_board);
	}

	((Dragon_Vale*)player)->save_dragon_vale_tick();
	player->sync_dragon_vale_to_db_view();
}

void Dragon_Vale_Manager::process_enter_rob_protect_time(int64_t role_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			return;
		}

		if(!find_it->second) {
			return;
		}

		find_it->second->dragon_vale_view.is_rob_protect = true;
		Time_Value now = Time_Value::gettimeofday();
		find_it->second->dragon_vale_view.begin_rob_protect_time.set(now);

		Block_Buffer db_buf;
		db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
		find_it->second->dragon_vale_view.serialize(db_buf);
		db_buf.finish_message();
		DB_MANAGER->push_data_block(db_buf, 0);

		return;
	}

	player->dragon_vale_detail_no_const().is_rob_protect = true;
	Time_Value now = Time_Value::gettimeofday();
	player->dragon_vale_detail_no_const().begin_rob_protect_time.set(now);

	((Dragon_Vale*)player)->save_dragon_vale_tick();
	player->sync_dragon_vale_to_db_view();
}

void Dragon_Vale_Manager::process_rob_match_fail_back_coin(int64_t uuid, int64_t role_id, uint32_t coin, uint8_t type) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		return;
	}

	uint32_t gain_money_type = 0;
	if(1 == type) {
		gain_money_type = Pack::MONEY_ADD_ROB_MATCH_FAIL_BACK_COIN;
	}
	else {
		gain_money_type = Pack::MONEY_ADD_ROB_ATTCK_FAIL_BACK_COIN;
	}

	player->gain_money_in_dragon_vale(COPPER_ONLY, coin, gain_money_type, false);
}

void Dragon_Vale_Manager::process_clear_rob_protect_status(int64_t role_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			return;
		}

		if(!find_it->second) {
			return;
		}

		find_it->second->dragon_vale_view.is_rob_protect = false;

		Block_Buffer db_buf;
		db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
		find_it->second->dragon_vale_view.serialize(db_buf);
		db_buf.finish_message();
		DB_MANAGER->push_data_block(db_buf, 0);

		return;
	}

	player->dragon_vale_detail_no_const().is_rob_protect = false;
	((Dragon_Vale*)player)->save_dragon_vale_tick();
	player->sync_dragon_vale_to_db_view();
}

void Dragon_Vale_Manager::proecss_notify_tod_rob_production_finish(int64_t to_role_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(to_role_id);
	if(!player) {
		return;
	}
	player->achievement_listen_dragon_wale_rob_times();
}

void Dragon_Vale_Manager::process_set_first_enter_after_robbed(int64_t role_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			return;
		}

		if(!find_it->second) {
			return;
		}

		find_it->second->dragon_vale_view.is_first_enter_after_robbed = true;

		Block_Buffer db_buf;
		db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
		find_it->second->dragon_vale_view.serialize(db_buf);
		db_buf.finish_message();
		DB_MANAGER->push_data_block(db_buf, 0);

		return;
	}

	player->dragon_vale_detail_no_const().is_first_enter_after_robbed = true;

	((Dragon_Vale*)player)->save_dragon_vale_tick();
	player->sync_dragon_vale_to_db_view();
}

void Dragon_Vale_Manager::process_clear_rob_production_process(int64_t to_role_id) {
	Logic_Rob_Production_Map::iterator it = logic_rob_production_map.find(to_role_id);
	if(it != logic_rob_production_map.end()) {
		it->second.clear();
	}
	logic_rob_production_map.erase(to_role_id);
}

void Dragon_Vale_Manager::process_dragon_vale_reset() {
	Logic_Player *player = 0;
	const Main_Base_Cfg &main_base_cfg =  CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	for(DB_View::ID_Player_View_Map::iterator it = LOGIC_MONITOR->db_view()->id_player_view_map.begin();
			it != LOGIC_MONITOR->db_view()->id_player_view_map.end();
			++it) {
		if(!it->second) {
			continue;
		}

		Time_Value now = Time_Value::gettimeofday();
		Date_Time date_time(now);
		std::ostringstream oss;
		oss << date_time.year() << "-" << date_time.month() << "-" << date_time.day()
			<< "  " << date_time.hour() << ":" << date_time.minute() <<":" << date_time.second();
		MSG_DEBUG("DRAGON_VALE_SIX_REFRESH_ROLE: role_id=%ld, now=%s", it->second->role_id, oss.str().c_str());

		player = LOGIC_MONITOR->find_logic_player_by_role_id(it->second->role_id);
		if(player) {
			player->dragon_vale_detail_no_const().matched_num = 1;
			player->dragon_vale_detail_no_const().star_num = 0;
			player->dragon_vale_detail_no_const().award_foot_num = 0;
			player->dragon_vale_detail_no_const().foot_info_map.clear();
			player->dragon_vale_detail_no_const().foot_id_set.clear();
			player->dragon_vale_detail_no_const().dragon_heart_num = 0;
			player->dragon_vale_detail_no_const().play_dragon_num = 0;
			player->dragon_vale_detail_no_const().drive_thief_num = 0;

			player->sync_dragon_vale_reset_to_db_view();

			if(dragon_vale_role_id_map.find(player->role_id()) == dragon_vale_role_id_map.end()) {
				continue;
			}

			MSG_82300030 msg_82300030;
			msg_82300030.head_info.role_id = player->role_id();
			msg_82300030.head_info.role_name = player->role_name();
			msg_82300030.head_info.star_num = player->dragon_vale_detail().star_num;
			msg_82300030.head_info.head_photo_id = player->friend_detail().head_photo_id;
			const uint8_t *star_add_speed =
						CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(player->dragon_vale_detail().star_num);
			if(!star_add_speed) {
				msg_82300030.head_info.star_add_speed = 0;
				MSG_USER("star up spped not exist, star=%d", player->dragon_vale_detail().star_num);
			}
			else {
				msg_82300030.head_info.star_add_speed = (uint16_t)(*star_add_speed);
			}

			const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
			msg_82300030.head_info.thief_sub_speed =
				(uint16_t)(player->dragon_vale_detail().thief_id_quality_map.size() * main_base_cfg.thief_down_production);
			for(Foot_Info_Map::const_iterator it = player->dragon_vale_detail().foot_info_map.begin();
					it != player->dragon_vale_detail().foot_info_map.end();
					++it) {
				Dragon_Vale_Foot_Info msg_foot_info;
				msg_foot_info.sex = it->second.sex;
				msg_foot_info.name = it->second.name;
				msg_82300030.head_info.foot_list.push_back(msg_foot_info);
			}

			Block_Buffer buf_82300030;
			buf_82300030.make_message(msg_82300030.msg_id);
			msg_82300030.serialize(buf_82300030);
			buf_82300030.finish_message();

			Block_Buffer broad_cast_buf;
			broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
			broad_cast_buf.write_int64(player->role_id());
			broad_cast_buf.copy(&buf_82300030);
			broad_cast_buf.finish_message();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

			continue;
		}//end if

		it->second->dragon_vale_view.matched_num = 1;
		it->second->dragon_vale_view.star_num = 0;
		it->second->dragon_vale_view.award_foot_num = 0;
		it->second->dragon_vale_view.foot_info_map.clear();
		it->second->dragon_vale_view.foot_id_set.clear();
		it->second->dragon_vale_view.dragon_heart_num = 0;
		it->second->dragon_vale_view.play_dragon_num = 0;
		it->second->dragon_vale_view.drive_thief_num = 0;

		if(dragon_vale_role_id_map.find(it->second->role_id) == dragon_vale_role_id_map.end()) {
			continue;
		}

		MSG_82300030 msg_82300030;
		msg_82300030.head_info.role_id = it->second->role_id;
		msg_82300030.head_info.role_name = it->second->role_name;
		msg_82300030.head_info.star_num = it->second->dragon_vale_view.star_num;
		msg_82300030.head_info.head_photo_id = it->second->friend_.head_photo_id;
		const uint8_t *star_add_speed =
					CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(it->second->dragon_vale_view.star_num);
		if(!star_add_speed) {
			msg_82300030.head_info.star_add_speed = 0;
			MSG_USER("star up spped not exist, star=%d", it->second->dragon_vale_view.star_num);
		}
		else {
			msg_82300030.head_info.star_add_speed = (uint16_t)(*star_add_speed);
		}

		msg_82300030.head_info.thief_sub_speed =
			(uint16_t)(it->second->dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);
		for(Foot_Info_Map::const_iterator foot_it = it->second->dragon_vale_view.foot_info_map.begin();
				foot_it != it->second->dragon_vale_view.foot_info_map.end();
				++foot_it) {
			Dragon_Vale_Foot_Info msg_foot_info;
			msg_foot_info.sex = foot_it->second.sex;
			msg_foot_info.name = foot_it->second.name;
			msg_82300030.head_info.foot_list.push_back(msg_foot_info);
		}

		Block_Buffer buf_82300030;
		buf_82300030.make_message(msg_82300030.msg_id);
		msg_82300030.serialize(buf_82300030);
		buf_82300030.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(it->second->role_id);
		broad_cast_buf.copy(&buf_82300030);
		broad_cast_buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);
	}//end for

	uint32_t matched_num = 1;
	uint8_t star_num = 0;
	uint8_t award_foot_num = 0;
	Foot_Info_Map foot_info_map;
	foot_info_map.clear();
	Foot_Id_Set foot_id_set;
	foot_id_set.clear();
	uint8_t dragon_heart_num = 0;
	uint32_t  play_dragon_num = 0;
	uint32_t  drive_thief_num = 0;

	{
		Time_Value now = Time_Value::gettimeofday();
		Date_Time date_time(now);
		std::ostringstream oss;
		oss << date_time.year() << "-" << date_time.month() << "-" << date_time.day()
			<< "  " << date_time.hour() << ":" << date_time.minute() <<":" << date_time.second();
		MSG_DEBUG("save_dragon_vale_reset_to_db: now=%s", oss.str().c_str());
	}

	save_dragon_vale_reset_to_db(0, matched_num, star_num, award_foot_num, foot_info_map, foot_id_set,
								dragon_heart_num, play_dragon_num, drive_thief_num);
}

void Dragon_Vale_Manager::save_dragon_vale_reset_to_db(int64_t role_id, uint32_t matched_num, uint8_t star_num, uint8_t award_foot_num,
		const Foot_Info_Map &foot_info_map, const Foot_Id_Set &foot_id_set, uint8_t dragon_heart_num, const uint32_t play_dragon_num,
		uint32_t  drive_thief_num) {
	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_RESET);
	db_buf.write_int64(role_id);
	db_buf.write_uint32(matched_num);
	db_buf.write_uint8(star_num);
	db_buf.write_uint8(award_foot_num);

	uint8_t size = foot_info_map.size();
	db_buf.write_uint8(size);
	for(Foot_Info_Map::const_iterator it = foot_info_map.begin();
			it != foot_info_map.end();
			++it) {
		db_buf.write_int64(it->first);
		db_buf.write_uint8(it->second.sex);
		db_buf.write_string(it->second.name);
	}

	size = foot_id_set.size();
	db_buf.write_uint8(size);
	for(Foot_Id_Set::const_iterator it = foot_id_set.begin();
			it != foot_id_set.end();
			++it) {
		db_buf.write_int64(*it);
	}

	db_buf.write_uint8(dragon_heart_num);
	db_buf.write_uint32(play_dragon_num);
	db_buf.write_uint32(drive_thief_num);

	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);
}

void Dragon_Vale_Manager::trigger_daily_six(const Time_Value &now) {
	Date_Time date_time(now);
	std::ostringstream oss;
	oss << date_time.year() << "-" << date_time.month() << "-" << date_time.day()
		<< "  " << date_time.hour() << ":" << date_time.minute() <<":" << date_time.second();
	MSG_DEBUG("DRAGON_VALE_SIX_REFRESH: now=%s", oss.str().c_str());
	process_refresh_special();
	process_dragon_vale_reset();
}

void Dragon_Vale_Manager::process_refresh_special_test(int64_t role_id, bool flag) {
	Logic_Player *player = 0;
	Time_Value now = Time_Value::gettimeofday();

	player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(player) {
		bool is_special_before = player->dragon_vale_detail_no_const().is_special;
		player->dragon_vale_detail_no_const().is_special = flag;
		if(flag) {
			MSG_82300028 msg;
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}

		if(is_special_before !=  player->dragon_vale_detail_no_const().is_special) {
			player->sync_special_to_db_view();
			save_special_to_db(player->role_id(), player->dragon_vale_detail_no_const().is_special);
			player->hook_dragon_vale_special_change();
		}
		return;
	}

	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	bool is_special_before = find_it->second->dragon_vale_view.is_special;
	find_it->second->dragon_vale_view.is_special = flag;

	if(is_special_before != find_it->second->dragon_vale_view.is_special) {
		if(flag) {
			Logic_Player::offline_dragon_vale_special_notice(find_it->second->role_id, true);
		}
		save_special_to_db(find_it->second->role_id, find_it->second->dragon_vale_view.is_special);
		find_it->second->hook_dragon_vale_special_change();
	}
}

void Dragon_Vale_Manager::refresh_dragon_vale_op(const int64_t target_id) {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	DB_View::ID_Player_View_Map::const_iterator target_it  =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(target_id);
	if(target_it == LOGIC_MONITOR->db_view()->id_player_view_map.end() || !target_it->second) {
		return;
	}

	Logic_Player *player = 0;

	boost::unordered_set<role_id_t> temp_role_id_set;
	temp_role_id_set.clear();

	for(Role_Id_Set::const_iterator it = target_it->second->friend_.firend_list_.begin();
			it != target_it->second->friend_.firend_list_.end();
			++it) {
		if(*it == target_id) {
			continue;
		}

		player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if(!player) {
			continue;
		}

		if(!player->dragon_vale_detail().target_player_id) {
			continue;
		}

		uint8_t op = 0;
		if(!target_it->second->dragon_vale_view.thief_id_quality_map.empty()) {
			op  = 3;
		}

		if(target_it->second->dragon_vale_view.dragon_info.is_play_dragon) {
			op = 2;
		}

		if(player->dragon_vale_detail().foot_id_set.find(target_id) == player->dragon_vale_detail().foot_id_set.end()
				&& player->dragon_vale_detail().award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
			op = 1;
		}

		MSG_82300037 msg;
		msg.target_id = target_id;
		msg.op = op;
	    OBJ_SEND_TO_CLIENT(msg, (*player));

	    temp_role_id_set.insert(player->role_id());
	}//end for

	const Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(target_it->second->gang_id);
	if (!gang_view) {
		return;
	}

	for (Gang_DB_View::Member_Map::const_iterator it = gang_view->member_map.begin();
			it != gang_view->member_map.end(); ++it) {
		if(it->second.role_id == target_id) {
			continue;
		}

		if(temp_role_id_set.find(it->second.role_id) != temp_role_id_set.end()) {
			continue;
		}

		player = LOGIC_MONITOR->find_logic_player_by_role_id(it->second.role_id);
		if(!player) {
			continue;
		}

		if(!player->dragon_vale_detail().target_player_id) {
			continue;
		}

		uint8_t op = 0;
		if(!target_it->second->dragon_vale_view.thief_id_quality_map.empty()) {
			op  = 3;
		}

		if(target_it->second->dragon_vale_view.dragon_info.is_play_dragon) {
			op = 2;
		}

		if(player->dragon_vale_detail().foot_id_set.find(target_id) == player->dragon_vale_detail().foot_id_set.end()
				&& player->dragon_vale_detail().award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
			op = 1;
		}

		MSG_82300037 msg;
		msg.target_id = target_id;
		msg.op = op;
	    OBJ_SEND_TO_CLIENT(msg, (*player));
	}//end for

	//clear
	temp_role_id_set.clear();
}

void Dragon_Vale_Manager::process_dragon_vale_rob(int64_t to_uuid, int64_t to_role_id, std::string to_server_name, std::string to_role_name,
							int64_t uuid, int64_t role_id, std::string server_name, std::string role_name, uint32_t coin, uint8_t flag) {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	if(0 == flag) {
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
		if(!player) {
			MSG_20600031 msg;
			msg.to_uuid = to_uuid;
			msg.to_role_id = to_role_id;
			msg.to_server_name = to_server_name;
			msg.to_role_name = to_role_name;
			msg.uuid = uuid;
			msg.role_id = role_id;
			msg.server_name = server_name;
			msg.role_name = role_name;
			msg.coin = coin;
			msg.flag = 0;
			Block_Buffer buf;
			buf.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
			msg.serialize(buf);
			buf.finish_message();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
			return;
		}

		const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player->location().scene_id);
		if(!scene_config) {
			MSG_20600031 msg;
			msg.to_uuid = to_uuid;
			msg.to_role_id = to_role_id;
			msg.to_server_name = to_server_name;
			msg.to_role_name = to_role_name;
			msg.uuid = uuid;
			msg.role_id = role_id;
			msg.server_name = server_name;
			msg.role_name = role_name;
			msg.coin = coin;
			msg.flag = 0;
			Block_Buffer buf;
			buf.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
			msg.serialize(buf);
			buf.finish_message();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
			return;
		}

		if(player->get_player_in_trans_all() ||
				((Dragon_Vale_Type != scene_config->type)  && (Main_Scene_Type != scene_config->type))) {
			MSG_20600031 msg;
			msg.to_uuid = to_uuid;
			msg.to_role_id = to_role_id;
			msg.to_server_name = to_server_name;
			msg.to_role_name = to_role_name;
			msg.uuid = uuid;
			msg.role_id = role_id;
			msg.server_name = server_name;
			msg.role_name = role_name;
			msg.coin = coin;
			msg.flag = 0;
			Block_Buffer buf;
			buf.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
			msg.serialize(buf);
			buf.finish_message();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
			return;
		}

		MSG_20600031 msg;
		msg.to_uuid = to_uuid;
		msg.to_role_id = to_role_id;
		msg.to_server_name = to_server_name;
		msg.to_role_name = to_role_name;
		msg.uuid = uuid;
		msg.role_id = role_id;
		msg.server_name = server_name;
		msg.role_name = role_name;
		msg.coin = coin;
		msg.flag = 1;
		Block_Buffer buf;
		buf.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
		msg.serialize(buf);
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(player->location().scene_id , 0, buf);
		return;
	}//end if 0

	if(1 == flag) {
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
		if(!player) {
			MSG_20600031 msg;
			msg.to_uuid = to_uuid;
			msg.to_role_id = to_role_id;
			msg.to_server_name = to_server_name;
			msg.to_role_name = to_role_name;
			msg.uuid = uuid;
			msg.role_id = role_id;
			msg.server_name = server_name;
			msg.role_name = role_name;
			msg.coin = coin;
			msg.flag = 0;
			Block_Buffer buf;
			buf.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
			msg.serialize(buf);
			buf.finish_message();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
			return;
		}

		{
			MSG_20600031 msg;
			msg.to_uuid = to_uuid;
			msg.to_role_id = to_role_id;
			msg.to_server_name = to_server_name;
			msg.to_role_name = to_role_name;
			msg.uuid = uuid;
			msg.role_id = role_id;
			msg.server_name = server_name;
			msg.role_name = role_name;
			msg.coin = coin;
			msg.flag = 2;
			Block_Buffer buf;
			buf.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
			msg.serialize(buf);
			buf.finish_message();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		}

		{
			MSG_82300026 msg_82300026;
			msg_82300026.left_time = 10 + main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;
			OBJ_SEND_TO_CLIENT(msg_82300026, (*player));
		}

		{
			MSG_82300012 msg;
			msg.uuid = to_uuid;
			msg.server_name = to_server_name;
			msg.role_id = to_role_id;
			msg.role_name = to_role_name;
			msg.limit_time = 5;
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}

		return;
	}//end if 1

	if(2 == flag) {
		MSG_20600031 msg;
		msg.to_uuid = to_uuid;
		msg.to_role_id = to_role_id;
		msg.to_server_name = to_server_name;
		msg.to_role_name = to_role_name;
		msg.uuid = uuid;
		msg.role_id = role_id;
		msg.server_name = server_name;
		msg.role_name = role_name;
		msg.coin = coin;
		msg.flag = 0;
		Block_Buffer buf;
		buf.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
		msg.serialize(buf);
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);
		return;
	}//end if 2
}

void Dragon_Vale_Manager::notify_master_of_dragon_fight_end(std::string server_name, std::string player_name,
											uint8_t result, uint8_t is_drive, int64_t role_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		return;
	}

	MSG_82300015 msg_82300015;
	msg_82300015.reset();
	msg_82300015.server_name = server_name;
	msg_82300015.player_name  = player_name;
	msg_82300015.result = result;
	msg_82300015.is_drive = is_drive;
	Block_Buffer buf;
	buf.make_message(ACTIVE_NOTIFY_MASTER_OF_DRAGON_FIGHT_END);
	msg_82300015.serialize(buf);
	buf.finish_message();
	player->send_to_client(buf);
}

void Dragon_Vale_Manager::notify_draong_vale_no_enter(uint32_t left_time, int64_t role_id) {
	Time_Value now = Time_Value::gettimeofday();
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		DB_View::ID_Player_View_Map::const_iterator find_it  =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end() || !find_it->second) {
			return;
		}

		find_it->second->dragon_vale_view.begin_no_enter_time.set(now);
		find_it->second->dragon_vale_view.left_time = left_time;

		Block_Buffer db_buf;
		db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
		find_it->second->dragon_vale_view.serialize(db_buf);
		db_buf.finish_message();
		DB_MANAGER->push_data_block(db_buf, 0);

		return;
	}

	MSG_82300026 msg_82300026;
	msg_82300026.left_time = left_time;
	Block_Buffer buf;
	buf.make_message(ACTIVE_DRAONG_VALE_NO_ENTER);
	msg_82300026.serialize(buf);
	buf.finish_message();
	player->send_to_client(buf);

	player->dragon_vale_detail_no_const().begin_no_enter_time.set(now);
	player->dragon_vale_detail_no_const().left_time = left_time;
	player->save_dragon_vale_tick();
	player->sync_dragon_vale_to_db_view();
}

void Dragon_Vale_Manager::process_target_in_defence_dragon_vale_fight(int64_t role_id, uint8_t flag) {
	if(1 == flag) {
		battle_role_id_set.insert(role_id);
	}
	else {
		battle_role_id_set.erase(role_id);
	}
}

void Dragon_Vale_Manager::process_clear_battle_role_id_set() {
	battle_role_id_set.clear();
}

uint8_t Dragon_Vale_Manager::get_dragon_vale_refresh_monster_quality() {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->get_main_config().main_base_cfg;
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	int num = random() % 100;
	int curr_left_border = 0;
	int next_left_border = main_base_cfg.white_chance;

	if(num >= curr_left_border && num <= (next_left_border - 1)) {
		return (uint8_t)THIEF_QUALITY_WHITE;
	}

	curr_left_border = next_left_border;
	next_left_border += main_base_cfg.blue_chance;
	if(num >= curr_left_border && num <= (next_left_border - 1)) {
		return (uint8_t)THIEF_QUALITY_BLUE;
	}

	curr_left_border = next_left_border;
	next_left_border += main_base_cfg.orange_chance;
	if(num >= curr_left_border && num <= next_left_border - 1) {
		return (uint8_t)THIEF_QUALITY_ORANGE;
	}

	return THIEF_QUALITY_WHITE;
}
