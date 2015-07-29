/*
 * Double_Queue.h
 *
 * Created on: 2012-05-26 19:51
 *     Author: lyz
 */

#ifndef _DOUBLE_QUEUE_H_
#define _DOUBLE_QUEUE_H_

#include <queue>
#include "Thread_Mutex.h"
#include "Mutex_Guard.h"

template<class Obj>
class Double_Queue {
public:
    typedef std::queue<Obj> Item_Queue;

    Double_Queue(void);
    ~Double_Queue(void);

    void clear(void);
    bool empty(void);
    size_t size(void);
    int push(const Obj &item);
    int pop(Obj &item);

protected:
    void swap(void);

protected:
    Item_Queue one_queue_;
    Item_Queue two_queue_;
    Item_Queue *read_queue_;
    Item_Queue *write_queue_;

    RW_MUTEX mutex_;

    RE_MUTEX write_mutex_;
    size_t read_size_;
    size_t write_size_;
};


template<class Obj>
inline Double_Queue<Obj>::Double_Queue(void) {
	this->read_queue_ = &(this->one_queue_);
	this->read_size_ = 0;
	this->write_queue_ = &(this->two_queue_);
	this->write_size_ = 0;
}

template<class Obj>
inline Double_Queue<Obj>::~Double_Queue(void) { /*NULL*/ }

template<class Obj>
inline void Double_Queue<Obj>::clear(void) {
    GUARD_WRITE(RW_MUTEX, mon_1, this->mutex_);
    GUARD(RE_MUTEX, mon_2, this->write_mutex_);
	while (this->one_queue_.empty() == false)
		this->one_queue_.pop();
	while (this->two_queue_.empty() == false)
		this->two_queue_.pop();
}

template<class Obj>
inline bool Double_Queue<Obj>::empty(void) {
	size_t read_size = this->read_size_, write_size = this->write_size_;
    if (read_size > 0 || write_size > 0)
        return false;

    return true;
}

template<class Obj>
inline size_t Double_Queue<Obj>::size(void) {
	size_t read_size = this->read_size_, write_size = this->write_size_;
    return (read_size + write_size);
}

template<class Obj>
inline int Double_Queue<Obj>::push(const Obj &item) {
    GUARD_READ(RW_MUTEX, mon_1, this->mutex_);
    GUARD(RE_MUTEX, mon_2, this->write_mutex_);

    this->write_queue_->push(item);
    ++(this->write_size_);

    return 0;
}

template<class Obj>
inline int Double_Queue<Obj>::pop(Obj &item) {
    if (this->empty() == true)
        return -1;

    {
        GUARD_READ(RW_MUTEX, mon, this->mutex_);

        if (this->read_queue_->empty() == false)
        {
            item = this->read_queue_->front();
            this->read_queue_->pop();
            --this->read_size_;

            return 0;
        }
    }

    if (this->size() <= 0)
        return -1;

    {
        GUARD_WRITE(RW_MUTEX, mon_1, this->mutex_);
        GUARD(RE_MUTEX, mon_2, this->write_mutex_);
        this->swap();
//        if (this->read_queue_->empty() == false)
//        {
//            item = this->read_queue_->front();
//            this->read_queue_->pop();
//            --this->read_size_;
//
//            return 0;
//        }
    }

    return -1;
}

template<class Obj>
inline void Double_Queue<Obj>::swap(void) {
    Item_Queue *tmp_queue = this->write_queue_;
    this->write_queue_ = this->read_queue_;
    this->read_queue_ = tmp_queue;

    this->write_size_ = this->write_queue_->size();
    this->read_size_ = this->read_queue_->size();
}

#endif //_DOUBLE_QUEUE_H_
