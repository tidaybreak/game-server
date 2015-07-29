/*
 * server_define.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef SERVER_DEFINE_H_
#define SERVER_DEFINE_H_

#define LOGIN_GATE_TIMEOUT 15
#define SAVE_DETAIL_INTERVAL 60
#define MAP_X_PIEX 32
#define MAP_Y_PIEX 32
#define HALF_MAP_X_PIEX 16
#define HALF_MAP_Y_PIEX 16

// 按X坐标计算像素
inline int grid_x_to_pixel(int grid_x) {
	return grid_x * MAP_X_PIEX + HALF_MAP_X_PIEX;
}

// 按Y坐标计算像素
inline int grid_y_to_pixel(int grid_y) {
	return grid_y * MAP_Y_PIEX + HALF_MAP_Y_PIEX;
}

//  按X坐标计算格子
inline int pixel_x_to_grid(int pixel_x) {
	return pixel_x / MAP_X_PIEX;
}

//  按Y坐标计算格子
inline int pixel_y_to_grid(int pixel_y) {
	return pixel_y / MAP_Y_PIEX;
}

inline int get_msg_type(int msg_id) {
	return msg_id / 100000;
}

namespace Server_Enum {
	// monitor type
	enum MONITOR_TYPE {
		MONITOR_ALL		= 0,
		MONITOR_LOGIC	= 1,
		MONITOR_SCENE	= 2,
		MONITOR_CHAT	= 3,
		MONITOR_GATE	= 4,
		MONITOR_RECORD	= 5,
		MONITOR_ROBOT	= 6,
		MONITOR_END
	};

	// monitor status
    enum MONITOR_STATUS {
    	MONITOR_STATUS_PREPARE = 0,						// 正常状态
    	MONITOR_STATUS_NORMAL,							// 正常状态
    	MONITOR_STATUS_CLOSING,							// 关闭中..
    };

    // message type
    enum SERVER_MSG_TYPE {
    	CLIENT_TO_LOGIC		=	101,
    	CLIENT_TO_SCENE		=	102,
    	CLIENT_TO_CHAT		=	103,
    	CLIENT_TO_GATE		=	104,
    	CLIENT_TO_RECORD	=	105,

    	CLIENT_HEART		=	999,
    };

    // main message
    enum SERVER_INNER_MSG {
    	SERVER_CLIEN_KEEP_ALIVE     = 99999999,

    	SERVER_INNER_LINK_CLOSE		= 20000000,		// 内部链接关闭
    	SERVER_INNER_LINK_BUILD		= 20000010,		// 链接建立
    	SERVER_INNER_CLOSE_TIP		= 20000019,		// 服务器关闭tip
    	SERVER_INNER_SELF_CLOSE		= 20000020,		// 服务器关闭
    	SERVER_INNER_CLIENT_CLOSE	= 20000030,		// 定时关闭客户端
    	SERVER_INNER_CONFIG_UPDATE	= 20000040,		// 配制文件热更
    	SERVER_INNER_HEARTBEAT		= 20000050,		// server间内部心跳
    	SERVER_INNER_HEARTBEAT_BACK	= 20000052,		// server间内部心跳返回
    	SERVER_INNER_ABNORMAL		= 20000053,		// 链接异常断线
    	SERVER_INNER_MSG_TEST		= 20000055,		// 测试通不通消息


    	LOGIC_SCENE_LOGIN_READY		= 20010005,
    	LOGIC_SYNC_LOGIN_SESSION	= 20010008,
    	LOGIC_CHAT_LOGIN_READY		= 20010010,
    	LOGIC_GATE_LOGIN_READY		= 20010015,
    	LOGIC_SCENE_LOGOUT_READY	= 20010020,				//	逻辑服通知场景服客户端连网关断线
    	LOGIC_CHAT_LOGOUT_READY		= 20010025,				//	逻辑服通知聊天服客户端连网关断线
    	LOGIC_GATE_LOGOUT_READY		= 20010030,				//	逻辑服通知网关客户端连网关断线
    	GATE_LOGIC_CLIENT_LOGIN		= 20010035,				//	网关通知逻辑服客户端登录返回
    	GATE_LOGIC_CLIENT_DROP		= 20010040,				//	网关通知逻辑服客户端连网关断线
    	LOGIC_SCENE_SCENE_APPEAR	= 20010045,				//	逻辑服通知场景服进入场景
		INNER_GATE_ASK_LOGIC_TRAN 	= 20010050,				//	网关通知逻辑转场景
		INNER_GATE_SCENE_COMPLETE 	= 20010051,				//	网关通知场景加载完成
		INNER_INNER_ASK_GATE_TRAN 	= 20010052,				//	内部通知网关转场景
		SCENE_GATE_BATTLE_STATUS 	= 20010055,				//	场景通知网关战斗状态
		GATE_LOGIC_TRUSTEESHIP	 	= 20010060,				//	网关通知逻辑服托管状态
		GATE_SCENE_LGOUT_IN_BATTLE	= 20010061,				//  战斗下线，通知场景服同步fighter到逻辑服
		CHAT_GATE_BROADCAST_BUFF	= 20010065,				//	通知网关全服广播BUFF接口


    	TIMER_GLOBAL_MONITOR_SELF		= 60000001,			//  全局定时器
    	TIMER_TRIGGER_FIRST				= 60000002,       //  周期第一个刷新时间
    	TIMER_TRIGGER_DAILY_ZERO		= 60000003,			//  每日0点刷新时间
    	TIMER_TRIGGER_DAILY_SIX		   = 60000004,			//  每日6点刷新时间
    	TIMER_TRIGGER_DAILY_TWENTY	   = 60000005,			//  每日20点刷新时间
    	TIMER_TRIGGER_WEEKLY_ZERO		= 60000006,			//  每周0点刷新时间


    	TIMER_LOGIC_LOGIN_GATE_TIMEOUT 	= 60100011,
    	TIMER_LOGIC_SAVE_PLAYER_DETAIL 	= 60100012,
    	TIMER_REBOOT_SERVER				= 60100015,
    };

};

#endif /* SERVER_DEFINE_H_ */
