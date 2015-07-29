/*
 * Dragon_Vale_Manager.h
 *
 *  Created on: 2014年7月26日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_MANAGER_H_
#define DRAGON_VALE_MANAGER_H_

#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"
#include "Singleton.h"
#include "Logic_Player_Detail.h"

class Logic_Player;

struct MSG_20600010;
struct Player_DB_View;

typedef boost::unordered_set<int64_t> Enter_Role_Id_Set;
typedef boost::unordered_map<int64_t, Enter_Role_Id_Set> Dragon_Vale_Role_Id_Map;
typedef boost::unordered_set<int64_t> Battle_Role_Id_Set;

class Dragon_Vale_Manager {
public:
	Dragon_Vale_Manager() {
		reset();
	}
	void reset();
	void tick(const Time_Value &now);
	inline  Dragon_Vale_Role_Id_Map& get_dragon_vale_role_id_map();
	inline const Battle_Role_Id_Set& get_battle_role_id_set() const;

	int match_level(uint32_t level, uint32_t &begin_level, uint32_t &end_level);
	int process_rob_match(int64_t uuid, int64_t role_id, uint32_t level, uint32_t coin,
							std::string server_name, std::string role_name, uint32_t last_coin);
	void process_rob_match(Logic_Player_Detail* logic_player_detail);
	int process_rob_fight_load_player_data(int64_t to_uuid, int64_t to_role_id,
										   std::string to_server_name, std::string to_role_name, int64_t role_id);
	void process_rob_fight_load_player_data(Logic_Player_Detail* logic_player_detail);
	void porocss_rob_enter_dragon_vale(Logic_Player &player);
	void process_rob_enter_dragon_vale(int64_t to_uuid, int64_t to_role_id, int64_t role_id, uint8_t limit_time);
	void process_rob_enter_offline_player_dragon_vale(int64_t to_uuid, int64_t to_role_id, int64_t role_id, uint8_t limit_time);
	void process_gain_rob_production(int64_t to_uuid, int64_t to_role_id,
									std::string to_server_name,std::string to_role_name,
									int64_t uuid, int64_t role_id, uint32_t building_id, uint8_t building_type);
	void process_gain_rob_offline_player_production(int64_t to_uuid, int64_t to_role_id,
									std::string to_server_name, std::string to_role_name, int64_t uuid, int64_t role_id,
									uint32_t building_id, uint8_t building_type);
	void process_gain_rob_production_res(const MSG_20600010 &msg);
	void process_enter_dragon_vale(int64_t creater, int64_t role_id);
	void process_exit_dragon_vale(int64_t creater, int64_t role_id);
	void process_drive_thief(int64_t player_id, int64_t role_id, int64_t id, uint8_t quality);
	void process_drive_offline_dragon_vale_thief(int64_t player_id, int64_t role_id, int64_t id, uint8_t quality);
	void process_offline_tick(Player_DB_View &player_db_view, const Time_Value &now);
	void process_add_monster_to_scene_first_enter(int64_t creater);
	void process_offline_add_monster_to_scene_first_enter(int64_t creater);
	void listen_offline_building_output_production_by_one_minute(Player_DB_View &player_db_view,
														const Time_Value &now);
	void listen_offline_building_finish(Player_DB_View &player_db_view, const Time_Value &now);
	void listen_offline_dragon_born_finish(Player_DB_View &player_db_view, const Time_Value &now);
	void listen_offline_play_dragon_interval_time(Player_DB_View &player_db_view, const Time_Value &now);
	void listen_offline_dragon_feeling_down_time(Player_DB_View &player_db_view, const Time_Value &now);
	void listen_offline_dragon_rob_protect_time(Player_DB_View &player_db_view, const Time_Value &now);
	void listen_offline_refresh_monster(Player_DB_View &player_db_view, const Time_Value &now);
	void process_refresh_special();
	void save_special_to_db(int64_t role_id, bool is_special);
	void process_save_notice_board_drive_robber(int64_t role_id, std::string rob_name);
	void process_save_notice_board_rob_production(int64_t role_id, std::string rob_name,
							uint32_t gold_mine, uint32_t moon_well, uint32_t totem);
	void process_enter_rob_protect_time(int64_t role_id);
	void process_rob_match_fail_back_coin(int64_t uuid, int64_t role_id, uint32_t coin, uint8_t type);
	void process_clear_rob_protect_status(int64_t role_id);
	void process_clear_rob_production_process(int64_t to_role_id);
	void proecss_notify_tod_rob_production_finish(int64_t to_role_id);
	void process_set_first_enter_after_robbed(int64_t role_id);
	void process_dragon_vale_reset();
	void save_dragon_vale_reset_to_db(int64_t role_id, uint32_t matched_num, uint8_t star_num, uint8_t award_foot_num,
			const Foot_Info_Map &foot_info_map, const Foot_Id_Set &foot_id_set, uint8_t dragon_heart_num,
			const uint32_t play_dragon_num, uint32_t  drive_thief_num);
	void trigger_daily_six(const Time_Value &now);
	void process_refresh_special_test(int64_t role_id, bool flag);
	void refresh_dragon_vale_op(const int64_t target_id);
	void process_dragon_vale_rob(int64_t to_uuid, int64_t to_role_id, std::string to_server_name, std::string to_role_name,
								int64_t uuid, int64_t role_id, std::string server_name, std::string role_name, uint32_t coin, uint8_t flag);
	void notify_master_of_dragon_fight_end(std::string server_name, std::string player_name, uint8_t result,
										uint8_t is_drive, int64_t role_id);
	void notify_draong_vale_no_enter(uint32_t left_time, int64_t role_id);
	void process_target_in_defence_dragon_vale_fight(int64_t role_id, uint8_t flag);
	void process_clear_battle_role_id_set();
	uint8_t get_dragon_vale_refresh_monster_quality();

private:
	Dragon_Vale_Role_Id_Map dragon_vale_role_id_map;
	Logic_Rob_Production_Map logic_rob_production_map;
	Battle_Role_Id_Set battle_role_id_set;
};

Dragon_Vale_Role_Id_Map& Dragon_Vale_Manager::get_dragon_vale_role_id_map() {
	return dragon_vale_role_id_map;
}

const Battle_Role_Id_Set& Dragon_Vale_Manager::get_battle_role_id_set() const {
	return battle_role_id_set;
}

typedef Singleton<Dragon_Vale_Manager> Dragon_Vale_Manager_Single;
#define DRAGON_VALE_MANAGER   (Dragon_Vale_Manager_Single::instance())

#endif /* DRAGON_VALE_MANAGER_H_ */
