/*
 * Team_Scene.h
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#ifndef TEAM_SCENE_H_
#define TEAM_SCENE_H_

#include "Move_Scene.h"

class Team_Scene: public Move_Scene {
public:
	typedef boost::unordered_map<role_id_t, Int_Int_Map> Cumulate_Award_Info;
	Team_Scene();
	virtual ~Team_Scene();

	void reset(void);

	void set_role_id(role_id_t role_id);

	virtual int enter_scene(Mover *mover);

	virtual int exit_scene(Mover *mover, bool offline);

	virtual int client_scene_complete(Fighter &fighter);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	virtual int add_new_monster_point(const int point, bool is_boss, Coord point_coord);
	virtual int scene_pass_to_logic(void);

	virtual int tick(const Time_Value &now);

	virtual void send_path_guide(int point, Scene_Player* player);

	virtual void hook_player_enter_battle(Scene_Player* player);

	virtual void hook_scene_pass(void);

	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters);

	virtual void hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums);

	virtual void hook_monster_point_clear(const int point_id);

	void active_cumulate_award(Scene_Player *player);
private:
	void check_team_transmit(void);
private:
	role_id_t role_id_;
	Role_Id_Set role_vec_;
	Int_Set monster_poinit_set_;
	bool has_gain_rewards;
	Cumulate_Award_Info cumulate_award_;
	Role_Int_Map kill_nums_;
	Time_Value start_time_;
	Role_Id_Set delay_kick_player_;

	RoleID_Double_Map hurt_map_;
	RoleID_Double_Map be_hurt_map_;
	RoleID_Double_Map cure_map_;
};

#endif /* TEAM_SCENE_H_ */
