/*
 * Hero_Monster.h
 *
 *  Created on: May 26, 2014
 *      Author: chenyulin
 */

#ifndef HERO_MONSTER_H_
#define HERO_MONSTER_H_

#include "Monster.h"

class Hero_Monster:	public Monster {
public:
	Hero_Monster(void);
	virtual ~Hero_Monster(void);

	int reset(void);

	int battle_birth(role_id_t npc_id, int npc_type_id, NPC_Record *record,
			NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene, Block_Buffer& buf);

	int battle_init(int npc_type_id, NPC_Record *record, NPC_Addition_Info
			&npc_addition_info, Battle_Scene* battle_scene, Block_Buffer& buf);

	virtual int tick(Time_Value &now);

	virtual bool is_hero_monster(void);

	virtual int battle_appear_info(Block_Buffer &buf);

protected:
//	NPC_Record npc_record_;
//	Pet_Base_Detail pet_base_detail_;

};

#endif /* HERO_MONSTER_H_ */
