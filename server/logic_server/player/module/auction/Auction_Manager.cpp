/*
 * Auction_Manager.cpp
 *
 *  Created on: May 12, 2014
 *      Author: xiaoliang
 */

#include "Auction_Manager.h"
#include "Logic_Public.h"
#include "Auction_Def.h"
#include "Err_Code.h"
#include "Base_Struct.h"
#include "Config_Cache_Auction.h"
#include "DB_Manager.h"
#include "DB_Def.h"
#include "DB_Operator.h"
#include "Config_Cache_Mail.h"
#include "Config_Cache_Item.h"
#include "pack/Pack_Def.h"
#include "pack/Pack_Struct.h"

Auction_Manager::Auction_Manager(void) {
	id_uper = 0;
}

Auction_Manager::~Auction_Manager(void) { }

const Aution_Goods_Data_Map &Auction_Manager::get_autction_goods_data_map(void) {
	return aution_goods_data_map_;
}

const Auction_Goods_Data *Auction_Manager::find_auction_goods_data_by_id(uint32_t id) {
	Aution_Goods_Data_Map::iterator it = aution_goods_data_map_.find(id);
	if(it != aution_goods_data_map_.end()) {
		return &it->second;
	}
	return 0;
}

int Auction_Manager::player_todo_under_auction_goods(uint32_t id, role_id_t issue_role_id) {
	Aution_Goods_Data_Map::iterator it = aution_goods_data_map_.find(id);
	if(it == aution_goods_data_map_.end()) {
		return ERROR_CLIENT_PARAM;
	}
	if(it->second.issue_role_id != issue_role_id) {
		return ERROR_CLIENT_PARAM;
	}
	MSG_DEBUG("player_todo_under_auction_goods, issue:%ld, auctioner:%ld", issue_role_id, it->second.auction_role_id);
	under_goods_issue_mail(it->second);
	under_goods_auction_mail(it->second);
	del_auction_goods(id);
	return 0;
}

void Auction_Manager::start(void) {
	MONGO_OPERATOR->load_action_goods_data(aution_goods_data_map_, id_uper);
	if(id_uper < 10000) {
		id_uper = 10000;
	}
	tick_time_update_config_to_auction_goods_list(Time_Value::gettimeofday());
}

void Auction_Manager::tick(const Time_Value &now) {
	time_over_under_auction_goods(now);
	tick_time_update_config_to_auction_goods_list(now);
}

void Auction_Manager::tick_time_update_config_to_auction_goods_list(const Time_Value &now) {
	const Auction_Goods_Config_Map &conf = CONFIG_CACHE_AUCTION->get_sys_auction_goods_map();
	for(Auction_Goods_Config_Map::const_iterator it = conf.begin(); it != conf.end(); ++it) {
		if(it->second.start_time.sec() <= now.sec() && now.sec() > (it->second.start_time.sec()*it->second.continue_time*3600)) {
			Aution_Goods_Data_Map::iterator iter = aution_goods_data_map_.find(it->first);
			if(iter != aution_goods_data_map_.end()) {
				iter->second.update_of_config(it->second);
			} else {
				Auction_Goods_Data goods = Auction_Goods_Data(it->second);
				aution_goods_data_map_.insert(std::make_pair(it->first, goods));
				update_auction_goods_to_db(goods);//更新到数据库
			}
		}
	}
}

void Auction_Manager::time_over_under_auction_goods(const Time_Value &now) {
	UInt_Vec to_del_vec;
	to_del_vec.clear();
	for(Aution_Goods_Data_Map::iterator it = aution_goods_data_map_.begin(); it != aution_goods_data_map_.end(); ++it) {
		if(it->second.over_time.sec() <= now.sec()) {
			//如果是人发布的;
			if(it->second.issue_role_id) {
				//如果有竞价人;给竞价人发物品;给发布人回馈邮件,发钻石
				if(it->second.auction_role_id) {
					MSG_DEBUG('time_over_under_auction_goods, issue:%ld, has auctioner:%ld',it->second.issue_role_id, it->second.auction_role_id);
					success_auction_goods_issue_mail(it->second, AUCTION_SUCCESS_BY_TIMEOVER);
					success_auction_goods_auction_mail(it->second);
				} else {
				//如果没有竞价人; 给发布人发回物品的邮件
					MSG_DEBUG('time_over_under_auction_goods, issue:%ld',it->second.issue_role_id);
					auction_fail_issue_mail(it->second);
				}
			}
			to_del_vec.push_back(it->first);
		}
	}
	for(UInt_Vec::iterator dit = to_del_vec.begin(); dit != to_del_vec.end(); ++dit) {
		del_auction_goods(*dit);
	}
}

const Auction_Goods_Data &Auction_Manager::add_auction_goods(uint32_t item_id, uint32_t num, uint32_t continue_time,
		uint32_t auction_price, uint32_t ones_price, role_id_t issue_role_id) {
	MSG_DEBUG("add_auction_goods , item_id:%d, num:%d, continue_time:%d, auction_price:%d, ones_price:%d, issue:%ld", item_id, num, continue_time, auction_price, ones_price, issue_role_id);
	Auction_Goods_Data goods = Auction_Goods_Data(++id_uper, item_id, num, continue_time, auction_price,
			ones_price,	issue_role_id);
	std::pair<Aution_Goods_Data_Map::iterator, bool> ret = aution_goods_data_map_.insert(std::make_pair(goods.id, goods));
	update_auction_goods_to_db(goods);//更新到数据库
	return (ret.first)->second;
}

int Auction_Manager::player_ones_price_buy(uint32_t id, role_id_t role_id) {
	Aution_Goods_Data_Map::iterator it = aution_goods_data_map_.find(id);
	if(it == aution_goods_data_map_.end()) {
		return ERROR_CLIENT_PARAM;
	}
	MSG_DEBUG("player_ones_price_buy , last actioner:%ld, issue:%ld, oner:%ld", it->second.auction_role_id, it->second.issue_role_id, role_id);
	success_oner_goods_actioner_mail(it->second);//给上次出价人发邮件
	success_auction_goods_issue_mail(it->second, AUCTION_SUCCESS_BY_ONES);//成功拍卖商品(时间到或一口价);卖家(发布人)邮件
	success_auction_goods_oner_mail(role_id, it->second); //成功拍卖商品(一口价类型);一口价方邮件
	del_auction_goods(it->first);
	return 0;
}

int Auction_Manager::update_auction_role_and_price(uint32_t id, role_id_t role_id, uint32_t price) {
	Aution_Goods_Data_Map::iterator it = aution_goods_data_map_.find(id);
	if(it == aution_goods_data_map_.end()) {
		return ERROR_CLIENT_PARAM;
	}
	if(it->second.l_auction_price >= price || it->second.auction_price >= price) {
		return ERROR_CLIENT_PARAM;//竞拍价不正确
	}
	auction_fail_actioner_mail(it->second);//给上次出价人发邮件
	if(it->second.auction_role_id) {
		it->second.auction_fail_role_id.insert(it->second.auction_role_id);//记录上一次竞价人到记录
	}
	it->second.auction_role_id = role_id;
	it->second.l_auction_price = price;
	update_auction_goods_to_db(it->second);//更新到数据库
	return 0;
}

void Auction_Manager::update_auction_goods_to_db(Auction_Goods_Data &goods_data) {
	Block_Buffer buf;
	buf.make_message(MONGO_DATA_AUCTION_GOODS);
	buf.write_uint8(MONGO_DATA_UPDATE);
	goods_data.serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
}

void Auction_Manager::delete_auction_goods_to_db(uint32_t id) {
	Block_Buffer buf;
	buf.make_message(MONGO_DATA_AUCTION_GOODS);
	buf.write_uint8(MONGO_DATA_DELETE);
	buf.write_uint32(id);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
}

void Auction_Manager::del_auction_goods(uint32_t id) {
	Aution_Goods_Data_Map::iterator it = aution_goods_data_map_.find(id);
	if(it != aution_goods_data_map_.end()) {
		aution_goods_data_map_.erase(it);
		delete_auction_goods_to_db(id);
	}
}

void Auction_Manager::under_goods_issue_mail(Auction_Goods_Data &goods_data) {//发送商品人为操作下架,卖家(发布人)邮件
	if(!goods_data.issue_role_id) {
		return;
	}
	std::vector<Item_Basic_Info> item_vector;
	item_vector.clear();
	Item_Basic_Info ibi;
	ibi.reset();
	ibi.id = goods_data.item_id;
	ibi.amount = goods_data.num;
	ibi.bind = Item_Detail::UNBIND;
	item_vector.push_back(ibi);

	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(AUCTION_SEND_UNDER_GOODS_ISSUER_MAIL);
	if(!mailc) {
		MSG_DEBUG("under_goods_issue_mail mail_id Does not exist, mail_id:%d",AUCTION_SEND_UNDER_GOODS_ISSUER_MAIL);
		return;
	}
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(goods_data.item_id);
	if(!item) {
		MSG_DEBUG("under_goods_issue_mail item Does not exist, item_id:%d",goods_data.item_id);
		return;
	}
	std::string contents;
	const int size_buf = 1024;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), item->name.c_str(), goods_data.num);
	contents.assign(content_buf);
	/************************************************/

	LOGIC_PUBLIC->send_mail(AUCTION_SEND_UNDER_GOODS_ISSUER_MAIL, goods_data.issue_role_id, item_vector, "", contents);
}

void Auction_Manager::under_goods_auction_mail(Auction_Goods_Data &goods_data) {//发送商品人为操作下架,竞拍方邮件
	if(!goods_data.auction_role_id) {
		return;
	}
	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(AUCTION_SEND_UNDER_GOODS_AUCTION_MAIL);
	if(!mailc) {
		MSG_DEBUG("under_goods_auction_mail mail_id Does not exist, mail_id:%d",AUCTION_SEND_UNDER_GOODS_AUCTION_MAIL);
		return; }
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(goods_data.item_id);
	if(!item) { return;}
	std::string contents;
	const int size_buf = 1024;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), item->name.c_str(), goods_data.l_auction_price);
	contents.assign(content_buf);
	/************************************************/
	LOGIC_PUBLIC->send_mail(AUCTION_SEND_UNDER_GOODS_AUCTION_MAIL, goods_data.auction_role_id, goods_data.l_auction_price, "", contents);
}

void Auction_Manager::success_auction_goods_issue_mail(Auction_Goods_Data &goods_data, uint8_t type) {//成功拍卖商品(时间到或一口价);卖家(发布人)邮件
	if(!goods_data.issue_role_id) {
		return;
	}
	int32_t gold = 0;
	uint32_t price = 0;
	switch(type) {
		case AUCTION_SUCCESS_BY_TIMEOVER: {
			price = goods_data.l_auction_price;
			gold = goods_data.l_auction_price - (goods_data.l_auction_price * AUCTION_COMMON_FREE_RATE)/100;
			break;
		}
		case AUCTION_SUCCESS_BY_ONES: {
			price = goods_data.ones_price;
			gold = goods_data.ones_price - (goods_data.ones_price * AUCTION_COMMON_FREE_RATE)/100;
			break;
		}
	}
	if(gold < 0) {
		gold = 0;
	}
	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(AUCTION_SEND_SUCCESS_AUCTION_GOODS_ISSUE_MAIL);
	if(!mailc) {
		MSG_DEBUG("success_auction_goods_issue_mail mail_id Does not exist, mail_id:%d",AUCTION_SEND_SUCCESS_AUCTION_GOODS_ISSUE_MAIL);
		return; }
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(goods_data.item_id);
	if(!item) { return;}
	std::string contents;
	const int size_buf = 1024;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), item->name.c_str(), gold, price);
	contents.assign(content_buf);
	/************************************************/
	LOGIC_PUBLIC->send_mail(AUCTION_SEND_SUCCESS_AUCTION_GOODS_ISSUE_MAIL, goods_data.issue_role_id, gold, "", contents);
}
void Auction_Manager::success_auction_goods_auction_mail(Auction_Goods_Data &goods_data) {//成功拍卖商品(时间到类型);竞拍方邮件
	if(!goods_data.auction_role_id) {
		return;
	}
	std::vector<Item_Basic_Info> item_vector;
	item_vector.clear();
	Item_Basic_Info ibi;
	ibi.reset();
	ibi.id = goods_data.item_id;
	ibi.amount = goods_data.num;
	ibi.bind = Item_Detail::UNBIND;
	item_vector.push_back(ibi);
	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(AUCTION_SEND_SUCCESS_AUCTION_GOODS_AUCTION_MAIL);
	if(!mailc) {
		MSG_DEBUG("success_auction_goods_auction_mail mail_id Does not exist, mail_id:%d",AUCTION_SEND_SUCCESS_AUCTION_GOODS_AUCTION_MAIL);
		return; }
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(goods_data.item_id);
	if(!item) { return;}
	std::string contents;
	const int size_buf = 1024;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), item->name.c_str(), goods_data.l_auction_price);
	contents.assign(content_buf);
	/************************************************/
	LOGIC_PUBLIC->send_mail(AUCTION_SEND_SUCCESS_AUCTION_GOODS_AUCTION_MAIL, goods_data.auction_role_id, item_vector, "", contents);
}

void Auction_Manager::success_auction_goods_oner_mail(role_id_t role_id, Auction_Goods_Data &goods_data) { //成功拍卖商品(一口价类型);一口价方邮件
	if(!role_id) {
		return;
	}
	std::vector<Item_Basic_Info> item_vector;
	item_vector.clear();
	Item_Basic_Info ibi;
	ibi.reset();
	ibi.id = goods_data.item_id;
	ibi.amount = goods_data.num;
	ibi.bind = Item_Detail::UNBIND;
	item_vector.push_back(ibi);
	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(AUCTION_SEND_SUCCESS_AUCTION_GOODS_ONER_MAIL);
	if(!mailc) {
		MSG_DEBUG("success_auction_goods_oner_mail mail_id Does not exist, mail_id:%d",AUCTION_SEND_SUCCESS_AUCTION_GOODS_ONER_MAIL);
		return; }
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(goods_data.item_id);
	if(!item) { return;}
	std::string contents;
	const int size_buf = 1024;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), item->name.c_str(), goods_data.ones_price);
	contents.assign(content_buf);
	/************************************************/
	LOGIC_PUBLIC->send_mail(AUCTION_SEND_SUCCESS_AUCTION_GOODS_ONER_MAIL, role_id, item_vector, "", contents);
}

void Auction_Manager::success_oner_goods_actioner_mail(Auction_Goods_Data &goods_data) { //成功拍卖商品(一口价类型);最后竞拍方邮件
	if(!goods_data.auction_role_id) {
		return;
	}
	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(AUCTION_SEND_SUCCESS_ONER_GOODS_ACTIONER_MAIL);
	if(!mailc) {
		MSG_DEBUG("success_auction_goods_oner_mail mail_id Does not exist, mail_id:%d",AUCTION_SEND_SUCCESS_ONER_GOODS_ACTIONER_MAIL);
		return; }
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(goods_data.item_id);
	if(!item) { return;}
	std::string contents;
	const int size_buf = 1024;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), item->name.c_str(), goods_data.l_auction_price);
	contents.assign(content_buf);
	/************************************************/
	LOGIC_PUBLIC->send_mail(AUCTION_SEND_SUCCESS_ONER_GOODS_ACTIONER_MAIL, goods_data.auction_role_id, goods_data.l_auction_price, "", contents);
}

void Auction_Manager::auction_fail_actioner_mail(Auction_Goods_Data &goods_data) {//竞拍失败(竞拍方)邮件; 有人比自己出价高的时候就发
	if(!goods_data.auction_role_id) {
		return;
	}
	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(AUCTION_SEND_AUCTION_FAIL_ACTIONER_MAIL);
	if(!mailc) {
		MSG_DEBUG("success_auction_goods_oner_mail mail_id Does not exist, mail_id:%d",AUCTION_SEND_AUCTION_FAIL_ACTIONER_MAIL);
		return; }
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(goods_data.item_id);
	if(!item) { return;}
	std::string contents;
	const int size_buf = 1024;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), item->name.c_str(), goods_data.l_auction_price);
	contents.assign(content_buf);
	/************************************************/
	LOGIC_PUBLIC->send_mail(AUCTION_SEND_AUCTION_FAIL_ACTIONER_MAIL, goods_data.auction_role_id, goods_data.l_auction_price, "", contents);
}

void Auction_Manager::auction_fail_issue_mail(Auction_Goods_Data &goods_data) {//竞拍失败(发布人)邮件(时间到类型); 无人竞拍
	if(!goods_data.issue_role_id) {
		return;
	}
	std::vector<Item_Basic_Info> item_vector;
	item_vector.clear();
	Item_Basic_Info ibi;
	ibi.reset();
	ibi.id = goods_data.item_id;
	ibi.amount = goods_data.num;
	ibi.bind = Item_Detail::UNBIND;
	item_vector.push_back(ibi);

	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(AUCTION_SEND_AUCTION_FAIL_ISSUE_MAIL);
	if(!mailc) {
		MSG_DEBUG("success_auction_goods_oner_mail mail_id Does not exist, mail_id:%d",AUCTION_SEND_AUCTION_FAIL_ISSUE_MAIL);
		return; }
	const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(goods_data.item_id);
	if(!item) { return;}
	std::string contents;
	const int size_buf = 1024;
    char content_buf[size_buf];
	memset(content_buf, 0, size_buf);
	snprintf(content_buf, size_buf, mailc->mail_content.c_str(), item->name.c_str());
	contents.assign(content_buf);
	/************************************************/
	LOGIC_PUBLIC->send_mail(AUCTION_SEND_AUCTION_FAIL_ISSUE_MAIL, goods_data.issue_role_id, item_vector, "", contents);
}
