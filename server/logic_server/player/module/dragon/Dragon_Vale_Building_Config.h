/*
 * Dragon_Config.h
 *
 *  Created on: 2014年7月10日
 *      Author: gaojianshan
 */

#ifndef DRAGON_CONFIG_H_
#define DRAGON_CONFIG_H_

#include "boost/unordered_map.hpp"

//建筑等级特性配置
struct Building_Level_Cfg {
	Building_Level_Cfg() {
		reset();
	}
	void reset() {
		type = 0;
		level = 0;
		production_per_min = 0;
		min_gain_limit = 0;
		max_storage = 0;
		upgrade_need_hour = 0;
		upgrade_need_min = 0;
		upgrade_need_second = 0;
		upgrade_money_type = 0;
		upgrade_money_num = 0;
		cancel_upgrade_return_money_num = 0;
	}

	uint8_t type;                            //建筑类型
	uint16_t level;                          //建筑等级
	uint32_t production_per_min;             //每分钟资源产出量
	uint32_t min_gain_limit;                 //最低资源收获标准(资源产出量)
	uint32_t max_storage;                    //资源存储上限
	uint32_t upgrade_need_hour;              //建筑升级所用时间1(单位：小时)
	uint32_t upgrade_need_min;               //建筑升级所用时间2(单位：分钟)
	uint32_t upgrade_need_second;            //建筑升级所用时间3(单位：秒)
	uint8_t upgrade_money_type;              //建筑升级消耗货币类型
	uint32_t upgrade_money_num;              //建筑升级消耗货币数量
	uint32_t cancel_upgrade_return_money_num;//取消升级返还货币数量
};

//key:   ((uint32_t)type  << 16) | (uint32_t)level   value:Building_Level_Cfg
typedef boost::unordered_map<uint32_t, Building_Level_Cfg> Building_Level_Cfg_Map;

//生成 Building_Level_Cfg_Map 的key   building_type:建筑类型     building_level:建筑等级
inline uint32_t create_key_of_Building_Level_Cfg_Map(uint8_t building_type, uint16_t building_level) {
	return ((uint32_t)building_type  << 16) | (uint32_t)building_level;
}

//角色等级限制配置
struct Role_Level_Cfg {
	Role_Level_Cfg() {
		reset();
	}
	void reset() {
		player_level = 0;
		type = 0;
		max_building_level = 0;
		max_building_num = 0;
	}
	uint32_t player_level;       //角色等级
	uint8_t type;                //建筑类型
	uint16_t max_building_level; //建筑最大等级
	uint16_t max_building_num;   //建筑最大数量
};

//key:  ((uint64_t)player_level << 32) | (uint64_t)type   value:Role_Level_Cfg
typedef boost::unordered_map<uint64_t, Role_Level_Cfg> Role_Level_Cfg_Map;

//生成 Role_Level_Cfg_Map 的key    player_level:角色等级   building_type:建筑类型
inline uint64_t create_key_of_Role_Level_Cfg_Map(uint32_t player_level, uint8_t building_type) {
	return ((uint64_t)player_level << 32) | (uint64_t)building_type;
}

//vip等级限制配置
struct Vip_Level_Cfg {
	Vip_Level_Cfg() {
		reset();
	}
	void reset() {
		vip_level = 0;
		max_queue_num = 0;
	}
	uint32_t vip_level;     //vip等级
	uint8_t max_queue_num;  //最大开启建筑队列数目
};

//key: vip_level   value:Vip_Level_Cfg
typedef boost::unordered_map<uint32_t, Vip_Level_Cfg> Vip_Level_Cfg_Map;

//开启建筑队列消耗特性配置
struct Building_Queue_Cfg {
	Building_Queue_Cfg() {
		reset();
	}
	void reset() {
		building_order = 0;
		open_queue_money_type = 0;
		open_queue_money_num = 0;
	}
	uint8_t building_order;        //建筑队列次序
	uint8_t open_queue_money_type; //开启当前次序队列消耗货币类型
	uint32_t open_queue_money_num; //开启当前次序队列消耗货币数量
};

//key:building_order  value:Building_Queue_Cfg
typedef boost::unordered_map<uint8_t, Building_Queue_Cfg> Building_Queue_Cfg_Map;

//加速升级配置
struct Quick_Upgrade_Cfg {
	Quick_Upgrade_Cfg() {
		reset();
	}
	void reset() {
		upgrade_left_time = 0;
		quick_upgrade_money_type = 0;
		quick_upgrade_money_ratio = 0;
	}
	uint32_t upgrade_left_time;        //升级剩余时间(单位：小时)  注：不够1小时，算1小时
	uint8_t quick_upgrade_money_type;  //加速升级消耗货币类型
	uint32_t quick_upgrade_money_ratio;//加速升级消耗货币系数
};

//key:upgrade_left_time  value:Quick_Upgrade_Cfg
typedef boost::unordered_map<uint32_t, Quick_Upgrade_Cfg> Quick_Upgrade_Cfg_Map;

//建造建筑消耗配置
struct Create_Building_Cfg {
	Create_Building_Cfg() {
		reset();
	}
	void reset() {
		order = 0;
		create_need_hour = 0;
		create_need_min = 0;
		create_need_second = 0;
		create_money_type = 0;
		create_money_num = 0;
		cancel_create_return_money_num = 0;
		create_money_type2 = 0;
		create_money_num2 = 0;
		cancel_create_return_2 = 0;
	}

	uint8_t order;//建筑建造次序
	uint32_t create_need_hour;//建筑建造所用时间1(单位：小时)
	uint32_t create_need_min;//建筑建造所用时间2(单位：分钟)
	uint32_t create_need_second;//建筑建造所用时间3(单位：秒)
	uint8_t create_money_type;//建筑建造消耗货币类型
	uint32_t create_money_num;//建筑建造消耗货币数量
	uint32_t cancel_create_return_money_num;//取消建筑建造返还货币数量
	uint8_t create_money_type2;
	uint32_t create_money_num2;
	uint32_t cancel_create_return_2;
};

//key: order
typedef boost::unordered_map<uint8_t, Create_Building_Cfg> Create_Building_Cfg_Map;

//龙谷建筑总配置
struct Dragon_Building_Cfg {
	Dragon_Building_Cfg() {
		reset();
	}
	void reset() {
		building_level_cfg_map.clear();
		role_level_cfg_map.clear();
		vip_level_cfg_map.clear();
		building_queue_cfg_map.clear();
		quick_upgrade_cfg_map.clear();
		create_building_cfg_map.clear();
	}
	Building_Level_Cfg_Map building_level_cfg_map;  //建筑等级特性配置的map
	Role_Level_Cfg_Map role_level_cfg_map;          //角色等级限制配置的map
	Vip_Level_Cfg_Map  vip_level_cfg_map;           //vip等级限制配置的map
	Building_Queue_Cfg_Map building_queue_cfg_map;  //开启建筑队列消耗特性配置的map
	Quick_Upgrade_Cfg_Map  quick_upgrade_cfg_map;   //加速升级配置的map
	Create_Building_Cfg_Map create_building_cfg_map;//建造建筑消耗配置的map
};


#endif /* DRAGON_CONFIG_H_ */
