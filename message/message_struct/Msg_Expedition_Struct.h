/*
* Generate by devtool
*/

#ifndef _MSG_EXPEDITION_H_
#define _MSG_EXPEDITION_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求获取每日奖励
*/
struct MSG_10201300  {
	MSG_10201300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求每日奖励信息
*/
struct MSG_10201301  {
	MSG_10201301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求每日奖励信息(返回)
*/
struct MSG_50201301 : public Base_Msg  {
	int8_t state;
	std::vector<Item_Info> award_info;
	MSG_50201301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
		uint16_t __award_info_vec_size = award_info.size();
		w.write_uint16(__award_info_vec_size);
		for(uint16_t i = 0; i < __award_info_vec_size; ++i) {
				award_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  )
				return -1;
 		uint16_t __award_info_vec_size;
		if(r.read_uint16(__award_info_vec_size)  )
				return -1;
		award_info.reserve(__award_info_vec_size);
		for(uint16_t i = 0; i < __award_info_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				award_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50201301;
		state = 0;
		award_info.clear();
	}
};

/*
请求建筑信息(非公会基地)
*/
struct MSG_10201302  {
	int32_t point_id;
	int16_t line;
	MSG_10201302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int16(line);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int16(line)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
		line = 0;
	}
};

/*
请求建筑信息(非公会基地)(返回)
state: 
 0.未被初始化状态, 1.中立(和平状态), 2.中立(攻占状态), 11.占领(和平状态), 12.占领(攻占状态)
*/
struct MSG_50201302 : public Base_Msg  {
	int32_t point_id;
	int32_t line;
	int8_t type;
	int8_t state;
	std::string attack_name;
	int16_t attack_nums;
	std::string defend_name;
	int16_t defend_nums;
	std::vector<Expedition_Defend_Info> defend_info;
	int32_t collect;
	MSG_50201302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int32(line);
		w.write_int8(type);
		w.write_int8(state);
		w.write_string(attack_name);
		w.write_int16(attack_nums);
		w.write_string(defend_name);
		w.write_int16(defend_nums);
		uint16_t __defend_info_vec_size = defend_info.size();
		w.write_uint16(__defend_info_vec_size);
		for(uint16_t i = 0; i < __defend_info_vec_size; ++i) {
				defend_info[i].serialize(w);
		}

		w.write_int32(collect);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int32(line)  ||  r.read_int8(type)  ||  r.read_int8(state)  ||  r.read_string(attack_name)  ||  r.read_int16(attack_nums)  ||  r.read_string(defend_name)  ||  r.read_int16(defend_nums)  )
				return -1;
 		uint16_t __defend_info_vec_size;
		if(r.read_uint16(__defend_info_vec_size)  )
				return -1;
		defend_info.reserve(__defend_info_vec_size);
		for(uint16_t i = 0; i < __defend_info_vec_size; ++i) {
				Expedition_Defend_Info v;
				if(v.deserialize(r))
						return -1;
				defend_info.push_back(v);
		}

		if( r.read_int32(collect)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201302;
		point_id = 0;
		line = 0;
		type = 0;
		state = 0;
		attack_name.clear();
		attack_nums = 0;
		defend_name.clear();
		defend_nums = 0;
		defend_info.clear();
		collect = 0;
	}
};

/*
请求公会排名
城堡+野兽区+宝箱区 
公会基地
*/
struct MSG_10201303  {
	MSG_10201303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求公会排名(返回)
城堡+野兽区+宝箱区 
公会基地
*/
struct MSG_50201303 : public Base_Msg  {
	std::vector<Expedition_Gang_Rank_Info> rank_info;
	int32_t time;
	MSG_50201303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __rank_info_vec_size = rank_info.size();
		w.write_uint16(__rank_info_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_size; ++i) {
				rank_info[i].serialize(w);
		}

		w.write_int32(time);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __rank_info_vec_size;
		if(r.read_uint16(__rank_info_vec_size)  )
				return -1;
		rank_info.reserve(__rank_info_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_size; ++i) {
				Expedition_Gang_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				rank_info.push_back(v);
		}

		if( r.read_int32(time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201303;
		rank_info.clear();
		time = 0;
	}
};

/*
请求对战信息
*/
struct MSG_10201304  {
	int32_t point_id;
	int16_t line;
	MSG_10201304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int16(line);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int16(line)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
		line = 0;
	}
};

/*
请求对战信息(返回)
*/
struct MSG_50201304 : public Base_Msg  {
	int32_t point_id;
	int16_t line;
	std::vector<Expedition_Battle_Role_Info> attack_list;
	std::vector<Expedition_Battle_Role_Info> defend_list;
	MSG_50201304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int16(line);
		uint16_t __attack_list_vec_size = attack_list.size();
		w.write_uint16(__attack_list_vec_size);
		for(uint16_t i = 0; i < __attack_list_vec_size; ++i) {
				attack_list[i].serialize(w);
		}

		uint16_t __defend_list_vec_size = defend_list.size();
		w.write_uint16(__defend_list_vec_size);
		for(uint16_t i = 0; i < __defend_list_vec_size; ++i) {
				defend_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int16(line)  )
				return -1;
 		uint16_t __attack_list_vec_size;
		if(r.read_uint16(__attack_list_vec_size)  )
				return -1;
		attack_list.reserve(__attack_list_vec_size);
		for(uint16_t i = 0; i < __attack_list_vec_size; ++i) {
				Expedition_Battle_Role_Info v;
				if(v.deserialize(r))
						return -1;
				attack_list.push_back(v);
		}

		uint16_t __defend_list_vec_size;
		if(r.read_uint16(__defend_list_vec_size)  )
				return -1;
		defend_list.reserve(__defend_list_vec_size);
		for(uint16_t i = 0; i < __defend_list_vec_size; ++i) {
				Expedition_Battle_Role_Info v;
				if(v.deserialize(r))
						return -1;
				defend_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50201304;
		point_id = 0;
		line = 0;
		attack_list.clear();
		defend_list.clear();
	}
};

/*
请求公会基地信息
*/
struct MSG_10201305  {
	int32_t point_id;
	int16_t line;
	MSG_10201305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int16(line);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int16(line)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
		line = 0;
	}
};

/*
请求公会基地信息(返回)
*/
struct MSG_50201305 : public Base_Msg  {
	int32_t point_id;
	int16_t line;
	Expedition_Gang_Base_Place_Info base_info;
	int8_t state;
	MSG_50201305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int16(line);
		base_info.serialize(w);
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int16(line)  ||  base_info.deserialize(r)  ||  r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201305;
		point_id = 0;
		line = 0;
		base_info.reset();
		state = 0;
	}
};

/*
请求进出战斗队列
*/
struct MSG_10201306  {
	int32_t point_id;
	int8_t type;
	MSG_10201306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
		type = 0;
	}
};

/*
请求进出战斗队列(返回)
*/
struct MSG_50201306 : public Base_Msg  {
	int8_t result;
	int8_t state;
	int32_t point_id;
	MSG_50201306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int8(state);
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int8(state)  ||  r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201306;
		result = 0;
		state = 0;
		point_id = 0;
	}
};

/*
请求援兵信息
*/
struct MSG_10201307  {
	int32_t point_id;
	MSG_10201307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
	}
};

/*
请求援兵信息(返回)
*/
struct MSG_50201307 : public Base_Msg  {
	std::vector<Expedition_Help_Info> self;
	std::vector<Expedition_Help_Info> enemy;
	int32_t point_id;
	MSG_50201307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __self_vec_size = self.size();
		w.write_uint16(__self_vec_size);
		for(uint16_t i = 0; i < __self_vec_size; ++i) {
				self[i].serialize(w);
		}

		uint16_t __enemy_vec_size = enemy.size();
		w.write_uint16(__enemy_vec_size);
		for(uint16_t i = 0; i < __enemy_vec_size; ++i) {
				enemy[i].serialize(w);
		}

		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __self_vec_size;
		if(r.read_uint16(__self_vec_size)  )
				return -1;
		self.reserve(__self_vec_size);
		for(uint16_t i = 0; i < __self_vec_size; ++i) {
				Expedition_Help_Info v;
				if(v.deserialize(r))
						return -1;
				self.push_back(v);
		}

		uint16_t __enemy_vec_size;
		if(r.read_uint16(__enemy_vec_size)  )
				return -1;
		enemy.reserve(__enemy_vec_size);
		for(uint16_t i = 0; i < __enemy_vec_size; ++i) {
				Expedition_Help_Info v;
				if(v.deserialize(r))
						return -1;
				enemy.push_back(v);
		}

		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201307;
		self.clear();
		enemy.clear();
		point_id = 0;
	}
};

/*
远征换线
*/
struct MSG_10201308  {
	int16_t from;
	int16_t to;
	MSG_10201308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(from);
		w.write_int16(to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(from)  ||  r.read_int16(to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		from = 0;
		to = 0;
	}
};

/*
远征换线(返回)
*/
struct MSG_50201308 : public Base_Msg  {
	MSG_50201308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50201308;
	}
};

/*
远征请求宝箱点领取
*/
struct MSG_10101301  {
	int32_t point_id;
	MSG_10101301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
	}
};

/*
远征请求宝箱点领取(返回)
*/
struct MSG_50101301 : public Base_Msg  {
	int32_t point_id;
	int8_t result;
	MSG_50101301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101301;
		point_id = 0;
		result = 0;
	}
};

/*
请求获取每日奖励(返回)
*/
struct MSG_50201300 : public Base_Msg  {
	int8_t result;
	MSG_50201300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201300;
		result = 0;
	}
};

/*
远征打开宝箱区
*/
struct MSG_10101300  {
	int32_t point_id;
	MSG_10101300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
	}
};

/*
远征打开宝箱区(返回)
*/
struct MSG_50101300 : public Base_Msg  {
	int32_t point_id;
	std::vector<Reward_Info> list;
	MSG_50101300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101300;
		point_id = 0;
		list.clear();
	}
};

/*
远征进入待战列表(返回)
*/
struct MSG_50201310 : public Base_Msg  {
	int32_t point_id;
	int8_t result;
	MSG_50201310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201310;
		point_id = 0;
		result = 0;
	}
};

/*
远征进入待战列表
*/
struct MSG_10201310  {
	int32_t point_id;
	int8_t type;
	MSG_10201310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
		type = 0;
	}
};

/*
远征伤害排名
*/
struct MSG_10201309  {
	MSG_10201309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
远征伤害排名(返回)
*/
struct MSG_50201309 : public Base_Msg  {
	std::vector<Expedition_Role_Rank> rank;
	int32_t setttle_time;
	MSG_50201309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __rank_vec_size = rank.size();
		w.write_uint16(__rank_vec_size);
		for(uint16_t i = 0; i < __rank_vec_size; ++i) {
				rank[i].serialize(w);
		}

		w.write_int32(setttle_time);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __rank_vec_size;
		if(r.read_uint16(__rank_vec_size)  )
				return -1;
		rank.reserve(__rank_vec_size);
		for(uint16_t i = 0; i < __rank_vec_size; ++i) {
				Expedition_Role_Rank v;
				if(v.deserialize(r))
						return -1;
				rank.push_back(v);
		}

		if( r.read_int32(setttle_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201309;
		rank.clear();
		setttle_time = 0;
	}
};

/*
请求远征攻城榜
*/
struct MSG_10201311  {
	MSG_10201311(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求远征攻城榜(返回)
*/
struct MSG_50201311 : public Base_Msg  {
	std::vector<Expedition_Occupy_Castle> list;
	std::vector<Item_Info> award_list;
	int32_t title;
	int8_t state;
	int16_t get_nums;
	MSG_50201311(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

		uint16_t __award_list_vec_size = award_list.size();
		w.write_uint16(__award_list_vec_size);
		for(uint16_t i = 0; i < __award_list_vec_size; ++i) {
				award_list[i].serialize(w);
		}

		w.write_int32(title);
		w.write_int8(state);
		w.write_int16(get_nums);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Expedition_Occupy_Castle v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		uint16_t __award_list_vec_size;
		if(r.read_uint16(__award_list_vec_size)  )
				return -1;
		award_list.reserve(__award_list_vec_size);
		for(uint16_t i = 0; i < __award_list_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				award_list.push_back(v);
		}

		if( r.read_int32(title)  ||  r.read_int8(state)  ||  r.read_int16(get_nums)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50201311;
		list.clear();
		award_list.clear();
		title = 0;
		state = 0;
		get_nums = 0;
	}
};

/*
领取攻城奖励
*/
struct MSG_10201312  {
	MSG_10201312(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};



#endif