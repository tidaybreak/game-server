/*
 * Service_Monitor.h
 *
 *  Created on: Nov 2, 2012
 *      Author: Ti
 */

#ifndef SERVICE_MONITOR_H_
#define SERVICE_MONITOR_H_

#include "Acceptor.h"
#include "Connector.h"
#include "Receiver.h"
#include "Sender.h"
#include "Stream_Packer.h"
#include "Svc.h"
#include "Svc_Static_List.h"
#include "Thread_Mutex.h"
#include "Object_Pool.h"
#include "Singleton.h"
#include "Block_Pool_Group.h"
#include "server_define.h"

using namespace Server_Enum;

class Message_Unit;
class Service_Monitor;

class Service_Connector: public Connector {
public:
	void set_monitor(Service_Monitor *server_conn);

	virtual int connect_svc(int connfd);

private:
	Service_Monitor *service_monitor_;
};

////////////////////////////////////////////////////////////////////////////////

class Service_Acceptor: public Acceptor {
public:
	void set_monitor(Service_Monitor *server_listen);

	virtual int accept_svc(int connfd);

	void set_svc_id(int cid, int64_t id);

private:
	Service_Monitor *service_monitor_;
};

////////////////////////////////////////////////////////////////////////////////

class Service_Svc: public Svc {
public:
	void reset(void);

	void set_service_monitor(Service_Monitor *server_listen);

	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *block);

	virtual int register_recv_handler(void);

	virtual int unregister_recv_handler(void);

	virtual int register_send_handler(void);

	virtual int unregister_send_handler(void);

	virtual int recv_handler(int cid);

	virtual int close_handler(int cid);

private:
	Service_Monitor *service_monitor_;
};

////////////////////////////////////////////////////////////////////////////////

class Service_Receiver: public Receiver {
public:
	void set_monitor(Service_Monitor *server_listen);

	virtual int drop_handler(int cid);

	virtual Svc *find_svc(int cid);

private:
	Service_Monitor *service_monitor_;
};

////////////////////////////////////////////////////////////////////////////////

class Service_Sender: public Sender {
public:
	void set_monitor(Service_Monitor *server_listen);

	/// 获取、释放一个buf
	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *buf);

	virtual int drop_handler(int cid);

	virtual Svc *find_svc(int cid);

private:
	Service_Monitor *service_monitor_;
};

////////////////////////////////////////////////////////////////////////////////

class Service_Packer: public Stream_Packer {
public:
	void set_monitor(Service_Monitor *server_listen);

	virtual Svc *find_svc(int cid);

	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *block);

	virtual int packed_data_handler(int cid, Block_Vector &block_vec);

	virtual int drop_handler(int cid);

private:
	Service_Monitor *service_monitor_;
};

////////////////////////////////////////////////////////////////////////////////

class Service_Monitor {
public:
	Service_Monitor(void);
	virtual ~Service_Monitor(void);

	virtual void set_monitor(Unit_Message::UNIT_TYPE type, Message_Unit *unit, Time_Value &recv_timeout);
	virtual int init(void);
	virtual int start(void);

	Svc *find_svc(int cid);

	Unit_Message::UNIT_TYPE &service_type(void);

	Service_Receiver &receiver(void);
	Service_Sender &sender(void);
	Service_Packer &packer(void);

	Block_Buffer *pop_block(int cid);
	int push_block(int cid, Block_Buffer *buf);

	virtual void packed_data_handler(int cid, Block_Buffer *buf);
	void push_drop_cid(int cid);

	virtual int monitor_drop(const int cid);

	int push_data_block_with_len(int cid, Block_Buffer &buf, bool use_swap = false);

private:
	Service_Monitor(const Service_Monitor &);
	const Service_Monitor &operator=(const Service_Monitor &);

protected:
	Service_Receiver receiver_;
	Service_Sender sender_;
	Service_Packer packer_;

	Unit_Message::UNIT_TYPE unit_type_;
	Message_Unit *message_unit_;
};

//////////////////////////////////////////////////////////////////////////////

class Service_Accept : public Service_Monitor {
public:
	Service_Accept(void);
	virtual ~Service_Accept(void);

	virtual void set_monitor(Unit_Message::UNIT_TYPE type, Message_Unit *unit, Time_Value &recv_timeout);

	virtual int init(void);
	virtual int start(void);

	void set_port(int port);
	int get_port(void);

	void set_svc_id(int cid, int64_t id);

	virtual int monitor_drop(const int cid);

private:
	Service_Acceptor acceptor_;
};

////////////////////////////////////////////////////////////////////////////////

class Service_Connect : public Service_Monitor {
public:
	Service_Connect(void);
	virtual ~Service_Connect(void);

	virtual void set_monitor(Unit_Message::UNIT_TYPE type, Message_Unit *unit, Time_Value &recv_timeout);

	virtual int init(void);
	virtual int start(void);

	int connect(const char *ip, int port);
	virtual int monitor_drop(const int cid);

private:
	Service_Connector connect_;
};

////////////////////////////////////////////////////////////////////////////////

class Service_Manager {
public:
	typedef Svc_Static_List<Service_Svc *, Thread_Mutex> Service_Svc_List;
	typedef Object_Pool<Service_Svc, Thread_Mutex> Service_Svc_Pool;

	Service_Svc_List &service_svc_list(void);
	Service_Svc_Pool &service_svc_pool(void);

protected:
	Service_Svc_List service_svc_list_;
	Service_Svc_Pool service_svc_pool_;
};
typedef Singleton<Service_Manager> Service_Manager_Single;
#define SERVICE_MANAGER    (Service_Manager_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* SERVICE_MONITOR_H_ */
