/*
 * Config_Cache_Alchemy.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_ALCHEMY_H_
#define CONFIG_CACHE_ALCHEMY_H_

#include "Cache_Base.h"
#include "alchemy/Alchemy_Config.h"


class Config_Cache_Alchemy : public Cache_Base {
public:

	const Cards_Info_Map &hero_critical_config(void) const;
	const Cards_Info_Map &hero_gold_config(void) const;
	const Cards_info *find_critical_config(uint32_t id) const;
	const Cards_info *find_gold_config(uint32_t id) const;


	const int get_initial_gold_by_level(int role_level) const;
	const int get_chances_free_by_vip_level(int level) const;
	const int get_chances_pay_by_vip_level(int level) const;
	const int get_pay_num_by_count(int count) const;
	const int get_pay_type_by_count(int count) const;
//_souls
	const Cards_Info_Map &hero_souls_critical_config(void) const;
	const Cards_Info_Map &hero_souls_gold_config(void) const;
	const Cards_info *find_souls_critical_config(uint32_t id) const;
	const Cards_info *find_souls_gold_config(uint32_t id) const;


	const int get_initial_souls_gold_by_level(int role_level) const;
	const int get_chances_souls_free_by_vip_level(int level) const;
	const int get_chances_souls_pay_by_vip_level(int level) const;
	const int get_pay_souls_num_by_count(int count) const;
	const int get_pay_souls_type_by_count(int count) const;

	void refresh_config_cache(void);
	void refresh_hero_critical_config_cache(void);
	void refresh_hero_gold_config_cache(void);
	void refresh_initial_gold_config_cache(void);
	void refresh_chances_config_cache(void);
	void refresh_chances_chargend_config_cache(void);
//souls
	void refresh_hero_souls_critical_config_cache(void);
	void refresh_hero_souls_gold_config_cache(void);
	void refresh_initial_souls_gold_config_cache(void);
	void refresh_chances_souls_config_cache(void);
	void refresh_chances_souls_chargend_config_cache(void);

private:
	Cards_Info_Map hero_critical_config_;
	Cards_Info_Map hero_gold_config_;
	Int_Int_Map initial_gold_map;
	Int_Int_Map chances_free_map;
	Int_Int_Map chances_pay_map;
	Int_Int_Map chances_charged_map;
	Int_Int_Map chances_money_type_map;
//souls
	Cards_Info_Map hero_souls_critical_config_;
	Cards_Info_Map hero_souls_gold_config_;
	Int_Int_Map initial_souls_gold_map;
	Int_Int_Map chances_souls_free_map;
	Int_Int_Map chances_souls_pay_map;
	Int_Int_Map chances_souls_charged_map;
	Int_Int_Map chances_souls_money_type_map;

};
typedef Singleton<Config_Cache_Alchemy> Config_Cache_Alchemy_Single;
#define CONFIG_CACHE_ALCHEMY    (Config_Cache_Alchemy_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ALCHEMY_H_ */
