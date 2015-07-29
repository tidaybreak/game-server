/*
 * Config_Cache_Exchange.cpp
 *
 *  Created on: 2014年6月25日
 *      Author: Linqiyou
 */

#include "Configurator.h"
#include "exchange/Exchange_Def.h"
#include "Config_Cache_Exchange.h"

const Exchange_Config *Config_Cache_Exchange::find_cache(const int16_t &type) {
	switch(type) {
	case EXCHANGE_TYPE_EQUIP: {
		return &exchange_equip_cfg_;
	}
	case EXCHANGE_TYPE_MEDAL: {
		return &exchange_medal_cfg_;
	}
	case EXCHANGE_TYPE_ITEM: {
		return &exchange_item_cfg_;
	}
	case EXCHANGE_TYPE_FRIEND: {
		return &exchange_friend_cfg_;
	}
	case EXCHANGE_TYPE_ACTIVITY: {
		return &exchange_activity_cfg_;
	}
	default:
		break;
	}
	return NULL;
}

const Exchange_Config_Detail *Config_Cache_Exchange::find_cache(const int16_t &type, const int32_t &id) {
	switch(type) {
	case EXCHANGE_TYPE_EQUIP: {
		return get_map_second_pointer_by_key(id, exchange_equip_cfg_);
	}
	case EXCHANGE_TYPE_MEDAL: {
		return get_map_second_pointer_by_key(id, exchange_medal_cfg_);
	}
	case EXCHANGE_TYPE_ITEM: {
		return get_map_second_pointer_by_key(id, exchange_item_cfg_);
	}
	case EXCHANGE_TYPE_FRIEND: {
		return get_map_second_pointer_by_key(id, exchange_friend_cfg_);
	}
	case EXCHANGE_TYPE_ACTIVITY: {
		return get_map_second_pointer_by_key(id, exchange_activity_cfg_);
	}
	default:
		break;
	}
	return NULL;
}

const Int_IntSet_Map_Reverse *Config_Cache_Exchange::find_lvl_greater(const int16_t &type) {
	switch(type) {
	case EXCHANGE_TYPE_EQUIP: {
		return &ecg_lvl_equip_gt_cfg_;
	}
	case EXCHANGE_TYPE_MEDAL: {
		return &ecg_lvl_medal_gt_cfg_;
	}
	case EXCHANGE_TYPE_ITEM: {
		return &ecg_lvl_item_gt_cfg_;
	}
	case EXCHANGE_TYPE_FRIEND: {
		return &ecg_lvl_friend_gt_cfg_;
	}
	case EXCHANGE_TYPE_ACTIVITY: {
		return &ecg_lvl_activity_gt_cfg_;
	}
	default:
		break;
	}
	return NULL;
}

const Int_Vec *Config_Cache_Exchange::find_lvl(const int16_t &type) {
	switch(type) {
	case EXCHANGE_TYPE_EQUIP: {
		return &ecg_equip_cfg_;
	}
	case EXCHANGE_TYPE_MEDAL: {
		return &ecg_medal_cfg_;
	}
	case EXCHANGE_TYPE_ITEM: {
		return &ecg_item_cfg_;
	}
	case EXCHANGE_TYPE_FRIEND: {
		return &ecg_friend_cfg_;
	}
	case EXCHANGE_TYPE_ACTIVITY: {
		return &ecg_activity_cfg_;
	}
	default:
		break;
	}
	return NULL;
}

void Config_Cache_Exchange::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Exchange");
	ecg_lvl_equip_cfg_.clear();
	ecg_lvl_medal_cfg_.clear();
	ecg_lvl_item_cfg_.clear();
	ecg_lvl_friend_cfg_.clear();
	ecg_lvl_activity_cfg_.clear();
	ecg_lvl_equip_gt_cfg_.clear();
	ecg_lvl_medal_gt_cfg_.clear();
	ecg_lvl_item_gt_cfg_.clear();
	ecg_lvl_friend_gt_cfg_.clear();
	ecg_lvl_activity_gt_cfg_.clear();

	ecg_equip_cfg_.clear();
	ecg_medal_cfg_.clear();
	ecg_item_cfg_.clear();
	ecg_friend_cfg_.clear();
	ecg_activity_cfg_.clear();

	const Json::Value exg_json = CONFIG_INSTANCE->config_json()["exchange"]["gameExchangeConfig"];
	Exchange_Config_Detail detail;
	for (Json::Value::const_iterator it = exg_json.begin(); it != exg_json.end(); ++it) {
		detail.reset();
		detail.idx = (*it)["itemKey"].asInt();
		detail.type = (*it)["index"].asInt();
		detail.id = (*it)["id"].asInt();
		detail.bind = (*it)["binding"].asInt();
		detail.level = (*it)["level"].asInt();
		detail.exchange_type = (*it)["moneytype"].asInt();
		detail.amount = (*it)["money"].asInt();
		detail.reputation = (*it)["reputation"].asInt();
		detail.limit = (*it)["limit"].asInt();
		detail.stack = (*it)["Stack"].asInt();
		switch(detail.type) {
		case EXCHANGE_TYPE_EQUIP: {
			set_map_second_value_by_key(detail.idx, exchange_equip_cfg_, detail);
			ecg_lvl_equip_cfg_[detail.level].insert(detail.idx);
			ecg_lvl_equip_gt_cfg_[detail.level].insert(detail.idx);
			ecg_equip_cfg_.push_back(detail.idx);
			break;
		}
		case EXCHANGE_TYPE_MEDAL: {
			set_map_second_value_by_key(detail.idx, exchange_medal_cfg_, detail);
			ecg_lvl_medal_cfg_[detail.level].insert(detail.idx);
			ecg_lvl_medal_gt_cfg_[detail.level].insert(detail.idx);
			ecg_medal_cfg_.push_back(detail.idx);
			break;
		}
		case EXCHANGE_TYPE_ITEM: {
			set_map_second_value_by_key(detail.idx, exchange_item_cfg_, detail);
			ecg_lvl_item_cfg_[0].insert(detail.idx);
			ecg_lvl_item_gt_cfg_[0].insert(detail.idx);
			ecg_item_cfg_.push_back(detail.idx);
			break;
		}
		case EXCHANGE_TYPE_FRIEND: {
			set_map_second_value_by_key(detail.idx, exchange_friend_cfg_, detail);
			ecg_lvl_friend_cfg_[0].insert(detail.idx);
			ecg_lvl_friend_gt_cfg_[0].insert(detail.idx);
			ecg_friend_cfg_.push_back(detail.idx);
			break;
		}
		case EXCHANGE_TYPE_ACTIVITY: {
			set_map_second_value_by_key(detail.idx, exchange_activity_cfg_, detail);
			ecg_lvl_activity_cfg_[0].insert(detail.idx);
			ecg_lvl_activity_gt_cfg_[0].insert(detail.idx);
			ecg_activity_cfg_.push_back(detail.idx);
			break;
		}
		default:
			LOG_ABORT("error exchange type: %d", detail.type);
			break;
		}
	}

	refresh_config_skill_exchange();
}

const Skill_Exchange_Item *Config_Cache_Exchange::find_skill_exchange_item(const int id) {
	return get_map_second_pointer_by_key(id, skill_exchange_map_);
}

void Config_Cache_Exchange::refresh_config_skill_exchange(void) {

	const Json::Value exg_json = CONFIG_INSTANCE->config_json()["exchange"]["skill_Exchange"];
	Skill_Exchange_Item cfg;
	for (Json::Value::const_iterator it = exg_json.begin(); it != exg_json.end(); ++it)  {
		cfg.reset();
		cfg.id = (*it)["id"].asInt();
		cfg.career = (*it)["career"].asInt();
		cfg.skill_series = (*it)["skill_series"].asInt();
		cfg.skill_lv = (*it)["skill_level"].asInt();
		cfg.type_cost = (*it)["moneytype"].asInt();
		cfg.num_cost = (*it)["val"].asInt();
		cfg.reputation_need = (*it)["reputation"].asInt();
		skill_exchange_map_[cfg.id] = cfg;
	}
}
