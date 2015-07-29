/*
 * Config_Cache_Secret_Shop.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: root
 */

#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_Secret_Shop.h"
#include "secret_shop/Secret_Shop_Struct.h"

void Config_Cache_Secret_Shop::refresh_config_cache(void){
	refresh_base();
	refresh_point();
	refresh_shop();

	refresh_campfire_base();
	refresh_campfire_point();
	refresh_campfire_gain();
	refresh_campfire_level_exp();
	refresh_campfire_award();

	refresh_month_hero();
}

const Secret_Base_Config* Config_Cache_Secret_Shop::base_config(void){
	return &base_config_;
}

int Config_Cache_Secret_Shop::get_secret_point(const int scene_id, Coord& coord, std::vector<int>& ann_ids, int& ann_space){
	Secret_Point_Map::iterator it =  point_map_.find(scene_id);
	if( it != point_map_.end()){
		int count = it->second.size();
		if(count > 0){
			int index = random()%count;
			coord = it->second[index].coord;
			ann_ids = it->second[index].ann_id_vec;
			ann_space = it->second[index].ann_space;
			if(ann_space < 60){
				ann_space = 60;
			}
			return 0;
		}
	}

	return -1;
}

bool Config_Cache_Secret_Shop::is_secret_point(const int scene_id){
	Secret_Point_Map::iterator it =  point_map_.find(scene_id);
	if( it != point_map_.end()){
		return true;
	}
	return false;
}

int Config_Cache_Secret_Shop::get_secret_shop_item(const int chapman, std::vector<SC_Item_Info>& items){
	int max_count = 9;
	std::vector<const Secret_Item_Config*> item_vec;
	std::vector<const Secret_Item_Config*> item_result;
	get_chapman_item(chapman, copper_item_map_, item_vec);
	get_chapman_item(chapman, gold_item_map_, item_vec);
	get_chapman_item(chapman, rare_item_map_, item_vec);
	int max_weight = 0;
	int item_count = item_vec.size();
	for(int i = 0; i < item_count; ++i){
		max_weight += item_vec[i]->rate;
	}
	for(int k = 0; k < max_count; ++k){
		int weight = random()%max_weight;
		int total_weight = 0;
		for(int i = 0; i < item_count; ++i){
			total_weight += item_vec[i]->rate;
			if(total_weight > weight){
				item_result.push_back(item_vec[i]);
				break;
			}
		}
	}

	int result_count = item_result.size();
	for(int i = 0; i < result_count; ++i){
		SC_Item_Info info;
		info.config = item_result[i];
		items.push_back(info);
	}
	return 0;
}

bool Config_Cache_Secret_Shop::is_valid_activate_time(const int time, int& valid_time){
	if(base_config_.refresh_zone.empty()){
		valid_time = time;
		return true;
	}
	valid_time = base_config_.refresh_zone[0].val_1;
	int count = base_config_.refresh_zone.size();
	for(int i = 0; i < count; ++i){
		if(time < base_config_.refresh_zone[i].val_1){
			valid_time = base_config_.refresh_zone[i].val_1;
			return false;
		}else{
			if(time < base_config_.refresh_zone[i].val_2){
				return true;
			}
		}
	}
	return false;
}


void Config_Cache_Secret_Shop::add_secret_point(const Secret_Point_Config& config){
	Secret_Point_Map::iterator it = point_map_.find(config.scene_id);
	if( it != point_map_.end()){
		it->second.push_back(config);
	}else{
		std::vector<Secret_Point_Config> point_vec;
		point_vec.push_back(config);
		point_map_.insert(std::make_pair(config.scene_id, point_vec));
	}
}

void Config_Cache_Secret_Shop::get_random_shop_item(const Secret_Item_Config_Map& item_map, int count, std::vector<SC_Item_Info>& items){
	int add_count = 0;
	Secret_Item_Config_Map::const_iterator it;
	for(it = item_map.begin(); it != item_map.end(); ++it){
		SC_Item_Info info;
		info.config = &(it->second);
		items.push_back(info);
		add_count += 1;
		if( add_count >= count){
			return;
		}
	}
}

std::vector<Coord> Config_Cache_Secret_Shop::get_campfire_monster_points() {
	return campfire_monster_point_.meat_coord;
}

Coord Config_Cache_Secret_Shop::get_goddess_offset(void) {
	return campfire_base_.goddess_offset;
}

Campfire_Award_Config* Config_Cache_Secret_Shop::campfire_award_config(void) {
	return &campfire_award_cfg_;
}

void Config_Cache_Secret_Shop::get_chapman_item(int chapman, const Secret_Item_Config_Map& item_map, std::vector<const Secret_Item_Config*>& item_vec){
	Secret_Item_Config_Map::const_iterator it;
	for(it = item_map.begin(); it != item_map.end(); ++it){
		if(it->second.chapman == 0 || (it->second.chapman == chapman)){
			item_vec.push_back(&(it->second));
		}
	}
}

Coord Config_Cache_Secret_Shop::get_campfire_birth_coord(const int point_id){
	Campfire_Point_Config_Map::iterator it =  campfire_birth_point_map_.find(point_id);
	if(it != campfire_birth_point_map_.end()){
		int count = it->second.meat_coord.size();
		if(count > 0){
			return it->second.meat_coord[random()%count];
		}
	}
	return Coord(0,0,1);
}

int Config_Cache_Secret_Shop::get_campfire_level_exp(int level){
	Campfire_Level_Exp_Map::iterator it = campfire_level_exp_map_.find(level);
	if(it != campfire_level_exp_map_.end()){
		return it->second;
	}
	return 1;
}

int Config_Cache_Secret_Shop::get_campfire_level_limit(void){
	return campfire_base_.level_limit;
}

int Config_Cache_Secret_Shop::get_campfire_dancing_time(void){
	if(campfire_base_.dancing_time < 10){
		return 10;
	}
	return campfire_base_.dancing_time;
}

int Config_Cache_Secret_Shop::get_campfire_exp_time(void){
	if(campfire_base_.exp_time < 5){
		return 5;
	}
	return campfire_base_.exp_time;
}

int Config_Cache_Secret_Shop::cal_campfire_open_time(const int cur_time, int& valid_status, int& valid_time, int& valid_last_time, bool& next_day){
	int count = campfire_base_.open_time.size();
	if(count <= 0){
		return -1;
	}
	for(int i = 0; i < count; ++i){
		int cfg_time = campfire_base_.open_time[i].val_1;
		int b_time  = cfg_time/100*Time_Value::ONE_HOUR_IN_SECS+cfg_time%100*Time_Value::ONE_MINUTE_IN_SECS;
		int f_time = b_time + campfire_base_.open_time[i].val_2;
		if(cur_time  >= f_time){
			continue;
		}
		next_day = false;
		if(cur_time >= b_time){
			valid_status = 3;
		}else if(cur_time >= (b_time - 5*60)){
			valid_status = 2;
		}else{
			valid_status = 1;
		}
		valid_time = b_time;
		valid_last_time = campfire_base_.open_time[i].val_2;
		return 0;
	}
	next_day = true;
	valid_status = 1;
	int cfg_time = campfire_base_.open_time[0].val_1;
	valid_time  = cfg_time/100*Time_Value::ONE_HOUR_IN_SECS+cfg_time%100*Time_Value::ONE_MINUTE_IN_SECS;
	valid_last_time = campfire_base_.open_time[0].val_2;
	return 0;
}

void Config_Cache_Secret_Shop::get_month_hero_day(const int game_open_day, int& cur_day, int& next_day){
	int count = this->month_hero_info_.size();
	if(count <= 0){
		return;
	}
	int day1 = 0;
	int day2 = 0;
	for(int i = 0; i < count; ++i){
		if(game_open_day >= month_hero_info_[i]){
			day1 = month_hero_info_[i];
			continue;
		}
		day2 = month_hero_info_[i];
		break;
	}
	if(day1 != 0){
		if(day2 == 0){
			day2 = 30;
		}
	}
	cur_day = day1;
	next_day = day2;
}

Campfire_Base_Config* Config_Cache_Secret_Shop::campfire_base_config(void){
	return &campfire_base_;
}

Campfire_Point_Config* Config_Cache_Secret_Shop::campfire_meat_point_config(){
	return &campfire_meat_point_;
}

Campfire_Gain_Config* Config_Cache_Secret_Shop::get_campfire_gain(int level){
	Campfire_Gain_Map::iterator it = campfire_gain_map_.find(level);
	if(it != campfire_gain_map_.end()){
		return &(it->second);
	}
	return NULL;
}

int Config_Cache_Secret_Shop::get_campfire_level_max_firewood(int cur_level){
	Campfire_Gain_Map::iterator it = campfire_gain_map_.find(cur_level);
	if(it != campfire_gain_map_.end()){
		return it->second.firewood_num;
	}
	return 1;
}

void Config_Cache_Secret_Shop::refresh_base(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["secret_shop"]["secret_base"];
	base_config_.reset();
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		if (config_json.isMember("refresh_time")){
			Json_Utility::assign_int_int(config_json["refresh_time"], base_config_.refresh_time);
		}
		if (config_json.isMember("duration_time")){
			Json_Utility::assign_int_int(config_json["duration_time"], base_config_.duration_time);
		}
		if (config_json.isMember("refresh_zone")){
			Json_Utility::assign_int_int_vec(config_json["refresh_zone"], base_config_.refresh_zone);
		}
	}
}

void Config_Cache_Secret_Shop::refresh_point(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["secret_shop"]["refresh_point"];
	point_map_.clear();
	Secret_Point_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("scene_id")){
			cache.scene_id = config_json["scene_id"].asInt();
		}
		if (config_json.isMember("coord")){
			Json_Utility::assign_coord(config_json["coord"], cache.coord);
		}
		if(config_json.isMember("ann")){
			Json_Utility::assign_int_vec(config_json["ann"], cache.ann_id_vec);
		}
		if(config_json.isMember("space")){
			cache.ann_space = config_json["space"].asInt();
		}
		if(cache.scene_id != 0){
			this->add_secret_point(cache);
		}
	}
}

void Config_Cache_Secret_Shop::refresh_shop(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["secret_shop"]["secret_shop"];
	Secret_Item_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("item")){
			cache.item = config_json["item"].asInt();
		}
		if (config_json.isMember("num")){
			cache.num = config_json["num"].asInt();
		}
		if (config_json.isMember("money_type")){
			cache.money_type = config_json["money_type"].asInt();
		}
		if (config_json.isMember("cost")){
			cache.cost = config_json["cost"].asInt();
		}
		if (config_json.isMember("limit_num")){
			cache.limit_num = config_json["limit_num"].asInt();
		}
		if (config_json.isMember("type")){
			cache.type = config_json["type"].asInt();
		}
		if (config_json.isMember("rate")){
			cache.rate = config_json["rate"].asInt();
		}
		if(config_json.isMember("binding")){
			cache.binding = config_json["binding"].asInt();
		}
		if (config_json.isMember("chapman")){
			cache.chapman = config_json["chapman"].asInt();
		}
		if(cache.type == 0){
			set_map_second_value_by_key(cache.item, copper_item_map_, cache);
		}else if(cache.type == 1){
			set_map_second_value_by_key(cache.item, gold_item_map_, cache);
		}else if(cache.type == 2){
			set_map_second_value_by_key(cache.item, rare_item_map_, cache);
		}

	}
}

void Config_Cache_Secret_Shop::refresh_campfire_base(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["campfire"]["fire_base"];
	campfire_base_.reset();
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		if (config_json.isMember("fire_range")){
			campfire_base_.fire_range = config_json["fire_range"].asInt();
		}
		if (config_json.isMember("meat_cont")){
			Json_Utility::assign_int_int(config_json["meat_cont"], campfire_base_.meat_cont);
		}
		if (config_json.isMember("level_limit")){
			campfire_base_.level_limit = config_json["level_limit"].asInt();
		}
		if (config_json.isMember("dancing_time")){
			campfire_base_.dancing_time = config_json["dancing_time"].asInt();
		}
		if (config_json.isMember("time")){
			campfire_base_.exp_time = config_json["time"].asInt();
		}
		if (config_json.isMember("open_time")){
			Json_Utility::assign_int_int_vec(config_json["open_time"], campfire_base_.open_time);
		}
		if (config_json.isMember("text_fire_success")){
			campfire_base_.text_fire_success = config_json["text_fire_success"].asString();
		}
		if (config_json.isMember("text_firewood_success")){
			campfire_base_.text_firewood_success = config_json["text_firewood_success"].asString();
		}

		if (config_json.isMember("text_enter_own_fire")){
			campfire_base_.text_enter_own_fire = config_json["text_enter_own_fire"].asString();
		}
		if (config_json.isMember("text_enter_other_fire")){
			campfire_base_.text_enter_other_fire = config_json["text_enter_other_fire"].asString();
		}
		if (config_json.isMember("text_leave_fire")){
			campfire_base_.text_leave_fire = config_json["text_leave_fire"].asString();
		}
		if (config_json.isMember("birth")){
			Json_Utility::assign_coord(config_json["birth"], campfire_base_.goddess_offset);
		}
	}
}

void Config_Cache_Secret_Shop::refresh_campfire_point(void){
	{
		const Json::Value& info_list =
				CONFIG_INSTANCE->config_json()["campfire"]["meat_point"];
		campfire_meat_point_.reset();
		for (Json::Value::const_iterator it = info_list.begin(); it
				!= info_list.end(); ++it)
		{
			const Json::Value &config_json = (*it);
			Coord coord;
			if (config_json.isMember("coord")){
				Json_Utility::assign_coord(config_json["coord"], coord);
				campfire_meat_point_.meat_coord.push_back(coord);
			}
		}
	}

	Campfire_Point_Config birth_point_cache;
	{
		const Json::Value& info_list =
				CONFIG_INSTANCE->config_json()["campfire"]["birth_point"];

		birth_point_cache.reset();
		for (Json::Value::const_iterator it = info_list.begin(); it
				!= info_list.end(); ++it)
		{
			const Json::Value &config_json = (*it);
			Coord coord;
			if (config_json.isMember("coord")){
				Json_Utility::assign_coord(config_json["coord"], coord);
				birth_point_cache.meat_coord.push_back(coord);
			}
		}
		set_map_second_value_by_key(0, campfire_birth_point_map_, birth_point_cache);
	}

	{
		const Json::Value& info_list =
				CONFIG_INSTANCE->config_json()["campfire"]["birth_point1"];

		for (Json::Value::const_iterator it = info_list.begin(); it
				!= info_list.end(); ++it){
			birth_point_cache.reset();
			const Json::Value &config_json = (*it);
			Coord coord;
			if (config_json.isMember("coord")){
				for (Json::Value::iterator s_it = config_json["coord"].begin();
						s_it != config_json["coord"].end(); ++s_it) {
					Coord coord;
					Json_Utility::assign_coord(*s_it, coord);
					birth_point_cache.meat_coord.push_back(coord);
				}
			}
			int point_id = 0;
			if (config_json.isMember("id")){
				point_id = config_json["id"].asInt();
			}
			if(point_id != 0){
				set_map_second_value_by_key(point_id, campfire_birth_point_map_, birth_point_cache);
			}
		}
	}

	{
		const Json::Value& info_list =
				CONFIG_INSTANCE->config_json()["campfire"]["monster_birth"];

		campfire_monster_point_.reset();
		for (Json::Value::const_iterator it = info_list.begin(); it
		!= info_list.end(); ++it)
		{
			const Json::Value &config_json = (*it);
			Coord coord;
			if (config_json.isMember("birth_point")){
				Json_Utility::assign_coord(config_json["birth_point"], coord);
				campfire_monster_point_.meat_coord.push_back(coord);
			}
		}
	}
}

void Config_Cache_Secret_Shop::refresh_campfire_gain(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["campfire"]["fire_gain"];
	Campfire_Gain_Config cache;
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("level")){
			cache.level = config_json["level"].asInt();
		}
		if (config_json.isMember("exp_add")){
			cache.exp_add = config_json["exp_add"].asDouble();
		}
		if (config_json.isMember("fs_add")){
			cache.fs_add = config_json["fs_add"].asDouble();
		}
		if (config_json.isMember("firewood_num")){
			cache.firewood_num = config_json["firewood_num"].asInt();
		}
		set_map_second_value_by_key(cache.level, campfire_gain_map_, cache);
	}
}

void Config_Cache_Secret_Shop::refresh_campfire_award(void) {
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["campfire"]["reward"];
	for (Json::Value::const_iterator it = info_list.begin(); it
	!= info_list.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		if (config_json.isMember("id") && config_json.isMember("reward")){
			std::vector<Id_Int_Int> id_int_ints;
			Json_Utility::assign_id_int_int_vec(config_json["reward"], id_int_ints);
			std::vector<Id_Int_Int>::const_iterator iter;
			Campfire_Award_Item_Vec vec;
			for (iter = id_int_ints.begin(); iter != id_int_ints.end(); ++iter) {
				Campfire_Award_Item item;
				item.item_id = (*iter).id;
				item.item_count = (*iter).val1;
				item.bind_type = (*iter).val2;
				vec.push_back(item);
			}
			campfire_award_cfg_.award_configs.insert(std::make_pair(config_json["id"].asInt(), vec));
		}
	}

	const Json::Value& info_list1 = CONFIG_INSTANCE->config_json()["campfire"]["rank"];
	for (Json::Value::const_iterator it = info_list1.begin(); it
	!= info_list1.end(); ++it)
	{
		const Json::Value &config_json = (*it);
		if (config_json.isMember("rank") && config_json.isMember("reward")){
			Rank_Award_Config cfg;

			Int_Int int_int;
			Json_Utility::assign_int_int(config_json["rank"], int_int);
			cfg.rank_start = int_int.val_1;
			cfg.rank_stop  = int_int.val_2;

			std::vector<Id_Int_Int> id_int_ints;
			Json_Utility::assign_id_int_int_vec(config_json["reward"], id_int_ints);
			std::vector<Id_Int_Int>::const_iterator iter;
			for (iter = id_int_ints.begin(); iter != id_int_ints.end(); ++iter) {
				Campfire_Award_Item item;
				item.item_id = (*iter).id;
				item.item_count = (*iter).val1;
				item.bind_type = (*iter).val2;
				cfg.awards.push_back(item);
			}

			campfire_award_cfg_.rank_award_configs.push_back(cfg);
		}
	}
}

void Config_Cache_Secret_Shop::refresh_campfire_level_exp(void){
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["campfire"]["level_exp"];
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		int level = 0;
		int exp_add = 0;
		const Json::Value &config_json = (*it);
		if (config_json.isMember("level")){
			level = config_json["level"].asInt();
		}
		if (config_json.isMember("exp_add")){
			exp_add = config_json["exp_add"].asInt();
		}
		set_map_second_value_by_key(level, campfire_level_exp_map_, exp_add);
	}
}

void Config_Cache_Secret_Shop::refresh_month_hero(void){
	month_hero_info_.clear();
	const Json::Value& info_list =
			CONFIG_INSTANCE->config_json()["monthlyHero"]["month_hero"];
	for (Json::Value::const_iterator it = info_list.begin(); it
			!= info_list.end(); ++it)
	{
		int day = 0;
		const Json::Value &config_json = (*it);
		if (config_json.isMember("day")){
			day = config_json["day"].asInt();
			month_hero_info_.push_back(day);
		}
	}
}

std::string Config_Cache_Secret_Shop::get_text_fire_success(void){
	return campfire_base_.text_fire_success;
}

std::string Config_Cache_Secret_Shop::get_text_firewood_success(void){
	return campfire_base_.text_firewood_success;
}

std::string Config_Cache_Secret_Shop::get_text_enter_own_fire(void){
	return campfire_base_.text_enter_own_fire;
}

std::string Config_Cache_Secret_Shop::get_text_enter_other_fire(void){
	return campfire_base_.text_enter_other_fire;
}

std::string Config_Cache_Secret_Shop::get_text_leave_fire(void){
	return campfire_base_.text_leave_fire;
}

