/*
 * Covenant_Config.h
 *
 *  Created on: 2014-07-15
 *      Author: xiaoliang
 *
 *      盟约配制文件结构
 */

#ifndef COVENANT_CONFIG_H_
#define COVENANT_CONFIG_H_

#include "status/Status_Struct.h"

typedef std::vector<Effect_Argv_Prop> Effect_Argv_Prop_Vec;
typedef boost::unordered_map<int32_t, Effect_Argv_Prop_Vec> Effect_Argv_Prop_Map;

struct Covenant_Conf {
	Covenant_Conf(){ reset(); }
	void reset(void) {
		id = 0;
		days = 0;
		price = 0;
		become_time = 0;
		name.clear();
		mail_id = 0;
		prop_map.clear();
	}
	uint32_t id;//盟约ID
	std::string name;//精灵名称
	uint32_t days;//开通/续费天数
	uint32_t price;//开通或续费价格
	uint32_t become_time;//特殊精灵,在当天有持续时间, 表示当天开启后持续时间过期时间
	uint32_t mail_id; //过期提示邮件id

	Effect_Argv_Prop_Map prop_map; //状态加成特殊
};

typedef boost::unordered_map<uint32_t, Covenant_Conf> Covenant_Conf_Map;

#endif /* COVENANT_CONFIG_H_ */
