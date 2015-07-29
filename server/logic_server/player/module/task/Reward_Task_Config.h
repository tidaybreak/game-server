/*
 * Reward_Task_Config.h
 *
 *  Created on: Jun 25, 2014
 *      Author: gaojianshan
 */

#ifndef REWARD_TASK_CONFIG_H_
#define REWARD_TASK_CONFIG_H_

#include <list>
//#include <map>
#include "boost/unordered_set.hpp"

//key: item_id
typedef boost::unordered_set<uint32_t> RewardTokenSet;

struct White_Reward {
	White_Reward() {
		reset();
	}
	void reset() {
		accept_player_level = 0;
		exp = 0;
		coin = 0;
	}
	uint16_t accept_player_level; //领取玩家等级
	uint32_t exp;                 //奖励经验
	uint32_t coin;                //奖励金币
};

typedef std::list<White_Reward> White_Reward_List;

struct Reward_Task_Config {
	Reward_Task_Config() {
		reset();
	}
	void reset() {
		auto_refresh = 0;
		free_refresh_max_count_per_day = 0;
		accept_max_count_per_day = 0;
		do_max_count_at_same_time = 0;
		open_level = 0;
		money_type = 0;
		money_num = 0;
		green = 0;
		blue = 0;
		purple = 0;
		orange = 0;
		reward_token_set.clear();
		white_chance = 0;
		green_chance = 0;
		blue_chance = 0;
		purple_chance = 0;
		orange_chance = 0;
		item_id = 0;
		item_num = 0;
	}

	uint32_t auto_refresh;                        //悬赏榜自动刷新时间(单位：分钟)
	uint32_t free_refresh_max_count_per_day;      //每日免费刷新最大次数
	uint32_t accept_max_count_per_day;			 //每日领取悬赏任务最大次数
	uint32_t do_max_count_at_same_time;           //可同时做悬赏任务最大个数
	uint8_t open_level;					         //打开悬赏榜等级限制
	uint32_t money_type;                         //立即刷新消耗货币类型  1:只扣金币  2:优先扣粉钻,粉钻不足，再扣钻石 3:只扣钻石  4:只扣粉钻  5:只扣英灵
	uint32_t money_num;                          //立即刷新消耗货币数量
	uint8_t green;                               //绿色品质倍数
	uint8_t blue;                                //蓝色品质倍数
	uint8_t purple;                              //紫色品质倍数
	uint8_t orange;                              //橙色品质倍数
	White_Reward_List white_reward_list;         //白色品质奖励
	RewardTokenSet reward_token_set;             //可以在背包直接使用的悬赏令集合
	uint8_t white_chance;                        //白色品质刷新概率
	uint8_t green_chance;                        //绿色品质刷新概率
	uint8_t blue_chance;                         //蓝色品质刷新概率
	uint8_t purple_chance;                       //紫色品质刷新概率
	uint8_t orange_chance;                       //橙色品质刷新概率
	uint32_t item_id;                            //领取悬赏任务获得道具id
	uint32_t item_num;                           //领取悬赏任务获得道具数量
};

#endif /* REWARD_TASK_CONFIG_H_ */
