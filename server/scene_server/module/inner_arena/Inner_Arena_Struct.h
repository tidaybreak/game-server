/*
 * Inner_Arena_Struct.h
 *
 *  Created on: 2014年9月11日
 *      Author: root
 */

#ifndef INNER_ARENA_STRUCT_H_
#define INNER_ARENA_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"
#include "arena/Arena_Struct.h"

struct Inner_Arena_Rank_Player{
	Inner_Arena_Rank_Player();

	void reset();
	const Player_Rank_Info& player_rank_info_const(void) const;
	role_id_t role_id(void){
		return rank_info.role_id;
	}
	std::string role_name(void){
		return rank_info.role_name;
	}
	std::string server_name(void){
		return rank_info.server_name;
	}
	int local_rank(void){
		return rank_info.rank;
	}
	int inner_rank(void){
		return rank_info.inner_rank;
	}
	int fight(void){
		return rank_info.fight;
	}
	int group(void){
		return rank_info.group;
	}
	int seg(void){
		return rank_info.seg;
	}
	int level(void){
		return rank_info.level;
	}
	int score(void){
		return rank_info.score;
	}
	int career(void){
		return rank_info.career;
	}
	int gender(void){
		return rank_info.gender;
	}
	uint32_t head_id(void){
		return rank_info.head_id;
	}

	int rise_fail(void){
		return arena_rise_fail;
	}
	int streak_lose(void){
		return arena_streak_lose;
	}
	int king_time(void){
		return rank_info.king_time;
	}

	void set_role_id(role_id_t role_id){
		rank_info.role_id = role_id;
	}
	void set_local_rank(int value){
		rank_info.rank = value;
	}
	void set_inner_rank(int value){
			rank_info.inner_rank = value;
	}
	void set_king_time(int value){
		rank_info.king_time = value;
	}
	void season_reset(void){
		arena_rise_fail = 0;
		arena_streak_lose = 0;
		rank_info.season_reset();
	}
	Player_Rank_Info rank_info;
	int arena_rise_fail;
	int arena_streak_lose;
	int64_t uuid;
};


extern bool Inner_Rank_Greater(Inner_Arena_Rank_Player* player1, Inner_Arena_Rank_Player* player2);

extern bool Inner_Fight_Greater(Inner_Arena_Rank_Player* player1, Inner_Arena_Rank_Player* player2);

#endif /* INNER_ARENA_STRUCT_H_ */
