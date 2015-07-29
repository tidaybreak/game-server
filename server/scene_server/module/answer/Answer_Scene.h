/*
 * Answer_Scene.h
 *
 *  Created on: 2014年8月13日
 *      Author: xiaoliang
 */

#ifndef ANSWER_SCENE_H_
#define ANSWER_SCENE_H_


#include "Move_Scene.h"
#include "answer/Answer_Def.h"
#include "Answer_Struct_Scene.h"

struct Answer_Rank_Role_Info;

class Answer_Scene: public Move_Scene {

	enum QUESTION_STATUS {
		QUESTION_STATUS_INIT = 0,//初始化状态
		QUESTION_STATUS_READ = 1,//阅题状态
		QUESTION_STATUS_ANSWER = 2,//答题状态
		QUESTION_STATUS_ANN = 3,//公布答案状态
//		QUESTION_STATUS_WAIT = 99,//等待结算答案
		QUESTION_STATUS_OK = 100,//答案结算完成
	};

public:
	Answer_Scene();
	virtual ~Answer_Scene();
public:
	void reset(void);
	virtual int tick(const Time_Value &now);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	void kick_all_palyer(void);

	virtual int enter_scene(Mover *mover);
	virtual int exit_scene(Mover *mover, bool offline);

	virtual int client_scene_complete(Fighter &fighter);

	void init_scene_data(const Time_Value &stime, const Answer_All_Data_Info &answer_info_);
	void scene_close_note(const Time_Value &etime);

	int req_use_answer_free(Scene_Player &player);
	int req_use_answer_double(Scene_Player &player);
	int req_answer_free_double_num(Scene_Player &player);

private:

	void check_make_question(const Time_Value &now);//开始生成题目
	void update_question_status(const Time_Value &now);//检测题目状态
	uint32_t rand_make_question(void);//随机生成题目
	int make_active_question(uint32_t qid);//出题时候对即兴题目进行处理
	int make_active_question_on_fulfil(uint32_t qid);//题目结算时候对即兴题进行处理
	int fulfil_active_question();//结算题目
	uint32_t get_ones_time_sec(void);//得到一题的耗时
	void refresh_sorce(void);//刷新分数
	void refresh_rank(void);//刷新排行榜
	uint32_t get_mover_xy_to_answer(Mover *mover);//得到玩家是在左边还是在右边, 1.左边; 2右边
	bool get_is_set_free(Mover *mover);//得到玩家是否有使用免答
	uint8_t get_is_set_multiple(Mover *mover);//得到玩家当前题目的积分倍数

	void send_to_logic_save_data(void);//更改数据发送到逻辑服务器存库
	void send_to_logic_get_rank_reward(void);
	void send_client_question_info(void);
	void send_client_question_info(Mover *mover);
	void send_client_rank_info(void);
	void send_client_rank_info(Mover *mover);
	void send_client_rank_info_msg_rank(std::vector<Answer_Rank_Role_Info> &rank);
	void send_logic_get_integral_reward(Mover *mover, const uint32_t sorce);//通知获取单题目结束奖励(积分)

	////////////////////为即兴题准备的接口 start////////////////
	void get_a_and_b_num(uint32_t &a, uint32_t &b);//累加得到左右两边人数
	void get_a_and_b_male_num(uint32_t &a, uint32_t &b);//累加得到左右两边男性人数
	void get_a_and_b_female_num(uint32_t &a, uint32_t &b);//累加得到左右两边女性人数
	void get_male_and_female_num(uint32_t &male, uint32_t &female);//累加得到男性he女性人数
	////////////////////为即兴题准备的接口 end////////////////


	Answer_All_Data_Info answer_info;
/////////////////////////////////////////////
	RoleID_Data_Vec_Map role_sorce_rank;//用户积分排行榜基本信息
	Time_Value start_time;
	Time_Value end_time;
	Question_Info q;
//	uint32_t this_qid;//当前题目ID
//	uint8_t this_qstatus;//当前题目状态
};



#endif /* ANSWER_SCENE_H_ */
