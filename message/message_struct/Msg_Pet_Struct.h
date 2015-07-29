/*
* Generate by devtool
*/

#ifndef _MSG_PET_H_
#define _MSG_PET_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取宠物信息
*/
struct MSG_10113001  {
	MSG_10113001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取宠物信息(返回）
*/
struct MSG_50113001 : public Base_Msg  {
	int16_t lv;
	int8_t state;
	int32_t upgrade_val;
	int32_t active_illusion_id;
	std::vector<Int_Int> skill_vec;
	std::vector<int32_t> illusion_vec;
	MSG_50113001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(lv);
		w.write_int8(state);
		w.write_int32(upgrade_val);
		w.write_int32(active_illusion_id);
		uint16_t __skill_vec_vec_size = skill_vec.size();
		w.write_uint16(__skill_vec_vec_size);
		for(uint16_t i = 0; i < __skill_vec_vec_size; ++i) {
				skill_vec[i].serialize(w);
		}

		uint16_t __illusion_vec_vec_size = illusion_vec.size();
		w.write_uint16(__illusion_vec_vec_size);
		for(uint16_t i = 0; i < __illusion_vec_vec_size; ++i) {
				w.write_int32(illusion_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(lv)  ||  r.read_int8(state)  ||  r.read_int32(upgrade_val)  ||  r.read_int32(active_illusion_id)  )
				return -1;
 		uint16_t __skill_vec_vec_size;
		if(r.read_uint16(__skill_vec_vec_size)  )
				return -1;
		skill_vec.reserve(__skill_vec_vec_size);
		for(uint16_t i = 0; i < __skill_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				skill_vec.push_back(v);
		}

		uint16_t __illusion_vec_vec_size;
		if(r.read_uint16(__illusion_vec_vec_size)  )
				return -1;
		illusion_vec.reserve(__illusion_vec_vec_size);
		for(uint16_t i = 0; i < __illusion_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				illusion_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50113001;
		lv = 0;
		state = 0;
		upgrade_val = 0;
		active_illusion_id = 0;
		skill_vec.clear();
		illusion_vec.clear();
	}
};

/*
宠物升级, 进阶
*/
struct MSG_10113002  {
	int8_t auto_buy;
	MSG_10113002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		auto_buy = 0;
	}
};

/*
宠物升级, 进阶(返回）
*/
struct MSG_50113002 : public Base_Msg  {
	int8_t result;
	int16_t lv;
	int32_t upgrade_val;
	MSG_50113002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int16(lv);
		w.write_int32(upgrade_val);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int16(lv)  ||  r.read_int32(upgrade_val)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50113002;
		result = 0;
		lv = 0;
		upgrade_val = 0;
	}
};

/*
宠物自动升级，进阶
*/
struct MSG_10113003  {
	int8_t auto_buy;
	MSG_10113003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		auto_buy = 0;
	}
};

/*
宠物自动升级，进阶(返回)
*/
struct MSG_50113003 : public Base_Msg  {
	int8_t result;
	int16_t lv;
	int32_t upgrade_val;
	MSG_50113003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int16(lv);
		w.write_int32(upgrade_val);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int16(lv)  ||  r.read_int32(upgrade_val)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50113003;
		result = 0;
		lv = 0;
		upgrade_val = 0;
	}
};

/*
 宠物激活
*/
struct MSG_10113004  {
	MSG_10113004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
 宠物激活(返回）
*/
struct MSG_50113004 : public Base_Msg  {
	int8_t result;
	MSG_50113004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50113004;
		result = 0;
	}
};

/*
宠物幻化
*/
struct MSG_10113005  {
	int32_t illusion_id;
	MSG_10113005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(illusion_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(illusion_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		illusion_id = 0;
	}
};

/*
宠物幻化(返回)
*/
struct MSG_50113005 : public Base_Msg  {
	int8_t result;
	int32_t illusion_id;
	MSG_50113005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(illusion_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(illusion_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50113005;
		result = 0;
		illusion_id = 0;
	}
};

/*
物品激活宠物幻化
*/
struct MSG_10113006  {
	int32_t illusion_id;
	MSG_10113006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(illusion_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(illusion_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		illusion_id = 0;
	}
};

/*
物品激活宠物幻化(返回)
*/
struct MSG_50113006 : public Base_Msg  {
	int8_t result;
	int32_t active_illusion_id;
	std::vector<int32_t> illusion_vec;
	MSG_50113006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(active_illusion_id);
		uint16_t __illusion_vec_vec_size = illusion_vec.size();
		w.write_uint16(__illusion_vec_vec_size);
		for(uint16_t i = 0; i < __illusion_vec_vec_size; ++i) {
				w.write_int32(illusion_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(active_illusion_id)  )
				return -1;
 		uint16_t __illusion_vec_vec_size;
		if(r.read_uint16(__illusion_vec_vec_size)  )
				return -1;
		illusion_vec.reserve(__illusion_vec_vec_size);
		for(uint16_t i = 0; i < __illusion_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				illusion_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50113006;
		result = 0;
		active_illusion_id = 0;
		illusion_vec.clear();
	}
};

/*
获取激活宠物物品
*/
struct MSG_10113007  {
	MSG_10113007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取激活宠物物品(返回)
*/
struct MSG_50113007 : public Base_Msg  {
	int8_t result;
	MSG_50113007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50113007;
		result = 0;
	}
};

/*
宠物低级学习
*/
struct MSG_10113008  {
	int32_t skill_id;
	MSG_10113008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
	}
};

/*
宠物低级学习(返回)
*/
struct MSG_50113008 : public Base_Msg  {
	int8_t result;
	std::vector<Int_Int> skill_vec;
	MSG_50113008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		uint16_t __skill_vec_vec_size = skill_vec.size();
		w.write_uint16(__skill_vec_vec_size);
		for(uint16_t i = 0; i < __skill_vec_vec_size; ++i) {
				skill_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		uint16_t __skill_vec_vec_size;
		if(r.read_uint16(__skill_vec_vec_size)  )
				return -1;
		skill_vec.reserve(__skill_vec_vec_size);
		for(uint16_t i = 0; i < __skill_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				skill_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50113008;
		result = 0;
		skill_vec.clear();
	}
};

/*
宠物高级学习
*/
struct MSG_10113009  {
	int32_t skill_id;
	MSG_10113009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
	}
};

/*
宠物高级学习(返回)
*/
struct MSG_50113009 : public Base_Msg  {
	int8_t result;
	std::vector<Int_Int> skill_vec;
	MSG_50113009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		uint16_t __skill_vec_vec_size = skill_vec.size();
		w.write_uint16(__skill_vec_vec_size);
		for(uint16_t i = 0; i < __skill_vec_vec_size; ++i) {
				skill_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		uint16_t __skill_vec_vec_size;
		if(r.read_uint16(__skill_vec_vec_size)  )
				return -1;
		skill_vec.reserve(__skill_vec_vec_size);
		for(uint16_t i = 0; i < __skill_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				skill_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50113009;
		result = 0;
		skill_vec.clear();
	}
};

/*
改变宠物跟随状态
*/
struct MSG_10113010  {
	int8_t state;
	MSG_10113010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		state = 0;
	}
};

/*
改变宠物跟随状态(返回）
*/
struct MSG_50113010 : public Base_Msg  {
	int8_t result;
	int8_t new_state;
	MSG_50113010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int8(new_state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int8(new_state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50113010;
		result = 0;
		new_state = 0;
	}
};



#endif