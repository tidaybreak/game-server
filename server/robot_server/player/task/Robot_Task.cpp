/*
 * Robot_Task.cpp
 *
 *  Created on: 2014-06-07
 *      Author: xiaoliang
 */

#include "Robot_Task.h"
#include "Config_Cache.h"
#include "Config_Cache_Robot.h"
#include "Robot_Player.h"
#include "Msg_Task_Struct.h"
#include "Config_Cache_NPC.h"

Robot_Task::Robot_Task() {
	reset();
}

void Robot_Task::reset(void) {
	task_info_.reset();
	analyze_progress = 0;
	do_num = 0;
}

Robot_Task::~Robot_Task() {

}

void Robot_Task::do_task(Time_Value &now) {
	Robot_Player *player = (Robot_Player*)this;
	switch(player->base_detail().task_status) {
		case Robot_Detail::TASK_IDLE: {
			//请求任务
			player->base_detail().task_status = Robot_Detail::TASK_FETCHING;
			req_fetch_task_list();
			break;
		}
		case Robot_Detail::TASK_FETCH_OK: {
			MSG_USER("当前任务ID:%d", task_info_.id);
			analyze_task();

			break;
		}
	}
}

void Robot_Task::analyze_task(void) {
	const Task_Mission_Detail_Config *conf = CONFIG_CACHE_TASK->find_task(task_info_.id);
	if(conf) {
		if(conf->execute.size() <= analyze_progress) {
			//todo 完成任务了
			return;
		}
		Task_Mission_Exec_Detail_Cfg exe_cfg = conf->execute[analyze_progress];
		switch(exe_cfg.type) {
			case 111112: {
				do_111112(exe_cfg.id, exe_cfg.amount);
				break;
			}
		}
	}
}

void Robot_Task::do_111112(uint32_t id, uint32_t num) {
	Robot_Player *player = (Robot_Player*)this;
	player->base_detail().task_status = Robot_Detail::TASK_TURN_SCENES;
	if(player->wayfind_info().cur_location.scene_id != (int32_t)id) {
		player->wayfind_info().det_scene_id = id;
		player->base_detail().status = Robot_Detail::TURN_SCENES;
		return;
	}
}


void Robot_Task::res_fetch_task_list(std::vector<Task_Info> &tasks) {
	Robot_Player *player = (Robot_Player*)this;
	bool have_task = false;
	for(std::vector<Task_Info>::iterator iter = tasks.begin(); iter != tasks.end(); ++iter) {
		if(iter->type == 101 && iter->status) {
			task_info_ = *iter;
			player->base_detail().task_status = Robot_Detail::TASK_FETCH_OK;
			have_task = true;
			break;
		}
	}
	if(!have_task) {
		player->base_detail().task_status = Robot_Detail::TASK_FETCH_FAIL;
	}

	MSG_USER(">>> 请求任务列表（返回） ");
}

void Robot_Task::req_fetch_task_list(void) {
	Robot_Player *player = (Robot_Player*)this;

	MSG_10110000 msg;
	Block_Buffer buf;
	buf.make_client_message(10110000, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg.serialize(buf);
	buf.finish_message();
	player->send_to_server(buf);

	MSG_USER(">>> 请求任务列表 ");

	player->base_detail().status = Robot_Detail::INITING;
}

void Robot_Task::active_task_list_update(const uint8_t operate, const Task_Info& task_info) {
	if (task_info.type != 101 || !task_info.status) {
		return;
	}
	switch (operate) {
	case 1: {
		// add
		task_info_.reset();
		task_info_ = task_info;
		break;
	}
	case 2: {
		// sub
		task_info_.reset();
		break;
	}
	case 3: {
		// update
		task_info_.reset();
		task_info_ = task_info;
		break;
	}
	default:
		return;
	}
}
