/*
 * Config_Cache_World_Boss.h
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#ifndef CONFIG_CACHE_WORLD_BOSS_H_
#define CONFIG_CACHE_WORLD_BOSS_H_

#include "Cache_Base.h"
#include "world_boss/World_Boss_Config.h"


class Config_Cache_World_Boss : public Cache_Base {
public:
	void refresh_config_cache(void);
private:
	void refresh_base(void);
	void refresh_level(void);
	void refresh_reward(void);

	void refresh_base_gang(void);
	void refresh_level_gang(void);
	void refresh_reward_gang(void);
public:
	const World_Boss_Level_Config* get_level_config_from_lv(int lv, const int8_t icon_id);
	int get_boss_lv_from_kill_num(const int kill_num, const int8_t icon_id);
	int get_boss_kill_num_from_lv(const int lv, const int8_t icon_id);
	int get_boss_rapid_kill_time(const int8_t icon_id);
	int get_boss_max_lv(const int8_t icon_id);
	const World_Boss_Reward_Config* get_reward_config(int boss_lv, int rank, const int8_t icon_id);
	World_Boss_Base_Config* get_base_config(const int8_t icon_id);
	int cal_active_time(const int cur_time, int& valid_status, int& valid_time, int& valid_last_time, bool& next_day);
	int get_ability0_num(const int vip_lv, const int8_t icon_id);
	int get_ability1_num(const int vip_lv, const int8_t icon_id);
	int get_ability2_num(const int vip_lv, const int8_t icon_id);
	int get_ability3_num(const int vip_lv, const int8_t icon_id);
	int get_ability0_cost(const int use_num, const int8_t icon_id);
	int get_ability1_cost(const int use_num, const int8_t icon_id);
	int get_ability2_cost(const int use_num, const int8_t icon_id);
	int get_ability3_cost(const int use_num, const int8_t icon_id);
	int get_ability1_cost_type(const int8_t icon_id);
	int get_ability2_cost_type(const int8_t icon_id);
	int get_ability3_cost_type(const int8_t icon_id);
	int achieve_1_coe(const int boss_lv, const int8_t icon_id);
	int achieve_2_coe(const int boss_lv, const int8_t icon_id);
	int achieve_3_coe(const int boss_lv, const int8_t icon_id);
	int get_ability3_coe(const int use_num, const int8_t icon_id);
	int get_revive_time(const int8_t icon_id);
	double leader_hurt_coe(const int8_t icon_id);
	int get_hurt_reward_coe(const int boss_cfg_lv, const int8_t icon_id);
	bool get_boss_weak_check(const int8_t icon_id);
	int get_boss_weak_1(const int8_t icon_id);
	int get_boss_weak_2(const int8_t icon_id);
	int get_boss_weak_3(const int8_t icon_id);
	int get_enter_lv_limit(const int8_t icon_id);
private:
	World_Boss_Base_Config cfg_base_;
	World_Boss_Level_Config_Map cfg_level_map_;
	World_Boss_Reward_Config_Map cfg_reward_map_;

	World_Boss_Base_Config gang_cfg_base_;
	World_Boss_Level_Config_Map gang_cfg_level_map_;
	World_Boss_Reward_Config_Map gang_cfg_reward_map_;
};

typedef Singleton<Config_Cache_World_Boss> Config_Cache_World_Boss_Single;
#define CONFIG_CACHE_WORLD_BOSS (Config_Cache_World_Boss_Single::instance())

#endif /* CONFIG_CACHE_WORLD_BOSS_H_ */
