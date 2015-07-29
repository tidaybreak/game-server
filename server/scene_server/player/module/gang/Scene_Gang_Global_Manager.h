/*
 * Scene_Gang_Global_Manager.h
 *
 *  Created on: Jan 12, 2015
 *      Author: linqiyou
 */

#ifndef SCENE_GANG_GLOBAL_MANAGER_H_
#define SCENE_GANG_GLOBAL_MANAGER_H_

#include "Server_Struct.h"
#include "Module_Manager.h"
#include "Scene_Gang_Manager.h"


class Scene_Gang_Global_Manager : public Module_Manager {
public:
	typedef Object_Pool_Ex<Scene_Gang_Manager, NULL_MUTEX> Manager_Pool;
	typedef boost::unordered_map<int, Scene_Gang_Manager*> Area_SceneGangManager_Map;
	typedef boost::unordered_map<int64_t, Scene_Gang_Manager*> Int64_SceneGangManager_Map;

	Scene_Gang_Global_Manager(void);
	virtual ~Scene_Gang_Global_Manager(void);
	void reset(void);

//	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual void tick(Time_Value &now);

//	virtual void show_scene_status(void);

	// scene_gang_manager
	Gang_DB_View* gang_view_by_id(int64_t gang_id);

	std::string gang_name(int64_t gang_id);
	role_id_t gang_leader_id(int64_t gang_id);
//	bool has_member(int64_t gang_id, role_id_t role_id);
	int get_gang_level(int64_t gang_id);

	void process_matched_gangs_pair(void);
	int gang_war_load_source_gangs_from_logic(Block_Buffer &buf);
	int64_t find_server_id(const int64_t gang_id);
	int sync_to_logic(Block_Buffer &buf, const int64_t gang_id);
	int sync_to_logic_by_area(Block_Buffer &buf, const int area);
	std::string *find_zone(const int64_t gang_id);

	int set_activity_state(Block_Buffer &buf);
	int gang_war_create_16(void);
	int ready_activity(void); // 开始
	int open_activity(void); // 开始
	int close_activity(void); // 结束
	bool is_open(void);
	int set_start_time(Time_Value &now);
	int8_t stage(const int64_t gang_id);

	const Gang_War_Matched_Gang_Info *find_matched_gang_info(const int64_t gang_id);
	const Gang_War_Matched_Gangs_Pair *find_matched_gangs_pair(const int64_t scene_index, const int area);
	const Gang_War_Matched_Gangs_Pair* find_matched_gangs_pair_by_gang_id(const int64_t gang_id, const int64_t scene_index);
	int matched_handle(int64_t team_id1, int64_t team_id2); // 记录、广播匹配的两信息
	void matched_handle(Gang_War_Spot &spot);
	GangWarMatchedGangInfo_Map &gang_index_map(void);
	// scene_gang_manager

	Scene_Gang_Manager *find_manager(const int area);
	bool add_gang(const int64_t gang_id, Scene_Gang_Manager*);
	int erase_gang(const int64_t gang_id);
	Scene_Gang_Manager *find_manager_by_gang_id(const int64_t gang_id);

	int64_t make_scene_index(void);
private:
	Manager_Pool manager_pool_;
	Area_SceneGangManager_Map manager_map_;
	Int64_SceneGangManager_Map gang_id_manager_map_;
	int64_t scene_index_; //用于传送使用;递增 from 1
};

typedef Singleton<Scene_Gang_Global_Manager> Scene_Gang_Global_Manager_Single;
#define SCENE_GANG_GLOBAL_MANAGER Scene_Gang_Global_Manager_Single::instance()



#endif /* GANG_WAR_GLOBAL_MANAGER_H_ */
