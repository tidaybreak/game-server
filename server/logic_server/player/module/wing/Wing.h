/*
 * Wing.h
 *
 *  Created on: 2014年10月13日
 *      Author: root
 */

#ifndef WING_H_
#define WING_H_

#include "Logic_Player_Base.h"

struct Wing_Detail;
struct Prop_Setter;
struct Wing_Config;
typedef std::vector<Prop_Setter> Prop_Setter_Vec;

class Wing : virtual public Logic_Player_Base {
public:
	Wing(void) { reset(); }
	virtual ~Wing(void);
	void reset(void);

public:
	void load_detail(Wing_Detail &detail);
	void save_detail(Wing_Detail &detail);

	void sign_int(void);
	void sign_out(void);

	void module_init(void);

	int trigger_daily_six(const Time_Value& now);
	void listen_wing_experience_expired(const Time_Value& now);

	int handle_active_wing_experience(void);
	int handle_get_wing_item(void);
	int handle_active_wing(void);
	int handle_get_wing_info(void);
	int handle_upgrade_wing_once(int is_auto_buy);
	int handle_upgrade_wing_auto(int is_auto_buy);
	int handle_change_wing_show_state(int state);
	int handle_get_ret_reward(int stage);
	void refresh_wing_avatar(void);
	void refresh_wing_attr(int pre_lv, int pos_lv);
	void get_wing_props_for_hero(Prop_Setter_Vec &vec);

	inline const Wing_Detail& wing_detail(void) const;

private:
	int handle_upgrade_wing(const Wing_Config &cfg, int is_auto_buy);
	bool handle_upgrade_wing(int &progress, int &rate, const Wing_Config &cfg);
//	int handle_upgrade_wing(int is_auto_buy);
	int get_wing_stage_score(int lv);
	bool is_wing_normal(void);

private:
	Wing_Detail *wing_detail_;
};

inline const Wing_Detail& Wing::wing_detail() const {
	return *wing_detail_;
}

#endif /* WING_H_ */
