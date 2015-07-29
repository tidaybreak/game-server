/*
 * Scene_Teamer.cpp
 *
 *  Created on: 2014年6月6日
 *      Author: Linqiyou
 */

#include "Scene_Teamer.h"
#include "Scene_Team_Manager.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Msg_Active_Struct.h"
#include "Move_Scene.h"
#include "Config_Cache_Team.h"
#include "Team_Arena_Controler.h"

Scene_Teamer::Scene_Teamer(void) {
	reset();
}

Scene_Teamer::~Scene_Teamer(void) {
}

void Scene_Teamer::reset(void) {
	this->teamer_detail_.reset();
	follow_ = 0;
	profile_set_ = 0;
	cumulate_award_.clear();
	kill_nums_ = 0;
}

Teamer_Detail &Scene_Teamer::teamer_detail(void) {
	return this->teamer_detail_;
}

bool Scene_Teamer::team_get_ready_status(void) {
	return this->teamer_detail_.ready;
}

int Scene_Teamer::team_fb_create_player_monster(void) {
	if (!player_self()->move_scene() || !player_self()->move_scene()->scene_config()
			|| Team_Fb_Scene_Type != player_self()->move_scene()->scene_config()->type) {
		return -1;
	}
	/*
	 * 第一次组队副本创建机器人
	 * */
	Map_Team* team = SCENE_TEAM_MANAGER->get_team(this->teamer_detail_.team_id);
	if (NULL != team) {
		if (team->type() == Map_Team::TEAM_TYPE_FB && !team->is_not_first_team_fb() && team->team_member_set().empty()) {
			team->team_fb_create_player_monster(*player_self());
		}
	}
	return 0;
}

bool Scene_Teamer::is_use_profile(void) {
	return profile_set_;
}
void Scene_Teamer::add_cumulate_info(const Int_Int_Map &award, const int kill_nums) {
	for (Int_Int_Map::const_iterator it = award.begin(); it != award.end(); ++it) {
		cumulate_award_[it->first] += it->second;
	}
	kill_nums_ += kill_nums;
	return;
}

Int_Int_Map &Scene_Teamer::cumulate_award(void) {
	return cumulate_award_;
}
int &Scene_Teamer::kill_nums(void) {
	return kill_nums_;
}
Time_Value &Scene_Teamer::start_time(void) {
	return start_time_;
}

void Scene_Teamer::notice_team_rank(void) {
	TEAM_ARENA_CONTROLER->refresh_rank_list(role_id());
}

int Scene_Teamer::syn_teamer_info(Block_Buffer &buf) {
	this->teamer_detail_.deserialize(buf);
	buf.read_int8(follow_);
	buf.read_int32(profile_set_);
	Map_Team *team = SCENE_TEAM_MANAGER->get_team(this->teamer_detail_.team_id);
	if (team) {
//		MSG_USER("has team %ld", this->teamer_detail_.team_id);
		if (Map_Team::TEAM_TYPE_SPORTS == team->type()) {
			if (0 == team->check_all_member_ready() && CONFIG_CACHE_TEAM->team_config().max_player == team->get_online_team_member_nums()) {
				if (Map_Team::TEAM_SPORTS_NORMAL == team->sports_status()) {
					team->begin_match();
				}
			} else {
				if (Map_Team::TEAM_SPORTS_SIGNED == team->sports_status()) {
					MSG_DEBUG("cancel_match");
					team->cancel_match();
				}
			}
		}
	}
	return 0;
}

int64_t Scene_Teamer::get_team_id() {
	return this->teamer_detail_.team_id;
}

int8_t &Scene_Teamer::follow(void) {
	return this->follow_;
}
