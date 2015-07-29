/*
 * Team_Struct.cpp
 *
 *  Created on: 2014年4月15日
 *      Author: Linqiyou
 */

#include "Team_Struct.h"
#include "DB_Operator.h"

void Team_Sports_Ranking_Detail::serialize(Block_Buffer &buf) const {
	buf.write_int64(role_id_);
	buf.write_int32(integrate_);
	buf.write_uint16(win_times);
	buf.write_uint16(fail_times);

}
void Team_Sports_Ranking_Detail::deserialize(Block_Buffer &buf) {
	buf.read_int64(role_id_);
	buf.read_int32(integrate_);
	buf.read_uint16(win_times);
	buf.read_uint16(fail_times);

}

Teamer_Detail::Teamer_Detail(void) {
	reset();
}

Teamer_Detail::~Teamer_Detail(void) {

}

void Teamer_Detail::reset(void) {
	this->ready = false;
	this->role_id = 0;
	this->team_id = 0;
	this->profile_num_fb = 0;
	this->profile_num_fb_pre = 0;
	this->profile_num_sports = 0;
	this->profile_num_hero_dream = 0;
	profile_num_defend = 0;
	this->profile_time = Time_Value::zero;
	this->team_match_info.reset();
	this->wait_area_type = 0;
	this->is_not_first_team_fb = false;
	this->is_change = false;
	team_award.clear();
}

int Teamer_Detail::serialize(Block_Buffer &buf) const {
	buf.write_bool(ready);
	buf.write_int64(role_id);
	buf.write_int64(team_id);
	buf.write_int32(profile_num_fb);
	buf.write_int32(profile_num_fb_pre);
	buf.write_int32(profile_num_sports);
	buf.write_int32(profile_num_hero_dream);
	buf.write_int32(profile_num_defend);
	buf.write_int8(wait_area_type);
	this->profile_time.serialize(buf);
	this->team_match_info.serialize(buf);
	buf.write_bool(is_not_first_team_fb);
	return 0;
}
int Teamer_Detail::deserialize(Block_Buffer &buf) {
	buf.read_bool(ready);
	buf.read_int64(role_id);
	buf.read_int64(team_id);
	buf.read_int32(profile_num_fb);
	buf.read_int32(profile_num_fb_pre);
	buf.read_int32(profile_num_sports);
	buf.read_int32(profile_num_hero_dream);
	buf.read_int32(profile_num_defend);
	buf.read_int8(wait_area_type);
	this->profile_time.deserialize(buf);
	this->team_match_info.deserialize(buf);
	buf.read_bool(is_not_first_team_fb);
	return 0;
}
int Teamer_Detail::load(void) {
	return MONGO_OPERATOR->load_teamer_detail(*this);
}
int Teamer_Detail::save(void) {
	return MONGO_OPERATOR->save_teamer_detail(*this);
}
void First_Lvl_Detail::serialize(Block_Buffer &w) const {
	w.write_uint16(first_lvl_role_id_his.size());
	for (Int_Uid::const_iterator it = first_lvl_role_id_his.begin(); it != first_lvl_role_id_his.end(); ++it) {
		w.write_int32(it->first);
		w.write_int64(it->second);
	}
	w.write_uint16(first_lvl_ck_his.size());
	for (Int_Uid::const_iterator it = first_lvl_ck_his.begin(); it != first_lvl_ck_his.end(); ++it) {
		w.write_int32(it->first);
		w.write_int64(it->second);
	}
	w.write_uint16(first_lvl_zs_his.size());
	for (Int_Uid::const_iterator it = first_lvl_zs_his.begin(); it != first_lvl_zs_his.end(); ++it) {
		w.write_int32(it->first);
		w.write_int64(it->second);
	}
	w.write_uint16(first_lvl_yss_his.size());
	for (Int_Uid::const_iterator it = first_lvl_yss_his.begin(); it != first_lvl_yss_his.end(); ++it) {
		w.write_int32(it->first);
		w.write_int64(it->second);
	}
}
void First_Lvl_Detail::deserialize(Block_Buffer &r) {
	uint16_t len = 0;
	int32_t lvl = 0;
	role_id_t role_id = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(lvl);
		r.read_int64(role_id);
		first_lvl_role_id_his.insert(std::make_pair(lvl, role_id));
	}
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(lvl);
		r.read_int64(role_id);
		first_lvl_ck_his.insert(std::make_pair(lvl, role_id));
	}
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(lvl);
		r.read_int64(role_id);
		first_lvl_zs_his.insert(std::make_pair(lvl, role_id));
	}
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(lvl);
		r.read_int64(role_id);
		first_lvl_yss_his.insert(std::make_pair(lvl, role_id));
	}
}
