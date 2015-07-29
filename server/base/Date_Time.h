/*
 * DateTime.h
 *
 *  Created on: Jul 11, 2012
 *      Author: "ti"
 *
 *  拷贝自ACE_Date_Time
 */

#ifndef DATETIME_H_
#define DATETIME_H_

#include <string>
#include <time.h>
#include <sys/time.h>
#include <cstdlib>
#include "Core_Define.h"

class Time_Value;

class Date_Time {
public:
	/// Constructor initializes current time/date info.
	Date_Time(void);

	/// Constructor initializes with the given Time_Value
	explicit Date_Time(const Time_Value &timevalue);

	/// Constructor with init values, no check for validy
	/// Set/get portions of Date_Time, no check for validity.
	Date_Time(long day,
			long month = 0,
			long year = 0,
			long hour = 0,
			long minute = 0,
			long second = 0,
			long microsec = 0,
			long wday = 0,
			int isdst = -1);

	/// Update to the current time/date.
	void update(void);

	/// Update to the given Time_Value
	void update(const Time_Value &timevalue);

	/// Get day.
	long day(void) const;

	/// Set day.
	void day(long day);

	/// Get month.
	long month(void) const;

	/// Set month.
	void month(long month);

	/// Get year.
	long year(void) const;

	/// Set year.
	void year(long year);

	/// Get hour.
	long hour(void) const;

	/// Set hour.
	void hour(long hour);

	/// Get minute.
	long minute(void) const;

	/// Set minute.
	void minute(long minute);

	/// Get second.
	long second(void) const;

	/// Set second.
	void second(long second);

	/// Get microsec.
	long microsec(void) const;

	/// Set microsec.
	void microsec(long microsec);

	/// Get weekday.
	long weekday(void) const;

	/// Set weekday.
	void weekday(long wday);

	int isdst(void) const;

	/// Get sec
	time_t time_sec(void);

private:
	long day_;
	long month_;
	long year_;
	long hour_;
	long minute_;
	long second_;
	long microsec_;
	long wday_;
	int isdst_;
};




#endif /* DATETIME_H_ */
