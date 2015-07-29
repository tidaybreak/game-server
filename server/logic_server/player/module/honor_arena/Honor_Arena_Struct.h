/*
 * Honor_Arena_Struct.h
 *
 *  Created on: 2015年3月16日
 *      Author: lys
 */

#ifndef HONOR_ARENA_STRUCT_H_
#define HONOR_ARENA_STRUCT_H_

#include "Game_Typedef.h"
#include "Msg_Struct.h"

enum Honor_Arena_Show_Type{
	HONOR_ARENA_FIRST_WIN = 1000,
	HONOR_ARENA_TEN_FIGHT = 1001,
};

struct Honor_Arena_Player_Info{
	Honor_Arena_Player_Info(void){
		reset();
	}
	~Honor_Arena_Player_Info(void){

	}
	void reset(void){
		state = 0;
		auto_match = 0;
		match_lv = 0;
		first_win = 0;
		ten_fight = 0;
		streak_win_state.clear();
		streak_win = 0;
		streak_lose = 0;
		max_streak_win = 0;
		max_streak_lose = 0;
		fight_num = 0;
		win_num = 0;
		match_time = 0;
		gain_items.clear();
		rank_change = false;

		role_info.reset();
		rank_info.reset();
	}
	void add_reward_to_gain_items(const int id, const int amount){
		bool is_find = false;
		Int_Int_Vec::iterator it;
		for(it = gain_items.begin(); it != gain_items.end(); ++it){
			if(it->val_1 == id){
				it->val_2 += amount;
				is_find = true;
				break;
			}
		}
		if(!is_find){
			Int_Int value;
			value.val_1 = id;
			value.val_2 = amount;
			gain_items.push_back(value);
		}
	}
	void add_score(const int value){
		rank_info.score += value;
	}
	void fight_win(void);
	void fight_lose(void);

	int8_t state;										// -1 out scene 0 idle 1 matching 2 wait fight 3 fight
	int8_t auto_match;								// 自动匹配
	uint8_t match_lv;									// 匹配级别 每等待10秒增加1
	int8_t first_win;									// 首胜 0 未达成 1可领取 2已领取
	int8_t ten_fight;									// 十战 0 未达成 1可领取 2已领取
	Int_Int_Vec streak_win_state;					// 连胜状态 0 未达成 1可领取 2已领取
	int8_t streak_win;
	int8_t streak_lose;
	int8_t max_streak_win;							// 最大连胜次数
	int8_t max_streak_lose;							// 最大连败次数
	int fight_num;										// 战斗次数
	int win_num;										// 胜利次数
	int match_time;									// 开启匹配时间戳
	Int_Int_Vec gain_items;							// 今日获得物品
	bool rank_change;

	Role_Info_With_Avatar role_info;
	Honor_Arena_Rank_Info rank_info;
	std::vector<Honor_Arena_Player_Info*>::iterator rank_iter;

};
typedef boost::unordered_map<role_id_t, Honor_Arena_Player_Info> Honor_Arena_Player_Info_Map;

struct Honor_Arena_Match_Team{
	Honor_Arena_Match_Team(void){
		reset();
	}
	~Honor_Arena_Match_Team(void){

	}
	void reset(void){
		fight_time = 0;
		role_id_1 = 0;
		role_id_2 = 0;
	}

	int fight_time;
	role_id_t role_id_1;
	role_id_t role_id_2;
};

extern bool Honor_Arena_Rank_Greater(Honor_Arena_Player_Info* player1, Honor_Arena_Player_Info* player2);

extern bool Honor_Arena_Match_Greater(Honor_Arena_Player_Info* player1, Honor_Arena_Player_Info* player2);

#endif /* HONOR_ARENA_STRUCT_H_ */
