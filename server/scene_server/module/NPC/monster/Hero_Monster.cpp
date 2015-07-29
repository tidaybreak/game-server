/*
 * Hero_Monster.cpp
 *
 *  Created on: May 26, 2014
 *      Author: chenyulin
 */

#include "Hero_Monster.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_NPC.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"

Hero_Monster::Hero_Monster(void) {
	reset();
}

Hero_Monster::~Hero_Monster(void) {

}

int Hero_Monster::reset(void) {
	Monster::reset();
	return 0;
}

int Hero_Monster::battle_birth(role_id_t npc_id, int npc_type_id, NPC_Record *record, NPC_Addition_Info &npc_addition_info,
		Battle_Scene* battle_scene, Block_Buffer& buf) {

	return 0;
}


int Hero_Monster::battle_init(int npc_type_id, NPC_Record *record, NPC_Addition_Info
		&npc_addition_info, Battle_Scene* battle_scene, Block_Buffer& buf) {

	return 0;
}

int Hero_Monster::tick(Time_Value &now) {
	Monster::tick(now);
	return 0;
}


bool Hero_Monster::is_hero_monster(void) {
	return true;
}

int Hero_Monster::battle_appear_info(Block_Buffer &buf){

	return 0;
}
