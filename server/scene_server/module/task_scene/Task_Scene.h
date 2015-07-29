/*
 * Task_Scene.h
 *
 *  Created on: Sep 3, 2014
 *      Author: gaojianshan
 */

#ifndef TASK_SCENE_H_
#define TASK_SCENE_H_

#include "Move_Scene.h"
#include "boost/unordered_set.hpp"
#include "scene_move/Move_Scene_Base.h"

typedef boost::unordered_set<role_id_t> PlayerIdSet;

class Task_Scene: public Move_Scene {
public:
	Task_Scene();
	virtual ~Task_Scene();

	void reset(void);

	virtual int enter_scene(Mover *mover);

	int tick(const Time_Value &now);
	virtual int exit_scene(Mover *mover, bool offline);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int client_scene_complete(Fighter &fighter);
	virtual void hook_battle_finish(int battle_type, int win_pos,
									std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec,
									std::vector<int> kill_monters);

	inline const uint32_t get_scene_task_id() const;
	inline void set_scene_task_id(const uint32_t task_id);
	void refresh_task_limit_time_kill_boss();
	inline uint32_t get_task_scene_monster_num();

	void kick_all_player(void);
private:
	PlayerIdSet player_id_set;
	uint32_t task_id;
	bool is_win;
	Time_Value end_time_;
};

const uint32_t Task_Scene::get_scene_task_id() const {
	return task_id;
}

void Task_Scene::set_scene_task_id(const uint32_t task_id) {
	this->task_id = task_id;
}

uint32_t Task_Scene::get_task_scene_monster_num() {
	uint32_t num = 0;
    Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->monster_self()) {
				++num;
			}
		}
	}

	return num;
}

#endif /* TASK_SCENE_H_ */
