/*
 * Expedition_Config.h
 *
 *  Created on: 2014-05-05
 *      Author: xiaoliang
 */

#ifndef EXPEDITION_CONFIG_H_
#define EXPEDITION_CONFIG_H_

struct Expedition_Map_Point_Cfg {
	Expedition_Map_Point_Cfg(void) { reset(); }
	void reset(void) {
		point_id = 0;
		name.clear();
		bewrite.clear();
		type = 0;
		x = 0;
		y = 0;
		abut.clear();
		attribution = 0;
		territory.clear();
		areavalue = 0;
		monster_group.clear();
		drops.clear();
	}

	uint32_t point_id;//建筑ID
	std::string name;
	std::string bewrite;//描述
	uint32_t type;//类型
	uint16_t x;/*位置*/
	uint16_t y;/*位置*/
	UInt_Set abut; //邻近的建筑ID
	int32_t attribution;//占领所属建筑ID
	Int_Set territory;//建筑领土
	int areavalue;
	Int_Int_Map monster_group;
	Int_Vec drops;
};

typedef boost::unordered_map<uint32_t, Expedition_Map_Point_Cfg> ExpeditionMapPointCfg_Map;

struct Expedition_Gang_Home_Cfg {
	Expedition_Gang_Home_Cfg(void) { reset(); }
	void reset(void) {
		rank = 0;
		point_id = 0;
	}
	uint32_t rank;//公会排名
	uint32_t point_id;//基地ID
};

typedef boost::unordered_map<uint32_t, Expedition_Gang_Home_Cfg> ExpeditionGangHomeCfg_Map;

struct Expedition_Common_Cfg {
	Expedition_Common_Cfg(void) { reset(); }
	void reset(void) {
		key = 0;
		value = 0;
		val_2 = 0;
	}
	uint32_t key;
	uint32_t value;
	int val_2;
};

typedef boost::unordered_map<uint32_t, Expedition_Common_Cfg> ExpeditionCommonCfg_Map;

struct Expedition_Box_Data {
	Expedition_Box_Data(void) { reset(); }
	void reset(void) {
		max = 0;
		min = 0;
		percent = 0;
	}
	int max;
	int min;
	int percent;
};
typedef std::vector<Expedition_Box_Data> ExpeditionBoxData_Vec;

struct Expedition_Award_Demage_Cfg {
	Expedition_Award_Demage_Cfg(void) { reset(); }
	void reset(void) {
		gold = 0;
		souls = 0;
		modify = 0;
	}

	int gold;
	int souls;
	int modify;
};

struct Expedition_Daily_Award_Big_Data {
	Expedition_Daily_Award_Big_Data(void) {
		reset();
	}
	void reset(void) {
		nums = 0;
		amount = 0;
		award.clear();
	}
	int nums;
	int amount;
	Item_Info_Vec award;
};
typedef std::map<int, Expedition_Daily_Award_Big_Data> ExpeditionDailyAwardBigData_Map;

struct Expedition_Demage_Count_Time {
	Expedition_Demage_Count_Time(void) { reset(); }
	void reset(void) {
		low = 0;
		high = 0;
	}
	int low;
	int high;
};
typedef std::vector<Expedition_Demage_Count_Time> ExpeditionDemageCountTime_Vec;

const static int WORLD_LEVEL_MONSTER_MODIFY = 5;
struct Expedition_World_Level_Monster_Modify {
	Expedition_World_Level_Monster_Modify(void) { reset(); }
	void reset(void) {
		memset(this, 0x00, sizeof(Expedition_World_Level_Monster_Modify));
	}
	int modify[WORLD_LEVEL_MONSTER_MODIFY];			// 战斗属性改变
};
typedef std::map<int, Expedition_World_Level_Monster_Modify> Expe_WorldLevelMonsterModify_Map;

struct Expedition_Refresh_Monster {
	Expedition_Refresh_Monster(void) { reset(); }
	void reset(void) {
		id = 0;
		monster_vec.clear();
		refresh_time.clear();
	}
	int id;
	Int_Vec monster_vec;
	Int_Set refresh_time;
};
typedef std::vector<Expedition_Refresh_Monster> Expedition_Refresh_Monster_Vec;

struct Expedition_Refresh_Drop_Or_Material {
	Expedition_Refresh_Drop_Or_Material(void) { reset(); }
	void reset(void) {
		id = 0;
		monster_vec.clear();
		refresh_time.clear();
	}
	int id;
	Int_Vec monster_vec;
	Int_Set refresh_time;
};
typedef std::vector<Expedition_Refresh_Drop_Or_Material> Expedition_Refresh_Drop_Or_Material_Vec;

struct Expedition_Refresh_Monster_Kill_Gap {
	Expedition_Refresh_Monster_Kill_Gap(void) { reset(); }
	void reset(void) {
		id = 0;
		monster_vec.clear();
		refresh_time.clear();
	}
	int id;
	Int_Set monster_vec;
	Int_Vec refresh_time;
};
typedef std::vector<Expedition_Refresh_Monster_Kill_Gap> Expedition_Refresh_Monster_Kill_Gap_Vec;

#endif /* EXPEDITION_CONFIG_H_ */
