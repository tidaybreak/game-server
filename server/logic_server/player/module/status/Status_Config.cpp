/*
 * Status_Config.cpp
 *
 *  Created on: 2014年5月5日
 *      Author: Linqiyou
 */

#include "Status_Config.h"


void Btl_Buff_Effect::reset(void) {
	buff_effect_id			= 0;
	buff_effect_type		= 0;
	buff_hit				= 0;
	buff_delay.clear();
	buff_direct				= 0;
	beat_type_vec.clear();
	beat_gap_vec.clear();
	beat_type_set.clear();

	time_gap				= 0;
	action_gap				= 0;
	passive_gap				= 0;
	delay_time_gap			= 0;
	delay_action_gap		= 0;
	delay_passive_gap		= 0;

	buff_overlay			= false;
	buff_trigger_type		= 0;
	buff_trigger_skill 		= 0;
	buff_chance				= 0;
	trigger_buff.reset();
	val1					= 0;
	val2					= 0;
	val3					= 0;
	val4					= 0;
	val5					= 0;
}

void Btl_Buff_Cfg::reset(void) {
	buff_id				= 0;
	buff_charactor		= 0;
	overlay				= 0;
	is_dispel			= false;
	is_pure			= false;
	is_stoic			= false;
	buff_disappear_vec.clear();
	buff_disappear.clear();
	buff_spdisappear.clear();
	effect_map.clear();
	is_effect_un_overlay =false;
	is_fight_buff = false;
	buff_scene.clear();
	scene_visible.clear();
}
