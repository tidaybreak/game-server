/*
* Generate by devtool
*/

#ifndef _MSG_TEAMER_H_
#define _MSG_TEAMER_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求大厅信息
主动广播：
80100300
80100301

*/
struct MSG_10100300  {
	int8_t type;
	MSG_10100300(void) { reset(); };

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
请求大厅信息(返回)
*/
struct MSG_50100300 : public Base_Msg  {
	MSG_50100300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100300;
	}
};

/*
创建房间
*/
struct MSG_10100301  {
	int32_t scene_id;
	std::string password;
	int8_t type;
	int8_t degree;
	MSG_10100301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
		w.write_string(password);
		w.write_int8(type);
		w.write_int8(degree);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  ||  r.read_string(password)  ||  r.read_int8(type)  ||  r.read_int8(degree)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		scene_id = 0;
		password.clear();
		type = 0;
		degree = 0;
	}
};

/*
创建房间(返回)
*/
struct MSG_50100301 : public Base_Msg  {
	int32_t index;
	int32_t scene_id;
	std::string password;
	MSG_50100301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_int32(scene_id);
		w.write_string(password);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_int32(scene_id)  ||  r.read_string(password)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100301;
		index = 0;
		scene_id = 0;
		password.clear();
	}
};

/*
邀请加入房间
*/
struct MSG_10100302  {
	int64_t role_id;
	int8_t type;
	int32_t value;
	std::string monster;
	MSG_10100302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(type);
		w.write_int32(value);
		w.write_string(monster);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(type)  ||  r.read_int32(value)  ||  r.read_string(monster)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		type = 0;
		value = 0;
		monster.clear();
	}
};

/*
邀请加入房间(返回)
*/
struct MSG_50100302 : public Base_Msg  {
	int8_t result;
	MSG_50100302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100302;
		result = 0;
	}
};

/*
申请加入房间
*/
struct MSG_10100303  {
	int64_t role_id;
	int64_t index;
	std::string password;
	int8_t is_world;
	int8_t profile_set;
	MSG_10100303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int64(index);
		w.write_string(password);
		w.write_int8(is_world);
		w.write_int8(profile_set);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int64(index)  ||  r.read_string(password)  ||  r.read_int8(is_world)  ||  r.read_int8(profile_set)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		index = 0;
		password.clear();
		is_world = 0;
		profile_set = 0;
	}
};

/*
申请加入房间(返回)
不返回错误码，用于返回给客户端特殊处理
*/
struct MSG_50100303 : public Base_Msg  {
	MSG_50100303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100303;
	}
};

/*
设为队长
*/
struct MSG_10100304  {
	int64_t role_id;
	MSG_10100304(void) { reset(); };

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
设为队长
*/
struct MSG_50100304 : public Base_Msg  {
	MSG_50100304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100304;
	}
};

/*
踢出队伍
*/
struct MSG_10100305  {
	int64_t role_id;
	MSG_10100305(void) { reset(); };

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
踢出队伍(返回)
*/
struct MSG_50100305 : public Base_Msg  {
	MSG_50100305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100305;
	}
};

/*
离开队伍
*/
struct MSG_10100306  {
	MSG_10100306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
离开队伍(返回)
*/
struct MSG_50100306 : public Base_Msg  {
	MSG_50100306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100306;
	}
};

/*
解散队伍
*/
struct MSG_10100307  {
	MSG_10100307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
解散队伍(返回)
*/
struct MSG_50100307 : public Base_Msg  {
	MSG_50100307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100307;
	}
};

/*
同意组队邀请
*/
struct MSG_10100309  {
	int64_t role_id;
	int8_t profile_set;
	MSG_10100309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(profile_set);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(profile_set)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		profile_set = 0;
	}
};

/*
同意组队邀请(返回)
*/
struct MSG_50100309 : public Base_Msg  {
	int64_t role_id;
	MSG_50100309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100309;
		role_id = 0;
	}
};

/*
请求队伍招人
*/
struct MSG_10100311  {
	int64_t role_id;
	MSG_10100311(void) { reset(); };

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
请求队伍招人(返回)
*/
struct MSG_50100311 : public Base_Msg  {
	MSG_50100311(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100311;
	}
};

/*
玩家进入待战区
*/
struct MSG_10100312  {
	int8_t type;
	MSG_10100312(void) { reset(); };

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
玩家进入待战区(返回)
主动广播：
80100301
*/
struct MSG_50100312 : public Base_Msg  {
	int8_t result;
	int8_t type;
	MSG_50100312(void) { reset(); };

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
		msg_id = 50100312;
		result = 0;
		type = 0;
	}
};

/*
玩家退出待战区(返回)
*/
struct MSG_50100313 : public Base_Msg  {
	int32_t result;
	int8_t type;
	MSG_50100313(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100313;
		result = 0;
		type = 0;
	}
};

/*
玩家退出待战区
*/
struct MSG_10100313  {
	int8_t type;
	MSG_10100313(void) { reset(); };

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
监听组队信息
*/
struct MSG_10100315  {
	int8_t listened;
	int8_t type;
	MSG_10100315(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(listened);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(listened)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		listened = 0;
		type = 0;
	}
};

/*
监听组队信息(返回)
*/
struct MSG_50100315 : public Base_Msg  {
	int8_t type;
	std::vector<Team_Basic_Info> team_info;
	std::vector<Role_Info> player_info;
	MSG_50100315(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		uint16_t __team_info_vec_size = team_info.size();
		w.write_uint16(__team_info_vec_size);
		for(uint16_t i = 0; i < __team_info_vec_size; ++i) {
				team_info[i].serialize(w);
		}

		uint16_t __player_info_vec_size = player_info.size();
		w.write_uint16(__player_info_vec_size);
		for(uint16_t i = 0; i < __player_info_vec_size; ++i) {
				player_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		uint16_t __team_info_vec_size;
		if(r.read_uint16(__team_info_vec_size)  )
				return -1;
		team_info.reserve(__team_info_vec_size);
		for(uint16_t i = 0; i < __team_info_vec_size; ++i) {
				Team_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				team_info.push_back(v);
		}

		uint16_t __player_info_vec_size;
		if(r.read_uint16(__player_info_vec_size)  )
				return -1;
		player_info.reserve(__player_info_vec_size);
		for(uint16_t i = 0; i < __player_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				player_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100315;
		type = 0;
		team_info.clear();
		player_info.clear();
	}
};

/*
拒绝组队邀请
*/
struct MSG_10100316  {
	int64_t role_id;
	MSG_10100316(void) { reset(); };

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
拒绝组队邀请(返回)
*/
struct MSG_50100316 : public Base_Msg  {
	int64_t role_id;
	MSG_50100316(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100316;
		role_id = 0;
	}
};

/*
请求队伍设置
*/
struct MSG_10100308  {
	int8_t type;
	MSG_10100308(void) { reset(); };

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
请求队伍设置(返回)
*/
struct MSG_50100308 : public Base_Msg  {
	int8_t profile_set;
	int32_t num;
	int8_t type;
	MSG_50100308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(profile_set);
		w.write_int32(num);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(profile_set)  ||  r.read_int32(num)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100308;
		profile_set = 0;
		num = 0;
		type = 0;
	}
};

/*
修改组队设置-个人
*/
struct MSG_10100310  {
	int32_t profile_set;
	MSG_10100310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(profile_set);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(profile_set)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		profile_set = 0;
	}
};

/*
修改组队设置-个人(返回)
*/
struct MSG_50100310 : public Base_Msg  {
	int32_t profile_set;
	MSG_50100310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(profile_set);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(profile_set)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100310;
		profile_set = 0;
	}
};

/*
快速加入
*/
struct MSG_10100317  {
	int8_t type;
	MSG_10100317(void) { reset(); };

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
快速加入(返回)
	不返回错误码，用于返回给客户端特殊处理
*/
struct MSG_50100317 : public Base_Msg  {
	int8_t result;
	MSG_50100317(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100317;
		result = 0;
	}
};

/*
设置房间密码
*/
struct MSG_10100318  {
	std::string password_new;
	MSG_10100318(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(password_new);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(password_new)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		password_new.clear();
	}
};

/*
设置房间密码(返回)
*/
struct MSG_50100318 : public Base_Msg  {
	std::string password;
	MSG_50100318(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(password);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(password)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100318;
		password.clear();
	}
};

/*
取消房间密码(返回)
*/
struct MSG_50100319 : public Base_Msg  {
	std::string password;
	MSG_50100319(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(password);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(password)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100319;
		password.clear();
	}
};

/*
取消房间密码
*/
struct MSG_10100319  {
	std::string password;
	MSG_10100319(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(password);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(password)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		password.clear();
	}
};

/*
玩家准备
*/
struct MSG_10100320  {
	int8_t ready;
	MSG_10100320(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(ready);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(ready)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		ready = 0;
	}
};

/*
玩家准备
*/
struct MSG_50100320 : public Base_Msg  {
	int8_t ready;
	MSG_50100320(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(ready);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(ready)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100320;
		ready = 0;
	}
};

/*
请求等待区信息
备注：80100301 
*/
struct MSG_10100321  {
	int8_t type;
	MSG_10100321(void) { reset(); };

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
设置队伍位置开启关闭
*/
struct MSG_10100322  {
	int8_t set;
	int8_t position;
	MSG_10100322(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(set);
		w.write_int8(position);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(set)  ||  r.read_int8(position)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		set = 0;
		position = 0;
	}
};

/*
设置队伍位置开启关闭(返回)
*/
struct MSG_50100322 : public Base_Msg  {
	std::vector<int8_t> index_list;
	MSG_50100322(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __index_list_vec_size = index_list.size();
		w.write_uint16(__index_list_vec_size);
		for(uint16_t i = 0; i < __index_list_vec_size; ++i) {
				w.write_int8(index_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __index_list_vec_size;
		if(r.read_uint16(__index_list_vec_size)  )
				return -1;
		index_list.reserve(__index_list_vec_size);
		for(uint16_t i = 0; i < __index_list_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				index_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100322;
		index_list.clear();
	}
};

/*
修改队伍设置
*/
struct MSG_10100323  {
	int8_t type;
	int32_t scene_id;
	int32_t degree;
	MSG_10100323(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(scene_id);
		w.write_int32(degree);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(scene_id)  ||  r.read_int32(degree)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		scene_id = 0;
		degree = 0;
	}
};

/*
修改队伍设置(返回)
*/
struct MSG_50100323 : public Base_Msg  {
	int8_t type;
	int32_t scene_id;
	int32_t degree;
	MSG_50100323(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(scene_id);
		w.write_int32(degree);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(scene_id)  ||  r.read_int32(degree)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100323;
		type = 0;
		scene_id = 0;
		degree = 0;
	}
};

/*
组队布阵信息
*/
struct MSG_10100324  {
	MSG_10100324(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
组队布阵信息(返回)
Int_Role_Id: 
  role_id(用户id) 
  int_value(0表示前1表示后)
*/
struct MSG_50100324 : public Base_Msg  {
	std::vector<Int_Role_Id> info;
	MSG_50100324(void) { reset(); };

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
		msg_id = 50100324;
		info.clear();
	}
};

/*
组队布阵设置
Int_Role_Id: 
  role_id(用户id) 
  int_value(0表示前1表示后)
*/
struct MSG_10100325  {
	std::vector<Int_Role_Id> info;
	MSG_10100325(void) { reset(); };

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
		info.clear();
	}
};

/*
组队布阵设置(返回)
Int_Role_Id: 
  role_id(用户id) 
  int_value(0表示前1表示后)
*/
struct MSG_50100325 : public Base_Msg  {
	std::vector<Int_Role_Id> info;
	MSG_50100325(void) { reset(); };

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
		msg_id = 50100325;
		info.clear();
	}
};

/*
返回房间
*/
struct MSG_10100326  {
	MSG_10100326(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
返回房间(返回)
*/
struct MSG_50100326 : public Base_Msg  {
	MSG_50100326(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100326;
	}
};

/*
请求英雄梦境列表
*/
struct MSG_10100327  {
	MSG_10100327(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求英雄梦境列表（返回）
*/
struct MSG_50100327 : public Base_Msg  {
	std::vector<int32_t> scene_ids;
	MSG_50100327(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __scene_ids_vec_size = scene_ids.size();
		w.write_uint16(__scene_ids_vec_size);
		for(uint16_t i = 0; i < __scene_ids_vec_size; ++i) {
				w.write_int32(scene_ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __scene_ids_vec_size;
		if(r.read_uint16(__scene_ids_vec_size)  )
				return -1;
		scene_ids.reserve(__scene_ids_vec_size);
		for(uint16_t i = 0; i < __scene_ids_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				scene_ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100327;
		scene_ids.clear();
	}
};

/*
组队跟随设置
*/
struct MSG_10100328  {
	int8_t follow;
	MSG_10100328(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(follow);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(follow)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		follow = 0;
	}
};

/*
组队跟随设置（返回）
*/
struct MSG_50100328 : public Base_Msg  {
	int8_t follow;
	MSG_50100328(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(follow);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(follow)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100328;
		follow = 0;
	}
};

/*
组队竞技测试
*/
struct MSG_10100329  {
	MSG_10100329(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
组队竞技测试(返回)
*/
struct MSG_50100329 : public Base_Msg  {
	MSG_50100329(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100329;
	}
};

/*
请求组队信息
返回房间类型，密码，本人的收益次数返回给我。
*/
struct MSG_10100330  {
	int64_t team_id;
	MSG_10100330(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(team_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(team_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		team_id = 0;
	}
};

/*
请求组队信息(返回)
*/
struct MSG_50100330 : public Base_Msg  {
	int64_t team_id;
	int8_t exist;
	int8_t type;
	std::string password;
	int32_t num;
	int8_t in_battle;
	MSG_50100330(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(team_id);
		w.write_int8(exist);
		w.write_int8(type);
		w.write_string(password);
		w.write_int32(num);
		w.write_int8(in_battle);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(team_id)  ||  r.read_int8(exist)  ||  r.read_int8(type)  ||  r.read_string(password)  ||  r.read_int32(num)  ||  r.read_int8(in_battle)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100330;
		team_id = 0;
		exist = 0;
		type = 0;
		password.clear();
		num = 0;
		in_battle = 0;
	}
};

/*
快速邀请
备注：世界发布
*/
struct MSG_10100331  {
	MSG_10100331(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
快速邀请(返回)
*/
struct MSG_50100331 : public Base_Msg  {
	int8_t result;
	MSG_50100331(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100331;
		result = 0;
	}
};

/*
获取积分排名
*/
struct MSG_10100332  {
	int32_t level;
	int32_t page;
	MSG_10100332(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(level);
		w.write_int32(page);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(level)  ||  r.read_int32(page)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		level = 0;
		page = 0;
	}
};

/*
获取积分排名(返回)
*/
struct MSG_50100332 : public Base_Msg  {
	std::vector<Team_Score_Info> team_score_list;
	int32_t page;
	int32_t page_max;
	int32_t level;
	int32_t award_time;
	MSG_50100332(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __team_score_list_vec_size = team_score_list.size();
		w.write_uint16(__team_score_list_vec_size);
		for(uint16_t i = 0; i < __team_score_list_vec_size; ++i) {
				team_score_list[i].serialize(w);
		}

		w.write_int32(page);
		w.write_int32(page_max);
		w.write_int32(level);
		w.write_int32(award_time);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __team_score_list_vec_size;
		if(r.read_uint16(__team_score_list_vec_size)  )
				return -1;
		team_score_list.reserve(__team_score_list_vec_size);
		for(uint16_t i = 0; i < __team_score_list_vec_size; ++i) {
				Team_Score_Info v;
				if(v.deserialize(r))
						return -1;
				team_score_list.push_back(v);
		}

		if( r.read_int32(page)  ||  r.read_int32(page_max)  ||  r.read_int32(level)  ||  r.read_int32(award_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100332;
		team_score_list.clear();
		page = 0;
		page_max = 0;
		level = 0;
		award_time = 0;
	}
};

/*
请求领取组队积分奖励
*/
struct MSG_10100333  {
	int32_t season;
	MSG_10100333(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(season);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(season)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		season = 0;
	}
};

/*
请求领取组队积分奖励(返回)
*/
struct MSG_50100333 : public Base_Msg  {
	std::vector<int32_t> level;
	MSG_50100333(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __level_vec_size = level.size();
		w.write_uint16(__level_vec_size);
		for(uint16_t i = 0; i < __level_vec_size; ++i) {
				w.write_int32(level[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __level_vec_size;
		if(r.read_uint16(__level_vec_size)  )
				return -1;
		level.reserve(__level_vec_size);
		for(uint16_t i = 0; i < __level_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				level.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100333;
		level.clear();
	}
};



#endif