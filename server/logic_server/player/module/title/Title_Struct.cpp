/*
 * Title_Struct.cpp
 *
 *  Created on: 2014年7月24日
 *      Author: Linqiyou
 */

#include "Title_Struct.h"
#include "DB_Operator.h"
#include "Title_Func.h"

int Title_Detail_Info_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(id);
	buffer.write_int32(last_time);
	start_time.serialize(buffer);
	last_end_time.serialize(buffer);
	return 0;
}

int Title_Detail_Info_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(id);
	buffer.read_int32(last_time);
	start_time.deserialize(buffer);
	last_end_time.deserialize(buffer);
	return 0;
}

int Title_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);

	buffer.write_int32(title_set.size());
	for (Title_Detail_Info::const_iterator it = title_set.begin();
			it != title_set.end(); ++it) {
		it->second.serialize(buffer);
	}
	buffer.write_int32(equip_title);

	buffer.write_int8((int8_t)is_change);
	return 0;
}

int Title_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);

	int n = 0;
	buffer.read_int32(n);
	Title_Detail_Info_Detail info;
	title_set.clear();
	title_time_list.clear();
	for (int i = 0; i < n; ++i) {
		info.reset();
		info.deserialize(buffer);
		title_set.insert(std::make_pair(info.id, info));
		Title_Detail_Info::iterator it = title_set.find(info.id);
		if (it != title_set.end()) {
			insert_title_and_sort(&(it->second), title_time_list);
		}
	}
	buffer.read_int32(equip_title);

	buffer.read_int8((int8_t &)is_change);
	return 0;
}

int Title_Detail::load(void) {
	return MONGO_OPERATOR->load_title_detail(*this);
}

int Title_Detail::save(void) {
	if (is_change) {
		is_change = false;
		return MONGO_OPERATOR->save_title_detail(*this);
	}
	else
		return 0;
}

void Title_Detail::reset(void) {
	role_id = 0;

	title_set.clear();
	title_time_list.clear();
	equip_title = 0;
}

int Title_Record_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(charts_title_map.size());
	for (Charts_Title_Map::const_iterator iter = charts_title_map.begin();
			iter != charts_title_map.end(); ++iter) {
		buffer.write_int32(iter->first);

		buffer.write_int32(iter->second.size());
		for (Charts_Info_Vector::const_iterator it = iter->second.begin();
				it != iter->second.end(); ++it) {
			buffer.write_int64(it->role_id);
			buffer.write_int32(it->title_id);
		}
	}
	buffer.write_int64(gang_war_first_leader);
	return 0;
}

int Title_Record_Detail::deserialize(Block_Buffer &buffer) {
	int first_amount = 0;
	int second_amount = 0;

	buffer.read_int32(first_amount);
	for (int i = 0; i < first_amount; ++i) {
		int charts_type = 0;
		buffer.read_int32(charts_type);

		buffer.read_int32(second_amount);
		Charts_Info_Vector charts_info_vector;
		charts_info_vector.clear();
		for (int j = 0; j < second_amount; ++j) {
			Charts_Info charts_info;
			buffer.read_int64(charts_info.role_id);
			buffer.read_int32(charts_info.title_id);
			charts_info_vector.push_back(charts_info);
		}
		charts_title_map.insert(std::make_pair(charts_type, charts_info_vector));
	}
	buffer.read_int64(gang_war_first_leader);
	return 0;
}

int Title_Record_Detail::load(void) {
	return MONGO_OPERATOR->load_title_record(*this);
}

int Title_Record_Detail::save(void) {
	return MONGO_OPERATOR->save_title_record(*this);
}

void Title_Record_Detail::reset(void) {
	charts_title_map.clear();
	gang_war_first_leader = 0;
}

