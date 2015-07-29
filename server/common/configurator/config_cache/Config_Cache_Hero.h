/*
 * Config_Cache_Hero.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_HERO_H_
#define CONFIG_CACHE_HERO_H_

#include "Cache_Base.h"
#include "heroer/Hero_Config_Struct.h"

class Config_Cache_Hero : public Cache_Base {
public:
	const Hero_Config *find_hero(const int hero_id) const;
	const Int_Double_Map *find_hero_lvl_prop(const int hero_lvl) const;
	const Hero_Lvl_Misc_Config *find_hero_lvl_misc(const int hero_lvl) const;
	const Int_Double_Map *find_hero_id_correct_prop(const int hero_id) const;
	const Int_Double_Map *find_hero_id_conver_prop(const int hero_id) const;
	const Artifact_Cfg_Id *find_artifact(const int artifact_id) const;
	const Artifact_Cfg_Lvl *find_artifact_lvl(const int artifact_lvl) const;
	//const Artifact_Cfg_Id_Map& artifact_cfg_id_map(void) const;

	const Intimacy_Base* find_intimacy_base(int lv) const;
	const int find_intimacy_common(int type)const;
	const Hero_Intimacy_Gift* find_intimacy_gift(int item_id) const;
	const Intimacy_Community* find_intimacy_community(int id) const;

	void refresh_config_cache(void);

private:
	void refresh_hero_correction(void);
	void refresh_hero_conversion(void);
	void refresh_hero(void);
	void refresh_hero_property(void);
	void refresh_hero_lvl_misc(void);
	void refresh_artifact_cfg_id(void);
	void refresh_artifact_cfg_lvl(void);

	// hero assist
	void refresh_hero_intimacy(Hero_Intimacy_Map& map);
	void refresh_hero_intimacy_base(void);
	void refresh_hero_intimacty_common(void);
	void refresh_hero_intimacy_gift(void);
	void refresh_hero_intimacy_community(void);
private:
	Hero_Config_Map hero_config_map_;
	Hero_Lvl_Pro_val_Cfg_Map hero_lvl_prop_cfg_map_;
	Hero_Lvl_Misc_Cfg_Map hero_lvl_misc_cfg_map_;
	Hero_ID_Correct_Cfg_Map hero_id_correct_cfg_map_;
	Hero_ID_Conver_Cfg_Map hero_id_conver_cfg_map_;
	Artifact_Cfg_Id_Map artifact_cfg_id_map_;
	Artifact_Cfg_Lvl_Map artifact_cfg_lvl_map_;
	Intimacy_Base_Map intimacy_base_map;
	Int_Int_Map intimacy_common_map;
	Hero_Intimacy_Gift_Map hero_intimacy_gift_map;
	Intimacy_Community_Map intimacy_community_map;
};
typedef Singleton<Config_Cache_Hero> Config_Cache_Hero_Single;
#define CONFIG_CACHE_HERO    (Config_Cache_Hero_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_HERO_H_ */
