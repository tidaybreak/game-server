/*
 * Secret_Shop_Struct.h
 *
 *  Created on: 2014年7月31日
 *      Author: root
 */

#ifndef SECRET_SHOP_STRUCT_H_
#define SECRET_SHOP_STRUCT_H_

#include "Game_Header.h"
#include "Base_Struct.h"
#include "Game_Typedef.h"


struct Secret_Item_Config;

struct SC_Item_Info{
	const Secret_Item_Config* config;
	int buy_num;
	boost::unordered_map<role_id_t, int> player_buy_num_map;
	SC_Item_Info(void);
	void reset(void);
	int player_buy_num(role_id_t role_id);
	void buy_item(role_id_t role_id);
};

typedef std::vector<SC_Item_Info> SC_Item_Info_Vec;
typedef boost::unordered_map<int64_t, SC_Item_Info_Vec> SC_Item_Info_Vec_Map;


struct Vagrant_Item_Info{
	Vagrant_Item_Info(void){
		reset();
	}
	~Vagrant_Item_Info(void){

	}
	void reset(void){
		cfg_key = 0;
		cfg_item_id = 0;
		cfg_limit_num = 0;
		cfg_num = 0;
		cfg_cost_type = 0;
		cfg_cost = 0;
		buy_num = 0;
		player_buy_num_map.clear();
	}
	int player_buy_num(role_id_t role_id){
		boost::unordered_map<role_id_t, int>::iterator it = player_buy_num_map.find(role_id);
		if( it != player_buy_num_map.end()){
			return it->second;
		}
		return 0;
	}
	void buy_item(role_id_t role_id){
		buy_num += 1;
		if(cfg_limit_num != 0){
			int player_buy_num = player_buy_num_map[role_id] +1;
			player_buy_num_map[role_id] = player_buy_num;
		}
	}
	int cfg_key;
	int cfg_item_id;
	int cfg_limit_num;
	int cfg_num;
	int cfg_cost_type;
	int cfg_cost;
	int buy_num;
	boost::unordered_map<role_id_t, int> player_buy_num_map;
};

typedef std::vector<Vagrant_Item_Info> Vagrant_Item_Info_Vec;
typedef boost::unordered_map<int64_t, Vagrant_Item_Info_Vec> Vagrant_Store_Map;
typedef boost::unordered_map<int, Vagrant_Store_Map> Vagrant_Store_Scene_Map;

#endif /* SECRET_SHOP_STRUCT_H_ */
