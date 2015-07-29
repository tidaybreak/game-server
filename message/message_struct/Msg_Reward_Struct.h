/*
* Generate by devtool
*/

#ifndef _MSG_REWARD_H_
#define _MSG_REWARD_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求获取在线礼包
*/
struct MSG_10106300  {
	MSG_10106300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求7日登陆奖励信息
*/
struct MSG_10106301  {
	MSG_10106301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取7日登陆奖励
*/
struct MSG_10106302  {
	uint32_t days;
	MSG_10106302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(days);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(days)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		days = 0;
	}
};

/*
请求7日登陆奖励信息(返回)
*/
struct MSG_50106301 : public Base_Msg  {
	std::vector<Uint_Uint> status;
	MSG_50106301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __status_vec_size = status.size();
		w.write_uint16(__status_vec_size);
		for(uint16_t i = 0; i < __status_vec_size; ++i) {
				status[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __status_vec_size;
		if(r.read_uint16(__status_vec_size)  )
				return -1;
		status.reserve(__status_vec_size);
		for(uint16_t i = 0; i < __status_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				status.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106301;
		status.clear();
	}
};

/*
请求领取7日登陆奖励(返回)
*/
struct MSG_50106302 : public Base_Msg  {
	uint32_t days;
	MSG_50106302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(days);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(days)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106302;
		days = 0;
	}
};

/*
请求领取激活码
*/
struct MSG_10106303  {
	std::string code;
	MSG_10106303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(code);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(code)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		code.clear();
	}
};

/*
请求领取激活码(返回)
*/
struct MSG_50106303 : public Base_Msg  {
	MSG_50106303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50106303;
	}
};

/*
请求奖励追回信息
*/
struct MSG_10106304  {
	MSG_10106304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求奖励追回信息(返回)
*/
struct MSG_50106304 : public Base_Msg  {
	std::vector<Reward_Back_Info> list;
	MSG_50106304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Reward_Back_Info v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106304;
		list.clear();
	}
};

/*
请求追回奖励
*/
struct MSG_10106305  {
	uint32_t type;
	uint32_t item_id;
	MSG_10106305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(type);
		w.write_uint32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(type)  ||  r.read_uint32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		item_id = 0;
	}
};

/*
请求追回奖励(返回)
*/
struct MSG_50106305 : public Base_Msg  {
	uint32_t type;
	uint32_t item_id;
	std::vector<Uint_Uint> money_info;
	std::vector<Uint_Uint> item_info;
	uint32_t exp;
	MSG_50106305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(type);
		w.write_uint32(item_id);
		uint16_t __money_info_vec_size = money_info.size();
		w.write_uint16(__money_info_vec_size);
		for(uint16_t i = 0; i < __money_info_vec_size; ++i) {
				money_info[i].serialize(w);
		}

		uint16_t __item_info_vec_size = item_info.size();
		w.write_uint16(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				item_info[i].serialize(w);
		}

		w.write_uint32(exp);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(type)  ||  r.read_uint32(item_id)  )
				return -1;
 		uint16_t __money_info_vec_size;
		if(r.read_uint16(__money_info_vec_size)  )
				return -1;
		money_info.reserve(__money_info_vec_size);
		for(uint16_t i = 0; i < __money_info_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				money_info.push_back(v);
		}

		uint16_t __item_info_vec_size;
		if(r.read_uint16(__item_info_vec_size)  )
				return -1;
		item_info.reserve(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				item_info.push_back(v);
		}

		if( r.read_uint32(exp)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106305;
		type = 0;
		item_id = 0;
		money_info.clear();
		item_info.clear();
		exp = 0;
	}
};

/*
请求是否领取过收藏有礼
*/
struct MSG_10106306  {
	MSG_10106306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求是否领取过收藏有礼(返回)
*/
struct MSG_50106306 : public Base_Msg  {
	uint8_t keep_reward_status;
	MSG_50106306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(keep_reward_status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(keep_reward_status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106306;
		keep_reward_status = 0;
	}
};

/*
请求收藏添加成功
*/
struct MSG_10106307  {
	MSG_10106307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取收藏有礼
*/
struct MSG_10106308  {
	MSG_10106308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取收藏有礼(返回)
*/
struct MSG_50106308 : public Base_Msg  {
	MSG_50106308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50106308;
	}
};

/*
请求奖励系统TIP数
*/
struct MSG_10106309  {
	MSG_10106309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求奖励系统TIP数(返回)
*/
struct MSG_50106309 : public Base_Msg  {
	uint32_t num;
	MSG_50106309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106309;
		num = 0;
	}
};



#endif