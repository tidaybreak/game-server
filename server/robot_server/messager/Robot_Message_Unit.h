/*
 * Robot_Message_Unit.h
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#ifndef ROBOT_MESSAGER_UNIT_H_
#define ROBOT_MESSAGER_UNIT_H_

#include "Message_Unit.h"

class Robot_Monitor;
class Robot_Player;

class Robot_Message_Unit : public Message_Unit {
public:
	typedef std::function<int(Robot_Player&, Block_Buffer&)> Client_Func;
	typedef boost::unordered_map<uint32_t, Client_Func> Client_Func_Map;

	Robot_Message_Unit(void);

	virtual ~Robot_Message_Unit(void);

	Robot_Monitor *monitor(void);

	virtual int register_client_message(void);

	virtual int register_inner_message(void);

	virtual int process_inner_message(const uint32_t msg_id, const int32_t cid, Block_Buffer &buf);

	int register_client_process(const uint32_t msg_id, Client_Func callback);

	int process_client_message(Robot_Player &player, const uint32_t msg_id, Block_Buffer &buf);

    virtual int process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher);

    virtual int process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf);

    virtual int process_async_buff(uint32_t msg_id, Block_Buffer &buf);

    virtual int process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now);


private:
	Client_Func_Map client_message_func_;

};

#endif /* ROBOT_MESSAGER_UNIT_H_ */
