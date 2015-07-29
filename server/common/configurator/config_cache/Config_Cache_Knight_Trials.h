/*
 * Config_Cache_Knight_Trials.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_KNIGHT_TRIALS_H_
#define CONFIG_CACHE_KNIGHT_TRIALS_H_

#include "Cache_Base.h"
#include "knight_trials/Knight_Trials_Config.h"


class Config_Cache_Knight_Trials : public Cache_Base {
public:
	const Knight_Trials_Points_Conf_Map &get_points_conf(void);
	const Knight_Trials_Level_Conf *get_level_conf_by_level(const uint32_t level);
	const Knight_Trials_Points_Conf *get_points_conf_by_points(const uint8_t points);
	const uint32_t get_exp_buy_level_points(uint32_t level, uint8_t points);
	const Knight_Trials_Box_Reward_Conf_List *get_box_reward_list_by_type(uint32_t type);

	void refresh_config_cache(void);
	void refresh_knight_trials_points_conf_cache(void);
	void refresh_knight_trials_level_conf_cache(void);
	void refresh_exp_reward_conf_cache(void);
	void refresh_box_reward_conf_cache(void);

private:
	Knight_Trials_Points_Conf_Map knight_trials_points_conf_map_;
	Knight_Trials_Level_Conf_List knight_trials_level_conf_list_;
	Knight_Trials_Exp_Reward_Conf_Map exp_reward_conf_map;
	Knight_Trials_Box_Reward_Conf_Map box_reward_conf_map;
};
typedef Singleton<Config_Cache_Knight_Trials> Config_Cache_Knight_Trials_Single;
#define CONFIG_CACHE_KNIGHT_TRIALS    (Config_Cache_Knight_Trials_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_KNIGHT_TRIALS_H_ */
