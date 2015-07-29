/*
* Generate by devtool
*/

#ifndef _MSG_PACKER_H_
#define _MSG_PACKER_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取背包信息
备注：
1、更新容量 
2、更新物品

10000：人物装备
20000：人物背包
30000：人物仓库
40000：成就勋章身上

100000：符石熔炉
110000：符石背包
120000：符石仓库
130000：符石身上
140000：符石魔盒
*/
struct MSG_10100100  {
	uint32_t pack_type;
	uint32_t pack_sub_type;
	MSG_10100100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(pack_type);
		w.write_uint32(pack_sub_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(pack_type)  ||  r.read_uint32(pack_sub_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pack_type = 0;
		pack_sub_type = 0;
	}
};

/*
获取背包信息(返回)
备注：
1、更新容量 
2、更新物品
*/
struct MSG_50100100 : public Base_Msg  {
	int32_t pack_type;
	int16_t capacity;
	std::vector<Item_Basic_Info> item_info;
	std::vector<Item_Equip_Info> equip_info;
	std::vector<Item_Rune_Stone_Info> rune_stone_info;
	std::vector<Card_Info> card_info;
	int64_t target_role_id;
	MSG_50100100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(pack_type);
		w.write_int16(capacity);
		uint16_t __item_info_vec_size = item_info.size();
		w.write_uint16(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				item_info[i].serialize(w);
		}

		uint16_t __equip_info_vec_size = equip_info.size();
		w.write_uint16(__equip_info_vec_size);
		for(uint16_t i = 0; i < __equip_info_vec_size; ++i) {
				equip_info[i].serialize(w);
		}

		uint16_t __rune_stone_info_vec_size = rune_stone_info.size();
		w.write_uint16(__rune_stone_info_vec_size);
		for(uint16_t i = 0; i < __rune_stone_info_vec_size; ++i) {
				rune_stone_info[i].serialize(w);
		}

		uint16_t __card_info_vec_size = card_info.size();
		w.write_uint16(__card_info_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_size; ++i) {
				card_info[i].serialize(w);
		}

		w.write_int64(target_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(pack_type)  ||  r.read_int16(capacity)  )
				return -1;
 		uint16_t __item_info_vec_size;
		if(r.read_uint16(__item_info_vec_size)  )
				return -1;
		item_info.reserve(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				Item_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				item_info.push_back(v);
		}

		uint16_t __equip_info_vec_size;
		if(r.read_uint16(__equip_info_vec_size)  )
				return -1;
		equip_info.reserve(__equip_info_vec_size);
		for(uint16_t i = 0; i < __equip_info_vec_size; ++i) {
				Item_Equip_Info v;
				if(v.deserialize(r))
						return -1;
				equip_info.push_back(v);
		}

		uint16_t __rune_stone_info_vec_size;
		if(r.read_uint16(__rune_stone_info_vec_size)  )
				return -1;
		rune_stone_info.reserve(__rune_stone_info_vec_size);
		for(uint16_t i = 0; i < __rune_stone_info_vec_size; ++i) {
				Item_Rune_Stone_Info v;
				if(v.deserialize(r))
						return -1;
				rune_stone_info.push_back(v);
		}

		uint16_t __card_info_vec_size;
		if(r.read_uint16(__card_info_vec_size)  )
				return -1;
		card_info.reserve(__card_info_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_size; ++i) {
				Card_Info v;
				if(v.deserialize(r))
						return -1;
				card_info.push_back(v);
		}

		if( r.read_int64(target_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100100;
		pack_type = 0;
		capacity = 0;
		item_info.clear();
		equip_info.clear();
		rune_stone_info.clear();
		card_info.clear();
		target_role_id = 0;
	}
};

/*
增加容量
*/
struct MSG_10100101  {
	uint8_t pay_type;
	uint32_t pack_type;
	uint16_t add_capacity;
	MSG_10100101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(pay_type);
		w.write_uint32(pack_type);
		w.write_uint16(add_capacity);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(pay_type)  ||  r.read_uint32(pack_type)  ||  r.read_uint16(add_capacity)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pay_type = 0;
		pack_type = 0;
		add_capacity = 0;
	}
};

/*
增加容量(返回)
*/
struct MSG_50100101 : public Base_Msg  {
	MSG_50100101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100101;
	}
};

/*
丢弃物品
*/
struct MSG_10100102  {
	std::vector<uint32_t> index;
	MSG_10100102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __index_vec_size = index.size();
		w.write_uint16(__index_vec_size);
		for(uint16_t i = 0; i < __index_vec_size; ++i) {
				w.write_uint32(index[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __index_vec_size;
		if(r.read_uint16(__index_vec_size)  )
				return -1;
		index.reserve(__index_vec_size);
		for(uint16_t i = 0; i < __index_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				index.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		index.clear();
	}
};

/*
丢弃物品(返回)
*/
struct MSG_50100102 : public Base_Msg  {
	MSG_50100102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100102;
	}
};

/*
移动物品
*/
struct MSG_10100103  {
	uint32_t index_from;
	uint32_t index_to;
	MSG_10100103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index_from);
		w.write_uint32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index_from)  ||  r.read_uint32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index_from = 0;
		index_to = 0;
	}
};

/*
移动物品(返回)
*/
struct MSG_50100103 : public Base_Msg  {
	uint32_t index;
	MSG_50100103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100103;
		index = 0;
	}
};

/*
拆分物品
*/
struct MSG_10100104  {
	uint32_t index;
	int16_t split_num;
	MSG_10100104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_int16(split_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_int16(split_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		split_num = 0;
	}
};

/*
拆分物品(返回)
*/
struct MSG_50100104 : public Base_Msg  {
	MSG_50100104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100104;
	}
};

/*
整理物品
*/
struct MSG_10100105  {
	uint32_t pack_type;
	MSG_10100105(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(pack_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(pack_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pack_type = 0;
	}
};

/*
整理物品(返回)
*/
struct MSG_50100105 : public Base_Msg  {
	MSG_50100105(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100105;
	}
};

/*
使用指定位置的物品
备注：所有类型的物品都由此接口使用，由服务端判断使用物品的效果并以主动消息形式通知。
*/
struct MSG_10100106  {
	uint32_t index;
	int32_t amount;
	int32_t e_value;
	MSG_10100106(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_int32(amount);
		w.write_int32(e_value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_int32(amount)  ||  r.read_int32(e_value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		amount = 0;
		e_value = 0;
	}
};

/*
使用指定位置的物品(返回)
*/
struct MSG_50100106 : public Base_Msg  {
	uint32_t id;
	int8_t result;
	int32_t tip;
	MSG_50100106(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_int8(result);
		w.write_int32(tip);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_int8(result)  ||  r.read_int32(tip)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100106;
		id = 0;
		result = 0;
		tip = 0;
	}
};

/*
使用指定型礼包
*/
struct MSG_10100107  {
	int32_t item_index;
	std::vector<int32_t> select_index_vector;
	MSG_10100107(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_index);
		uint16_t __select_index_vector_vec_size = select_index_vector.size();
		w.write_uint16(__select_index_vector_vec_size);
		for(uint16_t i = 0; i < __select_index_vector_vec_size; ++i) {
				w.write_int32(select_index_vector[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_index)  )
				return -1;
 		uint16_t __select_index_vector_vec_size;
		if(r.read_uint16(__select_index_vector_vec_size)  )
				return -1;
		select_index_vector.reserve(__select_index_vector_vec_size);
		for(uint16_t i = 0; i < __select_index_vector_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				select_index_vector.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		item_index = 0;
		select_index_vector.clear();
	}
};

/*
使用指定型礼包(返回)
*/
struct MSG_50100107 : public Base_Msg  {
	MSG_50100107(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100107;
	}
};

/*
请求合并物品
备注：整理并合并物品，可能会将非绑定物品转成绑定物品以节约空间，同凡2的合并
*/
struct MSG_10100108  {
	uint32_t pack_type;
	MSG_10100108(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(pack_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(pack_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pack_type = 0;
	}
};

/*
请求合并物品(返回)
*/
struct MSG_50100108 : public Base_Msg  {
	MSG_50100108(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100108;
	}
};

/*
请求出售物品
Int_Int:val_1位置 val_2数量
*/
struct MSG_10100109  {
	std::vector<Int_Int> index;
	MSG_10100109(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __index_vec_size = index.size();
		w.write_uint16(__index_vec_size);
		for(uint16_t i = 0; i < __index_vec_size; ++i) {
				index[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __index_vec_size;
		if(r.read_uint16(__index_vec_size)  )
				return -1;
		index.reserve(__index_vec_size);
		for(uint16_t i = 0; i < __index_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				index.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		index.clear();
	}
};

/*
请求出售物品(返回)
备注：金钱的改变由主动消息通知，此消息不同于商店模块的，出售之后不能找回。
*/
struct MSG_50100109 : public Base_Msg  {
	int8_t result;
	MSG_50100109(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100109;
		result = 0;
	}
};

/*
取出爬塔副本仓库
*/
struct MSG_10100110  {
	uint32_t index;
	MSG_10100110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
	}
};

/*
取出爬塔副本仓库(返回)
*/
struct MSG_50100110 : public Base_Msg  {
	MSG_50100110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100110;
	}
};

/*
从任意仓库取出物品
*/
struct MSG_10100111  {
	int8_t type;
	int32_t index;
	MSG_10100111(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		index = 0;
	}
};

/*
从任意仓库取出物品(返回)
*/
struct MSG_50100111 : public Base_Msg  {
	MSG_50100111(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100111;
	}
};

/*
获取指定角色背包信息
*/
struct MSG_10100112  {
	int64_t role_id;
	MSG_10100112(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
获取指定角色背包信息(返回)
*/
struct MSG_50100112 : public Base_Msg  {
	int64_t role_id;
	std::vector<Item_Basic_Info> item_info;
	std::vector<Item_Equip_Info> equip_info;
	std::vector<Item_Rune_Stone_Info> rune_stone_info;
	MSG_50100112(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __item_info_vec_size = item_info.size();
		w.write_uint16(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				item_info[i].serialize(w);
		}

		uint16_t __equip_info_vec_size = equip_info.size();
		w.write_uint16(__equip_info_vec_size);
		for(uint16_t i = 0; i < __equip_info_vec_size; ++i) {
				equip_info[i].serialize(w);
		}

		uint16_t __rune_stone_info_vec_size = rune_stone_info.size();
		w.write_uint16(__rune_stone_info_vec_size);
		for(uint16_t i = 0; i < __rune_stone_info_vec_size; ++i) {
				rune_stone_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __item_info_vec_size;
		if(r.read_uint16(__item_info_vec_size)  )
				return -1;
		item_info.reserve(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				Item_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				item_info.push_back(v);
		}

		uint16_t __equip_info_vec_size;
		if(r.read_uint16(__equip_info_vec_size)  )
				return -1;
		equip_info.reserve(__equip_info_vec_size);
		for(uint16_t i = 0; i < __equip_info_vec_size; ++i) {
				Item_Equip_Info v;
				if(v.deserialize(r))
						return -1;
				equip_info.push_back(v);
		}

		uint16_t __rune_stone_info_vec_size;
		if(r.read_uint16(__rune_stone_info_vec_size)  )
				return -1;
		rune_stone_info.reserve(__rune_stone_info_vec_size);
		for(uint16_t i = 0; i < __rune_stone_info_vec_size; ++i) {
				Item_Rune_Stone_Info v;
				if(v.deserialize(r))
						return -1;
				rune_stone_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100112;
		role_id = 0;
		item_info.clear();
		equip_info.clear();
		rune_stone_info.clear();
	}
};

/*
购买并使用
*/
struct MSG_10100113  {
	int32_t item_id;
	int32_t amount;
	MSG_10100113(void) { reset(); };

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
		item_id = 0;
		amount = 0;
	}
};

/*
购买并使用(返回)
*/
struct MSG_50100113 : public Base_Msg  {
	MSG_50100113(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100113;
	}
};



#endif