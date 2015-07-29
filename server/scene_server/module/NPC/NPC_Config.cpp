/*
 * NPC_Config.cpp
 *
 *  Created on: 2014年4月1日
 *      Author: root
 */

#include "NPC_Config.h"

Drop_Box_Config::Drop_Box_Config(void){
	reset();
}

void Drop_Box_Config::reset(void){
	rate_type = 0;
	item_id_vec1.clear();
	item_id_vec2.clear();
	item_id_vec3.clear();
	item_rate_vec.clear();
	item_amount_vec.clear();
	item_bind_vec.clear();
}

const std::vector<int>& Drop_Box_Config::item_id(int8_t career/* = 0*/) const{
	if(career == 1){
		return item_id_vec1;
	}else if(career == 2){
		return item_id_vec2;
	}else if(career == 3){
		return item_id_vec3;
	}
	return item_id_vec1;
}
////////////////////////////////////////////////////////////////////////////////////

Drop_Config::Drop_Config(void){
	reset();
}

void Drop_Config::reset(void){
	id = 0;
	name = "";
	jid = 0;
	rate_type = 0;
	box_vec.clear();
	box_rate_vec.clear();
	cfg_max_gt_times = -1;
}

///////////////////////////////////////////////////////////////////

Material_Point_Config::Material_Point_Config(void){
	reset();
}

void Material_Point_Config::reset(void){
	scene_key = 0;
	id = 0;
	material_id = 0;
	type = 0;
	gather_time = 0;
	gather_times = 0;
	refresh_time = 0;
	refresh_times = 0;
	coord.reset();
	rate = 0;
	drops_vec.clear();
	task_vec.clear();
	event_vec.clear();
}

////////////////////////////////////////////////////////////////////////

Material_Section_Config::Material_Section_Config(void){
	reset();
}

void Material_Section_Config::reset(void){
	material_point_map.clear();
}

/////////////////////////////////////////////////////////////////////

Machine_Point_Config::Machine_Point_Config(void){
	reset();
}

void Machine_Point_Config::reset(void){
	scene_key = 0;
	id = 0;
	machine_id = 0;
	type = 0;
	view_range = 0;
	trigger_times = 0;
	trigger_cd = 0;
	gather_time = 0;
	refresh_time = 0;
	refresh_times = 0;
	start_plot = "";
	end_plot = "";
	coord.reset();
	event_vec.clear();
	acc_id = 0;
	acc_coord.reset();
}

////////////////////////////////////////////////////////////////////

Machine_Section_Config::Machine_Section_Config(void){
	reset();
}

void Machine_Section_Config::reset(void){
	point_map.clear();
}
///////////////////////////////////////////////////////////////////

Machine_Event_Config::Machine_Event_Config(void){
	reset();
}

void Machine_Event_Config::reset(void){
	id = 0;
	type = 0;
	val_1 = 0;
	val_2 = 0;
	val_3 = 0;
	val_4 = 0;
	val_5 = 0;
	words = "";
	coord_1.reset();
	coord_2.reset();
}

//////////////////////////////////////////////////////////////////

Monster_AI_Config::Monster_AI_Config(void){
	reset();
}

void Monster_AI_Config::reset(void){
	id = 0;
	pri = 0;
	times = 0;
	cd = 0;
	rate = 0;
	stop_sing = 0;
	c_type = 0;
	c_val1 = 0;
	c_val2 = 0;
	c_val3 = 0;
	a_type = 0;
	a_val1 = 0;
	a_val2 = 0;
	a_val3 = 0;
	battle_over = 0;
	foreshow = 0;

	skill_vec.clear();
	skill_rate_vec.clear();
	buf_argv.reset();
}

////////////////////////////////////////////////////////////////////

Monster_Point_Config::Monster_Point_Config(void){
	reset();
}

void Monster_Point_Config::reset(void){
	id = 0;
	scene_key = 0;
	point_type = 0;
	state_type = 0;
	mult_type = 0;
	type = 0;
	refresh_times = 0;
	refresh_time = 0;
	refresh_con = 0;
	con_time = 0;
	after_time = 0;
	after_point = 0;
	birth_coord.reset();
	view_range = 0;
	move_range = 0;
	chase_range = 0;
	speed[0] = 150;
	speed[1] = 150;
	speed[2] = 150;
	npc_vec.clear();
	add_coord_vec.clear();
	start_plot = "";
	start_plot_time = 0;
	end_plot = "";
	end_plot_time = 0;
	talk_vec.clear();
	chat_vec.clear();
	group_vec.clear();
	walk_path.clear();
	ia  = 0;
	tasks_vec.clear();
	events_vec.clear();
	rate = 0;
	gather_time = 0;
	gt_cd = 0;
	gt_times = 0;
	acc_id = 0;
	acc_coord.reset();

	drop_type = 0;
	drop_mult_mode = 0;
	drop_mult_coe = 0;
	drop_lv_limit = 0;
	drop_lv_coe = 0;
	drops.clear();
	drops_coord.clear();

	fightMap_key = 0;
	monster_chance = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

Monster_Group_Config::Monster_Group_Config(void){
	reset();
}

void Monster_Group_Config::reset(void){
	scene_key = 0;
	id = 0;
	monster.clear();
	wave.clear();
	pos.clear();
	chest_id.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////

Monster_Section_Config::Monster_Section_Config(void){
	reset();
}
void Monster_Section_Config::reset(void) {
	point_map.clear();
	group_map.clear();
	path_guide_vec.clear();
}
