/*
 * Friend_Config.h
 *
 *  Created on: 2014-06-09
 *      Author: xiaoliang
 */

#ifndef FRIEND_CONFIG_H_
#define FRIEND_CONFIG_H_

#include "Base_Struct.h"

struct Friend_Head_Photo_Config {
	Friend_Head_Photo_Config(void) { reset(); }
	void reset(void) {
		head_photo_id = 0;
		open_status = 0;
		hero_id = 0;
	}
	int32_t head_photo_id;
	int32_t open_status;
	int32_t hero_id;
};

struct Friend_Default_Head_Photo_Config {
	Friend_Default_Head_Photo_Config(void) { reset(); }
	void reset(void) {
		head_photo_id = 0;
		ob = 0;
	}
	int32_t head_photo_id;
	int32_t ob;
};

typedef boost::unordered_map<int32_t, Friend_Default_Head_Photo_Config> Friend_Default_Head_Photo_Config_Map;
typedef boost::unordered_map<int32_t, Friend_Head_Photo_Config> Friend_Head_Photo_Map;

#endif /* FRIEND_CONFIG_H_ */
