/*
 * Chat_Player.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef CHAT_PLAYER_H_
#define CHAT_PLAYER_H_

#include "Monitor_Struct.h"
#include "Base_Detail.h"
#include "module/chater/Chater.h"
#include "module/friend/Chat_Friend.h"
#include "module/team/Chat_Teamer.h"

class Chat_Monitor;
struct Chat_Content;
struct Chat_Record;

class Chat_Player : public Chater,
					public Chat_Friend,
					public Chat_Teamer {
public:
	Chat_Player();
	virtual ~Chat_Player();

	void reset(void);

	Chat_Monitor *monitor(void);

	virtual Chat_Player *player_self(void);
	int test_command(std::string cmd_string) ;

	Player_Monitor_Link const &monitor_link(void);

	////////////////////////////////////////////////////////////////

	virtual int send_to_client(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_gate(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_logic(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_scene(Block_Buffer &buf, bool use_swap = false);

	virtual int send_err_to_client(int error_id, int source);

	////////////////////////////////////////////////////////////////

	void sign_out(void);

	void load_detail(Block_Buffer &buf);

	void module_init(void);
	int client_ready_now(void);

	int system_chat(std::vector<Chat_Content *> &contents);
	////////////////////////////////////////////////////////////////

	virtual role_id_t role_id(void);

	virtual uint8_t  gender(void);

	virtual int level(void);

	virtual int career(void);

	std::string role_name(void);

	int echo_test_result(std::string result, role_id_t value = -1);

	int vip(void);
	void set_vip(int32_t over_time, int32_t vip_level);
	void set_level(int32_t level);

	void sign_in(void);

	Base_Detail const &base_detail(void) const;
	void set_base_detail(const Base_Detail& detail);

	void update_gang_info(const int64_t gang_id, const std::string &gang_name);
	////////////////////////////////////////////////////////////////

	int set_scene_muid(Monitor_Unique_ID &muid);

	int set_role_name(const std::string &new_name);

	int modify_gender(const uint8_t gender);
	//////////////////////////////////////////////////////////////
//	bool check_chat_frequency(uint8_t chat_type);
//	void set_chat_time(uint8_t chat_type);

	/////////////////////////////////////////////////////////////
	int broadcast_buf(Block_Buffer &buf);

private:
	Chat_Monitor *monitor_;
	Player_Monitor_Link monitor_link_;

	Base_Detail base_detail_;



	//聊天间隔时间
	//std::vector<Time_Value> channel_limit_time;
	//std::vector<Time_Value> channel_last_chat_time;
};

inline Chat_Monitor *Chat_Player::monitor(void) {
	return monitor_;
}

inline Chat_Player *Chat_Player::player_self(void) {
	return this;
}

inline Player_Monitor_Link const &Chat_Player::monitor_link(void) {
	return monitor_link_;
}

inline Base_Detail const &Chat_Player::base_detail(void) const {
	return base_detail_;
}

inline void Chat_Player::set_base_detail(const Base_Detail& detail) {
	base_detail_ = detail;
}

inline void Chat_Player::update_gang_info(const int64_t gang_id, const std::string &gang_name) {
	base_detail_.gang_id = gang_id;
	base_detail_.gang_name = gang_name;
}

#endif /* CHAT_PLAYER_H_ */
