/*
 * Block_Pool_Group.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Root
 */

#ifndef BLOCK_POOL_GROUP_H_
#define BLOCK_POOL_GROUP_H_

#include "Object_Pool.h"
#include "Block_Buffer.h"
#include "Thread_Mutex.h"
#include "Lib_Log.h"
#include "Misc.h"
#include <vector>

class Block_Pool_Group {
public:
	typedef Object_Pool_Ex<Block_Buffer, Thread_Mutex> Block_Pool;
	typedef std::vector<Block_Pool *> Pool_Group;

	Block_Pool_Group(size_t group_size = 1)
	: group_size_(group_size), x_(0) {
		init();
	}

	virtual ~Block_Pool_Group(void) {
		fini();
	}

	int init(void) {
		pool_group_.clear();

		Block_Pool *pool = 0;
		for (size_t i = 0; i < group_size_; ++i) {
			if ((pool = new Block_Pool) == 0) {
				LOG_ABORT("new return 0 errno:%d", errno);
			}
			pool_group_.push_back(pool);
		}
		return 0;
	}

	int fini(void) {
		for (Pool_Group::iterator it = pool_group_.begin(); it != pool_group_.end(); ++it) {
			(*it)->clear();
		}
		pool_group_.clear();
		return 0;
	}

	Block_Buffer *pop_block(int cid) {
		/*
		if (++x_ > 500) {
			dump_size();
			x_ = 0;
		}
		*/
		Block_Buffer *buf = pool_group_[cid % group_size_]->pop();

		return buf;
	}

	int push_block(int cid, Block_Buffer *buf) {
		if (! buf) {
			LOG_USER_TRACE("buf == 0");
			return -1;
		}
		buf->recycle_space();
		return pool_group_[cid % group_size_]->push(buf);
	}

	void push_shrink(void) {
		for (size_t i = 0; i < group_size_; ++i) {
			pool_group_[i]->push_shrink();
		}
	}

	void dump_info(void) {
		LOG_TEXT("***pool_group_[%u]: ", group_size_);
		for (size_t i = 0; i < group_size_; ++i) {
			pool_group_[i]->debug_info();
		}
	}

	void dump_size(void) {
		size_t sum_size = 0;
		for (Pool_Group::iterator group_it = pool_group_.begin(); group_it != pool_group_.end(); ++group_it) {
			sum_size += (* group_it)->sum_size();
		}
		//std::cout << "******* " << (sum_size/(1024*1024)) << "MB" << std::endl;
	}

private:
	Pool_Group pool_group_;
	size_t group_size_;
	int x_;
};

#endif /* BLOCK_POOL_GROUP_H_ */
