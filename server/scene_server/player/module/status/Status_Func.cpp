/*
 * Status_Func.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: ti
 */

#include "Status_Func.h"
#include "Status_Struct.h"
#include "Config_Cache_Buff.h"
#include "fighter/Fighter_Struct.h"
#include "fighter/Fighter.h"
#include "Fighter_Detail.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Enum.h"

void handle_poison_trigger(Fighter & fighter, Status_Effect_Detail & info_, int skill_id) {
	if (fighter.dead_status()) {
		return ;
	}
	Attack_Param attack_param;
	attack_param.victim = fighter.role_id();
	attack_param.attacker = info_.caster_id;
	attack_param.attack_hurt = abs(info_.d_effect_value1 * info_.overlay);
//	attack_param.attack_hurt *= info_.overlay;
	attack_param.attack_result = Attack_Result::ATTACK_BUFF;
	attack_param.skill_id = skill_id;
//	fighter.actual_sub_blood_for_buff(attack_param, info_.status_id, info_.effect_id, 0, Status_Def::STATUS_EFFECT_TYPE_POISON);

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_ASYNC_STATUS);
	inner_buf.write_int64(fighter.role_id());
	attack_param.buff_serialize(inner_buf);
	inner_buf.write_int32(info_.status_id);
	inner_buf.write_int32(info_.effect_id);
	inner_buf.write_int32(Status_Def::STATUS_EFFECT_TYPE_POISON);
	inner_buf.finish_message();
	SCENE_MONITOR->push_async_buff(inner_buf);

}
void handle_poison_by_expression_trigger(Fighter & fighter, Status_Effect_Detail & info_, int skill_id) {
	if (fighter.dead_status()) {
		return ;
	}
	Attack_Param attack_param;
	attack_param.victim = fighter.role_id();
	attack_param.attacker = info_.caster_id;
	attack_param.attack_hurt = abs(fighter.handle_poition_buff(info_.i_effect_value1,
			info_.d_effect_value5, info_.d_effect_value6, info_.d_effect_value8, info_.d_effect_value7, info_.d_effect_value9));
	attack_param.attack_hurt *= info_.overlay;
	attack_param.attack_result = Attack_Result::ATTACK_BUFF;
	attack_param.skill_id = skill_id;
//	fighter.actual_sub_blood_for_buff(attack_param, info_.status_id, info_.effect_id, 0, Status_Def::STATUS_EFFECT_TYPE_POISON);
	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_ASYNC_STATUS);
	inner_buf.write_int64(fighter.role_id());
	attack_param.buff_serialize(inner_buf);
	inner_buf.write_int32(info_.status_id);
	inner_buf.write_int32(info_.effect_id);
	inner_buf.write_int32(Status_Def::STATUS_EFFECT_TYPE_POISON);
	inner_buf.finish_message();
	SCENE_MONITOR->push_async_buff(inner_buf);
}
//void handle_attr_trigger(Fighter & fighter, Status & status, Status_Info & info) {
//	status.add_status_prop(info.prop_fix_);
//	info.total_prop_fix.cover += info.prop_fix_.cover;
//	info.total_prop_fix.fixed += info.prop_fix_.fixed;
//	info.total_prop_fix.percent += info.prop_fix_.percent;
//}
//
//void handle_moral_trigger(Fighter & fighter, Status_Info & info) {
//	if (Property_Type::PT_MORALE_CURRENT != info.prop_fix_.type) return;
//	Prop_Setter_Vec prop_vec;
//	Prop_Setter prop;
//	prop.prop_type = info.prop_fix_.type;
//	prop.basic = abs(info.prop_fix_.fixed);
//	prop.operate = (info.prop_fix_.fixed > 0) ? O_ADD : O_SUB;
//	prop.addi_type = Addition_Type::AT_BASIC;
//	prop_vec.push_back(prop);
//
//	MSG_81000060 msg;
//	msg.role_id = fighter.role_id();
//	fighter.fight_modify_fight_prop(prop_vec, msg.change_info);
//
//	Block_Buffer buf;
//	MAKE_MSG_BUF(msg, buf);
//	fighter.call_broadcast_around(buf);
//}
