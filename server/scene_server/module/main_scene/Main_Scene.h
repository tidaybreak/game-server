/*
 * Main_Scene.h
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_

#include "Move_Scene.h"

class Main_Scene: public Move_Scene {
public:
	Main_Scene();
	virtual ~Main_Scene();

	void reset(void);
public:
	int open_secret_shop(const Coord& coord, const int shop_type);
	int close_secret_shop(void);
	virtual void be_hurt_and_sub_blood(Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id);
};

#endif /* MAIN_SCENE_H_ */
