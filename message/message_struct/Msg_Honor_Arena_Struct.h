/*
* Generate by devtool
*/

#ifndef _MSG_HONOR_ARENA_H_
#define _MSG_HONOR_ARENA_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
荣誉竞技场请求匹配
*/
struct MSG_10211201  {
	MSG_10211201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
荣誉竞技场请求匹配返回
*/
struct MSG_50211201 : public Base_Msg  {
	Role_Info role_info;
	int32_t hero_id;
	int8_t result;
	MSG_50211201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		role_info.serialize(w);
		w.write_int32(hero_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( role_info.deserialize(r)  ||  r.read_int32(hero_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211201;
		role_info.reset();
		hero_id = 0;
		result = 0;
	}
};

/*
荣誉竞技场请求领取
*/
struct MSG_10211202  {
	int32_t type;
	MSG_10211202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
荣誉竞技场请求领取返回
*/
struct MSG_50211202 : public Base_Msg  {
	int32_t type;
	int8_t result;
	MSG_50211202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211202;
		type = 0;
		result = 0;
	}
};

/*
荣誉竞技场战斗结果返回
*/
struct MSG_50211203 : public Base_Msg  {
	int8_t result;
	int32_t rank;
	int32_t rank_up;
	int32_t score;
	int32_t score_up;
	std::vector<Item_Info> items;
	MSG_50211203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(rank);
		w.write_int32(rank_up);
		w.write_int32(score);
		w.write_int32(score_up);
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(rank)  ||  r.read_int32(rank_up)  ||  r.read_int32(score)  ||  r.read_int32(score_up)  )
				return -1;
 		uint16_t __items_vec_size;
		if(r.read_uint16(__items_vec_size)  )
				return -1;
		items.reserve(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				items.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50211203;
		result = 0;
		rank = 0;
		rank_up = 0;
		score = 0;
		score_up = 0;
		items.clear();
	}
};

/*
荣誉竞技场公告返回
*/
struct MSG_50211204 : public Base_Msg  {
	Honor_Arena_Report report;
	MSG_50211204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		report.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( report.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211204;
		report.reset();
	}
};

/*
荣誉竞技场排名返回
*/
struct MSG_50211205 : public Base_Msg  {
	int32_t self_rank;
	std::vector<Honor_Arena_Rank_Info> rank_infos;
	MSG_50211205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(self_rank);
		uint16_t __rank_infos_vec_size = rank_infos.size();
		w.write_uint16(__rank_infos_vec_size);
		for(uint16_t i = 0; i < __rank_infos_vec_size; ++i) {
				rank_infos[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(self_rank)  )
				return -1;
 		uint16_t __rank_infos_vec_size;
		if(r.read_uint16(__rank_infos_vec_size)  )
				return -1;
		rank_infos.reserve(__rank_infos_vec_size);
		for(uint16_t i = 0; i < __rank_infos_vec_size; ++i) {
				Honor_Arena_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				rank_infos.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50211205;
		self_rank = 0;
		rank_infos.clear();
	}
};

/*
荣誉竞技场匹配界面排名返回
*/
struct MSG_50211206 : public Base_Msg  {
	std::vector<Honor_Arena_Rank_Info> near_rank;
	std::vector<Role_Info_With_Avatar> top_info;
	MSG_50211206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __near_rank_vec_size = near_rank.size();
		w.write_uint16(__near_rank_vec_size);
		for(uint16_t i = 0; i < __near_rank_vec_size; ++i) {
				near_rank[i].serialize(w);
		}

		uint16_t __top_info_vec_size = top_info.size();
		w.write_uint16(__top_info_vec_size);
		for(uint16_t i = 0; i < __top_info_vec_size; ++i) {
				top_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __near_rank_vec_size;
		if(r.read_uint16(__near_rank_vec_size)  )
				return -1;
		near_rank.reserve(__near_rank_vec_size);
		for(uint16_t i = 0; i < __near_rank_vec_size; ++i) {
				Honor_Arena_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				near_rank.push_back(v);
		}

		uint16_t __top_info_vec_size;
		if(r.read_uint16(__top_info_vec_size)  )
				return -1;
		top_info.reserve(__top_info_vec_size);
		for(uint16_t i = 0; i < __top_info_vec_size; ++i) {
				Role_Info_With_Avatar v;
				if(v.deserialize(r))
						return -1;
				top_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50211206;
		near_rank.clear();
		top_info.clear();
	}
};

/*
荣誉竞技场面板信息返回
*/
struct MSG_50211207 : public Base_Msg  {
	std::vector<Int_Int> achievements;
	std::vector<Int_Int> items;
	int32_t score;
	int32_t fight_num;
	int16_t win_num;
	int8_t auto_match;
	MSG_50211207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __achievements_vec_size = achievements.size();
		w.write_uint16(__achievements_vec_size);
		for(uint16_t i = 0; i < __achievements_vec_size; ++i) {
				achievements[i].serialize(w);
		}

		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

		w.write_int32(score);
		w.write_int32(fight_num);
		w.write_int16(win_num);
		w.write_int8(auto_match);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __achievements_vec_size;
		if(r.read_uint16(__achievements_vec_size)  )
				return -1;
		achievements.reserve(__achievements_vec_size);
		for(uint16_t i = 0; i < __achievements_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				achievements.push_back(v);
		}

		uint16_t __items_vec_size;
		if(r.read_uint16(__items_vec_size)  )
				return -1;
		items.reserve(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				items.push_back(v);
		}

		if( r.read_int32(score)  ||  r.read_int32(fight_num)  ||  r.read_int16(win_num)  ||  r.read_int8(auto_match)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211207;
		achievements.clear();
		items.clear();
		score = 0;
		fight_num = 0;
		win_num = 0;
		auto_match = 0;
	}
};

/*
荣誉竞技场匹配面板信息请求
*/
struct MSG_10211207  {
	MSG_10211207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
荣誉竞技场请求排名信息
*/
struct MSG_10211205  {
	MSG_10211205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
荣誉竞技场请求设置自动匹配
*/
struct MSG_10211208  {
	int8_t is_auto;
	MSG_10211208(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_auto);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_auto)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_auto = 0;
	}
};

/*
荣誉竞技场请求设置自动匹配返回
*/
struct MSG_50211208 : public Base_Msg  {
	int8_t is_auto;
	int8_t result;
	MSG_50211208(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_auto);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_auto)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211208;
		is_auto = 0;
		result = 0;
	}
};



#endif