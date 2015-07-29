/*
 * Card_Config.h
 *
 *  Created on: 2014年11月25日
 *      Author: root
 */

#ifndef CARD_CONFIG_H_
#define CARD_CONFIG_H_

#include "Base_Struct.h"
#include "Game_Typedef.h"

const static int EXP_CARD_TYPE = 1000;

typedef boost::unordered_map<int, Int_Int_Map> Card_Sub_Attr_Map;

typedef Int_Double_Map Card_Attr_Lv_Map;
typedef boost::unordered_map<int, Card_Attr_Lv_Map> Card_Lv_Map;

struct Card_Label {
	Card_Label(void) { reset(); }
	Card_Label(int qlt, int tp, int lvl);
	void reset(void);
	inline bool operator== (const Card_Label &b) const;
	inline bool operator!= (const Card_Label &b) const;

	int quality;
	int type;
	int lv;
};

inline bool Card_Label::operator== (const Card_Label &b) const {
	return (quality == b.quality && type == b.type && lv == b.lv);
}

inline bool Card_Label::operator!= (const Card_Label &b) const {
	return !(*this == b);
}

inline std::size_t hash_value(const Card_Label &key) {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.quality);
    boost::hash_combine(seed, key.type);
    boost::hash_combine(seed, key.lv);
    return seed;
}

struct Card_Lv_Data {
	Card_Lv_Data(void) { reset(); }
	void reset();

	Card_Label card_label;
	int coin;
	int init_exp;
	int exp;
	int price;
	int merge_rate;
	int merge_cost;
};
typedef boost::unordered_map<Card_Label, Card_Lv_Data> Card_Lv_Data_Map;

struct Item_Cost {
	Item_Cost(void) { reset(); }
	void reset();

	int id;
	int amount;
	int bind;
};
typedef std::vector<Item_Cost> Item_Cost_Vec;

struct Card_Poten_Interval {
	Card_Poten_Interval(void) { reset(); }
	void reset(void);

	int id;
	int merge_poten_rate;
	Int_Int interval;
	Int_Int merge_potent_interval;
	Item_Cost_Vec item_cost_vec;
	int copper_cost;
};
typedef boost::unordered_map<int, Card_Poten_Interval> Card_Poten_Interval_Map;

struct Poten_Upgrade_Config {
	Poten_Upgrade_Config(void) { reset(); }
	void reset(void) {
		weight = 0;
		min = 0;
		max = 0;
	}

	int weight;
	int min;
	int max;
};
typedef std::vector<Poten_Upgrade_Config> Poten_Upgrade_Config_Vec;

struct Card_Attr_Config {
	Card_Attr_Config(void) { reset(); }
	void reset(void);

	int id;								// 属性id
	Int_Double_Map attr_lv_map;			// 属性值
};
typedef boost::unordered_map<int, Card_Attr_Config> Card_Attr_Config_Map;

struct Card_Config {
	Card_Config(void) { reset(); }
	void reset(void);

	int id;
	int type;
	int max_star;
	int sub_attr_rate;
	int init_lv;
	int max_lv;
	Int_Int weapon;
	Int_Int hat;
	Int_Int cloth;
	Int_Int shoe;
	Int_Int necklace;
	Int_Int ring;
	Int_Int_Map poten_weight_map;
	Int_Int_Map sub_attr_map;
};
typedef boost::unordered_map<int, Card_Config> Card_Config_Map;

typedef boost::unordered_map<int, Int_Int> Vip_Upgrade_Card_Map;

#endif /* CARD_CONFIG_H_ */
