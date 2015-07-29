/*
 * Config_Cache_Icon.h
 *
 *  Created on: 2014年5月22日
 *      Author: gaojianshan
 */

#ifndef CONFIG_CACHE_ICON_H_
#define CONFIG_CACHE_ICON_H_

#include "Cache_Base.h"
#include "icon/Icon_Config.h"

class Config_Cache_Icon : public Cache_Base {
public:

	const Icon_Config_Map& icon_config(void) const;
	const Icon_Base_Config* find_config(uint16_t id) const;
	void refresh_config_cache(void);

private:
	Icon_Config_Map icon_config_map;

};
typedef Singleton<Config_Cache_Icon> Config_Cache_Icon_Single;
#define CONFIG_CACHE_ICON    (Config_Cache_Icon_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ICON_H_ */
