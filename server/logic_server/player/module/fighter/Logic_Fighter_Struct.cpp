/*
 * Logic_Fighter_Struct.cpp
 *
 *  Created on: Dec 13, 2014
 *      Author: linqiyou
 */


#include "Logic_Fighter_Struct.h"

int Exp_All::exp_sum(void) {
	all = exp + vip_exp + buff_exp;
	all_without_world_level = all;
	return all;
}
