/*
 * ThorLottery_Struct.cpp
 *
 *  Created on: 2014年11月11日
 *      Author: root
 */

#include "ThorLottery_Struct.h"
#include "DB_Operator.h"

void ThorLottery_Item_Log::reset(void) {
	time_		= Time_Value::zero;
	role_name_ 	= "";
	role_id_	= 0;
	item_id_	= 0;
	item_num_ 	= 0;
	bind_state_ = 0;
}

void ThorLottery_Manager_Detail::reset(void) {
	is_change_	= false;
	season_id_ 	= 0;;
	state_ 		= 0;
	icon_id_	= 0;
	lv_limit	= 0;
	thorlottery_item_map_.clear();
	lottery_log_vec_.clear();
}

int ThorLottery_Manager_Detail::load() {
	return MONGO_OPERATOR->load_thorlottery_manager(*this);
}

int ThorLottery_Manager_Detail::save() {
	return MONGO_OPERATOR->save_thorlottery_manager(*this);
}

int ThorLottery_Manager_Detail::serialize(Block_Buffer &buffer) const {

	buffer.write_int32(season_id_);
	buffer.write_int32(state_);
	buffer.write_int32(icon_id_);
	buffer.write_int32(lv_limit);
	uint size = thorlottery_item_map_.size();
	buffer.write_uint32(size);
	for (ThorLottery_Item_Map::const_iterator it = thorlottery_item_map_.begin();
			it != thorlottery_item_map_.end(); ++it) {
		buffer.write_int32(it->second.index_);
		buffer.write_int32(it->second.counter_);
	}
	size = thorlottery_item_map_.size();
	buffer.write_uint32(size);
	for (ThorLottery_Log_Vec::const_iterator it = lottery_log_vec_.begin();
			it != lottery_log_vec_.end(); ++it) {
		int64_t time = it->time_.sec() * 1000 + it->time_.usec() / 1000;
		buffer.write_int64(time);
		buffer.write_string(it->role_name_);
		buffer.write_int64(it->role_id_);
		buffer.write_int32(it->item_id_);
		buffer.write_int32(it->item_num_);
		buffer.write_int32(it->bind_state_);
	}

	return 0;
}

int ThorLottery_Manager_Detail::deserialize(Block_Buffer &buffer) {

	 buffer.read_int32(season_id_);
	 buffer.read_int32(state_);
	 buffer.read_int32(icon_id_);
	 buffer.read_int32(lv_limit);
	 uint size = 0;
	 buffer.read_uint32(size);
	 ThorLottery_Item item;
	 for (uint i=0; i < size; ++i) {
		 item.reset();
		 buffer.read_int32(item.index_);
		 buffer.read_int32(item.counter_);
		 thorlottery_item_map_[item.index_] = item;
	 }
	 ThorLottery_Item_Log log;
	 buffer.read_uint32(size);
	 for (uint i=0; i < size; ++i) {
		 log.reset();
		 int64_t time = 0;
		 buffer.read_int64(time);
		 log.time_ = Time_Value(time / 1000, time % 1000 * 1000);
		 buffer.read_string(log.role_name_);
		 buffer.read_int64(log.role_id_);
		 buffer.read_int32(log.item_id_);
		 buffer.read_int32(log.item_num_);
		 buffer.read_int32(log.bind_state_);
		 lottery_log_vec_.push_back(log);
	 }

	return 0;
}

void ThorLottery_Item::reset(void) {
	index_ 			= 0;
	counter_		= 0;
}

void ThorLottery_Detail::reset(void) {
	role_id							= 0;
	season_id 					= 0;
	score							= 0;
	daily_lottery_times_ 	= 0;
	lottery_time_ 	= Time_Value::zero;
	thorlottery_item_map_.clear();
}

int ThorLottery_Detail::load() {
	return MONGO_OPERATOR->load_thorlottery(*this);
}

int ThorLottery_Detail::save(void) {
	return MONGO_OPERATOR->save_thorlottery(*this);
}

int ThorLottery_Detail::serialize(Block_Buffer &buffer) const {
	return 0;
}

int ThorLottery_Detail::deserialize(Block_Buffer &buffer) {
	return 0;
}
