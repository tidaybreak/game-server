/*
* Generate by devtool
*/

#ifndef _MSG_EQUIP_H_
#define _MSG_EQUIP_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
装备穿脱
*/
struct MSG_10101000  {
	uint32_t index_from;
	uint32_t index_to;
	MSG_10101000(void) { reset(); };

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
装备穿脱(返回)
*/
struct MSG_50101000 : public Base_Msg  {
	MSG_50101000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50101000;
	}
};

/*
装备解封
*/
struct MSG_10101001  {
	uint32_t index;
	uint32_t lucky_stone_index;
	uint32_t lucky_stone_id;
	int8_t auto_buy;
	MSG_10101001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_uint32(lucky_stone_index);
		w.write_uint32(lucky_stone_id);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_uint32(lucky_stone_index)  ||  r.read_uint32(lucky_stone_id)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		lucky_stone_index = 0;
		lucky_stone_id = 0;
		auto_buy = 0;
	}
};

/*
装备解封(返回)
*/
struct MSG_50101001 : public Base_Msg  {
	uint32_t index;
	uint32_t lucky_stone_id;
	int8_t result;
	MSG_50101001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_uint32(lucky_stone_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_uint32(lucky_stone_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101001;
		index = 0;
		lucky_stone_id = 0;
		result = 0;
	}
};

/*
装备精通
*/
struct MSG_10101002  {
	uint32_t index;
	uint8_t auto_buy;
	MSG_10101002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_uint8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_uint8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		auto_buy = 0;
	}
};

/*
装备精通(返回)
*/
struct MSG_50101002 : public Base_Msg  {
	int8_t result;
	int32_t copper;
	std::vector<Item_Basic_Info> item_vec;
	uint8_t level;
	int32_t luck_value;
	int8_t fail_add_rate;
	uint32_t index;
	MSG_50101002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(copper);
		uint16_t __item_vec_vec_size = item_vec.size();
		w.write_uint16(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				item_vec[i].serialize(w);
		}

		w.write_uint8(level);
		w.write_int32(luck_value);
		w.write_int8(fail_add_rate);
		w.write_uint32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(copper)  )
				return -1;
 		uint16_t __item_vec_vec_size;
		if(r.read_uint16(__item_vec_vec_size)  )
				return -1;
		item_vec.reserve(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				Item_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				item_vec.push_back(v);
		}

		if( r.read_uint8(level)  ||  r.read_int32(luck_value)  ||  r.read_int8(fail_add_rate)  ||  r.read_uint32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101002;
		result = 0;
		copper = 0;
		item_vec.clear();
		level = 0;
		luck_value = 0;
		fail_add_rate = 0;
		index = 0;
	}
};

/*
装备合成
*/
struct MSG_10101003  {
	int32_t id;
	int32_t amount;
	int8_t use_diamond;
	MSG_10101003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(amount);
		w.write_int8(use_diamond);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(amount)  ||  r.read_int8(use_diamond)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		amount = 0;
		use_diamond = 0;
	}
};

/*
装备合成(返回)
*/
struct MSG_50101003 : public Base_Msg  {
	int8_t result;
	MSG_50101003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101003;
		result = 0;
	}
};

/*
获取精通信息
*/
struct MSG_10101004  {
	MSG_10101004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取精通信息(返回)
*/
struct MSG_50101004 : public Base_Msg  {
	std::vector<Master_Part_Info> part_infos;
	MSG_50101004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __part_infos_vec_size = part_infos.size();
		w.write_uint16(__part_infos_vec_size);
		for(uint16_t i = 0; i < __part_infos_vec_size; ++i) {
				part_infos[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __part_infos_vec_size;
		if(r.read_uint16(__part_infos_vec_size)  )
				return -1;
		part_infos.reserve(__part_infos_vec_size);
		for(uint16_t i = 0; i < __part_infos_vec_size; ++i) {
				Master_Part_Info v;
				if(v.deserialize(r))
						return -1;
				part_infos.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101004;
		part_infos.clear();
	}
};

/*
装备附魔
*/
struct MSG_10101005  {
	int32_t equip_index;
	int8_t is_bat;
	int8_t auto_buy;
	int8_t auto_buy_stone;
	int8_t auto_buy_lock;
	std::vector<int8_t> locks;
	MSG_10101005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(is_bat);
		w.write_int8(auto_buy);
		w.write_int8(auto_buy_stone);
		w.write_int8(auto_buy_lock);
		uint16_t __locks_vec_size = locks.size();
		w.write_uint16(__locks_vec_size);
		for(uint16_t i = 0; i < __locks_vec_size; ++i) {
				w.write_int8(locks[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(is_bat)  ||  r.read_int8(auto_buy)  ||  r.read_int8(auto_buy_stone)  ||  r.read_int8(auto_buy_lock)  )
				return -1;
 		uint16_t __locks_vec_size;
		if(r.read_uint16(__locks_vec_size)  )
				return -1;
		locks.reserve(__locks_vec_size);
		for(uint16_t i = 0; i < __locks_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				locks.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		equip_index = 0;
		is_bat = 0;
		auto_buy = 0;
		auto_buy_stone = 0;
		auto_buy_lock = 0;
		locks.clear();
	}
};

/*
装备附魔属性组替换
*/
struct MSG_10101006  {
	int32_t equip_index;
	int8_t group_index;
	MSG_10101006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(group_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(group_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		equip_index = 0;
		group_index = 0;
	}
};

/*
装备附魔属性组替换（返回）
*/
struct MSG_50101006 : public Base_Msg  {
	int32_t equip_index;
	int8_t group_index;
	int8_t result;
	MSG_50101006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(group_index);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(group_index)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101006;
		equip_index = 0;
		group_index = 0;
		result = 0;
	}
};

/*
装备附魔（返回）
*/
struct MSG_50101005 : public Base_Msg  {
	int8_t result;
	MSG_50101005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101005;
		result = 0;
	}
};

/*
镶嵌宝石
*/
struct MSG_10101007  {
	int32_t equip_index;
	int8_t hole_index;
	int32_t gemstone_id;
	MSG_10101007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(hole_index);
		w.write_int32(gemstone_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(hole_index)  ||  r.read_int32(gemstone_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		equip_index = 0;
		hole_index = 0;
		gemstone_id = 0;
	}
};

/*
镶嵌宝石(返回)
*/
struct MSG_50101007 : public Base_Msg  {
	int32_t result;
	MSG_50101007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101007;
		result = 0;
	}
};

/*
卸装宝石
*/
struct MSG_10101008  {
	int32_t equip_index;
	int8_t hole_index;
	MSG_10101008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(hole_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(hole_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		equip_index = 0;
		hole_index = 0;
	}
};

/*
卸装宝石(返回)
*/
struct MSG_50101008 : public Base_Msg  {
	int32_t result;
	MSG_50101008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101008;
		result = 0;
	}
};

/*
装备开宝石孔
*/
struct MSG_10101009  {
	int32_t equip_index;
	int8_t hole_index;
	MSG_10101009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(hole_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(hole_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		equip_index = 0;
		hole_index = 0;
	}
};

/*
装备开宝石孔
*/
struct MSG_50101009 : public Base_Msg  {
	int32_t result;
	MSG_50101009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101009;
		result = 0;
	}
};

/*
宝石转化
*/
struct MSG_10101010  {
	int32_t src_gemstone_id;
	int32_t des_gemstone_id;
	int32_t num;
	MSG_10101010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(src_gemstone_id);
		w.write_int32(des_gemstone_id);
		w.write_int32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(src_gemstone_id)  ||  r.read_int32(des_gemstone_id)  ||  r.read_int32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		src_gemstone_id = 0;
		des_gemstone_id = 0;
		num = 0;
	}
};

/*
宝石转化
*/
struct MSG_50101010 : public Base_Msg  {
	int32_t result;
	MSG_50101010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101010;
		result = 0;
	}
};

/*
宝石信息合成
*/
struct MSG_10101011  {
	MSG_10101011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
宝石信息合成(返回)
*/
struct MSG_50101011 : public Base_Msg  {
	std::vector<int32_t> gem_id;
	MSG_50101011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __gem_id_vec_size = gem_id.size();
		w.write_uint16(__gem_id_vec_size);
		for(uint16_t i = 0; i < __gem_id_vec_size; ++i) {
				w.write_int32(gem_id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __gem_id_vec_size;
		if(r.read_uint16(__gem_id_vec_size)  )
				return -1;
		gem_id.reserve(__gem_id_vec_size);
		for(uint16_t i = 0; i < __gem_id_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				gem_id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101011;
		gem_id.clear();
	}
};

/*
装备分解
*/
struct MSG_10101012  {
	std::vector<int32_t> equip_index;
	MSG_10101012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __equip_index_vec_size = equip_index.size();
		w.write_uint16(__equip_index_vec_size);
		for(uint16_t i = 0; i < __equip_index_vec_size; ++i) {
				w.write_int32(equip_index[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __equip_index_vec_size;
		if(r.read_uint16(__equip_index_vec_size)  )
				return -1;
		equip_index.reserve(__equip_index_vec_size);
		for(uint16_t i = 0; i < __equip_index_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				equip_index.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		equip_index.clear();
	}
};

/*
装备分解(返回)
*/
struct MSG_50101012 : public Base_Msg  {
	int8_t result;
	MSG_50101012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101012;
		result = 0;
	}
};

/*
装备分解获得道具
*/
struct MSG_10101013  {
	std::vector<int32_t> equip_index;
	MSG_10101013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __equip_index_vec_size = equip_index.size();
		w.write_uint16(__equip_index_vec_size);
		for(uint16_t i = 0; i < __equip_index_vec_size; ++i) {
				w.write_int32(equip_index[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __equip_index_vec_size;
		if(r.read_uint16(__equip_index_vec_size)  )
				return -1;
		equip_index.reserve(__equip_index_vec_size);
		for(uint16_t i = 0; i < __equip_index_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				equip_index.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		equip_index.clear();
	}
};

/*
装备分解获得道具（返回）
*/
struct MSG_50101013 : public Base_Msg  {
	std::vector<Item_Id_Amount> prop;
	int32_t enchant_num;
	int32_t gold;
	MSG_50101013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __prop_vec_size = prop.size();
		w.write_uint16(__prop_vec_size);
		for(uint16_t i = 0; i < __prop_vec_size; ++i) {
				prop[i].serialize(w);
		}

		w.write_int32(enchant_num);
		w.write_int32(gold);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __prop_vec_size;
		if(r.read_uint16(__prop_vec_size)  )
				return -1;
		prop.reserve(__prop_vec_size);
		for(uint16_t i = 0; i < __prop_vec_size; ++i) {
				Item_Id_Amount v;
				if(v.deserialize(r))
						return -1;
				prop.push_back(v);
		}

		if( r.read_int32(enchant_num)  ||  r.read_int32(gold)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101013;
		prop.clear();
		enchant_num = 0;
		gold = 0;
	}
};

/*
请求宝石雕刻面板
*/
struct MSG_10101014  {
	int32_t gem_essence_num;
	MSG_10101014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(gem_essence_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(gem_essence_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gem_essence_num = 0;
	}
};

/*
请求宝石雕刻面板（返回）
*/
struct MSG_50101014 : public Base_Msg  {
	int32_t result;
	int32_t carve_lvl;
	int32_t gem_progress;
	int32_t ess_num;
	MSG_50101014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int32(carve_lvl);
		w.write_int32(gem_progress);
		w.write_int32(ess_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int32(carve_lvl)  ||  r.read_int32(gem_progress)  ||  r.read_int32(ess_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101014;
		result = 0;
		carve_lvl = 0;
		gem_progress = 0;
		ess_num = 0;
	}
};

/*
请求宝石雕刻等级
*/
struct MSG_10101015  {
	MSG_10101015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求宝石雕刻等级(返回)
*/
struct MSG_50101015 : public Base_Msg  {
	int32_t carve_lvl;
	int32_t gem_progress;
	MSG_50101015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(carve_lvl);
		w.write_int32(gem_progress);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(carve_lvl)  ||  r.read_int32(gem_progress)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101015;
		carve_lvl = 0;
		gem_progress = 0;
	}
};



#endif