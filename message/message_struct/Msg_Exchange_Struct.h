/*
* Generate by devtool
*/

#ifndef _MSG_EXCHANGE_H_
#define _MSG_EXCHANGE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
物品兑换
*/
struct MSG_10100500  {
	int16_t type;
	int32_t item_id;
	int32_t amount;
	MSG_10100500(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(type);
		w.write_int32(item_id);
		w.write_int32(amount);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(type)  ||  r.read_int32(item_id)  ||  r.read_int32(amount)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		item_id = 0;
		amount = 0;
	}
};

/*
获取兑换物品信息
*/
struct MSG_10100501  {
	int16_t type;
	MSG_10100501(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
获取兑换物品信息(返回)
*/
struct MSG_50100501 : public Base_Msg  {
	std::vector<Exchange_Info_Msg> exchange_item_vector;
	int16_t type;
	MSG_50100501(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __exchange_item_vector_vec_size = exchange_item_vector.size();
		w.write_uint16(__exchange_item_vector_vec_size);
		for(uint16_t i = 0; i < __exchange_item_vector_vec_size; ++i) {
				exchange_item_vector[i].serialize(w);
		}

		w.write_int16(type);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __exchange_item_vector_vec_size;
		if(r.read_uint16(__exchange_item_vector_vec_size)  )
				return -1;
		exchange_item_vector.reserve(__exchange_item_vector_vec_size);
		for(uint16_t i = 0; i < __exchange_item_vector_vec_size; ++i) {
				Exchange_Info_Msg v;
				if(v.deserialize(r))
						return -1;
				exchange_item_vector.push_back(v);
		}

		if( r.read_int16(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100501;
		exchange_item_vector.clear();
		type = 0;
	}
};

/*
物品兑换(返回)
*/
struct MSG_50100500 : public Base_Msg  {
	int32_t item_id;
	int32_t amount;
	MSG_50100500(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(amount);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(amount)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100500;
		item_id = 0;
		amount = 0;
	}
};

/*
技能消耗品兑换
*/
struct MSG_10100502  {
	int32_t id;
	MSG_10100502(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
技能消耗品兑换(返回）
*/
struct MSG_50100502 : public Base_Msg  {
	int32_t result;
	MSG_50100502(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100502;
		result = 0;
	}
};



#endif