/*
 * Anci_Artifact_Struct.cpp
 *
 *  Created on: 2014年10月10日
 *      Author: Linqiyou
 */

#include "Anci_Artifact_Struct.h"
#include "DB_Operator.h"


Anci_Artifact_Detail::Anci_Artifact_Detail(void) {
	reset();
}
Anci_Artifact_Detail::~Anci_Artifact_Detail() {}
void Anci_Artifact_Detail::reset(void) {
	role_id = 0;
//	perfectPoint = 0;
	aa_info_map.clear();
	complete_ids.clear();
	aa_unban_info.clear();
}

int Anci_Artifact_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
//	buffer.write_int64(perfectPoint);
	buffer.write_uint16(aa_info_map.size());
	for (AA_Info_Map::const_iterator it = aa_info_map.begin(); it != aa_info_map.end(); ++it) {
		buffer.write_int32(it->second.id);
		buffer.write_int8(it->second.state_count);
		buffer.write_int32(it->second.data);
	}
	buffer.write_uint16(complete_ids.size());
	for (Int_Hash_Set::const_iterator it = complete_ids.begin(); it != complete_ids.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_uint16(aa_unban_info.size());
	for (AA_Unban_Info::const_iterator it = aa_unban_info.begin(); it != aa_unban_info.end(); ++it) {
		buffer.write_int32(it->second.id);
		buffer.write_int8(it->second.state);
		buffer.write_int8(it->second.box_state);
	}
	return 0;
}
int Anci_Artifact_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
//	buffer.read_int64(perfectPoint);
	uint16_t len = 0;
	buffer.read_uint16(len);
	aa_info_map.clear();
	Anci_Artifact_Cond_Info info;
	for (uint16_t i = 0; i < len; ++i) {
		info.reset();
		buffer.read_int32(info.id);
		buffer.read_int8(info.state_count);
		buffer.read_int32(info.data);
		aa_info_map.insert(std::make_pair(info.id, info));
	}
	buffer.read_uint16(len);
	complete_ids.clear();
	int32_t complete_id = 0;
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(complete_id);
		complete_ids.insert(complete_id);
	}
	aa_unban_info.clear();
	Anci_Artifact_Unban_Info aa_ui;
	for (uint16_t i = 0; i < len; ++i) {
		aa_ui.reset();
		buffer.read_int32(aa_ui.id);
		buffer.read_int8(aa_ui.state);
		buffer.read_int8(aa_ui.box_state);
		aa_unban_info.insert(std::make_pair(aa_ui.id, aa_ui));
	}
	return 0;
}

int Anci_Artifact_Detail::load(void) {
	return MONGO_OPERATOR->load_anci_artifact_detail(*this);
}
int Anci_Artifact_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_anci_artifact_detail(*this);
	else
		return 0;
}
