/*
 * Card.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: root
 */

#include "Card.h"
#include "Logic_Player.h"
#include "Card_Config.h"
#include "Card_Func.h"
#include "Config_Cache_Card.h"
#include "Config_Cache_Item.h"
#include "Err_Code.h"
#include "Msg_Struct.h"
#include "equiper/Equiper.h"
#include "equiper/Equiper_Def.h"
#include "pack/Packer.h"
#include "pack/Pack_Struct.h"
#include "Config_Cache_Item.h"
#include "Msg_Card_Struct.h"
#include "Msg_Active_Struct.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "ranking/Ranking_Manager.h"

Card::~Card(void) {
}

void Card::reset() {
}

void Card::module_init(void) {
	// fixed card info
	for (uint32_t idx = PACK_T_CARD_BOX_INDEX+1; idx <= PACK_T_CARD_BOX_INDEX + 1000; ++idx) {
		Item_Detail *card = pack_get_item(idx);
		if (0 == card) continue;
		if (Item_Detail::CARD != card->type_) continue;
		if (0 == card->addon.card.equip_id) continue;

		const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(card->addon.card.equip_id);
		if (0 == cfg) return;

		bool is_exist = false;
		// 遍历英雄装备
		if (4 == cfg->equip_cfg.career || 0 == cfg->equip_cfg.career) {
			for (Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
					it != player_self()->heroer_detail().hero_map.end(); ++it) {
				int hero_equip_index = PACK_T_HERO_EQUIP_INDEX + it->first * ONE_HERO_EQUIP_GAP;
				for (int index = hero_equip_index+EQUIP_BEGIN; index<hero_equip_index+EQUIP_END; ++index) {
					Item_Detail* item = pack_get_item(index);
					if (0 == item) continue;
					for (int i=0; i<Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
						if (item->addon.equip.card_hole_array_[i].card_index == card->index_) {
							is_exist = true;
							break;
						}
					}
					if (is_exist) break;
				}
				if (is_exist) break;
			}
		}
		if (is_exist) continue;

		// 遍历身上装备
		if (4 != cfg->equip_cfg.career) {
			for (int index=PACK_T_EQUIP_INDEX+EQUIP_BEGIN; index<PACK_T_EQUIP_INDEX+EQUIP_END; ++index) {
				Item_Detail* item = pack_get_item(index);
				if (0 == item) continue;
				for (int i=0; i<Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
					if (item->addon.equip.card_hole_array_[i].card_index == card->index_) {
						is_exist = true;
						break;
					}
				}
				if (is_exist) break;
			}
		}
		if (is_exist) continue;

		// 遍历背包装备
		int cap = pack_get_capacity(PACK_T_PACKAGE_INDEX);
		for (int index = PACK_T_PACKAGE_INDEX +1;
				index <= PACK_T_PACKAGE_INDEX+cap; ++index) {
			Item_Detail* item = pack_get_item(index);
			if (0 == item) continue;
			if (Item_Detail::EQUIP != item ->type_) continue;
			for (int i=0; i<Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
				if (item->addon.equip.card_hole_array_[i].card_index == card->index_){
					is_exist = true;
					break;
				}
			}
			if (is_exist) break;
		}
		if (is_exist) continue;

		card->addon.card.equip_id = 0;
		card->addon.card.equip_loc = 0;
	}
}

int Card::handle_embed_card(uint32_t from_index, uint32_t to_index, uint8_t loc, uint32_t card_index) {

	if (from_index == 0 && to_index == 0) return ERROR_CLIENT_PARAM;
	if (from_index != 0 && to_index != 0 && from_index != to_index) return ERROR_CLIENT_PARAM;
	if (to_index != 0 && loc == 0) return ERROR_CLIENT_PARAM;
	if (to_index == 0 && loc != 0) return ERROR_CLIENT_PARAM;

	int ret_val = 0;
	if (to_index == 0 && loc == 0) {
		ret_val = unembed_card(from_index, to_index, loc, card_index);
	} else {
		ret_val= embed_card(from_index, to_index, loc, card_index);
	}

	return ret_val;
}

int Card::embed_card(uint32_t from_index, uint32_t to_index, uint8_t loc, uint32_t card_index) {

	// 检查是否有此物品
	Item_Detail *equip = pack_get_item(to_index);
	if (0 == equip) return ERROR_INNER_SERVER;

	// 检查是否装备
	if (Item_Detail::EQUIP != equip->type_) return ERROR_CLIENT_PARAM;

	if (Hole_State::STATE_NOT_OPEN == equip->addon.equip.card_hole_array_[loc-1].state)
		return ERROR_CLIENT_PARAM;

	// 检查是否有此卡
	Item_Detail *card = pack_get_item(card_index);
	if (0 == card) return ERROR_CLIENT_PARAM;

	// 检查是否该卡能够镶嵌到该装备
	if (card->addon.card.part_ != player_self()->get_part_by_index(to_index))
		return ERROR_INNER_SERVER;

	if (0 == from_index && card->addon.card.equip_id != 0)
		return ERROR_CARD_HAS_EMBEDED;

	const Card_Config *cfg = CONFIG_CACHE_CARD->get_card_config(card->id_);
	if (0 == cfg) return ERROR_INNER_SERVER;

	// 经验卡不能镶嵌
	if (EXP_CARD_TYPE == cfg->type) return ERROR_CLIENT_PARAM;

	const Item_Detail_Config *equip_cfg = CONFIG_CACHE_ITEM->find_item(equip->id_);
	if (0 == equip_cfg) return ERROR_INNER_SERVER;

	// 装备等级必须大于等待卡牌类型
	if (equip_cfg->equip_cfg.equip_lvl < cfg->type) return ERROR_CLIENT_PARAM;
	if (equip_cfg->equip_cfg.card_holes < loc) return ERROR_CLIENT_PARAM;

	uint8_t old_loc = card->addon.card.equip_loc;
	if (old_loc > Equip_Addon_Detail::MAX_CARD_HOLE) return ERROR_INNER_SERVER;

	// 检查是否拖放位置不变
	if (card->addon.card.equip_id == equip->id_ &&
			card->addon.card.equip_loc == loc &&
			equip->addon.equip.card_hole_array_[old_loc-1].card_index == card->index_)
		return ERROR_CLIENT_PARAM;

	// 检查新位置是否有卡
	Item_Detail *old_card = 0;
	if (equip->addon.equip.card_hole_array_[loc-1].card_index != 0) {
		old_card = pack_get_item(equip->addon.equip.card_hole_array_[loc-1].card_index);
	}

	// 切换位置
	if (0 != old_card) {
		old_card->addon.card.equip_id = card->addon.card.equip_id;
		old_card->addon.card.equip_loc = card->addon.card.equip_loc;
		if (old_card->addon.card.equip_id == equip->id_) {
			equip->addon.equip.card_hole_array_[card->addon.card.equip_loc-1].card_index =
					old_card->index_;
		}
	} else if (0 != from_index) { // 清除位置
		if (card->addon.card.equip_id == equip->id_) {
			equip->addon.equip.card_hole_array_[card->addon.card.equip_loc-1].card_index = 0;
		}
	}

	card->addon.card.equip_id = equip->id_;
	card->addon.card.equip_loc = loc;
	equip->addon.equip.card_hole_array_[loc-1].card_index = card->index_;

	// 更新前端装备部分信息
	player_self()->active_item_addit_info_update(equip);

	if (from_index != to_index) {
		call_prop_refresh(to_index, 10001);
	}

	MSG_50109002 msg;
	msg.result = 0;
	msg.embed_type = 1;
	Card_Info info;
	if (0 != old_card) {
		old_card->addon.card.make_item_card_info(old_card->id_, old_card->index_, info);
		msg.card_info_vec.push_back(info);
	}

	card->addon.card.make_item_card_info(card->id_, card->index_, info);
	msg.card_info_vec.push_back(info);
	THIS_SEND_TO_CLIENT(msg);

	//task  listen
	player_self()->task_listen_equip_card();
	RANKING_MANAGER->monitor_equipment_change(player_self()->role_id(), equip);
	return 0;
}

int Card::unembed_card(uint32_t from_index, uint32_t to_index, uint8_t loc, uint32_t card_index) {

	if (0 == from_index)
		return ERROR_CLIENT_PARAM;

	Item_Detail *equip = pack_get_item(from_index);
	if (0 == equip)
		return ERROR_INNER_SERVER;

	// 检查是否有此卡牌
	Item_Detail *card = pack_get_item(card_index);
	if (0 == card)
		return ERROR_CLIENT_PARAM;

	int old_loc = card->addon.card.equip_loc;
	if (card->addon.card.equip_id != equip->id_)
		return ERROR_INNER_SERVER;

	if (old_loc > Equip_Addon_Detail::MAX_CARD_HOLE)
		return ERROR_INNER_SERVER;

	if (equip->addon.equip.card_hole_array_[old_loc-1].card_index != card->index_)
		return ERROR_INNER_SERVER;

	equip->addon.equip.card_hole_array_[old_loc-1].card_index = 0;
	player_self()->active_item_addit_info_update(equip);

	card->addon.card.equip_id = 0;
	card->addon.card.equip_loc = 0;

	// 刷新属性
	call_prop_refresh(from_index, 10002);

	MSG_50109002 msg;
	msg.result = 0;
	msg.embed_type = 2;
	Card_Info info;
	card->addon.card.make_item_card_info(card->id_, card->index_, info);
	msg.card_info_vec.push_back(info);
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Card::handle_upgrade(uint32_t card_id, std::vector<uint32_t> &card_id_vec) {

	Item_Detail *card = pack_get_item(card_id);
	if (0 == card) return ERROR_CLIENT_PARAM;

	const Card_Config* cfg = CONFIG_CACHE_CARD->get_card_config(card->id_);
	if (0 == cfg) return ERROR_INNER_SERVER;

	int max_lv = CONFIG_CACHE_CARD->get_max_lv_by_role_lv(this->level());
	max_lv = max_lv > cfg->max_lv ? cfg->max_lv : max_lv;
	if (card->addon.card.lv_ >= max_lv) return ERROR_INNER_SERVER;

	std::vector<Item_Detail*> card_vec;
	int ttl_exp = 0;
	int coin_cost = 0;
	for (std::vector<uint32_t>::iterator it = card_id_vec.begin(); it != card_id_vec.end(); ++it) {
		Item_Detail *exp_card = pack_get_item(*it);
		if (0 == exp_card) return ERROR_CLIENT_PARAM;
		// 检查是否卡牌
		if (Item_Detail::CARD != exp_card->type_) return ERROR_CLIENT_PARAM;
		// 检查是否镶嵌在装备上
		if (0 != exp_card->addon.card.equip_id) return ERROR_INNER_SERVER;

		const Card_Config *exp_cfg = CONFIG_CACHE_CARD->get_card_config(exp_card->id_);
		if (0 == exp_cfg) return ERROR_INNER_SERVER;

		int qlt = CONFIG_CACHE_CARD->get_quality_id(exp_card->addon.card.poten_);
		const Card_Lv_Data* lv_data = CONFIG_CACHE_CARD->get_card_lv_data(qlt, exp_cfg->type, exp_card->addon.card.lv_);
		if (0 == lv_data) return ERROR_INNER_SERVER;

		int his_exp = handle_get_ttl_exp(exp_card->addon.card.lv_, qlt, exp_cfg->type);
		ttl_exp += floor((his_exp + exp_card->addon.card.exp_) * 0.7) + lv_data->init_exp;
		coin_cost += lv_data->coin;

		card_vec.push_back(exp_card);
	}

	Money_Sub_Info money_sub_info(Money_Sub_Type(COPPER_ONLY), coin_cost, MONEY_SUB_CARD_UPGRADE);
	int ret = player_self()->pack_try_sub_money(money_sub_info);
	if (0 != ret) return ret;

	player_self()->pack_sub_money(money_sub_info);

	// 删除旧卡牌
	pack_erase_item(card_id_vec, Lose_Item_DM_Info(ITEM_LOSE_CARD_UPGRADE));

	// 加经验
	card->addon.card.exp_ += ttl_exp;
	int old_lv = card->addon.card.lv_;
	// 升级处理
	int qlt = CONFIG_CACHE_CARD->get_quality_id(card->addon.card.poten_);
	int type = cfg->type;
	for (int i = card->addon.card.lv_; i < max_lv; ++i) {
		const Card_Lv_Data* lv_data = CONFIG_CACHE_CARD->get_card_lv_data(
				qlt, type, i);
		if (0 == lv_data) break;

		if (card->addon.card.exp_ >= lv_data->exp) {
			card->addon.card.exp_ -= lv_data->exp;
			++card->addon.card.lv_;
		} else {
			break;
		}
	}

	check_and_refresh_prop(old_lv, card->addon.card.lv_, card->index_, card->addon.card.equip_id);

	MSG_50109003 msg;
	msg.result = 0;
	card->addon.card.make_item_card_info(card->id_, card->index_, msg.card_info);
	THIS_SEND_TO_CLIENT(msg);

	//task  listen
	player_self()->task_listen_upgrade_card();
	player_self()->task_listen_equip_card();
	return 0;
}

void Card::call_prop_refresh(uint32_t equip_index, int from) {

	Pack_Type pack_type = PACK_T_HERO_EQUIP_INDEX;
	pack_get_pack_type(equip_index, pack_type);
	if (pack_type == PACK_T_EQUIP_INDEX) {
		player_self()->equiper_prop_refresh(10050);
		return;
	}

	// hero equip index
	int index = player_self()->get_hero_index_by_pack_index(equip_index);
	if (index > 0) {
		player_self()->heroer_prop_refresh(index, from);
	}
}

void Card::card_prop_refresh(const Item_Detail &equip, std::map<int, double> &prop_map) {

	if (Item_Detail::EQUIP != equip.type_) return;
	for (int i=0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
		int idx = equip.addon.equip.card_hole_array_[i].card_index;
		if (0 == idx) continue;

		Item_Detail* card = pack_get_item(idx);
		if (0 == card || Item_Detail::CARD != card->type_) continue;

		int attr_id = card->addon.card.main_attr_;
		if (0 != attr_id) {
			double value = handle_get_attr_value(card->id_, attr_id,
					card->addon.card.star_, card->addon.card.lv_, card->addon.card.poten_);
			prop_map[attr_id] += value;
		}
		attr_id = card->addon.card.sub_attr_;
		if (0 != attr_id) {
			double value = handle_get_attr_value(card->id_, attr_id,
					card->addon.card.star_, card->addon.card.lv_, card->addon.card.poten_);
			prop_map[attr_id] += value;
		}

	}
}

void Card::check_and_refresh_prop(int pre_lv, int pos_lv, uint32_t card_idx, int item_id) {

	if (0 == item_id) return;

	const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(item_id);
	if (0 == cfg) return;

	if (4 == cfg->equip_cfg.career || 0 == cfg->equip_cfg.career) {			// 英雄处理
		for (Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
				it != player_self()->heroer_detail().hero_map.end(); ++it) {
			int hero_equip_index = PACK_T_HERO_EQUIP_INDEX + it->first * ONE_HERO_EQUIP_GAP;
			for (int idx = hero_equip_index + EQUIP_BEGIN; idx < hero_equip_index + EQUIP_END; ++idx) {
				Item_Detail* item = pack_get_item(idx);
				if (0 == item) continue;
				for (int i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
					if (item->addon.equip.card_hole_array_[i].card_index != card_idx) continue;
					if (pre_lv < pos_lv)
						player_self()->heroer_prop_refresh(it->second.hero_index);
					player_self()->active_item_addit_info_update(item);
					return;
				}
			}
		}
	}

	if (4 != cfg->equip_cfg.career) {		 // 主角处理
		for (int idx = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; idx < PACK_T_EQUIP_INDEX + EQUIP_END; ++idx) {
			Item_Detail* item = pack_get_item(idx);
			if (0 == item) continue;
			for (int i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
				if (item->addon.equip.card_hole_array_[i].card_index != card_idx) continue;
				if (pre_lv < pos_lv)
					player_self()->equiper_prop_refresh(10051);
				player_self()->active_item_addit_info_update(item);
				RANKING_MANAGER->monitor_equipment_change(player_self()->role_id(), item);
				return;
			}
		}
	}

	// 处理既不在人物，也不在英雄身上的情况
	int cap = pack_get_capacity(PACK_T_PACKAGE_INDEX);
	for (int idx = PACK_T_PACKAGE_INDEX+1; idx <= PACK_T_PACKAGE_INDEX+cap; ++idx) {
		Item_Detail* item = pack_get_item(idx);
		if (0 == item) continue;
		if (item->type_ != Item_Detail::EQUIP) continue;
		for (int i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
			if (item->addon.equip.card_hole_array_[i].card_index != card_idx) continue;
			player_self()->active_item_addit_info_update(item);
			return;
		}
	}
}

void Card::get_all_embed_card_prop(std::map<int, double> &prop_map) {

	prop_map.clear();

	for (int idx = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; idx < PACK_T_EQUIP_INDEX + EQUIP_END; ++idx) {
		Item_Detail* item = pack_get_item(idx);
		if (0 == item) continue;
		card_prop_refresh(*item, prop_map);
	}
	int out_idx = player_self()->get_hero_out_index();
	if (0 == out_idx) return;
	int hero_equip_index = PACK_T_HERO_EQUIP_INDEX + out_idx * ONE_HERO_EQUIP_GAP;
	for (int idx = hero_equip_index + EQUIP_BEGIN; idx < hero_equip_index + EQUIP_END; ++idx) {
		Item_Detail* item = pack_get_item(idx);
		if (0 == item) continue;
		card_prop_refresh(*item, prop_map);
	}
}

void Card::reset_card_location(const Item_Detail& equip) {

	if (Item_Detail::EQUIP != equip.type_) return;
	MSG_50109004 msg;
	for (int i=0; i<Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
		int card_id = equip.addon.equip.card_hole_array_[i].card_index;
		if (0 == card_id) continue;
		Item_Detail *card = pack_get_item(card_id);
		if (0 == card || Item_Detail::CARD != card->type_) continue;
		card->addon.card.equip_id = 0;
		card->addon.card.equip_loc = 0;
		Card_Info card_info;
		card->addon.card.make_item_card_info(card->id_, card->index_, card_info);
		msg.card_info_vec.push_back(card_info);
	}

	if (!msg.card_info_vec.empty()) {
		THIS_SEND_TO_CLIENT(msg);
	}
}

int Card::handle_sell_card(int type, int card_index) {
	if (type == 1 && 0 != card_index) {
		return sell_one_card(card_index);
	} else if (type == 2) {
		return sell_cards_by_one_clicked(card_index);
	}

	return ERROR_CLIENT_PARAM;
}

int Card::sell_one_card(int card_index) {

	Item_Detail* card = pack_get_item(card_index);
	if (0 == card) return ERROR_CARD_HAD_SOLD;
	if (Item_Detail::CARD != card->type_) return ERROR_CLIENT_PARAM;
	if (0 != card->addon.card.equip_id || 0 != card->addon.card.equip_loc)
		return ERROR_INNER_SERVER;

	int qlt = CONFIG_CACHE_CARD->get_quality_id(card->addon.card.poten_);
	const Card_Config *cfg_card = CONFIG_CACHE_CARD->get_card_config(card->id_);
	if (0 == cfg_card) return ERROR_CONFIG_NOT_EXIST;
	const Card_Lv_Data* lv_data = CONFIG_CACHE_CARD->get_card_lv_data(qlt, cfg_card->type,
			card->addon.card.lv_);
	if (0 == lv_data) return ERROR_CONFIG_NOT_EXIST;

	Money_Add_Info add_money(Money_Type::COPPER, lv_data->price, MONEY_ADD_SELL_CARD);
	int try_add_money_ret =  pack_try_add_money(add_money);
	if (0 != try_add_money_ret) return try_add_money_ret;

	Index_Amount index_amount(card->index_, card->amount_);
	int erase_item_ret = pack_erase_item(index_amount, Lose_Item_DM_Info(ITEM_LOSE_SELL_CARD));
	if (0 != erase_item_ret) return erase_item_ret;

	pack_add_money(add_money);
	MSG_81000102 item_msg;
	item_msg.property.push_back(Property(Money_Type::COPPER, add_money.nums));
	THIS_SEND_TO_CLIENT(item_msg);

	MSG_50109005 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Card::sell_cards_by_one_clicked(int card_qlt) {

	Index_Amount_Vec index_amount_vec;
	int price_ttl = 0;
	int start_index = PACK_T_CARD_BOX_INDEX + 1;
	int end_index = PACK_T_CARD_BOX_INDEX + CARD_BOX_MAX_CAPACITY;
	for (int i = start_index; i < end_index; ++i) {
		Item_Detail* card = pack_get_item(i);
		price_ttl += check_one_card_qlt(card_qlt, card, index_amount_vec);
	}

	if (index_amount_vec.empty()) return ERROR_NO_SUITABLE_CARD;

	Money_Add_Info add_money(Money_Type::COPPER, price_ttl, MONEY_ADD_SELL_CARD);
	int try_add_money_ret =  pack_try_add_money(add_money);
	if (0 != try_add_money_ret) return try_add_money_ret;

	int erase_item_ret = pack_erase_item(index_amount_vec,
			Lose_Item_DM_Info(ITEM_LOSE_SELL_CARD));
	if (0 != erase_item_ret) return erase_item_ret;

	pack_add_money(add_money);
	MSG_81000102 item_msg;
	item_msg.property.push_back(Property(Money_Type::COPPER, add_money.nums));
	THIS_SEND_TO_CLIENT(item_msg);

	MSG_50109005 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Card::check_one_card_qlt(int qlt, Item_Detail* card, Index_Amount_Vec & index_amount_vec) {


	const static int EXP_CARD_TYPE = 1000;
	if (0 == card)  return 0;
	if (Item_Detail::CARD != card->type_) return 0;
	if (0 != card->addon.card.equip_id || 0 != card->addon.card.equip_loc) return 0;

	int card_qlt = CONFIG_CACHE_CARD->get_quality_id(card->addon.card.poten_);
	if (card_qlt != qlt) return 0;
	if (card->addon.card.exp_ != 0 || card->addon.card.lv_ > 1) return 0;

	const Card_Config *cfg_card = CONFIG_CACHE_CARD->get_card_config(card->id_);
	if (0 == cfg_card) {
		LOG_TRACE_ABORT("connot find config, card_id = %d", card->id_);
		return 0;
	}

	if (EXP_CARD_TYPE == cfg_card->type) return 0;

	const Card_Lv_Data* lv_data = CONFIG_CACHE_CARD->get_card_lv_data(card_qlt, cfg_card->type,
			card->addon.card.lv_);
	if (0 == lv_data) {
		LOG_TRACE_ABORT("cannot find lv data config, card_qlt=%d, card_type=%d, card_lv=%d",
				card_qlt, cfg_card->type, card->addon.card.lv_);
		return 0;
	}

	index_amount_vec.push_back(Index_Amount(card->index_, card->amount_));
	return lv_data->price;
}

int Card::handle_open_card_hole(int equip_index, int8_t loc) {

	Item_Detail *equip = pack_get_item(equip_index);
	if (0 == equip)
		return ERROR_INNER_SERVER;
	if (Item_Detail::EQUIP != equip->type_)
		return ERROR_CLIENT_PARAM;

	const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(equip->id_);
	if (0 == cfg)
		return ERROR_CONFIG_NOT_EXIST;

	if (loc > cfg->equip_cfg.card_holes)
		return ERROR_CLIENT_PARAM;

	if (Hole_State::STATE_OPEN == equip->addon.equip.card_hole_array_[loc-1].state)
		return ERROR_CLIENT_PARAM;

	const int open_hole_item_id = CONFIG_CACHE_ITEM->get_gemstone_open_hole_item_id();
	if (0 == open_hole_item_id)
		return ERROR_CONFIG_NOT_EXIST;
	Id_Amount item_cost(open_hole_item_id, 1);
	int ret_val = pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost,
			Lose_Item_DM_Info(Pack::ITEM_LOSE_CARD_OPEN_HOLE));
	if (0 != ret_val) {
//		MSG_82100008 msg;
//		msg.item_ids.push_back(open_hole_item_id);
//		THIS_SEND_TO_CLIENT(msg);
		return 0;
	}

	equip->addon.equip.card_hole_array_[loc-1].state = Hole_State::STATE_OPEN;
	player_self()->active_item_addit_info_update(equip);

	MSG_50109006 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Card::handle_merge_new_card(uint32_t card_index, const std::vector<uint32_t> &card_index_vec) {

	if (2 != card_index_vec.size())
		return ERROR_CLIENT_PARAM;

	Item_Detail *card = pack_get_item(card_index);
	if (0 == card) return ERROR_INNER_SERVER;
	if (Item_Detail::CARD != card->type_) return ERROR_INNER_SERVER;
	if (card->addon.card.lv_ < 10) return ERROR_INNER_SERVER;

	const Card_Config *cfg_card = CONFIG_CACHE_CARD->get_card_config(card->id_);
	if (0 == cfg_card) return ERROR_CONFIG_NOT_EXIST;

	int card_name_type = card->id_ % 1000;
	int type = cfg_card->type;
	int qlt = CONFIG_CACHE_CARD->get_quality_id(card->addon.card.poten_);
	int star = card->addon.card.star_;
	const Card_Lv_Data* cfg_upgrade = CONFIG_CACHE_CARD->get_card_lv_data(qlt, cfg_card->type,
			card->addon.card.lv_);
	if (0 == cfg_upgrade) return ERROR_CONFIG_NOT_EXIST;
	const Card_Poten_Interval *cfg_poten = CONFIG_CACHE_CARD->get_card_poten_interval(qlt);
	if (0 == cfg_poten) return ERROR_CONFIG_NOT_EXIST;

	if (is_card_max_type(type)) return ERROR_INNER_SERVER;

	int id_delta =  get_next_id_dif_by_type(type);
	const Card_Config *cfg_target_card = CONFIG_CACHE_CARD->get_card_config(card->id_ + id_delta);
	if (0 == cfg_target_card) return ERROR_CONFIG_NOT_EXIST;

	Index_Amount_Vec cost_vec;
	for (std::vector<uint32_t>::const_iterator it = card_index_vec.begin();
			it != card_index_vec.end(); ++it) {

		if (card_index == *it) return ERROR_CLIENT_PARAM;
		Item_Detail *stuff = pack_get_item(*it);
		if (0 == stuff) return ERROR_CLIENT_PARAM;
		if (Item_Detail::CARD != stuff->type_) return ERROR_CLIENT_PARAM;
		const Card_Config *cfg_stuff = CONFIG_CACHE_CARD->get_card_config(stuff->id_);
		if (0 == cfg_stuff) return ERROR_CONFIG_NOT_EXIST;
		if (0 != stuff->addon.card.equip_loc && 0 != stuff->addon.card.equip_id)
			return ERROR_CLIENT_PARAM;

		int card_name_type_stuff = stuff->id_ % 1000;
		int type_stuff = cfg_stuff->type;
		int qlt_stuff = CONFIG_CACHE_CARD->get_quality_id(stuff->addon.card.poten_);
		int star_stuff = stuff->addon.card.star_;

		if (card_name_type != card_name_type_stuff) return ERROR_INNER_SERVER;
		if (qlt != qlt_stuff) return ERROR_INNER_SERVER;
		if (type != type_stuff) return ERROR_INNER_SERVER;
		if (star != star_stuff) return ERROR_INNER_SERVER;
		cost_vec.push_back(Index_Amount(*it, 1));
	}

	int ret_val = pack_try_erase_item(cost_vec);
	if (0 != ret_val) return ret_val;
	Money_Sub_Info money_sub_info(Money_Sub_Type(COPPER_ONLY), cfg_upgrade->merge_cost,
			MONEY_SUB_CARD_MERGE);

	ret_val = pack_sub_money(money_sub_info);
	if (0 != ret_val) return ret_val;

	pack_erase_item(cost_vec, Lose_Item_DM_Info(Pack::ITEM_LOSE_CARD_MERGE));

	MSG_50109007 msg;

	int merge_rand = random() % 1000;
	if (merge_rand >= cfg_upgrade->merge_rate) { // fail
		msg.result = 1;
		card->addon.card.make_item_card_info(card->id_, card->index_, msg.card_info);
	} else { // success
		msg.result = 0;

//		card->id_ += get_next_id_dif_by_type(type);
		card->id_ += id_delta;

		int merge_poten_rand = random() % 1000;
		if ( merge_poten_rand < cfg_poten->merge_poten_rate) {
			int start = cfg_poten->merge_potent_interval.val_1;
			int end = cfg_poten->merge_potent_interval.val_2;
			int poten_rand = random() % (end -start);
			card->addon.card.poten_ += (start + poten_rand);
		}
		card->addon.card.make_item_card_info(card->id_, card->index_, msg.card_info);

		// 更新装备
		check_and_refresh_prop(1, 2, card->index_, card->addon.card.equip_id);
		player_self()->active_content_listen_card_fusion(cfg_target_card->type);//开服活动卡牌升级监听
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Card::handle_get_vip_upgrade_times(void) {

	MSG_50109008 msg;
	msg.times = player_self()->get_vip_upgrade_card();

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Card::handle_vip_upgrade_card(uint32_t card_index) {

	Item_Detail *card = pack_get_item(card_index);
	if (0 == card) return ERROR_CLIENT_PARAM;
	if (Item_Detail::CARD != card->type_) return ERROR_CLIENT_PARAM;

	const Card_Config *cfg_card = CONFIG_CACHE_CARD->get_card_config(card->id_);
	if (0 == cfg_card) return ERROR_CONFIG_NOT_EXIST;
	int max_lv = CONFIG_CACHE_CARD->get_max_lv_by_role_lv(this->level());
	max_lv = max_lv > cfg_card->max_lv ? cfg_card->max_lv : max_lv;
	if (card->addon.card.lv_ >= max_lv) return ERROR_INNER_SERVER;

	int vip = player_self()->vip();
	int max_use_times = CONFIG_CACHE_CARD->get_vip_max_upgrade_times(vip);
	int use_times = player_self()->get_vip_upgrade_card();

	if (use_times >= max_use_times) return ERROR_VIP_UPGRADE_CARD_TIMES_LIMIT;

	++use_times;

	const Int_Int* cfg_cost_exp = CONFIG_CACHE_CARD->get_vip_upgrade_value(use_times);
	if (0 == cfg_cost_exp) return ERROR_CONFIG_NOT_EXIST;

	Money_Sub_Info money_sub_info(Money_Sub_Type(BIND_GOLD_FIRST), cfg_cost_exp->val_1,
			MONEY_SUB_CARD_UPGRADE);
	int ret = player_self()->pack_sub_money(money_sub_info);
	if (0 != ret) return ret;

	card->addon.card.exp_ += cfg_cost_exp->val_2;
	int old_lv = card->addon.card.lv_;

	int qlt = CONFIG_CACHE_CARD->get_quality_id(card->addon.card.poten_);
	int type = cfg_card->type;
	for (int i = card->addon.card.lv_; i < max_lv; ++i) {
		const Card_Lv_Data* lv_data = CONFIG_CACHE_CARD->get_card_lv_data(qlt, type, i);
		if (0 == lv_data) break;

		if (card->addon.card.exp_ >= lv_data->exp) {
			card->addon.card.exp_ -= lv_data->exp;
			++card->addon.card.lv_;
		} else {
			break;
		}
 	}
	check_and_refresh_prop(old_lv, card->addon.card.lv_, card->index_, card->addon.card.equip_id);
	player_self()->set_vip_upgrade_card(use_times);

	MSG_50109009 msg;
	msg.result = 0;
	msg.vip_times = player_self()->get_vip_upgrade_card();
	card->addon.card.make_item_card_info(card->id_, card->index_, msg.card_info);

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

const static int POTEN_GOLD = 7;

int Card::handle_card_poten_upgrade(uint32_t card_index) {

	Item_Detail *card = pack_get_item(card_index);
	if (0 == card) return ERROR_INNER_SERVER;
	if (Item_Detail::CARD != card->type_) return ERROR_CLIENT_PARAM;
	int qlt = CONFIG_CACHE_CARD->get_quality_id(card->addon.card.poten_);
	if (qlt >= POTEN_GOLD) return ERROR_INNER_SERVER;
	const Card_Poten_Interval* cfg = CONFIG_CACHE_CARD->get_card_poten_interval(qlt);
	if (0 == cfg) return ERROR_CONFIG_NOT_EXIST;

	Id_Amount_Vec id_amount_vec;
	for (Item_Cost_Vec::const_iterator it = cfg->item_cost_vec.begin();
			it != cfg->item_cost_vec.end(); ++it) {
		id_amount_vec.push_back(Id_Amount(it->id, it->amount, Bind_Verify(it->bind)));
	}

	int ret = this->pack_try_erase_item(PACK_T_PACKAGE_INDEX, id_amount_vec);
	if (0 != ret) return ret;
	Money_Sub_Info money_sub_info(COPPER_ONLY, cfg->copper_cost,
			MONEY_SBU_CARD_POTEN_UPGRADE);
	ret = this->pack_try_sub_money(money_sub_info);
	if (0 != ret) return ret;

	const Poten_Upgrade_Config_Vec &poten_vec =
			CONFIG_CACHE_CARD->get_poten_upgrade_config_vec();
	int ttl_weight = 0;
	for (Poten_Upgrade_Config_Vec::const_iterator it = poten_vec.begin(); it != poten_vec.end(); ++it) {

		ttl_weight += it->weight;
	}
	if (0 == ttl_weight) return ERROR_CONFIG_NOT_EXIST;
	int rand = random() % ttl_weight;

	this->pack_sub_money(money_sub_info);
	this->pack_erase_item(PACK_T_PACKAGE_INDEX, id_amount_vec,
			Lose_Item_DM_Info(ITEM_LOSE_CARD_POTEN_UPGRADE));

	for (Poten_Upgrade_Config_Vec::const_iterator it = poten_vec.begin(); it != poten_vec.end(); ++it) {

		if (rand >= it->weight) {
			rand -= it->weight;
			continue;
		}

		int delta = it->max - it->min +1;
		card->addon.card.poten_ = random() % delta + it->min;
		break;
	}

	check_and_refresh_prop(0, card->addon.card.lv_, card->index_, card->addon.card.equip_id);

	MSG_50109010 msg;
	msg.result = 0;
	card->addon.card.make_item_card_info(card->id_, card->index_, msg.card);
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}
