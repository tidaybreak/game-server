/*
 * Mall.cpp
 *
 *  Created on: 2012-6-15
 *      Author: ChenTeng
 */

#include "Logic_Module_Head.h"
#include "pack/Packer.h"
#include "Mall.h"
#include "Mall_Manager.h"
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Msg_Mall_Struct.h"
#include "pack/Pack_Struct.h"
#include "Err_Code.h"
#include "Msg_Active_Struct.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "public/Public_Def.h"
#include "Logic_Public.h"
#include "Config_Cache_Mail.h"
#include "Config_Cache_Item.h"
#include "helper/Helper_Def.h"
#include "Config_Cache_Mall.h"
#include "active/Active_Manager.h"
#include "mall/Mall_Config.h"

Mall::Mall(void)  {
	reset();
}

Mall::~Mall(void) { }
void Mall::reset(void) { }

void Mall::fite_promo_goods_num(std::vector<Goods_Info> &goods_list) {
	for(std::vector<Goods_Info>::iterator it = goods_list.begin(); it != goods_list.end(); ++it) {
		it->per_all = it->per_all?it->per_all:it->per_num;
		int32_t sa_num =  MALL_MANAGER->get_num_by_item(it->id);
		int32_t sp_num = MALL_MANAGER->get_num_by_role_id(it->id, player_self()->role_id());
		if(it->all_num) {
			it->all_num = it->all_num -sa_num;
		}
		if(it->per_num) {
			it->per_num = it->per_num -sp_num;
		}
	}
}

void Mall::fite_tabs_goods_num(std::vector<Goods_Info> &goods_list) {
	for(std::vector<Goods_Info>::iterator it = goods_list.begin(); it != goods_list.end(); ++it) {
		it->per_all = it->per_all?it->per_all:it->per_num;
		int32_t sa_num =  MALL_MANAGER->get_tabs_num_by_item(it->tabs,it->id);
		int32_t sp_num = MALL_MANAGER->get_tabs_num_by_role_id(it->tabs,it->id, player_self()->role_id());
		if(it->all_num) {
			it->all_num = it->all_num -sa_num;
		}
		if(it->per_num) {
			it->per_num = it->per_num -sp_num;
		}
	}
}

int Mall::req_fetch_mall_list() {

	MSG_50151000 msg;
	msg.reset();
	Mall_Promo_Info &promo_goods_ = MALL_MANAGER->get_promo_goods();
	msg.promo_start_time = promo_goods_.start_time;
	msg.promo_end_time = promo_goods_.end_time;
	msg.promo_goods_info = promo_goods_.goods_list;
	fite_promo_goods_num(msg.promo_goods_info);
//	for(std::vector<Goods_Info>::iterator it = promo_goods_.goods_list.begin(); it != promo_goods_.goods_list.end(); ++it) {
//		int32_t sa_num =  MALL_MANAGER->get_num_by_item(it->id);
//		int32_t sp_num = MALL_MANAGER->get_num_by_role_id(it->id, player_self()->role_id());
//		//if(it->all_num == 0 || it->all_num > sa_num) {
//			Goods_Info gi = *it;
//			if(it->all_num) {
//				gi.all_num = it->all_num -sa_num;
//			}
//			if(it->per_num) {
//				gi.per_num = it->per_num -sp_num;
//			}
//			gi.per_all = it->per_all?it->per_all:it->per_num;
//			msg.promo_goods_info.push_back(gi);
//		//}
//	}

	//banner
	Mall_Promo_Info &banner_goods_ = MALL_MANAGER->get_banner_goods();
	msg.banner_start_time = banner_goods_.start_time;
	msg.banner_end_time = banner_goods_.end_time;
	msg.banner_goods_info = banner_goods_.goods_list;

//	if(this->player_self()->get_power_by_power_type(Power_Type(pwoer_mall)) == 0){
		this->player_self()->set_power_switch(Power_Type(pwoer_mall),1);
		//回应客户端
		msg.goods_info = MALL_MANAGER->get_goods_info();
		fite_tabs_goods_num(msg.goods_info);
//	}

	const UInt_Set &activity_id_set = CONFIG_CACHE_MALL->get_activity_id_set();
	for(UInt_Set::const_iterator it = activity_id_set.begin(); it != activity_id_set.end(); ++it) {
		if(ACTIVE_MANAGER->get_active_up_day(*it)) {
			msg.act_ids.push_back(*it);
		}
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}
int Mall::req_buy_goods_in_mall(const int32_t id, int32_t &amount, const int32_t tabs, std::string &role_name, const int32_t buy_type,  uint8_t sub_type, int8_t helper_fast_use) {
	if(amount > 999) {
		return ERROR_BUY_IN_MALL;
	}
	MSG_50151001 msg;
	msg.reset();
	int32_t status= MallDef::SUCCESS;//购买状态 0:成功
	Goods_Info gi;
	int price = 0; //price
	Item_Detail item;//item
	if(tabs == 99) {
		Goods_Info_Vec goods_list_ = MALL_MANAGER->get_promo_goods().goods_list;
		for( size_t i = 0; i < goods_list_.size(); i++){
			if(goods_list_[i].id == id && goods_list_[i].tabs == tabs){
				gi = goods_list_[i];
				break;
			}
		}
	} else {
		Goods_Info_Vec goods_list_ = MALL_MANAGER->get_goods_info();
		for( size_t i = 0; i < goods_list_.size(); i++){
			if(goods_list_[i].id == id && goods_list_[i].tabs == tabs){
				gi = goods_list_[i];
				break;
			}
		}
	}



	//查找商城是否有物品
	if(gi.id == 0 || amount == 0){
		if(sub_type) {
			return -1;
		} else {
			return ERROR_CLIENT_PARAM;
		}
	}

	if(gi.tabs==99) {
		//判断总销量限制
		if(gi.all_num) {
			int32_t num = MALL_MANAGER->get_num_by_item(gi.id);
			if(gi.all_num < num + amount) {
				if(sub_type) {
					if(gi.all_num - num <= 0) {
						return -1;//减少到0;返回-1;内部用;不能返回客户端
					} else {
						amount = gi.all_num - num;
					}
				} else {
					return ERROR_MALL_ALL_NUM;
				}
			}
		}
		//判断个人销量限制
		if(gi.per_num) {
			int32_t num = MALL_MANAGER->get_num_by_role_id(gi.id, player_self()->role_id());
			if(gi.per_num < num + amount) {
				if(sub_type) {
					if(gi.per_num - num <= 0) {
						return -1;//减少到0;返回-1;内部用;不能返回客户端
					} else {
						amount = gi.per_num - num;
					}
				} else {
					return ERROR_SELF_BUY_NUM;
				}
			}
		}
	} else {
		//判断总销量限制
		if(gi.all_num) {
			int32_t num = MALL_MANAGER->get_tabs_num_by_item(gi.tabs,gi.id);
			if(gi.all_num < num + amount) {
				if(sub_type) {
					if(gi.all_num - num <= 0) {
						return -1;//减少到0;返回-1;内部用;不能返回客户端
					} else {
						amount = gi.all_num - num;
					}
				} else {
					return ERROR_MALL_ALL_NUM;
				}
			}
		}
		//判断个人销量限制
		if(gi.per_num) {
			int32_t num = MALL_MANAGER->get_tabs_num_by_role_id(gi.tabs, gi.id, player_self()->role_id());
			if(gi.per_num < num + amount) {
				if(sub_type) {//减少数量继续购买
					if(gi.per_num - num <= 0) {//减少到0;返回-1;内部用;不能返回客户端
						return -1;
					} else {
						amount = gi.per_num - num;
					}
				} else {
					return ERROR_SELF_BUY_NUM;
				}
			}
		}
	}

	if(gi.tabs==99) {
		price = amount*gi.promo_price;
	} else {
		price = amount*gi.price;
	}
	act_redef_price(gi, price);//活动有可能需要重新定义价格
	if(role_name != "") {
		gi.pay_type = 3;
	} else if(buy_type == 0 && gi.pay_type == 2) {
		gi.pay_type=4;
	}
	item.id_=gi.id;
	item.amount_=amount;
	if(gi.pay_type == GOLD_ONLY && role_name == ""){
		item.bind_=Item_Detail::UNBIND;
	} else {
		item.bind_=Item_Detail::BIND;
	}

	//是否有足够的金钱
	int return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(gi.pay_type), price, MONEY_SUB_MALL_USE));
	if(return_status != 0) {
		return return_status;
	}

	if(role_name == ""){
		status = buy_to_self(item,price,gi);
		if(status != 0 && ERROR_PACK_FULL_BUT_MAIL != status) {
			return status;
		}
	}else{
		return 0;//不能赠送
		role_id_t role_id = LOGIC_MONITOR->get_role_id_by_name(role_name);
		status = buy_to_player(item,price,gi,role_id);
		if(status != 0) {
			return status;
		}
	}

	//回应客户端
	msg.status=status;
	msg.helper_fast_use = helper_fast_use;
	msg.id = gi.id;
	THIS_SEND_TO_CLIENT(msg);

	//任务监听商城购买物品
	if(status == MallDef::SUCCESS || status == ERROR_PACK_FULL_BUT_MAIL) {
		std::vector<Item_Detail> items;
		items.push_back(item);
		player_self()->task_listen_buy_goods(items.begin(), items.end());

		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_MALL_BUY_ITEM);//小助手监听
	}
	if((status == MallDef::SUCCESS || status == ERROR_PACK_FULL_BUT_MAIL) && gi.tabs == 99) {
		Chat_Item_Info citem;
		citem.reset();
		item.set_item_basic_info(citem.item_base_info);
		player_self()->create_ann_buy_promo_goods(citem);
		for(int32_t i=0; i<item.amount_; ++i) {
			MALL_MANAGER->update_item_sales_num(item.id_, player_self()->role_id());
		}
	}
	if((status == MallDef::SUCCESS || status == ERROR_PACK_FULL_BUT_MAIL) && gi.tabs != 99 && (gi.all_num || gi.per_num)) {
		for(int32_t i=0; i<item.amount_; ++i) {
			MALL_MANAGER->update_tabs_item_sales_num(gi.tabs, item.id_, player_self()->role_id());
		}
	}
	return status;
}

int Mall::buy_to_self(Item_Detail &item, const int price,Goods_Info &gi){
	//是否能添加物品到背包
//	int return_status = pack_try_insert_item(PACK_T_PACKAGE_INDEX,item);
//	if(return_status != 0) {
//		return return_status;
//	}

	if(pack_sub_money(Money_Sub_Info(Money_Sub_Type(gi.pay_type), price, Money_DM_Info(MONEY_SUB_MALL_USE, item.id_, item.amount_))) == 0) {
		int res = pack_insert_item(PACK_T_PACKAGE_INDEX,item, Gain_Item_DM_Info(ITEM_GAIN_MALL));
		if(res && ERROR_PACK_FULL_BUT_MAIL != res) {
			return res;
		}
		MSG_81000102 msg;
		msg.type = 0;
		Item_Basic_Info item_basic;
		item.set_item_basic_info(item_basic);
		msg.item_info.push_back(item_basic);
		THIS_SEND_TO_CLIENT(msg);
		return res;
	}
	return 0;
}

int Mall::buy_to_player(Item_Detail &item, const int price, Goods_Info &gi, role_id_t role_id ){
	if(player_self()->is_friend(role_id) != 0) {
		return ERROR_FRIEND_NOTEXIST;
	}

	if(pack_sub_money(Money_Sub_Info(Money_Sub_Type(gi.pay_type), price, Money_DM_Info(MONEY_SUB_MALL_USE, item.id_, item.amount_))) == 0) {
		//mail;
		const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(1002);
		std::vector<Item_Basic_Info> item_vector;
		Item_Basic_Info mail_item;
		mail_item.id = item.id_;
		mail_item.amount = item.amount_;
		mail_item.bind = item.bind_;
		item_vector.push_back(mail_item);
		std::string contents;
		/***********************************************/
		const int size_buf = mailc->mail_content.size() + \
				player_self()->role_name().size() + 10 + \
				sizeof(item.amount_) + 10 + \
				CONFIG_CACHE_ITEM->find_item(item.id_)->name.size() + 10;
	    char content_buf[size_buf];
		memset(content_buf, 0, size_buf);
		snprintf(content_buf, size_buf, mailc->mail_content.c_str(), player_self()->role_name().c_str(), item.amount_, CONFIG_CACHE_ITEM->find_item(item.id_)->name.c_str());
		contents.assign(content_buf);
		/************************************************/

		LOGIC_PUBLIC->send_mail(1002, role_id, item_vector, "", contents);
	}

	return 0;
}

int Mall::req_one_key_buy_goods_in_mall(const int32_t tabs, Int_Vec &ids) {
	MSG_50151002 msg;
	msg.reset();
	int32_t status= MallDef::SUCCESS;//购买状态 0:成功
	int price = 0; //price
	std::vector<Item_Detail> items;//items
	Goods_Info_Vec goods_list_ = MALL_MANAGER->get_goods_info();
	for(Int_Vec::iterator it = ids.begin(); it != ids.end(); it++){
		for( Goods_Info_Vec::iterator git = goods_list_.begin(); git != goods_list_.end(); git++){
			if(git->id == (*it) && git->tabs == tabs){
				Item_Detail item;
				item.id_=git->id;
				item.amount_=1;
				item.bind_=Item_Detail::UNBIND;
				items.push_back(item);
				price += git->price;
				break;
			}
		}
	}
	//判断是否所有物品都是存在的
	if(ids.size() != items.size()) {
		return ERROR_CLIENT_PARAM;
	}else if((int)items.size() > pack_get_remain_capacity(Pack_Type(PACK_T_PACKAGE_INDEX))){
		return ERROR_PACK_FULL;
	}
	//判断是否有足够的钱
	int return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, MONEY_SUB_MALL_USE));
	if(return_status != 0) {
		return return_status;
	}


	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	//减掉相应的钱
	if(pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, MONEY_SUB_MALL_USE)) == 0) {
		//往背包放东西
		for(std::vector<Item_Detail>::iterator it = items.begin(); it != items.end(); it++){
			if(pack_insert_item(PACK_T_PACKAGE_INDEX,(*it), Gain_Item_DM_Info(ITEM_GAIN_MALL))) {
				return ERROR_INNER_SERVER;
			} else {
				Item_Basic_Info ibi;
				(*it).set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
			}
		}
	}
	if (!acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	//回应客户端
	msg.status=status;
	THIS_SEND_TO_CLIENT(msg);

	//任务监听商城购买物品
	if(status == MallDef::SUCCESS) {
		player_self()->task_listen_buy_goods(items.begin(), items.end());
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_MALL_BUY_ITEM);//小助手监听
	}
	return 0;
}

int Mall::req_fast_buy_item_in_mall(int32_t item_id, uint32_t amount, uint8_t buy_type) {
	if(amount > 999) {
		return ERROR_CLIENT_PARAM;
	}
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(item_id);
	if(item) {
		if(!item->sys_sell_type || !item->sys_sell) {
			return ERROR_CONFIG_ERROR_TIP_ITEM_CAN_NOT_BUY;
		}
		int return_status = 0;
		//确定购买方式(粉钻,钻石等)
		int pay_type = item->sys_sell_type;
		if(item->sys_sell_type == BIND_GOLD_FIRST && buy_type == 0) {
			pay_type = BIND_GOLD_ONLY;
		}
		//计算价钱
		int price = amount * item->sys_sell;
		//是否有足够的金钱
		return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(pay_type), price, MONEY_SUB_MALL_USE));
		if(return_status != 0) {
			return return_status;
		}

		//构造物品
		Item_Detail item_;
		item_.reset();
		item_.id_ = item_id;
		item_.amount_ = amount;
		item_.bind_ = Item_Detail::BIND;

		//是否能添加物品到背包
		return_status = pack_try_insert_item(PACK_T_PACKAGE_INDEX,item_);
		if(return_status != 0) {
			return return_status;
		}

		//减掉相应的钱
		if(pack_sub_money(Money_Sub_Info(Money_Sub_Type(pay_type), price, Money_DM_Info(MONEY_SUB_MALL_USE, item_.id_, item_.amount_))) == 0) {
			//往背包放东西
			if(pack_insert_item(PACK_T_PACKAGE_INDEX,item_, Gain_Item_DM_Info(ITEM_GAIN_MALL))) {
				return ERROR_INNER_SERVER;
			}
			{
				MSG_81000102 acv_msg;
				acv_msg.type = 0;
				Item_Basic_Info ibi;
				item_.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				THIS_SEND_TO_CLIENT(acv_msg);
			}
		}
		MSG_50151003 msg;
		msg.reset();
		msg.status=return_status;
		THIS_SEND_TO_CLIENT(msg);
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_MALL_BUY_ITEM);//小助手监听
	} else {
		return ERROR_CONFIG_ERROR_TIP_ITEM_CAN_NOT_BUY;
	}
	return 0;
}

int Mall::req_fast_buy_item_in_mall_tabs(int32_t item_id, int32_t amount, uint8_t buy_type, int8_t helper_fast_use) {
	const UInt_Vec &tabs = CONFIG_CACHE_MALL->get_fast_buy_tabs_sort();
	for(UInt_Vec::const_iterator iter = tabs.begin(); iter != tabs.end(); ++iter) {
		std::string str = "";
		int32_t arg_amount  = amount;
		int res = req_buy_goods_in_mall(item_id, arg_amount, *iter, str, buy_type,1);
		if(res == -1) {
			continue;
		}
		if(res) {
			return res;
		}
		if(amount != arg_amount) {
			return ERROR_FAST_BUY_NUM_NOT_HAVE_SUCCESS;
		}
		MSG_50151003 msg;
		msg.reset();
		msg.status=0;
		msg.helper_fast_use = helper_fast_use;
		msg.id = item_id;
		THIS_SEND_TO_CLIENT(msg);
		return 0;
	}
	return ERROR_MALL_ALL_NUM;
}

bool Mall::find_goods_price_and_num_by_tabs_and_id(int32_t tabs, int32_t id, int32_t &price, int32_t &num) {
	Goods_Info_Vec &goods_info = MALL_MANAGER->get_goods_info();
	for(Goods_Info_Vec::iterator it = goods_info.begin(); it != goods_info.end(); ++it) {
		if(it->tabs == tabs && it->id == id) {
			price = it->price;
			int32_t snum = INT_MAX;
			//判断总销量限制
			if(it->all_num) {
				int32_t num_ = MALL_MANAGER->get_tabs_num_by_item(it->tabs,it->id);
				snum = it->all_num - num_;
			}
			//判断个人销量限制
			if(it->per_num) {
				int32_t num_ = MALL_MANAGER->get_tabs_num_by_role_id(it->tabs,it->id, player_self()->role_id());
				snum = ((it->per_num - num_)<snum)?(it->per_num - num_):snum;
			}
			return true;
		}
	}
	return false;
}

void Mall::act_redef_price(const Goods_Info &gi, int &price) {
	const Mall_Activity_Price_Vec *conf = CONFIG_CACHE_MALL->find_activity_price_set_by_tabs_item(gi.tabs, gi.id);
	if(conf) {
		for(Mall_Activity_Price_Vec::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(ACTIVE_MANAGER->get_active_up_day(iter->act_id)) {
				if(iter->gender) {
					if(iter->gender == player_self()->base_detail().gender) {
						price = iter->price;
					}
				} else {
					price = iter->price;
				}
				break;
			}
		}
	}
}
