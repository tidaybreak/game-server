/*
 * Robot_Server_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Robot_Server_Request.h"
#include "Robot_Monitor.h"
#include "Pool_Manager.h"
#include "Robot_Player.h"
#include "Global_Timer.h"
#include "Config_Cache.h"
#include "Msg_Role_Scene_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Fight_Struct.h"
#include "Msg_Task_Struct.h"
#include "Msg_Task_Enum.h"
#include "Msg_Role_Scene_Enum.h"
#include "Robot_Struct.h"
#include "Config_Cache_Scene.h"
#include "scene/Scene_Def.h"
#include "Config_Cache_Robot.h"
#include <openssl/md5.h>


Robot_Server_Request::Robot_Server_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = ROBOT_MONITOR;
}

Robot_Server_Request::~Robot_Server_Request() {
	// TODO Auto-generated destructor stub
}

int Robot_Server_Request::find_player_by_buf(Block_Buffer &buf, Robot_Player *&player) {
	role_id_t role_id = 0;
	if (buf.read_int64(role_id)) {
		MSG_USER_TRACE("read role id:%ld", role_id);
		return CALL_RETURN;
	}

	player = monitor()->find_player_by_role_id(role_id);
	if (! player) {
		MSG_USER_TRACE("can not find logic player:%ld", role_id);
		return CALL_RETURN;
	}

	return 0;
}

int Robot_Server_Request::process_80000001(int cid, Block_Buffer &buf) {
	MSG_80000001 msg;
	msg.deserialize(buf);
	if(msg.error_source == 10100000) {
		MSG_USER("<<<<<<<<error message id:%d, error source id:%d, error_id:%d", msg.msg_id, msg.error_source, msg.error_id);
	}
	if(msg.error_source == 10100001) {
		Account_Key account;
		if (monitor()->get_account_by_cid(cid, account)) {
			LOG_ABORT("can not find account, cid:%d", cid);
		}
		ROBOT_MONITOR->unset_loging(account.account_name);
		ROBOT_MONITOR->set_loginoff(account.account_name);
	}
	return 0;
}

//获取角色(返回)
int Robot_Server_Request::process_50100000(int cid, Block_Buffer &buf) {
	MSG_USER("<<<<<<<<获取角色(返回)");
	MSG_50100000 msg;
	msg.deserialize(buf);

	Account_Key account;
	if (monitor()->get_account_by_cid(cid, account)) {
		MSG_USER("can not find account, cid:%d", cid);
		return -1;
	}
	ROBOT_MONITOR->unset_fetch_role(account);
	Block_Buffer res_buf;
	if (msg.role_list.empty()) {
		//创建角色
		MSG_USER("创建角色 %s >>>>>>", account.account_name.c_str());
		MSG_10100005 res_msg;
		res_msg.account = account.account_name;
		res_msg.agent_num = account.agent_num;
		res_msg.server_num = account.server_num;
		res_msg.career = (rand()%3) + 1;
		int8_t gender = 1;
		if(res_msg.career == 3) {
			gender = 2;
		}
		res_msg.gender = gender;
		std::string name_pix = "机器人_";
		res_msg.role_name = name_pix + account.account_name;
		//res_msg.session_id = "xxxx";
		res_msg.timestamp = std::to_string((long long)Time_Value::gettimeofday().sec());

		// md5验证
		{
			const char* key = CONFIG_CACHE->server_maintainer_cache().md5_key.c_str();
			char mine_src[256 + 1], mine_md5[256 + 1];
			memset(mine_src, 0x00, 256 + 1);
			memset(mine_md5, 0x00, 256 + 1);
			snprintf(mine_src, sizeof(mine_src), "%s%s%s", account.account_name.c_str(), res_msg.timestamp.c_str(), key);
			const unsigned char *tmp_md5 = MD5((const unsigned char *) mine_src, strlen(mine_src), 0);
			for (uint i = 0; i < 16; i++) {
				sprintf(&mine_md5[i * 2], "%.2x", tmp_md5[i]);
			}
			res_msg.session_id = mine_md5;
		}

		res_buf.make_client_message(10100005, 0, 0);
		res_msg.serialize(res_buf);
	} else {
		//登录游戏
		MSG_USER("%s 登录游戏>>>>>>",account.account_name.c_str());
		MSG_10100001 res_msg;
		res_msg.account = account.account_name;
		res_msg.agent_num = account.agent_num;
		res_msg.server_num = account.server_num;
		res_msg.session_id = "xxxx";
		res_msg.timestamp = "11111";
		res_buf.make_client_message(10100001, 0, 0);
		res_msg.serialize(res_buf);
		ROBOT_MONITOR->set_loging(account.account_name);
	}

	res_buf.finish_message();
	monitor()->create_send_to_logic(cid, account.account_name, res_buf);

	return 0;
}

//创建角色(返回)
int Robot_Server_Request::process_50100005(int cid, Block_Buffer &buf) {
//	MSG_USER("<<<<<<<<创建角色(返回)");

	MSG_50100005 msg;
	msg.deserialize(buf);

	Account_Key account;
	if (monitor()->get_account_by_cid(cid, account)) {
		MSG_USER("can not find account, cid:%d", cid);
		return -1;
	}

	//登录游戏
	MSG_USER("%s 登录游戏>>>>>>",account.account_name.c_str());
	MSG_10100001 res_msg;
	res_msg.account = account.account_name;
	res_msg.agent_num = account.agent_num;
	res_msg.server_num = account.server_num;
	res_msg.session_id = "xxxx";
	res_msg.timestamp = "11111";
	Block_Buffer res_buf;
	res_buf.make_client_message(10100001, 0, 0);
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	monitor()->create_send_to_logic(cid, account.account_name, res_buf);
	ROBOT_MONITOR->set_loging(account.account_name);

	return 0;
}

//登录或创建角色成功时返回
int Robot_Server_Request::process_81000000(int cid, Block_Buffer &buf) {

	MSG_81000000 msg;
	msg.deserialize(buf);

	Account_Key account;
	if (monitor()->get_account_by_cid(cid, account)) {
		MSG_USER("can not find account, cid:%d", cid);
		return -1;
	}
	MSG_USER("%s <<<<<<<登录或创建角色成功时返回", account.account_name.c_str());
	//登录网关
	MSG_USER("登录网关 %ld >>>>>>>>", msg.role_id);
	MSG_10400001 res_msg;
	res_msg.role_id = msg.role_id;
	res_msg.session_id = "xxxx";
	Block_Buffer res_buf;
	res_buf.make_client_message(10400001, 0, 0);
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	int gate_cid = monitor()->connect_to_gate(msg.gate_link.ip, msg.gate_link.port);
	if (gate_cid >= 2) {
		Robot_Player *player = POOL_MANAGER->pop_robot_player();
		player->reset();
		player->set_cid(gate_cid);
		player->base_detail().role_id = msg.role_id;
		player->base_detail().account = account.account_name;


		ROBOT_MONITOR->set_loging_gate(account.account_name);//...
		ROBOT_MONITOR->unset_loging(account.account_name);//...

		monitor()->bind_cid_player(*player);
		monitor()->bind_rid_player(*player);
		player->send_to_server(res_buf);
		player->wayfind_info().last_move_time = Time_Value::gettimeofday();
		player->wayfind_info().move_interval_time = CONFIG_CACHE_ROBOT->robot_config().command_interval;
		//获取角色信息
		MSG_10100006 role_info_msg;
		role_info_msg.role_id = msg.role_id;
		Block_Buffer res_buf;
		res_buf.make_client_message(10100006, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		role_info_msg.serialize(res_buf);
		res_buf.finish_message();
		player->send_to_server(res_buf);
		//获取角色信息 send end

		// close link with logic server
		//monitor()->close_link_by_cid(monitor()->get_cid_by_account(account), Link_Close(0, 0));
		monitor()->drop_link_to_logic(account);
		//MSG_DEBUG("   process_81000000，创号成功连网关成功，断开逻辑服连接");

		//player->player_init();

	} else {
		MSG_USER("connect to gate error");
	}

	return 0;
}

//登录网关(返回)
int Robot_Server_Request::process_50400001(int cid, Block_Buffer &buf) {
	MSG_50400001 msg;
	msg.deserialize(buf);

	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;

	if (!msg.location.scene_id) {
		LOG_ABORT("process_50400001 scene id:%d", msg.location.scene_id);
	}

	player->wayfind_info().cur_location = msg.location;
	//start 坑
	player->wayfind_info().cur_location.coord.type = 1;
	//end 坑
	//player->wayfind_info().scene = ROBOT_SCENE_MANAGER->find_scene(msg.location.scene_id);
	MSG_USER("%s <<<<<<<<<登录网关(返回) 场景:%d", player->base_detail().account.c_str(), msg.location.scene_id);
	ROBOT_MONITOR->unset_loging_gate(player->base_detail().account);

	//场景加载完成
	MSG_USER("%s 场景加载完成 (登录网关)>>>>>>>", player->base_detail().account.c_str());
	MSG_10200030 res_msg;
	Block_Buffer res_buf;
	res_buf.make_client_message(REQ_SCENE_COMPLETE, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	player->send_to_server(res_buf);
//	////////////////登陆时候转到主城///////////////////////////
//	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player->wayfind_info().cur_location.scene_id);
//	if(scene_config && scene_config->type != Main_Scene_Type) {
//		MSG_USER("%s 转到主城场景 (登录网关)>>>>>>>", player->base_detail().account.c_str());
//		MSG_10400005 res_msg_;
//		Block_Buffer res_buf_;
//		res_buf_.make_client_message(10400005, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
//		res_msg_.serialize(res_buf_);
//		res_buf_.finish_message();
//		player->send_to_server(res_buf_);
//	} else {
//		//player->base_detail().status = Robot_Detail::IDLE;
//	}
	return 0;
}

//场景服登录通知-基础信息
int Robot_Server_Request::process_81000100(int cid, Block_Buffer &buf) {
	MSG_USER("<<<<<<<<<<场景服登录通知-基础信息");
	MSG_81000100 msg;
	msg.deserialize(buf);

	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;

	player->base_detail().role_name = msg.role_name;

	//客户端初始化完成
	MSG_USER("%s 客户端初始化完成>>>>>>>>>", player->base_detail().account.c_str());
	MSG_10100015 res_msg;
	Block_Buffer res_buf;
	res_buf.make_client_message(10100015, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	player->send_to_server(res_buf);

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
int Robot_Server_Request::process_81000001(int cid, Block_Buffer &buf) {
	MSG_81000001 msg;
	msg.deserialize(buf);

	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;
/*
	player->base_detail().location.coord = msg.role_info.coord;
	player->base_detail().career = msg.role_info.career;
	player->base_detail().level = msg.role_info.level;
	player->base_detail().role_id = msg.role_info.role_id;
*/

	//MSG_USER("<<<<<<<<<<<人物出现信息:%d, role_name:%s", msg.msg_id, msg.role_info.role_name.c_str());

	return 0;
}

int Robot_Server_Request::process_81000005(int cid, Block_Buffer &buf) {
	MSG_81000005 msg;
	msg.deserialize(buf);
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) {
		return 0;
	}
	player->npc_appear(msg.npc_info);
	return 0;
}

//对象消失(玩家/NPC)
int Robot_Server_Request::process_81000010(int cid, Block_Buffer &buf) {
	MSG_81000010 msg;
	msg.deserialize(buf);
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) {
		return 0;
	}
	player->npc_disappear(msg.ids);
	return 0;
}

int Robot_Server_Request::process_82200101(int cid, Block_Buffer &buf) {
	MSG_82200101 msg;
	msg.deserialize(buf);

	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;

//	MSG_USER("<<<<<<<<< message:%d, role_name:%s, 防沉迷状态:%d", msg.msg_id, player->base_detail().role_name.c_str(), msg.fcm);

	return 0;
}

//移动(返回)
int Robot_Server_Request::process_50200102(int cid, Block_Buffer &buf) {
	MSG_50200102 msg;
	msg.deserialize(buf);
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;
	player->wayfind_info().cur_location.coord = msg.coord;
	MSG_USER("<<<<<<<<<< 移动(返回)");
	return 0;
}
//获取角色信息(返回)
int Robot_Server_Request::process_50100006(int cid, Block_Buffer &buf) {
	MSG_50100006 msg;
	msg.deserialize(buf);
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;
	player->base_detail().role_name = msg.role_info.role_name;
	player->base_detail().career = msg.role_info.career;
	player->base_detail().gender = msg.role_info.gender;
	player->base_detail().level = msg.role_info.level;
	MSG_USER("<<<<<<<<<< 获取角色信息(返回)");

	//
	player->player_init();

	return 0;
}

//重置坐标
int Robot_Server_Request::process_81000015(int cid, Block_Buffer &buf) {
	MSG_81000015 msg;
	msg.deserialize(buf);
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;
	if (!player->wayfind_info().cur_location.scene_id) {
		LOG_ABORT("process_81000015 scene id:%d, 机器人状态:%d", player->wayfind_info().cur_location.scene_id, player->base_detail().status);
	}

	//MSG_USER("%s --- 重置坐标 --- scene_id: %d, client: x=%d,y=%d. server: x=%d,y=%d", player->base_detail().account.c_str(),
	//		player->wayfind_info().cur_location.scene_id, player->wayfind_info().cur_location.coord.x, player->wayfind_info().cur_location.coord.y,
	//		msg.coord.x, msg.coord.y);

	player->wayfind_info().cur_location.coord = msg.coord;
	player->wayfind_info().first_move = true;

	//player->path_find();
	player->base_detail().status = Robot_Detail::IDLE;
	//player->path_find_move();

	//player->base_detail().status = Robot_Detail::REWAYFIND;

	return 0;
}

//转场景通知
int Robot_Server_Request::process_81000030(int cid, Block_Buffer &buf) {
	MSG_81000030 msg;
	msg.deserialize(buf);

	if (!msg.location.scene_id) {
		LOG_ABORT("process_81000030 scene id:%d", msg.location.scene_id);
	}

	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;
	player->wayfind_info().cur_location = msg.location;
	//start 坑
	player->wayfind_info().cur_location.coord.type = 1;
	//end 坑

	//MSG_USER("%s <<<<<<<<<< message:%d, 转场景通知", player->base_detail().account.c_str(), msg.msg_id);

	//场景加载完成
	MSG_USER("%s 转场景成功通知 message:%d , cur_scene:%d", player->base_detail().account.c_str(), msg.msg_id,
			player->wayfind_info().cur_location.scene_id);

	MSG_10200030 res_msg;
	Block_Buffer res_buf;
	res_buf.make_client_message(REQ_SCENE_COMPLETE, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	player->send_to_server(res_buf);

//	player->wayfind_info().det_coord.reset();
//	player->wayfind_info().det_scene_id = 0;
//	//player->base_detail().last_move_time = Time_Value::gettimeofday();
//	player->base_detail().status = Robot_Detail::IDLE;

	player->wayfind_info().det_scene_id = player->wayfind_info().cur_location.scene_id;
	//player->path_find();
	//player->base_detail().status = Robot_Detail::MOVE;
	//player->path_find_move();

	return 0;
}

//通知关闭连接
int Robot_Server_Request::process_80000005(int cid, Block_Buffer &buf) {
	MSG_80000005 msg;
	msg.deserialize(buf);
	LOG_USER("<<<<<<< 通知关闭连接 : %d", msg.reason);
	return 0;
}








//Mover(玩家/NPC/宠物)移动
int Robot_Server_Request::process_81000020(int cid, Block_Buffer &buf) {
	return 0;
}
//更新地图传送点数据
int Robot_Server_Request::process_81000040(int cid, Block_Buffer &buf) {
	return 0;

	MSG_81000040 msg;
	msg.deserialize(buf);
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) return 0;
	MSG_USER("%s <<<< 更新地图scene_id:%d的传送点数据: %d个", player->base_detail().account.c_str(), player->wayfind_info().cur_location.scene_id, msg.transmit_point.size());

	if (!msg.transmit_point.empty() && !player->in_main_scene()) {
		//player->gm_finish_task(player->task_info().id);
		//player->exit_diff_space();
		return 0;
	}

	for(std::vector<Transmit_Point>::iterator iter = msg.transmit_point.begin(); iter != msg.transmit_point.end(); ++iter) {
		if(iter->des_scene_id == 80100004) {
			continue; //测试主城
		}
		if(iter->des_scene_id != player->wayfind_info().cur_location.scene_id && iter->des_scene_id) {
			const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(iter->des_scene_id);
			if(scene_config && scene_config->type == Scene_Type::Main_Scene_Type) {
				if (!iter->scene_id) {
					LOG_ABORT("process_81000040 scene id:%d", iter->scene_id);
				}
				player->wayfind_info().det_coord = iter->coord;
				player->wayfind_info().det_scene_id = iter->des_scene_id;
				break;
			}
		} else {
//			ROBOT_MONITOR->rand_move(player);
//			player->wayfind_info().first_move = true;
//			player->base_detail().status = Robot_Detail::IDLE;
			if(iter->scene_id == 80100001) {
//				player->wayfind_info().det_coord = iter->coord;
//				Coord coord;
//				switch(rand()%8) {
//					case 0:{
//						coord.x += 32*5;
//						break;
//					}
//					case 1:{
//						coord.x -= 32*5;
//						break;
//					}
//					case 2:{
//						coord.y += 32*(rand()%5);
//						break;
//					}
//					case 3:{
//						coord.y -= 32*(rand()%5);
//						break;
//					}
//					case 4:{
//						coord.x += 32*(rand()%5);
//						coord.y += 32*(rand()%5);
//						break;
//					}
//					case 5:{
//						coord.x -= 32*(rand()%5);
//						coord.y += 32*(rand()%5);
//						break;
//					}
//					case 6:{
//						coord.x -= 32*(rand()%5);
//						coord.y -= 32*(rand()%5);
//						break;
//					}
//					case 7:{
//						coord.x += 32*(rand()%5);
//						coord.y -= 32*(rand()%5);
//						break;
//					}
//				}
				player->wayfind_info().det_coord = iter->coord;
				//player->wayfind_info().det_scene_id = 80100001;
			}
			if(iter->scene_id == 80100002) {
				player->wayfind_info().det_coord = iter->coord;
				//player->wayfind_info().det_scene_id = 80100003;
			}
			if(iter->scene_id == 80100003) {
				player->wayfind_info().det_coord = iter->coord;
				//player->wayfind_info().det_scene_id = 80100001;
			}

		}
	}
	player->wayfind_info().first_move = true;
	//player->base_detail().status = Robot_Detail::REWAYFIND;
	//player->path_find();
	player->base_detail().status = Robot_Detail::IDLE;
	//player->path_find_move();

	return 0;
}

//寻路传送点数据通知
int Robot_Server_Request::process_81000041(int cid, Block_Buffer &buf) {
	//MSG_USER("<<<< 寻路传送点数据通知");
	return 0;
}
//主动推送不能引导的列表
int Robot_Server_Request::process_80100500(int cid, Block_Buffer &buf) {
	return 0;
}
//更新人物属性信息
int Robot_Server_Request::process_81000101(int cid, Block_Buffer &buf) {
	return 0;
}
//发送功能图标开启信息列表
int Robot_Server_Request::process_80100400(int cid, Block_Buffer &buf) {
	return 0;
}
//逻辑服登录通知-基础信息
int Robot_Server_Request::process_81000099(int cid, Block_Buffer &buf) {
	return 0;
}

//获取任务列表(返回)
int Robot_Server_Request::process_50110000(int cid, Block_Buffer &buf) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if(player) {
		MSG_50110000 msg;
		msg.deserialize(buf);
		player->res_fetch_task_list(msg.tasks);
	}
	return 0;
}

//获取任务列表(返回)
int Robot_Server_Request::process_81110000(int cid, Block_Buffer &buf) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if(player) {
		MSG_81110000 msg;
		msg.deserialize(buf);
		player->active_task_list_update(msg.operate, msg.task);
	}
	return 0;
}

int Robot_Server_Request::process_50100200(int cid, Block_Buffer &buf) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if(player) {
//		MSG_81110000 msg;
//		msg.deserialize(buf);
//		player->active_task_list_update(msg.operate, msg.task);
	}
	return 0;
}

int Robot_Server_Request::process_81000201(int cid, Block_Buffer &buf) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (player) {
		MSG_USER("81000201 enter plot");

		MSG_81000201 active_msg;
		active_msg.deserialize(buf);

		//------- enter plot ----------
		MSG_10110008 msg_08;
		msg_08.plot_id = active_msg.plot_id;
		msg_08.action = 0;
		msg_08.num = 0;
		msg_08.type = 1;
		Block_Buffer req_buf;
		req_buf.make_client_message(REQ_TASK_PLOT, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		msg_08.serialize(req_buf);
		req_buf.finish_message();
		player->send_to_server(req_buf);

		MSG_10100010 msg_10;
		msg_10.plot_id = active_msg.plot_id;
		req_buf.reset();
		req_buf.make_client_message(REQ_SET_PLOT_INFO, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		msg_10.serialize(req_buf);
		req_buf.finish_message();
		player->send_to_server(req_buf);

		//---------- exit plot ----------
		MSG_USER("81000201 exit plot");

		msg_08.reset();
		msg_08.plot_id = active_msg.plot_id;
		msg_08.action = 1;
		msg_08.num = 0;
		msg_08.type = 0;
		req_buf.reset();
		req_buf.make_client_message(REQ_TASK_PLOT, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		msg_08.serialize(req_buf);
		req_buf.finish_message();
		player->send_to_server(req_buf);

		msg_10.reset();
		msg_10.plot_id = "";
		req_buf.reset();
		req_buf.make_client_message(REQ_SET_PLOT_INFO, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		msg_10.serialize(req_buf);
		req_buf.finish_message();
		player->send_to_server(req_buf);
	}
	return 0;
}

int Robot_Server_Request::process_81000050(int cid, Block_Buffer &buf) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (player) {
//		MSG_81110000 msg;
//		msg.deserialize(buf);
		player->base_detail().sub_status = Robot_Detail::FIGHT;
		player->base_detail().status = Robot_Detail::FIGHT;
	}
	return 0;
}

int Robot_Server_Request::process_81000055(int cid, Block_Buffer &buf) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (player) {
//		MSG_81110000 msg;
//		msg.deserialize(buf);
//		player->active_task_list_update(msg.operate, msg.task);
		//player->base_detail().sub_status = 0;
		player->handle_clear_monster_point();
	}
	return 0;
}

int Robot_Server_Request::process_81000060(int cid, Block_Buffer &buf) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) {
		return 0;
	}
//	MSG_81000060 msg;
//	msg.deserialize(buf);
//	for (uint16_t i = 0; i < msg.change_info.size(); ++i) {
//		if (msg.change_info[i].prop_type == 502403) {	// level up
//			player->base_detail().level = msg.change_info[i].current_val;
//		}
//	}

	return 0;
}

int Robot_Server_Request::process_50400005(int cid, Block_Buffer &buf) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) {
		return 0;
	}
	MSG_50400005 msg;
	msg.deserialize(buf);

	if (! msg.local.scene_id) {
		LOG_ABORT("process_50400005 scene id:%d", msg.local.scene_id);
	}

	MSG_USER("%s message:%d, 转场景失败, old_scene:id=%d, x=%d,y=%d.err_num=%d, task_id=%d,task_status=%d,des_scene_id=%d", player->base_detail().account.c_str(), msg.msg_id,
			player->wayfind_info().cur_location.scene_id,
			player->wayfind_info().cur_location.coord.x, player->wayfind_info().cur_location.coord.y,
			msg.result,	player->task_info().id, player->task_info().status, player->wayfind_info().det_scene_id);

	//MSG_USER("%s msg_scene:id=%d, x=%d,y=%d, type=%d", player->base_detail().account.c_str(), msg.local.scene_id,
	//		 msg.local.coord.x,  msg.local.coord.y, msg.local.coord.type);

	player->wayfind_info().cur_location = msg.local;

	MSG_USER("%s message:%d, cur_scene:id=%d, x=%d,y=%d", player->base_detail().account.c_str(), msg.msg_id,
			player->wayfind_info().cur_location.scene_id,
			player->wayfind_info().cur_location.coord.x, player->wayfind_info().cur_location.coord.y);
	//player->wayfind_info().cur_location.coord.type = 1;

	//player->path_find();
	player->base_detail().status = Robot_Detail::IDLE;
	//player->path_find_move();

	return 0;
}

//int Robot_Server_Request::process_81000030(int cid, Block_Buffer &buf) {
//	Robot_Player *player = monitor()->find_player_by_cid(cid);
//	if (! player) {
//		return 0;
//	}
//	MSG_81000030 msg;
//	msg.deserialize(buf);
//	player->wayfind_info().cur_location = msg.location;
//	player->base_detail().status = Robot_Detail::MOVE;
//
//	return 0;
//}
