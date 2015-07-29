/*
 * Config_Cache_Buff.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_BUFF_H_
#define CONFIG_CACHE_BUFF_H_

#include "Cache_Base.h"
#include "status/Status_Config.h"

class Config_Cache_Buff : public Cache_Base {
public:
	const Btl_Buff_Map &btl_buff_cache(void) const;
	const Btl_Buff_Cfg *btl_buff(int buff_id) const;
	const Btl_Buff_Effect *btl_effect(int buff_id, int effect_id) const;
	const Btl_Buff_Effect *btl_effect(int effect_id) const;
	const Attribute_Number *attribute_number(const int attribute_number_id) const;
	const Int_Hash_Set *btl_scene_visible(const int scene_id) const;
	const Status_Effect_Argv &special_level_buff(void) const;

	void refresh_config_cache(void);

private:

	Btl_Buff_Map btl_buff_map_;
	Buff_Effect_Map effect_map_;
	Attribute_Number_Map attribute_number_map_;
	boost::unordered_map<int, Int_Hash_Set> scene_visible_;
	Status_Effect_Argv special_level_buff_;
};
typedef Singleton<Config_Cache_Buff> Config_Cache_Buff_Single;
#define CONFIG_CACHE_BUFF    (Config_Cache_Buff_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_BUFF_H_ */
