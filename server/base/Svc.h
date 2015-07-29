/*
 * Client_Svc.h
 *
 *  Created on: Oct 10, 2012
 *      Author: Root
 */

#ifndef SVC_H_
#define SVC_H_

#include "Event_Handler.h"
#include "Block_List.h"
#include "Thread_Mutex.h"

class Block_Buffer;

class Svc: public Event_Handler {
public:
	typedef Block_List<Thread_Mutex> Data_Block_List;
	typedef std::vector<Block_Buffer *> Block_Vector;

	const static uint MAX_CLIENT_LIST_SIZE = 500;
	const static uint MAX_INNER_LIST_SIZE = 3000000;

	const static char req_flash_policy[];
	const static char res_flash_policy[];
	const static int MAX_TRY_RES_FLASH_POLICY = 10;

	Svc(void);

	virtual ~Svc(void);

	void reset(void);

	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *block);

	virtual int register_recv_handler(void);

	virtual int unregister_recv_handler(void);

	virtual int register_send_handler(void);

	virtual int unregister_send_handler(void);

	virtual int recv_handler(int cid);

	virtual int close_handler(int cid);

	virtual int handle_input(void);

	virtual int handle_close(Link_Close close_info);

	int close_fd(void);

	int recv_data(void);

	int send_data(void);

	int check_flash_policy(Block_Buffer *buf);

	int pack_recv_data(Block_Vector &block_vec);

	int push_recv_block(Block_Buffer *buf);

	int push_send_block(Block_Buffer *buf);

	void set_cid(int cid);

	int get_cid(void);

	int64_t get_role_id(void);

	bool get_reg_recv(void);

	void set_reg_recv(bool val);

	bool get_reg_send(void);

	void set_reg_send(bool val);

	bool is_closed(void);

	bool get_closed(void);

	void set_closed(bool v);

	int get_peer_addr(std::string &ip, int &port);

	int get_local_addr(std::string &ip, int &port);

	void set_max_list_size(size_t max_size);

	void set_max_pack_size(size_t max_size);

	void set_flash_policy(bool v);

	bool get_flash_policy(void);

	void set_peer_addr(void);

	void set_role_id(int64_t role_id);

	Link_Close &close_info(void);

	std::string &peer_ip(void);
	int peer_port(void);

	void recv_pack_size(int size);
	int recv_pack_size(void);

protected:
	int cid_;
	Data_Block_List recv_block_list_;
	Data_Block_List send_block_list_;

	size_t max_list_size_;
	size_t max_pack_size_;

	bool is_closed_;
	Link_Close close_info_;
	bool is_reg_recv_, is_reg_send_, is_client_;

	std::string peer_ip_;
	int peer_port_;

	bool validate_flash_policy_; /// 是否启用flash安全沙箱验证

	int recv_pack_size_;
	int64_t role_id_;
	int reset_times_;

};

////////////////////////////////////////////////////////////////////////////////

inline void Svc::reset(void) {
	reset_times_++;

	Data_Block_List::BList blist;

	blist.clear();
	recv_block_list_.swap(blist);
	for (Data_Block_List::BList::iterator it = blist.begin(); it != blist.end(); ++it) {
		push_block(cid_, *it);
	}

	blist.clear();
	send_block_list_.swap(blist);
	for (Data_Block_List::BList::iterator it = blist.begin(); it != blist.end(); ++it) {
		push_block(cid_, *it);
	}

	cid_ = 0;
	is_closed_ = false;
	close_info_.reset();
	is_reg_recv_ = false;
	is_reg_send_ = false;
	is_client_ = false;

	max_list_size_ = 0;
	max_pack_size_ = 0;

	peer_ip_.clear();
	peer_port_ = 0;

	validate_flash_policy_ = false;
	recv_pack_size_ = 0;
	role_id_ = -1;
	Event_Handler::reset();
}

inline int Svc::push_recv_block(Block_Buffer *buf) {
	if (is_closed_)
		return -1;

	if (recv_block_list_.size() >= max_list_size_) {
		LOG_USER("recv_block_list_ has full, recv_block_list:%d, max_list_size:%d, cid:%d %d", recv_block_list_.size(), max_list_size_, cid_, is_closed_);
		recv_block_list_.debug_buff(100);
		return -1;
	}

	recv_block_list_.push_back(buf);
	return 0;
}

inline int Svc::push_send_block(Block_Buffer *buf) {
	if (is_closed_)
		return -1;

	if (send_block_list_.size() >= max_list_size_) {
		LOG_USER_TRACE("send_block_list_ has full cid:%d, role_id:%ld, send_block_list_.size() = %d, max_list_size_ = %d", cid_, role_id_, send_block_list_.size(), max_list_size_);

		if(is_client_) {
			send_block_list_.debug_buff(100);
			handle_close(Link_Close(LINK_CLOSE_SEND_BLOCK_FULL, send_block_list_.size()));
		} else {
			send_block_list_.debug_buff(100);
			LOG_TRACE_ABORT("send_block_list_");
		}

		return -1;
	}

	send_block_list_.push_back(buf);
	return 0;
}

inline void Svc::set_cid(int cid) {
	cid_ = cid;
}

inline int Svc::get_cid(void) {
	return cid_;
}

inline int64_t Svc::get_role_id(void) {
	return role_id_;
}

inline bool Svc::get_reg_recv(void) {
	return is_reg_recv_;
}

inline void Svc::set_reg_recv(bool val) {
	is_reg_recv_ = val;
}

inline bool Svc::get_reg_send(void) {
	return is_reg_send_;
}

inline void Svc::set_reg_send(bool val) {
	is_reg_send_ = val;
}

inline bool Svc::is_closed(void) {
	return is_closed_;
}

inline bool Svc::get_closed(void) {
	return is_closed_;
}

inline void Svc::set_closed(bool v) {
	is_closed_ = v;
}

inline void Svc::set_max_list_size(size_t max_size) {
	max_list_size_ = max_size;

	if (max_size == MAX_CLIENT_LIST_SIZE)
		is_client_ = true;
}

inline void Svc::set_max_pack_size(size_t max_size) {
	max_pack_size_ = max_size;
}

inline void Svc::set_flash_policy(bool v) {
	validate_flash_policy_ = v;
}

inline bool Svc::get_flash_policy(void) {
	return validate_flash_policy_;
}

inline void Svc::set_peer_addr(void) {
	get_peer_addr(peer_ip_, peer_port_);
}

inline void Svc::set_role_id(int64_t role_id) {
	role_id_ = role_id;
}

inline Link_Close &Svc::close_info(void) {
	return close_info_;
}

inline std::string &Svc::peer_ip(void) {
	return peer_ip_;
}

inline int Svc::peer_port(void) {
	return peer_port_;
}

inline void Svc::recv_pack_size(int size) {
	recv_pack_size_ += size;
}

inline int Svc::recv_pack_size(void) {
	return recv_pack_size_;
}

#endif /* SVC_H_ */
