/*
 * Config_Cache_Task.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_TASK_H_
#define CONFIG_CACHE_TASK_H_

#include "Cache_Base.h"
#include "task/Task_Config.h"
#include "task/Day_Week_Task_Config.h"
#include "task/Safe_Guide_Config.h"
#include "task/Ring_Task.h"

class Config_Cache_Task : public Cache_Base {
public:

	// == task config ========
	const Task_Config &get_task_config(void) const;
	const Task_Mission_Detail_Config *find_task(int task_id) const;
//	const Task_Config::IntSet *npc_task_list(int npc_id) const;

	const Day_Week_Task_Config& get_day_task_config() const;
	const Day_Group_Set* find_day_group_set(uint8_t time_hour) const;
	const Week_Group_Set* find_week_group_set(uint8_t wday, uint8_t time_hour) const;
	const Day_Week_Task_Award_Cfg* find_day_award_cfg(uint32_t role_level);
	void add_day_task_award_cfg_to_map(uint32_t role_level, const Day_Week_Task_Award_Cfg &cfg);
	const Day_Week_Task_Award_Cfg* find_week_award_cfg(uint32_t role_level);
	void add_week_task_award_cfg_to_map(uint32_t role_level, const Day_Week_Task_Award_Cfg &cfg);

	Day_Group_Set* find_day_group_set_no_const(uint8_t time_hour);
	Week_Group_Set* find_week_group_set_no_const(uint8_t wday, uint8_t time_hour);

	const Safe_Guide_Config_Map& get_safe_guide_config_map();
	void add_safe_guide_config_to_map(uint64_t safe_id, const Safe_Guide_Config &cfg);

	const Ring_Task_All_Cfg& get_ring_task_all_cfg() const;

	void refresh_config_cache(void);
	void refresh_safe_guide_config();
	void check_task_cfg();

private:
	void refresh_day_task_config();
	void refresh_week_task_config();
	void refresh_day_task_award_config();
	void refresh_week_task_award_config();
	void refresh_ring_task_base_cfg();
	void refresh_ring_task_ring_award_cfg();
	void refresh_ring_task_player_level_award_cfg();
	void refresh_ring_task_ring_num_type_cfg();
	void check_ring_task_cfg();
	void check_reward_task_cfg();
	void refresh_ring_monster();
	void refresh_level_monster();

private:
	Task_Config task_config_;
	Day_Week_Task_Config day_week_task_config_;
	Safe_Guide_Config_Map safe_guide_config_map_;
	Ring_Task_All_Cfg ring_task_all_cfg_;
};

typedef Singleton<Config_Cache_Task> Config_Cache_Task_Single;
#define CONFIG_CACHE_TASK    (Config_Cache_Task_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_TASK_H_ */
