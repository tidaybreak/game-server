/*
 * Active_Manager.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef ACTIVE_MANAGER_H_
#define ACTIVE_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Active_Struct.h"

struct Item_Detail;

class Active_Manager {

public:
	Active_Manager();
	virtual ~Active_Manager();
	void reset(void);

	int start(void);

	void tick(Time_Value &now);

	void loop_all_db_active(Time_Value &now);
	void loop_all_conf_active(Time_Value &now);
	void loop_switch_on_active(Time_Value &now);

	UInt_Vec get_all_today_open_active(void);

	int get_active_status(uint32_t id, uint32_t &next_hour_min);

	//test
	void set_action_count_rank_active(uint32_t id, Time_Value &time);
	void inner_close_active(uint32_t id, const Time_Value &time, bool dof = false);
	void gm_close_active(uint32_t id, Time_Value &time, bool dof = false);
	void gm_open_active(uint32_t id, Time_Value &time);

	//phpback
	void php_open_active(uint32_t id, uint32_t stime, uint32_t etime);
	void php_close_active(uint32_t id, uint32_t etime);

	//interface
	void active_action(const Switch_On_Event &event, const Switch_On_Active &acitve);
	int get_active_up_day(uint32_t id);//得到当前活动开启的第几天
	bool active_have_up(uint32_t id);//得到活动是否在运行中
	int get_active_add_num(uint32_t id);

private:
	DB_Active_Data_Info_Map db_active_map;
	Switch_On_Active_Map switch_on_map;
	Active_Data_Map active_data_map;   //记录执行次数,上次执行时间;上次执行结束时间
	bool is_change;
//执行数据(次数)相关
	void update_active_data(uint32_t id,const Time_Value &begin_time,const Time_Value &end_time);
	bool check_loop_num(const Active_Config &conf); //检测是否执行到最大次数(按配置的次数)
	bool check_over_time(const Time_Value &start_time, const Active_Config &conf, const Time_Value &now = Time_Value::gettimeofday());
//检测时间类型
	bool check_start_up(const Active_Config &conf, Time_Value &now);
	bool check_open_on_range_week(const Active_Config &conf, Time_Value &now);
	bool check_open_on_range_day(const Active_Config &conf, Time_Value &now);
	bool check_open_on_loop_day(const Active_Config &conf, Time_Value &now);
	bool check_open_on_set_time(const Active_Config &conf, Time_Value &now);
//开始时间
	bool get_start_time(const Active_Config &conf, Time_Value &now, Time_Value &start_time);
	bool get_start_time_on_range_week(const Active_Config &conf, Time_Value &now, Time_Value &start_time);
	bool get_start_time_on_range_day(const Active_Config &conf, Time_Value &now, Time_Value &start_time);
	bool get_start_time_on_loop_day(const Active_Config &conf, Time_Value &now, Time_Value &start_time);
	bool get_start_time_on_set_time(const Active_Config &conf, Time_Value &now, Time_Value &start_time);
	bool get_start_time_on_start_type(const Active_Config &conf, Time_Value &now, Time_Value &start_time);
	uint32_t get_active_continued_time_sec(const Active_Config &conf);
//检测开启间隔
	bool check_blank_set(const Active_Config &conf, Time_Value &now);
	Time_Value get_blank_set_time(const Active_Config &conf);
//时间相关
	tm strtotime(const std::string &str, const std::string &format = "%Y-%m-%d", uint8_t type = 0);
	int get_subdays(const Time_Value &start, const Time_Value &end);
	Time_Value get_day_zero_time(const Time_Value &date);
//开始事件动作
	void todo_action_send_client_notice(const Switch_On_Active &active,const UInt_Vec &argv);
	void todo_action_open_active(const Switch_On_Active &active,const UInt_Vec &argv);
	void todo_action_close_active(const Switch_On_Active &active,const UInt_Vec &argv);
	void todo_open_active_icon(const Switch_On_Active &active,const UInt_Vec &argv);
	void todo_close_active_icon(const Switch_On_Active &active,const UInt_Vec &argv);
	void todo_action_count_rank_active(const Switch_On_Active &active,const UInt_Vec &argv);
//执行活动事件特殊处理
	bool special_active_delay(const Switch_On_Active &active);//全局事件处理
	bool special_active_event_delay(const uint32_t event_type, const Switch_On_Active &active, const UInt_Vec &argv);//单一事件处理,全局事件处理返回false,不会调用单一事件验证
};


typedef Singleton<Active_Manager> Active_Manager_Single;
#define ACTIVE_MANAGER    (Active_Manager_Single::instance())

#endif /* ACTIVE_H_ */
