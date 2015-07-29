/*
* Generate by devtool
*/

#ifndef _MSG_BATTLE_SCENE_H_
#define _MSG_BATTLE_SCENE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求战场排名信息
*/
struct MSG_10211001  {
	MSG_10211001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求战场排名信息返回
*/
struct MSG_50211001 : public Base_Msg  {
	std::vector<War_Player_Rank_Info> rank_vec;
	MSG_50211001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __rank_vec_vec_size = rank_vec.size();
		w.write_uint16(__rank_vec_vec_size);
		for(uint16_t i = 0; i < __rank_vec_vec_size; ++i) {
				rank_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __rank_vec_vec_size;
		if(r.read_uint16(__rank_vec_vec_size)  )
				return -1;
		rank_vec.reserve(__rank_vec_vec_size);
		for(uint16_t i = 0; i < __rank_vec_vec_size; ++i) {
				War_Player_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				rank_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50211001;
		rank_vec.clear();
	}
};

/*
请求战场治疗
*/
struct MSG_10211002  {
	MSG_10211002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求加入战场
*/
struct MSG_10111000  {
	MSG_10111000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求战场进入次数
*/
struct MSG_10111001  {
	MSG_10111001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求战场进入次数返回
*/
struct MSG_50111001 : public Base_Msg  {
	uint8_t remain_num;
	uint8_t max_num;
	int16_t income_rate;
	uint16_t total_merit;
	uint16_t max_merit;
	MSG_50111001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(remain_num);
		w.write_uint8(max_num);
		w.write_int16(income_rate);
		w.write_uint16(total_merit);
		w.write_uint16(max_merit);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(remain_num)  ||  r.read_uint8(max_num)  ||  r.read_int16(income_rate)  ||  r.read_uint16(total_merit)  ||  r.read_uint16(max_merit)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50111001;
		remain_num = 0;
		max_num = 0;
		income_rate = 0;
		total_merit = 0;
		max_merit = 0;
	}
};

/*
请求战场领取结算奖励
*/
struct MSG_50111003 : public Base_Msg  {
	int8_t result;
	MSG_50111003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50111003;
		result = 0;
	}
};

/*
请求领取战场结算奖励
*/
struct MSG_10111003  {
	MSG_10111003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
领取战场领取额外奖励通知
*/
struct MSG_50111004 : public Base_Msg  {
	int8_t result;
	uint8_t rank;
	uint16_t kill_num;
	uint16_t coe;
	MSG_50111004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_uint8(rank);
		w.write_uint16(kill_num);
		w.write_uint16(coe);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_uint8(rank)  ||  r.read_uint16(kill_num)  ||  r.read_uint16(coe)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50111004;
		result = 0;
		rank = 0;
		kill_num = 0;
		coe = 0;
	}
};



#endif