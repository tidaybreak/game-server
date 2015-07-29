/*
 * Arena_Rank_Player.h
 *
 *  Created on: 2014年5月19日
 *      Author: root
 */

#ifndef ARENA_RANK_PLAYER_H_
#define ARENA_RANK_PLAYER_H_

#include "Arena_Struct.h"

class Logic_Player;
struct Arena_Battle_Report;

class Arena_Rank_Player{
public:
	Arena_Rank_Player(void);
	virtual ~Arena_Rank_Player(void);

	void reset(void);

	void sign_in(Logic_Player* player);
	void sign_out(Logic_Player* player);

	int load_detail(void);
	int save_detail(void);

	int load_fighter_detail(Block_Buffer& buf);
	int save_fighter_detail(Block_Buffer& buf);

	Player_Rank_Info* player_rank_info(void);
	const Player_Rank_Info& player_rank_info_const(void) const;
	Logic_Player* logic_player(void);

public:
	//进入黄金组全服公共
	void check_rise_announce(void);
	inline bool need_notice_client(void);
	inline void set_need_notice_client(bool value);

	role_id_t role_id(void);
	std::string role_name(void);
	int local_rank(void);
	int inner_rank(void);
	int fight(void);
	int group(void);
	int seg(void);
	int level(void);
	int score(void);
	int career(void);
	int gender(void);
	uint32_t head_id(void);
	int rise_fail(void);
	int be_worship(void);

	void set_role_id(role_id_t value);
	void set_role_name(const std::string& value);
	void set_server_name(const std::string& value);
	void set_local_rank(int value);
	void set_inner_rank(int value, bool notice = false);
	void set_king_time(int value);
	void set_fight(int value);
	void set_group_seg(const int value1, const int value2);
	void set_level(int value);
	void set_score(int value);
	void set_career(int value);
	void set_gender(int value);
	void set_head_id(uint32_t value);
	void set_fight_times(int value);
	void set_be_worship(int value);

	void set_battle_start_time(const Time_Value& time, role_id_t match_role_id, std::string match_role_name, std::string match_server_name);
	bool check_battle_start_time(const Time_Value& now);
	role_id_t get_match_id(void);
	void set_match_id(role_id_t value);
	void set_match_role_name(std::string role_name);
	std::string match_role_name(void){
		return match_role_name_;
	}
	std::string match_server_name(void){
		return match_server_name_;
	}
	void set_match_difficult_lv(const int8_t value){
		match_difficult_lv_ = value;
	}
	int8_t get_match_difficult_lv(void){
		return match_difficult_lv_;
	}
public:
	Player_Rank_Info rank_info_;
private:
	bool need_notice_client_;
	Time_Value battle_start_time_;
	role_id_t match_role_id_;
	std::string match_role_name_;
	std::string match_server_name_;
	int8_t match_difficult_lv_;
};

bool Arena_Rank_Player::need_notice_client(void){
	return need_notice_client_;
}

void Arena_Rank_Player::set_need_notice_client(bool value){
	need_notice_client_ = value;
}

////////////////////////////////////////////////////////

struct Arena_Rank_Player_Greater {
	bool operator()(const Arena_Rank_Player *player1, const Arena_Rank_Player *player2) const;
};

extern bool Rank_Greater(Arena_Rank_Player* player1, Arena_Rank_Player* player2);

extern bool Fight_Greater(Arena_Rank_Player* player1, Arena_Rank_Player* player2);
#endif /* ARENA_RANK_PLAYER_H_ */
