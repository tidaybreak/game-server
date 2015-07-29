/*
 * Robot_Task.h
 *
 *  Created on: Dec 17, 2013
 *      Author: xiaoliang
 */

#ifndef ROBOT_TASK_H_
#define ROBOT_TASK_H_

#include "Game_Typedef.h"
#include "Msg_Struct.h"
#include "Config_Cache_Task.h"

class Robot_Task {
public:
	Robot_Task();
	virtual ~Robot_Task();
	void reset(void);

	inline const Task_Info& task_info(void) const;

	void do_task(Time_Value &now);
	void req_fetch_task_list(void);
	void res_fetch_task_list(std::vector<Task_Info> &tasks);
	void analyze_task(void);

	void active_task_list_update(const uint8_t operate, const Task_Info& task_info);

	void do_111112(uint32_t id, uint32_t num);



protected:
	Task_Info task_info_;
	uint32_t analyze_progress;
	uint32_t do_num;
};

//===============================================================

inline const Task_Info& Robot_Task::task_info(void) const {
	return task_info_;
}

#endif /* ROBOT_TASK_H_ */
