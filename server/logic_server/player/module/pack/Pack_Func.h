/*
 * Pack_Func.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef PACK_FUNC_H_
#define PACK_FUNC_H_

#include "Err_Code.h"
#include "Pack_Struct.h"
#include "Packer.h"
#include "Msg_Packer_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Struct.h"

class Logic_Player;

// 与Packer成员无关的函数放在这里

void merge_same_item(const Item_Vec &item_vec_in, Item_Vec &item_vec_out);

inline void pack_insert_item_to_msg(const Item_Detail &item, MSG_50100100 &msg, Packer& packer) {
	Item_Basic_Info item_base;
	item_base.seq = item.seq_;
	item_base.index = item.index_;
	item_base.id = item.id_;
	item_base.bind = item.bind_;
	item_base.amount = item.amount_;
	item_base.overdue_time = item.overdue_time.sec();
	msg.item_info.push_back(item_base);

	switch (item.type_) {
	case Item_Detail::EQUIP: {
		Item_Equip_Info equip_info;
		item.addon.equip.make_item_equip_info(equip_info, packer);
		msg.equip_info.push_back(equip_info);
		msg.equip_info.rbegin()->index = item.index_;
		break;
	}
	case Item_Detail::RUNE_STONE: {
		Item_Rune_Stone_Info addon_info;
		item.addon.rune_stone.make_item_rune_stone_info(addon_info);
		msg.rune_stone_info.push_back(addon_info);
		msg.rune_stone_info.rbegin()->index = item.index_;
		break;
	}
	case Item_Detail::CARD: {
		Card_Info card_info;
		item.addon.card.make_item_card_info(item.id_, item.index_, card_info);
		msg.card_info.push_back(card_info);
		msg.card_info.rbegin()->index = item.index_;
		break;
	}
	default:
		//msg.item_info.push_back(item_base);
		break;
	}
}

inline void pack_insert_item_to_msg(const Item_Detail &item, MODULE_TYPE type, MSG_82100000 &item_msg, MSG_82100004 &addit_msg,  Packer &packer, int offset = 0) {
	Item_Basic_Info item_base;
	item_base.seq = item.seq_;
	item_base.index = item.index_;
	item_base.id = item.id_;
	item_base.bind = item.bind_;
	item_base.amount = item.amount_;
	item_base.overdue_time = item.overdue_time.sec();
	item_msg.item_info.push_back(item_base);

	int new_index = item.index_ + offset;
	switch (item.type_) {
	case Item_Detail::EQUIP: {
		addit_msg.type = type;
		Item_Equip_Info equip_info;
		item.addon.equip.make_item_equip_info(equip_info, packer);
		addit_msg.equip_addit.push_back(equip_info);
		addit_msg.equip_addit.rbegin()->index = new_index;
		break;
	}
	case Item_Detail::RUNE_STONE: {
		addit_msg.type = type;
		Item_Rune_Stone_Info addon_info;
		item.addon.rune_stone.make_item_rune_stone_info(addon_info);
		addit_msg.rune_stone_addit.push_back(addon_info);
		addit_msg.rune_stone_addit.rbegin()->index = new_index;
		break;
	}
//	case Item_Detail::CARD: {
//		addit_msg.type = type;
//		Card_Info addon_info;
//		item.addon.card.make_item_card_info(item.id_, item.index_, addon_info);
//		addon_info.index = item.index_;
//		addon_info.id = item.id_;
//		addit_msg.card_info_addit.push_back(addon_info);
//		addit_msg.card_info_addit.rbegin()->index = new_index;
//		break;
//	}
	default:
		break;
	}

	item_msg.item_info.rbegin()->index = new_index;
}

inline void pack_insert_item_to_msg(const Item_Detail &item, MSG_50100112 &res_msg, Packer &packer) {
	Item_Basic_Info item_base;
	item_base.seq = item.seq_;
	item_base.index = item.index_;
	item_base.id = item.id_;
	item_base.bind = item.bind_;
	item_base.amount = item.amount_;
	item_base.overdue_time = item.overdue_time.sec();
	res_msg.item_info.push_back(item_base);

	switch (item.type_) {
	case Item_Detail::EQUIP: {
		Item_Equip_Info equip_info;
		item.addon.equip.make_item_equip_info(equip_info, packer);
		res_msg.equip_info.push_back(equip_info);
		res_msg.equip_info.rbegin()->index = item.index_;
		break;
	}
	case Item_Detail::RUNE_STONE: {
		Item_Rune_Stone_Info addon_info;
		item.addon.rune_stone.make_item_rune_stone_info(addon_info);
		res_msg.rune_stone_info.push_back(addon_info);
		res_msg.rune_stone_info.rbegin()->index = item.index_;
		break;
	}
	default:
		//msg.item_info.push_back(item_base);
		break;
	}
}

inline void insert_get_item_tips(const Item_Detail &item, Logic_Player &player) {
//	player.push_tips(item.item_basic.id, item.item_basic.amount, GET_ITEM);
}

void pack_merge_item_array(std::vector<Item_Detail*> &item_array, Pack::Pack_Type pack_type,
		Pack::MERGE_WAY merge_way, UInt_Set &changed_set);
bool pack_merge_equal_item(Item_Detail &item_des, Item_Detail &item_src);
bool pack_merge_similar_item(Item_Detail &item_des, Item_Detail &item_src);// 用于背包整理
bool pack_merge_similar_item_move(Item_Detail &item_des, Item_Detail &item_src);// 用于道具移动

bool pack_is_req_item(const Item_Detail &item, uint32_t id, Bind_Verify bind_verify = BIND_NONE, const Item_Vertify_By_Condition con = CON_START);

int get_capacity_price(Pack::Pack_Type pack_type, int start, int end, int &price);
void get_capacity_super_item_nums(int start, int end, int pack_type, int &item_nums);

inline int get_sub_type_by_money_type(Pack::Money_Type money_type, Pack::Money_Sub_Type &sub_type) {
	switch (money_type) {
	case Pack::BIND_GOLD:
		sub_type = Pack::BIND_GOLD_FIRST;
		break;
	case Pack::GOLD:
		sub_type = Pack::GOLD_ONLY;
		break;
	case Pack::COPPER:
		sub_type = Pack::COPPER_ONLY;
		break;
	case Pack::DRAGON_SOUL:
		sub_type = Pack::DRAGON_SOUL_ONLY;
		break;
	case Pack::SOULS:
		sub_type = Pack::SOULS_ONLY;
		break;
	case Pack::FRIENDSHIP:
		sub_type = Pack::FRIENDSHIP_ONLY;
		break;
	case Pack::REPUTATION:
		sub_type = Pack::REPUTATION_ONLY;
		break;
	default:
		return ERROR_MONEY_TYPE_ILLEGAL;
	}
	return 0;
}

int get_item_color(const int item_id);

inline int get_full_error_code(const Pack_Type pack_type) {
	switch (pack_type) {
	case PACK_T_EQUIP_INDEX:
		return ERROR_INNER_SERVER;
	case PACK_T_PACKAGE_INDEX:
		return ERROR_PACK_FULL;
	case PACK_T_STORAGE_INDEX:
		return ERROR_STORAGE_FULL;
	case PACK_T_HERO_EQUIP_INDEX:
		return ERROR_INNER_SERVER;
	case PACK_T_RUNE_STONE_SMELT_INDEX:
		return ERROR_RUNE_STONE_SMELTER_FULL;
	case PACK_T_RUNE_STONE_PACK_INDEX:
		return ERROR_RUNE_STONE_PACK_FULL;
	case PACK_T_CARD_BOX_INDEX: {
		return ERROR_CARD_PACK_FULL;
	}
	default:
		return ERROR_INNER_SERVER;
	}
}

bool is_need_trade_pay_limit(const Money_Sub_Info &info);
bool is_need_trade_get_limit(const Money_Add_Info &info);

inline int is_item_type(const uint32_t item_id, Item_Detail::Item_Type item_type) {
	bool is_infer_type = false;
	if (Item_Detail::EQUIP_START <= item_id && item_id < Item_Detail::EQUIP_END) {
		is_infer_type = (item_type == Item_Detail::EQUIP);
	}
	if (Item_Detail::RUNE_STONE_START <= item_id && item_id < Item_Detail::RUNE_STONE_END) {
		is_infer_type = (item_type == Item_Detail::RUNE_STONE);
	}
	if (Item_Detail::CARD_START <= item_id && item_id < Item_Detail::CARD_END) {
		is_infer_type = (item_type == Item_Detail::CARD);
	}


	return is_infer_type? 0 : -1;
}
inline int get_sub_money_type(const int id) {

	if ( 501502 == id )
		return 1;
	else if( 501602 == id )
		return 3;
	else if( 501702 == id )
		return 4;
	else if ( 502803 == id )
		return 7;
	else if ( 502903 == id )
		return 5;
	else if ( 503403 == id )
		return 6;
	else if ( 503603 == id )
		return 8;
	return 0;
}

void make_item_vec(Item_Vec& item_vec, int32_t item_id, int32_t quantity, int8_t state);

int get_item_type_second(int id);

Money_Sub_Type get_money_sub_type(int id);

#endif /* PACK_FUNC_H_ */
