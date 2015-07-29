/*
 * Dragon_Vale_Scene.h
 *
 *  Created on: 2014年7月12日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_SCENE_H_
#define DRAGON_VALE_SCENE_H_

#include "Move_Scene.h"
#include "boost/unordered_set.hpp"
#include "scene_move/Move_Scene_Base.h"

typedef boost::unordered_set<role_id_t> PlayerIdSet;

class Dragon_Vale_Scene: public Move_Scene {
public:
	Dragon_Vale_Scene();
	virtual ~Dragon_Vale_Scene();

	void reset(void);

	virtual int enter_scene(Mover *mover);

	virtual int exit_scene(Mover *mover, bool offline);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	virtual int client_scene_complete(Fighter &fighter);
	virtual void hook_battle_finish(int battle_type, int win_pos,
									std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec,
									std::vector<int> kill_monters);
	inline role_id_t get_scene_creater() const;
	inline void set_scene_creater(role_id_t creater);
	inline int64_t get_creater_uuid() const;
	inline void set_creater_uuid(int64_t uuid);
	inline uint32_t get_dragon_vale_scene_monster_num();

private:
	PlayerIdSet player_id_set;
	role_id_t creater;
	int64_t creater_uuid;
};

role_id_t Dragon_Vale_Scene::get_scene_creater() const {
	return creater;
}

void Dragon_Vale_Scene::set_scene_creater(role_id_t creater) {
	this->creater = creater;
}

int64_t Dragon_Vale_Scene::get_creater_uuid() const {
	return this->creater_uuid;
}

void Dragon_Vale_Scene::set_creater_uuid(int64_t uuid) {
	this->creater_uuid = uuid;
}

uint32_t Dragon_Vale_Scene::get_dragon_vale_scene_monster_num() {
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

#endif /* DRAGON_VALE_SCENE_H_ */
