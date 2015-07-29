/*
 * Config_Cache_Exchange.h
 *
 *  Created on: 2014年6月25日
 *      Author: Linqiyou
 */

#ifndef CONFIG_CACHE_EXCHANGE_H_
#define CONFIG_CACHE_EXCHANGE_H_

#include "Cache_Base.h"
#include "exchange/Exchange_Config.h"

class Config_Cache_Exchange : public Cache_Base {
public:
	const Exchange_Config *find_cache(const int16_t &type);
	const Exchange_Config_Detail *find_cache(const int16_t &type, const int32_t &id);
	const Int_Vec *find_lvl(const int16_t &type);
	const Int_IntSet_Map_Reverse *find_lvl_greater(const int16_t &type);
	Int_IntSetReverse_Map_Reverse &ecg_lvl_medal_cfg(void);

	void refresh_config_cache(void);
	void refresh_config_skill_exchange(void);

	const Skill_Exchange_Item *find_skill_exchange_item(const int id);

private:
	Exchange_Config exchange_equip_cfg_;
	Exchange_Config exchange_medal_cfg_;
	Exchange_Config exchange_item_cfg_;
	Exchange_Config exchange_friend_cfg_;
	Exchange_Config exchange_activity_cfg_;
	Int_IntSet_Map ecg_lvl_equip_cfg_;
	Int_IntSet_Map ecg_lvl_medal_cfg_;
	Int_IntSet_Map ecg_lvl_item_cfg_;
	Int_IntSet_Map ecg_lvl_friend_cfg_;
	Int_IntSet_Map ecg_lvl_activity_cfg_;
	Int_IntSet_Map_Reverse ecg_lvl_equip_gt_cfg_;
	Int_IntSet_Map_Reverse ecg_lvl_medal_gt_cfg_;
	Int_IntSet_Map_Reverse ecg_lvl_item_gt_cfg_;
	Int_IntSet_Map_Reverse ecg_lvl_friend_gt_cfg_;
	Int_IntSet_Map_Reverse ecg_lvl_activity_gt_cfg_;

	Int_Vec ecg_equip_cfg_;
	Int_Vec ecg_medal_cfg_;
	Int_Vec ecg_item_cfg_;
	Int_Vec ecg_friend_cfg_;
	Int_Vec ecg_activity_cfg_;

	Skill_Exchange_Map skill_exchange_map_;
};

typedef Singleton<Config_Cache_Exchange> Config_Cache_Exchange_Single;
#define CONFIG_CACHE_EXCHANGE (Config_Cache_Exchange_Single::instance())

#endif /* CONFIG_CACHE_EXCHANGE_H_ */
