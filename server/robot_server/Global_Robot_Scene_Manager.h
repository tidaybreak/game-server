/*
 * Global_Robot_Scene_Manager.h
 *
 *  Created on: Jun 11, 2014
 *      Author: xiaoliang
 */

#ifndef GLOBAL_ROBOT_SCENE_MANAGER_H_
#define GLOBAL_ROBOT_SCENE_MANAGER_H_

#include "Move_Scene.h"


struct Path_ID {
	Path_ID(void) { reset(); };
	~Path_ID() {};

	void reset(void) {
		src_scene_id = 0;
		det_scene_id = 0;
		src.reset();
		det.reset();
	}

	int src_scene_id;
	int det_scene_id;
	Coord src;
	Coord det;
};

inline std::size_t hash_value(const Path_ID &map_id) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<int>(map_id.src_scene_id));
    boost::hash_combine(seed, static_cast<int>(map_id.det_scene_id));
    boost::hash_combine(seed, static_cast<int>(map_id.src.x));
    boost::hash_combine(seed, static_cast<int>(map_id.src.y));
    boost::hash_combine(seed, static_cast<int>(map_id.src.type));
    boost::hash_combine(seed, static_cast<int>(map_id.det.x));
    boost::hash_combine(seed, static_cast<int>(map_id.det.y));
    boost::hash_combine(seed, static_cast<int>(map_id.det.type));
    return seed;
}

inline bool operator==(const Path_ID &a, const Path_ID &b) {
	return  a.src_scene_id == b.src_scene_id &&
			a.det_scene_id == b.det_scene_id &&
			a.src == b.src &&
			a.det == b.det;
}



class Global_Robot_Scene_Manager {
public:

	typedef boost::unordered_map<int, Move_Scene *> Move_Scene_Map;
	typedef Object_Pool_Ex<Move_Scene, NULL_MUTEX> Scene_Pool;
	typedef boost::unordered_map<Path_ID, Coord_Vec> Path_Map;


	Global_Robot_Scene_Manager();
	~Global_Robot_Scene_Manager();

	void reset(void);

	int init_global_scene(int idx);

	Move_Scene *find_scene(const int &muid);
	int path_find_cache(int src_scene_id, int det_scene_id, const Coord &src, const Coord &det, Coord_Vec &path);
	int path_save_cache(int src_scene_id, int det_scene_id, const Coord &src, const Coord &det, const Coord_Vec &path);

private:
	Move_Scene_Map move_scene_map_;
	Scene_Pool scene_pool_;
	Path_Map path_map_;
};

typedef Singleton<Global_Robot_Scene_Manager> Global_Robot_Scene_Manager_Single;
#define GLOBAL_ROBOT_SCENE_MANAGER    (Global_Robot_Scene_Manager_Single::instance())

#endif /* GLOBAL_ROBOT_SCENE_MANAGER_H_ */
