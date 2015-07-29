/*
 * Logic_Pet.cpp
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */

#include "Logic_Pet.h"
#include "Logic_Player.h"
#include "Pet_Struct.h"
#include "Pet_Config.h"
#include "Fighter_Detail.h"
#include "Msg_Pet_Struct.h"
#include "Msg_Active_Struct.h"
#include "Config_Cache_Pet.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Fighter.h"
#include "Err_Code.h"
#include "pack/Packer.h"
#include "Pet_Def.h"

Logic_Pet::Logic_Pet(void) {
	reset();
}

Logic_Pet::~Logic_Pet(void) {
}

void Logic_Pet::reset(void) {

	pet_detail_ = 0;
}

void Logic_Pet::load_detail(Pet_Detail &detail) {

	pet_detail_ = &detail;
}

void Logic_Pet::save_detail(Pet_Detail &detail) {

	if (pet_detail_->is_change) {
		detail = pet_detail();
		pet_detail_->is_change = false;
	}
}

void Logic_Pet::sign_int(void) {

}

void Logic_Pet::sign_out(void) {

}

void Logic_Pet::module_init(void) {

	if (0 == pet_detail_->lv) return;

	refresh_pet_attr(0, pet_detail_->lv, pet_detail_->illusion_vec);
	refresh_pet_skill(pet_detail_->skill_map);
}

int Logic_Pet::handle_get_pet_active_item(void) {

	// task condition checks

	Item_Detail item_detail(ACTIVE_PET_ITEM_ID, 1, Item_Detail::BIND);
	int ret = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item_detail,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_WING));
	if (0 != ret) return ret;

	MSG_50113007 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Logic_Pet::handle_get_pet_info(void) {

	MSG_50113001 msg;

	msg.lv = pet_detail_->lv;
	msg.state = pet_detail_->state;
	msg.upgrade_val = pet_detail_->upgrade_val;
	msg.active_illusion_id = pet_detail_->active_illusion_id;
	msg.illusion_vec = pet_detail_->illusion_vec;
	Int_Int temp_val;
	for (Int_Int_Map::iterator it = pet_detail_->skill_map.begin();
			it != pet_detail_->skill_map.end(); ++it) {
		temp_val.reset();
		temp_val.val_1 = it->first;
		temp_val.val_2 = it->second;
		msg.skill_vec.push_back(temp_val);
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Logic_Pet::handle_pet_upgrade_once(int auto_buy) {

	if (0 == pet_detail_) return ERROR_INNER_SERVER;
	if (0 == pet_detail_->lv) return ERROR_INNER_SERVER;
	const Pet_Config_Map &pet_cfg_map = CONFIG_CACHE_PET->get_pet_config_map();
	int max_lv = pet_cfg_map.size();
	if (pet_detail_->lv >= max_lv) return ERROR_INNER_SERVER;

	const Pet_Config *pet_cfg = CONFIG_CACHE_PET->get_pet_config(pet_detail_->lv);
	if (0 == pet_cfg) return ERROR_CONFIG_NOT_EXIST;

	int ret = pet_upgrade(*pet_cfg, auto_buy);
	if (0 != ret) return ret;

	MSG_50113002 msg;
	msg.result = 0;
	msg.lv = pet_detail_->lv;
	msg.upgrade_val = pet_detail_->upgrade_val;

	THIS_SEND_TO_CLIENT(msg);

	pet_detail_->detail_change();
	if (pet_cfg->lv == pet_detail_->lv) return 0; // not upgrade
	pet_illusion_active_by_lv(pet_detail_->lv);
	Int_Vec illusion_vec;
	refresh_pet_attr(pet_cfg->lv, pet_detail_->lv, illusion_vec);
	player_self()->heroer_prop_refresh();

	return 0;
}

int Logic_Pet::handle_pet_upgrade_auto(int auto_buy) {

	if (0 == pet_detail_) return ERROR_INNER_SERVER;
	if (0 == pet_detail_->lv) return ERROR_INNER_SERVER;
	const Pet_Config_Map &pet_cfg_map = CONFIG_CACHE_PET->get_pet_config_map();
	int max_lv = pet_cfg_map.size();
	if (pet_detail_->lv >= max_lv) return ERROR_INNER_SERVER;

	const Pet_Config *pet_cfg = CONFIG_CACHE_PET->get_pet_config(pet_detail_->lv);
	if (0 == pet_cfg) return ERROR_CONFIG_NOT_EXIST;

	bool success_once = false;
	int ret = 0;
	for (int i=0; i<100; ++i) {
		ret = pet_upgrade(*pet_cfg, auto_buy);
		if (0 == ret) success_once = true;
		if (0 != ret) break;
		if (pet_cfg->lv != pet_detail_->lv) break;
	}

	if (!success_once) return ret;

	MSG_50113002 msg;
	msg.result = 0;
	msg.lv = pet_detail_->lv;
	msg.upgrade_val = pet_detail_->upgrade_val;

	THIS_SEND_TO_CLIENT(msg);

	pet_detail_->detail_change();
	if (pet_cfg->lv == pet_detail_->lv)  return 0;// not upgraded
	pet_illusion_active_by_lv(pet_detail_->lv);
	Int_Vec illusion_vec;
	refresh_pet_attr(pet_cfg->lv, pet_detail_->lv, illusion_vec);
	player_self()->heroer_prop_refresh();

	return 0;
}

int Logic_Pet::pet_upgrade(const Pet_Config &pet_cfg, int auto_buy) {

	Money_Sub_Info gold_sub_info(Money_Sub_Type(COPPER_ONLY),
			pet_cfg.gold_cost, MONEY_SUB_UPGRADE_WING);

	int ret = player_self()->pack_try_sub_money(gold_sub_info);
	if (0 != ret) return ret;

	int item_amount = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, pet_cfg.item_cost.val_1);
	if (item_amount < pet_cfg.item_cost.val_2 && 0 == auto_buy) {
		MSG_82100008 msg;
		msg.item_ids.push_back(pet_cfg.item_cost.val_1);
		THIS_SEND_TO_CLIENT(msg);
		return ERROR_ITEM_NOT_ENOUGH;
	}
	item_amount = item_amount > pet_cfg.item_cost.val_2 ?
			pet_cfg.item_cost.val_2 : item_amount;

	if (item_amount > 0) {
		Id_Amount pack_item(pet_cfg.item_cost.val_1, item_amount);
		ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, pack_item);
		if (0 != ret) return ret;
	}

	if (item_amount < pet_cfg.item_cost.val_2) {
		int amount_need_to_buy = pet_cfg.item_cost.val_2 - item_amount;
		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(pet_cfg.item_cost.val_1);
		if (0 == item_cfg) return ERROR_CONFIG_NOT_EXIST;

		int diamont_need = item_cfg->marketPrice * amount_need_to_buy;
		Money_Sub_Info diamond_sub_info(Money_Sub_Type(GOLD_ONLY),
				diamont_need,MONEY_SUB_UPGRADE_WING);
		ret = player_self()->pack_sub_money(diamond_sub_info);
		if (0 != ret) return ret;
	}
	if (item_amount >0) {
		Id_Amount pack_item(pet_cfg.item_cost.val_1, item_amount);
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
				pack_item, Lose_Item_DM_Info(ITEM_LOSE_FASHION_PART_UPGRADE));
	}
	player_self()->pack_sub_money(gold_sub_info);

	pet_detail_->upgrade_val += pet_cfg.upgrade_val.val_1;
	if (pet_detail_->upgrade_val >= pet_cfg.upgrade_val.val_2) {
		pet_detail_->upgrade_val -= pet_cfg.upgrade_val.val_2;
		pet_detail_->lv += 1;
	}

	return 0;
}

int Logic_Pet::handle_active_pet(void) {

	if (0 == pet_detail_) return ERROR_INNER_SERVER;
	if (0 != pet_detail_->lv) return ERROR_INNER_SERVER;

	Id_Amount item_cost(ACTIVE_PET_ITEM_ID, 1);
	int ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, item_cost);
	if (0 != ret) return ret;

	player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost,
			Lose_Item_DM_Info(ITEM_LOSE_ACTIVE_WING));

	pet_detail_->lv = 1;
	pet_detail_->state = Pet_State::PET_ON;

	pet_illusion_active_by_lv(pet_detail_->lv);
	refresh_pet_attr(0, pet_detail_->lv, pet_detail_->illusion_vec);

	MSG_50113004 msg;
	msg.result = 0;

	THIS_SEND_TO_CLIENT(msg);
	handle_get_pet_info();

	pet_detail_->detail_change();
	player_self()->heroer_prop_refresh();

	return 0;
}

int Logic_Pet::handle_pet_illusion(int illusion_id) {

	if (0 == pet_detail_) return ERROR_INNER_SERVER;
	if (0 == pet_detail_->lv) return ERROR_INNER_SERVER;

	if (illusion_id == pet_detail_->active_illusion_id) return ERROR_INNER_SERVER;

	Int_Vec::iterator find_it = std::find(pet_detail_->illusion_vec.begin(),
			pet_detail_->illusion_vec.end(), illusion_id);
	if (find_it == pet_detail_->illusion_vec.end()) return ERROR_INNER_SERVER;

	pet_detail_->active_illusion_id = illusion_id;

	// avatar operate

	MSG_50113005 msg;
	msg.result = 0;
	msg.illusion_id = illusion_id;

	THIS_SEND_TO_CLIENT(msg);

	pet_detail_->detail_change();

	return 0;
}

int Logic_Pet::handle_active_pet_illusion(int illusion_id) {

	if (0 == pet_detail_) return ERROR_INNER_SERVER;
	if (0 != pet_detail_->lv) return ERROR_INNER_SERVER;
	if (illusion_id < 1000) return ERROR_INNER_SERVER;

	Int_Vec::iterator find_it = std::find(pet_detail_->illusion_vec.begin(),
			pet_detail_->illusion_vec.end(), illusion_id);
	if (find_it != pet_detail_->illusion_vec.end()) return ERROR_INNER_SERVER;

	int ret = pet_illusion_active_by_item(illusion_id);
	if (0 != ret) return ret;

	MSG_50113006 msg;
	msg.result = 0;
	msg.active_illusion_id = pet_detail_->active_illusion_id;
	msg.illusion_vec = pet_detail_->illusion_vec;
	THIS_SEND_TO_CLIENT(msg);

	player_self()->heroer_prop_refresh();

	return 0;
}

void Logic_Pet::pet_illusion_active_by_lv(int lv) {

	const Pet_Illusion_Config_Map &illusion_map = CONFIG_CACHE_PET->get_pet_illusion_map();
	for (Pet_Illusion_Config_Map::const_iterator it = illusion_map.begin();
			it != illusion_map.end(); ++it) {

		if (it->second.active_cond != lv) continue;
		Int_Vec::iterator find_it = std::find(pet_detail_->illusion_vec.begin(),
				pet_detail_->illusion_vec.end(), it->second.id);
		if (find_it != pet_detail_->illusion_vec.end()) continue;
		pet_detail_->illusion_vec.push_back(it->second.id);
		pet_detail_->active_illusion_id = it->second.id;
	}
}

int Logic_Pet::pet_illusion_active_by_item(int illusion_id) {

	const Pet_Illusion_Config *illusion_cfg = CONFIG_CACHE_PET->get_pet_illusion(illusion_id);
	if (0 == illusion_cfg) return ERROR_CONFIG_NOT_EXIST;
	Id_Amount item_cost(illusion_cfg->active_cond, 1);
	int ret = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost,
			Lose_Item_DM_Info(ITEM_LOSE_UPGRADE_WING));
	if (0 != ret) {
		MSG_82100008 msg;
		msg.item_ids.push_back(illusion_cfg->active_cond);
		THIS_SEND_TO_CLIENT(msg);
		return ret;
	}

	pet_detail_->illusion_vec.push_back(illusion_id);
	pet_detail_->active_illusion_id = illusion_id;

	return 0;
}

int Logic_Pet::handle_pet_learn_skill_normal(int skill_id) {

	if (0 == pet_detail_ || 0 == pet_detail_->lv) return ERROR_INNER_SERVER;
	const Pet_Config *pet_cfg = CONFIG_CACHE_PET->get_pet_config(pet_detail_->lv);
	if (0 == pet_cfg) return ERROR_CONFIG_NOT_EXIST;
	const Pet_Skill_Config *skill_cfg = CONFIG_CACHE_PET->get_pet_skill_config(skill_id);
	if (0 == skill_cfg) return ERROR_CONFIG_NOT_EXIST;

	for (Int_Int_Map::iterator it = pet_detail_->skill_map.begin(); it != pet_detail_->skill_map.end(); ++it) {

		if (skill_id == it->first) return ERROR_INNER_SERVER;
		const Pet_Skill_Config *old_skill_cfg = CONFIG_CACHE_PET->get_pet_skill_config(it->first);
		if (0 == old_skill_cfg) return ERROR_CONFIG_NOT_EXIST;
		if (old_skill_cfg->category != skill_cfg->category) continue;
		if (old_skill_cfg->type > skill_cfg->type) return ERROR_INNER_SERVER;
	}

	Id_Amount pack_item(skill_cfg->item_id, 1);
	int ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, pack_item);
	if (0 != ret) return ret;

	const int max_grid_num = pet_cfg->grid_num;
	int cur_used_grid = pet_detail_->skill_map.size();

	bool is_replace = true;
	if (max_grid_num > cur_used_grid) {

		const Pet_Learn_Skill_Config * learn_cfg = CONFIG_CACHE_PET->get_pet_learn_skill(cur_used_grid);
		if (0 == learn_cfg) return ERROR_CONFIG_NOT_EXIST;

		int rand = random() % 1000;
		if (rand < learn_cfg->rate) {
			is_replace = false;
		}
	}

	Int_Int_Map changed_skill_map;
	if (is_replace) {
		int ret = pet_learn_skill_replace_old_skill(*skill_cfg, changed_skill_map);
		if (0 != ret) return ret;
	} else {
		pet_learn_skill_add_new_skill(*skill_cfg, cur_used_grid+1, changed_skill_map);
	}

	player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, pack_item,
			Lose_Item_DM_Info(ITEM_LOSE_FASHION_PART_UPGRADE));

	MSG_50113008 msg;
	msg.result = 0;
	Int_Int val;
	for (Int_Int_Map::iterator it = pet_detail_->skill_map.begin(); it != pet_detail_->skill_map.end(); ++it) {

		val.reset();
		val.val_1 = it->first;
		val.val_2 = it->second;
		msg.skill_vec.push_back(val);
	}
	THIS_SEND_TO_CLIENT(msg);

	refresh_pet_skill(changed_skill_map);

	return 0;
}

int Logic_Pet::handle_pet_learn_skill_advance(int skill_id) {

	if (0 == pet_detail_ || 0 == pet_detail_->lv) return ERROR_INNER_SERVER;
	const Pet_Config *pet_cfg = CONFIG_CACHE_PET->get_pet_config(pet_detail_->lv);
	if (0 == pet_cfg) return ERROR_CONFIG_NOT_EXIST;

	for (Int_Int_Map::iterator it = pet_detail_->skill_map.begin(); it != pet_detail_->skill_map.end(); ++it) {

		if (skill_id == it->first) return ERROR_INNER_SERVER;
	}

	const int max_grid_num = pet_cfg->grid_num;
	int cur_used_grid = pet_detail_->skill_map.size();
	if (max_grid_num <= cur_used_grid) return ERROR_INNER_SERVER;

	const Pet_Skill_Config *skill_cfg = CONFIG_CACHE_PET->get_pet_skill_config(skill_id);
	if (0 == skill_cfg) return ERROR_CONFIG_NOT_EXIST;
	const Pet_Learn_Skill_Config *learn_cfg = CONFIG_CACHE_PET->get_pet_learn_skill(cur_used_grid);
	if (0 == learn_cfg) return ERROR_CONFIG_NOT_EXIST;

	Id_Amount pack_item(skill_cfg->item_id, 1);
	int ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, pack_item);
	if (0 != ret) return ret;
	Money_Sub_Info gold_sub_info(Money_Sub_Type(GOLD_ONLY), learn_cfg->money_cost,
			MONEY_SUB_UPGRADE_WING);
	ret = player_self()->pack_sub_money(gold_sub_info);
	if (0 != ret) return ret;

	player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, pack_item,
			Lose_Item_DM_Info(ITEM_LOSE_FASHION_PART_UPGRADE));

	Int_Int_Map changed_skill_map;
	pet_learn_skill_add_new_skill(*skill_cfg, cur_used_grid+1, changed_skill_map);

	MSG_50113009 msg;
	msg.result = 0;
	Int_Int val;
	for (Int_Int_Map::iterator it = pet_detail_->skill_map.begin(); it != pet_detail_->skill_map.end(); ++it) {

		val.reset();
		val.val_1 = it->first;
		val.val_2 = it->second;
		msg.skill_vec.push_back(val);
	}
	THIS_SEND_TO_CLIENT(msg);

	refresh_pet_skill(changed_skill_map);

	return 0;
}

int Logic_Pet::pet_learn_skill_replace_old_skill(const Pet_Skill_Config &cfg,
		Int_Int_Map& changed_skill_map) {

	Int_Vec same_category_vec;
	Int_Vec diff_category_vec;
	for (Int_Int_Map::iterator it = pet_detail_->skill_map.begin(); it != pet_detail_->skill_map.end(); ++it) {

		const Pet_Skill_Config *old_skill_cfg = CONFIG_CACHE_PET->get_pet_skill_config(it->first);
		if (0 == old_skill_cfg) return ERROR_CONFIG_NOT_EXIST;

		if (old_skill_cfg->category == cfg.category) {
			if (old_skill_cfg->type > cfg.type) return ERROR_INNER_SERVER;
			same_category_vec.push_back(old_skill_cfg->skill_id);
		} else {
			diff_category_vec.push_back(old_skill_cfg->skill_id);
		}
	}

	int old_skill_id = 0;
	if (!same_category_vec.empty()) {
		int size = same_category_vec.size();
		int index = random() % size;
		old_skill_id = same_category_vec[index];
	}

	if (0 == old_skill_id && !diff_category_vec.empty()) {
		int size = diff_category_vec.size();
		int index = random() % size;
		old_skill_id = diff_category_vec[index];
	}
	if (0 == old_skill_id) return ERROR_INNER_SERVER;

	Int_Int_Map::iterator find_it = pet_detail_->skill_map.find(old_skill_id);
	if(find_it == pet_detail_->skill_map.end()) return ERROR_INNER_SERVER;
	int old_loc = find_it->second;
	pet_detail_->skill_map.erase(find_it);
	pet_detail_->skill_map[cfg.skill_id] = old_loc;

	changed_skill_map[old_skill_id] = 0;
	changed_skill_map[cfg.skill_id] = old_loc;

	return 0;
}

void Logic_Pet::pet_learn_skill_add_new_skill(const Pet_Skill_Config &cfg, int loc,
		Int_Int_Map& changed_skill_map){

	pet_detail_->skill_map[cfg.skill_id] = loc;
	changed_skill_map[cfg.skill_id] = loc;
}

int Logic_Pet::handle_change_pet_state(int new_state) {

	if (0 == pet_detail_ || 0 == pet_detail_->lv) return ERROR_INNER_SERVER;
	if (new_state == pet_detail_->state) return ERROR_INNER_SERVER;

	pet_detail_->state = new_state;

	// change followed state
	MSG_50113010 msg;
	msg.result = 0;
	msg.new_state = new_state;

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

void Logic_Pet::refresh_pet_attr(int pre_lv, int pos_lv, Int_Vec &illusion_vec) {

	typedef boost::unordered_map<int, Prop_Setter> Prop_Setter_Map;
	if (0 == pet_detail_ || 0 == pet_detail_->lv) return;
	const Pet_Config *pre_cfg = CONFIG_CACHE_PET->get_pet_config(pre_lv);
	const Pet_Config *pos_cfg = CONFIG_CACHE_PET->get_pet_config(pos_lv);

	Prop_Setter_Map prop_map;
	Prop_Setter_Vec vec;
	Prop_Setter setter;
	if (0 != pre_cfg) {
		for (Pet_Prop_Value_Map::const_iterator it = pre_cfg->role_prop_map.begin();
				it != pre_cfg->role_prop_map.end(); ++it) {
			setter.reset();
			setter.addi_type = Addition_Type::AT_PET;
			setter.operate = O_ADD;
			setter.prop_type = Property_Type(it->first);
			setter.basic = 0.0;
			setter.fixed = -it->second.fix;
			setter.percent = -it->second.percent;
			prop_map[it->first] = setter;
		}
		std::vector<Skill_Info> skill_vec;
		Skill_Info skill;
		for (Int_Vec::const_iterator it = pre_cfg->skill_vec.begin();
				it != pre_cfg->skill_vec.end(); ++it) {
			skill.skill_id = *it;
			skill.skill_lv = 1;
			skill_vec.push_back(skill);
		}
		if (!skill_vec.empty()) {
			player_self()->remove_be_skills(skill_vec);
		}
	}

	if (0 != pos_cfg) {
		std::vector<Skill_Info> skill_vec;
		Skill_Info skill;
		for (Int_Vec::const_iterator it = pos_cfg->skill_vec.begin();
				it != pos_cfg->skill_vec.end(); ++it) {
			skill.reset();
			skill.skill_id = *it;
			skill.skill_lv = 1;
			skill_vec.push_back(skill);
		}

		if (!skill_vec.empty()) {
			player_self()->add_be_skills(skill_vec);
		}

		for (Pet_Prop_Value_Map::const_iterator it = pos_cfg->role_prop_map.begin();
				it != pos_cfg->role_prop_map.end(); ++it) {
			Prop_Setter_Map::iterator find_it = prop_map.find(it->first);
			if (find_it != prop_map.end()) {
				find_it->second.fixed += it->second.fix;
				find_it->second.percent += it->second.percent;
			} else {
				setter.reset();
				setter.addi_type = Addition_Type::AT_PET;
				setter.operate = O_ADD;
				setter.prop_type = Property_Type(it->first);
				setter.basic = 0.0;
				setter.fixed = it->second.fix;
				setter.percent = it->second.percent;
				prop_map[it->first] = setter;
			}
		}
	}

	for (Int_Vec::iterator it = illusion_vec.begin(); it != illusion_vec.end(); ++it) {

		const Pet_Illusion_Config *illusion_cfg = CONFIG_CACHE_PET->get_pet_illusion(*it);
		if (0 == illusion_cfg) continue;
		for (Int_Int_Map::const_iterator iter = illusion_cfg->attr_map.begin();
				iter != illusion_cfg->attr_map.end(); ++iter) {

			Prop_Setter_Map::iterator find_it = prop_map.find(iter->first);
			if (find_it != prop_map.end()) {
				find_it->second.fixed += iter->second;
			} else {

				setter.reset();
				setter.addi_type = Addition_Type::AT_PET;
				setter.operate = O_ADD;
				setter.prop_type = Property_Type(iter->first);
				setter.basic = 0.0;
				setter.fixed = iter->second;
				prop_map[iter->first] = setter;
			}
		}
	}

	for (Prop_Setter_Map::iterator it = prop_map.begin(); it != prop_map.end(); ++it) {
		vec.push_back(it->second);
	}
	if (0 != pre_cfg || 0 != pos_cfg) {
		player_self()->fight_prop_modify(vec, 327);
	}
}

void Logic_Pet::refresh_pet_skill(Int_Int_Map &skill_map) {

	Skill_Info skill;
	std::vector<Skill_Info> skill_add_vec;
	std::vector<Skill_Info> skill_sub_vec;
	for (Int_Int_Map::iterator it = skill_map.begin();
			it != skill_map.end(); ++it) {

		skill.reset();
		skill.skill_id = it->first;
		skill.skill_lv = 1;
		if (it->second == 0) { // del extra be skill
			skill_sub_vec.push_back(skill);
		} else { // add extra be skill
			skill_add_vec.push_back(skill);
		}
	}

	if (!skill_sub_vec.empty()) {
		player_self()->remove_be_skills(skill_sub_vec);
	}
	if (!skill_add_vec.empty()) {
		player_self()->add_be_skills(skill_add_vec);
	}
}

void Logic_Pet::refresh_pet_attr_for_hero(Prop_Setter_Vec &prop_vec, Int_Vec &skill_vec) {

	typedef boost::unordered_map<int, Prop_Setter> Prop_Setter_Map;
	if (0 == pet_detail_ || 0 == pet_detail_->lv) return;
	const Pet_Config *cfg = CONFIG_CACHE_PET->get_pet_config(pet_detail_->lv);
	if (0 == cfg) return;
	Prop_Setter_Map prop_map;
	Prop_Setter setter;
	for (Pet_Prop_Value_Map::const_iterator it = cfg->hero_prop_map.begin();
			it != cfg->hero_prop_map.end(); ++it) {

		setter.reset();
		setter.addi_type = Addition_Type::AT_PET;
		setter.operate = O_SET;
		setter.prop_type = Property_Type(it->first);
		setter.fixed = it->second.fix;
		setter.percent = it->second.percent;
		prop_map[it->first] = setter;
	}

	for (Int_Vec::iterator it = pet_detail_->illusion_vec.begin();
			it != pet_detail_->illusion_vec.end(); ++it) {

		const Pet_Illusion_Config *illusion_cfg = CONFIG_CACHE_PET->get_pet_illusion(*it);
		if (0 == illusion_cfg) continue;

		for (Int_Int_Map::const_iterator iter = illusion_cfg->attr_map.begin();
				iter != illusion_cfg->attr_map.end(); ++iter) {

			Prop_Setter_Map::iterator find_it = prop_map.find(iter->first);
			if (find_it == prop_map.end()) {
				setter.reset();
				setter.addi_type = Addition_Type::AT_PET;
				setter.operate = O_SET;
				setter.prop_type = Property_Type(iter->first);
				setter.basic = 0.0;
				setter.fixed = iter->second;
				setter.percent = 0.0;
				prop_map[iter->first] = setter;
			} else {
				find_it->second.fixed += iter->second;
			}
		}
	}

	for (Int_Int_Map::iterator it = pet_detail_->skill_map.begin();
			it != pet_detail_->skill_map.end(); ++it) {

		const Skill_Config *skill_cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->first);
		if (0 == skill_cfg) continue;
		if (skill_cfg->skill_affiliation != Skill_Aff_Type::TYPE_HERO) continue;
		skill_vec.push_back(it->first);
	}

	if (prop_map.empty()) return;
	for (Prop_Setter_Map::iterator it = prop_map.begin();
			it != prop_map.end(); ++it) {
		prop_vec.push_back(it->second);
	}
}
