/*
 * Robot_Player.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef ROBOT_PLAYER_H_
#define ROBOT_PLAYER_H_

#include "Robot_Player_Base.h"
#include "Robot_Struct.h"
#include "wayfind/Wayfind.h"
#include "chat/Chat.h"
#include "randmsg/RandMsg.h"
#include "task/Robot_Task.h"

class Robot_Monitor;

class Robot_Player : virtual public Robot_Player_Base,
					 public Wayfind,
					 public Chat,
					 public RandMsg,
					 public Robot_Task {
public:
	Robot_Player();
	virtual ~Robot_Player();

	void reset(void);

	Robot_Monitor *monitor(void);

	virtual Robot_Player *player_self(void);

	////////////////////////////////////////////////////////////////

	virtual int send_to_server(Block_Buffer &buf);

	////////////////////////////////////////////////////////////////

	int load_detail(Block_Buffer &buf);

	int cid(void);

	Robot_Detail &base_detail(void);

	Client_MSG_Info &msg_detail(void);

	std::string role_name(void);

	virtual role_id_t role_id(void);

	virtual int level(void);

	virtual int career(void);


	////////////////////////////////////////////////////////////////

	void set_cid(int cid);
	void set_login_logic_cid(int cid);

	void set_recv_pack_off(bool value);

	void set_battle_status(bool status);

	////////////////////////////////////////////////////////////////

	int notice_logic_sign_out(int out_reason);

	int verify_msg_detail(uint32_t serial_cipher, uint32_t msg_time_cipher);

	int tick(Time_Value &now);

	int rand_move(void);

	int ai_task(Time_Value &now);

	int player_init(void);

	int transfer_scene(void);

	void handle_clear_monster_point(void);

	int npc_appear(const NPC_Appear_Info &appear_info);
	int npc_disappear(Role_Vec &role_id_vec);
	inline const std::map<role_id_t, NPC_Appear_Info>& npc_appear_map(void);

	bool in_main_scene(void);
	int exit_diff_space(void);
	void gm_finish_task(const int task_id);
	int set_path_find_status(void);
	int path_find_move(void);
private:
	int tick_logined(Time_Value &now);
	int move(void);

	int ai_task_visible(void);
	int ai_task_acceptable(void);
	int ai_task_accepted(void);
	int ai_task_finish(void);
	int ai_task_submit(void);

	int task_exec_collect(const Task_Mission_Detail_Config *task_cfg);
	int task_exec_fb(const Task_Mission_Detail_Config *task_cfg);
	int task_exec_arrive(const Task_Mission_Detail_Config *task_cfg);

	void gm_level(const int level);
	void gm_modify_prop(void);

	bool reach_dest_grid(void);

private:
	int cid_;				// gate_cid;
	int login_logic_cid_;
	Robot_Detail base_detail_;
	Robot_Monitor *monitor_;
	Client_MSG_Info msg_detail_;
	Time_Value tick_;
	std::map<role_id_t, NPC_Appear_Info> npc_appear_map_;

};

inline int Robot_Player::cid(void) {
	return cid_;
}


inline Robot_Monitor *Robot_Player::monitor(void) {
	return monitor_;
}

inline Robot_Detail &Robot_Player::base_detail(void) {
	return base_detail_;
}

inline Client_MSG_Info &Robot_Player::msg_detail(void) {
	return msg_detail_;
}

inline const std::map<role_id_t, NPC_Appear_Info>& Robot_Player::npc_appear_map(void) {
	return npc_appear_map_;
}

#endif /* ROBOT_PLAYER_H_ */
