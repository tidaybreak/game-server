/*
* Generate by devtool
*/

#ifndef _MSG_ACTIVE_H_
#define _MSG_ACTIVE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
玩家出现
*/
struct MSG_81000001 : public Base_Msg  {
	Role_Appear_Info role_info;
	MSG_81000001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		role_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( role_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000001;
		role_info.reset();
	}
};

/*
对象消失(玩家/NPC)
*/
struct MSG_81000010 : public Base_Msg  {
	std::vector<int64_t> ids;
	MSG_81000010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __ids_vec_size = ids.size();
		w.write_uint16(__ids_vec_size);
		for(uint16_t i = 0; i < __ids_vec_size; ++i) {
				w.write_int64(ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __ids_vec_size;
		if(r.read_uint16(__ids_vec_size)  )
				return -1;
		ids.reserve(__ids_vec_size);
		for(uint16_t i = 0; i < __ids_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000010;
		ids.clear();
	}
};

/*
Mover(玩家/NPC/宠物)移动
*/
struct MSG_81000020 : public Base_Msg  {
	int64_t role_id;
	std::vector<Coord> coord;
	MSG_81000020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __coord_vec_size = coord.size();
		w.write_uint16(__coord_vec_size);
		for(uint16_t i = 0; i < __coord_vec_size; ++i) {
				coord[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __coord_vec_size;
		if(r.read_uint16(__coord_vec_size)  )
				return -1;
		coord.reserve(__coord_vec_size);
		for(uint16_t i = 0; i < __coord_vec_size; ++i) {
				Coord v;
				if(v.deserialize(r))
						return -1;
				coord.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000020;
		role_id = 0;
		coord.clear();
	}
};

/*
NPC出现
*/
struct MSG_81000005 : public Base_Msg  {
	NPC_Appear_Info npc_info;
	MSG_81000005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		npc_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( npc_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000005;
		npc_info.reset();
	}
};

/*
错误提示
备注：
error_id 错误号
*/
struct MSG_80000001 : public Base_Msg  {
	int32_t error_id;
	int32_t error_source;
	std::string content;
	MSG_80000001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(error_id);
		w.write_int32(error_source);
		w.write_string(content);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(error_id)  ||  r.read_int32(error_source)  ||  r.read_string(content)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80000001;
		error_id = 0;
		error_source = 0;
		content.clear();
	}
};

/*
通知关闭连接
*/
struct MSG_80000005 : public Base_Msg  {
	int32_t reason;
	MSG_80000005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(reason);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(reason)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80000005;
		reason = 0;
	}
};

/*
登录或创建角色成功时返回
*/
struct MSG_81000000 : public Base_Msg  {
	int64_t role_id;
	Server_Link_Info gate_link;
	Server_Link_Info chat_link;
	std::string session_id;
	MSG_81000000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		gate_link.serialize(w);
		chat_link.serialize(w);
		w.write_string(session_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  gate_link.deserialize(r)  ||  chat_link.deserialize(r)  ||  r.read_string(session_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000000;
		role_id = 0;
		gate_link.reset();
		chat_link.reset();
		session_id.clear();
	}
};

/*
转场景通知
*/
struct MSG_81000030 : public Base_Msg  {
	Location location;
	int8_t order_total;
	MSG_81000030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		location.serialize(w);
		w.write_int8(order_total);
	}

	int deserialize(Block_Buffer & r) {
		if( location.deserialize(r)  ||  r.read_int8(order_total)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000030;
		location.reset();
		order_total = 0;
	}
};

/*
背包物品更新
*/
struct MSG_82100000 : public Base_Msg  {
	std::vector<Item_Basic_Info> item_info;
	std::vector<int32_t> index_list;
	MSG_82100000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_info_vec_size = item_info.size();
		w.write_uint16(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				item_info[i].serialize(w);
		}

		uint16_t __index_list_vec_size = index_list.size();
		w.write_uint16(__index_list_vec_size);
		for(uint16_t i = 0; i < __index_list_vec_size; ++i) {
				w.write_int32(index_list[i]);
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

		uint16_t __index_list_vec_size;
		if(r.read_uint16(__index_list_vec_size)  )
				return -1;
		index_list.reserve(__index_list_vec_size);
		for(uint16_t i = 0; i < __index_list_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				index_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82100000;
		item_info.clear();
		index_list.clear();
	}
};

/*
背包容量改变
*/
struct MSG_82100001 : public Base_Msg  {
	int32_t type;
	int16_t capacity;
	MSG_82100001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_int16(capacity);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_int16(capacity)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82100001;
		type = 0;
		capacity = 0;
	}
};

/*
金钱更新
*/
struct MSG_82100002 : public Base_Msg  {
	Money money;
	MSG_82100002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		money.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( money.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82100002;
		money.reset();
	}
};

/*
背包物品删除
备注：在某个格子中的物品全部被删除才会发送这条消息，若格子中还剩余物品，物品信息会在背包物品更新消息中发送。
*/
struct MSG_82100003 : public Base_Msg  {
	std::vector<uint32_t> item_erased;
	MSG_82100003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_erased_vec_size = item_erased.size();
		w.write_uint16(__item_erased_vec_size);
		for(uint16_t i = 0; i < __item_erased_vec_size; ++i) {
				w.write_uint32(item_erased[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __item_erased_vec_size;
		if(r.read_uint16(__item_erased_vec_size)  )
				return -1;
		item_erased.reserve(__item_erased_vec_size);
		for(uint16_t i = 0; i < __item_erased_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				item_erased.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82100003;
		item_erased.clear();
	}
};

/*
物品附加信息更新
备注：在某个格子中的物品全部被删除才会发送这条消息，若格子中还剩余物品，物品信息会在背包物品更新消息中发送。
*/
struct MSG_82100004 : public Base_Msg  {
	int8_t type;
	std::vector<Item_Equip_Info> equip_addit;
	std::vector<Item_Rune_Stone_Info> rune_stone_addit;
	MSG_82100004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		uint16_t __equip_addit_vec_size = equip_addit.size();
		w.write_uint16(__equip_addit_vec_size);
		for(uint16_t i = 0; i < __equip_addit_vec_size; ++i) {
				equip_addit[i].serialize(w);
		}

		uint16_t __rune_stone_addit_vec_size = rune_stone_addit.size();
		w.write_uint16(__rune_stone_addit_vec_size);
		for(uint16_t i = 0; i < __rune_stone_addit_vec_size; ++i) {
				rune_stone_addit[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		uint16_t __equip_addit_vec_size;
		if(r.read_uint16(__equip_addit_vec_size)  )
				return -1;
		equip_addit.reserve(__equip_addit_vec_size);
		for(uint16_t i = 0; i < __equip_addit_vec_size; ++i) {
				Item_Equip_Info v;
				if(v.deserialize(r))
						return -1;
				equip_addit.push_back(v);
		}

		uint16_t __rune_stone_addit_vec_size;
		if(r.read_uint16(__rune_stone_addit_vec_size)  )
				return -1;
		rune_stone_addit.reserve(__rune_stone_addit_vec_size);
		for(uint16_t i = 0; i < __rune_stone_addit_vec_size; ++i) {
				Item_Rune_Stone_Info v;
				if(v.deserialize(r))
						return -1;
				rune_stone_addit.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82100004;
		type = 0;
		equip_addit.clear();
		rune_stone_addit.clear();
	}
};

/*
通知交易额度信息
*/
struct MSG_82100005 : public Base_Msg  {
	int8_t limit_type;
	int8_t trade_type;
	int32_t limit_left;
	int32_t limit_total;
	MSG_82100005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(limit_type);
		w.write_int8(trade_type);
		w.write_int32(limit_left);
		w.write_int32(limit_total);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(limit_type)  ||  r.read_int8(trade_type)  ||  r.read_int32(limit_left)  ||  r.read_int32(limit_total)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82100005;
		limit_type = 0;
		trade_type = 0;
		limit_left = 0;
		limit_total = 0;
	}
};

/*
场景服登录通知-基础信息
*/
struct MSG_81000100 : public Base_Msg  {
	std::vector<Property> property;
	std::string role_name;
	std::string gang_name;
	uint8_t gender;
	int64_t gang_id;
	int8_t scene_order_total;
	int8_t scene_order_now;
	MSG_81000100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __property_vec_size = property.size();
		w.write_uint16(__property_vec_size);
		for(uint16_t i = 0; i < __property_vec_size; ++i) {
				property[i].serialize(w);
		}

		w.write_string(role_name);
		w.write_string(gang_name);
		w.write_uint8(gender);
		w.write_int64(gang_id);
		w.write_int8(scene_order_total);
		w.write_int8(scene_order_now);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __property_vec_size;
		if(r.read_uint16(__property_vec_size)  )
				return -1;
		property.reserve(__property_vec_size);
		for(uint16_t i = 0; i < __property_vec_size; ++i) {
				Property v;
				if(v.deserialize(r))
						return -1;
				property.push_back(v);
		}

		if( r.read_string(role_name)  ||  r.read_string(gang_name)  ||  r.read_uint8(gender)  ||  r.read_int64(gang_id)  ||  r.read_int8(scene_order_total)  ||  r.read_int8(scene_order_now)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000100;
		property.clear();
		role_name.clear();
		gang_name.clear();
		gender = 0;
		gang_id = 0;
		scene_order_total = 0;
		scene_order_now = 0;
	}
};

/*
更新地图传送点数据
*/
struct MSG_81000040 : public Base_Msg  {
	std::vector<Transmit_Point> transmit_point;
	MSG_81000040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __transmit_point_vec_size = transmit_point.size();
		w.write_uint16(__transmit_point_vec_size);
		for(uint16_t i = 0; i < __transmit_point_vec_size; ++i) {
				transmit_point[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __transmit_point_vec_size;
		if(r.read_uint16(__transmit_point_vec_size)  )
				return -1;
		transmit_point.reserve(__transmit_point_vec_size);
		for(uint16_t i = 0; i < __transmit_point_vec_size; ++i) {
				Transmit_Point v;
				if(v.deserialize(r))
						return -1;
				transmit_point.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000040;
		transmit_point.clear();
	}
};

/*
使用物品得到物品列表
*/
struct MSG_82100006 : public Base_Msg  {
	std::vector<Item_Basic_Info> item_info;
	MSG_82100006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_info_vec_size = item_info.size();
		w.write_uint16(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				item_info[i].serialize(w);
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

		return 0;
	}

	void reset(void) {
		msg_id = 82100006;
		item_info.clear();
	}
};

/*
任务信息更新
备注：主要包括有杀怪,升级,交谈,捕获,其他. 
*/
struct MSG_81110000 : public Base_Msg  {
	uint8_t operate;
	Task_Info task;
	MSG_81110000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(operate);
		task.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(operate)  ||  task.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110000;
		operate = 0;
		task.reset();
	}
};

/*
寻路传送点数据通知
*/
struct MSG_81000041 : public Base_Msg  {
	std::vector<Transmit_Point> transmit_point;
	MSG_81000041(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __transmit_point_vec_size = transmit_point.size();
		w.write_uint16(__transmit_point_vec_size);
		for(uint16_t i = 0; i < __transmit_point_vec_size; ++i) {
				transmit_point[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __transmit_point_vec_size;
		if(r.read_uint16(__transmit_point_vec_size)  )
				return -1;
		transmit_point.reserve(__transmit_point_vec_size);
		for(uint16_t i = 0; i < __transmit_point_vec_size; ++i) {
				Transmit_Point v;
				if(v.deserialize(r))
						return -1;
				transmit_point.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000041;
		transmit_point.clear();
	}
};

/*
进入战斗模式
*/
struct MSG_81000050 : public Base_Msg  {
	int32_t map_id;
	int32_t position;
	std::vector<int64_t> monster_id;
	uint8_t wave_cur;
	uint8_t wave_max;
	int8_t battle_type;
	MSG_81000050(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(map_id);
		w.write_int32(position);
		uint16_t __monster_id_vec_size = monster_id.size();
		w.write_uint16(__monster_id_vec_size);
		for(uint16_t i = 0; i < __monster_id_vec_size; ++i) {
				w.write_int64(monster_id[i]);
		}

		w.write_uint8(wave_cur);
		w.write_uint8(wave_max);
		w.write_int8(battle_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(map_id)  ||  r.read_int32(position)  )
				return -1;
 		uint16_t __monster_id_vec_size;
		if(r.read_uint16(__monster_id_vec_size)  )
				return -1;
		monster_id.reserve(__monster_id_vec_size);
		for(uint16_t i = 0; i < __monster_id_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				monster_id.push_back(v);
		}

		if( r.read_uint8(wave_cur)  ||  r.read_uint8(wave_max)  ||  r.read_int8(battle_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000050;
		map_id = 0;
		position = 0;
		monster_id.clear();
		wave_cur = 0;
		wave_max = 0;
		battle_type = 0;
	}
};

/*
结束战斗通知
*/
struct MSG_81000052 : public Base_Msg  {
	int32_t win;
	std::string plot_id;
	int8_t over_type;
	std::vector<Uid_Name> btl_unit_name_vec;
	MSG_81000052(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(win);
		w.write_string(plot_id);
		w.write_int8(over_type);
		uint16_t __btl_unit_name_vec_vec_size = btl_unit_name_vec.size();
		w.write_uint16(__btl_unit_name_vec_vec_size);
		for(uint16_t i = 0; i < __btl_unit_name_vec_vec_size; ++i) {
				btl_unit_name_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(win)  ||  r.read_string(plot_id)  ||  r.read_int8(over_type)  )
				return -1;
 		uint16_t __btl_unit_name_vec_vec_size;
		if(r.read_uint16(__btl_unit_name_vec_vec_size)  )
				return -1;
		btl_unit_name_vec.reserve(__btl_unit_name_vec_vec_size);
		for(uint16_t i = 0; i < __btl_unit_name_vec_vec_size; ++i) {
				Uid_Name v;
				if(v.deserialize(r))
						return -1;
				btl_unit_name_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000052;
		win = 0;
		plot_id.clear();
		over_type = 0;
		btl_unit_name_vec.clear();
	}
};

/*
退出战斗模式
*/
struct MSG_81000055 : public Base_Msg  {
	int32_t type;
	MSG_81000055(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000055;
		type = 0;
	}
};

/*
战斗单位出现
*/
struct MSG_81000008 : public Base_Msg  {
	Fighter_Appear_Info fighter_info;
	MSG_81000008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		fighter_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( fighter_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000008;
		fighter_info.reset();
	}
};

/*
属性信息更新(战斗属性)
*/
struct MSG_81000060 : public Base_Msg  {
	int64_t role_id;
	int64_t attack_id;
	std::vector<Prop_Change> change_info;
	int32_t result;
	double extra_value;
	int32_t source_id;
	MSG_81000060(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int64(attack_id);
		uint16_t __change_info_vec_size = change_info.size();
		w.write_uint16(__change_info_vec_size);
		for(uint16_t i = 0; i < __change_info_vec_size; ++i) {
				change_info[i].serialize(w);
		}

		w.write_int32(result);
		w.write_double(extra_value);
		w.write_int32(source_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int64(attack_id)  )
				return -1;
 		uint16_t __change_info_vec_size;
		if(r.read_uint16(__change_info_vec_size)  )
				return -1;
		change_info.reserve(__change_info_vec_size);
		for(uint16_t i = 0; i < __change_info_vec_size; ++i) {
				Prop_Change v;
				if(v.deserialize(r))
						return -1;
				change_info.push_back(v);
		}

		if( r.read_int32(result)  ||  r.read_double(extra_value)  ||  r.read_int32(source_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000060;
		role_id = 0;
		attack_id = 0;
		change_info.clear();
		result = 0;
		extra_value = 0.0;
		source_id = 0;
	}
};

/*
技能动作效果
*/
struct MSG_81000070 : public Base_Msg  {
	int32_t skill_id;
	int32_t skill_lv;
	int32_t step;
	int64_t attack_id;
	std::vector<int64_t> target_id;
	std::vector<uint32_t> effect_id_vec;
	int32_t errcode;
	int32_t step_time;
	MSG_81000070(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int32(skill_lv);
		w.write_int32(step);
		w.write_int64(attack_id);
		uint16_t __target_id_vec_size = target_id.size();
		w.write_uint16(__target_id_vec_size);
		for(uint16_t i = 0; i < __target_id_vec_size; ++i) {
				w.write_int64(target_id[i]);
		}

		uint16_t __effect_id_vec_vec_size = effect_id_vec.size();
		w.write_uint16(__effect_id_vec_vec_size);
		for(uint16_t i = 0; i < __effect_id_vec_vec_size; ++i) {
				w.write_uint32(effect_id_vec[i]);
		}

		w.write_int32(errcode);
		w.write_int32(step_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int32(skill_lv)  ||  r.read_int32(step)  ||  r.read_int64(attack_id)  )
				return -1;
 		uint16_t __target_id_vec_size;
		if(r.read_uint16(__target_id_vec_size)  )
				return -1;
		target_id.reserve(__target_id_vec_size);
		for(uint16_t i = 0; i < __target_id_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				target_id.push_back(v);
		}

		uint16_t __effect_id_vec_vec_size;
		if(r.read_uint16(__effect_id_vec_vec_size)  )
				return -1;
		effect_id_vec.reserve(__effect_id_vec_vec_size);
		for(uint16_t i = 0; i < __effect_id_vec_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				effect_id_vec.push_back(v);
		}

		if( r.read_int32(errcode)  ||  r.read_int32(step_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000070;
		skill_id = 0;
		skill_lv = 0;
		step = 0;
		attack_id = 0;
		target_id.clear();
		effect_id_vec.clear();
		errcode = 0;
		step_time = 0;
	}
};

/*
收到邮件通知
*/
struct MSG_82200000 : public Base_Msg  {
	int32_t mail_count;
	int32_t not_read_count;
	MSG_82200000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mail_count);
		w.write_int32(not_read_count);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mail_count)  ||  r.read_int32(not_read_count)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200000;
		mail_count = 0;
		not_read_count = 0;
	}
};

/*
重置坐标
*/
struct MSG_81000015 : public Base_Msg  {
	int64_t role_id;
	Coord coord;
	MSG_81000015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000015;
		role_id = 0;
		coord.reset();
	}
};

/*
通知玩家死亡
*/
struct MSG_81000045 : public Base_Msg  {
	int64_t role_id;
	int8_t forced_dead;
	MSG_81000045(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(forced_dead);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(forced_dead)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000045;
		role_id = 0;
		forced_dead = 0;
	}
};

/*
聊天广播
*/
struct MSG_83000000 : public Base_Msg  {
	uint8_t chat_type;
	int32_t system_type;
	uint8_t role_type;
	std::vector<Chat_Content> content;
	MSG_83000000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(chat_type);
		w.write_int32(system_type);
		w.write_uint8(role_type);
		uint16_t __content_vec_size = content.size();
		w.write_uint16(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				content[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(chat_type)  ||  r.read_int32(system_type)  ||  r.read_uint8(role_type)  )
				return -1;
 		uint16_t __content_vec_size;
		if(r.read_uint16(__content_vec_size)  )
				return -1;
		content.reserve(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				Chat_Content v;
				if(v.deserialize(r))
						return -1;
				content.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 83000000;
		chat_type = 0;
		system_type = 0;
		role_type = 0;
		content.clear();
	}
};

/*
更新人物属性信息
*/
struct MSG_81000101 : public Base_Msg  {
	int8_t type;
	std::vector<Property> property;
	MSG_81000101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		uint16_t __property_vec_size = property.size();
		w.write_uint16(__property_vec_size);
		for(uint16_t i = 0; i < __property_vec_size; ++i) {
				property[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		uint16_t __property_vec_size;
		if(r.read_uint16(__property_vec_size)  )
				return -1;
		property.reserve(__property_vec_size);
		for(uint16_t i = 0; i < __property_vec_size; ++i) {
				Property v;
				if(v.deserialize(r))
						return -1;
				property.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000101;
		type = 0;
		property.clear();
	}
};

/*
好友申请列表推送
*/
struct MSG_82200001 : public Base_Msg  {
	int8_t friend_apply_in;
	MSG_82200001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(friend_apply_in);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(friend_apply_in)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200001;
		friend_apply_in = 0;
	}
};

/*
采集结束通知
*/
struct MSG_81000110 : public Base_Msg  {
	int64_t material_role_id;
	int8_t result;
	MSG_81000110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(material_role_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(material_role_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000110;
		material_role_id = 0;
		result = 0;
	}
};

/*
增加英雄信息更新
*/
struct MSG_81001000 : public Base_Msg  {
	Hero_Info hero_info;
	MSG_81001000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		hero_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( hero_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81001000;
		hero_info.reset();
	}
};

/*
英雄信息更新
备注：
主动更新类型:
0：默认
1：英雄升级
2：觉醒升级
*/
struct MSG_81001001 : public Base_Msg  {
	Hero_Info hero_info;
	int32_t active_type;
	MSG_81001001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		hero_info.serialize(w);
		w.write_int32(active_type);
	}

	int deserialize(Block_Buffer & r) {
		if( hero_info.deserialize(r)  ||  r.read_int32(active_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81001001;
		hero_info.reset();
		active_type = 0;
	}
};

/*
战斗buff添修改
*/
struct MSG_81000071 : public Base_Msg  {
	int32_t type;
	std::vector<Status_Info_Msg> buff_detail;
	int8_t is_die;
	MSG_81000071(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		uint16_t __buff_detail_vec_size = buff_detail.size();
		w.write_uint16(__buff_detail_vec_size);
		for(uint16_t i = 0; i < __buff_detail_vec_size; ++i) {
				buff_detail[i].serialize(w);
		}

		w.write_int8(is_die);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		uint16_t __buff_detail_vec_size;
		if(r.read_uint16(__buff_detail_vec_size)  )
				return -1;
		buff_detail.reserve(__buff_detail_vec_size);
		for(uint16_t i = 0; i < __buff_detail_vec_size; ++i) {
				Status_Info_Msg v;
				if(v.deserialize(r))
						return -1;
				buff_detail.push_back(v);
		}

		if( r.read_int8(is_die)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000071;
		type = 0;
		buff_detail.clear();
		is_die = 0;
	}
};

/*
怪物触发剧情
*/
struct MSG_81000201 : public Base_Msg  {
	std::string plot_id;
	MSG_81000201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(plot_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(plot_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000201;
		plot_id.clear();
	}
};

/*
广播房间列表
备注：
大厅
*/
struct MSG_80100300 : public Base_Msg  {
	uint8_t operate;
	int8_t type;
	std::vector<Team_Basic_Info> teams;
	MSG_80100300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(operate);
		w.write_int8(type);
		uint16_t __teams_vec_size = teams.size();
		w.write_uint16(__teams_vec_size);
		for(uint16_t i = 0; i < __teams_vec_size; ++i) {
				teams[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(operate)  ||  r.read_int8(type)  )
				return -1;
 		uint16_t __teams_vec_size;
		if(r.read_uint16(__teams_vec_size)  )
				return -1;
		teams.reserve(__teams_vec_size);
		for(uint16_t i = 0; i < __teams_vec_size; ++i) {
				Team_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				teams.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100300;
		operate = 0;
		type = 0;
		teams.clear();
	}
};

/*
广播等待区信息
*/
struct MSG_80100301 : public Base_Msg  {
	uint8_t operate;
	int8_t type;
	std::vector<Role_Team_Info> roles;
	MSG_80100301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(operate);
		w.write_int8(type);
		uint16_t __roles_vec_size = roles.size();
		w.write_uint16(__roles_vec_size);
		for(uint16_t i = 0; i < __roles_vec_size; ++i) {
				roles[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(operate)  ||  r.read_int8(type)  )
				return -1;
 		uint16_t __roles_vec_size;
		if(r.read_uint16(__roles_vec_size)  )
				return -1;
		roles.reserve(__roles_vec_size);
		for(uint16_t i = 0; i < __roles_vec_size; ++i) {
				Role_Team_Info v;
				if(v.deserialize(r))
						return -1;
				roles.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100301;
		operate = 0;
		type = 0;
		roles.clear();
	}
};

/*
通知小队信息
备注：玩家上线、加入队伍、创建队伍时收到信息
*/
struct MSG_80100302 : public Base_Msg  {
	int64_t leader_id;
	int8_t type;
	std::vector<Online_Teamer_Info> online_teamer;
	std::vector<Role_Info> offline_teamer;
	Team_Basic_Info team_info;
	int8_t profile_set;
	MSG_80100302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(leader_id);
		w.write_int8(type);
		uint16_t __online_teamer_vec_size = online_teamer.size();
		w.write_uint16(__online_teamer_vec_size);
		for(uint16_t i = 0; i < __online_teamer_vec_size; ++i) {
				online_teamer[i].serialize(w);
		}

		uint16_t __offline_teamer_vec_size = offline_teamer.size();
		w.write_uint16(__offline_teamer_vec_size);
		for(uint16_t i = 0; i < __offline_teamer_vec_size; ++i) {
				offline_teamer[i].serialize(w);
		}

		team_info.serialize(w);
		w.write_int8(profile_set);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(leader_id)  ||  r.read_int8(type)  )
				return -1;
 		uint16_t __online_teamer_vec_size;
		if(r.read_uint16(__online_teamer_vec_size)  )
				return -1;
		online_teamer.reserve(__online_teamer_vec_size);
		for(uint16_t i = 0; i < __online_teamer_vec_size; ++i) {
				Online_Teamer_Info v;
				if(v.deserialize(r))
						return -1;
				online_teamer.push_back(v);
		}

		uint16_t __offline_teamer_vec_size;
		if(r.read_uint16(__offline_teamer_vec_size)  )
				return -1;
		offline_teamer.reserve(__offline_teamer_vec_size);
		for(uint16_t i = 0; i < __offline_teamer_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				offline_teamer.push_back(v);
		}

		if( team_info.deserialize(r)  ||  r.read_int8(profile_set)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100302;
		leader_id = 0;
		type = 0;
		online_teamer.clear();
		offline_teamer.clear();
		team_info.reset();
		profile_set = 0;
	}
};

/*
通知队员上线
备注：玩家上线时，会对小队的其他玩家发送此消息
*/
struct MSG_80100303 : public Base_Msg  {
	Online_Teamer_Info teamer;
	MSG_80100303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		teamer.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( teamer.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100303;
		teamer.reset();
	}
};

/*
通知队员下线
*/
struct MSG_80100304 : public Base_Msg  {
	int64_t role_id;
	MSG_80100304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100304;
		role_id = 0;
	}
};

/*
通知队员入队
*/
struct MSG_80100305 : public Base_Msg  {
	Online_Teamer_Info teamer;
	MSG_80100305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		teamer.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( teamer.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100305;
		teamer.reset();
	}
};

/*
通知队员离队
*/
struct MSG_80100306 : public Base_Msg  {
	int64_t role_id;
	int8_t is_kick;
	MSG_80100306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(is_kick);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(is_kick)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100306;
		role_id = 0;
		is_kick = 0;
	}
};

/*
通知小队解散
备注：玩家上线、加入队伍、创建队伍时收到信息
*/
struct MSG_80100307 : public Base_Msg  {
	MSG_80100307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 80100307;
	}
};

/*
通知入队邀请
*/
struct MSG_80100308 : public Base_Msg  {
	Role_Team_Info inviter;
	int8_t type;
	int32_t value;
	int32_t scene_degree;
	int32_t nums;
	int8_t in_battle;
	std::string monster;
	MSG_80100308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		inviter.serialize(w);
		w.write_int8(type);
		w.write_int32(value);
		w.write_int32(scene_degree);
		w.write_int32(nums);
		w.write_int8(in_battle);
		w.write_string(monster);
	}

	int deserialize(Block_Buffer & r) {
		if( inviter.deserialize(r)  ||  r.read_int8(type)  ||  r.read_int32(value)  ||  r.read_int32(scene_degree)  ||  r.read_int32(nums)  ||  r.read_int8(in_battle)  ||  r.read_string(monster)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100308;
		inviter.reset();
		type = 0;
		value = 0;
		scene_degree = 0;
		nums = 0;
		in_battle = 0;
		monster.clear();
	}
};

/*
通知入队申请
*/
struct MSG_80100309 : public Base_Msg  {
	Role_Team_Info applicant;
	MSG_80100309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		applicant.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( applicant.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100309;
		applicant.reset();
	}
};

/*
通知拒绝入队邀请
*/
struct MSG_80100310 : public Base_Msg  {
	Role_Team_Info rejecter;
	MSG_80100310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		rejecter.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( rejecter.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100310;
		rejecter.reset();
	}
};

/*
通知小队队长改变
*/
struct MSG_80100312 : public Base_Msg  {
	int64_t old_leader;
	int64_t new_leader;
	MSG_80100312(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(old_leader);
		w.write_int64(new_leader);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(old_leader)  ||  r.read_int64(new_leader)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100312;
		old_leader = 0;
		new_leader = 0;
	}
};

/*
通知队员状态改变
*/
struct MSG_80100313 : public Base_Msg  {
	Online_Teamer_Info teamer;
	MSG_80100313(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		teamer.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( teamer.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100313;
		teamer.reset();
	}
};

/*
通知队员未准备
*/
struct MSG_80100317 : public Base_Msg  {
	std::string name;
	MSG_80100317(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100317;
		name.clear();
	}
};

/*
好友同意添加
*/
struct MSG_82200002 : public Base_Msg  {
	int8_t handel_friend_apply;
	std::string role_name;
	MSG_82200002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(handel_friend_apply);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(handel_friend_apply)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200002;
		handel_friend_apply = 0;
		role_name.clear();
	}
};

/*
广播房间信息
*/
struct MSG_80100318 : public Base_Msg  {
	Team_Basic_Info info;
	MSG_80100318(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100318;
		info.reset();
	}
};

/*
通知商城列表变化
*/
struct MSG_82151000 : public Base_Msg  {
	std::vector<int32_t> diff_tabs;
	std::vector<Goods_Info> diff_list;
	Mall_Promo_Info promo_goods;
	Mall_Promo_Info banner_goods;
	MSG_82151000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __diff_tabs_vec_size = diff_tabs.size();
		w.write_uint16(__diff_tabs_vec_size);
		for(uint16_t i = 0; i < __diff_tabs_vec_size; ++i) {
				w.write_int32(diff_tabs[i]);
		}

		uint16_t __diff_list_vec_size = diff_list.size();
		w.write_uint16(__diff_list_vec_size);
		for(uint16_t i = 0; i < __diff_list_vec_size; ++i) {
				diff_list[i].serialize(w);
		}

		promo_goods.serialize(w);
		banner_goods.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __diff_tabs_vec_size;
		if(r.read_uint16(__diff_tabs_vec_size)  )
				return -1;
		diff_tabs.reserve(__diff_tabs_vec_size);
		for(uint16_t i = 0; i < __diff_tabs_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				diff_tabs.push_back(v);
		}

		uint16_t __diff_list_vec_size;
		if(r.read_uint16(__diff_list_vec_size)  )
				return -1;
		diff_list.reserve(__diff_list_vec_size);
		for(uint16_t i = 0; i < __diff_list_vec_size; ++i) {
				Goods_Info v;
				if(v.deserialize(r))
						return -1;
				diff_list.push_back(v);
		}

		if( promo_goods.deserialize(r)  ||  banner_goods.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82151000;
		diff_tabs.clear();
		diff_list.clear();
		promo_goods.reset();
		banner_goods.reset();
	}
};

/*
战斗剧情结束通知
*/
struct MSG_81000202 : public Base_Msg  {
	MSG_81000202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 81000202;
	}
};

/*
战斗挂起通知
*/
struct MSG_81000203 : public Base_Msg  {
	MSG_81000203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 81000203;
	}
};

/*
战斗恢复通知
*/
struct MSG_81000204 : public Base_Msg  {
	MSG_81000204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 81000204;
	}
};

/*
逻辑服登录通知-基础信息
*/
struct MSG_81000099 : public Base_Msg  {
	std::string plot_id;
	int32_t plot_step;
	MSG_81000099(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(plot_id);
		w.write_int32(plot_step);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(plot_id)  ||  r.read_int32(plot_step)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000099;
		plot_id.clear();
		plot_step = 0;
	}
};

/*
npc对话通知
*/
struct MSG_81000205 : public Base_Msg  {
	int64_t role_id;
	int32_t talk_id;
	int32_t talk_time;
	MSG_81000205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(talk_id);
		w.write_int32(talk_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(talk_id)  ||  r.read_int32(talk_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000205;
		role_id = 0;
		talk_id = 0;
		talk_time = 0;
	}
};

/*
任务随身怪出现
*/
struct MSG_81110001 : public Base_Msg  {
	std::vector<int64_t> npc_ids;
	std::vector<NPC_Appear_Info_Task> npc_info;
	MSG_81110001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __npc_ids_vec_size = npc_ids.size();
		w.write_uint16(__npc_ids_vec_size);
		for(uint16_t i = 0; i < __npc_ids_vec_size; ++i) {
				w.write_int64(npc_ids[i]);
		}

		uint16_t __npc_info_vec_size = npc_info.size();
		w.write_uint16(__npc_info_vec_size);
		for(uint16_t i = 0; i < __npc_info_vec_size; ++i) {
				npc_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __npc_ids_vec_size;
		if(r.read_uint16(__npc_ids_vec_size)  )
				return -1;
		npc_ids.reserve(__npc_ids_vec_size);
		for(uint16_t i = 0; i < __npc_ids_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				npc_ids.push_back(v);
		}

		uint16_t __npc_info_vec_size;
		if(r.read_uint16(__npc_info_vec_size)  )
				return -1;
		npc_info.reserve(__npc_info_vec_size);
		for(uint16_t i = 0; i < __npc_info_vec_size; ++i) {
				NPC_Appear_Info_Task v;
				if(v.deserialize(r))
						return -1;
				npc_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81110001;
		npc_ids.clear();
		npc_info.clear();
	}
};

/*
广播组队布阵信息
Int_Role_Id: 
  role_id(用户id) 
  int_value(0表示前1表示后)
*/
struct MSG_80100319 : public Base_Msg  {
	std::vector<Int_Role_Id> info;
	MSG_80100319(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Int_Role_Id v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100319;
		info.clear();
	}
};

/*
请求飞行(坐电梯)
*/
struct MSG_81000025 : public Base_Msg  {
	Coord coord;
	MSG_81000025(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000025;
		coord.reset();
	}
};

/*
公会列表信息更新
*/
struct MSG_81070000 : public Base_Msg  {
	int8_t operate;
	Gang_Row_Info row_info;
	MSG_81070000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(operate);
		row_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(operate)  ||  row_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81070000;
		operate = 0;
		row_info.reset();
	}
};

/*
公会成员列表更新信息
*/
struct MSG_81070001 : public Base_Msg  {
	int8_t operate;
	Ganger_Row_Info row_info;
	MSG_81070001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(operate);
		row_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(operate)  ||  row_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81070001;
		operate = 0;
		row_info.reset();
	}
};

/*
移动速度更改通知
*/
struct MSG_81000021 : public Base_Msg  {
	int64_t role_id;
	int16_t speed;
	MSG_81000021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int16(speed);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int16(speed)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000021;
		role_id = 0;
		speed = 0;
	}
};

/*
同步时间
*/
struct MSG_80000010 : public Base_Msg  {
	int32_t time;
	MSG_80000010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80000010;
		time = 0;
	}
};

/*
地图可走信息通知
*/
struct MSG_81000022 : public Base_Msg  {
	std::vector<int16_t> moveable_info_vec;
	MSG_81000022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __moveable_info_vec_vec_size = moveable_info_vec.size();
		w.write_uint16(__moveable_info_vec_vec_size);
		for(uint16_t i = 0; i < __moveable_info_vec_vec_size; ++i) {
				w.write_int16(moveable_info_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __moveable_info_vec_vec_size;
		if(r.read_uint16(__moveable_info_vec_vec_size)  )
				return -1;
		moveable_info_vec.reserve(__moveable_info_vec_vec_size);
		for(uint16_t i = 0; i < __moveable_info_vec_vec_size; ++i) {
				int16_t v;
				if(r.read_int16(v) )
						return -1;
				moveable_info_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000022;
		moveable_info_vec.clear();
	}
};

/*
防沉迷时间提示
*/
struct MSG_82200100 : public Base_Msg  {
	int32_t online_time;
	MSG_82200100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(online_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(online_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200100;
		online_time = 0;
	}
};

/*
防沉迷登陆提示
*/
struct MSG_82200101 : public Base_Msg  {
	int32_t online_time;
	int32_t fcm;
	MSG_82200101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(online_time);
		w.write_int32(fcm);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(online_time)  ||  r.read_int32(fcm)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200101;
		online_time = 0;
		fcm = 0;
	}
};

/*
移动场景击退通知
*/
struct MSG_81000023 : public Base_Msg  {
	int64_t role_id;
	int16_t pos_x;
	int16_t pos_y;
	MSG_81000023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int16(pos_x);
		w.write_int16(pos_y);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int16(pos_x)  ||  r.read_int16(pos_y)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000023;
		role_id = 0;
		pos_x = 0;
		pos_y = 0;
	}
};

/*
公会动态信息更新
*/
struct MSG_81070002 : public Base_Msg  {
	Gang_Passed_Event passed_event;
	MSG_81070002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		passed_event.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( passed_event.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81070002;
		passed_event.reset();
	}
};

/*
公会信息更新
*/
struct MSG_81070003 : public Base_Msg  {
	Gang_Info gang_info;
	MSG_81070003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		gang_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( gang_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81070003;
		gang_info.reset();
	}
};

/*
玩家公会信息更新
*/
struct MSG_81070004 : public Base_Msg  {
	Ganger_Info ganger_info;
	MSG_81070004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		ganger_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( ganger_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81070004;
		ganger_info.reset();
	}
};

/*
移动场景击退开始通知
*/
struct MSG_81000024 : public Base_Msg  {
	int16_t pos_x;
	int32_t pos_y;
	int16_t distance;
	MSG_81000024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(pos_x);
		w.write_int32(pos_y);
		w.write_int16(distance);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(pos_x)  ||  r.read_int32(pos_y)  ||  r.read_int16(distance)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000024;
		pos_x = 0;
		pos_y = 0;
		distance = 0;
	}
};

/*
副本npc路径点指引
*/
struct MSG_81000016 : public Base_Msg  {
	int8_t type;
	uint16_t pos_x;
	uint16_t pos_y;
	uint16_t point;
	MSG_81000016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_uint16(pos_x);
		w.write_uint16(pos_y);
		w.write_uint16(point);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_uint16(pos_x)  ||  r.read_uint16(pos_y)  ||  r.read_uint16(point)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000016;
		type = 0;
		pos_x = 0;
		pos_y = 0;
		point = 0;
	}
};

/*
地图不可联通点通知
*/
struct MSG_81000017 : public Base_Msg  {
	std::vector<Int_Int> unlink_vec;
	MSG_81000017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __unlink_vec_vec_size = unlink_vec.size();
		w.write_uint16(__unlink_vec_vec_size);
		for(uint16_t i = 0; i < __unlink_vec_vec_size; ++i) {
				unlink_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __unlink_vec_vec_size;
		if(r.read_uint16(__unlink_vec_vec_size)  )
				return -1;
		unlink_vec.reserve(__unlink_vec_vec_size);
		for(uint16_t i = 0; i < __unlink_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				unlink_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000017;
		unlink_vec.clear();
	}
};

/*
怪物开始追击
*/
struct MSG_81000018 : public Base_Msg  {
	int64_t role_id;
	uint32_t talk_id;
	MSG_81000018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint32(talk_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint32(talk_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000018;
		role_id = 0;
		talk_id = 0;
	}
};

/*
符文孔信息更新
*/
struct MSG_81002000 : public Base_Msg  {
	int32_t part;
	Rune_Hole_Info hole_info;
	int8_t is_upgrade;
	MSG_81002000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(part);
		hole_info.serialize(w);
		w.write_int8(is_upgrade);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(part)  ||  hole_info.deserialize(r)  ||  r.read_int8(is_upgrade)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81002000;
		part = 0;
		hole_info.reset();
		is_upgrade = 0;
	}
};

/*
竞技场本服排名变化通知
*/
struct MSG_81000300 : public Base_Msg  {
	uint16_t rank;
	MSG_81000300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(rank);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(rank)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000300;
		rank = 0;
	}
};

/*
竞技场跨服排名变化通知
*/
struct MSG_81000301 : public Base_Msg  {
	uint16_t rank;
	MSG_81000301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(rank);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(rank)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000301;
		rank = 0;
	}
};

/*
对象播放动画
*/
struct MSG_81000014 : public Base_Msg  {
	int64_t role_id;
	MSG_81000014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000014;
		role_id = 0;
	}
};

/*
属性信息更新(buff属性)
*/
struct MSG_81000061 : public Base_Msg  {
	int64_t role_id;
	int64_t attack_id;
	int32_t status_id;
	int32_t effect_id;
	int32_t skill_id;
	std::vector<Prop_Change> change_info;
	int32_t result;
	double extra_value;
	MSG_81000061(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int64(attack_id);
		w.write_int32(status_id);
		w.write_int32(effect_id);
		w.write_int32(skill_id);
		uint16_t __change_info_vec_size = change_info.size();
		w.write_uint16(__change_info_vec_size);
		for(uint16_t i = 0; i < __change_info_vec_size; ++i) {
				change_info[i].serialize(w);
		}

		w.write_int32(result);
		w.write_double(extra_value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int64(attack_id)  ||  r.read_int32(status_id)  ||  r.read_int32(effect_id)  ||  r.read_int32(skill_id)  )
				return -1;
 		uint16_t __change_info_vec_size;
		if(r.read_uint16(__change_info_vec_size)  )
				return -1;
		change_info.reserve(__change_info_vec_size);
		for(uint16_t i = 0; i < __change_info_vec_size; ++i) {
				Prop_Change v;
				if(v.deserialize(r))
						return -1;
				change_info.push_back(v);
		}

		if( r.read_int32(result)  ||  r.read_double(extra_value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000061;
		role_id = 0;
		attack_id = 0;
		status_id = 0;
		effect_id = 0;
		skill_id = 0;
		change_info.clear();
		result = 0;
		extra_value = 0.0;
	}
};

/*
离线聊天记录广播
*/
struct MSG_83000001 : public Base_Msg  {
	std::vector<Chat_Record> chat_record;
	MSG_83000001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __chat_record_vec_size = chat_record.size();
		w.write_uint16(__chat_record_vec_size);
		for(uint16_t i = 0; i < __chat_record_vec_size; ++i) {
				chat_record[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __chat_record_vec_size;
		if(r.read_uint16(__chat_record_vec_size)  )
				return -1;
		chat_record.reserve(__chat_record_vec_size);
		for(uint16_t i = 0; i < __chat_record_vec_size; ++i) {
				Chat_Record v;
				if(v.deserialize(r))
						return -1;
				chat_record.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 83000001;
		chat_record.clear();
	}
};

/*
多人副本翻牌结果
按顺序排的结果
*/
struct MSG_81000080 : public Base_Msg  {
	std::vector<Reward_Info_List> list;
	MSG_81000080(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Reward_Info_List v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000080;
		list.clear();
	}
};

/*
buff效果广播
*/
struct MSG_81000062 : public Base_Msg  {
	int32_t buff_type;
	int64_t role_id;
	int64_t attack_id;
	double extra_value;
	std::vector<int32_t> status_ids;
	MSG_81000062(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(buff_type);
		w.write_int64(role_id);
		w.write_int64(attack_id);
		w.write_double(extra_value);
		uint16_t __status_ids_vec_size = status_ids.size();
		w.write_uint16(__status_ids_vec_size);
		for(uint16_t i = 0; i < __status_ids_vec_size; ++i) {
				w.write_int32(status_ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(buff_type)  ||  r.read_int64(role_id)  ||  r.read_int64(attack_id)  ||  r.read_double(extra_value)  )
				return -1;
 		uint16_t __status_ids_vec_size;
		if(r.read_uint16(__status_ids_vec_size)  )
				return -1;
		status_ids.reserve(__status_ids_vec_size);
		for(uint16_t i = 0; i < __status_ids_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				status_ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000062;
		buff_type = 0;
		role_id = 0;
		attack_id = 0;
		extra_value = 0.0;
		status_ids.clear();
	}
};

/*
发送功能图标开启信息列表
*/
struct MSG_80100400 : public Base_Msg  {
	std::vector<Icon_Info> icon_list;
	MSG_80100400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __icon_list_vec_size = icon_list.size();
		w.write_uint16(__icon_list_vec_size);
		for(uint16_t i = 0; i < __icon_list_vec_size; ++i) {
				icon_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __icon_list_vec_size;
		if(r.read_uint16(__icon_list_vec_size)  )
				return -1;
		icon_list.reserve(__icon_list_vec_size);
		for(uint16_t i = 0; i < __icon_list_vec_size; ++i) {
				Icon_Info v;
				if(v.deserialize(r))
						return -1;
				icon_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100400;
		icon_list.clear();
	}
};

/*
发送开启/关闭功能图标
*/
struct MSG_80100401 : public Base_Msg  {
	uint16_t id;
	uint8_t status;
	MSG_80100401(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(id);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(id)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100401;
		id = 0;
		status = 0;
	}
};

/*
通知开始翻牌
*/
struct MSG_81000081 : public Base_Msg  {
	MSG_81000081(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 81000081;
	}
};

/*
竞技场个人信息变化通知
*/
struct MSG_81000302 : public Base_Msg  {
	uint8_t group;
	uint8_t seg;
	uint16_t local_rank;
	uint16_t score;
	uint8_t free_times;
	uint8_t pay_times;
	uint8_t max_pay_times;
	uint8_t buy_pay_times;
	uint32_t reward_times1;
	uint32_t reward_times2;
	std::vector<Arena_Battle_Report> report_vec;
	uint16_t three_day_rank;
	uint16_t seven_day_rank;
	uint8_t rise;
	std::vector<uint8_t> rise_result_vec;
	int32_t remain_cd_time;
	uint8_t total_free_times;
	MSG_81000302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(group);
		w.write_uint8(seg);
		w.write_uint16(local_rank);
		w.write_uint16(score);
		w.write_uint8(free_times);
		w.write_uint8(pay_times);
		w.write_uint8(max_pay_times);
		w.write_uint8(buy_pay_times);
		w.write_uint32(reward_times1);
		w.write_uint32(reward_times2);
		uint16_t __report_vec_vec_size = report_vec.size();
		w.write_uint16(__report_vec_vec_size);
		for(uint16_t i = 0; i < __report_vec_vec_size; ++i) {
				report_vec[i].serialize(w);
		}

		w.write_uint16(three_day_rank);
		w.write_uint16(seven_day_rank);
		w.write_uint8(rise);
		uint16_t __rise_result_vec_vec_size = rise_result_vec.size();
		w.write_uint16(__rise_result_vec_vec_size);
		for(uint16_t i = 0; i < __rise_result_vec_vec_size; ++i) {
				w.write_uint8(rise_result_vec[i]);
		}

		w.write_int32(remain_cd_time);
		w.write_uint8(total_free_times);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(group)  ||  r.read_uint8(seg)  ||  r.read_uint16(local_rank)  ||  r.read_uint16(score)  ||  r.read_uint8(free_times)  ||  r.read_uint8(pay_times)  ||  r.read_uint8(max_pay_times)  ||  r.read_uint8(buy_pay_times)  ||  r.read_uint32(reward_times1)  ||  r.read_uint32(reward_times2)  )
				return -1;
 		uint16_t __report_vec_vec_size;
		if(r.read_uint16(__report_vec_vec_size)  )
				return -1;
		report_vec.reserve(__report_vec_vec_size);
		for(uint16_t i = 0; i < __report_vec_vec_size; ++i) {
				Arena_Battle_Report v;
				if(v.deserialize(r))
						return -1;
				report_vec.push_back(v);
		}

		if( r.read_uint16(three_day_rank)  ||  r.read_uint16(seven_day_rank)  ||  r.read_uint8(rise)  )
				return -1;
 		uint16_t __rise_result_vec_vec_size;
		if(r.read_uint16(__rise_result_vec_vec_size)  )
				return -1;
		rise_result_vec.reserve(__rise_result_vec_vec_size);
		for(uint16_t i = 0; i < __rise_result_vec_vec_size; ++i) {
				uint8_t v;
				if(r.read_uint8(v) )
						return -1;
				rise_result_vec.push_back(v);
		}

		if( r.read_int32(remain_cd_time)  ||  r.read_uint8(total_free_times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000302;
		group = 0;
		seg = 0;
		local_rank = 0;
		score = 0;
		free_times = 0;
		pay_times = 0;
		max_pay_times = 0;
		buy_pay_times = 0;
		reward_times1 = 0;
		reward_times2 = 0;
		report_vec.clear();
		three_day_rank = 0;
		seven_day_rank = 0;
		rise = 0;
		rise_result_vec.clear();
		remain_cd_time = 0;
		total_free_times = 0;
	}
};

/*
主动推送不能引导的列表
*/
struct MSG_80100500 : public Base_Msg  {
	std::vector<uint16_t> hand_list;
	MSG_80100500(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __hand_list_vec_size = hand_list.size();
		w.write_uint16(__hand_list_vec_size);
		for(uint16_t i = 0; i < __hand_list_vec_size; ++i) {
				w.write_uint16(hand_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __hand_list_vec_size;
		if(r.read_uint16(__hand_list_vec_size)  )
				return -1;
		hand_list.reserve(__hand_list_vec_size);
		for(uint16_t i = 0; i < __hand_list_vec_size; ++i) {
				uint16_t v;
				if(r.read_uint16(v) )
						return -1;
				hand_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100500;
		hand_list.clear();
	}
};

/*
获得物品飘特效
*/
struct MSG_82100007 : public Base_Msg  {
	std::vector<Item_Basic_Info> item_info;
	MSG_82100007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_info_vec_size = item_info.size();
		w.write_uint16(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				item_info[i].serialize(w);
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

		return 0;
	}

	void reset(void) {
		msg_id = 82100007;
		item_info.clear();
	}
};

/*
背包获得客户端特效
*/
struct MSG_81000102 : public Base_Msg  {
	int8_t type;
	std::vector<Property> property;
	std::vector<Item_Basic_Info> item_info;
	std::vector<Card_Info> card_info_vec;
	int32_t vip_exp;
	int32_t world_level;
	MSG_81000102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		uint16_t __property_vec_size = property.size();
		w.write_uint16(__property_vec_size);
		for(uint16_t i = 0; i < __property_vec_size; ++i) {
				property[i].serialize(w);
		}

		uint16_t __item_info_vec_size = item_info.size();
		w.write_uint16(__item_info_vec_size);
		for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
				item_info[i].serialize(w);
		}

		uint16_t __card_info_vec_vec_size = card_info_vec.size();
		w.write_uint16(__card_info_vec_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_vec_size; ++i) {
				card_info_vec[i].serialize(w);
		}

		w.write_int32(vip_exp);
		w.write_int32(world_level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		uint16_t __property_vec_size;
		if(r.read_uint16(__property_vec_size)  )
				return -1;
		property.reserve(__property_vec_size);
		for(uint16_t i = 0; i < __property_vec_size; ++i) {
				Property v;
				if(v.deserialize(r))
						return -1;
				property.push_back(v);
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

		uint16_t __card_info_vec_vec_size;
		if(r.read_uint16(__card_info_vec_vec_size)  )
				return -1;
		card_info_vec.reserve(__card_info_vec_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_vec_size; ++i) {
				Card_Info v;
				if(v.deserialize(r))
						return -1;
				card_info_vec.push_back(v);
		}

		if( r.read_int32(vip_exp)  ||  r.read_int32(world_level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000102;
		type = 0;
		property.clear();
		item_info.clear();
		card_info_vec.clear();
		vip_exp = 0;
		world_level = 0;
	}
};

/*
更新人物在场景的状态
*/
struct MSG_81000026 : public Base_Msg  {
	int64_t role_id;
	int32_t state;
	MSG_81000026(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000026;
		role_id = 0;
		state = 0;
	}
};

/*
任务随身怪出现
*/
struct MSG_81110002 : public Base_Msg  {
	std::vector<int64_t> npc_ids;
	std::vector<NPC_Appear_Info_Task> npc_info;
	MSG_81110002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __npc_ids_vec_size = npc_ids.size();
		w.write_uint16(__npc_ids_vec_size);
		for(uint16_t i = 0; i < __npc_ids_vec_size; ++i) {
				w.write_int64(npc_ids[i]);
		}

		uint16_t __npc_info_vec_size = npc_info.size();
		w.write_uint16(__npc_info_vec_size);
		for(uint16_t i = 0; i < __npc_info_vec_size; ++i) {
				npc_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __npc_ids_vec_size;
		if(r.read_uint16(__npc_ids_vec_size)  )
				return -1;
		npc_ids.reserve(__npc_ids_vec_size);
		for(uint16_t i = 0; i < __npc_ids_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				npc_ids.push_back(v);
		}

		uint16_t __npc_info_vec_size;
		if(r.read_uint16(__npc_info_vec_size)  )
				return -1;
		npc_info.reserve(__npc_info_vec_size);
		for(uint16_t i = 0; i < __npc_info_vec_size; ++i) {
				NPC_Appear_Info_Task v;
				if(v.deserialize(r))
						return -1;
				npc_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81110002;
		npc_ids.clear();
		npc_info.clear();
	}
};

/*
公会主动通知
备注：更新公会信息给单个玩家
通知类型:
1. 有新申请加入公会通知(发给公会管理员,提示有新申请)                                                [id=申请者id,name=申请者名] 
2. 有新邀请加入公会通知(发给受邀者, 提示有新邀请)                                                     [id=公会id,name=公会名] 

3. 被批准加入公会(发给申请加入公会的玩家,此玩家收到后发送10170002,重新获取公会信息)  [id=公会id,name=公会名] 
4. 被拒绝加入公会(发给申请加入公会的玩家,提示玩家被绝加)                                          [id=公会id,name=公会名] 
     
5. 同意加入公会(发给发出邀请公会的管理员,提示管理员被邀请者同意加入)                        [id=申请者id,name=申请者名] 
6. 拒绝加入公会(发给发出邀请公会的管理员,提示管理员被邀请者拒绝加入)                        [id=申请者id,name=申请者名] 

7. 职位权限改变通知(发送给被任命成员)                                                                    [id=任命人ID,name=任命人姓名,value=职位ID] 
8. 发送公会祝福buf(发送给被祝福者)                                                                    [id=发送人ID,name=发送人名,value=buf ID] 

9. 被夺权人上线提示(发送给被夺权玩家)                                                                              [id=夺权人ID,name=夺权人名] 
*/
struct MSG_81070005 : public Base_Msg  {
	uint8_t action;
	int64_t id;
	std::string name;
	int32_t value;
	MSG_81070005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(action);
		w.write_int64(id);
		w.write_string(name);
		w.write_int32(value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(action)  ||  r.read_int64(id)  ||  r.read_string(name)  ||  r.read_int32(value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81070005;
		action = 0;
		id = 0;
		name.clear();
		value = 0;
	}
};

/*
体力信息更新
*/
struct MSG_81000063 : public Base_Msg  {
	uint16_t phy_power_cur;
	uint16_t phy_power_max;
	uint8_t phy_power_buy_times;
	int32_t phy_power_recover_time;
	MSG_81000063(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(phy_power_cur);
		w.write_uint16(phy_power_max);
		w.write_uint8(phy_power_buy_times);
		w.write_int32(phy_power_recover_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(phy_power_cur)  ||  r.read_uint16(phy_power_max)  ||  r.read_uint8(phy_power_buy_times)  ||  r.read_int32(phy_power_recover_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000063;
		phy_power_cur = 0;
		phy_power_max = 0;
		phy_power_buy_times = 0;
		phy_power_recover_time = 0;
	}
};

/*
途径获得提示
途径：采集
*/
struct MSG_81000103 : public Base_Msg  {
	std::vector<Chat_Content> content;
	MSG_81000103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __content_vec_size = content.size();
		w.write_uint16(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				content[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __content_vec_size;
		if(r.read_uint16(__content_vec_size)  )
				return -1;
		content.reserve(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				Chat_Content v;
				if(v.deserialize(r))
						return -1;
				content.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000103;
		content.clear();
	}
};

/*
副本消耗通知
*/
struct MSG_81000064 : public Base_Msg  {
	int32_t scene_id;
	uint8_t phy_power;
	MSG_81000064(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
		w.write_uint8(phy_power);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  ||  r.read_uint8(phy_power)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000064;
		scene_id = 0;
		phy_power = 0;
	}
};

/*
战斗下一波怪开始通知
*/
struct MSG_81000051 : public Base_Msg  {
	uint8_t wave_cur;
	uint8_t wave_max;
	MSG_81000051(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(wave_cur);
		w.write_uint8(wave_max);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(wave_cur)  ||  r.read_uint8(wave_max)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000051;
		wave_cur = 0;
		wave_max = 0;
	}
};

/*
主动通知人物面板英雄信息
*/
struct MSG_82210000 : public Base_Msg  {
	int64_t target_role_id;
	std::vector<Hero_Info> hero_info_vec;
	int32_t outer_index;
	int32_t fighter_index;
	MSG_82210000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(target_role_id);
		uint16_t __hero_info_vec_vec_size = hero_info_vec.size();
		w.write_uint16(__hero_info_vec_vec_size);
		for(uint16_t i = 0; i < __hero_info_vec_vec_size; ++i) {
				hero_info_vec[i].serialize(w);
		}

		w.write_int32(outer_index);
		w.write_int32(fighter_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(target_role_id)  )
				return -1;
 		uint16_t __hero_info_vec_vec_size;
		if(r.read_uint16(__hero_info_vec_vec_size)  )
				return -1;
		hero_info_vec.reserve(__hero_info_vec_vec_size);
		for(uint16_t i = 0; i < __hero_info_vec_vec_size; ++i) {
				Hero_Info v;
				if(v.deserialize(r))
						return -1;
				hero_info_vec.push_back(v);
		}

		if( r.read_int32(outer_index)  ||  r.read_int32(fighter_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82210000;
		target_role_id = 0;
		hero_info_vec.clear();
		outer_index = 0;
		fighter_index = 0;
	}
};

/*
电梯升降开始通知
*/
struct MSG_81000027 : public Base_Msg  {
	std::vector<int64_t> player_vec;
	int64_t lift_id;
	int16_t distance;
	int32_t arc_mid_x;
	int32_t arc_mid_y;
	int32_t degree;
	int32_t out_x;
	int32_t out_y;
	MSG_81000027(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __player_vec_vec_size = player_vec.size();
		w.write_uint16(__player_vec_vec_size);
		for(uint16_t i = 0; i < __player_vec_vec_size; ++i) {
				w.write_int64(player_vec[i]);
		}

		w.write_int64(lift_id);
		w.write_int16(distance);
		w.write_int32(arc_mid_x);
		w.write_int32(arc_mid_y);
		w.write_int32(degree);
		w.write_int32(out_x);
		w.write_int32(out_y);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __player_vec_vec_size;
		if(r.read_uint16(__player_vec_vec_size)  )
				return -1;
		player_vec.reserve(__player_vec_vec_size);
		for(uint16_t i = 0; i < __player_vec_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				player_vec.push_back(v);
		}

		if( r.read_int64(lift_id)  ||  r.read_int16(distance)  ||  r.read_int32(arc_mid_x)  ||  r.read_int32(arc_mid_y)  ||  r.read_int32(degree)  ||  r.read_int32(out_x)  ||  r.read_int32(out_y)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000027;
		player_vec.clear();
		lift_id = 0;
		distance = 0;
		arc_mid_x = 0;
		arc_mid_y = 0;
		degree = 0;
		out_x = 0;
		out_y = 0;
	}
};

/*
电梯升降结束通知
*/
struct MSG_81000028 : public Base_Msg  {
	int64_t lift_id;
	MSG_81000028(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(lift_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(lift_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000028;
		lift_id = 0;
	}
};

/*
主动发送限时任务超时未完成消息
备注：主动发送限时任务超时未完成消息
*/
struct MSG_81110003 : public Base_Msg  {
	uint32_t task_id;
	uint32_t error_code;
	MSG_81110003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint32(error_code);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint32(error_code)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110003;
		task_id = 0;
		error_code = 0;
	}
};

/*
通知推荐好友
*/
struct MSG_82200003 : public Base_Msg  {
	std::vector<Role_Info> roles;
	MSG_82200003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __roles_vec_size = roles.size();
		w.write_uint16(__roles_vec_size);
		for(uint16_t i = 0; i < __roles_vec_size; ++i) {
				roles[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __roles_vec_size;
		if(r.read_uint16(__roles_vec_size)  )
				return -1;
		roles.reserve(__roles_vec_size);
		for(uint16_t i = 0; i < __roles_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				roles.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82200003;
		roles.clear();
	}
};

/*
快速完成任务主动发送咨询是否愿意消耗货币的消息
备注：如果快速完成任务的消耗类型为  物品 和 货币都可以消耗， 并且 物品不足， 就会发送此消息给客户端。其他情况下，不发此消息。
客户端收到消息后，需要弹窗提示是否愿意消耗xxx货币多少。 
money_type:    1:金币  2:优先扣粉钻,粉钻不足，再扣钻石   3:钻石  4:粉钻   5:英灵
*/
struct MSG_81110005 : public Base_Msg  {
	uint32_t task_id;
	uint8_t money_type;
	uint32_t money_num;
	MSG_81110005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint8(money_type);
		w.write_uint32(money_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint8(money_type)  ||  r.read_uint32(money_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110005;
		task_id = 0;
		money_type = 0;
		money_num = 0;
	}
};

/*
自动提交的任务，通知客户端弹出奖励提示框
*/
struct MSG_81110006 : public Base_Msg  {
	uint32_t task_id;
	uint8_t is_window;
	MSG_81110006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint8(is_window);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint8(is_window)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110006;
		task_id = 0;
		is_window = 0;
	}
};

/*
主动推送悬赏任务列表
*/
struct MSG_81110007 : public Base_Msg  {
	uint8_t is_open_countdown;
	uint32_t auto_refresh_time;
	uint32_t free_refresh_count;
	uint32_t normal_accept_count;
	std::vector<Task_Info> reward_task_list;
	MSG_81110007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_open_countdown);
		w.write_uint32(auto_refresh_time);
		w.write_uint32(free_refresh_count);
		w.write_uint32(normal_accept_count);
		uint16_t __reward_task_list_vec_size = reward_task_list.size();
		w.write_uint16(__reward_task_list_vec_size);
		for(uint16_t i = 0; i < __reward_task_list_vec_size; ++i) {
				reward_task_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_open_countdown)  ||  r.read_uint32(auto_refresh_time)  ||  r.read_uint32(free_refresh_count)  ||  r.read_uint32(normal_accept_count)  )
				return -1;
 		uint16_t __reward_task_list_vec_size;
		if(r.read_uint16(__reward_task_list_vec_size)  )
				return -1;
		reward_task_list.reserve(__reward_task_list_vec_size);
		for(uint16_t i = 0; i < __reward_task_list_vec_size; ++i) {
				Task_Info v;
				if(v.deserialize(r))
						return -1;
				reward_task_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81110007;
		is_open_countdown = 0;
		auto_refresh_time = 0;
		free_refresh_count = 0;
		normal_accept_count = 0;
		reward_task_list.clear();
	}
};

/*
广播匹配双方信息
*/
struct MSG_80100320 : public Base_Msg  {
	std::vector<Team_Match_Info> roles;
	MSG_80100320(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __roles_vec_size = roles.size();
		w.write_uint16(__roles_vec_size);
		for(uint16_t i = 0; i < __roles_vec_size; ++i) {
				roles[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __roles_vec_size;
		if(r.read_uint16(__roles_vec_size)  )
				return -1;
		roles.reserve(__roles_vec_size);
		for(uint16_t i = 0; i < __roles_vec_size; ++i) {
				Team_Match_Info v;
				if(v.deserialize(r))
						return -1;
				roles.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100320;
		roles.clear();
	}
};

/*
匹配中
*/
struct MSG_80100321 : public Base_Msg  {
	int8_t ready;
	MSG_80100321(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(ready);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(ready)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100321;
		ready = 0;
	}
};

/*
竞技场玩家奖励信息
*/
struct MSG_81000303 : public Base_Msg  {
	uint32_t time;
	uint16_t local_rank;
	MSG_81000303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(time);
		w.write_uint16(local_rank);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(time)  ||  r.read_uint16(local_rank)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000303;
		time = 0;
		local_rank = 0;
	}
};

/*
广播组队竞技积分
*/
struct MSG_80100322 : public Base_Msg  {
	std::vector<Team_Sports_Integrate> ranking;
	int8_t level;
	int32_t score;
	int32_t score_max;
	int32_t rank;
	int32_t win;
	int32_t fail;
	int32_t times;
	MSG_80100322(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __ranking_vec_size = ranking.size();
		w.write_uint16(__ranking_vec_size);
		for(uint16_t i = 0; i < __ranking_vec_size; ++i) {
				ranking[i].serialize(w);
		}

		w.write_int8(level);
		w.write_int32(score);
		w.write_int32(score_max);
		w.write_int32(rank);
		w.write_int32(win);
		w.write_int32(fail);
		w.write_int32(times);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __ranking_vec_size;
		if(r.read_uint16(__ranking_vec_size)  )
				return -1;
		ranking.reserve(__ranking_vec_size);
		for(uint16_t i = 0; i < __ranking_vec_size; ++i) {
				Team_Sports_Integrate v;
				if(v.deserialize(r))
						return -1;
				ranking.push_back(v);
		}

		if( r.read_int8(level)  ||  r.read_int32(score)  ||  r.read_int32(score_max)  ||  r.read_int32(rank)  ||  r.read_int32(win)  ||  r.read_int32(fail)  ||  r.read_int32(times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100322;
		ranking.clear();
		level = 0;
		score = 0;
		score_max = 0;
		rank = 0;
		win = 0;
		fail = 0;
		times = 0;
	}
};

/*
战场信息
*/
struct MSG_81000400 : public Base_Msg  {
	uint16_t remain_time;
	uint16_t light_score;
	uint8_t light_num;
	uint16_t dark_score;
	uint8_t dark_num;
	Coord spe_mine_coord;
	uint16_t spe_mine_ref_time;
	uint16_t fight_time;
	MSG_81000400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(remain_time);
		w.write_uint16(light_score);
		w.write_uint8(light_num);
		w.write_uint16(dark_score);
		w.write_uint8(dark_num);
		spe_mine_coord.serialize(w);
		w.write_uint16(spe_mine_ref_time);
		w.write_uint16(fight_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(remain_time)  ||  r.read_uint16(light_score)  ||  r.read_uint8(light_num)  ||  r.read_uint16(dark_score)  ||  r.read_uint8(dark_num)  ||  spe_mine_coord.deserialize(r)  ||  r.read_uint16(spe_mine_ref_time)  ||  r.read_uint16(fight_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000400;
		remain_time = 0;
		light_score = 0;
		light_num = 0;
		dark_score = 0;
		dark_num = 0;
		spe_mine_coord.reset();
		spe_mine_ref_time = 0;
		fight_time = 0;
	}
};

/*
战场玩家信息更新
*/
struct MSG_81000401 : public Base_Msg  {
	War_Pro info;
	MSG_81000401(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000401;
		info.reset();
	}
};

/*
通知日常活跃度完成次数改变
*/
struct MSG_82200200 : public Base_Msg  {
	uint32_t type;
	uint32_t id;
	uint32_t num;
	uint32_t today_activity;
	uint32_t all_activity;
	uint8_t receive_vip_activity;
	MSG_82200200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(type);
		w.write_uint32(id);
		w.write_uint32(num);
		w.write_uint32(today_activity);
		w.write_uint32(all_activity);
		w.write_uint8(receive_vip_activity);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(type)  ||  r.read_uint32(id)  ||  r.read_uint32(num)  ||  r.read_uint32(today_activity)  ||  r.read_uint32(all_activity)  ||  r.read_uint8(receive_vip_activity)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200200;
		type = 0;
		id = 0;
		num = 0;
		today_activity = 0;
		all_activity = 0;
		receive_vip_activity = 0;
	}
};

/*
战场结算通知
*/
struct MSG_81000402 : public Base_Msg  {
	std::vector<War_Player_Rank_Info> rank_vec;
	int8_t win_league;
	int16_t merit_coe;
	int16_t piece_coe;
	MSG_81000402(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __rank_vec_vec_size = rank_vec.size();
		w.write_uint16(__rank_vec_vec_size);
		for(uint16_t i = 0; i < __rank_vec_vec_size; ++i) {
				rank_vec[i].serialize(w);
		}

		w.write_int8(win_league);
		w.write_int16(merit_coe);
		w.write_int16(piece_coe);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __rank_vec_vec_size;
		if(r.read_uint16(__rank_vec_vec_size)  )
				return -1;
		rank_vec.reserve(__rank_vec_vec_size);
		for(uint16_t i = 0; i < __rank_vec_vec_size; ++i) {
				War_Player_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				rank_vec.push_back(v);
		}

		if( r.read_int8(win_league)  ||  r.read_int16(merit_coe)  ||  r.read_int16(piece_coe)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000402;
		rank_vec.clear();
		win_league = 0;
		merit_coe = 0;
		piece_coe = 0;
	}
};

/*
战斗治疗成功通知
*/
struct MSG_81000403 : public Base_Msg  {
	int64_t role_id;
	std::vector<int64_t> healed_player_vec;
	MSG_81000403(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __healed_player_vec_vec_size = healed_player_vec.size();
		w.write_uint16(__healed_player_vec_vec_size);
		for(uint16_t i = 0; i < __healed_player_vec_vec_size; ++i) {
				w.write_int64(healed_player_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __healed_player_vec_vec_size;
		if(r.read_uint16(__healed_player_vec_vec_size)  )
				return -1;
		healed_player_vec.reserve(__healed_player_vec_vec_size);
		for(uint16_t i = 0; i < __healed_player_vec_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				healed_player_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000403;
		role_id = 0;
		healed_player_vec.clear();
	}
};

/*
战场信息更新
*/
struct MSG_81000404 : public Base_Msg  {
	uint8_t light_num;
	uint16_t light_score;
	uint8_t dark_num;
	uint16_t dark_score;
	Coord spe_mine_coord;
	uint16_t spe_mine_ref_time;
	MSG_81000404(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(light_num);
		w.write_uint16(light_score);
		w.write_uint8(dark_num);
		w.write_uint16(dark_score);
		spe_mine_coord.serialize(w);
		w.write_uint16(spe_mine_ref_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(light_num)  ||  r.read_uint16(light_score)  ||  r.read_uint8(dark_num)  ||  r.read_uint16(dark_score)  ||  spe_mine_coord.deserialize(r)  ||  r.read_uint16(spe_mine_ref_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000404;
		light_num = 0;
		light_score = 0;
		dark_num = 0;
		dark_score = 0;
		spe_mine_coord.reset();
		spe_mine_ref_time = 0;
	}
};

/*
战场玩家复活通知
*/
struct MSG_81000405 : public Base_Msg  {
	int64_t role_id;
	int8_t revive;
	MSG_81000405(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(revive);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(revive)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000405;
		role_id = 0;
		revive = 0;
	}
};

/*
英雄梦境开启列表
*/
struct MSG_80100323 : public Base_Msg  {
	std::vector<int32_t> scene_vec;
	MSG_80100323(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __scene_vec_vec_size = scene_vec.size();
		w.write_uint16(__scene_vec_vec_size);
		for(uint16_t i = 0; i < __scene_vec_vec_size; ++i) {
				w.write_int32(scene_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __scene_vec_vec_size;
		if(r.read_uint16(__scene_vec_vec_size)  )
				return -1;
		scene_vec.reserve(__scene_vec_vec_size);
		for(uint16_t i = 0; i < __scene_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				scene_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100323;
		scene_vec.clear();
	}
};

/*
进入自己龙谷，主动发送龙谷内的所有信息
*/
struct MSG_82300000 : public Base_Msg  {
	uint8_t is_build_gold_mine;
	uint8_t is_build_moon_well;
	uint8_t is_build_totem;
	uint8_t build_queue_num;
	uint8_t all_open_build_queue_num;
	std::vector<Building_Info> building_info_list;
	Dragon_Vale_Head_Info head_info;
	uint32_t protect_time;
	uint8_t is_dragon_born;
	uint32_t born_time;
	uint8_t progress;
	uint8_t is_play_dragon;
	uint8_t is_first_enter_after_robbed;
	MSG_82300000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_build_gold_mine);
		w.write_uint8(is_build_moon_well);
		w.write_uint8(is_build_totem);
		w.write_uint8(build_queue_num);
		w.write_uint8(all_open_build_queue_num);
		uint16_t __building_info_list_vec_size = building_info_list.size();
		w.write_uint16(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				building_info_list[i].serialize(w);
		}

		head_info.serialize(w);
		w.write_uint32(protect_time);
		w.write_uint8(is_dragon_born);
		w.write_uint32(born_time);
		w.write_uint8(progress);
		w.write_uint8(is_play_dragon);
		w.write_uint8(is_first_enter_after_robbed);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_build_gold_mine)  ||  r.read_uint8(is_build_moon_well)  ||  r.read_uint8(is_build_totem)  ||  r.read_uint8(build_queue_num)  ||  r.read_uint8(all_open_build_queue_num)  )
				return -1;
 		uint16_t __building_info_list_vec_size;
		if(r.read_uint16(__building_info_list_vec_size)  )
				return -1;
		building_info_list.reserve(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				Building_Info v;
				if(v.deserialize(r))
						return -1;
				building_info_list.push_back(v);
		}

		if( head_info.deserialize(r)  ||  r.read_uint32(protect_time)  ||  r.read_uint8(is_dragon_born)  ||  r.read_uint32(born_time)  ||  r.read_uint8(progress)  ||  r.read_uint8(is_play_dragon)  ||  r.read_uint8(is_first_enter_after_robbed)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300000;
		is_build_gold_mine = 0;
		is_build_moon_well = 0;
		is_build_totem = 0;
		build_queue_num = 0;
		all_open_build_queue_num = 0;
		building_info_list.clear();
		head_info.reset();
		protect_time = 0;
		is_dragon_born = 0;
		born_time = 0;
		progress = 0;
		is_play_dragon = 0;
		is_first_enter_after_robbed = 0;
	}
};

/*
主动推送建筑的资源产出
*/
struct MSG_82300001 : public Base_Msg  {
	std::vector<Building_Info> building_info_list;
	MSG_82300001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __building_info_list_vec_size = building_info_list.size();
		w.write_uint16(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				building_info_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __building_info_list_vec_size;
		if(r.read_uint16(__building_info_list_vec_size)  )
				return -1;
		building_info_list.reserve(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				Building_Info v;
				if(v.deserialize(r))
						return -1;
				building_info_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82300001;
		building_info_list.clear();
	}
};

/*
主动通知龙谷内的状态发生改变的建筑列表
备注： 玩家等级升级引起的变化、 建筑状态是否可升级变化、 可建造建筑数目增加、建造新建筑完成、升级完成  都会通知
*/
struct MSG_82300002 : public Base_Msg  {
	int64_t role_id;
	uint8_t is_build_gold_mine;
	uint8_t is_build_moon_well;
	uint8_t is_build_totem;
	uint8_t build_queue_num;
	std::vector<Building_Info> building_info_list;
	MSG_82300002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(is_build_gold_mine);
		w.write_uint8(is_build_moon_well);
		w.write_uint8(is_build_totem);
		w.write_uint8(build_queue_num);
		uint16_t __building_info_list_vec_size = building_info_list.size();
		w.write_uint16(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				building_info_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(is_build_gold_mine)  ||  r.read_uint8(is_build_moon_well)  ||  r.read_uint8(is_build_totem)  ||  r.read_uint8(build_queue_num)  )
				return -1;
 		uint16_t __building_info_list_vec_size;
		if(r.read_uint16(__building_info_list_vec_size)  )
				return -1;
		building_info_list.reserve(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				Building_Info v;
				if(v.deserialize(r))
						return -1;
				building_info_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82300002;
		role_id = 0;
		is_build_gold_mine = 0;
		is_build_moon_well = 0;
		is_build_totem = 0;
		build_queue_num = 0;
		building_info_list.clear();
	}
};

/*
玩家附加信息更新
*/
struct MSG_81000002 : public Base_Msg  {
	int64_t role_id;
	std::vector<Appear_Addition> add_info_vec;
	MSG_81000002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __add_info_vec_vec_size = add_info_vec.size();
		w.write_uint16(__add_info_vec_vec_size);
		for(uint16_t i = 0; i < __add_info_vec_vec_size; ++i) {
				add_info_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __add_info_vec_vec_size;
		if(r.read_uint16(__add_info_vec_vec_size)  )
				return -1;
		add_info_vec.reserve(__add_info_vec_vec_size);
		for(uint16_t i = 0; i < __add_info_vec_vec_size; ++i) {
				Appear_Addition v;
				if(v.deserialize(r))
						return -1;
				add_info_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000002;
		role_id = 0;
		add_info_vec.clear();
	}
};

/*
广播英雄梦境奖励
*/
struct MSG_81000082 : public Base_Msg  {
	int8_t type;
	std::vector<Reward_Info> award;
	MSG_81000082(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		uint16_t __award_vec_size = award.size();
		w.write_uint16(__award_vec_size);
		for(uint16_t i = 0; i < __award_vec_size; ++i) {
				award[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
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

		return 0;
	}

	void reset(void) {
		msg_id = 81000082;
		type = 0;
		award.clear();
	}
};

/*
TOD助手通知战斗力提升达到条件产于模块
*/
struct MSG_82200210 : public Base_Msg  {
	std::vector<uint32_t> model_type;
	MSG_82200210(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __model_type_vec_size = model_type.size();
		w.write_uint16(__model_type_vec_size);
		for(uint16_t i = 0; i < __model_type_vec_size; ++i) {
				w.write_uint32(model_type[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __model_type_vec_size;
		if(r.read_uint16(__model_type_vec_size)  )
				return -1;
		model_type.reserve(__model_type_vec_size);
		for(uint16_t i = 0; i < __model_type_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				model_type.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82200210;
		model_type.clear();
	}
};

/*
不足的物品列表
*/
struct MSG_82100008 : public Base_Msg  {
	std::vector<int32_t> item_ids;
	MSG_82100008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_ids_vec_size = item_ids.size();
		w.write_uint16(__item_ids_vec_size);
		for(uint16_t i = 0; i < __item_ids_vec_size; ++i) {
				w.write_int32(item_ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __item_ids_vec_size;
		if(r.read_uint16(__item_ids_vec_size)  )
				return -1;
		item_ids.reserve(__item_ids_vec_size);
		for(uint16_t i = 0; i < __item_ids_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				item_ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82100008;
		item_ids.clear();
	}
};

/*
进入别人龙谷，主动推送龙谷内的信息
*/
struct MSG_82300003 : public Base_Msg  {
	std::vector<Building_Info> building_info_list;
	Dragon_Vale_Head_Info head_info;
	uint8_t is_dragon_born;
	uint32_t born_time;
	uint8_t progress;
	uint8_t is_play_dragon;
	uint8_t is_can_step_foot;
	MSG_82300003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __building_info_list_vec_size = building_info_list.size();
		w.write_uint16(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				building_info_list[i].serialize(w);
		}

		head_info.serialize(w);
		w.write_uint8(is_dragon_born);
		w.write_uint32(born_time);
		w.write_uint8(progress);
		w.write_uint8(is_play_dragon);
		w.write_uint8(is_can_step_foot);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __building_info_list_vec_size;
		if(r.read_uint16(__building_info_list_vec_size)  )
				return -1;
		building_info_list.reserve(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				Building_Info v;
				if(v.deserialize(r))
						return -1;
				building_info_list.push_back(v);
		}

		if( head_info.deserialize(r)  ||  r.read_uint8(is_dragon_born)  ||  r.read_uint32(born_time)  ||  r.read_uint8(progress)  ||  r.read_uint8(is_play_dragon)  ||  r.read_uint8(is_can_step_foot)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300003;
		building_info_list.clear();
		head_info.reset();
		is_dragon_born = 0;
		born_time = 0;
		progress = 0;
		is_play_dragon = 0;
		is_can_step_foot = 0;
	}
};

/*
聊天广播过快
*/
struct MSG_83000002 : public Base_Msg  {
	MSG_83000002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 83000002;
	}
};

/*
孵化龙蛋完成，主动通知
*/
struct MSG_82300004 : public Base_Msg  {
	uint8_t progress;
	uint8_t is_play_dragon;
	MSG_82300004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(progress);
		w.write_uint8(is_play_dragon);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(progress)  ||  r.read_uint8(is_play_dragon)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300004;
		progress = 0;
		is_play_dragon = 0;
	}
};

/*
时间消耗、被掠夺，龙的心情值下降，主动通知
*/
struct MSG_82300005 : public Base_Msg  {
	uint8_t type;
	uint8_t feeling;
	MSG_82300005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint8(feeling);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint8(feeling)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300005;
		type = 0;
		feeling = 0;
	}
};

/*
主动推送精灵盟约状态
*/
struct MSG_82200220 : public Base_Msg  {
	std::vector<Covenant_Status> covenant;
	MSG_82200220(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __covenant_vec_size = covenant.size();
		w.write_uint16(__covenant_vec_size);
		for(uint16_t i = 0; i < __covenant_vec_size; ++i) {
				covenant[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __covenant_vec_size;
		if(r.read_uint16(__covenant_vec_size)  )
				return -1;
		covenant.reserve(__covenant_vec_size);
		for(uint16_t i = 0; i < __covenant_vec_size; ++i) {
				Covenant_Status v;
				if(v.deserialize(r))
						return -1;
				covenant.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82200220;
		covenant.clear();
	}
};

/*
进化龙不足，提示是否愿意消耗多少货币
*/
struct MSG_82300006 : public Base_Msg  {
	uint8_t monye_type;
	uint32_t money_num;
	MSG_82300006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(monye_type);
		w.write_uint32(money_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(monye_type)  ||  r.read_uint32(money_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300006;
		monye_type = 0;
		money_num = 0;
	}
};

/*
战场飘字通知
备注：type
1：被击杀信息：损失25积分
2：击杀信息：你击杀了XX，获取了20功勋，20积分。
3：运送：成功提交资源，获得了10功勋，100积分，粉钻宝箱碎片X5。
4：战场获胜：战场获胜，你累计获得了110功勋，200积分, 100竞技硬币。
5：战场失败：战场失败，你累计获得了110功勋，200积分, 100竞技硬币。
6：助攻：您协助XXX击杀了XX，获得了10功勋。
7：女神祝福：您获得女神祝福。
8：采集时间：复活时间缩短1秒

*/
struct MSG_81000406 : public Base_Msg  {
	int8_t type;
	std::string name1;
	std::string name2;
	int32_t score;
	int32_t merit;
	int32_t piece;
	int32_t arena_coin;
	int32_t rep;
	MSG_81000406(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_string(name1);
		w.write_string(name2);
		w.write_int32(score);
		w.write_int32(merit);
		w.write_int32(piece);
		w.write_int32(arena_coin);
		w.write_int32(rep);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_string(name1)  ||  r.read_string(name2)  ||  r.read_int32(score)  ||  r.read_int32(merit)  ||  r.read_int32(piece)  ||  r.read_int32(arena_coin)  ||  r.read_int32(rep)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000406;
		type = 0;
		name1.clear();
		name2.clear();
		score = 0;
		merit = 0;
		piece = 0;
		arena_coin = 0;
		rep = 0;
	}
};

/*
倒计时结束，由不可逗龙变为可以逗龙，主动通知
*/
struct MSG_82300007 : public Base_Msg  {
	int64_t role_id;
	uint8_t is_play_dragon;
	MSG_82300007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(is_play_dragon);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(is_play_dragon)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300007;
		role_id = 0;
		is_play_dragon = 0;
	}
};

/*
战场踢人通知
*/
struct MSG_81000407 : public Base_Msg  {
	int8_t reason;
	MSG_81000407(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(reason);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(reason)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000407;
		reason = 0;
	}
};

/*
通知客户端，使用悬赏精灵完成悬赏任务
*/
struct MSG_81110008 : public Base_Msg  {
	uint32_t task_id;
	MSG_81110008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110008;
		task_id = 0;
	}
};

/*
 通知被赠送精灵盟约
*/
struct MSG_82200221 : public Base_Msg  {
	std::vector<uint32_t> id;
	std::string role_name;
	MSG_82200221(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __id_vec_size = id.size();
		w.write_uint16(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				w.write_uint32(id[i]);
		}

		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __id_vec_size;
		if(r.read_uint16(__id_vec_size)  )
				return -1;
		id.reserve(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				id.push_back(v);
		}

		if( r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200221;
		id.clear();
		role_name.clear();
	}
};

/*
通知精灵盟约过期
*/
struct MSG_82200222 : public Base_Msg  {
	std::vector<uint32_t> id;
	MSG_82200222(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __id_vec_size = id.size();
		w.write_uint16(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				w.write_uint32(id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __id_vec_size;
		if(r.read_uint16(__id_vec_size)  )
				return -1;
		id.reserve(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82200222;
		id.clear();
	}
};

/*
龙谷系统资源存储达到上限，主动通知不再产出
备注：在龙谷内、不在龙谷内都会发送这条消息。 如果玩家在线，直接发；如果不在线，下次登录，发送。
*/
struct MSG_82300008 : public Base_Msg  {
	uint8_t is_gold_mine;
	uint8_t is_moon_weel;
	uint8_t is_totem;
	MSG_82300008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_gold_mine);
		w.write_uint8(is_moon_weel);
		w.write_uint8(is_totem);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_gold_mine)  ||  r.read_uint8(is_moon_weel)  ||  r.read_uint8(is_totem)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300008;
		is_gold_mine = 0;
		is_moon_weel = 0;
		is_totem = 0;
	}
};

/*
建筑建造或者升级完成提示
备注：在龙谷内、不在龙谷内都会发送这条消息。 如果玩家在线，直接发；如果不在线，下次登录，发送。
*/
struct MSG_82300009 : public Base_Msg  {
	uint8_t gold_mine_flag;
	uint8_t moon_well_flag;
	uint8_t totem_flag;
	MSG_82300009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(gold_mine_flag);
		w.write_uint8(moon_well_flag);
		w.write_uint8(totem_flag);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(gold_mine_flag)  ||  r.read_uint8(moon_well_flag)  ||  r.read_uint8(totem_flag)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300009;
		gold_mine_flag = 0;
		moon_well_flag = 0;
		totem_flag = 0;
	}
};

/*
活动图标变更信息通知
icon_id：开启的图标
0：战场
1：公会战
2：VIP世界之树
3：篝火晚会
6：装备图鉴
10:投资计划
11:组队竞技
*/
struct MSG_81000408 : public Base_Msg  {
	int8_t open;
	int8_t active;
	uint32_t time;
	int32_t icon_id;
	int32_t ex_data;
	MSG_81000408(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(open);
		w.write_int8(active);
		w.write_uint32(time);
		w.write_int32(icon_id);
		w.write_int32(ex_data);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(open)  ||  r.read_int8(active)  ||  r.read_uint32(time)  ||  r.read_int32(icon_id)  ||  r.read_int32(ex_data)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000408;
		open = 0;
		active = 0;
		time = 0;
		icon_id = 0;
		ex_data = 0;
	}
};

/*
成就完成
*/
struct MSG_81100800 : public Base_Msg  {
	std::vector<Achievement_Detail_Msg> list;
	MSG_81100800(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Achievement_Detail_Msg v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81100800;
		list.clear();
	}
};

/*
主动通知龙谷主人被踩脚印
备注：只有在星级提升的踩脚印，才会发给龙谷主人，客户端弹窗提示。  
*/
struct MSG_82300010 : public Base_Msg  {
	uint8_t is_special;
	std::string name;
	uint8_t star;
	uint32_t dragon_heart;
	Dragon_Vale_Head_Info head_info;
	MSG_82300010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_special);
		w.write_string(name);
		w.write_uint8(star);
		w.write_uint32(dragon_heart);
		head_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_special)  ||  r.read_string(name)  ||  r.read_uint8(star)  ||  r.read_uint32(dragon_heart)  ||  head_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300010;
		is_special = 0;
		name.clear();
		star = 0;
		dragon_heart = 0;
		head_info.reset();
	}
};

/*
通知VIP状态信息
*/
struct MSG_82200230 : public Base_Msg  {
	uint32_t vip_level;
	uint32_t vip_overtime;
	uint32_t exp_overtime;
	uint32_t flock_vip_level;
	uint32_t grow_val;
	uint8_t active_type;
	uint8_t daily_rec;
	MSG_82200230(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(vip_level);
		w.write_uint32(vip_overtime);
		w.write_uint32(exp_overtime);
		w.write_uint32(flock_vip_level);
		w.write_uint32(grow_val);
		w.write_uint8(active_type);
		w.write_uint8(daily_rec);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(vip_level)  ||  r.read_uint32(vip_overtime)  ||  r.read_uint32(exp_overtime)  ||  r.read_uint32(flock_vip_level)  ||  r.read_uint32(grow_val)  ||  r.read_uint8(active_type)  ||  r.read_uint8(daily_rec)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200230;
		vip_level = 0;
		vip_overtime = 0;
		exp_overtime = 0;
		flock_vip_level = 0;
		grow_val = 0;
		active_type = 0;
		daily_rec = 0;
	}
};

/*
主动通知人物面板英雄标签更新
*/
struct MSG_82210001 : public Base_Msg  {
	std::vector<int32_t> panal_heros_tags;
	MSG_82210001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __panal_heros_tags_vec_size = panal_heros_tags.size();
		w.write_uint16(__panal_heros_tags_vec_size);
		for(uint16_t i = 0; i < __panal_heros_tags_vec_size; ++i) {
				w.write_int32(panal_heros_tags[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __panal_heros_tags_vec_size;
		if(r.read_uint16(__panal_heros_tags_vec_size)  )
				return -1;
		panal_heros_tags.reserve(__panal_heros_tags_vec_size);
		for(uint16_t i = 0; i < __panal_heros_tags_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				panal_heros_tags.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82210001;
		panal_heros_tags.clear();
	}
};

/*
战斗连击统计广播
*/
struct MSG_81000065 : public Base_Msg  {
	int8_t pos;
	int32_t times;
	double hurt_ttl;
	MSG_81000065(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(pos);
		w.write_int32(times);
		w.write_double(hurt_ttl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(pos)  ||  r.read_int32(times)  ||  r.read_double(hurt_ttl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000065;
		pos = 0;
		times = 0;
		hurt_ttl = 0.0;
	}
};

/*
有人来掠夺龙谷，给龙谷主人提示，是否应战
*/
struct MSG_82300012 : public Base_Msg  {
	int64_t uuid;
	std::string server_name;
	int64_t role_id;
	std::string role_name;
	uint8_t limit_time;
	MSG_82300012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(uuid);
		w.write_string(server_name);
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_uint8(limit_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(uuid)  ||  r.read_string(server_name)  ||  r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_uint8(limit_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300012;
		uuid = 0;
		server_name.clear();
		role_id = 0;
		role_name.clear();
		limit_time = 0;
	}
};

/*
通知龙谷掠夺者进入战斗倒计时
*/
struct MSG_82300013 : public Base_Msg  {
	uint8_t limit_time;
	MSG_82300013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(limit_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(limit_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300013;
		limit_time = 0;
	}
};

/*
通知掠夺者掠夺战斗结束
*/
struct MSG_82300014 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint8_t limit_time;
	uint8_t result;
	int32_t uuid;
	int64_t role_id;
	MSG_82300014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint8(limit_time);
		w.write_uint8(result);
		w.write_int32(uuid);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint8(limit_time)  ||  r.read_uint8(result)  ||  r.read_int32(uuid)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300014;
		server_name.clear();
		player_name.clear();
		limit_time = 0;
		result = 0;
		uuid = 0;
		role_id = 0;
	}
};

/*
通知被掠夺龙谷的主人，战斗结束
*/
struct MSG_82300015 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint8_t result;
	uint8_t is_drive;
	MSG_82300015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint8(result);
		w.write_uint8(is_drive);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint8(result)  ||  r.read_uint8(is_drive)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300015;
		server_name.clear();
		player_name.clear();
		result = 0;
		is_drive = 0;
	}
};

/*
主动通知掠夺者在被掠夺者的跨服龙谷内收获了多少资源
*/
struct MSG_82300017 : public Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string player_name;
	uint32_t total_gold_mine;
	uint32_t total_moon_weel;
	uint32_t total_totem;
	MSG_82300017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint32(total_gold_mine);
		w.write_uint32(total_moon_weel);
		w.write_uint32(total_totem);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint32(total_gold_mine)  ||  r.read_uint32(total_moon_weel)  ||  r.read_uint32(total_totem)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300017;
		to_uuid = 0;
		to_role_id = 0;
		uuid = 0;
		role_id = 0;
		server_name.clear();
		player_name.clear();
		total_gold_mine = 0;
		total_moon_weel = 0;
		total_totem = 0;
	}
};

/*
主动通知龙谷主人被掠夺了多少资源
*/
struct MSG_82300018 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint32_t total_gold_mine;
	uint32_t total_moon_weel;
	uint32_t total_totem;
	MSG_82300018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint32(total_gold_mine);
		w.write_uint32(total_moon_weel);
		w.write_uint32(total_totem);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint32(total_gold_mine)  ||  r.read_uint32(total_moon_weel)  ||  r.read_uint32(total_totem)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300018;
		server_name.clear();
		player_name.clear();
		total_gold_mine = 0;
		total_moon_weel = 0;
		total_totem = 0;
	}
};

/*
更新成就状态
*/
struct MSG_81100801 : public Base_Msg  {
	int8_t op;
	std::vector<Achievement_Detail_Msg> list;
	MSG_81100801(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(op);
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(op)  )
				return -1;
 		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Achievement_Detail_Msg v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81100801;
		op = 0;
		list.clear();
	}
};

/*
龙谷主人的建筑资源被掠夺者收获，更新建筑信息(预留 一键收获掠夺资源的返回))
*/
struct MSG_82300019 : public Base_Msg  {
	std::vector<Building_Info> building_info;
	MSG_82300019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __building_info_vec_size = building_info.size();
		w.write_uint16(__building_info_vec_size);
		for(uint16_t i = 0; i < __building_info_vec_size; ++i) {
				building_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __building_info_vec_size;
		if(r.read_uint16(__building_info_vec_size)  )
				return -1;
		building_info.reserve(__building_info_vec_size);
		for(uint16_t i = 0; i < __building_info_vec_size; ++i) {
				Building_Info v;
				if(v.deserialize(r))
						return -1;
				building_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82300019;
		building_info.clear();
	}
};

/*
avatar更改通知
*/
struct MSG_81000003 : public Base_Msg  {
	int64_t role_id;
	std::vector<int32_t> avatar;
	MSG_81000003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __avatar_vec_size = avatar.size();
		w.write_uint16(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				w.write_int32(avatar[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
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

		return 0;
	}

	void reset(void) {
		msg_id = 81000003;
		role_id = 0;
		avatar.clear();
	}
};

/*
玩家获得称号
*/
struct MSG_81000900 : public Base_Msg  {
	int32_t title_id;
	int8_t equip;
	int32_t time;
	MSG_81000900(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(title_id);
		w.write_int8(equip);
		w.write_int32(time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(title_id)  ||  r.read_int8(equip)  ||  r.read_int32(time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000900;
		title_id = 0;
		equip = 0;
		time = 0;
	}
};

/*
玩家失去称号
*/
struct MSG_81000901 : public Base_Msg  {
	int32_t title_id;
	MSG_81000901(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(title_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(title_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000901;
		title_id = 0;
	}
};

/*
玩家称号更新
此消息用来广播玩家的称号信息给周围玩家
*/
struct MSG_81000902 : public Base_Msg  {
	int64_t role_id;
	int32_t equip_title;
	MSG_81000902(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(equip_title);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(equip_title)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000902;
		role_id = 0;
		equip_title = 0;
	}
};

/*
掠夺者进入被掠夺者的龙谷
*/
struct MSG_82300020 : public Base_Msg  {
	int64_t to_uuid;
	uint8_t limit_time;
	int64_t uuid;
	std::vector<Building_Info> builing_info_list;
	Dragon_Vale_Head_Info head_info;
	uint8_t is_dragon_born;
	uint32_t born_time;
	uint8_t progress;
	uint8_t is_paly_dragon;
	uint32_t total_gold_mine;
	uint32_t total_moon_weel;
	uint32_t total_totem;
	MSG_82300020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_uint8(limit_time);
		w.write_int64(uuid);
		uint16_t __builing_info_list_vec_size = builing_info_list.size();
		w.write_uint16(__builing_info_list_vec_size);
		for(uint16_t i = 0; i < __builing_info_list_vec_size; ++i) {
				builing_info_list[i].serialize(w);
		}

		head_info.serialize(w);
		w.write_uint8(is_dragon_born);
		w.write_uint32(born_time);
		w.write_uint8(progress);
		w.write_uint8(is_paly_dragon);
		w.write_uint32(total_gold_mine);
		w.write_uint32(total_moon_weel);
		w.write_uint32(total_totem);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_uint8(limit_time)  ||  r.read_int64(uuid)  )
				return -1;
 		uint16_t __builing_info_list_vec_size;
		if(r.read_uint16(__builing_info_list_vec_size)  )
				return -1;
		builing_info_list.reserve(__builing_info_list_vec_size);
		for(uint16_t i = 0; i < __builing_info_list_vec_size; ++i) {
				Building_Info v;
				if(v.deserialize(r))
						return -1;
				builing_info_list.push_back(v);
		}

		if( head_info.deserialize(r)  ||  r.read_uint8(is_dragon_born)  ||  r.read_uint32(born_time)  ||  r.read_uint8(progress)  ||  r.read_uint8(is_paly_dragon)  ||  r.read_uint32(total_gold_mine)  ||  r.read_uint32(total_moon_weel)  ||  r.read_uint32(total_totem)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300020;
		to_uuid = 0;
		limit_time = 0;
		uuid = 0;
		builing_info_list.clear();
		head_info.reset();
		is_dragon_born = 0;
		born_time = 0;
		progress = 0;
		is_paly_dragon = 0;
		total_gold_mine = 0;
		total_moon_weel = 0;
		total_totem = 0;
	}
};

/*
VIP等级改变，主动通知建筑队列数目改变
*/
struct MSG_82300022 : public Base_Msg  {
	uint8_t all_open_build_queue_num;
	uint8_t build_queue_num;
	MSG_82300022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(all_open_build_queue_num);
		w.write_uint8(build_queue_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(all_open_build_queue_num)  ||  r.read_uint8(build_queue_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300022;
		all_open_build_queue_num = 0;
		build_queue_num = 0;
	}
};

/*
通知收到鲜花
*/
struct MSG_82200240 : public Base_Msg  {
	std::vector<Flowers_Info> info;
	MSG_82200240(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Flowers_Info v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82200240;
		info.clear();
	}
};

/*
通知全屏飘花效果
*/
struct MSG_82200241 : public Base_Msg  {
	MSG_82200241(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 82200241;
	}
};

/*
公会战倒计时
*/
struct MSG_81170000 : public Base_Msg  {
	int8_t type;
	int32_t end_time;
	MSG_81170000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(end_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(end_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81170000;
		type = 0;
		end_time = 0;
	}
};

/*
公会战个人信息
*/
struct MSG_81170001 : public Base_Msg  {
	std::string gang_name;
	int64_t role_id;
	int8_t camp;
	int32_t kill_num;
	int32_t hold_spot;
	int32_t contribute;
	int32_t mercit_cur;
	int32_t mercit_all;
	int32_t revive_time;
	int64_t leader_id;
	int32_t skill_cd;
	int64_t leader_id_other;
	int32_t wait_time;
	MSG_81170001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(gang_name);
		w.write_int64(role_id);
		w.write_int8(camp);
		w.write_int32(kill_num);
		w.write_int32(hold_spot);
		w.write_int32(contribute);
		w.write_int32(mercit_cur);
		w.write_int32(mercit_all);
		w.write_int32(revive_time);
		w.write_int64(leader_id);
		w.write_int32(skill_cd);
		w.write_int64(leader_id_other);
		w.write_int32(wait_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(gang_name)  ||  r.read_int64(role_id)  ||  r.read_int8(camp)  ||  r.read_int32(kill_num)  ||  r.read_int32(hold_spot)  ||  r.read_int32(contribute)  ||  r.read_int32(mercit_cur)  ||  r.read_int32(mercit_all)  ||  r.read_int32(revive_time)  ||  r.read_int64(leader_id)  ||  r.read_int32(skill_cd)  ||  r.read_int64(leader_id_other)  ||  r.read_int32(wait_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81170001;
		gang_name.clear();
		role_id = 0;
		camp = 0;
		kill_num = 0;
		hold_spot = 0;
		contribute = 0;
		mercit_cur = 0;
		mercit_all = 0;
		revive_time = 0;
		leader_id = 0;
		skill_cd = 0;
		leader_id_other = 0;
		wait_time = 0;
	}
};

/*
公会战信息
*/
struct MSG_81170002 : public Base_Msg  {
	std::vector<Gang_War_Total_Info> info;
	MSG_81170002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Gang_War_Total_Info v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81170002;
		info.clear();
	}
};

/*
公会战采集主动信息
*/
struct MSG_81170003 : public Base_Msg  {
	int32_t people_num;
	int32_t gather_time;
	int32_t gather_time_all;
	MSG_81170003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(people_num);
		w.write_int32(gather_time);
		w.write_int32(gather_time_all);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(people_num)  ||  r.read_int32(gather_time)  ||  r.read_int32(gather_time_all)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81170003;
		people_num = 0;
		gather_time = 0;
		gather_time_all = 0;
	}
};

/*
公会战奖励
*/
struct MSG_81170004 : public Base_Msg  {
	std::vector<Reward_Info_Gang_War> gang;
	std::vector<Reward_Info> self;
	int8_t result;
	MSG_81170004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __gang_vec_size = gang.size();
		w.write_uint16(__gang_vec_size);
		for(uint16_t i = 0; i < __gang_vec_size; ++i) {
				gang[i].serialize(w);
		}

		uint16_t __self_vec_size = self.size();
		w.write_uint16(__self_vec_size);
		for(uint16_t i = 0; i < __self_vec_size; ++i) {
				self[i].serialize(w);
		}

		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __gang_vec_size;
		if(r.read_uint16(__gang_vec_size)  )
				return -1;
		gang.reserve(__gang_vec_size);
		for(uint16_t i = 0; i < __gang_vec_size; ++i) {
				Reward_Info_Gang_War v;
				if(v.deserialize(r))
						return -1;
				gang.push_back(v);
		}

		uint16_t __self_vec_size;
		if(r.read_uint16(__self_vec_size)  )
				return -1;
		self.reserve(__self_vec_size);
		for(uint16_t i = 0; i < __self_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				self.push_back(v);
		}

		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81170004;
		gang.clear();
		self.clear();
		result = 0;
	}
};

/*
主动更新积分
*/
struct MSG_81080000 : public Base_Msg  {
	int32_t poins;
	MSG_81080000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(poins);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(poins)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81080000;
		poins = 0;
	}
};

/*
服务器请求飘字
*/
struct MSG_81003000 : public Base_Msg  {
	int8_t type;
	std::string text;
	MSG_81003000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_string(text);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_string(text)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81003000;
		type = 0;
		text.clear();
	}
};

/*
通知签到信息
*/
struct MSG_82200201 : public Base_Msg  {
	uint32_t refresh_time;
	uint32_t check_in_day;
	uint8_t today_is_check_in;
	uint32_t group_id;
	MSG_82200201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(refresh_time);
		w.write_uint32(check_in_day);
		w.write_uint8(today_is_check_in);
		w.write_uint32(group_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(refresh_time)  ||  r.read_uint32(check_in_day)  ||  r.read_uint8(today_is_check_in)  ||  r.read_uint32(group_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200201;
		refresh_time = 0;
		check_in_day = 0;
		today_is_check_in = 0;
		group_id = 0;
	}
};

/*
通知在线奖励状态信息
*/
struct MSG_82200202 : public Base_Msg  {
	uint8_t online_reward_over;
	uint32_t online_id;
	uint32_t online_reward_time;
	MSG_82200202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(online_reward_over);
		w.write_uint32(online_id);
		w.write_uint32(online_reward_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(online_reward_over)  ||  r.read_uint32(online_id)  ||  r.read_uint32(online_reward_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200202;
		online_reward_over = 0;
		online_id = 0;
		online_reward_time = 0;
	}
};

/*
公会战资源塔信息
采集点从左往右，从上往下排
数组按顺序排
*/
struct MSG_81170005 : public Base_Msg  {
	std::vector<Gang_War_Tower_Detail> tower_vec;
	MSG_81170005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __tower_vec_vec_size = tower_vec.size();
		w.write_uint16(__tower_vec_vec_size);
		for(uint16_t i = 0; i < __tower_vec_vec_size; ++i) {
				tower_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __tower_vec_vec_size;
		if(r.read_uint16(__tower_vec_vec_size)  )
				return -1;
		tower_vec.reserve(__tower_vec_vec_size);
		for(uint16_t i = 0; i < __tower_vec_vec_size; ++i) {
				Gang_War_Tower_Detail v;
				if(v.deserialize(r))
						return -1;
				tower_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81170005;
		tower_vec.clear();
	}
};

/*
掠夺者玩家上线，通知掠夺战斗结果
*/
struct MSG_82300023 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint8_t result;
	MSG_82300023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300023;
		server_name.clear();
		player_name.clear();
		result = 0;
	}
};

/*
被掠夺者玩家上线，通知掠夺战斗结果
*/
struct MSG_82300024 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint8_t result;
	MSG_82300024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300024;
		server_name.clear();
		player_name.clear();
		result = 0;
	}
};

/*
更改采集时间通知
*/
struct MSG_81000111 : public Base_Msg  {
	int64_t role_id;
	int16_t total_time;
	int16_t remain_time;
	int16_t gather_num;
	MSG_81000111(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int16(total_time);
		w.write_int16(remain_time);
		w.write_int16(gather_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int16(total_time)  ||  r.read_int16(remain_time)  ||  r.read_int16(gather_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000111;
		role_id = 0;
		total_time = 0;
		remain_time = 0;
		gather_num = 0;
	}
};

/*
广播剧情信息
*/
struct MSG_81110009 : public Base_Msg  {
	std::vector<Plot_Info> info_list;
	MSG_81110009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_list_vec_size = info_list.size();
		w.write_uint16(__info_list_vec_size);
		for(uint16_t i = 0; i < __info_list_vec_size; ++i) {
				info_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_list_vec_size;
		if(r.read_uint16(__info_list_vec_size)  )
				return -1;
		info_list.reserve(__info_list_vec_size);
		for(uint16_t i = 0; i < __info_list_vec_size; ++i) {
				Plot_Info v;
				if(v.deserialize(r))
						return -1;
				info_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81110009;
		info_list.clear();
	}
};

/*
被掠夺者上线，通知龙谷资源被掠夺了多少
*/
struct MSG_82300025 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint32_t total_gold_mine;
	uint32_t total_moon_weel;
	uint32_t total_totem;
	MSG_82300025(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint32(total_gold_mine);
		w.write_uint32(total_moon_weel);
		w.write_uint32(total_totem);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint32(total_gold_mine)  ||  r.read_uint32(total_moon_weel)  ||  r.read_uint32(total_totem)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300025;
		server_name.clear();
		player_name.clear();
		total_gold_mine = 0;
		total_moon_weel = 0;
		total_totem = 0;
	}
};

/*
篝火信息变更通知
*/
struct MSG_81000700 : public Base_Msg  {
	Campfire_Info campfire_info;
	Coord coord;
	MSG_81000700(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		campfire_info.serialize(w);
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( campfire_info.deserialize(r)  ||  coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000700;
		campfire_info.reset();
		coord.reset();
	}
};

/*
龙谷正在被掠夺，不可进入
*/
struct MSG_82300026 : public Base_Msg  {
	uint32_t left_time;
	MSG_82300026(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(left_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(left_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300026;
		left_time = 0;
	}
};

/*
公会战战斗治疗成功通知
*/
struct MSG_81170006 : public Base_Msg  {
	int64_t role_id;
	std::vector<int64_t> healed_player_vec;
	MSG_81170006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __healed_player_vec_vec_size = healed_player_vec.size();
		w.write_uint16(__healed_player_vec_vec_size);
		for(uint16_t i = 0; i < __healed_player_vec_vec_size; ++i) {
				w.write_int64(healed_player_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __healed_player_vec_vec_size;
		if(r.read_uint16(__healed_player_vec_vec_size)  )
				return -1;
		healed_player_vec.reserve(__healed_player_vec_vec_size);
		for(uint16_t i = 0; i < __healed_player_vec_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				healed_player_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81170006;
		role_id = 0;
		healed_player_vec.clear();
	}
};

/*
公会战飘字通知
type 
1：击杀信息：获取了xx功勋，xx公会贡献。
2：采集时间：复活时间缩短1秒
3：被击杀信息：损失25积分  
4：战场获胜：战场获胜，你累计获得了110功勋，200积分。 
5：战场失败：战场失败，你累计获得了110功勋，200积分。 
5：助攻：您协助XXX击杀了XX，获得了10功勋。 
*/
struct MSG_81170007 : public Base_Msg  {
	int8_t type;
	std::string name1;
	std::string name2;
	int32_t merit;
	int32_t contribute;
	MSG_81170007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_string(name1);
		w.write_string(name2);
		w.write_int32(merit);
		w.write_int32(contribute);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_string(name1)  ||  r.read_string(name2)  ||  r.read_int32(merit)  ||  r.read_int32(contribute)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81170007;
		type = 0;
		name1.clear();
		name2.clear();
		merit = 0;
		contribute = 0;
	}
};

/*
掠夺匹配失败
*/
struct MSG_82300027 : public Base_Msg  {
	int32_t error_id;
	uint32_t coin;
	MSG_82300027(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(error_id);
		w.write_uint32(coin);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(error_id)  ||  r.read_uint32(coin)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300027;
		error_id = 0;
		coin = 0;
	}
};

/*
公会战弹框提示
type:
1：公会战开始提示
2：协助提示
3：提示决斗
*/
struct MSG_81170008 : public Base_Msg  {
	int8_t type;
	int64_t role_id;
	MSG_81170008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81170008;
		type = 0;
		role_id = 0;
	}
};

/*
提示公会战决斗
*/
struct MSG_81170009 : public Base_Msg  {
	int64_t role_id;
	std::string role_name;
	MSG_81170009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81170009;
		role_id = 0;
		role_name.clear();
	}
};

/*
恭喜你成为成为至尊宝
*/
struct MSG_82300028 : public Base_Msg  {
	MSG_82300028(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 82300028;
	}
};

/*
通知客户端弹出接任务、提交任务特效
*/
struct MSG_82300029 : public Base_Msg  {
	uint32_t task_id;
	uint8_t type;
	MSG_82300029(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300029;
		task_id = 0;
		type = 0;
	}
};

/*
主动推送龙谷头像区信息改变通知
*/
struct MSG_82300030 : public Base_Msg  {
	Dragon_Vale_Head_Info head_info;
	MSG_82300030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		head_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( head_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300030;
		head_info.reset();
	}
};

/*
被踩脚印的人，上线后，提示被踩并且星级提升
备注：只有在踩脚印得到星级提升的时候，才会发
*/
struct MSG_82300031 : public Base_Msg  {
	uint8_t is_special;
	std::string name;
	uint8_t star;
	uint32_t dragon_heart;
	MSG_82300031(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_special);
		w.write_string(name);
		w.write_uint8(star);
		w.write_uint32(dragon_heart);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_special)  ||  r.read_string(name)  ||  r.read_uint8(star)  ||  r.read_uint32(dragon_heart)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300031;
		is_special = 0;
		name.clear();
		star = 0;
		dragon_heart = 0;
	}
};

/*
字符串信息改变更新
*/
struct MSG_81000066 : public Base_Msg  {
	std::vector<String_Info_Change> change_info;
	int64_t role_id;
	MSG_81000066(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __change_info_vec_size = change_info.size();
		w.write_uint16(__change_info_vec_size);
		for(uint16_t i = 0; i < __change_info_vec_size; ++i) {
				change_info[i].serialize(w);
		}

		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __change_info_vec_size;
		if(r.read_uint16(__change_info_vec_size)  )
				return -1;
		change_info.reserve(__change_info_vec_size);
		for(uint16_t i = 0; i < __change_info_vec_size; ++i) {
				String_Info_Change v;
				if(v.deserialize(r))
						return -1;
				change_info.push_back(v);
		}

		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000066;
		change_info.clear();
		role_id = 0;
	}
};

/*
玩家在主城中，显示龙谷龙蛋孵化剩余时间
*/
struct MSG_82300032 : public Base_Msg  {
	uint32_t left_time;
	MSG_82300032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(left_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(left_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300032;
		left_time = 0;
	}
};

/*
重新登录扫荡副本id通知
*/
struct MSG_81000409 : public Base_Msg  {
	int8_t wipe_type;
	std::vector<int32_t> wipe_scenes;
	MSG_81000409(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(wipe_type);
		uint16_t __wipe_scenes_vec_size = wipe_scenes.size();
		w.write_uint16(__wipe_scenes_vec_size);
		for(uint16_t i = 0; i < __wipe_scenes_vec_size; ++i) {
				w.write_int32(wipe_scenes[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(wipe_type)  )
				return -1;
 		uint16_t __wipe_scenes_vec_size;
		if(r.read_uint16(__wipe_scenes_vec_size)  )
				return -1;
		wipe_scenes.reserve(__wipe_scenes_vec_size);
		for(uint16_t i = 0; i < __wipe_scenes_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				wipe_scenes.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000409;
		wipe_type = 0;
		wipe_scenes.clear();
	}
};

/*
整型信息改变更新
*/
struct MSG_81000067 : public Base_Msg  {
	int64_t role_id;
	int8_t int_type;
	std::vector<Int32_Info_Change> vec_32_change_info;
	MSG_81000067(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(int_type);
		uint16_t __vec_32_change_info_vec_size = vec_32_change_info.size();
		w.write_uint16(__vec_32_change_info_vec_size);
		for(uint16_t i = 0; i < __vec_32_change_info_vec_size; ++i) {
				vec_32_change_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(int_type)  )
				return -1;
 		uint16_t __vec_32_change_info_vec_size;
		if(r.read_uint16(__vec_32_change_info_vec_size)  )
				return -1;
		vec_32_change_info.reserve(__vec_32_change_info_vec_size);
		for(uint16_t i = 0; i < __vec_32_change_info_vec_size; ++i) {
				Int32_Info_Change v;
				if(v.deserialize(r))
						return -1;
				vec_32_change_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000067;
		role_id = 0;
		int_type = 0;
		vec_32_change_info.clear();
	}
};

/*
副本章节奖励领取通知
备注：副本返回主城后弹
*/
struct MSG_82100009 : public Base_Msg  {
	int8_t chapter_id;
	MSG_82100009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(chapter_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(chapter_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82100009;
		chapter_id = 0;
	}
};

/*
主动通知人物面板符文信息
*/
struct MSG_82210002 : public Base_Msg  {
	std::vector<Rune_Part_Info> part_infos;
	int64_t target_role_id;
	MSG_82210002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __part_infos_vec_size = part_infos.size();
		w.write_uint16(__part_infos_vec_size);
		for(uint16_t i = 0; i < __part_infos_vec_size; ++i) {
				part_infos[i].serialize(w);
		}

		w.write_int64(target_role_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __part_infos_vec_size;
		if(r.read_uint16(__part_infos_vec_size)  )
				return -1;
		part_infos.reserve(__part_infos_vec_size);
		for(uint16_t i = 0; i < __part_infos_vec_size; ++i) {
				Rune_Part_Info v;
				if(v.deserialize(r))
						return -1;
				part_infos.push_back(v);
		}

		if( r.read_int64(target_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82210002;
		part_infos.clear();
		target_role_id = 0;
	}
};

/*
主动推精通数据
*/
struct MSG_82210003 : public Base_Msg  {
	int64_t target_role_id;
	std::vector<Master_Part_Info> part_infos;
	MSG_82210003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(target_role_id);
		uint16_t __part_infos_vec_size = part_infos.size();
		w.write_uint16(__part_infos_vec_size);
		for(uint16_t i = 0; i < __part_infos_vec_size; ++i) {
				part_infos[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(target_role_id)  )
				return -1;
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
		msg_id = 82210003;
		target_role_id = 0;
		part_infos.clear();
	}
};

/*
主动通知龙谷信息(龙谷以外的场景接收)
*/
struct MSG_82300033 : public Base_Msg  {
	uint8_t build_queue_num;
	uint8_t all_open_build_queue_num;
	std::vector<Building_Info> building_info_list;
	uint8_t progress;
	MSG_82300033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(build_queue_num);
		w.write_uint8(all_open_build_queue_num);
		uint16_t __building_info_list_vec_size = building_info_list.size();
		w.write_uint16(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				building_info_list[i].serialize(w);
		}

		w.write_uint8(progress);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(build_queue_num)  ||  r.read_uint8(all_open_build_queue_num)  )
				return -1;
 		uint16_t __building_info_list_vec_size;
		if(r.read_uint16(__building_info_list_vec_size)  )
				return -1;
		building_info_list.reserve(__building_info_list_vec_size);
		for(uint16_t i = 0; i < __building_info_list_vec_size; ++i) {
				Building_Info v;
				if(v.deserialize(r))
						return -1;
				building_info_list.push_back(v);
		}

		if( r.read_uint8(progress)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300033;
		build_queue_num = 0;
		all_open_build_queue_num = 0;
		building_info_list.clear();
		progress = 0;
	}
};

/*
GM广播公告
*/
struct MSG_83000003 : public Base_Msg  {
	std::string contents;
	MSG_83000003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(contents);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(contents)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 83000003;
		contents.clear();
	}
};

/*
副本状态更改通知
*/
struct MSG_81000019 : public Base_Msg  {
	int32_t status;
	int32_t value1;
	int32_t value2;
	int32_t value3;
	MSG_81000019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(status);
		w.write_int32(value1);
		w.write_int32(value2);
		w.write_int32(value3);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(status)  ||  r.read_int32(value1)  ||  r.read_int32(value2)  ||  r.read_int32(value3)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000019;
		status = 0;
		value1 = 0;
		value2 = 0;
		value3 = 0;
	}
};

/*
批量进化龙， 提示是否愿意消耗多少道具、多少货币
*/
struct MSG_82300034 : public Base_Msg  {
	uint16_t batch_num;
	uint16_t ten_times;
	uint8_t type;
	uint32_t item_id;
	uint16_t item_num;
	uint8_t money_type;
	uint32_t money_num;
	MSG_82300034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(batch_num);
		w.write_uint16(ten_times);
		w.write_uint8(type);
		w.write_uint32(item_id);
		w.write_uint16(item_num);
		w.write_uint8(money_type);
		w.write_uint32(money_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(batch_num)  ||  r.read_uint16(ten_times)  ||  r.read_uint8(type)  ||  r.read_uint32(item_id)  ||  r.read_uint16(item_num)  ||  r.read_uint8(money_type)  ||  r.read_uint32(money_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300034;
		batch_num = 0;
		ten_times = 0;
		type = 0;
		item_id = 0;
		item_num = 0;
		money_type = 0;
		money_num = 0;
	}
};

/*
充值返利可领取通知
*/
struct MSG_82200203 : public Base_Msg  {
	uint8_t num;
	MSG_82200203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200203;
		num = 0;
	}
};

/*
通知精灵盟约卡使用
*/
struct MSG_82200223 : public Base_Msg  {
	std::vector<Uint_Uint> info;
	uint8_t exp;
	MSG_82200223(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

		w.write_uint8(exp);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		if( r.read_uint8(exp)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200223;
		info.clear();
		exp = 0;
	}
};

/*
广播组队副本是否为第一次且未开始战斗
*/
struct MSG_80100324 : public Base_Msg  {
	int8_t state;
	MSG_80100324(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100324;
		state = 0;
	}
};

/*
更新被掠夺保护时间
*/
struct MSG_82300035 : public Base_Msg  {
	uint32_t protect_time;
	MSG_82300035(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(protect_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(protect_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300035;
		protect_time = 0;
	}
};

/*
主动通知已播放的剧情列表
*/
struct MSG_82300036 : public Base_Msg  {
	std::vector<uint32_t> plot_id_list;
	MSG_82300036(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __plot_id_list_vec_size = plot_id_list.size();
		w.write_uint16(__plot_id_list_vec_size);
		for(uint16_t i = 0; i < __plot_id_list_vec_size; ++i) {
				w.write_uint32(plot_id_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __plot_id_list_vec_size;
		if(r.read_uint16(__plot_id_list_vec_size)  )
				return -1;
		plot_id_list.reserve(__plot_id_list_vec_size);
		for(uint16_t i = 0; i < __plot_id_list_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				plot_id_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82300036;
		plot_id_list.clear();
	}
};

/*
竞技场个人信息变化通知
*/
struct MSG_81000304 : public Base_Msg  {
	Arena_Personal_Info personal_info;
	std::vector<Uid_Name> the_first_three;
	MSG_81000304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		personal_info.serialize(w);
		uint16_t __the_first_three_vec_size = the_first_three.size();
		w.write_uint16(__the_first_three_vec_size);
		for(uint16_t i = 0; i < __the_first_three_vec_size; ++i) {
				the_first_three[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( personal_info.deserialize(r)  )
				return -1;
 		uint16_t __the_first_three_vec_size;
		if(r.read_uint16(__the_first_three_vec_size)  )
				return -1;
		the_first_three.reserve(__the_first_three_vec_size);
		for(uint16_t i = 0; i < __the_first_three_vec_size; ++i) {
				Uid_Name v;
				if(v.deserialize(r))
						return -1;
				the_first_three.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000304;
		personal_info.reset();
		the_first_three.clear();
	}
};

/*
竞技场前三名变化通知
*/
struct MSG_81000305 : public Base_Msg  {
	std::vector<Uid_Name> the_first_three;
	MSG_81000305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __the_first_three_vec_size = the_first_three.size();
		w.write_uint16(__the_first_three_vec_size);
		for(uint16_t i = 0; i < __the_first_three_vec_size; ++i) {
				the_first_three[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __the_first_three_vec_size;
		if(r.read_uint16(__the_first_three_vec_size)  )
				return -1;
		the_first_three.reserve(__the_first_three_vec_size);
		for(uint16_t i = 0; i < __the_first_three_vec_size; ++i) {
				Uid_Name v;
				if(v.deserialize(r))
						return -1;
				the_first_three.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000305;
		the_first_three.clear();
	}
};

/*
通知客户端更新龙谷操作区
*/
struct MSG_82300037 : public Base_Msg  {
	int64_t target_id;
	uint8_t op;
	MSG_82300037(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(target_id);
		w.write_uint8(op);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(target_id)  ||  r.read_uint8(op)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300037;
		target_id = 0;
		op = 0;
	}
};

/*
上古神器条伯完成
*/
struct MSG_81101100 : public Base_Msg  {
	std::vector<Anci_Artifact_Cond> list;
	MSG_81101100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Anci_Artifact_Cond v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81101100;
		list.clear();
	}
};

/*
主动通知更新升级投资的页面信息
备注：因单个领取按钮，引起的马上投资按钮状态改变。发送这条消息给客户端。
*/
struct MSG_82310000 : public Base_Msg  {
	uint8_t invest_button;
	MSG_82310000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(invest_button);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(invest_button)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82310000;
		invest_button = 0;
	}
};

/*
战场3分钟没有积分变化提示
*/
struct MSG_81000410 : public Base_Msg  {
	MSG_81000410(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 81000410;
	}
};

/*
使用道具获得效果飘字
*/
struct MSG_81000104 : public Base_Msg  {
	std::vector<Property> prop;
	MSG_81000104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __prop_vec_size = prop.size();
		w.write_uint16(__prop_vec_size);
		for(uint16_t i = 0; i < __prop_vec_size; ++i) {
				prop[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __prop_vec_size;
		if(r.read_uint16(__prop_vec_size)  )
				return -1;
		prop.reserve(__prop_vec_size);
		for(uint16_t i = 0; i < __prop_vec_size; ++i) {
				Property v;
				if(v.deserialize(r))
						return -1;
				prop.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000104;
		prop.clear();
	}
};

/*
主动通知升级投资的领取按钮列表信息
备注：当玩家的等级发生变化的时候，会主动发送这条消息，用来更新升级投资的领取按钮列表
*/
struct MSG_82310001 : public Base_Msg  {
	std::vector<Upgrade_Invest_Info> button_list;
	MSG_82310001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __button_list_vec_size = button_list.size();
		w.write_uint16(__button_list_vec_size);
		for(uint16_t i = 0; i < __button_list_vec_size; ++i) {
				button_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __button_list_vec_size;
		if(r.read_uint16(__button_list_vec_size)  )
				return -1;
		button_list.reserve(__button_list_vec_size);
		for(uint16_t i = 0; i < __button_list_vec_size; ++i) {
				Upgrade_Invest_Info v;
				if(v.deserialize(r))
						return -1;
				button_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82310001;
		button_list.clear();
	}
};

/*
主动通知龙谷可建造的建筑数量
备注：玩家登录时，发送这条消息给客户端，用来在下方的图标显示数字
*/
struct MSG_82300038 : public Base_Msg  {
	uint8_t num;
	MSG_82300038(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300038;
		num = 0;
	}
};

/*
主动通知客户端关闭悬赏榜自动刷新倒计时
*/
struct MSG_81110010 : public Base_Msg  {
	MSG_81110010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 81110010;
	}
};

/*
主动通知客户端月卡投资领取收益按钮变为可领取
备注：6点刷新，会发送这条消息
*/
struct MSG_82310002 : public Base_Msg  {
	uint8_t day;
	MSG_82310002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(day);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(day)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82310002;
		day = 0;
	}
};

/*
远征建筑信息列表
换线信息：后面部分不发
*/
struct MSG_81101300 : public Base_Msg  {
	std::vector<Expedition_Gang_Brief_Info> gang_info;
	std::vector<Expedition_Build_Brief_Info> build_info;
	int32_t line_people;
	int32_t line_nums;
	int32_t next_forces;
	MSG_81101300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __gang_info_vec_size = gang_info.size();
		w.write_uint16(__gang_info_vec_size);
		for(uint16_t i = 0; i < __gang_info_vec_size; ++i) {
				gang_info[i].serialize(w);
		}

		uint16_t __build_info_vec_size = build_info.size();
		w.write_uint16(__build_info_vec_size);
		for(uint16_t i = 0; i < __build_info_vec_size; ++i) {
				build_info[i].serialize(w);
		}

		w.write_int32(line_people);
		w.write_int32(line_nums);
		w.write_int32(next_forces);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __gang_info_vec_size;
		if(r.read_uint16(__gang_info_vec_size)  )
				return -1;
		gang_info.reserve(__gang_info_vec_size);
		for(uint16_t i = 0; i < __gang_info_vec_size; ++i) {
				Expedition_Gang_Brief_Info v;
				if(v.deserialize(r))
						return -1;
				gang_info.push_back(v);
		}

		uint16_t __build_info_vec_size;
		if(r.read_uint16(__build_info_vec_size)  )
				return -1;
		build_info.reserve(__build_info_vec_size);
		for(uint16_t i = 0; i < __build_info_vec_size; ++i) {
				Expedition_Build_Brief_Info v;
				if(v.deserialize(r))
						return -1;
				build_info.push_back(v);
		}

		if( r.read_int32(line_people)  ||  r.read_int32(line_nums)  ||  r.read_int32(next_forces)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81101300;
		gang_info.clear();
		build_info.clear();
		line_people = 0;
		line_nums = 0;
		next_forces = 0;
	}
};

/*
主动通知客户端龙谷已取消建造建筑的编号列表
*/
struct MSG_82300039 : public Base_Msg  {
	std::vector<uint8_t> number_list;
	MSG_82300039(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __number_list_vec_size = number_list.size();
		w.write_uint16(__number_list_vec_size);
		for(uint16_t i = 0; i < __number_list_vec_size; ++i) {
				w.write_uint8(number_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __number_list_vec_size;
		if(r.read_uint16(__number_list_vec_size)  )
				return -1;
		number_list.reserve(__number_list_vec_size);
		for(uint16_t i = 0; i < __number_list_vec_size; ++i) {
				uint8_t v;
				if(r.read_uint8(v) )
						return -1;
				number_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 82300039;
		number_list.clear();
	}
};

/*
远征相应通知
type:
1防守通知
2已被攻陷
*/
struct MSG_81101301 : public Base_Msg  {
	int8_t type;
	int32_t point_id;
	int32_t gang_name;
	std::vector<Item_Info> award_list;
	MSG_81101301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(point_id);
		w.write_int32(gang_name);
		uint16_t __award_list_vec_size = award_list.size();
		w.write_uint16(__award_list_vec_size);
		for(uint16_t i = 0; i < __award_list_vec_size; ++i) {
				award_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(point_id)  ||  r.read_int32(gang_name)  )
				return -1;
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

		return 0;
	}

	void reset(void) {
		msg_id = 81101301;
		type = 0;
		point_id = 0;
		gang_name = 0;
		award_list.clear();
	}
};

/*
远征倒计时通知
type:
1：复活倒计时
2：宝箱数
3：镜像在战斗
4：远征状态 state:0第一季未开启1之后季未开启 2已开启
*/
struct MSG_81101302 : public Base_Msg  {
	int8_t type;
	int32_t time;
	int8_t state;
	int32_t award_time;
	MSG_81101302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(time);
		w.write_int8(state);
		w.write_int32(award_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(time)  ||  r.read_int8(state)  ||  r.read_int32(award_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81101302;
		type = 0;
		time = 0;
		state = 0;
		award_time = 0;
	}
};

/*
主动通知客户端投资历史更新
备注：(仅实时更新自己的)
*/
struct MSG_82310003 : public Base_Msg  {
	INVEST_PLAN_HISTORY_RECORD history_record;
	MSG_82310003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		history_record.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( history_record.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82310003;
		history_record.reset();
	}
};

/*
活动倒计时和开启活动预告通知
*/
struct MSG_82200211 : public Base_Msg  {
	uint32_t id;
	uint32_t start_time;
	uint32_t type;
	MSG_82200211(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(start_time);
		w.write_uint32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(start_time)  ||  r.read_uint32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200211;
		id = 0;
		start_time = 0;
		type = 0;
	}
};

/*
广播组队累计奖励
*/
struct MSG_80100325 : public Base_Msg  {
	int8_t type;
	std::vector<Reward_Info> award;
	int32_t kill_nums;
	MSG_80100325(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		uint16_t __award_vec_size = award.size();
		w.write_uint16(__award_vec_size);
		for(uint16_t i = 0; i < __award_vec_size; ++i) {
				award[i].serialize(w);
		}

		w.write_int32(kill_nums);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
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

		if( r.read_int32(kill_nums)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100325;
		type = 0;
		award.clear();
		kill_nums = 0;
	}
};

/*
通知客户端更新下一次匹配消耗数量
*/
struct MSG_82300040 : public Base_Msg  {
	uint32_t coin;
	MSG_82300040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(coin);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(coin)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300040;
		coin = 0;
	}
};

/*
通知客户端显示/关闭提交道具链接
*/
struct MSG_81110011 : public Base_Msg  {
	uint32_t task_id;
	uint8_t status;
	MSG_81110011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110011;
		task_id = 0;
		status = 0;
	}
};

/*
通知是否可以逗龙
*/
struct MSG_82300041 : public Base_Msg  {
	uint8_t is_play_dragon;
	MSG_82300041(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_play_dragon);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_play_dragon)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300041;
		is_play_dragon = 0;
	}
};

/*
主动通知客户端所有可领取的按钮数量
备注：total_num: 包括升级投资、月卡投资的所有可领取按钮的总数量
*/
struct MSG_82310004 : public Base_Msg  {
	uint32_t total_num;
	MSG_82310004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(total_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(total_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82310004;
		total_num = 0;
	}
};

/*
主动推送引导步骤列表
*/
struct MSG_80100501 : public Base_Msg  {
	std::vector<Guide_Info> guide_info_list;
	MSG_80100501(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __guide_info_list_vec_size = guide_info_list.size();
		w.write_uint16(__guide_info_list_vec_size);
		for(uint16_t i = 0; i < __guide_info_list_vec_size; ++i) {
				guide_info_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __guide_info_list_vec_size;
		if(r.read_uint16(__guide_info_list_vec_size)  )
				return -1;
		guide_info_list.reserve(__guide_info_list_vec_size);
		for(uint16_t i = 0; i < __guide_info_list_vec_size; ++i) {
				Guide_Info v;
				if(v.deserialize(r))
						return -1;
				guide_info_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100501;
		guide_info_list.clear();
	}
};

/*
主动通知更新龙信息面板的逗龙冷却时间
*/
struct MSG_82300042 : public Base_Msg  {
	uint32_t left_time;
	MSG_82300042(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(left_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(left_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300042;
		left_time = 0;
	}
};

/*
你的好友邀请去踩脚印
*/
struct MSG_82300043 : public Base_Msg  {
	int64_t role_id;
	std::string role_name;
	MSG_82300043(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82300043;
		role_id = 0;
		role_name.clear();
	}
};

/*
别人来进攻掠夺龙谷，需要转龙谷场景
*/
struct MSG_82310005 : public Base_Msg  {
	MSG_82310005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 82310005;
	}
};

/*
广播世界等级
*/
struct MSG_80100600 : public Base_Msg  {
	int32_t level;
	MSG_80100600(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100600;
		level = 0;
	}
};

/*
主动通知悬赏任务剩余领取次数
*/
struct MSG_81110012 : public Base_Msg  {
	uint32_t num;
	MSG_81110012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110012;
		num = 0;
	}
};

/*
装备穿上错误提示
*/
struct MSG_81001002 : public Base_Msg  {
	int32_t right_id;
	int32_t wrong_id;
	int32_t item_id;
	MSG_81001002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(right_id);
		w.write_int32(wrong_id);
		w.write_int32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(right_id)  ||  r.read_int32(wrong_id)  ||  r.read_int32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81001002;
		right_id = 0;
		wrong_id = 0;
		item_id = 0;
	}
};

/*
通知骑士试练剩余次数
*/
struct MSG_82200250 : public Base_Msg  {
	uint8_t num;
	uint8_t rnum;
	MSG_82200250(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(num);
		w.write_uint8(rnum);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(num)  ||  r.read_uint8(rnum)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200250;
		num = 0;
		rnum = 0;
	}
};

/*
主动通知当前预告
*/
struct MSG_81110013 : public Base_Msg  {
	uint32_t id;
	uint8_t is_play;
	uint8_t type;
	MSG_81110013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(is_play);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(is_play)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110013;
		id = 0;
		is_play = 0;
		type = 0;
	}
};

/*
通知智慧试练题目更新
*/
struct MSG_82330000 : public Base_Msg  {
	uint32_t id;
	uint32_t qno;
	uint16_t status;
	std::string a;
	std::string b;
	uint32_t true_answer;
	uint32_t time;
	MSG_82330000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(qno);
		w.write_uint16(status);
		w.write_string(a);
		w.write_string(b);
		w.write_uint32(true_answer);
		w.write_uint32(time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(qno)  ||  r.read_uint16(status)  ||  r.read_string(a)  ||  r.read_string(b)  ||  r.read_uint32(true_answer)  ||  r.read_uint32(time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82330000;
		id = 0;
		qno = 0;
		status = 0;
		a.clear();
		b.clear();
		true_answer = 0;
		time = 0;
	}
};

/*
通知刷新智慧试练排行榜信息
*/
struct MSG_82330001 : public Base_Msg  {
	std::vector<Answer_Rank_Role_Info> rank;
	uint32_t true_num;
	uint32_t sorce;
	uint32_t exp;
	uint32_t start_time;
	uint32_t free_num;
	uint32_t double_num;
	MSG_82330001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __rank_vec_size = rank.size();
		w.write_uint16(__rank_vec_size);
		for(uint16_t i = 0; i < __rank_vec_size; ++i) {
				rank[i].serialize(w);
		}

		w.write_uint32(true_num);
		w.write_uint32(sorce);
		w.write_uint32(exp);
		w.write_uint32(start_time);
		w.write_uint32(free_num);
		w.write_uint32(double_num);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __rank_vec_size;
		if(r.read_uint16(__rank_vec_size)  )
				return -1;
		rank.reserve(__rank_vec_size);
		for(uint16_t i = 0; i < __rank_vec_size; ++i) {
				Answer_Rank_Role_Info v;
				if(v.deserialize(r))
						return -1;
				rank.push_back(v);
		}

		if( r.read_uint32(true_num)  ||  r.read_uint32(sorce)  ||  r.read_uint32(exp)  ||  r.read_uint32(start_time)  ||  r.read_uint32(free_num)  ||  r.read_uint32(double_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82330001;
		rank.clear();
		true_num = 0;
		sorce = 0;
		exp = 0;
		start_time = 0;
		free_num = 0;
		double_num = 0;
	}
};

/*
通知智慧试练退出倒计时通知
*/
struct MSG_82330002 : public Base_Msg  {
	uint32_t time;
	MSG_82330002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82330002;
		time = 0;
	}
};

/*
开启传送点
*/
struct MSG_81110014 : public Base_Msg  {
	uint32_t scene_id;
	MSG_81110014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110014;
		scene_id = 0;
	}
};

/*
通知全部传送点
*/
struct MSG_81110015 : public Base_Msg  {
	std::vector<uint32_t> id_list;
	MSG_81110015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __id_list_vec_size = id_list.size();
		w.write_uint16(__id_list_vec_size);
		for(uint16_t i = 0; i < __id_list_vec_size; ++i) {
				w.write_uint32(id_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __id_list_vec_size;
		if(r.read_uint16(__id_list_vec_size)  )
				return -1;
		id_list.reserve(__id_list_vec_size);
		for(uint16_t i = 0; i < __id_list_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				id_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81110015;
		id_list.clear();
	}
};

/*
上古副本获得追踪信息
*/
struct MSG_81000090 : public Base_Msg  {
	int32_t coin_num;
	std::vector<Item_Info> items;
	int8_t type;
	int8_t first_pass;
	std::vector<Item_Info> first_pass_items;
	MSG_81000090(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(coin_num);
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

		w.write_int8(type);
		w.write_int8(first_pass);
		uint16_t __first_pass_items_vec_size = first_pass_items.size();
		w.write_uint16(__first_pass_items_vec_size);
		for(uint16_t i = 0; i < __first_pass_items_vec_size; ++i) {
				first_pass_items[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(coin_num)  )
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

		if( r.read_int8(type)  ||  r.read_int8(first_pass)  )
				return -1;
 		uint16_t __first_pass_items_vec_size;
		if(r.read_uint16(__first_pass_items_vec_size)  )
				return -1;
		first_pass_items.reserve(__first_pass_items_vec_size);
		for(uint16_t i = 0; i < __first_pass_items_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				first_pass_items.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000090;
		coin_num = 0;
		items.clear();
		type = 0;
		first_pass = 0;
		first_pass_items.clear();
	}
};

/*
广播boss信息
*/
struct MSG_80100326 : public Base_Msg  {
	std::vector<Int_String> info;
	std::vector<Reward_Info> reward;
	MSG_80100326(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

		uint16_t __reward_vec_size = reward.size();
		w.write_uint16(__reward_vec_size);
		for(uint16_t i = 0; i < __reward_vec_size; ++i) {
				reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Int_String v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		uint16_t __reward_vec_size;
		if(r.read_uint16(__reward_vec_size)  )
				return -1;
		reward.reserve(__reward_vec_size);
		for(uint16_t i = 0; i < __reward_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100326;
		info.clear();
		reward.clear();
	}
};

/*
守卫帝都剩余时间
*/
struct MSG_80100327 : public Base_Msg  {
	int32_t time;
	int32_t add_time;
	MSG_80100327(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(time);
		w.write_int32(add_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(time)  ||  r.read_int32(add_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100327;
		time = 0;
		add_time = 0;
	}
};

/*
怪物出现倒计时
*/
struct MSG_80100328 : public Base_Msg  {
	int16_t the_time;
	std::string name;
	MSG_80100328(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(the_time);
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(the_time)  ||  r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100328;
		the_time = 0;
		name.clear();
	}
};

/*
战斗怪物ai预告
*/
struct MSG_81000056 : public Base_Msg  {
	std::vector<AI_Foreshow_Info> ai_foreshow_vec;
	MSG_81000056(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __ai_foreshow_vec_vec_size = ai_foreshow_vec.size();
		w.write_uint16(__ai_foreshow_vec_vec_size);
		for(uint16_t i = 0; i < __ai_foreshow_vec_vec_size; ++i) {
				ai_foreshow_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __ai_foreshow_vec_vec_size;
		if(r.read_uint16(__ai_foreshow_vec_vec_size)  )
				return -1;
		ai_foreshow_vec.reserve(__ai_foreshow_vec_vec_size);
		for(uint16_t i = 0; i < __ai_foreshow_vec_vec_size; ++i) {
				AI_Foreshow_Info v;
				if(v.deserialize(r))
						return -1;
				ai_foreshow_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000056;
		ai_foreshow_vec.clear();
	}
};

/*
多人副本通关结算
reward_list
经验：502503
  1：通关经验
  2：vip奖励
  3：道具加成
  4：好友奖励（组队）/首通经验(单人)
金币：501502
英灵：502903
*/
struct MSG_81000500 : public Base_Msg  {
	int32_t scene_id;
	int32_t type;
	std::vector<Reward_Info> reward_list;
	std::vector<Int_String> hurt_vec;
	std::vector<Int_String> shield_vec;
	std::vector<Int_String> cure_vec;
	MSG_81000500(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
		w.write_int32(type);
		uint16_t __reward_list_vec_size = reward_list.size();
		w.write_uint16(__reward_list_vec_size);
		for(uint16_t i = 0; i < __reward_list_vec_size; ++i) {
				reward_list[i].serialize(w);
		}

		uint16_t __hurt_vec_vec_size = hurt_vec.size();
		w.write_uint16(__hurt_vec_vec_size);
		for(uint16_t i = 0; i < __hurt_vec_vec_size; ++i) {
				hurt_vec[i].serialize(w);
		}

		uint16_t __shield_vec_vec_size = shield_vec.size();
		w.write_uint16(__shield_vec_vec_size);
		for(uint16_t i = 0; i < __shield_vec_vec_size; ++i) {
				shield_vec[i].serialize(w);
		}

		uint16_t __cure_vec_vec_size = cure_vec.size();
		w.write_uint16(__cure_vec_vec_size);
		for(uint16_t i = 0; i < __cure_vec_vec_size; ++i) {
				cure_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  ||  r.read_int32(type)  )
				return -1;
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

		uint16_t __hurt_vec_vec_size;
		if(r.read_uint16(__hurt_vec_vec_size)  )
				return -1;
		hurt_vec.reserve(__hurt_vec_vec_size);
		for(uint16_t i = 0; i < __hurt_vec_vec_size; ++i) {
				Int_String v;
				if(v.deserialize(r))
						return -1;
				hurt_vec.push_back(v);
		}

		uint16_t __shield_vec_vec_size;
		if(r.read_uint16(__shield_vec_vec_size)  )
				return -1;
		shield_vec.reserve(__shield_vec_vec_size);
		for(uint16_t i = 0; i < __shield_vec_vec_size; ++i) {
				Int_String v;
				if(v.deserialize(r))
						return -1;
				shield_vec.push_back(v);
		}

		uint16_t __cure_vec_vec_size;
		if(r.read_uint16(__cure_vec_vec_size)  )
				return -1;
		cure_vec.reserve(__cure_vec_vec_size);
		for(uint16_t i = 0; i < __cure_vec_vec_size; ++i) {
				Int_String v;
				if(v.deserialize(r))
						return -1;
				cure_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000500;
		scene_id = 0;
		type = 0;
		reward_list.clear();
		hurt_vec.clear();
		shield_vec.clear();
		cure_vec.clear();
	}
};

/*
服务器即将关闭提示
*/
struct MSG_80000006 : public Base_Msg  {
	int32_t delay;
	MSG_80000006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(delay);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(delay)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80000006;
		delay = 0;
	}
};

/*
要显示的场景npc
*/
struct MSG_81110016 : public Base_Msg  {
	std::vector<Scene_Npc> scene_npc_list;
	MSG_81110016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __scene_npc_list_vec_size = scene_npc_list.size();
		w.write_uint16(__scene_npc_list_vec_size);
		for(uint16_t i = 0; i < __scene_npc_list_vec_size; ++i) {
				scene_npc_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __scene_npc_list_vec_size;
		if(r.read_uint16(__scene_npc_list_vec_size)  )
				return -1;
		scene_npc_list.reserve(__scene_npc_list_vec_size);
		for(uint16_t i = 0; i < __scene_npc_list_vec_size; ++i) {
				Scene_Npc v;
				if(v.deserialize(r))
						return -1;
				scene_npc_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81110016;
		scene_npc_list.clear();
	}
};

/*
添加/删除场景npc
*/
struct MSG_81110017 : public Base_Msg  {
	uint8_t type;
	uint32_t scene_id;
	uint32_t npc_id;
	MSG_81110017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint32(scene_id);
		w.write_uint32(npc_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint32(scene_id)  ||  r.read_uint32(npc_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110017;
		type = 0;
		scene_id = 0;
		npc_id = 0;
	}
};

/*
广播失败——守卫帝都
*/
struct MSG_80100329 : public Base_Msg  {
	MSG_80100329(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 80100329;
	}
};

/*
打开天赋技能面板
*/
struct MSG_81110018 : public Base_Msg  {
	uint32_t talent_id;
	uint8_t type;
	MSG_81110018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(talent_id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(talent_id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110018;
		talent_id = 0;
		type = 0;
	}
};

/*
主动通知体验过期
*/
struct MSG_81110019 : public Base_Msg  {
	uint8_t type;
	MSG_81110019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110019;
		type = 0;
	}
};

/*
主动通知体验过期列表
*/
struct MSG_81110020 : public Base_Msg  {
	std::vector<uint8_t> type_list;
	MSG_81110020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __type_list_vec_size = type_list.size();
		w.write_uint16(__type_list_vec_size);
		for(uint16_t i = 0; i < __type_list_vec_size; ++i) {
				w.write_uint8(type_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __type_list_vec_size;
		if(r.read_uint16(__type_list_vec_size)  )
				return -1;
		type_list.reserve(__type_list_vec_size);
		for(uint16_t i = 0; i < __type_list_vec_size; ++i) {
				uint8_t v;
				if(r.read_uint8(v) )
						return -1;
				type_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81110020;
		type_list.clear();
	}
};

/*
副本首次通关通知
*/
struct MSG_81000029 : public Base_Msg  {
	int32_t scene_id;
	MSG_81000029(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000029;
		scene_id = 0;
	}
};

/*
远征建筑状态变化
*/
struct MSG_81101303 : public Base_Msg  {
	std::vector<Expedition_Build_Brief_Info> build_list;
	MSG_81101303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __build_list_vec_size = build_list.size();
		w.write_uint16(__build_list_vec_size);
		for(uint16_t i = 0; i < __build_list_vec_size; ++i) {
				build_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __build_list_vec_size;
		if(r.read_uint16(__build_list_vec_size)  )
				return -1;
		build_list.reserve(__build_list_vec_size);
		for(uint16_t i = 0; i < __build_list_vec_size; ++i) {
				Expedition_Build_Brief_Info v;
				if(v.deserialize(r))
						return -1;
				build_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81101303;
		build_list.clear();
	}
};

/*
通知最新的今日获得粉钻数
*/
struct MSG_82200204 : public Base_Msg  {
	uint32_t num;
	MSG_82200204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 82200204;
		num = 0;
	}
};

/*
主动通知任务引导
*/
struct MSG_81110021 : public Base_Msg  {
	uint32_t guide_id;
	MSG_81110021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(guide_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(guide_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81110021;
		guide_id = 0;
	}
};

/*
世界boss信息更新
*/
struct MSG_81000600 : public Base_Msg  {
	int64_t act_guild_bless_role_id;
	std::string act_guild_bless_role_name;
	int8_t act_guild_bless_role_pos;
	int8_t ability_0_num;
	int8_t ability_1_num;
	int8_t ability_2_num;
	int8_t ability_3_num;
	int8_t act_time_type;
	int32_t act_time;
	int32_t boss_cur_hp;
	int32_t boss_max_hp;
	int32_t boss_type_id;
	std::string boss_name;
	int32_t boss_bonus_id;
	std::string boss_bonus_role_name;
	int32_t boss_kill_num;
	int32_t revive_time;
	int8_t icon_id;
	int8_t boss_buffer_type;
	int32_t boss_lv;
	MSG_81000600(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(act_guild_bless_role_id);
		w.write_string(act_guild_bless_role_name);
		w.write_int8(act_guild_bless_role_pos);
		w.write_int8(ability_0_num);
		w.write_int8(ability_1_num);
		w.write_int8(ability_2_num);
		w.write_int8(ability_3_num);
		w.write_int8(act_time_type);
		w.write_int32(act_time);
		w.write_int32(boss_cur_hp);
		w.write_int32(boss_max_hp);
		w.write_int32(boss_type_id);
		w.write_string(boss_name);
		w.write_int32(boss_bonus_id);
		w.write_string(boss_bonus_role_name);
		w.write_int32(boss_kill_num);
		w.write_int32(revive_time);
		w.write_int8(icon_id);
		w.write_int8(boss_buffer_type);
		w.write_int32(boss_lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(act_guild_bless_role_id)  ||  r.read_string(act_guild_bless_role_name)  ||  r.read_int8(act_guild_bless_role_pos)  ||  r.read_int8(ability_0_num)  ||  r.read_int8(ability_1_num)  ||  r.read_int8(ability_2_num)  ||  r.read_int8(ability_3_num)  ||  r.read_int8(act_time_type)  ||  r.read_int32(act_time)  ||  r.read_int32(boss_cur_hp)  ||  r.read_int32(boss_max_hp)  ||  r.read_int32(boss_type_id)  ||  r.read_string(boss_name)  ||  r.read_int32(boss_bonus_id)  ||  r.read_string(boss_bonus_role_name)  ||  r.read_int32(boss_kill_num)  ||  r.read_int32(revive_time)  ||  r.read_int8(icon_id)  ||  r.read_int8(boss_buffer_type)  ||  r.read_int32(boss_lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000600;
		act_guild_bless_role_id = 0;
		act_guild_bless_role_name.clear();
		act_guild_bless_role_pos = 0;
		ability_0_num = 0;
		ability_1_num = 0;
		ability_2_num = 0;
		ability_3_num = 0;
		act_time_type = 0;
		act_time = 0;
		boss_cur_hp = 0;
		boss_max_hp = 0;
		boss_type_id = 0;
		boss_name.clear();
		boss_bonus_id = 0;
		boss_bonus_role_name.clear();
		boss_kill_num = 0;
		revive_time = 0;
		icon_id = 0;
		boss_buffer_type = 0;
		boss_lv = 0;
	}
};

/*
世界boss公会激活通知
*/
struct MSG_81000601 : public Base_Msg  {
	int64_t role_id;
	std::string role_name;
	int8_t role_pos;
	int8_t bless_num;
	int8_t icon_id;
	MSG_81000601(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int8(role_pos);
		w.write_int8(bless_num);
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int8(role_pos)  ||  r.read_int8(bless_num)  ||  r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000601;
		role_id = 0;
		role_name.clear();
		role_pos = 0;
		bless_num = 0;
		icon_id = 0;
	}
};

/*
世界boss奖励通知
*/
struct MSG_81000602 : public Base_Msg  {
	int8_t result;
	int32_t rank;
	int32_t boss_kill_num;
	int32_t damage;
	double damage_rate;
	int8_t achieve_1;
	int8_t achieve_2;
	int8_t achieve_3;
	int8_t achieve_4;
	int8_t achieve_5;
	std::vector<Item_Info> items;
	std::vector<Item_Info> rank_items;
	int8_t icon_id;
	MSG_81000602(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(rank);
		w.write_int32(boss_kill_num);
		w.write_int32(damage);
		w.write_double(damage_rate);
		w.write_int8(achieve_1);
		w.write_int8(achieve_2);
		w.write_int8(achieve_3);
		w.write_int8(achieve_4);
		w.write_int8(achieve_5);
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

		uint16_t __rank_items_vec_size = rank_items.size();
		w.write_uint16(__rank_items_vec_size);
		for(uint16_t i = 0; i < __rank_items_vec_size; ++i) {
				rank_items[i].serialize(w);
		}

		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(rank)  ||  r.read_int32(boss_kill_num)  ||  r.read_int32(damage)  ||  r.read_double(damage_rate)  ||  r.read_int8(achieve_1)  ||  r.read_int8(achieve_2)  ||  r.read_int8(achieve_3)  ||  r.read_int8(achieve_4)  ||  r.read_int8(achieve_5)  )
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

		uint16_t __rank_items_vec_size;
		if(r.read_uint16(__rank_items_vec_size)  )
				return -1;
		rank_items.reserve(__rank_items_vec_size);
		for(uint16_t i = 0; i < __rank_items_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				rank_items.push_back(v);
		}

		if( r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000602;
		result = 0;
		rank = 0;
		boss_kill_num = 0;
		damage = 0;
		damage_rate = 0.0;
		achieve_1 = 0;
		achieve_2 = 0;
		achieve_3 = 0;
		achieve_4 = 0;
		achieve_5 = 0;
		items.clear();
		rank_items.clear();
		icon_id = 0;
	}
};

/*
世界boss血量更新通知
*/
struct MSG_81000603 : public Base_Msg  {
	int32_t cur_hp;
	int32_t max_hp;
	int32_t bonus_id;
	std::string bonus_role_name;
	int32_t damage;
	int8_t icon_id;
	int8_t buffer_type;
	MSG_81000603(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(cur_hp);
		w.write_int32(max_hp);
		w.write_int32(bonus_id);
		w.write_string(bonus_role_name);
		w.write_int32(damage);
		w.write_int8(icon_id);
		w.write_int8(buffer_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(cur_hp)  ||  r.read_int32(max_hp)  ||  r.read_int32(bonus_id)  ||  r.read_string(bonus_role_name)  ||  r.read_int32(damage)  ||  r.read_int8(icon_id)  ||  r.read_int8(buffer_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000603;
		cur_hp = 0;
		max_hp = 0;
		bonus_id = 0;
		bonus_role_name.clear();
		damage = 0;
		icon_id = 0;
		buffer_type = 0;
	}
};

/*
世界boss伤害排名更新通知
*/
struct MSG_81000604 : public Base_Msg  {
	int32_t damage;
	double damage_rate;
	int32_t rank;
	std::vector<World_Boss_Rank_Info> rank_info;
	int8_t icon_id;
	int32_t career_rank;
	std::vector<World_Boss_Rank_Info> career_rank_info;
	MSG_81000604(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(damage);
		w.write_double(damage_rate);
		w.write_int32(rank);
		uint16_t __rank_info_vec_size = rank_info.size();
		w.write_uint16(__rank_info_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_size; ++i) {
				rank_info[i].serialize(w);
		}

		w.write_int8(icon_id);
		w.write_int32(career_rank);
		uint16_t __career_rank_info_vec_size = career_rank_info.size();
		w.write_uint16(__career_rank_info_vec_size);
		for(uint16_t i = 0; i < __career_rank_info_vec_size; ++i) {
				career_rank_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(damage)  ||  r.read_double(damage_rate)  ||  r.read_int32(rank)  )
				return -1;
 		uint16_t __rank_info_vec_size;
		if(r.read_uint16(__rank_info_vec_size)  )
				return -1;
		rank_info.reserve(__rank_info_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_size; ++i) {
				World_Boss_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				rank_info.push_back(v);
		}

		if( r.read_int8(icon_id)  ||  r.read_int32(career_rank)  )
				return -1;
 		uint16_t __career_rank_info_vec_size;
		if(r.read_uint16(__career_rank_info_vec_size)  )
				return -1;
		career_rank_info.reserve(__career_rank_info_vec_size);
		for(uint16_t i = 0; i < __career_rank_info_vec_size; ++i) {
				World_Boss_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				career_rank_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 81000604;
		damage = 0;
		damage_rate = 0.0;
		rank = 0;
		rank_info.clear();
		icon_id = 0;
		career_rank = 0;
		career_rank_info.clear();
	}
};

/*
世界boss活动时间更新
*/
struct MSG_81000605 : public Base_Msg  {
	int8_t act_time_type;
	int32_t act_time;
	int8_t icon_id;
	MSG_81000605(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(act_time_type);
		w.write_int32(act_time);
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(act_time_type)  ||  r.read_int32(act_time)  ||  r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000605;
		act_time_type = 0;
		act_time = 0;
		icon_id = 0;
	}
};

/*
远征切线建筑信息列表
*/
struct MSG_81101304 : public Base_Msg  {
	std::vector<Expedition_Gang_Brief_Info> gang_info;
	std::vector<Expedition_Build_Brief_Info> build_info;
	int16_t cur_line;
	MSG_81101304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __gang_info_vec_size = gang_info.size();
		w.write_uint16(__gang_info_vec_size);
		for(uint16_t i = 0; i < __gang_info_vec_size; ++i) {
				gang_info[i].serialize(w);
		}

		uint16_t __build_info_vec_size = build_info.size();
		w.write_uint16(__build_info_vec_size);
		for(uint16_t i = 0; i < __build_info_vec_size; ++i) {
				build_info[i].serialize(w);
		}

		w.write_int16(cur_line);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __gang_info_vec_size;
		if(r.read_uint16(__gang_info_vec_size)  )
				return -1;
		gang_info.reserve(__gang_info_vec_size);
		for(uint16_t i = 0; i < __gang_info_vec_size; ++i) {
				Expedition_Gang_Brief_Info v;
				if(v.deserialize(r))
						return -1;
				gang_info.push_back(v);
		}

		uint16_t __build_info_vec_size;
		if(r.read_uint16(__build_info_vec_size)  )
				return -1;
		build_info.reserve(__build_info_vec_size);
		for(uint16_t i = 0; i < __build_info_vec_size; ++i) {
				Expedition_Build_Brief_Info v;
				if(v.deserialize(r))
						return -1;
				build_info.push_back(v);
		}

		if( r.read_int16(cur_line)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81101304;
		gang_info.clear();
		build_info.clear();
		cur_line = 0;
	}
};

/*
上古副本弹购买额外挑战次数框
*/
struct MSG_81000800 : public Base_Msg  {
	int32_t scene_id;
	MSG_81000800(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000800;
		scene_id = 0;
	}
};

/*
广播组队竞技战斗结果
*/
struct MSG_80100331 : public Base_Msg  {
	int8_t level;
	int32_t score;
	int32_t score_max;
	int32_t score_get;
	std::vector<Item_Basic_Info> item;
	int8_t result;
	MSG_80100331(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(level);
		w.write_int32(score);
		w.write_int32(score_max);
		w.write_int32(score_get);
		uint16_t __item_vec_size = item.size();
		w.write_uint16(__item_vec_size);
		for(uint16_t i = 0; i < __item_vec_size; ++i) {
				item[i].serialize(w);
		}

		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(level)  ||  r.read_int32(score)  ||  r.read_int32(score_max)  ||  r.read_int32(score_get)  )
				return -1;
 		uint16_t __item_vec_size;
		if(r.read_uint16(__item_vec_size)  )
				return -1;
		item.reserve(__item_vec_size);
		for(uint16_t i = 0; i < __item_vec_size; ++i) {
				Item_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				item.push_back(v);
		}

		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100331;
		level = 0;
		score = 0;
		score_max = 0;
		score_get = 0;
		item.clear();
		result = 0;
	}
};

/*
通知组队积分奖励
*/
struct MSG_80100332 : public Base_Msg  {
	std::vector<Int_Int> award_info;
	MSG_80100332(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __award_info_vec_size = award_info.size();
		w.write_uint16(__award_info_vec_size);
		for(uint16_t i = 0; i < __award_info_vec_size; ++i) {
				award_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __award_info_vec_size;
		if(r.read_uint16(__award_info_vec_size)  )
				return -1;
		award_info.reserve(__award_info_vec_size);
		for(uint16_t i = 0; i < __award_info_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				award_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 80100332;
		award_info.clear();
	}
};

/*
发送宝石合成开启状态
*/
struct MSG_80100316 : public Base_Msg  {
	int32_t form;
	MSG_80100316(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(form);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(form)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 80100316;
		form = 0;
	}
};

/*
情人节杀怪活动结果通知
*/
struct MSG_81000610 : public Base_Msg  {
	int8_t result;
	MSG_81000610(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000610;
		result = 0;
	}
};

/*
主动更新精华
*/
struct MSG_81090000 : public Base_Msg  {
	int32_t essence;
	MSG_81090000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(essence);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(essence)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81090000;
		essence = 0;
	}
};

/*
请求篝火结点面板
*/
struct MSG_81000701 : public Base_Msg  {
	int8_t rank;
	int8_t rank_award;
	int8_t campfire_award_type;
	int32_t campfire_award;
	MSG_81000701(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(rank);
		w.write_int8(rank_award);
		w.write_int8(campfire_award_type);
		w.write_int32(campfire_award);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(rank)  ||  r.read_int8(rank_award)  ||  r.read_int8(campfire_award_type)  ||  r.read_int32(campfire_award)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000701;
		rank = 0;
		rank_award = 0;
		campfire_award_type = 0;
		campfire_award = 0;
	}
};

/*
篝火个人信息更新
*/
struct MSG_81000702 : public Base_Msg  {
	std::vector<Campfire_Collect_Info> f_coord;
	int8_t if_receive;
	int8_t receive;
	int8_t rank;
	MSG_81000702(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __f_coord_vec_size = f_coord.size();
		w.write_uint16(__f_coord_vec_size);
		for(uint16_t i = 0; i < __f_coord_vec_size; ++i) {
				f_coord[i].serialize(w);
		}

		w.write_int8(if_receive);
		w.write_int8(receive);
		w.write_int8(rank);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __f_coord_vec_size;
		if(r.read_uint16(__f_coord_vec_size)  )
				return -1;
		f_coord.reserve(__f_coord_vec_size);
		for(uint16_t i = 0; i < __f_coord_vec_size; ++i) {
				Campfire_Collect_Info v;
				if(v.deserialize(r))
						return -1;
				f_coord.push_back(v);
		}

		if( r.read_int8(if_receive)  ||  r.read_int8(receive)  ||  r.read_int8(rank)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000702;
		f_coord.clear();
		if_receive = 0;
		receive = 0;
		rank = 0;
	}
};

/*
竞技场战斗结束后更新竞技大界面第一名玩家信息
*/
struct MSG_81000306 : public Base_Msg  {
	Arena_Rank_Info first_info;
	std::vector<int32_t> avatar_vec;
	int8_t career;
	int8_t gender;
	int32_t be_worship;
	MSG_81000306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		first_info.serialize(w);
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
		if( first_info.deserialize(r)  )
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
		msg_id = 81000306;
		first_info.reset();
		avatar_vec.clear();
		career = 0;
		gender = 0;
		be_worship = 0;
	}
};

/*
对象状态改变通知
*/
struct MSG_81000013 : public Base_Msg  {
	int64_t role_id;
	uint8_t state;
	MSG_81000013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81000013;
		role_id = 0;
		state = 0;
	}
};

/*
远征刷boss
*/
struct MSG_81101305 : public Base_Msg  {
	int32_t monster_id;
	Coord coord;
	MSG_81101305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(monster_id);
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(monster_id)  ||  coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 81101305;
		monster_id = 0;
		coord.reset();
	}
};



#endif