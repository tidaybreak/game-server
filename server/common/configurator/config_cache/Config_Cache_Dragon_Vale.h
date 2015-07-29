/*
 * Config_Cache_Dragon_Vale_Building.h
 *
 *  Created on: 2014年7月10日
 *      Author: gaojianshan
 */

#ifndef CONFIG_CACHE_DRAGON_BUILDING_H_
#define CONFIG_CACHE_DRAGON_BUILDING_H_

#include "Cache_Base.h"
#include "dragon/Dragon_Vale_Building_Config.h"
#include "dragon/Dragon_Vale_Dragon_Config.h"
#include "dragon/Dragon_Vale_Main_Config.h"

class Config_Cache_Dragon_Vale : public Cache_Base {
public:
	const Dragon_Building_Cfg& get_buidling_config() const;
	const Dragon_All_Cfg& get_dragon_config() const;
	const Dragon_Vale_Main_Cfg& get_main_config() const;

	const Building_Level_Cfg* find_Building_Level_Cfg(uint8_t building_type, uint16_t building_level);
	const Role_Level_Cfg* find_Building_Role_Level_Cfg(uint32_t player_level, uint8_t building_type);
	const Vip_Level_Cfg* find_Building_Vip_Level_Cfg(uint32_t vip_level);
	const Building_Queue_Cfg* find_Building_Queue_Cfg(uint8_t building_order);
	const Quick_Upgrade_Cfg* find_Building_Quick_Upgrade_Cfg(uint32_t upgrade_left_time);
	const Create_Building_Cfg* find_Building_Create_Cfg(uint8_t order);
	const Dragon_Base_Cfg& find_Dragon_Base_Cfg();
	const Dragon_Role_Level_Cfg* find_Dragon_Role_Level_Cfg(uint32_t player_level, uint8_t dragon_keep_type);
	const Dragon_Keep_Cfg* find_Dragon_Keep_Cfg(uint8_t dragon_keep_type, uint32_t dragon_level);
	const Dragon_Evolution_Cfg* find_Dragon_Evolution_Cfg(uint8_t progress);
	const Dragon_Play_Dragon_Cfg* find_Dragon_Play_Dragon_Cfg(uint32_t player_level);
	const Dragon_Feeling_Cfg* find_Dragon_Feeling_Cfg(uint8_t dragon_feeling);
	const Play_Dragon_Get_Heart_Cfg* find_Dragon_Play_Dragon_Get_Heart_Cfg(uint16_t heart_num);
	const Main_Base_Cfg& find_main_base_cfg();
	const uint8_t* find_Main_Star_Up_Spped(uint8_t star);
	const uint32_t* find_main_match_level(uint32_t begin_level);
	const uint32_t* find_main_foot_award(uint32_t player_level);
	const uint32_t* find_main_thief_award(uint32_t player_level, uint8_t thief_quality);
	const uint32_t* find_main_match_coin(uint32_t match_num);

	void add_Building_Level_Cfg_to_map(uint8_t building_type, uint16_t building_level, const Building_Level_Cfg &cfg);
	void add_Building_Role_Level_Cfg_to_map(uint32_t player_level, uint8_t building_type, const Role_Level_Cfg &cfg);
	void add_Building_Vip_Level_Cfg_to_map(uint32_t vip_level, const Vip_Level_Cfg &cfg);
	void add_Building_Queue_Cfg_to_map(uint8_t building_order, const Building_Queue_Cfg &cfg);
	void add_Building_Quick_Upgrade_Cfg_to_map(uint32_t upgrade_left_time, const Quick_Upgrade_Cfg  &cfg);
	void add_Building_Create_Cfg_to_map(uint8_t order, const Create_Building_Cfg &cfg);
	void add_Dragon_Role_Level_Cfg_to_map(uint32_t player_level, uint8_t dragon_keep_type,
					const Dragon_Role_Level_Cfg &cfg);
	void add_Dragon_Keep_Cfg_to_map(uint8_t dragon_keep_type, uint32_t dragon_level, const Dragon_Keep_Cfg &cfg);
	void add_Dragon_Feeling_Cfg_to_map(uint8_t dragon_feeling, const Dragon_Feeling_Cfg &cfg);
	void add_Dragon_Evolution_Cfg_to_map(uint8_t progress, const Dragon_Evolution_Cfg &cfg);
	void add_Dragon_Play_Dragon_Cfg_to_map(uint32_t player_level, const Dragon_Play_Dragon_Cfg &cfg);
	void add_Dragon_Play_Dragon_Get_Heart_Cfg_to_map(uint16_t heart_num, const Play_Dragon_Get_Heart_Cfg &cfg);
	void add_Main_Star_Up_Speed_to_map(uint8_t star, uint8_t percent);
	void add_Main_Match_Level_to_map(uint32_t begin_level, uint32_t end_level);
	void add_Main_Foot_Award_to_map(uint32_t player_level, uint32_t dragon_soul_award);
	void add_main_thief_award_to_map(uint32_t player_level, uint8_t thief_quality, uint32_t award);
	void add_main_match_coin_to_map(uint32_t match_num, uint32_t coin);

	void refresh_config_cache();
private:
	void refresh_config_cache_Building_Level_Cfg();
	void refresh_config_cache_Building_Role_Level_Cfg();
	void refresh_config_cache_Building_Vip_Level_Cfg();
	void refresh_config_cache_Building_Queue_Cfg();
	void refresh_config_cache_Building_Quick_Upgrade_Cfg();
	void refresh_config_cache_Building_Create_Cfg();
	void refresh_config_cache_Dragon_Base_Cfg();
	void refresh_config_cache_Dragon_Role_Level_Cfg();
	void refresh_config_cache_Dragon_Keep_Cfg();
	void refresh_config_cache_Dragon_Feeling_Cfg();
	void refresh_config_cache_Dragon_Evolution_Cfg();
	void refresh_config_cache_Dragon_Play_Dragon_Cfg();
	void refresh_config_cache_Dragon_Play_Dragon_Get_Heart_Cfg();
	void refresh_config_cache_Main_Base_Cfg();
	void refresh_config_cache_Main_Star_Up_Speed();
	void refresh_config_cache_Main_Match_Level();
	void refresh_config_cache_Main_Foot_Award();
	void refresh_config_cache_Main_Thief_Award();
	void refresh_config_cache_Main_Thief_Id();
	void refresh_config_cache_Main_Match_Coin();
private:
	Dragon_Building_Cfg dragon_building_cfg;
	Dragon_All_Cfg dragon_all_cfg;
	Dragon_Vale_Main_Cfg dragon_vale_main_cfg;
};

typedef Singleton<Config_Cache_Dragon_Vale> Config_Cache_Dragon_Vale_Single;
#define CONFIG_CACHE_DRAGON_VALE   (Config_Cache_Dragon_Vale_Single::instance())

#endif /* CONFIG_CACHE_DRAGON_BUILDING_H_ */
