/*
 * Pack_Func.cpp
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#include "Config_Cache_Package.h"
#include "Pack_Func.h"
#include "Err_Code.h"
#include "card/Card_Config.h"
#include "Config_Cache_Card.h"

void pack_merge_item_array(std::vector<Item_Detail*> &item_array, Pack::Pack_Type pack_type,
		Pack::MERGE_WAY merge_way, UInt_Set &changed_set) {
	const uint32_t array_size = item_array.size();
	// 模拟人的的操作，不停将道具从index_now合并至index_pre
	uint32_t index_pre = 0;
	uint32_t index_now = 1;
	uint32_t index_flag = 1 + pack_type;		// 标记排序后的道具所在的index
	for (index_pre = 0; index_now < array_size; ++index_now) {
		bool merged = false;
		if (merge_way == Pack::MERGE_WAY_SIMILAR) {
			merged = pack_merge_similar_item(*item_array[index_pre], *item_array[index_now]);
		} else {
			merged = pack_merge_equal_item(*item_array[index_pre], *item_array[index_now]);
		}

		if (merged) {
			// 如果合并了，那么前后2个位置均改变了
			changed_set.insert(item_array[index_pre]->index_);
			changed_set.insert(item_array[index_now]->index_);
			if (item_array[index_now]->amount_ <= 0) {
				continue;
			}
		}
		// 如果次序不对
		if (item_array[index_pre]->index_ != index_flag) {
			changed_set.insert(item_array[index_pre]->index_);
			item_array[index_pre]->index_ = index_flag;
			changed_set.insert(index_flag);
		}
		index_pre = index_now;
		index_flag++;
	}

	if (index_pre < array_size) {
		// 如果次序不对
		if (item_array[index_pre]->index_ != index_flag) {
			changed_set.insert(item_array[index_pre]->index_);
			item_array[index_pre]->index_ = index_flag;
			changed_set.insert(index_flag);
		}
	}
}

bool pack_merge_equal_item(Item_Detail &item_des, Item_Detail &item_src) {
	int pre_remain_space = item_des.get_item_remain_amount();
	if (is_equal_item(item_des, item_src) && pre_remain_space  > 0) {
		int32_t item_src_amount = item_src.amount_;
		int32_t item_src_remain = std::max(0, item_src_amount - pre_remain_space);
		item_src.amount_ = item_src_remain;
		item_des.amount_ += std::min(item_src_amount, pre_remain_space);
		return true;
	}

	return false;
}

bool pack_merge_similar_item(Item_Detail &item_des, Item_Detail &item_src) {
	int pre_remain_space = item_des.get_item_remain_amount();
	if (is_similar_item(item_des, item_src) && pre_remain_space > 0) {
		int32_t item_src_amount = item_src.amount_;
		int32_t item_src_remain = std::max(0, item_src_amount - pre_remain_space);
		item_src.amount_ = item_src_remain;
		item_des.amount_ += std::min(item_src_amount, pre_remain_space);
		// 合并时，只要有一个是绑定的，则物品数量较多的格子全部转换为绑定的
		if (item_des.bind_ == Item_Detail::BIND || item_src.bind_ == Item_Detail::BIND) {
			// 若不是只是有一个物品为绑定的，则物品数量较少的格子为不绑定
			if (item_des.bind_ != item_src.bind_) {
				item_src.bind_ = Item_Detail::UNBIND;
			}
			item_des.bind_ = Item_Detail::BIND;
		}
		return true;
	}

	return false;
}

bool pack_merge_similar_item_move(Item_Detail &item_des, Item_Detail &item_src) {
	int pre_remain_space = item_des.get_item_remain_amount();
	if (is_similar_item(item_des, item_src) && pre_remain_space > 0) {
		int32_t item_src_amount = item_src.amount_;
		int32_t item_src_remain = std::max(0, item_src_amount - pre_remain_space);
		item_src.amount_ = item_src_remain;
		item_des.amount_ += std::min(item_src_amount, pre_remain_space);
		// 合并时，只要有一个是绑定的，则物品数量较多的格子全部转换为绑定的
		if (item_des.bind_ == Item_Detail::BIND || item_src.bind_ == Item_Detail::BIND) {
			// 若不是只是有一个物品为绑定的，则物品数量较少的格子为不绑定
//			if (item_des.bind_ != item_src.bind_) {
//				item_src.bind_ = Item_Detail::UNBIND;
//			}
			item_des.bind_ = Item_Detail::BIND;
		}
		return true;
	}

	return false;
}

bool pack_is_req_item(const Item_Detail &item, uint32_t id, Bind_Verify bind_verify, const Item_Vertify_By_Condition con) {
	bool result = false;
	switch (bind_verify) {
	case BIND_NONE:
		if (item.id_ == id) {
			result = true;
		} else {
			return false;
		}
		break;
		// no break
	case UNBIND_ONLY:
		if (item.id_ == id &&
				(item.bind_ == Item_Detail::UNBIND || item.bind_ == Item_Detail::UNDEFINED)) {
			result = true;
		} else {
			return false;
		}
		break;
		// no break
	case BIND_ONLY:
		if (item.id_ == id && item.bind_ == Item_Detail::BIND) {
			result = true;
		} else {
			return false;
		}
		break;
		// no break
	}

	if (result) {
		if (con != CON_START) {
			switch(con) {
			case CON_OVERDUE: {
				if (item.overdue_time != Time_Value::zero
						&& item.overdue_time < Time_Value::gettimeofday()) {
					return false;
				}
				break;
			}
			default:
				break;
			}
		}
		return result;
	}

	return false;
}

void merge_same_item(const Item_Vec& item_vec_in, Item_Vec& item_vec_out) {
	item_vec_out.clear();
	// 合并可重复item
	boost::unordered_map<Item_Detail, int32_t> amount_map;
	for (Item_Vec::const_iterator it = item_vec_in.begin(); it != item_vec_in.end(); ++it) {
		amount_map[*it] += it->amount_;
	}

	for (boost::unordered_map<Item_Detail, int32_t>::const_iterator it = amount_map.begin(); it != amount_map.end(); ++it) {
		Item_Detail item_temp(it->first);
		item_temp.amount_ = it->second;
		item_vec_out.push_back(item_temp);
	}
}

void get_capacity_super_item_nums(int start, int end, int pack_type, int &item_nums) {
	item_nums = 0;
	if (start >= end) {
		return;
	}
	int gap = end - start;
	int grid_per_line = 0;
	if (pack_type == PACK_T_PACKAGE_INDEX) {
		grid_per_line = PACKAGE_GRID_PER_LINE;
	} else {
		grid_per_line = STORAGE_GRID_PER_LINE;
	}
	int item_nums_temp = gap / grid_per_line;
	if (gap % grid_per_line != 0) {
		++item_nums_temp;
	}
	item_nums = item_nums_temp;
}

int get_capacity_price(Pack::Pack_Type pack_type, int start, int end, int &price) {
	const Package_Config package_config = CONFIG_CACHE_PACKAGE->package_config();
	Line_Capacity_Conf cap_price_conf = package_config.pack_line_cap_conf_;
	if (cap_price_conf.empty()) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	switch (pack_type) {
	case Pack::PACK_T_PACKAGE_INDEX:
		start -= PACK_INIT_CAPACITY;
		end -= PACK_INIT_CAPACITY;
		break;
	case Pack::PACK_T_STORAGE_INDEX:
		start -= STORAGE_INIT_CAPACITY;
		end -= STORAGE_INIT_CAPACITY;
		break;
	default:
		MSG_USER("wrong pack type : %d", pack_type);
		break;
	}

	if (start < 0 || end < 0 || static_cast<size_t>(start) > cap_price_conf.size() || static_cast<size_t>(end) > cap_price_conf.size()) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	price = 0;
	Line_Capacity_Conf::iterator it;
	for (int i = start; i < end; ++i) {
		it = cap_price_conf.find(i);
		if (it != cap_price_conf.end()) {
			price += it->second.gold;//lvl检查
		} else {
			MSG_USER("error capacity grid %d", i);
			return ERROR_CONFIG_NOT_EXIST;
		}
	}

	return 0;
}

bool is_need_trade_pay_limit(const Money_Sub_Info &info) {
	if (info.type != GOLD_ONLY && info.type != COPPER_ONLY) {
		return false;
	}

	switch (info.dm_info.type) {
	case MONEY_SUB_MAIL_MONEY:
	case MONEY_SUB_TRADE_USE:
	case MONEY_SUB_MARKET_BUY:
	case MONEY_SUB_MARKET_PLEDGE:
		return true;
	default:
		return false;
	}

	return false;
}

bool is_need_trade_get_limit(const Money_Add_Info &info) {
	if (info.type != GOLD && info.type != COPPER) {
		return false;
	}

//	if (info.dm_info.type == MONEY_ADD_MAIL_PICKUP) {
//		if (info.dm_info.ext_1 == Mail_Info_Detail::ROLE_MAIL || info.dm_info.ext_1 == Mail_Info_Detail::MARKET_MAIL) {
//			return true;
//		}
//	}

	if (info.dm_info.type == MONEY_ADD_TRADE_GAIN) {
		return true;
	}

	return false;
}

//int get_item_color(const int item_id) {
//	const Json::Value &json = CONFIG_INSTANCE->item(item_id);
//	if (json == Json::Value::null)
//		return -1;
//	return json["color"].asInt();
//}

void make_item_vec(Item_Vec& item_vec, int32_t item_id, int32_t quantity, int8_t state) {
	const Card_Config *card_cfg = CONFIG_CACHE_CARD->get_card_config(item_id);
	if (0 == card_cfg) {
		item_vec.push_back(Item_Detail(item_id, quantity, state));
	} else {
		for (int i=0; i<quantity; ++i){
			Item_Detail item(item_id, 1, state);
			item_vec.push_back(item);
		}
	}
}

int get_item_type_second(int id) {
	int first_type = id / 10000000;
	int second_type = id / 100000;
	switch(first_type) {
	case Item_Detail::EQUIP:
		return Item_Detail::EQUIP;
	case Item_Detail::TOOL:
	{
		switch(second_type) {
		case Item_Detail::TOOL_TASK:
			return Item_Detail::TOOL_TASK;
		case Item_Detail::TOOL_LOTTERY:
			return Item_Detail::TOOL_LOTTERY;
		case Item_Detail::TOOL_FLOWER:
			return Item_Detail::TOOL_FLOWER;
		case Item_Detail::TOOL_DRUG:
			return Item_Detail::TOOL_DRUG;
		case Item_Detail::TOOL_MATERIAL:
			return Item_Detail::TOOL_MATERIAL;
		case Item_Detail::TOOL_CARD:
			return Item_Detail::TOOL_CARD;
		case Item_Detail::TOOL_RESOURCE:
			return Item_Detail::TOOL_RESOURCE;
		case Item_Detail::TOOL_CONSUME_TOOL:
			return Item_Detail::TOOL_CONSUME_TOOL;
		case Item_Detail::TOOL_ROLL:
			return Item_Detail::TOOL_ROLL;
		case Item_Detail::TOOL_GEMSTONE:
			return Item_Detail::TOOL_GEMSTONE;
		default:
			break;
		}
		break;
	}
	case Item_Detail::DRUG:
		return Item_Detail::DRUG;
	case Item_Detail::RUNE_STONE:
		return Item_Detail::RUNE_STONE;
	case Item_Detail::CARD:
		return Item_Detail::CARD;
	default:
		break;
	}
	return Item_Detail::NORMAL;
}

Money_Sub_Type get_money_sub_type(int id){
	switch(id){
		case COPPER:{
			return COPPER_ONLY;
			break;
		}
		case GOLD:{
			return GOLD_ONLY;
			break;
		}
		case BIND_GOLD:{
			return BIND_GOLD_FIRST;
			break;
		}
		case DRAGON_SOUL:{
			return DRAGON_SOUL_ONLY;
			break;
		}
		case SOULS:{
			return SOULS_ONLY;
			break;
		}
		case FRIENDSHIP:{
			return FRIENDSHIP_ONLY;
			break;
		}
		case REPUTATION:{
			return REPUTATION_ONLY;
			break;
		}
		default:{
			return MONEY_SUB_TYPE_END;
			break;
		}
	}
}

