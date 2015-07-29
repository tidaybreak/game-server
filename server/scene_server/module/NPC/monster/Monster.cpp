/*
 * Monster.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */


#include "Move_Scene.h"
#include "Monster.h"
#include "NPC/NPC_Manager.h"
#include "ai_machine/AI_Machine.h"
#include "Scene_Manager.h"
#include "Config_Cache_NPC.h"
#include "Msg_Active_Struct.h"
#include "Battle_Scene.h"
#include "team/Scene_Team_Manager.h"
#include "Scene_Monitor.h"
#include "Scene_Player.h"
#include "Monster_Def.h"
#include "team/Team_Func.h"
#include "Scene_Public.h"
#include "Config_Cache_Expedition.h"

const int MIN_MOVE_GRID_NUM = 3;			//开始移动的步数

Monster::Monster(void){
	reset();
}

Monster::~Monster(void) {

}

int Monster::birth(role_id_t npc_id, int npc_type_id, int operate_type, Move_Scene* move_scene, NPC_Record *record, NPC_Addition_Info &npc_addition_info) {

	if (!move_scene) {
		MSG_USER("monster birth move scene null");
		return -1;
	}

//	NPC_Record::NPC_Type_Map::iterator npc_it = record->npc_type_map.find(npc_type_id);
//	if (npc_it == record->npc_type_map.end()) {
//		MSG_USER("Monster:%d not in record", npc_type_id);
//		return -1;
//	}

	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
		MSG_USER("Monster:% birth error coord:%d %d", npc_type_id, npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
		return -1;
	}

//	if (!npc_addition_info.unmoveable_birth && record->scene->mpt_value_by_coord(grid_x_to_pixel(npc_addition_info.birth_coord.x), grid_y_to_pixel(npc_addition_info.birth_coord.y)) < Move_Scene::MOVE_ABLE_FROM) {
//		MSG_USER("Monster:%d birth error coord:%d %d error", npc_type_id, npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return -1;
//	}

	npc_detail_.npc_id = npc_id;
	int ret = this->init(npc_type_id, operate_type, move_scene, record, npc_addition_info);
	if (ret != 0) {
		reset();
		return -1;
	}

//	if (ai_config().tick_time <= Time_Value::zero) {
//		reset();
//		MSG_USER("AI: monster:%d tick error.", npc_type_id);
//		return -1;
//	}

//	if (monster_detail_.current_skill == NULL) {
//		reset();
//		MSG_USER("AI: monster:%d skill error.", npc_type_id);
//		return -1;
//	}
//
//	if (npc_it->second.camps > 0) {
//		fighter_detail().camps = npc_it->second.camps;
//	}



	enter_move_scene(move_scene);
	move_appear();

	//insert_default_buff();
	//appear_broadcasting();

//	this->del_addition(0);

	// ai init
	if(npc_detail().ai_id == 2){
		behavior_func = &Monster::update_move_behavior;
	}else{
		if(npc_detail().chat_vec.empty()){
			behavior_func = &Monster::update_null_behavior;
		}
		else{
			behavior_func = &Monster::update_idle_behavior;
		}
	}

	//this->init_bt();
	//this->start_ai();

	return 0;
}

int Monster::init(int npc_type_id, int operate_type, Move_Scene* move_scene, NPC_Record *record, NPC_Addition_Info &npc_addition_info) {
	const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(npc_type_id);
	if (config_cache == 0) {
		MSG_USER("find npc config:%d error", npc_type_id);
		return -1;
	}

	monster_detail_.config_cache = config_cache;

//	int default_skill = npc_addition_info.default_skill ? npc_addition_info.default_skill : config_cache->default_skill;
//	if (set_skill_detail(default_skill)) {
//		MSG_USER("set skill:%d error, npc:%d", default_skill, npc_type_id);
//		return -1;
//	}
//	add_ext_skill(npc_addition_info.ext_skill_set);

	/// npc base init
	npc_record_ = record;
//	npc_detail_.direct_master = npc_addition_info.identify.master_id;
//	npc_detail_.master_master = npc_addition_info.identify.final_master;
	npc_detail_.title_id = config_cache->title_id;
	npc_detail_.birth_by_skill = npc_addition_info.birth_by_skill;
	npc_detail_.npc_status = NORMAL_STATUS;
	npc_detail_.birth_time = Time_Value::gettimeofday();
	npc_detail_.npc_type_id = npc_type_id;
	npc_detail_.operation_type = operate_type;
	npc_detail_.is_ai_start = true;
	npc_detail_.birth_coord = npc_addition_info.birth_coord;
	//npc_detail_.life_time = (npc_detail_.ai_config->life_time == Time_Value::zero) ? Time_Value(INT_MAX, 0) : Time_Value::gettimeofday() + npc_detail_.ai_config->life_time;
	npc_detail_.ai_running_info.tick_vision_.set_interval(Time_Value(0,50000));
	npc_detail_.ai_running_info.tick_patrol_.set_interval(Time_Value(0,100000));
	if(record){
		npc_detail_.ai_id = record->config.ia;
		if(record->birth_by != 0){
			npc_detail_.add_id.push_back(record->birth_by);
		}
		set_move_speed(record->config.speed[0]);
		npc_detail_.state_type = record->config.state_type;
	}


	/// monster base init
	monster_detail_.name = npc_addition_info.name.empty() ? config_cache->name : npc_addition_info.name;
//	monster_detail_.additional_id.push_back(master_id());
//	for (Role_Vec::iterator it = npc_addition_info.additional_id.begin(); it != npc_addition_info.additional_id.end(); ++it) {
//		monster_detail_.additional_id.push_back(*it);
//	}

	monster_detail_.jid = npc_addition_info.jid ? npc_addition_info.jid : config_cache->jid;
	if(record){
		monster_detail_.team_hatred_type = record->get_team_hatred_type(npc_type_id);
		monster_detail_.speed[0] = record->config.speed[0];
		monster_detail_.speed[1] = record->config.speed[1];
		monster_detail_.speed[2] = record->config.speed[2];
	}

	/// monster mover init
	Mover::set_detail_prt(npc_detail_.mover_detail_);
//	set_scene_pointer(record->scene);
//	mover_detail().scene_id = record->scene->scene_id();
//	mover_detail().mover_id = npc_id();
	//mover_detail().toward = npc_addition_info.toward > 0 ? npc_addition_info.toward : get_rand_in_range(Move_Scene::toward_up, Move_Scene::toward_left_up);
	int toward = npc_addition_info.toward > 0 ? npc_addition_info.toward : Move_Scene::toward_up;
	this->set_toward(toward);
	this->set_grid_coord(npc_detail().birth_coord.grid_coord().x, npc_detail().birth_coord.grid_coord().y, true);
	this->set_pixel_coord(npc_detail().birth_coord.pixel_coord().x, npc_detail().birth_coord.pixel_coord().y);
	this->retify_gauge_grid_coord();

	npc_detail_.mover_detail_.mover_type = (Mover_Type)config_cache->type;
	npc_detail_.fighter_detail_.level = config_cache->level;
	if(record){
		set_layer_id(record->layer_id);
	}
//	addition_info_ = npc_addition_info.additional_info;

//	Role_Info_Addition addi;
//	for (Int_Vec::const_iterator iter = config_cache->addition_vec.begin(); iter != config_cache->addition_vec.end(); ++iter) {
//		addi.reset();
//		addi.type = *iter;
//		addition_info_.push_back(addi);
//	}
//	/// monster status
//	status_detail_.role_id = npc_id();
//	set_invincible_attrib(config_cache->invincible);

	/// monster fighter init
	Fighter::set_detail_prt(npc_detail_.fighter_detail_);
//	fighter_detail().role_id = npc_id();
//	fighter_detail().camps = npc_addition_info.identify.camps ? npc_addition_info.identify.camps : config_cache->camps;
//	fighter_detail().speed_map.insert(std::make_pair(0, config_cache->speed));
//	fighter_detail().level = npc_addition_info.level ? npc_addition_info.level : config_cache->level;
//	fighter_detail().country = npc_addition_info.identify.country;
//	fighter_detail().experience = config_cache->experience;
//	fighter_detail().fight_mode = npc_addition_info.identify.fight_mode;
//	fighter_detail().property = npc_addition_info.property.empty() ? config_cache->property : npc_addition_info.property;
//	refresh_move_interval();

//	for (Int_Set::iterator it = config_cache->trigger_set.begin(); it != config_cache->trigger_set.end(); ++it) {
//		AI_Action_Running_Info running_info = AI_Action_Running_Info((*it));
//		if (running_info.action_config) ai_action_running_.condition_action_vec.push_back(running_info);
//	}

//	die_by_attack_nums_ = config_cache->die_by_attack_nums;

//	if (config_cache->appear_effect >= 0) {
//		Role_Info_Addition addition_info;
//		addition_info.int_value1 = config_cache->appear_effect;
//		this->set_addition(addition_info);
//	}

//	if (master_id()) {
//		Fighter *fighter = this->find_fighter_in_same_scene(master_id());
//		if (fighter && fighter->master_id() && fighter->fighter_id() != fighter->master_id()) {
//			fighter_detail().unattackable_ids.insert(fighter->master_id());
//		}
//	}

	return 0;
}

int Monster::battle_birth(role_id_t npc_id, int npc_type_id, NPC_Record *record, NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene) {

	if (battle_scene == NULL) {
		MSG_USER("monster birth parm error");
		return -1;
	}

	npc_detail_.npc_id = npc_id;
	int ret = this->battle_init(npc_type_id, record, npc_addition_info, battle_scene);
	if (ret != 0) {
		return -1;
	}
	for (Prop_Setter_Vec::iterator it = npc_addition_info.prop_vec.begin();
			it != npc_addition_info.prop_vec.end(); ++it) {
		if (!it->percent > 0) {
			it->percent = 0;
			continue;
		}
		double value = this->fighter_detail().fetch_fight_property(it->prop_type);
		it->basic = value * it->percent;
		it->percent = 0;
	}
	if (0 != this->fighter() && npc_addition_info.prop_vec.size() > 0) {
		//this->fighter()->fight_modify_fight_prop(npc_addition_info.prop_vec, false);
		fighter()->modify_props_normal(npc_addition_info.prop_vec, false, 14);
	}
	if (0 != this->fighter() && battle_scene->get_other_info().type == OT_EXPEDITION) {
		const Expedition_World_Level_Monster_Modify *ewlmm = CONFIG_CACHE_EXPEDITION->get_monster_modify_by_world_level(SCENE_PUBLIC->world_level());
		if (ewlmm) {
			Prop_Setter_Vec prop_vec;
			for (int i = 0; i < WORLD_LEVEL_MONSTER_MODIFY; ++i) {
				Prop_Setter ps;
				int val = ewlmm->modify[i];
				switch(i) {
				case 0: {
					ps.prop_type = PT_PHY_ATTACK;
					break;
				}
				case 1: {
					ps.prop_type = PT_SPELL_ATTACK;
					break;
				}
				case 2: {
					ps.prop_type = PT_PHY_DEFENSE;
					break;
				}
				case 3: {
					ps.prop_type = PT_SPELL_DEFENSE;
					break;
				}
				case 4: {
					ps.prop_type = PT_BLOOD_MAX;
					break;
				}
				default:
					break;
				}
				if (ps.prop_type != PT_START) {
					ps.addi_type = AT_BASIC;
					ps.operate = O_SET;
					ps.basic = fighter()->fighter_detail().fetch_fight_property(ps.prop_type, Prop_Value::ELEM_FIGHT);
					ps.basic *= (val / 100.0);
					prop_vec.push_back(ps);
				}
			}
			if (!prop_vec.empty()) {
				fighter()->modify_props_normal(prop_vec, false, 14);
			}
		}
	}

	if(npc_addition_info.birth_coord.x < 0 || npc_addition_info.birth_coord.x > 1 ||
			npc_addition_info.birth_coord.y < 0 || npc_addition_info.birth_coord.y > 12){
		MSG_TRACE_ABORT("create battle_monster positon error: role_id = %ld, scene:%d, battle id:%d, monster_type_id: %d", role_id(), move_scene_id(), battle_scene_id(), npc_type_id);
		return -1;
	}
	if(set_battle_position(npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y, battle_scene) == 0){
		if (enter_battle_scene(battle_scene) == 0){
			battle_appear();
		}
	}
	return 0;
}

int Monster::battle_init(int npc_type_id, NPC_Record *record, NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene) {
	const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(npc_type_id);
	if (config_cache == 0) {
		MSG_USER("find npc config:%d error", npc_type_id);
		return -1;
	}
	monster_detail_.config_cache = config_cache;

//	int default_skill = npc_addition_info.default_skill ? npc_addition_info.default_skill : config_cache->default_skill;
//	if (set_skill_detail(default_skill)) {
//		MSG_USER("set skill:%d error, npc:%d", default_skill, npc_type_id);
//		return -1;
//	}
//	add_ext_skill(npc_addition_info.ext_skill_set);

	/// npc base init
	npc_record_ = record;
//	npc_detail_.direct_master = npc_addition_info.identify.master_id;
//	npc_detail_.master_master = npc_addition_info.identify.final_master;
	npc_detail_.birth_by_skill = npc_addition_info.birth_by_skill;
	if(npc_detail_.birth_by_skill != 0){
		set_summon_monster_flag();
	}
	npc_detail_.npc_status = NORMAL_STATUS;
	//npc_detail_.birth_time = Time_Value::gettimeofday();
	npc_detail_.npc_type_id = npc_type_id;
	if(record){
		npc_detail_.state_type = record->config.state_type;
	}
//	npc_detail_.birth_coord = npc_addition_info.birth_coord;

	/// monster base init
	monster_detail_.name = npc_addition_info.name.empty() ? config_cache->name : npc_addition_info.name;
	monster_detail_.jid = npc_addition_info.jid ? npc_addition_info.jid : config_cache->jid;
//	monster_detail_.additional_id.push_back(master_id());
//	for (Role_Vec::iterator it = npc_addition_info.additional_id.begin(); it != npc_addition_info.additional_id.end(); ++it) {
//		monster_detail_.additional_id.push_back(*it);
//	}

	/// monster mover init
	npc_detail_.mover_detail_.mover_type = (Mover_Type)config_cache->type;
	Mover::set_detail_prt(npc_detail_.mover_detail_);

	/// monster fighter init
	Fighter::set_detail_prt(npc_detail_.fighter_detail_);
	npc_detail_.fighter_detail_.role_id = npc_id();
	//prop_value
	for (std::vector<Int_Double> ::const_iterator it = monster_detail_.config_cache->prop_value_vec.begin();
			it != monster_detail_.config_cache->prop_value_vec.end(); ++it)
	{
		Property_Type prop_type = Property_Type((*it).id);
		npc_detail_.fighter_detail_.modify_basic_property(prop_type, Addition_Type::AT_BASIC, Operate_Type::O_SET, (*it).value);
		if(prop_type == PT_BLOOD_MAX)
		{
			int cur_blood = (*it).value;
			if(record && record->is_share_hp()){
				cur_blood = record->share_hp_cur;
				int max_blood = record->share_hp_max;
				npc_detail_.fighter_detail_.modify_basic_property(PT_BLOOD_MAX, Addition_Type::AT_BASIC, Operate_Type::O_SET, max_blood);
			}
			npc_detail_.fighter_detail_.modify_basic_property(PT_BLOOD_CURRENT, Addition_Type::AT_BASIC, Operate_Type::O_SET, cur_blood);
		}
	}



	Talent talent;
	talent.talent_id = 1;
	// passive skill
	int passive_skill_count = config_cache->passive_skills.size();
	for(int i = 0; i < passive_skill_count; ++i){
		Skill_DB_Info skill_info;
		skill_info.skill_id = config_cache->passive_skills[i];
		skill_info.skill_level = 1;
		skill_info.skill_loc = 1;
		talent.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
	}
	//skill
	int skill_count = config_cache->skill_vec.size();
	if(skill_count > 0)
	{
		npc_detail_.fighter_detail_.default_skill = config_cache->skill_vec[0];
		Skill_DB_Info skill_info;
		for(int i=0; i<skill_count; ++i)
		{
			skill_info.skill_id = config_cache->skill_vec[i];
			skill_info.skill_level = 1;
			skill_info.skill_loc = 1;
			talent.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
		}
	}

	npc_detail_.fighter_detail_.talent_map[talent.talent_id] = talent;
	npc_detail_.fighter_detail_.cur_used_talent_ = talent.talent_id;
	npc_detail_.fighter_detail_.set_be_skill_attr();
	npc_detail_.fighter_detail_.level = config_cache->level;

//	for (Int_Set::iterator it = config_cache->trigger_set.begin(); it != config_cache->trigger_set.end(); ++it) {
//		AI_Action_Running_Info running_info = AI_Action_Running_Info((*it));
//		if (running_info.action_config) ai_action_running_.condition_action_vec.push_back(running_info);
//	}
	return 0;
}

int Monster::move_appear_info(Block_Buffer &buf) {
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	MSG_81000005 msg;
	msg.npc_info.role_id = role_id();
	msg.npc_info.npc_type = npc_detail().npc_type_id;
	msg.npc_info.npc_jid = monster_detail().jid;
	msg.npc_info.type = this->npc_operation_type();
	msg.npc_info.role_name = monster_detail_.name;
	msg.npc_info.title_id = npc_detail().title_id;
	msg.npc_info.coord = this->pixel_coord();
	msg.npc_info.hp_curr = 1000;
	msg.npc_info.hp_max = 1000;
	msg.npc_info.add_id = npc_detail().add_id;

	if(mover_type() == MOVER_T_MONSTER_WAR_LIGHT_SRC ||
			mover_type() == MOVER_T_MONSTER_WAR_LIGHT_GUARD ||
			mover_type() == MOVER_T_MONSTER_WAR_LIGHT_REVIVE){
		msg.npc_info.tmp_camp = 1;
	}else if(mover_type() == MOVER_T_MONSTER_WAR_DARK_SRC ||
			mover_type() == MOVER_T_MONSTER_WAR_DARK_GUARD ||
			mover_type() == MOVER_T_MONSTER_WAR_DARK_REVIVE){
		msg.npc_info.tmp_camp = 2;
	}

	msg.npc_info.speed = get_move_speed();
	//if (0 != this->battle_scene()) {
	if (this->npc_status() == FIGHT_STATUS) {
		msg.npc_info.state = 1;
	}
	msg.npc_info.addition_prop = addition_prop();
	buf.make_message(msg.msg_id);
	msg.serialize(buf);

	// refill len in head
	wr_end_idx = buf.get_write_idx();
	len = wr_end_idx - wr_begin_idx - sizeof(uint32_t);
	buf.set_write_idx(wr_begin_idx);
	buf.write_uint32(len);
	buf.set_write_idx(wr_end_idx);

	return 0;
}

int Monster::make_move_info(Block_Buffer &buf, uint8_t toward, uint16_t x, uint16_t y)
{
	MSG_81000020 msg;
	msg.role_id = role_id();

	msg.coord.push_back(Coord(x, y));
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	return 0;
}

bool Monster::get_first_time_use_skill(void) {
	if (first_time_use_skill_) {
		first_time_use_skill_ = false;
		return true;
	}
	return first_time_use_skill_;
}

int Monster::battle_appear_info(Block_Buffer &buf) {
	// size_t rd_idx = buf.get_read_idx();
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	// fill head
	MSG_81000008 msg;
	msg.fighter_info.role_id = role_id();
	msg.fighter_info.role_name = role_name();
	msg.fighter_info.type = (int)mover_detail().mover_type;
	msg.fighter_info.role_type = npc_detail().npc_type_id;
	msg.fighter_info.role_jid = monster_detail().jid;
	msg.fighter_info.position.val_1 = mover_detail().battle_position.pos;
	msg.fighter_info.position.val_2 = mover_detail().battle_position.idx;
	msg.fighter_info.hp_curr = blood_current();
	msg.fighter_info.hp_max = blood_max();
	if(is_summon_monster_){// 召唤的怪物，fight_state设置为2
		msg.fighter_info.fight_state = 2;
	}
//	Int_Int model;
//	model.val_1 = monster_detail_.jid;
//	msg.fighter_info.avatar.push_back(model);
	buf.make_message(msg.msg_id);
	msg.serialize(buf);

	// refill len in head
	wr_end_idx = buf.get_write_idx();
	len = wr_end_idx - wr_begin_idx - sizeof(uint32_t);
	buf.set_write_idx(wr_begin_idx);
	buf.write_uint32(len);
	buf.set_write_idx(wr_end_idx);

	return 0;
}

int Monster::battle_appear(void) {
	NPC::battle_appear();

	return 0;
}

int Monster::battle_disappear(void) {
	NPC::battle_disappear();

	return 0;
}

int Monster::enter_battle_scene(Battle_Scene *scene) {
	if(scene == NULL){
		return -1;
	}
	if (this->battle_scene()) {
		MSG_TRACE_ABORT("has battle scene, role:%ld battle_id:%d, b_id:%d", role_id(), battle_scene_id(), scene->battle_id());
		return -1;
	}

	NPC::enter_battle_scene(scene);

	first_time_use_skill_ = true;
	return 0;
}

int Monster::exit_battle_scene(int exit_from) {
	if(recover_state()){
		return -1;
	}
	NPC::exit_battle_scene();
	first_time_use_skill_ = true;
	this->set_npc_status(RECOVER_STATUS);
	return 0;
}

int Monster::die_process(const Attack_Param &info) {
	NPC::die_process(info);
	if(this->battle_scene()){
		this->battle_scene()->listen_die(this->fighter());
	}
	this->set_npc_status(NPC::DEAD_STATUS);
	return 0;
}


void Monster::set_npc_status(int npc_status) {
	if (npc_detail_.npc_status == RECOVER_STATUS) {
		MSG_USER_TRACE("ERROR_9 error npc status");
		return ;
	}
	if(npc_status == RECOVER_STATUS){
		if(this->validate_battle_scene()){
			MSG_TRACE_ABORT("set_npc_status error: role_id = %ld, scene:%d, battle id:%d", role_id(), move_scene_id(), battle_scene_id());
			return;
		}
	}

	int old_status = npc_detail_.npc_status;

	npc_detail_.npc_status = npc_status;
	if(npc_status != RECOVER_STATUS && this->move_scene()){
		int speed = this->monster_detail_.speed[0];
		if(npc_status == NPC::CHASE_STATUS){
			speed = this->monster_detail_.speed[1];
		}else if(npc_status == NPC::BACK_STATUS){
			speed = this->monster_detail_.speed[2];
		}
		if(speed != this->get_move_speed()){
			this->set_move_speed(speed);
			//to client change
			Block_Buffer buf;
			MSG_81000021 msg;
			msg.role_id = this->role_id();
			msg.speed = speed;
			MAKE_MSG_BUF(msg, buf);
			this->scene_broad_around(buf);
		}

		if (old_status == FIGHT_STATUS || npc_status == FIGHT_STATUS) {
			MSG_81000026 state_msg;
			state_msg.role_id = role_id();
			if (npc_status == FIGHT_STATUS) {
				state_msg.state = 1;
			} else {
				state_msg.state = 0;
			}

			Block_Buffer buf;
			MAKE_MSG_BUF(state_msg, buf);
			scene_broad_around(buf);
		}
	}
}


int Monster::ai_config_init(void) {
	walk_path_.walk_path = npc_record()->config.walk_path;

	return 0;
}

int Monster::fini(void) {
	NPC::fini();

	return 0;
}

void Monster::reset(void) {
	NPC::reset();
	walk_path_.reset();
	grid_coord_vec_.clear();
	monster_detail_.reset();
	first_time_use_skill_ = true;
	behavior_func = NULL;
	vision_tick_count_ = 0;
	is_summon_monster_ = false;
	param_1_ = 0;
	param_2_ = 0;
	param_3_ = 0;
}

void Monster::sing_end(const Time_Value &now, int skill_id){
	if(battle_scene()){
		battle_scene()->listen_skill_sing_end(this, skill_id);
	}
}

std::string Monster::role_name(void)
{
	return monster_detail_.name;
}

int Monster::tick(Time_Value &now) {
	if(recover_state() || this->battle_scene() != NULL || npc_record() == NULL)
	{
		return 0;
	}
	gauge_grid_coord_tick(now);
	if(npc_detail_.check_ai_tick){
		if(now > npc_detail_.ai_tick_time){
			(this->*behavior_func)();
			this->pop_watcher_tick();
		}
	}else{
		(this->*behavior_func)();
	}
	if(vision_tick_count_ > 0){
		--vision_tick_count_;
	}else{
		vision_tick();
		vision_tick_count_ = 5;
	}

	return 0;
}

int Monster::recover_self(void) {
//	if (monster_detail_.config_cache && monster_detail_.config_cache->disappear_effect >= 0) {
//		mover_detail().disappear_info.clear();
//		mover_detail().disappear_info.push_back(10);
//		mover_detail().disappear_info.push_back(monster_detail_.config_cache->disappear_effect);
//	}
//
//	NPC::recover_self();

	return 0;
}

int Monster::vision_tick(void) {
	if(npc_record() == NULL){
		return 0;
	}
	if(this->npc_operation_type() != 1){
		return 0;
	}
	if(npc_status() == CHASE_STATUS){
		return 0;
	}
	if(npc_status() == FIGHT_STATUS || npc_status() == PLOT_STATUS){
		if(npc_record()->is_teams_up() && npc_self()->npc_record()->battle_scene){//组队打怪
			for (Fighter_Set::iterator it = npc_record()->fight_player_set.begin();
					it != npc_record()->fight_player_set.end(); ++it) {
				if (!(*it)->mover_detail().is_player()) {
					continue;
				}
				Map_Team* team = SCENE_TEAM_MANAGER->get_team((*it)->player_self()->get_team_id());
				if (NULL == team) {
					continue;
				}
				int idx = Position_Key::LINE_ONE_FRONT;
				Role_Hash_Set members;
				team->team_all_member_set(members);
				for(Role_Hash_Set::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
					role_id_t id = *iter;
					if (id == (*it)->role_id()) {
						continue;
					}
					Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(id);
					if (!player || player->is_in_battle()
							|| !is_in_range(player->grid_coord(), this->gauge_grid_coord_, FIGHT_DISTANCE)
							) {
						continue;
					}
					int pos = 0;
					Int_Hash_Set idx_set;
					{
						for (Battle_Scene::Debug_Mover_Map::const_iterator it = npc_self()->npc_record()->battle_scene->pos_debug_map().begin();
								it != npc_self()->npc_record()->battle_scene->pos_debug_map().end(); ++it) {
							if (it->first.pos != pos) continue;
							idx_set.insert(it->first.idx);
						}
						int ret = ensure_battle_pos_unique(idx_set);
						if (0 != ret) {
							continue;
						}
						if (0 != player->team_position().idx) {
							idx = player->team_position().idx;
						}
						get_empty_pos_check_idx(idx_set, idx);
						if (-1 == idx) {
							continue;
						}
					}
					this->npc_record()->join_battle(player->fighter(), pos, idx);
					idx_set.insert(idx);
				}
			}
		}else{
			return 0;
		}
	}else{
		if(npc_status() == BACK_STATUS){
			Scene_Player_Set player_set;
			move_scene()->collect_round_fight_player(mover_detail().layer_id, gauge_grid_coord_, player_set, FIGHT_DISTANCE);
			for (Scene_Player_Set::iterator iter = player_set.begin(); iter != player_set.end(); ++iter)
			{
				//enter battle
				if(this->can_attack(*iter))
				{
					this->npc_record()->enter_battle(*iter);
					(*iter)->team_fb_create_player_monster();
					return 0;
				}
			}
		}else{
			if (npc_record()->config.view_range > 0){
				Scene_Player_Set player_set;
				move_scene()->collect_round_fight_player(mover_detail().layer_id, this->gauge_grid_coord_,
						player_set, npc_record()->config.view_range);
				for (Scene_Player_Set::iterator iter = player_set.begin(); iter != player_set.end(); ++iter)
				{
					if(this->can_attack(*iter))
					{
						this->npc_record()->set_chase_target((*iter)->role_id());
						return 0;
					}
				}
				//npc_detail_.ai_running_info.tick_vision_.set_tick_now();
			}
			else
			{
				Scene_Player_Set player_set;
				move_scene()->collect_round_fight_player(mover_detail().layer_id, this->gauge_grid_coord_, player_set, FIGHT_DISTANCE);
				for (Scene_Player_Set::iterator iter = player_set.begin(); iter != player_set.end(); ++iter)
				{
					//enter battle
					if(this->can_attack(*iter))
					{
						this->npc_record()->enter_battle(*iter);
						(*iter)->team_fb_create_player_monster();
						return 0;
					}
				}
			}
		}
	}

	return 0;
}


//
//int Monster::die_process(const Attack_Info &info, Fighter &killer) {
//	if (npc_status() == RECOVER_STATUS || npc_status() == DEAD_STATUS) return -1;
//	NPC::die_process(info, killer);
//
//	tick_walk_.clear();
//
//	// notice logic server
//	notice_logic_die(info.attacker);
//
//	// clean ai tick 为后续正常触发
//	set_monster_status(Monster::DIE_STATUS);
//	Time_Value time_zero = Time_Value::zero;
//	set_ai_tick(time_zero);
//
//	// calculate hatred
//	calculate_hatred();
//
//	// die drop
//	monster_drop(info.attacker);
//
//	// die exp
//	monster_exp();
//
//	return Fighter::die_process(info, killer);
//}

int Monster::notice_logic_die(role_id_t attacker) {
//	if (mover_type() > MOVER_MONSTER_BOSS) {
//		return 0;
//	}
//
//	role_id_t killer_id = find_direct_master_id(attacker);
//	if (killer_id == 0) return -1;
//
//	MSG_700015 inner_msg;
//	inner_msg.role_id = killer_id;
//	inner_msg.npc_type_id = npc_self()->npc_type_id();
//	inner_msg.scene = npc_self()->mover_detail().scene_id;
//	if (npc_self()->ai_config().die_share) {
//		for (Hatred_Map::iterator iter = hatred_map_.begin(); iter != hatred_map_.end(); ++iter) {
//			killer_id = find_direct_master_id(iter->first);
//			if (killer_id == 0) continue;
//			Role_Vec::iterator find_it = std::find(inner_msg.kill_roles.begin(), inner_msg.kill_roles.end(), killer_id);
//			if (find_it != inner_msg.kill_roles.end()) continue;
//			inner_msg.kill_roles.push_back(killer_id);
//		}
//	} else {
//		inner_msg.kill_roles.push_back(inner_msg.role_id);
//	}
//	Block_Buffer inner_buf;
//	inner_buf.make_message(INNER_KILL_NPC);
//	inner_msg.serialize(inner_buf);
//	inner_buf.finish_message();
//	MAP_MANAGER_INSTANCE->send_to_logic_server(inner_buf);

	return 0;
}

bool Monster::is_monster(void) {
	return true;
}

bool Monster::is_special_monser(void) {
	return false;
}

Monster *Monster::monster_self(void) {
	return this;
}

int Monster::talk(Block_Buffer &buf) {
	this->scene_broad_around(buf, SCENE_MOVE);
	return 0;
}

int Monster::change_self(int jid) {
	monster_detail_.jid = jid;
//	notify_fight_info(UPDATE_NPC_JID, jid, true, VIEW_DISABLE);

	return 0;
}

/*
40 *300 1:27.4 137.29977116704805491990846681922  0.29133333333333333333333333333334

300*300 1:31.6 146.46733477072858273422534948021  0.30533333333333333333333333333334

const int X_TIME = 200 * 291333;
const int Y_TIME = 200 * 145666;
const int X_Y_TIME = 200 * 305333;
 */

int Monster::move_watcher_tick(Coord &coord) {
	int toward = move_scene()->calculate_toward_by_grid_coord(mover_detail().grid_coord.x, mover_detail().grid_coord.y, coord.x, coord.y);
	if (toward == Move_Scene::toward_left || toward == Move_Scene::toward_right) {
		if (move_interval[0] != Time_Value::zero)
			push_watcher_tick(move_interval[0]);
		else
			push_watcher_tick(0, 500000);
	} else if (toward == Move_Scene::toward_up || toward == Move_Scene::toward_down) {
		if (move_interval[1] != Time_Value::zero)
			push_watcher_tick(move_interval[1]);
		else
			push_watcher_tick(0, 500000);
	} else {
		if (move_interval[2] != Time_Value::zero)
			push_watcher_tick(move_interval[2]);
		else
			push_watcher_tick(0, 500000);
	}
	return 0;
}


int Monster::push_walk_path(const Coord &coord) {
	walk_path_.reset();
	walk_path_.walk_path.push_back(coord);

	return 0;
}

void Monster::teammate_move(Coord& target_coord)
{
	if(teammate_count_ > 0)
	{
		for(int i=0; i<MAX_TEAMMATE_NUM; ++i)
		{
			if(teammate_id_[i] != 0)
			{
				Monster* monster_team = NPC_MANAGER->find_monster(teammate_id_[i]);
				if(monster_team)
				{
					monster_team->fallow_move_from_leader(target_coord);
				}
			}
		}
	}
}

void Monster::fallow_move_from_leader(Coord& target_coord)
{
	Coord target;
	get_team_coord(target_coord, target);
	walk_path().clean_walk_coord();
	path_find(grid_coord(), target, walk_path().walk_coord, false);
	walk_path().curr_coord_index = walk_path().walk_coord.size();
}

int Monster::ai_select_skill(void){
	if(this->battle_scene()){
		return battle_scene()->ai_select_skill(this);
	}
	return 0;
}

void Monster::be_hurt_and_sub_blood(double sub_blood, double current_blood, role_id_t attacker_id){
	int point_id = 0;
	if(npc_record()){
		point_id = npc_record()->config.id;
		npc_record()->be_hurt_and_sub_blood(sub_blood, current_blood, attacker_id);
	}
	if(battle_scene() && battle_scene()->move_scene() && is_boss()){
		battle_scene()->move_scene()->be_hurt_and_sub_blood(battle_scene(), sub_blood, current_blood, attacker_id, point_id);
	}
}

void Monster::set_chase_target(role_id_t role_id){
	if(move_scene() == NULL){
		return;
	}
	npc_detail().chase_coord = this->gauge_grid_coord();
	move_scene()->mover_change_coord(this, this->gauge_grid_coord());
	this->retify_gauge_grid_coord();
	this->most_hatred_role().role_id = role_id;
	this->set_npc_status(NPC::CHASE_STATUS);
	int talk_id = get_talk_id();
	push_watcher_tick(0, 700000);
	//talk
	Block_Buffer buf;
	MSG_81000018 msg;
	msg.role_id = this->role_id();
	msg.talk_id = talk_id;
	MAKE_MSG_BUF(msg, buf);
	this->scene_broad_around(buf, SCENE_MOVE);
}

void Monster::clear_chase_target(void){
	this->most_hatred_role().role_id = 0;
	this->set_npc_status(NPC::NORMAL_STATUS);
}

int Monster::play_chat(void){
	int chat_id = this->get_chat_id();
	if(chat_id != 0){
		Block_Buffer buf;
		MSG_81000205 msg;
		msg.role_id = role_id();
		msg.talk_id = chat_id;
		msg.talk_time = 2000;
		MAKE_MSG_BUF(msg, buf);
		this->scene_broad_around(buf, SCENE_MOVE);
		return 0;
	}
	return -1;
}


int Monster::teammate_play_chat(void)
{
	if(teammate_count_ > 0)
	{
		for(int i=0; i<MAX_TEAMMATE_NUM; ++i)
		{
			if(teammate_id_[i] != 0)
			{
				Monster* monster_team = NPC_MANAGER->find_monster(teammate_id_[i]);
				if(monster_team)
				{
					monster_team->play_chat();
				}
			}
		}
	}
	return 0;
}

bool Monster::can_attack(Scene_Player* player){
	if(!player){
		return false;
	}
	if(player->is_revive()){
		return false;
	}
	if(this->mover_type() == MOVER_T_MONSTER_WAR_LIGHT_SRC ||
			this->mover_type() == MOVER_T_MONSTER_WAR_LIGHT_GUARD ||
			this->mover_type() == MOVER_T_MONSTER_WAR_LIGHT_REVIVE){
		if(player->war_info()->league == 1){
			return false;
		}
	}
	if(this->mover_type() == MOVER_T_MONSTER_WAR_DARK_SRC ||
			this->mover_type() == MOVER_T_MONSTER_WAR_DARK_GUARD||
			this->mover_type() == MOVER_T_MONSTER_WAR_DARK_REVIVE){
		if(player->war_info()->league == 2){
			return false;
		}
	}
	return true;
}

bool Monster::is_same_toward(int t1, int t2){
	if(t1 == 0 || t2 == 0){
		return false;
	}
	if(t1 == 1){
		if(t2 == 2 || t2 == 8){
			return true;
		}
	}else if(t1 == 8){
		if(t2 == 1 || t2 == 7){
			return true;
		}
	}else{
		if(t2 == t1+1 || t2 == t1-1){
			return true;
		}
	}
	return false;
}

int Monster::ai_transform(int transform_type, int transform_id){
	return 0;
}

int Monster::update_chase_mode(void){
	Mover* player = this->move_scene()->find_mover_with_layer(this->mover_detail().layer_id,
			this->most_hatred_role().role_id);
	if(player == NULL || (player && player->is_in_battle())){
		if(this->get_gauge_coord_num() > 0){
			return k_BRS_SUCCESS;
		}
		//back
		this->set_npc_status(NPC::BACK_STATUS);
		this->most_hatred_role().role_id = 0;
		this->walk_path().clean_walk_coord();
		this->walk_path().reset();
		this->walk_path().walk_path = this->npc_record()->config.walk_path;
		this->path_find(this->grid_coord(), this->npc_detail().chase_coord, this->walk_path().walk_coord, false);
		this->walk_path().curr_coord_index = this->walk_path().walk_coord.size();
		if (this->walk_path().curr_coord_index > 0)
		{
			int remain_move_num = this->get_gauge_coord_num();
			int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
			Coord_Vec coord_vec;
			if(move_num > this->walk_path().curr_coord_index){
				move_num = this->walk_path().curr_coord_index;
			}
			for(int i = 0; i < move_num; ++i){
				Coord coord;
				this->walk_path().get_next_coord(coord);
				coord_vec.push_back(coord.pixel_coord());
			}
			if(move_num > 0){
				this->move_inner_vec(coord_vec);
				this->push_watcher_tick(0, 100000);
			}
		}else{
			this->push_watcher_tick(1);
		}
		return k_BRS_SUCCESS;
	}
	if(player){
		if(player->can_fight() && this->can_attack(player->player_self())){
			// 追击时玩家朝向和怪物朝向一样时，检测距离+32，避免贴近也没触发战
			int fight_dis = FIGHT_DISTANCE;
			bool same_toward = is_same_toward(player->mover_detail().toward, mover_detail().toward);
			if(same_toward){
				fight_dis += 32;
			}
			if(is_in_range(player->mover_detail().grid_coord, this->gauge_grid_coord_, fight_dis))
			{
				this->npc_record()->enter_battle(player->player_self());
				player->player_self()->team_fb_create_player_monster();
				return k_BRS_SUCCESS;
			}
		}
	}
	//
	Coord target_coord = player->mover_detail().grid_coord;
	int distance = calculate_pixels_distance(this->grid_coord(), this->npc_detail().chase_coord);
	if(distance > this->npc_record()->config.chase_range){
		if(this->get_gauge_coord_num() > 0){
			return k_BRS_SUCCESS;
		}
		//back
		this->set_npc_status(NPC::BACK_STATUS);
		this->most_hatred_role().role_id = 0;
		this->walk_path().clean_walk_coord();
		this->walk_path().reset();
		this->walk_path().walk_path = this->npc_record()->config.walk_path;
		this->path_find(this->mover_detail().grid_coord, this->npc_detail().chase_coord, this->walk_path().walk_coord, false);
		this->walk_path().curr_coord_index = this->walk_path().walk_coord.size();
		if (this->walk_path().curr_coord_index > 0)
		{
			Coord_Vec coord_vec;
			int remain_move_num = this->get_gauge_coord_num();
			int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
			if(move_num > this->walk_path().curr_coord_index){
				move_num = this->walk_path().curr_coord_index;
			}
			for(int i = 0; i < move_num; ++i){
				Coord coord;
				this->walk_path().get_next_coord(coord);
				coord_vec.push_back(coord.pixel_coord());
			}
			if(move_num){
				this->move_watcher_tick(coord_vec[0]);
				this->move_inner_vec(coord_vec);
			}
		}else{
			this->push_watcher_tick(1);
		}
		return k_BRS_SUCCESS;
	}
	else
	{
		if(this->walk_path().has_walk_coord()){
			Coord coord;
			this->walk_path().get_next_coord(coord);
			this->move_watcher_tick(coord);
			if (this->move_inner(coord.x, coord.y) != 0) {
				if(this->walk_path().curr_coord_index > 0){
					this->walk_path().curr_coord_index++;
					this->walk_path().move_gride_count--;
				}
			}
			if(this->walk_path().move_gride_count >= 5){
				this->walk_path().clean_walk_coord();
			}
			return k_BRS_SUCCESS;
		}
		if(this->get_gauge_coord_num() > 2){
			return k_BRS_SUCCESS;
		}else{
			this->path_find(this->mover_detail().grid_coord, target_coord, this->walk_path().walk_coord, false);
			this->walk_path().curr_coord_index = this->walk_path().walk_coord.size();
			if (this->walk_path().curr_coord_index > 0)
			{
				Coord_Vec coord_vec;
				int remain_move_num = this->get_gauge_coord_num();
				int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
				if(move_num > this->walk_path().curr_coord_index){
					move_num = this->walk_path().curr_coord_index;
				}
				for(int i = 0; i < move_num; ++i){
					Coord coord;
					this->walk_path().get_next_coord(coord);
					coord_vec.push_back(coord.pixel_coord());
				}
				if(move_num > 0){
					this->move_watcher_tick(coord_vec[0]);
					this->move_inner_vec(coord_vec);
					//this->push_watcher_tick(0, 100000);
				}
			}
			return k_BRS_SUCCESS;
		}
	}
	return k_BRS_SUCCESS;
}

int Monster::update_back_mode(void){
	if(this->walk_path().has_walk_coord())
	{
		Coord coord;
		this->walk_path().get_next_coord(coord);
		this->move_watcher_tick(coord);
		if (this->move_inner(coord.x, coord.y) != 0) {
			if(this->walk_path().curr_coord_index > 0){
				this->walk_path().curr_coord_index++;
			}
		}
		return k_BRS_SUCCESS;
	}
	else
	{
		if(this->get_gauge_coord_num() > 0){
			return k_BRS_SUCCESS;
		}
		this->set_npc_status(NPC::NORMAL_STATUS);
		return k_BRS_SUCCESS;
	}
	return k_BRS_Failure;
}

int Monster::update_walk_mode(void){
	Monster* leader;
	if(this->get_leader_id() != 0 &&
			(leader = NPC_MANAGER->find_monster(this->get_leader_id())) != NULL)
	{
		if(this->walk_path().has_walk_coord())
		{
			Coord coord;

			this->walk_path().get_next_coord(coord);
			if (this->move_scene()->grid_mover_count(coord, this->mover_detail().layer_id) > 0) {
				this->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			this->move_watcher_tick(coord);
			if (this->move_inner(coord.x, coord.y) != 0) {
				if(this->walk_path().curr_coord_index > 0){
					this->walk_path().curr_coord_index++;
				}
			}
		}
		return k_BRS_SUCCESS;
	}
	else
	{
		if (this->walk_path().has_walk_path()) {
			if (this->walk_path().has_walk_coord()) {
				Coord coord;
				this->walk_path().get_next_coord(coord);
				if (this->move_scene()->grid_mover_count(coord, this->mover_detail().layer_id) > 0) {
					this->walk_path().reset();
					return k_BRS_SUCCESS;
				}
				this->move_watcher_tick(coord);
				if (this->move_inner(coord.x, coord.y) != 0) {
					if(this->walk_path().curr_coord_index > 0){
						this->walk_path().curr_coord_index++;
					}
				}
			} else {
				// 超过3个格子没有走完的，返回
				if(this->get_gauge_coord_num() >= MIN_MOVE_GRID_NUM){
					return k_BRS_SUCCESS;
				}
				Coord target_coord;
				if (this->walk_path().get_next_path(target_coord) != 0 ||
						(target_coord != this->birth_coord().grid_coord() && this->move_scene()->grid_mover_count(target_coord) > 0)) {
					this->walk_path().reset();
					return k_BRS_SUCCESS;
				}

				this->path_find(this->mover_detail().grid_coord, target_coord, this->walk_path().walk_coord, false);

				this->walk_path().curr_coord_index = this->walk_path().walk_coord.size();
				if (this->walk_path().curr_coord_index > 0) {
					int remain_move_num = this->get_gauge_coord_num();
					int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
					Coord_Vec coord_vec;
					if(move_num > this->walk_path().curr_coord_index){
						move_num = this->walk_path().curr_coord_index;
					}
					for(int i = 0; i < move_num; ++i){
						Coord coord;
						this->walk_path().get_next_coord(coord);
						coord_vec.push_back(coord.pixel_coord());
					}
					if(move_num > 0){
						this->move_inner_vec(coord_vec);
						this->push_watcher_tick(0, 100000);
					}
					this->teammate_move(target_coord);
				} else {
					MSG_DEBUG("patrol A scene:%d type id:%d x:%d y:%d => x:%d y:%d",this->move_scene_id(), this->npc_type_id(),this->mover_detail().grid_coord.x, this->mover_detail().grid_coord.y, target_coord.x, target_coord.y);
					this->push_watcher_tick(3,0);
				}
			}

			return k_BRS_SUCCESS;
		}

		// 设置巡逻状态
		if (this->npc_status() != NPC::PATROL_STATUS) {
			this->set_npc_status(NPC::PATROL_STATUS);
		}

		// 巡逻距离为0表示不巡逻
		if (this->npc_record()->config.move_range == 0) {
			return k_BRS_Failure;
		}

		// 巡逻tick
		if (!this->ai_running_info().tick_patrol_.cycle_interval()) {
			return k_BRS_SUCCESS;
		}
		// 等移动完成后，再走
		if(this->get_gauge_coord_num() > 0){
			return k_BRS_SUCCESS;
		}
		// 返回或者随机移动
		int distance = calculate_pixels_distance(this->grid_coord(), this->birth_coord().grid_coord());
		if (distance > this->npc_record()->config.move_range) {
			this->walk_path().walk_path.push_back(this->birth_coord().grid_coord());
		} else {
			this->ai_running_info().tick_patrol_.set_tick_now();
			if(this->play_chat() == 0){
				int rand_sec = 2 + random()%2;
				this->ai_running_info().tick_patrol_.set_interval(rand_sec, 0);
				return k_BRS_SUCCESS;
			}
			this->teammate_play_chat();
			Coord target_coord;

			int r = 3 + random()%3;
			double rndn = random()%101;
			double n = rndn/100*3.1415;
			target_coord.x = this->grid_coord().x+r*sin(n);
			target_coord.y = this->grid_coord().y+r*cos(n);
			uint16_t pix = grid_x_to_pixel(target_coord.x);
			uint16_t piy = grid_y_to_pixel(target_coord.y);
			if (!this->validate_move_value(pix, piy ) )
			{
				this->ai_running_info().tick_patrol_.set_interval(1 + random()%2, 0);
				return k_BRS_SUCCESS;
			}

			/// 是否没人地方
			if (this->move_scene()->calculate_coord_has_mover(this->mover_detail().layer_id, target_coord, 0, 2)) {
				int rand_sec = 1 + random()%2;
				this->ai_running_info().tick_patrol_.set_interval(rand_sec, 0);
				return k_BRS_SUCCESS;
			}

			this->walk_path().walk_path.push_back(target_coord);
		}

		int rand_sec = 3 + random()%2;
		this->ai_running_info().tick_patrol_.set_interval(rand_sec, 0);

		return k_BRS_SUCCESS;
	}
}

int Monster::update_idle_mode(void){
	int time = 3+random()%3;
	this->push_watcher_tick(time);
	this->monster_self()->play_chat();
	return k_BRS_SUCCESS;
}

int Monster::update_null_mode(void){
	return 0;
}

int Monster::update_idle_behavior(void){
	if(this->npc_status() == NPC::FIGHT_STATUS || this->npc_status() == NPC::PLOT_STATUS){
		return 0;
	}
	return update_idle_mode();
}

int Monster::update_null_behavior(void){
	return 0;
}

int Monster::update_move_behavior(void){
	if(this->npc_status() == NPC::FIGHT_STATUS || this->npc_status() == NPC::PLOT_STATUS){
		return 0;
	}
	if(this->npc_status() == NPC::BACK_STATUS ){
		return update_back_mode();
	}else if(this->npc_status() == NPC::CHASE_STATUS){
		return update_chase_mode();
	}else{
		return update_walk_mode();
	}
}

// 人物id
role_id_t Monster::master_role_id(void){
	return monster_detail_.master_role_id;
}

// 人物id
void Monster::set_master_role_id(const role_id_t role_id){
	monster_detail_.master_role_id  = role_id;
}

role_id_t Monster::caller_id(void) {
	return monster_detail_.caller_id;
}

void Monster::set_caller_id(const role_id_t role_id) {
	monster_detail_.caller_id = role_id;
}

void Monster::set_summon_monster_flag(void){
	is_summon_monster_ = true;
}

void Monster::set_ex_param(const int param_1, const int param_2, const int param_3){
	param_1_ = param_1;
	param_2_ = param_2;
	param_3_ = param_3;
}

void Monster::get_ex_param(int& param_1, int& param_2, int& param_3){
	param_1 = param_1_;
	param_2 = param_2_;
	param_3 = param_3_;
}
