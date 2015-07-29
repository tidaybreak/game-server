/*
 * Logic_Player.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#include "team/Team.h"
#include "Logic_Player.h"
#include "Logic_Module_Head.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "Err_Code.h"
#include "Scene_Func.h"
#include "Config_Cache.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Scene.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "Msg_Role_Scene_Struct.h"
#include "gang/Gang_Manager.h"
#include "Msg_Active_Struct.h"
#include "mount/Logic_Mount.h"
#include "Msg_Inner_Struct.h"
#include "Config_Cache_Dragon_Vale.h"
#include "Scene_Func.h"
#include "Config_Cache_Gang.h"
#include "team/Team_Manager.h"
#include "anci_artifact/Anci_Artifact.h"
#include "Config_Cache_Expedition.h"
#include "expedition_scene/Expedition_Scene_Def.h"
#include "expedition/Expedition_Manager.h"
#include "answer/Answer_Manager.h"

Logic_Player::Logic_Player() {
	monitor_ = LOGIC_MONITOR;
	detail_ptr_ = 0;
	base_detail_ = 0;
	reset();
}

Logic_Player::~Logic_Player() {
	// TODO Auto-generated destructor stub
}

void Logic_Player::reset(void) {
	Logic_Player_Base::reset();
	Example::reset();
	Packer::reset();
	Operating::reset();
	Item_User::reset();
	Tasker::reset();
	Mail::reset();
	Friend::reset();
	Single_Dungeon::reset();
	Equiper::reset();
	Logic_Heroer::reset();
	Rune::reset();
	Rune_Stone::reset();
	Teamer::reset();
	Public::reset();
	Ganger::reset();
	Alchemy::reset();
	Icon::reset();
	Logic_Arenaer::reset();
	Exchange::reset();
	Dragon_Vale::reset();
	Achievement::reset();
	Logic_Mount::reset();
	Fashion::reset();
	Knight_Trials::reset();
	Anci_Artifact::reset();
	Invest_Plan::reset();
	Wing::reset();
	Expedition::reset();
	Logic_ThorLottery::reset();
	Card::reset();
	Reward::reset();
	Life_Skill::reset();
	Logic_Pet::reset();

	base_detail_ = 0;
	monitor_link_.reset();

	if (detail_ptr_) {
		POOL_MANAGER->push_player_data_pool(detail_ptr_, 25);
	}
	detail_ptr_ = 0;

	player_has_init_ = false;
	player_in_trans_ = false;
	player_in_trans_all_ = false;
}

int Logic_Player::send_to_gate(Block_Buffer &buf, bool use_swap) {
	if (!player_has_init_)
		return 0;

	return monitor_->send_to_monitor(monitor_link_.gate_muid, buf, use_swap);
}

int Logic_Player::send_to_scene(Block_Buffer &buf, bool use_swap) {
	if (!player_has_init_)
		return 0;

	// 跨进程转场景时不发到场景服，发了也找不到player
	if (player_in_trans_) {
		MSG_DEBUG("in trans, role:%ld, msg_id:%d", role_id(), buf.get_msg_id());
		return 0;
	}

	CHECK_BUFFER_LEN(buf);

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(MONITOR_SCENE,
				location().scene_id, location().scene_order, buf, 0, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(MONITOR_SCENE,
				location().scene_id, location().scene_order, tmp_buf, 0, true);
	}

	return ret;
}

int Logic_Player::send_to_local_chat(Block_Buffer &buf, bool use_swap) {
	if (!player_has_init_)
		return 0;

	CHECK_BUFFER_LEN(buf);

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.local_chat_muid,
				buf, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.local_chat_muid,
				tmp_buf, true);
	}

	return ret;
}

int Logic_Player::send_to_client(Block_Buffer &buf, bool use_swap) {
	CHECK_BUFFER_LEN(buf);

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(0, role_id());
		ret = send_to_gate(buf, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(0, role_id());
		ret = send_to_gate(tmp_buf, true);
	}

	return ret;
}

int Logic_Player::send_err_to_client(int error_id, int source) {
	Block_Buffer buf;
	buf.make_message(80000001);
	buf.write_int32(error_id);
	buf.write_int32(source);
	buf.write_string("");
	buf.finish_message();
	return send_to_client(buf);
}

int Logic_Player::send_tip_to_client(int error_id, int arg1) {
	std::string content = CONFIG_CACHE_ROLE->error_tip_map(error_id);
	if (content.empty()) {
		return this->send_err_to_client(error_id, 0);
	}

	const int size_buf = content.size() + 10;
	char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, content.c_str(), arg1);
	std::string error_content_to_send;
	error_content_to_send.assign(content_buf);

	Block_Buffer buf;
	buf.make_message(80000001);
	buf.write_int32(error_id);
	buf.write_int32(0);
	buf.write_string(error_content_to_send);
	buf.finish_message();
	return send_to_client(buf);
}

int Logic_Player::send_tip_to_client(int error_id, std::string &str1) {
	std::string content = CONFIG_CACHE_ROLE->error_tip_map(error_id);
	if (content.empty()) {
		return this->send_err_to_client(error_id, 0);
	}

	const int size_buf = content.size() + str1.length() + 10;
	char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, content.c_str(), str1.c_str());
	std::string error_content_to_send;
	error_content_to_send.assign(content_buf);

	Block_Buffer buf;
	buf.make_message(80000001);
	buf.write_int32(error_id);
	buf.write_int32(0);
	buf.write_string(error_content_to_send);
	buf.finish_message();
	return send_to_client(buf);
}

int Logic_Player::create_init_player(Logic_Player_Detail &detail) {
	detail.base_detail.role_id = detail.role_id;
	detail.base_detail.level = CONFIG_CACHE_ROLE->player_init_cache().level;
	detail.base_detail.online = 1;
	detail.base_detail.location.scene_id = CONFIG_CACHE_ROLE->player_init_cache().birth_scene;
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(detail.base_detail.location.scene_id);
	detail.base_detail.location.scene_order = random()%scene_config->order_total;
	detail.base_detail.location.coord =
			CONFIG_CACHE_ROLE->player_init_cache().birth_coord;

	detail.example_detail.role_id = detail.role_id;
	detail.public_detail.role_id = detail.role_id;
	detail.operating_detail.role_id = detail.role_id;
	detail.fighter_detail.role_id = detail.role_id;
	detail.packer_detail.role_id = detail.role_id;
	detail.item_user_detail.role_id = detail.role_id;
	detail.tasker_detail.role_id = detail.role_id;
	detail.mail_detail.role_id = detail.role_id;
	detail.friend_detail.role_id = detail.role_id;
	detail.item_sgl_dungeon_detail.role_id = detail.role_id;
	detail.equiper_detail.role_id = detail.role_id;
	detail.heroer_detail.role_id = detail.role_id;
	detail.rune_detail.role_id = detail.role_id;
	detail.rune_stone_detail.role_id = detail.role_id;
	detail.teamer_detail.role_id = detail.role_id;
	detail.ganger_detail.role_id = detail.role_id;
	detail.alchemy_detail.role_id = detail.role_id;
	detail.icon_detail.role_id = detail.role_id;
	detail.arenaer_detail.role_id = detail.role_id;
	detail.logic_status_detail.role_id = detail.role_id;
	detail.exchange_detail.role_id = detail.role_id;
	detail.helper_detail.role_id = detail.role_id;
	detail.covenant_detail.role_id = detail.role_id;
	detail.dragon_vale_detail.role_id = detail.role_id;
	detail.achievement_detail.role_id = detail.role_id;
	detail.title_detail.role_id = detail.role_id;
	detail.vip_deatil.role_id = detail.role_id;
	detail.mount_detail.role_id = detail.role_id;
	detail.reward_detail.role_id = detail.role_id;
	detail.active_content_detail.role_id = detail.role_id;
	detail.fashion_detail.role_id = detail.role_id;
	detail.knight_trials_player_detail.role_id = detail.role_id;
	detail.knight_trials_detail.role_id = detail.role_id;
	detail.anci_artifact_detail.role_id = detail.role_id;
	detail.invest_plan_detail.role_id = detail.role_id;
	detail.wing_detail.role_id = detail.role_id;
	detail.expedition_detail.role_id = detail.role_id;
	detail.thorlottery_detail.role_id = detail.role_id;
	detail.activity_detail.role_id = detail.role_id;
	detail.r2_invite_detail.role_id = detail.role_id;
	detail.activities_stores_detail.role_id = detail.role_id;
	detail.life_skill_detail.role_id = detail.role_id;
	detail.oper_activity_detail.role_id = detail.role_id;
	detail.pet_detail.role_id = detail.role_id;

	Example::create_init(detail.example_detail);
	Logic_Fighter::create_init(detail.fighter_detail);
	Packer::create_init(detail.packer_detail);
	Item_User::create_init(detail.item_user_detail);
	Tasker::create_init(detail.tasker_detail);
	Mail::create_init(detail.mail_detail);
	Friend::create_init(detail.friend_detail);
	Single_Dungeon::create_init(detail.item_sgl_dungeon_detail);
	Equiper::create_init(detail.equiper_detail);
	Logic_Heroer::create_init(detail.heroer_detail);
	Rune::create_init(detail.rune_detail);
	Rune_Stone::create_init(detail.rune_stone_detail);
	Teamer::create_init(detail.teamer_detail);
	Ganger::create_init(detail.ganger_detail);

	//Icon_Detail's init must  after Base_Detail
	Icon::create_init(detail.icon_detail, detail.base_detail);
	Logic_Arenaer::create_init(detail.arenaer_detail);
	Public::create_init(detail.public_detail, detail.base_detail);
	Exchange::create_init(detail.exchange_detail);
	//Dragon_Vale_Detail's init must after Base_Detail
	Dragon_Vale::create_init(detail.dragon_vale_detail, detail.base_detail);
	Achievement::create_init(detail.achievement_detail);
	Title::create_init(detail.title_detail);
	Vip::create_init(detail.vip_deatil);
	Logic_Mount::create_init(detail.mount_detail);
	Reward::create_init(detail.reward_detail);
	Auction::create_init(detail.auction_detail);
	Anci_Artifact::create_init(detail.anci_artifact_detail);
	Invest_Plan::create_init(detail.invest_plan_detail, detail.base_detail);
	Activity::create_init(detail.activity_detail);
	Life_Skill::create_init(detail.life_skill_detail);
	Fashion::create_init(detail.fashion_detail);
	return 0;
}

role_id_t Logic_Player::role_id(void) {
	return base_detail().role_id;
}

std::string Logic_Player::role_name(void) {
	return base_detail().role_name;
}

int Logic_Player::level(void) {
	return fighter_detail().level;
}

int Logic_Player::career(void) {
	return base_detail().career;
}

int Logic_Player::gender(void) {
	return base_detail().gender;
}

bool Logic_Player::player_has_init(void) {
	return player_has_init_;
}

int Logic_Player::scene_id(void) {
	return base_detail().location.scene_id;
}

role_id_t Logic_Player::gang_id(void) {
	return base_detail().gang_id;
}

uint32_t Logic_Player::head_id(void){
	return base_detail().head_id;
}

Location const &Logic_Player::location(void) const {
	return base_detail().location;
}

Player_Monitor_Link const &Logic_Player::monitor_link(void) {
	return monitor_link_;
}

int Logic_Player::login_logic(Logic_Player_Detail &detail) {

	load_detail(detail);

	JUDGE_ERROR_RETURN(module_init());

	JUDGE_ERROR_RETURN(monitor_link_init()); //必须优先于每天/周刷新

	trigger_daily_zero(monitor()->trigger_daily_zero_time(), true);
	trigger_daily_six(monitor()->trigger_daily_six_time(), true);
	trigger_weekly_zero(monitor()->trigger_weekly_zero_time(), true);

	player_has_init_ = true;

	JUDGE_ERROR_RETURN(notice_chat_login_ready());

	JUDGE_ERROR_RETURN(notice_scene_login_ready(LOGIC_SCENE_LOGIN_READY));

	return 0;
}

void Logic_Player::sign_in(void) {
	double save_interval = 1 + monitor_->player_role_id_map().size() / 1000.0;
	this->create_event(Logic_Enum::EVENT_LOGIC_SAVE_DETAIL, &Logic_Player::save_player_data, Time_Value(SAVE_DETAIL_INTERVAL * save_interval));

	base_detail_->last_sign_in_time = Time_Value::gettimeofday();

	set_online_flag(1);

	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id());
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		find_it->second->last_sign_in_time = base_detail_->last_sign_in_time;
		public_detail_->fcm = find_it->second->fcm;
		base_detail_->ip = find_it->second->ip;
	}
	base_detail_->detail_change();

	monitor()->scene_change_info(monitor_link_.scene_muid, 1);

	Logic_Fighter::sign_in();
	Example::sign_in();
	Public::sign_in();
	Operating::sign_in();
	Packer::sign_in();
	Item_User::sign_in();
	Tasker::sign_in();
	Mail::sign_in();
	Friend::sign_in();
	Single_Dungeon::sign_in();
	Equiper::sign_in();
	Logic_Heroer::sign_in();
	Rune::sign_in();
	Rune_Stone::sign_in();
	Teamer::sign_in();
	Ganger::sign_in();
	Icon::sign_in();
	Exchange::sign_in();
	Dragon_Vale::sign_in();
	Achievement::sign_in();
	Title::sign_in();
	Logic_Mount::sign_in();
	Helper::sign_in();
	Reward::sign_in();
	Fashion::sign_int();
	Logic_Arenaer::sign_in();
	Logic_Status::sign_in();
	Knight_Trials::sign_in();
	Anci_Artifact::sign_in();
	Invest_Plan::sign_in();
	Wing::sign_int();
	Logic_ThorLottery::sign_in();
	Activity::sign_in();
	Life_Skill::sign_in();
	Logic_Pet::sign_int();
	Oper_Activity::sign_in();
}

void Logic_Player::sign_out(int out_reason) {
	MSG_DEBUG("sign out role:%ld, out_reason:%d, force:%d, hero force:%d", role_id(), out_reason, fighter_detail().force, get_fight_hero_force());


	// 保证sign_in被调用到(客户端可能没发登录网关)
	if (out_reason == ERROR_LOGIN_GATE_TIMEOUT) {
		MSG_USER("some error, role:%ld", role_id());
		sign_in();
	}
	set_online_flag(0);

	monitor()->scene_change_info(monitor_link_.scene_muid, 0);

	base_detail_->last_sign_out_time = Time_Value::gettimeofday();
	Example::sign_out();
	Public::sign_out();
	Operating::sign_out();
	Packer::sign_out();
	Item_User::sign_out();
	Tasker::sign_out();
	Mail::sign_out();
	Friend::sign_out();
	Single_Dungeon::sign_out();
	Equiper::sign_out();
	Logic_Heroer::sign_out();
	Rune::sign_out();
	Rune_Stone::sign_out();
	Teamer::sign_out();
	Ganger::sign_out();
	Icon::sign_out();
	Exchange::sign_out();
	Dragon_Vale::sign_out();
	Achievement::sign_out();
	Title::sign_out();
	Logic_Mount::sign_out();
	Fashion::sign_out();
	Auction::sign_out();
	Logic_Arenaer::sign_out();
	Logic_Status::sign_out();
	Logic_Fighter::sign_out();
	Knight_Trials::sign_out();
	Anci_Artifact::sign_out();
	Invest_Plan::sign_out();
	Wing::sign_out();
	Logic_ThorLottery::sign_out();
	Life_Skill::sign_out();
	Logic_Pet::sign_out();

	Player_DB_View *player_db_view = 0;
	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id());
	if (find_it == monitor()->db_view()->id_player_view_map.end()) {
		MSG_USER("can not find db_view player:%ld", role_id());
		return;
	}
	player_db_view = find_it->second;

	// 同步db_view数据
	//if (Player_DB_View::ACCOUNT_STATUS_TRUSTEESHIP & player_db_view->account_mode)
	{
		player_db_view->location = base_detail_->location;
		player_db_view->last_sign_out_time = base_detail_->last_sign_out_time;
		player_db_view->add_up_online_time = public_detail_->add_up_online_time;
		player_db_view->fcm = public_detail_->fcm;
		player_db_view->title_id = base_detail_->title_id;
		player_db_view->avatar_vec = base_detail_->avatar_vec;
	}


	{
		Block_Buffer buf;
		buf.make_message(LOGIC_CHAT_LOGOUT_READY);
		buf.finish_message();
		send_to_local_chat(buf);
	}

	{
		Block_Buffer buf;
		buf.make_message(LOGIC_GATE_LOGOUT_READY);
		buf.write_int64(role_id());
		buf.write_int32(out_reason);
		buf.write_int32(1);
		buf.finish_message();
		send_to_gate(buf);
	}

}

void Logic_Player::trusteeship_sign_out(void) {
	this->set_online_flag(2);
	base_detail_->detail_change();

	Example::trusteeship_sign_out();
	Public::trusteeship_sign_out();
	Teamer::trusteeship_sign_out();
	Achievement::trusteeship_sign_out();
	Title::trusteeship_sign_out();
	Auction::trusteeship_sign_out();
	Anci_Artifact::trusteeship_sign_out();

	Player_DB_View *player_db_view = 0;
	DB_View::ID_Player_View_Map::iterator find_it =
			monitor()->db_view()->id_player_view_map.find(role_id());
	if (find_it == monitor()->db_view()->id_player_view_map.end()) {
		MSG_USER("can not find db_view player:%ld", role_id());
		return;
	}
	player_db_view = find_it->second;
	// 同步db_view数据
	player_db_view->location = base_detail_->location;
	player_db_view->last_sign_out_time = base_detail_->last_sign_out_time;
	player_db_view->add_up_online_time = public_detail_->add_up_online_time;
	player_db_view->fcm = public_detail_->fcm;
}

void Logic_Player::load_detail(Logic_Player_Detail &detail) {
	detail_ptr_ = &detail;
	base_detail_ = &detail.base_detail;

	Example::load_detail(detail.example_detail);
	Public::load_detail(detail.public_detail);
	Operating::load_detail(detail.operating_detail);
	Logic_Fighter::load_detail(detail.fighter_detail);
	Packer::load_detail(detail.packer_detail);
	Item_User::load_detail(detail.item_user_detail);
	Tasker::load_detail(detail.tasker_detail);
	Mail::load_detail(detail.mail_detail);
	Friend::load_detail(detail.friend_detail);
	Single_Dungeon::load_detail(detail.item_sgl_dungeon_detail);
	Equiper::load_detail(detail.equiper_detail);
	Logic_Heroer::load_detail(detail.heroer_detail);
	Rune::load_detail(detail.rune_detail);
	Rune_Stone::load_detail(detail.rune_stone_detail);
	Teamer::load_detail(detail.teamer_detail);
	Ganger::load_detail(detail.ganger_detail);
	Alchemy::load_detail(detail.alchemy_detail);
	Icon::load_detail(detail.icon_detail);
	Logic_Arenaer::load_detail(detail.arenaer_detail);
	Logic_Status::load_detail(detail.logic_status_detail);
	Exchange::load_detail(detail.exchange_detail);
	Helper::load_detail(detail.helper_detail);
	Covenant::load_detail(detail.covenant_detail);
	Dragon_Vale::load_detail(detail.dragon_vale_detail);
	Achievement::load_detail(detail.achievement_detail);
	Title::load_detail(detail.title_detail);
	Vip::load_detail(detail.vip_deatil);
	Logic_Mount::load_detail(detail.mount_detail);
	Reward::load_detail(detail.reward_detail);
	Active_Content::load_detail(detail.active_content_detail);
	Fashion::load_detail(detail.fashion_detail);
	Auction::load_detail(detail.auction_detail);
	Knight_Trials::load_player_detail(detail.knight_trials_player_detail);
	Knight_Trials::load_detail(detail.knight_trials_detail);
	Anci_Artifact::load_detail(detail.anci_artifact_detail);
	Invest_Plan::load_detail(detail.invest_plan_detail);
	Wing::load_detail(detail.wing_detail);
	Expedition::load_detail(detail.expedition_detail);
	Logic_ThorLottery::load_detail(detail.thorlottery_detail);
	Activity::load_detail(detail.activity_detail);
	R2_Invite::load_detail(detail.r2_invite_detail);
	Activities_Stores::load_detail(detail.activities_stores_detail);
	Life_Skill::load_detail(detail.life_skill_detail);
	Oper_Activity::load_detail(detail.oper_activity_detail);
	Logic_Pet::load_detail(detail.pet_detail);
	// set base detail
	base_detail_->gang_id = ganger_detail().gang_id;
	base_detail_->gang_name = GANG_MANAGER->gang_name(ganger_detail().gang_id);
}

void Logic_Player::save_detail(Logic_Player_Detail &detail) {
	if (base_detail().is_change) {
		detail.base_detail = base_detail();
		base_detail_->is_change = false;
	}

	Example::save_detail(detail.example_detail);
	Public::save_detail(detail.public_detail);
	Operating::save_detail(detail.operating_detail);
	Logic_Fighter::save_detail(detail.fighter_detail);
	Packer::save_detail(detail.packer_detail,detail.logout);
	Item_User::save_detail(detail.item_user_detail);
	Tasker::save_detail(detail.tasker_detail);
	Mail::save_detail(detail.mail_detail);
	Friend::save_detail(detail.friend_detail);
	Single_Dungeon::save_detail(detail.item_sgl_dungeon_detail);
	Equiper::save_detail(detail.equiper_detail);
	Logic_Heroer::save_detail(detail.heroer_detail);
//	Rune::save_detail(detail.rune_detail);
	Rune_Stone::save_detail(detail.rune_stone_detail);
	Teamer::save_detail(detail.teamer_detail);
	Ganger::save_detail(detail.ganger_detail);
	Alchemy::save_detail(detail.alchemy_detail);
	Icon::save_detail(detail.icon_detail);
	Logic_Arenaer::save_detail(detail.arenaer_detail);
	Logic_Status::save_detail(detail.logic_status_detail);
	Exchange::save_detail(detail.exchange_detail);
	Helper::save_detail(detail.helper_detail);
	Covenant::save_detail(detail.covenant_detail);
	Dragon_Vale::save_detail(detail.dragon_vale_detail);
	Achievement::save_detail(detail.achievement_detail);
	Title::save_detail(detail.title_detail);
	Vip::save_detail(detail.vip_deatil);
	Logic_Mount::save_detail(detail.mount_detail);
	Reward::save_detail(detail.reward_detail);
	Active_Content::save_detail(detail.active_content_detail);
	Fashion::save_detail(detail.fashion_detail);
	Auction::save_detail(detail.auction_detail);
	Knight_Trials::save_player_detail(detail.knight_trials_player_detail);
	Knight_Trials::save_detail(detail.knight_trials_detail);
	Anci_Artifact::save_detail(detail.anci_artifact_detail);
	Invest_Plan::save_detail(detail.invest_plan_detail);
	Wing::save_detail(detail.wing_detail);
	Expedition::save_detail(detail.expedition_detail);
	Logic_ThorLottery::save_detail(detail.thorlottery_detail);
	Activity::save_detail(detail.activity_detail);
	R2_Invite::save_detail(detail.r2_invite_detail);
	Life_Skill::save_detail(detail.life_skill_detail);
	Oper_Activity::save_detail(detail.oper_activity_detail);
	Logic_Pet::save_detail(detail.pet_detail);
}

void Logic_Player::set_detail_change(bool change) {
	player_in_trans_ = false;
	player_in_trans_all_  = false;

	base_detail_->detail_change();

	detail_ptr_->example_detail.detail_change();
	detail_ptr_->public_detail.detail_change();
	detail_ptr_->operating_detail.detail_change();
	detail_ptr_->fighter_detail.detail_change();
	detail_ptr_->packer_detail.detail_change();
	detail_ptr_->tasker_detail.detail_change();
	detail_ptr_->mail_detail.detail_change();
	detail_ptr_->friend_detail.detail_change();
	detail_ptr_->item_sgl_dungeon_detail.detail_change();
	detail_ptr_->equiper_detail.detail_change();
	detail_ptr_->heroer_detail.detail_change();
	detail_ptr_->rune_detail.detail_change();
	detail_ptr_->rune_stone_detail.detail_change();
	detail_ptr_->teamer_detail.detail_change();
	detail_ptr_->ganger_detail.detail_change();
	detail_ptr_->alchemy_detail.detail_change();
	detail_ptr_->icon_detail.detail_change();
	detail_ptr_->arenaer_detail.detail_change();
	detail_ptr_->logic_status_detail.detail_change();
	detail_ptr_->exchange_detail.detail_change();
	detail_ptr_->dragon_vale_detail.detail_change();
	detail_ptr_->achievement_detail.detail_change();
	detail_ptr_->title_detail.detail_change();
	detail_ptr_->vip_deatil.detail_change();
	detail_ptr_->mount_detail.detail_change();
	detail_ptr_->covenant_detail.detail_change();
	detail_ptr_->reward_detail.detail_change();
	detail_ptr_->active_content_detail.detail_change();
	detail_ptr_->fashion_detail.detail_change();
	detail_ptr_->knight_trials_player_detail.detail_change();
	detail_ptr_->knight_trials_detail.detail_change();
	detail_ptr_->anci_artifact_detail.detail_change();
	detail_ptr_->invest_plan_detail.detail_change();
	detail_ptr_->wing_detail.detail_change();
	detail_ptr_->expedition_detail.detail_change();
	detail_ptr_->thorlottery_detail.detail_change();
	detail_ptr_->activity_detail.detail_change();
	detail_ptr_->r2_invite_detail.detail_change();
	detail_ptr_->activities_stores_detail.detail_change();
	detail_ptr_->life_skill_detail.detail_change();
	detail_ptr_->oper_activity_detail.detail_change();
	detail_ptr_->pet_detail.detail_change();
}

void Logic_Player::set_online_flag(int online) {
	base_detail_->online = online;
}

void Logic_Player::set_logout_time(const Time_Value &time) {
	base_detail_->last_sign_out_time = time;
}

void Logic_Player::refresh_mover_detail(Location &location) {
	if (base_detail_->location.coord != location.coord) {
		void_coord_change(base_detail_->location.coord, location.coord);
	}
	base_detail_->location.coord = location.coord;
	base_detail_->location.toward = location.toward;
	base_detail_->location.scene_layer = location.scene_layer;
	base_detail_->location.scene_order = location.scene_order;

	base_detail_->detail_change();
}

void Logic_Player::void_coord_change(Coord &src_coord, Coord &des_coord) {
	task_listen_arrive(scene_id(), des_coord.pixel_coord().x,
			des_coord.pixel_coord().y);
}

void Logic_Player::save_player_data(void) {
	//MSG_DEBUG("success:%ld", Time_Value::gettimeofday().sec());

	Logic_Player_Detail *detail = POOL_MANAGER->pop_player_data_pool(7);
	if (!detail) {
		MSG_USER("DB_MANAGER_INSTANCE->player_data_pool().pop() return 0, "
				"player data lost, role_id = %ld.", role_id());
	}

	if (!base_detail_->online)
		detail->set_logout();

	this->save_detail(*detail);
	DB_MANAGER->push_save_player_detail(detail);
}

int Logic_Player::module_init(void) {
	if (!career())
		base_detail_->career = 1;
	base_detail_->level = fighter_detail().level;
	exit_diff_scence();

	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id());
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		find_it->second->last_sign_in_time = base_detail_->last_sign_in_time;
		public_detail_->fcm = find_it->second->fcm;
		base_detail_->ip = find_it->second->ip;
	}
	base_detail_->detail_change();

	// 注意顺序
	Logic_Fighter::module_init();
	Example::module_init();
	Public::module_init();
	Operating::module_init();
	Packer::module_init();
	Item_User::module_init();
//	Rune::module_init();					// 符文放装备前面
	Rune_Stone::module_init();
	Fashion::module_init();
	Wing::module_init();						// 翅膀放英雄前面
	Logic_Pet::module_init();				// 宠物必须放英雄前面
	Equiper::module_init();					// 装备放英雄前面
	Ganger::module_init();					// 公会放英雄前面
	Logic_Mount::module_init();			// 坐骑必须放在英雄前面
	Logic_Heroer::module_init();
	Tasker::module_init();
	Mail::module_init();
	Friend::module_init();
	Single_Dungeon::module_init();
	Alchemy::module_init();
	Icon::module_init();
	Logic_Arenaer::module_init();
	Logic_Status::module_init();
	Exchange::module_init();
	Helper::module_init();
	Covenant::module_init();
	Teamer::module_init();
	Achievement::module_init();
	Title::module_init();
	Vip::module_init();
	Reward::module_init();
	Auction::module_init();
	Anci_Artifact::module_init();
	Invest_Plan::module_init();
	Expedition::module_init();
	Logic_ThorLottery::module_init();
	Knight_Trials::module_init();
	Active_Content::module_init();
	Life_Skill::module_init();
	Oper_Activity::module_init();
//	Card::module_init();

	//一定要放在vip后面
	Dragon_Vale::module_init();

	// 恢复当前属性值(放最后)
	Logic_Fighter::reset_current_prop();
	refresh_avatar();
	return 0;
}

int Logic_Player::trigger_daily_zero(const Time_Value &now,
		bool logic_check/* = false*/) {
	if (logic_check
			&& is_same_day(now, base_detail_->last_trigger_daily_zero)) {
		return 0;
	}

	Logic_Fighter::trigger_daily_zero(now, logic_check);
	Example::trigger_daily_zero(now);
	Teamer::trigger_daily_zero(now);
	Ganger::trigger_daily_zero(now);
	Alchemy::trigger_daily_zero(now);
	Single_Dungeon::trigger_daily_zero(now, logic_check);
	Logic_Arenaer::trigger_daily_zero(now, logic_check);
	Helper::trigger_daily_zero(now);
	Covenant::trigger_daily_zero(now);
	Public::trigger_daily_zero(now, logic_check);
	Reward::trigger_daily_zero(now);
	Rune_Stone::trigger_daily_zero(now);
	Tasker::trigger_daily_zero(now);
	Auction::trigger_daily_zero(now);
	Anci_Artifact::trigger_daily_zero(now);
	Invest_Plan::trigger_daily_zero(now);
	Mail::trigger_daily_zero(now);
	Exchange::trigger_daily_zero(now);
	Vip::trigger_daily_zero(now);
	Active_Content::trigger_daily_zero(now);
	Activity::trigger_daily_zero(now);
	Oper_Activity::trigger_daily_zero(now);

	base_detail_->last_trigger_daily_zero = now;
	base_detail_->detail_change();
	return 0;
}

int Logic_Player::trigger_daily_six(const Time_Value& now,
		bool logic_check/* = false*/) {
	if (logic_check && is_same_day(now, base_detail_->last_trigger_daily_six)) {
		return 0;
	}
	MSG_DEBUG("trigger_daily_six on role_id:%ld, now:%ld",  player_self()->role_id(), now.sec());
//	base_detail_->last_trigger_daily_six = now;
	Logic_Fighter::trigger_daily_six(now, logic_check);
	Teamer::trigger_daily_six(now, logic_check);
	Ganger::trigger_daily_six(now, logic_check);
	Single_Dungeon::trigger_daily_six(now, logic_check);
	Logic_Arenaer::trigger_daily_six(now, logic_check);
	Public::trigger_daily_six(now, logic_check);
	Vip::trigger_daily_six(now);
	Knight_Trials::trigger_daily_six(now);
	Reward::trigger_daily_six(now);
	Invest_Plan::trigger_daily_six(now);
	Wing::trigger_daily_six(now);
	Alchemy::trigger_daily_six(now);
	Logic_ThorLottery::trigger_daily_six(now);
	Exchange::trigger_daily_six(now);
	Logic_Heroer::trigger_daily_six(now);
	Active_Content::trigger_daily_six(now);
	Helper::trigger_daily_six(now);
    Life_Skill::trigger_daily_six(now,logic_check);

    //activity must be the end
    Oper_Activity::trigger_daily_six(now);
	//task must be the end
	Tasker::trigger_daily_six(now);
	base_detail_->last_trigger_daily_six = now;
	base_detail_->detail_change();
	return 0;
}

int Logic_Player::trigger_weekly_zero(const Time_Value &now,
		bool logic_check/* = false*/) {
	if (logic_check
			&& is_same_day(now, base_detail_->last_trigger_weekly_zero)) {
		return 0;
	}

	Example::trigger_weekly_zero(now);
	Ganger::trigger_weekly_zero(now);
	Public::trigger_weekly_zero(now);
	base_detail_->last_trigger_weekly_zero = now;
	base_detail_->detail_change();
	return 0;
}

int Logic_Player::bad_scene_protect(void) {
	// 防止场景配制删除
	const Scene_Config *scene_config =
			CONFIG_CACHE_SCENE->scene_config_cache(location().scene_id);
	int map_value = 0;
	if (scene_config) {
		map_value = CONFIG_CACHE_SCENE->get_map_value(scene_config->map_id,
				base_detail_->location.coord);

		// 如果是跨服场景就强制修改order
		base_detail_->location.scene_order = scene_config->inter_order >= 0 ? scene_config->inter_order : base_detail_->location.scene_order;
	} else {
		MSG_USER("can not find scene:%d ", location().scene_id);
	}

	// 防止场景方案删除
	Monitor_Unique_ID scene_muid;
	int muid_find = monitor()->find_scene_muid(location().scene_id, location().scene_order,
			scene_muid);

	// 场景是否能连上
	int conn_ret = 0;
	if (!muid_find) {
		Block_Buffer test_buf;
		test_buf.make_message(SERVER_INNER_MSG_TEST);
		test_buf.finish_message();
		conn_ret = monitor()->send_to_scene(location().scene_id, location().scene_order, test_buf);
	}

	if (!scene_config || map_value <= 0 || !scene_muid.id || conn_ret) {
		MSG_USER("scene id:%d, order:%d, x:%d, y:%d, muid:%d, conn_ret:%d",
				location().scene_id, location().scene_order,
				location().coord.x, location().coord.y, scene_muid.id, conn_ret);
		base_detail_->location.scene_id =
				CONFIG_CACHE_ROLE->player_init_cache().safe_scene;
		base_detail_->location.scene_order = 0;
		base_detail_->location.scene_layer = 0;
		base_detail_->location.coord =
				CONFIG_CACHE_ROLE->player_init_cache().safe_coord;
	}
	return 0;
}

int Logic_Player::exit_diff_scence(void) {
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(location().scene_id);
	if(!scene_config) {
		bad_scene_protect();
		scene_config = CONFIG_CACHE_SCENE->scene_config_cache(location().scene_id);
	} else {
		// 如果是跨服场景就强制修改order
		base_detail_->location.scene_order = scene_config->inter_order >= 0 ? scene_config->inter_order : base_detail_->location.scene_order;
	}

	// 队伍不存在，人在组队场景，回滚
	if (Scene_Team == scene_config->manager
			|| Scene_Task_Manager == scene_config->manager
			|| Scene_Team_Defend == scene_config->manager) {
		Team* team = Teamer::get_team();
		bool rollback = false;
		if (NULL == team) {
			rollback = true;
		} else if (!team->is_in_team(this->role_id())) {
			rollback = true;
		}

		if (rollback) {
			Coord coord;
			int des_scene = 0, scene_order = 0;
			this->scene_rollback(des_scene, scene_order, coord, true);
		}
	} else if (Scene_Gang_War == scene_config->manager) {
		bool need_rollback = false;
		const Gang_War_Matched_Gang_Info *info = GANG_MANAGER->gang_war_find_matched_gang_info(gang_id());
		if (!info) {
			need_rollback = true;
		} else {
			if (GANG_MANAGER->gang_war_is_open()
					&& info
					&& this->ganger_detail().gang_war_attend_time.sec() == info->match_time.sec()
					&& this->ganger_detail().gang_war_attend_gang_id == gang_id()) {
				const Gang_War_Spot *result_info = GANG_MANAGER->gang_war_find_result_map(gang_id());
				if (!result_info || result_info->result != 0) {
					need_rollback = true;
				}
			} else {
				need_rollback = true;
			}
		}
		if (need_rollback) {
			Coord coord;
			int des_scene = 0, scene_order = 0;
			this->scene_rollback(des_scene, scene_order, coord, true);
		}
	} else if (Scene_Expedition_Manager == scene_config->manager) {
		Coord coord;
		int des_scene = 0, scene_order = 0;
		this->scene_rollback(des_scene, scene_order, coord, true);
	}


	bad_scene_protect();

	return 0;
}

int Logic_Player::monitor_link_init(void) {
	monitor_->select_gate_link_info(monitor_link_.gate_muid);

	monitor_link_.scene_muid = Monitor_Unique_ID(CONFIG_CACHE->server_flag(),
			MONITOR_SCENE, location().scene_id, location().scene_order);

	Monitor_Mark_Vec id_order;
	const Server_Solution_Cache &server_solution_cache =
			CONFIG_CACHE->server_solution_cache();
	server_solution_cache.fetch_muid_by_type(id_order,
			Server_Enum::MONITOR_LOGIC);
	if (id_order.empty()) {
		return -1;
	} else {
		monitor_link_.logic_muid = id_order.front();
	}

	id_order.clear();
	server_solution_cache.fetch_muid_by_type(id_order,
			Server_Enum::MONITOR_CHAT);
	if (id_order.empty()) {
		return -1;
	} else {
		monitor_link_.local_chat_muid = id_order.front();
	}
	if (id_order.size() == 2) {
		monitor_link_.inter_chat_muid = id_order[1];
	}

	return 0;
}

int Logic_Player::notice_gate_login_ready(void) {
	std::string session;
	DB_View::Role_Session_Map::iterator find_it = monitor()->db_view()->role_session_map.find(role_id());
	if (find_it != monitor()->db_view()->role_session_map.end()) {
		session = find_it->second;
	}

	Block_Buffer buf;
	buf.make_message(LOGIC_GATE_LOGIN_READY);
	buf.write_int64(role_id());
	buf.write_string(session);
	monitor_link_.serialize(buf);
	base_detail().serialize(buf);
	buf.finish_message();
	return send_to_gate(buf);
}

int Logic_Player::notice_scene_login_ready(int msg_id) {
	Block_Buffer buf;
	buf.make_message(msg_id);

	// 链接信息
	monitor_link_.serialize(buf);
	// 基础信息
	base_detail().serialize(buf);
	// 战斗信息
	Logic_Fighter::sync_scene_module_info(buf);

	Example::sync_scene_module_info(buf);

	Logic_Status::sync_status_module_info(buf);

	Ganger::sync_scene_module_info(buf);

	Teamer::sync_teamer_detail_to_scene(buf);
	buf.write_string(CONFIG_CACHE->server_maintainer_cache().game_server_flag);
	Expedition::sync_scene_module_info(buf);
	Life_Skill::sync_energy_info(buf);


	buf.finish_message();
	return send_to_scene(buf, true);
}

int Logic_Player::notice_chat_login_ready(void) {
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_LOGIN_READY);
	monitor_link_.serialize(buf);
	base_detail().serialize(buf);

	Friend::syn_black_list_to_chat_server(buf);

	buf.finish_message();
	return send_to_local_chat(buf, true);
}

int Logic_Player::sign_in_repeat(bool is_trustership) {
	set_online_flag(1);
	base_detail_->detail_change();

	Example::sign_in_repeat();
	Public::sign_in_repeat();
	Teamer::sign_in_repeat(is_trustership);
	Achievement::sign_in_repeat();
	Title::sign_in_repeat();
	Ganger::sign_in_repeat();
	Anci_Artifact::sign_in_repeat();
	return 0;
}

int Logic_Player::client_ready_now(void) {
	{
		// 角色基础信息
		MSG_50100015 role_msg;
		role_msg.property.push_back(Property(PT_LEVEL, level()));
		role_msg.property.push_back(Property(PT_CAREER, career()));
		role_msg.property.push_back(Property(PT_FORCE, fighter_detail().force));
		role_msg.property.push_back(Property(PT_ENERGY,life_skill_detail().energy));

		role_msg.role_base_info.role_id = role_id();
		role_msg.role_base_info.role_name = role_name();
		role_msg.role_base_info.gender = base_detail().gender;
		role_msg.role_base_info.guild_name = base_detail().gang_name;
		role_msg.role_base_info.career = base_detail().career;
		role_msg.role_base_info.level = level();
		role_msg.role_base_info.vip_level = base_detail().vip;
		role_msg.role_base_info.combat = fighter_detail().force;
		role_msg.role_type = base_detail().role_type;
		THIS_SEND_TO_CLIENT(role_msg);
	}

	{
		// 主场景传送点信息
		this->send_to_client(CONFIG_CACHE_SCENE->client_find_path_point());
	}

	Example::client_ready_now();
	Public::client_ready_now();
	Packer::client_ready_now();
	Tasker::client_ready_now();
	Teamer::client_ready_now();
	Friend::client_ready_now();
	Rune::client_ready_now();
	Rune_Stone::client_ready_now();
	Equiper::client_ready_now();
	Logic_Heroer::client_ready_now();
	Icon::client_ready_now();
	Mail::client_ready_now();
	Exchange::client_ready_now();
	Covenant::client_ready_now();
	Dragon_Vale::client_ready_now();
	Achievement::client_ready_now();
	Title::client_ready_now();
	Vip::client_ready_now();
	Helper::client_ready_now();
	Reward::client_ready_now();
	Single_Dungeon::client_ready_now();
	Active_Content::client_ready_now();
	Auction::client_ready_now();
	Anci_Artifact::client_ready_now();
	Invest_Plan::client_ready_now();
	Logic_Status::client_ready_now();
	Rank::client_ready_now();
	Answer::client_ready_now();
	Knight_Trials::client_ready_now();
	Expedition::client_ready_now();
	Oper_Activity::client_ready_now();

	//通知聊天服
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_CLIENT_READY_NOW);
	monitor_link_.serialize(buf);
	buf.finish_message();
	send_to_local_chat(buf);
	//
	sync_phy_power_to_client();

	//must after vip / convent/ magic wing
	notify_try_time_out_list();

	//note: notify_guide_step_list function must after all module' message in client_ready_now()
	notify_guide_setp_list();

	return 0;
}

int Logic_Player::move_scene_completed(void) {

	Example::move_scene_completed();
	Friend::move_scene_completed();
	Icon::move_scene_completed();
	Logic_Heroer::move_scene_completed();
	Exchange::move_scene_completed();
	Teamer::move_scene_completed();
	Dragon_Vale::move_scene_completed();
	Achievement::move_scene_completed();
	Title::move_scene_completed();
	Ganger::move_scene_completed();
	Tasker::move_scene_completed();
	Anci_Artifact::move_scene_completed();
	Logic_Status::move_scene_completed();
	Answer::move_scene_completed();
	Single_Dungeon::move_scene_completed();
	Expedition::move_scene_completed();
	return 0;
}

int Logic_Player::enter_move_scene(void){
	Logic_Heroer::enter_move_scene();
	return 0;
}

int Logic_Player::player_login_gate(int cid) {
	if (!cid) {
		MSG_USER("login error cid:%d role:%ld", cid, role_id());
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(ACTIVE_PLAYER_LOGIN);
	MSG_81000000 msg;
	msg.role_id = role_id();
	monitor_->fetch_gate_link_info(cid, monitor_link_.gate_muid,
			msg.gate_link.ip, msg.gate_link.port);

	msg.serialize(buf);
	buf.finish_message();
	return monitor_->send_to_client_by_cid(cid, buf);
}

int Logic_Player::player_login_info(void) {

	MSG_50400001 msg;
	msg.time = Time_Value::gettimeofday().sec();
	msg.role_id = role_id();
	msg.location = location();
	msg.location.coord = base_detail_->location.coord.pixel_coord();
	THIS_SEND_TO_CLIENT(msg);

	DB_View::ID_Player_View_Map::iterator find_it =
			monitor()->db_view()->id_player_view_map.find(role_id());
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		find_it->second->cancel_account_status(
				Player_DB_View::ACCOUNT_STATUS_TRUSTEESHIP);
	}

	return 0;
}

int Logic_Player::set_scene_muid(Monitor_Unique_ID &muid) {
	player_in_trans_ = false;
	player_in_trans_all_ = false;

	monitor()->scene_change_info(monitor_link_.scene_muid, muid);

	hook_scene_change(monitor_link_.scene_muid.id, muid.id);

	if ((public_detail_->local_his.size() && public_detail_->local_his.back().scene_id == base_detail_->location.scene_id) == false) {
		public_detail_->local_his.push_back(base_detail_->location);
	}

	monitor_link_.scene_muid = muid;
	base_detail_->location.scene_id = muid.id;
	base_detail_->location.scene_order = muid.order;

	while (public_detail_->local_his.size() > 20) { /// 暂定只保留最近的20次场景切换
		public_detail_->local_his.pop_front();
	}

	return 0;
}

int Logic_Player::hook_scene_change(int src_scene, int des_scene) {
	const Scene_Config * s_scene = CONFIG_CACHE_SCENE->scene_config_cache(
			src_scene);
	const Scene_Config * d_scene = CONFIG_CACHE_SCENE->scene_config_cache(
			des_scene);
	if (!s_scene || !d_scene) {
		return 0;
	}
	if (!this->base_detail_->create_scene_args_.slave) {
		if ((Scene_Team == s_scene->manager || Scene_Task_Manager == s_scene->manager
				|| Scene_Team_Defend == s_scene->manager)
				&& (Scene_Team != d_scene->manager && Scene_Task_Manager != d_scene->manager
						&& Scene_Team_Defend != d_scene->manager)) {
			this->leave_team();
		}
	}
	if (HERO_DREAM_Scene_Type == d_scene->type) {
		Team *team = player_self()->get_team();
		if (team && team->is_leader(role_id())) {
			for (Role_Hash_Set::const_iterator it =
					team->team_member_set().begin();
					it != team->team_member_set().end(); ++it) {
				Logic_Player *member =
						LOGIC_MONITOR->find_logic_player_by_role_id(*it);
				if (member && 1 == member->follow()) {
//					member
					member->set_create_scene_args(0, Time_Value::zero,
							team->get_team_id_for_scene(), 0, 0, 0, true);
					member->inner_transmit(des_scene, 0, Coord(), 0, "");
//					member->team_client_ready_broadcast(true);
				}
			}
		}
	}
	if ( Campfire_Scene_Type == s_scene->type ) {
		// 离开篝火时，删除在篝火恢复精力的定时器
		this->remove_event(Logic_Enum::EVENT_LOGIC_ENERGY_COMFIRE_TICK);
	}
	player_self()->hero_hook_scene_change(src_scene, des_scene);

	return 0;
}

void Logic_Player::set_grid_coord(uint16_t x, uint16_t y) {
	base_detail_->location.coord.type = Coord::GRID_TYPE;
	base_detail_->location.coord.x = x;
	base_detail_->location.coord.y = y;
}

/*
 * exist_des_scene exist_scene_order
 * 用于组队副本刷新：
 * 条件: 1.在组队副本
 *      2.组队副本已经完成
 * 动作: 返回组队房间
 * 在场景服时，检测条件，返回exist_des_scene
 * 判断exist_des_scene不为0，检测是否在回滚历史中且队伍存在:
 *      则处理相应的组队回副本操作、让其返回组队场景
 * */
int Logic_Player::scene_rollback(int &des_scene, int &scene_order, Coord &coord,
		bool set, int exist_des_scene, int exist_scene_order, Scene_Type rollback_type) {

	if (public_detail_->local_his.empty()) {
		des_scene = CONFIG_CACHE_ROLE->player_init_cache().safe_scene;
		coord = CONFIG_CACHE_ROLE->player_init_cache().safe_coord;
		scene_order = 0;
	} else {
		while (!public_detail_->local_his.empty()) {
			Location &location = public_detail_->local_his.back();
			public_detail_->local_his.pop_back();
			if (exist_des_scene != 0 && exist_des_scene == location.scene_id) {
				Team *team = this->get_team(); //
				if (team && team->is_in_team(role_id())) {
					exist_des_scene = 0;
					des_scene = location.scene_id;
					scene_order = location.scene_order;
					coord = location.coord;
					break;
				}
			}
			Scene_Type scene_ty = get_scene_type(location.scene_id);
			if (scene_ty != rollback_type) {
				if ( rollback_type == Main_Scene_Type ) {
					if ( scene_ty != Main_Scene_Type && scene_ty != Arena_Panel_Scene_Type )
						continue;
				}else
					continue;
			}
			des_scene = location.scene_id;
			scene_order = location.scene_order;
			coord = location.coord;
			break;
		}
	}

	Monitor_Unique_ID muid;
	if (des_scene == 0 || monitor()->find_scene_muid(des_scene, scene_order, muid)) {
		MSG_USER_TRACE("role:%ld, scene:%d, order:%d, local size:%d", role_id(),
				des_scene, scene_order, public_detail_->local_his.size());
		des_scene = CONFIG_CACHE_ROLE->player_init_cache().safe_scene;
		coord = CONFIG_CACHE_ROLE->player_init_cache().safe_coord;
		scene_order = 0;
	}

	if (set) {
		monitor_link_.scene_muid.id = des_scene;
		monitor_link_.scene_muid.order = scene_order;
		base_detail_->location.scene_id = des_scene;
		base_detail_->location.scene_order = scene_order;
		base_detail_->location.coord = coord;
	}

	/*
	 * 回滚转场景，不发剧情，并次数+1
	 * */
	public_plot_un_broadcast(true);
	return 0;
}

int Logic_Player::scene_transmit(int &des_scene, int scene_order, int64_t arg3,
		Coord des_coord, std::string arg4, int32_t sort, int32_t sub_sort) {
	/*
	 * 传送流程
	 1 同进程:直接改变场景
	 2 不同进程:注销原场景,登录新场景
	 *
	 */



	const Scene_Config *src_scene_config =
			CONFIG_CACHE_SCENE->scene_config_cache(location().scene_id);
	if (!src_scene_config) {
		MSG_USER("role:%ld, scene:%d, order:%d", role_id(), location().scene_id,
				location().scene_order);
		return ERROR_SCENE_NOT_EXIST;
	}

	Coord coord;

	// 退出异空间
	if (!des_scene) {
		if (src_scene_config->type == Main_Scene_Type) {
			MSG_USER("role:%ld, scene:%d, order:%d", role_id(), des_scene,
					scene_order);
			return ERROR_CLIENT_OPERATE;
		} else {
			if(src_scene_config->type == Tbay_Scene_Type){// 宝藏海湾，返回上个单人副本
				scene_rollback(des_scene, scene_order, coord, false, 0, 0, FuBen_Scene_Type);
				if(des_scene == 0){
					scene_rollback(des_scene, scene_order, coord);
				}
			}else{
				scene_rollback(des_scene, scene_order, coord);
			}

		}
	} else {
		coord = des_coord;
	}

	const Scene_Config *des_scene_config =
			CONFIG_CACHE_SCENE->scene_config_cache(des_scene);
	if (!des_scene_config) {
		MSG_USER("role:%ld, scene:%d, order:%d", role_id(), des_scene,
				scene_order);
		return ERROR_SCENE_NOT_EXIST;
	}

	if ((0 != coord.x || 0 != coord.y)
			&& CONFIG_CACHE_SCENE->get_map_value(des_scene_config->map_id,
					coord) <= 0) {
		return ERROR_MOVE_VALUE;
	}

	// 如果是跨服场景就强制修改order
	Monitor_Unique_ID des_muid;
	scene_order = des_scene_config->inter_order >= 0 ? des_scene_config->inter_order : scene_order;
	if (monitor()->find_scene_muid(des_scene, scene_order, des_muid)) {
		MSG_USER("role:%ld, scene:%d, order:%d", role_id(), des_scene,
				scene_order);
		return ERROR_SCENE_NOT_EXIST;
	}

	//TODO:龙谷转转龙谷场景: 暂时特殊处理
	if (Dragon_Vale_Type != des_scene_config->type
			&& des_muid == monitor_link().scene_muid) {
		MSG_USER("same scene, role:%ld, scene:%d, order:%d", role_id(),
				des_scene, scene_order);
		return ERROR_TRANSMIT_SAME_SCENE;
	}

	// 是否允许传送
	int ret = allow_transfer(monitor_link().scene_muid, des_muid, des_scene,
			arg3, arg4);
	if (ret) {
		return ret;
	}

	// 转线人数限制
	if (location().scene_id == des_scene) {
		if (monitor()->scene_role_limit(des_muid))
			return ERROR_SCENE_FULL;
	} else {
		des_muid.order = monitor()->scene_order_free(des_muid);
	}


	// 获取传送坐标
	if (coord.x == 0 && coord.y == 0) {
		ret = transfer_coord(monitor_link().scene_muid, des_muid, des_scene,
				arg3, arg4, coord);
		if (ret)
			return ret;
	}

	// 特殊场景传送
	int ret_special_tran = special_transmit(des_scene, scene_order, arg3,
			des_coord, arg4, sort, sub_sort);
	if (ret_special_tran) {
		return ret_special_tran;
	}

	int des_pidx = monitor()->find_scene_process_idx(des_scene, scene_order);

	int now_pidx = monitor()->find_scene_process_idx(
			monitor_link_.scene_muid.id, monitor_link_.scene_muid.order);

	// 调试版想模拟108多进程可以把==改成!=  调完记的改回来
	int msg_id =
			(des_pidx == now_pidx) ?
					INNER_TRAN_SCENE_SAME_PRO : INNER_ASK_SCENE_SIGN_OUT;

	Block_Buffer buf;
	buf.make_message(msg_id);
	des_muid.serialize(buf);
	coord.grid_coord().serialize(buf);
	base_detail_->create_scene_args_.serialize(buf);
	buf.finish_message();
	ret = this->send_to_scene(buf);

	if (ret == 0 && msg_id == INNER_ASK_SCENE_SIGN_OUT)
		player_in_trans_ = true;

	if(ret == 0) {
		player_in_trans_all_ = true;
	}

	return ret;
}

int Logic_Player::inner_transmit(int &des_scene, int scene_order, int64_t arg3,
		std::string arg4) {
	MSG_DEBUG("inner transmit start, role:%ld. src scene:%d, des scene:%d %d", role_id(), this->location().scene_id, des_scene, scene_order);

	Coord coord;
	MSG_10400005 msg;
	msg.scene_id = des_scene;
	msg.scene_order = scene_order;
	msg.role_id = arg3;
	Block_Buffer buf;
	make_message(buf, INNER_INNER_ASK_GATE_TRAN);
	msg.serialize(buf);
	coord.serialize(buf);
	buf.finish_message();
	return this->send_to_gate(buf);
}

int Logic_Player::inner_transmit(int &des_scene, int scene_order, Coord coord,
		int64_t arg3, std::string arg4) {
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(
			des_scene);
	if (NULL == scene_config) {
		return ERROR_LAYER_ID;
	}
	if ((0 != coord.x || 0 != coord.y)
			&& CONFIG_CACHE_SCENE->get_map_value(scene_config->map_id, coord)
					<= 0) {
		return ERROR_MOVE_VALUE;
	}
	MSG_DEBUG("inner transmit start, role:%ld. src scene:%d, des scene:%d %d", role_id(), this->location().scene_id, des_scene, scene_order);

	MSG_10400005 msg;
	msg.scene_id = des_scene;
	msg.scene_order = scene_order;
	msg.role_id = arg3;
	msg.name = arg4;
	Block_Buffer buf;
	make_message(buf, INNER_INNER_ASK_GATE_TRAN);
	msg.serialize(buf);
	coord.serialize(buf);
	buf.finish_message();
	return this->send_to_gate(buf);
}

int Logic_Player::allow_transfer(const Monitor_Unique_ID &src_muid,
		const Monitor_Unique_ID &des_muid, int &des_scene, int64_t arg3,
		std::string arg4) {
	const Scene_Config *src_scene_config =
			CONFIG_CACHE_SCENE->scene_config_cache(src_muid.id);

	const Scene_Config *des_scene_config =
			CONFIG_CACHE_SCENE->scene_config_cache(des_muid.id);
	if(!src_scene_config || !des_scene_config){
		return ERROR_NOT_ALLOW_ENTER_DES_SCENE;
	}
	switch(src_scene_config->type){
		case FuBen_Scene_Type:{
			if(des_scene_config->type != Main_Scene_Type && des_scene_config->type != Tbay_Scene_Type){
				if(src_scene_config->dungeon.relation == 0 || src_scene_config->dungeon.relation == -1){//
					return ERROR_COPY_IS_GOING;
				}else{// 转多层副本检测
					if(src_scene_config->dungeon.relation != des_scene_config->dungeon.id){
						return ERROR_COPY_IS_GOING;
					}
				}
			}
			break;
		}
		case War_Scene_Type:
		case GANG_WAR_TYPE:
		case Campfire_Scene_Type:
		case World_Boss_Scene_Type:
		case Task_Scene_Type: {// 打副本不能转非主城场景
			if(des_scene_config->type != Main_Scene_Type && des_scene_config->type != Arena_Panel_Scene_Type){
				if(src_scene_config->dungeon.relation == 0 || src_scene_config->dungeon.relation == -1){//
					return ERROR_COPY_IS_GOING;
				}else{// 转多层副本检测
					if(src_scene_config->dungeon.relation != des_scene_config->dungeon.id){
						return ERROR_COPY_IS_GOING;
					}
				}
			}
			break;
		}
		case Elite_Scene_Type:{// 精英推图
			if(des_scene_config->type != Main_Scene_Type){
//				// 是否通关
//				bool is_cur_complete = player_self()->is_elite_dungeon_complete(src_scene_config->dungeon.id);
//				if(is_cur_complete){
//					if(des_scene_config->dungeon.is_cost_times()){
//						int check_result = player_self()->check_elite_tran(des_scene_config->scene_id);
//						if(check_result != 0){
//							return check_result;
//						}
//					}
//				}else{
//					return ERROR_COPY_IS_GOING;
//				}
				// 是否通关
				bool is_cur_complete = player_self()->is_elite_dungeon_complete(src_scene_config->dungeon.id);
				if(is_cur_complete){
					int next_id = src_scene_config->dungeon.next_id;
					if(next_id == des_scene_config->scene_id){
						//相应的单人副本是否打通关
						if(player_self()->is_elite_dungeon_complete(next_id)){
							bool is_unreap = player_self()->is_elite_dungeon_unreap(next_id);
							if(is_unreap){
								return ERROR_SCENE_TRANSMIT_ELITE_HAS_REAP;
							}
						}else{
							int single_id = des_scene_config->dungeon.elite;
							bool is_single_complete = player_self()->check_relation_single_dungeon(single_id);
							if(!is_single_complete){
								return ERROR_SCENE_TRANSMIT_NO_PASS_SGL;
							}
						}

					}else{
						return ERROR_NOT_ALLOW_ENTER_DES_SCENE;
					}
				}else{
					return ERROR_COPY_IS_GOING;
				}
			}
			break;
		}
		case Team_Scene_Type:{// 组队房间
			if(des_scene_config->type != Main_Scene_Type && des_scene_config->type != Team_Fb_Scene_Type
					&& des_scene_config->type != HERO_DREAM_Scene_Type && des_scene_config->type != Task_Scene_Type
					 && des_scene_config->type != Team_Scene_Type
					 && des_scene_config->type != TEAM_DEFEND_Scene_Type
					 && des_scene_config->type != Arena_Panel_Scene_Type){
				return ERROR_COPY_IS_GOING;
			}
			break;
		}
		case HERO_DREAM_Scene_Type:
		case Team_Fb_Scene_Type:
		case TEAM_DEFEND_Scene_Type:{// 组队副本
			if(des_scene_config->type != Main_Scene_Type && des_scene_config->type != Team_Scene_Type){
				if(src_scene_config->dungeon.relation == 0 || src_scene_config->dungeon.relation == -1){//
					return ERROR_COPY_IS_GOING;
				}else{// 转多层副本检测
					if(src_scene_config->dungeon.relation != des_scene_config->dungeon.id){
						return ERROR_COPY_IS_GOING;
					}
				}
			}
			break;
		}
		case Dragon_Vale_Type:{// 打副本不能转非主城场景
			if(des_scene_config->type != Main_Scene_Type &&
					des_scene_config->type != War_Scene_Type &&
					des_scene_config->type != Dragon_Vale_Type &&
					des_scene_config->type != GANG_WAR_TYPE &&
					des_scene_config->type != Campfire_Scene_Type){
				return ERROR_COPY_IS_GOING;
			}
			break;
		}
		case Arena_Panel_Scene_Type:{
			if ( des_scene_config->type != Main_Scene_Type
				   && des_scene_config->type != Team_Scene_Type
				   && des_scene_config->type != GANG_WAR_TYPE
				   && des_scene_config->type != War_Scene_Type)
				return ERROR_NOT_ALLOW_ENTER_DES_SCENE;
			break;
		}
		default:{
			break;
		}
	}

	switch(des_scene_config->type){
		case Expedition_Type: {
			if(player_self()->expedition_detail().coord.x == 0 && player_self()->expedition_detail().coord.y == 0) {
				const Expedition_Common_Cfg *conf =
						CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(Expedition_common_conf_type::LIMIT_LEVEL);
				if(!conf || conf->value == 0) {
					return ERROR_CONFIG_NOT_EXIST;
				}
				if((uint32_t)player_self()->level() < conf->value) {
					return ERROR_EXPEDITION_LEVEL_LIMIT;
				}
				if(!player_self()->ganger_detail().gang_id) {
					return ERROR_EXPEDITION_GANG_LIMIT;
				}
				const Coord *init_coord =
						EXPEDITION_MANAGER->get_coord_by_gang_id(player_self()->ganger_detail().gang_id);
				if(!init_coord) {
					MSG_USER("ERROR_expedition_init_coord");
					return ERROR_EXPEDITION_NOT_READY;
				}
			}
			break;
		}
		case Answer_Type: {
			int res = ANSWER_MANAGER->is_open(player_self());
			if(res) {
				return res;
			}
			break;
		}
		case FuBen_Scene_Type:
		case Elite_Scene_Type:
		case Team_Scene_Type:
		case Team_Fb_Scene_Type:
		case HERO_DREAM_Scene_Type:
		case TEAM_DEFEND_Scene_Type:{// 扫荡中不能打副本
			if(player_self()->is_in_wipe_out()){
				return ERROR_COPY_IS_WIPE_OUT;
			}
			break;
		}
		case Arena_Panel_Scene_Type:{
			if ( src_scene_config->type != Main_Scene_Type
					&& src_scene_config->type != Team_Scene_Type
					&& src_scene_config->type != GANG_WAR_TYPE
					&& src_scene_config->type != War_Scene_Type)
				return ERROR_NOT_ALLOW_ENTER_DES_SCENE;
			break;
		}
//		case Elite_Scene_Type:{
//			if(player_self()->is_in_wipe_out()){
//				return ERROR_COPY_IS_WIPE_OUT;
//			}
//			int check_result = player_self()->check_elite_challenge(des_scene_config->scene_id);
//			if(check_result != 0){
//				return check_result;
//			}
//			break;
//		}
		default: {
			break;
		}
	}

	if (!des_scene_config->des_scene_limit.empty()) {
		if (!des_scene_config->des_scene_limit.count(src_scene_config->type)) {
			MSG_USER("scene :%d not allow to des scene:%d", src_muid.id,
					des_muid.id);
			return ERROR_NOT_ALLOW_ENTER_DES_SCENE;
		}
	}


	switch(des_scene_config->manager){
		case Scene_Gang_War: {
			const Gang_War_Matched_Gang_Info *info = GANG_MANAGER->gang_war_find_matched_gang_info(this->ganger_detail().gang_id);
			if (!info
					|| (Time_Value::gettimeofday() - info->match_time > Time_Value(60*60))) {
				return ERROR_GANG_WAR_ACTIVITY_NOT_OPEN;
			}
			break;
		}
	}

	// 进入等级限制
	if(this->level() < des_scene_config->dungeon.lv){
		return ERROR_FB_ENTER_LV_LIMIT;
	}
	// 所需完成任务检测
	if(des_scene_config->dungeon.require_task != 0 &&
			this->player_self()->is_main_task_commit(des_scene_config->dungeon.require_task) == false){
		return ERROR_FB_ENTER_TASK_LIMIT;
	}
	return 0;
}

int Logic_Player::transfer_coord(const Monitor_Unique_ID &src_muid,
		const Monitor_Unique_ID &des_muid, int &des_scene, int64_t arg3,
		std::string arg4, Coord &coord) {
	// 转线坐标不变
	if (src_muid.id == des_muid.id) {
		coord = this->location().coord;
		return 0;
	}

	// 根据配制取
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(
			des_scene);


	switch(scene_config->type){
		case Expedition_Type: {
			//转送到远征场景: 转送出生点特殊处理
			if(player_self()->expedition_detail().coord.x == 0 && player_self()->expedition_detail().coord.y == 0) {
				const Coord *init_coord =
						EXPEDITION_MANAGER->get_coord_by_gang_id(player_self()->ganger_detail().gang_id);
				if(!init_coord) {
					return ERROR_CLIENT_OPERATE;
				}
				coord = (*init_coord);
			} else {
				coord = player_self()->expedition_detail().coord;
			}
			return 0;
			break;
		}
		default: {

		}
	}


	Scene_Config::Transmit_Coord_Map::const_iterator coord_find_it =
			scene_config->transmit_coord.find(monitor_link().scene_muid.id);
	if (coord_find_it == scene_config->transmit_coord.end()) {
		coord_find_it = scene_config->transmit_coord.find(0);
		if (coord_find_it == scene_config->transmit_coord.end()) {
			MSG_USER("find coord, src_scene:%ld", monitor_link().scene_muid.id);
			return ERROR_SCENE_SRC_COORD;
		}
	}

	coord = coord_find_it->second.coord;

	return 0;
}

int Logic_Player::set_create_scene_args(int type, Time_Value time, role_id_t id,
		int v1, int v2, int v3, bool v4) {
	base_detail_->create_scene_args_.type = type;
	base_detail_->create_scene_args_.time = time;
	base_detail_->create_scene_args_.id = id;
	base_detail_->create_scene_args_.sort = v1;
	base_detail_->create_scene_args_.sub_sort = v2;
	base_detail_->create_scene_args_.int_value = v3;
	base_detail_->create_scene_args_.slave = v4;

	return 0;
}

int Logic_Player::set_create_scene_args(const Create_Scene_Args &csa) {
	base_detail_->create_scene_args_.type = csa.type;
	base_detail_->create_scene_args_.time = csa.time;
	base_detail_->create_scene_args_.id = csa.id;
	base_detail_->create_scene_args_.sort = csa.sort;
	base_detail_->create_scene_args_.sub_sort = csa.sub_sort;
	base_detail_->create_scene_args_.int_value = csa.int_value;
	base_detail_->create_scene_args_.slave = csa.slave;

	return 0;
}

int Logic_Player::set_create_scene_args(bool v4) {
	base_detail_->create_scene_args_.slave = v4;

	return 0;
}

int Logic_Player::set_level(int level) {
	base_detail_->level = level;
	base_detail_->detail_change();
	return 0;
}

int Logic_Player::set_role_name(const std::string& new_name) {
	DB_View::Name_Player_View_Map::iterator n_find_it = LOGIC_MONITOR->db_view()->name_player_view_map.find(base_detail_->role_name);
	if (n_find_it != LOGIC_MONITOR->db_view()->name_player_view_map.end()) {
		monitor()->db_view()->name_player_view_map.insert(std::make_pair(new_name, n_find_it->second));
		monitor()->db_view()->name_player_view_map.erase(n_find_it);
	}

	DB_View::ID_Player_View_Map::iterator find_it =	monitor()->db_view()->id_player_view_map.find(role_id());
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		find_it->second->role_name = new_name;
	}

	base_detail_->role_name = new_name;
	base_detail_->detail_change();
	// 刷新竞技场
	Logic_Arenaer::sign_in();
	// 刷新荣誉竞技场
	{
		int honor_scene_id = 80219001;
		Block_Buffer buf;
		buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
		buf.write_int32(22);
		buf.write_int8(99);
		buf.write_int64(role_id());
		buf.write_string(role_name());
		buf.write_uint8(base_detail_->gender);
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(honor_scene_id, 0, buf);
	}
	return 0;
}

int Logic_Player::modify_gender() {

	if ( base_detail_->gender == 1)
		base_detail_->gender = 2;
	else
		base_detail_->gender = 1;
	base_detail_->detail_change();
	// sync new gender to other server
	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_SYNC_PLAYER_GENDER);
	inner_buf.write_uint8(base_detail_->gender);
	inner_buf.finish_message();
	this->send_to_scene(inner_buf);
	this->send_to_local_chat(inner_buf);
	this->refresh_avatar();

	// refresh ui

	// end refresh ui

	//****sysn dbview
	DB_View::ID_Player_View_Map::iterator find_it =
				monitor()->db_view()->id_player_view_map.find(role_id());
	if ( find_it != monitor()->db_view()->id_player_view_map.end())
		find_it->second->gender = base_detail_->gender;
	//****end sysn
	// 刷新竞技场
	Logic_Arenaer::sign_in();
	// 刷新荣誉竞技场
	{
		int honor_scene_id = 80219001;
		Block_Buffer buf;
		buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
		buf.write_int32(22);
		buf.write_int8(99);
		buf.write_int64(role_id());
		buf.write_string(role_name());
		buf.write_uint8(base_detail_->gender);
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(honor_scene_id, 0, buf);
	}
	return 0;
}

int Logic_Player::set_gang_id(const role_id_t gang_id) {
	base_detail_->gang_id = gang_id;
	base_detail_->detail_change();

	DB_View::ID_Player_View_Map::iterator find_it =
			monitor()->db_view()->id_player_view_map.find(role_id());
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		find_it->second->gang_id = base_detail_->gang_id;
	}

	return 0;
}

int Logic_Player::set_gang_name(const std::string &gang_name) {
	base_detail_->gang_name = gang_name;
	base_detail_->detail_change();

	DB_View::ID_Player_View_Map::iterator find_it =
			monitor()->db_view()->id_player_view_map.find(role_id());
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		find_it->second->gang_name = base_detail_->gang_name;
	}

	return 0;
}

void Logic_Player::hook_fighter_level(const int new_level,
		const int src_level) {
	//警告：不要使用src_level 作为等级变化前的等级， 这是个坑！！！

	Logic_Fighter::hook_fighter_level(new_level, src_level);

	//other level up's listen must after  Logic_Fighter::hook_fighter_level
	//任务监听玩家等级变化
	task_listen_lvl_up(new_level);

	//环任务监听玩家等级变化
	ring_task_listen_player_level_change(new_level);

	//如果满足等级条件，推送开启图标消息
	notify_open_icon_by_level(new_level);

	//龙谷监听玩家等级变化
	dragon_vale_listen_level_up(new_level);
	dragon_vale_sync_player_level_change_to_db_view(new_level);

	//投资计划监听玩家等级变化
	invest_plan_listen_level_up(new_level);

	Logic_Arenaer::hook_fighter_level(new_level, src_level);
	Teamer::hook_fighter_level(new_level, src_level);
	Public::listen_fighter_level(new_level, src_level);
	Achievement::achievement_listen_level(new_level, src_level);
	TEAM_MANAGER->hook_level_change(new_level, *this);
	Logic_Status::special_status_handle();
}

void Logic_Player::hook_fighter_ttl_force(int new_force){
	//监听战斗力;推荐精英副本和竞技场(TOD助手)
	player_self()->helper_listen_fighter_force(new_force);
	//成就监听
	player_self()->achievement_listen_force(new_force);

	Logic_Arenaer::hook_fighter_ttl_force(new_force);
	//成就监听
	Teamer::hook_fighter_force(new_force);

	Achievement::achievement_listen_force(new_force);
}

void Logic_Player::update_ttl_force(void) {
	int ttl_force = fighter_detail().force + get_fight_hero_force();
	detail_ptr_->fighter_detail.ttl_force = ttl_force;

	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(this->role_id());
	if (iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		iter->second->force = detail_ptr_->fighter_detail.force;
		iter->second->ttl_force = ttl_force;
	}

	hook_fighter_ttl_force(ttl_force);
}

int Logic_Player::set_last_sign_out_time(Time_Value& time) {
	base_detail_->last_sign_out_time = time;
	return 0;
}

int Logic_Player::load_base_detail(Base_Detail& detail) {
	base_detail_ = &detail;
	return 0;
}

uint32_t Logic_Player::vip(void) {
	Time_Value now = Time_Value::gettimeofday();
	if (base_detail_->vip_over_time <= now)
		return 0;
	return base_detail_->vip;
}

int Logic_Player::set_vip_level(uint32_t new_vip_level) {
	base_detail_->vip = new_vip_level;
	DB_View::ID_Player_View_Map::iterator iter =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(
					player_self()->role_id());
	if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		MSG_USER("player(role_id=%ld) is not in db_view",
				player_self()->role_id());
	}
	iter->second->vip_level = new_vip_level;
	base_detail_->detail_change();
	return 0;
}

int Logic_Player::set_vip_over_time(const Time_Value &new_time) {
	base_detail_->vip_over_time = new_time;
	DB_View::ID_Player_View_Map::iterator iter =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(
					player_self()->role_id());
	if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		MSG_USER("player(role_id=%ld) is not in db_view",
				player_self()->role_id());
	}
	iter->second->vip_over_time = new_time;
	base_detail_->detail_change();
	return 0;
}

int Logic_Player::set_title(const int title_id) {
	base_detail_->title_id = title_id;
	base_detail_->detail_change();
	return 0;
}

void Logic_Player::add_charm(uint32_t num) {
	base_detail_->charm += num;
	base_detail_->detail_change();
	{	//通知场景服
		MSG_20200022 msg;
		msg.reset();
		msg.num = num;
		THIS_SEND_TO_SCENE(msg);
	}
}

int Logic_Player::special_transmit(int des_scene, int scene_order, int64_t arg3,
		Coord &des_coord, std::string &arg4, int32_t sort, int32_t sub_sort) {
	int ret_special_tran = 0;
	const Scene_Config *des_scene_config =
			CONFIG_CACHE_SCENE->scene_config_cache(des_scene);
	if (!des_scene_config) {
		MSG_USER("role:%ld, scene:%d, order:%d", role_id(), des_scene,
				scene_order);
		return ERROR_SCENE_NOT_EXIST;
	}

	// 龙谷特殊处理
	this->dragon_vale_detail_no_const().target_player_id = 0;
	this->dragon_vale_detail_no_const().is_rob_enter_dragon_vale = false;
	this->dragon_vale_detail_no_const().uuid = 0;

	//任务限时击杀场景特殊处理
	this->tasker_detail_no_const().target_player_id = 0;
	this->tasker_detail_no_const().task_id = 0;

	switch (des_scene_config->type) {
	case FuBen_Scene_Type:{
		bool is_complete = this->is_single_dungeon_complete(des_scene);
		int first_complete = 0;
		if(is_complete){
			first_complete = 1;
		}
		set_create_scene_args(first_complete, Time_Value::zero, arg3, sort, sub_sort, 0, true);
		break;
	}
	case Tbay_Scene_Type:{
		set_create_scene_args(0, Time_Value::zero, arg3, sort, sub_sort, 0, true);
		break;
	}
	case Team_Scene_Type: {
		ret_special_tran = team_manor_transmit(des_scene, 0, Time_Value::zero, arg3, sort, sub_sort, 0, true);
		break;
	}
	case Killing_Scene_Type:{
		if(des_scene_config->dungeon.can_team != 0){
			Team *team = this->get_team();
			if (!team) {
				create_team_timelimit(des_scene, 0, Time_Value::zero, arg3, sort, sub_sort, 0, true);
				int64_t team_id = 0;
				if(this->get_team()){
					team_id = this->get_team()->get_team_id();
				}
				set_create_scene_args(0, Time_Value::zero, team_id, sort, sub_sort, 0, true);
			}
		}else{
			set_create_scene_args(0, Time_Value::zero, role_id(), sort, sub_sort, 0, true);
		}
		break;
	}
	case Expedition_Type: {
		const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(Expedition_common_conf_type::LIMIT_LEVEL);
		if(!conf || conf->value == 0) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if((uint32_t)player_self()->level() < conf->value) {
			return ERROR_EXPEDITION_LEVEL_LIMIT;
		}
		if(!player_self()->ganger_detail().gang_id) {
			return ERROR_EXPEDITION_GANG_LIMIT;
		}
		uint32_t line = EXPEDITION_MANAGER->get_line_by_gang_id(player_self()->ganger_detail().gang_id);
		if(!line) {
			return ERROR_CLIENT_OPERATE;
		}
		int64_t uuid = CONFIG_CACHE->server_flag();
		set_create_scene_args(0, Time_Value::zero, uuid, 0, 0, line, true);
		break;
	}
	case Gang_Manor_Snene_Type: {
		ret_special_tran = gang_manor_transmit();
		break;
	}
	case War_Scene_Type: {
		int league = 1;
		if (arg4 == "dark") {
			league = 2;
		}
		set_create_scene_args(league, Time_Value::zero, arg3, 0, 0, 0, true);
		break;
	}
	case Gang_Boss_Scene_Type: {
		set_create_scene_args(0, Time_Value::zero, arg3, sort, sub_sort, 0, true);
		break;
	}
	case Elite_Scene_Type:{
		int double_income = 0;
		if(des_scene_config->dungeon.difficulty == 1){
			double_income = this->sgl_dungeon_detail().wipe_elite_double?1:0;
		}
		set_create_scene_args(double_income, Time_Value::zero, arg3, sort, sub_sort, 0, true);
		break;
	}
	case Dragon_Vale_Type: {
		this->dragon_vale_detail_no_const().target_player_id = arg3;
		this->dragon_vale_detail_no_const().is_rob_enter_dragon_vale =
				(sort == 1) ? true : false;
		this->dragon_vale_detail_no_const().uuid = sub_sort;
		set_create_scene_args(0, Time_Value::zero, arg3, sort, sub_sort, 0, true);

		((Dragon_Vale*)this)->save_dragon_vale_tick();
		this->sync_dragon_vale_create_args_to_db_view();

		break;
	}
	case GANG_WAR_TYPE: {
//		set_create_scene_args(0, Time_Value::zero, arg3, 0, 0, 0, true);
		break;
	}
	case Team_Fb_Scene_Type:
	case TEAM_DEFEND_Scene_Type: {
		ret_special_tran = multi_team_scene_transmit(des_scene_config);
		break;
	}
	case Task_Scene_Type: {
		this->tasker_detail_no_const().target_player_id = arg3;
		this->tasker_detail_no_const().task_id = sort;
		set_create_scene_args(0, Time_Value::zero, arg3, sort, sub_sort, 0, true);
		ret_special_tran = team_manor_transmit(des_scene, 0, Time_Value::zero, arg3, sort, sub_sort, 0, true);
		((Tasker*)this)->save_tick();
		break;
	}

	default:
		break;

	}
	return ret_special_tran;
}

int Logic_Player::team_manor_transmit(const int des_scene, int type, Time_Value time, role_id_t id,
									  int v1, int v2, int v3, bool v4 /*= false*/) {
	const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(des_scene);
	Team *team = this->get_team();
	if (!team) {
		int32_t scene_id = des_scene;
		if (cfg && cfg->timeLimit) {
			this->create_team_timelimit(scene_id, type, time, id, v1, v2, v3, v4);
		}
	}
	return 0;
}

int Logic_Player::gang_manor_transmit(void) {
	if (!this->ganger_detail().gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	set_create_scene_args(0, Time_Value::zero, this->ganger_detail().gang_id, 0,
			0, 0, true);

	return 0;
}

int Logic_Player::multi_team_scene_transmit(const Scene_Config* dest_cfg) {
	if (!dest_cfg || dest_cfg->dungeon.first_layer_id == 0) {
		return 0;
	}
	// 组队传送检测
	Team *team = this->get_team();
	if (!team) {
		return 0;
	}
	Logic_Player* team_leader = team->get_team_leader();
//	if (!team_leader) {
//		return 0;
//	}
	Role_Hash_Set team_set = team->team_member_set();
//	if (team_set.empty()) {
//		return 0;
//	}
	std::vector<Logic_Player*> team_players;
	team_players.push_back(this);
	Coord coord1 = base_detail().location.coord;
	if (team_leader && team_leader->role_id() != this->role_id()) {
		Coord coord2 = team_leader->base_detail().location.coord;
		if (is_in_range(coord1.grid_coord(), coord2.grid_coord(), 400)) {
			team_players.push_back(team_leader);
		}
	}
	Role_Hash_Set::iterator it;
	for (it = team_set.begin(); it != team_set.end(); ++it) {
		role_id_t t_player_role_id = (*it);
		if (t_player_role_id != role_id()) {
			Logic_Player* t_player =
					LOGIC_MONITOR->find_logic_player_by_role_id(
							t_player_role_id);
			if (t_player
					&& (t_player->base_detail().location.scene_id
							== this->base_detail().location.scene_id)) {
				Coord coord2 = t_player->base_detail().location.coord;
				if (is_in_range(coord1.grid_coord(), coord2.grid_coord(), 400)) {
					team_players.push_back(t_player);
				}
			}
		}
	}
	// 组队传送
	int scene_id = dest_cfg->scene_id;
	int64_t arg3 = 0;
	std::string str = "";

//	if (team_leader->role_id() != this->role_id()) {
//		team_leader->inner_transmit(scene_id, 0, Coord(0, 0), arg3, str);
//	}
//
//	for (it = team_set.begin(); it != team_set.end(); ++it) {
//		role_id_t t_player_role_id = (*it);
//		if (t_player_role_id != role_id()) {
//			Logic_Player* t_player =
//					LOGIC_MONITOR->find_logic_player_by_role_id(
//							t_player_role_id);
//			if (t_player
//					&& (t_player->base_detail().location.scene_id
//							== this->base_detail().location.scene_id)
//					&& (t_player->role_id() != this->role_id())) {
//				team_players.push_back(t_player);
//			}
//		}
//	}
	std::vector<Logic_Player*>::iterator s_it;
	for (s_it = team_players.begin(); s_it != team_players.end(); ++s_it) {
		(*s_it)->set_create_scene_args(0, Time_Value::zero, team->get_team_id_for_scene(), 0, 0, 0, true);
		(*s_it)->inner_transmit(scene_id, 0, Coord(0, 0), arg3, str);
	}
	return 0;
}

void Logic_Player::set_head_id(uint32_t id) {
	base_detail_->head_id = id;
	base_detail_->detail_change();
}

void Logic_Player::reset_fashion_id(void) {
	base_detail_->avatar_vec.clear();
}

void Logic_Player::set_fashion_id(int32_t id) {
	base_detail_->avatar_vec.push_back(id);
}

int Logic_Player::awake_lvl(void) const{
	return base_detail_->awake_lvl;
}

void Logic_Player::add_awake_lvl(void){
	++ base_detail_->awake_lvl;
	base_detail_->detail_change();
}

void Logic_Player::refresh_avatar(void) {

	reset_fashion_id();

	Int_Int_Map avatar_map;
	refresh_equip_avatar(avatar_map);
	refresh_fashion_avatar(avatar_map);

	for (Int_Int_Map::iterator it = avatar_map.begin(); it != avatar_map.end(); ++it) {
		base_detail_->avatar_vec.push_back(it->second);
	}

	refresh_wing_avatar();

	Player_DB_View *target = LOGIC_MONITOR->find_player_db_view_by_role_id(role_id());
	if (0 == target) return;
	target->avatar_vec = base_detail().avatar_vec;

	MSG_20100060 msg;
	msg.avatar_vec = base_detail().avatar_vec;
	THIS_SEND_TO_SCENE(msg);

	base_detail_->detail_change();
}

const bool Logic_Player::get_player_in_trans_all() const {
	return player_in_trans_all_;
}
