/*
 * Active_Manager.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include "Active_Manager.h"
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Msg_Struct.h"
#include "Global_Timer.h"
#include "Config_Cache_Active.h"
#include "Active_Def.h"
#include "Config_Cache.h"
#include "arena/Arena_Manager.h"
#include "valentine_monster/Valentine_Monster_Active_Manager.h"
#include "world_boss/World_Boss_Active_Manager.h"
#include "honor_arena/Honor_Arena_Active_Manager.h"
#include "Active_Content_Manager.h"
#include "recharge/Recharge_Rebate_Manager.h"
#include "recharge/Two_Recharge_Manager.h"
#include "recharge/First_Recharge_Manager.h"
#include "recharge/Limit_Time_Recharge_Manager.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"
#include "invest_plan/Invest_Plan_Manager.h"
#include "thorlottery/ThorLottery_Manager.h"
#include "answer/Answer_Manager.h"
#include "Config_Cache_Active_Content.h"
#include "activities_stores/Activities_Stores_Manager.h"
#include "DB_Def.h"
#include "oper_activity/Oper_Activity_Manager.h"
#include "Active_Content_Def.h"

Active_Manager::Active_Manager() {
	// TODO Auto-generated constructor stub
	reset();
}

Active_Manager::~Active_Manager() {
	// TODO Auto-generated destructor stub
}

void Active_Manager::reset(void) {
	switch_on_map.clear();
	active_data_map.clear();
	db_active_map.clear();
	is_change = false;
}

int Active_Manager::start(void) {
	MONGO_OPERATOR->load_active_data(active_data_map);
	MONGO_OPERATOR->load_active_db_event(db_active_map);
	return 0;
}

UInt_Vec Active_Manager::get_all_today_open_active(void) {
	const Active_Conf_Map &conf_map = CONFIG_CACHE_ACTIVE->act_conf_map();
	Time_Value now = Time_Value::gettimeofday();
	UInt_Vec re;
	re.clear();
	for(Active_Conf_Map::const_iterator conf_iter = conf_map.begin(); conf_iter != conf_map.end(); ++conf_iter) {
		if(check_start_up(conf_iter->second, now)) {
			re.push_back(conf_iter->second.id);
		}
	}
	return re;
}

void Active_Manager::set_action_count_rank_active(uint32_t id, Time_Value &time) {
	Switch_On_Active_Map::iterator on_iter = switch_on_map.find(id);
	if(on_iter != switch_on_map.end()) {
		for(Switch_On_Event_Vec::iterator on_action_it = on_iter->second.event_action.begin();
						 on_action_it != on_iter->second.event_action.end(); ++on_action_it) {
			if(on_action_it->action == ACTION_COUNT_RANK) {
				on_action_it->is_run = false;
				on_action_it->on_time = time;
				return;
			}
		}
	}
}

void Active_Manager::inner_close_active(uint32_t id, const Time_Value &time, bool dof) {
	Switch_On_Active_Map::iterator on_iter = switch_on_map.find(id);
	if(on_iter != switch_on_map.end()) {
		for(Switch_On_Event_Vec::iterator on_action_it = on_iter->second.event_action.begin();
						 on_action_it != on_iter->second.event_action.end(); ++on_action_it) {
			if(on_action_it->is_run == false) {
				on_action_it->is_run = dof;
				if(on_action_it->on_time.sec() > time.sec()) {
					on_action_it->on_time = time;
				}
			}
		}//end for
	}
}

void Active_Manager::gm_close_active(uint32_t id, Time_Value &time, bool dof) {
	Switch_On_Active_Map::iterator on_iter = switch_on_map.find(id);
	if(on_iter != switch_on_map.end()) {
		for(Switch_On_Event_Vec::iterator on_action_it = on_iter->second.event_action.begin();
						 on_action_it != on_iter->second.event_action.end(); ++on_action_it) {
			if(on_action_it->is_run == false) {
				on_action_it->is_run = dof;
				if(on_action_it->on_time.sec() > time.sec()) {
					on_action_it->on_time = time;
				}
			}
		}//end for
		on_iter->second.end_time = time;
	}
}

void Active_Manager::php_open_active(uint32_t id, uint32_t stime, uint32_t etime) {
	MSG_DEBUG("PHP OPEN ACTIVE ID:%d, start:%ld, end:%ld", id, stime, etime);
	Time_Value now = Time_Value::gettimeofday();
	DB_Active_Data_Info_Map::iterator iter =  db_active_map.find(id);
	if(iter != db_active_map.end()) {
		iter->second.start_time.sec(stime);
		iter->second.end_time.sec(etime);
	} else {
		DB_Active_Data_Info value;
		value.id = id;
		value.start_time.sec(stime);
		value.end_time.sec(etime);
		db_active_map.insert(std::make_pair(id, value));
	}
	gm_close_active(id, now);
}

void Active_Manager::php_close_active(uint32_t id, uint32_t etime) {
	MSG_DEBUG("PHP CLOSE ACTIVE ID:%d, end:%ld", id, etime);
	Time_Value end_time = Time_Value(etime);
	DB_Active_Data_Info_Map::iterator iter =  db_active_map.find(id);
	if(iter != db_active_map.end()) {
		iter->second.midway_end_time.sec(etime);
	} else {
		DB_Active_Data_Info value;
		value.id = id;
		value.midway_end_time.sec(etime);
		db_active_map.insert(std::make_pair(id, value));
	}
	gm_close_active(id, end_time);
}

void Active_Manager::gm_open_active(uint32_t id, Time_Value &start_time) {
	const Active_Conf_Map &conf_map = CONFIG_CACHE_ACTIVE->act_conf_map();
	Active_Conf_Map::const_iterator conf_iter = conf_map.find(id);
	if(conf_iter != conf_map.end()) {
		Time_Value end_time = Time_Value(start_time.sec() + conf_iter->second.continued * 60 + conf_iter->second.before_time * Time_Value::ONE_MINUTE_IN_SECS);
		if(conf_iter->second.continued == 0) {
			uint32_t continued = get_active_continued_time_sec(conf_iter->second);
			if(continued == 0) { return; }
			end_time = Time_Value(end_time.sec() + continued);
		}
		Switch_On_Active switch_on;
		switch_on.id = conf_iter->second.id;
		switch_on.type = conf_iter->second.type;
		switch_on.openLevel = conf_iter->second.openLevel;
		switch_on.icon = conf_iter->second.icon;
		switch_on.start_time.sec(start_time.sec() + conf_iter->second.before_time * Time_Value::ONE_MINUTE_IN_SECS);
		switch_on.before_start_time = start_time;
		switch_on.end_time = end_time;
		Switch_On_Event switch_on_event;
		MSG_DEBUG("gm_make activity switch_on activity id:%d start:%ld before_start:%ld end:%ld", switch_on.id, switch_on.start_time.sec(), switch_on.before_start_time.sec(), switch_on.end_time.sec());
		for(Event_Vec::const_iterator siter = conf_iter->second.event_action.begin(); siter != conf_iter->second.event_action.end(); ++siter ) {
			switch_on_event.reset();
			switch_on_event.action =  siter->action;
			switch_on_event.argv = siter->argv;
			if(siter->rel_back) {
				switch_on_event.on_time.sec(end_time.sec() - siter->relatively  * Time_Value::ONE_MINUTE_IN_SECS);
			} else {
				uint32_t relatively = siter->relatively;
				switch_on_event.on_time.sec(start_time.sec() + relatively * Time_Value::ONE_MINUTE_IN_SECS);
			}
			switch_on.event_action.push_back(switch_on_event);
			MSG_DEBUG("gm_make activity switch_on of event actionId:%d on_time:%ld", switch_on_event.action, switch_on_event.on_time.sec());
		}
		Switch_On_Active_Map::iterator runit = switch_on_map.find(conf_iter->first);
		if(runit != switch_on_map.end()) {
			MSG_DEBUG("gm_make activity is runing switch_on activity id:%d start:%ld before_start:%ld end:%ld", switch_on.id, switch_on.start_time.sec(), switch_on.before_start_time.sec(), switch_on.end_time.sec());
			runit->second = switch_on;
		} else {
			MSG_DEBUG("gm_make activity is not runing switch_on activity id:%d start:%ld before_start:%ld end:%ld", switch_on.id, switch_on.start_time.sec(), switch_on.before_start_time.sec(), switch_on.end_time.sec());
			switch_on_map.insert(std::make_pair(conf_iter->first, switch_on));
		}
//		update_active_data(conf_iter->first, start_time, switch_on.end_time);//更新执行次数
	}
}

int Active_Manager::get_active_status(uint32_t id, uint32_t &next_hour_min) {
	if(switch_on_map.find(id) != switch_on_map.end()) {
		return ACTIVE_OPEING_STATUS;
	} else {
		const Active_Conf_Map &conf_map = CONFIG_CACHE_ACTIVE->act_conf_map();
		Active_Conf_Map::const_iterator cit = conf_map.find(id);
		if(cit != conf_map.end()) {
			Time_Value now = Time_Value::gettimeofday();
			Time_Value start_time = Time_Value::zero;
			get_start_time(cit->second, now, start_time);
			if(!check_start_up(cit->second, now) || !check_loop_num(cit->second) || check_over_time(start_time, cit->second)) {
				return ACTIVE_NOT_OPEN_STATUS; //当天不会开启 或 次数完了,不会再开;
			}
			tm tm_now;
			time_t t_now = now.sec();
			localtime_r(&t_now, &tm_now);
			uint32_t hour_min = tm_now.tm_hour*100+tm_now.tm_min;//当前时分
			uint32_t next_hour_min_ = 9999;//下一个开始时分
			for(UInt_Vec::const_iterator it = cit->second.time.begin(); it != cit->second.time.end(); ++it) {
						//当前时分     //下一个开始时分
				if(*it > hour_min && next_hour_min_ > *it) { //求出下一个开始时分 ,
					next_hour_min_ = *it;
				}
			}
			next_hour_min = next_hour_min_;
			if(next_hour_min_ == 9999) {//没有下一个开始时分
				return ACTIVE_NOT_OPEN_STATUS; //今天没有下一个开启,过期
			} else {
				return ACTIVE_WILL_OPEN_STATUS; //今天会再次开启,时间是next_hour_min
			}
		}
		return ACTIVE_NOT_OPEN_STATUS;
	}
}

void Active_Manager::tick(Time_Value &now) {
	loop_all_db_active(now);
	loop_all_conf_active(now);
	loop_switch_on_active(now);
	if(is_change) {
		is_change = false;
		Block_Buffer buf;
		buf.make_message(MONGO_SAVE_ACTIVE_DATA);
		buf.write_uint16(active_data_map.size());
		for(Active_Data_Map::iterator it =  active_data_map.begin(); it != active_data_map.end(); ++it) {
			it->second.serialize(buf);
		}
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, 0);
	}
}

void Active_Manager::loop_all_db_active(Time_Value &now) {
	const Active_Conf_Map &conf_map = CONFIG_CACHE_ACTIVE->act_conf_map();
	for(DB_Active_Data_Info_Map::iterator db_iter = db_active_map.begin(); db_iter != db_active_map.end(); ++db_iter) {
		Active_Conf_Map::const_iterator conf_iter = conf_map.find(db_iter->first);
		if(conf_iter == conf_map.end()) {
			continue;
		}
		if(switch_on_map.find(conf_iter->first) != switch_on_map.end()) {
			continue;
		}
		if(!check_loop_num(conf_iter->second)) {
			continue;
		}
		if(db_iter->second.start_time.sec() ==0 && db_iter->second.end_time.sec() == 0) {
			continue;
		}
		//得到开始时间
		Time_Value start_time = db_iter->second.start_time;
		start_time.sec(start_time.sec() -  conf_iter->second.before_time * Time_Value::ONE_MINUTE_IN_SECS);
		//得到结束时间
		Time_Value end_time = db_iter->second.end_time;
		if(db_iter->second.end_time.sec() == 0) {//后台没配置结束时间按配置的结束时间
			end_time = Time_Value(start_time.sec() + conf_iter->second.continued * 60 + conf_iter->second.before_time * Time_Value::ONE_MINUTE_IN_SECS);
			if(conf_iter->second.continued == 0) {//配置没有配置持续时间;看有没回调
				uint32_t continued = get_active_continued_time_sec(conf_iter->second);
				if(continued == 0) { continue; }
				end_time = Time_Value(end_time.sec() + continued);
			}
		}
		//判断这一刻是否要执行活动
		if((now.sec() >= start_time.sec() && now.sec() < end_time.sec()) && (db_iter->second.midway_end_time.sec() < start_time.sec() || db_iter->second.midway_end_time.sec() > end_time.sec())) {
			Switch_On_Active switch_on;
			switch_on.id = conf_iter->second.id;
			switch_on.type = conf_iter->second.type;
			switch_on.openLevel = conf_iter->second.openLevel;
			switch_on.icon = conf_iter->second.icon;
			switch_on.start_time.sec(start_time.sec() + conf_iter->second.before_time * Time_Value::ONE_MINUTE_IN_SECS);
			switch_on.before_start_time = start_time;
			switch_on.end_time = end_time;
			Switch_On_Event switch_on_event;
			MSG_DEBUG("db make activity switch_on activity id:%d start:%ld before_start:%ld end:%ld", switch_on.id, switch_on.start_time.sec(), switch_on.before_start_time.sec(), switch_on.end_time.sec());
			for(Event_Vec::const_iterator siter = conf_iter->second.event_action.begin(); siter != conf_iter->second.event_action.end(); ++siter ) {
				switch_on_event.reset();
				switch_on_event.action =  siter->action;
				switch_on_event.argv = siter->argv;
				if(siter->rel_back) {
					switch_on_event.on_time.sec(end_time.sec() - siter->relatively  * Time_Value::ONE_MINUTE_IN_SECS);
				} else {
					switch_on_event.on_time.sec(start_time.sec() + siter->relatively * Time_Value::ONE_MINUTE_IN_SECS);
				}
				switch_on.event_action.push_back(switch_on_event);
				MSG_DEBUG("db make activity switch_on of event actionId:%d on_time:%ld", switch_on_event.action, switch_on_event.on_time.sec());
			}
			switch_on_map.insert(std::make_pair(conf_iter->first, switch_on));
//			update_active_data(conf_iter->first, start_time, switch_on.end_time);//更新执行次数
		}
	}
}

void Active_Manager::loop_all_conf_active(Time_Value &now) {
	const Active_Conf_Map &conf_map = CONFIG_CACHE_ACTIVE->act_conf_map();
	for(Active_Conf_Map::const_iterator conf_iter = conf_map.begin(); conf_iter != conf_map.end(); ++conf_iter) {
		//MSG_DEBUG("loop conf active id:%d", conf_iter->first);
		if(switch_on_map.find(conf_iter->first) != switch_on_map.end()) {
			continue;
		}
		if(!check_loop_num(conf_iter->second)) {
			continue;
		}
		Time_Value start_time = Time_Value::zero;
		if(get_start_time(conf_iter->second, now, start_time)) {
			if(check_over_time(start_time,conf_iter->second)) continue;//不考虑提前的时间
			start_time.sec(start_time.sec() -  conf_iter->second.before_time * Time_Value::ONE_MINUTE_IN_SECS);
			Time_Value end_time = Time_Value(start_time.sec() + conf_iter->second.continued * 60 + conf_iter->second.before_time * Time_Value::ONE_MINUTE_IN_SECS);
			if(conf_iter->second.continued == 0) {
				uint32_t continued = get_active_continued_time_sec(conf_iter->second);
				if(continued == 0) { continue; }
				end_time = Time_Value(end_time.sec() + continued);
			}
			if(now.sec() >= start_time.sec() && now.sec() < end_time.sec()) {
				//如果后台停止了活动 在本次活动范围内
				DB_Active_Data_Info_Map::iterator db_iter = db_active_map.find(conf_iter->second.id);
				if(db_iter != db_active_map.end()) {
					if(db_iter->second.midway_end_time.sec() > start_time.sec() && db_iter->second.midway_end_time.sec() < end_time.sec()) {
						continue;
					}
				}
				Switch_On_Active switch_on;
				switch_on.id = conf_iter->second.id;
				switch_on.type = conf_iter->second.type;
				switch_on.openLevel = conf_iter->second.openLevel;
				switch_on.icon = conf_iter->second.icon;
				switch_on.start_time.sec(start_time.sec() + conf_iter->second.before_time * Time_Value::ONE_MINUTE_IN_SECS);
				switch_on.before_start_time = start_time;
				switch_on.end_time = end_time;
				Switch_On_Event switch_on_event;
				MSG_DEBUG("make activity switch_on activity id:%d start:%ld before_start:%ld end:%ld", switch_on.id, switch_on.start_time.sec(), switch_on.before_start_time.sec(), switch_on.end_time.sec());
				for(Event_Vec::const_iterator siter = conf_iter->second.event_action.begin(); siter != conf_iter->second.event_action.end(); ++siter ) {
					switch_on_event.reset();
					switch_on_event.action =  siter->action;
					switch_on_event.argv = siter->argv;
					if(siter->rel_back) {
						switch_on_event.on_time.sec(end_time.sec() - siter->relatively  * Time_Value::ONE_MINUTE_IN_SECS);
					} else {
						switch_on_event.on_time.sec(start_time.sec() + siter->relatively * Time_Value::ONE_MINUTE_IN_SECS);
					}
					switch_on.event_action.push_back(switch_on_event);
					MSG_DEBUG("make activity switch_on of event actionId:%d on_time:%ld", switch_on_event.action, switch_on_event.on_time.sec());
				}
				switch_on_map.insert(std::make_pair(conf_iter->first, switch_on));
//				update_active_data(conf_iter->first, start_time, switch_on.end_time);//更新执行次数
			}
		}
//		if(check_start_up(conf_iter->second, now)) {
//			tm tm_now;
//			time_t t_now = now.sec() + conf_iter->second.before_time * 60;
//			localtime_r(&t_now, &tm_now);
//			uint32_t hour_min = tm_now.tm_hour*100+tm_now.tm_min;
//			for(UInt_Vec::const_iterator it = conf_iter->second.time.begin(); it != conf_iter->second.time.end(); ++it) {
//				if(*it == hour_min) {//todo 有可能需要优化
//					Switch_On_Active switch_on;
//					switch_on.id = conf_iter->second.id;
//					switch_on.start_time = Time_Value(t_now);
//					switch_on.end_time.sec(switch_on.start_time.sec() + conf_iter->second.continued * 60);
//					Switch_On_Event switch_on_event;
//					for(Event_Vec::const_iterator siter = conf_iter->second.event_action.begin(); siter != conf_iter->second.event_action.end(); ++siter ) {
//						switch_on_event.reset();
//						switch_on_event.action =  siter->action;
//						switch_on_event.argv = siter->argv;
//						switch_on_event.on_time.sec(now.sec() + siter->relatively * 60);
//						switch_on.event_action.push_back(switch_on_event);
//					}
//					switch_on_map.insert(std::make_pair(conf_iter->first, switch_on));
//					update_active_data(conf_iter->first, now, switch_on.end_time);//更新执行次数
//					continue;
//				}
//			}
//		}
	}
}

void Active_Manager::loop_switch_on_active(Time_Value &now) {
	UInt_Vec to_del;
	to_del.clear();
	for(Switch_On_Active_Map::iterator on_iter = switch_on_map.begin(); on_iter != switch_on_map.end(); ++on_iter) {
		if(special_active_delay(on_iter->second)) continue;//特殊活动延后处理
		unsigned int num = 0;
		for(Switch_On_Event_Vec::iterator on_action_it = on_iter->second.event_action.begin();
				 on_action_it != on_iter->second.event_action.end(); ++on_action_it) {
			if(special_active_event_delay(on_action_it->action, on_iter->second, on_action_it->argv)) continue;//单一特殊事件延后处理
			if(on_action_it->on_time <= now && !on_action_it->is_run) {
				on_action_it->is_run = true;
				++num;
				MSG_DEBUG("loop active:%d do action:%d", on_iter->second.id, on_action_it->action);
				active_action(*on_action_it, on_iter->second);
				switch(on_action_it->action) {
					//根据相应的活动action做相应的事情
					case ACTION_SEND_CLIENT_NOTICE: {//通知客户端预告
						todo_action_send_client_notice(on_iter->second, on_action_it->argv);
						break;
					}
					case ACTION_OPEN_ACTIVE: { //开启活动
						todo_action_open_active(on_iter->second, on_action_it->argv);
						break;
					}
					case ACTION_CLOSE_ACTIVE: { //关闭活动
						todo_action_close_active(on_iter->second, on_action_it->argv);
						break;
					}
					case ACTION_OPEN_ICON: { //开启图标
						todo_open_active_icon(on_iter->second, on_action_it->argv);
						break;
					}
					case ACTION_CLOSE_ICON: { //关闭图标
						todo_close_active_icon(on_iter->second, on_action_it->argv);
						break;
					}
					case ACTION_SEND_CHAT: { //消息广播
						break;
					}
					case ACTION_COUNT_RANK: {//结算排行榜
						todo_action_count_rank_active(on_iter->second, on_action_it->argv);
						break;
					}
				}
			}
		}
		//判断是否执行完所有的事件
		if(on_iter->second.event_action.size() && on_iter->second.event_action.size() <= num) { //有事件列表的; 按事件完成为结束
			//to_del.push_back(on_iter->first);//事件完成;活动结束时间未到会有异常
		} else if(on_iter->second.end_time.sec() < now.sec()) {//事件未完成的;活动结束也删除
			to_del.push_back(on_iter->first);
		}
	}
	//删除已经执行完的
	for(UInt_Vec::iterator it = to_del.begin(); it != to_del.end(); ++it) {
		Switch_On_Active_Map::iterator del_iter = switch_on_map.find(*it);
		if(del_iter != switch_on_map.end()) {
			update_active_data(del_iter->first, del_iter->second.start_time, del_iter->second.end_time);//更新执行次数
			switch_on_map.erase(del_iter);
		}
	}
}

bool Active_Manager::check_start_up(const Active_Config &conf, Time_Value &now) {
	switch(conf.open_type) {
		case OPEN_ON_RANGE_WEEK: { //1.指定星期,eq:[1,3,5] 指定周一三五
			return check_open_on_range_week(conf, now);
			break;
		}
		case OPEN_ON_RANGE_DAY: { //2.指定日, eq:[1,6,20] 指定每月1号,6号,20号
			return check_open_on_range_day(conf, now);
			break;
		}
		case OPEN_ON_LOOP_DAY: { //3.日循环, eq:5, 间隔5天执行一次
			return check_open_on_loop_day(conf, now);
			break;
		}
		case OPEN_ON_SET_TIME: { //4.指定日期, eq:2014-12-14, 指定的时间开启
			return check_open_on_set_time(conf, now);
			break;
		}
	}
	return false;
}

bool Active_Manager::check_open_on_loop_day(const Active_Config &conf, Time_Value &now) {
	if((get_subdays(now, get_blank_set_time(conf)) % 3) == 0) {
		return true;
	}
	return false;
}

bool Active_Manager::check_open_on_set_time(const Active_Config &conf, Time_Value &now) {
	tm tm_ = strtotime(conf.assign_open_date);
	tm tm_now;
	time_t t_now = now.sec();
	localtime_r(&t_now, &tm_now);

	if(tm_.tm_year == tm_now.tm_year && tm_.tm_mon == tm_now.tm_mon && tm_.tm_wday == tm_now.tm_wday) {
		return true;
	}
	return false;
}

bool Active_Manager::get_start_time(const Active_Config &conf, Time_Value &now, Time_Value &start_time) {
	switch(conf.open_type) {
		case OPEN_ON_RANGE_WEEK: { //1.指定星期,eq:[1,3,5] 指定周一三五
			return get_start_time_on_range_week(conf, now, start_time);
			break;
		}
		case OPEN_ON_RANGE_DAY: { //2.指定日, eq:[1,6,20] 指定每月1号,6号,20号
			return get_start_time_on_range_day(conf, now, start_time);
			break;
		}
		case OPEN_ON_LOOP_DAY: { //3.日循环, eq:5, 间隔5天执行一次
			return get_start_time_on_loop_day(conf, now, start_time);
			break;
		}
		case OPEN_ON_SET_TIME: { //4.指定日期, eq:2014-12-14, 指定的时间开启
			return get_start_time_on_set_time(conf, now, start_time);
			break;
		}
		case OPEN_ON_START_TYPE: {//5.按起始类型执行
			return get_start_time_on_start_type(conf, now, start_time);
			break;
		}
	}
	return false;
}

bool Active_Manager::get_start_time_on_start_type(const Active_Config &conf, Time_Value &now, Time_Value &start_time) {
	start_time = get_blank_set_time(conf);
	return true;
}

uint32_t Active_Manager::get_active_continued_time_sec(const Active_Config &conf) {
	switch(conf.id) {
		case ACTION_ACTIVE_ANSWER_SYS: {
			return ANSWER_MANAGER->get_continued_time_sec();
			break;
		}
	}
	return 0;
}

bool Active_Manager::get_start_time_on_set_time(const Active_Config &conf, Time_Value &now, Time_Value &start_time) {
	tm tm_ = strtotime(conf.assign_open_date);
	start_time.sec(mktime(&tm_));
	return true;
}

bool Active_Manager::get_start_time_on_loop_day(const Active_Config &conf, Time_Value &now, Time_Value &start_time) {
	Time_Value blank_set_time = get_blank_set_time(conf);
	start_time = Time_Value::zero;
	if(now.sec() >= blank_set_time.sec()) {
		int spli = get_subdays(blank_set_time, now) / conf.loop_open_date;
		start_time.sec(blank_set_time.sec() + conf.loop_open_date * spli * Time_Value::ONE_DAY_IN_SECS);
		return true;
	}
	return false;
}

bool Active_Manager::get_start_time_on_range_day(const Active_Config &conf, Time_Value &now, Time_Value &start_time) {
	start_time = Time_Value::zero;
	if(check_blank_set(conf, now)) {
		tm tm_now;
		time_t t_now = now.sec();
		localtime_r(&t_now, &tm_now);
		for(UInt_Vec::const_iterator day_it = conf.interval_open_date.begin(); day_it != conf.interval_open_date.end(); ++day_it) {
			tm tmp_tm_now = tm_now;
			if(tm_now.tm_mday < (int)*day_it) {
				--tmp_tm_now.tm_mon;
				tmp_tm_now.tm_mday = (int)*day_it;
			} else {
				tmp_tm_now.tm_mday = (int)*day_it;
			}
			for(UInt_Vec::const_iterator time_it = conf.time.begin(); time_it != conf.time.end(); ++time_it) {
				int hour = (*time_it)/100;
				int min = (*time_it) % 100;

				tmp_tm_now.tm_hour = hour;
				tmp_tm_now.tm_min = min;  //那一天那一小时那一分钟的时间
				int32_t tmp_spli_sec = now.sec() - mktime(&tmp_tm_now);
				int32_t start_spli_sec = now.sec() - start_time.sec();
				if(tmp_spli_sec >= 0 && tmp_spli_sec < start_spli_sec) {//间隔时间最小的那个是最近要开始的
					start_time.sec(mktime(&tmp_tm_now));
				}
			}
		}
		if(start_time != Time_Value::zero) {
			return true;
		}
	}
	return false;
}

bool Active_Manager::get_start_time_on_range_week(const Active_Config &conf, Time_Value &now, Time_Value &start_time) {
	start_time = Time_Value::zero;
	if(check_blank_set(conf, now)) {
		tm tm_now;
		time_t t_now = now.sec();
		localtime_r(&t_now, &tm_now);
		for(UInt_Vec::const_iterator week_it = conf.interval_open_date.begin(); week_it != conf.interval_open_date.end(); ++week_it) {
			//时间间隔
			int spli_day;
			if(tm_now.tm_wday < (int)*week_it) {
				spli_day = Time_Value::ONE_WEEK_DAYS - ((int)*week_it - tm_now.tm_wday);
			} else {
				spli_day = tm_now.tm_wday - (int)*week_it;
			}
			Time_Value tmp_time = Time_Value(now.sec() - spli_day * Time_Value::ONE_DAY_IN_SECS);//那一天的时间
			for(UInt_Vec::const_iterator time_it = conf.time.begin(); time_it != conf.time.end(); ++time_it) {
				int hour = (*time_it)/100;
				int min = (*time_it) % 100;
				Date_Time dt_time = Date_Time(tmp_time);
				dt_time.hour(hour);
				dt_time.minute(min);  //那一天那一小时那一分钟的时间
				dt_time.second(0);
				int32_t tmp_spli_sec = abs(now.sec() - dt_time.time_sec());
				int32_t start_spli_sec = abs(now.sec() - start_time.sec());
				if(tmp_spli_sec >= 0 && tmp_spli_sec < start_spli_sec) {//间隔时间最小的那个是最近要开始的
					start_time.sec(dt_time.time_sec());
				}
			}
		}
		if(start_time != Time_Value::zero) {
			return true;
		}
	}
	return false;
}

bool Active_Manager::check_open_on_range_day(const Active_Config &conf, Time_Value &now) {
	if(check_blank_set(conf, now)) {
		tm tm_now;
		time_t t_now = now.sec();
		localtime_r(&t_now, &tm_now);
		for(UInt_Vec::const_iterator week_it = conf.interval_open_date.begin(); week_it != conf.interval_open_date.end(); ++week_it) {
			if(tm_now.tm_mday == (int)*week_it) {
				return true;
			}
		}
	}
	return false;
}

bool Active_Manager::check_open_on_range_week(const Active_Config &conf, Time_Value &now) {
	if(check_blank_set(conf, now)) {
		tm tm_now;
		time_t t_now = now.sec();
		localtime_r(&t_now, &tm_now);
		for(UInt_Vec::const_iterator week_it = conf.interval_open_date.begin(); week_it != conf.interval_open_date.end(); ++week_it) {
			if(tm_now.tm_wday == (int)*week_it) {
				return true;
			}
		}
	}
	return false;
}

bool Active_Manager::check_blank_set(const Active_Config &conf, Time_Value &now) {
	const Server_Maintainer_Cache server_maintainer = CONFIG_CACHE->server_maintainer_cache();
	switch(conf.blank_type) {
		case START_ON_START_ZERO: {//1.开服时间0点
			if(get_subdays(get_day_zero_time(server_maintainer.game_open_time), now) >= (int)conf.blank_days) {
				return true;
			}
			break;
		}
		case START_ON_MERGER_ZERO: {//2.合服时间0点
			if(server_maintainer.game_merge_time > server_maintainer.game_open_time &&
					get_subdays(get_day_zero_time(server_maintainer.game_merge_time), now) >= (int)conf.blank_days) {
				return true;
			}
			break;
		}
		case START_ON_START_TIME: {//3.开服时间
			if(server_maintainer.game_open_time.sec() < now.sec() - conf.blank_days*3600*24) {
				return true;
			}
			break;
		}
		case START_ON_MERGER_TIME: {//4.合服时间
			if(server_maintainer.game_merge_time > server_maintainer.game_open_time &&
					server_maintainer.game_merge_time.sec() < now.sec() - conf.blank_days*3600*24) {
				return true;
			}
			break;
		}
		case START_ON_SET_TIME: {//5.以自定时间开始
			tm tm_ = strtotime(conf.assign_blank_time);
			time_t t_ = mktime(&tm_);

			if(t_ <= now.sec()) {
				return true;
			}

			break;
		}
		case START_ON_START_NEXT_WEEK: {//6.以开服时间下星期
			Time_Value start_zero = get_day_zero_time(server_maintainer.game_open_time);
			Date_Time dt = Date_Time(server_maintainer.game_open_time);
			long week = dt.weekday();
			if(week == 0) {
				week = 7;
			}
			return  now.sec() >
			(start_zero.sec() + (8-week) * Time_Value::ONE_DAY_IN_SECS + conf.blank_days * Time_Value::ONE_WEEK_IN_SECS);
			break;
		}
	}
	return false;
}

Time_Value Active_Manager::get_blank_set_time(const Active_Config &conf) {
	const Server_Maintainer_Cache server_maintainer = CONFIG_CACHE->server_maintainer_cache();
		switch(conf.blank_type) {
			case START_ON_START_ZERO: {//1.开服时间0点
				Time_Value start_zero  = get_day_zero_time(server_maintainer.game_open_time);
				if(conf.blank_days) {
					start_zero.sec(start_zero.sec()+Time_Value::ONE_DAY_IN_SECS * conf.blank_days);
				}
				return start_zero;
				break;
			}
			case START_ON_MERGER_ZERO: {//2.合服时间0点
				return get_day_zero_time(server_maintainer.game_merge_time);
				break;
			}
			case START_ON_START_TIME: {//3.开服时间
				return server_maintainer.game_open_time;
				break;
			}
			case START_ON_MERGER_TIME: {//4.合服时间
				return server_maintainer.game_merge_time;
				break;
			}
			case START_ON_SET_TIME: {//5.以自定时间开始
				tm tm_ = strtotime(conf.assign_blank_time);
				time_t t = mktime(&tm_);
				Time_Value re_time = Time_Value::zero;
				re_time.sec(t);
				return re_time;
				break;
			}
			case START_ON_START_NEXT_WEEK: {//6.以开服时间下星期
				Time_Value start_zero = get_day_zero_time(server_maintainer.game_open_time);
				Date_Time dt = Date_Time(server_maintainer.game_open_time);
				long week = dt.weekday();
				if(week == 0) {
					week = 7;
				}
				return Time_Value(start_zero.sec() + (8-week) * Time_Value::ONE_DAY_IN_SECS + conf.blank_days * Time_Value::ONE_WEEK_IN_SECS);
				break;
			}
		}
		return Time_Value::zero;
}

tm Active_Manager::strtotime(const std::string &str, const std::string &format, uint8_t type) {
	tm tm_;
	char buf[128]= {0};
	strcpy(buf, str.c_str());
	strptime(buf, format.c_str(), &tm_); //将字符串转换为tm时间
	tm_.tm_isdst = -1;
	if(type == 0) {
		tm_.tm_hour = 0;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
	}
	return tm_;
}

int Active_Manager::get_subdays(const Time_Value &start, const Time_Value &end) {
	return (end.sec() - start.sec()) / Time_Value::ONE_DAY_IN_SECS;
}

Time_Value Active_Manager::get_day_zero_time(const Time_Value &date) {
	Date_Time date_(date);
	date_.hour(0);
	date_.minute(0);
	date_.second(0);
	return Time_Value(date_.time_sec());
}

void Active_Manager::todo_action_send_client_notice(const Switch_On_Active &active,const UInt_Vec &argv) {
	MSG_82200211 effec_msg;
	effec_msg.id = active.id;
	effec_msg.start_time = active.start_time.sec();
	effec_msg.type = ACTION_SEND_CLIENT_NOTICE;
	Block_Buffer effec_buf;
	effec_buf.make_message(effec_msg.msg_id);
	effec_msg.serialize(effec_buf);
	effec_buf.finish_message();
	//全服活动广播,由聊天服转发
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
	buf.write_int32(0);
	buf.copy(&effec_buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}

void Active_Manager::todo_action_open_active(const Switch_On_Active &active,const UInt_Vec &argv) {
	if(active.id >= 10000 && active.id < 20000) {//活动倒计时和开启活动预告通知 to client
		MSG_82200211 effec_msg;
		effec_msg.id = active.id;
		effec_msg.start_time = active.start_time.sec();
		effec_msg.type = ACTION_OPEN_ACTIVE;
		Block_Buffer effec_buf;
		effec_buf.make_message(effec_msg.msg_id);
		effec_msg.serialize(effec_buf);
		effec_buf.finish_message();
		//全服活动广播,由聊天服转发
		Block_Buffer buf;
		buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
		buf.write_int32(0);
		buf.copy(&effec_buf);
		buf.finish_message();
		LOGIC_MONITOR->send_to_local_chat(buf);
	}
//////////////////////////////////////////////////////////////////////////////////
	switch(active.id) {
		case ACTION_ACTIVE_TAME_AREA: {
			//开启竞技场
			//ARENA_MANAGER->open_activity(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_RECHARGE_REBATE: {
			RECHARGE_REBATE_MANAGER->open(active.start_time, active.end_time, active.id);
			break;
		}
		case ACTION_ACTIVE_RECHARGE_REBATE_DIY_TIME: {
			RECHARGE_REBATE_MANAGER->open(active.start_time, active.end_time, active.id);
			break;
		}
		case ACTION_ACTIVE_TWO_RECHARGE: {
			TWO_RECHARGE_MANAGER->open(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_FIRST_RECHARGE: {
			FIRST_RECHARGE_MANAGER->open(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_LIMIT_TIME_RECHARGE: {
			LIMIT_TIME_RECHARGE_MANAGER->open(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_LIMIT_BUY: {
			ACTIVE_CONTENT_MANAGER->open_limit_buy(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_VIP_CLUB: {	//VIP俱乐部
			ACTIVE_CONTENT_MANAGER->open_vip_club(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_LEVEL_ACE: {	//冲级活动
			ACTIVE_CONTENT_MANAGER->open_level_ace(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_SUIT_COLLECT: {	//套装收集
			ACTIVE_CONTENT_MANAGER->open_suit_collect(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_GUILD_LEVEL: {	//公会升级
			ACTIVE_CONTENT_MANAGER->open_guild_level(active.start_time, active.end_time);
			break;
		}

		case ACTION_ACTIVE_LEVEL_RANK: {	//等级排名
			if(argv.size()) {
				ACTIVE_CONTENT_MANAGER->open_level_rank(active.start_time, active.end_time, argv[0]);
			}
			break;
		}
		case ACTION_ACTIVE_ARENA_RANK: {	//单人竞技排名
			if(argv.size()) {
				ACTIVE_CONTENT_MANAGER->open_arena_rank(active.start_time, active.end_time, argv[0]);
			}
			break;
		}
		case ACTION_ACTIVE_FORCE_RANK: {	//战力排名
			if(argv.size()) {
				ACTIVE_CONTENT_MANAGER->open_force_rank(active.start_time, active.end_time, argv[0]);
			}
			break;
		}
//		case ACTION_ACTIVE_GEM_LEVELUP: {  //宝石升级
//			ACTIVE_CONTENT_MANAGER->open_gem_levelup(active.start_time, active.end_time);
//			break;
//		}
		case ACTION_ACTIVE_WING_LEVELUP: {	//翅膀升级
			ACTIVE_CONTENT_MANAGER->open_wing_levelup(active.start_time, active.end_time);
			break;
		}
//		case ACTION_ACTIVE_MOUNT_LEVELUP: {	//坐骑升级
//			ACTIVE_CONTENT_MANAGER->open_mount_levelup(active.start_time, active.end_time);
//			break;
//		}
		case ACTION_ACTIVE_THOR_LOTTERY: { //博彩系统（雷神宝藏）
			THORLOTTERY_MANAGER->activity_start(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_ANSWER_SYS: {//智慧试炼
			ANSWER_MANAGER->open_active(active.start_time, active.end_time, active.before_start_time, active.openLevel);
			break;
		}
		case ACTION_ACTIVE_WORLD_BOSS: {
			WORLD_BOSS_ACTIVE_MANAGER->listen_open_active(active.before_start_time, active.start_time, active.end_time);
			break;
		}
		case 10109:{// 荣誉竞技场
			HONOR_ARENA_ACTIVE_MANAGER->listen_open_active(active.before_start_time, active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_SG_RANK_PASS: {
			if(argv.size()) {
				ACTIVE_CONTENT_MANAGER->open_sg_rank(active.start_time, active.end_time, argv[0]);
			}
			break;
		}
		case ACTION_ACTIVE_MOUNT_RANK: {
			if(argv.size()) {
				ACTIVE_CONTENT_MANAGER->open_mount_rank(active.start_time, active.end_time, argv[0]);
			}
			break;
		}
		case ACTION_ACTIVE_WING_RANK: {
			if(argv.size()) {
				ACTIVE_CONTENT_MANAGER->open_wing_rank(active.start_time, active.end_time, argv[0]);
			}
			break;
		}
		case ACTION_ACTIVE_ACTIVITIES_STORES: {
			ACTIVITIES_STORES_MANAGER->open(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_NPC_RUNAWAY:{//怪物暴动(情人节)
			VALENTINE_MONSTER_ACTIVE_MANAGER->listen_open_active(active.before_start_time, active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_CONTENT_ARENA_DAN_TWO:
		case ACTION_ACTIVE_ARENA_DAN:  {
			ACTIVE_CONTENT_MANAGER->open_arena_dan(active.id,active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_OPER_ACTIVITY_2:
		case ACTION_ACTIVE_OPER_ACTIVITY: {
			OPER_ACTIVE_MANAGER->open(active.start_time, active.end_time);
			break;
		}
		default: {
			switch (active.type) {
				case ACTIVE_CONTENT_REWARD_ACT_TYPE_OPER_ACTIVITY: {
					OPER_ACTIVE_MANAGER->open(active.start_time, active.end_time);
					break;
				}
				case ACTIVE_CONTENT_REWARD_ACT_TYPE_OPER_ACTIVITY_S: {
					OPER_ACTIVE_MANAGER->open_item(active.id, active.start_time, active.end_time);
					break;
				}
				default: {
					ACTIVE_CONTENT_MANAGER->open_common(active.id, active.type, active.start_time, active.end_time);
				}
			}
			break;
		}
	}
}

void Active_Manager::todo_action_close_active(const Switch_On_Active &active,const UInt_Vec &argv) {
	if(active.id >= 10000 && active.id < 20000) {//活动倒计时和关闭活动预告通知 to client
		MSG_82200211 effec_msg;
		effec_msg.reset();
		effec_msg.id = active.id;
		//effec_msg.start_time = active.start_time.sec();
		effec_msg.type = ACTION_CLOSE_ACTIVE;
		Block_Buffer effec_buf;
		effec_buf.make_message(effec_msg.msg_id);
		effec_msg.serialize(effec_buf);
		effec_buf.finish_message();
		//全服活动广播,由聊天服转发
		Block_Buffer buf;
		buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
		buf.write_int32(0);
		buf.copy(&effec_buf);
		buf.finish_message();
		LOGIC_MONITOR->send_to_local_chat(buf);
	}
//////////////////////////////////////////////////////////////////////////////////
	switch(active.id) {
		case ACTION_ACTIVE_TAME_AREA: {
			//开启竞技场
			//ARENA_MANAGER->close_activity(active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_RECHARGE_REBATE_DIY_TIME:
		case ACTION_ACTIVE_RECHARGE_REBATE: {
			RECHARGE_REBATE_MANAGER->close(active.end_time);
			break;
		}
		case ACTION_ACTIVE_TWO_RECHARGE: {
			TWO_RECHARGE_MANAGER->close(active.end_time);
			break;
		}
		case ACTION_ACTIVE_FIRST_RECHARGE: {
			FIRST_RECHARGE_MANAGER->close(active.end_time);
			break;
		}
		case ACTION_ACTIVE_LIMIT_TIME_RECHARGE: {
			LIMIT_TIME_RECHARGE_MANAGER->close(active.end_time);
			break;
		}
		case ACTION_ACTIVE_LIMIT_BUY: {
			ACTIVE_CONTENT_MANAGER->close_limit_buy();
			break;
		}
		case ACTION_ACTIVE_VIP_CLUB: {	//VIP俱乐部
			ACTIVE_CONTENT_MANAGER->close_vip_club();
			break;
		}
		case ACTION_ACTIVE_LEVEL_ACE: {	//冲级活动
			ACTIVE_CONTENT_MANAGER->close_level_ace();
			break;
		}
		case ACTION_ACTIVE_SUIT_COLLECT: {	//套装收集
			ACTIVE_CONTENT_MANAGER->close_suit_collect();
			break;
		}
		case ACTION_ACTIVE_GUILD_LEVEL: {	//公会升级
			ACTIVE_CONTENT_MANAGER->close_guild_level();
			break;
		}

		case ACTION_ACTIVE_LEVEL_RANK: {	//等级排名
			ACTIVE_CONTENT_MANAGER->close_level_rank();
			break;
		}
		case ACTION_ACTIVE_ARENA_RANK: {	//单人竞技排名
			ACTIVE_CONTENT_MANAGER->close_arena_rank();
			break;
		}
		case ACTION_ACTIVE_FORCE_RANK: {	//战力排名
			ACTIVE_CONTENT_MANAGER->close_force_rank();
			break;
		}
//		case ACTION_ACTIVE_GEM_LEVELUP: {  //宝石升级
//			ACTIVE_CONTENT_MANAGER->close_gem_levelup();
//			break;
//		}
		case ACTION_ACTIVE_WING_LEVELUP: {  //翅膀升级
			ACTIVE_CONTENT_MANAGER->close_wing_levelup();
			break;
		}
//		case ACTION_ACTIVE_MOUNT_LEVELUP: {  //坐骑升级
//			ACTIVE_CONTENT_MANAGER->close_mount_levelup();
//			break;
//		}
		case ACTION_ACTIVE_THOR_LOTTERY: { //博彩系统（雷神宝藏）
			THORLOTTERY_MANAGER->activity_end(active.end_time);
			break;
		}
		case ACTION_ACTIVE_ANSWER_SYS: { //智慧试炼
			ANSWER_MANAGER->close_active(active.end_time);
			break;
		}
		case ACTION_ACTIVE_WORLD_BOSS: {
			WORLD_BOSS_ACTIVE_MANAGER->listen_close_active();
			break;
		}
		case 10109:{// 荣誉竞技场
			HONOR_ARENA_ACTIVE_MANAGER->listen_close_active();
			break;
		}
		case ACTION_ACTIVE_SG_RANK_PASS: {
			ACTIVE_CONTENT_MANAGER->close_sg_rank();
			break;
		}
		case ACTION_ACTIVE_MOUNT_RANK: {
			ACTIVE_CONTENT_MANAGER->close_mount_rank();
			break;
		}
		case ACTION_ACTIVE_WING_RANK: {
			ACTIVE_CONTENT_MANAGER->close_wing_rank();
			break;
		}
		case ACTION_ACTIVE_ACTIVITIES_STORES: {
			ACTIVITIES_STORES_MANAGER->end( active.end_time);
			break;
		}
		case ACTION_ACTIVE_NPC_RUNAWAY:{
			VALENTINE_MONSTER_ACTIVE_MANAGER->listen_close_active();
			break;
		}
		case ACTION_ACTIVE_CONTENT_ARENA_DAN_TWO:
		case ACTION_ACTIVE_ARENA_DAN: {
			ACTIVE_CONTENT_MANAGER->close_arena_dan();
			break;
		}
		case ACTION_ACTIVE_OPER_ACTIVITY_2:
		case ACTION_ACTIVE_OPER_ACTIVITY: {
			OPER_ACTIVE_MANAGER->close(active.end_time);
			break;
		}
		default: {
			switch (active.type) {
				case ACTIVE_CONTENT_REWARD_ACT_TYPE_OPER_ACTIVITY: {
					OPER_ACTIVE_MANAGER->open(active.start_time, active.end_time);
					break;
				}
				case ACTIVE_CONTENT_REWARD_ACT_TYPE_OPER_ACTIVITY_S: {
					OPER_ACTIVE_MANAGER->close_item(active.id, active.end_time);
					break;
				}
				default: {
					ACTIVE_CONTENT_MANAGER->close_common(active.id);
				}
			}
			break;
		}
	}
}

void Active_Manager::todo_open_active_icon(const Switch_On_Active &active,const UInt_Vec &argv) {
	int8_t act = 0;
	Time_Value over_time = Time_Value::zero;
	if(argv.size() >= 2) {
		act = argv[0];
		if(argv[1]) {
			over_time = active.end_time;
		} else {
			over_time = active.start_time;
		}
	}
	if(active.id >= 80000 && active.id < 90000) {
		//if(argv.size() >= 2) {
			ACTIVE_CONTENT_MANAGER->open_icon(active.icon, active.end_time, active.openLevel, active.id);
		//}
	}
	switch(active.id) {
		case ACTION_ACTIVE_RECHARGE_REBATE_DIY_TIME:
		case ACTION_ACTIVE_RECHARGE_REBATE: {
			//if(argv.size() >= 2) {
				RECHARGE_REBATE_MANAGER->open_icon(active.icon, active.end_time, active.openLevel);
			//}
			break;
		}
		case ACTION_ACTIVE_THOR_LOTTERY: { //博彩系统（雷神宝藏）
			//if(argv.size() >= 2) {
				ACTIVE_CONTENT_MANAGER->common_open_icon(active.icon, active.end_time, active.openLevel, over_time, act);
			//}
			break;
		}
		case ACTION_ACTIVE_ANSWER_SYS: {
			ACTIVE_CONTENT_MANAGER->common_open_icon(active.icon, active.end_time, active.openLevel, over_time, act);
			break;
		}
		case ACTION_ACTIVE_WORLD_BOSS: {
			ACTIVE_CONTENT_MANAGER->common_open_icon(active.icon, active.end_time, active.openLevel, over_time, act);
			break;
		}
		case 10109:{// 荣誉竞技场
			ACTIVE_CONTENT_MANAGER->common_open_icon(active.icon, active.end_time, active.openLevel, over_time, act);
			HONOR_ARENA_ACTIVE_MANAGER->listen_open_active_icon(active.before_start_time, active.start_time, active.end_time);
			break;
		}
		case ACTION_ACTIVE_DEFEND_DD: {
			ACTIVE_CONTENT_MANAGER->common_open_icon(active.icon, active.end_time, active.openLevel, over_time, act);
			break;
		}
		case ACTION_ACTIVE_TWO_RECHARGE: {
			ACTIVE_CONTENT_MANAGER->common_open_icon(active.icon, active.end_time, active.openLevel, over_time, act);
			TWO_RECHARGE_MANAGER->open_icon(active.icon, active.end_time, active.openLevel);
			break;
		}
		case ACTION_ACTIVE_FIRST_RECHARGE: {
			FIRST_RECHARGE_MANAGER->open_icon(active.icon, over_time, active.openLevel);
			break;
		}
		case ACTION_ACTIVE_LIMIT_TIME_RECHARGE: {
			LIMIT_TIME_RECHARGE_MANAGER->open_icon(active.icon, over_time, active.openLevel);
			break;
		}
		default : {
			if(active.icon) {
				ACTIVE_CONTENT_MANAGER->common_open_icon(active.icon, active.end_time, active.openLevel, over_time, act);
			}
		}
	}//end switch
}

void Active_Manager::todo_close_active_icon(const Switch_On_Active &active,const UInt_Vec &argv) {
	if(active.id >= 80000 && active.id < 90000) {
		//if(argv.size()) {
			ACTIVE_CONTENT_MANAGER->close_icon(active.icon, active.id);
		//}
	}
	switch(active.id) {
		case ACTION_ACTIVE_RECHARGE_REBATE_DIY_TIME:
		case ACTION_ACTIVE_RECHARGE_REBATE: {
			//if(argv.size() >= 2) {
				RECHARGE_REBATE_MANAGER->close_icon(active.icon);
			//}
			break;
		}
		case ACTION_ACTIVE_THOR_LOTTERY: { //博彩系统（雷神宝藏）
			//if(argv.size()) {
				ACTIVE_CONTENT_MANAGER->common_close_icon(active.icon);
			//}
			break;
		}
		case ACTION_ACTIVE_ANSWER_SYS: {
			ACTIVE_CONTENT_MANAGER->common_close_icon(active.icon);
			break;
		}
		case ACTION_ACTIVE_WORLD_BOSS: {
			ACTIVE_CONTENT_MANAGER->common_close_icon(active.icon);
			break;
		}
		case 10109:{// 荣誉竞技场
			ACTIVE_CONTENT_MANAGER->common_close_icon(active.icon);
			HONOR_ARENA_ACTIVE_MANAGER->listen_close_active_icon();
			break;
		}
		case ACTION_ACTIVE_DEFEND_DD: {
			ACTIVE_CONTENT_MANAGER->common_close_icon(active.icon);
			break;
		}
		case ACTION_ACTIVE_TWO_RECHARGE: {
			ACTIVE_CONTENT_MANAGER->common_close_icon(active.icon);
			TWO_RECHARGE_MANAGER->close_icon(active.icon);
			break;
		}
		case ACTION_ACTIVE_FIRST_RECHARGE: {
			FIRST_RECHARGE_MANAGER->close_icon(active.icon);
			break;
		}
		case ACTION_ACTIVE_LIMIT_TIME_RECHARGE: {
			LIMIT_TIME_RECHARGE_MANAGER->close_icon(active.icon);
			break;
		}
		default: {
			ACTIVE_CONTENT_MANAGER->common_close_icon(active.icon);
		}
	}
}

void Active_Manager::todo_action_count_rank_active(const Switch_On_Active &active,const UInt_Vec &argv) {
	switch(active.id) {
		case ACTION_ACTIVE_LEVEL_RANK: {	//等级排名
			ACTIVE_CONTENT_MANAGER->count_level_rank();
			break;
		}
		case ACTION_ACTIVE_ARENA_RANK: {	//单人竞技排名
			ACTIVE_CONTENT_MANAGER->count_arena_rank();
			break;
		}
		case ACTION_ACTIVE_FORCE_RANK: {	//战力排名
			ACTIVE_CONTENT_MANAGER->count_force_rank();
			break;
		}
		case ACTION_ACTIVE_SG_RANK_PASS: {	//上古副本
			ACTIVE_CONTENT_MANAGER->count_sg_rank();
			break;
		}
		case ACTION_ACTIVE_MOUNT_RANK: {	//
			ACTIVE_CONTENT_MANAGER->count_mount_rank();
			break;
		}
		case ACTION_ACTIVE_WING_RANK: {	//
			ACTIVE_CONTENT_MANAGER->count_wing_rank();
			break;
		}
	}
}


void Active_Manager::update_active_data(uint32_t id,const Time_Value &begin_time,const Time_Value &end_time) {
	Active_Data_Map::iterator iter = active_data_map.find(id);
	if(iter != active_data_map.end()) {
		++iter->second.num;
		iter->second.begin_time = begin_time;
		iter->second.end_time = end_time;
	} else {
		Active_Data_Stauts act_date;
		act_date.reset();
		act_date.id = id;
		act_date.num = 1;
		act_date.begin_time = begin_time;
		act_date.end_time = end_time;
		active_data_map.insert(std::make_pair(id, act_date));
	}
	is_change = true;
}

bool Active_Manager::check_loop_num(const Active_Config &conf) {
	Active_Data_Map::iterator iter = active_data_map.find(conf.id);
	if(iter != active_data_map.end() && conf.loop) {
		return iter->second.num < conf.loop;
	} else {
		return true;
	}
}

bool Active_Manager::check_over_time(const Time_Value &start_time, const Active_Config &conf, const Time_Value &now ) {
	if(conf.over_time) {
		if(now.sec() >= conf.over_time * Time_Value::ONE_MINUTE_IN_SECS + get_blank_set_time(conf).sec()) {
			return true;
		}
	}
	return false;//不过时或not过时
}

void Active_Manager::active_action(const Switch_On_Event &event, const Switch_On_Active &acitve) {
	//监听所有活动和活动信息 event//活动当次事件  active//活动全部信息
}

int Active_Manager::get_active_up_day(uint32_t id) {
	Switch_On_Active_Map::iterator iter = switch_on_map.find(id);
	if(iter != switch_on_map.end()) {
		return get_subdays(iter->second.start_time, Time_Value::gettimeofday()) + 1;
	}
	return 0;
}

bool Active_Manager::active_have_up(uint32_t id) {
	return switch_on_map.find(id) != switch_on_map.end();
}

int Active_Manager::get_active_add_num(uint32_t id) {
	int day = get_active_up_day(id);
	if(day) {
		return CONFIG_CACHE_ACTIVE_CONTENT->get_act_add_num_config_by_act_id_day(id, day);
	}
	return 0;
}

bool Active_Manager::special_active_delay(const Switch_On_Active &active) {
	switch(active.id) {
		case ACTION_ACTIVE_RECHARGE_REBATE_DIY_TIME: {
			if(active_have_up(ACTION_ACTIVE_RECHARGE_REBATE)) {
				return true;//需要延后
			}
			break;
		}
	}
	const Active_Conf_Map &conf = CONFIG_CACHE_ACTIVE->act_conf_map();
	Active_Conf_Map::const_iterator cit = conf.find(active.id);
	while(cit != conf.end() && cit->second.after_id) {
		if(active_have_up(cit->second.after_id)) {
			return true;//需要延后
		}
		cit = conf.find(cit->second.after_id);
	}
	return false;
}

bool Active_Manager::special_active_event_delay(const uint32_t event_type, const Switch_On_Active &active, const UInt_Vec &argv) {
	return false;
}
