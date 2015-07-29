/*
 * Pet_Monster.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "Pet_Monster.h"
#include "NPC/NPC_Manager.h"
#include "Configurator.h"

Pet_Monster::Pet_Monster(void) { reset(); }

Pet_Monster::~Pet_Monster(void) { }

int Pet_Monster::birth(role_id_t npc_id, int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info) {
//	npc_record_.scene = scene;
//	npc_record_.birth_by = npc_addition_info.identify.master_id;
//
//	NPC_Record_Info record_coord;
//	record_coord.reset();
//	record_coord.coord_vec.push_back(npc_addition_info.birth_coord);
//	npc_record_.npc_type_map.insert(std::make_pair(npc_type_id, record_coord));
//
//	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
//		MSG_USER("Monster:% birth error coord:%d %d", npc_type_id, npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return -1;
//	}
//
//	if (scene->mpt_value_by_coord(npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y) < Move_Scene::MOVE_ABLE_FROM) {
//		MSG_USER("Monster:%d birth error coord:%d %d error", npc_type_id, npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return -1;
//	}
//
//	npc_detail_.npc_id = npc_id;
//	pet_base_detail_.pet_role_id = npc_id;
//	int ret = this->init(npc_type_id, &npc_record_, npc_addition_info);
//	if (ret != 0) {
//		reset();
//		MSG_USER("AI: monster:%d birth error:%d.", npc_type_id, ret);
//		return -1;
//	}
//
//	if (ai_config().tick_time <= Time_Value::zero) {
//		reset();
//		MSG_USER("AI: monster:%d tick error.", npc_type_id);
//		return -1;
//	}
//
//	if (monster_detail_.current_skill->config_cache->skill_id == 0) {
//		reset();
//		MSG_USER("AI: monster:%d skill error.", npc_type_id);
//		return -1;
//	}
//
//	this->scene()->enter_scene(this);
//
//	/// ai init
//	this->init_bt();
//	this->start_ai();
//
//	this->appear();

	return 0;
}

int Pet_Monster::reset(void) {
	Monster::reset();

	npc_record_.reset();
//	pet_base_detail_.reset();

	return 0;
}

//int Pet_Monster::refresh_base_detail(Pet_Base_Detail &detail) {
//	pet_base_detail_ = detail;
//
//	return 0;
//}

int Pet_Monster::refresh_fighter_detail(Fighter_Detail &detail) {
//	fighter_detail_ = detail;
//	fighter_detail_.fight_timestamp = Time_Value::zero;
//	fighter_detail_.camps = CAMPS_FB_PLAYER_ENEMY;
//
//	refresh_player_combat_power();
//	restore_full_status();
//	double anger_cur = fighter_detail_.fetch_property(Fighter_Detail::ANGER);
//	map_set_prop(anger_cur, Fighter_Detail::ANGER, Fighter_Detail::BASIC);
//
//	/*
//	Skill_Running_Args running_args;
//	for (Int_Vec::iterator it = pet_base_detail_.skill_vector.begin(); it != pet_base_detail_.skill_vector.end(); ++it) {
//		running_args.reset();
//		fetch_skill_running_args(*it, running_args);
//
//		if (running_args.config_cache != NULL && running_args.config_cache->skill_category == 5) {
//			ai_skill_vec_.push_back(running_args);
//		}
//	}
//	*/

	return 0;
}

NPC_Record *Pet_Monster::npc_record(void) {
	return &npc_record_;
}

//int Pet_Monster::die_process(const Attack_Info &info, Fighter &killer) {
//	if (npc_status() == RECOVER_STATUS || npc_status() == DEAD_STATUS) return -1;
//	NPC::die_process(info, killer);
//
//	/// notice logic server
//	notice_logic_die(info.attacker);
//
//	return Fighter::die_process(info, killer);
//}

bool Pet_Monster::is_pet_monser(void) {
	return true;
}

//Pet_Base_Detail const &Pet_Monster::pet_base_detail(void) {
//	return pet_base_detail_;
//}

int Pet_Monster::make_appear_info(Block_Buffer &buf) {
//	size_t wr_begin_idx = buf.get_write_idx();
//	size_t wr_end_idx = 0;
//	uint16_t len = 0;
//
//	/// fill head
//	buf.write_uint16(0);						/// len
//	buf.write_uint32(ACTIVE_PET_APPEAR);		/// msg_id
//	buf.write_uint32(0);						/// status
//
//	MSG_800006 msg;
//	make_appear_info(msg);
//	msg.serialize(buf);
//
//	/// refill len in head
//	wr_end_idx = buf.get_write_idx();
//	len = wr_end_idx - wr_begin_idx - sizeof(uint16_t);
//	buf.set_write_idx(wr_begin_idx);
//	buf.write_uint16(len);
//	buf.set_write_idx(wr_end_idx);

	return 0;
}

//int Pet_Monster::make_appear_info(MSG_800006 &msg) {
//	msg.reset();
//
//	msg.pet_info.role_id = pet_base_detail_.pet_role_id;
//	msg.pet_info.master_id = pet_base_detail_.master_role_id;
//	msg.pet_info.master_name = pet_base_detail_.master_role_name;
//	msg.pet_info.name = pet_base_detail_.name;
//	msg.pet_info.pet_id = pet_base_detail_.pet_id;
//	msg.pet_info.pet_cid = pet_base_detail_.pet_cid;
//	msg.pet_info.px = mover_detail_.grid_coord.x;
//	msg.pet_info.py = mover_detail_.grid_coord.y;
//	msg.pet_info.level = fighter_detail_.level;
//	msg.pet_info.hp = fighter_detail_.fetch_property(Fighter_Detail::HP);
//	msg.pet_info.hp_max = fighter_detail_.fetch_property(Fighter_Detail::HP_MAX);
//	msg.pet_info.growth = pet_base_detail_.growth;
//	msg.pet_info.quality = pet_base_detail_.quality;
//	msg.pet_info.skill_vector = pet_base_detail_.skill_vector;
//	msg.pet_info.camps = fighter_detail_.camps;
//	msg.pet_info.fight_mode = fighter_detail_.fight_mode;
//	return 0;
//}
