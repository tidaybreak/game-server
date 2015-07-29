/*
 * Blocking_Queue.h
 *
 *  Created on: Sep 3, 2014
 *      Author: ti
 */


#ifndef _BLOCKIN_GQUEUE_H_
#define _BLOCKIN_GQUEUE_H_

#include <queue>
#include <pthread.h>

template<class Obj>
class Blocking_Queue {
public:
	typedef std::queue<Obj> Item_Queue;

    Blocking_Queue();

    virtual ~Blocking_Queue();

    size_t size(void);
    int push(const Obj &item);
    int pop(Obj &item);

protected:
    void swap(void);

private:
    pthread_mutexattr_t attr;
    pthread_mutex_t _mutex;

    pthread_cond_t _cond;

    Item_Queue *read_queue_;

    Item_Queue *write_queue_;

    Item_Queue _read_queue;

    Item_Queue _write_queue;

    size_t read_size_;
    size_t write_size_;
};


template<class Obj>
Blocking_Queue<Obj>::Blocking_Queue() {
    pthread_mutex_init(&this->_mutex,NULL);
    pthread_cond_init(&this->_cond,NULL);

	this->read_queue_ = &(this->_read_queue);
	this->read_size_ = 0;
	this->write_queue_ = &(this->_write_queue);
	this->write_size_ = 0;
}

template<class Obj>
Blocking_Queue<Obj>::~Blocking_Queue() {
    pthread_mutex_destroy(&this->_mutex);
    pthread_cond_destroy(&this->_cond);
}

template<class Obj>
int Blocking_Queue<Obj>::pop(Obj &item) {

    if( !this->read_queue_->empty() ) {
        item = this->read_queue_->front();
        this->read_queue_->pop();
    } else {
        pthread_mutex_lock(&this->_mutex);

        while(this->write_queue_->empty())
        {
            pthread_cond_wait(&this->_cond,&this->_mutex);
        }

        this->swap();

        pthread_mutex_unlock(&this->_mutex);

        return -1;
    }


    return 0;
}

template<class Obj>
inline size_t Blocking_Queue<Obj>::size(void) {
    return _read_queue.size();
}

template<class Obj>
int Blocking_Queue<Obj>::push(const Obj &item) {
    pthread_mutex_lock(&this->_mutex);
    this->write_queue_->push(item);
    pthread_cond_signal(&this->_cond);
    pthread_mutex_unlock(&this->_mutex);

    ++(this->write_size_);

    return 0;
}

template<class Obj>
inline void Blocking_Queue<Obj>::swap(void) {
	Item_Queue *tmp_queue = this->write_queue_;
    this->write_queue_ = this->read_queue_;
    this->read_queue_ = tmp_queue;

    this->write_size_ = this->write_queue_->size();
    this->read_size_ = this->read_queue_->size();
}


#endif
/* BLOCKING_QUEUE_H_ */
