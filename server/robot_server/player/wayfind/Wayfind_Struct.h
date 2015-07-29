/*
 * Wayfind_Struct.h
 *
 *  Created on: 2014-06-07
 *      Author: xiaoliang
 */

#ifndef WAYFIND_STRUCT_H_
#define WAYFIND_STRUCT_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"
#include "Move_Scene.h"

struct Wayfind_Info {
	Wayfind_Info(void);
	virtual ~Wayfind_Info();
	void reset(void);
	Coord det_coord;  //目标位置信息
	int32_t det_scene_id; //目标场景
	Move_Scene *scene;
	//////////////////////////////
	Location cur_location;  //当前位置信息

	Coord_Vec path;

	Time_Value last_move_time;              //上次移动时间
	Time_Value move_interval_time;
	bool first_move;
};

#endif /* WAYFIND_STRUCT_H */
