/*
 * Inner_Arena_Struct.cpp
 *
 *  Created on: 2014年9月11日
 *      Author: root
 */

#include "Inner_Arena_Struct.h"

Inner_Arena_Rank_Player::Inner_Arena_Rank_Player():arena_rise_fail(0),
	arena_streak_lose(0),
	uuid(0) {

}

void Inner_Arena_Rank_Player::reset(void){
	rank_info.reset();
	arena_rise_fail = 0;
	arena_streak_lose = 0;
}
const Player_Rank_Info& Inner_Arena_Rank_Player::player_rank_info_const(void) const{
	return rank_info;
}

bool Inner_Rank_Greater(Inner_Arena_Rank_Player* player1, Inner_Arena_Rank_Player* player2){
	return player1->player_rank_info_const() > player2->player_rank_info_const();
}

bool Inner_Fight_Greater(Inner_Arena_Rank_Player* player1, Inner_Arena_Rank_Player* player2){
	if(player1->fight() > player2->fight()){
		return true;
	}else if(player1->fight() < player2->fight()){
		return false;
	}else{
		if(player1->group() > player2->group()){
			return true;
		}else if(player1->group() < player2->group()){
			return false;
		}else{
			if(player1->seg() > player2->seg()){
				return true;
			}else if(player1->seg() < player2->seg()){
				return false;
			}else{
				return false;
			}
		}
	}
	return false;
}
