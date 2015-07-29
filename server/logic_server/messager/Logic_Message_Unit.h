/*
 * Logic_Message_Unit.h
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#ifndef LOGIC_MESSAGER_UNIT_H_
#define LOGIC_MESSAGER_UNIT_H_

#include "Message_Unit.h"

class Logic_Monitor;
class Logic_Player;

class Logic_Message_Unit : public Message_Unit {
public:
	typedef std::function<int(Logic_Player&, Block_Buffer&)> Client_Func;
	typedef boost::unordered_map<uint32_t, Client_Func> Client_Func_Map;

	typedef std::function<int(int, role_id_t, Logic_Player*, Block_Buffer&)> Server_Func;
	typedef boost::unordered_map<uint32_t, Server_Func> Server_Func_Map;


	Logic_Message_Unit(void);

	virtual ~Logic_Message_Unit(void);

	Logic_Monitor *monitor(void);

	virtual int register_client_message(void);

	virtual int register_inner_message(void);

	int register_client_process(const uint32_t msg_id, Client_Func callback);

	int process_client_message(Logic_Player &player, const uint32_t msg_id, Block_Buffer &buf);

	int register_server_process(const uint32_t msg_id, Server_Func callback);

	int process_server_message(int cid, role_id_t role_id, Logic_Player *player, const uint32_t msg_id, Block_Buffer &buf);

	virtual int process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher);

    virtual int process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf);

    virtual int process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now);

    virtual int process_async_data(int data_type, void *ptr_data);

    virtual int process_async_buff(uint32_t msg_id, Block_Buffer &buf);

	virtual int recover_ptr_body(Unit_Message *ptr_data);

	virtual void show_server_status(void);

	virtual int process_config_hook(std::string &module, bool restart);


private:
	Client_Func_Map client_message_func_;
	Server_Func_Map server_message_func_;
};

#endif /* LOGIC_MESSAGER_UNIT_H_ */
