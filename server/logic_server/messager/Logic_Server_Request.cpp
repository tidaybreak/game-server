/*
 * Logic_Server_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Logic_Server_Request.h"
#include "Logic_Client_Request.h"
#include "Logic_Monitor.h"
#include "Pool_Manager.h"
#include "Logic_Player.h"
#include "Global_Timer.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Php_Enum.h"
#include "Msg_Php_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Dungeon_Struct.h"
#include "Msg_Dungeon_Enum.h"
#include "mall/Mall_Manager.h"
#include "mail/Mail_Manager.h"
#include "Logic_Public.h"
#include "team/Team.h"
#include "team/Team_Manager.h"
#include "heroer/Logic_Heroer.h"
#include "Err_Code.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Mount.h"
#include "helper/Helper_Def.h"
#include "scene_battle/Battle_Scene.h"
#include "dragon/Dragon_Vale_Manager.h"
#include "gang/Gang_Manager.h"
#include "valentine_monster/Valentine_Monster_Active_Manager.h"
#include "world_boss/Gang_Boss_Active_Manager.h"
#include "world_boss/World_Boss_Active_Manager.h"
#include "honor_arena/Honor_Arena_Active_Manager.h"
#include "logic_outpost/Logic_Outpost_Manager.h"
#include "campfire/Campfire_Manager.h"
#include "announce/Logic_Announce_Manager.h"
#include "arena/Arena_Manager.h"
#include "Msg_Arena_Struct.h"
#include "knight_trials/Knight_Trials_Manager.h"
#include "Config_Cache_Team.h"
#include "expedition/Expedition_Manager.h"
#include "reward/Reward_Manager.h"
#include "answer/Answer_Manager.h"
#include "pack/Pack_Def.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "Scene_Func.h"
#include "reward/Reward_Def.h"
#include "Config_Cache_NPC.h"
#include "Config_Cache_Recharge.h"
#include "active/Active_Manager.h"
#include "Game_Manager.h"
#include "activities_stores/Activities_Stores_Manager.h"
#include "Config_Cache.h"
#include "expedition/Expedition_Def.h"

Logic_Server_Request::Logic_Server_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = LOGIC_MONITOR;
}

Logic_Server_Request::~Logic_Server_Request() {
	// TODO Auto-generated destructor stub
}

int Logic_Server_Request::find_player_by_buf(Block_Buffer &buf, Logic_Player *&player) {
	role_id_t role_id = 0;
	if (buf.read_int64(role_id)) {
		MSG_USER_TRACE("read role id:%ld", role_id);
		return CALL_RETURN;
	}

	player = monitor()->find_logic_player_by_role_id(role_id);
	if (! player) {
		MSG_USER_TRACE("can not find logic player:%ld", role_id);
		return CALL_RETURN;
	}

	return 0;
}

int Logic_Server_Request::player_sign_out(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_DEBUG("role:%ld sign out", player->role_id());
	int out_reason = 0;
	int abnormal_time = 0;
	buf.read_int32(out_reason);
	buf.read_int32(abnormal_time);

	monitor()->db_view()->abnormal_account_add(player->role_id(), abnormal_time);

	player->sign_out(out_reason);

	return 0;
}

int Logic_Server_Request::scene_login_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {

	int login_abnormal = 0;
	int exist_scene = 0;
	/*
	 * 用于组队副本刷新：
	 * 条件: 1.在组队副本
	 *      2.组队副本已经完成
	 * 动作: 返回组队房间*/
	int exist_des_scene = 0;
	buf.read_int32(exist_scene);
	buf.read_int32(exist_des_scene);

	Player_DB_View *player_db_view = 0;
	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id);
	if (find_it == monitor()->db_view()->id_player_view_map.end()) {
		MSG_USER("can not find db_view player:%ld", role_id);
		return 0;
	}
	player_db_view = find_it->second;

	int player_cid = player_db_view->mode_value;
	if (Player_DB_View::ACCOUNT_DOING_LOGIN & player_db_view->account_mode) {
		JUDGE_PLAYER_NULL(player, role_id);
		if (player_cid) {
			if (! exist_scene) {
				// 场景已经不存在,回滚场景
				Coord coord;
				int des_scene = 0, scene_order = 0;
				player->scene_rollback(des_scene, scene_order, coord, true, exist_des_scene, 0);
				int ret = player->notice_scene_login_ready(LOGIC_SCENE_LOGIN_READY);
				if (ret) {
					// 不能连接场景,异常下线
					login_abnormal = 1101;
				}
			} else {
				// 登录完成,通知网关
				int ret = player->notice_gate_login_ready();
				if (ret) {
					// 不能连接网关,异常下线
					login_abnormal = 1102;
				} else {
					MSG_DEBUG("notice gate login, role:%ld, role_name:%s, cid:%d", role_id, player->base_detail().role_name.c_str(), player_cid);
				}
			}
		} else {
			login_abnormal = 1103;
		}
	} else if (Player_DB_View::ACCOUNT_DOING_LOGOUT & player_db_view->account_mode) {
		// 登录过程下线
		login_abnormal = 1104;
	} else {
		// 未知错误
		login_abnormal = 1105;
	}

	if (login_abnormal) {
		MSG_USER("scene_login_ready login fail:%d!, role:%ld, cid:%d", login_abnormal, role_id, player_cid);
	}

	return 0;
}

int Logic_Server_Request::abnormal_logout(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	{
		int p_size = 0;
		role_id_t role_id = 0;
		Logic_Player *player = 0;
		buf.read_int32(p_size);
		for (int i = 0; i < p_size; ++i) {
			buf.read_int64(role_id);
			player = monitor()->find_logic_player_by_role_id(role_id);
			if (!player) {
				LOG_DEBUG("can not find player, role:%ld", role_id);
				continue;
			}
			monitor()->abnormal_logout(player, 10000127);
		}
	}

	return 0;
}

int Logic_Server_Request::scene_logout_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	Time_Value sign_out_flag;
	sign_out_flag.deserialize(buf);

	Player_DB_View *player_db_view = 0;
	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(player->role_id());
	if (find_it == monitor()->db_view()->id_player_view_map.end()) {
		MSG_USER("can not find db_view player:%ld", player->role_id());
		return 0;
	}
	player_db_view = find_it->second;

	if (player_db_view->sign_out_flag_ != sign_out_flag) {
		LOG_DEBUG("sign out flag error role:%ld", role_id);
		return 0;
	}

	player->set_logout_time(Time_Value::gettimeofday());
	player->set_online_flag(0);
	player->set_detail_change(true);
	player->save_player_data();

	monitor()->unbind_logic_player(player_db_view->role_id);

	player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGOUT);
	monitor()->db_view()->abnormal_account.erase(player_db_view->role_id);

	MSG_DEBUG("player logout last, role:%ld, role_name:%s", player_db_view->role_id, player_db_view->role_name.c_str());

	return 0;
}

int Logic_Server_Request::gate_logout_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	int out_reason = 0;
	buf.read_int32(out_reason);

	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(player->role_id());
	if (find_it == monitor()->db_view()->id_player_view_map.end()) {
		MSG_USER("can not find db_view player:%ld", player->role_id());
		return 0;
	}
	find_it->second->sign_out_flag_ = Time_Value::gettimeofday();


	Block_Buffer inner_buf;
	inner_buf.make_message(LOGIC_SCENE_LOGOUT_READY);
	find_it->second->sign_out_flag_.serialize(inner_buf);
	inner_buf.write_int32(out_reason);
	inner_buf.write_int32(1);
	inner_buf.finish_message();
	player->send_to_scene(inner_buf);

	MSG_DEBUG("player logout start, role:%ld, scene:%d", role_id, player->location().scene_id);

	return 0;
}

int Logic_Server_Request::chat_logout_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {

	return 0;
}

int Logic_Server_Request::process_login_session(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	std::string session;
	buf.read_string(session);
	int p_cid = 0;
	buf.read_int32(p_cid);

	// 顶号的登陆
	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id);
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		Time_Value now = Time_Value::gettimeofday();
		player->set_last_sign_out_time(now);
	}

	player->player_login_gate(p_cid);

	return 0;
}

int Logic_Server_Request::gate_login_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	// 登录网关异常处理
	int result = 0;
	buf.read_int32(result);
	if (result) {
		MSG_USER("login gate error, gate exist player, role:%ld", role_id);
		return 0;
	}

	Player_DB_View *player_db_view = 0;
	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(player->role_id());
	if (find_it == monitor()->db_view()->id_player_view_map.end()) {
		MSG_USER("can not find db_view player:%ld", player->role_id());
		return 0;
	}
	player_db_view = find_it->second;

	int player_cid = player_db_view->mode_value;
	if (Player_DB_View::ACCOUNT_DOING_LOGIN & player_db_view->account_mode) {
		player->player_login_gate(player_cid);

		// 登录完成,重置登录流程
		monitor()->db_view()->login_cid_role_id_map.erase(player_cid);
		monitor()->db_view()->abnormal_account.erase(player_db_view->role_id);
		player_db_view->cancel_account_status(Player_DB_View::ACCOUNT_DOING_LOGIN);

		MSG_DEBUG("notice player login, role:%ld, role_name:%s, cid:%d, force:%d, hero force:%d", player->role_id(), player->base_detail().role_name.c_str(),
				player_cid, player->fighter_detail().force, player->get_fight_hero_force());
	} else if (Player_DB_View::ACCOUNT_DOING_LOGOUT & player_db_view->account_mode) {
		// 登录过程下线
		MSG_USER("login break, cid:%d role:%ld", player_cid, player->role_id());
	} else {
		MSG_USER("error status");
	}

	return 0;
}

int Logic_Server_Request::chat_login_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {

	return 0;
}

int Logic_Server_Request::gate_client_login(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	int re_login = 0;
	buf.read_int32(re_login);

	// 是否是托管顶号
	bool is_trustership = false;
	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id);
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		if (Player_DB_View::ACCOUNT_STATUS_TRUSTEESHIP & find_it->second->account_mode) {
			is_trustership = true;
		}
	}

	player->player_login_info();

	MSG_DEBUG("client sign gate->logic:%ld account:%s, re_login:%d, is_trustership:%d", role_id, player->role_name().c_str(), re_login, is_trustership);

	if (! re_login) {
		player->sign_in();
	} else {
		player->sign_in_repeat(is_trustership);
	}

	return 0;
}

int Logic_Server_Request::player_trusteeship(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id);
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		find_it->second->set_account_status(Player_DB_View::ACCOUNT_STATUS_TRUSTEESHIP, 0);
		find_it->second->last_sign_out_time = Time_Value::gettimeofday();
	} else {
		MSG_USER("can not find role:%ld", role_id);
	}

	player->trusteeship_sign_out();

	return 0;
}

int Logic_Server_Request::process_20200010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20200010 msg;
	msg.deserialize(buf);

	player->refresh_mover_detail(msg.location);

	return 0;
}

int Logic_Server_Request::process_20100012(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	player->move_scene_completed();

	return 0;
}

int Logic_Server_Request::process_20100013(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	player->enter_move_scene();

	return 0;
}

int Logic_Server_Request::process_20010050(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	Coord coord;
	MSG_10400005 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	coord.deserialize(buf);
	bool slave = false;
	buf.read_bool(slave);
	player->set_create_scene_args(slave);
	Scene_Key scene = Scene_Key(msg.scene_id, msg.sort, msg.sub_sort);
	int ret = player->scene_transmit(msg.scene_id, msg.scene_order, msg.role_id, coord, msg.name, msg.sort, msg.sub_sort);
//	int dest_scene_id = msg.scene_id;
//	const Scene_Config *src_scene_cfg =
//			CONFIG_CACHE_SCENE->scene_config_cache(player->location().scene_id);
//	if(src_scene_cfg && src_scene_cfg->is_elite_scene() &&
//			src_scene_cfg->dungeon.is_cost_times()){
//		dest_scene_id = player->get_elite_challenge_id(dest_scene_id);
//	}
//	int ret = player->scene_transmit(dest_scene_id, msg.scene_order, msg.role_id, coord, msg.name, msg.sort, msg.sub_sort);
	if (ret) {
		Block_Buffer inner_buf;
		inner_buf.make_message(INNER_GATE_ASK_LOGIC_TRAN);
		inner_buf.write_int64(player->role_id());
		inner_buf.write_int32(ret);
		player->monitor_link().scene_muid.serialize(inner_buf);
		inner_buf.finish_message();
		player->send_to_gate(inner_buf);
	}
	return ret;
}

int Logic_Server_Request::process_20200050(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	Coord coord;
	Monitor_Unique_ID scene_muid;
	scene_muid.deserialize(buf);
	coord.deserialize(buf);
	player->set_scene_muid(scene_muid);
	player->set_grid_coord(coord.grid_coord().x, coord.grid_coord().y);

	{
		Block_Buffer inner_buf;
		inner_buf.make_message(INNER_SYNC_MONITOR_LINK);
		scene_muid.serialize(inner_buf);
		inner_buf.finish_message();
		player->send_to_local_chat(inner_buf);
	}

	{
		Block_Buffer inner_buf;
		inner_buf.make_message(INNER_GATE_ASK_LOGIC_TRAN);
		inner_buf.write_int64(player->role_id());
		inner_buf.write_int32(0);
		scene_muid.serialize(inner_buf);
		player->location().serialize(inner_buf);
		inner_buf.finish_message();
		player->send_to_gate(inner_buf);
	}

	return 0;
}

int Logic_Server_Request::process_20200055(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	Coord coord;
	Monitor_Unique_ID scene_muid;
	Create_Scene_Args csa;
	scene_muid.deserialize(buf);
	coord.deserialize(buf);
	csa.deserialize(buf);

	player->set_scene_muid(scene_muid);
	player->set_grid_coord(coord.grid_coord().x, coord.grid_coord().y);
	player->set_create_scene_args(csa);

	{
		player->notice_scene_login_ready(INNER_ASK_SCENE_SIGN_IN);
	}

	return 0;
}

int Logic_Server_Request::process_20200056(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	Coord coord;
	Monitor_Unique_ID scene_muid;
	scene_muid.deserialize(buf);
	coord.deserialize(buf);

	{
		Block_Buffer inner_buf;
		inner_buf.make_message(INNER_SYNC_MONITOR_LINK);
		scene_muid.serialize(inner_buf);
		inner_buf.finish_message();
		player->send_to_local_chat(inner_buf);
	}

	{
		Block_Buffer inner_buf;
		inner_buf.make_message(INNER_GATE_ASK_LOGIC_TRAN);
		inner_buf.write_int64(player->role_id());
		inner_buf.write_int32(0);
		scene_muid.serialize(inner_buf);
		player->location().serialize(inner_buf);
		inner_buf.finish_message();
		player->send_to_gate(inner_buf);
	}

	return 0;
}

int Logic_Server_Request::process_20200013(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	player->refresh_fighter_detail(buf);
	return 0;
}

int Logic_Server_Request::process_20200200(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20200200 msg;
	msg.deserialize(buf);
	if(msg.battle_type == Battle_Scene::BATTLE_TYPE_ARENA){//arena battle end
		player->arena_battle_result(msg.battle_result);
		if (msg.battle_result) {
			// 任务侦听竞技场胜利
			player->task_listen_arena();
			player->achievement_listen_arena_win_nums();
			player->title_listen_arena_win();
			player->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_ARENA_WIN);
		}
		player->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_ARENA_NUM);
		// 任务侦听竞技场参与（单人、组队）
		player->task_listen_arena_all();
	} else if(msg.battle_type == Battle_Scene::BATTLE_TYPE_CHALLENGE) {
		bool is_same_gang = false;
		for(std::vector<int64_t>::const_iterator it = msg.kill_player.begin(); it != msg.kill_player.end(); ++it) {
			Logic_Player *target_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
			if(target_player) {
				if(target_player->ganger_detail().gang_id != 0
						&& player->ganger_detail().gang_id == target_player->ganger_detail().gang_id) {
					is_same_gang = true;
				}
			}
			else {
				DB_View::ID_Player_View_Map::iterator find_it =
						LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
				if(find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end()
						&& find_it->second) {
					if(find_it->second->gang_id != 0
							&& find_it->second->gang_id == player->ganger_detail().gang_id) {
						is_same_gang = true;
					}
				}
			}

			break;
		}

		if(msg.kill_player.empty() && player->ganger_detail().gang_id != 0) {
			player->task_listen_same_gang_challenge_all();
		}

		if(is_same_gang) {
			player->task_listen_same_gang_challenge_all();
		}
	}
	else if (msg.battle_type == Battle_Scene::BATTLE_TYPE_TEAM_ARENA) { // 组队竞技
		Team *team = player->get_team();
		if (team) {
//			if (msg.battle_result) {
//				++player->integrate();
//				TEAM_MANAGER->add_integrage(role_id, 1);
//			} else {
//				--player->integrate();
//				TEAM_MANAGER->add_integrage(role_id, -1);
//			}
			player->daily_helper_listen(DAILY_FULFILL_LISTEN_TEAM_ARENA);
			player->team_sports_get_award(msg.battle_result, msg.monster_point, msg.monster_group);
			if (msg.battle_result) {
				player->task_listen_team_scene_or_pvp();
				//组队竞技参与并打赢X次
				player->task_listen_team_arena_win();
				player->active_content_listen_team_arena_win();//开服活动监听组队竞技胜利
				player->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TIME_ARENA);
			}
			if (Team::BATTLE_OVER_FLAG_UNSET == team->battle_over_flag()) {
				team->battle_over_flag() = Team::BATTLE_OVER_FLAG_SET;
//				team->sports_status() = Team::TEAM_SPORTS_NORMAL;
//				team->unready_leader();
				team->cancel_match();
				if (msg.battle_result) {
					team->fail_times() = 0;
				} else {
					++team->fail_times();
				}
			}
			MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`)" " VALUES (%d,%ld,%ld,%d)",
							2,  player->role_id(), Time_Value::gettimeofday().sec(), player->level());
			// 任务侦听竞技场参与（单人、组队）
			player->task_listen_arena_all();
			// 组队竞技参与X次(打赢、打输都可以)
			player->task_listen_team_arena_all();
		}
	} else if (msg.battle_type == Battle_Scene::BATTLE_TYPE_KNIGHT_TRIALS) { // 骑士试炼
		player->listen_knight_trials_win_or_lost(msg.battle_result);
	} else {
		// 杀人处理
		for (Role_Vec::iterator it = msg.kill_player.begin(); it != msg.kill_player.end(); ++it) {
			player->hook_kill_player(*it);
		}

		// 杀怪处理
		for (std::vector<Int_Int>::iterator it = msg.monster.begin(); it != msg.monster.end(); ++it) {
			player->hook_kill_monster((*it).val_1, (*it).val_2);
		}

		// 怪点处理
		if(msg.monster_point != 0){
			player->hook_kill_monster_point(msg.scene_id, msg.monster_point);
		}
		// 怪组处理
		if(msg.monster_group != 0){
			player->hook_kill_monster_group(msg.scene_id, msg.monster_group);
		}
	}

	if (msg.battle_type == Battle_Scene::BATTLE_TYPE_HONOR_ARENA) {
		if(msg.battle_result) { //胜利
			player->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_HONOR_ARENA_WIN_NUM_TYPE);//运营活动监听荣耀竞技场胜利次数
		}
	}
	if(msg.scene_id == EXPEDITION_INNER_SCENE_ID) {
		player->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_EXPEDITION_FIGHTER);//运营活动监听
	}

	//单人、精英副本侦听
	const Scene_Config * scene = CONFIG_CACHE_SCENE->scene_config_cache(player->scene_id());
	if(scene){
		switch (scene->type) {
		case War_Scene_Type: {
				if (msg.battle_result) {
					if (msg.battle_type != Battle_Scene::BATTLE_TYPE_PVE) {
						player->achievement_listen_war_kill_nums();
						player->title_listen_war_scene_win();
						player->title_listen_war_scene_kill();
					}
				}
				break;
			}
		case Team_Fb_Scene_Type: {
			Team *team = player->get_team();
			if (team && team->is_leader(player->role_id())) {
				if (team->type() == Team::TEAM_TYPE_FB
						&& !team->is_not_first_team_fb()
						&& team->get_all_team_member_nums() == 1) {
					player->active_is_first_team_fb();
				}
			}
			break;
		}
		case TEAM_DEFEND_Scene_Type: {
			break;
		}
		case Task_Scene_Type: {
			if (msg.battle_result) {
				Team *team = player->get_team();
				int friendship = CONFIG_CACHE_TEAM->team_config().friendship_timelimit;
				MSG_81000102 acv_msg;
				acv_msg.property.push_back(Property(FRIENDSHIP, friendship));
				Role_Hash_Set has_finish_pl;
				if (team && !msg.monster.empty()) {
					Role_Hash_Set members;
					team->team_all_member_set(members);
					for(Role_Hash_Set::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
						Logic_Player *lp = LOGIC_MONITOR->find_logic_player_by_role_id(*iter);
						if (!lp) continue;
						for (std::vector<Int_Int>::iterator it = msg.monster.begin(); it != msg.monster.end(); ++it) {
							if (1 == lp->task_listen_limit_time_kill_boss((*it).val_1, (*it).val_2)) {
								has_finish_pl.insert(*iter);
							}
						}
					}
					if (friendship > 0) {
						bool has_boss = false;
						for (std::vector<Int_Int>::iterator it = msg.monster.begin(); it != msg.monster.end(); ++it) {
							const NPC_Config_Cache* ncc = CONFIG_CACHE_NPC->find_npc_config_cache((*it).val_1);
							if (!ncc
									|| (ncc->type != MOVER_T_MONSTER_BOSS
											&& ncc->type != MOVER_T_MONSTER_ELITE_BOSS)) {
								continue;
							}
							has_boss = true;
						}
						if (has_boss) {
							for(Role_Hash_Set::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
								if (has_finish_pl.count(*iter) == 0) {
									Logic_Player *lp = LOGIC_MONITOR->find_logic_player_by_role_id(*iter);
									if (!lp) continue;
									int ret = lp->pack_add_money(Money_Add_Info(FRIENDSHIP,
											friendship,
											Money_DM_Info(MONEY_ADD_TIMELIMIT)), MONEY_OPT_REAL);
									if (0 == ret) {
										OBJ_SEND_TO_CLIENT(acv_msg, (*lp));
									}
								}
							}
						}
					}
				} else if (!msg.monster.empty()){
					for (std::vector<Int_Int>::iterator it = msg.monster.begin(); it != msg.monster.end(); ++it) {
						player->task_listen_limit_time_kill_boss((*it).val_1, (*it).val_2);
					}
				}
			}
			break;
		}
		default:
			break;
		}
	}

//	// 托管处理
//	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id);
//	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
//		if (Player_DB_View::ACCOUNT_STATUS_TRUSTEESHIP & find_it->second->account_mode) {
//			player->sign_out();
//			MSG_DEBUG("trusteeship role:%ld", role_id);
//		}
//	}

	return 0;
}


int Logic_Server_Request::process_20200202(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	// 托管处理
	DB_View::ID_Player_View_Map::iterator find_it = monitor()->db_view()->id_player_view_map.find(role_id);
	if (find_it != monitor()->db_view()->id_player_view_map.end()) {
		if (Player_DB_View::ACCOUNT_STATUS_TRUSTEESHIP & find_it->second->account_mode) {
			player->sign_out();
			MSG_DEBUG("trusteeship role:%ld", role_id);
		}
	}

	return 0;
}

int Logic_Server_Request::process_20200110(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20200110 msg;
	msg.deserialize(buf);
	switch (msg.prop_type) {
	case PT_LEVEL: {
		int src_level = player->level();
		player->set_level(msg.value);
		player->hook_fighter_level(msg.value, src_level);
		break;
	}
	case PT_FORCE: {
//		player->hook_fighter_force(msg.value);
		player->update_force_by_sync_data(msg.value);
		break;
	}
	}
	return 0;
}

int Logic_Server_Request::process_20300000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20300000 msg;
	msg.deserialize(buf);
	int ret = 0;
	if(msg.item_index == -2){
		ret = player->add_drops_to_package(msg.scene_type, msg.scene_id, msg.item_vec);
	}else{
		ret = player->get_drops_goods(msg.scene_type, msg.scene_id, msg.drops_role_id, msg.item_vec, msg.item_index);
	}
	if (0 == ret) {
		Scene_Type st = get_scene_type(player->location().scene_id);
		switch(st) {
		case Team_Fb_Scene_Type: {
			if (0 == player->team_sub_profile_num()) {
				player->reward_back_listen_fulfill(
						REWARD_BACK_PRO_TEAM_EXPLORE);
			}
			break;
		}
		case TEAM_DEFEND_Scene_Type: {
			player->daily_helper_listen(
					DAILY_FULFILL_LISTEN_Team_Defend);
			if (0 == player->team_sub_profile_num()) {
				player->reward_back_listen_fulfill(
						REWARD_BACK_PRO_TEAM_DEFEND);
			}
			break;
		}
		case HERO_DREAM_Scene_Type: {
			if (0 == player->team_sub_profile_num()) {
				player->reward_back_listen_fulfill(
						REWARD_BACK_PRO_HERO_DREAMS);
			}
			break;
		}
		default:
			break;
		}
	}
	return ret;
}

int Logic_Server_Request::process_20300010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20300010 msg;
	msg.deserialize(buf);
	return player->gather_goods(msg.material_role_id, msg.item_vec);
}

int Logic_Server_Request::process_20200080(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20200080 msg;
	msg.deserialize(buf);

	const Scene_Config* scene_config = CONFIG_CACHE_SCENE->scene_config_cache(msg.scene_id);
	if(scene_config == NULL){
		MSG_USER("cannot find dungeon info, id=~d", msg.scene_id);
		return -1;
	}
	int scene_type = scene_config->type;
	if(scene_type == FuBen_Scene_Type){
		bool is_no_profit = false;
		if (msg.result == 0) {// 没有收益
			is_no_profit = true;
		}
		player->clear_single_dungeon(scene_config, is_no_profit);
	}else if(scene_type == Elite_Scene_Type){
		player->clear_elite_dungeon(scene_config);
	}
	player->hook_compelte_scene(msg.scene_id);
	return 0;
}

int Logic_Server_Request::process_20200081(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20200081 msg;
	msg.deserialize(buf);

	if (msg.result == 0) {
		Team *team = player->get_team();
		if (NULL != team) {
			team->team_fb_role_id_non_strength().assign(msg.no_profit_player_vec.begin(), msg.no_profit_player_vec.end());
			team->in_battle() = 0;
			team->unready_leader();
//			team->set_last_act_time_now();

			TEAM_MANAGER->set_team_index(team);
			TEAM_MANAGER->set_team_id_for_scene(team);
			Int64_Team_Map teams;
			teams.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
			TEAM_MANAGER->active_team_add(team->type(), player, teams, true);

			Logic_Player *leader = team->get_team_leader();
			if (NULL != leader) {
				team->info_all_team_info(*leader);
				leader->hook_compelte_scene(msg.scene_id);
				leader->team_hook_finish_fb(msg.scene_id, 0);
				bool is_not_first_team_fb = team->is_not_first_team_fb();
				team->is_not_first_team_fb() = leader->is_task_submited(CONFIG_CACHE_TEAM->team_config().robot_task);
				if (is_not_first_team_fb != team->is_not_first_team_fb()) {
					team->sync_team_info_to_scene(Team::SYNC_ADD);
				}
				MSG_80100318 info_msg;
				team->get_team_basic_info(info_msg.info);
				{
					OBJ_SEND_TO_CLIENT(info_msg, (*leader));
				}
			}
			for (Role_Hash_Set::const_iterator it = team->team_member_set().begin();
					it != team->team_member_set().end(); ++it) {
				Logic_Player * member = LOGIC_MONITOR->find_logic_player_by_role_id(*it);;
				if (NULL != member) {
					member->hook_compelte_scene(msg.scene_id);
					member->team_hook_finish_fb(msg.scene_id, 0);
				}
			}

			switch(team->type()) {
			case Team::TEAM_TYPE_FB: {
				{
					team->hurt_map().clear();
					team->be_hurt_map().clear();
					team->cure_map().clear();
					uint16_t len = 0;
					role_id_t ri = 0;
					double va = 0.0;
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->hurt_map()[ri] = va;
					}
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->be_hurt_map()[ri] = va;
					}
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->cure_map()[ri] = va;
					}
				}

				team->team_hero_dream_award();
				team->team_card_handle_drops();
				team->has_notice_card() = false;
				team->start_open_card_time() = Time_Value::gettimeofday();
				if (NULL != leader) {
					leader->task_listen_fb(team->scene_id());
					leader->achievement_listen_team_fb_complete_nums(team->enter_level());
					leader->active_content_listen_heroes_door(team->degree(), team->enter_level());
					leader->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_HERO_DOOR);
				}
				for (Role_Hash_Set::const_iterator it = team->team_member_set().begin();
						it != team->team_member_set().end(); ++it) {
					Logic_Player * member = LOGIC_MONITOR->find_logic_player_by_role_id(*it);;
					if (NULL != member) {
						member->task_listen_fb(team->scene_id());
						member->achievement_listen_team_fb_complete_nums(team->enter_level());
						member->active_content_listen_heroes_door(team->degree(), team->enter_level());
						member->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_HERO_DOOR);
					}
				}
				break;
			}
			case Team::TEAM_TYPE_DEFEND: {
				{
					team->hurt_map().clear();
					team->be_hurt_map().clear();
					team->cure_map().clear();
					uint16_t len = 0;
					role_id_t ri = 0;
					double va = 0.0;
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->hurt_map()[ri] = va;
					}
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->be_hurt_map()[ri] = va;
					}
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->cure_map()[ri] = va;
					}
				}

				team->team_hero_dream_award();
				break;
			}
			case Team::TEAM_TYPE_HERO_DREAM: {
				{
					team->hurt_map().clear();
					team->be_hurt_map().clear();
					team->cure_map().clear();
					uint16_t len = 0;
					role_id_t ri = 0;
					double va = 0.0;
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->hurt_map()[ri] = va;
					}
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->be_hurt_map()[ri] = va;
					}
					buf.read_uint16(len);
					for (uint16_t i = 0; i < len; ++i) {
						buf.read_int64(ri);
						buf.read_double(va);
						team->cure_map()[ri] = va;
					}
				}

				team->team_hero_dream_award();
				if (NULL != leader) {
					leader->task_listen_hero_dream_scene(msg.scene_id);
				}
				for (Role_Hash_Set::const_iterator it = team->team_member_set().begin();
						it != team->team_member_set().end(); ++it) {
					Logic_Player * member = LOGIC_MONITOR->find_logic_player_by_role_id(*it);;
					if (NULL != member) {
						member->task_listen_hero_dream_scene(msg.scene_id);
					}
				}
				break;
			}
			default:
				break;
			}
		}
	}

	return 0;
}

int Logic_Server_Request::process_20400004(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20400004 msg;
	msg.deserialize(buf);

//	if (msg.result == 0) {
//		player->hook_compelte_scene(msg.scene_id);
//
//		// 副本清完怪点，通知副本模块，结算通关奖励
//		//if (get_scene_type(msg.scene_id) == FuBen_Scene_Type) {
//			player->clear_monster(msg.scene_id);
//		//}
//	}

	return 0;
}

int Logic_Server_Request::process_20400005(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20400005 msg;
	msg.deserialize(buf);

//	if (msg.result == 0) {
//		player->hook_compelte_scene(msg.scene_id);
//
//		// 副本清完怪点，通知副本模块，结算通关奖励
//		//if (get_scene_type(msg.scene_id) == FuBen_Scene_Type) {
//			player->clear_monster(msg.scene_id);
//		//}
//	}

	return 0;
}

int Logic_Server_Request::process_20400008(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int fight_index = player->get_hero_fight_index();
	if (fight_index != 0) {
		player->req_hero_set_fight(fight_index, 1, false);
	}else{
		int out_index = player->get_hero_out_index();
		if (out_index != 0) {
			player->req_hero_out(out_index, 1, false);
		}
	}
	return 0;
}

int Logic_Server_Request::process_20400009(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int8_t type = 0;
	int hero_id = 0;
	buf.read_int8(type);
	buf.read_int32(hero_id);
	if(type == 0){
		player->req_inner_add_and_fight(hero_id);
	}else{
		player->req_inner_erase_recycle(hero_id);
	}
	return 0;
}


int Logic_Server_Request::process_20300011(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_20300011 msg;
	msg.deserialize(buf);
	return player->add_goods(msg.scene_type, msg.scene_id, msg.goods_vec);
}

/***PHP通知封号处理***/
int Logic_Server_Request::process_30100001(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100001 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	if(msg.ban_type < 10) { //封号
		if(msg.ban_type == 0) {
			//封帐号
			GM_MANAGER->set_forbid_account(msg.account, Time_Value(msg.unban_time, 0));

			const Server_List_Map &server_list = CONFIG_CACHE->server_list_map();
			for(Server_List_Map::const_iterator ser_it = server_list.begin(); ser_it != server_list.end(); ++ser_it) {
				DB_View::Account_Player_View_Map::iterator find_it = monitor()->db_view()->account_player_view_map.find(Account_Key(msg.account, ser_it->first.agent_num, ser_it->first.server_num));
				if (find_it != monitor()->db_view()->account_player_view_map.end()) {
					Logic_Player *_player = monitor()->find_logic_player_by_role_id(find_it->second->role_id);
					if (_player) {
						_player->sign_out();
					}
				}
			}
		} else {
			//封角色ID
			GM_MANAGER->set_forbid_role_id(msg.role_id, Time_Value(msg.unban_time, 0));
			//踢相应玩家下线
			Logic_Player *_player = monitor()->find_logic_player_by_role_id(msg.role_id);
			if (_player) {
				_player->sign_out();
			}
		}

	}else{                //解封
		if(msg.ban_type == 10) {
			//解封帐号
			GM_MANAGER->erase_forbid_account(msg.account);
		} else {
			//解封角色ID
			GM_MANAGER->erase_forbid_role_id(msg.role_id);
		}
	}
	Block_Buffer inner_buf;
	inner_buf.make_message(30100000);
	inner_buf.write_int32(1);
	inner_buf.finish_message();
	monitor()->send_to_server_by_cid(cid, inner_buf);
	return 0;
}

/***PHP通知封禁IP处理***/
int Logic_Server_Request::process_30100010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100010 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	if(msg.type == 0) {
		//封IP
		GM_MANAGER->set_forbid_ip(msg.ip_address, Time_Value(msg.unban_time, 0));
	} else if(msg.type == 1) {
		//解封IP
		GM_MANAGER->erase_forbid_ip(msg.ip_address);
	}

	Block_Buffer inner_buf;
	inner_buf.make_message(RETURN_TO_PHP_MSG);
	inner_buf.write_int32(1);
	inner_buf.finish_message();
	monitor()->send_to_server_by_cid(cid, inner_buf);
	return 0;
}

/***PHP商品信息变动通知***/
int Logic_Server_Request::process_30100002(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100002 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));

	MALL_MANAGER->goods_changed(msg.goods_list, msg.diff_tabs, msg.promo_goods_list, msg.banner_goods_list);

	Block_Buffer inner_buf;
	inner_buf.make_message(RETURN_TO_PHP_MSG);
	inner_buf.write_int32(1);
	inner_buf.finish_message();
	monitor()->send_to_server_by_cid(cid, inner_buf);
	return 0;
}

/****充值通知*****/
int Logic_Server_Request::process_30100003(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100003 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));

	int status = LOGIC_PUBLIC->recharge_grant(role_id, msg.pay_num);

	Block_Buffer inner_buf;
	inner_buf.make_message(RETURN_TO_PHP_MSG);
	inner_buf.write_int32(status);
	inner_buf.finish_message();
	monitor()->send_to_server_by_cid(cid, inner_buf);

	//充值礼包
	uint32_t mail_id = CONFIG_CACHE_RECHARGE->get_pay_gift_mail_id_by_pay(msg.pay_num);
	if(mail_id) {
		LOGIC_PUBLIC->send_mail(mail_id, role_id);
	}
	return 0;
}

int Logic_Server_Request::process_30100009(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100003 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));

	int status = LOGIC_PUBLIC->inner_recharge_grant(role_id, msg.pay_num);

	Block_Buffer inner_buf;
	inner_buf.make_message(RETURN_TO_PHP_MSG);
	inner_buf.write_int32(status);
	inner_buf.finish_message();
	monitor()->send_to_server_by_cid(cid, inner_buf);

	//充值礼包
	uint32_t mail_id = CONFIG_CACHE_RECHARGE->get_pay_gift_mail_id_by_pay(msg.pay_num);
	if(mail_id) {
		LOGIC_PUBLIC->send_mail(mail_id, role_id);
	}
	return 0;
}

int Logic_Server_Request::process_30100004(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100004 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	if(msg.type == 3) {//发送给全服玩家
		return MAIL_MANAGER->send_sys_mail_to_all(msg.sender, msg.title, msg.contents, msg.gold, msg.item);
	}
	if(msg.type == 4) {// 按等级段发送
		return MAIL_MANAGER->send_sys_mail_to_level_rang(msg.start_level, msg.end_level, msg.sender, msg.title, msg.contents, msg.gold, msg.item, msg.career);
	}

	for(Role_Vec::iterator iter = msg.role_ids.begin(); iter != msg.role_ids.end(); iter++) {
		//if(LOGIC_MONITOR->find_logic_player_by_role_id(*iter)){
			Mail_Send_Info send_info = Mail_Send_Info(*iter, 0l, 0, msg.sender, msg.title, msg.contents);
			send_info.show_send_time = Time_Value::gettimeofday();
			send_info.gold = msg.gold;
			send_info.send_type = MAIL_TYPE_PHP;
			send_info.item_vector.clear();
			for(std::vector<Item_Info>::iterator it = msg.item.begin(); it != msg.item.end(); ++it) {
				send_info.item_vector.push_back(Item_Detail(it->id, it->amount, it->bind));
			}
			LOGIC_PUBLIC->send_mail(send_info);
		//}
	}
	return 0;
}

int Logic_Server_Request::process_30100006(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100006 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));

	MSG_30500006 re_msg;
	re_msg.reset();
	switch(msg.type) {
		case 1: {
			re_msg.num = LOGIC_MONITOR->player_role_id_map().size();
			break;
		}
	}

	Block_Buffer inner_buf;
	inner_buf.make_message(RES_PHP_GET_ACCESS_COUNT);
	re_msg.serialize(inner_buf);
	inner_buf.finish_message();
	monitor()->send_to_server_by_cid(cid, inner_buf);
	return 0;
}

int Logic_Server_Request::process_30100007(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100007 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return LOGIC_PUBLIC->reply_proposal(msg.role_id, msg.sender, msg.title, msg.contents, msg.gold);
}

int Logic_Server_Request::process_30100011(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100011 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return REWARD_MANAGER->add_activation_key(msg.key);
}

int Logic_Server_Request::process_30100012(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100012 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return LOGIC_PUBLIC->send_facebook_share_mail(role_id, msg.type);
}

int Logic_Server_Request::process_30100013(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100013 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	 ACTIVE_MANAGER->php_open_active(msg.id, msg.start_time, msg.end_time);
	 return 0;
}

int Logic_Server_Request::process_30100014(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100014 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	 ACTIVE_MANAGER->php_close_active(msg.id, msg.end_time);
	 return 0;
}

int Logic_Server_Request::process_30100015(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100015 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	ACTIVITIES_STORES_MANAGER->php_send_activity_exchange_mall_goods_change(msg.goods);
	 return 0;
}

int Logic_Server_Request::process_30100016(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100016 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	CONFIG_CACHE_MOUNT->php_notice_fuse_time(msg.mount_id, msg.begin_time, msg.end_time);
	return 0;
}

int Logic_Server_Request::process_30100008(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100008 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));

	Block_Buffer tbuf;
	tbuf.make_message(30100008);
	msg.serialize(tbuf);
	tbuf.finish_message();
	return LOGIC_MONITOR->send_to_local_chat(tbuf);
}

int Logic_Server_Request::process_30100005(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_30100005 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));

	Block_Buffer tbuf;
	tbuf.make_message(30100005);
	msg.serialize(tbuf);
	tbuf.finish_message();
	return LOGIC_MONITOR->send_to_local_chat(tbuf);
}

int Logic_Server_Request::process_20100024(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20100024 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return player->skill_cost(msg.type, msg.arg1, msg.arg2, msg.cost);
}

int Logic_Server_Request::process_20300031(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	return 0;
}

int Logic_Server_Request::process_20300033(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	int8_t inner_fetch = 0;
	role_id_t fight_id = 0;
	role_id_t match_id = 0;
	buf.read_int8(inner_fetch);
	buf.read_int64(fight_id);
	buf.read_int64(match_id);
	ARENA_MANAGER->inner_arena_fetch_fight_data(inner_fetch, fight_id, match_id);
	return 0;
}

int Logic_Server_Request::process_20300034(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	ARENA_MANAGER->inner_arena_match_player(buf);
	return 0;
}

int Logic_Server_Request::process_20300035(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	role_id_t fetch_role_id = 0;
	buf.read_int64(fetch_role_id);
	Logic_Player* fetch_player = LOGIC_MONITOR->find_logic_player_by_role_id(fetch_role_id);
	if(fetch_player){
		MSG_50100400 inner_msg;
		inner_msg.deserialize(buf);
		OBJ_SEND_TO_CLIENT(inner_msg, (*fetch_player));
	}
	return 0;
}


int Logic_Server_Request::process_20300036(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20300036 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	ARENA_MANAGER->inner_arena_sync_inner_rank(msg.rank_info);
	return 0;
}

int Logic_Server_Request::process_20300037(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
//	MSG_20300037 msg;
//	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	ARENA_MANAGER->inner_arena_player_data_complete(buf);
	return 0;
}

int Logic_Server_Request::process_20300039(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20300039 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	ARENA_MANAGER->inner_arena_save_king_time(msg.role_id, msg.king_time);
	return 0;
}

int Logic_Server_Request::process_20100312(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100312 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	TEAM_MANAGER->active_integrate(role_id, msg.rank);

	return 0;
}

int Logic_Server_Request::process_20100310(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	player->send_score_list_to_client(buf);


	return 0;
}


int Logic_Server_Request::process_20100304(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	player->team_load_info(buf);
	return 0;
}

int Logic_Server_Request::process_20100305(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100305 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->use_drug_inner(msg.index, msg.val, msg.amount, buf);
	return 0;
}

int Logic_Server_Request::process_20100307(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	player->team_load_cumulate_award(buf);
	return 0;
}

int Logic_Server_Request::process_20100030(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf){
	MSG_20100030 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	player->sync_phy_power_info(msg.phy_power_cur, msg.phy_power_recover_time,
			msg.phy_power_buy_times);
	return 0;
}

int Logic_Server_Request::process_20100500(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf){
	JUDGE_PLAYER_NULL(player, role_id);
	player->load_status_info(buf);
	return 0;
}

int Logic_Server_Request::process_20100504(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf){
	JUDGE_PLAYER_NULL(player, role_id);
	player->handle_hero_status_info_from_scene(buf);
	return 0;
}

int Logic_Server_Request::process_20100401(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf){
	JUDGE_PLAYER_NULL(player, role_id);
	player->req_battle_status(buf);
	return 0;
}

int Logic_Server_Request::process_20410000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	role_id_t target_role_id = 0;
	buf.read_int64(target_role_id);
	Logic_Player* target_player = monitor()->find_logic_player_by_role_id(target_role_id);
	if (! target_player) {
		MSG_USER_TRACE("player in scene but can not find logic player:%ld", target_role_id);
		return ERROR_PLAYER_NOT_ONLINE;
	}

	/*
	 * 各模块主动消息
	 */
	player->active_panal_hero_tags(target_player);
	player->active_panal_hero_info(target_player);
	player->active_rune_info(target_player);
	player->pack_active_role_panel_hero_equip(target_player);
	player->active_equip_master_info(target_player);
	player->pack_active_pack_rune_stone(target_player);
	player->active_exploit_info(target_player);

	return 0;
}

int Logic_Server_Request::process_20420000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	role_id_t sender_role_id = 0;
	int32_t bless_buf_id = 0;
	int8_t has_buf_id = 0;
	buf.read_int64(sender_role_id);
	buf.read_int32(bless_buf_id);
	buf.read_int8(has_buf_id);

	return player->inner_send_bless_buf(sender_role_id, bless_buf_id, has_buf_id);
}

int Logic_Server_Request::process_20300100(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	role_id_t player_role_id = 0;
	role_id_t war_scene_id = 0;
	int side = 0;
	Coord coord;
	buf.read_int64(player_role_id);
	buf.read_int64(war_scene_id);
	buf.read_int32(side);
	coord.deserialize(buf);
	Logic_Player * l_player = LOGIC_MONITOR->find_logic_player_by_role_id(player_role_id);
	if(l_player){
		l_player->war_scene_enter(war_scene_id, side, coord);
	}
	return 0;
}

int Logic_Server_Request::process_20300101(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 0:{
			int add_merit = 0;
			int add_contribute = 0;
			buf.read_int32(add_merit);
			buf.read_int32(add_contribute);
			player->add_gang_merit_contribute(add_merit, add_contribute);
			break;
		}
		case 1:{
			int add_merit = 0;
			int add_pieces = 0;
			int add_rep = 0;
			int add_arena_coin = 0;
			int prop_num = 0;
			std::vector<Id_Int_Int> tmp_props;
			buf.read_int32(add_merit);
			buf.read_int32(add_pieces);
			buf.read_int32(add_rep);
			buf.read_int32(add_arena_coin);
			buf.read_int32(prop_num);
			for(int i = 0; i < prop_num; ++i){
				Id_Int_Int prop;
				prop.deserialize(buf);
				tmp_props.push_back(prop);
			}

			MSG_81000102 prop_acv_msg;
			prop_acv_msg.type = 0;
			Money_Add_List money_add_list;
			Item_Vec pack_item_vec;
			int prop_exp = 0;

			int prop_count = tmp_props.size();
			for(int i = 0; i < prop_count; ++i){
				if(tmp_props[i].id == PT_EXP_CURRENT){
					prop_exp += tmp_props[i].val1;
				}else if(tmp_props[i].id == COPPER || tmp_props[i].id == GOLD ||tmp_props[i].id == BIND_GOLD ||
						tmp_props[i].id == SOULS || tmp_props[i].id == DRAGON_SOUL || tmp_props[i].id == FRIENDSHIP ||
						tmp_props[i].id == REPUTATION){
					money_add_list.push_back(Money_Add_Info((Money_Type)tmp_props[i].id, tmp_props[i].val1, Money_DM_Info(MONEY_ADD_WAR_AWARD)));
					prop_acv_msg.property.push_back(Property(tmp_props[i].id, tmp_props[i].val1));
				}else{
					Item_Detail item(tmp_props[i].id, tmp_props[i].val1,tmp_props[i].val2);
					pack_item_vec.push_back(item);
					Item_Basic_Info item_base;
					item.set_item_basic_info(item_base);
					prop_acv_msg.item_info.push_back(item_base);
				}
			}

			if(prop_exp > 0){
				Exp_All exp_all = player->modify_experience_inter(prop_exp, false, false, true);
				if (0 == exp_all.result) {
					prop_acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
					prop_acv_msg.vip_exp = exp_all.vip_exp;
					prop_acv_msg.world_level = player->get_world_level_add();
				}
			}
			if(add_rep > 0){// 声望
				money_add_list.push_back(Money_Add_Info((Money_Type)REPUTATION, add_rep, Money_DM_Info(MONEY_ADD_WAR_AWARD)));
				prop_acv_msg.property.push_back(Property(REPUTATION, add_rep));
			}
			if (!money_add_list.empty()) {
				player->pack_add_money(money_add_list, MONEY_OPT_REAL);
			}
			if(!pack_item_vec.empty()){
				player->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
							pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_WAR_SCENE));
			}

			player->add_war_merit_piece(add_merit, add_pieces, add_arena_coin);
			if (!prop_acv_msg.item_info.empty() || !prop_acv_msg.property.empty()) {
				OBJ_SEND_TO_CLIENT(prop_acv_msg, (*player));
			}
			break;
		}
		case 2:{
			Time_Value forbid_time;
			forbid_time.deserialize(buf);
			player->sync_war_forbid_time(forbid_time);
			break;
		}
		case 3:{
			int gold = 0;
			buf.read_int32(gold);
			int8_t revive_result = 0;
			if(gold > 0){
				int result = player->pack_sub_money(Money_Sub_Info(Money_Sub_Type(BIND_GOLD_FIRST), gold, MONEY_SUB_FB_REVIVE));
				if(result != 0){
					player->send_err_to_client(result, REQ_SINGLE_DUNGEON_REVIVE);
				}else{
					revive_result = 1;
				}
			}

			Block_Buffer buf;
			buf.make_message(INNER_WAR_SCENE_SYNC_INFO);
			buf.write_int8(3);
			buf.write_int8(revive_result);
			buf.finish_message();
			player->send_to_scene(buf);
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}


int Logic_Server_Request::process_20100040(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100040 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return player->sync_mount_ride(msg.ride, msg.model_id, msg.speed);
}

int Logic_Server_Request::process_20600001(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600001 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return DRAGON_VALE_MANAGER->process_rob_match(msg.uuid, msg.role_id, msg.level, msg.coin,
							msg.server_name, msg.role_name, msg.last_coin);
}

int Logic_Server_Request::process_20600003(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600003 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return DRAGON_VALE_MANAGER->process_rob_fight_load_player_data(msg.to_uuid, msg.to_role_id,
						msg.to_server_name, msg.to_role_name, msg.role_id);
}

int Logic_Server_Request::process_20600005(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600005 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	Logic_Player *rob_player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!rob_player) {
		Logic_Player::offline_dragon_vale_rob_fight_result_notice(msg.role_id, DRAGON_VALE_ROB_NOTICE,
									msg.server_name, msg.player_name, msg.result);
	}
	else {
		MSG_TRACE_ABORT("rob is not in dragon vale, rob_role_name=%s, target_role_name=%s",
				rob_player->role_name().c_str(), msg.player_name.c_str());
		MSG_82300023 msg_82300023;
		msg_82300023.server_name = msg.server_name;
		msg_82300023.player_name = msg.player_name;
		msg_82300023.result = msg.result;
		Block_Buffer buf;
		buf.make_message(msg_82300023.msg_id);
		msg_82300023.serialize(buf);
	    buf.finish_message();
	    rob_player->send_to_client(buf);
	}

	return 0;
}

int Logic_Server_Request::process_20600006(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600006 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	Logic_Player *target_player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!target_player) {
		Logic_Player::offline_dragon_vale_rob_fight_result_notice(msg.role_id, DRAGON_VALE_TARGET_NOTICE,
									msg.server_name, msg.player_name, msg.result);
	}
	else {
		MSG_82300024 msg_82300024;
		msg_82300024.server_name = msg.server_name;
		msg_82300024.player_name = msg.player_name;
		msg_82300024.result = msg.result;
		Block_Buffer buf;
		buf.make_message(msg_82300024.msg_id);
		msg_82300024.serialize(buf);
	    buf.finish_message();
	    target_player->send_to_client(buf);
	}

	return 0;
}

int Logic_Server_Request::process_20600007(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600007 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_rob_enter_dragon_vale(msg.to_uuid, msg.to_role_id, msg.role_id, msg.limit_time);
	return 0;
}

int Logic_Server_Request::process_20600009(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600009 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_gain_rob_production(msg.to_uuid, msg.to_role_id, msg.to_server_name, msg.to_role_name,
									msg.uuid, msg.role_id, msg.building_id, msg.building_type);
	return 0;
}

int Logic_Server_Request::process_20600010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600010 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_gain_rob_production_res(msg);
	return 0;
}

int Logic_Server_Request::process_20600011(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600011 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	Logic_Player *target_player = LOGIC_MONITOR->find_logic_player_by_role_id(msg.role_id);
	if(!target_player) {
		Logic_Player::offline_dargon_vale_gain_rob_production_notice(msg.role_id, msg.server_name, msg.role_name,
											msg.total_gold_mine, msg.total_moon_weel, msg.total_totem);
	}
	else {
		MSG_82300018 msg_82300018;
		msg_82300018.server_name  = msg.server_name;
		msg_82300018.player_name = msg.role_name;
		msg_82300018.total_gold_mine = msg.total_gold_mine;
		msg_82300018.total_moon_weel = msg.total_moon_weel;
		msg_82300018.total_totem = msg.total_totem;
		OBJ_SEND_TO_CLIENT(msg_82300018, (*target_player));
	}

	return 0;
}

int Logic_Server_Request::process_20600013(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600013 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_exit_dragon_vale(msg.creater, msg.role_id);
	return 0;
}

int Logic_Server_Request::process_20600016(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600016  msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_drive_thief(msg.player_id, msg.role_id, msg.id, msg.quality);
	return 0;
}

int Logic_Server_Request::process_20600017(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600017 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_save_notice_board_drive_robber(msg.role_id, msg.name);
	return 0;
}

int Logic_Server_Request::process_20600018(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600018 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_save_notice_board_rob_production(msg.role_id, msg.name,
							msg.gold_mine, msg.moon_well, msg.totem);
	return 0;
}

int Logic_Server_Request::process_20600019(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600019 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_enter_rob_protect_time(msg.role_id);
	return 0;
}

int Logic_Server_Request::process_20600020(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600020 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_rob_match_fail_back_coin(msg.uuid, msg.role_id, msg.coin, msg.type);
	return 0;
}

int Logic_Server_Request::process_20600021(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600021 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_clear_rob_protect_status(msg.role_id);
	return 0;
}

int Logic_Server_Request::process_20600022(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600022 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_clear_rob_production_process(msg.to_role_id);
	return 0;
}

int Logic_Server_Request::process_20600023(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600023 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->proecss_notify_tod_rob_production_finish(msg.to_role_id);
	return 0;
}

int Logic_Server_Request::process_20600024(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600024 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_set_first_enter_after_robbed(msg.role_id);
	return 0;
}

int Logic_Server_Request::process_20600025(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20600025 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->rob_dragon_vale_task_listen();
	player->daily_helper_listen(DAILY_FULFILL_LISTEN_DRAGON_VALE_ROB);

	return 0;
}

int Logic_Server_Request::process_20600026(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600026 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	LOGIC_ANN_MANAGER->create_ann_success_protect_dragon_vale(msg.to_server_name, msg.to_role_name, msg.role_id);
	return 0;
}

int Logic_Server_Request::process_20600027(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20600027 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->process_robber_win_in_rob_fight();
	return 0;
}

int Logic_Server_Request::process_20600028(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20600028 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->process_robber_gain_total_production(msg.gold_mine, msg.moon_well, msg.totem);
	return 0;
}

int Logic_Server_Request::process_20600029(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20600029 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->process_robber_begin_attck();
	return 0;
}

int Logic_Server_Request::process_20600031(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600031 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_dragon_vale_rob(msg.to_uuid, msg.to_role_id, msg.to_server_name, msg.to_role_name,
												msg.uuid, msg.role_id, msg.server_name, msg.role_name, msg.coin, msg.flag);
	return 0;
}

int Logic_Server_Request::process_20600032(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600032 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->notify_master_of_dragon_fight_end(msg.server_name, msg.player_name, msg.result, msg.is_drive, msg.role_id);
	return 0;
}

int Logic_Server_Request::process_20600033(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600033 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->notify_draong_vale_no_enter(msg.left_time, msg.role_id);
	return 0;
}

int Logic_Server_Request::process_20600034(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20600034 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	DRAGON_VALE_MANAGER->process_target_in_defence_dragon_vale_fight(msg.role_id, msg.flag);
	return 0;
}

int Logic_Server_Request::process_20300113(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20300113 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	CAMPFIRE_MANAGER->player_get_bonfire_award(player, msg);
	return 0;
}

int Logic_Server_Request::process_20300115(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20300115 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	CAMPFIRE_MANAGER->load_player_info(player);
	return 0;
}

int Logic_Server_Request::process_20300116(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20300116 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	CAMPFIRE_MANAGER->save_player_info(player, msg);
	return 0;
}

int Logic_Server_Request::process_20300117(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20300117 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	CAMPFIRE_MANAGER->mail_to_player(player, msg);
	return 0;
}

int Logic_Server_Request::process_20200021(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	player->helper_listen_world_chat();
	return 0;
}


int Logic_Server_Request::process_20200201(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20200201 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	player->activity_result(msg.act_id, msg.type, msg.result, msg.value1, msg.value2, msg.value3);
	return 0;
}

int Logic_Server_Request::process_20500000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20500000 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));

	Block_Buffer buf__;
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();

	LOGIC_MONITOR->send_to_local_chat(buf__);
	return 0;
}

int Logic_Server_Request::process_20610000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	return KNIGHT_TRIALS_MANAGER->logic_knight_trials_match_player(buf);
}

int Logic_Server_Request::process_20610010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	return EXPEDITION_MANAGER->data_channle(buf);
}

int Logic_Server_Request::process_20610011(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20610011 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	return player->set_expedition_coord(msg.coord);
}

int Logic_Server_Request::process_20610012(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	return player->expedition_data_channle(buf);
}

int Logic_Server_Request::process_20610020(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	if(player) {
		return player->data_channle(buf);
	} else {
		return ANSWER_MANAGER->data_channle(buf);
	}
	return 0;
}

int Logic_Server_Request::process_20100200(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_50100032 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(msg.role_id);
	if (pl) {
		msg.role_id = player->role_id();
		OBJ_SEND_TO_CLIENT(msg, (*pl));

		/*
		 * 各模块主动消息
		 */
		pl->active_panal_hero_tags(player);
		pl->active_panal_hero_info(player);
		pl->pack_active_role_panel_hero_equip(player);
		pl->active_rune_info(player);
		pl->active_equip_master_info(player);
		pl->pack_active_pack_rune_stone(player);
		pl->active_exploit_info(player);
	}
	return 0;//player->set_expedition_coord(msg.coord);
}

int Logic_Server_Request::process_20300110(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	//MSG_20300110 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	//JUDGE_PLAYER_NULL(player, role_id);
	//player->;
	return 0;
}

int Logic_Server_Request::process_20300111(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	//MSG_20300110 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	//JUDGE_PLAYER_NULL(player, role_id);
	//player->;
	return 0;
}

int Logic_Server_Request::process_20300112(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	//MSG_20300110 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	//JUDGE_PLAYER_NULL(player, role_id);
	return CAMPFIRE_MANAGER->cmd_sync_campfire_info(buf);
}


int Logic_Server_Request::process_20300300(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	//MSG_20300300 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	//JUDGE_PLAYER_NULL(player, role_id);
	int act_icon_id = 0;
	buf.read_int32(act_icon_id);
	switch(act_icon_id){
		case 16:{
			return WORLD_BOSS_ACTIVE_MANAGER->inner_sync_info(player, buf);
			break;
		}
		case 19:{
			return GANG_BOSS_ACTIVE_MANAGER->inner_sync_info(player, buf);
			break;
		}
		case 20:{
			return VALENTINE_MONSTER_ACTIVE_MANAGER->inner_sync_info(player, buf);
		}
		case 22:{
			return HONOR_ARENA_ACTIVE_MANAGER->inner_sync_info(player, buf);
		}
		default:{
			break;
		}
	}
	return 0;
}

int Logic_Server_Request::process_20300301(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	//MSG_20300301 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	//JUDGE_PLAYER_NULL(player, role_id);
	//player->;
	return 0;
}

int Logic_Server_Request::process_20300302(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	//MSG_20300302 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	//JUDGE_PLAYER_NULL(player, role_id);
	return 0;
}


int Logic_Server_Request::process_20300303(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	//MSG_20300303 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	//JUDGE_PLAYER_NULL(player, role_id);
	int act_scene_icon_id = 0;
	buf.read_int32(act_scene_icon_id);
	switch(act_scene_icon_id){
		case 16:{
			return WORLD_BOSS_ACTIVE_MANAGER->inner_act_end(buf);
			break;
		}
		case 19:{
			return GANG_BOSS_ACTIVE_MANAGER->inner_act_end(buf);
		}
		case 22:{
			return HONOR_ARENA_ACTIVE_MANAGER->inner_act_end(buf);
		}
		default:{
			break;
		}
	}
	return 0;
}

int Logic_Server_Request::process_20300400(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	return LOGIC_OUTPOST_MANAGER->inner_sync_info(player, buf);
}

int Logic_Server_Request::process_20170001(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	GANG_MANAGER->gang_war_load_matched_info(buf);
	return 0;
}
int Logic_Server_Request::process_20170002(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	player->ganger_load_war_detail(buf);
	return 0;
}
int Logic_Server_Request::process_20170003(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	GANG_MANAGER->gang_war_load_spot_rank_info(buf);
	return 0;
}
int Logic_Server_Request::process_20170005(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	GANG_MANAGER->gang_war_load_result_info(buf);
	return 0;
}
int Logic_Server_Request::process_20170007(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	GANG_MANAGER->gang_war_load_award(buf);
	return 0;
}
int Logic_Server_Request::process_20170008(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	GANG_MANAGER->gang_war_help(buf);
	return 0;
}
//int Logic_Server_Request::process_20170009(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
//	player->gang_war_load_award(buf);
//	return 0;
//}

int Logic_Server_Request::process_20100016(int cid, role_id_t role_id, Logic_Player* player, Block_Buffer &buf) {

	MSG_20100016 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	return player->learn_skill_cost_item(msg.skill_id, msg.skill_lv);
}

int Logic_Server_Request::process_20100017(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20100017 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	player->close_guide_by_type(GUIDE_FINISH_TYPE_LEARN_ROLE_SKILL, msg.skill_id);
	return 0;
}

int Logic_Server_Request::process_20100027(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	MSG_20100027 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);

	player->update_force_by_sync_data(msg.force);
	return 0;
}

int Logic_Server_Request::process_20000052(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {
	int sec = 0, usec = 0;
	Monitor_Unique_ID muid;
	muid.deserialize(buf);
	buf.read_int32(sec);
	buf.read_int32(usec);
	Time_Value t = Time_Value::gettimeofday() - Time_Value(sec, usec);
	monitor_->update_inner_tick(muid, t);

	return 0;
}

int Logic_Server_Request:: process_20100031(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf) {

	JUDGE_PLAYER_NULL(player, role_id);
	uint16_t energy = 0;
	buf.read_uint16(energy);
	player->handle_energy_from_sence(energy);
	return 0;
}
