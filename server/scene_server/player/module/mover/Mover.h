/*
 * Mover.h
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 *
 *      新模块例子
 */

#ifndef MOVER_H_
#define MOVER_H_

#include "Scene_Move_Base.h"

class Move_Scene;
class Battle_Scene;
struct Mover_Detail;
struct Position_Key;
struct Appear_Addition;

class Mover : virtual public Scene_Move_Base {
public:
	enum Mover_Scene_Type {
		SCENE_ALL = 0,
		SCENE_MOVE = 1,			// 移动场景
		SCENE_BATTLE = 2,		// 战斗场景
	};

	typedef std::vector<Appear_Addition> Appear_Prop_Vec;

	Mover();
	virtual ~Mover();
	void reset(void);

	void set_detail_prt(Mover_Detail &detail);

	// 登录时module初始化
	void module_init(void);
	// 地图通知登录完成时调用
	void sign_in(void);
	// 登出时调用
	void sign_out(void);

	virtual void fight_tick(const Time_Value &now);

	/////////////////////////////////////////////////////////////////////////////////////

	virtual Move_Scene *move_scene(void);

	Battle_Scene *battle_scene(void);

	Block_Buffer &appear_buff(void);

	Appear_Prop_Vec &addition_prop(void);

	const Mover_Detail *mover_detail_ptr (void) const;

	const Mover_Detail &mover_detail (void) const;

	const Position_Key &battle_position(void);

	const Position_Key &cur_battle_position(void);

	const Position_Key &team_position(void);

	int mover_type(void);

	const Coord &grid_coord(void);

	const Coord &pixel_coord(void);

	int move_scene_id(void);

	int move_scene_order(void);

	int battle_scene_id(void);

	void set_block_index(int idx);

	void set_layer_id(int id);

	void set_toward(int toward);

	void set_grid_coord(uint16_t x, uint16_t y, bool reset_idx = false);

	void set_pixel_coord(uint16_t x, uint16_t y);

	int set_battle_position(int pos, int idx, Battle_Scene *battle_scene);

	void set_cur_battle_position(int pos, int idx);

	void set_team_position(int pos, int idx);

	int scene_broad_around(Block_Buffer &buf, Mover_Scene_Type scene_type = SCENE_ALL);

	//////////////////////////////////////////////////////////////////////////////////////

	void add_moveable_value(int16_t value);

	void del_moveable_value(int16_t value);

	void clear_moveable_value(void);

	/////////////////////////////////////////////////////////////////////////////////////

	virtual int enter_move_scene(Move_Scene *scene);

	virtual int exit_move_scene(bool offline_exit = true);

	virtual int move_appear(void);

	virtual int move_disappear(void);

	virtual int move_appear_info(Block_Buffer &buf);

	virtual int make_disappear_info(Block_Buffer &buf);

	virtual int make_move_info(Block_Buffer &buf, uint8_t toward, uint16_t x, uint16_t y);

	/////////////////////////////////////////////////////////////////////////////////////

	virtual int enter_battle_scene(Battle_Scene *scene);

	virtual int exit_battle_scene(int exit_from = 0);

	virtual int battle_appear(void);

	virtual int battle_disappear(void);

	virtual int battle_appear_info(Block_Buffer &buf);

	/////////////////////////////////////////////////////////////////////////////////////

	int move(Coord_Vec &coord);

	int reset_coord(void);

	virtual bool is_in_battle(void);

	virtual bool is_in_mover_scene(void);

	virtual bool can_fight(void);

	virtual int move_able(void);

	bool validate_mover_scene(void);

	bool validate_battle_scene(void);

	int validate_move_value(uint16_t &x, uint16_t &y);

	int change_coord_by_pixel(uint16_t &x, uint16_t &y);

	bool is_adjacent_coord(uint16_t &x, uint16_t &y);

	/////////////////////////////////////////////////////////////////////////////////////

	Mover *find_mover_in_move_scene(role_id_t role_id);

	Fighter *find_fighter_in_battle_scene(role_id_t role_id);

	// 场景通关侦听
	virtual void hook_scene_pass(int scene_id);

	//
	int set_addition(Appear_Addition &addition);

	int get_addition(Appear_Addition &addition);

	int del_addition(int addition_type);

	bool has_addition(int addition_type);

	bool is_player(void);

	bool is_hero(void);

	bool is_normal_monster(void);

	bool is_boss(void);

private:
	Mover_Detail *mover_detail_;
	Move_Scene *move_scene_;		// 移动场景
	Battle_Scene *battle_scene_;	// 战斗场景
	Block_Buffer appear_buff_;		// 出现缓存

	Appear_Prop_Vec addition_prop_;

public:
	std::string debug_str_reset_;		// core debug
	std::string debug_str_disappear_;	// core debug
};

inline Move_Scene *Mover::move_scene(void) {
	return move_scene_;
}

inline Battle_Scene *Mover::battle_scene(void) {
	return battle_scene_;
}

inline Block_Buffer &Mover::appear_buff(void) {
	return appear_buff_;
}

inline Mover::Appear_Prop_Vec &Mover::addition_prop(void) {
	return addition_prop_;
}

inline bool Mover::is_in_battle(void) {
	return battle_scene_;
}

inline bool Mover::is_in_mover_scene(void){
	return move_scene_;
}

inline bool Mover::can_fight(void){
	return true;
}

inline const Mover_Detail *Mover::mover_detail_ptr(void) const {
	return mover_detail_;
}

inline const Mover_Detail &Mover::mover_detail(void) const {
	return *mover_detail_;
}

#endif /* MOVER_H_ */
