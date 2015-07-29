/*
 * Vip_Config.h
 *
 *  Created on: 2014-07-21
 *      Author: xiaoliang
 *
 *      配制文件结构
 */

#ifndef VIP_CONFIG_H_
#define VIP_CONFIG_H_

struct Vip_Open_Config {
	uint32_t id;
	uint32_t price;
	uint32_t init_level;
	uint32_t days;
	Vip_Open_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		price = 0;
		init_level = 0;
		days = 0;
	}
};

typedef boost::unordered_map<uint32_t, Vip_Open_Config> Vip_Open_Conf_Map;

struct Grow_Get_Config {
	uint32_t vip_level;
	uint32_t get_val;
	uint32_t need_val;
	uint32_t lost_val;
	uint32_t cost_ratio;//消费成长值汇率
	uint32_t pri_num; //特权项数
	Grow_Get_Config(void) { reset(); }
	void reset(void) {
		vip_level = 0;
		get_val = 0;
		need_val = 0;
		lost_val = 0;
		cost_ratio = 0;
		pri_num = 0;
	}
};
typedef boost::unordered_map<uint32_t, Grow_Get_Config> Grow_Get_Conf_Map;

struct World_Tree_Config {
	uint32_t id;
	std::vector<uint32_t> addition;
	World_Tree_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		addition.clear();
	}
};
typedef boost::unordered_map<uint32_t, World_Tree_Config> World_Tree_Conf_Map;

typedef std::set<uint8_t> UInt8_Set;
struct Turntable_Item_Config {
	uint32_t id; //物品序号id
	uint32_t item_id; //物品id
	uint32_t num; //物品数量
	int32_t rate; //物品权重
	UInt8_Set type; //生效次数
	uint8_t ann;//是否发送广播
	Turntable_Item_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		item_id = 0;
		num = 0;
		rate = 0;
		type.clear();
		ann = 0;
	}
};
typedef boost::unordered_map<int32_t, Turntable_Item_Config> Turntable_Item_Conf_Map;

typedef std::vector<Turntable_Item_Config> Turntable_Item_Config_Vec;
typedef boost::unordered_map<uint8_t, Turntable_Item_Config_Vec> Turntable_Type_Item_Conf_Map;//生效次数,物品数组

typedef std::multiset<uint32_t> UInt_Mset;
struct Turntable_Config {
	uint32_t id;	//物品组ID
	UInt_Mset item_list;//物品列表
	uint32_t list_size;//物品列表大小
	uint32_t max_open;//最大获许数
	int32_t rate; //权重

	Turntable_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		item_list.clear();
		list_size = 0;
		max_open = 0;
	}
};
typedef std::vector<Turntable_Config> Turntable_Config_Vec;

struct Turntable_Cost_Config {
	uint8_t num;
	uint16_t cost;
	Turntable_Cost_Config(void) { reset(); }
	void reset(void) {
		num = 0;
		cost = 0;
	}
};
typedef boost::unordered_map<uint8_t, Turntable_Cost_Config> Turntable_Cost_Conf_Map;

struct Vip_Daily_Item_Config {
	Vip_Daily_Item_Config(void) { reset(); }
	void reset(void) {
		vip_level = 0;
		item_id = 0;
		bind = 0;
		num = 0;
	}
	uint32_t vip_level;
	uint32_t item_id;//物品ID
	uint32_t num;//数量
	uint32_t bind;//绑定类型
};
typedef std::vector<Vip_Daily_Item_Config> Vip_Daily_Item_Config_Vec;
typedef boost::unordered_map<uint32_t, Vip_Daily_Item_Config_Vec> Vip_Daily_Item_Config_Vec_Map;

#endif /* VIP_CONFIG_H_ */
