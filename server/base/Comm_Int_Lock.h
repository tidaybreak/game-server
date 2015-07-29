/*
 * Comm_Int_Lock.h
 *
 * Created on: 2015-03-02
 *     Author: Root
 */

#ifndef _COMM_INT_LOCK_H_
#define _COMM_INT_LOCK_H_

#include "Thread_Mutex.h"
#include "Mutex_Guard.h"

class Comm_Int_Lock {
public:
	explicit Comm_Int_Lock(const int default_val = 0);
	int read_value();
	void write_value(const int write);

private:
	int value_;
	Thread_Mutex mutex_;
};

inline Comm_Int_Lock::Comm_Int_Lock(const int default_val) : value_(default_val) {

}

inline int Comm_Int_Lock::read_value() {
	GUARD_READ(MUTEX, OBJ, mutex_);
	return value_;
}

inline void Comm_Int_Lock::write_value(const int write) {
	GUARD_WRITE(MUTEX, OBJ, mutex_);
	value_ = write;
}

#endif //_COMM_INT_LOCK_H_
