/*
 * Base_Detail.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: ti
 */
#include "Base_Detail.h"
#include "DB_Operator.h"
#include "Config_Cache.h"

Base_Detail::Base_Detail(void) {
	reset();
}

Base_Detail::~Base_Detail(void) {

}

void Base_Detail::reset(void) {
	role_id = 0;
	account_info.reset();
	location.reset();
	create_scene_args_.reset();
	role_name = "";
	level = 0;
	gender = 0;
	career = 0;
	create_time = Time_Value::zero;
	gang_id = 0;
	gang_name = "";
	last_sign_in_time = Time_Value::zero;
	last_sign_out_time = Time_Value::zero;
	last_trigger_daily_zero = Time_Value::zero;
	last_trigger_daily_six = Time_Value::zero;
	last_trigger_weekly_zero = Time_Value::zero;
	vip_over_time = Time_Value::zero;
	online = 0;
	vip = 0;
	ride = 0;
	mount_model_id = 0;
	rela_scene_id = 0;
	title_id = 0;
	head_id = 0;
	charm = 0;

	CLEAR_VECTOR(Int_Vec, avatar_vec);
	awake_lvl = 0;
	role_type = 0;
}

int Base_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	account_info.serialize(buffer);
	location.serialize(buffer);
	create_scene_args_.serialize(buffer);
	buffer.write_string(role_name);
	buffer.write_int32(level);
	buffer.write_uint8(gender);
	buffer.write_uint8(career);
	buffer.write_int64(gang_id);
	buffer.write_string(gang_name);
	buffer.write_int64(last_sign_out_time.sec());
	buffer.write_uint8(ride);
	buffer.write_int32(mount_model_id);
	buffer.write_int32(title_id);
	buffer.write_int32(vip);
	buffer.write_int64(vip_over_time.sec());
	buffer.write_uint32(head_id);
	buffer.write_uint32(charm);

	buffer.write_int16(avatar_vec.size());
	for (Int_Vec::const_iterator it = avatar_vec.begin(); it != avatar_vec.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_int32(awake_lvl);
	buffer.write_string(ip);
	buffer.write_uint8(role_type);
	return 0;
}

int Base_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	account_info.deserialize(buffer);
	location.deserialize(buffer);
	create_scene_args_.deserialize(buffer);
	buffer.read_string(role_name);

	buffer.read_int32(level);
	buffer.read_uint8(gender);
	buffer.read_uint8(career);
	buffer.read_int64(gang_id);
	buffer.read_string(gang_name);
	int64_t time = 0;
	buffer.read_int64(time);
	last_sign_out_time.sec(time);
	buffer.read_uint8(ride);
	buffer.read_int32(mount_model_id);
	buffer.read_int32(title_id);
	buffer.read_int32(vip);
	buffer.read_int64(time);
	vip_over_time.sec(time);
	buffer.read_uint32(head_id);
	buffer.read_uint32(charm);

	int16_t size = 0;
	buffer.read_int16(size);
	for (int16_t i = 0; i < size; ++i) {
		int32_t id = 0;
		buffer.read_int32(id);
		avatar_vec.push_back(id);
	}
	buffer.read_int32(awake_lvl);
	buffer.read_string(ip);
	buffer.read_uint8(role_type);
	return 0;
}

int Base_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_base_detail(*this);
	return 0;
}

int Base_Detail::load(void) {
	return MONGO_OPERATOR->load_base_detail(*this);
}

int Base_Detail::arena_load(void){
	return MONGO_OPERATOR->load_base_arena_detail(*this);
}
////////////////////////////////////////////////////////////////////

