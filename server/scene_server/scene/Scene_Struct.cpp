/*
 * Scene_Struct.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *
 */

#include "Scene_Struct.h"

Path_Node::Path_Node(void) {
	g_val = h_val = 0;
	valid_node = false;
}

Path_Node::Path_Node(int g, int h, Coord cur, Coord prev) {
	g_val = g;
	h_val = h;
	cur_coord = cur;
	prev_coord = prev;
	valid_node = true;
}

void Path_Node::set(int g, int h, Coord cur, Coord prev) {
	g_val = g;
	h_val = h;
	cur_coord = cur;
	prev_coord = prev;
	valid_node = true;
}

Battle_Fighter::Battle_Fighter(void) { reset(); };

void Battle_Fighter::reset(void) {
	p_id = 0;
	mover = 0;
}

void Map_Head::reset(void) {
	cell_width = 0;		/// 格子长宽
	cell_height = 0;
	x_len = 0;			/// 格子数
	y_len = 0;
	map_width = 0;		/// 地图长宽(像素)
	map_height = 0;
	i_point_y = 0;
}

void Scene_Map_Data::reset(void) {
	id = 0;
	head.reset();
	content.clear();
}

Safe_Data::Safe_Data(void) {
	mover_set.clear();
	range_type = 0;
	distance = 0;
}


void Scene_Config::Coord_Value::reset(void) {
	scene_id = 0;
	value = 0;
	coord.reset();
	coord_vec.clear();
}

void Scene_Config::Transmit_Coord::reset(void) {
	src_scene_id = 0;
	coord.reset();
}

void Scene_Config::reset(void) {
	scene_id = 0;
	order_total = 0;
	scene_block_x = 25;
	scene_block_y = 20;
	inter_order = -1;
	role_limit = 0;
	type = Scene_Type_Null;
	manager = 0;
	scene_name = "";
	move_value.clear();
	transmit_point.clear();
	transmit_coord.clear();
	revive_point.clear();
	des_scene_limit.clear();
	scene_keep_time = Time_Value::zero;
	login_create = false;
	timeLimit = 0;
	relive_protect_time = 0;
	relive_per = 0.0;
	map_id = 0;
	pet_back = 0;
	mount_back = 0;
	peace = 0;
	restore_status = 0.0;
	fly_sky = 0;
	enter_level = 0;
	relive_drug_type = 0;
	trace_scene_level = 0;
	exp_add_gap = Time_Value::zero;
	exp_add_map.clear();
	same_scene_transfer_inner = 0;
	same_scene_forbid_cloud = 0;
	hero_disable = false;
	dungeon.reset();
}

bool Scene_Config::is_elite_scene(void) const {
	if(type == Elite_Scene_Type){
		return true;
	}
	return false;
}
