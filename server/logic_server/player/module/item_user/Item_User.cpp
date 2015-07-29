/*
 * Item_User.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: "linqiyou"
 */

#include "Logic_Module_Head.h"
#include "Config_Cache_Item.h"
#include "pack/Packer.h"
#include "Item_User.h"
#include "pack/Pack_Func.h"
#include "Item_User_Struct.h"
#include "Item_User_Func.h"
#include "Msg_Packer_Struct.h"
#include "Msg_Active_Struct.h"
#include "Logic_Player.h"
#include "Config_Cache_Package.h"
#include "Msg_Inner_Struct.h"
#include "Config_Cache_Mount.h"
#include "Config_Cache_Scene.h"
#include "wing/Wing_Struct.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Equip.h"
#include "Config_Cache_Role.h"

Item_User::Item_User(void) : item_user_detail_(0) { }

Item_User::~Item_User(void) { }

int Item_User::create_init(Item_User_Detail &detail) {
	return 0;
}

void Item_User::load_detail(Item_User_Detail &detail) {
	item_user_detail_ = &detail;
}

void Item_User::save_detail(Item_User_Detail &detail) {
	if(item_user_detail().is_change) {
		detail = item_user_detail();
		item_user_detail_->is_change = false;
	}
	return ;
}

void Item_User::module_init(void) {
//	for (Int_Double_Map::const_iterator it = item_user_detail_->extra_prop_.begin(); it != item_user_detail_->extra_prop_.end(); ++it) {
//		if (it->second <= 0) {
//			continue;
//		}
//	}
	return ;
}

void Item_User::sign_in(void) {

}

void Item_User::sign_out(void) {

}

void Item_User::reset(void) {
	item_user_detail_ = 0;
}

//void Item_User::pack_clean_overdue(void) {
//	Packer::pack_clean_overdue();
//}

int Item_User::item_user_use_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value) {
	uint32_t item_id = 0;
	int result = this->use_item(index, amount, e_value, item_id);
	item_res_client_use_item(result, item_id);
	//使用道具，通知客户端开启图标
	player_self()->notify_open_icon_by_use_item(item_id);
	return result;
}

int Item_User::use_special_gift(const int32_t item_index, const Int_Vec &select_index_vector) {
//	const Item_Detail *item = pack_get_const_item(item_index);
//	if (NULL == item)
//		return send_err_to_client(ERROR_ITEM_NOT_EXIST);
//
//	const Item_Gift_Detail_Config *gift_config = CONFIG_CACHE_ITEM->find_item_gift(item->id_);
//	if (!gift_config)
//		return send_err_to_client(ERROR_CONFIG_NOT_EXIST);
//
//	MSG_81000102 res_msg;
//	res_msg.type = 0;
//
////	const Json::Value &item_json = gift_json["item_list"];
//	int item_size = gift_config->item_list.size();
//	Int_Set select_set;
//	std::vector<Item_Detail> insert_item_vector;
//	for (Int_Vec::const_iterator iter = select_index_vector.begin();
//			iter != select_index_vector.end(); ++iter) {
//		int index = *iter;
//		if (index < 0 || index >= item_size)
//			return send_err_to_client(ERROR_CLIENT_PARAM);
//
//		if (0 == gift_config->item_list[index].id)
//			return send_err_to_client(ERROR_CONFIG_NOT_EXIST);
//
//		Item_Basic_Info item_basic;
//		item_basic.reset();
//		item_basic.id = gift_config->item_list[index].id;
//		item_basic.amount = gift_config->item_list[index].amount;
//		item_basic.bind = gift_config->item_list[index].bind;
//		insert_item_vector.push_back(Item_Detail(item_basic));
//		select_set.insert(index);
//	}
//
//	if (gift_config->select_amount != (int)select_set.size())
//		return send_err_to_client(ERROR_CLIENT_PARAM);
//
//	int result = this->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, insert_item_vector, Gain_Item_DM_Info(ITEM_GAIN_GIFT));
//	if (result != 0)
//		return send_err_to_client(result);
//
//	this->pack_erase_item(Index_Amount(item_index, 1), Lose_Item_DM_Info(ITEM_LOSE_USE));
//
//	MSG_82100006 msg_active;
//	msg_active.reset();
//	std::vector<Item_Detail> result_item_vector;
//	merge_same_item(insert_item_vector, result_item_vector);
//	Item_Basic_Info item_base;
//	for (std::vector<Item_Detail>::iterator iter = result_item_vector.begin();
//			iter != result_item_vector.end(); ++iter) {
//		item_base.reset();
//		iter->set_item_basic_info(item_base);
//		msg_active.item_info.push_back(item_base);
//
//		res_msg.item_info.push_back(item_base);
//	}
//	THIS_SEND_TO_CLIENT(msg_active);
//
//	if (!res_msg.item_info.empty() || !res_msg.property.empty()) {
//		THIS_SEND_TO_CLIENT(res_msg);
//	}
	return 0;
}

int Item_User::item_res_client_use_item(int result, uint32_t item_id) {
	if (result == 0) {
		MSG_50100106 res_msg;
		res_msg.id = item_id;
		THIS_SEND_TO_CLIENT(res_msg);
	}
//	else if (result != ERROR_HANDLE_LATER) {// 此时返回结果由地图服决定，故此时不返回客户端
//		respond_error_result(RES_USE_SPECINDEX_ITEM, result, NULL);
//	}
	return 0;
}
int Item_User::req_buy_and_use(const int32_t item_id, const int32_t amount) {
	const Quick_Buy *qb = CONFIG_CACHE_ITEM->find_quick_buy(item_id);
	const Item_Drug_Detail *item_drug = CONFIG_CACHE_ITEM->find_item_drug(item_id);
	if (NULL == qb || qb->price <= 0 || NULL == item_drug) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (item_drug->usetype == ITEM_USE_DRUG_BLOOD_POOL) {
		const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
		if (sc && (sc->type == War_Scene_Type || sc->type == GANG_WAR_TYPE)) {
			return ERROR_ITEM_USER_SCENE_LIMIT;
		}
	}

	{//尝试扣钱
		int res = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(qb->type), qb->price, Pack::MONEY_SUB_QUICK_BUY));
		if(res) { return res; }
	}

	MSG_20100305 inner_msg;
	inner_msg.tp = ITEM_USE_DRUG;
	inner_msg.val = item_drug->val;
	inner_msg.index = 0;
	inner_msg.amount = amount;
	Block_Buffer buf;
	buf.make_message(inner_msg.msg_id);
	inner_msg.serialize(buf);
	buf.write_int32(item_id);
	buf.write_int8(item_drug->usetype);
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

int Item_User::use_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, uint32_t &id) {
	const Item_Detail *item = pack_get_const_item(index);
	if (NULL == item) return ERROR_ITEM_NOT_EXIST;
	id = item->id_;

	Time_Value now = Time_Value::gettimeofday();
	Time_Value cd;
	CD_Type type;
	get_item_cooldown_info(item->id_, type, cd);
	int result = check_item_param(amount, item, now, cd, type);
	if (result != 0) return result;

//	result = use_item(index, amount, e_value, item, get_item_target(item->id_));
	result = use_item(index, amount, e_value, item, ITEM_TARGET_PLAYER);

	if (result == 0 && cd > Time_Value::zero) {
		item_user_detail_->can_use_time_[type] = now + cd;
		item_user_detail_->is_change = true;
	}

	return result;
}

//Item_User::Item_Effect_Type Item_User::get_item_type(uint32_t id) {
//	uint32_t type = id / 10000000;
//	if (type != 2) {
//		return ITEM_EFFECT_T_END;
//	}
//
//	type = (id - type * 10000000) / 100000;
//	switch(type) {
//	case 2:
//		return ITEM_EFFECT_T_LOTTERY;
//	default :
//		return ITEM_EFFECT_T_END;
//	}
//}

int Item_User::check_item_param(const int amount, const Item_Detail *item, const Time_Value &now, const Time_Value &cd,
		const CD_Type &type) {
	// 不能批量使用的amount不能大于1
	if (NULL == item || (!is_item_batch_using(item->id_) && amount > 1)) {
		return ERROR_CLIENT_PARAM;
	}
	// amount不能大于指定index处的amount
	if (amount > item->amount_ || amount < 0) {
		return ERROR_CLIENT_PARAM;
	}
	if (cd > Time_Value::zero && item_user_detail_->can_use_time_[type] > now) {
		return ERROR_ITEM_NOT_CD;
	}
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item->id_);
	if(!item_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (level() < item_config->needLevel) {
        player_self()->send_tip_to_client(ERROR_LEVEL_NEED_LIMIT,item_config->needLevel);
		return -1;
	}

	if (item_config->occupation > 0 && career() != item_config->occupation) {
		std::string name = CONFIG_CACHE_ROLE->get_career_name(career());
		player_self()->send_tip_to_client(ERROR_CAREER_NOT_USE,name);
		return -1;
	}

	if (item_config->time > Time_Value::zero && now > item->overdue_time ) {
		return ERROR_ITEM_OVERDUE;
	}

	if (!item_config->timeLimit.size() == 2) {
		if (item_config->timeLimit[0] > 0 && item_config->timeLimit[1] > 0
				&& (item_config->timeLimit[0] < now.sec() || item_config->timeLimit[1] > now.sec())) {
			return ERROR_ITEM_UNUSABLE;
		}
	}

	// 当前场景是否可使用
	if (!item_config->scene.empty()) {
		bool could_use = false;
		for (Int_Vec::const_iterator it = item_config->scene.begin(); it != item_config->scene.end(); ++it) {
			if (this->player_self()->scene_id() == *it) {
				could_use = true;
				break;
			}
		}
		if (!could_use) {
			return ERROR_ITEM_SCENE_UNABLE;
		}
	}

	return 0;
}

int Item_User::use_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Target target) {
	if (NULL == item) return ERROR_INNER_SERVER;
	int result = pack_try_erase_item(Index_Amount(index, amount));
	if (0 != result) return result;
//	if (target == Item_User_Detail::ITEM_TARGET_PET && !this->player_self()->pet_is_fight()) {
//		return ERROR_FIGHT_PET_NOT_EXIST;
//	}

//	const Item_Prop_Detail_Config *item_prop_config = CONFIG_CACHE_ITEM->find_item_prop(item->id_);
//	if (!item_prop_config) return ERROR_CONFIG_NOT_EXIST;

	Item_Effect_Type item_type = get_item_type(item->id_);
	if (ITEM_EFFECT_T_END == item_type) {
		return ERROR_INNER_SERVER;
	}

	Item_Vec item_info;

	// 新增加所有道具使用都能获得资源或者道具
	const Item_Resource_Detail *resource_item = CONFIG_CACHE_ITEM->find_item_resource(item->id_);
	if (resource_item) {
		result =  use_resource_item(index, amount, e_value, item, item_info);
	}else {
		switch (item_type) {
		case ITEM_EFFECT_T_LOTTERY:{
			result = use_lottery_item(index, amount, e_value, item, item_info);
			if (0 == result) {
				result = ERROR_ITEM_USER_SUCCESS;
			}
			break;
		}
		case ITEM_EFFECT_T_DRUG: {
			result = use_drug_item(index, amount, e_value, item, item_info);
			break;
		}
		case ITEM_EFFECT_T_MATERIAL: {
			result = use_material_item(index, amount, e_value, item, item_info);
			break;
		}
		case ITEM_EFFECT_T_CARD: {
			result = use_card_item(index, amount, e_value, item, item_info);
			if (0 == result) {
			    result = ERROR_ITEM_USER_SUCCESS;
			}
			break;
		}
		case ITEM_EFFECT_T_RESOURCE: {
			result = use_resource_item(index, amount, e_value, item, item_info);
			break;
		}
		case ITEM_EFFECT_T_CONSUME_TOOL: {
			result = use_consume_tool(index, amount, e_value, item, item_info);
			if (0 == result) {
				result = ERROR_ITEM_USER_SUCCESS;
			}
			break;
		}
		case ITEM_EFFECT_T_EQUIP: {
			result = use_resource_item(index, amount, e_value, item, item_info);
			break;
		}
		//	case ITEM_EFFECT_T_EXCHANGE: {
		//		result = use_consume_tool(index, amount, e_value, item, item_info);
		//		break;
		//	}
		//	case ITEM_EFFECT_T_ACHIEVEMENT: {
		//		result = use_consume_tool(index, amount, e_value, item, item_info);
		//		break;
		//	}
		case ITEM_EFFECT_T_MOUNT: {
			result = use_mount(index, amount, e_value, item, item_info);
			if (0 == result) {
				result = ERROR_ITEM_USER_SUCCESS;
			}
			break;
		}
		case ITEM_EFFECT_T_GEM_COMPOSE: {
			result = use_gem_compose(index, amount, e_value, item, item_info);
				//if (0 == result) {
				//	result = ERROR_ITEM_USER_SUCCESS;
				//}
			break;
		}
		case ITEM_EFFECT_T_LITTLE_FUN: {

			result = use_little_fun(index, amount, e_value, item, item_info);
			break;
		}
		default:{
			result = 0;//use_normal_item(index, amount, e_value, item, item_prop_config->effect, target);
			break;
		}
		}
	}

//	switch (item_type) {
//	case ITEM_EFFECT_T_LOTTERY:{
//		result = use_lottery_item(index, amount, e_value, item, item_info);
//		if (0 == result) {
//			result = ERROR_ITEM_USER_SUCCESS;
//		}
//		break;
//	}
//	case ITEM_EFFECT_T_DRUG: {
//		result = use_drug_item(index, amount, e_value, item, item_info);
//		break;
//	}
//	case ITEM_EFFECT_T_MATERIAL: {
//		result = use_material_item(index, amount, e_value, item, item_info);
//		break;
//	}
//	case ITEM_EFFECT_T_CARD: {
//		result = use_card_item(index, amount, e_value, item, item_info);
//		if (0 == result) {
//			result = ERROR_ITEM_USER_SUCCESS;
//		}
//		break;
//	}
//	case ITEM_EFFECT_T_RESOURCE: {
//		result = use_resource_item(index, amount, e_value, item, item_info);
//		break;
//	}
//	case ITEM_EFFECT_T_CONSUME_TOOL: {
//		result = use_consume_tool(index, amount, e_value, item, item_info);
//		if (0 == result) {
//			result = ERROR_ITEM_USER_SUCCESS;
//		}
//		break;
//	}
//	case ITEM_EFFECT_T_EQUIP: {
//		result = use_resource_item(index, amount, e_value, item, item_info);
//		break;
//	}
////	case ITEM_EFFECT_T_EXCHANGE: {
////		result = use_consume_tool(index, amount, e_value, item, item_info);
////		break;
////	}
////	case ITEM_EFFECT_T_ACHIEVEMENT: {
////		result = use_consume_tool(index, amount, e_value, item, item_info);
////		break;
////	}
//	case ITEM_EFFECT_T_MOUNT: {
//		result = use_mount(index, amount, e_value, item, item_info);
//		if (0 == result) {
//			result = ERROR_ITEM_USER_SUCCESS;
//		}
//		break;
//	}
//	case ITEM_EFFECT_T_GEM_COMPOSE: {
//		result = use_gem_compose(index, amount, e_value, item, item_info);
//		//if (0 == result) {
//		//	result = ERROR_ITEM_USER_SUCCESS;
//		//}
//		break;
//	}
//	default:{
//		result = 0;//use_normal_item(index, amount, e_value, item, item_prop_config->effect, target);
//		break;
//	}
//	}
	if(!item_info.empty()) {
		Item_Basic_Info item_base;
		MSG_82100006 msg;
		for(Item_Vec::iterator it = item_info.begin(); it != item_info.end(); ++it) {
			item_base.reset();
			it->set_item_basic_info(item_base);
			msg.item_info.push_back(item_base);
		}
		THIS_SEND_TO_CLIENT(msg);
	}
	return result;
}

int Item_User::use_normal_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail* item, const Item_Prop_Effect_Config &effect, Item_Target target) {
	if (effect.name.empty()) return ERROR_CONFIG_NOT_EXIST;
	int result = 0;

	std::string effect_name = effect.name;
	int value = effect.value;

	//可能会失败的要放在前面
//	if (effect_name == Effect::LEVEL_UP){
//		result = use_xiandan_item(amount, effect);
//	}
	if (result != 0)
		return result;

	// 先扣除道具
	pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);

	// 经验，钱是可以批量使用的
	// 以下这些不太可能失败
//	if (effect_name == Effect::BLOOD) {
//		this->player_self()->restore_hp((double) value, target);
//	} else if (effect_name == Effect::MAGIC) {
//		this->player_self()->restore_mp((double) value, target);
//	} else if (effect_name == Effect::ADD_EXP) {
//		this->player_self()->fighter_exp_add(value * double(msg.amount), EXP_USE_ITEM);
//	} else
	if (effect_name == Effect::ADD_COPPER) {
		result = this->pack_add_money(Money_Add_Info(COPPER, value * double(amount),
			Money_DM_Info(MONEY_ADD_USE_ITEM, item->id_, item->amount_)));
	} else if (effect_name == Effect::ADD_GOLD) {
		if (item->bind_ == Item_Detail::BIND) {
			result = this->pack_add_money(Money_Add_Info(BIND_GOLD, value * double(amount),
				Money_DM_Info(MONEY_ADD_USE_ITEM, item->id_, item->amount_)));
		} else {
			result = this->pack_add_money(Money_Add_Info(GOLD, value * double(amount),
					Money_DM_Info(MONEY_ADD_USE_ITEM, item->id_, item->amount_)));
		}
		} else {
		result = ERROR_ITEM_UNUSABLE;
	}

	return result;
}

int Item_User::use_lottery_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {
	int result = 0;
	Item_Vec insert_item;
	Item_Vec broadcast_item;
	Item_Vec insert_item_merged;
	Item_Vec broadcast_item_merged;
	Money_Add_List money_add_list;
	int used_times = 0;
	Id_Amount_Vec item_cost;
	Money_Sub_List money_cost;
    uint32_t tmp_id = item->id_; // 主要是为了广播
	MSG_81000102 res_msg;
	res_msg.type = 0;
	std::vector<Int_Int> sp_receive;

	std::map<role_id_t, int>::iterator iter = item_user_detail_->item_had_used_times_.find(item->id_);
	if(iter != item_user_detail_->item_had_used_times_.end()) {
		used_times = iter->second;
	}

	Id_Amount_Vec item_cost_real;
	if (result == 0) {
		result = get_lottery_item(item->id_, level(), career(), amount, insert_item, broadcast_item, money_add_list, used_times,
				item_cost, money_cost,sp_receive);
		if (!item_cost.empty() || !money_cost.empty()) {
			if (!item_cost.empty()) {
				Id_Amount_Vec special_type;
				for (Id_Amount_Vec::const_iterator it_cost = item_cost.begin(); it_cost != item_cost.end(); ++it_cost) {
					if (it_cost->id < 100000) {
						special_type.push_back(*it_cost);
					} else {
						item_cost_real.push_back(*it_cost);
					}
				}
				for (Id_Amount_Vec::const_iterator it_spec = special_type.begin(); it_spec != special_type.end(); ++it_spec) {
					switch(it_spec->id) {
					case WING_LV: {
						if (player_self()->wing_detail().lv < it_spec->amount) {
							return ERROR_ITEM_USER_LOTTERY_WING_LV_NOT_ENOUGH;
						}
						break;
					}
					default:
						break;
					}
				}
			}
			if (!item_cost_real.empty()) {
				int ret = pack_try_erase_item(PACK_T_PACKAGE_INDEX, item_cost_real);
				if (ret != 0) {
					return ret;
				}
			}
			if (!money_cost.empty()) {
				int ret = pack_try_sub_money(money_cost);
				if (ret != 0) {
					return ret;
				}
			}
		}
		if (result == 0 && !insert_item.empty()) {
			merge_same_item(insert_item, insert_item_merged);
			result = pack_try_insert_item(PACK_T_PACKAGE_INDEX, insert_item_merged);
		}
		if (result == 0 && !money_add_list.empty()) {
			result = pack_try_add_money(money_add_list);
		}
	}
	if (result == 0) {
		if (!item_cost_real.empty()) {
			int ret = pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost_real, Lose_Item_DM_Info(ITEM_LOSE_USE_ITEM));
			if (ret != 0) {
				return ret;
			}
		}
		if (!money_cost.empty()) {
			int ret = pack_sub_money(money_cost);
			if (ret != 0) {
				return ret;
			}
		}

		pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
		if(used_times > 0) {
			if(iter != item_user_detail_->item_had_used_times_.end()) {
				++item_user_detail_->item_had_used_times_[item->id_];
			} else {
				item_user_detail_->item_had_used_times_[item->id_] = 1;
			}
			item_user_detail_->is_change = true;
		}
		if (!insert_item_merged.empty()) {
			pack_insert_item(PACK_T_PACKAGE_INDEX, insert_item_merged, Gain_Item_DM_Info(ITEM_GAIN_TREASURE), GET_ITEM, WITHOUT_TRY);
			merge_same_item(insert_item_merged, item_info);

			Item_Basic_Info item_base;
			Card_Info card_info;
			for (Item_Vec::iterator item_it = item_info.begin(); item_it != item_info.end(); ++item_it) {
				if (item_it->type_ == Item_Detail::CARD && item_it->amount_ > 1) {
					for (int i = 0; i < item_it->amount_; ++i) {
						item_base.reset();
						(*item_it).set_item_basic_info(item_base);
						item_base.amount = 1;
						res_msg.item_info.push_back(item_base);
						card_info.reset();
						if ((*item_it).set_item_card_info(card_info)) {
							res_msg.card_info_vec.push_back(card_info);
						}
					}
				} else {
					item_base.reset();
					(*item_it).set_item_basic_info(item_base);
					res_msg.item_info.push_back(item_base);
					card_info.reset();
					if ((*item_it).set_item_card_info(card_info)) {
						res_msg.card_info_vec.push_back(card_info);
					}
				}
			}
			//广播
			open_lottery_broadcast(tmp_id,item_info);
		}
		if (!money_add_list.empty()) {
			pack_add_money(money_add_list);
			for (Money_Add_List::iterator money_it = money_add_list.begin(); money_it != money_add_list.end();
					++money_it) {
				res_msg.property.push_back(Property((*money_it).type, (*money_it).nums));
			}
		}
		if ( !sp_receive.empty()) {
			for (std::vector<Int_Int>::iterator iter = sp_receive.begin();iter != sp_receive.end();++iter) {
				switch(iter->val_1){
				case 503703:
					player_self()->add_contribute(iter->val_2);
					res_msg.property.push_back(Property(iter->val_1,iter->val_2));
					break;
				default:
				    break;
				}
			}
		}
	}

	if (!res_msg.item_info.empty() || !res_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(res_msg);
	}

	return result;
}

int Item_User::use_drug_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {
	int use_amount = amount;
	const Item_Drug_Detail *item_drug = CONFIG_CACHE_ITEM->find_item_drug(item->id_);
	if (NULL == item_drug) {
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item->id_);
		if (NULL == item_config) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (item_config && item_config->special_cfg.trigger_buff.status_id) {
			MSG_20100305 inner_msg;
			inner_msg.tp = ITEM_USE_BUFF;
			inner_msg.val = item->id_;
			inner_msg.index = index;
			inner_msg.amount = amount;
			THIS_SEND_TO_SCENE(inner_msg);
			if (item_config->special_cfg.insert_hero) {
				Status_Effect_Argv argv = item_config->special_cfg.trigger_buff;
				for (int i = 0; i < amount; ++i) {
					player_self()->hero_insert_status(argv);
				}
			}
			pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
			{
				MSG_81000104 tip_msg;
				for (int i = 0; i < amount; ++i) {
					for (Effect_Argv_Prop_Vec::const_iterator it_ad = item_config->special_cfg.trigger_buff.data.begin();
							it_ad != item_config->special_cfg.trigger_buff.data.end(); ++it_ad) {
						double val = 0.0;
						if (PT_BLOOD_STEAL == it_ad->i_effect_val1) {
							val = it_ad->d_effect_val1 / 100.0;
						} else if (it_ad->d_effect_val1 > 0.00000000001) {
							val = it_ad->d_effect_val1;
						} else {
							val = it_ad->d_effect_val2 / 100.0;
						}
						if (it_ad->i_effect_val1 > 0) {
							tip_msg.prop.push_back(Property(it_ad->i_effect_val1, val));
						} else if (it_ad->i_effect_val1 == 0) {
							tip_msg.prop.push_back(Property(502503, val));
						}
					}
				}
				THIS_SEND_TO_CLIENT(tip_msg);
			}
			return ERROR_ITEM_USER_SUCCESS;
		}
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (item_drug->usetype == ITEM_USE_DRUG_BLOOD_POOL) {
		const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
		if (sc && (sc->type == War_Scene_Type || sc->type == GANG_WAR_TYPE)) {
			return ERROR_ITEM_USER_SCENE_LIMIT;
		}
	}else if(item_drug->usetype == ITEM_USE_DRUG_STRENGTH){
		int vip_lv = player_self()->vip();
		int max_buy_times = CONFIG_CACHE_FIGHTER->get_phy_power_max_buy_times(
				vip_lv);
		int cur_buy_times = player_self()->fighter_detail().phy_power_buy_times;
		int phy_power_cur = player_self()->fighter_detail().current_strength();
		if (phy_power_cur >= player_self()->fighter_detail().max_strength()) {
			return ERROR_PHY_POWER_BUY_VALUE_FULL;
		}
		if (cur_buy_times >= max_buy_times) {
			return ERROR_PHY_POWER_BUY_TIMES_FULL;
		}
		if(cur_buy_times+amount > max_buy_times){
			use_amount = max_buy_times - cur_buy_times;
		}
	}
	MSG_20100305 inner_msg;
	inner_msg.tp = ITEM_USE_DRUG;
	inner_msg.val = item_drug->val;
	inner_msg.index = index;
	inner_msg.amount = use_amount;
	Block_Buffer buf;
	buf.make_message(inner_msg.msg_id);
	inner_msg.serialize(buf);
	buf.write_int32(item->id_);
	buf.write_int8(item_drug->usetype);
	buf.finish_message();
	this->send_to_scene(buf);

	Packer::lock_item(index);
	return 0;
}

int Item_User::use_material_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {
//	const Item_Resource_Detail *item_drug = CONFIG_CACHE_ITEM->find_item_resource(item->id_);
//	if (item_drug) {
//		return use_resource_item(index, amount, e_value, item, item_info);
//	}
	const Item_Material_Detail *cfg = CONFIG_CACHE_ITEM->find_item_material(item->id_);
	if (NULL == cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	int result = 0;
	if (cfg->from_table == "task_check") {
		result = this->player_self()->is_consume_reward_token(item->id_, amount);
		if (0 == result) {
			pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
		}
	}
	Item_Effect_Type item_type = get_item_type(item->id_);
	if (0 == result) {
		if (item_type == ITEM_EFFECT_T_MATERIAL) {
			result = ERROR_ITEM_USER_REWARD_SUCCESS;
		} else {
			result = ERROR_ITEM_USER_CARD_REWARD;
		}
	}
	return result;
}

int Item_User::use_card_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item->id_);
	if (NULL == item_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	Item_Effect_Type type = get_item_type_third(item->id_);
	switch(type) {
	case ITEM_EFFECT_T_CARD_VIP:{
		bool has_used = false;
		const Item_Card_Detail *item_card = CONFIG_CACHE_ITEM->find_item_card(item->id_);
		if (NULL == item_card) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (item_card->vip_lvl > 0 && item_card->deadline) {
			player_self()->open_vip_hours(item_card->deadline * amount, item_card->vip_lvl);
			has_used =true;
		}
		if (item_card->covenant_type > 0 && item_card->deadline) {
			player_self()->open_covenant_by_card(item_card->covenant_type, item_card->deadline * amount);
			has_used =true;
		}
		if (has_used) {
			pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
		}
		return 0;
		break;
	}
	case ITEM_EFFECT_T_CARD_BUFF:
	case ITEM_EFFECT_T_CARD_OTHER: {
		MSG_20100305 inner_msg;
		inner_msg.tp = ITEM_USE_BUFF;
		inner_msg.val = item->id_;
		inner_msg.index = index;
		inner_msg.amount = amount;
		THIS_SEND_TO_SCENE(inner_msg);
		if (item_config->special_cfg.insert_hero) {
			Status_Effect_Argv argv = item_config->special_cfg.trigger_buff;
			for (int i = 0; i < amount; ++i) {
				player_self()->hero_insert_status(argv);
			}
		}
		pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);

		{
			MSG_81000104 tip_msg;
			if (item_config->special_cfg.trigger_buff.status_id) {
				for (int i = 0; i < amount; ++i) {
					for (Effect_Argv_Prop_Vec::const_iterator it_ad = item_config->special_cfg.trigger_buff.data.begin();
							it_ad != item_config->special_cfg.trigger_buff.data.end(); ++it_ad) {
						if (it_ad->i_effect_val1) {
							double val = 0.0;
							if (PT_BLOOD_STEAL == it_ad->i_effect_val1) {
								val = it_ad->d_effect_val1 / 100.0;
							} else if (it_ad->d_effect_val1 > 0.00000000001) {
								val = it_ad->d_effect_val1;
							} else {
								val = it_ad->d_effect_val2 / 100.0;
							}
							tip_msg.prop.push_back(Property(it_ad->i_effect_val1, val));
						} else if (it_ad->i_effect_val1 == 0) {
							double val = it_ad->d_effect_val1 / 100.0;
							tip_msg.prop.push_back(Property(502503, val));
						}
					}
				}
			}
			THIS_SEND_TO_CLIENT(tip_msg);
		}
		return ERROR_ITEM_USER_SUCCESS;
		break;
	}
	case ITEM_EFFECT_T_CARD_HERO: {
		if (item_config->special_cfg.type != 2) {
			return ERROR_CLIENT_OPERATE;
		}
		int ret = player_self()->hero_add(item_config->special_cfg.addon.iu_hero.id, false, true);
		if(0 == ret) {
			int hero_index = player_self()->heroer_get_hero_index(item_config->special_cfg.addon.iu_hero.id);
			Heroer_Detail::Hero_Map::const_iterator it_hero = player_self()->heroer_detail().hero_map.find(hero_index);
			if (it_hero == player_self()->heroer_detail().hero_map.end()) {
				return ERROR_HERO_NOT_EXIST;
			}
			while(it_hero->second.level < item_config->special_cfg.addon.iu_hero.lvl
						|| it_hero->second.awake_lvl < item_config->special_cfg.addon.iu_hero.wake_lvl) {
				int ret = player_self()->req_hero_level_up(hero_index, true);
				if (-2 == ret) {
					ret = player_self()->req_hero_awake(true, hero_index, true);
					if (0 != ret) {
						break;
					}
				} else if (0 != ret) {
					break;
				}
			}
			ret = pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
		}
		return ret;
		break;
	}
	default:
		break;
	}

	if (item_config->special_cfg.trigger_buff.status_id) {
		MSG_20100305 inner_msg;
		inner_msg.tp = ITEM_USE_BUFF;
		inner_msg.val = item->id_;
		inner_msg.index = index;
		inner_msg.amount = amount;
		THIS_SEND_TO_SCENE(inner_msg);
		pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
		{
			MSG_81000104 tip_msg;
			for (int i = 0; i < amount; ++i) {
				for (Effect_Argv_Prop_Vec::const_iterator it_ad = item_config->special_cfg.trigger_buff.data.begin();
						it_ad != item_config->special_cfg.trigger_buff.data.end(); ++it_ad) {
					double val = 0.0;
					if (PT_BLOOD_STEAL == it_ad->i_effect_val1) {
						val = it_ad->d_effect_val1 / 100.0;
					} else if (it_ad->d_effect_val1 > 0.00000000001) {
						val = it_ad->d_effect_val1;
					} else {
						val = it_ad->d_effect_val2 / 100.0;
					}
					if (it_ad->i_effect_val1 > 0.00000000001) {
						tip_msg.prop.push_back(Property(it_ad->i_effect_val1, val));
					} else if (it_ad->i_effect_val1 == 0) {
						tip_msg.prop.push_back(Property(502503, val));
					}
				}
			}
			THIS_SEND_TO_CLIENT(tip_msg);
		}
		return ERROR_ITEM_USER_SUCCESS;
	}

	return 0;
}

int Item_User::use_resource_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {

	Item_Vec item_list;
	Item_Vec insert_item_merged;
	Money_Add_List money_add_list;
	std::vector<Int_Int> spe_item;
	int result = get_resource_item(item_list,money_add_list,spe_item,item->id_,amount);
	if ( 0 != result )
		return result;
	merge_same_item(item_list, insert_item_merged);
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	result = pack_try_erase_item(Index_Amount(index, amount));
	if ( 0!= result )
		return result;
    if ( !insert_item_merged.empty() ) {

    	result = pack_try_insert_item(PACK_T_PACKAGE_INDEX,insert_item_merged);
    }
    if (0 == result && !money_add_list.empty() )
    	result = pack_try_add_money(money_add_list);
    if ( 0 == result && !spe_item.empty() ) {
    	for(std::vector<Int_Int>::iterator it = spe_item.begin();it != spe_item.end();++it ) {
    		switch(it->val_1){
    		case 502503:{
    			MSG_20100305 inner_msg;
    			inner_msg.tp = ITEM_USE_EXP;
    			inner_msg.val = it->val_2;
    			inner_msg.index = index;
    			inner_msg.amount = amount;
    		    THIS_SEND_TO_SCENE(inner_msg);

    			acv_msg.property.push_back(Property(502503, it->val_2 * amount));

    			break;
    		}
    		case 503003:{
    			player_self()->change_exploit_val(it->val_2 * amount);
    			acv_msg.property.push_back(Property(503003, it->val_2 * amount));

    			break;
    		}
    		default:
    			break;
    		}
    	}
    }
    if ( 0 != result )
    	return result;
    pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
    if ( !insert_item_merged.empty() ) {
    	pack_insert_item(PACK_T_PACKAGE_INDEX, insert_item_merged, Gain_Item_DM_Info(ITEM_GAIN_USE_ITEM), GET_ITEM, WITHOUT_TRY);
    	merge_same_item(item_list, item_info);
    }
    if ( !money_add_list.empty() )
    	pack_add_money(money_add_list);
    client_get_item_effect(item_info,money_add_list,acv_msg);


//	const Item_Resource_Detail *item_drug = CONFIG_CACHE_ITEM->find_item_resource(item->id_);
//
//	if ( NULL == item_drug || item_drug->val == 0 )
//		return ERROR_CONFIG_NOT_EXIST;
//	const Item_Detail_Config *ic = CONFIG_CACHE_ITEM->find_item(item_drug->prop);
//
//	if ( NULL == ic ) {
//		if ( (item_drug->prop != COPPER && item_drug->prop != SOULS
//					&& item_drug->prop != FRIENDSHIP && item_drug->prop != 502503
//					&& item_drug->prop != 503003
//					&& item_drug->prop != GOLD && item_drug->prop != BIND_GOLD
//					&& item_drug->prop != DRAGON_SOUL
//		)) {
//			return ERROR_CONFIG_NOT_EXIST;
//		}
//	}
//
//	MSG_81000102 acv_msg;
//	acv_msg.type = 0;
//	int result = 0;
//
//	if ( NULL == ic ) {
//		switch(item_drug->prop) {
//		case COPPER:
//			result = pack_add_money(Money_Add_Info(COPPER, item_drug->val * amount,
//					Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, item->id_, 1)));
//			if (0 == result) {
//				pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//				acv_msg.property.push_back(Property(COPPER, item_drug->val * amount));
//			}
//			break;
//		case 502503: {
//			MSG_20100305 inner_msg;
//			inner_msg.tp = ITEM_USE_EXP;
//			inner_msg.val = item_drug->val;
//			inner_msg.index = index;
//			inner_msg.amount = amount;
//			THIS_SEND_TO_SCENE(inner_msg);
//
//			if (0 == result) {
//				pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//				acv_msg.property.push_back(Property(502503, item_drug->val * amount));
//			}
//				break;
//		}
//		case DRAGON_SOUL: {
//			result = pack_add_money(Money_Add_Info(DRAGON_SOUL, item_drug->val * amount,
//					Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, item->id_, 1)));
//			if (0 == result) {
//				pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//				acv_msg.property.push_back(Property(DRAGON_SOUL, item_drug->val * amount));
//			}
//			break;
//		}
//		case SOULS: {
//			result = pack_add_money(Money_Add_Info(SOULS, item_drug->val * amount,
//					Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, item->id_, 1)));
//			if (0 == result) {
//				pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//				acv_msg.property.push_back(Property(SOULS, item_drug->val * amount));
//			}
//			break;
//		}
//		case FRIENDSHIP: {
//			result = pack_add_money(Money_Add_Info(FRIENDSHIP, item_drug->val * amount,
//					Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, item->id_, 1)));
//			if (0 == result) {
//				pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//				acv_msg.property.push_back(Property(FRIENDSHIP, item_drug->val * amount));
//			}
//				break;
//		}
//		case GOLD: {
//			result = pack_add_money(Money_Add_Info(GOLD, item_drug->val * amount,
//					Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, item->id_, 1)));
//			if (0 == result) {
//				pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//				acv_msg.property.push_back(Property(GOLD, item_drug->val * amount));
//			}
//			break;
//		}
//		case BIND_GOLD: {
//			result = pack_add_money(Money_Add_Info(BIND_GOLD, item_drug->val * amount,
//					Money_DM_Info(Pack::MONEY_ADD_USE_ITEM, item->id_, 1)));
//			if (0 == result) {
//				pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//				acv_msg.property.push_back(Property(BIND_GOLD, item_drug->val * amount));
//			}
//			break;
//		}
//		case 503003: {
//			player_self()->change_exploit_val(item_drug->val * amount);
//			if (0 == result) {
//				pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//				acv_msg.property.push_back(Property(503003, item_drug->val * amount));
//			}
//			break;
//		}
//		default:
//			break;
//		}
//	} else {
//		// 换取道具
//		Item_Detail new_item;
//		new_item.reset();
//		new_item.set_basic(0, item_drug->prop, item_drug->val *amount , ic->bind);
//		result = pack_try_insert_item(PACK_T_PACKAGE_INDEX,new_item);
//		if ( 0 != result )
//			return result;
//		pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
//		pack_insert_item(PACK_T_PACKAGE_INDEX, new_item, Gain_Item_DM_Info(ITEM_GAIN_TREASURE), GET_ITEM, WITHOUT_TRY);
//
//		Item_Basic_Info item_base;
//		Card_Info card_info;
//		if (new_item.type_ == Item_Detail::CARD && new_item.amount_ > 1) {
//			for (int i = 0; i < new_item.amount_; ++i) {
//			    item_base.reset();
//			    new_item.set_item_basic_info(item_base);
//				item_base.amount = 1;
//				acv_msg.item_info.push_back(item_base);
//				card_info.reset();
//				if (new_item.set_item_card_info(card_info)) {
//					acv_msg.card_info_vec.push_back(card_info);
//				}
//			}
//		} else {
//			item_base.reset();
//			new_item.set_item_basic_info(item_base);
//			acv_msg.item_info.push_back(item_base);
//			card_info.reset();
//			if (new_item.set_item_card_info(card_info)) {
//				acv_msg.card_info_vec.push_back(card_info);
//		    }
//		}
//		item_info.push_back(new_item);
//	}
//
//	if (0 == result) {
//		result = ERROR_ITEM_USER_SUCCESS;
//		if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
//			THIS_SEND_TO_CLIENT(acv_msg);
//		}
//	}
    if (0 == result )
    	result = ERROR_ITEM_USER_SUCCESS;
	return result;
}

int Item_User::use_consume_tool(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {
	int result = 0;
	uint32_t item_id = CONFIG_CACHE_PACKAGE->package_config().super_item_id;
	if (item_id == item->id_) {
		result = this->pack_add_capacity(0, PACK_T_PACKAGE_INDEX, amount, 0);
		if (ERROR_ADD_CAPACITY_SUCCESS == result) {
			pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
		}
	} else {
		return use_material_item(index, amount, e_value, item, item_info);
	}

	return result;
}

int Item_User::use_mount(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {
	int result = 0;
	int horse_id = CONFIG_CACHE_MOUNT->find_horse_id(item->id_);
	if (horse_id > 0) {
		result = player_self()->mount_activate_by_item(horse_id);//pack_add_capacity(0, PACK_T_PACKAGE_INDEX, amount, 0);
		if (0 == result) {
			pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
		}
	}

	return result;
}

int Item_User::use_gem_compose(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item->id_);
	if (NULL == item_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	const Equip_Formula_Cfg *equip_formula_cfg = CONFIG_CACHE_EQUIP->find_equip_formula(item_config->item_id);
	if (NULL == equip_formula_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	int form = 0;
	if(!equip_formula_cfg->formula.empty()){
		int idx = Utility_Func::lottery_one(equip_formula_cfg->weight);
		form = equip_formula_cfg->formula[idx];
	}

	player_self()->set_gem_compose_id(form);

	pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);
	return 0;
}

int Item_User::use_little_fun(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info) {

	const Item_Little_Fuction_Config *item_cof = CONFIG_CACHE_ITEM->find_item_little_fun(item->id_);
	int result = 0;
	if ( NULL == item_cof )
		return ERROR_CONFIG_NOT_EXIST;
	switch( item_cof->type ) {
	case FUN_MODIFY_GENDER: {
		result = use_item_modify_gender(index,amount,e_value,item);
		break;
	}
	case FUN_SENCE_TRAN: {
		result = use_item_tran_sence(index,amount,e_value,item);
		break;
	}
	default:
		break;
	}
	return result;
}

int Item_User::use_normal_item(const Money_Sub_Info &money_sub_info, const Item_Detail &item, const Json::Value &effect,
		const Item_Target &target) {
	if (effect == Json::Value::null) return ERROR_CONFIG_NOT_EXIST;
	int result = 0;

	std::string effect_name = effect["name"].asString();
	int value = effect["value"].asInt();

	// 先扣除money
	result = pack_sub_money(money_sub_info);
	if (result != 0) {
		return result;
	}

	// 经验，钱是可以批量使用的
	if (effect_name == Effect::ADD_COPPER) {
		result = this->pack_add_money(Money_Add_Info(COPPER, value * double(item.amount_),
			Money_DM_Info(MONEY_ADD_USE_ITEM, item.id_, item.amount_)));
	} else {
		result = ERROR_ITEM_UNUSABLE;
	}

	return result;
}

int Item_User::use_status_item(const Money_Sub_Info &money_sub_info, const Item_Detail &item, int status_id, int target) {
	int result = 0;
	return result;
}

int Item_User::use_lottery_item(const Money_Sub_Info &money_sub_info, const Item_Detail &item) {
	if (item.amount_ > 1) return ERROR_CLIENT_OPERATE;

	int result = 0;
	Item_Vec insert_item;
	Item_Vec broadcast_item;
	Money_Add_List money_add_list;
	Id_Amount_Vec item_cost;
	Money_Sub_List money_cost;
	int used_times = 0;
	std::vector<Int_Int> sp_recive;

	std::map<role_id_t, int>::iterator iter = item_user_detail_->item_had_used_times_.find(item.id_);
	if(iter != item_user_detail_->item_had_used_times_.end()) {
		used_times = iter->second;
	}
	if (result == 0) {
		result = get_lottery_item(item.id_, level(), career(),
				insert_item, broadcast_item, money_add_list, used_times,
				item_cost, money_cost,sp_recive);
		if (!item_cost.empty() || !money_cost.empty()) {
			if (!item_cost.empty()) {
				int ret = pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost, Lose_Item_DM_Info(ITEM_LOSE_USE_ITEM));
				if (ret != 0) {
					return ret;
				}
			}
			if (!money_cost.empty()) {
				int ret = pack_sub_money(money_cost);
				if (ret != 0) {
					return ret;
				}
			}
		}

		if (result == 0 && !insert_item.empty()) {
			result = pack_try_insert_item(PACK_T_PACKAGE_INDEX, insert_item);
		}
		if (result == 0 && !money_add_list.empty()) {
			result = pack_try_add_money(money_add_list);
		}
	}
	if (result == 0) {
		result = pack_sub_money(money_sub_info);
		if (result == 0) {
			if (!insert_item.empty()) {
				pack_insert_item(PACK_T_PACKAGE_INDEX, insert_item, Gain_Item_DM_Info(ITEM_GAIN_TREASURE), GET_ITEM, WITHOUT_TRY);
			}
			if (!money_add_list.empty()) {
				pack_add_money(money_add_list);
			}
		}
	}

	return result;
}

int Item_User::use_drug_inner(const uint32_t index, const double val, const int amount, Block_Buffer &buf) {
	int ret = 0;
	if (index > 0) {
		Packer::unlock_item(index);
		if (amount > 0) {
			const Item_Detail *item = pack_get_const_item(index);
			if (NULL == item) return this->send_err_to_client(ERROR_ITEM_NOT_EXIST, 10100106 );
			const Item_Drug_Detail *item_drug = CONFIG_CACHE_ITEM->find_item_drug(item->id_);
			if (NULL == item_drug) {
				return this->send_err_to_client(ERROR_CONFIG_NOT_EXIST, 10100106 );
			}
			ret = pack_erase_item(Index_Amount(index, amount), Lose_Item_DM_Info(ITEM_LOSE_USE), USE_ITEM, WITHOUT_TRY);

			MSG_81000104 tip_msg;
			switch(item_drug->usetype) {
			case ITEM_USE_DRUG_BLOOD_POOL: {
				tip_msg.prop.push_back(Property(PT_BLOOD_POOL_CURRENT, item_drug->val * amount));
				break;
			}
			case ITEM_USE_DRUG_STRENGTH: {
//				tip_msg.prop.push_back(Property(PT_STRENGTH_CURRENT, item_drug->val * amount));
				int cur_buy_times = player_self()->fighter_detail().phy_power_buy_times;
				int phy_power_cur = player_self()->fighter_detail().current_strength();
				int phy_power_recover_time = player_self()->fighter_detail().phy_power_recover_time;
				cur_buy_times += amount;
				player_self()->sync_phy_power_info(phy_power_cur, phy_power_recover_time,
						cur_buy_times);
				{
					MSG_20100030 msg;
					msg.phy_power_buy_times = cur_buy_times;
					msg.phy_power_cur = phy_power_cur;
					msg.phy_power_recover_time = phy_power_recover_time;
					THIS_SEND_TO_SCENE(msg);
				}
				break;
			}
			default:
				break;
			}
			if (!tip_msg.prop.empty()) {
				THIS_SEND_TO_CLIENT(tip_msg);
			}
		} else {
			const Item_Detail *item = pack_get_const_item(index);
			if (NULL == item) return this->send_err_to_client(ERROR_ITEM_NOT_EXIST, 10100106 );
			const Item_Drug_Detail *item_drug = CONFIG_CACHE_ITEM->find_item_drug(item->id_);
			if (NULL == item_drug) {
				return this->send_err_to_client(ERROR_CONFIG_NOT_EXIST, 10100106 );
			}
			switch(item_drug->usetype) {
			case ITEM_USE_DRUG_BLOOD_POOL: {
				return this->send_err_to_client(ERROR_PACK_BLOOD_POOL_FULL, 10100106 );
			}
			case ITEM_USE_DRUG_STRENGTH: {
				return this->send_err_to_client(ERROR_PACK_STRENGTH_FULL, 10100106 );
			}
			default:
				break;
			}
		}
	} else {
		int item_id = 0;
		buf.read_int32(item_id);
		if (item_id <= 0) {
			return ERROR_CLIENT_OPERATE;
		}
		const Quick_Buy *qb = CONFIG_CACHE_ITEM->find_quick_buy(item_id);
		const Item_Drug_Detail *item_drug = CONFIG_CACHE_ITEM->find_item_drug(item_id);
		if (NULL == qb || qb->price <= 0 || NULL == item_drug) {
			return this->send_err_to_client(ERROR_CONFIG_NOT_EXIST, 10100106 );
		}
		if (amount > 0) {
			int res = player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(qb->type), qb->price, Pack::MONEY_SUB_QUICK_BUY));
			if(res) { this->send_err_to_client(res, 10100106 ); }
		} else {
			switch(item_drug->usetype) {
			case ITEM_USE_DRUG_BLOOD_POOL: {
				return this->send_err_to_client(ERROR_PACK_BLOOD_POOL_FULL, 10100106 );
			}
			case ITEM_USE_DRUG_STRENGTH: {
				return this->send_err_to_client(ERROR_PACK_STRENGTH_FULL, 10100106 );
			}
			default:
				break;
			}
		}
	}
	if (0 == ret) {
		this->send_err_to_client(ERROR_PACK_USE_DRUG_SUCCESS, 10100106 );
	}
	return ret;
}

Item_Target Item_User::get_item_target(const uint32_t item_id) {
	const Item_Prop_Detail_Config *item_prop_config = 0;//CONFIG_CACHE_ITEM->find_item_prop(item_id);
	if (!item_prop_config) return ITEM_TARGET_PLAYER;

	switch (item_prop_config->target) {
	case 5:
		return ITEM_TARGET_PET;
	default:
		return ITEM_TARGET_PLAYER;
	}
}

bool Item_User::is_item_batch_using(const uint32_t item_id) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_id);
	if(!item_config) {
		return 0;
	}
	return item_config->overlay;
}

int Item_User::get_item_cooldown_info(const uint32_t item_id, CD_Type &type, Time_Value &cd) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_id);
	if (!item_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	int item_cd = item_config->cdTime;
	if (item_cd < 0) {
		item_cd = 0;
	} else if (item_cd > 0) {
		int cool_type = CD_T_ALL;//item_config->cool_type;
		if (cool_type < CD_T_VEC_SIZE) {
			type = static_cast<CD_Type>(cool_type);
		} else {
			MSG_USER("configure error! item_id : %d", item_id);
			return ERROR_CONFIG_NOT_EXIST;
		}
	}
	cd.set(item_cd, 0);
	return 0;
}

int Item_User::use_item_tran_sence(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item) {

	const Item_Little_Fuction_Config *item_cof = CONFIG_CACHE_ITEM->find_item_little_fun(item->id_);
	if (item_cof && item_cof->type == FUN_SENCE_TRAN ) {
		if ( item_cof->arg == 0 )
			return ERROR_CONFIG_NOT_EXIST;
		if ( player_self()->location().scene_id == item_cof->arg )
			return ERROR_TRANSMIT_SAME_SCENE;
	    int result = pack_erase_item(Index_Amount(index, 1), Lose_Item_DM_Info(ITEM_LOSE_USE_ITEM), USE_ITEM, WITHOUT_TRY);
        if (0 != result)
        	return result;
        int des_sence = item_cof->arg;
        player_self()->inner_transmit(des_sence,0,role_id());
	}
	return 0;
}
int Item_User::use_item_modify_gender(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item) {

	const Item_Little_Fuction_Config *item_cof = CONFIG_CACHE_ITEM->find_item_little_fun(item->id_);
	if (item_cof && item_cof->type == FUN_MODIFY_GENDER ) {
	    int result = pack_erase_item(Index_Amount(index, 1), Lose_Item_DM_Info(ITEM_LOSE_USE_ITEM), USE_ITEM, WITHOUT_TRY);
		if (0 != result)
		    return result;
		player_self()->modify_gender();
	}
	return 0;
}

void Item_User::open_lottery_broadcast(const uint32_t item_id,const Item_Vec &item_list) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_id);
	if ( NULL == item_config)
		return;
	const Item_Lottery_Detail_Config *lottery_config = CONFIG_CACHE_ITEM->find_item_lottery(item_id);
	Int_Hash_Set broadcast_item;
	if ( lottery_config ) {
		for(Item_Lottery_Item_Info_Vec_Config::const_iterator it = lottery_config->item_list.begin();
				it != lottery_config->item_list.end();++it) {
		    for ( Int_Hash_Set::const_iterator p_it = it->item_set.begin();p_it != it->item_set.end();++p_it) {
				broadcast_item.insert(*p_it);
			}
		}
	}
	if ( !item_list.empty() && !broadcast_item.empty()) {

		for (Item_Vec::const_iterator iter = item_list.begin();iter != item_list.end();++iter ) {
			if ( !broadcast_item.count(iter->id_) )
				continue;
			const Item_Detail_Config *tmp_item_config = CONFIG_CACHE_ITEM->find_item(iter->id_);
			if ( tmp_item_config ) {
				Chat_Item_Info item_info;
				item_info.reset();
				item_info.item_base_info.id = item_config->item_id;
				item_info.item_base_info.amount = 0;
				item_info.item_base_info.bind = Item_Detail::UNBIND;

				Chat_Item_Info get_item_info;
				get_item_info.reset();
				iter->set_item_basic_info(get_item_info.item_base_info);
				get_item_info.item_base_info.amount = 0;

				player_self()->create_ann_open_lottery(player_self(),item_info,get_item_info);
			}
//				player_self()->create_ann_open_lottery(player_self(),item_config->name,tmp_item_config->name);
		}
	}
}

// EFFECT START	 ////////////////////////////////////////////////////

const std::string Effect::BLOOD = "blood";					//加生命
const std::string Effect::MAGIC = "magic";					//加灵气
const std::string Effect::COMPREHENSION = "comprehension";	//加悟性
const std::string Effect::ANGER = "anger";					//加怒气
const std::string Effect::LEVEL_UP = "level_up";			//升级
const std::string Effect::CHANGE_GENDER = "change_gender";	//改变性别

const std::string Effect::ADD_EXP = "add_exp";				//加经验
const std::string Effect::ADD_COUPON = "add_coupon";		//加礼券
const std::string Effect::ADD_COPPER = "add_copper";		//加铜钱
const std::string Effect::ADD_MOUNT_FACADE = "add_mount_facade";//加坐骑
const std::string Effect::ADD_FACTION_MEDAL = "add_faction_medal";//加帮派勋章
const std::string Effect::ADD_FACTION_CONTRIBUTION = "add_faction_contribution"; //加帮派贡献
const std::string Effect::ADD_FACTION_LEVEL = "add_faction_level"; //加帮派等级
const std::string Effect::ADD_FACTION_GOLD = "add_faction_gold";  //加帮派荒金
const std::string Effect::ADD_PET_FAME = "add_pet_fame";	//加宠物声望
const std::string Effect::ADD_PET_LING = "add_pet_ling";	//加宠物灵光值
const std::string Effect::ADD_CARD_SOUL = "add_card_soul";				//加卡牌魂魄
const std::string Effect::ADD_GOLD = "add_gold";						// 加锁妖塔可进入次数
const std::string Effect::ADD_TIME_TIANDI_CIFU = "add_time_tiandi_cifu";		//加天帝赐福时间
const std::string Effect::ADD_TIME_ZHANSHEN_CIFU = "add_time_zhanshen_cifu";	//加战神赐福时间
const std::string Effect::ADD_TIME_YUZHOU_CIFU = "add_time_yuzhou_cifu";		//加云洲赐福时间
const std::string Effect::ADD_TIME_TIANGUAN_CIFU = "add_time_tianguan_cifu";	//加天官赐福时间
const std::string Effect::ADD_FRAGMENT_LEFT = "add_fragment_left";				//加神器碎片
const std::string Effect::ADD_BLUE_SOUL = "blue_soul";							//加蓝魂值
const std::string Effect::ADD_PURPLE_SOUL = "purple_soul";						//加紫魂值
const std::string Effect::ADD_MULTI_ARENA_HONOR = "multi_arena_honor";			//加跨服荣誉

// EFFECT END	 ////////////////////////////////////////////////////
