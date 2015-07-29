/*
 * Answer_Manager.h
 *
 *  Created on: 2014年8月14日
 *      Author: root
 */

#ifndef ANSWER_MANAGER_H_
#define ANSWER_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "answer/Answer_Struct_Scene.h"
#include "Answer_Def.h"

class Logic_Player;

class Answer_Manager{
public:
	Answer_Manager(void);
	~Answer_Manager(void);

	int start(void);
	int tick(const Time_Value &now);

	void open_active(const Time_Value &st, const Time_Value &et, const Time_Value &bt, const uint32_t lv_limit);
	void close_active(const Time_Value &et);
	const Time_Value &get_start_time(void);
	const Time_Value &get_end_time(void);
	uint32_t get_continued_time_sec(void);
	int is_open(Logic_Player *player);//活动是否正在开启

	int data_channle(Block_Buffer& buf);

private:
	void open_active_make_answer_info(Answer_All_Data_Info &answer_info);
	void send_scene_open_action(void);
	void send_scene_close_action(void);
	void reward_to_player(role_id_t role_id, uint16_t rank);
	void save_data(Block_Buffer& buf);


	Time_Value start_time;
	Time_Value end_time;
	Time_Value bef_start_time;
	uint32_t level_limit;
	bool is_close;
	///save
	Answer_All_Data_Info answer_info;
};


typedef Singleton<Answer_Manager> Answer_Manager_Single;
#define ANSWER_MANAGER (Answer_Manager_Single::instance())


#endif /* ANSWER_MANAGER_H_ */
