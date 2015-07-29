/*
 * Auction.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: xiaoliang
 */

#include "Auction.h"
#include "Auction_Struct.h"
#include "Logic_Module_Head.h"
#include "Msg_Auction_Struct.h"
#include "Auction_Manager.h"
#include "Logic_Player.h"
#include "Config_Cache_Auction.h"
#include "Err_Code.h"
#include "Auction_Def.h"

Auction::Auction(void) {
	reset();
}

Auction::~Auction() {
	// TODO Auto-generated destructor stub
}

void Auction::reset(void) {
	auction_detail_ = 0;
}

int Auction::create_init(Auction_Detail &detail) {

	return 0;
}

void Auction::load_detail(Auction_Detail &detail) {
	auction_detail_ = &detail;
}

void Auction::save_detail(Auction_Detail &detail) {
	if (auction_detail().is_change) {
		detail = auction_detail();
		auction_detail_->is_change = false;
	}
}

int Auction::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int Auction::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Auction::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Auction::move_scene_completed(void) {

	return 0;
}

int Auction::client_ready_now(void) {

	return 0;
}

void Auction::module_init(void) {

}

void Auction::sign_in(void) {

}

void Auction::sign_out(void) {

}

void Auction::trusteeship_sign_out(void) {

}

int Auction::sign_in_repeat(void) {

	return 0;
}

int Auction::req_auction_goods_list(void) {
	MSG_50107000 msg;
	msg.reset();
	const Aution_Goods_Data_Map &auction_goods = AUCTION_MANAGER->get_autction_goods_data_map();
	Auction_Goods_Info agi;
	for(Aution_Goods_Data_Map::const_iterator iter = auction_goods.begin(); iter!= auction_goods.end(); ++iter) {
		agi.reset();
		iter->second.make_auction_goods_info(agi);
		msg.list.push_back(agi);
		if(iter->second.issue_role_id == player_self()->role_id()) {
			++msg.num;
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Auction::req_add_auction_goods(uint32_t item_id, uint32_t num, uint32_t continue_time,
		uint32_t auction_price, uint32_t ones_price, uint8_t open_ann) {
	//根据时长读取配置
	const Auction_Costs_Config *conf = CONFIG_CACHE_AUCTION->get_auction_costs_by_continue_time(continue_time);
	if(!conf){
		return ERROR_CLIENT_PARAM;
	}
	uint32_t limit = CONFIG_CACHE_AUCTION->get_comm_config_val(AUCTION_NUM_LIMIT);
	if(limit && get_my_goods_num() >= limit) {
		return ERROR_AUCTION_NUM_LIMIT;
	}
	//计算需要花的钱
	uint32_t need_money = conf->keep_costs;
	if(open_ann){
		need_money += conf->ann_costs;
	}
	{//尝试扣除物品
		int res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX,Id_Amount(item_id, num, UNBIND_ONLY));
		if(res) { return res; }
	}
	{//尝试扣钱
		int res = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), need_money, Pack::MONEY_SUB_MONEY_ADD_AUCTION_GOODS));
		if(res) { return res; }
	}
	{//扣钱
		int res = player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), need_money, Pack::MONEY_SUB_MONEY_ADD_AUCTION_GOODS));
		if(res) { return res; }
	}
	{//扣物品
		// 检查是否装备，重设卡片数据
		int res = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,Id_Amount(item_id, num, UNBIND_ONLY), Lose_Item_DM_Info(Pack::ITEM_LOSE_ADD_AUCTION_GOODS));
		if(res) { return res; }
	}
	//发送广播广播
	if(open_ann && conf->ann_id){
		Chat_Item_Info citem;
		citem.reset();
		citem.item_base_info.id = item_id;
		citem.item_base_info.amount = num;
		citem.item_base_info.bind = Item_Detail::UNBIND;
		player_self()->create_ann_add_auction_goods(conf->ann_id, citem, auction_price);
	}
	const Auction_Goods_Data &goods = AUCTION_MANAGER->add_auction_goods(item_id, num, continue_time, auction_price, ones_price, player_self()->role_id());
	MSG_50107001 msg;
	msg.reset();
	goods.make_auction_goods_info(msg.goods);
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Auction::req_under_auction_goods(uint32_t id) {
	const Auction_Goods_Data *goods = AUCTION_MANAGER->find_auction_goods_data_by_id(id);
	if(!goods) {
		return ERROR_AUCTION_GOODS_NOT_EXIST;
	}
	if(goods->issue_role_id != player_self()->role_id()) {
		return ERROR_CLIENT_PARAM;
	}
	int res = AUCTION_MANAGER->player_todo_under_auction_goods(id, player_self()->role_id());
	if(res){
		return res;
	}
	MSG_50107002 msg;
	msg.reset();
	msg.id = id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Auction::req_ones_price_buy_auction_goods(uint32_t id) {
	const Auction_Goods_Data *goods = AUCTION_MANAGER->find_auction_goods_data_by_id(id);
	if(!goods){
		return ERROR_AUCTION_GOODS_NOT_EXIST;
	}
	uint32_t item_id =  goods->item_id;
//	if(goods->issue_role_id == player_self()->role_id()) {
//		return ERROR_CLIENT_PARAM;
//	}
	{//扣钱
		int res = player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), goods->ones_price, Pack::MONEY_SUB_ONES_BUY_AUCTION_GOODS));
		if(res) { return res; }
	}
	int res = AUCTION_MANAGER->player_ones_price_buy(id, player_self()->role_id());
	if(res){
		return res;
	}
	MSG_50107003 msg;
	msg.reset();
	msg.id = id;
	msg.item_id = item_id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Auction::req_update_auction_goods(uint32_t id, uint32_t price) {
	const Auction_Goods_Data *goods = AUCTION_MANAGER->find_auction_goods_data_by_id(id);
	if(!goods){
		return ERROR_AUCTION_GOODS_NOT_EXIST;
	}
//	if(goods->issue_role_id == player_self()->role_id()) {
//		return ERROR_CLIENT_PARAM;
//	}
	if(goods->auction_role_id == player_self()->role_id()) {
		return ERROR_CLIENT_PARAM;//你已经是最高出价
	}
	if(goods->l_auction_price >= price || goods->auction_price >= price) {
		return ERROR_AUCTION_GOODS_HAVE_NEW_PRICE;//已经有人比你出价高
	}
	{//扣钱
		int res = player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, Pack::MONEY_SUB_UPDATE_AUCTION_GOODS));
		if(res) { return res; }
	}
	uint32_t item_id = goods->item_id;
	int res = AUCTION_MANAGER->update_auction_role_and_price(id, player_self()->role_id(), price);
	if(res){
		return res;
	}
	MSG_50107004 msg;
	msg.reset();
	msg.id = id;
	msg.price = price;
	msg.item_id = item_id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

uint32_t Auction::get_my_goods_num() {
	uint32_t num = 0;
	const Aution_Goods_Data_Map &auction_goods = AUCTION_MANAGER->get_autction_goods_data_map();
	for(Aution_Goods_Data_Map::const_iterator iter = auction_goods.begin(); iter!= auction_goods.end(); ++iter) {
		if(iter->second.issue_role_id == player_self()->role_id()) {
			++num;
		}
	}
	return num;
}
