/*
 * Config_Cache_Invest_Plan.h
 *
 *  Created on: Oct 14, 2014
 *      Author: gaojianshan
 */

#ifndef CONFIG_CACHE_INVEST_PLAN_H_
#define CONFIG_CACHE_INVEST_PLAN_H_

#include "Cache_Base.h"
#include "invest_plan/Invest_Plan_Config.h"

class Config_Cache_Invest_Plan : public Cache_Base {
public:
	const Month_Card_Invest_Cfg& get_month_card_invest_cfg(void) const;
	const Upgrade_Invest_Cfg& get_upgrade_invest_cfg(void) const;
	const Level_Profit_Cfg_Map& get_level_profit_cfg_map() const;
	const Vip_Defualt_Invest_Cfg_Map& get_vip_defualt_invest_cfg_map() const;
	const uint32_t* find_Level_Profit_Cfg(const uint32_t level);
	const uint32_t* find_vip_default_invest_cfg(const uint32_t vip);

	void refresh_config_cache(void);

private:
	void refresh_month_card_invest_cfg();
	void refresh_upgrade_invest_cfg();
	void refresh_level_profit_cfg();
	void refresh_vip_default_upgrade_invest_cfg();

private:
	Month_Card_Invest_Cfg month_card_invest_cfg;
	Upgrade_Invest_Cfg upgrade_invest_cfg;
	Level_Profit_Cfg_Map level_profit_cfg_map;
	Vip_Defualt_Invest_Cfg_Map vip_default_invest_cfg_map;

};


typedef Singleton<Config_Cache_Invest_Plan> Config_Cache_Invest_Plan_Single;
#define CONFIG_CACHE_INVEST_PLAN    (Config_Cache_Invest_Plan_Single::instance())

#endif /* CONFIG_CACHE_INVEST_PLAN_H_ */
