/*
 * Item_User_Config.h
 *
 *  Created on: 2014年3月26日
 *      Author: Linqiyou
 */

#ifndef ITEM_USER_CONFIG_H_
#define ITEM_USER_CONFIG_H_

#include "Base_Struct.h"
#include "status/Status_Struct.h"
#include "pack/Pack_Struct.h"

struct Item_Gain_Source_Detail {
	Item_Gain_Source_Detail(void) { reset(); }
	void reset(void) {
		type = 0;
		name.clear();
	}
	int type;
	std::string name;
};
typedef boost::unordered_map<int, Item_Gain_Source_Detail> Item_Gain_Source_Cfg;

struct Quick_Buy {
	Quick_Buy(void) { reset(); }
	void reset(void) {
		id = 0;
		price = 0;
		type = 0;
	}
	int id;
	int price;
	int8_t type;
};
typedef boost::unordered_map<int, Quick_Buy> Quick_Buy_Config;

// ITEM CONFIG ==================================

struct Item_Prop_Effect_Config {
	Item_Prop_Effect_Config(void) { reset(); }
	void reset(void) {
		name.clear();
		value = 0;
		max_level = 0;
		exp = 0;
	}

	std::string name; //物品功能
	int value;
	int max_level;//升级药：使用物品能达到的最大等级
	int exp;//升级药：使用物品获得经验
};

struct Item_Prop_Detail_Config {
	Item_Prop_Detail_Config(void) { reset(); }
	void reset(void) {
		item_id = 0;
		item_effect_type = 0;
		target = 0;
		effect.reset();
	}

	uint32_t item_id;
	int item_effect_type;//物品效果类型,默认为正常使用物品
	uint8_t target;//使用对像
	Item_Prop_Effect_Config effect;
	int status;//物品赋加的状态
	int vip;//物品给予的vip等级
};
typedef boost::unordered_map<uint32_t, Item_Prop_Detail_Config> Item_Prop_Config;

//掉落物
struct Item_Lottery_Item_Info_Config {
	Item_Lottery_Item_Info_Config(void) { reset(); }
	void reset(void) {
		count = 0;
		level_low = -1;
		level_high = -1;
		drate_rate = 0;
		item_list.clear();
		item_list_ck.clear();
		item_list_yss.clear();
		weight.clear();
		type = 0;
		cost.clear();
		item_set.clear();
	}

	int count;//概率执行次数(默认1)
	int level_low;//等级限制-1
	int level_high;//等级限制-1
	int drate_rate;//物品概率,100制的
	Item_Info_Vec item_list;
	Item_Info_Vec item_list_ck;
	Item_Info_Vec item_list_yss;
	Int_Vec weight;//物品列表的权重比例:(随机数%所有和)与对应项比大小
	int8_t type; // 掉落类型
	Item_Info_Vec cost; // 消耗
	Int_Hash_Set item_set; // 开礼包时，需要被广播的道具
};
typedef std::vector<Item_Lottery_Item_Info_Config> Item_Lottery_Item_Info_Vec_Config;
struct Item_Lottery_Detail_Config {
	Item_Lottery_Detail_Config(void) { reset(); }
	void reset(void) {
		item_id = 0;
		item_list.clear();
	}

	uint32_t item_id;
	Item_Lottery_Item_Info_Vec_Config item_list;
};
typedef boost::unordered_map<uint32_t, Item_Lottery_Detail_Config> Item_Lottery_Config;

struct Item_Little_Fuction_Config {
	Item_Little_Fuction_Config(void) {reset();}
	void reset(void) {
		type = 0;
		arg = 0;
		arg1 = 0;
	}
	int type;
	int arg;
	int arg1;
};
typedef boost::unordered_map<uint32_t, Item_Little_Fuction_Config> Item_Little_Fuction_Map;

struct Item_Drug_Detail {
	Item_Drug_Detail(void) { reset(); }
	void reset(void) {
		val = 0.0;
		usetype = 0;
	}

	double val;
	int8_t usetype;
};
typedef boost::unordered_map<uint32_t, Item_Drug_Detail> Item_Drug_Cfg;

struct Item_Material_Detail {
	Item_Material_Detail(void) { reset(); }
	void reset(void) {
		from_table.clear();
	}

	std::string from_table;
};
typedef boost::unordered_map<uint32_t, Item_Material_Detail> Item_Material_Cfg;

struct Item_Card_Detail {
	Item_Card_Detail(void) { reset(); }
	void reset(void) {
		deadline = 0;
		vip_lvl = 0;
		covenant_type = 0; // 精灵卡类型
	}

	int deadline;
	int vip_lvl;
	int covenant_type;
};
typedef boost::unordered_map<uint32_t, Item_Card_Detail> Item_Card_Cfg;

struct Item_Resource_Detail {
	Item_Resource_Detail(void) { reset(); }
	void reset(void) {
//		val = 0.0;
//		prop = 0;
		gain_list.clear();
	}
//	double val;
//	int32_t prop;
	std::vector<Int_Int> gain_list;
};
typedef boost::unordered_map<uint32_t, Item_Resource_Detail> Item_Resource_Cfg;

typedef boost::unordered_map<int, Int_Int_Map> Equip_Avatar_Map;
typedef std::vector<Int_Double> Prop_Value_Vec;
struct Item_Equip_Cfg {
	Item_Equip_Cfg(void) { reset(); };
	void reset(void) {
		career = 0;
		part = 0;
		equip_lvl = 0;
		max_unban_lvl = 0;
		suit_id = 0;
		hero_id = 0;
		init_card_holes = 0;
		card_holes = 0;
		gemstone_holes.reset();
		avatar_map.clear();
		skills.clear();
		prop_val_vec.clear();
	}

	int career;
	int part;
	int equip_lvl;
	int max_unban_lvl;
	int suit_id;
	int hero_id;
	int init_card_holes;
	int card_holes;
	Int_Int gemstone_holes;
	Equip_Avatar_Map avatar_map;
	Int_Vec skills;
	Prop_Value_Vec prop_val_vec;
};

struct IU_Hero {
	void reset(void) {
		id = 0;
		wake_lvl = 0;
		lvl = 0;
	}
	int id;
	int wake_lvl;
	int lvl;
};
struct Item_Special_Cfg {
	Item_Special_Cfg(void) { reset(); };
	void reset(void) {
		lucky_stone_rate = 0;
		rune_essence_energy = 0;
		trigger_buff.reset();
		insert_hero = false;
		type = 0; // type:0历史数据（幸运石、符文、buff）
		memset(&addon, 0x00, sizeof(addon));
	}

	int lucky_stone_rate;
	int rune_essence_energy;
	Status_Effect_Argv trigger_buff;//触发BUFF
	bool insert_hero; // 1:该buff也对英雄生效
	int type;
	union Addon {
		int mount_skill;
		IU_Hero iu_hero;
	};
	Addon addon;
};

struct Item_Rune_Stone_Cfg {
	Item_Rune_Stone_Cfg(void) { reset(); };
	void reset(void) {
		rune_type = 0;
		need_points = 0;
		combine_id = 0;
		prop_val_map.clear();
		need_id.clear();
	}

	int rune_type;
	int need_points;
	int combine_id;
	Int_Double_Map prop_val_map;
	Id_Amount_Vec need_id;
};

struct Item_Gang_Cfg {
	Item_Gang_Cfg(void) { reset(); };
	void reset(void) {
		cost_contrib = 0;
		need_shop_lvl = 0;
	}

	int cost_contrib;
	int need_shop_lvl;
};

struct Item_Gemstone_Cfg {
	Item_Gemstone_Cfg(void) { reset(); }
	void reset(void) {
		type = 0;
		lv = 0;
		attr.reset();
	}

	int type;
	int lv;
	Int_Double attr;
};

struct Item_Detail_Config {
	Item_Detail_Config(void) { reset(); };
	void reset(void) {
		type = 0;
		name.clear();
		item_id = 0;
		needLevel = 0;
		occupation = 0;
		sellPrice = 0;
		marketPrice = 0;
		sys_sell = 0;
		sys_sell_type = 0;
		bind = 0;
		notAbandon = 0;
		rare = 0;
		overlay = 0;
		time = Time_Value::zero;
		clickHandle = 0;
		moduleId = 0;
		scene.clear();
		timeLimit.clear();
		cdTime = 0;
		cool_type = 0;
		color = 0;
		fashion_upgrade_type = 0;
		equip_cfg.reset();
		special_cfg.reset();
		rune_stone.reset();
		gang.reset();
		gemstone_cfg.reset();
		next_spar_id = 0;
	}

	int type;//物品类型
	std::string name;// mail
	uint32_t item_id;//道具ID
	int needLevel;//使用等级
	int8_t occupation;//职业限制
	int sellPrice;//出售价格
	int marketPrice;//寄售底价
	int sys_sell; // 系统售价
	int sys_sell_type; // 系统售价类型
	int bind;//是否绑定 1.未绑定（装备：装备后绑定） 2.已绑定
	int8_t notAbandon;//1.可丢弃 2.不可丢弃
	int rare;//1.垃圾 2.普通 3.贵重
	int overlay;//堆叠数
	Time_Value time;//时效
	/* 单击操作
	 * 1.使用（装备类：装备）（礼包类：打开开）
	 * （功能类：使用）匹配功能ID
	 * 2.批量使用
	 * 3.丢弃
	 * 4.合成（装备制作图纸：制作）
	 * 5.拆分
	 * 6.强化
	 * 7.展示
	 * 根据数组顺序改变显示顺序
	 */
	int clickHandle;
	int moduleId;//功能ID
	Int_Vec scene;//场景限制
	Int_Vec timeLimit;//时段限制
	int cdTime;//冷却时间
	int cool_type;
	int8_t color;
	int8_t fashion_upgrade_type;
	uint32_t next_spar_id;//晶石精练后得到的下一高品质id

	Item_Equip_Cfg equip_cfg;
	Item_Special_Cfg special_cfg;
	Item_Rune_Stone_Cfg rune_stone;
	Item_Gang_Cfg gang;
	Item_Gemstone_Cfg gemstone_cfg;
};
typedef boost::unordered_map<uint32_t, Item_Detail_Config> Item_Config;

typedef boost::unordered_map<int, Int_Int> Gemstone_Hole_Config;
struct Gemstone_Ext_Attr {
	Gemstone_Ext_Attr(void) { reset(); }
	void reset(void) {
		color = 0;
		num = 0;
		attr_map.clear();
	}

	int color;
	int num;
	Int_Double_Map attr_map;
};
typedef std::vector<Gemstone_Ext_Attr> Gemstone_Ext_Attr_Config;

// ITEM CONFIG ==================================



#endif /* ITEM_USER_CONFIG_H_ */
