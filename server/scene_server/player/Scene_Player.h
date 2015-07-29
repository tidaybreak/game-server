/*
 * Scene_Player.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef SCENE_PLAYER_H_
#define SCENE_PLAYER_H_

#include "Monitor_Struct.h"
#include "fighter/Fighter.h"
#include "task/Scene_Tasker.h"
#include "common/Scene_Common.h"
#include "team/Scene_Teamer.h"
#include "war/Scene_Warer.h"
#include "gang/Scene_Ganger.h"
#include "campfire/Scene_Campfire.h"
#include "expedition/Scene_Expeditioner.h"

class Scene_Monitor;

struct Base_Detail;
struct Scene_Player_Detail;
struct Drops_Item;

class Scene_Player :
					 public Scene_Campfire,
					 public Scene_Warer,
					 public Scene_Tasker,
					 public Scene_Common,
					 public Fighter,
					 public Scene_Teamer,
					 public Scene_Ganger,
					 public Scene_Expeditioner {
public:
	Scene_Player();
	virtual ~Scene_Player();

	void reset(void);

	Scene_Monitor *monitor(void);

	virtual Scene_Player *player_self(void);

	int test_command(std::string cmd_string);

	int echo_test_result(std::string result, role_id_t role_id = 0, double value = -1);

	////////////////////////////////////////////////////////////////

	virtual int send_to_client(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_gate(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_logic(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_local_chat(Block_Buffer &buf, bool use_swap = false);

	virtual int send_err_to_client(int error_id, int source);

	////////////////////////////////////////////////////////////////

	virtual int time_up(const Time_Value &now);

	int sign_in(void);

	int sign_out(bool quiet = false, bool offline_exit = true);

	int load_detail(Block_Buffer &buf);

	int module_init(void);

	int scene_init(void);

	int login_scene_exist(int &des_scene);

	int move_scene_completed(void);

	int battle_scene_completed(bool completed = true);

	int get_role_info(void);

	// 场景转换(内部调用)
	int inner_transmit(int des_scene, int scene_order, int64_t arg3, std::string arg4 = "", Coord coord = Coord(0,0));

	////////////////////////////////////////////////////////////////

	virtual role_id_t role_id(void);

	virtual int level(void);

	virtual std::string role_name(void);

	virtual int career(void);

	virtual int vip(void);

	Base_Detail const &base_detail(void) const;

	Player_Monitor_Link const &monitor_link(void) const;

	int set_scene_muid(Monitor_Unique_ID &muid);

	int set_create_scene_args(int type, Time_Value time, role_id_t id, int v1, int v2, int v3, bool v4 = false);

	////////////////////////////////////////////////////////////////

	virtual int enter_move_scene(Move_Scene *scene);

	virtual int exit_move_scene(bool offline_exit = true);

	virtual int move_appear_info(Block_Buffer &buf);

	virtual int make_move_info(Block_Buffer &buf, uint8_t toward, uint16_t x, uint16_t y);

	virtual int move_appear(void);

	virtual int move_disappear(void);

	////////////////////////////////////////////////////////////////

	virtual int battle_appear_info(Block_Buffer &buf);

	virtual int battle_appear(void);

	virtual int battle_disappear(void);

	virtual int enter_battle_scene(Battle_Scene *scene);
	int enter_battle_scene_team_member(Battle_Scene *scene);

	virtual int exit_battle_scene(int exit_from = 0);

	void player_pos_key_fit_hero(Position_Key &player_pos_key, Position_Key &hero_pos_key);

	int hero_battle_birth(void);

	////////////////////////////////////////////////////////////////
	//是否在移动场景（备注：玩家加载完成才算在移动场景）
	virtual bool is_in_mover_scene(void);

	virtual bool can_fight(void);

	virtual int move_able(void);

	//////////////////////////////////////////////////////////////

	void sync_mover_detail_to_logic(void);

	void sync_fighter_detail_to_logic(void);

	///////////////////////////////////////////////////////////////

	// 其它接口尽量加到Scene_Common
	int notify_gate_exit_battle(int from = 0);

	//侦听进入战斗
	int hook_player_enter_battle(void);
	//返回出生点
	void back_to_revive_point(void);
	// 没有收益
	bool is_no_profit(void);
	void set_no_profit(bool value);

	int set_role_name(const std::string &new_name);
	int modify_gender(const uint8_t gender);

	int set_gang_id(const role_id_t gang_id);
	int set_gang_name(const std::string &gang_name);

	void set_mount_ride(const int ride);
	void set_mount_horse_model_id(const int id);

	void set_rela_scene_id(const int id);

	int set_title(const int title_id);

	int set_vip_info_change(uint32_t vip_level, int64_t vip_over_time);

	role_id_t gang_id(void);
	std::string zone(void);

	//送花系统,魅力值
	void add_charm(uint32_t num);
	void set_head_id(uint32_t id);
	void set_avatar(Int_Vec &avatar_vec);

	int hook_scene_change(int src_scene, int des_scene);

	bool btl_scene_complate(void);

	void force_end_battle_scene(void);

	void force_finish_battle_scene(void);

	void call_ann_battle_in_bonfire_scene(Role_Vec& loser_vec, int scene_id);

	// 是否战斗托管
	bool is_system_hosting(void);

	// // 通知逻辑服进入场景
	void notice_enter_scene(void);
private:
	Scene_Monitor *monitor_;
	Player_Monitor_Link monitor_link_;

	Base_Detail *base_detail_;
	Scene_Player_Detail *detail_ptr_;

	bool move_scene_completed_;
	bool battle_scene_completed_;
	bool player_has_init_;				// 是否同步到场景完成,初始化属性判断用
	bool is_no_profit_;					// 没收益
	bool is_hosting_;					// 是否状态托管
	std::string zone_;

};

inline Scene_Monitor *Scene_Player::monitor(void) {
	return monitor_;
}

inline Scene_Player *Scene_Player::player_self(void) {
	return this;
}

inline Base_Detail const &Scene_Player::base_detail(void) const {
	return *base_detail_;
}

inline Player_Monitor_Link const &Scene_Player::monitor_link(void) const {
	return monitor_link_;
}

inline bool Scene_Player::btl_scene_complate(void) {
	return battle_scene_completed_;
}

#endif /* SCENE_PLAYER_H_ */
