/*
 * Answer.h
 *
 *  Created on: Jan 21, 2014
 *      Author: xiaoliang
 *
 *      新模块例子
 */

#ifndef ANSWER_H_
#define ANSWER_H_

#include "Logic_Player_Base.h"

class Answer : virtual public Logic_Player_Base {
public:
	Answer(void);
	virtual ~Answer();
	void reset(void);

//	// 创建角色时初始化
//	static int create_init(Answer_Detail &detail);
//
//	// 登录时detail赋值
//	void load_detail(Answer_Detail &detail);
//
//	// 保存时detail赋值
//	void save_detail(Answer_Detail &data);
//
//	Answer_Detail const &answer_detail(void) const;

	// 登录时module初始化,在这里发客户端消息无效
	void module_init(void);

	// 登录网关后后调用(只会的正常登录调用一次,顶号不会调)
	void sign_in(void);

	// 顶号时调用
	int sign_in_repeat(void);

	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	// 登出前调用
	void sign_out(void);

	// 场景加载完成
	int move_scene_completed(void);

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	int data_channle(Block_Buffer& buf);
	int get_integral_reward(uint32_t sorce);
	int get_rank_reward(uint16_t rank);

private:

};



#endif /* ANSWER_H_ */
