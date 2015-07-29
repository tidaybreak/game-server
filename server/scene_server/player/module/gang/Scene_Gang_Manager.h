/*
 * Scene_Gang_Manager.h
 *
 *  Created on: 2014年7月31日
 *      Author: Linqiyou
 */

#ifndef SCENE_GANG_MANAGER_H_
#define SCENE_GANG_MANAGER_H_

#include "Server_Struct.h"
#include "Module_Manager.h"
#include "gang/Gang_Struct.h"

class Scene_Player;
//class Player_DB_View;
struct Gang_Passed_Event;
struct Gang_Info;
struct Gang_Row_Info;
struct Ganger_Row_Info;
struct Gang_Apply_Info;
struct Gang_DB_View;
struct Ganger_Detail;
struct MSG_81070005;

struct Gang_War_Matched_Gangs_Pair;
typedef std::map<int64_t, Gang_War_Matched_Gangs_Pair> SceneIndex_GangWarMatchedGangsPair_Map;
typedef std::vector<Gang_War_Matched_Gangs_Pair> GangWarMatchedGangsPair_Vec;
typedef std::deque<Gang_War_Matched_Gangs_Pair> GangWarMatchedGangsPair_Deque;
struct Gang_War_Matched_Gang_Info;
typedef std::map<int64_t, Gang_War_Matched_Gang_Info> GangWarMatchedGangInfo_Map;
struct Gang_War_Spot;

class Scene_Gang_Manager : public Module_Manager {
public:
	enum {
		ACTIVITY_CLOSE 	= 0,
		ACTIVITY_OPEN 	= 1,
		ACTIVITY_READY = 2,
		ACTIVITY_MATCHED = 3,
		ACTIVITY_FIGHT = 3,
		GANG_WAR_CREATE_16 = 4,
		GANG_WAR_CREATE_16_END = 5,
	};
	typedef boost::unordered_map<int64_t, Gang_DB_View> Gang_Map;

	Scene_Gang_Manager();
	~Scene_Gang_Manager();
	void reset();
	virtual void tick(Time_Value &now);

	Gang_DB_View* gang_view_by_id(int64_t gang_id);

	std::string gang_name(int64_t gang_id);
	role_id_t gang_leader_id(int64_t gang_id);
//	bool has_member(int64_t gang_id, role_id_t role_id);
	int get_gang_level(int64_t gang_id);

	void process_matched_gangs_pair(void);
	int gang_war_load_source_gangs_from_logic(Block_Buffer &buf);
	int64_t find_server_id(const int64_t gang_id);
	int sync_to_logic(Block_Buffer &buf);
	int sync_to_logic(Block_Buffer &buf, const int64_t gang_id);
	std::string *find_zone(const int64_t gang_id);

	// controler===========================
	int set_activity_state(Block_Buffer &buf);
	int gang_war_create_16(void);
	int ready_activity(void); // 开始
	int open_activity(void); // 开始
	int close_activity(void); // 结束
	bool is_open(void);
	int set_start_time(Time_Value &now);

	const Gang_War_Matched_Gang_Info *find_matched_gang_info(const int64_t gang_id);
	const Gang_War_Matched_Gangs_Pair *find_matched_gangs_pair(const int64_t index);
	int matched_handle(int64_t team_id1, int64_t team_id2); // 记录、广播匹配的两信息
	void matched_handle(Gang_War_Spot &spot);
	GangWarMatchedGangInfo_Map &gang_index_map(void);
	// controler===========================

private:
	void process_source_gangs_rank_16(void); // 将公会排名
	void process_source_gangs_rank_16_handle_matched(const Role_Vec &rank_vec);
	void process_source_gangs_rank_other(void); // 8->4->2->1

	// controler===========================
	int match_arena_team(void); // 处理匹配
	int8_t make_index(void);
//	int64_t make_scene_index(void);
	int sync_to_logic(void);
	// controler===========================

public:
	inline const Gang_Map &gang_map(void);
	inline int8_t stage(void);

private:
	Scene_Gang_Manager(const Scene_Gang_Manager&);
	Scene_Gang_Manager& operator=(const Scene_Gang_Manager&);

	void dismiss_gang(Gang_DB_View& gang_view);
	Gang_Map::iterator dismiss_gang(Gang_Map::iterator &it_gang);

private:
	Gang_Map gang_map_;
	RoleID_RoleID_Map gangid_serverid_map_;
	RoleID_String_Map gangid_zone_map_;
	Role_Hash_Set server_ids_;
	int8_t stage_;
	std::map<int8_t, Gang_War_Spot> gang_war_result_map_;

	Time_Value load_source_;

	// controler=========================
	SceneIndex_GangWarMatchedGangsPair_Map scene_index_gang_map_;//索引-匹配信息
	GangWarMatchedGangsPair_Vec gang_vec_;
	Time_Value last_match_time_;

	GangWarMatchedGangInfo_Map gang_index_map_; // 战场索引
	int8_t index_; // 场次索引
	int8_t switch_;		// 开关
	Role_Vec rank_info_vec_; // 公会战力排名
//	int64_t scene_index_; //用于传送使用;递增 from 1
	// controler=========================
};

//typedef Singleton<Scene_Gang_Manager> Scene_Gang_Manager_Single;
//#define SCENE_GANG_MANAGER Scene_Gang_Manager_Single::instance()

/////////////////////////////////////////////////////////////////////////wyn//////////////////

inline const Scene_Gang_Manager::Gang_Map &Scene_Gang_Manager::gang_map(void) {
	return gang_map_;
}
inline int8_t Scene_Gang_Manager::stage(void) {
	return stage_;
}


#endif /* SCENE_GANG_MANAGER_H_ */
