/*
 * Fighter.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "item_user/Item_User_Def.h"
#include "Fighter.h"
#include "Scene_Module_Head.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Fighter.h"
#include "Move_Scene.h"
#include "Battle_Scene.h"
#include "Fighter_Func.h"
#include "Fighter_Struct.h"
#include "Err_Code.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "Msg_Role_Scene_Struct.h"
#include "Fighter_Struct.h"
#include "Msg_Fight_Struct.h"
#include "status/Status_Struct.h"
#include "NPC/hero/Hero.h"
#include "Scene_Public.h"
#include "Config_Cache_Scene.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Buff.h"

Fighter::Fighter() : fighter_detail_(0),
battle_status_(READY),
current_skill_id_(0),
is_dead_broadcast(false),
is_singing_end(false),
skill_step_used_(0),
last_skill_step_used_(0),
pre_used_skill_id_(0),
pre_use_morale_(0),
first_used_skill_(0),
cost_morale(0.0),
spec_btl_morale_tmp_(0.0),
spec_btl_blood_tmp_(0.0),
battle_ttl_damage_(0),
battle_ttl_be_damaged_(0),
battle_ttl_cure_(0),
cache_skill_step_used_(0),
cache_current_skill_id_(0){  }

Fighter::~Fighter() { }

void Fighter::reset(void) {
	// for debug
	if(is_in_battle()){
		MSG_TRACE_ABORT("reset error: role_id = %ld, scene:%d, battle id:%d", role_id(), move_scene_id(), battle_scene_id());
	}
	if(is_in_mover_scene()){
		MSG_USER_TRACE("reset error: role_id = %ld, scene:%d", role_id(), move_scene_id());
	}
	Mover::reset();
	Status::reset();

	fighter_detail_ 						= 0;
	last_skill_use_timestamp_ 		= Time_Value::zero;
	last_be_attack_timestamp_ 	= Time_Value::zero;
	conflict_time_							= Time_Value::zero;
	sing_time_start_						= Time_Value::zero;

	skill_tick_time_ 						= Time_Value::zero;
	skill_step_used_ 					= 0;
	current_skill_id_ 					= 0;
	is_dead_broadcast 					= false;
	is_singing_end 						= false;
	battle_status_ 						= READY;
	CLEAR_VECTOR(Role_Vec, simulate_targets_);
	current_effect_map_.clear();

	pre_used_skill_id_ 					= 0;
	pre_use_morale_ 					= 0;
	first_used_skill_ 						= 0;
	cost_morale 							= 0;
	spec_btl_morale_tmp_ 			= 0.0;
	spec_btl_blood_tmp_ 			= 0.0;

	battle_ttl_damage_				= 0.0;
	battle_ttl_be_damaged_		= 0.0;
	battle_ttl_cure_						= 0.0;

	fix_buff_time_.clear();
	fix_skill_cd_time_.clear();
	fix_morale_cost_map_.clear();
	fight_be_skill_map_.clear();
	be_skill_effect_used_map_.clear();

	cache_skill_tick_time_ 			= Time_Value::zero;
	cache_skill_step_used_ 			= 0;
	cache_current_skill_id_ 			= 0;
	CLEAR_VECTOR(Role_Vec, cache_simulate_targets_);
	cache_current_effect_map_.clear();
}

void Fighter::module_init(void) { }

void Fighter::sign_in(void) { }

void Fighter::sign_out(void) { }

void Fighter::load_detail(Block_Buffer &detail_buf) {
	fighter_detail_->deserialize(detail_buf);
	Status::load_detail();
}

void Fighter::set_detail_prt(Fighter_Detail &detail) {
	fighter_detail_ = &detail;
	Status::load_detail();
}

int Fighter::phy_power_recover_time(void){
	return fighter_detail_->phy_power_recover_time;
}

int Fighter::phy_power_buy_times(void){
	return fighter_detail_->phy_power_buy_times;
}

void Fighter::set_strength_cur(int value){
	fighter_detail_->strength_current.basic.basic_value = value;
}

void Fighter::set_strength_max(int value){
	fighter_detail_->strength_max.basic.basic_value = value;
}

void Fighter::set_phy_power_recover_time(int value){
	fighter_detail_->phy_power_recover_time = value;
}

void Fighter::set_phy_power_buy_times(int value){
	fighter_detail_->phy_power_buy_times = value;
}

int Fighter::get_first_used_skill(void) const {
	return first_used_skill_;
}

double Fighter::current_mp_rate(void) {
	return fighter_detail_->current_morale() / fighter_detail_->max_morale();
}

double Fighter::blood_pool_max(void) {
	return fighter_detail_->fetch_fight_property(PT_BLOOD_POOL_MAX, Prop_Value::ELEM_FIGHT);
}

double Fighter::blood_pool_current(void) {
	return fighter_detail_->current_blood_pool();
}

double Fighter::blood_max(void) {
	return fighter_detail_->fetch_fight_property(PT_BLOOD_MAX, Prop_Value::ELEM_FIGHT);
}

double Fighter::blood_current(void) {
	return fighter_detail_->current_blood();
}

double Fighter::current_blood_rate(void) {
	double value_max = fighter_detail_-> fetch_fight_property(PT_BLOOD_MAX, Prop_Value::ELEM_FIGHT);
	double value = fighter_detail_->current_blood();
	if (!(0 < value_max) || 0 > value) {
		MSG_USER("role_id=%ld, blood_max=%f, blood_current=%f", role_id(), value_max, value);
		return 0.0;
	}
	return value / value_max;
}

double Fighter::mp_max(void) {
	return fighter_detail_->max_morale();
}

double Fighter::mp_current(void) {
	return fighter_detail_->current_morale();
}

void Fighter::fix_all_partner_conflict_time(const Time_Value& now) {
	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (0 == cfg || 3 == cfg->skill_conflict) return;

	Skill_DB_Info *skill = get_skill_db_info(current_skill_id_);
	if (0 == skill) return;
	const Skill_Detail* detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill->skill_id, skill->skill_level);
	if (0 == skill || 0 == detail) return;

	battle_scene()->fix_partner_wait_time(detail->hurt_time, now, *this);
}

void Fighter::fix_partner_wait_time(const Time_Value &add_time, const Time_Value &now) {
	if (current_skill_id_ == 0 ||
		skill_step_used_ != 0  ||
		now < (skill_tick_time_ + conflict_time_)) return;

	conflict_time_ = (now - skill_tick_time_) + add_time;
}

void Fighter::fight_tick(const Time_Value &now) {
	if (battle_status_ == READY) return;

	if (is_dizz() && skill_step_used_ == 0) {	//被眩晕不能使用技能
		if (current_skill_id_ != 0)  break_use_skill();					//打断使用技能
		return;
	}

	// tick = 100ms， conflict_time当且仅当在吟唱阶段，才有此字段
	if ((0 == skill_step_used_ && now < skill_tick_time_ + conflict_time_) ||
			(0 != skill_step_used_ && now < skill_tick_time_)) return;

	// singing end
	if (0 == skill_step_used_ && 0 != current_skill_id_ && !is_singing_end) {
		is_singing_end = true;
		sing_end(now, current_skill_id_);
	}

	if(current_skill_id_ == 0){
		use_skill(ai_select_skill());
		return;
	}

	if (skill_step_used_ == 0) {
		if (this->dead_status()) return;
		if (0 == battle_scene()) return;
		// 检查技能冲突
		int delay_ms = battle_scene()->check_skill_conflict(now, *this);
		// conflict
		if (delay_ms > 0) return;
		check_and_clear_simulate_target();

		if (validate_battle_scene() && battle_scene()->check_all_enemy_dead(*this)) {
			if (!battle_scene()->is_battle_over()) return;

			simulate_targets_.clear();
			current_skill_id_ = 0;
			MSG_DEBUG("skill id zeor role:%ld", role_id(), this->battle_scene_id());
			return;
		}

		// non-conflict
		conflict_time_ = Time_Value::zero;
		skill_step_used_ = 1;

		fix_all_partner_conflict_time(now);
		confirm_current_skill_effect();

		Skill_DB_Info *skill = get_skill_db_info(current_skill_id_);
		if (0 != skill) {
			const Skill_Detail *detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill->skill_id, skill->skill_level);
			if (0 != detail)  add_skill_cd_time(detail->skill_cd_vec, now);
		}
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
		if (0 != cfg && 1 != cfg->ignore_enemy_conflict)
			battle_scene()->set_statistic_shot(mover_detail().battle_position.pos);
	}

	inner_skill_action();
}

void Fighter::sing_end(const Time_Value &now, int skill_id) {
	return;
}

void Fighter::check_and_clear_simulate_target(void) {
	if (simulate_targets_.size() <= 0) return ;
	bool reset = false;
	for (Role_Vec::iterator it = simulate_targets_.begin();
			it != simulate_targets_.end(); ++it) {
		Mover *mover = battle_scene()->find_mover_with_role_id(*it);
		if (0 != mover && 0 != mover->fighter() &&
				!mover->fighter()->dead_status()) continue;
		reset = true;
		break;
	}

	if (reset)
		simulate_targets_.clear();
}

bool Fighter::all_unit_dead(const Role_Vec &vec) {
	if (0 == vec.size()) return true;
	for (Role_Vec::const_iterator it = vec.begin(); it != vec.end(); ++it){
		Mover *mover = battle_scene()->find_mover_with_role_id(*it);
		if (0 == mover || 0 == mover->fighter()) continue;
		// 有任意一个存活，都不是all dead
		if (!mover->fighter()->dead_status()) return false;
	}
	return true;
}

int Fighter::ai_select_skill(void){
	return 0;
}

void Fighter::call_broadcast_around(Block_Buffer &buf) {
	if (battle_scene()) {
		battle_scene() ->broadcast_around(buf);
	} else {
		scene_broad_around(buf);
	}
}

// 是否可走，如果被晕炫，则不可走
int Fighter::move_able(void) {
	int ret = Mover::move_able();
	if (0 == ret) {
		return (int)is_dizz();
	}
	return ret;
}

void Fighter::restore_hp(double val, bool board) {

	if (val < 0) return;
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = PT_BLOOD_CURRENT;
	setter.operate = O_ADD;
	setter.addi_type = AT_BASIC;
	setter.basic = val;
	fight_setter.push_back(setter);
	//fight_modify_fight_prop(fight_setter, board);
	modify_props_normal(fight_setter, board, 11);
}

void Fighter::reset_player_data(Battle_Scene *scene) {

	if (mover_type() != MOVER_T_PLAYER) return;

	double blood_val = fighter_detail_->current_blood();
	double morale_val = fighter_detail_->fetch_fight_property(Property_Type::PT_INIT_MORALE);

	Fighter* fighter = SCENE_PUBLIC->find_fight_hero(role_id());
	if (0 != fighter && 0 != fighter->npc_self()) {
		morale_val += fighter->npc_self()->npc_detail().fighter_detail_.fetch_fight_property(
				Property_Type::PT_INIT_MORALE);
	}

	int btl_type = scene->get_battle_type();
	int move_scene_type = player_self()->move_scene()->scene_type();
	if ((Battle_Scene::BATTLE_TYPE_ARENA == btl_type ||
		 Battle_Scene::BATTLE_TYPE_HONOR_ARENA == btl_type ||
		 Battle_Scene::BATTLE_TYPE_TEAM_ARENA == btl_type ||
		 Battle_Scene::BATTLE_TYPE_DRAGON_VALE == btl_type ||
		 Battle_Scene::BATTLE_TYPE_KNIGHT_TRIALS == btl_type ||
		 Battle_Scene::BATTLE_TYPE_CHALLENGE == btl_type)
			&& MOVER_T_PLAYER == this->mover_type()) {
		spec_btl_morale_tmp_ = mp_current();
		spec_btl_blood_tmp_ = blood_current();
		blood_val = blood_max();
	} else if (Battle_Scene::BATTLE_TYPE_PVE == btl_type &&
			move_scene_type == Elite_Scene_Type) {
		spec_btl_morale_tmp_ = mp_current();
		spec_btl_blood_tmp_ = blood_current();

		morale_val = (morale_val > fighter_detail_->elite_btl_morale) ? morale_val : fighter_detail_->elite_btl_morale;
	} else {
		morale_val = (morale_val > mp_current()) ? morale_val : mp_current();
	}
	reset_morale_and_hp(morale_val, blood_val, false);

}

void Fighter::reset_morale_and_hp(double morale_val, double hp_val, bool board) {
	if (morale_val < 0 && hp_val < 0) return;
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	if (!(morale_val < 0)) {
		setter.reset();
		setter.prop_type = PT_MORALE_CURRENT;
		setter.operate = O_SET;
		setter.addi_type = AT_BASIC;
		setter.basic = morale_val;
		fight_setter.push_back(setter);
	}
	if (!(hp_val < 0)) {
		setter.reset();
		setter.prop_type = PT_BLOOD_CURRENT;
		setter.operate = O_SET;
		setter.addi_type = AT_BASIC;
		setter.basic = hp_val;
		fight_setter.push_back(setter);
	}
	modify_props_normal(fight_setter, board, 7);
}

void Fighter::blood_pool_add_blood() {
	// 非玩家类战斗单位，无须血池加血
	if (mover_type() != Mover_Type::MOVER_T_PLAYER) return;

	double cur_blood = blood_current();
	double max_blood = blood_max();
	if (!(cur_blood < max_blood))
		return;

	double value = max_blood - cur_blood;

	double add = value;
	if (!has_status_id(11100300)) {			// 没有治疗精灵
		double cur_blood_pool = blood_pool_current();
		double one_percent_max = max_blood * 0.1;
		if (!(cur_blood_pool > 0) && cur_blood < one_percent_max)
			cur_blood_pool = one_percent_max - cur_blood;
		add = cur_blood_pool > value ? value : cur_blood_pool;
		Prop_Setter_Vec blood_pool_setter;
		Prop_Setter setter;
		setter.prop_type = PT_BLOOD_POOL_CURRENT;
		setter.operate = O_SUB;
		setter.addi_type = AT_BASIC;
		setter.basic = add;
		blood_pool_setter.push_back(setter);
		//fight_modify_fight_prop(blood_pool_setter, false);
		modify_props_normal(blood_pool_setter, false, 1);
	}
	restore_hp(add, false);
}

bool Fighter::dead_status(void) {
	if (DIE_OVER == battle_status_) return true;
	return (this->fighter_detail_->current_blood() > 0.0) ? false : true;
}

bool Fighter::dead_and_cast(void) {
	bool dead = !(fighter_detail_->current_blood() > 0.0);
	return dead && is_dead_broadcast;
}

bool Fighter::dead_and_skill_end(void) {
	bool is_dead = (this->fighter_detail_->current_blood() > 0.0) ? false : true;
	return is_dead && (0 == skill_step_used_) && is_dead_broadcast;
}

Time_Value Fighter::get_conflic_time(const Time_Value & now)const {
	if (skill_step_used_ != 0 || now < skill_tick_time_) return Time_Value::zero;
	return now - skill_tick_time_;
}

bool Fighter::is_singing(const Time_Value &now) const {
	if (0 == current_skill_id_) return false;
	if (0 != skill_step_used_ || now < skill_tick_time_) return false;
	return true;
}

// 获取技能等级
int Fighter::get_learned_skill_lv(int skill_id) const {
	if (0 == skill_id) return 0;
	if (0 == fighter_detail_) return 0;

	Talent_Map::const_iterator find_it = fighter_detail_->talent_map.find(fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) return 0;
	Skill_DB_Info_Map::const_iterator skill_it = find_it->second.skill_db_info_map.find(skill_id);
	if (skill_it == find_it->second.skill_db_info_map.end()) return 0;
	return skill_it->second.skill_level;
}

// 召唤战斗单位，如果已经召唤，再次召唤时，先杀死原有的召唤单位
int Fighter::call_monster_partner(int monster_partner_id, int loc_type, Fix_Call_Monster_Vec fix_call_monster_vec,
		Skill_Running_Info &skill_running) {
	int idx = mover_detail().battle_position.idx;
	while (idx > 3) {
		idx -= 3;
	}
	if (0 == battle_scene()) return -1;
	Position_Key position;
	int pos = mover_detail().battle_position.pos;
	if (1 == loc_type) {
		position.idx = idx;
		position.pos = pos;
	} else {
		position.idx = 0;
		position.pos = (pos == 0)? 1 : 0;
	}
	Mover* mover = battle_scene()->find_mover_with_position(position);
	if (0 != mover && 0 != mover->fighter()) {
		Block_Buffer buf;
		MSG_81000045 msg;
		msg.role_id = mover->role_id();
		msg.forced_dead = 1;
		MAKE_MSG_BUF(msg, buf);
		scene_broad_around(buf);
		mover->exit_battle_scene(9);
	}

	NPC_Addition_Info add_info;
	add_info.birth_coord.x = position.pos;
	add_info.birth_coord.y = position.idx;
	add_info.birth_by_skill = skill_running.skill_id;
	if(add_info.birth_by_skill == 0){
		add_info.birth_by_skill = -1;
	}
	for (Fix_Call_Monster_Vec::iterator it = fix_call_monster_vec.begin();
			it != fix_call_monster_vec.end(); ++it) {
		double value = fighter_detail_->fetch_fight_property(static_cast<Property_Type>(it->type));
		it->value = value * it->cfg_percent;
		it->cfg_percent = 0;
	}

	NPC_MANAGER->create_battle_monster(monster_partner_id, NULL, add_info, battle_scene(), fix_call_monster_vec);
	mover = 0;
	mover = battle_scene()->find_mover_with_position(position);
	if (0 == mover) return 0;
	if (0 != mover->fighter()) mover->fighter()->set_battle_status(BATTLING);

	role_id_t master_role_id = 0;
	if(this->player_self()){
		master_role_id = this->role_id();
	}else if(this->hero_self()){
		master_role_id = this->hero_self()->master_role_id();
	}else if(this->monster_self()){
		master_role_id = this->monster_self()->master_role_id();
	}

	if(mover->monster_self()){
		mover->monster_self()->set_master_role_id(master_role_id);
		mover->monster_self()->set_caller_id(role_id());
	}

	skill_running.step_target_map[skill_step_used_].clear();
	skill_running.step_target_map[skill_step_used_].push_back(mover->role_id());

	battle_scene()->clear_enemy_simulate_target(*this);

	return 0;
}

void Fighter::transfer_call_monster(const Skill_Effect &effect, Skill_Running_Info &skill_running) {
	if (effect.transfer_call_monster_vec.size() <= 0) return;

	skill_running.step_target_map[skill_step_used_].clear();

	int pos = mover_detail().battle_position.pos;
	for (Transfer_Call_Monster_Vec::const_iterator it = effect.transfer_call_monster_vec.begin();
			it != effect.transfer_call_monster_vec.end(); ++it) {

		if (it->idx == this->mover_detail().battle_position.idx) continue;
		Position_Key position(pos, it->idx);
		if (0 == battle_scene()) continue;
		Mover* mover = battle_scene()->find_mover_with_position(position);
		if (0 != mover && 0 != mover->fighter()) {
			Block_Buffer buf;
			MSG_81000045 msg;
			msg.role_id = mover->role_id();
			msg.forced_dead = 1;
			MAKE_MSG_BUF(msg, buf);
			scene_broad_around(buf);
			mover->exit_battle_scene(11);
		}
		NPC_Addition_Info add_info;
		add_info.birth_coord.x = position.pos;
		add_info.birth_coord.y = position.idx;
		add_info.birth_by_skill = effect.effect_id;
		if(add_info.birth_by_skill == 0){
			add_info.birth_by_skill = -1;
		}

		Fix_Call_Monster_Vec vec;
		if (it->fix_blood_percent < 1) {
			Fix_Call_Monster fix;
			fix.type = Property_Type::PT_BLOOD_CURRENT;
			fix.value = 0;
			fix.cfg_percent = it->fix_blood_percent;
			vec.push_back(fix);
		}
		NPC_MANAGER->create_battle_monster(it->monster_type_id, NULL, add_info, battle_scene(), vec);
		mover = 0;
		mover = battle_scene()->find_mover_with_position(position);
		if (0 == mover) continue;
		if (0 != mover->fighter()) mover->fighter()->set_battle_status(BATTLING);
		skill_running.step_target_map[skill_step_used_].push_back(mover->role_id());

		role_id_t master_role_id = 0;
		if(this->player_self()){
			master_role_id = this->role_id();
		}else if(this->hero_self()){
			master_role_id = this->hero_self()->master_role_id();
		}else if(this->monster_self()){
			master_role_id = this->monster_self()->master_role_id();
		}

		if(mover->monster_self()){
			mover->monster_self()->set_master_role_id(master_role_id);
			mover->monster_self()->set_caller_id(role_id());
		}
	}

	battle_scene()->clear_enemy_simulate_target(*this);
}

void Fighter::handle_buff_on_off(const Skill_Effect &effect, int skill_id) {

	int buff_id =effect.val1.id;
	if (this->has_status_id(buff_id)) { // del op
		this->erase_status_type_for_msg(buff_id);
	} else { // add op
		Fighter_Vec vec;
		vec.push_back(this);
		this->handle_buff(effect.val1, vec, role_id(), skill_id, Statue_Source_Type::TYPE_SOURCE_SKILL);
	}
}

// 战斗剧情中，如果刷新，则路过剧情
void Fighter::continue_battle_after_refresh(void) {
	if (0 == battle_scene()) return;
	battle_scene()->handle_plot_client_request(PLOT_CONTINUE);
}

// 获取当前战斗step 处理对象
void Fighter::get_current_step_targets(Role_Vec& vec) {
	vec.clear();
	if (0 == skill_step_used_) return;
	Skill_Running_Info_Map::iterator find_it =
			fighter_detail_->skill_running_info_map.find(current_skill_id_);
	if (find_it == fighter_detail_->skill_running_info_map.end())
		return;
	vec = find_it->second.step_target_map[1];
}

bool Fighter::will_use_spec_skill(const Time_Value & now) const {
	//检查是否在使用大招或者正要使用大招
	if (0 >= current_skill_id_) return false;
	const Skill_Config * cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (0 == cfg) return false;
	if (3 > cfg->skill_conflict) return false;
	if (skill_step_used_ == 0 && now < (skill_tick_time_+conflict_time_)) return false;
	return true;
}

bool Fighter::is_using_spec_skill() const {
	//检查是否在使用大招或者正要使用大招
	if (0 >= current_skill_id_) return false;
	const Skill_Config * cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (0 == cfg) return false;
	if (3 > cfg->skill_conflict) return false;
	if (skill_step_used_ == 0) return false;
	return true;
}

void Fighter::fix_sing_time(const Time_Value &add_time, const Time_Value &now) {
	if (current_skill_id_ == 0 || skill_step_used_ != 0 || now >= skill_tick_time_) return;
	skill_tick_time_ += add_time;
}

void Fighter::fix_skill_step_time(const Time_Value & add_time) {
	skill_tick_time_ += add_time;
}

void Fighter::calc_conflict_time(const Time_Value &now, int fix_sing_time) {
	if (0 == current_skill_id_ || 0 != skill_step_used_) return;
	if (now < skill_tick_time_ && fix_sing_time > 0) { // fighter is sing
		skill_tick_time_ += Time_Value(fix_sing_time/1000, fix_sing_time%1000*1000);
	}
}

bool Fighter::check_target_conflict(int target_conflict, const Role_Vec &vec) {
	if (0 == current_skill_id_) return false;
	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (0 == cfg || (1 == target_conflict && 1 == cfg->target_conflict)) return false;

	if (0 == skill_step_used_) {
		for (Role_Vec::const_iterator it = vec.begin(); it != vec.end(); ++it) {
			if (std::find(simulate_targets_.begin(), simulate_targets_.end(), *it) !=
					simulate_targets_.end()) return true;
		}
	} else {
		if (3 == target_conflict) {
			if (std::find(vec.begin(), vec.end(), role_id()) != vec.end()) return true;
		}

		Skill_Running_Info_Map::iterator find_it = fighter_detail_->skill_running_info_map.find(current_skill_id_);
		if (find_it == fighter_detail_->skill_running_info_map.end()) return false;
		Role_Vec &find_vec = find_it->second.step_target_map[last_skill_step_used_];
		if (0 == find_vec.size()) return false;
		for (Role_Vec::const_iterator it = vec.begin(); it != vec.end(); ++it) {
			if (std::find(find_vec.begin(), find_vec.end(), *it) != find_vec.end()) return true;
		}
	}
	return false;
}

// 检查是否正在使用浮空技能
bool Fighter::is_using_floating_skill(void) const {
	if (0 == current_skill_id_ || 0 == skill_step_used_) return false;
	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (0 == cfg) return false;
	return Skill_Type::TYPE_FLOAT_SKILL == cfg->skill_type;
}

// 停止吟唱
int Fighter::stop_sing(void){
	if (0 != skill_step_used_) return -1;

	skill_step_used_ = 0;
	current_skill_id_ = 0;
	skill_tick_time_ = Time_Value::zero;
	MSG_DEBUG("skill id zeor role:%ld", role_id(), this->battle_scene_id());
	return 0;
}

Skill_DB_Info* Fighter::get_skill_db_info(int skill_id) {
	if (0 == skill_id || 0 == fighter_detail_) return 0;

	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) {
		MSG_USER("cannot find skill, no talent, cur_talent_id=%d, skill_id=%d",
				fighter_detail_->cur_used_talent_, skill_id);
		return 0;
	}
	Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.find(skill_id);
	if (it == find_it->second.skill_db_info_map.end()) {
		MSG_USER("cannot find skill,talnet=%d, skill_id=%d, role_id=%ld",
				fighter_detail_->cur_used_talent_, skill_id, role_id());
		return 0;
	}
	return &it->second;
}

int Fighter::break_use_skill() {
	if (0 != skill_step_used_) return 0;

	//恢复怒气
	if ((current_skill_id_ != fighter_detail_->default_skill)
			&& (skill_step_used_ == 0)) {
		Skill_DB_Info* skill = get_skill_db_info(current_skill_id_);
		if (skill != 0) {
			const Skill_Detail *skill_detail =
					CONFIG_CACHE_FIGHTER->skill_detail_cache(current_skill_id_,
							skill->skill_level);
			if (0 != skill_detail) {
				Skill_Cost_Vec cost_vec = skill_detail->fight_cost_vec;
				for (Skill_Cost_Vec::iterator cost_find_it = cost_vec.begin();
						cost_find_it != cost_vec.end(); ++cost_find_it) {
					cost_find_it->value = -cost_find_it->value;
				}
				fight_add(cost_vec, TYPE_SKILL_RETURN);
			}
		}
	}
	if(battle_scene()){
		battle_scene()->listen_break_use_skill(this, current_skill_id_);
	}
	current_skill_id_ 		= 0;
	skill_step_used_ 		= 0;
	last_skill_step_used_ = 0;
	pre_used_skill_id_ 		= 0;
	pre_use_morale_			= 0;
	simulate_targets_.clear();
	current_effect_map_.clear();
	MSG_DEBUG("skill id zeor role:%ld", role_id(), this->battle_scene_id());
	return 0;
}

int Fighter::handle_fight_pause(int type) {
	if (0 == battle_scene()) return ERROR_INNER_SERVER;
	int ret = battle_scene()->handle_plot_client_request(type);
	if (0 == ret) {
		MSG_50200111 msg;
		msg.type = type;
		THIS_SEND_TO_CLIENT(msg);
	}
	return ret;
}

bool Fighter::check_locked_dead_target(const role_id_t chk_id, const role_id_t atk_id) {
	if (0 == chk_id) return false;
	if (0 == fighter_detail_ || 0 == current_skill_id_) return false;

	// 吟唱阶段选定角色不算锁定
	if (0 == skill_step_used_) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
		if (0 == cfg || TYPE_ADD_SKILL != cfg->skill_type) return false;

		Role_Vec::iterator exist_it = std::find(simulate_targets_.begin(),
				simulate_targets_.end(), chk_id);
		if (exist_it != simulate_targets_.end()) return true;
	} else if (0 != skill_step_used_){
		// 检查攻击阶段锁定
		uint i = (role_id() == atk_id) ? skill_step_used_ + 1 : skill_step_used_;
		Skill_Effect_Map::iterator find_it = current_effect_map_.find(i);
		// 不存在的效果，非锁定
		if (find_it == current_effect_map_.end()) return false;

		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
		if (0 == cfg) return false;

		Skill_Running_Info_Map::iterator info_it = fighter_detail_->skill_running_info_map.find(current_skill_id_);
		if (info_it == fighter_detail_->skill_running_info_map.end()) return false;

		//  浮空技特殊检查
		if (TYPE_FLOAT_SKILL == cfg->skill_type &&
			(EFFECT_FLOAT_OVER == find_it->second.effect_type ||
			 EFFECT_ATTACK_DEFAULT == find_it->second.effect_type ||
			 EFFECT_ATTACK_BUFF_SELF == find_it->second.effect_type ||
			 EFFECT_ATTACK_BUFF_OPP == find_it->second.effect_type)) {
			Role_Vec::iterator exist_it = std::find(info_it->second.step_target_map[1].begin(),
					info_it->second.step_target_map[1].end(), chk_id);
			if (exist_it != info_it->second.step_target_map[1].end()) return true;
		}

		if (find_it->second.target_sellect_info.select_type == TARGET_TEMP) return false;

		// 非攻击帧或者浮空技结束帧，则非锁定
		if (EFFECT_FLOAT_OVER != find_it->second.effect_type &&
				EFFECT_ATTACK_DEFAULT != find_it->second.effect_type &&
				EFFECT_PLAY_EFFECT != find_it->second.effect_type) return false;

		// 延续之前选择目标，检查是否锁定
		if (TARGET_EXIST == find_it->second.target_sellect_info.select_type) {
			if (EFFECT_FLOAT_OVER == find_it->second.effect_type) {
				Role_Vec::iterator exist_it = std::find(info_it->second.step_target_map[1].begin(),
						info_it->second.step_target_map[1].end(), chk_id);
				if (exist_it != info_it->second.step_target_map[1].end()) return true;
			} else {
				Skill_Running_Info_Map::iterator info_it = fighter_detail_->skill_running_info_map.find(current_skill_id_);
				if (info_it == fighter_detail_->skill_running_info_map.end()) return false;

				Role_Vec::iterator exist_it = std::find(info_it->second.step_target_map[i-1].begin(),
						info_it->second.step_target_map[i-1].end(), chk_id);
				if (exist_it != info_it->second.step_target_map[i-1].end()) return true;
			}
		} else if (TARGET_AUTO == find_it->second.target_sellect_info.select_type &&
				TYPE_ENEMY == find_it->second.target_sellect_info.target_relation) {
			// 如果是TARGET_AUTO的情况，唯一一种情况下会锁定，
			// 当且仅当所有敌人死亡，且是Fighter上个Effect攻击的目标
			if (!validate_battle_scene()) return false;
			// 非所以敌人都死亡
			if (!battle_scene()->check_all_enemy_dead(*this)) return false;
			Skill_Running_Info_Map::iterator info_it = fighter_detail_->skill_running_info_map.find(current_skill_id_);
			if (info_it == fighter_detail_->skill_running_info_map.end()) return false;

			Role_Vec::iterator exist_it = std::find(info_it->second.step_target_map[i-1].begin(),
					info_it->second.step_target_map[i-1].end(), chk_id);
			if (exist_it != info_it->second.step_target_map[i-1].end()) return true;
		}
	}

	return false;
}

bool Fighter::is_be_skill(int skill_id) const {
	if (0 == skill_id) return false;
	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (0 == cfg) return false;
	return cfg->skill_sort == Skill_Sort::BE_SKILL;
}

int Fighter::can_use_skill(int skill_id){
	Time_Value now = Time_Value::gettimeofday();
	if (!skill_id) {
		skill_id = fighter_detail_->default_skill;
	}

	if (!skill_id) {
		MSG_USER("role:%ld", role_id());
		return ERROR_INNER_SERVER;
	}

	Skill_DB_Info* skill = get_skill_db_info(skill_id);
	if (skill == 0) return ERROR_INNER_SERVER;
	if (0 == skill->skill_loc &&
			fighter_detail_->default_skill != skill->skill_id) return ERROR_INNER_SERVER;

	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	const Skill_Detail *skill_detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(
			skill_id, skill->skill_level);
	if (0 == cfg || 0 == skill_detail) {
		MSG_USER("skill:%d", skill_id);
		return ERROR_INNER_SERVER;
	}

	if (Skill_Sort::BE_SKILL == cfg->skill_sort) return ERROR_INNER_SERVER;
	if (Skill_Type::TYPE_ADD_SKILL == cfg->skill_type) return ERROR_INNER_SERVER;

	int ret =  use_skill_inner_check(skill_id, *skill_detail, cfg->skill_series, now);
	if (0 != ret) return ret;

	double fix_morale_cost = get_skill_fix_morale_cost(skill_id);
	Skill_Cost_Vec cost_vec;
	if (!fight_cost_check(skill_detail->fight_cost_vec, cost_vec, fix_morale_cost)) {
		return ERROR_MORALE_LIMIT;
	}

	return 0;
}

void Fighter::add_skill_cd_time(const Skill_Cd_Vec & cd_vec, const Time_Value& Now) {
	Skill_Cd_Vec::const_iterator iter;
	for (iter = cd_vec.begin(); iter != cd_vec.end(); ++iter) {
		int fixed_cd_time = 0;
		Int_Int_Map::iterator find_it = fix_skill_cd_time_.find(iter->type);
		if (find_it != fix_skill_cd_time_.end()) {
			fixed_cd_time = find_it->second;
		}
		Common_Skill_Cd skill_cd;
		skill_cd.type_ = iter->type;
		skill_cd.last_use_ = Now;
		skill_cd.millisecond_ = iter->millisecond + fixed_cd_time;

		fighter_detail_->common_cd_map_[skill_cd.type_] = skill_cd;
	}
}

//int Fighter::use_skill_inner_check(int skill_id, const Skill_Detail& skill_detail, int skill_talent, const Time_Value& now) {
//
//	//检查所有敌人是否已死
//	if (validate_battle_scene()) {
//		if (battle_scene()->check_all_enemy_dead(*this)) {
//			//battle_scene()->set_check_over_state(*this);
//			return ERROR_ENEMY_ALL_DEAD;
//		}
//	}
//
//	// 被眩晕不能使用技能
//	if (is_dizz()) {
//		MSG_USER("user in dizz, cannot use skill, role_id=%ld, skill_id=%d", role_id(), skill_id);
//		return ERROR_ROLE_DIZZ;
//	}
//
//	//第一个释放的技能应该是默认技能
//	if (skill_id != fighter_detail_->default_skill &&
//			battle_status_ == READY) {
//		if (skill_detail.pre_cd_vec.size() > 0) return ERROR_INNER_SERVER;
//		battle_scene()->first_use_skill_trigger(skill_id, *this);
//		return BTL_ERR_PRE_USE;
//	}
//
//	// 默认技能不能打断默认技能
//	if ((skill_id == fighter_detail_->default_skill
//			&& current_skill_id_ == fighter_detail_->default_skill)
//			&& skill_step_used_) {
//		MSG_USER("role:%ld", role_id());
//		return ERROR_INNER_SERVER;
//	}
//
//	//只能在默认技能吟唱中才能使用技能
//	if (skill_step_used_ ||
//		(current_skill_id_ && current_skill_id_ != fighter_detail().default_skill)) {
//		pre_used_skill_id_ = skill_id;
//		return BTL_ERR_PRE_USE;
//	}
//
//	//检查是否在cd中
//	Skill_Cd_Vec::const_iterator cd_it;
//	for (cd_it = skill_detail.skill_cd_vec.begin();
//			cd_it != skill_detail.skill_cd_vec.end(); ++cd_it) {
//		Common_Cd_Map::const_iterator ccd_it =
//				fighter_detail().common_cd_map_.find((*cd_it).type);
//		if (ccd_it == fighter_detail().common_cd_map_.end()) continue;
//		//判断是否cd
//		int misec = ccd_it->second.millisecond_ > 0 ? ccd_it->second.millisecond_ : 0;
//		Time_Value cd_time = Time_Value(misec / 1000, (misec % 1000) * 1000);
//
//		//Time_Value ttime = ccd_it->second.last_use_ + cd_time + skill_detail.sing_time;
//		Time_Value ttime = ccd_it->second.last_use_ + cd_time;
//		if (now < ttime) {
////			MSG_USER("skill in cd, cannot use skill, skill_id=%d, role_id=%ld", skill_id, role_id());
//			return ERROR_SKILL_IN_CD;
//		}
//	}
//
//	return 0;
//}

int Fighter::use_skill(int skill_id) {

	//第一个释放的技能应该是默认技能
	if (READY == battle_status_ && 0 != skill_id) {
		Skill_DB_Info *skill = get_skill_db_info(skill_id);
		if(0 == skill) return ERROR_INNER_SERVER;
		const Skill_Detail *skill_detail =
				CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill->skill_level);
		if (0 == skill_detail) return ERROR_INNER_SERVER;
		bool is_default_skill = (skill_id == fighter_detail_->default_skill);
		if (!is_default_skill && skill_detail->pre_cd_vec.size() > 0) return ERROR_INNER_SERVER;
		battle_scene()->first_use_skill_trigger(skill_id, *this);
		return BTL_ERR_PRE_USE;
	}

	if (0 != skill_id)
		return use_skill_inner(skill_id);

	if (0 == skill_id && 0 == pre_used_skill_id_)
		return use_skill_inner(fighter_detail_->default_skill);

	int use_skill_ret = use_skill_inner(pre_used_skill_id_);
	if (0 != use_skill_ret) {
		// return morale
		Skill_Cost_Vec cost_vec;
		Skill_Cost cost;
		cost.type = COST_MORALE;
		cost.coster = COST_SELF;
		cost.value = pre_use_morale_;
		fight_add(cost_vec, TYPE_SKILL_RETURN);
		pre_used_skill_id_ = 0;
		pre_use_morale_ = 0;
		use_skill_ret = use_skill_inner(fighter_detail_->default_skill);
	} else {
		pre_used_skill_id_ = 0;
		pre_use_morale_ = 0;
	}
	return use_skill_ret;
}

int Fighter::use_skill_inner(int skill_id) {

	if (0 == skill_id) {
		MSG_USER("role_id=%ld use non_id skill");
		return ERROR_INNER_SERVER;
	}

	Skill_DB_Info* skill =
			get_skill_db_info(skill_id);
	if (0 == skill) {
		MSG_USER("cannot find skill, role_id = %ld, skill_id=%d", role_id(), skill_id);
		return ERROR_INNER_SERVER;
	}
	if (0 == skill->skill_loc && fighter_detail_->default_skill != skill->skill_id)
		return ERROR_INNER_SERVER;

	const Skill_Config *skill_cfg =
			CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	const Skill_Detail *skill_detail =
			CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill->skill_level);
	if (0 == skill_cfg ||  0 == skill_detail) {
		MSG_USER("skill_id=%d, skill_lv=%d", skill_id, skill->skill_level);
		return ERROR_NO_SKILL_CONFIG;
	}

	if (Skill_Sort::BE_SKILL == skill_cfg->skill_sort) {
		MSG_USER("be skill cannot be used, role_id=%ld, skill_id=%d", role_id(), skill_id);
		return ERROR_CANNOT_USE_BE_SKILL;
	}

	if (Skill_Type::TYPE_ADD_SKILL == skill_cfg->skill_type)
		return use_additional_skill(*skill_cfg, *skill_detail);

	return use_normal_skill(*skill_cfg, *skill_detail, *skill);
}

int Fighter::use_normal_skill(const Skill_Config &cfg, const Skill_Detail& detail,
		const Skill_DB_Info& skill) {

	Time_Value now = Time_Value::gettimeofday();
	int check_ret = use_skill_inner_check(cfg.skill_id, detail, cfg.skill_series, now);
	if (0 != check_ret) return check_ret;

	// 检查消耗东西是否满足
	double fix_morale_cost = get_skill_fix_morale_cost(cfg.skill_id);
	Skill_Cost_Vec cost_vec;
	bool is_fight_cost_satis = fight_cost_check(detail.fight_cost_vec, cost_vec, fix_morale_cost);
	if (!is_fight_cost_satis && cfg.skill_id != pre_used_skill_id_) {
		return ERROR_MORALE_LIMIT;
	}

	// 预使用技能
	bool is_using_skill = (0 != skill_step_used_ && 0 != current_skill_id_);
	if (is_using_skill) {
		// 扣怒气
		fight_cost(cost_vec, 1);
		for (Skill_Cost_Vec::iterator it = cost_vec.begin(); it != cost_vec.end(); ++it) {
			if (it->type != COST_MORALE) continue;
			pre_use_morale_ = fabs(it->value);
		}
		pre_used_skill_id_ = cfg.skill_id;
		return BTL_ERR_PRE_USE;
	}

	return use_normal_skill(cfg, detail, skill, cost_vec, now);
}

int Fighter::use_normal_skill(const Skill_Config &cfg, const Skill_Detail& detail,
		const Skill_DB_Info& skill, const Skill_Cost_Vec &cost_vec,  const Time_Value &now) {

	simulate_select_targets(detail);

	if (simulate_targets_.empty()) {
//		MSG_DEBUG("cannot find targets, role_id=%ld, skill_id=%d", role_id(), skill_id);
		return ERROR_NO_SKILL_TARGET;
	}

	Time_Value last_sing = Time_Value::zero;
	if (current_skill_id_ == fighter_detail_->default_skill && skill_step_used_ == 0) {
		last_sing = now - sing_time_start_;
		last_sing = last_sing > Time_Value(1) ? Time_Value(1) : last_sing;
	}
	current_skill_id_ = cfg.skill_id;
	skill_step_used_ = 0;
	int64_t usec = static_cast<int64_t>((detail.sing_time.sec() * 1000000 + detail.sing_time.usec()) *
			(1+fighter_detail_->fetch_fight_property(PT_SING_RATE, Prop_Value::ELEMADD::ELEM_FIGHT)));
	Time_Value final_sing_time(usec / 1000000, usec % 1000000);
	final_sing_time = final_sing_time > last_sing ? final_sing_time - last_sing : Time_Value::zero;
	skill_tick_time_ = now + final_sing_time;
	sing_time_start_ = now;
	conflict_time_ = Time_Value::zero;
	is_singing_end	= false;

	MSG_81000070 msg;
	msg.attack_id = role_id();
	msg.skill_id = current_skill_id_;
	msg.skill_lv = skill.skill_level;
	msg.errcode = BTL_ERR_SUCCESS;
	msg.step_time = final_sing_time.sec() * 1000 + final_sing_time.usec() / 1000;
	uint8_t ef_size = detail.step_effect.size();
	for (uint8_t i=1 ; i<=ef_size; ++i) {
		Skill_Effect_Map::const_iterator ef_it = detail.step_effect.find(i);
		if (detail.step_effect.end() == ef_it) continue;
		msg.effect_id_vec.push_back(ef_it->second.effect_id);
	}

	for (Role_Vec::iterator p_id = simulate_targets_.begin();
			p_id != simulate_targets_.end(); ++p_id) {
		msg.target_id.push_back(*p_id);
	}

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);

	call_broadcast_around(buf);

	cost_morale = 0;
	if (cfg.skill_id != pre_used_skill_id_) {
		fight_cost(cost_vec, 1);
		for (Skill_Cost_Vec::const_iterator it = cost_vec.begin(); it != cost_vec.end(); ++it) {
			if (it->type != COST_MORALE) continue;
			cost_morale = fabs(it->value);
		}
	} else {
		cost_morale = pre_use_morale_;
	}

	if(battle_scene()){
		battle_scene()->listen_use_skill(this, cfg.skill_id);
	}
	return BTL_ERR_SUCCESS;

	return 0;
}

int Fighter::use_skill_inner_check(int skill_id, const Skill_Detail& skill_detail,
		int skill_talent, const Time_Value& now) {

	if (dead_status())
		return ERROR_INNER_SERVER;

	bool is_chaos_status = is_silent() || is_confusion();
	if (is_chaos_status && skill_id != fighter_detail_->default_skill)
		return ERROR_INNER_SERVER;

	if (!validate_battle_scene())
		return ERROR_INNER_SERVER;
	if (battle_scene()->check_all_enemy_dead(*this))
		return ERROR_ENEMY_ALL_DEAD;

	if (is_dizz()) {
		MSG_USER("user in dizz, cannot use skill, role_id=%ld, skill_id=%d", role_id(), skill_id);
		return ERROR_ROLE_DIZZ;
	}

	if (skill_step_used_== 0 && skill_id == current_skill_id_) {
		return ERROR_SKILL_BEEN_USING;
	}

	// 非默认技能在吟唱中，不能打断
	bool is_cur_use_default = (current_skill_id_ == fighter_detail_->default_skill);
	if (0 == skill_step_used_ && !is_cur_use_default && 0 != current_skill_id_) {
		return ERROR_INNER_SERVER;
	}

	// 默认技能不能打断默认技能
	bool is_use_default = (skill_id == fighter_detail_->default_skill);
	if ((is_use_default && 0 != current_skill_id_)) {
		MSG_USER("role:%ld", role_id());
		return ERROR_INNER_SERVER;
	}

	if (is_use_default && 0 != skill_step_used_) {
		MSG_USER("role:%ld", role_id());
		return ERROR_INNER_SERVER;
	}

	bool is_using_skill = (0 != skill_step_used_ && 0 != current_skill_id_);
	bool has_pre_used_skill = (0 != pre_used_skill_id_);
	if (is_using_skill && has_pre_used_skill)
		return ERROR_SKILL_BEEN_USING;

	//检查是否在cd中
	Skill_Cd_Vec::const_iterator cd_it;
	for (cd_it = skill_detail.skill_cd_vec.begin();
			cd_it != skill_detail.skill_cd_vec.end(); ++cd_it) {
		Common_Cd_Map::const_iterator ccd_it =
				fighter_detail().common_cd_map_.find((*cd_it).type);
		if (ccd_it == fighter_detail().common_cd_map_.end()) continue;
		//判断是否cd
		int misec = ccd_it->second.millisecond_ > 0 ? ccd_it->second.millisecond_ : 0;
		Time_Value cd_time = Time_Value(misec / 1000, (misec % 1000) * 1000);

		//Time_Value ttime = ccd_it->second.last_use_ + cd_time + skill_detail.sing_time;
		Time_Value ttime = ccd_it->second.last_use_ + cd_time;
		if (now < ttime) {
//			MSG_USER("skill in cd, cannot use skill, skill_id=%d, role_id=%ld", skill_id, role_id());
			return ERROR_SKILL_IN_CD;
		}
	}

	return 0;
}

int Fighter::use_additional_skill(const Skill_Config& cfg, const Skill_Detail& detail) {

	if (cfg.skill_type != Skill_Type::TYPE_ADD_SKILL) return BTL_NOT_ADD;

	if (dead_status())
		return ERROR_INNER_SERVER;

	bool is_chaos_status = is_silent() || is_confusion();
	if (is_chaos_status && cfg.skill_id != fighter_detail_->default_skill)
		return ERROR_INNER_SERVER;

	if (0 != current_skill_id_) {
		const Skill_Config *cur_cfg_1 = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
		if (!cur_cfg_1 || cur_cfg_1->skill_type == Skill_Type::TYPE_ADD_SKILL) return BTL_ADD_FAIL;
	}

	Role_Vec tmp_targets;

	// 查找自己或者同伴是否处于浮空技能中
	Fighter *fighter = 0;
	if (mover_detail().is_player()) {
		fighter = SCENE_PUBLIC->find_battle_hero(role_id());
	} else if (mover_detail().is_hero()) {
		fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
		if (!fighter) return BTL_ADD_FAIL;
	} else {
		return BTL_ADD_FAIL;
	}

	Fighter *floater_user = 0;

	if ( check_use_additional_skill() ) {
		get_current_step_targets(tmp_targets);
		floater_user = this;
	} else {
		if (!fighter) return BTL_ADD_FAIL;
		if ( fighter-> check_use_additional_skill() ) {
			fighter->get_current_step_targets(tmp_targets);
			floater_user = fighter;
		} else {
			return BTL_ADD_FAIL;
		}
	}

	if (tmp_targets.size() <= 0) return BTL_ADD_FAIL;

	double fix_morale_cost = get_skill_fix_morale_cost(cfg.skill_id);
	Skill_Cost_Vec cost_vec;
	if (!fight_cost_check(detail.fight_cost_vec, cost_vec, fix_morale_cost)) return BTL_ADD_FAIL;

	// 缓存数据
	cache_skill_tick_time_ = skill_tick_time_;
	cache_skill_step_used_ = skill_step_used_;
	cache_current_skill_id_ = current_skill_id_;
	cache_simulate_targets_ = simulate_targets_;
	cache_current_effect_map_ = current_effect_map_;

	skill_tick_time_ = Time_Value::zero;
	skill_step_used_ = 0;
	current_skill_id_ = cfg.skill_id;
	simulate_targets_ = tmp_targets;
	current_effect_map_.clear();
	confirm_current_skill_effect();

	if (0 != floater_user)
		floater_user->run_all_effect_left();

	Time_Value fix_time = Time_Value::zero;
	for (Skill_Effect_Map::const_iterator it = current_effect_map_.begin();
			it != current_effect_map_.end(); ++it) {
		fix_time += it->second.continue_time;
	}

	// 修正所有战斗单位等待时间
//	battle_scene()->fix_all_unit_tick_time(fix_time, role_id());
	floater_user->fix_skill_step_time(fix_time);
	cache_skill_tick_time_ += fix_time;

	MSG_81000070 msg;
	msg.attack_id = role_id();
	msg.skill_id = current_skill_id_;
	msg.skill_lv = detail.skill_level;
	msg.errcode = BTL_ERR_SUCCESS;

	for (Role_Vec::iterator p_it = simulate_targets_.begin();
			p_it != simulate_targets_.end(); ++p_it) {
		Mover* mover = battle_scene()->find_mover_with_role_id(*p_it);
		if (0 == mover) continue;
		msg.target_id.push_back(mover->role_id());
	}

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);

	call_broadcast_around(buf);
//	if (validate_battle_scene()) {
//		battle_scene()->broadcast_around(buf);
//	}

	fight_cost(detail.fight_cost_vec, 1);

	skill_step_used_ = 1;
	inner_skill_action();

	return BTL_ERR_SUCCESS;
}

//int Fighter::use_skill(int skill_id) {
//
//	int ret = BTL_ERR_SUCCESS;
//	if (0 == skill_id && 0 != pre_used_skill_id_) {
//		ret = use_skill_inner(pre_used_skill_id_);
//		pre_used_skill_id_ = 0;
//		if (BTL_ERR_SUCCESS != ret) {
//			ret = use_skill_inner();
//		}
//	} else {
//		ret = use_skill_inner(skill_id);
//	}
////	if (0 == skill_id) {
////		if (0 != pre_used_skill_id_) {
////			ret = use_skill_inner(pre_used_skill_id_);
////			pre_used_skill_id_ = 0;
////			if (BTL_ERR_SUCCESS != ret) {
////				ret = use_skill_inner();
////			}
////		} else {
////			ret = use_skill_inner(skill_id);
////		}
////	} else {
////		ret = use_skill_inner(skill_id);
////	}
//	return ret;
//}

//int Fighter::use_skill_inner(int skill_id) {
//	if (dead_status()) {
//		return ERROR_INNER_SERVER;
//	}
//
//	Time_Value now = Time_Value::gettimeofday();
//
//	if (is_silent() || is_confusion()) {
//		if (skill_id != fighter_detail_->default_skill) {
//			skill_id = 0;
//		}
//	}
//
//	if (!skill_id) {
//		skill_id = fighter_detail_->default_skill;
//	}
//
//	if (!skill_id) {
//		MSG_USER("role:%ld", role_id());
//		return ERROR_INNER_SERVER;
//	}
//
//	Skill_DB_Info* skill = get_skill_db_info(skill_id);
//	if (skill == 0) {
//		MSG_USER("cannot find skill, role_id=%ld, skill_id=%d", role_id(), skill_id);
//		return ERROR_INNER_SERVER;
//	}
//
//	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
//	const Skill_Detail *skill_detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(
//			skill_id, skill->skill_level);
//	if (0 == cfg || 0 == skill_detail) {
//		MSG_USER("skill:%d", skill_id);
//		return ERROR_NO_SKILL_CONFIG;
//	}
//
//	if (Skill_Sort::BE_SKILL == cfg->skill_sort) {
//		MSG_USER("be skill cannot be used, role_id=%ld, skill_id=%d", role_id(), skill_id);
//		return ERROR_CANNOT_USE_BE_SKILL;
//	}
//
//	int ret = is_additional_skill(*cfg, *skill_detail);
//	if (BTL_NOT_ADD != ret)
//		return ret;
//
//	int check_ret = use_skill_inner_check(skill_id, *skill_detail, cfg->skill_series, now);
//	if (check_ret) return check_ret;
//
//	//检查消耗东西是否满足
//	double fix_morale_cost = get_skill_fix_morale_cost(skill_id);
//	Skill_Cost_Vec cost_vec;
//	if (!fight_cost_check(skill_detail->fight_cost_vec, cost_vec, fix_morale_cost)) {
//		return ERROR_MORALE_LIMIT;
//	}
//
//	simulate_select_targets(*skill_detail);
//
//	if (simulate_targets_.empty()) {
////		MSG_DEBUG("cannot find targets, role_id=%ld, skill_id=%d", role_id(), skill_id);
//		return ERROR_NO_SKILL_TARGET;
//	}
//
//	Time_Value last_sing = Time_Value::zero;
//	if (current_skill_id_ == fighter_detail_->default_skill && skill_step_used_ == 0) {
//		last_sing = now - sing_time_start_;
//		last_sing = last_sing > Time_Value(1) ? Time_Value(1) : last_sing;
//	}
//	current_skill_id_ = skill_id;
//	skill_step_used_ = 0;
//	int64_t usec = static_cast<int64_t>((skill_detail->sing_time.sec() * 1000000 + skill_detail->sing_time.usec()) *
//			(1+fighter_detail_->fetch_fight_property(PT_SING_RATE, Prop_Value::ELEMADD::ELEM_FIGHT)));
//	Time_Value final_sing_time(usec / 1000000, usec % 1000000);
//	final_sing_time = final_sing_time > last_sing ? final_sing_time - last_sing : Time_Value::zero;
//	skill_tick_time_ = now + final_sing_time;
//	sing_time_start_ = now;
//	conflict_time_ = Time_Value::zero;
//	is_singing_end	= false;
//
//	MSG_81000070 msg;
//	msg.attack_id = role_id();
//	msg.skill_id = current_skill_id_;
//	msg.skill_lv = skill->skill_level;
//	msg.errcode = BTL_ERR_SUCCESS;
//	msg.step_time = final_sing_time.sec() * 1000 + final_sing_time.usec() / 1000;
//	uint8_t ef_size = skill_detail->step_effect.size();
//	for (uint8_t i=1 ; i<=ef_size; ++i) {
//		Skill_Effect_Map::const_iterator ef_it = skill_detail->step_effect.find(i);
//		if (skill_detail->step_effect.end() == ef_it) continue;
//		msg.effect_id_vec.push_back(ef_it->second.effect_id);
//	}
//
//	for (Role_Vec::iterator p_id = simulate_targets_.begin();
//			p_id != simulate_targets_.end(); ++p_id) {
//		msg.target_id.push_back(*p_id);
//	}
//
//	Block_Buffer buf;
//	MAKE_MSG_BUF(msg, buf);
//
//	call_broadcast_around(buf);
//
//	fight_cost(cost_vec, 1);
//
//	cost_morale = 0;
//	for (Skill_Cost_Vec::iterator c_it = cost_vec.begin(); c_it != cost_vec.end(); ++c_it) {
//		if (c_it->type != COST_MORALE) continue;
//		cost_morale = fabs(c_it->value);
//	}
//
//	if(battle_scene()){
//		battle_scene()->listen_use_skill(this, skill_id);
//	}
//	return BTL_ERR_SUCCESS;
//}

bool Fighter::check_use_additional_skill(void) {
	if (current_skill_id_ <= 0) return false;
	const Skill_Config * cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (!cfg || cfg->skill_type != Skill_Type::TYPE_FLOAT_SKILL || skill_step_used_ == 0) return false;
	if (skill_step_used_ > current_effect_map_.size()) return false;

	return true;
}

void Fighter::run_all_effect_left(void) {

	uint size = current_effect_map_.size();
	if (skill_step_used_ > size) return;
	Time_Value delta = Time_Value::zero;
	//先执行完该单位当前技能所有效果
	for (uint i = skill_step_used_; i <= size; ++i) {
		Skill_Effect_Map::const_iterator find_it = current_effect_map_.find(i);
		if (find_it == current_effect_map_.end()) {
			MSG_USER("cannot find skill step effect, skill_id=%d, step=%d", current_skill_id_, skill_step_used_);
			continue;
		}
		delta += skill_tick_time_ - Time_Value::gettimeofday();
		skill_tick_time_ = Time_Value::zero;

		inner_skill_action(false);
		// 修改时间
	}
	skill_tick_time_ += delta;
}

//int Fighter::is_additional_skill(const Skill_Config& cfg, const Skill_Detail& detail) {
//
//	if (cfg.skill_type != Skill_Type::TYPE_ADD_SKILL) return BTL_NOT_ADD;
//
//	if (0 != current_skill_id_) {
//		const Skill_Config *cur_cfg_1 = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
//		if (!cur_cfg_1 || cur_cfg_1->skill_type == Skill_Type::TYPE_ADD_SKILL) return BTL_ADD_FAIL;
//	}
//
//	Role_Vec tmp_targets;
//
//	// 查找自己或者同伴是否处于浮空技能中
//	Fighter *fighter = 0;
//	if (mover_detail().is_player()) {
//		fighter = SCENE_PUBLIC->find_battle_hero(role_id());
//	} else if (mover_detail().is_hero()) {
//		fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
//		if (!fighter) return BTL_ADD_FAIL;
//	} else {
//		return BTL_ADD_FAIL;
//	}
//
//	Fighter *floater_user = 0;
//
//	if ( check_use_additional_skill() ) {
//		get_current_step_targets(tmp_targets);
//		floater_user = this;
//	} else {
//		if (!fighter) return BTL_ADD_FAIL;
//		if ( fighter-> check_use_additional_skill() ) {
//			fighter->get_current_step_targets(tmp_targets);
//			floater_user = fighter;
//		} else {
//			return BTL_ADD_FAIL;
//		}
//	}
//
//	if (tmp_targets.size() <= 0) return BTL_ADD_FAIL;
//
//	double fix_morale_cost = get_skill_fix_morale_cost(cfg.skill_id);
//	Skill_Cost_Vec cost_vec;
//	if (!fight_cost_check(detail.fight_cost_vec, cost_vec, fix_morale_cost)) return BTL_ADD_FAIL;
//
//	// 缓存数据
//	cache_skill_tick_time_ = skill_tick_time_;
//	cache_skill_step_used_ = skill_step_used_;
//	cache_current_skill_id_ = current_skill_id_;
//	cache_simulate_targets_ = simulate_targets_;
//	cache_current_effect_map_ = current_effect_map_;
//
//	skill_tick_time_ = Time_Value::zero;
//	skill_step_used_ = 0;
//	current_skill_id_ = cfg.skill_id;
//	simulate_targets_ = tmp_targets;
//	current_effect_map_.clear();
//	confirm_current_skill_effect();
//
//	if (0 != floater_user)
//		floater_user->run_all_effect_left();
//
//	Time_Value fix_time = Time_Value::zero;
//	for (Skill_Effect_Map::const_iterator it = current_effect_map_.begin();
//			it != current_effect_map_.end(); ++it) {
//		fix_time += it->second.continue_time;
//	}
//
//	// 修正所有战斗单位等待时间
////	battle_scene()->fix_all_unit_tick_time(fix_time, role_id());
//	floater_user->fix_skill_step_time(fix_time);
//	cache_skill_tick_time_ += fix_time;
//
//	MSG_81000070 msg;
//	msg.attack_id = role_id();
//	msg.skill_id = current_skill_id_;
//	msg.skill_lv = detail.skill_level;
//	msg.errcode = BTL_ERR_SUCCESS;
//
//	for (Role_Vec::iterator p_it = simulate_targets_.begin();
//			p_it != simulate_targets_.end(); ++p_it) {
//		Mover* mover = battle_scene()->find_mover_with_role_id(*p_it);
//		if (0 == mover) continue;
//		msg.target_id.push_back(mover->role_id());
//	}
//
//	Block_Buffer buf;
//	MAKE_MSG_BUF(msg, buf);
//
//	call_broadcast_around(buf);
////	if (validate_battle_scene()) {
////		battle_scene()->broadcast_around(buf);
////	}
//
//	fight_cost(detail.fight_cost_vec, 1);
//
//	skill_step_used_ = 1;
//	inner_skill_action();
//
//	return BTL_ERR_SUCCESS;
//}

bool Fighter::fight_cost_check(const Skill_Cost_Vec &vec, Skill_Cost_Vec &final_cost, double fix_morale_cost) {
	fight_cost_filter(vec, final_cost, fix_morale_cost);

	for (Skill_Cost_Vec::const_iterator it = final_cost.begin(); it != final_cost.end();
			++it) {
		if (!(it->value < 0)) continue; //加属性(怒气)
		switch (it->type) {
		case COST_MORALE: {
			Fighter *fighter = 0;
			if (mover_detail().is_hero()) {
				fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
			} else {
				fighter = this;
			}
			if (0 == fighter) return false;
			double value = fighter->fighter_detail().current_morale();
			if (it->value < 0 && value < abs(it->value))
				return false;
			break;
		}
		case COST_MORALE_PERCENT: {
			Fighter *fighter = 0;
			if (mover_detail().is_hero()) {
				fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
			} else {
				fighter = this;
			}
			if (0 == fighter) return false;
			if (!(fighter->fighter_detail().current_morale() > 0.0)) return false;
			break;
		}
		case COST_BLD: case COST_BLD_PERCENT: {
			double value = fighter_detail_->current_blood();
			if (!(value > 0)) return false;
			break;
		}
		default:
			break;
		}
	}
	return true;
}

void Fighter::fight_cost_filter(const Skill_Cost_Vec &cost_vec, Skill_Cost_Vec &final_cost, double fix_morale_cost) {
	final_cost.clear();
	Fighter *fighter = 0;
	if (mover_detail().is_hero()) {
		fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
	} else {
		fighter = this;
	}
	if (0 == fighter) {
		final_cost = cost_vec;
		return;
	}
	double cur_morale = fighter->fighter_detail().current_morale();
	double value = 0;
	for (Skill_Cost_Vec::const_iterator it = cost_vec.begin(); it != cost_vec.end(); ++it) {
		if (it->type != COST_MORALE) {
			final_cost.push_back(*it);
					continue;
		}

		if (it->value > 0.0) {
			Skill_Cost skill_cost;
			skill_cost.type = it->type;
			skill_cost.coster = it->coster;
			skill_cost.value = it->value + fix_morale_cost;
			if (skill_cost.value > 0.0) {
				final_cost.push_back(skill_cost);
			}
			continue;
		}

		if (!(value < 0.0)) {
			value = it->value + fix_morale_cost;
			if (value > 0.0) value = 0.0;
		}
		double temp_cost_morale = it->value + fix_morale_cost;
		if (temp_cost_morale > 0.0) temp_cost_morale = 0.0;
		if (fabs(temp_cost_morale) > cur_morale) continue;
		if ((!(fabs(value) > cur_morale)) && (value < temp_cost_morale)) continue;
		value = temp_cost_morale;
	}
	if (value < 0) {
		Skill_Cost cost;
		cost.type = COST_MORALE;
		cost.value = value;
		cost.coster = COST_SELF;
		final_cost.push_back(cost);
	}
}

void Fighter::fight_cost(const Skill_Cost_Vec &vec, int stage) {
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	for (Skill_Cost_Vec::const_iterator it = vec.begin(); it != vec.end();
			++it) {
		if (it->value > 0 || !(it->value>0 || it->value<0))
			continue; //加属性(怒气)

		switch (it->type) {
		case COST_MORALE: {
			if (stage != 1) break;
			Fighter *fighter = 0;
			if (mover_detail().is_hero()) {
				fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
			} else {
				fighter = this;
			}
			if (0 == fighter) break;
			setter.reset();
			setter.prop_type = PT_MORALE_CURRENT;
			setter.addi_type = AT_BASIC;
			setter.basic = it->value;
			fight_setter.push_back(setter);
			MSG_81000060 msg;
			msg.role_id = fighter->role_id();
			fighter->fight_modify_props(fight_setter, msg.change_info);
			if (msg.change_info.size() > 0) {
				OBJ_SEND_TO_CLIENT(msg, (*fighter));
			}
			break;
		}
		case COST_MORALE_PERCENT: {
			if (stage != 1) break;
			Fighter *fighter = 0;
			if (mover_detail().is_hero()) {
				fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
			} else {
				fighter = this;
			}
			if (0 == fighter) break;
			setter.reset();
			double cost = (double)(fighter->fighter_detail().current_morale() * it->value);
			setter.prop_type = PT_MORALE_CURRENT;
			//setter.operate = it->value > 0.0 ? O_ADD : O_SUB;
			setter.addi_type = AT_BASIC;
			setter.basic = cost; //cost > 1.0 ? cost : 1.0;
			fight_setter.push_back(setter);
			MSG_81000060 msg;
			msg.role_id = fighter->role_id();
			//fight_modify_fight_prop(fight_setter, msg.change_info);
			fighter->fight_modify_props(fight_setter, msg.change_info);
			if (msg.change_info.size() > 0) {
				OBJ_SEND_TO_CLIENT(msg, (*fighter));
			}
			break;
		}
		case COST_BLD: {
			if (stage != 2) break;
			double cost = fabs(it->value);
			double max_cost = fighter_detail_->current_blood();
			max_cost = ceil(max_cost > 1.0 ? max_cost - 1.0 : 0.0);
			cost = cost < max_cost ? cost : max_cost;
			if (max_cost < 1.0) break;
 			Attack_Param param;
			param.attacker = role_id();
			param.victim = role_id();
			param.attack_result = ATTACK_BLD_COST;
			param.attack_hurt = ceil(cost);
			actual_sub_blood(param);
			break;
		}
		case COST_BLD_PERCENT: {
			if (stage != 2) break;
			double cost = fighter_detail_->current_blood();
			if (!(cost > 1.0)) break;
			cost = ceil(fabs(cost * it->value));
			if (cost < 1.0) break;
			Attack_Param param;
			param.attacker = role_id();
			param.victim = role_id();
			param.attack_result = ATTACK_BLD_COST;
			param.attack_hurt = cost;
			actual_sub_blood(param);
			break;
		}
		default:
			if (stage != 2) break;
			if (it->type > 1000) {
				if (it->coster == COST_SELF) {
					del_status_overlay(it->type, static_cast<int>(fabs(it->value)));
				}
			}
			break;
		}
	}
}

void Fighter::handle_crit_add_moral(const Skill_Step_Fix_Vec & step_fix, double be_skill_moral, Attack_Result result, int skill_id) {
	if (Attack_Result::ATTACK_CRIT != result) return;
	double value = 0.0;
	for (Skill_Step_Fix_Vec::const_iterator it = step_fix.begin();
			it != step_fix.end(); ++it) {
		if (it->id != TYPE_CRIT_MORAL) continue;
		value += it->val2;
	}
	value += be_skill_moral;
	if (value > 0.0) {
		Skill_Cost cost;
		cost.type = COST_MORALE;
		cost.coster = COST_SELF;
		cost.value = value;
		Skill_Cost_Vec vec;
		vec.push_back(cost);
		fight_add(vec, TYPE_SKILL_CRIT_ADD, skill_id);
	}
}

void Fighter::Filter_crit_add_moral_Fix(const Skill_Step_Fix_Vec &in, Skill_Step_Fix_Vec &out) {
	for (Skill_Step_Fix_Vec::const_iterator it = in.begin(); it != in.end(); ++it) {
		if (it->id != TYPE_CRIT_MORAL) continue;
		out.push_back(*it);
	}
}

void Fighter::fight_add(const Skill_Cost_Vec &vec, int add_type, int skill_id) {

	Fighter *fighter = 0;
	if (mover_detail().is_hero()) {
		fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
	} else {
		fighter = this;
	}
	if (0 == fighter) return;

	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	for (Skill_Cost_Vec::const_iterator it = vec.begin(); it != vec.end();
			++it) {
		if (it->value < 0 || !(it->value>0 || it->value<0))
			continue; //加属性(怒气)

		switch (it->type) {
		case Battle_Cost_Type::COST_MORALE: {
			double curMor = fighter->fighter_detail().current_morale();
			double maxMor = fighter->fighter_detail().max_morale();
			setter.reset();
			setter.prop_type = PT_MORALE_CURRENT;
			setter.operate = it->value > 0.0 ? O_ADD : O_SUB;
			setter.addi_type = AT_BASIC;
			setter.basic =
					((curMor + abs(it->value)) <= maxMor) ?
							abs(it->value) : (maxMor - curMor);
			fight_setter.push_back(setter);
			break;
		}
		default:
			break;
		}
	}
	MSG_81000060 msg;
	msg.role_id = fighter->role_id();
	msg.source_id = skill_id;
	msg.result = add_type;

	fighter->fight_modify_fight_prop(fight_setter, msg.change_info);

	if (msg.change_info.empty()) return ;

	be_skill_trigger_active_skill(*this, TYPE_MORALE_VAL);

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	fighter->send_to_client(buf);
}

void Fighter::handle_inner_call_skill_action(Role_Vec roles, int skill_id, int skill_lv) {
	const Skill_Detail* skill_detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_lv);
	if (0 == skill_detail) {
		MSG_USER("cannot find config data, skill_id=%d, skill_lv=%d", skill_id, skill_lv);
		return;
	}
	Skill_Effect_Map::const_iterator find_it = skill_detail->step_effect.find(1);
	if (find_it == skill_detail->step_effect.end()) return;

	Fighter_Vec fighter_vec;
	fighter_vec.clear();
	switch(find_it->second.target_sellect_info.select_type) {
	case Target_Select_Type::TARGET_BUFF: {
		if (roles.size() <= 0) {
			MSG_USER("no role id exist, size=%d", roles.size());
			return;
		}
		for (Role_Vec::iterator it = roles.begin(); it != roles.end(); ++it)  {
			Fighter *target = battle_scene() ->find_mover_with_role_id(*it);
			if (0 == target || Battle_Status::BATTLING != target->get_battle_status()) continue;
			//if (target->dead_status()) continue;
			fighter_vec.push_back(target);
		}
		break;
	}
	case Target_Select_Type::TARGET_AUTO: {
		Skill_Step_Fix_Vec step_fix;
		check_and_get_step_fix(find_it->second.step_fix_type,
				find_it->second.skill_step_fix_vec, step_fix);

		get_all_be_skill_step_fix(step_fix, find_it->second.effectgroup_id);

		int amount = find_it->second.target_sellect_info.target_amount;

		for (Skill_Step_Fix_Vec::iterator it = step_fix.begin(); it != step_fix.end(); ++it) {
			if (it->id == TYPE_TARGET_NUM && it->val1 != 0) {
				amount = it->val1;
			}
		}

		battle_scene()->find_fighter_enemy(*this, fighter_vec, find_it->second.target_sellect_info, amount);
		break;
	}
	default:
		break;
	}

	for (Fighter_Vec::iterator f_it = fighter_vec.begin(); f_it != fighter_vec.end(); ++f_it) {
		int size = skill_detail->step_effect.size();
		for (int index = 1; index <= size; ++index) {
			inner_call_skill_action(*f_it, *skill_detail, index);
		}
	}
}

void Fighter::inner_call_skill_action(Fighter* target, const Skill_Detail & detail, int step) {

	if (0 == target) return;
	Skill_Effect_Map::const_iterator find_it = detail.step_effect.find(step);
	if (find_it == detail.step_effect.end()) return;

	// 中间不能直接返回
	uint temp_old_step_used = skill_step_used_;
	skill_step_used_ = static_cast<uint>(step);
	int temp_old_skill_id = current_skill_id_;
	current_skill_id_ = detail.skill_id;
	Skill_Effect_Map temp_current_effect_map;
	temp_current_effect_map.insert(current_effect_map_.begin(),
			current_effect_map_.end());
	current_effect_map_.clear();
	current_effect_map_.insert(detail.step_effect.begin(), detail.step_effect.end());

	Fighter_Vec vec;
	vec.push_back(target);

	Skill_Running_Info info;
	info.skill_id = detail.skill_id;
	info.skill_level = detail.skill_level;

	Skill_Step_Fix_Vec step_fix;
	check_and_get_step_fix(find_it->second.step_fix_type,
			find_it->second.skill_step_fix_vec, step_fix);

	get_all_be_skill_step_fix(step_fix, find_it->second.effectgroup_id);


	Fighter_Vec fighter_vec;
	fighter_vec.push_back(target);
	get_step_fix_by_target(find_it->second, step_fix, fighter_vec);

	Skill_Effect_Correct crt_vec;
	total_skill_correct(find_it->second.effect_correct, crt_vec, step_fix);

	skill_ps_attr(crt_vec);

	Skill_Step_Fix_Vec crit_add_moral_step;
	Filter_crit_add_moral_Fix(step_fix, crit_add_moral_step);

	skill_action_effect(detail, find_it->second, info, vec, crit_add_moral_step);

	skill_recover_attr(crt_vec);

	// 中间不能直接返回
	skill_step_used_ = temp_old_step_used;
	current_skill_id_ =  temp_old_skill_id;
	current_effect_map_.clear();
	current_effect_map_.insert(temp_current_effect_map.begin(),
			temp_current_effect_map.end());
}

int Fighter::inner_skill_action(bool normal) {

	// 主角追加，只有在追加成功情况下才会不跳过追加效果
	skip_addition_skill(normal);

	Skill_DB_Info* skill = get_skill_db_info(current_skill_id_);
	if (skill == 0) return ERROR_INNER_SERVER;
	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (NULL == cfg) {
		MSG_USER("skill:%d", current_skill_id_);
		current_skill_id_ = 0;
		skill_step_used_ = 0;
		return ERROR_CONFIG_NOT_EXIST;
	}

	// get running info
	Skill_Running_Info_Map::iterator skill_running_find_it =
			fighter_detail_->skill_running_info_map.find(current_skill_id_);
	if (skill_running_find_it == fighter_detail_->skill_running_info_map.end()) {
		Skill_Running_Info info;
		info.skill_id 				= current_skill_id_;
		info.skill_level 		= skill->skill_level;
		info.last_step			= 0;
		info.config_cache 	= cfg;
		skill_running_find_it = fighter_detail_->skill_running_info_map.insert(
				std::make_pair(info.skill_id, info)).first;
	}

	// get skill detail
	Skill_Detail_Map::const_iterator skill_detail_find_it =
			skill_running_find_it->second.config_cache->skill_level_map.find(skill->skill_level);
	if (skill_detail_find_it == skill_running_find_it->second.config_cache->skill_level_map.end()) {
		MSG_USER("skill:%d level:%d", current_skill_id_, skill->skill_level);
		current_skill_id_ = 0;
		skill_step_used_ = 0;
		return ERROR_INNER_SERVER;
	}

	// 判断技能结束退出
	if (is_all_effect_end(skill_detail_find_it->second)) return 0;

	// get effect info
	Skill_Effect_Map::const_iterator skill_effect_find_it = current_effect_map_.find(skill_step_used_);
	if (skill_effect_find_it == current_effect_map_.end()) {
		MSG_USER("skill:%d level:%d step:%d", current_skill_id_, skill->skill_level, skill_step_used_);
		current_skill_id_ = 0;
		skill_step_used_ = 0;
		return ERROR_INNER_SERVER;
	}

	Skill_Step_Fix_Vec step_fix;
	check_and_get_step_fix(skill_effect_find_it->second.step_fix_type,
			skill_effect_find_it->second.skill_step_fix_vec, step_fix);
	get_all_be_skill_step_fix(step_fix, skill_effect_find_it->second.effectgroup_id);

	Fighter_Vec fighter_vec;
	select_target(skill_effect_find_it->second, skill_running_find_it->second, fighter_vec, step_fix);

	if (1 == skill_step_used_) {
		Status::fight_ation_tick(skill_running_find_it->second.step_target_map[skill_step_used_]);
	}

	get_step_fix_by_target(skill_effect_find_it->second, step_fix, fighter_vec);

	//修改技能附加属性
	Skill_Effect_Correct crt_vec;
	total_skill_correct(skill_effect_find_it->second.effect_correct, crt_vec, step_fix);
	skill_ps_attr(crt_vec);

	Skill_Step_Fix_Vec crit_add_moral_step;
	Filter_crit_add_moral_Fix(step_fix, crit_add_moral_step);

	skill_action_effect(skill_detail_find_it->second, skill_effect_find_it->second,
			skill_running_find_it->second, fighter_vec, crit_add_moral_step);

	//恢复原来属性
	skill_recover_attr(crt_vec);

	last_skill_step_used_ = skill_step_used_;
	skill_step_used_++;
	// 最后一个效果时，处理技能加怒气(Q技能)
	if (skill_step_used_ > current_effect_map_.size()) {
		fight_add(skill_detail_find_it->second.fight_cost_vec, TYPE_SKILL_END_ADD, current_skill_id_);
	}

	// 未释放完到下个效果时间
	skill_tick_time_ = Time_Value::gettimeofday() + skill_effect_find_it->second.continue_time;

	// 追加技能AI
	if (2 == skill_step_used_ && 0 != player_monster_self()) {
		player_monster_self()->floating_skill_trigger(*cfg);
	}

	return 0;
}

bool Fighter::is_all_effect_end(const Skill_Detail& detail) {

	if (skill_step_used_ <= current_effect_map_.size()) return false;
//	MSG_DEBUG("skill id zeor role:%ld", role_id(), this->battle_scene_id());

	if(dead_status()) {
		// 死亡即清掉buff
		Status::clear_all_fighter_status();
		this->broadcast_die_info();
	}
	if (battle_scene())
		battle_scene()->set_check_over_state(*this);

	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(detail.skill_id);
	//是否追加技结束, 恢复现场
	if (0 != cfg && cfg->skill_type == Skill_Type::TYPE_ADD_SKILL) {
		skill_tick_time_ 				= cache_skill_tick_time_;
		cache_skill_tick_time_ 	= Time_Value::zero;
		skill_step_used_ 			= cache_skill_step_used_;
		cache_skill_step_used_ 	= 0;
		current_skill_id_ 			= cache_current_skill_id_;
		cache_current_skill_id_ 	= 0;
		simulate_targets_ 			= cache_simulate_targets_;
		current_effect_map_ 		= cache_current_effect_map_;
		cache_simulate_targets_.clear();
		cache_current_effect_map_.clear();

		////检查是否所有对象已死
		if (all_unit_dead(simulate_targets_)) simulate_targets_.clear();
		if (0 != pre_used_skill_id_) use_skill();

		return true;
	}

	// 退出当前技能， 进入使用默认技能（普攻）
	if(battle_scene()){// 使用玩技能算是一个回合结束
		battle_scene()->listen_fight_round(this);
	}
	skill_step_used_ 			= 0;
	last_skill_step_used_ 	= 0;
	current_skill_id_ 			= 0;
	current_effect_map_.clear();

	use_skill(ai_select_skill());

	return true;
}

void Fighter::handle_buff(const Skill_Effect_Val & val1, Fighter_Vec &fighter_vec,
		role_id_t  caster_id, int skill_id, Statue_Source_Type type) {

	if (0 == val1.id) return;
	const Btl_Buff_Cfg *cfg = CONFIG_CACHE_BUFF->btl_buff(val1.id);
	if (!cfg || val1.inner_vec.size() != cfg->effect_map.size()) return;
	Status_Effect_Argv argv;

	for (Fighter_Vec::iterator it_fighter = fighter_vec.begin();
			it_fighter != fighter_vec.end(); ++it_fighter) {

		if (0 == (*it_fighter) || (*it_fighter)->dead_status()) continue;
		int ext_time = get_fix_buff_time(val1.id);
		argv.caster_id = caster_id;
		argv.status_id = val1.id;
		argv.disappear_gap = val1.data1;
		argv.overlay = 1;
		argv.status_from = type;
		argv.other_id = skill_id;

		int i = 0;
		argv.data.clear();
		bool has_dizz = false;
		for (Buff_Effect_Vec:: const_iterator it = cfg->effect_map.begin(); it != cfg->effect_map.end(); ++it) {
			Effect_Argv_Prop prop;
			prop.i_effect_val1 = static_cast<Property_Type>(val1.inner_vec[i].val1);
			prop.d_effect_val1 = val1.inner_vec[i].val4;
			prop.d_effect_val2 = val1.inner_vec[i].val5;
			prop.d_effect_val3 = val1.inner_vec[i].val6;
			prop.d_effect_val4 = val1.inner_vec[i].val7;
			if (it->buff_effect_type == Status_Def::STATUS_ET_POISON_BY_EXPRESSION) {
				prop.d_effect_val5 = get_pointion_buff_atk(prop.i_effect_val1) * prop.d_effect_val1;
				prop.d_effect_val6 = get_pointion_buff_atk_hurt_rate(prop.i_effect_val1);
				prop.d_effect_val7 = get_pointion_buff_hurt_rate();
				prop.d_effect_val8 = get_pointion_buff_pk_hurt();
				prop.d_effect_val9 = get_pointion_buff_race_hurt(**it_fighter);
			} else if (it->buff_effect_type == Status_Def::STATUS_EFFECT_TYPE_DIZZ) {
				has_dizz = true;
			}
			argv.data.push_back(prop);
			++i;
		}

		if (has_dizz) {
			ext_time *= (1 + fighter_detail().fetch_fight_property(PT_DIZZ_STRENGTHEN)
					 - (*it_fighter)->fighter_detail().fetch_fight_property(PT_DIZZ_WEAKEN));
		}

		argv.add_last_time_ms(ext_time);
		(*it_fighter)->insert_status(argv);
	}
}

void Fighter::skill_ps_attr(const Skill_Effect_Correct &crt) {
	if (crt.attr_crr_vec.size() <= 0) return;
	Prop_Setter_Vec prop_vec;
	prop_vec.clear();
	Prop_Change_Vec change_info;
	change_info.clear();
	Prop_Setter prop_setter;
	for (Attr_Correct_Vec::const_iterator acv_it = crt.attr_crr_vec.begin();
			acv_it != crt.attr_crr_vec.end(); ++acv_it) {
		prop_setter.reset();
		prop_setter.prop_type = static_cast<Property_Type>(acv_it->type);
		prop_setter.fixed = acv_it->fixed_value;
		prop_setter.percent = acv_it->percent_value;
		prop_setter.addi_type = Addition_Type::AT_INITIATIVE_SKILL;
		prop_vec.push_back(prop_setter);
	}
	fight_modify_props(prop_vec, change_info);
}

void Fighter::skill_recover_attr(const Skill_Effect_Correct& crt) {
	if (crt.attr_crr_vec.size() <= 0) return;
	Prop_Setter_Vec prop_vec;
	prop_vec.clear();
	Prop_Change_Vec change_info;
	change_info.clear();
	Prop_Setter prop_setter;
	for (Attr_Correct_Vec::const_iterator acv_it = crt.attr_crr_vec.begin();
			acv_it != crt.attr_crr_vec.end(); ++acv_it) {
		prop_setter.reset();
		prop_setter.prop_type = static_cast<Property_Type>(acv_it->type);
		prop_setter.fixed = -acv_it->fixed_value;
		prop_setter.percent = -acv_it->percent_value;
		prop_setter.addi_type = Addition_Type::AT_INITIATIVE_SKILL;
		prop_vec.push_back(prop_setter);
	}
	fight_modify_props(prop_vec, change_info);
}

void Fighter::simulate_select_targets(const Skill_Detail& skill_detail) {
	simulate_targets_.clear();
	if (skill_detail.step_effect.size() <= 0) return;

	Skill_Effect_Map::const_iterator find_it = skill_detail.step_effect.find(1);
	if (find_it == skill_detail.step_effect.end()) return;
	if (find_it->second.target_sellect_info.select_type != TARGET_AUTO) return;

	Skill_Step_Fix_Vec step_fix;
	check_and_get_step_fix(find_it->second.step_fix_type,
			find_it->second.skill_step_fix_vec, step_fix);
	get_all_be_skill_step_fix(step_fix, find_it->second.effectgroup_id);

	int amount = find_it->second.target_sellect_info.target_amount;

	for (Skill_Step_Fix_Vec::const_iterator it = step_fix.begin(); it != step_fix.end(); ++it) {
		if (it->id == TYPE_TARGET_NUM && it->val1 != 0) {
			amount = it->val1;
		}
	}

	Fighter_Vec fighter_vec;
	battle_scene()->find_fighter_enemy(*this, fighter_vec, find_it->second.target_sellect_info, amount);
	for (Fighter_Vec::iterator it = fighter_vec.begin(); it != fighter_vec.end(); ++it) {
		simulate_targets_.push_back((*it)->role_id());
	}
}

void Fighter::add_floating_target(Skill_Running_Info &skill_running, Fighter_Vec &fighter_vec, Role_Vec& role_vec) {
	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (0 == cfg || TYPE_FLOAT_SKILL != cfg->skill_type) return;
	if (skill_step_used_ <= 1) return;
	if (!validate_battle_scene()) return;

	Role_Vec& vec = skill_running.step_target_map[1];
	for (Role_Vec::iterator it = vec.begin(); it != vec.end(); ++it) {
		Mover *mover = battle_scene()->find_mover_with_role_id(*it);
		if (0 != mover && 0 != mover->fighter() && mover->fighter()->dead_status()) {
			Role_Vec::iterator exist_it = std::find(role_vec.begin(), role_vec.end(), mover->role_id());
			if (exist_it != role_vec.end()) continue;
			fighter_vec.push_back(mover->fighter());
			role_vec.push_back(mover->role_id());
		}
	}
}

int Fighter::select_target(const Skill_Effect &effect,
		Skill_Running_Info &skill_running, Fighter_Vec &fighter_vec, const Skill_Step_Fix_Vec &step_fix) {

	int amount = effect.target_sellect_info.target_amount;

	for (Skill_Step_Fix_Vec::const_iterator it = step_fix.begin(); it != step_fix.end(); ++it) {
		if (it->id == TYPE_TARGET_NUM && it->val1 != 0) {
			amount = it->val1;
		}
	}

	if (!validate_battle_scene()) {
		simulate_targets_.clear();
		return 0;
	}

	if (1 == skill_step_used_ && simulate_targets_.size() > 0) {
		skill_running.step_target_map[skill_step_used_].clear();
		Role_Vec::iterator select_it = simulate_targets_.begin();
		for (int i=0; i<=amount; ++i) {
			if (select_it == simulate_targets_.end()) break;
			role_id_t id = *select_it;
			++select_it;
			Fighter* fgt = battle_scene()->find_mover_with_role_id(id);
			if (0 == fgt) continue;
			skill_running.step_target_map[skill_step_used_].push_back(id);
			fighter_vec.push_back(fgt);
		}
		simulate_targets_.clear();
		if (fighter_vec.size() <= 0)
			select_target(effect, skill_running, fighter_vec, step_fix);
		return 0;
	}

	switch (effect.target_sellect_info.select_type) {
	case TARGET_EXIST: {
		for (Role_Vec::iterator it = skill_running.step_target_map[last_skill_step_used_].begin();
				it != skill_running.step_target_map[last_skill_step_used_].end(); ++it) {
			Mover *mover = battle_scene()->find_mover_with_role_id(*it);
			if (0 != mover && 0 != mover->fighter())
				fighter_vec.push_back(mover->fighter());
		}
		skill_running.step_target_map[skill_step_used_] =
				skill_running.step_target_map[last_skill_step_used_];
		break;
	}
	case TARGET_SELF: {
		break;
	}
	case TARGET_AUTO: {
		battle_scene()->find_fighter_enemy(*this, fighter_vec, effect.target_sellect_info, amount);
		Role_Vec role_set;

		// 按规则成功选择
		if (fighter_vec.size() > 0) {
			for (Fighter_Vec::iterator it = fighter_vec.begin();
					it != fighter_vec.end(); ++it) {
				role_set.push_back((*it)->role_id());
			}
//			add_floating_target(skill_running, fighter_vec, role_set);
			skill_running.step_target_map[skill_step_used_] = role_set;
			break;
		}

		if (EFFECT_PLAY_EFFECT != effect.effect_type && EFFECT_ATTACK_DEFAULT != effect.effect_type)
			break;
		if (skill_step_used_ <= 1) break;
		if (role_set.size() > 0) break;
		if (!validate_battle_scene()) break;
		if (!battle_scene()->check_all_enemy_dead(*this)) break;
		for (uint i = skill_step_used_-1; i>0; --i) {
			Skill_Effect_Map::iterator find_it = current_effect_map_.find(i);
			if (find_it == current_effect_map_.end()) continue;
			if (EFFECT_PLAY_EFFECT != find_it ->second.effect_type) continue;
			Role_Vec dead_vec = skill_running.step_target_map[i];
			if (static_cast<uint>(amount) >= dead_vec.size()) {
				role_set = dead_vec;
			} else {
				role_set = Role_Vec(dead_vec.begin(), dead_vec.begin() + amount);
			}
			for (Role_Vec::iterator r_it = role_set.begin(); r_it != role_set.end(); ++r_it) {
				Fighter* fgt = battle_scene()->find_mover_with_role_id(*r_it);
				if (0 == fgt) continue;
				fighter_vec.push_back(fgt);
			}
			break;
		}
//		add_floating_target(skill_running, fighter_vec, role_set);
		skill_running.step_target_map[skill_step_used_] = role_set;
		break;
	}
	case TARGET_TEMP: {
		skill_running.step_target_map[skill_step_used_] =
				skill_running.step_target_map[last_skill_step_used_];
		Role_Vec &ef_target_vec = skill_running.step_target_map[skill_step_used_];
		int extra_amount = ef_target_vec.size();
		Fighter_Vec temp_vec;
		battle_scene()->find_fighter_enemy(*this, temp_vec, effect.target_sellect_info, amount + extra_amount);
		int index = 0;
		for (Fighter_Vec::iterator it = temp_vec.begin(); it != temp_vec.end(); ++it) {
			if (index >= amount) break;
			Role_Vec::iterator find_it = std::find(ef_target_vec.begin(), ef_target_vec.end(), (*it)->role_id());
			if (find_it != ef_target_vec.end()) continue;
			fighter_vec.push_back(*it);
			++index;
		}
		break;
	}
	default: {
		MSG_USER("skill:%d error select type:%d", current_skill_id_,
				effect.target_sellect_info.select_type);
		return ERROR_CONFIG_NOT_EXIST;
		break;
	}
	}
	return 0;
}

int Fighter::skill_action_effect(const Skill_Detail &detail,
		const Skill_Effect &effect, Skill_Running_Info &skill_running,
		Fighter_Vec &fighter_vec, const Skill_Step_Fix_Vec& skill_fix) {
	switch (effect.effect_type) {
	case EFFECT_PLAY_EFFECT: {
		MSG_81000070 msg;
		msg.step = skill_step_used_;
		msg.attack_id = role_id();
		msg.skill_id = skill_running.skill_id;
		msg.skill_lv = skill_running.skill_level;
		msg.effect_id_vec.clear();
		uint size = current_effect_map_.size();
		bool slice = false;
		for (uint i = skill_step_used_; i <= size; ++i) {
			Skill_Effect_Map::iterator find_it = current_effect_map_.find(i);
			if (find_it == current_effect_map_.end()) continue;
			if (EFFECT_PLAY_EFFECT == find_it->second.effect_type && slice) break;
			if (EFFECT_FLOAT_OVER == find_it->second.effect_type)
				break;
			if (EFFECT_PLAY_EFFECT == find_it->second.effect_type && !slice) {
				slice = true;
			}
			msg.effect_id_vec.push_back(find_it->second.effect_id);
		}
		for (Fighter_Vec::iterator it_fighter = fighter_vec.begin();
				it_fighter != fighter_vec.end(); ++it_fighter) {
			msg.target_id.push_back((*it_fighter)->role_id());
		}

		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		call_broadcast_around(buf);

		slice_floating_skill();

		break;
	}
	case EFFECT_ATTACK_DEFAULT: {
		for (Fighter_Vec::iterator it_fighter = fighter_vec.begin();
				it_fighter != fighter_vec.end(); ++it_fighter) {
			Attack_Param attack_param(skill_running.skill_id, role_id(),
					(*it_fighter)->role_id(), &effect);
			skill_attack(attack_param, *this, **it_fighter,
					get_attack_by_default, skill_fix);
		}
		break;
	}
	case EFFECT_ATTACK_BUFF_SELF: {
		Fighter_Vec vec;
		vec.push_back(this);
		handle_buff(effect.val1, vec, role_id(), detail.skill_id, Statue_Source_Type::TYPE_SOURCE_SKILL);
		break;
	}
	case EFFECT_ATTACK_BUFF_OPP: {
		handle_buff(effect.val1, fighter_vec, role_id(), detail.skill_id,
				Statue_Source_Type::TYPE_SOURCE_SKILL);
		break;
	}
	case EFFECT_CURE: {
		for (Fighter_Vec::iterator it_fighter = fighter_vec.begin();
				it_fighter != fighter_vec.end(); ++it_fighter) {
			if (0 == (*it_fighter) || (*it_fighter)->dead_status()) continue;
			skill_cure(*this, **it_fighter, effect.hp_cure.id, effect.hp_cure.value, skill_running.skill_id);
		}
		break;
	}
	case EFFECT_CALL_MONSTER: {
		if (0 == effect.call_monster.monster_type_id) return 0;
		call_monster_partner(effect.call_monster.monster_type_id, effect.call_monster.idx,
				effect.call_monster.fix_monster_vec, skill_running);
		break;
	}
	case EFFECT_PURE: {
		// pure self's buff
		for (Fighter_Vec::iterator it_fighter = fighter_vec.begin();
				it_fighter != fighter_vec.end(); ++it_fighter) {
			if (0 == (*it_fighter) || (*it_fighter)->dead_status()) continue;
			(*it_fighter)->handle_pure();
		}
		break;
	}
	case EFFECT_DISPERSE: {
		for (Fighter_Vec::iterator it_fighter = fighter_vec.begin();
				it_fighter != fighter_vec.end(); ++it_fighter) {
			if (0 == (*it_fighter) || (*it_fighter)->dead_status()) continue;
			// disperse enemy's buff
			(*it_fighter)->handle_dispel();
		}
		break;
	}
	case EFFECT_SUB_ENEMY_MORALE: {
		if (effect.morale_damage.id != COST_MORALE ||
				effect.morale_damage.id != COST_MORALE_PERCENT) return 0;
		Skill_Cost cost;
		cost.type = effect.morale_damage.id;
		cost.coster = COST_SELF;
		cost.value = effect.morale_damage.value;
		Skill_Cost_Vec vec;
		vec.push_back(cost);
		for (Fighter_Vec::iterator it = fighter_vec.begin();
				it != fighter_vec.end(); ++it) {
			if (0 == (*it) || (*it)->dead_status()) continue;
			// cost enemy's morale
			(*it)->fight_cost(vec, 1);
		}
		break;
	}
	case EFFECT_T_CALL_MONSTER: {
		transfer_call_monster(effect, skill_running);
		break;
	}
	case EFFECT_DESPERSE_ONE: {
		for (Fighter_Vec::iterator it = fighter_vec.begin(); it != fighter_vec.end(); ++it) {
			if (0 == *it || (*it)->dead_status()) continue;
			(*it)->erase_evil_status_by_rand(1);
		}
		break;
	}
	case EFFECT_BUFF_SWITCH: {
		handle_buff_on_off(effect, detail.skill_id);
		break;
	}
	case EFFECT_FLOAT_OVER: {
		for (Fighter_Vec::iterator it = fighter_vec.begin(); it != fighter_vec.end(); ++it) {
			if (0 == *it) continue;
			(*it)->check_dead_and_broadcast(**it, role_id());
		}
		break;
	}
	default :{
		break;
	}
	}
	return 0;
}

int Fighter::skill_cure(Fighter &attacker, Fighter &peer, int type, double value, int skill_id) {
	double mate_blood = peer.fighter_detail().fetch_fight_property(PT_BLOOD_CURRENT, Prop_Value::ELEM_FIGHT);
	if (mate_blood > 0 ) {
		double cure_blood =get_cure_by_default(attacker, peer, type, value);
		cure_blood = ceil(cure_blood);
		cure_blood = cure_blood < 1.0 ? 1.0 : cure_blood;
		peer.actual_cure(attacker, cure_blood, skill_id);
	}

	return 0;
}

void Fighter::actual_cure(Fighter &attacker, double cure_blood, int skill_id) {
	if (dead_status()) return;
	if (!(cure_blood > 0.0)) return;

	cure_blood = ceil(cure_blood);
	MSG_81000060 msg;
	msg.role_id = role_id();
	msg.attack_id = attacker.role_id();
	msg.result = Attack_Result::ATTACK_CURE;
	msg.source_id = skill_id;

	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = PT_BLOOD_CURRENT;
	setter.operate = O_ADD;
	setter.addi_type = AT_BASIC;
	setter.basic = cure_blood;
	fight_setter.push_back(setter);
//	fight_modify_fight_prop(fight_setter, msg, true);
	fight_modify_fight_prop(fight_setter, msg.change_info);

	bool is_cured = false;
	for (std::vector<Prop_Change>::iterator it = msg.change_info.begin();
			it != msg.change_info.end(); ++it) {
		if (it->prop_type == PT_BLOOD_CURRENT) {
			it->change_val = cure_blood;
			is_cured = true;
		}
	}
	if (!is_cured) {
		Prop_Change change;
		change.prop_type = PT_BLOOD_CURRENT;
		change.current_val = blood_current();
		change.change_val = cure_blood;
		msg.change_info.push_back(change);
	}

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	scene_broad_around(buf);

	attacker.statistic_battle_cure(cure_blood);

	return;
}

int Fighter::skill_attack(Attack_Param &attack_param, Fighter &attacker,
		Fighter &peer, Attack_Calc_Func func, const Skill_Step_Fix_Vec& skill_fix) {
	Attack_Result attack_result = select_attack_result(attacker, peer);
	switch (attack_result) {
	case ATTACK_AVOID: {
		attack_param.attack_hurt = 0.0;
		break;
	}
	case ATTACK_CRIT: {
		func(attack_param, *this, peer, attack_param.attack_hurt);
		double rate = attacker.fighter_detail().fetch_fight_property(PT_CRIT_HURT, Prop_Value::ELEM_FIGHT);
		rate += peer.fighter_detail().fetch_fight_property(PT_CRIT_HURT_SUB, Prop_Value::ELEM_FIGHT);
		attack_param.attack_hurt *= rate;
		// 至少1点伤害
		attack_param.attack_hurt =
				attack_param.attack_hurt < 1.0 ? 1.0 : attack_param.attack_hurt;

		break;
	}
	case ATTACK_NORMAL: {
		func(attack_param, *this, peer, attack_param.attack_hurt);
		// 至少1点伤害
		attack_param.attack_hurt =
				attack_param.attack_hurt < 1.0 ? 1.0 : attack_param.attack_hurt;

		break;
	}
	default:
		break;
	}
	attack_param.attack_result = attack_result;

	last_skill_use_timestamp_ = Time_Value::gettimeofday();

	double actual_hurt = peer.hurt_by(attack_param);

	double blood_steal = attacker.fighter_detail().fetch_fight_property(Property_Type::PT_BLOOD_STEAL,
			Prop_Value::ELEMADD::ELEM_FIGHT);

	if (blood_steal > 0) {
		double cure_rate = 1;
		cure_rate += attacker.fighter_detail().fetch_fight_property(PT_CURE_RATE,
				Prop_Value::ELEMADD::ELEM_FIGHT);
		cure_rate += attacker.fighter_detail().fetch_fight_property(PT_BE_CURE_RATE,
				Prop_Value::ELEMADD::ELEM_FIGHT);
		double cured_blood = actual_hurt * blood_steal * cure_rate;
		attacker.actual_cure(attacker, cured_blood, attack_param.skill_id);
	}

	double moral_at_crit = get_be_skill_add_moral(static_cast<Attack_Result>(attack_param.attack_result), ADD_TYPE_CRIT);
	double moral_at_hit = get_be_skill_add_moral(static_cast<Attack_Result>(attack_param.attack_result), ADD_TYPE_HIT);

	handle_crit_add_moral(skill_fix, moral_at_crit, static_cast<Attack_Result>(attack_param.attack_result),
			attack_param.skill_id);
	add_moral_every_hit(moral_at_hit, static_cast<Attack_Result>(attack_param.attack_result),
			attack_param.skill_id);

	if (validate_battle_scene() && actual_hurt > 0.0) {
		battle_scene()->total_hurt_statistic(attacker.mover_detail().battle_position.pos,
				actual_hurt, Time_Value::gettimeofday());
	}

	return 0;
}

double Fighter::hurt_by(Attack_Param &attack_param) {

	attack_param.attack_hurt = handle_shield(attack_param.attack_hurt, attack_param.attacker);

	last_be_attack_timestamp_ = Time_Value::gettimeofday();

	Fighter *attacker = find_fighter_in_battle_scene(attack_param.attacker);

	// 攻击时触发
	if (attacker) {
		attacker->passive_tigger_attack(attack_param);
	}

	// 被攻击时触发
	passive_tigger_be_hit(attack_param);

	// 反弹处理
	hurt_anti_process(attack_param);

	//受击触发
	Status::be_hit_tick(attack_param);

	// 最后减血
	return actual_sub_blood(attack_param);
}

int Fighter::hurt_anti_process(const Attack_Param &attack_param) {
	Fighter *attacker = find_fighter_in_battle_scene(attack_param.attacker);
	if (0 == attacker) return 0;

	if (attacker->is_stoic()) {
		attacker->immuno_injury(role_id());
		return 0;
	}

	double anti_rate = this->rebound_sum();
	double anti_hurt = attack_param.attack_hurt * anti_rate;
	if (attacker && anti_hurt > 0.0) {
		Attack_Param param;
		param.attacker = role_id();
		param.skill_id = attack_param.skill_id;
		param.attack_hurt = anti_hurt;
		attacker->actual_sub_blood_for_buff(param, 0, 0, 0, Status_Def::STATUS_ET_REBOUND);
		//attacker->actual_sub_blood_for_buff()
	}

	return 0;
}

void Fighter::immuno_injury(role_id_t attacker_id) {
	MSG_81000061 msg;
	msg.role_id = role_id();
	msg.attack_id = attacker_id;
	msg.result = Status_Def::STATUS_EFFECT_TYPE_STOIC;

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	scene_broad_around(buf);
	return;
}

int Fighter::passive_tigger_attack(const Attack_Param &attack_param) {
	Fighter *peer = find_fighter_in_battle_scene(attack_param.victim);
	if (peer) {
	}

	return 0;
}

int Fighter::passive_tigger_be_hit(const Attack_Param &attack_param) {
	Fighter *attacker = find_fighter_in_battle_scene(attack_param.attacker);
	if (attacker) {

	}

	be_skill_trigger_active_skill(*this, TYPE_BE_ATTACKED);

	return 0;
}

void Fighter::fight_prop_change_hook(int prop, double value) {
	if(prop == PT_FORCE && battle_status_ == READY){
		Block_Buffer buf;
		MSG_20200110 msg;
		msg.prop_type = PT_FORCE;
		msg.value = value;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();
		send_to_logic(buf);

		if (move_scene() && move_scene()->scene_id() && player_self()) {
			const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(move_scene()->scene_id());
			if (cfg && Scene_Gang_War == cfg->manager) {
				player_self()->gang_war_info().force = static_cast<int>(value);
			}
		}
	}
	if(battle_scene()){
		battle_scene()->fight_prop_change_hook(this, prop, value);
	}
	if (prop == PT_FORCE) {
		if (value > 0.0000000001 || value < 0.0000000001) {
			MSG_20100027 to_logic_msg;
			to_logic_msg.force = fighter_detail_->force;
			THIS_SEND_TO_LOGIC(to_logic_msg);
		}
	}
}

int Fighter::die_process(const Attack_Param &info) {
	//删除所有战斗buff
	clear_all_fighter_status(true);
	//恢复怒气
	if ((0 != current_skill_id_) && (current_skill_id_ != fighter_detail_->default_skill)
			&& (skill_step_used_ == 0)) {
		Skill_DB_Info* skill = get_skill_db_info(current_skill_id_);
		if (skill != 0) {
			const Skill_Detail *skill_detail =
					CONFIG_CACHE_FIGHTER->skill_detail_cache(current_skill_id_,
							skill->skill_level);
			if (0 != skill_detail) {
				Skill_Cost_Vec cost_vec = skill_detail->fight_cost_vec;
				for (Skill_Cost_Vec::iterator cost_find_it = cost_vec.begin();
						cost_find_it != cost_vec.end(); ++cost_find_it) {
					cost_find_it->value = -cost_find_it->value;
				}
				fight_add(cost_vec, TYPE_SKILL_RETURN);
			}
		}
	}
	return 0;
}

void Fighter::broadcast_die_info(void) {
	if (!this->dead_status()) return;

	if (is_dead_broadcast) return;

	be_skill_trigger_active_skill(
			*this, Be_Skill_Trigger_Type::TYPE_DYING);

	battle_status_ = DIE_OVER;
	Block_Buffer buf;
	MSG_81000045 msg;
	msg.role_id = role_id();
	MAKE_MSG_BUF(msg, buf);
	scene_broad_around(buf);
	is_dead_broadcast = true;

//	long_id_t b_id = 0;
	if (validate_battle_scene()) {
//		b_id = battle_scene()->battle_id();
		battle_scene()->kill_partner_called_when_dead(*this);
	}
//	MSG_DEBUG("role_die_broadcast, role_id=%ld, b_id=%ld", role_id(), b_id);
}

void Fighter::die_revive(void) {
	if (dead_status()) {
		Prop_Setter_Vec fight_setter;
		Prop_Setter setter;
		setter.prop_type = PT_BLOOD_CURRENT;
		setter.operate = O_ADD;
		setter.addi_type = AT_BASIC;
		setter.basic = 1;
		fight_setter.push_back(setter);

		modify_props_normal(fight_setter, false, 503);
	}
}

int Fighter::handle_give_up_battle() {
	if (!validate_battle_scene()) return ERROR_BATTLE_OVER;
	if (battle_scene()->is_battle_over()) return ERROR_BATTLE_OVER;

	int win_pos = (mover_detail().battle_position.pos == 0) ? 1 : 0;
	battle_scene()->force_battle_to_end(win_pos, Battle_Scene::OVER_TYPE_GIVE_UP);

	fighter_detail_->modify_basic_property(Property_Type::PT_BLOOD_CURRENT,
			Addition_Type::AT_BASIC, Operate_Type::O_SET, 0);

	return 0;
}

int Fighter::handle_give_up_battle_multi(int type) {

	if (dead_status()) return 0;
	if (!validate_battle_scene()) return ERROR_BATTLE_OVER;
	if (battle_scene()->is_battle_over()) return ERROR_BATTLE_OVER;

	return battle_scene()->handle_multi_role_battle_give_up(role_id(),
			mover_detail().battle_position.pos, type);
}

int Fighter::enter_battle_scene(Battle_Scene *scene) {
	if (0 == scene) return -1;

	Talent_Map::iterator it = fighter_detail_->talent_map.find(fighter_detail_->cur_used_talent_);
	if (it == fighter_detail_->talent_map.end()) {
		MSG_USER("role:%ld, cur_used_talent_:%d", role_id(), fighter_detail_->cur_used_talent_);
		return -1;
	}
	if (it->second.skill_db_info_map.empty()) {
		MSG_USER("role:%ld, cur_used_talent_:%d", role_id(), fighter_detail_->cur_used_talent_);
		return -1;
	}

	Mover::enter_battle_scene(scene);

	reset_player_data(scene);

	skill_tick_time_ 				= Time_Value::zero;
	sing_time_start_ 				= Time_Value::zero;
	skill_step_used_ 			= 0;
	last_skill_step_used_ 	= 0;
	current_skill_id_ 			= 0;
	is_dead_broadcast 			= false;
	battle_status_ 				= Battle_Status::READY;
	cost_morale 					= 0;
	first_used_skill_ 				= 0;
	pre_used_skill_id_ 			= 0;
	pre_use_morale_ 			= 0;

	battle_ttl_damage_ 		= 0.0;
	battle_ttl_be_damaged_= 0.0;
	battle_ttl_cure_ 				= 0.0;

	init_be_skill_data();

	return 0;
}

bool Fighter::is_pk_battle_type(void) {
	if(!validate_battle_scene()) return false;

	int type = battle_scene()->get_battle_type();
	return (type == Battle_Scene::Battle_Type::BATTLE_TYPE_PVP ||
			type == Battle_Scene::Battle_Type::BATTLE_TYPE_ARENA ||
			type == Battle_Scene::Battle_Type::BATTLE_TYPE_TEAM_ARENA ||
			type == Battle_Scene::Battle_Type::BATTLE_TYPE_DRAGON_VALE ||
			type == Battle_Scene::Battle_Type::BATTLE_TYPE_KNIGHT_TRIALS ||
			type == Battle_Scene::Battle_Type::BATTLE_TYPE_GANG_WAR_LEADER ||
			type == Battle_Scene::Battle_Type::BATTLE_TYPE_CHALLENGE);
}

void Fighter::be_skill_call_buff(const Skill_DB_Info_Map& skill_map, const Skill_DB_Info_Vec& extra_skill_vec) {

	Fighter_Vec vec;
	vec.push_back(this);
	for (Skill_DB_Info_Map::const_iterator it =skill_map.begin();
			it != skill_map.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id, it->second.skill_level);
		if (0 == dtl)  continue;
		for (Be_Skill_Effect_Map::const_iterator e_it = dtl->be_skill_effect_map.begin();
				e_it != dtl->be_skill_effect_map.end(); ++e_it) {
			handle_buff(e_it->second.val1, vec, role_id(), it->second.skill_id,
					Statue_Source_Type::TYPE_SOURCE_BE_SKILL);
		}
	}

	for (Skill_DB_Info_Vec::const_iterator it = extra_skill_vec.begin();
			it != extra_skill_vec.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->skill_id);
		if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->skill_id, it->skill_level);
		if (0 == dtl)  continue;
		for (Be_Skill_Effect_Map::const_iterator e_it = dtl->be_skill_effect_map.begin();
				e_it != dtl->be_skill_effect_map.end(); ++e_it) {
			handle_buff(e_it->second.val1, vec, role_id(), it ->skill_id,
					Statue_Source_Type::TYPE_SOURCE_BE_SKILL);
		}
	}
}

void Fighter::init_be_skill_fix_time(const Skill_DB_Info_Map& skill_map, const Skill_DB_Info_Vec& extra_skill_vec) {

	for (Skill_DB_Info_Map::const_iterator it = skill_map.begin(); it != skill_map.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id, it->second.skill_level);
		if (0 == dtl) continue;
		for (Be_Skill_Effect_Map::const_iterator e_it = dtl->be_skill_effect_map.begin();
				e_it != dtl->be_skill_effect_map.end(); ++e_it) {
			if (e_it->second.fix_buff_time.size() > 0) {
				this->fix_buff_time_.insert(e_it->second.fix_buff_time.begin(),
						e_it->second.fix_buff_time.end());
			}
			if (e_it->second.fix_skill_time.size() > 0) {
				this->fix_skill_cd_time_.insert(e_it->second.fix_skill_time.begin(),
						e_it->second.fix_skill_time.end());
			}
		}
	}

	for (Skill_DB_Info_Vec::const_iterator it = extra_skill_vec.begin();
			it != extra_skill_vec.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->skill_id);
		if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->skill_id, it->skill_level);
		if (0 == dtl) continue;
		for (Be_Skill_Effect_Map::const_iterator e_it = dtl->be_skill_effect_map.begin();
				e_it != dtl->be_skill_effect_map.end(); ++e_it) {
			if (e_it->second.fix_buff_time.size() > 0) {
				this->fix_buff_time_.insert(e_it->second.fix_buff_time.begin(),
						e_it->second.fix_buff_time.end());
			}
			if (e_it->second.fix_skill_time.size() > 0) {
				this->fix_skill_cd_time_.insert(e_it->second.fix_skill_time.begin(),
						e_it->second.fix_skill_time.end());
			}
		}
	}
}

void Fighter::init_be_skill_fix_morale_cost(const Skill_DB_Info_Map& skill_map, const Skill_DB_Info_Vec& extra_skill_vec) {

	for (Skill_DB_Info_Map::const_iterator iter = skill_map.begin(); iter != skill_map.end(); ++iter) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(iter->second.skill_id);
		if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(iter->second.skill_id, iter->second.skill_level);
		if (0 == dtl) continue;
		for (Be_Skill_Effect_Map::const_iterator it = dtl->be_skill_effect_map.begin();
				it != dtl->be_skill_effect_map.end(); ++it) {
			for (Int_Double_Map::const_iterator fix_morale_it = it->second.fix_morale_cost_map.begin();
					fix_morale_it != it->second.fix_morale_cost_map.end(); ++fix_morale_it) {
				Int_Double_Map::iterator find_it = fix_morale_cost_map_.find(fix_morale_it->first);
				if (find_it == fix_morale_cost_map_.end()) {
					fix_morale_cost_map_[fix_morale_it->first] = fix_morale_it->second;
				} else {
					find_it->second += fix_morale_it->second;
				}
			}
		}
	}

	for (Skill_DB_Info_Vec::const_iterator it = extra_skill_vec.begin();
			it != extra_skill_vec.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->skill_id);
		if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->skill_id, it->skill_level);
		if (0 == dtl) continue;
		for (Be_Skill_Effect_Map::const_iterator it = dtl->be_skill_effect_map.begin();
				it != dtl->be_skill_effect_map.end(); ++it) {
			for (Int_Double_Map::const_iterator fix_morale_it = it->second.fix_morale_cost_map.begin();
					fix_morale_it != it->second.fix_morale_cost_map.end(); ++fix_morale_it) {
				Int_Double_Map::iterator find_it = fix_morale_cost_map_.find(fix_morale_it->first);
				if (find_it == fix_morale_cost_map_.end()) {
					fix_morale_cost_map_[fix_morale_it->first] = fix_morale_it->second;
				} else {
					find_it->second += fix_morale_it->second;
				}
			}
		}
	}
}

double Fighter::get_skill_fix_morale_cost(int skill_id) {
	Int_Double_Map::iterator find_it = fix_morale_cost_map_.find(skill_id);
	if (find_it == fix_morale_cost_map_.end()) return 0.0;
	return find_it->second;
}

int Fighter::get_fix_buff_time(int status_id) {
	Int_Int_Map::iterator find_it = fix_buff_time_.find(status_id);
	if (find_it == fix_buff_time_.end()) return 0;
	return find_it->second;
}

int Fighter::exit_battle_scene(int exit_from) {

	int btl_type = -1;
	if (0 != battle_scene()) {
		btl_type = battle_scene()->get_battle_type();
	}
	Mover::exit_battle_scene();

	skill_tick_time_ = Time_Value::zero;
	skill_step_used_ = 0;
	last_skill_step_used_ = 0;
	current_skill_id_ = 0;
	is_dead_broadcast = false;
	fighter_detail_->common_cd_map_.clear();
	battle_status_ = Battle_Status::READY;
	first_used_skill_ = 0;
	pre_used_skill_id_ = 0;
	pre_use_morale_ = 0;

	fix_buff_time_.clear();
	fix_skill_cd_time_.clear();
	fix_morale_cost_map_.clear();
	fight_be_skill_map_.clear();
	be_skill_effect_used_map_.clear();

	if (0 == player_self()) return 0;

	int move_scene_type = player_self()->move_scene()->scene_type();
	if (Battle_Scene::BATTLE_TYPE_PVE == btl_type &&
			Elite_Scene_Type == move_scene_type) {
		fighter_detail_->elite_btl_morale = mp_current();
	}

	if (spec_btl_morale_tmp_ > 1.0 || spec_btl_blood_tmp_ > 1.0) {
		reset_morale_and_hp(spec_btl_morale_tmp_, spec_btl_blood_tmp_, false);
		spec_btl_morale_tmp_ = 0.0;
		spec_btl_blood_tmp_ = 0.0;
	}

	return 0;
}

void Fighter::handle_check_over_start(void) {
	Status::clear_all_fighter_status(true);
}

void Fighter::delete_fighter(void) {
	if (!is_dead_broadcast) return;
	if (0 == monster_self()) return;
	this->exit_battle_scene(10);
}

void Fighter::handle_ai_kill_fighter() {
	double blood_max = fighter_detail_->blood_max.total();
	Prop_Setter_Vec prop_vec;
	Prop_Setter prop;
	prop.prop_type = PT_BLOOD_CURRENT;
	prop.operate = O_SUB;
	prop.addi_type = AT_BASIC;
	prop.basic = blood_max;
	prop_vec.push_back(prop);
	MSG_81000060 msg;
	fight_modify_fight_prop(prop_vec, msg.change_info);
	clear_all_fighter_status();

	Block_Buffer buf;
	MSG_81000045 dead_msg;
	dead_msg.role_id = role_id();
	dead_msg.forced_dead = 1;
	MAKE_MSG_BUF(dead_msg, buf);
	scene_broad_around(buf);

	battle_status_ = DIE_OVER;
	is_dead_broadcast = true;
}

double Fighter::actual_sub_blood(const Attack_Param &attack_param) {

	if (0 == fighter_detail_)  return 0;

	bool has_dead = this->dead_status();
	double actual_hurt = ceil(attack_param.attack_hurt);
	Fighter *attacker = find_fighter_in_battle_scene(attack_param.attacker);
	if (role_id() != attack_param.attacker && attacker) {
		attacker->statistic_battle_damage(actual_hurt);
	}
	statistic_battle_be_damaged(actual_hurt);

	MSG_81000060 msg;
	msg.role_id 		= role_id();
	msg.attack_id 		= attack_param.attacker;
	msg.result 			= attack_param.attack_result;
	msg.source_id 	= attack_param.skill_id;

	if (actual_hurt > 0.0) {
		Prop_Setter_Vec fight_setter;
		Prop_Setter setter;
		setter.prop_type = PT_BLOOD_CURRENT;
		setter.operate = O_SUB;
		setter.addi_type = AT_BASIC;
		setter.basic = actual_hurt;
		fight_setter.push_back(setter);
		fight_modify_fight_prop(fight_setter, msg.change_info);

		if (this->dead_status()) {
			if (msg.change_info.size()<=0) {
				Prop_Change chg;
				chg.prop_type = PT_BLOOD_CURRENT;
				chg.old_val = fighter_detail_->current_blood();
				chg.current_val = chg.old_val;
				msg.change_info.push_back(chg);
			}
			for (std::vector<Prop_Change>::iterator it = msg.change_info.begin();
					it != msg.change_info.end(); ++it) {
				if (it->prop_type == PT_BLOOD_CURRENT) {
					it->change_val = -actual_hurt;
				}
			}
		}
	}

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	scene_broad_around(buf);

	if (actual_hurt > 0.0) {
		be_hurt_and_sub_blood(actual_hurt,  blood_current(), attack_param.attacker);
	}

	// 死亡处理
	// 攻击前未死亡，攻击后才死亡，视为死亡，其它情况视为鞭尸
	if (!has_dead && this->dead_status()) {
		die_process(attack_param);

		role_id_t attacker_id = 0;
		int attacker_skill_step = 0;
		int attacker_use_skill = 0;
		if (attacker) {
			attacker_id = attacker->role_id();
			attacker_skill_step = attacker->skill_step_used();
			attacker_use_skill = attacker->current_skill_id();
		}

		long_id_t b_id = 0;
		if (validate_battle_scene()) {
			battle_scene()->listen_die_process(attack_param, *this);
			b_id = battle_scene()->battle_id();
			battle_status_ = DIE_OVER;
		} else {
			MSG_USER("not in battle, role:%ld", role_id());
		}

//		MSG_DEBUG("fighter die, role_id=%ld, attacker_id=%ld, attack_use_skill=%d, attacker_skill_step=%d, b_id=%ld",
//				role_id(), attacker_id, attacker_use_skill, attacker_skill_step, b_id);
	}

	check_dead_and_broadcast(*this, attack_param.attacker);

	return actual_hurt;
}

int Fighter::actual_sub_blood_for_buff(const Attack_Param &attack_param, int status_id,
		int effect_id, double link_hurt_dis, int type) {
	if (0 == fighter_detail_) return 0;

	MSG_81000061 msg;
	msg.role_id = role_id();
	msg.attack_id = attack_param.attacker;
	msg.skill_id = attack_param.skill_id;
	msg.status_id = status_id;
	msg.effect_id = effect_id;
	msg.result = type;

	double actual_hurt = 0.0;
	if (type == Status_Def::STATUS_EFFECT_TYPE_POISON
			|| type == Status_Def::STATUS_ET_POISON_BY_EXPRESSION
			|| type == Status_Def::STATUS_ET_REBOUND) {
		actual_hurt = attack_param.attack_hurt;
		msg.result = Attack_Result::ATTACK_BUFF;
	} else {
		Scene_Player *attacker = SCENE_MONITOR->find_scene_player_by_role_id(attack_param.attacker);
		if (NULL != attacker) {
			get_attack_by_default(attack_param, *(attacker->fighter()), *this, actual_hurt);
		}
	}
	actual_hurt = handle_shield(actual_hurt, attack_param.attacker);
	actual_hurt = ceil(actual_hurt);

	Fighter *attacker = find_fighter_in_battle_scene(attack_param.attacker);
	if (attacker) {
		attacker->statistic_battle_damage(actual_hurt);
	}
	statistic_battle_be_damaged(actual_hurt);

	if (actual_hurt > 0.0) {
		Prop_Setter_Vec fight_setter;
		Prop_Setter setter;
		setter.prop_type = PT_BLOOD_CURRENT;
		setter.operate = O_SUB;
		setter.addi_type = AT_BASIC;
		setter.basic = actual_hurt;
		fight_setter.push_back(setter);
		fight_modify_fight_prop(fight_setter, msg.change_info);

		for (std::vector<Prop_Change>::iterator it = msg.change_info.begin();
				it != msg.change_info.end(); ++it) {
			if (it->prop_type != PT_BLOOD_CURRENT) continue;
			it->change_val = -actual_hurt;
		}
	}

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	scene_broad_around(buf);

	if (actual_hurt > 0.0) {
		be_hurt_and_sub_blood(actual_hurt,  blood_current(), attack_param.attacker);
	}

	// 死亡处理
	if (this->dead_status()) {
		die_process(attack_param);
		if (validate_battle_scene()) {
			battle_scene()->listen_die_process(attack_param, *this);
			battle_status_ = DIE_OVER;
			MSG_DEBUG("role_die, role_id=%ld, buf_id=%d", role_id(), msg.status_id);
		} else {
			MSG_USER("not in battle, role:%ld", role_id());
		}
	}

	check_dead_and_broadcast(*this, 0);

	return 0;
}

void Fighter::check_dead_and_broadcast(Fighter& fighter, role_id_t atk_id) {
	if (!fighter.dead_status()) return;			// 未死亡，不作任何处理

	// 死亡即清掉buff
//	Status::clear_all_fighter_status();

	// 死亡处理，如果已经处理过死亡退出播放,则不做任何处理
	// 否则，检查敌人是否有锁定情况
	if (is_dead_broadcast) return;

	if (validate_battle_scene()) {
		// 如果有被锁定，则做不退出处理
		// 否则做退出处理
		if (battle_scene()->check_dead_fighter_locked(fighter.role_id(), atk_id,
				fighter.mover_detail().battle_position.pos))
			return;
	}
	broadcast_die_info();

//	if (0 == monster_self()) return;
//	this->exit_battle_scene();
}



int Fighter::fight_basic_prop_modify(Property_Type pen, Addition_Type aen,
		Operate_Type oen, double basic) {
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = pen;
	setter.operate = oen;
	setter.addi_type = aen;
	setter.basic = basic;
	fight_setter.push_back(setter);
	//return fight_modify_fight_prop(fight_setter, true);
	return modify_props_normal(fight_setter, true, 3);
}

int Fighter::fight_fixed_prop_modify(Property_Type pen, Addition_Type aen,
		Operate_Type oen, double fixed) {
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = pen;
	setter.operate = oen;
	setter.addi_type = aen;
	setter.fixed = fixed;
	fight_setter.push_back(setter);
	// return fight_modify_fight_prop(fight_setter, true);
	return modify_props_normal(fight_setter, true, 4);
}

int Fighter::fight_percent_prop_modify(Property_Type pen, Addition_Type aen,
		Operate_Type oen, double percent) {
	Prop_Setter_Vec fight_setter;
	Prop_Setter setter;
	setter.prop_type = pen;
	setter.operate = oen;
	setter.addi_type = aen;
	setter.percent = percent;
	fight_setter.push_back(setter);
	// return fight_modify_fight_prop(fight_setter, true);
	return modify_props_normal(fight_setter, true, 6);
}

int Fighter::fight_modify_fight_prop(const Prop_Setter_Vec &prop, MSG_81000060& msg, bool board) {
	msg.role_id = role_id();

	fight_modify_fight_prop(prop, msg.change_info);

	if (msg.change_info.empty()) return 0;

	if (board) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		scene_broad_around(buf);
	} else {
		THIS_SEND_TO_CLIENT(msg);
	}

	return 0;
}

int Fighter::fight_modify_fight_prop(const Prop_Setter_Vec &prop,
		Prop_Change_Vec &change_info) {

	boost::unordered_set<Property_Type> property_set;
	for (Prop_Setter_Vec::const_iterator iter = prop.begin();
			iter != prop.end(); ++iter) {
		switch (iter->prop_type) {
		case PT_POWER: {
			property_set.insert(PT_PHY_ATTACK);
			property_set.insert(PT_SPELL_ATTACK);
			break;
		}
		case PT_AGILITY: {
			property_set.insert(PT_HIT);
			property_set.insert(PT_CRIT);
			break;
		}
		case PT_DEXTERITY: {
			property_set.insert(PT_AVOID);
			property_set.insert(PT_CRIT_DEF);
			break;
		}
		case PT_ARMOR: {
			property_set.insert(PT_PHY_DEFENSE);
			property_set.insert(PT_SPELL_DEFENSE);
			break;
		}
		case PT_STAMINA: {
			property_set.insert(PT_BLOOD_MAX);
//			property_set.insert(PT_BLOOD_CURRENT);
			break;
		}
		case PT_BLOOD_MAX: {
//			property_set.insert(PT_BLOOD_CURRENT);
			break;
		}
//		case PT_PHY_ATTACK: case PT_SPELL_ATTACK: case PT_PHY_DEFENSE: case PT_SPELL_DEFENSE:
//		case PT_HIT: case PT_AVOID: case PT_CRIT: case PT_CRIT_DEF: {
//			property_set.insert(PT_FORCE);
//			break;
//		}
		default: {

			break;
		}
		}
		property_set.insert(iter->prop_type);
	}

	for (boost::unordered_set<Property_Type>::iterator it =
			property_set.begin(); it != property_set.end(); ++it) {
		change_info.push_back(Prop_Change(*it));
	}

	for (std::vector<Prop_Change>::iterator it = change_info.begin();
			it != change_info.end(); ++it) {
		(*it).old_val = fighter_detail_->fetch_fight_property(
				Property_Type((*it).prop_type));
	}

	double old_max_blood = fighter_detail_->fetch_fight_property(Property_Type::PT_BLOOD_MAX);
	double old_cur_blood = fighter_detail_->current_blood();

	for (Prop_Setter_Vec::const_iterator iter = prop.begin();
			iter != prop.end(); ++iter) {
		if (iter->prop_type < PT_FIGHT_PROP_START
				|| iter->prop_type > PT_FIGHT_PROP_END) {
			continue;
		}

		/// 实际增减属性
		fighter_detail_->modify_fight_property(iter->prop_type, iter->addi_type,
				iter->operate, iter->basic, iter->fixed, iter->percent);
	}

	double new_max_blood = fighter_detail_->fetch_fight_property(Property_Type::PT_BLOOD_MAX);
	double changed_blood = new_max_blood - old_max_blood;
	if (changed_blood > 0.0 || changed_blood < 0.0) {
		Prop_Change prop_cur_blood(PT_BLOOD_CURRENT);
		prop_cur_blood.old_val = old_cur_blood;
		change_info.push_back(prop_cur_blood);
	}

	for (std::vector<Prop_Change>::iterator it = change_info.begin();
			it != change_info.end();) {
		(*it).current_val = fighter_detail_->fetch_fight_property(
				Property_Type((*it).prop_type));
		/// 属性值改变通知
		(*it).change_val = (*it).current_val - (*it).old_val;

		if (!((*it).change_val > 0.0 || (*it).change_val < 0.0)) {
			it = change_info.erase(it);
		} else {
			fight_prop_change_hook((*it).prop_type, (*it).current_val);
			++it;
		}
	}

	return 0;
}

int Fighter::fight_modify_props_by_status(const Prop_Setter_Vec &prop, MSG_81000061 &msg, bool is_fighting, int from) {

	if (prop.empty()) return 0;

	Prop_Change force(PT_FORCE);

	Int_Double_Map force_map;
	get_all_pre_force(force_map);

	if (!is_fighting) force.old_val = fighter_detail_->force;

	fight_modify_props(prop, msg.change_info);

	fighter_detail_->update_force();
	force.current_val = fighter_detail_->force;
	force.change_val = force.current_val - force.old_val;

	if (!is_fighting && (force.change_val > 0.0000000001 ||
			force.change_val < -0.0000000001)) {
		msg.change_info.push_back(force);

		MSG_20100027 to_logic_msg;
		to_logic_msg.force = fighter_detail_->force;
		THIS_SEND_TO_LOGIC(to_logic_msg);

		for (std::vector<Prop_Change>::const_iterator it = msg.change_info.begin(); it != msg.change_info.end(); ++it) {
			MSG_DEBUG("%d  %f  %f", (*it).prop_type, (*it).change_val, (*it).old_val, (*it).current_val);
		}
		print_all_part_force(prop[0].addi_type, force.change_val, force_map);
	}

	if (msg.change_info.empty()) return 0;

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	call_broadcast_around(buf);
	if (!is_fighting) {
		THIS_SEND_TO_CLIENT(msg);
	}
	if (hero_self()) {
		Scene_Player* sp = hero_self()->master();
		if (sp) {
			OBJ_SEND_TO_CLIENT(msg, (*sp));
		}
	}

	return 0;
}

int Fighter::fight_modify_props(const Prop_Setter_Vec &prop, Prop_Change_Vec &change_info) {

	boost::unordered_set<Property_Type> property_set;
	for (Prop_Setter_Vec::const_iterator iter = prop.begin();
			iter != prop.end(); ++iter) {
		switch (iter->prop_type) {
		case PT_POWER: {
			property_set.insert(PT_PHY_ATTACK);
			property_set.insert(PT_SPELL_ATTACK);
//			property_set.insert(PT_FORCE);
			break;
		}
		case PT_AGILITY: {
			property_set.insert(PT_HIT);
			property_set.insert(PT_CRIT);
//			property_set.insert(PT_FORCE);
			break;
		}
		case PT_DEXTERITY: {
			property_set.insert(PT_AVOID);
			property_set.insert(PT_CRIT_DEF);
//			property_set.insert(PT_FORCE);
			break;
		}
		case PT_ARMOR: {
			property_set.insert(PT_PHY_DEFENSE);
			property_set.insert(PT_SPELL_DEFENSE);
//			property_set.insert(PT_FORCE);
			break;
		}
		case PT_STAMINA: {
			property_set.insert(PT_BLOOD_MAX);
//			property_set.insert(PT_BLOOD_CURRENT);
//			property_set.insert(PT_FORCE);
			break;
		}
//		case PT_BLOOD_MAX: {
//			property_set.insert(PT_BLOOD_CURRENT);
//			property_set.insert(PT_FORCE);
//			break;
//		}
//		case PT_PHY_ATTACK: case PT_SPELL_ATTACK: case PT_PHY_DEFENSE: case PT_SPELL_DEFENSE:
//		case PT_HIT: case PT_AVOID: case PT_CRIT: case PT_CRIT_DEF: {
//			property_set.insert(PT_FORCE);
//			break;
//		}
		default: {

			break;
		}
		}
		property_set.insert(iter->prop_type);
	}

	for (boost::unordered_set<Property_Type>::iterator it =
			property_set.begin(); it != property_set.end(); ++it) {
		change_info.push_back(Prop_Change(*it));
	}

	for (std::vector<Prop_Change>::iterator it = change_info.begin();
			it != change_info.end(); ++it) {
		(*it).old_val = fighter_detail_->fetch_fight_property(
				Property_Type((*it).prop_type));
	}

	double old_max_blood = fighter_detail_->fetch_fight_property(Property_Type::PT_BLOOD_MAX);

	for (Prop_Setter_Vec::const_iterator iter = prop.begin();
			iter != prop.end(); ++iter) {
		if (iter->prop_type < PT_FIGHT_PROP_START
				|| iter->prop_type > PT_FIGHT_PROP_END) {
			continue;
		}

		/// 实际增减属性
		fight_modify_prop(*iter);
	}

	double new_max_blood = fighter_detail_->fetch_fight_property(Property_Type::PT_BLOOD_MAX);
	double changed_blood = new_max_blood - old_max_blood;
	if (changed_blood > 0.0 || changed_blood < 0.0) {
		Prop_Change prop_cur_blood(PT_BLOOD_CURRENT);
		prop_cur_blood.old_val = old_max_blood;
		change_info.push_back(prop_cur_blood);
	}

	for (std::vector<Prop_Change>::iterator it = change_info.begin();
			it != change_info.end(); ) {
		(*it).current_val = fighter_detail_->fetch_fight_property(
				Property_Type((*it).prop_type));
		/// 属性值改变通知
		(*it).change_val = (*it).current_val - (*it).old_val;

		if (!((*it).change_val > 0.0 || (*it).change_val < 0.0)) {
			it = change_info.erase(it);
		} else {
			fight_prop_change_hook((*it).prop_type, (*it).old_val);
			++it;
		}
	}

	return 0;
}

int Fighter::fight_modify_prop(const Prop_Setter& prop) {
	if (prop.basic > 0.0 || prop.basic < 0.0) {
		Operate_Type o_type = prop.basic < 0 ? O_SUB : O_ADD;
		fighter_detail_->modify_basic_property(prop.prop_type, prop.addi_type, o_type, fabs(prop.basic));
	}
	if (prop.fixed > 0.0 || prop.fixed < 0.0) {
		Operate_Type o_type = prop.fixed < 0 ? O_SUB : O_ADD;
		fighter_detail_->modify_fixed_property(prop.prop_type, prop.addi_type, o_type, fabs(prop.fixed));
	}
	if (prop.percent > 0.0 || prop.percent < 0.0) {
		Operate_Type o_type = prop.percent < 0 ? O_SUB : O_ADD;
		fighter_detail_->modify_percent_property(prop.prop_type, prop.addi_type, o_type, fabs(prop.percent));
	}
	return 0;
}

int Fighter::modify_experience(int exp, Operate_Type oen, int from) {
	// 暂时不做减经验
	if (exp <= 0) {
		return -1;
	}
	if (level() > MAX_LEVEL)
		return 0;

	int64_t old_exp = fighter_detail_->fetch_normal_property(PT_EXP_CURRENT);

	if (old_exp >= LONG_MAX - exp) {
		old_exp = LONG_MAX;
	} else {
		old_exp += exp;
	}

	int64_t exp_up = CONFIG_CACHE_ROLE->level_up_need_exp_map(level());
	if (exp_up < 1.0) {
		MSG_USER("exp_up:%ld", exp_up);
		return -1;
	}
	// 当升级时，等级-经验需要顺序发，在升级时，广播的方式保证顺序发包
	bool is_lv_up = false;
	while (old_exp >= exp_up && level() < MAX_LEVEL) {
		old_exp -= exp_up;

		if (level() < 0)
			return -1;
		if (level() + 1 > MAX_LEVEL)
			return -1;

		fight_normal_prop_modify(PT_LEVEL, O_ADD, 1, true);
		is_lv_up = true;

		exp_up = CONFIG_CACHE_ROLE->level_up_need_exp_map(level());
		if (exp_up < 1.0)
			break;
	}

	fight_normal_prop_modify(PT_EXP_CURRENT, O_SET, old_exp, is_lv_up);

	return 0;
}

void Fighter::get_all_be_skill_step_fix(Skill_Step_Fix_Vec &step_fix, int effect_grp_id) {
	if (0 == fighter_detail_) return;
	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(
			fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) return;
	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++ it) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || cfg->skill_sort != Skill_Sort::BE_SKILL) continue;
		const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id,
				it->second.skill_level);
		if (0 == dtl) continue;

		for (Be_Skill_Effect_Map::const_iterator b_it = dtl->be_skill_effect_map.begin();
				b_it != dtl->be_skill_effect_map.end(); ++b_it) {

			Int_Vec::const_iterator g_it = find(b_it->second.effect_vec.begin(),
					b_it->second.effect_vec.end(), effect_grp_id);
			if (g_it == b_it->second.effect_vec.end()) continue;

			check_and_get_step_fix(b_it->second.be_fix_type, b_it->second.be_fix_steps, step_fix);
		}
	}

	for (Skill_DB_Info_Vec::iterator it = fighter_detail_->extra_skill_vec.begin();
			it != fighter_detail_->extra_skill_vec.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->skill_id);
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->skill_id, it->skill_level);
		if (0 == cfg || 0 == dtl || cfg->skill_sort != Skill_Sort::BE_SKILL) continue;

		for (Be_Skill_Effect_Map::const_iterator b_it = dtl->be_skill_effect_map.begin();
				b_it != dtl->be_skill_effect_map.end(); ++b_it) {

			Int_Vec::const_iterator g_it = find(b_it->second.effect_vec.begin(),
					b_it->second.effect_vec.end(), effect_grp_id);
			if (g_it == b_it->second.effect_vec.end()) continue;

			check_and_get_step_fix(b_it->second.be_fix_type, b_it->second.be_fix_steps, step_fix);
		}
	}

	for (Fight_Be_Skill_Map::iterator fb_it = fight_be_skill_map_.begin();
			fb_it != fight_be_skill_map_.end(); ++fb_it) {
		const Skill_Detail* be_dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(
				fb_it->second.id, fb_it->second.lv);
		if (0 == be_dtl) continue;
		for (Be_Skill_Effect_Map::const_iterator b_it = be_dtl->be_skill_effect_map.begin();
				b_it != be_dtl->be_skill_effect_map.end(); ++b_it) {
			Int_Vec::const_iterator g_it = find(b_it->second.effect_vec.begin(),
					b_it->second.effect_vec.end(), effect_grp_id);
			if (g_it == b_it->second.effect_vec.end()) continue;

			check_and_get_step_fix(b_it->second.be_fix_type, b_it->second.be_fix_steps, step_fix);
		}
	}
}

void Fighter::check_and_get_step_fix(int type, const Skill_Step_Fix_Vec& step_fix, Skill_Step_Fix_Vec & fixese) {
	if (type <= 0) return;
	Double_Vec vec;
	switch(type) {
	case FCT_BLOOD_VALUE:
		vec.push_back(blood_current());
		take_step_fix_out(vec, step_fix, fixese);
		break;
	case FCT_BLOOD_PERCENT:
		vec.push_back(current_blood_rate());
		take_step_fix_out(vec, step_fix, fixese);
		break;
	case FCT_MORAL_VALUE:
		vec.push_back(mp_current());
		take_step_fix_out(vec, step_fix, fixese);
		break;
	case FCT_MORAL_PERCENT:
		vec.push_back(current_mp_rate());
		take_step_fix_out(vec, step_fix, fixese);
		break;
	case FCT_STEP_MORALE:
		vec.push_back(cost_morale);
		take_step_fix_out(vec, step_fix, fixese);
		break;
	case FCT_BUFF_ID:{
		for (Skill_Step_Fix_Vec::const_iterator it=step_fix.begin();
				it!=step_fix.end(); ++it) {
			if (has_status_id(static_cast<int>(it->value))) {
				fixese.push_back(*it);
				break;
			}
		}
		break;
	}
	default:{
		if (type < 100) break;
		int num = status_num(type);
		Skill_Step_Fix_Vec::const_iterator ptr = step_fix.end();
		for (Skill_Step_Fix_Vec::const_iterator it = step_fix.begin();
				it != step_fix.end(); ++it) {
			if (num < (int)it->value) continue;
			if (ptr == step_fix.end()) ptr = it;
			if (it->value > ptr->value) ptr = it;
		}
		if (ptr == step_fix.end())break;
		fixese.push_back(*ptr);
		break;
	}
	}
}

void Fighter::get_step_fix_by_target(const Skill_Effect & effect, Skill_Step_Fix_Vec & step_fix, Fighter_Vec &vec) {
	Double_Vec val_double_vec;
	Double_Vec per_double_vec;
	for (Fighter_Vec::iterator it = vec.begin(); it != vec.end(); ++it) {
		val_double_vec.push_back((*it)->blood_current());
		per_double_vec.push_back((*it)->current_blood_rate());
	}
	switch(effect.step_fix_type) {
	case FCT_ENEMY_BLD_VAL:
		take_step_fix_out(val_double_vec, effect.skill_step_fix_vec, step_fix);
		break;
	case FCT_ENEMY_BLD_PER:
		take_step_fix_out(per_double_vec, effect.skill_step_fix_vec, step_fix);
		break;
	default:
		break;
	}

	if (0 == fighter_detail_) return;
	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(
			fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) return;
	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++ it) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || cfg->skill_sort != Skill_Sort::BE_SKILL) continue;
		const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id,
				it->second.skill_level);
		if (0 == dtl) continue;

		for (Be_Skill_Effect_Map::const_iterator b_it = dtl->be_skill_effect_map.begin();
				b_it != dtl->be_skill_effect_map.end(); ++b_it) {

			Int_Vec::const_iterator g_it = find(b_it->second.effect_vec.begin(),
					b_it->second.effect_vec.end(), effect.effectgroup_id);
			if (g_it == b_it->second.effect_vec.end()) continue;

			switch(b_it->second.be_fix_type) {
			case FCT_ENEMY_BLD_VAL:
				take_step_fix_out(val_double_vec, b_it->second.be_fix_steps, step_fix);
				break;
			case FCT_ENEMY_BLD_PER:
				take_step_fix_out(per_double_vec, b_it->second.be_fix_steps, step_fix);
				break;
			default:
				break;
			}
		}
	}

	for (Fight_Be_Skill_Map::iterator f_it = fight_be_skill_map_.begin();
			f_it != fight_be_skill_map_.end(); ++f_it) {
		const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(f_it->second.id, f_it->second.lv);
		if (0 == dtl) continue;
		for (Be_Skill_Effect_Map::const_iterator b_it = dtl->be_skill_effect_map.begin();
				b_it != dtl->be_skill_effect_map.end(); ++b_it) {

			Int_Vec::const_iterator g_it = find(b_it->second.effect_vec.begin(),
					b_it->second.effect_vec.end(), effect.effectgroup_id);
			if (g_it == b_it->second.effect_vec.end()) continue;

			switch(b_it->second.be_fix_type) {
			case FCT_ENEMY_BLD_VAL:
				take_step_fix_out(val_double_vec, b_it->second.be_fix_steps, step_fix);
				break;
			case FCT_ENEMY_BLD_PER:
				take_step_fix_out(per_double_vec, b_it->second.be_fix_steps, step_fix);
				break;
			default:
				break;
			}
		}
	}
}

void Fighter::take_step_fix_out(const Double_Vec& values, const Skill_Step_Fix_Vec& step_fix, Skill_Step_Fix_Vec & fixese) {
	if (values.size() <= 0) return;
	Skill_Step_Fix_Vec::const_iterator ptr = step_fix.end();
	bool is_allow = true;
	for (Skill_Step_Fix_Vec::const_iterator it = step_fix.begin(); it != step_fix.end(); ++it) {
		is_allow = true;
		for (Double_Vec::const_iterator v_it = values.begin();
				v_it != values.end(); ++v_it) {
			if ((*v_it) < it->value) {
				is_allow = false;
				break;
			}
		}
		if (is_allow) {
			if (ptr == step_fix.end()) ptr = it;
			if (it->value > ptr->value) ptr = it;
		}
	}
	if (ptr == step_fix.end()) return;
	fixese.push_back(*ptr);
}

void Fighter::confirm_current_skill_effect(void) {
	if (0 == current_skill_id_) return;
	Skill_DB_Info* skill = get_skill_db_info(current_skill_id_);
	if (0 == skill) return;
	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill->skill_id);
	if (0 == cfg) return;
	const Skill_Detail* detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill->skill_id,
			skill->skill_level);
	if (0 == detail) return;
	current_effect_map_.clear();

	Fighter_Vec fighter_vec;
	for (Role_Vec::iterator it = simulate_targets_.begin();
			it != simulate_targets_.end(); ++it) {
		Mover* mover = battle_scene()->find_mover_with_role_id(*it);
		if (0 == mover || 0 == mover->fighter()) continue;
		fighter_vec.push_back(mover->fighter());
	}

	int org_series_id = 0;
	bool effect_series_active = false;
	bool last_player_effect_active = true;
	int index = 1;
	for (uint i=1; i <= detail->step_effect.size(); ++i) {
		Skill_Effect_Map::const_iterator find_it = detail->step_effect.find(i);
		if (find_it == detail->step_effect.end()) continue;

		Skill_Step_Fix_Vec step_fixese;
		get_all_be_skill_step_fix(step_fixese, find_it->second.effectgroup_id);

		Skill_Effect_Correct dtl_crt;
		dtl_crt.effect_id = find_it->second.effect_id;

		check_and_get_step_fix(find_it->second.step_fix_type,
				find_it->second.skill_step_fix_vec, step_fixese);

		get_step_fix_by_target(find_it->second, step_fixese, fighter_vec);

		get_effect_rate_correct(step_fixese, dtl_crt);
		skill_ps_attr(dtl_crt);

		double effect_rate = fighter_detail_->fetch_fight_property(Property_Type::PT_EFFECT_RATE,
				Prop_Value::ELEMADD::ELEM_FIGHT);
		double final_rate = 1 + effect_rate + find_it->second.effect_rate;

		if (0 == find_it->second.effect_series_id ||
			org_series_id != find_it->second.effect_series_id) {
			double random_val = (random() % 100) / 100.0;
			effect_series_active = random_val < final_rate;
			org_series_id = find_it->second.effect_series_id;
		}

		if (cfg->skill_type == Skill_Type::TYPE_FLOAT_SKILL) {
			if (find_it->second.effect_type == EFFECT_PLAY_EFFECT) {
				if (effect_series_active) {
					if (!last_player_effect_active) effect_series_active = false;
				} else {
					last_player_effect_active = false;
				}
			}
		}

		if (effect_series_active) {
			current_effect_map_[index] = find_it->second;
			++index;
		}

		skill_recover_attr(dtl_crt);
	}

	fight_cost(detail->fight_cost_vec, 2);
}

void Fighter::total_skill_correct(const Skill_Effect_Correct &org, Skill_Effect_Correct &des, const Skill_Step_Fix_Vec &fix) {
	des.effect_id = org.effect_id;
	for (Attr_Correct_Vec::const_iterator iter = org.attr_crr_vec.begin(); iter != org.attr_crr_vec.end(); ++iter) {
		des.attr_crr_vec.push_back(*iter);
	}
	Attr_Correct crt;
	for (Skill_Step_Fix_Vec::const_iterator it = fix.begin(); it != fix.end(); ++it) {
		crt.reset();
		if (it->id == TYPE_HURT_RATE){
			crt.type = PT_HURT_RATE;
			crt.fixed_value = it->val2;
			des.attr_crr_vec.push_back(crt);
		} else if (it->id > 1000) {
			crt.type = it->id;
			crt.fixed_value = it->val2;
			crt.percent_value = it->val3;
			des.attr_crr_vec.push_back(crt);
		}
	}
}

void Fighter::get_effect_rate_correct(const Skill_Step_Fix_Vec &fix, Skill_Effect_Correct &des){
	for (Skill_Step_Fix_Vec::const_iterator it = fix.begin(); it != fix.end(); ++it) {
		if (it->id == TYPE_EFFECT_TRIGGER) {
			Attr_Correct crt;
			crt.type = PT_EFFECT_RATE;
			crt.fixed_value = it->val2;
			des.attr_crr_vec.push_back(crt);
		}
	}
}

void Fighter::be_skill_trriger_skill(void) {
	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(
			fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) return;

	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || cfg->skill_sort != Skill_Sort::BE_SKILL) continue;
		const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(
				it->second.skill_id, it->second.skill_level);
		if (0 == dtl) continue;

		for (Be_Skill_Effect_Map::const_iterator b_it = dtl->be_skill_effect_map.begin();
				b_it != dtl->be_skill_effect_map.end(); ++b_it) {
			for (Int_Vec::const_iterator s_it = b_it->second.open_skills.begin();
					s_it != b_it->second.open_skills.end(); ++s_it) {
				Skill_DB_Info_Map::iterator find_skill = find_it->second.skill_db_info_map.find(*s_it);
				Role_Vec vec;
				if (find_skill == find_it->second.skill_db_info_map.end()) {
					handle_inner_call_skill_action(vec, *s_it, 1);
				} else {
					handle_inner_call_skill_action(vec, find_skill->second.skill_id, find_skill->second.skill_level);
				}
			}
		}
	}
}

void Fighter::async_be_skill_trigger_active_skill(
		int type) {

	if (!validate_battle_scene()) return;

	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(
			fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) return;

	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		async_be_skill_trigger_active_skill(type, it->second.skill_id, it->second.skill_level);
	}

	for (Skill_DB_Info_Vec::iterator it = fighter_detail_->extra_skill_vec.begin();
			it != fighter_detail_->extra_skill_vec.end(); ++it) {
		async_be_skill_trigger_active_skill(type,  it->skill_id, it->skill_level);
	}

	for (Fight_Be_Skill_Map::iterator it = fight_be_skill_map_.begin();
			it != fight_be_skill_map_.end(); ++it) {
		async_be_skill_trigger_active_skill(type, it->second.id, it->second.lv);
	}
}

void Fighter::async_be_skill_trigger_active_skill(int type, int skill_id, int  skill_lv) {

	const Skill_Config *skill_cfg = CONFIG_CACHE_FIGHTER->
			skill_config_cache(skill_id);
	if (0 == skill_cfg) return;
	if (Skill_Sort::BE_SKILL != skill_cfg->skill_sort) return;
	const Skill_Detail *detail_cfg = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_lv);
	if (0 == detail_cfg) return;

	for (Be_Skill_Effect_Map::const_iterator it = detail_cfg->be_skill_effect_map.begin();
			it != detail_cfg->be_skill_effect_map.end(); ++it) {

		if (!judge_async_be_skiill_trigger_active_skill(it->second, type)) continue;

		Be_Skill_Effect_Used_Map::iterator find_it = be_skill_effect_used_map_.find(
				it->second.be_skill_effect_id);
		if (find_it == be_skill_effect_used_map_.end()) {
			Be_Skill_Effect_Used be_skill_effect_used;
			be_skill_effect_used.effect_id = it->second.be_skill_effect_id;
			be_skill_effect_used.used_times = 1;
			be_skill_effect_used.last_used_time = Time_Value::gettimeofday();
			be_skill_effect_used_map_[be_skill_effect_used.effect_id] = be_skill_effect_used;
		} else {
			find_it->second.used_times++;
			find_it->second.last_used_time = Time_Value::gettimeofday();
		}
		for (Int_Vec::const_iterator iter = it->second.trigger_skill.begin();
				iter != it->second.trigger_skill.end(); ++iter) {
			Role_Vec vec;
			handle_inner_call_skill_action(vec, *iter, 1);
		}
	}
}

bool Fighter::judge_async_be_skiill_trigger_active_skill(const Be_Skill_Effect &effect, int type) {

	if (type != effect.trigger_type) return false;

	Be_Skill_Effect_Used_Map::iterator find_it = be_skill_effect_used_map_.find(effect.be_skill_effect_id);
	if (find_it != be_skill_effect_used_map_.end()) {
		if (find_it->second.used_times >= effect.max_trigger_times) return false;
		Time_Value delta = Time_Value::gettimeofday() - find_it->second.last_used_time;
		if (delta.msec() <= effect.trigger_cd_time) return false;
	}

	if (type == TYPE_BLOOD_VAL) {
		double val = effect.trigger_threshold_val / 1000.0;
		if (current_blood_rate() > val) return false;
	} else if (type == TYPE_MORALE_VAL) {
		double val = effect.trigger_threshold_val / 1000.0;
		if (current_mp_rate() < val) return false;
	}

	return true;
}

void Fighter::fight_add_be_skill(int skill_id, int skill_lv) {
	if (skill_id <= 0 || skill_lv <= 0) return;
	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) return;
	Fight_Be_Skill_Map::iterator find_it = fight_be_skill_map_.find(skill_id);
	if (find_it != fight_be_skill_map_.end()) return;
	const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_lv);
	if (0 == dtl) return;

	Dynamic_Be_Skill be_skill;
	be_skill.id = skill_id;
	be_skill.lv = skill_lv;
	fight_be_skill_map_[skill_id] = be_skill;

	Prop_Setter_Vec setter_vec;
	get_be_skill_add_attr(*dtl, setter_vec, false);
	if (setter_vec.size() > 0) {
		Prop_Change_Vec change_vec;
		fight_modify_props(setter_vec, change_vec);
	}
}

void Fighter::fight_remove_be_skill(int skill_id) {
	if (skill_id <= 0) return;
	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) return;
	Fight_Be_Skill_Map::iterator find_it = fight_be_skill_map_.find(skill_id);
	if (find_it == fight_be_skill_map_.end()) return;
	const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, find_it->second.lv);
	if (0 == dtl) return;

	fight_be_skill_map_.erase(find_it);

	Prop_Setter_Vec setter_vec;
	get_be_skill_add_attr(*dtl, setter_vec, true);
	if (setter_vec.size() > 0) {
		Prop_Change_Vec change_vec;
		fight_modify_props(setter_vec, change_vec);
	}
}

double Fighter::get_be_skill_add_moral(Attack_Result result, Be_Skill_Add_Moral type) {
	double total = 0.0;
	if (ADD_TYPE_CRIT == type && Attack_Result::ATTACK_CRIT != result) return total;
	if (ADD_TYPE_HIT == type && Attack_Result::ATTACK_CRIT != result && Attack_Result::ATTACK_NORMAL != result) return total;
	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) {
		MSG_USER_TRACE("role:%ld, cur_talent_page:%d", role_id(), fighter_detail_->cur_used_talent_);
		return total;
	}

	if (find_it->second.skill_db_info_map.empty()) {
		MSG_USER_TRACE("role:%ld, cur_talent_page:%d", role_id(), fighter_detail_->cur_used_talent_);
		return total;
	}

	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id, it->second.skill_level);
		if (0 == dtl) continue;
		for (Be_Skill_Effect_Map::const_iterator b_it = dtl->be_skill_effect_map.begin();
				b_it != dtl->be_skill_effect_map.end(); ++b_it) {
			for (Int_Double_Map::const_iterator m_it = b_it->second.add_moral_map.begin();
					m_it != b_it->second.add_moral_map.end(); ++m_it) {
				total += m_it->first == type ? m_it->second : 0.0;
			}
		}
	}

	for (Skill_DB_Info_Vec::iterator it = fighter_detail_->extra_skill_vec.begin();
			it != fighter_detail_->extra_skill_vec.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->skill_id);
		if (0 == cfg || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->skill_id, it->skill_level);
		if (0 == dtl) continue;
		for (Be_Skill_Effect_Map::const_iterator b_it = dtl->be_skill_effect_map.begin();
				b_it != dtl->be_skill_effect_map.end(); ++b_it) {
			for (Int_Double_Map::const_iterator m_it = b_it->second.add_moral_map.begin();
					m_it != b_it->second.add_moral_map.end(); ++m_it) {
				total += m_it->first == type ? m_it->second : 0.0;
			}
		}
	}

	for (Fight_Be_Skill_Map::iterator fb_it = fight_be_skill_map_.begin();
			fb_it != fight_be_skill_map_.end(); ++fb_it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(fb_it->second.id);
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(fb_it->second.id, fb_it->second.lv);
		if (0 == cfg || 0 == dtl || Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		for (Be_Skill_Effect_Map::const_iterator b_it = dtl->be_skill_effect_map.begin();
				b_it != dtl->be_skill_effect_map.end(); ++b_it) {
			for (Int_Double_Map::const_iterator m_it = b_it->second.add_moral_map.begin();
					m_it != b_it->second.add_moral_map.end(); ++m_it) {
				total += m_it->first == type ? m_it->second : 0.0;
			}
		}
	}

	return total;
}

void Fighter::add_moral_every_hit(double moral, Attack_Result result, int skill_id) {
	if (!(moral < 0.0 || moral > 0.0)) return;
	if (Attack_Result::ATTACK_CRIT != result && Attack_Result::ATTACK_NORMAL != result) return;

	Skill_Cost cost;
	cost.type = COST_MORALE;
	cost.coster = COST_SELF;
	cost.value = moral;
	Skill_Cost_Vec vec;
	vec.push_back(cost);
	fight_add(vec, TYPE_SKILL_HIT_ADD, skill_id);
}

int Fighter::get_default_skill(void){
	if(fighter_detail_){
		return fighter_detail_->default_skill;
	}
	return 0;
}

void Fighter::init_fighter_skill_pre_cd(const Time_Value &now) {
	if (!validate_battle_scene()) return;

	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) return;

	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || BE_SKILL == cfg->skill_sort) continue;
		const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id, it->second.skill_level);
		if (0 == dtl || dtl->pre_cd_vec.size() <= 0) continue;
		add_skill_cd_time(dtl->pre_cd_vec, now);
	}
}

void Fighter::init_be_skill_data(void) {
	if (!validate_battle_scene()) return;

	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) return;

	init_be_skill_fix_time(find_it->second.skill_db_info_map,
			fighter_detail_->extra_skill_vec);
	init_be_skill_fix_morale_cost(find_it->second.skill_db_info_map,
			fighter_detail_->extra_skill_vec);
//	be_skill_call_buff(find_it->second.skill_db_info_map,
//			fighter_detail_->extra_skill_vec);

	Status::status_hook_enter_battle_scene();
	init_fighter_skill_pre_cd(Time_Value::gettimeofday());
}

void Fighter::send_fight_data_when_battle_start() {
	client_scene_complete();

	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(fighter_detail_->cur_used_talent_);
	if (find_it == fighter_detail_->talent_map.end()) return;

	be_skill_call_buff(find_it->second.skill_db_info_map, fighter_detail_->extra_skill_vec);
//	Status::special_status_handle();
}

void Fighter::send_init_data_to_client(){
	MSG_50200115 msg;
	msg.role_id = role_id();
	Int_Int fix_value;
	for (Int_Int_Map::iterator it = fix_skill_cd_time_.begin(); it != fix_skill_cd_time_.end(); ++it) {
		fix_value.reset();
		fix_value.val_1 = it->first;
		fix_value.val_2 = it->second;
		msg.be_skill_fix_cd_vec.push_back(fix_value);
	}

	for (Int_Double_Map::iterator it = fix_morale_cost_map_.begin(); it != fix_morale_cost_map_.end(); ++it) {
		msg.be_skill_fix_morale_cost_vec.push_back(Int_Double(it->first, it->second));
	}

	Time_Value now = Time_Value::gettimeofday();
	Int_Int skill_cd;
	for (Common_Cd_Map::iterator it = fighter_detail_->common_cd_map_.begin();
			it != fighter_detail_->common_cd_map_.end(); ++it) {
		skill_cd.reset();
		skill_cd.val_1 = it->second.type_;
		Time_Value delta = now - it->second.last_use_;
		if (delta < Time_Value::zero) continue;
		skill_cd.val_2 = it->second.millisecond_ - delta.msec();
		if (skill_cd.val_2 < 0) continue;
		msg.skill_cd_vec.push_back(skill_cd);
	}

	if (is_player()) {
		THIS_SEND_TO_CLIENT(msg);
	} else if(is_hero()) {
		Fighter* fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
		if (0 == fighter) return;
		OBJ_SEND_TO_CLIENT(msg, (*fighter));
	}
}

void Fighter::return_morale_when_blt_over(void) {
	if (0 == current_skill_id_|| 0 != skill_step_used_) return;
	Skill_DB_Info *skill = get_skill_db_info(current_skill_id_);
	if (0 == skill) return;
	const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill->skill_id, skill->skill_level);
	if (0 == dtl) return;
	Skill_Cost_Vec vec;
	Skill_Cost cost;
	for (Skill_Cost_Vec::const_iterator it = dtl->fight_cost_vec.begin();
			it != dtl->fight_cost_vec.end(); ++it) {
		cost.reset();
		if (it->type == COST_MORALE && it->value < 0) {
			cost.type = it->type;
			cost.value = -it->value;
			cost.coster = it->coster;
			vec.push_back(cost);
		} else if (it->type == COST_MORALE_PERCENT) {
			cost.type = COST_MORALE;
			cost.coster = 1;
			if (it->value + 1 > 0) {
				cost.value = this->fighter_detail_->current_morale() / (1 + it->value);
				vec.push_back(cost);
			}
		}
	}
	this->fight_add(vec, TYPE_SKILL_RETURN);
}

void Fighter::clear_simulate_target(void) {
	Time_Value now = Time_Value::gettimeofday();
	if (this->skill_step_used_ != 0) return;
	simulate_targets_.clear();
}

double Fighter::handle_poition_buff(int type, double atk, double atk_hrt_add, double pk_add, double hurt_add, double race_add) {
	if (Property_Type::PT_PHY_ATTACK != type && Property_Type::PT_SPELL_ATTACK != type) return 0.0;
	if (!validate_battle_scene()) return 0.0;

	double def = 0.0;
	double atk_hrt_sub = 0.0;
	if (Property_Type::PT_PHY_ATTACK == type) {
		def = fighter_detail().fetch_fight_property(Property_Type::PT_PHY_DEFENSE, Prop_Value::ELEM_FIGHT);
		atk_hrt_sub = fighter_detail().fetch_fight_property(Property_Type::PT_PHY_HURT_RATE_SUB, Prop_Value::ELEM_FIGHT);

	} else {
		def = fighter_detail().fetch_fight_property(Property_Type::PT_SPELL_DEFENSE, Prop_Value::ELEM_FIGHT);
		atk_hrt_sub = fighter_detail().fetch_fight_property(Property_Type::PT_SPELL_HURT_RATE_SUB, Prop_Value::ELEM_FIGHT);
	}

	double pk_sub = fighter_detail().fetch_fight_property(Property_Type::PT_PK_HURT_SUB, Prop_Value::ELEM_FIGHT);
	pk_sub = (battle_scene()->get_battle_type() == Battle_Scene::BATTLE_TYPE_PVP ? pk_sub : 0.0);

	if (!(atk + def >0.0)) return 0.0;

	double hurt = atk * atk / (atk + def) * (1 + atk_hrt_add + hurt_add + atk_hrt_sub) * (1 + pk_add + pk_sub + race_add);

	return ceil(hurt);
}

double Fighter::get_pointion_buff_atk(int type) {
	if (Property_Type::PT_PHY_ATTACK != type && Property_Type::PT_SPELL_ATTACK != type) return 0.0;
	if (Property_Type::PT_PHY_ATTACK == type)
		return fighter_detail().fetch_fight_property(Property_Type::PT_PHY_ATTACK, Prop_Value::ELEM_FIGHT);
	return fighter_detail().fetch_fight_property(Property_Type::PT_SPELL_ATTACK, Prop_Value::ELEM_FIGHT);
}
double Fighter::get_pointion_buff_atk_hurt_rate(int type) {
	if (Property_Type::PT_PHY_ATTACK != type && Property_Type::PT_SPELL_ATTACK != type) return 0.0;
	if (Property_Type::PT_PHY_ATTACK == type)
		return fighter_detail().fetch_fight_property(Property_Type::PT_PHY_HURT_RATE, Prop_Value::ELEM_FIGHT);
	return fighter_detail().fetch_fight_property(Property_Type::PT_SPELL_HURT_RATE, Prop_Value::ELEM_FIGHT);
}

double Fighter::get_pointion_buff_hurt_rate(void) {
	return fighter_detail().fetch_fight_property(Property_Type::PT_HURT_RATE, Prop_Value::ELEM_FIGHT);
}

double Fighter::get_pointion_buff_pk_hurt(void) {
	if (!validate_battle_scene()) return 0.0;
	double pk_sub = fighter_detail().fetch_fight_property(Property_Type::PT_PK_HURT_ADD, Prop_Value::ELEM_FIGHT);
	return (battle_scene()->get_battle_type() == Battle_Scene::BATTLE_TYPE_PVP ? pk_sub : 0.0);
}

double Fighter::get_pointion_buff_race_hurt(Fighter &peer) {
	return get_race_hurt_add(*this, peer);
}

void Fighter::slice_floating_skill() {
	typedef std::vector<Addition_Skill_Trigger_Info> Info_Vec;
	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (0 == cfg) return;
	if (Skill_Type::TYPE_FLOAT_SKILL != cfg->skill_type) return;
	if (1 != skill_step_used_) return;

	int idx = 0;
	bool slice = false;
	MSG_50200114 msg;
	msg.role_id = role_id();
	Addition_Skill_Trigger_Info info;
	for(Skill_Effect_Map::iterator it = current_effect_map_.begin();
			it != current_effect_map_.end(); ++it) {
		if (EFFECT_PLAY_EFFECT == it->second.effect_type &&  1 != it->first) {
			slice = true;
		}

		if (!slice) continue;
		if (EFFECT_PLAY_EFFECT == it->second.effect_type || EFFECT_FLOAT_OVER == it->second.effect_type) {
			if(info.effect_vec.size() != 0) {
				msg.trigger_vec.push_back(info);
			}
			info.reset();
			++idx;
			info.index = idx;
		}
		info.effect_vec.push_back(it->second.effect_id);
	}

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	call_broadcast_around(buf);
}

void Fighter::skip_addition_skill(bool is_skip) {
	if (!is_skip) return;
	if (1 == skill_step_used_)return;

	const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(current_skill_id_);
	if (0 == cfg) return;
	if (Skill_Type::TYPE_FLOAT_SKILL != cfg->skill_type) return;

	Skill_Effect_Map::iterator find_it = current_effect_map_.find(skill_step_used_);
	if (find_it == current_effect_map_.end()) return;

	if (EFFECT_PLAY_EFFECT != find_it->second.effect_type) return;
	skill_step_used_ = current_effect_map_.size();
}

void Fighter::statistic_battle_damage(double value) {
	Fighter *fighter = 0;
	if (is_hero()) {
		fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
	} else if (0 != monster_self() && 0 != monster_self()->master_role_id()){
		fighter = find_fighter_in_battle_scene(monster_self()->master_role_id());
	}else if (is_player()) {
		fighter = this;
	}
	if (0 == fighter) return;

	fighter->add_battle_damage(value);
}

void Fighter::add_battle_damage(double value) {
	battle_ttl_damage_ += value;
}

double Fighter::get_ttl_damage_in_battle(void) const {
	return battle_ttl_damage_;
}

void Fighter::statistic_battle_be_damaged(double value) {
	Fighter *fighter = 0;
	if (is_hero()) {
		fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
	} else if (0 != monster_self() && 0 != monster_self()->master_role_id()){
		fighter = find_fighter_in_battle_scene(monster_self()->master_role_id());
	}else if (is_player()) {
		fighter = this;
	}
	if (0 == fighter) return;

	fighter->add_battle_be_damaged(value);
}

void Fighter::add_battle_be_damaged(double value) {
	battle_ttl_be_damaged_ += value;
}

double Fighter::get_ttl_be_damanged_in_battle(void)const {
	return battle_ttl_be_damaged_;
}

void Fighter::statistic_battle_cure(double value) {
	Fighter *fighter = 0;
	if (is_hero()) {
		fighter = find_fighter_in_battle_scene(hero_self()->hero_base_detail().master_role_id);
	} else if (0 != monster_self() && 0 != monster_self()->master_role_id()){
		fighter = find_fighter_in_battle_scene(monster_self()->master_role_id());
	}else if (is_player()) {
		fighter = this;
	}
	if (0 == fighter) return;

	fighter->add_battle_cure(value);
}

void Fighter::add_battle_cure(double value) {
	battle_ttl_cure_ += value;
}

double Fighter::get_ttl_cure_in_battle(void) const {
	return battle_ttl_cure_;
}

void Fighter::be_hurt_and_sub_blood(double sub_blood,
		double current_blood, role_id_t attacker_id) {

	be_skill_trigger_active_skill(*this, TYPE_BLOOD_VAL);
}

void Fighter::set_blood_outer_of_battle(double sub_blood, double current_blood) {
	current_blood = current_blood > 0.0 ? current_blood : 0.0;
	fighter_detail_->modify_basic_property(PT_BLOOD_CURRENT, Addition_Type::AT_BASIC,
			Operate_Type::O_SET, current_blood);
}

void Fighter::manual_set_fighter_dead(void) {

	fighter_detail_->modify_basic_property(Property_Type::PT_BLOOD_CURRENT,
			Addition_Type::AT_BASIC, Operate_Type::O_SET, 0);
	battle_status_ = DIE_OVER;
	check_dead_and_broadcast(*this, 0);
	MSG_DEBUG("fighter_die in time, role_id=%ld", role_id());
}
