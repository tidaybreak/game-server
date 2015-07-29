/*
 * Config_Cache_Public.h
 *
 *  Created on: May 30, 2014
 *      Author: gaojianshan
 */

#ifndef CONFIG_CACHE_PUBLIC_H_
#define CONFIG_CACHE_PUBLIC_H_

#include "Cache_Base.h"
#include "public/Public_Config.h"
#include "public/Public_Def.h"

struct Arena_Adore_Public {
	void reset(void) {
		adore_cost_1.clear();
		adore_cost_2.clear();
		adore_cost_3.clear();
		adore_item_1.clear();
		adore_item_2.clear();
		adore_item_3.clear();
		adore_times = 0;
	}
	std::vector<Int_Int> adore_cost_1;
	std::vector<Int_Int> adore_cost_2;
	std::vector<Int_Int> adore_cost_3;
	std::vector<Int_Int> adore_item_1;
	std::vector<Int_Int> adore_item_2;
	std::vector<Int_Int> adore_item_3;
	int adore_times;
};

class Config_Cache_Public : public Cache_Base {
public:

	const Guide_Config_Map& guide_config(void) const;
	const Guide_Base_Config* find_guide_config(uint16_t id) const;
	const Sys_Cfg& sys_config() const;
	const Forenotice_Cfg_Lvl* find_forenotice_lvl(const int forenotice_lvl) const;
	const Forenotice_Cfg_Lvl_Map& forenotice_cfg_lvl_map(void);
	const std::map<int, int>& cfg_exploit_lvl_map(void);
	const Guide_Finish_Cfg_Map& get_guide_finish_cfg_map();
	const Notice_Cfg_Map& get_notice_cfg_map();
	const Challenge_Cfg& get_challenge_cfg();
	const Challenge_Area_Cfg* find_challenge_area_cfg(uint32_t scene_id);
	const int get_world_level_add_exp_inner_module(const int gap) const;
	const int get_world_level_add_exp(const int level, const int world_level) const;
	const Questionnaire_Reward_Config* get_questionnaire_reward_config_byqid(const uint8_t qid) const;
	const Questionnaire_Config_Map& get_questionnaire_config(void) const;
	const bool has_old_player(const std::string &account) const;
	const Invite_Reward_Config &get_invite_one_reward_config(void) const;
	const Invite_Reward_Config &get_invite_daily_reward_config(void) const;
	const Nobility_Config *find_title_nobility_cfg(const int32_t exploit_lvl) const;
	int world_level_index(void);

	int get_public_default_value(const int key);

	// 公会修行
	const int get_phy_power_every_practice(void) const;
	const int get_practice_gap_seconds(void) const;
	const int get_max_practice_times(void) const;
	const int speed_up_practice_every_min_cost(void) const;
	const int get_practice_scene_id(void) const;

	// 翅膀体验及返回配置
	const int get_wing_experience_lv(void) const;
	const int get_wing_experience_mins(void) const;
	const int get_wing_experience_ret_reward_time(void) const;

	//arena adore
	const Arena_Adore_Public &get_arena_adore_data(void) const;

	void refresh_config_cache(void);
	void refresh_guide_finish_cfg();
	void refresh_sys_config_cache();
	void refresh_notice_cfg_cache();
	void refresh_challenge_cfg_cache();
	void refresh_questionnaire_conf_cache(void);
	void refresh_invite_conf_cache(void);
	void refresh_world_level();
	void refresh_old_player_account();
	void refresh_config_nobility();

private:
	void refresh_guide_config_cache();
	void refresh_forenotice_cfg(void);
	void refresh_exploit_cfg(void);
private:
	Guide_Config_Map guide_config_map;
	Sys_Cfg sys_cfg;
	Forenotice_Cfg_Lvl_Map forenotice_cfg_lvl_map_;
	std::map<int, int> exploit_lvl_map_;					// 功勋等级－值有序Map
	Guide_Finish_Cfg_Map guide_finish_cfg_map_;
	Notice_Cfg_Map notice_cfg_map_;
	Challenge_Cfg challenge_cfg_;
	Int_Int_Map world_level_map_;
	int world_level_role_level_limit_;
	int world_level_effect_limit_;
	int world_level_index_;
	Questionnaire_Config_Map questionnaire_conf;
	String_Set old_player_account;
	Int_Int_Map phy_power_practice_conf_map_;
	Nobility_Config_Map nobility_config_map_;

	Invite_Reward_Config invite_ones_reward;
	Invite_Reward_Config invite_daily_reward;

	Int_Int_Map wing_public_cfg_map_;
	Int_Int_Map public_default_cfg_map_;

	Arena_Adore_Public arena_adore_data;
};

typedef Singleton<Config_Cache_Public> Config_Cache_Public_Single;
#define CONFIG_CACHE_PUBLIC   (Config_Cache_Public_Single::instance())


#endif /* CONFIG_CACHE_PUBLIC_H_ */
