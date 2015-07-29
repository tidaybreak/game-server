/*
 * Activities_Stores.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include <activities_stores/Activities_Stores.h>
#include <activities_stores/Activities_Stores_Struct.h>
#include "Logic_Module_Head.h"
#include "Activities_Stores_Manager.h"
#include "Msg_Activities_Stores_Struct.h"
#include "Err_Code.h"
#include "pack/Pack_Def.h"
#include "Msg_Active_Struct.h"
#include "pack/Pack_Struct.h"
#include "Logic_Player.h"
#include "active/Active_Manager.h"

using namespace Pack;

Activities_Stores::Activities_Stores(void) {
	reset();
}

Activities_Stores::~Activities_Stores() {
	// TODO Auto-generated destructor stub
}

void Activities_Stores::reset(void) {
	activities_stores_detail_ = 0;
}

int Activities_Stores::create_init(Activities_Stores_Detail &detail) {

	return 0;
}

void Activities_Stores::load_detail(Activities_Stores_Detail &detail) {
	activities_stores_detail_ = &detail;
}

void Activities_Stores::save_detail(Activities_Stores_Detail &detail) {
	if (activities_stores_detail().is_change) {
		detail = activities_stores_detail();
		activities_stores_detail_->is_change = false;
	}
}

int Activities_Stores::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int Activities_Stores::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Activities_Stores::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Activities_Stores::move_scene_completed(void) {

	return 0;
}

int Activities_Stores::client_ready_now(void) {

	return 0;
}

void Activities_Stores::module_init(void) {

}

void Activities_Stores::sign_in(void) {

}

void Activities_Stores::sign_out(void) {

}

void Activities_Stores::trusteeship_sign_out(void) {

}

int Activities_Stores::sign_in_repeat(void) {

	return 0;
}

int Activities_Stores::req_open_activity_exchange_mall(void) {
	if(!ACTIVITIES_STORES_MANAGER->is_open()) {
		return ERROR_IS_MALL_NOT_OPEN;
	}
	MSG_50106900 msg;
	Activity_Exchange_Mall_Goods_Info_Map &map  = ACTIVITIES_STORES_MANAGER->get_goods_map();
	for(Activity_Exchange_Mall_Goods_Info_Map::iterator it = map.begin(); it != map.end(); ++it) {
		if(ACTIVE_MANAGER->get_active_up_day(it->second.act_id) == 0) {
			continue;
		}
		Activity_Exchange_Mall_Goods_Info info = it->second;
		info.already_exchage = 0;
		UInt_UInt_Map::iterator iter = activities_stores_detail_->id_num_buy.find(info.id);
		if(iter != activities_stores_detail_->id_num_buy.end()) {
			info.already_exchage = iter->second;
		}
		msg.goods.push_back(info);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Activities_Stores::req_exchage_activity_exchange_goods(uint32_t id, uint32_t num) {
	if(!ACTIVITIES_STORES_MANAGER->is_open()) {
		return ERROR_IS_MALL_NOT_OPEN;
	}
	Activity_Exchange_Mall_Goods_Info *goods = ACTIVITIES_STORES_MANAGER->get_goods_by_id(id);
	if(goods) {
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		if(ACTIVE_MANAGER->get_active_up_day(goods->act_id) == 0) {
			return ERROR_EXCHANGE_NOT_ACTIVE;
		}
		if(goods->limit_lv > (uint32_t)player_self()->level()) {
			return ERROR_EXCHAGE_LEVEL_LIMIT;
		}
		if(goods->limit_exchange) {
			UInt_UInt_Map::iterator it = activities_stores_detail_->id_num_buy.find(goods->id);
			if(it != activities_stores_detail_->id_num_buy.end()) {
				if( goods->limit_exchange < it->second + num) {
					return ERROR_EXCHANGE_LIMIT;
				}
			}
		}
		//扣除道具 或钱
		if(goods->exchange_id > 999999) {
			//道具
			int return_status = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(goods->exchange_id, goods->exchange_num*num), Lose_Item_DM_Info(ITEM_LOSE_ACTIVITY_EXCHANGE_MALL));
			if(return_status != 0) {
				return return_status;
			}
		} else {
			//钱
			uint32_t pay_type = 0;
			switch(goods->exchange_id) {
				case 	COPPER: { //金币
					pay_type = COPPER_ONLY;
					break;
				}
				case GOLD : { //钻石
					pay_type = GOLD_ONLY;
					break;
				}
				case BIND_GOLD: { //粉钻
					pay_type = BIND_GOLD_ONLY;
					break;
				}
				case DRAGON_SOUL: {// 龙魂
					pay_type = DRAGON_SOUL_ONLY;
					break;
				}
				case SOULS: {//英灵
					pay_type = SOULS_ONLY;
					break;
				}
				case FRIENDSHIP: {//友情值
					pay_type = FRIENDSHIP_ONLY;
					break;
				}
				case REPUTATION: {//声望
					pay_type = REPUTATION_ONLY;
					break;
				}
				default: {
					return 0;
				}
			}
			if(pay_type) {
				int res = player_self()->pack_sub_money(Money_Sub_Info(Pack::Money_Sub_Type(pay_type), goods->exchange_num*num, Money_DM_Info(Pack::MONEY_SUB_ACTIVITY_EXCHANGE_MALL, goods->item_id, num)));
				if(res) {
					return res;
				}
			}
		}
		//得到道具 或钱
		if(goods->item_id > 999999) {
			//道具
			Item_Detail item = Item_Detail(goods->item_id, num, Item_Detail::UNDEFINED);
			int res = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX,item, Gain_Item_DM_Info(ITEM_GAIN_ACTIVITY_EXCHANGE_MALL));
			if(res) {
				return res;
			}
			Item_Basic_Info ibi;
			item.set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
		} else {
			//钱
			int res = player_self()->pack_add_money(Money_Add_Info(Money_Type(goods->item_id), num, Pack::MONEY_ADD_ACTIVITY_EXCHANGE_MALL));
			if(res) {
				return res;
			}
			acv_msg.property.push_back(Property(goods->item_id, num));
		}
		MSG_50106901 msg;
		msg.id = id;
		msg.num = num;
		THIS_SEND_TO_CLIENT(msg);
		if(!acv_msg.property.empty() || ! acv_msg.item_info.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		if(goods->limit_exchange) {
			UInt_UInt_Map::iterator it = activities_stores_detail_->id_num_buy.find(goods->id);
			if(it != activities_stores_detail_->id_num_buy.end()) {
				it->second += num;
			} else {
				activities_stores_detail_->id_num_buy.insert(std::make_pair(goods->id,num));
			}
		}
	}
	return 0;
}
