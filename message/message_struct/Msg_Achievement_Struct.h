/*
* Generate by devtool
*/

#ifndef _MSG_ACHIEVEMENT_H_
#define _MSG_ACHIEVEMENT_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取成就列表
*/
struct MSG_10100800  {
	MSG_10100800(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取成就列表(返回)
*/
struct MSG_50100800 : public Base_Msg  {
	std::vector<Achievement_Detail_Msg> list;
	MSG_50100800(void) { reset(); };

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
				Achievement_Detail_Msg v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100800;
		list.clear();
	}
};

/*
获取成就奖励
*/
struct MSG_10100801  {
	int8_t type;
	int32_t id;
	MSG_10100801(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		id = 0;
	}
};

/*
获取成就奖励(返回)
*/
struct MSG_50100801 : public Base_Msg  {
	int32_t result;
	int8_t type;
	int32_t id;
	MSG_50100801(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int8(type);
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int8(type)  ||  r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100801;
		result = 0;
		type = 0;
		id = 0;
	}
};

/*
穿脱奖章
奖章身上：
40001 - 40008 普通勋章
40009 - 40012 特殊的勋章
40013 - 40014 稀有勋章
*/
struct MSG_10100802  {
	int32_t source_index;
	int32_t dst_index;
	MSG_10100802(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(source_index);
		w.write_int32(dst_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(source_index)  ||  r.read_int32(dst_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		source_index = 0;
		dst_index = 0;
	}
};

/*
穿脱奖章(返回)
*/
struct MSG_50100802 : public Base_Msg  {
	int8_t onoff;
	MSG_50100802(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(onoff);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(onoff)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100802;
		onoff = 0;
	}
};



#endif