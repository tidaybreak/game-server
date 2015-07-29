/*
 * Hero_Struct.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: chenyulin
 */

#include "Hero_Struct.h"

int Hero_Base_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(master_role_id);
	buffer.write_string(master_role_name);
	buffer.write_int64(hero_role_id);
	buffer.write_int32(hero_index);
	buffer.write_int32(hero_id);
	buffer.write_string(hero_name);
	buffer.write_int32(hero_facade_id);
	buffer.write_int32(hero_formation);
	buffer.write_int32(hero_level);
	buffer.write_int32(awake_lvl);

	return 0;
}

int Hero_Base_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(master_role_id);
	buffer.read_string(master_role_name);
	buffer.read_int64(hero_role_id);
	buffer.read_int32(hero_index);
	buffer.read_int32(hero_id);
	buffer.read_string(hero_name);
	buffer.read_int32(hero_facade_id);
	buffer.read_int32(hero_formation);
	buffer.read_int32(hero_level);
	buffer.read_int32(awake_lvl);

	return 0;
}

void Hero_Base_Detail::reset(void) {
	master_role_id = 0;
	master_role_name.clear();
	hero_role_id = 0;
	hero_index = 0;
	hero_id = 0;
	hero_name.clear();
	hero_facade_id = 0;
	hero_formation = 0;
	hero_level = 0;
	awake_lvl = 0;
	pos_key.reset();
}
