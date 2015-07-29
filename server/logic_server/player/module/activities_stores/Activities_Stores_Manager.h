/*
 * Activities_Stores_Manager.h
 *
 *  Created on: 2012-6-15
 *      Author: ChenTeng
 */

#ifndef ACTIVITIES_STORES_MANAGER_H_
#define ACTIVITIES_STORES_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Msg_Struct.h"

typedef std::vector<Activity_Exchange_Mall_Goods_Info> Activity_Exchange_Mall_Goods_Info_Vec;
typedef boost::unordered_map<uint32_t, Activity_Exchange_Mall_Goods_Info> Activity_Exchange_Mall_Goods_Info_Map;

class Activities_Stores_Manager {

public:
	Activities_Stores_Manager();
	virtual ~Activities_Stores_Manager();

	int start(void);
	void tick(Time_Value &now);
	void trigger_daily_zero(const Time_Value &now);
	void trigger_daily_six(const Time_Value &now);

	void open(const Time_Value start, const Time_Value end);
	void end(const Time_Value end);

	bool is_open(void);

	Activity_Exchange_Mall_Goods_Info_Map &get_goods_map(void);
	Activity_Exchange_Mall_Goods_Info *get_goods_by_id(uint32_t id);

	void php_send_activity_exchange_mall_goods_change(Activity_Exchange_Mall_Goods_Info_Vec &goods);


private:
	void copy_vec_to_map(Activity_Exchange_Mall_Goods_Info_Vec &goods);
	int get_goods_act_id(Activity_Exchange_Mall_Goods_Info_Vec &goods);

	Time_Value start_time;
	Time_Value end_time;

	Activity_Exchange_Mall_Goods_Info_Map goods_map;
};


typedef Singleton<Activities_Stores_Manager> Activities_Stores_Manager_Single;
#define ACTIVITIES_STORES_MANAGER    (Activities_Stores_Manager_Single::instance())

#endif /* ACTIVITIES_STORES_H_ */
