/*
* Generate by devtool
*/

#ifndef _MSG_AUCTION_H_
#define _MSG_AUCTION_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求拍卖行商品列表
*/
struct MSG_10107000  {
	MSG_10107000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求拍卖行商品列表(返回)
*/
struct MSG_50107000 : public Base_Msg  {
	std::vector<Auction_Goods_Info> list;
	uint32_t num;
	MSG_50107000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Auction_Goods_Info v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		if( r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50107000;
		list.clear();
		num = 0;
	}
};

/*
请求增加拍卖商品
*/
struct MSG_10107001  {
	uint32_t item_id;
	uint32_t num;
	uint32_t continue_time;
	uint32_t auction_price;
	uint32_t ones_price;
	uint8_t open_ann;
	MSG_10107001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(item_id);
		w.write_uint32(num);
		w.write_uint32(continue_time);
		w.write_uint32(auction_price);
		w.write_uint32(ones_price);
		w.write_uint8(open_ann);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(item_id)  ||  r.read_uint32(num)  ||  r.read_uint32(continue_time)  ||  r.read_uint32(auction_price)  ||  r.read_uint32(ones_price)  ||  r.read_uint8(open_ann)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
		num = 0;
		continue_time = 0;
		auction_price = 0;
		ones_price = 0;
		open_ann = 0;
	}
};

/*
请求增加拍卖商品(返回)
*/
struct MSG_50107001 : public Base_Msg  {
	Auction_Goods_Info goods;
	MSG_50107001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		goods.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( goods.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50107001;
		goods.reset();
	}
};

/*
请求下架竞拍商品
*/
struct MSG_10107002  {
	uint32_t id;
	MSG_10107002(void) { reset(); };

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
请求下架竞拍商品(返回)
*/
struct MSG_50107002 : public Base_Msg  {
	uint32_t id;
	MSG_50107002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50107002;
		id = 0;
	}
};

/*
请求一口价购买商品
*/
struct MSG_10107003  {
	uint32_t id;
	MSG_10107003(void) { reset(); };

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
请求一口价购买商品(返回)
*/
struct MSG_50107003 : public Base_Msg  {
	uint32_t id;
	uint32_t item_id;
	MSG_50107003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50107003;
		id = 0;
		item_id = 0;
	}
};

/*
请求出价竞拍商品
*/
struct MSG_10107004  {
	uint32_t id;
	uint32_t price;
	MSG_10107004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(price);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(price)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		price = 0;
	}
};

/*
请求出价竞拍商品(返回)
*/
struct MSG_50107004 : public Base_Msg  {
	uint32_t id;
	uint32_t price;
	uint32_t item_id;
	MSG_50107004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(price);
		w.write_uint32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(price)  ||  r.read_uint32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50107004;
		id = 0;
		price = 0;
		item_id = 0;
	}
};



#endif