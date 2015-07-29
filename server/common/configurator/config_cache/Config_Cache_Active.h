/*
 * Config_Cache_Active.h
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_ACTIVE_H_
#define CONFIG_CACHE_ACTIVE_H_

#include "Cache_Base.h"
#include "active/Active_Config.h"

class Config_Cache_Active : public Cache_Base {
public:

	const Active_Conf_Map &act_conf_map(void) const;
	void refresh_config_cache(void);

private:
	Active_Conf_Map act_conf_map_;
};
typedef Singleton<Config_Cache_Active> Config_Cache_Active_Single;
#define CONFIG_CACHE_ACTIVE    (Config_Cache_Active_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ACTIVE_H_ */
