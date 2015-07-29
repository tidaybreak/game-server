/*
 * Item_User_Func.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: "linqiyou"
 */

#include "Item_User_Func.h"
#include "Config_Cache_Item.h"
#include "Utility_Func.h"
#include "Err_Code.h"
#include "pack/Pack_Func.h"

Money_Type get_money_type(uint32_t id){
	switch(id) {
	case COPPER:
		return COPPER;
	case GOLD:
		return GOLD;
	case BIND_GOLD:
		return BIND_GOLD;
	case DRAGON_SOUL:
		return DRAGON_SOUL;
	case SOULS:
		return SOULS;
	case FRIENDSHIP:
		return FRIENDSHIP;
	case REPUTATION:
		return REPUTATION;
	default:
		return MONEY_TYPE_END;
	}
}
int get_gift_item_list(const Json::Value &item_list_json, int amount, Item_Vec &item_list) {
	if (amount <= 0) {
		return -1;
	}

	int result = 0;
	for (int i = 0; i < amount; ++i) {
		result = get_gift_item_list(item_list_json, item_list);
		if (result != 0) {
			break;
		}
	}

	return result;
}

int get_gift_item_list(const Json::Value &item_list_json, Item_Vec &item_list) {
	if (item_list_json == Json::Value::null) return ERROR_INNER_SERVER;

	int item_nums = item_list_json.size();
	Item_Detail item_detail;
	for (int i = 0; i < item_nums; ++i) {
		item_detail.reset();
		int item_list_size = item_list_json[static_cast<uint32_t>(i)].size();
		if (item_list_size == 3) {
			item_detail.set_basic(0, item_list_json[static_cast<uint32_t>(i)][0u].asUInt(),
			item_list_json[static_cast<uint32_t>(i)][1u].asInt(),
			item_list_json[static_cast<uint32_t>(i)][2u].asInt());
//		} else if (item_list_size == 2) {
//			item_detail.set_basic(0, item_list_json[static_cast<uint32_t>(i)][0u].asUInt(),
//			item_list_json[static_cast<uint32_t>(i)][1u].asInt());
		} else {
			MSG_USER("configure error.");
			continue;
		}
		item_list.push_back(item_detail);
	}

	return 0;
}

int get_item_detail_list(const Item_Info_Vec &item_list_config, Item_Vec &item_list) {
	if (item_list_config.empty()) return ERROR_INNER_SERVER;

	Item_Detail item_detail;
	for (Item_Info_Vec::const_iterator it = item_list_config.begin(); it != item_list_config.end(); ++it) {
		item_detail.reset();
		item_detail.set_basic(0, (*it).id, (*it).amount, (*it).bind);
		item_list.push_back(item_detail);
	}

	return 0;
}

int get_lottery_item(uint32_t id, int level, int career, int amount, Item_Vec &item, Item_Vec &broadcast_item, Money_Add_List &money_add_list, int &used_times,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost,std::vector<Int_Int> &special_receive) {
	if (amount <= 0) {
		return -1;
	}

	int result = 0;
	for (int i = 0; i < amount; ++i) {
		result = get_lottery_item(id, level, career, item, broadcast_item, money_add_list, used_times, item_cost, money_cost,special_receive);
		if (result != 0) {
			break;
		}
	}

	return result;
}

int get_lottery_item(uint32_t id, int level, int career, Item_Vec &item, Item_Vec &broadcast_item, Money_Add_List &money_add_list, int &used_times,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost,std::vector<Int_Int> &special_receive) {
	const Item_Lottery_Detail_Config *lottery_config = CONFIG_CACHE_ITEM->find_item_lottery(id);
	if (!lottery_config)
		return ERROR_CONFIG_NOT_EXIST;

	return get_lottery_item(id, level, career, lottery_config->item_list, item, broadcast_item, money_add_list
			, item_cost, money_cost,special_receive);
}

int get_lottery_item(uint32_t id, int level, int career, const Item_Lottery_Item_Info_Vec_Config &item_list_config,
		Item_Vec &item_list, Item_Vec &broadcast_item,
		Money_Add_List &money_add_list,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost,std::vector<Int_Int> &special_receive) {
	Item_Detail new_item;
	uint32_t array_size = item_list_config.size();
	for (uint32_t i = 0; i < array_size; ++i) {
		if ((-1 != item_list_config[i].level_low && -1 != item_list_config[i].level_high)
				&& (level < item_list_config[i].level_low || level > item_list_config[i].level_high)) {
			continue;
		}
		for (Item_Info_Vec::const_iterator it_ilc = item_list_config[i].cost.begin();
				it_ilc != item_list_config[i].cost.end(); ++it_ilc) {
			uint32_t item_or_money_id = (*it_ilc).id;
			if (Pack::MONEY_TYPE_END != get_money_type(item_or_money_id)) {
				Pack::Money_Sub_Type sub_type;
				if (ERROR_MONEY_TYPE_ILLEGAL == get_sub_type_by_money_type(static_cast<Pack::Money_Type>(item_or_money_id), sub_type)) {
					return ERROR_MONEY_TYPE_ILLEGAL;
				}
				money_cost.push_back(
						Money_Sub_Info(sub_type, (*it_ilc).amount,
								Money_DM_Info(Pack::MONEY_SUB_USE_ITEM, id, 1)));
			} else {
				Id_Amount ia;
				ia.id = item_or_money_id;
				ia.amount = (*it_ilc).amount;
				item_cost.push_back(ia);
			}
		}
		//weight为空，获取所有
		{
			if (item_list_config[i].weight.empty()) {
				const Item_Info_Vec *il_cfg = 0;
				switch(career) {
				case 1: {
					il_cfg = &(item_list_config[i].item_list);
					break;
				}
				case 2: {
					il_cfg = &(item_list_config[i].item_list_ck);
					break;
				}
				case 3: {
					il_cfg = &(item_list_config[i].item_list_yss);
					break;
				}
				default:
					break;
				}
				if (il_cfg && !il_cfg->empty()) {
					for (Item_Info_Vec::const_iterator it = il_cfg->begin();
							it != il_cfg->end(); ++it) {
						uint32_t item_or_money_id = (*it).id;
						if (Pack::MONEY_TYPE_END != get_money_type(item_or_money_id)) {
							money_add_list.push_back(
									Money_Add_Info(static_cast<Pack::Money_Type>(item_or_money_id), (*it).amount,
											Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, id, 1)));
						} else {
							if ( item_or_money_id == 503703) {
								// 以后特殊物品加入
								Int_Int unit;
								unit.val_1 = item_or_money_id;
								unit.val_2 = (*it).amount;
								special_receive.push_back(unit);
								continue;
							}
							new_item.reset();
							new_item.set_basic(0, item_or_money_id, (*it).amount, (*it).bind);
							item_list.push_back(new_item);
						}
					}
				} else {
					MSG_TRACE_ABORT("error lottery item_list empty: item_id(%d)", id);
				}
				continue;
			}
		}
		if (0 == item_list_config[i].type) {
			get_lottery_item(id, level, career, item_list_config[i], item_list, broadcast_item,
					money_add_list,
					item_cost, money_cost);
		} else {
			get_drop_item(id, level, career, item_list_config[i], item_list, broadcast_item,
								money_add_list,
								item_cost, money_cost);
		}
	}

	return 0;
}

int get_lottery_item(uint32_t id, int level, int career, const Item_Lottery_Item_Info_Config &item_list_config,
		Item_Vec &item_list, Item_Vec &broadcast_item,
		Money_Add_List &money_add_list,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost) {
	Item_Detail new_item;

	/////-- weight不为空
	{	uint32_t count = item_list_config.count;
		if (count == 0) {				// 宝箱可以配置抽取次数 没配视为0
			count = 1;
		}
		const Item_Info_Vec *il_cfg = 0;
		switch(career) {
		case 1: {
			il_cfg = &(item_list_config.item_list);
			break;
		}
		case 2: {
			il_cfg = &(item_list_config.item_list_ck);
			break;
		}
		case 3: {
			il_cfg = &(item_list_config.item_list_yss);
			break;
		}
		default:
			break;
		}
		if (!il_cfg || il_cfg->empty()) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		for (uint32_t count_i = 0; count_i < count; ++count_i) {
			if (!Utility_Func::lottery_success(item_list_config.drate_rate)) {
				continue;
			}
			int index = Utility_Func::lottery_one(item_list_config.weight);
			if (index < 0)
				return ERROR_CONFIG_NOT_EXIST;
			if (index > static_cast<int>(il_cfg->size())) {
				return ERROR_CONFIG_NOT_EXIST;
			}
			uint32_t item_or_money_id = (*il_cfg)[index].id;
			if (0 == item_or_money_id) {
				continue;
			}
			if (item_or_money_id <= Pack::MONEY_TYPE_END) {
				money_add_list.push_back(
						Money_Add_Info(static_cast<Pack::Money_Type>(item_or_money_id), (*il_cfg)[index].amount,
								Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, id, 1)));
			} else {
				new_item.reset();
				new_item.set_basic(0, item_or_money_id, (*il_cfg)[index].amount,
						(*il_cfg)[index].bind);
				item_list.push_back(new_item);
			}
		}
	}
	/////-- weight不为空

	return 0;
}

/*
 * 权重处理：第1个100%出，之后的相关概率
 * */
int get_drop_item(uint32_t id, int level, int career, const Item_Lottery_Item_Info_Config &item_list_config,
		Item_Vec &item_list, Item_Vec &broadcast_item,
		Money_Add_List &money_add_list,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost) {
	Item_Detail new_item;

	/////-- weight不为空
	{	uint32_t count = item_list_config.count;
		if (count == 0) {				// 宝箱可以配置抽取次数 没配视为0
			count = 1;
		}
		const Item_Info_Vec *il_cfg = 0;
		switch(career) {
		case 1: {
			il_cfg = &(item_list_config.item_list);
			break;
		}
		case 2: {
			il_cfg = &(item_list_config.item_list_ck);
			break;
		}
		case 3: {
			il_cfg = &(item_list_config.item_list_yss);
			break;
		}
		default:
			break;
		}
		if (!il_cfg || il_cfg->empty()) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		Int_Vec weight = item_list_config.weight;
		for (uint32_t count_i = 0; count_i < count; ++count_i) {
			if (!Utility_Func::lottery_success(item_list_config.drate_rate)) {
				continue;
			}
			int index = 0;
			if (weight.size() == 1) {
				uint32_t item_or_money_id = (*il_cfg)[index].id;
				if (item_or_money_id <= Pack::MONEY_TYPE_END) {
					money_add_list.push_back(
							Money_Add_Info(static_cast<Pack::Money_Type>(item_or_money_id), (*il_cfg)[index].amount,
									Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, id, 1)));
				} else {
					new_item.reset();
					new_item.set_basic(0, item_or_money_id, (*il_cfg)[index].amount,
							(*il_cfg)[index].bind);
					item_list.push_back(new_item);
				}
				continue;
			} else {
				weight.erase(weight.begin());
				uint32_t item_or_money_id = (*il_cfg)[index].id;
				if (item_or_money_id <= Pack::MONEY_TYPE_END) {
					money_add_list.push_back(
							Money_Add_Info(static_cast<Pack::Money_Type>(item_or_money_id), (*il_cfg)[index].amount,
									Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, id, 1)));
				} else {
					new_item.reset();
					new_item.set_basic(0, item_or_money_id, (*il_cfg)[index].amount,
							(*il_cfg)[index].bind);
					item_list.push_back(new_item);
				}
			}

			index = Utility_Func::lottery_one(weight) + 1;
			if (index < 0)
				return ERROR_CONFIG_NOT_EXIST;
			if (index > static_cast<int>((*il_cfg).size())) {
				return ERROR_CONFIG_NOT_EXIST;
			}
			uint32_t item_or_money_id = (*il_cfg)[index].id;
			if (0 == item_or_money_id) {
				continue;
			}
			if (item_or_money_id <= Pack::MONEY_TYPE_END) {
				money_add_list.push_back(
						Money_Add_Info(static_cast<Pack::Money_Type>(item_or_money_id), (*il_cfg)[index].amount,
								Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, id, 1)));
			} else {
				new_item.reset();
				new_item.set_basic(0, item_or_money_id, (*il_cfg)[index].amount,
						(*il_cfg)[index].bind);
				item_list.push_back(new_item);
			}
		}
	}
	/////-- weight不为空

	return 0;
}

Item_Effect_Type get_item_type(uint32_t id) {
	uint32_t type = id / 10000000;
	if (type != 2) {
		return ITEM_EFFECT_T_END;
	}

	type = (id - type * 10000000) / 100000;
	switch(type) {
	case ITEM_EFFECT_T_NORMAL:
		return ITEM_EFFECT_T_NORMAL;
	case ITEM_EFFECT_T_TASK:
		return ITEM_EFFECT_T_TASK;
	case ITEM_EFFECT_T_LOTTERY:
		return ITEM_EFFECT_T_LOTTERY;
	case ITEM_EFFECT_T_FLOWER:
		return ITEM_EFFECT_T_FLOWER;
	case ITEM_EFFECT_T_DRUG:
		return ITEM_EFFECT_T_DRUG;
	case ITEM_EFFECT_T_MATERIAL:
		return ITEM_EFFECT_T_MATERIAL;
	case ITEM_EFFECT_T_CARD:
		return ITEM_EFFECT_T_CARD;
	case ITEM_EFFECT_T_RESOURCE:
		return ITEM_EFFECT_T_RESOURCE;
	case ITEM_EFFECT_T_CONSUME_TOOL:
		return ITEM_EFFECT_T_CONSUME_TOOL;
	case ITEM_EFFECT_T_EQUIP:
		return ITEM_EFFECT_T_EQUIP;
	case ITEM_EFFECT_T_EXCHANGE:
		return ITEM_EFFECT_T_EXCHANGE;
	case ITEM_EFFECT_T_ACHIEVEMENT:
		return ITEM_EFFECT_T_ACHIEVEMENT;
	case ITEM_EFFECT_T_MOUNT:
		return ITEM_EFFECT_T_MOUNT;
	case ITEM_EFFECT_T_GEM_COMPOSE:
		return ITEM_EFFECT_T_GEM_COMPOSE;
	case ITEM_EFFECT_T_LITTLE_FUN:
		return ITEM_EFFECT_T_LITTLE_FUN;
	default :
		return ITEM_EFFECT_T_END;
	}
}

Item_Effect_Type get_item_type_third(uint32_t id) {
	uint32_t type = id / 10000000;
	if (type != 2) {
		return ITEM_EFFECT_T_END;
	}

	type = (id - type * 10000000) / 100000;
	switch(type) {
	case ITEM_EFFECT_T_NORMAL:
		return ITEM_EFFECT_T_NORMAL;
	case ITEM_EFFECT_T_TASK:
		return ITEM_EFFECT_T_TASK;
	case ITEM_EFFECT_T_LOTTERY:
		return ITEM_EFFECT_T_LOTTERY;
	case ITEM_EFFECT_T_FLOWER:
		return ITEM_EFFECT_T_FLOWER;
	case ITEM_EFFECT_T_DRUG:
		return ITEM_EFFECT_T_DRUG;
	case ITEM_EFFECT_T_MATERIAL:
		return ITEM_EFFECT_T_MATERIAL;
	case ITEM_EFFECT_T_CARD: {
		uint16_t type_3 = (id % 10000000) / 1000;
		switch(type_3) {
		case ITEM_EFFECT_T_CARD_VIP:
			return ITEM_EFFECT_T_CARD_VIP;
		case ITEM_EFFECT_T_CARD_BUFF:
			return ITEM_EFFECT_T_CARD_BUFF;
		case ITEM_EFFECT_T_CARD_OTHER:
			return ITEM_EFFECT_T_CARD_OTHER;
		case ITEM_EFFECT_T_CARD_HERO:
			return ITEM_EFFECT_T_CARD_HERO;
		default:
			break;
		}
		return ITEM_EFFECT_T_CARD;
	}
	case ITEM_EFFECT_T_RESOURCE:
		return ITEM_EFFECT_T_RESOURCE;
	case ITEM_EFFECT_T_CONSUME_TOOL:
		return ITEM_EFFECT_T_CONSUME_TOOL;
	default :
		return ITEM_EFFECT_T_END;
	}
}

int get_resource_item(Item_Vec &item_list,Money_Add_List &money_add_list,std::vector<Int_Int> &spe_item,const uint32_t id,const int num) {

	const Item_Resource_Detail *item_config = CONFIG_CACHE_ITEM->find_item_resource(id);
	if ( item_config ) {
		if ( item_config->gain_list.empty() )
			return ERROR_CONFIG_NOT_EXIST;
		for ( std::vector<Int_Int>::const_iterator iter = item_config->gain_list.begin();iter != item_config->gain_list.end();++iter ) {
			const Item_Detail_Config *ic = CONFIG_CACHE_ITEM->find_item(iter->val_1);
			if ( ic ) {
				// 道具
				item_list.push_back(Item_Detail(ic->item_id,iter->val_2*num,ic->bind));
			}else {
				// 资源
				if ( (iter->val_1 != COPPER && iter->val_1 != SOULS
					   && iter->val_1 != FRIENDSHIP && iter->val_1 != 502503
					   && iter->val_1 != 503003
					   && iter->val_1 != GOLD && iter->val_1 != BIND_GOLD
					   && iter->val_1 != DRAGON_SOUL
					   && iter->val_1 != REPUTATION)) {
					return ERROR_CONFIG_NOT_EXIST;
				}
				if ( iter->val_1 == 503003 || iter->val_1 == 502503 ) {

					spe_item.push_back(*iter);
				}else
					money_add_list.push_back(Money_Add_Info(Money_Type(iter->val_1),iter->val_2*num,Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, id,1)));
			}
		}
	}
	if ( item_list.empty() && money_add_list.empty() && spe_item.empty() )
		return ERROR_CONFIG_NOT_EXIST;
	return 0;
}
