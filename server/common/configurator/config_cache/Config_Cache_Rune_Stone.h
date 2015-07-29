/*
 * Config_Cache_Rune_Stone.h
 *
 *  Created on: Aug 6, 2014
 *      Author: chenyulin
 */

#ifndef CONFIG_CACHE_RUNE_STONE_H_
#define CONFIG_CACHE_RUNE_STONE_H_

#include "Cache_Base.h"
#include "rune_stone/Rune_Stone_Cfg_Struct.h"

class Config_Cache_Rune_Stone : public Cache_Base {
public:
	const Item_Weight_Id_Cfg *find_item_weight_id(const int item_weight_id) const;
	const Smelter_Idx_Cfg *find_smelter_idx(const int smelter_idx) const;
	const Exp_Lvl_Cfg *find_exp_lvl(const int exp_lvl) const;
	const Rune_Stone_Color_Cfg *find_rune_stone_color(const int rune_stone_color) const;
	const Rune_Stone_Mix_Cfg& rune_stone_mix(void) const;
	const Exp_Lvl_Cfg_Map& exp_lvl_cfg_map(void) const;
	const Combine_Id_Cfg* find_combine(const int combine_id) const;
	const Combine_Id_Cfg_Map& combine_id_cfg_map(void);

	void refresh_config_cache(void);

private:
	void refresh_item_weight_id(void);
	void refresh_smelter_idx(void);
	void refresh_exp_lvl(void);
	void refresh_color(void);
	void refresh_mix(void);
	void refresh_combine(void);

private:
	Item_Weight_Id_Cfg_Map item_weight_id_cfg_map_;
	Smelter_Idx_Cfg_Map smelter_idx_cfg_map_;
	Exp_Lvl_Cfg_Map exp_lvl_cfg_map_;
	Rune_Stone_Color_Cfg_Map color_cfg_map_;
	Rune_Stone_Mix_Cfg mix_cfg_;
	Combine_Id_Cfg_Map combine_id_cfg_map_;
};

typedef Singleton<Config_Cache_Rune_Stone> Config_Cache_Rune_Stone_Single;
#define CONFIG_CACHE_RUNE_STONE    (Config_Cache_Rune_Stone_Single::instance())

//////////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_RUNE_STONE_H_ */
