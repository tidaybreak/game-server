/*
 * Service_Monitor.cpp
 *
 *  Created on: Nov 2, 2012
 *      Author: Ti
 */

#include "Service_Monitor.h"
#include "Server_Struct.h"
#include "Pool_Manager.h"
#include "Message_Unit.h"
#include "Server_Monitor.h"

void Service_Connector::set_monitor(Service_Monitor *server_conn) {
	service_monitor_ = server_conn;
}

int Service_Connector::connect_svc(int connfd) {
	Service_Svc *svc = SERVICE_MANAGER->service_svc_pool().pop();
	if (! svc) {
		LOG_USER("svc == NULL");
		return -1;
	}

	int cid = SERVICE_MANAGER->service_svc_list().record_svc(svc);
	if (cid == -1) {
		LOG_USER("cid == -1");
		SERVICE_MANAGER->service_svc_pool().push(svc);
		return -1;
	}

	svc->reset();
	svc->set_service_monitor(service_monitor_);
	svc->set_max_list_size(Svc::MAX_INNER_LIST_SIZE);
	svc->set_max_pack_size(Block_Buffer::max_recv_pack_len);
	svc->set_cid(cid);
	svc->set_fd(connfd);
	svc->set_peer_addr();
	svc->set_role_id(0);

	svc->register_recv_handler();
	svc->register_send_handler();

	return cid;
}

////////////////////////////////////////////////////////////////////////////////

void Service_Acceptor::set_monitor(Service_Monitor *server_listen) {
	service_monitor_ = server_listen;
}

void Service_Acceptor::set_svc_id(int cid, int64_t id) {
	Svc *svc = service_monitor_->find_svc(cid);
	if (svc)
		svc->set_role_id(id);
	else
		LOG_USER("error find svc, cid:%d, id:%ld", cid, id);
}

int Service_Acceptor::accept_svc(int connfd) {
	Service_Svc *svc = SERVICE_MANAGER->service_svc_pool().pop();
	if (! svc) {
		LOG_USER("svc == NULL");
		return -1;
	}

	int cid = SERVICE_MANAGER->service_svc_list().record_svc(svc);
	if (cid == -1) {
		LOG_USER("cid == -1");
		SERVICE_MANAGER->service_svc_pool().push(svc);
		return -1;
	}

	svc->reset();
	svc->set_service_monitor(service_monitor_);
	if (service_monitor_->service_type() == Unit_Message::UNIT_TYPE::TYPE_UNIT_CLIENT)
		svc->set_max_list_size(Svc::MAX_CLIENT_LIST_SIZE);
	else
		svc->set_max_list_size(Svc::MAX_INNER_LIST_SIZE);
	svc->set_max_pack_size(Block_Buffer::max_recv_pack_len);
	svc->set_flash_policy(true);
	svc->set_cid(cid);
	svc->set_fd(connfd);
	svc->set_peer_addr();
	svc->set_role_id(0);

	svc->register_recv_handler();
	svc->register_send_handler();

	LOG_DEBUG("connfd:%d, cid:%d, listen port:[%d] client:[%s:%d]", connfd, cid, get_port(), svc->peer_ip().c_str(), svc->peer_port());

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void Service_Receiver::set_monitor(Service_Monitor *server_listen) {
	service_monitor_ = server_listen;
}

int Service_Receiver::drop_handler(int cid) {
	return service_monitor_->sender().push_drop(cid);
}

Svc *Service_Receiver::find_svc(int cid) {
	return service_monitor_->find_svc(cid);
}

////////////////////////////////////////////////////////////////////////////////

void Service_Sender::set_monitor(Service_Monitor *server_listen) {
	service_monitor_ = server_listen;
}

Block_Buffer *Service_Sender::pop_block(int cid) {
	return service_monitor_->pop_block(cid);
}

int Service_Sender::push_block(int cid, Block_Buffer *buf) {
	return service_monitor_->push_block(cid, buf);
}

int Service_Sender::drop_handler(int cid) {
	return service_monitor_->packer().push_drop(cid);
}

Svc *Service_Sender::find_svc(int cid) {
	return service_monitor_->find_svc(cid);
};

////////////////////////////////////////////////////////////////////////////////

void Service_Packer::set_monitor(Service_Monitor *server_listen) {
	service_monitor_ = server_listen;
}

Svc *Service_Packer::find_svc(int cid) {
	return service_monitor_->find_svc(cid);
}

Block_Buffer *Service_Packer::pop_block(int cid) {
	return service_monitor_->pop_block(cid);
}

int Service_Packer::push_block(int cid, Block_Buffer *block) {
	return service_monitor_->push_block(cid, block);
}

int Service_Packer::packed_data_handler(int cid, Block_Vector &block_vec) {
	for (Block_Vector::iterator it = block_vec.begin(); it != block_vec.end(); ++it) {
		service_monitor_->packed_data_handler(cid, *it);
	}
	return 0;
}

int Service_Packer::drop_handler(int cid) {
	Service_Svc *svc = 0;
	SERVICE_MANAGER->service_svc_list().get_used_svc(cid, svc);

	if (svc) {
		/// 向逻辑层通知断线
		service_monitor_->push_drop_cid(cid);

		/// 关闭通讯层连接
		svc->close_fd();
		SERVICE_MANAGER->service_svc_list().erase_svc(cid);
		svc->reset();
		SERVICE_MANAGER->service_svc_pool().push(svc);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void Service_Svc::reset(void) {
	Svc::reset();
	service_monitor_ = NULL;
}

void Service_Svc::set_service_monitor(Service_Monitor *server_listen) {
	service_monitor_ = server_listen;
}

Block_Buffer *Service_Svc::pop_block(int cid) {
	return service_monitor_->pop_block(cid);
}

int Service_Svc::push_block(int cid, Block_Buffer *block) {
	return service_monitor_->push_block(cid, block);
}

int Service_Svc::register_recv_handler(void) {
	if (! get_reg_recv()) {
		service_monitor_->receiver().register_svc(this);
		set_reg_recv(true);
	}
	return 0;
}

int Service_Svc::unregister_recv_handler(void) {
	if (get_reg_recv()) {
		service_monitor_->receiver().unregister_svc(this);
		set_reg_recv(false);
	}
	return 0;
}

int Service_Svc::register_send_handler(void) {
	if (! get_reg_send()) {
		service_monitor_->sender().register_svc(this);
		set_reg_send(true);
	}
	return 0;
}

int Service_Svc::unregister_send_handler(void) {
	if (get_reg_send()) {
		service_monitor_->sender().unregister_svc(this);
		set_reg_send(true);
	}
	return 0;
}

int Service_Svc::recv_handler(int cid) {
	service_monitor_->packer().push_packing_cid(cid);
	return 0;
}

int Service_Svc::close_handler(int cid) {
	service_monitor_->receiver().push_drop(cid);
	return 0;
}

///////////////////////////////////////////////////////////////////////

Service_Monitor::Service_Monitor(void) :
		unit_type_(Unit_Message::TYPE_UNIT_NULL),
		message_unit_(0) { }

Service_Monitor::~Service_Monitor(void) { }

void Service_Monitor::set_monitor(Unit_Message::UNIT_TYPE type, Message_Unit *unit, Time_Value &recv_timeout) {
	unit_type_ = type;
	message_unit_ = unit;

	sender_.set_monitor(this);
	packer_.set_monitor(this);
	receiver_.set_monitor(this);

	receiver_.set(&recv_timeout);

}

int Service_Monitor::init(void) {
	receiver().init();
	sender().init();
	return 0;
}

int Service_Monitor::start(void) {
	receiver().thr_create();
	sender().thr_create();
	packer().thr_create();
	return 0;
}

Svc *Service_Monitor::find_svc(int cid) {
	Service_Svc *svc = 0;
	SERVICE_MANAGER->service_svc_list().get_used_svc(cid, svc);
	return svc;
}

Unit_Message::UNIT_TYPE &Service_Monitor::service_type(void) {
	return unit_type_;
}

Service_Receiver &Service_Monitor::receiver(void) {
	return receiver_;
}

Service_Sender &Service_Monitor::sender(void) {
	return sender_;
}

Service_Packer &Service_Monitor::packer(void) {
	return packer_;
}

Block_Buffer *Service_Monitor::pop_block(int cid) {
	return message_unit_->pop_block(cid);
}

int Service_Monitor::push_block(int cid, Block_Buffer *buf) {
	return message_unit_->push_block(cid, buf);
}

void Service_Monitor::packed_data_handler(int cid, Block_Buffer *buf) {
	if (message_unit_->push_ptr_data(Unit_Message::TYPE_DATA_BUFFER, buf, unit_type_, cid)) {
		LOG_USER("push_data_list_block error");
		int cid = 0;
		buf->peek_int32(cid);
		push_block(cid, buf);
	}
}

void Service_Monitor::push_drop_cid(int cid) {
	Svc *svc = find_svc(cid);
	if (svc) {
		monitor_drop(cid);

		Block_Buffer *buf = pop_block(cid);
		buf->make_message(SERVER_INNER_LINK_CLOSE);
		Link_Drop link_drop(cid, unit_type_, svc->close_info().close_reason, svc->close_info().close_value);
		link_drop.serialize(*buf);
		buf->finish_message();
		int ret = message_unit_->push_ptr_data(Unit_Message::TYPE_DATA_BUFFER, buf, Unit_Message::TYPE_UNIT_LOOP, cid);

		if (ret) {
			LOG_USER("push_data_list_block error");
			push_block(cid, buf);
		}
	} else {
		LOG_USER("chat scene drop can not find cid:%d", cid);
	}
}

int Service_Monitor::monitor_drop(const int cid) {

	return 0;
}

int Service_Monitor::push_data_block_with_len(int cid, Block_Buffer &buf, bool use_swap) {
	sender_.push_data_block_with_len(cid, buf, use_swap);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////

Service_Accept::Service_Accept(void) { }

Service_Accept::~Service_Accept(void) { }

void Service_Accept::set_port(int port) {
	acceptor_.set(port);
}

int Service_Accept::get_port(void) {
	return acceptor_.get_port();
}

void Service_Accept::set_svc_id(int cid, int64_t id) {
	acceptor_.set_svc_id(cid, id);
}

int Service_Accept::monitor_drop(const int cid) {
	//if (service_type() == Unit_Message::TYPE_UNIT_CLIENT)
	//	message_unit_->monitor()->Server_Monitor::client_link_drop(cid);
	return 0;
}

void Service_Accept::set_monitor(Unit_Message::UNIT_TYPE type, Message_Unit *unit, Time_Value &recv_timeout) {
	Service_Monitor::set_monitor(type, unit, recv_timeout);
	acceptor_.set_monitor(this);
}

int Service_Accept::init(void) {
	Service_Monitor::init();
	acceptor_.init();
	return 0;
}

int Service_Accept::start(void) {
	Service_Monitor::start();
	acceptor_.thr_create();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////

Service_Connect::Service_Connect(void) { }

Service_Connect::~Service_Connect(void) { }

void Service_Connect::set_monitor(Unit_Message::UNIT_TYPE type, Message_Unit *unit, Time_Value &recv_timeout) {
	Service_Monitor::set_monitor(type, unit, recv_timeout);
	connect_.set_monitor(this);
}

int Service_Connect::init(void) {
	Service_Monitor::init();
	return 0;
}

int Service_Connect::start(void) {
	Service_Monitor::start();
	return 0;
}

int Service_Connect::connect(const char *ip, int port) {
	return connect_.connect(ip, port);
}

int Service_Connect::monitor_drop(const int cid) {
	//message_unit_->monitor()->Server_Monitor::connect_link_drop(cid);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////


Service_Manager::Service_Svc_List &Service_Manager::service_svc_list(void) {
	return service_svc_list_;
}

Service_Manager::Service_Svc_Pool &Service_Manager::service_svc_pool(void) {
	return service_svc_pool_;
}
