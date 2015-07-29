/*
 * Config_Cache_Covenant.h
 *
 *  Created on: 2014-07-15
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_COVENANT_H_
#define CONFIG_CACHE_COVENANT_H_

#include "Cache_Base.h"
#include "covenant/Covenant_Config.h"

class Config_Cache_Covenant : public Cache_Base {
public:
	const Covenant_Conf *find_covenant_conf_by_id(uint32_t id);
	const Covenant_Conf_Map &covenant_conf_map(void);

	void refresh_config_cache(void);

private:
	Covenant_Conf_Map covenant_conf_map_;
};
typedef Singleton<Config_Cache_Covenant> Config_Cache_Covenant_Single;
#define CONFIG_CACHE_COVENANT    (Config_Cache_Covenant_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_COVENANT_H_ */
