/*
 * Config_Cache_Daily.h
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_DAILY_H_
#define CONFIG_CACHE_DAILY_H_

#include "Cache_Base.h"
#include "helper/Helper_Config.h"

class Config_Cache_Daily : public Cache_Base {
public:

	const uint32_t get_activity_group_id(void);

	const Daily_Conf_Map &daily_config_map(void) const;
	const Conf_Key_Map &daily_config_key_map(void) const;

	const Activity_Conf_Map &activity_config_map(void) const;
	const Conf_Key_Map &activity_config_key_map(void) const;

	const Activity_Award_Map &today_act_award_map(void);
	const Activity_Award_Map &all_act_award_map(void) const;

	const int get_force_by_elite_explore_id(const int force) const;
	const Check_In_Conf_Map &get_check_in_map_by_group_id(const uint32_t group_id) const;

	const uint32_t get_can_equip_by_level_index(const uint32_t level, const uint32_t index) const;

	void refresh_config_cache(void);
	void refresh_daily_config_cache(void);
	void refresh_activity_config_cache(void);
	void refresh_today_activity_award_config_cache(void);
	void refresh_all_activity_award_config_cache(void);
	void refresh_all_elite_explore_config_cache(void);
	void refresh_all_check_in_config_cache(void);
	void refresh_can_equip_lv_map_config_cache(void);


private:

	Daily_Conf_Map daily_config_map_;
	Conf_Key_Map daily_config_key_map_;

	Activity_Conf_Map activity_config_map_;
	Conf_Key_Map activity_config_key_map_;

//	Activity_Award_Map today_act_award_map_;
	Activity_Award_Group_Conf_Map today_act_award_map_;
	Activity_Group_Conf_Map today_act_group_map_;
	uint32_t today_act_group_max_end;
	Activity_Award_Map all_act_award_map_;

	Int_Int_Map elite_explore_map_;

	Check_In_Group_Conf_Map check_in_group_map_;

	UInt_UIntUIntMap_Map can_equip_lv_map_;
};
typedef Singleton<Config_Cache_Daily> Config_Cache_Daily_Single;
#define CONFIG_CACHE_DAILY    (Config_Cache_Daily_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_DAILY_H_ */
