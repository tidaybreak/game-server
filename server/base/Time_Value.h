// -*- C++ -*-
/*
 * aa.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: Root
 *
 *  此类大部分代码源自ACE_Time_Value
 */

#ifndef TIME_VALUE_H_
#define TIME_VALUE_H_

#include "Date_Time.h"

class Block_Buffer;

class Time_Value {
public:
	suseconds_t const static ONE_SECOND_IN_USECS = 1000000;
	long int const static ONE_MINUTE_IN_SECS = 60;
	long int const static ONE_HOURS_MINUTE = 60;
	long int const static ONE_HOUR_IN_SECS = (ONE_MINUTE_IN_SECS * ONE_HOURS_MINUTE);
	long int const static ONE_DAY_HOURS = 24;
	long int const static ONE_DAY_IN_SECS = (ONE_HOUR_IN_SECS * ONE_DAY_HOURS);
	long int const static ONE_WEEK_DAYS = 7;
	long int const static ONE_WEEK_IN_SECS = (ONE_DAY_IN_SECS * ONE_WEEK_DAYS);

	friend bool operator > (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator < (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator >= (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator <= (const Time_Value &tv1, const Time_Value &tv2);
	friend Time_Value operator - (const Time_Value &tv1, const Time_Value &tv2);
	friend Time_Value operator + (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator == (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator != (const Time_Value &tv1, const Time_Value &tv2);

public:
	static const Time_Value zero;
	static const Time_Value max;
	static Time_Value gettimeofday();
	static int sleep(const Time_Value &tv);

	explicit Time_Value(long sec = 0, long usec = 0);
	explicit Time_Value(const struct timeval &t);
	Time_Value(const Time_Value &tv);

	void set(long sec, long usec) {	this->tv_.tv_sec = sec; this->tv_.tv_usec = usec; }
	inline void set(const timeval &t) { this->tv_ = t; }
	inline void set(const Time_Value &tv) { this->tv_ = tv.get_tv(); }

	inline long sec(void) const { return this->tv_.tv_sec; }
	inline void sec(long sec)  { this->tv_.tv_sec = sec; }
	inline long usec(void) const  { return this->tv_.tv_usec; }
	inline void usec(long usec)  { this->tv_.tv_usec = usec; }

	inline long msec(void) const {return this->tv_.tv_sec * 1000 + this->tv_.tv_usec / 1000;}

	inline const timeval & get_tv(void) const { return this->tv_;	}

	Time_Value & operator += (const Time_Value &tv);
	Time_Value & operator -= (const Time_Value &tv);
	operator timespec() const;

	void normalize(void);

	void debug_dump(void) const;
	void debug_dump(char *str, size_t *sl);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

private:
	timeval tv_;
};

inline bool is_same_week(const Time_Value &tv1, const Time_Value &tv2) {
	tm tm1, tm2;
	time_t t1 = tv1.sec(), t2 = tv2.sec();

	if (std::abs(t1 - t2) > Time_Value::ONE_WEEK_IN_SECS) {
		return false;
	}

	if (tv1 > tv2) {
		std::swap(t1, t2);
	}

	localtime_r(&t1, &tm1);
	localtime_r(&t2, &tm2);

	if (tm1.tm_wday < tm2.tm_wday) {
		return true;
	} else if (tm1.tm_wday == tm2.tm_wday) {
		return tm1.tm_yday == tm2.tm_yday;
	}

	return false;
}

inline bool is_same_day(const Time_Value &tv1, const Time_Value &tv2) {
	tm tm1, tm2;
	time_t t1 = tv1.sec(), t2 = tv2.sec();

	localtime_r(&t1, &tm1);
	localtime_r(&t2, &tm2);

	if (tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon && tm1.tm_mday == tm2.tm_mday)
		return true;
	else
		return false;
}

inline bool is_same_hour(const Time_Value &tv1, const Time_Value &tv2) {
	tm tm1, tm2;
	time_t t1 = tv1.sec(), t2 = tv2.sec();

	localtime_r(&t1, &tm1);
	localtime_r(&t2, &tm2);

	if (tm1.tm_year == tm2.tm_year
			&& tm1.tm_mon == tm2.tm_mon
			&& tm1.tm_mday == tm2.tm_mday
			&& tm1.tm_hour == tm2.tm_hour)
		return true;
	else
		return false;
}

// 当前时间到指定时间相对时间
//下一个指定的 时,分,秒,星期 距离当前时间的秒数
inline Time_Value next_relative_time(int hour, int minute, int second, int week = -1) {
	Time_Value now(Time_Value::gettimeofday());
	Date_Time date_now(now);
	Date_Time date_des(date_now);
	date_des.hour(hour);
	date_des.minute(minute);
	date_des.second(second);

	if (date_now.time_sec() >= date_des.time_sec()) {
		date_des = Date_Time(Time_Value(date_des.time_sec() + Time_Value::ONE_DAY_IN_SECS));
	}

	if(week >= 0 && week <= 6) {
		long this_week = date_des.weekday(); //date_des是星期几 , date_des.weekday() = [0-6];
		int split_day = 0; /*距离下周几间隔多少天(忽略时分秒)*/
		if(this_week < week) {//指定星期在本周还没到
			split_day = abs(this_week - week);
		} else if(this_week > week) {//指定星期在本周已经过去
			split_day = Time_Value::ONE_WEEK_DAYS - (this_week - week);
		}
		if(split_day > 0) {
			date_des = Date_Time(Time_Value(date_des.time_sec() + split_day * Time_Value::ONE_DAY_IN_SECS));
		}
	}

	return Time_Value(date_des.time_sec() - date_now.time_sec());
}

//inline void get_zero_time(const Time_Value &tv1, Time_Value &tv2) {
//	int zeor_time = tv1.sec() + (86400 - (tv1.sec() + 28800) % 86400);
//	tv2.set(zeor_time, 0);
//}

inline int get_zero_time(const Time_Value &tv1, Time_Value &tv2) {
	Date_Time date_(tv1);
	date_.hour(0);
	date_.minute(0);
	date_.second(0);
	tv2.sec(date_.time_sec());
	return date_.isdst();
}

/// tv2和tv1相隔天数
inline int get_time_subdays(const Time_Value &tv1, const Time_Value &tv2) {
	Time_Value zero_time1;
	int isdst1 = get_zero_time(tv1, zero_time1);
	Time_Value zero_time2;
	int isdst2 = get_zero_time(tv2, zero_time2);
	return ((zero_time2.sec() - zero_time1.sec()) + ((isdst1 != isdst2)?Time_Value::ONE_HOUR_IN_SECS:0)) / Time_Value::ONE_DAY_IN_SECS;
}
//inline int get_time_subdays(const Time_Value &tv1, const Time_Value &tv2) {
//	int zero_time1 = tv1.sec() + (86400 - (tv1.sec() + 28800) % 86400);
//	int zero_time2 = tv2.sec() + (86400 - (tv2.sec() + 28800) % 86400);
//	return (zero_time2 - zero_time1) / 86400;
//}

#endif /* TIME_VALUE_H_ */
