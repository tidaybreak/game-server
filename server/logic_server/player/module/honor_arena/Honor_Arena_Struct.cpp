/*
 * Honor_Arena_Struct.cpp
 *
 *  Created on: 2015年3月16日
 *      Author: lys
 */

#include "Honor_Arena_Struct.h"


bool Honor_Arena_Rank_Greater(Honor_Arena_Player_Info* player1, Honor_Arena_Player_Info* player2){
	if(player1->rank_info.score > player2->rank_info.score){
		return true;
	}else if(player1->rank_info.score < player2->rank_info.score){
		return false;
	}else{
		if(player1->rank_info.force > player2->rank_info.force){
			return true;
		}else if(player1->rank_info.force < player2->rank_info.force){
			return false;
		}
	}
	return false;
}

bool Honor_Arena_Match_Greater(Honor_Arena_Player_Info* player1, Honor_Arena_Player_Info* player2){
	return player1->match_time < player2->match_time;
}


/////////////////////////////////////////////////////////////////////////////////////


void Honor_Arena_Player_Info::fight_win(void){
	fight_num += 1;
	win_num += 1;
	// 首胜
	if(win_num == 1){
		first_win = 1;
	}
	// 十战
	if(fight_num == 10){
		ten_fight = 1;
	}
	// 连胜
	streak_win += 1;
	if(streak_win > max_streak_win){
		max_streak_win += 1;
		Int_Int_Vec::iterator it;
		for(it = streak_win_state.begin(); it != streak_win_state.end(); ++it){
			if(it->val_1 == max_streak_win){
				it->val_2 = 1;
				break;
			}
		}
	}
	streak_lose = 0;
}

void Honor_Arena_Player_Info::fight_lose(void){
	fight_num += 1;

	// 十战
	if(fight_num == 10){
		ten_fight = 1;
	}

	// 连败
	streak_lose += 1;
	if(streak_lose > max_streak_lose){
		max_streak_lose = streak_lose;
	}
	//
	streak_win = 0;
}

