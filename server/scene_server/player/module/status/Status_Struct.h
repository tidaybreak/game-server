/*
 * Status_Struct.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *
 */

#ifndef STATUS_STRUCT_H_
#define STATUS_STRUCT_H_

#include "Base_Struct.h"
#include "Server_Struct.h"
#include "Status_Def.h"
#include "Game_Typedef.h"
#include "fighter/Fighter_Def.h"

struct Effect_Argv_Prop {
	Effect_Argv_Prop(void) {reset(); }
	void reset(void);
	void serialize(Block_Buffer &buf) const;
	void deserialize(Block_Buffer &buf);

	/*中毒时，用于存攻方的攻击力*/
	double effect_value;
	int i_effect_val1;//prop_id
	int i_effect_val2;//
	int i_effect_val3;
	double d_effect_val1;//fixed
	double d_effect_val2;//pecent
	double d_effect_val3;//cover
	double d_effect_val4;//other
	double d_effect_val5; // 中毒时，取施法者物理/法术攻击力
	double d_effect_val6; // 中毒时，伤害修正(乘法)
	double d_effect_val7; // 中毒时，物理/法术伤害加成(乘法)
	double d_effect_val8; // 中毒时，PK伤害加成(乘法)
	double d_effect_val9; // 中毒时，种族伤害加成(乘法)
};
typedef std::vector<Effect_Argv_Prop> Effect_Argv_Prop_Vec;
struct Status_Effect_Argv {
	Status_Effect_Argv(void) { reset(); }
	void reset(void);
	void dispath(void);//处理disappear_gap成last_time_ms、cfg_action_times、cfg_trigger_times
	int add_last_time_ms(const int &last_ms);//要disappear_gap、status_id两个参数都有之后
	bool check_last_time(void);

	void serialize(Block_Buffer &buf) const;
	void deserialize(Block_Buffer &buf);

	int status_id;
	int64_t last_time_ms;
	int cfg_action_gap;
	int cfg_trigger_gap;
	int status_from;
	role_id_t caster_id;
	int overlay;
	Int_Vec disappear_gap;
	Effect_Argv_Prop_Vec data;
	bool is_fight_buff;
	int other_id;
};

struct Status_Effect_Detail {
	enum Effect_Type {
		REPLACE_EFFECT = 0,
		OVERLAP_EFFECT = 1,
		KEEP_EFFECT = 2,
		REPLACE_OVERLAP_EFFECT = 3, // 同种替换,同id累加
	};

	enum Time_Type {
		REFRESH_TIME = 0,
		OVERLAP_TIME = 1,
		REFRESH_OVERLAP_TIME = 2	// 同种替换,同id累加
	};

	enum Same_Handle {
		OVERLAP = 0,			// 直接覆盖
		COEXIST = 1,			// 不同人释放的并存 否则覆盖
		SAME_ROLE_SKIP_OR_MERGE = 2,	// 同一个人释放的跳过 否则覆盖
		REPLACE_STATUS = 3,				// 直接替换
		IGNORE	= 4,					// 忽略
		SAME_ROLE_SKIP_OR_INSERT = 5,	// 不同人释放的直接插入
		DIRECT_INSERT = 6,				// 直接插入新状态
		ERASE_AND_INSERT = 7,			// 移除原来的，插入最新的
		SAME_ROLE_MERGE = 8,	// 同一个人叠加
	};

	Status_Effect_Detail(void) {
		reset();
	}

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	void reset(void);

	void set_time(int ms_interval);
	void set_delay_time(int ms_interval);
	int ms_interval(void) const ;
	int delay_ms_interval(void) const ;

	int32_t effect_id;
	double effect_value;

	int i_effect_value1;//prop_id
	int i_effect_value2;
	int i_effect_value3;
	double d_effect_value1;//fixed
	double d_effect_value2;//percent
	double d_effect_value3;//cover
	double d_effect_value4;//目前:Status_Effect_Type_CURE_BY_PROP
	double d_effect_value5; // 中毒时，攻方的法术攻击力/物理攻击力
	double d_effect_value6; // 中毒时，攻方的乘法伤害修正
	double d_effect_value7; // 中毒时，攻方的物理/法术伤害加成(乘法)
	double d_effect_value8; // 中毒时，PK伤害加成(乘法)
	double d_effect_value9; // 中毒时，种族伤害加成(乘法)
//	Int_Double_Map fighter_prop_modify;	// 对fighter属性的修改先保存起来，删除状态的时候要还原
//	Int_Vec int_vec;

	int32_t effect_type;	// 效果类型
	int32_t status_id;
	Int_Hash_Set heat_type;	// 心跳类型
	int effect_times;		// 已经生效次数,用于删除buff时
	int overlay;			// 叠加层数，0表示不生效，用于buff可叠effect不可叠时
	bool is_overlay;		// 配置是否可叠加
	bool is_hit;			// 配置是否受状态命中影响
	int skill_id;			// 配置的技能id
	Status_Effect_Argv trigger_buff;//配置的触发BUFF
	double buff_chance;		// 配置的触发概率
	role_id_t caster_id;	// buff释放者

	int action_times;		//
	int action_tick;		// 配置的生效次数
	int passive_times;		//
	int passive_tick;		// 配置的受击次数
	Time_Value next_opt;	// 下次操作时间
	Time_Value interval;	// 操作间隔 interval 为0，表示永久生效且只生效一次

	bool is_delay;				// 配置的延迟生效
	int delay_times;			// 被延迟的生效次数
	Time_Value delay_time_ms;	// 被延迟的时间
	int delay_action_times;		// 被延迟的出手次数
	int delay_passive_times;	// 被延迟的受击次数
	Time_Value delay_time_tick;	// 配置的延迟时间
	int delay_action_tick;		// 配置的延迟出手次数
	int delay_passive_tick;		// 配置的延迟受击次数

	// Status_Effect_Type_CURE_BY_PROP
	double last_cure_value;//治疗效果：施放者下线时，使用上次的值，即此值
	double last_cure_rate;//治疗效果：施放者下线时，使用上次的值，即此值

	Int_Vec buff_scene;		// 配置的指定场景有效;(buff层)未填不限定
};

struct Status_Info_Detail {
	typedef std::vector<Status_Effect_Detail> Effect_Vec;

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	int set(int status_id_, role_id_t caster_role_id_, int status_from = 1, role_id_t caster_master_id_ = 0, int overlay_ = 0);

	int32_t status_id;
	int status_from;			// buff出处
	role_id_t caster_role_id;	// 状态释放者id
	role_id_t caster_master_id;	// 主人id
	int overlay;				// 叠加数量 从1开始

//	bool is_pause;				// 是否在冻结状态
	Effect_Vec effect_map;
};

struct Status_Info {
	typedef boost::unordered_map<role_id_t, Status_Info_Detail> Status_Info_Map;
	Status_Info(void);
	~Status_Info(void);
	void reset(void);
	void serialize(Block_Buffer &buf) const;
	void deserialize(Block_Buffer &buf);

	int set(int status_id, role_id_t caster_role_id, int status_from = 0, role_id_t caster_master_id = 0);

	int status_id_;
	int overlay_;
	int action_times;				// 出手次数
	int trigger_times;				// 触发次数
	int cfg_action_times;			// 配置的出手次数
	int cfg_trigger_times;			// 配置的触发次数
	int last_status_from_;			// buff出处
	role_id_t last_caster_role_id_;	// 状态释放者id
	role_id_t last_caster_master_id_;	// 主人id
	bool is_fight_buff; 				// 是否是战斗buff
	Int_Vec buff_scene;		// 配置的指定场景有效;未填不限定
	Int_Vec scene_visible; // 是否在场景显示

	int64_t last_time_ms;
	Time_Value last_start_time;// 最新开始时间
	Time_Value last_end_time; // 最新结束时间

	std::deque<role_id_t> status_fifo; // 新旧buff判断
	Status_Info_Map status_info_map;

	int other_id;
};

// 状态运行时信息
struct Status_Running_Effect {
	Int_Int_Map hurt_link_map;						// 是否被伤害链
	Int_Int_Map cure_link_map;						// 是否被治疗链
	Int_Int_Map	stun_map;							// 是否被眩晕
	Int_Int_Map	stoic_map;							// 是否被霸体
	Int_Int_Map taunt_map;							// 是否被嘲讽
	Int_Int_Map lock_map;							// 是否被锁定
	Int_Int_Map invincible_map;						// 是否无敌
	Int_Int_Map	silent_map;							// 是否被沉默
	Int_Int_Map	confusion_map;						// 是否被混乱
	Int_Double_Map exp_map;							// 用于计算状态附加经验倍数
	Int_Int_Map shape_sheep_map;					// 变羊
	Int_Double_Map rebound_map;						// 反弹
	Int_Int_Map change_fashion_map;					// 变身
	Int_Double_Map exp_two_map;							// 用于计算状态附加经验倍数 two
	Int_Double_Map exp_activity_map;
//	Int_Double_Map lifesteal_map;					// 用于计算状态附加吸血
//	Int_Double_Map hurt_absort_map;					// 用于伤害吸收
//	Int_Double_Map hurt_cert_chg_map;				// 用于伤害改变
//	Int_Double_Map hurt_hurt_bounce_map;			// 用于反弹伤害百分比
//	Int_Double_Map hurt_def_bounce_map;				// 用于反弹防御上限的百分比伤害
//	Int_Double_Map hurt_transfer_map;				// 用于伤害转移
//	Int_Double_Map cure_filt_map;					// 用于改变受到的治疗量
//	Int_Double_Map hatred_flit_map;					// 用于改变仇恨
//	RoleID_Double_Map hurt_flit_map;				// 用于伤害过滤 0 对所有伤害生效 指定role_id，只对指定role_id的攻击者生效

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	inline int speed_sum(void) const {
		int val = 0;
//		for (Int_Int_Map::const_iterator it = speed_map.begin(); it != speed_map.end(); ++it) {
//			val += (it->second);
//		}
		return val;
	}

	inline double exp_multi_sum(void) {
		double exp_multi = 0.0;
		if (!exp_two_map.empty()) {
			for (Int_Double_Map::iterator it = exp_two_map.begin(); it != exp_two_map.end(); ++it) {
				exp_multi += it->second;
			}
		} else {
			for (Int_Double_Map::iterator it = exp_map.begin(); it != exp_map.end(); ++it) {
				exp_multi += it->second;
			}
		}
		for (Int_Double_Map::iterator it = exp_activity_map.begin(); it != exp_activity_map.end(); ++it) {
			exp_multi += it->second;
		}
		return (exp_multi > 0.0) ? exp_multi : 0.0;
	}

	inline double rebound_sum(void) {
		double exp_multi = 0.0;
		for (Int_Double_Map::iterator it = rebound_map.begin(); it != rebound_map.end(); ++it) {
			exp_multi += it->second;
		}
		return (exp_multi > 0.0) ? exp_multi : 0.0;
	}

	inline double lifesteal_sum(void) {
		double lifesteal = 0.0;
//		for (Int_Double_Map::iterator it = lifesteal_map.begin(); it != lifesteal_map.end(); ++it) {
//			lifesteal += it->second;
//		}
		return (lifesteal > 0.0) ? lifesteal : 0.0;
	}

	inline double hurt_filt(role_id_t attacker) {
//		double filter = hurt_flit_map[0] + 1.0;
//		if (hurt_flit_map.count(attacker) != 0) {
//			filter += hurt_flit_map[attacker];
//		}
//		if (filter < 0.0) filter = 0.0;
		return 0.0;//filter;
	}

	inline double cure_flit(void) {
		double cure = 1.0;
//		for (Int_Double_Map::iterator it = cure_filt_map.begin(); it != cure_filt_map.end(); ++it) {
//			cure += it->second;
//		}
		return (cure > 0.0) ? cure : 0.0;
	}

	inline double hatred_flit(void) const {
		double value = 1.0;
//		for (Int_Double_Map::const_iterator it = hatred_flit_map.begin(); it != hatred_flit_map.end(); ++it) {
//			value += it->second;
//		}
		return (value > 0.0) ? value : 0.0;
	}

	inline bool is_immune_debuff(int buff_type) {
//		for (Int_Int_Map::const_iterator it = immune_debuff_map.begin(); it != immune_debuff_map.end(); ++it) {
//			if (it->second & buff_type) {
//				return true;
//			}
//		}
		return false;
	}
};

struct Status_Link_Detail {
	Status_Link_Detail(void) { reset(); }
	void reset(void) {
		main = 0.0;
		dis = 0.0;
	}
	double main;
	double dis;
};
typedef boost::unordered_map<int, Status_Link_Detail> Status_Link;

const int STATUS_BUCKET_SIZE = 32;
struct Status_Detail {
	typedef boost::unordered_map<int32_t, Status_Info*> Status_Map;
	typedef std::list<Status_Info *> Status_Info_List;
	typedef std::list<Status_Effect_Detail *> Status_Effect_List;

	Status_Detail(void);
	~Status_Detail(void){}

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	void reset(void);

	role_id_t role_id;
	Status_Map status_map;
	Int_Hash_Set status_dispel_set;
	Int_Hash_Set status_pure_set;
	Int_Hash_Set status_stoic_set;
	Int_Hash_Set status_shield_set;
	Int_Hash_Set status_evil_set; // 恶性buff

	double shield_cfg; // 护盾
	double shield_hurt; // 护盾免疫的伤害
	Status_Link hurt_link;
	Status_Link cure_link;
	double cure_link_main; // 治疗系数
	double cure_link_dis; // 治疗分但系数

	Status_Info_List status_timepri_list; // buff tick
	Status_Info_List status_action_list; // buff tick
	Status_Info_List status_passive_list; // buff tick
	Status_Info_List out_status_timepri_list; // buff tick
	Status_Info_List out_status_action_list; // buff tick
	Status_Info_List out_status_passive_list; // buff tick
	Status_Effect_List timepri_list; // effect time tick
	Status_Effect_List action_list; // effect action tick
	Status_Effect_List passive_list; // effect action tick
	Status_Effect_List once_timepri_list; // effect time tick 0间隔的延迟处理

	Status_Running_Effect running_effect;
};

#endif /* STATUS_STRUCT_H_ */
