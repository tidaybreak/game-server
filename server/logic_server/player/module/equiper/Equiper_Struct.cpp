/*
 * Equiper_Struct.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: chenyulin
 */

#include "DB_Operator.h"
#include "Equiper_Struct.h"
#include "Equiper_Def.h"
#include "fighter/Fighter_Def.h"

void Master_Part_Info_Detail::serialize(Block_Buffer & w) const {
	w.write_int32(index);
	w.write_int32(level);
	w.write_int32(luck_value);
	w.write_int32(fail_cnt);
}

int Master_Part_Info_Detail::deserialize(Block_Buffer & r) {
	if( r.read_int32(index)  ||  r.read_int32(level)  ||  r.read_int32(luck_value)  || r.read_int32(fail_cnt))
			return -1;
	return 0;
}

void Master_Part_Info_Detail::reset(void) {
	index = 0;
	level = 0;
	luck_value = 0;
	fail_cnt = 0;
}

Equiper_Detail::Equiper_Detail() {
	reset();
}

Equiper_Detail::~Equiper_Detail() {
}

int Equiper_Detail::serialize(Block_Buffer &buffer) const {
	// not necessary
	return 0;
}

int Equiper_Detail::deserialize(Block_Buffer &buffer) {
	// not necessary
	return 0;
}

int Equiper_Detail::load(void) {
	return MONGO_OPERATOR->load_equiper_detail(*this);
}

int Equiper_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_equiper_detail(*this);
	else
		return 0;
}

void Equiper_Detail::reset(void) {
	role_id = 0;
	suit_cnt_map.clear();
	notified_overdue_idx.clear();
	master_part_info.clear();
	gem_data.clear();
	carve_lvl = 0;
	gem_progress = 0;
	is_change = false;
}

void Equip_Force_Info::reset(void) {
	pt_val_map.clear();

	pt_val_map[PT_FORCE] = 0.0;

	pt_val_map[PT_POWER] = 0.0;
	pt_val_map[PT_ARMOR] = 0.0;
	pt_val_map[PT_STAMINA] = 0.0;
	pt_val_map[PT_PHY_ATTACK] = 0.0;
	pt_val_map[PT_SPELL_ATTACK] = 0.0;
	pt_val_map[PT_PHY_DEFENSE] = 0.0;
	pt_val_map[PT_SPELL_DEFENSE] = 0.0;
	pt_val_map[PT_HIT] = 0.0;
	pt_val_map[PT_AVOID] = 0.0;
	pt_val_map[PT_CRIT] = 0.0;
	pt_val_map[PT_CRIT_DEF] = 0.0;
	pt_val_map[PT_BLOOD_MAX] = 0.0;
}

double Equip_Force_Info::calculate_force(void) {
	pt_val_map[PT_FORCE] = pt_val_map[PT_POWER] * 2.5
			+ pt_val_map[PT_ARMOR] * 2.5
			+ pt_val_map[PT_STAMINA] * 2.5
			+ pt_val_map[PT_PHY_ATTACK] * 1 + pt_val_map[PT_SPELL_ATTACK] * 1
			+ pt_val_map[PT_PHY_DEFENSE] * 0.5 + pt_val_map[PT_SPELL_DEFENSE] * 0.5
			+ pt_val_map[PT_BLOOD_MAX] * 0.1667
			+ pt_val_map[PT_CRIT] * 5
			+ pt_val_map[PT_CRIT_DEF] * 5
			+ pt_val_map[PT_HIT] * 5
			+ pt_val_map[PT_AVOID] * 5
			;
	return pt_val_map[PT_FORCE];
}
