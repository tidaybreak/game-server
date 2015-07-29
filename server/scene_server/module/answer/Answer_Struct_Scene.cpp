/*
 * Answer_Struct_Scene.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Answer_Struct_Scene.h"


void Answer_Rank_Role_Data::reset(void) {
	role_id = 0;
	name.clear();
	level = 0;
	sorce = 0;
	true_num = 0;
	exp = 0;
}

int Answer_Rank_Role_Data::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(name);
	buffer.write_int32(level);
	buffer.write_uint32(sorce);
	buffer.write_uint32(true_num);
	buffer.write_uint32(exp);
	return 0;
}

int Answer_Rank_Role_Data::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_string(name);
	buffer.read_int32(level);
	buffer.read_uint32(sorce);
	buffer.read_uint32(true_num);
	buffer.read_uint32(exp);
	return 0;
}

void Answer_Role_Data_Info::reset(void) {
	use_free_num = 0;
	use_double_num = 0;
	cur_free_qid = 0;
	cur_double_qid = 0;
}

int Answer_Role_Data_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_uint32(use_free_num);
	buffer.write_uint32(use_double_num);
	buffer.write_uint32(cur_free_qid);
	buffer.write_uint32(cur_double_qid);
	return 0;
}

int Answer_Role_Data_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_uint32(use_free_num);
	buffer.read_uint32(use_double_num);
	buffer.read_uint32(cur_free_qid);
	buffer.read_uint32(cur_double_qid);
	return 0;
}

void Answer_All_Data_Info::reset(void) {
	question_id_list.clear();
	role_sorce_map.clear();//用户积分和基本信息
	role_data_info_map.clear();//用户数据信息
	ext_num = 0;//已出即兴题数量
	cur_qno = 0;
	last_start_time = Time_Value::zero;
	force_rank.clear();
	arena_rank.clear();
	gang_rank.clear();
}

int Answer_All_Data_Info::serialize(Block_Buffer &buf) const {
	buf.write_uint16(question_id_list.size());
	for(UInt_Set::iterator it = question_id_list.begin(); it != question_id_list.end(); ++it) {
		buf.write_uint32(*it);
	}
	buf.write_uint16(role_sorce_map.size());
	for(RoleID_Data_Map::const_iterator it = role_sorce_map.begin(); it != role_sorce_map.end(); ++it) {
		buf.write_int64(it->first);
		it->second.serialize(buf);
	}
	buf.write_uint16(role_data_info_map.size());
	for(RoleID_Answer_Role_Data_Info_Map::const_iterator it = role_data_info_map.begin(); it != role_data_info_map.end(); ++it) {
		buf.write_int64(it->first);
		it->second.serialize(buf);
	}
	buf.write_uint16(force_rank.size());
	for(String_Vec::const_iterator it = force_rank.begin(); it != force_rank.end(); ++it) {
		buf.write_string(*it);
	}
	buf.write_uint16(arena_rank.size());
	for(String_Vec::const_iterator it = arena_rank.begin(); it != arena_rank.end(); ++it) {
		buf.write_string(*it);
	}
	buf.write_uint16(gang_rank.size());
	for(String_Vec::const_iterator it = gang_rank.begin(); it != gang_rank.end(); ++it) {
		buf.write_string(*it);
	}
	buf.write_uint32(ext_num);
	buf.write_uint32(cur_qno);
	buf.write_int64(last_start_time.sec());
	return 0;
}

int Answer_All_Data_Info::deserialize(Block_Buffer &buf) {
	uint16_t size = 0;
	buf.read_uint16(size);
	question_id_list.clear();
	for(uint32_t i = 0; i < size; ++i) {
		uint32_t tmp = 0;
		buf.read_uint32(tmp);
		question_id_list.insert(tmp);
	}

	size = 0;
	buf.read_uint16(size);
	role_sorce_map.clear();
	for(uint32_t i = 0; i < size; ++i) {
		int64_t role_id = 0;
		Answer_Rank_Role_Data data;
		buf.read_int64(role_id);
		data.deserialize(buf);
		role_sorce_map.insert(std::make_pair(role_id, data));
	}

	size = 0;
	buf.read_uint16(size);
	role_data_info_map.clear();
	for(uint32_t i = 0; i < size; ++i) {
		int64_t role_id = 0;
		Answer_Role_Data_Info data;
		buf.read_int64(role_id);
		data.deserialize(buf);
		role_data_info_map.insert(std::make_pair(role_id, data));
	}

	size = 0;
	buf.read_uint16(size);
	force_rank.clear();
	for(uint32_t i = 0; i < size; ++i) {
		std::string str;
		buf.read_string(str);
		force_rank.push_back(str);
	}

	size = 0;
	buf.read_uint16(size);
	arena_rank.clear();
	for(uint32_t i = 0; i < size; ++i) {
		std::string str;
		buf.read_string(str);
		arena_rank.push_back(str);
	}

	size = 0;
	buf.read_uint16(size);
	gang_rank.clear();
	for(uint32_t i = 0; i < size; ++i) {
		std::string str;
		buf.read_string(str);
		gang_rank.push_back(str);
	}

	buf.read_uint32(ext_num);//已出即兴题数量
	buf.read_uint32(cur_qno);
	int64_t time = 0;
	buf.read_int64(time);
	last_start_time.sec(time);
	return 0;
}

void Question_Info::reset(void) {
		id = 0;
		status = 0;
		true_answer = 0;
		a.clear();
		b.clear();
		over_time = 0;
}
