/*
 * Config_Cache_Rune.h
 *
 *  Created on: May 16, 2014
 *      Author: chenyulin
 */

#ifndef CONFIG_CACHE_RUNE_H_
#define CONFIG_CACHE_RUNE_H_

#include "Cache_Base.h"
#include "rune/Rune_Config_Struct.h"

class Config_Cache_Rune : public Cache_Base {
public:
	const Rune_Hole_Lvl_Cfg *find_hole_lvl_cfg(const int hole_lvl) const;
	const Rune_Part_Cfg *find_part_cfg(const int part) const;
	const Rune_Part_Cfg_Map *part_cfg_map(void) const;
	const Int_Vec *hole_lvl_energy_vec(void) const;
	const Hole_Lvl_Whole_Cfg *find_hole_lvl_whole_cfg(const int hole_lvl_whole) const;
	const Hole_Lvl_Whole_Cfg_Map &hole_lvl_whole_cfg_map(void) const;

	void refresh_config_cache(void);

private:
	void refresh_rune_hole_lvl(void);
	void refresh_rune_part(void);
	void refresh_hole_lvl_whole(void);

private:
	Rune_Hole_Lvl_Cfg_Map hole_lvl_cfg_map_;
	Rune_Part_Cfg_Map part_cfg_map_;
	Int_Vec hole_lvl_energy_vec_;
	Hole_Lvl_Whole_Cfg_Map hole_lvl_whole_cfg_map_;
};

typedef Singleton<Config_Cache_Rune> Config_Cache_Rune_Single;
#define CONFIG_CACHE_RUNE    (Config_Cache_Rune_Single::instance())

////////////////////////////////////////////////////////////////////////////////


#endif /* CONFIG_CACHE_RUNE_H_ */
