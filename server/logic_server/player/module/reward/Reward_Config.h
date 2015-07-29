/*
 * Reward_Config.h
 *
 *  Created on: Apr 16, 2014
 *      Author: ti
 *
 *      配制文件结构
 */

#ifndef REWARD_CONFIG_H_
#define REWARD_CONFIG_H_

struct Online_Item_Config {
	Online_Item_Config(void) { reset(); }
	void reset(void) {
		item_id = 0;
		num = 0;
	}
	uint32_t item_id;
	uint32_t num;

};
typedef std::vector<Online_Item_Config> Online_Item_Conf_Vec;

struct Online_Money_Config {
	Online_Money_Config(void) { reset(); }
	void reset(void) {
		money_id = 0;
		num = 0;
	}
	uint32_t money_id;
	uint32_t num;

};
typedef std::vector<Online_Money_Config> Online_Money_Conf_Vec;

struct Online_Gift_Config {
	Online_Gift_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		online_time = 0;
		item_list.clear();
		money_list.clear();
	}
	uint32_t id;
	uint32_t online_time;
	Online_Item_Conf_Vec item_list;
	Online_Money_Conf_Vec money_list;
};
typedef std::vector<Online_Gift_Config> Online_Gift_Conf_Vec;
typedef std::map<uint32_t, Online_Gift_Conf_Vec> Online_Gift_Conf_Vec_Map;

struct Online_Group_Conf {
	Online_Group_Conf(void) { reset(); }
	void reset(void) {
		start = 0;
		end = 0;
		group_id = 0;
	}
	uint32_t group_id;
	uint32_t start;
	uint32_t end;
};
typedef boost::unordered_map<uint32_t, Online_Group_Conf> Online_Group_Conf_Map;

struct Login_Seven_Config {
	Login_Seven_Config(void) { reset(); }
	void reset(void) {
		day = 0;
		item_id = 0;
	}
	uint32_t day;
	uint32_t item_id;
};
typedef boost::unordered_map<uint32_t, Login_Seven_Config> Login_Seven_Conf_Map;

struct Activation_Code_Config {
	Activation_Code_Config(void) { reset(); }
	void reset(void) {
		type_id =0;
		num = 0;
		item_id = 0;
	}
	uint32_t type_id;
	uint32_t num;
	uint32_t item_id;
};
typedef boost::unordered_map<uint32_t, Activation_Code_Config> Activation_Code_Conf_Map;

struct Reward_Back_Ratio_Cost {
	Reward_Back_Ratio_Cost(void) { reset(); }
	void reset(void) {
		level = 0;
		copper_ratio = 0;
		copper_cost = 0;
		gold_ratio = 0;
		gold_cost = 0;
		gold_cost_type = 0;
	}
	uint32_t level;
	uint32_t copper_ratio;
	uint32_t copper_cost;
	uint32_t gold_ratio;
	uint32_t gold_cost;
	uint32_t gold_cost_type;
};
typedef boost::unordered_map<uint32_t, Reward_Back_Ratio_Cost> Reward_Back_Ratio_Cost_Map;

struct Reward_Back_Level_Item_Key {
	Reward_Back_Level_Item_Key(void) { reset(); }
	void reset(void) {
		level = 0;
		item = 0;
	}
	bool operator< (const Reward_Back_Level_Item_Key &k) const {
		if(level == k.level) {
			return this->item < k.item;
		}
		return level < k.level;
	}
	uint32_t level;
	uint32_t item;
};

struct Reward_Back_Level_Item_Val {
	Reward_Back_Level_Item_Val(void) { reset(); }
	void reset(void) {
		level = 0;
		item = 0;
		value = 0;
	}
	uint32_t level;
	uint32_t item;
	uint32_t value;
};
typedef std::map<Reward_Back_Level_Item_Key, Reward_Back_Level_Item_Val> Reward_Back_Level_Item_Val_Map;

struct	Reward_Back_Config {
	Reward_Back_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		day_num = 0;
		copper_ratio = 0;
		copper_cost = 0;
		gold_ratio = 0;
		gold_cost = 0;
		gold_cost_type = 0;
		reward_item.clear();
		reward_money.clear();
		exp = 0;
		ratio_cost_map.clear();
		level_item_val.clear();
		group.clear();
		is_be_group = 0;
		pid = 0;
	}
	uint32_t id;//项目id
	uint32_t day_num;//每日次数
	uint32_t copper_ratio;//金币比例
	uint32_t copper_cost;//每次金币消耗
	uint32_t gold_ratio;//钻石比例
	uint32_t gold_cost;//每次钻石消耗
	uint32_t gold_cost_type;//消耗钻石类型
	UInt_UInt_Map reward_item;//能获取的奖励道具
	UInt_UInt_Map reward_money;//能获取的奖励货币
	uint32_t exp; //PT_EXP_CURRENT 能获取的经验
	Reward_Back_Ratio_Cost_Map ratio_cost_map; //等级对应的比例和消耗
	Reward_Back_Level_Item_Val_Map level_item_val; //等级对应奖励默认找回值
	UInt_Vec group;//汇聚项目ID
	uint32_t is_be_group;//是否汇聚项
	uint32_t pid; //汇聚项父ID
};

typedef boost::unordered_map<uint32_t, Reward_Back_Config> Reward_Back_Conf_Map;

struct Keep_Reward_Config {
	Keep_Reward_Config(void) { reset(); }
	void reset(void) {
		reward_item.clear();
		reward_money.clear();
		exp = 0;
	}
	UInt_UInt_Map reward_item;//能获取的奖励道具
	UInt_UInt_Map reward_money;//能获取的奖励货币
	uint32_t exp; //PT_EXP_CURRENT 能获取的经验
};

#endif /* REWARD_CONFIG_H_ */
