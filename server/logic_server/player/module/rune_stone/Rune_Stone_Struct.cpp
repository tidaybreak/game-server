/*
 * Rune_Stone_Struct.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: chenyulin
 */

#include "DB_Operator.h"
#include "Rune_Stone_Struct.h"
#include "Rune_Stone_Def.h"
#include "Msg_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Rune_Stone_Struct.h"

void Smelter_Detail::serialize(Block_Buffer & w) const {
	w.write_int8(smelter_idx);
	w.write_int8(is_open);
}

int Smelter_Detail::deserialize(Block_Buffer & r) {
	if(r.read_int8(smelter_idx) ||
			r.read_int8(is_open)) {
		return -1;
	}
 	return 0;
}

void Smelter_Detail::reset(void) {
	smelter_idx = 0;
	is_open = 0;
}

Rune_Stone_Detail::Rune_Stone_Detail() {
	reset();
}

Rune_Stone_Detail::~Rune_Stone_Detail() {

}

int Rune_Stone_Detail::serialize(Block_Buffer & w) const {
	w.write_int64(role_id);
	w.write_int32(points);
	w.write_int32(remain_double_times);

	w.write_uint16(smelter_detail_map.size());
	for (Smelter_Detail_Map::const_iterator it = smelter_detail_map.begin(); it != smelter_detail_map.end(); ++it) {
		it->second.serialize(w);
	}

	return 0;
}

int Rune_Stone_Detail::deserialize(Block_Buffer & r) {
	r.read_int64(role_id);
	r.read_int32(points);

	uint16_t size = 0;
	r.read_uint16(size);
	Smelter_Detail smelter_detail;
	for (uint16_t i = 0; i < size; ++i) {
		smelter_detail.reset();
		smelter_detail.deserialize(r);
		if (smelter_detail.smelter_idx) {
			smelter_detail_map[smelter_detail.smelter_idx] = smelter_detail;
		}
	}

	return 0;
}

void Rune_Stone_Detail::reset(void) {
	role_id = 0;
	is_change = false;

	points = 0;
	remain_double_times = 0;
	smelter_detail_map.clear();
	combine_cnt_map.clear();
	pro_val_map.clear();
}

void Rune_Stone_Detail::make_smelter_info(const int smelter_idx, Smelter_Info &info) const {
	Smelter_Detail_Map::const_iterator it_smelter = smelter_detail_map.find(smelter_idx);
	if (it_smelter != smelter_detail_map.end()) {
		info.smelter_idx = it_smelter->second.smelter_idx;
		info.is_open = it_smelter->second.is_open;
	}
}

int Rune_Stone_Detail::load(void) {
	return MONGO_OPERATOR->load_rune_stone_detail(*this);
}

int Rune_Stone_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_rune_stone_detail(*this);
	return 0;
}

//void Rune_Detail::get_rune_part_info(const int part, Rune_Part_Info &info) const {
//	Rune_Hole_Info hole_info;
//	Rune_Part_Detail_Map::const_iterator it_part = part_detail_map.find(part);
//	if (it_part != part_detail_map.end()) {
//		info.part = it_part->second.part;
//		for (Rune_Hole_Detail_Map::const_iterator it_hole = it_part->second.hole_detail_map.begin();
//				it_hole != it_part->second.hole_detail_map.end(); ++it_hole) {
//			hole_info.reset();
//			hole_info.index = it_hole->second.index;
//			hole_info.level = it_hole->second.level;
//			hole_info.energy = it_hole->second.energy;
//			info.hole_infos.push_back(hole_info);
//		}
//	}
//}
