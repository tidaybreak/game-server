/*
 * Gang_War_Struct.cpp
 *
 *  Created on: 2014年8月5日
 *      Author: Linqiyou
 */


#include "Gang_War_Struct.h"

Gang_War_Rank_Detail::Gang_War_Rank_Detail(void){
	reset();
}

void Gang_War_Rank_Detail::reset(void){
	role_id = 0;
	role_name = "";
	gang_id = 0;
	gang_name = "";
	league = 0;
	score = 100;
	src_hp = 0;
	hp =0;
	kill_num = 0;
	die_num = 0;
	tower_num = 0;
	merit = 0;
	total_merit = 0;
	contribute = 0;
	revive = 0;
	revive_time = Time_Value::zero;
	safe_time = Time_Value::zero;
	force = 0;
	skill_cd_time = Time_Value::zero;
	wait_time = Time_Value::zero;
	exit_ = false;
}

void Gang_War_Rank_Detail::serialize(Block_Buffer &buf) const {
	buf.write_int64(role_id);
	buf.write_string(role_name);
	buf.write_int64(gang_id);
	buf.write_string(gang_name);
	buf.write_int32(league);
	buf.write_int32(score);
	buf.write_double(src_hp);
	buf.write_double(hp);
	buf.write_int32(kill_num);
	buf.write_int32(die_num);
	buf.write_int32(tower_num);

	buf.write_int32(merit);
	buf.write_int32(total_merit);
	buf.write_int32(contribute);

	buf.write_int32(revive);
	revive_time.serialize(buf);
	safe_time.serialize(buf);
	buf.write_int32(force);
	skill_cd_time.serialize(buf);
	wait_time.serialize(buf);
	buf.write_bool(exit_);
}
void Gang_War_Rank_Detail::deserialize(Block_Buffer &buf) {
	buf.read_int64(role_id);
	buf.read_string(role_name);
	buf.read_int64(gang_id);
	buf.read_string(gang_name);
	buf.read_int32(league);
	buf.read_int32(score);
	buf.read_double(src_hp);
	buf.read_double(hp);
	buf.read_int32(kill_num);
	buf.read_int32(die_num);
	buf.read_int32(tower_num);

	buf.read_int32(merit);
	buf.read_int32(total_merit);
	buf.read_int32(contribute);

	buf.read_int32(revive);
	revive_time.deserialize(buf);
	safe_time.deserialize(buf);
	buf.read_int32(force);
	skill_cd_time.deserialize(buf);
	wait_time.deserialize(buf);
	buf.read_bool(exit_);
}
int Gang_War_Rank_Detail::add_merit(const int merit){
	this->merit += merit;
	this->total_merit += merit;
	return merit;
}
int Gang_War_Rank_Detail::add_contribute(const int contribute){
	this->contribute += contribute;
	return contribute;
}

/////////////////////////////////////////////////////////////////////////////////

Scene_Ganger_Detail::Scene_Ganger_Detail(void){
	reset();
}

void Scene_Ganger_Detail::reset(void){
	role_id = 0;
	level = 0;
	force = 0;
	gang_id = 0;
	total_merit = 0;
	remain_income_num = 0;
	income_rate = 0;
	league = 0;
	remain_time = 0;
	honor_field_id = 0;
}

////////////////////////////////////////////////////////////////////////////////////

//Gang_War_Scene_Info::Gang_War_Scene_Info(void){
//	reset();
//}
//
//void Gang_War_Scene_Info::reset(void){
//	id = 0;
//	remain_time = 0;
//}
