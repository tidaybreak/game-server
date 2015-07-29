/*
 * Monster.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef MONSTER_H_
#define MONSTER_H_


#include "NPC/NPC.h"
//#include "Scene_Struct.h"

class AI_Event;
class Battle_Scene;
struct Monster_AI_Config;

class Monster :	public NPC {
public:
	typedef int (Monster::*UpdateFunc)(void);
public:
	Monster(void);

	virtual ~Monster(void);

   int birth(role_id_t npc_id, int npc_type_id, int operate_type, Move_Scene* move_scene, NPC_Record *record, NPC_Addition_Info &npc_addition_info);
	int init(int npc_type_id, int operate_type, Move_Scene* move_scene, NPC_Record *record, NPC_Addition_Info &npc_addition_info);

	int battle_birth(role_id_t npc_id, int npc_type_id, NPC_Record *record, NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene);
	int battle_init(int npc_type_id, NPC_Record *record, NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene);

	int fini(void);
	void reset(void);

	virtual void sing_end(const Time_Value &now, int skill_id);

	virtual std::string role_name(void);
	virtual int tick(Time_Value &now);
	virtual int vision_tick(void);
	virtual int ai_select_skill(void);

	virtual void be_hurt_and_sub_blood(double sub_blood, double current_blood, role_id_t attacker_id);

	virtual int recover_self(void);

	int condition_action(Time_Value &now, AI_Action_Running &ai_action_running);

	virtual int move_appear_info(Block_Buffer &buf);
	virtual int make_move_info(Block_Buffer &buf, uint8_t toward, uint16_t x, uint16_t y);

	////////////////////////////////////////////////////////////////

	inline void reset_first_time_use_skill();

	bool get_first_time_use_skill(void);

	virtual int battle_appear_info(Block_Buffer &buf);

	virtual int battle_appear(void);

	virtual int battle_disappear(void);

	virtual int enter_battle_scene(Battle_Scene *scene);

	virtual int exit_battle_scene(int exit_from = 0);

	virtual int die_process(const Attack_Param &info);

	virtual void set_npc_status(int npc_status);

	virtual bool is_monster(void);

	virtual bool is_special_monser(void);

	virtual Monster *monster_self(void);

	inline Monster_Detail &monster_detail(void);

	inline const NPC_Config_Cache *monster_config(void);

	inline Walk_Path &walk_path(void);

	virtual int talk(Block_Buffer &buf);

	virtual int change_self(int jid);

	int move_watcher_tick(Coord &coord);

	int push_walk_path(const Coord &coord);

	virtual int ai_config_init(void);

	// 通知逻辑服死亡
	int notice_logic_die(role_id_t attacker);

	Coord_Vec &grid_coord_vec(void) {return grid_coord_vec_;}

	void teammate_move(Coord& target_coord);
	void fallow_move_from_leader(Coord& target_coord);
	void set_chase_target(role_id_t role_id);
	void clear_chase_target(void);

	int play_chat(void);
	int teammate_play_chat(void);

	bool can_attack(Scene_Player* player);
	// 是否相同朝向
	bool is_same_toward(int t1, int t2);
	// 怪物变身【变身类型，变身id】
	int ai_transform(const int transform_type, const int transform_id);

	// 人物id
	role_id_t master_role_id(void);
	// 人物id
	void set_master_role_id(const role_id_t role_id);

	role_id_t caller_id(void);

	void set_caller_id(const role_id_t role_id);
	// 标记为唤怪
	void set_summon_monster_flag(void);
	void set_ex_param(const int param_1, const int param_2, const int param_3);
	void get_ex_param(int& param_1, int& param_2, int& param_3);
private:
	int update_idle_behavior(void);
	int update_null_behavior(void);
	int update_move_behavior(void);
	// 追击模式
	int update_chase_mode(void);
	// 返回模式
	int update_back_mode(void);
	// 移动模式
	int update_walk_mode(void);
	// 空闲模式
	int update_idle_mode(void);
	//
	int update_null_mode(void);
protected:
	Walk_Path walk_path_;
	//Coord_Vec tick_walk_;
	Coord_Vec grid_coord_vec_;			// 用于存放临时数据
	Monster_Detail monster_detail_;

	//AI_Action_Running ai_action_running_;
	bool first_time_use_skill_;
private:
	UpdateFunc behavior_func;
	int vision_tick_count_;
	bool is_summon_monster_;			// 召唤的怪物
	int param_1_;
	int param_2_;
	int param_3_;
};

////////////////////////////////////////////////////////////////////////////////

Monster_Detail &Monster::monster_detail(void) {
	return monster_detail_;
}

const NPC_Config_Cache *Monster::monster_config(void) {
	return monster_detail_.config_cache;
}

Walk_Path &Monster::walk_path(void) {
	return walk_path_;
}

inline void Monster::reset_first_time_use_skill() {
	first_time_use_skill_ = true;
}

/*
40 *300 1:27.4 137.29977116704805491990846681922  0.29133333333333333333333333333334

300*300 1:31.6 146.46733477072858273422534948021  0.30533333333333333333333333333334

const int X_TIME = 200 * 291333;
const int Y_TIME = 200 * 145666;
const int X_Y_TIME = 200 * 305333;
 */

#endif /* MONSTER_H_ */
