/*
 * Robot_Struct.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef ROBOT_STRUCT_H_
#define ROBOT_STRUCT_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"
#include "NPC/NPC_Config.h"

enum Tick_Loops {
	LOOP_INITING = 7,
	LOOP_WAITING_EXIT_DIFF_SPACE = 11,
	LOOP_TURN_SCENES = 7,
	LOOP_WAYFINDING = 15,
	LOOP_COLLECTING = 5,
	LOOP_FB_WAYFINDING = 5,
	LOOP_FB_WAYFIND_END = 2,

	LOOP_LOGIC_LINK = 20,
};

struct Robot_Detail {
	/// 属性类型
	enum Property_Type {
		ROLE_ID 		= 500001,	/// 角色ID
		ROLE_NAME 		= 500101,	/// 角色名
		CAREER 			= 500201,	/// 职业
		GENDER 			= 500301,	/// 性别
		GUILD 			= 500401,	/// 公会ID
		GENERALS 		= 500501,	/// 武将ID
		//GENDER 			= 500601,  	/// 出战武将ID
	};

	enum Status_Type {
		LOGIN = 0,  //登陆中
		IDLE = 1,	//空闲
		WAYFINDING = 2, //寻路中
		REWAYFIND = 3, //需要重新寻路
		TURN_SCENES = 4, //转场景中
		MOVE = 5,
		FIGHT = 6,
		WAITING_EXIT_DIFF_SPACE = 7,
		INITING = 8,
		COLLECTING = 9,
		FB_WAYFINDING = 10,
		FB_WAYFIND_END = 11,

		LOGOUT,

		INNER_OPING = 999, //内部操作中
	};

	enum Task_Status_Type {
		TASK_IDLE = 0, //空闲
		TASK_FETCHING = 1, //任务获取中
		TASK_FETCH_OK = 2, //任务获取完成
		TASK_FETCH_FAIL = 3, //任务获取失败
		TASK_CONFIRM = 4, //需要提交确认任务
		TASK_REDO = 5, //任务提交失败,重做
		TASK_TURN_SCENES = 6, //任务转场景
	};

	Robot_Detail(void);
	virtual ~Robot_Detail();
	void reset(void);

	role_id_t role_id;
	std::string account;
	std::string role_name;					// 角色名
	int level;     			   				// 等级[由战斗模块初始化]

	int vip;								// vip类型0-4
	uint8_t gender; 						// 性别0 = 女(female), 1 = 男(male)
	uint8_t career; 						// 职业 1-4	([1,4])
	Time_Value create_time;					// 创建角色时刻
	Time_Value last_sign_in_time;			// 最后登录时间
	Time_Value last_sign_out_time;			// 最后登出时间
	Time_Value last_trigger_daily_zero;		// 上次每日0点刷新时间
	Time_Value last_trigger_weekly_zero;	// 上次每周0点刷新时间



	faction_id_t faction_id;			///	帮派Id[由帮派模块初始化]
	std::string faction_name;  			///	帮派名称[由帮派模块初始化]
	int8_t online;						/// 在线状态[0:不在线 1:在线]
	Time_Value vip_deadline;			/// vip到期时间

	std::string ip;					/// IP

	int32_t status;
	int32_t sub_status;
	int32_t task_status;

	std::map<int, Monster_Point_Config> need_walk_monster_points;
	bool walked_mosters_points;
	int loop_exit_diff_space;
	int loop_transfer_scene;
	int loop_initing;
	int loop_path_finding;
	int loop_collecting;
	int loop_fb_wayfinding;
	int loop_fb_wayfind_end;

//	Walk_Path walk_path_;
//	Task_Map task_detail_;
//	Attack_Args attack_args_;
};

struct Client_MSG_Info {
	bool is_inited;
	int cid;
	unsigned int hash_key;				/// 用于加解密的hash key
	uint32_t msg_serial;				/// 上一条消息序号
	Time_Value msg_timestamp;			/// 上一条消息时间戳
	uint32_t msg_interval_count_;		/// 操作频率统计
	Time_Value msg_interval_timestamp;

	void reset(void) {
		is_inited = false;
		cid = -1;
		hash_key = 0;
		msg_serial = 0;
		msg_timestamp = Time_Value::zero;
		msg_interval_count_ = 0;
		msg_interval_timestamp = Time_Value::zero;
	}
};

struct Robot_Config {
	std::string name_prefix;

	std::string ip;
	int port;
	uint total_count;					// 账号总数量
	uint online_max;					// 机器人在线数量
	Time_Value batch_login_interval;

	Time_Value command_interval;
	Time_Value run_time;
	Time_Value chat_time;
	Time_Value send_msg_time; //随机发送消息间隔 ms
	uint8_t send_mode;    //发送模式, 0表示随机消息发送,1表示顺序消息发送
	uint8_t move_skip_range; //跨格子移动的个数
	uint8_t robot_def; ////机器人动作

	void reset(void) {
		ip = "";
		port = 0;
		total_count = 0;
		name_prefix = "";
		online_max = -1;
		batch_login_interval = Time_Value::zero;
		run_time = Time_Value::zero;
		command_interval = Time_Value::zero;
		chat_time	= Time_Value::zero;
		send_msg_time = Time_Value::zero;
		send_mode	= 0;
		move_skip_range = 0;
		robot_def = 0;
	}
};

#endif /* ROBOT_STRUCT_H_ */
