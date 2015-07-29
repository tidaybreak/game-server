/*
 * ConfigCacheWing.h
 *
 *  Created on: 2014年10月14日
 *      Author: root
 */

#ifndef CONFIG_CACHE_WING_H_
#define CONFIG_CACHE_WING_H_

#include "Cache_Base.h"
#include "wing/Wing_Config.h"
#include "Game_Typedef.h"

class Config_Cache_Wing : public Cache_Base{
public:
	void refresh_config_cache(void);
	void refresh_ret_reward_config_cache(void);

public:
	const Wing_Config* get_wing_config(int lv) const;
	const Wing_Cfg_Map& get_wing_config_map(void) const;
	const Wing_Ret_Reward_Map& get_wing_ret_reward_map(void) const;
	const Int_Int_Vec* get_wing_ret_reward(int stage) const;

private:
	Wing_Cfg_Map wing_cfg_map_;
	Wing_Ret_Reward_Map wing_ret_reward_map_;
};

typedef Singleton<Config_Cache_Wing> Config_Cache_Wing_Single;
#define CONFIG_CACHE_WING (Config_Cache_Wing_Single::instance())

#endif /* CONFIGCACHEWING_H_ */
