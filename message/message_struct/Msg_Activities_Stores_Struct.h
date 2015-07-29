/*
* Generate by devtool
*/

#ifndef _MSG_ACTIVITIES_STORES_H_
#define _MSG_ACTIVITIES_STORES_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求打开活动兑换商店
*/
struct MSG_10106900  {
	MSG_10106900(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开活动兑换商店(返回)
*/
struct MSG_50106900 : public Base_Msg  {
	std::vector<Activity_Exchange_Mall_Goods_Info> goods;
	MSG_50106900(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __goods_vec_size = goods.size();
		w.write_uint16(__goods_vec_size);
		for(uint16_t i = 0; i < __goods_vec_size; ++i) {
				goods[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __goods_vec_size;
		if(r.read_uint16(__goods_vec_size)  )
				return -1;
		goods.reserve(__goods_vec_size);
		for(uint16_t i = 0; i < __goods_vec_size; ++i) {
				Activity_Exchange_Mall_Goods_Info v;
				if(v.deserialize(r))
						return -1;
				goods.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106900;
		goods.clear();
	}
};

/*
请求兑换活动兑换商城商品
*/
struct MSG_10106901  {
	uint32_t id;
	uint32_t num;
	MSG_10106901(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		num = 0;
	}
};

/*
请求兑换活动兑换商城商品(返回)
*/
struct MSG_50106901 : public Base_Msg  {
	uint32_t id;
	uint32_t num;
	MSG_50106901(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106901;
		id = 0;
		num = 0;
	}
};



#endif