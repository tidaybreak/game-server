/*
 * Scene_Player_Struct.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *
 */

#include "Scene_Player_Detail.h"
#include "Server_Struct.h"

////////////////////////////////////////////////////////////////////

Scene_Player_Detail::Scene_Player_Detail(void) {
	reset();
}

void Scene_Player_Detail::reset(void) {
	role_id = 0;
	status = 0;

	base_detail.reset();
	fighter_detail.reset();
	mover_detail.reset();
}
