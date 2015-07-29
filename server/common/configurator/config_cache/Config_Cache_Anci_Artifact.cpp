/*
 * Config_Cache_Anci_Artifact.cpp
 *
 *  Created on: 2014年10月10日
 *      Author: Linqiyou
 */

#include "Config_Cache_Anci_Artifact.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_Item.h"

const AA_Unban_Cond_Cfg &Config_Cache_Anci_Artifact::aa_unban_cond_cfg(void) const {
	return aa_unban_cond_cfg_;
}
const AA_Unban_Cond *Config_Cache_Anci_Artifact::find_aa_unban_cond(const int id) const {
	return get_map_second_pointer_by_key(id, aa_unban_cond_cfg_);
}
const AA_Gem *Config_Cache_Anci_Artifact::find_aa_gem(const int id) const {
	return get_map_second_pointer_by_key(id, aa_gem_cfg_);
}
const AA_Cond *Config_Cache_Anci_Artifact::find_aa_cond(const int id) const {
	return get_map_second_pointer_by_key(id, aa_cond_cfg_);
}
const AA_Cond_Cfg &Config_Cache_Anci_Artifact::aa_cond_cfg(void) const {
	return aa_cond_cfg_;
}

void Config_Cache_Anci_Artifact::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Anci_Artifact");

	{
		aa_cond_cfg_.clear();
		AA_Cond info;
		const Json::Value& config_json = CONFIG_INSTANCE->config_json()["anci_artifact"]["cond"];
		for (Json::Value::const_iterator be_it = config_json.begin();
				be_it != config_json.end(); ++be_it) {
			info.reset();
			info.id = (*be_it)["id"].asInt();
			info.name = (*be_it)["name"].asString();
			info.type = (*be_it)["type"].asInt();
			info.num = (*be_it)["num"].asInt();
			info.cond_ext = (*be_it)["cond_ext"].asInt();
			info.perfectPoint = (*be_it)["perfectPoint"].asInt();

			set_map_second_value_by_key(info.id, aa_cond_cfg_, info);
		}
	}

	{
		aa_gem_cfg_.clear();
		AA_Gem info;
		const Json::Value& config_json = CONFIG_INSTANCE->config_json()["anci_artifact"]["gem"];
		for (Json::Value::const_iterator be_it = config_json.begin();
				be_it != config_json.end(); ++be_it) {
			info.reset();
			info.id = (*be_it)["id"].asInt();
			info.name = (*be_it)["name"].asString();
			if (!(*be_it)["cond"].isNull() && (*be_it)["cond"].isArray()) {
				Json_Utility::assign_set_list((*be_it)["cond"], info.cond);
			}
			for (Int_Hash_Set::const_iterator it_cond = info.cond.begin(); it_cond != info.cond.end(); ++it_cond) {
				if (!find_aa_cond(*it_cond)) {
					LOG_ABORT("anci_artifact(%d): cond(%d) not exist", info.id, *it_cond);
				}
			}
			info.perfectPoint = (*be_it)["perfectPoint"].asInt();


			set_map_second_value_by_key(info.id, aa_gem_cfg_, info);
		}
	}

	{
		aa_unban_cond_cfg_.clear();
		AA_Unban_Cond info;
		const Json::Value& config_json = CONFIG_INSTANCE->config_json()["anci_artifact"]["unban_cond"];
		for (Json::Value::const_iterator be_it = config_json.begin();
				be_it != config_json.end(); ++be_it) {
			info.reset();
			info.id = (*be_it)["id"].asInt();
			info.name = (*be_it)["name"].asString();
			info.type = (*be_it)["type"].asInt();
			info.num = (*be_it)["num"].asInt();

			if (!(*be_it)["property"].isNull() && (*be_it)["property"].isArray()) {
				int array_size = (*be_it)["property"].size();
				for (int i = 0; i < array_size; ++i) {
					int id = 0;
					double value = 0.0;
					id = (*be_it)["property"][i][0u].asInt();
					value = (*be_it)["property"][i][1u].asDouble();
					info.property.insert(std::make_pair(id, value));
				}
			}
			if (!(*be_it)["gem"].isNull() && (*be_it)["gem"].isArray()) {
				Json_Utility::assign_set_list((*be_it)["gem"], info.gem);
			}
			for (Int_Hash_Set::const_iterator it_gem = info.gem.begin(); it_gem != info.gem.end(); ++it_gem) {
				AA_Gem_Cfg::iterator it_aa_cond = aa_gem_cfg_.find(*it_gem);
				if (it_aa_cond != aa_gem_cfg_.end()) {
					it_aa_cond->second.unban_cond_id = info.id;
				} else {
					LOG_ABORT("anci_artifact(%d): cond(%d) not exist", info.id, *it_gem);
				}
			}
			if (!(*be_it)["extraProperty"].isNull() && (*be_it)["extraProperty"].isArray()) {
				int array_size = (*be_it)["extraProperty"].size();
				for (int i = 0; i < array_size; ++i) {
					int id = 0;
					double value = 0.0;
					id = (*be_it)["extraProperty"][i][0u].asInt();
					value = (*be_it)["extraProperty"][i][1u].asDouble();
					info.extraProperty.insert(std::make_pair(id, value));
				}
			}
			if (!(*be_it)["award"].isNull() && (*be_it)["award"].isArray()) {
				Json_Utility::assign_item_vec((*be_it)["award"], info.award);
			}
			for (Item_Info_Vec::const_iterator it_award = info.award.begin(); it_award != info.award.end(); ++it_award) {
				if (!CONFIG_CACHE_ITEM->find_item(it_award->id)) {
					LOG_ABORT("anci_artifact(%d): award item(%d) not exist", info.id, it_award->id);
				}
			}

			set_map_second_value_by_key(info.id, aa_unban_cond_cfg_, info);
		}
	}

	for (AA_Gem_Cfg::iterator it_gem = aa_gem_cfg_.begin();
			it_gem != aa_gem_cfg_.end(); ++it_gem) {

		for (Int_Hash_Set::const_iterator it_cond = it_gem->second.cond.begin();
				it_cond != it_gem->second.cond.end(); ++it_cond) {
			AA_Cond_Cfg::iterator it_aa_cond = aa_cond_cfg_.find(*it_cond);
			if (it_aa_cond != aa_cond_cfg_.end()) {
				it_aa_cond->second.unban_cond_id = it_gem->second.unban_cond_id;
				it_aa_cond->second.gem_id = it_gem->second.id;
			}
		}
	}
}
