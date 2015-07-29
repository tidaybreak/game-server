/*
 * Gang_Boss_Scene.h
 *
 *  Created on: 2015年2月7日
 *      Author: root
 */

#ifndef GANG_BOSS_SCENE_H_
#define GANG_BOSS_SCENE_H_


#include "Move_Scene.h"

class World_Boss_Manager;

class Gang_Boss_Scene: public Move_Scene{
public:
	Gang_Boss_Scene(void);
	~Gang_Boss_Scene(void);
public:
	void reset(void);
	void npc_init(int scene_id, int sort, int sub_sort);
	void set_logic_uuid(const int64_t uuid);

	virtual int tick(const Time_Value &now);

	virtual int enter_scene(Mover *mover);
	virtual int exit_scene(Mover *mover, bool offline);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);

	virtual int client_scene_complete(Fighter &fighter);
	virtual int update_move_able_info(Mover* mover);

	virtual void hook_player_enter_battle(Scene_Player* player);
	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters);
public:
	virtual void be_hurt_and_sub_blood(Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id);
	void set_world_boss_blood(const int blood);
	int get_world_boss_act_time_type(void);
	void kick_player(const role_id_t role_id);
private:
	void kick_all_player(void);
	void act_over(void);
	void act_recover(void);
	void broadcast_boss_info(void);
	void broadcast_rank_info(void);
	void broadcast_act_time(void);
	void add_leader_buf(void);
	void add_first_blood_reward(const role_id_t role_id);
	void add_fight_reward(const role_id_t role_id, int damage, const int add_effect);
	void force_end_battle(void);
	World_Boss_Manager* world_boss_manager(void);
	void refresh_gang_boss_drop_box(void);

	void check_weak_up_buf(void);
	void add_weak_up_1_buf(void);
	void clear_weak_up_1_buf(void);
	void add_weak_up_2_buf(void);
	void clear_weak_up_2_buf(void);
	void add_weak_up_1_buf_to_fighter(Fighter* fighter);
	void add_weak_up_2_buf_to_fighter(Fighter* fighter);
public:
	void announce_to_all(const int ann_id, const role_id_t role_id, std::string role_name, std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");
	void announce_to_guild(const int64_t gang_id, const int ann_id, const role_id_t role_id, std::string role_name, std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");
	void announce_to_scene(const int ann_id, const role_id_t role_id, std::string role_name, std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");
private:
	int64_t logic_uuid_;
	int act_result_;
	int8_t act_time_type_;
	int act_fight_time_;
	Time_Value act_end_time_;
	bool world_boss_hp_change_;
	int hp_refresh_tick_;
	role_id_t first_blood_role_id_;
	int act_state_;
	Time_Value balance_time_;
	Coord drop_box_coord_;
	int boss_step_1_value_;
	int boss_step_2_value_;
	int boss_step_3_value_;
	bool is_check_boss_step_;
};



#endif /* GANG_BOSS_SCENE_H_ */
