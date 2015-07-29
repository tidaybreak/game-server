/*
 * Config_Cache_Pet.h
 *
 *  Created on: 2015年4月10日
 *      Author: root
 */

#ifndef CONFIG_CACHE_PET_H_
#define CONFIG_CACHE_PET_H_

#include "Cache_Base.h"
#include "pet/Pet_Config.h"

class Config_Cache_Pet : public Cache_Base{

public:
	const Pet_Config_Map &get_pet_config_map(void);
	const Pet_Config *get_pet_config(int lv);
	const Pet_Illusion_Config *get_pet_illusion(int id);
	const Pet_Illusion_Config_Map &get_pet_illusion_map(void);
	const Pet_Learn_Skill_Config *get_pet_learn_skill(int num);
	const Pet_Skill_Config*  get_pet_skill_config(int skill_id);

public:

	void refresh_config_cache();
	void refresh_pet_config_map();
	void refresh_pet_illusion_config_map();
	void refresh_pet_learn_skill_config_map();
	void refresh_pet_skill_config_map();

private:

	void read_pet_sub_config(Pet_Prop_Value_Map &map, const Json::Value &json);

private:

	Pet_Config_Map pet_config_map_;
	Pet_Illusion_Config_Map pet_illusion_config_map_;
	Pet_Learn_Skill_Config_Map pet_learn_skill_config_map_;
	Pet_Skill_Config_Map pet_skill_config_map_;
};
typedef Singleton<Config_Cache_Pet> Config_Cache_Pet_Single;
#define CONFIG_CACHE_PET    (Config_Cache_Pet_Single::instance())

#endif /* CONFIG_CACHE_PET_H_ */
