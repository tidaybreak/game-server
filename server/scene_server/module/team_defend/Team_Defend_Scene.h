/*
 * Team_Defend_Scene.h
 *
 *  Created on: Dec 10, 2014
 *      Author: linqiyou
 */

#ifndef TEAM_DEFEND_SCENE_H_
#define TEAM_DEFEND_SCENE_H_

#include "Move_Scene.h"

class Team_Defend_Scene: public Move_Scene {
public:
	typedef boost::unordered_map<role_id_t, Int_Int_Map> Cumulate_Award_Info;
	Team_Defend_Scene();
	virtual ~Team_Defend_Scene();

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
	int active_team_def_time(Scene_Player *pl, const int add_time = 0);
	int active_team_def_boss_info(Scene_Player *pl);
	int active_team_def_appear_time(Scene_Player *pl);

	void kick_all_player(void);
private:
	void npc_init(int scene_id, int sort, int sub_sort);
	void section_init(int scene_id, int sort, int sub_sort);
	void check_team_transmit(void);
private:
	role_id_t role_id_;
	Role_Id_Set role_vec_;
	Int_Set monster_poinit_set_;
	bool has_gain_rewards; // 是否已经传回逻辑服
	Cumulate_Award_Info cumulate_award_; //累计奖励
	Role_Int_Map kill_nums_; // 累计击杀
	Time_Value start_time_; // 累计奖励
	Role_Id_Set delay_kick_player_;

	/////defend
	Time_Value end_time_;
	int wave_; // 当前波数
	Int_Hash_Set point_ids_; // 已用(弃用)
	Int_Hash_Set point_all_; // 当前可用
	Int_Hash_Set point_all_bak_; // 总备份
	int point_boss_; // boss点
	Int_String_Map pointid_name_;
	static const int BOSS_WAVE;
	Time_Value monster_appear_time_;
	int will_appear_point_;
	RoleID_Double_Map hurt_map_;
	RoleID_Double_Map be_hurt_map_;
	RoleID_Double_Map cure_map_;
};

#endif /* TEAM_DEFEND_SCENE_H_ */
