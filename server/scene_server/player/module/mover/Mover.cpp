/*
 * Mover.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "Mover.h"
#include "Mover_Struct.h"
#include "Move_Scene.h"
#include "Battle_Scene.h"
#include "Scene_Module_Head.h"
#include "Err_Code.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "Scene_Public.h"
#include "NPC/hero/Hero.h"

Mover::Mover() : mover_detail_(0),
move_scene_(0),
battle_scene_(0) {

}

Mover::~Mover() {
	// TODO Auto-generated destructor stub
}

void Mover::reset(void) {
	move_scene_ = 0;
	mover_detail_ = 0;
	battle_scene_ = 0;
	appear_buff_.reset();
	CLEAR_VECTOR(Appear_Prop_Vec, addition_prop_);
}

void Mover::set_detail_prt(Mover_Detail &detail) {
	mover_detail_ = &detail;
}

void Mover::module_init(void) {

}

void Mover::sign_in(void) {

}

void Mover::sign_out(void) {

}

int Mover::mover_type(void) {
	return mover_detail().mover_type;
}

const Coord &Mover::grid_coord(void) {
	return mover_detail().grid_coord;
}

const Coord &Mover::pixel_coord(void) {
	return mover_detail().pixel_coord;
}

const Position_Key &Mover::battle_position(void) {
	return mover_detail_->battle_position;
}

const Position_Key &Mover::cur_battle_position(void) {
	return mover_detail_->cur_battle_position;
}

const Position_Key &Mover::team_position(void) {
	return mover_detail_->team_position;
}

void Mover::set_block_index(int idx) {
	mover_detail_->block_index = idx;
}

void Mover::set_layer_id(int id) {
	mover_detail_->layer_id = id;
}

void Mover::set_toward(int toward) {
	mover_detail_->toward = toward;
}

void Mover::set_grid_coord(uint16_t x, uint16_t y, bool reset_idx) {
	mover_detail_->grid_coord.x = x;
	mover_detail_->grid_coord.y = y;

	if (reset_idx) {
		uint16_t new_block_index = 0;
		if (this->validate_mover_scene()) {
			move_scene()->calculate_block_index_by_grid_coord_s(x, y, new_block_index);
			set_block_index(new_block_index);
		}
	}
}

void Mover::set_pixel_coord(uint16_t x, uint16_t y) {
	mover_detail_->pixel_coord.x = x;
	mover_detail_->pixel_coord.y = y;
}

int Mover::set_battle_position(int pos, int idx, Battle_Scene *battle_scene) {
	if (!battle_scene) {
		MSG_USER_TRACE("has battle scene, role:%ld battle_id:%d, b_id:%d", role_id(), battle_scene_id());
		return -1;
	}

	if (this->player_self()) {
		// tmp code
		Hero *hero = SCENE_PUBLIC->find_fight_hero(player_self()->role_id());
		if (hero) {
			Position_Key player_pos_key(pos, idx);
			Position_Key hero_pos_key(pos, hero->hero_const_base_detail().hero_formation);
			player_self()->player_pos_key_fit_hero(player_pos_key, hero_pos_key);
			pos = player_pos_key.pos;
			idx = player_pos_key.idx;
			hero->set_pos_key(hero_pos_key.pos, hero_pos_key.idx);
		}
	}

	// 已经在有战斗场景？
	if (battle_scene_) {
		MSG_TRACE_ABORT("has battle scene, role:%ld battle_id:%d, b_id:%d", role_id(), battle_scene_id());
		return -2;
	}

	// 位置是否有人用过？
	if (battle_scene) {
		Battle_Scene::Debug_Mover_Map::iterator find_it = battle_scene->pos_debug_map().find(Position_Key(pos, idx));
		if (find_it != battle_scene->pos_debug_map().end()) {
			Mover *mover = battle_scene->find_mover_with_position(Position_Key(pos, idx));
			role_id_t mover_map_id = mover ? mover->role_id() : 0;
			MSG_TRACE_ABORT("pos has exist, role:%ld exist role:%ld, battle_id:%d, pos:%d  %d, map_m_id:%ld", role_id(), find_it->second, battle_scene->battle_id(), pos, idx, mover_map_id);
			return -3;
		}
	}

	// 场景状态是否正常？
	if (battle_scene->check_battle_over_before_appear()) {
		MSG_USER_TRACE("battle over, role_id=%ld, battle_type=%d, battle_state=%d",
				role_id(), battle_scene->get_battle_type(), battle_scene->battle_status());
		return -4;
	}


	mover_detail_->battle_position.pos = pos;
	mover_detail_->battle_position.idx = idx;
	mover_detail_->cur_battle_position.pos = pos;
	mover_detail_->cur_battle_position.idx = idx;
	return 0;
}

void Mover::set_cur_battle_position(int pos, int idx) {
	mover_detail_->cur_battle_position.pos = pos;
	mover_detail_->cur_battle_position.idx = idx;
}

void Mover::set_team_position(int pos, int idx) {
	mover_detail_->team_position.pos = pos;
	mover_detail_->team_position.idx = idx;
}

void Mover::fight_tick(const Time_Value &now) {

}

int Mover::move_appear_info(Block_Buffer &buf) {

	return 0;
}

int Mover::make_disappear_info(Block_Buffer &buf) {
	buf.make_message(ACTIVE_MOVER_DISAPPEAR);
	MSG_81000010 msg;
	buf.write_uint16(1);
	buf.write_int64(role_id());
	buf.finish_message();
	return 0;
}

int Mover::move_scene_id(void) {
	if(move_scene_) return move_scene_->scene_id();
	return -1;
}

int Mover::move_scene_order(void) {
	if(move_scene_) return move_scene_->scene_order();
	return -1;
}

int Mover::battle_scene_id(void) {
	if(battle_scene_) return battle_scene_->battle_id();
	return -1;
}

int Mover::make_move_info(Block_Buffer &buf, uint8_t toward, uint16_t x, uint16_t y) {

	return 0;
}

int Mover::enter_battle_scene(Battle_Scene *scene) {
	if (battle_scene_) {
		MSG_TRACE_ABORT("has battle scene, role:%ld battle_id:%d, b_id:%d", role_id(), battle_scene_id(), scene->battle_id());
		return -1;
	}

	if (scene && scene->validate_position(battle_position().pos, battle_position().idx)) {
		appear_buff().set_ext_flag(true);
		battle_scene_ = scene;

		battle_scene_->enter_scene(this);
	} else {
		MSG_USER_TRACE("scene error, role:%d p:%d, idx:%d", role_id(), battle_position().pos, battle_position().idx);
	}

	return 0;
}

int Mover::exit_battle_scene(int exit_from) {
	if (validate_battle_scene()) {
		appear_buff().set_ext_flag(true);
		battle_disappear();
		battle_scene_->exit_scene(this);
		battle_scene_ = NULL;
		mover_detail_->battle_position.reset();

	} else {
		MSG_USER_TRACE("scene error, role:%ld, battle scene:%d", role_id(), battle_scene_id());
	}
	return 0;
}

int Mover::battle_appear(void) {
	int ret = 0;
	if (validate_battle_scene()) {
		ret = battle_scene_->appear(this);
	} else {
		ret = ERROR_SCENE_NULL;
		MSG_USER_TRACE("mover appear error, scene:%d is NULL, mover id:%ld  index:%d", move_scene_id(), role_id(), mover_detail().block_index);
	}
	return 0;
}

int Mover::battle_disappear(void) {
	int ret = 0;
	if (validate_battle_scene()) {
		if (battle_scene_->find_mover_with_position(mover_detail_->battle_position))
			ret = battle_scene_->disappear(this);
	} else {
		ret = ERROR_SCENE_NULL;
		MSG_USER_TRACE("mover disappear error, scene:%d is NULL, mover id:%ld  index:%d", move_scene_id(), role_id(), mover_detail().block_index);
	}
	return 0;
}

int Mover::scene_broad_around(Block_Buffer &buf, Mover_Scene_Type scene_type) {

	if ((scene_type == SCENE_ALL || scene_type == SCENE_MOVE) && validate_mover_scene()) {
		move_scene()->broadcast_around(mover_detail().layer_id, mover_detail().block_index, buf);
	}

	if ((scene_type == SCENE_ALL || scene_type == SCENE_BATTLE) && validate_battle_scene()) {
		Mover *mover = battle_scene()->find_mover_with_position(mover_detail().battle_position);
		if (mover) {
			if (mover->role_id() != role_id()) {
				this->send_to_client(buf);
				mover = battle_scene()->find_mover_with_role_id(role_id());
				if (!mover) {
					MSG_USER_TRACE("broad error, role:%ld", role_id());
				} else {
					MSG_USER_TRACE("broad error, role:%ld", role_id());
				}
			} else {
				battle_scene()->broadcast_around(buf);
			}
		} else {
			this->send_to_client(buf);
			mover = battle_scene()->find_mover_with_role_id(role_id());
			if (!mover) {
				//MSG_USER_TRACE("broad error, role:%ld", role_id());
			} else {
				MSG_USER_TRACE("broad error, role:%ld", role_id());
			}
		}
	}

	return 0;
}

void Mover::add_moveable_value(int16_t value){
	if(mover_detail_){
		mover_detail_->moveable_value.insert(value);
	}
}

void Mover::del_moveable_value(int16_t value){
	if(mover_detail_){
		mover_detail_->moveable_value.erase(value);
	}
}

void Mover::clear_moveable_value(void){
	if(mover_detail_){
		mover_detail_->moveable_value.clear();
	}
}

int Mover::battle_appear_info(Block_Buffer &buf) {
	return 0;
}


int Mover::move(Coord_Vec &coord) {
	int ret = 0;
	ret = move_able();
	if (ret) {
		return ret;
	}

	for (std::vector<Coord>::iterator it = coord.begin(); it != coord.end(); ++it) {
		Time_Value now = Time_Value::gettimeofday();
		//if (!mover_type() || mover_type() == MOVER_T_HERO)
		//	MSG_DEBUG("role:%ld  %d  %d", role_id(), (*it).x, (*it).y);

		if (mover_detail().pixel_coord == *it) /// 客户端坑 忽略重复格子
			continue;

		if (! move_scene_->validate_pix_coord(it->x, it ->y)) {
			MSG_USER("pix_coord error, role:%ld, pix_x = %d, pix_y = %d.", role_id(), it->x, it ->y);
			return ERROR_MOVE_TO_TARGET_COORD;
		}

		if (!validate_move_value(it->x, it ->y)) {
			MSG_USER("validate_move_value, role:%ld, scene:%d, coord:%d %d", role_id(), move_scene_id(), it->x, it ->y);
			return ERROR_MOVE_VALUE;
		}

		if (!is_adjacent_coord(it->x, it ->y)) {
			return ERROR_MOVE_NOT_ADJACENT;
		}

		int ret = move_scene_->move(this, it->x, it ->y);
		if (ret) {
			return ret;
		}
	}

	return 0;
}

int Mover::reset_coord(void) {
	MSG_81000015 msg;
	msg.role_id = role_id();
	msg.coord = mover_detail_->pixel_coord;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

bool Mover::validate_mover_scene(void) {
	return (move_scene_ && move_scene_->scene_id());
}

bool Mover::validate_battle_scene(void) {
	return (battle_scene_ && battle_scene_->battle_id());
}

int Mover::validate_move_value(uint16_t &x, uint16_t &y) {
	int moveable = move_scene_->mpt_value_by_coord(pixel_x_to_grid(x), pixel_y_to_grid(y));
	if (moveable == Move_Scene::UNMOVE_ABLE_VAL)
		return false;

	if (moveable != Move_Scene::MOVE_ABLE_FROM) {
		if (mover_detail_->validate_moveable(moveable))
			return true;
		return false;
	}

	return true;
}

int Mover::change_coord_by_pixel(uint16_t &x, uint16_t &y) {
	if (validate_move_value(x, y)) {
		this->move_disappear();
		this->set_grid_coord(pixel_x_to_grid(x), pixel_y_to_grid(y), true);
		this->set_pixel_coord(x, y);
		reset_coord();
		this->move_appear();
	} else {
		MSG_USER("error coord, role:%ld", role_id());
	}

	return 0;
}

int Mover::move_able(void) {
	if (is_in_battle()) {
		// 会打很多，先注释
		//MSG_DEBUG("role:%ld, in battle can not move", role_id());
		return ERROR_DIS_MOVE_BATTLE;
	}
	return 0;
}

int Mover::enter_move_scene(Move_Scene *scene) {
	if (scene) {
		move_scene_ = scene;

		uint16_t new_block_index = 0;
		move_scene_->calculate_block_index_by_grid_coord_s(mover_detail().grid_coord.x, mover_detail().grid_coord.y, new_block_index);
		set_block_index(new_block_index);

		if (! move_scene_->validate_layer_id(mover_detail().layer_id)) {
			MSG_DEBUG("check layer_id:%d try to layer:0", mover_detail().layer_id);
			set_layer_id(0);
		}

		move_scene_->enter_scene(this);
	} else {
		MSG_USER_TRACE("scene error, role:%d", role_id());
	}
	return 0;
}

int Mover::exit_move_scene(bool offline_exit) {
	if (validate_mover_scene()) {
		// 在场景才退出
		if (find_mover_in_move_scene(role_id()))
			move_disappear();
		move_scene_->exit_scene(this, offline_exit);
		move_scene_ = NULL;
	} else {
		MSG_USER_TRACE("scene error, role:%d", role_id());
	}
	return 0;
}

int Mover::move_appear(void) {
	int ret = 0;
	if (validate_mover_scene()) {
		ret = move_scene_->appear(this);
	} else {
		ret = ERROR_SCENE_NULL;
		MSG_USER_TRACE("mover appear error, scene:%d is NULL, mover id:%ld  index:%d", move_scene_id(), role_id(), mover_detail().block_index);
	}

	return ret;
}

int Mover::move_disappear(void) {
	int ret = 0;
	if (validate_mover_scene()) {
		ret = move_scene_->disappear(this);
	} else {
		ret = ERROR_SCENE_NULL;
		MSG_USER_TRACE("mover disappear error, scene:%d is NULL, mover id:%ld  index:%d", move_scene_id(), role_id(), mover_detail().block_index);
	}

	return ret;
}

bool Mover::is_adjacent_coord(uint16_t &x, uint16_t &y) {
	if (std::abs(grid_coord().x - pixel_x_to_grid(x)) > 10 || std::abs(grid_coord().y - pixel_y_to_grid(y)) > 10) {
		MSG_USER("mover_id:%ld adjacent error %d %d => %d %d", role_id(), grid_coord().x, grid_coord().y, pixel_x_to_grid(x), pixel_y_to_grid(y));
		return false;
	}
	return true;
}

Mover *Mover::find_mover_in_move_scene(role_id_t role_id) {
	if (validate_mover_scene()) {
		return move_scene()->find_mover_with_layer(mover_detail().layer_id, role_id);
	} else {
		//MSG_USER_TRACE("role:%ld", role_id);
	}
	return 0;
}

Fighter *Mover::find_fighter_in_battle_scene(role_id_t role_id) {
	if (validate_battle_scene()) {
		Fighter *fighter = battle_scene()->find_mover_with_role_id(role_id);
		if (fighter) return fighter;
	} else {
		//MSG_USER_TRACE("role:%ld", role_id);
	}
	return 0;
}

void Mover::hook_scene_pass(int scene_id){
	return;
}

int Mover::set_addition(Appear_Addition &addition){
	MSG_81000002 msg;
	bool exist = false;
	for (Appear_Prop_Vec::iterator it = addition_prop_.begin(); it != addition_prop_.end(); ++it) {
		if (it->type == addition.type) {
			it->int_value1 = addition.int_value1;
			it->str = addition.str;
			it->int64_vec = addition.int64_vec;
			it->array = addition.array;
			exist = true;
			break;
		}
	}
	if (!exist) {
		addition_prop_.push_back(addition);
	}

	if (move_scene()) {
		msg.role_id = role_id();
		msg.add_info_vec = addition_prop_;
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		this->scene_broad_around(buf, SCENE_MOVE);
	}
	this->appear_buff().set_ext_flag(true);
	return 0;
}

int Mover::get_addition(Appear_Addition &addition){
	for (Appear_Prop_Vec::iterator it = addition_prop_.begin(); it != addition_prop_.end(); ++it) {
		if (it->type == addition.type) {
			addition = (*it);
			return 0;
		}
	}
	return -1;
}

int Mover::del_addition(int addition_type){
	bool is_find = false;
	for (Appear_Prop_Vec::iterator it = addition_prop_.begin(); it != addition_prop_.end(); ++it) {
		if (it->type == addition_type) {
			addition_prop_.erase(it);
			is_find = true;
			break;
		}
	}
	if(is_find){
		MSG_81000002 msg;
		msg.role_id = role_id();
		msg.add_info_vec = addition_prop_;
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		this->scene_broad_around(buf, SCENE_MOVE);
		this->appear_buff().set_ext_flag(true);
	}
	return 0;
}

bool Mover::has_addition(int addition_type){
	for (Appear_Prop_Vec::iterator it = addition_prop_.begin(); it != addition_prop_.end(); ++it) {
		if (it->type == addition_type){
			return true;
		}
	}
	return false;
}

bool Mover::is_player(void) {
	return (0 != this->player_self() || 0 != this->player_monster_self());
}

bool Mover::is_hero(void) {
	return 0 != this->hero_self();
}

bool Mover::is_normal_monster(void) {
	return (mover_detail_->mover_type >=MOVER_T_MONSTER &&
			mover_detail_->mover_type < MOVER_T_MONSTER_END &&
			mover_detail_->mover_type != MOVER_T_MONSTER_BOSS &&
			mover_detail_->mover_type != MOVER_T_MONSTER_ELITE_BOSS);
}

bool Mover::is_boss(void) {
	return (mover_detail_->mover_type == MOVER_T_MONSTER_BOSS ||
			mover_detail_->mover_type == MOVER_T_MONSTER_ELITE_BOSS);
}
