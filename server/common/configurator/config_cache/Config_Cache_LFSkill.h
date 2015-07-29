/*
 * Config_Cache_LFSkill.h
 *
 *  Created on: 3 11, 2015
 *      Author: zhoujin
 */

#ifndef CONFIG_CACHE_LFSKILL_H_
#define CONFIG_CACHE_LFSKILL_H_

#include "Cache_Base.h"
#include "Base_Struct.h"

struct LFSkill_Key {
	int skill_id;          //id能得到阶数，1开头的id是收集技能，2开头的id是卡牌制作技能，3开头的id是熔炼技能
	uint8_t level;
	void inline reset(void) {
		skill_id = 0;
		level = 0;
	}
	inline bool operator== (const LFSkill_Key& key) const {
		return skill_id == key.skill_id && level == key.level;
	}
};

inline size_t hash_value(const LFSkill_Key& key) {
 size_t seed = 0;
 boost::hash_combine(seed,key.skill_id);
 boost::hash_combine(seed,key.level);
 return seed;
}

struct Skill_Con_Unit {
	int skill_id;
	int skilled_max;                //
	int skilled_once;
	uint32_t cast_gold_once;
	void inline reset(void) {
		skill_id = 0;
		skilled_max = 0;
		skilled_once = 0;
		cast_gold_once = 0;
	}
};

struct Collect_Unit : public Skill_Con_Unit {

	double base_success;  //  基准概率
	double refine_sucess;  // 加成概率
	int refine_cost;
	void inline reset(void) {
		Skill_Con_Unit::reset();
		base_success = 0.0;
		refine_sucess = 0.0;
		refine_cost = 0;
	}
};

struct Card_Make_Unit : public Skill_Con_Unit {

	double base_success;  //  基准概率
	double refine_sucess;  // 加成概率
	int make_cost;
	int energy_cost;
	std::vector<Int_Int> meterial_vec;
	std::vector<Int_Int> card_vec;
	std::vector<Int_Int> change_vec;
	void inline reset(void) {
		Skill_Con_Unit::reset();
		base_success = 0.0;
		refine_sucess = 0.0;
		make_cost = 0;
		energy_cost = 0;
		meterial_vec.clear();
		card_vec.clear();
		change_vec.clear();
	}
};

struct Smelt_Unie : public Skill_Con_Unit {

	void inline reset(void) {
		Skill_Con_Unit::reset();
	}
};

struct LFSkill_Base {
	LFSkill_Base(void){reset();}
	int skill_id;           //id 4位数字，1开头的id是收集技能，2开头的id是卡牌制作技能，3开头的id是熔炼技能 id除以1000得到类型，最后两位是等级   第二位是阶
	//uint8_t type;
	int next_id;            //升级后id
	int skill_cost;

	Collect_Unit collect_unit;
	Card_Make_Unit card_unit;
	Smelt_Unie smelt_unit;

	void inline reset(void) {
		skill_id = 0;
		next_id = 0;
		skill_cost = 0;
		collect_unit.reset();
		card_unit.reset();
		smelt_unit.reset();
	}
};

typedef boost::unordered_map<int,LFSkill_Base> LFSkill_Base_Map;

struct Smelt_Tab {
	Smelt_Tab(void){ reset();}
	int id;               //第一位是大标签，后两位是小标签
	uint32_t item_id;
	std::vector<Int_Int> meterial_vec;
    int skill_id;         // 最小技能等级
    int smelt_cost;
    int energy_cost;
    double base_success;  //  基准概率    千分比
    void inline reset(void) {
    	id = 0;
    	item_id = 0;
    	meterial_vec.clear();
    	skill_id = 0;
    	smelt_cost = 0;
    	energy_cost = 0;
    	base_success = 0.0;
    }
};
typedef boost::unordered_map<int,Smelt_Tab> Smelt_Tab_Map;

struct Life_Skill_Comm {
	Life_Skill_Comm(void){ reset();}
	int level;   //生活技能开启等级
	std::vector<double> card_probability;   // 卡牌制作跨阶修正概率
	std::vector<double> smelt_lv_probability;   // 熔炼跨级修正概率
	std::vector<double> smelt_probability;    // 熔炼跨阶修正概率
	void inline reset(void) {
		level = 0;
		card_probability.clear();
		smelt_lv_probability.clear();
		smelt_probability.clear();
	}
};

struct Gather_Con_Unit {
	Gather_Con_Unit(void){ reset();}
	int id;
	uint16_t energy_cost;
	uint8_t gather_num;
	int skill_id;
	Int_Vec award_vec;
	Int_Vec chance_vec;
	void inline reset(void) {
		id = 0;
		energy_cost = 0;
		gather_num = 0;
		skill_id = 0;
		award_vec.clear();
		chance_vec.clear();
	}
};
typedef boost::unordered_map<int,Gather_Con_Unit> Gather_Cof_Map;

struct Gather_Change_Cof {
	Gather_Change_Cof(void){ reset();}
	int level;
	Int_Vec gather_lv_chance;
	void inline reset(void) {
		level = 0;
		gather_lv_chance.clear();
	}
};
typedef boost::unordered_map<int,Gather_Change_Cof> Gather_Change_Map;

typedef boost::unordered_map<int,Collect_Unit> Collect_Unit_Map;
typedef boost::unordered_map<int,Card_Make_Unit> Card_Unit_Map;
typedef boost::unordered_map<int,Smelt_Unie> Smelt_Unit_Map;

class Config_Cache_LFSkill : public Cache_Base {
public:
	void refresh_config_cache(void);
	const LFSkill_Base *lfskill_base_config(const int skill_id) const;
	const Smelt_Tab *smelt_tab_config(const int id) const;
	const Life_Skill_Comm &common_config() const;
	const Gather_Con_Unit *gather_config(const int id) const;
	const Gather_Change_Cof *gather_change_config(const int level) const;
	void refresh_life_skill(void);
	void refresh_refine(void);
	void refresh_card(void);
	void refresh_smelt(void);
	void refresh_smelt_product(void);
	void refresh_other_config(void);
	void refresh_gather_config(void);
	void refresh_gather_other_config(void);
private:
	LFSkill_Base_Map lfskill_base_map;
	Smelt_Tab_Map tab_map;
	Life_Skill_Comm com_config;
	Gather_Cof_Map gather_cof_map;
	Gather_Change_Map gather_change_map;
};

typedef Singleton<Config_Cache_LFSkill> Config_Cache_LFSkill_Single;
#define CONFIG_CACHE_LFSKILL    (Config_Cache_LFSkill_Single::instance())

#endif /* CONFIG_CACHE_LFSKILL_H_ */
