/*
 * Scene_Team_Manager.cpp
 *
 *  Created on: 2014年6月6日
 *      Author: Linqiyou
 */


#include "team/Team_Struct.h"
#include "Scene_Team_Manager.h"
#include "Scene_Monitor.h"
#include "Scene_Player.h"


Scene_Team_Manager::Scene_Team_Manager(void) {
	reset();
}

Scene_Team_Manager::~Scene_Team_Manager(void) {
}

Map_Team *Scene_Team_Manager::get_team(int64_t team_id) {
	UInt_Map_Team_Map_Detail::iterator it = this->team_map_.find(team_id);
	if (it != this->team_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

int Scene_Team_Manager::add_team(Block_Buffer &buf) {
	int8_t operate = 0;
	int64_t team_id = 0;
	buf.read_int8(operate);
	buf.read_int64(team_id);
	if (Map_Team::SYNC_ADD == operate) {
//		if (this->team_map_.count(team_id)) {
//			this->del_team(team_id);
//		}
		Map_Team team;
		team.deserialize(buf);
		UInt_Map_Team_Map_Detail::iterator it = this->team_map_.find(team_id);
		if (it != this->team_map_.end()) {
			it->second.type() = team.type();
			it->second.words() = team.words();
			it->second.scene_id() = team.scene_id();
			it->second.degree() = team.degree();
			it->second.set_team_index(team.get_team_index());
			it->second.set_leader_role_id(team.leader_role_id());
			it->second.team_member_set().clear();
			for (Role_Hash_Set::const_iterator it_m = team.team_member_set().begin(); it_m != team.team_member_set().end(); ++it_m) {
				it->second.team_member_set().insert(*it_m);
			}
			it->second.offline_teamer_map().clear();
			for (Map_Team::RoleID_OnlineMap_TeamerInfo_Map::const_iterator it_o = team.offline_teamer_map().begin();
					it_o != team.offline_teamer_map().end(); ++it_o) {
				it->second.offline_teamer_map().insert(std::make_pair(it_o->first, Online_Teamer_Info()));
			}
			it->second.in_battle() = team.in_battle();
			it->second.lineup_replace(team.lineup());
			it->second.set_enter_level(team.enter_level());
			it->second.fail_times() = team.fail_times();
//			it->second.match_fail() = team.match_fail();
//			it->second = team;
		} else {
			this->team_map_.insert(std::make_pair(team_id, team));
			this->team_index_id_map_.insert(std::make_pair(team.get_team_index(), team.get_team_id()));
		}

		Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(team.leader_role_id());
		if (player) {
			int pos = team.get_position(team.leader_role_id());
			if (0 != pos) {
				player->set_team_position(0, pos);
			}
		}
		for (Role_Hash_Set::iterator it = team.team_member_set().begin(); it != team.team_member_set().end(); ++it) {
			Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
			if (player) {
				int pos = team.get_position(*it);
				if (0 != pos) {
					player->set_team_position(0, pos);
				}
			}
		}
	} else {
		UInt_Map_Team_Map_Detail::iterator it_team = this->team_map_.find(team_id);
		if (it_team != this->team_map_.end()) {
			Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(it_team->second.leader_role_id());
			if (player) {
				player->set_team_position(0, 0);
			}
			for (Role_Hash_Set::iterator it = it_team->second.team_member_set().begin(); it != it_team->second.team_member_set().end(); ++it) {
				Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
				if (player) {
					player->set_team_position(0, 0);
				}
			}
		}
		this->del_team(team_id);
	}
	return 0;
}
int Scene_Team_Manager::del_team(int64_t team_id) {
	UInt_Map_Team_Map_Detail::iterator it = this->team_map_.find(team_id);
	if (it != this->team_map_.end()) {
		this->team_index_id_map_.erase(it->second.get_team_index());
	}
	this->team_map_.erase(team_id);
	return 0;
}

void Scene_Team_Manager::reset(void) {
	this->team_map_.clear();
	this->team_index_id_map_.clear();
}
