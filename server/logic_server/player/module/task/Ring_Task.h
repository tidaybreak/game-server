/*
 * Ring_Task.h
 *
 *  Created on: Nov 10, 2014
 *      Author: root
 */

#ifndef RING_TASK_H_
#define RING_TASK_H_

#include "boost/unordered_map.hpp"

struct Ring_Task_Base_Cfg {
	Ring_Task_Base_Cfg() {
		reset();
	}
	void reset() {
		friend_ship = 0;
		diamond = 0;
		max_ring_num = 0;
		open_level = 0;
		npc_talk_chance = 0;
		collect_chance = 0;
		limit_time_kill_chance = 0;
		kill_monster_chance = 0;
		commit_consume_item_chance = 0;
		dragon_vale_op_chance = 0;
	}

	uint32_t friend_ship;
	uint32_t diamond;
	uint32_t max_ring_num;
	uint32_t open_level;
	uint8_t npc_talk_chance;
	uint8_t collect_chance;
	uint8_t limit_time_kill_chance;
	uint8_t kill_monster_chance;
	uint8_t commit_consume_item_chance;
	uint8_t dragon_vale_op_chance;
};

struct Ring_Task_Award_Cfg {
	Ring_Task_Award_Cfg() {
		reset();
	}
	void reset() {
		ring_num = 0;
		exp = 0;
		coin = 0;
		soul = 0;
		item1_id = 0;
		item1_num = 0;
		is_item1_bind = false;
		item2_id = 0;
		item2_num = 0;
		is_item2_bind = false;
		item3_id = 0;
		item3_num = 0;
		is_item3_bind = false;
		item4_id = 0;
		item4_num = 0;
		is_item4_bind = false;
	}
	uint32_t ring_num;
	uint32_t exp;
	uint32_t coin;
	uint32_t soul;

	uint32_t item1_id;
	uint32_t item1_num;
	bool is_item1_bind;

	uint32_t item2_id;
	uint32_t item2_num;
	bool is_item2_bind;

	uint32_t item3_id;
	uint32_t item3_num;
	bool is_item3_bind;

	uint32_t item4_id;
	uint32_t item4_num;
	bool is_item4_bind;
};

//key:ring_num
typedef boost::unordered_map<uint32_t, Ring_Task_Award_Cfg> Ring_Task_Award_Cfg_Map;


struct Player_Level_Award_Cfg {
	Player_Level_Award_Cfg() {
		reset();
	}
	void reset() {
		player_level = 0;
		exp_percent = 0;
		coin_percent = 0;
		soul_percent = 0;
		item1_percent = 0;
		item2_percent = 0;
		item3_percent = 0;
		item4_percent = 0;
	}
	uint32_t player_level;
	uint32_t exp_percent;
	uint32_t coin_percent;
	uint32_t soul_percent;
	uint32_t item1_percent;
	uint32_t item2_percent;
	uint32_t item3_percent;
	uint32_t item4_percent;
};

//key:player_level
typedef boost::unordered_map<uint32_t, Player_Level_Award_Cfg> Player_Level_Award_Cfg_Map;

struct Op_Ring_Type {
	Op_Ring_Type() {
		reset();
	}
	void reset() {
		op = 0;
		ring_type = 0;
	}
	uint8_t op;
	uint8_t ring_type;
};

//key:ring_num
typedef boost::unordered_map<uint32_t, Op_Ring_Type> Ring_Num_Type_Cfg_Map;

struct Ring_Monster_Cfg {
	Ring_Monster_Cfg() {
		reset();
	}
	void reset() {
		physical_attack = 0;
		magic_attack = 0;
		physical_defence = 0;
		magic_defence = 0;
		hit_rate = 0;
		avoid = 0;
		blood_max = 0;
	}
	uint32_t physical_attack;//物攻
	uint32_t magic_attack;//法攻
	uint32_t physical_defence;//物防
	uint32_t magic_defence;//法防
	uint32_t hit_rate;//命中
	uint32_t avoid;//闪避
	uint32_t blood_max;//生命
};

typedef boost::unordered_map<uint32_t, Ring_Monster_Cfg> Ring_Monster_Cfg_Map;

struct Level_Monster_Cfg {
	Level_Monster_Cfg() {
		reset();
	}
	void reset() {
		physical_attack = 0;
		magic_attack = 0;
		physical_defence = 0;
		magic_defence = 0;
		hit_rate = 0;
		avoid = 0;
		blood_max = 0;
	}
	uint32_t physical_attack;//物攻
	uint32_t magic_attack;//法攻
	uint32_t physical_defence;//物防
	uint32_t magic_defence;//法防
	uint32_t hit_rate;//命中
	uint32_t avoid;//闪避
	uint32_t blood_max;//生命
};

typedef boost::unordered_map<uint32_t, Level_Monster_Cfg> Level_Monster_Cfg_Map;

struct Ring_Task_All_Cfg {
	Ring_Task_All_Cfg() {
		reset();
	}
	void reset() {
		ring_task_base_cfg.reset();
		ring_task_award_cfg_map.clear();
		player_level_award_cfg_map.clear();
		ring_num_type_cfg_map.clear();
		ring_monster_cfg_map.clear();
		level_monster_cfg_map.clear();
	}
	Ring_Task_Base_Cfg ring_task_base_cfg;
	Ring_Task_Award_Cfg_Map ring_task_award_cfg_map;
	Player_Level_Award_Cfg_Map player_level_award_cfg_map;
	Ring_Num_Type_Cfg_Map ring_num_type_cfg_map;
	Ring_Monster_Cfg_Map ring_monster_cfg_map;
	Level_Monster_Cfg_Map level_monster_cfg_map;
};

#endif /* RING_TASK_H_ */
