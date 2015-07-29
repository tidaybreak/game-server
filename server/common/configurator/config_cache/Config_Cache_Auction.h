/*
 * Config_Cache_Auction.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_AUCTION_H_
#define CONFIG_CACHE_AUCTION_H_

#include "Cache_Base.h"
#include "auction/Auction_Config.h"


class Config_Cache_Auction : public Cache_Base {
public:

	const Auction_Goods_Config_Map &get_sys_auction_goods_map(void);
	const Auction_Costs_Config *get_auction_costs_by_continue_time(uint32_t continue_time);
	const uint32_t get_comm_config_val(uint32_t key);

	void refresh_config_cache(void);
	void refresh_sys_auction_goods_config_cache(void);
	void refresh_costs_config_cache(void);
	void refresh_comm_config_cache(void);

private:

	Auction_Goods_Config_Map sys_auction_goods_map;
	Auction_Costs_Config_Map auction_costs_map;
	UInt_UInt_Map comm_map;

};
typedef Singleton<Config_Cache_Auction> Config_Cache_Auction_Single;
#define CONFIG_CACHE_AUCTION    (Config_Cache_Auction_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_AUCTION_H_ */
