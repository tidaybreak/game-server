/*
 * Active_Config.cpp
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 */

#include "Config_Cache_Active.h"
#include "Configurator.h"
#include "Json_Utility.h"

const Active_Conf_Map &Config_Cache_Active::act_conf_map(void) const {
	return act_conf_map_;
}
void Config_Cache_Active::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Active");
	act_conf_map_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["helper"]["event"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active config: event.json");
	}
	Active_Config act_conf;
	for (uint i = 0; i < config_json.size(); ++i) {
		act_conf.reset();
		const Json::Value& json = config_json[i];
		act_conf.id = json["id"].asInt();
		act_conf.type = json["type"].asInt();
		act_conf.icon = json["icon"].asInt();
		act_conf.openLevel = json["openLevel"].asInt();
		//启始类型	启始值
		act_conf.blank_type = json["start_type"].asInt();
		if(json["start_date"].isInt()) {
			act_conf.blank_days = json["start_date"].asInt();
		}
		if(json["start_date"].isString()) {
			act_conf.assign_blank_time = json["start_date"].asString();
		}
		//开启类型	开启期限
		act_conf.open_type = json["open_type"].asInt();
		if(json["open_date"].isArray()) {
			for (uint t = 0; t < json["open_date"].size(); ++t) {
				act_conf.interval_open_date.push_back(json["open_date"][t].asInt()); //开启期限(数组)
			}
		}
		if(json["open_date"].isInt()) {
			act_conf.loop_open_date = json["open_date"].asInt();//开启期限(数字,循环)
		}
		if(json["open_date"].isString()) {
			act_conf.assign_open_date = json["open_date"].asString();//开启期限(指定日期,字符)
		}
		//相对时间
		for (uint t = 0; t < json["time"].size(); ++t) {
			act_conf.time.push_back(json["time"][t].asInt());
		}
		//动作提前(分)
		act_conf.before_time = json["before_time"].asInt();

		act_conf.loop = json["loop"].asInt();
		act_conf.over_time = json["over_time"].asInt();
		act_conf.continued = json["continued"].asInt();
		act_conf.after_id = json["after_id"].asInt();

		//读取相应动作信息
		Event evn;
		for (uint j = 0; j < json["action"].size(); ++j) {
			const Json::Value& action_json = json["action"][j];
			evn.reset();
			evn.action = action_json["action"].asInt();
			evn.relatively = action_json["relatively"].asInt();
			evn.rel_back = action_json["rel_back"].asInt();
			for (uint t = 0; t < action_json["argv"].size(); ++t) {
				evn.argv.push_back(action_json["argv"][t].asInt());
			}
			act_conf.event_action.push_back(evn);
		}
		set_map_second_value_by_key(act_conf.id, act_conf_map_, act_conf);
	}
}

