/*
 * Arena_Manager.h
 *
 *  Created on: 2014年5月16日
 *      Author: lys
 */

#ifndef ARENA_MANAGER_H_
#define ARENA_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "boost/unordered_map.hpp"
#include "Arena_Struct.h"
#include "Arena_Rank_Player.h"

struct Arena_Rank_Info;
struct MSG_50100405;
struct MSG_50100420;
struct MSG_50100410;
class Logic_Player;
struct Logic_Player_Detail;
struct Arena_Award_Config;
struct Arena_Season_Config;

enum {
	BATTLE_REPORT_WIN = 0,
	BATTLE_REPORT_LOSE = 1,
	BATTLE_REPORT_RISE_WIN = 2,
	BATTLE_REPORT_RISE_LOSE = 3,
	BATTLE_REPORT_STREAK_WIN = 4,
	BATTLE_REPORT_RANK_UP    = 6,
	BATTLE_REPORT_RANK_DOWN  = 7,
};

class Arena_Manager{
public:
	typedef boost::unordered_map<role_id_t, Arena_Rank_Player*> Arena_Rank_Player_Map;
	typedef boost::unordered_map<int, Arena_Rank_Player*> Arena_Ranked_Map;
	typedef std::set<Arena_Rank_Player*> Ready_Player_Set;
	//typedef std::set<Arena_Rank_Player*, Arena_Rank_Player_Greater> Arena_Ranking_Set;
	typedef std::vector<Arena_Rank_Player*> Arena_Ranking_Vec;
	typedef std::map<int, Arena_Ranking_Vec> Group_Rank_Map;
	typedef boost::unordered_map<role_id_t, std::vector<role_id_t>> Load_Data_Player_Map;
public:
	Arena_Manager(void);
	~Arena_Manager(void);
public:
	void reset(void);
	int start(void);
	int tick(const Time_Value &now);

	int trigger_daily_twenty(const Time_Value& now);

	void daily_refresh(bool check_one_day = false);

	bool is_arena_running(void);

	void sign_in(Logic_Player* player);
	void sign_out(Logic_Player* player);

	void hook_fighter_force(role_id_t role_id, int new_force);
	void hook_fighter_ttl_force(role_id_t role_id, int new_force);

	const Arena_Season_Config* arena_season_config();
	//for test
	void start_arena_battle(void);
	void refresh_1_debug(void);
	void refresh_3_debug(void);
	void refresh_7_debug(void);
	void set_grou_seg(Logic_Player* logic_player, int group , int seg);
	void set_score_debug(Logic_Player* logic_player, int score);

	int init_all_arena_player(bool first = true);
	int load_all_arena_db_info(void);

	Arena_Rank_Player* get_rank_player(role_id_t role_id);
	Arena_Rank_Player* find_rank_player(role_id_t role_id);
	int get_rank_player_size(void);
	void clean_arena_mem(void);

	int get_pay_times_cost(int& cost, int& increase);

	int process_player_data_complete(Logic_Player_Detail* match_data, const role_id_t fight_id, bool push_pool = true);
	//int process_player_match_complete(role_id_t rank_role_id, Logic_Player_Detail* match_data, std::string server_name, int score, int group, int seg, bool push_cache = true);

	int max_free_match_times(void);
	int max_buy_pay_times(void);

	void sort_rank(bool first_tims = false);
	// 下个本服奖励时间
	int32_t get_next_local_award_time(void);

	int32_t get_next_inner_award_time(void);

	Arena_Manager::Arena_Ranking_Vec& arena_ranking_vec(void);

	void set_sync_to_inner_scene_flag(bool value);

	int get_season_remain_day(void);

	Arena_Rank_Player* get_local_first_player(void);

public:
	int req_arena_match(role_id_t role_id, int highest_force, int8_t refresh = 0);
	int req_arena_fight(role_id_t role_id, int index);
	int req_arena_match_refresh(role_id_t role_id);
	int req_arena_role_panel(Logic_Player* player, role_id_t target_id, int8_t index);
	void season_finish(void);
	void season_begin(void);
public:
	int fetch_arena_local_rank_info(role_id_t role_id, uint32_t& self_rank, std::vector<Arena_Rank_Info>& rank_vec);
	int fetch_arena_inner_rank_info(const role_id_t role_id);
	int fetch_arena_info(role_id_t role_id, MSG_50100405& msg);
	int close_arena_panel(role_id_t role_id);

	int match_arena_battle(role_id_t role_id);
	int match_arena_battle(role_id_t role_id, role_id_t match_role_id);

	int battle_result(role_id_t role_id, uint8_t result);

	// 保存王者组名次参数
	int inner_arena_save_king_time(role_id_t role_id, int king_time);
	//
	int inner_arena_fetch_fight_data(int8_t inner_fetch, role_id_t fight_id, role_id_t match_id);

	int inner_arena_match_player(Block_Buffer& buf);

	int inner_arena_player_data_complete(Block_Buffer& buf);

	int inner_arena_sync_inner_rank(std::vector<Id_Int_Int>& inner_rank_vec);

	int arena_save_be_worship(const role_id_t role_id);
	// 活动开启
	void open_activity(const Time_Value& begin_time, const Time_Value& end_time);
	// 活动关闭
	void close_activity(const Time_Value& begin_time, const Time_Value& end_time);
	// 同步所有玩家到跨服
	void sync_all_to_inner_scene(void);
	// 同步单个玩家到跨服
	void sync_one_to_inner_scene(Arena_Rank_Player* rank_player);
	//
	void sync_result_to_inner_scene(Arena_Rank_Player* rank_player, const role_id_t match_id, const int8_t result);

	void clear_season_rank(const role_id_t role_id);

	int get_season_rank(const role_id_t role_id) const;

	void get_season_result(const role_id_t role_id, int &rank, int &group_index, int &level);

	void get_the_first_three(std::vector<Uid_Name>& first_three_vec);


private:
	void save_season_rank(void);
	void season_clear_data(void);
	Time_Value get_season_begin_time(const Time_Value& now);
	Time_Value get_season_finish_time(const Time_Value& now);
	int one_day_refresh(const Time_Value& now);
	int three_day_refresh(const Time_Value& now);
	int three_day_award(Arena_Rank_Player* rank_player, const Time_Value& now);

	void check_battle_start(const Time_Value& now);
	int battle_start(role_id_t fight_id, role_id_t match_id, std::string match_role_name, int8_t difficult_lv);

	Arena_Rank_Player* get_random_player(Arena_Rank_Player* rank_player);

	Arena_Rank_Player* get_rank_player_from_rank(int rank);

	void add_ready_player(Arena_Rank_Player* rank_player, role_id_t match_role_id, std::string match_role_name, std::string match_server_name);

	int fetch_match_player_data(const role_id_t fight_id, const role_id_t match_id);

	void add_match_status(const role_id_t fight_id, const std::vector<Arena_Load_Status> match_player, int8_t refresh = 0);
	void del_match_status(const role_id_t fight_id);
	bool get_match_status(const role_id_t fight_id, std::vector<role_id_t>& match_id_vec);

	void add_inner_load_player(const role_id_t fight_id, const role_id_t match_id);
	void del_inner_load_player(const role_id_t fight_id, const role_id_t match_id);
	bool is_inner_fighter(const role_id_t fight_id, const role_id_t match_id);

	bool get_inner_load_player(const role_id_t load_id, std::vector<role_id_t>& fight_id_vec);

	void load_data_complete(const role_id_t fight_id, const role_id_t loaded_id);
	// 是否本服打的玩家
	bool is_local_fighter(const role_id_t fight_id, const role_id_t match_id, int& match_pos);

	void add_ready_player(const role_id_t fight_id, std::vector<Arena_Load_Status> match_status);

	void check_match_status(const Time_Value &now);

	void get_zero_time(const Time_Value &tv1, Time_Value &tv2);
	// 世界广播
	void make_announce(int announce_id, role_id_t role_id, std::string role_name, std::string text1 = "", std::string text2 = "", std::string text3 = "");
	// 获取本服排名第一玩家
	int get_local_first(role_id_t& role_id, std::string& role_name);

	// 获取某个段位的玩家
	//void get_local_group_rank(int group_index, Arena_Ranking_Vec& rank_vec);

private:
	void match_player(Arena_Rank_Player* fight_player, int highest_force, const int rise, const int group, std::vector<Arena_Match_P> &match_players);
	void match_first_group_rise(Arena_Rank_Player* fight_player,int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_nor_group_rise(Arena_Rank_Player* fight_player,int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_nor_group_rise1(Arena_Rank_Player* fight_player,int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_nor_group_rise2(Arena_Rank_Player* fight_player,int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players);
	void match_low_group_fight(Arena_Rank_Player* fight_player,int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players,
			const role_id_t df_role_id_1 = 0, const role_id_t df_role_id_2 = 0);
	void match_low_fight(Arena_Rank_Player* fight_player,int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players,
			const role_id_t df_role_id_1 = 0, const role_id_t df_role_id_2 = 0);

	bool match_difficult_hard(Arena_Rank_Player* fight_player,int highest_force, const role_id_t god_role_id, role_id_t& rt_match_id);
	bool match_difficult_god(Arena_Rank_Player* fight_player,int highest_force, role_id_t& rt_match_id);
	int process_match_complete(role_id_t fight_id, std::vector<Arena_Load_Status> match_status, int8_t refresh, bool push_cache = true);

	bool is_exploit_id(int value);
	bool is_exp_id(int value);
	bool is_resource_id(int value);
	bool is_on_top_seg_key(const int group, const int seg);
private:
	int send_to_inner_arena(Block_Buffer& buf);
private:
	Arena_Rank_Player_Map rank_player_map_;
	Arena_Ranked_Map ranked_map_;
	//Arena_Ranking_Set ranking_set_;
	Arena_Ranking_Vec ranking_vec_;
	Role_Id_Set ranking_set_;
	Ready_Player_Set ready_player_set_;

	//Group_Rank_Map group_rank_map_;
	//Arena_Ranking_Vec fight_rank_vec_;

	Load_Data_Player_Map inner_load_player_map_;
private:
	Time_Value refresh_time_;
	bool rank_change_;
	Time_Value local_award_time_;				// 本服奖励时间
	Time_Value inner_award_time_;				// 跨服奖励时间
private:
	bool sync_to_inner_scene_;
private:
	Arena_Match_Status_Map arena_match_status_map_;
	Arena_Match_Status_Map arena_ready_status_map_;
	Arena_Season_Detail arena_season_detail_;

private:
	int get_arena_seg_key(int group, int seg);
	void arena_seg_add(role_id_t role_id, int seg_key, int force);
	void arena_seg_del(role_id_t role_id, int seg_key, int force);
	void arena_seg_change(role_id_t role_id, int old_seg_key, int new_seg_key, int old_force, int new_force);
private:
	Arena_Seg_Map arena_seg_map_;
	int64_t logic_uuid_;
};


typedef Singleton<Arena_Manager> Arena_Manager_Single;
#define ARENA_MANAGER (Arena_Manager_Single::instance())


#endif /* ARENA_MANAGER_H_ */
