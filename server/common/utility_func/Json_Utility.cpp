/*
 * Json_Utility.cpp
 *
 *  Created on: Oct 16, 2012
 *      Author: "ti"
 */

#include "Json_Utility.h"
#include "Record_Client.h"
#include "Base_Struct.h"
#include "pack/Pack_Struct.h"

struct Item_Detail;

int Json_Utility::assign_time_value(const Json::Value &array_json, Time_Value &time) {
	if (array_json.isInt()) {
		time.set(array_json.asInt()/1000, (array_json.asInt() % 1000) * 1000);
		return 0;
	}
	if (array_json.size() == 2) {
		time.set(array_json[0u].asInt(), array_json[1u].asInt());
	} else {
		LOG_ABORT("error config");
	}
	return 0;
}

int Json_Utility::assign_int_array(const Json::Value &array_json, int array[], int len) {
	if (array_json.isArray()) {
		int array_size = array_json.size();
		if (array_size != len) {
			LOG_USER_TRACE("array len error.");
			return -1;
		}
		for (int i = 0; i < array_size; ++i) {
			array[i] = array_json[i].asInt();
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}
	return 0;
}

int Json_Utility::assign_int_vec(const Json::Value &array_json, Int_Vec &vec) {
	vec.clear();
	if (array_json.isArray()) {
		int array_size = array_json.size();
		for (int i = 0; i < array_size; ++i) {
			vec.push_back(array_json[i].asInt());
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}

int Json_Utility::assign_str_vec(const Json::Value &array_json, String_Vec &vec) {
	vec.clear();
	if (array_json.isArray()) {
		int array_size = array_json.size();
		for (int i = 0; i < array_size; ++i) {
			vec.push_back(array_json[i].asString());
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}

int Json_Utility::assign_item_vec(const Json::Value &item_array_json, Item_Info_Vec &item_vec) {
	item_vec.clear();
	Item_Info info;
	if (item_array_json.isArray()) {
		int array_size = item_array_json.size();
		for (int i = 0; i < array_size; ++i) {
			info.reset();
			info.id = item_array_json[i][0u].asInt();
			info.amount = item_array_json[i][1u].asInt();
			info.bind = item_array_json[i][2u].asInt();
			item_vec.push_back(info);
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}

int Json_Utility::assign_item_vec(const Json::Value &item_array_json, Id_Amount_Vec &item_vec) {
	if (item_array_json.isArray()) {
		int array_size = item_array_json.size();
		for (int i = 0; i < array_size; ++i) {
			Id_Amount id_amount;
			id_amount.id = item_array_json[i][0u].asInt();
			id_amount.amount = item_array_json[i][1u].asInt();
			item_vec.push_back(id_amount);
		}
	} else {
		MSG_USER("configure file error");
		return -1;
	}
	return 0;
}

int Json_Utility::assign_vec_item_vec(const Json::Value &vec_item_array_json, std::vector<Item_Info_Vec> &vec_item_vec) {
	vec_item_vec.clear();
	Item_Info info;
	Item_Info_Vec item_info_vec;
	if (vec_item_array_json.isArray()) {
		int vec_item_array_json_sz = vec_item_array_json.size();
		for (int i = 0; i < vec_item_array_json_sz; ++i) {
			const Json::Value &item_array_json = vec_item_array_json[i];
			if (item_array_json.isArray()) {
				int array_size = item_array_json.size();
				item_info_vec.clear();
				for (int i = 0; i < array_size; ++i) {
					info.reset();
					info.id = item_array_json[i][0u].asInt();
					info.amount = item_array_json[i][1u].asInt();
					info.bind = item_array_json[i][2u].asInt();
					item_info_vec.push_back(info);
				}
				vec_item_vec.push_back(item_info_vec);
			} else {
				LOG_USER_TRACE("configure file error");
				return -1;
			}
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}
	return 0;
}

int Json_Utility::assign_double_vec(const Json::Value &array_json, Double_Vec &vec) {
	vec.clear();
	if (array_json.isArray()) {
		int array_size = array_json.size();
		for (int i = 0; i < array_size; ++i) {
			vec.push_back(array_json[i].asDouble());
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}

int Json_Utility::assign_set_list(const Json::Value &array_json, Int_Hash_Set &int_set) {
	int_set.clear();
	if (array_json.isArray()) {
		int array_size = array_json.size();
		for (int i = 0; i < array_size; ++i) {
			int_set.insert(array_json[i].asInt());
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}

int Json_Utility::assign_set_list(const Json::Value &array_json, Int_Set &int_set) {
	int_set.clear();
	if (array_json.isArray()) {
		int array_size = array_json.size();
		for (int i = 0; i < array_size; ++i) {
			int_set.insert(array_json[i].asInt());
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}


int Json_Utility::assign_int_int_map(const Json::Value &array_json, Int_Int_Map &int_int_map) {
	int_int_map.clear();
	if (array_json.isArray()) {
		int array_size = array_json.size();
		for (int i = 0; i < array_size; ++i) {
			if (array_json[i].isArray() && array_json[i].size() >= 2) {
				int_int_map.insert(std::make_pair(array_json[i][0u].asInt(),  array_json[i][1u].asInt()));
			} else {
				LOG_USER_TRACE("configure file error");
				return -1;
			}
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}

int Json_Utility::assign_int_int_set_map(const Json::Value &array_json, Int_Int_Set_Map &int_int_set_map) {
	int_int_set_map.clear();
	if (array_json.isArray()) {
		int array_size = array_json.size();
		for (int i = 0; i < array_size; ++i) {
			if (array_json[i].isArray() && array_json[i].size() >= 2) {
				Int_Set int_set;
				if (array_json[i][1].isArray()) {
					Json_Utility::assign_set_list(array_json[i][1u], int_set);
					int_int_set_map.insert(std::make_pair(array_json[i][0u].asInt(), int_set));
				}
			} else {
				LOG_USER_TRACE("configure file error");
				return -1;
			}
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}

/*
int Json_Utility::assign_int_item_info_vec_map(const Json::Value &array_json, Int_Item_Info_Vec_Map &int_item_info_vec_map) {
	int_item_info_vec_map.clear();
	if (array_json.isArray()) {
		int array_size = array_json.size();
		Item_Info_Vec item_info_vec;
		for (int i = 0; i < array_size; ++i) {
			if (array_json[i].isArray() && array_json[i].size() >= 2) {
				Int_Set int_set;
				if (array_json[i][1].isArray()) {
					item_info_vec.clear();
					Json_Utility::assign_item_vec(array_json[i][1u], item_info_vec);
					int_item_info_vec_map.insert(std::make_pair(array_json[i][0u].asInt(), item_info_vec));
				}
			} else {
				LOG_USER_TRACE("configure file error");
				return -1;
			}
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}
*/

int Json_Utility::assign_int_int_map_by_json(const Json::Value &json, Int_Int_Map &int_double_map) {
	int_double_map.clear();
	for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
		int key = atoi(it.key().asCString());
		double value = (*it).asInt();
		if (!int_double_map.count(key)) {
			int_double_map.insert(std::make_pair(key, value));
		} else {
			LOG_USER_TRACE("configure error");
		}
	}

	return 0;
}

int Json_Utility::assign_int_double_map(const Json::Value &array_json, Int_Double_Map &int_double_map) {
	int_double_map.clear();
//	for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
//		int key = it.key().asInt();
//		double value = (*it).asDouble();
//		if (!int_double_map.count(key)) {
//			int_double_map.insert(std::make_pair(key, value));
//		} else {
//			LOG_USER_TRACE("configure error");
//		}
//	}
		if (array_json.isArray()) {
			int array_size = array_json.size();
			for (int i = 0; i < array_size; ++i) {
				if (array_json[i].isArray() && array_json[i].size() >= 2) {
					int_double_map.insert(std::make_pair(array_json[i][0u].asInt(),  array_json[i][1u].asDouble()));
				} else {
					LOG_USER_TRACE("configure file error");
					return -1;
				}
			}
		} else {
			LOG_USER_TRACE("configure file error");
			return -1;
		}

	return 0;
}

int Json_Utility::assign_scene(const Json::Value &json, Scene_Key &scene) {
	scene.reset();
	if (json.isInt()) {
		scene.scene_id = json.asInt();
		return 0;
	}

	if (json.size() == 0 || json.size() > 3) {
		abort();
	}

	if (json.size() == 1) {
		scene.scene_id = json[0u].asInt();
		return 0;
	}

	if (json.size() == 2) {
		scene.scene_id = json[0u].asInt();
		scene.sort = json[1u].asInt();

		return 0;
	}

	if (json.size() == 3) {
		scene.scene_id = json[0u].asInt();
		scene.sort = json[1u].asInt();
		scene.sub_sort = json[2u].asInt();
		return 0;
	}

	return 0;
}

int Json_Utility::assign_coord(const Json::Value &json, Coord &coord) {
	if (json.size() != 3) {
		abort();
	}
	coord.set(json[1u].asInt(), json[2u].asInt(), json[0u].asInt());

	return 0;
}

int Json_Utility::assign_coord_list(const Json::Value &json, Coord_Vec &coord_vec) {
	coord_vec.clear();
	Coord coord;
	for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
		if ((*it).size() != 3) {
			continue;
		}
		coord.set((*it)[1u].asInt(), (*it)[2u].asInt(), (*it)[0u].asInt());
		coord_vec.push_back(coord);
	}

	return 0;
}

int Json_Utility::assign_int_by_string(const std::string &str, int &int_ret) {
	int_ret = 0;
	for (uint16_t i = 0; i < str.size(); ++i) {
		uint16_t dis_to_right = str.size() - i - 1;
		if (str[i] == '1') {
			int_ret |= (1 << dis_to_right);
		}
	}

	return 0;
}

int Json_Utility::assign_int_int(const Json::Value &json, Int_Int & int_int, int val2_default/* = 0*/){
	if (json.size() > 2) {
		abort();
	}
	if(json.size() == 0){
		int_int.val_1 = json.asInt();
		int_int.val_2 = val2_default;
		return 0;
	}
	if (json.size() == 1) {
		int_int.val_1 = json[0u].asInt();
		int_int.val_2 = val2_default;
		return 0;
	}

	if (json.size() == 2) {
		int_int.val_1 = json[0u].asInt();
		int_int.val_2 = json[1u].asInt();

		return 0;
	}
	return 0;
}

int Json_Utility::assign_int_int_vec(const Json::Value &json, std::vector<Int_Int> &vec, int val2_default/* = 0*/){
	vec.clear();
	if (json.isArray()) {
		int array_size = json.size();
		for (int i = 0; i < array_size; ++i) {
			Int_Int int_int;
			Json_Utility::assign_int_int(json[i], int_int, val2_default);
			vec.push_back(int_int);
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}
int Json_Utility::assign_id_int_int(const Json::Value &json, Id_Int_Int & id_int_int){
	if (json.size() != 3) {
		abort();
	}
	id_int_int.id = json[0u].asInt();
	id_int_int.val1 = json[1u].asInt();
	id_int_int.val2 = json[2u].asInt();
	return 0;
}

int Json_Utility::assign_id_int_int_vec(const Json::Value &json, std::vector<Id_Int_Int> &vec){
	vec.clear();
	if (json.isArray()) {
		int array_size = json.size();
		for (int i = 0; i < array_size; ++i) {
			Id_Int_Int id_int_int;
			Json_Utility::assign_id_int_int(json[i], id_int_int);
			vec.push_back(id_int_int);
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}

int Json_Utility::assign_int_vec_vec(const Json::Value &json, Int_Vec_Vec &vec) {
	vec.clear();
	if (json.isArray()) {
		int array_size = json.size();
		for (int i = 0; i < array_size; ++i) {
			Int_Vec int_vec;
			Json_Utility::assign_int_vec(json[i], int_vec);
			vec.push_back(int_vec);
		}
	} else {
		LOG_USER_TRACE("configure file error");
		return -1;
	}

	return 0;
}
