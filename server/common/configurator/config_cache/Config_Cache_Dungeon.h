/*
 * Config_Cache_Dungeon.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_DUNGEON_H_
#define CONFIG_CACHE_DUNGEON_H_

#include "Cache_Base.h"
#include "dungeon/Dungeon_Config.h"

class Item_Detail;

class Config_Cache_Dungeon : public Cache_Base {
public:
	void refresh_config_cache(void);

public:
	void refresh_single_copy_vip_config(void);
	void refresh_chapter_award_config(void);
	void refresh_elite_copy_config(void);
	void refresh_war_base_config(void);
	void refresh_war_streak_kill_award_config(void);
	void refresh_war_kill_num_award_config(void);
	void refresh_war_mine_award_config(void);
	void refresh_war_balance_config(void);
	void refresh_war_random_coord_config(void);
	void refresh_wipe_base(void);
	void refresh_wipe_award(void);
	void refresh_team_award(void);

	const Chapter_Award_Config* find_chapter_award_config(int chapter);
	int get_single_dungeon_chapter_count(std::vector<int>& chapter_vec);
	// 是否完成一个章节
	int check_single_dungeon_chapter_finish(int scene_id);

	int get_single_copy_vip_rate(int level);
	int get_elite_reset_vip_lv(void);
	int get_elite_reset_cost(void);
	int get_elite_hor_lv_limit(void);

	const War_Base_Config* war_base_config(void);
	const War_Balance_Config* war_balance_config(void);
	const War_Random_Coord_Config* war_random_coord_config(void);
	void get_kill_num_award(const int kill_num, int& rep);
	void get_streak_kill_award(int num, int end_num, int& merit, int& end_merit, int& rep, int& end_rep, double& score_pct, int& score);
	void get_war_mine_award(int mine_type, int& pieces, int& merit, int& score, int& rep);
	int get_war_level_limit(void);
	int get_war_over_rank_merit(int rank, std::vector<Id_Int_Int>& props);
	int get_war_win_merit(void);
	int get_war_win_piece(void);
	int get_war_win_coin(void);
	void get_war_win_prop(std::vector<Id_Int_Int>& props);
	int get_war_lose_merit(void);
	int get_war_lose_piece(void);
	int get_war_lose_coin(void);
	void get_war_lose_prop(std::vector<Id_Int_Int>& props);

	void get_war_result_ex_reward(const int result, const int kill_num, const int coe, std::vector<Item_Detail>& awards);

	std::vector<Int_Int> get_war_open_time(void);
	// 扫荡
	const Wipe_Award_Config* find_wipe_award_config(const int scene_id);
	const Wipe_Base_Config* wipe_base_config(void);

	const Team_Award_Config* find_team_award_config(const int scene_id);
	int get_wipe_out_lv_limit(const int wipe_type);
	int get_single_wipe_no_cd_lv(void);
	int get_wipe_out_cost_time(const int wipe_type);
	int get_wipe_out_cost_gold(const int wipe_type);
	bool is_nor_elite_wipe_out_activate(const int scene_id);
	bool is_hor_elite_wipe_out_activate(const int scene_id);
	// 上古、魔神殿扫荡开启副本id
	int get_elite_wipe_out_activate_id(const int diff);
	int get_elite_add_num_prop_id(void);
	int get_elite_add_num_cost(void);
	int get_elite_ch_num(void);
	int get_elite_add_num(void);
	int get_elite_add_num_type(void);
	// 单人副本版本号
	int get_single_ver(void);
	//
	int get_elite_ver(void);
	//
	int get_lord_temple_ver(void);
	// 魔神殿开启的上古副本id
	int get_lord_open_id(void);
private:
	Single_Copy_Vip_Config_Map single_copy_vip_config_map_;
	Chapter_Award_Map single_dungeon_chapter_award_map_;
	Elite_Copy_Config elite_copy_config_;

	War_Base_Config war_base_config_;
	War_Streak_Kill_Award_Map war_streak_kill_award_map_;
	War_Mine_Award_Config war_mine_award_config_;
	War_Balance_Config war_balance_config_;
	War_Random_Coord_Config war_random_coord_config_;
	War_Kill_Num_Award_Map war_kill_num_award_map_;

	Wipe_Base_Config wipe_base_;
	Wipe_Award_Config_Map wipe_award_map_;

	Team_Award_Config_Map team_award_map_;
};
typedef Singleton<Config_Cache_Dungeon> Config_Cache_Dungeon_Single;
#define CONFIG_CACHE_DUNGEON    (Config_Cache_Dungeon_Single::instance())

//////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_DUNGEON_H_ */
