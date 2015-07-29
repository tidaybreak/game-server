/*
 * Expedition_Scene_Struct.h
 *
 *  Created on: 2014年10月14日
 *      Author: xiaoliang
 */

#ifndef EXPEDITION_SCENE_STRUCT_H_
#define EXPEDITION_SCENE_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"
#include "Expedition_Scene_Def.h"
#include "Base_Detail.h"
#include "NPC/hero/Hero_Struct.h"
#include "Fighter_Detail.h"

struct Expedition_Map_Point_Cfg;

struct Expedition_Force {
	Expedition_Force(void) { reset(); }
	void reset(void) {
		server_uuid = 0;
		server_name.clear();
		gang_id = 0;
		gang_name.clear();
	}
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	int64_t server_uuid;//占领该据点的服务器ID
	std::string server_name;//占领该据点的服务器名字
	int64_t gang_id;//占领该据点的公会ID
	std::string gang_name;//占领该据点的公会名字
};

struct Soldier {
	Soldier(void);
	void reset(void);

	role_id_t id;
	Expedition_Force occupier;
	uint32_t point_id;//建筑ID
	role_id_t role_id; //角色ID //非玩家类型,改表示NPCid
	std::string role_name;//角色名
	uint32_t type;//类型; 0.玩家,1.中立驻守怪,2.公会骑士团,3.精锐远征军,4.远征军,5.公会守卫npc
	uint32_t force;//战斗力
	uint32_t blood;//剩余血量
	uint32_t head_id;//头像ID
	uint32_t level; //等级
	int16_t nums;
	int16_t nums_battle;
	bool exit;
	Expedition_Battle_Hero_Info hero_info;
};
//typedef Object_Pool_Ex<Soldier, NULL_MUTEX> Soldier_Pool;
typedef boost::unordered_map<int64_t, Soldier*> Soldier_Map;
typedef std::list<Soldier*> Soldier_List;
typedef boost::unordered_map<int64_t, Soldier_Map> GangID_SoldierMap_Map;
typedef boost::unordered_map<int64_t, Soldier_List> GangID_SoldierList_Map;
typedef boost::unordered_map<role_id_t, Soldier*> Player_Soldier_Map;

struct Expedition_Fight_Info {
	Expedition_Fight_Info(void) {
		reset();
	}
	void reset(void);
	Soldier_Map defend_map; //防守队列
	Soldier_Map attack_map; //攻占队列
	Soldier_List defend_list;
	Soldier_List attack_list;
//	uint32_t npc_soldier_aid;//npc怪的id自增长

	Soldier_Map defending_map;
	Soldier_Map attacking_map;
	Time_Value battle_finish_time;

	Soldier_Map looking_defend_map;
	GangID_SoldierMap_Map looking_attack_map;
	Soldier_List looking_defend_list;
	GangID_SoldierList_Map looking_attack_list;
	int8_t fight_status; // 0(和平状态) 1(攻占状态)

	bool has_complete;
};
typedef boost::unordered_map<int64_t, Expedition_Fight_Info> Role_ExpeditionFightInfo_Map;

struct Stronghold_Info {//据点信息结构
	Stronghold_Info(void);
	void reset(void);
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	int8_t get_status(void) const;

	Expedition_Force occupier;
	Expedition_Force attacker;
	int8_t fight_status; // 0(和平状态) 1(攻占状态)
	int8_t belong_status; // 0.未被初始化状态, 1.中立, 2.占领

	Soldier_Map defend_map; //防守队列
	Soldier_Map attack_map; //攻占队列
	Soldier_List defend_list;
	Soldier_List attack_list;
//	uint32_t npc_soldier_aid;//npc怪的id自增长

	Soldier_Map defending_map;
	Soldier_Map attacking_map;
	Time_Value battle_finish_time;

	Soldier_Map looking_defend_map;
	GangID_SoldierMap_Map looking_attack_map;
	Soldier_List looking_defend_list;
	GangID_SoldierList_Map looking_attack_list;

	// for self beast
	Role_ExpeditionFightInfo_Map role_defend_map_for_beast;

	// for deserialize
	Role_Int_Map defend_id_map;
};

struct Stronghold {//据点结构
	Stronghold(void);
	void reset(void);
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	void make_from_cfg(const Expedition_Map_Point_Cfg &conf);
	void make_to_build_brief_info(Expedition_Build_Brief_Info &ebbi) const;
	void init_monster_group(const Expedition_Map_Point_Cfg &conf, const bool from_cfg = true);
	void init_defend_map_occupier(void);

	uint32_t point_id; //建筑ID
	Expedition_Build_Type type; //类型	1.基地  2.关卡	3.大城堡	4.中城堡	5.小城堡	6.宝箱区	7.野兽区
//	std::string bewrite; //描述
//	Coord location; //位置
//	UInt_Set abut; //邻近的建筑ID
//	int attribution; //占领所属建筑ID
//	Int_Set territory;//建筑领土
	int areavalue;

	Stronghold_Info info;//据点信息

	Role_Info win_attack_people;
};
typedef boost::unordered_set<Stronghold*> Stronghold_Set;
typedef std::list<Stronghold*> Stronghold_List;
typedef boost::unordered_map<uint32_t, Stronghold> Stronghold_Map;
typedef boost::unordered_map<uint32_t, Stronghold*> StrongholdPoint_Map;

typedef Object_Pool_Ex<Expedition_Gang_Rank_Info, NULL_MUTEX> Expedition_Gang_Rank_Info_Pool;
typedef boost::unordered_map<gang_id_t, Expedition_Gang_Rank_Info*> Expedition_Gang_Rank_Info_Map;
typedef boost::unordered_map<gang_id_t, Expedition_Gang_Rank_Info> Gang_ExpeditionGangRankInfo_Map;
typedef std::vector<Expedition_Gang_Rank_Info*> Expedition_Gang_Rank_Info_Vec;
typedef boost::unordered_map<role_id_t, Expedition_Gang_Rank_Info_Map> Role_ExpeditionGangRankInfoMap_Map;
typedef boost::unordered_map<role_id_t, Gang_ExpeditionGangRankInfo_Map> Role_GangExpeditionGangRankInfoMap_Map;
typedef boost::unordered_map<role_id_t, Expedition_Gang_Rank_Info_Vec> Role_ExpeditionGangRankInfoVec_Map;


struct Expedition_Build_Base {
	Expedition_Build_Base(void) { reset(); }
	void reset(void) {
		point_id = 0;
		name.clear();
		type = EBT_UNDEFINE;
		status = EBS_UNINIT;
		fight_status = EBFS_PEACE;
		belong_status = EBBS_UNINIT;
		occupier.reset();
		attacker.reset();
	}
	int point_id;
	std::string name;
	Expedition_Build_Type type;	//类型	1.基地  2.关卡	3.大城堡	4.中城堡	5.小城堡	6.宝箱区	7.野兽区
	Expedition_Build_Status status;//据点状态 0.未被初始化状态, 1.中立(和平状态), 2.中立(攻占状态), 11.占领(和平状态), 12.占领(攻占状态)
	Expedition_Build_Fight_Status fight_status;
	Expedition_Build_Belong_Status belong_status;
	Expedition_Force occupier; //占领方
	Expedition_Force attacker; //攻方
};

struct Expedition_Build_Gang_Base : virtual public Expedition_Build_Base {
	Expedition_Build_Gang_Base(void) { reset(); }
	void reset(void) {
		occupy_build.clear();
		next_time_expedition_army = Time_Value::zero;
		next_time_knightage = Time_Value::zero;
	}
	Int_Hash_Set occupy_build; // 占领建筑
	Time_Value next_time_expedition_army; // 远征军出击时间
	Time_Value next_time_knightage; // 骑士团出击时间
};


// ------------------expedition_scene manager begin ---------------

struct Expedition_Scene_Demage_Rank_Data {
	Expedition_Scene_Demage_Rank_Data(void) { reset(); }
	void reset(void) {
		role_name.clear();
		role_id = 0;
		gang_id = 0;
		gang_name.clear();
		uuid = 0;
		server_name.clear();
		force = 0;
		level = 0;
		demage = 0.0;
	}
	void serialize(Block_Buffer &w) const;
	void deserialize(Block_Buffer &r);

	std::string role_name;
	role_id_t role_id;
	int64_t gang_id;
	std::string gang_name;
	int64_t uuid;
	std::string server_name;
	int force;
	int level;
	double demage;
};
typedef boost::unordered_map<role_id_t, Expedition_Scene_Demage_Rank_Data> Role_ExpeditionSceneDemageRankData_Map;
typedef std::vector<Expedition_Scene_Demage_Rank_Data> ExpeditionSceneDemageRankData_Vec;

struct Expedition_Occupy_Castle_Data {
	Expedition_Occupy_Castle_Data(void) { reset(); };
	void reset(void);
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	void set_msg_data(Expedition_Occupy_Castle &eoc) const;

	role_id_t role_id;
	role_id_t gang_id;
	role_id_t server_uuid;
	std::string role_name;/**/
	std::string gang_name;/**/
	std::string server_name;
	int32_t level;/**/
	int32_t force;/**/
	int16_t nums;/*占领城堡数*/

	int16_t get_nums;
};
typedef boost::unordered_map<role_id_t, Expedition_Occupy_Castle_Data> Role_ExpeditionOccupyCastleData_Map;
typedef std::vector<Expedition_Occupy_Castle_Data> ExpeditionOccupyCastleData_Vec;

// drops material
struct Expedition_Drop_Or_Material {
	Expedition_Drop_Or_Material(void) { reset(); }
	void reset(void) {
		birth_time = Time_Value::zero;
		had_collected.clear();
	}
	void serialize(Block_Buffer &w) const;
	void deserialize(Block_Buffer &r);

	Time_Value birth_time;
	Role_Hash_Set had_collected;
};
typedef boost::unordered_map<int, Expedition_Drop_Or_Material> Point_ExpeditionDropOrMaterial;

struct Expedition_Scene_Data {
	Expedition_Scene_Data(void) {
		reset();
	}
	void reset(void);
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);

	Stronghold_Map map_data_;//地图的总数据; 是所有据点(大中小城堡,关卡,宝箱点,怪物点)的数据;据点里面包含所有数据(含有攻防的玩家,状态等等)
//	uint32_t map_gang_in_num_;//公会入驻个数
	Int_Uid point_gang_id_;
	uint32_t map_base_point_num_;//地图数据中基地的总数
	uint32_t cur_line_; //场景的当前线
	Time_Value award_time_;
	int settle_time_;

	Role_ExpeditionOccupyCastleData_Map occupy_castle_rank_;

//	Role_IntHashSet_Map gang_id_areavalue_;
//	Int_IntHashSet_Map base_territorys_; // conf 公会基地：不配建筑领土，程序处理
//	Role_Hash_Set gang_ids_;

	Role_ExpeditionSceneDemageRankData_Map demage_rank_;
	Point_ExpeditionDropOrMaterial refreshed_drop_or_material_;
	Int_Time_Map refreshed_monster_timing_;
	Int_Time_Map refreshed_monster_kill_gap_;
};
typedef boost::unordered_map<uint32_t, Expedition_Scene_Data> Line_ExpeditionSceneData_Map;
typedef boost::unordered_map<int64_t, Line_ExpeditionSceneData_Map> UUID_LineExpeditionSceneData_Map_Map;

struct Expedition_Manager_Data {
	Expedition_Manager_Data(void) {
		reset();
	}
	void reset(void);
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);

	uint32_t inter_;

//	Role_Id_Time_Map uuid_opentime_map_;//服务器的开服时间
	Role_Id_Time_Map opened_uuid_opentime_map_;//服务器的开服时间
//	Role_RoleHashSet_Map uuid_gang_id_map_;
	RoleID_RoleID_Map gang_id_uuid_map_;
//	Time_Value first_listen_time_;

	Time_Value last_reset_time_;//最后一次重置地图时间
	Role_UInt_Map first_gang_rank_;//首次公会排名名次

	bool has_init_;
	Role_Bool_Map uuid_has_init_;

//	Expedition_Gang_Rank_Info_Vec gang_rank_;//公会排名
	Gang_ExpeditionGangRankInfo_Map gang_rank_map_;//公会信息图
//	Inter_ExpeditionGangRankInfoVec_Map inter_gang_rank_;
	Role_GangExpeditionGangRankInfoMap_Map uuid_gang_rank_map_;

//	Role_Hash_Set gang_rank_sync_waiting_;//公会排名获取等待服务器uuid列表
	Role_Hash_Set gang_rank_sync_success_;//公会排名获取成功服务器uuid列表
//	Role_Id_Set gang_rank_sync_new_;//公会排名的新公会列表; 这个必须要求要有顺序
	Role_Vec gang_rank_sync_new_vec_;
	Role_UInt_Map gang_id_point_id_;
	Role_Int_Map gang_id_cur_line_;
	Role_RoleUIntMap_Map uuid_gang_id_point_id_;
	Role_RoleIntMap_Map uuid_gang_id_cur_line_;

	// scene--
	Line_ExpeditionSceneData_Map line_scene_map_;
	UUID_LineExpeditionSceneData_Map_Map uuid_line_scene_map_;
	// -- scene
};
typedef boost::unordered_map<uint32_t, Expedition_Manager_Data> Inter_ExpeditionManagerData_Map;

struct Expedition_Global_Manager_Data {
	Expedition_Global_Manager_Data(void) {
		reset();
	}
	void reset(void);
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);


	// global --
//	Role_Id_Time_Map uuid_opentime_map_;//服务器的开服时间
	Role_Id_Time_Map opened_uuid_opentime_map_;//服务器的开服时间

	Role_Int_Map uuid_inter_map_;
//	Int_RoleHashSet_Map inter_uuids_map_;

	Int_Time_Map opentime_; // forver
//	Time_Value first_listen_time_;
//	bool is_first_month_;
//	bool has_open_;

	Monitor_Unique_ID mmuid_;
	// -- global

//	// manager --
//	Inter_ExpeditionManagerData_Map inter_manager_data_map_;
//	// -- manager
};

struct Expedition_Scene_Info {
	Expedition_Scene_Info(void) {
		reset();
	}
	void reset(void) {
		revive_time = Time_Value::zero;
		safe_time = Time_Value::zero;
		role_id = 0;

		src_hp = 0.0;
		hp = 0.0;
		revive = 0;
		look_point.clear();
		fight_point.clear();
		is_enter_scene = 0;
	}
	Time_Value revive_time;
	Time_Value safe_time;
	role_id_t role_id;

	double src_hp;
	double hp;
	int revive;

	Int_Hash_Set look_point;
	Int_Hash_Set fight_point;

	int is_enter_scene;
};
typedef boost::unordered_map<role_id_t, Expedition_Scene_Info> Role_ExpeditionSceneInfo_Map;


// player monster
struct Expedition_Offline_Player_Data {
	Expedition_Offline_Player_Data(void) { reset(); }
	void reset(void) {
		player_base_detail.reset();
		player_fighter_detail.reset();
		hero_base_detail.reset();
		hero_fighter_detail.reset();
	}

	Base_Detail player_base_detail;
	Fighter_Detail player_fighter_detail;
	Hero_Base_Detail hero_base_detail;
	Fighter_Detail hero_fighter_detail;
};
typedef boost::unordered_map<role_id_t, Expedition_Offline_Player_Data> ExpeditionOfflinePlayerData_Map;

// ------------------expedition_scene manager end -----------------
#endif /* EXPEDITION_SCENE_STRUCT_H_ */
