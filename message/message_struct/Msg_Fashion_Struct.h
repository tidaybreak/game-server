/*
* Generate by devtool
*/

#ifndef _MSG_FASHION_H_
#define _MSG_FASHION_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取已拥有的时装
*/
struct MSG_10105001  {
	MSG_10105001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取已拥有的时装(返回)
*/
struct MSG_50105001 : public Base_Msg  {
	std::vector<Fashion_Info> fashion_info_vec;
	int8_t is_show;
	MSG_50105001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __fashion_info_vec_vec_size = fashion_info_vec.size();
		w.write_uint16(__fashion_info_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_info_vec_vec_size; ++i) {
				fashion_info_vec[i].serialize(w);
		}

		w.write_int8(is_show);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __fashion_info_vec_vec_size;
		if(r.read_uint16(__fashion_info_vec_vec_size)  )
				return -1;
		fashion_info_vec.reserve(__fashion_info_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_info_vec_vec_size; ++i) {
				Fashion_Info v;
				if(v.deserialize(r))
						return -1;
				fashion_info_vec.push_back(v);
		}

		if( r.read_int8(is_show)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105001;
		fashion_info_vec.clear();
		is_show = 0;
	}
};

/*
改变时装穿戴状态
*/
struct MSG_10105002  {
	std::vector<int32_t> fashion_vec;
	MSG_10105002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __fashion_vec_vec_size = fashion_vec.size();
		w.write_uint16(__fashion_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_vec_vec_size; ++i) {
				w.write_int32(fashion_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __fashion_vec_vec_size;
		if(r.read_uint16(__fashion_vec_vec_size)  )
				return -1;
		fashion_vec.reserve(__fashion_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				fashion_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		fashion_vec.clear();
	}
};

/*
改变时装穿戴状态(返回）
*/
struct MSG_50105002 : public Base_Msg  {
	int8_t result;
	MSG_50105002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105002;
		result = 0;
	}
};

/*
合成时装
*/
struct MSG_10105003  {
	int32_t fashion_id;
	MSG_10105003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(fashion_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(fashion_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		fashion_id = 0;
	}
};

/*
升级时装
*/
struct MSG_10105004  {
	int32_t fashion_id;
	int16_t lv;
	std::vector<Item_Id_Amount> item_vec;
	MSG_10105004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(fashion_id);
		w.write_int16(lv);
		uint16_t __item_vec_vec_size = item_vec.size();
		w.write_uint16(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				item_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(fashion_id)  ||  r.read_int16(lv)  )
				return -1;
 		uint16_t __item_vec_vec_size;
		if(r.read_uint16(__item_vec_vec_size)  )
				return -1;
		item_vec.reserve(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				Item_Id_Amount v;
				if(v.deserialize(r))
						return -1;
				item_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		fashion_id = 0;
		lv = 0;
		item_vec.clear();
	}
};

/*
时装传承
*/
struct MSG_10105005  {
	int32_t fashion_id;
	int32_t target_id;
	MSG_10105005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(fashion_id);
		w.write_int32(target_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(fashion_id)  ||  r.read_int32(target_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		fashion_id = 0;
		target_id = 0;
	}
};

/*
合成时装(返回)
*/
struct MSG_50105003 : public Base_Msg  {
	int8_t result;
	MSG_50105003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105003;
		result = 0;
	}
};

/*
升级时装(返回)
*/
struct MSG_50105004 : public Base_Msg  {
	int8_t result;
	int32_t lv;
	MSG_50105004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105004;
		result = 0;
		lv = 0;
	}
};

/*
时装传承(返回)
*/
struct MSG_50105005 : public Base_Msg  {
	int8_t result;
	MSG_50105005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105005;
		result = 0;
	}
};

/*
时装抽卡牌
*/
struct MSG_10105006  {
	int8_t pos;
	int8_t type;
	MSG_10105006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(pos);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(pos)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pos = 0;
		type = 0;
	}
};

/*
时装抽卡牌(返回)
*/
struct MSG_50105006 : public Base_Msg  {
	int8_t pos;
	std::vector<int32_t> id_vec;
	MSG_50105006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(pos);
		uint16_t __id_vec_vec_size = id_vec.size();
		w.write_uint16(__id_vec_vec_size);
		for(uint16_t i = 0; i < __id_vec_vec_size; ++i) {
				w.write_int32(id_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(pos)  )
				return -1;
 		uint16_t __id_vec_vec_size;
		if(r.read_uint16(__id_vec_vec_size)  )
				return -1;
		id_vec.reserve(__id_vec_vec_size);
		for(uint16_t i = 0; i < __id_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				id_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50105006;
		pos = 0;
		id_vec.clear();
	}
};

/*
获取翅膀道具
*/
struct MSG_10105011  {
	MSG_10105011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取翅膀道具(返回)
*/
struct MSG_50105011 : public Base_Msg  {
	int32_t result;
	MSG_50105011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105011;
		result = 0;
	}
};

/*
激活翅膀道具
*/
struct MSG_10105012  {
	MSG_10105012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
激活翅膀道具(返回)
*/
struct MSG_50105012 : public Base_Msg  {
	int32_t result;
	MSG_50105012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105012;
		result = 0;
	}
};

/*
获取翅膀信息(返回)
*/
struct MSG_50105013 : public Base_Msg  {
	int32_t lv;
	int32_t progress;
	int32_t use_times;
	uint8_t is_show;
	int32_t cur_score;
	int32_t next_score;
	int8_t state;
	int32_t ret_reward_stage;
	int32_t secs;
	MSG_50105013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(lv);
		w.write_int32(progress);
		w.write_int32(use_times);
		w.write_uint8(is_show);
		w.write_int32(cur_score);
		w.write_int32(next_score);
		w.write_int8(state);
		w.write_int32(ret_reward_stage);
		w.write_int32(secs);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(lv)  ||  r.read_int32(progress)  ||  r.read_int32(use_times)  ||  r.read_uint8(is_show)  ||  r.read_int32(cur_score)  ||  r.read_int32(next_score)  ||  r.read_int8(state)  ||  r.read_int32(ret_reward_stage)  ||  r.read_int32(secs)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105013;
		lv = 0;
		progress = 0;
		use_times = 0;
		is_show = 0;
		cur_score = 0;
		next_score = 0;
		state = 0;
		ret_reward_stage = 0;
		secs = 0;
	}
};

/*
获取翅膀信息
*/
struct MSG_10105013  {
	MSG_10105013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
翅膀进阶
*/
struct MSG_10105014  {
	int8_t is_auto_buy;
	MSG_10105014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_auto_buy = 0;
	}
};

/*
翅膀进阶(返回)
*/
struct MSG_50105014 : public Base_Msg  {
	int32_t result;
	int32_t lv;
	int32_t progress;
	int32_t use_times;
	int32_t cur_score;
	int32_t next_score;
	MSG_50105014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int32(lv);
		w.write_int32(progress);
		w.write_int32(use_times);
		w.write_int32(cur_score);
		w.write_int32(next_score);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int32(lv)  ||  r.read_int32(progress)  ||  r.read_int32(use_times)  ||  r.read_int32(cur_score)  ||  r.read_int32(next_score)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105014;
		result = 0;
		lv = 0;
		progress = 0;
		use_times = 0;
		cur_score = 0;
		next_score = 0;
	}
};

/*
自动翅膀进阶
*/
struct MSG_10105015  {
	int8_t is_auto_buy;
	MSG_10105015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_auto_buy = 0;
	}
};

/*
自动翅膀进阶(返回)
*/
struct MSG_50105015 : public Base_Msg  {
	int32_t result;
	int32_t lv;
	int32_t progress;
	int32_t use_times;
	int32_t cur_score;
	int32_t next_score;
	MSG_50105015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int32(lv);
		w.write_int32(progress);
		w.write_int32(use_times);
		w.write_int32(cur_score);
		w.write_int32(next_score);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int32(lv)  ||  r.read_int32(progress)  ||  r.read_int32(use_times)  ||  r.read_int32(cur_score)  ||  r.read_int32(next_score)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105015;
		result = 0;
		lv = 0;
		progress = 0;
		use_times = 0;
		cur_score = 0;
		next_score = 0;
	}
};

/*
显示/隐藏翅膀avatar
*/
struct MSG_10105016  {
	int8_t state;
	MSG_10105016(void) { reset(); };

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
显示/隐藏翅膀avatar(返回)
*/
struct MSG_50105016 : public Base_Msg  {
	int32_t result;
	int8_t state;
	MSG_50105016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105016;
		result = 0;
		state = 0;
	}
};

/*
改变时装显示状态
*/
struct MSG_10105007  {
	int8_t state;
	MSG_10105007(void) { reset(); };

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
改变时装显示状态(返回)
*/
struct MSG_50105007 : public Base_Msg  {
	int8_t state;
	int8_t result;
	MSG_50105007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105007;
		state = 0;
		result = 0;
	}
};

/*
领取返利
*/
struct MSG_10105017  {
	int32_t stage;
	MSG_10105017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(stage);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(stage)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		stage = 0;
	}
};

/*
领取返利(返回）
*/
struct MSG_50105017 : public Base_Msg  {
	int32_t result;
	int32_t stage;
	MSG_50105017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int32(stage);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int32(stage)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105017;
		result = 0;
		stage = 0;
	}
};

/*
激活翅膀体验
*/
struct MSG_10105018  {
	MSG_10105018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
新版获取时装信息(返回）
*/
struct MSG_50105020 : public Base_Msg  {
	int8_t lv;
	int32_t exp;
	int8_t is_show;
	std::vector<Fashion_Part_Info> fashion_part_vec;
	MSG_50105020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(lv);
		w.write_int32(exp);
		w.write_int8(is_show);
		uint16_t __fashion_part_vec_vec_size = fashion_part_vec.size();
		w.write_uint16(__fashion_part_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_part_vec_vec_size; ++i) {
				fashion_part_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(lv)  ||  r.read_int32(exp)  ||  r.read_int8(is_show)  )
				return -1;
 		uint16_t __fashion_part_vec_vec_size;
		if(r.read_uint16(__fashion_part_vec_vec_size)  )
				return -1;
		fashion_part_vec.reserve(__fashion_part_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_part_vec_vec_size; ++i) {
				Fashion_Part_Info v;
				if(v.deserialize(r))
						return -1;
				fashion_part_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50105020;
		lv = 0;
		exp = 0;
		is_show = 0;
		fashion_part_vec.clear();
	}
};

/*
新版获取时装信息
*/
struct MSG_10105020  {
	MSG_10105020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
使用时装道具
*/
struct MSG_10105021  {
	std::vector<uint32_t> item_idx_vec;
	int8_t wear;
	MSG_10105021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_idx_vec_vec_size = item_idx_vec.size();
		w.write_uint16(__item_idx_vec_vec_size);
		for(uint16_t i = 0; i < __item_idx_vec_vec_size; ++i) {
				w.write_uint32(item_idx_vec[i]);
		}

		w.write_int8(wear);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __item_idx_vec_vec_size;
		if(r.read_uint16(__item_idx_vec_vec_size)  )
				return -1;
		item_idx_vec.reserve(__item_idx_vec_vec_size);
		for(uint16_t i = 0; i < __item_idx_vec_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				item_idx_vec.push_back(v);
		}

		if( r.read_int8(wear)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_idx_vec.clear();
		wear = 0;
	}
};

/*
使用时装道具(返回）
*/
struct MSG_50105021 : public Base_Msg  {
	int8_t result;
	int32_t soul_lv;
	int32_t exp;
	std::vector<int32_t> fashion_id_vec;
	int8_t wear;
	MSG_50105021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(soul_lv);
		w.write_int32(exp);
		uint16_t __fashion_id_vec_vec_size = fashion_id_vec.size();
		w.write_uint16(__fashion_id_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_id_vec_vec_size; ++i) {
				w.write_int32(fashion_id_vec[i]);
		}

		w.write_int8(wear);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(soul_lv)  ||  r.read_int32(exp)  )
				return -1;
 		uint16_t __fashion_id_vec_vec_size;
		if(r.read_uint16(__fashion_id_vec_vec_size)  )
				return -1;
		fashion_id_vec.reserve(__fashion_id_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_id_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				fashion_id_vec.push_back(v);
		}

		if( r.read_int8(wear)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105021;
		result = 0;
		soul_lv = 0;
		exp = 0;
		fashion_id_vec.clear();
		wear = 0;
	}
};

/*
升级时装部位
*/
struct MSG_10105022  {
	int8_t part_id;
	int8_t auto_buy;
	MSG_10105022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(part_id);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(part_id)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		part_id = 0;
		auto_buy = 0;
	}
};

/*
升级时装部位(返回)
*/
struct MSG_50105022 : public Base_Msg  {
	int32_t result;
	int8_t part_id;
	int16_t lv;
	int32_t exp;
	MSG_50105022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int8(part_id);
		w.write_int16(lv);
		w.write_int32(exp);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int8(part_id)  ||  r.read_int16(lv)  ||  r.read_int32(exp)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50105022;
		result = 0;
		part_id = 0;
		lv = 0;
		exp = 0;
	}
};



#endif