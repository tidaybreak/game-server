/*
 * Config_Cache_Reward.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_REWARD_H_
#define CONFIG_CACHE_REWARD_H_

#include "Cache_Base.h"
#include "reward/Reward_Config.h"


class Config_Cache_Reward : public Cache_Base {
public:

	const Reward_Back_Config* get_reward_back_conf(uint32_t item_id) const;
	const Login_Seven_Conf_Map& get_login_seven_config(void) const;
	const Login_Seven_Config* get_login_seven_conf_by_day(uint32_t day) const;
	const Activation_Code_Config* get_activation_code_conf_by_type(uint32_t type) const;
	const Reward_Back_Conf_Map* get_reward_back_map(void);
	const Online_Group_Conf* get_online_group_conf_by_group_id(uint32_t group_id) const;
	const uint32_t get_group_id_by_week(uint32_t week) const;
	const Online_Gift_Conf_Vec* get_online_gif_conf_vec_by_group_id(uint32_t group_id) const;
	const Keep_Reward_Config* get_keep_reward_config(void) const;

	void refresh_config_cache(void);
	void refresh_online_gif_config_cache(void);
	void refresh_login_seven_config_cache(void);
	void refresh_activation_code_config_cache(void);
	void refresh_reward_back_config_cache(void);
	void refresh_online_week_group_config_cache(void);
	void refresh_keep_reward_config_cache(void);

private:

	Online_Gift_Conf_Vec_Map online_gif_conf_vec_map_;
	UInt_UInt_Map online_week_group_;
	Online_Group_Conf_Map online_group_conf_map_;
	Login_Seven_Conf_Map login_seven_conf_map_;
	Activation_Code_Conf_Map activation_code_conf_map_;
	Reward_Back_Conf_Map reward_back_conf_map_;
	Keep_Reward_Config keep_reward_conf_;

};
typedef Singleton<Config_Cache_Reward> Config_Cache_Reward_Single;
#define CONFIG_CACHE_REWARD    (Config_Cache_Reward_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_REWARD_H_ */
