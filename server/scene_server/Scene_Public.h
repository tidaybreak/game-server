/*
 * Scene_Public.h
 *
 *  Created on: Nov 25, 2013
 *      Author: 杂项/公共
 */

#ifndef SCENE_PUBLIC_H_
#define SCENE_PUBLIC_H_

#include "Game_Typedef.h"

struct Hero;

class Scene_Public {
public:
	typedef Hash_Map<role_id_t, Hero *, Null_Mutex> Master_ID_Hero_Map;

	enum Sync_Type{
		SP_WORLD_LEVEL = 0,
	};

	Scene_Public();
	virtual ~Scene_Public();

	int init(int monitor_index);

	int start(void);

	///////////////////////////////////////////////////////////////////////////////////////////////////

	inline Hero *find_master_id_hero(role_id_t master_id);	// 查找未初始化或需要重新初始化的跟随英雄
	Hero *find_mover_hero(role_id_t master_id);		// 查找已进入移动场景的英雄
	int bind_master_id_hero(role_id_t master_id, Hero &hero);
	int unbind_master_id_hero(role_id_t master_id);

	inline Hero *find_fight_hero(role_id_t master_id);			// 查找未初始化或需要重新初始化的战斗英雄
	Hero *find_battle_hero(role_id_t master_id);				// 查找已进入战斗场景的英雄
	inline int bind_fight_hero(role_id_t master_id, Hero &hero);
	inline int unbind_fight_hero(role_id_t master_id);
	void hero_time_up(const Time_Value &now);

	void recycle_all_hero(role_id_t master_id);

	int sync_dispath(Block_Buffer &buf);


	int world_level(void);
	int set_world_level(const int world_level);
	bool had_ask_world_level(void);
	bool set_asked_world_level(void);
	///////////////////////////////////////////////////////////////////////////////////////////////////

	int inner_check_info_on_center_server(Block_Buffer &buf);


private:
	Master_ID_Hero_Map master_id_hero_map_; 		// 主人id - 跟随英雄 map
	Master_ID_Hero_Map master_id_fight_hero_map_; 	// 主人id - 战斗英雄 map
	int world_level_;
	bool had_ask_world_level_;
};

typedef Singleton<Scene_Public> Scene_Public_Single;
#define SCENE_PUBLIC    (Scene_Public_Single::instance())

////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Hero *Scene_Public::find_master_id_hero(role_id_t role_id) {
	// 查找未初始化或需要重新初始化的跟随英雄
	Master_ID_Hero_Map::iterator it = master_id_hero_map_.find(role_id);
	if (it != master_id_hero_map_.end()) {
		return it->second;
	} else {
		return NULL;
	}
}

inline Hero *Scene_Public::find_fight_hero(role_id_t role_id) {
	// 查找未初始化或需要重新初始化的战斗英雄
	Master_ID_Hero_Map::iterator it = master_id_fight_hero_map_.find(role_id);
	if (it != master_id_fight_hero_map_.end()) {
		return it->second;
	} else {
		return NULL;
	}
}


inline int Scene_Public::bind_fight_hero(role_id_t master_id, Hero &hero) {
	master_id_fight_hero_map_.bind(master_id, &hero);
	return 0;
}

inline int Scene_Public::unbind_fight_hero(role_id_t master_id) {
	Master_ID_Hero_Map::iterator find_it = master_id_fight_hero_map_.find(master_id);
	if (find_it != master_id_fight_hero_map_.end()) {
		master_id_fight_hero_map_.unbind(master_id);
	}
	return 0;
}

#endif /* SCENE_PUBLIC_H_ */
