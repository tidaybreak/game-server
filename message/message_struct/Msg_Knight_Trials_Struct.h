/*
* Generate by devtool
*/

#ifndef _MSG_KNIGHT_TRIALS_H_
#define _MSG_KNIGHT_TRIALS_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求打开骑士试炼界面(返回)
*/
struct MSG_50107100 : public Base_Msg  {
	std::vector<Knight_Trials_Role_Info> info;
	uint8_t points;
	uint8_t receive;
	uint8_t reset_status;
	std::vector<Uint_Uint> reward;
	MSG_50107100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

		w.write_uint8(points);
		w.write_uint8(receive);
		w.write_uint8(reset_status);
		uint16_t __reward_vec_size = reward.size();
		w.write_uint16(__reward_vec_size);
		for(uint16_t i = 0; i < __reward_vec_size; ++i) {
				reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Knight_Trials_Role_Info v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		if( r.read_uint8(points)  ||  r.read_uint8(receive)  ||  r.read_uint8(reset_status)  )
				return -1;
 		uint16_t __reward_vec_size;
		if(r.read_uint16(__reward_vec_size)  )
				return -1;
		reward.reserve(__reward_vec_size);
		for(uint16_t i = 0; i < __reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50107100;
		info.clear();
		points = 0;
		receive = 0;
		reset_status = 0;
		reward.clear();
	}
};

/*
请求打开骑士试炼界面
*/
struct MSG_10107100  {
	MSG_10107100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求进入战斗
*/
struct MSG_10107101  {
	MSG_10107101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求战斗结果(返回)
*/
struct MSG_50107101 : public Base_Msg  {
	uint8_t battle_result;
	MSG_50107101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(battle_result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(battle_result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50107101;
		battle_result = 0;
	}
};

/*
请求打开宝箱
*/
struct MSG_10107102  {
	MSG_10107102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开宝箱(返回)
*/
struct MSG_50107102 : public Base_Msg  {
	std::vector<Uint_Uint> reward;
	MSG_50107102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __reward_vec_size = reward.size();
		w.write_uint16(__reward_vec_size);
		for(uint16_t i = 0; i < __reward_vec_size; ++i) {
				reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __reward_vec_size;
		if(r.read_uint16(__reward_vec_size)  )
				return -1;
		reward.reserve(__reward_vec_size);
		for(uint16_t i = 0; i < __reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50107102;
		reward.clear();
	}
};

/*
请求领取宝箱奖励
*/
struct MSG_10107103  {
	MSG_10107103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取宝箱奖励(返回)
*/
struct MSG_50107103 : public Base_Msg  {
	std::vector<Uint_Uint> receive_reward;
	std::vector<Uint_Uint> total_reward;
	MSG_50107103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __receive_reward_vec_size = receive_reward.size();
		w.write_uint16(__receive_reward_vec_size);
		for(uint16_t i = 0; i < __receive_reward_vec_size; ++i) {
				receive_reward[i].serialize(w);
		}

		uint16_t __total_reward_vec_size = total_reward.size();
		w.write_uint16(__total_reward_vec_size);
		for(uint16_t i = 0; i < __total_reward_vec_size; ++i) {
				total_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __receive_reward_vec_size;
		if(r.read_uint16(__receive_reward_vec_size)  )
				return -1;
		receive_reward.reserve(__receive_reward_vec_size);
		for(uint16_t i = 0; i < __receive_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				receive_reward.push_back(v);
		}

		uint16_t __total_reward_vec_size;
		if(r.read_uint16(__total_reward_vec_size)  )
				return -1;
		total_reward.reserve(__total_reward_vec_size);
		for(uint16_t i = 0; i < __total_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				total_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50107103;
		receive_reward.clear();
		total_reward.clear();
	}
};

/*
请求重置骑士试炼挑战
*/
struct MSG_10107104  {
	MSG_10107104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求重置骑士试炼挑战(返回)
*/
struct MSG_50107104 : public Base_Msg  {
	MSG_50107104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50107104;
	}
};



#endif