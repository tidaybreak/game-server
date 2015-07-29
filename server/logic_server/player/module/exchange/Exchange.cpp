/*
 * Exchange.cpp
 *
 *  Created on: 2014年6月26日
 *      Author: Linqiyou
 */

#include "Err_Code.h"
#include "Msg_Exchange_Struct.h"
#include "Msg_Active_Struct.h"
#include "Exchange_Def.h"
#include "Exchange_Struct.h"
#include "pack/Pack_Struct.h"
#include "pack/Packer.h"
#include "Exchange.h"
#include "Config_Cache_Exchange.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Role.h"
#include "Logic_Player.h"
#include "public/Public_Struct.h"
#include "pack/Pack_Func.h"
#include "Msg_Struct.h"

Exchange::Exchange(void) {
	reset();
}

Exchange::~Exchange(void) {
}

void Exchange::reset(void) {
	exchange_detail_ = 0;
}

int Exchange::create_init(Exchange_Detail &detail) {
	return 0;
}

void Exchange::load_detail(Exchange_Detail &detail) {
	exchange_detail_ = &detail;
}

void Exchange::save_detail(Exchange_Detail &detail) {
	if (exchange_detail().is_change) {
		detail = exchange_detail();
		exchange_detail_->is_change = false;
	}
}

void Exchange::module_init(void) {
}

void Exchange::sign_in(void) {
}

void Exchange::sign_in_repeat(void) {
}

void Exchange::client_ready_now(void) {
}

//	每天0点触发
int Exchange::trigger_daily_zero(const Time_Value &now, bool logic_check) {
	return 0;
}
// 每天6点触发
int Exchange::trigger_daily_six(const Time_Value& now, bool logic_check) {
	exchange_detail_->ex_data_every_day.clear();
	exchange_detail_->detail_change();
	return 0;
}
void Exchange::sign_out(void) {
}

int Exchange::move_scene_completed(void) {
	return 0;
}

int Exchange::sync_scene_module_info(Block_Buffer &buf) {
	return 0;
}

void Exchange::trusteeship_sign_out(void) {
}

// msg
int Exchange::req_exchange(const int32_t &item_id, const int16_t &type, int32_t amount) {
	if (item_id <= 0 || amount <= 0) {
		return ERROR_CLIENT_OPERATE;
	}

	const Exchange_Config_Detail *cfg = CONFIG_CACHE_EXCHANGE->find_cache(type, item_id);
	if (!cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	if (amount <= cfg->stack) {
		amount = cfg->stack;
	}

	if (cfg->reputation != 0 && cfg->reputation > player_self()->public_detail().exploit_lvl) {
		return ERROR_EXCHANGE_REPUTATION_LIMIT;
	}

	if (0 != cfg->limit) {
		int daily_amount = amount;
		Int_Int_Map::iterator it = exchange_detail_->ex_data_every_day.find(item_id);
		if (it != exchange_detail_->ex_data_every_day.end()) {
			daily_amount += it->second;
		}
		if (daily_amount > cfg->limit) {
			return send_tip_to_client(ERROR_EXCHANGE_DAILY_LIMIT, cfg->limit);
		}
	}

	MSG_81000102 res_msg;
	res_msg.type = 0;

	Id_Amount_Vec ecg_all;
	Id_Amount ecg;
	Item_Vec pack_item_vec;
	int friendship = 0;
	int reputation = 0;

	ecg.id = cfg->exchange_type;
	ecg.amount = cfg->amount * amount;
	ecg_all.push_back(ecg);
	make_item_vec(pack_item_vec, cfg->id, amount, cfg->bind);
	friendship += cfg->amount;
	reputation += cfg->amount;
	Item_Basic_Info item_base;
	Card_Info card_info;
	for (Item_Vec::const_iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it) {
		item_base.reset();
		(*it).set_item_basic_info(item_base);
		res_msg.item_info.push_back(item_base);
		card_info.reset();
		if ((*it).set_item_card_info(card_info)) {
			res_msg.card_info_vec.push_back(card_info);
		}
	}

	switch(type) {
	case EXCHANGE_TYPE_EQUIP:
	case EXCHANGE_TYPE_MEDAL:
	case EXCHANGE_TYPE_ITEM: {
		int ret = Packer::pack_try_erase_item(Pack_Type::PACK_T_PACKAGE_INDEX, ecg_all);
		if (ret != 0) {
			return ret;
		}
		ret = Packer::pack_erase_item(PACK_T_PACKAGE_INDEX,
							ecg_all, Lose_Item_DM_Info(ITEM_LOSE_EXCHANGE));
		if (ret != 0) {
			return ret;
		}
		break;
	}
	case EXCHANGE_TYPE_FRIEND: {
		int ret = pack_try_sub_money(Money_Sub_Info(FRIENDSHIP_ONLY, friendship * amount, MONEY_SUB_EXCHANGE));
		if (ret) {
			return ret;
		}
		ret = pack_sub_money(Money_Sub_Info(FRIENDSHIP_ONLY, friendship * amount, MONEY_SUB_EXCHANGE));
		if(ret != 0) {
			return ret;
		}
		break;
	}
	case EXCHANGE_TYPE_ACTIVITY: {
		int ret = pack_try_sub_money(Money_Sub_Info(REPUTATION_ONLY, reputation * amount, MONEY_SUB_EXCHANGE));
		if (ret) {
			return ret;
		}
		ret = pack_sub_money(Money_Sub_Info(REPUTATION_ONLY, reputation * amount, MONEY_SUB_EXCHANGE));
		if(ret != 0) {
			return ret;
		}
		break;
	}
	default:
		break;
	}

	int ret = Packer::pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
						pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_EXCHANGE));
	if (0 == ret && (!res_msg.item_info.empty())) {
		THIS_SEND_TO_CLIENT(res_msg);
	}
	MSG_50100500 msg;
	if (0 == ret) {
		msg.item_id = item_id;
		Int_Int_Map::iterator it = exchange_detail_->ex_data.find(item_id);
		if (it != exchange_detail_->ex_data.end()) {
			it->second += amount;
		} else {
			exchange_detail_->ex_data.insert(std::make_pair(item_id, amount));
		}
		it = exchange_detail_->ex_data_every_day.find(item_id);
		if (it != exchange_detail_->ex_data_every_day.end()) {
			it->second += amount;
			msg.amount = it->second;
		} else {
			exchange_detail_->ex_data_every_day.insert(std::make_pair(item_id, amount));
			msg.amount = amount;
		}
		exchange_detail_->detail_change();
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}
int Exchange::req_fetch_exchange_info(const int16_t type) {
	const Exchange_Config *cfg = CONFIG_CACHE_EXCHANGE->find_cache(type);
	const Int_Vec *lvl_cfg = CONFIG_CACHE_EXCHANGE->find_lvl(type);
//	const Int_IntSet_Map_Reverse *lvl_gt_cfg = CONFIG_CACHE_EXCHANGE->find_lvl_greater(type);

	if (!cfg || !lvl_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
//	int32_t lvl = level();
//	if (lvl < 25) {
//		lvl = (lvl / 10) * 10;
//	} else {
//		lvl = ((lvl - 5) / 10) * 10 + 5;
//	}
	int car = career();

	MSG_50100501 msg;
	msg.type = type;
	Exchange_Info_Msg info;
	switch(type) {
	case EXCHANGE_TYPE_EQUIP: {
		Int_Int_Std_Map equip_master;
		Int_Int_Std_Map equip_hero;
		Int_Int_Std_Map equip_common;
		Int_Int_Std_Map equip_other;
//		for (Int_IntSet_Map_Reverse::const_iterator it = lvl_cfg->begin(); it != lvl_cfg->end(); ++it) {
//			if (it->first != lvl) {
//				continue;
//			}
			for (Int_Vec::const_iterator it_s = lvl_cfg->begin(); it_s != lvl_cfg->end(); ++it_s) {
				Exchange_Config::const_iterator it_e = cfg->find(*it_s);
				if (it_e == cfg->end()) {
					continue;
				}
				const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(it_e->second.id);
				if (!item_cfg
						|| (item_cfg->equip_cfg.equip_lvl > 0
								&& item_cfg->equip_cfg.career != EX_CAREER_ALL
								&& item_cfg->equip_cfg.career != EX_CAREER_HERO
								&& item_cfg->equip_cfg.career != car)) {
//					if (item_cfg && item_cfg->equip_cfg.equip_lvl == 0) {
//						equip_other.insert(std::make_pair(*it_s, it_e->second.amount));
//					}
					continue;
				}
				info.reset();
				info.id = *it_s;
				Int_Int_Map::const_iterator it_ex = exchange_detail_->ex_data_every_day.find(info.id);
				if (it_ex != exchange_detail_->ex_data_every_day.end()) {
					info.amount_left = it_ex->second;
				}
				info.amount_daily = it_e->second.limit;
				msg.exchange_item_vector.push_back(info);
//				if (item_cfg->equip_cfg.career == car) {
//					equip_master.insert(std::make_pair(*it_s, it_e->second.amount));
//				} else if (item_cfg->equip_cfg.career == EX_CAREER_HERO) {
//					equip_hero.insert(std::make_pair(*it_s, it_e->second.amount));
//				} else if (item_cfg->equip_cfg.career == EX_CAREER_ALL) {
//					equip_common.insert(std::make_pair(*it_s, it_e->second.amount));
//				}
			}
//			break;
//		}
//		for (Int_Int_Std_Map::const_iterator it_em = equip_master.begin(); it_em != equip_master.end(); ++it_em) {
//			info.reset();
//			info.val_1 = it_em->first;
//			info.val_2 = it_em->second;
//			msg.exchange_item_vector.push_back(info);
//		}
//		for (Int_Int_Std_Map::const_iterator it_eh = equip_hero.begin(); it_eh != equip_hero.end(); ++it_eh) {
//			info.reset();
//			info.val_1 = it_eh->first;
//			info.val_2 = it_eh->second;
//			msg.exchange_item_vector.push_back(info);
//		}
//		for (Int_Int_Std_Map::const_iterator it_ec = equip_common.begin(); it_ec != equip_common.end(); ++it_ec) {
//			info.reset();
//			info.val_1 = it_ec->first;
//			info.val_2 = it_ec->second;
//			msg.exchange_item_vector.push_back(info);
//		}
//		for (Int_Int_Std_Map::const_iterator it_eo = equip_other.begin(); it_eo != equip_other.end(); ++it_eo) {
//			info.reset();
//			info.val_1 = it_eo->first;
//			info.val_2 = it_eo->second;
//			msg.exchange_item_vector.push_back(info);
//		}
//		for (Int_IntSet_Map_Reverse::const_iterator it = lvl_gt_cfg->begin(); it != lvl_gt_cfg->end(); ++it) {
//			if (it->first == lvl) {
//				continue;
//			}
//			equip_master.clear();
//			equip_hero.clear();
//			equip_common.clear();
//			equip_other.clear();
//			for (Int_Set::const_iterator it_s = it->second.begin(); it_s != it->second.end(); ++it_s) {
//				Exchange_Config::const_iterator it_e = cfg->find(*it_s);
//				if (it_e == cfg->end()) {
//					continue;
//				}
//				const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(*it_s);
//				if (!item_cfg
//						|| (item_cfg->equip_cfg.equip_lvl > 0
//								&& item_cfg->equip_cfg.career != EX_CAREER_ALL
//								&& item_cfg->equip_cfg.career != EX_CAREER_HERO
//								&& item_cfg->equip_cfg.career != car)) {
//					if (item_cfg && item_cfg->equip_cfg.equip_lvl == 0) {
//						equip_other.insert(std::make_pair(*it_s, it_e->second.amount));
//					}
//					continue;
//				}
//				if (item_cfg->equip_cfg.career == car) {
//					equip_master.insert(std::make_pair(*it_s, it_e->second.amount));
//				} else if (item_cfg->equip_cfg.career == EX_CAREER_HERO) {
//					equip_hero.insert(std::make_pair(*it_s, it_e->second.amount));
//				} else if (item_cfg->equip_cfg.career == EX_CAREER_ALL) {
//					equip_common.insert(std::make_pair(*it_s, it_e->second.amount));
//				}
//			}
//			for (Int_Int_Std_Map::const_iterator it_em = equip_master.begin(); it_em != equip_master.end(); ++it_em) {
//				info.reset();
//				info.val_1 = it_em->first;
//				info.val_2 = it_em->second;
//				msg.exchange_item_vector.push_back(info);
//			}
//			for (Int_Int_Std_Map::const_iterator it_eh = equip_hero.begin(); it_eh != equip_hero.end(); ++it_eh) {
//				info.reset();
//				info.val_1 = it_eh->first;
//				info.val_2 = it_eh->second;
//				msg.exchange_item_vector.push_back(info);
//			}
//			for (Int_Int_Std_Map::const_iterator it_ec = equip_common.begin(); it_ec != equip_common.end(); ++it_ec) {
//				info.reset();
//				info.val_1 = it_ec->first;
//				info.val_2 = it_ec->second;
//				msg.exchange_item_vector.push_back(info);
//			}
//			for (Int_Int_Std_Map::const_iterator it_eo = equip_other.begin(); it_eo != equip_other.end(); ++it_eo) {
//				info.reset();
//				info.val_1 = it_eo->first;
//				info.val_2 = it_eo->second;
//				msg.exchange_item_vector.push_back(info);
//			}
//		}
		break;
	}
	case EXCHANGE_TYPE_MEDAL: {
//		Int_IntSet_Map_Reverse::const_iterator it = lvl_gt_cfg->find(lvl);
//		if (it != lvl_gt_cfg->end()) {
//			for (Int_Set::const_iterator it_s = it->second.begin(); it_s != it->second.end(); ++it_s) {
//				Exchange_Config::const_iterator it_e = cfg->find(*it_s);
//				if (it_e != cfg->end()) {
//					info.reset();
//					info.val_1 = *it_s;
//					info.val_2 = it_e->second.amount;
//					msg.exchange_item_vector.push_back(info);
//				}
//			}
//			for (Int_IntSet_Map_Reverse::const_iterator it = lvl_gt_cfg->begin(); it != lvl_gt_cfg->end(); ++it) {
//				if (it->first == lvl) {
//					continue;
//				}
//				for (Int_Set::const_iterator it_s = it->second.begin(); it_s != it->second.end(); ++it_s) {
//				Exchange_Config::const_iterator it_e = cfg->find(*it_s);
//					if (it_e == cfg->end()) {
//						continue;
//					}
//					info.reset();
//					info.val_1 = *it_s;
//					info.val_2 = it_e->second.amount;
//					msg.exchange_item_vector.push_back(info);
//				}
//			}
//		} else {
//			for (Int_IntSet_Map_Reverse::const_iterator it = lvl_gt_cfg->begin(); it != lvl_gt_cfg->end(); ++it) {
//				if (it->first == lvl) {
//					continue;
//				}
				for (Int_Vec::const_iterator it_s = lvl_cfg->begin(); it_s != lvl_cfg->end(); ++it_s) {
					Exchange_Config::const_iterator it_e = cfg->find(*it_s);
					if (it_e == cfg->end()) {
						continue;
					}
					info.reset();
					info.id = *it_s;
					Int_Int_Map::const_iterator it_ex = exchange_detail_->ex_data_every_day.find(info.id);
					if (it_ex != exchange_detail_->ex_data_every_day.end()) {
						info.amount_left = it_ex->second;
					}
					info.amount_daily = it_e->second.limit;
					msg.exchange_item_vector.push_back(info);
				}
		break;
	}
//		}
	default:{
		for (Int_Vec::const_iterator it_s = lvl_cfg->begin(); it_s != lvl_cfg->end(); ++it_s) {
			Exchange_Config::const_iterator it_e = cfg->find(*it_s);
			if (it_e == cfg->end()) {
				continue;
			}
			info.reset();
			info.id = *it_s;
			Int_Int_Map::const_iterator it_ex = exchange_detail_->ex_data_every_day.find(info.id);
			if (it_ex != exchange_detail_->ex_data_every_day.end()) {
				info.amount_left = it_ex->second;
			}
			info.amount_daily = it_e->second.limit;
			msg.exchange_item_vector.push_back(info);
		}
		break;
	}
	}
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Exchange::req_exchange_skill_item(int item_id) {

	const Skill_Exchange_Item* cfg = CONFIG_CACHE_EXCHANGE->find_skill_exchange_item(item_id);
	if (0 == cfg)
		return ERROR_CONFIG_NOT_EXIST;

	int ret = 0;
	if (cfg->type_cost > 20000000) {
		Id_Amount item_cost(cfg->type_cost, cfg->num_cost);
		ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, item_cost);
	} else {
		Money_Sub_Type sub_type = MONEY_SUB_TYPE_END;
		if (cfg->type_cost == 501502) {
			sub_type = COPPER_ONLY;
		} else if (cfg->type_cost == 502903) {
			sub_type = SOULS_ONLY;
		}
		if (sub_type == MONEY_SUB_TYPE_END) return ERROR_CONFIG_NOT_EXIST;

		Money_Sub_Info money(sub_type, cfg->num_cost,
				MONEY_SUB_UPGRADE_WING);
		ret = player_self()->pack_try_sub_money(money);
	}
	if (ret != 0) return ret;

	Item_Detail item(cfg->id, 1, Item_Detail::BIND);
	ret = player_self()->pack_try_insert_item(PACK_T_PACKAGE_INDEX, item);
	if (ret != 0) return ret;

	if (cfg->type_cost > 20000000) {
		Id_Amount item_cost(cfg->type_cost, cfg->num_cost);
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost,
				Lose_Item_DM_Info(ITEM_LOSE_ACTIVE_WING));
	} else {
		Money_Sub_Type sub_type = MONEY_SUB_TYPE_END;
		if (cfg->type_cost == 501502) {
			sub_type = COPPER_ONLY;
		} else if (cfg->type_cost == 502903) {
			sub_type = SOULS_ONLY;
		}

		Money_Sub_Info money(sub_type, cfg->num_cost,
				MONEY_SUB_UPGRADE_WING);
		player_self()->pack_sub_money(money);
	}

	player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_WING));

	MSG_50100502 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}
