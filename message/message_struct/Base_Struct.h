/*
* Generate by devtool
*/

#ifndef _BASE_STRUCT_H_
#define _BASE_STRUCT_H_


#include "Block_Buffer.h"
#include <cstring>

struct Base_Msg {
	uint32_t msg_id;
	Base_Msg(void) : msg_id(0) {}	
};

/* Base_Struct */
struct Coord {
	enum Coord_Type {
		GRID_TYPE = 0,
		PIXEL_TYPE = 1
	};

	uint16_t x;/**/
	uint16_t y;/**/
	uint8_t type;
	Coord(void) { reset(); };

	Coord(int x_, int y_, int type_ = GRID_TYPE) : x(x_), y(y_), type(type_) { }
	void set(int x_, int y_, int type_ = GRID_TYPE) {
		x = x_;
		y = y_;
		type = type_;
	}

	Coord pixel_coord(void) {
		if (type) {
			return *this;
		} else {
			return Coord(x * 32 + 16, y * 32 + 16, PIXEL_TYPE);
		}
	}

	Coord grid_coord(void) {
		if (type) {
			return Coord(x / 32, y / 32, GRID_TYPE);
		} else {
			return *this;
		}
	}

	Coord &operator+=(Coord &coord) {
		x += coord.x;
		y += coord.y;
		return *this;
	}

	bool operator==(const Coord &coord) const {
		return x == coord.x && y == coord.y && type == coord.type;
	}

	bool operator!=(const Coord &coord) const {
		return x != coord.x || y != coord.y || type != coord.type;
	}

	bool operator<(const Coord &coord) const  {
		if (x == coord.x)
			return y < coord.y;
		return x < coord.x;
	}

	Coord &operator-=(Coord &coord) {
		x -= coord.x;
		y -= coord.y;
		return *this;
	}

	Coord &operator+=(int num) {
		x += num;
		y += num;
		return *this;
	}

	Coord &operator-=(int num) {
		x -= num;
		y -= num;
		return *this;
	}

	void serialize(Block_Buffer & w) const {
                w.write_uint8(type);
		w.write_uint16(x);
		w.write_uint16(y);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint16(x)  ||  r.read_uint16(y) )
				return -1;
 		return 0;
	}

	void reset(void) {
		x = 0;
		y = 0;
		type = 0;
	}
};
/*
位置信息
*/
struct Location {
	int32_t scene_id;/**/
	int32_t scene_order;/**/
	int32_t scene_layer;/**/
	int32_t toward;/**/
	Coord coord;
	Location(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
		w.write_int32(scene_order);
		w.write_int32(scene_layer);
		w.write_int32(toward);
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  ||  r.read_int32(scene_order)  ||  r.read_int32(scene_layer)  ||  r.read_int32(toward)  ||  coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		scene_id = 0;
		scene_order = 0;
		scene_layer = 0;
		toward = 0;
		coord.reset();
	}
};

/*
服务器ip和端口
*/
struct Server_Link_Info {
	std::string ip;/*IP*/
	int32_t port;/*端口*/
	Server_Link_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(ip);
		w.write_int32(port);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(ip)  ||  r.read_int32(port)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		ip.clear();
		port = 0;
	}
};

/*
64位id-名字
*/
struct Uid_Name {
	int64_t uid;/**/
	std::string name;/**/
	Uid_Name(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(uid);
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(uid)  ||  r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		uid = 0;
		name.clear();
	}
};

/*
整形-字符串
*/
struct Int_String {
	int32_t int_value;/**/
	std::string str_value;/**/
	Int_String(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(int_value);
		w.write_string(str_value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(int_value)  ||  r.read_string(str_value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		int_value = 0;
		str_value.clear();
	}
};

/*
整形-整形
*/
struct Int_Int {
	int32_t val_1;/**/
	int32_t val_2;/**/
	Int_Int(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(val_1);
		w.write_int32(val_2);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(val_1)  ||  r.read_int32(val_2)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		val_1 = 0;
		val_2 = 0;
	}
};

/*
整形-64位ID
*/
struct Int_Role_Id {
	int32_t int_value;/**/
	int64_t role_id;/**/
	Int_Role_Id(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(int_value);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(int_value)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		int_value = 0;
		role_id = 0;
	}
};

/*
金钱
*/
struct Money {
	int32_t copper;/*金币*/
	int32_t bind_gold;/*绑定钻石*/
	int32_t gold;/*钻石*/
	int32_t dragon_soul;/*龙魂*/
	int32_t souls;/*英灵*/
	int32_t friendship;/*友情值*/
        int32_t reputation;/*声望*/
	Money(void) { reset(); };

	bool operator==(const Money &money) const {
		return bind_gold == money.bind_gold && copper == money.copper &&
			gold == money.gold && dragon_soul == money.dragon_soul && souls == money.souls && friendship== money.friendship && reputation== money.reputation;
	}

	bool operator!=(const Money &money) const {
		return !(bind_gold == money.bind_gold && copper == money.copper &&
			gold == money.gold && dragon_soul == money.dragon_soul  && souls == money.souls  && friendship== money.friendship && reputation == money.reputation);
	}

	void serialize(Block_Buffer & w) const {
		w.write_int32(copper);
		w.write_int32(bind_gold);
		w.write_int32(gold);
		w.write_int32(dragon_soul);
		w.write_int32(souls);
		w.write_int32(friendship);
		w.write_int32(reputation);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(copper)  ||  r.read_int32(bind_gold)  ||  r.read_int32(gold)   ||  r.read_int32(dragon_soul)   ||  r.read_int32(souls)  ||  r.read_int32(friendship)  ||  r.read_int32(reputation))
				return -1;
 		return 0;
	}

	void reset(void) {
		copper = 0;
		bind_gold = 0;
		gold = 0;
		dragon_soul = 0;
		souls = 0;
		friendship= 0;
                reputation = 0;
	}
};
/*
物品基本信息
备注：
EQUIP_INDEX = 10000,
PACKAGE_INDEX = EQUIP_INDEX + INDEX_GAP,
WANDER_INDEX = PACKAGE_INDEX + INDEX_GAP,
*/
struct Item_Basic_Info {
	uint32_t index;/*位置标识符*/
	uint32_t seq;/*序列，绝不重复*/
	uint32_t id;/*道具id（图标id整合到此id）*/
	int32_t amount;/*总量*/
	uint8_t bind;/*是否绑定（1）不绑，（2）已绑 其他值非法*/
	int32_t overdue_time;/*时效（秒）*/
	Item_Basic_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_uint32(seq);
		w.write_uint32(id);
		w.write_int32(amount);
		w.write_uint8(bind);
		w.write_int32(overdue_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_uint32(seq)  ||  r.read_uint32(id)  ||  r.read_int32(amount)  ||  r.read_uint8(bind)  ||  r.read_int32(overdue_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		seq = 0;
		id = 0;
		amount = 0;
		bind = 0;
		overdue_time = 0;
	}
};

/*
场景基本信息
*/
struct Scene_Base_Info {
	int32_t scene_id;/**/
	int32_t map_id;/**/
	int32_t config_id;/**/
	std::string scene_name;/**/
	Scene_Base_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(scene_id);
		w.write_int32(map_id);
		w.write_int32(config_id);
		w.write_string(scene_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(scene_id)  ||  r.read_int32(map_id)  ||  r.read_int32(config_id)  ||  r.read_string(scene_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		scene_id = 0;
		map_id = 0;
		config_id = 0;
		scene_name.clear();
	}
};

/*
指定物品Id与amount
*/
struct Item_Id_Amount {
	uint32_t id;/**/
	int32_t amount;/**/
	Item_Id_Amount(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_int32(amount);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_int32(amount)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		amount = 0;
	}
};

/*
地图传送点
*/
struct Transmit_Point {
	Coord coord;
	int32_t scene_id;/*属于哪个场景*/
	int32_t des_scene_id;/*目标场景*/
	int8_t visible;/*0 不显示 1 显示*/
	int64_t role_id;/*传送点id*/
	std::string role_name;/*传送点名*/
	Transmit_Point(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
		w.write_int32(scene_id);
		w.write_int32(des_scene_id);
		w.write_int8(visible);
		w.write_int64(role_id);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  ||  r.read_int32(scene_id)  ||  r.read_int32(des_scene_id)  ||  r.read_int8(visible)  ||  r.read_int64(role_id)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		coord.reset();
		scene_id = 0;
		des_scene_id = 0;
		visible = 0;
		role_id = 0;
		role_name.clear();
	}
};

struct Int_Double {
	int32_t id;/**/
	double value;/**/
	Int_Double(void) { reset(); };
	Int_Double(int id_, double value_) : id(id_), value(value_) { };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_double(value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_double(value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		value = 0.0;
	}
};
/*
Item_Info
*/
struct Item_Info {
	int32_t id;/**/
	int32_t amount;/**/
	int8_t bind;/**/
	Item_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(amount);
		w.write_int8(bind);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(amount)  ||  r.read_int8(bind)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		amount = 0;
		bind = 0;
	}
};

struct Prop_Change {
	int32_t prop_type;/*物品类型定义.xlsx(属性)*/
	double change_val;/*大于0加  小于0减*/
	double old_val;/*当前值*/
	double current_val;/*当前值*/
	Prop_Change(void) { reset(); };
	Prop_Change(int32_t prop) : prop_type(prop), change_val(0.0), old_val(0.0), current_val(0.0) {}

	void serialize(Block_Buffer & w) const {
		w.write_int32(prop_type);
		w.write_double(change_val);
		w.write_double(current_val);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(prop_type)  ||  r.read_double(change_val)  ||  r.read_double(current_val)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		prop_type = 0;
		change_val = 0.0;
		current_val = 0;
		old_val = 0.0;
	}
};
struct Property {
	int32_t type;/*物品类型定义.xlsx*/
	double value;/**/
	Property(int32_t t, double v) : type(t), value(v) { };

	Property(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_double(value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_double(value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		value = 0.0;
	}
};
/*
Id_Int_Int
*/
struct Id_Int_Int {
	int64_t id;/**/
	int32_t val1;/**/
	int32_t val2;/**/
	Id_Int_Int(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(id);
		w.write_int32(val1);
		w.write_int32(val2);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(id)  ||  r.read_int32(val1)  ||  r.read_int32(val2)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		val1 = 0;
		val2 = 0;
	}
};

/*
状态信息
*/
struct Status_Info_Msg {
	int32_t status_id;/**/
	int64_t status_owner;/*状态持有人*/
	int64_t last_time_ms;/*状态总时间(单位毫秒)*/
	int32_t overlay;/*层数(当前多少层)，0为删除*/
	int32_t remain_time;/*剩余时间(单位秒)*/
	std::vector<Property> prop;
	int32_t source_id;/*怪物id*/
	Status_Info_Msg(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(status_id);
		w.write_int64(status_owner);
		w.write_int64(last_time_ms);
		w.write_int32(overlay);
		w.write_int32(remain_time);
		uint16_t __prop_vec_size = prop.size();
		w.write_uint16(__prop_vec_size);
		for(uint16_t i = 0; i < __prop_vec_size; ++i) {
				prop[i].serialize(w);
		}

		w.write_int32(source_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(status_id)  ||  r.read_int64(status_owner)  ||  r.read_int64(last_time_ms)  ||  r.read_int32(overlay)  ||  r.read_int32(remain_time)  )
				return -1;
 		uint16_t __prop_vec_size;
		if(r.read_uint16(__prop_vec_size) )
				return -1;
		prop.reserve(__prop_vec_size);
		for(uint16_t i = 0; i < __prop_vec_size; ++i) {
				Property v;
				if(v.deserialize(r))
						return -1;
				prop.push_back(v);
		}

		if( r.read_int32(source_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		status_id = 0;
		status_owner = 0;
		last_time_ms = 0;
		overlay = 0;
		remain_time = 0;
		prop.clear();
		source_id = 0;
	}
};

/*
无符号整形-无符号整形
*/
struct Uint_Uint {
	uint32_t val_1;/**/
	uint32_t val_2;/**/
	Uint_Uint(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(val_1);
		w.write_uint32(val_2);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(val_1)  ||  r.read_uint32(val_2)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		val_1 = 0;
		val_2 = 0;
	}
};

/*
出现附加属性
类型:type:
 100:  战场附加信息 array 【连杀数、连续死亡数、hp、是否拉矿】
 200: 龙谷附加信息 int_value1: 品质
 300: 公会战array 资源塔[已走时间、总时间、人数、所属(0未被占领 1红色 2蓝色)、突袭状态(0未突袭1红突袭2蓝突袭)] 人物[hp、是否正在采集]
 400: 篝火晚会篝火附加信息 str:公会名称 array【等级、等级当前柴火数量、等级提升柴火数量、是否点燃】 int64_vec 【公会id】
 401：篝火晚会人物附加信息（有该附加信息表示玩家在跳舞）
500：远征血条百分比
510:采集物信息:int64_vec已采人列表(默认每人一次)，array:0已采次数1总数
*/
struct Appear_Addition {
	int16_t type;/*类型*/
	int32_t int_value1;/*值*/
	std::string str;/**/
	std::vector<int32_t> array;/**/
	std::vector<int64_t> int64_vec;/**/
	Appear_Addition(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(type);
		w.write_int32(int_value1);
		w.write_string(str);
		uint16_t __array_vec_size = array.size();
		w.write_uint16(__array_vec_size);
		for(uint16_t i = 0; i < __array_vec_size; ++i) {
				w.write_int32(array[i]);
		}

		uint16_t __int64_vec_vec_size = int64_vec.size();
		w.write_uint16(__int64_vec_vec_size);
		for(uint16_t i = 0; i < __int64_vec_vec_size; ++i) {
				w.write_int64(int64_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(type)  ||  r.read_int32(int_value1)  ||  r.read_string(str)  )
				return -1;
 		uint16_t __array_vec_size;
		if(r.read_uint16(__array_vec_size) )
				return -1;
		array.reserve(__array_vec_size);
		for(uint16_t i = 0; i < __array_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				array.push_back(v);
		}

		uint16_t __int64_vec_vec_size;
		if(r.read_uint16(__int64_vec_vec_size) )
				return -1;
		int64_vec.reserve(__int64_vec_vec_size);
		for(uint16_t i = 0; i < __int64_vec_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				int64_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		type = 0;
		int_value1 = 0;
		str.clear();
		array.clear();
		int64_vec.clear();
	}
};

/*
成就内容明细
*/
struct Achievement_Detail_Msg {
	int32_t id;/*成就id*/
	int8_t state_count;/*成就点状态（0不领1可领2已领）*/
	int8_t state_award;/*非成就点奖励状态（0不领1可领2已领）*/
	int32_t data;/*具体完成度*/
	Achievement_Detail_Msg(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int8(state_count);
		w.write_int8(state_award);
		w.write_int32(data);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int8(state_count)  ||  r.read_int8(state_award)  ||  r.read_int32(data)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		state_count = 0;
		state_award = 0;
		data = 0;
	}
};

/*
字符串信息改变
备注：
type:
1 公会名
2 角色名
*/
struct String_Info_Change {
	int32_t type;/**/
	std::string value;/**/
	String_Info_Change(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_string(value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_string(value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		value.clear();
	}
};

/*
整型信息改变
备注：
type：
1 英雄等级
*/
struct Int32_Info_Change {
	int32_t type;/**/
	int32_t value;/**/
	Int32_Info_Change(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_int32(value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_int32(value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		value = 0;
	}
};

/*
公会战公会排名数据
*/
struct Gang_War_Gang_Rank {
	std::string gang_name;/*公会名*/
	int32_t score;/*总积分*/
	int32_t win_nums;/*总胜场数*/
	int32_t kill_nums;/*总击杀数*/
	int32_t hold_nums;/*总抢点*/
	int32_t force;/*总战力*/
	std::string zone;/*服务器名*/
	Gang_War_Gang_Rank(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(gang_name);
		w.write_int32(score);
		w.write_int32(win_nums);
		w.write_int32(kill_nums);
		w.write_int32(hold_nums);
		w.write_int32(force);
		w.write_string(zone);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(gang_name)  ||  r.read_int32(score)  ||  r.read_int32(win_nums)  ||  r.read_int32(kill_nums)  ||  r.read_int32(hold_nums)  ||  r.read_int32(force)  ||  r.read_string(zone)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_name.clear();
		score = 0;
		win_nums = 0;
		kill_nums = 0;
		hold_nums = 0;
		force = 0;
		zone.clear();
	}
};

/*
整形--整形--整形
*/
struct Int_Int_Int {
	int32_t val1;/**/
	int32_t val2;/**/
	int32_t val3;/**/
	Int_Int_Int(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(val1);
		w.write_int32(val2);
		w.write_int32(val3);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(val1)  ||  r.read_int32(val2)  ||  r.read_int32(val3)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		val1 = 0;
		val2 = 0;
		val3 = 0;
	}
};



#endif