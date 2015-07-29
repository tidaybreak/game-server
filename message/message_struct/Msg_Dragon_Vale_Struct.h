/*
* Generate by devtool
*/

#ifndef _MSG_DRAGON_VALE_H_
#define _MSG_DRAGON_VALE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求建造新的建筑
*/
struct MSG_10180000  {
	uint32_t id;
	uint8_t type;
	uint8_t consume_type;
	MSG_10180000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(type);
		w.write_uint8(consume_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(type)  ||  r.read_uint8(consume_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		type = 0;
		consume_type = 0;
	}
};

/*
请求建造新的建筑(返回)
*/
struct MSG_50180000 : public Base_Msg  {
	uint8_t is_build_gold_mine;
	uint8_t is_build_moon_well;
	uint8_t is_build_totem;
	uint8_t build_queue_num;
	Building_Info building_info;
	MSG_50180000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_build_gold_mine);
		w.write_uint8(is_build_moon_well);
		w.write_uint8(is_build_totem);
		w.write_uint8(build_queue_num);
		building_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_build_gold_mine)  ||  r.read_uint8(is_build_moon_well)  ||  r.read_uint8(is_build_totem)  ||  r.read_uint8(build_queue_num)  ||  building_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180000;
		is_build_gold_mine = 0;
		is_build_moon_well = 0;
		is_build_totem = 0;
		build_queue_num = 0;
		building_info.reset();
	}
};

/*
请求升级建筑
*/
struct MSG_10180001  {
	uint32_t id;
	uint8_t type;
	MSG_10180001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		type = 0;
	}
};

/*
请求升级建筑（返回）
*/
struct MSG_50180001 : public Base_Msg  {
	uint8_t build_queue_num;
	Building_Info building_info;
	MSG_50180001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(build_queue_num);
		building_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(build_queue_num)  ||  building_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180001;
		build_queue_num = 0;
		building_info.reset();
	}
};

/*
请求加速建造或者升级建筑
备注： 如果不能加速或者加速失败，返回80000001，带有错误码 。
          如果加速成功，返回  82300002 。
*/
struct MSG_10180002  {
	uint32_t id;
	uint8_t type;
	MSG_10180002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		type = 0;
	}
};

/*
请求取消建造或者升级建筑
*/
struct MSG_10180003  {
	uint32_t id;
	uint8_t type;
	MSG_10180003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		type = 0;
	}
};

/*
请求取消建造或者升级建筑(返回)
备注：building_info:已取消建造或者升级的建筑信息.
                              如果 building_info.level = 0, 就是取消建造的建筑，客户端需要删除这个建筑。
                              如果 building_info.level > 0, 就是取消升级，就用这个building_info对象的信息覆盖客户端保存的对象信息。
*/
struct MSG_50180003 : public Base_Msg  {
	uint8_t is_build_gold_mine;
	uint8_t is_build_moon_well;
	uint8_t is_build_totem;
	uint8_t build_queue_num;
	Building_Info building_info;
	MSG_50180003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_build_gold_mine);
		w.write_uint8(is_build_moon_well);
		w.write_uint8(is_build_totem);
		w.write_uint8(build_queue_num);
		building_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_build_gold_mine)  ||  r.read_uint8(is_build_moon_well)  ||  r.read_uint8(is_build_totem)  ||  r.read_uint8(build_queue_num)  ||  building_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180003;
		is_build_gold_mine = 0;
		is_build_moon_well = 0;
		is_build_totem = 0;
		build_queue_num = 0;
		building_info.reset();
	}
};

/*
请求收获资源
*/
struct MSG_10180004  {
	uint32_t id;
	uint8_t type;
	MSG_10180004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		type = 0;
	}
};

/*
请求收获资源(返回)
*/
struct MSG_50180004 : public Base_Msg  {
	uint32_t gain_production;
	Building_Info building_info;
	MSG_50180004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(gain_production);
		building_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(gain_production)  ||  building_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180004;
		gain_production = 0;
		building_info.reset();
	}
};

/*
请求踩脚印
备注：如果成功，返回50180005 。如果失败，返回 错误码。
*/
struct MSG_10180005  {
	int64_t role_id;
	MSG_10180005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
请求踩脚印(返回)
*/
struct MSG_50180005 : public Base_Msg  {
	uint8_t is_up_star;
	uint32_t dragon_heart;
	uint32_t dragon_soul_award;
	Dragon_Vale_Head_Info head_info;
	MSG_50180005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_up_star);
		w.write_uint32(dragon_heart);
		w.write_uint32(dragon_soul_award);
		head_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_up_star)  ||  r.read_uint32(dragon_heart)  ||  r.read_uint32(dragon_soul_award)  ||  head_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180005;
		is_up_star = 0;
		dragon_heart = 0;
		dragon_soul_award = 0;
		head_info.reset();
	}
};

/*
请求增加龙谷建筑队列
备注：如果成功，返回 50180006。 如果失败，返回错误码。
*/
struct MSG_10180006  {
	MSG_10180006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求增加建筑队列(返回)
*/
struct MSG_50180006 : public Base_Msg  {
	uint8_t builing_queue_num;
	uint8_t all_open_build_queue_num;
	MSG_50180006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(builing_queue_num);
		w.write_uint8(all_open_build_queue_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(builing_queue_num)  ||  r.read_uint8(all_open_build_queue_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180006;
		builing_queue_num = 0;
		all_open_build_queue_num = 0;
	}
};

/*
请求获取有龙谷的好友列表
*/
struct MSG_10180007  {
	MSG_10180007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求获取有龙谷的同一个公会成员列表
*/
struct MSG_10180008  {
	MSG_10180008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求获取有龙谷的好友列表(返回)
*/
struct MSG_50180007 : public Base_Msg  {
	std::vector<Dragon_Vale_Member> friend_list;
	MSG_50180007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __friend_list_vec_size = friend_list.size();
		w.write_uint16(__friend_list_vec_size);
		for(uint16_t i = 0; i < __friend_list_vec_size; ++i) {
				friend_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __friend_list_vec_size;
		if(r.read_uint16(__friend_list_vec_size)  )
				return -1;
		friend_list.reserve(__friend_list_vec_size);
		for(uint16_t i = 0; i < __friend_list_vec_size; ++i) {
				Dragon_Vale_Member v;
				if(v.deserialize(r))
						return -1;
				friend_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50180007;
		friend_list.clear();
	}
};

/*
请求获取有龙谷的同一个公会成员列表(返回)
*/
struct MSG_50180008 : public Base_Msg  {
	std::vector<Dragon_Vale_Member> gang_memeber_list;
	MSG_50180008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __gang_memeber_list_vec_size = gang_memeber_list.size();
		w.write_uint16(__gang_memeber_list_vec_size);
		for(uint16_t i = 0; i < __gang_memeber_list_vec_size; ++i) {
				gang_memeber_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __gang_memeber_list_vec_size;
		if(r.read_uint16(__gang_memeber_list_vec_size)  )
				return -1;
		gang_memeber_list.reserve(__gang_memeber_list_vec_size);
		for(uint16_t i = 0; i < __gang_memeber_list_vec_size; ++i) {
				Dragon_Vale_Member v;
				if(v.deserialize(r))
						return -1;
				gang_memeber_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50180008;
		gang_memeber_list.clear();
	}
};

/*
请求孵化龙蛋
*/
struct MSG_10180009  {
	MSG_10180009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求孵化龙蛋(返回)
*/
struct MSG_50180009 : public Base_Msg  {
	uint32_t born_time;
	MSG_50180009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(born_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(born_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180009;
		born_time = 0;
	}
};

/*
请求逗龙
*/
struct MSG_10180010  {
	int64_t role_id;
	MSG_10180010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
请求逗龙(返回)
*/
struct MSG_50180010 : public Base_Msg  {
	int64_t role_id;
	uint8_t feeling;
	uint32_t reward;
	uint8_t is_play_dragon;
	uint32_t item_id;
	uint16_t item_num;
	MSG_50180010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(feeling);
		w.write_uint32(reward);
		w.write_uint8(is_play_dragon);
		w.write_uint32(item_id);
		w.write_uint16(item_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(feeling)  ||  r.read_uint32(reward)  ||  r.read_uint8(is_play_dragon)  ||  r.read_uint32(item_id)  ||  r.read_uint16(item_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180010;
		role_id = 0;
		feeling = 0;
		reward = 0;
		is_play_dragon = 0;
		item_id = 0;
		item_num = 0;
	}
};

/*
请求获取龙的总信息(培养、进化左侧面板)
*/
struct MSG_10180011  {
	int64_t role_id;
	MSG_10180011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
请求获取龙的总信息(返回)
*/
struct MSG_50180011 : public Base_Msg  {
	int64_t role_id;
	Dragon_Info dragon_info;
	uint32_t left_time;
	MSG_50180011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		dragon_info.serialize(w);
		w.write_uint32(left_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  dragon_info.deserialize(r)  ||  r.read_uint32(left_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180011;
		role_id = 0;
		dragon_info.reset();
		left_time = 0;
	}
};

/*
请求培养龙
备注：如果货币不足，返回错误码。 如果货币足够并且扣除成功，不管培养成功还是失败，都返回50180012。
*/
struct MSG_10180012  {
	uint8_t type;
	MSG_10180012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
请求培养龙(返回)
*/
struct MSG_50180012 : public Base_Msg  {
	uint8_t type;
	uint8_t is_success;
	Dragon_Info dragon_info;
	MSG_50180012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint8(is_success);
		dragon_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint8(is_success)  ||  dragon_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180012;
		type = 0;
		is_success = 0;
		dragon_info.reset();
	}
};

/*
请求增加1点龙心情值
备注：如果成功返回50180013， 如果货币不足，返回错误码。
*/
struct MSG_10180013  {
	MSG_10180013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求增加1点龙心情值(返回)
*/
struct MSG_50180013 : public Base_Msg  {
	uint8_t feeling;
	MSG_50180013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(feeling);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(feeling)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180013;
		feeling = 0;
	}
};

/*
请求进化龙
备注：如果type=1, 并且， 道具不足，返回82300006，通知客户端进化龙所需道具不足，是否愿意消耗货币。
         如果扣除道具或者货币成功， 返回50180014 。
*/
struct MSG_10180014  {
	uint8_t type;
	MSG_10180014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
请求进化龙(返回)
*/
struct MSG_50180014 : public Base_Msg  {
	uint8_t is_ten_times;
	uint32_t energy;
	uint8_t is_new_dragon;
	Dragon_Info dragon_info;
	MSG_50180014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_ten_times);
		w.write_uint32(energy);
		w.write_uint8(is_new_dragon);
		dragon_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_ten_times)  ||  r.read_uint32(energy)  ||  r.read_uint8(is_new_dragon)  ||  dragon_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180014;
		is_ten_times = 0;
		energy = 0;
		is_new_dragon = 0;
		dragon_info.reset();
	}
};

/*
请求掠夺别人龙谷
备注： 点击  好友列表下面的掠夺按钮，发送这条消息
*/
struct MSG_10180015  {
	MSG_10180015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求掠夺别人龙谷（返回）
*/
struct MSG_50180015 : public Base_Msg  {
	uint32_t coin;
	uint8_t is_rob_protect;
	MSG_50180015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(coin);
		w.write_uint8(is_rob_protect);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(coin)  ||  r.read_uint8(is_rob_protect)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180015;
		coin = 0;
		is_rob_protect = 0;
	}
};

/*
请求掠夺匹配
*/
struct MSG_10180016  {
	MSG_10180016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求掠夺别人龙谷匹配(返回)
*/
struct MSG_50180016 : public Base_Msg  {
	int64_t to_uuid;
	std::string to_server_name;
	int64_t uuid;
	std::string server_name;
	int64_t role_id;
	std::vector<Rob_Match_Member> rob_match_member_list;
	uint32_t fight;
	uint32_t rob_gold_mine;
	uint32_t rob_moon_weel;
	uint32_t rob_totem;
	uint32_t coin;
	MSG_50180016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_string(to_server_name);
		w.write_int64(uuid);
		w.write_string(server_name);
		w.write_int64(role_id);
		uint16_t __rob_match_member_list_vec_size = rob_match_member_list.size();
		w.write_uint16(__rob_match_member_list_vec_size);
		for(uint16_t i = 0; i < __rob_match_member_list_vec_size; ++i) {
				rob_match_member_list[i].serialize(w);
		}

		w.write_uint32(fight);
		w.write_uint32(rob_gold_mine);
		w.write_uint32(rob_moon_weel);
		w.write_uint32(rob_totem);
		w.write_uint32(coin);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_string(to_server_name)  ||  r.read_int64(uuid)  ||  r.read_string(server_name)  ||  r.read_int64(role_id)  )
				return -1;
 		uint16_t __rob_match_member_list_vec_size;
		if(r.read_uint16(__rob_match_member_list_vec_size)  )
				return -1;
		rob_match_member_list.reserve(__rob_match_member_list_vec_size);
		for(uint16_t i = 0; i < __rob_match_member_list_vec_size; ++i) {
				Rob_Match_Member v;
				if(v.deserialize(r))
						return -1;
				rob_match_member_list.push_back(v);
		}

		if( r.read_uint32(fight)  ||  r.read_uint32(rob_gold_mine)  ||  r.read_uint32(rob_moon_weel)  ||  r.read_uint32(rob_totem)  ||  r.read_uint32(coin)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180016;
		to_uuid = 0;
		to_server_name.clear();
		uuid = 0;
		server_name.clear();
		role_id = 0;
		rob_match_member_list.clear();
		fight = 0;
		rob_gold_mine = 0;
		rob_moon_weel = 0;
		rob_totem = 0;
		coin = 0;
	}
};

/*
请求进攻掠夺龙谷匹配到的人(可能在线、也可能不在线)
备注：请求进攻掠夺龙谷龙谷匹配到的人(可能在线、也可能不在线)
*/
struct MSG_10200152  {
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
	int64_t to_uuid;
	std::string to_server_name;
	uint32_t coin;
	MSG_10200152(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_string(server_name);
		w.write_string(role_name);
		w.write_int64(to_uuid);
		w.write_string(to_server_name);
		w.write_uint32(coin);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_string(server_name)  ||  r.read_string(role_name)  ||  r.read_int64(to_uuid)  ||  r.read_string(to_server_name)  ||  r.read_uint32(coin)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		uuid = 0;
		role_id = 0;
		server_name.clear();
		role_name.clear();
		to_uuid = 0;
		to_server_name.clear();
		coin = 0;
	}
};

/*
龙谷主人同意应战来掠夺的人
*/
struct MSG_10200153  {
	int64_t uuid;
	int64_t role_id;
	MSG_10200153(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(uuid);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(uuid)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		uuid = 0;
		role_id = 0;
	}
};

/*
请求获取布告栏
*/
struct MSG_10180017  {
	MSG_10180017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求获取布告栏(返回)
备注：
       类型: 1：拜访  2:驱逐  3：沙鼠偷猎者出现  4:掠夺  5:逗龙
               每个布告栏最多50条，超过50条，时间最早的删除。
*/
struct MSG_50180017 : public Base_Msg  {
	std::vector<Dragon_Vale_Notice> notice_list;
	MSG_50180017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __notice_list_vec_size = notice_list.size();
		w.write_uint16(__notice_list_vec_size);
		for(uint16_t i = 0; i < __notice_list_vec_size; ++i) {
				notice_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __notice_list_vec_size;
		if(r.read_uint16(__notice_list_vec_size)  )
				return -1;
		notice_list.reserve(__notice_list_vec_size);
		for(uint16_t i = 0; i < __notice_list_vec_size; ++i) {
				Dragon_Vale_Notice v;
				if(v.deserialize(r))
						return -1;
				notice_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50180017;
		notice_list.clear();
	}
};

/*
请求收获掠夺资源
备注：如果成功，返回50180018； 如果失败，返回错误码。
*/
struct MSG_10180018  {
	int64_t to_uuid;
	int64_t to_role_id;
	int64_t uuid;
	int64_t role_id;
	uint32_t building_id;
	uint8_t building_type;
	MSG_10180018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_uint32(building_id);
		w.write_uint8(building_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_uint32(building_id)  ||  r.read_uint8(building_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		to_uuid = 0;
		to_role_id = 0;
		uuid = 0;
		role_id = 0;
		building_id = 0;
		building_type = 0;
	}
};

/*
请求收获掠夺资源(返回)
*/
struct MSG_50180018 : public Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
	int64_t uuid;
	int64_t role_id;
	uint32_t gain_production;
	Building_Info building_info;
	MSG_50180018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(to_uuid);
		w.write_int64(to_role_id);
		w.write_int64(uuid);
		w.write_int64(role_id);
		w.write_uint32(gain_production);
		building_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(to_uuid)  ||  r.read_int64(to_role_id)  ||  r.read_int64(uuid)  ||  r.read_int64(role_id)  ||  r.read_uint32(gain_production)  ||  building_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180018;
		to_uuid = 0;
		to_role_id = 0;
		uuid = 0;
		role_id = 0;
		gain_production = 0;
		building_info.reset();
	}
};

/*
请求驱逐沙鼠偷猎者
*/
struct MSG_10200154  {
	int64_t role_id;
	int64_t monster_id;
	uint8_t quality;
	MSG_10200154(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int64(monster_id);
		w.write_uint8(quality);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int64(monster_id)  ||  r.read_uint8(quality)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		monster_id = 0;
		quality = 0;
	}
};

/*
请求驱逐沙鼠偷猎者(返回)
*/
struct MSG_50200154 : public Base_Msg  {
	uint8_t quality;
	uint32_t award;
	std::string name;
	MSG_50200154(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(quality);
		w.write_uint32(award);
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(quality)  ||  r.read_uint32(award)  ||  r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200154;
		quality = 0;
		award = 0;
		name.clear();
	}
};

/*
掠夺战斗胜利，但没有掠夺资源，通知逻辑服任务监听
*/
struct MSG_10180019  {
	MSG_10180019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求批量进化龙
备注：如果type=0,    道具完全足、道具有一部分，但是不足并且货币足、没有道具并且货币足够， 返回82300034，通知客户端批量进化龙， 是否愿意消耗x个道具和x货币。       这种情况下，不返回50180020。
        如果道具、货币都不足，返回错误码。
        如果type=1、2、3，并且足够，返回50180020。
        只有type = 1、2、3，才发 batch_num、ten_times。
*/
struct MSG_10180020  {
	uint8_t type;
	uint16_t batch_num;
	uint16_t ten_times;
	MSG_10180020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint16(batch_num);
		w.write_uint16(ten_times);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint16(batch_num)  ||  r.read_uint16(ten_times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		batch_num = 0;
		ten_times = 0;
	}
};

/*
请求批量进化龙(返回)
*/
struct MSG_50180020 : public Base_Msg  {
	uint16_t batch_num;
	uint16_t ten_times;
	uint32_t get_energy;
	uint32_t energy;
	uint8_t is_new_dragon;
	Dragon_Info dragon_info;
	MSG_50180020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(batch_num);
		w.write_uint16(ten_times);
		w.write_uint32(get_energy);
		w.write_uint32(energy);
		w.write_uint8(is_new_dragon);
		dragon_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(batch_num)  ||  r.read_uint16(ten_times)  ||  r.read_uint32(get_energy)  ||  r.read_uint32(energy)  ||  r.read_uint8(is_new_dragon)  ||  dragon_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180020;
		batch_num = 0;
		ten_times = 0;
		get_energy = 0;
		energy = 0;
		is_new_dragon = 0;
		dragon_info.reset();
	}
};

/*
请求获取建筑可收获剩余时间
*/
struct MSG_10180021  {
	uint32_t id;
	uint8_t type;
	MSG_10180021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		type = 0;
	}
};

/*
请求获取建筑可收获剩余时间（返回）
*/
struct MSG_50180021 : public Base_Msg  {
	uint32_t id;
	uint8_t type;
	uint32_t left_time;
	MSG_50180021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(type);
		w.write_uint32(left_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(type)  ||  r.read_uint32(left_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180021;
		id = 0;
		type = 0;
		left_time = 0;
	}
};

/*
获取邀请好友踩脚印的好友列表
*/
struct MSG_10180022  {
	MSG_10180022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取邀请好友踩脚印的好友列表(返回)
*/
struct MSG_50180022 : public Base_Msg  {
	std::vector<Dragon_Vale_Invite_Step_Foot_Friend> friend_list;
	MSG_50180022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __friend_list_vec_size = friend_list.size();
		w.write_uint16(__friend_list_vec_size);
		for(uint16_t i = 0; i < __friend_list_vec_size; ++i) {
				friend_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __friend_list_vec_size;
		if(r.read_uint16(__friend_list_vec_size)  )
				return -1;
		friend_list.reserve(__friend_list_vec_size);
		for(uint16_t i = 0; i < __friend_list_vec_size; ++i) {
				Dragon_Vale_Invite_Step_Foot_Friend v;
				if(v.deserialize(r))
						return -1;
				friend_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50180022;
		friend_list.clear();
	}
};

/*
 邀请好友踩脚印
*/
struct MSG_10180023  {
	int64_t role_id;
	MSG_10180023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
邀请全部好友踩脚印
*/
struct MSG_10180024  {
	MSG_10180024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
邀请好友踩脚印(返回)
*/
struct MSG_50180023 : public Base_Msg  {
	uint32_t error_code;
	MSG_50180023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(error_code);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(error_code)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50180023;
		error_code = 0;
	}
};

/*
邀请全部好友踩脚印(返回)
*/
struct MSG_50180024 : public Base_Msg  {
	MSG_50180024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50180024;
	}
};



#endif