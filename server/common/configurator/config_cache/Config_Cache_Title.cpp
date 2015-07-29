/*
 * Config_Cache_Title.cpp
 *
 *  Created on: 2014年7月24日
 *      Author: Linqiyou
 */

#include "Config_Cache_Title.h"
#include "Configurator.h"
#include "Json_Utility.h"

const Title_Cfg &Config_Cache_Title::title_cfg(void) {
	return this->title_cfg_;
}

const Int_Hash_Set &Config_Cache_Title::honour_task(void) {
	return this->honour_task_;
}

const Title_Cfg_Detail * const Config_Cache_Title::find_title(const int32_t id) const {
	Title_Cfg::const_iterator it = title_cfg_.find(id);
	if (it != title_cfg_.end()) {
		return &(it->second);
	}
	return NULL;
}
const Int_Hash_Set &Config_Cache_Title::handbook(void) const {
	return handbook_;
}

void Config_Cache_Title::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Title");

	title_cfg_.clear();
	Title_Cfg_Detail info;
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["title"]["title"];
	for (Json::Value::const_iterator be_it = config_json.begin();
			be_it != config_json.end(); ++be_it) {
		info.reset();
		info.id = (*be_it)["id"].asInt();
		info.quality = (*be_it)["quality"].asInt();
		info.validity = (*be_it)["validity"].asInt();
		if (!(*be_it)["prop"].isNull() && (*be_it)["prop"].isArray()) {
			Json_Utility::assign_int_double_map((*be_it)["prop"], info.prop);
		}
		if ((*be_it)["condition"] != Json::Value::null) {
			info.condition.from_table = (*be_it)["condition"][0u]["from_table"].asString();
			if ((*be_it)["condition"][0u]["cond"].isArray() && !(*be_it)["condition"][0u]["cond"].isNull()) {
				Json_Utility::assign_set_list((*be_it)["condition"][0u]["cond"], info.condition.cond_list);
			} else {
				info.condition.cond = (*be_it)["condition"][0u]["cond"].asInt();
			}
		}
		if ((*be_it).isMember("name")) {
			info.name = (*be_it)["name"].asCString();
		}
		if ((*be_it).isMember("dec")) {
			info.des = (*be_it)["dec"].asCString();
		}
		info.cond_type = (*be_it)["cond_type"].asInt();
		info.cond = (*be_it)["cond"].asInt();
		info.cond_ext = (*be_it)["cond_ext"].asInt();
		info.ann_sign_in = (*be_it)["ann_sign_in"].asInt();
		if ((*be_it).isMember("announ")) {
			info.announ = false;
		}

		set_map_second_value_by_key(info.id, title_cfg_, info);
	}

	{
		honour_task_.clear();
		const Json::Value& config_json = CONFIG_INSTANCE->config_json()["title"]["setting"];
		for (Json::Value::const_iterator be_it = config_json.begin();
				be_it != config_json.end(); ++be_it) {
			if ((*be_it)["task_ids"].isArray() && !(*be_it)["task_ids"].isNull()) {
				for (uint16_t i = 0; i < (*be_it)["task_ids"].size(); ++i) {
					int task_id = (*be_it)["task_ids"][i].asInt();
					honour_task_.insert(task_id);
				}
			}
		}
	}

	{
		handbook_.clear();
		const Json::Value& config_json = CONFIG_INSTANCE->config_json()["equip"]["handbook20"];
		for (Json::Value::const_iterator be_it = config_json.begin();
				be_it != config_json.end(); ++be_it) {
			if ((*be_it)["item_id"].isArray() && !(*be_it)["item_id"].isNull()) {
				if ((*be_it)["item_id"].size() != 3) {
					MSG_TRACE_ABORT("ERROR: config handbook20 not enought 3!");
					continue;
				}
				for (uint16_t i = 0; i < (*be_it)["item_id"].size(); ++i) {
					int task_id = (*be_it)["item_id"][i].asInt();
//					boost::unordered_map<int8_t, Int_Hash_Set>::const_iterator it_hb = handbook_.find(i);
//					if (it_hb != handbook_.end()) {
//						handbook_[i].insert(task_id);
//					} else {
//						Int_Hash_Set hb_info;
//						hb_info.insert(task_id);
//						handbook_.insert(std::make_pair(i, hb_info));
//					}
					handbook_.insert(task_id);
				}
			}
			if ((*be_it)["other_id"].isArray() && !(*be_it)["other_id"].isNull()) {
				for (uint16_t i = 0; i < (*be_it)["other_id"].size(); ++i) {
					int task_id = (*be_it)["other_id"][i].asInt();
					handbook_.insert(task_id);
				}
			}
		}
	}
}


