/*
 * Tasker_Imp_A.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: gaojianshan
 */


#include "Logic_Module_Head.h"
#include "Config_Cache_Task.h"
#include "Tasker.h"
#include "Task_Struct.h"
#include "Task_Def.h"
#include "Task_Config.h"
#include "pack/Pack_Struct.h"
#include "pack/Packer.h"
#include "Msg_Task_Struct.h"
#include "Msg_Active_Struct.h"
#include "Err_Code.h"
#include "Logic_Player.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Reward_Task.h"
#include "Msg_Active_Enum.h"
#include "Msg_Task_Enum.h"
#include "item_user/Item_User_Func.h"
#include "vip/Vip.h"
#include "vip/Vip_Def.h"
#include "achievement/Achievement.h"
#include "task/Day_Week_Task_Config.h"
#include "reward/Reward_Def.h"
#include "Scene_Func.h"
#include "Pool_Manager.h"
#include "fighter/Fighter_Def.h"
#include "gang/Gang_Manager.h"
#include "gang/Gang_Struct.h"
#include "equiper/Equiper.h"
#include "equiper/Equiper_Def.h"
#include "Game_Typedef.h"
#include "helper/Helper_Def.h"
#include "covenant/Covenant_Def.h"
#include "Msg_Active_Enum.h"
#include "helper/Helper_Struct.h"
#include "pack/Pack_Func.h"
#include "active/Active_Def.h"
#include "active/Active_Manager.h"


int Tasker::process_accept_imp(uint32_t task_id, bool is_auto_accept /* = true*/)
{
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }

	TaskInfo *task_info = find_task(task_id);
	if(!task_info) {
		return ERROR_TASK_NOT_EXIST;
	}

    if(!is_first_main_line_task(*task_info)) {
    	//处理接任务时，回满怒气
    	process_full_prop_when_task_accepted(task_info->__task_id);

    	//接任务时，添加/删除英雄
    	add_or_del_hero_when_task_accepted(*task_info, task_json->hero_op, task_json->hero_id);

    	//激活坐骑
    	process_activate_horse(task_info->__task_id);

    	//添加buf
    	process_add_buf(task_info->__task_id);

    	process_transfer_point(task_info->__task_id);
    }

	set_task_status(*task_info, TaskInfo::TASK_STATUS_ACCEPTED);

    //设置接受任务时间
	Time_Value now = Time_Value::gettimeofday();
	task_info->__accept_tick.set(now);
    save_tick();

    Log_Task_Stream(player_self()->role_id(), task_info->__task_id,
    								TASK_STREAM_ACCEPT, player_self()->level(),
    								now.sec());

	//添加限时任务到监听任务集
	add_limit_time_task_to_listen_set(task_info);

    if (!task_info->__condition_list.empty()) {
		if (task_info->is_logic_kill_monster()) {
			task_accepted_monster_set().insert(task_info->__task_id);
		}

		if (task_info->is_logic_fb()) {
			task_accepted_fb_set().insert(task_info->__task_id);
		}

		if (task_info->is_logic_npc_talk()) {
			task_accepted_npc_talk_set().insert(task_info->__task_id);
		}

		if (task_info->is_logic_arrive()) {
			task_accepted_arrive_set().insert(task_info->__task_id);
		}

		if (task_info->is_logic_level_up()) {
			task_accepted_lvl_set().insert(task_info->__task_id);
		}

		if (task_info->is_logic_package()) {
			task_accepted_package_set().insert(task_info->__task_id);
			init_task_listen_item(task_info);
		}

		if (task_info->is_logic_kill_monster_group()) {
			task_accepted_monster_group_set().insert(task_info->__task_id);
		}

		if (task_info->is_logic_collect()) {
			task_accepted_collect_set().insert(task_info->__task_id);
			init_task_listen_item(task_info);
		}

		if (task_info->is_logic_fb_type()) {
			task_accepted_fb_type_set().insert(task_info->__task_id);
		}

		if (task_info->is_logic_win_player()) {
			task_accepted_win_player_set().insert(task_info->__task_id);
		}

		if(task_info->is_logic_join_gang()) {
			task_accepted_join_gang_set().insert(task_info->__task_id);
		}

	    if(task_info->is_logic_ganger_level()) {
	    	task_accepted_ganger_level_set().insert(task_info->__task_id);
	    }


	    if(task_info->is_logic_player_all_equip()) {
	    	task_accepted_player_all_equip_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_body_equip()) {
	    	task_acccepted_body_equip_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_body_equip_level()) {
	    	task_accepted_body_equip_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_equip_level()) {
	    	task_accepted_equip_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_stone_level()) {
	    	task_accepted_stone_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_buy_goods()) {
	    	task_accepted_buy_goods_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_get_hero()) {
	    	task_accepted_get_hero_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_fight()) {
	    	task_accepted_fight_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_commit_consume_item()) {
	    	task_accepted_commit_consume_item_set().insert(task_info->__task_id);
	    	init_task_listen_item(task_info);
	    }

	    if(task_info->is_logic_deliver_letter()) {
	    	task_accepted_deliver_letter_set().insert(task_info->__task_id);

	    	for (size_t i = 0; i < task_json->execute.size(); ++i) {
	            TaskCondition *task_cond = task_info->task_condition(i);
	            if (!task_cond)
	            {
	                continue;
	            }
	    		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DELIVER_LETTER) {
	                continue;
	    		}
	    		if(!CONFIG_CACHE_ITEM->find_item(task_json->execute[i].id)) {//execute[i].id: 物品id
	    			continue;
	    		}

	    		Item_Vec item_list;
	    		make_item_vec(item_list, task_json->execute[i].id, task_json[i].amount,
	    				static_cast<Item_Detail::Bind_Type>(task_json->execute[i].param1));
//	    		Item_Detail item(task_json->execute[i].id,
//	    					     task_json[i].amount,
//	    				         static_cast<Item_Detail::Bind_Type>(task_json->execute[i].param1));//execute[i].param1:是否绑定
	    		init_task_listen_item(task_info);
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list,
						Gain_Item_DM_Info(Pack::ITEM_GAIN_TASK));
				MSG_81000102 acv_msg;
				acv_msg.type = 0;
				for (Item_Vec::iterator item_it = item_list.begin(); item_it != item_list.end(); ++item_it) {
					Item_Basic_Info ibi;
					(*item_it).set_item_basic_info(ibi);
					acv_msg.item_info.push_back(ibi);
					Card_Info card_info;
					if ((*item_it).set_item_card_info(card_info)) {
						acv_msg.card_info_vec.push_back(card_info);
					}
				}
				THIS_SEND_TO_CLIENT(acv_msg);
	    	}
	    }

	    if(task_info->is_logic_help_other_complete_team_scene()) {
	    	task_accepted_help_other_complete_team_scene_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_sell_item()) {
	    	task_accepted_sell_item_set().insert(task_info->__task_id);
	    }

/*	    if(taski->is_logic_use_skill()) {
	    	task_accepted_use_skill_set().insert(taski->__task_id);
	    }

	    if(taski->is_logic_append_skill()) {
	    	task_accepted_append_skill_set().insert(taski->__task_id);
	    }*/

	    if(task_info->is_logic_arena()) {
	    	task_accepted_arena_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_chat_with_friend()) {
	    	task_accepted_chat_with_friend_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_send_mail()) {
	    	task_accepted_send_mail_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_team_scene_or_pvp()) {
	    	task_accepted_team_scene_or_pvp_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_browse_other_player_info()) {
	    	task_accepted_browse_other_player_info_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_arena_level()) {
	    	task_accepted_arena_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_equip_crimson_level()) {
		   task_accepted_equip_crimson_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_arena_all()) {
	    	task_accepted_arena_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_create_dragon_vale_building()) {
	    	task_accepted_create_dragon_vale_building_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_gain_dragon_vale_builiding_production()) {
	    	task_accepted_gain_dragon_vale_builiding_production_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_wake_up_hero_to_quality_level()) {
	    	task_accepted_wake_up_hero_to_quality_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_keep_hero_to_level()) {
	    	task_accepted_keep_hero_to_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_compose_equip()) {
	    	task_accepted_compose_equip_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_hero_dream_scene()) {
	    	task_accepted_hero_dream_scene_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_ganger_skill_level()) {
	    	task_accepted_ganger_skill_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_war()) {
	    	task_accepted_war_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_war_all()) {
	    	task_accepted_war_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_kill_enemy_in_war()) {
	    	task_accepted_kill_enemy_in_war_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_single_arena_all()) {
	    	task_accepted_single_arena_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_single_arena_win()) {
	    	task_accepted_single_arena_win_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_team_arena_all()) {
	    	task_accepted_team_arena_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_team_arena_win()) {
	    	task_accepted_team_arena_win_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_ganger_bless_buf()) {
	    	task_accepted_ganger_bless_buf_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_rob_other_dragon_vale_all()) {
	    	task_accepted_rob_other_dragon_vale_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_loigc_dragon_vale_step_foot_all()) {
	    	task_accepted_dragon_vale_step_foot_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_activeness_of_today()) {
	    	task_accepted_activeness_of_today_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_dragon_vale_building_upgrade()) {
	    	task_accepted_dragon_vale_building_upgrade_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_limit_time_kill_boss()) {
	    	task_accepted_limit_time_kill_boss_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_dragon_keep_all()) {
	    	task_accepted_dragon_keep_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_dragon_keep_success()) {
	    	task_accepted_dragon_keep_success_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_donate_to_gang()) {
	    	task_accepted_donate_to_gang_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_present_buf_to_other()) {
	    	task_accepted_present_buf_to_other_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_finish_reward_task_num()) {
	    	task_accepted_finish_reward_task_num_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_keep_dragon_to_level()) {
	    	task_accepted_keep_dragon_to_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_knight_trial()) {
	    	task_accepted_knight_trial_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_suit()) {
	    	task_accepted_suit_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_role_pull_on_equip()) {
	    	task_accepted_role_pull_on_equip_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_hero_pull_on_equip()) {
	    	task_accepted_hero_pull_on_equip_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_activate_wing()) {
			task_accepted_activate_wing_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_get_quality_rune()) {
	    	task_accepted_get_quality_rune_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_pass_kngiht_trials()) {
	    	task_accepted_pass_kngiht_trials_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_have_hero()) {
	    	task_accepted_have_hero_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_have_hero_quality()) {
	    	task_accepted_have_hero_quality_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_first_pass_scene()) {
	    	task_accepted_first_pass_scene_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_dragon_born()) {
	    	task_accepted_dragon_born_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_horse_star_level()) {
	    	task_accepted_horse_star_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_have_item_but_no_erase()) {
	    	task_accepted_have_item_but_no_erase_set().insert(task_info->__task_id);
	    	init_task_listen_item(task_info);
	    }

	    if(task_info->is_logic_have_equip_but_no_erase()) {
	    	task_accepted_have_equip_but_no_erase_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_same_gang_challenge_all()) {
	    	task_accepted_same_gang_challenge_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_main_scene_npc_battle()) {
	    	task_accepted_main_scene_npc_battle_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_equip_card_all()) {
	    	task_accepted_equip_card_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_equip_card_by_position()) {
	    	task_accepted_equip_card_by_position_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_card_upgrade_to_level_all()) {
	    	task_accepted_card_upgrade_to_level_all_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_card_upgrade_to_level_by_position()) {
	    	task_accepted_card_upgrade_to_level_by_position_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_lottery_draw()) {
	    	task_accepted_lottery_draw_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_equip_jewelry()) {
	    	task_accepted_equip_jewelry_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_invite_friend_step_foot()) {
	    	task_accepted_invite_friend_step_foot_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_kill_group_num()) {
	    	task_accepted_kill_group_num_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_did_reward_task_num()) {
	    	task_accepted_did_reward_task_num_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_alchemy_num()) {
	    	task_accepted_alchemy_num_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_did_reward_task_num_today()) {
	    	task_accepted_did_reward_task_num_today_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_remote_scene_any_pass()) {
	    	task_accepted_remote_scene_any_pass_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_knight_trial_any_pass()) {
	    	task_accepted_knight_trial_any_pass_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_compose_gemstone_level()) {
	    	task_accepted_compose_gemstone_level_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_inlay_position_gemstone()) {
	    	task_accepted_inlay_position_gemstone_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_inlay_any_gemstone()) {
	    	task_accepted_inlay_any_gemstone_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_world_boss()) {
	    	task_accepted_world_boss_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_world_boss_immediate_revive()) {
	    	task_accepted_world_boss_immediate_revive_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_enchant_any_equip()) {
	    	task_accepted_enchant_any_equip_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_inlay_id_gemstone()) {
	    	task_accepted_inlay_id_gemstone_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_gang_well_recover_spirit()) {
	    	task_accepted_gang_well_recover_spirit_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_equip_purple_ring()) {
	    	task_accepted_equip_purple_ring_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_inlay_level_gemstone()) {
	    	task_accepted_inlay_level_gemstone_set().insert(task_info->__task_id);
	    }

	    if(task_info->is_logic_upgrade_card()) {
	    	task_accepted_upgrade_card_set().insert(task_info->__task_id);
	    }


    } else {
    	if (TaskInfo::TASK_STATUS_ACCEPTED == task_state(task_info->__task_id)) {
    		this->process_finish_imp(task_info);
    		return 0;
    	}
    }

    notify_task_update(*task_info, *task_json);

    //接任务特效
    if(!is_auto_accept)
    {
    	MSG_82300029 msg_82300029;
    	msg_82300029.task_id = task_info->__task_id;
    	msg_82300029.type = 1;
		THIS_SEND_TO_CLIENT(msg_82300029);
    }

    //悬赏任务:更改悬赏榜状态
    if(task_info->is_reward_task()) {
    	tasker_detail_no_const().used_accept_count += 1;
    	set_reward_task_list_show_status(task_id, 1);
    	send_to_client_reward_task_list_show_status();

    	//领取悬赏任务获得道具
    	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
		if(CONFIG_CACHE_ITEM->find_item(cfg.item_id)) {
			Item_Detail item(cfg.item_id,cfg.item_num, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
			player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
					Gain_Item_DM_Info(Pack::ITEM_GAIN_ACCEPT_REWARD_TASK));
		}

		notify_remain_reward_task_num();
    }

    process_task_listen_when_accepted(task_info->__task_id);
    return 0;
}

int Tasker::process_abandon_imp(TaskInfo *task_info)
{
	if(!task_info) {
		return ERROR_TASK_NOT_EXIST;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (!task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }

	TaskInfo *taski = find_task(task_info->__task_id);
	if(!taski) {
		return ERROR_TASK_NOT_EXIST;
	}

	erase_task_listen_item(taski);

	task_accepted_package_set().erase(taski->__task_id);
    task_accepted_monster_set().erase(taski->__task_id);
    task_accepted_fb_set().erase(taski->__task_id);
    task_accepted_arrive_set().erase(taski->__task_id);
    task_accepted_lvl_set().erase(taski->__task_id);
    task_accepted_monster_group_set().erase(taski->__task_id);
    task_accepted_collect_set().erase(taski->__task_id);
    task_accepted_fb_type_set().erase(taski->__task_id);
    task_accepted_win_player_set().erase(taski->__task_id);
    task_accepted_join_gang_set().erase(taski->__task_id);
   	task_accepted_ganger_level_set().erase(taski->__task_id);
    task_accepted_player_all_equip_set().erase(taski->__task_id);
    task_acccepted_body_equip_set().erase(taski->__task_id);
    task_accepted_body_equip_level_set().erase(taski->__task_id);
    task_accepted_equip_level_set().erase(taski->__task_id);
    task_accepted_stone_level_set().erase(taski->__task_id);
    task_accepted_buy_goods_set().erase(taski->__task_id);
    task_accepted_get_hero_set().erase(taski->__task_id);
    task_accepted_fight_set().erase(taski->__task_id);
    task_accepted_commit_consume_item_set().erase(taski->__task_id);
    task_accepted_deliver_letter_set().erase(taski->__task_id);
    task_accepted_help_other_complete_team_scene_set().erase(taski->__task_id);
    task_accepted_sell_item_set().erase(taski->__task_id);
//    task_accepted_use_skill_set().erase(taski->__task_id);
//    task_accepted_append_skill_set().erase(taski->__task_id);
    task_accepted_limit_time_set().erase(taski->__task_id);
    task_accepted_arena_set().erase(task_info->__task_id);
    task_accepted_chat_with_friend_set().erase(task_info->__task_id);
    task_accepted_send_mail_set().erase(task_info->__task_id);
    task_accepted_team_scene_or_pvp_set().erase(task_info->__task_id);
    task_accepted_browse_other_player_info_set().erase(task_info->__task_id);
    task_accepted_npc_talk_set().erase(task_info->__task_id);
    task_accepted_arena_level_set().erase(task_info->__task_id);
    task_accepted_equip_crimson_level_set().erase(task_info->__task_id);
	task_accepted_arena_all_set().erase(task_info->__task_id);
	task_accepted_create_dragon_vale_building_set().erase(task_info->__task_id);
	task_accepted_gain_dragon_vale_builiding_production_set().erase(task_info->__task_id);
	task_accepted_wake_up_hero_to_quality_level_set().erase(task_info->__task_id);
	task_accepted_keep_hero_to_level_set().erase(task_info->__task_id);
	task_accepted_compose_equip_set().erase(task_info->__task_id);
	task_accepted_hero_dream_scene_set().erase(task_info->__task_id);
	task_accepted_ganger_skill_level_set().erase(task_info->__task_id);
	task_accepted_war_set().erase(task_info->__task_id);
    task_accepted_war_all_set().erase(task_info->__task_id);
    task_accepted_kill_enemy_in_war_set().erase(task_info->__task_id);
    task_accepted_single_arena_all_set().erase(task_info->__task_id);
    task_accepted_single_arena_win_set().erase(task_info->__task_id);
    task_accepted_team_arena_all_set().erase(task_info->__task_id);
    task_accepted_team_arena_win_set().erase(task_info->__task_id);
    task_accepted_ganger_bless_buf_set().erase(task_info->__task_id);
    task_accepted_rob_other_dragon_vale_all_set().erase(task_info->__task_id);
    task_accepted_dragon_vale_step_foot_all_set().erase(task_info->__task_id);
    task_accepted_activeness_of_today_set().erase(task_info->__task_id);
    task_accepted_dragon_vale_building_upgrade_set().erase(task_info->__task_id);
    task_accepted_limit_time_kill_boss_set().erase(task_info->__task_id);
    task_accepted_dragon_keep_all_set().erase(task_info->__task_id);
    task_accepted_dragon_keep_success_set().erase(task_info->__task_id);
    task_accepted_donate_to_gang_set().erase(task_info->__task_id);
    task_accepted_present_buf_to_other_set().erase(task_info->__task_id);
    task_accepted_finish_reward_task_num_set().erase(task_info->__task_id);
    task_accepted_keep_dragon_to_level_set().erase(task_info->__task_id);
    task_accepted_knight_trial_set().erase(task_info->__task_id);
    task_accepted_suit_set().erase(task_info->__task_id);
    task_accepted_role_pull_on_equip_set().erase(task_info->__task_id);
    task_accepted_hero_pull_on_equip_set().erase(task_info->__task_id);
    task_accepted_activate_wing_set().erase(task_info->__task_id);
    task_accepted_get_quality_rune_set().erase(task_info->__task_id);
    task_accepted_pass_kngiht_trials_set().erase(task_info->__task_id);
    task_accepted_have_hero_set().erase(task_info->__task_id);
    task_accepted_have_hero_quality_set().erase(task_info->__task_id);
    task_accepted_first_pass_scene_set().erase(task_info->__task_id);
    task_accepted_dragon_born_set().erase(task_info->__task_id);
    task_accepted_horse_star_level_set().erase(task_info->__task_id);
    task_accepted_have_item_but_no_erase_set().erase(task_info->__task_id);
    task_accepted_have_equip_but_no_erase_set().erase(task_info->__task_id);
    task_accepted_same_gang_challenge_all_set().erase(task_info->__task_id);
    task_accepted_main_scene_npc_battle_set().erase(task_info->__task_id);
    task_accepted_equip_card_all_set().erase(task_info->__task_id);
    task_accepted_equip_card_by_position_set().erase(task_info->__task_id);
    task_accepted_card_upgrade_to_level_all_set().erase(task_info->__task_id);
    task_accepted_card_upgrade_to_level_by_position_set().erase(task_info->__task_id);
    task_accepted_lottery_draw_set().erase(task_info->__task_id);
    task_accepted_equip_jewelry_set().erase(task_info->__task_id);
    task_accepted_invite_friend_step_foot_set().erase(task_info->__task_id);
    task_accepted_kill_group_num_set().erase(task_info->__task_id);
    task_accepted_did_reward_task_num_set().erase(task_info->__task_id);
    task_accepted_alchemy_num_set().erase(task_info->__task_id);
    task_accepted_did_reward_task_num_today_set().erase(task_info->__task_id);
    task_accepted_remote_scene_any_pass_set().erase(task_info->__task_id);
    task_accepted_knight_trial_any_pass_set().erase(task_info->__task_id);
    task_accepted_compose_gemstone_level_set().erase(task_info->__task_id);
    task_accepted_inlay_position_gemstone_set().erase(task_info->__task_id);
    task_accepted_inlay_any_gemstone_set().erase(task_info->__task_id);
    task_accepted_world_boss_set().erase(task_info->__task_id);
    task_accepted_world_boss_immediate_revive_set().erase(task_info->__task_id);
    task_accepted_enchant_any_equip_set().erase(task_info->__task_id);
    task_accepted_inlay_id_gemstone_set().erase(task_info->__task_id);
    task_accepted_gang_well_recover_spirit_set().erase(task_info->__task_id);
    task_accepted_equip_purple_ring_set().erase(task_info->__task_id);
    task_accepted_inlay_level_gemstone_set().erase(task_info->__task_id);
    task_accepted_upgrade_card_set().erase(task_info->__task_id);

//    if (taski->is_main_task() || taski->is_branch_task()) {
//    	task_submited_once_set().insert(taski->__task_id);
//    }
//    notify_task_del(*task_info);
//    unbind_task(taski->__task_id);

    notify_task_del(*taski);
    uint32_t task_id = taski->__task_id;

    //悬赏任务:更改悬赏榜状态
    if(task_info->is_reward_task()) {
    	tasker_detail_no_const().give_up_num += 1;
    	set_reward_task_list_show_status(task_info->__task_id, 2);
    	send_to_client_reward_task_list_show_status();

    	//放弃悬赏任务，扣除领取时获得的道具
    	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
		if(CONFIG_CACHE_ITEM->find_item(cfg.item_id)) {
			int item_nums = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, cfg.item_id);
			if (item_nums >= (int)cfg.item_num) {
				player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
								              Id_Amount(cfg.item_id, cfg.item_num),
						                      Lose_Item_DM_Info(ITEM_LOSE_REWARD_TASK));
			}
		}

		notify_remain_reward_task_num();
    }

    //非悬赏任务放弃后，在主UI可接
    if(!taski->is_reward_task()) {
        unbind_task(taski->__task_id);
        insert_task(task_id, *task_json);
    }

    save_tick();

    return 0;
}

int Tasker::process_submit_imp(TaskInfo *task_info, bool is_test /*= false*/)
{
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (!task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }

	if(!task_info->is_finish()) {
		return ERROR_TASK_NOT_FINISH;
	}

    if(task_json->task_type != TaskInfo::TASK_GTYPE_REWARD_TASK && is_task_submited(task_info->__task_id)) {
    	return ERROR_TASK_SUBMITED;
    }

    if(task_json->task_type == TaskInfo::TASK_GTYPE_REWARD_TASK && task_info->is_submit()) {
    	return ERROR_TASK_SUBMITED;
    }

    //监听物品的任务扣除物品
	int ret = erase_task_listen_item(task_info, is_test);
	if(ret != 0) {
		set_task_status(*task_info, TaskInfo::TASK_STATUS_ACCEPTED);
		notify_task_update(*task_info, *task_json);

    	if(task_info->is_logic_commit_consume_item()) {
    		MSG_81110011 msg;
    		msg.task_id = task_info->__task_id;
    		msg.status = 0;
    		THIS_SEND_TO_CLIENT(msg);
    	}

		//通知场景可以继续采集任务物品和收集打怪掉落物品
		init_task_listen_item_source(task_info);
		return ret;
	}

    int need_pack_grid = 0;
    if (!task_json->award.item_list.empty())
        need_pack_grid += task_json->award.item_list.size();

    //策划需求：交付任务时，若背包已满，则道具以邮件的形式发至玩家邮箱  2014-6-6  gaojianshan
/*    if (need_pack_grid > tasker()->player_self()->pack_get_remain_capacity(Pack::PACK_T_PACKAGE_INDEX)) {
    	return ERROR_PACK_NOT_ENOUGH;
    }*/

    //处理非悬赏任务、非日常周常、非环任务任务奖励
    if(!task_info->is_reward_task()
    		&& !task_info->is_day_task()
    		&& !task_info->is_week_task()
    		&& !task_info->is_ring_task()) {
    	process_receive_normal_task_award(*task_json);
    }

    //处理悬赏任务奖励
    if(task_info->is_reward_task()) {
    	process_receive_reward_task_award(*task_info, *task_json);
    }

    //处理日常周常任务奖励
    if(task_info->is_day_task() || task_info->is_week_task()) {
    	process_receive_day_week_task_award(*task_info);
    }

    //处理环任务奖励
    if(task_info->is_ring_task()) {
    	process_receive_ring_task_award(task_info->__task_id);
    }

    int post_task_id = task_info->__post_task;
    int post_task_id2 = task_info->__post_task2;
    int post_task_id3 = task_info->__post_task3;

    //非悬赏任务、 非开启环任务、非环任务、非结束环任务只能做一次
    if(!task_info->is_reward_task()
    		&& !task_info->is_ring_task()
    		&& !task_info->is_open_ring_task()
    		&& !task_info->is_close_ring_task()) {
    	task_submited_once_set().insert(task_info->__task_id);
    }

	task_accepted_package_set().erase(task_info->__task_id);
    task_accepted_monster_set().erase(task_info->__task_id);
    task_accepted_fb_set().erase(task_info->__task_id);
    task_accepted_arrive_set().erase(task_info->__task_id);
    task_accepted_lvl_set().erase(task_info->__task_id);
    task_accepted_collect_set().erase(task_info->__task_id);
    task_accepted_monster_group_set().erase(task_info->__task_id);
    task_accepted_fb_type_set().erase(task_info->__task_id);
    task_accepted_win_player_set().erase(task_info->__task_id);
    task_accepted_join_gang_set().erase(task_info->__task_id);
   	task_accepted_ganger_level_set().erase(task_info->__task_id);
    task_accepted_player_all_equip_set().erase(task_info->__task_id);
    task_acccepted_body_equip_set().erase(task_info->__task_id);
    task_accepted_body_equip_level_set().erase(task_info->__task_id);
    task_accepted_equip_level_set().erase(task_info->__task_id);
    task_accepted_stone_level_set().erase(task_info->__task_id);
    task_accepted_buy_goods_set().erase(task_info->__task_id);
    task_accepted_get_hero_set().erase(task_info->__task_id);
    task_accepted_fight_set().erase(task_info->__task_id);
    task_accepted_commit_consume_item_set().erase(task_info->__task_id);
    task_accepted_deliver_letter_set().erase(task_info->__task_id);
    task_accepted_help_other_complete_team_scene_set().erase(task_info->__task_id);
    task_accepted_sell_item_set().erase(task_info->__task_id);
//    task_accepted_use_skill_set().erase(task_info->__task_id);
//    task_accepted_append_skill_set().erase(task_info->__task_id);
    task_accepted_limit_time_set().erase(task_info->__task_id);
    task_accepted_arena_set().erase(task_info->__task_id);
    task_accepted_chat_with_friend_set().erase(task_info->__task_id);
    task_accepted_send_mail_set().erase(task_info->__task_id);
    task_accepted_team_scene_or_pvp_set().erase(task_info->__task_id);
    task_accepted_browse_other_player_info_set().erase(task_info->__task_id);
    task_accepted_npc_talk_set().erase(task_info->__task_id);
    task_accepted_arena_level_set().erase(task_info->__task_id);
    task_accepted_equip_crimson_level_set().erase(task_info->__task_id);
	task_accepted_arena_all_set().erase(task_info->__task_id);
	task_accepted_create_dragon_vale_building_set().erase(task_info->__task_id);
	task_accepted_gain_dragon_vale_builiding_production_set().erase(task_info->__task_id);
	task_accepted_wake_up_hero_to_quality_level_set().erase(task_info->__task_id);
	task_accepted_keep_hero_to_level_set().erase(task_info->__task_id);
	task_accepted_compose_equip_set().erase(task_info->__task_id);
	task_accepted_hero_dream_scene_set().erase(task_info->__task_id);
	task_accepted_ganger_skill_level_set().erase(task_info->__task_id);
	task_accepted_war_set().erase(task_info->__task_id);
    task_accepted_war_all_set().erase(task_info->__task_id);
    task_accepted_kill_enemy_in_war_set().erase(task_info->__task_id);
    task_accepted_single_arena_all_set().erase(task_info->__task_id);
    task_accepted_single_arena_win_set().erase(task_info->__task_id);
    task_accepted_team_arena_all_set().erase(task_info->__task_id);
    task_accepted_team_arena_win_set().erase(task_info->__task_id);
    task_accepted_ganger_bless_buf_set().erase(task_info->__task_id);
    task_accepted_rob_other_dragon_vale_all_set().erase(task_info->__task_id);
    task_accepted_dragon_vale_step_foot_all_set().erase(task_info->__task_id);
    task_accepted_activeness_of_today_set().erase(task_info->__task_id);
    task_accepted_dragon_vale_building_upgrade_set().erase(task_info->__task_id);
    task_accepted_limit_time_kill_boss_set().erase(task_info->__task_id);
    task_accepted_dragon_keep_all_set().erase(task_info->__task_id);
    task_accepted_dragon_keep_success_set().erase(task_info->__task_id);
    task_accepted_donate_to_gang_set().erase(task_info->__task_id);
    task_accepted_present_buf_to_other_set().erase(task_info->__task_id);
    task_accepted_finish_reward_task_num_set().erase(task_info->__task_id);
    task_accepted_keep_dragon_to_level_set().erase(task_info->__task_id);
    task_accepted_knight_trial_set().erase(task_info->__task_id);
    task_accepted_suit_set().erase(task_info->__task_id);
    task_accepted_role_pull_on_equip_set().erase(task_info->__task_id);
    task_accepted_hero_pull_on_equip_set().erase(task_info->__task_id);
    task_accepted_activate_wing_set().erase(task_info->__task_id);
    task_accepted_get_quality_rune_set().erase(task_info->__task_id);
    task_accepted_pass_kngiht_trials_set().erase(task_info->__task_id);
    task_accepted_have_hero_set().erase(task_info->__task_id);
    task_accepted_have_hero_quality_set().erase(task_info->__task_id);
    task_accepted_first_pass_scene_set().erase(task_info->__task_id);
    task_accepted_dragon_born_set().erase(task_info->__task_id);
    task_accepted_horse_star_level_set().erase(task_info->__task_id);
    task_accepted_have_item_but_no_erase_set().erase(task_info->__task_id);
    task_accepted_have_equip_but_no_erase_set().erase(task_info->__task_id);
    task_accepted_same_gang_challenge_all_set().erase(task_info->__task_id);
    task_accepted_main_scene_npc_battle_set().erase(task_info->__task_id);
    task_accepted_equip_card_all_set().erase(task_info->__task_id);
    task_accepted_equip_card_by_position_set().erase(task_info->__task_id);
    task_accepted_card_upgrade_to_level_all_set().erase(task_info->__task_id);
    task_accepted_card_upgrade_to_level_by_position_set().erase(task_info->__task_id);
    task_accepted_lottery_draw_set().erase(task_info->__task_id);
    task_accepted_equip_jewelry_set().erase(task_info->__task_id);
    task_accepted_invite_friend_step_foot_set().erase(task_info->__task_id);
    task_accepted_kill_group_num_set().erase(task_info->__task_id);
    task_accepted_did_reward_task_num_set().erase(task_info->__task_id);
    task_accepted_alchemy_num_set().erase(task_info->__task_id);
    task_accepted_did_reward_task_num_today_set().erase(task_info->__task_id);
    task_accepted_remote_scene_any_pass_set().erase(task_info->__task_id);
    task_accepted_knight_trial_any_pass_set().erase(task_info->__task_id);
    task_accepted_compose_gemstone_level_set().erase(task_info->__task_id);
    task_accepted_inlay_position_gemstone_set().erase(task_info->__task_id);
    task_accepted_inlay_any_gemstone_set().erase(task_info->__task_id);
    task_accepted_world_boss_set().erase(task_info->__task_id);
    task_accepted_world_boss_immediate_revive_set().erase(task_info->__task_id);
    task_accepted_enchant_any_equip_set().erase(task_info->__task_id);
    task_accepted_inlay_id_gemstone_set().erase(task_info->__task_id);
    task_accepted_gang_well_recover_spirit_set().erase(task_info->__task_id);
    task_accepted_equip_purple_ring_set().erase(task_info->__task_id);
    task_accepted_inlay_level_gemstone_set().erase(task_info->__task_id);
    task_accepted_upgrade_card_set().erase(task_info->__task_id);


    set_task_status(*task_info, TaskInfo::TASK_STATUS_SUBMIT);

    if(task_info->__game_type == TaskInfo::TASK_GTYPE_DAY) {
    	notify_task_update(*task_info, *task_json);
    }
    else {
    	notify_task_del(*task_info);
    }

    process_scene_npc(*task_info);

    /*//FIXME
    if(task_info->is_logic_limit_time_kill_boss()) {
    	MSG_DEBUG("限时击杀任务 : 通知客户端删除, role_name=%s, task_id=%d", player_self()->role_name().c_str(), task_info->__task_id);
    }*/

    //提交任务特效
    {
		MSG_82300029 msg_82300029;
		msg_82300029.task_id = task_info->__task_id;
		msg_82300029.type = 2;
		THIS_SEND_TO_CLIENT(msg_82300029);
    }

    Time_Value now = Time_Value::gettimeofday();
    Log_Task_Stream(player_self()->role_id(), task_info->__task_id,
    								TASK_STREAM_COMMIT, player_self()->level(),
    								now.sec());

    if(task_info->is_reward_task()) {
    	MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`) VALUES (%d,%ld,%ld,%d)",  5,  player_self()->role_id(), Time_Value::gettimeofday().sec(), player_self()->level());
    }

    process_clear_full_prop_when_task_commit(task_info->__task_id);

//    if (post_task_id <= 0)
//        post_task_id = task_json->postcondition;

    if (post_task_id > 0 && task_submited_once_set().find(post_task_id) == task_submited_once_set().end())
    {
    	const Task_Mission_Detail_Config *post_task_json = CONFIG_CACHE_TASK->find_task(post_task_id);
    	if (post_task_json)
    	{
    		insert_task(post_task_id, *post_task_json);
    	}
    }

    if(post_task_id2 > 0 && task_submited_once_set().find(post_task_id2) == task_submited_once_set().end()) {
    	const Task_Mission_Detail_Config *post_task_json2 = CONFIG_CACHE_TASK->find_task(post_task_id2);
    	if(post_task_json2) {
    		insert_task(post_task_id2, *post_task_json2);
    	}
    }

    if(post_task_id3 > 0 && task_submited_once_set().find(post_task_id3) == task_submited_once_set().end()) {
    	const Task_Mission_Detail_Config *post_task_json3 = CONFIG_CACHE_TASK->find_task(post_task_id3);
    	if(post_task_json3) {
    		insert_task(post_task_id3, *post_task_json3);
    	}
    }

    bool flag = false;
    if(task_json->icon_hero_id && task_json->show_or_close_icon) {
    	for(Task_Icon_Hero_Id_Map::const_iterator it = tasker_detail().task_icon_hero_id_Map.begin();
    			it != tasker_detail().task_icon_hero_id_Map.end();
    			++it) {
    		if(task_json->task_id == (int)it->first && task_json->icon_hero_id != it->second) {
    			flag = true;
    		}
    	}

    	if(flag) {
    		tasker_detail_no_const().task_icon_hero_id_Map.erase(task_json->task_id);
    	}

    	tasker_detail_no_const().task_icon_hero_id_Map.insert(std::make_pair(task_info->__task_id,
    																				task_json->icon_hero_id));
    }

    if(task_json->gift_level != 0) {
//    	tasker_detail_no_const().task_gift_level_set.clear();
    	tasker_detail_no_const().task_gift_level_set.insert(task_info->__task_id);
    }

    //已提交任务，也发客户端的情况
    if (task_json->is_magic_wing
    		|| task_info->__recruid_hero
    		|| tasker_detail_no_const().task_icon_hero_id_Map.end() != tasker_detail_no_const().task_icon_hero_id_Map.find(task_info->__task_id)
    		|| tasker_detail_no_const().task_gift_level_set.end() != tasker_detail_no_const().task_gift_level_set.find(task_info->__task_id)
    		|| (task_info->is_talent_task() && task_json->talent_id)) {
    	notify_task_update(*task_info, *task_json);
    }

    //element: task_id
    if(task_json->show_or_close_icon == 6) {
        std::list<uint32_t> del_task_icon_hero_id_list;
    	for(Task_Icon_Hero_Id_Map::const_iterator it = tasker_detail().task_icon_hero_id_Map.begin();
    			it != tasker_detail().task_icon_hero_id_Map.end();
    			++it) {
    		if(task_json->icon_hero_id == it->second) {
    			del_task_icon_hero_id_list.push_back(it->first);
    		}
    	}

    	for(std::list<uint32_t>::const_iterator it = del_task_icon_hero_id_list.begin();
    			it != del_task_icon_hero_id_list.end();
    			++it) {
    		tasker_detail_no_const().task_icon_hero_id_Map.erase(*it);
    	}

    	del_task_icon_hero_id_list.clear();
    }

    //成就监听限时击杀任务，从接任务到提交任务所用时间(单位：秒)
    if(task_info->is_logic_limit_time_kill_boss() && task_json->limit_time && task_json->is_limit_time) {
        long use_time = 0;
        if(task_info->__accept_tick.sec() <= now.sec()) {
        	use_time = now.sec() - task_info->__accept_tick.sec();
        }
        else {
        	use_time = 0;
        }
        player_self()->achievement_listen_task_limit_time((int)use_time);
    }

    process_safe_guide(task_info->__task_id);

    //悬赏任务:更改悬赏榜状态
    if(task_info->is_reward_task()) {
    	player_self()->active_content_listen_reward_task_done();

    	if(tasker_detail_->did_reward_task_num < INT_MAX) {
    		++tasker_detail_->did_reward_task_num;
    	}

    	if(tasker_detail_->did_reward_task_num_today < INT_MAX) {
    		++tasker_detail_->did_reward_task_num_today;
    	}

    	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
		uint32_t max_accept_count = cfg.accept_max_count_per_day
									+ player_self()->achievement_reward_times()
									+ tasker_detail_->give_up_num
									+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);
		if(tasker_detail_->used_accept_count >= max_accept_count) {
			tasker_detail_->is_close_auto_refresh = true;
			tasker_detail_->is_open_countdown = false;
			MSG_81110010 msg_811100010;
			THIS_SEND_TO_CLIENT(msg_811100010);
		}

    	set_reward_task_list_show_status(task_info->__task_id, 3);
    	send_to_client_reward_task_list_show_status();

    	//提交悬赏任务，扣除领取时获得的道具
		if(CONFIG_CACHE_ITEM->find_item(cfg.item_id)) {
			int item_nums = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, cfg.item_id);
			if (item_nums >= (int)cfg.item_num) {
				player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
								              Id_Amount(cfg.item_id, cfg.item_num),
						                      Lose_Item_DM_Info(ITEM_LOSE_REWARD_TASK));
			}
		}

		notify_remain_reward_task_num();

    	//通知小助手，悬赏任务完成
    	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_REWARD_TASK);

    	task_listen_finish_reward_task_num();
    	task_listen_did_reward_task_num(tasker_detail_->did_reward_task_num);
    	task_listen_did_reward_task_num_today(tasker_detail_->did_reward_task_num_today);
    }

    if(task_info->is_talent_task() && task_json->talent_id) {
    	if(is_finish_last_talent_task(task_json->talent_id)) {
    		MSG_81110018 msg;
    		msg.type = 0;
    		msg.talent_id = task_json->talent_id;
    		THIS_SEND_TO_CLIENT(msg);
    	}
    }

    if(task_json->is_magic_wing && is_last_magic_wing_task_submit()) {
    	MSG_81110018 msg;
    	msg.type = 1;
    	msg.talent_id = 0;
    	THIS_SEND_TO_CLIENT(msg);
    }

    if(task_info->is_ring_task() || task_info->is_open_ring_task()) {
    	if(task_info->is_ring_task()) {
    		++tasker_detail_->did_ring_num;
    		hook_ring_task_complete(task_info->__task_id);
    		//refresh_close_ring_task must before  refresh_ring_task
    		refresh_close_ring_task();
    	}
    	refresh_ring_task();
    }
    else if(task_info->is_close_ring_task()) {
    	tasker_detail_->is_submit_close_ring_task = true;
    }
    else if(!task_info->is_reward_task()) {
    	unbind_task(task_info->__task_id);
    }

    save_tick();

	player_self()->title_listen_task_finish();
	player_self()->achievement_listen_task_complete_nums();

	bool is_exist_main_line_task = false;
	for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
		if(it->second.__game_type == TaskInfo::TASK_GTYPE_MAINLINE) {
			is_exist_main_line_task = true;
		}
	}

	if(!is_exist_main_line_task && !is_last_main_line_task_submit()) {
		task_construct();
	}

    return 0;
}

int Tasker::process_fast_finish_imp(TaskInfo *task_info, const uint8_t is_bind_diamond_first)
{
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (!task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }

    if(!task_json->is_fast_complete) {
    	return ERROR_TASK_NOT_FAST_FINISH;
    }

    if(task_json->task_type != TaskInfo::TASK_GTYPE_REWARD_TASK && is_task_submited(task_info->__task_id)) {
    	return ERROR_TASK_SUBMITED;
    }

    if(task_json->task_type == TaskInfo::TASK_GTYPE_REWARD_TASK && task_info->is_submit()) {
    	return ERROR_TASK_SUBMITED;
    }

    if(task_info->is_finish()) {
    	return ERROR_TASK_FINISHED;
    }

    if(!task_info->is_accepted()) {
    	return ERROR_TASK_ACCEPT_NOT_NOW;
    }

    bool is_consume_item = task_json->item_id > 0 && task_json->item_num > 0;
    bool is_consume_money = task_json->money_type > 0 && task_json->amount > 0;

    //悬赏任务判断是否有悬赏精灵
    if(TaskInfo::TASK_GTYPE_REWARD_TASK == task_info->__game_type) {
        bool is_reward_covenant = false;
        is_reward_covenant = player_self()->get_covenant_status(REWARD_COVENANT);
        if(is_reward_covenant) {
        	task_info->__is_req_fast_finish = true;

        	//通知用悬赏精灵完成悬赏任务
        	MSG_81110008 msg;
        	msg.task_id = task_info->__task_id;
        	THIS_SEND_TO_CLIENT(msg);

        	this->process_finish_imp(task_info);
        	return 0;
        }
    }

    if(!is_consume_item && !is_consume_money) {
    	task_info->__is_req_fast_finish = true;
    	this->process_finish_imp(task_info);
    	return 0;
    }

    if(is_consume_item && !is_consume_money) {
    	//判断物品是否足够
    	int pack_item_amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,
    			task_json->item_id);
    	if(pack_item_amount < task_json->item_num) {
    		return ERROR_ITEM_NOT_ENOUGH;
    	}

    	//扣物品
		int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
				 Id_Amount(task_json->item_id, task_json->item_num),
				 Lose_Item_DM_Info(ITEM_LOST_FAST_FINISH));
		if (0 != result) {
			MSG_USER("error :sub item failed, task_id:%d, item_id:%d, item_num:%d\n",
					task_info->__task_id, task_json->item_id, task_json->item_num);
			return ERROR_SUB_ITEM_FAIL;
		}

    	task_info->__is_req_fast_finish = true;
    	this->process_finish_imp(task_info);
    	return 0;
    }

    if(!is_consume_item && is_consume_money) {
    	//判断是否有足够的货币
    	int money_type = task_json->money_type;
    	if(BIND_GOLD_FIRST == money_type) {
    		if(is_bind_diamond_first == 1) {
    			money_type = BIND_GOLD_FIRST;
    		}
    		else {
    			money_type = GOLD_ONLY;
    		}
    	}

    	int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(money_type),
    			task_json->amount, Money_DM_Info(MONEY_SUB_FAST_TASK_CONSUME)));
    	if(return_status != 0) {
    		switch(money_type) {
    		case COPPER_ONLY:
    			return ERROR_COPPER_NOT_ENOUGH;
    		case BIND_GOLD_FIRST:
    			return ERROR_GOLD_NOT_ENOUGH;
    		case GOLD_ONLY:
    			return ERROR_GOLD_NOT_ENOUGH;
    		case BIND_GOLD_ONLY:
    			return ERROR_BIND_GOLD_NOT_ENOUGH;
    		case DRAGON_SOUL_ONLY:
    			return ERROR_DRAGON_SOUL_NOT_ENOUGH;
    		case SOULS_ONLY:
    			return ERROR_SOULS_NOT_ENOUGH;
    		case FRIENDSHIP_ONLY:
    			return ERROR_FRIENDSHIP_NOT_ENOUGH;
    		}

    		return ERROR_MONEY_TYPE_ILLEGAL;
    	}

    	//减掉相应的货币
    	if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(money_type),
    			task_json->amount, Money_DM_Info(MONEY_SUB_FAST_TASK_CONSUME))) != 0) {
    		return ERROR_SUB_MONEY_FAIL;
    	}

    	task_info->__is_req_fast_finish = true;
    	this->process_finish_imp(task_info);
    	return 0;
    }

    if(is_consume_item && is_consume_money) {
    	if(0 == task_info->__item_money_flag) {
        	int pack_item_amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,
        			task_json->item_id);
        	if(pack_item_amount >= task_json->item_num) {
            	//扣物品
        		int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
        				 Id_Amount(task_json->item_id, task_json->item_num),
        				 Lose_Item_DM_Info(ITEM_LOST_FAST_FINISH));
        		if (0 != result) {
        			MSG_USER("error :sub item failed, task_id:%d, item_id:%d, item_num:%d\n",
        					task_info->__task_id, task_json->item_id, task_json->item_num);
        			return ERROR_SUB_ITEM_FAIL;
        		}

        		task_info->__is_req_fast_finish = true;
        		this->process_finish_imp(task_info);
        		return 0;
        	} else {
        		MSG_81110005 msg;
        		msg.task_id = task_info->__task_id;
        		msg.money_type = task_json->money_type;
        		msg.money_num = task_json->amount;
        		THIS_SEND_TO_CLIENT(msg);
        		return 0;
        	}
    	}

    	if(1 == task_info->__item_money_flag) {
        	//判断是否有足够的货币
        	int money_type = task_json->money_type;
        	if(BIND_GOLD_FIRST == money_type) {
        		if(is_bind_diamond_first == 1) {
        			money_type = BIND_GOLD_FIRST;
        		}
        		else {
        			money_type = GOLD_ONLY;
        		}
        	}

        	int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(money_type),
        			task_json->amount, Money_DM_Info(MONEY_SUB_FAST_TASK_CONSUME)));
        	if(return_status != 0) {
        		switch(money_type) {
        		case COPPER_ONLY:
        			return ERROR_COPPER_NOT_ENOUGH;
        		case BIND_GOLD_FIRST:
        			return ERROR_GOLD_NOT_ENOUGH;
        		case GOLD_ONLY:
        			return ERROR_GOLD_NOT_ENOUGH;
        		case BIND_GOLD_ONLY:
        			return ERROR_BIND_GOLD_NOT_ENOUGH;
        		case SOULS_ONLY:
        			return ERROR_SOULS_NOT_ENOUGH;
        		case FRIENDSHIP_ONLY:
        			return ERROR_FRIENDSHIP_NOT_ENOUGH;
        		}
        		return ERROR_MONEY_TYPE_ILLEGAL;
        	}

        	//减掉相应的货币
        	if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(money_type),
        			task_json->amount, Money_DM_Info(MONEY_SUB_FAST_TASK_CONSUME))) != 0) {
        		return ERROR_SUB_MONEY_FAIL;
        	}

        	task_info->__is_req_fast_finish = true;
        	this->process_finish_imp(task_info);
    	}
    	return 0;
    }

    return 0;
}

int Tasker::process_finish_imp(TaskInfo *task_info, bool is_test /*= false*/)
{
/*
//	erase_task_listen_item(task_info);
//	task_accepted_package_set().erase(task_info->__task_id);
    task_accepted_lvl_set().erase(task_info->__task_id);
    task_accepted_monster_set().erase(task_info->__task_id);
    task_accepted_monster_group_set().erase(task_info->__task_id);
//    task_accepted_collect_set().erase(task_info->__task_id);
    task_accepted_fb_type_set().erase(task_info->__task_id);
    task_accepted_fb_set().erase(task_info->__task_id);
    task_accepted_arrive_set().erase(task_info->__task_id);
    task_accepted_win_player_set().erase(task_info->__task_id);
    task_accepted_join_gang_set().erase(task_info->__task_id);
   	task_accepted_gang_level_set().erase(task_info->__task_id);
    task_accepted_player_all_equip_set().erase(task_info->__task_id);
    task_acccepted_body_equip_set().erase(task_info->__task_id);
    task_accepted_body_equip_level_set().erase(task_info->__task_id);
    task_accepted_equip_level_set().erase(task_info->__task_id);
    task_accepted_stone_level_set().erase(task_info->__task_id);
    task_accepted_buy_goods_set().erase(task_info->__task_id);
    task_accepted_get_hero_set().erase(task_info->__task_id);
    task_accepted_fight_set().erase(task_info->__task_id);
    task_accepted_commit_consume_item_set().erase(task_info->__task_id);
    task_accepted_deliver_letter_set().erase(task_info->__task_id);
    task_accepted_help_other_complete_team_scene_set().erase(task_info->__task_id);
    task_accepted_sell_item_set().erase(task_info->__task_id);
//    task_accepted_use_skill_set().erase(task_info->__task_id);
//    task_accepted_append_skill_set().erase(task_info->__task_id);
*/
    set_task_status(*task_info, TaskInfo::TASK_STATUS_FINISH);
    save_tick();

    process_scene_npc(*task_info);

    Time_Value now = Time_Value::gettimeofday();
    Log_Task_Stream(player_self()->role_id(), task_info->__task_id,
    								TASK_STREAM_FINISH, player_self()->level(),
    								now.sec());

    //通知场景不能继续采集任务物品和收集打怪掉落物品
    erase_task_listen_item_source(task_info);

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (!task_json) {
    	return ERROR_CONFIG_NOT_EXIST;
    }
    if (task_json->complete.is_empty) {
    	//此处禁止发任务更新消息，否则，客户端会报错
    	//自动提交的非主线任务，弹奖励框
    	if(!task_info->is_main_task()) {
    		/*//FIXME
    		if(task_info->is_logic_limit_time_kill_boss()) {
        		MSG_DEBUG("限时击杀任务 ：更新状态为完成, role_name=%s, task_id=%d",
        				player_self()->role_name().c_str(), task_info->__task_id);
    		}*/


//    		if(task_info->is_reward_task()) {
    			notify_task_update(*task_info, *task_json);
//    		}

    		if(!task_info->is_day_task() ||
    				(task_info->is_day_task() && task_json->is_show_in_task_list)) {
    			MSG_81110006 msg;
    			msg.task_id = task_info->__task_id;
    			msg.is_window = 1;
    			THIS_SEND_TO_CLIENT(msg);
    		}

			/*//FIXME
			if(task_info->is_logic_limit_time_kill_boss()) {
				MSG_DEBUG("限时击杀任务: 完成，弹出奖励框, role_name=%s, task_id=%d",
						player_self()->role_name().c_str(), task_info->__task_id);
			}*/

    	}

        //监听任务状态，如果满足任务状态,通知客户端开启图标
        player_self()->notify_open_icon_by_task_status();

        if(task_info->is_main_task()) {
        	this->process_submit_imp(task_info, is_test);
        }

    	return (int)TaskInfo::TASK_STATUS_SUBMIT;
    } else {
    	notify_task_update(*task_info, *task_json);
        save_tick();
    	return (int)TaskInfo::TASK_STATUS_FINISH;
    }
    return 0;
}


int Tasker::process_listen_kill_monster(TaskInfo *task_info, const int monster_id, const int monster_num)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return -1;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        bool is_not_exist_in_array = !task_json->execute[i].id_list.empty()
        		 && (task_json->execute[i].id_list.find(monster_id) == task_json->execute[i].id_list.end());
        bool is_not_match_id = task_json->execute[i].id_list.empty()
        		&& task_json->execute[i].id != 0
        		&& monster_id != task_json->execute[i].id;

		if(TaskExecType::TASK_EXEC_KILL_MONSTER == task_json->execute[i].type
				&& (is_not_exist_in_array || is_not_match_id)) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_KILL_MONSTER) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += monster_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int Tasker::process_listen_fb(TaskInfo *task_info, const int sceneid)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if(TaskExecType::TASK_EXEC_FB == task_json->execute[i].type && sceneid != task_json->execute[i].id) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_FB) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        } else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        } else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
    return 0;
}

int Tasker::process_listen_arrive(TaskInfo *task_info, const int sceneid, const int target_x, const int target_y)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_ARRIVE
            				&& task_json->execute[i].sceneid != sceneid) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ARRIVE){////#do
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
            	is_all_pass = false;
            }
            continue;
		}

		if (calculate_pixels_distance_from_pixels(target_x, target_y,
				                                  task_json->execute[i].target_x,
				                                  task_json->execute[i].target_y) > (int)task_json->execute[i].param1) {
			is_all_pass = false;
//        	notify_task_update(*task_info, *task_json);
			continue;
		}

		task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
    return 0;
}

int Tasker::process_listen_lvl_up(TaskInfo *task_info, const int target_level)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_LVL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value = target_level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
    return 0;
}

int Tasker::process_listen_collect_item(TaskInfo *task_info, const int item_id, const int item_num, const int bind)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(TaskExecType::TASK_EXEC_COLLECT == task_json->execute[i].type && item_id != task_json->execute[i].id) {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_COLLECT) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		int old_num = task_cond->__current_value;
		task_cond->__current_value = item_num;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
		else {
			is_all_pass = false;
			if (task_info->is_finish()) {
		    	set_task_status(*task_info, TaskInfo::TASK_STATUS_ACCEPTED);
		    	notify_task_update(*task_info, *task_json);
		    	//通知场景可以继续采集任务物品
		    	init_task_listen_item_source(task_info);
			} else {
				if(old_num != (int)item_num) {
					notify_task_update(*task_info, *task_json);
				}
		    	//通知场景可以继续采集任务物品
		    	init_task_listen_item_source(task_info);
			}
		}
    }
    if (is_all_pass == true && task_info->is_accepted())
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_kill_monster_group(TaskInfo *task_info, const int sceneid,
												const int monster_point)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        Scene_Monster_Point scene_monster_point;
        scene_monster_point.scene_id = (uint32_t)sceneid;
        scene_monster_point.monster_point = (uint32_t)monster_point;

		if(TaskExecType::TASK_EXEC_KILL_MONSTER_GROUP == task_json->execute[i].type &&
				task_json->execute[i].scene_monster_point_set.find(scene_monster_point)
				== task_json->execute[i].scene_monster_point_set.end()) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_KILL_MONSTER_GROUP) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
    return 0;
}

int Tasker::process_listen_package_item(TaskInfo *task_info, const int item_id, const int item_num, const int bind)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        //id:物品id  数量：物品数量  param1:怪物id  param2:掉落概率
		if(TaskExecType::TASK_EXEC_PACKAGE == task_json->execute[i].type &&
				item_id != task_json->execute[i].id) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_PACKAGE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		int old_num = task_cond->__current_value;
		task_cond->__current_value = item_num;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
		else {
			is_all_pass = false;
			if (task_info->is_finish()) {
				set_task_status(*task_info, TaskInfo::TASK_STATUS_ACCEPTED);
		    	notify_task_update(*task_info, *task_json);
		    	//通知场景可以继续收集打怪掉落物品
		    	init_task_listen_item_source(task_info);
			} else {
				if(old_num != (int)item_num) {
					notify_task_update(*task_info, *task_json);
				}
		    	//通知场景可以继续采集任务物品
		    	init_task_listen_item_source(task_info);
			}
		}
    }
    if (is_all_pass == true && task_info->is_accepted())
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_fb_type(TaskInfo *task_info, const int &type)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        bool is_not_exist_in_array = !task_json->execute[i].id_list.empty()
        		 && (task_json->execute[i].id_list.find(type) == task_json->execute[i].id_list.end());

        if(TaskExecType::TASK_EXEC_FB_TYPE == task_json->execute[i].type
        		&& is_not_exist_in_array
        		&& type != task_json->execute[i].id) {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_FB_TYPE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
    return 0;
}

int Tasker::process_listen_win_player(TaskInfo *task_info, const role_id_t role_id)
{
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_WIN_PLAYER) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
    return 0;
}

int Tasker::process_listen_join_gang(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_JOIN_GANG) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }//end for

    if (is_all_pass == true) {
        this->process_finish_imp(task_info);
    }
	return 0;
}


int Tasker::process_listen_gang_level(TaskInfo *task_info, int gang_level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_GANGER_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value = gang_level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_player_all_equip_num(TaskInfo *task_info, int equip_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_PLAYER_ALL_EQUIP) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value += equip_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_body_equip_num(TaskInfo *task_info, int position, int equip_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(TaskExecType::TASK_EXEC_BODY_EQUIP == task_json->execute[i].type
                    				&& position != (int)task_json->execute[i].param1) {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_BODY_EQUIP) { //身体部位
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value += equip_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_body_equip_level(TaskInfo *task_info, int position,int equip_level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(TaskExecType::TASK_EXEC_BODY_EQUIP_LEVEL == task_json->execute[i].type
				&& position != (int)task_json->execute[i].param1) {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_BODY_EQUIP_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value = equip_level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_equip_level(TaskInfo *task_info, uint32_t equip_id, int equip_level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(TaskExecType::TASK_EXEC_EQUIP_LEVEL == task_json->execute[i].type
                    				&& task_json->execute[i].id_list.find((int32_t)equip_id) == task_json->execute[i].id_list.end()) {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_EQUIP_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value = equip_level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_stone_level(TaskInfo *task_info, int stone_level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_STONE_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value = stone_level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_buy_goods(TaskInfo *task_info, uint32_t item_id, int item_type, int32_t item_count) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(TaskExecType::TASK_EXEC_BUY_GOODS == task_json->execute[i].type) {
			if(task_json->execute[i].param1 && item_id != task_json->execute[i].param1) {
				is_all_pass = false;
				continue;
			}
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_BUY_GOODS) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value += item_count;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if(task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_get_hero(TaskInfo *task_info, int32_t hero_id) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if(TaskExecType::TASK_EXEC_GET_HERO == task_json->execute[i].type
				&& hero_id != (int32_t)task_json->execute[i].param1) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_GET_HERO) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value = +1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_fight(TaskInfo *task_info, const int scene_id, const int monster_point) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        Scene_Monster_Point scene_monster_point;
        scene_monster_point.scene_id = scene_id;
        scene_monster_point.monster_point = monster_point;

		if(TaskExecType::TASK_EXEC_FIGHT == task_json->execute[i].type &&
				task_json->execute[i].scene_monster_point_set.find(scene_monster_point)
					== task_json->execute[i].scene_monster_point_set.end()) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_FIGHT) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value = +1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_commit_consume_item(TaskInfo *task_info,
		uint32_t item_id, uint32_t item_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(TaskExecType::TASK_EXEC_COMMIT_CONSUME_ITEM == task_json->execute[i].type &&
                    				(item_id != (uint32_t)task_json->execute[i].id)) {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_COMMIT_CONSUME_ITEM) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		int old_num = task_cond->__current_value;
		task_cond->__current_value = item_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	if(old_num != (int)item_num) {
            	notify_task_update(*task_info, *task_json);
    			MSG_81110011 msg;
    			msg.task_id = task_info->__task_id;
    			msg.status = 0;
    			THIS_SEND_TO_CLIENT(msg);
        	}
        }
    }

    if (is_all_pass == true) {
		notify_task_update(*task_info, *task_json);
		MSG_81110011 msg;
		msg.task_id = task_info->__task_id;
		msg.status = 1;
		THIS_SEND_TO_CLIENT(msg);
		return 0;
    }

	return 0;
}

int Tasker::process_listen_deliver_letter(TaskInfo *task_info, uint32_t item_id, uint32_t item_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(TaskExecType::TASK_EXEC_DELIVER_LETTER == task_json->execute[i].type &&
				(item_id != (uint32_t)task_json->execute[i].id)) {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DELIVER_LETTER) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		int old_num = task_cond->__current_value;
		task_cond->__current_value  = item_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	if(old_num != (int)item_num) {
        		notify_task_update(*task_info, *task_json);
        	}
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_help_other_complete_team_scene(TaskInfo *task_info, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HELP_OTHER_COMPLETE_TEAM_SCENE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value  += num;  //协助其他玩家完成多人副本的次数累加 注： 一个任务中，不会同时出现2个或者2个以上的type=111133的execute
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_sell_item(TaskInfo *task_info, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_SELL_ITEM) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		task_cond->__current_value  += num;  //出售物品个数累加 注： 一个任务中，不会同时出现2个或者2个以上的type=111134的execute
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_chat_with_friend(TaskInfo *task_info, role_id_t friend_id) {
	if(!task_info) {
		return 0;
	}

	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		//每个好友只记录1次聊天
		if(TaskExecType::TASK_EXEC_CHAT_WITH_FRIEND == task_json->execute[i].type &&
				task_cond->ext_set.find(friend_id) != task_cond->ext_set.end()) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_CHAT_WITH_FRIEND) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->ext_set.insert(friend_id);
		task_cond->__current_value += 1;

		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_arena(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ARENA) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_send_mail(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_SEND_MAIL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_team_scene_or_pvp(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_TEAM_SCENE_OR_PVP) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_browse_other_player_info(TaskInfo *task_info, role_id_t role_id) {
	if(!task_info) {
		return 0;
	}

	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		//每个被查看的玩家只记录一次
		if(TaskExecType::TASK_EXEC_BROWSE_OTHER_PLAYER_INFO == task_json->execute[i].type &&
				task_cond->ext_set.find(role_id) != task_cond->ext_set.end()) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_BROWSE_OTHER_PLAYER_INFO) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->ext_set.insert(role_id);
		task_cond->__current_value += 1;

		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_npc_talk(TaskInfo *task_info, uint32_t npc_id) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_NPC_TALK &&
        		task_json->execute[i].talk != npc_id) {
    		is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_NPC_TALK) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        } else if (task_cond->__current_value > task_json->execute[i].amount) {
           	if(task_info->is_accepted()) {
            		task_cond->__current_value = task_json->execute[i].amount;
            	} else {
            		return -1;//当前条件已经完成
            	}
        } else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_arena_level(TaskInfo *task_info, uint32_t group_id, uint32_t level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_ARENA_LEVEL &&
        		task_json->execute[i].id != (int)group_id) {
    		is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ARENA_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        } else if (task_cond->__current_value > task_json->execute[i].amount) {
           	if(task_info->is_accepted()) {
            		task_cond->__current_value = task_json->execute[i].amount;
            	} else {
            		return -1;//当前条件已经完成
            	}
        } else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_equip_crimson_level(TaskInfo *task_info, uint32_t position, uint32_t level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_EQUIP_CRIMSON_LEVEL &&
        		task_json->execute[i].id != (int)position) {
    		is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_EQUIP_CRIMSON_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        } else if (task_cond->__current_value > task_json->execute[i].amount) {
           	if(task_info->is_accepted()) {
            		task_cond->__current_value = task_json->execute[i].amount;
            	} else {
            		return -1;//当前条件已经完成
            	}
        } else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_arena_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ARENA_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_create_dragon_vale_building(TaskInfo *task_info, uint8_t building_type) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_CREATE_DRAGON_VALE_BUILDING &&
				task_json->execute[i].id != building_type) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_CREATE_DRAGON_VALE_BUILDING) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value += 1;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_gain_dragon_vale_builiding_production(TaskInfo *task_info, uint8_t building_type) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION &&
				task_json->execute[i].id != building_type) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value += 1;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_wake_up_hero_to_quality_level(TaskInfo *task_info, uint32_t hero_id, uint32_t quality) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_WAKE_UP_HERO_TO_QUALITY_LEVEL &&
				task_json->execute[i].id != (int)hero_id) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_WAKE_UP_HERO_TO_QUALITY_LEVEL) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value = quality;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_keep_hero_to_level(TaskInfo *task_info, uint32_t hero_id, uint32_t hero_level) {
	if(!task_info) {
		return 0;
	}

	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_KEEP_HERO_TO_LEVEL &&
				task_json->execute[i].id != (int)hero_id) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_KEEP_HERO_TO_LEVEL) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value = hero_level;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_compose_equip(TaskInfo *task_info, uint32_t equip_id) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_COMPOSE_EQUIP &&
				task_json->execute[i].id_list.find((int32_t)equip_id) == task_json->execute[i].id_list.end()) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_COMPOSE_EQUIP) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value += 1;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_hero_dream_scene(TaskInfo *task_info, uint32_t id) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HERO_DREAM_SCENE &&
				task_json->execute[i].id != (int)id) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HERO_DREAM_SCENE) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value += 1;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_ganger_skill_level(TaskInfo *task_info, uint32_t id) {
	if(!task_info) {
		return 0;
	}

	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_GANGER_SKILL_LEVEL &&
				task_json->execute[i].id != (int)id) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_GANGER_SKILL_LEVEL) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value += 1;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_war(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_WAR) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_war_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_WAR_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_kill_enemy_in_war(TaskInfo *task_info, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_KILL_ENEMY_IN_WAR) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_single_arena_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_SINGLE_ARENA_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_single_arena_win(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_SINGLE_ARENA_WIN) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_team_arena_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_TEAM_ARENA_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_team_arena_win(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_TEAM_ARENA_WIN) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_ganger_bless_buf(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_GANGER_BLESS_BUF) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_rob_other_dragon_vale_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ROB_OTHER_DRAGON_VALE_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_dragon_vale_step_foot_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DRAGON_VALE_STEP_FOOT_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_activeness_of_today(TaskInfo *task_info, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ACTIVENESS_OF_TODAY) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_dragon_vale_building_upgrade(TaskInfo *task_info, uint8_t type, uint16_t level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		bool no_finish_flag = (type != task_json->execute[i].param1) || (level < task_json->execute[i].param2);
		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_DRAGON_VALE_BUILDING_UPGRADE
				&& no_finish_flag) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DRAGON_VALE_BUILDING_UPGRADE) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value += 1;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_limit_time_kill_boss(TaskInfo *task_info,
									uint32_t monster_id, uint32_t monster_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}
	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		//note: id: task_id  scene_id: 限时击杀场景id  param1:怪点  param2:怪物id
		bool no_finish_flag = (monster_id != task_json->execute[i].param2);
		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_LIMIT_TIME_KILL_BOSS
				&& no_finish_flag) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_LIMIT_TIME_KILL_BOSS) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value += (int)monster_num;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}//end for

	if (is_all_pass == true) {
		/*//FIXME
		MSG_DEBUG("限时击杀任务:  完成 role_name=%s, task_id=%d", player_self()->role_name().c_str(), task_info->__task_id);
		*/
		this->process_finish_imp(task_info);
	}

	return is_all_pass;
}

int Tasker::process_listen_dragon_keep_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DRAGON_KEEP_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_dragon_keep_success(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DRAGON_KEEP_SUCCESS) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_donate_to_gang(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DONATE_TO_GANG) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_present_buf_to_other(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_PRESENT_BUF_TO_OTHER) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_finish_reward_task_num(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_FINISH_REWARD_TASK_NUM) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_keep_dragon_to_level(TaskInfo *task_info, uint8_t keep_type, uint32_t keep_level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_KEEP_DRAGON_TO_LEVEL
        		&& keep_type != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_KEEP_DRAGON_TO_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = keep_level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_knight_trial(TaskInfo *task_info, uint8_t cur_points) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_KNIGHT_TRIAL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = cur_points;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_suit(TaskInfo *task_info, int id, int curr_cnt) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_SUIT
        		&& task_json->execute[i].id_list.find((int32_t)id) == task_json->execute[i].id_list.end()) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_SUIT) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

//        task_cond->__current_value = 1;
		task_cond->__current_value = curr_cnt;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_role_pull_on_equip(TaskInfo *task_info, uint32_t equip_id) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_ROLE_PULL_ON_EQUIP
        		&& task_json->execute[i].id_list.find((int32_t)equip_id) == task_json->execute[i].id_list.end()) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ROLE_PULL_ON_EQUIP) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_hero_pull_on_equip(TaskInfo *task_info, uint32_t equip_id) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HERO_PULL_ON_EQUIP
        		&& task_json->execute[i].id_list.find((int32_t)equip_id) == task_json->execute[i].id_list.end()) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HERO_PULL_ON_EQUIP) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_activate_wing(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ACTIVATE_WING) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_get_quality_rune(TaskInfo *task_info, uint32_t quality, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        //id: quality amount:数量
        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_GET_QUALITY_RUNE
        		&& (int)quality != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_GET_QUALITY_RUNE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_pass_kngiht_trials(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_PASS_KNGIHT_TRIALS) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_have_hero(TaskInfo *task_info, uint32_t hero_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HAVE_HERO) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = hero_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_have_hero_quality(TaskInfo *task_info, uint32_t quality, uint32_t hero_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        //id: quality amount:数量
        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HAVE_HERO_QUALITY
        		&& (int)quality < task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HAVE_HERO_QUALITY) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = hero_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_first_pass_scene(TaskInfo *task_info, uint32_t scene_id) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        //id: quality amount:数量
        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_FIRST_PASS_SCENE
        		&& (int)scene_id != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_FIRST_PASS_SCENE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_dragon_born(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DRAGON_BORN) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_horse_star_level(TaskInfo *task_info, uint32_t horse_id, uint32_t star_level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HORSE_STAR_LEVEL
        		&& (int)horse_id != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HORSE_STAR_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = star_level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_have_item_but_no_erase(TaskInfo *task_info, uint32_t item_id, uint32_t item_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HAVE_ITEM_BUT_NO_ERASE
        		&& (int)item_id != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HAVE_ITEM_BUT_NO_ERASE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

		int old_num = task_cond->__current_value;
        task_cond->__current_value = item_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	if(old_num != (int)item_num) {
        		notify_task_update(*task_info, *task_json);
        	}
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_have_equip_but_no_erase(TaskInfo *task_info, uint32_t item_id, uint32_t item_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HAVE_EQUIP_BUT_NO_ERASE
        		&& task_json->execute[i].id_list.find((int32_t)item_id) == task_json->execute[i].id_list.end()) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HAVE_EQUIP_BUT_NO_ERASE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = item_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_same_gang_challenge_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_SAME_GANG_CHALLENGE_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_main_scene_npc_battle(TaskInfo *task_info, uint32_t monster_id, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_MAIN_SCENE_NPC_BATTLE
        		&& (int)monster_id != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_MAIN_SCENE_NPC_BATTLE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true) {
    	this->process_finish_imp(task_info);
    }

	return 0;
}

int Tasker::process_listen_equip_card_all(TaskInfo *task_info, int num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_EQUIP_CARD_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
    return 0;
}

int Tasker::process_listen_equip_card_by_position(TaskInfo *task_info, const uint32_t position, int num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_EQUIP_CARD_BY_POSITION
        		&& (int)position != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_EQUIP_CARD_BY_POSITION) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_card_upgrade_to_level_all(TaskInfo *task_info, const uint32_t level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_CARD_UPGRADE_TO_LEVEL_ALL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_card_upgrade_to_level_by_position(TaskInfo *task_info, const uint32_t position, const uint32_t level) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_CARD_UPGRADE_TO_LEVEL_BY_POSITION
        		&& (int)position != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_CARD_UPGRADE_TO_LEVEL_BY_POSITION) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = level;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_lottery_draw(TaskInfo *task_info, const uint32_t times) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_LOTTERY_DRAW) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += times;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_equip_jewelry(TaskInfo *task_info, const uint32_t quality, const uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_EQUIP_JEWELRY
        		&& (int)quality < task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_EQUIP_JEWELRY) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_invite_friend_step_foot(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_INVITE_FRIEND_STEP_FOOT) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_kill_group_num(TaskInfo *task_info, const uint32_t group_id) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_KILL_GROUP_NUM
        		&& (int)group_id != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_KILL_GROUP_NUM) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_did_reward_task_num(TaskInfo *task_info, const uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DID_REWARD_TASK_NUM) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = (int)num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_alchemy_num(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ALCHEMY_NUM) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_did_reward_task_num_today(TaskInfo *task_info, const uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_DID_REWARD_TASK_NUM_TODAY) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = (int)num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_remote_scene_any_pass(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_REMOTE_SCENE_ANY_PASS) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_knight_trial_any_pass(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_KNIGHT_TRIAL_ANY_PASS) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_compose_gemstone_level(TaskInfo *task_info, uint32_t stone_level, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_COMPOSE_GEMSTONE_LEVEL
        		&& (int)stone_level != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_COMPOSE_GEMSTONE_LEVEL) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += (int)num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_inlay_any_gemstone(TaskInfo *task_info, int num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_INLAY_ANY_GEMSTONE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_inlay_position_gemstone(TaskInfo *task_info, const uint32_t position, const uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_INLAY_POSITION_GEMSTONE
        		&& (int)position != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_INLAY_POSITION_GEMSTONE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_world_boss(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_WORLD_BOSS) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_world_boss_immediate_revive(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_WORLD_BOSS_IMMEDIATE_REVIVE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_enchant_any_equip(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ENCHANT_ANY_EQUIP) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_inlay_gemsteone_id(TaskInfo *task_info, uint32_t stone_id, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_INLAY_ID_GEMSTONE
        		&& (int)stone_id != task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_INLAY_ID_GEMSTONE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_gang_well_recover_spirit(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_GANG_WELL_RECOVER_SPIRIT) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_equip_purple_ring(TaskInfo *task_info, uint32_t quality, uint32_t num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_EQUIP_PURPLE_RING
        		&& (int)quality < task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_EQUIP_PURPLE_RING) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_inlay_level_gemsteone(TaskInfo *task_info, uint32_t gemstone_level, uint32_t gemstone_num) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_INLAY_LEVEL_GEMSTONE
        		&& (int)gemstone_level < task_json->execute[i].id) {
        	is_all_pass = false;
        	continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_INLAY_LEVEL_GEMSTONE) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value = gemstone_num;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_upgrade_card(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
    bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            is_all_pass = false;
            continue;
        }

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_UPGRADE_CARD) {
            if (task_cond->__current_value < task_json->execute[i].amount)
            {
                is_all_pass = false;
            }
            continue;
		}

        task_cond->__current_value += 1;
        if (task_cond->__current_value == task_json->execute[i].amount) {
        	task_cond->__current_value = task_json->execute[i].amount;
        }
        else if (task_cond->__current_value > task_json->execute[i].amount) {
        	if(task_info->is_accepted()) {
        		task_cond->__current_value = task_json->execute[i].amount;
        	} else {
        		return -1;//当前条件已经完成
        	}
        }
        else {
        	is_all_pass = false;
        	notify_task_update(*task_info, *task_json);
        }
    }
    if (is_all_pass == true)
        this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_role_equip_4(TaskInfo *task_info, const uint32_t quality, const uint32_t num_act) {
	if(!task_info) {
		return 0;
	}

	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}

	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_ROLE_EQUIP_4
				&& quality < task_json->execute[i].param1) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_ROLE_EQUIP_4) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value = num_act;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_hero_equip_4(TaskInfo *task_info, const uint32_t quality, const uint32_t num_act) {
	if(!task_info) {
		return 0;
	}

	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}

	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HERO_EQUIP_4
				&& quality < task_json->execute[i].param1) {
			is_all_pass = false;
			continue;
		}

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HERO_EQUIP_4) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value = num_act;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

int Tasker::process_listen_dayly_single_arena_all(TaskInfo *task_info) {
	if(!task_info) {
		return 0;
	}

	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	if (NULL == task_json) {
		return ERROR_TASK_NOT_EXIST;
	}

	bool is_all_pass = true;
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		TaskCondition *task_cond = task_info->task_condition(i);
		if (task_cond == 0)
		{
			is_all_pass = false;
			continue;
		}

		int total_num;
		int enter_num;
		player_self()->get_arena_enter_num_info(total_num, enter_num);

		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_HERO_EQUIP_4) {
			if (task_cond->__current_value < task_json->execute[i].amount)
			{
				is_all_pass = false;
			}
			continue;
		}

		task_cond->__current_value = enter_num;
		if (task_cond->__current_value == task_json->execute[i].amount) {
			task_cond->__current_value = task_json->execute[i].amount;
		}
		else if (task_cond->__current_value > task_json->execute[i].amount) {
			if(task_info->is_accepted()) {
				task_cond->__current_value = task_json->execute[i].amount;
			} else {
				return -1;//当前条件已经完成
			}
		}
		else {
			is_all_pass = false;
			notify_task_update(*task_info, *task_json);
		}
	}
	if (is_all_pass == true)
		this->process_finish_imp(task_info);
	return 0;
}

/*int Tasker::process_listen_use_skill(TaskInfo *task_info, uint32_t skill_id) {
	   const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	    if (NULL == task_json) {
	    	return ERROR_TASK_NOT_EXIST;
	    }
	    bool is_all_pass = true;
		for (size_t i = 0; i < task_json->execute.size(); ++i) {
	        TaskCondition *task_cond = task_info->task_condition(i);
	        if (task_cond == 0)
	        {
	            is_all_pass = false;
	            continue;
	        }
			if (task_json->execute[i].type != TaskExecType::TASK_EXEC_USE_SKILL ||
					task_json->execute[i].id_list.find(skill_id) == task_json->execute[i].id_list.end()) {
	            if (task_cond->__current_value < task_json->execute[i].amount)
	            {
	                is_all_pass = false;
	            }
	            continue;
			}

			if(task_json->execute[i].type == TaskExecType::TASK_EXEC_USE_SKILL
					&& task_json->execute[i].id_list.find(skill_id) == task_json->execute[i].id_list.end()) {
				continue;
			}

			task_cond->__current_value  += 1;
	        if (task_cond->__current_value >= task_json->execute[i].amount) {
	        	task_cond->__current_value = task_json->execute[i].amount;
	        } else {
	        	is_all_pass = false;
	        	notify_task_update(*task_info, *task_json);
	        }
	    }
	    if (is_all_pass == true)
	        this->process_finish(task_info);
	return 0;
}

int Tasker::process_listen_append_skill(TaskInfo *task_info) {
	   const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	    if (NULL == task_json) {
	    	return ERROR_TASK_NOT_EXIST;
	    }
	    bool is_all_pass = true;
		for (size_t i = 0; i < task_json->execute.size(); ++i) {
	        TaskCondition *task_cond = task_info->task_condition(i);
	        if (task_cond == 0)
	        {
	            is_all_pass = false;
	            continue;
	        }
			if (task_json->execute[i].type != TaskExecType::TASK_EXEC_APPEND_SKILL) {
	            if (task_cond->__current_value < task_json->execute[i].amount)
	            {
	                is_all_pass = false;
	            }
	            continue;
			}

			task_cond->__current_value  += 1;
	        if (task_cond->__current_value >= task_json->execute[i].amount) {
	        	task_cond->__current_value = task_json->execute[i].amount;
	        } else {
	        	is_all_pass = false;
	        	notify_task_update(*task_info, *task_json);
	        }
	    }
	    if (is_all_pass == true)
	        this->process_finish(task_info);
	return 0;
}*/
