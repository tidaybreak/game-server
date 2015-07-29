/*
 * Pack_Config.h
 *
 *  Created on: 2014年3月26日
 *      Author: Linqiyou
 */

#ifndef PACK_CONFIG_H_
#define PACK_CONFIG_H_


// PACKAGE CONFIG ==================================

struct Trade_Limit_Config {
	Trade_Limit_Config(void) { reset(); }
	void reset(void) {
		gold_pay_limit = 0;
		gold_get_limit = 0;
		copper_pay_limit = 0;
		copper_get_limit = 0;
	}

	int gold_pay_limit;
	int gold_get_limit;
	int copper_pay_limit;
	int copper_get_limit;
};

struct Line_Capacity_Detail {
	Line_Capacity_Detail(void){ reset(); }
	void reset(void) {
		line = 0;
		grid = 0;
		lvl = 0;
		gold = 0;
	}

	uint8_t line;//哪一行
	uint16_t grid;//相对总的处于哪一格
	uint32_t lvl;
	uint32_t gold;
};
typedef boost::unordered_map<uint16_t, Line_Capacity_Detail> Line_Capacity_Conf;

struct Rune_Stone_Pack_Grid_Cfg {
	Rune_Stone_Pack_Grid_Cfg(void){ reset(); }
	void reset(void) {
		grid = 0;
		diamond = 0;
	}

	int grid;
	int diamond;
};
typedef std::map<int, Rune_Stone_Pack_Grid_Cfg> Rune_Stone_Pack_Grid_Cfg_Map;

struct Package_Config {
	Package_Config(void) { reset(); }
	void reset(void) {
		super_item_id = 0;
		normal_trade_limit_conf_.reset();
		inner_trade_limit_conf_.reset();
		pack_line_cap_conf_.clear();
		pack_grid_lvl_conf_.clear();
		rune_stone_pack_grid_cfg_map_.clear();
	}

	int super_item_id;//包裹:可用来开启背包或仓库的格子
	Trade_Limit_Config normal_trade_limit_conf_;
	Trade_Limit_Config inner_trade_limit_conf_;
	Line_Capacity_Conf pack_line_cap_conf_;
	UInt_Int_Map pack_grid_lvl_conf_; //
	Rune_Stone_Pack_Grid_Cfg_Map rune_stone_pack_grid_cfg_map_;
};

// PACKAGE CONFIG ==================================



#endif /* PACK_CONFIG_H_ */
