/*
 * Gang_War_Struct.h
 *
 *  Created on: 2014年8月5日
 *      Author: Linqiyou
 */

#ifndef GANG_WAR_STRUCT_H_
#define GANG_WAR_STRUCT_H_

#include "Game_Header.h"
#include "Game_Typedef.h"

#define LEAGUE_COUNT 2
#define MAX_GANG_WAR_SCENE_PLAYER_NUM 5000

#define WEIGHT_GANG  10000
#define WEIGHT_SIDE  1000
#define WEIGHT_FORCE 100

#define MAX_GANG_WAR_SCENE_MERIT 3600

#define WAR_GANG_SCENE_FORBID_TIME 0

#define WAR_SCENE_LEVEL_LIMIT 30

#define WAR_SCENE_ZONE_STAGE 5

enum GANG_CAMP{
	GANG_LIGHT_LEAGUE = 1,
	GANG_DARK_LEAGUE = 2,
};

enum Gang_War_Scene_Notice{
	GANG_WAR_SCENE_NOTICE_BE_KILL = 1,
	GANG_WAR_SCENE_NOTICE_KILL    = 2,
	GANG_WAR_SCENE_NOTICE_MINE    = 3,
	GANG_WAR_SCENE_NOTICE_WIN		 = 4,
	GANG_WAR_SCENE_NOTICE_LOSE    = 5,
	GANG_WAR_SCENE_NOTICE_ASSISTS = 6,
	GANG_WAR_SCENE_NOTICE_BLESS   = 7,
	GANG_WAR_SCENE_NOTICE_REDUCE  = 8,
};

enum Gang_War_Scene_Announce{
	GANG_WAR_SCENE_ANNOUNCE_END_STREAK_WIN  	= 1205,
	GANG_WAR_SCENE_ANNOUNCE_KILL_GUARD  	   = 1206,
	GANG_WAR_SCENE_ANNOUNCE_KILL_L_OFFICER     = 1207,
	GANG_WAR_SCENE_ANNOUNCE_KILL_D_OFFICER  	= 1208,
	GANG_WAR_SCENE_ANNOUNCE_WAR_CLOSE  	   = 1209,
	GANG_WAR_SCENE_ANNOUNCE_WAR_WIN  	   	= 1210,
	GANG_WAR_SCENE_ANNOUNCE_STREAK_WIN_3  	= 1211,
	GANG_WAR_SCENE_ANNOUNCE_STREAK_WIN_4  	= 1212,
	GANG_WAR_SCENE_ANNOUNCE_STREAK_WIN_5  	= 1213,
	GANG_WAR_SCENE_ANNOUNCE_STREAK_WIN_6  	= 1214,
	GANG_WAR_SCENE_ANNOUNCE_STREAK_WIN_7  	= 1215,
	GANG_WAR_SCENE_ANNOUNCE_STREAK_WIN_8  	= 1216,
	GANG_WAR_SCENE_ANNOUNCE_L_OFFICE_REVIVE  = 1217,
	GANG_WAR_SCENE_ANNOUNCE_D_OFFICE_REVIVE  = 1218,
};

struct Gang_War_Rank_Detail {
	role_id_t role_id;
	std::string role_name;
	int64_t gang_id;
	std::string gang_name;
	int8_t headship;
	int league;
	int score;
	double src_hp;
	double hp;
	int kill_num;
	int die_num;
	int tower_num;
	int merit;
	int total_merit;
	int contribute;
	int revive;
	Time_Value revive_time;
	Time_Value safe_time;
	int force;
	Time_Value skill_cd_time;
	Time_Value wait_time;
	bool exit_;
	Gang_War_Rank_Detail(void);
	void reset(void);
	void serialize(Block_Buffer &buf) const;
	void deserialize(Block_Buffer &buf);
	int add_merit(const int merit);
	int add_contribute(const int contribute);
};

struct Scene_Ganger_Detail{
	role_id_t role_id;
	int level;
	int force;
	role_id_t gang_id;
	int total_merit;
	int remain_income_num;
	int income_rate;
	int league;
	int remain_time;
	role_id_t honor_field_id;

	Scene_Ganger_Detail(void);
	void reset(void);
};
typedef boost::unordered_map<role_id_t, Scene_Ganger_Detail> Scene_Ganger_Map;

//struct Gang_War_Scene_Info {
//	role_id_t id;
//	int remain_time;
//	Gang_War_Scene_Info(void);
//	void reset(void);
//};
//typedef boost::unordered_map<role_id_t, Gang_War_Scene_Info> GangWarSceneInfo_Map;

struct Gang_War_Material_Info {
	Gang_War_Material_Info(void) { reset(); }
	void reset(void) {
		id = 0;
		t = Time_Value::zero;
		camp = 0;
		index = 0;
		role_id = 0;
		role_name.clear();
		npc_type_id = 0;
		other_role.clear();
	}
	int id;
	Time_Value t;
	int8_t camp;
	int8_t index;
	role_id_t role_id;
	std::string role_name;
	int npc_type_id;
	Role_Id_Set other_role;
};
typedef std::map<int8_t, Gang_War_Material_Info> Index_GangWarMaterialInfo_Map;
typedef std::list<Gang_War_Material_Info*> GangWarMaterialInfo_List;
typedef boost::unordered_map<int8_t, Index_GangWarMaterialInfo_Map> Camp_IndexGangWarMaterialInfoMap_Map;

#endif /* GANG_WAR_STRUCT_H_ */
