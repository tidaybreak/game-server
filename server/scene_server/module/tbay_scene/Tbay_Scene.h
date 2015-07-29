/*
 * Tbay_Scene.h
 *
 *  Created on: 2015年4月10日
 *      Author: root
 */

#ifndef TBAY_SCENE_H_
#define TBAY_SCENE_H_


#include "Move_Scene.h"


class Monitor_Unique_ID;

class Tbay_Scene: public Move_Scene {
public:
	Tbay_Scene();
	virtual ~Tbay_Scene();

	void reset(void);

	void set_role_id(role_id_t role_id);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	virtual int tick(const Time_Value &now);

	virtual int enter_scene(Mover *mover);

	virtual int exit_scene(Mover *mover, bool offline);

	virtual int client_scene_complete(Fighter &fighter);

	//virtual int scene_pass_to_logic(void);

	//virtual void send_path_guide(int point, Scene_Player* player);

	//virtual void hook_scene_pass(void);

	//virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters);

	virtual void hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums);

	virtual void hook_player_move(Scene_Player* player);

	void set_single_scene_info(Monitor_Unique_ID& muid);

	void notice_single_scene(bool clear_single);

	void notice_tbay_info(Scene_Player* player);
public:
	int req_tbay_bless(Scene_Player* player, const int8_t bless_index);
	int req_fetch_tbay_info(Scene_Player* player);
	int inner_tbay_bless(Scene_Player* player, const int8_t bless_index);
private:
	void kick_all_player(void);
	void add_tbay_bless(Scene_Player* player);
	void del_tbay_bless(Scene_Player* player);
	void set_all_drops_invalid(void);
private:
	role_id_t role_id_;
	Int_Set monster_poinit_set_;
	bool offline_;
	bool has_scene_pass_box_;		// 否有通关宝箱
	bool has_gain_rewards;			// 是不已经领取通关奖励
	//
	Monitor_Unique_ID single_muid_;
	int8_t tbay_state_;
	int tbay_max_time_;
	int gold_num_;
	int8_t gold_type_;
	int8_t bless_index_;
	int bless_buff_id_;
	Time_Value tbay_finish_time_;
	bool player_ready_;
	bool player_normal_exit_;
};



#endif /* TBAY_SCENE_H_ */
