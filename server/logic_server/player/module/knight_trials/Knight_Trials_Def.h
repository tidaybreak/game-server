/*
 * Knight_Trials_Def.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef KNIGHT_TRIALS_DEF_H_
#define KNIGHT_TRIALS_DEF_H_

#include "Knight_Trials_Struct.h"


const int KNIGHT_TRIALS_INNER_SCENE_ID = 80207001;
const int KNIGHT_TRIALS_FIGHTER_SCENE_MAP_ID = 80301002;

enum Knight_trials_Match_Type{
	KNIGHT_TRIALS_SIX_MATCH = 1,
	KNIGHT_TRIALS_ONE_MATCH = 2,

	KNIGHT_TRIALS_SIX_MATCH_BACK = 11,
	KNIGHT_TRIALS_ONE_MATCH_BACK = 12,
};

enum Knight_trials_Comm_Def {
	KNIGHT_TRIALS_BOX_REWARD_CONF_TYPE_MONEY = 1,
	KNIGHT_TRIALS_BOX_REWARD_CONF_TYPE_ITEM = 2,
	KNIGHT_TRIALS_ICON_ID = 11801,

};


#endif /* KNIGHT_TRIALS_DEF_H_ */
