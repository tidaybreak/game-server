/*
 * Fighter_Struct.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *      只在场景服用
 */

#ifndef FIGHTER_STRUCT_H_
#define FIGHTER_STRUCT_H_

#include "Base_Struct.h"
#include "Server_Struct.h"
#include "Fighter_Def.h"

struct Skill_Effect;
struct Skill_Info;

// 攻击参数
struct Attack_Param {
	int skill_id;						// 技能(直接伤血忽略)
	role_id_t attacker;					// 攻击者(直接伤血忽略)
	role_id_t victim;					// 被攻击者
	double attack_hurt;					// 伤害量
	int attack_result;					// 攻击结果
	const Skill_Effect *skill_effect;	// 技能效果
	Prop_Change_Effect pc_effect;
	Prop_Change_Source pc_source;

	Attack_Param(void) { reset(); };
	Attack_Param(int skill_id_, role_id_t attacker, role_id_t peer, const Skill_Effect *effect_) :
		skill_id(skill_id_), attacker(attacker), victim(peer), attack_hurt(0.0), attack_result(0), skill_effect(effect_), pc_effect(Prop_Change_Effect::EFFECT_SHOW_ALL), pc_source(Prop_Change_Source::SOURCE_DEFAULT) { }
	void reset(void);
	void buff_serialize(Block_Buffer &buf) const;
	void buff_deserialize(Block_Buffer &buf);
};

struct Target_Select_Info {
	int select_type;		// 选择方式
	int target_relation;	// 目标关系
	Int_Set target_type;	// 目标类型
	int prior_line;			// 优先列
	int prior_row;			// 优先排
	int prior_type;			// 优先类型
	int target_amount;		// 目标数量

	Target_Select_Info(void) { reset(); };
	void reset(void);
};

struct Skill_Effect_Val_Inner {
	int val1;
	int	val2;
	bool val3;
	double val4;//fixed
	double val5;//pecent
	double val6;//cover
	double val7;//other
	Skill_Effect_Val_Inner(void){ reset(); };
	void reset(void);
};

typedef std::vector<Skill_Effect_Val_Inner> Skill_Effect_Val_Inner_Vec;

struct Skill_Effect_Val {
	int id;
	Int_Vec data1; // 对应状态的消失类型
	double data2;
	Skill_Effect_Val_Inner_Vec inner_vec;
	Int_Vec int_vec;

	Skill_Effect_Val(void) { reset(); };
	void reset(void);
};

struct Attr_Correct {
	int type;
	double fixed_value;
	double percent_value;
	double mask_value;

	Attr_Correct(void) { reset(); }
	void reset(void);
};
typedef std::vector<Attr_Correct> Attr_Correct_Vec;

struct Skill_Effect_Correct {
	int effect_id;
	Attr_Correct_Vec attr_crr_vec;

	Skill_Effect_Correct(void) { reset(); }
	void reset(void);
};
typedef boost::unordered_map<int, Skill_Effect_Correct> Skill_Correct_Map;

struct Skill_Step_Fix {
	double value;
	int id;
	int val1;
	double val2;
	double val3;

	Skill_Step_Fix(void){ reset(); }
	void reset(void);
};
typedef std::vector<Skill_Step_Fix> Skill_Step_Fix_Vec;

struct Fix_Call_Monster {
	int type;
	double value;
	double cfg_percent;

	Fix_Call_Monster(void) { reset(); }
	void reset(void);
};
typedef std::vector<Fix_Call_Monster> Fix_Call_Monster_Vec;

struct Call_Monster {
	int monster_type_id;
	int idx;
	Fix_Call_Monster_Vec fix_monster_vec;

	Call_Monster(void) { reset(); }
	void reset(void);
};

struct Transfer_Call_Monster {
	int monster_type_id;
	int idx;
	double fix_blood_percent;

	Transfer_Call_Monster(void) { reset(); }
	void reset(void);
};
typedef std::vector<Transfer_Call_Monster> Transfer_Call_Monster_Vec;

struct Skill_Effect {
	int effect_id;
	int effect_type;
	int effectgroup_id;
	int effect_series_id;
	int step_fix_type;
	double effect_rate;
	Int_Double morale_damage;
	Int_Double hp_cure;
	Time_Value continue_time;				// 持续时间
	Target_Select_Info target_sellect_info; // 目标规则
	Skill_Effect_Correct effect_correct;
	Skill_Step_Fix_Vec skill_step_fix_vec;	// 阶梯修正

	Skill_Effect_Val val1;
	Call_Monster call_monster;
	Transfer_Call_Monster_Vec transfer_call_monster_vec;

	Skill_Effect(void) { reset(); };
	void reset(void);
};
typedef boost::unordered_map<int, Skill_Effect> Skill_Effect_Map;

struct Be_Skill_Attr {
	int id;
	double fix;
	double percnet;

	Be_Skill_Attr(void) { reset(); }
	void reset(void);
};
typedef std::vector<Be_Skill_Attr> Be_Skill_Attr_Vec;

struct Be_Skill_Effect {
	int be_skill_effect_id;
	int be_fix_type;
	Int_Double_Map add_moral_map;
	Int_Vec effect_vec;
	Be_Skill_Attr_Vec attr_vec;
	Int_Vec open_skills;
	Skill_Step_Fix_Vec be_fix_steps;
	Skill_Effect_Val val1;
	Int_Int_Map fix_buff_time;
	Int_Int_Map fix_skill_time;
	Int_Double_Map fix_morale_cost_map;

	int trigger_type;
	int trigger_cd_time;			// mseconds
	int max_trigger_times;
	int trigger_threshold_val;
	Int_Vec trigger_skill;

	Be_Skill_Effect(void) { reset(); }
	void reset(void);
};
typedef boost::unordered_map<int, Be_Skill_Effect> Be_Skill_Effect_Map;

struct Skill_Cd{
	int type;
	int millisecond;

	Skill_Cd(void) { reset(); }
	void reset(void);
};
typedef std::vector<Skill_Cd> Skill_Cd_Vec;

struct Skill_Cost {
	int type;
	int coster;
	double value;

	Skill_Cost(void) { reset(); };
	void reset(void);
};
typedef std::vector<Skill_Cost> Skill_Cost_Vec;

struct Skill_Detail {
	int skill_id;
	int skill_level;
	int skill_act;
	int yl_cost;
	int goods_cost;
	int goods_cost_num;
	int lv_lim;
	int intimacy_lv;
//	int mutex_cond;
	int wake_lv;
	double force_factor;
	Int_Int_Vec pre_skill;
	Int_Vec mutex_cond_vec;
	Time_Value hurt_time;

	Skill_Cost_Vec fight_cost_vec;
	Skill_Effect_Map step_effect;
	Time_Value sing_time;
	Skill_Cd_Vec pre_cd_vec;
	Skill_Cd_Vec skill_cd_vec;
	Be_Skill_Effect_Map be_skill_effect_map;

	Skill_Detail(void) { reset(); };
	void reset(void);
};
typedef boost::unordered_map<int, Skill_Detail> Skill_Detail_Map;

struct Skill_Config {
	int skill_id;				// 技能id
	int skill_career;			// 所属职业
	int skill_series;			// 天赋系
	int skill_type;				// 技能使用类型
	int skill_sort;				// 主动还是被动技能
	int skill_affiliation;		// 所属
	int skill_attribute;		// 属性
	int skill_conflict;			// 冲突类型
	int target_conflict;		// 目标冲突
	int ignore_enemy_conflict;	// 忽略敌方冲突
	int skill_priority;			// 优先级

	Skill_Detail_Map skill_level_map;

	Skill_Config(void) { reset(); };
	void reset(void);
};
typedef boost::unordered_map<int, Skill_Config> Skill_Cache_Map;

struct Skill_Talent {
	int talent_id;
	int career;
	int lv_cond;
	int goods_id;
	int goods_cost;
	int task_cond;

	Skill_Talent(void) { reset(); };
	void reset(void);
};
typedef boost::unordered_map<int, Skill_Talent> Skill_Talent_Map;
typedef boost::unordered_map<int, Skill_Talent_Map> Skill_Talent_Cache_Map;

struct Skill_AI_Config{
	int id;
	int skill_id;
	int priority_add;
	std::vector<Id_Int_Int> strike_vec;
	Skill_AI_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Skill_AI_Config> Skill_AI_Config_Map;

struct Dynamic_Be_Skill {
	int id;
	int lv;

	Dynamic_Be_Skill (void) { reset(); };
	void reset(void);
};

typedef boost::unordered_map<int, Dynamic_Be_Skill> Fight_Be_Skill_Map;

//体力
struct Phy_Power_Cost_Config{
	int cost;
	int number;
	Phy_Power_Cost_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Phy_Power_Cost_Config> Phy_Power_Cost_Config_Map;

struct Phy_Power_Lv_Config{
	int level;
	int number;
	Phy_Power_Lv_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Phy_Power_Lv_Config> Phy_Power_Lv_Config_Map;

typedef std::vector<Skill_Info> Skill_Info_Vec;

struct Battle_Give_Up {
	enum {
		TYPE_NOT_GIVE_UP,
		TYPE_GIVE_UP,
		TYPE_SPONSOR,
	};
	role_id_t sponsor;		// 发起者
	int side;
	Time_Value give_up_time;
	Role_Int_Map give_up_map_;
};
typedef boost::unordered_map<int, Battle_Give_Up> Battle_Give_Up_Map;

struct Be_Skill_Effect_Used {
	Be_Skill_Effect_Used(void) { reset(); }
	void reset(void);

	int effect_id;
	int used_times;
	Time_Value last_used_time;
};
typedef boost::unordered_map<int, Be_Skill_Effect_Used> Be_Skill_Effect_Used_Map;

#endif /* FIGHTER_STRUCT_H_ */
