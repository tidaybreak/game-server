/*
 * Config_Cache_Scene.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_SCENE_H_
#define CONFIG_CACHE_SCENE_H_

#include "Cache_Base.h"
#include "scene/Scene_Struct.h"

class Config_Cache_Scene : public Cache_Base {
	typedef boost::unordered_map<int, int> Relation_Map;
public:

	int get_map_value(int map_id, Coord &coord);
	int map_value_by_grid_coord(int map_id, uint16_t x, uint16_t y);
	int map_value_by_pixel_coord(int map_id, uint16_t x, uint16_t y);

	// == scene config ========
	const Scene_Config_Map_Cache &scene_config_cache_map(void) const;
	const Scene_Config *scene_config_cache(int scene_id) const;
	const Scene_Map_Data *scene_map_data_cache(int id) const;

	Block_Buffer &client_find_path_point(void);

	void refresh_config_cache(std::string &module);
	void refresh_scene_config_cache(void);
	void refresh_scene_map_cache(std::string &module);

	//
	void insert_dungeon_relation(Relation_Map& relation_map, int value1, int value2);
	int get_dungeon_first_layer_id(Relation_Map& relation_map, int value);

	//	通过单人副本获取普通精英副本id
	int get_elite_nor_from_single(int scene_id);
	// 通过单人副本获取噩梦精英副本id
	int get_elite_hor_from_single(int scene_id);
	// 通过精英副本获取单人副本id
	int get_single_from_elite(int scene_id);
	// 获取单人副本前一个副本id
	int get_single_pre_single(int scene_id);
	// 获取地图id
	int get_map_id_from_scene_id(int scene_id);
	// 更新单人副本层次关系
	void update_single_dungeon_relation(void);
	//
	int get_single_dungeon_relation_index(const int scene_id);

	Int_Vec& get_all_single_dungeon_id(void);

	int get_elite_dungeon_relation_index(const int scene_id, const int diff);

	Int_Vec& get_all_elite_dungeon_id(const int diff);
private:
	Scene_Map_Data_Map_Cache scene_map_data_cache_map_;		// 地图id -> 地图数据
	Scene_Config_Map_Cache scene_config_cache_map_;			// 场景id -> 场景配制
	Block_Buffer client_find_path_point_;					// 客户端普通场景寻路信息

	Relation_Map single_elite_nor_map_;						// 单人--普通精英关联表
	Relation_Map single_elite_hor_map_;						// 单人--噩梦精英关联表
	Relation_Map elite_single_map_;						   // 精英--普通关联表

	Relation_Map single_id_index_map_;						// 单人副本对应第几个副本，例如 80201001是第1个 80201002是第2个
	Int_Vec		 single_dungeon_ids_;						// 所有单人副本，按顺序的

	Relation_Map elite_id_index_map_;						// 上古副本对应第几个副本，例如 80201001是第1个 80201002是第2个
	Int_Vec		 elite_dungeon_ids_;						   // 所有上古副本，按顺序的

	Relation_Map lord_id_index_map_;							// 魔神殿副本对应第几个副本，例如 80201001是第1个 80201002是第2个
	Int_Vec		 lord_dungeon_ids_;						   // 所有魔神殿副本，按顺序的

};
typedef Singleton<Config_Cache_Scene> Config_Cache_Scene_Single;
#define CONFIG_CACHE_SCENE    (Config_Cache_Scene_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_SCENE_H_ */
