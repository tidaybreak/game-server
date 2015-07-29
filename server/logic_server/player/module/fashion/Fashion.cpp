/*
 * Fashion.cpp
 *
 *  Created on: 2014年9月3日
 *      Author: root
 */

#include "Logic_Player.h"
#include "Fashion.h"
#include "Fashion_Def.h"
#include "Fashion_Struct.h"
#include "Fashion_Config.h"
#include "Msg_Fashion_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Game_Typedef.h"
#include "Config_Cache_Item.h"
#include "Err_Code.h"
#include "Config_Cache_Equip.h"
#include "equiper/Equiper.h"
#include "Config_Cache_Fashion.h"
#include "Fashion_Config.h"
#include "fighter/Fighter_Def.h"
#include "Logic_Monitor.h"
#include "Logic_Player_Struct.h"
#include "Msg_Active_Struct.h"
#include "pack/Pack_Struct.h"
#include "pack/Packer.h"
#include "mail/Mail_Config.h"
#include "Config_Cache_Mail.h"
#include "Logic_Public.h"

Fashion::~Fashion(void) {}

void Fashion::reset(void) {
	fashion_detail_ = 0;
}

int Fashion::create_init(Fashion_Detail &detail) {

	Fashion_Part part;
	for (int i=Fashion_Part_Type::PART_BEGIN; i<=Fashion_Part_Type::PART_END;++i) {
		part.reset();
		part.part_id_ = i;
		detail.fashion_part_map_[i] = part;
	}

	return 0;
}

void Fashion::load_detail(Fashion_Detail &detail) {
	fashion_detail_ = &detail;
}

void Fashion::save_detail(Fashion_Detail &detail) {
	if (fashion_detail_->is_change) {
		detail = fashion_detail();
		fashion_detail_->is_change = false;
	}
}

void Fashion::sign_int(void) {

}

void Fashion::sign_out(void) {

}

void Fashion::module_init(void) {
	if (0 == fashion_detail_) {
		MSG_USER("fashion data maybe not load properly, role_id=%ld", role_id());
	}

	if (fashion_detail_->fashion_part_map_.empty()) {
		create_init(*fashion_detail_);
	}

	for (Fashion_Part_Map::iterator it = fashion_detail_->fashion_part_map_.begin();
			it != fashion_detail_->fashion_part_map_.end(); ++it) {
		for (Int_Vec::iterator id_it = it->second.fashion_id_vec_.begin();
				id_it != it->second.fashion_id_vec_.end(); ++id_it) {
			const Fashion_Base_Cfg *cfg = CONFIG_CACHE_FASHION->get_fashion_base_cfg(*id_it);
			if (0 == cfg) {
				MSG_USER("fashion base cfg error, id=%d", *id_it);
				continue;
			}
			fashion_detail_->exp_ += cfg->exp;
		}
	}

	fashion_soul_upgrade();
}

int Fashion::handle_get_fashion_info(void) {
	if (0 == fashion_detail_) {
		MSG_USER("user has no fashion detail, role_id=%ld", role_id());
		return ERROR_INNER_SERVER;
	}

	MSG_50105020 msg;
	msg.lv = fashion_detail_->lv_;
	msg.exp = fashion_detail_->exp_;
	msg.is_show = fashion_detail_->is_show_;
	Fashion_Part_Info part_info;
	for (Fashion_Part_Map::iterator it = fashion_detail_->fashion_part_map_.begin();
			it != fashion_detail_->fashion_part_map_.end(); ++it) {
		part_info.reset();
		part_info.part_id = it->second.part_id_;
		part_info.lv = it->second.lv_;
		part_info.exp = it->second.exp_;
		part_info.used_fashion_id = it->second.used_fashion_id_;
		part_info.fashion_id_vec.insert(part_info.fashion_id_vec.end(),
				it->second.fashion_id_vec_.begin(), it->second.fashion_id_vec_.end());
		msg.fashion_part_vec.push_back(part_info);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Fashion::handle_use_fashion_item(UInt_Vec item_idx_vec, int to_wear) {

	if (0 == fashion_detail_) {
		MSG_USER("player has no fashion detail, role_id=%ld", role_id());
		return ERROR_INNER_SERVER;
	}

	MSG_50105021 msg;
	int err_code = ERROR_USE_FASHION_ERROR;
	Int_Int_Map suit_map;
	for (UInt_Vec::const_iterator it = item_idx_vec.begin(); it != item_idx_vec.end(); ++it) {
		int id = handle_use_one_fashion_item(*it, to_wear, suit_map);
		if (0 == id) continue;
		err_code = 0;
		msg.fashion_id_vec.push_back(id);
	}

	if (0 != err_code) return err_code;

	fashion_soul_upgrade();
	player_self()->equiper_prop_refresh(10021);
	if (0 != to_wear)
		player_self()->refresh_avatar();


	handle_get_fashion_info();

	fashion_detail_->detail_change();

	msg.wear = to_wear;
	msg.result = 0;
	msg.soul_lv = fashion_detail_->lv_;
	msg.exp = fashion_detail_->exp_;
	THIS_SEND_TO_CLIENT(msg);

	handle_get_new_suit(suit_map);

	return 0;
}

int Fashion::handle_use_one_fashion_item(uint32_t index, int to_wear, Int_Int_Map &suit_map) {

	uint32_t fashion_id = 0;
	int ret= player_self()->pack_get_item_id(index, fashion_id);
	if (0 != ret) return 0;
	if (0 == fashion_id) return 0;

	const Fashion_Base_Cfg* base_cfg = CONFIG_CACHE_FASHION->get_fashion_base_cfg(fashion_id);
	if (0 == base_cfg) {
		MSG_USER("fashion base cfg error, id=%d", fashion_id);
		return 0;
	}

	Fashion_Part_Map::iterator find_it = fashion_detail_->fashion_part_map_.find(base_cfg->part);
	if (fashion_detail_->fashion_part_map_.end() == find_it) {
		MSG_USER("no this part data, role_id=%ld, part_id=%d", role_id(), base_cfg->part);
		return 0;
	}
	// check has been used this fashion
	Int_Vec::iterator id_it = std::find(find_it->second.fashion_id_vec_.begin(),
			find_it->second.fashion_id_vec_.end(), base_cfg->id);
	if (find_it->second.fashion_id_vec_.end() != id_it)
		return 0;

	Index_Amount index_amount;
	index_amount.index = index;
	index_amount.amount = 1;
	ret = player_self()->pack_erase_item(
			index_amount, Lose_Item_DM_Info(ITEM_LOSE_USE_FASHION_ITEM));
	if (0 != ret) return 0;

	find_it->second.fashion_id_vec_.push_back(base_cfg->id);
	fashion_detail_->exp_ += base_cfg->exp;
	suit_map[base_cfg->suit_id] += 1;

	if (0 == to_wear) return base_cfg->id;

	find_it->second.used_fashion_id_ = base_cfg->id;
	return base_cfg->id;
}

void Fashion::handle_get_new_suit(Int_Int_Map &suit_map) {

	Int_Int_Map temp_suit_map;
	for (Fashion_Part_Map::const_iterator it = fashion_detail_->fashion_part_map_.begin();
			it != fashion_detail_->fashion_part_map_.end(); ++it) {

		for (Int_Vec::const_iterator id_it = it->second.fashion_id_vec_.begin();
				id_it != it->second.fashion_id_vec_.end(); ++id_it) {

			const Fashion_Base_Cfg *cfg = CONFIG_CACHE_FASHION->get_fashion_base_cfg(*id_it);
			if (0 == cfg) continue;
			temp_suit_map[cfg->suit_id] += 1;
		}
	}

	for (Int_Int_Map::const_iterator it = temp_suit_map.begin();
			it != temp_suit_map.end(); ++it) {

		const Suit_ID_Cfg *cfg = CONFIG_CACHE_EQUIP->find_suit(it->first);
		if (0 == cfg) continue;
		if (it->second < cfg->max_cnt_put_on) continue;
		Int_Int_Map::const_iterator find_it = suit_map.find(it->first);
		if (find_it == suit_map.end()) continue;

		// 获取一套新的时装
		const Mail_Base_Config *mail_cfg = CONFIG_CACHE_MAIL->find_config(3012);
		if (0 == mail_cfg) continue;

		const int buf_size = mail_cfg->mail_content.size() + cfg->name.size();
		char buff[buf_size];
		std::string content;
		memset(buff, 0, buf_size);
		snprintf(buff, buf_size, mail_cfg->mail_content.c_str(), cfg->name.c_str());
		content.assign(buff);

		LOGIC_PUBLIC->send_mail(3012, role_id(), mail_cfg->mail_title, content);
	}
}

int Fashion::handle_wear_fashion(Int_Vec fashion_id_vec) {

	if (0 == fashion_detail_) return ERROR_INNER_SERVER;
	Int_Int_Map part_id_map;
	for (Int_Vec::iterator it = fashion_id_vec.begin(); it != fashion_id_vec.end(); ++it) {

		const Fashion_Base_Cfg *base_cfg = CONFIG_CACHE_FASHION->get_fashion_base_cfg(*it);
		if (0 == base_cfg) continue;
		Fashion_Part_Map::iterator find_it = fashion_detail_->fashion_part_map_.find(base_cfg->part);
		if (fashion_detail_->fashion_part_map_.end() == find_it) continue;
		Int_Vec::iterator id_it = std::find(find_it->second.fashion_id_vec_.begin(),
				find_it->second.fashion_id_vec_.end(), *it);
		if (id_it == find_it->second.fashion_id_vec_.end()) continue;
		find_it->second.used_fashion_id_ = *id_it;
		part_id_map[find_it->second.part_id_] = 0;
	}

	for (Fashion_Part_Map::iterator it = fashion_detail_->fashion_part_map_.begin();
			it != fashion_detail_->fashion_part_map_.end(); ++it) {
		Int_Int_Map::iterator find_it = part_id_map.find(it->second.part_id_);
		if (part_id_map.end() != find_it) continue;
		it->second.used_fashion_id_ = 0;
	}

	player_self()->equiper_prop_refresh(10022);
	player_self()->refresh_avatar();

	handle_get_fashion_info();

	fashion_detail_->detail_change();

	MSG_50105002 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Fashion::handle_fashion_part_upgrade(int part, int auto_buy) {

	if (0 == fashion_detail_) return ERROR_INNER_SERVER;
	Fashion_Part_Map::iterator find_it = fashion_detail_->fashion_part_map_.find(part);
	if (fashion_detail_->fashion_part_map_.end() == find_it) return ERROR_INNER_SERVER;

	const int max_lv = CONFIG_CACHE_FASHION->get_fashion_reinforced_max_lv(part);
	if (max_lv <= find_it->second.lv_) return ERROR_INNER_SERVER;

	const Fashion_Reinforced_Cfg *reinforced_cfg =
			CONFIG_CACHE_FASHION->get_fashion_reinforced_cfg(find_it->second.lv_, part);
	if (0 == reinforced_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Money_Sub_Info money_sub_info(
			Money_Sub_Type(COPPER_ONLY), reinforced_cfg->coin_cost,
			MONEY_SUB_FASHION_PART_UPGRADE);
	int ret = player_self()->pack_try_sub_money(money_sub_info);
	if (0 != ret) return ret;

	int item_amount = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX,
			reinforced_cfg->item_cost.val_1);
	if (item_amount < reinforced_cfg->item_cost.val_2 && 0 == auto_buy) {
		MSG_82100008 msg;
		msg.item_ids.push_back(reinforced_cfg->item_cost.val_1);
		THIS_SEND_TO_CLIENT(msg);
		return ERROR_ITEM_NOT_ENOUGH;
	}

	item_amount = item_amount > reinforced_cfg->item_cost.val_2 ?
			reinforced_cfg->item_cost.val_2 : item_amount;

	// 自动购买
	if (0 != item_amount) {
		Id_Amount pack_item(reinforced_cfg->item_cost.val_1, item_amount);
		ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, pack_item);
		if (0 != ret) return ret;
	}
	if (item_amount < reinforced_cfg->item_cost.val_2) {
		int amount_need_to_buy = reinforced_cfg->item_cost.val_2 - item_amount;
		ret = player_self()->req_fast_buy_item_in_mall_tabs(
				reinforced_cfg->item_cost.val_1, amount_need_to_buy , 1);
		if (0 != ret) return ret;
	}
	if (item_amount > 0) {
		Id_Amount pack_item(reinforced_cfg->item_cost.val_1, item_amount);
		player_self()->pack_erase_item(
				PACK_T_PACKAGE_INDEX, pack_item,
				Lose_Item_DM_Info(ITEM_LOSE_FASHION_PART_UPGRADE));
	}
	player_self()->pack_sub_money(money_sub_info);

	MSG_50105022 msg;
	int rand_val = random() % 1000;
	if (rand_val < reinforced_cfg->suc_rate) { // success
		find_it->second.lv_++;
		find_it->second.exp_ = 0;
		msg.result = 0;
	} else { // failed
		find_it->second.exp_ += reinforced_cfg->exp_add;
		msg.result = 1;
		if (find_it->second.exp_ >= reinforced_cfg->exp_max) {
			find_it->second.lv_++;
			find_it->second.exp_ = 0;
			msg.result = 0;
		}
	}

	msg.part_id = part;
	msg.lv = find_it->second.lv_;
	msg.exp = find_it->second.exp_;
	THIS_SEND_TO_CLIENT(msg);

	player_self()->equiper_prop_refresh(10023);

	return 0;
}

void Fashion::fashion_soul_upgrade(void) {

	int max_lv = CONFIG_CACHE_FASHION->get_fashion_soul_max_lv();
	for (int i=fashion_detail_->lv_; i<max_lv; ++i) {
		const Fashion_Soul_Cfg *cfg = CONFIG_CACHE_FASHION->get_fashion_soul_cfg(i);
		if (0 == cfg)  {
			MSG_USER("fashion soul cfg error, lv=%d", i);
			continue;
		}
		if (fashion_detail_->exp_ < cfg->exp) break;
		fashion_detail_->exp_ -= cfg->exp;
		fashion_detail_->lv_ = i + 1;
	}
}

void Fashion::refresh_fashion_prop_total(std::map<int, double> &prop_val_map) {
	Int_Int_Map suit_map;
	refresh_fashion_prop(prop_val_map, suit_map);
	refresh_fashion_suit_prop(suit_map, prop_val_map);
}

void Fashion::refresh_fashion_prop(std::map<int, double> &prop_val_map, Int_Int_Map &suit_id_map) {
	if (0 == fashion_detail_) {
		MSG_USER("user has no fashion detail, role_id=%ld", role_id());
		return;
	}

	Int_Double_Map temp_prop_map;
	const Fashion_Soul_Cfg* soul_cfg = CONFIG_CACHE_FASHION->get_fashion_soul_cfg(
			fashion_detail_->lv_);
	if (0 == soul_cfg) {
		MSG_USER("config error , no fashion soul config, lv==%d", fashion_detail_->lv_);
		return;
	}

	for (Int_Double_Map::const_iterator prop_it = soul_cfg->props_map.begin();
			prop_it != soul_cfg->props_map.end(); ++prop_it) {
		temp_prop_map[prop_it->first] += prop_it->second;
	}

//	int part_reinforced_rate = 0;
	for (Fashion_Part_Map::const_iterator it = fashion_detail_->fashion_part_map_.begin();
			it != fashion_detail_->fashion_part_map_.end(); ++it) {

		for (Int_Vec::const_iterator id_it = it->second.fashion_id_vec_.begin();
				id_it != it->second.fashion_id_vec_.end(); ++id_it) {

			const Fashion_Base_Cfg* base_cfg = CONFIG_CACHE_FASHION->get_fashion_base_cfg(*id_it);
			if (0 == base_cfg) {
				MSG_USER("config error, no fashion base config, id =%d", *id_it);
				continue;
			}

			suit_id_map[base_cfg->suit_id] += 1;

			for (Int_Double_Map::const_iterator prop_it = base_cfg->hidden_props_map.begin();
					prop_it != base_cfg->hidden_props_map.end(); ++prop_it) {
				temp_prop_map[prop_it->first] += prop_it->second;
			}

			if (it->second.used_fashion_id_ != *id_it) continue;
			for (Int_Double_Map::const_iterator prop_it = base_cfg->wore_props_map.begin();
					prop_it != base_cfg->wore_props_map.end(); ++prop_it) {
				prop_val_map[prop_it->first] += prop_it->second;
			}
		}

		const Fashion_Reinforced_Cfg* reinforced_cfg =
				CONFIG_CACHE_FASHION->get_fashion_reinforced_cfg(it->second.lv_, it->second.part_id_);
		if (0 == reinforced_cfg) {
			MSG_USER("config error, no fashion reinforced config, lv=%d, part=%d",
					it->second.lv_, it->second.part_id_);
			continue;
		}

		if (0 == it->second.used_fashion_id_) continue;
//		part_reinforced_rate += reinforced_cfg->percent;
		for (Int_Double_Map::const_iterator prop_it = reinforced_cfg->wore_props_map.begin();
				prop_it != reinforced_cfg->wore_props_map.end(); ++prop_it) {
			prop_val_map[prop_it->first] += prop_it->second;
		}
	}

	// base
	for (Int_Double_Map::iterator it = temp_prop_map.begin(); it != temp_prop_map.end(); ++it) {
		int part_id = 0;
		if (it->first == PT_POWER) part_id = PART_WEAPON;
		else if (it->first == PT_STAMINA) part_id = PART_CLOTH;
		else if (it->first == PT_ARMOR) part_id = PART_HAT;

		if (0 == part_id) {
			prop_val_map[it->first] += floor(it->second);
			continue;
		}

		Fashion_Part_Map::const_iterator find_part_it =
				fashion_detail_->fashion_part_map_.find(part_id);
		if (fashion_detail_->fashion_part_map_.end() == find_part_it) {
			prop_val_map[it->first] += floor(it->second);
			continue;
		}

		const Fashion_Reinforced_Cfg* reinforced_cfg =
				CONFIG_CACHE_FASHION->get_fashion_reinforced_cfg(
						find_part_it->second.lv_, find_part_it->second.part_id_);
		if (0 == reinforced_cfg) {
			prop_val_map[it->first] += floor(it->second);
			continue;
		}

		double val = floor(it->second * (1000 + reinforced_cfg->percent) / 1000.0);
		prop_val_map[it->first] += val;
	}
}

void Fashion::refresh_fashion_suit_prop(Int_Int_Map &suit_id_map, std::map<int, double> &prop_val_map) {
	if (0 == fashion_detail_) {
		MSG_USER("user has no fashion detail, role_id=%ld", role_id());
		return;
	}
	if (suit_id_map.empty()) return;

	for (Int_Int_Map::iterator it = suit_id_map.begin(); it != suit_id_map.end(); ++it) {
		const Suit_ID_Cfg *cfg = CONFIG_CACHE_EQUIP->find_suit(it->first);
		if (0 == cfg) continue;
		for (Suit_Count_Cfg_Map::const_iterator cfg_it = cfg->count_cfg_map_.begin();
				cfg_it != cfg->count_cfg_map_.end(); ++cfg_it) {
			if (it->second < cfg_it->first) continue;
			for (Int_Double_Map::const_iterator p_it = cfg_it->second.prop_val_map.begin();
					p_it != cfg_it->second.prop_val_map.end(); ++p_it) {
				prop_val_map[p_it->first] += p_it->second;
			}
		}
	}
}

void Fashion::refresh_fashion_avatar(Int_Int_Map &avatar_map) {
	if (0 == fashion_detail_) return;

	if (Fashion_Detail::Fashion_Show_State::SHOW_OFF == fashion_detail_->is_show_) return;

	if (!fashion_detail_->get_show_state()) return;
	int career = player_self()->career();
	int gender = player_self()->gender();
	for (Fashion_Part_Map::const_iterator it = fashion_detail_->fashion_part_map_.begin();
			it != fashion_detail_->fashion_part_map_.end(); ++it) {
		if (0 == it->second.used_fashion_id_) continue;
		const Fashion_Base_Cfg* base_cfg = CONFIG_CACHE_FASHION->get_fashion_base_cfg(
				it->second.used_fashion_id_);
		if (0 == base_cfg) continue;
		if (it->second.part_id_ != base_cfg->part) {
			MSG_USER("used_fashion error, part_id is inconsistent, part_id=%d, cfg_part_id=%d",
					it->second.part_id_, base_cfg->part);
			continue;
		}

		Fashion_Avatar_Map::const_iterator find_it = base_cfg->fashion_avatar_map.find(career);
		if (find_it == base_cfg->fashion_avatar_map.end()) continue;
		Int_Int_Map::const_iterator avatar_it = find_it->second.find(gender);
		if (avatar_it == find_it->second.end()) continue;
		if (avatar_it->second < 0) continue;
		if (it->second.part_id_ <= 10) continue;
		avatar_map[it->second.part_id_-10] = avatar_it->second;
	}
}

int Fashion::handle_change_fashion_show_state(int state) {

	if (Fashion_Detail::SHOW_OFF != state && Fashion_Detail::SHOW_ON != state) return ERROR_INNER_SERVER;
	if (0 == fashion_detail_) return ERROR_INNER_SERVER;
	if (state == fashion_detail_->is_show_) return ERROR_SAME_WEAR_STATE;

	fashion_detail_->is_show_ = state;
	player_self()->refresh_avatar();

	MSG_50105007 msg;
	msg.result = 0;
	msg.state = state;

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}
