/*
 * Expedition_Global_Scene_Manager.h
 *
 *  Created on: 2014-10-14
 *      Author: xiaoliang
 */

#ifndef EXPEDITION_GLOBAL_SCENE_MANAGER_H_
#define EXPEDITION_GLOBAL_SCENE_MANAGER_H_

#include "Scene_Manager.h"
#include "Expedition_Scene_Manager.h"

class Expedition_Global_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<int64_t, Expedition_Scene_Manager*> Muid_ExpeditionSceneManager_Map;
	typedef boost::unordered_map<int, Expedition_Scene_Manager*> Inter_ExpeditionSceneManager_Map;
	typedef Object_Pool_Ex<Expedition_Scene_Manager, NULL_MUTEX> Manager_Pool;

	typedef std::multimap<int64_t, int64_t, std::greater<int64_t> > Int64_Int64_Map;

	Expedition_Global_Scene_Manager();
	virtual ~Expedition_Global_Scene_Manager();
	void reset(void);
	void load(void);
	void save(void);
	void save_by_inter(const int inter);

	virtual int init(const Monitor_Unique_ID &scene_uid);
	void module_init(const Expedition_Global_Manager_Data &data);
	void module_init(const Inter_ExpeditionManagerData_Map &map);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	Expedition_Scene_Manager *find_manager_by_inter(const int inter);
	Expedition_Scene_Manager *get_manager_by_inter(const int inter);
	Expedition_Scene_Manager *find_manager_by_uuid(const int64_t uuid);
	int find_inter_by_uuid(const int64_t uuid);

	int data_channle(Block_Buffer& buf);//逻辑服到场景服的数据接口通道

	//tick检测获取所有服开服时间
	void tick_check_all_server_open_time(Time_Value &now);
	// 限定在配置时间内，获得的各服时间数据：确定最早开服的时间
	void tick_check_for_open(Time_Value &now);
	// 检测开服时间，各个区是否开启活动
	void tick_open(Time_Value &now);
	void data_save_tick(Time_Value &now);
	void data_save_tick_direct(Time_Value &now);
	void ask_world_level_tick(const Time_Value &now);
	//定时给服务器进行分组和初始化分组manager
//	void tick_check_make_group_on_server_list(Time_Value &now);
	//创建分组
//	int make_group(Role_Id_Time_Map uuids);
	inline Time_Value get_day_zero_time(const Time_Value &date);

	role_id_t make_soldier_id_max(void);

	bool has_open(const int inter);
	bool is_first_month(const int inter);

	Int_Hash_Set &save_direct_inter(void);
private:
	bool check_is_first_month(const int inter);

private:
	bool save_data_change_;

	Role_Id_Time_Map uuid_opentime_map_;//服务器的开服时间
	Role_Id_Time_Map opened_uuid_opentime_map_;//服务器的开服时间

	Role_Int_Map uuid_inter_map_;
	Int_RoleHashSet_Map inter_uuids_map_;
	Inter_ExpeditionSceneManager_Map inter_manager_map_;
	Muid_ExpeditionSceneManager_Map uuid_manager_map_;//服务器ID对应管理器

	Int_Time_Map opentime_; // 开服时间
	Int_Time_Map first_listen_time_; // 第一次获得数据的时间：限定开服后多久开启活动
	Int_Bool_Map is_first_month_; // 区是否是第一个月开启
	Int_Bool_Map has_open_; // 区:是否已开启了活动

	Monitor_Unique_ID mmuid_;
	Manager_Pool manager_pool_;

	role_id_t soldier_id_max_;
	Time_Value save_time_;

	Int_Hash_Set save_direct_inter_;

	bool switch_;

	// world_level
	Role_Hash_Set uuid_had_ask_world_level_;
};

typedef Singleton<Expedition_Global_Scene_Manager> Expedition_Global_Scene_Manager_Single;
#define EXPEDITION_GLOBAL_SCENE_MANAGER    (Expedition_Global_Scene_Manager_Single::instance())


#endif /* EXPEDITION_GLOBAL_SCENE_MANAGER_H_ */
