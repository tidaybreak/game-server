/*
 * Config_Cache_Flowers.h
 *
 *  Created on: 2014-07-15
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_FLOWERS_H_
#define CONFIG_CACHE_FLOWERS_H_

#include "Cache_Base.h"
#include "public/Public_Config.h"

class Config_Cache_Flowers : public Cache_Base {
public:
	const Flowers_Config *get_flowers_conf_by_item_id(uint32_t item_id);

	void refresh_config_cache(void);

private:
	Flowers_Config_Map flowers_conf_map_;
};
typedef Singleton<Config_Cache_Flowers> Config_Cache_Flowers_Single;
#define CONFIG_CACHE_FLOWERS    (Config_Cache_Flowers_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_VIP_H_ */
