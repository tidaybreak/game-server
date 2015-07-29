/*
 * NPC_Def.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef NPC_DEF_H_
#define NPC_DEF_H_

#include "math.h"

#define MAX_TEAMMATE_NUM (5)
#define TEAMMATE_MIN_DISTANCE (96)

#define MAX_TEAM_POS (5)


const int X_TIME = 200 * 233359;
const int Y_TIME = 200 * 132952;
const int X_Y_TIME = 200 * 265165;

const int TEAM_POS_X[MAX_TEAMMATE_NUM+2][MAX_TEAM_POS+2]  = {
		{0},
		{0},
		{0,2},
		{0,1,2},
		{0,1,2,3},
		{0, 1, 2, 3, 4},
		{0,0,0,1,1,2}
};
const int TEAM_POS_Y[MAX_TEAMMATE_NUM+2][MAX_TEAM_POS+2]  = {
		{0},
		{0},
		{0,0},
		{0,-2,0},
		{0,-2,-1,0},
		{0,-2, 1,-1, 1},
		{0,-1,-2,0,-1,0}
};
const static double interval = 30; // 每帧移动系数
const static double grid_width = 32.0, grid_height = 32.0; // 地图格子宽高
//const static double grid_width = 20.0, grid_height = 20.0; // 地图格子宽高
const static double grid_s = sqrt(grid_width * grid_width + grid_height * grid_height);

#endif /* NPC_DEF_H_ */
