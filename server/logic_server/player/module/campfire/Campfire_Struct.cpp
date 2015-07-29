/*
 * Campfire_Struct.cpp
 *
 *  Created on: 2014年8月14日
 *      Author: root
 */

#include "Campfire_Struct.h"
#include "DB_Def.h"
#include "DB_Operator.h"
#include "DB_Manager.h"

Campfire_Detail::Campfire_Detail(void){
	reset();
}

Campfire_Detail::~Campfire_Detail(void){

}

void Campfire_Detail::reset(void){
	role_id = 0;
	is_change = false;
	logout = false;

	id = 0;
	lighter = 0;
	level = 0;
	gang_id = 0;
	gang_name = "";
	firewood_num = 0;
	firewood_num_temp = 0;
	burning = 0;
	level_firewood_num = 0;
	level_max_firewood_num = 0;
	coord.reset();
}

int Campfire_Detail::serialize(Block_Buffer &buffer) const{
	buffer.write_int32(id);
	buffer.write_int32(level);
	buffer.write_int64(gang_id);
	buffer.write_string(gang_name);
	buffer.write_int32(firewood_num);
	//buffer.write_int8(burning);
	buffer.write_int64(lighter);
	buffer.write_int32(level_firewood_num);
	buffer.write_int32(level_max_firewood_num);
	return 0;
}

int Campfire_Detail::deserialize(Block_Buffer &buffer){
	buffer.read_int32(id);
	buffer.read_int32(level);
	buffer.read_int64(gang_id);
	buffer.read_string(gang_name);
	buffer.read_int32(firewood_num);
	//buffer.read_int8(burning);
	buffer.read_int64(lighter);
	buffer.read_int32(level_firewood_num);
	buffer.read_int32(level_max_firewood_num);
	return 0;
}

int Campfire_Detail::load(void){
	return 0;
}

int Campfire_Detail::save(void){
	if(this->is_change){
		Block_Buffer buf;
		buf.make_message(MONGO_SAVE_CAMPFIRE_DETAIL);
		this->serialize(buf);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, this->id);
		this->is_change = false;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////

FB_Info_Detail::FB_Info_Detail(void){
	reset();
}

FB_Info_Detail::~FB_Info_Detail(void){

}

void FB_Info_Detail::reset(void){
	role_id = 0;
	is_change = false;
	logout = false;

	best_force = 0;
	best_role_id = 0;
	best_role_name = "";
}


int FB_Info_Detail::serialize(Block_Buffer &buffer) const{
	buffer.write_int64(role_id);
	buffer.write_int32(best_force);
	buffer.write_int64(best_role_id);
	buffer.write_string(best_role_name);
	return 0;
}

int FB_Info_Detail::deserialize(Block_Buffer &buffer){
	buffer.read_int64(role_id);
	buffer.read_int32(best_force);
	buffer.read_int64(best_role_id);
	buffer.read_string(best_role_name);
	return 0;
}

int FB_Info_Detail::load(void){
	return 0;
}

int FB_Info_Detail::save(void){
	if(this->is_change){
		Block_Buffer buf;
		buf.make_message(MONGO_SAVE_FB_INFO);
		this->serialize(buf);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, this->role_id);
		this->is_change = false;
	}
	return 0;
}
