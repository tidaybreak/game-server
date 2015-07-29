/*
 * Mover_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Mover_Struct.h"


Mover_Detail::Mover_Detail(void) {
	reset();
}

void Mover_Detail::reset(void) {
	layer_id = 0;
	mover_type = MOVER_T_PLAYER;
	block_index = 0;
	grid_coord.reset();
	pixel_coord.type = Coord::GRID_TYPE;
	pixel_coord.reset();
	pixel_coord.type = Coord::PIXEL_TYPE;
	toward = 0;
	battle_position.reset();
	cur_battle_position.reset();
	team_position.reset();
}

bool Mover_Detail::validate_moveable(int value) {
	if (moveable_value.count(value) > 0)
		return true;
	else
		return false;
}

bool Mover_Detail::is_player(void) const {
	return mover_type == MOVER_T_PLAYER;
}

bool Mover_Detail::is_hero(void) const {
	return mover_type == MOVER_T_HERO;
}
