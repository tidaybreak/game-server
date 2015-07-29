/*
 * Config_Cache_Title.h
 *
 *  Created on: 2014年7月24日
 *      Author: Linqiyou
 */

#ifndef CONFIG_CACHE_TITLE_H_
#define CONFIG_CACHE_TITLE_H_


#include "Cache_Base.h"
#include "title/Title_Config.h"

class Config_Cache_Title : public Cache_Base {
public:
	const Title_Cfg &title_cfg(void);
	const Int_Hash_Set &honour_task(void);
	const Title_Cfg_Detail * const find_title(const int32_t id) const;
	const Int_Hash_Set &handbook(void) const;
	void refresh_config_cache(void);

private:
	Title_Cfg title_cfg_;
	Int_Hash_Set honour_task_;
	Int_Hash_Set handbook_;
};
typedef Singleton<Config_Cache_Title> Config_Cache_Title_Single;
#define CONFIG_CACHE_TITLE    (Config_Cache_Title_Single::instance())


#endif /* CONFIG_CACHE_TITLE_H_ */
