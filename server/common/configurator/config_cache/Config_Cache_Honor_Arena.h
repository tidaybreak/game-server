/*
 * Config_Cache_Honor_Arena.h
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#ifndef CONFIG_CACHE_HONOR_ARENA_H_
#define CONFIG_CACHE_HONOR_ARENA_H_


#include "Cache_Base.h"
#include "honor_arena/Honor_Arena_Config.h"


class Config_Cache_Honor_Arena : public Cache_Base {
public:
	void refresh_config_cache(void);
private:
	void refresh_base(void);
	void refresh_add(void);
	void refresh_show(void);
	void refresh_rank(void);
public:
	const Honor_Arena_Show_Config* get_show_config(const int type);
	const Honor_Arena_Add_Config* get_add_config(const int type);
	const Honor_Arena_Base_Config* get_base_config(void);
	Int_Int_Vec& get_streak_show(void);
	Int_Int_Vec get_rank_reward(const int rank);
	// 匹配分差
	int get_match_score_spread(const int match_lv);
	void get_fight_lose_reward(int& score, int& copper, int& merit);
	void get_fight_win_reward(int& score, int& copper, int& merit);
	void get_streak_win_reward(const int num, int& score, int& merit);
	void get_finish_streak_win_reward(const int num, int& score, int& merit);
public:
	// 进入等级
	int get_enter_lv_limit(void);
	// 初始积分
	int get_base_score(void);
private:
	Honor_Arena_Base_Config cfg_base_;
	Honor_Arena_Add_Config_Map cfg_add_map_;
	Honor_Arena_Show_Config_Map cfg_show_map_;
	Honor_Arena_Rank_Config_Map cfg_rank_map_;
	Int_Int_Vec cfg_streak_show_;						// 连胜
};

typedef Singleton<Config_Cache_Honor_Arena> Config_Cache_Honor_Arena_Single;
#define CONFIG_CACHE_HONOR_ARENA (Config_Cache_Honor_Arena_Single::instance())



#endif /* CONFIG_CACHE_HONOR_ARENA_H_ */
