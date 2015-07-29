/*
 * Logic_Pet.h
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */

#ifndef LOGIC_PET_H_
#define LOGIC_PET_H_

#include "Logic_Player_Base.h"

struct Pet_Config;
struct Pet_Skill_Config;
struct Pet_Detail;
struct Prop_Setter;
typedef std::vector<Prop_Setter> Prop_Setter_Vec;

class Logic_Pet : virtual public Logic_Player_Base {
public:
	Logic_Pet(void);
	virtual ~Logic_Pet(void);
	void reset(void);

	void load_detail(Pet_Detail &detail);
	void save_detail(Pet_Detail &detail);

	void sign_int(void);
	void sign_out(void);

	void module_init(void);

	inline const Pet_Detail& pet_detail(void) const;

public:

	int handle_get_pet_active_item(void);
	int handle_get_pet_info(void);
	int handle_pet_upgrade_once(int auto_buy);
	int handle_pet_upgrade_auto(int auto_buy);
	int handle_active_pet(void);
	int handle_pet_illusion(int illusion_id);
	int handle_active_pet_illusion(int illusion_id);
	int handle_pet_learn_skill_normal(int skill_id);
	int handle_pet_learn_skill_advance(int skill_id);
	int handle_change_pet_state(int new_state);

	void pet_illusion_active_by_lv(int lv);
	int pet_illusion_active_by_item(int illusion_id);
	void refresh_pet_attr_for_hero(Prop_Setter_Vec &prop_vec, Int_Vec &skill_vec);

private:

	int pet_upgrade(const Pet_Config &pet_cfg, int auto_buy);
	void refresh_pet_attr(int pre_lv, int pos_lv, Int_Vec &illusion_vec);
	void refresh_pet_skill(Int_Int_Map &skill_map);
	int pet_learn_skill_replace_old_skill(const Pet_Skill_Config &cfg,
			Int_Int_Map& changed_skill_map);
	void pet_learn_skill_add_new_skill(const Pet_Skill_Config &cfg, int loc,
			Int_Int_Map& changed_skill_map);

private:

	Pet_Detail *pet_detail_;
};

const Pet_Detail& Logic_Pet::pet_detail(void) const {
	return *pet_detail_;
}

#endif /* LOGIC_PET_H_ */
