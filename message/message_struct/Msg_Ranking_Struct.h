/*
* Generate by devtool
*/

#ifndef _MSG_RANKING_H_
#define _MSG_RANKING_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求获取排行榜列表
*/
struct MSG_10171000  {
	int32_t type;
	MSG_10171000(void) { reset(); };

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
请求获取排行榜列表(返回)
*/
struct MSG_50171000 : public Base_Msg  {
	int32_t type;
	std::vector<Ranking_Info> ranking;
	int32_t update_time;
	MSG_50171000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		uint16_t __ranking_vec_size = ranking.size();
		w.write_uint16(__ranking_vec_size);
		for(uint16_t i = 0; i < __ranking_vec_size; ++i) {
				ranking[i].serialize(w);
		}

		w.write_int32(update_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		uint16_t __ranking_vec_size;
		if(r.read_uint16(__ranking_vec_size)  )
				return -1;
		ranking.reserve(__ranking_vec_size);
		for(uint16_t i = 0; i < __ranking_vec_size; ++i) {
				Ranking_Info v;
				if(v.deserialize(r))
						return -1;
				ranking.push_back(v);
		}

		if( r.read_int32(update_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50171000;
		type = 0;
		ranking.clear();
		update_time = 0;
	}
};

/*
请求排行榜人物信息
*/
struct MSG_10171001  {
	uint32_t rank;
	int32_t type;
	MSG_10171001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(rank);
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(rank)  ||  r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		rank = 0;
		type = 0;
	}
};

/*
请求排行榜人物信息(返回)
*/
struct MSG_50171001 : public Base_Msg  {
	int64_t role_id;
	std::vector<Property> prop_value;
	std::string role_name;
	std::string gang_name;
	uint8_t gender;
	int64_t gang_id;
	uint32_t title_id;
	int32_t exploit_val;
	int32_t exploit_lvl;
	std::vector<int32_t> avatar;
	std::vector<Item_Basic_Info> item_info;
	std::vector<Item_Equip_Info> equip_info;
	std::vector<Rune_Part_Info> rune_part_info;
	std::vector<Item_Rune_Stone_Info> rune_stone_info;
	std::vector<Master_Part_Info> master_part_info;
	int32_t hero_id;
	int32_t hero_index;
	std::vector<Item_Basic_Info> hero_item_info;
	std::vector<Item_Equip_Info> hero_equip_info;
	std::vector<Rune_Part_Info> hero_rune_part_info;
	std::vector<Item_Rune_Stone_Info> hero_rune_stone_info;
	std::vector<Property> hero_prop_value;
	MSG_50171001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __prop_value_vec_size = prop_value.size();
		w.write_uint16(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				prop_value[i].serialize(w);
		}

		w.write_string(role_name);
		w.write_string(gang_name);
		w.write_uint8(gender);
		w.write_int64(gang_id);
		w.write_uint32(title_id);
		w.write_int32(exploit_val);
		w.write_int32(exploit_lvl);
		uint16_t __avatar_vec_size = avatar.size();
		w.write_uint16(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				w.write_int32(avatar[i]);
		}

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

		uint16_t __rune_part_info_vec_size = rune_part_info.size();
		w.write_uint16(__rune_part_info_vec_size);
		for(uint16_t i = 0; i < __rune_part_info_vec_size; ++i) {
				rune_part_info[i].serialize(w);
		}

		uint16_t __rune_stone_info_vec_size = rune_stone_info.size();
		w.write_uint16(__rune_stone_info_vec_size);
		for(uint16_t i = 0; i < __rune_stone_info_vec_size; ++i) {
				rune_stone_info[i].serialize(w);
		}

		uint16_t __master_part_info_vec_size = master_part_info.size();
		w.write_uint16(__master_part_info_vec_size);
		for(uint16_t i = 0; i < __master_part_info_vec_size; ++i) {
				master_part_info[i].serialize(w);
		}

		w.write_int32(hero_id);
		w.write_int32(hero_index);
		uint16_t __hero_item_info_vec_size = hero_item_info.size();
		w.write_uint16(__hero_item_info_vec_size);
		for(uint16_t i = 0; i < __hero_item_info_vec_size; ++i) {
				hero_item_info[i].serialize(w);
		}

		uint16_t __hero_equip_info_vec_size = hero_equip_info.size();
		w.write_uint16(__hero_equip_info_vec_size);
		for(uint16_t i = 0; i < __hero_equip_info_vec_size; ++i) {
				hero_equip_info[i].serialize(w);
		}

		uint16_t __hero_rune_part_info_vec_size = hero_rune_part_info.size();
		w.write_uint16(__hero_rune_part_info_vec_size);
		for(uint16_t i = 0; i < __hero_rune_part_info_vec_size; ++i) {
				hero_rune_part_info[i].serialize(w);
		}

		uint16_t __hero_rune_stone_info_vec_size = hero_rune_stone_info.size();
		w.write_uint16(__hero_rune_stone_info_vec_size);
		for(uint16_t i = 0; i < __hero_rune_stone_info_vec_size; ++i) {
				hero_rune_stone_info[i].serialize(w);
		}

		uint16_t __hero_prop_value_vec_size = hero_prop_value.size();
		w.write_uint16(__hero_prop_value_vec_size);
		for(uint16_t i = 0; i < __hero_prop_value_vec_size; ++i) {
				hero_prop_value[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
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

		if( r.read_string(role_name)  ||  r.read_string(gang_name)  ||  r.read_uint8(gender)  ||  r.read_int64(gang_id)  ||  r.read_uint32(title_id)  ||  r.read_int32(exploit_val)  ||  r.read_int32(exploit_lvl)  )
				return -1;
 		uint16_t __avatar_vec_size;
		if(r.read_uint16(__avatar_vec_size)  )
				return -1;
		avatar.reserve(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				avatar.push_back(v);
		}

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

		uint16_t __rune_part_info_vec_size;
		if(r.read_uint16(__rune_part_info_vec_size)  )
				return -1;
		rune_part_info.reserve(__rune_part_info_vec_size);
		for(uint16_t i = 0; i < __rune_part_info_vec_size; ++i) {
				Rune_Part_Info v;
				if(v.deserialize(r))
						return -1;
				rune_part_info.push_back(v);
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

		uint16_t __master_part_info_vec_size;
		if(r.read_uint16(__master_part_info_vec_size)  )
				return -1;
		master_part_info.reserve(__master_part_info_vec_size);
		for(uint16_t i = 0; i < __master_part_info_vec_size; ++i) {
				Master_Part_Info v;
				if(v.deserialize(r))
						return -1;
				master_part_info.push_back(v);
		}

		if( r.read_int32(hero_id)  ||  r.read_int32(hero_index)  )
				return -1;
 		uint16_t __hero_item_info_vec_size;
		if(r.read_uint16(__hero_item_info_vec_size)  )
				return -1;
		hero_item_info.reserve(__hero_item_info_vec_size);
		for(uint16_t i = 0; i < __hero_item_info_vec_size; ++i) {
				Item_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				hero_item_info.push_back(v);
		}

		uint16_t __hero_equip_info_vec_size;
		if(r.read_uint16(__hero_equip_info_vec_size)  )
				return -1;
		hero_equip_info.reserve(__hero_equip_info_vec_size);
		for(uint16_t i = 0; i < __hero_equip_info_vec_size; ++i) {
				Item_Equip_Info v;
				if(v.deserialize(r))
						return -1;
				hero_equip_info.push_back(v);
		}

		uint16_t __hero_rune_part_info_vec_size;
		if(r.read_uint16(__hero_rune_part_info_vec_size)  )
				return -1;
		hero_rune_part_info.reserve(__hero_rune_part_info_vec_size);
		for(uint16_t i = 0; i < __hero_rune_part_info_vec_size; ++i) {
				Rune_Part_Info v;
				if(v.deserialize(r))
						return -1;
				hero_rune_part_info.push_back(v);
		}

		uint16_t __hero_rune_stone_info_vec_size;
		if(r.read_uint16(__hero_rune_stone_info_vec_size)  )
				return -1;
		hero_rune_stone_info.reserve(__hero_rune_stone_info_vec_size);
		for(uint16_t i = 0; i < __hero_rune_stone_info_vec_size; ++i) {
				Item_Rune_Stone_Info v;
				if(v.deserialize(r))
						return -1;
				hero_rune_stone_info.push_back(v);
		}

		uint16_t __hero_prop_value_vec_size;
		if(r.read_uint16(__hero_prop_value_vec_size)  )
				return -1;
		hero_prop_value.reserve(__hero_prop_value_vec_size);
		for(uint16_t i = 0; i < __hero_prop_value_vec_size; ++i) {
				Property v;
				if(v.deserialize(r))
						return -1;
				hero_prop_value.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50171001;
		role_id = 0;
		prop_value.clear();
		role_name.clear();
		gang_name.clear();
		gender = 0;
		gang_id = 0;
		title_id = 0;
		exploit_val = 0;
		exploit_lvl = 0;
		avatar.clear();
		item_info.clear();
		equip_info.clear();
		rune_part_info.clear();
		rune_stone_info.clear();
		master_part_info.clear();
		hero_id = 0;
		hero_index = 0;
		hero_item_info.clear();
		hero_equip_info.clear();
		hero_rune_part_info.clear();
		hero_rune_stone_info.clear();
		hero_prop_value.clear();
	}
};

/*
请求排行榜装备信息
*/
struct MSG_10171002  {
	uint32_t rank;
	int32_t type;
	int64_t roleId;
	uint32_t career;
	MSG_10171002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(rank);
		w.write_int32(type);
		w.write_int64(roleId);
		w.write_uint32(career);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(rank)  ||  r.read_int32(type)  ||  r.read_int64(roleId)  ||  r.read_uint32(career)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		rank = 0;
		type = 0;
		roleId = 0;
		career = 0;
	}
};

/*
请求排行榜装备信息(返回)
*/
struct MSG_50171002 : public Base_Msg  {
	Item_Basic_Info item_info;
	Item_Equip_Info equip_info;
	Rune_Part_Info rune_part_info;
	Master_Part_Info master_part_info;
	int64_t roleId;
	uint32_t career;
	MSG_50171002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		item_info.serialize(w);
		equip_info.serialize(w);
		rune_part_info.serialize(w);
		master_part_info.serialize(w);
		w.write_int64(roleId);
		w.write_uint32(career);
	}

	int deserialize(Block_Buffer & r) {
		if( item_info.deserialize(r)  ||  equip_info.deserialize(r)  ||  rune_part_info.deserialize(r)  ||  master_part_info.deserialize(r)  ||  r.read_int64(roleId)  ||  r.read_uint32(career)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50171002;
		item_info.reset();
		equip_info.reset();
		rune_part_info.reset();
		master_part_info.reset();
		roleId = 0;
		career = 0;
	}
};

/*
请求排行榜英雄信息
*/
struct MSG_10171003  {
	uint32_t rank;
	int32_t type;
	MSG_10171003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(rank);
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(rank)  ||  r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		rank = 0;
		type = 0;
	}
};

/*
请求排行榜英雄信息(返回)
*/
struct MSG_50171003 : public Base_Msg  {
	std::vector<Item_Basic_Info> item_info;
	std::vector<Item_Equip_Info> equip_info;
	std::vector<Rune_Part_Info> rune_part_info;
	std::vector<Property> prop_value;
	std::vector<Item_Rune_Stone_Info> rune_stone_info;
	std::vector<Master_Part_Info> master_part_info;
	MSG_50171003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
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

		uint16_t __rune_part_info_vec_size = rune_part_info.size();
		w.write_uint16(__rune_part_info_vec_size);
		for(uint16_t i = 0; i < __rune_part_info_vec_size; ++i) {
				rune_part_info[i].serialize(w);
		}

		uint16_t __prop_value_vec_size = prop_value.size();
		w.write_uint16(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				prop_value[i].serialize(w);
		}

		uint16_t __rune_stone_info_vec_size = rune_stone_info.size();
		w.write_uint16(__rune_stone_info_vec_size);
		for(uint16_t i = 0; i < __rune_stone_info_vec_size; ++i) {
				rune_stone_info[i].serialize(w);
		}

		uint16_t __master_part_info_vec_size = master_part_info.size();
		w.write_uint16(__master_part_info_vec_size);
		for(uint16_t i = 0; i < __master_part_info_vec_size; ++i) {
				master_part_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
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

		uint16_t __rune_part_info_vec_size;
		if(r.read_uint16(__rune_part_info_vec_size)  )
				return -1;
		rune_part_info.reserve(__rune_part_info_vec_size);
		for(uint16_t i = 0; i < __rune_part_info_vec_size; ++i) {
				Rune_Part_Info v;
				if(v.deserialize(r))
						return -1;
				rune_part_info.push_back(v);
		}

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

		uint16_t __master_part_info_vec_size;
		if(r.read_uint16(__master_part_info_vec_size)  )
				return -1;
		master_part_info.reserve(__master_part_info_vec_size);
		for(uint16_t i = 0; i < __master_part_info_vec_size; ++i) {
				Master_Part_Info v;
				if(v.deserialize(r))
						return -1;
				master_part_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50171003;
		item_info.clear();
		equip_info.clear();
		rune_part_info.clear();
		prop_value.clear();
		rune_stone_info.clear();
		master_part_info.clear();
	}
};



#endif