/*
 * Mail_Config.cpp
 *
 *  Created on: 2014-05-05
 *      Author: xiaoliang
 */

#include "Config_Cache_Mail.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "mail/Mail_Config.h"

const Mail_Config_Map &Config_Cache_Mail::mail_config(void) const {
	return mail_config_;
}

const Mail_Postage_Config_Map &Config_Cache_Mail::mail_postage_config(void) const {
	return mail_postage_config_;
}

const Mail_Base_Config *Config_Cache_Mail::find_config(uint32_t id) const {
	return get_map_second_pointer_by_key(id, mail_config_);
}

const Mail_Base_Config *Config_Cache_Mail::find_merge_config(uint32_t id) const {
	return get_map_second_pointer_by_key(id, merge_mail_config_);
}

const uint32_t Config_Cache_Mail::find_postage_config(uint32_t number) const {
	const Mail_Postage_Config* postage = get_map_second_pointer_by_key(number, mail_postage_config_);
	if(postage){
		return postage->postage;
	}
	return 0;
}

const uint32_t Config_Cache_Mail::find_common_config(const std::string &key) const {
	Mail_Common_Config_Map::const_iterator it = common_config_.find(key);
	if(it != common_config_.end()) {
		return it->second;
	}
	return 0;
}

void Config_Cache_Mail::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Mail");

	refresh_setting_config_cache();
	refresh_postage_config_cache();
	refresh_common_config_cache();

}

void Config_Cache_Mail::refresh_common_config_cache(void) {
	common_config_.clear();
	const Json::Value& mail_setting_json = CONFIG_INSTANCE->config_json()["mail"]["conf"];
	if (mail_setting_json == Json::Value::null || mail_setting_json.size() == 0) {
		LOG_ABORT("");
	}
	for (uint i = 0; i < mail_setting_json.size(); ++i) {
		const Json::Value& mail_json = mail_setting_json[i];
		set_map_second_value_by_key(mail_json["key"].asString(), common_config_, mail_json["value"].asInt());
	}
}

void Config_Cache_Mail::refresh_setting_config_cache(void) {
	const Json::Value& mail_setting_json = CONFIG_INSTANCE->config_json()["mail"]["mail_setting"];
	if (mail_setting_json == Json::Value::null || mail_setting_json.size() == 0) {
		LOG_ABORT("");
	}

	Mail_Base_Config base_set;
	for (uint i = 0; i < mail_setting_json.size(); ++i) {
		base_set.reset();
		const Json::Value& mail_json = mail_setting_json[i];
		base_set.type_id = mail_json["id"].asInt();
		base_set.sender_name = mail_json["sender_name"].asString();
		base_set.mail_title = mail_json["mail_title"].asString();
		base_set.mail_content = mail_json["mail_content"].asString();

		if(mail_json["item"] != Json::Value::null && mail_json["item"].size() > 0) {
			for (uint j = 0; j < mail_setting_json[i]["item"].size(); ++j) {
				const Json::Value& item_json = mail_json["item"][j];
				Item_Basic_Info item;
				item.reset();
				item.id = item_json[0u].asInt();
				item.amount = item_json[1u].asInt();
				item.bind = item_json[2u].asInt();
				base_set.item_vector.push_back(item);
			//	base_set.item_detal_vec.push_back(Item_Detail(item.id, item.amount, item.bind));
			}
		}

		set_map_second_value_by_key(base_set.type_id, mail_config_, base_set);
	}
}

void Config_Cache_Mail::refresh_merge_config_cache(void) {
	const Json::Value& mail_setting_json = CONFIG_INSTANCE->config_json()["merge_config"]["rename_mail"];
	if (mail_setting_json == Json::Value::null || mail_setting_json.size() == 0) {
		std::cout << "config/server_list.json rename_mail error!" << std::endl;
		exit(0);
	}

	Mail_Base_Config base_set;
	for (uint i = 0; i < mail_setting_json.size(); ++i) {
		base_set.reset();
		const Json::Value& mail_json = mail_setting_json[i];
		base_set.type_id = mail_json["id"].asInt();
		base_set.sender_name = mail_json["sender_name"].asString();
		base_set.mail_title = mail_json["mail_title"].asString();
		base_set.mail_content = mail_json["mail_content"].asString();

		if(mail_json["item"] != Json::Value::null && mail_json["item"].size() > 0) {
			for (uint j = 0; j < mail_setting_json[i]["item"].size(); ++j) {
				const Json::Value& item_json = mail_json["item"][j];
				Item_Basic_Info item;
				item.reset();
				item.id = item_json[0u].asInt();
				item.amount = item_json[1u].asInt();
				item.bind = item_json[2u].asInt();
				base_set.item_vector.push_back(item);
			//	base_set.item_detal_vec.push_back(Item_Detail(item.id, item.amount, item.bind));
			}
		}

		set_map_second_value_by_key(base_set.type_id, merge_mail_config_, base_set);
	}
}

void Config_Cache_Mail::refresh_postage_config_cache(void) {
	const Json::Value& mail_setting_json = CONFIG_INSTANCE->config_json()["mail"]["postage"];
	if (mail_setting_json == Json::Value::null || mail_setting_json.size() == 0) {
		LOG_ABORT("");
	}

	Mail_Postage_Config base_set;
	for (uint i = 0; i < mail_setting_json.size(); ++i) {
		base_set.reset();
		const Json::Value& mail_json = mail_setting_json[i];
		base_set.item_number = mail_json["number"].asInt();
		base_set.postage = mail_json["postage"].asInt();
		set_map_second_value_by_key(base_set.item_number, mail_postage_config_, base_set);
	}
}
