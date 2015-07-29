/*
 * Scene_Ganger.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: Linqiyou
 */

#include "Scene_Ganger.h"
#include "gang/Gang_Struct.h"
#include "gang/Gang_Def.h"
#include "Gang_War_Def.h"
#include "Msg_Gang_Struct.h"
#include "Config_Cache_Gang.h"
#include "Scene_Player.h"
#include "Scene_Gang_Global_Manager.h"
#include "Scene_Monitor.h"
#include "Base_Detail.h"
#include "Config_Cache_Scene.h"
#include "Msg_Role_Scene_Struct.h"
#include "Msg_Inner_Enum.h"
#include "mover/Mover_Def.h"
#include "Msg_Active_Struct.h"
#include "Fighter_Detail.h"
#include "gang_war_scene/Gang_War_Scene.h"
#include "chater/Chater_Def.h"
#include "NPC/NPC_Manager.h"
#include "announcement/Annoucement.h"

//const int GANG_WAR_DIE_REVIVE_TIME = 20;

Scene_Ganger::Scene_Ganger() {
	reset();
}

Scene_Ganger::~Scene_Ganger() { }

void Scene_Ganger::reset() {
	ganger_detail_.reset();
	gang_war_info_.reset();
	this->announce_buf_.clear();
	this->notice_buf_.clear();
	point_id_ = 0;
}
int Scene_Ganger::sign_out(void) {
	gang_war_info_.exit_ = true;

	return 0;
}
int Scene_Ganger::time_up(const Time_Value& now){
	gang_war_check_revive(now);
	return 0;
}

int Scene_Ganger::client_scene_complete(void){
	int n_count = notice_buf_.size();
	for(int i=0; i<n_count; ++i){
		send_to_client(notice_buf_[i]);
	}

	int a_count = announce_buf_.size();
	for(int i=0; i<a_count; ++i){
		if(player_self() && player_self()->move_scene()){
			player_self()->move_scene()->broadcast_all(0, announce_buf_[i]);
		}
	}
	notice_buf_.clear();
	announce_buf_.clear();
	return 0;
}

Ganger_Detail& Scene_Ganger::ganger_detail(void) {
	return ganger_detail_;
}
Gang_War_Rank_Detail &Scene_Ganger::gang_war_info(void) {
	return gang_war_info_;
}

int Scene_Ganger::gang_load_detail(Block_Buffer &buf) {
//	if (player_self()->move_scene_id() == GANG_WAR_SCENE_ID) {
		this->ganger_detail_.gang_war_deserialize(buf);

		gang_war_info_.gang_id = this->ganger_detail_.gang_id;
		gang_war_info_.role_id = this->ganger_detail_.role_id;
		gang_war_info_.role_name = player_self()->base_detail().role_name;
		gang_war_info_.headship = this->ganger_detail_.headship;
//		gang_war_info_.league = this->ganger_detail_.league;
//		gang_war_info_.score = this->ganger_detail_.score;
//		gang_war_info_.src_hp = gang_war_info.src_hp;
//		gang_war_info_.hp = gang_war_info.hp;
//		gang_war_info_.kill_num = gang_war_info.kill_num;
//		gang_war_info_.die_num = gang_war_info.die_num;
//		gang_war_info_.tower_num = gang_war_info.tower_num;
//		gang_war_info_.revive = gang_war_info.revive;
//		gang_war_info_.revive_time.set(gang_war_info.revive_time);
//		gang_war_info_.safe_time.set(gang_war_info.safe_time);
//	}
	return 0;
}

int Scene_Ganger::ask_gang_war_rank_info(Block_Buffer &buf) {
	if (player_self() && move_scene()
			&& GANG_WAR_TYPE == get_scene_type(move_scene()->scene_id())) {
		Gang_War_Scene* scene = dynamic_cast<Gang_War_Scene*>(move_scene());
		scene->req_war_scene_fetch_rank(player_self());
	} else {
		MSG_50170037 msg;
		msg.index = -1;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Scene_Ganger::sync_gang_war_to_logic(void) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_GANG_WAR_PLAYER_INFO);
	this->ganger_detail_.gang_war_serialize(buf);
	buf.finish_message();
	this->send_to_logic(buf);
	return 0;
}

int Scene_Ganger::set_gang_war_info(const Gang_War_Rank_Detail &gang_war_info) {
	gang_war_info_.role_id = gang_war_info.role_id;
	gang_war_info_.role_name = gang_war_info.role_name;
	gang_war_info_.gang_name = gang_war_info.gang_name;
	gang_war_info_.headship = gang_war_info.headship;
	gang_war_info_.league = gang_war_info.league;
	gang_war_info_.score = gang_war_info.score;
	gang_war_info_.src_hp = gang_war_info.src_hp;
	gang_war_info_.hp = gang_war_info.hp;
	gang_war_info_.kill_num = gang_war_info.kill_num;
	gang_war_info_.die_num = gang_war_info.die_num;
	gang_war_info_.tower_num = gang_war_info.tower_num;
	gang_war_info_.merit = gang_war_info.merit;
	gang_war_info_.total_merit = gang_war_info.total_merit;
	gang_war_info_.contribute = gang_war_info.contribute;
	gang_war_info_.revive = gang_war_info.revive;
	gang_war_info_.revive_time.set(gang_war_info.revive_time);
	gang_war_info_.safe_time.set(gang_war_info.safe_time);
	gang_war_info_.skill_cd_time.set(gang_war_info.skill_cd_time);
	gang_war_info_.exit_ = gang_war_info.exit_;
	gang_war_info_.wait_time = gang_war_info.wait_time;

	return 0;
}

void Scene_Ganger::update_mover_gang_war_info(void){
	if(player_self()){
		int8_t gather_state = 0;

		const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(ganger_detail().gang_id);
		if (info) {
			if (point_id_ != 0) {
				gather_state = info->camp;
			}
		}

		Appear_Addition app_add;
		app_add.type = MOVER_APPEAR_ADDITION_GANG_WAR;
		// 300:公会战array 资源塔[已走时间、总时间、人数、所属(0未被占领 1红色 2蓝色)] 人物[hp 是否在采集]
		app_add.array.push_back(get_gang_war_hp());
		app_add.array.push_back(gather_state);
		player_self()->set_addition(app_add);
	}
}

int Scene_Ganger::get_gang_war_hp(void){
	Scene_Player* player = this->player_self();
	if(!player){
		return 1000;
	}
	double hp_max = player->blood_max();
	double hp = player->blood_current();
	double hero_hp = 0;
	double hero_hp_max = 0;
	if (hp_max < 1.0) {
		hp_max = 1.0;
	}
//	Hero *hero = SCENE_PUBLIC->find_fight_hero(role_id());
//	if(hero){
//		hero_hp = this->war_info()->get_hero_hp(hero->role_id());
//		if(hero_hp == 0){
//			hero_hp = hero->blood_current();
//		}
//		hero_hp_max = hero->blood_max();
//	}
	int result = (hp+hero_hp)/(hp_max+hero_hp_max)*1000;
	return result;
}

int Scene_Ganger::send_gang_war_player_info(void) {
	MSG_81170001 msg;
	msg.role_id = role_id();

	msg.gang_name = SCENE_GANG_GLOBAL_MANAGER->gang_name(ganger_detail().gang_id);
	const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(ganger_detail().gang_id);
	int64_t scene_index = 0;
	if (info) {
		msg.camp = info->camp;
		scene_index = info->scene_index;
	}
	msg.kill_num = gang_war_info().kill_num;
	msg.hold_spot = gang_war_info().tower_num;
	msg.contribute = gang_war_info().contribute;
	msg.mercit_cur = gang_war_info().merit;
	msg.mercit_all = gang_war_info().total_merit;
	msg.leader_id = SCENE_GANG_GLOBAL_MANAGER->gang_leader_id(ganger_detail().gang_id);
	if(gang_war_info().revive == 1){
		int remain_time = gang_war_info().revive_time.sec() -Time_Value::gettimeofday().sec();
		if(remain_time < 0){
			remain_time = 0;
		}
		msg.revive_time = remain_time;
	}
	msg.skill_cd = gang_war_info().skill_cd_time.sec() - Time_Value::gettimeofday().sec();
	if (msg.skill_cd < 0) {
		msg.skill_cd = 0;
	}
	if (gang_war_info().wait_time > Time_Value::gettimeofday()) {
		msg.wait_time = gang_war_info().wait_time.sec() - Time_Value::gettimeofday().sec();
	}

	const Gang_War_Matched_Gangs_Pair *it_index = SCENE_GANG_GLOBAL_MANAGER->find_matched_gangs_pair_by_gang_id(ganger_detail().gang_id, scene_index);
	if (it_index) {
		role_id_t leader_id_other = 0;
		if (it_index->matched_pair.first != ganger_detail().gang_id) {
			leader_id_other = it_index->matched_pair.first;
		} else {
			leader_id_other = it_index->matched_pair.second;
		}
		Gang_DB_View* gbv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(leader_id_other);
		if (gbv) {
			msg.leader_id_other = gbv->leader;
		}
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}
void Scene_Ganger::gang_war_die_revive(void){
	Scene_Player* player = this->player_self();
	if(move_scene() == NULL || player == NULL){
		return;
	}
	this->gang_war_info().revive = 1;
	this->gang_war_info().revive_time = Time_Value::gettimeofday() + Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().revive_time);
	this->gang_war_set_point_id(0);
//	if(this->move_scene()){
//		this->move_scene()->update_move_able_info(player);
//	}
	//recover hp
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = PT_BLOOD_CURRENT;
	setter.operate = O_SET;
	setter.addi_type = AT_BASIC;
	setter.basic = player->blood_max();
	fight_setter.push_back(setter);
	// player->fight_modify_fight_prop(fight_setter, true);
	player->modify_props_normal(fight_setter, true, 16);

	//back to revive point
	Coord revive_point;
	move_scene()->get_revive_point(revive_point, gang_war_info().league-1);
	if (player->validate_move_value(revive_point.x, revive_point.y)) {
		player->move_disappear();
		player->set_grid_coord(pixel_x_to_grid(revive_point.x), pixel_y_to_grid(revive_point.y), true);
		player->set_pixel_coord(revive_point.x, revive_point.y);
//		player->reset_coord();
	}
	player->send_gang_war_player_info();
}
void Scene_Ganger::gang_war_die_revive_winner(void){
	Scene_Player* player = this->player_self();
	if(move_scene() == NULL || player == NULL){
		return;
	}
//	this->gang_war_info().revive = 1;
//	this->gang_war_info().revive_time = Time_Value::gettimeofday() + Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().revive_time);
//	this->gang_war_set_point_id(0);
//	if(this->move_scene()){
//		this->move_scene()->update_move_able_info(player);
//	}
	//recover hp
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = PT_BLOOD_CURRENT;
	setter.operate = O_SET;
	setter.addi_type = AT_BASIC;
	setter.basic = 1;
	fight_setter.push_back(setter);
	// player->fight_modify_fight_prop(fight_setter, true);
	player->modify_props_normal(fight_setter, true, 15);

	//back to revive point
//	Coord revive_point;
//	move_scene()->get_revive_point(revive_point, gang_war_info().league-1);
//	if (player->validate_move_value(revive_point.x, revive_point.y)) {
//		player->move_disappear();
//		player->set_grid_coord(pixel_x_to_grid(revive_point.x), pixel_y_to_grid(revive_point.y), true);
//		player->set_pixel_coord(revive_point.x, revive_point.y);
//		player->reset_coord();
//	}
//	player->send_gang_war_player_info();
}

void Scene_Ganger::gang_war_check_revive(const Time_Value& now){
	if(gang_war_info().revive == 1){
		if(now >= gang_war_info().revive_time){
			gang_war_info().revive = 0;
			Block_Buffer buf;
			MSG_81000026 msg;
			msg.role_id = this->role_id();
			msg.state = 0;
			MAKE_MSG_BUF(msg, buf);
			this->player_self()->scene_broad_around(buf, Mover::SCENE_MOVE);
//			gang_war_info().safe_time = now + Time_Value(10);

			// update move_able
			if(move_scene()){
				move_scene()->update_move_able_info(player_self());
			}
			this->update_mover_gang_war_info();
		}
	}
	if(gang_war_info().wait_time != Time_Value::zero && gang_war_info().wait_time < Time_Value::gettimeofday()){
		gang_war_info().wait_time = Time_Value::zero;
		if(move_scene()){
			move_scene()->update_move_able_info(player_self());
		}
		this->update_mover_gang_war_info();
	}
}

void Scene_Ganger::reset_gang_war_info(void){
	if(player_self() == NULL){
		return;
	}
	// 恢复血量
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = PT_BLOOD_CURRENT;
	setter.operate = O_SET;
	setter.addi_type = AT_BASIC;
	setter.basic = player_self()->gang_war_info().src_hp;
	fight_setter.push_back(setter);
	// player_self()->fight_modify_fight_prop(fight_setter, true);
	player_self()->modify_props_normal(fight_setter, true, 17);

	player_self()->gang_war_info().reset();
//	player_self()->del_addition(MOVER_APPEAR_ADDITION_GANG_WAR);
//	this->announce_buf_.clear();
//	this->notice_buf_.clear();
}
void Scene_Ganger::gang_war_gather_revive_ball(void){
	if(gang_war_info().revive == 1){
		gang_war_info().revive_time = gang_war_info().revive_time - Time_Value(1);
		this->send_gang_war_player_info();
		// 飘字 复活时间缩短
		this->gang_war_scene_notice(GANG_WAR_NOTICE_REDUCE, "", "", 0, 0, true);
	}
}

int Scene_Ganger::gang_war_scene_notice(int type, std::string name1, std::string name2, int merit, int contribute, bool delay/* = false*/){
	MSG_81170007 msg;
	msg.type = type;
	msg.name1 = name1;
	msg.name2 = name2;
	msg.merit = merit;
	msg.contribute = contribute;
	Block_Buffer buf;
	if(delay){
		MAKE_MSG_BUF(msg, buf);
		notice_buf_.push_back(buf);
	}else{
		THIS_SEND_TO_CLIENT(msg);
	}

	return 0;
}

int Scene_Ganger::gang_war_scene_announce(int type, role_id_t role_id, std::string name, std::string gang_name, int value1, int value2, int value3, bool delay /*= false*/){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	switch(type){
	case GANG_WAR_ANNOUNCE_WAR_WIN:{
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = name;
		ann_contents.push_back(new_content);
		break;
	}
	case GANG_WAR_ANNOUNCE_WAR_START: {
		break;
	}
	case GANG_WAR_ANNOUNCE_DUEL_REQ: {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = SCENE_GANG_GLOBAL_MANAGER->gang_name(this->ganger_detail_.gang_id);
		ann_contents.push_back(new_content);

		new_content.reset();
		role_info.role_id = this->role_id();
		role_info.role_name = role_name();
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);

		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = gang_name;
		ann_contents.push_back(new_content);

		new_content.reset();
		role_info.role_id = role_id;
		role_info.role_name = name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
		break;
	}
	case GANG_WAR_ANNOUNCE_DUEL_REOBJECT: {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = SCENE_GANG_GLOBAL_MANAGER->gang_name(this->ganger_detail_.gang_id);
		ann_contents.push_back(new_content);

		new_content.reset();
		role_info.role_id = this->role_id();
		role_info.role_name = role_name();
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);

		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = gang_name;
		ann_contents.push_back(new_content);

		new_content.reset();
		role_info.role_id = role_id;
		role_info.role_name = name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
		break;
	}
	case GANG_WAR_ANNOUNCE_DUEL_WIN: {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = SCENE_GANG_GLOBAL_MANAGER->gang_name(this->ganger_detail_.gang_id);
		ann_contents.push_back(new_content);

		new_content.reset();
		role_info.role_id = this->role_id();
		role_info.role_name = role_name();
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);

		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = gang_name;
		ann_contents.push_back(new_content);

		new_content.reset();
		role_info.role_id = role_id;
		role_info.role_name = name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
		break;
	}
//	case GANG_WAR_ANNOUNCE_ATTACK: {
//		role_info.role_id = this->role_id();
//		role_info.role_name = role_name();
//		new_content.type = ANN_CTTYPE_ROLE_INFO;
//		new_content.role_Info = role_info;
//		ann_contents.push_back(new_content);
//
//		new_content.reset();
//		new_content.type = ANN_CTTYPE_TEXT;
//		new_content.text = name;
//		ann_contents.push_back(new_content);
//		break;
//	}
//	case GANG_WAR_ANNOUNCE_BE_ATTACK: {
//		role_info.role_id = this->role_id();
//		role_info.role_name = role_name();
//		new_content.type = ANN_CTTYPE_ROLE_INFO;
//		new_content.role_Info = role_info;
//		ann_contents.push_back(new_content);
//
//		new_content.reset();
//		new_content.type = ANN_CTTYPE_TEXT;
//		new_content.text = name;
//		ann_contents.push_back(new_content);
//		break;
//	}
//	case GANG_WAR_ANNOUNCE_HOLD: {
//		role_info.role_id = this->role_id();
//		role_info.role_name = role_name();
//		new_content.type = ANN_CTTYPE_ROLE_INFO;
//		new_content.role_Info = role_info;
//		ann_contents.push_back(new_content);
//
//		new_content.reset();
//		new_content.type = ANN_CTTYPE_TEXT;
//		new_content.text = name;
//		ann_contents.push_back(new_content);
//		break;
//	}
	default:{
		break;
	}
	}

	MSG_83000000 msg;
	msg.chat_type = chat_scene;
	msg.system_type = type;
	msg.content = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	if(delay){
		announce_buf_.push_back(buf);
	}else{
		if (player_self() && player_self()->move_scene()) {
			player_self()->move_scene()->broadcast_all_within_battle(0, buf);
		}
	}
	return 0;
}

int Scene_Ganger::req_gang_war_heal(void){
	if(player_self() && move_scene()){
		Gang_War_Scene* scene = dynamic_cast<Gang_War_Scene*>(move_scene());
		if(scene){
			return scene->req_gang_war_heal(player_self());
		}
	}
	return -1;
}

int Scene_Ganger::gang_war_set_point_id(const int point_id) {
	point_id_ = point_id;
	return point_id_;
}
int Scene_Ganger::gang_war_point_id(void) {
	return point_id_;
}
