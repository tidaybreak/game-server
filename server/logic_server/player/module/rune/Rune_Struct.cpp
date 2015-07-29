/*
 * Rune_Struct.cpp
 *
 *  Created on: May 15, 2014
 *      Author: chenyulin
 */

#include "DB_Operator.h"
#include "Rune_Struct.h"
#include "Rune_Def.h"
#include "Msg_Struct.h"

void Rune_Hole_Detail::serialize(Block_Buffer & w) const {
	w.write_int32(index);
	w.write_int32(level);
	w.write_int32(energy);
}

int Rune_Hole_Detail::deserialize(Block_Buffer & r) {
	if( r.read_int32(index)  ||  r.read_int32(level)  ||  r.read_int32(energy)  )
			return -1;
	return 0;
}

void Rune_Hole_Detail::reset(void) {
	index = 0;
	level = 0;
	energy = 0;
}

void Rune_Part_Detail::serialize(Block_Buffer & w) const {
	w.write_int32(part);

	w.write_uint16(hole_detail_map.size());
	for (Rune_Hole_Detail_Map::const_iterator it = hole_detail_map.begin(); it != hole_detail_map.end(); ++it) {
		it->second.serialize(w);
	}
}

int Rune_Part_Detail::deserialize(Block_Buffer & r) {
	if(r.read_int32(part))
		return -1;

	uint16_t size = 0;
	r.read_uint16(size);
	Rune_Hole_Detail hole_detail;
	for (uint16_t i = 0; i < size; ++i) {
		hole_detail.reset();
		hole_detail.deserialize(r);
		hole_detail_map[hole_detail.index] = hole_detail;
	}

	return 0;
}

void Rune_Part_Detail::reset(void) {
	part = 0;
	hole_detail_map.clear();
}


Rune_Detail::Rune_Detail() {
	reset();
}

Rune_Detail::~Rune_Detail() {

}

int Rune_Detail::serialize(Block_Buffer & w) const {
	w.write_uint16(part_detail_map.size());
	for (Rune_Part_Detail_Map::const_iterator it = part_detail_map.begin(); it != part_detail_map.end(); ++it) {
		it->second.serialize(w);
	}
	return 0;
}

int Rune_Detail::deserialize(Block_Buffer & r) {
	uint16_t size = 0;
	r.read_uint16(size);
	Rune_Part_Detail part_detail;
	for (uint16_t i = 0; i < size; ++i) {
		part_detail.reset();
		part_detail.deserialize(r);
		part_detail_map[part_detail.part] = part_detail;
	}
	return 0;
}

void Rune_Detail::reset(void) {
	role_id = 0;
	is_change = false;
	part_detail_map.clear();
	all_rune_pro_val.clear();
}

int Rune_Detail::load(void) {
	return MONGO_OPERATOR->load_rune_detail(*this);
}

int Rune_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_rune_detail(*this);
	return 0;
}

void Rune_Detail::get_rune_part_info(const int part, Rune_Part_Info &info) const {
	Rune_Hole_Info hole_info;
	Rune_Part_Detail_Map::const_iterator it_part = part_detail_map.find(part);
	if (it_part != part_detail_map.end()) {
		info.part = it_part->second.part;
		for (Rune_Hole_Detail_Map::const_iterator it_hole = it_part->second.hole_detail_map.begin();
				it_hole != it_part->second.hole_detail_map.end(); ++it_hole) {
			hole_info.reset();
			hole_info.index = it_hole->second.index;
			hole_info.level = it_hole->second.level;
			hole_info.energy = it_hole->second.energy;
			info.hole_infos.push_back(hole_info);
		}
	}
}

void Rune_Detail::get_rune_hole_info(const int part, const int hole_index, Rune_Hole_Info &info) const {
	Rune_Part_Detail_Map::const_iterator it_part = part_detail_map.find(part);
	if (it_part != part_detail_map.end()) {
		Rune_Hole_Detail_Map::const_iterator it_hole = it_part->second.hole_detail_map.find(hole_index);
		if (it_hole != it_part->second.hole_detail_map.end()) {
			info.index = it_hole->second.index;
			info.level = it_hole->second.level;
			info.energy = it_hole->second.energy;
		}
	}
}
