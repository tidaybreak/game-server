/*
 * ThorLotteryManager.cpp
 *
 *  Created on: 2014年11月11日
 *      Author: root
 */

#include "ThorLottery_Manager.h"
#include "ThorLottery_Struct.h"
#include "ThorLottery_Config.h"
#include "DB_Operator.h"
#include "DB_Manager.h"
#include "DB_Def.h"
#include "Config_Cache.h"
#include "Config_Cache_ThorLottery.h"

#define MAX_LOG_SIZE 50
#define SAVE_TICK_TIME 300

void ThorLottery_Manager::reset(void) {

	active_end_time_ = Time_Value::zero;
	thorlottery_manager_detail_.reset();
	save_db_tick_time_ = Time_Value::zero;
}

void ThorLottery_Manager::start(void) {

	thorlottery_manager_detail_.load();
	//其它处理，检查活动是否还在启动中
	save_db_tick_time_ = Time_Value::gettimeofday() + Time_Value(SAVE_TICK_TIME);
}

void ThorLottery_Manager::tick(const Time_Value& now) {

	if (now < save_db_tick_time_) return;

	save_db_tick_time_ = now + Time_Value(SAVE_TICK_TIME);
	if (thorlottery_manager_detail_.is_change_) {
		thorlottery_manager_detail_.is_change_ = false;
		Block_Buffer buf;
		buf.make_message(MONGO_SAVE_THORLOTTERY_RECORD);
		thorlottery_manager_detail_.serialize(buf);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, 0);
	}
}

void ThorLottery_Manager::trigger_daily_six(const Time_Value& now) {

	thorlottery_manager_detail_.thorlottery_item_map_.clear();
	set_detail_change(true);
}

void ThorLottery_Manager::activity_start(const Time_Value &start_time, const Time_Value &end_time) {

	Time_Value now = Time_Value::gettimeofday();
	const Time_Value &open_time = CONFIG_CACHE->server_maintainer_cache().game_open_time;
	Time_Value open_time_day_start = Time_Value::zero;
	get_zero_time(open_time, open_time_day_start);

	int weeks = (now.sec() - open_time_day_start.sec()) / Time_Value::ONE_WEEK_IN_SECS;
	weeks = weeks + 1;

	MSG_DEBUG("ThorLottery activiy open, weeks =%d", weeks);

	const ThorLottery_Map &thorlottery_map = CONFIG_CACHE_THORLOTTERY->get_thorlottery_map();
	ThorLottery_Config const *cfg = 0;
	for (ThorLottery_Map::const_iterator it = thorlottery_map.begin();
			it != thorlottery_map.end(); ++it) {
		if (it->second.start_weeks > weeks || it->second.end_weeks < weeks) continue;
		cfg = &it->second;
		break;
	}

	if (0 == cfg) return;
	if (cfg->season_id == thorlottery_manager_detail_.season_id_) {
		thorlottery_manager_detail_.state_ = ThorLotteryState::STATE_ON;
	} else {
		thorlottery_manager_detail_.reset();
		thorlottery_manager_detail_.season_id_ = cfg->season_id;
		thorlottery_manager_detail_.state_ = ThorLotteryState::STATE_ON;
		thorlottery_manager_detail_.is_change_ = true;
	}
	active_end_time_ = end_time;
}

void ThorLottery_Manager::activity_end(const Time_Value& now) {

	reset();
	thorlottery_manager_detail_.is_change_ = true;
}

const ThorLottery_Manager_Detail& ThorLottery_Manager::get_thorlottery_manager_detail(void)const {

	return thorlottery_manager_detail_;
}

int ThorLottery_Manager::get_thorlottery_state(void) const {

	return thorlottery_manager_detail_.state_;
}

int ThorLottery_Manager::get_thorlottery_season_id(void) const {

	return thorlottery_manager_detail_.season_id_;
}

int ThorLottery_Manager::get_thorlottery_lv_limit(void) const {

	return thorlottery_manager_detail_.lv_limit;
}

int ThorLottery_Manager::get_thorlottery_end_time(const Time_Value &now) const {

	if (Time_Value::zero == active_end_time_) return 0;
	if (now >= active_end_time_) return 0;
	return (active_end_time_-now).sec();
}

void ThorLottery_Manager::add_thorlottery_item(int order) {

	ThorLottery_Item_Map::iterator find_it = thorlottery_manager_detail_.thorlottery_item_map_.find(order);
	if (thorlottery_manager_detail_.thorlottery_item_map_.end() == find_it) {
		ThorLottery_Item item;
		item.index_ = order;
		item.counter_ = 1;
		thorlottery_manager_detail_.thorlottery_item_map_[order] = item;
	} else {
		find_it->second.counter_ += 1;
	}
}

void ThorLottery_Manager::del_thorlottery_item(int order) {

	ThorLottery_Item_Map::iterator find_it = thorlottery_manager_detail_.thorlottery_item_map_.find(order);
	if (thorlottery_manager_detail_.thorlottery_item_map_.end() != find_it) {
		find_it->second.counter_ -= 1;
		if (find_it->second.counter_> 0) return;

		thorlottery_manager_detail_.thorlottery_item_map_.erase(find_it);
	}
}

void ThorLottery_Manager::add_thorlottery_log(role_id_t role_id, const std::string& name, int item_id, int amount, int state) {

	ThorLottery_Item_Log log;
	log.time_		= Time_Value::gettimeofday();
	log.role_name_	= name;
	log.role_id_ 	= role_id;
	log.item_id_	= item_id;
	log.item_num_	= amount;
	log.bind_state_ = state;

	Time_Value *the_first = 0;
	ThorLottery_Log_Vec::iterator the_first_iter;
	if (thorlottery_manager_detail_.lottery_log_vec_.size() >= MAX_LOG_SIZE) {
		for (ThorLottery_Log_Vec::iterator it = thorlottery_manager_detail_.lottery_log_vec_.begin();
				it != thorlottery_manager_detail_.lottery_log_vec_.end(); ++it) {
			if (0 == the_first) {
				the_first = &(it->time_);
				the_first_iter = it;
			} else {
				if (it->time_ >= (*the_first)) continue;
				the_first = &(it->time_);
				the_first_iter = it;
			}
		}
		thorlottery_manager_detail_.lottery_log_vec_.erase(the_first_iter);
	}
	thorlottery_manager_detail_.lottery_log_vec_.push_back(log);
}

void ThorLottery_Manager::set_detail_change(bool change) {

	thorlottery_manager_detail_.is_change_ = change;
}
