/*
* Generate by devtool
*/

#ifndef _ROBOT_MSG_STRUCT_H_
#define _ROBOT_MSG_STRUCT_H_
#include "Msg_Struct.h"


/* SERVER MSG */
struct Robot_Base_Msg {
	uint32_t msg_id;
	Robot_Base_Msg(void) : msg_id(0) {}	
	virtual ~Robot_Base_Msg() {};
	virtual void serialize(Block_Buffer & w) const = 0;
	virtual int deserialize(Block_Buffer & r) = 0;
	virtual  void set_rand(void) = 0;
};
#undef INT8_MAX
#define INT8_MAX (uint8_t)pow(2,8); 

#undef INT16_MAX
#define INT16_MAX (uint16_t)pow(2,16); 

#undef INT32_MAX
#define INT32_MAX (uint32_t)pow(2,32); 

#undef INT64_MAX
#define INT64_MAX (uint64_t)pow(2,64); 

typedef std::vector<Robot_Base_Msg*> Robot_Msg_Vec;

/*
获取角色
*/
struct Robot_MSG_10100000 : public Robot_Base_Msg  {
	std::string account;
	int32_t agent_num;
	int32_t server_num;
	Robot_MSG_10100000(void) { reset(); };

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
		msg_id = 10100000;
		account.clear();
		agent_num = 0;
		server_num = 0;
	}
  void set_rand(void) {
          agent_num = rand()%INT32_MAX;
          server_num = rand()%INT32_MAX;
  }
};

/*
登录网关
*/
struct Robot_MSG_10400001 : public Robot_Base_Msg  {
	std::string session_id;
	int64_t role_id;
	Robot_MSG_10400001(void) { reset(); };

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
		msg_id = 10400001;
		session_id.clear();
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
打开人物面板
*/
struct Robot_MSG_10200032 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t open_type;
	Robot_MSG_10200032(void) { reset(); };

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
		msg_id = 10200032;
		role_id = 0;
		open_type = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          open_type = rand()%INT8_MAX;
  }
};

/*
设置剧情信息
*/
struct Robot_MSG_10100010 : public Robot_Base_Msg  {
	std::string plot_id;
	int32_t plot_step;
	Robot_MSG_10100010(void) { reset(); };

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
		msg_id = 10100010;
		plot_id.clear();
		plot_step = 0;
	}
  void set_rand(void) {
          plot_step = rand()%INT32_MAX;
  }
};

/*
请求瞬移(坐电梯)
*/
struct Robot_MSG_10200007 : public Robot_Base_Msg  {
	Coord coord;
	Robot_MSG_10200007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200007;
		coord.reset();
	}
  void set_rand(void) {
  }
};

/*
请求飞行(坐电梯)
*/
struct Robot_MSG_10200005 : public Robot_Base_Msg  {
	Coord coord;
	Robot_MSG_10200005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200005;
		coord.reset();
	}
  void set_rand(void) {
  }
};

/*
客户端初始化完成
*/
struct Robot_MSG_10100015 : public Robot_Base_Msg  {
	Robot_MSG_10100015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100015;
	}
  void set_rand(void) {
  }
};

/*
移动场景击退请求
*/
struct Robot_MSG_10200008 : public Robot_Base_Msg  {
	int16_t pos_x;
	int16_t pos_y;
	Robot_MSG_10200008(void) { reset(); };

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
		msg_id = 10200008;
		pos_x = 0;
		pos_y = 0;
	}
  void set_rand(void) {
          pos_x = rand()%INT16_MAX;
          pos_y = rand()%INT16_MAX;
  }
};

/*
通知防沉迷验证通过
*/
struct Robot_MSG_10100090 : public Robot_Base_Msg  {
	int32_t fcm;
	Robot_MSG_10100090(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(fcm);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(fcm)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100090;
		fcm = 0;
	}
  void set_rand(void) {
          fcm = rand()%INT32_MAX;
  }
};

/*
从逻辑服获取角色信息
*/
struct Robot_MSG_10100006 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10100006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100006;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
电梯到达请求
*/
struct Robot_MSG_10200009 : public Robot_Base_Msg  {
	int64_t lift_id;
	Robot_MSG_10200009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(lift_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(lift_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200009;
		lift_id = 0;
	}
  void set_rand(void) {
          lift_id = rand()%INT64_MAX;
  }
};

/*
获取角色随机名字
*/
struct Robot_MSG_10100007 : public Robot_Base_Msg  {
	int8_t gender;
	Robot_MSG_10100007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(gender);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(gender)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100007;
		gender = 0;
	}
  void set_rand(void) {
          gender = rand()%INT8_MAX;
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
struct Robot_MSG_10188888 : public Robot_Base_Msg  {
	uint32_t type;
	Robot_MSG_10188888(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10188888;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT32_MAX;
  }
};

/*
从场景服获取人物信息
备注：返回 81000100
*/
struct Robot_MSG_10200006 : public Robot_Base_Msg  {
	Robot_MSG_10200006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10200006;
	}
  void set_rand(void) {
  }
};

/*
场景加载完成
备注：10200030--场景加载完成
*/
struct Robot_MSG_10200030 : public Robot_Base_Msg  {
	Robot_MSG_10200030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10200030;
	}
  void set_rand(void) {
  }
};

/*
请求改名
*/
struct Robot_MSG_10100008 : public Robot_Base_Msg  {
	std::string new_name;
	Robot_MSG_10100008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(new_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(new_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100008;
		new_name.clear();
	}
  void set_rand(void) {
  }
};

/*
打开人物面板_逻辑服
*/
struct Robot_MSG_10100032 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t open_type;
	Robot_MSG_10100032(void) { reset(); };

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
		msg_id = 10100032;
		role_id = 0;
		open_type = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          open_type = rand()%INT8_MAX;
  }
};

/*
请求进入活动场景
*/
struct Robot_MSG_10100040 : public Robot_Base_Msg  {
	int32_t active_id;
	Robot_MSG_10100040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(active_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(active_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100040;
		active_id = 0;
	}
  void set_rand(void) {
          active_id = rand()%INT32_MAX;
  }
};

/*
移动
*/
struct Robot_MSG_10200102 : public Robot_Base_Msg  {
	int64_t role_id;
	std::vector<Coord> path;
	Robot_MSG_10200102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __path_vec_size = path.size();
		w.write_uint16(__path_vec_size);
		for(uint16_t i = 0; i < __path_vec_size; ++i) {
				path[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __path_vec_size;
		if(r.read_uint16(__path_vec_size)  )
				return -1;
		path.reserve(__path_vec_size);
		for(uint16_t i = 0; i < __path_vec_size; ++i) {
				Coord v;
				if(v.deserialize(r))
						return -1;
				path.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10200102;
		role_id = 0;
		path.clear();
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
战斗准备完成
*/
struct Robot_MSG_10200105 : public Robot_Base_Msg  {
	Robot_MSG_10200105(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10200105;
	}
  void set_rand(void) {
  }
};

/*
使用技能
*/
struct Robot_MSG_10200110 : public Robot_Base_Msg  {
	int64_t role_id;
	int32_t skill;
	Robot_MSG_10200110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(skill);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(skill)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200110;
		role_id = 0;
		skill = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          skill = rand()%INT32_MAX;
  }
};

/*
邀请决斗
*/
struct Robot_MSG_10200150 : public Robot_Base_Msg  {
	int64_t role_id;
	uint8_t type;
	Robot_MSG_10200150(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200150;
		role_id = 0;
		type = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
洗点技能
*/
struct Robot_MSG_10100165 : public Robot_Base_Msg  {
	int32_t type;
	int32_t talent_id;
	Robot_MSG_10100165(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100165;
		type = 0;
		talent_id = 0;
	}
  void set_rand(void) {
          type = rand()%INT32_MAX;
          talent_id = rand()%INT32_MAX;
  }
};

/*
技能位置移动
*/
struct Robot_MSG_10100166 : public Robot_Base_Msg  {
	int32_t skill_id;
	int32_t new_loc;
	int32_t talent_id;
	Robot_MSG_10100166(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int32(new_loc);
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int32(new_loc)  ||  r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100166;
		skill_id = 0;
		new_loc = 0;
		talent_id = 0;
	}
  void set_rand(void) {
          skill_id = rand()%INT32_MAX;
          new_loc = rand()%INT32_MAX;
          talent_id = rand()%INT32_MAX;
  }
};

/*
请求和怪物战斗
*/
struct Robot_MSG_10200151 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10200151(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200151;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
学习技能
*/
struct Robot_MSG_10100141 : public Robot_Base_Msg  {
	int32_t talent_id;
	int32_t skill_id;
	int32_t lv;
	Robot_MSG_10100141(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
		w.write_int32(skill_id);
		w.write_int32(lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  ||  r.read_int32(skill_id)  ||  r.read_int32(lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100141;
		talent_id = 0;
		skill_id = 0;
		lv = 0;
	}
  void set_rand(void) {
          talent_id = rand()%INT32_MAX;
          skill_id = rand()%INT32_MAX;
          lv = rand()%INT32_MAX;
  }
};

/*
获取技能列表
*/
struct Robot_MSG_10100140 : public Robot_Base_Msg  {
	int32_t talent_id;
	Robot_MSG_10100140(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100140;
		talent_id = 0;
	}
  void set_rand(void) {
          talent_id = rand()%INT32_MAX;
  }
};

/*
英雄移动
*/
struct Robot_MSG_10200103 : public Robot_Base_Msg  {
	int64_t role_id;
	std::vector<Coord> path;
	Robot_MSG_10200103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __path_vec_size = path.size();
		w.write_uint16(__path_vec_size);
		for(uint16_t i = 0; i < __path_vec_size; ++i) {
				path[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __path_vec_size;
		if(r.read_uint16(__path_vec_size)  )
				return -1;
		path.reserve(__path_vec_size);
		for(uint16_t i = 0; i < __path_vec_size; ++i) {
				Coord v;
				if(v.deserialize(r))
						return -1;
				path.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10200103;
		role_id = 0;
		path.clear();
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
开启指定页，指定天赋
*/
struct Robot_MSG_10100142 : public Robot_Base_Msg  {
	int32_t page_id;
	int32_t talent_id;
	Robot_MSG_10100142(void) { reset(); };

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
		msg_id = 10100142;
		page_id = 0;
		talent_id = 0;
	}
  void set_rand(void) {
          page_id = rand()%INT32_MAX;
          talent_id = rand()%INT32_MAX;
  }
};

/*
开启指定天赋
*/
struct Robot_MSG_10100143 : public Robot_Base_Msg  {
	int32_t talent_id;
	Robot_MSG_10100143(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100143;
		talent_id = 0;
	}
  void set_rand(void) {
          talent_id = rand()%INT32_MAX;
  }
};

/*
使用天赋
*/
struct Robot_MSG_10100144 : public Robot_Base_Msg  {
	int32_t talent_id;
	Robot_MSG_10100144(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100144;
		talent_id = 0;
	}
  void set_rand(void) {
          talent_id = rand()%INT32_MAX;
  }
};

/*
新手期战斗引导暂停
*/
struct Robot_MSG_10200111 : public Robot_Base_Msg  {
	int32_t type;
	Robot_MSG_10200111(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200111;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT32_MAX;
  }
};

/*
应答邀请决斗
*/
struct Robot_MSG_10200155 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t duel;
	uint8_t type;
	Robot_MSG_10200155(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(duel);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(duel)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200155;
		role_id = 0;
		duel = 0;
		type = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          duel = rand()%INT8_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
发起挑战_普通场景
*/
struct Robot_MSG_10200156 : public Robot_Base_Msg  {
	int64_t role_id;
	uint8_t type;
	Robot_MSG_10200156(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200156;
		role_id = 0;
		type = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
战斗认输
*/
struct Robot_MSG_10200113 : public Robot_Base_Msg  {
	Robot_MSG_10200113(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10200113;
	}
  void set_rand(void) {
  }
};

/*
请求跟怪物战斗（不在场景的怪）
*/
struct Robot_MSG_10200149 : public Robot_Base_Msg  {
	int32_t monster_type_id;
	int8_t type;
	Robot_MSG_10200149(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(monster_type_id);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(monster_type_id)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200149;
		monster_type_id = 0;
		type = 0;
	}
  void set_rand(void) {
          monster_type_id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
多人战斗认输
*/
struct Robot_MSG_10200117 : public Robot_Base_Msg  {
	int32_t type;
	Robot_MSG_10200117(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200117;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT32_MAX;
  }
};

/*
获取背包信息
备注：
1、更新容量 
2、更新物品

10000：人物装备
20000：人物背包
30000：人物仓库
40000：成就勋章身上

100000：符石熔炉
110000：符石背包
120000：符石仓库
130000：符石身上
140000：符石魔盒
*/
struct Robot_MSG_10100100 : public Robot_Base_Msg  {
	uint32_t pack_type;
	uint32_t pack_sub_type;
	Robot_MSG_10100100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(pack_type);
		w.write_uint32(pack_sub_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(pack_type)  ||  r.read_uint32(pack_sub_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100100;
		pack_type = 0;
		pack_sub_type = 0;
	}
  void set_rand(void) {
          pack_type = rand()%INT32_MAX;
          pack_sub_type = rand()%INT32_MAX;
  }
};

/*
增加容量
*/
struct Robot_MSG_10100101 : public Robot_Base_Msg  {
	uint8_t pay_type;
	uint32_t pack_type;
	uint16_t add_capacity;
	Robot_MSG_10100101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(pay_type);
		w.write_uint32(pack_type);
		w.write_uint16(add_capacity);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(pay_type)  ||  r.read_uint32(pack_type)  ||  r.read_uint16(add_capacity)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100101;
		pay_type = 0;
		pack_type = 0;
		add_capacity = 0;
	}
  void set_rand(void) {
          pay_type = rand()%INT8_MAX;
          pack_type = rand()%INT32_MAX;
          add_capacity = rand()%INT16_MAX;
  }
};

/*
丢弃物品
*/
struct Robot_MSG_10100102 : public Robot_Base_Msg  {
	std::vector<uint32_t> index;
	Robot_MSG_10100102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __index_vec_size = index.size();
		w.write_uint16(__index_vec_size);
		for(uint16_t i = 0; i < __index_vec_size; ++i) {
				w.write_uint32(index[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __index_vec_size;
		if(r.read_uint16(__index_vec_size)  )
				return -1;
		index.reserve(__index_vec_size);
		for(uint16_t i = 0; i < __index_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				index.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10100102;
		index.clear();
	}
  void set_rand(void) {
  }
};

/*
移动物品
*/
struct Robot_MSG_10100103 : public Robot_Base_Msg  {
	uint32_t index_from;
	uint32_t index_to;
	Robot_MSG_10100103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index_from);
		w.write_uint32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index_from)  ||  r.read_uint32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100103;
		index_from = 0;
		index_to = 0;
	}
  void set_rand(void) {
          index_from = rand()%INT32_MAX;
          index_to = rand()%INT32_MAX;
  }
};

/*
拆分物品
*/
struct Robot_MSG_10100104 : public Robot_Base_Msg  {
	uint32_t index;
	int16_t split_num;
	Robot_MSG_10100104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_int16(split_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_int16(split_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100104;
		index = 0;
		split_num = 0;
	}
  void set_rand(void) {
          index = rand()%INT32_MAX;
          split_num = rand()%INT16_MAX;
  }
};

/*
整理物品
*/
struct Robot_MSG_10100105 : public Robot_Base_Msg  {
	uint32_t pack_type;
	Robot_MSG_10100105(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(pack_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(pack_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100105;
		pack_type = 0;
	}
  void set_rand(void) {
          pack_type = rand()%INT32_MAX;
  }
};

/*
使用指定位置的物品
备注：所有类型的物品都由此接口使用，由服务端判断使用物品的效果并以主动消息形式通知。
*/
struct Robot_MSG_10100106 : public Robot_Base_Msg  {
	uint32_t index;
	int32_t amount;
	int32_t e_value;
	Robot_MSG_10100106(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_int32(amount);
		w.write_int32(e_value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_int32(amount)  ||  r.read_int32(e_value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100106;
		index = 0;
		amount = 0;
		e_value = 0;
	}
  void set_rand(void) {
          index = rand()%INT32_MAX;
          amount = rand()%INT32_MAX;
          e_value = rand()%INT32_MAX;
  }
};

/*
使用指定型礼包
*/
struct Robot_MSG_10100107 : public Robot_Base_Msg  {
	int32_t item_index;
	std::vector<int32_t> select_index_vector;
	Robot_MSG_10100107(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_index);
		uint16_t __select_index_vector_vec_size = select_index_vector.size();
		w.write_uint16(__select_index_vector_vec_size);
		for(uint16_t i = 0; i < __select_index_vector_vec_size; ++i) {
				w.write_int32(select_index_vector[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_index)  )
				return -1;
 		uint16_t __select_index_vector_vec_size;
		if(r.read_uint16(__select_index_vector_vec_size)  )
				return -1;
		select_index_vector.reserve(__select_index_vector_vec_size);
		for(uint16_t i = 0; i < __select_index_vector_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				select_index_vector.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10100107;
		item_index = 0;
		select_index_vector.clear();
	}
  void set_rand(void) {
          item_index = rand()%INT32_MAX;
  }
};

/*
请求合并物品
备注：整理并合并物品，可能会将非绑定物品转成绑定物品以节约空间，同凡2的合并
*/
struct Robot_MSG_10100108 : public Robot_Base_Msg  {
	uint32_t pack_type;
	Robot_MSG_10100108(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(pack_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(pack_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100108;
		pack_type = 0;
	}
  void set_rand(void) {
          pack_type = rand()%INT32_MAX;
  }
};

/*
请求出售物品
Int_Int:val_1位置 val_2数量
*/
struct Robot_MSG_10100109 : public Robot_Base_Msg  {
	std::vector<Int_Int> index;
	Robot_MSG_10100109(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __index_vec_size = index.size();
		w.write_uint16(__index_vec_size);
		for(uint16_t i = 0; i < __index_vec_size; ++i) {
				index[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __index_vec_size;
		if(r.read_uint16(__index_vec_size)  )
				return -1;
		index.reserve(__index_vec_size);
		for(uint16_t i = 0; i < __index_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				index.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10100109;
		index.clear();
	}
  void set_rand(void) {
  }
};

/*
取出爬塔副本仓库
*/
struct Robot_MSG_10100110 : public Robot_Base_Msg  {
	uint32_t index;
	Robot_MSG_10100110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100110;
		index = 0;
	}
  void set_rand(void) {
          index = rand()%INT32_MAX;
  }
};

/*
从任意仓库取出物品
*/
struct Robot_MSG_10100111 : public Robot_Base_Msg  {
	int8_t type;
	int32_t index;
	Robot_MSG_10100111(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100111;
		type = 0;
		index = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          index = rand()%INT32_MAX;
  }
};

/*
获取指定角色背包信息
*/
struct Robot_MSG_10100112 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10100112(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100112;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
购买并使用
*/
struct Robot_MSG_10100113 : public Robot_Base_Msg  {
	int32_t item_id;
	int32_t amount;
	Robot_MSG_10100113(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(amount);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(amount)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100113;
		item_id = 0;
		amount = 0;
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
          amount = rand()%INT32_MAX;
  }
};

/*
获取任务列表
备注：
获得任务界面的任务列表            //可接,已接,日常. 
初次登录获取,或发生错时再次获取. 
*/
struct Robot_MSG_10110000 : public Robot_Base_Msg  {
	Robot_MSG_10110000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10110000;
	}
  void set_rand(void) {
  }
};

/*
获取NPC任务(历史原因，遗留，未用到，待删除)
返回 520201 
假如NPC有任务,服务器将返回任务列表
*/
struct Robot_MSG_10110001 : public Robot_Base_Msg  {
	uint32_t npc_id;
	uint8_t source;
	Robot_MSG_10110001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(npc_id);
		w.write_uint8(source);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(npc_id)  ||  r.read_uint8(source)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110001;
		npc_id = 0;
		source = 0;
	}
  void set_rand(void) {
          npc_id = rand()%INT32_MAX;
          source = rand()%INT8_MAX;
  }
};

/*
接受任务
成功返回 520203,否则返回错误号. 

如任务可接，800200  800201会主动通知更新任务信息,客户端根据任务状态变化改变任务可接/以接列表．同时800200  800201还会通知把任务加入到"可接列表". (一次接收至少有4条主动消息)
*/
struct Robot_MSG_10110002 : public Robot_Base_Msg  {
	uint32_t task_id;
	Robot_MSG_10110002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110002;
		task_id = 0;
	}
  void set_rand(void) {
          task_id = rand()%INT32_MAX;
  }
};

/*
放弃任务
成功返回 520204,否则返回错误号.  

如任务可放弃,800200  800201会主动通知移除"以接列表"的任务信息.　同时800200  800201还会通知把任务加入到"可接列表". (一次放弃至少有4条主动消息)
*/
struct Robot_MSG_10110003 : public Robot_Base_Msg  {
	uint32_t task_id;
	Robot_MSG_10110003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110003;
		task_id = 0;
	}
  void set_rand(void) {
          task_id = rand()%INT32_MAX;
  }
};

/*
交付任务
成功返回 50110004,否则返回错误号.  

如任务可放弃,800200  800201会主动通知移除"以接列表"的任务信息.
*/
struct Robot_MSG_10110004 : public Robot_Base_Msg  {
	uint32_t task_id;
	Robot_MSG_10110004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110004;
		task_id = 0;
	}
  void set_rand(void) {
          task_id = rand()%INT32_MAX;
  }
};

/*
领取任务奖励
自动交付任务用到．如领取成功返回 520206 ,　失败返回错误号 , 领取成功后客户端移除奖励状态．
*/
struct Robot_MSG_10110005 : public Robot_Base_Msg  {
	uint32_t task_id;
	Robot_MSG_10110005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110005;
		task_id = 0;
	}
  void set_rand(void) {
          task_id = rand()%INT32_MAX;
  }
};

/*
批量接受任务
自动交付任务用到．如领取成功返回 520206 ,　失败返回错误号 , 领取成功后客户端移除奖励状态．
*/
struct Robot_MSG_10110006 : public Robot_Base_Msg  {
	std::vector<uint32_t> task_id_list;
	int32_t type;
	Robot_MSG_10110006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __task_id_list_vec_size = task_id_list.size();
		w.write_uint16(__task_id_list_vec_size);
		for(uint16_t i = 0; i < __task_id_list_vec_size; ++i) {
				w.write_uint32(task_id_list[i]);
		}

		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __task_id_list_vec_size;
		if(r.read_uint16(__task_id_list_vec_size)  )
				return -1;
		task_id_list.reserve(__task_id_list_vec_size);
		for(uint16_t i = 0; i < __task_id_list_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				task_id_list.push_back(v);
		}

		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110006;
		task_id_list.clear();
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT32_MAX;
  }
};

/*
快速完成
客户端请求快速完成任务的消息，服务端成功不返回。 如果失败，采用通用的错误消息，返回相应的错误号。
不管成功还是失败，都会主动发送任务更新或删除的消息。

type:  
       默认情况下，  type=0；
      如果策划配置物品、货币都可以扣，在这种情况下，如果物品不足，不处理快速完成，服务器主动发送81110005 消息。 客户端收到此消息，弹窗告知物品不足，是否愿意消耗指定货币及数量。 玩家点确定， 客户端此时再次发快速完成消息， type =1.  其他所有情况下， type=0 。
 
*/
struct Robot_MSG_10110007 : public Robot_Base_Msg  {
	uint32_t task_id;
	uint8_t type;
	uint8_t is_bind_diamond_first;
	Robot_MSG_10110007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint8(type);
		w.write_uint8(is_bind_diamond_first);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint8(type)  ||  r.read_uint8(is_bind_diamond_first)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110007;
		task_id = 0;
		type = 0;
		is_bind_diamond_first = 0;
	}
  void set_rand(void) {
          task_id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
          is_bind_diamond_first = rand()%INT8_MAX;
  }
};

/*
剧情消息
*/
struct Robot_MSG_10110008 : public Robot_Base_Msg  {
	std::string plot_id;
	int8_t action;
	int16_t num;
	int8_t type;
	Robot_MSG_10110008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(plot_id);
		w.write_int8(action);
		w.write_int16(num);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(plot_id)  ||  r.read_int8(action)  ||  r.read_int16(num)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110008;
		plot_id.clear();
		action = 0;
		num = 0;
		type = 0;
	}
  void set_rand(void) {
          action = rand()%INT8_MAX;
          num = rand()%INT16_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
NPC对话任务，完成对话，请求完成任务
*/
struct Robot_MSG_10110009 : public Robot_Base_Msg  {
	uint32_t npc_id;
	Robot_MSG_10110009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(npc_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(npc_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110009;
		npc_id = 0;
	}
  void set_rand(void) {
          npc_id = rand()%INT32_MAX;
  }
};

/*
完成副本清除剧情数据
*/
struct Robot_MSG_10110010 : public Robot_Base_Msg  {
	std::vector<std::string> plot_ids;
	Robot_MSG_10110010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __plot_ids_vec_size = plot_ids.size();
		w.write_uint16(__plot_ids_vec_size);
		for(uint16_t i = 0; i < __plot_ids_vec_size; ++i) {
				w.write_string(plot_ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __plot_ids_vec_size;
		if(r.read_uint16(__plot_ids_vec_size)  )
				return -1;
		plot_ids.reserve(__plot_ids_vec_size);
		for(uint16_t i = 0; i < __plot_ids_vec_size; ++i) {
				std::string v;
				if(r.read_string(v) )
						return -1;
				plot_ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10110010;
		plot_ids.clear();
	}
  void set_rand(void) {
  }
};

/*
任务随身怪处理
*/
struct Robot_MSG_10110011 : public Robot_Base_Msg  {
	int32_t npc_type;
	int8_t set;
	int64_t npc_id;
	std::string funcName;
	int32_t direction;
	std::string sceneId;
	std::string open;
	int32_t x;
	int32_t y;
	std::string role_name;
	Robot_MSG_10110011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(npc_type);
		w.write_int8(set);
		w.write_int64(npc_id);
		w.write_string(funcName);
		w.write_int32(direction);
		w.write_string(sceneId);
		w.write_string(open);
		w.write_int32(x);
		w.write_int32(y);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(npc_type)  ||  r.read_int8(set)  ||  r.read_int64(npc_id)  ||  r.read_string(funcName)  ||  r.read_int32(direction)  ||  r.read_string(sceneId)  ||  r.read_string(open)  ||  r.read_int32(x)  ||  r.read_int32(y)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110011;
		npc_type = 0;
		set = 0;
		npc_id = 0;
		funcName.clear();
		direction = 0;
		sceneId.clear();
		open.clear();
		x = 0;
		y = 0;
		role_name.clear();
	}
  void set_rand(void) {
          npc_type = rand()%INT32_MAX;
          set = rand()%INT8_MAX;
          npc_id = rand()%INT64_MAX;
          direction = rand()%INT32_MAX;
          x = rand()%INT32_MAX;
          y = rand()%INT32_MAX;
  }
};

/*
请求悬赏任务列表
备注：打开悬赏榜时，悬赏榜任务列表显示上一次刷新出来的列表。
         免费刷新、立即刷新，会把悬赏榜刷新为新的任务列表。
         is_refresh:  是否请求刷新。0:否， 1：是。
                            (1)打开悬赏榜时，应该显示上一次刷新出来的列表，所以， is_refresh=0 。
                            (2)点击  免费刷新、立即刷新， is_refresh = 1
*/
struct Robot_MSG_10110012 : public Robot_Base_Msg  {
	uint8_t is_refresh;
	Robot_MSG_10110012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_refresh);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_refresh)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110012;
		is_refresh = 0;
	}
  void set_rand(void) {
          is_refresh = rand()%INT8_MAX;
  }
};

/*
请求打开悬赏榜
备注：如果可以打开悬赏榜，服务端返回 50110013， 并且，status =1 (0：不能打开悬赏榜)。 如果不能打开悬赏榜，不发送50110013。
发送80000001 消息给客户端。
*/
struct Robot_MSG_10110013 : public Robot_Base_Msg  {
	Robot_MSG_10110013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10110013;
	}
  void set_rand(void) {
  }
};

/*
任务监听打开私聊窗口
*/
struct Robot_MSG_10110014 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10110014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110014;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
请求提交道具
*/
struct Robot_MSG_10110015 : public Robot_Base_Msg  {
	uint32_t task_id;
	Robot_MSG_10110015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110015;
		task_id = 0;
	}
  void set_rand(void) {
          task_id = rand()%INT32_MAX;
  }
};

/*
请求恢复任务链
*/
struct Robot_MSG_10110016 : public Robot_Base_Msg  {
	uint8_t type;
	Robot_MSG_10110016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110016;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
设置特效已经播放
*/
struct Robot_MSG_10110017 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10110017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110017;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求显示日常任务列表
*/
struct Robot_MSG_10110018 : public Robot_Base_Msg  {
	Robot_MSG_10110018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10110018;
	}
  void set_rand(void) {
  }
};

/*
玩家已读体验过期消息
*/
struct Robot_MSG_10110019 : public Robot_Base_Msg  {
	uint8_t type;
	Robot_MSG_10110019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10110019;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
获取邮件列表
*/
struct Robot_MSG_10150000 : public Robot_Base_Msg  {
	int8_t page_count;
	int8_t require_mail_state;
	Robot_MSG_10150000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(page_count);
		w.write_int8(require_mail_state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(page_count)  ||  r.read_int8(require_mail_state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10150000;
		page_count = 0;
		require_mail_state = 0;
	}
  void set_rand(void) {
          page_count = rand()%INT8_MAX;
          require_mail_state = rand()%INT8_MAX;
  }
};

/*
查看邮件
*/
struct Robot_MSG_10150001 : public Robot_Base_Msg  {
	int32_t mail_id;
	Robot_MSG_10150001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mail_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mail_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10150001;
		mail_id = 0;
	}
  void set_rand(void) {
          mail_id = rand()%INT32_MAX;
  }
};

/*
发送邮件
*/
struct Robot_MSG_10150002 : public Robot_Base_Msg  {
	Mail_Info mail_info;
	Robot_MSG_10150002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		mail_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( mail_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10150002;
		mail_info.reset();
	}
  void set_rand(void) {
  }
};

/*
批量删除邮件
*/
struct Robot_MSG_10150005 : public Robot_Base_Msg  {
	std::vector<int32_t> muti_mail_id;
	Robot_MSG_10150005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __muti_mail_id_vec_size = muti_mail_id.size();
		w.write_uint16(__muti_mail_id_vec_size);
		for(uint16_t i = 0; i < __muti_mail_id_vec_size; ++i) {
				w.write_int32(muti_mail_id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __muti_mail_id_vec_size;
		if(r.read_uint16(__muti_mail_id_vec_size)  )
				return -1;
		muti_mail_id.reserve(__muti_mail_id_vec_size);
		for(uint16_t i = 0; i < __muti_mail_id_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				muti_mail_id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10150005;
		muti_mail_id.clear();
	}
  void set_rand(void) {
  }
};

/*
请求收取附件
*/
struct Robot_MSG_10150003 : public Robot_Base_Msg  {
	int32_t mail_id;
	std::vector<uint32_t> item_vector_id;
	uint8_t type;
	Robot_MSG_10150003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mail_id);
		uint16_t __item_vector_id_vec_size = item_vector_id.size();
		w.write_uint16(__item_vector_id_vec_size);
		for(uint16_t i = 0; i < __item_vector_id_vec_size; ++i) {
				w.write_uint32(item_vector_id[i]);
		}

		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mail_id)  )
				return -1;
 		uint16_t __item_vector_id_vec_size;
		if(r.read_uint16(__item_vector_id_vec_size)  )
				return -1;
		item_vector_id.reserve(__item_vector_id_vec_size);
		for(uint16_t i = 0; i < __item_vector_id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				item_vector_id.push_back(v);
		}

		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10150003;
		mail_id = 0;
		item_vector_id.clear();
		type = 0;
	}
  void set_rand(void) {
          mail_id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求一键收取附件
*/
struct Robot_MSG_10150004 : public Robot_Base_Msg  {
	Robot_MSG_10150004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10150004;
	}
  void set_rand(void) {
  }
};

/*
私聊聊天记录请求
*/
struct Robot_MSG_10300001 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10300001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10300001;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
公会聊天记录请求
*/
struct Robot_MSG_10300002 : public Robot_Base_Msg  {
	Robot_MSG_10300002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10300002;
	}
  void set_rand(void) {
  }
};

/*
发起输入状态
*/
struct Robot_MSG_10300003 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t status;
	Robot_MSG_10300003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10300003;
		role_id = 0;
		status = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          status = rand()%INT8_MAX;
  }
};

/*
请求同场景聊天
*/
struct Robot_MSG_10200011 : public Robot_Base_Msg  {
	std::vector<Chat_Content> content;
	Robot_MSG_10200011(void) { reset(); };

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
		msg_id = 10200011;
		content.clear();
	}
  void set_rand(void) {
  }
};

/*
逻辑服测试消息
*/
struct Robot_MSG_10199999 : public Robot_Base_Msg  {
	std::string command;
	Robot_MSG_10199999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(command);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(command)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10199999;
		command.clear();
	}
  void set_rand(void) {
  }
};

/*
场景服测试消息
*/
struct Robot_MSG_10299999 : public Robot_Base_Msg  {
	std::string command;
	Robot_MSG_10299999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(command);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(command)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10299999;
		command.clear();
	}
  void set_rand(void) {
  }
};

/*
聊天服测试消息
*/
struct Robot_MSG_10399999 : public Robot_Base_Msg  {
	std::string command;
	Robot_MSG_10399999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(command);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(command)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10399999;
		command.clear();
	}
  void set_rand(void) {
  }
};

/*
网关服测试消息
*/
struct Robot_MSG_10499999 : public Robot_Base_Msg  {
	std::string command;
	Robot_MSG_10499999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(command);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(command)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10499999;
		command.clear();
	}
  void set_rand(void) {
  }
};

/*
请求已通关单人副本
*/
struct Robot_MSG_10100200 : public Robot_Base_Msg  {
	Robot_MSG_10100200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100200;
	}
  void set_rand(void) {
  }
};

/*
请求翻牌
*/
struct Robot_MSG_10100201 : public Robot_Base_Msg  {
	int32_t index;
	int8_t type;
	Robot_MSG_10100201(void) { reset(); };

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
		msg_id = 10100201;
		index = 0;
		type = 0;
	}
  void set_rand(void) {
          index = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求精英副本信息
*/
struct Robot_MSG_10100202 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100202;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
请求重置精英副本
*/
struct Robot_MSG_10100204 : public Robot_Base_Msg  {
	int8_t bind_gold_first;
	Robot_MSG_10100204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(bind_gold_first);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(bind_gold_first)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100204;
		bind_gold_first = 0;
	}
  void set_rand(void) {
          bind_gold_first = rand()%INT8_MAX;
  }
};

/*
请求领取单人副本章节奖励
*/
struct Robot_MSG_10100205 : public Robot_Base_Msg  {
	int8_t chapter;
	Robot_MSG_10100205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(chapter);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(chapter)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100205;
		chapter = 0;
	}
  void set_rand(void) {
          chapter = rand()%INT8_MAX;
  }
};

/*
副本结算面板关闭通知
*/
struct Robot_MSG_10200200 : public Robot_Base_Msg  {
	Robot_MSG_10200200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10200200;
	}
  void set_rand(void) {
  }
};

/*
请求扫荡单人副本
*/
struct Robot_MSG_10100210 : public Robot_Base_Msg  {
	int32_t scene_id;
	uint8_t num;
	int8_t wipe_type;
	Robot_MSG_10100210(void) { reset(); };

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
		msg_id = 10100210;
		scene_id = 0;
		num = 0;
		wipe_type = 0;
	}
  void set_rand(void) {
          scene_id = rand()%INT32_MAX;
          num = rand()%INT8_MAX;
          wipe_type = rand()%INT8_MAX;
  }
};

/*
请求中断扫荡单人副本
*/
struct Robot_MSG_10100211 : public Robot_Base_Msg  {
	int8_t wipe_type;
	Robot_MSG_10100211(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100211;
		wipe_type = 0;
	}
  void set_rand(void) {
          wipe_type = rand()%INT8_MAX;
  }
};

/*
请求完成扫荡单人副本
*/
struct Robot_MSG_10100212 : public Robot_Base_Msg  {
	int8_t wipe_type;
	Robot_MSG_10100212(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100212;
		wipe_type = 0;
	}
  void set_rand(void) {
          wipe_type = rand()%INT8_MAX;
  }
};

/*
请求获得双倍扫荡收益
*/
struct Robot_MSG_10100213 : public Robot_Base_Msg  {
	int32_t wipe_type;
	Robot_MSG_10100213(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(wipe_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(wipe_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100213;
		wipe_type = 0;
	}
  void set_rand(void) {
          wipe_type = rand()%INT32_MAX;
  }
};

/*
请求勾选双倍扫荡
*/
struct Robot_MSG_10100214 : public Robot_Base_Msg  {
	int32_t wipe_type;
	int32_t is_selected;
	Robot_MSG_10100214(void) { reset(); };

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
		msg_id = 10100214;
		wipe_type = 0;
		is_selected = 0;
	}
  void set_rand(void) {
          wipe_type = rand()%INT32_MAX;
          is_selected = rand()%INT32_MAX;
  }
};

/*
请求购买上古副本额外次数
*/
struct Robot_MSG_10100206 : public Robot_Base_Msg  {
	int32_t scene_id;
	Robot_MSG_10100206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100206;
		scene_id = 0;
	}
  void set_rand(void) {
          scene_id = rand()%INT32_MAX;
  }
};

/*
请求上古副本传送
*/
struct Robot_MSG_10100207 : public Robot_Base_Msg  {
	int32_t dest_scene;
	Robot_MSG_10100207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(dest_scene);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(dest_scene)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100207;
		dest_scene = 0;
	}
  void set_rand(void) {
          dest_scene = rand()%INT32_MAX;
  }
};

/*
请求单人副本复活
*/
struct Robot_MSG_10200215 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10200215(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200215;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
请求领取上古魔神首通奖励
*/
struct Robot_MSG_10100208 : public Robot_Base_Msg  {
	Robot_MSG_10100208(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100208;
	}
  void set_rand(void) {
  }
};

/*
装备穿脱
*/
struct Robot_MSG_10101000 : public Robot_Base_Msg  {
	uint32_t index_from;
	uint32_t index_to;
	Robot_MSG_10101000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index_from);
		w.write_uint32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index_from)  ||  r.read_uint32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101000;
		index_from = 0;
		index_to = 0;
	}
  void set_rand(void) {
          index_from = rand()%INT32_MAX;
          index_to = rand()%INT32_MAX;
  }
};

/*
装备解封
*/
struct Robot_MSG_10101001 : public Robot_Base_Msg  {
	uint32_t index;
	uint32_t lucky_stone_index;
	uint32_t lucky_stone_id;
	int8_t auto_buy;
	Robot_MSG_10101001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_uint32(lucky_stone_index);
		w.write_uint32(lucky_stone_id);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_uint32(lucky_stone_index)  ||  r.read_uint32(lucky_stone_id)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101001;
		index = 0;
		lucky_stone_index = 0;
		lucky_stone_id = 0;
		auto_buy = 0;
	}
  void set_rand(void) {
          index = rand()%INT32_MAX;
          lucky_stone_index = rand()%INT32_MAX;
          lucky_stone_id = rand()%INT32_MAX;
          auto_buy = rand()%INT8_MAX;
  }
};

/*
装备精通
*/
struct Robot_MSG_10101002 : public Robot_Base_Msg  {
	uint32_t index;
	uint8_t auto_buy;
	Robot_MSG_10101002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_uint8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_uint8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101002;
		index = 0;
		auto_buy = 0;
	}
  void set_rand(void) {
          index = rand()%INT32_MAX;
          auto_buy = rand()%INT8_MAX;
  }
};

/*
装备合成
*/
struct Robot_MSG_10101003 : public Robot_Base_Msg  {
	int32_t id;
	int32_t amount;
	int8_t use_diamond;
	Robot_MSG_10101003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(amount);
		w.write_int8(use_diamond);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(amount)  ||  r.read_int8(use_diamond)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101003;
		id = 0;
		amount = 0;
		use_diamond = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          amount = rand()%INT32_MAX;
          use_diamond = rand()%INT8_MAX;
  }
};

/*
获取精通信息
*/
struct Robot_MSG_10101004 : public Robot_Base_Msg  {
	Robot_MSG_10101004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101004;
	}
  void set_rand(void) {
  }
};

/*
装备附魔
*/
struct Robot_MSG_10101005 : public Robot_Base_Msg  {
	int32_t equip_index;
	int8_t is_bat;
	int8_t auto_buy;
	int8_t auto_buy_stone;
	int8_t auto_buy_lock;
	std::vector<int8_t> locks;
	Robot_MSG_10101005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(is_bat);
		w.write_int8(auto_buy);
		w.write_int8(auto_buy_stone);
		w.write_int8(auto_buy_lock);
		uint16_t __locks_vec_size = locks.size();
		w.write_uint16(__locks_vec_size);
		for(uint16_t i = 0; i < __locks_vec_size; ++i) {
				w.write_int8(locks[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(is_bat)  ||  r.read_int8(auto_buy)  ||  r.read_int8(auto_buy_stone)  ||  r.read_int8(auto_buy_lock)  )
				return -1;
 		uint16_t __locks_vec_size;
		if(r.read_uint16(__locks_vec_size)  )
				return -1;
		locks.reserve(__locks_vec_size);
		for(uint16_t i = 0; i < __locks_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				locks.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10101005;
		equip_index = 0;
		is_bat = 0;
		auto_buy = 0;
		auto_buy_stone = 0;
		auto_buy_lock = 0;
		locks.clear();
	}
  void set_rand(void) {
          equip_index = rand()%INT32_MAX;
          is_bat = rand()%INT8_MAX;
          auto_buy = rand()%INT8_MAX;
          auto_buy_stone = rand()%INT8_MAX;
          auto_buy_lock = rand()%INT8_MAX;
  }
};

/*
装备附魔属性组替换
*/
struct Robot_MSG_10101006 : public Robot_Base_Msg  {
	int32_t equip_index;
	int8_t group_index;
	Robot_MSG_10101006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(group_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(group_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101006;
		equip_index = 0;
		group_index = 0;
	}
  void set_rand(void) {
          equip_index = rand()%INT32_MAX;
          group_index = rand()%INT8_MAX;
  }
};

/*
镶嵌宝石
*/
struct Robot_MSG_10101007 : public Robot_Base_Msg  {
	int32_t equip_index;
	int8_t hole_index;
	int32_t gemstone_id;
	Robot_MSG_10101007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(hole_index);
		w.write_int32(gemstone_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(hole_index)  ||  r.read_int32(gemstone_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101007;
		equip_index = 0;
		hole_index = 0;
		gemstone_id = 0;
	}
  void set_rand(void) {
          equip_index = rand()%INT32_MAX;
          hole_index = rand()%INT8_MAX;
          gemstone_id = rand()%INT32_MAX;
  }
};

/*
卸装宝石
*/
struct Robot_MSG_10101008 : public Robot_Base_Msg  {
	int32_t equip_index;
	int8_t hole_index;
	Robot_MSG_10101008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(hole_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(hole_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101008;
		equip_index = 0;
		hole_index = 0;
	}
  void set_rand(void) {
          equip_index = rand()%INT32_MAX;
          hole_index = rand()%INT8_MAX;
  }
};

/*
装备开宝石孔
*/
struct Robot_MSG_10101009 : public Robot_Base_Msg  {
	int32_t equip_index;
	int8_t hole_index;
	Robot_MSG_10101009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(hole_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(hole_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101009;
		equip_index = 0;
		hole_index = 0;
	}
  void set_rand(void) {
          equip_index = rand()%INT32_MAX;
          hole_index = rand()%INT8_MAX;
  }
};

/*
宝石转化
*/
struct Robot_MSG_10101010 : public Robot_Base_Msg  {
	int32_t src_gemstone_id;
	int32_t des_gemstone_id;
	int32_t num;
	Robot_MSG_10101010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(src_gemstone_id);
		w.write_int32(des_gemstone_id);
		w.write_int32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(src_gemstone_id)  ||  r.read_int32(des_gemstone_id)  ||  r.read_int32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101010;
		src_gemstone_id = 0;
		des_gemstone_id = 0;
		num = 0;
	}
  void set_rand(void) {
          src_gemstone_id = rand()%INT32_MAX;
          des_gemstone_id = rand()%INT32_MAX;
          num = rand()%INT32_MAX;
  }
};

/*
宝石信息合成
*/
struct Robot_MSG_10101011 : public Robot_Base_Msg  {
	Robot_MSG_10101011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101011;
	}
  void set_rand(void) {
  }
};

/*
装备分解
*/
struct Robot_MSG_10101012 : public Robot_Base_Msg  {
	std::vector<int32_t> equip_index;
	Robot_MSG_10101012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __equip_index_vec_size = equip_index.size();
		w.write_uint16(__equip_index_vec_size);
		for(uint16_t i = 0; i < __equip_index_vec_size; ++i) {
				w.write_int32(equip_index[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __equip_index_vec_size;
		if(r.read_uint16(__equip_index_vec_size)  )
				return -1;
		equip_index.reserve(__equip_index_vec_size);
		for(uint16_t i = 0; i < __equip_index_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				equip_index.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10101012;
		equip_index.clear();
	}
  void set_rand(void) {
  }
};

/*
装备分解获得道具
*/
struct Robot_MSG_10101013 : public Robot_Base_Msg  {
	std::vector<int32_t> equip_index;
	Robot_MSG_10101013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __equip_index_vec_size = equip_index.size();
		w.write_uint16(__equip_index_vec_size);
		for(uint16_t i = 0; i < __equip_index_vec_size; ++i) {
				w.write_int32(equip_index[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __equip_index_vec_size;
		if(r.read_uint16(__equip_index_vec_size)  )
				return -1;
		equip_index.reserve(__equip_index_vec_size);
		for(uint16_t i = 0; i < __equip_index_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				equip_index.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10101013;
		equip_index.clear();
	}
  void set_rand(void) {
  }
};

/*
请求宝石雕刻面板
*/
struct Robot_MSG_10101014 : public Robot_Base_Msg  {
	int32_t gem_essence_num;
	Robot_MSG_10101014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(gem_essence_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(gem_essence_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101014;
		gem_essence_num = 0;
	}
  void set_rand(void) {
          gem_essence_num = rand()%INT32_MAX;
  }
};

/*
请求宝石雕刻等级
*/
struct Robot_MSG_10101015 : public Robot_Base_Msg  {
	Robot_MSG_10101015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101015;
	}
  void set_rand(void) {
  }
};

/*
获取掉落详细信息
*/
struct Robot_MSG_10210000 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10210000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10210000;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
领取掉落宝箱物品
*/
struct Robot_MSG_10210001 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t index;
	Robot_MSG_10210001(void) { reset(); };

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
		msg_id = 10210001;
		role_id = 0;
		index = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          index = rand()%INT8_MAX;
  }
};

/*
开始采集
*/
struct Robot_MSG_10210100 : public Robot_Base_Msg  {
	int64_t material_role_id;
	Robot_MSG_10210100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(material_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(material_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10210100;
		material_role_id = 0;
	}
  void set_rand(void) {
          material_role_id = rand()%INT64_MAX;
  }
};

/*
中断采集
*/
struct Robot_MSG_10210101 : public Robot_Base_Msg  {
	int64_t material_role_id;
	Robot_MSG_10210101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(material_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(material_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10210101;
		material_role_id = 0;
	}
  void set_rand(void) {
          material_role_id = rand()%INT64_MAX;
  }
};

/*
战斗中创建怪物
*/
struct Robot_MSG_10210200 : public Robot_Base_Msg  {
	int32_t monter_type_id;
	int8_t pos;
	Robot_MSG_10210200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(monter_type_id);
		w.write_int8(pos);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(monter_type_id)  ||  r.read_int8(pos)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10210200;
		monter_type_id = 0;
		pos = 0;
	}
  void set_rand(void) {
          monter_type_id = rand()%INT32_MAX;
          pos = rand()%INT8_MAX;
  }
};

/*
开启机关
*/
struct Robot_MSG_10210103 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10210103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10210103;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
触发机关通知
*/
struct Robot_MSG_10210104 : public Robot_Base_Msg  {
	uint16_t event_id;
	Coord p_coord;
	Coord m_coord;
	Robot_MSG_10210104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(event_id);
		p_coord.serialize(w);
		m_coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(event_id)  ||  p_coord.deserialize(r)  ||  m_coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10210104;
		event_id = 0;
		p_coord.reset();
		m_coord.reset();
	}
  void set_rand(void) {
          event_id = rand()%INT16_MAX;
  }
};

/*
请求拾取物品
*/
struct Robot_MSG_10210002 : public Robot_Base_Msg  {
	std::vector<int64_t> role_id_vec;
	Robot_MSG_10210002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_id_vec_vec_size = role_id_vec.size();
		w.write_uint16(__role_id_vec_vec_size);
		for(uint16_t i = 0; i < __role_id_vec_vec_size; ++i) {
				w.write_int64(role_id_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_id_vec_vec_size;
		if(r.read_uint16(__role_id_vec_vec_size)  )
				return -1;
		role_id_vec.reserve(__role_id_vec_vec_size);
		for(uint16_t i = 0; i < __role_id_vec_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				role_id_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10210002;
		role_id_vec.clear();
	}
  void set_rand(void) {
  }
};

/*
请求在场景刷怪、采集物、宝箱
*/
struct Robot_MSG_10210010 : public Robot_Base_Msg  {
	int32_t point_id;
	Robot_MSG_10210010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10210010;
		point_id = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
  }
};

/*
请求跟怪点战斗
*/
struct Robot_MSG_10210011 : public Robot_Base_Msg  {
	int32_t point_id;
	Robot_MSG_10210011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10210011;
		point_id = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
  }
};

/*
获取玩家英雄信息
*/
struct Robot_MSG_10102000 : public Robot_Base_Msg  {
	Robot_MSG_10102000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10102000;
	}
  void set_rand(void) {
  }
};

/*
英雄升级
*/
struct Robot_MSG_10102001 : public Robot_Base_Msg  {
	int32_t hero_index;
	Robot_MSG_10102001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102001;
		hero_index = 0;
	}
  void set_rand(void) {
          hero_index = rand()%INT32_MAX;
  }
};

/*
英雄装备穿脱
*/
struct Robot_MSG_10102002 : public Robot_Base_Msg  {
	uint32_t index_from;
	uint32_t index_to;
	Robot_MSG_10102002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index_from);
		w.write_uint32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index_from)  ||  r.read_uint32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102002;
		index_from = 0;
		index_to = 0;
	}
  void set_rand(void) {
          index_from = rand()%INT32_MAX;
          index_to = rand()%INT32_MAX;
  }
};

/*
英雄跟随
*/
struct Robot_MSG_10102003 : public Robot_Base_Msg  {
	int32_t hero_index;
	int8_t is_out;
	Robot_MSG_10102003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
		w.write_int8(is_out);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  ||  r.read_int8(is_out)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102003;
		hero_index = 0;
		is_out = 0;
	}
  void set_rand(void) {
          hero_index = rand()%INT32_MAX;
          is_out = rand()%INT8_MAX;
  }
};

/*
英雄招募
*/
struct Robot_MSG_10102004 : public Robot_Base_Msg  {
	int32_t hero_id;
	int8_t use_diamond;
	Robot_MSG_10102004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_id);
		w.write_int8(use_diamond);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_id)  ||  r.read_int8(use_diamond)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102004;
		hero_id = 0;
		use_diamond = 0;
	}
  void set_rand(void) {
          hero_id = rand()%INT32_MAX;
          use_diamond = rand()%INT8_MAX;
  }
};

/*
英雄设置阵型
*/
struct Robot_MSG_10102005 : public Robot_Base_Msg  {
	int32_t hero_index;
	int32_t hero_formation;
	int8_t is_alert;
	Robot_MSG_10102005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
		w.write_int32(hero_formation);
		w.write_int8(is_alert);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  ||  r.read_int32(hero_formation)  ||  r.read_int8(is_alert)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102005;
		hero_index = 0;
		hero_formation = 0;
		is_alert = 0;
	}
  void set_rand(void) {
          hero_index = rand()%INT32_MAX;
          hero_formation = rand()%INT32_MAX;
          is_alert = rand()%INT8_MAX;
  }
};

/*
设置战斗英雄
*/
struct Robot_MSG_10102006 : public Robot_Base_Msg  {
	int32_t hero_index;
	int8_t is_fight;
	Robot_MSG_10102006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
		w.write_int8(is_fight);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  ||  r.read_int8(is_fight)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102006;
		hero_index = 0;
		is_fight = 0;
	}
  void set_rand(void) {
          hero_index = rand()%INT32_MAX;
          is_fight = rand()%INT8_MAX;
  }
};

/*
英雄觉醒
*/
struct Robot_MSG_10102007 : public Robot_Base_Msg  {
	int8_t is_hero;
	int32_t hero_index;
	Robot_MSG_10102007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_hero);
		w.write_int32(hero_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_hero)  ||  r.read_int32(hero_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102007;
		is_hero = 0;
		hero_index = 0;
	}
  void set_rand(void) {
          is_hero = rand()%INT8_MAX;
          hero_index = rand()%INT32_MAX;
  }
};

/*
英雄神器升级
*/
struct Robot_MSG_10102020 : public Robot_Base_Msg  {
	int32_t is_hero;
	int32_t artifact_id;
	int32_t hero_index;
	Robot_MSG_10102020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(is_hero);
		w.write_int32(artifact_id);
		w.write_int32(hero_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(is_hero)  ||  r.read_int32(artifact_id)  ||  r.read_int32(hero_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102020;
		is_hero = 0;
		artifact_id = 0;
		hero_index = 0;
	}
  void set_rand(void) {
          is_hero = rand()%INT32_MAX;
          artifact_id = rand()%INT32_MAX;
          hero_index = rand()%INT32_MAX;
  }
};

/*
点击新英雄去掉new
*/
struct Robot_MSG_10102021 : public Robot_Base_Msg  {
	int32_t hero_id;
	Robot_MSG_10102021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102021;
		hero_id = 0;
	}
  void set_rand(void) {
          hero_id = rand()%INT32_MAX;
  }
};

/*
获取新英雄信息
*/
struct Robot_MSG_10102022 : public Robot_Base_Msg  {
	Robot_MSG_10102022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10102022;
	}
  void set_rand(void) {
  }
};

/*
邀请英雄助阵
*/
struct Robot_MSG_10102032 : public Robot_Base_Msg  {
	int8_t type;
	int32_t hero_idx;
	int32_t loc;
	Robot_MSG_10102032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(hero_idx);
		w.write_int32(loc);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(hero_idx)  ||  r.read_int32(loc)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102032;
		type = 0;
		hero_idx = 0;
		loc = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          hero_idx = rand()%INT32_MAX;
          loc = rand()%INT32_MAX;
  }
};

/*
助阵英雄互动
*/
struct Robot_MSG_10102033 : public Robot_Base_Msg  {
	int32_t id;
	int32_t answer;
	int32_t hero_idx;
	Robot_MSG_10102033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(answer);
		w.write_int32(hero_idx);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(answer)  ||  r.read_int32(hero_idx)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102033;
		id = 0;
		answer = 0;
		hero_idx = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          answer = rand()%INT32_MAX;
          hero_idx = rand()%INT32_MAX;
  }
};

/*
赠送助阵英雄礼品
*/
struct Robot_MSG_10102034 : public Robot_Base_Msg  {
	int32_t item_id;
	int32_t hero_idx;
	Robot_MSG_10102034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(hero_idx);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(hero_idx)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10102034;
		item_id = 0;
		hero_idx = 0;
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
          hero_idx = rand()%INT32_MAX;
  }
};

/*
获取好友基本信息
*/
struct Robot_MSG_10160001 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10160001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10160001;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
获取好友列表
*/
struct Robot_MSG_10160000 : public Robot_Base_Msg  {
	Robot_MSG_10160000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10160000;
	}
  void set_rand(void) {
  }
};

/*
添加好友
*/
struct Robot_MSG_10160004 : public Robot_Base_Msg  {
	std::string role_name;
	Robot_MSG_10160004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10160004;
		role_name.clear();
	}
  void set_rand(void) {
  }
};

/*
获取好友申请列表
*/
struct Robot_MSG_10160002 : public Robot_Base_Msg  {
	Robot_MSG_10160002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10160002;
	}
  void set_rand(void) {
  }
};

/*
删除好友
*/
struct Robot_MSG_10160003 : public Robot_Base_Msg  {
	int64_t friend_id;
	int8_t friend_type;
	Robot_MSG_10160003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(friend_id);
		w.write_int8(friend_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(friend_id)  ||  r.read_int8(friend_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10160003;
		friend_id = 0;
		friend_type = 0;
	}
  void set_rand(void) {
          friend_id = rand()%INT64_MAX;
          friend_type = rand()%INT8_MAX;
  }
};

/*
拉进黑名单
*/
struct Robot_MSG_10160005 : public Robot_Base_Msg  {
	std::string role_name;
	Robot_MSG_10160005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10160005;
		role_name.clear();
	}
  void set_rand(void) {
  }
};

/*
处理好友申请
*/
struct Robot_MSG_10160009 : public Robot_Base_Msg  {
	int8_t handel_friend_apply;
	std::string role_name;
	Robot_MSG_10160009(void) { reset(); };

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
		msg_id = 10160009;
		handel_friend_apply = 0;
		role_name.clear();
	}
  void set_rand(void) {
          handel_friend_apply = rand()%INT8_MAX;
  }
};

/*
获取角色在线状态
*/
struct Robot_MSG_10160010 : public Robot_Base_Msg  {
	std::vector<int64_t> stranger_id_list;
	std::vector<int64_t> recent_id_list;
	Robot_MSG_10160010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __stranger_id_list_vec_size = stranger_id_list.size();
		w.write_uint16(__stranger_id_list_vec_size);
		for(uint16_t i = 0; i < __stranger_id_list_vec_size; ++i) {
				w.write_int64(stranger_id_list[i]);
		}

		uint16_t __recent_id_list_vec_size = recent_id_list.size();
		w.write_uint16(__recent_id_list_vec_size);
		for(uint16_t i = 0; i < __recent_id_list_vec_size; ++i) {
				w.write_int64(recent_id_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __stranger_id_list_vec_size;
		if(r.read_uint16(__stranger_id_list_vec_size)  )
				return -1;
		stranger_id_list.reserve(__stranger_id_list_vec_size);
		for(uint16_t i = 0; i < __stranger_id_list_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				stranger_id_list.push_back(v);
		}

		uint16_t __recent_id_list_vec_size;
		if(r.read_uint16(__recent_id_list_vec_size)  )
				return -1;
		recent_id_list.reserve(__recent_id_list_vec_size);
		for(uint16_t i = 0; i < __recent_id_list_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				recent_id_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10160010;
		stranger_id_list.clear();
		recent_id_list.clear();
	}
  void set_rand(void) {
  }
};

/*
获取设置头像列表
*/
struct Robot_MSG_10160011 : public Robot_Base_Msg  {
	Robot_MSG_10160011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10160011;
	}
  void set_rand(void) {
  }
};

/*
请求设置头像
*/
struct Robot_MSG_10160012 : public Robot_Base_Msg  {
	int32_t photo_id;
	Robot_MSG_10160012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(photo_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(photo_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10160012;
		photo_id = 0;
	}
  void set_rand(void) {
          photo_id = rand()%INT32_MAX;
  }
};

/*
批量添加好友
*/
struct Robot_MSG_10160013 : public Robot_Base_Msg  {
	std::vector<std::string> role_name_vec;
	Robot_MSG_10160013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_name_vec_vec_size = role_name_vec.size();
		w.write_uint16(__role_name_vec_vec_size);
		for(uint16_t i = 0; i < __role_name_vec_vec_size; ++i) {
				w.write_string(role_name_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_name_vec_vec_size;
		if(r.read_uint16(__role_name_vec_vec_size)  )
				return -1;
		role_name_vec.reserve(__role_name_vec_vec_size);
		for(uint16_t i = 0; i < __role_name_vec_vec_size; ++i) {
				std::string v;
				if(r.read_string(v) )
						return -1;
				role_name_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10160013;
		role_name_vec.clear();
	}
  void set_rand(void) {
  }
};

/*
获取好友基础信息列表
*/
struct Robot_MSG_10160014 : public Robot_Base_Msg  {
	Robot_MSG_10160014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10160014;
	}
  void set_rand(void) {
  }
};

/*
获取商品列表
*/
struct Robot_MSG_10151000 : public Robot_Base_Msg  {
	Robot_MSG_10151000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10151000;
	}
  void set_rand(void) {
  }
};

/*
购买商品
*/
struct Robot_MSG_10151001 : public Robot_Base_Msg  {
	int32_t id;
	int32_t tabs;
	int32_t amount;
	std::string role_name;
	int32_t buy_type;
	int32_t helper_fast_use;
	Robot_MSG_10151001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(tabs);
		w.write_int32(amount);
		w.write_string(role_name);
		w.write_int32(buy_type);
		w.write_int32(helper_fast_use);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(tabs)  ||  r.read_int32(amount)  ||  r.read_string(role_name)  ||  r.read_int32(buy_type)  ||  r.read_int32(helper_fast_use)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10151001;
		id = 0;
		tabs = 0;
		amount = 0;
		role_name.clear();
		buy_type = 0;
		helper_fast_use = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          tabs = rand()%INT32_MAX;
          amount = rand()%INT32_MAX;
          buy_type = rand()%INT32_MAX;
          helper_fast_use = rand()%INT32_MAX;
  }
};

/*
一键购买商品
*/
struct Robot_MSG_10151002 : public Robot_Base_Msg  {
	int32_t tabs;
	std::vector<int32_t> ids;
	Robot_MSG_10151002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(tabs);
		uint16_t __ids_vec_size = ids.size();
		w.write_uint16(__ids_vec_size);
		for(uint16_t i = 0; i < __ids_vec_size; ++i) {
				w.write_int32(ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(tabs)  )
				return -1;
 		uint16_t __ids_vec_size;
		if(r.read_uint16(__ids_vec_size)  )
				return -1;
		ids.reserve(__ids_vec_size);
		for(uint16_t i = 0; i < __ids_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10151002;
		tabs = 0;
		ids.clear();
	}
  void set_rand(void) {
          tabs = rand()%INT32_MAX;
  }
};

/*
请求快速购买物品
*/
struct Robot_MSG_10151003 : public Robot_Base_Msg  {
	int32_t item_id;
	uint32_t amount;
	uint8_t buy_type;
	int8_t helper_fast_use;
	Robot_MSG_10151003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_uint32(amount);
		w.write_uint8(buy_type);
		w.write_int8(helper_fast_use);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_uint32(amount)  ||  r.read_uint8(buy_type)  ||  r.read_int8(helper_fast_use)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10151003;
		item_id = 0;
		amount = 0;
		buy_type = 0;
		helper_fast_use = 0;
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
          amount = rand()%INT32_MAX;
          buy_type = rand()%INT8_MAX;
          helper_fast_use = rand()%INT8_MAX;
  }
};

/*
请求大厅信息
主动广播：
80100300
80100301

*/
struct Robot_MSG_10100300 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100300;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
创建房间
*/
struct Robot_MSG_10100301 : public Robot_Base_Msg  {
	int32_t scene_id;
	std::string password;
	int8_t type;
	int8_t degree;
	Robot_MSG_10100301(void) { reset(); };

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
		msg_id = 10100301;
		scene_id = 0;
		password.clear();
		type = 0;
		degree = 0;
	}
  void set_rand(void) {
          scene_id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
          degree = rand()%INT8_MAX;
  }
};

/*
邀请加入房间
*/
struct Robot_MSG_10100302 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t type;
	int32_t value;
	std::string monster;
	Robot_MSG_10100302(void) { reset(); };

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
		msg_id = 10100302;
		role_id = 0;
		type = 0;
		value = 0;
		monster.clear();
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          type = rand()%INT8_MAX;
          value = rand()%INT32_MAX;
  }
};

/*
申请加入房间
*/
struct Robot_MSG_10100303 : public Robot_Base_Msg  {
	int64_t role_id;
	int64_t index;
	std::string password;
	int8_t is_world;
	int8_t profile_set;
	Robot_MSG_10100303(void) { reset(); };

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
		msg_id = 10100303;
		role_id = 0;
		index = 0;
		password.clear();
		is_world = 0;
		profile_set = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          index = rand()%INT64_MAX;
          is_world = rand()%INT8_MAX;
          profile_set = rand()%INT8_MAX;
  }
};

/*
设为队长
*/
struct Robot_MSG_10100304 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10100304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100304;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
踢出队伍
*/
struct Robot_MSG_10100305 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10100305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100305;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
离开队伍
*/
struct Robot_MSG_10100306 : public Robot_Base_Msg  {
	Robot_MSG_10100306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100306;
	}
  void set_rand(void) {
  }
};

/*
解散队伍
*/
struct Robot_MSG_10100307 : public Robot_Base_Msg  {
	Robot_MSG_10100307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100307;
	}
  void set_rand(void) {
  }
};

/*
同意组队邀请
*/
struct Robot_MSG_10100309 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t profile_set;
	Robot_MSG_10100309(void) { reset(); };

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
		msg_id = 10100309;
		role_id = 0;
		profile_set = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          profile_set = rand()%INT8_MAX;
  }
};

/*
请求队伍招人
*/
struct Robot_MSG_10100311 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10100311(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100311;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
玩家进入待战区
*/
struct Robot_MSG_10100312 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100312(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100312;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
玩家退出待战区
*/
struct Robot_MSG_10100313 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100313(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100313;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
监听组队信息
*/
struct Robot_MSG_10100315 : public Robot_Base_Msg  {
	int8_t listened;
	int8_t type;
	Robot_MSG_10100315(void) { reset(); };

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
		msg_id = 10100315;
		listened = 0;
		type = 0;
	}
  void set_rand(void) {
          listened = rand()%INT8_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
拒绝组队邀请
*/
struct Robot_MSG_10100316 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10100316(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100316;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
请求队伍设置
*/
struct Robot_MSG_10100308 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100308;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
修改组队设置-个人
*/
struct Robot_MSG_10100310 : public Robot_Base_Msg  {
	int32_t profile_set;
	Robot_MSG_10100310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(profile_set);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(profile_set)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100310;
		profile_set = 0;
	}
  void set_rand(void) {
          profile_set = rand()%INT32_MAX;
  }
};

/*
快速加入
*/
struct Robot_MSG_10100317 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100317(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100317;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
设置房间密码
*/
struct Robot_MSG_10100318 : public Robot_Base_Msg  {
	std::string password_new;
	Robot_MSG_10100318(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(password_new);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(password_new)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100318;
		password_new.clear();
	}
  void set_rand(void) {
  }
};

/*
取消房间密码
*/
struct Robot_MSG_10100319 : public Robot_Base_Msg  {
	std::string password;
	Robot_MSG_10100319(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(password);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(password)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100319;
		password.clear();
	}
  void set_rand(void) {
  }
};

/*
玩家准备
*/
struct Robot_MSG_10100320 : public Robot_Base_Msg  {
	int8_t ready;
	Robot_MSG_10100320(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(ready);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(ready)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100320;
		ready = 0;
	}
  void set_rand(void) {
          ready = rand()%INT8_MAX;
  }
};

/*
请求等待区信息
备注：80100301 
*/
struct Robot_MSG_10100321 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100321(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100321;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
设置队伍位置开启关闭
*/
struct Robot_MSG_10100322 : public Robot_Base_Msg  {
	int8_t set;
	int8_t position;
	Robot_MSG_10100322(void) { reset(); };

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
		msg_id = 10100322;
		set = 0;
		position = 0;
	}
  void set_rand(void) {
          set = rand()%INT8_MAX;
          position = rand()%INT8_MAX;
  }
};

/*
修改队伍设置
*/
struct Robot_MSG_10100323 : public Robot_Base_Msg  {
	int8_t type;
	int32_t scene_id;
	int32_t degree;
	Robot_MSG_10100323(void) { reset(); };

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
		msg_id = 10100323;
		type = 0;
		scene_id = 0;
		degree = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          scene_id = rand()%INT32_MAX;
          degree = rand()%INT32_MAX;
  }
};

/*
组队布阵信息
*/
struct Robot_MSG_10100324 : public Robot_Base_Msg  {
	Robot_MSG_10100324(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100324;
	}
  void set_rand(void) {
  }
};

/*
组队布阵设置
Int_Role_Id: 
  role_id(用户id) 
  int_value(0表示前1表示后)
*/
struct Robot_MSG_10100325 : public Robot_Base_Msg  {
	std::vector<Int_Role_Id> info;
	Robot_MSG_10100325(void) { reset(); };

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
		msg_id = 10100325;
		info.clear();
	}
  void set_rand(void) {
  }
};

/*
返回房间
*/
struct Robot_MSG_10100326 : public Robot_Base_Msg  {
	Robot_MSG_10100326(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100326;
	}
  void set_rand(void) {
  }
};

/*
请求英雄梦境列表
*/
struct Robot_MSG_10100327 : public Robot_Base_Msg  {
	Robot_MSG_10100327(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100327;
	}
  void set_rand(void) {
  }
};

/*
组队跟随设置
*/
struct Robot_MSG_10100328 : public Robot_Base_Msg  {
	int8_t follow;
	Robot_MSG_10100328(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(follow);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(follow)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100328;
		follow = 0;
	}
  void set_rand(void) {
          follow = rand()%INT8_MAX;
  }
};

/*
组队竞技测试
*/
struct Robot_MSG_10100329 : public Robot_Base_Msg  {
	Robot_MSG_10100329(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100329;
	}
  void set_rand(void) {
  }
};

/*
请求组队信息
返回房间类型，密码，本人的收益次数返回给我。
*/
struct Robot_MSG_10100330 : public Robot_Base_Msg  {
	int64_t team_id;
	Robot_MSG_10100330(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(team_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(team_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100330;
		team_id = 0;
	}
  void set_rand(void) {
          team_id = rand()%INT64_MAX;
  }
};

/*
快速邀请
备注：世界发布
*/
struct Robot_MSG_10100331 : public Robot_Base_Msg  {
	Robot_MSG_10100331(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100331;
	}
  void set_rand(void) {
  }
};

/*
获取积分排名
*/
struct Robot_MSG_10100332 : public Robot_Base_Msg  {
	int32_t level;
	int32_t page;
	Robot_MSG_10100332(void) { reset(); };

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
		msg_id = 10100332;
		level = 0;
		page = 0;
	}
  void set_rand(void) {
          level = rand()%INT32_MAX;
          page = rand()%INT32_MAX;
  }
};

/*
请求领取组队积分奖励
*/
struct Robot_MSG_10100333 : public Robot_Base_Msg  {
	int32_t season;
	Robot_MSG_10100333(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(season);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(season)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100333;
		season = 0;
	}
  void set_rand(void) {
          season = rand()%INT32_MAX;
  }
};

/*
创建公会
*/
struct Robot_MSG_10170000 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t logo_id;
	std::string name;
	int8_t money_type;
	Robot_MSG_10170000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(logo_id);
		w.write_string(name);
		w.write_int8(money_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(logo_id)  ||  r.read_string(name)  ||  r.read_int8(money_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170000;
		role_id = 0;
		logo_id = 0;
		name.clear();
		money_type = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          logo_id = rand()%INT8_MAX;
          money_type = rand()%INT8_MAX;
  }
};

/*
加入公会
*/
struct Robot_MSG_10170001 : public Robot_Base_Msg  {
	int64_t gang_id;
	Robot_MSG_10170001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170001;
		gang_id = 0;
	}
  void set_rand(void) {
          gang_id = rand()%INT64_MAX;
  }
};

/*
获取公会信息
*/
struct Robot_MSG_10170002 : public Robot_Base_Msg  {
	int64_t gang_id;
	Robot_MSG_10170002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170002;
		gang_id = 0;
	}
  void set_rand(void) {
          gang_id = rand()%INT64_MAX;
  }
};

/*
获取个人公会信息
备注：
加入、创建公会后的个人信息
*/
struct Robot_MSG_10170003 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10170003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170003;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
获取公会列表
*/
struct Robot_MSG_10170004 : public Robot_Base_Msg  {
	Robot_MSG_10170004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170004;
	}
  void set_rand(void) {
  }
};

/*
获取公会成员列表
*/
struct Robot_MSG_10170005 : public Robot_Base_Msg  {
	int64_t gang_id;
	Robot_MSG_10170005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170005;
		gang_id = 0;
	}
  void set_rand(void) {
          gang_id = rand()%INT64_MAX;
  }
};

/*
退出公会
*/
struct Robot_MSG_10170006 : public Robot_Base_Msg  {
	Robot_MSG_10170006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170006;
	}
  void set_rand(void) {
  }
};

/*
转让公会
*/
struct Robot_MSG_10170007 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10170007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170007;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
监听公会信息更新
*/
struct Robot_MSG_10170008 : public Robot_Base_Msg  {
	int8_t listen;
	Robot_MSG_10170008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(listen);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(listen)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170008;
		listen = 0;
	}
  void set_rand(void) {
          listen = rand()%INT8_MAX;
  }
};

/*
踢出公会
*/
struct Robot_MSG_10170009 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10170009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170009;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
修改公会公告
*/
struct Robot_MSG_10170010 : public Robot_Base_Msg  {
	std::string announce;
	Robot_MSG_10170010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(announce);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(announce)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170010;
		announce.clear();
	}
  void set_rand(void) {
  }
};

/*
邀请好友加入公会
*/
struct Robot_MSG_10170011 : public Robot_Base_Msg  {
	std::vector<int64_t> role_ids;
	Robot_MSG_10170011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_ids_vec_size = role_ids.size();
		w.write_uint16(__role_ids_vec_size);
		for(uint16_t i = 0; i < __role_ids_vec_size; ++i) {
				w.write_int64(role_ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_ids_vec_size;
		if(r.read_uint16(__role_ids_vec_size)  )
				return -1;
		role_ids.reserve(__role_ids_vec_size);
		for(uint16_t i = 0; i < __role_ids_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				role_ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10170011;
		role_ids.clear();
	}
  void set_rand(void) {
  }
};

/*
获取公会动态
*/
struct Robot_MSG_10170012 : public Robot_Base_Msg  {
	Robot_MSG_10170012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170012;
	}
  void set_rand(void) {
  }
};

/*
获取公会简易动态
*/
struct Robot_MSG_10170013 : public Robot_Base_Msg  {
	Robot_MSG_10170013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170013;
	}
  void set_rand(void) {
  }
};

/*
发送公会群邮件
*/
struct Robot_MSG_10170014 : public Robot_Base_Msg  {
	Mail_Info mail_info;
	Robot_MSG_10170014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		mail_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( mail_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170014;
		mail_info.reset();
	}
  void set_rand(void) {
  }
};

/*
公会捐献
*/
struct Robot_MSG_10170015 : public Robot_Base_Msg  {
	int8_t type;
	uint32_t money;
	Robot_MSG_10170015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_uint32(money);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_uint32(money)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170015;
		type = 0;
		money = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          money = rand()%INT32_MAX;
  }
};

/*
公会加入设置
*/
struct Robot_MSG_10170016 : public Robot_Base_Msg  {
	int8_t join_block;
	Robot_MSG_10170016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(join_block);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(join_block)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170016;
		join_block = 0;
	}
  void set_rand(void) {
          join_block = rand()%INT8_MAX;
  }
};

/*
获取公会邀请列表
*/
struct Robot_MSG_10170017 : public Robot_Base_Msg  {
	Robot_MSG_10170017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170017;
	}
  void set_rand(void) {
  }
};

/*
获取公会申请列表
*/
struct Robot_MSG_10170018 : public Robot_Base_Msg  {
	Robot_MSG_10170018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170018;
	}
  void set_rand(void) {
  }
};

/*
公会列表操作[申请,取消]
*/
struct Robot_MSG_10170019 : public Robot_Base_Msg  {
	int64_t gang_id;
	int8_t action;
	Robot_MSG_10170019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int8(action);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int8(action)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170019;
		gang_id = 0;
		action = 0;
	}
  void set_rand(void) {
          gang_id = rand()%INT64_MAX;
          action = rand()%INT8_MAX;
  }
};

/*
邀请列表操作[同意,拒绝]
*/
struct Robot_MSG_10170020 : public Robot_Base_Msg  {
	int64_t gang_id;
	int8_t action;
	Robot_MSG_10170020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int8(action);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int8(action)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170020;
		gang_id = 0;
		action = 0;
	}
  void set_rand(void) {
          gang_id = rand()%INT64_MAX;
          action = rand()%INT8_MAX;
  }
};

/*
申请列表操作[同意,拒绝]
*/
struct Robot_MSG_10170021 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t action;
	Robot_MSG_10170021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(action);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(action)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170021;
		role_id = 0;
		action = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          action = rand()%INT8_MAX;
  }
};

/*
公会成员职位权限改变
*/
struct Robot_MSG_10170022 : public Robot_Base_Msg  {
	int64_t member_role_id;
	int8_t headship;
	std::vector<int8_t> add_auth_vec;
	Robot_MSG_10170022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(member_role_id);
		w.write_int8(headship);
		uint16_t __add_auth_vec_vec_size = add_auth_vec.size();
		w.write_uint16(__add_auth_vec_vec_size);
		for(uint16_t i = 0; i < __add_auth_vec_vec_size; ++i) {
				w.write_int8(add_auth_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(member_role_id)  ||  r.read_int8(headship)  )
				return -1;
 		uint16_t __add_auth_vec_vec_size;
		if(r.read_uint16(__add_auth_vec_vec_size)  )
				return -1;
		add_auth_vec.reserve(__add_auth_vec_vec_size);
		for(uint16_t i = 0; i < __add_auth_vec_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				add_auth_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10170022;
		member_role_id = 0;
		headship = 0;
		add_auth_vec.clear();
	}
  void set_rand(void) {
          member_role_id = rand()%INT64_MAX;
          headship = rand()%INT8_MAX;
  }
};

/*
公会升级
*/
struct Robot_MSG_10170023 : public Robot_Base_Msg  {
	Robot_MSG_10170023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170023;
	}
  void set_rand(void) {
  }
};

/*
公会升级发礼包
*/
struct Robot_MSG_10170024 : public Robot_Base_Msg  {
	Robot_MSG_10170024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170024;
	}
  void set_rand(void) {
  }
};

/*
公会清升级CD
*/
struct Robot_MSG_10170025 : public Robot_Base_Msg  {
	int8_t cd_type;
	int32_t building_id;
	Robot_MSG_10170025(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(cd_type);
		w.write_int32(building_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(cd_type)  ||  r.read_int32(building_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170025;
		cd_type = 0;
		building_id = 0;
	}
  void set_rand(void) {
          cd_type = rand()%INT8_MAX;
          building_id = rand()%INT32_MAX;
  }
};

/*
公会发世界公告
*/
struct Robot_MSG_10170026 : public Robot_Base_Msg  {
	Robot_MSG_10170026(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170026;
	}
  void set_rand(void) {
  }
};

/*
修改公会LOGO
*/
struct Robot_MSG_10170027 : public Robot_Base_Msg  {
	int8_t logo_id;
	Robot_MSG_10170027(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(logo_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(logo_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170027;
		logo_id = 0;
	}
  void set_rand(void) {
          logo_id = rand()%INT8_MAX;
  }
};

/*
公会夺权
*/
struct Robot_MSG_10170028 : public Robot_Base_Msg  {
	Robot_MSG_10170028(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170028;
	}
  void set_rand(void) {
  }
};

/*
请求发送公会祝福
*/
struct Robot_MSG_10170029 : public Robot_Base_Msg  {
	int32_t bless_buf_id;
	int64_t target_role_id;
	Robot_MSG_10170029(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(bless_buf_id);
		w.write_int64(target_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(bless_buf_id)  ||  r.read_int64(target_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170029;
		bless_buf_id = 0;
		target_role_id = 0;
	}
  void set_rand(void) {
          bless_buf_id = rand()%INT32_MAX;
          target_role_id = rand()%INT64_MAX;
  }
};

/*
请求研究公会技能
*/
struct Robot_MSG_10170030 : public Robot_Base_Msg  {
	int32_t skill_id;
	Robot_MSG_10170030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170030;
		skill_id = 0;
	}
  void set_rand(void) {
          skill_id = rand()%INT32_MAX;
  }
};

/*
请求学习公会技能
*/
struct Robot_MSG_10170031 : public Robot_Base_Msg  {
	int32_t skill_id;
	Robot_MSG_10170031(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170031;
		skill_id = 0;
	}
  void set_rand(void) {
          skill_id = rand()%INT32_MAX;
  }
};

/*
公会抽奖
*/
struct Robot_MSG_10170032 : public Robot_Base_Msg  {
	Robot_MSG_10170032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170032;
	}
  void set_rand(void) {
  }
};

/*
公会领取抽奖物品
*/
struct Robot_MSG_10170033 : public Robot_Base_Msg  {
	Robot_MSG_10170033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170033;
	}
  void set_rand(void) {
  }
};

/*
公会建筑升级
*/
struct Robot_MSG_10170034 : public Robot_Base_Msg  {
	int32_t building_id;
	Robot_MSG_10170034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(building_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(building_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170034;
		building_id = 0;
	}
  void set_rand(void) {
          building_id = rand()%INT32_MAX;
  }
};

/*
进入公会战
*/
struct Robot_MSG_10170035 : public Robot_Base_Msg  {
	Robot_MSG_10170035(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170035;
	}
  void set_rand(void) {
  }
};

/*
请求公会战面板信息
*/
struct Robot_MSG_10170036 : public Robot_Base_Msg  {
	Robot_MSG_10170036(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170036;
	}
  void set_rand(void) {
  }
};

/*
请求公会战面板结点信息
index:16进八（0-7），8进4（8-11），4进2（12-13），2进1（14）
*/
struct Robot_MSG_10170037 : public Robot_Base_Msg  {
	int8_t index;
	Robot_MSG_10170037(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170037;
		index = 0;
	}
  void set_rand(void) {
          index = rand()%INT8_MAX;
  }
};

/*
进入公会战
*/
struct Robot_MSG_10270000 : public Robot_Base_Msg  {
	Robot_MSG_10270000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10270000;
	}
  void set_rand(void) {
  }
};

/*
公会战请求治疗
*/
struct Robot_MSG_10270001 : public Robot_Base_Msg  {
	Robot_MSG_10270001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10270001;
	}
  void set_rand(void) {
  }
};

/*
请求公会战奖励列表
*/
struct Robot_MSG_10170038 : public Robot_Base_Msg  {
	Robot_MSG_10170038(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170038;
	}
  void set_rand(void) {
  }
};

/*
请求公会战奖励分配
*/
struct Robot_MSG_10170039 : public Robot_Base_Msg  {
	Reward_Info_Gang_War award;
	Robot_MSG_10170039(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		award.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( award.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170039;
		award.reset();
	}
  void set_rand(void) {
  }
};

/*
公会战请求决斗
*/
struct Robot_MSG_10270002 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10270002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10270002;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
同意公会战决斗
*/
struct Robot_MSG_10270004 : public Robot_Base_Msg  {
	int8_t type;
	int64_t role_id;
	Robot_MSG_10270004(void) { reset(); };

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
		msg_id = 10270004;
		type = 0;
		role_id = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          role_id = rand()%INT64_MAX;
  }
};

/*
公会商店购买道具
*/
struct Robot_MSG_10170041 : public Robot_Base_Msg  {
	int32_t item_id;
	int32_t amount;
	Robot_MSG_10170041(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(amount);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(amount)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10170041;
		item_id = 0;
		amount = 0;
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
          amount = rand()%INT32_MAX;
  }
};

/*
公会战抽签
*/
struct Robot_MSG_10170042 : public Robot_Base_Msg  {
	Robot_MSG_10170042(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170042;
	}
  void set_rand(void) {
  }
};

/*
公会战领取排名奖励
*/
struct Robot_MSG_10170043 : public Robot_Base_Msg  {
	Robot_MSG_10170043(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170043;
	}
  void set_rand(void) {
  }
};

/*
公会战排名奖励列表
*/
struct Robot_MSG_10170044 : public Robot_Base_Msg  {
	Robot_MSG_10170044(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170044;
	}
  void set_rand(void) {
  }
};

/*
请求公会战公会排名
*/
struct Robot_MSG_10170045 : public Robot_Base_Msg  {
	Robot_MSG_10170045(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170045;
	}
  void set_rand(void) {
  }
};

/*
开始修行
*/
struct Robot_MSG_10170047 : public Robot_Base_Msg  {
	Robot_MSG_10170047(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170047;
	}
  void set_rand(void) {
  }
};

/*
结束修行
*/
struct Robot_MSG_10170048 : public Robot_Base_Msg  {
	Robot_MSG_10170048(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170048;
	}
  void set_rand(void) {
  }
};

/*
加速修行
*/
struct Robot_MSG_10170049 : public Robot_Base_Msg  {
	Robot_MSG_10170049(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10170049;
	}
  void set_rand(void) {
  }
};

/*
获取炼金面板信息
*/
struct Robot_MSG_10152000 : public Robot_Base_Msg  {
	uint8_t mtype;
	Robot_MSG_10152000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(mtype);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(mtype)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10152000;
		mtype = 0;
	}
  void set_rand(void) {
          mtype = rand()%INT8_MAX;
  }
};

/*
请求炼金
*/
struct Robot_MSG_10152001 : public Robot_Base_Msg  {
	uint8_t tabs;
	uint8_t pay_type;
	uint8_t mtype;
	Robot_MSG_10152001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(tabs);
		w.write_uint8(pay_type);
		w.write_uint8(mtype);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(tabs)  ||  r.read_uint8(pay_type)  ||  r.read_uint8(mtype)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10152001;
		tabs = 0;
		pay_type = 0;
		mtype = 0;
	}
  void set_rand(void) {
          tabs = rand()%INT8_MAX;
          pay_type = rand()%INT8_MAX;
          mtype = rand()%INT8_MAX;
  }
};

/*
获取符文信息
*/
struct Robot_MSG_10103000 : public Robot_Base_Msg  {
	Robot_MSG_10103000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10103000;
	}
  void set_rand(void) {
  }
};

/*
符文激活
*/
struct Robot_MSG_10103001 : public Robot_Base_Msg  {
	int32_t part;
	int32_t hole_index;
	int8_t auto_buy;
	Robot_MSG_10103001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(part);
		w.write_int32(hole_index);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(part)  ||  r.read_int32(hole_index)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103001;
		part = 0;
		hole_index = 0;
		auto_buy = 0;
	}
  void set_rand(void) {
          part = rand()%INT32_MAX;
          hole_index = rand()%INT32_MAX;
          auto_buy = rand()%INT8_MAX;
  }
};

/*
符文充能
*/
struct Robot_MSG_10103002 : public Robot_Base_Msg  {
	int32_t part;
	int32_t hole_index;
	int8_t is_bat;
	int8_t auto_buy;
	Robot_MSG_10103002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(part);
		w.write_int32(hole_index);
		w.write_int8(is_bat);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(part)  ||  r.read_int32(hole_index)  ||  r.read_int8(is_bat)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103002;
		part = 0;
		hole_index = 0;
		is_bat = 0;
		auto_buy = 0;
	}
  void set_rand(void) {
          part = rand()%INT32_MAX;
          hole_index = rand()%INT32_MAX;
          is_bat = rand()%INT8_MAX;
          auto_buy = rand()%INT8_MAX;
  }
};

/*
获取竞技场本服排行
*/
struct Robot_MSG_10100400 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100400;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
获取竞技场信息
*/
struct Robot_MSG_10100405 : public Robot_Base_Msg  {
	Robot_MSG_10100405(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100405;
	}
  void set_rand(void) {
  }
};

/*
关闭竞技场面板
*/
struct Robot_MSG_10100406 : public Robot_Base_Msg  {
	Robot_MSG_10100406(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100406;
	}
  void set_rand(void) {
  }
};

/*
请求匹配竞技场战斗
*/
struct Robot_MSG_10100410 : public Robot_Base_Msg  {
	Robot_MSG_10100410(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100410;
	}
  void set_rand(void) {
  }
};

/*
请求竞技场购买付费挑战次数
*/
struct Robot_MSG_10100420 : public Robot_Base_Msg  {
	uint8_t buy_type;
	Robot_MSG_10100420(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(buy_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(buy_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100420;
		buy_type = 0;
	}
  void set_rand(void) {
          buy_type = rand()%INT8_MAX;
  }
};

/*
取消匹配竞技场战斗
*/
struct Robot_MSG_10100411 : public Robot_Base_Msg  {
	Robot_MSG_10100411(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100411;
	}
  void set_rand(void) {
  }
};

/*
领取竞技场3天或者7天奖励
*/
struct Robot_MSG_10100401 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10100401(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100401;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
获取竞技场玩家奖励信息
*/
struct Robot_MSG_10100402 : public Robot_Base_Msg  {
	Robot_MSG_10100402(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100402;
	}
  void set_rand(void) {
  }
};

/*
请求重置竞技场cd
*/
struct Robot_MSG_10100407 : public Robot_Base_Msg  {
	Robot_MSG_10100407(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100407;
	}
  void set_rand(void) {
  }
};

/*
请求竞技场匹配
*/
struct Robot_MSG_10100412 : public Robot_Base_Msg  {
	Robot_MSG_10100412(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100412;
	}
  void set_rand(void) {
  }
};

/*
请求竞技场战斗
*/
struct Robot_MSG_10100413 : public Robot_Base_Msg  {
	int8_t index;
	Robot_MSG_10100413(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100413;
		index = 0;
	}
  void set_rand(void) {
          index = rand()%INT8_MAX;
  }
};

/*
请求竞技场刷新匹配
*/
struct Robot_MSG_10100414 : public Robot_Base_Msg  {
	Robot_MSG_10100414(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100414;
	}
  void set_rand(void) {
  }
};

/*
竞技场查看人物面板
*/
struct Robot_MSG_10100415 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t index;
	Robot_MSG_10100415(void) { reset(); };

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
		msg_id = 10100415;
		role_id = 0;
		index = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          index = rand()%INT8_MAX;
  }
};

/*
请求竞技场面板信息
*/
struct Robot_MSG_10100408 : public Robot_Base_Msg  {
	Robot_MSG_10100408(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100408;
	}
  void set_rand(void) {
  }
};

/*
竞技场膜拜
*/
struct Robot_MSG_10100416 : public Robot_Base_Msg  {
	int64_t role_id;
	int8_t worship_type;
	Robot_MSG_10100416(void) { reset(); };

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
		msg_id = 10100416;
		role_id = 0;
		worship_type = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          worship_type = rand()%INT8_MAX;
  }
};

/*
竞技场打开面板后刷新
*/
struct Robot_MSG_10100417 : public Robot_Base_Msg  {
	Robot_MSG_10100417(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100417;
	}
  void set_rand(void) {
  }
};

/*
请求新手引导
*/
struct Robot_MSG_10104000 : public Robot_Base_Msg  {
	uint16_t id;
	Robot_MSG_10104000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10104000;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT16_MAX;
  }
};

/*
请求新手引导设置
*/
struct Robot_MSG_10104001 : public Robot_Base_Msg  {
	uint16_t guide_id;
	uint8_t step;
	Robot_MSG_10104001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(guide_id);
		w.write_uint8(step);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(guide_id)  ||  r.read_uint8(step)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10104001;
		guide_id = 0;
		step = 0;
	}
  void set_rand(void) {
          guide_id = rand()%INT16_MAX;
          step = rand()%INT8_MAX;
  }
};

/*
请求保存已播放的剧情
备注：客户端先播放剧情，然后，发这条消息给服务端用来存库
*/
struct Robot_MSG_10104002 : public Robot_Base_Msg  {
	uint32_t plot_id;
	Robot_MSG_10104002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(plot_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(plot_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10104002;
		plot_id = 0;
	}
  void set_rand(void) {
          plot_id = rand()%INT32_MAX;
  }
};

/*
请求设置任务引导进度
*/
struct Robot_MSG_10104003 : public Robot_Base_Msg  {
	uint32_t guide_id;
	Robot_MSG_10104003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(guide_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(guide_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10104003;
		guide_id = 0;
	}
  void set_rand(void) {
          guide_id = rand()%INT32_MAX;
  }
};

/*
请求获取排行榜列表
*/
struct Robot_MSG_10171000 : public Robot_Base_Msg  {
	int32_t type;
	Robot_MSG_10171000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10171000;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT32_MAX;
  }
};

/*
请求排行榜人物信息
*/
struct Robot_MSG_10171001 : public Robot_Base_Msg  {
	uint32_t rank;
	int32_t type;
	Robot_MSG_10171001(void) { reset(); };

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
		msg_id = 10171001;
		rank = 0;
		type = 0;
	}
  void set_rand(void) {
          rank = rand()%INT32_MAX;
          type = rand()%INT32_MAX;
  }
};

/*
请求排行榜装备信息
*/
struct Robot_MSG_10171002 : public Robot_Base_Msg  {
	uint32_t rank;
	int32_t type;
	int64_t roleId;
	uint32_t career;
	Robot_MSG_10171002(void) { reset(); };

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
		msg_id = 10171002;
		rank = 0;
		type = 0;
		roleId = 0;
		career = 0;
	}
  void set_rand(void) {
          rank = rand()%INT32_MAX;
          type = rand()%INT32_MAX;
          roleId = rand()%INT64_MAX;
          career = rand()%INT32_MAX;
  }
};

/*
请求排行榜英雄信息
*/
struct Robot_MSG_10171003 : public Robot_Base_Msg  {
	uint32_t rank;
	int32_t type;
	Robot_MSG_10171003(void) { reset(); };

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
		msg_id = 10171003;
		rank = 0;
		type = 0;
	}
  void set_rand(void) {
          rank = rand()%INT32_MAX;
          type = rand()%INT32_MAX;
  }
};

/*
购买体力
*/
struct Robot_MSG_10120000 : public Robot_Base_Msg  {
	int8_t bind_gold_first;
	Robot_MSG_10120000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(bind_gold_first);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(bind_gold_first)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120000;
		bind_gold_first = 0;
	}
  void set_rand(void) {
          bind_gold_first = rand()%INT8_MAX;
  }
};

/*
请求体力信息
*/
struct Robot_MSG_10120001 : public Robot_Base_Msg  {
	Robot_MSG_10120001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120001;
	}
  void set_rand(void) {
  }
};

/*
请求小喇叭聊天
*/
struct Robot_MSG_10120002 : public Robot_Base_Msg  {
	std::vector<Chat_Content> content;
	Robot_MSG_10120002(void) { reset(); };

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
		msg_id = 10120002;
		content.clear();
	}
  void set_rand(void) {
  }
};

/*
请求获取系统设置
*/
struct Robot_MSG_10120003 : public Robot_Base_Msg  {
	Robot_MSG_10120003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120003;
	}
  void set_rand(void) {
  }
};

/*
请求更改系统设置
*/
struct Robot_MSG_10120004 : public Robot_Base_Msg  {
	uint8_t is_open_music;
	uint8_t music_num;
	uint8_t is_open_voice;
	uint8_t voice_num;
	uint8_t is_hide_role_model;
	uint8_t is_hide_hero_model;
	uint8_t is_hide_title;
	uint32_t player_count;
	uint8_t is_total_open;
	uint8_t is_skip_plot;
	Robot_MSG_10120004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_open_music);
		w.write_uint8(music_num);
		w.write_uint8(is_open_voice);
		w.write_uint8(voice_num);
		w.write_uint8(is_hide_role_model);
		w.write_uint8(is_hide_hero_model);
		w.write_uint8(is_hide_title);
		w.write_uint32(player_count);
		w.write_uint8(is_total_open);
		w.write_uint8(is_skip_plot);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_open_music)  ||  r.read_uint8(music_num)  ||  r.read_uint8(is_open_voice)  ||  r.read_uint8(voice_num)  ||  r.read_uint8(is_hide_role_model)  ||  r.read_uint8(is_hide_hero_model)  ||  r.read_uint8(is_hide_title)  ||  r.read_uint32(player_count)  ||  r.read_uint8(is_total_open)  ||  r.read_uint8(is_skip_plot)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120004;
		is_open_music = 0;
		music_num = 0;
		is_open_voice = 0;
		voice_num = 0;
		is_hide_role_model = 0;
		is_hide_hero_model = 0;
		is_hide_title = 0;
		player_count = 0;
		is_total_open = 0;
		is_skip_plot = 0;
	}
  void set_rand(void) {
          is_open_music = rand()%INT8_MAX;
          music_num = rand()%INT8_MAX;
          is_open_voice = rand()%INT8_MAX;
          voice_num = rand()%INT8_MAX;
          is_hide_role_model = rand()%INT8_MAX;
          is_hide_hero_model = rand()%INT8_MAX;
          is_hide_title = rand()%INT8_MAX;
          player_count = rand()%INT32_MAX;
          is_total_open = rand()%INT8_MAX;
          is_skip_plot = rand()%INT8_MAX;
  }
};

/*
获取玩家觉醒等级
*/
struct Robot_MSG_10120020 : public Robot_Base_Msg  {
	Robot_MSG_10120020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120020;
	}
  void set_rand(void) {
  }
};

/*
玩家觉醒等级升级
*/
struct Robot_MSG_10120021 : public Robot_Base_Msg  {
	Robot_MSG_10120021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120021;
	}
  void set_rand(void) {
  }
};

/*
获取玩家神器信息
*/
struct Robot_MSG_10120030 : public Robot_Base_Msg  {
	Robot_MSG_10120030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120030;
	}
  void set_rand(void) {
  }
};

/*
请求领取等级礼包
*/
struct Robot_MSG_10120040 : public Robot_Base_Msg  {
	int32_t gift_lvl;
	Robot_MSG_10120040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(gift_lvl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(gift_lvl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120040;
		gift_lvl = 0;
	}
  void set_rand(void) {
          gift_lvl = rand()%INT32_MAX;
  }
};

/*
请求已领过的等级礼包
*/
struct Robot_MSG_10120041 : public Robot_Base_Msg  {
	Robot_MSG_10120041(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120041;
	}
  void set_rand(void) {
  }
};

/*
请求送花
*/
struct Robot_MSG_10120050 : public Robot_Base_Msg  {
	uint32_t item_id;
	std::string role_name;
	std::string msg;
	Robot_MSG_10120050(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(item_id);
		w.write_string(role_name);
		w.write_string(msg);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(item_id)  ||  r.read_string(role_name)  ||  r.read_string(msg)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120050;
		item_id = 0;
		role_name.clear();
		msg.clear();
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
  }
};

/*
请求神秘商店信息
*/
struct Robot_MSG_10120010 : public Robot_Base_Msg  {
	Robot_MSG_10120010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120010;
	}
  void set_rand(void) {
  }
};

/*
请求神秘商店购买物品
*/
struct Robot_MSG_10120011 : public Robot_Base_Msg  {
	int8_t index;
	Robot_MSG_10120011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120011;
		index = 0;
	}
  void set_rand(void) {
          index = rand()%INT8_MAX;
  }
};

/*
请求查看鲜花
*/
struct Robot_MSG_10120051 : public Robot_Base_Msg  {
	Robot_MSG_10120051(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120051;
	}
  void set_rand(void) {
  }
};

/*
请求发送你提我改
*/
struct Robot_MSG_10120052 : public Robot_Base_Msg  {
	uint8_t type;
	std::string content;
	Robot_MSG_10120052(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_string(content);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_string(content)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120052;
		type = 0;
		content.clear();
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
获取玩家功勋信息
*/
struct Robot_MSG_10120080 : public Robot_Base_Msg  {
	Robot_MSG_10120080(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120080;
	}
  void set_rand(void) {
  }
};

/*
请求打开首充面板
*/
struct Robot_MSG_10120090 : public Robot_Base_Msg  {
	Robot_MSG_10120090(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120090;
	}
  void set_rand(void) {
  }
};

/*
请求领取首充礼包
*/
struct Robot_MSG_10120091 : public Robot_Base_Msg  {
	Robot_MSG_10120091(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120091;
	}
  void set_rand(void) {
  }
};

/*
请求打开充值返利面板
*/
struct Robot_MSG_10120092 : public Robot_Base_Msg  {
	Robot_MSG_10120092(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120092;
	}
  void set_rand(void) {
  }
};

/*
请求领取充值返利奖励
*/
struct Robot_MSG_10120093 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10120093(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120093;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求充值返利是否可领取
*/
struct Robot_MSG_10120094 : public Robot_Base_Msg  {
	Robot_MSG_10120094(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120094;
	}
  void set_rand(void) {
  }
};

/*
新手引导，剧情中采集道具
备注：如果背包中已有足够数量的道具不再增加，否则，增加道具到背包中
*/
struct Robot_MSG_10120095 : public Robot_Base_Msg  {
	uint32_t item_id;
	uint32_t item_num;
	Robot_MSG_10120095(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(item_id);
		w.write_uint32(item_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(item_id)  ||  r.read_uint32(item_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120095;
		item_id = 0;
		item_num = 0;
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
          item_num = rand()%INT32_MAX;
  }
};

/*
请求发送擂台结果到公会聊天
*/
struct Robot_MSG_10120096 : public Robot_Base_Msg  {
	std::string role_name;
	int64_t role_id;
	Robot_MSG_10120096(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120096;
		role_name.clear();
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
请求调查问卷信息
*/
struct Robot_MSG_10120097 : public Robot_Base_Msg  {
	Robot_MSG_10120097(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120097;
	}
  void set_rand(void) {
  }
};

/*
请求提交调查问卷信息
*/
struct Robot_MSG_10120098 : public Robot_Base_Msg  {
	uint8_t qid;
	std::vector<Questionnaire_Answer_Info> ans;
	Robot_MSG_10120098(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(qid);
		uint16_t __ans_vec_size = ans.size();
		w.write_uint16(__ans_vec_size);
		for(uint16_t i = 0; i < __ans_vec_size; ++i) {
				ans[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(qid)  )
				return -1;
 		uint16_t __ans_vec_size;
		if(r.read_uint16(__ans_vec_size)  )
				return -1;
		ans.reserve(__ans_vec_size);
		for(uint16_t i = 0; i < __ans_vec_size; ++i) {
				Questionnaire_Answer_Info v;
				if(v.deserialize(r))
						return -1;
				ans.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10120098;
		qid = 0;
		ans.clear();
	}
  void set_rand(void) {
          qid = rand()%INT8_MAX;
  }
};

/*
请求打开邀请奖励信息
*/
struct Robot_MSG_10120060 : public Robot_Base_Msg  {
	Robot_MSG_10120060(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120060;
	}
  void set_rand(void) {
  }
};

/*
请求领取邀请奖励信息
*/
struct Robot_MSG_10120061 : public Robot_Base_Msg  {
	uint8_t type;
	Robot_MSG_10120061(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120061;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
请求每月英雄信息
*/
struct Robot_MSG_10120070 : public Robot_Base_Msg  {
	Robot_MSG_10120070(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120070;
	}
  void set_rand(void) {
  }
};

/*
请求打开二次(全民)充值面板
*/
struct Robot_MSG_10120099 : public Robot_Base_Msg  {
	Robot_MSG_10120099(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120099;
	}
  void set_rand(void) {
  }
};

/*
请求领取二次(全民)充值面板
*/
struct Robot_MSG_10120100 : public Robot_Base_Msg  {
	Robot_MSG_10120100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120100;
	}
  void set_rand(void) {
  }
};

/*
请求打开限时充值面板
*/
struct Robot_MSG_10120101 : public Robot_Base_Msg  {
	Robot_MSG_10120101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120101;
	}
  void set_rand(void) {
  }
};

/*
请求领取限时充值面板
*/
struct Robot_MSG_10120102 : public Robot_Base_Msg  {
	Robot_MSG_10120102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10120102;
	}
  void set_rand(void) {
  }
};

/*
剧情数群加1
*/
struct Robot_MSG_10120110 : public Robot_Base_Msg  {
	int8_t type;
	Robot_MSG_10120110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10120110;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
物品兑换
*/
struct Robot_MSG_10100500 : public Robot_Base_Msg  {
	int16_t type;
	int32_t item_id;
	int32_t amount;
	Robot_MSG_10100500(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(type);
		w.write_int32(item_id);
		w.write_int32(amount);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(type)  ||  r.read_int32(item_id)  ||  r.read_int32(amount)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100500;
		type = 0;
		item_id = 0;
		amount = 0;
	}
  void set_rand(void) {
          type = rand()%INT16_MAX;
          item_id = rand()%INT32_MAX;
          amount = rand()%INT32_MAX;
  }
};

/*
获取兑换物品信息
*/
struct Robot_MSG_10100501 : public Robot_Base_Msg  {
	int16_t type;
	Robot_MSG_10100501(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100501;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT16_MAX;
  }
};

/*
技能消耗品兑换
*/
struct Robot_MSG_10100502 : public Robot_Base_Msg  {
	int32_t id;
	Robot_MSG_10100502(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100502;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求日常信息
*/
struct Robot_MSG_10106000 : public Robot_Base_Msg  {
	Robot_MSG_10106000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106000;
	}
  void set_rand(void) {
  }
};

/*
请求活跃度信息
*/
struct Robot_MSG_10106001 : public Robot_Base_Msg  {
	Robot_MSG_10106001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106001;
	}
  void set_rand(void) {
  }
};

/*
请求领取活跃度礼包
*/
struct Robot_MSG_10106002 : public Robot_Base_Msg  {
	uint8_t type;
	uint32_t id;
	Robot_MSG_10106002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106002;
		type = 0;
		id = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          id = rand()%INT32_MAX;
  }
};

/*
请求当天开启的活动
*/
struct Robot_MSG_10106003 : public Robot_Base_Msg  {
	Robot_MSG_10106003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106003;
	}
  void set_rand(void) {
  }
};

/*
请求实力提升列表分数
*/
struct Robot_MSG_10106004 : public Robot_Base_Msg  {
	Robot_MSG_10106004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106004;
	}
  void set_rand(void) {
  }
};

/*
请求资源不足获得方式状态
*/
struct Robot_MSG_10106005 : public Robot_Base_Msg  {
	std::vector<uint32_t> get_type;
	std::string tip_name;
	int32_t goodsId;
	uint8_t isAuto;
	Robot_MSG_10106005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __get_type_vec_size = get_type.size();
		w.write_uint16(__get_type_vec_size);
		for(uint16_t i = 0; i < __get_type_vec_size; ++i) {
				w.write_uint32(get_type[i]);
		}

		w.write_string(tip_name);
		w.write_int32(goodsId);
		w.write_uint8(isAuto);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __get_type_vec_size;
		if(r.read_uint16(__get_type_vec_size)  )
				return -1;
		get_type.reserve(__get_type_vec_size);
		for(uint16_t i = 0; i < __get_type_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				get_type.push_back(v);
		}

		if( r.read_string(tip_name)  ||  r.read_int32(goodsId)  ||  r.read_uint8(isAuto)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106005;
		get_type.clear();
		tip_name.clear();
		goodsId = 0;
		isAuto = 0;
	}
  void set_rand(void) {
          goodsId = rand()%INT32_MAX;
          isAuto = rand()%INT8_MAX;
  }
};

/*
请求签到
*/
struct Robot_MSG_10106006 : public Robot_Base_Msg  {
	Robot_MSG_10106006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106006;
	}
  void set_rand(void) {
  }
};

/*
请求战场排名信息
*/
struct Robot_MSG_10211001 : public Robot_Base_Msg  {
	Robot_MSG_10211001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10211001;
	}
  void set_rand(void) {
  }
};

/*
请求战场治疗
*/
struct Robot_MSG_10211002 : public Robot_Base_Msg  {
	Robot_MSG_10211002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10211002;
	}
  void set_rand(void) {
  }
};

/*
请求加入战场
*/
struct Robot_MSG_10111000 : public Robot_Base_Msg  {
	Robot_MSG_10111000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10111000;
	}
  void set_rand(void) {
  }
};

/*
请求战场进入次数
*/
struct Robot_MSG_10111001 : public Robot_Base_Msg  {
	Robot_MSG_10111001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10111001;
	}
  void set_rand(void) {
  }
};

/*
请求领取战场结算奖励
*/
struct Robot_MSG_10111003 : public Robot_Base_Msg  {
	Robot_MSG_10111003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10111003;
	}
  void set_rand(void) {
  }
};

/*
请求建造新的建筑
*/
struct Robot_MSG_10180000 : public Robot_Base_Msg  {
	uint32_t id;
	uint8_t type;
	uint8_t consume_type;
	Robot_MSG_10180000(void) { reset(); };

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
		msg_id = 10180000;
		id = 0;
		type = 0;
		consume_type = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
          consume_type = rand()%INT8_MAX;
  }
};

/*
请求升级建筑
*/
struct Robot_MSG_10180001 : public Robot_Base_Msg  {
	uint32_t id;
	uint8_t type;
	Robot_MSG_10180001(void) { reset(); };

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
		msg_id = 10180001;
		id = 0;
		type = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求加速建造或者升级建筑
备注： 如果不能加速或者加速失败，返回80000001，带有错误码 。
          如果加速成功，返回  82300002 。
*/
struct Robot_MSG_10180002 : public Robot_Base_Msg  {
	uint32_t id;
	uint8_t type;
	Robot_MSG_10180002(void) { reset(); };

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
		msg_id = 10180002;
		id = 0;
		type = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求取消建造或者升级建筑
*/
struct Robot_MSG_10180003 : public Robot_Base_Msg  {
	uint32_t id;
	uint8_t type;
	Robot_MSG_10180003(void) { reset(); };

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
		msg_id = 10180003;
		id = 0;
		type = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求收获资源
*/
struct Robot_MSG_10180004 : public Robot_Base_Msg  {
	uint32_t id;
	uint8_t type;
	Robot_MSG_10180004(void) { reset(); };

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
		msg_id = 10180004;
		id = 0;
		type = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求踩脚印
备注：如果成功，返回50180005 。如果失败，返回 错误码。
*/
struct Robot_MSG_10180005 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10180005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10180005;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
请求增加龙谷建筑队列
备注：如果成功，返回 50180006。 如果失败，返回错误码。
*/
struct Robot_MSG_10180006 : public Robot_Base_Msg  {
	Robot_MSG_10180006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180006;
	}
  void set_rand(void) {
  }
};

/*
请求获取有龙谷的好友列表
*/
struct Robot_MSG_10180007 : public Robot_Base_Msg  {
	Robot_MSG_10180007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180007;
	}
  void set_rand(void) {
  }
};

/*
请求获取有龙谷的同一个公会成员列表
*/
struct Robot_MSG_10180008 : public Robot_Base_Msg  {
	Robot_MSG_10180008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180008;
	}
  void set_rand(void) {
  }
};

/*
请求孵化龙蛋
*/
struct Robot_MSG_10180009 : public Robot_Base_Msg  {
	Robot_MSG_10180009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180009;
	}
  void set_rand(void) {
  }
};

/*
请求逗龙
*/
struct Robot_MSG_10180010 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10180010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10180010;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
请求获取龙的总信息(培养、进化左侧面板)
*/
struct Robot_MSG_10180011 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10180011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10180011;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
请求培养龙
备注：如果货币不足，返回错误码。 如果货币足够并且扣除成功，不管培养成功还是失败，都返回50180012。
*/
struct Robot_MSG_10180012 : public Robot_Base_Msg  {
	uint8_t type;
	Robot_MSG_10180012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10180012;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
请求增加1点龙心情值
备注：如果成功返回50180013， 如果货币不足，返回错误码。
*/
struct Robot_MSG_10180013 : public Robot_Base_Msg  {
	Robot_MSG_10180013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180013;
	}
  void set_rand(void) {
  }
};

/*
请求进化龙
备注：如果type=1, 并且， 道具不足，返回82300006，通知客户端进化龙所需道具不足，是否愿意消耗货币。
         如果扣除道具或者货币成功， 返回50180014 。
*/
struct Robot_MSG_10180014 : public Robot_Base_Msg  {
	uint8_t type;
	Robot_MSG_10180014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10180014;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
请求掠夺别人龙谷
备注： 点击  好友列表下面的掠夺按钮，发送这条消息
*/
struct Robot_MSG_10180015 : public Robot_Base_Msg  {
	Robot_MSG_10180015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180015;
	}
  void set_rand(void) {
  }
};

/*
请求掠夺匹配
*/
struct Robot_MSG_10180016 : public Robot_Base_Msg  {
	Robot_MSG_10180016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180016;
	}
  void set_rand(void) {
  }
};

/*
请求进攻掠夺龙谷匹配到的人(可能在线、也可能不在线)
备注：请求进攻掠夺龙谷龙谷匹配到的人(可能在线、也可能不在线)
*/
struct Robot_MSG_10200152 : public Robot_Base_Msg  {
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
	int64_t to_uuid;
	std::string to_server_name;
	uint32_t coin;
	Robot_MSG_10200152(void) { reset(); };

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
		msg_id = 10200152;
		uuid = 0;
		role_id = 0;
		server_name.clear();
		role_name.clear();
		to_uuid = 0;
		to_server_name.clear();
		coin = 0;
	}
  void set_rand(void) {
          uuid = rand()%INT64_MAX;
          role_id = rand()%INT64_MAX;
          to_uuid = rand()%INT64_MAX;
          coin = rand()%INT32_MAX;
  }
};

/*
龙谷主人同意应战来掠夺的人
*/
struct Robot_MSG_10200153 : public Robot_Base_Msg  {
	int64_t uuid;
	int64_t role_id;
	Robot_MSG_10200153(void) { reset(); };

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
		msg_id = 10200153;
		uuid = 0;
		role_id = 0;
	}
  void set_rand(void) {
          uuid = rand()%INT64_MAX;
          role_id = rand()%INT64_MAX;
  }
};

/*
请求获取布告栏
*/
struct Robot_MSG_10180017 : public Robot_Base_Msg  {
	Robot_MSG_10180017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180017;
	}
  void set_rand(void) {
  }
};

/*
请求收获掠夺资源
备注：如果成功，返回50180018； 如果失败，返回错误码。
*/
struct Robot_MSG_10180018 : public Robot_Base_Msg  {
	int64_t to_uuid;
	int64_t to_role_id;
	int64_t uuid;
	int64_t role_id;
	uint32_t building_id;
	uint8_t building_type;
	Robot_MSG_10180018(void) { reset(); };

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
		msg_id = 10180018;
		to_uuid = 0;
		to_role_id = 0;
		uuid = 0;
		role_id = 0;
		building_id = 0;
		building_type = 0;
	}
  void set_rand(void) {
          to_uuid = rand()%INT64_MAX;
          to_role_id = rand()%INT64_MAX;
          uuid = rand()%INT64_MAX;
          role_id = rand()%INT64_MAX;
          building_id = rand()%INT32_MAX;
          building_type = rand()%INT8_MAX;
  }
};

/*
请求驱逐沙鼠偷猎者
*/
struct Robot_MSG_10200154 : public Robot_Base_Msg  {
	int64_t role_id;
	int64_t monster_id;
	uint8_t quality;
	Robot_MSG_10200154(void) { reset(); };

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
		msg_id = 10200154;
		role_id = 0;
		monster_id = 0;
		quality = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
          monster_id = rand()%INT64_MAX;
          quality = rand()%INT8_MAX;
  }
};

/*
掠夺战斗胜利，但没有掠夺资源，通知逻辑服任务监听
*/
struct Robot_MSG_10180019 : public Robot_Base_Msg  {
	Robot_MSG_10180019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180019;
	}
  void set_rand(void) {
  }
};

/*
请求批量进化龙
备注：如果type=0,    道具完全足、道具有一部分，但是不足并且货币足、没有道具并且货币足够， 返回82300034，通知客户端批量进化龙， 是否愿意消耗x个道具和x货币。       这种情况下，不返回50180020。
        如果道具、货币都不足，返回错误码。
        如果type=1、2、3，并且足够，返回50180020。
        只有type = 1、2、3，才发 batch_num、ten_times。
*/
struct Robot_MSG_10180020 : public Robot_Base_Msg  {
	uint8_t type;
	uint16_t batch_num;
	uint16_t ten_times;
	Robot_MSG_10180020(void) { reset(); };

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
		msg_id = 10180020;
		type = 0;
		batch_num = 0;
		ten_times = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          batch_num = rand()%INT16_MAX;
          ten_times = rand()%INT16_MAX;
  }
};

/*
请求获取建筑可收获剩余时间
*/
struct Robot_MSG_10180021 : public Robot_Base_Msg  {
	uint32_t id;
	uint8_t type;
	Robot_MSG_10180021(void) { reset(); };

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
		msg_id = 10180021;
		id = 0;
		type = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
获取邀请好友踩脚印的好友列表
*/
struct Robot_MSG_10180022 : public Robot_Base_Msg  {
	Robot_MSG_10180022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180022;
	}
  void set_rand(void) {
  }
};

/*
 邀请好友踩脚印
*/
struct Robot_MSG_10180023 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10180023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10180023;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
邀请全部好友踩脚印
*/
struct Robot_MSG_10180024 : public Robot_Base_Msg  {
	Robot_MSG_10180024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10180024;
	}
  void set_rand(void) {
  }
};

/*
请求开通或续费精灵盟约
*/
struct Robot_MSG_10106100 : public Robot_Base_Msg  {
	uint8_t type;
	uint32_t id;
	Robot_MSG_10106100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106100;
		type = 0;
		id = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          id = rand()%INT32_MAX;
  }
};

/*
请求赠送灵精盟约
*/
struct Robot_MSG_10106101 : public Robot_Base_Msg  {
	uint8_t type;
	uint32_t id;
	std::string role_name;
	Robot_MSG_10106101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint32(id);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint32(id)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106101;
		type = 0;
		id = 0;
		role_name.clear();
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          id = rand()%INT32_MAX;
  }
};

/*
请求开始每天倒计时盟约
*/
struct Robot_MSG_10106102 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106102;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
获取成就列表
*/
struct Robot_MSG_10100800 : public Robot_Base_Msg  {
	Robot_MSG_10100800(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100800;
	}
  void set_rand(void) {
  }
};

/*
获取成就奖励
*/
struct Robot_MSG_10100801 : public Robot_Base_Msg  {
	int8_t type;
	int32_t id;
	Robot_MSG_10100801(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100801;
		type = 0;
		id = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          id = rand()%INT32_MAX;
  }
};

/*
穿脱奖章
奖章身上：
40001 - 40008 普通勋章
40009 - 40012 特殊的勋章
40013 - 40014 稀有勋章
*/
struct Robot_MSG_10100802 : public Robot_Base_Msg  {
	int32_t source_index;
	int32_t dst_index;
	Robot_MSG_10100802(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(source_index);
		w.write_int32(dst_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(source_index)  ||  r.read_int32(dst_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100802;
		source_index = 0;
		dst_index = 0;
	}
  void set_rand(void) {
          source_index = rand()%INT32_MAX;
          dst_index = rand()%INT32_MAX;
  }
};

/*
请求开通VIP
*/
struct Robot_MSG_10106200 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106200;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求世界之树信息
*/
struct Robot_MSG_10106201 : public Robot_Base_Msg  {
	Robot_MSG_10106201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106201;
	}
  void set_rand(void) {
  }
};

/*
请求领取世界之树奖励
*/
struct Robot_MSG_10106202 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106202;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求打开VIP转盘获取物品组
*/
struct Robot_MSG_10106203 : public Robot_Base_Msg  {
	Robot_MSG_10106203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106203;
	}
  void set_rand(void) {
  }
};

/*
请求VIP转盘抽奖
*/
struct Robot_MSG_10106204 : public Robot_Base_Msg  {
	uint8_t type;
	Robot_MSG_10106204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106204;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
请求获取VIP转盘奖品
*/
struct Robot_MSG_10106205 : public Robot_Base_Msg  {
	Robot_MSG_10106205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106205;
	}
  void set_rand(void) {
  }
};

/*
请求领取VIP日礼包
*/
struct Robot_MSG_10106206 : public Robot_Base_Msg  {
	Robot_MSG_10106206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106206;
	}
  void set_rand(void) {
  }
};

/*
请求打开VIP日礼包
*/
struct Robot_MSG_10106207 : public Robot_Base_Msg  {
	Robot_MSG_10106207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106207;
	}
  void set_rand(void) {
  }
};

/*
请求坐骑信息
*/
struct Robot_MSG_10100600 : public Robot_Base_Msg  {
	Robot_MSG_10100600(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100600;
	}
  void set_rand(void) {
  }
};

/*
请求坐骑马厩信息
*/
struct Robot_MSG_10100601 : public Robot_Base_Msg  {
	Robot_MSG_10100601(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100601;
	}
  void set_rand(void) {
  }
};

/*
请求坐骑骑乘
*/
struct Robot_MSG_10100602 : public Robot_Base_Msg  {
	int8_t ride;
	int32_t mount_id;
	Robot_MSG_10100602(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(ride);
		w.write_int32(mount_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(ride)  ||  r.read_int32(mount_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100602;
		ride = 0;
		mount_id = 0;
	}
  void set_rand(void) {
          ride = rand()%INT8_MAX;
          mount_id = rand()%INT32_MAX;
  }
};

/*
请求坐骑激活
*/
struct Robot_MSG_10100603 : public Robot_Base_Msg  {
	int32_t mount_id;
	uint8_t type;
	Robot_MSG_10100603(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100603;
		mount_id = 0;
		type = 0;
	}
  void set_rand(void) {
          mount_id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求点击坐骑新组合
*/
struct Robot_MSG_10100604 : public Robot_Base_Msg  {
	int32_t group_id;
	Robot_MSG_10100604(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(group_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(group_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100604;
		group_id = 0;
	}
  void set_rand(void) {
          group_id = rand()%INT32_MAX;
  }
};

/*
请求坐骑培养
*/
struct Robot_MSG_10100605 : public Robot_Base_Msg  {
	int32_t mount_id;
	int8_t type;
	Robot_MSG_10100605(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100605;
		mount_id = 0;
		type = 0;
	}
  void set_rand(void) {
          mount_id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求坐骑上阵
*/
struct Robot_MSG_10100606 : public Robot_Base_Msg  {
	int32_t mount_id;
	int8_t deploy_pos;
	Robot_MSG_10100606(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int8(deploy_pos);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int8(deploy_pos)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100606;
		mount_id = 0;
		deploy_pos = 0;
	}
  void set_rand(void) {
          mount_id = rand()%INT32_MAX;
          deploy_pos = rand()%INT8_MAX;
  }
};

/*
请求坐骑学习技能
*/
struct Robot_MSG_10100608 : public Robot_Base_Msg  {
	int32_t mount_id;
	int32_t prop_id;
	Robot_MSG_10100608(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int32(prop_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int32(prop_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100608;
		mount_id = 0;
		prop_id = 0;
	}
  void set_rand(void) {
          mount_id = rand()%INT32_MAX;
          prop_id = rand()%INT32_MAX;
  }
};

/*
请求坐骑融合生效时间
*/
struct Robot_MSG_10100609 : public Robot_Base_Msg  {
	Robot_MSG_10100609(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100609;
	}
  void set_rand(void) {
  }
};

/*
获取称号信息
*/
struct Robot_MSG_10100900 : public Robot_Base_Msg  {
	Robot_MSG_10100900(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100900;
	}
  void set_rand(void) {
  }
};

/*
称号穿/脱
*/
struct Robot_MSG_10100901 : public Robot_Base_Msg  {
	int32_t title_id;
	int8_t op;
	Robot_MSG_10100901(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(title_id);
		w.write_int8(op);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(title_id)  ||  r.read_int8(op)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10100901;
		title_id = 0;
		op = 0;
	}
  void set_rand(void) {
          title_id = rand()%INT32_MAX;
          op = rand()%INT8_MAX;
  }
};

/*
请求符石系统信息
*/
struct Robot_MSG_10103200 : public Robot_Base_Msg  {
	Robot_MSG_10103200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10103200;
	}
  void set_rand(void) {
  }
};

/*
符石熔炼
*/
struct Robot_MSG_10103201 : public Robot_Base_Msg  {
	int8_t smelter_idx;
	int8_t is_bat;
	Robot_MSG_10103201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(smelter_idx);
		w.write_int8(is_bat);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(smelter_idx)  ||  r.read_int8(is_bat)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103201;
		smelter_idx = 0;
		is_bat = 0;
	}
  void set_rand(void) {
          smelter_idx = rand()%INT8_MAX;
          is_bat = rand()%INT8_MAX;
  }
};

/*
符石出售
*/
struct Robot_MSG_10103202 : public Robot_Base_Msg  {
	int8_t is_bat;
	int8_t under_color;
	int32_t index;
	Robot_MSG_10103202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int8(under_color);
		w.write_int32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int8(under_color)  ||  r.read_int32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103202;
		is_bat = 0;
		under_color = 0;
		index = 0;
	}
  void set_rand(void) {
          is_bat = rand()%INT8_MAX;
          under_color = rand()%INT8_MAX;
          index = rand()%INT32_MAX;
  }
};

/*
符石拾取
*/
struct Robot_MSG_10103203 : public Robot_Base_Msg  {
	int8_t is_bat;
	int32_t index;
	Robot_MSG_10103203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103203;
		is_bat = 0;
		index = 0;
	}
  void set_rand(void) {
          is_bat = rand()%INT8_MAX;
          index = rand()%INT32_MAX;
  }
};

/*
符石锁定
*/
struct Robot_MSG_10103204 : public Robot_Base_Msg  {
	int32_t index;
	int8_t is_lock;
	Robot_MSG_10103204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_int8(is_lock);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_int8(is_lock)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103204;
		index = 0;
		is_lock = 0;
	}
  void set_rand(void) {
          index = rand()%INT32_MAX;
          is_lock = rand()%INT8_MAX;
  }
};

/*
符石合并
*/
struct Robot_MSG_10103205 : public Robot_Base_Msg  {
	int8_t is_bat;
	int32_t index_type;
	int32_t index_from;
	int32_t index_to;
	Robot_MSG_10103205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_bat);
		w.write_int32(index_type);
		w.write_int32(index_from);
		w.write_int32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_bat)  ||  r.read_int32(index_type)  ||  r.read_int32(index_from)  ||  r.read_int32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103205;
		is_bat = 0;
		index_type = 0;
		index_from = 0;
		index_to = 0;
	}
  void set_rand(void) {
          is_bat = rand()%INT8_MAX;
          index_type = rand()%INT32_MAX;
          index_from = rand()%INT32_MAX;
          index_to = rand()%INT32_MAX;
  }
};

/*
符石移动(背包、仓库间移动，不包括拾取、穿脱)
*/
struct Robot_MSG_10103206 : public Robot_Base_Msg  {
	int32_t index_from;
	int32_t index_to;
	Robot_MSG_10103206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index_from);
		w.write_int32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index_from)  ||  r.read_int32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103206;
		index_from = 0;
		index_to = 0;
	}
  void set_rand(void) {
          index_from = rand()%INT32_MAX;
          index_to = rand()%INT32_MAX;
  }
};

/*
符石兑换
*/
struct Robot_MSG_10103207 : public Robot_Base_Msg  {
	int32_t item_id;
	Robot_MSG_10103207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103207;
		item_id = 0;
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
  }
};

/*
请求积分
备注：
以这条主动消息返回：
81080000	ACTIVE_POINTS	主动更新积分
*/
struct Robot_MSG_10103208 : public Robot_Base_Msg  {
	Robot_MSG_10103208(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10103208;
	}
  void set_rand(void) {
  }
};

/*
符石穿脱
*/
struct Robot_MSG_10103209 : public Robot_Base_Msg  {
	int32_t index_from;
	int32_t index_to;
	Robot_MSG_10103209(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index_from);
		w.write_int32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index_from)  ||  r.read_int32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103209;
		index_from = 0;
		index_to = 0;
	}
  void set_rand(void) {
          index_from = rand()%INT32_MAX;
          index_to = rand()%INT32_MAX;
  }
};

/*
请求精华
备注：
以这条主动消息返回：
81090000	ACTIVE_ESSENCE	主动更新精华
*/
struct Robot_MSG_10103210 : public Robot_Base_Msg  {
	Robot_MSG_10103210(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10103210;
	}
  void set_rand(void) {
  }
};

/*
符石精华兑换
*/
struct Robot_MSG_10103211 : public Robot_Base_Msg  {
	int32_t item_id;
	Robot_MSG_10103211(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10103211;
		item_id = 0;
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
  }
};

/*
请求获取在线礼包
*/
struct Robot_MSG_10106300 : public Robot_Base_Msg  {
	Robot_MSG_10106300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106300;
	}
  void set_rand(void) {
  }
};

/*
请求7日登陆奖励信息
*/
struct Robot_MSG_10106301 : public Robot_Base_Msg  {
	Robot_MSG_10106301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106301;
	}
  void set_rand(void) {
  }
};

/*
请求领取7日登陆奖励
*/
struct Robot_MSG_10106302 : public Robot_Base_Msg  {
	uint32_t days;
	Robot_MSG_10106302(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(days);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(days)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106302;
		days = 0;
	}
  void set_rand(void) {
          days = rand()%INT32_MAX;
  }
};

/*
请求领取激活码
*/
struct Robot_MSG_10106303 : public Robot_Base_Msg  {
	std::string code;
	Robot_MSG_10106303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(code);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(code)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106303;
		code.clear();
	}
  void set_rand(void) {
  }
};

/*
请求奖励追回信息
*/
struct Robot_MSG_10106304 : public Robot_Base_Msg  {
	Robot_MSG_10106304(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106304;
	}
  void set_rand(void) {
  }
};

/*
请求追回奖励
*/
struct Robot_MSG_10106305 : public Robot_Base_Msg  {
	uint32_t type;
	uint32_t item_id;
	Robot_MSG_10106305(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(type);
		w.write_uint32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(type)  ||  r.read_uint32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106305;
		type = 0;
		item_id = 0;
	}
  void set_rand(void) {
          type = rand()%INT32_MAX;
          item_id = rand()%INT32_MAX;
  }
};

/*
请求是否领取过收藏有礼
*/
struct Robot_MSG_10106306 : public Robot_Base_Msg  {
	Robot_MSG_10106306(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106306;
	}
  void set_rand(void) {
  }
};

/*
请求收藏添加成功
*/
struct Robot_MSG_10106307 : public Robot_Base_Msg  {
	Robot_MSG_10106307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106307;
	}
  void set_rand(void) {
  }
};

/*
请求领取收藏有礼
*/
struct Robot_MSG_10106308 : public Robot_Base_Msg  {
	Robot_MSG_10106308(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106308;
	}
  void set_rand(void) {
  }
};

/*
请求奖励系统TIP数
*/
struct Robot_MSG_10106309 : public Robot_Base_Msg  {
	Robot_MSG_10106309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106309;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息
*/
struct Robot_MSG_10106400 : public Robot_Base_Msg  {
	Robot_MSG_10106400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106400;
	}
  void set_rand(void) {
  }
};

/*
请求领取VIP俱乐部奖励
*/
struct Robot_MSG_10106401 : public Robot_Base_Msg  {
	uint32_t id;
	uint32_t reward_id;
	Robot_MSG_10106401(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(reward_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(reward_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106401;
		id = 0;
		reward_id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          reward_id = rand()%INT32_MAX;
  }
};

/*
请求领取冲级高手奖励
*/
struct Robot_MSG_10106402 : public Robot_Base_Msg  {
	uint32_t level;
	Robot_MSG_10106402(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106402;
		level = 0;
	}
  void set_rand(void) {
          level = rand()%INT32_MAX;
  }
};

/*
 请求领取套装收集奖励
*/
struct Robot_MSG_10106403 : public Robot_Base_Msg  {
	uint32_t level;
	Robot_MSG_10106403(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106403;
		level = 0;
	}
  void set_rand(void) {
          level = rand()%INT32_MAX;
  }
};

/*
请求领取公会升级奖励
*/
struct Robot_MSG_10106404 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106404(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106404;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求活动信息可领取TIP数
*/
struct Robot_MSG_10106405 : public Robot_Base_Msg  {
	Robot_MSG_10106405(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106405;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-限时购买
*/
struct Robot_MSG_10106420 : public Robot_Base_Msg  {
	Robot_MSG_10106420(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106420;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-vip俱乐部
*/
struct Robot_MSG_10106421 : public Robot_Base_Msg  {
	Robot_MSG_10106421(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106421;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-冲级高手
*/
struct Robot_MSG_10106422 : public Robot_Base_Msg  {
	Robot_MSG_10106422(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106422;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-套装收集
*/
struct Robot_MSG_10106423 : public Robot_Base_Msg  {
	Robot_MSG_10106423(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106423;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-公会升级 
*/
struct Robot_MSG_10106424 : public Robot_Base_Msg  {
	Robot_MSG_10106424(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106424;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-等级排名
*/
struct Robot_MSG_10106425 : public Robot_Base_Msg  {
	Robot_MSG_10106425(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106425;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-竞技场排名
*/
struct Robot_MSG_10106426 : public Robot_Base_Msg  {
	Robot_MSG_10106426(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106426;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-战斗力排名
*/
struct Robot_MSG_10106427 : public Robot_Base_Msg  {
	Robot_MSG_10106427(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106427;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-宝石升级
*/
struct Robot_MSG_10106428 : public Robot_Base_Msg  {
	Robot_MSG_10106428(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106428;
	}
  void set_rand(void) {
  }
};

/*
请求领取宝石升级奖励
*/
struct Robot_MSG_10106406 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106406(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106406;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求领取翅膀升级奖励
*/
struct Robot_MSG_10106407 : public Robot_Base_Msg  {
	uint32_t level;
	Robot_MSG_10106407(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106407;
		level = 0;
	}
  void set_rand(void) {
          level = rand()%INT32_MAX;
  }
};

/*
请求活动信息-翅膀升级
*/
struct Robot_MSG_10106429 : public Robot_Base_Msg  {
	Robot_MSG_10106429(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106429;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-坐骑升级
*/
struct Robot_MSG_10106430 : public Robot_Base_Msg  {
	Robot_MSG_10106430(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106430;
	}
  void set_rand(void) {
  }
};

/*
请求领取坐骑升级奖励
*/
struct Robot_MSG_10106408 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106408(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106408;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求活动信息-上古副本
*/
struct Robot_MSG_10106431 : public Robot_Base_Msg  {
	Robot_MSG_10106431(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106431;
	}
  void set_rand(void) {
  }
};

/*
请求领取上古排行通关奖励
*/
struct Robot_MSG_10106409 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106409(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106409;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求活动信息-竞技场段位
*/
struct Robot_MSG_10106432 : public Robot_Base_Msg  {
	Robot_MSG_10106432(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106432;
	}
  void set_rand(void) {
  }
};

/*
请求领取竞技场段位奖励
*/
struct Robot_MSG_10106410 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106410(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106410;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求活动信息-世界boss
*/
struct Robot_MSG_10106433 : public Robot_Base_Msg  {
	Robot_MSG_10106433(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106433;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-套装收集二段
*/
struct Robot_MSG_10106434 : public Robot_Base_Msg  {
	Robot_MSG_10106434(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106434;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-英雄之门
*/
struct Robot_MSG_10106435 : public Robot_Base_Msg  {
	Robot_MSG_10106435(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106435;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-组队竞技
*/
struct Robot_MSG_10106436 : public Robot_Base_Msg  {
	Robot_MSG_10106436(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106436;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-战场
*/
struct Robot_MSG_10106437 : public Robot_Base_Msg  {
	Robot_MSG_10106437(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106437;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-悬赏
*/
struct Robot_MSG_10106438 : public Robot_Base_Msg  {
	Robot_MSG_10106438(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106438;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-上古副本
*/
struct Robot_MSG_10106439 : public Robot_Base_Msg  {
	Robot_MSG_10106439(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106439;
	}
  void set_rand(void) {
  }
};

/*
请求领取活动奖励(公用1)
*/
struct Robot_MSG_10106411 : public Robot_Base_Msg  {
	uint32_t id;
	uint32_t act_type;
	Robot_MSG_10106411(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(act_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(act_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106411;
		id = 0;
		act_type = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          act_type = rand()%INT32_MAX;
  }
};

/*
请求活动信息-累计附魔X次
*/
struct Robot_MSG_10106440 : public Robot_Base_Msg  {
	Robot_MSG_10106440(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106440;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-累计X个X品质英雄
*/
struct Robot_MSG_10106441 : public Robot_Base_Msg  {
	Robot_MSG_10106441(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106441;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-坐骑进阶X次
*/
struct Robot_MSG_10106442 : public Robot_Base_Msg  {
	Robot_MSG_10106442(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106442;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-合成X张精英卡牌
*/
struct Robot_MSG_10106443 : public Robot_Base_Msg  {
	Robot_MSG_10106443(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106443;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-炼金有礼
*/
struct Robot_MSG_10106444 : public Robot_Base_Msg  {
	Robot_MSG_10106444(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106444;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-活跃度好礼
*/
struct Robot_MSG_10106445 : public Robot_Base_Msg  {
	Robot_MSG_10106445(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106445;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-环任务活动
*/
struct Robot_MSG_10106446 : public Robot_Base_Msg  {
	Robot_MSG_10106446(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106446;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-龙谷掠夺活动
*/
struct Robot_MSG_10106447 : public Robot_Base_Msg  {
	Robot_MSG_10106447(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106447;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-坐骑排名
*/
struct Robot_MSG_10106448 : public Robot_Base_Msg  {
	Robot_MSG_10106448(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106448;
	}
  void set_rand(void) {
  }
};

/*
请求活动信息-翅膀排名
*/
struct Robot_MSG_10106449 : public Robot_Base_Msg  {
	Robot_MSG_10106449(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106449;
	}
  void set_rand(void) {
  }
};

/*
请求领取坐骑排行通关奖励
*/
struct Robot_MSG_10106412 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106412(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106412;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求领取翅膀排行通关奖励
*/
struct Robot_MSG_10106413 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10106413(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106413;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求进入篝火晚会
*/
struct Robot_MSG_10100700 : public Robot_Base_Msg  {
	Robot_MSG_10100700(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10100700;
	}
  void set_rand(void) {
  }
};

/*
请求添加篝火柴火
*/
struct Robot_MSG_10200701 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10200701(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200701;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
请求点燃篝火
*/
struct Robot_MSG_10200702 : public Robot_Base_Msg  {
	int64_t role_id;
	Robot_MSG_10200702(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10200702;
		role_id = 0;
	}
  void set_rand(void) {
          role_id = rand()%INT64_MAX;
  }
};

/*
请求篝火排名
*/
struct Robot_MSG_10200703 : public Robot_Base_Msg  {
	Robot_MSG_10200703(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10200703;
	}
  void set_rand(void) {
  }
};

/*
请求点燃/添柴奖励
备注：请求点燃/添柴奖励
*/
struct Robot_MSG_10200704 : public Robot_Base_Msg  {
	Robot_MSG_10200704(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10200704;
	}
  void set_rand(void) {
  }
};

/*
请求篝火结算奖励
*/
struct Robot_MSG_10200705 : public Robot_Base_Msg  {
	Robot_MSG_10200705(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10200705;
	}
  void set_rand(void) {
  }
};

/*
获取已拥有的时装
*/
struct Robot_MSG_10105001 : public Robot_Base_Msg  {
	Robot_MSG_10105001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10105001;
	}
  void set_rand(void) {
  }
};

/*
改变时装穿戴状态
*/
struct Robot_MSG_10105002 : public Robot_Base_Msg  {
	std::vector<int32_t> fashion_vec;
	Robot_MSG_10105002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __fashion_vec_vec_size = fashion_vec.size();
		w.write_uint16(__fashion_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_vec_vec_size; ++i) {
				w.write_int32(fashion_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __fashion_vec_vec_size;
		if(r.read_uint16(__fashion_vec_vec_size)  )
				return -1;
		fashion_vec.reserve(__fashion_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				fashion_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10105002;
		fashion_vec.clear();
	}
  void set_rand(void) {
  }
};

/*
合成时装
*/
struct Robot_MSG_10105003 : public Robot_Base_Msg  {
	int32_t fashion_id;
	Robot_MSG_10105003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(fashion_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(fashion_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105003;
		fashion_id = 0;
	}
  void set_rand(void) {
          fashion_id = rand()%INT32_MAX;
  }
};

/*
升级时装
*/
struct Robot_MSG_10105004 : public Robot_Base_Msg  {
	int32_t fashion_id;
	int16_t lv;
	std::vector<Item_Id_Amount> item_vec;
	Robot_MSG_10105004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(fashion_id);
		w.write_int16(lv);
		uint16_t __item_vec_vec_size = item_vec.size();
		w.write_uint16(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				item_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(fashion_id)  ||  r.read_int16(lv)  )
				return -1;
 		uint16_t __item_vec_vec_size;
		if(r.read_uint16(__item_vec_vec_size)  )
				return -1;
		item_vec.reserve(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				Item_Id_Amount v;
				if(v.deserialize(r))
						return -1;
				item_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10105004;
		fashion_id = 0;
		lv = 0;
		item_vec.clear();
	}
  void set_rand(void) {
          fashion_id = rand()%INT32_MAX;
          lv = rand()%INT16_MAX;
  }
};

/*
时装传承
*/
struct Robot_MSG_10105005 : public Robot_Base_Msg  {
	int32_t fashion_id;
	int32_t target_id;
	Robot_MSG_10105005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(fashion_id);
		w.write_int32(target_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(fashion_id)  ||  r.read_int32(target_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105005;
		fashion_id = 0;
		target_id = 0;
	}
  void set_rand(void) {
          fashion_id = rand()%INT32_MAX;
          target_id = rand()%INT32_MAX;
  }
};

/*
时装抽卡牌
*/
struct Robot_MSG_10105006 : public Robot_Base_Msg  {
	int8_t pos;
	int8_t type;
	Robot_MSG_10105006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(pos);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(pos)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105006;
		pos = 0;
		type = 0;
	}
  void set_rand(void) {
          pos = rand()%INT8_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
获取翅膀道具
*/
struct Robot_MSG_10105011 : public Robot_Base_Msg  {
	Robot_MSG_10105011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10105011;
	}
  void set_rand(void) {
  }
};

/*
激活翅膀道具
*/
struct Robot_MSG_10105012 : public Robot_Base_Msg  {
	Robot_MSG_10105012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10105012;
	}
  void set_rand(void) {
  }
};

/*
获取翅膀信息
*/
struct Robot_MSG_10105013 : public Robot_Base_Msg  {
	Robot_MSG_10105013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10105013;
	}
  void set_rand(void) {
  }
};

/*
翅膀进阶
*/
struct Robot_MSG_10105014 : public Robot_Base_Msg  {
	int8_t is_auto_buy;
	Robot_MSG_10105014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105014;
		is_auto_buy = 0;
	}
  void set_rand(void) {
          is_auto_buy = rand()%INT8_MAX;
  }
};

/*
自动翅膀进阶
*/
struct Robot_MSG_10105015 : public Robot_Base_Msg  {
	int8_t is_auto_buy;
	Robot_MSG_10105015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105015;
		is_auto_buy = 0;
	}
  void set_rand(void) {
          is_auto_buy = rand()%INT8_MAX;
  }
};

/*
显示/隐藏翅膀avatar
*/
struct Robot_MSG_10105016 : public Robot_Base_Msg  {
	int8_t state;
	Robot_MSG_10105016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105016;
		state = 0;
	}
  void set_rand(void) {
          state = rand()%INT8_MAX;
  }
};

/*
改变时装显示状态
*/
struct Robot_MSG_10105007 : public Robot_Base_Msg  {
	int8_t state;
	Robot_MSG_10105007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105007;
		state = 0;
	}
  void set_rand(void) {
          state = rand()%INT8_MAX;
  }
};

/*
领取返利
*/
struct Robot_MSG_10105017 : public Robot_Base_Msg  {
	int32_t stage;
	Robot_MSG_10105017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(stage);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(stage)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105017;
		stage = 0;
	}
  void set_rand(void) {
          stage = rand()%INT32_MAX;
  }
};

/*
激活翅膀体验
*/
struct Robot_MSG_10105018 : public Robot_Base_Msg  {
	Robot_MSG_10105018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10105018;
	}
  void set_rand(void) {
  }
};

/*
新版获取时装信息
*/
struct Robot_MSG_10105020 : public Robot_Base_Msg  {
	Robot_MSG_10105020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10105020;
	}
  void set_rand(void) {
  }
};

/*
使用时装道具
*/
struct Robot_MSG_10105021 : public Robot_Base_Msg  {
	std::vector<uint32_t> item_idx_vec;
	int8_t wear;
	Robot_MSG_10105021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_idx_vec_vec_size = item_idx_vec.size();
		w.write_uint16(__item_idx_vec_vec_size);
		for(uint16_t i = 0; i < __item_idx_vec_vec_size; ++i) {
				w.write_uint32(item_idx_vec[i]);
		}

		w.write_int8(wear);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __item_idx_vec_vec_size;
		if(r.read_uint16(__item_idx_vec_vec_size)  )
				return -1;
		item_idx_vec.reserve(__item_idx_vec_vec_size);
		for(uint16_t i = 0; i < __item_idx_vec_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				item_idx_vec.push_back(v);
		}

		if( r.read_int8(wear)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105021;
		item_idx_vec.clear();
		wear = 0;
	}
  void set_rand(void) {
          wear = rand()%INT8_MAX;
  }
};

/*
升级时装部位
*/
struct Robot_MSG_10105022 : public Robot_Base_Msg  {
	int8_t part_id;
	int8_t auto_buy;
	Robot_MSG_10105022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(part_id);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(part_id)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10105022;
		part_id = 0;
		auto_buy = 0;
	}
  void set_rand(void) {
          part_id = rand()%INT8_MAX;
          auto_buy = rand()%INT8_MAX;
  }
};

/*
请求拍卖行商品列表
*/
struct Robot_MSG_10107000 : public Robot_Base_Msg  {
	Robot_MSG_10107000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10107000;
	}
  void set_rand(void) {
  }
};

/*
请求增加拍卖商品
*/
struct Robot_MSG_10107001 : public Robot_Base_Msg  {
	uint32_t item_id;
	uint32_t num;
	uint32_t continue_time;
	uint32_t auction_price;
	uint32_t ones_price;
	uint8_t open_ann;
	Robot_MSG_10107001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(item_id);
		w.write_uint32(num);
		w.write_uint32(continue_time);
		w.write_uint32(auction_price);
		w.write_uint32(ones_price);
		w.write_uint8(open_ann);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(item_id)  ||  r.read_uint32(num)  ||  r.read_uint32(continue_time)  ||  r.read_uint32(auction_price)  ||  r.read_uint32(ones_price)  ||  r.read_uint8(open_ann)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10107001;
		item_id = 0;
		num = 0;
		continue_time = 0;
		auction_price = 0;
		ones_price = 0;
		open_ann = 0;
	}
  void set_rand(void) {
          item_id = rand()%INT32_MAX;
          num = rand()%INT32_MAX;
          continue_time = rand()%INT32_MAX;
          auction_price = rand()%INT32_MAX;
          ones_price = rand()%INT32_MAX;
          open_ann = rand()%INT8_MAX;
  }
};

/*
请求下架竞拍商品
*/
struct Robot_MSG_10107002 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10107002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10107002;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求一口价购买商品
*/
struct Robot_MSG_10107003 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10107003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10107003;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求出价竞拍商品
*/
struct Robot_MSG_10107004 : public Robot_Base_Msg  {
	uint32_t id;
	uint32_t price;
	Robot_MSG_10107004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(price);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(price)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10107004;
		id = 0;
		price = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          price = rand()%INT32_MAX;
  }
};

/*
请求打开骑士试炼界面
*/
struct Robot_MSG_10107100 : public Robot_Base_Msg  {
	Robot_MSG_10107100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10107100;
	}
  void set_rand(void) {
  }
};

/*
请求进入战斗
*/
struct Robot_MSG_10107101 : public Robot_Base_Msg  {
	Robot_MSG_10107101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10107101;
	}
  void set_rand(void) {
  }
};

/*
请求打开宝箱
*/
struct Robot_MSG_10107102 : public Robot_Base_Msg  {
	Robot_MSG_10107102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10107102;
	}
  void set_rand(void) {
  }
};

/*
请求领取宝箱奖励
*/
struct Robot_MSG_10107103 : public Robot_Base_Msg  {
	Robot_MSG_10107103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10107103;
	}
  void set_rand(void) {
  }
};

/*
请求重置骑士试炼挑战
*/
struct Robot_MSG_10107104 : public Robot_Base_Msg  {
	Robot_MSG_10107104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10107104;
	}
  void set_rand(void) {
  }
};

/*
获取神器信息
*/
struct Robot_MSG_10101100 : public Robot_Base_Msg  {
	Robot_MSG_10101100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101100;
	}
  void set_rand(void) {
  }
};

/*
上古神器解封
*/
struct Robot_MSG_10101101 : public Robot_Base_Msg  {
	int32_t id;
	Robot_MSG_10101101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101101;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
上古神器条件激活
*/
struct Robot_MSG_10101102 : public Robot_Base_Msg  {
	int32_t id;
	Robot_MSG_10101102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101102;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
上古神器完美宝箱领取
*/
struct Robot_MSG_10101103 : public Robot_Base_Msg  {
	int32_t id;
	Robot_MSG_10101103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101103;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求打开月卡投资页面
*/
struct Robot_MSG_10101200 : public Robot_Base_Msg  {
	Robot_MSG_10101200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101200;
	}
  void set_rand(void) {
  }
};

/*
请求领取月卡投资的收益
*/
struct Robot_MSG_10101201 : public Robot_Base_Msg  {
	uint8_t day;
	Robot_MSG_10101201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(day);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(day)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101201;
		day = 0;
	}
  void set_rand(void) {
          day = rand()%INT8_MAX;
  }
};

/*
请求月卡投资
*/
struct Robot_MSG_10101202 : public Robot_Base_Msg  {
	Robot_MSG_10101202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101202;
	}
  void set_rand(void) {
  }
};

/*
请求打开升级投资页面
*/
struct Robot_MSG_10101203 : public Robot_Base_Msg  {
	Robot_MSG_10101203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101203;
	}
  void set_rand(void) {
  }
};

/*
请求领取升级投资的收益
*/
struct Robot_MSG_10101204 : public Robot_Base_Msg  {
	uint32_t level;
	Robot_MSG_10101204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101204;
		level = 0;
	}
  void set_rand(void) {
          level = rand()%INT32_MAX;
  }
};

/*
请求升级投资
*/
struct Robot_MSG_10101205 : public Robot_Base_Msg  {
	uint32_t money;
	uint8_t type;
	Robot_MSG_10101205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(money);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(money)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101205;
		money = 0;
		type = 0;
	}
  void set_rand(void) {
          money = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
一键领取所有升级投资收益
*/
struct Robot_MSG_10101206 : public Robot_Base_Msg  {
	Robot_MSG_10101206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101206;
	}
  void set_rand(void) {
  }
};

/*
请求获取投资计划的历史记录
*/
struct Robot_MSG_10101207 : public Robot_Base_Msg  {
	Robot_MSG_10101207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10101207;
	}
  void set_rand(void) {
  }
};

/*
请求获取每日奖励
*/
struct Robot_MSG_10201300 : public Robot_Base_Msg  {
	Robot_MSG_10201300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10201300;
	}
  void set_rand(void) {
  }
};

/*
请求每日奖励信息
*/
struct Robot_MSG_10201301 : public Robot_Base_Msg  {
	Robot_MSG_10201301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10201301;
	}
  void set_rand(void) {
  }
};

/*
请求建筑信息(非公会基地)
*/
struct Robot_MSG_10201302 : public Robot_Base_Msg  {
	int32_t point_id;
	int16_t line;
	Robot_MSG_10201302(void) { reset(); };

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
		msg_id = 10201302;
		point_id = 0;
		line = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
          line = rand()%INT16_MAX;
  }
};

/*
请求公会排名
城堡+野兽区+宝箱区 
公会基地
*/
struct Robot_MSG_10201303 : public Robot_Base_Msg  {
	Robot_MSG_10201303(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10201303;
	}
  void set_rand(void) {
  }
};

/*
请求对战信息
*/
struct Robot_MSG_10201304 : public Robot_Base_Msg  {
	int32_t point_id;
	int16_t line;
	Robot_MSG_10201304(void) { reset(); };

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
		msg_id = 10201304;
		point_id = 0;
		line = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
          line = rand()%INT16_MAX;
  }
};

/*
请求公会基地信息
*/
struct Robot_MSG_10201305 : public Robot_Base_Msg  {
	int32_t point_id;
	int16_t line;
	Robot_MSG_10201305(void) { reset(); };

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
		msg_id = 10201305;
		point_id = 0;
		line = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
          line = rand()%INT16_MAX;
  }
};

/*
请求进出战斗队列
*/
struct Robot_MSG_10201306 : public Robot_Base_Msg  {
	int32_t point_id;
	int8_t type;
	Robot_MSG_10201306(void) { reset(); };

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
		msg_id = 10201306;
		point_id = 0;
		type = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
请求援兵信息
*/
struct Robot_MSG_10201307 : public Robot_Base_Msg  {
	int32_t point_id;
	Robot_MSG_10201307(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10201307;
		point_id = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
  }
};

/*
远征换线
*/
struct Robot_MSG_10201308 : public Robot_Base_Msg  {
	int16_t from;
	int16_t to;
	Robot_MSG_10201308(void) { reset(); };

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
		msg_id = 10201308;
		from = 0;
		to = 0;
	}
  void set_rand(void) {
          from = rand()%INT16_MAX;
          to = rand()%INT16_MAX;
  }
};

/*
远征请求宝箱点领取
*/
struct Robot_MSG_10101301 : public Robot_Base_Msg  {
	int32_t point_id;
	Robot_MSG_10101301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101301;
		point_id = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
  }
};

/*
远征打开宝箱区
*/
struct Robot_MSG_10101300 : public Robot_Base_Msg  {
	int32_t point_id;
	Robot_MSG_10101300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10101300;
		point_id = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
  }
};

/*
远征进入待战列表
*/
struct Robot_MSG_10201310 : public Robot_Base_Msg  {
	int32_t point_id;
	int8_t type;
	Robot_MSG_10201310(void) { reset(); };

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
		msg_id = 10201310;
		point_id = 0;
		type = 0;
	}
  void set_rand(void) {
          point_id = rand()%INT32_MAX;
          type = rand()%INT8_MAX;
  }
};

/*
远征伤害排名
*/
struct Robot_MSG_10201309 : public Robot_Base_Msg  {
	Robot_MSG_10201309(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10201309;
	}
  void set_rand(void) {
  }
};

/*
请求远征攻城榜
*/
struct Robot_MSG_10201311 : public Robot_Base_Msg  {
	Robot_MSG_10201311(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10201311;
	}
  void set_rand(void) {
  }
};

/*
领取攻城奖励
*/
struct Robot_MSG_10201312 : public Robot_Base_Msg  {
	Robot_MSG_10201312(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10201312;
	}
  void set_rand(void) {
  }
};

/*
打开面板获取季数
*/
struct Robot_MSG_10108001 : public Robot_Base_Msg  {
	Robot_MSG_10108001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10108001;
	}
  void set_rand(void) {
  }
};

/*
雷神之锤抽奖
*/
struct Robot_MSG_10108002 : public Robot_Base_Msg  {
	int32_t times;
	Robot_MSG_10108002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(times);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10108002;
		times = 0;
	}
  void set_rand(void) {
          times = rand()%INT32_MAX;
  }
};

/*
雷神之锤兑换奖品
*/
struct Robot_MSG_10108003 : public Robot_Base_Msg  {
	int32_t item_index;
	int32_t item_id;
	Robot_MSG_10108003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_index);
		w.write_int32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_index)  ||  r.read_int32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10108003;
		item_index = 0;
		item_id = 0;
	}
  void set_rand(void) {
          item_index = rand()%INT32_MAX;
          item_id = rand()%INT32_MAX;
  }
};

/*
请求使用免答
*/
struct Robot_MSG_10209001 : public Robot_Base_Msg  {
	Robot_MSG_10209001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10209001;
	}
  void set_rand(void) {
  }
};

/*
请求使用双倍
*/
struct Robot_MSG_10209002 : public Robot_Base_Msg  {
	Robot_MSG_10209002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10209002;
	}
  void set_rand(void) {
  }
};

/*
请求获取免答和双倍次数
*/
struct Robot_MSG_10209003 : public Robot_Base_Msg  {
	Robot_MSG_10209003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10209003;
	}
  void set_rand(void) {
  }
};

/*
卡牌镶嵌
*/
struct Robot_MSG_10109002 : public Robot_Base_Msg  {
	uint32_t from_index;
	uint32_t to_index;
	uint8_t loc;
	uint32_t card_id;
	Robot_MSG_10109002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(from_index);
		w.write_uint32(to_index);
		w.write_uint8(loc);
		w.write_uint32(card_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(from_index)  ||  r.read_uint32(to_index)  ||  r.read_uint8(loc)  ||  r.read_uint32(card_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10109002;
		from_index = 0;
		to_index = 0;
		loc = 0;
		card_id = 0;
	}
  void set_rand(void) {
          from_index = rand()%INT32_MAX;
          to_index = rand()%INT32_MAX;
          loc = rand()%INT8_MAX;
          card_id = rand()%INT32_MAX;
  }
};

/*
卡牌升级
*/
struct Robot_MSG_10109003 : public Robot_Base_Msg  {
	uint32_t card_idx;
	std::vector<uint32_t> card_idx_vec;
	Robot_MSG_10109003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(card_idx);
		uint16_t __card_idx_vec_vec_size = card_idx_vec.size();
		w.write_uint16(__card_idx_vec_vec_size);
		for(uint16_t i = 0; i < __card_idx_vec_vec_size; ++i) {
				w.write_uint32(card_idx_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(card_idx)  )
				return -1;
 		uint16_t __card_idx_vec_vec_size;
		if(r.read_uint16(__card_idx_vec_vec_size)  )
				return -1;
		card_idx_vec.reserve(__card_idx_vec_vec_size);
		for(uint16_t i = 0; i < __card_idx_vec_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				card_idx_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10109003;
		card_idx = 0;
		card_idx_vec.clear();
	}
  void set_rand(void) {
          card_idx = rand()%INT32_MAX;
  }
};

/*
出售卡牌
*/
struct Robot_MSG_10109005 : public Robot_Base_Msg  {
	int8_t type;
	int32_t card_index;
	Robot_MSG_10109005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(card_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(card_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10109005;
		type = 0;
		card_index = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          card_index = rand()%INT32_MAX;
  }
};

/*
卡牌打孔
*/
struct Robot_MSG_10109006 : public Robot_Base_Msg  {
	int32_t equip_index;
	int8_t loc;
	Robot_MSG_10109006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(equip_index);
		w.write_int8(loc);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(equip_index)  ||  r.read_int8(loc)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10109006;
		equip_index = 0;
		loc = 0;
	}
  void set_rand(void) {
          equip_index = rand()%INT32_MAX;
          loc = rand()%INT8_MAX;
  }
};

/*
合成卡牌
*/
struct Robot_MSG_10109007 : public Robot_Base_Msg  {
	uint32_t card_index;
	std::vector<uint32_t> card_cost;
	Robot_MSG_10109007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(card_index);
		uint16_t __card_cost_vec_size = card_cost.size();
		w.write_uint16(__card_cost_vec_size);
		for(uint16_t i = 0; i < __card_cost_vec_size; ++i) {
				w.write_uint32(card_cost[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(card_index)  )
				return -1;
 		uint16_t __card_cost_vec_size;
		if(r.read_uint16(__card_cost_vec_size)  )
				return -1;
		card_cost.reserve(__card_cost_vec_size);
		for(uint16_t i = 0; i < __card_cost_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				card_cost.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10109007;
		card_index = 0;
		card_cost.clear();
	}
  void set_rand(void) {
          card_index = rand()%INT32_MAX;
  }
};

/*
获取vip升级卡牌次数
*/
struct Robot_MSG_10109008 : public Robot_Base_Msg  {
	Robot_MSG_10109008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10109008;
	}
  void set_rand(void) {
  }
};

/*
vip升级卡牌
*/
struct Robot_MSG_10109009 : public Robot_Base_Msg  {
	uint32_t card_idx;
	Robot_MSG_10109009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(card_idx);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(card_idx)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10109009;
		card_idx = 0;
	}
  void set_rand(void) {
          card_idx = rand()%INT32_MAX;
  }
};

/*
卡牌升金
*/
struct Robot_MSG_10109010 : public Robot_Base_Msg  {
	uint32_t card_index;
	Robot_MSG_10109010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(card_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(card_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10109010;
		card_index = 0;
	}
  void set_rand(void) {
          card_index = rand()%INT32_MAX;
  }
};

/*
请求领取37会员等级奖励
*/
struct Robot_MSG_10112000 : public Robot_Base_Msg  {
	uint32_t id;
	Robot_MSG_10112000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10112000;
		id = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
  }
};

/*
请求打开37会员等级奖励
*/
struct Robot_MSG_10112001 : public Robot_Base_Msg  {
	int32_t pt_vip;
	Robot_MSG_10112001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(pt_vip);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(pt_vip)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10112001;
		pt_vip = 0;
	}
  void set_rand(void) {
          pt_vip = rand()%INT32_MAX;
  }
};

/*
请求使用特殊技能
*/
struct Robot_MSG_10211100 : public Robot_Base_Msg  {
	int8_t type;
	int8_t icon_id;
	Robot_MSG_10211100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10211100;
		type = 0;
		icon_id = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
          icon_id = rand()%INT8_MAX;
  }
};

/*
请求世界boss公会祝福
*/
struct Robot_MSG_10211101 : public Robot_Base_Msg  {
	int8_t icon_id;
	Robot_MSG_10211101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10211101;
		icon_id = 0;
	}
  void set_rand(void) {
          icon_id = rand()%INT8_MAX;
  }
};

/*
请求世界boss击杀信息
*/
struct Robot_MSG_10111102 : public Robot_Base_Msg  {
	int8_t icon_id;
	Robot_MSG_10111102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10111102;
		icon_id = 0;
	}
  void set_rand(void) {
          icon_id = rand()%INT8_MAX;
  }
};

/*
请求设置世界boss开启时间
*/
struct Robot_MSG_10111103 : public Robot_Base_Msg  {
	int8_t icon_id;
	int8_t hour;
	int8_t minute;
	Robot_MSG_10111103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(icon_id);
		w.write_int8(hour);
		w.write_int8(minute);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(icon_id)  ||  r.read_int8(hour)  ||  r.read_int8(minute)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10111103;
		icon_id = 0;
		hour = 0;
		minute = 0;
	}
  void set_rand(void) {
          icon_id = rand()%INT8_MAX;
          hour = rand()%INT8_MAX;
          minute = rand()%INT8_MAX;
  }
};

/*
请求添加R2邀请邮箱
*/
struct Robot_MSG_10106800 : public Robot_Base_Msg  {
	std::vector<std::string> mails;
	Robot_MSG_10106800(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __mails_vec_size = mails.size();
		w.write_uint16(__mails_vec_size);
		for(uint16_t i = 0; i < __mails_vec_size; ++i) {
				w.write_string(mails[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __mails_vec_size;
		if(r.read_uint16(__mails_vec_size)  )
				return -1;
		mails.reserve(__mails_vec_size);
		for(uint16_t i = 0; i < __mails_vec_size; ++i) {
				std::string v;
				if(r.read_string(v) )
						return -1;
				mails.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10106800;
		mails.clear();
	}
  void set_rand(void) {
  }
};

/*
请求获取R2邀请邮箱信息
*/
struct Robot_MSG_10106801 : public Robot_Base_Msg  {
	Robot_MSG_10106801(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106801;
	}
  void set_rand(void) {
  }
};

/*
请求打开活动兑换商店
*/
struct Robot_MSG_10106900 : public Robot_Base_Msg  {
	Robot_MSG_10106900(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10106900;
	}
  void set_rand(void) {
  }
};

/*
请求兑换活动兑换商城商品
*/
struct Robot_MSG_10106901 : public Robot_Base_Msg  {
	uint32_t id;
	uint32_t num;
	Robot_MSG_10106901(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10106901;
		id = 0;
		num = 0;
	}
  void set_rand(void) {
          id = rand()%INT32_MAX;
          num = rand()%INT32_MAX;
  }
};

/*
打开生活技能界面
*/
struct Robot_MSG_10116000 : public Robot_Base_Msg  {
	Robot_MSG_10116000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10116000;
	}
  void set_rand(void) {
  }
};

/*
学习生活技能
*/
struct Robot_MSG_10116001 : public Robot_Base_Msg  {
	uint8_t type;
	Robot_MSG_10116001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10116001;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
生活技能升级
*/
struct Robot_MSG_10116002 : public Robot_Base_Msg  {
	int32_t skill_id;
	Robot_MSG_10116002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10116002;
		skill_id = 0;
	}
  void set_rand(void) {
          skill_id = rand()%INT32_MAX;
  }
};

/*
晶石精练
*/
struct Robot_MSG_10116003 : public Robot_Base_Msg  {
	std::vector<uint32_t> id_vec;
	Robot_MSG_10116003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __id_vec_vec_size = id_vec.size();
		w.write_uint16(__id_vec_vec_size);
		for(uint16_t i = 0; i < __id_vec_vec_size; ++i) {
				w.write_uint32(id_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __id_vec_vec_size;
		if(r.read_uint16(__id_vec_vec_size)  )
				return -1;
		id_vec.reserve(__id_vec_vec_size);
		for(uint16_t i = 0; i < __id_vec_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				id_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 10116003;
		id_vec.clear();
	}
  void set_rand(void) {
  }
};

/*
卡牌制作
*/
struct Robot_MSG_10116004 : public Robot_Base_Msg  {
	uint8_t type;
	Robot_MSG_10116004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10116004;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT8_MAX;
  }
};

/*
晶石熔炼
*/
struct Robot_MSG_10116005 : public Robot_Base_Msg  {
	int32_t tab_id;
	uint16_t amont;
	Robot_MSG_10116005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(tab_id);
		w.write_uint16(amont);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(tab_id)  ||  r.read_uint16(amont)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10116005;
		tab_id = 0;
		amont = 0;
	}
  void set_rand(void) {
          tab_id = rand()%INT32_MAX;
          amont = rand()%INT16_MAX;
  }
};

/*
荣誉竞技场请求匹配
*/
struct Robot_MSG_10211201 : public Robot_Base_Msg  {
	Robot_MSG_10211201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10211201;
	}
  void set_rand(void) {
  }
};

/*
荣誉竞技场请求领取
*/
struct Robot_MSG_10211202 : public Robot_Base_Msg  {
	int32_t type;
	Robot_MSG_10211202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10211202;
		type = 0;
	}
  void set_rand(void) {
          type = rand()%INT32_MAX;
  }
};

/*
荣誉竞技场匹配面板信息请求
*/
struct Robot_MSG_10211207 : public Robot_Base_Msg  {
	Robot_MSG_10211207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10211207;
	}
  void set_rand(void) {
  }
};

/*
荣誉竞技场请求排名信息
*/
struct Robot_MSG_10211205 : public Robot_Base_Msg  {
	Robot_MSG_10211205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10211205;
	}
  void set_rand(void) {
  }
};

/*
荣誉竞技场请求设置自动匹配
*/
struct Robot_MSG_10211208 : public Robot_Base_Msg  {
	int8_t is_auto;
	Robot_MSG_10211208(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_auto);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_auto)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10211208;
		is_auto = 0;
	}
  void set_rand(void) {
          is_auto = rand()%INT8_MAX;
  }
};

/*
请求运营活动信息
*/
struct Robot_MSG_10117000 : public Robot_Base_Msg  {
	Robot_MSG_10117000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10117000;
	}
  void set_rand(void) {
  }
};

/*
请求运营活动领取奖励
*/
struct Robot_MSG_10117001 : public Robot_Base_Msg  {
	uint32_t act_id;
	uint32_t rec_id;
	Robot_MSG_10117001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(act_id);
		w.write_uint32(rec_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(act_id)  ||  r.read_uint32(rec_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10117001;
		act_id = 0;
		rec_id = 0;
	}
  void set_rand(void) {
          act_id = rand()%INT32_MAX;
          rec_id = rand()%INT32_MAX;
  }
};

/*
请求运营活动图标堆叠数
*/
struct Robot_MSG_10117002 : public Robot_Base_Msg  {
	Robot_MSG_10117002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10117002;
	}
  void set_rand(void) {
  }
};

/*
请求运营活动单标签信息
*/
struct Robot_MSG_10117003 : public Robot_Base_Msg  {
	uint32_t act_id;
	Robot_MSG_10117003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(act_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(act_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10117003;
		act_id = 0;
	}
  void set_rand(void) {
          act_id = rand()%INT32_MAX;
  }
};

/*
请求流浪商人物品信息
*/
struct Robot_MSG_10211300 : public Robot_Base_Msg  {
	int64_t monster_role_id;
	Robot_MSG_10211300(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(monster_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(monster_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10211300;
		monster_role_id = 0;
	}
  void set_rand(void) {
          monster_role_id = rand()%INT64_MAX;
  }
};

/*
请求购买流浪商人物品
*/
struct Robot_MSG_10211301 : public Robot_Base_Msg  {
	int64_t monster_role_id;
	int32_t item_index;
	Robot_MSG_10211301(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(monster_role_id);
		w.write_int32(item_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(monster_role_id)  ||  r.read_int32(item_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10211301;
		monster_role_id = 0;
		item_index = 0;
	}
  void set_rand(void) {
          monster_role_id = rand()%INT64_MAX;
          item_index = rand()%INT32_MAX;
  }
};

/*
获取宝藏海湾信息
*/
struct Robot_MSG_10211310 : public Robot_Base_Msg  {
	Robot_MSG_10211310(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10211310;
	}
  void set_rand(void) {
  }
};

/*
请求进入副本关卡
*/
struct Robot_MSG_10211311 : public Robot_Base_Msg  {
	int64_t monster_role_id;
	Robot_MSG_10211311(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(monster_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(monster_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10211311;
		monster_role_id = 0;
	}
  void set_rand(void) {
          monster_role_id = rand()%INT64_MAX;
  }
};

/*
请求宝藏海外神秘祝福
*/
struct Robot_MSG_10211312 : public Robot_Base_Msg  {
	int8_t bless_index;
	Robot_MSG_10211312(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(bless_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(bless_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10211312;
		bless_index = 0;
	}
  void set_rand(void) {
          bless_index = rand()%INT8_MAX;
  }
};

/*
获取宠物信息
*/
struct Robot_MSG_10113001 : public Robot_Base_Msg  {
	Robot_MSG_10113001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10113001;
	}
  void set_rand(void) {
  }
};

/*
宠物升级, 进阶
*/
struct Robot_MSG_10113002 : public Robot_Base_Msg  {
	int8_t auto_buy;
	Robot_MSG_10113002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10113002;
		auto_buy = 0;
	}
  void set_rand(void) {
          auto_buy = rand()%INT8_MAX;
  }
};

/*
宠物自动升级，进阶
*/
struct Robot_MSG_10113003 : public Robot_Base_Msg  {
	int8_t auto_buy;
	Robot_MSG_10113003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10113003;
		auto_buy = 0;
	}
  void set_rand(void) {
          auto_buy = rand()%INT8_MAX;
  }
};

/*
 宠物激活
*/
struct Robot_MSG_10113004 : public Robot_Base_Msg  {
	Robot_MSG_10113004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10113004;
	}
  void set_rand(void) {
  }
};

/*
宠物幻化
*/
struct Robot_MSG_10113005 : public Robot_Base_Msg  {
	int32_t illusion_id;
	Robot_MSG_10113005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(illusion_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(illusion_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10113005;
		illusion_id = 0;
	}
  void set_rand(void) {
          illusion_id = rand()%INT32_MAX;
  }
};

/*
物品激活宠物幻化
*/
struct Robot_MSG_10113006 : public Robot_Base_Msg  {
	int32_t illusion_id;
	Robot_MSG_10113006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(illusion_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(illusion_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10113006;
		illusion_id = 0;
	}
  void set_rand(void) {
          illusion_id = rand()%INT32_MAX;
  }
};

/*
获取激活宠物物品
*/
struct Robot_MSG_10113007 : public Robot_Base_Msg  {
	Robot_MSG_10113007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 10113007;
	}
  void set_rand(void) {
  }
};

/*
宠物低级学习
*/
struct Robot_MSG_10113008 : public Robot_Base_Msg  {
	int32_t skill_id;
	Robot_MSG_10113008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10113008;
		skill_id = 0;
	}
  void set_rand(void) {
          skill_id = rand()%INT32_MAX;
  }
};

/*
宠物高级学习
*/
struct Robot_MSG_10113009 : public Robot_Base_Msg  {
	int32_t skill_id;
	Robot_MSG_10113009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10113009;
		skill_id = 0;
	}
  void set_rand(void) {
          skill_id = rand()%INT32_MAX;
  }
};

/*
改变宠物跟随状态
*/
struct Robot_MSG_10113010 : public Robot_Base_Msg  {
	int8_t state;
	Robot_MSG_10113010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 10113010;
		state = 0;
	}
  void set_rand(void) {
          state = rand()%INT8_MAX;
  }
};

Robot_Msg_Vec* get_robot_msg_map(void){
	static Robot_Msg_Vec robot_msg_vec_; 
	Robot_Base_Msg *m = 0;
	m = new Robot_MSG_10100000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10400001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200032();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100015();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200008();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100090();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200009();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10188888();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200030();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100008();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100032();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100040();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200102();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200105();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200110();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200150();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100165();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100166();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200151();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100141();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100140();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200103();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100142();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100143();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100144();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200111();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200155();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200156();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200113();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200149();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200117();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100100();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100101();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100102();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100103();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100104();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100105();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100106();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100107();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100108();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100109();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100110();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100111();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100112();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100113();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110008();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110009();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110012();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110013();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110014();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110015();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110016();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110017();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110018();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10110019();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10150000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10150001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10150002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10150005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10150003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10150004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10300001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10300002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10300003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10199999();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10299999();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10399999();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10499999();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100200();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100201();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100202();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100204();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100205();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200200();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100210();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100211();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100212();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100213();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100214();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100206();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100207();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200215();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100208();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101008();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101009();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101012();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101013();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101014();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101015();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210100();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210101();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210200();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210103();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210104();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10210011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102020();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102021();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102022();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102032();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102033();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10102034();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160009();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160012();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160013();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10160014();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10151000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10151001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10151002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10151003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100300();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100301();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100302();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100303();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100304();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100305();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100306();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100307();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100309();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100311();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100312();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100313();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100315();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100316();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100308();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100310();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100317();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100318();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100319();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100320();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100321();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100322();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100323();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100324();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100325();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100326();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100327();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100328();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100329();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100330();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100331();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100332();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100333();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170008();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170009();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170012();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170013();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170014();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170015();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170016();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170017();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170018();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170019();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170020();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170021();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170022();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170023();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170024();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170025();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170026();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170027();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170028();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170029();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170030();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170031();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170032();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170033();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170034();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170035();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170036();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170037();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10270000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10270001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170038();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170039();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10270002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10270004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170041();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170042();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170043();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170044();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170045();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170047();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170048();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10170049();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10152000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10152001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100400();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100405();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100406();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100410();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100420();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100411();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100401();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100402();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100407();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100412();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100413();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100414();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100415();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100408();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100416();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100417();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10104000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10104001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10104002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10104003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10171000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10171001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10171002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10171003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120020();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120021();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120030();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120040();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120041();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120050();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120051();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120052();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120080();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120090();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120091();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120092();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120093();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120094();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120095();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120096();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120097();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120098();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120060();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120061();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120070();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120099();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120100();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120101();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120102();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10120110();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100500();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100501();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100502();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10111000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10111001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10111003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180008();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180009();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180012();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180013();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180014();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180015();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180016();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200152();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200153();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180017();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180018();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200154();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180019();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180020();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180021();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180022();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180023();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10180024();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106100();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106101();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106102();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100800();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100801();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100802();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106200();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106201();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106202();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106203();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106204();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106205();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106206();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106207();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100600();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100601();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100602();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100603();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100604();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100605();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100606();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100608();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100609();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100900();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100901();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103200();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103201();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103202();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103203();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103204();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103205();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103206();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103207();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103208();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103209();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103210();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10103211();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106300();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106301();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106302();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106303();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106304();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106305();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106306();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106307();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106308();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106309();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106400();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106401();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106402();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106403();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106404();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106405();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106420();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106421();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106422();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106423();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106424();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106425();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106426();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106427();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106428();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106406();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106407();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106429();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106430();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106408();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106431();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106409();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106432();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106410();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106433();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106434();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106435();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106436();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106437();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106438();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106439();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106411();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106440();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106441();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106442();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106443();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106444();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106445();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106446();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106447();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106448();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106449();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106412();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106413();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10100700();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200701();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200702();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200703();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200704();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10200705();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105011();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105012();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105013();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105014();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105015();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105016();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105017();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105018();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105020();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105021();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10105022();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107100();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107101();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107102();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107103();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10107104();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101100();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101101();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101102();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101103();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101200();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101201();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101202();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101203();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101204();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101205();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101206();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101207();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201300();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201301();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201302();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201303();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201304();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201305();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201306();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201307();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201308();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101301();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10101300();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201310();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201309();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201311();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10201312();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10108001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10108002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10108003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10209001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10209002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10209003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10109002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10109003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10109005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10109006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10109007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10109008();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10109009();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10109010();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10112000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10112001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211100();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211101();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10111102();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10111103();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106800();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106801();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106900();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10106901();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10116000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10116001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10116002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10116003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10116004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10116005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211201();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211202();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211207();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211205();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211208();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10117000();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10117001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10117002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10117003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211300();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211301();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211310();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211311();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10211312();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113001();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113002();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113003();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113004();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113005();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113006();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113007();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113008();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113009();
	robot_msg_vec_.push_back(m);
	m = new Robot_MSG_10113010();
	robot_msg_vec_.push_back(m);
	return &robot_msg_vec_;
}

#endif
