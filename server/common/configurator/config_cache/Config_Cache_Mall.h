/*
 * Config_Cache_Mall.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_MALL_H_
#define CONFIG_CACHE_MALL_H_

#include "Cache_Base.h"
#include "mall/Mall_Config.h"


class Config_Cache_Mall : public Cache_Base {
public:

	const UInt_Vec &get_fast_buy_tabs_sort(void);
	const UInt_Set &get_activity_id_set(void);
	const Mall_Activity_Price_Vec* find_activity_price_set_by_tabs_item(uint32_t tabs, uint32_t item_id);

	void refresh_config_cache(void);

	void refresh_fast_buy_tabs_sort_config_cache(void);
	void refresh_activity_price_config_cache(void);

private:
	UInt_Vec fast_buy_tabs_sort;
	UInt_Set activity_id_set;
	Mall_Activity_Price_Vec_Map activity_price_map;
};
typedef Singleton<Config_Cache_Mall> Config_Cache_Mall_Single;
#define CONFIG_CACHE_MALL    (Config_Cache_Mall_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_MALL_H_ */
