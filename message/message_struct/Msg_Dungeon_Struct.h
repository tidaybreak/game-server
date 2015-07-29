/*
* Generate by devtool
*/

#ifndef _MSG_DUNGEON_H_
#define _MSG_DUNGEON_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求已通关单人副本
*/
struct MSG_10100200  {
	MSG_10100200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求已通关单人副本(返回)
*/
struct MSG_50100200 : public Base_Msg  {
	std::vector<Single_Dungeon_Info> sgl_dungeon_info_array;
	std::vector<int8_t> chapter_award_vec;
	MSG_50100200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __sgl_dungeon_info_array_vec_size = sgl_dungeon_info_array.size();
		w.write_uint16(__sgl_dungeon_info_array_vec_size);
		for(uint16_t i = 0; i < __sgl_dungeon_info_array_vec_size; ++i) {
				sgl_dungeon_info_array[i].serialize(w);
		}

		uint16_t __chapter_award_vec_vec_size = chapter_award_vec.size();
		w.write_uint16(__chapter_award_vec_vec_size);
		for(uint16_t i = 0; i < __chapter_award_vec_vec_size; ++i) {
				w.write_int8(chapter_award_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __sgl_dungeon_info_array_vec_size;
		if(r.read_uint16(__sgl_dungeon_info_array_vec_size)  )
				return -1;
		sgl_dungeon_info_array.reserve(__sgl_dungeon_info_array_vec_size);
		for(uint16_t i = 0; i < __sgl_dungeon_info_array_vec_size; ++i) {
				Single_Dungeon_Info v;
				if(v.deserialize(r))
						return -1;
				sgl_dungeon_info_array.push_back(v);
		}

		uint16_t __chapter_award_vec_vec_size;
		if(r.read_uint16(__chapter_award_vec_vec_size)  )
				return -1;
		chapter_award_vec.reserve(__chapter_award_vec_vec_size);
		for(uint16_t i = 0; i < __chapter_award_vec_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				chapter_award_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100200;
		sgl_dungeon_info_array.clear();
		chapter_award_vec.clear();
	}
};

/*
返回副本通关结算
*/
struct MSG_50100203 : public Base_Msg  {
	std::vector<Reward_Info> reward_list;
	std::vector<Reward_Info> first_reward_list;
	int32_t scene_id;
	MSG_50100203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __reward_list_vec_size = reward_list.size();
		w.write_uint16(__reward_list_vec_size);
		for(uint16_t i = 0; i < __reward_list_vec_size; ++i) {
				reward_list[i].serialize(w);
		}

		uint16_t __first_reward_list_vec_size = first_reward_list.size();
		w.write_uint16(__first_reward_list_vec_size);
		for(uint16_t i = 0; i < __first_reward_list_vec_size; ++i) {
				first_reward_list[i].serialize(w);
		}

		w.write_int32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __reward_list_vec_size;
		if(r.read_uint16(__reward_list_vec_size)  )
				return -1;
		reward_list.reserve(__reward_list_vec_size);
		for(uint16_t i = 0; i < __reward_list_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				reward_list.push_back(v);
		}

		uint16_t __first_reward_list_vec_size;
		if(r.read_uint16(__first_reward_list_vec_size)  )
				return -1;
		first_reward_list.reserve(__first_reward_list_vec_size);
		for(uint16_t i = 0; i < __first_reward_list_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				first_reward_list.push_back(v);
		}

		if( r.read_int32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100203;
		reward_list.clear();
		first_reward_list.clear();
		scene_id = 0;
	}
};

/*
请求翻牌
*/
struct MSG_10100201  {
	int32_t index;
	int8_t type;
	MSG_10100201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		type = 0;
	}
};

/*
请求翻牌(返回)
并会广播给其它人81000080
*/
struct MSG_50100201 : public Base_Msg  {
	std::vector<Reward_Info> award;
	int8_t type;
	MSG_50100201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __award_vec_size = award.size();
		w.write_uint16(__award_vec_size);
		for(uint16_t i = 0; i < __award_vec_size; ++i) {
				award[i].serialize(w);
		}

		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __award_vec_size;
		if(r.read_uint16(__award_vec_size)  )
				return -1;
		award.reserve(__award_vec_size);
		for(uint16_t i = 0; i < __award_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				award.push_back(v);
		}

		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100201;
		award.clear();
		type = 0;
	}
};

/*
请求精英副本信息
*/
struct MSG_10100202  {
	int8_t type;
	MSG_10100202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
请求精英副本信息返回
*/
struct MSG_50100202 : public Base_Msg  {
	int8_t reset_times;
	std::vector<Elite_Dungeon_Info> elite_nor_vec;
	int8_t nor_wipe_activate;
	int8_t hor_wipe_activate;
	int32_t wipe_elite_double_state;
	int8_t wipe_elite_double_ui_selected;
	int64_t role_id_1;
	std::string role_name_1;
	int64_t role_id_2;
	std::string role_name_2;
	int64_t role_id_3;
	std::string role_name_3;
	uint8_t add_num;
	uint8_t add_buy_num;
	uint8_t add_num_max;
	uint8_t challenge_num;
	uint8_t challenge_num_max;
	int8_t type;
	MSG_50100202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(reset_times);
		uint16_t __elite_nor_vec_vec_size = elite_nor_vec.size();
		w.write_uint16(__elite_nor_vec_vec_size);
		for(uint16_t i = 0; i < __elite_nor_vec_vec_size; ++i) {
				elite_nor_vec[i].serialize(w);
		}

		w.write_int8(nor_wipe_activate);
		w.write_int8(hor_wipe_activate);
		w.write_int32(wipe_elite_double_state);
		w.write_int8(wipe_elite_double_ui_selected);
		w.write_int64(role_id_1);
		w.write_string(role_name_1);
		w.write_int64(role_id_2);
		w.write_string(role_name_2);
		w.write_int64(role_id_3);
		w.write_string(role_name_3);
		w.write_uint8(add_num);
		w.write_uint8(add_buy_num);
		w.write_uint8(add_num_max);
		w.write_uint8(challenge_num);
		w.write_uint8(challenge_num_max);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(reset_times)  )
				return -1;
 		uint16_t __elite_nor_vec_vec_size;
		if(r.read_uint16(__elite_nor_vec_vec_size)  )
				return -1;
		elite_nor_vec.reserve(__elite_nor_vec_vec_size);
		for(uint16_t i = 0; i < __elite_nor_vec_vec_size; ++i) {
				Elite_Dungeon_Info v;
				if(v.deserialize(r))
						return -1;
				elite_nor_vec.push_back(v);
		}

		if( r.read_int8(nor_wipe_activate)  ||  r.read_int8(hor_wipe_activate)  ||  r.read_int32(wipe_elite_double_state)  ||  r.read_int8(wipe_elite_double_ui_selected)  ||  r.read_int64(role_id_1)  ||  r.read_string(role_name_1)  ||  r.read_int64(role_id_2)  ||  r.read_string(role_name_2)  ||  r.read_int64(role_id_3)  ||  r.read_string(role_name_3)  ||  r.read_uint8(add_num)  ||  r.read_uint8(add_buy_num)  ||  r.read_uint8(add_num_max)  ||  r.read_uint8(challenge_num)  ||  r.read_uint8(challenge_num_max)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100202;
		reset_times = 0;
		elite_nor_vec.clear();
		nor_wipe_activate = 0;
		hor_wipe_activate = 0;
		wipe_elite_double_state = 0;
		wipe_elite_double_ui_selected = 0;
		role_id_1 = 0;
		role_name_1.clear();
		role_id_2 = 0;
		role_name_2.clear();
		role_id_3 = 0;
		role_name_3.clear();
		add_num = 0;
		add_buy_num = 0;
		add_num_max = 0;
		challenge_num = 0;
		challenge_num_max = 0;
		type = 0;
	}
};

/*
请求重置精英副本
*/
struct MSG_10100204  {
	int8_t bind_gold_first;
	MSG_10100204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(bind_gold_first);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(bind_gold_first)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		bind_gold_first = 0;
	}
};

/*
请求重置精英副本返回
*/
struct MSG_50100204 : public Base_Msg  {
	int8_t result;
	MSG_50100204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100204;
		result = 0;
	}
};

/*
请求领取单人副本章节奖励
*/
struct MSG_10100205  {
	int8_t chapter;
	MSG_10100205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(chapter);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(chapter)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		chapter = 0;
	}
};

/*
请求领取单人副本章节奖励返回
*/
struct MSG_50100205 : public Base_Msg  {
	int8_t chapter;
	int8_t result;
	MSG_50100205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(chapter);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(chapter)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100205;
		chapter = 0;
		result = 0;
	}
};

/*
副本结算面板关闭通知
*/
struct MSG_10200200  {
	MSG_10200200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求扫荡单人副本
*/
struct MSG_10100210  {
	int32_t scene_id;
	uint8_t num;
	int8_t wipe_type;
	MSG_10100210(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
		w.write_uint8(num);
		w.write_int8(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  ||  r.read_uint8(num)  ||  r.read_int8(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		scene_id = 0;
		num = 0;
		wipe_type = 0;
	}
};

/*
请求扫荡单人副本返回
*/
struct MSG_50100210 : public Base_Msg  {
	int32_t remain_time;
	std::vector<Wipe_Info> wipe_info;
	int16_t remain_num;
	int8_t wipe_type;
	MSG_50100210(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(remain_time);
		uint16_t __wipe_info_vec_size = wipe_info.size();
		w.write_uint16(__wipe_info_vec_size);
		for(uint16_t i = 0; i < __wipe_info_vec_size; ++i) {
				wipe_info[i].serialize(w);
		}

		w.write_int16(remain_num);
		w.write_int8(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(remain_time)  )
				return -1;
 		uint16_t __wipe_info_vec_size;
		if(r.read_uint16(__wipe_info_vec_size)  )
				return -1;
		wipe_info.reserve(__wipe_info_vec_size);
		for(uint16_t i = 0; i < __wipe_info_vec_size; ++i) {
				Wipe_Info v;
				if(v.deserialize(r))
						return -1;
				wipe_info.push_back(v);
		}

		if( r.read_int16(remain_num)  ||  r.read_int8(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100210;
		remain_time = 0;
		wipe_info.clear();
		remain_num = 0;
		wipe_type = 0;
	}
};

/*
请求中断扫荡单人副本
*/
struct MSG_10100211  {
	int8_t wipe_type;
	MSG_10100211(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		wipe_type = 0;
	}
};

/*
请求中断扫荡单人副本返回
*/
struct MSG_50100211 : public Base_Msg  {
	int8_t result;
	int8_t wipe_type;
	MSG_50100211(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int8(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int8(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100211;
		result = 0;
		wipe_type = 0;
	}
};

/*
请求完成扫荡单人副本
*/
struct MSG_10100212  {
	int8_t wipe_type;
	MSG_10100212(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		wipe_type = 0;
	}
};

/*
请求完成扫荡单人副本返回
*/
struct MSG_50100212 : public Base_Msg  {
	int8_t result;
	int8_t wipe_type;
	int32_t fast_sweep_num;
	MSG_50100212(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int8(wipe_type);
		w.write_int32(fast_sweep_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int8(wipe_type)  ||  r.read_int32(fast_sweep_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100212;
		result = 0;
		wipe_type = 0;
		fast_sweep_num = 0;
	}
};

/*
请求获得双倍扫荡收益
*/
struct MSG_10100213  {
	int32_t wipe_type;
	MSG_10100213(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		wipe_type = 0;
	}
};

/*
请求获得双倍扫荡收益(返回)
*/
struct MSG_50100213 : public Base_Msg  {
	int32_t wipe_type;
	int32_t result;
	MSG_50100213(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(wipe_type);
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(wipe_type)  ||  r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100213;
		wipe_type = 0;
		result = 0;
	}
};

/*
请求勾选双倍扫荡
*/
struct MSG_10100214  {
	int32_t wipe_type;
	int32_t is_selected;
	MSG_10100214(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(wipe_type);
		w.write_int32(is_selected);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(wipe_type)  ||  r.read_int32(is_selected)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		wipe_type = 0;
		is_selected = 0;
	}
};

/*
请求勾选双倍扫荡(返回)
*/
struct MSG_50100214 : public Base_Msg  {
	int32_t wipe_type;
	int32_t is_selected;
	MSG_50100214(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(wipe_type);
		w.write_int32(is_selected);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(wipe_type)  ||  r.read_int32(is_selected)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100214;
		wipe_type = 0;
		is_selected = 0;
	}
};

/*
请求购买上古副本额外次数
*/
struct MSG_10100206  {
	int32_t scene_id;
	MSG_10100206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		scene_id = 0;
	}
};

/*
请求购买上古副本额外次数返回
*/
struct MSG_50100206 : public Base_Msg  {
	int32_t scene_id;
	int8_t result;
	uint8_t add_num;
	MSG_50100206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
		w.write_int8(result);
		w.write_uint8(add_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  ||  r.read_int8(result)  ||  r.read_uint8(add_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100206;
		scene_id = 0;
		result = 0;
		add_num = 0;
	}
};

/*
请求上古副本传送
*/
struct MSG_10100207  {
	int32_t dest_scene;
	MSG_10100207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(dest_scene);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(dest_scene)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		dest_scene = 0;
	}
};

/*
请求单人副本复活
*/
struct MSG_10200215  {
	int8_t type;
	MSG_10200215(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
请求单人副本复活返回
*/
struct MSG_50200215 : public Base_Msg  {
	int8_t result;
	MSG_50200215(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200215;
		result = 0;
	}
};

/*
请求领取上古魔神首通奖励
*/
struct MSG_10100208  {
	MSG_10100208(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取上古魔神首通奖励返回
*/
struct MSG_50100208 : public Base_Msg  {
	int8_t result;
	MSG_50100208(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100208;
		result = 0;
	}
};



#endif