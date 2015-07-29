/*
 * Public_Config.h
 *
 *  Created on: May 30, 2014
 *      Author: gaojianshan
 */

#ifndef PUBLIC_CONFIG_H_
#define PUBLIC_CONFIG_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"
#include <map>

struct Guide_Base_Config {
	Guide_Base_Config(void) { reset(); }
	void reset(void) {
		guide_id = 0;
		max_num = 0;

	}
	uint16_t guide_id; //引导id
	uint8_t max_num;   //最大引导次数
};
typedef boost::unordered_map<uint16_t, Guide_Base_Config> Guide_Config_Map;

struct First_Recharge_Item_Config {
	First_Recharge_Item_Config(void) { reset(); }
	First_Recharge_Item_Config(uint32_t item, uint32_t num_, uint32_t time_) {
		item_id = item;
		num = num_;
		time = time_;
	}
	void reset(void) {
		item_id = 0;
		num = 0;
		time = 0;
	}
	uint32_t item_id;
	uint32_t num;
	uint32_t time;
};
typedef std::vector<First_Recharge_Item_Config> First_Recharge_Item_Config_Vec;

struct First_Recharge_Reward_Config {
	First_Recharge_Reward_Config(void) { reset(); }
	void reset(void) {
		reward_item.clear();
		reward_money.clear();
		exp = 0;
	}
	First_Recharge_Item_Config_Vec reward_item;//能获取的奖励道具
	First_Recharge_Item_Config_Vec reward_money;//能获取的奖励货币
	uint32_t exp; //PT_EXP_CURRENT 能获取的经验
};

typedef First_Recharge_Reward_Config Limit_Time_Recharge_Reward_Config;

struct Recharge_Rebate_Reward_Config {
	Recharge_Rebate_Reward_Config(void) { reset(); }
	void reset(void) {
		reward_item.clear();
		reward_money.clear();
		exp = 0;
	}
	UInt_UInt_Map reward_item;//能获取的奖励道具
	UInt_UInt_Map reward_money;//能获取的奖励货币
	uint32_t exp; //PT_EXP_CURRENT 能获取的经验
};

struct Recharge_Rebate_Config {
	Recharge_Rebate_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		money = 0;
		ann_id = 0;
		act_id = 0;
		reward.reset();
	}
	uint32_t id;
	uint32_t money;//反馈金额
	uint32_t ann_id;//领取反馈广播ID
	uint32_t act_id;
	Recharge_Rebate_Reward_Config reward;
};

typedef boost::unordered_map<uint32_t, Recharge_Rebate_Config> Recharge_Rebate_Config_Map;


struct Sys_Cfg {
	Sys_Cfg() {
		reset();
	}

	void reset() {
		is_total_open = 0;
		is_open_music = 0;
		music_num = 0;
		is_open_voice = 0;
		voice_num = 0;
		is_hide_role_model = 0;
		is_hide_hero_model = 0;
		is_hide_title = 0;
		min_player_count = 0;
		max_player_count = 0;
		default_player_count = 0;
	}

	uint8_t is_total_open;          //总开关是否打开(1:开启，0:关闭)
	uint8_t is_open_music;			//是否开启音乐. 1:开启,0:关闭
	uint8_t music_num; 				//默认音乐音量
	uint8_t is_open_voice; 			//是否开启音效. 1:开启,0:关闭
	uint8_t voice_num;              //默认音效音量
	uint8_t is_hide_role_model;     //是否屏蔽角色模型. 1:是,0:否
	uint8_t is_hide_hero_model;     //是否屏蔽英雄模型. 1:是,0:否
	uint8_t is_hide_title;          //是否屏蔽称号.    1:是,0:否
	uint32_t min_player_count;      //同屏显示最少玩家数
	uint32_t max_player_count;      //同屏显示最多玩家数
	uint32_t default_player_count;  //默认同屏显示玩家数
};

// 预告系统等级配置
struct Forenotice_Cfg_Lvl {
	Forenotice_Cfg_Lvl(void) { reset(); }
	void reset(void) {
		level = 0;
		gifts.clear();
		copper = 0;
		soul = 0;
		bind_diamond = 0;
		dragon_soul = 0;
	}

	int level;
	Item_Info_Vec gifts;
	int copper;
	int soul;
	int bind_diamond;
	int dragon_soul;
};
typedef std::map<int, Forenotice_Cfg_Lvl> Forenotice_Cfg_Lvl_Map;	// 等级map有序，不要改成unordered_map

//送花系统配置
struct Flowers_Config {
	uint32_t item_id;//道具ID
	uint32_t have_msg;//是否允许寄语
	uint32_t effect;//飘花效果
	uint32_t announ;//系统广播
	uint32_t charm;//魅力值增加数
	uint32_t friend_num;//友好度增加数

	Flowers_Config(void) { reset(); }
	void reset(void) {
		item_id = 0;
		have_msg = 0;
		have_msg = 0;
		announ = 0;
		charm = 0;
		friend_num = 0;
	}
};
typedef boost::unordered_map<uint32_t, Flowers_Config> Flowers_Config_Map;

//引导完成配置
struct Guide_Finish_Cfg {
	Guide_Finish_Cfg() {
		reset();
	}
	void reset() {
		guide_id = 0;
		param1 = 0;
		param2 = 0;
		param1_list.clear();
	}
	uint16_t guide_id;//引导id
	uint64_t param1;//参数1
	uint64_t param2;//参数2
	Int_Set  param1_list; //参数数组
};

//type
typedef std::multimap<uint16_t, Guide_Finish_Cfg> Guide_Finish_Cfg_Map;

enum Notice_Type {
	NOTICE_TYPE_TASK = 1,
	NOTICE_TYPE_LEVEL = 2,
	NOTICE_TYPE_TASK_DELETE_NOTICE = 3,
};

struct Notice_Cfg {
	Notice_Cfg() {
		reset();
	}
	void reset() {
		id = 0;
		type = 0;
		task_id = 0;
		task_status = 0;
		role_level = 0;
	}
	uint32_t id;
	uint8_t type;
	uint32_t task_id;
	uint8_t task_status;
	uint32_t role_level;
};

typedef boost::unordered_map<uint32_t, Notice_Cfg> Notice_Cfg_Map;

struct Challenge_Area_Cfg {
	void Challeng_Area_Cfg() {
		reset();
	}
	void reset() {
		left_border = 0;
		right_border = 0;
		up_border = 0;
		down_border = 0;
	}
	uint32_t left_border;
	uint32_t right_border;
	uint32_t up_border;
	uint32_t down_border;
};

typedef boost::unordered_map<uint32_t, Challenge_Area_Cfg> Challenge_Area_Cfg_Map;

struct Challenge_Cfg {
	Challenge_Cfg() {
		reset();
	}
	void reset() {
		ganger_devote = 0;
		challenge_area_cfg_map.clear();
	}
	uint32_t ganger_devote;
	Challenge_Area_Cfg_Map challenge_area_cfg_map;
};

struct Questionnaire_Reward_Config {
	Questionnaire_Reward_Config(void) { reset(); }
	void reset(void) {
		reward_item.clear();
		reward_money.clear();
		exp = 0;
	}
	UInt_UInt_Map reward_item;//能获取的奖励道具
	UInt_UInt_Map reward_money;//能获取的奖励货币
	uint32_t exp; //PT_EXP_CURRENT 能获取的经验
};
typedef std::map<uint8_t, Questionnaire_Reward_Config> Questionnaire_Config_Map;

struct Invite_Item_Config {
	Invite_Item_Config(void) { reset();}
	void reset(void) {
		item_id = 0;
		num = 0;
		bind = 0;
	}
	uint32_t item_id;
	uint32_t num;
	uint32_t bind;
};
typedef std::vector<Invite_Item_Config> Invite_Item_Config_Vec;

struct Invite_Reward_Config {
	Invite_Reward_Config(void) { reset(); }
	void reset(void) {
		reward_item.clear();
		reward_money.clear();
		exp = 0;
	}
	Invite_Item_Config_Vec reward_item;//能获取的奖励道具
	Invite_Item_Config_Vec reward_money;//能获取的奖励货币
	uint32_t exp; //PT_EXP_CURRENT 能获取的经验
};

//爵位公告
struct Nobility_Config {
	uint32_t exploit_lvl;
	uint32_t exploit_val;//需要功勋值
	uint32_t broadcast_id;//系统广播
	std::string name; //功勋名称
	Nobility_Config(void) { reset(); }
	void reset(void) {
		exploit_lvl = 0;
		exploit_val = 0;
		broadcast_id = 0;
		name.clear();
	}
};
typedef boost::unordered_map<uint32_t, Nobility_Config> Nobility_Config_Map;

#endif /* PUBLIC_CONFIG_H_ */
