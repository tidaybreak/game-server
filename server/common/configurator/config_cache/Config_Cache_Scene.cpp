/*
 * Config_Cache_Scene.cpp
 *
 *  Created on: 2014-4-2
 *      Author: donald shaw
 */

#include "Config_Cache_Scene.h"
#include "Config_Cache_Role.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache.h"

const Scene_Config_Map_Cache &Config_Cache_Scene::scene_config_cache_map(void) const {
	return scene_config_cache_map_;
}

const Scene_Config *Config_Cache_Scene::scene_config_cache(int scene_id) const {
	return get_map_second_pointer_by_key(scene_id, scene_config_cache_map_);
}

const Scene_Map_Data *Config_Cache_Scene::scene_map_data_cache(int id) const {
	return get_map_second_pointer_by_key(id, scene_map_data_cache_map_);
}

void Config_Cache_Scene::refresh_scene_map_cache(std::string &module)  {
	PERF_MON("refresh_scene_map_cache");

	Int_Set scene_set;
	Scene_Map_Data map_config;
	const Json::Value json_scene = CONFIG_INSTANCE->config_json()["map_data"];\
	Int_String_Map int_string_map;
	for (Json::Value::const_iterator it = json_scene.begin(); it != json_scene.end(); it++) {
		map_config.reset();

		const Json::Value mapInfo = (*it)["mapInfo"];
		if (mapInfo == Json::Value::null) LOG_ABORT("找不到:mapInfo");

		map_config.id = mapInfo["mapId"].asInt();

		if (!map_config.id || scene_set.count(map_config.id))
			LOG_ABORT("mapInfo场景id不存在或重复:%d", map_config.id);

		uint baseMapIndex = mapInfo["baseMapIndex"].asInt();
		if (baseMapIndex >= mapInfo["mapLayers"].size()) {
			LOG_ABORT("id:%d, baseMapIndex:%d  mapLayers size:%d", map_config.id, baseMapIndex, mapInfo["mapLayers"].size());
		}

		map_config.head.map_width =  mapInfo["mapLayers"][baseMapIndex]["width"].asInt();
		map_config.head.map_height = mapInfo["mapLayers"][baseMapIndex]["height"].asInt();;

		map_config.head.cell_width = MAP_X_PIEX;
		map_config.head.cell_height = MAP_Y_PIEX;

		map_config.head.x_len = map_config.head.map_width/MAP_X_PIEX;
		map_config.head.x_len = map_config.head.x_len * MAP_X_PIEX < map_config.head.map_width ? map_config.head.x_len + 1 : map_config.head.x_len;
		map_config.head.y_len = map_config.head.map_height/MAP_Y_PIEX;
		map_config.head.y_len = map_config.head.y_len * MAP_Y_PIEX < map_config.head.map_height ? map_config.head.y_len + 1 : map_config.head.y_len;

		JUDGE_ERROR_ABORT(!map_config.head.map_width || !map_config.head.map_height || !map_config.head.x_len || !map_config.head.y_len);

		const Json::Value mapData = (*it)["mapData"];
		if (mapData == Json::Value::null) LOG_ABORT("");

		uint max_len = map_config.head.y_len > map_config.head.x_len ? map_config.head.y_len : map_config.head.x_len;
		if (int_string_map.size() < max_len) {
			for (uint i = int_string_map.size(); i <= max_len; ++i) {
				char temp[64] = {0};
				sprintf(temp, "%d", i);
				int_string_map[i] = std::string(temp);
			}
		}
		for (int y = 0; y < map_config.head.y_len; ++y) {
			std::string &y_s = int_string_map[y];
			const Json::Value &y_json = mapData[y_s];
			for (int x = 0; x < map_config.head.x_len; ++x) {
				if (y_json != Json::Value::null) {
					std::string &x_s = int_string_map[x];
					if (y_json[x_s] != Json::Value::null) {
						int16_t value = y_json[x_s].asInt();
						value = (0 < value && value <= 1000) ? 1 : value;
						map_config.content.push_back(value);

					} else {
						map_config.content.push_back(0);
					}
				} else {
					map_config.content.push_back(0);
				}
			}
		}

		scene_set.insert(map_config.id);
		set_map_second_value_by_key(map_config.id, scene_map_data_cache_map_, map_config);
	}

}



int Config_Cache_Scene::get_map_value(int map_id, Coord &coord) {
	if (coord.type == Coord::GRID_TYPE) {
		return map_value_by_grid_coord(map_id, coord.x, coord.y);
	} else {
		return map_value_by_pixel_coord(map_id, coord.x, coord.y);
	}
}

int Config_Cache_Scene::map_value_by_grid_coord(int map_id, uint16_t x, uint16_t y) {
	const Scene_Map_Data *scene_map_data = scene_map_data_cache(map_id);
	if (!scene_map_data) {
		return -1;
	}

	if (x < 0 || x >= scene_map_data->head.x_len || y < 0 || y >= scene_map_data->head.y_len) {
		return -1;
	}

	uint32_t grid_index = y * scene_map_data->head.x_len + x;
	return scene_map_data->content[grid_index];
}

int Config_Cache_Scene::map_value_by_pixel_coord(int map_id, uint16_t x, uint16_t y) {
	return map_value_by_grid_coord(map_id, pixel_x_to_grid(x), pixel_y_to_grid(y));
}

void Config_Cache_Scene::refresh_config_cache(std::string &module)  {
	PERF_MON("Config_Cache_Scene");

	refresh_scene_map_cache(module);
	refresh_scene_config_cache();
	update_single_dungeon_relation();
}

Block_Buffer &Config_Cache_Scene::client_find_path_point(void) {
	return client_find_path_point_;
}

void Config_Cache_Scene::refresh_scene_config_cache(void)  {
	PERF_MON("refresh_scene_config_cache");

	Relation_Map relation_map;
	Int_Set scene_set;
	Scene_Config scene_config;
	Scene_Config::Transmit_Point_Vec client_find_path_point;
	const Json::Value json_scene = CONFIG_INSTANCE->config_json()["scene"]["scene_config"];
	for (Json::Value::const_iterator it = json_scene.begin(); it != json_scene.end(); it++) {
		scene_config.reset();

		const Json::Value &config = (*it);

		scene_config.scene_id = config["sceneId"].asInt();

		if (config["scene_block"] != Json::Value::null) {
			if (config["scene_block"].size() != 2) {
				LOG_ABORT("scene:%d, error scene block", scene_config.scene_id);
			}
			scene_config.scene_block_x = config["scene_block"][0u].asInt();
			scene_config.scene_block_y = config["scene_block"][1u].asInt();
		}

		const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
		scene_config.order_total = server_solution_cache.fetch_order_total(Server_Enum::MONITOR_TYPE::MONITOR_SCENE, scene_config.scene_id);
		if (server_solution_cache.inter_server.count(scene_config.scene_id))
			scene_config.inter_order = CONFIG_CACHE->server_maintainer_cache().game_inter_flag % scene_config.order_total;

		scene_config.role_limit = config["role_limit"].asInt();
		scene_config.type = Scene_Type(config["type"].asInt());
		scene_config.manager = config["manager"].asInt();
		if (scene_config.manager == 0 || scene_config.manager >= Scene_Manager_Enum::Scene_Unknow) {
			LOG_ABORT("scene:%ld 未知manager:%d", scene_config.scene_id, scene_config.manager);
		}

		scene_config.map_id = config["mapId"].asInt();

		ABORT_INT_ZERO(scene_config.scene_id);
		ABORT_INT_ZERO(scene_config.map_id);

		if (scene_set.count(scene_config.scene_id))
			LOG_ABORT("场景id重复id:%d", scene_config.scene_id);

		scene_config.scene_name = config["defaultName"].asString();
		scene_config.login_create = config["login_create"].asBool();
		scene_config.timeLimit = config["timeLimit"].asInt();

		if (config["transmit_point"] != Json::Value::null) {
			Transmit_Point point;
			for (Json::Value::const_iterator it = config["transmit_point"].begin(); it != config["transmit_point"].end(); ++it) {
				Json_Utility::assign_coord((*it)["coord"], point.coord);
				point.scene_id = scene_config.scene_id;
				point.visible = (*it)["visible"].asInt();
				point.role_name = (*it)["name"].asString();
				point.des_scene_id = (*it)["des_scene_id"].asInt();
				point.role_id++;
				int map_value = get_map_value(scene_config.map_id, point.coord);
				if (map_value <= 0) {
					LOG_ABORT("传送点坐标非法map id:%d, x:%d, y:%d", scene_config.map_id, point.coord.x, point.coord.y);
				}

				scene_config.transmit_point.push_back(point);
				if ((*it)["client_find_path"].asInt())
					client_find_path_point.push_back(point);
			}
		}

		if (config["enter_point"] != Json::Value::null) {
			for (Json::Value::const_iterator it = config["enter_point"].begin(); it != config["enter_point"].end(); ++it) {
				Scene_Config::Transmit_Coord coord;
				Json_Utility::assign_coord((*it)["coord"], coord.coord);
				coord.src_scene_id = (*it)["src_scene_id"].asInt();
				int map_value = get_map_value(scene_config.map_id, coord.coord);
				if (map_value <= 0) {
					LOG_ABORT("进入点坐标非法map id:%d, x:%d, y:%d", scene_config.map_id, coord.coord.x, coord.coord.y);
				}

				scene_config.transmit_coord.insert(std::make_pair(coord.src_scene_id, coord));
			}
		}

		if (config["revive_point"] != Json::Value::null) {
			Coord coord;
			if (config["revive_point"].size() && config["revive_point"][0u].size() >= 2) {
				for (uint i = 0; i < config["revive_point"].size(); ++i) {
					Json_Utility::assign_coord(config["revive_point"][i], coord);
					int map_value = get_map_value(scene_config.map_id, coord);
					if (map_value <= 0) {
						LOG_ABORT("map id:%d, x:%d, y:%d", scene_config.map_id, coord.x, coord.y);
					}
					scene_config.revive_point.push_back(coord);
				}
			} else {
				Json_Utility::assign_coord(config["revive_point"], coord);
				int map_value = get_map_value(scene_config.map_id, coord);
				if (map_value <= 0) {
					LOG_ABORT("map id:%d, x:%d, y:%d", scene_config.map_id, coord.x, coord.y);
				}
				scene_config.revive_point.push_back(coord);
			}
		}

		if (config["des_scene_limit"] != Json::Value::null) {
			Json_Utility::assign_set_list(config["des_scene_limit"], scene_config.des_scene_limit);
		}

		if (config["scene_keep_time"] != Json::Value::null) {
			Json_Utility::assign_time_value(config["scene_keep_time"], scene_config.scene_keep_time);
		}

		scene_config.hero_disable = config["hero_disable"].asBool();

		//dungeon
		if(config["dungeon"] != Json::Value::null){
			const Json::Value &config_json = config["dungeon"];
			scene_config.dungeon.reset();
			if (config_json.isMember("id")) {
				scene_config.dungeon.id = config_json["id"].asInt();
			}
			if (config_json.isMember("name")) {
				scene_config.dungeon.name = config_json["name"].asString();
			}
			if (config_json.isMember("lv")) {
				scene_config.dungeon.lv = config_json["lv"].asInt();
			}
			if (config_json.isMember("difficulty")) {
				scene_config.dungeon.difficulty = config_json["difficulty"].asInt();
			}
			if(config_json.isMember("open_con")){
				for (Json::Value::iterator s_it = config_json["open_con"].begin();
					s_it != config_json["open_con"].end(); ++s_it) {
					scene_config.dungeon.open_con_map.insert(Scene_Config::Dungeon_Config_Detail::OpenConMap::value_type(
						(*s_it)[0u].asInt(), (*s_it)[1].asInt()));
				}
			}
			if (config_json.isMember("pass_con")) {
				scene_config.dungeon.pass_con = config_json["pass_con"].asInt();
			}
			if(config_json.isMember("cost")){
				for (Json::Value::iterator s_it = config_json["cost"].begin();
						s_it != config_json["cost"].end(); ++s_it) {
					scene_config.dungeon.cost_map.insert(Scene_Config::Dungeon_Config_Detail::CostMap::value_type(
							(*s_it)[0u].asInt(), (*s_it)[1].asInt()));
				}
			}
			if (config_json.isMember("entrance")) {
				scene_config.dungeon.be_enter = config_json["entrance"].asInt();
			}
			if (config_json.isMember("be_mop")) {
				scene_config.dungeon.be_mop = config_json["be_mop"].asInt();
			}
			if (config_json.isMember("can_team")) {
				scene_config.dungeon.can_team = config_json["can_team"].asInt();
			}
			if(config_json.isMember("task")){
				scene_config.dungeon.require_task = config_json["task"].asInt();
			}
			if (config_json.isMember("team_lim")) {
				scene_config.dungeon.team_lim = config_json["team_lim"].asInt();
			}
			if (config_json.isMember("relation")) {
				scene_config.dungeon.relation = config_json["relation"].asInt();
			}
			if (config_json.isMember("next_id")) {
				scene_config.dungeon.next_id = config_json["next_id"].asInt();
			}
			if (config_json.isMember("elite")) {
				scene_config.dungeon.elite = config_json["elite"].asInt();

			}
			if (config_json.isMember("vip_exp")) {
				int vip = 1;
				Scene_Config::Dungeon_Config_Detail::VipExtMap vipmap;
				for (Json::Value::iterator s_it = config_json["vip_exp"].begin();
						s_it != config_json["vip_exp"].end(); ++s_it) {
					vipmap.insert(Scene_Config::Dungeon_Config_Detail::VipExtMap::value_type(vip, (*s_it).asInt()));
					++vip;
				}
				scene_config.dungeon.vip_ext_map = vipmap;
			}

			insert_dungeon_relation(relation_map, scene_config.dungeon.relation, scene_config.dungeon.id);
			scene_config.dungeon.first_layer_id = get_dungeon_first_layer_id(relation_map,
					scene_config.dungeon.id);


			// 单人副本和精英副本对应关系
			if(scene_config.dungeon.elite != 0){
				if(scene_config.dungeon.difficulty ==
						Scene_Config::Dungeon_Config_Detail::DUNGEON_DIFF_NOR){
					single_elite_nor_map_.insert(std::make_pair(scene_config.dungeon.elite, scene_config.dungeon.id));
					elite_single_map_.insert(std::make_pair(scene_config.dungeon.id,scene_config.dungeon.elite));
				}else if(scene_config.dungeon.difficulty ==
						Scene_Config::Dungeon_Config_Detail::DUNGEON_DIFF_HOR){
					single_elite_hor_map_.insert(std::make_pair(scene_config.dungeon.elite, scene_config.dungeon.id));
					elite_single_map_.insert(std::make_pair(scene_config.dungeon.id,scene_config.dungeon.elite));
				}
			}

		}
		scene_set.insert(scene_config.scene_id);
		set_map_second_value_by_key(scene_config.scene_id, scene_config_cache_map_, scene_config);
	}

	client_find_path_point_.reset();
	client_find_path_point_.make_message(81000041);
	uint16_t __transmit_point_vec_size = client_find_path_point.size();
	client_find_path_point_.write_uint16(__transmit_point_vec_size);
	for(uint16_t i = 0; i < __transmit_point_vec_size; ++i) {
		client_find_path_point[i].serialize(client_find_path_point_);
	}
	client_find_path_point_.finish_message();
}

void Config_Cache_Scene::insert_dungeon_relation(Relation_Map& relation_map, int value1, int value2){
	if(value1 == 0 || value1 == -1 || value2 == 0 || value2 == -1){
		return;
	}
	int base_value = get_dungeon_first_layer_id(relation_map, value2);
	if(base_value == 0){
		relation_map.insert(std::make_pair(value1, value2));
		return;
	}else{
		return insert_dungeon_relation(relation_map, value1, base_value);
	}
}
int Config_Cache_Scene::get_dungeon_first_layer_id(Relation_Map& relation_map,int value){
	Relation_Map::iterator it = relation_map.find(value);
	if(it != relation_map.end()){
		return it->second;
	}
	return 0;
}

int Config_Cache_Scene::get_elite_nor_from_single(int scene_id){
	Relation_Map::const_iterator it = single_elite_nor_map_.find(scene_id);
	if(it != single_elite_nor_map_.end()){
		return it->second;
	}
	return 0;
}


int Config_Cache_Scene::get_elite_hor_from_single(int scene_id){
	Relation_Map::const_iterator it = single_elite_hor_map_.find(scene_id);
	if(it != single_elite_hor_map_.end()){
		return it->second;
	}
	return 0;
}


int Config_Cache_Scene::get_single_from_elite(int scene_id){
	Relation_Map::const_iterator it = elite_single_map_.find(scene_id);
	if(it != elite_single_map_.end()){
		return it->second;
	}
	return 0;
}

int Config_Cache_Scene::get_single_pre_single(int scene_id){
	Scene_Config_Map_Cache::iterator it;
	for(it = scene_config_cache_map_.begin(); it != scene_config_cache_map_.end(); ++it){
		if(it->second.dungeon.next_id == scene_id){
			return it->second.dungeon.id;
		}
	}
	return 0;
}

int Config_Cache_Scene::get_map_id_from_scene_id(int scene_id){
	const Scene_Config* cfg = scene_config_cache(scene_id);
	if(cfg){
		return cfg->map_id;
	}
	return 0;
}

void Config_Cache_Scene::update_single_dungeon_relation(void){
	int scene_index = 0;
	int scene_id = 0;

	// 单人副本
	single_id_index_map_.clear();
	single_dungeon_ids_.clear();
	scene_index = 0;
	scene_id = CONFIG_CACHE_ROLE->player_init_cache().first_single_id;
	while (scene_id != 0) {
		scene_index += 1;
		std::pair<Relation_Map::iterator,bool> ret = single_id_index_map_.insert(std::make_pair(scene_id, scene_index));
		if(ret.second == false){//
			LOG_ABORT("scene config single dungeon next_id error :%d", scene_id);
		}
		single_dungeon_ids_.push_back(scene_id);
		const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
		if (!cfg) {
			break;
		}
		scene_id = cfg->dungeon.next_id;
	}
	// 上古副本
	elite_id_index_map_.clear();
	elite_dungeon_ids_.clear();
	scene_index = 0;
	scene_id = CONFIG_CACHE_ROLE->player_init_cache().first_nor_elite_id;
	while (scene_id != 0) {
		scene_index += 1;
		std::pair<Relation_Map::iterator,bool> ret = elite_id_index_map_.insert(std::make_pair(scene_id, scene_index));
		if(ret.second == false){//
			LOG_ABORT("scene config elite dungeon next_id error :%d", scene_id);
		}
		elite_dungeon_ids_.push_back(scene_id);
		const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
		if (!cfg) {
			break;
		}
		scene_id = cfg->dungeon.next_id;
	}

	// 魔神殿副本
	lord_id_index_map_.clear();
	lord_dungeon_ids_.clear();
	scene_index = 0;
	scene_id = CONFIG_CACHE_ROLE->player_init_cache().first_hor_elite_id;
	while (scene_id != 0) {
		scene_index += 1;
		std::pair<Relation_Map::iterator,bool> ret = lord_id_index_map_.insert(std::make_pair(scene_id, scene_index));
		if(ret.second == false){//
			LOG_ABORT("scene config lord temple dungeon next_id error :%d", scene_id);
		}
		lord_dungeon_ids_.push_back(scene_id);
		const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
		if (!cfg) {
			break;
		}
		scene_id = cfg->dungeon.next_id;
	}
}

int Config_Cache_Scene::get_single_dungeon_relation_index(const int scene_id){
	Relation_Map::iterator it = single_id_index_map_.find(scene_id);
	if(it != single_id_index_map_.end()){
		return it->second;
	}
	return 0;
}

Int_Vec& Config_Cache_Scene::get_all_single_dungeon_id(void){
	return single_dungeon_ids_;
}

int Config_Cache_Scene::get_elite_dungeon_relation_index(const int scene_id, const int diff){
	if(diff == 1){
		Relation_Map::iterator it = elite_id_index_map_.find(scene_id);
		if(it != elite_id_index_map_.end()){
			return it->second;
		}
		return 0;
	}else{
		Relation_Map::iterator it = lord_id_index_map_.find(scene_id);
		if(it != lord_id_index_map_.end()){
			return it->second;
		}
		return 0;
	}

}

Int_Vec& Config_Cache_Scene::get_all_elite_dungeon_id(const int diff){
	if(diff == 1){
		return elite_dungeon_ids_;
	}else{
		return lord_dungeon_ids_;
	}

}
