/*
 * Pack_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef PACK_STRUCT_H_
#define PACK_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Struct.h"
#include "Pack_Def.h"

struct Money;
struct Item_Basic_Info;
struct Item_Id_Amount;
struct Item_Equip_Info;
struct Item_Rune_Stone_Info;
struct Card_Info;
class Packer;

struct Money_Detail {
	int32_t gold;					// 金币
	int32_t diamond;				// 钻石
	int32_t bind_diamond;		// 粉钻
	int32_t dragon_soul; 		// 龙魂
	int32_t souls; 				// 英灵
	int32_t friendship; 			// 友情值
	int32_t reputation;			//	声望

	// 玩家之间的交易 不含系统
	int32_t today_diamond_pay;
	int32_t today_diamond_get;
	int32_t today_gold_pay;
	int32_t today_gold_get;

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	Money money(void) const;
};

struct Enchant_Prop_Detail {
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	int32_t prop_type;/*属性类型*/
	int32_t prop_val;/*属性值*/
	int8_t star;/*星数*/
	int8_t is_lock;/*是否锁，1是0否*/
};

struct Enchant_Prop_Group_Detail {
	enum {
		MAX_ENCHANT_PROP = 5,
	};
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	Enchant_Prop_Detail enchant_props[MAX_ENCHANT_PROP];
	int32_t score;
	uint8_t cnt_enchant_prop;										// 该组当前附魔属性数
};

struct Enchant_Lock_Prop_Fail {
	int32_t prop;/**/
	int32_t fail_cnt;/**/

	void serialize(Block_Buffer & w) const {
		w.write_int32(prop);
		w.write_int32(fail_cnt);
	}
	int deserialize(Block_Buffer & r) {
		if( r.read_int32(prop)  ||  r.read_int32(fail_cnt)  )
				return -1;
 		return 0;
	}
	void reset(void) {
		prop = 0;
		fail_cnt = 0;
	}
};

struct GemStone {
	int8_t state;/**/
	int32_t stone_id;/**/

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
		w.write_int32(stone_id);
	}
	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  ||  r.read_int32(stone_id)  )
				return -1;
 		return 0;
	}
	void reset(void) {
		state = 0;
		stone_id = 0;
	}
};

struct CardHole {
	int8_t state;/**/
	uint32_t card_index;/**/

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
		w.write_uint32(card_index);
	}
	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  ||  r.read_uint32(card_index)  )
				return -1;
 		return 0;
	}
	void reset(void) {
		state = 0;
		card_index = 0;
	}
};

struct Equip_Addon_Detail {
	enum {
		MAX_EQUIP_HOLE = 5,
		MAX_ENCHANT_PROP_GROUP = 8,
		//MAX_ENCHANT_PROP = 5,
		MAX_CARD_HOLE = 3,
		MAX_GEMSTONE_HOLE = 4,
	};

	void serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	void reset(void);
	int init_equip_addon(int item_id);
	void make_item_equip_info(Item_Equip_Info &info) const;
	void make_item_equip_info(Item_Equip_Info &info, Packer &packer) const;

	uint32_t index;
	int32_t unban_lvl;	// 解封等级
	int32_t score;
	int32_t force;

	uint8_t incr_level;						//当前精通等级
	int32_t incr_luck_value;					//精通幸运值
	int16_t incr_lvl_conti_fail_cnt;			//精通等级连续失败次数 (不发客户端)

	int enchant_times;
	uint8_t cnt_enchant_group;														// 当前附魔属性组数
	Enchant_Prop_Group_Detail enchant_groups[MAX_ENCHANT_PROP_GROUP];				// 附魔属性组
	Enchant_Lock_Prop_Fail enchant_lock_prop_fail[Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP];	// 锁定属性失败信息: 锁定属性,失败的附魔组数（该组没洗出锁定属性算失败）

	CardHole card_hole_array_[MAX_CARD_HOLE];
	GemStone gemstones[MAX_GEMSTONE_HOLE];
};

struct Rune_Stone_Addon_Detail {
	void serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	void reset(void);
	int init_rune_stone_addon(int item_id);
	void make_item_rune_stone_info(Item_Rune_Stone_Info &info) const;

	uint32_t index;
	int32_t exp_lvl;
	int32_t exp;
	int8_t is_lock;
};

struct Card_Addon_Detail {
	void serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	void reset(void);
	int init_card_info(int item_id);
	void make_item_card_info(int id, int idx, Card_Info &info) const;

	int part_;
	int poten_;
	int star_;
	int lv_;
	int exp_;
	int main_attr_;
	int sub_attr_;
	uint equip_id;
	uint8_t equip_loc;
};

const int ITEM_INDEX_GAP = 10000000;
struct Item_Detail {
	enum Item_Type {
		NORMAL 				= 0,
		EQUIP 				= 1,	// 装备
		TOOL 				= 2,
		DRUG 				= 3,
		RUNE_STONE 			= 4, 	// 符石
		CARD				= 5,	// 卡牌
		TOOL_TASK 			= 201,  // 任务物品
		TOOL_LOTTERY 		= 202,  // 礼包
		TOOL_FLOWER 		= 203,  // 鲜花
		TOOL_DRUG 			= 204,  // 血包
		TOOL_MATERIAL 		= 205,  // 强化材料
		TOOL_CARD			= 206,  // 卡
		TOOL_RESOURCE		= 207,  // 资源
		TOOL_CONSUME_TOOL	= 208,  //消耗道具
		TOOL_ROLL			= 209,  //卷轴

		TOOL_GEMSTONE		= 215,	// 宝石
	};

	enum Item_Index {
		ITEM_START				= 10000000,
		ITEM_END				= 60000000,
		EQUIP_START 			= 10000000,
		EQUIP_END 				= 20000000,
		TOOL_START 				= 20000001,//道具
		TOOL_END				= 30000000,
		TOOL_TASK_START 		= 20100001,
		TOOL_TASK_END			= 20200000,
		TOOL_LOTTERY_START		= 20200001,
		TOOL_LOTTERY_END		= 20300000,
		TOOL_FLOWER_START		= 20300001,
		TOOL_FLOWER_END			= 20400000,
		TOOL_DRUG_START			= 20400001,
		TOOL_DRUG_END			= 20500000,
		TOOL_MATERIAL_START		= 20500001,
		TOOL_MATERIAL_END		= 20600000,
		TOOL_CARD_START			= 20600001,
		TOOL_CARD_END			= 20700000,
		TOOL_RUNE_STONE_START 	= 20700001,
		TOOL_RUNE_STONE_END 	= 20800000,
		DRUG_START				= 30000001,//药品
		DRUG_END				= 40000000,
		RUNE_STONE_START		= 40000001,
		RUNE_STONE_END 			= 50000000,
		CARD_START 				= 50000001,
		CARD_END 				= 60000000,
	};

	enum Item_Color {
		COLOR_BEGIN 		= 1,
		WHITE						= 1, //1.白
		GREEN					= 2, //2.绿
		BLUE						= 3, //3.蓝
		PURPLE					= 4, //4.紫
		GOLDEN_PURPLE 	= 5, //5.金边紫
		ORANGE					= 6, //6.橙
		RED							= 7,
		COLOR_END,
	};

	enum Bind_Type {
		UNDEFINED = 0,
		UNBIND = 1,
		BIND = 2,
//		DEFINE_BY_CFG = 3
	};

	Item_Detail(void);
	explicit Item_Detail(uint32_t id, int32_t amount, uint8_t bind);
	explicit Item_Detail(const Item_Basic_Info &item);
	virtual ~Item_Detail();
	int init();

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	void set_basic(const uint32_t index, const uint32_t id, const int32_t amount, const uint8_t bind);

	static int is_item_type(const uint32_t item_id, Item_Type item_type);
	static int get_item_type(const uint32_t item_id, Item_Type &item_type);
	int get_item_type(void) const;
	int get_item_type_second(void) const;
	int get_item_color(void) const;
	static int32_t get_item_stack_upper(const uint32_t item_id);
	bool is_item_nontradable(void) const;
	int get_item_remain_amount(void);

	friend bool operator<(const Item_Detail &item1, const Item_Detail &item2);
	friend bool less_detail(const Item_Detail &item1, const Item_Detail &item2);
	bool operator == (const Item_Detail &cmp) const;

	// 同类道具，如都是白铁剑，但绑定状态不一样
	friend bool is_similar_item(const Item_Detail &item1, const Item_Detail &item2);

	// 完全相等的道具
	friend bool is_equal_item(const Item_Detail &item1, const Item_Detail &item2);

	void set_item_basic_info(Item_Basic_Info &item_base) const;
	void get_item_basic_info(const Item_Basic_Info &item_base);
	bool set_item_card_info(Card_Info &card_info) const;

	uint32_t index_;/*位置标识符*/
	uint32_t seq_;/*序列，绝不重复*/
	uint32_t id_;/*道具id（图标id整合到此id）*/
	int32_t amount_;/*总量*/
	uint8_t bind_;/*是否绑定（0,1）不绑，（2）已绑 其他值非法*/
	Item_Type type_;
	Time_Value overdue_time;

	union Addon {
		Equip_Addon_Detail equip;
		Rune_Stone_Addon_Detail rune_stone;
		Card_Addon_Detail card;
	};
	Addon addon;
};

inline std::size_t hash_value(const Item_Detail &item) {
    std::size_t seed = 0;
    boost::hash_combine(seed, item.id_);
    boost::hash_combine(seed, item.bind_);
    return seed;
}
typedef std::vector<Item_Detail> Item_Vec;
typedef std::map<uint32_t, Item_Detail*> Goods_Map;
typedef boost::unordered_map<uint32_t, Item_Detail*> Goods_Seq_Map;


enum MODULE_TYPE {
	PACK_TYPE = 0,
	SHOP_TYPE = 1,
	MARKET_TYPE = 2
};

using namespace Pack;

struct Id_Amount {
	Id_Amount(uint32_t id_= 0, int32_t amount_ = 0, Bind_Verify bind_verify_ = BIND_NONE) :
			id(id_), amount(amount_), bind_verify(bind_verify_) {
	}
	uint32_t id;
	int32_t amount;
	Bind_Verify bind_verify;
	friend bool operator<(const Id_Amount &id_amount1, const Id_Amount &id_amount2);

	Id_Amount& operator= (const Item_Id_Amount& id_amount);
};

enum {
	Move_All = 999
};
typedef std::vector<Id_Amount> Id_Amount_Vec;
struct Index_Amount {
	Index_Amount(uint32_t index_ = 0, int32_t amount_ = 0, Bind_Verify bind_verify_  = BIND_NONE) :
			index(index_), amount(amount_), bind_verify(bind_verify_) {
	}
	uint32_t index;
	int32_t amount;
	Bind_Verify bind_verify;

	int serialize(Block_Buffer &buf) const{
		buf.write_uint32(index);
		buf.write_int32(amount);
		buf.write_uint8(bind_verify);
		return 0;
	}
	int deserialize(Block_Buffer &buf) {
		buf.read_uint32(index);
		buf.read_int32(amount);
		uint8_t bind_verify_ = 0;
		buf.read_uint8(bind_verify_);
		bind_verify = static_cast<Bind_Verify>(bind_verify_);
		return 0;
	}

	friend bool operator<(const Index_Amount &index_amount1, const Index_Amount &index_amount2) {
		if (index_amount1.index < index_amount2.index) return true;
		if (index_amount1.index == index_amount2.index && index_amount1.bind_verify > index_amount2.bind_verify) return true;
		return false;
	}
};
typedef std::vector<Index_Amount> Index_Amount_Vec;

// Data Mining
struct Money_DM_Info {
	Money_DM_Info(uint16_t type_,
		uint32_t item_id_ = 0, uint32_t item_num_ = 0, int64_t ext_1_ = 0, int64_t ext_2_ = 0):
		type(type_), item_id(item_id_), item_num(item_num_), ext_1(ext_1_), ext_2(ext_2_) {}

	void reset(void) {
		type = 0;
		item_id = 0;
		item_num = 0;
		ext_1 = 0;
		ext_2 = 0;
	}

	uint16_t type;		// 流水类型

	// 记录购买的物品与物品数量
	uint32_t item_id;
	uint32_t item_num;

	int64_t ext_1;		// 拓展字段1
	int64_t ext_2;		// 拓展字段2
};


struct Money_Stream_Info {
	Money_Stream_Info(void): dm_info(0) { reset(); }

	void reset(void);
	void set_basic_info(role_id_t role_id_, uint32_t opt_time_sec_, uint32_t money_type1_,
		uint32_t money_chg1_, uint32_t money_cur1_, const Money_DM_Info &dm_info_);
	void set_extra_money_info(uint32_t money_type2_, uint32_t money_chg2_, uint32_t money_cur2_);

	role_id_t role_id;
	uint32_t money_type1;
	uint32_t money_chg1;
	uint32_t money_cur1;
	uint32_t money_type2;
	uint32_t money_chg2;
	uint32_t money_cur2;
	uint32_t opt_time_sec;
	Money_DM_Info dm_info;
};

// 若要使用引用减少一些值拷贝，可以参考这里。不过会让代码变得古怪
// http://stackoverflow.com/questions/634662/non-static-const-member-cant-use-default-assignment-operator
struct Money_Add_Info {
	explicit Money_Add_Info(Money_Type type_, int nums_, const Money_DM_Info &dm_info_) : type(type_), nums(nums_), dm_info(dm_info_) {}

	Money_Type type;
	int nums;
	Money_DM_Info dm_info;
};

typedef std::vector<Money_Add_Info> Money_Add_List;

struct Money_Sub_Info {
	explicit Money_Sub_Info(Money_Sub_Type type_, int nums_, const Money_DM_Info &dm_info_) : type(type_), nums(nums_), dm_info(dm_info_) {}

	Money_Sub_Type type;
	int nums;
	Money_DM_Info dm_info;
};

typedef std::vector<Money_Sub_Info> Money_Sub_List;

inline void Money_Stream_Info::reset(void) {
	role_id = 0;
	money_type1 = 0;
	money_chg1 = 0;
	money_cur1 = 0;
	money_type2 = 0;
	money_chg2 = 0;
	money_cur2 = 0;
	opt_time_sec = 0;
	dm_info.reset();
	// bzero(this, sizeof(Money_Stream_Info));
}

inline void Money_Stream_Info::set_basic_info(role_id_t role_id_, uint32_t opt_time_sec_, uint32_t money_type1_, uint32_t money_chg1_, uint32_t money_cur1_,
		const Money_DM_Info &dm_info_) {
	role_id = role_id_;
	opt_time_sec = opt_time_sec_;
	money_type1 = money_type1_;
	money_chg1 = money_chg1_;
	money_cur1 = money_cur1_;
	dm_info = dm_info_;
}

inline void Money_Stream_Info::set_extra_money_info(uint32_t money_type2_, uint32_t money_chg2_, uint32_t money_cur2_) {
	money_type2 = money_type2_;
	money_chg2 = money_chg2_;
	money_cur2 = money_cur2_;
}

struct Gain_Item_DM_Info {
	Gain_Item_DM_Info(Item_Gain_Type type_, int64_t ext_1_ = 0, int64_t ext_2_ = 0) : type(type_), ext_1(ext_1_), ext_2(ext_2_) {}

	uint16_t type;
	int64_t ext_1;
	int64_t ext_2;
};

struct Lose_Item_DM_Info {
	Lose_Item_DM_Info(Item_Lose_Type type_, int64_t ext_1_ = 0, int64_t ext_2_ = 0) : type(type_), ext_1(ext_1_), ext_2(ext_2_) {}

	uint16_t type;
	int64_t ext_1;
	int64_t ext_2;
};

struct Move_Item_DM_Info {
	Move_Item_DM_Info(Item_Move_Type type_, int64_t ext_1_ = 0, int64_t ext_2_ = 0) : type(type_), ext_1(ext_1_), ext_2(ext_2_) {}

	uint16_t type;
	int64_t ext_1;
	int64_t ext_2;
};

//-- 背包容量信息
struct Capacity {
	uint16_t pack_cap;
	uint16_t storage_cap;
	uint16_t spirit_equip_cap;
	uint16_t spirit_package_cap;
	uint16_t rune_stone_pack_cap;
	uint16_t rune_stone_equip_cap;
	uint16_t thorlottery_cap;
	uint16_t card_cap;
	//Int_Set hero_rune_stone_equip_cap;	// 有规律，暂不存

	const static uint16_t equip_cap;
	const static uint16_t achievement_cap;
	const static uint16_t rune_stone_smelt_cap;
	const static uint16_t hero_equip_cap;
};

struct Packer_Detail : public Detail {
	Packer_Detail();
	virtual ~Packer_Detail();
	virtual void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	virtual int load(void);
	virtual int save(void);

	inline void save_tick(void) { is_change = true; }

	Packer_Detail &operator=(const Packer_Detail &detail);

	// 仅背包内部用!!
	void erase(uint32_t index);
	void erase(Goods_Map::iterator iter);
	void erase(Goods_Map::iterator begin, Goods_Map::iterator end);

	role_id_t role_id;
	Capacity capacity;
	Money_Detail money_detail;
	Goods_Map goods_map;
	Int_Int_Map money_lock_map;		// 锁定后只能加不能减少
	Int_Int_Map item_lock_map;
	Time_Value last_wipe_time;
};

#endif /* PACK_STRUCT_H_ */
