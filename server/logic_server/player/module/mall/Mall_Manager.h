/*
 * Mall.h
 *
 *  Created on: 2012-6-15
 *      Author: ChenTeng
 */

#ifndef MALL_MANAGER_H_
#define MALL_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Msg_Struct.h"

typedef std::vector<Mall_Promo_Info> Promo_Goods_Info_Vec;
typedef std::vector<Goods_Info> Goods_Info_Vec;

typedef std::multiset<role_id_t> Role_Id_MSet;
typedef boost::unordered_map<int32_t, Role_Id_MSet> Item_Role_Set_Map;
typedef boost::unordered_map<int32_t, Item_Role_Set_Map> Tabs_Item_Role_Set_Map;

class Mall_Manager {

public:
	Mall_Manager();
	virtual ~Mall_Manager();

	int start(void);
	void tick(Time_Value &now);
	void trigger_daily_zero(const Time_Value &now);
	void trigger_daily_six(const Time_Value &now);

	Goods_Info_Vec &get_goods_info();
	Mall_Promo_Info &get_promo_goods();
	Mall_Promo_Info &get_banner_goods();
	int goods_changed(Goods_Info_Vec &goods_list, Int_Vec &diff_tabs, Promo_Goods_Info_Vec &promo_goods_list, Promo_Goods_Info_Vec &banner_goods_list);
	void update_item_sales_num(int32_t item_id,role_id_t role_id);
	uint32_t get_num_by_item(int32_t item_id);
	uint32_t get_num_by_role_id(int32_t item_id, role_id_t role_id);

	void update_tabs_item_sales_num(int32_t tabs, int32_t item_id,role_id_t role_id);
	uint32_t get_tabs_num_by_item(int32_t tabs, int32_t item_id);
	uint32_t get_tabs_num_by_role_id(int32_t tabs, int32_t item_id, role_id_t role_id);

	int32_t find_goods_price_by_tabs_and_id(int32_t tabs, int32_t id);
private:
	bool refresh_promo_banner_goods(Promo_Goods_Info_Vec &goods_list, Mall_Promo_Info &goods);
	long long int get_day_zero_time(const Time_Value &time);
	long long int get_time_by_int32(const int32_t hm);

	void send_inner_goods_list_to_client(void);

	Goods_Info_Vec goods_list_;
	Promo_Goods_Info_Vec promo_goods_list_;
	Promo_Goods_Info_Vec banner_goods_list_;
	Mall_Promo_Info promo_goods_;
	Mall_Promo_Info banner_goods_;
	Time_Value tick_time_;
	Item_Role_Set_Map item_sales_num;//物品ID->角色ID(mset),因为允许一个用户购买多次或限定次数
	Tabs_Item_Role_Set_Map tabs_item_sales_num;//tabs -> 物品ID->角色ID(mset),因为允许一个用户购买多次或限定次数
	bool save_db;
	uint32_t cycle_day_sum;
};


typedef Singleton<Mall_Manager> Mall_Manager_Single;
#define MALL_MANAGER    (Mall_Manager_Single::instance())

#endif /* MALL_H_ */
