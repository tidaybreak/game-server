/*
 * Scene_Team_Manager.h
 *
 *  Created on: 2014年6月6日
 *      Author: Linqiyou
 */

#ifndef SCENE_TEAM_MANAGER_H_
#define SCENE_TEAM_MANAGER_H_


#include "Map_Team.h"

class Scene_Team_Manager {
public:
	Scene_Team_Manager(void);
	~Scene_Team_Manager(void);

	Map_Team *get_team(int64_t team_id);
	int add_team(Block_Buffer &buf);
	int del_team(int64_t team_id);

	void reset(void);

private:
	UInt_Map_Team_Map_Detail team_map_;			// 队伍 id->队伍 映射表
	UInt_UInt_Map team_index_id_map_;		// 队伍 index->id 映射表
};
typedef Singleton<Scene_Team_Manager> Scene_Team_Manager_Single;
#define SCENE_TEAM_MANAGER Scene_Team_Manager_Single::instance()

#endif /* SCENE_TEAM_MANAGER_H_ */
