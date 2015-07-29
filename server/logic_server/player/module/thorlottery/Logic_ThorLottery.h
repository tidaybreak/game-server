/*
 * Logic_ThorLottery.h
 *
 *  Created on: 2014年11月11日
 *      Author: root
 */

#ifndef LOGIC_THORLOTTERY_H_
#define LOGIC_THORLOTTERY_H_

#include "Logic_Player_Base.h"

struct ThorLottery_Item_Return;
struct ThorLottery_Detail;
struct ThorLottery_Item_Config;
struct ThorLottery_Order_Config;
struct ThorLottery_Manager_Detail;
typedef std::vector<ThorLottery_Item_Return> ThorLottery_Item_Return_Vec;
typedef boost::unordered_map<int, ThorLottery_Item_Config const*> Get_Item_Map;

class Logic_ThorLottery : virtual public Logic_Player_Base{
public:
	Logic_ThorLottery(void);
	~Logic_ThorLottery(void);
	void reset(void);

public:
	void load_detail(ThorLottery_Detail &detail);
	void save_detail(ThorLottery_Detail &detail);

	void sign_in(void);
	void sign_out(void);
	void module_init(void);

	int trigger_daily_six(const Time_Value& now);

	int get_thorlottery_info(void);
	int handle_lottery(int type, int times);
	int lottery_by_diamond(int times, 	Get_Item_Map& get_item_map, ThorLottery_Item_Return_Vec& item_return_vec);
	int lottery_by_goods(int times, Get_Item_Map& get_item_map, ThorLottery_Item_Return_Vec& item_return_vec);
	int lottery_by_diamond_and_goods(int times, Get_Item_Map& get_item_map, ThorLottery_Item_Return_Vec& item_return_vec);

	int handle_lottery(int times);
	int handle_exchange_item(int order, int item_id);

	const ThorLottery_Detail& thorlottery_detail(void);

private:
	void init_player_lottery_data(int season_id);
	// 单次抽奖操作
	ThorLottery_Item_Config const* handle_lottery_once(void);
	// 计算该季单个奖品的权值，当权值为0时，则忽略该奖品
	int get_item_weight(const ThorLottery_Order_Config &cfg, const ThorLottery_Manager_Detail& manager_detail);
	// 计算所需的钻石
	int handle_calc_cost_need(int goods_num, int times);

private:
	ThorLottery_Detail *thorlottery_detail_;
};

#endif /* LOGIC_THORLOTTERY_H_ */
