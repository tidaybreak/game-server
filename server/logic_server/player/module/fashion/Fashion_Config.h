/*
 * FashionConfig.h
 *
 *  Created on: 2014年9月3日
 *      Author: root
 */

#ifndef FASHION_CONFIG_H_
#define FASHION_CONFIG_H_

#include "Base_Struct.h"
#include "Game_Typedef.h"

typedef boost::unordered_map<int, Int_Int_Map> Fashion_Avatar_Map;
struct Fashion_Base_Cfg {
	Fashion_Base_Cfg(void) { reset(); }
	void reset(void) {
		id = 0;
		exp = 0;
		hidden_props_map.clear();
		wore_props_map.clear();
	}

	int id;
	int exp;
	int part;
	int suit_id;
	Int_Double_Map hidden_props_map;
	Int_Double_Map wore_props_map;
	Fashion_Avatar_Map fashion_avatar_map;
};
typedef boost::unordered_map<int, Fashion_Base_Cfg> Fashion_Base_Map;

struct Fashion_Soul_Cfg {
	Fashion_Soul_Cfg(void) { reset(); }
	void reset(void) {
		lv = 0;
		exp = 0;
		props_map.clear();
	}

	int lv;
	int exp;
	Int_Double_Map props_map;

};
typedef boost::unordered_map<int, Fashion_Soul_Cfg> Fashion_Soul_Map;

struct Fashion_Reinforced_Cfg {
	Fashion_Reinforced_Cfg(void) { reset(); }
	void reset(void) {
		lv = 0;
		part = 0;
		percent = 0;
		coin_cost = 0;
		suc_rate = 0;
		exp_add = 0;
		exp_max = 0;
		wore_props_map.clear();
		item_cost.reset();
	}

	int lv;
	int part;
	int percent;
	int coin_cost;
	int suc_rate;
	int exp_add;
	int exp_max;
	Int_Double_Map wore_props_map;
	Int_Int item_cost;
};
typedef boost::unordered_map<int, Fashion_Reinforced_Cfg> Fashion_Reinforced_Part_Map;
typedef boost::unordered_map<int, Fashion_Reinforced_Part_Map> Fashion_Reinforced_Map;

struct Fashion_Suit_Cfg {
	Fashion_Suit_Cfg(void) { reset();  }
	void reset(void) {
		id = 0;
		fashion_id_vec.clear();
		props_map.clear();
	}

	int id;
	Int_Vec fashion_id_vec;
	Int_Double_Map props_map;
};
typedef boost::unordered_map<int, Fashion_Suit_Cfg> Fashion_Suit_Map;

#endif /* FASHION_CONFIG_H_ */
