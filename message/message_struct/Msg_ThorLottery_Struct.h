/*
* Generate by devtool
*/

#ifndef _MSG_THORLOTTERY_H_
#define _MSG_THORLOTTERY_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
打开面板获取季数
*/
struct MSG_10108001  {
	MSG_10108001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
打开面板获取季数(返回)
*/
struct MSG_50108001 : public Base_Msg  {
	int32_t season_id;
	std::vector<ThorLottery_Log> log_vec;
	int32_t score;
	int32_t daily_lottery_times;
	int32_t time_left;
	MSG_50108001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(season_id);
		uint16_t __log_vec_vec_size = log_vec.size();
		w.write_uint16(__log_vec_vec_size);
		for(uint16_t i = 0; i < __log_vec_vec_size; ++i) {
				log_vec[i].serialize(w);
		}

		w.write_int32(score);
		w.write_int32(daily_lottery_times);
		w.write_int32(time_left);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(season_id)  )
				return -1;
 		uint16_t __log_vec_vec_size;
		if(r.read_uint16(__log_vec_vec_size)  )
				return -1;
		log_vec.reserve(__log_vec_vec_size);
		for(uint16_t i = 0; i < __log_vec_vec_size; ++i) {
				ThorLottery_Log v;
				if(v.deserialize(r))
						return -1;
				log_vec.push_back(v);
		}

		if( r.read_int32(score)  ||  r.read_int32(daily_lottery_times)  ||  r.read_int32(time_left)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50108001;
		season_id = 0;
		log_vec.clear();
		score = 0;
		daily_lottery_times = 0;
		time_left = 0;
	}
};

/*
雷神之锤抽奖
*/
struct MSG_10108002  {
	int32_t times;
	MSG_10108002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(times);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		times = 0;
	}
};

/*
雷神之锤抽奖(返回)
*/
struct MSG_50108002 : public Base_Msg  {
	std::vector<ThorLottery_Reward_Item> item_vec;
	std::vector<ThorLottery_Log> log_vec;
	int32_t score;
	int32_t daily_lottery_times;
	MSG_50108002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_vec_vec_size = item_vec.size();
		w.write_uint16(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				item_vec[i].serialize(w);
		}

		uint16_t __log_vec_vec_size = log_vec.size();
		w.write_uint16(__log_vec_vec_size);
		for(uint16_t i = 0; i < __log_vec_vec_size; ++i) {
				log_vec[i].serialize(w);
		}

		w.write_int32(score);
		w.write_int32(daily_lottery_times);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __item_vec_vec_size;
		if(r.read_uint16(__item_vec_vec_size)  )
				return -1;
		item_vec.reserve(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				ThorLottery_Reward_Item v;
				if(v.deserialize(r))
						return -1;
				item_vec.push_back(v);
		}

		uint16_t __log_vec_vec_size;
		if(r.read_uint16(__log_vec_vec_size)  )
				return -1;
		log_vec.reserve(__log_vec_vec_size);
		for(uint16_t i = 0; i < __log_vec_vec_size; ++i) {
				ThorLottery_Log v;
				if(v.deserialize(r))
						return -1;
				log_vec.push_back(v);
		}

		if( r.read_int32(score)  ||  r.read_int32(daily_lottery_times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50108002;
		item_vec.clear();
		log_vec.clear();
		score = 0;
		daily_lottery_times = 0;
	}
};

/*
雷神之锤兑换奖品
*/
struct MSG_10108003  {
	int32_t item_index;
	int32_t item_id;
	MSG_10108003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_index);
		w.write_int32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_index)  ||  r.read_int32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_index = 0;
		item_id = 0;
	}
};

/*
雷神之锤兑换奖品(返回)
*/
struct MSG_50108003 : public Base_Msg  {
	int32_t score;
	MSG_50108003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(score);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(score)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50108003;
		score = 0;
	}
};



#endif