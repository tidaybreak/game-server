/*
 * Rune_Stone.h
 *
 *  Created on: Aug 5, 2014
 *      Author: chenyulin
 */

#ifndef RUNE_STONE_H_
#define RUNE_STONE_H_

#include "Logic_Player_Base.h"

struct Rune_Stone_Detail;

class Rune_Stone : virtual public Packer {
public:
	Rune_Stone(void);
	virtual ~Rune_Stone();
	void reset(void);

	static int create_init(Rune_Stone_Detail &detail);

	void load_detail(Rune_Stone_Detail &detail);
	void save_detail(Rune_Stone_Detail &detail);

	void sign_in(void);
	void sign_out(void);

	void module_init(void);
	int client_ready_now(void);

	int trigger_daily_zero(const Time_Value &now);

	inline const Rune_Stone_Detail &rune_stone_detail(void) const;

//	// MSG start ===================================
	int req_rune_stone_syst_info(void);
	int req_rune_stone_smelt(const int8_t smelter_idx, const int8_t is_bat);
	int req_rune_stone_sell(const int8_t is_bat, const int under_color, const int index);
	int req_rune_stone_pick(const int8_t is_bat, const int index);
	int req_rune_stone_lock(const int index, const int8_t is_lock);
	int req_rune_stone_merge(const int8_t is_bat, const int index_type, const int index_from, const int index_to);
	int req_rune_stone_move(const int index_from, const int index_to);
	int req_rune_stone_exchange(const int item_id);
	int req_rune_stone_essence_exchange(const int item_id);
	int req_points(void);
	int req_essence(void);
	int req_rune_stone_on_off(const int index_from, const int index_to);

	void active_points(void);
	void active_essence(void);
	void active_rune_stone_addit_info(const Item_Detail *item);
	// MSG end =====================================

	int rune_stone_prop_refresh(void);
	int exp_lvl_prop_refresh(const Item_Detail_Config *item_cfg, const int exp_lvl, std::map<int, double> &pro_val_map);
	int combine_prop_refresh(const std::map<int, int> &combine_cnt_map, std::map<int, double> &pro_val_map);

	void update_rune_stone_equip_cap(void);
	void update_rune_stone_hero_equip_cap(void);

	int gm_add_points(const int points);

	bool has_put_on_same_rune_type(const int pack_type, const int rune_type);

	// interface
	int get_rune_stone_prop(std::map<int, double> &pro_val_map) const;
	int active_pack_rune_stone(void);

	//获取符石品质对应数量的map
	void get_rune_quality_num_map(std::map<int8_t, uint32_t> &rune_quality_num_map);

private:
	void init_smelters(void);
	static void init_smelters(Rune_Stone_Detail &detail);
	void init_player_all_pack_cap(void);

	int update_exp_lvl(Item_Detail *item);

private:
	Rune_Stone_Detail *rune_stone_detail_;
};

/////////////////////////////////////////////////////////////////////////////////

inline const Rune_Stone_Detail &Rune_Stone::rune_stone_detail(void) const{
	return *rune_stone_detail_;
}

#endif /* RUNE_STONE_H_ */
