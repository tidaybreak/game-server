/*
 * World_Boss_Struct.cpp
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */


#include "World_Boss_Struct.h"
#include "DB_Def.h"
#include "DB_Operator.h"
#include "DB_Manager.h"



World_Boss_Detail::World_Boss_Detail(void){
	reset();
}

World_Boss_Detail::~World_Boss_Detail(void){

}

void World_Boss_Detail::reset(void){
	act_id = 0;
	act_state = 0;
	open_num = 0;
	kill_num = 0;
	boss_lv = 0;
}

int World_Boss_Detail::serialize(Block_Buffer &buffer) const{
	buffer.write_int64(act_id);
	buffer.write_int8(act_state);
	buffer.write_int32(open_num);
	buffer.write_int32(kill_num);
	buffer.write_int32(boss_lv);
	return 0;
}

int World_Boss_Detail::deserialize(Block_Buffer &buffer){
	buffer.read_int64(act_id);
	buffer.read_int8(act_state);
	buffer.read_int32(open_num);
	buffer.read_int32(kill_num);
	buffer.read_int32(boss_lv);
	return 0;
}

int World_Boss_Detail::load(void){
	return 0;
}

int World_Boss_Detail::save(void){
	if(this->is_change){
		Block_Buffer buf;
		buf.make_message(MONGO_SAVE_WORLD_BOSS_INFO);
		this->serialize(buf);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, 0);
		this->is_change = false;
	}
	return 0;
}


