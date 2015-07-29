/*
 * Battle_Scene.h
 *
 *  Created on: Mar 13, 2014
 *      Author: ti
 *
 *      战斗场景
 */

#ifndef BATTLE_SCENE_H_
#define BATTLE_SCENE_H_

#include "Scene_Struct.h"
#include "NPC/NPC_Struct.h"
#include "NPC/Battle_AI_Manager.h"
#include "Msg_Struct.h"
#include "Battle_Scene_Struct.h"

class Fighter;
struct Target_Select_Info;
typedef std::vector<Fighter*> Fighter_Vec;
typedef std::vector<AI_Foreshow_Info> AI_Foreshow_Info_Vec;
typedef boost::unordered_map<role_id_t, AI_Foreshow_Info_Vec> AI_Foreshow_Map;
class Move_Scene;

struct Attack_Param;
struct Battle_Give_Up;

struct Hurt_Total {
	Time_Value last_time;
	int times;
	double total_hurt;

	Hurt_Total(void) { reset(); }
	void reset(void);
};
typedef boost::unordered_map<int, Hurt_Total> Hurt_Total_Map;
typedef boost::unordered_map<role_id_t, Time_Value> Role_Time_Map;
typedef boost::unordered_map<int, Battle_Give_Up> Battle_Give_Up_Map;

class Battle_Scene {
public:
	enum Battle_Type{
		BATTLE_TYPE_PVP = 0,
		BATTLE_TYPE_PVE,
		BATTLE_TYPE_ARENA,
		BATTLE_TYPE_TEAM,
		BATTLE_TYPE_TEAM_ARENA,
		BATTLE_TYPE_DRAGON_VALE,
		BATTLE_TYPE_GANG_WAR_LEADER,
		BATTLE_TYPE_KNIGHT_TRIALS,
		BATTLE_TYPE_CHALLENGE,
		BATTLE_TYPE_HONOR_ARENA,
	};

	enum Conflict_Type {
		Conflict_With_Time = 0,
		Conflict_Without_Time,
	};

	enum Battle_Status {
		STATUS_NULL= 0,
		STATUS_READY,			// 战斗准备
		STATUS_ONGOING,		// 战斗进行
		STATUS_PAUSEING,
		STATUS_WILL_OVER,
		STATUS_OVER,			// 战斗完成
		STATUS_RECOVER,		// 等待回收
	};

	enum Over_Step{
		OVER_BEGIN = 0,
		OVER_BALANCE,			// 战斗结算
		OVER_SWITCH_SCENE,	// 切屏
		OVER_PLOT_ONE,			// 单人剧情
		OVER_PLOT_MULT,		// 多人剧情
		OVER_DROP,
		OVER_D_BALANCE,		// 副本结算
	};

	enum Pause_Type{
		PAUSE_TYPE_PLOT_ONE = 0,		// 单人剧情
		PAUSE_TYPE_PLOT_MULT,			// 多人剧情
		PAUSE_TYPE_TALK,					// 说话
		PAUSE_TYPE_USE_GUID,				// 新手引导
		PAUSE_TYPE_NEXT_WAVE_APPEAR,	// 下一波怪出现
		PAUSE_TYPE_NEXT_WAVE_FIGHT, 	// 下一波怪开打
	};

	enum Over_Type {
		OVER_TYPE_NORMAL = 0,
		OVER_TYPE_TIME_UP = 1,
		OVER_TYPE_GIVE_UP = 2,
	};

	typedef Hash_Map<Position_Key, role_id_t, NULL_MUTEX> Debug_Mover_Map;
	typedef Hash_Map<Position_Key, Mover *, NULL_MUTEX> Mover_Map;
	typedef Hash_Map<role_id_t, Int_Int_Map, NULL_MUTEX> Kill_Monster_Map;
	typedef Hash_Map<role_id_t, Role_Vec, NULL_MUTEX> Kill_Player_Map;

	Battle_Scene();
	virtual ~Battle_Scene();
	void reset();

	int init_scene_normal(long_id_t battle_id, Move_Scene* move_scene);
	void release_scene_normal(void);

	long_id_t battle_id(void);
	int battle_status(void);
	int over_step(void);
	Time_Value &last_tick();

	int check_fighters_at_start(const Time_Value &now);

	int tick(const Time_Value &now);

	void handle_battle_going(const Time_Value &now, Mover_Map& mover_map_copy);

	void handle_battle_start(const Time_Value &now);

	int empty_mover(void);

	int broadcast_around(Block_Buffer &buf);

	int enter_scene(Mover *mover);

	int exit_scene(Mover *mover);

	int appear(Mover *mover);

	int disappear(Mover *mover);

	Mover *find_mover_with_position(const Position_Key &position);

	Fighter *find_mover_with_role_id(role_id_t role_id);

	int client_scene_complete(Fighter &fighter);

	int listen_die_process(const Attack_Param &info, Fighter &dead);

	bool validate_position(uint16_t p, uint16_t idx);

	void find_fighter_enemy(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	bool find_fighter_enemy_random(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	void find_fighter_enemy_rule(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	void find_fighter_enemy_locked(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	bool find_fighter_enemy_blood(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	bool find_fighter_enemy_neg_buff(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	bool find_fighter_enemy_row(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	bool find_fighter_enemy_col(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	bool find_fighter_enemy_random_col(Fighter& fighter, Fighter_Vec &fighter_vec,
			const Target_Select_Info& target_info, int amount);
	bool judge_select_type(Mover &mover, const Int_Set &type_set);

	int find_index_from_rule(int idx1, int idx2, int ref_col);

	int check_skill_conflict(const Time_Value& now, Fighter &fighter);

	bool check_enemy_conflict(const Time_Value &now, Fighter &fighter,
			int &enemy_alive,Time_Value &wait_time, int &count, bool &all_dizz, bool &using_skill);

	bool check_allies_conflict(const Time_Value &now, Fighter &fighter,
			role_id_t &longest, Time_Value &wait_time, int &count, bool &all_wait, bool &using_skill);

	void set_statistic_shot(int pos);

	void total_hurt_statistic(int pos, double value, const Time_Value &now);

	void fix_partner_wait_time(const Time_Value & add_time, const Time_Value & now, Fighter &fighter);

//	void fix_all_unit_tick_time(const Time_Value & add_time, role_id_t role_id);

	bool check_battle_over_before_appear(void);

	bool check_all_enemy_dead(Fighter &fighter)const;

	bool check_all_enemy_dead_and_cast(Fighter &fighter)const;

	int check_battle_over(void);

	int check_over_step(const Time_Value& now);

	int check_pause_over(const Time_Value& now);

	void first_use_skill_trigger(int skill_id, Fighter& fighter);

//	void be_skill_trigger(void);

	void init_battle_info(Monster_Battle_Info& info, Fighter* player);

	void init_battle_hero_info(Fighter *player, Position_Key &player_pos_key);

	void refresh_monsters(int wave);

	void exit_monster_point(void);

	void notify_logic_kill_data(void);		//杀怪点结束通知

	void notify_gate_exit_battle(void);

	void notify_resume_battle(void);

	void notify_end_plot(void);

	void set_check_over_state(Fighter& fighter);

//	void check_locked_dead_target(Fighter &fighter, const Role_Vec & role_vec);

	bool check_dead_fighter_locked(const role_id_t chk_id, const role_id_t atk_id, int pos);

	int ai_select_skill(Fighter* fighter);
	void listen_use_skill(Fighter* fighter, int skill_id);
	void listen_break_use_skill(Fighter* fighter, int skill_id);
	void listen_fight_round(Fighter* fighter);
	void listen_die(Fighter* fighter);
	void listen_start_fight(Fighter* fighter);
	void listen_play_plot(std::string plot_id);
	void listen_play_talk(int talk_id);
	void listen_skill_sing_end(Fighter* fighter, int skill_id);
	int handle_plot_client_request(int type);
	bool battle_plot_is_over();
	int play_battle_plot(std::string plot_id, int plot_time);
	int play_battle_talk(role_id_t role_id, int talk_id, int talk_time);
	int ai_kill_all_monsters(int pos);
	void fight_prop_change_hook(Fighter* fighter, int prop, double value);
	void hook_status_overlay(Fighter* fighter, const int status_id, const int num);

	const Mover_Map &mover_map(void);

	inline void set_battle_type(int value);
	inline int get_battle_type(void)const;
	inline bool is_battle_over(void)const;

	int get_cur_monster_wave(void);
	int get_max_monster_wave(void);
	// 中断比赛
//	void interrupt_game(int win_pos);
	void set_map_id(uint32_t scene_map_id);
	uint32_t get_map_id();

	void add_player_on_battle(role_id_t id);

	void del_player_on_battle(role_id_t id);

	bool check_and_force_to_end(void);

	bool force_to_check_over(const Time_Value &now);

	void force_battle_to_end(int winner, Over_Type type = OVER_TYPE_NORMAL);

	void plot_to_end_battle(int winner);

	void clear_enemy_simulate_target(const Fighter &fighter);

	inline void set_end_battle_after_plot(bool value);

	void force_to_recycle_battle_scene(void);

	void force_finish_battle(void);

	Debug_Mover_Map &pos_debug_map(void);

	void battle_scene_debug(void);
	//
	void add_ai_foreshow_info(const role_id_t monster_role_id, AI_Foreshow_Info_Vec infos);

	void kill_partner_called_when_dead(Fighter &fighter);

	// 多人战斗认输
	int handle_multi_role_battle_give_up(role_id_t role_id, int side, int give_up_type);
	void check_battle_over_when_player_give_up(int side);
	void sync_give_up_data_to_client(int side);
	void sync_give_up_data_when_client_scene_completed(Scene_Player &player);

	void set_other_info(const Other_Info &ot);
	const Other_Info &get_other_info(void);
public:
	void set_world_boss_blood(const int blood);
	Fighter* get_boss_fighter(void);
	Move_Scene* move_scene(void);
	const Monster_Battle_Info &monster_battle_info(void) const;
private:
	//怪点是否有效
	bool monster_point_is_valid(void);

	void get_monsters_from_point(std::vector<int>& monsters);
	// 非正常中断比赛
	//void interrupt_game(Scene_Player* player);

	void put_player_to_battle(const Time_Value& now);

	NPC_Record* npc_record(void);

	int get_conflict_type();

	// 发送怪物ai预告信息
	void send_ai_foreshow_to_player(Fighter* fighter);
	void send_ai_foreshow_to_all(void);
	void check_all_monsters_exit(void);
	// 获取战斗力高的战斗位置
	int get_player_high_force_pos(void);
private:
	Time_Value last_tick_;
	Role_Time_Map player_on_battle_;
	int scene_id_;						// 场景ID
	bool signal_exit_check;				// 退出检查标志
	int conflict_time_;
	int check_times_;					// 强制检查次数

	long_id_t battle_id_;				// 战场id
	Mover_Map mover_map_;
	Time_Value start_time_;
	Time_Value exit_time_;
	Time_Value plot_pause_time_;
	Int_Int statistic_shot;				// 统计出手
	Hurt_Total_Map hurt_total_map;		// 统计10002效果伤害

	Battle_Status battle_status_;
	Battle_Status plot_btl_status_;
	Over_Step over_step_;
	Pause_Type pause_type_;
	Over_Type over_type_;
	bool end_battle_after_plot_;

	int monster_point_;					// 场景怪点
	int monster_group_;					// 场景怪组
	bool is_kill_boss_;					// kill boss
	Kill_Monster_Map kill_monster_;		// 记录杀怪
	Kill_Player_Map kill_player_;		// 记录杀人
	std::vector<role_id_t> balance_player_;

	////////////////////////////////////////////////////
	Monster_Battle_Info monster_battle_info_;

	int battle_type_;
	Battle_AI_Manager ai_manager_;
	int win_pos_;

	bool check_battle_over_;
	uint32_t map_id;

	Debug_Mover_Map pos_debug_map_;
	Move_Scene* move_scene_;

	// 怪物ai预告
	bool ai_foreshow_change_;
	AI_Foreshow_Map ai_foreshow_map_;

	Battle_Give_Up_Map battle_give_up_map_;

	// 标识该场战斗的信息：远征记录该场战斗所在点
	Other_Info other_info_;
};

inline long_id_t Battle_Scene::battle_id(void) {
	return battle_id_;
}

inline void Battle_Scene::set_battle_type(int value){
	battle_type_ = value;
	if (value == Battle_Scene::BATTLE_TYPE_PVE || value == Battle_Scene::BATTLE_TYPE_TEAM) {
		//MSG_TRACE_ABORT();
	}
}

inline int Battle_Scene::get_battle_type(void) const {
	return battle_type_;
}

inline bool Battle_Scene::is_battle_over(void) const {
	return (battle_status_ == STATUS_OVER || battle_status_ == STATUS_WILL_OVER);
}

inline int Battle_Scene::battle_status(void){
	return this->battle_status_;
}

inline int Battle_Scene::over_step(void){
	return this->over_step_;
}

inline Time_Value &Battle_Scene::last_tick() {
	return this->last_tick_;
}

inline void Battle_Scene::set_end_battle_after_plot(bool value) {
	end_battle_after_plot_ = value;
}

#endif /* BATTLE_SCENE_H_ */
