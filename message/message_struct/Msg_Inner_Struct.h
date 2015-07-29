/*
* Generate by devtool
*/

#ifndef _MSG_INNER_H_
#define _MSG_INNER_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
逻辑服同步场景Detail到场景服
*/
struct MSG_20100010 : public Base_Msg  {
	MSG_20100010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100010;
	}
};

/*
地图同步Mover到逻辑服
*/
struct MSG_20200010 : public Base_Msg  {
	Location location;
	MSG_20200010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		location.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( location.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200010;
		location.reset();
	}
};

/*
同进程改变场景
*/
struct MSG_20200050 : public Base_Msg  {
	MSG_20200050(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200050;
	}
};

/*
逻辑通知场景退出
*/
struct MSG_20200055 : public Base_Msg  {
	MSG_20200055(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200055;
	}
};

/*
跨进程改变场景
*/
struct MSG_20200060 : public Base_Msg  {
	MSG_20200060(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200060;
	}
};

/*
同步Monitor_Link
*/
struct MSG_20100015 : public Base_Msg  {
	MSG_20100015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100015;
	}
};

/*
逻辑通知场景登录
*/
struct MSG_20200056 : public Base_Msg  {
	MSG_20200056(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200056;
	}
};

/*
地图同步Fighter到逻辑服
*/
struct MSG_20200013 : public Base_Msg  {
	MSG_20200013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200013;
	}
};

/*
逻辑服战斗属性改变
*/
struct MSG_20200100 : public Base_Msg  {
	MSG_20200100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200100;
	}
};

/*
战斗结束通知逻辑服
*/
struct MSG_20200200 : public Base_Msg  {
	std::vector<Int_Int> monster;
	std::vector<int64_t> kill_player;
	int32_t scene_id;
	int32_t monster_point;
	uint8_t battle_type;
	uint8_t battle_result;
	int32_t monster_group;
	MSG_20200200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __monster_vec_size = monster.size();
		w.write_uint16(__monster_vec_size);
		for(uint16_t i = 0; i < __monster_vec_size; ++i) {
				monster[i].serialize(w);
		}

		uint16_t __kill_player_vec_size = kill_player.size();
		w.write_uint16(__kill_player_vec_size);
		for(uint16_t i = 0; i < __kill_player_vec_size; ++i) {
				w.write_int64(kill_player[i]);
		}

		w.write_int32(scene_id);
		w.write_int32(monster_point);
		w.write_uint8(battle_type);
		w.write_uint8(battle_result);
		w.write_int32(monster_group);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __monster_vec_size;
		if(r.read_uint16(__monster_vec_size)  )
				return -1;
		monster.reserve(__monster_vec_size);
		for(uint16_t i = 0; i < __monster_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				monster.push_back(v);
		}

		uint16_t __kill_player_vec_size;
		if(r.read_uint16(__kill_player_vec_size)  )
				return -1;
		kill_player.reserve(__kill_player_vec_size);
		for(uint16_t i = 0; i < __kill_player_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				kill_player.push_back(v);
		}

		if( r.read_int32(scene_id)  ||  r.read_int32(monster_point)  ||  r.read_uint8(battle_type)  ||  r.read_uint8(battle_result)  ||  r.read_int32(monster_group)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200200;
		monster.clear();
		kill_player.clear();
		scene_id = 0;
		monster_point = 0;
		battle_type = 0;
		battle_result = 0;
		monster_group = 0;
	}
};

/*
场景服属性改变通知
*/
struct MSG_20200110 : public Base_Msg  {
	int32_t prop_type;
	double value;
	MSG_20200110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(prop_type);
		w.write_double(value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(prop_type)  ||  r.read_double(value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200110;
		prop_type = 0;
		value = 0.0;
	}
};

/*
拾取掉落物品
*/
struct MSG_20300000 : public Base_Msg  {
	int64_t drops_role_id;
	int8_t item_index;
	std::vector<Drops_Item> item_vec;
	int32_t scene_type;
	int32_t scene_id;
	MSG_20300000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(drops_role_id);
		w.write_int8(item_index);
		uint16_t __item_vec_vec_size = item_vec.size();
		w.write_uint16(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				item_vec[i].serialize(w);
		}

		w.write_int32(scene_type);
		w.write_int32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(drops_role_id)  ||  r.read_int8(item_index)  )
				return -1;
 		uint16_t __item_vec_vec_size;
		if(r.read_uint16(__item_vec_vec_size)  )
				return -1;
		item_vec.reserve(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				Drops_Item v;
				if(v.deserialize(r))
						return -1;
				item_vec.push_back(v);
		}

		if( r.read_int32(scene_type)  ||  r.read_int32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300000;
		drops_role_id = 0;
		item_index = 0;
		item_vec.clear();
		scene_type = 0;
		scene_id = 0;
	}
};

/*
场景完成通知(所有场景)
*/
struct MSG_20200080 : public Base_Msg  {
	int32_t result;
	int32_t scene_id;
	MSG_20200080(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200080;
		result = 0;
		scene_id = 0;
	}
};

/*
同步成功学习技能
*/
struct MSG_20100018 : public Base_Msg  {
	int32_t skill_id;
	int32_t skill_lv;
	int32_t page_id;
	MSG_20100018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int32(skill_lv);
		w.write_int32(page_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int32(skill_lv)  ||  r.read_int32(page_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100018;
		skill_id = 0;
		skill_lv = 0;
		page_id = 0;
	}
};

/*
添加采集物到背包
*/
struct MSG_20300010 : public Base_Msg  {
	int64_t material_role_id;
	std::vector<Drops_Item> item_vec;
	MSG_20300010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(material_role_id);
		uint16_t __item_vec_vec_size = item_vec.size();
		w.write_uint16(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				item_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(material_role_id)  )
				return -1;
 		uint16_t __item_vec_vec_size;
		if(r.read_uint16(__item_vec_vec_size)  )
				return -1;
		item_vec.reserve(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				Drops_Item v;
				if(v.deserialize(r))
						return -1;
				item_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 20300010;
		material_role_id = 0;
		item_vec.clear();
	}
};

/*
同步任务数据
同步任务采集、收集源和物品信息至地图服
*/
struct MSG_20110001 : public Base_Msg  {
	MSG_20110001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20110001;
	}
};

/*
NPC触发的剧情结束通知
*/
struct MSG_20300020 : public Base_Msg  {
	std::string plot_id;
	int8_t plot_step;
	MSG_20300020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(plot_id);
		w.write_int8(plot_step);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(plot_id)  ||  r.read_int8(plot_step)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300020;
		plot_id.clear();
		plot_step = 0;
	}
};

/*
同步英雄基本信息
*/
struct MSG_20400000 : public Base_Msg  {
	MSG_20400000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20400000;
	}
};

/*
同步英雄战斗信息
*/
struct MSG_20400001 : public Base_Msg  {
	MSG_20400001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20400001;
	}
};

/*
通知场景服修改英雄属性
*/
struct MSG_20400002 : public Base_Msg  {
	int64_t master_role_id;
	int32_t type;
	int32_t value;
	MSG_20400002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(master_role_id);
		w.write_int32(type);
		w.write_int32(value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(master_role_id)  ||  r.read_int32(type)  ||  r.read_int32(value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20400002;
		master_role_id = 0;
		type = 0;
		value = 0;
	}
};

/*
通知场景服英雄跟随或收回
*/
struct MSG_20400003 : public Base_Msg  {
	int64_t master_role_id;
	int32_t is_out;
	MSG_20400003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(master_role_id);
		w.write_int32(is_out);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(master_role_id)  ||  r.read_int32(is_out)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20400003;
		master_role_id = 0;
		is_out = 0;
	}
};

/*
通知逻辑服英雄死亡
*/
struct MSG_20400004 : public Base_Msg  {
	int64_t master_role_id;
	MSG_20400004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(master_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(master_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20400004;
		master_role_id = 0;
	}
};

/*
同步英雄信息到逻辑服
*/
struct MSG_20400005 : public Base_Msg  {
	int64_t master_role_id;
	int32_t hero_index;
	double exp;
	std::vector<Int_Double> prop_value;
	MSG_20400005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(master_role_id);
		w.write_int32(hero_index);
		w.write_double(exp);
		uint16_t __prop_value_vec_size = prop_value.size();
		w.write_uint16(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				prop_value[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(master_role_id)  ||  r.read_int32(hero_index)  ||  r.read_double(exp)  )
				return -1;
 		uint16_t __prop_value_vec_size;
		if(r.read_uint16(__prop_value_vec_size)  )
				return -1;
		prop_value.reserve(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				Int_Double v;
				if(v.deserialize(r))
						return -1;
				prop_value.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 20400005;
		master_role_id = 0;
		hero_index = 0;
		exp = 0.0;
		prop_value.clear();
	}
};

/*
通知逻辑服玩家登录
*/
struct MSG_20100012 : public Base_Msg  {
	MSG_20100012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100012;
	}
};

/*
同步洗点技能
*/
struct MSG_20100019 : public Base_Msg  {
	int32_t type;
	int32_t page_id;
	MSG_20100019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_int32(page_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_int32(page_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100019;
		type = 0;
		page_id = 0;
	}
};

/*
同步移动技能位置
*/
struct MSG_20100020 : public Base_Msg  {
	int32_t skill_id;
	int32_t new_loc;
	int32_t page_id;
	MSG_20100020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int32(new_loc);
		w.write_int32(page_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int32(new_loc)  ||  r.read_int32(page_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100020;
		skill_id = 0;
		new_loc = 0;
		page_id = 0;
	}
};

/*
添加物品到背包（包括金钱、英灵等）
*/
struct MSG_20300011 : public Base_Msg  {
	std::vector<Drops_Item> goods_vec;
	int32_t scene_type;
	int32_t scene_id;
	MSG_20300011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __goods_vec_vec_size = goods_vec.size();
		w.write_uint16(__goods_vec_vec_size);
		for(uint16_t i = 0; i < __goods_vec_vec_size; ++i) {
				goods_vec[i].serialize(w);
		}

		w.write_int32(scene_type);
		w.write_int32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __goods_vec_vec_size;
		if(r.read_uint16(__goods_vec_vec_size)  )
				return -1;
		goods_vec.reserve(__goods_vec_vec_size);
		for(uint16_t i = 0; i < __goods_vec_vec_size; ++i) {
				Drops_Item v;
				if(v.deserialize(r))
						return -1;
				goods_vec.push_back(v);
		}

		if( r.read_int32(scene_type)  ||  r.read_int32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300011;
		goods_vec.clear();
		scene_type = 0;
		scene_id = 0;
	}
};

/*
同步开启天赋
*/
struct MSG_20100021 : public Base_Msg  {
	int32_t page_id;
	int32_t talent_id;
	MSG_20100021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(page_id);
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(page_id)  ||  r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100021;
		page_id = 0;
		talent_id = 0;
	}
};

/*
同步开启天赋页
*/
struct MSG_20100022 : public Base_Msg  {
	int32_t page_id;
	MSG_20100022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(page_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(page_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100022;
		page_id = 0;
	}
};

/*
同步使用天赋页
*/
struct MSG_20100023 : public Base_Msg  {
	int32_t page_id;
	MSG_20100023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(page_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(page_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100023;
		page_id = 0;
	}
};

/*
同步公会信息
*/
struct MSG_20170000 : public Base_Msg  {
	int64_t gang_id;
	int64_t member_id;
	std::string gang_name;
	int8_t headship;
	MSG_20170000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int64(member_id);
		w.write_string(gang_name);
		w.write_int8(headship);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int64(member_id)  ||  r.read_string(gang_name)  ||  r.read_int8(headship)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20170000;
		gang_id = 0;
		member_id = 0;
		gang_name.clear();
		headship = 0;
	}
};

/*
技能逻辑服消耗
*/
struct MSG_20100024 : public Base_Msg  {
	int32_t type;
	int32_t arg1;
	int32_t arg2;
	int32_t cost;
	MSG_20100024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_int32(arg1);
		w.write_int32(arg2);
		w.write_int32(cost);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_int32(arg1)  ||  r.read_int32(arg2)  ||  r.read_int32(cost)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100024;
		type = 0;
		arg1 = 0;
		arg2 = 0;
		cost = 0;
	}
};

/*
技能逻辑服成功消耗
*/
struct MSG_20100025 : public Base_Msg  {
	int32_t type;
	int32_t arg1;
	int32_t arg2;
	MSG_20100025(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_int32(arg1);
		w.write_int32(arg2);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_int32(arg1)  ||  r.read_int32(arg2)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100025;
		type = 0;
		arg1 = 0;
		arg2 = 0;
	}
};

/*
同步黑名单列表
*/
struct MSG_20160000 : public Base_Msg  {
	int64_t role_id;
	int8_t type;
	MSG_20160000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20160000;
		role_id = 0;
		type = 0;
	}
};

/*
发布系统公告
*/
struct MSG_20500000 : public Base_Msg  {
	int8_t district;
	int32_t announce_id;
	int64_t gang_id;
	std::vector<int64_t> recievers;
	std::vector<Chat_Content> contents;
	MSG_20500000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(district);
		w.write_int32(announce_id);
		w.write_int64(gang_id);
		uint16_t __recievers_vec_size = recievers.size();
		w.write_uint16(__recievers_vec_size);
		for(uint16_t i = 0; i < __recievers_vec_size; ++i) {
				w.write_int64(recievers[i]);
		}

		uint16_t __contents_vec_size = contents.size();
		w.write_uint16(__contents_vec_size);
		for(uint16_t i = 0; i < __contents_vec_size; ++i) {
				contents[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(district)  ||  r.read_int32(announce_id)  ||  r.read_int64(gang_id)  )
				return -1;
 		uint16_t __recievers_vec_size;
		if(r.read_uint16(__recievers_vec_size)  )
				return -1;
		recievers.reserve(__recievers_vec_size);
		for(uint16_t i = 0; i < __recievers_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				recievers.push_back(v);
		}

		uint16_t __contents_vec_size;
		if(r.read_uint16(__contents_vec_size)  )
				return -1;
		contents.reserve(__contents_vec_size);
		for(uint16_t i = 0; i < __contents_vec_size; ++i) {
				Chat_Content v;
				if(v.deserialize(r))
						return -1;
				contents.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 20500000;
		district = 0;
		announce_id = 0;
		gang_id = 0;
		recievers.clear();
		contents.clear();
	}
};

/*
通知聊天服客户端初始化完成
*/
struct MSG_20010085 : public Base_Msg  {
	MSG_20010085(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20010085;
	}
};

/*
逻辑服通知场景服竞技场战斗开始
*/
struct MSG_20300030 : public Base_Msg  {
	MSG_20300030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300030;
	}
};

/*
场景服通知逻辑服竞技场战斗结束
*/
struct MSG_20300031 : public Base_Msg  {
	MSG_20300031(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300031;
	}
};

/*
多人场景完成通知(所有场景)
*/
struct MSG_20200081 : public Base_Msg  {
	int32_t result;
	int32_t scene_id;
	std::vector<int64_t> no_profit_player_vec;
	MSG_20200081(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int32(scene_id);
		uint16_t __no_profit_player_vec_vec_size = no_profit_player_vec.size();
		w.write_uint16(__no_profit_player_vec_vec_size);
		for(uint16_t i = 0; i < __no_profit_player_vec_vec_size; ++i) {
				w.write_int64(no_profit_player_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int32(scene_id)  )
				return -1;
 		uint16_t __no_profit_player_vec_vec_size;
		if(r.read_uint16(__no_profit_player_vec_vec_size)  )
				return -1;
		no_profit_player_vec.reserve(__no_profit_player_vec_vec_size);
		for(uint16_t i = 0; i < __no_profit_player_vec_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				no_profit_player_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 20200081;
		result = 0;
		scene_id = 0;
		no_profit_player_vec.clear();
	}
};

/*
同步个人队伍信息到聊天服
*/
struct MSG_20100301 : public Base_Msg  {
	MSG_20100301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100301;
	}
};

/*
同步队伍信息到聊天服
*/
struct MSG_20100302 : public Base_Msg  {
	MSG_20100302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100302;
	}
};

/*
逻辑服通知地图服英雄战斗
*/
struct MSG_20400007 : public Base_Msg  {
	int64_t master_role_id;
	int8_t is_fight;
	MSG_20400007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(master_role_id);
		w.write_int8(is_fight);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(master_role_id)  ||  r.read_int8(is_fight)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20400007;
		master_role_id = 0;
		is_fight = 0;
	}
};

/*
同步体力信息
*/
struct MSG_20100030 : public Base_Msg  {
	uint16_t phy_power_cur;
	int32_t phy_power_recover_time;
	uint8_t phy_power_buy_times;
	MSG_20100030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(phy_power_cur);
		w.write_int32(phy_power_recover_time);
		w.write_uint8(phy_power_buy_times);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(phy_power_cur)  ||  r.read_int32(phy_power_recover_time)  ||  r.read_uint8(phy_power_buy_times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100030;
		phy_power_cur = 0;
		phy_power_recover_time = 0;
		phy_power_buy_times = 0;
	}
};

/*
同步个人队伍信息到地图服
*/
struct MSG_20100303 : public Base_Msg  {
	MSG_20100303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100303;
	}
};

/*
同步队伍信息到地图服
*/
struct MSG_20100304 : public Base_Msg  {
	MSG_20100304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100304;
	}
};

/*
使用物品需要地图服处理的道具
tp:0 血包 1 经验
*/
struct MSG_20100305 : public Base_Msg  {
	int16_t tp;
	double val;
	uint32_t index;
	int32_t amount;
	MSG_20100305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(tp);
		w.write_double(val);
		w.write_uint32(index);
		w.write_int32(amount);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(tp)  ||  r.read_double(val)  ||  r.read_uint32(index)  ||  r.read_int32(amount)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100305;
		tp = 0;
		val = 0.0;
		index = 0;
		amount = 0;
	}
};

/*
地图服通知英雄出战
*/
struct MSG_20400008 : public Base_Msg  {
	MSG_20400008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20400008;
	}
};

/*
添加采集物到背包结果
*/
struct MSG_20300012 : public Base_Msg  {
	int64_t role_id;
	int32_t result;
	MSG_20300012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300012;
		role_id = 0;
		result = 0;
	}
};

/*
逻辑服通知地图服回满指定属性
*/
struct MSG_20100400 : public Base_Msg  {
	int32_t type;
	MSG_20100400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100400;
		type = 0;
	}
};

/*
传递状态数据
*/
struct MSG_20100500 : public Base_Msg  {
	MSG_20100500(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100500;
	}
};

/*
通知聊天服小喇叭聊天
*/
struct MSG_20100099 : public Base_Msg  {
	std::vector<Chat_Content> content;
	MSG_20100099(void) { reset(); };

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
		msg_id = 20100099;
		content.clear();
	}
};

/*
检测战斗状态
是否在战斗中
*/
struct MSG_20100401 : public Base_Msg  {
	MSG_20100401(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100401;
	}
};

/*
地图服通知逻辑服战斗结果
*/
struct MSG_20100402 : public Base_Msg  {
	int32_t is_win;
	MSG_20100402(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(is_win);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(is_win)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100402;
		is_win = 0;
	}
};

/*
获取玩家人物属性
*/
struct MSG_20410000 : public Base_Msg  {
	MSG_20410000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20410000;
	}
};

/*
发送公会祝福
*/
struct MSG_20420000 : public Base_Msg  {
	MSG_20420000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20420000;
	}
};

/*
逻辑服插入buff接口
*/
struct MSG_20100501 : public Base_Msg  {
	MSG_20100501(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100501;
	}
};

/*
进入战场
*/
struct MSG_20300100 : public Base_Msg  {
	MSG_20300100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300100;
	}
};

/*
同步战场信息
*/
struct MSG_20300101 : public Base_Msg  {
	MSG_20300101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300101;
	}
};

/*
通知地图服被动技能
*/
struct MSG_20100403 : public Base_Msg  {
	MSG_20100403(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100403;
	}
};

/*
同步坐骑骑乘
*/
struct MSG_20100040 : public Base_Msg  {
	int8_t ride;
	int32_t model_id;
	int32_t speed;
	MSG_20100040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(ride);
		w.write_int32(model_id);
		w.write_int32(speed);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(ride)  ||  r.read_int32(model_id)  ||  r.read_int32(speed)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100040;
		ride = 0;
		model_id = 0;
		speed = 0;
	}
};

/*
传送称号数据
*/
struct MSG_20100600 : public Base_Msg  {
	MSG_20100600(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100600;
	}
};

/*
掠夺匹配
*/
struct MSG_20600000 : public Base_Msg  {
	int64_t uuid;
	std::string server_name;
	int64_t role_id;
	uint32_t coin;
	uint32_t last_coin;
	MSG_20600000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(uuid);
		w.write_string(server_name);
		w.write_int64(role_id);
		w.write_uint32(coin);
		w.write_uint32(last_coin);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(uuid)  ||  r.read_string(server_name)  ||  r.read_int64(role_id)  ||  r.read_uint32(coin)  ||  r.read_uint32(last_coin)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600000;
		uuid = 0;
		server_name.clear();
		role_id = 0;
		coin = 0;
		last_coin = 0;
	}
};

/*
掠夺匹配
*/
struct MSG_20600001 : public Base_Msg  {
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
	uint32_t level;
	uint32_t coin;
	uint32_t last_coin;
	MSG_20600001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_string(server_name);
		w.write_string(role_name);
		w.write_uint32(level);
		w.write_uint32(coin);
		w.write_uint32(last_coin);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_string(server_name)  ||  r.read_string(role_name)  ||  r.read_uint32(level)  ||  r.read_uint32(coin)  ||  r.read_uint32(last_coin)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600001;
		uuid = 0;
		role_id = 0;
		server_name.clear();
		role_name.clear();
		level = 0;
		coin = 0;
		last_coin = 0;
	}
};

/*
通知聊天服广播全服buff接口
*/
struct MSG_20100098 : public Base_Msg  {
	MSG_20100098(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100098;
	}
};

/*
掠夺匹配返回
*/
struct MSG_20600002 : public Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
	std::string to_server_name;
	std::string to_role_name;
	int64_t uuid;
	std::string server_name;
	int64_t role_id;
	std::vector<Rob_Match_Member> match_member_list;
	uint32_t fight;
	uint32_t rob_gold_mine;
	uint32_t rob_moon_well;
	uint32_t rob_totem;
	uint32_t coin;
	MSG_20600002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_string(to_server_name);
		w.write_string(to_role_name);
		w.write_int64(uuid);
		w.write_string(server_name);
		w.write_int64(role_id);
		uint16_t __match_member_list_vec_size = match_member_list.size();
		w.write_uint16(__match_member_list_vec_size);
		for(uint16_t i = 0; i < __match_member_list_vec_size; ++i) {
				match_member_list[i].serialize(w);
		}

		w.write_uint32(fight);
		w.write_uint32(rob_gold_mine);
		w.write_uint32(rob_moon_well);
		w.write_uint32(rob_totem);
		w.write_uint32(coin);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_string(to_server_name)  ||  r.read_string(to_role_name)  ||  r.read_int64(uuid)  ||  r.read_string(server_name)  ||  r.read_int64(role_id)  )
				return -1;
 		uint16_t __match_member_list_vec_size;
		if(r.read_uint16(__match_member_list_vec_size)  )
				return -1;
		match_member_list.reserve(__match_member_list_vec_size);
		for(uint16_t i = 0; i < __match_member_list_vec_size; ++i) {
				Rob_Match_Member v;
				if(v.deserialize(r))
						return -1;
				match_member_list.push_back(v);
		}

		if( r.read_uint32(fight)  ||  r.read_uint32(rob_gold_mine)  ||  r.read_uint32(rob_moon_well)  ||  r.read_uint32(rob_totem)  ||  r.read_uint32(coin)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600002;
		to_uuid = 0;
		to_role_id = 0;
		to_server_name.clear();
		to_role_name.clear();
		uuid = 0;
		server_name.clear();
		role_id = 0;
		match_member_list.clear();
		fight = 0;
		rob_gold_mine = 0;
		rob_moon_well = 0;
		rob_totem = 0;
		coin = 0;
	}
};

/*
逻辑服通知场景服VIP信息改变
*/
struct MSG_20200020 : public Base_Msg  {
	uint32_t vip;
	int64_t vip_over_time;
	MSG_20200020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(vip);
		w.write_int64(vip_over_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(vip)  ||  r.read_int64(vip_over_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200020;
		vip = 0;
		vip_over_time = 0;
	}
};

/*
聊天服通知逻辑服进行了一次世界聊天
*/
struct MSG_20200021 : public Base_Msg  {
	MSG_20200021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200021;
	}
};

/*
同步公会战公会信息
*/
struct MSG_20170001 : public Base_Msg  {
	MSG_20170001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170001;
	}
};

/*
同步公会战玩家信息
*/
struct MSG_20170002 : public Base_Msg  {
	MSG_20170002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170002;
	}
};

/*
同步神秘商店
*/
struct MSG_20300200 : public Base_Msg  {
	MSG_20300200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300200;
	}
};

/*
掠夺战斗加载玩家数据
*/
struct MSG_20600003 : public Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
	std::string to_server_name;
	std::string to_role_name;
	int64_t role_id;
	MSG_20600003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_string(to_server_name);
		w.write_string(to_role_name);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_string(to_server_name)  ||  r.read_string(to_role_name)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600003;
		to_uuid = 0;
		to_role_id = 0;
		to_server_name.clear();
		to_role_name.clear();
		role_id = 0;
	}
};

/*
掠夺战斗加载玩家数据(返回)
*/
struct MSG_20600004 : public Base_Msg  {
	MSG_20600004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20600004;
	}
};

/*
逻辑服通知场景服魅力值改变
*/
struct MSG_20200022 : public Base_Msg  {
	uint32_t num;
	MSG_20200022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200022;
		num = 0;
	}
};

/*
掠夺战斗结束，场景通知逻辑服保存离线掠夺者的战斗结果
*/
struct MSG_20600005 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint8_t result;
	int64_t role_id;
	MSG_20600005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint8(result);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint8(result)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600005;
		server_name.clear();
		player_name.clear();
		result = 0;
		role_id = 0;
	}
};

/*
掠夺战斗结束，场景通知逻辑服保存离线被掠夺者的战斗结果
*/
struct MSG_20600006 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint8_t result;
	int64_t role_id;
	MSG_20600006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint8(result);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint8(result)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600006;
		server_name.clear();
		player_name.clear();
		result = 0;
		role_id = 0;
	}
};

/*
同步公会战排行榜数据
*/
struct MSG_20170003 : public Base_Msg  {
	MSG_20170003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170003;
	}
};

/*
请求公会战排行榜数据
*/
struct MSG_20170004 : public Base_Msg  {
	MSG_20170004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170004;
	}
};

/*
掠夺者进入被掠夺者的跨服龙谷
*/
struct MSG_20600007 : public Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
	int64_t uuid;
	int64_t role_id;
	uint8_t limit_time;
	MSG_20600007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_uint8(limit_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_uint8(limit_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600007;
		to_uuid = 0;
		to_role_id = 0;
		uuid = 0;
		role_id = 0;
		limit_time = 0;
	}
};

/*
掠夺者进入被掠夺者的跨服龙谷(返回)
*/
struct MSG_20600008 : public Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
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
	MSG_20600008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
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
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_uint8(limit_time)  ||  r.read_int64(uuid)  )
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
		msg_id = 20600008;
		to_uuid = 0;
		to_role_id = 0;
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
掠夺者请求收获被掠者的跨服龙谷资源
*/
struct MSG_20600009 : public Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
	std::string to_server_name;
	std::string to_role_name;
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
	uint32_t building_id;
	uint8_t building_type;
	MSG_20600009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_string(to_server_name);
		w.write_string(to_role_name);
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_string(server_name);
		w.write_string(role_name);
		w.write_uint32(building_id);
		w.write_uint8(building_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_string(to_server_name)  ||  r.read_string(to_role_name)  ||  r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_string(server_name)  ||  r.read_string(role_name)  ||  r.read_uint32(building_id)  ||  r.read_uint8(building_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600009;
		to_uuid = 0;
		to_role_id = 0;
		to_server_name.clear();
		to_role_name.clear();
		uuid = 0;
		role_id = 0;
		server_name.clear();
		role_name.clear();
		building_id = 0;
		building_type = 0;
	}
};

/*
掠夺者请求收获被掠夺者的跨服龙谷资源(返回)
*/
struct MSG_20600010 : public Base_Msg  {
	int32_t error_id;
	int64_t to_uuid;
	int64_t to_role_id;
	std::string to_server_name;
	std::string to_role_name;
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
	uint32_t gain_production;
	Building_Info building_info;
	MSG_20600010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(error_id);
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_string(to_server_name);
		w.write_string(to_role_name);
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_string(server_name);
		w.write_string(role_name);
		w.write_uint32(gain_production);
		building_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(error_id)  ||  r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_string(to_server_name)  ||  r.read_string(to_role_name)  ||  r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_string(server_name)  ||  r.read_string(role_name)  ||  r.read_uint32(gain_production)  ||  building_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600010;
		error_id = 0;
		to_uuid = 0;
		to_role_id = 0;
		to_server_name.clear();
		to_role_name.clear();
		uuid = 0;
		role_id = 0;
		server_name.clear();
		role_name.clear();
		gain_production = 0;
		building_info.reset();
	}
};

/*
同步公会战结果
*/
struct MSG_20170005 : public Base_Msg  {
	MSG_20170005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170005;
	}
};

/*
掠夺资源结束，场景通知逻辑服保存离线被掠夺者的资源被掠夺通知
*/
struct MSG_20600011 : public Base_Msg  {
	std::string server_name;
	std::string role_name;
	uint32_t total_gold_mine;
	uint32_t total_moon_weel;
	uint32_t total_totem;
	int64_t role_id;
	MSG_20600011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(role_name);
		w.write_uint32(total_gold_mine);
		w.write_uint32(total_moon_weel);
		w.write_uint32(total_totem);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(role_name)  ||  r.read_uint32(total_gold_mine)  ||  r.read_uint32(total_moon_weel)  ||  r.read_uint32(total_totem)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600011;
		server_name.clear();
		role_name.clear();
		total_gold_mine = 0;
		total_moon_weel = 0;
		total_totem = 0;
		role_id = 0;
	}
};

/*
通知场景广播龙谷内的变化
*/
struct MSG_20600012 : public Base_Msg  {
	MSG_20600012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20600012;
	}
};

/*
场景通知逻辑服玩家退出龙谷
*/
struct MSG_20600013 : public Base_Msg  {
	int64_t creater;
	int64_t role_id;
	MSG_20600013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(creater);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(creater)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600013;
		creater = 0;
		role_id = 0;
	}
};

/*
公会战活动状态
*/
struct MSG_20170006 : public Base_Msg  {
	MSG_20170006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170006;
	}
};

/*
逻辑服加技能点
*/
struct MSG_20100026 : public Base_Msg  {
	int32_t point;
	MSG_20100026(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100026;
		point = 0;
	}
};

/*
进入篝火活动
*/
struct MSG_20300110 : public Base_Msg  {
	MSG_20300110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300110;
	}
};

/*
篝火活动开始
*/
struct MSG_20300111 : public Base_Msg  {
	MSG_20300111(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300111;
	}
};

/*
同步篝火信息
*/
struct MSG_20300112 : public Base_Msg  {
	MSG_20300112(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300112;
	}
};

/*
龙谷场景错误提示
*/
struct MSG_20600014 : public Base_Msg  {
	MSG_20600014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20600014;
	}
};

/*
龙谷刷怪
*/
struct MSG_20600015 : public Base_Msg  {
	uint8_t type;
	int64_t role_id;
	int64_t id;
	uint8_t quality;
	MSG_20600015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_int64(role_id);
		w.write_int64(id);
		w.write_uint8(quality);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_int64(role_id)  ||  r.read_int64(id)  ||  r.read_uint8(quality)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600015;
		type = 0;
		role_id = 0;
		id = 0;
		quality = 0;
	}
};

/*
同步公会战奖励
*/
struct MSG_20170007 : public Base_Msg  {
	MSG_20170007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170007;
	}
};

/*
同步公会战协助请求
*/
struct MSG_20170008 : public Base_Msg  {
	MSG_20170008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170008;
	}
};

/*
龙谷清理怪
*/
struct MSG_20600016 : public Base_Msg  {
	int64_t player_id;
	int64_t role_id;
	int64_t id;
	uint8_t quality;
	MSG_20600016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(player_id);
		w.write_int64(role_id);
		w.write_int64(id);
		w.write_uint8(quality);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(player_id)  ||  r.read_int64(role_id)  ||  r.read_int64(id)  ||  r.read_uint8(quality)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600016;
		player_id = 0;
		role_id = 0;
		id = 0;
		quality = 0;
	}
};

/*
同步公会战奖励
*/
struct MSG_20170009 : public Base_Msg  {
	MSG_20170009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170009;
	}
};

/*
公会战请求决斗
*/
struct MSG_20170010 : public Base_Msg  {
	MSG_20170010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170010;
	}
};

/*
通知逻辑服记录被掠夺者的布告栏，已经将掠夺者赶跑
*/
struct MSG_20600017 : public Base_Msg  {
	std::string name;
	int64_t role_id;
	MSG_20600017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(name);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(name)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600017;
		name.clear();
		role_id = 0;
	}
};

/*
通知逻辑服记录被掠夺者的布告栏，被掠夺了多少资源
*/
struct MSG_20600018 : public Base_Msg  {
	int64_t role_id;
	std::string name;
	uint32_t gold_mine;
	uint32_t moon_well;
	uint32_t totem;
	MSG_20600018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(name);
		w.write_uint32(gold_mine);
		w.write_uint32(moon_well);
		w.write_uint32(totem);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(name)  ||  r.read_uint32(gold_mine)  ||  r.read_uint32(moon_well)  ||  r.read_uint32(totem)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600018;
		role_id = 0;
		name.clear();
		gold_mine = 0;
		moon_well = 0;
		totem = 0;
	}
};

/*
通知逻辑服被掠夺者进入掠夺保护时间
*/
struct MSG_20600019 : public Base_Msg  {
	int64_t role_id;
	MSG_20600019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600019;
		role_id = 0;
	}
};

/*
掠夺匹配失败，返还金币
*/
struct MSG_20600020 : public Base_Msg  {
	int64_t uuid;
	int64_t role_id;
	uint32_t coin;
	uint8_t type;
	MSG_20600020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_uint32(coin);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_uint32(coin)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600020;
		uuid = 0;
		role_id = 0;
		coin = 0;
		type = 0;
	}
};

/*
龙谷被掠夺者战斗胜利，通知逻辑服清除掠夺保护状态
*/
struct MSG_20600021 : public Base_Msg  {
	int64_t role_id;
	MSG_20600021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600021;
		role_id = 0;
	}
};

/*
通知逻辑服，清除掠夺资源的进度
*/
struct MSG_20600022 : public Base_Msg  {
	int64_t to_role_id;
	MSG_20600022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600022;
		to_role_id = 0;
	}
};

/*
通知逻辑服，记录TOD助手，掠夺者收获掠夺资源完成
备注：另外：成就也在这里监听
*/
struct MSG_20600023 : public Base_Msg  {
	int64_t to_role_id;
	MSG_20600023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600023;
		to_role_id = 0;
	}
};

/*
通知逻辑服，设置龙谷主人被掠夺资源后第一次进入龙谷的标志
*/
struct MSG_20600024 : public Base_Msg  {
	int64_t role_id;
	MSG_20600024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600024;
		role_id = 0;
	}
};

/*
内部通知字符串信息改变更新
*/
struct MSG_20100050 : public Base_Msg  {
	MSG_20100050(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100050;
	}
};

/*
活动结果通知
*/
struct MSG_20200201 : public Base_Msg  {
	int32_t act_id;
	int8_t result;
	int32_t value1;
	int32_t value2;
	int8_t type;
	int32_t value3;
	MSG_20200201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(act_id);
		w.write_int8(result);
		w.write_int32(value1);
		w.write_int32(value2);
		w.write_int8(type);
		w.write_int32(value3);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(act_id)  ||  r.read_int8(result)  ||  r.read_int32(value1)  ||  r.read_int32(value2)  ||  r.read_int8(type)  ||  r.read_int32(value3)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200201;
		act_id = 0;
		result = 0;
		value1 = 0;
		value2 = 0;
		type = 0;
		value3 = 0;
	}
};

/*
逻辑服删除状态接口
*/
struct MSG_20100502 : public Base_Msg  {
	MSG_20100502(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100502;
	}
};

/*
逻辑服同步头像到场景服
*/
struct MSG_20200023 : public Base_Msg  {
	uint32_t head_id;
	MSG_20200023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(head_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(head_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200023;
		head_id = 0;
	}
};

/*
内部通知整型信息改变更新
*/
struct MSG_20100051 : public Base_Msg  {
	MSG_20100051(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100051;
	}
};

/*
英雄添加状态
*/
struct MSG_20100503 : public Base_Msg  {
	MSG_20100503(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100503;
	}
};

/*
通知逻辑服，掠夺任务监听
*/
struct MSG_20600025 : public Base_Msg  {
	MSG_20600025(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20600025;
	}
};

/*
被掠夺者战斗胜利，通知被掠夺者的服务器全服广播
*/
struct MSG_20600026 : public Base_Msg  {
	std::string to_server_name;
	std::string to_role_name;
	int64_t role_id;
	MSG_20600026(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(to_server_name);
		w.write_string(to_role_name);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(to_server_name)  ||  r.read_string(to_role_name)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600026;
		to_server_name.clear();
		to_role_name.clear();
		role_id = 0;
	}
};

/*
组队竞技传参
*/
struct MSG_20100306 : public Base_Msg  {
	MSG_20100306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100306;
	}
};

/*
竞技场同步排名相关信息
*/
struct MSG_20300032 : public Base_Msg  {
	MSG_20300032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300032;
	}
};

/*
竞技场请求战斗数据
*/
struct MSG_20300033 : public Base_Msg  {
	MSG_20300033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300033;
	}
};

/*
竞技场请求匹配跨服玩家
*/
struct MSG_20300034 : public Base_Msg  {
	MSG_20300034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300034;
	}
};

/*
竞技场请求跨服排行信息
*/
struct MSG_20300035 : public Base_Msg  {
	MSG_20300035(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300035;
	}
};

/*
竞技场同步跨服排名
*/
struct MSG_20300036 : public Base_Msg  {
	std::vector<Id_Int_Int> rank_info;
	MSG_20300036(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __rank_info_vec_size = rank_info.size();
		w.write_uint16(__rank_info_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_size; ++i) {
				rank_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __rank_info_vec_size;
		if(r.read_uint16(__rank_info_vec_size)  )
				return -1;
		rank_info.reserve(__rank_info_vec_size);
		for(uint16_t i = 0; i < __rank_info_vec_size; ++i) {
				Id_Int_Int v;
				if(v.deserialize(r))
						return -1;
				rank_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 20300036;
		rank_info.clear();
	}
};

/*
同步avatar时装到场景服
*/
struct MSG_20100060 : public Base_Msg  {
	std::vector<int32_t> avatar_vec;
	MSG_20100060(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __avatar_vec_vec_size = avatar_vec.size();
		w.write_uint16(__avatar_vec_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_vec_size; ++i) {
				w.write_int32(avatar_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
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

		return 0;
	}

	void reset(void) {
		msg_id = 20100060;
		avatar_vec.clear();
	}
};

/*
同步场景服学习技能到逻辑服
*/
struct MSG_20100017 : public Base_Msg  {
	int32_t skill_id;
	int8_t lv;
	MSG_20100017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int8(lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int8(lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100017;
		skill_id = 0;
		lv = 0;
	}
};

/*
骑士试炼请求匹配跨服玩家
*/
struct MSG_20610000 : public Base_Msg  {
	uint8_t type;
	MSG_20610000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20610000;
		type = 0;
	}
};

/*
竞技场战斗数据同步
*/
struct MSG_20300037 : public Base_Msg  {
	MSG_20300037(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300037;
	}
};

/*
竞技场战斗结果同步跨服
*/
struct MSG_20300038 : public Base_Msg  {
	MSG_20300038(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300038;
	}
};

/*
同步英雄状态属性
*/
struct MSG_20100504 : public Base_Msg  {
	MSG_20100504(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100504;
	}
};

/*
同步战力到逻辑服
*/
struct MSG_20100027 : public Base_Msg  {
	uint32_t force;
	MSG_20100027(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(force);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(force)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100027;
		force = 0;
	}
};

/*
骑士试炼通知场景进入战斗
*/
struct MSG_20610001 : public Base_Msg  {
	MSG_20610001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20610001;
	}
};

/*
竞技场王者名称参数变化通知
*/
struct MSG_20300039 : public Base_Msg  {
	int64_t role_id;
	int32_t king_time;
	MSG_20300039(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(king_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(king_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300039;
		role_id = 0;
		king_time = 0;
	}
};

/*
竞技场赛季结束通知
*/
struct MSG_20300040 : public Base_Msg  {
	MSG_20300040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300040;
	}
};

/*
掠夺者在掠夺战斗中胜利，通知逻辑服准备相关数据
*/
struct MSG_20600027 : public Base_Msg  {
	MSG_20600027(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20600027;
	}
};

/*
内部远征逻辑服于场景服之间的数据传输通道
*/
struct MSG_20610010 : public Base_Msg  {
	MSG_20610010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20610010;
	}
};

/*
远征系统玩家退出场景通知玩家记录最新信息
*/
struct MSG_20610011 : public Base_Msg  {
	Coord coord;
	MSG_20610011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20610011;
		coord.reset();
	}
};

/*
刷新人物面板属性
*/
struct MSG_20410001 : public Base_Msg  {
	MSG_20410001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20410001;
	}
};

/*
同步玩家角色名
*/
struct MSG_20100028 : public Base_Msg  {
	MSG_20100028(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100028;
	}
};

/*
通知逻辑服，掠夺者总共掠夺了多少资源
*/
struct MSG_20600028 : public Base_Msg  {
	int64_t role_id;
	uint32_t gold_mine;
	uint32_t moon_well;
	uint32_t totem;
	MSG_20600028(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint32(gold_mine);
		w.write_uint32(moon_well);
		w.write_uint32(totem);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint32(gold_mine)  ||  r.read_uint32(moon_well)  ||  r.read_uint32(totem)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600028;
		role_id = 0;
		gold_mine = 0;
		moon_well = 0;
		totem = 0;
	}
};

/*
通知逻辑服，掠夺者开始进攻
*/
struct MSG_20600029 : public Base_Msg  {
	MSG_20600029(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20600029;
	}
};

/*
组队副本、英雄梦镜
*/
struct MSG_20100307 : public Base_Msg  {
	MSG_20100307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100307;
	}
};

/*
异步状态操作
*/
struct MSG_20100100 : public Base_Msg  {
	MSG_20100100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100100;
	}
};

/*
内部打开人物面板
*/
struct MSG_20100200 : public Base_Msg  {
	MSG_20100200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100200;
	}
};

/*
内部查询战斗状态
*/
struct MSG_20100308 : public Base_Msg  {
	MSG_20100308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100308;
	}
};

/*
检测信息传到中央服务器
*/
struct MSG_20999999 : public Base_Msg  {
	std::string server;
	int32_t core_num;
	MSG_20999999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server);
		w.write_int32(core_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server)  ||  r.read_int32(core_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20999999;
		server.clear();
		core_num = 0;
	}
};

/*
通知被掠夺者可以进入龙谷了
*/
struct MSG_20600030 : public Base_Msg  {
	int64_t role_id;
	MSG_20600030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600030;
		role_id = 0;
	}
};

/*
通知聊天服VIP信息改变
*/
struct MSG_20200024 : public Base_Msg  {
	int32_t over_time;
	int32_t vip_level;
	MSG_20200024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(over_time);
		w.write_int32(vip_level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(over_time)  ||  r.read_int32(vip_level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200024;
		over_time = 0;
		vip_level = 0;
	}
};

/*
同步擂台双方玩家到逻辑服
*/
struct MSG_20100070 : public Base_Msg  {
	MSG_20100070(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100070;
	}
};

/*
内部智慧试练逻辑与场景间数据通道
*/
struct MSG_20610020 : public Base_Msg  {
	MSG_20610020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20610020;
	}
};

/*
场景服请求临时英雄
*/
struct MSG_20400009 : public Base_Msg  {
	MSG_20400009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20400009;
	}
};

/*
公会战产生16名单并随机匹配
*/
struct MSG_20170011 : public Base_Msg  {
	MSG_20170011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20170011;
	}
};

/*
通知聊天服等级改变
*/
struct MSG_20200025 : public Base_Msg  {
	uint32_t level;
	MSG_20200025(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20200025;
		level = 0;
	}
};

/*
聊天黑/白名单
*/
struct MSG_20200026 : public Base_Msg  {
	MSG_20200026(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200026;
	}
};

/*
被掠夺者逻辑服/掠夺者龙谷场景服/被掠夺者主城场景服处理掠夺
*/
struct MSG_20600031 : public Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
	std::string to_server_name;
	std::string to_role_name;
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
	uint32_t coin;
	uint8_t flag;
	MSG_20600031(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_string(to_server_name);
		w.write_string(to_role_name);
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_string(server_name);
		w.write_string(role_name);
		w.write_uint32(coin);
		w.write_uint8(flag);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_string(to_server_name)  ||  r.read_string(to_role_name)  ||  r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_string(server_name)  ||  r.read_string(role_name)  ||  r.read_uint32(coin)  ||  r.read_uint8(flag)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600031;
		to_uuid = 0;
		to_role_id = 0;
		to_server_name.clear();
		to_role_name.clear();
		uuid = 0;
		role_id = 0;
		server_name.clear();
		role_name.clear();
		coin = 0;
		flag = 0;
	}
};

/*
通知被掠夺者战斗后的结果
*/
struct MSG_20600032 : public Base_Msg  {
	std::string server_name;
	std::string player_name;
	uint8_t result;
	uint8_t is_drive;
	int64_t role_id;
	MSG_20600032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_string(player_name);
		w.write_uint8(result);
		w.write_uint8(is_drive);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_string(player_name)  ||  r.read_uint8(result)  ||  r.read_uint8(is_drive)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600032;
		server_name.clear();
		player_name.clear();
		result = 0;
		is_drive = 0;
		role_id = 0;
	}
};

/*
龙谷正在被掠夺，不可进入
*/
struct MSG_20600033 : public Base_Msg  {
	uint32_t left_time;
	int64_t role_id;
	MSG_20600033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(left_time);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(left_time)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600033;
		left_time = 0;
		role_id = 0;
	}
};

/*
开始/结束被入侵龙谷的战斗中
*/
struct MSG_20600034 : public Base_Msg  {
	int64_t role_id;
	uint8_t flag;
	MSG_20600034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(flag);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(flag)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20600034;
		role_id = 0;
		flag = 0;
	}
};

/*
同步活动场景信息
备注：
active_id：16 世界boss
*/
struct MSG_20300300 : public Base_Msg  {
	MSG_20300300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300300;
	}
};

/*
同步活动场景玩家进入
*/
struct MSG_20300301 : public Base_Msg  {
	MSG_20300301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300301;
	}
};

/*
活动场景开启
*/
struct MSG_20300302 : public Base_Msg  {
	MSG_20300302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300302;
	}
};

/*
同步活动场景结束
*/
struct MSG_20300303 : public Base_Msg  {
	MSG_20300303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300303;
	}
};

/*
战斗结束通知逻辑服
*/
struct MSG_20200202 : public Base_Msg  {
	MSG_20200202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20200202;
	}
};

/*
远征玩家数据通道
*/
struct MSG_20610012 : public Base_Msg  {
	MSG_20610012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20610012;
	}
};

/*
通知逻辑服玩家进入场景
*/
struct MSG_20100013 : public Base_Msg  {
	MSG_20100013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100013;
	}
};

/*
同步组队竞技积分
备注：integrate
*/
struct MSG_20100309 : public Base_Msg  {
	MSG_20100309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100309;
	}
};

/*
获取组队积分榜
*/
struct MSG_20100310 : public Base_Msg  {
	int64_t role_id;
	int32_t level;
	MSG_20100310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100310;
		role_id = 0;
		level = 0;
	}
};

/*
同步组队竞技排名
*/
struct MSG_20100312 : public Base_Msg  {
	int32_t rank;
	MSG_20100312(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(rank);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(rank)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100312;
		rank = 0;
	}
};

/*
同步逻辑学习技能扣除物品
*/
struct MSG_20100016 : public Base_Msg  {
	int32_t skill_id;
	int32_t skill_lv;
	MSG_20100016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int32(skill_lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int32(skill_lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20100016;
		skill_id = 0;
		skill_lv = 0;
	}
};

/*
异常操作被动技能调用主动技能
*/
struct MSG_20100101 : public Base_Msg  {
	MSG_20100101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100101;
	}
};

/*
同步性别到地图
*/
struct MSG_20100029 : public Base_Msg  {
	MSG_20100029(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100029;
	}
};

/*
同步精力值到逻辑
*/
struct MSG_20100031 : public Base_Msg  {
	MSG_20100031(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100031;
	}
};

/*
同步生活技能收集技能到场景
*/
struct MSG_20100032 : public Base_Msg  {
	MSG_20100032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20100032;
	}
};

/*
同步公共信息
*/
struct MSG_20020001 : public Base_Msg  {
	MSG_20020001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20020001;
	}
};

/*
内部同步关卡信息
*/
struct MSG_20300400 : public Base_Msg  {
	MSG_20300400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 20300400;
	}
};

/*
通知逻辑服领取篝火奖励
备注：通知逻辑服领取篝火奖励（点燃、添柴、结算）
*/
struct MSG_20300113 : public Base_Msg  {
	std::vector<Item_Info> items;
	int8_t type;
	MSG_20300113(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
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

		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300113;
		items.clear();
		type = 0;
	}
};

/*
通知逻辑服领取篝火奖励(返回)
备注：通知逻辑服领取篝火奖励的返回
*/
struct MSG_20300114 : public Base_Msg  {
	int8_t result;
	int8_t type;
	MSG_20300114(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300114;
		result = 0;
		type = 0;
	}
};

/*
请求读取玩家篝火信息
备注：请求读取玩家篝火信息
*/
struct MSG_20300115 : public Base_Msg  {
	int32_t role_id;
	MSG_20300115(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300115;
		role_id = 0;
	}
};

/*
请求同步玩家篝火信息(返回)
备注：请求同步玩家篝火信息(返回)
*/
struct MSG_20300116 : public Base_Msg  {
	int64_t role_id;
	int32_t finish_time;
	int32_t rank;
	std::vector<int32_t> killed_match_man;
	int8_t is_lighter;
	int32_t recieve_stat;
	int32_t rank_rev_stat;
	int32_t add_firewood;
	MSG_20300116(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(finish_time);
		w.write_int32(rank);
		uint16_t __killed_match_man_vec_size = killed_match_man.size();
		w.write_uint16(__killed_match_man_vec_size);
		for(uint16_t i = 0; i < __killed_match_man_vec_size; ++i) {
				w.write_int32(killed_match_man[i]);
		}

		w.write_int8(is_lighter);
		w.write_int32(recieve_stat);
		w.write_int32(rank_rev_stat);
		w.write_int32(add_firewood);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(finish_time)  ||  r.read_int32(rank)  )
				return -1;
 		uint16_t __killed_match_man_vec_size;
		if(r.read_uint16(__killed_match_man_vec_size)  )
				return -1;
		killed_match_man.reserve(__killed_match_man_vec_size);
		for(uint16_t i = 0; i < __killed_match_man_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				killed_match_man.push_back(v);
		}

		if( r.read_int8(is_lighter)  ||  r.read_int32(recieve_stat)  ||  r.read_int32(rank_rev_stat)  ||  r.read_int32(add_firewood)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 20300116;
		role_id = 0;
		finish_time = 0;
		rank = 0;
		killed_match_man.clear();
		is_lighter = 0;
		recieve_stat = 0;
		rank_rev_stat = 0;
		add_firewood = 0;
	}
};

/*
请求邮件结算玩家篝火奖励
备注：请求邮件结算玩家篝火奖励
*/
struct MSG_20300117 : public Base_Msg  {
	int32_t mail_id;
	std::vector<Item_Basic_Info> items;
	MSG_20300117(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mail_id);
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mail_id)  )
				return -1;
 		uint16_t __items_vec_size;
		if(r.read_uint16(__items_vec_size)  )
				return -1;
		items.reserve(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				Item_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				items.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 20300117;
		mail_id = 0;
		items.clear();
	}
};



#endif