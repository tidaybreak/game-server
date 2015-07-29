/*
 * Expedition_Scene_Def.h
 *
 *  Created on: Oct 14, 2014
 *      Author: xiaoliang
 */

#ifndef EXPEDITION_SCENE_DEF_H_
#define EXPEDITION_SCENE_DEF_H_


enum Expedition_channel_type {
	EXPEDITION_CHANNEL_SERVER_OPEN_TIME				= 1,//1.开服时间
	EXPEDITION_CHANNEL_SERVER_GANG_INFO				= 2,//2.获取单数所有公会信息
	EXPEDITION_CHANNEL_SCENE_NOTICE_SERVER_GANG_NO	= 3,//3.场景通知单服公会所在线号
	EXPEDITION_CHANNEL_SERVER_GANG_INFO_ADD			= 4,//2.获取单数所有公会信息
	EXPEDITION_CHANNEL_SERVER_GANG_INFO_DEL			= 5,//2.获取单数所有公会信息
	EXPEDITION_CHANNEL_DAILY_AWARD					= 6, //远征：日常奖励
	EXPEDITION_CHANNEL_CREATE_DAILY_AWARD			= 7, // 远征：日常奖励产生
	EXPEDITION_CHANNEL_DEFEND_NOTICE				= 8, // 防守通知
	EXPEDITION_CHANNEL_DEMAGE_RANK_AWARD			= 9, // 伤害排行奖励
	EXPEDITION_CHANNEL_CREATE_DEMAGE_RANK_AWARD		= 10, // 远征：伤害奖励产生
	EXPEDITION_CHANGE_LEADER						= 11, // 公会换会长
	EXPEDITION_ASK_WORLD_LEVEL						= 12, // 远征请求世界等级
	EXPE_MONSTER_TIMING_REFRESH						= 13, //
	EXPE_MONSTER_TIMING_KILLED						= 14, //
	EXPE_BOX_OPEND									= 15,
};

enum Expedition_common_conf_type {
	GROUP_SERVER_NUM_MAX			= 10000001,// 服务器分组区间的服务器个数
	LIMIT_LEVEL						= 10000002,// 进入远征场景等级限制
	GROUP_DESIDE_GAP				= 10000003, // 服务器开服x天分区
	GANG_BASE_DEFEND_MONSTER_GROUP	= 10000004, // 公会守卫npc怪组
	FIGHT_GAP						= 10000005, // 远征每场战斗间隔
	OPEN_SERVER_GAP					= 10000006, // 服务器启动后，x分钟，才开始
	OPEN_GAP_FOR_FIRST_MONTH		= 10000007, // 第一季，开服当天0点到x秒后执行
	MANAGER_GANG_INIT_GAP			= 10000008, // 等待x秒，开始按公会排名分配所在线
	BATTLE_SCENE					= 10000009, // 战斗场景
	REVIVE_TIME						= 10000010, // 复活时间（秒）
	SWITCH							= 10000011, // switch
	MONSTER_NUMS					= 10000012, // 每批怪数量
	START_ANN_NUM					= 10000013, // 远征：开启广播次数
};

enum Expedition_Build_Type {
	EBT_UNDEFINE					= 0,
	EBT_GANG_BASE 					= 1,//1.基地
	EBT_CHECKPOINT 					= 2,//2.关卡
	EBT_CASTLE_BIG 					= 3,//3.大城堡
	EBT_CASTLE_MEDIUM 				= 4,//4.中城堡
	EBT_CASTLE_SMALL 				= 5,//5.小城堡
	EBT_CHEST						= 6,//6.宝箱区
	EBT_BEAST						= 7,//7.野兽区
	EBT_MONSTER_REFRESH_TIMING		= 8,//8.野兽区定时刷新
	EBT_MONSTER_REFRESH_KILL_GAP		= 9,//9.野兽区打死间隔刷新
};

enum Expedition_Build_Fight_Status {
	EBFS_PEACE				= 0,
	EBFS_FIGHT				= 1,
};

enum Expedition_Build_Belong_Status {
	EBBS_UNINIT				= 0,
	EBBS_NEUTRAL			= 1,
	EBBS_OCCUPY				= 2,
};
enum Expedition_Build_Status {
	EBS_UNINIT				= 0,//0.未被初始化状态,
	EBS_NEUTRAL_PEACE		= 1,//1.中立(和平状态),
	EBS_NEUTRAL_FIGHTING	= 2,//2.中立(攻占状态),
	EBS_OCCUPY_PEACE		= 11,//11.占领(和平状态),
	EBS_OCCUPY_FIGHTING		= 12,//12.占领(攻占状态)
};

//类型; 0.玩家,1.中立驻守怪,2.公会骑士团,3.精锐远征军,4.远征军,5.公会守卫npc
enum Expedition_Soldier_Type {
	EST_PLAYER				= 0, //类型; 0.玩家
	EST_MONSTER				= 1, //类型; 1.中立驻守怪
	EST_M_KNIGHTAGE			= 2, //类型; 2.公会骑士团
	EST_M_EXPE_ARMY_STRONG	= 3, //类型; 3.精锐远征军
	EST_M_EXPE_ARMY			= 4, //类型; 4.远征军
	EST_M_NPC				= 5, //类型; 5.公会守卫npc
};

enum Const_Value {
	LOOKING_PLAYER_MAX = 3,
	FIGHT_PLAYER_MAX = 6,
	GROUP_MONSTER = 2,
};

enum Expedition_Refresh_Sight {
	ERS_SHOW	= 0,
	ERS_HIDE	= 1,
};

#endif /* EXPEDITION_SCENE_DEF_H_ */
