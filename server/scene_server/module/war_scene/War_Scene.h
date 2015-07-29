/*
 * War_Scene.h
 *
 *  Created on: 2014年7月1日
 *      Author: root
 */

#ifndef WAR_SCENE_H_
#define WAR_SCENE_H_

#include "Move_Scene.h"
#include "war/Scene_War_Struct.h"
#include "dungeon/Dungeon_Config.h"

class War_Scene: public Move_Scene{
	typedef boost::unordered_map<role_id_t, Scene_War_Rank_Info> Offline_Mover_Map;
public:
	War_Scene(void);
	virtual ~War_Scene(void);
public:
	void reset(void);
	virtual int tick(const Time_Value &now);

	virtual int enter_scene(Mover *mover);
	virtual int exit_scene(Mover *mover, bool offline);

	virtual int client_scene_complete(Fighter &fighter);
	virtual int update_move_able_info(Mover* mover);

	void npc_init(int scene_id, int sort, int sub_sort);
	void section_init(int scene_id, int sort, int sub_sort);
	void nor_mine_init(Monster_Point_Config* point_cfg);
	void revive_ball_init(Monster_Point_Config* point_cfg);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	virtual int scene_pass_to_logic(void);

	virtual void hook_player_enter_battle(Scene_Player* player);
	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters);
	virtual void hook_gather_success(Mover* mover, int point_id, int material_type);
public:
	int req_war_scene_fetch_rank(Scene_Player* player);

	int req_war_scene_heal(Scene_Player* player);

	void war_battle_finish(int win_camp);
	// 踢人命令
	void gm_kick_player(const role_id_t role_id);
private:
	void send_war_info(Mover* mover);
	void send_player_info(Scene_Player* player);
	void broadcast_war_info(void);
	Scene_War_Rank_Info* find_offline_mover(role_id_t role_id);
	void add_offline_mover(role_id_t role_id, Scene_War_Rank_Info* info);
	void del_offline_mover(role_id_t role_id);
	void check_recover_mine(void);
	void check_over(void);
	bool check_player_empty(void);
	void check_offline_player(const Time_Value& now);
	void check_inactive_player(const Time_Value& now);
	void check_is_war_over_player(const Time_Value& now);
	void war_over(int win_league);
	void recover_mine(int league, Scene_Player_Set& player_set);
	void kick_all_player(void);
	void kick_player(Scene_Player* player, int reason);
	void pop_random_coord(role_id_t point_id, Coord& coord);
	void push_random_coord(role_id_t point_id, const Coord& coord);
	void change_random_coord(role_id_t point_id);
	int war_scene_announce(int type, role_id_t role_id = 0, std::string name = "", int value1 = 0, int value2 = 0, int value3 = 0);
	int get_ann_id_from_streak_win_num(const int num);
private:
	int player_initial_score_;

	int light_score_;
	int light_num_;
	int dark_score_;
	int dark_num_;

	bool gain_first_blood_;			// 获得首杀
	bool check_over_;
	Time_Value war_over_time_;
	Offline_Mover_Map offline_mover_map_;

	Coord src_recover_coord_[2];
	War_Random_Coord_Config random_coord_;
	int war_total_time_;							// 活动剩余时间
	int act_time_;									// 活动配置的持续时间
	int ready_time_;								// 战斗开始准备时间
	Coord spe_mine_coord_;
	int spe_mine_cfg_ref_time_;
	Time_Value spe_mine_ref_time_;
	bool is_war_over_;							// 战场已经结束
	Time_Value check_is_war_over_;			// 战场已经结束检测
};




#endif /* WAR_SCENE_H_ */
