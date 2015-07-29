/*
 * Config_Cache_Recharge.h
 *
 *  Created on: 2014-05-05
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_RECHARGE_H_
#define CONFIG_CACHE_RECHARGE_H_

#include "Cache_Base.h"
#include "public/Public_Config.h"

class Config_Cache_Recharge : public Cache_Base {
public:

	const First_Recharge_Reward_Config &get_first_recharge_item_reward(void);
	const First_Recharge_Reward_Config &get_two_recharge_item_reward(void);
	const Limit_Time_Recharge_Reward_Config &get_limit_time_recharge_reward(void);
	const uint32_t get_common_config_by_key(const uint32_t key);
	const Recharge_Rebate_Config_Map &get_recharge_rebate_reward(void);
	const uint32_t get_pay_gift_mail_id_by_pay(uint32_t pay);

	void refresh_config_cache(void);
	void refresh_first_recharge(void);
	void refresh_two_recharge(void);
	void refresh_limit_time_recharge(void);
	void refresh_common(void);
	void refresh_rebate_recharge(void);
	void refresh_pay_gift_conf(void);

private:
	First_Recharge_Reward_Config first_recharge_reward;
	First_Recharge_Reward_Config two_recharge_reward;
	Limit_Time_Recharge_Reward_Config limit_time_recharge_reward;
	UInt_UInt_Map common_config;
	Recharge_Rebate_Config_Map recharge_rebate_reward;
	UInt_UInt_Map pay_gift_conf;
};
typedef Singleton<Config_Cache_Recharge> Config_Cache_Recharge_Single;
#define CONFIG_CACHE_RECHARGE    (Config_Cache_Recharge_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_RECHARGE_H_ */
