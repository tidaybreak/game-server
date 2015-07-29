/*
 * Hash_Map.h
 *
 * Created on: 2013-01-07 11:57
 *     Author: Root
 */

#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include "Thread_Mutex.h"
#include "Mutex_Guard.h"
#include <boost/unordered_map.hpp>

template<class Key, class Value, class HSMUTEX>
class Hash_Map {
public:
    typedef boost::unordered_map<Key, Value> BASEMAP;
    typedef typename boost::unordered_map<Key, Value>::iterator iterator;
    typedef typename boost::unordered_map<Key, Value>::const_iterator const_iterator;
    typedef HSMUTEX Mutex;
public:
    explicit Hash_Map(const size_t size = boost::unordered_detail::default_bucket_count);

    std::size_t size(void) const;
    bool empty(void) const;
    int find(const Key &key, Value &value);
    bool exist(const Key &key);

    int bind(const Key &key, const Value &value);
    int rebind(const Key &key, const Value &value);

    int unbind(const Key &key);
    int unbind(const Key &key, Value &value);

    int unbind_all(void);
    void clear(void);

    HSMUTEX &mutex(void);

    iterator begin(void);
    iterator find(const Key &key);
    const_iterator begin(void) const;
    iterator end(void);
    const_iterator end(void) const;
    Value &operator[](const Key &k);

private:
    HSMUTEX mutex_;

    boost::unordered_map<Key, Value> base_map_;
};

template<class Key, class Value, class HSMUTEX>
inline Hash_Map<Key, Value, HSMUTEX>::Hash_Map(const size_t size) :
    base_map_(size)
{ /*NULL*/ }

template<class Key, class Value, class HSMUTEX>
inline std::size_t Hash_Map<Key, Value, HSMUTEX>::size(void) const {
    GUARD_READ(HSMUTEX, mon, *(const_cast<HSMUTEX *>(&(this->mutex_))));
    return this->base_map_.size();
}

template<class Key, class Value, class HSMUTEX>
inline bool Hash_Map<Key, Value, HSMUTEX>::empty(void) const {
    GUARD_READ(HSMUTEX, mon, *(const_cast<HSMUTEX *>(&(this->mutex_))));
    return this->base_map_.empty();
}

template<class Key, class Value, class HSMUTEX>
inline int Hash_Map<Key, Value, HSMUTEX>::find(const Key &key, Value &value) {
    GUARD_READ(HSMUTEX, mon, this->mutex_);

    iterator iter  = this->base_map_.find(key);
    if (iter == this->end())
        return -1;

    value = iter->second;
    return 0;
}

template<class Key, class Value, class HSMUTEX>
inline bool Hash_Map<Key, Value, HSMUTEX>::exist(const Key &key) {
    GUARD_READ(HSMUTEX, mon, this->mutex_);

    iterator iter  = this->base_map_.find(key);
    return (iter == this->end()) ? false : true;
}

template<class Key, class Value, class HSMUTEX>
inline int Hash_Map<Key, Value, HSMUTEX>::bind(const Key &key, const Value &value) {
    GUARD_WRITE(HSMUTEX, mon, this->mutex_);

    if (this->base_map_.insert(typename BASEMAP::value_type(key, value)).second == false)
        return -1;
    return 0;
}

template<class Key, class Value, class HSMUTEX>
inline int Hash_Map<Key, Value, HSMUTEX>::rebind(const Key &key, const Value &value) {
    GUARD_WRITE(HSMUTEX, mon, this->mutex_);

    this->base_map_.erase(key);
    if (this->base_map_.insert(typename BASEMAP::value_type(key, value)).second == false)
        return -1;
    return 0;
}

template<class Key, class Value, class HSMUTEX>
inline int Hash_Map<Key, Value, HSMUTEX>::unbind(const Key &key) {
    GUARD_WRITE(HSMUTEX, mon, this->mutex_)

    if (this->base_map_.erase(key) <= 0)
        return -1;
    return 0;
}

template<class Key, class Value, class HSMUTEX>
inline int Hash_Map<Key, Value, HSMUTEX>::unbind(const Key &key, Value &value) {
    GUARD_WRITE(HSMUTEX, mon, this->mutex_)

    iterator iter  = this->base_map_.find(key);
    if (iter == this->end())
        return -1;

    value = iter->second;
    this->base_map_.erase(iter);
    return 0;
}

template<class Key, class Value, class HSMUTEX>
inline int Hash_Map<Key, Value, HSMUTEX>::unbind_all(void) {
    GUARD_WRITE(HSMUTEX, mon, this->mutex_)

    this->base_map_.clear();
    return 0;
}

template<class Key, class Value, class HSMUTEX>
inline void Hash_Map<Key, Value, HSMUTEX>::clear(void) {
    this->unbind_all();
}

template<class Key, class Value, class HSMUTEX>
inline HSMUTEX &Hash_Map<Key, Value, HSMUTEX>::mutex(void) {
    return this->mutex_;
}

/// 返回大于1.5 * num的第一个素数
inline size_t get_hash_table_size(unsigned int num) {
    size_t return_num = 1.5 * num, i, j;
    while (1) {
        /// 素数判断
        j = (size_t)sqrt(return_num++);
        for (i = 2; i < j; ++i) {
            if (return_num % i == 0)
                break;
        }
        if (i == j)
            return return_num;
    }
    return 0;
}

template<class Key, class Value, class HSMUTEX>
inline typename Hash_Map<Key, Value, HSMUTEX>::iterator Hash_Map<Key, Value, HSMUTEX>::begin(void) {
    return this->base_map_.begin();
}

template<class Key, class Value, class HSMUTEX>
inline typename Hash_Map<Key, Value, HSMUTEX>::iterator Hash_Map<Key, Value, HSMUTEX>::find(const Key &key) {
    return this->base_map_.find(key);
}

template<class Key, class Value, class HSMUTEX>
inline typename Hash_Map<Key, Value, HSMUTEX>::const_iterator Hash_Map<Key, Value, HSMUTEX>::begin(void) const {
    return this->base_map_.begin();
}

template<class Key, class Value, class HSMUTEX>
inline typename Hash_Map<Key, Value, HSMUTEX>::iterator Hash_Map<Key, Value, HSMUTEX>::end(void) {
    return this->base_map_.end();
}

template<class Key, class Value, class HSMUTEX>
inline typename Hash_Map<Key, Value, HSMUTEX>::const_iterator Hash_Map<Key, Value, HSMUTEX>::end(void) const {
    return this->base_map_.end();
}

template<class Key, class Value, class HSMUTEX>
inline Value& Hash_Map<Key, Value, HSMUTEX>::operator[](const Key &k) {
    return this->base_map_[k];
}

#endif //_HASH_MAP_H_
