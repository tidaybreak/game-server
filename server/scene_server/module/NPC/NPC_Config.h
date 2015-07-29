/*
 * NPC_Config.h
 *
 *  Created on: 2014年4月1日
 *      Author: root
 */

#ifndef NPC_CONFIG_H_
#define NPC_CONFIG_H_

#include "Game_Header.h"
#include "Base_Struct.h"
#include "Server_Struct.h"
#include "status/Status_Struct.h"

enum{
	MONSTER_AI_C_NULL = 0,
	MONSTER_AI_C_HP 	= 1,
	MONSTER_AI_C_MP	= 2,
	MONSTER_AI_C_DIE  = 3,
	MONSTER_AI_C_SKILL= 4,
	MONSTER_AI_C_TIMES= 5,
	MONSTER_AI_C_TIME = 6,
	MONSTER_AI_C_PLOT = 7,
	MONSTER_AI_C_TALK = 8,
	MONSTER_AI_C_AI   = 9,					// ai触发
};

enum{
	MONSTER_AI_A_NULL 			= 0,
	MONSTER_AI_A_LOOP_SKILLS 	= 1,
	MONSTER_AI_A_RAN_SKILLS		= 2,
	MONSTER_AI_A_ONE_SKILL  	= 3,
	MONSTER_AI_A_ONE_SKILL_BB  = 4,			// 被打断后下一回合重新使用的技能
	MONSTER_AI_A_ONE_SKILL_DL  = 5,			//	延迟使用技能 被打断后下一回合重新使用的技能 [skill_id, time]
	MONSTER_AI_A_PLOT				= 100,		//
	MONSTER_AI_A_TALK				= 200,		//
	MONSTER_AI_A_CLEAN_MP		= 300,		// 清理MP
	MONSTER_AI_A_CLEAN_BUF		= 400,		// 清理buf
	MONSTER_AI_A_REFRESH_HERO  = 500,		//
	MONSTER_AI_A_ADD_BUF		   = 600,		// 添加buf 【id，【1000】，【506100，20】】
	MONSTER_AI_A_KILL_MONSTER  = 700,		// 怪物死亡（连自己一起）
	MONSTER_AI_A_TRANSFORM     = 800,		// 怪物变身【变身类型，变身id】
};

enum {
	MACHINE_EVENT_MOVEABLE 		= 0,
	MACHINE_EVENT_REF_MONSTER  = 1,
	MACHINE_EVENT_BEAK_BACK		= 2,
	MACHINE_EVENT_BUFFER			= 3,
	MACHINE_EVENT_TAKE_LIFT		= 4,
};

enum {
	REFRESH_POINT_TYPE_MONSTER 			= 0,			// 怪物
	REFRESH_POINT_TYPE_MATERIAL 			= 1,			// 采集物
	REFRESH_POINT_TYPE_MACHINE 			= 2,			// 机关
	REFRESH_POINT_TYPE_BOX 					= 3,			// 掉落物
	REFRESH_POINT_TYPE_MONSTER_BOX 		= 4,			// 怪物或者掉落物
	REFRESH_POINT_TYPE_SCATTER 			= 5,			// 散落物
};
struct Drop_Box_Config{
	int rate_type;
	std::vector<int> item_id_vec1;
	std::vector<int> item_id_vec2;
	std::vector<int> item_id_vec3;
	std::vector<double> item_rate_vec;
	std::vector<int> item_amount_vec;
	std::vector<int> item_bind_vec;
	Drop_Box_Config(void);
	void reset(void);
	const std::vector<int>& item_id(int8_t career = 0) const;

};

struct Drop_Config{
	int id;
	std::string name;
	int jid;
	int rate_type;
	std::vector<Drop_Box_Config> box_vec;
	std::vector<double> box_rate_vec;
	int cfg_max_gt_times; // 不配1次，配0无限次
	Drop_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Drop_Config> Drop_Config_Map;

struct Material_Point_Config{
	int scene_key;
	int id;
	int material_id;
	int type;
	int lv_limit;
	int gather_time;
	int gather_times;
	int refresh_time;
	int refresh_times;
	Coord coord;
	double rate;
	std::vector<int> drops_vec;
	std::vector<int> task_vec;
	std::vector<int> event_vec;

	Material_Point_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Material_Point_Config> Material_Point_Config_Map;

struct Material_Section_Config{
	Material_Point_Config_Map material_point_map;
	Material_Section_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Material_Section_Config> Material_Section_Config_Map;


struct Machine_Point_Config{
	int scene_key;
	int id;
	int machine_id;
	int type;
	int view_range;
	int trigger_times;
	int trigger_cd;
	int gather_time;
	int refresh_time;
	int refresh_times;
	std::string start_plot;
	std::string end_plot;
	Coord coord;
	std::vector<int> event_vec;
	int acc_id;
	Coord acc_coord;

	Machine_Point_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Machine_Point_Config> Machine_Point_Config_Map;

struct Machine_Section_Config{
	Machine_Point_Config_Map point_map;
	Machine_Section_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Machine_Section_Config> Machine_Section_Config_Map;

struct Machine_Event_Config{
	int id;
	int type;
	int val_1;
	int val_2;
	int val_3;
	int val_4;
	int val_5;
	std::string words;
	Coord coord_1;
	Coord coord_2;
	Machine_Event_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Machine_Event_Config> Machine_Event_Config_Map;

struct Monster_AI_Config{
	int id;
	int pri;
	int times;
	int cd;
	int rate;
	int stop_sing;
	int c_type;
	int c_val1;
	int c_val2;
	int c_val3;
	int a_type;
	int a_val1;
	int a_val2;
	int a_val3;
	int battle_over;
	int foreshow;

	std::vector<int> skill_vec;
	std::vector<int> skill_rate_vec;

	Status_Effect_Argv buf_argv;
	Monster_AI_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Monster_AI_Config> Monster_AI_Config_Map;


///////////////////////////////////////////////////////////////////////////////////////////////////

struct Monster_Point_Config{
	typedef std::vector<Int_Int> Monster_Talk;
	int id;
	int scene_key;
	int point_type;						// 0:monster 1:material 2:machine 3:box
	int state_type;						//0：状态不变 1：采集后状态变成1
	int mult_type;							// 0: 场景唯一 1:个人唯一
	int type;								// 1：主动;2：被动;3：友善;4：小动物
	int refresh_times;					// -1：不主动刷怪 0：反复刷怪 1~n：刷怪次数
	int refresh_time;
	int refresh_con;						//刷怪条件 1，时间刷怪 2，击杀上一波后刷怪
	int con_time;							//刷怪时间
	int after_time;						//间隔时间
	int after_point;						//后置ID
	Coord birth_coord;
	int view_range;
	int move_range;
	int chase_range;
	int speed[3];
	std::vector<Int_Int> npc_vec;
	std::vector<Coord> add_coord_vec;
	std::string start_plot;
	int start_plot_time;
	std::string end_plot;
	int end_plot_time;
	std::vector<Monster_Talk> talk_vec;
	std::vector<Monster_Talk> chat_vec;
	std::vector<Int_Int> group_vec;
	Coord_Vec walk_path;
	int ia;
	int rate;
	int gather_time;
	int gt_cd;
	int gt_times;
	std::vector<int> tasks_vec;
	std::vector<int> events_vec;
	int acc_id;
	Coord acc_coord;

	int drop_type;
	int drop_mult_mode;
	double drop_mult_coe;
	int drop_lv_limit;
	double drop_lv_coe;
	std::vector<int> drops;
	std::vector<Coord> drops_coord;

	int fightMap_key;
	int monster_chance;							// 不刷怪点的概率

	Monster_Point_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Monster_Point_Config> Monster_Point_Config_Map;

///////////////////////////////////////////////////////////////////////////////////////////////////

struct Monster_Group_Config{
	int scene_key;
	int id;
	std::vector<int> monster;
	std::vector<int> wave;
	std::vector<int> pos;
	std::vector<Int_Int> chest_id;
	Monster_Group_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Monster_Group_Config> Monster_Group_Config_Map;

/////////////////////////////////////////////////////////////////////////////////////////////////////

struct Monster_Section_Config
{
	Monster_Point_Config_Map point_map;
	Monster_Group_Config_Map group_map;
	std::vector<int> path_guide_vec;
	Monster_Section_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Monster_Section_Config> Monster_Section_Config_Map;

//////////////////////////////////////////////////////////////////////////////////////////////////////

struct Monster_Chest_Config{
	Monster_Chest_Config(void){
		reset();
	}
	void reset(void){
		id = 0;
		chest_name = "";
		chest_priority = 0;
		dropid.clear();
		drop_type = 0;
		soul_max = 0;
		jta = 0;
		begin_time = 0;
		finish_time = 0;
	}
	bool is_valid_time(const int time) const{
		if(begin_time == 0){
			return true;
		}
		if(time >= begin_time && time <= finish_time){
			return true;
		}
		return false;
	}
	int id;
	std::string chest_name;
	int chest_priority;
	std::vector<int> dropid;
	int drop_type;
	int soul_max;
	int jta;
	int begin_time;				//生效开始时间
	int finish_time;				//生效结束时间
};
typedef boost::unordered_map<int, Monster_Chest_Config> Monster_Chest_Config_Map;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct Chest_Lv_Config{
	Chest_Lv_Config(void){
		reset();
	}
	void reset(void){
		lvl = 0;
		base_correction = 0;
		item = 0;
		monster_id = 0;
	}
	int lvl;
	int base_correction;
	int item;
	int monster_id;
};
typedef boost::unordered_map<int, Chest_Lv_Config> Chest_Lv_Config_Map;

struct Vagrant_Item_Config{
	Vagrant_Item_Config(void){
		reset();
	}
	~Vagrant_Item_Config(void){

	}
	void reset(void){
		key = 0;
		monsters.clear();
		item_id = 0;
		sys_sell_type = 0;
		limit_num = 0;
		num = 0;
		cost = 0;
	}
	bool is_valid(const int monster_id){
		for(Int_Vec::iterator it = monsters.begin(); it != monsters.end(); ++it){
			if(*it == monster_id){
				return true;
			}
		}
		return false;
	}

	int key;
	Int_Vec monsters;
	int item_id;
	int sys_sell_type;
	int limit_num;
	int num;
	int cost;
};
typedef boost::unordered_map<int, Vagrant_Item_Config> Vagrant_Item_Config_Map;

struct Outpost_Trigger_Key{
	Outpost_Trigger_Key(void){
		reset();
	}
	~Outpost_Trigger_Key(void){

	}
	void reset(void){
		val_1 = 0;
		val_2 = 0;
	}
	bool operator==(const Outpost_Trigger_Key &key) const {
		return val_1 == key.val_1 && val_2 == key.val_2;
	}
	bool operator!=(const Outpost_Trigger_Key &key) const {
		return !(val_1 == key.val_1 && val_2 == key.val_2);
	}
	bool operator<(const Outpost_Trigger_Key &key) const  {
		if (val_1 == key.val_1)
			return val_2 < key.val_2;
		return val_1 < key.val_1;
	}
	int val_1;
	int val_2;
};

inline std::size_t hash_value(const Outpost_Trigger_Key &key) {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.val_1);
    boost::hash_combine(seed, key.val_2);
    return seed;
}
//
//inline bool operator==(const Outpost_Trigger_Key &a, const Outpost_Trigger_Key &b) {
//	return a.val_1 == b.val_1 && a.val_2 == b.val_2;
//}

struct Outpost_Trigger_Config{
	Outpost_Trigger_Config(void){
		reset();
	}
	~Outpost_Trigger_Config(void){

	}
	void reset(void){
		scene_id = 0;
		points.clear();
		monster_id = 0;
		chance = 0;
		type = 0;
		target_scene_id = 0;
		target_scene_time = 0;
		target_scene_type = 0;
	}

	int scene_id;
	Int_Vec points;
	int monster_id;
	int chance;
	int type;						// 0、流浪商人 1、藏宝海湾
	int target_scene_id;					// 传送目的场景
	int target_scene_time;				// 场景限定时间（秒）
	int target_scene_type;				// 场景类型（1、金币 2、英灵 3、龙魂)
};
typedef boost::unordered_map<Outpost_Trigger_Key, Outpost_Trigger_Config> Outpost_Trigger_Config_Map;

struct Tbay_Gold_Npc_Config{
	Tbay_Gold_Npc_Config(void){
		reset();
	}
	~Tbay_Gold_Npc_Config(void){

	}
	void reset(void){
		cost = 0;
		sys_sell_type = 0;
		name = "";
		buff_argv.reset();
	}
	int cost;
	int sys_sell_type;
	std::string name;
	Status_Effect_Argv buff_argv;
};
typedef boost::unordered_map<int, Tbay_Gold_Npc_Config> Tbay_Gold_Npc_Config_Map;

#endif /* NPC_CONFIG_H_ */
