/*
 * Config_Cache_Arena.h
 *
 *  Created on: 2014年5月23日
 *      Author: root
 */

#ifndef CONFIG_CACHE_ARENA_H_
#define CONFIG_CACHE_ARENA_H_


#include "Cache_Base.h"
#include "arena/Arena_Config.h"

class Config_Cache_Arena : public Cache_Base {
public:
	void refresh_config_cache(void);
public:
	Arena_Award_Config* get_arena_award_conifg(int id);
	Arena_Season_Award_Config* get_arena_season_rank_award_conifg(int id);
	Arena_Season_Award_Config* get_arena_season_group_award_conifg(int id);
	Arena_Base_Config* get_arena_base_conifg(int id);
	Arena_Setting_Config* get_arena_setting_conifg(void);
	Arena_Levelfix_Config* get_arena_levelfix_conifg(int id);
	double get_arena_levelfix(int id);
	Arena_Season_Config* arena_season_config(void);
	void save_arena_season_to_json(void);
	int get_arena_cd(void);
	// 竞技场最少cd
	int get_arena_min_cd(void);
	int get_arena_cd_cost(void);
	int get_arena_no_cd_vip_lv(void);
	// 竞技场背景地图
	int get_arena_fight_map_id(void);
	// 青铜段匹配参考战斗力
	int get_bronze_refer_force(const int group, const int seg);
	// 获取竞技场开放等级
	int get_arena_open_lv(void);
	// 竞技场购买付费次数vip等级
	int get_arena_pay_time_vip_lv(void);
	// 刷新匹配消耗
	int get_refresh_match_cost(void);
	//
	int get_refresh_match_time(void);
	// 获取段位名称
	std::string get_part_name(const int group, const int seg);
	// 奖励邮件标题
	std::string get_mail_title(void);
	// 奖励邮件发送者
	std::string get_mail_sender(void);
	// 奖励邮件内容
	std::string get_mail_content(void);

	std::string get_text_copper(void);
	std::string get_text_soul(void);
	std::string get_text_exploit(void);
	int get_arena_force_level(int force);
	int get_arena_max_force_lv(void);
private:
	void refresh_arena_award(void);
	void refresh_arena_season_award(void);
	void refresh_arena_base(void);
	void refresh_arena_levelfix(void);
	void refresh_arena_setting(void);
	void refresh_arena_season(void);
	void refresh_arena_force_level(void);
private:
	Arena_Award_Config_Map arena_award_map_;
	Arena_Season_Award_Config_Map arena_season_group_award_map_;
	Arena_Season_Award_Config_Map arena_season_rank_award_map_;
	Arena_Base_Config_Map arena_base_map_;
	Arena_Setting_Config arena_setting_;
	Arena_Levelfix_Config_Map arena_levelfix_map_;
	Arena_Season_Config arena_season_;
	Arena_Force_Level_Map force_level_map_;
	int max_force_level_;
};
typedef Singleton<Config_Cache_Arena> Config_Cache_Arena_Single;
#define CONFIG_CACHE_ARENA    (Config_Cache_Arena_Single::instance())


#endif /* CONFIG_CACHE_ARENA_H_ */
