/*
 * Config_Cache_Achievement.h
 *
 *  Created on: 2014年7月21日
 *      Author: Linqiyou
 */

#ifndef CONFIG_CACHE_ACHIEVEMENT_H_
#define CONFIG_CACHE_ACHIEVEMENT_H_

#include "Cache_Base.h"
#include "achievement/Achievement_Config.h"

class Config_Cache_Achievement : public Cache_Base {
public:
	Achievement_Cfg_Detail_Map &achievement_cfg(void);
	const Achievement_Cfg_Detail *find_achievement_cfg(const int32_t id) const;
	Achievement_Cfg_Setting &achievement_cfg_setting(void);
	const Achievement_Cfg_Medal_Detail *find_achievement_medal(const int32_t id) const;

	void refresh_config_cache(void);

private:
	Achievement_Cfg_Detail_Map achievement_cfg_detail_map_;
	Achievement_Cfg_Setting achievement_cfg_setting_;
	Achievement_Cfg_Medal_Map achievement_cfg_medal_map_;
};
typedef Singleton<Config_Cache_Achievement> Config_Cache_Achievement_Single;
#define CONFIG_CACHE_ACHIEVEMENT    (Config_Cache_Achievement_Single::instance())



#endif /* CONFIG_CACHE_ACHIEVEMENT_H_ */
