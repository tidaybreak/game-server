/*
 * Config_Cache_Skill.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_SKILL_H_
#define CONFIG_CACHE_SKILL_H_

#include "Cache_Base.h"
#include "fighter/Fighter_Struct.h"

class Config_Cache_Fighter : public Cache_Base {
public:
	const Skill_Config *skill_config_cache(int skill_id) const;
	const Skill_Detail *skill_detail_cache(int skill_id, int level) const;
	const Skill_Cache_Map& skill_cache_map(void)const;
	const int fight_misc(const std::string &str);

	const Skill_Talent_Map *skill_talent_cache(int career) const;
	const Skill_Talent *skill_talent(int career, int id) const;
	const Skill_Talent_Cache_Map& skill_talent_cache_map(void) const;

	void get_skill_ai_config(int skill_id, std::vector<Skill_AI_Config*>& ai_config_vec);

	void refresh_config_cache(void);
	void get_attr_change_data(int id, Skill_Effect_Val_Inner & inner, double value);
	void refresh_talent_config_cache(void);
	void read_be_skill_config(Be_Skill_Effect_Map& be_skill_effect_map);
	void get_step_fix_config(const Json::Value & cfg, Skill_Step_Fix_Vec & vec);

	void refresh_phy_power_config(void);
	void refresh_fight_misc(void);

	int get_phy_power_buy_cost(int number);

	int get_phy_power_max_buy_times(int vip_level);

	void read_pre_skill(const Json::Value &cfg, Int_Int_Vec &pre_skill);
private:
	Skill_AI_Config_Map skill_ai_config_map_;
	Skill_Cache_Map skill_cache_map_;
	Skill_Talent_Cache_Map skill_talent_cache_map_;
	//体力
	Phy_Power_Cost_Config_Map phy_power_cost_map_;
	Phy_Power_Lv_Config_Map phy_power_lv_map_;
	String_Int_Map fighter_misc_;
};
typedef Singleton<Config_Cache_Fighter> Config_Cache_Skill_Single;
#define CONFIG_CACHE_FIGHTER    (Config_Cache_Skill_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_SKILL_H_ */
