/*
 * Logic_Client_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Logic_Client_Request.h"
#include "Logic_Monitor.h"
#include "Config_Cache.h"
#include "Game_Manager.h"
#include "DB_Manager.h"
#include "Pool_Manager.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Utility_Func.h"
#include "DB_Def.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "Err_Code.h"
#include "Head_Struct.h"
#include "friend/Friend_Def.h"
#include "friend/Friend.h"
#include "mall/Mall.h"
#include "ranking/Ranking_Manager.h"
#include "gang/Gang_Manager.h"
#include "time.h"
#include "team/Team_Def.h"

Logic_Client_Request::Logic_Client_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = LOGIC_MONITOR;
}

Logic_Client_Request::~Logic_Client_Request() {
	// TODO Auto-generated destructor stub
}

Logic_Player *Logic_Client_Request::process_success_login(int cid, Logic_Player_Detail &detail) {
	Logic_Player *player = POOL_MANAGER->pop_logic_player();
	if (! player) {
		LOG_TRACE_ABORT("logic_player_pool_.pop() return 0.");
		return 0;
	}

	if (player->login_logic(detail)) {
		POOL_MANAGER->push_logic_player(player);
		return 0;
	} else {
		monitor()->bind_logic_player(*player);
	}

	return player;
}

int Logic_Client_Request::process_fetch_role(int cid, MSG_10100000 &msg) {
	if (LOGIC_MONITOR->server_tick_check()) {
		return monitor()->close_client_by_cid(cid, Time_Value(3), ERROR_LOGIN_MAX_LIMIT, ERROR_LOGIN_MAX_LIMIT);
	}

	// todo 合服要返回当前账号所有角色,这里到时要改改
	DB_View::Account_Player_View_Map::iterator find_it = monitor()->db_view()->account_player_view_map.find(Account_Key(msg.account, msg.agent_num, msg.server_num));
	MSG_50100000 res_msg;
	Block_Buffer res_buf;
	res_buf.make_message(res_msg.msg_id);
	if (find_it != monitor()->db_view()->account_player_view_map.end()) {
		Uid_Name uid_name;
		uid_name.uid = find_it->second->role_id;
		uid_name.name = find_it->second->role_name;
		res_msg.role_list.push_back(uid_name);
	}
	if (res_msg.role_list.empty()) {
		Role_Key key = monitor()->db_view()->get_min_career();
		res_msg.career = key.career;
		res_msg.gender = key.gender;
	}

	{
		  time_t t = time(NULL);
		  struct tm lt = {0};
		  localtime_r(&t, &lt);
		  res_msg.timezone = lt.tm_gmtoff / Time_Value::ONE_HOUR_IN_SECS;
		  res_msg.center_url = CONFIG_CACHE->server_maintainer_cache().center_url;
	}

	res_msg.serialize(res_buf);
	res_buf.finish_message();
	return monitor()->send_to_client(cid, res_buf);
}


int Logic_Client_Request::process_random_name(int cid, MSG_10100007 &msg) {
	MSG_50100007 res_msg;
	Block_Buffer res_buf;
	res_buf.make_message(res_msg.msg_id);

	res_msg.name = LOGIC_PUBLIC->get_name(msg.gender);

	res_msg.serialize(res_buf);
	res_buf.finish_message();
	return monitor()->send_to_client(cid, res_buf);
}

int Logic_Client_Request::process_player_login(int cid, MSG_10100001 &msg) {
	MSG_DEBUG("cid:%d===============================================================", cid);

	Player_DB_View *player_db_view = 0;
	DB_View::Account_Player_View_Map::iterator find_it = monitor()->db_view()->account_player_view_map.find(Account_Key(msg.account, msg.agent_num, msg.server_num));
	if (find_it != monitor()->db_view()->account_player_view_map.end()) {
		player_db_view = find_it->second;
	} else {
		MSG_USER("login account:%s agent:%d server:%d not exist:", msg.account.c_str(), msg.agent_num, msg.server_num);
		return ERROR_ACCOUNT_NOT_EXIST;
	}

	std::string ip;
	monitor()->get_client_peer_addr(cid, ip);

	bool validate = monitor()->get_validate_account(ip);

	player_db_view->ip = ip; //记录玩家登陆IP

	// 超时验证
	Time_Value now = Time_Value::gettimeofday();
	if (validate && abs(atoi(msg.timestamp.c_str()) - now.sec()) > LOGIN_VALIDATE_TIMEOUT) {
		MSG_USER("login validate timeout account:%s  time:%s flag:%s", msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str());
		return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_CLOSE_CLIENT_LOGIN_TIMEOUT, atoi(msg.timestamp.c_str()));
    }

	// md5验证
	if (validate && Utility_Func::validate_md5(CONFIG_CACHE->server_maintainer_cache().md5_key.c_str(), msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str())) {
		MSG_USER("login validate md5 error account:%s  time:%s flag:%s", msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str());
		return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_CLOSE_CLIENT_MD5_CHECK);
	}

	// 是否正在登录/创建流程
	Int_Uid::iterator c_find_it = monitor()->db_view()->login_cid_role_id_map.find(cid);
	if (c_find_it != monitor()->db_view()->login_cid_role_id_map.end()) {
		MSG_USER("in login_map account:%s, cid:%d, real id:%ld, map id:%ld ", msg.account.c_str(), cid, player_db_view->role_id, c_find_it->second);
		return ERROR_ROLE_IN_LOGIN_MAP;
	}

	// 封禁ip判断
	if (GM_MANAGER->check_forbid_ip_set(ip)) {
		MSG_USER("login ip:%s forbid, account:%s  time:%s flag:%s", ip.c_str(), msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str());
		return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_CLOSE_CLIENT_FORBID_IP);
	}

	// 封禁account判断
	if (GM_MANAGER->check_forbid_account_list(player_db_view->account_info.account_name)) {
		MSG_USER("login account:%s forbid,  time:%s flag:%s", msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str());
		return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_CLOSE_CLIENT_FORBID_ACCOUNT);
	}

	// 判断role封禁
	if (GM_MANAGER->check_forbid_role_id_list(player_db_view->role_id)) {
		MSG_USER("login role:%s name forbid account:%s  time:%s flag:%s", player_db_view->role_name.c_str(), msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str());
		return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_CLOSE_CLIENT_FORBID_ROEL_ID);
	}

	// 判断防沉迷
	if(msg.fcm && player_db_view->fcm){
	//if(player_db_view->fcm){
		player_db_view->fcm = msg.fcm;
		if(now - player_db_view->last_sign_out_time >= Time_Value(FCM_RESET_TIME)) {
			//gt 5 h
			player_db_view->add_up_online_time = 0;
		}
		if(player_db_view->add_up_online_time >= FCM_LOCK_TIME) {
			return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_FCM_LOGIN);
		}
	} else if(player_db_view->fcm && msg.fcm == 0) {
		player_db_view->fcm = msg.fcm;
	}

	// 正在登录中
	if (Player_DB_View::ACCOUNT_DOING_LOGIN & player_db_view->account_mode) {
		MSG_USER("player login mode, account:%s  time:%s flag:%s", msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str());
		return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_CLOSE_CLIENT_ROLE_LOGINGING);
	}

	// 正在下线中(让客户端延迟1s再试)
	if (Player_DB_View::ACCOUNT_DOING_LOGOUT & player_db_view->account_mode) {
		MSG_50100001 req_msg;
		Block_Buffer req_buf;
		MAKE_MSG_BUF(req_msg, req_buf);
		monitor()->send_to_client_by_cid(cid, req_buf);
		MSG_USER("player logout mode, account:%s  time:%s flag:%s", msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str());
		return  0;
	}

	Logic_Player *player = monitor()->find_logic_player_by_role_id(player_db_view->role_id);
	if (! player) {
		// 正常登录
		Block_Buffer buf;
		buf.make_message(MONGO_LOAD_PLAYER_ALL_DATA);
		buf.write_int64(player_db_view->role_id);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, cid);

		// 设置登录状态
		player_db_view->set_account_status(Player_DB_View::ACCOUNT_DOING_LOGIN, cid);
		monitor()->db_view()->role_session_map[player_db_view->role_id] = msg.session_id;
		monitor()->db_view()->login_cid_role_id_map.insert(std::make_pair(cid, player_db_view->role_id));
		MSG_DEBUG("login role:%ld account:%s agent:%d server:%d session:%s", player_db_view->role_id, msg.account.c_str(), msg.agent_num, msg.server_num, msg.session_id.c_str());
	} else {
		Block_Buffer i_buf;
		i_buf.make_message(LOGIC_SYNC_LOGIN_SESSION);
		i_buf.write_int64(player->role_id());
		i_buf.write_string(msg.session_id);
		i_buf.write_int32(cid);
		i_buf.finish_message();
		player->send_to_gate(i_buf);

		MSG_DEBUG("relgoin role:%ld account:%s agent:%d server:%d session:%s", player_db_view->role_id, msg.account.c_str(), msg.agent_num, msg.server_num, msg.session_id.c_str());
	}

	return 0;
}

int Logic_Client_Request::process_player_create(int cid, MSG_10100005 &msg) {
	MSG_DEBUG("cid:%d============================================================", cid);

	msg.role_name.erase(0, msg.role_name.find_first_not_of(" \t"));
	msg.role_name.erase(msg.role_name.find_last_not_of(" \t")+1);

	if (msg.account.empty() || msg.role_name.empty() || msg.role_name.size() > 100 || msg.career <=0 || msg.career > 3 || msg.gender > 2 || !msg.agent_num || !msg.server_num || msg.session_id.empty()  || msg.timestamp.empty()) {
		MSG_USER("invalid parameter account:[%s], role_name:[%s], career:%d gender:%d, agent_num:%d, server_num:%d, session_id:[%s], timestamp:[%s]",
				msg.account.c_str(), msg.role_name.c_str(), msg.career, msg.gender, msg.agent_num, msg.server_num, msg.session_id.c_str(), msg.timestamp.c_str());
		return 	ERROR_CLIENT_PARAM;
	}

	// 封禁ip判断
	std::string ip;
	monitor()->get_client_peer_addr(cid, ip);
	if (GM_MANAGER->check_forbid_ip_set(ip)) {
		MSG_DEBUG("ip:[%s] forbid", ip.c_str());
		return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_CLOSE_CLIENT_FORBID_IP);
	}

	// md5验证
	bool validate = monitor()->get_validate_account(ip);
	if (validate && Utility_Func::validate_md5(CONFIG_CACHE->server_maintainer_cache().md5_key.c_str(), msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str())) {
		MSG_DEBUG("login validate fial account:%s  time:%s flag:%s", msg.account.c_str(), msg.timestamp.c_str(), msg.session_id.c_str());
		return monitor()->close_client_by_cid(cid, Time_Value(2), ERROR_CLOSE_CLIENT_MD5_CHECK);
	}

	// 平台id服id验证
	DB_View::Server_Key_Amount_Map::iterator s_find_it = monitor()->db_view()->server_amount_map.find(Server_Key(msg.agent_num, msg.server_num));
	if (s_find_it == monitor()->db_view()->server_amount_map.end()) {
		MSG_DEBUG("error agent_num:%d, server_num:%d", msg.agent_num, msg.server_num);
		return ERROR_AGENT_SERVER_ID;
	}

	// 是否正在登录/创建流程
	Int_Uid::iterator c_find_it = monitor()->db_view()->login_cid_role_id_map.find(cid);
	if (c_find_it != monitor()->db_view()->login_cid_role_id_map.end()) {
		MSG_DEBUG("role login_map role_id:%ld, role_name:%s, cid:%d ", c_find_it->second, msg.role_name.c_str(), cid);
		return ERROR_ROLE_IN_LOGIN_MAP;
	}

	// 账号是否存在
	DB_View::Account_Player_View_Map::iterator a_find_it = monitor()->db_view()->account_player_view_map.find(Account_Key(msg.account, msg.agent_num, msg.server_num));
	if (a_find_it != monitor()->db_view()->account_player_view_map.end()) {
		MSG_DEBUG("account repeat account:%s  agent:%d server:%d", msg.account.c_str(), msg.agent_num, msg.server_num);
		return ERROR_ROLE_EXIST;
	}

	// 角色名是否存在
	DB_View::Name_Player_View_Map::iterator n_find_it = monitor()->db_view()->name_player_view_map.find(msg.role_name);
	if (n_find_it != monitor()->db_view()->name_player_view_map.end()) {
		MSG_DEBUG("role exist role_name:%s ", msg.role_name.c_str());
		return ERROR_EXIST_ROLE_NAME;
	}

	Logic_Player_Detail *detail = POOL_MANAGER->pop_player_data_pool(6);
	if (! detail) {
		MSG_USER("DB_MANAGER_INSTANCE->player_data_pool().pop() return 0.");
		return -1;
	}

	detail->role_id = s_find_it->second + 1;
	detail->base_detail.account_info = Account_Key(msg.account, msg.agent_num, msg.server_num);
	detail->base_detail.role_name = msg.role_name;
	detail->base_detail.gender = msg.gender;
	detail->base_detail.career = msg.career;
	detail->base_detail.ip = ip;
	//创号时间
	detail->base_detail.create_time = Time_Value::gettimeofday();

	// 初始化数据
	Logic_Player::create_init_player(*detail);

	// 登录游戏
	Logic_Player *player = process_success_login(cid, *detail);
	if (player) {
		s_find_it->second++;

		// 存入数据库
		player->set_detail_change(true);
		player->save_player_data();

		// 添加进DB_View
		Player_DB_View *player = POOL_MANAGER->pop_player_db_view();
		player->role_id = detail->role_id;
		player->account_info = detail->base_detail.account_info;
		player->role_name = detail->base_detail.role_name;
		player->level = detail->base_detail.level;
		player->gender = detail->base_detail.gender;
		player->career = detail->base_detail.career;
		player->head_id = detail->base_detail.head_id;
		player->last_sign_out_time = detail->base_detail.last_sign_out_time;
		player->last_sign_in_time = detail->base_detail.last_sign_in_time;
		player->fcm = msg.fcm;
		player->title_id = detail->base_detail.title_id;
		player->friend_.head_photo_id = detail->friend_detail.head_photo_id;
		player->ip = ip; //记录玩家登陆IP
		monitor()->db_view()->insert_new_player(player);

		// 设置登录状态
		player->set_account_status(Player_DB_View::ACCOUNT_DOING_LOGIN, cid);
		monitor()->db_view()->abnormal_account_add(player->role_id);
		monitor()->db_view()->role_session_map[player->role_id] = msg.session_id;
		monitor()->db_view()->login_cid_role_id_map.insert(std::make_pair(cid, player->role_id));

		// 删除随机名字
		LOGIC_PUBLIC->erase_name(player->gender, player->role_name);

		//判断是否有邀请人
		if(!msg.invite_user.empty()) {
			DB_View::Account_Role_View_Map::iterator iter = LOGIC_MONITOR->db_view()->account_role_view_map.find(msg.invite_user);
			if(iter != LOGIC_MONITOR->db_view()->account_role_view_map.end()) {
				for(std::vector<Player_DB_View *>::iterator pit = iter->second.begin(); pit != iter->second.end(); ++pit) {
					if(*pit) {
						Logic_Player *invite_player = LOGIC_MONITOR->find_logic_player_by_role_id((*pit)->role_id);
						if(invite_player) {//online
							invite_player->set_inv_time(Time_Value::gettimeofday());
						} else { //not online
							Logic_Player::offline_invite_player((*pit)->role_id, Time_Value::gettimeofday());
						}
					}
				}
			}
		}

		MSG_DEBUG("create success account:[%s], role_name:[%s], gender:%d, agent_num:%d, server_num:%d, session_id:[%s], timestamp:[%s]",
				msg.account.c_str(), msg.role_name.c_str(), msg.gender, msg.agent_num, msg.server_num, msg.session_id.c_str(), msg.timestamp.c_str());
	} else {
		POOL_MANAGER->push_player_data_pool(detail, 19);

		MSG_DEBUG("create login fail, account:[%s], role_name:[%s], gender:%d, agent_num:%d, server_num:%d, session_id:[%s], timestamp:[%s]",
				msg.account.c_str(), msg.role_name.c_str(), msg.gender, msg.agent_num, msg.server_num, msg.session_id.c_str(), msg.timestamp.c_str());

		return ERROR_INNER_SERVER;
	}

	return 0;
}

int Logic_Client_Request::record_access_count(int cid, MSG_10188888 &msg) {
	return LOGIC_PUBLIC->record_access_count(msg.type);
}

int Logic_Client_Request::process_10199999(Logic_Player &player, Block_Buffer &buf) {
	MSG_10199999 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.test_command(msg.command);
}

int Logic_Client_Request::process_10188888(Logic_Player &player, Block_Buffer &buf) {
	MSG_10188888 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return LOGIC_PUBLIC->record_access_count(msg.type);
}

int Logic_Client_Request::process_10100010(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.set_plot_info(msg.plot_id, msg.plot_step);
}

int Logic_Client_Request::process_10100015(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100015 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.client_ready_now();
}

int Logic_Client_Request::process_10100006(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100006 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_role_info(msg.role_id);
}

int Logic_Client_Request::process_10100008(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100008 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rename(msg.new_name);
}

int Logic_Client_Request::process_10100032(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100032 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_open_role_panel_logic(msg.role_id, msg.open_type);
}

int Logic_Client_Request::process_10100100(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100100 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_fetch_package_info(msg.pack_type);
}

int Logic_Client_Request::process_10100101(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100101 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_add_capacity(msg.pay_type, msg.pack_type, msg.add_capacity);
}

int Logic_Client_Request::process_10100102(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100102 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_discard_item(msg.index);
}

int Logic_Client_Request::process_10100103(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100103 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_move_item_msg(msg.index_from, msg.index_to);
}

int Logic_Client_Request::process_10100104(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100104 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_split_item(msg.index, msg.split_num);
}

int Logic_Client_Request::process_10100105(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100105 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_sort_item(msg.pack_type);
}

int Logic_Client_Request::process_10100106(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100106 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.item_user_use_item(msg.index, msg.amount, msg.e_value);
}

int Logic_Client_Request::process_10100107(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100107 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.use_special_gift(msg.item_index, msg.select_index_vector);
}

int Logic_Client_Request::process_10100108(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100108 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_merge_item(msg.pack_type);
}

int Logic_Client_Request::process_10100109(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100109 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_sell_item(msg.index);
}

int Logic_Client_Request::process_10100110(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100110 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_move_tower_package_item(msg.index);
}

int Logic_Client_Request::process_10100111(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100111 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pack_move_item_from_package(msg.type, msg.index);
}

int Logic_Client_Request::process_10100112(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100112 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	MSG_50100112 res_msg;
	res_msg.role_id = msg.role_id;
	Logic_Player *target = LOGIC_MONITOR->find_logic_player_by_role_id(msg.role_id);
	if (target) {
		target->pack_fetch_package_info_other(res_msg);
	} else {
		Player_DB_View *target = LOGIC_MONITOR->find_player_db_view_by_role_id(msg.role_id);
		if (target) {

		}
	}
	player.task_listen_browse_other_player_info(msg.role_id);
	{
		OBJ_SEND_TO_CLIENT(res_msg, player);
	}
	return 0;
}

int Logic_Client_Request::process_10100113(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100113 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_buy_and_use(msg.item_id, msg.amount);
}

//邮件有关
int Logic_Client_Request::process_10150000(Logic_Player &player, Block_Buffer &buf) {
    MSG_10150000 msg;
    JUDGE_MSG_ERROR_RETURN(msg);
    return player.fetch_mail_state_list(msg.page_count, msg.require_mail_state);
}

int Logic_Client_Request::process_10150001(Logic_Player &player, Block_Buffer &buf) {
    MSG_10150001 msg;
    JUDGE_MSG_ERROR_RETURN(msg);
    return player.view_mail(msg.mail_id);
}

int Logic_Client_Request::process_10150002(Logic_Player &player, Block_Buffer &buf) {
    MSG_10150002 msg;
    JUDGE_MSG_ERROR_RETURN(msg);
    return player.client_send_mail(msg.mail_info);
}

int Logic_Client_Request::process_10150003(Logic_Player &player, Block_Buffer &buf) {
    MSG_10150003 msg;
    JUDGE_MSG_ERROR_RETURN(msg);
    return player.pickup_mail_item(msg.mail_id, msg.item_vector_id, msg.type);
}

int Logic_Client_Request::process_10150004(Logic_Player &player, Block_Buffer &buf) {
    MSG_10150004 msg;
    JUDGE_MSG_ERROR_RETURN(msg);
    return player.pickup_mail_all_item();
}

int Logic_Client_Request::process_10150005(Logic_Player &player, Block_Buffer &buf) {
    MSG_10150005 msg;
    JUDGE_MSG_ERROR_RETURN(msg);
    return player.mutidelete_mail(msg.muti_mail_id);
}

int Logic_Client_Request::process_10110000(Logic_Player &player, Block_Buffer &buf) {
//	MSG_10110000 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
	return player.task_get_list();
}

/*int Logic_Client_Request::process_10110001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.task_npc_list(msg.npc_id, msg.source);
}*/

int Logic_Client_Request::process_10110002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.task_accept(msg.task_id);
}

int Logic_Client_Request::process_10110003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.task_abandon(msg.task_id);
}

int Logic_Client_Request::process_10110004(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.task_submit(msg.task_id);
}

int Logic_Client_Request::process_10110007(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110007 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.task_fast_finish(msg.task_id, msg.type, msg.is_bind_diamond_first);
}

int Logic_Client_Request::process_10110008(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110008 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.task_handle_plot_info(msg);
}

int Logic_Client_Request::process_10110009(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110009 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.reqFinishNpcTalkTask(msg.npc_id);
}

int Logic_Client_Request::process_10110010(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.task_clean_plot_info(msg.plot_ids);
}

int Logic_Client_Request::process_10110011(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_task_add_npc(msg.npc_type, msg.set, msg.npc_id, msg.funcName, msg.direction, msg.sceneId, msg.open, msg.x, msg.y, msg.role_name);
}

int Logic_Client_Request::process_10110012(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110012 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_reward_task_list(msg.is_refresh);
}

int Logic_Client_Request::process_10110013(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_reward_task_list();
}

int Logic_Client_Request::process_10110014(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110014 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
    player.task_listen_chat_with_friend(msg.role_id);
    return 0;
}

int Logic_Client_Request::process_10110015(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110015 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_commit_item(msg.task_id);
}

int Logic_Client_Request::process_10110016(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110016 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_recover_task_chain(msg.type);
}

int Logic_Client_Request::process_10110018(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110018 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_show_day_task_list();
}

int Logic_Client_Request::process_10110019(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110019 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_player_readed_timeout_try_message(msg.type);
}

//handle role get all its single dungeon's data
int Logic_Client_Request::process_10100200(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100200 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_single_dungeon_info();
}

int Logic_Client_Request::process_10100201(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100201 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	if(msg.type == 0){
		return player.req_open_card(msg.index);
	}else{
		return player.req_open_single_card(msg.index);
	}

}

int Logic_Client_Request::process_10100202(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100202 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_elite_dungeon_info(msg.type);
}

int Logic_Client_Request::process_10100204(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100204 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_reset_elite_dungeon_times(msg.bind_gold_first);
}

int Logic_Client_Request::process_10100205(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100205 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_chapter_award(msg.chapter);
}

int Logic_Client_Request::process_10100206(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100206 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_elite_buy_add_num(msg.scene_id);
}

int Logic_Client_Request::process_10100208(Logic_Player &player, Block_Buffer &buf) {
	//MSG_10100208 msg;
	//JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_elite_first_pass();
}

int Logic_Client_Request::process_10101000(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_equip_on_off(msg.index_from, msg.index_to);
}

int Logic_Client_Request::process_10101001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_equip_unban(msg.index, msg.lucky_stone_id, msg.auto_buy);
}

int Logic_Client_Request::process_10101002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_equip_master(msg.index, msg.auto_buy);
}

int Logic_Client_Request::process_10101003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_equip_compose(msg.id, msg.amount, msg.use_diamond);
}

int Logic_Client_Request::process_10101004(Logic_Player &player, Block_Buffer &buf) {
	return player.req_equip_master_info();
}

int Logic_Client_Request::process_10101005(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_equip_enchant(msg.equip_index, msg.is_bat, msg.auto_buy_stone, msg.auto_buy_lock, msg.locks);
}

int Logic_Client_Request::process_10101006(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101006 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_equip_enchant_replace_prop_group(msg.equip_index, msg.group_index);
}

int Logic_Client_Request::process_10101007(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101007 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_embed_gemstone(msg.equip_index, msg.hole_index, msg.gemstone_id);
}

int Logic_Client_Request::process_10101008(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101008 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_unembed_gemstone(msg.equip_index, msg.hole_index);
}

int Logic_Client_Request::process_10101009(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101009 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_open_equip_gemstone_hole(msg.equip_index, msg.hole_index);
}

int Logic_Client_Request::process_10101010(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_gemstone_convert(msg.src_gemstone_id, msg.des_gemstone_id, msg.num);
}

int Logic_Client_Request::process_10101011(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_gem_enable_info();
}

int Logic_Client_Request::process_10101012(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101012 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_equip_breakdown(msg.equip_index);
}

int Logic_Client_Request::process_10101013(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101013 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_equip_breakdown_prop(msg.equip_index);
}

int Logic_Client_Request::process_10101014(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101014 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gem_carve_info(msg.gem_essence_num);
}

int Logic_Client_Request::process_10101015(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101015 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gem_carve_lvl_info();
}

int Logic_Client_Request::process_10103000(Logic_Player &player, Block_Buffer &buf) {
	return player.req_fetch_rune_info();
}

int Logic_Client_Request::process_10103001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.req_rune_activate(msg.part, msg.hole_index, msg.auto_buy);
	return 0;
}

int Logic_Client_Request::process_10103002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.req_rune_fill_energy(msg.part, msg.hole_index, msg.is_bat, msg.auto_buy);
	return 0;
}

int Logic_Client_Request::process_10103200(Logic_Player &player, Block_Buffer &buf) {
	return player.req_rune_stone_syst_info();
}

int Logic_Client_Request::process_10103201(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103201 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_smelt(msg.smelter_idx, msg.is_bat);
}

int Logic_Client_Request::process_10103202(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103202 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_sell(msg.is_bat, msg.under_color, msg.index);
}

int Logic_Client_Request::process_10103203(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103203 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_pick(msg.is_bat, msg.index);
}

int Logic_Client_Request::process_10103204(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103204 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_lock(msg.index, msg.is_lock);
}

int Logic_Client_Request::process_10103205(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103205 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_merge(msg.is_bat, msg.index_type, msg.index_from, msg.index_to);
}

int Logic_Client_Request::process_10103206(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103206 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_move(msg.index_from, msg.index_to);
}

int Logic_Client_Request::process_10103207(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103207 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_exchange(msg.item_id);
}

int Logic_Client_Request::process_10103208(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103208 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_points();
}

int Logic_Client_Request::process_10103209(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103209 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_on_off(msg.index_from, msg.index_to);
}

int Logic_Client_Request::process_10103210(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103210 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_essence();
}

int Logic_Client_Request::process_10103211(Logic_Player &player, Block_Buffer &buf) {
	MSG_10103211 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_rune_stone_essence_exchange(msg.item_id);
}
// learn skill
int Logic_Client_Request::process_10100140(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100140 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.get_all_learned_skills(msg.talent_id);
}

int Logic_Client_Request::process_10100141(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100141 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.manual_learn_skill(msg.talent_id, msg.skill_id, msg.lv);
}

//int Logic_Client_Request::process_10100142(Logic_Player &player, Block_Buffer &buf) {
//	MSG_10100142 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.open_a_skill_talent(msg.page_id, msg.talent_id);
//}

int Logic_Client_Request::process_10100143(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100143 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.open_a_talent_page(msg.talent_id);
}

int Logic_Client_Request::process_10100144(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100144 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.use_a_talent_page(msg.talent_id);
}

int Logic_Client_Request::process_10100165(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100165 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.erase_skill(msg.talent_id, msg.type);
	return 0;
}

int Logic_Client_Request::process_10100166(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100166 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.move_skill(msg.skill_id, msg.new_loc, msg.talent_id);
}

//friend
int Logic_Client_Request::process_10160000(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.fetch_friend_list();
}

int Logic_Client_Request::process_10160001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.get_friend_info(msg.role_id);
}
int Logic_Client_Request::process_10160002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.fetch_apply_list();
	return 0;
}

int Logic_Client_Request::process_10160003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.remove_friend(msg.friend_id, msg.friend_type);
	return 0;
}

int Logic_Client_Request::process_10160004(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.append_add_friend(msg.role_name);
}

int Logic_Client_Request::process_10160013(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160013 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.multi_add_friend(msg.role_name_vec);
}

int Logic_Client_Request::process_10160005(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.add_to_black(msg.role_name);
}

int Logic_Client_Request::process_10160009(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160009 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
//	if(msg.handel_friend_apply){
		return player.handle_friend_apply(msg.handel_friend_apply, msg.role_name);
//	}else{
//		return 0;
//	}
}

int Logic_Client_Request::process_10160010(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_role_online_status(msg.stranger_id_list, msg.recent_id_list);
}

int Logic_Client_Request::process_10160011(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_friend_set_photo();
}

int Logic_Client_Request::process_10160012(Logic_Player &player, Block_Buffer &buf) {
	MSG_10160012 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_set_head_photo(msg.photo_id);
}

int Logic_Client_Request::process_10160014(Logic_Player &player, Block_Buffer &buf) {
	return player.req_friend_base_info_list();
}

int Logic_Client_Request::process_10102000(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_heroer_info();
}

int Logic_Client_Request::process_10102001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_hero_level_up(msg.hero_index);
}

int Logic_Client_Request::process_10102002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_hero_equip_on_off(msg.index_from, msg.index_to);
}

int Logic_Client_Request::process_10102003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_set_hero_out_state(msg.is_out);
}

//请求商品列表（商城）
int Logic_Client_Request::process_10151000(Logic_Player &player, Block_Buffer &buf){
	MSG_10151000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_mall_list();
}
int Logic_Client_Request::process_10151001(Logic_Player &player, Block_Buffer &buf){
	MSG_10151001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_buy_goods_in_mall(msg.id, msg.amount,  msg.tabs,  msg.role_name, msg.buy_type, 0, msg.helper_fast_use);
}

int Logic_Client_Request::process_10151002(Logic_Player &player, Block_Buffer &buf){
	MSG_10151002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_one_key_buy_goods_in_mall(msg.tabs, msg.ids);
}

int Logic_Client_Request::process_10151003(Logic_Player &player, Block_Buffer &buf){
	MSG_10151003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fast_buy_item_in_mall_tabs(msg.item_id, msg.amount, msg.buy_type,  msg.helper_fast_use);
}

int Logic_Client_Request::process_10102004(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	int ret = player.req_hero_recruit(msg.hero_id, msg.use_diamond);

	if(!ret && msg.hero_id == 60230004){// 招募凯莱布丽安跟随
		player.set_hero_out_state(1);
	}
	// 第一次招募时出战
	if (!ret && player.req_hero_amount() == 1) {
		int index = player.heroer_get_hero_index(msg.hero_id);
		player.req_hero_set_fight(index, 1, false);
	}
	return ret;
}

int Logic_Client_Request::process_10102005(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_hero_set_formation(msg.hero_index, msg.hero_formation, msg.is_alert);
}

int Logic_Client_Request::process_10102006(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102006 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_hero_set_fight(msg.hero_index, msg.is_fight);
}

int Logic_Client_Request::process_10102007(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102007 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_hero_awake(msg.is_hero, msg.hero_index);
}

int Logic_Client_Request::process_10102020(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102020 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_artifact_upgrade(msg.is_hero, msg.artifact_id, msg.hero_index);
}

int Logic_Client_Request::process_10102021(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102021 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_new_hero_click(msg.hero_id);
}

int Logic_Client_Request::process_10102022(Logic_Player &player, Block_Buffer &buf) {
	return player.req_fetch_clicked_new_heros();
}

int Logic_Client_Request::process_10102032(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102032 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return player.handle_hero_assist(msg.type, msg.hero_idx, msg.loc);
}

int Logic_Client_Request::process_10102033(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102033 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return player.handle_hero_community(msg.id, msg.answer, msg.hero_idx);
}

int Logic_Client_Request::process_10102034(Logic_Player &player, Block_Buffer &buf) {
	MSG_10102034 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return player.handle_gift_hero(msg.item_id, msg.hero_idx);
}

int Logic_Client_Request::process_10100300(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100300 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.fetch_team_info(msg.type);
}

int Logic_Client_Request::process_10100301(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100301 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.create_team(msg.scene_id, msg.password, msg.type, msg.degree);
}

int Logic_Client_Request::process_10100302(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100302 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	int ret = 0;
	if (msg.role_id) {
		ret = player.invite_role(msg);
	} else {
		//
	}
	return ret;
}

int Logic_Client_Request::process_10100303(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100303 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.apply_team(msg);
}

int Logic_Client_Request::process_10100304(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100304 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.make_leader(msg.role_id);
}

int Logic_Client_Request::process_10100305(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100305 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.kick_role(msg.role_id);
}

int Logic_Client_Request::process_10100306(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100306 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.leave_team();
}

int Logic_Client_Request::process_10100308(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100308 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.profile_info(msg.type);
}

int Logic_Client_Request::process_10100309(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100309 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.agree_team_invite(msg);
}

int Logic_Client_Request::process_10100310(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100310 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.change_team_setting(msg.profile_set);
}

int Logic_Client_Request::process_10100312(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100312 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.team_enter_wait_area(player.role_id(), msg.type);
}

int Logic_Client_Request::process_10100313(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100313 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.team_exit_wait_area(msg.type);
}

int Logic_Client_Request::process_10100315(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100315 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.listen_team_info(msg.type, msg.listened);
}

int Logic_Client_Request::process_10100316(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100316 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.reject_team_invite(msg.role_id);
}

int Logic_Client_Request::process_10100317(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100317 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_apply_team_quick(msg.type);
}

int Logic_Client_Request::process_10100318(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100318 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.set_password(msg.password_new);
}

int Logic_Client_Request::process_10100319(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100319 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.cancel_password(msg.password);
}

int Logic_Client_Request::process_10100320(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100320 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_set_ready_state(msg.ready);
}

int Logic_Client_Request::process_10100321(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100321 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.active_wait_area(msg.type);
}

int Logic_Client_Request::process_10100322(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100322 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_team_set_position(msg.set, msg.position);
}

int Logic_Client_Request::process_10100323(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100323 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_team_set_info(msg.type, msg.scene_id, msg.degree);
}

int Logic_Client_Request::process_10100324(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100324 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_team_lineup_info();
}

int Logic_Client_Request::process_10100325(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100325 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_team_lineup_set(msg.info);
}

int Logic_Client_Request::process_10100326(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100326 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_back_room();
}

int Logic_Client_Request::process_10100327(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100327 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_team_hero_dream();
}

int Logic_Client_Request::process_10100328(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100328 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_team_follow(msg.follow);
}

int Logic_Client_Request::process_10100330(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100330 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_team_info(msg);
}

int Logic_Client_Request::process_10100331(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100331 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_team_quick_invite(msg);
}

int Logic_Client_Request::process_10100333(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100333 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.get_team_award(msg.season);
}

int Logic_Client_Request::process_10170000(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_create_gang(msg.logo_id, msg.name, msg.money_type);
}

int Logic_Client_Request::process_10170001(Logic_Player& player, Block_Buffer& buf) {
//	MSG_10170001 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.req_join_gang( msg.gang_id );
	return 0;
}

int Logic_Client_Request::process_10170002(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_info( msg.gang_id );
}

int Logic_Client_Request::process_10170003(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_ganger_info(msg.role_id);
}

int Logic_Client_Request::process_10170004(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_list();
}

int Logic_Client_Request::process_10170005(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_ganger_list( msg.gang_id );
}

int Logic_Client_Request::process_10170006(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170006 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_quit_gang();
}

int Logic_Client_Request::process_10170007(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170007 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_leader_resign(msg.role_id);
}

int Logic_Client_Request::process_10170008(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170008 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_info_listen( msg.listen );
}

int Logic_Client_Request::process_10170009(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170009 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_kick_out_gang(msg.role_id);
}

int Logic_Client_Request::process_10170010(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_set_gang_announce( msg.announce );
}

int Logic_Client_Request::process_10170011(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_invite_to_join( msg.role_ids );
}

int Logic_Client_Request::process_10170012(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170012 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_passed_events();
}

int Logic_Client_Request::process_10170013(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170013 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_simple_passed_events();
}

int Logic_Client_Request::process_10170014(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170014 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_group_mail( msg.mail_info );
}

int Logic_Client_Request::process_10170015(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170015 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_donate(msg.type, msg.money);
}

int Logic_Client_Request::process_10100090(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100090 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fcm_pass(msg.fcm);
}

int Logic_Client_Request::process_10170016(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170016 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_set_join_block( msg.join_block );
}

int Logic_Client_Request::process_10170017(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170017 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_invite_list();
}

int Logic_Client_Request::process_10170018(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170018 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_apply_list();
}

int Logic_Client_Request::process_10170019(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170019 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_list_action(msg.gang_id, msg.action);
}

int Logic_Client_Request::process_10170020(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170020 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_invite_action(msg.gang_id, msg.action);
}

int Logic_Client_Request::process_10170021(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170021 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_apply_action(msg.role_id, msg.action);
}

int Logic_Client_Request::process_10170022(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170022 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_ganger_change_headship_auth(msg.member_role_id, msg.headship, msg.add_auth_vec);
}

int Logic_Client_Request::process_10170023(Logic_Player& player, Block_Buffer& buf) {
	return player.req_gang_upgrade();
}

int Logic_Client_Request::process_10170024(Logic_Player& player, Block_Buffer& buf) {
	return player.req_gang_upgrade_send_gift();
}

int Logic_Client_Request::process_10170025(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170025 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_upgrade_cd_clear(msg.cd_type, msg.building_id);
}

int Logic_Client_Request::process_10170026(Logic_Player& player, Block_Buffer& buf) {
	return player.req_gang_world_notice();
}

int Logic_Client_Request::process_10170027(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170027 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_logo_modify(msg.logo_id);
}

int Logic_Client_Request::process_10170028(Logic_Player& player, Block_Buffer& buf) {
	return player.req_gang_seize_power();
}

int Logic_Client_Request::process_10170029(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170029 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_send_bless_buf(msg.target_role_id, msg.bless_buf_id);
}

int Logic_Client_Request::process_10170030(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170030 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_research_gang_skill(msg.skill_id);
}

int Logic_Client_Request::process_10170031(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170031 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_learn_ganger_skill(msg.skill_id);
}

int Logic_Client_Request::process_10170032(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170032 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_draw();
}

int Logic_Client_Request::process_10170033(Logic_Player& player, Block_Buffer& buf) {
	return player.req_gang_take_draw_item();
}

int Logic_Client_Request::process_10170034(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170034 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_building_upgrade(msg.building_id);
}

int Logic_Client_Request::process_10170035(Logic_Player& player, Block_Buffer& buf) {
	return player.req_gang_war_enter();
}

int Logic_Client_Request::process_10170036(Logic_Player& player, Block_Buffer& buf) {
	return GANG_MANAGER->req_gang_war_board(player);
}

int Logic_Client_Request::process_10170037(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170037 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return GANG_MANAGER->req_gang_war_spot(player, msg.index);
}

int Logic_Client_Request::process_10170038(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170038 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_war_award();
}

int Logic_Client_Request::process_10170039(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170039 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_war_award_assign(msg.award);
}

int Logic_Client_Request::process_10170041(Logic_Player& player, Block_Buffer& buf) {
	MSG_10170041 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gang_shop_buy_item(msg.item_id, msg.amount);
}

int Logic_Client_Request::process_10170042(Logic_Player& player, Block_Buffer& buf) {
	return player.req_gang_war_draw();
}

int Logic_Client_Request::process_10170043(Logic_Player& player, Block_Buffer& buf) {
	return GANG_MANAGER->req_gang_war_get_rank_award(player);//player.req_gang_war_get_rank_award();
}

int Logic_Client_Request::process_10170044(Logic_Player& player, Block_Buffer& buf) {
	return GANG_MANAGER->req_gang_war_rank_award_list(player);//player.req_gang_war_rank_award_list();
}

int Logic_Client_Request::process_10170045(Logic_Player& player, Block_Buffer& buf) {
	return GANG_MANAGER->req_gang_war_gang_rank(player);
}

int Logic_Client_Request::process_10170047(Logic_Player &player, Block_Buffer &buf) {
	MSG_10170047 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.start_phy_power_practice();
}

int Logic_Client_Request::process_10170048(Logic_Player &player, Block_Buffer &buf) {
	MSG_10170048 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.stop_phy_power_practice(true);
}

int Logic_Client_Request::process_10170049(Logic_Player &player, Block_Buffer &buf) {
	MSG_10170049 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.speed_up_practice();
}

int Logic_Client_Request::process_10152000(Logic_Player &player, Block_Buffer &buf) {
	MSG_10152000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_alchemy_info(msg.mtype);
}

int Logic_Client_Request::process_10152001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10152001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	if(msg.mtype) {
		return player.req_souls_alchemy(msg.tabs, msg.pay_type);
	} else {
		return player.req_alchemy(msg.tabs, msg.pay_type);
	}
}

int Logic_Client_Request::process_10100400(Logic_Player &player, Block_Buffer &buf){
	MSG_10100400 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.fetch_arena_rank_info(msg.type);
}

int Logic_Client_Request::process_10100405(Logic_Player &player, Block_Buffer &buf){
	MSG_10100405 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.fetch_arena_info(true);
}

int Logic_Client_Request::process_10100406(Logic_Player &player, Block_Buffer &buf){
	MSG_10100406 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.close_arena_panel();
}

int Logic_Client_Request::process_10100407(Logic_Player &player, Block_Buffer &buf){
	//MSG_10100407 msg;
	//JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_reset_arena_cd();
}

int Logic_Client_Request::process_10100408(Logic_Player &player, Block_Buffer &buf){
	//MSG_10100408 msg;
	//JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_arena_panel_info();
}
int Logic_Client_Request::process_10100410(Logic_Player &player, Block_Buffer &buf){
	MSG_10100410 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_match_arena_battle();
}


int Logic_Client_Request::process_10100412(Logic_Player &player, Block_Buffer &buf){
	MSG_10100412 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_arena_match();
}


int Logic_Client_Request::process_10100413(Logic_Player &player, Block_Buffer &buf){
	MSG_10100413 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_arena_fight(msg.index);
}


int Logic_Client_Request::process_10100414(Logic_Player &player, Block_Buffer &buf){
	MSG_10100414 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_arena_match_refresh();
}

int Logic_Client_Request::process_10100415(Logic_Player &player, Block_Buffer &buf){
	MSG_10100415 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_arena_role_panel(msg.role_id, msg.index);
}

int Logic_Client_Request::process_10100420(Logic_Player &player, Block_Buffer &buf){
	MSG_10100420 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_buy_arena_pay_times(msg.buy_type);
}

int Logic_Client_Request::process_10100401(Logic_Player &player, Block_Buffer &buf){
	MSG_10100401 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.receive_reward(msg.type);
}

int Logic_Client_Request::process_10100402(Logic_Player &player, Block_Buffer &buf){
	MSG_10100402 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_arena_award_info();
}

int Logic_Client_Request::process_10100416(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100416 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_arena_worship(msg.role_id,msg.worship_type);
}

int Logic_Client_Request::process_10100417(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100417 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_arena_panel_refresh();
}

int Logic_Client_Request::process_10104000(Logic_Player &player, Block_Buffer &buf) {
	MSG_10104000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_guide(msg.id);
}

int Logic_Client_Request::process_10104001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10104001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_guide_set(msg.guide_id, msg.step);
}

int Logic_Client_Request::process_10104002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10104002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_save_played_plot(msg.plot_id);
}

int Logic_Client_Request::process_10120095(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120095 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_guide_add_item_in_plot(msg.item_id, msg.item_num);
}

int Logic_Client_Request::process_10104003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10104003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_task_guide(msg.guide_id);
}

int Logic_Client_Request::process_10171000(Logic_Player &player, Block_Buffer &buf){
	MSG_10171000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_ranking_info(msg.type);
}

int Logic_Client_Request::process_10171001(Logic_Player &player, Block_Buffer &buf){
	MSG_10171001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_ranking_role_info(msg.type, msg.rank);
}

int Logic_Client_Request::process_10171002(Logic_Player &player, Block_Buffer &buf){
	MSG_10171002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_ranking_item_info(msg.type, msg.rank, msg.roleId, msg.career);
}

int Logic_Client_Request::process_10171003(Logic_Player &player, Block_Buffer &buf){
	MSG_10171003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_ranking_heroer_info(msg.type, msg.rank);
}

int Logic_Client_Request::process_10120000(Logic_Player &player, Block_Buffer &buf){
	MSG_10120000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_buy_phy_power(msg.bind_gold_first);
}

int Logic_Client_Request::process_10120001(Logic_Player &player, Block_Buffer &buf){
	MSG_10120001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.fetch_phy_power_info();
}

int Logic_Client_Request::process_10120002(Logic_Player &player, Block_Buffer &buf){
	MSG_10120002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_chat_bugle(msg.content);
}

int Logic_Client_Request::process_10100500(Logic_Player &player, Block_Buffer &buf){
	MSG_10100500 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
//	if (msg.item_id > 0) {
//		msg.item_vector.push_back(msg.item_id);
//	}
	return player.req_exchange(msg.item_id, msg.type, msg.amount);
}

int Logic_Client_Request::process_10106000(Logic_Player &player, Block_Buffer &buf){
	return player.req_daily();
}

int Logic_Client_Request::process_10106001(Logic_Player &player, Block_Buffer &buf){
	return player.req_activity();
}

int Logic_Client_Request::process_10106003(Logic_Player &player, Block_Buffer &buf){
	return player.req_today_open_active();
}

int Logic_Client_Request::process_10106004(Logic_Player &player, Block_Buffer &buf){
	return player.req_upgrade_score();
}

int Logic_Client_Request::process_10106005(Logic_Player &player, Block_Buffer &buf){
	MSG_10106005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_upgrade_get_status(msg.get_type, msg.tip_name, msg.goodsId, msg.isAuto);
}

int Logic_Client_Request::process_10106006(Logic_Player &player, Block_Buffer &buf){
	return player.req_helper_check_in();
}

int Logic_Client_Request::process_10106002(Logic_Player &player, Block_Buffer &buf){
	MSG_10106002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_activity_receive(msg.type, msg.id);
}

int Logic_Client_Request::process_10100501(Logic_Player &player, Block_Buffer &buf){
	MSG_10100501 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_exchange_info(msg.type);
}

int Logic_Client_Request::process_10100502(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100502 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_exchange_skill_item(msg.id);
}


int Logic_Client_Request::process_10111102(Logic_Player &player, Block_Buffer &buf){
	MSG_10111102 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_world_boss_kill_info(msg.icon_id);
}

int Logic_Client_Request::process_10111103(Logic_Player &player, Block_Buffer &buf){
	MSG_10111103 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_world_boss_set_time(msg.icon_id, msg.hour, msg.minute);
}

int Logic_Client_Request::process_10111000(Logic_Player &player, Block_Buffer &buf){
	return player.req_war_scene_enter();
}

int Logic_Client_Request::process_10111001(Logic_Player &player, Block_Buffer &buf){
	return player.req_war_scene_enter_num();
}

int Logic_Client_Request::process_10111003(Logic_Player &player, Block_Buffer &buf){
	return player.req_war_scene_receive();
}

int Logic_Client_Request::process_10120003(Logic_Player &player, Block_Buffer &buf) {
	return player.req_get_sys_cfg();
}

int Logic_Client_Request::process_10120004(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_set_sys_cfg(msg);
}

int Logic_Client_Request::process_10120020(Logic_Player &player, Block_Buffer &buf) {
	return player.req_fetch_awake_lvl();
}

int Logic_Client_Request::process_10120021(Logic_Player &player, Block_Buffer &buf) {
	return player.req_awake_lvl_upgrade();
}

int Logic_Client_Request::process_10120030(Logic_Player &player, Block_Buffer &buf) {
	return player.req_fetch_player_artifact_info();
}

int Logic_Client_Request::process_10120040(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120040 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_forenotice_receive_gift(msg.gift_lvl);
}

int Logic_Client_Request::process_10120041(Logic_Player &player, Block_Buffer &buf) {
	return player.req_forenotice_received_gifts();
}

int Logic_Client_Request::process_10120080(Logic_Player &player, Block_Buffer &buf) {
	return player.req_exploit_info();
}

int Logic_Client_Request::process_10106100(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106100 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_buy_covenant(msg.type, msg.id);
}

int Logic_Client_Request::process_10106101(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106101 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_handsel_covenant(msg.type, msg.id, msg.role_name);
}

int Logic_Client_Request::process_10106102(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106102 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_start_covenant_become_time(msg.id);
}

int Logic_Client_Request::process_10180000(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_add_new_building(msg.id, msg.type, msg.consume_type);
}

int Logic_Client_Request::process_10180001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_upgrade_building(msg.id, msg.type);
}

int Logic_Client_Request::process_10180002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_quick_finish_building(msg.id, msg.type);
}

int Logic_Client_Request::process_10180003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_cancel_finish_building(msg.id, msg.type);
}

int Logic_Client_Request::process_10180004(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gain_production(msg.id, msg.type);
}

int Logic_Client_Request::process_10180005(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_step_foot(msg.role_id);
}

int Logic_Client_Request::process_10180006(Logic_Player &player, Block_Buffer &buf) {
	return 0;
}

int Logic_Client_Request::process_10180007(Logic_Player &player, Block_Buffer &buf) {
	player.req_get_dragon_vale_friend_list();
	return 0;
}

int Logic_Client_Request::process_10180008(Logic_Player &player, Block_Buffer &buf) {
	player.req_get_dranon_vale_gang_member_list();
	return 0;
}

int Logic_Client_Request::process_10180009(Logic_Player &player, Block_Buffer &buf) {
	return player.req_dragon_born();
}

int Logic_Client_Request::process_10180010(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_play_dragon(msg.role_id);
}

int Logic_Client_Request::process_10180011(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_get_dragon_total_info(msg.role_id);
}

int Logic_Client_Request::process_10180012(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180012 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_keep_dragon(msg.type);
}

int Logic_Client_Request::process_10180013(Logic_Player &player, Block_Buffer &buf) {
	return player.req_add_dragon_feeling();
}

int Logic_Client_Request::process_10180014(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180014 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_evolute_dragon(msg.type);
}

int Logic_Client_Request::process_10180015(Logic_Player &player, Block_Buffer &buf) {
	return player.req_rob_other_dragon_vale();
}

int Logic_Client_Request::process_10180016(Logic_Player &player, Block_Buffer &buf) {
	return player.req_rob_other_dragon_vale_match();
}

int Logic_Client_Request::process_10180017(Logic_Player &player, Block_Buffer &buf) {
	return player.req_get_dragon_vale_notice_board();
}

int Logic_Client_Request::process_10180018(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180018 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_gain_rob_dragon_vale_production(msg.to_uuid, msg.to_role_id, msg.uuid, msg.role_id,
												msg.building_id, msg.building_type);
}

int Logic_Client_Request::process_10180019(Logic_Player &player, Block_Buffer &buf) {
	//this message can't process from now on
/*	MSG_10180019 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	player.rob_dragon_vale_task_listen();*/
	return 0;
}

int Logic_Client_Request::process_10180020(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180020 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	player.req_batch_evolution_dragon(msg.type, msg.batch_num, msg.ten_times);
	return 0;
}

int Logic_Client_Request::process_10180021(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180021 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_dragon_building_gain_production_left_time(msg.id, msg.type);
}

int Logic_Client_Request::process_10180022(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180022 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_get_invite_friend_step_foot_list();
}

int Logic_Client_Request::process_10180023(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180023 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_invite_friend_step_foot(msg.role_id);
}

int Logic_Client_Request::process_10180024(Logic_Player &player, Block_Buffer &buf) {
	MSG_10180024 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_invite_all_friend_step_foot();
}

int Logic_Client_Request::process_10100800(Logic_Player &player, Block_Buffer &buf) {
	return player.req_achievement_list();
}

int Logic_Client_Request::process_10100801(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100801 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_achievement_get(msg.type, msg.id);
}

int Logic_Client_Request::process_10100802(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100802 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_achievement_medal_onoff(msg.source_index, msg.dst_index);
}

int Logic_Client_Request::process_10100900(Logic_Player &player, Block_Buffer &buf) {
	return player.req_fetch_title_info();
}

int Logic_Client_Request::process_10100901(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100901 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_title_on_off(msg.title_id, msg.op);
}

int Logic_Client_Request::process_10106200(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106200 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_open_vip(msg.id);
}

int Logic_Client_Request::process_10106201(Logic_Player &player, Block_Buffer &buf) {
	return player.req_vip_world_tree_info();
}

int Logic_Client_Request::process_10106203(Logic_Player &player, Block_Buffer &buf) {
	return player.req_vip_turntable_open();
}

int Logic_Client_Request::process_10106202(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106202 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_vip_receive_world_tree(msg.id);
}

int Logic_Client_Request::process_10106204(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106204 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_vip_turntable_lottery(msg.type);
}

int Logic_Client_Request::process_10106205(Logic_Player &player, Block_Buffer &buf) {
	return player.req_vip_turntable_receive();
}

int Logic_Client_Request::process_10106206(Logic_Player &player, Block_Buffer &buf) {
	return player.req_vip_daily_receive();
}

int Logic_Client_Request::process_10106207(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_vip_daily_receive();
}

int Logic_Client_Request::process_10100600(Logic_Player &player, Block_Buffer &buf) {
	return player.req_fecth_mount_info();
}

int Logic_Client_Request::process_10100601(Logic_Player &player, Block_Buffer &buf) {
	return player.req_fecth_mount_stable_info();
}

int Logic_Client_Request::process_10100602(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100602 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_mount_ride(msg.ride, msg.mount_id);
}

int Logic_Client_Request::process_10100603(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100603 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_mount_activate(msg.mount_id, msg.type);
}

int Logic_Client_Request::process_10100604(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100604 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_click_mount_group(msg.group_id);
}



int Logic_Client_Request::process_10100605(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100605 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	if(msg.type == 0){
		return player.req_mount_train(msg.mount_id);
	}else{
		return player.req_mount_train_auto_lv(msg.mount_id, msg.type);
	}
	return 0;
}

int Logic_Client_Request::process_10100606(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100606 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_mount_deploy(msg.mount_id, msg.deploy_pos);
}

int Logic_Client_Request::process_10100608(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100608 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_mount_learn_skill(msg.mount_id, msg.prop_id);
}


int Logic_Client_Request::process_10100609(Logic_Player &player, Block_Buffer &buf) {
	//MSG_10100609 msg;
	//JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_mount_fuse_time();
}


int Logic_Client_Request::process_10120050(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120050 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_handsel_flowers(msg.item_id, msg.role_name, msg.msg);
}

int Logic_Client_Request::process_10120051(Logic_Player &player, Block_Buffer &buf) {
	return player.req_look_flowers();
}

int Logic_Client_Request::process_10106309(Logic_Player &player, Block_Buffer &buf) {
	return player.req_reward_tip_num();
}

int Logic_Client_Request::process_10106300(Logic_Player &player, Block_Buffer &buf) {
	return player.req_receive_online_reward();
}

int Logic_Client_Request::process_10106301(Logic_Player &player, Block_Buffer &buf) {
	return player.req_reward_login_seven_info();
}

int Logic_Client_Request::process_10106302(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106302 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_login_seven(msg.days);
}

int Logic_Client_Request::process_10106303(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106303 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_activation_code_reward(msg.code);
}

int Logic_Client_Request::process_10106305(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106305 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_reward_back_receive(msg.type, msg.item_id);
}

int Logic_Client_Request::process_10106306(Logic_Player &player, Block_Buffer &buf) {
	return player.req_have_receive_keep_reward();
}

int Logic_Client_Request::process_10106307(Logic_Player &player, Block_Buffer &buf) {
	return player.req_keep_reward_success();
}

int Logic_Client_Request::process_10106308(Logic_Player &player, Block_Buffer &buf) {
	return player.req_receive_keep_reward();
}

int Logic_Client_Request::process_10106304(Logic_Player &player, Block_Buffer &buf) {
	return player.req_reward_back_info();
}

int Logic_Client_Request::process_10120010(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_secret_shop_info();
}

int Logic_Client_Request::process_10120011(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_secret_shop_buy_item(msg.index);
}

int Logic_Client_Request::process_10100210(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100210 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_wipe_out(msg.wipe_type, msg.num, msg.scene_id);
}

int Logic_Client_Request::process_10100211(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100211 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_interrupt_wipe_out(msg.wipe_type);
}

int Logic_Client_Request::process_10100212(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100212 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_complete_wipe_out(msg.wipe_type);
}

int Logic_Client_Request::process_10100213(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100213 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_obtain_wipe_double_income(msg.wipe_type);
}

int Logic_Client_Request::process_10100214(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100214 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_select_wipe_double(msg.wipe_type, msg.is_selected);
}

int Logic_Client_Request::process_10100700(Logic_Player &player, Block_Buffer &buf) {
	//MSG_10100700 msg;
	//JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_campfire_enter();
}

int Logic_Client_Request::process_10120070(Logic_Player &player, Block_Buffer &buf) {
	//MSG_10120070 msg;
	//JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_month_hero_info();
}


int Logic_Client_Request::process_10100040(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100040 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_act_enter_scene(msg.active_id);
}

int Logic_Client_Request::process_10106400(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_info();
}

int Logic_Client_Request::process_10106401(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106401 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_vip_club_reward(msg.id, msg.reward_id);
}

int Logic_Client_Request::process_10106402(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106402 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_level_ace_reward(msg.level);
}

int Logic_Client_Request::process_10106403(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106403 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_suit_collect_reward(msg.level);
}

int Logic_Client_Request::process_10106404(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106404 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_guild_level_reward(msg.id);
}

int Logic_Client_Request::process_10106406(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106406 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_gem_levelup_reward(msg.id);
}

int Logic_Client_Request::process_10106407(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106407 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_wing_levelup_reward(msg.level);
}

int Logic_Client_Request::process_10106408(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106408 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_mount_levelup_reward(msg.id);
}

int Logic_Client_Request::process_10106409(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106409 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_sg_pass_reward(msg.id);
}

int Logic_Client_Request::process_10106412(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106412 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_mount_pass_reward(msg.id);
}

int Logic_Client_Request::process_10106413(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106413 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_wing_pass_reward(msg.id);
}

int Logic_Client_Request::process_10106410(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106410 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_arena_dan_reward(msg.id);
}

int Logic_Client_Request::process_10106411(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106411 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_active_content_reward(msg.act_type, msg.id);
}

int Logic_Client_Request::process_10106405(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_tip_num();
}

int Logic_Client_Request::process_10106420(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_limit_buy();
}

int Logic_Client_Request::process_10106421(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_vip_club();
}

int Logic_Client_Request::process_10106422(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_level_ace();
}

int Logic_Client_Request::process_10106423(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_suit_collect();
}

int Logic_Client_Request::process_10106424(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_guild_level();
}

int Logic_Client_Request::process_10106425(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_level_rank();
}

int Logic_Client_Request::process_10106426(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_arena_rank();
}

int Logic_Client_Request::process_10106427(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_force_rank();
}

int Logic_Client_Request::process_10106431(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_sg_rank();
}

int Logic_Client_Request::process_10106448(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_mount_rank();
}

int Logic_Client_Request::process_10106449(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_wing_rank();
}

int Logic_Client_Request::process_10106428(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_gem_levelup();
}

int Logic_Client_Request::process_10106429(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_wing_levelup();
}

int Logic_Client_Request::process_10106430(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_mount_levelup();
}

int Logic_Client_Request::process_10106432(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_arena_dan();
}

int Logic_Client_Request::process_10106433(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_world_boss();
}

int Logic_Client_Request::process_10106434(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_suit_collect_two();
}

int Logic_Client_Request::process_10106435(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_heroes_door();
}

int Logic_Client_Request::process_10106436(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_team_arena();
}

int Logic_Client_Request::process_10106437(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_warer();
}

int Logic_Client_Request::process_10106438(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_reward_task();
}

int Logic_Client_Request::process_10106439(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_sgfb();
}

int Logic_Client_Request::process_10106440(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_fm_x_ci();
}

int Logic_Client_Request::process_10106441(Logic_Player &player, Block_Buffer &buf) {
	return player.	req_active_content_x_x_quality_heroer();
}

int Logic_Client_Request::process_10106442(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_mount_lvup_x();
}

int Logic_Client_Request::process_10106443(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_fusion_x_elite_card();
}

int Logic_Client_Request::process_10106444(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_alchemy_gift();
}

int Logic_Client_Request::process_10106445(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_activity_gift();
}

int Logic_Client_Request::process_10106446(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_ring_task();
}

int Logic_Client_Request::process_10106447(Logic_Player &player, Block_Buffer &buf) {
	return player.req_active_content_dragon_prey();
}

int Logic_Client_Request::process_10120052(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120052 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.send_proposal(msg.type, msg.content);
}

int Logic_Client_Request::process_10120090(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_first_recharge();
}

int Logic_Client_Request::process_10120091(Logic_Player &player, Block_Buffer &buf) {
	return player.req_receive_first_recharge();
}

//int Logic_Client_Request::process_10105001(Logic_Player &player, Block_Buffer &buf) {
//	return 0;
//}

int Logic_Client_Request::process_10120092(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_recharge_rebate();
}

int Logic_Client_Request::process_10120093(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120093 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_recharge_rebate(msg.id);
}

int Logic_Client_Request::process_10120094(Logic_Player &player, Block_Buffer &buf) {
	return player.req_recharge_rebate_status();
}

int Logic_Client_Request::process_10105002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_wear_fashion(msg.fashion_vec);
}
//
//int Logic_Client_Request::process_10105003(Logic_Player &player, Block_Buffer &buf) {
//	MSG_10105003 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.handle_merge_fashion(msg.fashion_id);
//}
//int Logic_Client_Request::process_10105004(Logic_Player &player, Block_Buffer &buf) {
//	MSG_10105004 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.handle_upgrade_fashion(msg.fashion_id, msg.lv, msg.item_vec);
//}
//int Logic_Client_Request::process_10105005(Logic_Player &player, Block_Buffer &buf) {
//	MSG_10105005 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.handle_fashion_inherit(msg.fashion_id, msg.target_id);
//}
//
//int Logic_Client_Request::process_10105006(Logic_Player &player, Block_Buffer &buf) {
//	MSG_10105006 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
//	return player.handle_draw_cards(msg.pos, msg.type);
//}
//
int Logic_Client_Request::process_10105007(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105007 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_change_fashion_show_state(msg.state);
}

int Logic_Client_Request::process_10105020(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105020 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_get_fashion_info();
}

int Logic_Client_Request::process_10105021(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105021 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_use_fashion_item(msg.item_idx_vec, msg.wear);
}

int Logic_Client_Request::process_10105022(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105022 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_fashion_part_upgrade(msg.part_id, msg.auto_buy);
}

int Logic_Client_Request::process_10105011(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_get_wing_item();
}
int Logic_Client_Request::process_10105012(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105012 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_active_wing();
}
int Logic_Client_Request::process_10105013(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105013 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_get_wing_info();
}
int Logic_Client_Request::process_10105014(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105014 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_upgrade_wing_once(msg.is_auto_buy);
}
int Logic_Client_Request::process_10105015(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105015 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_upgrade_wing_auto(msg.is_auto_buy);
}
int Logic_Client_Request::process_10105016(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105016 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_change_wing_show_state(msg.state);
}

int Logic_Client_Request::process_10105017(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105017 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_get_ret_reward(msg.stage);
}

int Logic_Client_Request::process_10105018(Logic_Player &player, Block_Buffer &buf) {
	MSG_10105018 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_active_wing_experience();
}

int Logic_Client_Request::process_10107000(Logic_Player &player, Block_Buffer &buf) {
	return player.req_auction_goods_list();
}

int Logic_Client_Request::process_10107001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10107001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_add_auction_goods(msg.item_id, msg.num, msg.continue_time, msg.auction_price, msg.ones_price, msg.open_ann);
}

int Logic_Client_Request::process_10107002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10107002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_under_auction_goods(msg.id);
}

int Logic_Client_Request::process_10107003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10107003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_ones_price_buy_auction_goods(msg.id);
}

int Logic_Client_Request::process_10107004(Logic_Player &player, Block_Buffer &buf) {
	MSG_10107004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_update_auction_goods(msg.id, msg.price);
}

int Logic_Client_Request::process_10107100(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_knight_trials_plan();
}

int Logic_Client_Request::process_10107101(Logic_Player &player, Block_Buffer &buf) {
	return player.req_knight_trials_enter_fighter();
}

int Logic_Client_Request::process_10107102(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_knight_trials_reward_box();
}

int Logic_Client_Request::process_10107103(Logic_Player &player, Block_Buffer &buf) {
	return player.req_receive_knight_trials_reward_box();
}

int Logic_Client_Request::process_10107104(Logic_Player &player, Block_Buffer &buf) {
	return player.req_up_hide_knight_trials_palyer();
}

int Logic_Client_Request::process_10101100(Logic_Player &player, Block_Buffer &buf) {
	return player.req_anci_artifact_list();
}

int Logic_Client_Request::process_10101101(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101101 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_anci_artifact_unban(msg.id);
}

int Logic_Client_Request::process_10101102(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101102 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_anci_artifact_active(msg.id);
}

int Logic_Client_Request::process_10101103(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101103 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_anci_artifact_box_get(msg.id);
}

int Logic_Client_Request::process_10101200(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101200 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_open_month_card_invest_plan();
}

int Logic_Client_Request::process_10101201(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101201 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_get_month_card_profit(msg.day);
}

int Logic_Client_Request::process_10101202(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101202 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_month_card_invest();
}

int Logic_Client_Request::process_10101203(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101203 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_open_upgrade_invest();
}

int Logic_Client_Request::process_10101204(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101204 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_get_upgrade_invest_profit(msg.level);
}

int Logic_Client_Request::process_10101205(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101205 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_upgrade_invest_money(msg.money, msg.type);
}

int Logic_Client_Request::process_10101206(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101206 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_get_all_upgrade_invest_profit();
}

int Logic_Client_Request::process_10101207(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101207 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_get_invest_plan_history();
}

int Logic_Client_Request::process_10101300(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101300 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_expedition_open_chest(msg);
}

int Logic_Client_Request::process_10101301(Logic_Player &player, Block_Buffer &buf) {
	MSG_10101301 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_expedition_chest_collect(msg);
}

int Logic_Client_Request::process_10108001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10108001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.get_thorlottery_info();
}

int Logic_Client_Request::process_10108002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10108002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_lottery(msg.times);
}

int Logic_Client_Request::process_10108003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10108003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_exchange_item(msg.item_index, msg.item_id);
}

int Logic_Client_Request::process_10110017(Logic_Player &player, Block_Buffer &buf) {
	MSG_10110017 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_set_forenotice_played_effect(msg.id);
}

int Logic_Client_Request::process_10100332(Logic_Player &player, Block_Buffer &buf) {
	MSG_10100332 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	Block_Buffer inner_buf;
	MSG_20100310 inner_msg;
	inner_msg.role_id = player.role_id();
	inner_msg.level = msg.level;
	MAKE_MSG_BUF(inner_msg, inner_buf);
	LOGIC_MONITOR->send_to_scene(TEAM_SPORTS_SCENE_ID, 0, inner_buf);

	return 0;
}

int Logic_Client_Request::process_10120096(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120096 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_send_challenge_to_gang_chat(msg.role_id, msg.role_name);
}

int Logic_Client_Request::process_10109002(Logic_Player &player, Block_Buffer &buf) {
	MSG_10109002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_embed_card(msg.from_index, msg.to_index, msg.loc, msg.card_id);
}

int Logic_Client_Request::process_10109003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10109003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	return player.handle_upgrade(msg.card_idx, msg.card_idx_vec);
}

int Logic_Client_Request::process_10109005(Logic_Player &player, Block_Buffer &buf) {
	MSG_10109005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	return player.handle_sell_card(msg.type, msg.card_index);
}

int Logic_Client_Request::process_10109006(Logic_Player &player, Block_Buffer &buf) {
	MSG_10109006 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_open_card_hole(msg.equip_index, msg.loc);
}

int Logic_Client_Request::process_10109007(Logic_Player &player, Block_Buffer &buf) {
	MSG_10109007 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_merge_new_card(msg.card_index, msg.card_cost);
}

int Logic_Client_Request::process_10109008(Logic_Player &player, Block_Buffer &buf) {

	MSG_10109008 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_get_vip_upgrade_times();
}

int Logic_Client_Request::process_10109009(Logic_Player &player, Block_Buffer &buf) {

	MSG_10109009 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_vip_upgrade_card(msg.card_idx);
}

int Logic_Client_Request::process_10109010(Logic_Player &player, Block_Buffer &buf) {

	MSG_10109010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_card_poten_upgrade(msg.card_index);
}

int Logic_Client_Request::process_10120097(Logic_Player &player, Block_Buffer &buf) {
	return player.req_questionnaire_info();
}

int Logic_Client_Request::process_10120098(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120098 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_submit_questionnaire(msg.qid, msg.ans);
}

int Logic_Client_Request::process_10120099(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_two_recharge();
}

int Logic_Client_Request::process_10120101(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_limit_time_recharge();
}

int Logic_Client_Request::process_10120102(Logic_Player &player, Block_Buffer &buf) {
	return player.req_receive_limit_time_recharge();
}

int Logic_Client_Request::process_10120110(Logic_Player &player, Block_Buffer &buf) {
	MSG_10120110 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_plot_all_add_one(msg);
}

int Logic_Client_Request::process_10120100(Logic_Player &player, Block_Buffer &buf) {
	return player.req_receive_two_recharge();
}

int Logic_Client_Request::process_10106900(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_activity_exchange_mall();
}

int Logic_Client_Request::process_10106901(Logic_Player &player, Block_Buffer &buf) {
	MSG_10106901 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_exchage_activity_exchange_goods(msg.id, msg.num);
}

int Logic_Client_Request::process_10120060(Logic_Player &player, Block_Buffer &buf) {
	return player.req_open_invite_reward_info();
}

int Logic_Client_Request::process_10120061(Logic_Player &player, Block_Buffer &buf) {
	//MSG_10120061 msg;
	//JUDGE_MSG_ERROR_RETURN(msg);
	return 0;//player.req_receive_invite_reward_info(msg.type);
}

int Logic_Client_Request::process_10112000(Logic_Player &player, Block_Buffer &buf) {
	MSG_10112000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_receive_37vip_level_reward(msg.id);
}

int Logic_Client_Request::process_10112001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10112001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_open_37vip_level_reward(msg.pt_vip);
}

//life_skill
int Logic_Client_Request::process_10116000(Logic_Player &player, Block_Buffer &buf) {

	return player.open_life_skill_panel();
}

int Logic_Client_Request::process_10116001(Logic_Player &player, Block_Buffer &buf) {
	MSG_10116001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.learn_life_skill(msg.type);
}

int Logic_Client_Request::process_10116002(Logic_Player &player, Block_Buffer &buf) {

	MSG_10116002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.life_skill_upgrade(msg.skill_id);
}

int Logic_Client_Request::process_10116003(Logic_Player &player, Block_Buffer &buf) {

	MSG_10116003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.spar_refine(msg.id_vec);
}

int Logic_Client_Request::process_10116004(Logic_Player &player, Block_Buffer &buf) {

	MSG_10116004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.card_make(msg.type);
}

int Logic_Client_Request::process_10116005(Logic_Player &player, Block_Buffer &buf) {

	MSG_10116005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.spar_smelt(msg.tab_id,msg.amont);
}

int Logic_Client_Request::process_10117000(Logic_Player &player, Block_Buffer &buf) {
//	MSG_10117000 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_oper_activity_content_info();
}

int Logic_Client_Request::process_10117001(Logic_Player &player, Block_Buffer &buf) {
		MSG_10117001 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return player.req_oper_activity_content_reward(msg.act_id, msg.rec_id);
}

int Logic_Client_Request::process_10117002(Logic_Player &player, Block_Buffer &buf) {
		return player.req_oper_activity_content_icon_num();
}

int Logic_Client_Request::process_10117003(Logic_Player &player, Block_Buffer &buf) {
		MSG_10117003 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return player.req_oper_activity_content_item(msg.act_id);
}

int Logic_Client_Request::process_10113001(Logic_Player &player, Block_Buffer &buf) {

	MSG_10113001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_get_pet_info();
}

int Logic_Client_Request::process_10113002(Logic_Player &player, Block_Buffer &buf) {

	MSG_10113002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_pet_upgrade_once(msg.auto_buy);
}

int Logic_Client_Request::process_10113003(Logic_Player &player, Block_Buffer &buf) {
	MSG_10113003 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_pet_upgrade_auto(msg.auto_buy);
}

int Logic_Client_Request::process_10113004(Logic_Player &player, Block_Buffer &buf) {
	MSG_10113004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_active_pet();
}

int Logic_Client_Request::process_10113005(Logic_Player &player, Block_Buffer &buf) {
	MSG_10113005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_pet_illusion(msg.illusion_id);
}

int Logic_Client_Request::process_10113006(Logic_Player &player, Block_Buffer &buf) {
	MSG_10113006 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_active_pet_illusion(msg.illusion_id);
}

int Logic_Client_Request::process_10113007(Logic_Player &player, Block_Buffer &buf) {
	MSG_10113007 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_get_pet_active_item();
}

int Logic_Client_Request::process_10113008(Logic_Player &player, Block_Buffer &buf) {
	MSG_10113008 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_pet_learn_skill_normal(msg.skill_id);
}

int Logic_Client_Request::process_10113009(Logic_Player &player, Block_Buffer &buf) {
	MSG_10113009 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_pet_learn_skill_advance(msg.skill_id);
}

int Logic_Client_Request::process_10113010(Logic_Player &player, Block_Buffer &buf) {
	MSG_10113010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_change_pet_state(msg.state);
}
