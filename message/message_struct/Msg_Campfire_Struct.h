/*
* Generate by devtool
*/

#ifndef _MSG_CAMPFIRE_H_
#define _MSG_CAMPFIRE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求进入篝火晚会
*/
struct MSG_10100700  {
	MSG_10100700(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求添加篝火柴火
*/
struct MSG_10200701  {
	int64_t role_id;
	MSG_10200701(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
请求点燃篝火
*/
struct MSG_10200702  {
	int64_t role_id;
	MSG_10200702(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
请求篝火排名
*/
struct MSG_10200703  {
	MSG_10200703(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求篝火排名（返回）
*/
struct MSG_50200703 : public Base_Msg  {
	std::vector<campfire_rank_info> rank_info;
	int8_t rank;
	uint32_t time;
	MSG_50200703(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __rank_info_vec_size = rank_info.size();
		w.write_uint16(__rank_info_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_size; ++i) {
				rank_info[i].serialize(w);
		}

		w.write_int8(rank);
		w.write_uint32(time);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __rank_info_vec_size;
		if(r.read_uint16(__rank_info_vec_size)  )
				return -1;
		rank_info.reserve(__rank_info_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_size; ++i) {
				campfire_rank_info v;
				if(v.deserialize(r))
						return -1;
				rank_info.push_back(v);
		}

		if( r.read_int8(rank)  ||  r.read_uint32(time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200703;
		rank_info.clear();
		rank = 0;
		time = 0;
	}
};

/*
请求点燃/添柴奖励
备注：请求点燃/添柴奖励
*/
struct MSG_10200704  {
	MSG_10200704(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求点燃/添柴奖励(返回)
备注：请求点燃/添柴奖励(返回)
*/
struct MSG_50200704 : public Base_Msg  {
	int32_t result;
	MSG_50200704(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200704;
		result = 0;
	}
};

/*
请求篝火结算奖励
*/
struct MSG_10200705  {
	MSG_10200705(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求篝火结算奖励(返回)
备注：请求篝火结算奖励(返回)
*/
struct MSG_50200705 : public Base_Msg  {
	int32_t result;
	MSG_50200705(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200705;
		result = 0;
	}
};



#endif