/*
 * Fighter_Ext.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "item_user/Item_User_Def.h"
#include "Fighter.h"
#include "Scene_Module_Head.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Fighter.h"
#include "Move_Scene.h"
#include "Battle_Scene.h"
#include "Fighter_Func.h"
#include "Fighter_Struct.h"
#include "Err_Code.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "Msg_Role_Scene_Struct.h"
#include "Fighter_Struct.h"
#include "Msg_Fight_Struct.h"
#include "status/Status_Struct.h"
#include "NPC/hero/Hero.h"
#include "Scene_Public.h"
#include "Config_Cache_Scene.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_Item.h"

/*======================================================================================*
 * 学习技能代码(begin)
 *======================================================================================*/

int Fighter::add_skill_extra(int skill_id, int skill_lv) {
	if (0 == fighter_detail_) return -1;
	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (0 == cfg) return -1;
	const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_lv);
	if (0 == dtl) return -1;

	if (cfg->skill_sort == Skill_Sort::BE_SKILL) {
		Prop_Setter_Vec setter_vec;
//		Skill_DB_Info_Map::iterator find_it = fighter_detail_->extra_skill_map.find(skill_id);
//		if (find_it != fighter_detail_->extra_skill_map.end()) {
//			const Skill_Detail* pre_dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(find_it->second.skill_id,
//					find_it->second.skill_level);
//			if (0 != pre_dtl ) { // sub pre skill level attribute
//				get_be_skill_add_attr(*pre_dtl, setter_vec, true);
//			}
//		}
		get_be_skill_add_attr(*dtl, setter_vec, false);
		MSG_81000060 msg;
		msg.role_id = role_id();
		fight_modify_props(setter_vec, msg.change_info);
		THIS_SEND_TO_CLIENT(msg);
	}

	Skill_DB_Info skill_info;
	skill_info.skill_id = skill_id;
	skill_info.skill_level = skill_lv;
	fighter_detail_->extra_skill_vec.push_back(skill_info);

	return 0;
}

void Fighter::remove_skill_extra(int skill_id, int skill_lv) {
	if (0 == fighter_detail_) return;
//	Skill_DB_Info_Map::iterator find_it = fighter_detail_->extra_skill_map.find(skill_id);
	Skill_DB_Info skill_info;
	skill_info.skill_id = skill_id;
	skill_info.skill_level = skill_lv;
	Skill_DB_Info_Vec::iterator find_it = std::find(fighter_detail_->extra_skill_vec.begin(),
			fighter_detail_->extra_skill_vec.end(), skill_info);
	if (find_it == fighter_detail_->extra_skill_vec.end()) return;

	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_lv);
	if (0 != cfg && 0 != dtl && cfg->skill_sort == Skill_Sort::BE_SKILL) {
		Prop_Setter_Vec setter_vec;
		get_be_skill_add_attr(*dtl, setter_vec, true);
//		Prop_Setter setter;
//		for (Be_Skill_Effect_Map::const_iterator it = dtl->be_skill_effect_map.begin();
//				it != dtl->be_skill_effect_map.end(); ++it) {
//			for (Be_Skill_Attr_Vec::const_iterator a_it = it->second.attr_vec.begin();
//					a_it != it->second.attr_vec.end(); ++a_it) {
//				setter.reset();
//				setter.prop_type = static_cast<Property_Type>(a_it->id);
//				setter.addi_type = Addition_Type::AT_PASSIVE_SKILL;
//				setter.operate = Operate_Type::O_ADD;
//				setter.basic = 0;
//				setter.fixed = -a_it->fix;
//				setter.percent = -a_it->percnet;
//				setter_vec.push_back(setter);
//			}
//		}
		MSG_81000060 msg;
		msg.role_id = role_id();
		fight_modify_props(setter_vec, msg.change_info);
		THIS_SEND_TO_CLIENT(msg);
	}

	fighter_detail_->extra_skill_vec.erase(find_it);

//	sync_force_to_client();
}

void Fighter::cost_skill_pointer(const int skill_points, int talent_id) {
	Talent_Map::iterator it = fighter_detail_->talent_map.find(talent_id);
	if (it == fighter_detail_->talent_map.end()) return;
	it->second.skill_point = it->second.skill_point > skill_points ?
			it->second.skill_point - skill_points : 0;
}

void Fighter::add_skill_pointer(const int skill_points) {
	fighter_detail_->ttl_skill_points_ += skill_points;
	for (Talent_Map::iterator it = fighter_detail_->talent_map.begin();
			it != fighter_detail_->talent_map.end(); ++it) {
		it->second.skill_point += skill_points;
		sync_all_skill_to_client(it->second.talent_id);
	}
	player_self()->sync_fighter_detail_to_logic();
}

int Fighter::learn_skill(int skill_id, int skill_level, int talent_id) {
	const Skill_Config * scfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	const Skill_Detail * cfg = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_level);
	if (!scfg || !cfg) return  0;

	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(talent_id);
	if (find_it == fighter_detail_->talent_map.end()) return ERROR_INNER_SERVER;
	if (find_it->first != scfg->skill_series && 0 != scfg->skill_series) return ERROR_INNER_SERVER;

	// 检查是否已学习该技能
	Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.find(skill_id);
	if (it == find_it->second.skill_db_info_map.end()) { //未学习该技能
		if (skill_level > 1) return ERROR_PRE_SKILL_LIM;
	} else {
		if (it->second.skill_level >= skill_level) { //已学习该类型技能
			return ERROR_SKILL_HAS_LEARNED;
		}
		if (it->second.skill_level < (skill_level-1)) { //技能等级不足，不能学习该等级技能
			return ERROR_PRE_SKILL_LIM;
		}
	}

	//检查前置技能条件
	if (0 != cfg->pre_skill.size()) {
		bool pre_cond = false;
		for (Int_Int_Vec::const_iterator iter = cfg->pre_skill.begin();
				iter != cfg->pre_skill.end(); ++iter) {
			Skill_DB_Info_Map::iterator pre_it =
					find_it->second.skill_db_info_map.find(iter->val_1);
			if (pre_it != find_it->second.skill_db_info_map.end() &&
				pre_it->second.skill_level >= iter->val_2) {
				pre_cond = true;
				break;
			}
		}
		if (!pre_cond) return ERROR_PRE_SKILL_LIM;
	}
//	if (0 != cfg->pre_skill) {
//		Skill_DB_Info_Map::iterator pre_it =
//				find_it->second.skill_db_info_map.find(cfg->pre_skill);
//		if (pre_it == find_it->second.skill_db_info_map.end()) {
//			return ERROR_PRE_SKILL_LIM;
//		}
//		if (pre_it->second.skill_level < cfg->pre_skill_lv) {
//			return ERROR_PRE_SKILL_LIM;
//		}
//	}

	//检查互斥条件
//	if (0 != cfg->mutex_cond) {
//		Skill_DB_Info_Map::iterator m_skill_it = find_it->second.skill_db_info_map.find(cfg->mutex_cond);
//		if (m_skill_it != find_it->second.skill_db_info_map.end()) {
//			return ERROR_MUTEXT_SKILL;
//		}
//	}

	int ret = save_learned_skill(skill_id, skill_level, talent_id);
	return ret;
}

int Fighter::save_learned_skill(int skill_id, int skill_lv, int talent_id) {
	const Skill_Config *skill_config = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (!skill_config) {
		return ERROR_NO_SUCH_SKILL;
	}
	Skill_Detail_Map::const_iterator sd_it = skill_config->skill_level_map.find(skill_lv);
	if (sd_it == skill_config->skill_level_map.end()) {
		return ERROR_NO_SUCH_SKILL;
	}
	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(talent_id);
	if (find_it == fighter_detail_->talent_map.end()) return ERROR_INNER_SERVER;

	if (find_it->second.skill_point < sd_it->second.yl_cost ) {
		return ERROR_SKILL_YL_LIM;
	}

	MSG_20100016 cost_msg;
	cost_msg.skill_id = skill_id;
	cost_msg.skill_lv = skill_lv;
	THIS_SEND_TO_LOGIC(cost_msg);

	cost_skill_pointer(sd_it->second.yl_cost, talent_id);

	MSG_50100141 msg;
	Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.find(skill_id);
	if (it == find_it->second.skill_db_info_map.end()) {
		int loc = 0;
		if (Skill_Type::TYPE_UNIQUE_SKILL == skill_config->skill_type) {
			loc = 5;
			for (it = find_it->second.skill_db_info_map.begin();
					it != find_it->second.skill_db_info_map.end(); ++it) {
				if (it->second.skill_loc == loc) {
					loc = 0;
					break;
				}
			}
		} else if (Skill_Sort::ACT_SKILL == skill_config->skill_sort) {
			loc = get_skill_null_cell(find_it->second);
		}

		Skill_DB_Info info;
		info.skill_id = skill_id;
		info.skill_level = skill_lv;
		info.skill_loc = loc;
		find_it->second.skill_db_info_map.insert(std::make_pair(skill_id, info));
		msg.skill.skill_loc = loc;

		MSG_20100017 to_logic_msg;
		to_logic_msg.skill_id = skill_id;
		to_logic_msg.lv = skill_lv;
		THIS_SEND_TO_LOGIC(to_logic_msg);
	} else {
		it->second.skill_level = skill_lv;
		msg.skill.skill_loc = it->second.skill_loc;
	}
	msg.skill.skill_id = skill_id;
	msg.skill.skill_lv = skill_lv;
	msg.talent_id = talent_id;
	msg.skill_point = find_it->second.skill_point;
	THIS_SEND_TO_CLIENT(msg);

	update_attr_when_learn_be_skill(*skill_config, sd_it->second);

	sync_force_to_client();

	player_self()->sync_fighter_detail_to_logic();

	return 0;
}

void Fighter::update_attr_when_learn_be_skill(const Skill_Config & cfg, const Skill_Detail& dtl) {
	if (cfg.skill_sort != Skill_Sort::BE_SKILL) return;
	Prop_Setter_Vec setter_vec;
	if (dtl.skill_level > 1) {
		Skill_Detail_Map::const_iterator find_it = cfg.skill_level_map.find(dtl.skill_level -1);
		if (find_it != cfg.skill_level_map.end()) {
			get_be_skill_add_attr(find_it->second, setter_vec, true);
		}
	}
	get_be_skill_add_attr(dtl, setter_vec, false);

	MSG_81000060 msg;
	msg.role_id = role_id();
	fight_modify_props(setter_vec, msg.change_info);
	THIS_SEND_TO_CLIENT(msg);
}

int Fighter::change_skill_loc(int skill_id, int new_loc, int talent_id) {
	const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (0 == cfg || Skill_Sort::BE_SKILL == cfg->skill_sort) return ERROR_INNER_SERVER;

	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(talent_id);
	if (find_it == fighter_detail_->talent_map.end()) {
		return ERROR_INNER_SERVER;
	}

	Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.find(skill_id);
	if (it == find_it->second.skill_db_info_map.end()) {
		return ERROR_INNER_SERVER;
	}

	if (0 != new_loc) {

		// check mutex
		const Skill_Detail *skill_cfg = CONFIG_CACHE_FIGHTER->skill_detail_cache(
				it->second.skill_id, it->second.skill_level);
		if (0 == skill_cfg) return ERROR_CONFIG_NOT_EXIST;
		for (Int_Vec::const_iterator id_it = skill_cfg->mutex_cond_vec.begin();
				id_it != skill_cfg->mutex_cond_vec.end(); ++id_it) {
			Skill_DB_Info_Map::iterator check_it = find_it->second.skill_db_info_map.find(*id_it);
			if (find_it->second.skill_db_info_map.end() == check_it) continue;
			if (0 == check_it->second.skill_loc) continue;
			return ERROR_MUTEXT_SKILL;
		}

		for (Skill_DB_Info_Map::iterator old_it =
				find_it->second.skill_db_info_map.begin();
				old_it != find_it->second.skill_db_info_map.end(); ++old_it) {
			if (new_loc == old_it->second.skill_loc) {
				old_it->second.skill_loc = it->second.skill_loc;
				break;
			}
		}
	}
	it->second.skill_loc = new_loc;
	sync_all_skill_to_client(talent_id);
	return 0;
}

int Fighter::erase_skill(int Type, int talent_id) {

	if (validate_battle_scene()) {
		MSG_DEBUG("int battle cannot erase skill, role_id=%ld, talent_id=%d", role_id(), talent_id);
		return ERROR_INNER_SERVER;
	}
	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(talent_id);
	if (find_it == fighter_detail_->talent_map.end())
		return 0;

	Player_Init_Cache::Role_Default_Skill_Map::const_iterator role_skill_it =
			CONFIG_CACHE_ROLE->player_init_cache().default_skill.find(career());
	if (role_skill_it == CONFIG_CACHE_ROLE->player_init_cache().default_skill.end())
		return ERROR_INNER_SERVER;

	Player_Init_Cache::Default_Skill_Map::const_iterator def_it = role_skill_it->second.find(talent_id);
	if (def_it == role_skill_it->second.end())
		return ERROR_INNER_SERVER;

	bool need_erase_skill = false;
	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		if (it->second.skill_id != def_it->second.val_1 ||
				it->second.skill_id != def_it->second.val_2) {
			need_erase_skill = true;
			break;
		}
		if (it->second.skill_level != 1) {
			need_erase_skill = true;
			break;
		}
	}

	if (!need_erase_skill) return 0;

	MSG_20100024 msg;
	msg.type = 1;
	msg.arg1 = talent_id;
	msg.cost = fighter_detail_->reset_skill_cost();
	THIS_SEND_TO_LOGIC(msg);

	return 0;
}

// 最终洗点技能
void Fighter::erase_skill_after_cost(int talent_id) {
	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(talent_id);
	if (find_it == fighter_detail_->talent_map.end()) return;

	Player_Init_Cache::Role_Default_Skill_Map::const_iterator role_skill_it =
			CONFIG_CACHE_ROLE->player_init_cache().default_skill.find(career());
	if (role_skill_it == CONFIG_CACHE_ROLE->player_init_cache().default_skill.end()) return;

	Player_Init_Cache::Default_Skill_Map::const_iterator def_it =
			role_skill_it->second.find(talent_id);
	if (def_it == role_skill_it->second.end()) return;

	Prop_Setter_Vec setter_vec;

	// 处理被动技能加属性的部分
	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || cfg->skill_sort != Skill_Sort::BE_SKILL) continue;
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(
				it->second.skill_id, it->second.skill_level);
		if (0 == dtl) continue;
		get_be_skill_add_attr(*dtl, setter_vec, true);
	}

	int ret_skill_point = 0;
	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ) {
		int min_lv = 0;
		if (it->second.skill_id == def_it->second.val_1 ||
				it->second.skill_id == def_it->second.val_2) {
			min_lv = 1;
		}
		if (it->second.skill_level > min_lv) {
			for (int lv = it->second.skill_level; lv > min_lv; --lv) {
				const Skill_Detail *cfg_skill =
						CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id, lv);
				if (0 != cfg_skill)
					ret_skill_point += cfg_skill->yl_cost;
			}
			if (min_lv == 0) {
				it = find_it->second.skill_db_info_map.erase(it);
				continue;
			} else {
				it->second.skill_level = 1;
			}
		}
		++it;
	}
	++fighter_detail_->reset_skill_times;

	find_it->second.skill_point += ret_skill_point;
	MSG_81000060 msg;
	msg.role_id = role_id();
	fight_modify_props(setter_vec, msg.change_info);
	THIS_SEND_TO_CLIENT(msg);
	sync_all_skill_to_client(talent_id);
	player_self()->sync_fighter_detail_to_logic();
}

int Fighter::open_a_talent(int talent_id) {
	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(talent_id);
	if (find_it != fighter_detail_->talent_map.end())
		return ERROR_INNER_SERVER;

	MSG_20100024 msg;
	msg.type = 2;
	msg.arg1 = talent_id;

	THIS_SEND_TO_LOGIC(msg);
	return 0;
}

int Fighter::use_a_talent_page(int talent_id) {
	if (validate_battle_scene()) {
		MSG_USER("enter_battle_scene, role: %ld, talent:%d", role_id(), fighter_detail().cur_used_talent_);
		return ERROR_INNER_SERVER;
	}

	Talent_Map::iterator find_it = fighter_detail_->talent_map.find(talent_id);
	if (find_it == fighter_detail_->talent_map.end()) {
		return ERROR_INNER_SERVER;
	}

	Player_Init_Cache::Role_Default_Skill_Map::const_iterator role_skill_it =
			CONFIG_CACHE_ROLE->player_init_cache().default_skill.find(career());
	if (role_skill_it == CONFIG_CACHE_ROLE->player_init_cache().default_skill.end()) return ERROR_INNER_SERVER;
	Player_Init_Cache::Default_Skill_Map::const_iterator default_skill_find_it =
			role_skill_it->second.find(talent_id);
	if (default_skill_find_it == role_skill_it->second.end()) return ERROR_INNER_SERVER;

	int talent_default_skill_id = default_skill_find_it->second.val_1;
	Skill_DB_Info_Map::iterator skill_db_find_it = find_it->second.skill_db_info_map.find(talent_default_skill_id);
	if (skill_db_find_it == find_it->second.skill_db_info_map.end()) return ERROR_INNER_SERVER;

	int tmp_id = fighter_detail_->cur_used_talent_;
	fighter_detail_->default_skill = talent_default_skill_id;
	fighter_detail_->cur_used_talent_ = talent_id;

	change_be_skill(tmp_id, talent_id);
	sync_all_skill_to_client(talent_id);

	MSG_50100144 msg;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

void Fighter::change_be_skill(int org_talent_id, int cur_talent_id) {
	Talent_Map::iterator org_it = fighter_detail_->talent_map.find(org_talent_id);
	if (org_it == fighter_detail_->talent_map.end()) {
		MSG_USER("cannot find talent page, role_id=%ld, page_id=%d", role_id(), org_talent_id);
		return;
	}
	Talent_Map::iterator cur_it = fighter_detail_->talent_map.find(cur_talent_id);
	if (cur_it == fighter_detail_->talent_map.end()) {
		MSG_USER("cannot find talent page, role_id=%ld, page_id=%d", role_id(), cur_talent_id);
		return;
	}

	Prop_Setter_Vec setter_vec;

	for (Skill_DB_Info_Map::iterator it = org_it->second.skill_db_info_map.begin();
			it != org_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || cfg->skill_sort != Skill_Sort::BE_SKILL) continue;
		const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id, it->second.skill_level);
		if (0 == dtl) continue;
		get_be_skill_add_attr(*dtl, setter_vec, true);
	}

	for (Skill_DB_Info_Map::iterator it = cur_it->second.skill_db_info_map.begin();
			it != org_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg || cfg->skill_sort != Skill_Sort::BE_SKILL) continue;
		const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id, it->second.skill_level);
		if (0 == dtl) continue;
		get_be_skill_add_attr(*dtl, setter_vec, false);
	}

	MSG_81000060 msg;
	msg.role_id = role_id();
	fight_modify_props(setter_vec, msg.change_info);
	THIS_SEND_TO_CLIENT(msg);
}

int Fighter::skill_cost_and_save(int type, int arg1, int arg2) {
	switch(type) {
	case 1:	{ 		//洗点
		erase_skill_after_cost(arg1);
		break;
	}
	case 2: {
		Player_Init_Cache::Role_Default_Skill_Map::const_iterator role_skill_it =
				CONFIG_CACHE_ROLE->player_init_cache().default_skill.find(career());
		if (role_skill_it == CONFIG_CACHE_ROLE->player_init_cache().default_skill.end()) return 0;
		Player_Init_Cache::Default_Skill_Map::const_iterator find_it = role_skill_it->second.find(arg1);
		if (find_it == role_skill_it->second.end()) return 0;

		Talent talent;
		talent.talent_id = arg1;
		talent.skill_point = fighter_detail_->ttl_skill_points_;

		Skill_DB_Info skill;
		skill.skill_id = find_it->second.val_1;
		skill.skill_level = 1;
		skill.skill_loc = 0;
		talent.skill_db_info_map[skill.skill_id] = skill;

		skill.reset();
		skill.skill_id = find_it->second.val_2;
		skill.skill_level = 1;
		skill.skill_loc = 1;
		talent.skill_db_info_map[skill.skill_id] = skill;

		fighter_detail_->talent_map[talent.talent_id] = talent;
		sync_all_skill_to_client(talent.talent_id);
		player_self()->sync_fighter_detail_to_logic();
		break;
	}
	default:
		break;
	}

	return 0;
}

int Fighter::get_skill_null_cell(const Talent &talent) const {

	bool loc[] = {false, false, false, false, false};
	for (Skill_DB_Info_Map::const_iterator it = talent.skill_db_info_map.begin();
			it != talent.skill_db_info_map.end(); ++it) {
		if (it->second.skill_loc != 0 && it->second.skill_loc != 5){
			if ((it->second.skill_loc > 4) || (it->second.skill_loc < 1)) {
				MSG_USER("skill index error");
			} else {
				loc[it->second.skill_loc] = true; //此位置存在技能
			}
		}
	}
	for (int i=1; i < 5; ++i) {
		if (loc[i] == false) { //此位置未放置技能
			return i;
		}
	}
	return 0;
}

void Fighter::sync_all_skill_to_client(int talent_id) {
	MSG_50100140 msg;
	Talent_Map::iterator it = fighter_detail_->talent_map.find(talent_id);
	if (it == fighter_detail_->talent_map.end()) return;

	msg.cur_used_talent = fighter_detail_->cur_used_talent_;
	msg.reset_skill_cost = fighter_detail_->reset_skill_cost();
	msg.ttl_skill_point = fighter_detail_->ttl_skill_points_;
	Skill_Talent_Info talent_info;
	talent_info.talent_id = talent_id;
	talent_info.skill_point = it->second.skill_point;
	Skill_Info info;
	for (Skill_DB_Info_Map::const_iterator sit = it->second.skill_db_info_map.begin();
			sit != it->second.skill_db_info_map.end(); ++sit) {
		info.reset();
		info.skill_id = sit->second.skill_id;
		info.skill_lv = sit->second.skill_level;
		info.skill_loc = sit->second.skill_loc;
		talent_info.skill_info_vec.push_back(info);
	}
	msg.skill_talent_info_vec.push_back(talent_info);
	THIS_SEND_TO_CLIENT(msg);

	sync_force_to_client();
}

/*======================================================================================*
 * 学习技能代码(end)
 *======================================================================================*/

int Fighter::req_battle_status(Block_Buffer &buf) {
	role_id_t role_id = 0;
	buf.read_int64(role_id);

	Block_Buffer res_buf;
	res_buf.make_message(INNER_BATTLE_STATUS);
	res_buf.write_int64(role_id);
	res_buf.write_bool(this->is_in_battle());
	res_buf.finish_message();
	this->send_to_logic(res_buf);
	return 0;
}

int Fighter::restore_full_prop(Block_Buffer &buf) {
	int32_t type = 0;
	buf.read_int32(type);
	if (type < PT_START || type > PT_FIGHT_PROP_END) {
		return -1;
	}
	if (type != PT_EXP_CURRENT && type != PT_BLOOD_CURRENT
			&& type != PT_MORALE_CURRENT && type != PT_STRENGTH_CURRENT
			&& type != PT_BLOOD_POOL_CURRENT) {
		return -1;
	}
	switch (type) {
	case PT_EXP_CURRENT: {
		int64_t exp_up = CONFIG_CACHE_ROLE->level_up_need_exp_map(level());
		double val = this->fighter_detail_->fetch_normal_property(PT_EXP_CURRENT);
		this->modify_experience(exp_up - val);
		break;
	}
	case PT_BLOOD_CURRENT: {
		this->restore_hp(blood_max(), true);
		break;
	}
	case PT_MORALE_CURRENT: {
		int32_t per = 0;
		buf.read_int32(per);
		if (-1 == per) {
			this->set_clear_label(true);
		} else {
			if (per == 0) {
				per = 100;
			} else if (per > 100) {
				per = 100;
			}
			double val = this->fighter_detail_->current_morale();
			double val_max = this->fighter_detail_->max_morale();
			this->set_clear_label(false);
			if (val_max <= val) {
				return -1;
			}
			if (val >= val_max * per / 100) {
				return -1;
			}
			Prop_Setter_Vec setter_vec;
			Prop_Setter setter(PT_MORALE_CURRENT, AT_BASIC, O_ADD, (val_max * per / 100 - val), 0.0, 0.0);
			setter_vec.push_back(setter);

			// this->fight_modify_fight_prop(setter_vec, true);
			modify_props_normal(setter_vec, true, 8);
		}
		break;
	}
	case PT_STRENGTH_CURRENT: {
		double val = this->fighter_detail_->fetch_fight_property(PT_STRENGTH_CURRENT);
		double val_max = this->fighter_detail_->fetch_fight_property(PT_STRENGTH_MAX);
		if (val_max <= val) {
			MSG_DEBUG("morale full!");
			return -1;
		}
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter(PT_STRENGTH_CURRENT, AT_BASIC, O_ADD, (val_max - val), 0.0, 0.0);
		setter_vec.push_back(setter);

		//this->fight_modify_fight_prop(setter_vec, true);
		modify_props_normal(setter_vec, false, 9);
		break;
	}
	case PT_BLOOD_POOL_CURRENT: {
		double val = blood_pool_current();
		double val_max = blood_pool_max();
		if (val_max <= val) {
			MSG_DEBUG("morale full!");
			return -1;
		}
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter(PT_BLOOD_POOL_CURRENT, AT_BASIC, O_ADD, (val_max - val), 0.0, 0.0);
		setter_vec.push_back(setter);

		// this->fight_modify_fight_prop(setter_vec, true);
		modify_props_normal(setter_vec, false, 10);
		break;
	}
	}
	return 0;
}

int Fighter::use_drug_inner(const int16_t tp, const uint32_t index, const double val, const int amount, Block_Buffer &buf) {
	MSG_20100305 inner_msg;
	inner_msg.tp = tp;
	inner_msg.index = index;
	inner_msg.amount = 0;
	inner_msg.val = val;
	switch(tp) {
	case ITEM_USE_DRUG: {
		int32_t item_id = 0;
		int8_t usetype = 0;
		buf.read_int32(item_id);
		buf.read_int8(usetype);
		switch(usetype) {
		case ITEM_USE_DRUG_BLOOD_POOL: {
			int j = 0;
			for (int i =0; i< amount; ++i) {
				double left = val;
				double need = blood_max() - blood_current();
				if (need > 0) {
					need = need > left ? left : need;
					left = left -need;
					restore_hp(need, false);
				}
				need = blood_pool_max() - blood_pool_current();
				if (need > 0) {
					this->fight_basic_prop_modify(PT_BLOOD_POOL_CURRENT, AT_BASIC, O_ADD, left);
				}
				++j;
				if (need < left) break;
			}
			inner_msg.amount = j;
			{
				Block_Buffer buf__;
				buf__.make_message(inner_msg.msg_id);
				inner_msg.serialize(buf__);
				buf__.write_int32(item_id);
				buf__.finish_message();
				this->send_to_logic(buf__);
			}
			break;
		}
		case ITEM_USE_DRUG_STRENGTH: {
			int cur_strength = fighter_detail_->current_strength();
			int max_strength = fighter_detail_->max_strength();
			double left = val;
			double need = max_strength - cur_strength;
			if (need > left * amount) {
				need = left * amount;
				restore_phy_power(need);
				inner_msg.amount = amount;
			} else {
				restore_phy_power(need);
				int am = floor(need / val);
				if (need > am * val) {
					++am;
				}
				inner_msg.amount = am;
			}
			{
				Block_Buffer buf__;
				buf__.make_message(inner_msg.msg_id);
				inner_msg.serialize(buf__);
				buf__.write_int32(item_id);
				buf__.finish_message();
				this->send_to_logic(buf__);
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	case ITEM_USE_EXP:
		this->modify_experience(val * amount, O_ADD);
		break;
	case ITEM_USE_BUFF: {
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(val);
		if (item_config && item_config->special_cfg.trigger_buff.status_id) {
			for (int i = 0; i < amount; ++i) {
				Status_Effect_Argv argv = item_config->special_cfg.trigger_buff;
				Status::insert_status_out_fight(argv);
			}
		}
		break;
	}
	default:
		break;
	}
	return 0;
}

int Fighter::cost_phy_power(int value){
	int cur_strength = fighter_detail_->current_strength();
	if(value > cur_strength){
		return -1;
	}
//	if(phy_power_recover_time() == 0){
//		Time_Value time = Time_Value::gettimeofday() + Time_Value(PHY_POWER_RECOVER_TIME_VALUE);
//		set_phy_power_recover_time(time.sec());
//	}
	int new_phy_power = cur_strength - value;
	set_strength_cur(new_phy_power);
	// 在聊天框显示扣除体力
	MSG_81000102 msg;
	msg.type = 1;
	Property prop;
	prop.type = 507505;
	prop.value = value;
	msg.property.push_back(prop);
	THIS_SEND_TO_CLIENT(msg);
	// sync client
	sync_phy_power_to_client();
	// sync logic
	sync_phy_power_to_logic();
	return 0;
}

int Fighter::restore_phy_power(int value){
	if (value == 0) {
		return 0;
	}
	int cur_strength = fighter_detail_->current_strength();
	int max_strength = fighter_detail_->max_strength();

	int new_phy_power = cur_strength + value;
	if(new_phy_power > max_strength){
		new_phy_power = max_strength;
		//set_phy_power_recover_time(0);
	}
	set_strength_cur(new_phy_power);
	// 在聊天框显示扣除体力
	MSG_81000102 msg;
	msg.type = 0;
	Property prop;
	prop.type = 507505;
	prop.value = value;
	msg.property.push_back(prop);
	THIS_SEND_TO_CLIENT(msg);
	// sync client
	sync_phy_power_to_client();
	// sync logic
	sync_phy_power_to_logic();
	return 0;
}

int Fighter::time_up(const Time_Value &now){
	Status::time_up(now);

	return 0;
}

void Fighter::sync_phy_power_info(uint16_t cur, int32_t recover_time, uint8_t buy_times){
	fighter_detail_->strength_current.basic.basic_value = cur;
	fighter_detail_->phy_power_recover_time = recover_time;
	fighter_detail_->phy_power_buy_times = buy_times;
	sync_phy_power_to_client();
}

void Fighter::sync_phy_power_to_client(void){
	MSG_81000063 msg;
	msg.phy_power_buy_times = fighter_detail_->phy_power_buy_times;
	msg.phy_power_cur = fighter_detail_->current_strength();
	msg.phy_power_max = fighter_detail_->max_strength();
	int remain_time = 0;
	if(fighter_detail_->phy_power_recover_time > 0){
		remain_time = fighter_detail_->phy_power_recover_time - Time_Value::gettimeofday().sec();
	}
	msg.phy_power_recover_time = remain_time;
	THIS_SEND_TO_CLIENT(msg);
}

void Fighter::sync_phy_power_to_logic(void){
	Block_Buffer buf;
	MSG_20100030 msg;
	msg.phy_power_buy_times = fighter_detail_->phy_power_buy_times;
	msg.phy_power_cur = fighter_detail_->current_strength();
	msg.phy_power_recover_time = fighter_detail_->phy_power_recover_time;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	this->send_to_logic(buf);
}

Battle_Status Fighter::battle_status(void) {
	return battle_status_;
}

bool Fighter::set_clear_label(bool is_clear_morale) {
	bool old = this->fighter_detail_->is_clear_moral_;
	this->fighter_detail_->is_clear_moral_ = is_clear_morale;
	return old;
}

void Fighter::sync_force_to_client(void) {
	MSG_81000060 msg;
	msg.role_id = role_id();
	Prop_Change change;
	change.prop_type = Property_Type::PT_FORCE;

	fighter_detail_->update_force();
	change.current_val = fighter_detail_->fetch_fight_property(PT_FORCE);
	msg.change_info.push_back(change);
	THIS_SEND_TO_CLIENT(msg);

	MSG_20100027 to_logic_msg;
	to_logic_msg.force = fighter_detail_->force;
	THIS_SEND_TO_LOGIC(to_logic_msg);
}

void Fighter::clear_currnet_morale(void) {
	if (mover_type() != MOVER_T_PLAYER) return;
	if (!fighter_detail_->is_clear_moral_) return;

	double init_value = fighter_detail_->fetch_fight_property(Property_Type::PT_INIT_MORALE);
	Fighter* fighter = SCENE_PUBLIC->find_fight_hero(role_id());
	if (0 != fighter && 0 != fighter->npc_self()) {
		init_value += fighter->npc_self()->npc_detail().fighter_detail_.fetch_fight_property(Property_Type::PT_INIT_MORALE);
	}

	Prop_Setter_Vec vec;
	Prop_Setter setter;
	setter.basic = init_value;
	setter.fixed = 0;
	setter.percent = 0;
	setter.prop_type = Property_Type::PT_MORALE_CURRENT;
	setter.addi_type = Addition_Type::AT_BASIC;
	setter.operate = Operate_Type::O_SET;
	vec.push_back(setter);
	modify_props_normal(vec, false, 2);
}

int Fighter::fight_normal_prop_modify(Property_Type pen, Operate_Type oen,
		int value, bool board) {
	MSG_81000060 msg;
	msg.role_id = role_id();
	Prop_Change pc;
	pc.prop_type = pen;
	int64_t old_value = fighter_detail_->fetch_normal_property(pen);
	if (fighter_detail_->modify_normal_property(pen, oen, value)) {
		return -1;
	}
	pc.current_val = fighter_detail_->fetch_normal_property(pen);
	pc.change_val = pc.current_val - old_value;
	msg.change_info.push_back(pc);

	/// 修改属性造成影响
	switch (pen) {
	case PT_LEVEL: {
		// 更新出现缓存
		appear_buff().set_ext_flag(true);

		// 等级基础属性
		const Prop_Value_Vec *prop_base_value = CONFIG_CACHE_ROLE->level_fighter_prop_cache(level());
		if (prop_base_value) {
			Prop_Setter_Vec prop;
			for (Prop_Value_Vec::const_iterator it = prop_base_value->begin(); it != prop_base_value->end(); ++it) {
				Property_Type prop_type = Property_Type((*it).id);
				if (prop_type == PT_SKILL_POINT || prop_type == PT_BLOOD_POOL_CURRENT) continue;
				prop.push_back(Prop_Setter(prop_type, AT_BASIC, O_SET, (*it).value, 0.0, 0.0));
			}

			if (!prop.empty()) {
				//fight_modify_fight_prop(prop, false);
				modify_props_normal(prop, false, 5);
			}
		} else {
			MSG_USER("base property config error, level:%d", level());
		}

		// 升级加技能点
		int skill_point_ = CONFIG_CACHE_ROLE->level_up_add_skill_points(level());
		if ( skill_point_ > 0) {
			add_skill_pointer(skill_point_);
		}

		// 满级回满血
		if (this->blood_current() < this->blood_max()) {
			restore_hp(blood_max(), true);
		}

		// 自动学技能
		// auto_learn_skill(level());

		// 升级需要经验
		Prop_Change exp_pc;
		exp_pc.prop_type = PT_EXP_LEVEL_UP;
		exp_pc.current_val = CONFIG_CACHE_ROLE->level_up_need_exp_map(level());
		msg.change_info.push_back(exp_pc);

		MSG_50200032 res_msg;
		res_msg.role_id = role_id();
		fighter_detail().property_list(res_msg.prop_value);

		Hero *hero = SCENE_PUBLIC->find_fight_hero(player_self()->role_id());
		if (hero) {
			int hero_init_morale_type = hero->fighter_detail().init_morale.type;
			double hero_init_morale_val = hero->fighter_detail().init_morale.total();
			for (uint16_t i = 0; i < res_msg.prop_value.size(); ++i) {
				if (res_msg.prop_value[i].type == hero_init_morale_type) {
					res_msg.prop_value[i].value += hero_init_morale_val;
				}
			}
		}

		res_msg.prop_value.push_back(Property(PT_CAREER, this->career()));

		res_msg.role_name = this->role_name();/*人物姓名*/
		res_msg.gang_name = player_self()->base_detail().gang_name;/*人物id*/
		res_msg.gender = player_self()->base_detail().gender;/*性别（1男，2女）*/
		res_msg.gang_id = player_self()->base_detail().gang_id;/*职业*/
		res_msg.headship = player_self()->ganger_detail().headship;
		res_msg.avatar_vec = player_self()->base_detail().avatar_vec;

		THIS_SEND_TO_CLIENT(res_msg);

		if (battle_scene()) {
			Mover *mover = battle_scene()->find_mover_with_role_id(role_id());
			if (mover) {
				MSG_DEBUG("level up role:%ld  %f b_id:%d", role_id(), pc.current_val, this->battle_scene_id());
			} else {
				MSG_DEBUG("level up role:%ld  %f b_id:%d", role_id(), pc.current_val, this->battle_scene_id());
			}
		} else {
			MSG_DEBUG("level up role:%ld  %f b_id:%d scene:%d", role_id(), pc.current_val, 0, this->move_scene_id());
		}
		break;
	}
	case PT_EXP_CURRENT: {

		break;
	}
	default : {
		MSG_USER_TRACE("unknow property:%d", pen);
		return -1;
	}
	}

	if (board) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		scene_broad_around(buf);
	} else {
		THIS_SEND_TO_CLIENT(msg);
	}

	// 通知逻辑服
	MSG_20200110 inner_msg;
	inner_msg.prop_type = pen;
	inner_msg.value = pc.current_val;
	THIS_SEND_TO_LOGIC(inner_msg);

	if (pen == PT_FORCE) {
		if (move_scene() && move_scene()->scene_id()) {
			const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(move_scene()->scene_id());
			if (cfg && Scene_Gang_War == cfg->manager) {
				player_self()->gang_war_info().force = pc.current_val;
			}
		}
	}
	return 0;
}

int Fighter::modify_props_normal(const Prop_Setter_Vec &prop, bool board, int from) {
	MSG_81000060 msg;
	msg.role_id = role_id();

	Prop_Change force(PT_FORCE);
	force.old_val = fighter_detail_->force;

	Int_Double_Map force_map;
	get_all_pre_force(force_map);

	fight_modify_fight_prop(prop, msg.change_info);

	fighter_detail_->update_force();
	force.current_val = fighter_detail_->force;
	force.change_val = force.current_val - force.old_val;

	if (force.change_val > 0.0000000001 || force.change_val < -0.0000000001) {
		msg.change_info.push_back(force);

		MSG_20100027 to_logic_msg;
		to_logic_msg.force = fighter_detail_->force;
		THIS_SEND_TO_LOGIC(to_logic_msg);
		for (std::vector<Prop_Change>::const_iterator it = msg.change_info.begin(); it != msg.change_info.end(); ++it) {
			MSG_DEBUG("%d  %f  %f", (*it).prop_type, (*it).change_val, (*it).old_val, (*it).current_val);
		}
		print_all_part_force(prop[0].addi_type, force.change_val, force_map, from);
	}

	if (msg.change_info.empty()) return 0;

	if (board) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		scene_broad_around(buf);
	} else {
		THIS_SEND_TO_CLIENT(msg);
	}

	for (Prop_Setter_Vec::const_iterator it = prop.begin();
			it != prop.end(); ++it) {
		if (it->prop_type != PT_STRENGTH_CURRENT) continue;
		if (!(it->basic > 0.0)) continue;
		sync_phy_power_to_logic();
		MSG_81000102 phy_msg;
		phy_msg.type = 0;
		Property prop;
		prop.type = 507505;
		prop.value = it->basic;
		phy_msg.property.push_back(prop);
		THIS_SEND_TO_CLIENT(phy_msg);
	}

	return 0;
}

void Fighter::print_all_part_force(int index, double change, Int_Double_Map force_map, int from) {

	if (0 == player_self()) return;
	if (change > 0.0000000001)
		MSG_DEBUG("from:%d force change, ++++++++++, role_id=%ld, change part=%d, current=%d, change_value=%f", from, role_id(), index, fighter_detail_->force, change);
	if (change < -0.0000000001)
		MSG_DEBUG("from:%d force change, ----------, role_id=%ld, change part=%d, current=%d, change_value=%f", from, role_id(), index, fighter_detail_->force, change);

	MSG_DEBUG("role_id=%ld, 0=%f, 1=%f, 2=%f, 4=%f, 5=%f, 6=%f, 7=%f, 8=%f, 9=%f, 10=%f, 11=%f, 12=%f, 13=%f, 14=%f, 15=%f, 16=%f, skill=%f",
			role_id(),
			force_map[AT_BASIC],
			force_map[AT_EQUIP],
			force_map[AT_CONVERSION],
			force_map[AT_STATUS],
			force_map[AT_PASSIVE_SKILL],
			force_map[AT_RUNE_STONE],
//			force_map[AT_RUNE],
			force_map[AT_HERO_ASSIST],
			force_map[AT_GANG],
			force_map[AT_ARTIFACTS],
			force_map[AT_ACHIEVEMENT],
			force_map[AT_TITLE],
			force_map[AT_DRAGON_VALE],
			force_map[AT_ANCI_ARTIFACT],
			force_map[AT_WING],
			force_map[AT_AWAKE],
			force_map[AT_MOUNT],
			force_map[100]);
	MSG_DEBUG("role_id=%ld, 0=%f, 1=%f, 2=%f, 4=%f, 5=%f, 6=%f, 7=%f, 8=%f, 9=%f, 10=%f, 11=%f, 12=%f, 13=%f, 14=%f, 15=%f, 16=%f, skill=%f",
			role_id(),
			fighter_detail_->get_force_part(AT_BASIC),			//0
			fighter_detail_->get_force_part(AT_EQUIP),			//1
			fighter_detail_->get_force_part(AT_CONVERSION),		//2
			fighter_detail_->get_force_part(AT_STATUS),			//4
			fighter_detail_->get_force_part(AT_PASSIVE_SKILL),	//5
			fighter_detail_->get_force_part(AT_RUNE_STONE),		//6
//			fighter_detail_->get_force_part(AT_RUNE),			//7
			fighter_detail_->get_force_part(AT_HERO_ASSIST),
			fighter_detail_->get_force_part(AT_GANG),			//8
			fighter_detail_->get_force_part(AT_ARTIFACTS),		//9
			fighter_detail_->get_force_part(AT_ACHIEVEMENT),	//10
			fighter_detail_->get_force_part(AT_TITLE),			//11
			fighter_detail_->get_force_part(AT_DRAGON_VALE),	//12
			fighter_detail_->get_force_part(AT_ANCI_ARTIFACT),	//13
			fighter_detail_->get_force_part(AT_WING),			//14
			fighter_detail_->get_force_part(AT_AWAKE),			//15
			fighter_detail_->get_force_part(AT_MOUNT),
			fighter_detail_->get_skill_force_ttl());
//	for (int i = AT_BASIC; i < AT_END; ++i) {
//		if (i == AT_INITIATIVE_SKILL) continue;
//		double ttl = fighter_detail_->get_force_part(i);
//		MSG_DEBUG("role_id=%ld, part=%d, force=%f", role_id(), i, ttl);
//	}
}

void Fighter::get_all_pre_force(Int_Double_Map &force_map) {
	for (int i = AT_BASIC; i < AT_END; ++i) {
		if (i == AT_INITIATIVE_SKILL) continue;
		double ttl = fighter_detail_->get_force_part(i);
		force_map[i] = ttl;
	}
	double ttl= fighter_detail_->get_skill_force_ttl();
	force_map[100] = ttl;
}
