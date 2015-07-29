/*
 * Utility_Func.h
 *
 *  Created on: Oct 16, 2012
 *      Author: "Ti"
 */

#ifndef UTILITY_FUNC_H_
#define UTILITY_FUNC_H_

#include "Game_Typedef.h"

namespace Utility_Func {
	inline double random_double(double start, double end) {
		return start + (end - start) * rand() / (RAND_MAX + 1.0);
	}

	/// 返回大于1.5 * num的第一个素数
	size_t get_hash_table_size(unsigned int num);

	void get_no_repeat_uint(const uint32_t max_size, const uint32_t generate_nums, UInt_Vec &vec);
	int lottery_one(const Int_Vec &weight);
	int lottery_one_element(const Int_Vec &int_vec);
	double lottery_one_element(const Double_Vec &double_vec);
	inline bool lottery_success(double percent) {
		if (percent == 1.0) {
			return true;
		}
		if (percent < 0.0 || percent > 1.0) {	// config error ret false
			return false;
		}
		if (random() % 10000 < (percent * 10000)) {
			return  true;
		}
		return false;
	}

	inline bool is_phy_career(const int career) {
		return career == 0 || career == 1 || career == 2;
	}

	uint32_t get_section(const Int_Set &section, int value);
	uint32_t get_section_not_equal(const Int_Set &section, int value);

	void check_is_not_under_zero(double value);

	inline int get_type_by_id(int id) {
		return (id / 100 * 100);
	}

	inline int get_fb_id_by_scene_id(int scene_id) {
		return get_type_by_id(scene_id);
	}

	template<typename Block_Buff_Type, typename Msg_Type>
	inline void get_block_buff_by_msg(int msg_id, const Msg_Type &msg, Block_Buff_Type &buff) {
		buff.make_message(msg_id);
		msg.serialize(buff);
		buff.finish_message();
	}

	template<typename FB_Map_Type, typename Map_Type>
	inline void insert_fb_times_to_map(Map_Type &map_to, const FB_Map_Type &map_from) {
		for (typename FB_Map_Type::const_iterator it = map_from.begin(); it != map_from.end(); ++it) {
			if (map_to.count(it->first)) {
				MSG_USER_TRACE("map_to has %d", it->first);
			}
			map_to.insert(std::make_pair(it->first, it->second.times));
		}
	}
	template<typename Key_Type, typename Map_Type>
	inline typename Map_Type::mapped_type get_map_second_by_key(Key_Type key, const Map_Type &map) {
		typename Map_Type::const_iterator it = map.find(key);
		if (it != map.end()) {
			return it->second;
		} else {
			return NULL;
		}
	}

	template<typename Key_Type, typename Map_Type>
	inline typename Map_Type::mapped_type *get_map_second_pointer_by_key(Key_Type key, Map_Type &map) {
		typename Map_Type::iterator it = map.find(key);
		if (it != map.end()) {
			return &it->second;
		} else {
			return NULL;
		}
	}

	inline int dequals(double a, double b)
	{
		return fabs(a-b) < 0.000001;
	}

	int validate_md5(const char *key, const char *account, const char *time, const char *flag);

	int get_rand_index(const Int_Vec &wight);
	int get_rand_index(const Json::Value &wight);
	int get_rand_min_max(const int min, const int max);

	inline bool is_zero(double value) {
		return fabs(value - 0.0) < 0.000001;
	}

	// 除法
	inline int div(int dividend, int divisor) {
		return !divisor ?  0 : dividend / divisor;
	}

	inline double div(double dividend, double divisor) {
		return is_zero(divisor) ?  0.0 : dividend / divisor;
	}
}

#define U_FUNC	Utility_Func

#endif /* UTILITY_FUNC_H_ */
