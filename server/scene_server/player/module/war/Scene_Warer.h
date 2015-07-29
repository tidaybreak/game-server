/*
 * Scene_Warer.h
 *
 *  Created on: 2014年7月3日
 *      Author: root
 */

#ifndef SCENE_WARER_H_
#define SCENE_WARER_H_

#include "Scene_Move_Base.h"
#include "Scene_War_Struct.h"
#include "Base_Struct.h"

extern const int WAR_DIE_REVIVE_TIME;

class Scene_Warer: virtual public Scene_Move_Base {
public:
	Scene_Warer(void);
	virtual ~Scene_Warer(void);
public:
	void reset(void);
	// 地图通知登录完成时调用
	void sign_in(void);
	// 登出时调用
	void sign_out(void);

	int time_up(const Time_Value& now);

	int client_scene_complete(void);

public:
	int req_war_scene_fetch_rank(void);

	int req_war_scene_heal(void);

	int war_scene_reduce_revive_time(void);

	int send_war_scene_player_info(void);

	int war_scene_notice(int type, std::string name1, std::string name2, int score, int merit, int piece, int rep, int arena_coin = 0, bool delay = false);
	int war_scene_announce(int type, role_id_t role_id = 0, std::string name = "", int value1 = 0, int value2 = 0, int value3 = 0, bool delay = false);
	Scene_War_Rank_Info* war_info();
	void set_war_info(Scene_War_Rank_Info* info);

	int get_war_hp(void);

	void recover_war_hp(double rate);

	void war_die_revive(void);

	void check_revive(const Time_Value& now);

	bool is_safe_time(void);

	// 拉矿减速
	void add_pull_mine_buf(void);
	void del_pull_mine_buf(void);
	void clear_pull_mine_buf(void);

	// 祝福
	void add_bless_buf(void);
	void del_bless_buf(void);
	void clear_bless_buf(void);

	// 治疗减半
	void add_reduce_cure_buf(void);
	void del_reduce_cure_buf(void);
	void clear_reduce_cure_buf(void);

	// 战场逃脱
	void add_escape_buf(void);

	void forbid_enter_war(void);

	void update_mover_war_info(void);

	int add_merit_contribute_to_logic(const int add_merit, const int add_contribute);

	int add_merit_piece_to_logic(const int add_merit, const int add_piece, const int add_rep, const int add_arena_coin, std::vector<Id_Int_Int> props);

	int sync_forbid_time_to_logic(const Time_Value& time);

	void reset_war_info(void);

	void gather_revive_ball(void);
private:
	Scene_War_Rank_Info war_info_;
	std::vector<Block_Buffer> announce_buf_;
	std::vector<Block_Buffer> notice_buf_;
};



#endif /* SCENE_WARER_H_ */
