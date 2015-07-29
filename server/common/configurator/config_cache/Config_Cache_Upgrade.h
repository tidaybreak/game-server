/*
 * Config_Cache_Upgrade.h
 *
 *  Created on: 2014-07-07
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_UPGRADE_H_
#define CONFIG_CACHE_UPGRADE_H_

#include "Cache_Base.h"
#include "helper/Helper_Config.h"

class Config_Cache_Upgrade : public Cache_Base {
public:

	const Upgrade_Conf_Map &upgrade_conf_map(void) const;
	void refresh_config_cache(void);

private:
	Upgrade_Conf_Map upgrade_conf_map_;
};
typedef Singleton<Config_Cache_Upgrade> Config_Cache_Upgrade_Single;
#define CONFIG_CACHE_UPGRADE    (Config_Cache_Upgrade_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_UPGRADE_H_ */
