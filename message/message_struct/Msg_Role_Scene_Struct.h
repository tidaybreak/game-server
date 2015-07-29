/*
* Generate by devtool
*/

#ifndef _MSG_ROLE_SCENE_H_
#define _MSG_ROLE_SCENE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
登录游戏
备注：登录成功时返回消息81000000
*/
struct MSG_10100001  {
	std::string account;
	int32_t agent_num;
	int32_t server_num;
	std::string session_id;
	std::string timestamp;
	int32_t fcm;
	MSG_10100001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(account);
		w.write_int32(agent_num);
		w.write_int32(server_num);
		w.write_string(session_id);
		w.write_string(timestamp);
		w.write_int32(fcm);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(account)  ||  r.read_int32(agent_num)  ||  r.read_int32(server_num)  ||  r.read_string(session_id)  ||  r.read_string(timestamp)  ||  r.read_int32(fcm)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		account.clear();
		agent_num = 0;
		server_num = 0;
		session_id.clear();
		timestamp.clear();
		fcm = 0;
	}
};

/*
登录游戏(返回)
*/
struct MSG_50100001 : public Base_Msg  {
	MSG_50100001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100001;
	}
};

/*
创建角色
备注：创建成功时返回消息81000000
*/
struct MSG_10100005  {
	std::string account;
	int32_t agent_num;
	int32_t server_num;
	std::string role_name;
	int8_t gender;
	int8_t career;
	int8_t fcm;
	int32_t country;
	std::string session_id;
	std::string timestamp;
	std::string invite_user;
	MSG_10100005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(account);
		w.write_int32(agent_num);
		w.write_int32(server_num);
		w.write_string(role_name);
		w.write_int8(gender);
		w.write_int8(career);
		w.write_int8(fcm);
		w.write_int32(country);
		w.write_string(session_id);
		w.write_string(timestamp);
		w.write_string(invite_user);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(account)  ||  r.read_int32(agent_num)  ||  r.read_int32(server_num)  ||  r.read_string(role_name)  ||  r.read_int8(gender)  ||  r.read_int8(career)  ||  r.read_int8(fcm)  ||  r.read_int32(country)  ||  r.read_string(session_id)  ||  r.read_string(timestamp)  ||  r.read_string(invite_user)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		account.clear();
		agent_num = 0;
		server_num = 0;
		role_name.clear();
		gender = 0;
		career = 0;
		fcm = 0;
		country = 0;
		session_id.clear();
		timestamp.clear();
		invite_user.clear();
	}
};

/*
创建角色(返回)
*/
struct MSG_50100005 : public Base_Msg  {
	int64_t role_id;
	MSG_50100005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100005;
		role_id = 0;
	}
};

/*
获取角色
*/
struct MSG_10100000  {
	std::string account;
	int32_t agent_num;
	int32_t server_num;
	MSG_10100000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(account);
		w.write_int32(agent_num);
		w.write_int32(server_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(account)  ||  r.read_int32(agent_num)  ||  r.read_int32(server_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		account.clear();
		agent_num = 0;
		server_num = 0;
	}
};

/*
获取角色(返回)
*/
struct MSG_50100000 : public Base_Msg  {
	std::vector<Uid_Name> role_list;
	int8_t career;
	int8_t gender;
	int8_t timezone;
	std::string center_url;
	MSG_50100000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_list_vec_size = role_list.size();
		w.write_uint16(__role_list_vec_size);
		for(uint16_t i = 0; i < __role_list_vec_size; ++i) {
				role_list[i].serialize(w);
		}

		w.write_int8(career);
		w.write_int8(gender);
		w.write_int8(timezone);
		w.write_string(center_url);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_list_vec_size;
		if(r.read_uint16(__role_list_vec_size)  )
				return -1;
		role_list.reserve(__role_list_vec_size);
		for(uint16_t i = 0; i < __role_list_vec_size; ++i) {
				Uid_Name v;
				if(v.deserialize(r))
						return -1;
				role_list.push_back(v);
		}

		if( r.read_int8(career)  ||  r.read_int8(gender)  ||  r.read_int8(timezone)  ||  r.read_string(center_url)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100000;
		role_list.clear();
		career = 0;
		gender = 0;
		timezone = 0;
		center_url.clear();
	}
};

/*
登录网关
*/
struct MSG_10400001  {
	std::string session_id;
	int64_t role_id;
	MSG_10400001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(session_id);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(session_id)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		session_id.clear();
		role_id = 0;
	}
};

/*
登录网关(返回)
*/
struct MSG_50400001 : public Base_Msg  {
	int64_t role_id;
	Location location;
	int32_t time;
	MSG_50400001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		location.serialize(w);
		w.write_int32(time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  location.deserialize(r)  ||  r.read_int32(time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50400001;
		role_id = 0;
		location.reset();
		time = 0;
	}
};

/*
切换场景(返回)
*/
struct MSG_50400005 : public Base_Msg  {
	Location local;
	int32_t result;
	MSG_50400005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		local.serialize(w);
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( local.deserialize(r)  ||  r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50400005;
		local.reset();
		result = 0;
	}
};

/*
切换场景(场景改变都用它)
备注：（1）转换本服龙谷场景:  
                 scne_id:龙谷场景id
                 role_id:对方的角色id
                 sort、sub_sort 为0
          (2)  掠夺者在掠夺战斗中胜利，转换进入跨服龙谷场景:
                 scne_id:龙谷场景id
                 role_id:对方的角色id
                 sort:是否是掠夺龙谷     1：是，0:否
                 sub_sort: 跨服标识
          (3)  掠夺者在掠夺战斗中胜利，进入被掠夺者的跨服龙谷场景内，1分钟倒计时结束，自动回到自己龙谷。
                 scne_id:龙谷场景id
                 role_id:掠夺者的角色id
                 sort: 0
                 sub_sort:0
           
         (4)限时击杀任务，切换场景
              scene_id:限时击杀任务场景
              role_id:角色id
              sort:任务id
              sub_sort:0
*/
struct MSG_10400005  {
	int32_t scene_id;
	int32_t scene_order;
	int32_t sort;
	int32_t sub_sort;
	int64_t role_id;
	std::string name;
	MSG_10400005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
		w.write_int32(scene_order);
		w.write_int32(sort);
		w.write_int32(sub_sort);
		w.write_int64(role_id);
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  ||  r.read_int32(scene_order)  ||  r.read_int32(sort)  ||  r.read_int32(sub_sort)  ||  r.read_int64(role_id)  ||  r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		scene_id = 0;
		scene_order = 0;
		sort = 0;
		sub_sort = 0;
		role_id = 0;
		name.clear();
	}
};

/*
打开人物面板
*/
struct MSG_10200032  {
	int64_t role_id;
	int8_t open_type;
	MSG_10200032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(open_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(open_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		open_type = 0;
	}
};

/*
打开人物面板(返回)
*/
struct MSG_50200032 : public Base_Msg  {
	int64_t role_id;
	int8_t open_type;
	std::vector<Property> prop_value;
	std::string role_name;
	std::string gang_name;
	uint8_t gender;
	int64_t gang_id;
	int8_t headship;
	std::vector<int32_t> avatar_vec;
	MSG_50200032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(open_type);
		uint16_t __prop_value_vec_size = prop_value.size();
		w.write_uint16(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				prop_value[i].serialize(w);
		}

		w.write_string(role_name);
		w.write_string(gang_name);
		w.write_uint8(gender);
		w.write_int64(gang_id);
		w.write_int8(headship);
		uint16_t __avatar_vec_vec_size = avatar_vec.size();
		w.write_uint16(__avatar_vec_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_vec_size; ++i) {
				w.write_int32(avatar_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(open_type)  )
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

		if( r.read_string(role_name)  ||  r.read_string(gang_name)  ||  r.read_uint8(gender)  ||  r.read_int64(gang_id)  ||  r.read_int8(headship)  )
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

		return 0;
	}

	void reset(void) {
		msg_id = 50200032;
		role_id = 0;
		open_type = 0;
		prop_value.clear();
		role_name.clear();
		gang_name.clear();
		gender = 0;
		gang_id = 0;
		headship = 0;
		avatar_vec.clear();
	}
};

/*
设置剧情信息
*/
struct MSG_10100010  {
	std::string plot_id;
	int32_t plot_step;
	MSG_10100010(void) { reset(); };

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
		plot_id.clear();
		plot_step = 0;
	}
};

/*
请求瞬移(坐电梯)
*/
struct MSG_10200007  {
	Coord coord;
	MSG_10200007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		coord.reset();
	}
};

/*
请求飞行(坐电梯)
*/
struct MSG_10200005  {
	Coord coord;
	MSG_10200005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		coord.reset();
	}
};

/*
客户端初始化完成
*/
struct MSG_10100015  {
	MSG_10100015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
移动场景击退请求
*/
struct MSG_10200008  {
	int16_t pos_x;
	int16_t pos_y;
	MSG_10200008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(pos_x);
		w.write_int16(pos_y);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(pos_x)  ||  r.read_int16(pos_y)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pos_x = 0;
		pos_y = 0;
	}
};

/*
通知防沉迷验证通过
*/
struct MSG_10100090  {
	int32_t fcm;
	MSG_10100090(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(fcm);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(fcm)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		fcm = 0;
	}
};

/*
从逻辑服获取角色信息
*/
struct MSG_10100006  {
	int64_t role_id;
	MSG_10100006(void) { reset(); };

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
获取角色信息(返回)
*/
struct MSG_50100006 : public Base_Msg  {
	Role_Info role_info;
	MSG_50100006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		role_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( role_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100006;
		role_info.reset();
	}
};

/*
电梯到达请求
*/
struct MSG_10200009  {
	int64_t lift_id;
	MSG_10200009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(lift_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(lift_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		lift_id = 0;
	}
};

/*
获取角色随机名字
*/
struct MSG_10100007  {
	int8_t gender;
	MSG_10100007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(gender);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(gender)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gender = 0;
	}
};

/*
获取角色随机名字（返回）
*/
struct MSG_50100007 : public Base_Msg  {
	std::string name;
	MSG_50100007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100007;
		name.clear();
	}
};

/*
客户端初始化完成(返回)
*/
struct MSG_50100015 : public Base_Msg  {
	Role_Base_Info role_base_info;
	std::vector<Property> property;
	uint8_t role_type;
	MSG_50100015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		role_base_info.serialize(w);
		uint16_t __property_vec_size = property.size();
		w.write_uint16(__property_vec_size);
		for(uint16_t i = 0; i < __property_vec_size; ++i) {
				property[i].serialize(w);
		}

		w.write_uint8(role_type);
	}

	int deserialize(Block_Buffer & r) {
		if( role_base_info.deserialize(r)  )
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

		if( r.read_uint8(role_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100015;
		role_base_info.reset();
		property.clear();
		role_type = 0;
	}
};

/*
访问数据统计
备注：
创号页面loading: 1
最早的三句话的剧情:2
创号界面: 3
在主角喊琳娜小心:4
第一次使用技能 : 5
第二场战斗前，说到有埋伏的剧情: 6
战斗结束后BOSS变身: 7
琳娜喊“救我”: 8
第一章开启:9
*/
struct MSG_10188888  {
	uint32_t type;
	MSG_10188888(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
从场景服获取人物信息
备注：返回 81000100
*/
struct MSG_10200006  {
	MSG_10200006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
场景加载完成
备注：10200030--场景加载完成
*/
struct MSG_10200030  {
	MSG_10200030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
场景加载完成(返回)
备注：10200030--场景加载完成
*/
struct MSG_50200030 : public Base_Msg  {
	MSG_50200030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50200030;
	}
};

/*
请求改名
*/
struct MSG_10100008  {
	std::string new_name;
	MSG_10100008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(new_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(new_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		new_name.clear();
	}
};

/*
请求改名（返回）
*/
struct MSG_50100008 : public Base_Msg  {
	int8_t result;
	std::string new_name;
	MSG_50100008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_string(new_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_string(new_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100008;
		result = 0;
		new_name.clear();
	}
};

/*
打开人物面板_逻辑服
*/
struct MSG_10100032  {
	int64_t role_id;
	int8_t open_type;
	MSG_10100032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(open_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(open_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		open_type = 0;
	}
};

/*
打开人物面板_逻辑服(返回)
*/
struct MSG_50100032 : public Base_Msg  {
	int64_t role_id;
	int8_t open_type;
	std::vector<Property> prop_value;
	std::string role_name;
	uint8_t gender;
	int64_t gang_id;
	std::string gang_name;
	int8_t headship;
	int8_t vip;
	std::vector<int32_t> avatar_vec;
	MSG_50100032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(open_type);
		uint16_t __prop_value_vec_size = prop_value.size();
		w.write_uint16(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				prop_value[i].serialize(w);
		}

		w.write_string(role_name);
		w.write_uint8(gender);
		w.write_int64(gang_id);
		w.write_string(gang_name);
		w.write_int8(headship);
		w.write_int8(vip);
		uint16_t __avatar_vec_vec_size = avatar_vec.size();
		w.write_uint16(__avatar_vec_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_vec_size; ++i) {
				w.write_int32(avatar_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(open_type)  )
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

		if( r.read_string(role_name)  ||  r.read_uint8(gender)  ||  r.read_int64(gang_id)  ||  r.read_string(gang_name)  ||  r.read_int8(headship)  ||  r.read_int8(vip)  )
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

		return 0;
	}

	void reset(void) {
		msg_id = 50100032;
		role_id = 0;
		open_type = 0;
		prop_value.clear();
		role_name.clear();
		gender = 0;
		gang_id = 0;
		gang_name.clear();
		headship = 0;
		vip = 0;
		avatar_vec.clear();
	}
};

/*
请求进入活动场景
*/
struct MSG_10100040  {
	int32_t active_id;
	MSG_10100040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(active_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(active_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		active_id = 0;
	}
};



#endif