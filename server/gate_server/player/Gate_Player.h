/*
 * Gate_Player.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef GATE_PLAYER_H_
#define GATE_PLAYER_H_

#include "Monitor_Struct.h"
#include "Gate_Player_Base.h"
#include "Base_Detail.h"
#include "Gate_Player_Struct.h"

class Gate_Monitor;

class Gate_Player : virtual public Gate_Player_Base {
public:
	Gate_Player();
	virtual ~Gate_Player();

	void reset(void);

	Gate_Monitor *monitor(void);

	virtual Gate_Player *player_self(void);

	////////////////////////////////////////////////////////////////

	virtual int send_to_client(Block_Buffer &buf);

	virtual int send_to_logic(Block_Buffer &buf);

	virtual int send_to_scene(Block_Buffer &buf);

	virtual int send_to_local_chat(Block_Buffer &buf);

	virtual int send_to_inter_chat(Block_Buffer &buf);

	int abnormal_logout(void);

	////////////////////////////////////////////////////////////////

	int echo_test_result(std::string result, role_id_t value = 0);

	int test_command(std::string cmd_string);

	int load_detail(Block_Buffer &buf);

	int cid(void);

	bool recv_pace_off(void);

	bool battle_status(void);

	Base_Detail const &base_detail(void) const;

	std::string role_name(void);

	MSG_Detail &msg_detail(void);

	std::string &login_session(void);

	void set_login_session(std::string &session);

	virtual role_id_t role_id(void);

	virtual int level(void);

	virtual int career(void);

	Player_Monitor_Link const &monitor_link(void);

	////////////////////////////////////////////////////////////////

	void set_cid(int cid);

	void set_recv_pack_off(bool value);

	void set_battle_status(bool status);

	int set_scene_muid(Monitor_Unique_ID &muid);

	void reset_msg_detail(void);

	////////////////////////////////////////////////////////////////

	int notice_logic_sign_out(int out_reason, int abnormal_time = 5);

	int verify_msg_detail(uint32_t msg_id, uint32_t serial_cipher, uint32_t msg_time_cipher);

private:
	int cid_;
	Base_Detail base_detail_;
	Gate_Monitor *monitor_;
	Monitor_Unique_ID muid_;
	Player_Monitor_Link monitor_link_;
	MSG_Detail msg_detail_;
	std::string session_id_;

	bool recv_pace_off_;
	bool battle_status_;

	bool player_sign_out_;
};

inline int Gate_Player::cid(void) {
	return cid_;
}

inline bool Gate_Player::recv_pace_off(void) {
	return recv_pace_off_;
}

inline bool Gate_Player::battle_status(void) {
	return battle_status_;
}

inline Gate_Monitor *Gate_Player::monitor(void) {
	return monitor_;
}

inline Base_Detail const &Gate_Player::base_detail(void) const {
	return base_detail_;
}

inline Player_Monitor_Link const &Gate_Player::monitor_link(void) {
	return monitor_link_;
}

#endif /* GATE_PLAYER_H_ */
