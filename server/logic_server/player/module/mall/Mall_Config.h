/*
 * Mall_Config.h
 *
 *  Created on: 2014-05-05
 *      Author: xiaoliang
 */

#ifndef MALL_CONFIG_H_
#define MALL_CONFIG_H_

#include "Base_Struct.h"

struct Mall_Activity_Price {
	Mall_Activity_Price(void) { reset(); }
	void reset(void) {
		act_id = 0;
		tabs = 0;
		item_id = 0;
		gender = 0;
		price = 0;
	}
	uint32_t act_id;
	uint32_t tabs;
	uint32_t item_id;
	uint8_t gender;
	uint32_t price;
};

typedef std::vector<Mall_Activity_Price> Mall_Activity_Price_Vec;
typedef boost::unordered_map<long long int, Mall_Activity_Price_Vec> Mall_Activity_Price_Vec_Map;

#endif /* MALL_CONFIG_H_ */
