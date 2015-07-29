/*
 * Config_Cache_Reward_Task.h
 *
 *  Created on: Jun 25, 2014
 *      Author: gaojianshan
 */

#ifndef CONFIG_CACHE_REWARD_TASK_H_
#define CONFIG_CACHE_REWARD_TASK_H_

#include "Cache_Base.h"
#include "task/Reward_Task_Config.h"

class Config_Cache_Reward_Task : public Cache_Base {
public:
	const Reward_Task_Config& get_config() const;
	Reward_Task_Config& get_config_no_const();
	void refresh_config_cache();


private:
	Reward_Task_Config reward_task_cfg;
};

typedef Singleton<Config_Cache_Reward_Task> Config_Cache_Reward_Task_Single;
#define CONFIG_CACHE_REWARD_TASK    (Config_Cache_Reward_Task_Single::instance())



#endif /* CONFIG_CACHE_REWARD_TASK_H_ */
