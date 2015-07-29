/*
 * Config_Cache_Team.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_TEAM_H_
#define CONFIG_CACHE_TEAM_H_

#include "Cache_Base.h"
#include "team/Team_Config.h"

class Config_Cache_Team : public Cache_Base {
public:

	// == task config ========
	const Team_Config &team_config(void) const;
	const int* find_scene(int8_t type) const;
	Arena_Multi_Detail *arena_multi_cfg_win(const int32_t &level);
	Arena_Multi_Detail *arena_multi_cfg_fail(const int32_t &level);
	const Arena_Multi_Checkout_Detail *arena_multi_cfg_checkout(const int32_t &score);
	const Arena_Multi_Checkout_Detail *arena_multi_cfg_level(const int32_t &level);
	int arena_multi_level(const int32_t &score);
	const Team_Robot_Config_Map &team_robot_cfg(void);
	const Team_Defend_Cfg_Detail *find_team_defend(const int scene_id) const;
	int get_correct_by_gap_and_victory(const int gap, const int victory) const;

	void refresh_config_cache(void);

private:
	Team_Config team_config_;
	Level_ArenaMultiDetail_Map arena_multi_cfg_win_;
	Level_ArenaMultiDetail_Map arena_multi_cfg_fail_;
	Index_ArenaMultiCheckout_Map arena_multi_cfg_checkout_;
	Team_Robot_Config_Map team_robot_cfg_;
	Team_Defend_Cfg_Map team_defend_cfg_;
	Team_Arena_Correct_Vec team_arena_correct_cfg_;
};
typedef Singleton<Config_Cache_Team> Config_Cache_Team_Single;
#define CONFIG_CACHE_TEAM    (Config_Cache_Team_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_TEAM_H_ */
