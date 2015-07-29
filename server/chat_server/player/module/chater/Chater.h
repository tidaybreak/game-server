/*
 * Chater.h
 *
 *  Created on: May 8, 2014
 *      Author: caijianwei
 */

#ifndef CHATER_H_
#define CHATER_H_

#include "Chat_Player_Base.h"
#include "Msg_Struct.h"
#include "Chater_Def.h"

typedef boost::unordered_map<int64_t, std::vector<Chat_Record>> Role_Id_Chat_Record_Map;

class Chater : public Chat_Player_Base {
public:

	Chater();
	virtual ~Chater();
	void reset();
	void module_init(void);

	int client_ready_now(void);
	//客户端发
	int req_chat(int8_t district, int8_t type, int64_t target, std::string &target_name, std::vector<Chat_Content>& contents);

	//输入状态
	int send_input_status(role_id_t target, int8_t status);

	//聊天记录
	int req_history_private_chat_record(role_id_t role_id);
	int req_history_gang_chat_record();
	void save_or_update_target_chat_record_map(role_id_t target, std::vector<Chat_Record>& chat_record_vec);

	//小喇叭
	int chat_in_bugle(const std::vector<Chat_Content>& contents);

private:
	int broadcast_contents(int8_t district, int32_t type, const std::vector<Chat_Content>& contents);
	//人物发送聊天
	int chat_in_world(const std::vector<Chat_Content>& contents);
	int chat_in_horn(const std::vector<Chat_Content>& contents);
	int chat_in_gang(const std::vector<Chat_Content>& contents);
	int chat_in_private(int64_t target, const std::vector<Chat_Content>& contents);
	int chat_in_team(const std::vector<Chat_Content>& contents);

	bool chat_too_fast();

	Time_Value last_world_chat_time;
	Role_Id_Chat_Record_Map target_chat_record_map;
	bool send_world_chat_to_logic;
};

#endif /* CHATER_H_ */
