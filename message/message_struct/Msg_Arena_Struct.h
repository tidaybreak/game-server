/*
* Generate by devtool
*/

#ifndef _MSG_ARENA_H_
#define _MSG_ARENA_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取竞技场本服排行
*/
struct MSG_10100400  {
	int8_t type;
	MSG_10100400(void) { reset(); };

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
获取竞技场本服排行返回
*/
struct MSG_50100400 : public Base_Msg  {
	uint32_t rank;
	std::vector<Arena_Rank_Info> rank_info_vec;
	int8_t type;
	MSG_50100400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(rank);
		uint16_t __rank_info_vec_vec_size = rank_info_vec.size();
		w.write_uint16(__rank_info_vec_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_vec_size; ++i) {
				rank_info_vec[i].serialize(w);
		}

		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(rank)  )
				return -1;
 		uint16_t __rank_info_vec_vec_size;
		if(r.read_uint16(__rank_info_vec_vec_size)  )
				return -1;
		rank_info_vec.reserve(__rank_info_vec_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_vec_size; ++i) {
				Arena_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				rank_info_vec.push_back(v);
		}

		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100400;
		rank = 0;
		rank_info_vec.clear();
		type = 0;
	}
};

/*
获取竞技场信息
*/
struct MSG_10100405  {
	MSG_10100405(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取竞技场信息返回
*/
struct MSG_50100405 : public Base_Msg  {
	Arena_Info season_info;
	Arena_Personal_Info personal_info;
	MSG_50100405(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		season_info.serialize(w);
		personal_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( season_info.deserialize(r)  ||  personal_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100405;
		season_info.reset();
		personal_info.reset();
	}
};

/*
关闭竞技场面板
*/
struct MSG_10100406  {
	MSG_10100406(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求匹配竞技场战斗
*/
struct MSG_10100410  {
	MSG_10100410(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求匹配竞技场战斗返回
*/
struct MSG_50100410 : public Base_Msg  {
	uint8_t career;
	uint8_t gender;
	std::string name;
	uint16_t level;
	uint8_t group;
	uint8_t seg;
	uint16_t score;
	uint16_t fight;
	int32_t hero_id;
	std::string hero_name;
	uint16_t hero_level;
	uint32_t head_id;
	std::string server_name;
	MSG_50100410(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(career);
		w.write_uint8(gender);
		w.write_string(name);
		w.write_uint16(level);
		w.write_uint8(group);
		w.write_uint8(seg);
		w.write_uint16(score);
		w.write_uint16(fight);
		w.write_int32(hero_id);
		w.write_string(hero_name);
		w.write_uint16(hero_level);
		w.write_uint32(head_id);
		w.write_string(server_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(career)  ||  r.read_uint8(gender)  ||  r.read_string(name)  ||  r.read_uint16(level)  ||  r.read_uint8(group)  ||  r.read_uint8(seg)  ||  r.read_uint16(score)  ||  r.read_uint16(fight)  ||  r.read_int32(hero_id)  ||  r.read_string(hero_name)  ||  r.read_uint16(hero_level)  ||  r.read_uint32(head_id)  ||  r.read_string(server_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100410;
		career = 0;
		gender = 0;
		name.clear();
		level = 0;
		group = 0;
		seg = 0;
		score = 0;
		fight = 0;
		hero_id = 0;
		hero_name.clear();
		hero_level = 0;
		head_id = 0;
		server_name.clear();
	}
};

/*
请求竞技场购买付费挑战次数
*/
struct MSG_10100420  {
	uint8_t buy_type;
	MSG_10100420(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(buy_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(buy_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		buy_type = 0;
	}
};

/*
请求购买竞技场付费挑战次数返回
*/
struct MSG_50100420 : public Base_Msg  {
	uint8_t pay_times;
	uint8_t buy_pay_times;
	uint8_t result;
	MSG_50100420(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(pay_times);
		w.write_uint8(buy_pay_times);
		w.write_uint8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(pay_times)  ||  r.read_uint8(buy_pay_times)  ||  r.read_uint8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100420;
		pay_times = 0;
		buy_pay_times = 0;
		result = 0;
	}
};

/*
取消匹配竞技场战斗
*/
struct MSG_10100411  {
	MSG_10100411(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
取消匹配竞技场战斗返回
*/
struct MSG_50100411 : public Base_Msg  {
	uint8_t result;
	MSG_50100411(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100411;
		result = 0;
	}
};

/*
领取竞技场3天或者7天奖励
*/
struct MSG_10100401  {
	int8_t type;
	MSG_10100401(void) { reset(); };

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
领取竞技场3天或者7天奖励返回
*/
struct MSG_50100401 : public Base_Msg  {
	uint16_t three_day_rank;
	uint16_t seven_day_rank;
	uint8_t result;
	int8_t type;
	MSG_50100401(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(three_day_rank);
		w.write_uint16(seven_day_rank);
		w.write_uint8(result);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(three_day_rank)  ||  r.read_uint16(seven_day_rank)  ||  r.read_uint8(result)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100401;
		three_day_rank = 0;
		seven_day_rank = 0;
		result = 0;
		type = 0;
	}
};

/*
获取竞技场玩家奖励信息
*/
struct MSG_10100402  {
	MSG_10100402(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取竞技场玩家奖励信息返回
*/
struct MSG_50100402 : public Base_Msg  {
	uint16_t local_rank;
	uint32_t next_local_award_time;
	uint16_t season_rank;
	uint8_t season_group;
	uint8_t season_level;
	MSG_50100402(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(local_rank);
		w.write_uint32(next_local_award_time);
		w.write_uint16(season_rank);
		w.write_uint8(season_group);
		w.write_uint8(season_level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(local_rank)  ||  r.read_uint32(next_local_award_time)  ||  r.read_uint16(season_rank)  ||  r.read_uint8(season_group)  ||  r.read_uint8(season_level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100402;
		local_rank = 0;
		next_local_award_time = 0;
		season_rank = 0;
		season_group = 0;
		season_level = 0;
	}
};

/*
请求重置竞技场cd
*/
struct MSG_10100407  {
	MSG_10100407(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求重置竞技场cd返回
*/
struct MSG_50100407 : public Base_Msg  {
	int8_t result;
	MSG_50100407(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100407;
		result = 0;
	}
};

/*
请求竞技场匹配
*/
struct MSG_10100412  {
	MSG_10100412(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求竞技场匹配返回
*/
struct MSG_50100412 : public Base_Msg  {
	std::vector<Arena_Match_Info> match_info;
	int32_t remain_time;
	MSG_50100412(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __match_info_vec_size = match_info.size();
		w.write_uint16(__match_info_vec_size);
		for(uint16_t i = 0; i < __match_info_vec_size; ++i) {
				match_info[i].serialize(w);
		}

		w.write_int32(remain_time);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __match_info_vec_size;
		if(r.read_uint16(__match_info_vec_size)  )
				return -1;
		match_info.reserve(__match_info_vec_size);
		for(uint16_t i = 0; i < __match_info_vec_size; ++i) {
				Arena_Match_Info v;
				if(v.deserialize(r))
						return -1;
				match_info.push_back(v);
		}

		if( r.read_int32(remain_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100412;
		match_info.clear();
		remain_time = 0;
	}
};

/*
请求竞技场战斗
*/
struct MSG_10100413  {
	int8_t index;
	MSG_10100413(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
	}
};

/*
请求竞技场战斗返回
*/
struct MSG_50100413 : public Base_Msg  {
	int8_t result;
	MSG_50100413(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100413;
		result = 0;
	}
};

/*
请求竞技场刷新匹配
*/
struct MSG_10100414  {
	MSG_10100414(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求竞技场刷新匹配返回
*/
struct MSG_50100414 : public Base_Msg  {
	int8_t result;
	MSG_50100414(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100414;
		result = 0;
	}
};

/*
竞技场查看人物面板
*/
struct MSG_10100415  {
	int64_t role_id;
	int8_t index;
	MSG_10100415(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		index = 0;
	}
};

/*
竞技场查看人物面板返回
*/
struct MSG_50100415 : public Base_Msg  {
	int64_t role_id;
	std::string role_name;
	std::string gang_name;
	uint8_t gender;
	int64_t gang_id;
	std::vector<Property> prop_value;
	MSG_50100415(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_string(gang_name);
		w.write_uint8(gender);
		w.write_int64(gang_id);
		uint16_t __prop_value_vec_size = prop_value.size();
		w.write_uint16(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				prop_value[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_string(gang_name)  ||  r.read_uint8(gender)  ||  r.read_int64(gang_id)  )
				return -1;
 		uint16_t __prop_value_vec_size;
		if(r.read_uint16(__prop_value_vec_size)  )
				return -1;
		prop_value.reserve(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				Property v;
				if(v.deserialize(r))
						return -1;
				prop_value.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100415;
		role_id = 0;
		role_name.clear();
		gang_name.clear();
		gender = 0;
		gang_id = 0;
		prop_value.clear();
	}
};

/*
请求竞技场面板信息返回
*/
struct MSG_50100408 : public Base_Msg  {
	int32_t single_remain_day;
	int32_t single_remain_num;
	int32_t team_remain_num;
	int32_t war_remain_num;
	int32_t rank;
	Arena_Rank_Info first_player;
	std::vector<int32_t> first_avatar;
	int8_t first_career;
	int8_t first_gender;
	int32_t first_be_worship;
	int8_t arena_worship;
	int32_t be_worship;
	MSG_50100408(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(single_remain_day);
		w.write_int32(single_remain_num);
		w.write_int32(team_remain_num);
		w.write_int32(war_remain_num);
		w.write_int32(rank);
		first_player.serialize(w);
		uint16_t __first_avatar_vec_size = first_avatar.size();
		w.write_uint16(__first_avatar_vec_size);
		for(uint16_t i = 0; i < __first_avatar_vec_size; ++i) {
				w.write_int32(first_avatar[i]);
		}

		w.write_int8(first_career);
		w.write_int8(first_gender);
		w.write_int32(first_be_worship);
		w.write_int8(arena_worship);
		w.write_int32(be_worship);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(single_remain_day)  ||  r.read_int32(single_remain_num)  ||  r.read_int32(team_remain_num)  ||  r.read_int32(war_remain_num)  ||  r.read_int32(rank)  ||  first_player.deserialize(r)  )
				return -1;
 		uint16_t __first_avatar_vec_size;
		if(r.read_uint16(__first_avatar_vec_size)  )
				return -1;
		first_avatar.reserve(__first_avatar_vec_size);
		for(uint16_t i = 0; i < __first_avatar_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				first_avatar.push_back(v);
		}

		if( r.read_int8(first_career)  ||  r.read_int8(first_gender)  ||  r.read_int32(first_be_worship)  ||  r.read_int8(arena_worship)  ||  r.read_int32(be_worship)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100408;
		single_remain_day = 0;
		single_remain_num = 0;
		team_remain_num = 0;
		war_remain_num = 0;
		rank = 0;
		first_player.reset();
		first_avatar.clear();
		first_career = 0;
		first_gender = 0;
		first_be_worship = 0;
		arena_worship = 0;
		be_worship = 0;
	}
};

/*
请求竞技场面板信息
*/
struct MSG_10100408  {
	MSG_10100408(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
竞技场膜拜
*/
struct MSG_10100416  {
	int64_t role_id;
	int8_t worship_type;
	MSG_10100416(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(worship_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(worship_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		worship_type = 0;
	}
};

/*
竞技场膜拜返回
*/
struct MSG_50100416 : public Base_Msg  {
	int8_t arena_worship;
	int32_t be_worship;
	MSG_50100416(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(arena_worship);
		w.write_int32(be_worship);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(arena_worship)  ||  r.read_int32(be_worship)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100416;
		arena_worship = 0;
		be_worship = 0;
	}
};

/*
竞技场打开面板后刷新
*/
struct MSG_10100417  {
	MSG_10100417(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
竞技场打开面板后刷新返回
*/
struct MSG_50100417 : public Base_Msg  {
	Arena_Rank_Info first_arena_player;
	std::vector<int32_t> avatar_vec;
	int8_t career;
	int8_t gender;
	int32_t be_worship;
	MSG_50100417(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		first_arena_player.serialize(w);
		uint16_t __avatar_vec_vec_size = avatar_vec.size();
		w.write_uint16(__avatar_vec_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_vec_size; ++i) {
				w.write_int32(avatar_vec[i]);
		}

		w.write_int8(career);
		w.write_int8(gender);
		w.write_int32(be_worship);
	}

	int deserialize(Block_Buffer & r) {
		if( first_arena_player.deserialize(r)  )
				return -1;
 		uint16_t __avatar_vec_vec_size;
		if(r.read_uint16(__avatar_vec_vec_size)  )
				return -1;
		avatar_vec.reserve(__avatar_vec_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				avatar_vec.push_back(v);
		}

		if( r.read_int8(career)  ||  r.read_int8(gender)  ||  r.read_int32(be_worship)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100417;
		first_arena_player.reset();
		avatar_vec.clear();
		career = 0;
		gender = 0;
		be_worship = 0;
	}
};



#endif