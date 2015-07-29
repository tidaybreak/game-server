/*
 * Mall.h
 *
 *  Created on: 2012-6-15
 *      Author: ChenTeng
 */

#ifndef MALL_H_
#define MALL_H_

#include "Logic_Player_Base.h"

#include "Mall_Def.h"


class Packer;
struct Item_Detail;
struct Goods_Info;

class Mall : virtual public Packer {
public:
	Mall(void);
	virtual ~Mall();
	void reset(void);
	//请求获取商品列表
	int req_fetch_mall_list();
	void fite_promo_goods_num(std::vector<Goods_Info> &goods_list);
	void fite_tabs_goods_num(std::vector<Goods_Info> &goods_list);
	//请求购买商品
	int req_buy_goods_in_mall(const int32_t id, int32_t &amount, const int32_t tabs, std::string &role_name, const int32_t buy_type, uint8_t sub_type = 0, int8_t helper_fast_use = 0);
	int req_one_key_buy_goods_in_mall(const int32_t tabs, Int_Vec &ids);
	int req_fast_buy_item_in_mall(int32_t item_id, uint32_t amount, uint8_t buy_type);
	int req_fast_buy_item_in_mall_tabs(int32_t item_id, int32_t amount, uint8_t buy_type, int8_t helper_fast_use = 0);

	//interface
	bool find_goods_price_and_num_by_tabs_and_id(int32_t tabs, int32_t id, int32_t &price, int32_t &num);
private:
	int buy_to_self(Item_Detail &item, const int price, Goods_Info &gi);
	int buy_to_player(Item_Detail &item, const int price, Goods_Info &gi, role_id_t role_id);

	void act_redef_price(const Goods_Info &gi, int &price);
};

#endif /* MALL_H_ */
