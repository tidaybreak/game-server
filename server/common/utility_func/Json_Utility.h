/*
 * Json_Utility.h
 *
 *  Created on: Oct 16, 2012
 *      Author: "ti"
 *      Json的工具函数
 */

#ifndef JSON_UTILITY_H_
#define JSON_UTILITY_H_

#include "Server_Struct.h"

struct Int_Int;
struct Id_Int_Int;
struct Id_Amount;

typedef std::vector<Id_Amount> Id_Amount_Vec;

namespace Json {
class Value;
}


namespace Json_Utility {
	int assign_time_value(const Json::Value &array_json, Time_Value &time);
	int assign_int_array(const Json::Value &array_json, int array[], int len);
	int assign_int_vec(const Json::Value &array_json, Int_Vec &vec);
	int assign_str_vec(const Json::Value &array_json, String_Vec &vec);
	int assign_double_vec(const Json::Value &array_json, Double_Vec &vec);
	int assign_item_vec(const Json::Value &item_array_json, Item_Info_Vec &item_vec);
	int assign_item_vec(const Json::Value &item_array_json, Id_Amount_Vec &item_vec);
	int assign_vec_item_vec(const Json::Value &vec_item_array_json, std::vector<Item_Info_Vec> &vec_item_vec);
	int assign_set_list(const Json::Value &array_json, Int_Hash_Set &int_set);
	int assign_set_list(const Json::Value &array_json, Int_Set &int_set);
	int assign_int_int_map(const Json::Value &array_json, Int_Int_Map &int_int_map);
	int assign_int_int_set_map(const Json::Value &array_json, Int_Int_Set_Map &int_int_set_map);
	int assign_int_int_map_by_json(const Json::Value &json, Int_Int_Map &int_double_map);
	int assign_int_double_map(const Json::Value &json, Int_Double_Map &int_double_map);
	int assign_int_by_string(const std::string &str, int &int_ret);
	int assign_scene(const Json::Value &json, Scene_Key &scene);
	int assign_coord(const Json::Value &json, Coord &coord);
	int assign_coord_list(const Json::Value &json, Coord_Vec &coord);
	int assign_int_item_info_vec_map(const Json::Value &array_json, Int_Item_Info_Vec_Map &int_item_info_vec_map);
	int assign_int_int(const Json::Value &json, Int_Int & int_int, int val2_default = 0);
	int assign_int_int_vec(const Json::Value &json, std::vector<Int_Int> &vec,int val2_default = 0);
	int assign_id_int_int(const Json::Value &json, Id_Int_Int & id_int_int);
	int assign_id_int_int_vec(const Json::Value &json, std::vector<Id_Int_Int> &vec);
	int assign_int_vec_vec(const Json::Value &json, Int_Vec_Vec &vec);
}

#endif /* JSON_UTILITY_H_ */
