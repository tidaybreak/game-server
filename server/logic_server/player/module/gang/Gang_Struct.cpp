/*
 * Gang_Struct.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: caijianwei
 */

#include "Gang_Struct.h"
#include "DB_Operator.h"
#include "Fighter_Detail.h"
#include "Msg_Struct.h"
#include "Msg_Active_Struct.h"
#include "Logic_Player_Struct.h"
#include "Logic_Player.h"
#include "Logic_Monitor.h"
#include "DB_Def.h"
#include "Pool_Manager.h"
#include "Config_Cache_Gang.h"

static bool make_role_info(role_id_t role_id, Role_Info& role_info) {
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id( role_id );
	if ( player ) { // online
		role_info.career = player->career();
		role_info.combat = player->fighter_detail().force;
		role_info.gender = player->base_detail().gender;
		role_info.level  = player->base_detail().level;
		role_info.role_id = player->role_id();
		role_info.role_name = player->base_detail().role_name;
		role_info.vip_level = player->vip();
	} else {
		DB_View::ID_Player_View_Map::iterator find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find( role_id );
		if ( find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end() ) {
			Player_DB_View& player_view = *(find_it->second);
			role_info.career = player_view.career;
			role_info.combat = 0; // TODO unknown;
			role_info.gender = player_view.gender;
			role_info.level  = player_view.level;
			role_info.role_id = player_view.role_id;
			role_info.role_name = player_view.role_name;
			role_info.vip_level = player_view.vip_level;
		}
	}
	return true;
}


void Ganger_Skill_Detail::serialize(Block_Buffer & w) const {
	w.write_int32(skill_id);
}

int Ganger_Skill_Detail::deserialize(Block_Buffer & r) {
	if(r.read_int32(skill_id))
		return -1;
	return 0;
}

void Ganger_Skill_Detail::reset(void) {
	skill_id = 0;
}

Ganger_Detail::Ganger_Detail() {
	reset();
}

Ganger_Detail::~Ganger_Detail() {
}

void Ganger_Detail::reset(void) {
	gang_id = 0;
	last_gang_id = 0;
	role_id = 0;
	cur_contribute = 0;
	total_contribute = 0;
	achi_contribute = 0;
	day_contribute = 0;
	day_gold_contrib_times = 0;
	day_diam_contrib_times = 0;
	contrib_of_week = 0;
	last_rank = 0;
	authority = 0;
	join_time = Time_Value::zero;
	leave_time = Time_Value::zero;
	headship = 0;
	receive_bless_times = 0;
	//remain_draw_times = 0;
	today_has_draw_times = 0;
	CLEAR_VECTOR(Affair_Vec, affairs);
	invites.clear();
	apply_ids.clear();
	skill_map.clear();
	draw_awards.clear();

	gang_war_kill_num = 0;
	gang_war_hold_tower = 0;
	gang_war_contribute = 0;
	gang_war_merit = 0;
	gang_war_total_merit = 0;
	gang_war_camp = 0;
	gang_war_attend_time = Time_Value::zero;
	gang_war_attend_gang_id = 0;
	gang_war_attend_gang_id = 0;
	gang_war_award_get_times_by_day = 0;
	skill_pro_val.clear();

	practice_times_ = 0;
	practice_times_ttl_ = 0;
	practice_start_time_ = Time_Value::zero;
}

int Ganger_Detail::serialize(Block_Buffer& buf) const {
	buf.write_int64(gang_id);
	buf.write_int64(last_gang_id);
	buf.write_int64(role_id);
	buf.write_int32(cur_contribute);
	buf.write_uint32(total_contribute);
	buf.write_uint32(day_contribute);
	buf.write_uint32(contrib_of_week);
	buf.write_int32(day_gold_contrib_times);
	buf.write_int32(day_diam_contrib_times);
	buf.write_int32(last_rank);
	buf.write_int32(authority);
	buf.write_uint32(join_time.sec());
	buf.write_uint32(leave_time.sec());
	buf.write_int8(headship);
	buf.write_int8(receive_bless_times);
	//buf.write_int32(remain_draw_times);
	buf.write_int32(today_has_draw_times);

	uint16_t invites_size = invites.size();
	buf.write_uint16(invites_size);
	for (uint16_t i = 0; i < invites_size; ++i) {
		//invites[i].serialize(buf);
		buf.write_int64(invites[i]);
	}

	uint16_t size = apply_ids.size();
	buf.write_uint16(size);
	for (Role_Id_Set::const_iterator it = apply_ids.begin();
			it != apply_ids.end(); ++it) {
		buf.write_int64(*it);
	}

	size = skill_map.size();
	buf.write_uint16(size);
	for (Ganger_Skill_Map::const_iterator it = skill_map.begin();
			it != skill_map.end(); ++it) {
		it->second.serialize(buf);
	}

	size = draw_awards.size();
	buf.write_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buf.write_int32(draw_awards[i]);
	}

	gang_war_attend_time.serialize(buf);
	buf.write_int64(gang_war_attend_gang_id);
	buf.write_int64(gang_war_attend_gang_id2);

	buf.write_uint16(gang_war_award_get_times_by_day);
	return 0;
}

int Ganger_Detail::deserialize(Block_Buffer& buf) {
	uint32_t uint_join_time = 0;
	uint32_t uint_leave_time = 0;
	if (buf.read_int64(gang_id)
		|| buf.read_int64(last_gang_id)
		|| buf.read_int64(role_id)
		|| buf.read_int32(cur_contribute)
		|| buf.read_uint32(total_contribute)
		|| buf.read_uint32(day_contribute)
		|| buf.read_uint32(contrib_of_week)
		|| buf.read_int32(day_gold_contrib_times)
		|| buf.read_int32(day_diam_contrib_times)
		|| buf.read_int32(last_rank)
		|| buf.read_int32(authority)
		|| buf.read_int8(receive_bless_times)
		//|| buf.read_int32(remain_draw_times)
		|| buf.read_int32(today_has_draw_times)
		|| buf.read_uint32(uint_join_time)
		|| buf.read_uint32(uint_leave_time)
		|| buf.read_int8(headship)) {
		return -1;
	}
	join_time.sec( uint_join_time );
	leave_time.sec( uint_leave_time );

	uint16_t invites_size = 0;
	if(buf.read_uint16(invites_size)) {
		return -1;
	}
	invites.reserve(invites_size);
	//Gang_Invite_Info infite_info;
	int64_t gang_id = 0;
	for(uint16_t i = 0; i < invites_size; ++i) {
		//infite_info.reset();
		//if(infite_info.deserialize(buf)) {
		//	return -1;
		//}
		gang_id = 0;
		if(buf.read_int64(gang_id)) {
			return -1;
		}
		invites.push_back(gang_id);
	}

	uint16_t size = 0;

	if (buf.read_uint16(size)) {
		return -1;
	}
	role_id_t apply_role_id = 0;
	for(uint16_t i = 0; i < size; ++i) {
		apply_role_id = 0;
		if(buf.read_int64(apply_role_id)) {
			return -1;
		}
		apply_ids.insert(gang_id);
	}


	if (buf.read_uint16(size)) {
		return -1;
	}
	Ganger_Skill_Detail skill_detail;
	for(uint16_t i = 0; i < size; ++i) {
		skill_detail.reset();
		if (skill_detail.deserialize(buf)) {
			continue;
		};
		skill_map.insert(std::make_pair(skill_detail.skill_id, skill_detail));
	}

	if (buf.read_uint16(size)) {
		return -1;
	}
	int32_t draw_card_id = 0;
	for(uint16_t i = 0; i < size; ++i) {
		draw_card_id = 0;
		if (buf.read_int32(draw_card_id)) {
			return -1;
		}
		draw_awards.push_back(draw_card_id);
	}
	gang_war_attend_time.deserialize(buf);
	buf.read_int64(gang_war_attend_gang_id);
	buf.read_int64(gang_war_attend_gang_id2);
	buf.read_uint16(gang_war_award_get_times_by_day);

	return 0;
}

int Ganger_Detail::load(void) {
	return MONGO_OPERATOR->load_ganger_detail(*this);
}

int Ganger_Detail::save(void) {
	if(this->role_id)
		return MONGO_OPERATOR->save_ganger_detail(*this);

	return 0;
}

void Ganger_Detail::set_quit(gang_id_t quit_gang_id) {
	last_gang_id = quit_gang_id;
	gang_id = 0;
	cur_contribute = 0;
	total_contribute = 0;
	day_contribute = 0;
	contrib_of_week = 0;
	day_gold_contrib_times = 0;
	day_diam_contrib_times = 0;
	last_rank = 0;
	receive_bless_times = 0;
	//remain_draw_times = 0;
	//today_has_draw_times = 0;
	authority = 0;
	headship = 0;
	affairs.clear();
	join_time = Time_Value::zero;
	leave_time = Time_Value::gettimeofday();
}
void Ganger_Detail::set_gang_war_award_get_times_by_day(uint16_t gw) {
	gang_war_award_get_times_by_day = gw;
}

void Ganger_Detail::offline_set_quit() {
	last_gang_id = gang_id;
	gang_id = 0;
	cur_contribute = 0;
	total_contribute = 0;
	day_contribute = 0;
	contrib_of_week = 0;
	day_gold_contrib_times = 0;
	day_diam_contrib_times = 0;
	last_rank = 0;
	authority = 0;
	receive_bless_times = 0;
	//remain_draw_times = 0;
	//today_has_draw_times = 0;
	headship = 0;
	affairs.clear();
	join_time = Time_Value::zero;
	//leave_time = Time_Value::gettimeofday();
}

int Ganger_Detail::gang_war_serialize(Block_Buffer& buf) const {
	buf.write_int64(gang_id);
	buf.write_int64(role_id);
	buf.write_int32(authority);
	buf.write_int8(headship);

	buf.write_int32(gang_war_kill_num);
	buf.write_int32(gang_war_hold_tower);
	buf.write_uint32(gang_war_contribute);
	buf.write_int32(gang_war_merit);
	buf.write_int32(gang_war_total_merit);
	buf.write_int8(gang_war_camp);

	gang_war_attend_time.serialize(buf);
	buf.write_int64(gang_war_attend_gang_id);
	buf.write_int64(gang_war_attend_gang_id2);
	buf.write_uint16(gang_war_award_get_times_by_day);
	return 0;
}

int Ganger_Detail::gang_war_deserialize(Block_Buffer& buf) {
	if ( buf.read_int64(gang_id)
		|| buf.read_int64(role_id)
		|| buf.read_int32(authority)
		|| buf.read_int8(headship)
		|| buf.read_int32(gang_war_kill_num)
		|| buf.read_int32(gang_war_hold_tower)
		|| buf.read_uint32(gang_war_contribute)
		|| buf.read_int32(gang_war_merit)
		|| buf.read_int32(gang_war_total_merit)
		|| buf.read_int8(gang_war_camp)
		|| gang_war_attend_time.deserialize(buf)
		|| buf.read_int64(gang_war_attend_gang_id)
		|| buf.read_int64(gang_war_attend_gang_id2)
		|| buf.read_uint16(gang_war_award_get_times_by_day)) {
		return -1;
	}

	return 0;
}


void Gang_Member_Detail::serialize(Block_Buffer & w) const {
	w.write_int64(role_id);
	w.write_uint32(week_contribute);
	w.write_uint32(total_contribute);
	w.write_uint32(fight);
	w.write_int32(amplify_times);
	w.write_int32(level);
	w.write_int32(authority);
	w.write_int8(vip);
	w.write_int8(headship);
	w.write_int8(career);
	w.write_int8(line_stat);
	w.write_int8(gender);
	w.write_string(ganger_name);
}

int Gang_Member_Detail::deserialize(Block_Buffer & r) {
	if(r.read_int64(role_id) ||
			r.read_uint32(week_contribute) ||
			r.read_uint32(total_contribute) ||
			r.read_uint32(fight) ||
			r.read_int32(amplify_times) ||
			r.read_int32(level) ||
			r.read_int32(authority) ||
			r.read_int8(vip) ||
			r.read_int8(headship) ||
			r.read_int8(career) ||
			r.read_int8(line_stat) ||
			r.read_int8(gender) ||
			r.read_string(ganger_name)) {
		return -1;
	}
	return 0;
}

void Gang_Member_Detail::reset(void) {
	role_id = 0;
	week_contribute = 0;
	total_contribute = 0;
	fight = 0;
	amplify_times = 0;
	level = 0;
	authority = 0;
	vip = 0;
	headship = 0;
	career = 0;
	line_stat = 0;
	gender = 0;
	ganger_name.clear();
}

void Gang_Skill_Detail::serialize(Block_Buffer & w) const {
	w.write_int32(skill_id);
}

int Gang_Skill_Detail::deserialize(Block_Buffer & r) {
	if(r.read_int32(skill_id))
		return -1;
	return 0;
}

void Gang_Skill_Detail::reset(void) {
	skill_id = 0;
}

void Gang_Building_Detail::serialize(Block_Buffer & w) const {
	w.write_int32(building_id);
	w.write_int32(building_lvl);
	w.write_int32(cd_upgrade.sec());
}

int Gang_Building_Detail::deserialize(Block_Buffer & r) {
	r.read_int32(building_id);
	r.read_int32(building_lvl);

	int time_value = 0;
	r.read_int32(time_value);
	cd_upgrade.set(time_value, 0);

	return 0;
}

void Gang_Building_Detail::reset(void) {
	building_id = 0;
	building_lvl = 0;
	cd_upgrade = Time_Value::zero;
}

// ====================  Gang_DB_View ===================================
Gang_DB_View::Gang_DB_View() {
	reset();
}

Gang_DB_View::~Gang_DB_View() {
}

void Gang_DB_View::reset(void) {
	gang_id = 0;
	name = "";
	leader = 0;
	fund = 0;
	maintain_cost = 0;
	fund_lack_times = 0;
	level = 0;
	boss_act = 0;
	boss_hour = 0;
	boss_minute = 0;
	boss_kill_num = 0;
	boss_lv = 0;
	rank = 0;
	max_members = 0;
	logo = 0;
	day_deficit_count = 0;
	status = GANG_STAT_NON;
	join_block = 0;
	announcement = "";
	can_seize_power = 0;
	last_leader_id = 0;
	notified_last_leader = 0;
	force_total = 0;

	create_time = Time_Value::zero;
	dismiss_time = Time_Value::zero;
	cd_upgrade = Time_Value::zero;
	cd_annouce = Time_Value::zero;
	tv_leader_offline = Time_Value::zero;

	server_info.agent_num = 0;
	server_info.server_num = 0;
	member_map.clear();
	CLEAR_VECTOR(Affair_Vec, affairs);
	CLEAR_VECTOR(Simple_Event_Vec, simple_events);
	CLEAR_VECTOR(Passed_Event_Vec, passed_events);
	direct_in.clear();
	CLEAR_VECTOR(Apply_Info_Vec, apply_infos);
	researched_skills.clear();
	buildings.clear();

	gang_war_award.clear();
	gang_war_award_index = 0;

	acitve_annon_ed.clear();
	level_leader_id.clear();

	boss_act_state = 0;
	boss_act_time = Time_Value::zero;
	boss_act_next_time = Time_Value::zero;

	change = false;
}

int Gang_DB_View::serialize(Block_Buffer& w) const {
	w.write_int64(gang_id);
	w.write_string(name);
	w.write_int64(leader);
	w.write_int32(fund);
	w.write_int32(maintain_cost);
	w.write_int32(fund_lack_times);
	w.write_int32(level);
	w.write_int32(rank);
	//w.write_int32(max_members);
	w.write_int8(logo);
	w.write_int8(day_deficit_count);
	w.write_int8(status);
	w.write_int8(join_block);
	w.write_string(announcement);
	w.write_int8(can_seize_power);
	w.write_int64(last_leader_id);
	w.write_int8(notified_last_leader);
	w.write_int32(force_total);

	w.write_int32(create_time.sec());
	w.write_int32(dismiss_time.sec());
	w.write_int32(cd_upgrade.sec());
	w.write_int32(cd_annouce.sec());
	w.write_int32(tv_leader_offline.sec());

	server_info.serialize(w);

	w.write_uint16(member_map.size());
	for (Member_Map::const_iterator it = member_map.begin(); it != member_map.end(); ++it) {
		it->second.serialize(w);
	}

	w.write_uint16(direct_in.size());
	for (Role_Hash_Set::const_iterator it = direct_in.begin(); it != direct_in.end(); ++it) {
		w.write_int64(*it);
	}

//	uint16_t size = affairs.size();
//	w.write_uint16(size);
//	for(uint16_t i = 0; i < size; ++i) {
//		affairs[i].serialize(w);
//	}

	//passed_events

	uint16_t size = apply_infos.size();
	w.write_uint16(size);
	for(uint16_t i = 0; i < size; ++i) {
		apply_infos[i].serialize(w);
	}

	w.write_uint16(researched_skills.size());
	for (Gang_Skill_Map::const_iterator it = researched_skills.begin(); it != researched_skills.end(); ++it) {
		it->second.serialize(w);
	}

	w.write_uint16(buildings.size());
	for (Gang_Building_Map::const_iterator it = buildings.begin(); it != buildings.end(); ++it) {
		it->second.serialize(w);
	}

	w.write_uint16(gang_war_award.size());
	for (boost::unordered_map<int64_t, Reward_Info_Gang_War>::const_iterator it = gang_war_award.begin(); it != gang_war_award.end(); ++it) {
		it->second.serialize(w);
	}
	w.write_int64(gang_war_award_index);

	w.write_uint16(acitve_annon_ed.size());
	for (UInt_Set::const_iterator it = acitve_annon_ed.begin(); it != acitve_annon_ed.end(); ++it) {
		w.write_uint32(*it);
	}

	w.write_uint16(level_leader_id.size());
	for(boost::unordered_map<int32_t, role_id_t>::const_iterator it = level_leader_id.begin(); it != level_leader_id.end(); ++it) {
		w.write_int32(it->first);
		w.write_int64(it->second);
	}
	return 0;
}

int Gang_DB_View::deserialize(Block_Buffer& r) {
	r.read_int64(gang_id);
	r.read_string(name);
	r.read_int64(leader);
	r.read_int32(fund);
	r.read_int32(maintain_cost);
	r.read_int32(fund_lack_times);
	r.read_int32(level);
	r.read_int32(rank);
	//r.read_int32(max_members);
	r.read_int8(logo);
	r.read_int8(day_deficit_count);
	r.read_int8(status);
	r.read_int8(join_block);
	r.read_string(announcement);
	r.read_int8(can_seize_power);
	r.read_int64(last_leader_id);
	r.read_int8(notified_last_leader);
	r.read_int32(force_total);

	int time_value = 0;
	r.read_int32(time_value);
	create_time.set(time_value, 0);

	r.read_int32(time_value);
	dismiss_time.set(time_value, 0);

	r.read_int32(time_value);
	cd_upgrade.set(time_value, 0);

	r.read_int32(time_value);
	cd_annouce.set(time_value, 0);

	r.read_int32(time_value);
	tv_leader_offline.set(time_value, 0);

	server_info.deserialize(r);

	uint16_t size = 0;
	r.read_uint16(size);
	Gang_Member_Detail member_detail;
	for (uint16_t i = 0; i < size; i++) {
		member_detail.reset();
		member_detail.deserialize(r);
		member_map[member_detail.role_id] = member_detail;
	}

	role_id_t direct_in_id = 0;
	r.read_uint16(size);
	for (uint16_t i = 0; i < size; i++) {
		r.read_int64(direct_in_id);
		direct_in.insert(direct_in_id);
	}

	r.read_uint16(size);
	Gang_Apply_Info apply_info;
	for(uint16_t i = 0; i < size; ++i) {
		apply_info.reset();
		apply_info.deserialize(r);
		apply_infos.push_back(apply_info);
	}

	Gang_Skill_Detail skill_detail;
	r.read_uint16(size);
	for(uint16_t i = 0; i < size; ++i) {
		skill_detail.reset();
		if (skill_detail.deserialize(r)) {
			continue;
		};
		researched_skills.insert(std::make_pair(skill_detail.skill_id, skill_detail));
	}

	Gang_Building_Detail building_detail;
	r.read_uint16(size);
	for(uint16_t i = 0; i < size; ++i) {
		building_detail.reset();
		if (building_detail.deserialize(r)) {
			continue;
		};
		if (building_detail.building_id > 0) {
			buildings.insert(std::make_pair(building_detail.building_id, building_detail));
		}
	}

	r.read_uint16(size);
	Reward_Info_Gang_War rigw;
	for(uint16_t i = 0; i < size; ++i) {
		rigw.reset();
		if (rigw.deserialize(r)) {
			continue;
		}
		gang_war_award.insert(std::make_pair(rigw.index, rigw));
		if (building_detail.building_id > 0) {
			buildings.insert(std::make_pair(building_detail.building_id, building_detail));
		}
	}
	r.read_int64(gang_war_award_index);

	r.read_uint16(size);
	for(;size > 0; --size) {
		uint32_t val = 0;
		r.read_uint32(val);
		acitve_annon_ed.insert(val);
	}

	r.read_uint16(size);
	for(;size > 0; --size) {
		int32_t level = 0;
		role_id_t role_id = 0;
		r.read_int32(level);
		r.read_int64(role_id);
		level_leader_id[level] = role_id;
	}
	return 0;
}

int Gang_DB_View::gang_war_serialize(Block_Buffer& w) const {
	w.write_int64(gang_id);
	w.write_string(name);
	w.write_int64(leader);
	w.write_int32(fund);
	w.write_int32(level);
	w.write_int64(last_leader_id);
	w.write_int32(force_total);
	server_info.serialize(w);
	return 0;
}

int Gang_DB_View::gang_war_deserialize(Block_Buffer& r) {
	r.read_int64(gang_id);
	r.read_string(name);
	r.read_int64(leader);
	r.read_int32(fund);
	r.read_int32(level);
	r.read_int64(last_leader_id);
	r.read_int32(force_total);
	server_info.deserialize(r);
	return 0;
}

int Gang_DB_View::save(void) {
	Gang_DB_View *gang = POOL_MANAGER->pop_gang_db_view();
	if (!gang) {
		LOG_USER("pop gang error");
		return 0;
	}

	*gang = *this;

	DB_MANAGER->push_save_gang(gang);

	/*
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_GANG);
	this->serialize(buf);
	buf.finish_message();
	//MONGO_OPERATOR->save_gang_db_view(*this)
	 * */
	return 0;
}

void Gang_DB_View_Rank::reset(void) {
	gang_id = 0;
	level = 0;
	member_size = 0;
	fund = 0;
	force_total = 0;
}

bool operator<(const Gang_DB_View_Rank &param1, const Gang_DB_View_Rank &param2) {
	if (param1.level < param2.level) {
		return true;
	} else if (param1.level > param2.level) {
		return false;
	}

	if (param1.force_total < param2.force_total) {
		return true;
	} else if (param1.force_total > param2.force_total) {
		return false;
	}

	if (param1.member_size < param2.member_size) {
		return true;
	} else if (param1.member_size > param2.member_size) {
		return false;
	}

	if (param1.fund < param2.fund) {
		return true;
	} else if (param1.fund > param2.fund) {
		return false;
	}

	if (param1.gang_id > param2.gang_id) {
		return true;
	} else {
		return false;
	}
}

void Gang_DB_View_War_Rank::reset(void) {
	gang_id = 0;
	level = 0;
	member_size = 0;
	fund = 0;
	force_total = 0;
}

bool operator<(const Gang_DB_View_War_Rank &param1, const Gang_DB_View_War_Rank &param2) {
	if (param1.force_total < param2.force_total) {
		return true;
	} else if (param1.force_total > param2.force_total) {
		return false;
	}

	if (param1.member_size < param2.member_size) {
		return true;
	} else if (param1.member_size > param2.member_size) {
		return false;
	}

	if (param1.fund < param2.fund) {
		return true;
	} else if (param1.fund > param2.fund) {
		return false;
	}

	if (param1.level < param2.level) {
		return true;
	} else if (param1.level > param2.level) {
		return false;
	}

	if (param1.gang_id > param2.gang_id) {
		return true;
	} else {
		return false;
	}
}

void Gang_DB_View::send_last_passed_event() {
	if ( passed_events.empty() ) return ;
	MSG_81070002 msg;
	msg.passed_event = passed_events.back();
	for( Member_Map::iterator iter = member_map.begin(); iter != member_map.end(); ++iter) {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id( iter->first );
		if ( player ) {
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
}

void Gang_DB_View::get_last_n_events(int n, Passed_Event_Vec& out) {
	int count = 0;
	for( Passed_Event_Vec::reverse_iterator riter=passed_events.rbegin();
			riter!=passed_events.rend() && count<n; ++riter ) {
		out.push_back( *riter );
		++count;
	}
}

//void Gang_DB_View::add_passed_event101900(role_id_t role_act, role_id_t role_id) {
//	Gang_Passed_Event new_event;
//	new_event.event_id = GANG_PASSED_EVENT_WELLCOM;
//	new_event.time = Time_Value::gettimeofday().sec();
//	Gang_Passed_Event_Data data;
//	data.pack_type = 2;
//	make_role_info(role_id, data.role_info);
//	data.role_info.guild_name = name;
//	new_event.event_data.push_back( data );
//	passed_events.push_back( new_event );
//	// make simple string event
//	Role_Info role_info_act;
//	make_role_info(role_act, role_info_act);
//	std::stringstream sss_maker;
//	sss_maker << "成员招募  " << role_info_act.role_name << "批准"
//			<< data.role_info.role_name << "加入公会";
//	Int_String new_simple_event;
//	new_simple_event.int_value = new_event.time;
//	new_simple_event.str_value = sss_maker.str();
//	simple_events.push_back( new_simple_event );
//}

void Gang_DB_View::add_passed_event_101900(role_id_t role_act, role_id_t role_in) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_act;
	event_data_act.pack_type = 2;
	make_role_info(role_in, event_data_act.role_info);
	event_data_act.role_info.guild_name = name;

	Gang_Passed_Event_Data event_data_in;
	event_data_in.pack_type = 2;
	make_role_info(role_in, event_data_in.role_info);
	event_data_in.role_info.guild_name = name;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_WELLCOM;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_act);
	passed_event.event_data.push_back(event_data_in);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	Role_Info role_info_act;
	make_role_info(role_act, role_info_act);

	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	std::string title_content = event_cfg->event_name + " " + event_cfg->event_content;
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), title_content.c_str(),
			role_info_act.role_name.c_str(), event_data_in.role_info.role_name.c_str());

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101902(role_id_t role_id) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data;
	event_data.pack_type = 2;
	make_role_info(role_id, event_data.role_info);
	event_data.role_info.guild_name = name;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_QUIT;
	passed_event.time = Time_Value::gettimeofday().sec();

	passed_event.event_data.push_back(event_data);
	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	std::string title_content = event_cfg->event_name + " " + event_cfg->event_content;
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), title_content.c_str(), event_data.role_info.role_name.c_str());

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101903(role_id_t role_act, role_id_t role_out) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_act;
	event_data_act.pack_type = 2;
	make_role_info(role_act, event_data_act.role_info);
	event_data_act.role_info.guild_name = name;

	Gang_Passed_Event_Data event_data_out;
	event_data_out.pack_type = 2;
	make_role_info(role_out, event_data_out.role_info);
	event_data_out.role_info.guild_name = name;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_KICK_OUT;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_act);
	passed_event.event_data.push_back(event_data_out);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	std::string title_content = event_cfg->event_name + " " + event_cfg->event_content;
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), title_content.c_str(),
			event_data_act.role_info.role_name.c_str(), event_data_out.role_info.role_name.c_str());

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101901(role_id_t role_active, role_id_t role_passive, const int head_ship) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_act;
	event_data_act.pack_type = 2;
	make_role_info(role_active, event_data_act.role_info);
	event_data_act.role_info.guild_name = name;

	Gang_Passed_Event_Data event_data_pas;
	event_data_pas.pack_type = 2;
	make_role_info(role_passive, event_data_pas.role_info);
	event_data_pas.role_info.guild_name = name;

	Gang_Passed_Event_Data event_data_str;
	event_data_str.pack_type = 3;
	event_data_str.text = CONFIG_CACHE_GANG->power(head_ship).name;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_HEADSHIP;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_act);
	passed_event.event_data.push_back(event_data_pas);
	passed_event.event_data.push_back(event_data_str);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	//std::string title_content = event_cfg->event_name + " " + event_cfg->event_content;
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), event_cfg->event_content.c_str(), event_data_act.role_info.role_name.c_str(),
			event_data_pas.role_info.role_name.c_str(), event_data_str.text.c_str());

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101904(role_id_t role_active, const int cost_fund) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_act;
	event_data_act.pack_type = 2;
	make_role_info(role_active, event_data_act.role_info);
	event_data_act.role_info.guild_name = name;

	Gang_Passed_Event_Data event_data_int1;
	event_data_int1.pack_type = 0;
	event_data_int1.value32 = cost_fund;

	Gang_Passed_Event_Data event_data_int2;
	event_data_int2.pack_type = 0;
	event_data_int2.value32 = level;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_GANG_UPGRADE;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_act);
	passed_event.event_data.push_back(event_data_int1);
	passed_event.event_data.push_back(event_data_int2);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), event_cfg->event_content.c_str(),
			event_data_act.role_info.role_name.c_str(),	cost_fund, level);

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101906(role_id_t role_active, role_id_t role_passive) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_act;
	event_data_act.pack_type = 2;
	make_role_info(role_active, event_data_act.role_info);
	event_data_act.role_info.guild_name = name;

	Gang_Passed_Event_Data event_data_pas;
	event_data_pas.pack_type = 2;
	make_role_info(role_passive, event_data_pas.role_info);
	event_data_pas.role_info.guild_name = name;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_RESIGN;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_act);
	passed_event.event_data.push_back(event_data_pas);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), event_cfg->event_content.c_str(),
			event_data_act.role_info.role_name.c_str(), event_data_pas.role_info.role_name.c_str());

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101907(role_id_t role_active, role_id_t role_passive) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_act;
	event_data_act.pack_type = 2;
	make_role_info(role_active, event_data_act.role_info);
	event_data_act.role_info.guild_name = name;

	Gang_Passed_Event_Data event_data_pas;
	event_data_pas.pack_type = 2;
	make_role_info(role_passive, event_data_pas.role_info);
	event_data_pas.role_info.guild_name = name;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_SEIZE_POWER;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_act);
	passed_event.event_data.push_back(event_data_pas);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), event_cfg->event_content.c_str(),
			event_data_act.role_info.role_name.c_str(), event_data_pas.role_info.role_name.c_str());

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101908(void) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_int1;
	event_data_int1.pack_type = 0;
	event_data_int1.value32 = rank;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_RANK_CHANG;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_int1);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), event_cfg->event_content.c_str(), rank);

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101909(const int day) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_int1;
	event_data_int1.pack_type = 0;
	event_data_int1.value32 = day;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_DAILY_FUND_LACK;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_int1);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), event_cfg->event_content.c_str(), day);

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

void Gang_DB_View::add_passed_event_101910(role_id_t role_active) {
	/*
	 * passed_events
	 */
	Gang_Passed_Event_Data event_data_act;
	event_data_act.pack_type = 2;
	make_role_info(role_active, event_data_act.role_info);
	event_data_act.role_info.guild_name = name;

	Gang_Passed_Event passed_event;
	passed_event.event_id = GANG_PASSED_EVENT_SET_ANNOUNCE;
	passed_event.time = Time_Value::gettimeofday().sec();
	passed_event.event_data.push_back(event_data_act);

	passed_events.push_back(passed_event);

	/*
	 * simple_events
	 */
	const Gang_Cfg_Event *event_cfg = CONFIG_CACHE_GANG->find_event(passed_event.event_id);
	if (! event_cfg) {
		return;
	}
	char event_char[100];
	memset(event_char, 0x00, sizeof(event_char));
	snprintf(event_char, sizeof(event_char), event_cfg->event_content.c_str(),
			event_data_act.role_info.role_name.c_str());

	Int_String simple_event;
	simple_event.int_value = passed_event.time;
	simple_event.str_value = event_char;
	simple_events.push_back(simple_event);
}

bool Gang_DB_View::has_apply_info(role_id_t role_id) const {
	for (Gang_DB_View::Apply_Info_Vec::const_iterator it_apply = apply_infos.begin();
			it_apply != apply_infos.end(); ++it_apply) {
		if (it_apply->role_id == role_id) {
			return true;
		}
	}
	return false;
}

bool Gang_DB_View::erase_apply_info(role_id_t role_id) {
	for (Gang_DB_View::Apply_Info_Vec::iterator it_apply = apply_infos.begin();
			it_apply != apply_infos.end(); ) {
		if (it_apply->role_id == role_id) {
			it_apply = apply_infos.erase(it_apply);
		} else {
			++it_apply;
		}
	}
	return false;
}

bool Gang_DB_View::can_direct_in(role_id_t role_id) {
	return direct_in.find(role_id) != direct_in.end();
}

bool Gang_DB_View::has_member(role_id_t role_id) {
	return member_map.count(role_id);
}

std::string Gang_DB_View::headship_name(const int head_ship) {
	switch (head_ship) {
	case GANG_HEADSHIP_NORMAL:
		return "NORMAL";
	case GANG_HEADSHIP_OFFICER:
		return "OFFICER";
	case GANG_HEADSHIP_VICE:
		return "VICE_LEADER";
	case GANG_HEADSHIP_LEADER:
		return "LEADER";
	default:
		return "";
	}
}
boost::unordered_map<int64_t, Reward_Info_Gang_War> &Gang_DB_View::gang_war_award_map(void) {
	return gang_war_award;
}
role_id_t Gang_DB_View::make_gang_war_index(void) {
	return ++gang_war_award_index;
}
int Gang_DB_View::add_contribute(const int donate_gain) {
	if (donate_gain == 0) {
		return 0;
	}
	// add donate gain in gang fund
	fund += donate_gain;
	change_view();
	return 0;
}


int Gang_War_Rank_Award_Detail::serialize(Block_Buffer &w) const {
	w.write_int8(rank);
	w.write_int64(gang_id);
	w.write_int8(state);
	w.write_string(gang_name);
	w.write_string(zone);
	w.write_int32(score);
	w.write_int8(win_nums);
	w.write_int32(kill_nums);
	w.write_int32(force);
	return 0;
}
int Gang_War_Rank_Award_Detail::deserialize(Block_Buffer &r) {
	r.read_int8(rank);
	r.read_int64(gang_id);
	r.read_int8(state);
	r.read_string(gang_name);
	r.read_string(zone);
	r.read_int32(score);
	r.read_int8(win_nums);
	r.read_int32(kill_nums);
	r.read_int32(force);
	return 0;
}
bool compress_gang_war_rank_award_detail(const Gang_War_Rank_Award_Detail &gwrad1, const Gang_War_Rank_Award_Detail &gwrad2) {
	if (gwrad1.score > gwrad2.score) {
		return true;
	} else if (gwrad1.score < gwrad2.score) {
		return false;
	}
	if (gwrad1.win_nums > gwrad2.win_nums) {
		return true;
	} else if (gwrad1.win_nums < gwrad2.win_nums) {
		return false;
	}
	if (gwrad1.kill_nums > gwrad2.kill_nums) {
		return true;
	} else if (gwrad1.kill_nums < gwrad2.kill_nums) {
		return false;
	}
	if (gwrad1.force > gwrad2.force) {
		return true;
	} else if (gwrad1.force < gwrad2.force) {
		return false;
	}
	if (gwrad1.gang_id < gwrad2.gang_id) {
		return true;
	}
	return false;
}
int Gang_War_Gang_Rank_Info::serialize(Block_Buffer &w) const {
	w.write_int64(gang_id);
	w.write_string(gang_name);
	w.write_string(zone);
	w.write_int32(score);
	w.write_int32(win_nums);
	w.write_int32(kill_nums);
	w.write_int32(hold_nums);
	w.write_int32(force);
	w.write_uint16(ganger_rank_info.size());
	for (boost::unordered_map<role_id_t, Gang_War_Rank_Info>::const_iterator it = ganger_rank_info.begin(); it != ganger_rank_info.end(); ++it) {
		it->second.serialize(w);
	}
	return 0;
}
int Gang_War_Gang_Rank_Info::deserialize(Block_Buffer &r) {
	r.read_int64(gang_id);
	r.read_string(gang_name);
	r.read_string(zone);
	r.read_int32(score);
	r.read_int32(win_nums);
	r.read_int32(kill_nums);
	r.read_int32(hold_nums);
	r.read_int32(force);
	uint16_t len = 0;
	r.read_uint16(len);
	Gang_War_Rank_Info gwri;
	for (uint16_t i = 0; i < len; ++i) {
		gwri.reset();
		gwri.deserialize(r);
		ganger_rank_info.insert(std::make_pair(gwri.role_id, gwri));
	}
	return 0;
}
bool compress_gang_war_rank_info(const Gang_War_Gang_Rank_Info &gwrad1, const Gang_War_Gang_Rank_Info &gwrad2) {
	if (gwrad1.score > gwrad2.score) {
		return true;
	} else {
		return false;
	}
	return false;
}

int Gang_War_Gang_Rank_Info::add_ganger_rank_info(const Gang_War_Rank_Info &gri) {
	gang_id = gri.gang_id;
	gang_name = gri.gang_name;
	kill_nums += gri.kill_num;
	hold_nums += gri.hold_spot;
	score += gri.kill_num * CONFIG_CACHE_GANG->gang_war_setting_cfg().kill_score + gri.hold_spot * CONFIG_CACHE_GANG->gang_war_setting_cfg().hold_score;
	ganger_rank_info[gri.role_id].role_id = gri.role_id;
	ganger_rank_info[gri.role_id].gang_id = gri.gang_id;
	ganger_rank_info[gri.role_id].role_name = gri.role_name;
	ganger_rank_info[gri.role_id].kill_num += gri.kill_num;
	ganger_rank_info[gri.role_id].die_num += gri.die_num;
	ganger_rank_info[gri.role_id].hold_spot += gri.hold_spot;
	ganger_rank_info[gri.role_id].headship = gri.headship;
	ganger_rank_info[gri.role_id].force = gri.force;
	ganger_rank_info[gri.role_id].gang_name = gri.gang_name;
	ganger_rank_info[gri.role_id].camp = gri.camp;
	return 0;
}
const int Gang_War_Info::FROM_WEEK = 1;
int Gang_War_Info::serialize(Block_Buffer &w) const {
	w.write_uint16(this->gang_war_matched_gangs_pair_vec_.size());
	for (GangWarMatchedGangsPair_Vec::const_iterator it = gang_war_matched_gangs_pair_vec_.begin();
			it != gang_war_matched_gangs_pair_vec_.end(); ++it) {
		it->match_time.serialize(w);
		w.write_int64(it->scene_index);
		w.write_int8(it->matched_index);
		w.write_int64(it->matched_pair.first);
		w.write_int64(it->matched_pair.second);
		w.write_int8(it->result);
	}
	w.write_uint16(gang_war_matched_gang_map_.size());
	for (GangWarMatchedGangInfo_Map::const_iterator it = gang_war_matched_gang_map_.begin();
			it != gang_war_matched_gang_map_.end(); ++it) {
		w.write_int64(it->second.scene_index);
		w.write_int64(it->second.matched_index);
		it->second.match_time.serialize(w);
		w.write_int8(it->second.camp);
		w.write_string(it->second.zone);
	}
	w.write_uint16(gang_war_spot_rank_map_.size());
	for (std::map<int8_t, std::vector<Gang_War_Rank_Info> >::const_iterator it = gang_war_spot_rank_map_.begin();
			it != gang_war_spot_rank_map_.end(); ++it) {
		w.write_int8(it->first);
		w.write_uint16(it->second.size());
		for (std::vector<Gang_War_Rank_Info>::const_iterator it_gwri = it->second.begin();
				it_gwri != it->second.end(); ++it_gwri) {
			it_gwri->serialize(w);
		}
	}
	w.write_uint16(gang_war_result_map_.size());
	for (std::map<int8_t, Gang_War_Spot>::const_iterator it = gang_war_result_map_.begin();
			it != gang_war_result_map_.end(); ++it) {
		it->second.serialize(w);
	}
	w.write_uint16(gang_rank_map.size());
	for (GangWarGangRankInfo_Map::const_iterator it = gang_rank_map.begin();
			it != gang_rank_map.end(); ++it) {
		it->second.serialize(w);
	}
	w.write_int8(stage_);
	w.write_int8(switch_);
	activity_time_.serialize(w);
	w.write_uint16(rank_award_.size());
	for (Gang_War_Rank_Award::const_iterator it = rank_award_.begin();
			it != rank_award_.end(); ++it) {
		it->second.serialize(w);
	}
	return 0;
}
int Gang_War_Info::deserialize(Block_Buffer &r) {
	uint16_t len = 0;
	r.read_uint16(len);
	Gang_War_Matched_Gangs_Pair gwmgp;
	for (uint16_t i = 0; i < len; ++i) {
		gwmgp.match_time.deserialize(r);
		r.read_int64(gwmgp.scene_index);
		r.read_int8(gwmgp.matched_index);
		r.read_int64(gwmgp.matched_pair.first);
		r.read_int64(gwmgp.matched_pair.second);
		r.read_int8(gwmgp.result);
		gang_war_matched_gangs_pair_vec_.push_back(gwmgp);
	}
	r.read_uint16(len);
	Gang_War_Matched_Gang_Info gwmgi;
	for (uint16_t i = 0; i < len; ++i) {
		gwmgi.reset();
		r.read_int64(gwmgi.scene_index);
		r.read_int64(gwmgi.matched_index);
		gwmgi.match_time.deserialize(r);
		r.read_int8(gwmgi.camp);
		r.read_string(gwmgi.zone);
		gang_war_matched_gang_map_.insert(std::make_pair(gwmgi.scene_index, gwmgi));
	}
	r.read_uint16(len);
	std::vector<Gang_War_Rank_Info> gwri_vec;
	int8_t matched_index = 0;
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int8(matched_index);
		gwri_vec.clear();
		uint16_t len_vec = 0;
		r.read_uint16(len_vec);
		for (uint16_t j = 0; j < len_vec; ++j) {
			Gang_War_Rank_Info gwri;
			gwri.deserialize(r);
			gwri_vec.push_back(gwri);
		}
		gang_war_spot_rank_map_.insert(std::make_pair(matched_index, gwri_vec));
	}
	r.read_uint16(len);
	Gang_War_Spot gws;
	for (uint16_t i = 0; i < len; ++i) {
		gws.reset();
		gws.deserialize(r);
		gang_war_result_map_.insert(std::make_pair(gws.index, gws));
	}

	Gang_War_Gang_Rank_Info gwgri;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		gwgri.reset();
		gwgri.deserialize(r);
		gang_rank_map.insert(std::make_pair(gwgri.gang_id, gwgri));
	}

	r.read_int8(stage_);
	r.read_int8(switch_);
	activity_time_.deserialize(r);
	r.read_uint16(len);
	Gang_War_Rank_Award_Detail gwrad;
	for (uint16_t i = 0; i < len; ++i) {
		gwrad.reset();
		gwrad.deserialize(r);
		rank_award_.insert(std::make_pair(gwrad.rank, gwrad));
		gang_id_rank_award_.insert(std::make_pair(gwrad.gang_id, gwrad));
	}
	return 0;
}
int Gang_War_Info::save(void) {
	set_activity_from();
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_GANG_WAR_INFO);
	this->serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, this->activity_time_.sec());
	return 0;
}
void Gang_War_Info::set_activity_from(void) {
//	Time_Value now(Time_Value::gettimeofday());
//	Date_Time date_now(now);
//	Date_Time date_des(date_now);
//	date_des.hour(0);
//	date_des.minute(0);
//	date_des.second(0);
//	date_des.microsec(0);
//
//	long this_week = date_des.weekday(); // date_des是星期几 , date_des.weekday() = [0-6];
//	if (this_week == 0) {
//		this_week = 7;
//	}
//	int split_day = 0; /*隔多少天*/
//	if (this_week > FROM_WEEK) {//指定星期在本周已经过去
//		split_day = - abs(this_week - FROM_WEEK);//Time_Value::ONE_WEEK_DAYS - (this_week - from_week);
//	} else if (this_week < FROM_WEEK) {
//		split_day = abs(this_week - FROM_WEEK);
//		MSG_USER("it must set problem");
//	}
//	if(split_day != 0) {
//		date_des = Date_Time(Time_Value(date_des.time_sec() + split_day * Time_Value::ONE_DAY_IN_SECS));
//	}

	activity_time_ = Time_Value::zero;//Time_Value(date_des.time_sec()) - Time_Value(Time_Value::ONE_WEEK_IN_SECS);
}
