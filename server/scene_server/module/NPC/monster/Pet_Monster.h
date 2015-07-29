/*
 * Pet_Monster.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef PET_MONSTER_H_
#define PET_MONSTER_H_

#include "Monster.h"

class Pet_Monster :	public Monster {
public:
	Pet_Monster(void);

	virtual ~Pet_Monster(void);

	int birth(role_id_t npc_id, int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info);

	int reset(void);

//	int refresh_base_detail(Pet_Base_Detail &detail);

	int refresh_fighter_detail(Fighter_Detail &detail);

//	Pet_Base_Detail const &pet_base_detail(void);

	virtual bool is_pet_monser(void);

	virtual int make_appear_info(Block_Buffer &buf);

//	int make_appear_info(MSG_800006 &msg);

//	virtual int die_process(const Attack_Info &info, Fighter &killer);

	virtual NPC_Record *npc_record(void);

protected:
	NPC_Record npc_record_;
//	Pet_Base_Detail pet_base_detail_;
};

#endif /* PET_MONSTER_H_ */
