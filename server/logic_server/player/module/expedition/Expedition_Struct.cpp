/*
 * Expedition_Struct.cpp
 *
 *  Created on: 2014年10月16日
 *      Author: xiaoliang
 */

#include "Expedition_Struct.h"
#include "DB_Operator.h"

Gang_Data_Info::Gang_Data_Info(void) {
	reset();
}

void Gang_Data_Info::reset(void) {
	line = 0;
	coord.reset();
}

Expedition_Detail::Expedition_Detail(void) {
	reset();
}

Expedition_Detail::~Expedition_Detail(void) {

}

void Expedition_Detail::reset(void) {
	role_id = 0;
	coord.reset();

	kill_beast.clear();
	collect_beast.clear();
	collect_chest.clear();
	award_time = Time_Value::zero;

	award_nums = 0;
	drops_all_vec_.clear();
	box_collect.clear();
}

int Expedition_Detail::serialize(Block_Buffer &w) const {
	w.write_int64(role_id);
	coord.serialize(w);
	w.write_uint16(kill_beast.size());
	for (Int_Int_Map::const_iterator it = kill_beast.begin(); it != kill_beast.end(); ++it) {
		w.write_int32(it->first);
		w.write_int32(it->second);
	}
	w.write_uint16(collect_beast.size());
	for (Int_Hash_Set::const_iterator it = collect_beast.begin(); it != collect_beast.end(); ++it) {
		w.write_int32(*it);
	}
	w.write_uint16(collect_chest.size());
	for (Int_Hash_Set::const_iterator it = collect_chest.begin(); it != collect_chest.end(); ++it) {
		w.write_int32(*it);
	}
	award_time.serialize(w);

	w.write_uint16(box_collect.size());
	for (Int_Time_Map::const_iterator it = box_collect.begin(); it != box_collect.end(); ++it) {
		w.write_int32(it->first);
		it->second.serialize(w);
	}

	w.write_int32(award_nums);

	return 0;
}

int Expedition_Detail::deserialize(Block_Buffer &r) {
	r.read_int64(role_id);
	coord.deserialize(r);
	uint16_t len = 0;
	int k = 0;
	int v = 0;
	r.read_uint16(len);
	kill_beast.clear();
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(k);
		r.read_int32(v);
		kill_beast.insert(std::make_pair(k, v));
	}

	r.read_uint16(len);
	collect_beast.clear();
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(v);
		collect_beast.insert(v);
	}

	r.read_uint16(len);
	collect_chest.clear();
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(v);
		collect_chest.insert(v);
	}
	award_time.deserialize(r);

	r.read_uint16(len);
	Time_Value tv = Time_Value::zero;
	box_collect.clear();
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(k);
		tv.deserialize(r);
		box_collect.insert(std::make_pair(k, tv));
	}

	r.read_int32(award_nums);

	return 0;
}

int Expedition_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_expedition_detail(*this);
	return 0;
}

int Expedition_Detail::load(void) {
	return MONGO_OPERATOR->load_expedition_detail(*this);
}

int Expedition_Manager_Detail::load(void) {
	MONGO_OPERATOR->load_expedition_manager_detail(*this);
	return 0;
}
int Expedition_Manager_Detail::save(void) {
	MONGO_OPERATOR->save_expedition_manager_detail(*this);
	return 0;
}

int Expedition_Manager_Detail::serialize(Block_Buffer &w) const {
	w.write_uint16(gang_data_info_map.size());
	for (Int64_Gang_Data_Info_Map::const_iterator it = gang_data_info_map.begin();
			it != gang_data_info_map.end(); ++it) {
		w.write_int64(it->first);
		w.write_uint32(it->second.line);
		it->second.coord.serialize(w);
	}
	flag_time.serialize(w);
	w.write_uint16(award.size());
	for (Int64_Item_Info_Vec_Map::const_iterator it_iii = award.begin();
			it_iii != award.end(); ++it_iii) {
		w.write_int64(it_iii->first);
		w.write_uint16(it_iii->second.size());
		for (Item_Info_Vec::const_iterator it = it_iii->second.begin();
				it != it_iii->second.end(); ++it) {
			it->serialize(w);
		}
	}
	award_time.serialize(w);
	first_listen_time.serialize(w);
	return 0;
}

int Expedition_Manager_Detail::deserialize(Block_Buffer &r) {
	uint16_t len = 0;
	r.read_uint16(len);
	Gang_Data_Info gdi;
	int64_t gang_id = 0;
	for (uint16_t i = 0; i < len; ++i) {
		gdi.reset();
		gang_id = 0;
		r.read_int64(gang_id);
		r.read_uint32(gdi.line);
		gdi.coord.deserialize(r);
		gang_data_info_map.insert(std::make_pair(gang_id, gdi));
	}
	flag_time.deserialize(r);
	Item_Info item_info;
	r.read_uint16(len);
	uint16_t len_2 = 0;
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(gang_id);
		r.read_uint16(len_2);
		for (uint16_t j = 0; j < len_2; ++j) {
			item_info.deserialize(r);
			award[gang_id].push_back(item_info);
		}
	}
	award_time.deserialize(r);
	first_listen_time.deserialize(r);
	return 0;
}
