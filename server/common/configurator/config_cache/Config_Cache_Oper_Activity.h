/*
 * Config_Cache_Oper_Activity.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_OPER_ACTIVITY_H_
#define CONFIG_CACHE_OPER_ACTIVITY_H_

#include "Cache_Base.h"
#include "oper_activity/Oper_Activity_Config.h"
#include "oper_activity/Oper_Activity_Struct.h"


class Config_Cache_Oper_Activity : public Cache_Base {
public:

	const Oper_Activity_Project_Config_Map &get_project_map(void) const;
	const Oper_Activity_Project_Config *get_project_config_by_id(const uint32_t id) const;
	const UInt_Set &get_has_effect_id(void) const;
	const UInt_Set *get_type_actidset(const uint32_t type) const;

	void refresh_config_cache(void);
	void refresh_activity_config_cache(void);



private:

	void refresh_activity_detail_config_cache(const Json::Value& config_json, Oper_Activity_Item_Line_Config_Map &item_line, const uint32_t act_id);
	void refresh_activity_detail_reward_config_cache(const Json::Value& config_json, Oper_Activity_Reward_Info_Config &reward);

	void refresh_activity_detail_config_pram_ext_all_type(const Json::Value& config_json, Oper_Activity_Item_Line_Config &value);
	bool refresh_activity_detail_config_pram_ext_array(const Json::Value& config_json, UInt_Vec &value);
	void refresh_activity_detail_config_pram_ext2_array(const Json::Value& config_json, UInt_2_Vec &value);
	void refresh_activity_detail_config_pram_ext_int(const Json::Value& config_json, uint32_t &value);

	void refresh_activity_extra_config_cache(const Json::Value& config_json, Oper_Activity_Append_Effect_Config_Map &effect_map, const uint32_t act_id);
	void refresh_activity_extra_buff_effect_config_cache(const Json::Value& config_json, Effect_Argv_Prop_Map &buff);

	Oper_Activity_Project_Config_Map project_map;
	UInt_Set has_effcet_id;
	UInt_UInt_Set_Map type_actidset;
};
typedef Singleton<Config_Cache_Oper_Activity> Config_Cache_Oper_Activity_Single;
#define CONFIG_CACHE_OPER_ACTIVITY    (Config_Cache_Oper_Activity_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_OPER_ACTIVITY_H_ */
