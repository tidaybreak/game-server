/*
 * Logic_Arenaer.h
 *
 *  Created on: 2014年5月19日
 *      Author: root
 */

#ifndef LOGIC_ARENAER_H_
#define LOGIC_ARENAER_H_

#include "Logic_Player_Base.h"
#include "Arena_Struct.h"

class Arenaer_Detail;
class Logic_Player_Detail;

class Logic_Arenaer: virtual public Logic_Player_Base{
public:
	Logic_Arenaer(void);
	virtual ~Logic_Arenaer(void);
public:
	void reset(void);

	void load_detail(Arenaer_Detail &detail);
	void save_detail(Arenaer_Detail &detail);

	inline Arenaer_Detail* arenaer_detail(void);

	void daily_refresh(bool sync_client = false);
	// 凌晨0点刷新
	int trigger_daily_zero(const Time_Value &now, bool logic_check = false);
	// 凌晨6点刷新
	int trigger_daily_six(const Time_Value &now, bool logic_check = false);

	void module_init(void);
	static int create_init(Arenaer_Detail& detail);
	void sign_in(void);
	void sign_out(void);

	// 离线奖励
	void update_offline_reward(void);
	// 等级改变监听
	void hook_fighter_level(const int new_level, const int src_level);
	// 等级
	void hook_fighter_force(int new_force);

	void hook_fighter_ttl_force(int new_force);

	void arena_listen_vip_level_change(uint32_t new_vip_level, uint32_t old_vip_level);

	void arena_season_clear_data(void);

public:
	int req_arena_match(void);
	int req_arena_fight(int index);
	int req_arena_match_refresh(void);
	int req_arena_role_panel(role_id_t role_id, int8_t index);
	int req_arena_panel_refresh(void);
	int req_arena_worship(const role_id_t role_id,const int8_t type);
public:
	int fetch_arena_rank_info(int type);
	int fetch_arena_info(bool panel_open = false);
	int close_arena_panel(void);
	int req_match_arena_battle(void);
	int req_buy_arena_pay_times(const uint8_t& buy_type);
	int req_reset_arena_cd(void);

	int arena_battle_result(uint8_t result);

	int receive_reward(int8_t type);

	int req_fetch_arena_award_info(void);
	// 是否能进入竞技场
	bool can_join_arena(void);
	// 竞技场进入次数信息
	void get_arena_enter_num_info(int& total_num, int& enter_num);
	// 获取竞技场赛季剩余次数
	int get_arena_season_remain_day(void);

	int get_arena_free_num(void);
	// 获取竞技场段位
	void get_arena_level(int& group, int& seg);
	// 设置竞技场cd
	void set_arena_cd(const int cd);
	// 重置竞技场cd
	void reset_arena_cd(void);
	// 获取竞技场cd
	int get_arena_cd(void);

	int notice_inner_rank(void);

	int notice_local_rank(void);

	void set_arena_match_data(Logic_Player_Detail* match_data, int pos);

	int worship_cost_get(const role_id_t role_id ,const std::vector<Int_Int> &cost,const std::vector<Int_Int> &get);

	Arena_Match_Player_Data* get_arena_match_player_data(const role_id_t match_id);
public:
	int group(void);
	int seg(void);
	int score(void);
	int local_rank(void);
	int inner_rank(void);
	int rise_fail(void);
	int rise_win(void);
	int rise_lose(void);
	int enter_num(void);
	int pay_times(void);
	int buy_pay_times(void);
	int streak_win(void);
	int streak_lose(void);
	int compensate_times(void);
	int three_day_rank(void);
	int seven_day_rank(void);
	Time_Value arena_cd(void);
	bool match_player_info(role_id_t &match_player1, role_id_t &match_player2, role_id_t &match_player3,
			int8_t &match_dlv_1, int8_t &match_dlv_2, int8_t &match_dlv_3);
	void get_arena_match_remain_time(bool& match, int& remain_time);
	int be_worship(void);

	void set_fight_times(int value);
	void set_group_seg(const int value1, const int value2);
	void set_score(int value);
	void set_local_rank(int value);
	void set_inner_rank(int value);
	void set_king_time(int value);
	void set_rise_fail(int value);
	void set_rise_win(int value);
	void set_rise_lose(int value);
	void set_enter_num(int value);
	void set_pay_times(int value);
	void set_buy_pay_times(int value);
	void set_streak_win(int value);
	void set_streak_lose(int value);
	void set_compensate_times(int value);
	void set_three_day_rank(int value);
	void set_seven_day_rank(int value);
	void set_match_player_info(Time_Value time, const role_id_t player1, const role_id_t player2, const role_id_t player3,
			const int dlv_1, const int dlv_2, const int dlv_3);
	void set_be_worship(int value);
	int add_buy_pay_times(const int& value);
	void add_battle_report(Arena_Battle_Report& report);
	void add_battle_report_vec(std::vector<Arena_Battle_Report>& report_vec);
	void add_score(int value);

	void notice_arena_info(void);

	void notice_award_info(void);

	int is_arena_rise(void);

	int email_three_day_reward(int rank, const Time_Value& now);

	void clear_season_rank(void);
	int get_season_rank(void);
	void get_season_result(int &rank, int &group_index, int& level);
protected:
	Arenaer_Detail* arenaer_detail_;
	bool notice_client_;
public:
	Arena_Match_Player_Data arena_player_data[3];
};

Arenaer_Detail* Logic_Arenaer::arenaer_detail(void){
	return arenaer_detail_;
}

#endif /* LOGIC_ARENAER_H_ */
