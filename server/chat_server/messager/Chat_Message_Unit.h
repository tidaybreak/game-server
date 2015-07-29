/*
 * Chat_Message_Unit.h
 *
 *  Created on: Aug 2, 2012
 *      Chator: Ti
 */

#ifndef CHAT_MESSAGER_UNIT_H_
#define CHAT_MESSAGER_UNIT_H_

#include "Message_Unit.h"

class Chat_Monitor;
class Chat_Player;

class Chat_Message_Unit : public Message_Unit {
public:
	typedef std::function<int(Chat_Player&, Block_Buffer&)> Client_Func;
	typedef boost::unordered_map<uint32_t, Client_Func> Client_Func_Map;

	typedef std::function<int(int, role_id_t, Chat_Player*, Block_Buffer&)> Server_Func;
	typedef boost::unordered_map<uint32_t, Server_Func> Server_Func_Map;

	Chat_Message_Unit(void);

	virtual ~Chat_Message_Unit(void);

	Chat_Monitor *monitor(void);

	virtual int register_client_message(void);

	virtual int register_inner_message(void);

	int register_client_process(const uint32_t msg_id, Client_Func callback);

	int process_client_message(Chat_Player &player, const uint32_t msg_id, Block_Buffer &buf);

	int register_server_process(const uint32_t msg_id, Server_Func callback);

	int process_server_message(int cid, role_id_t role_id, Chat_Player *player, const uint32_t msg_id, Block_Buffer &buf);

	virtual int process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher);

    virtual int process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf);

    virtual int process_async_buff(uint32_t msg_id, Block_Buffer &buf);

    virtual int process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now);

private:
	Client_Func_Map client_message_func_;
	Server_Func_Map server_message_func_;
};

#endif /* CHAT_MESSAGER_UNIT_H_ */
