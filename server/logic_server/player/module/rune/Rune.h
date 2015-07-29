/*
 * Rune.h
 *
 *  Created on: May 15, 2014
 *      Author: chenyulin
 */

#ifndef RUNE_H_
#define RUNE_H_

#include "Logic_Player_Base.h"

class Packer;

struct Rune_Detail;
struct Bat_Consume_Info;
struct Rune_Hole_Detail;
struct Rune_Hole_Lvl_Cfg;
struct Item_Detail_Config;
struct Rune_Part_Detail;

class Rune : virtual public Packer {
public:
	Rune(void);
	virtual ~Rune();
	void reset(void);

	static int create_init(Rune_Detail &detail);

	void load_detail(Rune_Detail &detail);
	void save_detail(Rune_Detail &detail);

	void sign_in(void);
	void sign_out(void);

	void module_init(void);
	int client_ready_now(void);

	inline const Rune_Detail &rune_detail(void) const;

	// MSG start ===================================
	int req_fetch_rune_info(void);
	int req_rune_activate(const int part, const int hole_index, const int8_t auto_buy);
	int req_rune_fill_energy(const int part, const int hole_index, int8_t is_bat, const int8_t auto_buy);

	void active_rune_hole_info(const int part, const int hole_index, bool is_upgrade);
	int active_rune_info(Logic_Player* target_player);
	// MSG end =====================================

	int rune_prop_refresh(void);
	//int rune_prop_refresh(const int rune_part, std::map<int, double> &pro_val_map);
	int calculate_rune_prop(const int hole_lvl, const int prop_type, std::map<int, double> &pro_val_map);
	int rune_prop_refresh(const int rune_part, const int equip_index, std::map<int, double> &pro_val_map);
	int hole_lvl_whole_prop_refresh(const int hole_lvl_whole, std::map<int, double> &pro_val_map);
	int get_part_min_hole_lvl(const Rune_Part_Detail& part_detail);

	bool rune_part_is_activated(const int part);

	// interfaces
	int get_rune_pro_val(std::map<int, double> &pro_val_map) const;
	int get_rune_part_lvl(Int_Int_Map &part_lvl_map) const;

	int test_runes_lvl_up(const int lvl);

	int get_rune_amount_by_lvl(const int lvl);
private:
	void init_new_part(void);
	int rune_fill_energy(Rune_Hole_Detail &hole_detail, bool is_bat, const int8_t auto_buy);
	int fill_energy_times(int &times, const Rune_Hole_Detail &hole_detail,
			 const Rune_Hole_Lvl_Cfg *hole_lvl_cfg, const Item_Detail_Config *item_material_cfg,
			 const int8_t auto_buy, int &need_copper, int &need_gold ,int &erase_material_num, int &buy_material_num);

	int set_rune_pro_val(const std::map<int, double> &pro_val_map);

private:
	Rune_Detail *rune_detail_;
};

/////////////////////////////////////////////////////////////////////////////////

inline const Rune_Detail &Rune::rune_detail(void) const{
	return *rune_detail_;
}

#endif /* RUNE_H_ */
