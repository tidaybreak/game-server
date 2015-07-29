/*
 * Arena_Scene.h
 *
 *  Created on: 15 2 2015
 *      Author: zhoujin
 */

#ifndef ARENA_SCENE_H_
#define ARENA_SCENE_H_

#include "Move_Scene.h"

class Arena_Scene: public Move_Scene {
public:
	Arena_Scene();
	virtual ~Arena_Scene();

	void reset(void);
public:
//	int open_secret_shop(const Coord& coord, const int shop_type);
//	int close_secret_shop(void);
//	virtual void be_hurt_and_sub_blood(Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id);
};

#endif /* MAIN_SCENE_H_ */
