/*
 * Auction_Manager.h
 *
 *  Created on: May 12, 2014
 *      Author: xiaoliang
 */

#ifndef AUCTION_MANAGER_H_
#define AUCTION_MANAGER_H_

#include "Game_Typedef.h"
#include "auction/Auction_Struct.h"

typedef boost::unordered_map<uint32_t, Auction_Goods_Data> Aution_Goods_Data_Map;

class Auction_Manager {
public:
	Auction_Manager(void);
	~Auction_Manager(void);

	const Aution_Goods_Data_Map &get_autction_goods_data_map(void);
	const Auction_Goods_Data *find_auction_goods_data_by_id(uint32_t id);

	void start(void);
	void tick(const Time_Value &now);

	//玩家主动下架竞拍商品
	int player_todo_under_auction_goods(uint32_t id, role_id_t issue_role_id);

	//增加竞拍商品
	const Auction_Goods_Data &add_auction_goods(uint32_t item_id, uint32_t num, uint32_t continue_time,
			uint32_t auction_price, uint32_t ones_price, role_id_t issue_role_id);

	//一口价购买商品
	int player_ones_price_buy(uint32_t id, role_id_t role_id);

	//更新最新竞拍价(更换最后出价人和最后竞拍价格)
	int update_auction_role_and_price(uint32_t id, role_id_t role_id, uint32_t price);



private:
	//时间到;商品被自动处理
	void time_over_under_auction_goods(const Time_Value &now);

	//定时更新配置上的物品到竞拍商品列表
	void tick_time_update_config_to_auction_goods_list(const Time_Value &now);

	//更新竞拍商品信息到数据库
	void update_auction_goods_to_db(Auction_Goods_Data &goods_data);
	void delete_auction_goods_to_db(uint32_t id);

	void del_auction_goods(uint32_t id);//删除竞拍商品
	void under_goods_issue_mail(Auction_Goods_Data &goods_data);//发送商品人为操作下架,卖家(发布人)邮件
	void under_goods_auction_mail(Auction_Goods_Data &goods_data);//发送商品人为操作下架,竞拍方邮件
	void success_auction_goods_issue_mail(Auction_Goods_Data &goods_data, uint8_t type);//成功拍卖商品(时间到或一口价);卖家(发布人)邮件
	void success_auction_goods_auction_mail(Auction_Goods_Data &goods_data);//成功拍卖商品(时间到类型);竞拍方邮件
	void success_auction_goods_oner_mail(role_id_t role_id, Auction_Goods_Data &goods_data); //成功拍卖商品(一口价类型);一口价方邮件
	void success_oner_goods_actioner_mail(Auction_Goods_Data &goods_data); //成功拍卖商品(一口价类型);最后竞拍方邮件
	void auction_fail_actioner_mail(Auction_Goods_Data &goods_data);//竞拍失败(竞拍方)邮件; 有人比自己出价高的时候就发
	void auction_fail_issue_mail(Auction_Goods_Data &goods_data);//竞拍失败(发布人)邮件(时间到类型); 无人竞拍

	long long int id_uper;
	Aution_Goods_Data_Map aution_goods_data_map_;
};

typedef Singleton<Auction_Manager> Auction_Manager_Single;
#define AUCTION_MANAGER    (Auction_Manager_Single::instance())

#endif /* AUCTION_MANAGER_H_ */
