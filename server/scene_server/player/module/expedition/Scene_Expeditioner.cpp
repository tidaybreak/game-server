/*
 * Scene_Expeditioner.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: linqiyou
 */

#include "Scene_Expeditioner.h"
#include "expedition/Expedition_Struct.h"
#include "expedition/Expedition_Def.h"
#include "Scene_Player.h"
#include "Scene_Func.h"
#include "expedition_scene/Expedition_Scene.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Expedition_Struct.h"
#include "Config_Cache_Expedition.h"
#include "Msg_Active_Struct.h"
#include "expedition_scene/Expedition_Scene_Manager.h"
#include "Scene_Public.h"
#include "NPC/hero/Hero.h"

Scene_Expeditioner::Scene_Expeditioner(void) {
	reset();
}
Scene_Expeditioner::~Scene_Expeditioner() {
}

int Scene_Expeditioner::load_detail(Block_Buffer &r) {
	expedition_detail_.deserialize(r);
	expedition_scene_info_.role_id = expedition_detail_.role_id;
	return 0;
}

void Scene_Expeditioner::reset(void) {
	expedition_detail_.reset();
	expedition_scene_info_.reset();
}

int Scene_Expeditioner::time_up(const Time_Value& now){
	expedition_check_revive(now);
	return 0;
}

int Scene_Expeditioner::expedition_data_channel(Block_Buffer &r) {
	int8_t type = 0;
	r.read_int8(type);
	switch(type) {
	case Data_Channel::ASK_KILL_BEAST: {
		int point_id = 0;
		r.read_int32(point_id);
		if (Expedition_Type == get_scene_type(player_self()->move_scene_id())) {
//			Expedition_Scene *expedition_scene = (Expedition_Scene*)(player_self()->move_scene());
		}
		break;
	}
	case Data_Channel::ASK_COLLECT_CHEST: {
		int point_id = 0;
		r.read_int32(point_id);
		int8_t tp = 0;
		r.read_int8(tp);
		if (Expedition_Type == get_scene_type(player_self()->move_scene_id())) {
			Expedition_Scene *expedition_scene = (Expedition_Scene*)(player_self()->move_scene());
			Stronghold *sh = expedition_scene->find_map_data_by_point_id(point_id);
			if (sh && sh->type == EBT_CHEST
					&& sh->info.occupier.gang_id == player_self()->ganger_detail().gang_id) {
				Block_Buffer buf;
				buf.make_message(INNER_EXPEDITION_PLAYER);
				buf.write_int8((int8_t)Data_Channel::COULD_COLLECT_CHEST);
				buf.write_int32(point_id);
				buf.write_int8(tp);
				buf.finish_message();
				this->send_to_logic(buf);
			} else {
				if (tp == 0) {
					MSG_50101300 res_msg;
					res_msg.point_id = point_id;
					THIS_SEND_TO_CLIENT(res_msg);
				} else {
					MSG_50101301 res_msg;
					res_msg.point_id = point_id;
					THIS_SEND_TO_CLIENT(res_msg);
				}
			}
		}
		break;
	}
//	case Data_Channel::ASK_COLLECT_BEAST: {
//		int point_id = 0;
//		r.read_int32(point_id);
//		break;
//	}
	case Data_Channel::SYNC_PLAYER_DETAIL: {
		load_detail(r);
		int point_id = 0;
		r.read_int32(point_id);
		if (Expedition_Type == get_scene_type(player_self()->move_scene_id())) {
			Expedition_Scene *expedition_scene = (Expedition_Scene*)(player_self()->move_scene());
			expedition_scene->active_map_data_by_point_id(player_self(), point_id);
		}
		break;
	}
	case Data_Channel::SYNC_CHEST_BY_POINT: {
		int point_id = 0;
		bool is_complete = false;
		r.read_int32(point_id);
		r.read_bool(is_complete);
		if (is_complete) {
			expedition_detail_.collect_chest.insert(point_id);
		}
		if (Expedition_Type == get_scene_type(player_self()->move_scene_id())) {
			Expedition_Scene *expedition_scene = (Expedition_Scene*)(player_self()->move_scene());
			expedition_scene->active_map_data_by_point_id(player_self(), point_id);
		}
		break;
	}
	case Data_Channel::REQ_ACV_DAILY_AWARD: {
		if (Expedition_Type == get_scene_type(player_self()->move_scene_id())) {
			Expedition_Scene *expedition_scene = (Expedition_Scene*)(player_self()->move_scene());
			expedition_scene->active_expedition_daily_award_info(player_self(), true);
		}
		break;
	}
	case Data_Channel::SYNC_REPEAT_TO_SCENE: {
		if (Expedition_Type == get_scene_type(player_self()->move_scene_id())) {
			player_self()->expedition_scene_info().is_enter_scene = 2;
		}
		break;
	}
	case Data_Channel::OCCUPY_CASTLE_AWARD_GET: {
		if (Expedition_Type == get_scene_type(player_self()->move_scene_id())) {
			int result = 0;
			r.read_int32(result);
			Expedition_Scene *expedition_scene = (Expedition_Scene*)(player_self()->move_scene());
			expedition_scene->active_occupy_castle_rank(player_self());
		}
		break;
	}
	default:
		break;
	}
	return 0;
}

int Scene_Expeditioner::update_mover_expedition_info(void) {
	if (!player_self()) {
		return -1;
	}
	Appear_Addition app_add;
	app_add.type = MOVER_APPEAR_ADDITION_EXPEDITION;
	// 500
	app_add.array.push_back(get_hp());
	player_self()->set_addition(app_add);
	return 0;
}

int Scene_Expeditioner::get_hp(void) {
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
	Hero *hero = SCENE_PUBLIC->find_fight_hero(role_id());
	if(hero){
//		hero_hp = this->expedition_scene_info().get_hero_hp(hero->role_id());
//		if(hero_hp == 0){
			hero_hp = hero->blood_current();
//		}
		hero_hp_max = hero->blood_max();
	}
	int result = (hp+hero_hp)/(hp_max+hero_hp_max)*1000;
	return result;
	return 0;
}

int Scene_Expeditioner::set_expedition_scene_info(const Expedition_Scene_Info &info) {
	expedition_scene_info_ = info;
	return 0;
}

int Scene_Expeditioner::reset_expedition_scene_info(void) {
	player_self()->del_addition(MOVER_APPEAR_ADDITION_EXPEDITION);
	expedition_scene_info_.reset();
	return 0;
}

int Scene_Expeditioner::send_expedition_scene_info(void) {
	MSG_81101302 msg;
	msg.type = 1;
//	msg.role_id = role_id();

	if(expedition_scene_info().revive == 1){
		int remain_time = expedition_scene_info().revive_time.sec() -Time_Value::gettimeofday().sec();
		if(remain_time < 0){
			remain_time = 0;
		}
		msg.time = remain_time;
	} else {
		return -1;
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Scene_Expeditioner::expedition_die_revive(void){
	Scene_Player* player = this->player_self();
	if(move_scene() == NULL || player == NULL){
		return;
	}
	this->expedition_scene_info().revive = 1;
	int revive_time = 15;
	const Expedition_Common_Cfg *ecc = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(REVIVE_TIME);
	if (ecc) {
		revive_time = ecc->value;
	}
	this->expedition_scene_info().revive_time = Time_Value::gettimeofday() + Time_Value(revive_time);
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
	player->modify_props_normal(fight_setter, true, 16);

	//back to revive point
	Coord revive_point;
	if(get_scene_type(player->move_scene_id()) == Expedition_Type) {
		Expedition_Scene *es = (Expedition_Scene*) player->move_scene();
		if (es->get_manager()) {
			int point_id = es->get_manager()->find_point_id_by_gang_id(player->ganger_detail().gang_id);
			const Expedition_Map_Point_Cfg *empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
			if (empc) {
				revive_point.type = Coord::PIXEL_TYPE;
				revive_point.x = empc->x;
				revive_point.y = empc->y;
			}
		}
	}
	if (player->validate_move_value(revive_point.x, revive_point.y)) {
		player->move_disappear();
		player->set_grid_coord(pixel_x_to_grid(revive_point.x), pixel_y_to_grid(revive_point.y), true);
		player->set_pixel_coord(revive_point.x, revive_point.y);
//		player->reset_coord();
	}
	player->send_expedition_scene_info();
}

void Scene_Expeditioner::expedition_die_revive_winner(void){
	Scene_Player* player = this->player_self();
	if(move_scene() == NULL || player == NULL){
		return;
	}
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

}

void Scene_Expeditioner::expedition_check_revive(const Time_Value& now){
	if(expedition_scene_info().revive == 1){
		if(now >= expedition_scene_info().revive_time){
			expedition_scene_info().revive = 0;
			Block_Buffer buf;
			MSG_81000026 msg;
			msg.role_id = this->role_id();
			msg.state = 0;
			MAKE_MSG_BUF(msg, buf);
			this->player_self()->scene_broad_around(buf, Mover::SCENE_MOVE);
			expedition_scene_info().safe_time = now + Time_Value(10);

			// update move_able
			if(move_scene()){
				move_scene()->update_move_able_info(player_self());
			}
			this->update_mover_expedition_info();
		}
	}
}

void Scene_Expeditioner::check_defending(void) {
	if (Expedition_Type != get_scene_type(player_self()->move_scene_id())) {
		return;
	}
	Expedition_Scene *es = (Expedition_Scene *)player_self()->move_scene();

	Int_Hash_Set fight_point = expedition_scene_info_.fight_point;
	Int_Hash_Set need_clear;
	for (Int_Hash_Set::const_iterator it = fight_point.begin();
			it != fight_point.end(); ++it) {
		Stronghold *sh = es->find_map_data_by_point_id(*it);
		if (!sh) {
			need_clear.insert(sh->point_id);
			continue;
		}
		if (EBT_BEAST == sh->type) {
			if (!sh->info.role_defend_map_for_beast.count(player_self()->role_id())
					|| !sh->info.role_defend_map_for_beast[player_self()->role_id()].attack_map.count(player_self()->role_id())) {
				need_clear.insert(sh->point_id);
			}
		} else {
			if (!sh->info.attack_map.count(player_self()->role_id())) {
				need_clear.insert(sh->point_id);
			}
		}
	}
	for (Int_Hash_Set::iterator it = need_clear.begin();
			it != need_clear.end(); ++it) {
		expedition_scene_info_.fight_point.erase(*it);
	}
	expedition_scene_info_.look_point.clear();
}

void Scene_Expeditioner::active_fight_status(void) {
	if (!expedition_is_fight_status()) {
		return ;
	}
	MSG_81101302 acv_msg;
	acv_msg.type = 3;
	acv_msg.state = 1;
	THIS_SEND_TO_CLIENT(acv_msg);
}

void Scene_Expeditioner::active_fight_status_for_all(void) {
	MSG_81101302 acv_msg;
	acv_msg.type = 3;
	acv_msg.state = expedition_is_fight_status();
	THIS_SEND_TO_CLIENT(acv_msg);
}

bool Scene_Expeditioner::expedition_is_fight_status(void) {
	if (Expedition_Type != get_scene_type(player_self()->move_scene_id())) {
		return false;
	}

	Expedition_Scene *es = (Expedition_Scene *)player_self()->move_scene();
	for (Int_Hash_Set::const_iterator it = expedition_scene_info_.fight_point.begin();
			it != expedition_scene_info_.fight_point.end(); ++it) {
		Stronghold *sh = es->find_map_data_by_point_id(*it);
		if (!sh) {
			continue;
		}
		if ((sh->info.attacking_map.count(player_self()->role_id())) ) {
				//|| (EBT_BEAST == sh->type
				//		&& (sh->info.role_defend_map_for_beast.count(player_self()->role_id())
				//				&& sh->info.role_defend_map_for_beast[player_self()->role_id()].attacking_map.count(player_self()->role_id())))) {
			return true;
		}
	}
	return false;
}

Expedition_Detail &Scene_Expeditioner::expedition_detail(void) {
	return expedition_detail_;
}

Expedition_Scene_Info &Scene_Expeditioner::expedition_scene_info(void) {
	return expedition_scene_info_;
}
