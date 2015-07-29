/*
 * Player_Monster.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef PLAYER_MONSTER_H_
#define PLAYER_MONSTER_H_

#include "Monster.h"
#include "Base_Detail.h"

struct Skill_Config;

class Player_Monster :	public Monster {
public:
	Player_Monster(void);
	virtual ~Player_Monster(void);

public:

	int reset(void);
	int battle_birth(role_id_t npc_id, int npc_type_id, NPC_Record *record,
			NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene, Block_Buffer& buf);
	int battle_init(int npc_type_id, NPC_Record *record, NPC_Addition_Info
			&npc_addition_info, Battle_Scene* battle_scene, Block_Buffer& buf);

	virtual int tick(Time_Value &now);
	virtual void fight_tick(const Time_Value &now);

	virtual bool is_monster(void);

	virtual Monster *monster_self(void);

	virtual bool is_player_monster(void);

	virtual Player_Monster *player_monster_self(void);

	virtual int battle_appear_info(Block_Buffer &buf);

	void player_pos_key_fit_hero(Position_Key &player_pos_key, Position_Key &hero_pos_key);

	int hero_battle_birth(Block_Buffer& buf, Battle_Scene* scene);

	void floating_skill_trigger(const Skill_Config& config);

	Hero* find_hero(void);

	int battle_enter_appaer(Battle_Scene* scene);

	inline void set_player_monster_avatar(Int_Vec &vec);

private:
	int career_;
	int gender_;
	role_id_t hero_role_id_;
	Int_Vec avatar_vec;
};

inline void Player_Monster::set_player_monster_avatar(Int_Vec &vec) {
	avatar_vec = vec;
}

#endif /* PLAYER_MONSTER_H_ */
