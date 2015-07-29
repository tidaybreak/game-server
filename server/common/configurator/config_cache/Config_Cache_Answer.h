/*
 * Config_Cache_Answer.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_ANSWER_H_
#define CONFIG_CACHE_ANSWER_H_

#include "Cache_Base.h"
#include "answer/Answer_Config.h"


class Config_Cache_Answer : public Cache_Base {
public:

	const uint32_t get_common_cfg_by_key(uint32_t key);
	const Answer_Question_Config_Map &get_question(void);
	const Answer_Question_Config* get_question_byid(uint32_t id);
	const Answer_Integral_Reward_Config* get_integral_reward_by_level(uint32_t level);
	const Answer_Rank_Reward_Config_List &get_rank_reward_list(void);
	const double get_modulus_by_level(uint32_t level);
	const double get_modulus_item_by_level(uint32_t level);

	void refresh_config_cache(void);

	void refresh_question_config_cache(void);
	void refresh_common_config_cache(void);
	void refresh_integral_reward_config_cache(void);
	void refresh_rank_reward_config_cache(void);
	void refresh_level_modulus_config_cache(void);

	void refresh_answer_reward_config_cache(Answer_Reward_Config &reward, const Json::Value& json);


private:
	Answer_Question_Config_Map question;//题库
	UInt_UInt_Map common_cfg;//公用配置
	Answer_Integral_Reward_Config_Map integral_reward;//积分奖励
	Answer_Rank_Reward_Config_List rank_reward;//排名奖励
	Int_Double_Map level_modulus;//排名等级兑换系数
	Int_Double_Map level_item_modulus;//排名等级道具系数

};
typedef Singleton<Config_Cache_Answer> Config_Cache_Answer_Single;
#define CONFIG_CACHE_ANSWER    (Config_Cache_Answer_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ANSWER_H_ */
