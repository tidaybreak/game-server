/*
 * Config_Cache_Anci_Artifact.h
 *
 *  Created on: 2014年10月10日
 *      Author: Linqiyou
 */

#ifndef CONFIG_CACHE_ANCI_ARTIFACT_H_
#define CONFIG_CACHE_ANCI_ARTIFACT_H_

#include "Cache_Base.h"
#include "anci_artifact/Anci_Artifact_Config.h"

class Config_Cache_Anci_Artifact : public Cache_Base {
public:
	const AA_Unban_Cond_Cfg &aa_unban_cond_cfg(void) const;
	const AA_Unban_Cond *find_aa_unban_cond(const int id) const;
	const AA_Gem *find_aa_gem(const int id) const;
	const AA_Cond *find_aa_cond(const int id) const;
	const AA_Cond_Cfg &aa_cond_cfg(void) const;

	void refresh_config_cache(void);

private:
	AA_Unban_Cond_Cfg aa_unban_cond_cfg_;
	AA_Gem_Cfg aa_gem_cfg_;
	AA_Cond_Cfg aa_cond_cfg_;
};
typedef Singleton<Config_Cache_Anci_Artifact> Config_Cache_Anci_Artifact_Single;
#define CONFIG_CACHE_ANCI_ARTIFACT    (Config_Cache_Anci_Artifact_Single::instance())


#endif /* CONFIG_CACHE_ANCI_ARTIFACT_H_ */
