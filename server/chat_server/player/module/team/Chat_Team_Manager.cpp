/*
 * Chat_Team_Manager.cpp
 *
 *  Created on: 2014年6月4日
 *      Author: Linqiyou
 */

#include "team/Team_Struct.h"
#include "Chat_Team_Manager.h"


Chat_Team_Manager::Chat_Team_Manager(void) {
	reset();
}

Chat_Team_Manager::~Chat_Team_Manager(void) {
}

Team *Chat_Team_Manager::get_team(int64_t team_id) {
	Int64_Team_Map_Detail::iterator it = this->team_map_.find(team_id);
	if (it != this->team_map_.end()) {
		return &(it->second);
	}
	return NULL;
}
int Chat_Team_Manager::add_team(Block_Buffer &buf) {
	int8_t operate = 0;
	int64_t team_id = 0;
	buf.read_int8(operate);
	buf.read_int64(team_id);
	if (Team::SYNC_ADD == operate) {
		if (this->team_map_.count(team_id)) {
			this->del_team(team_id);
		}
		Team team;
		team.deserialize(buf);
		this->team_map_.insert(std::make_pair(team_id, team));
		this->team_index_id_map_.insert(std::make_pair(team.get_team_index(), team.get_team_id()));
	} else {
		this->del_team(team_id);
	}
	return 0;
}
int Chat_Team_Manager::del_team(int64_t team_id) {
	Int64_Team_Map_Detail::iterator it = this->team_map_.find(team_id);
	if (it != this->team_map_.end()) {
		this->team_index_id_map_.erase(it->second.get_team_index());
	}
	this->team_map_.erase(team_id);
	return 0;
}

void Chat_Team_Manager::reset(void) {
	this->team_map_.clear();
	this->team_index_id_map_.clear();
}
