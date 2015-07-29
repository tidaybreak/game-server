/*
 * Equiper_Ext.cpp
 *
 *  Created on: 2015年1月15日
 *      Author: root
 */

#include "Logic_Player.h"
#include "Config_Cache_Item.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "Equiper_Def.h"
#include "Equiper_Struct.h"
#include "Equiper.h"
#include "Msg_Equip_Struct.h"
#include "Config_Cache_Equip.h"
#include "Fighter_Detail.h"
#include "Err_Code.h"
#include "pack/Pack_Func.h"
#include "pack/Pack_Def.h"

// 装备开宝石孔
int Equiper::handle_open_equip_gemstone_hole(int equip_id, int index) {

	if (index >= Equip_Addon_Detail::MAX_GEMSTONE_HOLE) return ERROR_INNER_SERVER;
	Item_Detail *equip = pack_get_item(equip_id);
	if (0 == equip) return ERROR_INNER_SERVER;
	if (Item_Detail::EQUIP != equip->type_) return ERROR_NOT_EQUIP_ITEM;

	if (equip->addon.equip.gemstones[index].state == Hole_State::STATE_OPEN)
		return ERROR_HOLE_HAVE_OPENED;

	const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(equip->id_);
	if (0 == cfg) return ERROR_CONFIG_NOT_EXIST;
//	const Int_Int* value = CONFIG_CACHE_ITEM->get_gemstone_hole_info(cfg->color);
//	if (0 == value) return ERROR_CONFIG_NOT_EXIST;
	if (index >= cfg->equip_cfg.gemstone_holes.val_2) {
		this->send_tip_to_client(ERROR_HOLE_NUM_LIMIT, cfg->equip_cfg.gemstone_holes.val_2);
		return 0;
	}
	int ttl_opened_hole = 0;
	for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
		if (Hole_State::STATE_NOT_OPEN == equip->addon.equip.gemstones[i].state) continue;
		++ ttl_opened_hole;
	}
	if (ttl_opened_hole >= cfg->equip_cfg.gemstone_holes.val_2) {
		this->send_tip_to_client(ERROR_HOLE_NUM_LIMIT, cfg->equip_cfg.gemstone_holes.val_2);
		return 0;
	}

	// 消耗东西
	const int open_hole_item_id = CONFIG_CACHE_ITEM->get_gemstone_open_hole_item_id();
	if (0 == open_hole_item_id) return ERROR_CONFIG_NOT_EXIST;
	Id_Amount item_cost(open_hole_item_id, 1);
	int ret_val = pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost,
			Lose_Item_DM_Info(Pack::ITEM_LOSE_GEMSTONE_OPEN_HOLE));
	if (0 != ret_val) {
//		MSG_82100008 msg;
//		msg.item_ids.push_back(open_hole_item_id);
//		THIS_SEND_TO_CLIENT(msg);
		return 0;
	}

	equip->addon.equip.gemstones[index].state = Hole_State::STATE_OPEN;

	active_item_addit_info_update(equip);

	MSG_50101009 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

// 宝石镶嵌装备
int Equiper::handle_embed_gemstone(int equip_index, int index, int gemstone_id) {

	Item_Detail *equip = pack_get_item(equip_index);
	if (0 == equip) return ERROR_INNER_SERVER;
	if (Item_Detail::EQUIP != equip->type_) return ERROR_CLIENT_PARAM;

	if (index < 0 || index >=Equip_Addon_Detail::MAX_GEMSTONE_HOLE)
		return ERROR_CLIENT_PARAM;

	if (Hole_State::STATE_NOT_OPEN == equip->addon.equip.gemstones[index].state)
		return ERROR_HOLE_NOT_OPEN;

	if (gemstone_id == equip->addon.equip.gemstones[index].stone_id)
		return ERROR_EMBED_SAME_GEMSTONE;

	const Item_Detail_Config* gemstone_cfg = CONFIG_CACHE_ITEM->find_item(gemstone_id);
	if (0 == gemstone_cfg) return ERROR_CLIENT_PARAM;
	if (Item_Detail::TOOL_GEMSTONE != get_item_type_second(gemstone_cfg->item_id))
		return ERROR_NOT_GEMSTONE_TYPE;

	for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
		if (i == index) continue;
		int& test_id = equip->addon.equip.gemstones[i].stone_id;
		if (0 == test_id) continue;
		const Item_Detail_Config* test_gemstone = CONFIG_CACHE_ITEM->find_item(test_id);
		if (0 == test_gemstone) return ERROR_CONFIG_NOT_EXIST;
		if (Item_Detail::TOOL_GEMSTONE != get_item_type_second(test_gemstone->item_id))
			return ERROR_NOT_GEMSTONE_TYPE;
		if (test_gemstone->gemstone_cfg.type == gemstone_cfg->gemstone_cfg.type)
			return ERROR_SAME_GEMSTONE_TYPE;
	}

	if (0 != equip->addon.equip.gemstones[index].stone_id) {
		int ret_val = unembed_gemstone(equip, index);
		if (0 != ret_val) return ret_val;
	}

	Id_Amount gemstone(gemstone_id, 1);
	int ret_val = pack_erase_item(PACK_T_PACKAGE_INDEX, gemstone,
			Lose_Item_DM_Info(Pack::ITEM_LOSE_GEMSTONE_EMBED));

	if (0 != ret_val) return ret_val;

	equip->addon.equip.gemstones[index].stone_id = gemstone_id;
	equip->bind_ = Item_Detail::BIND;

	// refresh attr
	refresh_prop_when_gemstone_change(equip->index_);

	active_item_addit_info_update(equip);

	// return msg
	MSG_50101007 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	// 通知任务系统
	player_self()->task_listen_inlay_gemstone();
	player_self()->achievement_listen_rune();
	return 0;
}

// 卸装宝石
int Equiper::handle_unembed_gemstone(int equip_id, int index) {

	Item_Detail *equip = pack_get_item(equip_id);
	if (0 == equip) return ERROR_INNER_SERVER;
	if (Item_Detail::EQUIP != equip->type_) return ERROR_CLIENT_PARAM;

	int ret = unembed_gemstone(equip, index);

	// refresh attr
	refresh_prop_when_gemstone_change(equip->index_);

	active_item_addit_info_update(equip);

	// return msg
	MSG_50101008 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	player_self()->achievement_listen_rune();
	return ret;
}

// 宝石转化
int Equiper::handle_gemstone_convert(int src_gemstone_id, int des_gemstone_id, int num) {

	if (0 == src_gemstone_id || 0 == des_gemstone_id) return ERROR_CLIENT_PARAM;
	if (src_gemstone_id == des_gemstone_id) return ERROR_CLIENT_PARAM;

	const Item_Detail_Config *src_cfg = CONFIG_CACHE_ITEM->find_item(src_gemstone_id);
	const Item_Detail_Config *des_cfg = CONFIG_CACHE_ITEM->find_item(des_gemstone_id);
	if (0 == src_cfg || 0 == des_cfg) return ERROR_INNER_SERVER;
	if (Item_Detail::TOOL_GEMSTONE != get_item_type_second(src_cfg->item_id) ||
			Item_Detail::TOOL_GEMSTONE != get_item_type_second(des_cfg->item_id))
		return ERROR_SAME_GEMSTONE_TYPE;

	// judge leve
	if (src_cfg->gemstone_cfg.lv != des_cfg->gemstone_cfg.lv) return ERROR_NOT_SAME_LV;

	Id_Amount item_cost(src_gemstone_id, num);
	int ret_val = pack_try_erase_item(PACK_T_PACKAGE_INDEX, item_cost);
	if (0 != ret_val) return ret_val;

	Item_Detail new_item(des_gemstone_id, num, Item_Detail::BIND);
	ret_val = this->pack_try_insert_item(PACK_T_PACKAGE_INDEX, new_item);
	if (0 != ret_val) return ret_val;

	const int cost = CONFIG_CACHE_ITEM->get_gemstone_conversion_cost(src_cfg->gemstone_cfg.lv);
	Money_Sub_Info money_sub_info(Money_Sub_Type(COPPER_ONLY), cost * num,
			MONEY_SUB_GEMSTONE_CONVERT);
	ret_val = this->pack_sub_money(money_sub_info);
	if (0 != ret_val) return ret_val;

	pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost,
			Lose_Item_DM_Info(Pack::ITEM_LOSE_GEMSTONE_CONVERT));
	pack_insert_item(PACK_T_PACKAGE_INDEX, new_item,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_GEMSTONE_CONVERT));

	// return msg
	MSG_50101010 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

// 获取宝石合成信息
int Equiper::handle_gem_enable_info(void) {
	MSG_50101011 res_msg;
	Block_Buffer res_buf;
	res_buf.make_message(res_msg.msg_id);

	Int_Hash_Set::iterator iter;
	for(iter = equiper_detail_->gem_data.begin(); iter != equiper_detail_->gem_data.end(); iter++){
		res_msg.gem_id.push_back(*iter);
	}

	res_msg.serialize(res_buf);
	res_buf.finish_message();
	this->send_to_client(res_buf);
	return 0;
}

// 卸装宝石
int Equiper::unembed_gemstone(Item_Detail *equip, int index) {

	if (0 == equip) return ERROR_INNER_SERVER;
	if (index < 0 || index >= Equip_Addon_Detail::MAX_GEMSTONE_HOLE)
		return ERROR_CLIENT_PARAM;
	int gemstone_id = equip->addon.equip.gemstones[index].stone_id;
	if (0 == gemstone_id) return ERROR_INNER_SERVER;

	const Item_Detail_Config * gemstone = CONFIG_CACHE_ITEM->find_item(
			equip->addon.equip.gemstones[index].stone_id);
	if (0 == gemstone) return ERROR_CONFIG_NOT_EXIST;

	const int cost = CONFIG_CACHE_ITEM->get_gemstone_umembed_cost();
	Money_Sub_Info money_sub_info(Money_Sub_Type(COPPER_ONLY), cost,
			MONEY_SUB_GEMSTONE_UNEMBED);
	int ret_val = pack_try_sub_money(money_sub_info);
	if (0 != ret_val) return ret_val;

	Item_Detail new_gemstone_item(gemstone_id, 1, Item_Detail::BIND);
	ret_val = pack_insert_item(PACK_T_PACKAGE_INDEX,  new_gemstone_item,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_GEMSTONE_UNEMBED));
	if (0 != ret_val) return ret_val;

	pack_sub_money(money_sub_info);
	equip->addon.equip.gemstones[index].stone_id = 0;

	return 0;
}

void Equiper::refresh_prop_when_gemstone_change(int equip_index) {
	int role_equip_index_start = PACK_T_EQUIP_INDEX + EQUIP_BEGIN;
	int role_equip_index_end = PACK_T_EQUIP_INDEX + EQUIP_END;
	if (equip_index >= role_equip_index_start && equip_index <= role_equip_index_end) {
		equiper_prop_refresh(10008);
		return;
	}

	for (Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end(); ++it) {
		int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX +
				it->second.hero_index * ONE_HERO_EQUIP_GAP;
		int hero_equip_index_start = hero_pack_idx_gap + EQUIP_BEGIN;
		int hero_equip_index_end = hero_pack_idx_gap + EQUIP_END;
		if (equip_index >= hero_equip_index_start && equip_index <= hero_equip_index_end) {
			player_self()->heroer_prop_refresh(it->second.hero_index);
			return;
		}
	}
}

void Equiper::gemstone_prop_refresh(const Item_Detail &equip,
		std::map<int, double> &prop_map) {

	if (Item_Detail::EQUIP != equip.type_) return;

	const Gem_Carve_Prop *gemstone_carve_propo = CONFIG_CACHE_ITEM->get_gemstone_carve_propo(equiper_detail_->carve_lvl);

	for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
		int id = equip.addon.equip.gemstones[i].stone_id;
		if (0 == id) continue;
		const Item_Detail_Config* cfg = CONFIG_CACHE_ITEM->find_item(id);
		if (0 == cfg) {
			MSG_DEBUG("role:%ld, cannot find item cfg, item_id=%d", role_id(), id);
			continue;
		}
		if (get_item_type_second(cfg->item_id) != Item_Detail::TOOL_GEMSTONE) continue;
		prop_map[cfg->gemstone_cfg.attr.id] += cfg->gemstone_cfg.attr.value * gemstone_carve_propo->gem_stats;
	}
}

void Equiper::gemstone_suit_prop_refresh(const Int_Int_Map &color_suit_map,
		std::map<int, double> &prop_map) {

	const Gemstone_Ext_Attr_Config& ext_attr_cfg = CONFIG_CACHE_ITEM->get_gemstone_ext_attr();
	for (Gemstone_Ext_Attr_Config::const_iterator it = ext_attr_cfg.begin();
			it != ext_attr_cfg.end(); ++it) {
		int color = it->color;
		int num_ttl = 0;
		for (Int_Int_Map::const_iterator color_it = color_suit_map.begin();
				color_it != color_suit_map.end(); ++color_it) {
			if (color_it->first < color) continue;
			num_ttl += color_it->second;
		}
		if (num_ttl < it->num) continue;
		for (Int_Double_Map::const_iterator prop_it = it->attr_map.begin();
				prop_it != it->attr_map.end(); ++prop_it) {
			prop_map[prop_it->first] += prop_it->second;
		}
	}
}

void Equiper::calc_genstone(const Item_Detail& equip, Int_Int_Map &color_suit_map) {

	if (Item_Detail::EQUIP != equip.type_) return;

	for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
		int id = equip.addon.equip.gemstones[i].stone_id;
		if (0 == id) continue;
		const Item_Detail_Config* cfg = CONFIG_CACHE_ITEM->find_item(id);
		if (0 == cfg) {
			MSG_DEBUG("role:%ld, cannot find item cfg, item_id=%d", role_id(), id);
			continue;
		}
		if (get_item_type_second(cfg->item_id) != Item_Detail::TOOL_GEMSTONE) continue;
		color_suit_map[cfg->gemstone_cfg.lv] += 1;
	}
}

int Equiper::get_gemstone_count(int lv) {

	int num_ttl = 0;
	int index_start = PACK_T_EQUIP_INDEX + EQUIP_BEGIN;
	int index_end = PACK_T_EQUIP_INDEX + EQUIP_END;
	num_ttl += get_gemstone_count(index_start, index_end, lv);

	for (Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end(); ++it) {
		int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX +
				it->second.hero_index * ONE_HERO_EQUIP_GAP;
		int index_start = hero_pack_idx_gap + EQUIP_BEGIN;
		int index_end = hero_pack_idx_gap + EQUIP_END;
		num_ttl += get_gemstone_count(index_start, index_end, lv);
	}

	return num_ttl;
}

int Equiper::get_gemstone_count(int start_index, int end_index, int lv) {

	int num_ttl = 0;
	for (int index = start_index; index < end_index; ++index) {
		Item_Detail *equip = pack_get_item(index);
		if (0 == equip || equip->type_ != Item_Detail::EQUIP) continue;
		for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
			int gemstone_id = equip->addon.equip.gemstones[i].stone_id;
			if (0 == gemstone_id) continue;
			const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(gemstone_id);
			if (0 == cfg) {
				LOG_TRACE_ABORT("gemstone item cannot be found, id =%d", gemstone_id);
				continue;
			}
			if (cfg->gemstone_cfg.lv < lv) continue;
			++num_ttl;
		}
	}
	return num_ttl;
}

int Equiper::set_gem_compose_id(int gem_id){
	equiper_detail_->gem_data.insert(gem_id);

	MSG_80100316 res_msg;
	Block_Buffer res_buf;
	res_buf.make_message(res_msg.msg_id);

	res_msg.form = gem_id;

	res_msg.serialize(res_buf);
	res_buf.finish_message();
	this->send_to_client(res_buf);
	return 0;
}

void Equiper::get_all_embed_gemstone_prop(std::map<int, double> &prop_map) {

	int index_start = PACK_T_EQUIP_INDEX + EQUIP_BEGIN;
	int index_end = PACK_T_EQUIP_INDEX + EQUIP_END;
	get_all_embed_gemstone_prop(index_start, index_end, prop_map);

	for (Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end(); ++it) {
		int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX +
				it->second.hero_index * ONE_HERO_EQUIP_GAP;
		int index_start = hero_pack_idx_gap + EQUIP_BEGIN;
		int index_end = hero_pack_idx_gap + EQUIP_END;
		get_all_embed_gemstone_prop(index_start, index_end, prop_map);
	}
}

void Equiper::get_all_embed_gemstone_prop(int start_index, int end_index,
		std::map<int, double> &prop_map) {

	for (int index = start_index; index < end_index; ++index) {
		Item_Detail *equip = pack_get_item(index);
		if (0 == equip || equip->type_ != Item_Detail::EQUIP) continue;
		for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
			int gemstone_id = equip->addon.equip.gemstones[i].stone_id;
			if (0 == gemstone_id) continue;
			const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(gemstone_id);
			if (0 == cfg) {
				LOG_TRACE_ABORT("gemstone item cannot be found, id =%d", gemstone_id);
				continue;
			}
			prop_map[cfg->gemstone_cfg.attr.id] +=cfg->gemstone_cfg.attr.value;
		}
	}
}

bool Equiper::is_equip_embed_gemstone(const Item_Detail &equip) {

	if (Item_Detail::EQUIP != equip.type_) return false;
	for (int i=0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
		if (0 == equip.addon.equip.gemstones[i].stone_id) continue;
		return true;
	}
	return false;
}

int Equiper::req_gem_carve_info(int essence_num) {
	MSG_50101014 msg;
	if (essence_num <= 0) {
		return -1;
	}
	const uint32_t MAX_LVL = CONFIG_CACHE_ITEM->get_gemstone_carve_propo_max();
	if (equiper_detail_->carve_lvl >= MAX_LVL)
		return 0;

	const Gem_Carve_Prop *gemstone_carve_propo = CONFIG_CACHE_ITEM->get_gemstone_carve_propo(equiper_detail_->carve_lvl + 1);
	if (NULL == gemstone_carve_propo) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	int essence_id = gemstone_carve_propo->item_base.val_1;
	int carve_base = gemstone_carve_propo->item_base.val_2;
	uint32_t ess_num = essence_num * carve_base;
	if (equiper_detail_->gem_progress + ess_num >= gemstone_carve_propo->carve_max) {
		equiper_detail_->gem_progress = equiper_detail_->gem_progress + ess_num - gemstone_carve_propo->carve_max;
		equiper_detail_->carve_lvl += 1;
		equiper_prop_refresh(10050);
		player_self()->gem_carve_announce();

		while(equiper_detail_->carve_lvl < MAX_LVL) {
			const Gem_Carve_Prop *gemstone_carve_prop = CONFIG_CACHE_ITEM->get_gemstone_carve_propo(equiper_detail_->carve_lvl + 1);
				if (NULL == gemstone_carve_prop) {
					return ERROR_CONFIG_NOT_EXIST;
				}
			if (equiper_detail_->gem_progress >= gemstone_carve_prop->carve_max) {
				equiper_detail_->gem_progress = equiper_detail_->gem_progress  - gemstone_carve_prop->carve_max;
				equiper_detail_->carve_lvl += 1;
				equiper_prop_refresh(10060);
				player_self()->gem_carve_announce();
			}
			if (equiper_detail_->carve_lvl == MAX_LVL){
				break;
				}
			const Gem_Carve_Prop *gemstone_carve_pro = CONFIG_CACHE_ITEM->get_gemstone_carve_propo(equiper_detail_->carve_lvl + 1);
			if (NULL == gemstone_carve_pro) {
				return ERROR_CONFIG_NOT_EXIST;
				}
			if (equiper_detail_->gem_progress < gemstone_carve_pro->carve_max){
				break;
			}
		}
	} else {
		equiper_detail_->gem_progress += ess_num;
	}
	int ret = 0;
	if(equiper_detail_->carve_lvl == MAX_LVL) {
		ret = pack_erase_item(PACK_T_PACKAGE_INDEX,Id_Amount(essence_id, ess_num -equiper_detail_->gem_progress),Lose_Item_DM_Info(ITEM_LOSE_GEMSTONE_CARVE));
	}else {
		ret = pack_erase_item(PACK_T_PACKAGE_INDEX,Id_Amount(essence_id, ess_num),Lose_Item_DM_Info(ITEM_LOSE_GEMSTONE_CARVE));
	}
	if (0 != ret) {
		return -1;
	}
	msg.carve_lvl = equiper_detail_->carve_lvl;
	msg.gem_progress = equiper_detail_->gem_progress;
	if(equiper_detail_->carve_lvl == MAX_LVL) {
		msg.ess_num = ess_num -equiper_detail_->gem_progress;
	}else {
		msg.ess_num = ess_num;
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Equiper::req_gem_carve_lvl_info(void) {
	MSG_50101015 res_msg;
	res_msg.carve_lvl = equiper_detail_->carve_lvl;
	res_msg.gem_progress = equiper_detail_->gem_progress;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Equiper::gem_compose_announce(int gem_id) {
	Equip_Compose_Id_Cost::const_iterator it_cost = CONFIG_CACHE_EQUIP->level_up_id_cost_map().find(gem_id);
	if(it_cost->second.broadcast_id == 9012) {
		player_self()->create_ann_gem_four(player_self(),it_cost->second.name);
	}if(it_cost->second.broadcast_id == 9013) {
		player_self()->create_ann_gem_five(player_self(),it_cost->second.name);
	}if(it_cost->second.broadcast_id == 9014) {
		player_self()->create_ann_gold_syn(player_self(),it_cost->second.name);
	}
	return 0;
}

int Equiper::gem_carve_announce(void) {
	const Gem_Carve_Prop *gemstone_carve_propo = CONFIG_CACHE_ITEM->get_gemstone_carve_propo(equiper_detail_->carve_lvl);
	if(gemstone_carve_propo->broadcast_id == 9015) {
		player_self()->create_ann_gem_carve_ten(player_self());
	} if(gemstone_carve_propo->broadcast_id == 9016) {
		player_self()->create_ann_gem_carve_twenty(player_self());
	} if(gemstone_carve_propo->broadcast_id == 9017) {
		player_self()->create_ann_gem_carve_thirty(player_self());
	}
	return 0;
}
