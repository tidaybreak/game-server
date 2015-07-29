/*
 * Ranking.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: zhouyajun
 */
#include "Logic_Module_Head.h"
#include "Ranking_Struct.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Rank.h"
#include "Msg_Ranking_Struct.h"
#include "Ranking_Manager.h"

Rank::Rank(void) {
	reset();
}

Rank::~Rank() {
	// TODO Auto-generated destructor stub
}

void Rank::reset(void) {
}
int Rank::client_ready_now(void) {
	RANKING_MANAGER->active_world_level(player_self());
	return 0;
}

int Rank::req_ranking_item_info(int32_t type, uint32_t rank_, role_id_t role_id, uint32_t career) {
	Ranking *rank = RANKING_MANAGER->get_rank_by_type(type);
	if(rank && rank->item_info_.size() && rank->item_info_.size() >= rank_) {
		MSG_50171002 msg = rank->item_info_[rank_-1];
		msg.roleId = role_id;
		msg.career = career;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Rank::req_ranking_role_info(int32_t type, uint32_t rank_) {
	Ranking *rank = RANKING_MANAGER->get_rank_by_type(type);
	if(rank && rank->role_info_.size() && rank->role_info_.size() >= rank_) {
		MSG_50171001 msg = rank->role_info_[rank_-1];
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Rank::req_ranking_heroer_info(int32_t type, uint32_t rank_) {
	Ranking *rank = RANKING_MANAGER->get_rank_by_type(type);
	if(rank && rank->heroer_info_.size() && rank->heroer_info_.size() >= rank_) {
		MSG_50171003 msg = rank->heroer_info_[rank_-1];
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Rank::req_ranking_info(int32_t type) {
	Ranking *rank = RANKING_MANAGER->get_rank_by_type(type);
	MSG_50171000 msg;
	msg.reset();
	msg.type = type;
	if(rank) {
		int i = 0;
		for(Ranking_Sort_List::iterator iter = rank->ranking_.begin(); iter != rank->ranking_.end(); ++iter) {
			iter->rank = ++i;
			msg.ranking.push_back(*iter);
		}
		msg.update_time = rank->update_time.sec();
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}
