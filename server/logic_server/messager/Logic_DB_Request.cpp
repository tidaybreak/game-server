/*
 * Logic_DB_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Logic_DB_Request.h"
#include "Logic_Client_Request.h"
#include "Logic_Monitor.h"
#include "Pool_Manager.h"
#include "Logic_Player.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "ranking/Ranking_Manager.h"
#include "reward/Reward_Def.h"


Logic_DB_Request::Logic_DB_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = LOGIC_MONITOR;
}

Logic_DB_Request::~Logic_DB_Request() {
	// TODO Auto-generated destructor stub
}

int Logic_DB_Request::process_load_player_data(Logic_Player_Detail *detail) {
	if (! detail) {
		MSG_USER("player_data == 0.");
		return -1;
	}

	if (detail->role_id <= 0) {
		POOL_MANAGER->push_player_data_pool(detail, 20);
		MSG_USER("player_data role_id:%d", detail->role_id);
		return -1;
	}

	Player_DB_View *player_db_view = 0;
	DB_View::ID_Player_View_Map::iterator db_player_find_it = monitor()->db_view()->id_player_view_map.find(detail->role_id);
	if (db_player_find_it == monitor()->db_view()->id_player_view_map.end()) {
		POOL_MANAGER->push_player_data_pool(detail, 21);
		MSG_USER("can not find db_view player:%ld", detail->role_id);
		return 0;
	}

	player_db_view = db_player_find_it->second;
	int cid = player_db_view->mode_value;

	// 处理登录过程中断线
	if (Player_DB_View::ACCOUNT_DOING_LOGOUT & player_db_view->account_mode) {
		POOL_MANAGER->push_player_data_pool(detail, 22);
		player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGOUT);
		MSG_USER("player login break, role:%ld, role_name:%s", detail->role_id, detail->base_detail.role_name.c_str());
		return 0;
	}

	// 没有此玩家数据, 可能数据库被删除
	if (detail->status == Logic_Player_Detail::ROLE_NOT_EXIST) {
		MSG_USER("can not find role:%ld", detail->role_id);
		POOL_MANAGER->push_player_data_pool(detail, 23);
		player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGIN);
		return monitor()->send_error_to_client(cid, 10000000, 10100001);
	}

	if (detail->status == Logic_Player_Detail::SUCCESS_LOADED) { /// [登录]数据加载成功
		Logic_Player *player = LOGIC_CLIENT_REQUEST->process_success_login(cid, *detail);
		if (!player) {
			monitor()->db_view()->login_cid_role_id_map.erase(cid);
			player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGIN);

			monitor()->send_error_to_client(cid, 10000000, 10100001);
			//POOL_MANAGER->push_player_data_pool(detail); 	player回收时已经回收detail
			MSG_USER("player login logic error, role:%ld, role_name:%s", player_db_view->role_id, player_db_view->role_name.c_str());
		} else {
			monitor()->db_view()->abnormal_account_add(player->role_id());
			MSG_DEBUG("player login logic success, role:%ld, role_name:%s", player_db_view->role_id, player_db_view->role_name.c_str());
		}
	} else {
		MSG_USER("unknow status = %d.", detail->status);
		POOL_MANAGER->push_player_data_pool(detail, 24);
		player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGIN);
		return monitor()->send_error_to_client(cid, 10000000, 10100001);
	}

	return 0;
}

int Logic_DB_Request::process_refresh_ranking_data(Block_Buffer &buf) {
	return RANKING_MANAGER->process_refresh_ranking_data(buf);
}

//int Logic_DB_Request::process_activation_code_data(Block_Buffer &buf) {
//	uint32_t type = 0;
//	buf.read_uint32(type);
//	switch(type) {
//		case ACTIVATION_CODE_OP_QUERY: {
//			role_id_t role_id = 0;
//			buf.read_int64(role_id);
//			Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
//			if (player) {
//				Activation_Code_Info code_info;
//				code_info.deserialize(buf);
//				return player->process_activation_code_data(code_info);
//			}
//			break;
//		}
//	}
//	return 0;
//}
