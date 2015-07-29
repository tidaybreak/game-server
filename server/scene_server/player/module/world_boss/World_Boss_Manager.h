/*
 * World_Boss_Manager.h
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#ifndef WORLD_BOSS_MANAGER_H_
#define WORLD_BOSS_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "world_boss/World_Boss_Struct.h"
#include "world_boss/World_Boss_Scene_Struct.h"

class World_Boss_Manager{
	enum Act_State{
		ACT_STATE_NULL = 0,
		ACT_STATE_START = 1,
		ACT_STATE_END = 2,
	};
	typedef boost::unordered_map<role_id_t, World_Boss_Rank_Info> Player_Info_Map;
public:
	World_Boss_Manager(void);
	~World_Boss_Manager(void);
public:
	void reset(void);
	void set_icon_id(const int8_t icon_id);
public:
	int req_world_boss_act_guild_bless(Scene_Player* player);
	int req_world_boss_use_ability(Scene_Player* player, int type);
	int req_fight_world_boss(Scene_Player* player);
	int inner_player_enter(const role_id_t gang_id, Block_Buffer& buf);
	int inner_sync_info(Scene_Player* player, Block_Buffer& buf);
	int inner_act_start(const role_id_t gang_id, Block_Buffer& buf);
	int inner_act_end(const role_id_t gang_id, Block_Buffer& buf);
public:
	World_Boss_Hp_Info& boss_info(void);
	World_Boss_Player_Info* get_player_info(const role_id_t role_id);
	bool find_player(const role_id_t role_id);
	const World_Boss_Hp_Info* get_boss_info(void);
	bool check_first_blood(Scene_Player* player);
	void refresh_rank(void);
	void set_player_revive_time(const role_id_t role_id, const int time);
	bool player_is_revive(role_id_t role_id);
	void player_act_guild_bless(Scene_Player* player);
	void player_damage(const role_id_t role_id, const int damage, int& bonus_id, role_id_t& bonus_role_id, std::string& bonus_role_name);
	void get_first_rank_info(role_id_t& role_id_1, std::string& role_name_1, role_id_t& role_id_2, std::string& role_name_2, role_id_t& role_id_3, std::string& role_name_3);
	void listen_gang_info_sync(const gang_id_t src_gang_id, Scene_Player* player);
	void listen_vip_info_change(Scene_Player* player);
private:
	void del_player_info(const role_id_t role_id);
public:
	int8_t icon_id_;
	int act_state_;
	role_id_t first_blood_role_id_;					// 一血玩家
	World_Boss_Detail act_detail_;
	int act_fight_time_;
	Time_Value act_end_time_;

	World_Boss_Hp_Info boss_info_;
	World_Boss_Player_Info_Map player_info_map_;
	World_Boss_Gang_Bless_Map gang_bless_map_;
	std::vector<World_Boss_Player_Info*> rank_info_;
	std::vector<World_Boss_Rank_Info> rank_show_info_;
	std::vector<World_Boss_Rank_Info> rank_show_info_1;
	std::vector<World_Boss_Rank_Info> rank_show_info_2;
	std::vector<World_Boss_Rank_Info> rank_show_info_3;
};

typedef Singleton<World_Boss_Manager> World_Boss_Manager_Single;
#define WORLD_BOSS_MANAGER (World_Boss_Manager_Single::instance())

#endif /* WORLD_BOSS_MANAGER_H_ */
