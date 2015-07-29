/*
 * Player_Test.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: ti
 */

#include "Logic_Player.h"
#include "Logic_Module_Head.h"
#include "Logic_Client_Request.h"
#include "Logic_Player_Detail.h"
#include "Msg_Active_Struct.h"
#include "Msg_Task_Struct.h"
#include "Logic_Public.h"
#include "Global_Timer.h"
#include "Err_Code.h"
#include "arena/Arena_Manager.h"
#include "Config_Cache_Icon.h"
#include "Game_Typedef.h"
#include "Msg_Inner_Struct.h"
#include "icon/Icon_Config.h"
#include "icon/Icon_Struct.h"
#include "icon/Icon.h"
#include "gang/Gang_Manager.h"
#include "team/Team_Manager.h"
#include "Config_Cache_Task.h"
#include "task/Task_Config.h"
#include "task/Task_Struct.h"
#include "task/Tasker.h"
#include "Err_Code.h"
#include "Config_Cache.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Item.h"
#include "secret_shop/Secret_Shop_Manager.h"
#include "dragon/Dragon_Vale_Manager.h"
#include "campfire/Campfire_Manager.h"
#include "valentine_monster/Valentine_Monster_Active_Manager.h"
#include "world_boss/World_Boss_Active_Manager.h"
#include "honor_arena/Honor_Arena_Active_Manager.h"
#include "active/Active_Manager.h"
#include "active/Active_Content_Manager.h"
#include "icon/Icon_Struct.h"
#include "ranking/Ranking_Manager.h"
#include "Logic_Monitor.h"
#include "arena/Arena_Manager.h"
#include "Logic_Player_Struct.h"
#include "Configurator.h"
#include "DB_Operator.h"
#include "gang/Gang_Manager.h"
#include "Msg_Inner_Enum.h"
#include "expedition/Expedition_Def.h"
#include "expedition_scene/Expedition_Scene_Def.h"
#include "team/Team_Def.h"
#include "Daemon_Server.h"
#include "DB_Merge_Server_Manager.h"

struct Chat_Content;

//测试指令：开启所有图标
static void test_one_key_open_all_icon(Logic_Player *player) {
	if(!player) {
		return;
	}

	MSG_80100400  icon_msg;
	icon_msg.reset();

	const Icon_Config_Map &icon_cfg_map = CONFIG_CACHE_ICON->icon_config();
	for(Icon_Config_Map::const_iterator cfg_it = icon_cfg_map.begin(); cfg_it != icon_cfg_map.end(); ++cfg_it) {
		Icon_Info_Map::iterator find_it = player->icon_detail_no_const().icon_map.find(cfg_it->first);
		if(find_it != player->icon_detail_no_const().icon_map.end()) {
			if(cfg_it->second.is_forever_close && find_it->second.is_ever_opened && find_it->second.status == 0) {
				continue;
			}

			find_it->second.id = cfg_it->first;
			find_it->second.status = 1;
			find_it->second.is_ever_opened = true;

			if(cfg_it->first == 11701 && player->has_hero_id(60220001)) {
				find_it->second.id = cfg_it->first;
				find_it->second.status = 0;
				find_it->second.is_ever_opened = true;
			}
		}
		else {
			Svr_Icon_Info svr_icon_info;
			svr_icon_info.id = cfg_it->first;
			svr_icon_info.status = 1;
			svr_icon_info.is_ever_opened = true;

			if(cfg_it->first == 11701 && player->has_hero_id(60220001)) {
				svr_icon_info.id = cfg_it->first;
				svr_icon_info.status = 0;
				svr_icon_info.is_ever_opened = true;
			}

			player->icon_detail_no_const().icon_map.insert(std::make_pair(svr_icon_info.id, svr_icon_info));
		}

		if(cfg_it->first == 11701 && player->has_hero_id(60220001)) {
			continue;
		}

		Icon_Info iconInfo;
		iconInfo.id = cfg_it->first;
		iconInfo.status = 1;
		icon_msg.icon_list.push_back(iconInfo);

		player->hook_icon_open(cfg_it->first);
	}

	OBJ_SEND_TO_CLIENT(icon_msg, (*player));

	player->icon_detail_no_const().detail_change();
}

//测试指令： 放弃主线任务。 此函数不添加后置任务
static int test_process_abandon(Logic_Player *player, TaskInfo *task_info)
{
	if(!task_info) {
		return ERROR_TASK_NOT_EXIST;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (!task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }

    player->set_task_status(*task_info, TaskInfo::TASK_STATUS_ACCEPTED);
	player->erase_task_listen_item(task_info);

    player->task_accepted_package_set().erase(task_info->__task_id);
    player->task_accepted_monster_set().erase(task_info->__task_id);
    player->task_accepted_fb_set().erase(task_info->__task_id);
    player->task_accepted_arrive_set().erase(task_info->__task_id);
    player->task_accepted_lvl_set().erase(task_info->__task_id);
    player->task_accepted_monster_group_set().erase(task_info->__task_id);
    player->task_accepted_collect_set().erase(task_info->__task_id);
    player->task_accepted_fb_type_set().erase(task_info->__task_id);
    player->task_accepted_win_player_set().erase(task_info->__task_id);
    player->task_accepted_join_gang_set().erase(task_info->__task_id);
    player->task_accepted_ganger_level_set().erase(task_info->__task_id);
    player->task_accepted_player_all_equip_set().erase(task_info->__task_id);
    player->task_acccepted_body_equip_set().erase(task_info->__task_id);
    player->task_accepted_body_equip_level_set().erase(task_info->__task_id);
    player->task_accepted_equip_level_set().erase(task_info->__task_id);
    player->task_accepted_stone_level_set().erase(task_info->__task_id);
    player->task_accepted_buy_goods_set().erase(task_info->__task_id);
    player->task_accepted_get_hero_set().erase(task_info->__task_id);
    player->task_accepted_fight_set().erase(task_info->__task_id);
    player->task_accepted_commit_consume_item_set().erase(task_info->__task_id);
    player->task_accepted_deliver_letter_set().erase(task_info->__task_id);
    player->task_accepted_help_other_complete_team_scene_set().erase(task_info->__task_id);
    player->task_accepted_sell_item_set().erase(task_info->__task_id);
    player->task_accepted_limit_time_set().erase(task_info->__task_id);
    player->task_accepted_arena_set().erase(task_info->__task_id);
    player->task_accepted_chat_with_friend_set().erase(task_info->__task_id);
    player->task_accepted_send_mail_set().erase(task_info->__task_id);
    player->task_accepted_team_scene_or_pvp_set().erase(task_info->__task_id);
    player->task_accepted_browse_other_player_info_set().erase(task_info->__task_id);
    player->task_accepted_npc_talk_set().erase(task_info->__task_id);
    player->task_accepted_arena_level_set().erase(task_info->__task_id);
    player->task_accepted_equip_crimson_level_set().erase(task_info->__task_id);
	player->task_accepted_arena_all_set().erase(task_info->__task_id);
	player->task_accepted_create_dragon_vale_building_set().erase(task_info->__task_id);
	player->task_accepted_gain_dragon_vale_builiding_production_set().erase(task_info->__task_id);
	player->task_accepted_wake_up_hero_to_quality_level_set().erase(task_info->__task_id);
	player->task_accepted_keep_hero_to_level_set().erase(task_info->__task_id);
	player->task_accepted_compose_equip_set().erase(task_info->__task_id);
	player->task_accepted_hero_dream_scene_set().erase(task_info->__task_id);
	player->task_accepted_ganger_skill_level_set().erase(task_info->__task_id);
	player->task_accepted_war_set().erase(task_info->__task_id);
    player->task_accepted_war_all_set().erase(task_info->__task_id);
    player->task_accepted_kill_enemy_in_war_set().erase(task_info->__task_id);
    player->task_accepted_single_arena_all_set().erase(task_info->__task_id);
    player->task_accepted_single_arena_win_set().erase(task_info->__task_id);
    player->task_accepted_team_arena_all_set().erase(task_info->__task_id);
    player->task_accepted_team_arena_win_set().erase(task_info->__task_id);
    player->task_accepted_ganger_bless_buf_set().erase(task_info->__task_id);
    player->task_accepted_rob_other_dragon_vale_all_set().erase(task_info->__task_id);
    player->task_accepted_dragon_vale_step_foot_all_set().erase(task_info->__task_id);
    player->task_accepted_activeness_of_today_set().erase(task_info->__task_id);
    player->task_accepted_dragon_vale_building_upgrade_set().erase(task_info->__task_id);
    player->task_accepted_limit_time_kill_boss_set().erase(task_info->__task_id);
    player->task_accepted_dragon_keep_all_set().erase(task_info->__task_id);
    player->task_accepted_dragon_keep_success_set().erase(task_info->__task_id);
    player->task_accepted_donate_to_gang_set().erase(task_info->__task_id);
    player->task_accepted_present_buf_to_other_set().erase(task_info->__task_id);
    player->task_accepted_finish_reward_task_num_set().erase(task_info->__task_id);
    player->task_accepted_keep_dragon_to_level_set().erase(task_info->__task_id);
    player->task_accepted_knight_trial_set().erase(task_info->__task_id);
    player->task_accepted_suit_set().erase(task_info->__task_id);
    player->task_accepted_role_pull_on_equip_set().erase(task_info->__task_id);
    player->task_accepted_hero_pull_on_equip_set().erase(task_info->__task_id);
    player->task_accepted_activate_wing_set().erase(task_info->__task_id);
    player->task_accepted_get_quality_rune_set().erase(task_info->__task_id);
    player->task_accepted_pass_kngiht_trials_set().erase(task_info->__task_id);
    player->task_accepted_have_hero_set().erase(task_info->__task_id);
    player->task_accepted_have_hero_quality_set().erase(task_info->__task_id);
    player->task_accepted_first_pass_scene_set().erase(task_info->__task_id);
    player->task_accepted_dragon_born_set().erase(task_info->__task_id);
    player->task_accepted_horse_star_level_set().erase(task_info->__task_id);
    player->task_accepted_have_item_but_no_erase_set().erase(task_info->__task_id);
    player->task_accepted_have_equip_but_no_erase_set().erase(task_info->__task_id);
    player->task_accepted_same_gang_challenge_all_set().erase(task_info->__task_id);
    player->task_accepted_main_scene_npc_battle_set().erase(task_info->__task_id);
    player->task_accepted_equip_card_all_set().erase(task_info->__task_id);
    player->task_accepted_equip_card_by_position_set().erase(task_info->__task_id);
    player->task_accepted_card_upgrade_to_level_all_set().erase(task_info->__task_id);
    player->task_accepted_card_upgrade_to_level_by_position_set().erase(task_info->__task_id);
    player->task_accepted_lottery_draw_set().erase(task_info->__task_id);
    player->task_accepted_equip_jewelry_set().erase(task_info->__task_id);
    player->task_accepted_invite_friend_step_foot_set().erase(task_info->__task_id);
    player->task_accepted_kill_group_num_set().erase(task_info->__task_id);
    player->task_accepted_did_reward_task_num_set().erase(task_info->__task_id);
    player->task_accepted_alchemy_num_set().erase(task_info->__task_id);
    player->task_accepted_did_reward_task_num_today_set().erase(task_info->__task_id);
    player->task_accepted_remote_scene_any_pass_set().erase(task_info->__task_id);
    player->task_accepted_knight_trial_any_pass_set().erase(task_info->__task_id);

    player->notify_task_del(*task_info);
    player->tasker_detail_no_const().task_submited_once_.insert(task_info->__task_id);
    player->unbind_task(task_info->__task_id);
    player->tasker_detail_no_const().save_tick();

    return 0;
}

/*
 *  测试命令（放弃当前主线任务、接指定主线任务）
 */
static int  test_process_accept_quickly(Logic_Player *player, int task_id) {
	if(!player) {
		MSG_USER("accept task role_name=%s, role_id=%d, task_id=%d\n",
				player->role_name().c_str(), player->role_id(), task_id);
		return ERROR_CLOSE_CLIENT_ROLE_OFFLINEING;
	}


    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
    if (!task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }

    if (TaskInfo::TASK_GTYPE_MAINLINE != task_json->task_type &&
    		TaskInfo::TASK_GTYPE_BRANCH != task_json->task_type) {
    	return ERROR_NOT_MAINLINE_OR_BRANCH_LINE;
    }

    if(player->find_task(task_id)) {
    	return ERROR_TASK_ACCEPTED;
    }

    for (TaskMap::iterator it = player->tasker_detail_no_const().task_map_.begin();
    		it != player->tasker_detail_no_const().task_map_.end(); ++it) {
    	if (it->second.is_main_task() && TaskInfo::TASK_GTYPE_MAINLINE == task_json->task_type && task_id != it->first) {
    	    test_process_abandon(player, &(it->second));
    	    player->tasker_detail_no_const().save_tick();
			break;
    	}

    	if (it->second.is_branch_task() && TaskInfo::TASK_GTYPE_BRANCH == task_json->task_type && task_id != it->first) {
    	    test_process_abandon(player, &(it->second));
    	    player->tasker_detail_no_const().save_tick();
			break;
    	}
    }

    if(task_json->job && task_json->job != player->career()) {
    	return ERROR_TASK_NOT_SUIT_CAREER;
    }

    TaskInfo task_info;
	player->init_task(task_id, *task_json, task_info);
	if (!task_info.__task_id)
	{
		return ERROR_TASK_NOT_EXIST;
	}

//    if (!task_info.is_acceptable()) {
//    	return ERROR_TASK_NOT_ACCEPT;
//    }

    player->tasker_detail_no_const().task_submited_once_.erase(task_id);

	TaskInfo *task_info_ptr = player->find_task(task_id);
	if (0 == task_info_ptr) {
    	return ERROR_TASK_NOT_EXIST;
	}

	//前置任务做完，当前任务不可接，给客户端发送不可接状态
	if(task_info_ptr->is_visible() && task_json->precondition != 0
			&& player->tasker_detail_no_const().task_submited_once_.find(task_json->precondition)
			  != player->tasker_detail_no_const().task_submited_once_.end()) {
		player->notify_task_add(*task_info_ptr, *task_json);
		return 0;
	}

	player->process_accept_imp(task_info.__task_id);
	return 0;
}

//测试指令： 处理完成某个任务，置为可提交状态，或者自动提交任务
static int test_process_finish_task(Logic_Player *player, int task_id) {
	if(!player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}

    TaskMap::iterator it = player->tasker_detail_no_const().task_map_.find(task_id);
	if(it == player->tasker_detail_no_const().task_map_.end()) {
		return ERROR_TASK_NOT_EXIST;
	}

	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}

/*
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = it->second.task_condition(i);
		if (task_cond == 0)
		{
			continue;
		}

		if (!player->task_execute_type_is_need_listen_item(task_json->execute[i].type)) {
			continue;
		}

		int item_id = task_json->execute[i].id;
		int item_num = task_json->execute[i].amount;
		if(!CONFIG_CACHE_ITEM->find_item(task_json->execute[i].id)) {//execute[i].id: 物品id
			continue;
		}

		Item_Detail item(item_id,
						item_num,
						 static_cast<Item_Detail::Bind_Type>(task_json->execute[i].param1));//execute[i].param1:是否绑定
		player->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
				Gain_Item_DM_Info(Pack::ITEM_GAIN_TASK));

		task_cond->__current_value = task_json->execute[i].amount;
	}//end for
*/

	it->second.__is_req_fast_finish = true;
	player->process_finish_imp(&(it->second), true);

	return 0;
}

int Logic_Player::echo_test_result(std::string result, role_id_t value) {
	MSG_83000000 msg;
	msg.chat_type = 100;
	std::stringstream stream;
	stream <<  result;
	if (value != -1)
		stream <<  value;

	Chat_Content content;
	content.text = stream.str();
	msg.content.push_back(content);

	Block_Buffer buf;
	msg.serialize(buf);
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Logic_Player::test_command(std::string cmd_string) {
	std::vector<std::string> cmd_tok;

	std::istringstream cmd_stream(cmd_string);
	std::copy(std::istream_iterator<std::string>(cmd_stream),
			std::istream_iterator<std::string>(), std::back_inserter(cmd_tok));

	std::cout << "cmd_string = [" << (cmd_string) << "]" << std::endl;
	if (cmd_tok.empty())
		return 0;

	// 0:关闭 1:全开 2:开放部分命令
	int command_mode = CONFIG_CACHE->server_maintainer_cache().command_mode;
	if (command_mode == 1) {
	} else if (command_mode == 2) {
		if (!CONFIG_CACHE->server_maintainer_cache().command.count(cmd_tok[0])) {
			return 0;//echo_test_result("this command not able");
		}
	} else {
		return echo_test_result("test command not able");
	}

	if(cmd_tok[0] == "like") {
		LOGIC_PUBLIC->send_facebook_share_mail(player_self()->role_id(), 2);
	}
	if(cmd_tok[0] == "share") {
		LOGIC_PUBLIC->send_facebook_share_mail(player_self()->role_id(), 1);
	}
	if(cmd_tok[0] == "invite") {
		player_self()->set_inv_time(Time_Value::gettimeofday());
	}

	if(cmd_tok[0] == "fcm_pass") {
		req_fcm_pass(0);
	}

	if(cmd_tok[0] == "reward_back") {
		player_self()->test_all_reward_back_sub_day();
	}

	if(cmd_tok[0] == "invest_plan") {
		if(cmd_tok.size() >= 2) {
			if(cmd_tok[1] == "refresh") {
				Time_Value now = Time_Value::gettimeofday();
				((Invest_Plan*)this)->trigger_daily_six(now);
			}
		}
	}

	if(cmd_tok[0] == "ring_task")  {
		if(cmd_tok.size() == 1) {
			player_self()->refresh_open_ring_task();
		}

		if(cmd_tok.size() >= 3) {
			int op = atoi(cmd_tok[1].c_str());
			if(op == 1) {
				uint32_t ring_num = atoi(cmd_tok[2].c_str());
				player_self()->test_update_ring_num(ring_num);
			}

			if(op == 2) {
				uint32_t task_id = atoi(cmd_tok[2].c_str());
				player_self()->test_refresh_ring_task(task_id);
			}

			if(op == 3) {
				uint32_t ring_num = atoi(cmd_tok[2].c_str());
				player_self()->test_refresh_ring_task__ring_num(ring_num);
			}
		}
	}

	if(cmd_tok[0] == "l_clean_mem") {
		DB_View::ID_Player_View_Map::iterator it;
		for(it = LOGIC_MONITOR->db_view()->id_player_view_map.begin();
				it != LOGIC_MONITOR->db_view()->id_player_view_map.end(); ++it){
			POOL_MANAGER->push_player_db_view(it->second);
		}
		LOGIC_MONITOR->db_view()->id_player_view_map.clear();
		LOGIC_MONITOR->db_view()->name_player_view_map.clear();
		LOGIC_MONITOR->db_view()->account_player_view_map.clear();
		LOGIC_MONITOR->db_view()->account_role_view_map.clear();

		GANG_MANAGER->clean_ganger_detail();
		ARENA_MANAGER->clean_arena_mem();
		return 0;
	}

	if(cmd_tok[0] == "l_server_status") {
		monitor()->message_unit()->server_status();
		return 0;
	}

	if(cmd_tok[0] == "debug_msg") {
		MSG_80100501 msg;
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
	    buf.finish_message();
	    this->send_to_client(buf, true);
	}

	if(cmd_tok[0] == "size") {
		union Addon {
			Equip_Addon_Detail equip;
			Rune_Stone_Addon_Detail rune_stone;
			Card_Addon_Detail card;
		};

		Addon add;
		echo_test_result("equip size:", sizeof(Equip_Addon_Detail));
		echo_test_result("rune_stone size:", sizeof(Rune_Stone_Addon_Detail));
		echo_test_result("card size:", sizeof(Card_Addon_Detail));
		echo_test_result("Addon size:", sizeof(add));

		echo_test_result("player size:", sizeof(*this));
		echo_test_result("player data size:", sizeof(Logic_Player_Detail));
		echo_test_result("item size:", sizeof(Item_Detail));
		echo_test_result("Gang_DB_View size:", sizeof(Gang_DB_View));
		echo_test_result("Team size:", sizeof(Team));
		echo_test_result("Arena_Rank_Player size:", sizeof(Arena_Rank_Player));

		int db_view_size = 0;
		for (DB_View::ID_Player_View_Map::iterator it = monitor_->db_view()->id_player_view_map.begin(); it != monitor_->db_view()->id_player_view_map.end(); ++it) {
			db_view_size += it->second->db_view_size();
		}
		echo_test_result("db_view size:", db_view_size);

		return 0;
	}

	if(cmd_tok[0] == "dragon") {
		if(cmd_tok.size() >= 2) {
			if(cmd_tok[1] == "special") {
				DRAGON_VALE_MANAGER->process_refresh_special();
			}

			if(cmd_tok[1] == "reset") {
				DRAGON_VALE_MANAGER->process_dragon_vale_reset();
			}

			if(cmd_tok[1] == "add") {
				if(cmd_tok.size() >= 3) {
					int64_t role_id = (int64_t)atoll(cmd_tok[2].c_str());
					DRAGON_VALE_MANAGER->process_refresh_special_test(role_id, true);
				}
			}

			if(cmd_tok[1] == "sub") {
				if(cmd_tok.size() >= 3) {
					int64_t role_id = (int64_t)atoll(cmd_tok[2].c_str());
					DRAGON_VALE_MANAGER->process_refresh_special_test(role_id, false);
				}
			}

			if(cmd_tok[1] == "born") {
				dragon_born_right_now_test();
			}
		}
	}

	if(cmd_tok[0] == "task_refresh") {
		if(cmd_tok.size() >= 3) {
			if(cmd_tok[1] == "day") {
				uint8_t time = (uint8_t)atoi(cmd_tok[2].c_str());
				Tasker::refresh_day_task(time);
			}
		}
		if(cmd_tok.size() >= 4) {
			if(cmd_tok[1] == "week") {
				uint8_t wday = (uint8_t)atoi(cmd_tok[2].c_str());
				uint8_t time = (uint8_t)atoi(cmd_tok[3].c_str());
				Tasker::refresh_week_task(wday, time);
			}
		}
	}

	if (cmd_tok[0] == "alchemy") {
		this->req_alchemy_info();
	}

	if (cmd_tok[0] == "log_abort") {
		int amount = Lib_Log::instance()->check_core_amount();
		echo_test_result("amount:", amount);
		LOG_TRACE_ABORT("");
	}

	if (cmd_tok[0] == "msg_abort") {
		MSG_TRACE_ABORT("");
	}

	if(cmd_tok[0] == "revipann") {
		ACTIVE_CONTENT_MANAGER->reset_vip_announce_ed_level();
	}

	if(cmd_tok[0] == "ast") {
		player_self()->req_open_activity_exchange_mall();
	}

	if (cmd_tok[0] == "total") {
		if (cmd_tok.size() == 1) {
			echo_test_result("logic total:", LOGIC_MONITOR->player_role_id_map().size());
		}
	}

	if(cmd_tok[0] == "delmail") {
		player_self()->del_all_mail();
	}

	if(cmd_tok[0] == "vip_daily") {
		player_self()->Vip::trigger_daily_zero(Time_Value::gettimeofday());
	}

	if(cmd_tok[0] == "count_act_rank") {
		if (cmd_tok.size() == 2) {
			Time_Value now = Time_Value::gettimeofday();
			ACTIVE_CONTENT_MANAGER->reset_count_flag();
			ACTIVE_MANAGER->set_action_count_rank_active((uint32_t)atoi(cmd_tok[1].c_str()) , now);
		}
	}

	if(cmd_tok[0] == "close_act") {
		if (cmd_tok.size() == 2) {
			Time_Value now = Time_Value::gettimeofday();
			ACTIVE_MANAGER->inner_close_active((uint32_t)atoi(cmd_tok[1].c_str()) , now);
		}
	}

	if(cmd_tok[0] == "close_act_un") {
		if (cmd_tok.size() == 2) {
			Time_Value now = Time_Value::gettimeofday();
			ACTIVE_MANAGER->inner_close_active((uint32_t)atoi(cmd_tok[1].c_str()) , now, true);
		}
	}

	if(cmd_tok[0] == "php_open_act") {
		if (cmd_tok.size() == 4) {
			ACTIVE_MANAGER->php_open_active((uint32_t)atoi(cmd_tok[1].c_str()), (uint32_t)atoi(cmd_tok[2].c_str()) , (uint32_t)atoi(cmd_tok[3].c_str()));
		}
	}

	if(cmd_tok[0] == "oard") {
		if(cmd_tok.size() >= 2) {
			player_self()->sub_n_day_actid_recid_last_refresh_time(atoi(cmd_tok[1].c_str()));
		} else {
			player_self()->sub_n_day_actid_recid_last_refresh_time(1);
		}
	}

	if(cmd_tok[0] == "oaald") {
		if(cmd_tok.size() >= 2) {
			player_self()->oper_activity_add_login_day_on_test_cmd(atoi(cmd_tok[1].c_str()));
		} else {
			player_self()->oper_activity_add_login_day_on_test_cmd(1);
		}
	}

	if(cmd_tok[0] == "php_close_act") {
		if (cmd_tok.size() == 3) {
			ACTIVE_MANAGER->php_close_active((uint32_t)atoi(cmd_tok[1].c_str()), (uint32_t)atoi(cmd_tok[2].c_str()));
		}
	}

	if(cmd_tok[0] == "open_act") {
		if (cmd_tok.size() == 2) {
			Time_Value now = Time_Value::gettimeofday();
			ACTIVE_MANAGER->gm_open_active((uint32_t)atoi(cmd_tok[1].c_str()) , now);
		}
	}

	if(cmd_tok[0] == "loop") {
		if (cmd_tok.size() == 2) {
			int times = (int)atoi(cmd_tok[1].c_str());
			Time_Value t = Time_Value::gettimeofday() + Time_Value(times);
			while (true) {
				if (Time_Value::gettimeofday() > t) {
					break;
				}
			}
		}
	}

	if(cmd_tok[0] == "random_1") {
		if (cmd_tok.size() == 3) {
			int times = (int)atoi(cmd_tok[1].c_str());
			int percent = (int)atoi(cmd_tok[2].c_str());
			int loop = 0;
			for (int i = 0; i < times; ++i) {
				int rand_ = random()%100;
				if (rand_ < percent)
					loop++;
			}
			echo_test_result("result:", loop);

		}
	}

	if(cmd_tok[0] == "random_2") {
		if (cmd_tok.size() == 3) {
			int times = (int)atoi(cmd_tok[1].c_str());
			int percent = (int)atoi(cmd_tok[2].c_str());
			if (percent > 100) return 0;
			if (times > 10000 || times < 0) return 0;
			int loop = 0;
			for (int i = 0; i < times; ++i) {
				srand(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec() + i);
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec() + i);
				int rand_ = random()%100;
				if (rand_ < percent)
					loop++;
			}
			echo_test_result("result:", loop);

		}
	}


	if(cmd_tok[0] == "has_seven") {
		player_self()->test_set_seven_is_receive();
	}

	if(cmd_tok[0] == "rank") {
		if(cmd_tok.size() == 2) {
			RANKING_MANAGER->test_refresh_ranking(atoi(cmd_tok[1].c_str()));
		}
	}
	if(cmd_tok[0] == "knight") {
		player_self()->Knight_Trials::trigger_daily_six(Time_Value::gettimeofday());
	}
	if(cmd_tok[0] == "knight_fighter") {
		player_self()->req_knight_trials_enter_fighter();
	}
	if(cmd_tok[0] == "knight_reset") {
		player_self()->req_up_hide_knight_trials_palyer();
	}
	if(cmd_tok[0] == "knight_get") {
		player_self()->req_receive_knight_trials_reward_box();
	}
	if(cmd_tok[0] == "charm") {
		if(cmd_tok.size() == 2) {
			player_self()->add_charm(atoi(cmd_tok[1].c_str()));
		}
	}

	if(cmd_tok[0] == "oper_num") {
		if(cmd_tok.size() >= 3) {
			player_self()->oper_activity_listen_type_common(atoi(cmd_tok[1].c_str()), atoi(cmd_tok[2].c_str()));
		}
	}

	if(cmd_tok[0] == "vip") {
		if(cmd_tok.size() == 2) {
			player_self()->open_vip(atoi(cmd_tok[1].c_str()), 1);
		} else if(cmd_tok.size() == 3) {
			if(cmd_tok[1] == "set") {
				Time_Value now = Time_Value::gettimeofday();
				now.sec(now.sec() + atoi(cmd_tok[2].c_str())*60);
				player_self()->set_vip_over_time(now);
				player_self()->set_vip_exp_overtime(now);
				player_self()->send_vip_info_client(4);
			}
			if(cmd_tok[1] == "setlevel") {
				uint32_t old_level = player_self()->vip();
				player_self()->set_vip_level(atoi(cmd_tok[2].c_str()));
				player_self()->vip_level_change(player_self()->vip(), old_level);
				player_self()->send_vip_info_client(4);
			}
			if(cmd_tok[1] == "settle") {
				player_self()->test_set_last_settle_grow(atoi(cmd_tok[2].c_str()));
			}
		}
	}
	if(cmd_tok[0] == "no_check_in") {
		player_self()->test_set_today_is_check_in();
	}
	if(cmd_tok[0] == "vip_six") {
		Time_Value now = Time_Value::gettimeofday();
		player_self()->test_trigger_daily_six(now);
	}

	if(cmd_tok[0] == "viptree") {
		if(cmd_tok.size() >= 4) {
			player_self()->listen_vip_reward(atoi(cmd_tok[1].c_str()), atoi(cmd_tok[2].c_str()), atoi(cmd_tok[3].c_str()));
		}
	}
	if(cmd_tok[0] == "turn") {
		if(cmd_tok.size() >= 2) {
			if(cmd_tok[1] == "open") {
				player_self()->req_vip_turntable_open();
			} else if(cmd_tok[1] == "lottery") {
				if(cmd_tok.size() >= 3) {
					player_self()->req_vip_turntable_lottery(atoi(cmd_tok[2].c_str()));
				}
			}
		}
	}
	if (cmd_tok[0] == "team_zero") {
		Time_Value now = Time_Value::gettimeofday();
		Teamer::trigger_daily_six(now);
	}
	if (cmd_tok[0] == "team") {
		if(cmd_tok.size() >= 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());
			switch(i) {
			case 0: {
				if (cmd_tok.size() >= 5) {
					int type = atoi(cmd_tok[2].c_str());
					int scene_id = atoi(cmd_tok[3].c_str());
					int degree = atoi(cmd_tok[4].c_str());
					player_self()->create_team(scene_id, "", type, degree);
				}
				break;
			}
			case 1: {
				player_self()->req_set_ready_state(1);
				break;
			}
			case 2: {
				echo_test_result("logic team_id:", player_self()->teamer_detail().team_id);
				break;
			}
			default:
				break;
			}
		}
	}


	if(cmd_tok[0] == "cov") {
		if(cmd_tok.size() >= 2) {
			if(cmd_tok[1] == "o_all") {
				req_buy_covenant(2, 0);
			}else if(cmd_tok[1] == "x_all") {
				req_buy_covenant(3, 0);
			}else if(cmd_tok[1] == "open") {
				if(cmd_tok.size() >= 3){
					req_buy_covenant(1, atoi(cmd_tok[2].c_str()));
				}
			}else if(cmd_tok[1] == "sub") {
				if(cmd_tok.size() >= 4){
					player_self()->test_covenant(atoi(cmd_tok[2].c_str()), atoi(cmd_tok[3].c_str()) * -1);
				}
			}else if(cmd_tok[1] == "add") {
				if(cmd_tok.size() >= 4){
					player_self()->test_covenant(atoi(cmd_tok[2].c_str()), atoi(cmd_tok[3].c_str()));
				}
			} else if(cmd_tok[1] == "set") {
				if(cmd_tok.size() >= 4){
					player_self()->test_covenant(atoi(cmd_tok[2].c_str()), atoi(cmd_tok[3].c_str()), 1);
						//iter->second.over_time.sec(now.sec() + atoi(cmd_tok[3].c_str()) * 60);
				}
			}
		}
	}

	if (cmd_tok[0] == "81110001") {
		MSG_81110001 msg;
		NPC_Appear_Info_Task task;
		task.funcName = "aaa";
		msg.npc_info.push_back(task);
		THIS_SEND_TO_CLIENT(msg);
	}

	if (cmd_tok[0] == "stream") {
		MSG_STREAM("`fuck`(`a`, `b`) VALUES (%d,%d)", 1, 2);
	}

	if (cmd_tok[0] == "mall_list") {
		req_fetch_mall_list();
	}

	if (cmd_tok[0] == "hero_recruit") {
		if(cmd_tok.size() == 3){
			int hero_id = atoi(cmd_tok[1].c_str());
			int use_diamond = atoi(cmd_tok[2].c_str());
			int ret = req_hero_recruit(hero_id, use_diamond);

			// 第一次招募时出战
			if (!ret && req_hero_amount() == 1) {
				set_hero_out_state(1);
				req_hero_set_fight(1, 1, false);
			}
		}

	}

	if (cmd_tok[0] == "help") {
		LOGIC_PUBLIC->set_reboot_time(Time_Value::zero);
		std::stringstream str_stream;
		str_stream << "@reboot 		重启,可被任意命令打断<b><br>";
		str_stream << "@level 10  	升到10级";
		echo_test_result(str_stream.str());
	}

	if (cmd_tok[0] == "id") {
		echo_test_result("", role_id());
	}

	if (LOGIC_PUBLIC->reboot_time() != Time_Value::zero) {
		LOGIC_PUBLIC->set_reboot_time(Time_Value::zero);
		std::stringstream str_stream;
		str_stream << " 重启被[" << this->base_detail().account_info.account_name << "]打断!!!";
		Block_Buffer buf;
		MSG_83000000 msg;
		msg.chat_type = 100;
		Chat_Content content;
		content.type = 0;
		content.text = str_stream.str();
		msg.content.push_back(content);
		MAKE_MSG_BUF(msg, buf);
		monitor_->send_to_all_client(buf);
	}

	if (cmd_tok[0] == "open_time") {
		if (cmd_tok.size() == 1) {
			Date_Time date(CONFIG_CACHE->server_maintainer_cache().game_open_time);
			std::stringstream stream;
			stream 	<< "Open Time: " << date.year() << "-" <<  date.month() << "-" <<  date.day() << " " <<
					date.hour() << ":" <<  date.minute() << ":" <<  date.second();
			this->echo_test_result(stream.str());
		} else if (cmd_tok.size() == 7) {
			int year = abs(atoi(cmd_tok[1].c_str()));
			int month = abs(atoi(cmd_tok[2].c_str()));
			int day = abs(atoi(cmd_tok[3].c_str()));
			int hour = abs(atoi(cmd_tok[4].c_str()));
			int minute = abs(atoi(cmd_tok[5].c_str()));
			int second = abs(atoi(cmd_tok[6].c_str()));

			if (year < 2000 || year > 3000
					|| month < 1 || month > 12
					||  day < 1 || day > 31
					||  hour < 0 || hour > 23
					||  minute < 0 || minute > 59
					||  second < 0 || second > 60) {
				this->echo_test_result("时间格式错误!!!");
			}
			Date_Time open_data;
			open_data.year(year);
			open_data.month(month);
			open_data.day(day);
			open_data.hour(hour);
			open_data.minute(minute);
			open_data.second(second);

			if (CONFIG_CACHE->server_maintainer_cache().game_open_time.sec() == open_data.time_sec()) {
				this->echo_test_result("设置的开服时间和当前开服时间一致");
				return 0;
			}

			int result = DAEMON_SERVER_INSTANCE->change_open_time(open_data.time_sec());
			if (result == 0) {
				if (LOGIC_PUBLIC->reboot_time() != Time_Value::zero) return 0;

				std::stringstream stream;
							stream 	<< "Open Time: " << open_data.year() << "-" <<  open_data.month() << "-" <<  open_data.day() << " " <<
									open_data.hour() << ":" <<  open_data.minute() << ":" <<  open_data.second();

				this->echo_test_result("[" + this->base_detail().account_info.account_name + "]新开服时间:" + stream.str());

				LOGIC_PUBLIC->set_reboot_time(Time_Value::gettimeofday());

				Block_Buffer buf;
				buf.make_message(TIMER_REBOOT_SERVER);
				buf.write_string("[" + this->base_detail().account_info.account_name + "]新开服时间:" + stream.str());
				buf.finish_message();
				GLOBAL_TIMER->register_loop_handler(buf, Time_Value(0), monitor()->message_unit(), 1);
			} else {
				this->echo_test_result("改文件开服时间错误");
			}
		} else {
			this->echo_test_result("时间格式错误!!!");
		}
	}

	if (cmd_tok[0] == "reboot") {
		if (LOGIC_PUBLIC->reboot_time() != Time_Value::zero) return 0;

		int delay_time = 15;
		if (cmd_tok.size() == 2) {
			delay_time = abs(atoi(cmd_tok[1].c_str()));
		}

		this->echo_test_result("[" + this->base_detail().account_info.account_name + "]重启中,输入任意命令可打断!");

		LOGIC_PUBLIC->set_reboot_time(Time_Value::gettimeofday() + Time_Value(delay_time));

		Block_Buffer buf;
		buf.make_message(TIMER_REBOOT_SERVER);
		buf.write_string("[" + this->base_detail().account_info.account_name + "]重启中,输入任意命令可打断!");
		buf.finish_message();
		GLOBAL_TIMER->register_loop_handler(buf, Time_Value(2), monitor()->message_unit(), (delay_time + 2)/2);
	}

	if (cmd_tok[0] == "scene") {
		if (cmd_tok.size() == 2) {
			int scene_id = abs(atoi(cmd_tok[1].c_str()));
			this->inner_transmit(scene_id, 0, Coord(0, 0), 0);
		}
		if (cmd_tok.size() == 3) {
			int scene_id = abs(atoi(cmd_tok[1].c_str()));
			int scene_order = abs(atoi(cmd_tok[2].c_str()));

			this->inner_transmit(scene_id, scene_order, Coord(0, 0), 0);
		}
	}
	if (cmd_tok[0] == "war_scene_enter") {
		this->req_war_scene_enter(false);
	}
	if (cmd_tok[0] == "war_scene_reset") {
		this->req_war_scene_reset_enter_num();
	}

	if (cmd_tok[0] == "gang_scene") {
		int scene_id = 80206101;
		scene_transmit(scene_id, 0, role_id(), Coord(0, 0), role_name(), 0, 0);
	}

	if (cmd_tok[0] == "expe") {
		if (cmd_tok.size() == 2) {
			int i = atoi(cmd_tok[1].c_str());
			switch(i) {
			case 1: {
				int scene_id = 80216001;
				scene_transmit(scene_id, 0, role_id(), Coord(0, 0), role_name(), 0, 0);
				break;
			}
			default:
				break;
			}
		}
	}
	if (cmd_tok[0] == "expe_ann") {
		create_ann_expedition_start(5);
	}

	if (cmd_tok[0] == "answer_scene") {
		int scene_id = 80302001;
		scene_transmit(scene_id, 0, role_id(), Coord(0, 0), role_name(), 0, 0);
	}

	if (cmd_tok[0] == "l_exp") {
		if (cmd_tok.size() == 2) {
			int exp = abs(atoi(cmd_tok[1].c_str()));
			this->modify_experience(exp);
		}
	}
	else if(cmd_tok[0] == "add_friend"){
		std::vector<std::string> apply_list;
		for(int i=1; i< (int)(cmd_tok.size()); i++)
		{
			apply_list.push_back(cmd_tok[i]);
		}
        //player_self()->friend_fetch_friend_apply_list();

	}

	else if(cmd_tok[0] == "fetch_friend_list"){
		//player_self()->friend_fetch_friend_list(Friend::Fetch_Friend_List_Type::FETCH_FRIEND);
	}

	else if(cmd_tok[0] == "get_mail_list")
	{
		player_self()->fetch_mail_state_list(1, ALL_MAIL);
		player_self()->fetch_mail_state_list(1, HAVE_READ);
		player_self()->fetch_mail_state_list(1, HAVE_NOT_READ);
	}

	else if(cmd_tok[0] == "send_mail")
	{
		Mail_Info mail_info;
		mail_info.reset();
		mail_info.role_name = "t1";
		mail_info.mail_title = "title";
		mail_info.mail_content = "content";
/*		Item_Basic_Info bi;
		bi.amount=1;
		bi.bind=0;
		bi.id=1;
		bi.seq=0;
		mail_info.item_vector.push_back(bi);
*/
		client_send_mail(mail_info);
	} else if(cmd_tok[0] == "sys_mail") {
		LOGIC_PUBLIC->send_mail(1001,1001000000000001);
	}

	if (cmd_tok[0] == "tt1") {
		std::vector<int> vec;
		vec.push_back(1);
		CLEAR_VECTOR(std::vector<int>, vec);
		int i = 0;
		i = 1;
	}

	if (cmd_tok[0] == "task") {
		int i = 0;
		int j = 0;
		int k = 0;
		if (cmd_tok.size() >= 2) {
			i = atoi(cmd_tok[1].c_str());
		}
		if (i == 0) {
			task_get_list();
		}
		else if(i == 6) {
			Time_Value now = Time_Value::gettimeofday();
			Tasker::daily_refresh_reward_task_list(now);
		}

		else if(cmd_tok.size() >= 3) {
			j = atoi(cmd_tok[2].c_str());
			if(j > 0) {
				switch(i) {
				case 1:
				{
					TaskInfo *task_info = this->find_task(j);
					if(task_info) {
						if(task_info->is_main_task() || task_info->is_reward_task()) {
							break;
						}
					}
					process_accept(j);
					break;
				}
				case 2:
					process_abandon(j);
					break;
				case 3:
					process_submit(j);
					break;
				case 4:
					if(cmd_tok.size() >= 4) {
						k = atoi(cmd_tok[3].c_str());
						process_fast_finish(j, k, 0);
					}
					break;
				case 5:
					test_process_accept_quickly(this, j);
					break;

				case 7:
					test_process_finish_task(this, j);
					break;
				case 8:
					return test_process_finish_all_task_befor_id(j);
				default:
					break;
				}
			}
		}
	}

	if (cmd_tok[0] == "plot") {
		if (cmd_tok.size() >= 2) {
			std::string plot_id;
			plot_id = cmd_tok[2];
			MSG_50110008 msg;
			THIS_SEND_TO_CLIENT(msg);
		}
	}

	if (cmd_tok[0] == "item" ) {
		if (cmd_tok.size() == 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());
			Item_Detail item(i, 1, 1);
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item.id_);
			if (! item_config) {
				return -1;
			}
			Pack_Type pack_type = PACK_T_PACKAGE_INDEX;
			if (item_config->rune_stone.rune_type) {
				pack_type = PACK_T_RUNE_STONE_PACK_INDEX;
			}

			if (item.amount_/item_config->overlay >= PACKAGE_ITEM_MAX_AMOUNT) {
				this->echo_test_result("没发成功，数量少点");
				return ERROR_PACK_AMOUNT_LIMIT;
			}

			int ret = pack_insert_item(pack_type, item, Gain_Item_DM_Info(ITEM_GAIN_TEST));
			if (0 != ret) {
				send_err_to_client(ret, 0);
			}
		}
		if (cmd_tok.size() == 3) {
			int i = 0;
			int j = 1;
			i = atoi(cmd_tok[1].c_str());
			j = atoi(cmd_tok[2].c_str());
			Item_Detail item(i, j, 1);
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item.id_);
			if (! item_config) {
				return -1;
			}
			Pack_Type pack_type = PACK_T_PACKAGE_INDEX;
			if (item_config->rune_stone.rune_type) {
				pack_type = PACK_T_RUNE_STONE_PACK_INDEX;
			}
			if (item.amount_/item_config->overlay >= PACKAGE_ITEM_MAX_AMOUNT) {
				this->echo_test_result("没发成功，数量少点");
				return ERROR_PACK_AMOUNT_LIMIT;
			}
			int ret = pack_insert_item(pack_type, item, Gain_Item_DM_Info(ITEM_GAIN_TEST));
			if (0 != ret) {
				return ret;
			}
		}
		if (cmd_tok.size() == 4) {
			int i = 0;
			int j = 1;
			int k = 1;
			i = atoi(cmd_tok[1].c_str());
			j = atoi(cmd_tok[2].c_str());
			k = atoi(cmd_tok[3].c_str());
			Item_Detail item(i, j, k);
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item.id_);
			if (! item_config) {
				return -1;
			}
			Pack_Type pack_type = PACK_T_PACKAGE_INDEX;
			if (item_config->rune_stone.rune_type) {
				pack_type = PACK_T_RUNE_STONE_PACK_INDEX;
			}
			if (item.amount_/item_config->overlay >= PACKAGE_ITEM_MAX_AMOUNT) {
				this->echo_test_result("没发成功，数量少点");
				return ERROR_PACK_AMOUNT_LIMIT;
			}
			int ret = pack_insert_item(pack_type, item, Gain_Item_DM_Info(ITEM_GAIN_TEST));
			if (0 != ret) {
				send_err_to_client(ret, 0);
			}
		}
	}

	if (cmd_tok[0] == "sell" ) {
		//pack_sell_item
		if (cmd_tok.size() >= 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());
			Int_Int_Vec index;
			Int_Int ve;
			ve.val_1 = i;
			ve.val_2 = 1;
			index.push_back(ve);
			this->pack_sell_item(index);
		}
	}

	if (cmd_tok[0] == "cap" ) {
		//
		if (cmd_tok.size() >= 4) {
			int i = 0;
			int j = 0;
			int k = 0;
			i = atoi(cmd_tok[1].c_str());
			j = atoi(cmd_tok[2].c_str());
			k = atoi(cmd_tok[3].c_str());
			pack_add_capacity(i, j, k);
		}
	}


	if (cmd_tok[0] == "trigger_daily" ) {
		if (cmd_tok.size() == 2) {
			int type = atoi(cmd_tok[1].c_str());
			if (type == 1) {
				monitor()->player_trigger_daily_zero();
			} else if (type == 2) {
				monitor()->player_trigger_daily_six();
			} else if (type == 3) {
				monitor()->player_trigger_daily_twenty();
			} else if (type == 4) {
				monitor()->player_trigger_weekly_zero();
			}
		}
	}


	if (cmd_tok[0] == "money") {
		Money_Add_List money_add_list;
		Money_Sub_List money_sub_list;
		if (cmd_tok.size() == 2) {
			int money = 0;
			int money_bak = 0;
			money = atoi(cmd_tok[1].c_str());
			if (money > 0) {
				money_add_list.push_back(
						Money_Add_Info(COPPER, money, Money_DM_Info(MONEY_ADD_GM)));
				money_add_list.push_back(
						Money_Add_Info(BIND_GOLD, money, Money_DM_Info(MONEY_ADD_GM)));
				money_add_list.push_back(
						Money_Add_Info(GOLD, money, Money_DM_Info(MONEY_ADD_GM)));
				money_add_list.push_back(
						Money_Add_Info(DRAGON_SOUL, money, Money_DM_Info(MONEY_ADD_GM)));
				money_add_list.push_back(
						Money_Add_Info(SOULS, money, Money_DM_Info(MONEY_ADD_GM)));
				money_add_list.push_back(
						Money_Add_Info(FRIENDSHIP, money, Money_DM_Info(MONEY_ADD_GM)));
				money_add_list.push_back(
						Money_Add_Info(REPUTATION, money, Money_DM_Info(MONEY_ADD_GM)));
			} else {
				money = abs(money);
				money_bak = player_self()->packer_detail().money_detail.gold > money ? money : player_self()->packer_detail().money_detail.gold;
				money_sub_list.push_back(
						Money_Sub_Info(COPPER_ONLY, money_bak, Money_DM_Info(MONEY_SUB_GM)));
				money_bak = player_self()->packer_detail().money_detail.bind_diamond > money ? money : player_self()->packer_detail().money_detail.bind_diamond;
				money_sub_list.push_back(
						Money_Sub_Info(BIND_GOLD_ONLY, money_bak, Money_DM_Info(MONEY_SUB_GM)));
				money_bak = player_self()->packer_detail().money_detail.diamond > money ? money : player_self()->packer_detail().money_detail.diamond;
				money_sub_list.push_back(
						Money_Sub_Info(GOLD_ONLY, money_bak, Money_DM_Info(MONEY_SUB_GM)));
				money_bak = player_self()->packer_detail().money_detail.dragon_soul > money ? money : player_self()->packer_detail().money_detail.dragon_soul;
				money_sub_list.push_back(
						Money_Sub_Info(DRAGON_SOUL_ONLY, money_bak, Money_DM_Info(MONEY_SUB_GM)));
				money_bak = player_self()->packer_detail().money_detail.souls > money ? money : player_self()->packer_detail().money_detail.souls;
				money_sub_list.push_back(
						Money_Sub_Info(SOULS_ONLY, money_bak, Money_DM_Info(MONEY_SUB_GM)));
				money_bak = player_self()->packer_detail().money_detail.friendship > money ? money : player_self()->packer_detail().money_detail.friendship;
				money_sub_list.push_back(
						Money_Sub_Info(FRIENDSHIP_ONLY, money_bak, Money_DM_Info(MONEY_SUB_GM)));
				money_bak = player_self()->packer_detail().money_detail.reputation > money ? money : player_self()->packer_detail().money_detail.reputation;
				money_sub_list.push_back(
						Money_Sub_Info(REPUTATION_ONLY, money_bak, Money_DM_Info(MONEY_SUB_GM)));
			}
		} else if(cmd_tok.size() >= 3) {
			int code = 0;
			int money = 0;
			code = atoi(cmd_tok[1].c_str());
			money = atoi(cmd_tok[2].c_str());
			if (money > 0) {
				switch(code) {
				case COPPER:
					money_add_list.push_back(
							Money_Add_Info(COPPER, money, Money_DM_Info(MONEY_ADD_GM)));
					break;
				case BIND_GOLD:
					money_add_list.push_back(
							Money_Add_Info(BIND_GOLD, money, Money_DM_Info(MONEY_ADD_GM)));
					break;
				case GOLD:
					money_add_list.push_back(
							Money_Add_Info(GOLD, money, Money_DM_Info(MONEY_ADD_GM)));
					break;
				case DRAGON_SOUL:
					money_add_list.push_back(
							Money_Add_Info(DRAGON_SOUL, money, Money_DM_Info(MONEY_ADD_GM)));
					break;
				case SOULS:
					money_add_list.push_back(
							Money_Add_Info(SOULS, money, Money_DM_Info(MONEY_ADD_GM)));
					break;
				case FRIENDSHIP:
					money_add_list.push_back(
							Money_Add_Info(FRIENDSHIP, money, Money_DM_Info(MONEY_ADD_GM)));
					break;
				case REPUTATION:
					money_add_list.push_back(
							Money_Add_Info(REPUTATION, money, Money_DM_Info(MONEY_ADD_GM)));
					break;
				default:
					send_err_to_client(ERROR_ITEM_NOT_EXIST, 0);
					break;
				}
			} else {
				money = abs(money);
				switch(code) {
				case COPPER:
					money = player_self()->packer_detail().money_detail.gold > money ? money : player_self()->packer_detail().money_detail.gold;
					money_sub_list.push_back(
							Money_Sub_Info(COPPER_ONLY, abs(money), Money_DM_Info(MONEY_SUB_GM)));
					break;
				case BIND_GOLD:
					money = player_self()->packer_detail().money_detail.bind_diamond > money ? money : player_self()->packer_detail().money_detail.bind_diamond;
					money_sub_list.push_back(
							Money_Sub_Info(BIND_GOLD_ONLY, abs(money), Money_DM_Info(MONEY_SUB_GM)));
					break;
				case GOLD:
					money = player_self()->packer_detail().money_detail.diamond > money ? money : player_self()->packer_detail().money_detail.diamond;
					money_sub_list.push_back(
							Money_Sub_Info(GOLD_ONLY, abs(money), Money_DM_Info(MONEY_SUB_GM)));
					break;
				case DRAGON_SOUL:
					money = player_self()->packer_detail().money_detail.dragon_soul > money ? money : player_self()->packer_detail().money_detail.dragon_soul;
					money_sub_list.push_back(
							Money_Sub_Info(DRAGON_SOUL_ONLY, abs(money), Money_DM_Info(MONEY_SUB_GM)));
					break;
				case SOULS:
					money = player_self()->packer_detail().money_detail.souls > money ? money : player_self()->packer_detail().money_detail.souls;
					money_sub_list.push_back(
							Money_Sub_Info(SOULS_ONLY, abs(money), Money_DM_Info(MONEY_SUB_GM)));
					break;
				case FRIENDSHIP:
					money = player_self()->packer_detail().money_detail.friendship > money ? money : player_self()->packer_detail().money_detail.friendship;
					money_sub_list.push_back(
							Money_Sub_Info(FRIENDSHIP_ONLY, abs(money), Money_DM_Info(MONEY_SUB_GM)));
					break;
				case REPUTATION:
					money = player_self()->packer_detail().money_detail.reputation > money ? money : player_self()->packer_detail().money_detail.reputation;
					money_sub_list.push_back(
							Money_Sub_Info(REPUTATION_ONLY, abs(money), Money_DM_Info(MONEY_SUB_GM)));
					break;
				default:
					send_err_to_client(ERROR_ITEM_NOT_EXIST, 0);
					break;
				}
			}
		} else {
			money_add_list.push_back(
					Money_Add_Info(COPPER, 10000, Money_DM_Info(MONEY_ADD_GM)));
			money_add_list.push_back(
					Money_Add_Info(BIND_GOLD, 10000,
							Money_DM_Info(MONEY_ADD_GM)));
			money_add_list.push_back(
					Money_Add_Info(GOLD, 10000, Money_DM_Info(MONEY_ADD_GM)));
			money_add_list.push_back(
					Money_Add_Info(DRAGON_SOUL, 10000, Money_DM_Info(MONEY_ADD_GM)));
			money_add_list.push_back(
					Money_Add_Info(SOULS, 10000, Money_DM_Info(MONEY_ADD_GM)));
			money_add_list.push_back(
					Money_Add_Info(FRIENDSHIP, 10000, Money_DM_Info(MONEY_ADD_GM)));
		}
		int ret = 0;
		if (!money_add_list.empty()) {
			ret = this->pack_add_money(money_add_list, MONEY_OPT_REAL);
		}
		if (!money_sub_list.empty()) {
			ret = this->pack_sub_money(money_sub_list, MONEY_OPT_REAL);
		}
	}

	if (cmd_tok[0] == "hero_add" ) {
		if (cmd_tok.size() >= 2) {
			int hero_id = atoi(cmd_tok[1].c_str());
			//int hero_index = atoi(cmd_tok[2].c_str());
			hero_add(hero_id);
			player_self()->other_module_notify_icon_to_close_icon(117);
			return 0;
		}
	}

	if (cmd_tok[0] == "hero_erase" ) {
		if (cmd_tok.size() >= 2) {
			int hero_index = atoi(cmd_tok[1].c_str());
			hero_erase(hero_index);
		}
	}

	if (cmd_tok[0] == "hero_level_up" ) {
		if (cmd_tok.size() >= 2) {
			int hero_index = atoi(cmd_tok[1].c_str());
			req_hero_level_up(hero_index);
		}
	}

	if (cmd_tok[0] == "hero_out" ) {
		if (cmd_tok.size() >= 2) {
			//int hero_index = atoi(cmd_tok[1].c_str());
			int is_out = atoi(cmd_tok[2].c_str());
			req_set_hero_out_state(is_out);
		}
	}

	if (cmd_tok[0] == "hero_fight" ) {
		if (cmd_tok.size() >= 2) {
			int hero_index = atoi(cmd_tok[1].c_str());
			int is_fight = atoi(cmd_tok[2].c_str());
			req_hero_set_fight(hero_index, is_fight);
		}
	}

	if (cmd_tok[0] == "hero_awake" ) {
		if (cmd_tok.size() >= 2) {
			int is_hero = atoi(cmd_tok[1].c_str());
			int hero_index = atoi(cmd_tok[2].c_str());
			req_hero_awake(is_hero, hero_index);
		}
	}

	if (cmd_tok[0] == "yzz_test" ) {
		Time_Value tv;
		int ntime;
		tv = tv.gettimeofday();
		ntime = (tv.sec()/86400)*86400;
		echo_test_result("yzz", ntime);
	}

	if (cmd_tok[0] == "config1" ) {

		echo_test_result("yes", 100);

		}

	if (cmd_tok[0] == "artifact_up" ) {
		if (cmd_tok.size() >= 3) {
			int is_hero = atoi(cmd_tok[1].c_str());
			int artifact_id = atoi(cmd_tok[1].c_str());
			int hero_index = atoi(cmd_tok[2].c_str());
			req_artifact_upgrade(is_hero, artifact_id, hero_index);
		}
	}

	if (cmd_tok[0] == "yzz_test1" ) {
			Time_Value tv;
			Time_Value tv1(tv);
			tv = tv.gettimeofday();
			tv = tv + tv1;
			Block_Buffer buffer;
			tv.serialize(buffer);
			echo_test_result("ntv+5s:", tv.sec());
		}

	if (cmd_tok[0] == "gemt" ) {
		handle_gem_enable_info();

	}

	if (cmd_tok[0] == "merge_db") {
		DBClientConnection *dest_conn = DB_MERGE_SERVER_MANAGER->connection_merge_server("192.168.8.108", 27023);
		if(!dest_conn) {
			echo_test_result("can not connect db");
			return 0;
		}


		DBClientConnection *source_conn = DB_MERGE_SERVER_MANAGER->connection_merge_server("192.168.8.108", 27024);
		if(!source_conn) {
			echo_test_result("can not connect db");
			return 0;
		}

		std::vector<Int_String> server_list;
		Int_String ip_port;
		ip_port.str_value = "192.168.8.108";
		ip_port.int_value = 27024;
		server_list.push_back(ip_port);

		DB_MERGE_SERVER_MANAGER->merge_database_backstage(*dest_conn, server_list);
	}

	if (cmd_tok[0] == "porp" ) {
		Int_Vec equip_index;
		equip_index.push_back(20001);
		equip_index.push_back(20002);
		req_equip_breakdown_prop(equip_index);

	}

	if (cmd_tok[0] == "gc" ) {
		req_gem_carve_info(1500);
	}

	if (cmd_tok[0] == "test_awake" ) {
		if (cmd_tok.size() >= 2) {
			int is_hero = atoi(cmd_tok[1].c_str());
			int hero_id = atoi(cmd_tok[2].c_str());
			test_awake_lvl_up(is_hero, hero_id);
		}
	}

	if(cmd_tok[0] == "l_pack_test") {
		if(cmd_tok.size() == 2){
			int pack_len = atoi(cmd_tok[1].c_str());
			Block_Buffer buf;
			buf.make_message(81000020);
			for (int i = 0; i < pack_len; ++i) {
				buf.write_int32(i);
			}
			buf.finish_message();
			this->send_to_client(buf);
		}
	}

	if (cmd_tok[0] == "hero_zhen" ) {
		if (cmd_tok.size() >= 2) {
			int hero_index = atoi(cmd_tok[1].c_str());
			int zhen = atoi(cmd_tok[2].c_str());
			req_hero_set_formation(hero_index, zhen);
		}
	}

	if (cmd_tok[0] == "hero_prop") {
		if (cmd_tok.size() == 3) {
			Property_Type prop_type = Property_Type(abs(atoi(cmd_tok[1].c_str())));
			double prop_val = atoi(cmd_tok[2].c_str());
			return test_prop(player_self()->heroer_detail().fighter_index, prop_type, prop_val);
		}
	}

	if (cmd_tok[0] == "bt") {
		Int_Int_Map pp;
		pp.insert(std::make_pair(504605, 100000));
		pp.insert(std::make_pair(504905, 100000));
		pp.insert(std::make_pair(507405, 100));
		for (Int_Int_Map::const_iterator it = pp.begin(); it != pp.end(); ++it) {
			Property_Type prop_type = Property_Type(it->first);
			double prop_val = it->second;
			test_prop(player_self()->heroer_detail().fighter_index, prop_type, prop_val);
		}
	}

	if (cmd_tok[0] == "new_hero_click" ) {
		if (cmd_tok.size() >= 2) {
			int hero_id = atoi(cmd_tok[1].c_str());
			return req_new_hero_click(hero_id);
		}
	}

	if (cmd_tok[0] == "new_heros" ) {
		if (cmd_tok.size() >= 1) {
			return req_fetch_clicked_new_heros();
		}
	}

	if (cmd_tok[0] == "u") {
		if (cmd_tok.size() == 2) {
			Time_Value now = Time_Value::gettimeofday();
			if (CONFIG_INSTANCE->update_tick_ > now) {
				std::stringstream str_stream;
				str_stream << "更新频繁,再等" << CONFIG_INSTANCE->update_tick_.sec() - now.sec()  << "秒后操作!!";
				echo_test_result(str_stream.str());
				return 0;
			}
			String_Vec files;
			std::string module = cmd_tok[1];
			if (CONFIG_INSTANCE->load_module_config(module, files)) {
				echo_test_result("热更模块不存在:" + module);
			} else {
				{
					MSG_83000000 msg;
					msg.chat_type = 100;
					Chat_Content content;
					content.text = "开始热更配制(世界会卡10秒左右):" + module;
					msg.content.push_back(content);
					Block_Buffer buf;
					MAKE_MSG_BUF(msg, buf);
					monitor()->send_to_all_client(buf);
				}

				Block_Buffer inner_buf;
				inner_buf.make_message(SERVER_INNER_CONFIG_UPDATE);
				inner_buf.write_string(module);
				inner_buf.write_uint16(files.size());
				for (String_Vec::iterator it = files.begin(); it != files.end(); ++it) {
					inner_buf.write_string(*it);
				}
				inner_buf.finish_message();
				monitor()->send_to_all_process(inner_buf);

				CONFIG_INSTANCE->update_tick_ = Time_Value::gettimeofday() + Time_Value(30);
			}
		}
	}

	if (cmd_tok[0] == "exploit" ) {
		if (cmd_tok.size() >= 2) {
			int val = atoi(cmd_tok[1].c_str());
			if (val) {
				return change_exploit_val(val);
			}
		}
	}

	if (cmd_tok[0] == "equip_on_off" ) {
	//	if (cmd_tok.size() == 2) {
	//		int index = 0;
	//		index = atoi(cmd_tok[1].c_str());
	//		req_equip_on_off(index);
	//	}
	}

	if (cmd_tok[0] == "req_unban" ) {
		if (cmd_tok.size() >= 2) {
			int index = 0;
			index = atoi(cmd_tok[1].c_str());
			req_equip_unban(index, 0, 0);
		}
	}

	if (cmd_tok[0] == "enchant" ) {
		if (cmd_tok.size() >= 9) {
//			int index = atoi(cmd_tok[1].c_str());
//			int8_t is_bat = atoi(cmd_tok[2].c_str());
//			int8_t auto_buy = atoi(cmd_tok[3].c_str());
//
//			Int8_Vec locks;
//			locks.push_back(atoi(cmd_tok[4].c_str()));
//			locks.push_back(atoi(cmd_tok[5].c_str()));
//			locks.push_back(atoi(cmd_tok[6].c_str()));
//			locks.push_back(atoi(cmd_tok[7].c_str()));
//			locks.push_back(atoi(cmd_tok[8].c_str()));
//			return req_equip_enchant(index, is_bat, auto_buy, locks);
		}
		if (cmd_tok.size() >= 4) {
//			int index = atoi(cmd_tok[1].c_str());
//			int8_t is_bat = atoi(cmd_tok[2].c_str());
//			int8_t auto_buy = atoi(cmd_tok[3].c_str());
//
//			Int8_Vec locks(5, 0);
//			return req_equip_enchant(index, is_bat, auto_buy, locks);
		}
	}

	if (cmd_tok[0] == "enchant_s" ) {
		if (cmd_tok.size() >= 3) {
			int equip_index = atoi(cmd_tok[1].c_str());
			int group_index = atoi(cmd_tok[2].c_str());
			return req_equip_enchant_replace_prop_group(equip_index, group_index);
		}
	}

	if (cmd_tok[0] == "rune_info" ) {
		req_fetch_rune_info();
	}

	if (cmd_tok[0] == "rune_activate" ) {
		if (cmd_tok.size() >= 3) {
			int part = atoi(cmd_tok[1].c_str());
			int hole_index = atoi(cmd_tok[2].c_str());
			req_rune_activate(part, hole_index, 0);
		}
	}

	if (cmd_tok[0] == "rune_fill" ) {
		if (cmd_tok.size() >= 4) {
			int part = atoi(cmd_tok[1].c_str());
			int hole_index = atoi(cmd_tok[2].c_str());
			int is_bat = atoi(cmd_tok[3].c_str());
			req_rune_fill_energy(part, hole_index, is_bat, 0);
		}
	}

	if (cmd_tok[0] == "points" ) {
		if (cmd_tok.size() >= 2) {
			int points = atoi(cmd_tok[1].c_str());
			return gm_add_points(points);
		}
	}

	if (cmd_tok[0] == "rename" ) {
		if (cmd_tok.size() >= 2) {
			std::string new_name = cmd_tok[1];
			return req_rename(new_name);
		}
	}

	if (cmd_tok[0] == "rs_daily" ) {
		Rune_Stone::trigger_daily_zero(Time_Value::zero);
	}

	if (cmd_tok[0] == "get_suit" ) {
		if (cmd_tok.size() >= 3) {
			int suit_id = atoi(cmd_tok[1].c_str());
			int unban_lvl = atoi(cmd_tok[2].c_str());
			return test_get_and_enhance_equips(suit_id, unban_lvl);
		}
		if (cmd_tok.size() >= 2) {
			int suit_id = atoi(cmd_tok[1].c_str());
			int unban_lvl = 1;
			return test_get_and_enhance_equips(suit_id, unban_lvl);
		}
	}

	if (cmd_tok[0] == "unban_lvl" ) {
		if (cmd_tok.size() >= 2) {
			int unban_lvl = atoi(cmd_tok[1].c_str());
			return test_unban_equips(unban_lvl);
		}
	}
	if (cmd_tok[0] == "master_lvl") {
		if (cmd_tok.size() >= 2) {
			int master_lvl = atoi(cmd_tok[1].c_str());
			return test_master(master_lvl);
		}
	}

	if (cmd_tok[0] == "rune_lvl" ) {
		if (cmd_tok.size() >= 2) {
			int hole_lvl = atoi(cmd_tok[1].c_str());
			return test_runes_lvl_up(hole_lvl);
		}
	}

	if (cmd_tok[0] == "apply_list" ) {
		req_gang_apply_list();
	}

	if (cmd_tok[0] == "gang_gift" ) {
		req_gang_upgrade_send_gift();
	}

	if (cmd_tok[0] == "new_gang_cd" ) {
		ganger_reset_leave_time();
	}

	if (cmd_tok[0] == "gang_daily" ) {
		GANG_MANAGER->daily_refresh();
	}

	if (cmd_tok[0] == "seize_power" ) {
		Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(this->ganger_detail().gang_id);
		if (gang_view) {
			gang_view->can_seize_power = 1;
			return req_gang_seize_power();
		}
	}

	if (cmd_tok[0] == "bless_buf" ) {
		if (cmd_tok.size() >= 2) {
			int bless_buf_id = atoi(cmd_tok[1].c_str());
			return req_send_bless_buf(player_self()->role_id(), bless_buf_id);
		}
	}

	if (cmd_tok[0] == "gang_skill" ) {
		if (cmd_tok.size() >= 2) {
			int skill_id = atoi(cmd_tok[1].c_str());
			return req_learn_ganger_skill(skill_id);
		}
	}

	if (cmd_tok[0] == "gang_draw" ) {
		return req_gang_draw();
	}

	if (cmd_tok[0] == "draw_award" ) {
		return req_gang_take_draw_item();
	}

	if (cmd_tok[0] == "gang_daily" ) {
		Ganger::trigger_daily_zero(Time_Value::zero);
	}
	if (cmd_tok[0] == "gang_daily_6" ) {
		Ganger::trigger_daily_six(Time_Value::zero, true);
	}

	if (cmd_tok[0] == "research_skill" ) {
		return test_research_all_gang_skill();
	}

	if (cmd_tok[0] == "gang_buy" ) {
		if (cmd_tok.size() >= 3) {
			int item_id = atoi(cmd_tok[1].c_str());
			int amount = atoi(cmd_tok[2].c_str());
			return req_gang_shop_buy_item(item_id, amount);
		}
	}


	if (cmd_tok[0] == "single_daily" ) {
		Single_Dungeon::daily_refresh();
		return 0;
	}


	if (cmd_tok[0] == "10102002" ) {
		if (cmd_tok.size() >= 3) {
			int index_from = atoi(cmd_tok[1].c_str());
			int index_to = atoi(cmd_tok[2].c_str());
			req_hero_equip_on_off(index_from, index_to);
		}
	}

	if (cmd_tok[0] == "fornotice_gift" ) {
		if (cmd_tok.size() >= 2) {
			int gift_lvl = atoi(cmd_tok[1].c_str());
			req_forenotice_receive_gift(gift_lvl);
		}
	}

	//learn skill
	if (cmd_tok[0] == "get_skills") {
		if (cmd_tok.size() == 1) {
			this->get_all_learned_skills(0);
		}
	}

	if (cmd_tok[0] == "l_skill") {
		if (cmd_tok.size() == 4) {
			int page_id = atoi(cmd_tok[1].c_str());
			int id = atoi(cmd_tok[2].c_str());
			int lv = atoi(cmd_tok[3].c_str());
			this->manual_learn_skill(page_id, id, lv);
		}
	}

	if (cmd_tok[0] == "erase_skill") {
		erase_skill(1, 0);
	}
	if (cmd_tok[0] == "skill_loc") {
		if (cmd_tok.size() == 3) {
			int skill_id = atoi(cmd_tok[1].c_str());
			int newloc = atoi(cmd_tok[2].c_str());
			move_skill(skill_id, newloc, 1);
		}
	}
	if(cmd_tok[0] == "pass_scene"){
		int type = 0;
		if(cmd_tok.size() == 2){
			type = atoi(cmd_tok[1].c_str());
		}
		if(type == 0 || type == 1){
			Int_Vec scene_ids = CONFIG_CACHE_SCENE->get_all_single_dungeon_id();
			for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
				const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(*it);
				if(!cfg){
					break;
				}
				this->clear_single_dungeon(cfg);
				this->hook_compelte_scene(*it);
			}
		}
		if(type == 0 || type == 2){
			Int_Vec scene_ids = CONFIG_CACHE_SCENE->get_all_elite_dungeon_id(1);
			for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
				const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(*it);
				if(!cfg){
					break;
				}
				this->clear_elite_dungeon(cfg);
				this->hook_compelte_scene(*it);
			}
		}
		if(type == 0 || type == 3){
			Int_Vec scene_ids = CONFIG_CACHE_SCENE->get_all_elite_dungeon_id(2);
			for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
				const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(*it);
				if(!cfg){
					break;
				}
				this->clear_elite_dungeon(cfg);
				this->hook_compelte_scene(*it);
			}
		}
		if(type > 80000000){
			int scene_id = type;
			const Scene_Config* scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
			if(scene_config != NULL){
				int scene_type = scene_config->type;
				if(scene_type == FuBen_Scene_Type){
					if(scene_config->dungeon.first_layer_id != 0){
						const Scene_Config* target_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_config->dungeon.first_layer_id);
						if(target_config){
							clear_single_dungeon(target_config);
							this->hook_compelte_scene(target_config->scene_id);
						}
					}else{
						clear_single_dungeon(scene_config);
						this->hook_compelte_scene(scene_config->scene_id);
					}
				}else if(scene_type == Elite_Scene_Type){
					clear_elite_dungeon(scene_config);
					this->hook_compelte_scene(scene_config->scene_id);
				}

			}
		}
		return 0;
	}

	if(cmd_tok[0] == "pass_scene_to"){
		if(cmd_tok.size() == 2){
			int scene_id = atoi(cmd_tok[1].c_str());
			const Scene_Config* scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
			if(scene_config != NULL){
				const Scene_Config_Map_Cache &scene_config_map = CONFIG_CACHE_SCENE->scene_config_cache_map();
				for (Scene_Config_Map_Cache::const_iterator it = scene_config_map.begin(); it != scene_config_map.end(); ++it) {
					if (it->second.scene_id == scene_id) {
						break;
					}
					int scene_type = it->second.type;
					this->hook_compelte_scene(it->second.scene_id);
					if(scene_type == FuBen_Scene_Type){
						if(it->second.dungeon.first_layer_id != 0){
							const Scene_Config* target_config = CONFIG_CACHE_SCENE->scene_config_cache(it->second.dungeon.first_layer_id);
							if(target_config){
								clear_single_dungeon(target_config);
								this->hook_compelte_scene(target_config->scene_id);
							}
						}else{
							clear_single_dungeon(&(it->second));
							this->hook_compelte_scene(it->second.scene_id);
						}
					}else if(scene_type == Elite_Scene_Type){
						clear_elite_dungeon(&(it->second));
						this->hook_compelte_scene(it->second.scene_id);
					}
				}
				int scene_type = scene_config->type;
				if(scene_type == FuBen_Scene_Type){
					if(scene_config->dungeon.first_layer_id != 0){
						const Scene_Config* target_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_config->dungeon.first_layer_id);
						if(target_config){
							clear_single_dungeon(target_config);
							this->hook_compelte_scene(target_config->scene_id);
						}
					}else{
						clear_single_dungeon(scene_config);
						this->hook_compelte_scene(scene_config->scene_id);
					}
				}else if(scene_type == Elite_Scene_Type){
					clear_elite_dungeon(scene_config);
					this->hook_compelte_scene(scene_config->scene_id);
				}
			}
		}
	}

	if(cmd_tok[0] == "act_elite"){
		player_self()->act_first_elite_dungeon();
		return 0;
	}

	if(cmd_tok[0] == "act_single"){
		player_self()->act_first_single_dungeon();
		return 0;
	}
	if(cmd_tok[0] == "arena_start"){
		ARENA_MANAGER->start_arena_battle();
	}
	if(cmd_tok[0] == "arena_info"){
		this->fetch_arena_info();
	}
	if(cmd_tok[0] == "arena_local_rank"){
			this->fetch_arena_rank_info(0);
	}
	if(cmd_tok[0] == "arena_inner_rank"){
			this->fetch_arena_rank_info(1);
	}
	if(cmd_tok[0] == "arena_buy"){
			this->req_buy_arena_pay_times(1);
	}
	if(cmd_tok[0] == "arena_match"){
			this->req_match_arena_battle();
	}
	if(cmd_tok[0] == "arena_refresh_1"){
		ARENA_MANAGER->refresh_1_debug();
	}
	if(cmd_tok[0] == "arena_refresh_3"){
		ARENA_MANAGER->refresh_3_debug();
	}
	if(cmd_tok[0] == "arena_refresh_7"){
		ARENA_MANAGER->refresh_7_debug();
	}
	if(cmd_tok[0] == "arena_set"){
		if (cmd_tok.size() == 3) {
			int group = atoi(cmd_tok[1].c_str());
			int seg = atoi(cmd_tok[2].c_str());
			if(group < 1 || group > 6 || seg < 1 || seg > 6 || (group*10+seg)>61){
				this->send_err_to_client(ERROR_CLIENT_PARAM, 0);
			}else{
				ARENA_MANAGER->set_grou_seg(this->player_self(), group, seg);
			}

		}
	}
	if(cmd_tok[0] == "arena_set_score"){
		if (cmd_tok.size() == 2) {
			int score = atoi(cmd_tok[1].c_str());
			ARENA_MANAGER->set_score_debug(this->player_self(), score);
		}
	}
	if(cmd_tok[0] == "arena_match"){
		if (cmd_tok.size() == 2) {
			//role_id_t match_role_id = atoll(cmd_tok[1].c_str());
			//ARENA_MANAGER->match_arena_battle(this->role_id(), match_role_id);
		}
	}
	if(cmd_tok[0] == "arena_season_finish"){
		ARENA_MANAGER->season_finish();
	}
	if(cmd_tok[0] == "icon") {
		if(cmd_tok.size() < 2) {
			test_one_key_open_all_icon(this);
		}
		else {
			if(cmd_tok.size() >= 3) {
				if(cmd_tok[1] == "close") {
					uint16_t icon_id = (uint16_t)atoi(cmd_tok[2].c_str());
					other_module_notify_icon_to_close_icon(icon_id);
				}
				if(cmd_tok[1] == "open") {
					uint16_t icon_id = (uint16_t)atoi(cmd_tok[2].c_str());
					safe_guide_open_icon(icon_id);
					hook_icon_open(icon_id);
				}
			}
		}
	}

	if(cmd_tok[0] == "buy_phy_power") {
		this->req_buy_phy_power(1);
	}
	if(cmd_tok[0] == "fetch_phy_power") {
		this->fetch_phy_power_info();
	}
	if(cmd_tok[0] == "reset_phy_power") {
		int phy_power_cur = 100;
		int phy_power_recover_time = 0;
		int cur_buy_times = 0;
		this->sync_phy_power_info(phy_power_cur, phy_power_recover_time, cur_buy_times);
		//sync scene
		{
			Block_Buffer buf;
			MSG_20100030 msg;
			msg.phy_power_buy_times = this->fighter_detail().phy_power_buy_times;
			msg.phy_power_cur = this->fighter_detail().current_strength();
			msg.phy_power_recover_time = this->fighter_detail().phy_power_recover_time;
			buf.make_message(msg.msg_id);
			msg.serialize(buf);
			buf.finish_message();
			this->send_to_scene(buf);
		}
	}
	if(cmd_tok[0] == "elite_add_reset_times") {
		this->add_elite_reset_times();
	}
	if(cmd_tok[0] == "elite_reset_times"){
		this->req_reset_elite_dungeon_times(1);
	}
	if (cmd_tok[0] == "master"){
		if (cmd_tok.size() == 3) {
			int i = 0;
			int j = 0;
			i = atoi(cmd_tok[1].c_str());
			j = atoi(cmd_tok[2].c_str());
			this->req_equip_master(i, j);
		}
	}
	/*if (cmd_tok[0] == "compose"){
		if (cmd_tok.size() == 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());
			this->req_equip_compose(i, 1);
		}else if (cmd_tok.size() == 3) {
			int i = 0;
			int j = 0;
			i = atoi(cmd_tok[1].c_str());
			j = atoi(cmd_tok[2].c_str());
			this->req_equip_compose(i, j);
		}
	}*/
	if (cmd_tok[0] == "title") {
		if (cmd_tok.size() == 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());
			this->add_title(i);
		}
	}
	if (cmd_tok[0] == "mount_ride"){
		int ride = 1;
		int horse_id = 0;
		if (cmd_tok.size() == 2) {
			ride = atoi(cmd_tok[1].c_str());
		}
		if (cmd_tok.size() == 3) {
			ride = atoi(cmd_tok[1].c_str());
			horse_id = atoi(cmd_tok[2].c_str());
		}
		this->req_mount_ride(ride, horse_id);
	}
	if (cmd_tok[0] == "mount_activate"){
		if (cmd_tok.size() == 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());
			this->req_mount_activate(i, 0);
		}
	}
	if (cmd_tok[0] == "mount_timeout"){
		int min = 1;
		int horse_id = 0;
		if (cmd_tok.size() == 2) {
			horse_id = atoi(cmd_tok[1].c_str());
			this->cmd_mount_timeout(horse_id, min);
		}else if (cmd_tok.size() == 3) {
			horse_id = atoi(cmd_tok[1].c_str());
			min = atoi(cmd_tok[2].c_str());
			this->cmd_mount_timeout(horse_id, min);
		}
	}

	if (cmd_tok[0] == "jj") {
		if (cmd_tok.size() >= 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());

			switch(i) {
			case 0:
				TEAM_MANAGER->team_sports_test_close();
				TEAM_MANAGER->close_team_sports();
				break;
			case 1:
				TEAM_MANAGER->team_sports_test();
				TEAM_MANAGER->open_team_sports();
				break;
			case 2:
				TEAM_MANAGER->handle_ranking_checkout();
				break;
			case 3: {
				int id = 0;
				if (cmd_tok.size()== 3) {
					id = atoi(cmd_tok[2].c_str());
					return get_team_award(id);
				}
				break;
			}
			case 4: {
				int id = 0;
				if (cmd_tok.size()== 3) {
					id = atoi(cmd_tok[2].c_str());
					Block_Buffer inner_buf;
					MSG_20100310 inner_msg;
					inner_msg.role_id = role_id();
					inner_msg.level = id;
					MAKE_MSG_BUF(inner_msg, inner_buf);
					LOGIC_MONITOR->send_to_scene(TEAM_SPORTS_SCENE_ID, 0, inner_buf);
				}
				break;
			}
			case 5: {
				int score = 0;
				if (cmd_tok.size()== 3) {
					score = atoi(cmd_tok[2].c_str());
					if (score >= 0) {
						TEAM_MANAGER->change_team_score(role_id(), score);
						TEAM_MANAGER->active_integrate(role_id());
					}
				}
				break;
			}
			default:
				break;
			}
			TEAM_MANAGER->save_detail();
		}
	}

	if (cmd_tok[0] == "gang" || cmd_tok[0] == "gang_se") {
		if (cmd_tok.size() >= 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());
			switch(i) {
			case -2:
				GANG_MANAGER->gang_war_start_sync(2);
				break;
			case -1:
				GANG_MANAGER->gang_war_start_sync(1);
				break;
			case 4:
				GANG_MANAGER->create_mateched_16();
				break;
			case 3:
				player_self()->req_gang_war_enter();
				break;
			case 2:
				GANG_MANAGER->close_activity();
				break;
			case 1:
				GANG_MANAGER->open_activity();
				break;
			case 0:
				GANG_MANAGER->ready_activity();
				break;
			case 1000:
				GANG_MANAGER->gang_war_ready_mail();
				break;
			case 2000:
				GANG_MANAGER->gang_war_clear_award_list();
				break;
			case 3000:
				GANG_MANAGER->gang_war_send_mail_for_start();
				break;
			case 4000:
				GANG_MANAGER->gang_war_produce_rank_award_when_all_close();
				break;
			case 5000:
				GANG_MANAGER->gang_war_rank_award_auto_get();
				break;
			default:
				break;
			}
		}
	}
	if (cmd_tok[0] == "expedition_create_daily") {
		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
		buf.write_uint8(EXPEDITION_CHANNEL_CREATE_DAILY_AWARD);
		buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器uuid

		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(EXPEDITION_INNER_SCENE_ID, 0, buf);
	}
	if (cmd_tok[0] == "expedition_send_demage") {
		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
		buf.write_uint8(EXPEDITION_CHANNEL_CREATE_DEMAGE_RANK_AWARD);
		buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器uuid

		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(EXPEDITION_INNER_SCENE_ID, 0, buf);
	}
	if (cmd_tok[0] == "expe") {
		if (cmd_tok.size() >= 2) {
			int i = atoi(cmd_tok[1].c_str());
			switch(i) {
			case 2: {
				player_self()->reset_award_time();
				break;
			}
			}
		}
	}
	if(cmd_tok[0] == "flowers") {
		if (cmd_tok.size() >= 3) {
			std::string msg = "";
			if (cmd_tok.size() >= 4) {
				msg = cmd_tok[3];
			}
			player_self()->req_handsel_flowers(atoi(cmd_tok[1].c_str()), cmd_tok[2], msg);
		}
	}
	if(cmd_tok[0] == "secret_shop_refresh") {
		SECRET_SHOP_MANAGER->refresh_shop();
	}
	if(cmd_tok[0] == "secret_shop_buy") {
		int i = 0;
		if (cmd_tok.size() >= 2) {
			i = atoi(cmd_tok[1].c_str());
		}
		SECRET_SHOP_MANAGER->req_buy_shop_item(player_self(), i);
	}


	if(cmd_tok[0] == "wipe_single") {
		int scene = 0;
		int num = 0;
		int type = 1;
		if (cmd_tok.size() >= 2) {
			scene = atoi(cmd_tok[1].c_str());
		}
		if (cmd_tok.size() >= 3) {
			num = atoi(cmd_tok[2].c_str());
		}
		if (cmd_tok.size() >= 4) {
			type = atoi(cmd_tok[3].c_str());
		}
		player_self()->req_wipe_out(type, num,scene);
	}
	if(cmd_tok[0] == "complete_wipe_single") {
		int type = 1;
		if (cmd_tok.size() >= 2) {
			type = atoi(cmd_tok[1].c_str());
		}
		player_self()->req_complete_wipe_out(type);
	}
	if(cmd_tok[0] == "interrupt_wipe_single") {
		player_self()->req_interrupt_wipe_out(0);
	}
	if(cmd_tok[0] == "campfire_enter"){
		player_self()->req_campfire_enter();
	}
	if (cmd_tok[0] == "pack_clear") {
		if (cmd_tok.size() >= 2) {
			int i = 0;
			i = atoi(cmd_tok[1].c_str());
			player_self()->pack_discard_all(Pack::Pack_Type(i));
		}
	}
	if(cmd_tok[0] == "bonfire_reset1"){
		player_self()->cmd_add_firewood(0);
	}
	if(cmd_tok[0] == "bonfire_start"){
		int time = 20;
		if (cmd_tok.size() >= 2) {
			time = atoi(cmd_tok[1].c_str());
		}
		CAMPFIRE_MANAGER->bonfire_act_start(time);
	}
	if(cmd_tok[0] == "bonfire_finish"){
		CAMPFIRE_MANAGER->bonfire_act_finish();
	}
	if(cmd_tok[0] == "bonfire_enter"){
		player_self()->req_campfire_enter();
	}
	if(cmd_tok[0] == "valentine_monster_start"){
		int time = 60;
		if (cmd_tok.size() >= 2) {
			time = atoi(cmd_tok[1].c_str());
		}
		VALENTINE_MONSTER_ACTIVE_MANAGER->cmd_start_active(time);
		return 0;
	}
	if(cmd_tok[0] == "valentine_monster_end"){
		VALENTINE_MONSTER_ACTIVE_MANAGER->cmd_end_active();
		return 0;
	}
	if(cmd_tok[0] == "world_boss_enter"){
		player_self()->req_act_enter_scene(16);
	}
	if(cmd_tok[0] == "world_boss_start"){
		int time = 60;
		if (cmd_tok.size() >= 2) {
			time = atoi(cmd_tok[1].c_str());
		}
		WORLD_BOSS_ACTIVE_MANAGER->cmd_start_active(time);
		return 0;
	}
	if(cmd_tok[0] == "world_boss_lv"){
		int value = 1;
		if (cmd_tok.size() >= 2) {
			value = atoi(cmd_tok[1].c_str());
		}
		WORLD_BOSS_ACTIVE_MANAGER->cmd_set_world_boss_lv(value);
		return 0;
	}
	if(cmd_tok[0] == "world_boss_kill_num"){
		int value = 0;
		if (cmd_tok.size() >= 2) {
			value = atoi(cmd_tok[1].c_str());
		}
		WORLD_BOSS_ACTIVE_MANAGER->cmd_set_world_boss_kill_num(value);
		return 0;
	}

	if(cmd_tok[0] == "honor_arena_enter"){
		player_self()->req_act_enter_scene(22);
	}
	if(cmd_tok[0] == "honor_arena_start"){
		int time = 60;
		if (cmd_tok.size() >= 2) {
			time = atoi(cmd_tok[1].c_str());
		}
		HONOR_ARENA_ACTIVE_MANAGER->cmd_start_active(time);
		return 0;
	}

	if(cmd_tok[0] == "gang_boss_start"){
		if(player_self()->gang_id() == 0){
			return 0;
		}
		GANG_MANAGER->daily_refresh();
		return 0;
	}
	if(cmd_tok[0] == "gang_boss_lv"){
		int value = 1;
		if (cmd_tok.size() >= 2) {
			value = atoi(cmd_tok[1].c_str());
		}
		GANG_MANAGER->cmd_set_boss_lv(gang_id(), value);
		return 0;
	}
	if(cmd_tok[0] == "gang_boss_kill_num"){
		int value = 60;
		if (cmd_tok.size() >= 2) {
			value = atoi(cmd_tok[1].c_str());
		}
		GANG_MANAGER->cmd_set_boss_kill_num(gang_id(), value);
		return 0;
	}
	if (cmd_tok[0] == "fashion") {
//		if (cmd_tok.size() >= 2) {
//			int id = atoi(cmd_tok[1].c_str());
//			player_self()->handle_insert_fashion(id, 1, Fashion_Item::STATE_OFF);
//		}
	}
	if (cmd_tok[0] == "arena_to_inner") {
		ARENA_MANAGER->sync_all_to_inner_scene();
	}
	if (cmd_tok[0] == "reset_practice") {
		player_self()->reset_practice();
	}
	{
		// life_skill
	    if (cmd_tok[0] == "learn_life_skill") {
		    player_self()->learn_life_skill(atoi(cmd_tok[1].c_str()));
		    return 0;
	    }
	    if (cmd_tok[0] == "life_skill_up") {
	    	player_self()->life_skill_upgrade(atoi(cmd_tok[1].c_str()));
	        return 0;
	    }
	    if ( cmd_tok[0] == "spar_refine" && 4 == cmd_tok.size()) {
	    	std::vector<uint32_t> vec;
	    	vec.push_back(atoi(cmd_tok[1].c_str()));
	    	vec.push_back(atoi(cmd_tok[2].c_str()));
	    	vec.push_back(atoi(cmd_tok[3].c_str()));
	    	player_self()->spar_refine(vec);
	    	return 0;
	    }
	    if ( cmd_tok[0] == "card_make") {

	    	player_self()->card_make(atoi(cmd_tok[1].c_str()));
	        return 0;
	    }
	    if ( cmd_tok[0] == "spar_smelt") {
	    	player_self()->spar_smelt(atoi(cmd_tok[1].c_str()),atoi(cmd_tok[2].c_str()));
	    	return 0;
	    }
	    if ( cmd_tok[0] == "add_energy" ) {
	    	player_self()->add_energy(atoi(cmd_tok[1].c_str()));
	    	return 0;
	    }
	}
	{
		if ( cmd_tok[0] == "arena_worship" ) {
			player_self()->req_arena_worship((int64_t)atoll(cmd_tok[1].c_str()),atoi(cmd_tok[2].c_str()));
			return 0;
		}
	}
	return 0;
}
