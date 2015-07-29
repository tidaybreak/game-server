/*
 * Secret_Shop_Struct.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: root
 */

#include "Secret_Shop_Struct.h"
#include "Secret_Shop_Config.h"

SC_Item_Info::SC_Item_Info(void){
	reset();
}

void SC_Item_Info::reset(void){
	config = 0;
	buy_num = 0;
	player_buy_num_map.clear();
}

int SC_Item_Info::player_buy_num(role_id_t role_id){
	boost::unordered_map<role_id_t, int>::iterator it = player_buy_num_map.find(role_id);
	if( it != player_buy_num_map.end()){
		return it->second;
	}
	return 0;
}

void SC_Item_Info::buy_item(role_id_t role_id){
	buy_num += 1;
	if(config->limit_num != 0){
		int player_buy_num = player_buy_num_map[role_id] +1;
		player_buy_num_map[role_id] = player_buy_num;
	}
}



