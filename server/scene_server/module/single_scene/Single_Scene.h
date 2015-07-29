/*
 * Single_Scene.h
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#ifndef SINGLE_SCENE_H_
#define SINGLE_SCENE_H_

#include "Move_Scene.h"

class Single_Scene: public Move_Scene {
public:
	Single_Scene();
	virtual ~Single_Scene();

	void reset(void);

	void set_role_id(role_id_t role_id);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	virtual int enter_scene(Mover *mover);

	virtual int exit_scene(Mover *mover, bool offline);

	virtual int add_new_monster_point(const int point, bool is_boss, Coord point_coord);

	virtual int scene_pass_to_logic(void);

	virtual void send_path_guide(int point, Scene_Player* player);

	virtual void save_battle_plot_date(std::string plot_id, int plot_step, int record_id);

	virtual void hook_player_enter_battle(Scene_Player* player);

	virtual void hook_scene_pass(void);

	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters);

	virtual void hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums);

	// 是否双倍收益
	virtual bool is_double_income(void);

	void set_double_income(const bool value);
	//
	void outpost_notice_exit(const int64_t monster_role_id, bool clear_single);
private:
	role_id_t role_id_;
	bool offline_;
	std::string plot_id_;
	int plot_record_id_;
	int plot_step_;
	bool has_scene_pass_box_;		// 否有通关宝箱
	bool has_gain_rewards;			// 是不已经领取通关奖励
	bool is_double_income_;			// 否双倍收益
	//
	role_id_t outpost_role_id_;
};

#endif /* SINGLE_SCENE_H_ */
