/*
* Generate by devtool
*/

#ifndef _MSG_VIP_H_
#define _MSG_VIP_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求开通VIP
*/
struct MSG_10106200  {
	uint32_t id;
	MSG_10106200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求开通VIP(返回)
*/
struct MSG_50106200 : public Base_Msg  {
	uint32_t id;
	uint32_t vip_over_time;
	MSG_50106200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(vip_over_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(vip_over_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106200;
		id = 0;
		vip_over_time = 0;
	}
};

/*
请求世界之树信息
*/
struct MSG_10106201  {
	MSG_10106201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求世界之树信息(返回)
*/
struct MSG_50106201 : public Base_Msg  {
	std::vector<World_Tree_Flock> info;
	int32_t all_day;
	int32_t re_day;
	MSG_50106201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

		w.write_int32(all_day);
		w.write_int32(re_day);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				World_Tree_Flock v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		if( r.read_int32(all_day)  ||  r.read_int32(re_day)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106201;
		info.clear();
		all_day = 0;
		re_day = 0;
	}
};

/*
请求领取世界之树奖励
*/
struct MSG_10106202  {
	uint32_t id;
	MSG_10106202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取世界之树奖励(返回)
*/
struct MSG_50106202 : public Base_Msg  {
	uint32_t id;
	World_Tree_Flock tree_info;
	MSG_50106202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		tree_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  tree_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106202;
		id = 0;
		tree_info.reset();
	}
};

/*
请求打开VIP转盘获取物品组
*/
struct MSG_10106203  {
	MSG_10106203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开VIP转盘获取物品组(返回)
*/
struct MSG_50106203 : public Base_Msg  {
	std::vector<uint32_t> item_id;
	MSG_50106203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_id_vec_size = item_id.size();
		w.write_uint16(__item_id_vec_size);
		for(uint16_t i = 0; i < __item_id_vec_size; ++i) {
				w.write_uint32(item_id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __item_id_vec_size;
		if(r.read_uint16(__item_id_vec_size)  )
				return -1;
		item_id.reserve(__item_id_vec_size);
		for(uint16_t i = 0; i < __item_id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				item_id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106203;
		item_id.clear();
	}
};

/*
请求VIP转盘抽奖
*/
struct MSG_10106204  {
	uint8_t type;
	MSG_10106204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
请求VIP转盘抽奖(返回)
*/
struct MSG_50106204 : public Base_Msg  {
	std::vector<int32_t> id;
	MSG_50106204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __id_vec_size = id.size();
		w.write_uint16(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				w.write_int32(id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __id_vec_size;
		if(r.read_uint16(__id_vec_size)  )
				return -1;
		id.reserve(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106204;
		id.clear();
	}
};

/*
请求获取VIP转盘奖品
*/
struct MSG_10106205  {
	MSG_10106205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取VIP日礼包
*/
struct MSG_10106206  {
	MSG_10106206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取VIP日礼包(返回)
*/
struct MSG_50106206 : public Base_Msg  {
	MSG_50106206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50106206;
	}
};

/*
请求打开VIP日礼包
*/
struct MSG_10106207  {
	MSG_10106207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开VIP日礼包(返回)
*/
struct MSG_50106207 : public Base_Msg  {
	uint8_t rec;
	MSG_50106207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(rec);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(rec)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106207;
		rec = 0;
	}
};



#endif