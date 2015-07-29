/*
 * Config_Cache_Activity.h
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_ACTIVITY_H_
#define CONFIG_CACHE_ACTIVITY_H_

#include <activity/Activity_Config.h>
#include "Cache_Base.h"

class Config_Cache_Activity : public Cache_Base {
public:

	const Vip37_Conf_Map &get_vip_config(void);

	void refresh_config_cache(void);
	void refreshvip_config_cache(void);
	void refreshphone_config_cache(void);

	void refresh_active_reward_info_config(const Json::Value& item_conf, Active_Reward_Info_Config &info);

private:

	Vip37_Conf_Map vip37_conf;
	Active_Reward_Info_Config phone37_reward;

};
typedef Singleton<Config_Cache_Activity> Config_Cache_Activity_Single;
#define CONFIG_CACHE_ACTIVITY    (Config_Cache_Activity_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ACTIVITY_H_ */
