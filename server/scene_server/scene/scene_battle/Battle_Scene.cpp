/*
 * Battle_Scene.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: ti
 */

#include "Battle_Scene.h"
#include "mover/Mover_Struct.h"
#include "fighter/Fighter.h"
#include "Global_Scene_Manager.h"
#include "NPC/NPC.h"
#include "Move_Scene.h"
#include "fighter/Fighter_Struct.h"
#include "Err_Code.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "NPC/NPC_Manager.h"
#include "Scene_Public.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Pool_Manager.h"
#include "module/team/Scene_Team_Manager.h"
#include "Msg_Fight_Struct.h"
#include "Config_Cache_Fighter.h"
#include "team/Scene_Teamer.h"
#include "Config_Cache_Scene.h"

const static int TWENTY_MINUTES = 60 * 20;

void Hurt_Total::reset(void) {
	last_time 	= Time_Value::zero;
	times 		= 0;
	total_hurt 	= 0.0;
}

Battle_Scene::Battle_Scene() {
	reset();
}

Battle_Scene::~Battle_Scene() {
	reset();
}

void Battle_Scene::reset() {
	last_tick_ = Time_Value::gettimeofday();
	player_on_battle_.clear();
	scene_id_ = 0;
	signal_exit_check = false;
	battle_id_ = 0;
	conflict_time_ = 0;
	check_times_ = 0;
	battle_status_ = STATUS_NULL;
	plot_btl_status_ = STATUS_NULL;
	start_time_ = Time_Value::zero;
	exit_time_ = Time_Value::zero;
	plot_pause_time_ = Time_Value::zero;
	statistic_shot.reset();
	hurt_total_map.clear();
	over_step_ = OVER_BEGIN;
	pause_type_  = PAUSE_TYPE_PLOT_ONE;

	mover_map_.clear();

	monster_point_ = 0;
	monster_group_ = 0;
	is_kill_boss_ = false;
	kill_monster_.clear();
	kill_player_.clear();
	balance_player_.clear();

	battle_type_ = BATTLE_TYPE_PVP;
	monster_battle_info_.reset();
	win_pos_ = 0;
	check_battle_over_ = false;
	over_type_ = OVER_TYPE_NORMAL;
	end_battle_after_plot_ = false;
	map_id = 0;
	pos_debug_map_.clear();
	move_scene_ = NULL;
	ai_foreshow_change_ = false;
	ai_foreshow_map_.clear();
	battle_give_up_map_.clear();

	other_info_.reset();
}

int Battle_Scene::init_scene_normal(long_id_t battle_id, Move_Scene* move_scene) {
	if (battle_id == 0) {
		MSG_TRACE_ABORT("error battle_id:%d", battle_id);
	}
	battle_id_ = battle_id;
	move_scene_ = move_scene;
	battle_status_ = STATUS_READY;
	over_type_ = OVER_TYPE_NORMAL;
	start_time_ = Time_Value::gettimeofday();
	if(move_scene_){
		move_scene_->add_battle_scene_id(battle_id_);
	}
	return 0;
}

void Battle_Scene::release_scene_normal(void){
	MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
	battle_status_ = STATUS_RECOVER;
	empty_mover();
	if(move_scene_){
		move_scene_->remove_battle_scene_id(battle_id_);
	}
}

int Battle_Scene::check_fighters_at_start(const Time_Value& now) {

	// 检查是否超时只有单方进入战斗
	if (now-start_time_ > Time_Value(60)) {
		int pos = -1;
		bool force_to_over = true;
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second) continue;
			if (pos == it->first.pos) continue;
			if (-1 == pos) {
				pos = it->first.pos;
				continue;
			}
			force_to_over = false;
		}
		if (force_to_over) {
			MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
			battle_status_ = STATUS_OVER;
			over_step_ = OVER_BEGIN;
			win_pos_ = pos;
		}
	}

	switch(this->battle_type_) {
	case BATTLE_TYPE_HONOR_ARENA:
	case BATTLE_TYPE_TEAM_ARENA:
	case BATTLE_TYPE_PVP: {
		int pos = -1;
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second) continue;
			if (Mover_Type::MOVER_T_PLAYER != it->second->mover_detail().mover_type) continue;
			if (-1 == pos)  {
				pos = it->first.pos;
			} else {
				if (pos != it->first.pos)
					return 0;
			}
		}
		break;
	}
	case BATTLE_TYPE_DRAGON_VALE:
	case BATTLE_TYPE_ARENA:
	case BATTLE_TYPE_TEAM:
	case BATTLE_TYPE_PVE:
	case BATTLE_TYPE_KNIGHT_TRIALS:
	case BATTLE_TYPE_CHALLENGE :{
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second) continue;
			if (Mover_Type::MOVER_T_PLAYER != it->second->mover_detail().mover_type) continue;
			return 0;
		}
		break;
	}
	default: {
		int pos = -1;
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second) continue;
			if (-1 == pos)  {
				pos = it->first.pos;
			} else {
				if (pos != it->first.pos)
					return 0;
			}
		}
		break;
	}
	}
	return 1;
}

int Battle_Scene::tick(const Time_Value &now) {
	//	PERF_MON("battle_scene tick");
	if (player_on_battle_.size() > 0) put_player_to_battle(now);

	Mover_Map mover_map_copy;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (!(it->second->mover_detail_ptr())) {
			MSG_TRACE_ABORT("battle_id:%d map size:%d", battle_id_, mover_map_.size());
			return 0;
		}
		mover_map_copy.bind(it->first, it->second);
	}

	last_tick_ = now;

	switch (battle_status_) {
	case STATUS_READY: {
		handle_battle_start(now);
		break;
	}
	case STATUS_ONGOING: {
		handle_battle_going(now, mover_map_copy);
		break;
	}
	case STATUS_WILL_OVER: {
		bool is_over = true;
		for (Mover_Map::iterator it = mover_map_copy.begin(); it != mover_map_copy.end(); ++it) {
			if (0 == it->second || 0 == it->second->fighter()) continue;
			if (it->second->fighter()->dead_status()) continue;
			if (0 == it->second->fighter()->skill_step_used()) continue;
			it->second->fight_tick(now);
			is_over = false;
		}
		if (is_over) {
			MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
			battle_status_ = STATUS_OVER;
		}
		break;
	}
	case STATUS_PAUSEING:{
		if (PAUSE_TYPE_USE_GUID != pause_type_) {
			// 暂停时，非吟唱等待的技能可以继续完成
			for (Mover_Map::iterator it = mover_map_copy.begin(); it != mover_map_copy.end(); ++it) {
				if (0 == it->second) continue;
				if (it->second->fighter()->dead_status()) continue;
				if (0 == it->second->fighter()->skill_step_used()) continue;
				it->second->fight_tick(now);
			}
		}
		if(check_pause_over(now) == 0){
			if (end_battle_after_plot_) {
				plot_to_end_battle(0);
				break;
			}
			MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
			statistic_shot.reset();
			battle_status_ = STATUS_ONGOING;
		}
		break;
	}
	case STATUS_OVER:{
		if (check_over_step(now) == 0) {
			notify_gate_exit_battle();
			GLOBAL_SCENE_MANAGER->push_destroy_scene(this);
			return 0;
		}
		break;
	}
	default : {
		break;
	}
	}

	return 0;
}

void Battle_Scene::handle_battle_start(const Time_Value &now) {
	if (BATTLE_TYPE_PVE == battle_type_ && (now -start_time_) < Time_Value(5)) return;
	if (BATTLE_TYPE_PVE != battle_type_ && (now -start_time_) < Time_Value(3)) return;

	int fighter_ready = check_fighters_at_start(now);
	if (0 != fighter_ready) {
		return;
	}
	MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
	battle_status_ = STATUS_ONGOING;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		it->second->fighter()->set_battle_status(BATTLING);
		it->second->fighter()->send_fight_data_when_battle_start();
		// 战斗开始侦听
		this->listen_start_fight(it->second->fighter());
	}
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;

		ai_manager_.start_fight(it->second->fighter());
		if ( it->second->fighter()->use_skill() != 0) continue;

		int skill_id = it->second->fighter()->get_first_used_skill();
		if (0 != skill_id) {
			if (it->second->monster_self()) {
				it->second->monster_self()->reset_first_time_use_skill();
			}
			it->second->fighter()->use_skill(skill_id);
		}
	}

	if (BATTLE_TYPE_PVP == battle_type_ || BATTLE_TYPE_ARENA == battle_type_ ||
		BATTLE_TYPE_PVE == battle_type_ || BATTLE_TYPE_KNIGHT_TRIALS == battle_type_ ||
		BATTLE_TYPE_CHALLENGE == battle_type_ || BATTLE_TYPE_TEAM == battle_type_ ||
		BATTLE_TYPE_HONOR_ARENA == battle_type_) {

		MSG_50200112 msg;
		int max_time = CONFIG_CACHE_FIGHTER->fight_misc("fight_time_arena");
		if (BATTLE_TYPE_PVP == battle_type_ || BATTLE_TYPE_ARENA == battle_type_
			|| BATTLE_TYPE_CHALLENGE == battle_type_) {
			if (max_time > 0) {
				msg.msecs_left = max_time * 1000;
			}
		}else if(BATTLE_TYPE_HONOR_ARENA == battle_type_){
			msg.msecs_left = 60 * 1000;
		}else if (BATTLE_TYPE_PVE == battle_type_) {
			msg.msecs_left = TWENTY_MINUTES * 1000;
		}

		msg.give_up_time_left = CONFIG_CACHE_FIGHTER->fight_misc("fight_giveup_time") * 1000;

		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		this->broadcast_around(buf);
	}

	start_time_ = now;
}

void Battle_Scene::handle_battle_going(const Time_Value &now, Mover_Map& mover_map_copy) {

	if(check_battle_over_ == true)
		this->check_battle_over();
	if(battle_status_ != STATUS_ONGOING) return;

	ai_manager_.fight_tick(now);
	bool is_over = true;
	int secs = 0;
	switch(battle_type_){
		case BATTLE_TYPE_PVE:{
			secs = TWENTY_MINUTES;
			break;
		}
		case BATTLE_TYPE_HONOR_ARENA:{
			secs = 60;
			break;
		}
		default:{
			secs = CONFIG_CACHE_FIGHTER->fight_misc("fight_time_arena");
			break;
		}
	}
	bool time_out = (now - start_time_) > Time_Value(secs);

	bool is_pve_time_out =
			time_out && ( BATTLE_TYPE_PVE == battle_type_);
	bool is_other_time_out =
			time_out && (BATTLE_TYPE_PVP == battle_type_ || BATTLE_TYPE_ARENA == battle_type_ ||
					BATTLE_TYPE_HONOR_ARENA == battle_type_);

	for (Mover_Map::iterator it = mover_map_copy.begin(); it != mover_map_copy.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if ((is_pve_time_out || is_other_time_out) &&
				(0 == it->second->fighter()->skill_step_used())) continue;
		it->second->fight_tick(now);
		is_over = false;
	}

	// 回收死亡单位
	for (Mover_Map::iterator it = mover_map_copy.begin(); it != mover_map_copy.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		it->second->fighter()->delete_fighter();
	}

	// 强制结束
	if (is_pve_time_out || is_other_time_out) {
		int winer_pos = 1;
		if(battle_type_ == BATTLE_TYPE_HONOR_ARENA){// 荣誉竞技场1分钟限时，超时战力高的胜利
			winer_pos = get_player_high_force_pos();
		}
		force_battle_to_end(winer_pos);
		return;
	}

	if (Conflict_Without_Time == get_conflict_type()) conflict_time_ = 0;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		it->second->fighter()->calc_conflict_time(now, conflict_time_);
	}
	conflict_time_ = 0;

	if (check_and_force_to_end()) return;
	if (force_to_check_over(now)) return;

}

int Battle_Scene::get_conflict_type() {
	int ret_value = 0;
	switch(battle_type_) {
	case BATTLE_TYPE_PVE:
	case BATTLE_TYPE_PVP:
	case BATTLE_TYPE_HONOR_ARENA:
	case BATTLE_TYPE_ARENA:
	case BATTLE_TYPE_DRAGON_VALE:
	case BATTLE_TYPE_KNIGHT_TRIALS:
	case BATTLE_TYPE_CHALLENGE:
		ret_value =  Conflict_With_Time;
		break;
	default:
		ret_value = Conflict_Without_Time;
		break;
	}
	return ret_value;
}

void Battle_Scene::first_use_skill_trigger(int skill_id, Fighter& fighter) {
	fighter.set_first_used_skill(skill_id);

	if (BATTLE_TYPE_PVE != battle_type_) return;
	int role_num = 0;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (!it->second) return;
		if (Mover_Type::MOVER_T_PLAYER == it->second->mover_type()) {
			++ role_num;
		}
	}
	if (1 != role_num) return;
	start_time_ = Time_Value::zero;
}

//void Battle_Scene::be_skill_trigger(void) {
//	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
//		if (0 == it->second || 0 == it->second->fighter()) continue;
//		it->second->fighter()->be_skill_trriger_skill();
//	}
//}

int Battle_Scene::empty_mover(void) {
	std::vector<Mover *> mover_vec;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->second) {
			mover_vec.push_back(it->second);
		}
	}
	for (std::vector<Mover *>::iterator it = mover_vec.begin(); it != mover_vec.end(); ++it) {
		(*it)->exit_battle_scene(2);
		if((*it)->player_self()){
			(*it)->player_self()->notify_gate_exit_battle(1);
		}
	}
	mover_map_.clear();

	for (Role_Time_Map::iterator it = player_on_battle_.begin(); it != player_on_battle_.end(); ++it) {
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(it->first);
		if (0 == player) continue;
		player->exit_battle_scene(3);

		player->notify_gate_exit_battle(2);
	}
	player_on_battle_.clear();

	return 0;
}

int Battle_Scene::broadcast_around(Block_Buffer &buf) {
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->second) {
			it->second->send_to_client(buf);
		}
	}

	return 0;
}

int Battle_Scene::enter_scene(Mover *mover) {
	if (pos_debug_map_.bind(mover->battle_position(), mover->role_id())) {
		MSG_USER_TRACE("enter_scene:%d error pos :%d  %d, role:%ld  %ld", battle_id_, mover->battle_position().idx, mover->battle_position().pos, pos_debug_map_[mover->battle_position()], mover->role_id());
	}

	if(mover && mover->fighter()){
		ai_manager_.join(mover->fighter());
		if(battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM ||
				battle_type_ == BATTLE_TYPE_TEAM_ARENA || battle_type_ == BATTLE_TYPE_GANG_WAR_LEADER){
			if(this->move_scene_ && monster_battle_info_.record && mover->player_self()){
				monster_battle_info_.record->hook_fighter_enter(mover->fighter());
			}
		}
	}

	return 0;
}

int Battle_Scene::exit_scene(Mover *mover) {
	if (pos_debug_map_.unbind(mover->battle_position())) {
		MSG_USER_TRACE("exit_scene error pos :%d  %d, role:%ld  %ld", mover->battle_position().idx, mover->battle_position().pos, pos_debug_map_[mover->battle_position()], mover->role_id());
	}
	if(mover && mover->fighter()){
		ai_manager_.leave(mover->fighter());
		if(battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM ||
				battle_type_ == BATTLE_TYPE_TEAM_ARENA || battle_type_ == BATTLE_TYPE_GANG_WAR_LEADER){
			if(this->move_scene_ && monster_battle_info_.record && mover->player_self()){
				monster_battle_info_.record->hook_fighter_leave(mover->fighter());
			}
		}
	}


	return 0;
}

int Battle_Scene::appear(Mover *mover) {
	if (!mover || mover->role_id() == 0) {
		MSG_USER(" == 0, error");
		return -1;
	}
	const Mover_Detail &detail = mover->mover_detail();

	if (! validate_position(detail.battle_position.pos, detail.battle_position.idx)) {
		MSG_USER("invalid grid_x = %d, grid_y = %d", detail.grid_coord.x, detail.grid_coord.y);
		return -1;
	}

	Block_Buffer appear_me_buf, appear_other_buf(0, 4 * 1024);

	// 通知周围区块“我”出现
	bool is_player = detail.mover_type == MOVER_T_PLAYER ? true : false;
	// 自己出现消息发给自己
	if (is_player) {
		mover->battle_appear_info(appear_other_buf);
	}
	mover->battle_appear_info(appear_me_buf);

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->second) {
			if (appear_me_buf.readable_bytes() > 0 && it->second->player_self() &&
					it->second->player_self()->btl_scene_complate())
				it->second->send_to_client(appear_me_buf);	/// 通知其他人屏幕显示"我"
			if (is_player)
				it->second->battle_appear_info(appear_other_buf);
		}
	}

	// 通知"我的屏幕"显示周围区块Mover信息
	if (is_player && appear_other_buf.readable_bytes() > 0 && mover->player_self() &&
			mover->player_self()->btl_scene_complate())
		mover->send_to_client(appear_other_buf);

	if (mover_map_.bind(detail.battle_position, mover)) {
		MSG_TRACE_ABORT("battle scene bind error b_id:%d, role:%ld, %d %d exist role:%ld", battle_id_, mover->role_id(), detail.battle_position.idx, detail.battle_position.pos, mover_map_[detail.battle_position]->role_id());
	}

	return 0;
}

int Battle_Scene::disappear(Mover *mover) {
	if (!mover || mover->role_id() == 0) {
		MSG_USER("unbind error == 0, error");
		return -1;
	}
	const Mover_Detail &detail = mover->mover_detail();

	if (! validate_position(detail.battle_position.pos, detail.battle_position.idx)) {
		MSG_TRACE_ABORT("unbind error battle_position pos = %d, idx = %d", detail.battle_position.pos, detail.battle_position.idx);
		return -1;
	}

	bool is_player = detail.mover_type == MOVER_T_PLAYER ? true : false;

	Position_Key pos_key = detail.battle_position;
	if (mover_map_.exist(detail.battle_position)) {
		if (mover_map_[detail.battle_position]->role_id() != mover->role_id()) {
			for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
				MSG_USER("unbind error pos :%d  %d, role:%ld", it->first.idx, it->first.pos, it->second->role_id());
				if(it->second->role_id() == mover->role_id()){
					pos_key = it->first;
				}
			}
			MSG_TRACE_ABORT("b_id:%d, unbind error, mover id:%ld, role_id:%ld", battle_id_, mover_map_[detail.battle_position]->role_id(), mover->role_id());
		}
	}

	if (mover_map_.unbind(pos_key)) {
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			MSG_USER("unbind error pos :%d  %d, role:%ld", it->first.idx, it->first.pos, it->second->role_id());
		}
		MSG_USER_TRACE("b_id:%d, unbind error, id:%ld, %d  %d", battle_id_, mover->role_id(), detail.battle_position.idx, detail.battle_position.pos);
	}

	Block_Buffer disappear_me_buf, disappear_other_buf;
	mover->make_disappear_info(disappear_me_buf);
	disappear_other_buf.make_message(81000010);
	int old_wri_idx = disappear_other_buf.get_write_idx();
	disappear_other_buf.write_uint16(0);
	uint16_t disappear_amount = 0;

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->second) {
			it->second->send_to_client(disappear_me_buf);
			if (is_player) {
				disappear_amount++;
				it->second->make_disappear_info(disappear_other_buf);
			}
		}
	}

	if (is_player && disappear_amount) {
		int new_wri_idx = disappear_other_buf.get_write_idx();
		disappear_other_buf.set_write_idx(old_wri_idx);
		disappear_other_buf.write_uint16(disappear_amount);
		disappear_other_buf.set_write_idx(new_wri_idx);
		disappear_other_buf.finish_message();
		mover->send_to_client(disappear_other_buf);
	}

	return 0;
}

Mover *Battle_Scene::find_mover_with_position(const Position_Key &position) {
	Mover_Map::iterator find_it = mover_map_.find(position);
	if (find_it != mover_map_.end())
		return find_it->second;
	return 0;
}

Fighter *Battle_Scene::find_mover_with_role_id(role_id_t role_id) {
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->second) {
			if (it->second->role_id() == role_id) {
				return it->second->fighter();
			}
		}
	}
	return 0;
}

int Battle_Scene::client_scene_complete(Fighter &fighter) {

	// 更新人物在场景的状态
	MSG_81000026 state_msg;
	state_msg.role_id = fighter.role_id();
	state_msg.state = 1;
	Block_Buffer buf;
	MAKE_MSG_BUF(state_msg, buf);
	fighter.scene_broad_around(buf);


	if (STATUS_ONGOING == this->battle_status_ || STATUS_PAUSEING == this->battle_status_) {
		if (!fighter.dead_status()) {
			fighter.set_battle_status(BATTLING);
		}
		MSG_DEBUG("fighter login when battle on goning, role_id=%ld, b_id=%d",
				fighter.role_id(), battle_id_);
		Time_Value now = Time_Value::gettimeofday();

		if (BATTLE_TYPE_PVP == battle_type_ || BATTLE_TYPE_ARENA == battle_type_ ||
				BATTLE_TYPE_PVE == battle_type_ || BATTLE_TYPE_KNIGHT_TRIALS == battle_type_ ||
				BATTLE_TYPE_CHALLENGE == battle_type_ || BATTLE_TYPE_TEAM == battle_type_ ||
				BATTLE_TYPE_HONOR_ARENA == battle_type_) {

			Time_Value delta = now - start_time_;
			int msec = delta.sec() * 1000 + delta.usec() / 1000;

			for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
				if (0 == it->second || 0 == it->second->fighter()) continue;
				it->second->fighter()->send_fight_data_when_battle_start();
			}

			MSG_50200112 msg;
			if (BATTLE_TYPE_PVP == battle_type_ || BATTLE_TYPE_ARENA == battle_type_
					|| BATTLE_TYPE_HONOR_ARENA == battle_type_) {
				int time_val1 = CONFIG_CACHE_FIGHTER->fight_misc("fight_time_arena") * 1000;
				msg.msecs_left = time_val1 - msec;
			} else if (BATTLE_TYPE_PVE == battle_type_) {
				msg.msecs_left = TWENTY_MINUTES * 1000;
			}

			int time_val2 = CONFIG_CACHE_FIGHTER->fight_misc("fight_giveup_time") * 1000;
			msg.give_up_time_left =  (time_val2 - msec) < 0 ? 0 : (time_val2 - msec);

			OBJ_SEND_TO_CLIENT(msg, fighter);
		}
	}
	// 怪物ai预告信息
	send_ai_foreshow_to_player(&fighter);
	return 0;
}

int Battle_Scene::listen_die_process(const Attack_Param &info, Fighter &dead) {
	Fighter *attacker = find_mover_with_role_id(info.attacker);
	if (attacker) {
		if(dead.monster_self()){
			int npc_type_id = dead.npc_self()->npc_type_id();
			if(attacker->hero_self()){
				role_id_t master_role_id = attacker->hero_self()->master_role_id();
				kill_monster_[master_role_id][npc_type_id]++;
			}else if(attacker->monster_self()){
				role_id_t master_role_id = attacker->monster_self()->master_role_id();
				if(master_role_id != 0){
					kill_monster_[master_role_id][npc_type_id]++;
				}
			}else if(attacker->player_self()){
				kill_monster_[attacker->role_id()][npc_type_id]++;
			}

		}else if(dead.player_self()){
			if(attacker->hero_self()){
				role_id_t master_role_id = attacker->hero_self()->master_role_id();
				kill_player_[master_role_id].push_back(dead.role_id());
			}else if(attacker->monster_self()){
				role_id_t master_role_id = attacker->monster_self()->master_role_id();
				if(master_role_id != 0){
					kill_player_[master_role_id].push_back(dead.role_id());
				}
			}
			else if(attacker->player_self()){
				kill_player_[attacker->role_id()].push_back(dead.role_id());
			}

		}
	}
	if(dead.mover_type() == MOVER_T_MONSTER_BOSS){
		is_kill_boss_ = true;
	}
//	check_battle_over_ = true;
	return 0;
}

bool Battle_Scene::validate_position(uint16_t p, uint16_t idx) {
	if (p < 0 || p > 1 || idx < 1 || idx > 12) {
		MSG_USER_TRACE("invalid position p=%d, idx=%d\n", p, idx);
	}
	return true;
}

void Battle_Scene::find_fighter_enemy(Fighter &fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info & target_info, int amount) {
	if (0 >= amount) return;

	// 优先选出1.嘲讽， 锁定
	find_fighter_enemy_locked(fighter, fighter_vec, target_info, amount);

	// 优先规则，血量
	if (find_fighter_enemy_blood(fighter, fighter_vec, target_info, amount)) {
		if (fighter_vec.size() > 0) return;
	}

	// 优先规则，负面buff
	if (find_fighter_enemy_neg_buff(fighter, fighter_vec, target_info, amount)) {
		if (fighter_vec.size() > 0) return;
	}

	// 优先行
	if (find_fighter_enemy_row(fighter, fighter_vec, target_info, amount)) {
		if (fighter_vec.size() > 0) return;
	}

	// 优先列
	if (find_fighter_enemy_col(fighter, fighter_vec, target_info, amount)) {
		if (fighter_vec.size() > 0) return;
	}

	// 随机列
	if (find_fighter_enemy_random_col(fighter, fighter_vec, target_info, amount)) {
		if (fighter_vec.size() >0) return;
	}

	// 随机选择
	if (find_fighter_enemy_random(fighter, fighter_vec, target_info, amount)) {
		if (fighter_vec.size() > 0) return;
	}

	// 按规则选
	find_fighter_enemy_rule(fighter, fighter_vec, target_info, amount);

	return;
}

bool Battle_Scene::find_fighter_enemy_random(Fighter& fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info& target_info, int amount) {

	if ((int)fighter_vec.size() >= amount) return true;

	int self_pos = fighter.mover_detail().battle_position.pos;
	int col = fighter.mover_detail().battle_position.idx % 3;
	if (target_info.prior_type != PRIORITY_RANDOM) return false;
	Mover_Map temp_mover_map;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (BATTLING != it->second->fighter()->get_battle_status() ||
				it->second->fighter()->dead_status()) continue;

		if (TYPE_ENEMY == target_info.target_relation && it->first.pos == self_pos) continue;
		if ((TYPE_PARTNER == target_info.target_relation || TYPE_FRIEND == target_info.target_relation)
				&& it->first.pos != self_pos) continue;
		if (TYPE_PARTNER == target_info.target_relation) {
			int temp_col = it->first.idx % 3;
			if (temp_col != col) continue;
		}
		if (!judge_select_type(*it->second, target_info.target_type)) continue;

		temp_mover_map.bind(it->first, it->second);
	}

	uint size = temp_mover_map.size();
	uint left = size;
	for (Mover_Map::iterator it = temp_mover_map.begin(); it != temp_mover_map.end(); ++it) {

		Fighter_Vec::iterator exist_it = std::find(fighter_vec.begin(), fighter_vec.end(), it->second->fighter());
		if (exist_it != fighter_vec.end()) continue;

		if ((int)(left+fighter_vec.size()) <= amount) {	// select all left
			fighter_vec.push_back(it->second->fighter());
		} else {			//randmon
			int random_val = random() % size;
			if (random_val < amount) {
				fighter_vec.push_back(it->second->fighter());
			}
		}
		if ((int)fighter_vec.size() >= amount) return true;
		--left;
	}

	return true;
}

void Battle_Scene::find_fighter_enemy_rule(Fighter& fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info& target_info, int amount) {

	if ((int)fighter_vec.size() >= amount)
		return;

	int self_pos = fighter.mover_detail().battle_position.pos;
	int idx = fighter.mover_detail().battle_position.idx;
	while (idx > 3) { idx -= 3; }
	int tmp_idx = 0;
	Position_Key p_key;
	p_key.pos = (target_info.target_relation==1) ? (self_pos == 0 ? 1 : 0) : self_pos;

	for (int i=0; i<3; ++i) {
		p_key.idx = 0;
		tmp_idx = 0;
		for (int j=0; j<3; ++j) {
			if (0 != tmp_idx && tmp_idx != idx) {
				if ((idx + tmp_idx) == 3) p_key.idx = 3 + 3 * i;
				else if ((idx + tmp_idx) == 4) p_key.idx = 2 + 3 * i;
				else p_key.idx = 1 + 3 * i;
			} else {
				int rand_val = random() % 2;
				if (0 == p_key.idx) tmp_idx = idx;
				else if (1 == idx && 0 == rand_val) tmp_idx = 2;
				else if (1 == idx && 1 == rand_val) tmp_idx = 3;
				else if (2 == idx && 0 == rand_val) tmp_idx = 1;
				else if (2 == idx && 1 == rand_val) tmp_idx = 3;
				else if (3 == idx && 0 == rand_val) tmp_idx = 1;
				else tmp_idx = 2;
				p_key.idx = tmp_idx + 3 * i;
			}
			Mover_Map::iterator it = mover_map_.find(p_key);
			if (it == mover_map_.end()) continue;
			if (0 == it->second || 0 == it->second->fighter()) continue;
			if (it->second->fighter()->dead_status() ||
					BATTLING != it->second->fighter()->get_battle_status()) continue;

			Fighter_Vec::iterator exist_it =
					std::find(fighter_vec.begin(), fighter_vec.end(), it->second->fighter());
			if (exist_it != fighter_vec.end()) continue;

			int temp_col = it->first.idx;
			while (temp_col > 3) temp_col -= 3;
			if (TYPE_PARTNER == target_info.target_relation && temp_col != idx) continue;
			if (!judge_select_type(*it->second, target_info.target_type)) continue;

			fighter_vec.push_back(it->second->fighter());
			if ((int)fighter_vec.size() >= amount)
				return;
		}
	}
}

void Battle_Scene::find_fighter_enemy_locked(Fighter& fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info& target_info, int amount) {
	int self_pos = fighter.mover_detail().battle_position.pos;

	// 选择敌人，才可能出现敌人被锁定或者敌人使用嘲讽
	if (target_info.target_relation != Target_Rela_Type::TYPE_ENEMY) return;
	for (Mover_Map::iterator it = mover_map_.begin();
			it != mover_map_.end(); ++it) {
		if ((int)fighter_vec.size() >= amount) return;
		if (it->first.pos == self_pos) continue;
		if (!it->second || !it->second->fighter()) continue;
		if (BATTLING != it->second->fighter()->get_battle_status() ||
				it->second->fighter()->dead_status()) continue;

		if (it->second->fighter()->is_lock() || it->second->fighter()->is_taunt()) {
			fighter_vec.push_back(it->second->fighter());
		}
	}

	// 如果是行或者列选，那就随机选一个
	uint size = fighter_vec.size();
	if (size > 1 && (target_info.prior_line > 0 || target_info.prior_row > 0)) {
		uint rand = random() % size;
		uint i = 0;
		for (Fighter_Vec::iterator it = fighter_vec.begin(); it != fighter_vec.end(); ) {
			if (i == rand) {
				++it;
			} else {
				it = fighter_vec.erase(it);
			}
			++i;
		}
	}
}

int Battle_Scene::find_index_from_rule(int idx1, int idx2, int ref_col) {
	int row1 = idx1 / 3 + 1;
	int row2 = idx2 / 3 + 1;
	int col2 = idx2 % 3;
	if (row1 > row2) {
		return idx2;
	} else if (row1 < row2) {
		return idx1;
	} else if (col2 == ref_col) {
		return idx2;
	}
	return idx1;
}

bool Battle_Scene::find_fighter_enemy_blood(Fighter& fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info& target_info, int amount) {

	if ((int)fighter_vec.size() >= amount) return true;

	int self_pos = fighter.mover_detail().battle_position.pos;
	int col = fighter.mover_detail().battle_position.idx % 3;

	if (target_info.prior_type != PRIORITY_LEAST_BLOOD &&
		target_info.prior_type != PRIORITY_BLOOD_PERCENT) return false;

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;

		if (BATTLING != it->second->fighter()->get_battle_status() ||
				it->second->fighter()->dead_status()) continue;
		if (TYPE_ENEMY == target_info.target_relation &&
				self_pos == it->first.pos) continue;
		if ((TYPE_PARTNER == target_info.target_relation || TYPE_FRIEND == target_info.target_relation) &&
				self_pos != it->first.pos) continue;
		if (TYPE_PARTNER == target_info.target_relation) {
			int temp_col = it->first.idx % 3;
			if (temp_col != col) continue;
		}

		if (!judge_select_type(*it->second, target_info.target_type)) continue;

		Fighter_Vec::iterator exist_it = std::find(fighter_vec.begin(), fighter_vec.end(), it->second->fighter());
		if (exist_it != fighter_vec.end()) continue;

		if (static_cast<int>(fighter_vec.size()) < amount) {
			fighter_vec.push_back(it->second->fighter());
		}

		for (Fighter_Vec::iterator v_it = fighter_vec.begin(); v_it != fighter_vec.end(); ++v_it) {
			int idx1 = it->first.idx;
			int idx2 = (*v_it)->mover_detail().battle_position.idx;
			if (target_info.prior_type == PRIORITY_LEAST_BLOOD) {
				double blood1 = it->second->fighter()->fighter_detail().current_blood();
				double blood2 = (*v_it)->fighter_detail().current_blood();
				if (!(blood1 > blood2 || blood1 < blood2)) {
					if (idx1 == find_index_from_rule(idx1, idx2, col)) {
						(*v_it) = it->second->fighter();
						break;
					}
				} else if (blood1 < blood2) {
					(*v_it) = it->second->fighter();
					break;
				}
			}
			if (target_info.prior_type == PRIORITY_BLOOD_PERCENT) {
				double rate1 = it->second->fighter()->current_blood_rate();
				double rate2 = (*v_it)->current_blood_rate();
				if (!(rate1 > rate2 || rate1 < rate2)) {
					if (idx1 == find_index_from_rule(idx1, idx2, col)) {
						(*v_it) = it->second->fighter();
						break;
					}
				} else if (rate1 < rate2) {
					(*v_it) = it->second->fighter();
					break;
				}
			}
		}
	}
	return true;
}

bool Battle_Scene::find_fighter_enemy_neg_buff(Fighter& fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info& target_info, int amount) {

	if ((int)fighter_vec.size() >= amount) return true;

	int self_pos = fighter.mover_detail().battle_position.pos;

	if (PRIORITY_NGE_BUFF != target_info.prior_type) return false;

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (TYPE_ENEMY == target_info.target_relation && self_pos == it->first.pos) continue;
		if (TYPE_PARTNER == target_info.target_relation && self_pos != it->first.pos) continue;
		if (TYPE_FRIEND == target_info.target_relation && self_pos != it->first.pos) continue;

		if (BATTLING != it->second->fighter()->get_battle_status() ||
				it->second->fighter()->dead_status()) continue;

		if (!it->second->fighter()->has_status_evil()) continue;

		Fighter_Vec::iterator exist_it = std::find(fighter_vec.begin(), fighter_vec.end(), it->second->fighter());
		if (exist_it != fighter_vec.end()) continue;

		if (static_cast<int>(fighter_vec.size()) < amount) {
			fighter_vec.push_back(it->second->fighter());
		} else {
			uint size = fighter_vec.size();
			uint rand_val = random() % (size + 1);
			if (rand_val >= size) continue;
			fighter_vec[rand_val] = it->second->fighter();
		}
	}
	return true;
}

bool Battle_Scene::find_fighter_enemy_row(Fighter& fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info& target_info, int amount) {

	if ((int)fighter_vec.size() >= amount) return true;

	int self_pos = fighter.mover_detail().battle_position.pos;

	if (target_info.prior_row != PRIORITY_FORE_ROW && target_info.prior_row != PRIORITY_REAR_ROW )
		return false;
	if (TYPE_PARTNER == target_info.target_relation)
		return false;

	int row = 0;

	if (!fighter_vec.empty()) {
		row = fighter_vec[0]->mover_detail().battle_position.idx;
		row = (row-1) / 3 + 1;
	}
	if (0 == row){
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second || 0 == it->second->fighter()) continue;
			if (TYPE_ENEMY == target_info.target_relation && self_pos == it->first.pos) continue;
			if (TYPE_FRIEND == target_info.target_relation && self_pos != it->first.pos) continue;

			if (BATTLING != it->second->fighter()->get_battle_status() ||
					it->second->fighter()->dead_status()) continue;
			int temp_idx = (it->first.idx - 1) / 3 +1;

			if (row != 0 && target_info.prior_row == PRIORITY_FORE_ROW && temp_idx > row) continue;
			if (row != 0 && target_info.prior_row == PRIORITY_REAR_ROW && temp_idx < row) continue;
			row = temp_idx;
		}
	}
	if (0 == row) return false;

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (TYPE_ENEMY == target_info.target_relation && self_pos == it->first.pos) continue;
		if (TYPE_FRIEND == target_info.target_relation && self_pos != it->first.pos) continue;

		if (BATTLING != it->second->fighter()->get_battle_status() ||
				it->second->fighter()->dead_status()) continue;
		int p_idx = (it->first.idx - 1) / 3 + 1;

		if (p_idx != row) continue;
		if (std::find(fighter_vec.begin(), fighter_vec.end(), it->second->fighter()) !=
				fighter_vec.end()) continue;

		fighter_vec.push_back(it->second->fighter());
	}
	return true;
}

bool Battle_Scene::find_fighter_enemy_col(Fighter& fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info& target_info, int amount){

	if ((int)fighter_vec.size() >= amount) return true;

	int self_pos = fighter.mover_detail().battle_position.pos;

	if (target_info.prior_line != PRIORITY_UP_COL &&
		target_info.prior_line != PRIORITY_DOWN_COL &&
		target_info.prior_line != PRIORITY_SELF_COL) return false;

	if (TYPE_PARTNER == target_info.target_relation) return false;

	int col = 0;
	int self_col = fighter.mover_detail().battle_position.idx;
	while (self_col > 3) self_col -= 3;

	if (!fighter_vec.empty()) {
		col = fighter_vec[0]->mover_detail().battle_position.idx;
		while (col > 3) col -= 3;
	}
	if (0 == col) {
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second || 0 == it->second->fighter()) continue;
			if (TYPE_ENEMY == target_info.target_relation && self_pos == it->first.pos) continue;
			if (TYPE_FRIEND == target_info.target_relation && self_pos != it->first.pos) continue;
			if (BATTLING != it->second->fighter()->get_battle_status() ||
					it->second->fighter()->dead_status()) continue;

			int tmp_idx = it->first.idx;
			while(tmp_idx > 3) tmp_idx -= 3;

			if (tmp_idx == self_col && target_info.prior_line == PRIORITY_SELF_COL) {
				col = tmp_idx;
				break;
			}
			if (col != 0 && target_info.prior_line == PRIORITY_UP_COL && tmp_idx > col) continue;
			if (col != 0 && target_info.prior_line == PRIORITY_DOWN_COL && tmp_idx < col) continue;
			if (col != 0 && target_info.prior_line == PRIORITY_SELF_COL && tmp_idx > col) continue;

			col = tmp_idx;
		}
	}
	if (0 == col) return false;

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (TYPE_ENEMY == target_info.target_relation && self_pos == it->first.pos) continue;
		if (TYPE_FRIEND == target_info.target_relation && self_pos != it->first.pos) continue;
		if (BATTLING != it->second->fighter()->get_battle_status() || it->second->fighter()->dead_status()) continue;
		int p_col = it->first.idx;
		while(p_col > 3)  p_col -= 3;

		if (p_col != col) continue;
		if (std::find(fighter_vec.begin(), fighter_vec.end(), it->second->fighter()) !=
				fighter_vec.end()) continue;
		fighter_vec.push_back(it->second->fighter());
	}
	return true;
}

bool Battle_Scene::find_fighter_enemy_random_col(Fighter& fighter, Fighter_Vec &fighter_vec,
		const Target_Select_Info& target_info, int amount) {

	if ((int)fighter_vec.size() >= amount) return true;

	int self_pos = fighter.mover_detail().battle_position.pos;

	if (target_info.prior_line != PRIORITY_RANDOM_COL) return false;
	if (TYPE_PARTNER == target_info.target_relation) return false;
	int col = 0;

	if (fighter_vec.size() > 0) {
		Fighter* figther = fighter_vec[0];
		if (0 != figther) {
			col = fighter.mover_detail_ptr()->battle_position.idx;
			while (col > 3) col -= 3;
		}
	}
	if (col == 0) {
		Int_Vec col_vec;
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second || 0 == it->second->fighter()) continue;
			if (TYPE_ENEMY == target_info.target_relation &&
					self_pos == it->first.pos) continue;
			if (TYPE_FRIEND == target_info.target_relation &&
					self_pos != it->first.pos) continue;
			int temp_idx = it->first.idx;
			while (temp_idx > 3) temp_idx -= 3;
			if (std::find(col_vec.begin(), col_vec.end(), temp_idx) != col_vec.end()) continue;
			col_vec.push_back(temp_idx);
		}
		if (col_vec.empty()) {
			MSG_USER("cannot find target by random col");
			return false;
		}
		int rand = random() %  col_vec.size();
		col = col_vec[rand];
	}

	if (0 == col) return false;

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (TYPE_ENEMY == target_info.target_relation &&
				self_pos == it->first.pos) continue;
		if (TYPE_FRIEND == target_info.target_relation &&
				self_pos != it->first.pos) continue;
		if (std::find(fighter_vec.begin(), fighter_vec.end(), it->second->fighter()) !=
				fighter_vec.end()) continue;

		int temp_idx = it->first.idx;
		while (temp_idx > 3) temp_idx -= 3;
		if (temp_idx != col) continue;
		fighter_vec.push_back(it->second->fighter());
	}

	return true;
}

bool Battle_Scene::judge_select_type(Mover &mover, const Int_Set &type_set) {
	if (type_set.size() <= 0) return true;
	for (Int_Set::iterator it = type_set.begin(); it != type_set.end(); ++it) {
		if (mover.is_player() && (*it) == SELECT_TYPE_PLAYER) return true;
		if (mover.is_hero() && (*it) == SELECT_TYPE_HERO) return true;
		if (mover.is_normal_monster() && (*it) == SELECT_TYPE_MONSTER) return true;
		if (mover.is_boss() && (*it) == SELECT_TYPE_BOSS) return true;
	}
	return false;
}

// 检查与敌人的冲突情况
bool Battle_Scene::check_enemy_conflict(const Time_Value &now, Fighter &fighter,
		int &enemy_alive, Time_Value &wait_time, int &count, bool &all_dizz, bool &using_skill) {
#define F(S, E, I) (std::find(S, E, I) == E)

	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(fighter.current_skill_id());
	if (0 == cfg) {
		MSG_DEBUG("cannot find skill config, skill_id=%d", fighter.current_skill_id());
		return true;
	}
	enemy_alive = 0;
	count = 0;
	all_dizz = true;
	using_skill = false;
	wait_time = Time_Value::zero;
	bool use_spec = fighter.will_use_spec_skill(now);
	int pos = fighter.mover_detail().battle_position.pos;
	Role_Vec vec = fighter.get_simulate_targets();
	Time_Value tmp_time = Time_Value::zero;
	if (F(vec.begin(), vec.end(), fighter.role_id()))
		vec.push_back(fighter.role_id());

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (pos == it->first.pos) continue;
		if (it->second->fighter()->dead_status()) continue;
		if (BATTLING != it->second->fighter()->get_battle_status()) continue;

		++enemy_alive;

		if (it->second->fighter()->is_dizz()) continue;

		all_dizz = false;

		if (it->second->fighter()->is_using_spec_skill()) return true;

		// 敌方正在释放技能, 检查是否有忽略敌方释放技能情况
		if (0 != it->second->fighter()->skill_step_used()) {
			using_skill = true;

			if (use_spec) return true;

			const Skill_Config* it_cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(
					it->second->fighter()->current_skill_id());
			if (0 == it_cfg) continue;
			if (1 == cfg->ignore_enemy_conflict || 1 == it_cfg->ignore_enemy_conflict) {
				// 检查是否在攻击对象是否冲突
				Role_Vec targets;
				it->second->fighter()->get_current_step_targets(targets);
				if (F(targets.begin(), targets.end(), it->second->role_id()))
					targets.push_back(it->second->role_id());

				for (Role_Vec::iterator role_it = vec.begin(); role_it != vec.end(); ++role_it) {
					if (!F(targets.begin(), targets.end(), *role_it))
						return true;
				}
				continue;
			}

			return true;

		// 敌人不在释放技能, 不在吟唱中
		} else {
			if (it->second->fighter()->is_singing(now)) {
				tmp_time = it->second->fighter()->get_conflic_time(now);
				++count;
				if (tmp_time > wait_time) {
					wait_time = tmp_time;
				}
			} else {
//				all_wait = false;
			}
		}
	}

	return false;
}

// 检查与同盟的冲突情况
bool Battle_Scene::check_allies_conflict(const Time_Value &now, Fighter &fighter,
		role_id_t& longest, Time_Value &wait_time, int &count, bool &all_wait, bool &using_skill) {

	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(fighter.current_skill_id());
	if (0 == cfg) {
		MSG_DEBUG("cannot find skill config, skill_id=%d", fighter.current_skill_id());
		return true;
	}
	bool use_spec = fighter.will_use_spec_skill(now);
	int pos = fighter.mover_detail().battle_position.pos;
	Time_Value tmp_time = Time_Value::zero;
	longest = fighter.role_id();
	wait_time = fighter.get_conflic_time(now);
	count = 0;
	all_wait = true;
	using_skill = false;

	// 将要翻放浮空技，检查英雄是否正在释放技能
	if (fighter.mover_detail().is_player() && Skill_Type::TYPE_FLOAT_SKILL == cfg->skill_type) {
		Fighter* partner = SCENE_PUBLIC->find_battle_hero(fighter.role_id());
		if (0 != partner && BATTLING == partner->get_battle_status() &&
				!partner->dead_status() && 0 != partner->skill_step_used() ) return true;
	}

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (pos != it->first.pos) continue;
		if (it->second->role_id() == fighter.role_id()) continue;
		if (it->second->fighter()->dead_status()) continue;
		if (BATTLING != it->second->fighter()->get_battle_status()) continue;
		if (it->second->fighter()->is_dizz()) continue;

		if (it->second->fighter()->is_using_spec_skill()) return true;

		// 检查英雄对应的主角是否正在释放浮空技
		if (fighter.mover_detail().is_hero() &&
				fighter.hero_self()->hero_base_detail().master_role_id == it->second->role_id()) {
			if (it->second->fighter()->is_using_floating_skill()) return true;
		}

		int step = it->second->fighter()->skill_step_used();

		if (0 != step && use_spec) return true;
		// 已方正在释放非绝对冲突技能，检查是否目标冲突
		if (0 != step) {
			all_wait = false;
			using_skill = true;
			if (it->second->fighter()->check_target_conflict(cfg->target_conflict,
					fighter.get_simulate_targets()))
				return true;
		} else {
			if (it->second->fighter()->is_singing(now)) {
				tmp_time = it->second->fighter()->get_conflic_time(now);
				++count;
				if (tmp_time > wait_time) {
					longest = it->second->role_id();
					wait_time = tmp_time;
				}
			} else {
				all_wait = false;
			}
		}

	}
	return false;
}

int Battle_Scene::check_skill_conflict(const Time_Value &now, Fighter &fighter) {

	int conflict_type = get_conflict_type();
	int max_shot_allies = 0, max_shot_enemy = 0;
	if (Conflict_Without_Time == conflict_type) {
		for (Mover_Map::const_iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second || 0 == it->second->fighter()) continue;
			if (it->second->fighter()->dead_status()) continue;
			if (it->first.pos == fighter.mover_detail().battle_position.pos) {
				++max_shot_allies;
			} else {
				++max_shot_enemy;
			}
		}
		max_shot_allies = (max_shot_allies == 1) ? 2 : max_shot_allies;
		max_shot_enemy = (max_shot_enemy == 1) ? 2 : max_shot_enemy;
	} else {
		max_shot_allies = max_shot_enemy = 6;
	}

//	const static int enemy_shot = 6;
	const static int cfl_time = 100;
	if (fighter.current_skill_id() == 0) {
		conflict_time_ = cfl_time;
		return cfl_time;
	}

	bool allies_shot_limit = ((Conflict_Without_Time == conflict_type) &&
			statistic_shot.val_1 == fighter.mover_detail().battle_position.pos &&
			statistic_shot.val_2 >= max_shot_allies);

	bool force_to_use_our_side = ((Conflict_Without_Time == conflict_type) &&
			statistic_shot.val_1 != fighter.mover_detail().battle_position.pos &&
			statistic_shot.val_2 >= max_shot_enemy);

	int enemy_alive = 0;
	Time_Value enemy_wait_time = Time_Value::zero;
	int enemy_wait_count = 0;
	bool all_enemy_dizz = true;
	bool enemy_using_skill = false;
	if (check_enemy_conflict(now, fighter, enemy_alive, enemy_wait_time, enemy_wait_count,
			all_enemy_dizz, enemy_using_skill)) {
		conflict_time_ = cfl_time;
		return cfl_time;
	}

	if (all_enemy_dizz) {
		force_to_use_our_side = true;
	}

	if (!force_to_use_our_side && allies_shot_limit) {
		if (!(enemy_alive == 1 && enemy_wait_count == 0)) {
			conflict_time_ = cfl_time;
			return cfl_time;
		}
	}

	role_id_t allies_waiter = 0;
	Time_Value allies_wait_time = Time_Value::zero;
	int allies_wait_count = 0;
	bool all_allies_wait = true;
	bool allies_using_skill = false;
	if (check_allies_conflict(now, fighter, allies_waiter, allies_wait_time, allies_wait_count,
			all_allies_wait, allies_using_skill)) {
		conflict_time_ = cfl_time;
		return cfl_time;
	}

	role_id_t id = fighter.role_id();
	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(fighter.current_skill_id());
	if(0 == cfg) {
		conflict_time_ = cfl_time;
		return cfl_time;
	}
	if (1 == cfg->ignore_enemy_conflict) {
		return 0;
	}


	if (force_to_use_our_side && id != allies_waiter) {
		return cfl_time;
	} else if (!force_to_use_our_side) {
		if (allies_using_skill && allies_waiter == id) {
			return 0;
		}
		if (enemy_wait_time > allies_wait_time) {
			conflict_time_ = cfl_time;
			return cfl_time;
		}
		if (enemy_wait_time <= allies_wait_time && allies_waiter != id) {
			return cfl_time;
		}
	}

	return 0;
}

void Battle_Scene::set_statistic_shot(int pos) {
	if (pos == statistic_shot.val_1) {
		++statistic_shot.val_2;
	} else {
		statistic_shot.val_1 = pos;
		statistic_shot.val_2 = 1;
	}
}

void Battle_Scene::total_hurt_statistic(int pos, double value, const Time_Value &now) {
	Hurt_Total_Map::iterator find_it = hurt_total_map.find(pos);
	if (find_it == hurt_total_map.end()) {
		Hurt_Total hurt_total;
		hurt_total_map[pos] = hurt_total;
	}
	if ((hurt_total_map[pos].last_time + Time_Value(3, 0)) < now) {
		hurt_total_map[pos].last_time = now;
		hurt_total_map[pos].times = 1;
		hurt_total_map[pos].total_hurt = value;
	} else {
		hurt_total_map[pos].last_time = now;
		++hurt_total_map[pos].times;
		hurt_total_map[pos].total_hurt += value;
	}

	MSG_81000065 msg;
	msg.pos = pos;
	msg.times = hurt_total_map[pos].times;
	msg.hurt_ttl = hurt_total_map[pos].total_hurt;
	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	this->broadcast_around(buf);
}

void Battle_Scene::fix_partner_wait_time(const Time_Value & add_time, const Time_Value & now, Fighter &fighter) {
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (!it->second) continue;
		if (it->first.pos != fighter.mover_detail().battle_position.pos) continue;
		it->second->fighter()->fix_partner_wait_time(add_time, now);
	}
}

bool Battle_Scene::check_battle_over_before_appear(void) {
	if (battle_status_ == STATUS_READY) return false;
	if (battle_status_ == STATUS_PAUSEING) return false;

	if (battle_status_ == STATUS_WILL_OVER) return true;
	if (battle_status_ == STATUS_OVER) return true;
	if (battle_status_ == STATUS_RECOVER) return true;

	bool pos_a_lose = true, pos_b_lose = true;
	for (Mover_Map::const_iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second->fighter()->fighter_detail_ptr()) {
			MSG_TRACE_ABORT("fighter detail null, mover size:%d", mover_map_.size());
			continue;
		}
		if (! it->second->fighter()->dead_status()) {
			if (it->second->mover_detail().battle_position.pos == 0) {
				pos_a_lose = false;
			} else {
				pos_b_lose = false;
			}
		}
	}
	return (pos_a_lose || pos_b_lose);
}

bool Battle_Scene::check_all_enemy_dead(Fighter &fighter) const {
	for (Mover_Map::const_iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (!it->second) continue;
		if (it->first.pos == fighter.mover_detail().battle_position.pos) continue;

		if (!it->second->fighter()->dead_status())
			return false;
	}
	return true;
}

bool Battle_Scene::check_all_enemy_dead_and_cast(Fighter &fighter)const {
	for (Mover_Map::const_iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (!it->second) continue;
		if (it->first.pos == fighter.mover_detail().battle_position.pos) continue;

		if (!it->second->fighter()->dead_and_cast())
			return false;
	}
	return true;
}

int Battle_Scene::check_battle_over(void) {

	check_battle_over_ = false;

	if (battle_status_ != STATUS_ONGOING) {
		MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
		return 0;
	}

	bool pos_a_die = true, pos_b_die = true;
	int move_scene_id = 0;

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->second) {
			if (! it->second->fighter()->dead_status()) {
				if (it->second->mover_detail().battle_position.pos == 0) {
					pos_a_die = false;
					move_scene_id = it->second->move_scene_id();
				}

				if (it->second->mover_detail().battle_position.pos == 1) {
					pos_b_die = false;
				}
			}
		}
	}

	MSG_DEBUG("status:%d b_id:%ld, over_step:%d  a:%d  b:%d move:%d", battle_status_, battle_id_, over_step_, pos_a_die, pos_b_die, mover_map_.size());
	if (false == pos_a_die && false == pos_b_die) {
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			MSG_DEBUG("role_id=%ld, status=%d", it->second->role_id(),
					it->second->fighter()->get_battle_status());
		}
	}

	if (pos_a_die || pos_b_die) {
		//pve have wave
		if((battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM)
				&& pos_b_die && monster_battle_info_.cur_wave < monster_battle_info_.wave_amount){
			monster_battle_info_.cur_wave += 1;
			MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
			this->battle_status_ = STATUS_PAUSEING;
			this->pause_type_ = PAUSE_TYPE_NEXT_WAVE_APPEAR;
			this->exit_time_ = Time_Value::gettimeofday() + Time_Value(1);
			check_all_monsters_exit();
			return 0;
		}

		MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
		battle_status_ = STATUS_OVER;

		over_step_ = OVER_BEGIN;
		exit_time_ = Time_Value::gettimeofday() + Time_Value(0);

		// pve end
		if(battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM){
			// player win
			for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
				if (it->second && it->second->player_self()){
					monster_battle_info_.player_vec.push_back(it->second->role_id());
				}
			}
			if(pos_b_die){
				win_pos_ = 0;
				monster_battle_info_.result = 1;
			}else{
				win_pos_ = 1;
				monster_battle_info_.result = 0;
			}
		}else{
			win_pos_ = pos_a_die ? 1:0;
		}
	} else {

	}

	return 0;
}

int Battle_Scene::check_over_step(const Time_Value& now){
	switch(over_step_){
		case OVER_BEGIN:{
			for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
				if (0 == it->second || 0 == it->second->fighter()) continue;
				it->second->fighter()->handle_check_over_start();
				if (it->second->fighter()->dead_status()) continue;
				it->second->fighter()->return_morale_when_blt_over();
			}
			if(now > exit_time_){
				MSG_DEBUG("battle balance: id %ld winpos %d, type %d", battle_id_, win_pos_, battle_type_);
				over_step_ = OVER_BALANCE;
				exit_time_ = now + Time_Value(1, 200000);
				Block_Buffer buf;
				MSG_81000052 msg;
				msg.win = win_pos_;
				msg.over_type = over_type_;
				if((battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM)
					&& win_pos_ == 0){
					msg.plot_id = monster_battle_info_.end_plot_id;//plot_id
				}else{
					msg.plot_id = "";
				}
				if (battle_type_ == BATTLE_TYPE_CHALLENGE) {
					for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
						if (0 == it->second || !it->second->is_player()) continue;
						Uid_Name id_name;
						id_name.uid = it->second->role_id();
						id_name.name = it->second->role_name();
						msg.btl_unit_name_vec.push_back(id_name);
					}
				}
				MAKE_MSG_BUF(msg, buf);
				broadcast_around(buf);
				//monster point
				if((battle_type_ == BATTLE_TYPE_PVE  || battle_type_ == BATTLE_TYPE_TEAM)
						&& monster_battle_info_.result == 1){
					if (this->move_scene_ && monster_battle_info_.record && monster_battle_info_.record->scene) {
						monster_battle_info_.record->scene->add_new_monster_point(monster_point_, is_kill_boss_,
								monster_battle_info_.coord);
					}
				}
				// 杀怪通知
				this->notify_logic_kill_data();
			}
			break;
		}
		case OVER_BALANCE:{
			if(now > exit_time_){
				std::vector<Mover*> win_vec;
				std::vector<Mover*> lose_vec;
				Move_Scene* move_scene = NULL;
				bool is_dragon_vale_scene_appear = false;
				// monster piont
				MSG_DEBUG("battle exit monster point: id %ld ", battle_id_);
				exit_monster_point();
				std::vector<Mover*> tmp_mover_vec;
				for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
					if (it->second) {
						tmp_mover_vec.push_back(it->second);
						if(it->second->player_self()){
							balance_player_.push_back(it->second->role_id());
							//龙谷掠夺战斗特殊处理
							if(!is_dragon_vale_scene_appear) {
								move_scene = it->second->move_scene();
								if(move_scene && move_scene->scene_type() == Dragon_Vale_Type) {
									is_dragon_vale_scene_appear = true;
								}
							}
						}
						if(this->win_pos_ == it->second->fighter()->battle_position().pos){
							win_vec.push_back(it->second);
						}else{
							lose_vec.push_back(it->second);
						}
					}
				}
				// 公会战：统计死亡次数
				if(move_scene){
					std::vector<int> point_monsters;
					point_monsters.clear();
					if(battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM){
						get_monsters_from_point(point_monsters);
					}
					move_scene->hook_battle_finish(battle_type_, win_pos_, win_vec, lose_vec, point_monsters);
				}
				int count = tmp_mover_vec.size();
				for(int i=0; i<count; ++i){
					//pve战斗失败返回出生点
					if((battle_type_ == BATTLE_TYPE_PVE  || battle_type_ == BATTLE_TYPE_TEAM)
							&& monster_battle_info_.result == 0 && monster_battle_info_.record &&
							tmp_mover_vec[i]->player_self() &&
							get_scene_type(tmp_mover_vec[i]->move_scene()->scene_id()) != War_Scene_Type){
						tmp_mover_vec[i]->player_self()->back_to_revive_point();
					}
					tmp_mover_vec[i]->exit_battle_scene(1);
				}
				if(battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM){
					if(monster_battle_info_.end_plot_id != "" && this->win_pos_ == 0){
						MSG_DEBUG("battle plot: id %ld type %d", battle_id_, battle_type_);
						int plot_time = monster_battle_info_.end_plot_time;
						if(plot_time != 0){
							over_step_ = OVER_PLOT_MULT;
							exit_time_ = Time_Value::gettimeofday() + Time_Value(plot_time/1000, plot_time%1000*1000);
						}else{
							over_step_ = OVER_PLOT_ONE;
							exit_time_ = Time_Value::gettimeofday() + Time_Value(180);
						}
						//
						int b_count = balance_player_.size();
						for(int i=0; i<b_count; ++i){
							Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(balance_player_[i]);
							if(player){
								player->play_plot(monster_battle_info_.end_plot_id, 0,false);
							}
						}
					}else{
						MSG_DEBUG("battle drop: id %ld ", battle_id_);
						over_step_ = OVER_DROP;
						exit_time_ = Time_Value::gettimeofday() + Time_Value(1);
					}
				}else{
					return 0;
				}
			}
			break;
		}
		case OVER_PLOT_ONE:{
			bool in_plot = false;
			int b_count = balance_player_.size();
			for(int i=0; i<b_count; ++i){
				Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(balance_player_[i]);
				if(player){
					if(player->is_in_plot()){
						in_plot = true;
						break;
					}
				}
			}
			if( in_plot == false){
				over_step_ = OVER_DROP;
//				// monster piont
				MSG_DEBUG("battle plot one over: id %ld", battle_id_);
//				exit_monster_point();
				exit_time_ = Time_Value::gettimeofday() + Time_Value(0);
			}else if(now > exit_time_){
				over_step_ = OVER_DROP;
				MSG_DEBUG("battle plot one timeout: id %ld", battle_id_);
				exit_time_ = Time_Value::gettimeofday() + Time_Value(0);
			}
			break;
		}
		case OVER_PLOT_MULT:{
			if (now > exit_time_) {
				//end plot status
				int b_count = balance_player_.size();
				for(int i=0; i<b_count; ++i){
					Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(balance_player_[i]);
					if(player){
						player->end_plot(monster_battle_info_.end_plot_id);
					}
				}
				over_step_ = OVER_DROP;
//				// monster piont
//				MSG_DEBUG("battle:%ld exit monster point", battle_id_);
//				exit_monster_point();
				exit_time_ = Time_Value::gettimeofday() + Time_Value(0);
			}
			break;
		}
		case OVER_DROP:{
			if(now > exit_time_){
				over_step_ = OVER_D_BALANCE;
				exit_time_ = Time_Value::gettimeofday() + Time_Value(0);
				MSG_DEBUG("battle d_balance: id %ld", battle_id_);
				//d balance
				//通关
				if(battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM){
					if(this->win_pos_ == 0){
						int b_count = balance_player_.size();
						for(int i=0; i<b_count; ++i){
							Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(balance_player_[i]);
							if(player && player->move_scene()){
								player->move_scene()->scene_pass_to_logic();
								break;
							}
						}
					}
				}
			}
			break;
		}
		case OVER_D_BALANCE:{
			if (now > exit_time_) {
				return 0;
			}
			break;
		}
		default:{
			break;
		}
	}
	return -1;
}

int Battle_Scene::check_pause_over(const Time_Value& now){
	switch(pause_type_){
		case PAUSE_TYPE_PLOT_ONE:{
			bool is_in_plot = false;
			Mover_Map::iterator it;
			for(it = mover_map_.begin(); it != mover_map_.end(); ++it){
				if(it->second && it->second->player_self()){
					if(it->second->player_self()->is_in_plot()){
						is_in_plot = true;
						break;
					}
				}
			}
			if(is_in_plot == false){
				return 0;
			}else if(now > exit_time_){
				return 0;
			}
			break;
		}
		case PAUSE_TYPE_PLOT_MULT:{
			if(now > exit_time_){
				return 0;
			}
			break;
		}
		case PAUSE_TYPE_TALK:{
			if(now > exit_time_){
				notify_resume_battle();
				return 0;
			}
			break;
		}
		case PAUSE_TYPE_USE_GUID: {
			Time_Value detal = now - plot_pause_time_;
			if (detal >= Time_Value(180, 0)) {
				plot_pause_time_ = Time_Value::zero;
				if (STATUS_ONGOING == this->plot_btl_status_) {
					for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it)  {
						if (0 == it->second || 0 == it->second->fighter()) continue;
						it->second->fighter()->fix_skill_step_time(detal);
					}
				}
				MSG_DEBUG("status:%d b_id:%ld, over_step:%d, btl_status:%d", battle_status_, battle_id_, over_step_, this->plot_btl_status_);
				this->battle_status_ = this->plot_btl_status_;
			}
			break;
		}
		case PAUSE_TYPE_NEXT_WAVE_APPEAR:{
			if(now > exit_time_){
				this->pause_type_ = PAUSE_TYPE_NEXT_WAVE_FIGHT;
				this->exit_time_ = Time_Value::gettimeofday() + Time_Value(1);
				//TO CLIENT NEXT WAVE
				Block_Buffer buf;
				MSG_81000051 msg;
				msg.wave_cur = this->monster_battle_info_.cur_wave;
				msg.wave_max = this->monster_battle_info_.wave_amount;
				buf.make_message(msg.msg_id);
				msg.serialize(buf);
				buf.finish_message();
				broadcast_around(buf);
				refresh_monsters(monster_battle_info_.cur_wave);
				return -1;
			}
			break;
		}
		case PAUSE_TYPE_NEXT_WAVE_FIGHT:{
			if(now > exit_time_){

				return 0;
			}
			break;
		}
		default:{
			break;
		}
	}
	return -1;
}

void Battle_Scene::init_battle_info(Monster_Battle_Info& info, Fighter* player){
	battle_type_ = BATTLE_TYPE_PVE;
	if (player->player_self()) {
		const Scene_Config * scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player->move_scene_id());
		if (scene_config && Scene_Config::Dungeon_Config_Detail::CAN_TEAM == scene_config->dungeon.can_team) {
			battle_type_ = BATTLE_TYPE_TEAM;
		}
	}
	monster_battle_info_ = info;
	if(info.record){
		this->monster_point_ = info.record->config.id;
	}
	this->monster_group_ = info.group_id;
	monster_battle_info_.cur_wave = 1;
	refresh_monsters(monster_battle_info_.cur_wave);

	Position_Key player_pos(0, Position_Key::LINE_TWO_FRONT);	// 主角位置

	if (0 != player->team_position().idx) {
		player_pos.idx = player->team_position().idx;
	}

	//根据英雄位置作调整
	init_battle_hero_info(player, player_pos);

	if(player->set_battle_position(player_pos.pos, player_pos.idx, this) == 0){
		player->enter_battle_scene(this);

		// 拉队员进入
		if (player->player_self()) {
			player->player_self()->enter_battle_scene_team_member(this);
		}
	}
}

void Battle_Scene::init_battle_hero_info(Fighter* player, Position_Key &player_pos_key){
	if (!player || !player->is_player()) {
		return;
	}
	Hero *hero = SCENE_PUBLIC->find_fight_hero(player->role_id());
	if (hero) {
		Position_Key hero_pos_key(player_pos_key.pos, hero->hero_const_base_detail().hero_formation);

		player->player_self()->player_pos_key_fit_hero(player_pos_key, hero_pos_key);

		hero->set_pos_key(hero_pos_key.pos, hero_pos_key.idx);
	}
}

void Battle_Scene::refresh_monsters(int wave){
	if(wave > monster_battle_info_.wave_amount)
	{
		return;
	}
	int count = monster_battle_info_.wave_vec[wave-1].monster_vec.size();
	Monster* monster;
	for(int i=0; i<count; ++i)
	{
		int monster_type_id = monster_battle_info_.wave_vec[wave-1].monster_vec[i];
		NPC_Addition_Info add_info;
		add_info.birth_coord.x = 1;
		add_info.birth_coord.y = monster_battle_info_.wave_vec[wave-1].pos_vec[i];
		if(monster_battle_info_.monster_chest_info.is_monster_chest(monster_type_id)){// 如果宝箱怪，替换名称和模型id
			add_info.name = monster_battle_info_.monster_chest_info.monster_name;
			add_info.jid = monster_battle_info_.monster_chest_info.monster_model_id;
		}
		monster = NPC_MANAGER->create_battle_monster(monster_type_id, monster_battle_info_.record, add_info, this);
		if(wave != 1 && monster != NULL){
			this->listen_start_fight(monster->fighter());
			monster->set_battle_status(BATTLING);
		}
	}
}

void Battle_Scene::exit_monster_point(void){
	if(monster_battle_info_.result == 1){
		//clear monster piont
		if (this->move_scene_ && monster_battle_info_.record) {
			monster_battle_info_.record->clear_point(this->battle_id_);
			if(move_scene_){
				move_scene_->hook_monster_point_clear(monster_battle_info_.record->config.id);
			}
		}
		//drops
		if(this->move_scene_){
			// 有收益玩家
			std::vector<role_id_t> player_vec;
			int count = monster_battle_info_.player_vec.size();
			for(int i=0; i<count; ++i){
				role_id_t role_id = monster_battle_info_.player_vec[i];
				Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
				if(player && player->move_scene()){
					if(player->move_scene()->scene_type() == Team_Fb_Scene_Type
							|| player->move_scene()->scene_type() == HERO_DREAM_Scene_Type
							|| player->move_scene()->scene_type() == TEAM_DEFEND_Scene_Type){
						if(player->is_use_profile()){
							player_vec.push_back(role_id);
						}
					}else{
						if(player->is_no_profit() == false){
							player_vec.push_back(role_id);
						}
					}
				}
			}

			std::vector<int> monster_vec;
			count = monster_battle_info_.wave_vec.size();
			for(int i=0; i<count; ++i){
				int m_count = monster_battle_info_.wave_vec[i].monster_vec.size();
				for(int index=0; index<m_count; ++index){
					int type_id = monster_battle_info_.wave_vec[i].monster_vec[index];
					monster_vec.push_back(type_id);
				}
			}

			if(player_vec.empty() == false){
				NPC_MANAGER->drop_by_monster_point(monster_battle_info_.coord, this->move_scene_,
						monster_battle_info_.record, monster_vec, player_vec, monster_battle_info_.monster_chest_info);
			}
		}
	}else{
		if(this->move_scene_ && monster_battle_info_.record){
			monster_battle_info_.record->reset_point(this->battle_id_);
		}

	}
}

void Battle_Scene::notify_logic_kill_data(void){
	// 组队副本杀怪统计
	int team_scene_kill_num = 0;
	if(battle_type_ == BATTLE_TYPE_TEAM){
		int count = this->monster_battle_info_.wave_vec.size();
		for(int i = 0; i < count; ++i){
			int num = this->monster_battle_info_.wave_vec[i].monster_vec.size();
			team_scene_kill_num += num;
		}
	}

	MSG_20200200 msg;
	for(Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if(it->second){
			Scene_Player* player = it->second->player_self();
			if(!player){
				continue;
			}
			msg.reset();
			msg.battle_type  = battle_type_;
			if(battle_type_ == BATTLE_TYPE_ARENA){
				msg.battle_result = (win_pos_ == 0) ? 1:0;
			} else if (battle_type_ == BATTLE_TYPE_TEAM_ARENA) {
				int score_gap = 0;
				Other_Info ot = this->get_other_info();
				if (ot.type == OT_TEAM_ARENA) {
					if (win_pos_ == 0) {
						score_gap = ot.int_v1 - ot.int_v2;
					} else {
						score_gap = ot.int_v2 - ot.int_v1;
					}
				}
				msg.monster_group = score_gap;
				msg.battle_result = (win_pos_ == player->cur_battle_position().pos) ? 1 : 0;
				Map_Team *team = SCENE_TEAM_MANAGER->get_team(player->get_team_id());
				if (team) {
					if (Map_Team::TEAM_SPORTS_NORMAL != team->sports_status()) {
						team->sports_status() = Map_Team::TEAM_SPORTS_NORMAL;
						if (msg.battle_result) {
							team->fail_times() = 0;
						} else {
							++team->fail_times();
						}
					}
					msg.monster_point = team->sports_battle_level();
				}
			} else if(battle_type_ == BATTLE_TYPE_PVE || battle_type_ == BATTLE_TYPE_TEAM){
				if(win_pos_ == 0){
					msg.monster_point = monster_point_;
					msg.monster_group = monster_group_;
					if(player->move_scene()){
						Int_Int_Map scene_reward_map;
						player->move_scene()->hook_award_player(player, scene_reward_map, team_scene_kill_num);
					}
					msg.battle_result = 1;
				}
			} else {
				msg.battle_result = (win_pos_ == player->cur_battle_position().pos) ? 1 : 0;
			}
			msg.scene_id = player->move_scene_id();
			// MSG_DEBUG("player finish battle role_id:%ld",player->role_id());
			for (Role_Vec::iterator r_it = kill_player_[player->role_id()].begin();
					r_it != kill_player_[player->role_id()].end(); ++r_it) {
				msg.kill_player.push_back(*r_it);
			}
			for (Int_Int_Map::iterator r_it = kill_monster_[player->role_id()].begin();
					r_it != kill_monster_[player->role_id()].end(); ++r_it) {
				Int_Int monster;
				monster.val_1 = r_it->first;
				monster.val_2 = r_it->second;
				msg.monster.push_back(monster);
			}

			if (msg.battle_result == 1) {
				player->call_ann_battle_in_bonfire_scene(msg.kill_player, msg.scene_id);
			}

			Block_Buffer block_buf;
			block_buf.make_message(msg.msg_id);
			msg.serialize(block_buf);
			block_buf.finish_message();
			player->send_to_logic(block_buf);
		}
	}
}

void Battle_Scene::notify_gate_exit_battle(void){
	int b_count = balance_player_.size();
	for(int i=0; i<b_count; ++i){
		Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(balance_player_[i]);
		if(player){
			player->notify_gate_exit_battle(5);
		}
	}
	balance_player_.clear();
}

int Battle_Scene::ai_select_skill(Fighter* fighter){
	return ai_manager_.ai_select_skill(fighter);
}

void Battle_Scene::listen_use_skill(Fighter* fighter, int skill_id){
	ai_manager_.use_skill(fighter, skill_id);
}

void Battle_Scene::listen_break_use_skill(Fighter* fighter, int skill_id){
	ai_manager_.break_use_skill(fighter, skill_id);;
}

void Battle_Scene::listen_fight_round(Fighter* fighter){
	ai_manager_.fight_round(fighter);
}

void Battle_Scene::listen_die(Fighter* fighter){
	ai_manager_.die(fighter);
}

void Battle_Scene::listen_start_fight(Fighter* fighter){
	ai_manager_.start_fight(fighter);
}

void Battle_Scene::listen_play_plot(std::string plot_id){
	ai_manager_.play_plot(plot_id);
}

void Battle_Scene::listen_play_talk(int talk_id){
	ai_manager_.play_talk(talk_id);
}

void Battle_Scene::listen_skill_sing_end(Fighter* fighter, int skill_id){
	//清除ai预告
	if(!fighter){
		return;
	}
	AI_Foreshow_Map::iterator it = ai_foreshow_map_.find(fighter->role_id());
	if(it == ai_foreshow_map_.end()){
		return;
	}
	bool change = false;
	AI_Foreshow_Info_Vec::iterator s_it;
	for(s_it = it->second.begin(); s_it != it->second.end(); ++s_it){
		if(s_it->skill_id == skill_id){
			it->second.erase(s_it);
			change = true;
			break;
		}
	}
	if(change){
		send_ai_foreshow_to_all();
	}
}

int Battle_Scene::handle_plot_client_request(int type) {
	switch(type) {
	case PLOT_PAUSE: {
		if (this->battle_status_ != STATUS_ONGOING &&
				this->battle_status_ != STATUS_READY) return 1;
		if (plot_pause_time_ != Time_Value::zero) break;
		plot_pause_time_ = Time_Value::gettimeofday();
		this->plot_btl_status_ = this->battle_status_;
		MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
		this->battle_status_ = STATUS_PAUSEING;
		this->pause_type_ = PAUSE_TYPE_USE_GUID;
		break;
	}
	case PLOT_CONTINUE: {
		if (this->battle_status_ != STATUS_PAUSEING) return 1;
		if (plot_pause_time_ == Time_Value::zero) break;
		Time_Value detal = Time_Value::gettimeofday() - plot_pause_time_;
		plot_pause_time_ = Time_Value::zero;
		if (this->plot_btl_status_ == STATUS_ONGOING) {
			for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
				if (0 == it->second || 0 == it->second->fighter()) continue;
				it->second->fighter()->fix_skill_step_time(detal);
			}
		}
		MSG_DEBUG("status:%d b_id:%ld, over_step:%d, btl_status:%d", battle_status_, battle_id_, over_step_, this->plot_btl_status_);
		this->battle_status_ = this->plot_btl_status_;
		break;
	}
	default:
		break;
	}
	return 0;
}

void Battle_Scene::fight_prop_change_hook(Fighter* fighter, int prop, double value){
	if(prop == PT_BLOOD_CURRENT){
		ai_manager_.hp_change(fighter);
	}else if(prop == PT_MORALE_CURRENT){
		ai_manager_.mp_change(fighter);
	}
}

void Battle_Scene::hook_status_overlay(Fighter* fighter, const int status_id, const int num){
	ai_manager_.status_overlay(fighter, status_id, num);
}

const Battle_Scene::Mover_Map& Battle_Scene::mover_map() {
	return this->mover_map_;
}

bool Battle_Scene::battle_plot_is_over(){
	bool is_over = true;
	Mover_Map::iterator it;
	for(it = mover_map_.begin(); it != mover_map_.end(); ++it){
		if(it->second && it->second->player_self()){
			if(it->second->player_self()->is_in_plot()){
				is_over = false;
				break;
			}
		}
	}
	return is_over;
}

int Battle_Scene::play_battle_plot(std::string plot_id, int plot_time){
	if(battle_status_ != STATUS_ONGOING){
		return -1;
	}
	if(plot_time == 0){
		pause_type_ = PAUSE_TYPE_PLOT_ONE;
		// 剧情最大时间
		exit_time_= Time_Value::gettimeofday() +Time_Value(60);
	}else{
		pause_type_ = PAUSE_TYPE_PLOT_MULT;
		exit_time_ = Time_Value::gettimeofday() +
				Time_Value(plot_time/1000, plot_time%1000*1000);
	}
	MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
	battle_status_ = STATUS_PAUSEING;
	Mover_Map::iterator it;
	for(it = mover_map_.begin(); it != mover_map_.end(); ++it){
		if(it->second && it->second->player_self()){
			it->second->player_self()->play_plot(plot_id, 0, true);
		}
	}
	listen_play_plot(plot_id);
	return 0;
}

int Battle_Scene::play_battle_talk(role_id_t role_id, int talk_id, int talk_time){
	if(battle_status_ != STATUS_ONGOING){
		return -1;
	}
	//pause_type_ = PAUSE_TYPE_TALK;
	//exit_time_ = Time_Value::gettimeofday() +
	//			Time_Value(talk_time/1000, talk_time%1000*1000);
	//battle_status_ = STATUS_PAUSEING;
//
//	{
//		Block_Buffer buf;
//		MSG_50200111 msg;
//		msg.type = 1;
//		buf.make_message(msg.msg_id);
//		msg.serialize(buf);
//		buf.finish_message();
//		broadcast_around(buf);
//	}

	{
		Block_Buffer buf;
		MSG_81000205 msg;
		msg.role_id = role_id;
		msg.talk_id = talk_id;
		msg.talk_time = talk_time;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();
		broadcast_around(buf);
	}
	listen_play_talk(talk_id);
	return 0;
}

int Battle_Scene::ai_kill_all_monsters(int pos){
	Mover_Map::iterator it;
	for(it = mover_map_.begin(); it != mover_map_.end(); ++it){

		if (0 == it->second || 0 == it->second->monster_self()) continue;
		if (it->second->fighter()->dead_status()) continue;
		if (it->first.pos != pos) continue;

		it->second->fighter()->handle_ai_kill_fighter();
	}
	return 0;
}

void Battle_Scene::notify_end_plot(void){
	Mover_Map::iterator it;
	for(it = mover_map_.begin(); it != mover_map_.end(); ++it){
		if(it->second && it->second->player_self()){
			if(it->second->player_self()->is_in_plot()){
				it->second->player_self()->notify_end_plot();
			}
		}
	}
}

void Battle_Scene::set_check_over_state(Fighter& fighter) {
	if (!signal_exit_check) return;
	signal_exit_check = false;
	if (check_all_enemy_dead_and_cast(fighter)) {
		check_battle_over_ = true;
		MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
	}
}

bool Battle_Scene::check_dead_fighter_locked(const role_id_t chk_id, const role_id_t atk_id, int pos) {
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (it->second->role_id() == chk_id) {
			if (0 != it->second->fighter()->skill_step_used())
				return true;
		}

		if (it->first.pos == pos) continue;
		if (it->second->fighter()->dead_status()) continue;

		if (it->second->fighter()->check_locked_dead_target(chk_id, atk_id))
			return true;
	}

	signal_exit_check = true;

	return false;
}

void Battle_Scene::notify_resume_battle(void){
	Block_Buffer buf;
	MSG_50200111 msg;
	msg.type = 0;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	broadcast_around(buf);
}

bool Battle_Scene::monster_point_is_valid(void){
	bool is_has_player = false;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->second) {
			if(it->second->player_self()){
				is_has_player = true;
				break;
			}
		}
	}
	if(is_has_player == false){
		return false;
	}
	return true;
}

void Battle_Scene::get_monsters_from_point(std::vector<int>& monsters){
	int count = monster_battle_info_.wave_vec.size();
	for(int i=0; i<count; ++i){
		int m_count = monster_battle_info_.wave_vec[i].monster_vec.size();
		for(int index=0; index<m_count; ++index){
			int type_id = monster_battle_info_.wave_vec[i].monster_vec[index];
			monsters.push_back(type_id);
		}
	}
}

int Battle_Scene::get_cur_monster_wave(void){
	return this->monster_battle_info_.cur_wave;
}

int Battle_Scene::get_max_monster_wave(void){
	return this->monster_battle_info_.wave_amount;
}

void Battle_Scene::set_map_id(uint32_t scene_map_id) {
	this->map_id = scene_map_id;
}

uint32_t Battle_Scene::get_map_id() {
	return this->map_id;
}

void Battle_Scene::add_player_on_battle(role_id_t id) {
	player_on_battle_.insert(std::make_pair(id, Time_Value::gettimeofday()));
}

void Battle_Scene::del_player_on_battle(role_id_t id) {
	Role_Time_Map::iterator find_it = player_on_battle_.find(id);
	if (find_it == player_on_battle_.end()) return;
	player_on_battle_.erase(find_it);
}

void Battle_Scene::put_player_to_battle(const Time_Value& now) {
	for (Role_Time_Map::iterator it = player_on_battle_.begin(); it != player_on_battle_.end(); ++it) {
		if (BATTLE_TYPE_PVE == battle_type_ && (now - it->second < Time_Value(3))) continue;
		if (BATTLE_TYPE_PVE != battle_type_ && (now - it->second < Time_Value(5))) continue;

		Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(it->first);
		if (0 == player) {
			this->del_player_on_battle(it->first);
			return;
		}
		player->battle_scene_completed(false);
		return;
	}
}

bool Battle_Scene::check_and_force_to_end() {
	if (check_battle_over_) return true;
	if (mover_map_.empty()) check_battle_over_ = true;
	int pos = -1;
	for(Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (it->second->fighter()->dead_and_skill_end()) continue;
		it->second->fighter()->check_dead_and_broadcast(*it->second->fighter(), 0);

		if (-1 == pos) pos = it->first.pos;
		if (pos != it->first.pos) return false;
		if (0 != it->second->fighter()->skill_step_used()) return false;
	}
	check_battle_over_ = true;
	MSG_DEBUG("check and dead broadcast, b_id=%d", battle_id_);
	return true;
}

bool Battle_Scene::force_to_check_over(const Time_Value &now) {

	if (check_battle_over_) return true;
	int sec = now.sec() - start_time_.sec();
	int over_sec = 180 * (check_times_ + 1);
	if (sec < over_sec) return false;
	check_times_ += 1;
	check_battle_over_ = true;
	MSG_DEBUG("force to check over, b_id=%d", battle_id_);
	return true;
}

void Battle_Scene::force_battle_to_end(int winner, Over_Type type) {
	MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
	battle_status_ = STATUS_WILL_OVER;
	over_type_ = type;
	over_step_ = OVER_BEGIN;
	win_pos_ = winner;
}

void Battle_Scene::plot_to_end_battle(int winner) {
	MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
	battle_status_ = STATUS_WILL_OVER;
	over_step_ = OVER_BEGIN;
	win_pos_ = winner;
	exit_time_ = Time_Value::gettimeofday() + Time_Value(1);
	this->monster_battle_info_.result = 1;


	Attack_Param param;
	std::vector<Mover*> die_vec_;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it ->second || 0 == it->second->fighter()) continue;
		if (it->second->player_self()) {
			param.attacker = it->second->role_id();
			this->monster_battle_info_.player_vec.push_back(it->second->role_id());
		}
		if (winner == it->first.pos) continue;
		if (it->second->fighter()->dead_status()) continue;


//		it->second->fighter()->fight_basic_prop_modify(PT_BLOOD_CURRENT, AT_BASIC, O_SET, 0);
		Prop_Setter prop(PT_BLOOD_CURRENT, AT_BASIC, O_SET, 0, 0, 0);
		Prop_Setter_Vec vec;
		vec.push_back(prop);

		MSG_81000060 c_msg;
		c_msg.attack_id = it->second->role_id();
		it->second->fighter()->fight_modify_fight_prop(vec, c_msg, true);
		this->listen_die_process(param, *it->second->fighter());

		Block_Buffer buf;
		MSG_81000045 msg;
		msg.role_id = it->second->role_id();
		msg.forced_dead = 1;
		MAKE_MSG_BUF(msg, buf);
		broadcast_around(buf);

		if (0 == it->second->monster_self()) continue;
		die_vec_.push_back(it->second);
	}

	for (std::vector<Mover*>::iterator it = die_vec_.begin(); it != die_vec_.end(); ++it) {
		(*it)->fighter()->exit_battle_scene(8);
	}
}

void Battle_Scene::clear_enemy_simulate_target(const Fighter &fighter) {
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->first.pos == fighter.mover_detail().battle_position.pos) continue;
		it->second->fighter()->clear_simulate_target();
	}
}

void Battle_Scene::force_to_recycle_battle_scene() {
	GLOBAL_SCENE_MANAGER->push_destroy_scene(this);
}

void Battle_Scene::force_finish_battle(void){
	MSG_DEBUG("status:%d b_id:%ld, over_step:%d", battle_status_, battle_id_, over_step_);
	if(battle_status_ == STATUS_RECOVER){
		return;
	}
	battle_status_ = STATUS_OVER;

	MSG_81000055 msg;
	msg.type = 1;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	this->broadcast_around(buf);

	std::vector<Mover *> mover_vec;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (it->second) {
			mover_vec.push_back(it->second);
		}
	}
	for (std::vector<Mover *>::iterator it = mover_vec.begin(); it != mover_vec.end(); ++it) {
		(*it)->exit_battle_scene(4);
		if((*it)->player_self()){
			(*it)->player_self()->notify_gate_exit_battle(3);
			(*it)->player_self()->clear_all_fighter_status();
		}
	}
	mover_map_.clear();

	for (Role_Time_Map::iterator it = player_on_battle_.begin(); it != player_on_battle_.end(); ++it) {
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(it->first);
		if (0 == player) continue;
		player->exit_battle_scene(5);

		player->notify_gate_exit_battle(4);
	}
	player_on_battle_.clear();
	monster_battle_info_.reset();
	GLOBAL_SCENE_MANAGER->push_destroy_scene(this);
}

Battle_Scene::Debug_Mover_Map &Battle_Scene::pos_debug_map(void) {
	return pos_debug_map_;
}

NPC_Record* Battle_Scene::npc_record(void){
	return this->monster_battle_info_.record;
}


void Battle_Scene::send_ai_foreshow_to_player(Fighter* fighter){
	if(!fighter){
		return;
	}
	if(ai_foreshow_map_.empty()){
		return;
	}
	MSG_81000056 msg;
	for(AI_Foreshow_Map::iterator it = ai_foreshow_map_.begin();
			it != ai_foreshow_map_.end(); ++it){
		std::vector<AI_Foreshow_Info>::iterator s_it;
		for(s_it = it->second.begin(); s_it != it->second.end(); ++s_it){
			msg.ai_foreshow_vec.push_back(*s_it);
		}
	}
	OBJ_SEND_TO_CLIENT(msg, (*fighter));
	return;
}

void Battle_Scene::send_ai_foreshow_to_all(void){
	Block_Buffer buf;
	MSG_81000056 msg;
	for(AI_Foreshow_Map::iterator it = ai_foreshow_map_.begin();
			it != ai_foreshow_map_.end(); ++it){
		std::vector<AI_Foreshow_Info>::iterator s_it;
		for(s_it = it->second.begin(); s_it != it->second.end(); ++s_it){
			msg.ai_foreshow_vec.push_back(*s_it);
		}
	}
	MAKE_MSG_BUF(msg, buf);
	broadcast_around(buf);
	return;
}

void Battle_Scene::battle_scene_debug(void) {
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		MSG_DEBUG("id:%ld, hp:%f battle_status:%d, battle_position:%d", it->second->role_id(), it->second->fighter()->blood_current(), it->second->fighter()->battle_status(), it->second->mover_detail().battle_position.pos);
	}
}

void Battle_Scene::set_world_boss_blood(const int blood){
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if(it->second->fighter() && it->second->is_boss()){
			it->second->fighter()->set_blood_outer_of_battle(0, blood);
			break;
		}
	}
}

Fighter* Battle_Scene::get_boss_fighter(void){
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if(it->second->fighter() && it->second->is_boss()){
			return it->second->fighter();
		}
	}
	return NULL;
}

Move_Scene* Battle_Scene::move_scene(void){
	return move_scene_;
}

const Monster_Battle_Info &Battle_Scene::monster_battle_info(void) const {
	return monster_battle_info_;
}

void Battle_Scene::add_ai_foreshow_info(const role_id_t monster_role_id, AI_Foreshow_Info_Vec infos){
	bool change = false;

	if(infos.empty()){// 空
		AI_Foreshow_Map::iterator it = ai_foreshow_map_.find(monster_role_id);
		if(it != ai_foreshow_map_.end()){
			ai_foreshow_map_.erase(it);
			change = true;
		}
	}else{
		ai_foreshow_map_[monster_role_id] = infos;
		change = true;
	}
	if(change){
		send_ai_foreshow_to_all();
	}
}

void Battle_Scene::check_all_monsters_exit(void) {
	Mover_Map mover_map_copy_;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		mover_map_copy_.bind(it->first, it->second);
	}
	for (Mover_Map::iterator it = mover_map_copy_.begin(); it != mover_map_copy_.end(); ++it) {
		// 怪物永远在1方
		if (it->first.pos != 1) continue;
		if (!it->second->monster_self()) continue;
		LOG_TRACE_ABORT("monster_id=%ld, b_id=%d, cur_blood=%f, state=%d", it->second->role_id(),
				battle_id_, it->second->fighter()->blood_current(), it->second->fighter()->get_battle_status());
		it->second->fighter()->broadcast_die_info();
		it->second->fighter()->exit_battle_scene(10);
	}
}

int Battle_Scene::get_player_high_force_pos(void){
	int high_pos = 0;
	int high_force = 0;

	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if(it->second->player_self()){
			if(it->second->player_self()->fighter_detail().ttl_force > high_force){
				high_force = it->second->player_self()->fighter_detail().ttl_force;
				high_pos = it->second->battle_position().pos;
			}
		}
	}
	return high_pos;
}

int Battle_Scene::handle_multi_role_battle_give_up(role_id_t role_id, int side, int give_up_type) {

//	int secs = (Time_Value::gettimeofday() - start_time_).sec();
//	if (secs < 60) return ERROR_INNER_SERVER;
	// 发起认输战斗类型
	if (give_up_type == Battle_Give_Up::TYPE_SPONSOR) {
		Battle_Give_Up_Map::iterator find_it = battle_give_up_map_.find(side);
		if (battle_give_up_map_.end() != find_it) {
			Time_Value elapse_time = Time_Value::gettimeofday() - find_it->second.give_up_time;
			if (elapse_time.sec() < 15) return ERROR_GIVE_UP_TWICE;
		}
		Battle_Give_Up battle_give_up;
		battle_give_up.give_up_time = Time_Value::gettimeofday();
		battle_give_up.side = side;
		battle_give_up.sponsor = role_id;
		for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
			if (0 == it->second || 0 == it->second->fighter()) continue;
			if ( it->second->fighter()->dead_status()) continue;
			if (0 == it->second->player_monster_self()) continue;
			battle_give_up.give_up_map_[it->second->role_id()] = Battle_Give_Up::TYPE_GIVE_UP;
		}
//		battle_give_up.give_up_map_[role_id] = Battle_Give_Up::TYPE_GIVE_UP;
		battle_give_up_map_[side] = battle_give_up;
		sync_give_up_data_to_client(side);

//		check_battle_over_when_player_give_up(side);
		return 0;
	}

	Battle_Give_Up_Map::iterator find_it = battle_give_up_map_.find(side);
	if (battle_give_up_map_.end() == find_it) return ERROR_INNER_SERVER;

	const Time_Value &now = Time_Value::gettimeofday();
	int secs = (now - find_it->second.give_up_time).sec();
	if (secs > 20) return ERROR_INNER_SERVER;

	find_it->second.give_up_map_[role_id] = give_up_type;
	sync_give_up_data_to_client(side);

	check_battle_over_when_player_give_up(side);
	return 0;
}

void Battle_Scene::check_battle_over_when_player_give_up(int side) {

	// 检查并处理是否退出
	Battle_Give_Up_Map::iterator find_it = battle_give_up_map_.find(side);
	if (battle_give_up_map_.end() == find_it) return;
	bool is_give_up = true;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (side != it->first.pos) continue;
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (it->second->fighter()->dead_status()) continue;
		if (0 == it->second->player_self()) continue;

		Role_Int_Map::iterator role_int_find_it = find_it->second.give_up_map_.find(it->second->role_id());
		if (find_it->second.give_up_map_.end() == role_int_find_it) {
			is_give_up = false;
			break;
		}
		if (role_int_find_it->second == Battle_Give_Up::TYPE_NOT_GIVE_UP) {
			is_give_up = false;
			break;
		}
	}

	if (!is_give_up) return;

	// 处理认输
	int win_pos = side == 0 ? 1 : 0;
	MSG_50200118 msg;
	msg.result = 0;
	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	broadcast_around(buf);
	force_battle_to_end(win_pos, Battle_Scene::OVER_TYPE_GIVE_UP);

	Prop_Setter_Vec setter_vec;
	setter_vec.push_back(Prop_Setter(Property_Type::PT_BLOOD_CURRENT,
			Addition_Type::AT_BASIC, Operate_Type::O_SET, 0, 0, 0));
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (side != it->first.pos) continue;
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (it->second->fighter()->dead_status()) continue;
		if (0 == it->second->player_self()) continue;
		if (it->second->player_self()->is_system_hosting()) continue;

		it->second->fighter()->modify_props_normal(setter_vec, true, 500);
	}
}

void Battle_Scene::sync_give_up_data_to_client(int side) {

	Battle_Give_Up_Map::iterator find_it = battle_give_up_map_.find(side);
	if (battle_give_up_map_.end() == find_it) return;

	MSG_50200117 msg;
	msg.sponsor = find_it->second.sponsor;
	msg.side = find_it->first;
	int secs = 15 - (Time_Value::gettimeofday() - find_it->second.give_up_time).sec();
	msg.secs = secs > 0 ? secs : 0;
	Id_Int_Int value;
	for (Role_Int_Map::iterator it = find_it->second.give_up_map_.begin();
			it != find_it->second.give_up_map_.end(); ++it) {
		value.reset();
		value.id = it->first;
		value.val1 = it->second;
		msg.give_up_vec.push_back(value);
//		MSG_DEBUG("role_id=%ld, state=%d", value.id, value.val1);
	}
	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	broadcast_around(buf);
}

void Battle_Scene::sync_give_up_data_when_client_scene_completed(Scene_Player &player) {

	Battle_Give_Up_Map::iterator find_it = battle_give_up_map_.find(
			player.mover_detail().battle_position.pos);
	if (battle_give_up_map_.end() == find_it) return;

	int secs = 15 - (Time_Value::gettimeofday() - find_it->second.give_up_time).sec();
	if (secs <= 0) return;

	MSG_50200117 msg;
	msg.sponsor = find_it->second.sponsor;
	msg.side = find_it->first;
	msg.secs = secs;
	Id_Int_Int value;
	for (Role_Int_Map::iterator it = find_it->second.give_up_map_.begin();
			it != find_it->second.give_up_map_.end(); ++it) {
		value.reset();
		value.id = it->first;
		value.val1 = it->second;
		msg.give_up_vec.push_back(value);
	}

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	player.send_to_client(buf);
}

void Battle_Scene::set_other_info(const Other_Info &ot) {
	other_info_.type = ot.type;
	other_info_.int_v1 = ot.int_v1;
	other_info_.int_v2 = ot.int_v2;
	other_info_.iv_1 = ot.iv_1;
	other_info_.iv_2 = ot.iv_2;
}
const Other_Info &Battle_Scene::get_other_info(void) {
	return other_info_;
}

void Battle_Scene::kill_partner_called_when_dead(Fighter &fighter) {

	if (0 == fighter.hero_self()) return;
	for (Mover_Map::iterator it = mover_map_.begin(); it != mover_map_.end(); ++it) {
		if (0 == it->second || 0 == it->second->fighter()) continue;
		if (it->second->fighter()->dead_status()) continue;

		if (0 == it->second->monster_self()) continue;
		if (it->second->monster_self()->caller_id() != fighter.role_id()) continue;
		it->second->fighter()->manual_set_fighter_dead();
	}
}
