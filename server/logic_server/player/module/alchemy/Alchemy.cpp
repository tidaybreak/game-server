/*
 * Alchemy.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: XIAOLIANG
 */
#include "Logic_Module_Head.h"
#include "pack/Packer.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Alchemy.h"
#include "Alchemy_Struct.h"
#include "Msg_Alchemy_Struct.h"
#include "Config_Cache_Alchemy.h"
#include "Err_Code.h"
#include "helper/Helper_Def.h"
#include "Alchemy_Def.h"
#include "Config_Cache.h"
#include "oper_activity/Oper_Activity_Def.h"

Alchemy::Alchemy(void) {
	reset();
}

Alchemy::~Alchemy() {
	// TODO Auto-generated destructor stub
}

void Alchemy::reset(void) {
	alchemy_detail_ = 0;
}



void Alchemy::load_detail(Alchemy_Detail &detail) {
	alchemy_detail_ = &detail;
}

void Alchemy::save_detail(Alchemy_Detail &detail) {
	if (alchemy_detail().is_change) {
		detail = alchemy_detail();
		alchemy_detail_->is_change = false;
	}
}

void Alchemy::module_init(void) {
	this->refresh_cards();

}

int Alchemy::trigger_daily_zero(const Time_Value &now) {
//	alchemy_detail_->used_free_bout = 0;
//	alchemy_detail_->used_pay_bout = 0;
//	alchemy_detail_->six_critical_num = 0;
//	alchemy_detail_->detail_change();
	return 0;
}

void Alchemy::trigger_daily_six(const Time_Value &now) {
	alchemy_detail_->used_free_bout = 0;
	alchemy_detail_->used_pay_bout = 0;
	alchemy_detail_->six_critical_num = 0;

	alchemy_detail_->used_souls_free_bout = 0;
	alchemy_detail_->used_souls_pay_bout = 0;
	alchemy_detail_->six_critical_souls_num = 0;
	alchemy_detail_->detail_change();
}

void Alchemy::refresh_cards(void) {
	alchemy_detail_->current_hero_critical.reset();
	for(Cards_Info_Map::const_iterator iter = CONFIG_CACHE_ALCHEMY->hero_critical_config().begin(); iter != CONFIG_CACHE_ALCHEMY->hero_critical_config().end(); iter++) {
//		Cards_info c = iter->second;
		if(player_self()->has_hero_id(iter->second.heroID) && alchemy_detail_->current_hero_critical.index < iter->second.index) {
			alchemy_detail_->current_hero_critical = iter->second;
		}
	}
	alchemy_detail_->current_hero_gold.reset();
	for(Cards_Info_Map::const_iterator iter = CONFIG_CACHE_ALCHEMY->hero_gold_config().begin(); iter != CONFIG_CACHE_ALCHEMY->hero_gold_config().end(); iter++) {
		if(player_self()->has_hero_id(iter->second.heroID) && alchemy_detail_->current_hero_gold.index < iter->second.index) {
			alchemy_detail_->current_hero_gold = iter->second;
		}
	}
	alchemy_detail_->current_souls_hero_critical.reset();
	for(Cards_Info_Map::const_iterator iter = CONFIG_CACHE_ALCHEMY->hero_souls_critical_config().begin(); iter != CONFIG_CACHE_ALCHEMY->hero_souls_critical_config().end(); iter++) {
		if(player_self()->has_hero_id(iter->second.heroID) && alchemy_detail_->current_souls_hero_critical.index < iter->second.index) {
			alchemy_detail_->current_souls_hero_critical = iter->second;
		}
	}
	alchemy_detail_->current_souls_hero_gold.reset();
	for(Cards_Info_Map::const_iterator iter = CONFIG_CACHE_ALCHEMY->hero_souls_gold_config().begin(); iter != CONFIG_CACHE_ALCHEMY->hero_souls_gold_config().end(); iter++) {
		if(player_self()->has_hero_id(iter->second.heroID) && alchemy_detail_->current_souls_hero_gold.index < iter->second.index) {
			alchemy_detail_->current_souls_hero_gold = iter->second;
		}
	}
	alchemy_detail_->detail_change();
}

int Alchemy::req_alchemy_info(uint8_t mtype) {
	MSG_50152000 msg;
	msg.reset();
	if(mtype) {
		msg.current_hero_critical = alchemy_detail_->current_souls_hero_critical.heroID;
		msg.current_hero_gold = alchemy_detail_->current_souls_hero_gold.heroID;
		msg.over_free_bout = get_over_alchemy_souls_free_about();
		msg.over_pay_bout = get_over_alchemy_souls_pay_about();
		msg.next_pay_diamond = CONFIG_CACHE_ALCHEMY->get_pay_souls_num_by_count(alchemy_detail_->used_souls_pay_bout + 1);
		msg.init_gold = CONFIG_CACHE_ALCHEMY->get_initial_souls_gold_by_level(player_self()->level());
		msg.last_tabs = alchemy_detail_->last_souls_tabs;
	} else {
		msg.current_hero_critical = alchemy_detail_->current_hero_critical.heroID;
		msg.current_hero_gold = alchemy_detail_->current_hero_gold.heroID;
		msg.over_free_bout = get_over_alchemy_free_about();
		msg.over_pay_bout = get_over_alchemy_pay_about();
		msg.next_pay_diamond = CONFIG_CACHE_ALCHEMY->get_pay_num_by_count(alchemy_detail_->used_pay_bout + 1);
		msg.init_gold = CONFIG_CACHE_ALCHEMY->get_initial_gold_by_level(player_self()->level());
		msg.last_tabs = alchemy_detail_->last_tabs;
	}
	msg.mtype = mtype;
    THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Alchemy::get_alchemy_chances_free(void) {
	return CONFIG_CACHE_ALCHEMY->get_chances_free_by_vip_level(player_self()->vip())
			+ player_self()->achievement_alchemy_times();
}

int Alchemy::get_over_alchemy_free_about(void) {
	int num = get_alchemy_chances_free() - alchemy_detail_->used_free_bout;
	if(num > 0) {
		return num;
	}
	return 0;
}

int  Alchemy::get_over_alchemy_pay_about(void) {
	int num = CONFIG_CACHE_ALCHEMY->get_chances_pay_by_vip_level(player_self()->vip()) - alchemy_detail_->used_pay_bout;
	if(num > 0) {
		return num;
	}
	return 0;
}

int Alchemy::get_alchemy_souls_chances_free(void) {
	return CONFIG_CACHE_ALCHEMY->get_chances_souls_free_by_vip_level(player_self()->vip())
			+ player_self()->achievement_alchemy_times();
}

int Alchemy::get_over_alchemy_souls_free_about(void) {
	int num = get_alchemy_souls_chances_free() - alchemy_detail_->used_souls_free_bout;
	if(num > 0) {
		return num;
	}
	return 0;
}

int  Alchemy::get_over_alchemy_souls_pay_about(void) {
	int num = CONFIG_CACHE_ALCHEMY->get_chances_souls_pay_by_vip_level(player_self()->vip()) - alchemy_detail_->used_souls_pay_bout;
	if(num > 0) {
		return num;
	}
	return 0;
}

int Alchemy::req_alchemy(uint8_t tabs, uint8_t pay_type) {
	Cards_info *card;
	switch(tabs) {
		case 0: {
			if(alchemy_detail_->current_hero_gold.heroID == 0) {
				//是否已经招募
				return ERROR_CLIENT_PARAM;
			} else {
				card = &alchemy_detail_->current_hero_gold;
			}
		break;
		}
		case 1: {
			if(alchemy_detail_->current_hero_critical.heroID == 0) {
				//是否已经招募
				return ERROR_CLIENT_PARAM;
			} else {
				card = &alchemy_detail_->current_hero_critical;
			}
		break;
		}
		default:
			return ERROR_CLIENT_PARAM;
	}

	bool need_pay = true;
	//判断次数
	if(get_alchemy_chances_free() - alchemy_detail_->used_free_bout <= 0 ) {
		need_pay = true;
	} else {
		need_pay = false;
	}
	if(need_pay && CONFIG_CACHE_ALCHEMY->get_chances_pay_by_vip_level(player_self()->vip())
			<= alchemy_detail_->used_pay_bout) {
		//次数不够
		return ERROR_ALCHEMY_NUM;
	}
	//判断是否免费
	if(need_pay){
		//收费,判断是否足够的钻石
		int pay_amount = CONFIG_CACHE_ALCHEMY->get_pay_num_by_count(alchemy_detail_->used_pay_bout + 1);
		int pay_type_c = CONFIG_CACHE_ALCHEMY->get_pay_type_by_count(alchemy_detail_->used_pay_bout + 1);
		if(pay_type_c == BIND_GOLD_FIRST) {
			pay_type_c = (pay_type==1)?BIND_GOLD_FIRST:BIND_GOLD_ONLY;
		}
		int return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(pay_type_c), pay_amount, MONEY_SUB_ALCHEMY));
//		if(CONFIG_CACHE->server_maintainer_cache().game_ver_type == 0) {	//判断国内
//			return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), pay_amount, MONEY_SUB_ALCHEMY));
//		} else {//国外
//			return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type((pay_type==1)?BIND_GOLD_FIRST:BIND_GOLD_ONLY), pay_amount, MONEY_SUB_ALCHEMY));
//		}
		if(return_status != 0) {
			return return_status;
		}
	}

	//算出倍率
	int critical = 0;
	Int_Vec weight;
	weight.clear();
	weight.push_back(card->critical_1);
	weight.push_back(card->critical_3);
	weight.push_back(card->critical_6);
	int index = Utility_Func::lottery_one(weight);
	switch(index) {
		case 0: {
			critical = 1; //1倍
			break;
		}
		case 1: {
			critical = 3;//3倍
			break;
		}
		case 2: {
			critical = 6;//6倍
			++alchemy_detail_->six_critical_num;
			break;
		}
		default :
			return ERROR_INNER_SERVER;
	}

	//限制6倍每天最多次数
	if(critical == 6 && alchemy_detail_->six_critical_num > SIX_CRITICAL_MAX) {
		critical = 3;
	}

	//计算获得的钱
	int init_gold = CONFIG_CACHE_ALCHEMY->get_initial_gold_by_level(player_self()->level());
	int get_gold = (init_gold + init_gold*card->gold_added/100) * critical;
	//实际操作,扣钻石或加钱
	if(need_pay) {
		//收费,扣钻石
		int pay_amount = CONFIG_CACHE_ALCHEMY->get_pay_num_by_count(alchemy_detail_->used_pay_bout + 1);
		int pay_type_c = CONFIG_CACHE_ALCHEMY->get_pay_type_by_count(alchemy_detail_->used_pay_bout + 1);
		if(pay_type_c == BIND_GOLD_FIRST) {
			pay_type_c = (pay_type==1)?BIND_GOLD_FIRST:BIND_GOLD_ONLY;
		}
		int return_status =  pack_sub_money(Money_Sub_Info(Money_Sub_Type(pay_type_c), pay_amount, MONEY_SUB_ALCHEMY));;
//		if(CONFIG_CACHE->server_maintainer_cache().game_ver_type == 0) {	//判断国内
//			return_status = pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), pay_amount, MONEY_SUB_ALCHEMY));
//		} else {//国外
//			return_status = pack_sub_money(Money_Sub_Info(Money_Sub_Type((pay_type==1)?BIND_GOLD_FIRST:BIND_GOLD_ONLY), pay_amount, MONEY_SUB_ALCHEMY));
//		}
		if(return_status != 0) {
			return return_status;
		}
	}
	int return_status = pack_add_money(Money_Add_Info(Money_Type(COPPER), get_gold, MONEY_ADD_ALCHEMY));
	if(return_status != 0) {
		return return_status;
	}
	if(need_pay) {
		alchemy_detail_->used_pay_bout++;
	} else {
		alchemy_detail_->used_free_bout++;
	}
	alchemy_detail_->last_tabs = tabs;
	alchemy_detail_->detail_change();
	//返回信息到客户端
	MSG_50152001 msg;
	msg.reset();
	msg.over_free_bout = get_over_alchemy_free_about();
	msg.over_pay_bout = CONFIG_CACHE_ALCHEMY->get_chances_pay_by_vip_level(player_self()->vip()) - alchemy_detail_->used_pay_bout;
	msg.next_pay_diamond = CONFIG_CACHE_ALCHEMY->get_pay_num_by_count(alchemy_detail_->used_pay_bout + 1);
	msg.critical = critical;
	msg.get_gold = get_gold;
	THIS_SEND_TO_CLIENT(msg);

	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_ALCHEMY); //TOD助手日常监听
	player_self()->task_listen_alchemy_num();  //炼金调用这个接口
	player_self()->active_content_listen_alchemy_num();
	player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_ALCHEMY_NUM);
	return 0;
}


int Alchemy::req_souls_alchemy(uint8_t tabs, uint8_t pay_type) {
	Cards_info *card;
	switch(tabs) {
		case 0: {
			if(alchemy_detail_->current_souls_hero_gold.heroID == 0) {
				//是否已经招募
				return ERROR_CLIENT_PARAM;
			} else {
				card = &alchemy_detail_->current_souls_hero_gold;
			}
		break;
		}
		case 1: {
			if(alchemy_detail_->current_souls_hero_critical.heroID == 0) {
				//是否已经招募
				return ERROR_CLIENT_PARAM;
			} else {
				card = &alchemy_detail_->current_souls_hero_critical;
			}
		break;
		}
		default:
			return ERROR_CLIENT_PARAM;
	}

	bool need_pay = true;
	//判断次数
	if(get_alchemy_souls_chances_free() - alchemy_detail_->used_souls_free_bout <= 0 ) {
		need_pay = true;
	} else {
		need_pay = false;
	}
	if(need_pay && CONFIG_CACHE_ALCHEMY->get_chances_souls_pay_by_vip_level(player_self()->vip())
			<= alchemy_detail_->used_souls_pay_bout) {
		//次数不够
		return ERROR_ALCHEMY_NUM;
	}
	//判断是否免费
	if(need_pay){
		//收费,判断是否足够的钻石
		int pay_amount = CONFIG_CACHE_ALCHEMY->get_pay_souls_num_by_count(alchemy_detail_->used_souls_pay_bout + 1);
		int pay_type_c = CONFIG_CACHE_ALCHEMY->get_pay_souls_type_by_count(alchemy_detail_->used_souls_pay_bout + 1);
		if(pay_type_c == BIND_GOLD_FIRST) {
			pay_type_c = (pay_type==1)?BIND_GOLD_FIRST:BIND_GOLD_ONLY;
		}
		int return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(pay_type_c), pay_amount, MONEY_SUB_ALCHEMY));
//		if(CONFIG_CACHE->server_maintainer_cache().game_ver_type == 0) {	//判断国内
//			return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), pay_amount, MONEY_SUB_ALCHEMY));
//		} else {//国外
//			return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type((pay_type==1)?BIND_GOLD_FIRST:BIND_GOLD_ONLY), pay_amount, MONEY_SUB_ALCHEMY));
//		}
		if(return_status != 0) {
			return return_status;
		}
	}

	//算出倍率
	int critical = 0;
	Int_Vec weight;
	weight.clear();
	weight.push_back(card->critical_1);
	weight.push_back(card->critical_3);
	weight.push_back(card->critical_6);
	int index = Utility_Func::lottery_one(weight);
	switch(index) {
		case 0: {
			critical = 1; //1倍
			break;
		}
		case 1: {
			critical = 3;//3倍
			break;
		}
		case 2: {
			critical = 6;//6倍
			++alchemy_detail_->six_critical_souls_num;
			break;
		}
		default :
			return ERROR_INNER_SERVER;
	}

	//限制6倍每天最多次数
	if(critical == 6 && alchemy_detail_->six_critical_souls_num > SIX_CRITICAL_MAX) {
		critical = 3;
	}

	//计算获得的钱
	int init_gold = CONFIG_CACHE_ALCHEMY->get_initial_souls_gold_by_level(player_self()->level());
	int get_gold = (init_gold + init_gold*card->gold_added/100) * critical;
	//实际操作,扣钻石或加钱
	if(need_pay) {
		//收费,扣钻石
		int pay_amount = CONFIG_CACHE_ALCHEMY->get_pay_souls_num_by_count(alchemy_detail_->used_souls_pay_bout + 1);
		int pay_type_c = CONFIG_CACHE_ALCHEMY->get_pay_souls_type_by_count(alchemy_detail_->used_souls_pay_bout + 1);
		if(pay_type_c == BIND_GOLD_FIRST) {
			pay_type_c = (pay_type==1)?BIND_GOLD_FIRST:BIND_GOLD_ONLY;
		}
		int return_status =  pack_sub_money(Money_Sub_Info(Money_Sub_Type(pay_type_c), pay_amount, MONEY_SUB_ALCHEMY));;
//		if(CONFIG_CACHE->server_maintainer_cache().game_ver_type == 0) {	//判断国内
//			return_status = pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), pay_amount, MONEY_SUB_ALCHEMY));
//		} else {//国外
//			return_status = pack_sub_money(Money_Sub_Info(Money_Sub_Type((pay_type==1)?BIND_GOLD_FIRST:BIND_GOLD_ONLY), pay_amount, MONEY_SUB_ALCHEMY));
//		}
		if(return_status != 0) {
			return return_status;
		}
	}
	int return_status = pack_add_money(Money_Add_Info(Money_Type(SOULS), get_gold, MONEY_ADD_ALCHEMY));
	if(return_status != 0) {
		return return_status;
	}
	if(need_pay) {
		alchemy_detail_->used_souls_pay_bout++;
	} else {
		alchemy_detail_->used_souls_free_bout++;
	}
	alchemy_detail_->last_souls_tabs = tabs;
	alchemy_detail_->detail_change();
	//返回信息到客户端
	MSG_50152001 msg;
	msg.reset();
	msg.over_free_bout = get_over_alchemy_souls_free_about();
	msg.over_pay_bout = CONFIG_CACHE_ALCHEMY->get_chances_souls_pay_by_vip_level(player_self()->vip()) - alchemy_detail_->used_souls_pay_bout;
	msg.next_pay_diamond = CONFIG_CACHE_ALCHEMY->get_pay_souls_num_by_count(alchemy_detail_->used_souls_pay_bout + 1);
	msg.critical = critical;
	msg.get_gold = get_gold;
	msg.mtype = 1;
	THIS_SEND_TO_CLIENT(msg);

//	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_ALCHEMY); //TOD助手日常监听
//	player_self()->task_listen_alchemy_num();  //炼金调用这个接口
	player_self()->active_content_listen_alchemy_num();
	return 0;
}



