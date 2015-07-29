// -*- C++ -*-
/*
 * Object_Pool.h
 *
 *  Created on: Mar 23, 2012
 *      Author: Root
 */

#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include "Thread_Mutex.h"
#include "Mutex_Guard.h"
#include "Lib_Log.h"
#include <list>
#include <algorithm>
#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"

template <typename Obj, typename LOCK = Spin_Lock>
class Object_Pool {
	typedef std::list<Obj *> Obj_List;
	typedef boost::unordered_set<Obj *> Obj_Set;

	const static int used_obj_list_bucket = 2000;

public:
	Object_Pool(void);

	Object_Pool(int shurink_level,
			int shrink_delay_level,
			int dump_log_interval,
			size_t used_list_bucket);

	virtual ~Object_Pool(void);

	Obj *pop(void);

	int push(Obj *obj);

	void clear(void);

	int all_obj_sizeof(void);

	void push_shrink(void);

	void shrink_rate(double rate = 0.8);

	void shrink_all(void);

	void shrink_all_i(void);

	void dump_info(void);

	void dump_info_i(void);

	void debug_info(void);

	size_t sum_size(void);

	size_t free_obj_list_size(void);

	size_t used_obj_list_size(void);

	void xxx_info(void);

private:
	void shrink_info_dump(void);


private:
	Obj_List free_obj_list_;
	size_t free_obj_list_size_;

	Obj_Set used_obj_list_; /// 使用hash_set防止push时std::list::remove操作的低效

	LOCK lock_;

	int shrink_level_;			/// 收缩水位(默认10万个)

	int shrink_delay_level_;		/// 收缩延迟次数水平(默认1万次)
	int shrink_delay_;			/// 收缩延迟次数

	int dump_log_interval_;		/// 日志记录间隔
	int dump_log_;				/// 操作次数
};

template <typename Obj, typename LOCK>
Object_Pool<Obj, LOCK>::Object_Pool(void)
: free_obj_list_size_(0),
  used_obj_list_(used_obj_list_bucket),
  shrink_level_(100000),
  shrink_delay_level_(10000),
  shrink_delay_(0),
  dump_log_interval_(500),
  dump_log_(0)
{ }

template <typename Obj, typename LOCK>
Object_Pool<Obj, LOCK>::Object_Pool(int shrink_level,
		int shrink_delay_level,
		int dump_log_interval,
		size_t used_list_bucket)
: free_obj_list_size_(0),
  used_obj_list_(used_list_bucket),
  shrink_level_(shrink_level),
  shrink_delay_(0),
  shrink_delay_level_(shrink_delay_level),
  dump_log_interval_(dump_log_interval),
  dump_log_(0)
{ }

template <typename Obj, typename LOCK>
Object_Pool<Obj, LOCK>::~Object_Pool(void) {
	this->clear();
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::push_shrink(void) {
	GUARD(LOCK, mon, this->lock_);
	if (used_obj_list_.size() + free_obj_list_size_ > 0) {
		double free_rate = (free_obj_list_size_ * 1.0 )/(used_obj_list_.size() + free_obj_list_size_);
		if (free_rate > 0.8) {
			shrink_rate(0.5);
		}
	}
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::shrink_rate(double rate) {
	int shrink_num = free_obj_list_size_ * rate;

	Obj *obj = 0;
	for (int i = 0; i < shrink_num; ++i) {
		obj = free_obj_list_.front();
		free_obj_list_.pop_front();
		--free_obj_list_size_;
		delete obj;
	}
	if (shrink_num > 0)
		shrink_delay_ = 0;
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::shrink_all(void) {
	GUARD(LOCK, mon, this->lock_);
	shrink_all_i();
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::shrink_all_i(void) {
	for (typename Obj_List::iterator it = free_obj_list_.begin(); it != free_obj_list_.end(); ++it) {
		delete *it;
	}
	free_obj_list_.clear();
	free_obj_list_size_ = 0;
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::dump_info(void) {
	GUARD(LOCK, mon, this->lock_);
	dump_info_i();
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::dump_info_i(void) {
	GUARD(LOCK, mon, this->lock_);
	if (++dump_log_ > dump_log_interval_) {
		LOG_DEBUG("free_obj_list_siz = %u, used_obj_list_size = %u", free_obj_list_size_, used_obj_list_.size());
		dump_log_ = 0;
	}
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::debug_info(void) {
	GUARD(LOCK, mon, this->lock_);
	LOG_TEXT("free_obj_list_size_ = %u, free_obj_list_size_ = %u, used_size = %u, shrink_level_ = %u, shrink_delay_level_ = %u, shrink_delay_ = %u",
			free_obj_list_size_, free_obj_list_.size(), used_obj_list_.size(), shrink_level_, shrink_delay_level_, shrink_delay_);
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::xxx_info(void) {
	if (++shrink_delay_ > 500) {
		LOG_DEBUG("free_obj_list_size_ = %u, used_size = %u, SUM = %u", free_obj_list_size_, used_obj_list_.size(), free_obj_list_size_ + used_obj_list_.size());
		shrink_delay_ = 0;
	}
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::shrink_info_dump(void) {
/*	std::cerr << "free_obj_list_size_ = " << free_obj_list_size_
			<< " shrink_level_ = " << shrink_level_
			<< " shrink_delay_level_ = " << shrink_delay_level_
			<< " shrink_delay_ = " << shrink_delay_ << std::endl;*/
}

template <typename Obj, typename LOCK>
Obj * Object_Pool<Obj, LOCK>::pop(void) {
#ifdef NO_CACHE
	Obj *obj = new Obj;
#else
	GUARD(LOCK, mon, this->lock_);

	Obj *obj = 0;
	if (this->free_obj_list_size_ != 0) {
		obj = this->free_obj_list_.front();
		this->free_obj_list_.pop_front();
		if (! this->used_obj_list_.insert(obj).second) {
			LOG_USER_TRACE("insert failure !");
		}
		--(this->free_obj_list_size_);
	} else {
		while (1) {
			if ((obj = new Obj) == 0) {
				LOG_USER_TRACE("new return 0");
				continue;
			} else {
				if (! this->used_obj_list_.insert(obj).second) {
					LOG_USER_TRACE("insert failure !");
				}
				break;
			}
		}
	}
#endif
	return obj;
}

template <typename Obj, typename LOCK>
int Object_Pool<Obj, LOCK>::push(Obj *obj) {
#ifdef NO_CACHE
	delete obj;
#else
	GUARD(LOCK, mon, this->lock_);

	if (obj == 0)
		return -1;

	typename Obj_Set::iterator used_obj_it = this->used_obj_list_.find(obj);
	if (used_obj_it == this->used_obj_list_.end()) {
		LOG_TRACE_ABORT("***** Object_Pool<Obj, LOCK>::push(Obj *obj) can't find this Object Block !!! free:%d, used:%d", free_obj_list_size_, used_obj_list_.size());
		return -1;
	}

	this->used_obj_list_.erase(used_obj_it);
	this->free_obj_list_.push_back(obj);
	++free_obj_list_size_;
#endif
	return 0;
}

template <typename Obj, typename LOCK>
void Object_Pool<Obj, LOCK>::clear(void) {
	GUARD(LOCK, mon, this->lock_);

	for (typename Obj_List::iterator it = this->free_obj_list_.begin();
			it != this->free_obj_list_.end(); ++it) {
		delete *it;
	}
	for (typename Obj_Set::iterator it = this->used_obj_list_.begin();
			it != this->used_obj_list_.end(); ++it) {
		delete *it;
	}

	this->free_obj_list_size_ = 0;
	this->free_obj_list_.clear();
	this->used_obj_list_.clear();
}

template <typename Obj, typename LOCK>
int Object_Pool<Obj, LOCK>::all_obj_sizeof(void) {
	return (sizeof(Obj) * (free_obj_list_size() + used_obj_list_size()))/(1000 * 1000);
}

template <typename Obj, typename LOCK>
size_t Object_Pool<Obj, LOCK>::sum_size(void) {
	GUARD(LOCK, mon, this->lock_);

	size_t sum_size = 0;
	for (typename Obj_List::iterator it = free_obj_list_.begin(); it != free_obj_list_.end(); ++it) {
		sum_size += (*it)->capacity();
	}
	for (typename Obj_Set::iterator it = used_obj_list_.begin(); it != used_obj_list_.end(); ++it) {
		sum_size += (*it)->capacity();
	}
	return sum_size;
}

template <typename Obj, typename LOCK>
size_t Object_Pool<Obj, LOCK>::free_obj_list_size(void) {
	return this->free_obj_list_size_;
}

template <typename Obj, typename LOCK>
size_t Object_Pool<Obj, LOCK>::used_obj_list_size(void) {
	return this->used_obj_list_.size();
}



///////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename CObject, typename LOCK = RE_Mutex>
class Object_Pool_Ex : public Object_Pool<CObject, LOCK> {
public:
    typedef Object_Pool<CObject, LOCK> SUPPER_POOL;
public:
    virtual ~Object_Pool_Ex(void);
    virtual CObject *pop(void);
    virtual int push(CObject *obj);
    void debug(int act, int type);
    void show_debug(void);

private:
    boost::unordered_map<int, int> debug_;
};

template<typename CObject, typename LOCK>
Object_Pool_Ex<CObject, LOCK>::~Object_Pool_Ex(void) { /*NULL*/ }

template<typename CObject, typename LOCK>
inline CObject *Object_Pool_Ex<CObject, LOCK>::pop(void) {
    CObject *obj = SUPPER_POOL::pop();
    if (obj == 0)
        return obj;

    obj->reset();
    return obj;
}

template<typename CObject, typename LOCK>
int Object_Pool_Ex<CObject, LOCK>::push(CObject *obj) {
    if (obj == 0) {
        //LOG_USER("ERROR push obejct is null: %s", typeid(CObject).name());
        return -1;
    }

    obj->reset();
    return SUPPER_POOL::push(obj);
}

template<typename CObject, typename LOCK>
void Object_Pool_Ex<CObject, LOCK>::debug(int act, int type) {
	if (act) {
		debug_[type]++;
	} else {
		debug_[type]--;
	}
}

template<typename CObject, typename LOCK>
void Object_Pool_Ex<CObject, LOCK>::show_debug(void) {
	for (boost::unordered_map<int, int>::iterator it = debug_.begin(); it != debug_.end(); ++it) {
		LOG_TEXT("	  type:%d  amount:%d", it->first, it->second);
	}
}

#endif /* OBJECT_POOL_H_ */
