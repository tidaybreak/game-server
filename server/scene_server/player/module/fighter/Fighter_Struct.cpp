/*
 * Fighter_Struct.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *
 */

#include "Fighter_Struct.h"

void Attack_Param::reset(void) {
	skill_id = 0;
	attacker = 0;
	victim = 0;
	attack_hurt = 0.0;
	attack_result = 0;
	skill_effect = 0;
	pc_source = Prop_Change_Source::SOURCE_DEFAULT;
}

void Attack_Param::buff_serialize(Block_Buffer &buf) const {
	buf.write_int32(skill_id);
	buf.write_int64(attacker);
	buf.write_int64(victim);
	buf.write_double(attack_hurt);
	buf.write_int32(attack_result);
}

void Attack_Param::buff_deserialize(Block_Buffer &buf) {
	buf.read_int32(skill_id);
	buf.read_int64(attacker);
	buf.read_int64(victim);
	buf.read_double(attack_hurt);
	buf.read_int32(attack_result);
}

void Target_Select_Info::reset(void) {
	select_type = 0;
	target_relation = 0;
	target_type.clear();
	prior_line = 0;
	prior_row = 0;
	target_amount = 0;
}

void Skill_Effect_Val_Inner::reset(void) {
	val1 = 0;
	val2 = 0;
	val3 = false;
	val4 = 0.0;
	val5 = 0.0;
	val6 = 0.0;
	val7 = 0.0;
}

void Skill_Effect_Val::reset(void) {
	id 		= 0;
	data1.clear();
	data2 	= 0.0;
	int_vec.clear();
	inner_vec.clear();
}

void Skill_Effect::reset(void) {
	effect_id = 0;
	effect_type = 0;
	effectgroup_id = 0;
	effect_series_id = 0;
	continue_time = Time_Value::zero;
	target_sellect_info.reset();
	effect_rate = 0;
	morale_damage.reset();
	hp_cure.reset();
	effect_correct.reset();

	val1.reset();
	call_monster.reset();
	transfer_call_monster_vec.clear();
}

void Be_Skill_Attr::reset(void) {
	id = 0;
	fix = 0;
	percnet = 0;
}

void Be_Skill_Effect::reset(void) {
	be_skill_effect_id = 0;
	be_fix_type = 0;
	add_moral_map.clear();
	effect_vec.clear();
	attr_vec.clear();
	open_skills.clear();
	be_fix_steps.clear();
	val1.reset();
	fix_buff_time.clear();
	fix_skill_time.clear();
	fix_morale_cost_map.clear();
	trigger_type = 0;
	trigger_cd_time = 0;
	max_trigger_times = 0;
	trigger_threshold_val = 0;
	trigger_skill.clear();
}

void Attr_Correct::reset(void) {
	type			= 0;;
	fixed_value 	= 0;
	percent_value	= 0;
	mask_value 		= 0;
}

void Skill_Effect_Correct::reset(void) {
	effect_id = 0;
	attr_crr_vec.clear();
}

void Skill_Step_Fix::reset(void) {
	value = 0.0;
	id = 0;
	val1 = 0;
	val2 = 0.0;
	val3 = 0.0;
}

void Fix_Call_Monster::reset(void) {
	type = 0;
	value = 0.0;
	cfg_percent = 0.0;
}

void Call_Monster::reset(void) {
	monster_type_id = 0;
	idx = 0;
	fix_monster_vec.clear();
}

void Transfer_Call_Monster::reset(void) {
	monster_type_id = 0;
	idx = 0;
	fix_blood_percent = 1.0;
}

void Skill_Cd::reset(void) {
	type 		= 0;
	millisecond = 0;
}

void Skill_Cost::reset(void) {
	type 	= 0;
	coster  = Cost_Target::COST_SELF;
	value 	= 0;
}

void Skill_Detail::reset(void) {
	skill_id 					= 0;
	skill_level 				= 0;
	skill_act 					= 0;
	yl_cost 					= 0;
	goods_cost 				= 0;
	goods_cost_num 	= 0;
	lv_lim 						= 0;
	intimacy_lv				= 0;
//	mutex_cond			= 0;
	wake_lv 					= 0;
	force_factor 			= 0.0;
	hurt_time				= Time_Value::zero;
	sing_time 				= Time_Value::zero;
	pre_skill.clear();
	mutex_cond_vec.clear();
	fight_cost_vec.clear();
	step_effect.clear();
	pre_cd_vec.clear();
	skill_cd_vec.clear();
	be_skill_effect_map.clear();
}

void Skill_Config::reset(void) {
	skill_id 				= 0;
	skill_career 			= 0;
	skill_series 			= 0;
	skill_type 				= 0;
	skill_affiliation 		= 0;
	skill_attribute 		= 0;
	skill_conflict 			= 0;
	target_conflict			= 1;
	ignore_enemy_conflict 	= 0;
	skill_priority 			= 0;
}

void Skill_Talent::reset(void) {
	talent_id				= 0;
	career					= 0;
	lv_cond					= 0;
	goods_id				= 0;
	goods_cost				= 0;
	task_cond				= 0;
}

Skill_AI_Config::Skill_AI_Config(void){
	reset();
}

void Skill_AI_Config::reset(void){
	id = 0;
	skill_id = 0;
	priority_add = 0;
	strike_vec.clear();
}

void Dynamic_Be_Skill::reset(void) {
	id = 0;
	lv = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Phy_Power_Cost_Config::Phy_Power_Cost_Config(void){
	reset();
}

void Phy_Power_Cost_Config::reset(void){
	cost = 0;
	number = 0;
}

Phy_Power_Lv_Config::Phy_Power_Lv_Config(void){
	reset();
}

void Phy_Power_Lv_Config::reset(void){
	level = 0;
	number = 0;
}

void Be_Skill_Effect_Used::reset(void) {
	effect_id = 0;
	used_times = 0;
	last_used_time = Time_Value::zero;
}
