/*
 * Status.h
 *
 *  Created on: Mar 14, 2014
 *      Author: ti
 */

#ifndef STATUS_H_
#define STATUS_H_

#include "Scene_Move_Base.h"
#include "Status_Def.h"
#include "fighter/Fighter_Def.h"
#include "Base_Struct.h"

struct Btl_Buff_Cfg;
struct Status_Effect_Argv;
struct Status_Effect_Detail;
struct Status_Info_Detail;
struct Status_Info;
struct Status_Detail;
struct Status_Info_Msg;

struct Position_Key;
typedef std::vector<Position_Key> Position_Vec;
class Status : virtual public Scene_Move_Base {
public:
	Status(void);
	virtual ~Status(void);

	void load_detail(void);
	int sign_in(void);
	int sign_out(void);
	void reset(void);

	virtual int client_scene_complete(void);

	int daily_refresh(void);

	const Status_Detail* status_detail(void) const;

	//心跳------------------------
	int time_up(const Time_Value &now); 				// time心跳
	void fight_ation_tick(Role_Vec &vec);				// 出手心跳
	void be_hit_tick(const Attack_Param &attack_param);	// 受击触发
	//心跳------------------------

	//插入状态------------------------
	int insert_status_out_fight(Status_Effect_Argv &argv); // 插入场景状态
	int insert_status_from_fight(Status_Effect_Detail &effect); // 战斗buff引发buff
	int insert_status_from_buff(Status_Effect_Argv &argv); // 状态插入状态
	int insert_status(Status_Effect_Argv &argv);
	int insert_status(Status_Info &detail);

	int insert_status_to_hero(Status_Effect_Argv &argv);
	int del_status_to_hero(Int_Hash_Set &ihs);
	int del_status_to_hero(const int id);
	//插入状态------------------------

	int make_buff_msg(const Status_Info *status_info, Status_Info_Msg &msg_info);
	int status_hook_enter_battle_scene(void); // 监听进入战斗时处理

	//删除状态------------------------
	void erase_status_type_for_msg(const int status_type);
	int erase_status_type(int status_type);
	void erase_status_in_set(const Int_Hash_Set &set);
	void clear_all_fighter_status(const bool is_from_die = false);
	void erase_evil_status_by_rand(const int num); // 随机删除指定个数恶性状态
	//删除状态------------------------

	// 消耗层
	int del_status_overlay(const int status_id, const int num);

	//效果操作------------------------
	// prop_change用于发给客户端显示
	virtual int insert_status_effect_handle(Status_Info_Detail &status_info, Status_Effect_Detail &effect);
	virtual int insert_status_effect_handle(Status_Effect_Detail &effect);//间隔为0的状态的延迟处理
	virtual int insert_status_effect_handle(Status_Effect_Detail &effect, const int scene);//转场景处理
	virtual int update_status_effect_handle(Status_Info_Detail &status_info, Status_Info_Detail &status_info_from,
			Status_Effect_Detail &old_effect, Status_Effect_Detail &new_effect);
	virtual int opt_status_effect_handle(Status_Effect_Detail &effect, const Time_Value &now_time,
			int skill_id = 0, Role_Vec role_vec = Role_Vec());//会导致死亡，清除status_map,所以操作完要检查
	virtual int erase_status_effect_handle(Status_Effect_Detail &effect);
	bool check_status_effect(Status_Effect_Detail &effect); // 检测buff场景设置
	bool check_status_effect(Status_Effect_Detail &effect, const int scene); // 检测buff场景设置
	//效果操作------------------------

	// 监听------------------------
	int hook_status_overlay(const int status_id, const int num);// 状态层数变化
	// 监听------------------------

	//状态释放技能
	virtual int buff_use_skill(Status_Effect_Detail &effect, const Role_Vec &role_vec); // 主动技能
	virtual int buff_use_skill_passive(Status_Effect_Detail &effect); // 被动技能
	virtual int buff_unuse_skill_passive(Status_Effect_Detail &effect); // 清除被动技能

	bool has_status_id(const int status_id); // buff是否存在
	bool has_status_evil(void); // 是否存在恶性buff
	bool hurt_link_exist(const int status_id); // 伤害链是否存在某状态id
	bool cure_link_exist(const int status_id); // 治疗链是否存在某状态id
	int status_num(const int status_id); // buff层数
	double exp_sum(void);	// buff经验加成
	double rebound_sum(void);	// buff反弹
	bool control_judge(void);
	bool is_hurt_link(void); // 是否存在伤害链
	bool is_dizz(void); // 眩晕
	bool is_stoic(void); // 霸体
	bool is_taunt(void); // 嘲讽
	bool is_lock(void); // 锁定
	bool is_slow(void); //
	bool is_invincible(void); // 无敌
	bool is_cure_link(void); // 是否存在治疗链
	bool is_silent(void);	// 沉默
	bool is_confusion(void); // 混乱

	int check_param(int status_id); // 检测是否可以插入(霸体)
	bool lottery_success(void); // 计算状态命中率是否命中

	//效果的处理------------------
	void handle_dispel(void); //驱散buff
	void handle_pure(void); //净化buff
	void handle_stoic(void); //霸体buff
	void handle_taunt(void); // 嘲讽
	double handle_shield(double hurt, role_id_t attack_id); // 护盾伤害免疫
	double handle_hurt_link(const Attack_Param &attack_param, double hurt_rate, Fighter &attacker); // 伤害链
	double handle_cure_link(Fighter &attacker, double attack_cure, double cure_fix); //
	void hand_lock(const int status_id); // 被锁定时，清除本方其它锁定对象
	//效果的处理------------------

	// 加属性
	int add_status_prop(Status_Effect_Detail& fix, int skill_id = 0);
	int sub_status_prop(Status_Effect_Detail& fix);
	//直接计算前后effect差值
	int cover_status_prop(Status_Effect_Detail& fix, Status_Effect_Detail& fix_from);
	int add_status_prop_by_per(Status_Effect_Detail& fix, int skill_id = 0);

	// 按属性治疗
	int cure_status_prop(Status_Effect_Detail& fix, int skill_id = 0);

	// 伤害链
	int add_hurt_link(Status_Effect_Detail &effect);
	int sub_hurt_link(Status_Effect_Detail &effect);
	int cover_hurt_link(Status_Effect_Detail& effect, Status_Effect_Detail& effect_from);

	// 治疗
	int add_cure_link(Status_Effect_Detail &effect);
	int sub_cure_link(Status_Effect_Detail &effect);
	int cover_cure_link(Status_Effect_Detail& effect, Status_Effect_Detail& effect_from);

	// 加护盾
	int add_shield_prop(Status_Effect_Detail& effect);
	int sub_shield_prop(Status_Effect_Detail& effect);
	int cover_shield_prop(Status_Effect_Detail& effect, Status_Effect_Detail& effect_from);

	int sync_status_info(void); //传数据到逻辑服
	int load_status_info(Block_Buffer &buf); //接收逻辑服传递过来的数据 (init)
	int inner_status_insert(Block_Buffer &buf); // 逻辑服插入buff

	bool &buff_insert_handle_switch(void);

	int special_status_handle(void);

	int hook_scene_change(int src_scene, int des_scene);
private:
	 // prop_change用于发给客户端显示
	int insert_new_status(const Status_Info &detail, const Btl_Buff_Cfg * status_config);
//	int merge_status_to_status_unoverlay(Status_Info &status_info_to, Status_Info &status_info_from); // 有损叠加
	int merge_status_to_status_overlay(Status_Info &status_info_to, Status_Info &status_info_from, bool has_overlay); // 无损叠加
	int merge_status_to_status(Status_Info &status_info_to, Status_Info &status_info_from, const Btl_Buff_Cfg *status_config);//叠加
	// 覆盖
	int cover_status_to_status(Status_Info &status_info_to, Status_Info &status_info_from);
	int special_insert_handle(Status_Info *status_info_to, std::list<Status_Info *> &status_list);
	int special_replace_handle(Status_Info *status_info_to, std::list<Status_Info *> &status_list, const Time_Value &last_end_time = Time_Value::zero);

	void active_erase_status(const Status_Info_Detail &status_info);
	void active_update_status(const Status_Info *status_info);
	void active_status_buf(bool broadcast, Block_Buffer &buf);

//	bool is_hero(void);
protected:
	Status_Detail *status_detail_;
	bool buff_insert_handle_switch_; // 状态生效开关：true表示为插入buff而不生效
};

#endif /* STATUS_H_ */
