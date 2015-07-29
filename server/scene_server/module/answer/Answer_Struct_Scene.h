/*
 * Answer_Struct_Scene.h
 *
 *  Created on: 2014年2月18日
 *      Author: xiaoliang
 */

#ifndef ANSWER_STRUCT_SCENE_H_
#define ANSWER_STRUCT_SCENE_H_

#include "Game_Typedef.h"

struct Answer_Rank_Role_Data {
	Answer_Rank_Role_Data(void) { reset(); }
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	role_id_t role_id;
	std::string name;
	int32_t level;
	uint32_t sorce;
	uint32_t true_num;
	uint32_t exp;
};

typedef boost::unordered_map<role_id_t, Answer_Rank_Role_Data> RoleID_Data_Map;
typedef std::vector<Answer_Rank_Role_Data> RoleID_Data_Vec;
typedef std::map<uint32_t, RoleID_Data_Vec, std::greater<uint32_t>> RoleID_Data_Vec_Map;

struct Answer_Role_Data_Info {
	Answer_Role_Data_Info(void) { reset(); }
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	uint32_t use_free_num;
	uint32_t use_double_num;
	uint32_t cur_free_qid;
	uint32_t cur_double_qid;
};

typedef boost::unordered_map<role_id_t, Answer_Role_Data_Info> RoleID_Answer_Role_Data_Info_Map;

struct Question_Info {
	Question_Info(void) { reset(); }
	void reset(void);

	uint32_t id;
	uint16_t status;
	std::string a;//选项A
	std::string b;//选项B
	uint16_t true_answer;//正确答案
	uint32_t over_time;
};

struct Answer_All_Data_Info {
	Answer_All_Data_Info(void) { reset(); }
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	UInt_Set question_id_list;
	RoleID_Data_Map role_sorce_map;//用户积分和基本信息
	RoleID_Answer_Role_Data_Info_Map role_data_info_map;//用户数据信息
	uint32_t ext_num;//已出即兴题数量
	uint32_t cur_qno;//当前处于第几题
	Time_Value last_start_time;
	String_Vec force_rank;
	String_Vec arena_rank;
	String_Vec gang_rank;
};

#endif /* ANSWER_STRUCT_SCENE_H_ */
