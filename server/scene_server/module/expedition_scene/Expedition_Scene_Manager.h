/*
 * Expedition_Scene_Manager.h
 *
 *  Created on: 2014-10-14
 *      Author: xiaoliang
 */

#ifndef EXPEDITION_SCENE_MANAGER_H_
#define EXPEDITION_SCENE_MANAGER_H_

#include "Scene_Manager.h"
#include "Expedition_Scene_Struct.h"

class Move_Scene;
class Expedition_Scene;
class Expedition_Global_Scene_Manager;

class Expedition_Scene_Manager {
public:
	typedef boost::unordered_map<uint32_t, Move_Scene *> Expedition_Scene_Map;
	typedef boost::unordered_map<int64_t, Expedition_Scene_Map> UUID_ExpeditionSceneMap_Map;
	typedef Object_Pool_Ex<Expedition_Scene, NULL_MUTEX> Scene_Pool;

	Expedition_Scene_Manager();
	virtual ~Expedition_Scene_Manager();
	void reset(void);

	virtual int init(const Monitor_Unique_ID &scene_uid, uint32_t groupid, Role_Id_Time_Map uuids);

	virtual int init_new(const Monitor_Unique_ID &scene_uid, uint32_t groupid);

	virtual int tick(Time_Value &now);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	Move_Scene *find_scene_by_cur_line(const int cur_line);
	Move_Scene *find_scene_by_cur_line(const int64_t uuid, const int cur_line);

	/*********************************************/
	int data_channle(Block_Buffer& buf);//逻辑服到场景服的数据接口通道
	int add_uuid_opentime_map(const int64_t uuid, const Time_Value open_time);

	void load_common_scene_data(void);
	void load_all_map_data(Expedition_Scene_Map &expedition_scene_map);

	//tick检测重置场景数据
	void tick_check_reset_scene_map_data(Time_Value &now);
	//tick检测所有新公会拖入远征场景并生成城堡数据
	void tick_check_all_new_gang_to_scene_make_data(Time_Value &now);
	void tick_send_logic_gang_info(Time_Value &now);
	//通知对应的逻辑服的公会所在线号
	int send_to_server_gang_on_line_no(int64_t server_uuid, int64_t gang_id, uint32_t cur_line, Coord &coord);

	int add_gang_to_scene(const int64_t gang_id, const bool need_try_again = true);
	int del_gang_to_scene(const int64_t gang_id);

	int find_point_id_by_gang_id(const int64_t gang_id);
	int find_cur_line_by_gang_id(const int64_t gang_id);
	Expedition_Gang_Rank_Info *find_gang_rank_info_by_gang_id(const int64_t gang_id);

	void save_tick(void);
	void save_tick_direct(void);

	int line_nums(void);
	const Expedition_Gang_Rank_Info_Vec *gang_rank(const int64_t uuid);
	const Time_Value &last_reset_time(void);
	uint32_t &get_inter(void);
	void make_ranking_num_info_on_vec_for_rank(void);//对数据进行排名名次索引的建立
	void sort_gang_rank_info_to_vec(void);
	Time_Value *find_open_time_by_uuid(const int64_t uuid);

	friend class Expedition_Global_Scene_Manager;

private:
	Time_Value get_day_zero_time(const Time_Value &date);
	void sort_gang_rank_info_to_vec(const int64_t uuid, const Role_Vec &gang_ids);
	int add_gang_rank_info(const int64_t uuid, const Role_Vec &gang_ids);
	Expedition_Gang_Rank_Info* find_gang_rank_info(const int64_t uuid, const int64_t gang_id);
	int del_gang_rank_info(const int64_t uuid, const int64_t gang_id);
	void make_ranking_num_info_on_vec(void);//对数据进行排名名次索引的建立
	//gi1 和 gi2 比较大小; 大返回true; 小返回false
//	bool expedition_gang_rank_info_compare(Expedition_Gang_Rank_Info *gi1, Expedition_Gang_Rank_Info *gi2);


	//向所有逻辑服发送得到公会排名信息的消息
	void tick_send_get_gang_rank_info_to_all_logic_server(void);
	void tick_init(Time_Value &now);
	void tick_add_init(Time_Value &now);
	//返回一个场景;最后一个或者还有空间的;或者一个新的
	Expedition_Scene* get_one_available_scene(const int64_t gang_id);


	// db load init --
	Expedition_Scene* get_scene_by_db(const int line, const int64_t gang_id, const bool is_first_month);
	int set_data_by_db(const Expedition_Manager_Data &data);
	void init_gang_rank_map(void);
	void init_uuid_gang_rank_map(void);
	void init_scene_map(const Expedition_Manager_Data &data);
	void init_uuid_scene_map(const Expedition_Manager_Data &data);
	// -- db load init


private:
	Scene_Pool scene_pool_;
	Expedition_Scene_Map scene_map_;
	UUID_ExpeditionSceneMap_Map uuid_scene_map_;

	Expedition_Gang_Rank_Info_Pool gang_rank_info_pool_;
	Monitor_Unique_ID mmuid_;
	uint32_t inter_;

	Role_Id_Time_Map uuid_opentime_map_;//服务器的开服时间
	Role_Id_Time_Map opened_uuid_opentime_map_;//服务器的开服时间
	Role_RoleHashSet_Map uuid_gang_id_map_;
	RoleID_RoleID_Map gang_id_uuid_map_;
	Time_Value first_listen_time_;
	Role_Id_Time_Map uuid_first_listen_time_;

	Time_Value last_reset_time_;//最后一次重置地图时间
	Role_UInt_Map first_gang_rank_;//首次公会排名名次

	bool has_init_;
	Role_Bool_Map uuid_has_init_;
	Expedition_Gang_Rank_Info_Vec gang_rank_;//公会排名
	Expedition_Gang_Rank_Info_Map gang_rank_map_;//公会信息图
	Role_ExpeditionGangRankInfoVec_Map uuid_gang_rank_;
	Role_ExpeditionGangRankInfoMap_Map uuid_gang_rank_map_;

	Role_Hash_Set gang_rank_sync_waiting_;//公会排名获取等待服务器uuid列表
	Role_Hash_Set gang_rank_sync_success_;//公会排名获取成功服务器uuid列表
	Role_Id_Set gang_rank_sync_new_;//公会排名的新公会列表; 这个必须要求要有顺序
	Role_Vec gang_rank_sync_new_vec_;
//	Role_RoleIdSet_Map uuid_gang_rank_sync_new_;//公会排名的新公会列表; 这个必须要求要有顺序
//	Role_RoleVec_Map uuid_gang_rank_sync_new_vec_;
	Role_UInt_Map gang_id_point_id_;
	Role_Int_Map gang_id_cur_line_;
	Role_RoleUIntMap_Map uuid_gang_id_point_id_;
	Role_RoleIntMap_Map uuid_gang_id_cur_line_;

	Role_Hash_Set gang_ids_need_sync_to_logic_;
	Role_Hash_Set logic_start_announce_;

	bool change_;
};

#endif /* EXPEDITION_SCENE_MANAGER_H_ */
