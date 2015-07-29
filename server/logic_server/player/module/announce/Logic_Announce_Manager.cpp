/*
 * Logic_Announce_Manager.cpp
 *
 *  Created on: May 12, 2014
 *      Author: caijianwei
 */

#include "Logic_Announce_Manager.h"
#include "Msg_Inner_Struct.h"
#include "announcement/Announcement_Struct.h"
#include "Msg_Struct.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Logic_Player_Struct.h"


Logic_Announce_Manager::Logic_Announce_Manager() { }

Logic_Announce_Manager::~Logic_Announce_Manager() { }

void Logic_Announce_Manager::announce_send() {
	if ( district == 0 ) return ;
	MSG_20500000 msg;
	msg.district = district;
	msg.announce_id = announce_id;
	msg.gang_id = gang_id;
	msg.contents = ann_contents;
	msg.recievers = recievers;
	Block_Buffer buf__;
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf__);
}

void Logic_Announce_Manager::create_ann_active_content_count(const int32_t id) {
//	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_ACTIVE_CONTENT_COUNT);
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, id);
	//announce_push_text(name);
	announce_send();
}

void Logic_Announce_Manager::create_ann_success_protect_dragon_vale(const std::string server_name, const std::string &player_name,
																	int64_t role_id) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_PROTECT_SELF_DRAGON_VALE);
	announce_push_text(server_name);
	announce_push_text(player_name);
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);

	Role_Info role_info;

	if(player) {
		role_info.career = player->career();
		role_info.combat = 0;
		role_info.gender = player->base_detail().gender;
		role_info.level  = player->level();
		role_info.role_id = player->role_id();
		role_info.role_name = player->role_name();
		role_info.vip_level = player->vip();
		role_info.guild_name = player->base_detail().gang_name;
	}
	else {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			return;
		}

		if(!find_it->second) {
			return;
		}

		role_info.career = find_it->second->career;
		role_info.combat = 0;
		role_info.gender = find_it->second->gender;
		role_info.level = find_it->second->level;
		role_info.role_id = find_it->second->role_id;
		role_info.role_name = find_it->second->role_name;
		role_info.vip_level = find_it->second->vip();
		role_info.guild_name = find_it->second->gang_name;
	}

	announce_push_role(role_info);
	announce_send();
}

void Logic_Announce_Manager::create_ann_answer_rank_first(role_id_t role_id) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_ANSWER_RANK_FIRST);
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);

	Role_Info role_info;

	if(player) {
		role_info.career = player->career();
		role_info.combat = 0;
		role_info.gender = player->base_detail().gender;
		role_info.level  = player->level();
		role_info.role_id = player->role_id();
		role_info.role_name = player->role_name();
		role_info.vip_level = player->vip();
		role_info.guild_name = player->base_detail().gang_name;
	}
	else {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			return;
		}

		if(!find_it->second) {
			return;
		}

		role_info.career = find_it->second->career;
		role_info.combat = 0;
		role_info.gender = find_it->second->gender;
		role_info.level = find_it->second->level;
		role_info.role_id = find_it->second->role_id;
		role_info.role_name = find_it->second->role_name;
		role_info.vip_level = find_it->second->vip();
		role_info.guild_name = find_it->second->gang_name;
	}
	announce_push_role(role_info);

	announce_send();
}

void Logic_Announce_Manager::create_ann_gang_rank_change(const int rank, const gang_id_t gang_id) {
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_RANK_CHANGE, gang_id);
	std::stringstream ss;
	ss << rank;
	announce_push_text(ss.str());
	announce_send();
}

void Logic_Announce_Manager::create_ann_gang_fund_lack(const int day, const gang_id_t gang_id) {
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_FUND_LACK, gang_id);
	std::stringstream ss;
	ss << day;
	announce_push_text(ss.str());
	announce_send();
}
