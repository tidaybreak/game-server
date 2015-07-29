/*
 * Config_Cache_R2_Invite.h
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_ACTIVITY_H_
#define CONFIG_CACHE_ACTIVITY_H_


#include "Cache_Base.h"

class Config_Cache_R2_Invite : public Cache_Base {
public:

	uint32_t get_common_config_val(uint32_t type);

	void refresh_config_cache(void);
	void refresh_common_config_cache(void);


private:

	UInt_UInt_Map common_conf;

};
typedef Singleton<Config_Cache_R2_Invite> Config_Cache_R2_Invite_Single;
#define CONFIG_CACHE_R2_INVITE    (Config_Cache_R2_Invite_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ACTIVITY_H_ */
