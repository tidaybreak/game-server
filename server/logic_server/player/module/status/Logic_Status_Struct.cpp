/*
 * Logic_Status_Struct.cpp
 *
 *  Created on: 2014年6月12日
 *      Author: Linqiyou
 */


#include "Logic_Status_Struct.h"
#include "DB_Operator.h"


Logic_Status_Detail::Logic_Status_Detail(void) {
	reset();
}

Logic_Status_Detail::~Logic_Status_Detail(void) {

}

int Logic_Status_Detail::serialize(Block_Buffer &buffer) const {
//	buffer.write_int64(this->role_id);
	buffer.write_uint16(status_map.size());
	for (Status_Map::const_iterator it = status_map.begin(); it != status_map.end(); ++it) {
		it->second.serialize(buffer);
	}
	running_effect.serialize(buffer);
	return 0;
}

int Logic_Status_Detail::deserialize(Block_Buffer &buffer) {
//	buffer.read_int64(this->role_id);
	uint16_t len = 0;
	buffer.read_uint16(len);
	Status_Info status_info;
	status_map.clear();
	for (uint16_t i = 0; i < len; ++i) {
		status_info.reset();
		status_info.deserialize(buffer);
		status_map.insert(std::make_pair(status_info.status_id_, status_info));
	}
	running_effect.reset();
	running_effect.deserialize(buffer);
	return 0;
}

void Logic_Status_Detail::reset(void) {
	role_id = 0;
//	status_detail.reset();
	status_map.clear();
	running_effect.reset();
}

int Logic_Status_Detail::load(void) {
	return MONGO_OPERATOR->load_status_detail(*this);
}

int Logic_Status_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_status_detail(*this);
	else
		return 0;
}
