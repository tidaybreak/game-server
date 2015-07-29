/*
 * Config_Cache_Ranking.h
 *
 *  Created on: 2014-05-23
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_RANKING_H_
#define CONFIG_CACHE_RANKING_H_

#include "Cache_Base.h"
#include "ranking/Ranking_Config.h"


class Config_Cache_Ranking : public Cache_Base {
public:

	void refresh_config_cache(void);

	uint32_t get_time_by_ranking_type(uint32_t type);
	uint32_t get_num_by_ranking_type(uint32_t type);
	Ranking_Config_Map& get_config();

private:

	Ranking_Config_Map ranking_config_;

};
typedef Singleton<Config_Cache_Ranking> Config_Cache_Ranking_Single;
#define CONFIG_CACHE_RANKING    (Config_Cache_Ranking_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_RANKING_H_ */
