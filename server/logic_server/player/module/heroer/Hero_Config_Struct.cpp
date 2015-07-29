/*
 * Hero_Config_Struct.cpp
 *
 *  Created on: Apr 12, 2014
 *      Author: chenyulin
 */

#include "Hero_Config_Struct.h"

void Hero_Config::reset(void) {
	hero_id = 0;
	hero_name = "";
	hero_career = 0;
	hero_init_lvl = 0;
	hero_initial_awake = 0;
	hero_quality = 0;
	hero_sm_type = 0;
	hero_open_lvl = 0;
	vip_recruit_diamond = 0;
	recruit_cost_soul = 0;
	recruit_exploit_lvl = 0;
	max_awake_lvl = 0;
	factor_lvl_up_soul = 0;
	artifact_ids.clear();

	skill_id_vec.clear();
	recruit_tasks.clear();
	recruit_cost_items.clear();

	awake_skill_id_vec.clear();
	awake_tasks_map.clear();
	awake_lvl_cost_items.clear();
	hero_lvl_awake_limit.clear();
	awake_lvl_cfg_map.clear();

	hero_prop_correct.clear();
	hero_prop_conver.clear();

	assist_skill_id_vec.clear();
	hero_intimacy.reset();
}

void Hero_Lvl_Misc_Config::reset(void) {
	hero_lvl = 0;
	lvl_up_cost_souls = 0;
}
