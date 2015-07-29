/*
 * Auction_Sruct.h
 *
 *  Created on: Mar 14, 2014
 *      Author: xiaoliang
 */

#ifndef AUCTION_SRUCT_H_
#define AUCTION_SRUCT_H_

#include "Server_Struct.h"

struct Auction_Goods_Config;
struct Auction_Goods_Info;

struct Auction_Goods_Data {
	Auction_Goods_Data(void);
	Auction_Goods_Data(uint32_t id,uint32_t item_id, uint32_t num, uint32_t continue_time,
			uint32_t auction_price, uint32_t ones_price, role_id_t issue_role_id);
	void reset(void);

	Auction_Goods_Data(const Auction_Goods_Config &conf);
	void update_of_config(const Auction_Goods_Config &conf);
	void make_auction_goods_info(Auction_Goods_Info &agi) const;

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	uint32_t id;//id
	uint32_t item_id;//道具ID
	uint32_t num;//数量
	uint32_t auction_price;//默认竞拍价
	uint32_t ones_price;//默认一口价
	Time_Value over_time;//过期时间
	Time_Value start_time;//开始时间
	uint32_t continue_time;//延续时长(小时)
	role_id_t issue_role_id;//拍卖商品发布人
	role_id_t auction_role_id;//最后一次竞拍人
	uint32_t l_auction_price;//最后一次竞拍价
	Role_Id_Set auction_fail_role_id;//所有竞拍过,但又失败的角色ID
};

struct Auction_Detail: public Detail {
	Auction_Detail(void);
	virtual ~Auction_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);
};

#endif /* AUCTION_SRUCT_H_ */
