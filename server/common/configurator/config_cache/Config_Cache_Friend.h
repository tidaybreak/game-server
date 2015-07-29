/*
 * Config_Cache_Friend.h
 *
 *  Created on: 2014-06-09
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_FRIEND_H_
#define CONFIG_CACHE_FRIEND_H_

#include "Cache_Base.h"
#include "friend/Friend_Config.h"

class Config_Cache_Friend : public Cache_Base {
public:

	void refresh_config_cache(void);
	void refresh_default_head_photo_config_cache(void);
	void refresh_set_head_photo_config_cache(void);
	void refresh_common_config_cache(void);

	const Friend_Head_Photo_Config *find_set_head_config_by_photo_id(int32_t photo_id) const;
	const Friend_Default_Head_Photo_Config *find_default_head_config(uint8_t career, uint8_t gender) const;
	const Friend_Head_Photo_Map &get_set_head_photo_map(void) const;
	const uint32_t find_common_config_val(uint32_t key) const;


private:
	Friend_Default_Head_Photo_Config_Map default_head_photo_map;
	Friend_Head_Photo_Map photo_id_head_photo_map;
	UInt_UInt_Map common_config;
};
typedef Singleton<Config_Cache_Friend> Config_Cache_Friend_Single;
#define CONFIG_CACHE_FRIEND    (Config_Cache_Friend_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_FRIEND_H_ */
