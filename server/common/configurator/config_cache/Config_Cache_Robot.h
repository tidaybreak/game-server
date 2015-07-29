/*
 * Config_Cache_Robot.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_ROBOT_H_
#define CONFIG_CACHE_ROBOT_H_

#include "Cache_Base.h"
#include "Robot_Struct.h"

class Config_Cache_Robot : public Cache_Base {
public:

	// == robot config ========
	const Robot_Config &robot_config(void) const;

	void refresh_config_cache(void);

private:
	Robot_Config robot_config_;

};
typedef Singleton<Config_Cache_Robot> Config_Cache_Robot_Single;
#define CONFIG_CACHE_ROBOT    (Config_Cache_Robot_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ROBOT_H_ */
