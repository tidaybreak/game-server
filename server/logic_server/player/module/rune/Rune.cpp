/*
 * Rune.cpp
 *
 *  Created on: May 15, 2014
 *      Author: chenyulin
 */

#include "pack/Packer.h"
#include "Rune.h"
#include "Rune_Struct.h"
#include "Rune_Def.h"
#include "Rune_Func.h"
#include "equiper/Equiper_Func.h"
#include "Logic_Module_Head.h"
#include "Msg_Active_Struct.h"
#include "Msg_Rune_Struct.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Rune.h"
#include "Config_Cache.h"
#include "Logic_Player.h"
#include "Err_Code.h"
#include "pack/Packer.h"
#include "equiper/Equiper_Def.h"
#include "heroer/Logic_Heroer_Struct.h"

Rune::Rune(void) {
	reset();
}

Rune::~Rune() {

}

void Rune::reset(void) {
	rune_detail_ = 0;
}

int Rune::create_init(Rune_Detail &detail) {

	return 0;
}

void Rune::load_detail(Rune_Detail &detail) {
	rune_detail_ = &detail;
}

void Rune::save_detail(Rune_Detail &detail) {
	if (rune_detail().is_change) {
		detail = rune_detail();
		rune_detail_->is_change = false;
	}
}

void Rune::sign_in(void) {

}

void Rune::sign_out(void) {

}

void Rune::module_init(void) {
	init_new_part();
	rune_prop_refresh();
}

void Rune::init_new_part(void) {
	Rune_Part_Detail part_detail;
	Rune_Hole_Detail hole_detail;

	// player
	for (uint32_t rune_part = PACK_T_EQUIP_INDEX + EQUIP_BEGIN;
			rune_part < PACK_T_EQUIP_INDEX + EQUIP_END; ++rune_part) {
		if (! rune_detail_->part_detail_map.count(rune_part)) {
			part_detail.reset();
			part_detail.part = rune_part;

			for (uint8_t hole_index = 1; hole_index < 4; ++hole_index) {
				hole_detail.reset();
				hole_detail.index = hole_index;
				part_detail.hole_detail_map.insert(std::make_pair(hole_detail.index, hole_detail));
			}

			rune_detail_->part_detail_map.insert(std::make_pair(part_detail.part, part_detail));
		}
	}

	// hero
	for (Heroer_Detail::Hero_Map::const_iterator it_hero = player_self()->heroer_detail().hero_map.begin();
			it_hero != player_self()->heroer_detail().hero_map.end(); ++it_hero) {

		uint32_t hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + it_hero->second.hero_index * ONE_HERO_EQUIP_GAP;
		for (uint32_t rune_part = hero_pack_idx_gap + EQUIP_BEGIN;
				rune_part < hero_pack_idx_gap + EQUIP_END; ++rune_part) {
			if (! rune_detail_->part_detail_map.count(rune_part)) {
				part_detail.reset();
				part_detail.part = rune_part;

				for (uint8_t hole_index = 1; hole_index < 4; ++hole_index) {
					hole_detail.reset();
					hole_detail.index = hole_index;
					part_detail.hole_detail_map.insert(std::make_pair(hole_detail.index, hole_detail));
				}

				rune_detail_->part_detail_map.insert(std::make_pair(part_detail.part, part_detail));
			}
		}

		break;	// 改版：英雄共用符文，只保留首个英雄符文作为共用符文
	}
}

int Rune::client_ready_now(void) {

	return 0;
}

int Rune::req_fetch_rune_info(void) {
//	MSG_50103000 res_msg;
//	Rune_Part_Info part_info;
//	for (Rune_Part_Detail_Map::iterator it_part = rune_detail_->part_detail_map.begin();
//			it_part != rune_detail_->part_detail_map.end(); ++it_part) {
//		part_info.reset();
//		rune_detail_->get_rune_part_info(it_part->second.part, part_info);
//		res_msg.part_infos.push_back(part_info);
//	}
//	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Rune::req_rune_activate(const int part, const int hole_index, const int8_t auto_buy) {
	if (!part || !hole_index) {
		return ERROR_CLIENT_PARAM;
	}
	if (check_equip_idx_by_part(part, EP_MEDAL)) {
		return ERROR_PERM_LIMIT;
	}

	// Check hole exist
	Rune_Part_Detail_Map::iterator it_part = rune_detail_->part_detail_map.find(part);
	if (it_part == rune_detail_->part_detail_map.end()) {
		return ERROR_RUNE_PART_NOT_EXIST;
	}
	Rune_Hole_Detail_Map::iterator it_hole = it_part->second.hole_detail_map.find(hole_index);
	if (it_hole == it_part->second.hole_detail_map.end()) {
		return ERROR_RUNE_HOLE_NOT_EXIST;
	}

	// Check hole order
	int min_index = it_part->second.hole_detail_map.begin()->second.index;	// 默认第一孔
	int min_level = it_part->second.hole_detail_map.begin()->second.level;	// 默认第一孔
	for (Rune_Hole_Detail_Map::const_iterator it_check_hole = it_part->second.hole_detail_map.begin();
			it_check_hole != it_part->second.hole_detail_map.end(); ++it_check_hole) {
		// 找比第一孔等级低的
		if (it_check_hole->second.level < min_level) {
			min_index = it_check_hole->second.index;
			// 取找到的第一个
			break;
		}
	}
	if (hole_index != min_index) {
		return ERROR_RUNE_HOLE_ORDER;
	}

	// Check hole_lvl
	if (it_hole->second.level) {
		return ERROR_RUNE_HOLE_HAS_ACTIVATE;
	} else {
		// GM command
		it_hole->second.energy = 0;
	}

	// 激活 = 0 to 1 充能的过程
	//Bat_Consume_Info bat_info;
	int old_level = it_hole->second.level;
	int res_fill = rune_fill_energy(it_hole->second, 0, auto_buy);
	if (res_fill) {
		return res_fill;
	}

//	const Rune_Hole_Lvl_Cfg *hole_lvl_cfg = CONFIG_CACHE_RUNE->find_hole_lvl_cfg(it_hole->second.level + 1);
//	if (! hole_lvl_cfg) {
//		return ERROR_CONFIG_NOT_EXIST;
//	}
//	const Item_Detail_Config *item_material_cfg = CONFIG_CACHE_ITEM->find_item(hole_lvl_cfg->material_id);
//	if (! item_material_cfg) {
//		return ERROR_CONFIG_NOT_EXIST;
//	}
//
//	// Check copper
//	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY),
//			hole_lvl_cfg->cost_copper, MONEY_SUB_EQUIP_INCREASE));
//	if (hole_lvl_cfg->cost_copper > 0) {
//		int try_res = player_self()->pack_try_sub_money(money_info);
//		if (try_res) {
//			return try_res;
//		}
//	}
//
//	// Check material
//	Id_Amount id_amount(hole_lvl_cfg->material_id, hole_lvl_cfg->material_num_once);
//	if (id_amount.id > 0 && id_amount.amount > 0) {
//		int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, id_amount);
//		if (try_res) {
//			return try_res;
//		}
//	}
//
//	/*
//	 * Sub begin
//	 */
//	// Sub money
//	if (money_info.nums > 0) {
//		player_self()->pack_sub_money(money_info);
//	}
//	// Sub material
//	if (id_amount.id > 0 && id_amount.amount > 0) {
//		 player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, id_amount, Lose_Item_DM_Info(ITEM_LOSE_TEST));
//	}
//
//	bool is_activate = false;
//	if (item_material_cfg->special_cfg.rune_essence_energy > 0) {
//		//it_hole->second.energy += item_material_cfg->special_cfg.rune_essence_energy;
//
//		int remain_energy = it_hole->second.energy + item_material_cfg->special_cfg.rune_essence_energy;
//		int hole_lvl = it_hole->second.level;
//		int old_hole_lvl = it_hole->second.level;
//		const Int_Vec *p_cfg_energy_vec = CONFIG_CACHE_RUNE->hole_lvl_energy_vec();
//		if (!p_cfg_energy_vec || p_cfg_energy_vec->empty()) {
//			return ERROR_CONFIG_NOT_EXIST;
//		}
//		// 能量(经验)转化为等级
//		exp_to_lvl(remain_energy, hole_lvl, MAX_RUNE_HOLE_LVL, *p_cfg_energy_vec);
//
//		it_hole->second.level = hole_lvl;
//		it_hole->second.energy = remain_energy;
//
//		is_activate = it_hole->second.level > old_hole_lvl ? true : false;
//	}
//
	bool is_activate = old_level < it_hole->second.level ? true : false;

	active_rune_hole_info(part, hole_index, is_activate);

	rune_detail_->detail_change();

	if (is_activate) {
		if (is_hero_rune(it_part->first)) {
			player_self()->heroer_prop_refresh(player_self()->heroer_detail().fighter_index);
		} else {
			rune_prop_refresh();
		}
		// 装备分数
		Item_Detail *equip = this->player_self()->pack_get_item(part);
		if (equip) {
			this->player_self()->refresh_equip_force(equip);
			this->player_self()->active_item_addit_info_update(equip);
		}

		// 监听
		player_self()->achievement_listen_rune();
		player_self()->task_listen_equip_crimson_level(part, it_hole->second.level);
	}

	MSG_50103001 res_msg;
	res_msg.is_activate = is_activate;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Rune::req_rune_fill_energy(const int part, const int hole_index, int8_t is_bat, const int8_t auto_buy) {
	if (!part || !hole_index) {
		return ERROR_CLIENT_PARAM;
	}

	// Check hole exist
	Rune_Part_Detail_Map::iterator it_part = rune_detail_->part_detail_map.find(part);
	if (it_part == rune_detail_->part_detail_map.end()) {
		return ERROR_RUNE_PART_NOT_EXIST;
	}
	Rune_Hole_Detail_Map::iterator it_hole = it_part->second.hole_detail_map.find(hole_index);
	if (it_hole == it_part->second.hole_detail_map.end()) {
		return ERROR_RUNE_HOLE_NOT_EXIST;
	}

	// Check hole order
	int min_index = it_part->second.hole_detail_map.begin()->second.index;	// 默认第一孔
	int min_level = it_part->second.hole_detail_map.begin()->second.level;	// 默认第一孔
	for (Rune_Hole_Detail_Map::const_iterator it_check_hole = it_part->second.hole_detail_map.begin();
			it_check_hole != it_part->second.hole_detail_map.end(); ++it_check_hole) {
		// 找比第一孔等级低的
		if (it_check_hole->second.level < min_level) {
			min_index = it_check_hole->second.index;
			// 取找到的第一个
			break;
		}
	}
	if (hole_index != min_index) {
		//return ERROR_RUNE_HOLE_ORDER;
		return -1;	// 这里前端不让提示
	}

	// check is activate
	if (! it_hole->second.level) {
		return ERROR_RUNE_HOLE_FIRST_ACTIVATE;
	}

	// 充能过程
	//Bat_Consume_Info bat_info;
	int old_level = it_hole->second.level;
	int res_fill = rune_fill_energy(it_hole->second, is_bat, auto_buy);
	if (res_fill) {
		return res_fill;
	}

	bool is_upgrade = old_level < it_hole->second.level ? true : false;

	active_rune_hole_info(part, hole_index, is_upgrade);

	rune_detail_->detail_change();

	if (is_upgrade) {
		if (is_hero_rune(it_part->first)) {
			player_self()->heroer_prop_refresh(player_self()->heroer_detail().fighter_index);
		} else {
			rune_prop_refresh();
		}

		// 装备分数
		Item_Detail *equip = this->player_self()->pack_get_item(part);
		if (equip) {
			this->player_self()->refresh_equip_force(equip);
			this->player_self()->active_item_addit_info_update(equip);
		}

		// announce
		if (it_hole->second.level == MAX_RUNE_HOLE_LVL) {
			int equip_part = player_self()->get_part_by_index(part);
			if (equip_part) {
				const Rune_Part_Cfg *part_cfg = CONFIG_CACHE_RUNE->find_part_cfg(equip_part);
				if (part_cfg) {
					player_self()->create_ann_rune_fill_energy(part_cfg->prop_type, it_hole->second.level);
				}
			}
		}

		// 监听
		player_self()->achievement_listen_rune();
		player_self()->task_listen_equip_crimson_level(part, it_hole->second.level);
	}

	MSG_50103002 res_msg;
	res_msg.is_success = 1;
	res_msg.is_upgrade = is_upgrade;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Rune::rune_fill_energy(Rune_Hole_Detail &hole_detail, bool is_bat, const int8_t auto_buy) {
	// Check hole_lvl
//	if (! hole_detail.level) {
//		return ERROR_RUNE_HOLE_FIRST_ACTIVATE;
//	}
	if (hole_detail.level >= MAX_RUNE_HOLE_LVL) {
		return ERROR_UNBAN_LVL_MAX;
	}

	const Rune_Hole_Lvl_Cfg *hole_lvl_cfg = CONFIG_CACHE_RUNE->find_hole_lvl_cfg(hole_detail.level + 1);
	if (! hole_lvl_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (hole_lvl_cfg->open_level > player_self()->level()) {
		return ERROR_LEVEL_LIMIT;
	}
	const Item_Detail_Config *item_material_cfg = CONFIG_CACHE_ITEM->find_item(hole_lvl_cfg->material_id);
	if (! item_material_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	int times = is_bat ? 1000 : 1;
	int need_copper = 0;
	int need_gold = 0;
	int erase_material_num = 0;
	int buy_material_num = 0;
	//if (is_bat) {
		// 计算批量次数
		int check_res = fill_energy_times(times, hole_detail, hole_lvl_cfg, item_material_cfg, auto_buy,
				need_copper, need_gold, erase_material_num, buy_material_num);
		if (check_res) {
			return check_res;
		}
	//}

	// check money
	Money_Sub_Info money_info_copper(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), need_copper, MONEY_SUB_RUNE_FILL_ENERGY));
	if (money_info_copper.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info_copper);
		if (try_res) {
			return try_res;
		}
	}
	Money_Sub_Info money_info_gold(Money_Sub_Info(Money_Sub_Type(BIND_GOLD_FIRST), need_gold, MONEY_SUB_RUNE_FILL_ENERGY));
	if (money_info_gold.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info_gold);
		if (try_res) {
			return try_res;
		}
	}
	// check material
	Id_Amount cost_material(hole_lvl_cfg->material_id, erase_material_num);
	if (cost_material.id && cost_material.amount) {
		int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, cost_material);
		if (try_res) {
			return try_res;
		}
	}

	//-------------------------- all check passed line ------------------------------------------------

	/*
	 * Sub money and items
	 */
	if (money_info_copper.nums > 0) {
		player_self()->pack_sub_money(money_info_copper);
	}
	if (money_info_gold.nums > 0) {
		player_self()->pack_sub_money(money_info_gold);
	}
	if (cost_material.id && cost_material.amount) {
		 player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, cost_material, Lose_Item_DM_Info(ITEM_LOSE_RUNE_FILL_ENERGY));
	}


	bool is_upgrade = false;
	if (item_material_cfg->special_cfg.rune_essence_energy > 0) {
		int remain_energy = hole_detail.energy +
				item_material_cfg->special_cfg.rune_essence_energy * (cost_material.amount + buy_material_num);
		int hole_lvl = hole_detail.level;
		int old_hole_lvl = hole_detail.level;
		const Int_Vec *p_cfg_energy_vec = CONFIG_CACHE_RUNE->hole_lvl_energy_vec();
		if (!p_cfg_energy_vec || p_cfg_energy_vec->empty()) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		// 能量(经验)转化为等级
		exp_to_lvl(remain_energy, hole_lvl, MAX_RUNE_HOLE_LVL, *p_cfg_energy_vec);

		hole_detail.level = hole_lvl;
		hole_detail.energy = remain_energy;

		is_upgrade = hole_detail.level > old_hole_lvl ? true : false;
	}

	return 0;
}

int Rune::fill_energy_times(int &times, const Rune_Hole_Detail &hole_detail, const Rune_Hole_Lvl_Cfg *hole_lvl_cfg,
		const Item_Detail_Config *item_material_cfg, const int8_t auto_buy,
		int &need_copper, int &need_gold ,int &erase_material_num, int &buy_material_num) {
	if (!hole_lvl_cfg || !item_material_cfg) {
		return 0;
	}

	//int auto_buy = 0;

	// try upgrade
	int times_to_upgrade = 0;
	const Int_Vec *p_cfg_energy_vec = CONFIG_CACHE_RUNE->hole_lvl_energy_vec();
	if (!p_cfg_energy_vec || p_cfg_energy_vec->empty()) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	double once_material_energy = item_material_cfg->special_cfg.rune_essence_energy * hole_lvl_cfg->material_num_once;
	double upgrade_need_energy = hole_lvl_cfg->energy - hole_detail.energy;
	times_to_upgrade = ceil(U_FUNC::div(upgrade_need_energy, once_material_energy));


	Money_Detail money = player_self()->pack_get_money();

	/*
	 * 单算copper支持的次数
	 */
	int copper_times = U_FUNC::div(money.gold, hole_lvl_cfg->cost_copper);
	if (copper_times < 1) {
		return ERROR_COPPER_NOT_ENOUGH;
	}

	/*
	 * 单算gold支持的次数
	 */
	int cnt_gold = 0;				// gold支持的次数
	int buy_material_num_once = 0;	// 单次购买材料数
	int gold_cost_once = 0;			// 单次消耗gold数
	int64_t gold_all_cost = 0;		// N次gold总消耗

	int pack_material_num = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, hole_lvl_cfg->material_id);
	int pack_material_tmp = pack_material_num;

	int64_t money_gold_total = static_cast<int64_t>(money.bind_diamond) + static_cast<int64_t>(money.diamond);

	for(int8_t i = 1; i <= times_to_upgrade; ++i) {
		buy_material_num_once = 0;
		gold_cost_once = 0;
		//锁单次花费:
		if (pack_material_tmp >= hole_lvl_cfg->material_num_once) {
			buy_material_num_once = 0;
			pack_material_tmp -= hole_lvl_cfg->material_num_once;
		} else {
			pack_material_tmp = 0;
			if (auto_buy) {
				buy_material_num_once = hole_lvl_cfg->material_num_once - pack_material_tmp;
			} else {
				//
			}
		}
		//单次花费，可能是变动值，即单次不完全一样:
		if (buy_material_num_once) {
			gold_cost_once += item_material_cfg->sys_sell * buy_material_num_once;
		}
		//总花费
		gold_all_cost += gold_cost_once;
		if (money_gold_total < gold_all_cost) {	 // < not =
			break;
		}

		cnt_gold = i;
	}

	/*
	 * 单算无自动购买,material支持次数
	 */
	int cnt_material = cnt_gold;
	if (! auto_buy) {
		if (hole_lvl_cfg->material_num_once > 0) {
			int pack_material_times = U_FUNC::div(pack_material_num, hole_lvl_cfg->material_num_once);
			if (pack_material_times < 1) {
				MSG_82100008 active_msg;
				active_msg.item_ids.push_back(item_material_cfg->item_id);
				THIS_SEND_TO_CLIENT(active_msg);
				return -1;
			}
			cnt_material = pack_material_times;
		} else {
			//
		}
	} else {
		if (cnt_gold < 1 && hole_lvl_cfg->material_num_once > 0) {
			return ERROR_GOLD_NOT_ENOUGH;
		}
	}

	times = std::min(times, times_to_upgrade);
	times = std::min(times, cnt_gold);
	times = std::min(times, copper_times);
	times = std::min(times, cnt_material);

	if (times < 1) {
		return ERROR_EQUIP_ENCHANT_CNT_ZERO;
	}

	// 实际购买锁数
	//int buy_material_num = 0;
	if (pack_material_num < 1) {
		buy_material_num = times * hole_lvl_cfg->material_num_once;
	} else if (pack_material_num >= 1 && pack_material_num < times * hole_lvl_cfg->material_num_once) {
		buy_material_num =  times * hole_lvl_cfg->material_num_once - pack_material_num;
	} else {
		buy_material_num = 0;
	}

	need_copper = hole_lvl_cfg->cost_copper * times;
	need_gold = item_material_cfg->sys_sell * buy_material_num;

	erase_material_num = times * hole_lvl_cfg->material_num_once - buy_material_num;

	return 0;
}

void Rune::active_rune_hole_info(const int part, const int hole_index, bool is_upgrade) {
	MSG_81002000 active_msg;
	active_msg.reset();
	active_msg.part = part;
	rune_detail_->get_rune_hole_info(part, hole_index, active_msg.hole_info);
	active_msg.is_upgrade = is_upgrade;
	THIS_SEND_TO_CLIENT(active_msg);
}

int Rune::active_rune_info(Logic_Player* target_player) {
	if (! target_player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	MSG_82210002 active_msg;
	active_msg.target_role_id = target_player->role_id();
	Rune_Part_Info part_info;
	for (Rune_Part_Detail_Map::const_iterator it_part = target_player->rune_detail().part_detail_map.begin();
			it_part != target_player->rune_detail().part_detail_map.end(); ++it_part) {
		part_info.reset();
		target_player->rune_detail().get_rune_part_info(it_part->second.part, part_info);
		active_msg.part_infos.push_back(part_info);
	}
	THIS_SEND_TO_CLIENT(active_msg);
	return 0;
}

int Rune::rune_prop_refresh(void) {
	/*
	 * 符文系统属性和装备属性互为条件
	 */
	std::map<int, double> pro_val_map;
	int hole_lvl_whole = 1000;
	for (Rune_Part_Detail_Map::const_iterator it_part = rune_detail_->part_detail_map.begin();
			it_part != rune_detail_->part_detail_map.end(); ++it_part) {
		if (it_part->first > PACK_T_HERO_EQUIP_INDEX) {
			break;	// hero prop refresh at heroer module
		}

		// check equip
		uint rune_index = it_part->first;
		if (player_self()->is_item_exist(rune_index)) {
			rune_prop_refresh(rune_index, rune_index, pro_val_map);

			if (rune_index < EQUIP_IDX_SUIT_END) {
				if (hole_lvl_whole) {
					int part_min_hole_lvl = get_part_min_hole_lvl(it_part->second);
					hole_lvl_whole = part_min_hole_lvl < hole_lvl_whole ? part_min_hole_lvl : hole_lvl_whole;
				}
			}

		} else {
			if (rune_index < EQUIP_IDX_SUIT_END) {
				hole_lvl_whole = 0;
			}
		}
	}

	// 全身符文属性
	if (hole_lvl_whole && hole_lvl_whole != 1000) {
		hole_lvl_whole_prop_refresh(hole_lvl_whole, pro_val_map);
	}

	// 人物属性加成
//	player_self()->addition_prop_refresh(AT_RUNE, PST_FIXED, pro_val_map, 261);

//	set_rune_pro_val(pro_val_map);

	return 0;
}

int Rune::rune_prop_refresh(const int rune_part, const int equip_index, std::map<int, double> &pro_val_map) {
	Rune_Part_Detail_Map::iterator it_part = rune_detail_->part_detail_map.find(rune_part);
	if (it_part == rune_detail_->part_detail_map.end()) {
		return ERROR_RUNE_PART_NOT_EXIST;
	}

	int equip_part = player_self()->get_part_by_index(equip_index);
	if (! equip_part) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Rune_Part_Cfg *part_cfg = CONFIG_CACHE_RUNE->find_part_cfg(equip_part);
	if (! part_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	for (Rune_Hole_Detail_Map::const_iterator it_hole = it_part->second.hole_detail_map.begin();
			it_hole != it_part->second.hole_detail_map.end(); ++it_hole) {
		if (it_hole->second.level) {
			calculate_rune_prop(it_hole->second.level, part_cfg->prop_type, pro_val_map);
		}
	}

	return 0;
}

int Rune::calculate_rune_prop(const int hole_lvl, const int prop_type, std::map<int, double> &pro_val_map) {
	const Rune_Hole_Lvl_Cfg *hole_lvl_cfg = CONFIG_CACHE_RUNE->find_hole_lvl_cfg(hole_lvl);
	if (!hole_lvl_cfg  || hole_lvl_cfg->prop_val_rate <= 0) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	double base_val = hole_lvl_cfg->prop_val;
	double final_val = base_val * hole_lvl_cfg->prop_val_rate * 0.01;
	pro_val_map[prop_type] += final_val;

	return 0;
}

int Rune::hole_lvl_whole_prop_refresh(const int hole_lvl_whole, std::map<int, double> &pro_val_map) {
	if (! hole_lvl_whole) {
		return 0;
	}

	int cfg_fix_whole_lvl = 0;
	const Hole_Lvl_Whole_Cfg_Map& hole_lvl_whole_cfg_map = CONFIG_CACHE_RUNE->hole_lvl_whole_cfg_map();
	for (Hole_Lvl_Whole_Cfg_Map::const_iterator it_cfg_whole_map = hole_lvl_whole_cfg_map.begin();
			it_cfg_whole_map != hole_lvl_whole_cfg_map.end(); ++it_cfg_whole_map) {
		if (hole_lvl_whole >= it_cfg_whole_map->first) {
			cfg_fix_whole_lvl = it_cfg_whole_map->first;
		} else {
			break;
		}
	}
	if (! cfg_fix_whole_lvl) {
		return 0;
	}

	const Hole_Lvl_Whole_Cfg *lvl_whole_cfg = CONFIG_CACHE_RUNE->find_hole_lvl_whole_cfg(cfg_fix_whole_lvl);
	if (!lvl_whole_cfg || lvl_whole_cfg->prop_val_map.empty()) {
		return -1;
	}
	for (Int_Double_Map::const_iterator it_prop_val = lvl_whole_cfg->prop_val_map.begin();
			it_prop_val != lvl_whole_cfg->prop_val_map.end(); ++it_prop_val) {
		int prop = it_prop_val->first;
		double val = it_prop_val->second;
		pro_val_map[prop] += val;
	}

	return 0;
}

bool Rune::rune_part_is_activated(const int part) {
	Rune_Part_Detail_Map::iterator it_part = rune_detail_->part_detail_map.find(part);
	if (it_part == rune_detail_->part_detail_map.end()) {
		return false;
	}
	for (Rune_Hole_Detail_Map::const_iterator it_hole = it_part->second.hole_detail_map.begin();
			it_hole != it_part->second.hole_detail_map.end(); ++it_hole) {
		if (! it_hole->second.level) {
			return false;
		}
	}
	return true;
}

int Rune::get_rune_pro_val(std::map<int, double> &pro_val_map) const {
	pro_val_map.clear();
	pro_val_map = rune_detail_->all_rune_pro_val;
	return 0;
}

int Rune::get_rune_part_lvl(Int_Int_Map &part_lvl_map) const {
	part_lvl_map.clear();
	for (Rune_Part_Detail_Map::iterator it_part = rune_detail_->part_detail_map.begin();
			it_part != rune_detail_->part_detail_map.end(); ++it_part) {
		int max_hole_lvl = 0;
		for (Rune_Hole_Detail_Map::const_iterator it_hole = it_part->second.hole_detail_map.begin();
				it_hole != it_part->second.hole_detail_map.end(); ++it_hole) {
			if (it_hole->second.level) {
				max_hole_lvl = it_hole->second.level > max_hole_lvl  ? it_hole->second.level : max_hole_lvl;
			}
		}
		part_lvl_map.insert(std::make_pair(it_part->first, max_hole_lvl));
	}
	return 0;
}

int Rune::set_rune_pro_val(const std::map<int, double> &pro_val_map) {
	rune_detail_->all_rune_pro_val.clear();
	rune_detail_->all_rune_pro_val = pro_val_map;
	//rune_detail_->detail_change();
	return 0;
}

int Rune::test_runes_lvl_up(const int lvl) {
	int lvl_bak = lvl;
	if (lvl >= MAX_RUNE_HOLE_LVL) {
//		return ERROR_UNBAN_LVL_MAX;
		lvl_bak = MAX_RUNE_HOLE_LVL - 1;
	}

	for (Rune_Part_Detail_Map::iterator it_part = rune_detail_->part_detail_map.begin();
			it_part != rune_detail_->part_detail_map.end(); ++it_part) {
		for (Rune_Hole_Detail_Map::iterator it_hole = it_part->second.hole_detail_map.begin();
				it_hole != it_part->second.hole_detail_map.end(); ++it_hole) {
			it_hole->second.level = lvl_bak;
		}
	}

	rune_prop_refresh();
	player_self()->heroer_prop_refresh();

	return 0;
}

int Rune::get_rune_amount_by_lvl(const int lvl) {
	int am = 0;
	for (Rune_Part_Detail_Map::iterator it_part = rune_detail_->part_detail_map.begin();
			it_part != rune_detail_->part_detail_map.end(); ++it_part) {
		for (Rune_Hole_Detail_Map::iterator it_hole = it_part->second.hole_detail_map.begin();
				it_hole != it_part->second.hole_detail_map.end(); ++it_hole) {
			if (it_hole->second.level >= lvl) {
				++am;
			}
		}
	}
	return am;
}

int Rune::get_part_min_hole_lvl(const Rune_Part_Detail& part_detail) {
	int min_lvl = 1000;
	for (Rune_Hole_Detail_Map::const_iterator it_hole = part_detail.hole_detail_map.begin();
			it_hole != part_detail.hole_detail_map.end(); ++it_hole) {
		min_lvl = it_hole->second.level < min_lvl ? it_hole->second.level : min_lvl;
	}
	min_lvl = (min_lvl == 1000) ? 0 : min_lvl;
	return min_lvl;
}
