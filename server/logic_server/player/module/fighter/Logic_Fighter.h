/*
 * Fighter.h
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 *
 *      新模块例子
 */

#ifndef LOGIC_FIGHTER_H_
#define LOGIC_FIGHTER_H_

#include "Logic_Player_Base.h"
#include "fighter/Fighter_Def.h"
#include "pack/Pack_Struct.h"

class Tasker;

struct Fighter_Detail;
struct Skill_DB_Info;
struct Skill_Detail;
struct Player_Init_Cache;
struct Prop_Setter;
struct Skill_Info;
struct Exp_All;

class Logic_Fighter : virtual public Tasker {
public:
	Logic_Fighter(void);
	virtual ~Logic_Fighter();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Fighter_Detail &detail);
	// 登录时detail赋值
	void load_detail(Fighter_Detail &detail);
	// 保存时detail赋值
	void save_detail(Fighter_Detail &data);

	void serialize_fighter_detail(Block_Buffer& buf);

	Fighter_Detail const &fighter_detail(void) const;

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);
    // 每天0点刷新
	int trigger_daily_zero(const Time_Value &now, bool logic_check = false);
	// 每天6点刷新
	int trigger_daily_six(const Time_Value &now, bool logic_check = false);

	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

	// 初始化外部获得的被动技能
	void init_be_skill_extra(int skill_id, int skill_lv);

    int fight_basic_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic, int from = 0);

    int fight_fixed_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double fixed, int from = 0);

    int fight_percent_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double percent, int from = 0);

    int fight_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic, double fixed, double percent, int from = 0);

    int fight_prop_modify(const std::vector<Prop_Setter> &setter_vec, int from);

    const Exp_All modify_experience_inter(int exp, const bool check_vip = false,
    		const bool check_buff = false,  const bool check_vip_team = false,
    		const bool check_world_level = true, const int double_exp = 1,
    		const int scene_id = 0,
    		Operate_Type oen = O_ADD);
    int modify_experience(int exp, Operate_Type oen = O_ADD, int from = 300);

	void reset_current_prop(void);

	int addition_prop_refresh(Addition_Type addi_type, Prop_Setter_Type pst, const std::map<int, double> &pro_val_map, int from);

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	void refresh_fighter_detail(Block_Buffer &detail_buf);

	// 等级改变监听
	virtual void hook_fighter_level(const int new_level, const int src_level);

	// 杀人监听
	void hook_kill_player(role_id_t role_id);

	// 杀怪监听
	void hook_kill_monster(int monster_type, int amount);

	// 杀点怪监听
	void hook_kill_monster_point(int scene_id, int row);
	// 杀怪点监听
	void hook_kill_monster_group(int scene_id, int id);

	// 场景目标完成监听
	void hook_compelte_scene(int scene_id);

	// 获取所有已学习技能
	int get_all_learned_skills(int talent_id = 0);
	// 学习技能
	int manual_learn_skill( int talent_id, int skill_id, int skill_lv);
	int learn_skill(int talent_id, const Skill_Detail &detail);
	int learn_skill_cost_item(int skill_id, int skill_lv);
	int erase_skill(int talent_id, int tye);
	int move_skill(int skill_id, int new_loc, int talent_id);
	int open_a_skill_talent(int page_id, int talent_id);
	int open_a_talent_page(int talent_id);
	int use_a_talent_page(int talent_id);
	int skill_cost(int type, int arg1, int arg2, int cost);
	int add_be_skills(const std::vector<Skill_Info> &skill_infos);
//	int module_init_add_be_skills(const std::vector<Skill_Info> &skill_infos);
	int remove_be_skills(const std::vector<Skill_Info> &skill_infos);
	int add_skill_pointer(const int skill_points);

	void sync_phy_power_info(uint16_t cur, int32_t recover_time, uint8_t buy_times);

	int restore_full_prop(int32_t type, int32_t per = 1);

	void set_phy_power_recover_time(int time);
	void set_phy_power(double value);
	void sync_phy_power_to_client(void);
	// 消耗体力
	int cost_strength(int value, bool to_scene = true);

	void update_force_by_sync_data(int force);

	int get_player_force(void);

	int get_ttl_force(void);

	int get_highest_force(void);
private:
	// 学习技能条件检查
	int normal_check(int career, int talent_id, int series, const Skill_Detail& config_detail);
	int manual_learn_checker(int career, int talent_id, int series, const Skill_Detail& config_detail);
	void init_skill(const Player_Init_Cache& init_config);
	void init_phy_power(void);
private:
	Fighter_Detail *fighter_detail_;

	double sign_current_blood_;
	double sign_current_morale_;
	double sign_current_strength_;
	double sign_current_blood_pool_;
};

inline Fighter_Detail const &Logic_Fighter::fighter_detail(void) const{
	return *fighter_detail_;
}

#endif /* LOGIC_FIGHTER_H_ */
