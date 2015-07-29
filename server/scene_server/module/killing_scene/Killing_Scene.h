/*
 * Killing_Scene.h
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */

#ifndef KILLING_SCENE_H_
#define KILLING_SCENE_H_

#include "Move_Scene.h"


class Killing_Scene: public Move_Scene {
public:
	Killing_Scene();
	virtual ~Killing_Scene();
public:
	void reset(void);
	virtual int tick(const Time_Value &now);
	virtual int enter_scene(Mover *mover);
	virtual int exit_scene(Mover *mover, bool offline);
	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int client_scene_complete(Fighter &fighter);

	void kick_all_player(void);
private:
	Role_Id_Set player_id_set;
	Time_Value end_time_;
};


#endif /* KILLING_SCENE_H_ */
