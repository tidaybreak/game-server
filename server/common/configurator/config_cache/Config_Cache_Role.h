/*
 * Config_Cache_Role.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_ROLE_H_
#define CONFIG_CACHE_ROLE_H_

#include "Singleton.h"
#include "Cache_Base.h"
#include "Base_Struct.h"

struct Player_Name_Depot {
	Player_Name_Depot(void) { reset(); };
	void reset(void);

	std::vector<std::string> family_name_depot;
	std::vector<std::string> m_name_depot;
	std::vector<std::string> f_name_depot;
};

struct Player_Init_Cache {
	typedef std::vector<Int_Double> Prop_Value_Vec;
	typedef boost::unordered_map<int, Prop_Value_Vec> Prop_Conversion;
	typedef boost::unordered_map<int, Int_Int> Default_Skill_Map;
	typedef boost::unordered_map<int, Default_Skill_Map> Role_Default_Skill_Map;

	Player_Init_Cache(void) { reset(); };
	void reset(void);

	int birth_scene;
	Coord birth_coord;

	int safe_scene;
	Coord safe_coord;

	int level;
	Role_Default_Skill_Map default_skill;

	Prop_Value_Vec initial_attribute;
	Prop_Conversion prop_conversion;
	int first_single_id;
	int first_nor_elite_id;
	int first_hor_elite_id;

};

// 角色战斗属性基础值
typedef std::vector<Int_Double> Prop_Value_Vec;
typedef boost::unordered_map<int, Prop_Value_Vec> Level_Prop_Map;
typedef boost::unordered_map<uint8_t, std::string> Career_Map;

class Config_Cache_Role : public Cache_Base {
public:
	Config_Cache_Role(void);
	~Config_Cache_Role(void);


	const Player_Init_Cache &player_init_cache(void) const;
	const Prop_Value_Vec *level_fighter_prop_cache(int level) const;
	const int64_t level_up_need_exp_map(int level) const;
	const int level_up_add_skill_points(int level) const;
	const std::string error_tip_map(int error_id) const;
	const Int_Set &fight_prop_set(void) const;
	const Int_Set &m_race_fight_props(void) const;

	const std::vector<std::string>& get_family_name_vec(void) const;
	const std::vector<std::string>& get_male_name_vec(void) const;
	const std::vector<std::string>& get_female_name_vec(void) const;
	const double get_ref_force(int lv) const;
	std::string get_career_name(const uint8_t id) const;

	/////////////////////////////////////////////////////////////////////////////////////

	void refresh_config_cache(void);

	void refresh_player_init_cache(void);
	void refresh_player_name_depot(void);
	void refresh_level_fighter_prop(void);
	void refresh_error_tip_cache(void);
	void refresh_fight_prop_cache(void);
	void refresh_m_race_fight_props(void);
	void refresh_career_tips(void);

public:
	int get_elite_first_id(const int diff);
	int get_single_first_id(void);

private:
	Int_Uid	level_up_need_exp_map_;					// 升级需要经验
	Int_Int_Map level_up_add_skill_points_;			// 升级加的技能点
	Player_Init_Cache player_init_cache_;			// 玩家初始化数据
	Level_Prop_Map level_prop_map_;					// 角色战斗属性基础值
	Int_String_Map error_tip_map_;
	Int_Set fight_prop_set_;						// 可修改的战斗属性集
	Int_Set m_race_fight_props_;					// 可修改的种族战斗属性集
	Player_Name_Depot player_name_depot_;			// 初始化名字库
	Int_Double_Map ref_force_map_;					// 参考战力表
	Career_Map career_map;                          // 职业--职业名

};
typedef Singleton<Config_Cache_Role> Config_Cache_Role_Single;
#define CONFIG_CACHE_ROLE    (Config_Cache_Role_Single::instance())

#define FIGHT_PROP_SET	(CONFIG_CACHE_ROLE->fight_prop_set())
#define M_RACE_FIGHT_PROPS	(CONFIG_CACHE_ROLE->m_race_fight_props())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ROLE_H_ */
