/*
 * Scene_Gang_Manager.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: Linqiyou
 */

#include "Scene_Gang_Manager.h"
#include "gang/Gang_Struct.h"
#include "Config_Cache.h"
#include "DB_Operator.h"
#include "Pool_Manager.h"
#include "Err_Code.h"
#include "Config_Cache_Gang.h"
#include "Msg_Gang_Struct.h"
#include "Msg_Active_Struct.h"
#include "Fighter_Detail.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Config_Cache_Mail.h"
#include "mail/Mail_Struct.h"
#include "Base_Detail.h"
#include "Msg_Inner_Enum.h"
#include "gang/Gang_Def.h"
#include "Pool_Manager.h"
#include "Global_Scene_Manager.h"
#include "Battle_Scene.h"
#include "Msg_Struct.h"
#include "gang_war_scene/Gang_War_Scene_Manager.h"
#include "Scene_Gang_Global_Manager.h"


Scene_Gang_Manager::Scene_Gang_Manager() {
	reset();
}

Scene_Gang_Manager::~Scene_Gang_Manager() { }

void Scene_Gang_Manager::reset() {
	gang_map_.clear();
	gangid_serverid_map_.clear();
	gangid_zone_map_.clear();
	server_ids_.clear();
	stage_ = 0;
	load_source_ = Time_Value::zero;

	switch_ = ACTIVITY_CLOSE;
	index_ = 0;
	last_match_time_ = Time_Value::zero;
	scene_index_gang_map_.clear();
	gang_index_map_.clear();
	gang_vec_.clear();

	rank_info_vec_.clear();
//	scene_index_ = 0;
}

Gang_DB_View* Scene_Gang_Manager::gang_view_by_id(int64_t gang_id) {
	Gang_Map::iterator iter = gang_map_.find( gang_id );
	return iter != gang_map_.end() ? &(iter->second):NULL;
}

void Scene_Gang_Manager::dismiss_gang(Gang_DB_View& gang_view) {
	gang_map_.erase( gang_view.gang_id );
}

Scene_Gang_Manager::Gang_Map::iterator Scene_Gang_Manager::dismiss_gang(Gang_Map::iterator &it_gang) {
	if (it_gang != gang_map_.end()) {
		return gang_map_.erase(it_gang);
	}
	return gang_map_.end();
}

std::string Scene_Gang_Manager::gang_name(int64_t gang_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( gang_view ) return gang_view->name;
	return "";
}

role_id_t Scene_Gang_Manager::gang_leader_id(int64_t gang_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( gang_view ) return gang_view->leader;
	return 0;
}

//bool Scene_Gang_Manager::has_member(int64_t gang_id, role_id_t role_id) {
//	Gang_DB_View* db_view = gang_view_by_id(gang_id);
//	if (db_view) {
//		return db_view->has_member(role_id);
//	}
//	return false;
//}

int Scene_Gang_Manager::get_gang_level(int64_t gang_id) {
	Gang_DB_View* db_view = gang_view_by_id(gang_id);
	return db_view ? db_view->level : 0;
}

void Scene_Gang_Manager::process_matched_gangs_pair(void) {
	if (stage_ == 2) {
		process_source_gangs_rank_16();
	} else {
		process_source_gangs_rank_other();
	}
}

void Scene_Gang_Manager::process_source_gangs_rank_16() {
	Role_Vec rank_vec;
	std::vector<Gang_DB_View_War_Rank> rank_info_vec;
	rank_vec.clear();
	Gang_DB_View_War_Rank rank_info;
	for (Gang_Map::const_iterator it_gang = gang_map_.begin(); it_gang != gang_map_.end(); ++it_gang) {
		rank_info.reset();
		rank_info.gang_id = it_gang->first;
		rank_info.level = it_gang->second.level;
		rank_info.member_size = it_gang->second.member_map.size();
		rank_info.fund = it_gang->second.fund;
		rank_info.force_total = it_gang->second.force_total;
		rank_info_vec.push_back(rank_info);
	}
	if (! rank_info_vec.empty()) {
		std::sort(rank_info_vec.begin(), rank_info_vec.end());
		std::reverse(rank_info_vec.begin(), rank_info_vec.end());

		for (size_t i = 0; i < rank_info_vec.size(); ++i) {
			Gang_Map::iterator find_it = gang_map_.find(rank_info_vec[i].gang_id);
			if (find_it != gang_map_.end()) {
				if (i >= GANG_WAR_ATTEND_NUMS) {
					break;
				}
				rank_vec.push_back(rank_info_vec[i].gang_id);
			}
		}
		process_source_gangs_rank_16_handle_matched(rank_vec);
	}
}

void Scene_Gang_Manager::process_source_gangs_rank_16_handle_matched(const Role_Vec &rank_info_vec) {
	Role_Vec rank_info_vec_bak = rank_info_vec;
	while(rank_info_vec_bak.size() > 0) {
		Role_Vec::iterator it_begin = rank_info_vec_bak.begin();
		Role_Vec::iterator it_end = rank_info_vec_bak.end();
		uint32_t size = rank_info_vec_bak.size();
		if (size < 2) {
			if (it_begin != it_end) {
				matched_handle(*it_begin, 0);
			}
			break;
		}
		uint32_t rand = std::rand() % size;
		Role_Vec::iterator first = it_begin;
		std::advance(first, rand);		// 随机抽中
		Role_Vec::iterator second = first;
		int dis_to_start = std::distance(it_begin, first);
		int dis_to_end = std::distance(first, it_end) - 1;
		int rand2 = std::rand() % 2;
		int advance_nums = 0;
		rand2 -= 1;
		if (rand2 == 0) {	// [0 - 1 to -1, 1] 除掉0
			rand2 = 1;
		}
		if (rand2 > 0) {
			advance_nums = std::min(rand2, dis_to_end);
			if (advance_nums == 0) {
				advance_nums = -1;
			}
		} else {
			advance_nums = std::min(-rand2, dis_to_start);
			if (advance_nums == 0) {
				advance_nums = 1;
			} else {
				advance_nums = -advance_nums;
			}
		}
		std::advance(second, advance_nums);
		int64_t team_id1 = *first;
		int64_t team_id2 = *second;
		matched_handle(team_id1, team_id2);
		for (int i = 0; i < 2; ++i) {
			for (Role_Vec::iterator it_team_ids = rank_info_vec_bak.begin(); it_team_ids != rank_info_vec_bak.end(); ++it_team_ids) {
				if (team_id1 == *it_team_ids || team_id2 == *it_team_ids) {
					rank_info_vec_bak.erase(it_team_ids);
					break;
				}
			}
		}
	}

	int8_t index = 0;
	for (Role_Vec::const_iterator it = rank_info_vec.begin(); it != rank_info_vec.end(); ++it) {
		GangWarMatchedGangInfo_Map::iterator it_gim = gang_index_map().find(*it);
		if (it_gim != gang_index_map().end()) {
			it_gim->second.rank = index;
		}
		++index;
	}
}

void Scene_Gang_Manager::process_source_gangs_rank_other(void) {
	for (std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_result_map_.begin(); it != gang_war_result_map_.end(); ++it) {
		if (it->second.gang_vec.size() == 2) {
			matched_handle(it->second);
		}
	}
}

int Scene_Gang_Manager::gang_war_load_source_gangs_from_logic(Block_Buffer &buf) {
	int8_t type = 0;
	buf.read_int8(type);
	if (type == 1) {
		int64_t server_id = 0;
		std::string zone;
		uint16_t len = 0;
		int8_t stage = 0;

		buf.read_int64(server_id);
		buf.read_string(zone);
		buf.read_int8(stage);
		if (stage != stage_
				|| load_source_ < Time_Value::gettimeofday()) {
			gang_map_.clear();
			gangid_serverid_map_.clear();
			gangid_zone_map_.clear();
			server_ids_.clear();
			stage_ = stage;
			load_source_ = Time_Value::gettimeofday() + Time_Value(Time_Value::ONE_MINUTE_IN_SECS * 5);
			MSG_USER("GANG_WAR: source_gangs server_id(%d) clear data",server_id);
		}
		server_ids_.insert(server_id);
		buf.read_uint16(len);
		Gang_War_Spot spot;
		std::map<int8_t, Gang_War_Spot> gang_war_result_map;
		for (uint16_t i = 0; i < len; ++i) {
			spot.reset();
			spot.deserialize(buf);
			gang_war_result_map.insert(std::make_pair(spot.index, spot));
		}
		if (!gang_war_result_map.empty()) {
			gang_war_result_map_.clear();
			gang_war_result_map_ = gang_war_result_map;
		}
		buf.read_uint16(len);
		Gang_DB_View info;
		for (uint16_t i = 0; i < len; ++i) {
			info.reset();
			info.gang_war_deserialize(buf);
			gang_map_.insert(std::make_pair(info.gang_id, info));
			gangid_serverid_map_.insert(std::make_pair(info.gang_id, server_id));
			gangid_zone_map_.insert(std::make_pair(info.gang_id, zone));
			SCENE_GANG_GLOBAL_MANAGER->add_gang(info.gang_id, this);
		}
		MSG_USER("GANG_WAR: source_gangs server_id(%d)",server_id);
	} else {
		role_id_t gang_id = 0;
		buf.read_int64(gang_id);
		gang_map_.erase(gang_id);
		gangid_serverid_map_.erase(gang_id);
		gangid_zone_map_.erase(gang_id);
		SCENE_GANG_GLOBAL_MANAGER->erase_gang(gang_id);
	}
	return 0;
}

int64_t Scene_Gang_Manager::find_server_id(const int64_t gang_id) {
	RoleID_RoleID_Map::const_iterator it = gangid_serverid_map_.find(gang_id);
	if (it != gangid_serverid_map_.end()) {
		return it->second;
	}
	return -1;
}

int Scene_Gang_Manager::sync_to_logic(Block_Buffer &buf) {
	Block_Buffer buf_bak;
	for (Role_Hash_Set::const_iterator it = server_ids_.begin(); it != server_ids_.end(); ++it) {
		const Monitor_Unique_ID* monitor_unique_id =
					SCENE_MONITOR->get_monitor_unique_id_by_uuid(*it, Server_Enum::MONITOR_LOGIC);
		if (!monitor_unique_id) {
			continue;
		}
		buf_bak.reset();
		buf_bak.copy(&buf);
		SCENE_MONITOR->send_to_logic(*it, buf_bak);
	}
	return 0;
}
int Scene_Gang_Manager::sync_to_logic(Block_Buffer &buf, const int64_t gang_id) {
	int64_t server_id = find_server_id(gang_id);
	if (server_id > 0) {
		const Monitor_Unique_ID* monitor_unique_id =
					SCENE_MONITOR->get_monitor_unique_id_by_uuid(server_id, Server_Enum::MONITOR_LOGIC);
		if (!monitor_unique_id) {
			return -1;
		}
		SCENE_MONITOR->send_to_logic(server_id, buf);
	}
	return 0;
}
std::string *Scene_Gang_Manager::find_zone(const int64_t gang_id) {
	RoleID_String_Map::iterator it = gangid_zone_map_.find(gang_id);
	if (it != gangid_zone_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

void Scene_Gang_Manager::tick(Time_Value &now) {
	if (ACTIVITY_CLOSE == this->switch_) {
		return ;
	}

	if (GANG_WAR_CREATE_16 == this->switch_) {
		if (stage() == 2) {
			index_ = 0;
		}
		match_arena_team();
		last_match_time_ = now;
		this->switch_ = GANG_WAR_CREATE_16_END;
	}

	if (ACTIVITY_READY == this->switch_) {
		match_arena_team();
		last_match_time_ = now;
		this->switch_ = ACTIVITY_MATCHED;
	}

	if (ACTIVITY_OPEN == this->switch_
			&& now - last_match_time_ > Time_Value(0)) {
		last_match_time_ = now;
		set_start_time(now);
		this->switch_ = ACTIVITY_FIGHT;
	}

}

int Scene_Gang_Manager::set_activity_state(Block_Buffer &buf) {
	int8_t swit = 0;
	buf.read_int8(swit);
	MSG_USER("GANG_WAR: state(%d)",swit);
	switch(swit) {
	case ACTIVITY_OPEN: {
		open_activity();
		break;
	}
	case ACTIVITY_CLOSE: {
		Role_Hash_Set scene_ids;
		for (SceneIndex_GangWarMatchedGangsPair_Map::const_iterator it = scene_index_gang_map_.begin();
				it != scene_index_gang_map_.end(); ++it) {
			scene_ids.insert(it->first);
		}
		GANG_WAR_SCENE_MANAGER->close_all_gang_war_scene(scene_ids);
		close_activity();
		break;
	}
	case GANG_WAR_CREATE_16: {
		gang_war_create_16();
		break;
	}
	case ACTIVITY_READY: {
		ready_activity();
		break;
	}
	default:
		break;
	}
	return 0;
}

int Scene_Gang_Manager::gang_war_create_16(void) {
	if (this->switch_ == ACTIVITY_CLOSE) {
		this->switch_ = GANG_WAR_CREATE_16;
		if (stage() == 2) {
			index_ = 0;
		}
	}
	return 0;
}

int Scene_Gang_Manager::ready_activity(void) {
	if (this->switch_ == ACTIVITY_CLOSE) {
		this->switch_ = ACTIVITY_READY;
		if (stage() == 2) {
			index_ = 0;
		}
	}
	return 0;
}

int Scene_Gang_Manager::open_activity(void) {
	if (this->switch_ == ACTIVITY_MATCHED) {
		this->switch_ = ACTIVITY_OPEN;
	}
	return 0;
}

int Scene_Gang_Manager::close_activity(void) {
	if (this->switch_ != ACTIVITY_CLOSE) {
		switch_ =  ACTIVITY_CLOSE;
		gang_vec_.clear();
		scene_index_gang_map_.clear();
		gang_index_map_.clear();
		rank_info_vec_.clear();
	}
	return 0;
}

bool Scene_Gang_Manager::is_open(void) {
	return ACTIVITY_CLOSE != this->switch_;
}
int Scene_Gang_Manager::set_start_time(Time_Value &now) {
	for (GangWarMatchedGangsPair_Vec::iterator it = gang_vec_.begin(); it != gang_vec_.end(); ++it) {
		it->match_time = now;
	}
	for (SceneIndex_GangWarMatchedGangsPair_Map::iterator it = scene_index_gang_map_.begin();
			it != scene_index_gang_map_.end(); ++it) {
		it->second.match_time = now;
	}

	for (GangWarMatchedGangInfo_Map::iterator it = gang_index_map_.begin();
			it != gang_index_map_.end(); ++it) {
		it->second.match_time = now;
	}
	sync_to_logic();
	return 0;
}

const Gang_War_Matched_Gang_Info *Scene_Gang_Manager::find_matched_gang_info(const int64_t gang_id) {
	if (!is_open()) {
		return NULL;
	}
	GangWarMatchedGangInfo_Map::iterator it = gang_index_map_.find(gang_id);
	if (it != gang_index_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

const Gang_War_Matched_Gangs_Pair *Scene_Gang_Manager::find_matched_gangs_pair(const int64_t scene_index) {
	if (!is_open()) {
		return NULL;
	}
	SceneIndex_GangWarMatchedGangsPair_Map::iterator it = scene_index_gang_map_.find(scene_index);
	if (it != scene_index_gang_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

int Scene_Gang_Manager::match_arena_team(void) {
	process_matched_gangs_pair();
	sync_to_logic();
	return 0;
}

// only for 16->8
int8_t Scene_Gang_Manager::make_index(void) {
	++this->index_;
	return this->index_;
}

//int64_t Scene_Gang_Manager::make_scene_index(void) {
//	return ++scene_index_;
//}

int Scene_Gang_Manager::matched_handle(int64_t gang_id1, int64_t gang_id2) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
//	dt.hour(20);
//	dt.minute(30);
	dt.second(0);
	dt.microsec(0);

	int8_t index = make_index() - 1;
	int64_t scene_index = SCENE_GANG_GLOBAL_MANAGER->make_scene_index();
	Gang_War_Matched_Gangs_Pair matched_gangs;
	matched_gangs.matched_pair.first = gang_id1;
	matched_gangs.matched_pair.second = gang_id2;
	matched_gangs.match_time.sec(dt.time_sec());
	matched_gangs.matched_index = index;
	matched_gangs.scene_index = scene_index;
	gang_vec_.push_back(matched_gangs);
	scene_index_gang_map_.insert(std::make_pair(scene_index, matched_gangs));

	Gang_War_Matched_Gang_Info info;
	info.matched_index = index;
	info.scene_index = scene_index;
	info.match_time = matched_gangs.match_time;
	info.camp = CAMP_RED;
	if (gang_id1 > 0) {
		info.force = 0;
		Gang_DB_View* gdv = gang_view_by_id(gang_id1);
		if (gdv) {
			info.force = gdv->force_total;
		}
		info.gang_name = gang_name(gang_id1);
		info.zone.clear();
		std::string *zone = find_zone(gang_id1);
		if (zone) {
			info.zone = *zone;
		}
		gang_index_map_.insert(std::make_pair(gang_id1, info));
	}
	info.camp = CAMP_BLUE;
	if (gang_id2 > 0) {
		info.force = 0;
		Gang_DB_View* gdv = gang_view_by_id(gang_id2);
		if (gdv) {
			info.force = gdv->force_total;
		}
		info.gang_name = gang_name(gang_id2);
		info.zone.clear();
		std::string *zone = find_zone(gang_id2);
		if (zone) {
			info.zone = *zone;
		}
		gang_index_map_.insert(std::make_pair(gang_id2, info));
	}
	return 0;
}

void Scene_Gang_Manager::matched_handle(Gang_War_Spot &spot) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
//	dt.hour(20);
//	dt.minute(30);
	dt.second(0);
	dt.microsec(0);

	int8_t index = spot.index;
	int64_t scene_index = SCENE_GANG_GLOBAL_MANAGER->make_scene_index();
	Gang_War_Matched_Gangs_Pair matched_gangs;
	for (std::vector<Gang_Basic_Detail>::iterator it = spot.gang_vec.begin(); it != spot.gang_vec.end(); ++it) {
		if (it->gang_id > 0) {
			if (it->index == 0) {
				matched_gangs.matched_pair.first = it->gang_id;
			} else if (it->index == 1) {
				matched_gangs.matched_pair.second = it->gang_id;
			}
		}
	}
	matched_gangs.match_time.sec(dt.time_sec());
	matched_gangs.matched_index = spot.index;
	matched_gangs.scene_index = scene_index;
	gang_vec_.push_back(matched_gangs);
	scene_index_gang_map_.insert(std::make_pair(scene_index, matched_gangs));

	Gang_War_Matched_Gang_Info info;
	info.matched_index = index;
	info.scene_index = scene_index;
	info.match_time = matched_gangs.match_time;
	info.camp = CAMP_RED;
	if (matched_gangs.matched_pair.first > 0) {
		info.force = 0;
		Gang_DB_View* gdv = gang_view_by_id(matched_gangs.matched_pair.first);
		if (gdv) {
			info.force = gdv->force_total;
		}
		info.gang_name = gang_name(matched_gangs.matched_pair.first);
		info.zone.clear();
		std::string *zone = find_zone(matched_gangs.matched_pair.first);
		if (zone) {
			info.zone = *zone;
		}
		gang_index_map_.insert(std::make_pair(matched_gangs.matched_pair.first, info));
	}
	info.camp = CAMP_BLUE;
	if (matched_gangs.matched_pair.second > 0) {
		info.force = 0;
		Gang_DB_View* gdv = gang_view_by_id(matched_gangs.matched_pair.second);
		if (gdv) {
			info.force = gdv->force_total;
		}
		info.gang_name = gang_name(matched_gangs.matched_pair.second);
		info.zone.clear();
		std::string *zone = find_zone(matched_gangs.matched_pair.second);
		if (zone) {
			info.zone = *zone;
		}
		gang_index_map_.insert(std::make_pair(matched_gangs.matched_pair.second, info));
	}
}
GangWarMatchedGangInfo_Map &Scene_Gang_Manager::gang_index_map(void) {
	return this->gang_index_map_;
}

int Scene_Gang_Manager::sync_to_logic(void) {
	MSG_USER("GANG_WAR:sync_to_logic, stage(%d), gang_vec_.size(%d)", stage(), gang_vec_.size());
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_GANG_WAR_INFO);
	buf.write_int8(stage());
	buf.write_uint16(gang_vec_.size());
	int64_t index = 0;
	int8_t camp = 0;
	Time_Value match_time = Time_Value::zero;
	int64_t scene_index = 0;
	std::string zone;
	std::string gang_name;
	int8_t rank = 0;
	int64_t force = 0;
	for (GangWarMatchedGangsPair_Vec::const_iterator it = gang_vec_.begin(); it != gang_vec_.end(); ++it) {
		buf.write_int64(it->matched_pair.first);
		{
			index = 0;
			camp = 0;
			match_time = Time_Value::zero;
			scene_index = 0;
			zone.clear();
			gang_name.clear();
			rank = 0;
			force = 0;
			const Gang_War_Matched_Gang_Info *it_index = find_matched_gang_info(it->matched_pair.first);
			if (it_index) {
				index = it_index->matched_index;
				camp = it_index->camp;
				match_time.set(it_index->match_time);
				scene_index = it_index->scene_index;
				zone = it_index->zone;
				gang_name = it_index->gang_name;
				rank = it_index->rank;
				force = it_index->force;
			}
		}
		buf.write_int64(index);
		buf.write_int8(camp);
		match_time.serialize(buf);
		buf.write_int64(scene_index);
		buf.write_string(zone);
		buf.write_string(gang_name);
		buf.write_int8(rank);
		buf.write_int64(force);

		buf.write_int64(it->matched_pair.second);
		{
			index = 0;
			camp = 0;
			match_time = Time_Value::zero;
			scene_index = 0;
			zone.clear();
			gang_name.clear();
			rank = 0;
			force = 0;
			const Gang_War_Matched_Gang_Info *it_index = find_matched_gang_info(it->matched_pair.second);
			if (it_index) {
				index = it_index->matched_index;
				camp = it_index->camp;
				match_time.set(it_index->match_time);
				scene_index = it_index->scene_index;
				zone = it_index->zone;
				gang_name = it_index->gang_name;
				rank = it_index->rank;
				force = it_index->force;
			}
		}
		buf.write_int64(index);
		buf.write_int8(camp);
		match_time.serialize(buf);
		buf.write_int64(scene_index);
		buf.write_string(zone);
		buf.write_string(gang_name);
		buf.write_int8(rank);
		buf.write_int64(force);

		it->match_time.serialize(buf);
		buf.write_int8(it->result);
		buf.write_int8(it->matched_index);
		buf.write_int64(it->scene_index);
	}
	buf.finish_message();
	sync_to_logic(buf);
	return 0;
}
