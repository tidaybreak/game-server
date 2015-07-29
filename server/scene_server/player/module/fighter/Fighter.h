/*
 * Fighter.h
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 *
 *      新模块例子
 */

#ifndef FIGHTER_H_
#define FIGHTER_H_

#include "Fighter_Def.h"
#include "mover/Mover.h"
#include "status/Status.h"
#include "Fighter_Struct.h"

class NPC;

struct Skill_Config;
struct Skill_Detail;
struct Skill_DB_Info;
struct Fighter_Detail;
struct Prop_Setter;
struct Attack_Param;
struct Skill_Detail;
struct Skill_Running_Info;
struct Skill_Effect_Correct;
struct Skill_Cost;
struct Skill_Step_Fix;
struct Talent;
struct MSG_81000060;
struct MSG_81000061;
typedef boost::unordered_map<int, Skill_DB_Info> Skill_DB_Info_Map;
typedef std::vector<Skill_DB_Info> Skill_DB_Info_Vec;
typedef boost::unordered_map<int, Skill_Talent> Skill_Talent_Map;
typedef std::vector<Prop_Setter> Prop_Setter_Vec;
typedef std::vector<Skill_Cost> Skill_Cost_Vec;
typedef std::vector<Skill_Step_Fix> Skill_Step_Fix_Vec;

class Fighter : public Mover, public Status  {
public:
	typedef std::vector<Fighter*> Fighter_Vec;
	typedef std::vector<Prop_Change> Prop_Change_Vec;

	Fighter();

	virtual ~Fighter();

	void reset(void);

	void set_detail_prt(Fighter_Detail &detail);

	void load_detail(Block_Buffer &detail_buf);

	// 登录时module初始化
	void module_init(void);
	// 地图通知登录完成时调用
	void sign_in(void);
	// 登出时调用
	void sign_out(void);

	/*====================================================================*
	 * misc related (begin)
	 *====================================================================*/

	inline virtual Fighter *fighter(void);

	inline Fighter_Detail const &fighter_detail(void) const;

	inline Fighter_Detail* fighter_detail_ptr(void) const;

	double blood_max(void);

	double blood_current(void);

	double current_blood_rate(void);

	double mp_max(void);

	double mp_current(void);

	double current_mp_rate(void);

	double blood_pool_max(void);

	double blood_pool_current(void);

	virtual int time_up(const Time_Value &now);

	int phy_power_recover_time(void);

	int phy_power_buy_times(void);

	void set_strength_cur(int value);

	void set_strength_max(int value);

	void set_phy_power_recover_time(int value);

	void set_phy_power_buy_times(int value);

	bool set_clear_label(bool is_clear_morale);

	int get_learned_skill_lv(int skill_id) const;

	//消耗体力
	int cost_phy_power(int value);
	//恢复体力
	int restore_phy_power(int value);

	void sync_phy_power_info(uint16_t cur, int32_t recover_time, uint8_t buy_times);

	// inner msg
	int use_drug_inner(const int16_t tp, const uint32_t index, const double val, const int amount, Block_Buffer &buf);

	int restore_full_prop(Block_Buffer &buf);

	int req_battle_status(Block_Buffer &buf);

	// 获取默认技能
	int get_default_skill(void);

	/*====================================================================*
	 * misc related (end)
	 *====================================================================*/

	/*====================================================================*
	 * learning skill related (begin)
	 *====================================================================*/
	int add_skill_extra(int skill_id, int skill_lv);

	void remove_skill_extra(int skill_id, int skill_lv);

	void cost_skill_pointer(const int skill_points, int talent_id);

	void add_skill_pointer(const int skill_points);

	int save_learned_skill(int skill_id, int skill_lv, int talent_id);

	int get_skill_null_cell(const Talent &talent) const;

	int learn_skill(int skill_id, int skill_level, int talent_id);

	int erase_skill(int Type, int talent_id);

	void erase_skill_after_cost(int talent_id);

	int change_skill_loc(int skill_id, int new_loc, int talent_id);

	void sync_all_skill_to_client(int talent_id);

	int open_a_talent(int talent_id);

	int use_a_talent_page(int talent_id);

	int skill_cost_and_save(int type, int arg1, int arg2);

	/*====================================================================*
	 * learning skill related (end)
	 *====================================================================*/

	/*====================================================================*
	 * fight related (begin)
	 *====================================================================*/

	virtual void fight_tick(const Time_Value &now);

	virtual void sing_end(const Time_Value &now, int skill_id);

	virtual int ai_select_skill(void);

	virtual void call_broadcast_around(Block_Buffer &buf);

	virtual int move_able(void);

	// 使用技能
	int use_skill(int skill_id = 0);

	int use_skill_inner(int skill_id = 0);

	int use_normal_skill(const Skill_Config &cfg, const Skill_Detail& detail,
			const Skill_DB_Info& skill);
	int use_normal_skill(const Skill_Config &cfg, const Skill_Detail& detail,
			const Skill_DB_Info& skill, const Skill_Cost_Vec &cost_vec, const Time_Value &now);

	int use_additional_skill(const Skill_Config& cfg, const Skill_Detail& detail);

	// 使用追加技能
	int is_additional_skill(const Skill_Config& cfg, const Skill_Detail& detail);

	// 调用使用技能
	void handle_inner_call_skill_action(Role_Vec roles, int skill_id, int skill_lv);

	// 释放触发的技能
	void inner_call_skill_action(Fighter* target, const Skill_Detail & detail, int step);

	// 一开始战斗，被动技能触发技能
	void be_skill_trriger_skill(void);

	// 被动技能触发技能
	void async_be_skill_trigger_active_skill(int type);
	void async_be_skill_trigger_active_skill(int type, int skill_id, int  skill_lv);
	bool judge_async_be_skiill_trigger_active_skill(const Be_Skill_Effect &effect, int type);

	// add be skill
	void fight_add_be_skill(int skill_id, int skill_lv);

	// remove be skill
	void fight_remove_be_skill(int skill_id);

	// 释放技能
	int inner_skill_action(bool normal = true);

	//处理buff
	void handle_buff(const Skill_Effect_Val & val1, Fighter_Vec &fighter_vec,
			role_id_t caster_id, int skill_id, Statue_Source_Type type);

	// 选择目标
	int select_target(const Skill_Effect &effect, Skill_Running_Info &skill_running,
			Fighter_Vec &fighter_vec, const Skill_Step_Fix_Vec &step_fix);

	void add_floating_target(Skill_Running_Info &skill_running, Fighter_Vec &fighter_vec, Role_Vec& role_vec);

	// 技能效果
	int skill_action_effect(const Skill_Detail &detail, const Skill_Effect &effect,
			Skill_Running_Info &skill_running, Fighter_Vec &fighter_vec, const Skill_Step_Fix_Vec &step_fix);

	// 技能攻击
	int skill_attack(Attack_Param &attack_param, Fighter &attacker, Fighter &peer, Attack_Calc_Func func, const Skill_Step_Fix_Vec &step_fix);

	// 技能给目标治疗
	int skill_cure(Fighter &attacker, Fighter &peer, int type, double value, int skill_id);

	void actual_cure(Fighter &attacker, double cure_blood, int skill_id = 0);

	// 受伤(技能或其它)
	double hurt_by(Attack_Param &attack_param);

	// 伤害扣血
	double actual_sub_blood(const Attack_Param &attack_param);

	int actual_sub_blood_for_buff(const Attack_Param &attack_param, int status_id, int effect_id, double link_hurt_dis = 0, int type = 4); // buff飘字扣血

	void check_dead_and_broadcast(Fighter& fighter, role_id_t atk_id);

	int hurt_anti_process(const Attack_Param &attack_param);		// 反弹处理

	void immuno_injury(role_id_t attacker_id);

	int passive_tigger_attack(const Attack_Param &attack_param);	// 被动:攻击时触发(命不命中都触发)

	int passive_tigger_be_hit(const Attack_Param &attack_param);	// 被动:被攻击时触发(命不命中都触发)

	void clear_currnet_morale(void);

	void restore_hp(double val, bool board);

	void reset_morale_and_hp(double morale_val, double hp_val, bool board);

	void reset_player_data(Battle_Scene *scene);

	void blood_pool_add_blood();

	bool dead_status(void);

	bool dead_and_cast(void);

	bool dead_and_skill_end(void);

	int skill_step_used(void) const;

	int current_skill_id(void) const;

	inline Time_Value skill_tick_time(void);

	Time_Value get_conflic_time(const Time_Value & now) const;

	void get_current_step_targets(Role_Vec& vec);

	bool will_use_spec_skill(const Time_Value & now) const;

	bool is_using_spec_skill(void) const;

	bool is_singing(const Time_Value &now) const;

	void fix_sing_time(const Time_Value &add_time, const Time_Value &now);

	void fix_skill_step_time(const Time_Value & add_time);

	void fix_partner_wait_time(const Time_Value &add_time, const Time_Value &now);

	void calc_conflict_time(const Time_Value &now, int fix_sing_time);

	bool check_target_conflict(int target_conflict, const Role_Vec &vec);

	bool is_using_floating_skill(void) const;

	inline const Role_Vec& get_simulate_targets(void) const;

	void clear_simulate_target(void);

	int get_first_used_skill(void) const;

	inline void set_first_used_skill(int skill_id);

	int call_monster_partner(int monster_partner_id, int loc_type, Fix_Call_Monster_Vec fix_call_monster_vec,
			Skill_Running_Info &skill_running);

	void continue_battle_after_refresh(void);

	bool is_be_skill(int skill_id) const;

	int can_use_skill(int skill_id);

	int stop_sing(void);

	int break_use_skill();

	int handle_fight_pause(int type);

//	void check_locked_dead_target(Role_Int_Map &role_int_map);

	bool check_locked_dead_target(const role_id_t chk_id, const role_id_t atk_id);

//	void set_skill_pre_cd(const Time_Value &now);

	void return_morale_when_blt_over(void);

	double handle_poition_buff(int type, double atk, double atk_hrt_add, double pk_add, double hurt_add, double race_add);
	double get_pointion_buff_atk(int type);
	double get_pointion_buff_atk_hurt_rate(int type);
	double get_pointion_buff_hurt_rate(void);
	double get_pointion_buff_pk_hurt(void);
	double get_pointion_buff_race_hurt(Fighter &peer);

	void slice_floating_skill(void);
	void skip_addition_skill(bool is_skip);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////BATTLE SCENE/////////////////////////////////////////////////////////////////////////////////

	bool is_pk_battle_type(void);

	virtual int enter_battle_scene(Battle_Scene *scene);

	virtual int exit_battle_scene(int exit_from = 0);

	virtual int die_process(const Attack_Param &info);

	void broadcast_die_info(void);

	// 死亡复活
	void die_revive(void);

	void manual_set_fighter_dead(void);

	int handle_give_up_battle(void);

	int handle_give_up_battle_multi(int type);

	inline int set_battle_status(Battle_Status status);

	inline const Battle_Status& get_battle_status(void) const;

	void init_fighter_skill_pre_cd(const Time_Value &now);

	void init_be_skill_data(void);

	void send_fight_data_when_battle_start(void);

	void send_init_data_to_client(void);

	void handle_check_over_start(void);

	void delete_fighter(void);

	void handle_ai_kill_fighter();

	/*====================================================================*
	 * fight related (end)
	 *====================================================================*/

	///PROPERTY RELATE//////////////////////////////////////////////////////////////////////////////////

	// 修改战斗属性基础值
	int fight_basic_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic);
	// 修改战斗属性固定附加值
	int fight_fixed_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double fixed);
	// 修改战斗属性百分比附加值
	int fight_percent_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double percent);
	// 修改非战斗属性(通知客户端)
	int fight_normal_prop_modify(Property_Type pen, Operate_Type oen, int value, bool board = true);
	// 修改非战斗中属性
	int modify_props_normal(const Prop_Setter_Vec &prop, bool board = true, int from = 0);

	// 修改战斗属性(通知客户端)
	int fight_modify_fight_prop(const Prop_Setter_Vec &prop, MSG_81000060 &msg, bool board = true);
	// 修改战斗属性(不通知客户端)
	int fight_modify_fight_prop(const Prop_Setter_Vec &prop, Prop_Change_Vec &change_info);
	// 修改战斗属性（以符号方式决定加减）
	int fight_modify_props(const Prop_Setter_Vec &prop, Prop_Change_Vec &change_info);
	// // 修改战斗属性（以符号方式决定加减）
	int fight_modify_props_by_status(const Prop_Setter_Vec &prop, MSG_81000061 &msg, bool is_fighting = true, int from = 0);
	// 修改单个战斗属性（以符号方式决定加减）
	int fight_modify_prop(const Prop_Setter& prop);
	// 改变经验
	int modify_experience(int exp, Operate_Type oen = O_ADD, int from = 0);

	// 同步战力到客户端
	void sync_force_to_client(void);

	// 打印各部分战力值
	void print_all_part_force(int index, double change, Int_Double_Map force_map, int from = 0);

	void get_all_pre_force(Int_Double_Map &force_map);

	// 统计战斗总伤害
	void statistic_battle_damage(double value);
	void add_battle_damage(double value);
	double get_ttl_damage_in_battle(void) const;
	// 统计战斗中总被伤害
	void statistic_battle_be_damaged(double value);
	void add_battle_be_damaged(double value);
	double get_ttl_be_damanged_in_battle(void)const;
	// 统计战斗总治疗
	void statistic_battle_cure(double value);
	void add_battle_cure(double value);
	double get_ttl_cure_in_battle(void) const;


	Battle_Status battle_status(void);

	// for world boss battle
public:
	virtual void be_hurt_and_sub_blood(double sub_blood, double current_blood, role_id_t attacker_id);
	void set_blood_outer_of_battle(double sub_blood, double current_blood);

private:
	virtual void fight_prop_change_hook(int prop, double value);
	int use_skill_inner_check(int skill_id, const Skill_Detail& skill_detail, int skill_talent, const Time_Value& now);
	Skill_DB_Info* get_skill_db_info(int skill_id);
	void be_skill_call_buff(const Skill_DB_Info_Map& skill_map,
			const Skill_DB_Info_Vec& extra_skill_vec);
	void init_be_skill_fix_time(const Skill_DB_Info_Map& skill_map,
			const Skill_DB_Info_Vec& extra_skill_vec);
	void init_be_skill_fix_morale_cost(const Skill_DB_Info_Map& skill_map,
			const Skill_DB_Info_Vec& extra_skill_vec);
	int get_fix_buff_time(int status_id);
	double get_skill_fix_morale_cost(int skill_id);
	void check_and_clear_simulate_target(void);

	//技能附加属性
	void skill_ps_attr(const Skill_Effect_Correct &crt);
	void skill_recover_attr(const Skill_Effect_Correct & crt);

	//怒气操作
	void fight_cost(const Skill_Cost_Vec &cost_vec, int stage);
	void fight_add(const Skill_Cost_Vec &cost_vec, int add_type, int skill_id = 0);
	bool fight_cost_check(const Skill_Cost_Vec &cost_vec, Skill_Cost_Vec &final_cost, double fix_morale_cost);
	void fight_cost_filter(const Skill_Cost_Vec &cost_vec, Skill_Cost_Vec &final_cost, double fix_morale_cost);
	void handle_crit_add_moral(const Skill_Step_Fix_Vec & step_fix, double be_skill_moral, Attack_Result result, int skill_id);
	void Filter_crit_add_moral_Fix(const Skill_Step_Fix_Vec &in, Skill_Step_Fix_Vec &out);
	double get_be_skill_add_moral(Attack_Result result, Be_Skill_Add_Moral type);
	void add_moral_every_hit(double moral, Attack_Result result, int skill_id);
	void add_skill_cd_time(const Skill_Cd_Vec &cd_vec, const Time_Value& Now);
	void fix_all_partner_conflict_time(const Time_Value& now);
	void simulate_select_targets(const Skill_Detail& skill_detail);
	bool check_use_additional_skill(void);
	void run_all_effect_left(void);
	bool all_unit_dead(const Role_Vec & vec);
	bool is_all_effect_end(const Skill_Detail& detail);
	void transfer_call_monster(const Skill_Effect &effect, Skill_Running_Info &skill_running);
	void handle_buff_on_off(const Skill_Effect &effect, int skill_id);

	void get_all_be_skill_step_fix(Skill_Step_Fix_Vec &step_fix, int effect_grp_id);
	void check_and_get_step_fix(int type, const Skill_Step_Fix_Vec& step_fix, Skill_Step_Fix_Vec & fixese);
	void take_step_fix_out(const Double_Vec& values, const Skill_Step_Fix_Vec& step_fix, Skill_Step_Fix_Vec & fixese);
	void get_step_fix_by_target(const Skill_Effect & effect, Skill_Step_Fix_Vec & step_fix, Fighter_Vec &vec);
	void confirm_current_skill_effect(void);
	void total_skill_correct(const Skill_Effect_Correct &org, Skill_Effect_Correct &des, const Skill_Step_Fix_Vec &fix);
	void change_be_skill(int org_talent_id, int cur_talent_id);
	void get_effect_rate_correct(const Skill_Step_Fix_Vec &fix, Skill_Effect_Correct &des);
	void update_attr_when_learn_be_skill(const Skill_Config & cfg, const Skill_Detail& dtl);
	// 同步体力到客户端
	void sync_phy_power_to_client(void);
	// 同步体力到逻辑服
	void sync_phy_power_to_logic(void);


private:
	Fighter_Detail *fighter_detail_;
	Time_Value last_skill_use_timestamp_;							// 最后攻击的时间
	Time_Value last_be_attack_timestamp_;							// 最后受伤害时间

	Battle_Status battle_status_;											// 战斗状态
	int current_skill_id_;															// 当前技能
	bool is_dead_broadcast;													// 死亡发送广播
	bool is_singing_end;															// 是否吟唱结束
	Time_Value skill_tick_time_;												// 技能tick
	Time_Value conflict_time_;													// 冲突累积时间
	Time_Value sing_time_start_;												// 开始吟唱时间
	uint skill_step_used_;														// 技能释放进度
	uint last_skill_step_used_;													// 上次效果进度
	Role_Vec simulate_targets_;												// 初步选角
	Skill_Effect_Map current_effect_map_;								// 当前技能效果组

	Int_Int_Map fix_buff_time_;												// 修正buff时间
	Int_Int_Map fix_skill_cd_time_;											// 修正技能cd时间
	Int_Double_Map fix_morale_cost_map_;							// 修正怒气消耗
	Fight_Be_Skill_Map fight_be_skill_map_;							// 战斗产生的被动技能
	Be_Skill_Effect_Used_Map be_skill_effect_used_map_; 	// 被技能使用主动技能记录

	int pre_used_skill_id_;														// 预存使用的技能
	double pre_use_morale_;													// 预使用技能时使用的怒气
	int first_used_skill_;															// 第一次使用的技能
	double cost_morale;															// 技能使用怒气
	double spec_btl_morale_tmp_;											// 特别战斗，存怒气
	double spec_btl_blood_tmp_;											// 特别战斗，存血

	double battle_ttl_damage_;												// 战斗总伤害
	double battle_ttl_be_damaged_;										// 战斗总被伤害
	double battle_ttl_cure_;													// 战斗总治疗

	Time_Value cache_skill_tick_time_;									// 追加技，时间缓存
	uint cache_skill_step_used_;												// 追加技，效果step缓存
	int cache_current_skill_id_;												// 追加技，当前技能id缓存
	Role_Vec cache_simulate_targets_;
	Skill_Effect_Map cache_current_effect_map_;
};

inline Fighter_Detail const &Fighter::fighter_detail(void) const{
	return *fighter_detail_;
}

inline Fighter_Detail* Fighter::fighter_detail_ptr(void) const{
	return fighter_detail_;
}

inline int Fighter::skill_step_used(void) const {
	return skill_step_used_;
}

inline int Fighter::current_skill_id(void) const {
	return current_skill_id_;
}

inline Fighter *Fighter::fighter(void) {
	return this;
}

inline Time_Value Fighter::skill_tick_time(void) {
	return skill_tick_time_;
}

inline void Fighter::set_first_used_skill(int skill_id) {
	first_used_skill_ = skill_id;
}

inline const Role_Vec& Fighter::get_simulate_targets(void) const {
	return simulate_targets_;
}

inline int Fighter::set_battle_status(Battle_Status status) {
	battle_status_ = status;
	return 0;
}

inline const Battle_Status& Fighter::get_battle_status(void)const {
	return battle_status_;
}

#endif /* FIGHTER_H_ */
