/*
 * Special_Monster.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "Special_Monster.h"
#include "NPC/NPC_Manager.h"

Special_Monster::Special_Monster(void) { reset(); }

Special_Monster::~Special_Monster(void) { }

int Special_Monster::birth(role_id_t npc_id, int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info) {
//	npc_record_.reset();
//	npc_record_.scene = scene;
////	npc_record_.birth_by = npc_addition_info.identify.master_id;
//	npc_record_.layer_id = npc_addition_info.layer_id;
//
//	NPC_Record_Info record_coord;
//	record_coord.coord_vec.push_back(npc_addition_info.birth_coord);
//	npc_record_.npc_type_map.insert(std::make_pair(npc_type_id, record_coord));
//
//	int ret = Monster::birth(npc_id, npc_type_id, &npc_record_, npc_addition_info);
//	if (ret != 0) {
//		reset();
//		MSG_USER("special monster:%d birth error:%d.", npc_type_id, ret);
//		return -1;
//	}
//
//	return ret;
	return 0;
}

int Special_Monster::reset(void) {
	Monster::reset();

	npc_record_.reset();

	return 0;
}

NPC_Record *Special_Monster::npc_record(void) {
	return &npc_record_;
}

void Special_Monster::set_record(NPC_Record *record) {
	npc_record_ = *record;
}

Special_Monster* Special_Monster::special_monster_self(void) {
	return this;
}

bool Special_Monster::is_special_monser(void) {
	return true;
}

void Special_Monster::set_life_time(int life_ms) {
	if (life_ms <= 0) {
		MSG_USER("skill npc %d, config error.", this->npc_type_id());
	}

	npc_detail_.life_time = Time_Value::gettimeofday() + Time_Value(life_ms / 1000, life_ms % 1000 * 1000);
}

int Special_Monster::skill_type_birth_by(void) {
	return npc_detail_.birth_by_skill;
}
