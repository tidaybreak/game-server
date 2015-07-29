/*
 * Logic_Fighter_Struct.h
 *
 *  Created on: Dec 13, 2014
 *      Author: linqiyou
 */

#ifndef LOGIC_FIGHTER_STRUCT_H_
#define LOGIC_FIGHTER_STRUCT_H_

enum Dungeon_Reward_Type {
	DR_EXP_START = 1,
	BASE_EXP = 1,
	VIP_EXP = 2,
	TOOL_EXP = 3,
	FRIEND_EXP = 4,

	DR_EXP_END
};
struct Exp_All {
	Exp_All(void) { reset(); }
	void reset(void) {
		exp = 0;
		vip_exp = 0;
		buff_exp = 0;
		result = 0;
		all = 0;
		all_without_world_level = 0;
	}
	int exp_sum(void);

	int exp;
	int vip_exp;
	int buff_exp;
	int result;
	int all;
	int all_without_world_level;
};


#endif /* LOGIC_FIGHTER_STRUCT_H_ */
