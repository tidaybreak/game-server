// -*- C++ -*-
/*
 * Block_Buffer.h
 *
 *  Created on: Mar 23, 2012
 *      Author: Root
 *
 *
 * +------------+----------------+-----------------+
 * | head_bytes | readable bytes | writeable bytes |
 * |            |   (CONTENT)    |                 |
 * +------------+----------------+-----------------+
 * |            |                |                 |
 * 0  read_index(init_offset)  write_index     vector::size()
 *
 */

#ifndef BLOCK_BUFFER_H_
#define BLOCK_BUFFER_H_

#include "Lib_Log.h"
#include <stdint.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <endian.h>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <limits.h>

#define BLOCK_LITTLE_ENDIAN
//#define BLOCK_BIG_ENDIAN

class Block_Buffer {
public:
	static const uint max_buff_pack_len = 1024 * 300; 		/// 接收单个包最大300k
	static const uint max_recv_pack_len = 1024 * 1024;	    /// 接收单个包最大1024k
	static const uint max_warn_pack_len = 1024 * 80; 		/// 警告包最大80k

	Block_Buffer(unsigned short max_use_times = 5, size_t init_size = 2048, size_t init_offset = 5 * sizeof(uint32_t));

	inline void reset(void);

	inline void swap(Block_Buffer &block);

	/// 当前缓冲内可读字节数
	inline size_t readable_bytes(void) const;

	/// 当前缓冲内可写字节数
	inline size_t writable_bytes(void) const;

	inline char * get_read_ptr(void);
	inline char * get_write_ptr(void);
	inline int get_read_idx(void);
	inline void set_read_idx(int ridx);
	inline int get_write_idx(void);
	inline void set_write_idx(int widx);
	inline bool get_ext_flag(void);
	inline void set_ext_flag(bool v);

	inline void copy(Block_Buffer *block);
	inline void copy(std::string const &str);
	inline void copy(char const *data, size_t len);
	inline void copy(void const *data, size_t len);

	inline void ensure_writable_bytes(size_t len);

	inline int peek_int8(int8_t &v);
	inline int peek_int16(int16_t &v);
	inline int peek_int32(int32_t &v);
	inline int peek_int32(int32_t &v, int off);
	inline int peek_int64(int64_t &v);
	inline int peek_uint8(uint8_t &v);
	inline int peek_uint16(uint16_t &v);
	inline int peek_uint32(uint32_t &v);
	inline int peek_double(double &v);
	inline int peek_bool(bool &v);
	inline int peek_string(std::string &str);

	inline int read(char *data, size_t len);
	inline int read_int8(int8_t &v);
	inline int read_int16(int16_t &v);
	inline int read_int32(int32_t &v);
	inline int read_int64(int64_t &v);
	inline int read_uint8(uint8_t &v);
	inline int read_uint16(uint16_t &v);
	inline int read_uint32(uint32_t &v);
	inline int read_uint64(uint64_t &v);
	inline int read_double(double &v);
	inline int read_bool(bool &v);
	inline int read_string(std::string &str);

	inline int write_int8(int8_t v);
	inline int write_int16(int16_t v);
	inline int write_int32(int32_t v);
	inline int write_int64(int64_t v);
	inline int write_uint8(uint8_t v);
	inline int write_uint16(uint16_t v);
	inline int write_uint32(uint32_t v);
	inline int write_uint64(uint64_t);
	inline int write_double(double v);
	inline int write_bool(bool v);
	inline int write_string(const std::string &str);

	inline Block_Buffer &operator>>(int8_t &v);
	inline Block_Buffer &operator>>(int16_t &v);
	inline Block_Buffer &operator>>(int32_t &v);
	inline Block_Buffer &operator>>(uint8_t &v);
	inline Block_Buffer &operator>>(uint16_t &v);
	inline Block_Buffer &operator>>(uint32_t &v);

	inline Block_Buffer &operator<<(int8_t v);
	inline Block_Buffer &operator<<(int16_t v);
	inline Block_Buffer &operator<<(int32_t v);
	inline Block_Buffer &operator<<(uint8_t v);
	inline Block_Buffer &operator<<(uint16_t v);
	inline Block_Buffer &operator<<(uint32_t v);

	inline Block_Buffer &operator>>(double &v);
	inline Block_Buffer &operator<<(double v);

	inline Block_Buffer &operator>>(std::string &v);
	inline Block_Buffer &operator<<(std::string &v);

	inline void make_message(int msg_id);
	inline void finish_message(void);
	inline void make_client_message(int msg_id, uint32_t serial, uint32_t time);
	inline void make_head(int msg_id, int64_t id);
	inline uint32_t get_msg_id(void);

	inline bool is_read_begin(void);
	inline void set_read_begin(void);
	inline bool is_write_begin(void);

	inline  size_t capacity(void);

	inline bool check_pack_len(void);

	inline void recycle_space(void);

	inline bool verify_read(size_t s);

	void log_binary_data(int len, int read_idx);
	inline void debug(void);

	inline void set_read_abort(bool value);

private:
	inline char *begin(void);
	inline const char *begin(void) const;
	inline void make_space(size_t len);

private:
	unsigned short max_use_times_;
	unsigned short use_times_;
	size_t init_size_;
	size_t init_offset_;
	size_t read_index_, write_index_;
	std::vector<char> buffer_;
	bool read_error_abort_;

	bool ext_flag_;
};

////////////////////////////////////////////////////////////////////////////////

#define peek8(x)	\
	memcpy(&x, &(buffer_[read_index_]), sizeof(uint8_t));

#define peek16(x)	\
	uint16_t t;		\
	memcpy(&t, &(buffer_[read_index_]), sizeof(t));	\
	x = be16toh(t);

#define peek32(x)	\
	uint32_t t;		\
	memcpy(&t, &(buffer_[read_index_]), sizeof(t));	\
	x = be32toh(t);

#define peek64(x)	\
	uint64_t t;		\
	memcpy(&t, &(buffer_[read_index_]), sizeof(t));	\
	x = be64toh(t);

#define read8(x)	\
	memcpy(&x, &(buffer_[read_index_]), sizeof(uint8_t));	\
	read_index_ += sizeof(uint8_t);

#define read16(x)	\
	uint16_t t;		\
	memcpy(&t, &(buffer_[read_index_]), sizeof(t));	\
	x = be16toh(t);	\
	read_index_ += sizeof(uint16_t);

#define read32(x)	\
	uint32_t t;		\
	memcpy(&t, &(buffer_[read_index_]), sizeof(t));	\
	x = be32toh(t);	\
	read_index_ += sizeof(uint32_t);

#define read64(x)	\
	uint64_t t;		\
	memcpy(&t, &(buffer_[read_index_]), sizeof(t));	\
	x = be64toh(t);	\
	read_index_ += sizeof(uint64_t);

#define write8(x)	\
	copy(&x, sizeof(uint8_t));

#define write16(x)	\
	uint16_t t;		\
	t = htobe16(x);	\
	copy(&t, sizeof(uint16_t));

#define write32(x)	\
	uint32_t t;		\
	t = htobe32(x);	\
	copy(&t, sizeof(uint32_t));

#define write64(x)	\
	uint64_t t;		\
	t = htobe64(x);	\
	copy(&t, sizeof(uint64_t));

#define swap_double(d)						\
	({ double __v, __x = (d);				\
		__asm__ ("bswap %q0" : "=r" (__v) : "0" (__x));	\
		__v; })

#define htobe_double(d)	swap_double(d)
#define betoh_double(d) swap_double(d)

void Block_Buffer::reset(void) {
	++use_times_;
	recycle_space();

	ensure_writable_bytes(init_offset_);
	read_index_ = write_index_ = init_offset_;
	ext_flag_ = false;
}

void Block_Buffer::swap(Block_Buffer &block) {
	std::swap(this->max_use_times_, block.max_use_times_);
	std::swap(this->use_times_, block.use_times_);
	std::swap(this->init_size_, block.init_size_);
	std::swap(this->init_offset_, block.init_offset_);
	std::swap(this->read_index_, block.read_index_);
	std::swap(this->write_index_, block.write_index_);
	buffer_.swap((std::vector<char>&&)block.buffer_);
}

size_t Block_Buffer::readable_bytes(void) const {
	return write_index_ - read_index_;
}

size_t Block_Buffer::writable_bytes(void) const {
	return buffer_.size() - write_index_;
}

char *Block_Buffer::get_read_ptr(void) {
	return begin() + read_index_;
}

char *Block_Buffer::get_write_ptr(void) {
	return begin() + write_index_;
}

int Block_Buffer::get_read_idx(void) {
	return read_index_;
}

void Block_Buffer::set_read_idx(int ridx) {
	if ((size_t)ridx > buffer_.size() || (size_t)ridx > write_index_) {
		LOG_ABORT("set_read_idx error ridx = %d.", ridx);
		debug();
	}

	read_index_ = ridx;
}

int Block_Buffer::get_write_idx(void) {
	return write_index_;
}

void Block_Buffer::set_write_idx(int widx) {
	if ((size_t)widx > buffer_.size() || (size_t)widx < read_index_) {
		LOG_ABORT("set_write_idx error widx = %d.", widx);
		debug();
	}

	write_index_ = widx;
}

void Block_Buffer::ensure_writable_bytes(size_t len) {
	if (writable_bytes() < len) {
		len = len <= 8 ? len * 3 : len;
		make_space(len);
	}
}

void Block_Buffer::make_space(size_t len) {
	// Block_Buffer，有些扩充内存时会修改读取内容的起始位置，那里我后面改为不修改了，不然很容易导致使用出错
//	int cond_pos = read_index_ - init_offset_;
//	size_t read_begin, head_size;
//	if (cond_pos < 0) {
//		read_begin = init_offset_ = read_index_;
//		head_size = 0;
//		LOG_USER_TRACE("read_index_ = %u, init_offset_ = %u", read_index_, init_offset_);
//	} else {
//		read_begin = init_offset_;
//		head_size = cond_pos;
//	}
//
//	if (writable_bytes() + head_size < len) {
//		buffer_.resize(write_index_ + len);
//	} else {
//		/// 把数据移到头部，为写腾出空间
//		size_t readable = readable_bytes();
//		std::copy(begin() + read_index_, begin() + write_index_, begin() + read_begin);
//		read_index_ = read_begin;
//		write_index_ = read_index_ + readable;
//	}

	buffer_.resize(write_index_ + len);
}

char *Block_Buffer::begin(void) {
	return &*buffer_.begin();
}

const char *Block_Buffer::begin(void) const {
	return &*buffer_.begin();
}

void Block_Buffer::copy(void const *data, size_t len) {
	copy(static_cast<const char*> (data), len);
}

void Block_Buffer::copy(char const *data, size_t len) {
	ensure_writable_bytes(len);
	std::copy(data, data + len, get_write_ptr());
	write_index_ += len;
}

void Block_Buffer::copy(std::string const &str) {
	copy(str.data(), str.length());
}

void Block_Buffer::copy(Block_Buffer *block) {
	copy(block->get_read_ptr(), block->readable_bytes());
}

void Block_Buffer::debug(void) {
	LOG_DEBUG("  read_index = %ul, write_index = %ul, buffer_.size = %ul.", read_index_, write_index_, buffer_.size());;
}

int Block_Buffer::peek_int8(int8_t &v) {
	if (verify_read(sizeof(v))) {
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::peek_int16(int16_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint16_t t, u;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		u = be16toh(t);
		memcpy(&v, &u, sizeof(v));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::peek_int32(int32_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint32_t t, u;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		u = be32toh(t);
		memcpy(&v, &u, sizeof(v));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::peek_int32(int32_t &v, int off) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint32_t t, u;
		memcpy(&t, &(buffer_[read_index_ + off]), sizeof(t));
		u = be32toh(t);
		memcpy(&v, &u, sizeof(v));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_ + off]), sizeof(v));
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::peek_int64(int64_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint64_t t, u;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		u = be64toh(t);
		memcpy(&v, &u, sizeof(v));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::peek_uint8(uint8_t &v) {
	if (verify_read(sizeof(v))) {
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::peek_uint16(uint16_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint16_t t;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		v = be16toh(t);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

uint32_t Block_Buffer::get_msg_id(void) {
	uint32_t msg_id = 0;
	if (verify_read(sizeof(msg_id))) {
#ifdef BLOCK_BIG_ENDIAN
		uint32_t t;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		v = be32toh(t);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		if (this->readable_bytes() >= 8)
			memcpy(&msg_id, &(buffer_[read_index_ + 4]), sizeof(msg_id));
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return 0;
	}
	return msg_id;
}

int Block_Buffer::peek_uint32(uint32_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint32_t t;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		v = be32toh(t);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::read(char *data, size_t len){
    if (verify_read(len)) {
        ::memcpy(data, &(buffer_[read_index_]), len);
        read_index_ += len;
    } else {
    	log_binary_data(write_index_ - init_offset_, init_offset_);
    	LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
        return -1;
    }
    return 0;
}

int Block_Buffer::read_int8(int8_t &v) {
	if (verify_read(sizeof(v))) {
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::read_int16(int16_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint16_t t, u;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		u = be16toh(t);
		memcpy(&v, &u, sizeof(v));
		read_index_ += sizeof(v);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::read_int32(int32_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint32_t t, u;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		u = be32toh(t);
		memcpy(&v, &u, sizeof(v));
		read_index_ += sizeof(v);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::read_int64(int64_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint64_t t, u;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		u = be64toh(t);
		memcpy(&v, &u, sizeof(v));
		read_index_ += sizeof(v);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::read_uint8(uint8_t &v) {
	if (verify_read(sizeof(v))) {
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::read_uint16(uint16_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint16_t t;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		v = be16toh(t);
		read_index_ += sizeof(v);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::read_uint32(uint32_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint32_t t;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		v = be32toh(t);
		read_index_ += sizeof(v);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::read_uint64(uint64_t &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint64_t t;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		v = be64toh(t);
		read_index_ += sizeof(v);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::write_int8(int8_t v) {
	copy(&v, sizeof(v));
	return 0;
}

int Block_Buffer::write_int16(int16_t v) {
#ifdef BLOCK_BIG_ENDIAN
	uint16_t t, u;
	t = *((uint16_t *)&v);
	u = htobe16(t);
	copy(&u, sizeof(u));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
	copy(&v, sizeof(v));
#endif
	return 0;
}

int Block_Buffer::write_int32(int32_t v) {
#ifdef BLOCK_BIG_ENDIAN
	uint32_t t, u;
	t = *((uint32_t *)&v);
	u = htobe32(t);
	copy(&u, sizeof(u));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
	copy(&v, sizeof(v));
#endif
	return 0;
}

int Block_Buffer::write_int64(int64_t v) {
#ifdef BLOCK_BIG_ENDIAN
	uint64_t t, u;
	t = *((uint64_t *)&v);
	u = htobe64(t);
	copy(&u, sizeof(u));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
	copy(&v, sizeof(v));
#endif
	return 0;
}

int Block_Buffer::write_uint8(uint8_t v) {
	copy(&v, sizeof(v));
	return 0;
}

int Block_Buffer::write_uint16(uint16_t v) {
#ifdef BLOCK_BIG_ENDIAN
	uint16_t t;
	t = htobe16(v);
	copy(&t, sizeof(t));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
	copy(&v, sizeof(v));
#endif
	return 0;
}

int Block_Buffer::write_uint32(uint32_t v) {
#ifdef BLOCK_BIG_ENDIAN
	uint32_t t;
	t = htobe32(v);
	copy(&t, sizeof(t));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
	copy(&v, sizeof(v));
#endif
	return 0;
}

int Block_Buffer::write_uint64(uint64_t v) {
#ifdef BLOCK_BIG_ENDIAN
	uint64_t t;
	t = htobe64(v);
	copy(&t, sizeof(t));
#endif

#ifdef BLOCK_LITTLE_ENDIAN
	copy(&v, sizeof(v));
#endif
	return 0;
}

int Block_Buffer::peek_double(double &v) {
#ifdef BLOCK_BIG_ENDIAN
	uint64_t t, u;
	memcpy(&t, &(buffer_[read_index_]), sizeof(t));
	u = be64toh(t);
	memcpy(&v, &u, sizeof(v));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
	memcpy(&v, &(buffer_[read_index_]), sizeof(v));
#endif
	return 0;
}

int Block_Buffer::read_double(double &v) {
	if (verify_read(sizeof(v))) {
#ifdef BLOCK_BIG_ENDIAN
		uint64_t t, u;
		memcpy(&t, &(buffer_[read_index_]), sizeof(t));
		u = be64toh(t);
		memcpy(&v, &u, sizeof(v));
		read_index_ += sizeof(t);
#endif
#ifdef BLOCK_LITTLE_ENDIAN
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
#endif
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

int Block_Buffer::write_double(double v) {
#ifdef BLOCK_BIG_ENDIAN
	uint64_t t, u;
	t = *((uint64_t *)&v);
	u = htobe64(t);
	copy(&u, sizeof(u));
#endif
#ifdef BLOCK_LITTLE_ENDIAN
	copy(&v, sizeof(v));
#endif
	return 0;
}

inline int Block_Buffer::peek_bool(bool &v) {
	if (verify_read(sizeof(v))) {
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

inline int Block_Buffer::read_bool(bool &v) {
	if (verify_read(sizeof(v))) {
		memcpy(&v, &(buffer_[read_index_]), sizeof(v));
		read_index_ += sizeof(v);
	} else {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	return 0;
}

inline int Block_Buffer::write_bool(bool v) {
	copy(&v, sizeof(v));
	return 0;
}

int Block_Buffer::peek_string(std::string &str) {
	uint16_t len;
	read_uint16(len);
	if (len < 0) return -1;
	str.append(buffer_[read_index_], len);
	return 0;
}

int Block_Buffer::read_string(std::string &str) {
	uint16_t len = 0;
	if (read_uint16(len) != 0) {
		return -1;
	}
	if (!verify_read(len)) {
		log_binary_data(write_index_ - init_offset_, init_offset_);
		if (read_error_abort_)
			LOG_TRACE_ABORT("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		else
			LOG_USER_TRACE("out of range r_idx:%d w_idx:%d", read_index_, write_index_);
		return -1;
	}
	str.clear();
	str.resize(len);
	memcpy((char *)str.c_str(), this->get_read_ptr(), len);
	read_index_ += len;
	return 0;
}

int Block_Buffer::write_string(const std::string &str) {
	uint16_t len = str.length();
	write_uint16(len);
	copy(str.c_str(), str.length());
	return 0;
}

Block_Buffer &Block_Buffer::operator>>(int8_t &v) {
	read_int8(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator>>(int16_t &v) {
	read_int16(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator>>(int32_t &v) {
	read_int32(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator>>(uint8_t &v) {
	read_uint8(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator>>(uint16_t &v) {
	read_uint16(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator>>(uint32_t &v) {
	read_uint32(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator<<(int8_t v) {
	write_int8(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator<<(int16_t v) {
	write_int16(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator<<(int32_t v) {
	write_int32(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator<<(uint8_t v) {
	write_uint8(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator<<(uint16_t v) {
	write_uint16(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator<<(uint32_t v) {
	write_uint32(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator>>(double &v) {
	read_double(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator<<(double v) {
	write_double(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator>>(std::string &v) {
	read_string(v);
	return *this;
}

Block_Buffer &Block_Buffer::operator<<(std::string &v) {
	write_string(v);
	return *this;
}

bool Block_Buffer::is_read_begin(void) {
	return read_index_ == init_offset_;
}

bool Block_Buffer::is_write_begin(void) {
	return write_index_ == init_offset_;
}

void Block_Buffer::set_read_begin(void) {
	read_index_ = init_offset_;
}

bool Block_Buffer::get_ext_flag(void) {
	return ext_flag_;
}

void Block_Buffer::set_ext_flag(bool v) {
	ext_flag_ = v;
}

void Block_Buffer::make_message(int msg_id) {
	write_uint32(0); /// length
	write_uint32(msg_id);
}

void Block_Buffer::make_client_message(int msg_id, uint32_t serial, uint32_t time) {
	write_uint32(0); /// length
	write_uint32(msg_id);
	write_uint32(serial);
	write_uint32(time);

}

void Block_Buffer::make_head(int msg_id, int64_t id) {
	uint32_t inner_len = readable_bytes() + sizeof(uint32_t) + sizeof(int64_t);
	if (inner_len > max_buff_pack_len) {
		LOG_TRACE_ABORT("get_read_idx:%d, msg_id:%d, inner_len:%d", get_read_idx(), msg_id, inner_len);
	}

	int wri_idx = get_write_idx();
	int new_idx = get_read_idx() - sizeof(inner_len) - sizeof(uint32_t) - sizeof(int64_t);
	if (new_idx < 0) {
		LOG_TRACE_ABORT("get_read_idx:%d", get_read_idx());
		return ;
	}

	set_read_idx(new_idx);
	set_write_idx(new_idx);
	write_uint32(inner_len);
	write_uint32(msg_id);
	write_int64(id);
	set_write_idx(wri_idx);

	//buf.set_read_idx(read_idx);

}

size_t Block_Buffer::capacity(void) {
	return buffer_.capacity();
}

bool Block_Buffer::check_pack_len(void) {
	uint32_t len = 0, read_idx = read_index_;
	while(1) {
		this->peek_uint32(len);

		if (len == 0) {
			return false;
		}

		if (len + sizeof(uint32_t) == this->readable_bytes()) {
			read_index_ = read_idx;
			return true;
		}

		if (len + sizeof(uint32_t) > this->readable_bytes())
			return false;

		if (len + sizeof(uint32_t) < this->readable_bytes()) {
			this->set_read_idx(this->get_read_idx() + len + sizeof(uint32_t));
			continue;
		}
	}

	return false;
}

void Block_Buffer::finish_message(void) {
	uint32_t len = readable_bytes() - sizeof(uint32_t);

	int wr_idx = get_write_idx();
	set_write_idx(get_read_idx());
	write_uint32(len);
	set_write_idx(wr_idx);

	if (len == 0) {
		int msg_id = 0;
		this->peek_int32(msg_id, 4);
		LOG_TRACE_ABORT("len:%d, msg_id:%d", len, msg_id);
	}

	if (len > max_buff_pack_len) {
		int msg_id = 0;
		this->peek_int32(msg_id, 4);
		LOG_TRACE_ABORT("len:%d, msg_id:%d", len, msg_id);
	}

	if (len > max_warn_pack_len) {
		int msg_id = 0;
		this->peek_int32(msg_id, 4);
		LOG_USER("buffer len warn, msg:%d  len:%d", msg_id, len);
	}
}

void Block_Buffer::recycle_space(void) {
	if (max_use_times_ == 0)
		return;
	if (use_times_ >= max_use_times_) {
			std::vector<char> buffer_free(init_offset_ + init_size_);
 			buffer_.swap((std::vector<char>&&)buffer_free);
 			ensure_writable_bytes(init_offset_);
 			read_index_ = write_index_ = init_offset_;
			use_times_ = 0;
	}
}

bool Block_Buffer::verify_read(size_t s) {
	return (read_index_ + s <= write_index_) && (write_index_ <= buffer_.size());
}

inline void Block_Buffer::set_read_abort(bool value) {
	read_error_abort_ = value;
}

inline void Block_Buffer::log_binary_data(int len, int read_idx) {
	if (len <= 0 || read_idx < 0)
		return;

	int readable_byte = write_index_ - read_idx;
	if (readable_byte <= 0) return ;

	int real_len = (len > readable_byte) ? readable_byte : len;
	int end_index = read_idx + real_len;
	std::stringstream str_stream;
	char str_buf[32];
	for (int i = read_idx; i < end_index; ++i) {
		memset(str_buf, 0, sizeof(str_buf));
		snprintf(str_buf, sizeof(str_buf), "0x%02x", (uint8_t)buffer_[i]);
		str_stream << str_buf << " ";
	}
	LOG_DEBUG("[%s]", str_stream.str().c_str());
}

#endif /* BLOCK_BUFFER_H_ */
