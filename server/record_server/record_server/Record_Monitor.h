/*
 * Record_Monitor.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef RECORD_MONITOR_H_
#define RECORD_MONITOR_H_

#include "File_Record.h"
#include "Server_Monitor.h"
#include "Record_Message_Unit.h"

class Record_Monitor: public Server_Monitor {
public:

	Record_Monitor();
	virtual ~Record_Monitor();

	int init(int monitor_index);

	int start(void);

	virtual int start_monitor_timer(void);

	virtual Record_Monitor *record_monitor(void);

	virtual int process_monitor_close(void);

	virtual int monitor_link_drop(const int cid);

	File_Record &file_record(void);

private:
	Record_Message_Unit message_unit_;
	File_Record file_record_;
};

inline Record_Monitor *Record_Monitor::record_monitor(void) {
	return this;
};

inline File_Record &Record_Monitor::file_record(void) {
	return file_record_;
}

typedef Singleton<Record_Monitor> Record_Monitor_Single;
#define RECORD_MONITOR    (Record_Monitor_Single::instance())

#endif /* RECORD_MONITOR_H_ */
