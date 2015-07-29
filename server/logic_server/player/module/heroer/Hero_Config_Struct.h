/*
 * Hero_Config_Struct.h
 *
 *  Created on: Apr 12, 2014
 *      Author: chenyulin
 */

#ifndef HERO_CONFIG_STRUCT_H_
#define HERO_CONFIG_STRUCT_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"
#include "pack/Pack_Struct.h"

struct Awake_Lvl_Cfg {
	Awake_Lvl_Cfg(void) { reset(); }
	inline void reset(void) {
		prop_val_map.clear();
	}

	Int_Double_Map prop_val_map;
};
typedef std::map<int, Awake_Lvl_Cfg> Awake_Lvl_Cfg_Map;

struct Hero_Intimacy {
	Hero_Intimacy(void) { reset(); }
	void reset(void) {
		assist_lv_limit = 0;
		lvl_limit = 0;
		exp_fix = 0.0;
		convert_fix = 0.0;
		favor_gift_type = 0.0;
		disfavor_gift_type = 0.0;
	}

	int assist_lv_limit;
	int lvl_limit;
	double exp_fix;
	double convert_fix;
	int favor_gift_type;
	int disfavor_gift_type;
};
typedef boost::unordered_map<int, Hero_Intimacy> Hero_Intimacy_Map;

struct Hero_Config {
	Hero_Config(void) { reset(); }
	void reset(void);

	int hero_id;
	std::string hero_name;
	int hero_career;
	int hero_init_lvl;
	int hero_initial_awake;
	int hero_quality;
	int hero_sm_type;
	int hero_open_lvl;
	int vip_recruit_diamond;
	int recruit_cost_soul;
	int recruit_exploit_lvl;
	int max_awake_lvl;
	double factor_lvl_up_soul;
	Int_Set artifact_ids;

	Int_Vec skill_id_vec;
	Int_Vec recruit_tasks;
	Id_Amount_Vec recruit_cost_items;

	Int_Vec awake_skill_id_vec;
	Int_IntVec_Map awake_tasks_map;
	std::vector<Id_Amount_Vec> awake_lvl_cost_items;
	Int_Vec hero_lvl_awake_limit;
	Awake_Lvl_Cfg_Map awake_lvl_cfg_map;

	Int_Double_Map hero_prop_correct;
	Int_Double_Map hero_prop_conver;

	Int_Vec assist_skill_id_vec;
	Hero_Intimacy hero_intimacy;
};
typedef boost::unordered_map<int, Hero_Config> Hero_Config_Map;


typedef boost::unordered_map<int, Int_Double_Map> Hero_Lvl_Pro_val_Cfg_Map;
typedef boost::unordered_map<int, Int_Double_Map> Hero_ID_Correct_Cfg_Map;
typedef boost::unordered_map<int, Int_Double_Map> Hero_ID_Conver_Cfg_Map;


struct Hero_Lvl_Misc_Config {
	Hero_Lvl_Misc_Config(void) { reset(); }
	void reset(void);

	int hero_lvl;
	int lvl_up_cost_souls;
};
typedef boost::unordered_map<int, Hero_Lvl_Misc_Config> Hero_Lvl_Misc_Cfg_Map;

// 神器ID配置
struct Artifact_Cfg_Id {
	Artifact_Cfg_Id(void) { reset(); }
	void reset(void) {
		artifact_id = 0;
		artifact_name.clear();
		max_lvl = 0;
		cost_soul_param = 0;
		cost_item_param = 0;
		aptitude_map.clear();
	}

	int artifact_id;
	std::string artifact_name;
	int max_lvl;
	int cost_soul_param;
	int cost_item_param;
	//Int_Double_Map aptitude_map;
	std::map<int, double> aptitude_map;
};
typedef std::map<int, Artifact_Cfg_Id> Artifact_Cfg_Id_Map;

// 神器等级配置
struct Artifact_Cfg_Lvl {
	Artifact_Cfg_Lvl(void) { reset(); }
	void reset(void) {
		artifact_lvl = 0;
		need_player_lvl = 0;
		cost_soul = 0;
		cost_items.clear();
		pro_val_map.clear();
	}

	int artifact_lvl;
	int need_player_lvl;
	int cost_soul;
	Id_Amount_Vec cost_items;

	//Int_Double_Map prop_val_map;
	std::map<int, double> pro_val_map;
};
typedef std::map<int, Artifact_Cfg_Lvl> Artifact_Cfg_Lvl_Map;

struct Intimacy_Base {
	Intimacy_Base(void) { reset(); }
	void reset(void) {
		lvl = 0;
		exp = 0;
		convert = 0.0;
	}

	int lvl;
	int exp;
	double convert;
};
typedef boost::unordered_map<int, Intimacy_Base> Intimacy_Base_Map;

struct Hero_Intimacy_Gift {
	Hero_Intimacy_Gift(void) { reset(); }
	void reset(void) {
		item_id = 0;
		type = 0;
		gift_intimacty = 0;
	}

	int item_id;
	int type;
	int gift_intimacty;
};
typedef boost::unordered_map<int, Hero_Intimacy_Gift> Hero_Intimacy_Gift_Map;

struct Intimacy_Community {
	Intimacy_Community(void) { reset(); }
	void reset(void) {
		id = 0;
		hero_id = 0;
		val.reset();
	}
	int id;
	int hero_id;
	Int_Int val;
};
typedef boost::unordered_map<int, Intimacy_Community> Intimacy_Community_Map;

#endif /* HERO_CONFIG_STRUCT_H_ */
