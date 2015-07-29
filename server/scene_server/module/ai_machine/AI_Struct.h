/*
 * AI_Struct.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef AI_STRUCT_H_
#define AI_STRUCT_H_

#include "Base_Struct.h"
#include "Game_Typedef.h"
#include "Time_Value.h"


struct Condition {
	int condition_type;
	int i_val1;
	int i_val2;
	int i_val3;
	int i_val4;
	int i_val5;
	double d_val1;
	double d_val2;
	Time_Value t_val1;
	int reqeat_count;
	Int_Vec iv_val1;
	Coord_Vec grid_coord_vec;

	Condition(void);
	void reset(void);
};

struct Action {
	int action_type;
	int i_val1;
	int i_val2;
	int i_val3;
	int i_val4;
	int i_val5;
	int i_val6;
	int i_val7;
	int i_val8;
	int i_val9;
	double d_val1;
	double d_val2;
	std::string s_val1;
	Time_Value t_val1;
	Time_Value t_val2;
	Time_Value t_val3;
	Int_Vec iv_val1;
	Coord_Vec grid_coord_vec;
	std::vector<Action> sub_action_vec;

	Action(void);
	void reset(void);
};

struct AI_Condition_Action {
	int id;
	Time_Value do_condition_cool;		// 条件冷却时间
	std::vector<Condition> condition;	// 触发条件
	int do_action_times;				// 行为次数
	Time_Value do_action_cool;			// 行为冷却时间
	double not_do_action_probability;	// 不触发动作概率
	std::vector<Action> action;			// 触发行为

	AI_Condition_Action(void);
	void reset(void);
};
typedef boost::unordered_map<int, AI_Condition_Action> AI_Condition_Action_Map;

struct AI_Action_Running_Info {
	typedef std::set<Coord> Coord_Set;
	const AI_Condition_Action *action_config;

	int action_id;
	Time_Value last_do_action_time;
	Time_Value last_do_condition_time;
	Time_Value pending_time;

	int has_do_action_times;
	uint action_step_now;
	int i_val1;
	int i_val2;

	Coord grid_coord;
	Coord_Set grid_coord_set;
	Coord_Vec grid_coord_vec;
	Mover_Set mover_set;

	AI_Action_Running_Info(void);
	AI_Action_Running_Info(int action_id);
	void reset(void);
};
typedef std::vector<AI_Action_Running_Info> Condition_Action_Vec;

struct AI_Action_Running {
	Condition_Action_Vec condition_action_vec;
	uint condition_action_step_now;

	AI_Action_Running(void) {
		reset();
	}

	void reset(void) {
		condition_action_vec.clear();
		condition_action_step_now = 0;
	}
};

struct AI_Time_Data {
	struct Hour_Min {
		int hour;
		int min;
		Hour_Min(void);
		void reset(void);
	};
	typedef std::vector<Hour_Min> Hour_Min_Vec;

	int date_loop;
	bool no_set;
	Hour_Min_Vec date_time;
	std::vector<int> date_week;

	AI_Time_Data(void);
	void reset(void);
};

struct AI_Info {
	int id;										/// ID

	Time_Value tick_time;						///	 Tick
	Time_Value life_time;                       ///  生命(秒)
	int vision_type;							///  视野类型
	int vision_area;							///  视野范围
	int vision_always;							///	 强制视野
	Time_Value vision_tick;		  				///  视野tick
	Time_Value patrol_tick;   	 				///  巡逻频率

	int distance_of_patrol;						///  巡逻最大距离(像素)
	Time_Value fight_timeout;					///  最后一次被攻击超时离开战斗状态时间
	int distance_of_fight;						///  战斗时最大追击距离

	int exit_combat_buf;						///  脱离战斗回血buf
	Coord_Vec walk_path;						///  定点巡逻路径

	AI_Time_Data date_appear;					///  复活时间
	bool die_share;								///	 死亡共享
	bool ignore_scene;							///	 忽略场景控制开启或关闭定时器

	Int_Int_Map hatred_value;					///  不同对象仇恨值

	AI_Info(void);
	void reset(void);
};
typedef boost::unordered_map<int, AI_Info> AI_Info_Map;

#endif /* AI_STRUCT_H_ */
