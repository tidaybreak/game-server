/*
 * Rune_Stone.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: chenyulin
 */

#include "Logic_Player.h"
#include "Logic_Module_Head.h"
#include "Rune_Stone_Def.h"
#include "Rune_Stone.h"
#include "Rune_Stone_Struct.h"
#include "Rune_Stone_Func.h"
#include "Config_Cache_Item.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "equiper/Equiper_Func.h"
#include "Msg_Active_Struct.h"
#include "Msg_Rune_Stone_Struct.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Rune_Stone.h"
#include "Config_Cache_Role.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "helper/Helper_Def.h"
#include "active/Active_Manager.h"
#include "active/Active_Def.h"
#include "oper_activity/Oper_Activity_Def.h"

Rune_Stone::Rune_Stone(void) {
	reset();
}

Rune_Stone::~Rune_Stone() {

}

void Rune_Stone::reset(void) {
	rune_stone_detail_ = 0;
}

int Rune_Stone::create_init(Rune_Stone_Detail &detail) {
	init_smelters(detail);

	detail.remain_double_times = 3;

	return 0;
}

void Rune_Stone::load_detail(Rune_Stone_Detail &detail) {
	rune_stone_detail_ = &detail;
}

void Rune_Stone::save_detail(Rune_Stone_Detail &detail) {
	if (rune_stone_detail().is_change) {
		detail = rune_stone_detail();
		rune_stone_detail_->is_change = false;
	}
}

void Rune_Stone::sign_in(void) {

}

void Rune_Stone::sign_out(void) {

}

void Rune_Stone::module_init(void) {
	init_smelters();
	init_player_all_pack_cap();
	rune_stone_prop_refresh();
}

int Rune_Stone::client_ready_now(void) {

	return 0;
}

int Rune_Stone::trigger_daily_zero(const Time_Value& now) {
	rune_stone_detail_->remain_double_times = 3;
	rune_stone_detail_->detail_change();
	req_rune_stone_syst_info();		// TODO acitve msg
	return 0;
}

int Rune_Stone::req_rune_stone_syst_info(void) {
	MSG_50103200 res_msg;
	res_msg.points = rune_stone_detail_->points;
	res_msg.remain_double_times = rune_stone_detail_->remain_double_times;

	Smelter_Info smelter_info;
	for (Smelter_Detail_Map::iterator it_smelter = rune_stone_detail_->smelter_detail_map.begin();
			it_smelter != rune_stone_detail_->smelter_detail_map.end(); ++it_smelter) {
		smelter_info.reset();
		rune_stone_detail_->make_smelter_info(it_smelter->second.smelter_idx, smelter_info);
		res_msg.smelter_infos.push_back(smelter_info);
	}

	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Rune_Stone::req_rune_stone_smelt(const int8_t smelter_idx, const int8_t is_bat) {
	if (smelter_idx < Rune_Stone_Detail::SMELTER_IDX_BEGIN || smelter_idx >= Rune_Stone_Detail::SMELTER_IDX_END) {
		return ERROR_CLIENT_PARAM;
	}
	Smelter_Detail_Map::iterator it_smelter = rune_stone_detail_->smelter_detail_map.find(smelter_idx);
	if (it_smelter == rune_stone_detail_->smelter_detail_map.end() || !it_smelter->second.is_open) {
		//return ERROR_CLIENT_OPERATE;
		return -1;	// 前端不提示
	}

	const Smelter_Idx_Cfg* smelter_idx_cfg = CONFIG_CACHE_RUNE_STONE->find_smelter_idx(smelter_idx);
	if (!smelter_idx_cfg || smelter_idx_cfg->item_ids.empty() || smelter_idx_cfg->item_weights.empty() ||
			smelter_idx_cfg->item_ids.size() != smelter_idx_cfg->item_weights.size()) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	const Rune_Stone_Mix_Cfg& rune_stone_mix_cfg = CONFIG_CACHE_RUNE_STONE->rune_stone_mix();

	/*
	 * Check money
	 */
	// check money
	Money_Sub_Info money_sub_copper(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), smelter_idx_cfg->cost_copper, MONEY_SUB_RUNE_STONE_SMELT));
	if (money_sub_copper.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_sub_copper);
		if (try_res) {
			return try_res;
		}
	}

	/*
	 * rand item
	 */
	Int_Vec weights = smelter_idx_cfg->item_weights;
	int valentine_rate = ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_RUNE_STONE_UP);

	if (! rune_stone_mix_cfg.smelt_bufs.empty()) {
		int buf_id = 0;
		for (Int_Set::const_iterator it_smelt_bufs = rune_stone_mix_cfg.smelt_bufs.begin();
				it_smelt_bufs != rune_stone_mix_cfg.smelt_bufs.end(); ++it_smelt_bufs) {
			if (player_self()->has_status_id(*it_smelt_bufs)) {
				buf_id = *it_smelt_bufs;
				break;
			}
		}
		if (buf_id) {
			double buf_effect = player_self()->find_rune_stone_smelt_status_rate(buf_id);
			for (uint16_t i = 0; i < smelter_idx_cfg->item_ids.size(); ++i) {
				const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(smelter_idx_cfg->item_ids[i]);
				if (! item_config) {
					continue;
				}
				if (item_config->color >= Item_Detail::GOLDEN_PURPLE) {
					weights[i] *= (buf_effect + valentine_rate);
				}
			}
		}
	}

	int wight_idx = U_FUNC::get_rand_index(weights);
	if (wight_idx < 0 || (uint)wight_idx >= smelter_idx_cfg->item_ids.size()) {
		return ERROR_INNER_SERVER;
	}
	int output_item_id_1st = smelter_idx_cfg->item_ids[wight_idx];
	if (! output_item_id_1st) {
		return ERROR_INNER_SERVER;
	}
	// rand double_times
	int output_item_id_2nd = 0;
	bool use_2nd = rune_stone_detail_->remain_double_times > 0;
	if (use_2nd) {
		int wight_idx_2nd = U_FUNC::get_rand_index(weights);
		if (wight_idx_2nd < 0 || (uint)wight_idx_2nd >= smelter_idx_cfg->item_ids.size()) {
			return ERROR_INNER_SERVER;
		}
		output_item_id_2nd = smelter_idx_cfg->item_ids[wight_idx_2nd];
		if (! output_item_id_2nd) {
			return ERROR_INNER_SERVER;
		}
	}

	// 产出金币产出转化为直接插入金钱
	int output_copper_1st = 0;
	if (rune_stone_mix_cfg.smelt_output_copper_items.count(output_item_id_1st)) {
		Item_Resource_Cfg::const_iterator find_item_resource = CONFIG_CACHE_ITEM->item_resource_config().find(output_item_id_1st);
		if (find_item_resource != CONFIG_CACHE_ITEM->item_resource_config().end()) {
			// 策划确认不会产出金币
			output_copper_1st += 0;
//			if ( find_item_resource->second.gain_list.size() > 0 ) {
//				output_copper_1st = find_item_resource->second.gain_list[0].val_2;
//			}
		} else {
			return ERROR_CONFIG_NOT_EXIST;
		}
		output_item_id_1st = 0;
	}
	int output_copper_2nd = 0;
	if (rune_stone_mix_cfg.smelt_output_copper_items.count(output_item_id_2nd)) {
		Item_Resource_Cfg::const_iterator find_item_resource = CONFIG_CACHE_ITEM->item_resource_config().find(output_item_id_2nd);
		if (find_item_resource != CONFIG_CACHE_ITEM->item_resource_config().end()) {
			// 策划确认不会产出金币
			output_copper_2nd += 0;
//			if ( find_item_resource->second.gain_list.size() > 0 ) {
//				output_copper_2nd += find_item_resource->second.gain_list[0].val_2;
//			}
		} else {
			return ERROR_CONFIG_NOT_EXIST;
		}
		output_item_id_2nd = 0;
	}

	Item_Detail output_item(output_item_id_1st, 1, Item_Detail::BIND);
	if (output_item.id_ > 0) {
		int ret_insert = pack_try_insert_item(PACK_T_RUNE_STONE_SMELT_INDEX, output_item);
		if (ret_insert) {
			return ret_insert;
		}
	}
	Item_Detail output_item_2nd(output_item_id_2nd, 1, Item_Detail::BIND);
	if (output_item_2nd.id_ > 0) {
		int ret_insert = pack_try_insert_item(PACK_T_RUNE_STONE_SMELT_INDEX, output_item_2nd);
		if (ret_insert) {
			return ret_insert;
		}
	}

	/*
	 * rand smelter_idx
	 */
	Int_Vec smelter_idx_weights;
	smelter_idx_weights.push_back(smelter_idx_cfg->front_rate);			// 0 front
	smelter_idx_weights.push_back(100 - smelter_idx_cfg->front_rate);	// 1 back
	int wight_idx_smelter = U_FUNC::get_rand_index(smelter_idx_weights);
	if (wight_idx_smelter < 0 || wight_idx_smelter >= 2) {
		return ERROR_INNER_SERVER;
	}

	//-------------------------- all check passed line ------------------------------------------------

	/*
	 * set next smelter
	 */
	bool smelter_go_front = wight_idx_smelter == 0  ? true : false;
	int smelter_idx_next = 1;
	if (smelter_go_front) {
		smelter_idx_next = (smelter_idx + 1) > Rune_Stone_Detail::SMELTER_IDX_END  ? smelter_idx : smelter_idx + 1;
	} else {
		//smelter_idx_next = (smelter_idx - 1) < Rune_Stone_Detail::SMELTER_IDX_BEGIN  ? smelter_idx : smelter_idx - 1;
		smelter_idx_next = Rune_Stone_Detail::SMELTER_IDX_BEGIN;
	}
	//Smelter_Detail_Map::iterator it_smelter = rune_stone_detail_->smelter_detail_map.find(smelter_idx);
	if (it_smelter != rune_stone_detail_->smelter_detail_map.end()) {
		it_smelter->second.is_open = it_smelter->first == 1  ?  1 : 0;	// 第一个熔炉一直亮
	}
	Smelter_Detail_Map::iterator it_smelter_next = rune_stone_detail_->smelter_detail_map.find(smelter_idx_next);
	if (it_smelter_next != rune_stone_detail_->smelter_detail_map.end()) {
		it_smelter_next->second.is_open = 1;
	}

	// sub money
	if (money_sub_copper.nums > 0) {
		player_self()->pack_sub_money(money_sub_copper);
	}

	// output money
	int output_copper = output_copper_1st + output_copper_2nd;
	if (output_copper > 0) {
		int ret_error = pack_add_money(Money_Add_Info(Pack::COPPER, output_copper, MONEY_ADD_RUNE_STONE_SMELT));
		if (ret_error) {
			return ret_error;
		}
	}

	// output points
	//int output_points = use_2nd ? smelter_idx_cfg->point * 2 : smelter_idx_cfg->point;
	int total_points_1st = rune_stone_detail_->points + smelter_idx_cfg->point;
	int total_points_2nd = use_2nd ? total_points_1st + smelter_idx_cfg->point : total_points_1st;
	rune_stone_detail_->points = total_points_2nd;

	if (use_2nd) {
		--rune_stone_detail_->remain_double_times;
	}

	rune_stone_detail_->detail_change();

	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	// 客户端要求先返回RES消息再主动通知物品更新
	MSG_50103201 res_msg;
	res_msg.smelter_idx = smelter_idx;
	res_msg.smelter_idx_next = smelter_idx_next;
	res_msg.is_bat = is_bat;
	res_msg.result = 1;
	res_msg.item_id = output_item_id_1st;
	res_msg.get_copper = output_copper_1st;
	res_msg.total_points = total_points_1st;
	{ THIS_SEND_TO_CLIENT(res_msg); }

	if (use_2nd) {
		res_msg.reset();
		res_msg.smelter_idx = smelter_idx;
		res_msg.smelter_idx_next = smelter_idx_next;
		res_msg.is_bat = is_bat;
		res_msg.result = 1;
		res_msg.item_id = output_item_id_2nd;
		res_msg.get_copper = output_copper_2nd;
		res_msg.total_points = total_points_2nd;
		{ THIS_SEND_TO_CLIENT(res_msg); }
	}

	req_rune_stone_syst_info();		// TODO acitve msg

	// output item
	if (output_item.id_) {
		Item_Basic_Info ibi;
		output_item.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
		pack_insert_item(PACK_T_RUNE_STONE_SMELT_INDEX, output_item, Gain_Item_DM_Info(ITEM_GAIN_RUNE_STONE_SMELT));
	}
	if (output_item_2nd.id_) {
		Item_Basic_Info ibi;
		output_item_2nd.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
		pack_insert_item(PACK_T_RUNE_STONE_SMELT_INDEX, output_item_2nd, Gain_Item_DM_Info(ITEM_GAIN_RUNE_STONE_SMELT));
	}
	if (!acv_msg.item_info.empty()) {
		// 需求暂不发
		//THIS_SEND_TO_CLIENT(acv_msg);
	}

	// announce
	if (output_item_id_1st) {
		const Item_Detail_Config* item_cfg = CONFIG_CACHE_ITEM->find_item(output_item_id_1st);
		if (item_cfg && item_cfg->color >= Item_Detail::GOLDEN_PURPLE) {
			Chat_Item_Info chat_item_info;
			chat_item_info.type = 4;
			chat_item_info.item_base_info.id = output_item_id_1st;
			chat_item_info.item_base_info.amount = 1;
			chat_item_info.item_base_info.bind = Item_Detail::BIND;
			chat_item_info.rune_stone_info.index = 0;
			chat_item_info.rune_stone_info.exp_lvl = 1;
			chat_item_info.rune_stone_info.exp = 0;
			player_self()->create_ann_rune_stone_smelt(chat_item_info);
		}
		player_self()->achievement_listen_rune_stone_lvl(1);
	}
	// announce
	if (output_item_id_2nd) {
		const Item_Detail_Config* item_cfg = CONFIG_CACHE_ITEM->find_item(output_item_id_2nd);
		if (item_cfg && item_cfg->color >= Item_Detail::GOLDEN_PURPLE) {
			Chat_Item_Info chat_item_info;
			chat_item_info.type = 4;
			chat_item_info.item_base_info.id = output_item_id_2nd;
			chat_item_info.item_base_info.amount = 1;
			chat_item_info.item_base_info.bind = Item_Detail::BIND;
			chat_item_info.rune_stone_info.index = 0;
			chat_item_info.rune_stone_info.exp_lvl = 1;
			chat_item_info.rune_stone_info.exp = 0;
			player_self()->create_ann_rune_stone_smelt(chat_item_info);
		}
	}

	// listening
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_RUNE_SMELT);
	player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_RUNE_STONE_NUM);


	return 0;
}

int Rune_Stone::req_rune_stone_sell(const int8_t is_bat, const int under_color, const int index) {
	if (is_bat) {
		if (!under_color || index) {
			return ERROR_CLIENT_PARAM;
		}
	} else {
		if (! index) {
			return ERROR_CLIENT_PARAM;
		}
	}

	MSG_50103202 res_msg;

	if (! is_bat) {
		Item_Detail* rune_stone = this->pack_get_item(index);
		if (! rune_stone) {
			return ERROR_ITEM_NOT_EXIST;
		}
		const Item_Detail_Config* item_cfg = CONFIG_CACHE_ITEM->find_item(rune_stone->id_);
		if (! item_cfg) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (item_cfg->sellPrice) {
			Index_Amount index_amount(index, 1);
			int try_res = pack_try_erase_item(index_amount);
			if (try_res) {
				return try_res;
			}

			int ret_error = pack_add_money(Money_Add_Info(Pack::COPPER, item_cfg->sellPrice, MONEY_ADD_RUNE_STONE_SMELT));
			if (ret_error) {
				return ret_error;
			}

			pack_erase_item(index_amount, Lose_Item_DM_Info(ITEM_LOSE_RUNE_STONE_SELL));

			res_msg.vec_index.push_back(index);
		}

	/*
	 * bat
	 */
	} else {
		Item_Detail *item = NULL;
		Index_Amount_Vec idx_amount_vec;
		int add_copper = 0;

		int idx_end = PACK_T_RUNE_STONE_SMELT_INDEX + pack_get_capacity(PACK_T_RUNE_STONE_SMELT_INDEX) + 1;
		for (int idx = PACK_T_RUNE_STONE_SMELT_INDEX + RS_INNER_IDX_BEGIN; idx < idx_end; ++idx) {
			item = this->pack_get_item(idx);
			if (! item) {
				continue;
			}
			const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
			if (! item_cfg) {
				continue;
			}

			if (item_cfg->color <= under_color) {
				idx_amount_vec.push_back(Index_Amount(idx, 1));
				res_msg.vec_index.push_back(idx);
				if (item_cfg->sellPrice) {
					add_copper += item_cfg->sellPrice;
				}
			}
		}

		if (! idx_amount_vec.empty()) {
			int ret_try = pack_try_erase_item(idx_amount_vec);
			if (ret_try) {
				return ret_try;
			}
			pack_erase_item(idx_amount_vec, Lose_Item_DM_Info(ITEM_LOSE_RUNE_STONE_SELL));
		}

		if (add_copper) {
			int ret_error = pack_add_money(Money_Add_Info(Pack::COPPER, add_copper, MONEY_ADD_RUNE_STONE_SELL));
			if (ret_error) {
				return ret_error;
			}
		}
	}

	res_msg.is_bat = is_bat;
	res_msg.under_color = under_color;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Rune_Stone::req_rune_stone_pick(const int8_t is_bat, const int index) {
	if ((is_bat && index) || (!is_bat && !index)) {
		return ERROR_CLIENT_PARAM;
	}

	if (! is_bat) {
		Item_Detail* rune_stone = this->pack_get_item(index);
		if (! rune_stone) {
			return ERROR_ITEM_NOT_EXIST;
		}
		int result = this->pack_move_item(index, Pack::PACK_T_RUNE_STONE_PACK_INDEX);
		if (result != 0) {
			return result;
		}

	/*
	 * bat
	 */
	} else {

	}

	MSG_50103203 res_msg;
	res_msg.is_bat = is_bat;
	res_msg.index = index;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	//task listen
	{
		std::map<int8_t, uint32_t> rune_quality_num_map;
		rune_quality_num_map.clear();
		get_rune_quality_num_map(rune_quality_num_map);

		for(std::map<int8_t, uint32_t>::const_iterator it = rune_quality_num_map.begin();
				it != rune_quality_num_map.end();
				++it) {
			player_self()->task_listen_get_quality_rune(it->first, it->second);
		}
	}

	return 0;
}

int Rune_Stone::req_rune_stone_lock(const int index, const int8_t is_lock) {
	if (! index) {
		return ERROR_CLIENT_PARAM;
	}
	Item_Detail* rune_stone = this->pack_get_item(index);
	if (! rune_stone) {
		return ERROR_ITEM_NOT_EXIST;
	}

	rune_stone->addon.rune_stone.is_lock = is_lock;

	MSG_50103204 res_msg;
	res_msg.index = index;
	res_msg.result = 1;
	res_msg.is_lock = is_lock;
	THIS_SEND_TO_CLIENT(res_msg);

	active_rune_stone_addit_info(rune_stone);

	return 0;
}

int Rune_Stone::req_rune_stone_merge(const int8_t is_bat, const int index_type, const int index_from, const int index_to) {
	if (is_bat) {
		if (!index_type || index_from) {
			return ERROR_CLIENT_PARAM;
		}
	} else {
		if (!index_from || !index_to) {
			return ERROR_CLIENT_PARAM;
		}
	}

	const Rune_Stone_Mix_Cfg& rune_stone_mix_cfg = CONFIG_CACHE_RUNE_STONE->rune_stone_mix();
	if (rune_stone_mix_cfg.color_exp_map.empty()) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	if (! is_bat) {
		Item_Detail* item_from = this->pack_get_item(index_from);
		Item_Detail* item_to = this->pack_get_item(index_to);
		if (!item_from || !item_to) {
			return ERROR_ITEM_NOT_EXIST;
		}
		if (item_from->addon.rune_stone.is_lock) {
			return ERROR_RUNE_STONE_LOCK;	// TODO
		}
		if (0 != item_from->is_item_type(item_from->id_, Item_Detail::RUNE_STONE) ||
				0 != item_to->is_item_type(item_to->id_, Item_Detail::RUNE_STONE)) {
			return ERROR_CLIENT_OPERATE;
		}

		const Item_Detail_Config* item_cfg_from = CONFIG_CACHE_ITEM->find_item(item_from->id_);
		const Item_Detail_Config* item_cfg_to = CONFIG_CACHE_ITEM->find_item(item_to->id_);
		if (!item_cfg_from || !item_cfg_to) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		std::map<int, int>::const_iterator it_mix_color_map = rune_stone_mix_cfg.color_exp_map.find(item_cfg_from->color);
		if (it_mix_color_map == rune_stone_mix_cfg.color_exp_map.end()) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		int all_exp_item_from = it_mix_color_map->second + item_from->addon.rune_stone.exp;
		int old_lvl_to = item_to->addon.rune_stone.exp_lvl;

		Index_Amount index_amount(index_from, 1);
		int ret_try = pack_try_erase_item(index_amount);
		if (ret_try) {
			return ret_try;
		}
		pack_erase_item(index_amount, Lose_Item_DM_Info(ITEM_LOSE_RUNE_STONE_MERGE));

		item_to->addon.rune_stone.exp += all_exp_item_from;

		update_exp_lvl(item_to);

		if (all_exp_item_from > 0) {
			active_rune_stone_addit_info(item_to);
		}

		if (0 == pack_is_pack_type(index_from, Pack::PACK_T_RUNE_STONE_EQUIP_INDEX) ||
				(0 == pack_is_pack_type(index_to, Pack::PACK_T_RUNE_STONE_EQUIP_INDEX) &&
						old_lvl_to > item_to->addon.rune_stone.exp_lvl)) {
			rune_stone_prop_refresh();
		}

	/*
	 * bat
	 */
	} else {
		Pack_Type pack_type = static_cast<Pack_Type>(index_type);
		if (pack_type != Pack::PACK_T_RUNE_STONE_SMELT_INDEX &&	pack_type != Pack::PACK_T_RUNE_STONE_PACK_INDEX) {
			return ERROR_CLIENT_PARAM;
		}

		int bat_index_to = 0;
		int bat_color_item_to = 0;
		Item_Detail *item = NULL;
		Index_Amount_Vec erase_idx_amount_vec;
		int all_exp_from_erase_items = 0;

		// bat find
		int idx_start = pack_type + RS_INNER_IDX_BEGIN;
		int idx_end = pack_type + pack_get_capacity(pack_type) + 1;
		for (int idx = idx_start; idx < idx_end; ++idx) {
			item = this->pack_get_item(idx);
			if (!item || item->addon.rune_stone.is_lock) {
				continue;
			}
			const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
			if (! item_cfg) {
				continue;
			}

			if (! bat_index_to) {	// first loop
				bat_index_to = idx;
				bat_color_item_to = item_cfg->color;
			} else {
				if (item_cfg->color > bat_color_item_to) {

					erase_idx_amount_vec.push_back(Index_Amount(bat_index_to, 1));
					Item_Detail *will_merged_item_to = this->pack_get_item(bat_index_to);
					if (will_merged_item_to) {
						const Item_Detail_Config *cfg_will_merged_item_to = CONFIG_CACHE_ITEM->find_item(will_merged_item_to->id_);
						if (cfg_will_merged_item_to) {
							std::map<int, int>::const_iterator it_mix_color_map = rune_stone_mix_cfg.color_exp_map.find(cfg_will_merged_item_to->color);
							if (it_mix_color_map != rune_stone_mix_cfg.color_exp_map.end()) {
								all_exp_from_erase_items += it_mix_color_map->second + will_merged_item_to->addon.rune_stone.exp;
							}
						};
					}

					// ---------bat_index_to has changed --------
					bat_index_to = idx;
					bat_color_item_to = item_cfg->color;
				} else {
					erase_idx_amount_vec.push_back(Index_Amount(idx, 1));

					std::map<int, int>::const_iterator it_mix_color_map = rune_stone_mix_cfg.color_exp_map.find(item_cfg->color);
					if (it_mix_color_map == rune_stone_mix_cfg.color_exp_map.end()) {
						return ERROR_CONFIG_NOT_EXIST;
					}
					all_exp_from_erase_items += it_mix_color_map->second + item->addon.rune_stone.exp;
				}
			}
		}

		// bat merge
		Item_Detail *item_to = this->pack_get_item(bat_index_to);
		if (! item_to) {
			return ERROR_ITEM_NOT_EXIST;
		}
		//int old_lvl_to = item_to->addon.rune_stone.exp_lvl;

		int ret_try = pack_try_erase_item(erase_idx_amount_vec);
		if (ret_try) {
			return ret_try;
		}
		pack_erase_item(erase_idx_amount_vec, Lose_Item_DM_Info(ITEM_LOSE_RUNE_STONE_MERGE));

		item_to->addon.rune_stone.exp += all_exp_from_erase_items;

		update_exp_lvl(item_to);

		if (all_exp_from_erase_items > 0) {
			active_rune_stone_addit_info(item_to);
		}

		//this->pack_sort_item(pack_type, MERGE_WAY_SIMILAR);
	}

	MSG_50103205 res_msg;
	res_msg.is_bat = is_bat;
	res_msg.index_from = index_from;
	res_msg.index_to = index_to;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Rune_Stone::req_rune_stone_move(const int index_from, const int index_to) {
	if (!index_from || !index_to) {
		return ERROR_CLIENT_PARAM;
	}

	Item_Detail* item_from = this->pack_get_item(index_from);
	if (!item_from) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (0 != item_from->is_item_type(item_from->id_, Item_Detail::RUNE_STONE)) {
		return ERROR_CLIENT_OPERATE;
	}

	/*
	 * 英雄特殊处理
	 */
	if (pack_is_hero_type(index_from) || pack_is_hero_type(index_to)) {
		return player_self()->req_hero_rune_stone_move(index_from, index_to);
	}

	/*
	 * 以下是主角穿脱
	 */

	Pack_Type pack_type_from;
	Pack_Type pack_type_to;
	if (0 != pack_get_pack_type(index_from, pack_type_from) ||
			0 != pack_get_pack_type(index_to, pack_type_to)) {
		return ERROR_PACK_INDEX_NOT_EXIST;
	}
	if (pack_type_from != pack_type_to) {
		return ERROR_CLIENT_OPERATE;
	}

	int ret_move = this->pack_move_item(index_from, index_to);
	if (ret_move) {
		return ret_move;
	}

	MSG_50103206 res_msg;
	res_msg.index_from = index_from;
	res_msg.index_type_to = pack_type_to;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Rune_Stone::req_rune_stone_exchange(const int item_id) {
	if (! item_id) {
		return ERROR_CLIENT_PARAM;
	}
	if (0 != Item_Detail::is_item_type(item_id, Item_Detail::RUNE_STONE)) {
		return ERROR_CLIENT_OPERATE;
	}
	if (! rune_stone_detail_->points) {
		return ERROR_RUNE_STONE_POINTS_LACK;
	}

	const Item_Detail_Config* item_cfg = CONFIG_CACHE_ITEM->find_item(item_id);
	if (! item_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	int sub_points = rune_stone_detail_->points - item_cfg->rune_stone.need_points;
	if (sub_points < 0) {
		return ERROR_RUNE_STONE_POINTS_LACK;
	}

	Item_Detail output_item(item_id, 1, Item_Detail::BIND);
	if (output_item.id_ > 0) {
		int ret_insert = pack_try_insert_item(PACK_T_RUNE_STONE_PACK_INDEX, output_item);
		if (ret_insert) {
			return ret_insert;
		}
	}
	if (output_item.id_) {
		pack_insert_item(PACK_T_RUNE_STONE_PACK_INDEX, output_item, Gain_Item_DM_Info(ITEM_GAIN_RUNE_STONE_EXCHANGE));
	}

	rune_stone_detail_->points -= item_cfg->rune_stone.need_points;

	rune_stone_detail_->detail_change();

	if (output_item.id_) {
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		Item_Basic_Info ibi;
		output_item.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	active_points();

	MSG_50103207 res_msg;
	res_msg.item_id = item_id;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Rune_Stone::req_rune_stone_essence_exchange(const int item_id) {
	if (! item_id) {
		return ERROR_CLIENT_PARAM;
	}
	if (0 != Item_Detail::is_item_type(item_id, Item_Detail::RUNE_STONE)) {
		return ERROR_CLIENT_OPERATE;
	}
	const Item_Detail_Config* item_cfg = CONFIG_CACHE_ITEM->find_item(item_id);
	if (! item_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	int need_essence = 0;
	int essence_id = 0;
	for (Id_Amount_Vec::const_iterator it = item_cfg->rune_stone.need_id.begin(); it != item_cfg->rune_stone.need_id.end(); ++it) {
		need_essence = (*it).amount;
		essence_id = (*it).id;
	}
	int pack_essence_amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,essence_id);
	int sub_essence = pack_essence_amount - need_essence;
	if (sub_essence < 0) {
		return ERROR_RUNE_STONE_ESSENCE_LACK;
	}

	Item_Detail output_item(item_id, 1, Item_Detail::BIND);
	if (output_item.id_ > 0) {
		int ret_insert = pack_try_insert_item(PACK_T_RUNE_STONE_PACK_INDEX, output_item);
		if (ret_insert) {
			return ret_insert;
		}
	}
	if (output_item.id_) {
		pack_insert_item(PACK_T_RUNE_STONE_PACK_INDEX, output_item, Gain_Item_DM_Info(ITEM_GAIN_RUNE_STONE_EXCHANGE));
	}

	pack_erase_item(PACK_T_PACKAGE_INDEX,Id_Amount(essence_id, need_essence),Lose_Item_DM_Info(ITEM_LOSE_RUNE_STONE_ESSENCE_EXCHANGE));
	pack_essence_amount -= need_essence;

	rune_stone_detail_->detail_change();

	if (output_item.id_) {
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		Item_Basic_Info ibi;
		output_item.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	active_essence();

	MSG_50103211 res_msg;
	res_msg.item_id = item_id;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Rune_Stone::req_points(void) {
	active_points();
	return 0;
}

int Rune_Stone::req_essence(void) {
	active_essence();
	return 0;
}

int Rune_Stone::req_rune_stone_on_off(const int index_from, const int index_to) {
	if (!index_from || !index_to) {
		return ERROR_CLIENT_PARAM;
	}

	Item_Detail *item_from = this->pack_get_item(index_from);
	if (! item_from) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (0 != item_from->is_item_type(item_from->id_, Item_Detail::RUNE_STONE)) {
		return ERROR_CLIENT_OPERATE;
	}

	/*
	 * 英雄穿脱特殊处理
	 */
	if (pack_is_hero_type(index_from) || pack_is_hero_type(index_to)) {
		return player_self()->req_hero_rune_stone_on_off(index_from, index_to);
	}
//	if (pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), index_from) ||
//			(pack_is_pack_type(index_from, Pack::PACK_T_PACKAGE_INDEX) == 0 && pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), index_to))) {
//
//	}

	/*
	 * 以下是主角穿脱
	 */

	/* 脱下 */
	if (pack_is_pack_type(index_from, PACK_T_RUNE_STONE_EQUIP_INDEX) == 0) {

		int ret_move = this->pack_move_item(index_from, PACK_T_RUNE_STONE_PACK_INDEX);
		if (ret_move) {
			return ret_move;
		}

	/* 穿上 */
	} else if (pack_is_pack_type(index_from, PACK_T_RUNE_STONE_PACK_INDEX) == 0 &&
			pack_is_pack_type(index_to, PACK_T_RUNE_STONE_EQUIP_INDEX) == 0) {

		const Item_Detail_Config *item_from_cfg = CONFIG_CACHE_ITEM->find_item(item_from->id_);
		if (!item_from_cfg || !item_from_cfg->rune_stone.rune_type) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (has_put_on_same_rune_type(PACK_T_RUNE_STONE_EQUIP_INDEX, item_from_cfg->rune_stone.rune_type)) {
			return ERROR_RUNE_STONE_PUT_ON_SAME_TYPE;
		}

		int ret_move = this->pack_move_item(index_from, index_to);
		if (ret_move) {
			return ret_move;
		}

		player_self()->set_item_bind(item_from);
	}

	rune_stone_prop_refresh();

	MSG_50103209 res_msg;
	res_msg.index_from = index_from;
	res_msg.index_to = index_to;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

void Rune_Stone::active_points(void) {
	MSG_81080000 active_msg;
	active_msg.poins = rune_stone_detail_->points;
	THIS_SEND_TO_CLIENT(active_msg);
}

void Rune_Stone::active_essence(void) {
	MSG_81090000 active_essence_msg;
	active_essence_msg.essence = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,21007001);
	THIS_SEND_TO_CLIENT(active_essence_msg);
}

void Rune_Stone::active_rune_stone_addit_info(const Item_Detail *item) {
	if (! item) {
		return;
	}
	MSG_82100004 active_msg;
	active_msg.type = PACK_TYPE;
	Item_Rune_Stone_Info addit_info;
	item->addon.rune_stone.make_item_rune_stone_info(addit_info);
	active_msg.rune_stone_addit.push_back(addit_info);
	active_msg.rune_stone_addit.rbegin()->index = item->index_;
	THIS_SEND_TO_CLIENT(active_msg);
}

void Rune_Stone::init_smelters(void) {
	if (rune_stone_detail_->smelter_detail_map.empty()) {
		Smelter_Detail smelter_detail;
		for (int smelter_idx = 1; smelter_idx < Rune_Stone_Detail::SMELTER_IDX_END; ++smelter_idx) {
			smelter_detail.reset();
			smelter_detail.smelter_idx = smelter_idx;
			smelter_detail.is_open = smelter_detail.smelter_idx == 1  ? 1 : 0;
			rune_stone_detail_->smelter_detail_map.insert(std::make_pair(smelter_detail.smelter_idx, smelter_detail));
		}
	}

	bool has_open = false;
	for (Smelter_Detail_Map::iterator it_smelter = rune_stone_detail_->smelter_detail_map.begin();
			it_smelter != rune_stone_detail_->smelter_detail_map.end(); ++it_smelter) {
		if (it_smelter->second.is_open) {
			has_open = true;
		}
	}
	if (! has_open) {
		rune_stone_detail_->smelter_detail_map[1].is_open = 1;
	}
}

void Rune_Stone::init_smelters(Rune_Stone_Detail &detail) {
	if (detail.smelter_detail_map.empty()) {
		Smelter_Detail smelter_detail;
		for (int smelter_idx = 1; smelter_idx < Rune_Stone_Detail::SMELTER_IDX_END; ++smelter_idx) {
			smelter_detail.reset();
			smelter_detail.smelter_idx = smelter_idx;
			detail.smelter_detail_map.insert(std::make_pair(smelter_detail.smelter_idx, smelter_detail));
		}
	}
}

void Rune_Stone::init_player_all_pack_cap(void) {
	// equip_cap
	update_rune_stone_equip_cap();

}

void Rune_Stone::update_rune_stone_equip_cap(void) {
	// equip_cap
	int now_cap_equip = pack_get_capacity(PACK_T_RUNE_STONE_EQUIP_INDEX);
	int std_cap_equip = std_equip_cap(player_self()->level());
	if (now_cap_equip < std_cap_equip) {
		this->pack_inner_set_cap(PACK_T_RUNE_STONE_EQUIP_INDEX, std_cap_equip);
	}

}

void Rune_Stone::update_rune_stone_hero_equip_cap(void) {
//	if (player_self()->heroer_detail().hero_map.empty()) {
//		return;
//	}
//	for (Heroer_Detail::Hero_Map::const_iterator it_hero = player_self()->heroer_detail().hero_map.begin();
//			it_hero != player_self()->heroer_detail().hero_map.end(); ++it_hero) {
//		// hero equip cap
//		int pack_type = pack_get_hero_pack_type(static_cast<Hero_Pack_Module>(HPM_RUNE_STONE_EQUIP_INDEX), it_hero->second.hero_index);
//		int now_cap_equip = pack_get_hero_capacity(PACK_T_RUNE_STONE_EQUIP_INDEX);
//		int std_cap_equip = std_equip_cap(player_self()->level());
//		if (now_cap_equip < std_cap_equip) {
//			this->pack_inner_set_cap(PACK_T_RUNE_STONE_EQUIP_INDEX, std_cap_equip);
//		}
//	}
}

int Rune_Stone::exp_lvl_prop_refresh(const Item_Detail_Config *item_cfg, const int exp_lvl, std::map<int, double> &pro_val_map) {
	if (!item_cfg || !exp_lvl) {
		return -1;
	}
	const Exp_Lvl_Cfg* exp_lvl_cfg = CONFIG_CACHE_RUNE_STONE->find_exp_lvl(exp_lvl);
	if (!exp_lvl_cfg || exp_lvl_cfg->prop_lvl_factor <= 0.0) {
		return -1;
	}
	const Int_Double_Map &cfg_pv_map = item_cfg->rune_stone.prop_val_map;
	if (! cfg_pv_map.empty()) {
		for (Int_Double_Map::const_iterator it_cfg_pv_map = cfg_pv_map.begin();
				it_cfg_pv_map != cfg_pv_map.end(); ++it_cfg_pv_map) {
			int prop = it_cfg_pv_map->first;
			double base_val = it_cfg_pv_map->second;
			double add_val = base_val * exp_lvl_cfg->prop_lvl_factor;
			if (prop) {
				pro_val_map[prop] += add_val;
			}
		}
	}
	return 0;
}

int Rune_Stone::combine_prop_refresh(const std::map<int, int> &combine_cnt_map, std::map<int, double> &pro_val_map) {
	if (combine_cnt_map.empty()) {
		return -1;
	}

	for (std::map<int, int>::const_iterator it = combine_cnt_map.begin(); it != combine_cnt_map.end(); ++it) {
		int combine_id = it->first;
		int combine_cnt = it->second;
		const Combine_Id_Cfg* combine_cfg = CONFIG_CACHE_RUNE_STONE->find_combine(combine_id);
		if (! combine_cfg) {
			continue;
		}

		// 从小到大叠加套装数对应属性，并非只找最大
		for (Combine_Count_Cfg_Map::const_iterator it_cfg_cnt = combine_cfg->count_cfg_map_.begin();
				it_cfg_cnt != combine_cfg->count_cfg_map_.end(); ++it_cfg_cnt) {
			if (combine_cnt < it_cfg_cnt->first) {
				break;
			}
			// prop
			for (Int_Double_Map::const_iterator it_cfg_prop = it_cfg_cnt->second.prop_val_map.begin();
					it_cfg_prop != it_cfg_cnt->second.prop_val_map.end(); ++it_cfg_prop) {
				int prop = it_cfg_prop->first;
				double val = it_cfg_prop->second;
				pro_val_map[prop] += val;
			}
		}
	}

	return 0;
}

int Rune_Stone::rune_stone_prop_refresh(void) {
	rune_stone_detail_->combine_cnt_map.clear();
	rune_stone_detail_->pro_val_map.clear();
	std::map<int, double> pro_val_map;
	Item_Detail *item = NULL;

	int idx_end = PACK_T_RUNE_STONE_EQUIP_INDEX + pack_get_capacity(PACK_T_RUNE_STONE_EQUIP_INDEX) + 1;
	for (int idx = PACK_T_RUNE_STONE_EQUIP_INDEX + RS_INNER_IDX_BEGIN; idx < idx_end; ++idx) {
		item = this->pack_get_item(idx);
		if (! item) {
			continue;
		}
		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
		if (! item_cfg) {
			continue;
		}

		// 获得组合件数
		if (item_cfg->rune_stone.combine_id) {
			++rune_stone_detail_->combine_cnt_map[item_cfg->rune_stone.combine_id];
		}

		// exp_lvl prop
		exp_lvl_prop_refresh(item_cfg, item->addon.rune_stone.exp_lvl, pro_val_map);
	}

	// 组合属性
	combine_prop_refresh(rune_stone_detail_->combine_cnt_map, pro_val_map);

	// 人物属性加成
	player_self()->addition_prop_refresh(AT_RUNE_STONE, PST_FIXED, pro_val_map, 262);

	rune_stone_detail_->pro_val_map = pro_val_map;

	return 0;
}

bool Rune_Stone::has_put_on_same_rune_type(const int pack_type, const int rune_type) {
	const Item_Detail *item = NULL;
	uint cap = pack_is_hero_type(pack_type) ?  pack_get_capacity((Pack_Type)pack_type) : pack_get_hero_capacity(pack_type);
	int idx_start = pack_type + RS_INNER_IDX_BEGIN;
	int idx_end = pack_type + cap + 1;
	for (int idx = idx_start; idx < idx_end; ++idx) {
		item = this->pack_get_const_item(idx);
		if (! item) {
			continue;
		}
		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
		if (! item_cfg) {
			continue;
		}
		if (item_cfg->rune_stone.rune_type == rune_type) {
			return true;
		}
	}
	return false;
}

int Rune_Stone::update_exp_lvl(Item_Detail *item) {
	if (! item) {
		return -1;
	}
	const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
	if (!item_cfg || !item_cfg->color) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Exp_Lvl_Cfg_Map& exp_lvl_cfg_map = CONFIG_CACHE_RUNE_STONE->exp_lvl_cfg_map();
	if (exp_lvl_cfg_map.empty()) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	int exp_lvl = item->addon.rune_stone.exp_lvl;
	for (Exp_Lvl_Cfg_Map::const_iterator it_cfg_exp_lvl_map = exp_lvl_cfg_map.begin();
			it_cfg_exp_lvl_map != exp_lvl_cfg_map.end(); ++it_cfg_exp_lvl_map) {
		std::map<int, int>::const_iterator it_cfg_color = it_cfg_exp_lvl_map->second.color_exp_map.find(item_cfg->color);
		if (it_cfg_color != it_cfg_exp_lvl_map->second.color_exp_map.end()) {
			if (item->addon.rune_stone.exp >= it_cfg_color->second) {
				exp_lvl = it_cfg_exp_lvl_map->first;
			} else {
				break;
			}
		}
	}

	if (exp_lvl != item->addon.rune_stone.exp_lvl) {
		item->addon.rune_stone.exp_lvl = exp_lvl;
		player_self()->achievement_listen_rune_stone_lvl(exp_lvl);
	}

	return 0;
}

int Rune_Stone::gm_add_points(const int points) {
	int total_points = rune_stone_detail_->points + points;
	rune_stone_detail_->points = total_points < 0  ?  0 : total_points;
	rune_stone_detail_->detail_change();
	active_points();
	return 0;
}

int Rune_Stone::get_rune_stone_prop(std::map<int, double> &pro_val_map) const {
	pro_val_map = rune_stone_detail_->pro_val_map;
	return 0;
}

int Rune_Stone::active_pack_rune_stone(void) {
	//
	return 0;
}

void Rune_Stone::get_rune_quality_num_map(std::map<int8_t, uint32_t> &rune_quality_num_map) {
	rune_quality_num_map.clear();
	// put on
	const Item_Detail *item = 0;
	int idx_end = PACK_T_RUNE_STONE_EQUIP_INDEX + pack_get_capacity(PACK_T_RUNE_STONE_EQUIP_INDEX) + 1;
	for (int idx = PACK_T_RUNE_STONE_EQUIP_INDEX + RS_INNER_IDX_BEGIN; idx < idx_end; ++idx) {
		item = this->pack_get_item(idx);
		if (! item) {
			continue;
		}
		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
		if (! item_cfg) {
			continue;
		}

		std::map<int8_t, uint32_t>::iterator find_it = rune_quality_num_map.find(item_cfg->color);
		if(find_it == rune_quality_num_map.end()) {
			rune_quality_num_map.insert(std::make_pair(item_cfg->color, (uint32_t)item->amount_));
		}
		else {
			find_it->second += (uint32_t)item->amount_;
		}
	}

	// pack
	idx_end = PACK_T_RUNE_STONE_PACK_INDEX + pack_get_capacity(PACK_T_RUNE_STONE_PACK_INDEX) + 1;
	for (int idx = PACK_T_RUNE_STONE_PACK_INDEX + RS_INNER_IDX_BEGIN; idx < idx_end; ++idx) {
		item = this->pack_get_item(idx);
		if (! item) {
			continue;
		}
		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
		if (! item_cfg) {
			continue;
		}

		std::map<int8_t, uint32_t>::iterator find_it = rune_quality_num_map.find(item_cfg->color);
		if(find_it == rune_quality_num_map.end()) {
			rune_quality_num_map.insert(std::make_pair(item_cfg->color, (uint32_t)item->amount_));
		}
		else {
			find_it->second += (uint32_t)item->amount_;
		}
	}

	//hero put on
	for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end();
			++it) {
		int pack_type = PACK_T_HERO_RUNE_STONE_EQUIP_INDEX + it->second.hero_index * ONE_HERO_EQUIP_GAP;
		uint cap = pack_is_hero_type(pack_type) ?  pack_get_hero_capacity(pack_type) :  pack_get_capacity((Pack_Type)pack_type);
		int idx_start = pack_type + RS_INNER_IDX_BEGIN;
		int idx_end = pack_type + cap + 1;

		for (int idx = idx_start; idx < idx_end; ++idx) {
			item = this->pack_get_const_item(idx);
			if (! item) {
				continue;
			}
			const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
			if (! item_cfg) {
				continue;
			}

			std::map<int8_t, uint32_t>::iterator find_it = rune_quality_num_map.find(item_cfg->color);
			if(find_it == rune_quality_num_map.end()) {
				rune_quality_num_map.insert(std::make_pair(item_cfg->color, (uint32_t)item->amount_));
			}
			else {
				find_it->second += (uint32_t)item->amount_;
			}
		}
	}
}
