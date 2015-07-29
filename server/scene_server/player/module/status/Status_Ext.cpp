/*
 * Status_Ext.cpp
 *
 *  Created on: 2014年9月18日
 *      Author: Linqiyou
 */

#include "Config_Cache_Buff.h"
#include "Err_Code.h"
#include "Game_Typedef.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Pool_Manager.h"
#include "Status_Func.h"
#include "Status_Struct.h"
#include "Status.h"
#include "Fighter_Detail.h"
#include "fighter/Fighter.h"
#include "fighter/Fighter_Struct.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Battle_Scene.h"
#include "Scene_Public.h"
#include "NPC/hero/Hero.h"
#include "Utility_Func.h"
#include "Config_Cache_Scene.h"
#include "Move_Scene.h"
#include "Base_Detail.h"
#include "NPC/NPC_Manager.h"

int Status::add_status_prop(Status_Effect_Detail & fix, int skill_id) {
	if (fix.i_effect_value1 < PT_FIGHT_PROP_START || fix.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;
	const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(fix.status_id);
	if (!bbc) {
		return -1;
	}
	bool is_fight_buff = bbc->is_fight_buff;
	if (fix.status_id == CONFIG_CACHE_BUFF->special_level_buff().status_id) {
		is_fight_buff = true;
	}

	Prop_Setter_Vec prop_vec;
	Prop_Setter prop;
	prop.prop_type = (Property_Type)fix.i_effect_value1;
	if (fix.i_effect_value1 == Property_Type::PT_MORALE_CURRENT) {
		prop.fixed = 0.0;
		prop.percent = 0.0;
		prop.basic = fix.d_effect_value1 * fix.overlay;
		prop.addi_type = Addition_Type::AT_BASIC;
		if (fighter()
				&& fighter()->mover_detail_ptr()
				&& fighter()->mover_detail().is_hero()){
			Fighter *fighter = this->fighter()->find_fighter_in_battle_scene(this->hero_self()->master_role_id());
			if (NULL != fighter) {
				prop_vec.push_back(prop);

				MSG_81000061 msg;
				msg.role_id = fighter->role_id();
				msg.attack_id = fix.caster_id;
				msg.status_id = fix.status_id;
				msg.effect_id = fix.effect_id;
				msg.skill_id = skill_id;
				//int ret = fighter()->fight_modify_fight_prop(prop_vec, msg.change_info);
//				int ret = player->fight_modify_props(prop_vec, msg.change_info);
//
//				Block_Buffer buf;
//				MAKE_MSG_BUF(msg, buf);
//				if (player->battle_scene()) {
//					player->battle_scene()->broadcast_around(buf);
//				}
//				if (0 != ret) {
//					player->send_error_to_client(ret, 0);
//				}

				fighter->fight_modify_props_by_status(prop_vec, msg, is_fight_buff, 2);
			}
			return 0;
		}
	} else if (fix.i_effect_value1 == Property_Type::PT_SPEED) {
		prop.fixed = fix.d_effect_value1 * fix.overlay;
		prop.percent = 0.0;
		prop.basic = 0.0;
		prop.addi_type = Addition_Type::AT_STATUS;
	} else if (fix.i_effect_value1 == PT_BLOOD_CURRENT) {
		prop.fixed = fix.d_effect_value1 * fix.overlay;
		prop.percent = fix.d_effect_value2 * fix.overlay;
		prop.addi_type = Addition_Type::AT_STATUS;
	} else {
		prop.fixed = fix.d_effect_value1 * fix.overlay;
		prop.percent = fix.d_effect_value2 * fix.overlay;
		prop.addi_type = Addition_Type::AT_STATUS;
	}
	prop_vec.push_back(prop);

	MSG_81000061 msg;
	msg.role_id = role_id();
	msg.attack_id = fix.caster_id;
	msg.status_id = fix.status_id;
	msg.effect_id = fix.effect_id;
	msg.skill_id = skill_id;
	//int ret = fighter()->fight_modify_fight_prop(prop_vec, msg.change_info);
	int ret = 0;
	if (fighter()) {
		ret = fighter()->fight_modify_props_by_status(prop_vec, msg, is_fight_buff, 6);
	}

	return ret;
}

int Status::add_status_prop_by_per(Status_Effect_Detail & effect, int skill_id) {
	if (!fighter()) {
		return -1;
	}
	if (effect.i_effect_value1 != PT_BLOOD_CURRENT)
		return 0;
	const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(effect.status_id);
	if (!bbc) {
		return -1;
	}

	Prop_Setter_Vec prop_vec;
	Prop_Setter prop;
	prop.prop_type = (Property_Type)effect.i_effect_value1;
	if (effect.d_effect_value1 * effect.overlay != 0) {

//		double cure_value = 0.0;
		double cure_rate_caster = 0.0;
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(effect.caster_id);
		Monster *monster = NPC_MANAGER->find_monster(effect.caster_id);
		Hero* hero = NPC_MANAGER->find_hero(effect.caster_id);
		if (0 != player) {
//			cure_value = player->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1, Prop_Value::ELEM_FIGHT);
//			cure_value *= effect.d_effect_value2;//val5
//			cure_value += effect.d_effect_value4;//val7
			cure_rate_caster = player->fighter_detail().fetch_fight_property(PT_CURE_RATE);
//			effect.last_cure_value = cure_value;
			effect.last_cure_rate = cure_rate_caster;
		} else if (monster) {
//			cure_value = monster->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1, Prop_Value::ELEM_FIGHT);
//			cure_value *= effect.d_effect_value2;//val5
//			cure_value += effect.d_effect_value4;//val7
			cure_rate_caster = monster->fighter_detail().fetch_fight_property(PT_CURE_RATE);
//			effect.last_cure_value = cure_value;
			effect.last_cure_rate = cure_rate_caster;
		} else if (hero) {
//			cure_value = hero->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1, Prop_Value::ELEM_FIGHT);
//			cure_value *= effect.d_effect_value2;//val5
//			cure_value += effect.d_effect_value4;//val7
			cure_rate_caster = hero->fighter_detail().fetch_fight_property(PT_CURE_RATE);
//			effect.last_cure_value = cure_value;
			effect.last_cure_rate = cure_rate_caster;
		} else {
//			cure_value = effect.last_cure_value;
			cure_rate_caster = effect.last_cure_rate;
		}
		double cure_rate_self = fighter()->fighter_detail().fetch_fight_property(PT_BE_CURE_RATE);

//		double cur = player_self()->blood_current();
		double mx = fighter()->blood_max();
		prop.basic = effect.d_effect_value1 * effect.overlay * mx * (1 + cure_rate_caster + cure_rate_self);
//		prop.percent = fix.d_effect_value2 * fix.overlay;
		prop.addi_type = Addition_Type::AT_BASIC;
		prop_vec.push_back(prop);
	} else {
		return 0;
	}

	MSG_81000061 msg;
	msg.result = 5;
	msg.role_id = role_id();
	msg.attack_id = effect.caster_id;
	msg.status_id = effect.status_id;
	msg.effect_id = effect.effect_id;
	msg.skill_id = skill_id;


	return fighter()->fight_modify_props_by_status(prop_vec, msg, bbc->is_fight_buff, 1);
}

int Status::sub_status_prop(Status_Effect_Detail& fix) {
	if (fix.i_effect_value1 < PT_FIGHT_PROP_START || fix.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;

	if (fix.i_effect_value1 == Property_Type::PT_MORALE_CURRENT||
			fix.i_effect_value1 == Property_Type::PT_BLOOD_POOL_CURRENT)
		return 0;
	const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(fix.status_id);
	if (!bbc) {
		return -1;
	}

	int effect_times = fix.effect_times;
//	if (0 == effect_times) {
//		effect_times = 1;
//	}
//	effect_times *=  fix.overlay;

	fix.effect_times = 0;

	Prop_Setter_Vec prop_vec;
	Prop_Setter prop;
	prop.prop_type = (Property_Type)fix.i_effect_value1;
	prop.fixed = -fix.d_effect_value1 * effect_times;
	prop.percent = -fix.d_effect_value2 * effect_times;
	//prop.operate = Operate_Type::O_SUB;
	prop.addi_type = Addition_Type::AT_STATUS;
	prop_vec.push_back(prop);

	MSG_81000061 msg;
	msg.role_id = role_id();
	msg.attack_id = fix.caster_id;
	msg.status_id = fix.status_id;
	msg.effect_id = fix.effect_id;
	//int ret = fighter()->fight_modify_fight_prop(prop_vec, msg.change_info);
//	int ret = fighter()->fight_modify_props(prop_vec, msg.change_info);
//
//	Block_Buffer buf;
//	MAKE_MSG_BUF(msg, buf);
//	call_broadcast_around(buf);
//	THIS_SEND_TO_CLIENT(msg);
//
//	return ret;
	bool is_fight_buff = bbc->is_fight_buff;
	if (fix.status_id == CONFIG_CACHE_BUFF->special_level_buff().status_id) {
		is_fight_buff = true;
	}
	return fighter()->fight_modify_props_by_status(prop_vec, msg, is_fight_buff, 5);
}

int Status::cover_status_prop(Status_Effect_Detail & fix, Status_Effect_Detail & fix_from) {
	if (fix.i_effect_value1 < PT_FIGHT_PROP_START || fix.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;
	const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(fix.status_id);
	if (!bbc) {
		return -1;
	}

	Prop_Setter_Vec prop_vec;
	Prop_Setter prop;
	prop.prop_type = (Property_Type)fix.i_effect_value1;
	if (fix.i_effect_value1 == Property_Type::PT_MORALE_CURRENT) {
		prop.fixed = 0.0;
		prop.percent = 0.0;
		prop.basic = fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay;
		//prop.operate = (fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay > 0) ? O_ADD : O_SUB;
		prop.addi_type = Addition_Type::AT_BASIC;
	} else if (fix.i_effect_value1 == Property_Type::PT_SPEED) {
		prop.fixed = fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay;
		prop.percent = 0.0;
		prop.basic = 0.0;
		//prop.operate = (fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay > 0) ? O_ADD : O_SUB;
		prop.addi_type = Addition_Type::AT_STATUS;
	} else {
		prop.fixed = fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay;
		prop.percent = fix.d_effect_value2 * fix.overlay - fix_from.d_effect_value2 * fix_from.overlay;
//		if (0 != fix.d_effect_value1 || 0 != fix_from.d_effect_value1) {
//			prop.operate = (fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay > 0) ? O_ADD : O_SUB;
//		} else if (0 != fix.d_effect_value2 || 0 != fix_from.d_effect_value2) {
//			prop.operate = (fix.d_effect_value2 * fix.overlay - fix_from.d_effect_value2 * fix_from.overlay > 0) ? O_ADD : O_SUB;
//		} else {
//			return 0;
//		}
		prop.addi_type = Addition_Type::AT_STATUS;
	}
	prop_vec.push_back(prop);

	MSG_81000061 msg;
	msg.role_id = role_id();
	msg.attack_id = fix.caster_id;
	msg.status_id = fix.status_id;
	msg.effect_id = fix.effect_id;
	//int ret = fighter()->fight_modify_fight_prop(prop_vec, msg.change_info);
//	int ret = fighter()->fight_modify_props(prop_vec, msg.change_info);
//
//	Block_Buffer buf;
//	MAKE_MSG_BUF(msg, buf);
//	call_broadcast_around(buf);
//
//	return ret;

	return fighter()->fight_modify_props_by_status(prop_vec, msg, bbc->is_fight_buff, 3);
}

int Status::cure_status_prop(Status_Effect_Detail& effect, int skill_id) {
	if (effect.i_effect_value1 < PT_FIGHT_PROP_START || effect.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;

	if (fighter()->dead_status()) return 0;

	Prop_Setter_Vec prop_vec;
	Prop_Setter prop;
	prop.prop_type = PT_BLOOD_CURRENT;
	prop.addi_type = Addition_Type::AT_BASIC;
	prop.operate = Operate_Type::O_ADD;

	double cure_value = 0.0;
	double cure_rate_caster = 0.0;
	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(effect.caster_id);
	Monster *monster = NPC_MANAGER->find_monster(effect.caster_id);
	Hero* hero = NPC_MANAGER->find_hero(effect.caster_id);
	if (0 != player) {
		cure_value = player->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1, Prop_Value::ELEM_FIGHT);
		cure_value *= effect.d_effect_value2;//val5
		cure_value += effect.d_effect_value4;//val7
		cure_rate_caster = player->fighter_detail().fetch_fight_property(PT_CURE_RATE);
		effect.last_cure_value = cure_value;
		effect.last_cure_rate = cure_rate_caster;
	} else if (monster) {
		cure_value = monster->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1, Prop_Value::ELEM_FIGHT);
		cure_value *= effect.d_effect_value2;//val5
		cure_value += effect.d_effect_value4;//val7
		cure_rate_caster = monster->fighter_detail().fetch_fight_property(PT_CURE_RATE);
		effect.last_cure_value = cure_value;
		effect.last_cure_rate = cure_rate_caster;
	} else if (hero) {
		cure_value = hero->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1, Prop_Value::ELEM_FIGHT);
		cure_value *= effect.d_effect_value2;//val5
		cure_value += effect.d_effect_value4;//val7
		cure_rate_caster = hero->fighter_detail().fetch_fight_property(PT_CURE_RATE);
		effect.last_cure_value = cure_value;
		effect.last_cure_rate = cure_rate_caster;
	} else {
		cure_value = effect.last_cure_value;
		cure_rate_caster = effect.last_cure_rate;
	}
	double cure_rate_self = fighter()->fighter_detail().fetch_fight_property(PT_BE_CURE_RATE);
	prop.basic = cure_value * effect.overlay * (1 + cure_rate_caster + cure_rate_self);
	prop_vec.push_back(prop);
//	MSG_DEBUG("role_id:%ld, cure_value(%lf), effect.overlay(%d), PT_BE_CURE_RATE(%lf), PT_CURE_RATE(%lf), value:%lf", role_id(), cure_value,
//			effect.overlay, cure_rate_self, cure_rate_caster, prop.basic);

	MSG_81000061 msg;
	msg.role_id = role_id();
	msg.attack_id = effect.caster_id;
	msg.status_id = effect.status_id;
	msg.effect_id = effect.effect_id;
	msg.result = 5;
	int ret = fighter()->fight_modify_fight_prop(prop_vec, msg.change_info);

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	call_broadcast_around(buf);

	return ret;
}

int Status::add_hurt_link(Status_Effect_Detail &effect) {
	Status_Link::iterator status_link_it = this->status_detail_->hurt_link.find(effect.status_id);
	if (status_link_it != this->status_detail_->hurt_link.end()) {
		status_link_it->second.main += effect.d_effect_value1;
		status_link_it->second.dis += effect.d_effect_value4;
	} else {
		Status_Link_Detail status_link_detail;
		status_link_detail.main = effect.d_effect_value1;
		status_link_detail.dis = effect.d_effect_value4;
		this->status_detail_->hurt_link.insert(std::make_pair(effect.status_id, status_link_detail));
	}
	return 0;
}

int Status::sub_hurt_link(Status_Effect_Detail &effect) {
	this->status_detail_->hurt_link.erase(effect.status_id);
	return 0;
}

int Status::cover_hurt_link(Status_Effect_Detail& effect, Status_Effect_Detail& effect_from) {
	this->sub_hurt_link(effect_from);
	this->add_hurt_link(effect);
	return 0;
}

int Status::add_cure_link(Status_Effect_Detail &effect) {
	Status_Link::iterator status_link_it = this->status_detail_->cure_link.find(effect.status_id);
	if (status_link_it != this->status_detail_->cure_link.end()) {
		status_link_it->second.main += effect.d_effect_value1;
		status_link_it->second.dis += effect.d_effect_value4;
	} else {
		Status_Link_Detail status_link_detail;
		status_link_detail.main = effect.d_effect_value1;
		status_link_detail.dis = effect.d_effect_value4;
		this->status_detail_->cure_link.insert(std::make_pair(effect.status_id, status_link_detail));
	}
	return 0;
}

int Status::sub_cure_link(Status_Effect_Detail &effect) {
	this->status_detail_->cure_link.erase(effect.status_id);
	return 0;
}

int Status::cover_cure_link(Status_Effect_Detail& effect, Status_Effect_Detail& effect_from) {
	this->sub_cure_link(effect_from);
	this->add_cure_link(effect);
	return 0;
}

int Status::add_shield_prop(Status_Effect_Detail& effect) {
	if (effect.i_effect_value1 < PT_FIGHT_PROP_START || effect.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;

	double shield_cfg = fighter()->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1);
	shield_cfg *= effect.d_effect_value2;
	shield_cfg += effect.d_effect_value4;

	this->status_detail_->shield_cfg += shield_cfg;
	if (this->status_detail_->shield_cfg <= 0) {
		this->status_detail_->shield_cfg = 0.0;
		this->status_detail_->shield_hurt = 0.0;
	}
	return 0;
}

int Status::sub_shield_prop(Status_Effect_Detail& effect) {
	if (effect.i_effect_value1 < PT_FIGHT_PROP_START || effect.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;

	double shield_cfg = fighter()->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1);
	shield_cfg *= effect.d_effect_value2;
	shield_cfg += effect.d_effect_value4;

	this->status_detail_->shield_cfg -= shield_cfg;
	this->status_detail_->shield_hurt -= shield_cfg;
	if (this->status_detail_->shield_cfg <= 0) {
		this->status_detail_->shield_cfg = 0.0;
		this->status_detail_->shield_hurt = 0.0;
	} else if (this->status_detail_->shield_hurt < 0) {
		this->status_detail_->shield_hurt = 0.0;
	}
	return 0;
}

int Status::cover_shield_prop(Status_Effect_Detail& effect, Status_Effect_Detail& effect_from) {
	if (effect.i_effect_value1 < PT_FIGHT_PROP_START || effect.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;

	double shield_cfg = fighter()->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1);
	shield_cfg *= effect.d_effect_value2;
	shield_cfg += effect.d_effect_value4;

	double shield_cfg_from = fighter()->fighter_detail().fetch_fight_property((Property_Type)effect_from.i_effect_value1);
	shield_cfg_from *= effect_from.d_effect_value2;
	shield_cfg_from += effect_from.d_effect_value4;

	this->status_detail_->shield_cfg += (shield_cfg - shield_cfg_from);
	this->status_detail_->shield_hurt -= shield_cfg_from;
	if (this->status_detail_->shield_cfg < 0) {
		this->status_detail_->shield_cfg = 0.0;
		this->status_detail_->shield_hurt = 0.0;
	} else if (this->status_detail_->shield_hurt < 0) {
		this->status_detail_->shield_hurt = 0.0;
	}
	return 0;
}

int Status::sync_status_info(void) {
	if (!status_detail_) {
		return 0;
	}
//	Scene_Player *sp = 0;
//	if (player_self()) {
//		sp = player_self();
//	} else if (hero_self()) {
//		sp = this->hero_self()->master();
//	}
//	if (!sp) {
//		return -1;
//	}
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_STATUS);
//	status_detail_->serialize(buf);
	buf.write_uint16(status_detail_->status_map.size());
	for (Status_Detail::Status_Map::const_iterator it = status_detail_->status_map.begin(); it != status_detail_->status_map.end(); ++it) {
		it->second->serialize(buf);
	}
	status_detail_->running_effect.serialize(buf);
	buf.finish_message();

	send_to_logic(buf);
	return 0;
}

int Status::load_status_info(Block_Buffer &buf) {
//	status_detail_->deserialize(buf);
	uint16_t len = 0;
	buf.read_uint16(len);
	Time_Value now;
	now = Time_Value::gettimeofday();
	for (uint16_t i = 0; i < len; ++i) {
		Status_Info status_info;// = POOL_MANAGER->pop_status_info_pool();
//		if (status_info) {
			status_info.deserialize(buf);
//			if ((now > status_info.last_end_time)
//				||(is_action_remove(status_info.status_id_) && status_info.cfg_action_times <= status_info.action_times)
//				|| (is_passive_remove(status_info.status_id_) && status_info.cfg_trigger_times <= status_info.trigger_times)) {
//				continue;
//			}
			this->insert_status(status_info);
//			status_detail_->status_map.insert(std::make_pair(status_info->status_id_, status_info));
//		}
	}
	status_detail_->running_effect.deserialize(buf);
	buff_insert_handle_switch_ = false;
	return 0;
}

int Status::inner_status_insert(Block_Buffer &buf) {
	Status_Effect_Argv argv;
	argv.deserialize(buf);
	this->insert_status(argv);
	return 0;
}

bool &Status::buff_insert_handle_switch(void) {
	return buff_insert_handle_switch_;
}

int Status::special_status_handle(void) {
	if (!player_self()) {
		return 0;
	}
	const int lv = 10;
	int status_id = CONFIG_CACHE_BUFF->special_level_buff().status_id;
	if (level() > lv) {
		if (has_status_id(status_id)) {
			erase_status_type(status_id);
		}
		return 0;
	} else if (!has_status_id(status_id)) {
		Status_Effect_Argv argv = CONFIG_CACHE_BUFF->special_level_buff();
		insert_status(argv);
	}
	if (fighter()->fighter_detail().current_morale() < 60) {
		Prop_Setter_Vec prop_vec;
		Prop_Setter prop;
		prop.prop_type = Property_Type::PT_MORALE_CURRENT;
		prop.fixed = 0.0;
		prop.percent = 0.0;
		prop.basic = 60 - fighter()->fighter_detail().current_morale();
		prop.addi_type = Addition_Type::AT_BASIC;
		prop_vec.push_back(prop);

		MSG_81000061 msg;
		msg.role_id = role_id();
		msg.attack_id = role_id();
		msg.status_id = status_id;
		msg.effect_id = 0;
		msg.skill_id = 0;
		if (fighter()) {
			fighter()->fight_modify_props_by_status(prop_vec, msg, true, 4);
		}
	}
	return 0;
}

int Status::hook_scene_change(int src_scene, int des_scene) {
	if (src_scene != des_scene) {
		for (Status_Detail::Status_Map::iterator it = status_detail_->status_map.begin();
				it != status_detail_->status_map.end(); ++it) {
			Status_Info *status_info = it->second;
			for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info->status_info_map.begin();
					status_info_map_it != status_info->status_info_map.end(); ++status_info_map_it) {
				for (Status_Info_Detail::Effect_Vec::iterator effect_map_it = status_info_map_it->second.effect_map.begin();
						effect_map_it != status_info_map_it->second.effect_map.end(); ++effect_map_it) {
					if (check_status_effect((*effect_map_it), src_scene) && !check_status_effect((*effect_map_it), des_scene)) {
						this->erase_status_effect_handle((*effect_map_it));
					} else if (!check_status_effect((*effect_map_it), src_scene) && check_status_effect((*effect_map_it), des_scene)) {
						this->insert_status_effect_handle((*effect_map_it), des_scene);
					}
				}
			}
		}
	}
	return 0;
}

int Status::insert_new_status(const Status_Info &detail, const Btl_Buff_Cfg *status_config) {
	Status_Info *status_info = POOL_MANAGER->pop_status_info_pool();
	if (!status_info) {
		MSG_USER("");
		return -1;
	}
	*status_info = detail;
	status_info->overlay_ = 1; // 最开始
	if (!this->status_detail_->status_map.insert(std::make_pair(detail.status_id_, status_info)).second) {
		return -1;
	}
	if (status_config->is_dispel) {
		status_detail_->status_dispel_set.insert(detail.status_id_);
	}
	if (status_config->is_pure) {
		status_detail_->status_pure_set.insert(detail.status_id_);
	}
	if (status_config->is_stoic) {
		status_detail_->status_stoic_set.insert(detail.status_id_);
	}
	if (status_config->buff_charactor == TYPE_CHARACTER_BAD) {
		status_detail_->status_evil_set.insert(detail.status_id_);
	}

	if (is_timepri_remove(status_info->status_id_)) {
		if (status_info->is_fight_buff) {
			insert_status_and_sort(status_info, this->status_detail_->status_timepri_list);
		} else {
			special_insert_handle(status_info, this->status_detail_->out_status_timepri_list);
		}
	}
	if (is_action_remove(status_info->status_id_)) {
		if (status_info->is_fight_buff) {
			insert_status_and_sort(status_info, this->status_detail_->status_action_list);
		} else {
			insert_status_and_sort(status_info, this->status_detail_->out_status_action_list);
		}
	}
	if (is_passive_remove(status_info->status_id_)) {
		if (status_info->is_fight_buff) {
			insert_status_and_sort(status_info, this->status_detail_->status_passive_list);
		} else {
			insert_status_and_sort(status_info, this->status_detail_->out_status_passive_list);
		}
	}

	for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info->status_info_map.begin();
			status_info_map_it != status_info->status_info_map.end(); ++status_info_map_it) {
		status_info_map_it->second.overlay = status_info->overlay_;
		insert_status_and_sort_timepri(&(status_info_map_it->second), status_detail_->timepri_list);
		insert_status_and_sort_action(&(status_info_map_it->second), status_detail_->action_list);
		insert_status_and_sort_passive(&(status_info_map_it->second), status_detail_->passive_list);
		insert_status_and_sort_once_timepri(&(status_info_map_it->second), status_detail_->once_timepri_list);//延迟处理 0间隔的
		for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
				it != status_info_map_it->second.effect_map.end(); ++it) {
			(*it).overlay = status_info->overlay_;
			if (((*it).heat_type.count(EFFECT_TYPE_TIME) && (*it).delay_time_tick > Time_Value::zero && Time_Value::zero == (*it).interval)
					|| (*it).delay_action_tick > 0 || (*it).delay_passive_tick > 0 || (*it).delay_time_tick > Time_Value::zero) {
			} else {
				if (!buff_insert_handle_switch_) {
					this->insert_status_effect_handle(status_info_map_it->second, (*it));
				} else {
					if (!check_status_effect((*it))) {
						this->erase_status_effect_handle((*it));
//						this->erase_status_effect_handle(status_info_map_it->second, (*it));
					} else if (!it->buff_scene.empty()){
						this->insert_status_effect_handle(status_info_map_it->second, (*it));
					}
				}
				if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
					return -1;
			}
		}
	}
//	active_update_status(*status_info);
	return 0;
}

int Status::merge_status_to_status_overlay(Status_Info &status_info_to, Status_Info &status_info_from, bool has_overlay) {
	//被插入，只有一个
	Status_Info::Status_Info_Map::iterator status_info_map_it_from = status_info_from.status_info_map.begin();
	if (status_info_map_it_from == status_info_from.status_info_map.end()) {
		return -1;
	}
//	//处理、清除其它人施放的buff效果:不可叠加effect的效果、层数
//	for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
//			status_info_map_it != status_info_to.status_info_map.end(); ++status_info_map_it) {
//		if (status_info_map_it->first == status_info_map_it_from->first) {
//			continue;
//		}
//		Status_Info_Detail status_info_detail_old = status_info_map_it->second;
//		for (Status_Info_Detail::Effect_Map::iterator it = status_info_map_it->second.effect_map.begin();
//				it != status_info_map_it->second.effect_map.end(); ++it) {
//			if (!it->second.is_overlay) {
//				this->erase_status_effect_handle(status_info_map_it->second, it->second);//延迟?相对于后面的插入
//				it->second.overlay = 0;
//			} else {
//				it->second.overlay = status_info_to.overlay_;
//				this->update_status_effect_handle(status_info_map_it->second, status_info_detail_old,
//						it->second, status_info_detail_old.effect_map[it->first]);
//			}
//		}
//	}

	Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.find(status_info_map_it_from->first);
	if (has_overlay) {
		std::deque<role_id_t>::const_iterator status_fifo_it = status_info_to.status_fifo.begin();
		if (status_fifo_it != status_info_to.status_fifo.end()) {
			if (*status_fifo_it == status_info_map_it_from->first) { // 已存在,且是要被替换的，不替换
				return 0; ///
			}
			Status_Info::Status_Info_Map::iterator it_pop = status_info_to.status_info_map.find(*status_fifo_it);
			if (it_pop != status_info_to.status_info_map.end()) {
				Status_Info_Detail status_info_detail_old = it_pop->second;
				--it_pop->second.overlay;
				if (it_pop->second.effect_map.size() == status_info_detail_old.effect_map.size()) {
					uint16_t len = 0;
					for (Status_Info_Detail::Effect_Vec::iterator effect_it = it_pop->second.effect_map.begin();
							effect_it != it_pop->second.effect_map.end(); ++effect_it) {
						this->update_status_effect_handle(it_pop->second, status_info_detail_old,
								(*effect_it), status_info_detail_old.effect_map[len]);
						if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
							return 0;
						++len;
					}
				} else {
					MSG_USER("effect map error:not equal");
				}
				//根据层数处理
				if (it_pop->second.overlay <= 0) {
					status_info_to.status_info_map.erase(it_pop);
				}
			} else {
				MSG_USER("ERROR_IMPOSSIBLE");
			}
		} else {
			MSG_USER("ERROR_IMPOSSIBLE: someone modify progress!");
		}
	}

	if (status_info_map_it == status_info_to.status_info_map.end()) { // 当前人物还未插入
		if (status_info_to.status_info_map.insert(*status_info_map_it_from).second) {
			status_info_map_it = status_info_to.status_info_map.find(status_info_map_it_from->first);
			if (status_info_map_it != status_info_to.status_info_map.end()) {
				status_info_map_it->second.overlay = status_info_to.overlay_;
				insert_status_and_sort_timepri(&(status_info_map_it->second), status_detail_->timepri_list);
				insert_status_and_sort_action(&(status_info_map_it->second), status_detail_->action_list);
				insert_status_and_sort_passive(&(status_info_map_it->second), status_detail_->passive_list);
				insert_status_and_sort_once_timepri(&(status_info_map_it->second), status_detail_->once_timepri_list);
				for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
						it != status_info_map_it->second.effect_map.end(); ++it) {
					(*it).overlay = status_info_to.overlay_;
					this->insert_status_effect_handle(status_info_map_it->second, (*it));
					if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
						return 0;
				}
			}
		}
	} else {
		/*
		 * 已插入：处理buff,effect层数效果,持续时间
		 * */
		Status_Info_Detail status_info_detail_old = status_info_map_it->second;
		status_info_map_it->second.overlay = status_info_to.overlay_;
		uint16_t len = 0;
		for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
				it != status_info_map_it->second.effect_map.end(); ++it) {
			if ((*it).is_overlay) {
				(*it).overlay = status_info_to.overlay_;
				this->update_status_effect_handle(status_info_map_it->second, status_info_detail_old,
						(*it), status_info_detail_old.effect_map[len]);
			}
			++len;
		}
	}

	return 0;
}

int Status::merge_status_to_status(Status_Info &status_info_to, Status_Info &status_info_from,const Btl_Buff_Cfg *status_config) {
	if (!status_config) {
		return -1;
	}
//	if (status_info_to.status_info_map.size() != status_info_from.status_info_map.size()) {
//		return ERROR_CLIENT_OPERATE;
//	}
//	status_info_to.cfg_action_times = status_info_from.cfg_action_times;
//	status_info_to.cfg_trigger_times = status_info_from.cfg_trigger_times;
	Time_Value last_end_time = status_info_to.last_end_time;
	status_info_to.last_caster_master_id_ = status_info_from.last_caster_master_id_;
	status_info_to.last_caster_role_id_ = status_info_from.last_caster_role_id_;
	status_info_to.last_end_time = status_info_from.last_end_time;
	status_info_to.last_start_time = status_info_from.last_start_time;
	status_info_to.last_time_ms = status_info_from.last_time_ms;
	status_info_to.last_status_from_ = status_info_from.last_status_from_;
	status_info_to.status_id_ = status_info_from.status_id_;

	if (is_timepri_remove(status_info_to.status_id_)) {
		if (status_info_to.is_fight_buff) {
			replace_status_and_sort(&status_info_to, this->status_detail_->status_timepri_list);
		} else {
			special_replace_handle(&status_info_to, this->status_detail_->out_status_timepri_list, last_end_time);
		}
	}
	if (is_action_remove(status_info_to.status_id_)) {
		if (status_info_to.is_fight_buff) {
			replace_status_and_sort(&status_info_to, this->status_detail_->status_action_list);
		} else {
			replace_status_and_sort(&status_info_to, this->status_detail_->out_status_action_list);
		}
	}
	if (is_passive_remove(status_info_to.status_id_)) {
		if (status_info_to.is_fight_buff) {
			replace_status_and_sort(&status_info_to, this->status_detail_->status_passive_list);
		} else {
			replace_status_and_sort(&status_info_to, this->status_detail_->out_status_passive_list);
		}
	}

	if (status_info_to.status_info_map.empty()) {
		if (!status_info_from.status_info_map.empty()) {
			MSG_DEBUG("ERROR_IMPOSSIBLE!");
			status_info_to.overlay_ = 1;
			status_info_to.status_info_map = status_info_from.status_info_map;
			Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
			status_info_map_it->second.overlay = status_info_to.overlay_;
			insert_status_and_sort_timepri(&(status_info_map_it->second), status_detail_->timepri_list);
			insert_status_and_sort_action(&(status_info_map_it->second), status_detail_->action_list);
			insert_status_and_sort_passive(&(status_info_map_it->second), status_detail_->passive_list);
			insert_status_and_sort_once_timepri(&(status_info_map_it->second), status_detail_->once_timepri_list);
			for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				(*it).overlay = status_info_to.overlay_;
				this->insert_status_effect_handle(status_info_map_it->second, (*it));
				if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
					return 0;
			}
		}
		return 0;
	} else {
		for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
				status_info_map_it != status_info_to.status_info_map.end(); ++status_info_map_it) {
			remove_status_and_sort_timepri(&(status_info_map_it->second), status_detail_->timepri_list);
			remove_status_and_sort_action(&(status_info_map_it->second), status_detail_->action_list);
			remove_status_and_sort_passive(&(status_info_map_it->second), status_detail_->passive_list);
			remove_status_and_sort_once_timepri(&(status_info_map_it->second), status_detail_->once_timepri_list);
		}
	}

	++status_info_to.overlay_;
	bool has_overlay = false; // 是否有超过,有则处理掉旧的
	if (status_config->overlay < status_info_to.overlay_) {
		status_info_to.overlay_ = status_config->overlay;
		has_overlay = true;
	}

	Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
	if (status_info_map_it != status_info_to.status_info_map.end()) {
		insert_status_and_sort_timepri(&(status_info_map_it->second), status_detail_->timepri_list);
		insert_status_and_sort_action(&(status_info_map_it->second), status_detail_->action_list);
		insert_status_and_sort_passive(&(status_info_map_it->second), status_detail_->passive_list);
		insert_status_and_sort_once_timepri(&(status_info_map_it->second), status_detail_->once_timepri_list);
	}
//	if (status_config->is_effect_un_overlay) {
//		merge_status_to_status_unoverlay(status_info_to, status_info_from);
//	} else { // 所有效果都可叠
		merge_status_to_status_overlay(status_info_to, status_info_from, has_overlay);
//	}

	return 0;
}

int Status::cover_status_to_status(Status_Info &status_info_to, Status_Info &status_info_from) {
	Time_Value last_end_time = status_info_to.last_end_time;
	status_info_to.last_caster_master_id_ = status_info_from.last_caster_master_id_;
	status_info_to.last_caster_role_id_ = status_info_from.last_caster_role_id_;
	status_info_to.last_end_time = status_info_from.last_end_time;
	status_info_to.last_start_time = status_info_from.last_start_time;
	status_info_to.last_time_ms = status_info_from.last_time_ms;
	status_info_to.last_status_from_ = status_info_from.last_status_from_;
	status_info_to.overlay_ = 1; // 因为是覆盖，只存在一个
	status_info_to.status_id_ = status_info_from.status_id_;

	if (is_timepri_remove(status_info_to.status_id_)) {
		if (status_info_to.is_fight_buff) {
			replace_status_and_sort(&status_info_to, this->status_detail_->status_timepri_list);
		} else {
			special_replace_handle(&status_info_to, this->status_detail_->out_status_timepri_list, last_end_time);
		}
	}
	if (is_action_remove(status_info_to.status_id_)) {
		if (status_info_to.is_fight_buff) {
			replace_status_and_sort(&status_info_to, this->status_detail_->status_action_list);
		} else {
			replace_status_and_sort(&status_info_to, this->status_detail_->out_status_action_list);
		}
	}
	if (is_passive_remove(status_info_to.status_id_)) {
		if (status_info_to.is_fight_buff) {
			replace_status_and_sort(&status_info_to, this->status_detail_->status_passive_list);
		} else {
			replace_status_and_sort(&status_info_to, this->status_detail_->out_status_passive_list);
		}
	}

	// 因为是覆盖，只存在一个
	if (status_info_to.status_info_map.empty()) {
		if (!status_info_from.status_info_map.empty()) {
			MSG_DEBUG("ERROR_IMPOSSIBLE!");
			status_info_to.status_info_map = status_info_from.status_info_map;
			Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
			status_info_map_it->second.overlay = status_info_to.overlay_;
			insert_status_and_sort_timepri(&(status_info_map_it->second), status_detail_->timepri_list);
			insert_status_and_sort_action(&(status_info_map_it->second), status_detail_->action_list);
			insert_status_and_sort_passive(&(status_info_map_it->second), status_detail_->passive_list);
			insert_status_and_sort_once_timepri(&(status_info_map_it->second), status_detail_->once_timepri_list);
			for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				(*it).overlay = status_info_to.overlay_;
				this->insert_status_effect_handle(status_info_map_it->second, (*it));
				if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
					return 0;
			}
		}
		return 0;
	} else {
		Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
		if (status_info_map_it != status_info_to.status_info_map.end()) {
			remove_status_and_sort_timepri(&(status_info_map_it->second), status_detail_->timepri_list);
			remove_status_and_sort_action(&(status_info_map_it->second), status_detail_->action_list);
			remove_status_and_sort_passive(&(status_info_map_it->second), status_detail_->passive_list);
			remove_status_and_sort_once_timepri(&(status_info_map_it->second), status_detail_->once_timepri_list);
		}
	}

	Status_Info::Status_Info_Map status_info_map_old = status_info_to.status_info_map;
	// from获取已经拥有的action_times,next_opt
	if (!status_info_from.status_info_map.empty()) {
		Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
		Status_Info::Status_Info_Map::iterator status_info_map_from_it = status_info_from.status_info_map.begin();
		if (status_info_map_it->second.effect_map.size() == status_info_map_it->second.effect_map.size()) {
			uint16_t len = 0;
			for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				status_info_map_from_it->second.effect_map[len].action_times = (*it).action_times;
				status_info_map_from_it->second.effect_map[len].next_opt = (*it).next_opt;
				++len;
			}
		} else {
			MSG_TRACE_ABORT("ERROR_IMPOSSIBLE:maybe config : %d", status_info_from.status_id_);
		}
	}
	status_info_to.status_info_map = status_info_from.status_info_map;

	// 因为是覆盖，只存在一个
	if (!status_info_map_old.empty() && !status_info_to.status_info_map.empty()) {
		Status_Info::Status_Info_Map::iterator status_info_map_old_it = status_info_map_old.begin();
		Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
		if (status_info_map_old_it != status_info_map_old.end()
				&& status_info_map_it != status_info_to.status_info_map.end()) {
			status_info_map_it->second.overlay = status_info_to.overlay_;
			insert_status_and_sort_timepri(&(status_info_map_it->second), status_detail_->timepri_list);
			insert_status_and_sort_action(&(status_info_map_it->second), status_detail_->action_list);
			insert_status_and_sort_passive(&(status_info_map_it->second), status_detail_->passive_list);
			insert_status_and_sort_once_timepri(&(status_info_map_it->second), status_detail_->once_timepri_list);
			if (status_info_map_it->second.effect_map.size() == status_info_map_old_it->second.effect_map.size()) {
				uint16_t len = 0;
				for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
						it != status_info_map_it->second.effect_map.end(); ++it) {
					(*it).overlay = status_info_to.overlay_;
					this->update_status_effect_handle(status_info_map_it->second, status_info_map_old_it->second,
							(*it), status_info_map_old_it->second.effect_map[len]);
					if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
						return 0;
					++len;
				}
			} else {
				MSG_TRACE_ABORT("ERROR_IMPOSSIBLE:maybe config : %d", status_info_from.status_id_);
			}
		} else {
			MSG_TRACE_ABORT("ERROR_IMPOSSIBLE: BUG : %d", status_info_from.status_id_);
		}
	} else {
		MSG_USER("ERROR_IMPOSSIBLE:maybe config : %d", status_info_from.status_id_);
	}

	return 0;
}

int Status::special_insert_handle(Status_Info *status_info_to, Status_Detail::Status_Info_List &status_list) {
	if (!status_info_to) return -1;
	if (status_detail_->running_effect.exp_map.empty() && status_detail_->running_effect.exp_two_map.empty()) {
		insert_status_and_sort(status_info_to, status_list);
		return -1;
	}
	bool is_exp = false;
	bool is_exp_two = false;
	for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to->status_info_map.begin();
			status_info_map_it != status_info_to->status_info_map.end(); ++status_info_map_it) {
		for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
				it != status_info_map_it->second.effect_map.end(); ++it) {
			if (it->effect_type == Status_Def::STATUS_EFFECT_TYPE_EXP) {
				is_exp = true;
			} else if (it->effect_type == Status_Def::STATUS_ET_EXP_TWO) {
				is_exp_two = true;
			}
		}
	}
	if (is_exp_two && is_exp) {
		MSG_TRACE_ABORT("status_id(%d)", status_info_to->status_id_);
	}
//	if (is_exp && !status_detail_->running_effect.exp_two_map.empty()) {
//		Int_Double_Map::iterator it = status_detail_->running_effect.exp_two_map.begin();
//		if (it != status_detail_->running_effect.exp_two_map.end()) {
//			Status_Detail::Status_Map::iterator it_sim = status_detail_->status_map.find(it->first);
//			if (it_sim != status_detail_->status_map.end()) {
//				status_info_to->last_end_time = status_info_to->last_end_time + Time_Value(it_sim->second->last_time_ms/ 1000);
//			}
//		}
//	} else
	if (is_exp_two && !status_detail_->running_effect.exp_map.empty()) {
		Int_Double_Map::iterator it = status_detail_->running_effect.exp_map.begin();
		if (it != status_detail_->running_effect.exp_map.end()) {
			Status_Detail::Status_Map::iterator it_sim = status_detail_->status_map.find(it->first);
			if (it_sim != status_detail_->status_map.end()) {
				it_sim->second->last_end_time = it_sim->second->last_end_time + Time_Value(status_info_to->last_time_ms/ 1000);
				replace_status_and_sort(it_sim->second, status_list);
				active_update_status(it_sim->second);
			}
		}
	}
	insert_status_and_sort(status_info_to, status_list);
	return 0;
}
int Status::special_replace_handle(Status_Info *status_info_to, Status_Detail::Status_Info_List &status_list, const Time_Value &last_end_time) {
	if (!status_info_to) return -1;
	if (status_detail_->running_effect.exp_map.empty() && status_detail_->running_effect.exp_two_map.empty()) {
		replace_status_and_sort(status_info_to, status_list);
		return -1;
	}
	bool is_exp = false;
	bool is_exp_two = false;
	for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to->status_info_map.begin();
			status_info_map_it != status_info_to->status_info_map.end(); ++status_info_map_it) {
		for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
				it != status_info_map_it->second.effect_map.end(); ++it) {
			if (it->effect_type == Status_Def::STATUS_EFFECT_TYPE_EXP) {
				is_exp = true;
			} else if (it->effect_type == Status_Def::STATUS_ET_EXP_TWO) {
				is_exp_two = true;
			}
		}
	}
	if (is_exp_two && is_exp) {
		MSG_TRACE_ABORT("status_id(%d)", status_info_to->status_id_);
	}
	if (is_exp || is_exp_two) {
		status_info_to->last_end_time = last_end_time + Time_Value(status_info_to->last_time_ms / 1000);
	}
//	if (is_exp && !status_detail_->running_effect.exp_two_map.empty()) {
//		Int_Double_Map::iterator it = status_detail_->running_effect.exp_two_map.begin();
//		if (it != status_detail_->running_effect.exp_two_map.end()) {
//			Status_Detail::Status_Map::iterator it_sim = status_detail_->status_map.find(it->first);
//			if (it_sim != status_detail_->status_map.end()) {
//				status_info_to->last_end_time = status_info_to->last_end_time + Time_Value(it_sim->second->last_time_ms/ 1000);
//			}
//		}
//	} else
	if (is_exp_two && !status_detail_->running_effect.exp_map.empty()) {
		Int_Double_Map::iterator it = status_detail_->running_effect.exp_map.begin();
		if (it != status_detail_->running_effect.exp_map.end()) {
			Status_Detail::Status_Map::iterator it_sim = status_detail_->status_map.find(it->first);
			if (it_sim != status_detail_->status_map.end()) {
				it_sim->second->last_end_time = it_sim->second->last_end_time + Time_Value(status_info_to->last_time_ms/ 1000);
				replace_status_and_sort(it_sim->second, status_list);
				active_update_status(it_sim->second);
			}
		}
	}
	replace_status_and_sort(status_info_to, status_list);
	return 0;
}

void Status::active_erase_status(const Status_Info_Detail &status_info) {
//	MSG_801801 msg;
//	msg.status_id = status_info.status_id;
//	msg.status_caster = status_info.caster_role_id;
//	Fighter* fighter = dynamic_cast<Fighter*>(this);
//	if (NULL != fighter) {
//		msg.status_owner = fighter->fighter_detail().role_id;
//	} else {
//		msg.status_owner = status_detail_->role_id;
//	}
//
//	Block_Buffer buf;
//	buf.make_message(ACTIVE_STATUS_ERASE_UPDATE);
//	msg.serialize(buf);
//	buf.finish_message();
//
//	active_status_buf(is_need_broadcast(status_info.status_id), buf);
}

void Status::active_update_status(const Status_Info *status_info) {
	if (!status_info) return;
	MSG_81000071 msg;
	msg.type = (int)status_info->is_fight_buff;
	Status_Info_Msg msginfo;
	msginfo.status_owner = role_id();
	msginfo.status_id = status_info->status_id_;
	msginfo.overlay = status_info->overlay_;
	msginfo.last_time_ms = status_info->last_time_ms;
	msginfo.remain_time = status_info->last_end_time.sec() - Time_Value::gettimeofday().sec();
	if (msginfo.remain_time < 0) {
		msginfo.remain_time = 0;
	}
	for (Status_Info::Status_Info_Map::const_iterator status_info_map_it = status_info->status_info_map.begin();
				status_info_map_it != status_info->status_info_map.end(); ++status_info_map_it) {
		for (Status_Info_Detail::Effect_Vec::const_iterator it = status_info_map_it->second.effect_map.begin();
				it != status_info_map_it->second.effect_map.end(); ++it) {
			if (it->effect_type == Status_Def::STATUS_ET_CHANGE_FASHION) {
				msginfo.source_id = it->i_effect_value1;
				break;
			}
		}
	}
	make_buff_msg(status_info, msginfo);
	msg.buff_detail.push_back(msginfo);

	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	call_broadcast_around(buf);
}

void Status::active_status_buf(bool broadcast, Block_Buffer &buf) {
	if (broadcast) {
//		Mover *mover_self = this->mover_self();
//		if (mover_self) {
//			mover_self->broadcast_around(buf);
//		}
	} else {
		this->send_to_client(buf);
	}
}

