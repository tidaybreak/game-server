/*
 * Common.h
 *
 *  Created on: Apr 11, 2014
 *      Author: Linqiyou
 */

#ifndef SCENE_COMMON_H_
#define SCENE_COMMON_H_

#include "Scene_Move_Base.h"

struct Drops_Item;
class Material;
class Machine;
struct MSG_10210001;
struct MSG_10210101;
struct MSG_10210100;
struct MSG_10210103;
struct MSG_10210200;
struct MSG_10210104;
struct Chat_Content;

class Scene_Common : virtual public Scene_Move_Base {
public:

	Scene_Common();
	virtual ~Scene_Common();

	// 登录时detail赋值
	void load_public_detail(Block_Buffer &buf);

	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

    void reset(void);
	int time_up(const Time_Value& now);
	bool is_revive(void);
	void set_revive_info(bool is_revive, Time_Value revive_time);
	void reset_revive_info(void);
    int gather_goods_result(role_id_t role_id, int result);
	int get_drops_goods(MSG_10210001& msg);
	int start_gather(MSG_10210100& msg);
	int stop_gather(MSG_10210101& msg);
	int stop_gather(void);
	int start_gather_material(Material* material);
	int stop_gather_material(Material* material);
	int open_machine(MSG_10210103& msg);
	int start_gather_machine(Machine* machine);
	int stop_gather_machine(Machine* machine);
	int add_drops_to_package(const int scene_type, const int scene_id, std::vector<Drops_Item>& drops_item);
	int play_plot(std::string plot_id, int record_id, bool is_send_to_client = true);
	int begin_plot(std::string plot_id);
	int end_plot(std::string plot_id);
	void notify_end_plot(void);
	bool plot_status(void);
	void set_battle_plot_data(std::string plot_id, int plot_step, int record_id);
	int notify_gate_exit_battle(void);
	bool is_in_plot();

	int create_battle_monster(MSG_10210200& msg);

	int beat_back(MSG_10210104& msg);

	int pick_up_goods(std::vector<role_id_t> role_id_vec);

	int req_refresh_scene_point(int point_id);

	int req_fight_scene_point(int point_id);

	int req_take_lift_finish(role_id_t role_id);

	bool is_take_lift(void);
	void set_take_lift_begin(void);
	void set_take_lift_finish(void);

	int inner_send_bless_buf(role_id_t sender_role_id, int32_t bless_buf_id);

	int req_dungeon_panel_close(void);

	int req_single_dungeon_revive(int8_t type);

	int req_scene_chat(std::vector<Chat_Content> &content, int32_t sys_type = 0);

	// 坐骑骑乘请求
	int sync_mount_ride(int ride, int horse_model_id, int horse_speed);
	void broadcast_avatar_to_client(void);

	int req_world_boss_act_guild_bless(int8_t icon_id);
	int req_world_boss_use_ability(int8_t icon_id, int type);
	void world_boss_add_power_up_buf(const int8_t icon_id);
	void world_boss_del_power_up_buf(const int8_t icon_id);
	void world_boss_clear_power_up_buf(const int8_t icon_id);
	void world_boss_add_leader_buf(const int8_t icon_id);
	void world_boss_del_leader_buf(const int8_t icon_id);
	void world_boss_clear_leader_buf(const int8_t icon_id);

	// ==       荣誉竞技场     ==//
	int req_honor_arena_match(void);
	int req_honor_arena_receive(const int type);
	int req_honor_arena_rank(void);
	int req_honor_arena_panel_info(void);
	int req_honor_arena_set_auto_match(const int8_t is_auto);
	//==         荣誉竞技场   ==//
	//========   流浪商人     ==========//
	int req_vagrant_item_info(const int64_t monster_role_id);
	int req_vagrant_buy_item(const int64_t monster_role_id, const int item_index);
	//========    流浪商人    ==========//

	int req_fetch_tbay_info(void);
	int req_enter_scene_outpost(const int64_t monster_role_id);
	int req_tbay_bless(const int8_t bless_index);

	int req_attck_rob_match_member(int64_t to_uuid, std::string to_server_name,
								   int64_t uuid, int64_t role_id, std::string server_name, std::string role_name, uint32_t coin);
	int req_accept_fight_with_robber(int64_t uuid, int64_t role_id);
	int req_gain_rob_production(int64_t to_uuid, int64_t to_role_id, int64_t uuid, int64_t role_id,
							    uint32_t building_id, uint8_t building_type);
	bool get_dragon_vale_rob();
	void set_dragon_vale_rob(bool flag);
	Time_Value& get_begin_rob_count_down();
	void set_begin_rob_count_down(const Time_Value &now);
	// ****life skill****
	int try_sub_energy(const uint16_t energy);
	int sub_energy(const uint16_t energy,const bool check = false);
	void set_energy(const uint16_t energy);
	void set_life_skill(const int skill_id);
    int spar_gather(const int id,Item_Info_Vec &item_vec);
    int spar_gather_cost(const int id);

protected:
	bool is_revive_;
	Time_Value revive_time_;
	std::string battle_plot_id_;
	int plot_step_;
	role_id_t plot_record_id_;
	bool plot_status_;
	bool is_take_lift_;

	bool is_drgaon_vale_rob_;
	Time_Value begin_rob_count_down_;
	uint16_t energy_;
	int life_skill_id_;               // 生活技能id,采集技能

	Role_Hash_Set material_gather_set_;
};

inline bool Scene_Common::is_in_plot(void){
	return plot_status_;
}
#endif /* SCENE_COMMON_H_ */
