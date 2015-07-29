/*
 * Config_Cache_Valentine_Monster.h
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */

#ifndef CONFIG_CACHE_VALENTINE_MONSTER_H_
#define CONFIG_CACHE_VALENTINE_MONSTER_H_


#include "Cache_Base.h"
#include "valentine_monster/Valentine_Monster_Config.h"


class Config_Cache_Valentine_Monster : public Cache_Base {
public:
	void refresh_config_cache(void);
private:
	void refresh_monster_cfg(void);
public:
	Vlt_Monster_Point_Map* get_monster_point_map(const int scene_id);
private:
	Vlt_Monster_Scene_Map cfg_monster_map_;
};

typedef Singleton<Config_Cache_Valentine_Monster> Config_Cache_Valentine_Monster_Single;
#define CONFIG_CACHE_VALENTINE_MONSTER (Config_Cache_Valentine_Monster_Single::instance())



#endif /* CONFIG_CACHE_VALENTINE_MONSTER_H_ */
