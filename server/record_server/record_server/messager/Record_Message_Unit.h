/*
 * Record_Message_Unit.h
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#ifndef RECORD_MESSAGER_UNIT_H_
#define RECORD_MESSAGER_UNIT_H_

#include "Message_Unit.h"

class Record_Message_Unit : public Message_Unit {
public:

	Record_Message_Unit(void);

	virtual ~Record_Message_Unit(void);

	virtual int register_client_message(void);

	virtual int register_inner_message(void);

    virtual int process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf);

    virtual int process_async_buff(uint32_t msg_id, Block_Buffer &buf);

    virtual int process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now);

private:

};

#endif /* RECORD_MESSAGER_UNIT_H_ */
