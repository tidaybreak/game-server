/*
 * Scene_Warer.cpp
 *
 *  Created on: 2014年7月3日
 *      Author: root
 */

#include "Scene_Warer.h"
#include "war_scene/War_Scene.h"
#include "Scene_Public.h"
#include "Scene_Player.h"
#include "NPC/hero/Hero.h"
#include "Msg_Active_Struct.h"
#include "status/Status.h"
#include "status/Status_Struct.h"
#include "war_scene/War_Scene.h"
#include "Config_Cache_Dungeon.h"
#include "Msg_Inner_Enum.h"
#include "announcement/Annoucement.h"
#include "announcement/Announcement_Struct.h"
#include "Msg_Inner_Struct.h"
#include "chater/Chater_Def.h"
#include "Base_Struct.h"

const int REDUCE_CURE_BUF_ID = 32010117;
const int PULL_MINE_BUF_ID = 14030100;
const int BLESS_BUF_ID = 14020100;
const int ESCAPE_BUF_ID = 34201000;
const int WAR_DIE_REVIVE_TIME = 30;

Scene_Warer::Scene_Warer(void){

}

Scene_Warer::~Scene_Warer(void){

}

void Scene_Warer::reset(void){

}

void Scene_Warer::sign_in(void){
	war_info_.role_id = role_id();
	if(this->player_self()){
		war_info_.role_name = this->player_self()->role_name();
	}
}

void Scene_Warer::sign_out(void){

}

int Scene_Warer::time_up(const Time_Value& now){
	check_revive(now);
	return 0;
}

int Scene_Warer::client_scene_complete(void){
	int n_count = notice_buf_.size();
	for(int i=0; i<n_count; ++i){
		send_to_client(notice_buf_[i]);
	}

	int a_count = announce_buf_.size();
	for(int i=0; i<a_count; ++i){
		if(player_self() && player_self()->move_scene() &&
				player_self()->move_scene()->scene_type() == War_Scene_Type){
			player_self()->move_scene()->broadcast_all_within_battle(0, announce_buf_[i]);
		}
	}
	notice_buf_.clear();
	announce_buf_.clear();
	return 0;
}

Scene_War_Rank_Info* Scene_Warer::war_info(){
	return &war_info_;
}

void Scene_Warer::set_war_info(Scene_War_Rank_Info* info){
	war_info_ = *info;
	// buffer
	if(war_info_.is_pull_mine){
		this->add_pull_mine_buf();
	}
	if(war_info_.streak_lose_num > 1){
		int count = war_info_.streak_lose_num - 1;
		if(count > 9){
			count = 9;
		}
		for(int i=0; i<count; ++i){
			this->add_bless_buf();
		}
	}
}

int Scene_Warer::get_war_hp(void){
	Scene_Player* player = this->player_self();
	if(!player){
		return 1000;
	}
	double hp_max = player->blood_max();
	double hp = player->blood_current();
	double hero_hp = 0;
	double hero_hp_max = 0;

	Hero *hero = SCENE_PUBLIC->find_fight_hero(role_id());
	if(hero){
		hero_hp = this->war_info()->get_hero_hp(hero->hero_base_detail().hero_id);
		if(hero_hp == 0){
			hero_hp = hero->blood_current();
		}
		hero_hp_max = hero->blood_max();
	}
	int result = (hp+hero_hp)/(hp_max+hero_hp_max)*1000;
	return result;
}

void Scene_Warer::recover_war_hp(double rate){
	Scene_Player* player = player_self();
	if(!player){
		return;
	}
	double add_hp = player->blood_max()*rate;
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = PT_BLOOD_CURRENT;
	setter.operate = O_ADD;
	setter.addi_type = AT_BASIC;
	setter.basic = add_hp;
	fight_setter.push_back(setter);
	// player->fight_modify_fight_prop(fight_setter, true);
	player->modify_props_normal(fight_setter, true, 20);
	// hero
	Hero *hero = SCENE_PUBLIC->find_fight_hero(role_id());
	if(hero){
		int hero_cur_hp = war_info()->hero_hp_map[hero->hero_base_detail().hero_id];
		if(hero_cur_hp > 0){
			int hero_max_hp = hero->blood_max();
			int add_hero_hp = hero_max_hp*rate;
			hero_cur_hp += add_hero_hp;
			if(hero_cur_hp > hero_max_hp){
				hero_cur_hp = hero_max_hp;
			}
			war_info()->hero_hp_map[hero->hero_base_detail().hero_id] = hero_cur_hp;
		}
	}
}

int Scene_Warer::req_war_scene_fetch_rank(void){
	if(player_self()&& move_scene()){
		War_Scene* scene = dynamic_cast<War_Scene*>(move_scene());
		if(scene){
			return scene->req_war_scene_fetch_rank(player_self());
		}
	}
	return 0;
}

int Scene_Warer::req_war_scene_heal(void){
	if(player_self() && move_scene()){
		War_Scene* scene = dynamic_cast<War_Scene*>(move_scene());
		if(scene){
			return scene->req_war_scene_heal(player_self());
		}
	}
	return -1;
}

int Scene_Warer::war_scene_reduce_revive_time(void){
	if(move_scene() && player_self()){
		War_Scene* scene = dynamic_cast<War_Scene*>(move_scene());
		if(scene){
			if(war_info()->revive == 1){
				war_info()->revive_time = war_info()->revive_time - Time_Value(1);
				send_war_scene_player_info();
			}
		}
	}
	return 0;
}

int Scene_Warer::send_war_scene_player_info(void){
	MSG_81000401 msg;
	msg.info.role_id = role_id();
	msg.info.score = war_info()->score;
	msg.info.kill_num = war_info()->kill_num;
	msg.info.assists_num = war_info()->assists_num;
	msg.info.merit = war_info()->merit;
	msg.info.max_streak_kill_num = war_info()->max_streak_win_num;
	msg.info.total_merit = war_info()->total_merit;
	msg.info.heal_num = war_info()->heal_num;
	msg.info.league = war_info()->league;
	msg.info.reputation = war_info()->rep;
	if(war_info()->revive == 1){
		int remain_time = war_info()->revive_time.sec() -Time_Value::gettimeofday().sec();
		if(remain_time < 0){
			remain_time = 0;
		}
		msg.info.revive_time = remain_time;
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Scene_Warer::war_scene_notice(int type, std::string name1, std::string name2, int score, int merit, int piece, int rep, int arena_coin/* = 0*/, bool delay/* = false*/){
	MSG_81000406 msg;
	msg.type = type;
	msg.name1 = name1;
	msg.name2 = name2;
	msg.score = score;
	msg.merit = merit;
	msg.piece = piece;
	msg.rep = rep;
	msg.arena_coin = arena_coin;
	if(delay){
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		notice_buf_.push_back(buf);
	}else{
		THIS_SEND_TO_CLIENT(msg);
	}

	return 0;
}

int Scene_Warer::war_scene_announce(int type, role_id_t role_id, std::string name, int value1, int value2, int value3, bool delay /*= false*/){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	switch(type){
		case WAR_SCENE_ANNOUNCE_STREAK_WIN_3:
		case WAR_SCENE_ANNOUNCE_STREAK_WIN_4:
		case WAR_SCENE_ANNOUNCE_STREAK_WIN_5:
		case WAR_SCENE_ANNOUNCE_STREAK_WIN_6:
		case WAR_SCENE_ANNOUNCE_STREAK_WIN_7:
		case WAR_SCENE_ANNOUNCE_STREAK_WIN_8:{
			role_info.role_id = this->role_id();
			role_info.role_name = role_name();
			new_content.type = ANN_CTTYPE_ROLE_INFO;
			new_content.role_Info = role_info;
			ann_contents.push_back(new_content);

			role_info.reset();
			role_info.role_id = role_id;
			role_info.role_name = name;
			new_content.reset();
			new_content.type = ANN_CTTYPE_ROLE_INFO;
			new_content.role_Info = role_info;
			ann_contents.push_back(new_content);

			std::stringstream s_stream;
			s_stream<<value1;
			new_content.reset();
			new_content.type = ANN_CTTYPE_TEXT;
			new_content.text = s_stream.str();
			ann_contents.push_back(new_content);
			break;
		}
		case WAR_SCENE_ANNOUNCE_END_STREAK_WIN:{
			role_info.role_id = this->role_id();
			role_info.role_name = role_name();
			new_content.type = ANN_CTTYPE_ROLE_INFO;
			new_content.role_Info = role_info;
			ann_contents.push_back(new_content);

			role_info.reset();
			role_info.role_id = role_id;
			role_info.role_name = name;
			new_content.reset();
			new_content.type = ANN_CTTYPE_ROLE_INFO;
			new_content.role_Info = role_info;
			ann_contents.push_back(new_content);

			std::stringstream s_stream1;
			s_stream1<<value1;
			new_content.reset();
			new_content.type = ANN_CTTYPE_TEXT;
			new_content.text = s_stream1.str();
			ann_contents.push_back(new_content);

			std::stringstream s_stream2;
			s_stream2<<value2;
			new_content.reset();
			new_content.type = ANN_CTTYPE_TEXT;
			new_content.text = s_stream2.str();
			ann_contents.push_back(new_content);

			std::stringstream s_stream3;
			s_stream3<<value3;
			new_content.reset();
			new_content.type = ANN_CTTYPE_TEXT;
			new_content.text = s_stream3.str();
			ann_contents.push_back(new_content);
			break;
		}
		case WAR_SCENE_ANNOUNCE_KILL_GUARD:{
			role_info.role_id = this->role_id();
			role_info.role_name = role_name();
			new_content.type = ANN_CTTYPE_ROLE_INFO;
			new_content.role_Info = role_info;
			ann_contents.push_back(new_content);

			new_content.reset();
			new_content.type = ANN_CTTYPE_TEXT;
			new_content.text = name;
			ann_contents.push_back(new_content);
			break;
		}
		case WAR_SCENE_ANNOUNCE_KILL_L_OFFICER:
		case WAR_SCENE_ANNOUNCE_KILL_D_OFFICER:{
			role_info.role_id = this->role_id();
			role_info.role_name = role_name();
			new_content.type = ANN_CTTYPE_ROLE_INFO;
			new_content.role_Info = role_info;
			ann_contents.push_back(new_content);

			break;
		}
		case WAR_SCENE_ANNOUNCE_L_OFFICE_REVIVE:
		case WAR_SCENE_ANNOUNCE_D_OFFICE_REVIVE:
		case WAR_SCENE_ANNOUNCE_WAR_CLOSE:
		case WAR_SCENE_ANNOUNCE_WAR_L_WIN:
		case WAR_SCENE_ANNOUNCE_WAR_D_WIN:{
			break;
		}
		default:{
			break;
		}
	}

//	MSG_20500000 msg;
//	msg.district = ANNOUNCE_DISTRICT_SYSTEM;
//	msg.announce_id = type;
//	msg.contents = ann_contents;
//	//msg.recievers = recievers;
//	Block_Buffer buf;
//	buf.make_message(msg.msg_id);
//	msg.serialize(buf);
//	buf.finish_message();
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
		//send_to_local_chat(buf);
		if(player_self() && player_self()->move_scene()){
			player_self()->move_scene()->broadcast_all_within_battle(0, buf);
		}
	}
	return 0;
}

void Scene_Warer::war_die_revive(void){
	Scene_Player* player = this->player_self();
	if(move_scene() == NULL || player == NULL){
		return;
	}
	this->war_info()->revive = 1;
	this->war_info()->revive_time = Time_Value::gettimeofday() + Time_Value(WAR_DIE_REVIVE_TIME);
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
	//player->fight_modify_fight_prop(fight_setter, true);
	player->modify_props_normal(fight_setter, true, 21);

	//back to revive point
	Coord revive_point;
	move_scene()->get_revive_point(revive_point, war_info()->league-1);
	if (player->validate_move_value(revive_point.x, revive_point.y)) {
		player->move_disappear();
		player->set_grid_coord(pixel_x_to_grid(revive_point.x), pixel_y_to_grid(revive_point.y), true);
		player->set_pixel_coord(revive_point.x, revive_point.y);
//		player->reset_coord();
	}
}

void Scene_Warer::check_revive(const Time_Value& now){
	if(war_info()->revive == 1){
		if(now >= war_info()->revive_time){
			war_info()->revive = 0;
			Block_Buffer buf;
			MSG_81000026 msg;
			msg.role_id = this->role_id();
			msg.state = 0;
			MAKE_MSG_BUF(msg, buf);
			this->player_self()->scene_broad_around(buf, Mover::SCENE_MOVE);
			war_info()->safe_time = now + Time_Value(10);

			if(war_info()->add_streak_lose_num){
				war_info()->streak_lose_num += 1;
				war_info()->add_streak_lose_num = false;
				// 飘字通知 女神祝福
				if(war_info()->streak_lose_num > 1 &&
						war_info()->streak_lose_num < 11){
					add_bless_buf();
					war_scene_notice(WAR_SCENE_NOTICE_BLESS, "", "", 0, 0, 0, 0);
				}
			}

			// update move_able
			if(move_scene()){
				move_scene()->update_move_able_info(player_self());
			}
			this->update_mover_war_info();
		}
	}
}

bool Scene_Warer::is_safe_time(void){
	if(Time_Value::gettimeofday() >= war_info()->safe_time){
		return false;
	}
	return true;
}

void Scene_Warer::add_pull_mine_buf(void){
	if(player_self()){
		//int move_speed = player_self()->
		Status_Effect_Argv argv;
		argv.caster_id = this->role_id();
		argv.status_id = PULL_MINE_BUF_ID;
		argv.last_time_ms = -1;
		Effect_Argv_Prop prop;
		prop.i_effect_val1 = 507205;
		prop.d_effect_val1 = -76;
		argv.data.push_back(prop);
		player_self()->insert_status_out_fight(argv);
	}
}

void Scene_Warer::del_pull_mine_buf(void){
	if(player_self() && move_scene()){
		int result = player_self()->erase_status_type(PULL_MINE_BUF_ID);
		if(result == 0){
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = PULL_MINE_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			player_self()->move_scene()->broadcast_all_within_battle(0, buf);
		}
	}
}

void Scene_Warer::clear_pull_mine_buf(void){
	if(player_self()){
		int result = player_self()->erase_status_type(PULL_MINE_BUF_ID);
		if(result == 0){
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = PULL_MINE_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			this->send_to_client(buf);
		}
	}
}

void Scene_Warer::add_bless_buf(void){
	if(player_self() && war_info()->streak_lose_num > 1 &&
			war_info()->streak_lose_num < 11){
		Status_Effect_Argv argv;
		argv.caster_id = this->role_id();
		argv.status_id = BLESS_BUF_ID;
		argv.last_time_ms = -1;
		Effect_Argv_Prop prop;
		for(int i=0; i<8; ++i){
			prop.i_effect_val1 = 505605+i*100;
			prop.d_effect_val2 = 0.15;
			argv.data.push_back(prop);
		}
		prop.i_effect_val1 = 506405;
		prop.d_effect_val2 = 0.15;
		argv.data.push_back(prop);
		player_self()->insert_status_out_fight(argv);
	}
}

void Scene_Warer::add_escape_buf(void){
	if(player_self()){
		//int move_speed = player_self()->
		Status_Effect_Argv argv;
		argv.caster_id = this->role_id();
		argv.status_id = ESCAPE_BUF_ID;
		argv.last_time_ms = 5*60*1000;
		Effect_Argv_Prop prop;
		prop.i_effect_val1 = 500306;
		prop.d_effect_val1 = 0;
		argv.data.push_back(prop);
		player_self()->insert_status_out_fight(argv);
	}
}

void Scene_Warer::del_bless_buf(void){
	if(player_self() && move_scene()){
		int result = player_self()->erase_status_type(BLESS_BUF_ID);
		if(result == 0){
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = BLESS_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			player_self()->move_scene()->broadcast_all_within_battle(0, buf);
		}
	}
}

void Scene_Warer::clear_bless_buf(void){
	if(player_self()){
		int result = player_self()->erase_status_type(BLESS_BUF_ID);
		if(result == 0){
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = BLESS_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			this->send_to_client(buf);
		}
	}
}

void Scene_Warer::add_reduce_cure_buf(void){
	if(player_self()){
		//int move_speed = player_self()->
		Status_Effect_Argv argv;
		argv.caster_id = this->role_id();
		argv.status_id = REDUCE_CURE_BUF_ID;
		argv.last_time_ms = -1;
		Effect_Argv_Prop prop;
		prop.i_effect_val1 = 519007;
		prop.d_effect_val1 = -0.5;
		argv.data.push_back(prop);
		player_self()->insert_status_out_fight(argv);
	}
}
void Scene_Warer::del_reduce_cure_buf(void){
	if(player_self() && move_scene()){
		int result = player_self()->erase_status_type(REDUCE_CURE_BUF_ID);
		if(result == 0){
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = REDUCE_CURE_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			player_self()->move_scene()->broadcast_all_within_battle(0, buf);
		}
	}
}
void Scene_Warer::clear_reduce_cure_buf(void){
	if(player_self()){
		int result = player_self()->erase_status_type(REDUCE_CURE_BUF_ID);
		if(result == 0){
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = REDUCE_CURE_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			this->send_to_client(buf);
		}
	}
}

void Scene_Warer::forbid_enter_war(void){
	int sync_type = 2;
	Time_Value forbid_time = Time_Value::gettimeofday() + Time_Value(60*5);
	Block_Buffer buf;
	buf.make_message(INNER_WAR_SCENE_SYNC_INFO);
	buf.write_int8(sync_type);
	forbid_time.serialize(buf);
	buf.finish_message();
	this->send_to_logic(buf);
	return;
}

void Scene_Warer::update_mover_war_info(void){
	if(player_self()){
		Appear_Addition app_add;
		app_add.type = MOVER_APPEAR_ADDITION_WAR;
		// 100: 战场附加信息 array 【击杀数、被杀数、hp、是否拉矿】
		app_add.array.push_back(player_self()->war_info()->streak_win_num);
		app_add.array.push_back(player_self()->war_info()->streak_lose_num);
		app_add.array.push_back(player_self()->get_war_hp());
		app_add.array.push_back(player_self()->war_info()->is_pull_mine);
		player_self()->set_addition(app_add);
	}
}

int Scene_Warer::add_merit_contribute_to_logic(const int add_merit, const int add_contribute){
	int8_t sync_type = 0;
	Block_Buffer buf;
	buf.make_message(INNER_WAR_SCENE_SYNC_INFO);
	buf.write_int8(sync_type);
	buf.write_int32(add_merit);
	buf.write_int32(add_contribute);
	buf.finish_message();
	this->send_to_logic(buf);
	return 0;
}

int Scene_Warer::add_merit_piece_to_logic(const int add_merit, const int add_piece, const int add_rep, const int add_arena_coin, std::vector<Id_Int_Int> props){
	int8_t sync_type = 1;
	Block_Buffer buf;
	buf.make_message(INNER_WAR_SCENE_SYNC_INFO);
	buf.write_int8(sync_type);
	buf.write_int32(add_merit);
	buf.write_int32(add_piece);
	buf.write_int32(add_rep);
	buf.write_int32(add_arena_coin);
	int prop_count = props.size();
	buf.write_int32(prop_count);
	for(int i = 0; i < prop_count; ++i){
		props[i].serialize(buf);
	}
	buf.finish_message();
	this->send_to_logic(buf);
	return 0;
}

int Scene_Warer::sync_forbid_time_to_logic(const Time_Value& time){
	int8_t sync_type = 2;
	Block_Buffer buf;
	buf.make_message(INNER_WAR_SCENE_SYNC_INFO);
	buf.write_int8(sync_type);
	time.serialize(buf);
	buf.finish_message();
	this->send_to_logic(buf);
	return 0;
}

void Scene_Warer::reset_war_info(void){
	if(player_self() == NULL){
		return;
	}
	player_self()->clear_bless_buf();
	player_self()->clear_pull_mine_buf();
	player_self()->clear_reduce_cure_buf();
	// 恢复血量
//	Prop_Setter_Vec fight_setter;
//	Prop_Setter setter;
//	setter.prop_type = PT_BLOOD_CURRENT;
//	setter.operate = O_SET;
//	setter.addi_type = AT_BASIC;
//	setter.basic = player_self()->war_info()->src_hp;
//	fight_setter.push_back(setter);
//	player_self()->fight_modify_fight_prop(fight_setter, true);

	player_self()->war_info()->reset();
	player_self()->del_addition(MOVER_APPEAR_ADDITION_WAR);
	this->announce_buf_.clear();
	this->notice_buf_.clear();
}

void Scene_Warer::gather_revive_ball(void){
	if(war_info()->revive == 1){
		war_info()->revive_time = war_info()->revive_time - Time_Value(1);
		this->send_war_scene_player_info();
		// 飘字 复活时间缩短
		this->war_scene_notice(WAR_SCENE_NOTICE_REDUCE, "", "", 0, 0, 0, 0);
	}
}
