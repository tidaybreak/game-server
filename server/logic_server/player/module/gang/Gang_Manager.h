/*
 * Gang_Manager.h
 *
 *  Created on: Apr 24, 2014
 *      Author: caijianwei
 */

#ifndef GANG_MANAGER_H_
#define GANG_MANAGER_H_

#include "Server_Struct.h"
#include "Gang_Struct.h"

class Logic_Player;
class Player_DB_View;
struct Gang_Passed_Event;
struct Gang_Info;
struct Gang_Row_Info;
struct Ganger_Row_Info;
struct Gang_Apply_Info;
struct Gang_DB_View;
struct Ganger_Detail;
struct MSG_81070005;

struct Gang_War_Matched_Gangs_Pair;
typedef std::vector<Gang_War_Matched_Gangs_Pair> GangWarMatchedGangsPair_Vec;
typedef std::deque<Gang_War_Matched_Gangs_Pair> GangWarMatchedGangsPair_Deque;
struct Gang_War_Matched_Gang_Info;
typedef std::map<int64_t, Gang_War_Matched_Gang_Info> GangWarMatchedGangInfo_Map;
struct Gang_War_Rank_Info;
struct Gang_War_Spot;
struct Gang_War_Info;

class Gang_Manager {
public:
	typedef boost::unordered_map<Server_Key, int64_t> Server_Max_Gang_Map;
	typedef boost::unordered_map<int64_t, Gang_DB_View*> Gang_ID_Map;
	typedef boost::unordered_map<std::string, Gang_DB_View*> Gang_Name_Map;
	typedef boost::unordered_map<int64_t, Ganger_Detail*>	Gang_ID_Detail_Map;
	typedef boost::unordered_map<role_id_t, gang_id_t> Gang_Boss_Join_flag_Map;
	const static int GANG_WAR_SCENE_ID = 80213001;

	Gang_Manager();
	~Gang_Manager();

	void start();
	void tick(Time_Value& now);
	void daily_refresh(void);
	void save(void);

	void add_gang_db_view(Gang_DB_View* db_view);
	bool validate_new_name(const std::string& name);
	int64_t create_gang(const Server_Key& server_info, role_id_t leader, int8_t logo, const std::string& name);
	Gang_DB_View* gang_view_by_id(int64_t gang_id);

	int  res_gang_info(int64_t gang_id, Logic_Player* client);
	int  res_gang_info_update(int64_t gang_id, Logic_Player* client);
	void res_gang_list(Logic_Player* client);
	int  res_ganger_list(Logic_Player* client, int64_t gang_id);

	int  join_gang(Logic_Player* client, int64_t gang_id);
	int  quit_gang(Logic_Player* client, int64_t gang_id);
	int  resign_to(Logic_Player* client, int64_t gang_id, role_id_t new_leader);

	int  set_gang_annouce(int64_t gang_id, const std::string& announce);
	void add_gang_info_listener(role_id_t role_id);
	void remove_gang_info_listener(role_id_t role_id);

	bool invite_friend_cd_ok(role_id_t role_id);
	void update_invite_friend_cd(role_id_t role_id);
	void add_direct_in_gang(int64_t gang_id, role_id_t role_id);
	void remove_direct_in_gang(int64_t gang_id, role_id_t role_id);
	bool can_direct_in_gang(int64_t gang_id, role_id_t role_id);
	void remove_offline_ganger_detail(int64_t role_id);
	void cache_offline_ganger_detail(const Ganger_Detail& detail);

	int res_gang_list_action(Logic_Player* player, int64_t gang_id, int8_t action);
	int join_gang_direct(Logic_Player* player, Gang_DB_View* db_view);
	int res_gang_apply_list(Logic_Player* player, int64_t gang_id);
	int res_gang_apply_list_action(Logic_Player* player, int64_t gang_id, int8_t action, role_id_t target_id);
	int req_gang_change_headship_auth(Gang_DB_View* gang_view, role_id_t role_id, int8_t headship, int auth);
	int req_gang_upgrade(role_id_t role_id, Gang_DB_View* gang_view);
	int gang_lvl_clear_cd(Gang_DB_View* gang_view);
	int gang_building_clear_cd(Gang_DB_View* gang_view, int building_id);
	int req_gang_war_board(Logic_Player &player);
	int req_gang_war_gang_rank(Logic_Player &player);
	int req_gang_war_spot(Logic_Player &player, const int8_t index);
	int active_gang_war_board_notice(const int8_t type, const int64_t role_id = 0, const int64_t gang_id = 0);
	int req_gang_war_rank_award_list(Logic_Player &player);
	int req_gang_war_get_rank_award(Logic_Player &player);

	void notify_gang_info_update(Gang_DB_View const& gang_view);
	void notify_gang_info_update(int64_t gang_id);
	void notify_gang_row_info(int8_t oper, Gang_DB_View const& gang_view);
	void notify_gang_row_info(int8_t oper, int64_t gang_id);
	void notify_ganger_row_info(int8_t oper, role_id_t role_id, role_id_t gang_id, bool sign_out = false);
	void active_gang_notify(int64_t id_1, uint8_t action, int64_t id_2, std::string name, int32_t value = 0);

	void refresh_rank(void);
	int update_member_info(Gang_DB_View* gang_view, role_id_t role_id);
	int send_group_mail(Gang_DB_View* gang_view, bool send_to_leader = false);
	int send_daily_fund_mail(Gang_DB_View* gang_view, bool send_to_leader = false);
	int refresh_force(Gang_DB_View* gang_view);

	std::string gang_name(int64_t gang_id);
	int erase_all_gang_apply(Logic_Player* player);
	bool has_member(int64_t gang_id, role_id_t role_id);
	int get_gang_level(int64_t gang_id);
	int change_gang_fund(int64_t gang_id, int change_value);

	inline const Gang_ID_Map &gang_map(void);
	inline Gang_ID_Map &gang_map_no_const(void);

	// ========gang_war start
	int gang_war_start_sync(const int stage, const bool not_create_matched_info = false);
	void create_mateched_16(void); // 产生名单
	void ready_activity(void);
	void open_activity(void); // 开始计时
	void close_activity(void);

	int gang_war_load_matched_info(Block_Buffer &buf);
	int gang_war_load_spot_rank_info(Block_Buffer &buf);
	void gang_war_load_result_info(Block_Buffer &buf);
	void gang_war_load_award(Block_Buffer &buf);

	const Gang_War_Matched_Gang_Info *gang_war_find_matched_gang_info(const int64_t gang_id);
	const Gang_War_Matched_Gangs_Pair *gang_war_find_matched_gangs_pair(const int64_t gang_id);
	const Gang_War_Spot *gang_war_find_result_map(const int8_t matched_index);
	const Gang_War_Spot *gang_war_find_result_map(const int64_t gang_id);

	// btn state
	int gang_war_set_matched_gang_info_btn_state(const int64_t gang_id, const int8_t btn_state);
	// btn state

	void gang_war_send_mail_for_start(void);
	void gang_war_ready_mail(void);

	int gang_war_help(Block_Buffer &buf);

	bool gang_war_is_open(void);

	int gang_war_sync_switch(void);
	void gang_war_icon_act(void);
	Gang_War_Info &gang_war_info(void);
//	int64_t gang_war_get_next_index(const int64_t gang_id);

	// test command
	void gang_war_clear_award_list(void);
	void gang_war_produce_rank_award_when_all_close(void);
	void gang_war_rank_award_auto_get(void);
	// ========gang_war end

	bool in_active_annon_ed(int64_t gang_id, uint32_t level);//获取公会指定等级有没有发过活动公告 在返回true
	void set_active_annon_ed(int64_t gang_id, uint32_t level);//设置公会发过活动公告
	void clean_ganger_detail(void);
public:
	// 获取公会boss图标状态
	void get_gang_boss_act_state(const role_id_t gang_id, int& state, int& remain_time);
	// 获取公会boss击杀信息
	int get_boss_kill_info(const gang_id_t gang_id, int& kill_num, int& boss_lv, int& hour, int& minute);
	// 设置公会boss开启时间
	int set_boss_open_time(const gang_id_t gang_id, const int8_t hour, const int8_t minute, int8_t& r_hour, int8_t& r_minute);
	int cmd_set_boss_lv(const gang_id_t gang_id, const int lv);
	int cmd_set_boss_kill_num(const gang_id_t gang_id, const int kill_num);
	// 设置公会boss结果
	int set_boss_act_result(const gang_id_t gang_id, const int result);
	// 设置进入公会boss标志
	void set_gang_boss_join_flag(const role_id_t role_id, const gang_id_t gang_id);
	// 是否可以进入公会boss
	bool can_join_gang_boss(const role_id_t role_id, const gang_id_t gang_id);
	// 是否在公会
	bool is_in_gang(const role_id_t role_id, const gang_id_t gang_id);
private:
	Gang_Manager(const Gang_Manager&);
	Gang_Manager& operator=(const Gang_Manager&);
	void reset();

	Time_Value next_save_time(Time_Value const& now);
	int64_t make_id(const Server_Key& server_info);
	Player_DB_View* player_view_by_role_id(role_id_t role_id);
	const Ganger_Detail& offline_ganger_detail(role_id_t role_id);
	void dismiss_gang(Gang_DB_View& gang_view, bool kick_ganger = false);
	Gang_ID_Map::iterator dismiss_gang(Gang_ID_Map::iterator &it_gang, bool kick_ganger = false);

	void daily_fund(void);
	// 每日公会boss刷新
	void daily_boss(void);
	void daily_leader(void);

	void gang_init(void);
	void building_init(Gang_DB_View* gang_view);

	void make_gang_info(Gang_DB_View const& gang_view, Gang_Info& info);
	void make_gang_row_info(Gang_DB_View const& gang_view, Gang_Row_Info& info);
	void make_ganger_row_info(role_id_t role_id, role_id_t gang_id, Ganger_Row_Info& info, bool sign_out = false);
	void make_gang_apply_info(role_id_t role_id, Gang_Apply_Info& info);
	void make_gang_notify_info(MSG_81070005 &msg, uint8_t action, int64_t id, std::string name, int32_t value);

	// ========gang_war start
	// sync_data
	int gang_war_sync_data_to_scene(const bool not_create_matched_info = false);
	void gang_war_sync_gang_data_by_rank_limit_16(Block_Buffer &buf);
	void gang_war_sync_matched_info(Block_Buffer &buf);
	// sync_data

	void gang_war_clear_last_match_info(void);
	void gang_war_produce_matched_info(void);

	// tick
	void gang_war_tick(const Time_Value& now);
	void gang_war_tick_normal(const Time_Value &now);
	void gang_war_tick_first_week(const Time_Value &now, const int week_open);
	int gang_war_tick_get_first_open_time(const int week_open);
	int gang_war_tick_get_today_state(const int start_day);
	// tick

	//
	void gang_boss_tick(const Time_Value& now);
	// 公会boss图标状态通知
	void gang_boss_act_notice(const Gang_DB_View* gang_view, const int state, const int remain_time);

	// single_spot
	void gang_war_handle_single_spot_when_open(void);
	void gang_war_award_single_spot(const Gang_War_Spot &spot);
	void gang_war_handle_no_battle_spot(void);
//	void gang_war_award_no_battle_spot(const Gang_War_Spot &spot, const int8_t stage = -1);
	// single_spot

	// 排名奖励
	void gang_war_clear_assigned_award_list(void);
	// ========gang_war end
private:
	const Time_Value SAVE_INTER;
	Time_Value save_time;
	Server_Max_Gang_Map server_max_gang_map;	// <Server_Key, int64_t> 某服,某服最大公会ID
	Gang_ID_Map gang_id_map;	// <int64_t, Gang_DB_View*> 正常帮派ID,帮派信息
	Gang_ID_Map dismiss_gang_id_map;	// <int64_t, Gang_DB_View*> 已解散帮派ID,帮派信息
	Gang_Name_Map gang_name_map;	// <std::string, Gang_DB_View*> 帮派name,帮派信息
	Role_Hash_Set row_update_listener;
	Role_Id_Time_Map ganger_invite_cd;	// <role_id_t, Time_Value> 帮派成员,邀请解禁时间
	Gang_ID_Detail_Map offline_ganger_detail_cache;	// <int64_t, Ganger_Detail*> 离线玩家ID,离线玩家帮派信息

	// ========gang_war start
	Gang_War_Info gang_war_info_;
	// ========gang_war end

	Gang_Boss_Join_flag_Map gang_boss_join_flag_map_;
};

typedef Singleton<Gang_Manager> Gang_Manager_Single;
#define GANG_MANAGER Gang_Manager_Single::instance()

///////////////////////////////////////////////////////////////////////////////////////////

inline const Gang_Manager::Gang_ID_Map &Gang_Manager::gang_map(void) {
	return gang_id_map;
}

inline Gang_Manager::Gang_ID_Map &Gang_Manager::gang_map_no_const(void) {
	return gang_id_map;
}

#endif /* GANG_MANAGER_H_ */
