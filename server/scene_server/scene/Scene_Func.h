/*
 * Scene_Func.h
 *
 *  Created on: Oct 30, 2012
 *      Author: "ti"
 */

#ifndef SCENE_FUNC_H_
#define SCENE_FUNC_H_

#include "math.h"
#include "Base_Struct.h"
#include "Scene_Def.h"

const int SHIFT_MULTIPLE = 10;
const int X_TO_PIXEL = 32;
const int HALF_X_TO_PIXEL = X_TO_PIXEL / 2;
const int X_SHIFT = X_TO_PIXEL / SHIFT_MULTIPLE / 2;
const int Y_TO_PIXEL = 32;
const int Y_SHIFT = Y_TO_PIXEL / SHIFT_MULTIPLE / 2;
const int HALF_Y_TO_PIXEL = Y_TO_PIXEL / 2;
const int X_DIV_Y = (X_TO_PIXEL / Y_TO_PIXEL);
const double PI = 3.1415926535898;

Scene_Type get_scene_type(uint32_t scene_id);

// X坐标转换像素
inline int get_pixel_width(int width) {
	return width * X_TO_PIXEL;
}

// Y坐标转换像素
inline int get_pixel_height(int height) {
	return height * Y_TO_PIXEL;
}

inline int get_x_by_pixel(int pixel_width) {
	if (pixel_width == 0) {
		return 0;
	} else {
		return (pixel_width - 1) / X_TO_PIXEL;
	}
}

inline int get_y_by_pixel(int pixel_height) {
	if (pixel_height == 0) {
		return 0;
	} else {
		return (pixel_height - 1) / Y_TO_PIXEL;
	}
}

// 防止越界
inline int get_positive_or_zero(int input) {
	if (input < 0) {
		return 0;
	} else {
		return input;
	}
}


inline int calculate_grid_distance(const Coord &a_coord, const Coord &b_coord) {
	int dx = std::abs(a_coord.x - b_coord.x);
	int dy = std::abs(a_coord.y - b_coord.y);

	return hypot(dx, dy);
}

inline int calculate_pixels_distance(const Coord &a_coord, const Coord &b_coord) {
	int dx = get_pixel_width(a_coord.x - b_coord.x);
	int dy = get_pixel_height(a_coord.y - b_coord.y);
	return hypot(dx, dy);
//	int dx = (a_coord.x - b_coord.x) << X_SHIFT;
//	int dy = (a_coord.y - b_coord.y) << Y_SHIFT;
//
//	return hypot(dx, dy) * SHIFT_MULTIPLE;
}

inline int calculate_pixels_distance(const int x1, const int y1, const int x2, const int y2) {
	return hypot((x1 - x2) * X_TO_PIXEL, (y1 - y2) * Y_TO_PIXEL);
}

inline int calculate_pixels_distance_from_pixels(const int x1, const int y1, const int x2, const int y2) {
	return hypot((x1 - x2) , (y1 - y2) );
}

inline int double_to_int(double value) {
	return ((value > 0) ? static_cast<int>(value + 0.5) : static_cast<int>(value - 0.5));
}

// 不会大于180度 目前的扇形都是小于180度
inline double get_degree(double radian) {
	double ret = radian / PI * 180;
	if (ret > 180) {
		ret -= 180;
	}
	return ret;
}

// 请把初中数学还给我 T_T
// 点1：圆心，点2：根据玩家朝向，技能长度计算出的最远中间释放点，点3：判断(攻击目标)点
inline bool is_in_sector(const Coord &origin, const Coord &dest, const Coord &point, int degree, bool check_around) {
	if (origin == point) {
		return true;
	}

	double dx1, dx2, dy1, dy2;
	double radian;

	dx1 = get_pixel_width(dest.x - origin.x);
	dy1 = get_pixel_height(dest.y - origin.y);
	dx2 = get_pixel_width(point.x - origin.x);
	dy2 = get_pixel_height(point.y - origin.y);

	double radius = hypot(dx1, dy1);
	double line = hypot(dx2, dy2);
	if(check_around){// 玩家周围的也计算进去
		if (line < 100) {
			return true;
		}
	}


	// 大于半径，不在扇形范围内
	if (line > radius) {
		return false;
	}

	double c = radius * line;
	if (c == 0) {
		return false;
	}

	radian = acos((dx1 * dx2 + dy1 * dy2) / c);
	// 到中心点的夹角应小于技能指定夹角的二分之一
	if (get_degree(radian) <= (degree / 2)) {
		return true;
	}

	return false;
}

// 椭圆 focus_length:焦距/2   range:2a
inline bool is_in_oval(const Coord &coord1, const Coord &coord2, int focus_length, int range) {
	if (focus_length <= 0) return false;

	int d1 = hypot((coord1.x - coord2.x) * X_TO_PIXEL - focus_length, (coord1.y  - coord2.y) * Y_TO_PIXEL);
	int d2 = hypot((coord1.x - coord2.x) * X_TO_PIXEL + focus_length, (coord1.y  - coord2.y) * Y_TO_PIXEL);

	if(range >= d1 + d2)
		return true;

	return false;
}

inline bool is_in_range(const Coord &coord1, const Coord &coord2, int pixel) {
	if (pixel <= 0) return false;

	if (pixel >= calculate_pixels_distance(coord1, coord2)) {
		return true;
	} else {
		return false;
	}
}

bool is_in_same_line(const Coord &coordA, const Coord &coordB, const Coord &coordC);

// 根据点A,B，计算出方向(角度)
void calc_direction_by_a_b(const Coord &pointB, Coord &pointA_fix, double &angle, int toward);


#endif /* SCENE_FUNC_H_ */
