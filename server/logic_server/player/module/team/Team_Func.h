/*
 * Team_Func.h
 *
 *  Created on: Sep 28, 2012
 *      Author: "dingxinglong"
 */

#ifndef TEAM_FUNC_H_
#define TEAM_FUNC_H_

#include "Game_Typedef.h"
#include "Server_Struct.h"
#include <stdint.h>
#include <vector>

#include "Err_Code.h"
#include "NPC/NPC_Struct.h"

class Logic_Player;
struct Role_Team_Info;
struct Online_Teamer_Info;
class Team;
class Team_Basic_Info;

inline int get_empty_pos(const Int_Hash_Set &idx_set, int &idx);

inline int get_team_enter_error_type(int error_code) {
	switch (error_code) {
//	case ERROR_CLIENT_OPERATE:
//	case ERROR_CONFIG_NOT_EXIST:
//		return 6;
	default:
		return 0;
	}
}

//void cpy_arena_teammer_info(const Inner_Arena_Teammer &teammer_detail, Arena_Teammer &teammer, int server = 0);
//void cpy_arena_team_info(const Arena_Team_Common_Detail &team_detail, Arena_Team_Info &info, int server = 0);
//void get_arena_teamer_detail(const Logic_Player &player, Inner_Arena_Teammer &detail);
//void get_arena_teamer_info(const Logic_Player &player, Arena_Teammer &info);
//void get_online_teamer_info(const Logic_Player &player, Online_Teamer_Info &teamer_info);
//void get_player_basic_info(const Logic_Player &player, Player_Basic_Info &basic_info);
void get_team_info(const Team &team, Team_Basic_Info &info);
//void get_teamer_pos_info(const Map_Player &player, Teamer_Pos_Info &pos_info);
int get_online_teamer_info_from_logic(Logic_Player &player, Online_Teamer_Info &online_teamer_info);
int get_team_role_info_from_logic(Logic_Player *player, Role_Team_Info &role_info);

inline void merge_int_vec(const std::vector<int64_t> &vec_from, std::vector<int64_t> &vec_to) {
	for (std::vector<int64_t>::const_iterator it = vec_from.begin(); it != vec_from.end(); ++it) {
		vec_to.push_back(*it);
	}
}
inline int get_other_idx(const int idx) {
	switch (idx) {
	case 4:
		return 7;
	case 5:
		return 8;
	case 6:
		return 9;
	case 7:
		return 4;
	case 8:
		return 5;
	case 9:
		return 6;
	default:
		break;
	}
	return 0;
}

inline int ensure_battle_pos_unique(Int_Hash_Set &idx_set) {
	if (0 != idx_set.count(Position_Key::LINE_ONE_FRONT)
			 || 0 != idx_set.count(Position_Key::LINE_ONE_BEHIND)
			 || 0 != idx_set.count(Position_Key::LINE_ONE_FIRST)) {
		idx_set.erase(Position_Key::LINE_ONE_FRONT);
		idx_set.erase(Position_Key::LINE_ONE_BEHIND);
		idx_set.erase(Position_Key::LINE_ONE_FIRST);
		idx_set.insert(Position_Key::LINE_ONE_FRONT);
	}
	if (0 != idx_set.count(Position_Key::LINE_TWO_FRONT)
			 || 0 != idx_set.count(Position_Key::LINE_TWO_BEHIND)
			 || 0 != idx_set.count(Position_Key::LINE_TWO_FIRST)) {
		idx_set.erase(Position_Key::LINE_TWO_FRONT);
		idx_set.erase(Position_Key::LINE_TWO_BEHIND);
		idx_set.erase(Position_Key::LINE_TWO_FIRST);
		idx_set.insert(Position_Key::LINE_TWO_FRONT);
	}
	if (0 != idx_set.count(Position_Key::LINE_THREE_FRONT)
			 || 0 != idx_set.count(Position_Key::LINE_THREE_BEHIND)
			 || 0 != idx_set.count(Position_Key::LINE_THREE_FIRST)) {
		idx_set.erase(Position_Key::LINE_THREE_FRONT);
		idx_set.erase(Position_Key::LINE_THREE_BEHIND);
		idx_set.erase(Position_Key::LINE_THREE_FIRST);
		idx_set.insert(Position_Key::LINE_THREE_FRONT);
	}
	if (idx_set.size() >= 3){
		return ERROR_FIGHT_NOT_EMPTY;
	}
	return 0;
}

inline int get_empty_pos_check_idx(const Int_Hash_Set &idx_set, int &idx) {
	bool need_reset = false;
	switch(idx) {
	case Position_Key::LINE_ONE_FIRST:
	case Position_Key::LINE_ONE_FRONT:
	case Position_Key::LINE_ONE_BEHIND:
		if (0 != idx_set.count(Position_Key::LINE_ONE_FRONT)
				 || 0 != idx_set.count(Position_Key::LINE_ONE_BEHIND)
				 || 0 != idx_set.count(Position_Key::LINE_ONE_FIRST)) {
			need_reset = true;
		}
		break;
	case Position_Key::LINE_TWO_FIRST:
	case Position_Key::LINE_TWO_FRONT:
	case Position_Key::LINE_TWO_BEHIND:
		if (0 != idx_set.count(Position_Key::LINE_TWO_FRONT)
				|| 0 != idx_set.count(Position_Key::LINE_TWO_BEHIND)
				|| 0 != idx_set.count(Position_Key::LINE_TWO_FIRST)) {
			need_reset = true;
		}
		break;
	case Position_Key::LINE_THREE_FIRST:
	case Position_Key::LINE_THREE_FRONT:
	case Position_Key::LINE_THREE_BEHIND:
		if (0 != idx_set.count(Position_Key::LINE_THREE_FRONT)
				|| 0 != idx_set.count(Position_Key::LINE_THREE_BEHIND)
				|| 0 != idx_set.count(Position_Key::LINE_THREE_FIRST)) {
			need_reset = true;
		}
		break;
	default:
		need_reset = true;
		break;
	}
	if (need_reset) {
		get_empty_pos(idx_set, idx);
	}
	return 0;
}

inline int get_empty_pos(const Int_Hash_Set &idx_set, int &idx) {
	if (0 == idx_set.count(Position_Key::LINE_ONE_FRONT)
			&& 0 == idx_set.count(Position_Key::LINE_ONE_BEHIND)
			&& 0 == idx_set.count(Position_Key::LINE_ONE_FIRST)) {
		idx = Position_Key::LINE_ONE_FRONT;
	} else if (0 == idx_set.count(Position_Key::LINE_TWO_FRONT)
			&& 0 == idx_set.count(Position_Key::LINE_TWO_BEHIND)
			&& 0 == idx_set.count(Position_Key::LINE_TWO_FIRST)) {
		idx = Position_Key::LINE_TWO_FRONT;
	} else if (0 == idx_set.count(Position_Key::LINE_THREE_FRONT)
			&& 0 == idx_set.count(Position_Key::LINE_THREE_BEHIND)
			&& 0 == idx_set.count(Position_Key::LINE_THREE_FIRST)) {
		idx = Position_Key::LINE_THREE_FRONT;
	} else {
		idx = -1;
	}
	return 0;
}


int team_card_process_drop_mode(int8_t player_career, Drop_Vec &drops_all, const Drop_Config* drop_config,
		double mult_coe/* = 1.0*/, double lv_coe /*= 1.0*/);
int team_card_process_drop_box_mode(int8_t player_career, Drop_Vec &drops_all, const Drop_Box_Config& drop_box_config,
		double mult_coe /*= 1.0*/, double lv_coe /*= 1.0*/);
int team_card_drop_success_index(const std::vector<double>& rate_vec);
#endif /* TEAM_FUNC_H_ */
