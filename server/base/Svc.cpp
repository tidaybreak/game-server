/*
 * Svc.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Root
 */

#include "Svc.h"
#include "Block_Buffer.h"
#include "Lib_Log.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <limits.h>
#include <errno.h>
#include <cstring>
#include <sstream>

const char Svc::req_flash_policy[] = "<policy-file-request/>";
const char Svc::res_flash_policy[] =
		"<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\"/></cross-domain-policy>\0";

Svc::Svc(void)
: cid_(0),
  max_list_size_(0),
  max_pack_size_(0),
  is_closed_(false),
  is_reg_recv_(false),
  is_reg_send_(false),
  is_client_(false),
  peer_port_(0),
  validate_flash_policy_(false),
  recv_pack_size_(0),
  role_id_(0),
  reset_times_(0)
{ }

Svc::~Svc(void) { }

Block_Buffer *Svc::pop_block(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Svc::push_block(int cid, Block_Buffer *block) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Svc::register_recv_handler(void) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Svc::unregister_recv_handler(void) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Svc::register_send_handler(void) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Svc::unregister_send_handler(void) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Svc::recv_handler(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Svc::close_handler(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Svc::handle_input(void) {
	return recv_data();
}

int Svc::handle_close(Link_Close close_info) {
	if (is_closed_) {
		return 0;
	} else {
		is_closed_ = true;
		close_info_ = close_info;
		return close_handler(cid_);
	}
}

int Svc::close_fd(void) {
	if (is_closed_) {
		LOG_DEBUG("role_id:%ld, close fd:%d, cid:%d, peer_ip:%s, peer_port:%d, close_reason:%d, close_value:%ld", role_id_, this->get_fd(), cid_, peer_ip_.c_str(), peer_port_, close_info_.close_reason, close_info_.close_value);
		::close(this->get_fd());
	}
	return 0;
}

int Svc::get_peer_addr(std::string &ip, int &port) {
	struct sockaddr_in sa;
	socklen_t len = sizeof(sa);

	if (::getpeername(this->get_fd(), (struct sockaddr *)&sa, &len) < 0) {
		LOG_SYS("getpeername");
		LOG_USER_TRACE("getpeername");
		return -1;
	}

	std::stringstream stream;
	stream << inet_ntoa(sa.sin_addr);

	ip = stream.str();
	port = ntohs(sa.sin_port);

	return 0;
}

int Svc::get_local_addr(std::string &ip, int &port) {
	struct sockaddr_in sa;
	socklen_t len = sizeof(sa);

	if (::getsockname(this->get_fd(), (struct sockaddr *)&sa, &len) < 0) {
		LOG_SYS("getsockname");
		LOG_USER_TRACE("getsockname");
		return -1;
	}

	std::stringstream stream;
	stream << inet_ntoa(sa.sin_addr);

	ip = stream.str();
	port = sa.sin_port;

	return 0;
}

int Svc::recv_data(void) {
	if (is_closed_)
		return 0;

	Block_Buffer *buf = pop_block(cid_);
	if (! buf) {
		LOG_USER_TRACE("pop_block return 0");
		return -1;
	}

	buf->write_int32(cid_);

	int n = 0;
	while (1) {
		buf->ensure_writable_bytes(2000); /// every read system call try to read 2k data
		n = 0;
		if ((n = ::read(get_fd(), buf->get_write_ptr(), buf->writable_bytes())) < 0) {
			if (errno == EINTR)
				continue;
			if (errno == EWOULDBLOCK)
				break;

			LOG_SYS("read role_id:%ld, cid:%d, peer_ip:%s, peer_port:%d", role_id_, cid_, peer_ip_.c_str(), peer_port_);

			push_block(cid_, buf);
			handle_close(Link_Close(LINK_CLOSE_RECV_ERROR, errno));

			return 0;
		} else if (n == 0) { /// EOF
			//LOG_DEBUG("fd=%d, read return 0, EOF close", get_fd());
			// 短连接bug
			if (buf->readable_bytes() > sizeof(cid_) && push_recv_block(buf) == 0) {
				recv_handler(cid_);
			} else {
				push_block(cid_, buf);
			}

			//push_block(cid_, buf);
			handle_close(Link_Close(LINK_CLOSE_BY_PEER, 0));

			return 0;
		} else {
			buf->set_write_idx(buf->get_write_idx() + n);
		}
	}

	//LOG_DEBUG("recv %d data", buf->readable_bytes());

	if (push_recv_block(buf) == 0) {
		recv_handler(cid_);
	} else {
		push_block(cid_, buf);
	}

	return 0;
}

int Svc::send_data(void) {
	if (is_closed_)
		return 0;

	while (1) {
		size_t buf_size = send_block_list_.size();
		if (! buf_size)
			return 0;

		size_t sum_bytes = 0;
		std::vector<iovec> iov_vec;
		std::vector<Block_Buffer *> iov_buff;

		iov_vec.clear();
		iov_buff.clear();

		if (send_block_list_.construct_iov(iov_vec, iov_buff, sum_bytes) == 0) {
			LOG_USER_TRACE("construct_iov return 0");
			return 0;
		}

		int ret = ::writev(this->get_fd(), &*iov_vec.begin(), iov_vec.size());
		if (ret == -1) {
			LOG_SYS("writev cid:%d ip = [%s], port = %d", cid_, peer_ip_.c_str(), peer_port_);
			if (errno == EINTR) { /// 被打断, 重写
				continue;
			} else if (errno == EWOULDBLOCK) { /// EAGAIN,下一次超时再写
				return ret;
			} else { /// 其他错误，丢掉该客户端全部数据
				LOG_SYS("writev cid:%d ip = [%s], port = %d handle_colse", cid_, peer_ip_.c_str(), peer_port_);
				handle_close(Link_Close(LINK_CLOSE_SEND_ERROR, errno));

				return ret;
			}

			if (iov_vec.size() >= 100)
				LOG_DEBUG("ret:%d, sum:%d, send_block_list:%d, iov_vec:%d", ret, sum_bytes, send_block_list_.size(), iov_vec.size());

		} else {
			if ((size_t)ret == sum_bytes) { /// 本次全部写完, 尝试继续写
				for (std::vector<Block_Buffer *>::iterator it = iov_buff.begin(); it != iov_buff.end(); ++it) {
					if (push_block(cid_, *it)) {
						LOG_USER("read idx:%d write idx:%d", (*it)->get_read_idx(), (*it)->get_write_idx());
						(*it)->log_binary_data(3000, (*it)->get_read_idx());
						LOG_USER("role_id:%ld, cid:%d, is_closed:%d, send_block_list:%d, sum_bytes:%d, reset_times:%d", role_id_, cid_, is_closed_, send_block_list_.size(), sum_bytes, reset_times_);
					}
				}
				if (send_block_list_.pop_front(iov_buff.size())) {
					LOG_USER("role_id:%ld, cid:%d, is_closed:%d, send_block_list:%d", role_id_, cid_, is_closed_, send_block_list_.size());
				}
				continue;
			} else { /// 未写完, 丢掉已发送的数据, 下一次超时再写
				size_t writed_bytes = ret, remove_count = 0;
				Block_Buffer *buf = 0;
				for (std::vector<Block_Buffer *>::iterator it = iov_buff.begin(); it != iov_buff.end(); ++it) {
					buf = *it;
					if (writed_bytes >= buf->readable_bytes()) {
						++remove_count;
						writed_bytes -= buf->readable_bytes();
						if (push_block(cid_, buf)) {
							(*it)->log_binary_data(3000, (*it)->get_read_idx());
							LOG_USER("role_id:%ld, cid:%d, is_closed:%d, send_block_list:%d", role_id_, cid_, is_closed_, send_block_list_.size());
						}
					} else {
						break;
					}
				}
				LOG_DEBUG("ret:%d, sum:%d, send_block_list:%d, remove_count:%d, writed_bytes:%d, iov_vec:%d", ret, sum_bytes, send_block_list_.size(), remove_count, writed_bytes, iov_vec.size());

				if (send_block_list_.pop_front(remove_count, writed_bytes)) {
					LOG_USER("role_id:%ld, cid:%d, is_closed:%d, send_block_list:%d", role_id_, cid_, is_closed_, send_block_list_.size());
				}
				continue;
				//return ret;
			}
		}
	}

	return 0;
}

int Svc::check_flash_policy(Block_Buffer *buf) {
	if (validate_flash_policy_) {
		if (	buf &&
				buf->readable_bytes() >= sizeof(req_flash_policy) &&
				0 == strncmp(req_flash_policy, buf->get_read_ptr(), sizeof(req_flash_policy))) {
			buf->set_read_idx(buf->get_read_idx() + sizeof(req_flash_policy));
			size_t cb = 0;
			ssize_t n = 0;
			///  try to execute MAX_TRY_RES_FLASH_POLICY times nonblock write, avoid CPU busy loop
			for (int i = 0; i < MAX_TRY_RES_FLASH_POLICY; ++i) {
				n = ::write(this->get_fd(), res_flash_policy + cb, sizeof(res_flash_policy) - cb);
				if (n < 0) {
					if (errno != EWOULDBLOCK) {
						LOG_SYS("write");
						break;
					}
				} else {
					cb += n;
					if (cb >= sizeof(res_flash_policy)) /// write complete
						break;
				}
			}
			set_flash_policy(false);
		}
	}
	return 0;
}

int Svc::pack_recv_data(Block_Vector &block_vec) {
	size_t rd_idx_orig = 0;
	int32_t cid = 0;
	uint32_t len = 0;
	Block_Buffer *front_buf = 0;
	Block_Buffer *free_buf = 0;

	while (! recv_block_list_.empty()) {
		front_buf = recv_block_list_.front();
		if (! front_buf) {
			LOG_USER_TRACE("front_buf == 0");
			continue;
		}

		rd_idx_orig = front_buf->get_read_idx();
		front_buf->read_int32(cid);
		if (front_buf->readable_bytes() <= 0) { /// 数据块异常, 关闭该连接
			LOG_USER_TRACE("server inner error.");
			recv_block_list_.pop_front();
			front_buf->reset();
			push_block(cid_, front_buf);
			handle_close(Link_Close(LINK_CLOSE_BLOCK_ERROR, front_buf->readable_bytes()));
			return -1;
		}

		//check_flash_policy(front_buf);

		if (front_buf->readable_bytes() <= 0) {
			recv_block_list_.pop_front();
			front_buf->reset();
			push_block(cid_, front_buf);
			continue;
		}

		if (front_buf->readable_bytes() < sizeof(len)) { /// 4字节的包长度标识都不够
			front_buf->set_read_idx(rd_idx_orig);
			if ((free_buf = recv_block_list_.merge_first_second()) == 0) {
				return 0;
			} else {
				push_block(cid_, free_buf);
				continue;
			}
		}

		front_buf->peek_uint32(len);
		size_t data_len = front_buf->readable_bytes() - sizeof(len);

		if (len == 0 || len > max_pack_size_) { /// 包长度标识为0, 包长度标识超过max_pack_size_, 即视为无效包, 异常, 关闭该连接
			if(is_client_) {
				// 客户端异常, 关闭该连接
				front_buf->log_binary_data(512, front_buf->get_read_idx());
				recv_block_list_.pop_front();
				front_buf->reset();
				push_block(cid_, front_buf);
				this->handle_close(Link_Close(LINK_CLOSE_HEAD_ERROR, len));
				return -1;
				//LOG_USER_TRACE("may be telnet!! cid:%d role_id:%ld data block len = %u", cid_, role_id_, len);
			} else {

				if (len == 0 || (recv_pack_size_ > 0 && len > max_pack_size_ * 10) || (recv_pack_size_ == 0 && len > max_pack_size_)) {
					int i_arg1 = 0, i_arg2 = 0, i_arg3 = 0, i_arg4 = 0;
					front_buf->log_binary_data(3000, front_buf->get_read_idx());
					front_buf->read_int32(i_arg1);
					front_buf->read_int32(i_arg2);
					front_buf->read_int32(i_arg3);
					front_buf->read_int32(i_arg4);
					LOG_TRACE_ABORT("may be telnet!! len:%d, recv_pack_size:%d, b_size:%d, i_arg1:%d, i_arg2:%d", len, recv_pack_size_, recv_block_list_.size(), i_arg1, i_arg2, i_arg3, i_arg4);

					// 正常不应该大于这个数
					this->handle_close(Link_Close(LINK_CLOSE_HEAD_ERROR, len));
					return -1;
				}
			}
		}

		if (data_len == (size_t)len) {
			#ifdef ROBOT_MODE
			if (front_buf->readable_bytes() >= 8) {
				int msg_id = 0;
				front_buf->peek_int32(msg_id, 4);

				if (msg_id == 81000001 || msg_id == 81000020 || msg_id == 81000101) {	//|| msg_id == 81000010
					recv_block_list_.pop_front();
					push_block(cid, front_buf);
					continue;
				}
			}
			#endif

			front_buf->set_read_idx(rd_idx_orig);
			recv_block_list_.pop_front();
			
			block_vec.push_back(front_buf);
			continue;
		} else {
			if (data_len < (size_t)len) {
				front_buf->set_read_idx(rd_idx_orig);
				if ((free_buf = recv_block_list_.merge_first_second()) == 0) {
					return 0;
				} else {
					push_block(cid_, free_buf);
					continue;
				}
			}

			if (data_len > len) {
				size_t wr_idx_orig = front_buf->get_write_idx();

				#ifdef ROBOT_MODE
				int msg_id = 0;
				if (front_buf->readable_bytes() >= 8) {
					front_buf->peek_int32(msg_id, 4);

					if (msg_id == 81000001 || msg_id == 81000020 || msg_id == 81000101) {	//|| msg_id == 81000010
						size_t cid_idx = front_buf->get_read_idx() + sizeof(len) + len - sizeof(int32_t);
						front_buf->set_read_idx(cid_idx);
						front_buf->set_write_idx(cid_idx);
						front_buf->write_int32(cid_);
						front_buf->set_write_idx(wr_idx_orig);
						continue;
					}
				}
				#endif

				Block_Buffer *data_buf = pop_block(cid_);
				data_buf->reset();
				data_buf->write_int32(cid_);
				data_buf->copy(front_buf->get_read_ptr(), sizeof(len) + len);
				
				block_vec.push_back(data_buf);

				size_t cid_idx = front_buf->get_read_idx() + sizeof(len) + len - sizeof(int32_t);
				front_buf->set_read_idx(cid_idx);
				front_buf->set_write_idx(cid_idx);
				front_buf->write_int32(cid_);
				front_buf->set_write_idx(wr_idx_orig);
				continue;

			}
		}
	}

	return 0;
}
