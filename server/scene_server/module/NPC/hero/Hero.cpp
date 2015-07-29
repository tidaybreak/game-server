/*
 * Hero.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */
#include "Hero.h"
#include "Move_Scene.h"
#include "ai_machine/AI_Machine.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache.h"
#include "Msg_Active_Struct.h"
#include "Msg_NPC_Struct.h"
#include "Scene_Monitor.h"
#include "Scene_Public.h"
#include "Pool_Manager.h"
#include "Scene_Player.h"
#include "Fighter_Detail.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_NPC.h"
#include "Battle_Scene.h"

Hero::Hero(void) :
hero_die_(false),
is_register_timer_(false),
is_sync_(false),
has_init_(false),
has_battle_ia_(false)
{  }

Hero::~Hero(void) { }

int Hero::init(role_id_t role_id, Move_Scene* scene, const Coord& birth_coord){
	/// npc base init
	npc_detail_.npc_id = role_id;
	npc_detail_.npc_status = NORMAL_STATUS;
	npc_detail_.birth_time = Time_Value::gettimeofday();
	npc_detail_.ai_id = 0;
	npc_detail_.is_ai_start = true;
	npc_detail_.birth_coord = birth_coord;
	npc_detail_.life_time = Time_Value::gettimeofday() + Time_Value(300,0);
	set_hero_speed();

	/// monster fighter init
	Fighter::set_detail_prt(npc_detail_.fighter_detail_);
	npc_detail_.fighter_detail_.role_id = npc_id();

	///mover init
	Mover::set_detail_prt(npc_detail_.mover_detail_);
	this->set_grid_coord(npc_detail().birth_coord.grid_coord().x, npc_detail().birth_coord.grid_coord().y, true);
	this->set_pixel_coord(npc_detail().birth_coord.pixel_coord().x,npc_detail().birth_coord.pixel_coord().y);
	this->retify_gauge_grid_coord();
	npc_detail_.mover_detail_.mover_type = MOVER_T_HERO;
	this->set_layer_id(0);

	has_init_ = true;

	return 0;
}

int Hero::birth(role_id_t role_id, Move_Scene* scene, const Coord& birth_coord){
	if (this->init(role_id, scene, birth_coord) != 0) {
		//MSG_USER("hero type id:%d configure file error.", npc_id);
		return -1;
	}

	/// ai init
//	AI_Data * ai_data = AI_MACHINE->find_ai_behavior(npc_detail().ai_id);
//	if (ai_data != NULL) {
//		bev_tree_root_.install(*ai_data->ai_behavior);
//	} else {
//		bev_tree_root_.install(*AI_MACHINE->blockhead_ai());
//	}

	//this->start_ai();
	this->enter_move_scene(scene);
	this->move_appear();
	return 0;
}


int Hero::battle_init(role_id_t role_id, Battle_Scene* scene, const Coord& birth_coord){
	/// npc base init
	npc_detail_.npc_id = role_id;
	npc_detail_.npc_status = NORMAL_STATUS;
	npc_detail_.birth_time = Time_Value::gettimeofday();
	npc_detail_.ai_id = 0;
	npc_detail_.is_ai_start = true;
	npc_detail_.birth_coord = birth_coord;
	//npc_detail_.life_time = (npc_detail_.ai_config->life_time == Time_Value::zero) ? Time_Value(INT_MAX, 0) : Time_Value::gettimeofday() + npc_detail_.ai_config->life_time;
	npc_detail_.life_time = Time_Value::gettimeofday() + Time_Value(300,0);

	/// monster fighter init
	Fighter::set_detail_prt(npc_detail_.fighter_detail_);
	npc_detail_.fighter_detail_.role_id = npc_id();
//	//prop_value
//	for (std::vector<Int_Double> ::const_iterator it = monster_detail_.config_cache->prop_value_vec.begin();
//			it != monster_detail_.config_cache->prop_value_vec.end(); ++it)
//	{
//		Property_Type prop_type = Property_Type((*it).id);
//		npc_detail_.fighter_detail_.modify_basic_property(prop_type, Addition_Type::AT_BASIC, Operate_Type::O_SET, (*it).value);
//		if(prop_type == PT_BLOOD_MAX)
//		{
//			npc_detail_.fighter_detail_.modify_basic_property(PT_BLOOD_CURRENT, Addition_Type::AT_BASIC, Operate_Type::O_SET, (*it).value);
//		}
//	}

//	for (size_t i = 0; i < ARRAY_SIZE(g_pt_fight); ++i) {
//		npc_detail_.fighter_detail_.modify_basic_property(Property_Type(g_pt_fight[i]), Addition_Type::AT_BASIC, Operate_Type::O_SET, 1000);
//	}
//	npc_detail_.fighter_detail_.modify_basic_property(PT_BLOOD_CURRENT, Addition_Type::AT_BASIC, Operate_Type::O_SET, 1000);
//	npc_detail_.fighter_detail_.modify_basic_property(PT_PHY_ATTACK, Addition_Type::AT_BASIC, Operate_Type::O_SET, 50);
//	npc_detail_.fighter_detail_.modify_basic_property(PT_SPELL_ATTACK, Addition_Type::AT_BASIC, Operate_Type::O_SET, 50);

//	const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(npc_detail_.npc_type_id);
//	if (config_cache == 0) {
//		MSG_USER("find npc config:%d error", npc_detail_.npc_type_id);
//		return -1;
//	}
//	monster_detail_.config_cache = config_cache;
//
//	int default_skill = npc_addition_info.default_skill ? npc_addition_info.default_skill : config_cache->default_skill;
//	if (set_skill_detail(default_skill)) {
//		MSG_USER("set skill:%d error, npc:%d", default_skill, npc_type_id);
//		return -1;
//	}
//	add_ext_skill(npc_addition_info.ext_skill_set);

	//skill

//	npc_detail_.fighter_detail_.default_skill = 21000001;
//	Skill_DB_Info skill_info;
//
//	skill_info.skill_id = 21000001;
//	skill_info.skill_level = 1;
//	npc_detail_.fighter_detail_.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));



	///mover init
	Mover::set_detail_prt(npc_detail_.mover_detail_);
	this->set_grid_coord(npc_detail().birth_coord.grid_coord().x, npc_detail().birth_coord.grid_coord().y, true);
	this->set_pixel_coord(npc_detail().birth_coord.pixel_coord().x,npc_detail().birth_coord.pixel_coord().y);
	npc_detail_.mover_detail_.mover_type = MOVER_T_HERO;
	this->set_layer_id(0);

	has_init_ = true;

	return 0;
}

int Hero::battle_birth(role_id_t role_id, Battle_Scene* scene, const Coord& birth_coord){
	if (this->battle_init(role_id, scene, birth_coord) != 0) {
		return -1;
	}

	/// ai init
//	AI_Data * ai_data = AI_MACHINE->find_ai_behavior(npc_detail().ai_id);
//	if (ai_data != NULL) {
//		bev_tree_root_.install(*ai_data->ai_behavior);
//	} else {
//		bev_tree_root_.install(*AI_MACHINE->blockhead_ai());
//	}

	//this->start_ai();

	if (! hero_base_detail_.pos_key.idx) {
		return -1;
	}
	if(set_battle_position(hero_base_detail_.pos_key.pos, hero_base_detail_.pos_key.idx, scene) == 0){
		enter_battle_scene(scene);
		battle_appear();
	}
	return 0;
}

int Hero::fini(void) {
	NPC::fini();

	reset();

	return 0;
}


int Hero::reset(void) {
	NPC::reset();

	hero_base_detail_.reset();
	hero_die_ = 0;
	Time_Value tick_time_ = Time_Value::zero;
	is_register_timer_ = false;
	is_sync_ = false;
	has_init_ = false;
	has_battle_ia_ = false;
	return 0;
}

int Hero::tick(Time_Value &now) {
	//NPC::tick(now);
	return 0;
}

void Hero::fight_tick(const Time_Value &now) {
	Fighter::fight_tick(now);
	Fighter::time_up(now);
}

bool Hero::is_hero(void){
	return true;
}

int Hero::move_appear_info(Block_Buffer &buf) {
	bool need_appear = false;
	if (role_id() != 60210021) {	// TODO config
		need_appear = true;
	}
	if (need_appear) {
		size_t wr_begin_idx = buf.get_write_idx();
		size_t wr_end_idx = 0;
		uint32_t len = 0;

		MSG_81000005 msg;
		msg.npc_info.role_id = role_id();
		msg.npc_info.npc_type = hero_base_detail_.hero_id;
		msg.npc_info.npc_jid = hero_base_detail_.hero_id;
		msg.npc_info.role_name = hero_base_detail_.hero_name;
		msg.npc_info.master_direct = hero_base_detail_.master_role_id;
		msg.npc_info.awake_lvl = hero_base_detail_.awake_lvl;
		msg.npc_info.coord = this->pixel_coord();
		msg.npc_info.hp_curr = 1000;
		msg.npc_info.hp_max = 1000;
		msg.npc_info.speed = get_move_speed();
		buf.make_message(msg.msg_id);
		msg.serialize(buf);

		// refill len in head
		wr_end_idx = buf.get_write_idx();
		len = wr_end_idx - wr_begin_idx - sizeof(uint32_t);
		buf.set_write_idx(wr_begin_idx);
		buf.write_uint32(len);
		buf.set_write_idx(wr_end_idx);
	}

	return 0;
}

int Hero::make_move_info(Block_Buffer &buf, uint8_t toward, uint16_t x, uint16_t y)
{
	MSG_81000020 msg;
	msg.role_id = role_id();

	msg.coord.push_back(Coord(x, y));
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	return 0;
}

int Hero::battle_appear_info(Block_Buffer &buf) {
	// size_t rd_idx = buf.get_read_idx();
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	// fill head
	int state = 0;
	if (0 != fighter() && DIE_OVER == fighter()->get_battle_status()) {
		state = 1;
	}
	MSG_81000008 msg;
	msg.fighter_info.role_id = role_id();
	msg.fighter_info.role_name = hero_base_detail_.hero_name;
	msg.fighter_info.type = (int)mover_detail().mover_type;
	msg.fighter_info.role_type = hero_base_detail_.hero_id;
	msg.fighter_info.role_jid = hero_base_detail_.hero_id;
	msg.fighter_info.master_direct = hero_base_detail_.master_role_id;
	msg.fighter_info.position.val_1 = mover_detail().battle_position.pos;
	msg.fighter_info.position.val_2 = mover_detail().battle_position.idx;
	msg.fighter_info.hp_curr = blood_current();
	msg.fighter_info.hp_max = blood_max();
	msg.fighter_info.fight_state = state;
	msg.fighter_info.awake_lvl = hero_base_detail_.awake_lvl;
	Int_Int model;
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

int Hero::battle_appear(void) {
	Mover::battle_appear();

	return 0;
}

int Hero::battle_disappear(void) {
	Fighter::battle_disappear();

	return 0;
}

int Hero::enter_battle_scene(Battle_Scene *scene) {
	if(scene == NULL){
		return -1;
	}
	if (this->battle_scene()) {
		MSG_TRACE_ABORT("has battle scene, role:%ld battle_id:%d, b_id:%d", role_id(), battle_scene_id(), scene->battle_id());
		return -1;
	}

	MSG_DEBUG("hero enter battle scene, hero:%ld, b_id:%d, pos:%d  %d", role_id(), scene->battle_id(), this->battle_position().pos, this->battle_position().idx);

	Fighter::enter_battle_scene(scene);
	Scene_Player* player = this->master();
	if(player && player->move_scene() && player->move_scene()->scene_type() == War_Scene_Type){
		int hp = player->war_info()->get_hero_hp(this->hero_base_detail_.hero_id);
		if(hp > 0){
			//recover hp
			Prop_Setter_Vec fight_setter;
			Prop_Setter setter;
			setter.prop_type = PT_BLOOD_CURRENT;
			setter.operate = O_SET;
			setter.addi_type = AT_BASIC;
			setter.basic = hp;
			fight_setter.push_back(setter);
			this->modify_props_normal(fight_setter, false, 13);
		}
	}
	return 0;
}

int Hero::exit_battle_scene(int exit_from) {
	if(recover_state()){
		return -1;
	}
	if (!this->battle_scene()) {
		MSG_DEBUG("may be exit by master, hero:%ld", role_id());
		return 0;
	}

	MSG_DEBUG("hero exit battle scene, hero:%ld, b_id:%d", role_id(), this->battle_scene_id());

	NPC::exit_battle_scene();

//	if (npc_detail_.fighter_detail_.fetch_fight_property(PT_BLOOD_CURRENT) < 1) {
		double hp_max = npc_detail_.fighter_detail_.fetch_fight_property(PT_BLOOD_MAX);
		npc_detail_.fighter_detail_.modify_basic_property(PT_BLOOD_CURRENT, AT_BASIC, O_SET, hp_max);
//	}
	if(has_battle_ia_){// 竞技场英雄镜像，需要回收
		this->set_npc_status(RECOVER_STATUS);
	}
	return 0;
}

Hero *Hero::hero_self(void){
	return this;
}

int Hero::refresh_hero_base_detail(Hero_Base_Detail &detail) {
	role_id_t hero_role_id = hero_base_detail_.hero_role_id;
	hero_base_detail_ = detail;
	// 英雄属性改变时候，同步到场景服，更新英雄基本信息，此时Hero_Base_Detail中英雄role_id为0，不能直接覆盖
	if (hero_base_detail_.hero_role_id <= 0) {
		hero_base_detail_.hero_role_id = hero_role_id;
	}
	return 0;
}

int Hero::refresh_hero_fighter_detail(Fighter_Detail &detail) {
	npc_detail_.fighter_detail_ = detail;
	npc_detail_.fighter_detail_.role_id = hero_base_detail_.hero_role_id;
	Fighter::set_detail_prt(npc_detail_.fighter_detail_);
	// 英雄属性变化同步宠物信息到地图服时候，地图服英雄要重新设置阵营战斗模式
	Scene_Player *player = this->master();
	if (player) {
//		set_pet_camps(player->fighter_camps());
//		set_pet_country(player->country());
//		set_pet_team_id(player->team_id());
//		set_pet_faction_id(player->fighter_detail().faction_id);
//		set_pet_fight_mode(player->fighter_mode());
	}

	// 添加英雄技能
	//add_hero_skill();

	//死亡后满血复活
//	if (fighter_detail_.fetch_property(Fighter_Detail::HP) < 1) {
//		double hp = max_hp();
//		fighter_detail_.modify_property(Fighter_Detail::HP, Fighter_Detail::BASIC,
//						Fighter_Detail::SET, hp);
//	}
//
//	if (scene()) {
//		pet_notify_fight_info();
//	}

	return 0;
}

int Hero::sync_hero_fighter_detail_to_logic(void) {
	Block_Buffer sync_buff;
	sync_buff.make_message(INNER_SYNC_HERO_FIGHTER_DETAIL);
//	fighter_detail()->serialize(sync_buff);
//	sync_buff.write_int64(hero_base_detail_.master_role_id);
//	sync_buff.write_int32(hero_base_detail_.hero_index);
//	sync_buff.finish_message();
//	return send_to_logic(sync_buff);
	return 0;
}

int Hero::sync_hero_die_to_logic(void) {
	Block_Buffer sync_buff;
	sync_buff.make_message(INNER_SYNC_HERO_DIE_TO_LOGIC);
	sync_buff.write_int64(hero_base_detail_.master_role_id);
	sync_buff.finish_message();
	return send_to_logic(sync_buff);
}

int Hero::sync_hero_call_back_to_logic() {
	Block_Buffer sync_buff;
	sync_buff.make_message(INNER_SYNC_HERO_OUT);
	sync_buff.write_int64(hero_base_detail_.master_role_id);
	sync_buff.finish_message();
	return send_to_logic(sync_buff);
}

int Hero::sync_hero_info_to_logic(void) {
	Block_Buffer sync_buff;
	sync_buff.make_message(INNER_SYNC_HERO_INFO_TO_LOGIC);
	MSG_20400005 sync_msg;
	sync_msg.master_role_id = hero_base_detail_.master_role_id;
	sync_msg.hero_index = hero_base_detail_.hero_index;
	//sync_msg.exp = fighter_detail_->experience;
	//sync_msg.h = fighter_detail_->fetch_fight_property(PT_BLOOD_CURRENT);
	sync_msg.serialize(sync_buff);
	sync_buff.finish_message();
	return send_to_logic(sync_buff);
	return 0;
}

int Hero::send_to_logic(Block_Buffer &buf) {
	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(hero_base_detail_.master_role_id);
	if (player) {
		return player->send_to_logic(buf);
	}
	return 0;
	//return MAP_MANAGER_INSTANCE->send_to_logic_server(buf);
}

int Hero::outer_hero_recycle(int type) {
	sync_hero_fighter_detail_to_logic();

	if (this->validate_battle_scene()) {
		exit_battle_scene();
	}
	if (this->validate_mover_scene()) {
		exit_move_scene();
	}

	if (type == CALL_BACK) {
		//skill_erase_status();
		sync_hero_call_back_to_logic();
	} else if (type == DEAD) {
		//skill_erase_status();
		sync_hero_die_to_logic();
	}

	NPC_MANAGER->hero_map().erase(hero_base_detail_.hero_role_id);
	SCENE_PUBLIC->unbind_master_id_hero(hero_base_detail_.master_role_id);

	//reset();
	POOL_MANAGER->push_hero(this);

	return 0;
}

int Hero::fight_hero_recycle(int type) {
	sync_hero_fighter_detail_to_logic();

	if (this->validate_battle_scene()) {
		exit_battle_scene();
	}
	if (this->validate_mover_scene()) {
		exit_move_scene();
	}

	if (type == CALL_BACK) {
		//skill_erase_status();
		sync_hero_call_back_to_logic();
	} else if (type == DEAD) {
		//skill_erase_status();
		sync_hero_die_to_logic();
	}

	NPC_MANAGER->hero_map().erase(hero_base_detail_.hero_role_id);
	SCENE_PUBLIC->unbind_fight_hero(hero_base_detail_.master_role_id);

	//reset();
	POOL_MANAGER->push_hero(this);

	return 0;
}

//int Hero::add_hero_skill(void) {
//	if (! hero_base_detail_.skill_vector.empty()) {
//		if (hero_base_detail_.skill_vector[0] > 0) {
//			npc_detail_.fighter_detail_.default_skill = hero_base_detail_.skill_vector[0];
//		}
//
//		Talent talent;
//		talent.talent_id = 1;
////		Talent_Page page;
////		page.reset();
////		page.page_id = 1;
//		Skill_DB_Info skill_info;
//		for (size_t i = 0; i < hero_base_detail_.skill_vector.size(); ++i) {
//			if (hero_base_detail_.skill_vector[i] > 0) {
//				//this->add_new_skill(hero_base_detail_.skill_vector[i]);
//				skill_info.reset();
//				skill_info.skill_id = hero_base_detail_.skill_vector[i];
//				skill_info.skill_level = 1;
//				//npc_detail_.fighter_detail_.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
//				//page.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
//				talent.skill_db_info_map.insert(std::make_pair(skill_info.skill_id, skill_info));
//			}
//		}
////		npc_detail_.fighter_detail_.talent_page_map[page.page_id] = page;
////		npc_detail_.fighter_detail_.cur_used_talent_page = page.page_id;
//		npc_detail_.fighter_detail_.talent_map[talent.talent_id] = talent;
//		npc_detail_.fighter_detail_.cur_used_talent_ = talent.talent_id;
//	}
//
//	return 0;
//}

int Hero::die_process(const Attack_Param &info) {
	Fighter::die_process(info);
	if(this->battle_scene()){
		this->battle_scene()->listen_die(this->fighter());
	}
//	exit_battle_scene();
//
//	// 死亡后满血复活
//	if (npc_detail_.fighter_detail_.fetch_fight_property(PT_BLOOD_CURRENT) < 1) {
//		double hp_max = npc_detail_.fighter_detail_.fetch_fight_property(PT_BLOOD_MAX);
//		npc_detail_.fighter_detail_.modify_fight_property(PT_BLOOD_CURRENT, AT_BASIC, O_SET, hp_max, 0.0, 0.0);
//	}
	//hero_recycle();

	return 0;
}

void Hero::set_hero_speed(void) {
	if (hero_base_detail_.master_role_id) {
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(hero_base_detail_.master_role_id);
		if (player) {
			set_move_speed(player->fighter_detail().fetch_fight_property(PT_SPEED));
		}
	}
}

Scene_Player *Hero::master(void) {
	if (hero_base_detail_.master_role_id) {
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(hero_base_detail_.master_role_id);
		if (player) {
			return player;
		}
	}
	return NULL;
}

int Hero::level(void) {
	return hero_base_detail_.hero_level;
}


int Hero::ai_select_skill(void){
	if(has_battle_ia_ && this->battle_scene()){
		return battle_scene()->ai_select_skill(this);
	}
	return 0;
}

void Hero::set_battle_ai(void){
	has_battle_ia_ = true;
}

bool Hero::has_battle_ai(void){
	return has_battle_ia_;
}

int Hero::set_pos_key(const int pos, const int idx) {
	hero_base_detail_.pos_key.pos = pos;
	hero_base_detail_.pos_key.idx = idx;
	return 0;
}

