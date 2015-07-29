/*
 * Config_Cache_Mount.cpp
 *
 *  Created on: 2014年7月23日
 *      Author: root
 */

#include "Config_Cache_Mount.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "fighter/Fighter_Def.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Item.h"


void Config_Cache_Mount::refresh_config_cache(void){
	refresh_horse_base();
	refresh_horse_group();
	refresh_horse_train();
	refresh_horse_quality();
	refresh_horse_skill();
	refresh_horse_squad();
	refresh_horse_fuse();
	check_config_valid();
}

Horse_Base_Config* Config_Cache_Mount::get_horse_base_config(int id){
	Horse_Base_Config_Map::iterator it = horse_base_config_map_.find(id);
	if(it != horse_base_config_map_.end()){
		return &(it->second);
	}
	return NULL;
}

Horse_Group_Config* Config_Cache_Mount::get_horse_group_config(int id){
	Horse_Group_Config_Map::iterator it = horse_group_config_map_.find(id);
	if(it != horse_group_config_map_.end()){
		return &(it->second);
	}
	return NULL;
}

Horse_Train_Config* Config_Cache_Mount::get_horse_train_config(const int train_star, const int train_lv){
	int index = get_train_index(train_star, train_lv);
	Horse_Train_Config_Map::iterator it = horse_train_map_.find(index);
	if(it != horse_train_map_.end()){
		return &(it->second);
	}
	return NULL;
}

Horse_Base_Config_Map* Config_Cache_Mount::horse_base_config_map(void){
	return &horse_base_config_map_;
}

Horse_Fuse_Config* Config_Cache_Mount::get_horse_fuse_config(const int id){
	Horse_Fuse_Config_Map::iterator it = horse_fuse_map_.find(id);
	if( it != horse_fuse_map_.end()){
		return &(it->second);
	}
	return NULL;
}


void Config_Cache_Mount::find_activate_group(const std::vector<Int_Int>& horse_list, std::vector<int>& group_list, const std::vector<Horse_Group_Info> group_info){
	Horse_Group_Config_Map::iterator it;
	for(it = horse_group_config_map_.begin(); it != horse_group_config_map_.end(); ++it){
		if( is_in_group_info(it->second.group_id, group_info) ||
				check_group_activate(horse_list, it->second.group)){
			group_list.push_back(it->second.group_id);
		}
	}
}

int Config_Cache_Mount::find_horse_id(const int card_id) {
	Int_Int_Map::const_iterator it = card_id_horse_id_map_.find(card_id);
	if (it != card_id_horse_id_map_.end()) {
		return it->second;
	}
	return 0;
}

bool Config_Cache_Mount::get_horse_train_cost(
		const int train_star, const int train_lv, int &prop_id, int &prop_count, int &money,
		int &base_ratio, int &add_ratio, int &add_luck, int &max_luck, int &update_num, int &qlty_coe){
	int index = get_train_index(train_star, train_lv);
	Horse_Train_Config_Map::iterator it = horse_train_map_.find(index);
	if(it != horse_train_map_.end()){
		prop_id = it->second.consume.val_1;
		prop_count = it->second.consume.val_2;
		money = it->second.cast_money;
		base_ratio = it->second.success_train_base;
		add_ratio = it->second.success_train_once;
		add_luck = it->second.luck_once;
		max_luck = it->second.luck_max;
		update_num = it->second.upgrade_num;
		qlty_coe = it->second.qlty_coe;
		return true;
	}
	return false;
}

bool Config_Cache_Mount::get_horse_train_up_cost(const int train_star, int& prop_id, int &prop_count){
	int index = get_train_index(train_star, 0);
	Horse_Train_Config_Map::iterator it = horse_train_map_.find(index);
	if(it != horse_train_map_.end()){
		prop_id = it->second.up_consume.val_1;
		prop_count = it->second.up_consume.val_2;
		if(prop_id > 0 && prop_count > 0){
			return true;
		}else{
			return false;
		}
	}
	return false;
}

int Config_Cache_Mount::get_horse_train_skill_ratio(const int quality){
	Horse_Quality_Config_Map::iterator it = horse_quality_map_.find(quality);
	if(it != horse_quality_map_.end()){
		return it->second.get_skill;
	}
	return 10;
}

int Config_Cache_Mount::get_max_train_lv(int quality){
	Horse_Quality_Config_Map::iterator it = horse_quality_map_.find(quality);
	if(it != horse_quality_map_.end()){
		return it->second.high_star;
	}
	return 1;
}

void Config_Cache_Mount::get_star_lv_update_num(const int star_lv, int& num, int& qlty_coe){
	int index = get_train_index(star_lv, 0);
	Horse_Train_Config_Map::iterator it = horse_train_map_.find(index);
	if(it != horse_train_map_.end()){
		num = it->second.upgrade_num;
		qlty_coe = it->second.qlty_coe;
	}
	return;
}

int Config_Cache_Mount::get_next_update_train_num(const int train_star){
	int index = get_train_index(train_star, 0);
	Horse_Train_Config_Map::iterator it = horse_train_map_.find(index);
	if(it != horse_train_map_.end()){
		return it->second.upgrade_num;
	}
	return 0;
}

int Config_Cache_Mount::get_deploy_max_skill_num(const int quality){
	Horse_Quality_Config_Map::iterator it = horse_quality_map_.find(quality);
	if( it != horse_quality_map_.end()){
		return it->second.skill_num;
	}
	return 1;
}

void Config_Cache_Mount::get_deploy_skill_info(const int train_star, const int train_lv, int &open_skill_num, int &max_skill_lv){
	int index = get_train_index(train_star, train_lv);
	Horse_Train_Config_Map::iterator it = horse_train_map_.find(index);
	if(it != horse_train_map_.end()){
		open_skill_num = it->second.skill_open;
		max_skill_lv = it->second.skill_lv;
	}else{
		open_skill_num = 0;
		max_skill_lv = 1;
	}
	return;
}

int Config_Cache_Mount::get_random_deploy_skill(const int race, const int max_skill_lv){
	Horse_Skill_Config_Map::iterator it = horse_skill_map_.find(race);
	if(it != horse_skill_map_.end()){
		Int_Vec skills;
		int count = it->second.items.size();
		for(int i = 0; i < count; ++i){
			const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(it->second.items[i]);
			if (!cfg) {
				continue;
			}
			if(cfg->color > max_skill_lv){
				continue;
			}
			if(cfg->special_cfg.type != 1) {
				continue;
			}
			skills.push_back(cfg->special_cfg.addon.mount_skill);
		}
		if(skills.empty() == false){
			int result_count = skills.size();
			int index = random()%result_count;
			return skills[index];
		}
	}
	return 0;
}

bool Config_Cache_Mount::is_race_deploy_skill(const int race, const int prop_id){
	Horse_Skill_Config_Map::const_iterator it =  horse_skill_map_.find(race);
	if(it != horse_skill_map_.end()){
		int count = it->second.items.size();
		for(int i = 0; i < count; ++i){
			if(it->second.items[i] == prop_id){
				return true;
			}
		}
	}
	return false;
}

void Config_Cache_Mount::cal_train_attr(const int horse_id, const int train_lv, const int train_num, const int max_train_lv,
		int &next_update_num, int &total_train_num, int &next_lv_total_train_num,
		int &cur_attr_1, int &cur_attr_2,int &cur_attr_3, int &cur_attr_4, int &cur_attr_5,
		int &max_attr_1, int &max_attr_2, int &max_attr_3, int &max_attr_4, int &max_attr_5){
	const Horse_Base_Config* h_cfg = get_horse_base_config(horse_id);
	if(!h_cfg){
		return;
	}
	int initial_lv = h_cfg->initial_lv;
	if(initial_lv > train_lv){
		initial_lv = train_lv;
	}
	next_update_num = get_next_update_train_num(train_lv);
	int upgrade_num = 0;
	int cur_upgrade_num = 0;
	for(int i = initial_lv; i <= train_lv; ++i){
		int cur_star_lv_num = get_next_update_train_num(i);
		for(int k = 0; k < cur_star_lv_num; ++k){
			Horse_Train_Config* t_cfg = get_horse_train_config(i, k);
			if(!t_cfg){
				continue;
			}
			int add_val1 = h_cfg->attr_qlty1*t_cfg->qlty_coe/1000;
			int add_val2 = h_cfg->attr_qlty2*t_cfg->qlty_coe/1000;
			int add_val3 = h_cfg->attr_qlty3*t_cfg->qlty_coe/1000;
			int add_val4 = h_cfg->attr_qlty4*t_cfg->qlty_coe/1000;
			int add_val5 = h_cfg->attr_qlty5*t_cfg->qlty_coe/1000;

			max_attr_1 += add_val1;
			max_attr_2 += add_val2;
			max_attr_3 += add_val3;
			max_attr_4 += add_val4;
			max_attr_5 += add_val5;

			if(i < train_lv){
				cur_attr_1 += add_val1;
				cur_attr_2 += add_val2;
				cur_attr_3 += add_val3;
				cur_attr_4 += add_val4;
				cur_attr_5 += add_val5;

			}else{
				if(k < train_num){
					cur_attr_1 += add_val1;
					cur_attr_2 += add_val2;
					cur_attr_3 += add_val3;
					cur_attr_4 += add_val4;
					cur_attr_5 += add_val5;
				}
			}
		}
		upgrade_num += cur_star_lv_num;
		if(i < train_lv){
			cur_upgrade_num += cur_star_lv_num;
		}else{
			cur_upgrade_num += train_num;
		}
	}

	total_train_num = cur_upgrade_num;
	next_lv_total_train_num = upgrade_num;

	cur_attr_1 += h_cfg->attr_1;
	cur_attr_2 += h_cfg->attr_2;
	cur_attr_3 += h_cfg->attr_3;
	cur_attr_4 += h_cfg->attr_4;
	cur_attr_5 += h_cfg->attr_5;

	max_attr_1 += h_cfg->attr_1;
	max_attr_2 += h_cfg->attr_2;
	max_attr_3 += h_cfg->attr_3;
	max_attr_4 += h_cfg->attr_4;
	max_attr_5 += h_cfg->attr_5;

}

void Config_Cache_Mount::cal_first_train_lv_max_attr(const int horse_id, int &train_star, int &max_attr1, int &max_attr2,
		int &max_attr3, int &max_attr4, int &max_attr5, int &next_update_num){
	const Horse_Base_Config* h_cfg = get_horse_base_config(horse_id);
	if(!h_cfg){
		return;
	}
	train_star = h_cfg->initial_lv;
	next_update_num = get_next_update_train_num(train_star);
	for(int i = 0; i < next_update_num; ++i){
		Horse_Train_Config* t_cfg = get_horse_train_config(train_star, i);
		if(!t_cfg){
			continue;
		}
		max_attr1 += ((int)(h_cfg->attr_qlty1*t_cfg->qlty_coe/1000));
		max_attr2 += ((int)(h_cfg->attr_qlty2*t_cfg->qlty_coe/1000));
		max_attr3 += ((int)(h_cfg->attr_qlty3*t_cfg->qlty_coe/1000));
		max_attr4 += ((int)(h_cfg->attr_qlty4*t_cfg->qlty_coe/1000));
		max_attr5 += ((int)(h_cfg->attr_qlty5*t_cfg->qlty_coe/1000));
	}
	max_attr1 += h_cfg->attr_1;
	max_attr2 += h_cfg->attr_2;
	max_attr3 += h_cfg->attr_3;
	max_attr4 += h_cfg->attr_4;
	max_attr5 += h_cfg->attr_5;
}

int Config_Cache_Mount::get_train_fail_luck_value(const int train_star, const int train_lv){
	int index = get_train_index(train_star, train_lv);
	Horse_Train_Config_Map::iterator it = horse_train_map_.find(index);
	if(it != horse_train_map_.end()){
		return it->second.luck_once;
	}
	return 0;
}

void Config_Cache_Mount::php_notice_fuse_time(const int horse_id, const int64_t begin_time, const int64_t end_time){
	Int_Int value;
	value.val_1 = begin_time;
	value.val_2 = end_time;
	horse_php_fuse_time_map_[horse_id] = value;
}

void Config_Cache_Mount::get_fuse_time(std::vector<Int_Int_Int>& times){
	for(boost::unordered_map<int, Int_Int>::iterator it = horse_php_fuse_time_map_.begin();
			it != horse_php_fuse_time_map_.end(); ++it){
		Int_Int_Int value;
		value.val1 = it->first;
		value.val2 = it->second.val_1;
		value.val3 = it->second.val_2;
		times.push_back(value);
	}
}

bool Config_Cache_Mount::check_fuse_time(const int horse_id){
	boost::unordered_map<int, Int_Int>::iterator it = horse_php_fuse_time_map_.find(horse_id);
	if(it != horse_php_fuse_time_map_.end()){
		int now_sec = Time_Value::gettimeofday().sec();
		if(now_sec >= it->second.val_1 && now_sec <= it->second.val_2){
			return true;
		}
	}
	return false;
}

bool Config_Cache_Mount::check_group_activate(const std::vector<Int_Int>& horse_list, const std::vector<Int_Int>& group_horse_list){
	int count = group_horse_list.size();
	for(int i = 0; i < count; ++i){
		bool s_find = false;
		int s_count = horse_list.size();
		for(int k = 0; k < s_count; ++k){
			if(group_horse_list[i].val_1 == horse_list[k].val_1 && group_horse_list[i].val_2 <= horse_list[k].val_2){
				s_find = true;
				break;
			}
		}
		if(!s_find){
			return false;
		}
	}
	return true;
}

bool Config_Cache_Mount::is_in_group_info(const int group_id, const std::vector<Horse_Group_Info> group_info){
	bool result = false;
	std::vector<Horse_Group_Info>::const_iterator it;
	for(it = group_info.begin(); it != group_info.end(); ++it){
		if(group_id == it->group_id){
			if(it->status == 0){
				result = false;
			}else{
				result = true;
			}
			break;
		}
	}
	return result;
}

void Config_Cache_Mount::refresh_horse_base(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["horse"]["horse_base"];
	Horse_Base_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("horse_id")){
			cache.horse_id = config_json["horse_id"].asInt();
		}
		if (config_json.isMember("horse_name")){
			cache.horse_name = config_json["horse_name"].asString();
		}
		//prop
		if (config_json.isMember("attr_1")){
			Int_Int prop_value;
			Json_Utility::assign_int_int(config_json["attr_1"], prop_value);
			cache.attr_1 = prop_value.val_2;
		}
		if (config_json.isMember("attr_2")){
			Int_Int prop_value;
			Json_Utility::assign_int_int(config_json["attr_2"], prop_value);
			cache.attr_2 = prop_value.val_2;
		}
		if (config_json.isMember("attr_3")){
			Int_Int prop_value;
			Json_Utility::assign_int_int(config_json["attr_3"], prop_value);
			cache.attr_3 = prop_value.val_2;
		}
		if (config_json.isMember("attr_4")){
			Int_Int prop_value;
			Json_Utility::assign_int_int(config_json["attr_4"], prop_value);
			cache.attr_4 = prop_value.val_2;
		}
		if (config_json.isMember("attr_5")){
			Int_Int prop_value;
			Json_Utility::assign_int_int(config_json["attr_5"], prop_value);
			cache.attr_5 = prop_value.val_2;
		}
		//
		if (config_json.isMember("horse_quality")){
			cache.quality = config_json["horse_quality"].asInt();
		}
		if (config_json.isMember("mod_id")){
			if(config_json["mod_id"].isArray()){
				Json_Utility::assign_int_int_vec(config_json["mod_id"], cache.mod_id);
			}else{
				Int_Int int_value;
				int_value.val_1 = 0;
				int_value.val_2 = config_json["mod_id"].asInt();
				cache.mod_id.push_back(int_value);
			}

		}
		if (config_json.isMember("speed")){
			cache.speed = config_json["speed"].asInt();
		}
		if (config_json.isMember("time")){
			cache.time = config_json["time"].asInt();
		}
		if (config_json.isMember("vip_own")){
			cache.vip_own = config_json["vip_own"].asInt();
		}
		if (config_json.isMember("num")){
			Json_Utility::assign_int_int(config_json["num"], cache.num);
		}
		if (config_json.isMember("card_id")){
			cache.card_id = config_json["card_id"].asInt();
			card_id_horse_id_map_.insert(std::make_pair(cache.card_id, cache.horse_id));
		}

		if (config_json.isMember("horse_quality")){
			cache.quality = config_json["horse_quality"].asInt();
		}
		if (config_json.isMember("hores_race")){
			cache.race = config_json["hores_race"].asInt();
		}

		if (config_json.isMember("ATN_quality")){
			cache.attr_qlty1 = config_json["ATN_quality"].asInt();
		}
		if (config_json.isMember("INT_quality")){
			cache.attr_qlty2 = config_json["INT_quality"].asInt();
		}
		if (config_json.isMember("DEF_quality")){
			cache.attr_qlty3 = config_json["DEF_quality"].asInt();
		}
		if (config_json.isMember("RES_quality")){
			cache.attr_qlty4 = config_json["RES_quality"].asInt();
		}
		if (config_json.isMember("CON_quality")){
			cache.attr_qlty5 = config_json["CON_quality"].asInt();
		}
		if (config_json.isMember("initial")){
			cache.initial_lv = config_json["initial"].asInt();
		}

		set_map_second_value_by_key(cache.horse_id, horse_base_config_map_, cache);
	}
}

void Config_Cache_Mount::refresh_horse_group(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["horse"]["horse_group"];
	Horse_Group_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("group_id")){
			cache.group_id = config_json["group_id"].asInt();
		}
		// group
		if (config_json.isMember("group")){
			Json_Utility::assign_int_int_vec(config_json["group"], cache.group);
		}
		if (config_json.isMember("skill")){
			cache.skill = config_json["skill"].asInt();
		}
		set_map_second_value_by_key(cache.group_id, horse_group_config_map_, cache);
	}
}

void Config_Cache_Mount::refresh_horse_train(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["horse"]["horse_train"];
	Horse_Train_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("star_lv")){
			cache.star_lv = config_json["star_lv"].asInt();
		}
		if (config_json.isMember("skill_open")){
			cache.skill_open = config_json["skill_open"].asInt();
		}
		if (config_json.isMember("skill_lv")){
			cache.skill_lv = config_json["skill_lv"].asInt();
		}
		if (config_json.isMember("cast_money")){
			cache.cast_money = config_json["cast_money"].asInt();
		}
		//prop
		if (config_json.isMember("consume")){
			Json_Utility::assign_int_int(config_json["consume"], cache.consume);
		}
		if (config_json.isMember("up_consume")){
			Json_Utility::assign_int_int(config_json["up_consume"], cache.up_consume);
		}

		if (config_json.isMember("success_train_base")){
			cache.success_train_base = config_json["success_train_base"].asInt();
		}
		if (config_json.isMember("success_train_once")){
			cache.success_train_once = config_json["success_train_once"].asInt();
		}
		if (config_json.isMember("luck_max")){
			cache.luck_max = config_json["luck_max"].asInt();
		}
		if (config_json.isMember("luck_once")){
			cache.luck_once = config_json["luck_once"].asInt();
		}
		if (config_json.isMember("upgrade_num")){
			cache.upgrade_num = config_json["upgrade_num"].asInt();
		}
		if (config_json.isMember("qlty_coe")){
			cache.qlty_coe = config_json["qlty_coe"].asInt();
		}
		if (config_json.isMember("broadcast")){
			cache.broadcast = config_json["broadcast"].asInt();
		}
		set_map_second_value_by_key(cache.star_lv, horse_train_map_, cache);
	}
}

void Config_Cache_Mount::refresh_horse_quality(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["horse"]["horse_special"];
	Horse_Quality_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("quality")){
			cache.quality = config_json["quality"].asInt();
		}
		if (config_json.isMember("high_star")){
			cache.high_star = config_json["high_star"].asInt();
		}
		if (config_json.isMember("skill_num")){
			cache.skill_num = config_json["skill_num"].asInt();
		}
		if (config_json.isMember("get_skill")){
			cache.get_skill = config_json["get_skill"].asInt();
		}
		set_map_second_value_by_key(cache.quality, horse_quality_map_, cache);
	}
}

void Config_Cache_Mount::refresh_horse_skill(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["horse"]["horse_skill"];
	Horse_Skill_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("hores_race")){
			cache.race = config_json["hores_race"].asInt();
		}
		if (config_json.isMember("canuse_item")){
			Json_Utility::assign_int_vec(config_json["canuse_item"], cache.items);
		}
		set_map_second_value_by_key(cache.race, horse_skill_map_, cache);
	}
}

void Config_Cache_Mount::refresh_horse_squad(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["horse"]["squad"];
	Horse_Squad_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("squad_num")){
			cache.squad_num = config_json["squad_num"].asInt();
		}
		if (config_json.isMember("lv")){
			cache.lv = config_json["lv"].asInt();
		}
		set_map_second_value_by_key(cache.squad_num, horse_squad_map_, cache);
	}
}

void Config_Cache_Mount::refresh_horse_fuse(void){
	const Json::Value& info_list = CONFIG_INSTANCE->config_json()["horse"]["horse_fuse"];
	Horse_Fuse_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it){
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("horse_id")){
			cache.horse_id = config_json["horse_id"].asInt();
		}
		if (config_json.isMember("condition")){
			Json_Utility::assign_int_int_vec(config_json["condition"], cache.condition);
		}
		if (config_json.isMember("consume")){
			Json_Utility::assign_int_int_vec(config_json["consume"], cache.consume);
		}
		if(cache.condition.empty() || cache.consume.empty()){
			LOG_ABORT("horse fuse config error(condition or consume empty):[%d]", cache.horse_id);
		}
		set_map_second_value_by_key(cache.horse_id, horse_fuse_map_, cache);
	}
}

void Config_Cache_Mount::check_config_valid(void){
	// level config check
	int max_star = 0;
	for(Horse_Quality_Config_Map::iterator it = horse_quality_map_.begin();
			it != horse_quality_map_.end(); ++it){
		if(it->second.high_star > max_star){
			max_star = it->second.high_star;
		}
	}
	for(int i = 0; i <= max_star; ++i){
		Horse_Train_Config* max_cfg = this->get_horse_train_config(i, 0);
		if(!max_cfg){
			LOG_ABORT("horse train config error(star_lv %d empty)", i*100);
			continue;
		}
		int max_lv = max_cfg->upgrade_num;
		for(int k = 1; k <= max_lv; ++k){
			Horse_Train_Config* lv_cfg = this->get_horse_train_config(i, k);
			if(!lv_cfg){
				LOG_ABORT("horse train config error(star_lv %d empty)", i*100+k);
				continue;
			}
			if(lv_cfg->upgrade_num != max_lv){
				LOG_ABORT("horse train config error(star_lv %d upgrade_num error %d != %d):[%d]", lv_cfg->upgrade_num, max_lv);
			}
		}
	}
}

int Config_Cache_Mount::get_train_index(const int train_star, const int train_lv){
	return train_star*100+train_lv;
}

