/*
 * Gang_Manager.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: caijianwei
 */

#include "Gang_Manager.h"
//#include "Gang_Struct.h"
#include "Config_Cache.h"
#include "DB_Operator.h"
#include "Pool_Manager.h"
#include "Err_Code.h"
#include "Config_Cache_Gang.h"
#include "Msg_Gang_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_World_Boss_Struct.h"
#include "Fighter_Detail.h"
#include "Logic_Player_Struct.h"
#include "Logic_Player.h"
#include "Logic_Monitor.h"
#include "Logic_Public.h"
#include "Config_Cache_Mail.h"
#include "mail/Mail_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Struct.h"
#include "gang/Gang_War_Def.h"
#include "Config_Cache_Scene.h"
#include "announce/Logic_Announce.h"
#include "Config_Cache_Item.h"
#include "announce/Logic_Announce_Manager.h"
#include "title/Title_Manager.h"
#include "expedition/Expedition_Manager.h"

Gang_Manager::Gang_Manager() : SAVE_INTER(Time_Value(10)) {
	reset();
}

Gang_Manager::~Gang_Manager() { }

void Gang_Manager::start() {
	const Server_List_Map &list = CONFIG_CACHE->server_list_map();
	for (Server_List_Map::const_iterator it = list.begin(); it != list.end(); ++it) {
		int64_t agent_l = it->first.agent_num * 10000000000000L;
		int64_t server_l = it->first.server_num * 1000000000L;
		server_max_gang_map.insert(std::make_pair(it->first, agent_l + server_l + 0));
	}
	MONGO_OPERATOR->load_gang_db_view();

	gang_init();

	gang_war_info_.set_activity_from();
	MONGO_OPERATOR->load_gang_war_info(gang_war_info_);
}

void Gang_Manager::add_gang_db_view(Gang_DB_View* db_view) {
	if ( !db_view ) return;

	if ( db_view->status == Gang_DB_View::GANG_STAT_NORMAL ) {
		if ( !gang_id_map.insert(std::make_pair(db_view->gang_id, db_view)).second ) {
			MSG_USER("add normal gang failure, exists gang_id:%ld", db_view->gang_id);
		}

		if ( !gang_name_map.insert(std::make_pair(db_view->name, db_view)).second ) {
			MSG_USER("add normal gang failure, exists gang_name:%s", db_view->name.c_str());
		}
	}
	else if ( !dismiss_gang_id_map.insert(std::make_pair(db_view->gang_id, db_view)).second ){
		MSG_USER("add dismiss %d gang failure, exists gang_id:%ld", db_view->status, db_view->gang_id);
	}

	Server_Max_Gang_Map::iterator s_find_it = server_max_gang_map.find(Server_Key( db_view->server_info.agent_num, db_view->server_info.server_num));
	if ( s_find_it != server_max_gang_map.end() ) {
		if ( db_view->gang_id > s_find_it->second ) {
			s_find_it->second = db_view->gang_id;
		}
	} else {
		server_max_gang_map.insert(std::make_pair(Server_Key(db_view->server_info.agent_num, db_view->server_info.server_num), db_view->gang_id));
	}
}

void Gang_Manager::tick(Time_Value& now) {
	if ( now >= save_time ) {
		// 每次保存数量做个限制,以免内存占太多
		int save_max_once = 0;
		for ( Gang_ID_Map::const_iterator iter = gang_id_map.begin();
				iter != gang_id_map.end(); ++iter ) {
			if ( iter->second->is_change() && save_max_once++ <= 10) {
				iter->second->save();
				iter->second->change_view(false);
			}

			refresh_force(iter->second);	//TODO
			refresh_rank();				//TODO
		}

		for (Gang_ID_Map::const_iterator iter = dismiss_gang_id_map.begin();
				iter != dismiss_gang_id_map.end(); ++iter) {
			if (iter->second->is_change()) {
				iter->second->save();
			}
			iter->second->change_view(false);
		}

		daily_leader();

		save_time = next_save_time( now );
	}
	gang_war_tick(now);
	gang_boss_tick(now);
}

void Gang_Manager::save(void) {
	for ( Gang_ID_Map::const_iterator iter = gang_id_map.begin();
			iter != gang_id_map.end(); ++iter ) {
		if ( iter->second->is_change() ) {
			iter->second->save();
		}
		iter->second->change_view(false);
	}
}

void Gang_Manager::daily_refresh(void) {
	daily_fund();
	daily_leader();
	daily_boss();
	refresh_rank();
	gang_war_clear_assigned_award_list();
	close_activity();
}

void Gang_Manager::reset() {
	save_time = next_save_time( Time_Value::gettimeofday() );
	server_max_gang_map.clear();
	gang_id_map.clear();
	dismiss_gang_id_map.clear();
	gang_name_map.clear();
	row_update_listener.clear();
	ganger_invite_cd.clear();
	offline_ganger_detail_cache.clear();

	gang_war_info_.reset();
	gang_boss_join_flag_map_.clear();
}

bool Gang_Manager::validate_new_name(const std::string& name) {
	// TODO forbid words check
	// exists name check
	return gang_name_map.find(name) == gang_name_map.end();
}

Time_Value Gang_Manager::next_save_time(const Time_Value& now) {
	return now + SAVE_INTER;
}

int64_t Gang_Manager::create_gang(const Server_Key& server_info, role_id_t leader,
			int8_t logo, const std::string& name) {
	Gang_DB_View* new_gang = POOL_MANAGER->pop_gang_db_view();
	if ( !new_gang ) {
		MSG_DEBUG("pool pop Gang_DB_View error!");
		return 0;
	}
	const Gang_Config_Level& level_config = CONFIG_CACHE_GANG->base(1);
	new_gang->gang_id = make_id( server_info );
	if (! new_gang->gang_id) {
		MSG_DEBUG("create_gang make_id error: Server_Key.agent_num=%d,server_num=%d, server_max_gang_map.size=%ld",
				server_info.agent_num, server_info.server_num, server_max_gang_map.size());
		return -1;
	}

	new_gang->leader = leader;
	new_gang->last_leader_id = leader;
	new_gang->server_info = server_info;
	new_gang->logo = logo;
	new_gang->name = name;

	Gang_Member_Detail member_detail;
	member_detail.role_id = leader;
	member_detail.week_contribute = 0;
	member_detail.total_contribute = 0;
	member_detail.amplify_times = 0;
	member_detail.headship = GANG_HEADSHIP_LEADER;
	member_detail.authority = CONFIG_CACHE_GANG->power(GANG_HEADSHIP_LEADER).auth_to_bit();
	new_gang->member_map.insert(std::make_pair(member_detail.role_id, member_detail));

	new_gang->level = 1;
	new_gang->create_time = Time_Value::gettimeofday();
	new_gang->status = Gang_DB_View::GANG_STAT_NORMAL;
	new_gang->join_block = GANG_IN_NEED_AGREE;
	// configure maintain cost & max members
	new_gang->maintain_cost = level_config.cost;
	new_gang->max_members   = level_config.population;
	// TODO rank calculation
	new_gang->rank = gang_id_map.size() + 1;

	// 公会建筑
	building_init(new_gang);

	refresh_force(new_gang);

	// save & build relation
	new_gang->save();
	add_gang_db_view( new_gang );
	return new_gang->gang_id;
}

Gang_DB_View* Gang_Manager::gang_view_by_id(int64_t gang_id) {
	Gang_ID_Map::iterator iter = gang_id_map.find( gang_id );
	return iter != gang_id_map.end() ? iter->second:0;
}

int Gang_Manager::res_gang_info(int64_t gang_id, Logic_Player* client) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( ! gang_view ) return ERROR_GANG_NOT_EXIST;
	MSG_50170002 msg;
	make_gang_info( *gang_view, msg.gang_info );
	OBJ_SEND_TO_CLIENT(msg, (*client));
	return 0;
}

int Gang_Manager::res_gang_info_update(int64_t gang_id, Logic_Player* client) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( ! gang_view ) return ERROR_GANG_NOT_EXIST;
	MSG_81070003 msg;
	make_gang_info( *gang_view, msg.gang_info );
	OBJ_SEND_TO_CLIENT(msg, (*client));
	return 0;
}

int64_t Gang_Manager::make_id(const Server_Key& server_info) {
	Server_Max_Gang_Map::iterator s_find_it = server_max_gang_map.find( server_info );
	return s_find_it == server_max_gang_map.end() ? 0 : ++(s_find_it->second);
}

Player_DB_View* Gang_Manager::player_view_by_role_id(role_id_t role_id) {
	DB_View::ID_Player_View_Map::iterator find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find( role_id );
	if ( find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end() ) {
		return find_it->second;
	}
	return 0;
}

const Ganger_Detail& Gang_Manager::offline_ganger_detail(role_id_t role_id) {
	Gang_ID_Detail_Map::const_iterator iter = offline_ganger_detail_cache.find( role_id );
	if ( iter == offline_ganger_detail_cache.end() ) {
		Ganger_Detail* detail = POOL_MANAGER->pop_ganger_detail();
		detail->role_id = role_id;
		detail->load();
		offline_ganger_detail_cache.insert(std::make_pair(role_id, detail));
		return *detail;
	}
	return *(iter->second);
}

void Gang_Manager::clean_ganger_detail(void) {
	for( Gang_ID_Detail_Map::const_iterator c_iter = offline_ganger_detail_cache.begin();
			c_iter != offline_ganger_detail_cache.end(); ++c_iter ) {
		POOL_MANAGER->push_ganger_detail(c_iter->second);
	}
	offline_ganger_detail_cache.clear();
}

void Gang_Manager::res_gang_list(Logic_Player* client) {
	MSG_50170004 msg;
	Gang_Row_Info row_info;
	for( Gang_ID_Map::const_iterator c_iter = gang_id_map.begin();
			c_iter != gang_id_map.end(); ++c_iter ) {
		const Gang_DB_View& gang_view = *(c_iter->second);
		row_info.reset();
		make_gang_row_info( gang_view, row_info );
		if (! client->gang_id()) {
			row_info.apply_status = gang_view.has_apply_info(client->role_id()) ? GA_CANCEL : GA_APPLY;
		}
		msg.gang_infos.push_back( row_info );
	}
	OBJ_SEND_TO_CLIENT(msg, (*client));
}

int Gang_Manager::res_gang_list_action(Logic_Player* player, int64_t gang_id, int8_t action) {
	if (!player || !gang_id) {
		return -1;
	}
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) {
		return ERROR_GANG_NOT_EXIST;
	}

	switch (action) {
	case GA_APPLY: {
		if ( GANG_IN_ALLALLOW == gang_view->join_block || can_direct_in_gang(gang_id, player->role_id()) ) {
			int join_res = join_gang_direct(player, gang_view);
			if (join_res) {
				return join_res;
			}
		} else if (GANG_IN_NEED_AGREE == gang_view->join_block ) {
			//TODO add in gang affairs
			Gang_Apply_Info apply_info;
			apply_info.role_id = player->role_id();
			apply_info.role_name = player->role_name();
			apply_info.level = player->level();
			apply_info.gender = player->gender();
			apply_info.career = player->career();
			//apply_info.vip = client->vip();
			apply_info.combat_power = player->fighter_detail().fetch_fight_property(PT_FORCE);
			apply_info.time_sec = Time_Value::gettimeofday().sec();
			gang_view->apply_infos.push_back(apply_info);

			player->insert_apply_id(gang_view->gang_id);

			active_gang_notify(gang_view->gang_id, GNA_HAS_APPLY_TO_MANAGER, player->role_id(), player->role_name());
		}

		break;
	}

	case GA_CANCEL: {
		for (Gang_DB_View::Apply_Info_Vec::iterator it_apply = gang_view->apply_infos.begin();
				it_apply != gang_view->apply_infos.end(); ) {
			if (it_apply->role_id == player->role_id()) {
				it_apply = gang_view->apply_infos.erase(it_apply);
			} else {
				++it_apply;
			}
		}

		player->erase_apply_id(gang_view->gang_id);

		break;
	}
	default: {
		break;
	}
	}

//	MSG_50170019 res_msg;
//	res_msg.gang_id = gang_id;
//	res_msg.action = action;
//	OBJ_SEND_TO_CLIENT(res_msg, (*player));

	return 0;
}

int Gang_Manager::res_ganger_list(Logic_Player* client, int64_t gang_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	MSG_50170005 msg;
	msg.gang_id = gang_id;
	Ganger_Row_Info row_info;
	for (Gang_DB_View::Member_Map::const_iterator c_iter = gang_view->member_map.begin();
			c_iter != gang_view->member_map.end(); ++c_iter) {
		row_info.reset();
		make_ganger_row_info(c_iter->first, gang_id, row_info);
		msg.ganger_infos.push_back(row_info);
	}
	OBJ_SEND_TO_CLIENT(msg, (*client));
	return 0;
}

int Gang_Manager::res_gang_apply_list(Logic_Player* player, int64_t gang_id) {
	if (!player || !gang_id) {
		return -1;
	}
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) {
		return ERROR_GANG_NOT_EXIST;
	}
	MSG_50170018 msg;
	for (uint16_t i = 0; i < gang_view->apply_infos.size(); ++i) {
		msg.apply_list.push_back(gang_view->apply_infos[i]);
	}
	OBJ_SEND_TO_CLIENT(msg, (*player));

	return 0;
}

int Gang_Manager::res_gang_apply_list_action(Logic_Player* player, int64_t gang_id, int8_t action, role_id_t target_id) {
	Gang_DB_View* gang_view = gang_view_by_id(gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	if (! player) {
		return -1;
	}

	MSG_50170021 res_msg;
	res_msg.role_id = target_id;
	res_msg.action = action;

	switch (action) {
	case GA_AGREE: {
		Logic_Player* target_player = LOGIC_MONITOR->find_logic_player_by_role_id(target_id);
		if (! target_player) {
			return ERROR_PLAYER_NOT_ONLINE;
		}
		Gang_DB_View* gang_view = gang_view_by_id( gang_id );
		if ( !gang_view ) {
			return ERROR_GANG_NOT_EXIST;
		}
		const Gang_Config_Level& level_config = CONFIG_CACHE_GANG->base(gang_view->level);
		int member_space = level_config.population - gang_view->member_map.size();
		if (member_space < 1) {
			return ERROR_GANG_POPULATION_FULL;
		}
		int join_res = join_gang_direct(target_player, gang_view);
		if (join_res) {
			player->send_err_to_client(join_res, 0);
			if (join_res == ERROR_GANG_ALREADY_GANGER) {
				res_msg.done_roles.push_back(target_player->role_id());
				res_msg.result = 0;
			} else {
				//res_msg.result = 0;
			}
			//return join_res;
		} else {
			res_msg.done_roles.push_back(target_player->role_id());
			res_msg.result = 1;
			break;
		}
		//active_gang_notify(player->role_id(), GNA_BE_AGREE_TO_APPLYER, gang_view->gang_id, gang_view->name);
		break;
	}
	case GA_REFUSE: {
		Logic_Player* target_player = LOGIC_MONITOR->find_logic_player_by_role_id(target_id);
		if (! target_player) {
			return ERROR_PLAYER_NOT_ONLINE;
		}
		Gang_DB_View* gang_view = gang_view_by_id( gang_id );
		if ( !gang_view ) {
			return ERROR_GANG_NOT_EXIST;
		}
		gang_view->erase_apply_info(target_player->role_id());
		active_gang_notify(target_player->role_id(), GNA_BE_REFUSE_TO_APPLYER, gang_view->gang_id, gang_view->name);
		res_msg.done_roles.push_back(target_player->role_id());
		res_msg.result = 1;
		break;
	}
	case GA_AGREE_ALL: {
		Gang_DB_View* gang_view = gang_view_by_id(gang_id);
		if (! gang_view) {
			return ERROR_GANG_NOT_EXIST;
		}
		const Gang_Config_Level& level_config = CONFIG_CACHE_GANG->base(gang_view->level);
		int member_space = level_config.population - gang_view->member_map.size();
		if (member_space < 1) {
			return ERROR_GANG_POPULATION_FULL;
		}
		int apply_count = gang_view->apply_infos.size();
		size_t join_count = std::min(member_space, apply_count);
		for (uint16_t i = 0; i < join_count; ++i) {
			Logic_Player* target_player = LOGIC_MONITOR->find_logic_player_by_role_id(gang_view->apply_infos[i].role_id);
			if (target_player) {
				int join_res = join_gang_direct(target_player, gang_view);
				if (0 == join_res) {
					res_msg.done_roles.push_back(gang_view->apply_infos[i].role_id);
					res_msg.result = 1;
				}
			} else {
				res_msg.result = 3;
			}
		}

		if (!res_msg.result && (int)join_count < apply_count) {
			res_msg.result = 2;
		}

		break;
	}
	case GA_REFUSE_ALL: {
		Gang_DB_View* gang_view = gang_view_by_id(gang_id);
		if (! gang_view) {
			return ERROR_GANG_NOT_EXIST;
		}
		for (uint16_t i = 0; i < gang_view->apply_infos.size(); ++i) {
			res_msg.done_roles.push_back(gang_view->apply_infos[i].role_id);
			active_gang_notify(gang_view->apply_infos[i].role_id, GNA_BE_REFUSE_TO_APPLYER, gang_view->gang_id, gang_view->name);
		}
		gang_view->apply_infos.clear();
		res_msg.result = 1;
		break;
	}
	default: {
		break;
	}
	}

	OBJ_SEND_TO_CLIENT(res_msg, (*player));
	return 0;
}

int Gang_Manager::erase_all_gang_apply(Logic_Player* player) {
	for (Role_Id_Set::const_iterator it_apply_id = player->ganger_detail().apply_ids.begin();
			it_apply_id != player->ganger_detail().apply_ids.end(); ++it_apply_id) {
		Gang_DB_View* gang_view = gang_view_by_id(*it_apply_id);
		if (! gang_view) {
			continue;
		}
		gang_view->erase_apply_info(player->role_id());
	}

	return 0;
}

int Gang_Manager::join_gang(Logic_Player* client, int64_t gang_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	if ( GANG_IN_ALLALLOW == gang_view->join_block
		|| can_direct_in_gang(gang_id, client->role_id()) ) {

		client->approve_join_gang(gang_view);

		// gang_view change
		Gang_Member_Detail member_detail;
		member_detail.role_id = client->role_id();
		member_detail.headship = client->ganger_detail().headship;
		member_detail.authority = CONFIG_CACHE_GANG->power(GANG_HEADSHIP_LEADER).auth_to_bit();
		gang_view->member_map.insert(std::make_pair(member_detail.role_id, member_detail));
		remove_direct_in_gang(gang_id, client->role_id());
		refresh_force(gang_view);
		gang_view->change_view();

		// sync to local chat
		client->sync_ganger_info_to_chat();

		//TODO calculate ranks

		// notify gang/ganger info
		client->active_ganger_info_update();
		notify_gang_info_update(*gang_view);

		// notification gang row, ganger row,
		notify_gang_row_info( GANG_ROW_INFO_UPDATE, *gang_view);
		notify_ganger_row_info( GANG_ROW_INFO_ADD, client->role_id(), gang_id);

		// add gang passed event
		gang_view->add_passed_event_101900(gang_view->leader, client->role_id());
		gang_view->send_last_passed_event();
		// sync gang boss icon
		if(gang_view->boss_act_state == 2 || gang_view->boss_act == 3){// ready or open state
			Time_Value now = Time_Value::gettimeofday();
			int remain_time = gang_view->boss_act_next_time.sec() - Time_Value::gettimeofday().sec();
			if(remain_time < 0){
				remain_time = 0;
			}
			client->send_gang_boss_active_to_client(gang_view->boss_act_state, remain_time);
		}

	} else if (GANG_IN_NEED_AGREE == gang_view->join_block ) {
		//TODO add in gang affairs

	}

	return 0;
}

int Gang_Manager::join_gang_direct(Logic_Player* player, Gang_DB_View* gang_view) {
	//Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) {
		return ERROR_GANG_NOT_EXIST;
	}
	const Gang_Config_Level& level_config = CONFIG_CACHE_GANG->base(gang_view->level);
	if (level_config.population <= gang_view->member_map.size()) {
		return ERROR_GANG_POPULATION_FULL;
	}
	if (gang_view->has_member(player->role_id()) || player->ganger_detail().gang_id) {
		return ERROR_GANG_ALREADY_GANGER;
	}

	player->approve_join_gang(gang_view);

	// gang_view change
	Gang_Member_Detail member_detail;
	member_detail.role_id = player->role_id();
	member_detail.headship = player->ganger_detail().headship;
	member_detail.authority = CONFIG_CACHE_GANG->power(GANG_HEADSHIP_LEADER).auth_to_bit();
	gang_view->member_map.insert(std::make_pair(member_detail.role_id, member_detail));
	remove_direct_in_gang(gang_view->gang_id, player->role_id());
	refresh_force(gang_view);
	gang_view->change_view();

	// sync to local chat
	player->sync_ganger_info_to_chat();

	//TODO calculate ranks

	// notify gang/ganger info
	player->active_ganger_info_update();
	notify_gang_info_update(*gang_view);

	// notification gang row, ganger row,
	notify_gang_row_info( GANG_ROW_INFO_UPDATE, *gang_view);
	notify_ganger_row_info( GANG_ROW_INFO_ADD, player->role_id(), gang_view->gang_id);
	player->req_ganger_list(gang_view->gang_id);	// 前端要求发全部ganger_row

	// add gang passed event
	gang_view->add_passed_event_101900(gang_view->leader, player->role_id());
	gang_view->send_last_passed_event();
	player->create_ann_gang_join_in(player->role_id());

	active_gang_notify(player->role_id(), GNA_BE_AGREE_TO_APPLYER, gang_view->gang_id, gang_view->name);

	return 0;
}

void Gang_Manager::add_gang_info_listener(role_id_t role_id) {
	row_update_listener.insert( role_id );
}

void Gang_Manager::remove_gang_info_listener(role_id_t role_id) {
	row_update_listener.erase( role_id );
}

int Gang_Manager::quit_gang(Logic_Player* client, int64_t gang_id) {
	if (! client) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	if ( client->role_id() != gang_view->leader ) {
		// sync gang boss icon
		if(gang_view->boss_act_state == 2 || gang_view->boss_act_state == 3){
			client->send_gang_boss_active_to_client(1, 0);
		}
		// remove member
		gang_view->member_map.erase( client->role_id() );
		refresh_force(gang_view);
		// remove quit gang rank info
		GangWarGangRankInfo_Map::iterator it = gang_war_info_.gang_rank_map.find(gang_id);
		if (it != gang_war_info_.gang_rank_map.end()) {
			Gang_War_Rank_Info gwri;
			for (Ganer_Rank_Info::iterator it_gri = it->second.ganger_rank_info.begin(); it_gri != it->second.ganger_rank_info.end(); ++it_gri) {
				if(client->role_id() == it_gri->second.role_id) {
					it->second.ganger_rank_info.erase(it_gri);
					break;
				}
			}
		}
		// TODO calculate rank
		notify_gang_info_update(*gang_view);
		// notify gang row, ganger row
		notify_gang_row_info( GANG_ROW_INFO_UPDATE, *gang_view );
		notify_ganger_row_info( GANG_ROW_INFO_REMOVE, client->role_id(), gang_id);
		// add in passed event
		gang_view->add_passed_event_101902(client->role_id());
		gang_view->send_last_passed_event();
		client->create_ann_gang_quit(client->role_id(), gang_id);
	} else {
		if ( gang_view->member_map.size() > 1 ) {
			return ERROR_GANG_NOT_RESIGN;
		}
		// sync gang boss icon
		if(gang_view->boss_act_state == 2 || gang_view->boss_act_state == 3){
			client->send_gang_boss_active_to_client(1, 0);
		}
		// gang dismiss, cleaning jobs ?
		dismiss_gang(*gang_view);
		// notify gang row
		notify_gang_row_info( GANG_ROW_INFO_REMOVE, *gang_view );
	}
	gang_view->change_view();
	return 0;
}

int Gang_Manager::resign_to(Logic_Player* client, int64_t gang_id, role_id_t new_leader) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) {
		return ERROR_GANG_NOT_EXIST;
	}
	if (! client) {
		return ERROR_PLAYER_NOT_ONLINE;
	}

	Player_DB_View* player_view = player_view_by_role_id( new_leader );
	if ( !player_view || gang_view->member_map.find(new_leader) == gang_view->member_map.end() ) {
		return ERROR_GANG_NO_MEMBER; // player not exists ?
	}

	//change new leader ganger detail
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id( new_leader );
	if ( player ) { // online
		player->promote_to_leader();
		GANG_MANAGER->update_member_info(gang_view, player->role_id());
		player->active_ganger_info_update();
	} else {
		// change off line detail
		return ERROR_PLAYER_NOT_ONLINE;
	}
	//change leader
	gang_view->last_leader_id = gang_view->leader;
	gang_view->leader = new_leader;

	// change client
	if (client) {
		client->promote_to_headship(GANG_HEADSHIP_NORMAL);
		GANG_MANAGER->update_member_info(gang_view, client->role_id());
		client->active_ganger_info_update();
	}
	EXPEDITION_MANAGER->listen_gang_leader_change(player->base_detail());

	notify_gang_info_update(*gang_view);
	notify_gang_row_info(GANG_ROW_INFO_UPDATE, *gang_view);
	notify_ganger_row_info(GANG_ROW_INFO_UPDATE, player->role_id(), gang_id);
	notify_ganger_row_info(GANG_ROW_INFO_UPDATE, client->role_id(), gang_id);

	// add_passed_event
	gang_view->add_passed_event_101906(client->role_id(), player->role_id());
	gang_view->send_last_passed_event();

	client->create_ann_gang_resign_to(client->role_id(), new_leader);

	return 0;
}

void Gang_Manager::dismiss_gang(Gang_DB_View& gang_view, bool kick_ganger) {
	if (kick_ganger) {
		for (Gang_DB_View::Member_Map::iterator it_member = gang_view.member_map.begin();
				it_member != gang_view.member_map.end(); ++it_member) {
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(it_member->first);
			if (player) {
				player->kicked_out_gang(gang_view.gang_id);
			}
		}
	}

	gang_view.status = Gang_DB_View::GANG_STAT_DISMISS;
	gang_view.dismiss_time = Time_Value::gettimeofday();
	gang_view.affairs.clear();
	gang_view.change_view();
	// move to dismiss gang map
	if ( !dismiss_gang_id_map.insert(std::make_pair(gang_view.gang_id, &gang_view)).second ) {
		MSG_USER("add dismiss %d gang failure, exists gang_id:%ld", gang_view.status, gang_view.gang_id);
	}
	gang_name_map.erase(gang_name(gang_view.gang_id));

	Block_Buffer buf;
	buf.make_message(INNER_SYNC_GANG_WAR_INFO);
	buf.write_int32(CONFIG_CACHE->server_maintainer_cache().game_inter_flag);
	buf.write_int8(0);
	buf.write_int64(gang_view.gang_id);
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(GANG_WAR_SCENE_ID, 0, buf);

	EXPEDITION_MANAGER->send_del_gang_info_to_scene(gang_view.gang_id);
	gang_id_map.erase( gang_view.gang_id );
}

Gang_Manager::Gang_ID_Map::iterator Gang_Manager::dismiss_gang(Gang_ID_Map::iterator &it_gang, bool kick_ganger) {
	if (it_gang != gang_id_map.end()) {
		if (kick_ganger) {
			for (Gang_DB_View::Member_Map::iterator it_member = it_gang->second->member_map.begin();
					it_member != it_gang->second->member_map.end(); ++it_member) {
				Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(it_member->first);
				if (player) {
					player->kicked_out_gang(it_gang->first);
				}
			}
		}

		it_gang->second->status = Gang_DB_View::GANG_STAT_DISMISS;
		it_gang->second->dismiss_time = Time_Value::gettimeofday();
		it_gang->second->affairs.clear();
		it_gang->second->change_view();
		// move to dismiss gang map
		if ( !dismiss_gang_id_map.insert(std::make_pair(it_gang->second->gang_id, it_gang->second)).second ) {
			MSG_USER("add dismiss %d gang failure, exists gang_id:%ld", it_gang->second->status, it_gang->second->gang_id);
		}
		gang_name_map.erase(gang_name(it_gang->second->gang_id));
		EXPEDITION_MANAGER->send_del_gang_info_to_scene(it_gang->second->gang_id);
		return gang_id_map.erase(it_gang);
	}
	return gang_id_map.end();
}

int Gang_Manager::set_gang_annouce(int64_t gang_id,	const std::string& announce) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	if ( gang_view->cd_annouce > Time_Value::gettimeofday() ) return ERROR_GANG_OPER_IN_CD;
	gang_view->announcement = announce;
	gang_view->change_view();
	gang_view->cd_annouce = Time_Value::gettimeofday() + CONFIG_CACHE_GANG->limit().change_noticecd;
	return 0;
}

bool Gang_Manager::invite_friend_cd_ok(role_id_t role_id) {
	Role_Id_Time_Map::iterator iter = ganger_invite_cd.find( role_id );
	return iter == ganger_invite_cd.end() || iter->second <= Time_Value::gettimeofday();
}

void Gang_Manager::update_invite_friend_cd(role_id_t role_id) {
	// TODO config cd
	Time_Value next_cd = Time_Value::gettimeofday() + CONFIG_CACHE_GANG->limit().cd_friendset;
	std::pair<Role_Id_Time_Map::iterator, bool> res =
			ganger_invite_cd.insert(std::make_pair(role_id, next_cd));
	if ( !res.second ) res.first->second = next_cd;
}

void Gang_Manager::add_direct_in_gang(int64_t gang_id, role_id_t role_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return ;
	gang_view->direct_in.insert( role_id );
}

void Gang_Manager::remove_direct_in_gang(int64_t gang_id, role_id_t role_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return ;
	gang_view->direct_in.erase( role_id );
}

bool Gang_Manager::can_direct_in_gang(int64_t gang_id, role_id_t role_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return false;
	return gang_view->direct_in.find( role_id ) != gang_view->direct_in.end();
}

void Gang_Manager::remove_offline_ganger_detail(int64_t role_id) {
	Gang_ID_Detail_Map::iterator iter = offline_ganger_detail_cache.find( role_id );
	if ( iter != offline_ganger_detail_cache.end() ) {
		POOL_MANAGER->push_ganger_detail( iter->second );
		offline_ganger_detail_cache.erase( iter );
	}
}

void Gang_Manager::cache_offline_ganger_detail(const Ganger_Detail& detail) {
	remove_offline_ganger_detail( detail.role_id );
	Ganger_Detail* copy_detail = POOL_MANAGER->pop_ganger_detail();
	*copy_detail = detail;
	offline_ganger_detail_cache.insert( std::make_pair(copy_detail->role_id, copy_detail) );
}

std::string Gang_Manager::gang_name(int64_t gang_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( gang_view ) return gang_view->name;
	return "";
}

void Gang_Manager::notify_gang_info_update(Gang_DB_View const& gang_view) {
	MSG_81070003 msg;
	make_gang_info( gang_view, msg.gang_info );
	for ( Gang_DB_View::Member_Map::const_iterator iter=gang_view.member_map.begin();
			iter != gang_view.member_map.end(); ++iter ) {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(iter->first);
		if ( !player ) continue;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}
}

void Gang_Manager::notify_gang_info_update(int64_t gang_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return;
	notify_gang_info_update(*gang_view);
}

void Gang_Manager::notify_gang_row_info(int8_t oper, const Gang_DB_View& gang_view) {
	if ( row_update_listener.empty() ) return;
	MSG_81070000 msg;
	msg.operate = oper;
	if ( oper == GANG_ROW_INFO_REMOVE ) {
		msg.row_info.gang_id = gang_view.gang_id;
		msg.row_info.gang_name = gang_view.name;
	} else {
		make_gang_row_info(gang_view, msg.row_info );
	}

	for ( Role_Hash_Set::iterator iter = row_update_listener.begin();
			iter != row_update_listener.end(); ++iter ) {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id( *iter );
		if ( !player ) continue;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}
}

void Gang_Manager::notify_gang_row_info(int8_t oper, int64_t gang_id) {
	Gang_DB_View* gang_view = gang_view_by_id( gang_id );
	if ( !gang_view ) return ;
	notify_gang_row_info( oper, *gang_view );
}

void Gang_Manager::notify_ganger_row_info(int8_t oper, role_id_t role_id, role_id_t gang_id, bool sign_out) {
	if ( row_update_listener.empty() ) return;
	MSG_81070001 msg;
	msg.operate = oper;
	if ( oper == GANG_ROW_INFO_REMOVE ) {
		msg.row_info.role_id = role_id;
	} else {
		make_ganger_row_info(role_id, gang_id, msg.row_info, sign_out);
	}

	for ( Role_Hash_Set::iterator iter = row_update_listener.begin();
			iter != row_update_listener.end(); ++iter ) {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id( *iter );
		if ( !player || player->gang_id() != gang_id) {
			continue;
		}
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}
}

void Gang_Manager::make_gang_info(Gang_DB_View const& gang_view, Gang_Info& info) {
	const Gang_Config_Level& level_config = CONFIG_CACHE_GANG->base( gang_view.level );
	info.gang_id = gang_view.gang_id;
	info.leader = gang_view.leader;
	info.fund = gang_view.fund;
	info.maintain_cost = level_config.cost;
	info.cd_upgrade = gang_view.cd_upgrade.sec();
	info.level = gang_view.level;
	info.cur_members = gang_view.member_map.size();
	info.max_members = level_config.population;
	info.rank = gang_view.rank;
	info.logo = gang_view.logo;
	info.join_block = gang_view.join_block;
	Player_DB_View* player = player_view_by_role_id( info.leader );
	if ( player ) info.leader_name = player->role_name;
	info.gang_name = gang_view.name;
	info.announce = gang_view.announcement;
	info.can_seize_power = gang_view.can_seize_power;
	info.force = gang_view.force_total;

	std::map<int, int> type_max_id;
	for (Gang_Skill_Map::const_iterator it = gang_view.researched_skills.begin();
			it != gang_view.researched_skills.end(); ++it) {
		int skill_id = it->first;
		const Gang_Cfg_Skill_Id *cfg_skill = CONFIG_CACHE_GANG->find_skill(skill_id);
		if (! cfg_skill) {
			continue;
		}

		if (! type_max_id.count(cfg_skill->skill_type)) {
			type_max_id[cfg_skill->skill_type] = skill_id;
		} else {
			const Gang_Cfg_Skill_Id *cfg_skill_old = CONFIG_CACHE_GANG->find_skill(type_max_id[cfg_skill->skill_type]);
			if (! cfg_skill_old) {
				continue;
			}
			if (cfg_skill->skill_lvl > cfg_skill_old->skill_lvl) {
				type_max_id[cfg_skill->skill_type] = skill_id;
			}
		}
	}

	Gang_Skill_Info skill_info;
	for (std::map<int, int>::const_iterator it = type_max_id.begin();
			it != type_max_id.end(); ++it) {
		skill_info.reset();
		skill_info.skill_id = it->second;
		info.researched_skills.push_back(skill_info);
	}

//	Gang_Skill_Info skill_info;
//	for (Gang_Skill_Map::const_iterator it = gang_view.researched_skills.begin();
//			it != gang_view.researched_skills.end(); ++it) {
//		skill_info.reset();
//		skill_info.skill_id = it->second.skill_id;
//		info.researched_skills.push_back(skill_info);
//	}

	Gang_Building_Info building_info;
	for (Gang_Building_Map::const_iterator it = gang_view.buildings.begin();
			it != gang_view.buildings.end(); ++it) {
		building_info.reset();
		building_info.building_id = it->second.building_id;
		building_info.building_lvl = it->second.building_lvl;
		building_info.upgrade_cd = it->second.cd_upgrade.sec();
		info.building_infos.push_back(building_info);
	}
}

void Gang_Manager::make_gang_row_info(const Gang_DB_View& gang_view, Gang_Row_Info& info) {
	const Gang_Config_Level& level_config = CONFIG_CACHE_GANG->base( gang_view.level );
	info.gang_id = gang_view.gang_id;
	info.rank	 = gang_view.rank;
	info.level	 = gang_view.level;
	info.cur_members = gang_view.member_map.size();
	info.max_members = level_config.population;
	info.logo    = gang_view.logo;
	info.gang_name = gang_view.name;
	info.join_block = gang_view.join_block;
	Player_DB_View* player_view = player_view_by_role_id( gang_view.leader );
	if ( player_view ) {
		info.leader_info.role_id = player_view->role_id;
		info.leader_info.role_name = player_view->role_name;
		info.leader_info.head_photo_id = player_view->friend_.head_photo_id;
		info.leader_info.gender = player_view->gender;
		info.leader_info.guild_name = player_view->gang_name;
		info.leader_info.career = player_view->career;
		info.leader_info.combat = player_view->force;
		info.leader_info.level  = player_view->level;
		info.leader_info.vip_level = player_view->vip();
		info.leader_info.guild_name = player_view->gang_name;
		info.leader_info.title_id = player_view->title_id;
	}
	info.apply_status = GA_APPLY;
}

void Gang_Manager::make_ganger_row_info(role_id_t role_id, role_id_t gang_id, Ganger_Row_Info& info, bool sign_out) {
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id( role_id );
	info.role_id = role_id;
	if ( player ) {
		info.week_contribute = player->ganger_detail().contrib_of_week;
		info.total_contribute = player->ganger_detail().total_contribute;
		info.headship = player->ganger_detail().headship;
		Gang_Config_Authority::bit_auth_to_vec(player->ganger_detail().authority, info.authority);
		// TODO amplify times
		info.amplify_times = 0;
		info.fight = player->fighter_detail().ttl_force;
		info.level = player->base_detail().level;
		info.vip   = player->vip();
		info.career= player->base_detail().career;
		info.gender= player->base_detail().gender;
		info.ganger_name = player->base_detail().role_name;
		// TODO online state
		info.line_stat = sign_out ?  0 : 1;
		info.head_photo_id = player->friend_detail().head_photo_id;
		info.is_zhizunbao = player->is_dragon_vale_special();
		info.last_sign_out_time = player->base_detail().last_sign_out_time.sec();
	} else {
		Player_DB_View* player_view = player_view_by_role_id( role_id );
		if ( player_view ) {
			info.level = player_view->level;
			info.vip   = player_view->vip();
			info.career= player_view->career;
			info.gender= player_view->gender;
			info.ganger_name = player_view->role_name;
			info.line_stat = 0;
			info.head_photo_id = player_view->friend_.head_photo_id;
			info.fight = player_view->ttl_force;
			info.is_zhizunbao = player_view->is_dragon_vale_special();
			info.last_sign_out_time = player_view->last_sign_out_time.sec();
		}
		//const Ganger_Detail& detail = offline_ganger_detail( role_id );
		Gang_DB_View* gang_view = gang_view_by_id(gang_id);
		if ( !gang_view ) return ;
		Gang_DB_View::Member_Map::const_iterator find_it = gang_view->member_map.find(role_id);
		if (find_it != gang_view->member_map.end()) {
			info.week_contribute = find_it->second.week_contribute;
			info.total_contribute = find_it->second.total_contribute;
			info.headship = find_it->second.headship;
			Gang_Config_Authority::bit_auth_to_vec(find_it->second.authority, info.authority);
		}
	}
}

void Gang_Manager::make_gang_apply_info(role_id_t role_id, Gang_Apply_Info& info) {

}

void Gang_Manager::make_gang_notify_info(MSG_81070005 &msg, uint8_t action, int64_t id, std::string name, int32_t value) {
	msg.action = action;
	msg.id = id;
	msg.name = name;
	msg.value = value;
}

void Gang_Manager::active_gang_notify(int64_t id_1, uint8_t action, int64_t id_2, std::string name, int32_t value) {
	MSG_81070005 active_msg;
	switch (action) {
	case GNA_HAS_APPLY_TO_MANAGER: {
		Gang_DB_View* gang_view = gang_view_by_id(id_1);
		if (! gang_view) {
			return;
		}
		make_gang_notify_info(active_msg, action, id_2, name, value);
		for (Gang_DB_View::Member_Map::const_iterator iter = gang_view->member_map.begin();
				iter != gang_view->member_map.end(); ++iter) {
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(iter->first);
			if (! player) {
				continue;
			}
			std::bitset<32> auth_bit(player->ganger_detail().authority);
			if (auth_bit.test(GANGER_AU_CHECK_IN)) {
				OBJ_SEND_TO_CLIENT(active_msg, (*player));
			}
		}
		break;
	}
	case GNA_HAS_INVITE_TO_INVITEE: {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(id_1);
		if (player) {
			make_gang_notify_info(active_msg, action, id_2, name, value);
			OBJ_SEND_TO_CLIENT(active_msg, (*player));
		}
		break;
	}
	case GNA_BE_AGREE_TO_APPLYER: {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(id_1);
		if (player) {
			make_gang_notify_info(active_msg, action, id_2, name, value);
			OBJ_SEND_TO_CLIENT(active_msg, (*player));
		}
		break;
	}
	case GNA_BE_REFUSE_TO_APPLYER: {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(id_1);
		if (player) {
			make_gang_notify_info(active_msg, action, id_2, name, value);
			OBJ_SEND_TO_CLIENT(active_msg, (*player));
		}
		break;
	}
	case GNA_AGREE_TO_INVITER: {
		// turn to direct in or auto apply
		break;
	}
	case GNA_REFUSE_TO_INVITER: {
		Gang_DB_View* gang_view = gang_view_by_id( id_1 );
		if ( !gang_view ) {
			return;
		}
		make_gang_notify_info(active_msg, action, id_2, name, value);
		for (Gang_DB_View::Member_Map::const_iterator iter= gang_view->member_map.begin();
				iter != gang_view->member_map.end(); ++iter) {
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(iter->first);
			if ( !player ) {
				continue;
			}
			std::bitset<32> auth_bit(player->ganger_detail().authority);
			if (auth_bit.test(GANGER_AU_CHECK_IN)) {
				OBJ_SEND_TO_CLIENT(active_msg, (*player));
			}
		}
		break;
	}
	case GNA_HEADSHIP_AUTH_TO_ASSIGNEE: {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(id_1);
		if (player) {
			make_gang_notify_info(active_msg, action, id_2, name, value);
			OBJ_SEND_TO_CLIENT(active_msg, (*player));
		}
		break;
	}
	case GNA_BLESS_BUF_TO_ACCEPTER: {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(id_1);
		if (player) {
			make_gang_notify_info(active_msg, action, id_2, name, value);
			OBJ_SEND_TO_CLIENT(active_msg, (*player));
		}
		break;
	}
	case GNA_NOTIFY_MEMBER_BE_SEIZED: {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(id_1);
		if (player) {
			make_gang_notify_info(active_msg, action, id_2, name, value);
			OBJ_SEND_TO_CLIENT(active_msg, (*player));
		}
		break;
	}
	default: {
		break;
	}
	}
}

bool Gang_Manager::has_member(int64_t gang_id, role_id_t role_id) {
	Gang_DB_View* db_view = gang_view_by_id(gang_id);
	if (db_view) {
		return db_view->has_member(role_id);
	}
	return false;
}

int Gang_Manager::get_gang_level(int64_t gang_id) {
	Gang_DB_View* db_view = gang_view_by_id(gang_id);
	return db_view ? db_view->level : 0;
}

int Gang_Manager::change_gang_fund(int64_t gang_id, int change_value) {
	Gang_DB_View* gang_view = gang_view_by_id(gang_id);
	if (gang_view) {
		gang_view->fund += change_value;
		refresh_rank();
		notify_gang_info_update(*gang_view);
		gang_view->change_view();
	}
	return 0;
}

int Gang_Manager::req_gang_change_headship_auth(Gang_DB_View* gang_view, role_id_t role_id, int8_t headship, int auth) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	Gang_DB_View::Member_Map::iterator find_it = gang_view->member_map.find(role_id);
	if (find_it != gang_view->member_map.end()) {
		find_it->second.headship = headship;
		find_it->second.authority = auth;
		update_member_info(gang_view, role_id);
		gang_view->change_view();
	}
	return 0;
}

int Gang_Manager::req_gang_upgrade(role_id_t role_id, Gang_DB_View* gang_view) {
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (! player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	if (gang_view->level >= CONFIG_CACHE_GANG->limit().max_gang_lvl) {
		return ERROR_RUNE_HOLE_LVL_MAX;	// TODO error code
	}

	// check upgrade cd
	if (gang_view->cd_upgrade > Time_Value::gettimeofday()) {
		return ERROR_GANG_OPER_IN_CD;
	}
	// check fund
	const Gang_Config_Level& level_config = CONFIG_CACHE_GANG->base(gang_view->level);
	if ((uint)gang_view->fund < level_config.money) {
		return ERROR_GANG_NOTENOUGH_MONEY;
	}

	// sub fund
	gang_view->fund -= level_config.money;

	++gang_view->level;
	gang_view->level_leader_id[gang_view->level] = gang_view->leader;


	//---- 以下 gang_view->level 已变 ----
	// upgrade cd
	gang_view->cd_upgrade = Time_Value::gettimeofday() + CONFIG_CACHE_GANG->base(gang_view->level).upgrade_cd;

	// save and refresh
	refresh_rank();
	gang_view->change_view();
	notify_gang_info_update(*gang_view);
	notify_gang_row_info(GANG_ROW_INFO_UPDATE, *gang_view);

	// notify chat

	// add_passed_event
	gang_view->add_passed_event_101904(role_id, level_config.money);
	gang_view->send_last_passed_event();

	player->create_ann_gang_upgrade(role_id, level_config.money, gang_view->level);
	//gang boss open check
	int gang_boss_open_lv = CONFIG_CACHE_GANG->get_open_gang_boss_lv();
	if(gang_view->level == gang_boss_open_lv){// init open time
		gang_view->boss_hour = 22;
		gang_view->boss_minute = 0;
		Time_Value now = Time_Value::gettimeofday();
		Date_Time date_now(now);
		date_now.hour(gang_view->boss_hour);
		date_now.minute(gang_view->boss_minute);
		date_now.second(0);
		Time_Value set_time = Time_Value(date_now.time_sec());
		gang_view->boss_act_state = 0;
		gang_view->boss_act_time = set_time;
		player->create_gang_boss_announce(gang_view->gang_id, 7112, 0);
		{
			MSG_50111102 msg;
			msg.icon_id = 19;
			msg.kill_num = gang_view->boss_kill_num;
			msg.boss_lv = gang_view->boss_lv;
			msg.value1 = gang_view->boss_hour;
			msg.value2 = gang_view->boss_minute;
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}

	return 0;
}

int Gang_Manager::gang_lvl_clear_cd(Gang_DB_View* gang_view) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}

	gang_view->cd_upgrade = Time_Value::gettimeofday();

	gang_view->change_view();
	notify_gang_info_update(*gang_view);
	notify_gang_row_info(GANG_ROW_INFO_UPDATE, *gang_view);

	return 0;
}

int Gang_Manager::gang_building_clear_cd(Gang_DB_View* gang_view, int building_id) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	Gang_Building_Map::iterator it_building = gang_view->buildings.find(building_id);
	if (it_building == gang_view->buildings.end()) {
		return ERROR_INNER_SERVER;
	}

	it_building->second.cd_upgrade = Time_Value::gettimeofday();

	gang_view->change_view();
	notify_gang_info_update(*gang_view);
	//notify_gang_row_info(GANG_ROW_INFO_UPDATE, *gang_view);

	return 0;
}
int Gang_Manager::req_gang_war_board(Logic_Player &player) {
	MSG_50170036 res_msg;
	Gang_War_Spot spot;
	Gang_Basic_Detail gbd;
	if (gang_war_info_.stage_ == 2) {
		if (gang_war_info_.switch_ == GANG_WAR_CREATE_16) {
			const Gang_War_Matched_Gang_Info *gwmgi = gang_war_find_matched_gang_info(player.gang_id());
			if (gwmgi) {
				res_msg.icon = gwmgi->btn_state;
			}
		} else {
			res_msg.icon = 1;
		}
	} else {
		Time_Value now = Time_Value::gettimeofday();
		Date_Time dt(now);
		if ((dt.weekday() == 0
				&& ((dt.hour() > 20 || (dt.hour() == 20 && dt.minute() >= 55)))
				&& gang_war_info_.switch_ == GANG_WAR_STOP)
				|| !gang_war_info_.rank_award_.empty()) {
			res_msg.icon = 2;
		} else {
			res_msg.icon = 1;
		}
	}
	std::vector<Gang_War_Spot> info;
	for (std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.begin();
			it != gang_war_info_.gang_war_result_map_.end(); ++it) {
		if (gang_war_info_.stage_ == 2
				&& gang_war_info_.switch_ == GANG_WAR_CREATE_16) {
			Gang_War_Spot gws;
			gws.index = it->second.index;
			gws.result = it->second.result;
			bool is_self = false;
			for (std::vector<Gang_Basic_Detail>::const_iterator it_gv = it->second.gang_vec.begin();
					it_gv != it->second.gang_vec.end(); ++it_gv) {
				if (it_gv->gang_id == player.gang_id()) {
					if (res_msg.icon != 0) {
						gws.gang_vec.push_back(*it_gv);
						is_self = true;
					}
				} else {
					const Gang_War_Matched_Gang_Info *gwmgi = gang_war_find_matched_gang_info(it_gv->gang_id);
					if (gwmgi && gwmgi->btn_state != 0) {
						gws.gang_vec.push_back(*it_gv);
					}
				}
			}
			if (!gws.gang_vec.empty()) {
				if (is_self) {
					res_msg.info.push_back(gws);
				} else {
					info.push_back(gws);
				}
			}
		} else {
			Gang_War_Spot gws;
			gws.index = it->second.index;
			gws.result = it->second.result;
			bool is_self = false;
			for (std::vector<Gang_Basic_Detail>::const_iterator it_gv = it->second.gang_vec.begin();
					it_gv != it->second.gang_vec.end(); ++it_gv) {
				if (it_gv->gang_id == player.gang_id()) {
					is_self = true;
				}
				gws.gang_vec.push_back(*it_gv);
			}
			if (gws.gang_vec.empty()) {
				continue;
			}
			if (is_self) {
				res_msg.info.push_back(gws);
			} else {
				info.push_back(gws);
			}
		}
	}
	for (std::vector<Gang_War_Spot>::const_iterator it = info.begin(); it != info.end(); ++it) {
		res_msg.info.push_back(*it);
	}

	OBJ_SEND_TO_CLIENT(res_msg, player);
	return 0;
}
int Gang_Manager::req_gang_war_gang_rank(Logic_Player &player) {
	MSG_50170045 res_msg;
	GangWarGangRankInfo_Vec gang_rank_vec;
	Gang_War_Gang_Rank gwgr;
	for (GangWarGangRankInfo_Map::iterator it = gang_war_info_.gang_rank_map.begin();
			it != gang_war_info_.gang_rank_map.end(); ++it) {
		gang_rank_vec.push_back(&(it->second));
	}
	std::stable_sort(gang_rank_vec.begin(), gang_rank_vec.end(), Gang_War_Rank_Info_Less());

	for (GangWarGangRankInfo_Vec::const_iterator it = gang_rank_vec.begin(); it != gang_rank_vec.end(); ++it) {
		gwgr.reset();
		gwgr.gang_name = (**it).gang_name;
		gwgr.score = (**it).score;
		gwgr.win_nums = (**it).win_nums;
		gwgr.kill_nums = (**it).kill_nums;
		gwgr.hold_nums = (**it).hold_nums;
		gwgr.force = (**it).force;
		gwgr.zone = (**it).zone;
		res_msg.list.push_back(gwgr);
	}
	OBJ_SEND_TO_CLIENT(res_msg, player);
	return 0;
}
int Gang_Manager::req_gang_war_spot(Logic_Player &player, const int8_t index) {
	MSG_50170037 res_msg;
	res_msg.index = index;
	Gang_War_Rank_Info rank;
	if (index == -2) { // 面板：会员排名
		GangWarGangRankInfo_Map::iterator it = gang_war_info_.gang_rank_map.find(player.ganger_detail().gang_id);
		if (it != gang_war_info_.gang_rank_map.end()) {
			Gang_War_Rank_Info gwri;
			for (Ganer_Rank_Info::iterator it_gri = it->second.ganger_rank_info.begin(); it_gri != it->second.ganger_rank_info.end(); ++it_gri) {
				gwri.reset();
				gwri.gang_id = it_gri->second.gang_id;
				gwri.role_id = it_gri->second.role_id;
				gwri.gang_name = it_gri->second.gang_name;
				gwri.role_name = it_gri->second.role_name;
				gwri.kill_num = it_gri->second.kill_num;
				gwri.die_num = it_gri->second.die_num;
				gwri.hold_spot = it_gri->second.hold_spot;
				gwri.headship = it_gri->second.headship;
				gwri.force = it_gri->second.force;
				gwri.gang_name = it_gri->second.gang_name;
				gwri.camp = it_gri->second.camp;
				res_msg.rank.push_back(gwri);
			}
		}
		OBJ_SEND_TO_CLIENT(res_msg, player);
	} else if (index == -3) { // 请求参与并未分配的会员列表
		GangWarGangRankInfo_Map::iterator it = gang_war_info_.gang_rank_map.find(player.gang_id());
		if (it != gang_war_info_.gang_rank_map.end()) {
			Gang_War_Rank_Info gwri;
			for (Ganer_Rank_Info::iterator it_gri = it->second.ganger_rank_info.begin(); it_gri != it->second.ganger_rank_info.end(); ++it_gri) {
				gwri.reset();
				gwri.gang_id = it_gri->second.gang_id;
				gwri.role_id = it_gri->second.role_id;
				gwri.gang_name = it_gri->second.gang_name;
				gwri.role_name = it_gri->second.role_name;
				gwri.kill_num = it_gri->second.kill_num;
				gwri.die_num = it_gri->second.die_num;
				gwri.hold_spot = it_gri->second.hold_spot;
				gwri.headship = it_gri->second.headship;
				gwri.force = it_gri->second.force;
				gwri.camp = it_gri->second.camp;
				DB_View::ID_Player_View_Map::iterator iter =
							LOGIC_MONITOR->db_view()->id_player_view_map.find(it_gri->second.role_id);
				if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()
						|| iter->second->gang_war_award_get_times_by_day >= CONFIG_CACHE_GANG->gang_war_setting_cfg().box_limit) {
					gwri.index = -3;
				}
				res_msg.rank.push_back(gwri);
			}
		}
		OBJ_SEND_TO_CLIENT(res_msg, player);
	} else if (index >= 0) { // 当场节点结果
		std::map<int8_t, std::vector<Gang_War_Rank_Info> >::const_iterator it = gang_war_info_.gang_war_spot_rank_map_.find(index);
		if (it != gang_war_info_.gang_war_spot_rank_map_.end()) {
			res_msg.rank = it->second;
		}
		OBJ_SEND_TO_CLIENT(res_msg, player);
	} else if (index == -1) { // 公会战场内
		Block_Buffer buf;
		buf.make_message(INNER_SYNC_ASK_GANG_WAR_RANK_INFO);
		buf.write_int8(index);
		buf.finish_message();
		player.send_to_scene(buf);
	}

	return 0;
}
int Gang_Manager::active_gang_war_board_notice(const int8_t type, const int64_t role_id, const int64_t gang_id) {
	switch(type) {
	case 1: {
		for (GangWarMatchedGangInfo_Map::const_iterator it = gang_war_info_.gang_war_matched_gang_map_.begin();
				it != gang_war_info_.gang_war_matched_gang_map_.end(); ++it) {
			const Gang_War_Matched_Gangs_Pair *pr = GANG_MANAGER->gang_war_find_matched_gangs_pair(it->first);
			if (NULL == pr || pr->matched_pair.first == 0 || pr->matched_pair.second == 0) {
				continue;
			}
			Gang_DB_View *dbv = gang_view_by_id(it->first);
			if (dbv) {
				MSG_81170008 acv_msg;
				acv_msg.type = type;
				acv_msg.role_id = role_id;
				for (Gang_DB_View::Member_Map::const_iterator it = dbv->member_map.begin();
						it != dbv->member_map.end(); ++it) {
					Logic_Player *lp = LOGIC_MONITOR->find_logic_player_by_role_id(it->first);
					if (lp) {
						const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(lp->location().scene_id);
						if (cfg && Scene_Gang_War != cfg->manager && lp->gang_id() > 0) {
							OBJ_SEND_TO_CLIENT(acv_msg, (*lp));
						}
					}
				}
			}
		}
		break;
	}
	case 2: {
		Gang_DB_View *dbv = gang_view_by_id(gang_id);
		if (dbv) {
			MSG_81170008 acv_msg;
			acv_msg.type = type;
			acv_msg.role_id = role_id;
			for (Gang_DB_View::Member_Map::const_iterator it = dbv->member_map.begin();
					it != dbv->member_map.end(); ++it) {
				Logic_Player *lp = LOGIC_MONITOR->find_logic_player_by_role_id(it->first);
				if (lp && dbv->leader != it->first) {
					const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(lp->location().scene_id);
					if (cfg && Scene_Gang_War == cfg->manager && lp->gang_id() > 0) {
						OBJ_SEND_TO_CLIENT(acv_msg, (*lp));
					}
				}
			}
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

int Gang_Manager::req_gang_war_rank_award_list(Logic_Player &player) {
	MSG_50170044 res_msg;
	Reward_Info_Gang_War_Rank_Award rigwra;
//	Item_Detail item_detail;
//	Item_Vec item_vector;
//	Item_Vec item_vector_all;
	Reward_Info ri;
	for (Gang_War_Rank_Award::const_iterator it_ra = gang_war_info_.rank_award_.begin();
			it_ra != gang_war_info_.rank_award_.end(); ++it_ra) {
		rigwra.reset();
		rigwra.gang_id = it_ra->second.gang_id;
		rigwra.state = it_ra->second.state;
		rigwra.gang_name = it_ra->second.gang_name;
		rigwra.rank = it_ra->second.rank;
		rigwra.win_nums = it_ra->second.win_nums;
		rigwra.zone = it_ra->second.zone;
		const Gang_War_Rank_Award_Detail_Cfg *cfg = CONFIG_CACHE_GANG->find_gang_war_rank_award(rigwra.rank);
		if (cfg) {
			for (Item_Info_Vec::const_iterator it = cfg->list.begin(); it != cfg->list.end(); ++it) {
				ri.reset();
				ri.reward_id = it->id;
				ri.quantity = it->amount;
				rigwra.gang.push_back(ri);
			}

			for (Item_Info_Vec::const_iterator it = cfg->all_list.begin();
					it != cfg->all_list.end(); ++it) {
				ri.reset();
				ri.reward_id = it->id;
				ri.quantity = it->amount;
				rigwra.self.push_back(ri);
			}
		}
		res_msg.list.push_back(rigwra);
	}
	OBJ_SEND_TO_CLIENT(res_msg, player);
	return 0;
}
int Gang_Manager::req_gang_war_get_rank_award(Logic_Player &player) {
	int mail_id = 1039;
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(player.gang_id());
	if (!gang_view) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	if (player.ganger_detail().headship != GANG_HEADSHIP_LEADER
			&& player.ganger_detail().headship != GANG_HEADSHIP_VICE) {
		return ERROR_GANG_NOT_LEADER;
	}
	Gang_War_Gang_Id_Rank_Award::iterator it_gira = gang_war_info_.gang_id_rank_award_.find(player.gang_id());
	if (it_gira != gang_war_info_.gang_id_rank_award_.end()) {
		if (it_gira->second.state == 1) {
			return ERROR_GANG_WAR_GET_RANK_AWARD_SUCCESS;
		}
		int all_contribute = 0;
		Item_Detail item_detail;
		Item_Vec item_vec;
		std::vector<Item_Basic_Info> item_vec_all;
		Item_Basic_Info ibi;
		const Gang_War_Rank_Award_Detail_Cfg *cfg = CONFIG_CACHE_GANG->find_gang_war_rank_award(it_gira->second.rank);
		if (cfg) {
			int fund = 0;
			Reward_Info_Gang_War rigw;
			for (Item_Info_Vec::const_iterator it = cfg->list.begin(); it != cfg->list.end(); ++it) {
				rigw.reset();
				rigw.award.reward_id = it->id;
				rigw.award.quantity = it->amount;
				if (rigw.award.reward_id == 503803) {
					fund = rigw.award.quantity;
					continue;
				}
				int32_t amount = rigw.award.quantity;
				for (int32_t i = 0; i < amount; i++) {
					rigw.award.quantity = 1;
					rigw.index = gang_view->make_gang_war_index();
					gang_view->gang_war_award_map().insert(std::make_pair(rigw.index, rigw));
				}
				gang_view->change_view();
			}
			if (fund > 0) {
				gang_view->add_contribute(fund);
				GANG_MANAGER->refresh_rank();
				gang_view->change_view();
				// notify ganger info, gang info
				GANG_MANAGER->notify_gang_info_update(*gang_view);
			}

			for (Item_Info_Vec::const_iterator it = cfg->all_list.begin();
					it != cfg->all_list.end(); ++it) {
				switch (it->id) {
				case 503803: {
					all_contribute = it->amount;
					break;
				}
				default:
					item_detail.reset();
					item_detail.id_ = it->id;
					item_detail.amount_ = it->amount;
					item_detail.bind_ = Item_Detail::BIND;
					item_vec.push_back(item_detail);
					ibi.reset();
					item_detail.set_item_basic_info(ibi);
					item_vec_all.push_back(ibi);
					break;
				}
			}

			const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
			if(!mailc) {
				return ERROR_CONFIG_NOT_EXIST;
			}
			std::string contents;
			const int size_buf = mailc->mail_content.size() + \
					sizeof(it_gira->second.rank) + 10;
		    char content_buf[size_buf];
			memset(content_buf, 0, size_buf);
			snprintf(content_buf, size_buf, mailc->mail_content.c_str(), it_gira->second.rank);
			contents.assign(content_buf);

			for (Gang_DB_View::Member_Map::iterator it_mm = gang_view->member_map.begin();
					it_mm != gang_view->member_map.end(); ++it_mm) {
				Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(it_mm->first);
				if (pl) {
					if (all_contribute > 0) {
						pl->add_contribute(all_contribute);
					}
//						if (self_mercit > 0) {
//							pl->change_exploit_val(self_mercit);
//						}
				}
				LOGIC_PUBLIC->send_mail(1039, it_mm->first, item_vec_all, mailc->mail_title, contents);
			}
		}
		it_gira->second.state = 1;
		Gang_War_Rank_Award::iterator it_ra = gang_war_info_.rank_award_.find(it_gira->second.rank);
		if (it_ra != gang_war_info_.rank_award_.end()) {
			it_ra->second.state = 1;
		}
		gang_war_info_.save();
		req_gang_war_rank_award_list(player);
	} else {
		return ERROR_GANG_WAR_GET_RANK_AWARD_EMPTY;
	}
	return ERROR_GANG_WAR_GET_RANK_AWARD_SUCCESS;
}

void Gang_Manager::refresh_rank(void) {
	std::vector<Gang_DB_View_Rank> rank_info_vec;
	Gang_DB_View_Rank rank_info;
	for (Gang_ID_Map::const_iterator it_gang = gang_id_map.begin(); it_gang != gang_id_map.end(); ++it_gang) {
		rank_info.reset();
		rank_info.gang_id = it_gang->first;
		rank_info.level = it_gang->second->level;
		rank_info.member_size = it_gang->second->member_map.size();
		rank_info.fund = it_gang->second->fund;
		rank_info.force_total = it_gang->second->force_total;
		rank_info_vec.push_back(rank_info);
	}
	if (! rank_info_vec.empty()) {
		std::sort(rank_info_vec.begin(), rank_info_vec.end());
		std::reverse(rank_info_vec.begin(), rank_info_vec.end());

		std::map<gang_id_t, int> changed_gang_rank;
		for (size_t i = 0; i < rank_info_vec.size(); ++i) {
			Gang_ID_Map::iterator find_it = gang_id_map.find(rank_info_vec[i].gang_id);
			if (find_it != gang_id_map.end()) {
				int last_rank = find_it->second->rank;
				find_it->second->rank = i + 1;
				find_it->second->change_view();

				if (last_rank != find_it->second->rank) {
					changed_gang_rank.insert(std::make_pair(find_it->first, find_it->second->rank));
				}
			}
		}

		// 公会动态：排名变化
		if (! changed_gang_rank.empty()) {
			Gang_DB_View* gang_view = NULL;
			for (std::map<gang_id_t, int>::const_iterator it_changed = changed_gang_rank.begin();
					it_changed != changed_gang_rank.end(); ++it_changed) {
				gang_view = gang_view_by_id(it_changed->first);
				if (gang_view) {
					gang_view->add_passed_event_101908();
					gang_view->send_last_passed_event();

//					LOGIC_ANN_MANAGER->create_ann_gang_rank_change(it_changed->second, it_changed->first);
				}
			}
		}
	}
}

int Gang_Manager::update_member_info(Gang_DB_View* gang_view,  role_id_t role_id) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (! player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	Gang_DB_View::Member_Map::iterator find_it = gang_view->member_map.find(player->role_id());
	if (find_it != gang_view->member_map.end()) {
		find_it->second.week_contribute = player->ganger_detail().contrib_of_week;
		find_it->second.total_contribute = player->ganger_detail().total_contribute;
		find_it->second.headship = player->ganger_detail().headship;
		find_it->second.authority = CONFIG_CACHE_GANG->power(find_it->second.headship).auth_to_bit();
		gang_view->change_view();
	}
	return 0;
}

int Gang_Manager::send_group_mail(Gang_DB_View* gang_view, bool send_to_leader) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	Role_Vec sent_members;
	for (Gang_DB_View::Member_Map::iterator it_member = gang_view->member_map.begin();
			it_member != gang_view->member_map.end(); ++it_member) {
		if (it_member->first == gang_view->leader && !send_to_leader) {
			continue;
		}
		sent_members.push_back(it_member->first);
	}
	if (! sent_members.empty()) {
		LOGIC_PUBLIC->send_mail(1004, sent_members);
	}
	return 0;
}

int Gang_Manager::send_daily_fund_mail(Gang_DB_View* gang_view, bool send_to_leader) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	const Mail_Base_Config *mail_cfg = CONFIG_CACHE_MAIL->find_config(1004);
	if (! mail_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Role_Vec sent_members;
	for (Gang_DB_View::Member_Map::iterator it_member = gang_view->member_map.begin();
			it_member != gang_view->member_map.end(); ++it_member) {
		if (it_member->first == gang_view->leader && !send_to_leader) {
			continue;
		}
		sent_members.push_back(it_member->first);
	}

	if (! sent_members.empty()) {
		Mail_Send_Info send_info;
		send_info.sender_id = 0;
		send_info.sender_name = mail_cfg->sender_name;
		send_info.send_type = 0;
		send_info.title = mail_cfg->mail_title;

		// mail content
		int remain_days = MAX_FUND_LACK_TIMES - gang_view->fund_lack_times;
		std::string contents;
		const int size_buf = mail_cfg->mail_content.size() + \
				sizeof(remain_days) + 10;
	    char content_buf[size_buf];
		memset(content_buf, 0, size_buf);
		snprintf(content_buf, size_buf, mail_cfg->mail_content.c_str(), remain_days);
		contents.assign(content_buf);
		send_info.content = contents;

		LOGIC_PUBLIC->send_mail(sent_members, send_info);
	}

	return 0;
}

void Gang_Manager::daily_fund(void) {
	// 每日维护费用
	for (Gang_ID_Map::iterator it_gang = gang_id_map.begin(); it_gang != gang_id_map.end();) {
		const Gang_Config_Level& level_config = CONFIG_CACHE_GANG->base(it_gang->second->level);
		if (level_config.cost > 0) {
			if (it_gang->second->fund >= (int)level_config.cost) {
				it_gang->second->fund -= level_config.cost;
				it_gang->second->fund_lack_times = 0;
			} else {
				if (it_gang->second->fund_lack_times <= MAX_FUND_LACK_TIMES) {
					// 群邮件：维护费用不足
					send_daily_fund_mail(it_gang->second, true);
					// 公会动态：维护费用不足
					Gang_DB_View* gang_view = gang_view_by_id(it_gang->first);
					if (gang_view) {
						int remain_day = MAX_FUND_LACK_TIMES - gang_view->fund_lack_times;
						gang_view->add_passed_event_101909(remain_day);
						gang_view->send_last_passed_event();

						LOGIC_ANN_MANAGER->create_ann_gang_fund_lack(remain_day, gang_view->gang_id);
					}
				}
				++it_gang->second->fund_lack_times;
			}
		}
		// dismiss gang if lack fund 3 days
		if (it_gang->second->fund_lack_times > MAX_FUND_LACK_TIMES) {
			it_gang = dismiss_gang(it_gang, true);
			continue;
		}
		++it_gang;
	}
}

void Gang_Manager::daily_boss(void){
	Time_Value now = Time_Value::gettimeofday();
	Date_Time date_now(now);

	Gang_ID_Map::iterator it;
	for (it = gang_id_map.begin(); it != gang_id_map.end(); ++it) {
		it->second->boss_act_state = 0;
		date_now.hour(it->second->boss_hour);
		date_now.minute(it->second->boss_minute);
		date_now.second(0);
		it->second->boss_act_time = Time_Value(date_now.time_sec());
		if(it->second->boss_act != 0){
			it->second->boss_act = 0;
			it->second->change_view(true);
		}
	}
	gang_boss_join_flag_map_.clear();
}

void Gang_Manager::daily_leader(void) {
	//const Time_Value &expiration = Time_Value::gettimeofday() - Time_Value(Time_Value::ONE_DAY_IN_SECS * 4);
	//const Time_Value &expiration = Time_Value::gettimeofday() - Time_Value(Time_Value::ONE_MINUTE_IN_SECS * 3);
	const Time_Value &expiration = Time_Value::gettimeofday() - CONFIG_CACHE_GANG->limit().cd_seize_power;
	for (Gang_ID_Map::iterator it_gang = gang_id_map.begin(); it_gang != gang_id_map.end(); ++it_gang) {
		if (LOGIC_MONITOR->find_logic_player_by_role_id(it_gang->second->leader)) {		// 在线
			it_gang->second->can_seize_power = 0;
		} else if (it_gang->second->tv_leader_offline < expiration){					// 离线 如果过期
			it_gang->second->can_seize_power = 1;
		}
	}
}

void Gang_Manager::gang_init(void) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time date_now(now);

	Gang_Building_Detail building_detail;
	for (Gang_ID_Map::iterator it_gang = gang_id_map.begin(); it_gang != gang_id_map.end(); ++it_gang) {
		building_init(it_gang->second);
		// gang boss init
		if(it_gang->second->boss_hour == 0 && it_gang->second->boss_minute){// 默认22点开
			it_gang->second->boss_hour = 22;
		}
		date_now.hour(it_gang->second->boss_hour);
		date_now.minute(it_gang->second->boss_minute);
		date_now.second(0);
		it_gang->second->boss_act_time = Time_Value(date_now.time_sec());
	}
}

void Gang_Manager::building_init(Gang_DB_View* gang_view) {
	if (! gang_view) {
		return;
	}
	Gang_Building_Detail building_detail;
	for (int building_id = BUILDING_BEGIN; building_id < BUILDING_END; ++building_id) {
		if (! gang_view->buildings.count(building_id)) {
			building_detail.building_id = building_id;
			building_detail.building_lvl = 1;
			gang_view->buildings.insert(std::make_pair(building_detail.building_id, building_detail));
			gang_view->change_view();
		}
	}
}

int Gang_Manager::refresh_force(Gang_DB_View* gang_view) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	gang_view->force_total = 0;
	const DB_View::ID_Player_View_Map& player_view_map = LOGIC_MONITOR->db_view()->id_player_view_map;
	for (Gang_DB_View::Member_Map::const_iterator it_member = gang_view->member_map.begin();
			it_member != gang_view->member_map.end(); ++it_member) {
		DB_View::ID_Player_View_Map::const_iterator it_player_view = player_view_map.find(it_member->first);
		if (it_player_view != player_view_map.end()) {
			gang_view->force_total += it_player_view->second->ttl_force;
		}
	}
	return 0;
}

bool Gang_Manager::in_active_annon_ed(int64_t gang_id, uint32_t level) {
	Gang_ID_Map::iterator iter = gang_id_map.find( gang_id );
	if(iter != gang_id_map.end()) {
		return (iter->second->acitve_annon_ed.find(level) != iter->second->acitve_annon_ed.end());
	}
	return false;
}

void Gang_Manager::set_active_annon_ed(int64_t gang_id, uint32_t level) {
	Gang_ID_Map::iterator iter = gang_id_map.find( gang_id );
	if(iter != gang_id_map.end()) {
		iter->second->acitve_annon_ed.insert(level);
	}
}
