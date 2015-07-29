/*
 * Robot_Server_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef ROBOT_SERVER_REQUEST_H_
#define ROBOT_SERVER_REQUEST_H_

#include "Game_Typedef.h"

class Robot_Monitor;
class Robot_Player;

class Robot_Server_Request {
public:
	Robot_Server_Request();
	virtual ~Robot_Server_Request();

	Robot_Monitor *monitor(void);

	int find_player_by_buf(Block_Buffer &buf, Robot_Player *&player);

	//	错误提示
	int process_80000001(int cid, Block_Buffer &buf);

	//	获取角色(返回)
	int process_50100000(int cid, Block_Buffer &buf);

	//	创建角色(返回)
	int process_50100005(int cid, Block_Buffer &buf);

	//	登录或创建角色成功时返回
	int process_81000000(int cid, Block_Buffer &buf);

	//	登录网关(返回)
	int process_50400001(int cid, Block_Buffer &buf);

	//	场景服登录通知-基础信息
	int process_81000100(int cid, Block_Buffer &buf);

	//玩家出现
	int process_81000001(int cid, Block_Buffer &buf);

	// NPC出现
	int process_81000005(int cid, Block_Buffer &buf);

	//防沉迷登陆提示
	int process_82200101(int cid, Block_Buffer &buf);

	//转场景通知
	int process_81000030(int cid, Block_Buffer &buf);

	//移动(返回)
	int process_50200102(int cid, Block_Buffer &buf);
	//获取角色信息(返回)
	int process_50100006(int cid, Block_Buffer &buf);

	//重置坐标
	int process_81000015(int cid, Block_Buffer &buf);

	//通知关闭连接
	int process_80000005(int cid, Block_Buffer &buf);
////////////////////////////////////////////////////////
	//Mover(玩家/NPC/宠物)移动
	int process_81000020(int cid, Block_Buffer &buf);
	//更新地图传送点数据
	int process_81000040(int cid, Block_Buffer &buf);
	//对象消失(玩家/NPC)
	int process_81000010(int cid, Block_Buffer &buf);
	//寻路传送点数据通知
	int process_81000041(int cid, Block_Buffer &buf);
	int process_80100500(int cid, Block_Buffer &buf);
	int process_81000101(int cid, Block_Buffer &buf);
	int process_80100400(int cid, Block_Buffer &buf);
	int process_81000099(int cid, Block_Buffer &buf);

	int process_50110000(int cid, Block_Buffer &buf);
	int process_81110000(int cid, Block_Buffer &buf);
	int process_50100200(int cid, Block_Buffer &buf);
	int process_81000201(int cid, Block_Buffer &buf);
	int process_81000050(int cid, Block_Buffer &buf);
	int process_81000055(int cid, Block_Buffer &buf);
	int process_81000060(int cid, Block_Buffer &buf);

	int process_50400005(int cid, Block_Buffer &buf);
	//int process_81000030(int cid, Block_Buffer &buf);


private:
	Robot_Monitor *monitor_;
};

typedef Singleton<Robot_Server_Request> Robot_Server_Request_Single;
#define ROBOT_SERVER_REQUEST    (Robot_Server_Request_Single::instance())

inline Robot_Monitor *Robot_Server_Request::monitor(void) {
	return monitor_;
}

#endif /* ROBOT_SERVER_REQUEST_H_ */
