/*
 * Secret_Shop_Manager.h
 *
 *  Created on: 2014年7月30日
 *      Author: root
 */

#ifndef SECRET_SHOP_MANAGER_H_
#define SECRET_SHOP_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "Secret_Shop_Struct.h"
#include "Monitor_Struct.h"

class Logic_Player;

class Secret_Shop_Manager{
	typedef boost::unordered_map<int, Monitor_Unique_ID> Main_Scene_Map;
public:
	Secret_Shop_Manager(void);
	~Secret_Shop_Manager(void);
public:
	int start(void);
	int tick(const Time_Value &now);
	void init_main_scene(void);
	//	注册主城场景
	void register_main_scene(const Monitor_Unique_ID& muid);
	// 注销主城场景
	void unregister_main_scene(const Monitor_Unique_ID& muid);

	int refresh_shop(void);
public:
	// 请求购买商品
	int req_buy_shop_item(Logic_Player* player, int index);
	// 请求商店信息
	int req_fetch_shop_info(Logic_Player* player);

	void trigger_daily_six(const Time_Value &now);

	void get_month_hero_info(int& cfg_id, int& remain_day);
private:
	// 打开商店
	int open_shop(const Time_Value &now);
	// 关闭商店
	int close_shop(const Time_Value &now);
	// 激活商店
	int activate_shop(const Time_Value &now);
	// 神秘商人出现公告
	int announce_open_shop(void);
	//
	int announce_opening_shop(const Time_Value &now);
	// 神秘商人消失公告
	int announce_close_shop(void);

	void cal_month_hero_info(const Time_Value& now, int& cfg_id, int& cfg_next_id, int& cfg_remain_day);
private:
	Main_Scene_Map main_scene_map_;
	std::vector<SC_Item_Info> shop_item_;
	Time_Value open_time_;
	Time_Value close_time_;
	Time_Value activate_time_;
	Coord ref_point_;
	std::vector<int> ann_id_vec_;
	int ann_space_;
	int ann_index_;
	Time_Value ann_tick_;
	bool shop_open_;
	int scene_id_;
	int chapman_;
	// 每月英雄
	int month_hero_cfg_id_;
	int month_hero_cfg_next_id_;
	int month_hero_remain_day_;
};

typedef Singleton<Secret_Shop_Manager> Secret_Shop_Manager_Single;
#define SECRET_SHOP_MANAGER (Secret_Shop_Manager_Single::instance())

#endif /* SECRET_SHOP_MANAGER_H_ */
