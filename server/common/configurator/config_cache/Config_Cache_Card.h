/*
 * Config_Cache_Card.h
 *
 *  Created on: 2014年11月26日
 *      Author: root
 */

#ifndef CONFIG_CACHE_CARD_H_
#define CONFIG_CACHE_CARD_H_

#include "Cache_Base.h"
#include "card/Card_Config.h"
#include "Game_Typedef.h"

class Config_Cache_Card : public Cache_Base{
public:
	void refresh_card_config_cache(void);

public:
	const Card_Config* get_card_config(int id) const;
	const Card_Attr_Lv_Map* get_card_attr_lv_map(int attr_id) const;
	const Card_Poten_Interval* get_card_poten_interval(int quality) const;
	const Int_Int_Map* get_sub_attr_random_map(int set_id)const;
	const Card_Lv_Data* get_card_lv_data(int quality, int type, int lv) const;
	const Poten_Upgrade_Config_Vec &get_poten_upgrade_config_vec(void) const;
	double get_attr_lv_value(int attr_id, int lv) const;
	int get_quality_id(int poten) const;
	int get_init_exp(int quality, int type, int lv) const;
	int get_star_percent(int star);
	int get_type_percent(int type);
	const Int_Int_Map &get_type_percent();
	int get_max_lv_by_role_lv(int role_lv);

	int get_vip_max_upgrade_times(int vip_lv);
	const Int_Int* get_vip_upgrade_value(int times);

private:
	void refresh_base(void);
	void refresh_poten(void);
	void refresh_sub_attr(void);
	void refresh_max_lv_map(void);
	void refresh_star(void);
	void refresh_type(void);
	void refresh_attr(void);
	void refresh_upgrade_data(void);
	void refresh_vip_times(void);
	void refresh_vip_upgrade_card_map(void);
	void refresh_poten_upgrade_config_vec(void);

	int get_main_attr_id(int type, Int_Int_Map &map);

private:
	Card_Lv_Data_Map card_lv_data_map_;
	Card_Config_Map card_config_map_;
	Card_Poten_Interval_Map card_poten_interval_map_;
	Card_Sub_Attr_Map card_sub_attr_map_;
	Card_Lv_Map card_lv_map_;
	Int_Int_Map card_max_lv_map_;
	Int_Int_Map star_map_;
	Int_Int_Map type_map_;
	Int_Int_Map vip_times_map_;
	Vip_Upgrade_Card_Map vip_upgrade_card_map_;
	Poten_Upgrade_Config_Vec poten_upgrade_config_vec_;
};

typedef Singleton<Config_Cache_Card> Config_Cache_Card_Single;
#define CONFIG_CACHE_CARD (Config_Cache_Card_Single::instance())

#endif /* CONFIG_CACHE_CARD_H_ */
