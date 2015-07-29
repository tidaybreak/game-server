/*
 * Robot_Monitor.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#include "Robot_Monitor.h"
#include "Pool_Manager.h"
#include "Config_Cache.h"
#include "Robot_Player.h"
#include "Daemon_Server.h"
#include "Global_Timer.h"
#include "Robot_Def.h"
#include "Config_Cache_Robot.h"
#include "Msg_Role_Scene_Struct.h"
#include "Msg_Fight_Struct.h"
#include "DB_Operator.h"
#include "Global_Robot_Scene_Manager.h"

Robot_Monitor::Robot_Monitor() : robot_login_idx_(0), cnt_try_create_role_online_(0) {
	// TODO Auto-generated constructor stub
	server_name.clear();
}

Robot_Monitor::~Robot_Monitor() {
	// TODO Auto-generated destructor stub
}

int Robot_Monitor::init(int monitor_index) {
	Server_Monitor::init(Server_Enum::MONITOR_ROBOT, monitor_index, &message_unit_);

	robot_login_idx_ = 0;

	return 0;
}

int Robot_Monitor::start(char *name) {
	//reset
	robot_player_cid_map_.clear();
	player_player_id_map_.clear();
	robot_loginoff_set_.clear();
	fetch_role_ing_set.clear();
	login_ing_set.clear();
	login_gate_ing_set.clear();
	logiccid_loop_map_.clear();

	cnt_try_create_role_online_ = 0;

	login_logic_tick_ = Time_Value::zero;

	MONGO_OPERATOR->robot_db_view(account_vec);

	Server_Monitor::start();
	server_name = name;
	message_unit_.thr_create();

	GLOBAL_ROBOT_SCENE_MANAGER->init_global_scene(monitor_idx_);

	Block_Buffer buf;
	buf.make_message(TIMER_LOGIN);
	buf.finish_message();
	GLOBAL_TIMER->register_loop_handler(buf, CONFIG_CACHE_ROBOT->robot_config().batch_login_interval, unit_);

	buf.reset();
	buf.make_message(TIMER_TICK);
	buf.finish_message();
	GLOBAL_TIMER->register_loop_handler(buf, Time_Value(0, 10000), unit_);


	return 0;
}

int Robot_Monitor::process_monitor_close(void) {
	Server_Monitor::process_monitor_close();

	if (player_player_id_map_.empty()) {
		DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_ROBOT);
	}

	return 0;
}

int Robot_Monitor::client_link_drop(const int cid) {
	Robot_Player *player = find_player_by_cid(cid);
	// 主动踢或顶号时发生
	if (! player) {
		MSG_DEBUG("may be repeat login cid:%d", cid);
		return 0;
	}

	// 底层已经回收cid,可能被后来链接用,这里要解绑定cid
	unbind_cid_player(cid);
//	unbind_rid_player(player->role_id());
	player->set_cid(0);

	//--cnt_try_create_role_online_;


	player->notice_logic_sign_out(0);

	return 0;
}

int Robot_Monitor::server_link_drop(const int cid) {

	return 0;
}

int Robot_Monitor::monitor_link_drop(const int cid) {
	Robot_Player *player = 0;
	robot_player_cid_map_.find(cid, player);
	if (player) {
		robot_loginoff_set_.insert(player->base_detail().account);
		MSG_DEBUG("sign out role:%ld", player->role_id());

		//logic_logining_account_cid_map_.unbind(player->base_detail().account);

		unbind_rid_player(player->role_id());
		unbind_cid_player(cid);

		//--cnt_try_create_role_online_;
	}
	return 0;
}

int Robot_Monitor::connect_to_gate(std::string &ip, int port) {
	int cid = service_connect_.connect(ip.c_str(), port);
	if (cid < 2) {
		LOG_USER("connect error");
		return -1;
	}

	return cid;
}

int Robot_Monitor::send_to_gate(int cid, Block_Buffer &buf) {
	service_connect_.push_data_block_with_len(cid, buf);

	return 0;
}

int Robot_Monitor::connect_to_logic(Account_Key &account) {
	int cid = service_connect_.connect(CONFIG_CACHE_ROBOT->robot_config().ip.c_str(), CONFIG_CACHE_ROBOT->robot_config().port);
	if (cid < 2) {
		LOG_USER("connect error");
		return -1;
	}
	logic_logining_account_cid_map_[account] = cid;		// login cid
	logic_logining_cid_account_map_[cid] = account;

	logiccid_loop_map_.insert(std::make_pair(account, 0));

	return 0;
}

int Robot_Monitor::create_send_to_logic(int cid, std::string &account, Block_Buffer &buf) {
/*
	int cid = 0;
//	if (!logic_logining_account_cid_map_.find(account, cid)) {
//		service_connect_.push_data_block_with_len(cid, buf);
//	} else {
		cid = service_connect_.connect(CONFIG_CACHE_ROBOT->robot_config().ip.c_str(), CONFIG_CACHE_ROBOT->robot_config().port);
		if (cid >= 2) {
			logic_logining_account_cid_map_[account] = cid;	// login cid
			logic_logining_cid_account_map_[cid] = account;

			service_connect_.push_data_block_with_len(cid, buf);
		} else {
			LOG_USER("connect error");
			return -1;
		}
//	}
*/
	if (cid >= 2) {
		service_connect_.push_data_block_with_len(cid, buf);
	} else {
		return -1;
	}

	return 0;
}

int Robot_Monitor::bind_rid_player(Robot_Player &player) {
	return player_player_id_map_.bind(player.role_id(), &player);
}

int Robot_Monitor::unbind_rid_player(role_id_t role_id) {
	Robot_Player_Role_Id_Map::iterator find_it = player_player_id_map_.find(role_id);
	if (find_it != player_player_id_map_.end()) {
		POOL_MANAGER->push_robot_player(find_it->second);
		player_player_id_map_.unbind(role_id);
	}

	return 0;
}

int Robot_Monitor::bind_cid_player(Robot_Player &player) {
	return robot_player_cid_map_.bind(player.cid(), &player);
}

int Robot_Monitor::unbind_cid_player(int cid) {
	return robot_player_cid_map_.unbind(cid);
}

int Robot_Monitor::set_fetch_role(Account_Key &account) {
	fetch_role_ing_set.insert(account);
	return 0;
}
int Robot_Monitor::unset_fetch_role(Account_Key &account) {
	fetch_role_ing_set.erase(account);
	return 0;
}
int Robot_Monitor::set_loging(std::string account) {
	login_ing_set.insert(account);
	return 0;
}
int Robot_Monitor::unset_loging(std::string account) {
	login_ing_set.erase(account);
	return 0;
}
int Robot_Monitor::set_loging_gate(std::string account) {
	login_gate_ing_set.insert(account);
	return 0;
}
int Robot_Monitor::unset_loging_gate(std::string account) {
	login_gate_ing_set.erase(account);
	return 0;
}

int Robot_Monitor::set_loginoff(std::string account) {
	robot_loginoff_set_.insert(account);
	return 0;
}

int Robot_Monitor::get_account_by_cid(int cid, Account_Key &account) {
	return logic_logining_cid_account_map_.find(cid, account);
}

int Robot_Monitor::get_cid_by_account(Account_Key &account) {
	int cid = 0;
	logic_logining_account_cid_map_.find(account, cid);
	return cid;
}

int Robot_Monitor::tick(void) {
	Time_Value now = Time_Value::gettimeofday();
	//rand_move(now);
	tick_doing(now);
	return 0;
}

int Robot_Monitor::turn_scenes(Time_Value &now, Robot_Player *player) {
	if(!player->wayfind_info().det_scene_id) return 0;
	//MSG_USER("%s >< 开始转场景 source:%d =======> to:%d", player->base_detail().account.c_str(), player->wayfind_info().cur_location.scene_id, player->wayfind_info().det_scene_id);
	player->base_detail().status = Robot_Detail::INNER_OPING;
	MSG_10400005 res_msg_;
	res_msg_.scene_id = player->wayfind_info().det_scene_id;
	Block_Buffer res_buf_;
	res_buf_.make_client_message(10400005, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	res_msg_.serialize(res_buf_);
	res_buf_.finish_message();
	player->send_to_server(res_buf_);
	//MSG_USER("%s >< 转场景end", player->base_detail().account.c_str());
	return 0;
}

int Robot_Monitor::do_wayfind(Time_Value &now, Robot_Player *player) {
	if(!player->wayfind_info().det_scene_id) {
		//player->wayfind_info().det_scene_id = 80100001;
		//player->base_detail().status = Robot_Detail::IDLE;
		//rand_move(player);
		return 0;
	}
	//MSG_USER("%s >< 开始寻路", player->base_detail().account.c_str());
	player->base_detail().status = Robot_Detail::INNER_OPING;
	player->path_find();
	player->base_detail().status = Robot_Detail::WAYFINDING;
	//MSG_USER("%s >< 寻路end", player->base_detail().account.c_str());
	return 0;
}

int Robot_Monitor::do_task(Time_Value &now, Robot_Player *player) {
	player->do_task(now);
	return 0;
}

int Robot_Monitor::run_way_move(Time_Value &now, Robot_Player *player) {
	Coord_Vec::reverse_iterator rit = player->wayfind_info().path.rbegin();
	if(rit != player->wayfind_info().path.rend()) {
		MSG_10200102 move_msg;
		move_msg.role_id = player->base_detail().role_id;
		move_msg.path.push_back(rit->pixel_coord());

		player->wayfind_info().cur_location.coord = rit->pixel_coord();
		player->wayfind_info().path.erase(rit.base());
		uint8_t move_skip_range = CONFIG_CACHE_ROBOT->robot_config().move_skip_range;
		while(player->wayfind_info().path.size() > 1 && move_skip_range > 0) {
			player->wayfind_info().path.erase(--player->wayfind_info().path.end());
			--move_skip_range;
		}
/**************************/ //比较坑;第一次需要发多一个格仔
		if(player->wayfind_info().first_move) {
			player->wayfind_info().first_move = false;
			for(int i=0; i < 2; ++i) {
				Coord_Vec::reverse_iterator rit_first = player->wayfind_info().path.rbegin();
				if(rit_first != player->wayfind_info().path.rend()) {
					move_msg.path.push_back(rit_first->pixel_coord());
					player->wayfind_info().cur_location.coord = rit_first->pixel_coord();
					player->wayfind_info().path.erase(rit_first.base());
					uint8_t frist_move_skip_range = CONFIG_CACHE_ROBOT->robot_config().move_skip_range;
					while(player->wayfind_info().path.size() > 1 && frist_move_skip_range > 0) {
						player->wayfind_info().path.erase(--player->wayfind_info().path.end());
						--frist_move_skip_range;
					}
				}
			}
		}
/******************************/
		Block_Buffer move_buf;
		move_buf.make_client_message(10200102, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		move_msg.serialize(move_buf);
		move_buf.finish_message();
		player->send_to_server(move_buf);
	} else {
		//MSG_USER("%s >< 移动end", player->base_detail().account.c_str());
		//player->base_detail().status = Robot_Detail::TURN_SCENES;
		player->base_detail().status = Robot_Detail::IDLE;
	}
	return 0;
}

int Robot_Monitor::tick_doing(Time_Value &now) {
	if (login_logic_tick_ == Time_Value::zero) {
		login_logic_tick_ = now;
	}
	Time_Value tv_diff = now - login_logic_tick_;
	if (tv_diff.sec() < 1 && tv_diff.usec() < 500000) {	//
		return 0;
	} else {
		login_logic_tick_ = now;
	}


	for(Robot_Player_Cid_Map::iterator iter = robot_player_cid_map_.begin();
			iter != robot_player_cid_map_.end(); ++iter) {
		iter->second->tick(now);
	}

	if (! logiccid_loop_map_.empty()) {
		//MSG_DEBUG("logiccid_loop_map_.size: %d", logiccid_loop_map_.size());
		for (boost::unordered_map<Account_Key, int>::iterator it_cid_loop = logiccid_loop_map_.begin(); it_cid_loop != logiccid_loop_map_.end();) {
			//MSG_DEBUG("   logiccid_loop_map_: cid:%d, loop:%d", it_cid_loop->first, it_cid_loop->second);
			if (it_cid_loop->second < LOOP_LOGIC_LINK) {
				++it_cid_loop->second;
				++it_cid_loop;

			} else {
				MSG_DEBUG("   创号超时，服务端返回超时，断开逻辑服连接");
				//drop_link_to_logic(it_cid_loop->first);
				Account_Key account = it_cid_loop->first;
				int logic_cid = get_cid_by_account(account);

				this->close_link_by_cid(logic_cid, Link_Close(10, 0));

				logic_logining_account_cid_map_.unbind(account);
				logic_logining_cid_account_map_.unbind(logic_cid);

				it_cid_loop = logiccid_loop_map_.erase(it_cid_loop);

				continue;
				// not to operate it_cid_loop because it_cid_loop has been erase in 'drop_link_to_logic(it_cid_loop->first)'
			}
		}
	}

	return 0;
}

int Robot_Monitor::rand_move(Time_Value &now) {
	for(Robot_Player_Cid_Map::iterator iter = robot_player_cid_map_.begin(); iter != robot_player_cid_map_.end(); ++iter) {
		Coord coord = iter->second->wayfind_info().cur_location.coord;
		MSG_10200102 move_msg;
		move_msg.role_id = iter->second->base_detail().role_id;
		move_msg.path.push_back(coord);
			switch(rand()%8) {
				case 0:{
					coord.x += 32*5;
					break;
				}
				case 1:{
					coord.x -= 32*5;
					break;
				}
				case 2:{
					coord.y += 32*(rand()%5);
					break;
				}
				case 3:{
					coord.y -= 32*(rand()%5);
					break;
				}
				case 4:{
					coord.x += 32*(rand()%5);
					coord.y += 32*(rand()%5);
					break;
				}
				case 5:{
					coord.x -= 32*(rand()%5);
					coord.y += 32*(rand()%5);
					break;
				}
				case 6:{
					coord.x -= 32*(rand()%5);
					coord.y -= 32*(rand()%5);
					break;
				}
				case 7:{
					coord.x += 32*(rand()%5);
					coord.y -= 32*(rand()%5);
					break;
				}
			}
		move_msg.path.push_back(coord);
		Block_Buffer move_buf;
		move_buf.make_client_message(10200102, iter->second->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		move_msg.serialize(move_buf);
		move_buf.finish_message();
		iter->second->send_to_server(move_buf);
		iter->second->wayfind_info().cur_location.coord = coord;
	}
	return 0;
}

int Robot_Monitor::rand_move(Robot_Player *player) {
	if (! player) {
		return -1;
	}
	//for(Robot_Player_Cid_Map::iterator iter = robot_player_cid_map_.begin(); iter != robot_player_cid_map_.end(); ++iter) {
		Coord coord = player->wayfind_info().cur_location.coord;
		MSG_10200102 move_msg;
		move_msg.role_id = player->base_detail().role_id;
		move_msg.path.push_back(coord);
			switch(rand()%8) {
				case 0:{
					coord.x += 32*5;
					break;
				}
				case 1:{
					coord.x -= 32*5;
					break;
				}
				case 2:{
					coord.y += 32*(rand()%5);
					break;
				}
				case 3:{
					coord.y -= 32*(rand()%5);
					break;
				}
				case 4:{
					coord.x += 32*(rand()%5);
					coord.y += 32*(rand()%5);
					break;
				}
				case 5:{
					coord.x -= 32*(rand()%5);
					coord.y += 32*(rand()%5);
					break;
				}
				case 6:{
					coord.x -= 32*(rand()%5);
					coord.y -= 32*(rand()%5);
					break;
				}
				case 7:{
					coord.x += 32*(rand()%5);
					coord.y -= 32*(rand()%5);
					break;
				}
			}
		move_msg.path.push_back(coord);
		Block_Buffer move_buf;
		move_buf.make_client_message(10200102, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		move_msg.serialize(move_buf);
		move_buf.finish_message();
		player->send_to_server(move_buf);
		player->wayfind_info().cur_location.coord = coord;
	//}
	return 0;
}

int Robot_Monitor::login_logic_server(void) {
//	MSG_USER("----------------------------");
//	MSG_USER("获取角色列表中的链接数:%d", fetch_role_ing_set.size());
//	MSG_USER("等待登陆返回的链接数:%d", login_ing_set.size());
//	MSG_USER("等待网关返回的链接数:%d", login_gate_ing_set.size());
//	MSG_USER("离线需要重新登陆的链接数:%d", robot_loginoff_set_.size());
//	MSG_USER("目前在线的链接数:%d", robot_player_cid_map_.size());
//	MSG_USER("目前要求登陆的链接数:%d", robot_login_idx_);
//	MSG_USER("----------------------------");

	//size_t online_player = robot_player_cid_map_.size();
	//int online_max = CONFIG_CACHE_ROBOT->robot_config().online_max;
	//MSG_DEBUG("try player: %d, online_player: %d, config online_max: %d", cnt_try_create_role_online_, online_player, online_max);
	if (robot_login_idx_ >= CONFIG_CACHE_ROBOT->robot_config().online_max) {
	//if (cnt_try_create_role_online_ >= (int) CONFIG_CACHE_ROBOT->robot_config().online_max) {
		//MSG_DEBUG("login full");
		return 0;
	}

	Account_Key account;
	if (account_vec.empty()) {
		std::stringstream account_stream;
		account_stream << server_name;
		account_stream << CONFIG_CACHE_ROBOT->robot_config().name_prefix;
		account_stream << robot_login_idx_;
		account_stream << "_";
		account_stream << Time_Value::gettimeofday().sec();
		account_stream << "_";
		account_stream << Time_Value::gettimeofday().usec();
		account.account_name = account_stream.str();
		account.agent_num = CONFIG_CACHE->server_flag()/10000;
		account.server_num = CONFIG_CACHE->server_flag() - account.agent_num * 10000;
	} else {
		account = account_vec.front();
		account_vec.erase(account_vec.begin());
	}

	// connect_to_logic
	if (0 != connect_to_logic(account)) {
		return -1;
	}

	int login_logic_cid = get_cid_by_account(account);
	if (! login_logic_cid) {
		return -1;
	}

	Block_Buffer buf;
	MSG_10100000 msg;
	msg.account = account.account_name;
	msg.agent_num = account.agent_num;
	msg.server_num = account.server_num;
	buf.make_client_message(10100000, 0, 0);
	msg.serialize(buf);
	buf.finish_message();
	int ret = this->create_send_to_logic(login_logic_cid, account.account_name, buf);
	if (! ret) {
		robot_login_idx_++;
		set_fetch_role(account);

	}

	return 0;
}

int Robot_Monitor::drop_link_to_logic(Account_Key &account) {
	int logic_cid = get_cid_by_account(account);

	this->close_link_by_cid(logic_cid, Link_Close(81000000, 0));

	logic_logining_account_cid_map_.unbind(account);
	logic_logining_cid_account_map_.unbind(logic_cid);

	logiccid_loop_map_.erase(account);

	return 0;
}

int Robot_Monitor::drop_link_to_logic(const int logic_cid) {
	this->close_link_by_cid(logic_cid, Link_Close(0, 0));

	Account_Key account;
	get_account_by_cid(logic_cid, account);

	logic_logining_account_cid_map_.unbind(account);
	logic_logining_cid_account_map_.unbind(logic_cid);

	logiccid_loop_map_.erase(account);

	return 0;
}
