/*
 * Inner_Arena_Manager.h
 *
 *  Created on: 2014年9月11日
 *      Author: root
 */

#ifndef INNER_ARENA_MANAGER_H_
#define INNER_ARENA_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "boost/unordered_map.hpp"
#include "Inner_Arena_Struct.h"

class Inner_Arena_Manager{
public:
	typedef boost::unordered_map<role_id_t, Inner_Arena_Rank_Player*> Arena_Rank_Player_Map;
	typedef boost::unordered_map<int, Inner_Arena_Rank_Player*> Arena_Ranked_Map;
	//typedef std::set<Inner_Arena_Rank_Player*> Ready_Player_Set;
	//typedef std::set<Arena_Rank_Player*, Arena_Rank_Player_Greater> Arena_Ranking_Set;
	typedef std::vector<Inner_Arena_Rank_Player*> Arena_Ranking_Vec;
	//typedef boost::unordered_map<role_id_t, Logic_Player_Detail*> PLAYER_DATA_CACHE_MAP;
	typedef std::map<int, Arena_Ranking_Vec> Group_Rank_Map;
	typedef boost::unordered_map<role_id_t, Monitor_Unique_ID> Arena_Rank_Muid_Map;
	typedef boost::unordered_set<Monitor_Unique_ID> Arena_Rank_Muid_Set;
	typedef boost::unordered_map<role_id_t, std::vector<role_id_t>> Rank_Load_Data_Map;
public:
	Inner_Arena_Manager(void);
	~Inner_Arena_Manager(void);
public:
	void reset(void);
public:
	// 同步所有玩家到跨服
	int inner_arena_sync_rank_info(Block_Buffer& buf);
	// 匹配跨服玩家
	int inner_arena_match_player(Block_Buffer& buf);

	int inner_arena_fetch_fight_data(Block_Buffer& buf);

	int inner_arena_fetch_rank_info(Block_Buffer& buf);

	int inner_arena_battle_result(Block_Buffer& buf);

	int inner_arena_season_finish(Block_Buffer& buf);
public:
	void sort_rank(bool first_times = false);
	Inner_Arena_Rank_Player* get_rank_player(role_id_t role_id);
	Inner_Arena_Rank_Player* find_rank_player(role_id_t role_id);
public:
	Inner_Arena_Rank_Player* get_random_player(Inner_Arena_Rank_Player* rank_player);
	Inner_Arena_Rank_Player* get_rank_player_from_rank(int rank);

private:
	void match_player(Inner_Arena_Rank_Player* fight_player, int highest_force, const int rise, const int group, std::vector<Arena_Match_P> &match_players);
	void match_king_group(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_first_group_rise(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_nor_group_rise(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_nor_group_rise1(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_nor_group_rise2(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_low_group_fight(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players,
			const role_id_t df_role_id_1 = 0, const role_id_t df_role_id_2 = 0);
	void match_low_fight(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players,
			const role_id_t df_role_id_1 = 0, const role_id_t df_role_id_2 = 0);

	bool match_difficult_hard(Inner_Arena_Rank_Player* fight_player,int highest_force, const role_id_t god_role_id, role_id_t& rt_match_id);
	bool match_difficult_god(Inner_Arena_Rank_Player* fight_player,int highest_force, role_id_t& rt_match_id);
private:
	int64_t get_rank_player_uuid(role_id_t role_id);
	bool get_load_data_player(const role_id_t match_role_id, std::vector<role_id_t>& rank_role_id);
	void add_load_data_player(const role_id_t match_role_id, const role_id_t rank_role_id);
	void del_load_data_player(const role_id_t match_role_id);
	void sync_inner_rank_info(Arena_Ranking_Vec& rank_vec);
private:
	Arena_Rank_Player_Map rank_player_map_;
	Arena_Ranked_Map ranked_map_;
	//Arena_Ranking_Set ranking_set_;
	Arena_Ranking_Vec ranking_vec_;
	Arena_Rank_Muid_Set rank_muid_set_;
	Rank_Load_Data_Map rank_load_data_map_;
private:
	int get_arena_seg_key(int group, int seg);
	void arena_seg_add(role_id_t role_id, int seg_key, int force);
	void arena_seg_del(role_id_t role_id, int seg_key, int force);
	void arena_seg_change(role_id_t role_id, int old_seg_key, int old_force, int new_seg_key, int new_force);
	bool is_on_top_seg_key(const int group, const int seg);
private:
	Arena_Seg_Map arena_seg_map_;

};

class Inner_Arena_Area_Manger{
	typedef boost::unordered_map<int, Inner_Arena_Manager*> Inner_Arena_Manager_Map;
public:
	Inner_Arena_Area_Manger(void);
	~Inner_Arena_Area_Manger(void);
public:
	// 同步所有玩家到跨服
	int inner_arena_sync_rank_info(Block_Buffer& buf);
	// 匹配跨服玩家
	int inner_arena_match_player(Block_Buffer& buf);

	int inner_arena_fetch_fight_data(Block_Buffer& buf);

	int inner_arena_fetch_rank_info(Block_Buffer& buf);

	int inner_arena_battle_result(Block_Buffer& buf);

	int inner_arena_season_finish(Block_Buffer& buf);
private:
	Inner_Arena_Manager* get_inner_arena_manager(int inner_area);
private:
	Inner_Arena_Manager_Map inner_arena_manager_map_;
};

typedef Singleton<Inner_Arena_Area_Manger> Inner_Arena_Area_Manger_Single;
#define INNER_ARENA_AREA_MANAGER (Inner_Arena_Area_Manger_Single::instance())

#endif /* INNER_ARENA_MANAGER_H_ */
