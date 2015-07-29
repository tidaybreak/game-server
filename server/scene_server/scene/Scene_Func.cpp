/*
 * Scene_Func.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: "ti"
 */

#include "Scene_Func.h"
#include "Move_Scene.h"
#include "Config_Cache_Scene.h"

Scene_Type get_scene_type(uint32_t scene_id) {
	const Scene_Config *scene = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if (! scene) {
		MSG_USER_TRACE("scene:%d", scene_id);
		return Scene_Type_Null;
	}

	return scene->type;
}

// http://yiminghe.iteye.com/blog/568666
// 三点共线判断
bool is_in_same_line(const Coord &coordA, const Coord &coordB, const Coord &coordC) {
	 if ((coordA.x - coordC.x) * (coordB.y - coordC.y) ==
		(coordB.x - coordC.x) * (coordA.y - coordC.y)) {
		 return true;
	 } else {
		 return false;
	 }
}

void calc_direction_by_a_b(const Coord &pointB, Coord &pointA_fix, double &angle, int toward) {
	if (pointA_fix == pointB) { // 两点重合情况 需要根据人物朝向修正pointA
		switch (toward) {
		case Move_Scene::toward_up:
		case Move_Scene::toward_right_up:
		case Move_Scene::toward_right:
		case Move_Scene::toward_right_down:
		case Move_Scene::toward_down:
		case Move_Scene::toward_left_down:
		case Move_Scene::toward_left:
		case Move_Scene::toward_left_up:
			pointA_fix.x -= toward_direction[toward][0];
			pointA_fix.y -= toward_direction[toward][1];
			break;
		default:
			pointA_fix.y++;
			break;
		}
	}

	double length_AB = hypot(get_pixel_width(pointA_fix.x - pointB.x),
		get_pixel_height(pointA_fix.y - pointB.y));

	double dy_AB = abs(get_pixel_height(pointA_fix.y - pointB.y));
	angle = asin(dy_AB / length_AB);
}
