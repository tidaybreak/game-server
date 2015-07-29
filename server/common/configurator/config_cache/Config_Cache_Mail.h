/*
 * Config_Cache_Mail.h
 *
 *  Created on: 2014-05-05
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_MAIL_H_
#define CONFIG_CACHE_MAIL_H_

#include "Cache_Base.h"
#include "mail/Mail_Config.h"

class Config_Cache_Mail : public Cache_Base {
public:

	const Mail_Config_Map &mail_config(void) const;
	const Mail_Postage_Config_Map &mail_postage_config(void) const;
	const Mail_Base_Config *find_config(uint32_t id) const;
	const uint32_t find_postage_config(uint32_t number) const;
	const uint32_t find_common_config(const std::string &key) const;

	void refresh_config_cache(void);
	void refresh_setting_config_cache(void);
	void refresh_postage_config_cache(void);
	void refresh_common_config_cache(void);

	const Mail_Base_Config *find_merge_config(uint32_t id) const;
	void refresh_merge_config_cache(void);

private:
	Mail_Config_Map mail_config_;

	Mail_Postage_Config_Map mail_postage_config_;

	Mail_Common_Config_Map common_config_;

	Mail_Config_Map merge_mail_config_;

};
typedef Singleton<Config_Cache_Mail> Config_Cache_Mail_Single;
#define CONFIG_CACHE_MAIL    (Config_Cache_Mail_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_MAIL_H_ */
