/*
 * Config_Cache_Package.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_PACKAGE_H_
#define CONFIG_CACHE_PACKAGE_H_

#include "Cache_Base.h"
#include "pack/Pack_Config.h"

class Config_Cache_Package : public Cache_Base {
public:

	// == package config ========
	const Package_Config &package_config(void) const;
	const Trade_Limit_Config &trade_limit_config(bool is_inner) const;
	const Rune_Stone_Pack_Grid_Cfg *find_rune_stone_pack_grid_cfg(const int grid) const;

	void refresh_config_cache(void);

private:
	void refresh_rune_stone_grid(void);

private:
	Package_Config package_config_;
};
typedef Singleton<Config_Cache_Package> Config_Cache_Package_Single;
#define CONFIG_CACHE_PACKAGE    (Config_Cache_Package_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_PACKAGE_H_ */
