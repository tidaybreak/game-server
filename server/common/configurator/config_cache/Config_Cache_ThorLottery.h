/*
 * Config_Cache_ThorLottery.h
 *
 *  Created on: 2014年11月12日
 *      Author: root
 */

#ifndef CONFIG_CACHE_THORLOTTERY_H_
#define CONFIG_CACHE_THORLOTTERY_H_

#include "Cache_Base.h"
#include "thorlottery/ThorLottery_Config.h"
#include "Game_Typedef.h"

class Config_Cache_ThorLottery {
public:
	void refresh_config_cache(void);

public:

	const ThorLottery_Item_Config* get_thorlottery_item_config(int season_id, int order_id, int item_id) const;
	const ThorLottery_Config* get_thorlottery_config(int season_id) const;
	const ThorLottery_Map& get_thorlottery_map(void) const;

private:
	ThorLottery_Map thorlottery_map_;
};

typedef Singleton<Config_Cache_ThorLottery> Config_Cache_ThorLottery_Single;
#define CONFIG_CACHE_THORLOTTERY (Config_Cache_ThorLottery_Single::instance())

#endif /* CONFIG_CACHE_THORLOTTERY_H_ */
