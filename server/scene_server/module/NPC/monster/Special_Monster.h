/*
 * Special_Monster.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef SPECIAL_MONSTER_H_
#define SPECIAL_MONSTER_H_

#include "Monster.h"

class Special_Monster :	public Monster {
public:
	Special_Monster(void);

	virtual ~Special_Monster(void);

	int birth(role_id_t npc_id, int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info);

	int reset(void);

	virtual NPC_Record *npc_record(void);
	virtual void set_record(NPC_Record *record);

	virtual Special_Monster *special_monster_self(void);

	virtual bool is_special_monser(void);

	void set_life_time(int life_ms);

	int skill_type_birth_by(void);

protected:
	NPC_Record npc_record_;
};

#endif /* SPECIAL_MONSTER_H_ */
