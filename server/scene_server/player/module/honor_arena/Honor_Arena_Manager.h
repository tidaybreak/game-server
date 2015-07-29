/*
 * Honor_Arena_Manager.h
 *
 *  Created on: 2015年3月16日
 *      Author: root
 */

#ifndef HONOR_ARENA_MANAGER_H_
#define HONOR_ARENA_MANAGER_H_


#include "Game_Typedef.h"
#include "Singleton.h"
#include "honor_arena/Honor_Arena_Struct.h"

class Scene_Player;

class Honor_Arena_Manager{
	enum Act_State{
		ACT_STATE_NULL = 0,
		ACT_STATE_START = 1,
		ACT_STATE_END = 2,
	};
public:
	Honor_Arena_Manager(void);
	~Honor_Arena_Manager(void);
public:
	void reset(void);
	void set_icon_id(const int8_t icon_id);
	int tick(const Time_Value &now);
public:
	int inner_player_enter(const role_id_t gang_id, Block_Buffer& buf);
	int inner_sync_info(Scene_Player* player, Block_Buffer& buf);
	int inner_act_start(const role_id_t gang_id, Block_Buffer& buf);
	int inner_act_end(const role_id_t gang_id, Block_Buffer& buf);

	int req_match(Scene_Player* player);
	int req_receive(Scene_Player* player, const int type);
	int req_rank(Scene_Player* player);
	int req_panel_info(Scene_Player* player);
	int req_set_auto_match(Scene_Player* player, const int8_t is_auto);
public:
	Honor_Arena_Player_Info* get_player_info(const role_id_t role_id);
	Honor_Arena_Player_Info* get_player_info_from_rank(const int rank);
	bool find_player(const role_id_t role_id);
	void refresh_rank(void);
	void player_enter_scene(Scene_Player* player);
	void player_exit_scene(Scene_Player* player);
private:
	void make_match(void);
	void check_rank_change(void);
	void check_fight_teams(const Time_Value& now);
	int start_fight(const role_id_t role_id_1, const role_id_t role_id_2);
	void send_rank_change(Honor_Arena_Player_Info* player_info);
public:
	int8_t icon_id_;
	int act_state_;
	int act_fight_time_;
	Time_Value act_end_time_;

	Honor_Arena_Player_Info_Map player_info_map_;
	std::vector<Honor_Arena_Player_Info*> rank_info_;
	std::vector<Honor_Arena_Player_Info*> match_rank_info_;
	std::vector<Honor_Arena_Match_Team> fight_teams_;
	int match_tick_num_;
	int check_rank_change_tick_;
};

typedef Singleton<Honor_Arena_Manager> Honor_Arena_Manager_Single;
#define HONOR_ARENA_MANAGER (Honor_Arena_Manager_Single::instance())



#endif /* HONOR_ARENA_MANAGER_H_ */
