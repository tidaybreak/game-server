/*
 * Item_User_Func.h
 *
 *  Created on: Jan 10, 2013
 *      Author: "linqiyou"
 */

#ifndef ITEM_USER_FUNC_H_
#define ITEM_USER_FUNC_H_

#include "pack/Pack_Struct.h"
#include "Item_User_Config.h"
#include "Item_User_Def.h"

namespace Json {
class Value;
}

Money_Type get_money_type(uint32_t id);
int get_gift_item_list(const Json::Value &item_list_json, int amount, Item_Vec &item_list);
int get_gift_item_list(const Json::Value &item_list_json, Item_Vec &item_list);
int get_item_detail_list(const Item_Info_Vec &item_list_config, Item_Vec &item_list);

int get_lottery_item(uint32_t id, int level, int career, int amount, Item_Vec &item, Item_Vec &broadcast_item, Money_Add_List &money_add_list,int &used_times,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost,std::vector<Int_Int> &special_receive);
int get_lottery_item(uint32_t id, int level, int career, Item_Vec &item, Item_Vec &broadcast_item, Money_Add_List &money_add_list,int &used_times,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost,std::vector<Int_Int> &special_receive);
int get_lottery_item(uint32_t id, int level, int career, const Item_Lottery_Item_Info_Vec_Config &item_list_config,
		Item_Vec &item_list, Item_Vec &broadcast_item, Money_Add_List &money_add_list,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost,std::vector<Int_Int> &special_receive);
int get_lottery_item(uint32_t id, int level, int career, const Item_Lottery_Item_Info_Config &item_list_config,
		Item_Vec &item_list, Item_Vec &broadcast_item, Money_Add_List &money_add_list,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost);
int get_drop_item(uint32_t id, int level, int career, const Item_Lottery_Item_Info_Config &item_list_config,
		Item_Vec &item_list, Item_Vec &broadcast_item, Money_Add_List &money_add_list,
		Id_Amount_Vec &item_cost, Money_Sub_List &money_cost);

Item_Effect_Type get_item_type(uint32_t item_id);
Item_Effect_Type get_item_type_third(uint32_t item_id);
int get_resource_item(Item_Vec &item_list,Money_Add_List &money_add_list,std::vector<Int_Int> &spe_item,const uint32_t id,const int num);


#endif /* ITEM_USER_FUNC_H_ */
