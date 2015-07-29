/*
 * Config_Cache_Chat.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_CHAT_H_
#define CONFIG_CACHE_CHAT_H_

#include "Cache_Base.h"


class Config_Cache_Chat : public Cache_Base {
public:


	const uint32_t get_common_cfg_by_key(uint32_t key) const;
	const String_Set &get_white_list(void) const;
	const String_Set &get_black_list(void) const;


	void refresh_config_cache(void);
	void refresh_common_config_cache(void);
	void refresh_while_list_config_cache(void);
	void refresh_black_list_config_cache(void);


private:
	UInt_UInt_Map common_cfg;
	String_Set white_list;
	String_Set black_list;

};
typedef Singleton<Config_Cache_Chat> Config_Cache_Chat_Single;
#define CONFIG_CACHE_CHAT    (Config_Cache_Chat_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_CHAT_H_ */
