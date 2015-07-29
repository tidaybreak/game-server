/*
 * ConfigCacheFashion.h
 *
 *  Created on: 2014年9月10日
 *      Author: root
 */

#ifndef CONFIGCACHEFASHION_H_
#define CONFIGCACHEFASHION_H_

#include "Cache_Base.h"
#include "fashion/Fashion_Config.h"
#include "Game_Typedef.h"

class Config_Cache_Fashion: public Cache_Base {
public:
	void refresh_config_cache(void);
	void refresh_base_map(void);
	void refresh_soul_map(void);
	void refresh_reinforced_map(void);

public:
	const Fashion_Base_Cfg* get_fashion_base_cfg(int id) const;
	const Fashion_Soul_Cfg* get_fashion_soul_cfg(int id) const;
	const Fashion_Reinforced_Cfg* get_fashion_reinforced_cfg(int lv, int part)const;
	int get_fashion_soul_max_lv(void) const;
	int get_fashion_reinforced_max_lv(int part) const;

private:
	Fashion_Base_Map fashion_base_map_;
	Fashion_Soul_Map fashion_soul_map_;
	Fashion_Reinforced_Map fashion_reinforced_map_;
};

typedef Singleton<Config_Cache_Fashion> Config_Cache_Fashion_Single;
#define CONFIG_CACHE_FASHION (Config_Cache_Fashion_Single::instance())

#endif /* CONFIGCACHEFASHION_H_ */
