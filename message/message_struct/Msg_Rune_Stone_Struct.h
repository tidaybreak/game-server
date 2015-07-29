/*
* Generate by devtool
*/

#ifndef _MSG_RUNE_STONE_H_
#define _MSG_RUNE_STONE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求符石系统信息
*/
struct MSG_10103200  {
	MSG_10103200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求符石系统信息（返回）
*/
struct MSG_50103200 : public Base_Msg  {
	std::vector<Smelter_Info> smelter_infos;
	int32_t points;
	int32_t remain_double_times;
	MSG_50103200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __smelter_infos_vec_size = smelter_infos.size();
		w.write_uint16(__smelter_infos_vec_size);
		for(uint16_t i = 0; i < __smelter_infos_vec_size; ++i) {
				smelter_infos[i].serialize(w);
		}

		w.write_int32(points);
		w.write_int32(remain_double_times);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __smelter_infos_vec_size;
		if(r.read_uint16(__smelter_infos_vec_size)  )
				return -1;
		smelter_infos.reserve(__smelter_infos_vec_size);
		for(uint16_t i = 0; i < __smelter_infos_vec_size; ++i) {
				Smelter_Info v;
				if(v.deserialize(r))
						return -1;
				smelter_infos.push_back(v);
		}

		if( r.read_int32(points)  ||  r.read_int32(remain_double_times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103200;
		smelter_infos.clear();
		points = 0;
		remain_double_times = 0;
	}
};

/*
符石熔炼
*/
struct MSG_10103201  {
	int8_t smelter_idx;
	int8_t is_bat;
	MSG_10103201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(smelter_idx);
		w.write_int8(is_bat);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(smelter_idx)  ||  r.read_int8(is_bat)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		smelter_idx = 0;
		is_bat = 0;
	}
};

/*
符石熔炼（返回）
*/
struct MSG_50103201 : public Base_Msg  {
	int8_t smelter_idx;
	int8_t smelter_idx_next;
	int8_t is_bat;
	int8_t result;
	int32_t item_id;
	int32_t get_copper;
	int32_t total_points;
	MSG_50103201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(smelter_idx);
		w.write_int8(smelter_idx_next);
		w.write_int8(is_bat);
		w.write_int8(result);
		w.write_int32(item_id);
		w.write_int32(get_copper);
		w.write_int32(total_points);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(smelter_idx)  ||  r.read_int8(smelter_idx_next)  ||  r.read_int8(is_bat)  ||  r.read_int8(result)  ||  r.read_int32(item_id)  ||  r.read_int32(get_copper)  ||  r.read_int32(total_points)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103201;
		smelter_idx = 0;
		smelter_idx_next = 0;
		is_bat = 0;
		result = 0;
		item_id = 0;
		get_copper = 0;
		total_points = 0;
	}
};

/*
符石出售
*/
struct MSG_10103202  {
	int8_t is_bat;
	int8_t under_color;
	int32_t index;
	MSG_10103202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int8(under_color);
		w.write_int32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int8(under_color)  ||  r.read_int32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_bat = 0;
		under_color = 0;
		index = 0;
	}
};

/*
符石出售（返回）
*/
struct MSG_50103202 : public Base_Msg  {
	int8_t is_bat;
	int8_t under_color;
	std::vector<int32_t> vec_index;
	int8_t result;
	MSG_50103202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int8(under_color);
		uint16_t __vec_index_vec_size = vec_index.size();
		w.write_uint16(__vec_index_vec_size);
		for(uint16_t i = 0; i < __vec_index_vec_size; ++i) {
				w.write_int32(vec_index[i]);
		}

		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int8(under_color)  )
				return -1;
 		uint16_t __vec_index_vec_size;
		if(r.read_uint16(__vec_index_vec_size)  )
				return -1;
		vec_index.reserve(__vec_index_vec_size);
		for(uint16_t i = 0; i < __vec_index_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				vec_index.push_back(v);
		}

		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103202;
		is_bat = 0;
		under_color = 0;
		vec_index.clear();
		result = 0;
	}
};

/*
符石拾取
*/
struct MSG_10103203  {
	int8_t is_bat;
	int32_t index;
	MSG_10103203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_bat = 0;
		index = 0;
	}
};

/*
符石拾取（返回）
*/
struct MSG_50103203 : public Base_Msg  {
	int8_t is_bat;
	int32_t index;
	int8_t result;
	MSG_50103203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int32(index);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int32(index)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103203;
		is_bat = 0;
		index = 0;
		result = 0;
	}
};

/*
符石锁定
*/
struct MSG_10103204  {
	int32_t index;
	int8_t is_lock;
	MSG_10103204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_int8(is_lock);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_int8(is_lock)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		is_lock = 0;
	}
};

/*
符石锁定（返回）
*/
struct MSG_50103204 : public Base_Msg  {
	int32_t index;
	int32_t result;
	int8_t is_lock;
	MSG_50103204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_int32(result);
		w.write_int8(is_lock);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_int32(result)  ||  r.read_int8(is_lock)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103204;
		index = 0;
		result = 0;
		is_lock = 0;
	}
};

/*
符石合并
*/
struct MSG_10103205  {
	int8_t is_bat;
	int32_t index_type;
	int32_t index_from;
	int32_t index_to;
	MSG_10103205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int32(index_type);
		w.write_int32(index_from);
		w.write_int32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int32(index_type)  ||  r.read_int32(index_from)  ||  r.read_int32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_bat = 0;
		index_type = 0;
		index_from = 0;
		index_to = 0;
	}
};

/*
符石合并（返回）
*/
struct MSG_50103205 : public Base_Msg  {
	int8_t is_bat;
	int32_t index_from;
	int32_t index_to;
	int8_t result;
	MSG_50103205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int32(index_from);
		w.write_int32(index_to);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int32(index_from)  ||  r.read_int32(index_to)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103205;
		is_bat = 0;
		index_from = 0;
		index_to = 0;
		result = 0;
	}
};

/*
符石移动(背包、仓库间移动，不包括拾取、穿脱)
*/
struct MSG_10103206  {
	int32_t index_from;
	int32_t index_to;
	MSG_10103206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index_from);
		w.write_int32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index_from)  ||  r.read_int32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index_from = 0;
		index_to = 0;
	}
};

/*
符石移动(身上、背包、仓库间移动，不包括拾取)（返回）
*/
struct MSG_50103206 : public Base_Msg  {
	int32_t index_from;
	int32_t index_type_to;
	int8_t result;
	MSG_50103206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index_from);
		w.write_int32(index_type_to);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index_from)  ||  r.read_int32(index_type_to)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103206;
		index_from = 0;
		index_type_to = 0;
		result = 0;
	}
};

/*
符石兑换
*/
struct MSG_10103207  {
	int32_t item_id;
	MSG_10103207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
	}
};

/*
符石兑换(返回)
*/
struct MSG_50103207 : public Base_Msg  {
	int32_t item_id;
	int32_t result;
	MSG_50103207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103207;
		item_id = 0;
		result = 0;
	}
};

/*
请求积分
备注：
以这条主动消息返回：
81080000	ACTIVE_POINTS	主动更新积分
*/
struct MSG_10103208  {
	MSG_10103208(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
符石穿脱
*/
struct MSG_10103209  {
	int32_t index_from;
	int32_t index_to;
	MSG_10103209(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index_from);
		w.write_int32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index_from)  ||  r.read_int32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index_from = 0;
		index_to = 0;
	}
};

/*
符石穿脱(返回)
*/
struct MSG_50103209 : public Base_Msg  {
	int32_t index_from;
	int32_t index_to;
	int8_t result;
	MSG_50103209(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index_from);
		w.write_int32(index_to);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index_from)  ||  r.read_int32(index_to)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103209;
		index_from = 0;
		index_to = 0;
		result = 0;
	}
};

/*
请求精华
备注：
以这条主动消息返回：
81090000	ACTIVE_ESSENCE	主动更新精华
*/
struct MSG_10103210  {
	MSG_10103210(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
符石精华兑换
*/
struct MSG_10103211  {
	int32_t item_id;
	MSG_10103211(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
	}
};

/*
符石精华兑换(返回)
*/
struct MSG_50103211 : public Base_Msg  {
	int32_t item_id;
	int32_t result;
	MSG_50103211(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103211;
		item_id = 0;
		result = 0;
	}
};



#endif