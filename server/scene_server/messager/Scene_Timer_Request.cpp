/*
 * Scene_Timer_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Scene_Timer_Request.h"
#include "Scene_Monitor.h"
#include "Config_Cache.h"
#include "Pool_Manager.h"
#include "Scene_Player.h"
#include "Utility_Func.h"
#include "NPC/NPC_Manager.h"
#include "Global_Scene_Manager.h"


Scene_Timer_Request::Scene_Timer_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = SCENE_MONITOR;
}

Scene_Timer_Request::~Scene_Timer_Request() {
	// TODO Auto-generated destructor stub
}

void Scene_Timer_Request::scene_tick(Time_Value &now) {
	scene_tick_100ms(now);
	scene_tick_300ms(now);
	scene_tick_500ms(now);
	scene_tick_800ms(now);
	scene_tick_1000ms(now);
	scene_tick_50ms(now);
}

void Scene_Timer_Request::scene_tick_50ms(Time_Value &now) {
	//GLOBAL_SCENE_MANAGER->tick();

}

void Scene_Timer_Request::scene_tick_100ms(Time_Value &now) {
	if (now > tick_100ms_) {
		NPC_MANAGER->tick();
		GLOBAL_SCENE_MANAGER->tick();
		tick_100ms_ = now + Time_Value(0, 100000);
	}
}

void Scene_Timer_Request::scene_tick_300ms(Time_Value &now) {
	if (now > tick_300ms_) {
		monitor()->process_time_up();

		tick_300ms_ = now + Time_Value(0, 300000);
	}
}

void Scene_Timer_Request::scene_tick_500ms(Time_Value &now) {
	if (now > tick_500ms_) {

		tick_500ms_ = now + Time_Value(0, 500000);
	}
}

void Scene_Timer_Request::scene_tick_800ms(Time_Value &now) {
	if (now > tick_800ms_) {


		tick_800ms_ = now + Time_Value(0, 800000);
	}
}

void Scene_Timer_Request::scene_tick_1000ms(Time_Value &now) {
	if (now > tick_1000ms_) {
		GLOBAL_SCENE_MANAGER->module_tick(now);

		tick_1000ms_ = now + Time_Value(1, 0);
	}
}
