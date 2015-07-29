/*
 * Json_Value.h
 *
 *  Created on: Dec 11, 2013
 *      Author: ti
 */

#ifndef JSON_VALUE_H_
#define JSON_VALUE_H_

#include "Game_Typedef.h"

struct Config_Entry {
	Config_Entry(void) { }

	inline void cover_json_to_map(JsonKey_Func func) {
		get_int_map_json().clear();
		int key = 0;
		for (Json::Value::iterator it = get_json().begin(); it != get_json().end(); ++it) {
			func(it.key(), key);
			get_int_map_json().insert(std::make_pair(key, *it));
		}
	}

	inline const Json::Value &get_json(void) const { return json; }

	inline Int_Json_Map &get_int_map_json(void) { return int_json_map; }

	inline String_Json_Map &get_str_map_json(void) { return str_json_map; }

	inline void clear_json(void) { json.clear(); }

	Json::Value json;
	Int_Json_Map int_json_map;
	String_Json_Map str_json_map;
};

inline const Json::Value &get_json_value_in_map(const int id, Config_Entry &config) {
	Int_Json_Map &map = config.get_int_map_json();
	Int_Json_Map::const_iterator it = map.find(id);
	if (it != map.end()) {
		return it->second;
	} else {
		return Json::Value::null;
	}
}

inline const Json::Value &get_json_value_with_rlock_in_map(Config_Lock &lock, const int id, Config_Entry &config) {
	return get_json_value_in_map(id, config);
}

inline const Json::Value &get_json_value_with_rlock(Config_Lock &lock, Config_Entry &config) {
	return config.get_json();
}

inline void get_json_dir_type(const Json::Value &json, int &type) {
	type = json["type"].asInt();
}

inline void get_json_key_id(const Json::Value &json, int &type) {
	type = atoi(json.asCString());
}

inline void get_json_dir_solution_id(const Json::Value &json, int &type) {
	type = json["solution_id"].asInt();
}

#endif /* JSON_VALUE_H_ */
