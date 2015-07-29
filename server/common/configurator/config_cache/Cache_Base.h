/*
 * Cache_Base.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CACHE_BASE_H_
#define CACHE_BASE_H_

#include "Game_Typedef.h"
#include "Record_Client.h"

class Cache_Base {

public:
	template<typename Key_Type, typename Map_Type, typename Value>
	inline const int set_map_second_value_by_key(Key_Type key, Map_Type &map, const Value &value) const;

	// 使用get_map_second_pointer_by_key的map refresh时要用set_map_second_value_by_key,否则热更可能异常
	template<typename Key_Type, typename Map_Type>
	inline const typename Map_Type::mapped_type *get_map_second_pointer_by_key(Key_Type key, const Map_Type &map) const;

	template<typename Key_Type, typename Map_Type>
	inline const typename Map_Type::mapped_type get_value_by_key(Key_Type key, const Map_Type &map) const;
};


template<typename Key_Type, typename Map_Type, typename Value>
inline const int Cache_Base::set_map_second_value_by_key(Key_Type key, Map_Type &map, const Value &value) const {
	typename Map_Type::iterator it = map.find(key);
	if (it != map.end()) {
		it->second = value;
	} else {
		map.insert(std::make_pair(key, value));
	}

	return 0;
}

template<typename Key_Type, typename Map_Type>
inline const typename Map_Type::mapped_type *Cache_Base::get_map_second_pointer_by_key(Key_Type key, const Map_Type &map) const {
	typename Map_Type::const_iterator it = map.find(key);
	if (it != map.end()) {
		return &it->second;
	} else {
		//LOG_USER_TRACE("key:%d", key);
		return NULL;
	}
}

template<typename Key_Type, typename Map_Type>
inline const typename Map_Type::mapped_type Cache_Base::get_value_by_key(Key_Type key, const Map_Type &map) const {
	typename Map_Type::const_iterator it = map.find(key);
	if (it != map.end()) {
		return it->second;
	} else {
		//LOG_USER_TRACE("key:%d", key);
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////

#endif /* CACHE_BASE_H_ */
