/*
 * Config_Cache_Vip.h
 *
 *  Created on: 2014-07-15
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_VIP_H_
#define CONFIG_CACHE_VIP_H_

#include "Cache_Base.h"
#include "vip/Vip_Config.h"

class Config_Cache_Vip : public Cache_Base {
public:
	const World_Tree_Config *get_world_tree_config(uint32_t id);
	const Grow_Get_Config *get_grow_get_config(uint32_t vip_level);
	const Vip_Open_Config *get_vip_open_config(uint32_t id);
	const Turntable_Item_Config *get_turntable_item_config(uint32_t id);
	const Turntable_Cost_Config *get_turntable_cost_config(uint8_t num);
	const UInt_Vec *get_vip_addition_config(uint32_t type);
	const Vip_Daily_Item_Config_Vec *get_vip_daily_config(uint32_t vip_level);

	Vip_Open_Conf_Map &vip_open_conf_map(void);
	Grow_Get_Conf_Map &grow_get_conf_map(void);
	World_Tree_Conf_Map &world_tree_conf_map(void);
	Turntable_Item_Conf_Map &turntable_item_conf_map(void);
	Turntable_Config_Vec &turntable_conf_vec(void);
	const Turntable_Item_Config_Vec *turntable_item_conf_vec_by_type(uint8_t type);

	void refresh_config_cache(void);

	void refresh_vip_open_config_cache(void);
	void refresh_grow_get_config_cache(void);
	void refresh_world_tree_config_cache(void);
	void refresh_turntable_config_cache(void);
	void refresh_turntable_item_config_cache(void);
	void refresh_turntable_cost_config_cache(void);
	void refresh_vip_addition_config_cache(void);
	void refresh_vip_daily_item_config_cache(void);

private:

	void refresh_turntable_type_tiem_conf_map_(uint8_t type, Turntable_Item_Config &item_config);

	Vip_Open_Conf_Map vip_open_conf_map_;
	Grow_Get_Conf_Map grow_get_conf_map_;
	World_Tree_Conf_Map world_tree_conf_map_;
	Turntable_Item_Conf_Map turntable_item_conf_map_;
	Turntable_Type_Item_Conf_Map turntable_type_item_conf_map_;
	Turntable_Config_Vec turntable_conf_vec_;
	Turntable_Cost_Conf_Map turntable_cost_conf_map_;
	UInt_UIntVec_Map vip_addition_map_;
	Vip_Daily_Item_Config_Vec_Map vip_daily_item_conf_vec_map;
};
typedef Singleton<Config_Cache_Vip> Config_Cache_Vip_Single;
#define CONFIG_CACHE_VIP    (Config_Cache_Vip_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_VIP_H_ */
