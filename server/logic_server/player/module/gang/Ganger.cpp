/*
 * Ganger.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: caijianwei
 */

#include "Ganger.h"
#include "Gang_Struct.h"
#include "Gang_Def.h"
#include "Msg_Struct.h"
#include "Msg_Gang_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Active_Struct.h"
#include "Err_Code.h"
#include "Config_Cache_Gang.h"
#include "Fighter_Detail.h"
#include "Logic_Player.h"
#include "pack/Pack_Struct.h"
#include "operating/Operating.h"
#include "Gang_Manager.h"
#include "Logic_Monitor.h"
#include "Logic_Player_Struct.h"
#include "Config_Cache_Mail.h"
#include "mail/Mail_Struct.h"
#include "Logic_Public.h"
#include "Msg_Inner_Enum.h"
#include "status/Status_Struct.h"
#include "equiper/Equiper_Func.h"
#include "helper/Helper_Def.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Public.h"
#include "public/Public_Def.h"
#include "Config_Cache_Item.h"
#include "covenant/Covenant.h"
#include "covenant/Covenant_Def.h"
#include "Config_Cache.h"
#include "expedition/Expedition_Manager.h"

Ganger::Ganger() {
	reset();
}

Ganger::~Ganger() { }

void Ganger::reset() {
	ganger_detail_ = NULL;
}

int Ganger::create_init(Ganger_Detail& detail) {
	return 0;
}

void Ganger::load_detail(Ganger_Detail& detail) {
	ganger_detail_ = &detail;
}

void Ganger::save_detail(Ganger_Detail& data) {
	if ( ganger_detail_ && ganger_detail_->is_change ) {
		data = *ganger_detail_;
		ganger_detail_->is_change = false;
	}
}

int Ganger::trigger_daily_zero(const Time_Value& now) {
	// 没有公会也要重置,防止第一天被退公会第二天又加入新公会
	ganger_detail_->today_has_draw_times = 0;
	ganger_detail_->day_diam_contrib_times = 0;
	ganger_detail_->day_gold_contrib_times = 0;
	ganger_detail_->day_contribute = 0;
	ganger_detail_->receive_bless_times = 0;	// TODO

	if (! ganger_detail_->gang_id) {
		return -1;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return -1;
	}


	ganger_detail_->detail_change();
	active_ganger_info_update();
	return 0;
}

int Ganger::trigger_daily_six(const Time_Value &now, bool logic_check) {
	ganger_detail_->gang_war_attend_time = Time_Value::zero;
	ganger_detail_->gang_war_attend_gang_id = 0;
	ganger_detail_->gang_war_attend_gang_id2 = 0;
	ganger_detail_->gang_war_award_get_times_by_day = 0;

	ganger_detail_->practice_times_ttl_ = 0;
	stop_phy_power_practice();

	ganger_detail_->detail_change();
	return 0;
}

int Ganger::trigger_weekly_zero(const Time_Value& now) {
	if (ganger_detail_->gang_id) {
		ganger_detail_->contrib_of_week = 0;
		ganger_detail_->detail_change();
		ganger_detail_->gang_war_award_get_times_by_day = 0;
		GANG_MANAGER->update_member_info(GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id), ganger_detail_->role_id);
	}
	return 0;
}

int Ganger::sync_scene_module_info(Block_Buffer& buf) {
//	if (player_self()->location().scene_id == GANG_WAR_SCENE_ID) {
		this->ganger_detail_->gang_war_serialize(buf);
//	}
	return 0;
}

void Ganger::module_init(void) {
	if (ganger_detail_->achi_contribute < ganger_detail_->total_contribute) {
		ganger_detail_->achi_contribute = ganger_detail_->total_contribute;
	}

	if (ganger_detail_->gang_id && !GANG_MANAGER->has_member(ganger_detail_->gang_id, ganger_detail_->role_id)) {
		handle_offline_kick();
	}

	// 修复公会数据
	module_repair();

	module_init_headship_auth();

	if (ganger_detail_->gang_id) {
		ganger_prop_refresh();
	}

	ganger_refresh_base_detail(false);

	//--------------------------------------------

	if (player_self()->location().scene_id == GANG_WAR_SCENE_ID) {
		const Gang_War_Matched_Gang_Info *info = GANG_MANAGER->gang_war_find_matched_gang_info(this->ganger_detail_->gang_id);
		if (GANG_MANAGER->gang_war_is_open()
				&& info
				&& this->ganger_detail_->gang_war_attend_time.sec() == info->match_time.sec()
				&& this->ganger_detail_->gang_war_attend_gang_id == this->ganger_detail_->gang_id) {
			const Gang_War_Spot *result_info = GANG_MANAGER->gang_war_find_result_map(this->ganger_detail_->gang_id);
			if (result_info && result_info->result == 0) {
				this->ganger_detail_->gang_war_camp = info->camp;
				player_self()->set_create_scene_args(0, Time_Value::zero, info->scene_index, 0, 0, info->matched_index, true);
			}
		}
	}

}

void Ganger::sign_in(void) {
	//GANG_MANAGER->remove_offline_ganger_detail( role_id() );

	refresh_can_seize_power();
	notify_last_leader();

	// notify online status to client
	if (ganger_detail_->gang_id) {
		GANG_MANAGER->update_member_info(GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id), ganger_detail_->role_id);
		GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, role_id(), ganger_detail_->gang_id);
	}
}

void Ganger::sign_in_repeat(void) {
}

void Ganger::sign_out(void) {
	//GANG_MANAGER->cache_offline_ganger_detail( *ganger_detail_ );
	refresh_leader_offline_time();

	// notify online status to client
	if (ganger_detail_->gang_id) {

		GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, role_id(), ganger_detail_->gang_id, true);
	}
}
int Ganger::move_scene_completed(void) {
	if (level() >= 20) {
		if (GANG_MANAGER->gang_war_info().open_time_for_each_match == Time_Value::zero) {
			return -1;
		}
		Time_Value now = Time_Value::gettimeofday();
		Date_Time dt(GANG_MANAGER->gang_war_info().open_time_for_each_match);

		MSG_81000408 acv_msg;
		acv_msg.icon_id = 1;
		if (GANG_MANAGER->gang_war_info().switch_ == GANG_WAR_START) {
			acv_msg.open = 1;
			acv_msg.active = 1;
			acv_msg.time = 0;
			if (now > Time_Value(dt.time_sec() + CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time + 10)) {
				return 0;
			}
			acv_msg.time = dt.time_sec() + CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time - now.sec();
		} else {
			return 0;
		}
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return 0;
}

int Ganger::req_create_gang(int8_t logo, const std::string& msg_name, const int8_t money_type) {
	// check gang_id, if joined a gang
	if ( ganger_detail_->gang_id != 0 ) {
		return ERROR_GANG_ALREADY_GANGER;
	}
	// check forbid time
	if ( (ganger_detail_->leave_time + CONFIG_CACHE_GANG->limit().join_cd)
			> Time_Value::gettimeofday() ) {
		return ERROR_GANG_NOT_NOW;
	}

	Logic_Player* self = player_self();

	// TODO check logo

	// check level
	if ( self->level() < CONFIG_CACHE_GANG->limit().limit_level ) {
		return ERROR_GANG_LEVEL_LIMITED;
	}

	// check name
	std::string name = msg_name;
	name.erase(0, name.find_first_not_of(" \t"));
	name.erase(name.find_last_not_of(" \t")+1);
	if (name.empty() || name.size() > 100) {
		return ERROR_CLIENT_PARAM;
	}
	if ( ! GANG_MANAGER->validate_new_name(name) ) {
		return ERROR_GANG_WRONG_NAME;
	}

	// check money
	if (money_type != 0 && money_type != 1) {
		return ERROR_CLIENT_PARAM;
	}
	Money_Sub_Type money_sub_type = money_type ? COPPER_ONLY : GOLD_ONLY;
	int money_cost = (money_sub_type == GOLD_ONLY) ?
			CONFIG_CACHE_GANG->limit().create_cost : CONFIG_CACHE_GANG->limit().create_cost_copper;
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(money_sub_type), money_cost, MONEY_SUB_FACTION_CREATE));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}

	//------- all check passed ------
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}

	// create new gang_view
	gang_id_t new_gang_id = GANG_MANAGER->create_gang(
			Server_Key(self->base_detail().account_info.agent_num, self->base_detail().account_info.server_num),
			self->role_id(), logo, name );
	if (new_gang_id <= 0) {
		return ERROR_INNER_SERVER;
	}
	ganger_detail_->last_gang_id = ganger_detail_->gang_id;
	ganger_detail_->gang_id = new_gang_id;

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view ) {
		return ERROR_GANG_NOT_EXIST;
	}

	gang_view->level_leader_id[1] = gang_view->leader;//初始化

	create_gang_init_ganger(gang_view);

	ganger_refresh_base_detail();

	if (! ganger_detail_->skill_map.empty()) {
		ganger_prop_refresh();
		player_self()->heroer_prop_refresh();
	}

	active_ganger_string_info_change();

	ganger_detail_->detail_change();

	// sync to chat
	sync_ganger_info_to_chat();
	MSG_50170000 msg;
	msg.gang_id = ganger_detail_->gang_id;
	THIS_SEND_TO_CLIENT(msg);
	// notifications
	active_ganger_info_update();
	GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_ADD, role_id(), ganger_detail_->gang_id);
	GANG_MANAGER->notify_gang_info_update( ganger_detail_->gang_id );
	GANG_MANAGER->notify_gang_row_info( GANG_ROW_INFO_ADD, ganger_detail_->gang_id);
	// server announcement
	self->create_gang_announce( ganger_detail_->gang_id, name);

	//监听加入公会的任务
	if(ganger_detail_->gang_id) {
		self->task_listen_join_gang(ganger_detail_->gang_id);
		self->notify_open_icon_by_join_gang();
	}

	EXPEDITION_MANAGER->send_gang_info_to_scene(new_gang_id);
	return 0;
}

int Ganger::req_join_gang(int64_t gang_id) {
	// check gang_id, if joined a gang
	if ( ganger_detail_->gang_id != 0 ) {
		return ERROR_GANG_ALREADY_GANGER;
	}
	// check forbid time
	if ( (ganger_detail_->leave_time + CONFIG_CACHE_GANG->limit().join_cd)
			> Time_Value::gettimeofday() ) {
		return ERROR_GANG_NOT_NOW;
	}
	// check level
	if ( level() < CONFIG_CACHE_GANG->limit().limit_level ) {
		return ERROR_GANG_LEVEL_LIMITED;
	}
	return GANG_MANAGER->join_gang( player_self(), gang_id );
}

int Ganger::req_gang_info(int64_t gang_id) {
	if ( gang_id == 0 || gang_id == ganger_detail_->gang_id ) {
		return GANG_MANAGER->res_gang_info( ganger_detail_->gang_id, player_self() );
	}
	return 0;
}

int Ganger::req_ganger_info(role_id_t role_id) {
	Logic_Player* self = player_self();
	if ( role_id == 0 || self->role_id() == role_id ) {
		if ( ganger_detail_->gang_id == 0 ) {
			return ERROR_GANG_NOT_IN_GANG;
		}
		MSG_50170003 msg;
		make_ganger_info(msg.ganger_info);
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Ganger::req_gang_list( ) {
	GANG_MANAGER->res_gang_list( player_self() );
	return 0;
}

int Ganger::req_ganger_list( int64_t gang_id) {
	if ( gang_id == 0 || gang_id == ganger_detail_->gang_id ) {
		return GANG_MANAGER->res_ganger_list( player_self(), ganger_detail_->gang_id );
	}
	return 0;
}

void Ganger::approve_join_gang(Gang_DB_View* gang_view) {
	if (! gang_view) {
		return;
	}
	ganger_detail_->last_gang_id = ganger_detail_->gang_id;
	ganger_detail_->gang_id = gang_view->gang_id;
	ganger_detail_->headship = GANG_HEADSHIP_NORMAL;
	ganger_detail_->join_time = Time_Value::gettimeofday();
	ganger_detail_->authority = CONFIG_CACHE_GANG->power(GANG_HEADSHIP_NORMAL).auth_to_bit();

	GANG_MANAGER->erase_all_gang_apply(player_self());
	ganger_detail_->apply_ids.clear();

	//ganger_detail_->remain_draw_times = CONFIG_CACHE_GANG->base(gang_view->level).daily_draw_times;

	ganger_detail_->detail_change();

	ganger_refresh_base_detail();

	if (! ganger_detail_->skill_map.empty()) {
		ganger_prop_refresh();
		player_self()->heroer_prop_refresh();
	}

	active_ganger_string_info_change();

	MSG_50170001 msg;
	msg.gang_id = ganger_detail_->gang_id;
	msg.gang_name = gang_view->name;
	msg.result = 1;
	THIS_SEND_TO_CLIENT(msg);

	//监听加入公会的任务
	if(ganger_detail_->gang_id) {
		player_self()->task_listen_join_gang(ganger_detail_->gang_id);
		player_self()->notify_open_icon_by_join_gang();
		player_self()->active_content_listen_gang_change();
	}
}

void Ganger::promote_to_leader() {
	ganger_detail_->headship = GANG_HEADSHIP_LEADER;
	ganger_detail_->authority = CONFIG_CACHE_GANG->power(GANG_HEADSHIP_LEADER).auth_to_bit();
	ganger_detail_->detail_change();
	ganger_refresh_base_detail();
}

int Ganger::promote_to_headship(const int headship, bool sync/* = true*/) {
	if (headship < GANG_HEADSHIP_NORMAL || headship > GANG_HEADSHIP_LEADER ) {
		return ERROR_PERM_LIMIT;
	}
	if (ganger_detail_->headship != headship) {
		ganger_detail_->headship = headship;
		ganger_detail_->authority = CONFIG_CACHE_GANG->power(headship).auth_to_bit();
		ganger_detail_->detail_change();
		ganger_refresh_base_detail(sync);
	}
	return 0;
}

void Ganger::kicked_out_gang(gang_id_t quit_gang_id) {
	ganger_detail_->set_quit(quit_gang_id);
	ganger_refresh_base_detail();
	ganger_prop_refresh();
	player_self()->heroer_prop_refresh();
	active_ganger_string_info_change();
	active_ganger_info_update();
	sync_ganger_info_to_chat();
	ganger_detail_->detail_change();
	const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->base_detail().location.scene_id);
	if (sc
			&& (sc->manager == Scene_Gang_War
					|| sc->manager == Scene_Expedition_Manager)) {
		int scene_id = 0;
		player_self()->inner_transmit(scene_id, 0, 0, "");
	}
	player_self()->expedition_listen_quit_gang();
}

void Ganger::handle_offline_kick() {
	ganger_detail_->offline_set_quit();
	ganger_detail_->detail_change();
	player_self()->set_gang_id(ganger_detail_->gang_id);
	player_self()->set_gang_name(GANG_MANAGER->gang_name(ganger_detail_->gang_id));
}

void Ganger::offline_be_seize_power(void) {
	if (! ganger_detail_->gang_id || ganger_detail_->headship != GANG_HEADSHIP_LEADER) {
		return;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return;
	}
	Gang_DB_View::Member_Map::iterator it_member = gang_view->member_map.find(ganger_detail_->role_id);
	if (it_member != gang_view->member_map.end()) {
		this->promote_to_headship(it_member->second.headship);
	}
}

void Ganger::module_repair(void) {
	if (! ganger_detail_->gang_id) {
		if (ganger_detail_->last_gang_id) {
			// ganger_detail_ 退出，但gang_view来不及退出
			const Gang_DB_View* last_gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->last_gang_id);
			if (last_gang_view) {
				Gang_DB_View::Member_Map::const_iterator it_last_member = last_gang_view->member_map.find(ganger_detail_->role_id);
				if (it_last_member != last_gang_view->member_map.end()) {
					ganger_detail_->gang_id = last_gang_view->gang_id;
					//ganger_detail_->day_contribute = it_last_member->second.
					ganger_detail_->contrib_of_week = it_last_member->second.week_contribute;
					ganger_detail_->total_contribute = it_last_member->second.total_contribute;
					ganger_detail_->headship = it_last_member->second.headship;
					ganger_detail_->authority = it_last_member->second.authority;
				}
			}
		}
		return;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		// 无gang_view
		ganger_detail_->reset();
		ganger_detail_->role_id = role_id();
		ganger_detail_->detail_change();
		ganger_refresh_base_detail();
		player_self()->heroer_prop_refresh();
		return;
	}

	if (! gang_view->has_member(role_id())) {
		// 有gang_view，不是成员
		ganger_detail_->reset();
		ganger_detail_->role_id = role_id();
		ganger_detail_->detail_change();
		ganger_refresh_base_detail();
		player_self()->heroer_prop_refresh();
	}
}

void Ganger::module_init_headship_auth(void) {
	if (! ganger_detail_->gang_id) {
		return;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return;
	}
	Gang_DB_View::Member_Map::iterator it_member = gang_view->member_map.find(ganger_detail_->role_id);
	if (it_member != gang_view->member_map.end()) {
		promote_to_headship(it_member->second.headship, false);
	}
}

void Ganger::offline_operate_init(const Ganger_Detail& detail) {
	if (detail.gang_war_award_get_times_by_day > 0) {
		ganger_detail_->gang_war_award_get_times_by_day = detail.gang_war_award_get_times_by_day;
		ganger_detail_->detail_change();
	}
	// 离线踢然后core，回滚踢的操作
	if (ganger_detail_->gang_id) {
		Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
		if (!gang_view || gang_view->has_member(player_self()->role_id())) {
			return;
		}
	}
	ganger_detail_->gang_id = detail.gang_id;
	ganger_detail_->last_gang_id = detail.last_gang_id;
	ganger_detail_->day_contribute = detail.day_contribute;
	ganger_detail_->day_gold_contrib_times = detail.day_gold_contrib_times;
	ganger_detail_->day_diam_contrib_times = detail.day_diam_contrib_times;
	ganger_detail_->contrib_of_week = detail.contrib_of_week;
	ganger_detail_->authority = detail.authority;
	ganger_detail_->join_time = detail.join_time;
	ganger_detail_->leave_time = detail.leave_time;
	ganger_detail_->headship = detail.headship;
	ganger_detail_->detail_change();

	player_self()->set_gang_id(ganger_detail_->gang_id);
	player_self()->set_gang_name(GANG_MANAGER->gang_name(ganger_detail_->gang_id));

	GANG_MANAGER->update_member_info(GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id), ganger_detail_->role_id);
}

void Ganger::update_offline_ganger_detail(const Ganger_Detail& detail) {
//	if (ganger_detail_->gang_id != detail.gang_id) {
//		ganger_detail_->last_gang_id = ganger_detail_->gang_id;
//	}

	// 离线踢然后core，回滚踢的操作
	if (ganger_detail_->gang_id) {
		Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
		if (!gang_view || gang_view->has_member(player_self()->role_id())) {
			return;
		}
	}

	ganger_detail_->gang_id = detail.gang_id;
	ganger_detail_->last_gang_id = detail.last_gang_id;
	ganger_detail_->day_contribute = detail.day_contribute;
	ganger_detail_->day_gold_contrib_times = detail.day_gold_contrib_times;
	ganger_detail_->day_diam_contrib_times = detail.day_diam_contrib_times;
	ganger_detail_->contrib_of_week = detail.contrib_of_week;
	ganger_detail_->authority = detail.authority;
	ganger_detail_->join_time = detail.join_time;
	ganger_detail_->leave_time = detail.leave_time;
	ganger_detail_->headship = detail.headship;
	ganger_detail_->detail_change();

	if (ganger_detail_->gang_id) {
		GANG_MANAGER->update_member_info(GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id), ganger_detail_->role_id);
	}

	player_self()->set_gang_id(ganger_detail_->gang_id);
	player_self()->set_gang_name(GANG_MANAGER->gang_name(ganger_detail_->gang_id));

	// sync to chat
	sync_ganger_info_to_chat();
	// notify client
	active_ganger_info_update();
	// TODO sync to scene
}

int Ganger::req_quit_gang() {
	if ( ganger_detail_->gang_id != 0 ) {
		int res = GANG_MANAGER->quit_gang( player_self(), ganger_detail_->gang_id );
		if ( res != 0 ) return res;
		// set ganger detail
		ganger_detail_->set_quit(ganger_detail_->gang_id);
		ganger_refresh_base_detail();
		ganger_prop_refresh();
		player_self()->heroer_prop_refresh();
		active_ganger_string_info_change();
		ganger_detail_->detail_change();
		// sync to local chat
		sync_ganger_info_to_chat();
		active_ganger_info_update();
		MSG_50170006 msg;
		THIS_SEND_TO_CLIENT(msg);

		player_self()->active_content_listen_gang_change();

		const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->base_detail().location.scene_id);
		if (sc
				&& (sc->manager == Scene_Gang_War
						|| sc->manager == Scene_Expedition_Manager)) {
			int scene_id = 0;
			player_self()->inner_transmit(scene_id, 0, 0, "");
		}
		player_self()->expedition_listen_quit_gang();
	}
	return 0;
}

int Ganger::req_gang_info_listen(int8_t operate) {
	GANG_INFO_LISTEN_IN == operate ? GANG_MANAGER->add_gang_info_listener(role_id()):
			GANG_MANAGER->remove_gang_info_listener( role_id() );
	MSG_50170008 msg;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Ganger::req_gang_leader_resign(role_id_t target) {
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id( ganger_detail_->gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	if ( role_id() != gang_view->leader ) return ERROR_GANG_NOT_LEADER;
	int res = GANG_MANAGER->resign_to( player_self(), ganger_detail_->gang_id, target);
	if ( res != 0 ) return res;
	// response resign success message
	MSG_50170007 msg;
	THIS_SEND_TO_CLIENT(msg);
	// request quit gang
	//req_quit_gang();
	return 0;
}

int Ganger::req_kick_out_gang(role_id_t target) {
	Logic_Player* self = player_self();
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id( ganger_detail_->gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	std::bitset<32> set_author( ganger_detail_->authority );
	if ( !set_author.test(GANGER_AU_KICK_OUT) || target == gang_view->leader ) {
		return ERROR_GANG_NO_AUTHORITY;
	}
	// remove ganger
	if ( gang_view->member_map.find(target) == gang_view->member_map.end() ){
		return ERROR_GANG_NO_MEMBER;
	}
	gang_view->member_map.erase( target );
	GANG_MANAGER->refresh_force(gang_view);
	gang_view->change_view();
	// TODO calculate rank
	// notify gang info
	GANG_MANAGER->notify_gang_info_update(*gang_view);
	// notify gang/ganger row info
	GANG_MANAGER->notify_gang_row_info( GANG_ROW_INFO_UPDATE, *gang_view );
	GANG_MANAGER->notify_ganger_row_info( GANG_ROW_INFO_REMOVE, target, ganger_detail_->gang_id);
	// change target ganger detail
	Logic_Player* target_player = LOGIC_MONITOR->find_logic_player_by_role_id( target );
	if ( target_player ) { // online
		target_player->kicked_out_gang(gang_view->gang_id);
		MSG_50170009 msg;
		msg.kicker_info.career = self->career();
		msg.kicker_info.combat = self->fighter_detail().force;
		msg.kicker_info.gender = self->base_detail().gender;
		msg.kicker_info.level  = self->base_detail().level;
		msg.kicker_info.role_id = self->role_id();
		msg.kicker_info.role_name = self->base_detail().role_name;
		msg.kicker_info.vip_level = self->vip();
		msg.kicker_info.guild_name = gang_view->name;
		msg.be_kicked_name = target_player->role_name();
		{OBJ_SEND_TO_CLIENT(msg, (*target_player));}
		{OBJ_SEND_TO_CLIENT(msg, (*self));}

		target_player->active_content_listen_gang_change();

		const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(target_player->base_detail().location.scene_id);
		if (sc && sc->manager == Scene_Gang_War) {
			int scene_id = 0;
			target_player->inner_transmit(scene_id, 0, 0, "");
		}
	} else {
		// TODO off line handle
		Ganger_Detail detail;
		detail.set_quit(gang_view->gang_id);
		Operating::offline_ganger_info_update(target, detail);
	}
	// add in passed event
	gang_view->add_passed_event_101903( self->role_id(), target);
	gang_view->send_last_passed_event();
	self->create_ann_gang_kick(role_id(), target);
	return 0;
}

int Ganger::req_set_gang_announce(const std::string& announce) {
	if ( ganger_detail_->gang_id == 0 ) return ERROR_GANG_NOT_IN_GANG;
	std::bitset<32> auth_bit( ganger_detail_->authority );
	if ( !auth_bit.test(GANGER_AU_SET_ANNOUNCE) ) {
		return ERROR_GANG_NO_AUTHORITY;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}

	// TODO check announce, censorship
	int res = GANG_MANAGER->set_gang_annouce(ganger_detail_->gang_id, announce);
	if ( res != 0 ) return res;
	GANG_MANAGER->notify_gang_info_update(ganger_detail_->gang_id);

	gang_view->add_passed_event_101910(ganger_detail_->role_id);
	gang_view->send_last_passed_event();

	player_self()->create_ann_gang_announce_set(ganger_detail_->role_id);

	MSG_50170010 msg;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Ganger::req_invite_to_join( Role_Vec targets ) {
	Logic_Player* self = player_self();
	if ( ganger_detail_->gang_id == 0 ) return ERROR_GANG_NOT_IN_GANG;
	// authority check
	std::bitset<32> auth_bit( ganger_detail_->authority );
	if ( !auth_bit.test(GANGER_AU_INVITE_IN) ) return ERROR_GANG_NO_AUTHORITY;
	// check multi invitees cd
	if ( !GANG_MANAGER->invite_friend_cd_ok(self->role_id()) ) return ERROR_GANG_OPER_IN_CD;
	// check copper sub
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY),
			CONFIG_CACHE_GANG->limit().cost_friendset, MONEY_SUB_FACTION_INVITE_FRIEND));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}

	//------- all check passed ------
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}

//	if ( targets.size() > 1 ) {
//		int gold_res = self->pack_try_sub_money( Money_Sub_Info(COPPER_ONLY,
//						CONFIG_CACHE_GANG->limit().cost_friendset, MONEY_SUB_FACTION_INVITE_FRIEND) );
//		if ( gold_res != 0 ) return gold_res;
//	}
	bool direct_in = auth_bit.test(GANGER_AU_CHECK_IN);

//	MSG_50170011 res_msg;
//	res_msg.gang_id = ganger_detail_->gang_id;
//	res_msg.gang_name = GANG_MANAGER->gang_name(ganger_detail_->gang_id);
//	res_msg.inviter = self->role_id();
//	res_msg.inviter_name = self->role_name();
//	for ( Role_Vec::iterator iter = targets.begin(); iter != targets.end(); ++iter ) {
//		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id( (*iter) );
//		if ( player ) {
//			if ( direct_in ) {
//				GANG_MANAGER->add_direct_in_gang( msg.gang_id, player->role_id() );
//			}
//			OBJ_SEND_TO_CLIENT(msg, (*player));
//		}
//	}
//	if ( targets.size() > 1 ) {
//		GANG_MANAGER->update_invite_friend_cd(self->role_id());
//	}
//	res_msg.invitees = targets;
//	THIS_SEND_TO_CLIENT(res_msg);
	//



	for ( Role_Vec::iterator iter = targets.begin(); iter != targets.end(); ++iter ) {
		Logic_Player* player_target = LOGIC_MONITOR->find_logic_player_by_role_id( (*iter) );
		if ( player_target ) {
			if ( direct_in ) {
				GANG_MANAGER->add_direct_in_gang(ganger_detail_->gang_id, player_target->role_id());
			}
			//OBJ_SEND_TO_CLIENT(msg, (*player_target));

			player_target->add_invite_gang_id(ganger_detail_->gang_id);
//			Gang_Apply_Info apply_info;
//			apply_info.role_id = player->role_id();
//			apply_info.role_name = player->role_name();
//			apply_info.level = player->level();
//			apply_info.gender = player->gender();
//			apply_info.career = player->career();
//			//apply_info.vip = client->vip();
//			apply_info.combat_power = player->fighter_detail().fetch_fight_property(PT_FORCE);
//			apply_info.time_sec = Time_Value::gettimeofday().sec();
//			gang_view->apply_infos.push_back(apply_info);
//
//			player_target->insert_apply_id(gang_view->gang_id);

			GANG_MANAGER->active_gang_notify(player_target->role_id(), GNA_HAS_INVITE_TO_INVITEE,
					ganger_detail_->gang_id, GANG_MANAGER->gang_name(ganger_detail_->gang_id));


		}
	}
	if ( targets.size() > 1 ) {
		GANG_MANAGER->update_invite_friend_cd(self->role_id());
	}

	MSG_50170011 res_msg;
	res_msg.gang_id = ganger_detail_->gang_id;
	res_msg.gang_name = GANG_MANAGER->gang_name(ganger_detail_->gang_id);
	res_msg.inviter = self->role_id();
	res_msg.inviter_name = self->role_name();
	res_msg.invitees = targets;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Ganger::req_gang_passed_events() {
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id( ganger_detail_->gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	MSG_50170012 msg;
	gang_view->get_last_n_events(20, msg.passed_events );
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Ganger::req_gang_simple_passed_events() {
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id( ganger_detail_->gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	MSG_50170013 msg;
	msg.simple_events = gang_view->simple_events;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Ganger::req_gang_group_mail(Mail_Info& mail_info) {
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	std::bitset<32> bitset_auth(ganger_detail_->authority);
	if (! bitset_auth.test(GANGER_AU_SEND_GROUP_MAIL)) {
		return ERROR_GANG_NO_AUTHORITY;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}

	// cost money
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY),
			CONFIG_CACHE_GANG->limit().group_mail_cost, MONEY_SUB_GANG_SEND_GROUP_MAIL));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}

	Role_Vec sent_members;
	for (Gang_DB_View::Member_Map::iterator it_member = gang_view->member_map.begin();
			it_member != gang_view->member_map.end(); ++it_member) {
		if (ganger_detail_->role_id != it_member->first) {
			sent_members.push_back(it_member->first);
		}
	}
	if (! sent_members.empty()) {
		Mail_Send_Info send_info;
		send_info.sender_id = ganger_detail_->role_id;
		send_info.send_type = mail_info.mail_type;
		send_info.title = mail_info.mail_title;
		send_info.content = mail_info.mail_content;
		//群发邮件 send_info至少包含(sender_id,发送人  send_type,邮件类型  title,标题  content,内容)
		LOGIC_PUBLIC->send_mail(sent_members, send_info);
	}

	MSG_50170014 res_msg;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_donate(int8_t type, int money) {
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}

	int donate_gain = 0;
	const Gang_Config& gang_cfg_limit = CONFIG_CACHE_GANG->limit();

	if (type == GANG_DONATE_TYPE_GOLD) {
//		uint32_t plus = ganger_detail_->day_gold_contrib_times;
//		uint32_t gold_money = CONFIG_CACHE_GANG->limit().gold_donate +
//				plus*CONFIG_CACHE_GANG->limit().gold_donate_plus;
//		if ( gold_money != money ) return ERROR_GANG_DONATE;
		if (money < gang_cfg_limit.min_donate_copper) {
			return ERROR_GANG_MIN_DONATE_COPPER;
		}
		Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), money, MONEY_SUB_FACTION_DONATE));
		if (money_info.nums > 0) {
			int try_res = player_self()->pack_try_sub_money(money_info);
			if (try_res) {
				return try_res;
			}
		}
		//------- all check passed ------
		// Sub money
		if (money_info.nums > 0) {
			player_self()->pack_sub_money(money_info);
		}

		donate_gain = money * gang_cfg_limit.copper_to_contrib;
		++ganger_detail_->day_gold_contrib_times;
	}
	else if ( type == GANG_DONATE_TYPE_DIAM ) {
//		uint32_t plus = ganger_detail_->day_diam_contrib_times;
//		uint32_t diam_money = CONFIG_CACHE_GANG->limit().diam_donate +
//				plus*CONFIG_CACHE_GANG->limit().diam_donate_plus;
//		if ( diam_money != money ) return ERROR_GANG_DONATE;
		if (money < gang_cfg_limit.min_donate_diam) {
			return ERROR_GANG_MIN_DONATE_DIAM;	// TODO
		}
		Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), money, MONEY_SUB_FACTION_DONATE));
		if (money_info.nums > 0) {
			int try_res = player_self()->pack_try_sub_money(money_info);
			if (try_res) {
				return try_res;
			}
		}
		//------- all check passed ------
		// Sub money
		if (money_info.nums > 0) {
			player_self()->pack_sub_money(money_info);
		}

		donate_gain = money * gang_cfg_limit.diam_to_contrib;
		++ganger_detail_->day_diam_contrib_times;
	} else {
		return ERROR_GANG_DONATE;
	}

	//uint32_t donate_gain = CONFIG_CACHE_GANG->base(gang_view->level).donate_gain;
	ganger_detail_->day_contribute += donate_gain;
	ganger_detail_->contrib_of_week += donate_gain;
	ganger_detail_->total_contribute += donate_gain;
	ganger_detail_->achi_contribute += donate_gain;
	ganger_detail_->cur_contribute += donate_gain;

	ganger_detail_->detail_change();

	// add donate gain in gang fund
	gang_view->fund += donate_gain;
	GANG_MANAGER->update_member_info(GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id), ganger_detail_->role_id);
	GANG_MANAGER->refresh_rank();
	gang_view->change_view();

	// notify ganger info, gang info
	GANG_MANAGER->notify_gang_info_update(*gang_view);
	active_ganger_info_update();
	// notify ganger row info
	GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, role_id(), ganger_detail_->gang_id);

	MSG_50170015 res_msg;
	res_msg.type = type;
	res_msg.money = money;
	res_msg.get_contrib = donate_gain;
	THIS_SEND_TO_CLIENT(res_msg);

	// 监听
	player_self()->achievement_listen_gang_contribute();
	player_self()->title_listen_contribute();
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_GANG_DEDICATE);
	player_self()->task_listen_donate_to_gang();
	return 0;
}

int Ganger::req_set_join_block(int8_t set) {
	if ( ganger_detail_->gang_id == 0 ) return ERROR_GANG_NOT_IN_GANG;
	// authority check
	std::bitset<32> bit_auth( ganger_detail_->authority );
	if ( !bit_auth.test(GANGER_AU_ALLOW_ALL_IN) ) return ERROR_GANG_NO_AUTHORITY;
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id( ganger_detail_->gang_id );
	if ( !gang_view ) return ERROR_GANG_NOT_EXIST;
	if ( gang_view->join_block == set ) return 0;
	gang_view->join_block = set;
	gang_view->change_view();
	// notify gang info
	GANG_MANAGER->notify_gang_info_update( *gang_view );
	// notify gang row info
	GANG_MANAGER->notify_gang_row_info( GANG_ROW_INFO_UPDATE, *gang_view );
	return 0;
}

int Ganger::active_ganger_info_update() {
	MSG_81070004 msg;
	make_ganger_info(msg.ganger_info);
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Ganger::active_ganger_string_info_change(void) {
	String_Info_Change string_info_change;
	string_info_change.type = SICT_GANG;
	string_info_change.value = GANG_MANAGER->gang_name(ganger_detail_->gang_id);
	std::vector<String_Info_Change> change_info;
	change_info.push_back(string_info_change);
	player_self()->active_string_info_change(change_info);
}

int Ganger::req_gang_invite_list(void) {
	if (ganger_detail_->gang_id) {
		return ERROR_GANG_ALREADY_GANGER;
	}
	Gang_Invite_Info invite_info;
	Gang_DB_View* gang_view = NULL;
	MSG_50170017 res_msg;
	Role_Id_Set invite_ids;
	for (Role_Vec::const_iterator it = ganger_detail_->invites.begin();
			it != ganger_detail_->invites.end(); ++it) {
		gang_view = GANG_MANAGER->gang_view_by_id(*it);
		if (gang_view) {
			if (invite_ids.count(gang_view->gang_id)) {
				continue;
			}
			invite_ids.insert(gang_view->gang_id);

			invite_info.reset();
			invite_info.gang_id = gang_view->gang_id;
			invite_info.gang_name = gang_view->name;
			invite_info.leader = LOGIC_MONITOR->get_role_name_by_id(gang_view->leader);
			invite_info.level = gang_view->level;
			invite_info.cur_mem_num = gang_view->member_map.size();
			invite_info.max_mem_num = gang_view->max_members;
			//invite_info.time_sec =
			res_msg.invite_list.push_back(invite_info);
		}
	}

	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Ganger::req_gang_apply_list(void) {
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	std::bitset<32> auth_bit(ganger_detail_->authority);
	if (! auth_bit.test(GANGER_AU_CHECK_IN)) {
		return ERROR_GANG_NO_AUTHORITY;
	}

	int result = GANG_MANAGER->res_gang_apply_list(player_self(), ganger_detail_->gang_id);
	if (result) {
		return result;
	}

	return 0;
}

int Ganger::req_gang_list_action(int64_t gang_id, int8_t action) {
	if (!gang_id || !action) {
		return ERROR_CLIENT_PARAM;
	}
	if (ganger_detail_->gang_id) {
		return ERROR_GANG_ALREADY_GANGER;
	}

	int result = GANG_MANAGER->res_gang_list_action(player_self(), gang_id, action);
	if (result) {
		return result;
	}

	MSG_50170019 res_msg;
	res_msg.gang_id = gang_id;
	res_msg.action = action;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Ganger::req_gang_invite_action(int64_t gang_id, int8_t action) {
	if (!action) {
		return ERROR_CLIENT_PARAM;
	}
	if (ganger_detail_->gang_id) {
		return ERROR_GANG_ALREADY_GANGER;
	}

	MSG_50170020 res_msg;
	res_msg.gang_id = gang_id;
	res_msg.action = action;

	switch (action) {
	case GA_AGREE: {
		if (! gang_id) {
			return ERROR_CLIENT_PARAM;
		}
		Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(gang_id);
		if ( !gang_view ) {
			return ERROR_GANG_NOT_EXIST;
		}
		if ( GANG_IN_ALLALLOW == gang_view->join_block || gang_view->can_direct_in(ganger_detail_->role_id)) {
			int join_res = GANG_MANAGER->join_gang_direct(player_self(), gang_view);
			if (join_res) {
				return join_res;
			}
			res_msg.result = 1;
		} else if (GANG_IN_NEED_AGREE == gang_view->join_block ) {
			Gang_Apply_Info apply_info;
			apply_info.role_id = player_self()->role_id();
			apply_info.role_name = player_self()->role_name();
			apply_info.level = player_self()->level();
			apply_info.gender = player_self()->gender();
			apply_info.career = player_self()->career();
			//apply_info.vip = player_self()->vip();
			apply_info.combat_power = player_self()->fighter_detail().fetch_fight_property(PT_FORCE);
			apply_info.time_sec = Time_Value::gettimeofday().sec();
			gang_view->apply_infos.push_back(apply_info);
			player_self()->insert_apply_id(gang_view->gang_id);
			GANG_MANAGER->active_gang_notify(gang_view->gang_id, GNA_HAS_APPLY_TO_MANAGER, apply_info.role_id, apply_info.role_name);
			res_msg.result = 2;
		}
		break;
	}
	case GA_REFUSE: {
		if (! gang_id) {
			return ERROR_CLIENT_PARAM;
		}
		Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(gang_id);
		if (! gang_view ) {
			return ERROR_GANG_NOT_EXIST;
		}
		gang_view->erase_apply_info(player_self()->role_id());
		player_self()->erase_apply_id(gang_view->gang_id);
		GANG_MANAGER->active_gang_notify(gang_view->gang_id, GNA_REFUSE_TO_INVITER, player_self()->role_id(),
				player_self()->role_name());
		res_msg.result = 1;
		break;
	}
	case GA_AGREE_ALL: {
		break;
	}
	case GA_REFUSE_ALL: {
		for (Role_Vec::iterator it_invite = ganger_detail_->invites.begin();
				it_invite != ganger_detail_->invites.end(); ++it_invite) {
			Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(*it_invite);
			if (! gang_view ) {
				return ERROR_GANG_NOT_EXIST;
			}
			gang_view->erase_apply_info(player_self()->role_id());
			player_self()->erase_apply_id(gang_view->gang_id);
			GANG_MANAGER->active_gang_notify(gang_view->gang_id, GNA_REFUSE_TO_INVITER, player_self()->role_id(),
					player_self()->role_name());
		}
		res_msg.result = 1;
		break;
	}
	default: {
		break;
	}
	}

	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_apply_action(int64_t role_id, int8_t action) {
	if (! action) {
		return ERROR_CLIENT_PARAM;
	}
	if (!role_id && (action != GA_AGREE_ALL && action != GA_REFUSE_ALL)) {
		return ERROR_CLIENT_PARAM;
	}
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	std::bitset<32> bitset_auth(ganger_detail_->authority);
	if (! bitset_auth.test(GANGER_AU_CHECK_IN)) {
		return ERROR_GANG_NO_AUTHORITY;
	}

	MSG_50170021 res_msg;
	int result = GANG_MANAGER->res_gang_apply_list_action(player_self(), ganger_detail_->gang_id, action, role_id);
	if (result) {
		return result;
	}

	res_msg.role_id = role_id;
	res_msg.action = action;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_ganger_change_headship_auth(int64_t role_id, int8_t headship, Int8_Vec &add_auth_vec) {
	if (!role_id) {
		return ERROR_CLIENT_PARAM;
	}
	std::bitset<32> bitset_auth(ganger_detail_->authority);
	if (! bitset_auth.test(GANGER_AU_APPOINT)) {
		return ERROR_GANG_NO_AUTHORITY;
	}

	Logic_Player* target_player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	Player_DB_View* target_view = NULL;
	if (! target_player) {
		target_view = LOGIC_MONITOR->find_player_db_view_by_role_id(role_id);
		if (! target_view) {
			return ERROR_CLIENT_PARAM;
		}
	}

	/*
	 * !! 以下使用target_player要判断
	 */

	role_id_t target_player_id =  target_player ? target_player->role_id() : target_view->role_id;
	std::string target_player_name =  target_player ? target_player->role_name() : target_view->role_name;

	/*
	 * check add_auth_vec with headship
	 */
	int target_player_auth = 0;
	int check_res = check_add_auth(headship, add_auth_vec, target_player_auth);
	if (check_res) {
		return check_res;
	}

	// 玩家更新
	if (target_player) {
		// set authority
		target_player->ganger_detail_->headship = headship;
		target_player->ganger_detail_->authority = target_player_auth;
		GANG_MANAGER->update_member_info(GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id), target_player->role_id());
		// 玩家信息主动更新
		target_player->active_ganger_info_update();
		target_player->ganger_refresh_base_detail();
	} else {
		// offline player operate
	}

	// 公会更新
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	GANG_MANAGER->req_gang_change_headship_auth(gang_view, target_player_id, headship, target_player_auth);

	//ganger_refresh_base_detail();

	// 公会信息主动通知/更新
	GANG_MANAGER->active_gang_notify(target_player_id, GNA_HEADSHIP_AUTH_TO_ASSIGNEE,
			 player_self()->role_id(), player_self()->role_name(), headship);
	GANG_MANAGER->notify_gang_info_update(*gang_view);
	GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, role_id, ganger_detail_->gang_id);

	// add passed event
	gang_view->add_passed_event_101901(player_self()->role_id(), role_id, headship);
	gang_view->send_last_passed_event();

	player_self()->create_ann_gang_headship_appoint(player_self()->role_id(), role_id, headship);

	MSG_50170022 res_msg;
	res_msg.member_role_name = target_player_name;
	res_msg.headship = headship;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_upgrade(void) {
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	std::bitset<32> bitset_auth(ganger_detail_->authority);
	if (! bitset_auth.test(GANGER_AU_UPGRADE_GANG)) {
		return ERROR_GANG_NO_AUTHORITY;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}

	int res = GANG_MANAGER->req_gang_upgrade(ganger_detail_->role_id, gang_view);
	if (res) {
		return res;
	}

	// remain_draw_times has changed
	for (Gang_DB_View::Member_Map::const_iterator it_member = gang_view->member_map.begin();
			it_member != gang_view->member_map.end(); ++it_member ) {
		Logic_Player* player_member = LOGIC_MONITOR->find_logic_player_by_role_id(it_member->first);
		if (player_member) {
			player_member->active_ganger_info_update();
		}
	}

	//任务监听公会等级
	player_self()->task_listen_gang_level(gang_view->gang_id, gang_view->level);

	player_self()->active_content_listen_gang_level_up(gang_view->gang_id, gang_view->level);

	MSG_50170023 res_msg;
	res_msg.gang_level = gang_view->level;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_upgrade_send_gift(void) {
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	std::bitset<32> bitset_auth(ganger_detail_->authority);
	if (! bitset_auth.test(GANGER_AU_UPGRADE_GANG)) {
		return ERROR_GANG_NO_AUTHORITY;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}

	// Check money
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY),
			CONFIG_CACHE_GANG->limit().diam_upgrade_gift, MONEY_SUB_GANG_UPGRADE_GIFT));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}

	//------- all check passed ------
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}

	/*
	 * send mail
	 */
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(1003);
	if (! mailc) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Item_Info_Vec& cfg_upgrade_gifts = CONFIG_CACHE_GANG->base(gang_view->level).upgrade_gift;
	std::vector<Item_Basic_Info> item_vector;
	if (! cfg_upgrade_gifts.empty()) {
		Item_Basic_Info mail_item;
		for (uint16_t i = 0; i < cfg_upgrade_gifts.size(); ++i) {
			mail_item.reset();
			mail_item.id = cfg_upgrade_gifts[i].id;
			mail_item.amount = cfg_upgrade_gifts[i].amount;
			mail_item.bind = cfg_upgrade_gifts[i].bind;
			item_vector.push_back(mail_item);
		}
	}
	std::string contents;
	const int size_buf = mailc->mail_content.size() + \
			sizeof(player_self()->role_name()) + 10 + \
			sizeof(gang_view->level) + 10;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), player_self()->role_name().c_str(), gang_view->level);
	contents.assign(content_buf);

	for ( Gang_DB_View::Member_Map::const_iterator it_member = gang_view->member_map.begin();
			it_member != gang_view->member_map.end(); ++it_member ) {
		LOGIC_PUBLIC->send_mail(1003, it_member->first, item_vector, "", contents);
	}

	MSG_50170024 res_msg;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_upgrade_cd_clear(int8_t cd_type, int32_t building_id) {
	if (!cd_type || (cd_type < GCT_BEGIN || cd_type >= GCT_END) ||
			(cd_type == GCT_GANG_LVL && building_id) ||
			(cd_type == GCT_BUILDING_LVL && (building_id < BUILDING_BEGIN || building_id >= BUILDING_END))) {
		return ERROR_CLIENT_PARAM;
	}

	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	std::bitset<32> bitset_auth(ganger_detail_->authority);
	if (! bitset_auth.test(GANGER_AU_UPGRADE_GANG)) {
		return ERROR_GANG_NO_AUTHORITY;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}

	int res_clear = 0;
	switch (cd_type) {
	case GCT_GANG_LVL: {
		res_clear = clear_cd_gang_lvl(gang_view);
		break;
	}
	case GCT_BUILDING_LVL: {
		res_clear = clear_cd_building_lvl(gang_view, building_id);
		break;
	}
	default:
		break;
	}

	if (res_clear) {
		return res_clear;
	}

	MSG_50170025 res_msg;
	res_msg.result = 1;
	res_msg.cd_type = cd_type;
	res_msg.building_id = building_id;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::clear_cd_gang_lvl(Gang_DB_View* gang_view) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	const Time_Value& now = Time_Value::gettimeofday();
	if (gang_view->cd_upgrade <= now) {
		return -1;
	}

	// check money
	double time_diff = (gang_view->cd_upgrade - now).sec();
	int diamond_num = ceil(time_diff / 3600.0) * CONFIG_CACHE_GANG->limit().upgrade_cd_clear_rate;
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY),
			diamond_num, MONEY_SUB_GANG_CLEAR_CD_GANG_LVL));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}

	gang_view->cd_upgrade = now;

	gang_view->change_view();
	GANG_MANAGER->notify_gang_info_update(*gang_view);
	GANG_MANAGER->notify_gang_row_info(GANG_ROW_INFO_UPDATE, *gang_view);

	//GANG_MANAGER->gang_lvl_clear_cd(gang_view);
	return 0;
}

int Ganger::clear_cd_building_lvl(Gang_DB_View* gang_view, int building_id) {
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	Gang_Building_Map::iterator it_building = gang_view->buildings.find(building_id);
	if (it_building == gang_view->buildings.end()) {
		return ERROR_INNER_SERVER;
	}
	const Time_Value& now = Time_Value::gettimeofday();
	if (it_building->second.cd_upgrade <= now) {
		return -1;
	}

	// check money
	double time_diff = (it_building->second.cd_upgrade - now).sec();
	int diamond_num = ceil(time_diff / 3600.0) * CONFIG_CACHE_GANG->limit().upgrade_cd_clear_rate;
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY),
			diamond_num, MONEY_SUB_GANG_CLEAR_CD_BUILDING_LVL));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}

	it_building->second.cd_upgrade = now;

	gang_view->change_view();
	GANG_MANAGER->notify_gang_info_update(*gang_view);

	return 0;
}

int Ganger::req_gang_world_notice(void) {
	int diamond_num = CONFIG_CACHE_GANG->limit().cost_notice;
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY),
			diamond_num, MONEY_SUB_GANG_WORLD_NOTICE));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}

	MSG_50170026 res_msg;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_logo_modify(int8_t logo_id) {
	if (! logo_id) {
		return ERROR_CLIENT_PARAM;
	}
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	if (ganger_detail_->role_id != gang_view->leader) {
		return ERROR_GANG_NOT_LEADER;
	}

	gang_view->logo = logo_id;

	GANG_MANAGER->notify_gang_info_update(*gang_view);
	GANG_MANAGER->notify_gang_row_info(GANG_ROW_INFO_UPDATE, *gang_view);

	MSG_50170027 res_msg;
	res_msg.logo_id = logo_id;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_seize_power(void) {
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	if (ganger_detail_->headship == GANG_HEADSHIP_LEADER) {
		return ERROR_GANG_IS_LEADER;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	if (LOGIC_MONITOR->find_logic_player_by_role_id(gang_view->leader)) {
		return ERROR_GANG_LEADER_ONLINE;
	}
	if (! gang_view->can_seize_power) {
		return ERROR_GANG_SEIZE_TIME_LACK;
	}

	// Check gold
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY),
			CONFIG_CACHE_GANG->limit().change_chief, MONEY_SUB_GANG_SEIZE_POWER));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}

	//-------------- all check passed -------------------
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}

	role_id_t old_leader_id = gang_view->leader;

	/*
	 * gang_view operate
	 */
	// member.self
	Gang_DB_View::Member_Map::iterator it_member_self = gang_view->member_map.find(ganger_detail_->role_id);
	if (it_member_self != gang_view->member_map.end()) {
		it_member_self->second.headship = GANG_HEADSHIP_LEADER;
		it_member_self->second.authority = CONFIG_CACHE_GANG->power(GANG_HEADSHIP_LEADER).auth_to_bit();
	}
	// member.old_leader
	Gang_DB_View::Member_Map::iterator it_member_old_leader = gang_view->member_map.find(old_leader_id);
	if (it_member_old_leader != gang_view->member_map.end()) {
		it_member_old_leader->second.headship = GANG_HEADSHIP_NORMAL;
		it_member_old_leader->second.authority = CONFIG_CACHE_GANG->power(GANG_HEADSHIP_NORMAL).auth_to_bit();
	}
	gang_view->last_leader_id = gang_view->leader;
	gang_view->leader = ganger_detail_->role_id;
	gang_view->change_view();

	/*
	 * ganger operate
	 */
	// ganger
	this->promote_to_headship(GANG_HEADSHIP_LEADER);
	this->active_ganger_info_update();
	EXPEDITION_MANAGER->listen_gang_leader_change(player_self()->base_detail());
	// ganger_old_leader
	Logic_Player* player_old_leader = LOGIC_MONITOR->find_logic_player_by_role_id(old_leader_id);
	if (player_old_leader) {
		player_old_leader->promote_to_headship(GANG_HEADSHIP_NORMAL);
		player_old_leader->active_ganger_info_update();
	} else {
		// module_init_headship_auth() in module_init();
	}

	// notify
	GANG_MANAGER->notify_gang_info_update(*gang_view);
	GANG_MANAGER->notify_gang_row_info(GANG_ROW_INFO_UPDATE, *gang_view);
	GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, ganger_detail_->role_id, ganger_detail_->gang_id);
	GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, old_leader_id, ganger_detail_->gang_id);

	gang_view->add_passed_event_101907(ganger_detail_->role_id, old_leader_id);
	gang_view->send_last_passed_event();

	player_self()->create_ann_gang_seize_power(ganger_detail_->role_id, old_leader_id);

	MSG_50170028 res_msg;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_send_bless_buf(role_id_t target_role_id, int32_t bless_buf_id) {
	if (!bless_buf_id || !target_role_id) {
		return ERROR_CLIENT_PARAM;
	}
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	Logic_Player* target_player = LOGIC_MONITOR->find_logic_player_by_role_id(target_role_id);
	if (! target_player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	if (! target_player->ganger_detail().gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	if (target_player->ganger_detail().receive_bless_times >= 3) {
		return ERROR_GANGER_MAX_TIMES_BE_BLESS;
	}

	/*
	 * check condition in scene server
	 */
	role_id_t sender_role_id = role_id();
	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_SEND_BLESS_BUF);
	inner_buf.write_int64(sender_role_id);
	inner_buf.write_int32(bless_buf_id);
	inner_buf.finish_message();
	target_player->send_to_scene(inner_buf);

	// inner_send_bless_buf(bless_buf_id, has_buf_id);

	return 0;
}

int Ganger::inner_send_bless_buf(role_id_t sender_role_id, int32_t bless_buf_id, bool has_buf_id) {
	if (! bless_buf_id) {
		return ERROR_INNER_SERVER;
	}
	if (has_buf_id) {
		return ERROR_GANGER_IN_BLESS_BUF;
	}

	Logic_Player* target_player = player_self();
	Logic_Player* sender_player = LOGIC_MONITOR->find_logic_player_by_role_id(sender_role_id);
	if (! sender_player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	if (! sender_player->ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	if (! target_player->ganger_detail().gang_id) {
		return ERROR_GANG_NO_MEMBER;
	}

//	Logic_Player* target_player = LOGIC_MONITOR->find_logic_player_by_role_id(target_role_id);
//	if (! target_player) {
//		return ERROR_PLAYER_NOT_ONLINE;
//	}
//	if (! target_player->ganger_detail().gang_id) {
//		return ERROR_GANG_NOT_IN_GANG;
//	}

	/*
	 * check condition in logic server
	 */
	const Gang_Cfg_Bless_Buf_Id *cfg_bless_buf = CONFIG_CACHE_GANG->find_bless_buf(bless_buf_id);
	if (! cfg_bless_buf) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Gang_Cfg_Role_Bless *cfg_role_bless = CONFIG_CACHE_GANG->find_role_bless(target_player->level());
	if (! cfg_role_bless) {
		return ERROR_CONFIG_NOT_EXIST;
	}


	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY),
			cfg_role_bless->need_gold, MONEY_SUB_GANG_SEND_BLESS_BUF));
	if (money_info.nums > 0) {
		int try_res = sender_player->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}

	// --------- all check passed -------
	// Sub money
	if (money_info.nums > 0) {
		sender_player->pack_sub_money(money_info);
	}

	target_player->add_receive_bless_times();

	Effect_Argv_Prop effect_prop;
	effect_prop.i_effect_val1 = cfg_bless_buf->effect_prop;
	effect_prop.d_effect_val1 = cfg_bless_buf->effect_val;

	Status_Effect_Argv effect_argv;
	effect_argv.status_id = cfg_bless_buf->bless_buf_id;
	effect_argv.last_time_ms = cfg_bless_buf->effect_time.msec();
	effect_argv.data.push_back(effect_prop);

	target_player->logic_insert_status(effect_argv);

	target_player->active_ganger_info_update();

	GANG_MANAGER->active_gang_notify(target_player->role_id(), GNA_BLESS_BUF_TO_ACCEPTER,
			sender_player->role_id(), sender_player->role_name(), bless_buf_id);

	Block_Buffer res_buf;
	MSG_50170029 res_msg;
	res_msg.bless_buf_id = bless_buf_id;
	res_msg.result = 1;
	res_buf.make_message(res_msg.msg_id);
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	sender_player->send_to_client(res_buf, true);

	sender_player->task_listen_present_buf_to_other();
	return 0;
}

int Ganger::req_research_gang_skill(int32_t skill_id) {
	if (! skill_id) {
		return ERROR_CLIENT_PARAM;
	}
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	const Gang_Cfg_Skill_Id *cfg_skill = CONFIG_CACHE_GANG->find_skill(skill_id);
	if (! cfg_skill) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	Gang_Building_Map::iterator it_building = gang_view->buildings.find(TOWER);
	if (it_building == gang_view->buildings.end()) {
		return ERROR_INNER_SERVER;
	}
	if (it_building->second.building_lvl < cfg_skill->need_tower_lvl) {
		return ERROR_GANG_TOWER_LVL_LOW;
	}
	if (gang_view->fund < cfg_skill->research_cost_fund) {
		return ERROR_GANG_NOTENOUGH_MONEY;
	}
	if (gang_view->researched_skills.count(skill_id)) {
		return ERROR_SKILL_HAS_LEARNED;
	}

	gang_view->fund -= cfg_skill->research_cost_fund;

	Gang_Skill_Detail skill_detail;
	skill_detail.skill_id = skill_id;
	gang_view->researched_skills.insert(std::make_pair(skill_detail.skill_id, skill_detail));

	gang_view->change_view();
	GANG_MANAGER->notify_gang_info_update(*gang_view);

	MSG_50170030 res_msg;
	res_msg.skill_id = skill_id;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Ganger::req_learn_ganger_skill(int32_t skill_id) {
	if (! skill_id) {
		return ERROR_CLIENT_PARAM;
	}
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	if (ganger_detail_->skill_map.count(skill_id)) {
		return ERROR_SKILL_HAS_LEARNED;
	}

	const Gang_Cfg_Skill_Id *cfg_skill = CONFIG_CACHE_GANG->find_skill(skill_id);
	if (! cfg_skill) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	if (! gang_view->researched_skills.count(skill_id)) {
		return ERROR_GANG_SKILL_NOT_RESEARCH;
	}

	Gang_Building_Map::iterator it_building = gang_view->buildings.find(TOWER);
	if (it_building == gang_view->buildings.end()) {
		return ERROR_INNER_SERVER;
	}
	if (it_building->second.building_lvl < cfg_skill->need_tower_lvl) {
		return ERROR_GANG_TOWER_LVL_LOW;
	}
	if (player_self()->level() < cfg_skill->need_player_lvl) {
		return ERROR_ROLE_LV_LIM;
	}

	if (ganger_detail_->cur_contribute < cfg_skill->learn_cost_fund) {
		return ERROR_GANGER_CONTRIBUTE_LACK;
	}

	// sub contribute
	ganger_detail_->cur_contribute -= cfg_skill->learn_cost_fund;

	// insert skill_map in ganger_detail_
	Ganger_Skill_Detail skill_detail;
	skill_detail.skill_id = skill_id;
	ganger_detail_->skill_map.insert(std::make_pair(skill_detail.skill_id, skill_detail));

	ganger_prop_refresh();
	player_self()->heroer_prop_refresh();

	ganger_detail_->detail_change();
	active_ganger_info_update();

	// listening
	player_self()->task_listen_ganger_skill_level(skill_id);

	MSG_50170031 res_msg;
	res_msg.skill_id = skill_id;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Ganger::req_gang_draw(void) {
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	int remain_draw_times = get_remain_draw_times();
	if (remain_draw_times < 1) {
		return ERROR_GANGER_DRAW_TIMES_LACK;
	}

	int draw_cost_contrib = CONFIG_CACHE_GANG->base(gang_view->level).draw_cost_contrib;
	if (ganger_detail_->cur_contribute < draw_cost_contrib) {
		return ERROR_GANGER_CONTRIBUTE_LACK;
	}

	Int_Vec rate_vec;
	const Int_Vec& cfg_draw_box = CONFIG_CACHE_GANG->base(gang_view->level).draw_box;
	for (uint16_t i = 0; i < cfg_draw_box.size(); ++i) {
		const Gang_Cfg_Draw_Card* cfg_draw_card = CONFIG_CACHE_GANG->find_draw_card(cfg_draw_box[i]);
		if (! cfg_draw_card) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		rate_vec.push_back(cfg_draw_card->rate);
	}

	int rand_index = U_FUNC::get_rand_index(rate_vec);
	if (rand_index < 0) {
		return ERROR_INNER_SERVER;
	}
	int draw_card_id = cfg_draw_box[rand_index];

	const Gang_Cfg_Draw_Card* cfg_draw_card = CONFIG_CACHE_GANG->find_draw_card(draw_card_id);
	if (!cfg_draw_card || !cfg_draw_card->item_id || cfg_draw_card->item_amount < 0) {
		return ERROR_CONFIG_NOT_EXIST;
	}
//	Item_Detail draw_item(cfg_draw_card->item_id, cfg_draw_card->item_amount, 1);
//	int insert_ret = player_self()->pack_try_insert_item(PACK_T_PACKAGE_INDEX, draw_item);
//	if (insert_ret) {
//		return insert_ret;
//	}

	// --------------------- all check pass -------------------------
	//--ganger_detail_->remain_draw_times;
	++ganger_detail_->today_has_draw_times;
	ganger_detail_->cur_contribute -= draw_cost_contrib;

	ganger_detail_->draw_awards.push_back(cfg_draw_card->draw_card_id);
	//player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, draw_item, Gain_Item_DM_Info(ITEM_GAIN_TEST));

	ganger_detail_->detail_change();
	active_ganger_info_update();
	//GANG_MANAGER->notify_gang_row_info( GANG_ROW_INFO_UPDATE, *gang_view);

	// announce
	if (cfg_draw_card->is_rare) {
		Chat_Item_Info chat_item_info;
		chat_item_info.item_base_info.id = cfg_draw_card->item_id;
		chat_item_info.item_base_info.amount = cfg_draw_card->item_amount;
		chat_item_info.item_base_info.bind = Item_Detail::BIND;
		player_self()->create_ann_gang_draw(chat_item_info);
	}

	// listening
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_GANG_LOTTERY);
	player_self()->task_listen_ganger_bless_buf();

	MSG_50170032 res_msg;
	res_msg.draw_card_id = cfg_draw_card->draw_card_id;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_take_draw_item(void) {
	if (ganger_detail_->draw_awards.empty()) {
		return ERROR_GANGER_NO_DRAW_ITEM;
	}
//	if (player_self()->pack_get_remain_capacity(PACK_T_PACKAGE_INDEX) < (int)ganger_detail_->draw_awards.size()) {
//		return ERROR_PACK_NOT_ENOUGH;
//	}

	MSG_81000102 acv_msg;
	acv_msg.type = 0;

	Item_Vec item_vec;
	for (uint16_t i = 0; i < ganger_detail_->draw_awards.size(); ++i) {
		const Gang_Cfg_Draw_Card* cfg_draw_card = CONFIG_CACHE_GANG->find_draw_card(ganger_detail_->draw_awards[i]);
		if (!cfg_draw_card || !cfg_draw_card->item_id || cfg_draw_card->item_amount < 0) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		Item_Detail draw_item(cfg_draw_card->item_id, cfg_draw_card->item_amount, Item_Detail::BIND);
		item_vec.push_back(draw_item);
		Item_Basic_Info item_base;
		draw_item.set_item_basic_info(item_base);
		acv_msg.item_info.push_back(item_base);
	}
	if (! item_vec.empty()) {
		int insert_ret = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item_vec,
				Gain_Item_DM_Info(ITEM_GAIN_GANG_DRAW));
		if (insert_ret && ERROR_PACK_FULL_BUT_MAIL != insert_ret) {
			return insert_ret;
		}
		if (!acv_msg.item_info.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		ganger_detail_->draw_awards.clear();
	}

	ganger_detail_->detail_change();
	active_ganger_info_update();

	MSG_50170033 res_msg;
	res_msg.result = item_vec.empty() ?  0 : 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

//int Ganger::req_gang_take_draw_item(void) {
//	if (ganger_detail_->draw_awards.empty()) {
//		return ERROR_GANGER_NO_DRAW_ITEM;
//	}
//
//	int pack_cap = player_self()->pack_get_remain_capacity(PACK_T_PACKAGE_INDEX);
//	if (pack_cap < 1) {
//		return ERROR_PACK_NOT_ENOUGH;
//	}
//
//	int draw_awards_size = ganger_detail_->draw_awards.size();
//	int cap_after_take = pack_cap - draw_awards_size;
//	int cnt_sent_mail = 0;
//	int vec_advance = 0;
//	if (cap_after_take < 0) {
//		cnt_sent_mail = abs(cap_after_take);
//		vec_advance = draw_awards_size - cnt_sent_mail;
//	}
//
//	Int_Vec::iterator it_advance = ganger_detail_->draw_awards.begin();
//	if (vec_advance) {
//		// insert package
//
//		std::advance(it_advance, vec_advance);
//
//		Item_Vec item_vec;
//		MSG_81000102 acv_msg;
//		acv_msg.type = 0;
//		//for (uint16_t i = 0; i < ganger_detail_->draw_awards.size(); ++i) {
//		for (Int_Vec::iterator it = ganger_detail_->draw_awards.begin(); it != it_advance; ++it) {
//			const Gang_Cfg_Draw_Card* cfg_draw_card = CONFIG_CACHE_GANG->find_draw_card(*it);
//			if (!cfg_draw_card || !cfg_draw_card->item_id || cfg_draw_card->item_amount < 0) {
//				return ERROR_CONFIG_NOT_EXIST;
//			}
//			Item_Detail draw_item(cfg_draw_card->item_id, cfg_draw_card->item_amount, Item_Detail::BIND);
//			item_vec.push_back(draw_item);
//			Item_Basic_Info item_base;
//			draw_item.set_item_basic_info(item_base);
//			acv_msg.item_info.push_back(item_base);
//		}
//		if (! item_vec.empty()) {
//			int insert_ret = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item_vec,
//					Gain_Item_DM_Info(ITEM_GAIN_TEST));
//			if (insert_ret) {
//				return insert_ret;
//			}
//			if (!acv_msg.item_info.empty()) {
//				THIS_SEND_TO_CLIENT(acv_msg);
//			}
//			//ganger_detail_->draw_awards.clear();
//		}
//
//
//	} else {
//		// send mail
//		Item_Vec item_vec;
//		for (Int_Vec::iterator it = it_advance; it != ganger_detail_->draw_awards.end(); ++it) {
//			const Gang_Cfg_Draw_Card* cfg_draw_card = CONFIG_CACHE_GANG->find_draw_card(*it);
//			if (!cfg_draw_card || !cfg_draw_card->item_id || cfg_draw_card->item_amount < 0) {
//				return ERROR_CONFIG_NOT_EXIST;
//			}
//			Item_Detail draw_item(cfg_draw_card->item_id, cfg_draw_card->item_amount, Item_Detail::BIND);
//			item_vec.push_back(draw_item);
//		}
//
//		if (! item_vec.empty()) {
//			//item_vec
//			//ganger_detail_->draw_awards.clear();
//			/*
//			 * send mail
//			 */
//			const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(1005);
//			if (! mailc) {
//				return ERROR_CONFIG_NOT_EXIST;
//			}
////			if (mailc->item_vector.empty() || !mailc->item_vector[0u].id || !mailc->item_vector[0u].amount) {
////				return ERROR_CONFIG_NOT_EXIST;
////			}
//
//		}
//	}
//
//	ganger_detail_->draw_awards.clear();
//
//	ganger_detail_->detail_change();
//	active_ganger_info_update();
//
//	MSG_50170033 res_msg;
//	res_msg.result = 1;
//	THIS_SEND_TO_CLIENT(res_msg);
//	return 0;
//}

int Ganger::req_gang_building_upgrade(int32_t building_id) {
	if (building_id < BUILDING_BEGIN || building_id >= BUILDING_END) {
		return ERROR_CLIENT_PARAM;
	}
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	std::bitset<32> bitset_auth(ganger_detail_->authority);
	if (! bitset_auth.test(GANGER_AU_UPGRADE_ADDON)) {
		return ERROR_GANG_NO_AUTHORITY;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	Gang_Building_Map::iterator it_building = gang_view->buildings.find(building_id);
	if (it_building == gang_view->buildings.end()) {
		return ERROR_INNER_SERVER;
	}
	if (it_building->second.building_lvl >= MAX_GANG_LEVEL) {
		return ERROR_RUNE_HOLE_LVL_MAX;	// TODO error code
	}
	if (it_building->second.building_lvl >= gang_view->level) {
		return ERROR_GANG_BUILDING_LVL_LT_GANG_LVL;
	}

	// check fund
	int cost_contrib = 0;
	switch (building_id) {
	case TOWER:
		cost_contrib = CONFIG_CACHE_GANG->base(it_building->second.building_lvl).tower_up_cost_contrib;
		break;
	case ALTAR:
		cost_contrib = CONFIG_CACHE_GANG->base(it_building->second.building_lvl).altar_up_cost_contrib;
		break;
	case SHOP:
		cost_contrib = CONFIG_CACHE_GANG->base(it_building->second.building_lvl).shop_upgrade_contrib;
		break;
	default:
		break;
	}
	if (gang_view->fund < cost_contrib) {
		return ERROR_GANG_CONTRIBUTE_LACK;
	}

	gang_view->fund -= cost_contrib;

	++it_building->second.building_lvl;

	//---- 以下 building_lvl 已变 ----
	// upgrade cd
	it_building->second.cd_upgrade = Time_Value::gettimeofday();
	switch (building_id) {
	case TOWER:
		it_building->second.cd_upgrade += CONFIG_CACHE_GANG->base(it_building->second.building_lvl).tower_upgrade_cd;
		break;
	case ALTAR:
		it_building->second.cd_upgrade += CONFIG_CACHE_GANG->base(it_building->second.building_lvl).altar_upgrade_cd;
		break;
	case SHOP:
		it_building->second.cd_upgrade += CONFIG_CACHE_GANG->base(it_building->second.building_lvl).shop_upgrade_cd;
		break;
	default:
		break;
	}

	gang_view->change_view();
	GANG_MANAGER->notify_gang_info_update(*gang_view);

	MSG_50170034 res_msg;
	res_msg.building_id = building_id;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_shop_buy_item(const int item_id, const int amount) {
	if (!item_id || amount < 0) {
		return ERROR_CLIENT_PARAM;
	}
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	const Gang_Cfg_Shop_Item *cfg_shop_item = CONFIG_CACHE_GANG->find_shop_item(item_id);
	if (! cfg_shop_item) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_id);
	if (! item_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

//	if (player_self()->pack_get_remain_capacity(PACK_T_PACKAGE_INDEX) < (int)ganger_detail_->draw_awards.size()) {
//		return ERROR_PACK_NOT_ENOUGH;
//	}

	int total_cost_contrib = cfg_shop_item->cost_contrib * amount;
	if (ganger_detail_->cur_contribute < total_cost_contrib) {
		return ERROR_GANGER_CONTRIBUTE_LACK;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	Gang_Building_Map::iterator it_building = gang_view->buildings.find(SHOP);
	if (it_building == gang_view->buildings.end()) {
		return ERROR_INNER_SERVER;
	}
	if (it_building->second.building_lvl < cfg_shop_item->need_shop_lvl) {
		return ERROR_GANG_SHOP_LVL_LOW;
	}

	Item_Detail buy_item(item_id, amount, Item_Detail::BIND);
	int ret_try = player_self()->pack_try_insert_item(PACK_T_PACKAGE_INDEX, buy_item);
	if (ret_try) {
		return ret_try;
	}

	// --------------------- all check pass -------------------------
	int insert_ret = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, buy_item, Gain_Item_DM_Info(ITEM_GAIN_GANG_SHOP));
	if (insert_ret) {
		return insert_ret;
	}
	{
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		Item_Basic_Info ibi;
		buy_item.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	ganger_detail_->cur_contribute -= total_cost_contrib;

	active_ganger_info_update();

	ganger_detail_->detail_change();

	MSG_50170041 res_msg;
	res_msg.item_id = item_id;
	res_msg.amount = amount;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Ganger::req_gang_war_enter(void) {
	if (!GANG_MANAGER->gang_war_is_open()) {
		return ERROR_GANG_WAR_ACTIVITY_NOT_OPEN;
	}
	if (this->ganger_detail_->gang_id == 0) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	if (CONFIG_CACHE_GANG->gang_war_setting_cfg().level_limit > level()) {
		return this->send_tip_to_client(ERROR_TEAM_SPORTS_LEVEL_LIMIT, CONFIG_CACHE_GANG->gang_war_setting_cfg().level_limit);;
	}
//	int64_t matched_index = GANG_MANAGER->gang_war_get_next_index(this->ganger_detail_->gang_id);
//	if (matched_index != -1) {
//		const Gang_War_Spot *result_info = GANG_MANAGER->gang_war_find_result_map(matched_index);
//		if (NULL != result_info) {
//			for (std::vector<Gang_Basic_Detail>::const_iterator it_gbd = result_info->gang_vec.begin();
//					it_gbd != result_info->gang_vec.end(); ++it_gbd) {
//				if (it_gbd->gang_id == this->ganger_detail_->gang_id) {
//					return ERROR_GANG_WAR_SINGLE_SPOT;
//					break;
//				}
//			}
//			return ERROR_GANG_WAR_NOT_PERMIT;
//		}
//	}
	const Gang_War_Spot *result_info = GANG_MANAGER->gang_war_find_result_map(this->ganger_detail_->gang_id);
	if (NULL == result_info || result_info->result != 0) {
		if (result_info && result_info->result != 0) {
			return ERROR_GANG_WAR_SELF_OVER;
		}
		return ERROR_GANG_WAR_NOT_PERMIT;
	}
	const Gang_War_Matched_Gangs_Pair *pr = GANG_MANAGER->gang_war_find_matched_gangs_pair(this->ganger_detail_->gang_id);
	if (NULL == pr || pr->matched_pair.first == 0 || pr->matched_pair.second == 0) {
		if (pr) {
			player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_GANG_WAR);
			return ERROR_GANG_WAR_SINGLE_SPOT;
		}
		return ERROR_GANG_WAR_NOT_PERMIT;
	}
	const Gang_War_Matched_Gang_Info *info = GANG_MANAGER->gang_war_find_matched_gang_info(this->ganger_detail_->gang_id);
	if (NULL == info) {
		MSG_USER("GANG_WAR: cant gang_war_find_matched_gang_info");
		return ERROR_GANG_WAR_NOT_PERMIT;
	}

	if (this->ganger_detail_->gang_war_attend_time.sec() == info->match_time.sec()
			&& ((this->ganger_detail_->gang_war_attend_gang_id != 0
					&& (this->ganger_detail_->gang_war_attend_gang_id != this->ganger_detail_->gang_id))
					)) {
		return ERROR_GANG_WAR_ALREADY_ATTEND;
	}

	MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`)" " VALUES (%d,%ld,%ld,%d)",
			3,  player_self()->role_id(), Time_Value::gettimeofday().sec(), player_self()->level());

	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_GANG_WAR);

	this->ganger_detail_->gang_war_camp = info->camp;

	int scene_id = GANG_WAR_SCENE_ID;
	player_self()->set_create_scene_args(0, Time_Value::zero, info->scene_index, 0, CONFIG_CACHE->server_maintainer_cache().game_inter_flag, info->matched_index, true);
	//
	Coord coord;
	const Scene_Config* scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if(scene_config){
		Scene_Config::Transmit_Coord_Map::const_iterator it;
		it = scene_config->transmit_coord.find(info->camp);
		if(it != scene_config->transmit_coord.end()){
			coord = it->second.coord;
		}
	}
	player_self()->inner_transmit(scene_id, 0, coord, role_id(), "");
	sync_ganger_war_info_to_scene();

	this->ganger_detail_->gang_war_attend_time = info->match_time;
	this->ganger_detail_->gang_war_attend_gang_id = this->ganger_detail_->gang_id;
	ganger_detail_->detail_change();
	MSG_50170035 res_msg;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Ganger::req_gang_war_award(void) {
	MSG_50170038 res_msg;
	Gang_DB_View* gbv = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (gbv) {
		for (boost::unordered_map<int64_t, Reward_Info_Gang_War>::const_iterator it = gbv->gang_war_award.begin();
				it != gbv->gang_war_award.end(); ++it) {
			res_msg.list.push_back(it->second);
		}
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Ganger::req_gang_war_award_assign(Reward_Info_Gang_War &rigw) {
	if (rigw.role_id <= 0) {
		return ERROR_CLIENT_OPERATE;
	}
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	MSG_50170039 res_msg;
	Gang_DB_View* gbv = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (gbv && gbv->leader == role_id()) {
		boost::unordered_map<int64_t, Reward_Info_Gang_War>::iterator it = gbv->gang_war_award.find(rigw.index);
		if (it != gbv->gang_war_award.end()) {

			{
				int gang_war_award_get_times_by_day = 0;
				Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(rigw.role_id);
				if (pl) {
					if (pl->ganger_detail().gang_war_award_get_times_by_day < CONFIG_CACHE_GANG->gang_war_setting_cfg().box_limit) {
						pl->gang_war_add_gang_war_award_get_times_by_day(1);
						gang_war_award_get_times_by_day = pl->ganger_detail().gang_war_award_get_times_by_day;
					} else {
						return ERROR_CLIENT_OPERATE;
					}
					pl->ganger_detail_no_const().detail_change();
					DB_View::ID_Player_View_Map::iterator iter =
								LOGIC_MONITOR->db_view()->id_player_view_map.find(rigw.role_id);
					if (iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
						iter->second->gang_war_award_get_times_by_day = pl->ganger_detail().gang_war_award_get_times_by_day;
					}
				} else {
					DB_View::ID_Player_View_Map::iterator iter =
								LOGIC_MONITOR->db_view()->id_player_view_map.find(rigw.role_id);
					if (iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()
							&& iter->second->gang_war_award_get_times_by_day < CONFIG_CACHE_GANG->gang_war_setting_cfg().box_limit) {
						iter->second->gang_war_award_get_times_by_day ++;
						gang_war_award_get_times_by_day = iter->second->gang_war_award_get_times_by_day;
					} else {
						return ERROR_CLIENT_OPERATE;
					}
					Ganger_Detail detail;
					detail.set_gang_war_award_get_times_by_day(gang_war_award_get_times_by_day);
					Operating::offline_ganger_info_update(rigw.role_id, detail);
				}
			}

			Item_Detail item (it->second.award.reward_id, it->second.award.quantity, Item_Detail::BIND);

			Item_Vec item_list;
			item_list.push_back(item);
			int ret = LOGIC_PUBLIC->send_mail(GANG_WAR_AWARD_ASSIGN, rigw.role_id, item_list);

			if (0 == ret) {
				it->second.role_id = rigw.role_id;
				it->second.role_name = rigw.role_name;
				gbv->change_view();

				res_msg.award.role_id = it->second.role_id;
				res_msg.award.role_name = it->second.role_name;
				res_msg.award.award.reward_id = it->second.award.reward_id;
				res_msg.award.award.quantity = it->second.award.quantity;
				res_msg.award.award.vip_add = it->second.award.vip_add;
				res_msg.award.index = it->second.index;
			}
		}
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Ganger::gang_war_add_gang_war_award_get_times_by_day(const uint16_t i) {
	ganger_detail_->gang_war_award_get_times_by_day+= i;
	ganger_detail_->detail_change();
	return 0;
}
int Ganger::req_gang_war_draw(void) {
	Gang_DB_View* gbv = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (gbv && (this->ganger_detail_->headship == GANG_HEADSHIP_LEADER
			|| this->ganger_detail_->headship == GANG_HEADSHIP_VICE)) {
		const Gang_War_Matched_Gang_Info *info = GANG_MANAGER->gang_war_find_matched_gang_info(this->ganger_detail_->gang_id);
		if (NULL == info) {
			return ERROR_GANG_WAR_NOT_PERMIT;
		}
		if (info->btn_state == 0) {
			GANG_MANAGER->gang_war_set_matched_gang_info_btn_state(this->ganger_detail_->gang_id, 1);
			GANG_MANAGER->req_gang_war_board(*(player_self()));
		} else {
			return ERROR_GANG_WAR_DRAW_ALREADY;
		}
	} else {
		return ERROR_CLIENT_OPERATE;
	}
	MSG_50170042 res_msg;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}
//int Ganger::req_gang_war_get_rank_award(void) {
//	return 0;
//}
//int Ganger::req_gang_war_rank_award_list(void) {
//	return 0;
//}

void Ganger::sync_ganger_info_to_chat() {
	MSG_20170000 msg;
	msg.gang_id = ganger_detail_->gang_id;
	msg.member_id = ganger_detail_->role_id;
	msg.gang_name = GANG_MANAGER->gang_name(ganger_detail_->gang_id);
	msg.headship = ganger_detail_->headship;

	Block_Buffer buf;
	buf.make_message(INNER_SYNC_GANG_INFO);
	msg.serialize(buf);
	buf.finish_message();
	player_self()->send_to_local_chat(buf);
}

void Ganger::sync_ganger_info_to_scene(void) {
	Logic_Player* self = player_self();
	MSG_20170000 msg;
	msg.gang_id = ganger_detail_->gang_id;
	msg.member_id = self->role_id();
	msg.gang_name = GANG_MANAGER->gang_name( msg.gang_id );
	msg.headship = ganger_detail_->headship;

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	self->send_to_scene( buf );

	sync_ganger_war_info_to_scene();
}

void Ganger::sync_ganger_war_info_to_scene(void) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_GANG_WAR_PLAYER_INFO);
	this->ganger_detail_->gang_war_serialize(buf);
	buf.finish_message();
	this->send_to_scene(buf);
}

int Ganger::ganger_load_war_detail(Block_Buffer &buf) {
	this->ganger_detail_->gang_war_deserialize(buf);
	return 0;
}
//int Ganger::gang_war_load_award(Block_Buffer &buf) {
//	uint16_t len = 0;
//	buf.read_uint16(len);
//	int32_t item_id = 0;
//	int16_t amount = 0;
//	int8_t bind = 0;
//	Item_Vec pack_item_vec;
//
//	MSG_81000102 res_msg;
//	res_msg.type = 0;
//	for (uint16_t i = 0; i < len; ++i) {
//		buf.read_int32(item_id);
//		buf.read_int16(amount);
//		buf.read_int8(bind);
//		Item_Detail item (item_id, amount, bind);
//		pack_item_vec.push_back(item);
//
//		Item_Basic_Info item_base;
//		item.set_item_basic_info(item_base);
//		res_msg.item_info.push_back(item_base);
//	}
//	player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
//						pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_GANG_WAR));
//
//	if (!res_msg.item_info.empty() || !res_msg.property.empty()) {
//		THIS_SEND_TO_CLIENT(res_msg);
//	}
//	return 0;
//}

void Ganger::make_ganger_info(Ganger_Info& info) {
	info.gang_id = ganger_detail_->gang_id;
	info.role_id = role_id();
	info.current_contribute = ganger_detail_->cur_contribute;
	info.total_contribute = ganger_detail_->total_contribute;
	info.day_contribute = ganger_detail_->day_contribute;
	info.week_contribute = ganger_detail_->contrib_of_week;
	info.gold_contrib_times = ganger_detail_->day_gold_contrib_times;
	info.diam_contrib_times = ganger_detail_->day_diam_contrib_times;
	info.rank = ganger_detail_->last_rank;
	info.headship = ganger_detail_->headship;
	info.receive_bless_times = ganger_detail_->receive_bless_times;
	info.remain_draw_times = get_remain_draw_times();
	info.gang_name = GANG_MANAGER->gang_name(ganger_detail_->gang_id);
	info.phy_power_ttl = get_phy_power_practice_left();
	Gang_Config_Authority::bit_auth_to_vec(ganger_detail_->authority, info.authority);

	std::map<int, int> type_max_id;
	for (Ganger_Skill_Map::const_iterator it = ganger_detail_->skill_map.begin();
			it != ganger_detail_->skill_map.end(); ++it) {
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

	Ganger_Skill_Info skill_info;
	for (std::map<int, int>::const_iterator it = type_max_id.begin();
			it != type_max_id.end(); ++it) {
		skill_info.reset();
		skill_info.skill_id = it->second;
		info.skill_infos.push_back(skill_info);
	}

//	Ganger_Skill_Info skill_info;
//	for (Ganger_Skill_Map::const_iterator it = ganger_detail_->skill_map.begin();
//			it != ganger_detail_->skill_map.end(); ++it) {
//		skill_info.reset();
//		skill_info.skill_id = it->second.skill_id;
//		info.skill_infos.push_back(skill_info);
//	}

	info.draw_awards = ganger_detail_->draw_awards;
}

void Ganger::insert_apply_id(role_id_t gang_id) {
	ganger_detail_->apply_ids.insert(gang_id);
}

void Ganger::erase_apply_id(role_id_t gang_id) {
	ganger_detail_->apply_ids.erase(gang_id);
}

void Ganger::add_invite_gang_id(role_id_t gang_id) {
	ganger_detail_->invites.push_back(gang_id);
}

void Ganger::del_invite_gang_id(role_id_t gang_id) {
	for (Role_Vec::iterator it = ganger_detail_->invites.begin();
			it != ganger_detail_->invites.end();) {
		if ((*it) == gang_id) {
			it = ganger_detail_->invites.erase(it);
		} else {
			++it;
		}
	}
}

int Ganger::check_add_auth(int8_t headship, Int8_Vec &add_auth_vec, int &target_player_auth) {
	if (add_auth_vec.empty()) {
		return 0;
	}
	int32_t i_cfg_default_auth = CONFIG_CACHE_GANG->power(headship).auth_to_bit();
	int32_t i_cfg_add_auth = CONFIG_CACHE_GANG->power(headship).granted_to_bit();
	std::bitset<32> b_cfg_default_auth(i_cfg_default_auth);
	std::bitset<32> b_cfg_add_auth(i_cfg_add_auth);

	for (uint8_t i = 0; i < add_auth_vec.size(); ++i) {
		if (add_auth_vec[i] < 1) {
			continue;
		}
		int32_t test_auth = add_auth_vec[i] - 1;
		// 增加的权限已在默认权限里
		if (b_cfg_default_auth.test(test_auth)) {
			return ERROR_GANG_ADD_AUTH_IN_DEFAULT;
		}
		// 增加的权限超出该职位可赋权限范围
		if (! b_cfg_add_auth.test(test_auth)) {
			return ERROR_GANG_ADD_AUTH_OUT_OF_CFG;
		}
	}

	target_player_auth = i_cfg_default_auth | Gang_Config_Authority::vec_auth_to_bit(add_auth_vec);

	return 0;
}

void Ganger::ganger_reset_leave_time(void) {
	ganger_detail_->leave_time = Time_Value::zero;
}

void Ganger::ganger_refresh_base_detail(bool sync) {
	player_self()->set_gang_id(ganger_detail_->gang_id);
	player_self()->set_gang_name(GANG_MANAGER->gang_name(ganger_detail_->gang_id));

	if (sync) {
		// sync base_detail
		sync_ganger_info_to_chat();
		sync_ganger_info_to_scene();
	}
}

void Ganger::refresh_leader_offline_time(void) {
	if (ganger_detail_->headship != GANG_HEADSHIP_LEADER) {
		return;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return;
	}
	gang_view->tv_leader_offline = Time_Value::gettimeofday();
	gang_view->change_view();
}

void Ganger::refresh_can_seize_power(void) {
	if (!ganger_detail_->gang_id || ganger_detail_->headship != GANG_HEADSHIP_LEADER) {
		return;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return;
	}
	gang_view->can_seize_power = 0;
	gang_view->change_view();
}

int Ganger::ganger_prop_refresh(void) {
	// 公会技能属性加成
	ganger_skill_prop_refresh();

	return 0;
}

int Ganger::ganger_skill_prop_refresh(void) {
//	if (! ganger_detail_->gang_id) {
//		return ERROR_GANG_NOT_IN_GANG;
//	}
	if (ganger_detail_->skill_map.empty()) {
		return ERROR_NO_SUCH_SKILL;
	}

	std::map<int, double> pro_val_map;
	std::map<int, Int_Int> type_max_map;	// <skill_type, (skill_lvl, skill_id)>
	if (ganger_detail_->gang_id) {
		Int_Int lvl_id;
		for (Ganger_Skill_Map::const_iterator it_skill = ganger_detail_->skill_map.begin();
				it_skill != ganger_detail_->skill_map.end(); ++it_skill) {
			const Gang_Cfg_Skill_Id *cfg_skill = CONFIG_CACHE_GANG->find_skill(it_skill->first);
			if (! cfg_skill) {
				continue;
			}
			lvl_id.reset();
			std::map<int, Int_Int>::iterator it_type_max_map = type_max_map.find(cfg_skill->skill_type);
			if (it_type_max_map == type_max_map.end()) {
				lvl_id.val_1 = cfg_skill->skill_lvl;
				lvl_id.val_2 = cfg_skill->skill_id;
				type_max_map.insert(std::make_pair(cfg_skill->skill_type, lvl_id));
			} else {
				if (it_type_max_map->second.val_1 < cfg_skill->skill_lvl) {
					it_type_max_map->second.val_1 = cfg_skill->skill_lvl;
					it_type_max_map->second.val_2 = cfg_skill->skill_id;
				}
			}
		}

		for (std::map<int, Int_Int>::const_iterator it_type_max_map = type_max_map.begin();
				 it_type_max_map != type_max_map.end(); ++it_type_max_map) {
			const Gang_Cfg_Skill_Id *cfg_skill = CONFIG_CACHE_GANG->find_skill(it_type_max_map->second.val_2);
			if (! cfg_skill) {
				continue;
			}
			if (cfg_skill->prop && cfg_skill->prop_val > 0) {
				pro_val_map[cfg_skill->prop] += cfg_skill->prop_val;
			}
		}
	}

	// 人物属性加成 (退出公会的要即时刷掉战力)
	player_self()->addition_prop_refresh(AT_GANG, PST_FIXED, pro_val_map, 240);

	set_ganger_skill_pro_val(pro_val_map);

	return 0;
}

int Ganger::get_ganger_skill_pro_val(std::map<int, double> &pro_val_map) const {
	pro_val_map.clear();
	pro_val_map = ganger_detail_->skill_pro_val;
	return 0;
}

int Ganger::add_contribute(const int donate_gain) {
	if (donate_gain == 0) {
		return 0;
	}
	if (! ganger_detail_->gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_EXIST;
	}
	ganger_detail_->day_contribute += donate_gain;
	ganger_detail_->contrib_of_week += donate_gain;
	ganger_detail_->total_contribute += donate_gain;
	ganger_detail_->achi_contribute += donate_gain;
	ganger_detail_->cur_contribute += donate_gain;

	ganger_detail_->detail_change();

	// add donate gain in gang fund
	gang_view->fund += donate_gain;
	GANG_MANAGER->update_member_info(GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id), ganger_detail_->role_id);
	GANG_MANAGER->refresh_rank();
	gang_view->change_view();

	// notify ganger info, gang info
	GANG_MANAGER->notify_gang_info_update(*gang_view);
	active_ganger_info_update();
	// notify ganger row info
	GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, role_id(), ganger_detail_->gang_id);
//	MSG_50170015 msg;
//	THIS_SEND_TO_CLIENT(msg);

	// 监听
	player_self()->achievement_listen_gang_contribute();
	player_self()->title_listen_contribute();
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_GANG_DEDICATE);
	return 0;
}

bool Ganger::get_is_level_lader(int32_t level) {
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if(gang_view) {
		boost::unordered_map<int32_t, role_id_t>::const_iterator it = gang_view->level_leader_id.find(level);
		if(it != gang_view->level_leader_id.end()) {
			bool re = (it->second == player_self()->role_id());
			return re;
		}
	}
	return false;
}

int Ganger::get_gang_level(void) const {
	if (ganger_detail_->gang_id) {
		Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
		if (gang_view) {
			return gang_view->level;
		}
	}
	return 0;
}

bool Ganger::is_gang_leader(void) const {
	return ganger_detail_->headship == GANG_HEADSHIP_LEADER;
}

int Ganger::set_ganger_skill_pro_val(const std::map<int, double> &pro_val_map) {
	ganger_detail_->skill_pro_val.clear();
	ganger_detail_->skill_pro_val = pro_val_map;
	//ganger_detail_->detail_change();
	return 0;
}

int Ganger::test_research_all_gang_skill(void) {
	const Gang_Cfg_Skill_Id_Map *cfg_skill_map = CONFIG_CACHE_GANG->skill_map();
	if (! cfg_skill_map) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	for (Gang_Cfg_Skill_Id_Map::const_iterator it_cfg_map = cfg_skill_map->begin();
			it_cfg_map != cfg_skill_map->end(); ++it_cfg_map) {
		if (gang_view->researched_skills.count(it_cfg_map->first)) {
			continue;
		}
		Gang_Skill_Detail skill_detail;
		skill_detail.skill_id = it_cfg_map->first;
		gang_view->researched_skills.insert(std::make_pair(skill_detail.skill_id, skill_detail));
	}

	gang_view->change_view();
	GANG_MANAGER->notify_gang_info_update(*gang_view);

	return 0;
}

void Ganger::add_receive_bless_times(void) {
	++ganger_detail_->receive_bless_times;
}

void Ganger::notify_last_leader(void) {
	if (!ganger_detail_->gang_id || ganger_detail_->headship == GANG_HEADSHIP_LEADER) {
		return;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return;
	}

	if (gang_view->last_leader_id == ganger_detail_->role_id && !gang_view->notified_last_leader) {
		GANG_MANAGER->active_gang_notify(ganger_detail_->role_id, GNA_NOTIFY_MEMBER_BE_SEIZED,
				gang_view->leader, LOGIC_MONITOR->get_role_name_by_id(gang_view->leader));

		gang_view->notified_last_leader = 1;
		gang_view->change_view();
	}
}

void Ganger::create_gang_init_ganger(Gang_DB_View* gang_view) {
	ganger_detail_->headship = GANG_HEADSHIP_LEADER;
	ganger_detail_->join_time = Time_Value::gettimeofday();
	ganger_detail_->last_rank = 1;
	ganger_detail_->authority = CONFIG_CACHE_GANG->power(GANG_HEADSHIP_LEADER).auth_to_bit();
	//ganger_detail_->remain_draw_times = CONFIG_CACHE_GANG->base(gang_view->level).daily_draw_times;
}

int Ganger::get_remain_draw_times(void) {
	if (! ganger_detail_->gang_id) {
		return 0;
	}
	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(ganger_detail_->gang_id);
	if (! gang_view) {
		return 0;
	}
	int times = CONFIG_CACHE_GANG->base(gang_view->level).daily_draw_times -
			ganger_detail_->today_has_draw_times;
	return times < 0  ?  0 : times;
}

int Ganger::get_phy_power_practice_left(void) {
	int ever_time_value = CONFIG_CACHE_PUBLIC->get_phy_power_every_practice();
	int max_time = CONFIG_CACHE_PUBLIC->get_max_practice_times();

	if (ganger_detail_->practice_times_ttl_ >= max_time) return 0;
	return (max_time - ganger_detail_->practice_times_ttl_) * ever_time_value;
}

void Ganger::listen_recover_phy_power(Time_Value &now) {

	if (ganger_detail_->practice_start_time_ == Time_Value::zero) return;

	if (!check_in_gang_scene()) {
		if (ganger_detail_->practice_start_time_ == Time_Value::zero) return;
		stop_phy_power_practice(false);
		return;
	}

	if (now < ganger_detail_->practice_start_time_) {
		MSG_DEBUG("practice start time error, role_id=%ld, start_secs=%d, now_secs=%d",
				role_id(), ganger_detail_->practice_start_time_.sec(), now.sec());
		ganger_detail_->practice_start_time_ = now;
		ganger_detail_->practice_times_ = 0;
		return;
	}

	int max_time = CONFIG_CACHE_PUBLIC->get_max_practice_times();
	if (max_time <= ganger_detail_->practice_times_ttl_) {
		stop_phy_power_practice();
		return;
	}

	int gap_secs = CONFIG_CACHE_PUBLIC->get_practice_gap_seconds();
	if (gap_secs <= 0) {
		MSG_DEBUG("practice config error, gap secs = %d", gap_secs);
		stop_phy_power_practice();
		return;
	}

	int need_stren = need_strenght();
	if (need_stren<=0) {
		stop_phy_power_practice();
		return;
	}

	int delta_secs = now.sec() - ganger_detail_->practice_start_time_.sec();
	int gaps = delta_secs / gap_secs;
	if (gaps <= ganger_detail_->practice_times_) return;

	int phy_power_value = CONFIG_CACHE_PUBLIC->get_phy_power_every_practice();
	// add phy_power
	player_self()->fight_basic_prop_modify(Property_Type::PT_STRENGTH_CURRENT,
			Addition_Type::AT_BASIC, Operate_Type::O_ADD, phy_power_value, 100);

	++ganger_detail_->practice_times_;
	++ganger_detail_->practice_times_ttl_;

	sync_client_practice_info();

	if (max_time <= ganger_detail_->practice_times_ttl_) {
		stop_phy_power_practice(false);
	}
}

int Ganger::start_phy_power_practice(void) {

	if (!check_in_gang_scene()) return ERROR_NOT_IN_GANG_SCENE;
	int need_stren = need_strenght();
	if (need_stren <= 0)
		return ERROR_STRENGTH_FULL;

	int max_time = CONFIG_CACHE_PUBLIC->get_max_practice_times();
	if (max_time <= ganger_detail_->practice_times_ttl_)
		return ERROR_LIMIT_PRACTICE_TIMES;

	ganger_detail_->practice_start_time_ = Time_Value::gettimeofday();
	ganger_detail_->practice_times_ = 0;

	sync_client_practice_info();

	return 0;
}

void Ganger::sync_client_practice_info(void) {

	MSG_50170047 msg;
	int max_time = CONFIG_CACHE_PUBLIC->get_max_practice_times();
	int gap_secs = CONFIG_CACHE_PUBLIC->get_practice_gap_seconds();

	int secs_left = 0;
	if (max_time > ganger_detail_->practice_times_ttl_) {
		secs_left = gap_secs * (max_time - ganger_detail_->practice_times_ttl_);
	}

	msg.start_time_secs = secs_left;
	msg.phy_power_ttl = get_phy_power_practice_left();

	THIS_SEND_TO_CLIENT(msg);

	int ever_time_value = CONFIG_CACHE_PUBLIC->get_phy_power_every_practice();
	if(get_phy_power_practice_left() < ever_time_value * max_time) {
		player_self()->task_listen_gang_well_recover_spirit();
	}
}

int Ganger::stop_phy_power_practice(bool sync_to_cli) {

	ganger_detail_->practice_start_time_ = Time_Value::zero;
	ganger_detail_->practice_times_ = 0;

	if (!sync_to_cli) return 0;

	MSG_50170048 msg;
	msg.result = 0;

	THIS_SEND_TO_CLIENT(msg);

	sync_client_practice_info();
	return 0;
}

int Ganger::speed_up_practice(void) {

	if (ganger_detail_->practice_start_time_ == Time_Value::zero)
		return ERROR_INNER_SERVER;
	if (!check_in_gang_scene()) {
		stop_phy_power_practice(false);
		return ERROR_NOT_IN_GANG_SCENE;
	}

	int max_time = CONFIG_CACHE_PUBLIC->get_max_practice_times();
	if (max_time <= ganger_detail_->practice_times_ttl_) {
		stop_phy_power_practice();
		return ERROR_LIMIT_PRACTICE_TIMES;
	}
	const Time_Value &now = Time_Value::gettimeofday();
	if (now <= ganger_detail_->practice_start_time_) {
		MSG_DEBUG("time error, role_id=%ld, start_time=%ld, now=%ld",
				role_id(), ganger_detail_->practice_start_time_.sec(), now.sec());
		ganger_detail_->practice_start_time_ = now;
		ganger_detail_->practice_times_ = 0;
		return ERROR_INNER_SERVER;
	}

	int need_stren = need_strenght();
	if (need_stren <= 0) {
		stop_phy_power_practice();
		return ERROR_STRENGTH_FULL;
	}

	int phy_power_value = CONFIG_CACHE_PUBLIC->get_phy_power_every_practice();
	if (phy_power_value <= 0) {
		stop_phy_power_practice();
		MSG_DEBUG("practice config error, every_time_value =%d", phy_power_value);
		return ERROR_CONFIG_NOT_EXIST;
	}
	int need_times = need_stren / phy_power_value;
	need_times = ((need_stren % phy_power_value) == 0) ? need_times : need_times+1;

	int practice_times_left = max_time - ganger_detail_->practice_times_ttl_;
	int times = practice_times_left > need_times ? need_times : practice_times_left;

	int delta_secs = now.sec() - ganger_detail_->practice_start_time_.sec();
	int gap_secs = CONFIG_CACHE_PUBLIC->get_practice_gap_seconds();
	if (gap_secs <= 0) {
		stop_phy_power_practice();
		MSG_DEBUG("practice config error, gap_secs=%d", gap_secs);
		return ERROR_CONFIG_NOT_EXIST;
	}

	int gaps = delta_secs / gap_secs;
	int remainder = delta_secs % gap_secs;

	if (gaps < ganger_detail_->practice_times_) {
		MSG_DEBUG("practice error, role_id=%ld, cur_practice_time=%d, gaps=%d",
				role_id(), ganger_detail_->practice_times_, gaps);
		ganger_detail_->practice_start_time_ = now;
		ganger_detail_->practice_times_ = 0;
		return ERROR_INNER_SERVER;
	}

	remainder = gap_secs - remainder;
	remainder += (times - (gaps - ganger_detail_->practice_times_) - 1) * gap_secs;
	int mins = remainder / 60;
	mins = ((remainder % 60) == 0) ? mins : mins+1;
	int price = CONFIG_CACHE_PUBLIC->speed_up_practice_every_min_cost();
	if (price < 0) {
		MSG_DEBUG("practice speed up config error, role_id=%ld, price=%d",
				role_id(), price);
		return ERROR_CONFIG_NOT_EXIST;
	}
	int cost_ttl = price * mins;

	bool save_golds = true;
	if (!player_self()->get_covenant_status(RAIDS_COVENANT)) {
		save_golds = false;
		Money_Sub_Info money_sub_info(Money_Sub_Type(BIND_GOLD_FIRST), cost_ttl,
				MONEY_SUB_PHY_POWER_PRACTICE);
		int ret = player_self()->pack_sub_money(money_sub_info);
		if (0 != ret) return ret;
	}

	// 加体力
	int add_stren = times * phy_power_value;
	player_self()->fight_basic_prop_modify(Property_Type::PT_STRENGTH_CURRENT,
			Addition_Type::AT_BASIC, Operate_Type::O_ADD, add_stren, 101);

	ganger_detail_->practice_times_ttl_ += times;
	stop_phy_power_practice(false);

	MSG_50170049 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	int ever_time_value = CONFIG_CACHE_PUBLIC->get_phy_power_every_practice();
	if(get_phy_power_practice_left() < ever_time_value * max_time) {
		player_self()->task_listen_gang_well_recover_spirit();
	}

	if (save_golds && cost_ttl > 0) {
		this->send_tip_to_client(ERROR_SPEED_PRACTICE_SAVE_GOLDS, cost_ttl);
		return -1;
	}

	return 0;
}

bool Ganger::check_in_gang_scene(void) {
	int scene_id = CONFIG_CACHE_PUBLIC->get_practice_scene_id();
	return (scene_id == player_self()->base_detail().location.scene_id);
}

int Ganger::need_strenght(void) {
	double cur_strenght = player_self()->fighter_detail().current_strength();
	double max_strenght = player_self()->fighter_detail().max_strength();
	return static_cast<int>(max_strenght - cur_strenght);
}

void Ganger::reset_practice(void) {
	stop_phy_power_practice();
	ganger_detail_->practice_times_ttl_ = 0;
}
