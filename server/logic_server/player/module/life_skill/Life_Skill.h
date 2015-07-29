/*
 * Life_Skill.h
 *
 *  Created on: 2015年3月10日
 *      Author: zhoujin
 */

#ifndef LIFE_SKILL_H_
#define LIFE_SKILL_H_

#include "Logic_Player_Base.h"
#include "Life_Skill_Def.h"
#include "Json_Utility.h"

struct Life_Skill_Detail;
class Logic_Player;
class LFSkill_Base;
struct Smelt_Tab;

class Life_Skill : virtual public Logic_Player_Base {
public:

	Life_Skill(void);
	virtual ~Life_Skill();
	void reset(void);
	static int create_init(Life_Skill_Detail &detail);
	void load_detail(Life_Skill_Detail &detail);
	void save_detail(Life_Skill_Detail &data);

	Life_Skill_Detail const &life_skill_detail(void) const;

	void module_init(void);
	void sign_in(void);
	void sign_out(void);
	// 场景加载完成
	int move_scene_completed(void);

	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	// 每日6点刷新
	int trigger_daily_six(const Time_Value& now, bool logic_check = false);
	Int_Int * skill_unit(uint8_t type);

	void add_energy(const uint16_t energy,bool logic_check = false);
	int try_sub_energy(const uint16_t energy);
	int sub_energy(const uint16_t energy,const bool from_scene = false, const bool check = false);
	void start_energy_timer(const int type,const int time_inter = ENERGY_IN_BONFIRE_TIME);
	void energy_time_up(void);
	void energy_fire_time_up(void);
	void handle_energy_fire(void);              // 进入篝火处理
    void send_energy_change(void);
    void handle_energy_from_sence(const uint16_t energy);
    void syns_energy_to_sence();
    void syns_skill_to_scene();
    int sync_energy_info(Block_Buffer &buf);
	//msg
	int open_life_skill_panel(void);
	int learn_life_skill(const uint8_t type);
	int life_skill_upgrade(const int skill_id);
	int spar_refine(std::vector<uint32_t> &id_vec);
	int card_make(const uint8_t type);
    int spar_smelt(const int tab_id,const int num);

private:

    int collect_skill_up(Int_Int * unit,const LFSkill_Base * base);
    int card_skill_up(Int_Int * unit,const LFSkill_Base * base);
    int smelt_skill_up(Int_Int * unit,const LFSkill_Base * base);
    int up_item_id(const Id_Amount_Vec &vec,uint32_t &item_id);
    int refine_success(const int color,bool &success);
    int card_trigger_change(Int_Int * unit,const uint8_t ype,bool &success);
    int get_card_make_mer(const LFSkill_Base * base,Id_Amount_Vec &vec);
    int get_card_id(const bool success,const LFSkill_Base * base,uint32_t &item_id);
    int smelt_add(const Int_Int * unit,const int tab_id,const int num,int &add);
    int get_smelt_mer(const Smelt_Tab *tab_cof,Id_Amount_Vec &vec,const int num);

	Life_Skill_Detail *life_skill_detail_;
	Time_Value last_time;                // 上次启动恢复精力定时器时间
};

inline Life_Skill_Detail const &Life_Skill::life_skill_detail(void) const {
	return *life_skill_detail_;
}


#endif /* LIFE_SKILL_H_ */

