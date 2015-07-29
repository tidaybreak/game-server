/*
 * Fighter_Detail.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef FIGHTER_DETAIL_H_
#define FIGHTER_DETAIL_H_

#include "Base_Struct.h"
#include "Server_Struct.h"
#include "fighter/Fighter_Def.h"

struct Skill_Config;

struct Prop_Base {
	enum Calculate_Type {
		ADDITION_GOLOBAL = 0,
		ADDITION_SELF = 1
	};

	Prop_Base(void);
	Prop_Base (double basic, double fixed, double percent);

	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	double basic_value;		// 基础值
	double fixed_value;		// 加法加成
	double percent_value;	// 乘法加成
};

struct Prop_Value {
    enum ELEMADD {
        ELEM_NORMAL    	= 0,
	    ELEM_FIGHT 		= 1, // 用于计算战斗时属性(算主动技能效果)
	    ELEM_FORCE		= 2, // 用于计算战力，过滤主动技能部分及buff部分

        ELEM_ADD_END
    };


    int type;
    Prop_Base basic;						// 基础属性
    Prop_Base equip;					// 装备附加属性
    Prop_Base conversion;			// 一二级属性转换所得加成
    Prop_Base initiative_skill;		// 主动技能
    Prop_Base status;					// 状态附加属性
    Prop_Base passive_skill;		// 被动技能
    Prop_Base rune_stone;			// 符石
//    Prop_Base rune;					// 符文
    Prop_Base hero_assist;			// 英雄助阵
    Prop_Base gang;						// 公会
    Prop_Base artifacts;				// 神器
    Prop_Base achievement;		// 成就
    Prop_Base title;						// 称号
    Prop_Base dragon_vale;			// 龙谷系统
    Prop_Base anci_artifact;			// 上古神器
    Prop_Base wing;						// 翅膀
    Prop_Base awake;					// 觉醒
    Prop_Base mount;					// mount
    Prop_Base expedition;			// 远征
    Prop_Base pet;						//pet

	Prop_Value(void);

	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(int type_);

    void reset_single(const int offset = AT_BASIC);

    double total(const int elemadd = ELEM_NORMAL) const;

    double log_value_total(const int offset = AT_BASIC);

    void modify_property(const int offset, Operate_Type oen, double basic, double fixed, double percent, int op_code);

    void set_single(const double basic, const double fixed, const double percent, const int offset = AT_BASIC);
    void set_basic_single(const double value, const int offset = AT_BASIC);
    void set_fixed_single(const double value, const int offset = AT_BASIC);
    void set_percent_single(const double value, const int offset = AT_BASIC);

    void add_single(const double basic, const double fixed, const double percent, const int offset = AT_BASIC);
    void add_basic_single(const double value, const int offset = AT_BASIC);
    void add_fixed_single(const double value, const int offset = AT_BASIC);
    void add_percent_single(const double value, const int offset = AT_BASIC);

    void reduce_single(const double basic, const double fixed, const double percent, const int offset = AT_BASIC);
    void reduce_basic_single(const double value, const int offset = AT_BASIC);
    void reduce_fixed_single(const double value, const int offset = AT_BASIC);
    void reduce_percent_single(const double value, const int offset = AT_BASIC);
};

// 技能需要持久化的数据
struct Skill_DB_Info {
	int skill_id;
	int skill_level;
	int skill_loc;

	Skill_DB_Info(void) { reset();}
	void reset(void);
	int serialize(Block_Buffer &buf) const;
	int deserialize(Block_Buffer &buf);
};
typedef boost::unordered_map<int, Skill_DB_Info> Skill_DB_Info_Map;
typedef std::vector<Skill_DB_Info> Skill_DB_Info_Vec;

inline bool operator==(const Skill_DB_Info &a, const Skill_DB_Info &b) {
	return a.skill_id == b.skill_id &&
			a.skill_level == b.skill_level &&
			a.skill_loc == b.skill_loc;
}

struct Talent {
	int talent_id;
	int skill_point;
	Skill_DB_Info_Map skill_db_info_map;

	Talent(void) { reset(); }
	void reset(void);
	int serialize(Block_Buffer &buf)const;
	int deserialize(Block_Buffer &buf);
};
typedef boost::unordered_map<int, Talent> Talent_Map;

struct Skill_Running_Info {
	typedef boost::unordered_map<int, Role_Vec> Effect_Target_Map;

	int skill_id;
	int skill_level;
	int use_times;						// 使用次数
	Time_Value last_use_timestamp;		// 最后一次使用时间

	Effect_Target_Map step_target_map;	// 记录每一步选择的目标
	int last_step;						// 记录上个步骤，第二步状态只有在第一步状态中才能执行，任何状态下均可进入第一步状态

	const Skill_Config *config_cache;

	Skill_Running_Info(void);
	void reset(void);
	int serialize(Block_Buffer &buf) const;
	int deserialize(Block_Buffer &buf);
};
typedef boost::unordered_map<int, Skill_Running_Info> Skill_Running_Info_Map;

struct Common_Skill_Cd {
	int type_;
	int millisecond_;
	Time_Value last_use_;

	Common_Skill_Cd(void) { reset(); }

	void reset(void);
};
typedef boost::unordered_map<int, Common_Skill_Cd> Common_Cd_Map;

struct Fighter_Buff {
	int buff_id;
	int duration;
	double ext1;
	double ext2;
	double ext3;

	Fighter_Buff(void) { reset(); }
	void reset(void);
};
typedef std::vector<Fighter_Buff> Fgt_Buff_Vec;

template <typename PROP_VALUE, typename CHANGE_VALUE>
inline void modify_by_operate(Operate_Type oen, PROP_VALUE &prop, CHANGE_VALUE value) {
	switch (oen) {
	case O_ADD: {
		prop += value;
		break;
	}
	case O_SUB: {
		prop -= value;
		break;
	}
	case O_SET: {
		prop = value;
		break;
	}
	default: {
		MSG_USER_TRACE("Operate_Type = %d, error !\n", oen);
		break;
	}
	}
}

struct Fighter_Detail : public Detail {
	typedef std::vector<Int_Double> Prop_Value_Vec;

	enum OP_CODE {
		BASE_OP		= 1,
		FIX_OP		= 2,
		PERCENT_OP	= 4,
	};

	Fighter_Detail(void);
	virtual ~Fighter_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);
	int arena_load(void);

	Prop_Value power;					// 斗力
	Prop_Value agility;					// 敏捷
	Prop_Value dexterity;				// 灵巧
	Prop_Value armor;					// 护甲
	Prop_Value stamina;					// 耐力

	Prop_Value physical_attack;			// 物攻
	Prop_Value spell_attack;			// 法攻
	Prop_Value physical_defense;		// 物防
	Prop_Value spell_defense;			// 法防
	Prop_Value hit_rate;				// 命中
	Prop_Value avoid;					// 闪避
	Prop_Value ignore_def;				// 无视防御
	Prop_Value blood_max;				// 生命
	Prop_Value cure;					// 治疗
	Prop_Value cure_rate;				// 最终治疗乘法修正
	Prop_Value sing_rate;				// 吟唱修正
	Prop_Value crit;					// 暴击
	Prop_Value crit_def;				// 暴防
	Prop_Value init_morale;				// 初始士气
	Prop_Value morale_caps;				// 士气值上限
	Prop_Value crit_hurt;				// 暴击伤害
	Prop_Value crit_hurt_sub;			// 暴击伤害减免
	Prop_Value hit_rate_addition;		// 命中率加成
	Prop_Value avoid_rate_addition;		// 闪避率加成
	Prop_Value crit_rate_addition;		// 暴击率加成
	Prop_Value crit_def_rate_addition;	// 暴击抵抗率加成
	Prop_Value speed;					// 移动速度
	Prop_Value blood_current;			// 当前生命值
	Prop_Value morale_current;			// 当前怒气
	Prop_Value strength_current;		// 当前体力
	Prop_Value strength_max;			// 体力上限
	Prop_Value blood_pool_current;  	// 当前血池
	Prop_Value blood_pool_max;   		// 最大血池

	Prop_Value ignore_def_rate;			// 无视防御概率
	Prop_Value status_hit_rate;			// 状态命中率
	Prop_Value status_def_rate;			// 状态抵抗率
	Prop_Value blood_steal;				// 吸血

	Prop_Value phy_hurt_fix;			// 物理伤害加法修正
	Prop_Value phy_hurt_rate;			// 物理伤害乘法修正
	Prop_Value spell_hurt_fix;			// 法术伤害加法修正
	Prop_Value spell_hurt_rate;			// 法术伤害乘法修正
	Prop_Value hurt_fix;				// 最终伤害加法修正
	Prop_Value hurt_rate;				// 最终伤害乘法修正
	Prop_Value be_hurt_fix;				// 受到的伤害加法修正
	Prop_Value be_hurt_rate;			// 受到的伤害乘法修正

	Prop_Value pk_hurt_add;				// PK伤害加成
	Prop_Value pk_hurt_sub;				// PK伤害减免
	Prop_Value phy_hurt_rate_sub;		// 受到的物理伤害乘法修正
	Prop_Value spell_hurt_rate_sub;		// 受到的法术伤害乘法修正
	Prop_Value skill_hurt_rate;			// 技能伤害修正

	Prop_Value human_add;				// 人族伤害加成
	Prop_Value elf_add;					// 精灵族伤害加成
	Prop_Value orc_add;					// 兽族伤害加成
	Prop_Value dragon_add;				// 龙族伤害加成
	Prop_Value undead_add;				// 亡灵族伤害加成
	Prop_Value demon_add;				// 恶魔族伤害加成
	Prop_Value human_sub;				// 人族伤害减免
	Prop_Value elf_sub;					// 精灵族伤害减免
	Prop_Value orc_sub;					// 兽族伤害减免
	Prop_Value dragon_sub;				// 龙族伤害减免
	Prop_Value undead_sub;				// 亡灵族伤害减免
	Prop_Value demon_sub;				// 恶魔族伤害减免

	Prop_Value summon_add;				// 召唤物伤害加成
	Prop_Value summon_sub;				// 召唤物伤害减免

	Prop_Value be_cure_rate;			// 被治疗加成
	Prop_Value effect_rate;				// 效果生效修正概率

	Prop_Value pve_hurt_add;			// PVE伤害加成
	Prop_Value pve_hurt_sub;			// PVE伤害减免

	Prop_Value dizz_strengthen; 	// 晕眩强化
	Prop_Value dizz_weaken;			// 晕眩弱化

	double experience;					// 经验
	double elite_btl_morale;			// 精英副本战斗怒气

	int phy_power_recover_time;			// 体力下次恢复时间
	int phy_power_buy_times;			// 体力购买次数
    int level;
    int force;							// 战力
    int highest_force;					// 历史最高战力
    int ttl_force;						// 总战力（加上战斗英雄战力）
    int ttl_skill_points_;				// 技能点

    int default_skill;					// 默认技能
    int cur_used_talent_;				// 当前使用的天赋
    int reset_skill_times;				// 洗点次数
    Talent_Map talent_map;				// 技能天赋
    Skill_DB_Info_Vec extra_skill_vec;	// 非学习类技能

	Skill_Running_Info_Map skill_running_info_map;
	Common_Cd_Map common_cd_map_;		// 共用cd记录
	Fgt_Buff_Vec fgt_buff_vec_;			//当前身上的buff

	/**/
	bool is_clear_moral_;

    int64_t fetch_normal_property(Property_Type pen);
    int modify_normal_property(Property_Type pen, Operate_Type oen, int value);

	double fetch_fight_property(Property_Type pen, Prop_Value::ELEMADD aen = Prop_Value::ELEM_NORMAL) const;
    void modify_basic_property(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic);
    void modify_fixed_property(Property_Type pen, Addition_Type aen, Operate_Type oen, double fixed);
    void modify_percent_property(Property_Type pen, Addition_Type aen, Operate_Type oen, double percent);
    void modify_fight_property(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic, double fixed, double percent, int op_code=7);

    void update_force(void);
    double prop_force_ttl(double atk, double phy_def, double spl_def, double hp, double crit, double crit_def, double hit, double avoid);
    double get_force_part(int aen);
    double get_skill_force_ttl(void);

    int reset_skill_cost(void);

    void set_be_skill_attr(void);

    void property_list(Property_Vec  &prop_list) const;

	inline double current_blood(void) const {
		return blood_current.basic.basic_value;
	}

	inline double current_morale(void) const {
		return morale_current.basic.basic_value;
	}

	inline double max_morale(void) const {
		return morale_caps.basic.basic_value;
	}

	inline double current_strength(void) const {
		return strength_current.basic.basic_value;
	}

	inline double max_strength(void) const {
		return strength_max.basic.basic_value;
	}

	inline double current_blood_pool(void) const {
		return blood_pool_current.basic.basic_value;
	}

	inline double current_power(void) const {
		return power.total(Prop_Value::ELEM_NORMAL);
	}

};

struct Prop_Setter {
	Property_Type prop_type;/*战斗属性(一 二级)*/
	Addition_Type addi_type;/*属性附加类型(属性附加类型)*/
	Operate_Type operate;/*操作类型(加 减 等于)*/
	double basic;/*基础值*/
	double fixed;/*固定值*/
	double percent;/*百分比*/
	Prop_Setter(void) { reset(); };
	Prop_Setter(Property_Type pen, Addition_Type aen, Operate_Type oen, const double b, const double f, const double p) :
		prop_type(pen), addi_type(aen), operate(oen), basic(b), fixed(f), percent(p) { };

	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);
};
typedef std::vector<Prop_Setter> Prop_Setter_Vec;

#endif /* FIGHTER_DETAIL_H_ */
