/*
 * Auction.h
 *
 *  Created on: Jan 21, 2014
 *      Author: xiaoliang
 *
 *      新模块例子
 */

#ifndef AUCTION_H_
#define AUCTION_H_

#include "Logic_Player_Base.h"

struct Auction_Detail;

class Auction : virtual public Logic_Player_Base {
public:
	Auction(void);
	virtual ~Auction();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Auction_Detail &detail);

	// 登录时detail赋值
	void load_detail(Auction_Detail &detail);

	// 保存时detail赋值
	void save_detail(Auction_Detail &data);

	Auction_Detail const &auction_detail(void) const;

	// 登录时module初始化,在这里发客户端消息无效
	void module_init(void);

	// 登录网关后后调用(只会的正常登录调用一次,顶号不会调)
	void sign_in(void);

	// 顶号时调用
	int sign_in_repeat(void);

	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	// 登出前调用
	void sign_out(void);

	// 场景加载完成
	int move_scene_completed(void);

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	//客户端请求相关
	int req_auction_goods_list(void);
	int req_add_auction_goods(uint32_t item_id, uint32_t num, uint32_t continue_time,
			uint32_t auction_price, uint32_t ones_price, uint8_t open_ann);
	int req_under_auction_goods(uint32_t id);
	int req_ones_price_buy_auction_goods(uint32_t id);
	int req_update_auction_goods(uint32_t id, uint32_t price);

private:
	Auction_Detail *auction_detail_;

	uint32_t get_my_goods_num(void);
};

inline Auction_Detail const &Auction::auction_detail(void) const{
	return *auction_detail_;
}

#endif /* AUCTION_H_ */
