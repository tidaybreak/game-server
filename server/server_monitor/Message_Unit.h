/*
 * Message_Unit.h
 *
 * Created on: 2012-12-29 14:26
 *     Author: Ti
 */

#ifndef _MESSAGE_UNIT_H_
#define _MESSAGE_UNIT_H_

#include "Thread.h"
#include <functional>
#include "Monitor_Struct.h"
#include "Object_Pool.h"
#include "Record_Client.h"

class Server_Monitor;

// 说明：func函数名, _Object指针, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3表示回调函数的三个参数
#define SCT_CALLBACK_1(func, _Object) std::bind(&func,_Object, std::placeholders::_1)
#define SCT_CALLBACK_2(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2)
#define SCT_CALLBACK_3(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define SCT_CALLBACK_4(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

class Message_Unit : public Thread {
public:
	typedef Object_Pool_Ex<Unit_Message> Unit_Message_Pool;
	typedef Blocking_Queue<Unit_Message *> Unit_Message_DQ;

	typedef std::function<int(uint32_t, Block_Buffer&)> Inner_Func;	// 接收单个协议，@socket标识,@数据包
	typedef boost::unordered_map<uint32_t, Inner_Func> Inner_Func_Map;

	Message_Unit(void);

	virtual ~Message_Unit(void);

    virtual void run_handler(void);

    void set_monitor(Server_Monitor *monitor);

    Server_Monitor *monitor(void);

    void server_status(void);

    bool is_running(void);

	virtual void show_server_status(void);

    virtual int register_client_message(void);

    virtual int register_inner_message(void);

    int process_ptr_list(void);

    bool message_unit_busy(void);

	Block_Buffer *pop_block(int cid);

	int push_block(int cid, Block_Buffer *buf);

    ////////////////////////////////////////////////////////////////////////////////////////////

    int process_client_buffer(Block_Buffer &buf);

    int process_server_buffer(Block_Buffer &buf);

    int process_timer_buffer(Block_Buffer &buf);


    virtual int process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher);

    virtual int process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf);

    virtual int process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now);

    virtual int process_async_data(int data_type, void *ptr_data);

    virtual int process_async_buff(uint32_t msg_id, Block_Buffer &buf);

	virtual int process_inner_message(const uint32_t msg_id, const int32_t cid, Block_Buffer &buf);

	virtual int process_config_hook(std::string &module, bool restart);

    ////////////////////////////////////////////////////////////////////////////////////////////

	virtual int push_ptr_data(uint32_t data_type, void *ptr_data, uint32_t unit_type, uint32_t pool_gid = 0);

	int recover_ptr_shell(Unit_Message *ptr_data);

	virtual int recover_ptr_body(Unit_Message *ptr_data);

    ////////////////////////////////////////////////////////////////////////////////////////////

    void remake_buffer(int msg_id, role_id_t role_id, Block_Buffer &buf);

    void process_link_close(Link_Drop &link_drop);

    virtual void count_process_time(int msg_id);

	void show_process_time(void);

	int register_inner_process(const uint32_t msg_id, Inner_Func callback);


protected:
    bool is_running_;

    uint message_unit_max_limit_;
    timespec pause_ts_;
	Server_Monitor *monitor_;

	Unit_Message_DQ data_ptr_list_;
	Time_Value data_pro_tick_;
	Unit_Message_Pool unit_msg_pool_;
	Block_Pool_Group block_group_pool_;

	Time_Value msg_tick_;
	Message_Stat_Map msg_stat_;

protected:
	Inner_Func_Map inner_message_func_;
};

inline Block_Buffer *Message_Unit::pop_block(int cid) {
	return block_group_pool_.pop_block(cid);
}

inline Server_Monitor *Message_Unit::monitor(void) {
	return monitor_;
}

inline int Message_Unit::push_ptr_data(uint32_t data_type, void *ptr_data, uint32_t unit_type, uint32_t pool_gid) {
    Unit_Message *un_msg = 0;
    if ((un_msg = unit_msg_pool_.pop()) == 0) {
    	LOG_USER("pop unit msg error type:%d", data_type);
        return -1;
    }

    un_msg->data_type = data_type;
    un_msg->ptr_data = ptr_data;
    un_msg->unit_type = unit_type;
    un_msg->pool_gid = pool_gid;

	return data_ptr_list_.push(un_msg);
}

inline int Message_Unit::push_block(int cid, Block_Buffer *buf) {
	return block_group_pool_.push_block(cid, buf);
}

inline void Message_Unit::remake_buffer(int msg_id, role_id_t role_id, Block_Buffer &buf) {
	uint32_t len = buf.readable_bytes() + sizeof(msg_id) + sizeof(role_id);
	int wri_idx = buf.get_write_idx();
	int new_idx = buf.get_read_idx() - sizeof(len) - sizeof(msg_id) - sizeof(role_id);
	buf.set_read_idx(new_idx);
	buf.set_write_idx(new_idx);
	buf.write_uint32(len);
	buf.write_uint32(msg_id);
	buf.write_int64(role_id);
	buf.set_write_idx(wri_idx);
}

#endif //_MESSAGE_UNIT_H_
