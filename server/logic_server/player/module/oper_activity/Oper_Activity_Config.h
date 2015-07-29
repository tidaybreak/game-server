/*
 * Oper_Activity_Config.h
 *
 *  Created on: Apr 16, 2014
 *      Author: xiaoliang
 *
 *      配制文件结构
 */

#ifndef OPER_ACTIVITY_CONFIG_H_
#define OPER_ACTIVITY_CONFIG_H_

#include "Game_Typedef.h"
#include "Msg_Struct.h"
#include "status/Status_Struct.h"

struct Oper_Activity_Item_Config {
	Oper_Activity_Item_Config(void) { reset(); }
	Oper_Activity_Item_Config(uint32_t item_id_, uint32_t num_, uint32_t career_ = 0) {
		item_id = item_id_;
		num = num_;
		career = career_;
	}
	void reset(void) {
		item_id = 0;
		num = 0;
		career = 0;
	}
	uint32_t item_id;
	uint32_t num;
	uint32_t career;
};
typedef std::vector<Oper_Activity_Item_Config> Oper_Activity_Item_Vec;

struct Oper_Activity_Money_Config {
	Oper_Activity_Money_Config(void) { reset(); }
	Oper_Activity_Money_Config(uint32_t money_id_, uint32_t num_, uint32_t career_ = 0) {
		money_id = money_id_;
		num = num_;
		career = career_;
	}
	void reset(void) {
		money_id = 0;
		num = 0;
		career = 0;
	}
	uint32_t money_id;
	uint32_t num;
	uint32_t career;
};
typedef std::vector<Oper_Activity_Money_Config> Oper_Activity_Money_Vec;

struct Oper_Activity_Reward_Info_Config {
	Oper_Activity_Reward_Info_Config() { reset(); }
	void reset(void) {
		exp = 0;
		CLEAR_VECTOR(Oper_Activity_Money_Vec, money);
		CLEAR_VECTOR(Oper_Activity_Item_Vec, item);
		career = 0;
	}
	void push_back(const Oper_Activity_Reward_Info_Config &adder) {
		exp += adder.exp;
		for(Oper_Activity_Money_Vec::const_iterator it = adder.money.begin(); it != adder.money.end(); ++it) {
			money.push_back(*it);
		}
		for(Oper_Activity_Item_Vec::const_iterator it = adder.item.begin(); it != adder.item.end(); ++it) {
			item.push_back(*it);
		}
	}
	uint32_t exp;
	Oper_Activity_Money_Vec money;
	Oper_Activity_Item_Vec item;
	uint32_t career;
};

typedef std::vector<UInt_Vec> UInt_2_Vec;

struct Oper_Activity_Item_Line_Config {
	Oper_Activity_Item_Line_Config(void) {
		reset();
	}
	void reset(void) {
		rec_id = 0;
		type = 0;
		num = 0;
		parm = 0;
		parm_ex_2arr.clear();
		parm_ex_arr.clear();
		ditto = 0;
		refresh = 0;
		reward_info.reset();
	}
	uint32_t rec_id;//领取ID
	uint32_t type;//项目类型
	uint32_t num;//完成次数
	uint32_t parm;//完成参数 主要用来表示类型
	UInt_Vec parm_ex_arr;//完成参数 主要用来表示类型
	UInt_2_Vec parm_ex_2arr;//完成参数 主要用来表示类型
	bool ditto;//重复循环
	uint32_t refresh;//刷新时间(单位,天)
	Oper_Activity_Reward_Info_Config reward_info;//奖励内容
};

typedef boost::unordered_map<uint32_t, Oper_Activity_Item_Line_Config> Oper_Activity_Item_Line_Config_Map;


typedef std::vector<Effect_Argv_Prop> Effect_Argv_Prop_Vec;
typedef boost::unordered_map<int32_t, Effect_Argv_Prop_Vec> Effect_Argv_Prop_Map;
struct Oper_Activity_Append_Effect_Config {
	Oper_Activity_Append_Effect_Config(void) {
		reset();
	}
	void reset(void) {
		ae_id = 0;
		prop_map.clear();
	}
	uint32_t ae_id;
	uint32_t type;
	Effect_Argv_Prop_Map prop_map; //状态加成特殊
};
typedef boost::unordered_map<uint32_t, Oper_Activity_Append_Effect_Config> Oper_Activity_Append_Effect_Config_Map;

struct Oper_Activity_Project_Config {
	Oper_Activity_Project_Config(void) {
		reset();
	}
	void reset(void) {
		act_id = 0;
		mail_id = 0;
		not_show = 0;
		item_line.clear();
		append_effect.clear();
	}
	uint32_t act_id;//活动ID
	uint32_t not_show;
	uint32_t mail_id;
	Oper_Activity_Item_Line_Config_Map item_line;//活动项里的行
	Oper_Activity_Append_Effect_Config_Map append_effect;//活动开始活动特殊效果
};
typedef boost::unordered_map<uint32_t, Oper_Activity_Project_Config> Oper_Activity_Project_Config_Map;

#endif /* OPER_ACTIVITY_CONFIG_H_ */
