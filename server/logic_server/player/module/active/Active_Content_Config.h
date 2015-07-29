/*
 * Active_Content_Config.h
 *
 *  Created on: 2014-06-25
 *      Author: xiaoliang
 *
 *      配制文件结构
 */

#ifndef ACTIVE_CONTENT_CONFIG_H_
#define ACTIVE_CONTENT_CONFIG_H_

#include "Game_Typedef.h"
#include "Msg_Struct.h"

//typedef boost::unordered_map<uint32_t, Limit_Buy_Config> Limit_Buy_Conf_Map;

struct Vip_Club_Item_Config {
	Vip_Club_Item_Config(void) { reset(); }
	void reset(void) {
		reward_id = 0;
		item_id = 0;
		limit_vip_level = 0;
		num = 0;
	}

	uint32_t reward_id;//奖励ID(唯一)
	uint32_t item_id;//物品ID
	uint32_t num;//数量
	uint32_t limit_vip_level;//领取等级

};
typedef boost::unordered_map<uint32_t, Vip_Club_Item_Config> Vip_Club_Item_Conf_Map;
struct Vip_Club_Config {
	Vip_Club_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		vip_level = 0;
		needs_num = 0;
		reward.clear();
	}

	uint32_t id;
	uint32_t vip_level;//vip等级
	uint32_t needs_num;//需要到达人数

	Vip_Club_Item_Conf_Map reward;

};
typedef boost::unordered_map<uint32_t, Vip_Club_Config> Vip_Club_Conf_Map;

struct Active_Item_Config {
	Active_Item_Config(void) { reset(); }
	void reset(void) {
		item_id = 0;
		num = 0;
		career = 0;
	}
	uint32_t item_id;
	uint32_t num;
	uint32_t career;
};
typedef std::vector<Active_Item_Config> Active_Item_Vec;

struct Active_Money_Config {
	Active_Money_Config(void) { reset(); }
	void reset(void) {
		money_id = 0;
		num = 0;
		career = 0;
	}
	uint32_t money_id;
	uint32_t num;
	uint32_t career;
};
typedef std::vector<Active_Money_Config> Active_Money_Vec;

struct Active_Reward_Info_Config {
	Active_Reward_Info_Config() { reset(); }
	void reset(void) {
		exp = 0;
		CLEAR_VECTOR(Active_Money_Vec, money);
		CLEAR_VECTOR(Active_Item_Vec, item);
		career = 0;
	}
	void push_back(const Active_Reward_Info_Config &adder) {
		exp += adder.exp;
		for(Active_Money_Vec::const_iterator it = adder.money.begin(); it != adder.money.end(); ++it) {
			money.push_back(*it);
		}
		for(Active_Item_Vec::const_iterator it = adder.item.begin(); it != adder.item.end(); ++it) {
			item.push_back(*it);
		}
	}
	uint32_t exp;
	Active_Money_Vec money;
	Active_Item_Vec item;
	uint32_t career;
};

struct Level_Ace_Config {
	Level_Ace_Config(void) { reset(); }
	void reset(void) {
		level = 0;
		reward_info.reset();
	}
	uint32_t level;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Level_Ace_Config> Level_Ace_Conf_Map;

struct Sg_Pass_Config {
	Sg_Pass_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		sg_id = 0;
		act_id = 0;
		reward_info.reset();
	}
	uint32_t id;
	uint32_t sg_id;
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Sg_Pass_Config> Sg_Pass_Conf_Map;

struct Wing_Levelup_Config {
	Wing_Levelup_Config(void) { reset(); }
	void reset(void) {
		level = 0;
		reward_info.reset();
	}
	uint32_t level;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Wing_Levelup_Config> Wing_Levelup_Conf_Map;

typedef boost::unordered_map<uint32_t, UInt_Set> UInt_Set_Map;

struct Suit_Collect_Config {
	Suit_Collect_Config(void) { reset(); }
	void reset(void) {
		level = 0;
		reward_info.reset();
		suit_collect_item.clear();
		career_suit_collect_item.clear();
	}
	uint32_t level;
	UInt_Set suit_collect_item;//需要收集的时装ID
	UInt_Set_Map career_suit_collect_item;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Suit_Collect_Config> Suit_Collect_Conf_Map;

struct Guild_Level_Config {
	Guild_Level_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		level = 0;
		type =0;
	}
	uint32_t id;
	uint32_t level;//公会等级
	uint32_t type;//领取类型
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Guild_Level_Config> Guild_Level_Conf_Map;

struct Gem_Levelup_Config {
	Gem_Levelup_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		level = 0;
		count = 0;
		act_id = 0;
		reward_info.reset();
	}
	uint32_t id;
	uint32_t count;//宝石数量
	uint32_t level;//Gem等级
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Gem_Levelup_Config> Gem_Levelup_Conf_Map;

struct Mount_Levelup_Config {
	Mount_Levelup_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		mount_id = 0;
		level = 0;
		act_id = 0;
		reward_info.reset();
	}
	uint32_t id;
	uint32_t mount_id;
	uint32_t level;
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Mount_Levelup_Config> Mount_Levelup_Conf_Map;

struct Rank_Count_Config {
	Rank_Count_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		start = 0;
		end = 0;
	}

	uint32_t id;
	uint32_t start;//起始名次
	uint32_t end;//结束名次
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Rank_Count_Config> Rank_Count_Conf_Map;

struct Arena_Dan_Config {
	Arena_Dan_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		group = 0;
		seg = 0;
		time = 0;
	}

	uint32_t id;
	uint32_t group;//组
	uint32_t seg;//段
	uint32_t time;//过期时间(天)
	uint32_t act_id;//活动ID
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Arena_Dan_Config> Arena_Dan_Conf_Map;

struct World_Boss_Config {
	World_Boss_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		type = 0;
		val = 0;
		act_id = 0;
		sval = 0;
	}

	uint32_t id;
	uint32_t type;//项目类型(1.累计伤害, 2.单日伤害, 3.排名)
	uint32_t sval;
	uint32_t val;//值
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, World_Boss_Config> World_Boss_Conf_Map;

struct Suit_Collect_Two_Config {
	Suit_Collect_Two_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		act_id = 0;
		reward_info.reset();
		suit_collect_item.clear();
		career_suit_collect_item.clear();
	}
	uint32_t id;
	uint32_t act_id;
	UInt_Set suit_collect_item;//需要收集的时装ID
	UInt_Set_Map career_suit_collect_item;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Suit_Collect_Two_Config> Suit_Collect_Two_Conf_Map;

struct Heroes_Door_Config {
	Heroes_Door_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		type = 0;
		level = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t type;//难度(0.普通,1.噩梦)
	uint32_t level;//等级
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Heroes_Door_Config> Heroes_Door_Conf_Map;

struct Team_Arena_Config {
	Team_Arena_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//胜利次数
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Team_Arena_Config> Team_Arena_Conf_Map;

struct Warer_Config {
	Warer_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//胜利次数
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Warer_Config> Warer_Conf_Map;

struct Active_Content_Reward_Task_Config {
	Active_Content_Reward_Task_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//次数
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Active_Content_Reward_Task_Config> Reward_Task_Conf_Map;

struct Fm_X_Ci_Config {//累计附魔x次
	Fm_X_Ci_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//累计附魔x次
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Fm_X_Ci_Config> Fm_X_Ci_Conf_Map;

struct X_X_Quality_Heroer_Config {//累计X个X品质英雄
	X_X_Quality_Heroer_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//数量
	uint32_t type;//品质
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, X_X_Quality_Heroer_Config> X_X_Quality_Heroer_Map;

struct Mount_Lvup_X_Config {//坐骑进阶X次
	Mount_Lvup_X_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//进阶次数
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Mount_Lvup_X_Config> Mount_Lvup_X_Conf_Map;

struct Fusion_X_Elite_Card_Config {//合成X张精英卡牌
	Fusion_X_Elite_Card_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//当前张数
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Fusion_X_Elite_Card_Config> Fusion_X_Elite_Card_Conf_Map;

struct Alchemy_Gift_Config {//合成X张精英卡牌
	Alchemy_Gift_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//数
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Alchemy_Gift_Config> Alchemy_Gift_Conf_Map;

struct Activity_Gift_Config {//活跃度好礼
	Activity_Gift_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//数
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Activity_Gift_Config> Activity_Gift_Conf_Map;

struct Active_Ring_Task_Config {//天天跑环 活动期间完成相应环数
	Active_Ring_Task_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
		recharge = 0;
	}

	uint32_t id;
	uint32_t num;//数
	uint32_t act_id;
	uint8_t recharge;//任意充值(1.需要, 0或不填.不需要)
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Active_Ring_Task_Config> Active_Ring_Task_Conf_Map;

struct Dragon_Prey_Config {//活跃度好礼
	Dragon_Prey_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
		act_id = 0;
	}

	uint32_t id;
	uint32_t num;//数
	uint32_t act_id;
	Active_Reward_Info_Config reward_info;
};
typedef boost::unordered_map<uint32_t, Dragon_Prey_Config> Dragon_Prey_Conf_Map;

#endif /* ACTIVE_CONTENT_CONFIG_H_ */
