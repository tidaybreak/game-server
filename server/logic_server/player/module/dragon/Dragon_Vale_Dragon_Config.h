/*
 * Dragon_Vale_Dragon_Config.h
 *
 *  Created on: 2014年7月20日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_DRAGON_CONFIG_H_
#define DRAGON_VALE_DRAGON_CONFIG_H_

#include "boost/unordered_map.hpp"

//龙的通用配置
struct Dragon_Base_Cfg {
	Dragon_Base_Cfg() {
		reset();
	}

	void reset() {
		dragon_born_time = 0;
		max_feeling = 0;
		down_feeling_time = 0;
		money_type = 0;
		money_num = 0;
		play_dragon_time = 0;
		energy = 0;
		chance = 0;
		max_evolution = 0;
		max_batch_evolution_num = 0;
		evolution_open_level = 0;
		max_play_dragon_award_num = 0;
		dragon_born_level = 0;
		dragon_vale_op_type = 0;
	}
	uint32_t dragon_born_time; //龙蛋孵化所用时间(单位：分钟)
	uint8_t max_feeling;       //龙最大心情值  (不超过255,这里用uint8_t足够)
	uint32_t down_feeling_time;//多长时间龙的心情值下降一点(单位：分钟)
	uint8_t money_type;        //增加一点心情值，消耗货币类型
	uint32_t money_num;		   //增加一点心情值，消耗货币数量
	uint32_t play_dragon_time; //逗龙冷却时间(单位：分钟)
	uint32_t energy;           //每次进化获得的能量
	uint8_t chance;			   //每次进化获得10倍能量的概率(0 - 100之间的整数)
	uint8_t max_evolution;     //龙进化最大进度
	uint16_t max_batch_evolution_num; //批量进化龙的最大批量次数
	uint16_t evolution_open_level;
	uint32_t max_play_dragon_award_num;
	uint16_t dragon_born_level;
	uint32_t dragon_vale_op_type;
};

//龙培养的玩家角色等级限制配置
struct Dragon_Role_Level_Cfg {
	Dragon_Role_Level_Cfg() {
		reset();
	}
	void reset() {
		player_level = 0;
		type = 0;
		dragon_max_level = 0;
	}
	uint32_t player_level;       //玩家角色等级
	uint8_t type;				 //龙培养类型
	uint32_t dragon_max_level;   //龙培养的最大等级
};

//key:  ((uint64_t)player_level << 32) | (uint64_t)type   value:Dragon_Role_Level_Cfg
typedef boost::unordered_map<uint64_t, Dragon_Role_Level_Cfg> Dragon_Role_Level_Cfg_Map;

//生成 Dragon_Role_Level_Cfg_Map 的key    player_level:角色等级   dragon_keep_type:龙培养类型
inline uint64_t create_key_of_Dragon_Role_Level_Cfg_Map(uint32_t player_level, uint8_t dragon_keep_type) {
	return ((uint64_t)player_level << 32) | (uint64_t)dragon_keep_type;
}

//龙培养配置
struct Dragon_Keep_Cfg {
	Dragon_Keep_Cfg() {
		reset();
	}
	void reset() {
		type = 0;
		dragon_level = 0;
		coin_num = 0;
		dragon_soul_num = 0;
		add_property = (float)0;
		success_chance = 0;
	}
	uint8_t type;			    //龙培养类型
	uint32_t dragon_level;      //龙培养等级
	uint32_t coin_num;			//培养消耗金币数量
	uint32_t dragon_soul_num; 	//培养消耗龙魂数量
	float add_property;			//增加属性百分比(斗力 or 护甲 or 耐力)  如：12.5% 配置为：12.5
	uint8_t success_chance;     //培养成功率(0 - 100之间的整数)
};

//key:  ((uint64_t)type << 32) | (uint64_t)dragon_level   value:Dragon_Keep_Cfg
typedef boost::unordered_map<uint64_t, Dragon_Keep_Cfg> Dragon_Keep_Cfg_Map;

//生成 Dragon_Keep_Cfg_Map 的key   type:龙培养类型   dragon_level:龙培养等级
inline uint64_t create_key_of_Dragon_Keep_Cfg_Map(uint8_t type, uint32_t dragon_level) {
	return ((uint64_t)type << 32) | (uint64_t)dragon_level;
}

//龙心情配置
struct Dragon_Feeling_Cfg {
	Dragon_Feeling_Cfg() {
		reset();
	}
	void reset() {
		dragon_feeling = 0;
		chance = 0;
	}
	uint8_t dragon_feeling; //龙的心情值
	uint8_t chance;         //龙培养成功概率(0-100之间的整数)
};

//key:dragon_feeling  value:Dragon_Feeling_Cfg
typedef boost::unordered_map<uint8_t, Dragon_Feeling_Cfg> Dragon_Feeling_Cfg_Map;

//龙进化配置
struct Dragon_Evolution_Cfg {
	Dragon_Evolution_Cfg() {
		reset();
	}
	void reset() {
		progress = 0;
		total_energy = 0;
		base_power = 0;
		base_armor = 0;
		base_stamina = 0;
		base_crit = 0;
		base_prevent_crit = 0;
		item_id = 0;
		item_num = 0;
		money_type = 0;
		money_num = 0;
		dragon_name = "";
	}
	uint8_t progress; 	   //龙的进化程度
	uint32_t total_energy; //进化所需能量值
	uint32_t base_power;   //基础斗力
	uint32_t base_armor;   //基础护甲
	uint32_t base_stamina;  //基础耐力
	uint32_t base_crit;     //基础暴击
	uint32_t base_prevent_crit;//基础抗暴

	uint32_t item_id;	   //消耗龙珠道具id
	uint32_t item_num;     //消耗龙珠道具数量
	uint8_t money_type;    //消耗货币类型
	uint32_t money_num;    //消耗货币数量
	std::string dragon_name; //龙名字
};

//key:progress  value:Dragon_Evolution_Cfg
typedef boost::unordered_map<uint8_t, Dragon_Evolution_Cfg> Dragon_Evolution_Cfg_Map;


//逗龙奖励配置
struct Dragon_Play_Dragon_Cfg {
	Dragon_Play_Dragon_Cfg() {
		reset();
	}
	void reset() {
		player_level = 0;
		play_dragon_award = 0;
	}
	uint32_t player_level;       //玩家角色等级
	uint32_t play_dragon_award;  //逗龙奖励龙魂
};

//key:player_level   value:Dragon_Play_Dragon_Cfg
typedef boost::unordered_map<uint32_t, Dragon_Play_Dragon_Cfg> Dragon_Play_Dragon_Cfg_Map;

//逗龙概率获得龙心配置
struct Play_Dragon_Get_Heart_Cfg {
	Play_Dragon_Get_Heart_Cfg() {
		reset();
	}
	void reset() {
		chance = 0;
		item_id = 0;
	}
	uint8_t chance;
	uint32_t item_id;
};

//key:heart_num (must use std::map, because it must be sorted)
typedef std::map<uint16_t, Play_Dragon_Get_Heart_Cfg> Dragon_Play_Dragon_Get_Heart_Cfg_Map;

//龙的总配置
struct Dragon_All_Cfg {
	Dragon_All_Cfg() {
		reset();
	}
	void reset() {
		dragon_base_cfg.reset();
		dragon_role_level_cfg_map.clear();
		dragon_keep_cfg_map.clear();
		dragon_feeling_cfg_map.clear();
		dragon_evolution_cfg_map.clear();
		dragon_play_dragon_cfg_map.clear();
		dragon_play_dragon_get_heart_cfg_map.clear();
	}
	Dragon_Base_Cfg dragon_base_cfg;					   //龙的通用配置
	Dragon_Role_Level_Cfg_Map dragon_role_level_cfg_map;   //龙培养的玩家角色等级限制配置的map
	Dragon_Keep_Cfg_Map dragon_keep_cfg_map;	           //龙培养配置的map
	Dragon_Feeling_Cfg_Map dragon_feeling_cfg_map; 	       //龙心情配置的map
	Dragon_Evolution_Cfg_Map dragon_evolution_cfg_map;     //龙进化配置的map
	Dragon_Play_Dragon_Cfg_Map dragon_play_dragon_cfg_map; //逗龙奖励配置的map
	Dragon_Play_Dragon_Get_Heart_Cfg_Map dragon_play_dragon_get_heart_cfg_map;//逗龙概率获得龙心配置的map
};

#endif /* DRAGON_VALE_DRAGON_CONFIG_H_ */
