/*
 * Daemon_Message_Unit.h
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#ifndef DAEMON_MESSAGER_UNIT_H_
#define DAEMON_MESSAGER_UNIT_H_

#include "Message_Unit.h"

class Daemon_Monitor;

class Daemon_Message_Unit : public Message_Unit {
public:

	Daemon_Message_Unit(void);

	virtual ~Daemon_Message_Unit(void);

	virtual int process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher);

    virtual int process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf);

    virtual int process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now);

    virtual int process_async_data(int data_type, void *ptr_data);

    virtual int process_async_buff(uint32_t msg_id, Block_Buffer &buf);

	virtual int recover_ptr_body(Unit_Message *ptr_data);

	virtual void show_server_status(void);


private:

};

#endif /* DAEMON_MESSAGER_UNIT_H_ */
