/*
* Generate by devtool
*/

#ifndef _MSG_CARD_H_
#define _MSG_CARD_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
卡牌镶嵌
*/
struct MSG_10109002  {
	uint32_t from_index;
	uint32_t to_index;
	uint8_t loc;
	uint32_t card_id;
	MSG_10109002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(from_index);
		w.write_uint32(to_index);
		w.write_uint8(loc);
		w.write_uint32(card_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(from_index)  ||  r.read_uint32(to_index)  ||  r.read_uint8(loc)  ||  r.read_uint32(card_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		from_index = 0;
		to_index = 0;
		loc = 0;
		card_id = 0;
	}
};

/*
卡牌镶嵌(返回）
*/
struct MSG_50109002 : public Base_Msg  {
	int32_t result;
	int8_t embed_type;
	std::vector<Card_Info> card_info_vec;
	MSG_50109002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int8(embed_type);
		uint16_t __card_info_vec_vec_size = card_info_vec.size();
		w.write_uint16(__card_info_vec_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_vec_size; ++i) {
				card_info_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int8(embed_type)  )
				return -1;
 		uint16_t __card_info_vec_vec_size;
		if(r.read_uint16(__card_info_vec_vec_size)  )
				return -1;
		card_info_vec.reserve(__card_info_vec_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_vec_size; ++i) {
				Card_Info v;
				if(v.deserialize(r))
						return -1;
				card_info_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50109002;
		result = 0;
		embed_type = 0;
		card_info_vec.clear();
	}
};

/*
卡牌升级
*/
struct MSG_10109003  {
	uint32_t card_idx;
	std::vector<uint32_t> card_idx_vec;
	MSG_10109003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(card_idx);
		uint16_t __card_idx_vec_vec_size = card_idx_vec.size();
		w.write_uint16(__card_idx_vec_vec_size);
		for(uint16_t i = 0; i < __card_idx_vec_vec_size; ++i) {
				w.write_uint32(card_idx_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(card_idx)  )
				return -1;
 		uint16_t __card_idx_vec_vec_size;
		if(r.read_uint16(__card_idx_vec_vec_size)  )
				return -1;
		card_idx_vec.reserve(__card_idx_vec_vec_size);
		for(uint16_t i = 0; i < __card_idx_vec_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				card_idx_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		card_idx = 0;
		card_idx_vec.clear();
	}
};

/*
卡牌升级（返回）
*/
struct MSG_50109003 : public Base_Msg  {
	int32_t result;
	Card_Info card_info;
	MSG_50109003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		card_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  card_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50109003;
		result = 0;
		card_info.reset();
	}
};

/*
更新卡牌信息
*/
struct MSG_50109004 : public Base_Msg  {
	std::vector<Card_Info> card_info_vec;
	MSG_50109004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __card_info_vec_vec_size = card_info_vec.size();
		w.write_uint16(__card_info_vec_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_vec_size; ++i) {
				card_info_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __card_info_vec_vec_size;
		if(r.read_uint16(__card_info_vec_vec_size)  )
				return -1;
		card_info_vec.reserve(__card_info_vec_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_vec_size; ++i) {
				Card_Info v;
				if(v.deserialize(r))
						return -1;
				card_info_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50109004;
		card_info_vec.clear();
	}
};

/*
出售卡牌
*/
struct MSG_10109005  {
	int8_t type;
	int32_t card_index;
	MSG_10109005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(card_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(card_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		card_index = 0;
	}
};

/*
出售卡牌(返回）
*/
struct MSG_50109005 : public Base_Msg  {
	int32_t result;
	MSG_50109005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50109005;
		result = 0;
	}
};

/*
卡牌打孔
*/
struct MSG_10109006  {
	int32_t equip_index;
	int8_t loc;
	MSG_10109006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(loc);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(loc)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		equip_index = 0;
		loc = 0;
	}
};

/*
卡牌打孔(返回)
*/
struct MSG_50109006 : public Base_Msg  {
	int32_t result;
	MSG_50109006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50109006;
		result = 0;
	}
};

/*
合成卡牌
*/
struct MSG_10109007  {
	uint32_t card_index;
	std::vector<uint32_t> card_cost;
	MSG_10109007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(card_index);
		uint16_t __card_cost_vec_size = card_cost.size();
		w.write_uint16(__card_cost_vec_size);
		for(uint16_t i = 0; i < __card_cost_vec_size; ++i) {
				w.write_uint32(card_cost[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(card_index)  )
				return -1;
 		uint16_t __card_cost_vec_size;
		if(r.read_uint16(__card_cost_vec_size)  )
				return -1;
		card_cost.reserve(__card_cost_vec_size);
		for(uint16_t i = 0; i < __card_cost_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				card_cost.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		card_index = 0;
		card_cost.clear();
	}
};

/*
合成卡牌(返回)
*/
struct MSG_50109007 : public Base_Msg  {
	int32_t result;
	Card_Info card_info;
	MSG_50109007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		card_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  card_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50109007;
		result = 0;
		card_info.reset();
	}
};

/*
获取vip升级卡牌次数
*/
struct MSG_10109008  {
	MSG_10109008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取vip升级卡牌次数(返回）
*/
struct MSG_50109008 : public Base_Msg  {
	int32_t times;
	MSG_50109008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(times);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50109008;
		times = 0;
	}
};

/*
vip升级卡牌
*/
struct MSG_10109009  {
	uint32_t card_idx;
	MSG_10109009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(card_idx);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(card_idx)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		card_idx = 0;
	}
};

/*
vip升级卡牌(返回）
*/
struct MSG_50109009 : public Base_Msg  {
	int32_t result;
	Card_Info card_info;
	int32_t vip_times;
	MSG_50109009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		card_info.serialize(w);
		w.write_int32(vip_times);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  card_info.deserialize(r)  ||  r.read_int32(vip_times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50109009;
		result = 0;
		card_info.reset();
		vip_times = 0;
	}
};

/*
卡牌升金
*/
struct MSG_10109010  {
	uint32_t card_index;
	MSG_10109010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(card_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(card_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		card_index = 0;
	}
};

/*
卡牌升金(返回)
*/
struct MSG_50109010 : public Base_Msg  {
	int32_t result;
	Card_Info card;
	MSG_50109010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		card.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  card.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50109010;
		result = 0;
		card.reset();
	}
};



#endif