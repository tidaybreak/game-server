/*
 * MoveR_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef MOVER_STRUCT_H_
#define MOVER_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Struct.h"
#include "Mover_Def.h"

struct Mover_Detail {
	Mover_Type mover_type;
	Int16_Set moveable_value;

	uint layer_id;
	int block_index;
	Coord grid_coord;
	Coord pixel_coord;
	uint8_t toward;

	Position_Key battle_position;	// 战斗场景位置
	Position_Key cur_battle_position;	// 战斗场景位置
	Position_Key team_position;	// 组队时的布阵战斗场景位置

	Mover_Detail(void);
	void reset(void);
	bool is_player(void) const;
	bool is_hero(void) const;
	bool validate_moveable(int value);
};

#endif /* MOVER_STRUCT_H_ */
