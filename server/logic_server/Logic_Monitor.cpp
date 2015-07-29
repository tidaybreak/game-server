/*
 * Logic_Monitor.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#include "Logic_Monitor.h"
#include "Pool_Manager.h"
#include "Config_Cache.h"
#include "Config_Cache_Scene.h"
#include "DB_Manager.h"
#include "Game_Manager.h"
#include "Utility_Func.h"
#include "Logic_Player.h"
#include "Logic_Player_Struct.h"
#include "Daemon_Server.h"
#include "Logic_Public.h"
#include "mall/Mall_Manager.h"
#include "team/Team_Manager.h"
#include "gang/Gang_Manager.h"
#include "mail/Mail_Manager.h"
#include "arena/Arena_Manager.h"
#include "ranking/Ranking_Manager.h"
#include "active/Active_Manager.h"
#include "warer/War_Scene_Active_Manager.h"
#include "title/Title_Manager.h"
#include "db_mysql/Mysql_Operator.h"
#include "secret_shop/Secret_Shop_Manager.h"
#include "helper/Helper_Manager.h"
#include "dragon/Dragon_Vale_Manager.h"
#include "active/Active_Content_Manager.h"
#include "world_boss/World_Boss_Active_Manager.h"
#include "honor_arena/Honor_Arena_Active_Manager.h"
#include "valentine_monster/Valentine_Monster_Active_Manager.h"
#include "campfire/Campfire_Manager.h"
#include "reward/Reward_Manager.h"
#include "auction/Auction_Manager.h"
#include "invest_plan/Invest_Plan_Manager.h"
#include "thorlottery/ThorLottery_Manager.h"
#include "knight_trials/Knight_Trials_Manager.h"
#include "DB_Operator.h"
#include "answer/Answer_Manager.h"
#include "thorlottery/ThorLottery_Manager.h"
#include "recharge/Two_Recharge_Manager.h"
#include "expedition/Expedition_Manager.h"
#include "activities_stores/Activities_Stores_Manager.h"


Logic_Monitor::Logic_Monitor() :
db_view_(0),
verify_pack_onoff_(true) {
	// TODO Auto-generated constructor stub
}

Logic_Monitor::~Logic_Monitor() {
	// TODO Auto-generated destructor stub
}

int Logic_Monitor::init(int monitor_index) {
	Server_Monitor::init(Server_Enum::MONITOR_LOGIC, monitor_index, &message_unit_);

	Monitor_Mark_Vec muid;
	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
	server_solution_cache.fetch_muid_by_type(muid, Server_Enum::MONITOR_GATE);
	for (Monitor_Mark_Vec::iterator it = muid.begin(); it != muid.end(); ++it) {
		gate_amount_map_.rebind((*it), 0);
	}

	muid.clear();
	Scene_Unique_ID scene_uid;
	server_solution_cache.fetch_muid_by_type(muid, Server_Enum::MONITOR_SCENE);
	for (Monitor_Mark_Vec::iterator it = muid.begin(); it != muid.end(); ++it) {
		scene_uid.scene_id = (*it).id;
		scene_uid.scene_order = (*it).order;
		if (scene_muid_map_.bind(scene_uid, (*it))) {
			LOG_ABORT("scene repeat, scene_id:%d, scene_order:%d", (*it).id, (*it).order);
		}
	}
	for (Scene_Muid_Map::iterator it = scene_muid_map_.begin(); it != scene_muid_map_.end(); ++it) {
		int idx = server_solution_cache.fetch_scene_process_idx(it->second.id, it->second.order);
		if (idx >= 0) {
			scene_process_idx_.bind(it->first, idx);
		}
	}

	for (Server_Monitor_Vec::const_iterator monitor_it = server_solution_cache.monitor_list.begin(); monitor_it != server_solution_cache.monitor_list.end(); ++monitor_it) {
		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if (Server_Enum::MONITOR_SCENE == (*accept_it).monitor_type) {
				if ((*monitor_it).run_on < 8000) {
					for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
						muid_link_time_.bind(*it_order_it, Time_Value::gettimeofday());
						break;
					}
				} else if ((*monitor_it).run_on < 9000) {
					for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
						const Scene_Config *config = CONFIG_CACHE_SCENE->scene_config_cache((*it_order_it).id);
						if (config->inter_order == (*it_order_it).order) {
							muid_link_time_.bind(*it_order_it, Time_Value::gettimeofday());
							break;
						}
					}
				}
			} else if (Server_Enum::MONITOR_GATE == (*accept_it).monitor_type) {
				for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
					muid_link_time_.bind(*it_order_it, Time_Value(0));
					break;
				}
			}
		}
	}

	LOGIC_PUBLIC->init();

	//公司出口IP
	company_ip_set_.clear();
	company_ip_set_.insert("183.6.143.154");

	return 0;
}

int Logic_Monitor::start(void) {
	Server_Monitor::start();

	// 记录刷新时间
	Time_Value now = Time_Value::gettimeofday();
	Time_Value timeout;
	timeout = next_relative_time(00, 00, 00);
	trigger_daily_zero_ = now + timeout - Time_Value(Time_Value::ONE_DAY_IN_SECS);
	timeout = next_relative_time(6, 00, 00);
	trigger_daily_six_ = now + timeout - Time_Value(Time_Value::ONE_DAY_IN_SECS);
	timeout = next_relative_time(00, 00, 00, 1);
	trigger_weekly_zero_ = now + timeout - Time_Value(Time_Value::ONE_WEEK_IN_SECS);

	refresh_hi_power_ = now + Time_Value(Time_Value::ONE_HOUR_IN_SECS);

	MONGO_OPERATOR->reset_online_state();

	// 公会模块(放player_db_view之前)
	GANG_MANAGER->start();

	// 加载玩家信息
	DB_MANAGER->create_db_view(db_view_);

	// 后台GM模块
	GM_MANAGER->start();

	// 模块manag
	LOGIC_PUBLIC->start();

	// 商城模块
	MALL_MANAGER->start();

	// 系统邮件模块
	MAIL_MANAGER->start();

	// arena
	ARENA_MANAGER->start();

	//排行榜
	RANKING_MANAGER->start();

	//组队竞技
	TEAM_MANAGER->start();

	// 战场
	WAR_SCENE_ACTIVE_MANAGER->start();
	//
	WORLD_BOSS_ACTIVE_MANAGER->start();
	//
	VALENTINE_MONSTER_ACTIVE_MANAGER->start();

	// 称号
	TITLE_MANAGER->start();

	// 神秘商店
	SECRET_SHOP_MANAGER->start();

	//TOD助手
	HELPER_MANAGER->start();

	//活动
	ACTIVE_MANAGER->start();
	ACTIVE_CONTENT_MANAGER->start();

	// 篝火晚会
	CAMPFIRE_MANAGER->start();

	//奖励系统
	REWARD_MANAGER->start();

	//拍卖行
	AUCTION_MANAGER->start();

	//投资计划
	INVEST_PLAN_MANAGER->start();

	THORLOTTERY_MANAGER->start();

	ANSWER_MANAGER->start();

	EXPEDITION_MANAGER->start();

	ACTIVITIES_STORES_MANAGER->start();

	handle_hi_power();
	message_unit_.thr_create();

	thr_create();

	return 0;
}

void Logic_Monitor::run_handler(void) {
	Monitor_Mark_Vec muid_vec;
	Monitor_Link_Time muid_time;
	for (Monitor_Link_Time::iterator it = muid_link_time_.begin(); it != muid_link_time_.end(); ++it) {
		muid_vec.push_back(it->first);
		muid_time.bind(it->first, Time_Value::zero);
	}

	for (Monitor_Mark_Vec::iterator it = muid_vec.begin(); it != muid_vec.end(); ++it) {
		if ((*it).type == MONITOR_SCENE) {
			const Scene_Config *config = CONFIG_CACHE_SCENE->scene_config_cache((*it).id);
			if (config && config->inter_order >= 0) {
				// 保存除跨服场景外的muid
				muid_link_time_.unbind(*it);

				// 对跨服场景只在大于开服时间才保持心跳
				muid_time.rebind(*it, CONFIG_CACHE->server_maintainer_cache().game_open_time);
			}
		}
	}


	Time_Value inner_heartbeat = Time_Value::gettimeofday() + Time_Value(5);
	Time_Value send_center_sleep = Time_Value::gettimeofday() + Time_Value(5);
	while(true) {
		sleep(1);
		Time_Value now = Time_Value::gettimeofday();

		// server间内部心跳
		if (inner_heartbeat < now) {
			for (Monitor_Link_Time::iterator it = muid_time.begin(); it != muid_time.end(); ++it) {
				if (now < it->second) continue;

				Block_Buffer inner_buf;
				inner_buf.make_message(SERVER_INNER_HEARTBEAT);
				(it->first).serialize(inner_buf);
				Time_Value tick_time = Time_Value::gettimeofday();
				inner_buf.write_int32(tick_time.sec());
				inner_buf.write_int32(tick_time.usec());
				inner_buf.finish_message();
				if (send_to_monitor(it->first, inner_buf)) {
					if (muid_link_time_.exist(it->first)) {
						muid_link_timeout_.rebind(it->first, Time_Value(INT_MAX, 0));
					}
				}
			}

			{
				if (DAEMON_SERVER_INSTANCE->c_ip_ != "127.0.0.1" && DAEMON_SERVER_INSTANCE->c_port_) {
					Block_Buffer inner_buf;
					inner_buf.make_message(20999999);
					inner_buf.write_uint8(2);

					inner_buf.write_uint8(1);
					inner_buf.write_string(CONFIG_CACHE->server_maintainer_cache().game_server_flag + ":" + DAEMON_SERVER_INSTANCE->path_name_);

					inner_buf.write_uint8(7);
					inner_buf.write_int32(player_name_map_.size());

					inner_buf.write_uint8(18);
					inner_buf.write_uint32(DB_MANAGER->db_role_size());

					inner_buf.write_uint8(20);
					inner_buf.write_int32(highest_power_.read_value());

					inner_buf.write_uint8(21);
					inner_buf.write_int32(highest_power_avg_.read_value());

					inner_buf.finish_message();
					this->send_to_scene(80000000, 0, inner_buf);
				}
			}

			inner_heartbeat = now + CONFIG_CACHE->server_maintainer_cache().inner_tick_time_;
		}
	}
}

void Logic_Monitor::tick(Time_Value &now) {
	// 正在退出的玩家，core之类会影响下线流程，这里做下预防
	login_out_timeout(now);

	// 单个玩家事件
	process_player_event();

	MALL_MANAGER->tick(now);
	TEAM_MANAGER->tick(now);
	GANG_MANAGER->tick(now);
	ARENA_MANAGER->tick(now);
	RANKING_MANAGER->tick(now);
	ACTIVE_MANAGER->tick(now);
	WAR_SCENE_ACTIVE_MANAGER->tick(now);
	WORLD_BOSS_ACTIVE_MANAGER->tick(now);
	HONOR_ARENA_ACTIVE_MANAGER->tick(now);
	// 神秘商店
	SECRET_SHOP_MANAGER->tick(now);
	//TOD助手
	HELPER_MANAGER->tick(now);
	DRAGON_VALE_MANAGER->tick(now);
	// 篝火晚会
	CAMPFIRE_MANAGER->tick(now);
	//Public
	LOGIC_PUBLIC->tick(now);
	//奖励系统
	REWARD_MANAGER->tick(now);
	//活动
	ACTIVE_CONTENT_MANAGER->tick();
	TWO_RECHARGE_MANAGER->tick(now);
	//拍卖行
	AUCTION_MANAGER->tick(now);
	THORLOTTERY_MANAGER->tick(now);
	KNIGHT_TRIALS_MANAGER->tick(now);

	// 统计服务器最高战力
	power_tick(now);
}

void Logic_Monitor::power_tick(Time_Value &now) {
	if ( now > refresh_hi_power_ ) {
		refresh_hi_power_ = now + Time_Value(Time_Value::ONE_HOUR_IN_SECS);
		handle_hi_power_refresh();
	}
}

int Logic_Monitor::server_monitor_status(void) {
	LOG_TEXT("logic player amount:%d", player_role_id_map_.size());
	return 0;
}

int Logic_Monitor::process_monitor_close(void) {
	Server_Monitor::process_monitor_close();

	LOG_DEBUG("process_monitor_close player:%d", player_role_id_map_.size());

	if (player_role_id_map_.empty()) {
		DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_LOGIC);
	} else {
		// 存组队竞技数据
		TEAM_MANAGER->save_detail();
		// 公会数据保存
		GANG_MANAGER->save();

		for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
			it->second->sign_out(10000125);
		}

	}

	return 0;
}

int Logic_Monitor::send_to_all_monitor(Block_Buffer &buf) {
	Monitor_Mark_Vec list;
	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();

	server_solution_cache.fetch_muid_single_monitor(list, Server_Enum::MONITOR_ALL);
	for (Monitor_Mark_Vec::iterator it = list.begin(); it != list.end(); ++it) {
		if ((*it).type != Server_Enum::MONITOR_TYPE::MONITOR_CHAT
				&& (*it).type != Server_Enum::MONITOR_TYPE::MONITOR_RECORD)
			send_to_monitor(*it, buf);
	}

	return 0;
}

int Logic_Monitor::send_to_client(int cid, Block_Buffer &buf) {
	return send_to_client_by_cid(cid, buf);
}

int Logic_Monitor::send_to_all_client(Block_Buffer &buf) {
	for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->send_to_client(buf);
	}
	return 0;
}

int Logic_Monitor::send_to_gate(Monitor_Unique_ID &idx, Block_Buffer &buf) {
	return send_to_monitor(MONITOR_GATE, idx.id, idx.order, buf, idx.uuid);
}

int Logic_Monitor::send_to_scene(int scene_id, int scene_order, Block_Buffer &buf) {
	const Scene_Config *des_scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if (!des_scene_config) {
		MSG_USER("scene:%d, order:%d", scene_id, scene_order);
		return -1;
	}
	// 如果是跨服场景就强制修改order
	scene_order = des_scene_config->inter_order >= 0 ? des_scene_config->inter_order : scene_order;

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), 0);
		ret = send_to_monitor(MONITOR_SCENE, scene_id, scene_order, buf, 0);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), 0);
		ret = send_to_monitor(MONITOR_SCENE, scene_id, scene_order, tmp_buf, 0);
	}

	return ret;
}

int Logic_Monitor::send_to_local_chat(Block_Buffer &buf) {
	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), 0);
		ret = send_to_monitor(MONITOR_CHAT, 0, 0, buf, 0);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), 0);
		ret = send_to_monitor(MONITOR_CHAT, 0, 0, tmp_buf, 0);
	}

	return ret;
}

int Logic_Monitor::send_error_to_client(int cid, int error_id, int source) {
	Block_Buffer buf;
	buf.make_message(80000001);
	buf.write_int32(error_id);
	buf.write_int32(source);
	buf.write_string("");
	buf.finish_message();
	return send_to_client_by_cid(cid, buf);
}

bool Logic_Monitor::get_validate_account(std::string &ip) {
	if (company_ip_set_.count(ip))
		return 0;
	else
		return !CONFIG_CACHE->server_maintainer_cache().md5_key.empty();
}

int Logic_Monitor::bind_logic_player(Logic_Player &player) {
	Gate_Amount_Map::iterator gate_find_it = gate_amount_map_.find(player.monitor_link().gate_muid);
	if (gate_find_it != gate_amount_map_.end()) {
		gate_find_it->second++;
	}

	player_role_id_map_.bind(player.role_id(), &player);
	player_name_map_.bind(player.role_name(), &player);
	return 0;
}

int Logic_Monitor::unbind_logic_player(role_id_t role_id) {
	{
		Logic_Player_Role_Id_Map::iterator find_it = player_role_id_map_.find(role_id);
		if (find_it != player_role_id_map_.end()) {
			Gate_Amount_Map::iterator gate_find_it = gate_amount_map_.find(find_it->second->monitor_link().gate_muid);
			if (gate_find_it != gate_amount_map_.end()) {
				gate_find_it->second--;
			}

			POOL_MANAGER->push_logic_player(find_it->second);
			player_role_id_map_.unbind(role_id);
		}
		player_role_id_map_.unbind(role_id);
	}

	{
		DB_View::ID_Player_View_Map::iterator find_it = db_view()->id_player_view_map.find(role_id);
		if (find_it != db_view()->id_player_view_map.end()) {
			player_name_map_.unbind(find_it->second->role_name);
		}
	}

	{
		if (status_ == MONITOR_STATUS_CLOSING) {
			LOG_DEBUG("process_monitor_close player:%d, role id:%ld", player_role_id_map_.size(), role_id);
		}

		if (status_ == MONITOR_STATUS_CLOSING && player_role_id_map_.empty()) {
			// 等待数据库操作完成
			for (int i =0; i < 10; ++i) {
				if (DB_MANAGER->db_worker_block_empty())
					break;

				LOG_DEBUG("wait db work, times:%d", i);
				Time_Value::sleep(Time_Value(3));
			}
			MONGO_OPERATOR->reset_online_state();
			DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_LOGIC);
		}
	}

	return 0;
}

int Logic_Monitor::scene_order_free(const Monitor_Unique_ID &scene) {
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene.id);
	if (!scene_config->role_limit)
		return scene.order;

	int scene_order = scene.order;
	Scene_Amount_Map::iterator find_it = scene_amount_map_.find(scene);
	if (find_it != scene_amount_map_.end()) {
 		if (scene_config->role_limit <= find_it->second) {
 			int min_amount = INT_MAX;
 			Monitor_Unique_ID des_scene = scene;
			for (int i = 0; i < scene_config->order_total; ++i) {
				des_scene.order = i;
				Scene_Amount_Map::iterator order_find_it = scene_amount_map_.find(des_scene);

				if (order_find_it == scene_amount_map_.end() || scene_config->role_limit > order_find_it->second) {
					return i;
				}

				if(order_find_it != scene_amount_map_.end()) {
					if (min_amount > order_find_it->second) {
						min_amount = order_find_it->second;
						scene_order = i;
					}
				}
			}
		}
	}

	return scene_order;
}

bool Logic_Monitor::scene_role_limit(const Monitor_Unique_ID &scene) {
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene.id);
	if (!scene_config->role_limit)
		return false;

	Scene_Amount_Map::iterator find_it = scene_amount_map_.find(scene);
	if (find_it != scene_amount_map_.end()) {
 		if (scene_config->role_limit <= find_it->second) {
 			Monitor_Unique_ID des_scene = scene;
			for (int i = 0; i < scene_config->order_total; ++i) {
				des_scene.order = i;
				Scene_Amount_Map::iterator order_find_it = scene_amount_map_.find(des_scene);
				if (order_find_it == scene_amount_map_.end() || scene_config->role_limit >= order_find_it->second) {
					return true;
				}
			}
		}
	}

	return false;
}

int Logic_Monitor::scene_change_info(const Monitor_Unique_ID &scene, int type) {
	Scene_Amount_Map::iterator find_it = scene_amount_map_.find(scene);
	if (find_it == scene_amount_map_.end()) {
		scene_amount_map_.bind(scene, 1);
	} else {
		if (type)
			find_it->second++;
		else
			find_it->second--;
	}
	return 0;
}

int Logic_Monitor::scene_change_info(const Monitor_Unique_ID &src, const Monitor_Unique_ID &des) {
	Scene_Amount_Map::iterator find_it = scene_amount_map_.find(src);
	if (find_it == scene_amount_map_.end()) {
		MSG_USER("src scene id:%d, order:%d". src.id, src.order);
		scene_amount_map_.bind(src, 0);
	} else {
		find_it->second--;
	}

	find_it = scene_amount_map_.find(des);
	if (find_it == scene_amount_map_.end()) {
		scene_amount_map_.bind(des, 1);
	} else {
		find_it->second++;
	}
	return 0;
}

int Logic_Monitor::client_link_drop(const int cid) {
	Int_Uid::iterator find_it = db_view_->login_cid_role_id_map.find(cid);
	if (find_it != db_view_->login_cid_role_id_map.end()) {
		Player_DB_View *player_db_view = 0;
		DB_View::ID_Player_View_Map::iterator player_db_find_it = db_view_->id_player_view_map.find(find_it->second);
		if (player_db_find_it != db_view_->id_player_view_map.end()) {
			player_db_view = player_db_find_it->second;
			if (Player_DB_View::ACCOUNT_DOING_LOGIN & player_db_view->account_mode) {
				// 登录过程中断线
				player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGIN);
				player_db_view->set_account_status(Player_DB_View::ACCOUNT_DOING_LOGOUT, cid);
				db_view_->login_cid_role_id_map.erase(find_it);
				MSG_USER("login break, link close cid:%d role:%ld", cid, player_db_view->role_id);
			}
		} else {
			MSG_USER("cid:%d role_id:%ld", cid, find_it->second);
		}
	} else {
		MSG_DEBUG("cid:%d no player object", cid);
	}

	return 0;
}

int Logic_Monitor::monitor_link_drop(const int cid) {
	Monitor_Mark_Vec muid_list;
	get_muid_connect_by_cid(cid, muid_list);
	LOG_DEBUG("inner link drop, cid:%d, muid size:%d", cid, muid_list.size());
	for (Monitor_Mark_Vec::iterator m_it = muid_list.begin(); m_it != muid_list.end(); ++m_it) {
		process_muid_drop(*m_it);
	}

	if (!muid_list.empty())
		Server_Monitor::monitor_link_drop(cid);

	return 0;
}

int Logic_Monitor::process_muid_drop(Monitor_Unique_ID &muid) {
	LOG_DEBUG("muid drop uuid:%ld type:%d, id:%d order:%d player size:%d", muid.uuid, muid.type, muid.id, muid.order, player_role_id_map_.size());

	for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		if (muid.type == MONITOR_SCENE) {
			if (it->second->monitor_link().scene_muid == muid) {
				abnormal_logout(it->second, 0);
			}
		} else if (muid.type == MONITOR_GATE) {
			if (it->second->monitor_link().gate_muid == muid) {
				abnormal_logout(it->second, 0);
			}
		}
	}

	return 0;
}

int Logic_Monitor::abnormal_logout(Logic_Player *player, int close_reason) {
	LOG_DEBUG("core sign out, role:%ld, close_reason:%d", player->role_id(), close_reason);

	db_view()->abnormal_account_add(player->role_id(), 0);

	return 0;
}

int Logic_Monitor::server_muid_build(Monitor_Unique_ID &muid, int cid/* = -1*/, int service_type/* = 0*/){
	int ret = Server_Monitor::server_muid_build(muid, cid, service_type);
	// 连接到跨服时，同步竞技场数据到跨服,有待优化
	int inner_scene_id = 80207001;
	if(cid > 2 && muid.type == Server_Enum::MONITOR_SCENE && muid.id == inner_scene_id){
		ARENA_MANAGER->set_sync_to_inner_scene_flag(true);
	}

	const int dragon_vale_scene_id = 80212001;
	if(cid > 2 && muid.type == Server_Enum::MONITOR_SCENE && muid.id == dragon_vale_scene_id){
		DRAGON_VALE_MANAGER->process_clear_battle_role_id_set();
	}

	return ret;
}

int Logic_Monitor::select_gate_link_info(Monitor_Unique_ID &gate_muid) {
	int player_amount = INT_MAX;
	if (gate_amount_map_.empty()) {
		LOG_TRACE_ABORT("error");
	}
	for (Gate_Amount_Map::iterator it = gate_amount_map_.begin(); it != gate_amount_map_.end(); ++it) {
		if (it->second < player_amount) {
			gate_muid = it->first;
			player_amount = it->second;
		}
	}

	return 0;
}

int Logic_Monitor::fetch_gate_link_info(int cid, Monitor_Unique_ID &gate_muid, std::string &ip, int &port) {
	const Monitor_Link_Map &monitor_link_cache = CONFIG_CACHE->monitor_link_cache();

	Monitor_Link_Map::const_iterator find_it = monitor_link_cache.find(gate_muid);
	if (find_it != monitor_link_cache.end()) {
		std::string local_ip;
		get_client_local_addr(cid, local_ip);
		if (local_ip == find_it->second.telecom_ip) {
			ip = find_it->second.telecom_ip;
		} else if (local_ip == find_it->second.unicom_ip) {
			ip = find_it->second.unicom_ip;
		} else {
			ip = find_it->second.telecom_ip;
		}
		if (ip == "127.0.0.1") {
			ip = local_ip;
		}
		port = find_it->second.client_port;
	}

	return 0;
}

int Logic_Monitor::find_scene_muid(int scene_id, int scene_order, Monitor_Unique_ID &muid) {
	Scene_Unique_ID suid(scene_id, scene_order);
	return scene_muid_map_.find(suid, muid);
}

int Logic_Monitor::find_scene_process_idx(int scene_id, int scene_order) {
	int idx = -1;
	Scene_Unique_ID suid(scene_id, scene_order);
	scene_process_idx_.find(suid, idx);

	return idx;
}

role_id_t Logic_Monitor::get_role_id_by_name(std::string &role_name){
	DB_View::Name_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->name_player_view_map.find(role_name);
	if (iter == LOGIC_MONITOR->db_view()->name_player_view_map.end()) {
		MSG_USER("player(role_name=%s) is not in db_view", role_name.c_str());
		return 0;
	}
	return iter->second->role_id;
}

std::string Logic_Monitor::get_role_name_by_id(role_id_t role_id){
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		MSG_USER("player(role_id=%ld) is not in db_view", role_id);
		return "";
	}
	return iter->second->role_name;
}

Player_DB_View *Logic_Monitor::find_player_db_view_by_role_id(role_id_t role_id) {
	if (!db_view_) return 0;

	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		MSG_USER("player(role_id=%ld) is not in db_view", role_id);
		return NULL;
	}
	return iter->second;
}

Player_DB_View *Logic_Monitor::find_player_db_view_by_name(std::string &name) {
	DB_View::Name_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->name_player_view_map.find(name);
	if (iter == LOGIC_MONITOR->db_view()->name_player_view_map.end()) {
		MSG_USER("player(role_name=%s) is not in db_view", name.c_str());
		return NULL;
	}
	return iter->second;
}

bool Logic_Monitor::server_tick_check(void) {
	if (muid_link_timeout_.empty()) {
		return false;
	}

	for (Monitor_Link_Time::iterator it = muid_link_timeout_.begin(); it != muid_link_timeout_.end(); ++it) {
		MSG_DEBUG("server link delay mudi:%d  %d  %d, time:[%ld, %ld]", it->first.type, it->first.id, it->first.order, it->second.sec(), it->second.usec());
	}

	return true;
}

void Logic_Monitor::update_inner_tick(Monitor_Unique_ID &muid, Time_Value &time) {
	if (time > CONFIG_CACHE->server_maintainer_cache().inner_time_out_) {
		MSG_DEBUG("bind server link delay mudi:%d  %d  %d, time:[%ld, %ld]", muid.type, muid.id, muid.order, time.sec(), time.usec());
		muid_link_timeout_.rebind(muid, time);
	} else {
		if (muid_link_timeout_.exist(muid)) {
			MSG_DEBUG("unbind server link delay mudi:%d  %d  %d, time:[%ld, %ld]", muid.type, muid.id, muid.order, time.sec(), time.usec());
			muid_link_timeout_.unbind(muid);
		}
	}
}

int Logic_Monitor::login_out_timeout(Time_Value &now) {
	for (Role_Id_Time_Map::iterator it = db_view_->abnormal_account.begin(); it != db_view_->abnormal_account.end();) {
		if (now > it->second) {
			role_id_t role_id = it->first;
			db_view()->abnormal_account.erase(it++);
			Logic_Player *player = this->find_logic_player_by_role_id(role_id);
			if (!player) {
				MSG_USER("can not find player, role:%ld", role_id);
				continue;
			}
			Player_DB_View *player_db_view = 0;
			DB_View::ID_Player_View_Map::iterator find_it = db_view()->id_player_view_map.find(role_id);
			if (find_it == db_view()->id_player_view_map.end()) {
				MSG_USER("can not find db_view player:%ld", role_id);
				break;
			}
			find_it->second->sign_out_flag_ = Time_Value::gettimeofday();

			player->set_logout_time(Time_Value::gettimeofday());
			player->set_online_flag(0);
			player->save_player_data();

			Block_Buffer inner_buf;
			inner_buf.make_message(LOGIC_SCENE_LOGOUT_READY);
			find_it->second->sign_out_flag_.serialize(inner_buf);
			inner_buf.write_int32(0);
			inner_buf.write_int32(0);
			inner_buf.finish_message();
			player->send_to_scene(inner_buf);

			inner_buf.reset();
			inner_buf.make_message(LOGIC_GATE_LOGOUT_READY);
			inner_buf.write_int64(role_id);
			inner_buf.write_int32(0);
			inner_buf.write_int32(0);
			inner_buf.finish_message();
			player->send_to_gate(inner_buf);

			inner_buf.reset();
			inner_buf.make_message(LOGIC_CHAT_LOGOUT_READY);
			inner_buf.finish_message();
			player->send_to_local_chat(inner_buf);

			player_db_view = find_it->second;

			unbind_logic_player(role_id);

			int cid = player_db_view->mode_value;
			player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGIN);
			player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGOUT);

			if (cid) {
				db_view()->login_cid_role_id_map.erase(cid);
				close_client_by_cid(cid, Time_Value(2), 10000040);
			}

			MSG_USER("login out timeout, role:%ld, cid:%d", role_id, cid);
		} else {
			it++;
		}
	}

	return 0;
}

int Logic_Monitor::process_player_event(void) {
	for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->process_event();

		//监听限时任务
		Time_Value now = Time_Value::gettimeofday();
		it->second->task_listen_limit_time(now);

		//监听悬赏榜倒计时自动刷新
		it->second->listen_auto_refresh_reward_task_list(now);

		//精灵盟约实时监听过期
		it->second->listen_covenant_expired(now);

		//VIP监听是否已经过期
		it->second->listen_vip_expired(now);

		//监听龙谷建筑资源产出
		it->second->listen_building_output_production_by_one_minute(now);

		//监听龙谷建筑建造或者升级完成
		it->second->listen_building_finish(now);

		//监听龙蛋孵化完成
		it->second->listen_dragon_born_finish(now);

		//监听逗龙冷却时间
		it->second->listen_play_dragon_interval_time(now);

		//监听龙的心情值每隔多长时间下降1点
		it->second->listen_dragon_feeling_down_time(now);

		// 体力恢复
		//it->second->listen_strength_recover(now);

		// 扫荡副本
		it->second->listen_wipe_out_dungeon(now);
		// 坐骑过期
		it->second->listen_mount_deadline(now);

		//龙谷刷怪
		it->second->listen_refresh_dragon_vale_monster(now);

		//龙谷掠夺保护时间
		it->second->listen_dragon_rob_protect_time(now);

		// 公会修行，体力恢复
		it->second->listen_recover_phy_power(now);

		// 翅膀检验过期检查
		it->second->listen_wing_experience_expired(now);
	}
	return 0;
}

int Logic_Monitor::player_trigger_daily_zero(void) {
	Time_Value now = Time_Value::gettimeofday();
	trigger_daily_zero_ = now;
	for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->trigger_daily_zero(now);
	}
	GANG_MANAGER->daily_refresh();
	CAMPFIRE_MANAGER->daily_refresh();
	RANKING_MANAGER->trigger_daily_zero(now);
	MALL_MANAGER->trigger_daily_zero(now);
	return 0;
}

int Logic_Monitor::player_trigger_daily_six(void) {
	Time_Value now = Time_Value::gettimeofday();
	trigger_daily_six_ = now;
	for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->trigger_daily_six(now);
	}
	for (DB_View::ID_Player_View_Map::iterator it = LOGIC_MONITOR->db_view()->id_player_view_map.begin(); it != LOGIC_MONITOR->db_view()->id_player_view_map.end(); ++it) {
		it->second->gang_war_award_get_times_by_day =0;
	}

	RANKING_MANAGER->trigger_daily_six(now);
	SECRET_SHOP_MANAGER->trigger_daily_six(now);
	DRAGON_VALE_MANAGER->trigger_daily_six(now);
	MALL_MANAGER->trigger_daily_six(now);
	THORLOTTERY_MANAGER->trigger_daily_six(now);

	return 0;
}

int Logic_Monitor::player_trigger_daily_twenty(void) {
	Time_Value now = Time_Value::gettimeofday();
	trigger_daily_twenty_ = now;
	//	for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
	//it->second->trigger_daily_twenty(now);
	//	}
	ARENA_MANAGER->trigger_daily_twenty(now);
	return 0;
}


int Logic_Monitor::player_trigger_weekly_zero(void) {
	Time_Value now = Time_Value::gettimeofday();
	trigger_weekly_zero_ = now;
	for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->trigger_weekly_zero(now);
	}
	for (DB_View::ID_Player_View_Map::iterator it = LOGIC_MONITOR->db_view()->id_player_view_map.begin(); it != LOGIC_MONITOR->db_view()->id_player_view_map.end(); ++it) {
		it->second->gang_war_award_get_times_by_day =0;
	}
	RANKING_MANAGER->trigger_weekly_zero(now);
	return 0;
}

void Logic_Monitor::arena_season_clear_data(void){
	for (Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->arena_season_clear_data();
	}
}

void Logic_Monitor::handle_hi_power_refresh(void) {

	Int_Queue power_que;
	for( DB_View::ID_Player_View_Map::iterator iter = db_view_->id_player_view_map.begin();
			iter != db_view_->id_player_view_map.end();++iter ) {
		Logic_Player *player = find_logic_player_by_role_id(iter->first);
		int tmp_power = 0;
		if ( NULL != player )
			tmp_power = player->logic_player_detail()->fighter_detail.force;
		else
			tmp_power = iter->second->force;
		if ( power_que.size() < 10 ) { // 取前十战力
			power_que.push(tmp_power);
		} else {
			int min_power = power_que.top();
			if ( tmp_power > min_power ) {
				power_que.pop();
				power_que.push(tmp_power);
			} else
				continue;
		}
	}
	if ( power_que.empty() ) {
			// 正常情况下只有在服务器没人时出现
			highest_power_.write_value(0);
			highest_power_avg_.write_value(0);
	} else {
		uint8_t size = power_que.size();
		int total_power = 0;
		int max_power = 0;
		while ( !power_que.empty() ) {
			total_power += power_que.top();
			max_power = power_que.top();
			power_que.pop();
		}
		highest_power_.write_value(max_power);
		highest_power_avg_.write_value(total_power/size);
	}
}

void Logic_Monitor::handle_hi_power(void) {

	Int_Queue power_que;
	for( DB_View::ID_Player_View_Map::iterator iter = db_view_->id_player_view_map.begin();
			iter != db_view_->id_player_view_map.end();++iter ) {
		if ( power_que.size() < 10 ) { // 取前十战力
			power_que.push(iter->second->force);
		} else {
			int min_power = power_que.top();
			if ( iter->second->force > min_power ) {
				power_que.pop();
				power_que.push(iter->second->force);
			} else
				continue;
		}
	}
	if ( power_que.empty() ) {
		// 正常情况下只有在服务器没人时出现
		highest_power_.write_value(0);
		highest_power_avg_.write_value(0);
	} else {
		uint8_t size = power_que.size();
		int total_power = 0;
		int max_power = 0;
		while ( !power_que.empty() ) {
			total_power += power_que.top();
			max_power = power_que.top();
			power_que.pop();
		}
		highest_power_.write_value(max_power);
		highest_power_avg_.write_value(total_power/size);
	}
}
