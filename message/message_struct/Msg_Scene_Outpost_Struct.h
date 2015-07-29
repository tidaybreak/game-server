/*
* Generate by devtool
*/

#ifndef _MSG_SCENE_OUTPOST_H_
#define _MSG_SCENE_OUTPOST_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求流浪商人物品信息
*/
struct MSG_10211300  {
	int64_t monster_role_id;
	MSG_10211300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(monster_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(monster_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		monster_role_id = 0;
	}
};

/*
请求流浪商人物品信息返回
*/
struct MSG_50211300 : public Base_Msg  {
	std::vector<Secret_Item_Info> items;
	MSG_50211300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __items_vec_size;
		if(r.read_uint16(__items_vec_size)  )
				return -1;
		items.reserve(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				Secret_Item_Info v;
				if(v.deserialize(r))
						return -1;
				items.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50211300;
		items.clear();
	}
};

/*
请求购买流浪商人物品
*/
struct MSG_10211301  {
	int64_t monster_role_id;
	int32_t item_index;
	MSG_10211301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(monster_role_id);
		w.write_int32(item_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(monster_role_id)  ||  r.read_int32(item_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		monster_role_id = 0;
		item_index = 0;
	}
};

/*
请求购买流浪商人物品返回
*/
struct MSG_50211301 : public Base_Msg  {
	int32_t item_index;
	int8_t result;
	MSG_50211301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_index);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_index)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211301;
		item_index = 0;
		result = 0;
	}
};

/*
获取宝藏海湾信息
*/
struct MSG_10211310  {
	MSG_10211310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取宝藏海湾信息返回
*/
struct MSG_50211310 : public Base_Msg  {
	uint32_t remain_time;
	uint32_t gold_num;
	int8_t gold_type;
	int8_t bless_index;
	int8_t state;
	MSG_50211310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(remain_time);
		w.write_uint32(gold_num);
		w.write_int8(gold_type);
		w.write_int8(bless_index);
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(remain_time)  ||  r.read_uint32(gold_num)  ||  r.read_int8(gold_type)  ||  r.read_int8(bless_index)  ||  r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211310;
		remain_time = 0;
		gold_num = 0;
		gold_type = 0;
		bless_index = 0;
		state = 0;
	}
};

/*
请求进入副本关卡
*/
struct MSG_10211311  {
	int64_t monster_role_id;
	MSG_10211311(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(monster_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(monster_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		monster_role_id = 0;
	}
};

/*
请求进入场景关卡返回
*/
struct MSG_50211311 : public Base_Msg  {
	int8_t result;
	MSG_50211311(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211311;
		result = 0;
	}
};

/*
请求宝藏海外神秘祝福
*/
struct MSG_10211312  {
	int8_t bless_index;
	MSG_10211312(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(bless_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(bless_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		bless_index = 0;
	}
};

/*
请求宝藏海湾神秘祝福返回
*/
struct MSG_50211312 : public Base_Msg  {
	int8_t bless_index;
	int8_t result;
	MSG_50211312(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(bless_index);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(bless_index)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211312;
		bless_index = 0;
		result = 0;
	}
};

/*
宝藏海湾货币信息
*/
struct MSG_50211313 : public Base_Msg  {
	uint32_t gold_num;
	MSG_50211313(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(gold_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(gold_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211313;
		gold_num = 0;
	}
};



#endif