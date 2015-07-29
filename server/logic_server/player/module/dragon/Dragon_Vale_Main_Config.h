/*
 * Dragon_Vale_Main.h
 *
 *  Created on: 2014年7月25日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_MAIN_H_
#define DRAGON_VALE_MAIN_H_

#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"

//主配置通用配置
struct Main_Base_Cfg {
	Main_Base_Cfg() {
		reset();
	}
	void reset() {
		step_foot_max_num_per_day = 0;
		normal_player_chance = 0;
		special_player_chance = 0;
		whole_server_special_chance = 0;
		rob_time_hour = 0;
		rob_time_minute = 0;
		rob_percent = 0;
		rob_down_time = 0;
		border_match_num_of_max_match_coin = 0;
		refresh_thief_time = 0;
		refresh_thief_chance = 0;
		max_thief_num = 0;
		thief_down_production = 0;
		dragon_vale_scene_id = 0;
		battle_map_id = 0;
		dragon_heart_item_id = 0;
		max_dragon_heart_num = 0;
		rob_level = 0;
		drive_thief_max_num = 0;
		white_chance = 0;
		blue_chance = 0;
		orange_chance = 0;
	}
	uint8_t step_foot_max_num_per_day;//每日有奖励踩脚印最大次数
	uint8_t normal_player_chance;	  //普通人踩脚印提升星级概率(0-100之间的整数)
	uint8_t special_player_chance;    //至尊宝踩脚印提升星级概率(0-100之间的整数)
	uint8_t whole_server_special_chance;//每日6点重置，全服每个玩家概率成为至尊宝的概率(0-100之间的整数)
	uint32_t rob_time_hour;				//被掠夺保护时间(单位：小时)
	uint32_t rob_time_minute;	//被掠夺保护时间(单位：分钟)
	uint8_t rob_percent;//掠夺资源百分比(0 - 100之间的整数)
	uint32_t rob_down_time;//掠夺战斗胜利后，可掠夺资源时间(单位：分钟)
	uint32_t border_match_num_of_max_match_coin;//掠夺匹配消耗上限对应的临界匹配次数
	uint32_t refresh_thief_time;//每隔多长时间概率刷新沙鼠偷猎者(单位：分钟)
	uint8_t refresh_thief_chance;//刷新沙鼠偷猎者的概率(0-100之间的整数)
	uint8_t max_thief_num;//沙鼠偷猎者最大数量
	uint8_t thief_down_production;//沙鼠偷猎者降低资源产速百分比(0-100之间的整数)
	uint32_t dragon_vale_scene_id;
	uint32_t battle_map_id;//战斗背景地图id
	uint32_t dragon_heart_item_id;//踩脚印提升星时，奖励龙心道具id
	uint8_t max_dragon_heart_num;//每日给别人踩脚印提升星，获得龙心最大个数
	uint32_t rob_level;
	uint32_t drive_thief_max_num;
	uint8_t white_chance;
	uint8_t blue_chance;
	uint8_t orange_chance;
};

//龙谷星级提速配置
//key:龙谷星级   value:资源产速提升百分比(0-100之间的整数)
typedef boost::unordered_map<uint8_t, uint8_t> Main_Star_Up_Speed_Map;

//掠夺匹配等级区间配置
//key:begin_level  value:end_level
typedef boost::unordered_map<uint32_t, uint32_t> Main_Match_Level_Map;

//踩脚印奖励配置
//key:player_level  value:dragon_soul_award
typedef boost::unordered_map<uint32_t, uint32_t> Main_Foot_Award_Map;

//驱逐沙鼠偷猎者奖励
struct Main_Thief_Award {
	Main_Thief_Award() {
		reset();
	}
	void reset() {
		player_level = 0;
		thief_quality = 0;
		award = 0;
	}
	uint32_t player_level;//玩家角色等级
	uint8_t thief_quality;//沙鼠偷猎者品质
	uint32_t award;		  //奖励龙魂数目
};

//key:  ((uint64_t)player_level << 32) | (uint64_t)thief_quality
typedef boost::unordered_map<uint64_t, uint32_t> Main_Thief_Award_Map;

inline uint64_t create_key_of_Main_Thief_Award(uint32_t player_level, uint8_t thief_quality) {
	return ((uint64_t)player_level << 32) | (uint64_t)thief_quality;
};

//沙鼠偷猎者怪点id配置
typedef boost::unordered_set<uint32_t> Main_Thief_Id_Set;

//掠夺匹配次数消耗金币配置
typedef	boost::unordered_map<uint32_t, uint32_t> Main_Match_Coin_Map;

struct Dragon_Vale_Main_Cfg {
	Dragon_Vale_Main_Cfg() {
		reset();
	}
	void reset() {
		main_base_cfg.reset();
		main_star_up_speed_map.clear();
		main_match_level_map.clear();
		main_foot_award_map.clear();
		main_thief_award_map.clear();
		main_thief_id_set.clear();
		main_match_coin_map.clear();
	}
	Main_Base_Cfg main_base_cfg;
	Main_Star_Up_Speed_Map main_star_up_speed_map;
	Main_Match_Level_Map main_match_level_map;
	Main_Foot_Award_Map main_foot_award_map;
	Main_Thief_Award_Map main_thief_award_map;
	Main_Thief_Id_Set main_thief_id_set;
	Main_Match_Coin_Map main_match_coin_map;
};

#endif /* DRAGON_VALE_MAIN_H_ */
