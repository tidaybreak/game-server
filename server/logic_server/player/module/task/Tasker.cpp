/*
 * Tasker.cpp
 *
 *  Created on: Nov 26, 2013
 *      Author: ti
 */

#include "Logic_Module_Head.h"
#include "Config_Cache_Task.h"
#include "Tasker.h"
#include "Task_Struct.h"
#include "pack/Pack_Struct.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "Msg_Task_Struct.h"
#include "Msg_Active_Struct.h"
#include "Err_Code.h"
#include "Logic_Player.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Item.h"
#include "Task_Def.h"
#include "Config_Cache_Reward_Task.h"
#include "Msg_Active_Enum.h"
#include "Msg_Task_Enum.h"
#include "item_user/Item_User_Func.h"
#include "vip/Vip.h"
#include "vip/Vip_Def.h"
#include "achievement/Achievement.h"
#include "task/Day_Week_Task_Config.h"
#include "reward/Reward_Def.h"
#include "gang/Gang_Manager.h"
#include "equiper/Equiper_Def.h"
#include "equiper/Equiper_Struct.h"
#include "Config_Cache_Equip.h"
#include "helper/Helper_Struct.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "covenant/Covenant_Def.h"
#include "Config_Cache_Scene.h"
#include "team/Team_Manager.h"
#include "dragon/Dragon_Vale_Def.h"
#include "dragon/Dragon_Vale_Struct.h"
#include "wing/Wing_Struct.h"
#include "rune_stone/Rune_Stone_Def.h"
#include "helper/Helper_Def.h"
#include "active/Active_Def.h"
#include "active/Active_Manager.h"

Tasker::Tasker() {
	this->tasker_detail_ = 0;
    this->task_accepted_lvl_.clear();
    this->task_accepted_monster_.clear();
    this->task_accepted_collect_.clear();
    this->task_accepted_fb_type_.clear();
    this->task_accepted_package_.clear();
    this->task_listen_item_TaskIdSet_map_.unbind_all();
    this->task_accepted_fb_.clear();
    this->task_accepted_arrive_.clear();
    this->task_accepted_monster_group_.clear();
    this->task_accepted_win_player_.clear();
    this->task_accepted_join_gang_.clear();
   	this->task_accepted_ganger_level_.clear();
    this->task_accepted_player_all_equip_.clear();
    this->task_accepted_body_equip_.clear();
    this->task_accepted_body_equip_level_.clear();
    this->task_accepted_equip_level_.clear();
    this->task_accepted_stone_level_.clear();
    this->task_accepted_buy_goods_.clear();
    this->task_accepted_get_hero_.clear();
    this->task_accepted_fight_.clear();
    this->task_accepted_commit_consume_item_.clear();
    this->task_accepted_deliver_letter_.clear();
    this->task_accepted_help_other_complete_team_scene_.clear();
    this->task_accepted_sell_item_.clear();
//    this->task_accepted_use_skill_.clear();
//    this->task_accepted_append_skill_.clear();
    this->task_accepted_limit_time_.clear();
    this->task_accepted_arena_.clear();
    this->task_accepted_chat_with_friend_.clear();
    this->task_accepted_send_mail_.clear();
    this->task_accepted_team_scene_or_pvp_.clear();
    this->task_accepted_browse_other_player_info_.clear();
    this->task_accepted_npc_talk_.clear();
    this->task_accepted_arena_level_.clear();
    this->task_accepted_equip_crimson_level_.clear();
    this->task_accepted_arena_all_.clear();
    this->task_accepted_create_dragon_vale_building_.clear();
    this->task_accepted_gain_dragon_vale_builiding_production_.clear();
    this->task_accepted_wake_up_hero_to_quality_level_.clear();
    this->task_accepted_keep_hero_to_level_.clear();
    this->task_accepted_compose_equip_.clear();
    this->task_accepted_hero_dream_scene_.clear();
    this->task_accepted_ganger_skill_level_.clear();
    this->task_accepted_war_.clear();
    this->task_accepted_war_all_.clear();
    this->task_accepted_kill_enemy_in_war_.clear();
    this->task_accepted_single_arena_all_.clear();
    this->task_accepted_single_arena_win_.clear();
    this->task_accepted_team_arena_all_.clear();
    this->task_accepted_team_arena_win_.clear();
    this->task_accepted_ganger_bless_buf_.clear();
    this->task_accepted_rob_other_dragon_vale_all_.clear();
    this->task_accepted_dragon_vale_step_foot_all_.clear();
    this->task_accepted_activeness_of_today_.clear();
    this->task_accepted_dragon_vale_building_upgrade_.clear();
    this->task_accepted_limit_time_kill_boss_.clear();
    this->task_accepted_dragon_keep_all_.clear();
    this->task_accepted_dragon_keep_success_.clear();
    this->task_accepted_donate_to_gang_.clear();
    this->task_accepted_present_buf_to_other_.clear();
    this->task_accepted_finish_reward_task_num_.clear();
    this->task_accepted_keep_dragon_to_level_.clear();
    this->task_accepted_knight_trial_.clear();
    this->task_accepted_suit_.clear();
    this->task_accepted_role_pull_on_equip_.clear();
    this->task_accepted_hero_pull_on_equip_.clear();
    this->task_accepted_activate_wing_.clear();
    this->task_accepted_get_quality_rune_.clear();
    this->task_accepted_pass_kngiht_trials_.clear();
    this->task_accepted_have_hero_.clear();
    this->task_accepted_have_hero_quality_.clear();
    this->task_accepted_first_pass_scene_.clear();
    this->task_accepted_dragon_born_.clear();
    this->task_accepted_horse_star_level_.clear();
    this->task_accepted_have_item_but_no_erase_.clear();
    this->task_accepted_have_equip_but_no_erase_.clear();
    this->task_accepted_same_gang_challenge_all_.clear();
    this->task_accepted_main_scene_npc_battle_.clear();
    this->task_accepted_equip_card_all_.clear();
    this->task_accepted_equip_card_by_position_.clear();
    this->task_accepted_card_upgrade_to_level_all_.clear();
    this->task_accepted_card_upgrade_to_level_by_position_.clear();
    this->task_accepted_lottery_draw_.clear();
    this->task_accepted_equip_jewelry_.clear();
    this->task_accepted_invite_friend_step_foot_.clear();
    this->task_accepted_kill_group_num_.clear();
    this->task_accepted_did_reward_task_num_.clear();
    this->task_accepted_alchemy_num_.clear();
    this->task_accepted_did_reward_task_num_today_.clear();
    this->task_accepted_remote_scene_any_pass_.clear();
    this->task_accepted_knight_trial_any_pass_.clear();
    this->task_accepted_compose_gemstone_level_.clear();
    this->task_acceped_inlay_position_gemstone_.clear();
    this->task_accepted_inlay_any_gemstone_.clear();
    this->task_accepted_world_boss_.clear();
    this->task_accepted_world_boss_immediate_revive_.clear();
    this->task_accepted_enchant_any_equip_.clear();
    this->task_accepted_inlay_id_gemstone_.clear();
    this->task_accepted_gang_well_recover_spirit_.clear();
    this->task_accepted_equip_purple_ring_.clear();
    this->task_accepted_inlay_level_gemstone_.clear();
    this->task_accepted_upgrade_card_.clear();
}

Tasker::~Tasker() {
}

int Tasker::create_init(Tasker_Detail &detail) {
	return 0;
}

void Tasker::load_detail(Tasker_Detail &detail) {
	tasker_detail_ = &detail;
}

void Tasker::save_detail(Tasker_Detail &detail) {
	if(tasker_detail().is_change) {
		detail = tasker_detail();
		tasker_detail_->is_change = false;
	}
	return ;
}

void Tasker::module_init(void) {
	//must be first
	correct_task_cfg_execute_type();
	correct_ring_task_error_cfg();

	//将任务放到不同任务集中
	for (TaskMap::iterator it = tasker_detail_->task_map_.begin();
			it != tasker_detail_->task_map_.end(); ++it) {

		TaskInfo &task_info = it->second;
		model_init_one_task(task_info);
	}

	//将任务放到不同任务集中
	for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
			it != tasker_detail_->reward_task_map.end(); ++it) {

		TaskInfo &task_info = it->second;
		model_init_one_task(task_info);
	}

    this->task_construct(false);

	// 防止场景配制删除
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
	if (!scene_config) {
		MSG_USER("can not find scene:%d ", player_self()->location().scene_id);

		//找不到场景配置，按不在限时击杀场景内处理
		tasker_detail_->target_player_id = 0;
		tasker_detail_->task_id = 0;
		return;
	}

	//登录后不在限时击杀场景内，重置target_player_id，按不在限时击杀场景内处理
	if(Task_Scene_Type != scene_config->type) {
		tasker_detail_->target_player_id = 0;
		tasker_detail_->task_id = 0;
		return;
	}

	if(player_self()->role_id() == tasker_detail_->target_player_id) {
		TaskInfo *task_info = find_task(tasker_detail_->task_id);
		const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(tasker_detail_->task_id);
		if(!task_cfg || !task_info || !task_info->is_accepted() || !task_cfg->is_limit_time || !task_cfg->limit_time
			|| !task_info->is_logic_limit_time_kill_boss()) {
			tasker_detail_->target_player_id = 0;
			tasker_detail_->task_id = 0;
			player_self()->set_create_scene_args(0, Time_Value::zero, 0, 0, 0, 0, true);
			this->save_tick();
			return;
		}

		long total_time = task_info->__accept_tick.sec() + task_cfg->limit_time * Time_Value::ONE_MINUTE_IN_SECS;
		Time_Value now = Time_Value::gettimeofday();
		if(total_time <= now.sec()) {
			tasker_detail_->target_player_id = 0;
			tasker_detail_->task_id = 0;
			player_self()->set_create_scene_args(0, Time_Value::zero, 0, 0, 0, 0, true);
			this->save_tick();
			return;
		}

		player_self()->set_create_scene_args(0, Time_Value::zero, tasker_detail_->target_player_id,
							tasker_detail_->task_id, 0, 0, true);
	}
	else {
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(tasker_detail_->target_player_id);
		if(!player) {
			tasker_detail_->target_player_id = 0;
			tasker_detail_->task_id = 0;
			player_self()->set_create_scene_args(0, Time_Value::zero, 0, 0, 0, 0, true);
			this->save_tick();
			return;
		}

		const Scene_Config *target_scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player->location().scene_id);
		if(!target_scene_config) {
			tasker_detail_->target_player_id = 0;
			tasker_detail_->task_id = 0;
			player_self()->set_create_scene_args(0, Time_Value::zero, 0, 0, 0, 0, true);
			this->save_tick();
			return;
		}

		if(Task_Scene_Type != target_scene_config->type) {
			tasker_detail_->target_player_id = 0;
			tasker_detail_->task_id = 0;
			player_self()->set_create_scene_args(0, Time_Value::zero, 0, 0, 0, 0, true);
			this->save_tick();
			return;
		}

		Team *team = player->get_team();
		if (NULL == team || !team->is_in_team(player_self()->role_id())) {
			player_self()->set_no_team();
			tasker_detail_->target_player_id = 0;
			tasker_detail_->task_id = 0;
			player_self()->set_create_scene_args(0, Time_Value::zero, 0, 0, 0, 0, true);
			this->save_tick();
			return;
		}

		player_self()->set_create_scene_args(0, Time_Value::zero, tasker_detail_->target_player_id,
							0, 0, 0, true);
	}
}

void Tasker::sign_in(void)
{
	task_listen_item_update();
    return ;
}

void Tasker::sign_out(void)
{
    return ;
}

int Tasker::client_ready_now(void) {
    //must be here
    correct_task_status_and_execute_progress();

	if (!this->tasker_detail_->npc_type_map_.empty()||!tasker_detail_->npc_id_.empty()) {
		Location localtion = this->player_self()->location();
		MSG_81110001 msg;
		for (Task_Npc_Type_Map::const_iterator it = tasker_detail_->npc_type_map_.begin();
				it != tasker_detail_->npc_type_map_.end(); ++it) {
			NPC_Appear_Info_Task npc_info;
			if (it->second.x == 0 && it->second.y == 0) {
				npc_info.npc_info.coord = localtion.coord.pixel_coord();
			} else {
				npc_info.npc_info.coord.type = 1; // PIXEL_TYPE
				npc_info.npc_info.coord.x = it->second.x;
				npc_info.npc_info.coord.y = it->second.y;
			}
			npc_info.npc_info.hp_curr = 100;
			npc_info.npc_info.hp_max = 100;
			npc_info.npc_info.npc_type = it->second.npc_type;
			npc_info.npc_info.role_id = it->second.npc_id;
			npc_info.npc_info.role_name = it->second.npc_name;
			npc_info.npc_info.speed = 180;
			npc_info.funcName = it->second.funcName;
			npc_info.direction = it->second.direction;
			npc_info.sceneId = it->second.sceneId;
			npc_info.open = it->second.open;
			msg.npc_info.push_back(npc_info);
		}//end for

		msg.npc_ids.assign(tasker_detail_->npc_id_.begin(), tasker_detail_->npc_id_.end());
		THIS_SEND_TO_CLIENT(msg);
	}

	//通知场景新号第一个任务特殊处理
	sync_scene_special_process_first_task();

	//通知场景服监听收集任务
	this->sync_item_source();

	notify_remain_reward_task_num();

	{
		MSG_81110015 msg;
		for(Scene_Id_Set::const_iterator it = tasker_detail_->scene_id_set.begin(); it !=  tasker_detail_->scene_id_set.end(); ++it) {
			msg.id_list.push_back(*it);
		}
		if(!msg.id_list.empty()) {
			THIS_SEND_TO_CLIENT(msg);
		}
	}

	{
		MSG_81110016 msg;
		for(Svr_Scene_Npc_Set::const_iterator it = tasker_detail_->svr_scene_npc_set.begin();
				it != tasker_detail_->svr_scene_npc_set.end(); ++it) {
			Scene_Npc scene_npc;
			scene_npc.scene_id = it->scene_id;
			scene_npc.npc_id = it->npc_id;
			msg.scene_npc_list.push_back(scene_npc);
		}
		if(!msg.scene_npc_list.empty()) {
			THIS_SEND_TO_CLIENT(msg);
		}
	}

	return 0;
}

int Tasker::move_scene_completed(void) {
	//通知场景服监听收集任务
	this->sync_item_source();
	return 0;
}

void Tasker::reset(void)
{
    TaskInfo *task_info = 0;
    if (this->tasker_detail_) {
		for (TaskMap::iterator iter = this->tasker_detail_->task_map_.begin();
				iter != this->tasker_detail_->task_map_.end(); ++iter)
		{
			task_info = &(iter->second);
			task_info->reset();
		}

		for(RewardTaskMap::iterator it = this->tasker_detail_->reward_task_map.begin();
				it != this->tasker_detail_->reward_task_map.end(); ++it) {
			task_info = &(it->second);
			task_info->reset();
		}

	    this->tasker_detail_->reset();
	    this->tasker_detail_ = 0;
	}

    this->task_accepted_lvl_.clear();
    this->task_accepted_monster_.clear();
    this->task_accepted_monster_group_.clear();
    this->task_accepted_collect_.clear();
    this->task_accepted_fb_type_.clear();
    this->task_accepted_package_.clear();
    this->task_listen_item_TaskIdSet_map_.unbind_all();
    this->scene_npc_ItemIdSet_map_.unbind_all();
    this->scene_task_id_set_.clear();
    this->task_accepted_fb_.clear();
    this->task_accepted_arrive_.clear();
    this->task_accepted_win_player_.clear();
    this->task_accepted_join_gang_.clear();
   	this->task_accepted_ganger_level_.clear();
    this->task_accepted_player_all_equip_.clear();
    this->task_accepted_body_equip_.clear();
    this->task_accepted_body_equip_level_.clear();
    this->task_accepted_equip_level_.clear();
    this->task_accepted_stone_level_.clear();
    this->task_accepted_buy_goods_.clear();
    this->task_accepted_get_hero_.clear();
    this->task_accepted_fight_.clear();
    this->task_accepted_commit_consume_item_.clear();
    this->task_accepted_deliver_letter_.clear();
    this->task_accepted_help_other_complete_team_scene_.clear();
    this->task_accepted_sell_item_.clear();
//    this->task_accepted_use_skill_.clear();
//    this->task_accepted_append_skill_.clear();
    this->task_accepted_limit_time_.clear();
    this->task_accepted_arena_.clear();
    this->task_accepted_chat_with_friend_.clear();
    this->task_accepted_send_mail_.clear();
    this->task_accepted_team_scene_or_pvp_.clear();
    this->task_accepted_browse_other_player_info_.clear();
    this->task_accepted_npc_talk_.clear();
    this->task_accepted_arena_level_.clear();
    this->task_accepted_equip_crimson_level_.clear();
    this->task_accepted_arena_all_.clear();
    this->task_accepted_create_dragon_vale_building_.clear();
    this->task_accepted_gain_dragon_vale_builiding_production_.clear();
    this->task_accepted_wake_up_hero_to_quality_level_.clear();
    this->task_accepted_keep_hero_to_level_.clear();
    this->task_accepted_compose_equip_.clear();
    this->task_accepted_hero_dream_scene_.clear();
    this->task_accepted_ganger_skill_level_.clear();
    this->task_accepted_war_.clear();
    this->task_accepted_war_all_.clear();
    this->task_accepted_kill_enemy_in_war_.clear();
    this->task_accepted_single_arena_all_.clear();
    this->task_accepted_single_arena_win_.clear();
    this->task_accepted_team_arena_all_.clear();
    this->task_accepted_team_arena_win_.clear();
    this->task_accepted_ganger_bless_buf_.clear();
    this->task_accepted_rob_other_dragon_vale_all_.clear();
    this->task_accepted_dragon_vale_step_foot_all_.clear();
    this->task_accepted_activeness_of_today_.clear();
    this->task_accepted_dragon_vale_building_upgrade_.clear();
    this->task_accepted_limit_time_kill_boss_.clear();
    this->task_accepted_dragon_keep_all_.clear();
    this->task_accepted_dragon_keep_success_.clear();
    this->task_accepted_donate_to_gang_.clear();
    this->task_accepted_present_buf_to_other_.clear();
    this->task_accepted_finish_reward_task_num_.clear();
    this->task_accepted_keep_dragon_to_level_.clear();
    this->task_accepted_knight_trial_.clear();
    this->task_accepted_suit_.clear();
    this->task_accepted_role_pull_on_equip_.clear();
    this->task_accepted_hero_pull_on_equip_.clear();
    this->task_accepted_activate_wing_.clear();
    this->task_accepted_get_quality_rune_.clear();
    this->task_accepted_pass_kngiht_trials_.clear();
    this->task_accepted_have_hero_.clear();
    this->task_accepted_have_hero_quality_.clear();
    this->task_accepted_first_pass_scene_.clear();
    this->task_accepted_dragon_born_.clear();
    this->task_accepted_horse_star_level_.clear();
    this->task_accepted_have_item_but_no_erase_.clear();
    this->task_accepted_have_equip_but_no_erase_.clear();
    this->task_accepted_same_gang_challenge_all_.clear();
    this->task_accepted_main_scene_npc_battle_.clear();
    this->task_accepted_equip_card_all_.clear();
    this->task_accepted_equip_card_by_position_.clear();
    this->task_accepted_card_upgrade_to_level_all_.clear();
    this->task_accepted_card_upgrade_to_level_by_position_.clear();
    this->task_accepted_lottery_draw_.clear();
    this->task_accepted_equip_jewelry_.clear();
    this->task_accepted_invite_friend_step_foot_.clear();
    this->task_accepted_kill_group_num_.clear();
    this->task_accepted_did_reward_task_num_.clear();
    this->task_accepted_alchemy_num_.clear();
    this->task_accepted_did_reward_task_num_today_.clear();
    this->task_accepted_remote_scene_any_pass_.clear();
    this->task_accepted_knight_trial_any_pass_.clear();
    this->task_accepted_compose_gemstone_level_.clear();
    this->task_acceped_inlay_position_gemstone_.clear();
    this->task_accepted_inlay_any_gemstone_.clear();
    this->task_accepted_world_boss_.clear();
    this->task_accepted_world_boss_immediate_revive_.clear();
    this->task_accepted_enchant_any_equip_.clear();
    this->task_accepted_inlay_id_gemstone_.clear();
    this->task_accepted_gang_well_recover_spirit_.clear();
    this->task_accepted_equip_purple_ring_.clear();
    this->task_accepted_inlay_level_gemstone_.clear();
    this->task_accepted_upgrade_card_.clear();
}

void Tasker::save_tick(void) {
	if (tasker_detail_) {
		tasker_detail_->save_tick();
	}
}

int Tasker::sync_task_module_info(Block_Buffer &buf) {
	return 0;
}

int Tasker::task_construct(bool is_level_up /*= false*/)
{
	tasker_detail_->quality_RewardTaskIdSet_map.clear();
	tasker_detail_->ring_type_RingTaskIdSet_map.clear();

	TaskInfo *taskinfo = 0;
	const Task_Config &task_config = CONFIG_CACHE_TASK->get_task_config();
	for (Task_Mission_Config::const_iterator it = task_config.missions.begin();
			it != task_config.missions.end(); ++it) {
		int task_id = it->first;
		//悬赏任务特殊处理
		if(TaskInfo::TASK_GTYPE_REWARD_TASK == it->second.task_type) {
			bool is_exist = false;
			for(Quality_RewardTaskIdSet_Map::iterator quality_it = tasker_detail_->quality_RewardTaskIdSet_map.begin();
					quality_it != tasker_detail_->quality_RewardTaskIdSet_map.end(); ++quality_it) {
				if(quality_it->second.find(task_id) != quality_it->second.end()) {
					is_exist = true;
					break;
				}
			}

			if(!is_exist) {
				reward_task_construct(it->second);
			}
			continue;
		}

		//环任务特殊处理
		if(TaskInfo::TASK_GTYPE_RING_TASK == it->second.task_type) {
			bool is_exist = false;
			for(Ring_Type_RingTaskIdSet_Map::const_iterator ring_type_it = tasker_detail_->ring_type_RingTaskIdSet_map.begin();
					ring_type_it != tasker_detail_->ring_type_RingTaskIdSet_map.end();
					++ring_type_it) {
				if(ring_type_it->second.find(task_id) != ring_type_it->second.end()) {
					is_exist = true;
					break;
				}
			}

			if(!is_exist) {
				ring_task_construct(it->second);
			}
			continue;
		}

		//开启环任务类型特殊处理
		if(TaskInfo::TASK_GTYPE_OPEN_RING_TASK == it->second.task_type) {
			const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
			if(player_self()->level() < (int)ring_task_all_cfg.ring_task_base_cfg.open_level
					|| tasker_detail_->ring_num > 0
					|| is_exist_open_ring_task()) {
				continue;
			}
		}

		if(TaskInfo::TASK_GTYPE_CLOSE_RING_TASK == it->second.task_type) {
			continue;
		}

		if(it->second.job && it->second.job != player_self()->career()) {
			continue;
		}

		//日常、周常任务特殊处理
/*		if(is_level_up &&
				(TaskInfo::TASK_GTYPE_DAY == it->second.task_type || TaskInfo::TASK_GTYPE_WEEK == it->second.task_type)) {
			continue;
		}*/

		if(it->second.vip_accept_level > 0 && player_self()->vip() != it->second.vip_accept_level) {
			continue;
		}

		//主线任务只能出现一个
		if(TaskInfo::TASK_GTYPE_MAINLINE == it->second.task_type
				&& is_has_accepted_or_finished_main_line_task()) {
			continue;
		}

		if(TaskInfo::TASK_GTYPE_MAINLINE == it->second.task_type
				&& is_has_visible_main_line_task()
		        && get_visiable_main_line_task_id() != it->second.task_id) {
			continue;
		}

		//最后一个主线完成，不再继续做新插入的主线任务,否则，会导致任务流程混乱
		if(TaskInfo::TASK_GTYPE_MAINLINE == it->second.task_type && is_last_main_line_task_submit()) {
			continue;
		}

		taskinfo = this->find_task(task_id);

		//检查前置任务做完，由于等级不够不可接的任务，现在是否可接
		if(0 != taskinfo && taskinfo->is_visible() && it->second.precondition != 0) {
			const Task_Mission_Detail_Config *pre_task_cfg = CONFIG_CACHE_TASK->find_task(it->second.precondition);
			if(this->tasker_detail_->task_submited_once_.find(it->second.precondition) != this->tasker_detail_->task_submited_once_.end()
						  || (pre_task_cfg && pre_task_cfg->task_version > it->second.task_version)) {
				if(it->second.accept.level != 0 && player_self()->level() >= it->second.accept.level) {
					//先通知客户端删除不可接任务，再处理接受
					notify_task_del(*taskinfo);

					set_task_status(*taskinfo, TaskInfo::TASK_STATUS_ACCEPTABLE);
					this->save_tick();
					task_auto_accept(taskinfo->__task_id);
					continue;
				}
			}
		}

		//任务已接，不需要同步配置
		if(0 != taskinfo) {
			continue;
		}

		//任务已提交，不需要同步配置
		if (this->tasker_detail_->task_submited_once_.find(it->first) != this->tasker_detail_->task_submited_once_.end()) {
			continue;
		}

		//有前置任务，并且，前置任务未提交，不需要同步配置
		if (it->second.precondition > 0
				&& this->tasker_detail_->task_submited_once_.find( it->second.precondition) == this->tasker_detail_->task_submited_once_.end()) {
			continue;
		}

		const Task_Mission_Detail_Config &task_config = it->second;
		if(TaskInfo::TASK_GTYPE_MAINLINE != it->second.task_type
				&& task_config.accept.level != 0 && player_self()->level() < task_config.accept.level) {
			continue;
		}

		if(task_config.accept.level_max != 0 && player_self()->level() > task_config.accept.level_max) {
			continue;
		}

		if(task_config.vip_accept_level > 0 && player_self()->vip() != task_config.vip_accept_level) {
			continue;
		}

    	TaskInfo task_info;
    	this->init_task(it->first, task_config, task_info);
    	if (task_info.__task_id != 0)
    	{
    		task_auto_accept(task_info.__task_id);
    	}
	}//end for

	if(tasker_detail_->ring_type_RingTaskIdSet_map.empty()) {
		TaskIdSet temp_set;
		for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
			if(it->second.__game_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK
					|| it->second.__game_type == TaskInfo::TASK_GTYPE_RING_TASK) {
				temp_set.insert(it->second.__task_id);
			}
		}

		for(TaskIdSet::const_iterator  it =  temp_set.begin(); it != temp_set.end(); ++it) {
			TaskInfo *task_info = find_task(*it);
			if(task_info) {
				notify_task_del(*task_info);
			}
			tasker_detail_->task_map_.erase(*it);
		}

		temp_set.clear();
	}

    return 0;
}

int Tasker::init_task(uint32_t task_id, const Task_Mission_Detail_Config &task_json, TaskInfo &taskinfo)
{
//    if (task_json.task_type == TaskInfo::TASK_GTYPE_MAINLINE)
//    {
    	return this->init_main_task(task_id, task_json, taskinfo);
//    }

    return 0;
}

int Tasker::insert_task(uint32_t task_id, const Task_Mission_Detail_Config &json)
{
	if (this->tasker_detail_->task_submited_once_.find(task_id) != this->tasker_detail_->task_submited_once_.end())
		return -1;

	if(json.job && json.job != player_self()->career()) {
		return -1;
	}

	TaskInfo task_info;
	this->init_task(task_id, json, task_info);
	if (task_info.__task_id != 0)
	{
		task_auto_accept(task_info.__task_id);
		return 0;
	}
	return -1;
}

int Tasker::init_main_task(uint32_t task_id, const Task_Mission_Detail_Config &task_json, TaskInfo &task_info)
{
    int status = TaskInfo::TASK_STATUS_VISIBLE;
    int lvl = level();
    if (task_json.accept.level <= lvl)
    {
        status = TaskInfo::TASK_STATUS_ACCEPTABLE;
    }

    task_info.__task_id = task_id;
    task_info.__game_type = task_json.task_type;
	task_info.__prev_task = task_json.precondition;
	task_info.__post_task = task_json.postcondition;
	task_info.__post_task2 = task_json.postcondition2;
	task_info.__post_task3 = task_json.postcondition3;
	task_info.__recruid_hero = task_json.recruid_hero;
    set_task_status(task_info, status);

    this->init_task_condition(task_info, task_json);

    if (this->tasker_detail_->task_map_.insert(std::make_pair(task_id, task_info)).second == false)
    {
        return -1;
    }

    this->save_tick();

    return 0;
}

int Tasker::init_task_condition(TaskInfo &task_info, const Task_Mission_Detail_Config &task_json)
{
	task_info.__condition_list.clear();
    TaskCondition task_cond;
    const Task_Mission_Exec_Cfg &cond_json = task_json.execute;
    for (Task_Mission_Exec_Cfg::const_iterator it = cond_json.begin(); it != cond_json.end(); ++it)
    {
        task_cond.reset();
        switch(it->type) {
        case TaskExecType::TASK_EXEC_KILL_MONSTER:
            task_cond.__type = TaskCondition::TASK_CT_KILL_MONSTER;
            task_info.set_logic_type(TaskCondition::TASK_CT_KILL_MONSTER);
        	break;
        case TaskExecType::TASK_EXEC_FB:
            task_cond.__type = TaskCondition::TASK_CT_FB;
            task_info.set_logic_type(TaskCondition::TASK_CT_FB);
        	break;
        case TaskExecType::TASK_EXEC_NPC_TALK:
            task_cond.__type = TaskCondition::TASK_CT_NPC_TALK;
            task_info.set_logic_type(TaskCondition::TASK_CT_NPC_TALK);
        	break;
        case TaskExecType::TASK_EXEC_ARRIVE:
            task_cond.__type = TaskCondition::TASK_CT_ARRIVE;
            task_info.set_logic_type(TaskCondition::TASK_CT_ARRIVE);
        	break;
        case TaskExecType::TASK_EXEC_LVL:
            task_cond.__type = TaskCondition::TASK_CT_LVL;
            task_info.set_logic_type(TaskCondition::TASK_CT_LVL);
        	break;
        case TaskExecType::TASK_EXEC_COLLECT:
            task_cond.__type = TaskCondition::TASK_CT_COLLECT;
            task_info.set_logic_type(TaskCondition::TASK_CT_COLLECT);
        	break;
        case TaskExecType::TASK_EXEC_KILL_MONSTER_GROUP:
            task_cond.__type = TaskCondition::TASK_CT_KILL_MONSTER_GROUP;
            task_info.set_logic_type(TaskCondition::TASK_CT_KILL_MONSTER_GROUP);
        	break;
        case TaskExecType::TASK_EXEC_PACKAGE:
        	task_cond.__type = TaskCondition::TASK_CT_PACKAGE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_PACKAGE);
        	break;
        case TaskExecType::TASK_EXEC_FB_TYPE:
        	task_cond.__type = TaskCondition::TASK_CT_FB_TYPE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_FB_TYPE);
        	break;
        case TaskExecType::TASK_EXEC_WIN_PLAYER:
        	task_cond.__type = TaskCondition::TASK_CT_WIN_PLAYER;
        	task_info.set_logic_type(TaskCondition::TASK_CT_WIN_PLAYER);
        	break;
        case TaskExecType::TASK_EXEC_JOIN_GANG:
        	task_cond.__type = TaskCondition::TASK_CT_JOIN_GANG;
        	task_info.set_logic_type(TaskCondition::TASK_CT_JOIN_GANG);
        	break;
        case TaskExecType::TASK_EXEC_GANGER_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_GANGER_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_GANGER_LEVEL);
        	break;
        case TaskExecType::TASK_EXEC_PLAYER_ALL_EQUIP:
        	task_cond.__type = TaskCondition::TASK_CT_PLAYER_ALL_EQUIP;
        	task_info.set_logic_type(TaskCondition::TASK_CT_PLAYER_ALL_EQUIP);
        	break;
        case TaskExecType::TASK_EXEC_BODY_EQUIP:
        	task_cond.__type = TaskCondition::TASK_CT_BODY_EQUIP;
        	task_info.set_logic_type(TaskCondition::TASK_CT_BODY_EQUIP);
        	break;
        case TaskExecType::TASK_EXEC_BODY_EQUIP_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_BODY_EQUIP_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_BODY_EQUIP_LEVEL);
        	break;
        case TaskExecType::TASK_EXEC_EQUIP_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_EQUIP_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_EQUIP_LEVEL);
        	break;
        case TaskExecType::TASK_EXEC_STONE_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_STONE_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_STONE_LEVEL);
        	break;
        case TaskExecType::TASK_EXEC_BUY_GOODS:
        	task_cond.__type = TaskCondition::TASK_CT_BUY_GOODS;
        	task_info.set_logic_type(TaskCondition::TASK_CT_BUY_GOODS);
        	break;
        case TaskExecType::TASK_EXEC_GET_HERO:
        	task_cond.__type = TaskCondition::TASK_CT_GET_HERO;
        	task_info.set_logic_type(TaskCondition::TASK_CT_GET_HERO);
        	break;
        case TaskExecType::TASK_EXEC_FIGHT:
        	task_cond.__type = TaskCondition::TASK_CT_FIGHT;
        	task_info.set_logic_type(TaskCondition::TASK_CT_FIGHT);
        	break;
        case TaskExecType::TASK_EXEC_COMMIT_CONSUME_ITEM:
        	task_cond.__type = TaskCondition::TASK_CT_COMMIT_CONSUME_ITEM;
        	task_info.set_logic_type(TaskCondition::TASK_CT_COMMIT_CONSUME_ITEM);
        	break;
        case TaskExecType::TASK_EXEC_DELIVER_LETTER:
        	task_cond.__type = TaskCondition::TASK_CT_DELIVER_LETTER;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DELIVER_LETTER);
        	break;
        case TaskExecType::TASK_EXEC_HELP_OTHER_COMPLETE_TEAM_SCENE:
        	task_cond.__type = TaskCondition::TASK_CT_HELP_OTHER_COMPLETE_TEAM_SCENE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HELP_OTHER_COMPLETE_TEAM_SCENE);
        	break;
        case TaskExecType::TASK_EXEC_SELL_ITEM:
        	task_cond.__type = TaskCondition::TASK_CT_SELL_ITEM;
        	task_info.set_logic_type(TaskCondition::TASK_CT_SELL_ITEM);
        	break;
/*        case TaskExecType::TASK_EXEC_USE_SKILL:
        	task_cond.__type = TaskCondition::TASK_CT_USE_SKILL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_USE_SKILL);
        	break;
        case TaskExecType::TASK_EXEC_APPEND_SKILL:
        	task_cond.__type = TaskCondition::TASK_CT_APPEND_SKILL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_APPEND_SKILL);
        	break;*/

        case TaskExecType::TASK_EXEC_ARENA:
        	task_cond.__type = TaskCondition::TASK_CT_ARENA;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ARENA);
        	break;

        case TaskExecType::TASK_EXEC_CHAT_WITH_FRIEND:
        	task_cond.__type = TaskCondition::TASK_CT_CHAT_WITH_FRIEND;
        	task_info.set_logic_type(TaskCondition::TASK_CT_CHAT_WITH_FRIEND);
        	break;

        case TaskExecType::TASK_EXEC_SEND_MAIL:
        	task_cond.__type = TaskCondition::TASK_CT_SEND_MAIL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_SEND_MAIL);
        	break;

        case TaskExecType::TASK_EXEC_TEAM_SCENE_OR_PVP:
        	task_cond.__type = TaskCondition::TASK_CT_TEAM_SCENE_OR_PVP;
        	task_info.set_logic_type(TaskCondition::TASK_CT_TEAM_SCENE_OR_PVP);
        	break;

        case TaskExecType::TASK_EXEC_BROWSE_OTHER_PLAYER_INFO:
        	task_cond.__type = TaskCondition::TASK_CT_BROWSE_OTHER_PLAYER_INFO;
        	task_info.set_logic_type(TaskCondition::TASK_CT_BROWSE_OTHER_PLAYER_INFO);
        	break;

        case TaskExecType::TASK_EXEC_ARENA_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_ARENA_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ARENA_LEVEL);
        	break;

        case TaskExecType::TASK_EXEC_EQUIP_CRIMSON_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_EQUIP_CRIMSON_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_EQUIP_CRIMSON_LEVEL);
        	break;

        case TaskExecType::TASK_EXEC_ARENA_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_ARENA_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ARENA_ALL);
        	break;

        case TaskExecType::TASK_EXEC_CREATE_DRAGON_VALE_BUILDING:
        	task_cond.__type = TaskCondition::TASK_CT_CREATE_DRAGON_VALE_BUILDING;
        	task_info.set_logic_type(TaskCondition::TASK_CT_CREATE_DRAGON_VALE_BUILDING);
        	break;

        case TaskExecType::TASK_EXEC_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION:
        	task_cond.__type = TaskCondition::TASK_CT_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION;
        	task_info.set_logic_type(TaskCondition::TASK_CT_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION);
        	break;

        case TaskExecType::TASK_EXEC_WAKE_UP_HERO_TO_QUALITY_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_WAKE_UP_HERO_TO_QUALITY_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_WAKE_UP_HERO_TO_QUALITY_LEVEL);
        	break;

        case TaskExecType::TASK_EXEC_KEEP_HERO_TO_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_KEEP_HERO_TO_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_KEEP_HERO_TO_LEVEL);
        	break;

        case TaskExecType::TASK_EXEC_COMPOSE_EQUIP:
        	task_cond.__type = TaskCondition::TASK_CT_COMPOSE_EQUIP;
        	task_info.set_logic_type(TaskCondition::TASK_CT_COMPOSE_EQUIP);
        	break;

        case TaskExecType::TASK_EXEC_HERO_DREAM_SCENE:
        	task_cond.__type = TaskCondition::TASK_CT_HERO_DREAM_SCENE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HERO_DREAM_SCENE);
        	break;

        case TaskExecType::TASK_EXEC_GANGER_SKILL_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_GANGER_SKILL_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_GANGER_SKILL_LEVEL);
        	break;

        case TaskExecType::TASK_EXEC_WAR:
        	task_cond.__type = TaskCondition::TASK_CT_WAR;
        	task_info.set_logic_type(TaskCondition::TASK_CT_WAR);
        	break;

        case TaskExecType::TASK_EXEC_WAR_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_WAR_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_WAR_ALL);
        	break;

        case TaskExecType::TASK_EXEC_KILL_ENEMY_IN_WAR:
        	task_cond.__type = TaskCondition::TASK_CT_KILL_ENEMY_IN_WAR;
        	task_info.set_logic_type(TaskCondition::TASK_CT_KILL_ENEMY_IN_WAR);
        	break;

        case TaskExecType::TASK_EXEC_SINGLE_ARENA_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_SINGLE_ARENA_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_SINGLE_ARENA_ALL);
        	break;

        case TaskExecType::TASK_EXEC_SINGLE_ARENA_WIN:
        	task_cond.__type = TaskCondition::TASK_CT_SINGLE_ARENA_WIN;
        	task_info.set_logic_type(TaskCondition::TASK_CT_SINGLE_ARENA_WIN);
        	break;

        case TaskExecType::TASK_EXEC_TEAM_ARENA_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_TEAM_ARENA_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_TEAM_ARENA_ALL);
        	break;

        case TaskExecType::TASK_EXEC_TEAM_ARENA_WIN:
        	task_cond.__type = TaskCondition::TASK_CT_TEAM_ARENA_WIN;
        	task_info.set_logic_type(TaskCondition::TASK_CT_TEAM_ARENA_WIN);
        	break;

        case TaskExecType::TASK_EXEC_GANGER_BLESS_BUF:
        	task_cond.__type = TaskCondition::TASK_CT_GANGER_BLESS_BUF;
        	task_info.set_logic_type(TaskCondition::TASK_CT_GANGER_BLESS_BUF);
        	break;

        case TaskExecType::TASK_EXEC_ROB_OTHER_DRAGON_VALE_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_ROB_OTHER_DRAGON_VALE_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ROB_OTHER_DRAGON_VALE_ALL);
        	break;

        case TaskExecType::TASK_EXEC_DRAGON_VALE_STEP_FOOT_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_DRAGON_VALE_STEP_FOOT_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DRAGON_VALE_STEP_FOOT_ALL);
        	break;

        case TaskExecType::TASK_EXEC_ACTIVENESS_OF_TODAY:
        	task_cond.__type =  TaskCondition::TASK_CT_ACTIVENESS_OF_TODAY;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ACTIVENESS_OF_TODAY);
        	break;

        case TaskExecType::TASK_EXEC_DRAGON_VALE_BUILDING_UPGRADE:
        	task_cond.__type = TaskCondition::TASK_CT_DRAGON_VALE_BUILDING_UPGRADE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DRAGON_VALE_BUILDING_UPGRADE);
        	break;

        case TaskExecType::TASK_EXEC_LIMIT_TIME_KILL_BOSS:
        	task_cond.__type = TaskCondition::TASK_CT_LIMIT_TIME_KILL_BOSS;
        	task_info.set_logic_type(TaskCondition::TASK_CT_LIMIT_TIME_KILL_BOSS);
        	break;

        case TaskExecType::TASK_EXEC_DRAGON_KEEP_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_DRAGON_KEEP_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DRAGON_KEEP_ALL);
        	break;

        case TaskExecType::TASK_EXEC_DRAGON_KEEP_SUCCESS:
        	task_cond.__type = TaskCondition::TASK_CT_DRAGON_KEEP_SUCCESS;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DRAGON_KEEP_SUCCESS);
        	break;

        case TaskExecType::TASK_EXEC_DONATE_TO_GANG:
        	task_cond.__type = TaskCondition::TASK_CT_DONATE_TO_GANG;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DONATE_TO_GANG);
        	break;

        case TaskExecType::TASK_EXEC_PRESENT_BUF_TO_OTHER:
        	task_cond.__type = TaskCondition::TASK_CT_PRESENT_BUF_TO_OTHER;
        	task_info.set_logic_type(TaskCondition::TASK_CT_PRESENT_BUF_TO_OTHER);
        	break;

        case TaskExecType::TASK_EXEC_FINISH_REWARD_TASK_NUM:
        	task_cond.__type = TaskCondition::TASK_CT_FINISH_REWARD_TASK_NUM;
        	task_info.set_logic_type(TaskCondition::TASK_CT_FINISH_REWARD_TASK_NUM);
        	break;

        case TaskExecType::TASK_EXEC_KEEP_DRAGON_TO_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_KEEP_DRAGON_TO_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_KEEP_DRAGON_TO_LEVEL);
        	break;

        case TaskExecType::TASK_EXEC_KNIGHT_TRIAL:
        	task_cond.__type = TaskCondition::TASK_CT_KNIGHT_TRIAL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_KNIGHT_TRIAL);
        	break;

        case TaskExecType::TASK_EXEC_SUIT:
        	task_cond.__type = TaskCondition::TASK_CT_SUIT;
        	task_info.set_logic_type(TaskCondition::TASK_CT_SUIT);
        	break;

        case TaskExecType::TASK_EXEC_ROLE_PULL_ON_EQUIP:
        	task_cond.__type = TaskCondition::TASK_CT_ROLE_PULL_ON_EQUIP;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ROLE_PULL_ON_EQUIP);
        	break;

        case TaskExecType::TASK_EXEC_HERO_PULL_ON_EQUIP:
        	task_cond.__type = TaskCondition::TASK_CT_HERO_PULL_ON_EQUIP;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HERO_PULL_ON_EQUIP);
        	break;

        case TaskExecType::TASK_EXEC_ACTIVATE_WING:
        	task_cond.__type = TaskCondition::TASK_CT_ACTIVATE_WING;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ACTIVATE_WING);
        	break;

        case TaskExecType::TASK_EXEC_GET_QUALITY_RUNE:
        	task_cond.__type = TaskCondition::TASK_CT_GET_QUALITY_RUNE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_GET_QUALITY_RUNE);
        	break;

        case TaskExecType::TASK_EXEC_PASS_KNGIHT_TRIALS:
        	task_cond.__type = TaskCondition::TASK_CT_PASS_KNGIHT_TRIALS;
        	task_info.set_logic_type(TaskCondition::TASK_CT_PASS_KNGIHT_TRIALS);
        	break;

        case TaskExecType::TASK_EXEC_HAVE_HERO:
        	task_cond.__type = TaskCondition::TASK_CT_HAVE_HERO;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HAVE_HERO);
        	break;

        case TaskExecType::TASK_EXEC_HAVE_HERO_QUALITY:
        	task_cond.__type = TaskCondition::TASK_CT_HAVE_HERO_QUALITY;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HAVE_HERO_QUALITY);
        	break;

        case TaskExecType::TASK_EXEC_FIRST_PASS_SCENE:
        	task_cond.__type = TaskCondition::TASK_CT_FIRST_PASS_SCENE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_FIRST_PASS_SCENE);
        	break;

        case TaskExecType::TASK_EXEC_DRAGON_BORN:
        	task_cond.__type = TaskCondition::TASK_CT_DRAGON_BORN;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DRAGON_BORN);
        	break;

        case TaskExecType::TASK_EXEC_HORSE_STAR_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_HORSE_STAR_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HORSE_STAR_LEVEL);
        	break;

        case TaskExecType::TASK_EXEC_HAVE_ITEM_BUT_NO_ERASE:
        	task_cond.__type = TaskCondition::TASK_CT_HAVE_ITEM_BUT_NO_ERASE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HAVE_ITEM_BUT_NO_ERASE);
        	break;

        case TaskExecType::TASK_EXEC_HAVE_EQUIP_BUT_NO_ERASE:
        	task_cond.__type = TaskCondition::TASK_CT_HAVE_EQUIP_BUT_NO_ERASE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HAVE_EQUIP_BUT_NO_ERASE);
        	break;

        case TaskExecType::TASK_EXEC_SAME_GANG_CHALLENGE_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_SAME_GANG_CHALLENGE_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_SAME_GANG_CHALLENGE_ALL);
        	break;

        case TaskExecType::TASK_EXEC_MAIN_SCENE_NPC_BATTLE:
        	task_cond.__type = TaskCondition::TASK_CT_MAIN_SCENE_NPC_BATTLE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_MAIN_SCENE_NPC_BATTLE);
        	break;

        case TaskExecType::TASK_EXEC_EQUIP_CARD_ALL:
        	task_cond.__type =  TaskCondition::TASK_CT_EQUIP_CARD_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_EQUIP_CARD_ALL);
        	break;

        case TaskExecType::TASK_EXEC_EQUIP_CARD_BY_POSITION:
        	task_cond.__type = TaskCondition::TASK_CT_EQUIP_CARD_BY_POSITION;
        	task_info.set_logic_type(TaskCondition::TASK_CT_EQUIP_CARD_BY_POSITION);
        	break;

        case TaskExecType::TASK_EXEC_CARD_UPGRADE_TO_LEVEL_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_CARD_UPGRADE_TO_LEVEL_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_CARD_UPGRADE_TO_LEVEL_ALL);
        	break;

        case TaskExecType::TASK_EXEC_CARD_UPGRADE_TO_LEVEL_BY_POSITION:
        	task_cond.__type = TaskCondition::TASK_CT_CARD_UPGRADE_TO_LEVEL_BY_POSITION;
        	task_info.set_logic_type(TaskCondition::TASK_CT_CARD_UPGRADE_TO_LEVEL_BY_POSITION);
        	break;

        case TaskExecType::TASK_EXEC_LOTTERY_DRAW:
        	task_cond.__type = TaskCondition::TASK_CT_LOTTERY_DRAW;
        	task_info.set_logic_type(TaskCondition::TASK_CT_LOTTERY_DRAW);
        	break;

        case TaskExecType::TASK_EXEC_EQUIP_JEWELRY:
        	task_cond.__type = TaskCondition::TASK_CT_EQUIP_JEWELRY;
        	task_info.set_logic_type(TaskCondition::TASK_CT_EQUIP_JEWELRY);
        	break;

        case TaskExecType::TASK_EXEC_INVITE_FRIEND_STEP_FOOT:
        	task_cond.__type = TaskCondition::TASK_CT_INVITE_FRIEND_STEP_FOOT;
        	task_info.set_logic_type(TaskCondition::TASK_CT_INVITE_FRIEND_STEP_FOOT);
        	break;

        case TaskExecType::TASK_EXEC_KILL_GROUP_NUM:
        	task_cond.__type = TaskCondition::TASK_CT_KILL_GROUP_NUM;
        	task_info.set_logic_type(TaskCondition::TASK_CT_KILL_GROUP_NUM);
        	break;

        case TaskExecType::TASK_EXEC_DID_REWARD_TASK_NUM:
        	task_cond.__type = TaskCondition::TASK_CT_DID_REWARD_TASK_NUM;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DID_REWARD_TASK_NUM);
        	break;

        case TaskExecType::TASK_EXEC_ALCHEMY_NUM:
        	task_cond.__type = TaskCondition::TASK_CT_ALCHEMY_NUM;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ALCHEMY_NUM);
        	break;

        case TaskExecType::TASK_EXEC_DID_REWARD_TASK_NUM_TODAY:
        	task_cond.__type = TaskCondition::TASK_CT_DID_REWARD_TASK_NUM_TODAY;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DID_REWARD_TASK_NUM_TODAY);
        	break;

        case TaskExecType::TASK_EXEC_REMOTE_SCENE_ANY_PASS:
        	task_cond.__type = TaskCondition::TASK_CT_REMOTE_SCENE_ANY_PASS;
        	task_info.set_logic_type(TaskCondition::TASK_CT_REMOTE_SCENE_ANY_PASS);
        	break;

        case TaskExecType::TASK_EXEC_KNIGHT_TRIAL_ANY_PASS:
        	task_cond.__type = TaskCondition::TASK_CT_KNIGHT_TRIAL_ANY_PASS;
        	task_info.set_logic_type(TaskCondition::TASK_CT_KNIGHT_TRIAL_ANY_PASS);
        	break;

        case TaskExecType::TASK_EXEC_COMPOSE_GEMSTONE_LEVEL:
        	task_cond.__type = TaskCondition::TASK_CT_COMPOSE_GEMSTONE_LEVEL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_COMPOSE_GEMSTONE_LEVEL);
        	break;

        case TaskExecType::TASK_EXEC_INLAY_POSITION_GEMSTONE:
        	task_cond.__type = TaskCondition::TASK_CT_INLAY_POSITION_GEMSTONE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_INLAY_POSITION_GEMSTONE);
        	break;

        case TaskExecType::TASK_EXEC_INLAY_ANY_GEMSTONE:
        	task_cond.__type = TaskCondition::TASK_CT_INLAY_ANY_GEMSTONE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_INLAY_ANY_GEMSTONE);
        	break;

        case TaskExecType::TASK_EXEC_WORLD_BOSS:
        	task_cond.__type = TaskCondition::TASK_CT_WORLD_BOSS;
        	task_info.set_logic_type(TaskCondition::TASK_CT_WORLD_BOSS);
        	break;

        case TaskExecType::TASK_EXEC_WORLD_BOSS_IMMEDIATE_REVIVE:
        	task_cond.__type = TaskCondition::TASK_CT_WORLD_BOSS_IMMEDIATE_REVIVE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_WORLD_BOSS_IMMEDIATE_REVIVE);
        	break;

        case TaskExecType::TASK_EXEC_ENCHANT_ANY_EQUIP:
        	task_cond.__type = TaskCondition::TASK_CT_ENCHANT_ANY_EQUIP;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ENCHANT_ANY_EQUIP);
        	break;

        case TaskExecType::TASK_EXEC_INLAY_ID_GEMSTONE:
        	task_cond.__type = TaskCondition::TASK_CT_INLAY_ID_GEMSTONE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_INLAY_ID_GEMSTONE);
        	break;

        case TaskExecType::TASK_EXEC_GANG_WELL_RECOVER_SPIRIT:
        	task_cond.__type = TaskCondition::TASK_CT_GANG_WELL_RECOVER_SPIRIT;
        	task_info.set_logic_type(TaskCondition::TASK_CT_GANG_WELL_RECOVER_SPIRIT);
        	break;

        case TaskExecType::TASK_EXEC_EQUIP_PURPLE_RING:
        	task_cond.__type = TaskCondition::TASK_CT_EQUIP_PURPLE_RING;
        	task_info.set_logic_type(TaskCondition::TASK_CT_EQUIP_PURPLE_RING);
        	break;

        case TaskExecType::TASK_EXEC_INLAY_LEVEL_GEMSTONE:
        	task_cond.__type = TaskCondition::TASK_CT_INLAY_LEVEL_GEMSTONE;
        	task_info.set_logic_type(TaskCondition::TASK_CT_INLAY_LEVEL_GEMSTONE);
        	break;

        case TaskExecType::TASK_EXEC_UPGRADE_CARD:
        	task_cond.__type = TaskCondition::TASK_CT_UPGRADE_CARD;
        	task_info.set_logic_type(TaskCondition::TASK_CT_UPGRADE_CARD);
        	break;

        case TaskExecType::TASK_EXEC_ROLE_EQUIP_4:
        	task_cond.__type = TaskCondition::TASK_CT_ROLE_EQUIP_4;
        	task_info.set_logic_type(TaskCondition::TASK_CT_ROLE_EQUIP_4);
        	break;

        case TaskExecType::TASK_EXEC_HERO_EQUIP_4:
        	task_cond.__type = TaskCondition::TASK_CT_HERO_EQUIP_4;
        	task_info.set_logic_type(TaskCondition::TASK_CT_HERO_EQUIP_4);
        	break;

        case TaskExecType::TASK_EXEC_DAYLY_SINGLE_ARENA_ALL:
        	task_cond.__type = TaskCondition::TASK_CT_DAYLY_SINGLE_ARENA_ALL;
        	task_info.set_logic_type(TaskCondition::TASK_CT_DAYLY_SINGLE_ARENA_ALL);
        	break;

        default:
        	MSG_USER("error task condition type %d", it->type);
        	break;
        }
        task_info.__condition_list.push_back(task_cond);
    }

    return 0;
}

int Tasker::task_auto_accept(uint32_t task_id) {
	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
	if(!task_cfg) {
		MSG_USER("task_cfg is null, task_id=\n", task_id);
		return -1;
	}

	TaskInfo *task_info = find_task(task_id);
	if(!task_info) {
		MSG_USER("task_info is null, task_id=\n", task_id);
		return -1;
	}

	int ret = 0;
	if (task_cfg->accept.is_empty || (task_cfg->accept.auto_accept == 1 && task_cfg->accept.talk == 0))
	{
		ret = this->process_accept(task_info->__task_id, true);
	} else {
		this->notify_task_add(*task_info, *task_cfg);
	}

	return ret;
}

int Tasker::task_accept(uint32_t task_id)
{
    const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(task_id);
    if (!task_config) {
    	return ERROR_TASK_NOT_EXIST;
    }


    int ret = this->process_accept(task_id, false);

    if (0 == ret) {
//        MSG_50110002 rs_msg;
//		rs_msg.task_id = task_id;
//		rs_msg.type = task_json->task_type;
//		THIS_SEND_TO_CLIENT(rs_msg);
		return 0;
    } else {
    	return ret;
    }

    return ret;
}

int Tasker::task_abandon(uint32_t task_id)
{
    int ret = this->process_abandon(task_id);
    if (0 != ret) {
    	return ret;
    }

//    MSG_50110003 res;
//    res.task_id = task_id;
//	THIS_SEND_TO_CLIENT(res);
	return 0;
}
//
int Tasker::task_submit(uint32_t task_id)
{
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
    if (!task_json) {
    	return ERROR_CONFIG_NOT_EXIST;
    }

#ifndef LOCAL_DEBUG
    int npc_id = task_json->complete.npc_id;
    if (npc_id > 0)
    {
//        return this->is_nearby_npc(CLIENT_TASK_SUBMIT, msg, npc_id);
    }
#endif

    int ret = this->process_submit(task_id);
    if (0 != ret) {
    	return ret;
    }

//    MSG_50110004 res;
//    res.task_id = task_id;
//	THIS_SEND_TO_CLIENT(res);
	return 0;
}

int Tasker::task_get_list(void)     // 获取任务列表
{
    Task_Info msg_task_info;
    MSG_50110000 msg;

    //未领取奖励的任务集
    TaskIdSet award_set;

    for (TaskMap::iterator it = this->tasker_detail_->task_map_.begin();
            it != this->tasker_detail_->task_map_.end(); ++it)
    {
    	msg_task_info.reset();

    	const TaskInfo &svr_task_info = it->second;

        if (svr_task_info.is_finish() == false &&
        		svr_task_info.is_accepted() == false &&
        		svr_task_info.is_acceptable() == false &&
        		svr_task_info.is_visible() == false &&
        		svr_task_info.is_fail() == false)
            continue;

        process_task_get_list(msg, msg_task_info, svr_task_info, award_set);
    }

    for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
    		it != tasker_detail_->reward_task_map.end(); ++it) {

    	msg_task_info.reset();

		if (it->second.is_finish() == false &&
				it->second.is_accepted() == false &&
				it->second.is_fail() == false) {
			continue;
		}

    	const TaskInfo &svr_task_info = it->second;
    	process_task_get_list(msg, msg_task_info, svr_task_info, award_set);
    }

    //已提交的任务id
    TaskIdSet temp_set;
    temp_set.clear();

    //英雄任务相关
    for (TaskIdSet::iterator it = this->tasker_detail_->task_submited_once_.begin();
    		it != this->tasker_detail_->task_submited_once_.end(); ++it) {
    	const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(*it);
    	if (task_config && task_config->recruid_hero) {
    		if(task_config->task_type == TaskInfo::TASK_GTYPE_DAY && !task_config->is_show_in_task_list) {
    			continue;
    		}
    		msg_task_info.reset();
            msg_task_info.type = task_config->task_type;
            msg_task_info.id = *it;
            msg_task_info.status = TaskInfo::TASK_STATUS_SUBMIT;

            msg.tasks.push_back(msg_task_info);

            //临时判断用
            temp_set.insert(msg_task_info.id);
    	}
    }

    //任务触发新英雄图标显示/关闭(1:显示图标 2:1个星  3：2个星 4:3个星  5:4个星 6:5个星，并自动消失)
    for(Task_Icon_Hero_Id_Map::const_iterator it = this->tasker_detail_->task_icon_hero_id_Map.begin();
    		it != this->tasker_detail_->task_icon_hero_id_Map.end();
    		++it) {
    	if(temp_set.find(it->first) != temp_set.end()) {
    		continue;
    	}

    	const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(it->first);
    	if(!task_config) {
    		continue;
    	}

		if(task_config->task_type == TaskInfo::TASK_GTYPE_DAY && !task_config->is_show_in_task_list) {
			continue;
		}

    	msg_task_info.reset();
    	msg_task_info.id = it->first;
    	msg_task_info.type = task_config->task_type;
    	msg_task_info.status = TaskInfo::TASK_STATUS_SUBMIT;
        msg.tasks.push_back(msg_task_info);

        //临时判断用
        temp_set.insert(msg_task_info.id);
    }

    //任务开启礼包等级
    for(Task_Gift_Level_Set::const_iterator it = this->tasker_detail_->task_gift_level_set.begin();
    		it != this->tasker_detail_->task_gift_level_set.end();
    		++it) {
    	if(temp_set.find(*it) != temp_set.end()) {
    		continue;
    	}

    	const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(*it);
    	if(!task_config) {
    		continue;
    	}

		if(task_config->task_type == TaskInfo::TASK_GTYPE_DAY && !task_config->is_show_in_task_list) {
			continue;
		}

    	msg_task_info.reset();
    	msg_task_info.id = *it;
    	msg_task_info.type = task_config->task_type;
    	msg_task_info.status = TaskInfo::TASK_STATUS_SUBMIT;
        msg.tasks.push_back(msg_task_info);

        //临时判断用
        temp_set.insert(msg_task_info.id);
    }

    //魔翼相关任务
    for (TaskIdSet::iterator it = this->tasker_detail_->task_submited_once_.begin();
    		it != this->tasker_detail_->task_submited_once_.end(); ++it) {
    	if(temp_set.find(*it) != temp_set.end()) {
    		continue;
    	}

    	const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(*it);
    	if (task_config && task_config->is_magic_wing) {
    		if(task_config->task_type == TaskInfo::TASK_GTYPE_DAY && !task_config->is_show_in_task_list) {
    			continue;
    		}

    		msg_task_info.reset();
            msg_task_info.type = task_config->task_type;
            msg_task_info.id = *it;
            msg_task_info.status = TaskInfo::TASK_STATUS_SUBMIT;

            msg.tasks.push_back(msg_task_info);

            //临时判断用
            temp_set.insert(msg_task_info.id);
    	}
    }

    //天赋任务
    for (TaskIdSet::iterator it = this->tasker_detail_->task_submited_once_.begin();
    		it != this->tasker_detail_->task_submited_once_.end(); ++it) {
    	if(temp_set.find(*it) != temp_set.end()) {
    		continue;
    	}

    	const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(*it);
    	if (task_config && task_config->task_type == TaskInfo::TASK_GTYPE_TALENT && task_config->talent_id) {
    		if(task_config->task_type == TaskInfo::TASK_GTYPE_DAY && !task_config->is_show_in_task_list) {
    			continue;
    		}

    		msg_task_info.reset();
            msg_task_info.type = task_config->task_type;
            msg_task_info.id = *it;
            msg_task_info.status = TaskInfo::TASK_STATUS_SUBMIT;

            msg.tasks.push_back(msg_task_info);

            //临时判断用
            temp_set.insert(msg_task_info.id);
    	}
    }

	THIS_SEND_TO_CLIENT(msg);

	for(TaskIdSet::const_iterator it = award_set.begin(); it != award_set.end(); ++it) {
		MSG_81110006 msg_81110006;
		msg_81110006.task_id = *it;
		msg_81110006.is_window = 1;
		THIS_SEND_TO_CLIENT(msg_81110006);
	}

	temp_set.clear();
	award_set.clear();

	task_listen_item_update();

	return 0;
}

/*int Tasker::task_npc_list(int npc_id, uint8_t source)     // 获取npc列表
{
    const Task_Config::IntSet *task_set = CONFIG_CACHE_TASK->npc_task_list(npc_id);
    MSG_50110001 respond;
    respond.npc_id = npc_id;
    respond.source = source;
    if (task_set) {

        TaskInfo *task_info = 0;
        Task_Status tstatus;
        for (Task_Config::IntSet::iterator iter = task_set->begin();
                iter != task_set->end(); ++iter)
        {
        	task_info = this->find_task(*iter);
            if (0 == task_info)
                continue;

            if (task_info->is_finish() == false &&
                    task_info->is_accepted() == false &&
                    task_info->is_acceptable() == false &&
                    task_info->is_visible() == false)
                continue;

            tstatus.reset();
            tstatus.task_id = task_info->__task_id;
            tstatus.status = task_state(task_info->__task_id);
            tstatus.type = task_info->__game_type;
            respond.tasks.push_back(tstatus);
        }
    }

	THIS_SEND_TO_CLIENT(respond);
	return 0;
}
*/
int Tasker::task_fast_finish(uint32_t task_id, uint8_t type, uint8_t is_bind_diamond_first) // 快速完成任务
{
    int ret = this->process_fast_finish(task_id, type,  is_bind_diamond_first);
    if (0 != ret) {
    	return ret;
    }

//    MSG_50110007 res;
//    res.task_id = task_id;
//	THIS_SEND_TO_CLIENT(res);

	return 0;
}

//int Tasker::task_plot(uint32_t task_id, int8_t action) {
//
//	MSG_50110008 msg;
//	msg.task_id = task_id;
//	msg.action = action;
//
//	THIS_SEND_TO_CLIENT(msg);
//	return 0;
//}

int Tasker::task_after_check_npc(uint32_t npc_id)
{
//    switch (request->recogn())
//    {
//        case CLIENT_TASK_ACCEPT:
//        {
//            Proto11400326 sub_request;
//            sub_request.ParsePartialFromString(request->msg_body());
//            int ret = this->process_accept(sub_request.task_id());
//            CONDITION_PLAYER_NOTIFY_RETURN(ret == 0, RETURN_TASK_ACCEPT, ret);
//            break;
//        }
//        case CLIENT_TASK_ABANDON:
//        {
//            Proto11400327 sub_request;
//            sub_request.ParsePartialFromString(request->msg_body());
//            int ret = this->process_abandon(sub_request.task_id());
//            CONDITION_PLAYER_NOTIFY_RETURN(ret == 0, RETURN_TASK_ABANDON, ret);
//            break;
//        }
//        case CLIENT_TASK_SUBMIT:
//        {
//            Proto11400328 sub_request;
//            sub_request.ParsePartialFromString(request->msg_body());
//            int ret = this->process_submit(sub_request.task_id());
//            CONDITION_PLAYER_NOTIFY_RETURN(ret == 0, RETURN_TASK_SUBMIT, ret);
//            break;
//        }
//        default:
//            MSG_USER("after check npc %d", npc_id);
//            return -1;
//    }
    return 0;
}

bool Tasker::is_task_submited(uint32_t task_id)
{
    if (this->tasker_detail_->task_submited_once_.find(task_id) != this->tasker_detail_->task_submited_once_.end())
        return true;
    return false;
}

bool Tasker::is_nearby_npc(uint32_t task_id)
{
    return true;
}

/*int Tasker::bind_task(uint32_t task_id, TaskInfo &info)
{
    TaskMap::iterator iter = this->tasker_detail_->task_map_.find(task_id);
    if (iter != this->tasker_detail_->task_map_.end())
        return -1;

    if (this->tasker_detail_->task_map_.insert(TaskMap::value_type(task_id, info)).second == true)
        return 0;
    return -1;
}*/

int Tasker::unbind_task(uint32_t task_id)
{
    if (this->tasker_detail_->task_map_.erase(task_id) > 0)
        return 0;
    return -1;
}

TaskInfo *Tasker::find_task(uint32_t task_id)
{
	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
	if (!task_cfg) {
		return 0;
	}

	if(task_cfg->task_type == TaskInfo::TASK_GTYPE_REWARD_TASK) {
		return find_reward_task(task_id);
	}

    TaskMap::iterator iter = this->tasker_detail_->task_map_.find(task_id);
    if (iter == this->tasker_detail_->task_map_.end()) {
    	return 0;
    }
    return &(iter->second);
}

/*Tasker_Detail &Tasker::task_map(void)
{
    return *this->tasker_detail_;
}*/

TaskSet &Tasker::task_accepted_monster_set(void)
{
    return this->task_accepted_monster_;
}

TaskSet &Tasker::task_accepted_fb_set(void) {
	return this->task_accepted_fb_;
}

TaskSet &Tasker::task_accepted_arrive_set(void) {
	return this->task_accepted_arrive_;
}

TaskSet &Tasker::task_accepted_lvl_set(void)
{
    return this->task_accepted_lvl_;
}

TaskSet &Tasker::task_accepted_package_set(void)
{
	return this->task_accepted_package_;
}

TaskSet &Tasker::task_accepted_monster_group_set(void)
{
    return this->task_accepted_monster_group_;
}

TaskSet &Tasker::task_accepted_collect_set(void)
{
    return this->task_accepted_collect_;
}

TaskSet &Tasker::task_accepted_fb_type_set(void)
{
    return this->task_accepted_fb_type_;
}

TaskSet &Tasker::task_accepted_win_player_set(void) {
	return this->task_accepted_win_player_;
}

TaskSet& Tasker::task_accepted_join_gang_set() {
	return this->task_accepted_join_gang_;
}

TaskSet& Tasker::task_accepted_ganger_level_set() {
	return this->task_accepted_ganger_level_;
}

TaskSet& Tasker::task_accepted_player_all_equip_set() {
	return this->task_accepted_player_all_equip_;
}

TaskSet& Tasker::task_acccepted_body_equip_set() {
	return this->task_accepted_body_equip_;
}

TaskSet& Tasker::task_accepted_body_equip_level_set() {
	return this->task_accepted_body_equip_level_;
}

TaskSet& Tasker::task_accepted_equip_level_set() {
	return this->task_accepted_equip_level_;
}

TaskSet& Tasker::task_accepted_stone_level_set() {
	return this->task_accepted_stone_level_;
}

TaskSet& Tasker::task_accepted_buy_goods_set() {
	return this->task_accepted_buy_goods_;
}

TaskSet& Tasker::task_accepted_get_hero_set() {
	return this->task_accepted_get_hero_;
}

TaskSet& Tasker::task_accepted_fight_set() {
	return this->task_accepted_fight_;
}

TaskSet& Tasker::task_accepted_commit_consume_item_set() {
	return this->task_accepted_commit_consume_item_;
}

TaskSet& Tasker::task_accepted_deliver_letter_set() {
	return this->task_accepted_deliver_letter_;
}

TaskSet& Tasker::task_accepted_help_other_complete_team_scene_set() {
	return this->task_accepted_help_other_complete_team_scene_;
}

TaskSet& Tasker::task_accepted_sell_item_set() {
	return this->task_accepted_sell_item_;
}

/*TaskSet& Tasker::task_accepted_use_skill_set() {
	return this->task_accepted_use_skill_;
}

TaskSet& Tasker::task_accepted_append_skill_set() {
	return this->task_accepted_append_skill_;
}*/

Tasker::TaskSet& Tasker::task_accepted_limit_time_set() {
	return this->task_accepted_limit_time_;
}

Tasker::TaskSet& Tasker::task_accepted_arena_set() {
	return this->task_accepted_arena_;
}

Tasker::TaskSet& Tasker::task_accepted_chat_with_friend_set() {
	return this->task_accepted_chat_with_friend_;
}

Tasker::TaskSet& Tasker::task_accepted_send_mail_set() {
	return task_accepted_send_mail_;
}

Tasker::TaskSet& Tasker::task_accepted_team_scene_or_pvp_set() {
	return task_accepted_team_scene_or_pvp_;
}

Tasker::TaskSet& Tasker::task_accepted_browse_other_player_info_set() {
	return task_accepted_browse_other_player_info_;
}

Tasker::TaskSet& Tasker::task_accepted_npc_talk_set() {
	return task_accepted_npc_talk_;
}

Tasker::TaskSet& Tasker::task_accepted_arena_level_set() {
	return task_accepted_arena_level_;
}

Tasker::TaskSet& Tasker::task_accepted_equip_crimson_level_set() {
	return task_accepted_equip_crimson_level_;
}

Tasker::TaskSet& Tasker::task_accepted_arena_all_set() {
	return task_accepted_arena_all_;
}

Tasker::TaskSet& Tasker::task_accepted_create_dragon_vale_building_set() {
	return task_accepted_create_dragon_vale_building_;
}

Tasker::TaskSet& Tasker::task_accepted_gain_dragon_vale_builiding_production_set() {
	return task_accepted_gain_dragon_vale_builiding_production_;
}

Tasker::TaskSet& Tasker::task_accepted_wake_up_hero_to_quality_level_set() {
	return task_accepted_wake_up_hero_to_quality_level_;
}

Tasker::TaskSet& Tasker::task_accepted_keep_hero_to_level_set() {
	return task_accepted_keep_hero_to_level_;
}

Tasker::TaskSet& Tasker::task_accepted_compose_equip_set() {
	return task_accepted_compose_equip_;
}

Tasker::TaskSet& Tasker::task_accepted_hero_dream_scene_set() {
	return task_accepted_hero_dream_scene_;
}

Tasker::TaskSet& Tasker::task_accepted_ganger_skill_level_set() {
	return task_accepted_ganger_skill_level_;
}

Tasker::TaskSet& Tasker::task_accepted_war_set() {
	return task_accepted_war_;
}

Tasker::TaskSet& Tasker::task_accepted_war_all_set() {
	return task_accepted_war_all_;
}

Tasker::TaskSet& Tasker::task_accepted_kill_enemy_in_war_set() {
	return task_accepted_kill_enemy_in_war_;
}

Tasker::TaskSet& Tasker::task_accepted_single_arena_all_set() {
	return task_accepted_single_arena_all_;
}

Tasker::TaskSet& Tasker::task_accepted_single_arena_win_set() {
	return task_accepted_single_arena_win_;
}

Tasker::TaskSet& Tasker::task_accepted_team_arena_all_set() {
	return task_accepted_team_arena_all_;
}

Tasker::TaskSet& Tasker::task_accepted_team_arena_win_set() {
	return task_accepted_team_arena_win_;
}

Tasker::TaskSet& Tasker::task_accepted_ganger_bless_buf_set() {
	return task_accepted_ganger_bless_buf_;
}

Tasker::TaskSet& Tasker::task_accepted_rob_other_dragon_vale_all_set() {
	return task_accepted_rob_other_dragon_vale_all_;
}

Tasker::TaskSet& Tasker::task_accepted_dragon_vale_step_foot_all_set() {
	return task_accepted_dragon_vale_step_foot_all_;
}

Tasker::TaskSet& Tasker::task_accepted_activeness_of_today_set() {
	return task_accepted_activeness_of_today_;
}

Tasker::TaskSet& Tasker::task_accepted_dragon_vale_building_upgrade_set() {
	return task_accepted_dragon_vale_building_upgrade_;
}

Tasker::TaskSet& Tasker::task_accepted_limit_time_kill_boss_set() {
	return task_accepted_limit_time_kill_boss_;
}

Tasker::TaskSet& Tasker::task_accepted_dragon_keep_all_set() {
	return task_accepted_dragon_keep_all_;
}

Tasker::TaskSet& Tasker::task_accepted_dragon_keep_success_set() {
	return task_accepted_dragon_keep_success_;
}

Tasker::TaskSet& Tasker::task_accepted_donate_to_gang_set() {
	return task_accepted_donate_to_gang_;
}

Tasker::TaskSet& Tasker::task_accepted_present_buf_to_other_set() {
	return task_accepted_present_buf_to_other_;
}

Tasker::TaskSet& Tasker::task_accepted_finish_reward_task_num_set() {
	return task_accepted_finish_reward_task_num_;
}

Tasker::TaskSet& Tasker::task_accepted_keep_dragon_to_level_set() {
	return task_accepted_keep_dragon_to_level_;
}

Tasker::TaskSet& Tasker::task_accepted_knight_trial_set() {
	return task_accepted_knight_trial_;
}

Tasker::TaskSet& Tasker::task_accepted_suit_set() {
	return task_accepted_suit_;
}

Tasker::TaskSet& Tasker::task_accepted_role_pull_on_equip_set() {
	return task_accepted_role_pull_on_equip_;
}

Tasker::TaskSet& Tasker::task_accepted_hero_pull_on_equip_set() {
	return task_accepted_hero_pull_on_equip_;
}

Tasker::TaskSet& Tasker::task_accepted_activate_wing_set() {
	return task_accepted_activate_wing_;
}

Tasker::TaskSet& Tasker::task_accepted_get_quality_rune_set() {
	return task_accepted_get_quality_rune_;
}

Tasker::TaskSet& Tasker::task_accepted_pass_kngiht_trials_set() {
	return task_accepted_pass_kngiht_trials_;
}

Tasker::TaskSet& Tasker::task_accepted_have_hero_set() {
	return task_accepted_have_hero_;
}

Tasker::TaskSet& Tasker::task_accepted_have_hero_quality_set() {
	return task_accepted_have_hero_quality_;
}

Tasker::TaskSet& Tasker::task_accepted_first_pass_scene_set() {
	return task_accepted_first_pass_scene_;
}

Tasker::TaskSet& Tasker::task_accepted_dragon_born_set() {
	return task_accepted_dragon_born_;
}

Tasker::TaskSet& Tasker::task_accepted_horse_star_level_set() {
	return task_accepted_horse_star_level_;
}

Tasker::TaskSet& Tasker::task_accepted_have_item_but_no_erase_set() {
	return task_accepted_have_item_but_no_erase_;
}

Tasker::TaskSet& Tasker::task_accepted_have_equip_but_no_erase_set() {
	return task_accepted_have_equip_but_no_erase_;
}

Tasker::TaskSet& Tasker::task_accepted_same_gang_challenge_all_set() {
	return task_accepted_same_gang_challenge_all_;
}

Tasker::TaskSet& Tasker::task_accepted_main_scene_npc_battle_set() {
	return task_accepted_main_scene_npc_battle_;
}

Tasker::TaskSet& Tasker::task_accepted_equip_card_all_set() {
	return task_accepted_equip_card_all_;
}

Tasker::TaskSet& Tasker::task_accepted_equip_card_by_position_set() {
	return task_accepted_equip_card_by_position_;
}

Tasker::TaskSet& Tasker::task_accepted_card_upgrade_to_level_all_set() {
	return task_accepted_card_upgrade_to_level_all_;
}

Tasker::TaskSet& Tasker::task_accepted_card_upgrade_to_level_by_position_set() {
	return task_accepted_card_upgrade_to_level_by_position_;
}

Tasker::TaskSet& Tasker::task_accepted_lottery_draw_set() {
	return task_accepted_lottery_draw_;
}

Tasker::TaskSet& Tasker::task_accepted_equip_jewelry_set() {
	return task_accepted_equip_jewelry_;
}

Tasker::TaskSet& Tasker::task_accepted_role_equip_n_set() {
	return task_accepted_role_equip_n_;
}

Tasker::TaskSet& Tasker::task_accepted_hero_equip_n_set()
{
	return task_accepted_hero_equip_n_;
}

Tasker::TaskSet& Tasker::task_accepted_invite_friend_step_foot_set() {
	return task_accepted_invite_friend_step_foot_;
}

Tasker::TaskSet& Tasker::task_accepted_kill_group_num_set() {
	return task_accepted_kill_group_num_;
}

Tasker::TaskSet& Tasker::task_accepted_did_reward_task_num_set() {
	return task_accepted_did_reward_task_num_;
}

Tasker::TaskSet& Tasker::task_accepted_alchemy_num_set() {
	return task_accepted_alchemy_num_;
}

Tasker::TaskSet& Tasker::task_accepted_did_reward_task_num_today_set() {
	return task_accepted_did_reward_task_num_today_;
}

Tasker::TaskSet& Tasker::task_accepted_remote_scene_any_pass_set() {
	return task_accepted_remote_scene_any_pass_;
}

Tasker::TaskSet& Tasker::task_accepted_knight_trial_any_pass_set() {
	return task_accepted_knight_trial_any_pass_;
}

Tasker::TaskSet& Tasker::task_accepted_compose_gemstone_level_set() {
	return task_accepted_compose_gemstone_level_;
}

Tasker::TaskSet& Tasker::task_accepted_inlay_position_gemstone_set() {
	return task_acceped_inlay_position_gemstone_;
}

Tasker::TaskSet& Tasker::task_accepted_inlay_any_gemstone_set() {
	return task_accepted_inlay_any_gemstone_;
}

Tasker::TaskSet& Tasker::task_accepted_world_boss_set() {
	return task_accepted_world_boss_;
}

Tasker::TaskSet& Tasker::task_accepted_world_boss_immediate_revive_set() {
	return task_accepted_world_boss_immediate_revive_;
}

Tasker::TaskSet& Tasker::task_accepted_enchant_any_equip_set() {
	return task_accepted_enchant_any_equip_;
}

Tasker::TaskSet& Tasker::task_accepted_inlay_id_gemstone_set() {
	return task_accepted_inlay_id_gemstone_;
}

Tasker::TaskSet& Tasker::task_accepted_gang_well_recover_spirit_set() {
	return task_accepted_gang_well_recover_spirit_;
}

Tasker::TaskSet& Tasker::task_accepted_equip_purple_ring_set() {
	return task_accepted_equip_purple_ring_;
}

Tasker::TaskSet& Tasker::task_accepted_inlay_level_gemstone_set() {
	return task_accepted_inlay_level_gemstone_;
}

Tasker::TaskSet& Tasker::task_accepted_upgrade_card_set() {
	return task_accepted_upgrade_card_;
}

Tasker::TaskIdSet& Tasker::task_submited_once_set(void)
{
    return this->tasker_detail_->task_submited_once_;
}

Tasker::Item_TaskIdSet_Map& Tasker::task_listen_item_map() {
	return this->task_listen_item_TaskIdSet_map_;
}

Npc_Item_Chance_Set_Map& Tasker::task_listen_item_source_map() {
	return this->scene_npc_ItemIdSet_map_;
}

int Tasker::init_task_listen_item(TaskInfo *task_info)
{
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            continue;
        }
		if (!task_execute_type_is_need_listen_item(task_json->execute[i].type)
				 && task_json->execute[i].type  != TaskExecType::TASK_EXEC_HAVE_ITEM_BUT_NO_ERASE) {
            continue;
		}

		if(this->task_listen_item_TaskIdSet_map_.exist(task_json->execute[i].id)) {
			this->task_listen_item_TaskIdSet_map_[task_json->execute[i].id].insert(task_info->__task_id);
		}
		else {
			TaskIdSet task_id_set;
			task_id_set.insert(task_info->__task_id);
			this->task_listen_item_TaskIdSet_map_.bind(task_json->execute[i].id, task_id_set);
		}
    }

	init_task_listen_item_source(task_info);

    return 0;
}

int Tasker::erase_task_listen_item(TaskInfo *task_info, bool is_test /*= false*/)
{
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }
	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		if (!task_execute_type_is_need_listen_item(task_json->execute[i].type)
				&& task_json->execute[i].type  != TaskExecType::TASK_EXEC_HAVE_ITEM_BUT_NO_ERASE) {
            continue;
		}

		bool is_need_erase_item = true;

		//使用测试指令的任务，不扣道具
		if(is_test) {
			is_need_erase_item = false;
		}

		//快速完成的任务，没有物品，不用扣
		if(task_info->__is_req_fast_finish && task_json->execute[i].type != TaskExecType::TASK_EXEC_DELIVER_LETTER) {
			is_need_erase_item = false;
		}

		if(task_info->is_reward_task()
				&& task_json->execute[i].type == TaskExecType::TASK_EXEC_COMMIT_CONSUME_ITEM
				&& player_self()->get_covenant_status(REWARD_COVENANT)) {
			is_need_erase_item = false;
		}

		if(task_info->is_logic_have_item_but_no_erase()) {
			is_need_erase_item = false;
		}

		int item_id = task_json->execute[i].id;

		if (task_info->is_finish() && is_need_erase_item) {
			int item_nums = this->player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, item_id);
			if (item_nums >= task_json->execute[i].amount) {
				int result = this->player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(item_id, task_json->execute[i].amount),
						Lose_Item_DM_Info(ITEM_LOSE_TASK));
				if (0 != result) {
					MSG_USER("error item task_id:%d, item:%d", task_info->__task_id, item_id);
					return ERROR_SUB_ITEM_FAIL;
				}
			} else {
				return ERROR_ITEM_NOT_ENOUGH;
			}
		}//end if

		Item_TaskIdSet_Map::iterator it = this->task_listen_item_TaskIdSet_map_.find(item_id);
        if (it == this->task_listen_item_TaskIdSet_map_.end())
            continue;

        TaskIdSet &id_set = this->task_listen_item_TaskIdSet_map_[item_id];
        id_set.erase(task_info->__task_id);
        if (id_set.size() <= 0)
            this->task_listen_item_TaskIdSet_map_.unbind(item_id);
    }//end for

	erase_task_listen_item_source(task_info);

    return 0;
}

int Tasker::init_task_listen_item_source(TaskInfo *task_info)
{
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }

    Npc_Item_Chance_Map temp_npc_item_chance_map;
    temp_npc_item_chance_map.clear();

	for (size_t i = 0; i < task_json->execute.size(); ++i) {
        TaskCondition *task_cond = task_info->task_condition(i);
        if (task_cond == 0)
        {
            continue;
        }
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_COLLECT
				&& task_json->execute[i].type != TaskExecType::TASK_EXEC_PACKAGE) {
            continue;
		}

		int item_id = task_json->execute[i].id;
		int npc_id = task_json->execute[i].npc_id;
		int chance = task_json->execute[i].param2;

		Item_Chance item_chance;
		item_chance.val_1 = item_id;
		item_chance.val_2 = chance;

		if (item_id > 0 && npc_id > 0 && chance > 0) {
			if(this->scene_npc_ItemIdSet_map_.exist(npc_id)) {
				this->scene_npc_ItemIdSet_map_[npc_id].insert(item_chance);
			}
			else {
				Item_Chance_Set item_chance_set;
				item_chance_set.insert(item_chance);
				this->scene_npc_ItemIdSet_map_.bind(npc_id, item_chance_set);
			}

			temp_npc_item_chance_map.insert(std::make_pair(npc_id, item_chance));
		}
    }//end for


	this->scene_task_id_set_.insert(task_info->__task_id);
	TaskIdSet task_id_set;
	task_id_set.insert(task_info->__task_id);

	if(!temp_npc_item_chance_map.empty() || !task_id_set.empty()) {
		this->sync_item_source(SYNC_TASK_SOURCE_ADD, temp_npc_item_chance_map, task_id_set);
	}

    return 0;
}

int Tasker::erase_task_listen_item_source(TaskInfo *task_info)
{
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (NULL == task_json) {
    	return ERROR_TASK_NOT_EXIST;
    }

    Npc_Item_Chance_Map temp_npc_item_chance_map;
    temp_npc_item_chance_map.clear();

	for (size_t i = 0; i < task_json->execute.size(); ++i) {
		if (task_json->execute[i].type != TaskExecType::TASK_EXEC_COLLECT
				&& task_json->execute[i].type != TaskExecType::TASK_EXEC_PACKAGE) {
            continue;
		}

		int item_id = task_json->execute[i].id;
		int npc_id = task_json->execute[i].npc_id;
		int chance = task_json->execute[i].param2;
		Item_Chance item_chance;
		item_chance.val_1 = item_id;
		item_chance.val_2 = chance;

		if(item_id > 0 && npc_id > 0 && chance > 0) {
			Npc_Item_Chance_Set_Map::iterator it = this->scene_npc_ItemIdSet_map_.find(npc_id);
			if (it == this->scene_npc_ItemIdSet_map_.end())
				continue;

			Item_Chance_Set &item_chance_set = this->scene_npc_ItemIdSet_map_[npc_id];
			item_chance_set.erase(item_chance);
			if (item_chance_set.size() <= 0) {
				this->scene_npc_ItemIdSet_map_.unbind(npc_id);
			}

			temp_npc_item_chance_map.insert(std::make_pair(npc_id, item_chance));
		}
    }
	this->scene_task_id_set_.erase(task_info->__task_id);
	TaskIdSet task_id_set;
	task_id_set.insert(task_info->__task_id);

	if(!temp_npc_item_chance_map.empty() || !task_id_set.empty()) {
		this->sync_item_source(SYNC_TASK_SOURCE_DEL, temp_npc_item_chance_map, task_id_set);
	}

    return 0;
}

int Tasker::sync_item_source(const int8_t type, const Npc_Item_Chance_Map &npc_item_chance_map, const TaskIdSet &task_id_set) {
	Block_Buffer buf;
	buf.make_message(SYNC_TASK_INFO);
	buf.write_int8(type); // 1   flag
	buf.write_uint16(task_id_set.size()); //2 : task_id的个数
	for (TaskIdSet::const_iterator it = task_id_set.begin(); it != task_id_set.end(); ++it) {
		buf.write_int32(*it); //3...  task_id
	}
	buf.write_uint16(npc_item_chance_map.size()); //4  npc_id的个数
	for (Npc_Item_Chance_Map::const_iterator it = npc_item_chance_map.begin(); it != npc_item_chance_map.end(); ++it) {
		buf.write_int32(it->first);//5.npc_id;
		buf.write_int32(it->second.val_1); //6...  item_id
		buf.write_int32(it->second.val_2); //7...  chance
	}

	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

int Tasker::sync_item_source() {
	if(scene_task_id_set_.empty() && scene_npc_ItemIdSet_map_.empty()) {
		return 0;
	}

	Block_Buffer buf;
	buf.make_message(SYNC_TASK_INFO);
	buf.write_int8(SYNC_TASK_SOURCE_TRANSMIT); //1  flag
	buf.write_uint16(scene_task_id_set_.size()); //2   size  :task_id的个数
	for (TaskIdSet::const_iterator it = scene_task_id_set_.begin(); it != scene_task_id_set_.end(); ++it) {
		buf.write_int32(*it);//3... task_id
	}
	buf.write_uint16(scene_npc_ItemIdSet_map_.size());  //4  npc_id的个数
	for (Npc_Item_Chance_Set_Map::iterator it = scene_npc_ItemIdSet_map_.begin(); it != scene_npc_ItemIdSet_map_.end(); ++it) {
		buf.write_int32(it->first); //5...  npc_id
		buf.write_uint16(it->second.size()); //6... item_id 的个数
		for (Item_Chance_Set::const_iterator it_npc = it->second.begin(); it_npc != it->second.end(); ++it_npc) {
			buf.write_int32(it_npc->val_1); //7... item_id
			buf.write_int32(it_npc->val_2); //8... chance
		}
	}

	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

bool Tasker::is_task_listen_item(const int item_id)
{
	TaskIdSet task_id_set;
    return 0 == this->task_listen_item_TaskIdSet_map_.find(item_id, task_id_set);
}

int Tasker::task_listen_kill_monster(const int monster_id, const int monster_num)
{
    TaskInfo *task_info = 0;
    TaskSet temp;
    for (TaskSet::iterator iter = this->task_accepted_monster_.begin();
            iter != this->task_accepted_monster_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}
        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_kill_monster() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
    	process_listen_kill_monster(task_info, monster_id, monster_num);
    }

    temp.clear();
    return 0;
}

int Tasker::task_listen_fb(const int sceneid)
{
	TaskSet temp;
    TaskInfo *task_info = 0;
	for (TaskSet::iterator iter = this->task_accepted_fb_.begin();
			iter != this->task_accepted_fb_.end(); ++iter)
	{
		if (0 == (task_info = find_task(*iter))) {
			continue;
		}
		if (task_info->is_accepted() == false)
			continue;
		if (task_info->is_logic_fb() == false)
			continue;

		temp.insert(*iter);
	}

	for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_fb(task_info, sceneid);
	}

	temp.clear();
    return 0;
}

int Tasker::task_listen_arrive(const int sceneid, const int target_x, const int target_y)
{
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_arrive_.begin();
            iter != this->task_accepted_arrive_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}
        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_arrive() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_arrive(task_info, sceneid, target_x, target_y);
    }

    temp.clear();
    return 0;
}

int Tasker::task_listen_lvl_up(const int target_level)
{
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_lvl_.begin();
            iter != this->task_accepted_lvl_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_level_up() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
    	task_info = find_task(*it);
    	if(!task_info) {
    		continue;
    	}
    	process_listen_lvl_up(task_info, target_level);
    }
    process_safe_guide();

    temp.clear();
    this->task_construct(true);
    return 0;
}

int Tasker::task_listen_collect_item(const int item_id, const int item_num, const int bind)
{
    TaskInfo *task_info = 0;
    std::vector<TaskInfo> task_vc;
    for (TaskSet::iterator iter = this->task_accepted_collect_.begin();
            iter != this->task_accepted_collect_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}
        if (task_info->is_accepted() == false && task_info->is_finish() == false)
            continue;
        if (task_info->is_logic_collect() == false)
            continue;

        task_vc.push_back(*task_info);
    }

    for (std::vector<TaskInfo>::iterator iter = task_vc.begin();
    		iter != task_vc.end(); ++iter)
    {
    	task_info = this->find_task(iter->__task_id);
        process_listen_collect_item(task_info, item_id, item_num, bind);
    }

    task_vc.clear();
    return 0;
}

int Tasker::task_listen_kill_monster_group(const int sceneid, const int monster_group_id)
{
	TaskSet temp;
    TaskInfo *task_info = 0;
	for (TaskSet::iterator iter = this->task_accepted_monster_group_.begin();
			iter != this->task_accepted_monster_group_.end(); ++iter)
	{
		if (0 == (task_info = find_task(*iter))) {
			continue;
		}
		if (task_info->is_accepted() == false)
			continue;
		if (task_info->is_logic_kill_monster_group() == false)
			continue;

		temp.insert(*iter);
	}

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_kill_monster_group(task_info, sceneid, monster_group_id);
    }

    temp.clear();
    return 0;
}

int Tasker::task_listen_package_item(const int item_id, const int item_num, const int bind) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_package_.begin();
            iter != this->task_accepted_package_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}
        if (task_info->is_accepted() == false && task_info->is_finish() == false)
            continue;
        if (task_info->is_logic_package() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_package_item(task_info, item_id, item_num, bind);
    }

    temp.clear();
    return 0;
}

int Tasker::task_listen_fb_type(const int type)
{
	TaskSet temp;
    TaskInfo *task_info = 0;
	for (TaskSet::iterator iter = this->task_accepted_fb_type_.begin();
			iter != this->task_accepted_fb_type_.end(); ++iter)
	{
		if (0 == (task_info = find_task(*iter))) {
			continue;
		}
		if (task_info->is_accepted() == false)
			continue;
		if (task_info->is_logic_fb_type() == false)
			continue;

		temp.insert(*iter);
	}

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_fb_type(task_info, type);
    }

    temp.clear();
    return 0;
}

int Tasker::task_listen_win_player(const role_id_t role_id)
{
	TaskSet temp;
    TaskInfo *task_info = 0;
	for (TaskSet::iterator iter = this->task_accepted_win_player_.begin();
			iter != this->task_accepted_win_player_.end(); ++iter)
	{
		if (0 == (task_info = find_task(*iter))) {
			continue;
		}
		if (task_info->is_accepted() == false)
			continue;
		if (task_info->is_logic_win_player() == false)
			continue;

		temp.insert(*iter);
	}

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_win_player(task_info, role_id);
    }

    temp.clear();
    return 0;
}

int Tasker::task_listen_join_gang(gang_id_t gang_id) {
	if(!gang_id) {
		return -1;
	}

	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_join_gang_.begin();
            iter != this->task_accepted_join_gang_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_join_gang() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_join_gang(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_gang_level(gang_id_t gang_id, const int gang_level) {
	if(!gang_id) {
		return -1;
	}

	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_ganger_level_.begin();
            iter != this->task_accepted_ganger_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_ganger_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_gang_level(task_info, gang_level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_player_all_equip(const int equip_num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_player_all_equip_.begin();
            iter != this->task_accepted_player_all_equip_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_player_all_equip() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_player_all_equip_num(task_info, equip_num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_body_equip(int position, int equip_num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_body_equip_.begin();
            iter != this->task_accepted_body_equip_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_body_equip() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		 process_listen_body_equip_num(task_info, position, equip_num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_body_equip_level(int position, int equip_level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_body_equip_level_.begin();
            iter != this->task_accepted_body_equip_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_body_equip_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_body_equip_level(task_info, position, equip_level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_equip_level(uint32_t equip_id, const int equip_level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_equip_level_.begin();
            iter != this->task_accepted_equip_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_equip_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_equip_level(task_info, equip_id, equip_level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_stone_level(const int stone_level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_stone_level_.begin();
            iter != this->task_accepted_stone_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_stone_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_stone_level(task_info, stone_level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_buy_goods(Item_Vec::const_iterator item_begin, Item_Vec::const_iterator  item_end) {
	if(item_begin == item_end) {
		return -1;
	}

	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_buy_goods_.begin();
            iter != this->task_accepted_buy_goods_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_buy_goods() == false)
            continue;

        temp.insert(*iter);
    }

    while(item_begin != item_end) {
    	uint32_t item_id = item_begin->id_;
    	int item_type = get_item_type(item_begin->id_);
    	int32_t item_count = item_begin->amount_;

        for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
    		task_info = find_task(*it);
    		if(!task_info) {
    			continue;
    		}
    		process_listen_buy_goods(task_info, item_id, item_type, item_count);
        }

        ++item_begin;
    }

    temp.clear();

	return 0;
}

int Tasker::task_listen_get_hero(int32_t hero_id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_get_hero_.begin();
            iter != this->task_accepted_get_hero_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_get_hero() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_get_hero(task_info, hero_id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_fight(const int scene_id, const int monster_group_id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_fight_.begin();
            iter != this->task_accepted_fight_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_fight() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_fight(task_info, scene_id, monster_group_id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_commit_consume_item(uint32_t item_id, uint32_t item_num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_commit_consume_item_.begin();
            iter != this->task_accepted_commit_consume_item_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;

        if (task_info->is_logic_commit_consume_item() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_commit_consume_item(task_info, item_id, item_num);
    }

    temp.clear();
	return 0;
}

 int Tasker::task_listen_deliver_letter(uint32_t item_id, uint32_t item_num) {
	 	TaskSet temp;
	    TaskInfo *task_info = 0;
	    for (TaskSet::iterator iter = this->task_accepted_deliver_letter_.begin();
	            iter != this->task_accepted_deliver_letter_.end(); ++iter)
	    {
	    	if (0 == (task_info = find_task(*iter))) {
	    		continue;
	    	}

	        if (task_info->is_accepted() == false)
	            continue;
	        if (task_info->is_logic_deliver_letter() == false)
	            continue;

	        temp.insert(*iter);
	    }

	    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
			task_info = find_task(*it);
			if(!task_info) {
				continue;
			}
			process_listen_deliver_letter(task_info, item_id, item_num);
	    }

	    temp.clear();
	    return 0;
 }

int Tasker::task_listen_help_other_complete_team_scene(uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_help_other_complete_team_scene_.begin();
            iter != this->task_accepted_help_other_complete_team_scene_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_help_other_complete_team_scene() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_help_other_complete_team_scene(task_info, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_sell_item(uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_sell_item_.begin();
            iter != this->task_accepted_sell_item_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_sell_item() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_sell_item(task_info, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_arena() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_arena_.begin();
            iter != this->task_accepted_arena_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_arena() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_arena(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_chat_with_friend(role_id_t friend_id) {
	if(friend_id < 0 || player_self()->is_friend(friend_id) != 0) {
		return 0;
	}

	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_chat_with_friend_.begin();
            iter != this->task_accepted_chat_with_friend_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_chat_with_friend() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_chat_with_friend(task_info, friend_id);
    }

    temp.clear();

	return 0;
}

int Tasker::task_listen_send_mail() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_send_mail_.begin();
            iter != this->task_accepted_send_mail_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_send_mail() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_send_mail(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_team_scene_or_pvp() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_team_scene_or_pvp_.begin();
            iter != this->task_accepted_team_scene_or_pvp_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_team_scene_or_pvp() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_team_scene_or_pvp(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_browse_other_player_info(role_id_t role_id) {
	if(role_id < 0) {
		return 0;
	}

	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_browse_other_player_info_.begin();
            iter != this->task_accepted_browse_other_player_info_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_browse_other_player_info() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_browse_other_player_info(task_info, role_id);
    }

    temp.clear();

	return 0;
}

int Tasker::task_listen_npc_talk(uint32_t npc_id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_npc_talk_.begin();
            iter != this->task_accepted_npc_talk_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_npc_talk() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_npc_talk(task_info, npc_id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_arena_level(uint32_t group_id, uint32_t level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_arena_level_.begin();
            iter != this->task_accepted_arena_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_arena_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_arena_level(task_info, group_id, level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_equip_crimson_level(uint32_t position, uint32_t level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_equip_crimson_level_.begin();
            iter != this->task_accepted_equip_crimson_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_equip_crimson_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_equip_crimson_level(task_info, position, level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_arena_all() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_arena_all_.begin();
            iter != this->task_accepted_arena_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_arena_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_arena_all(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_create_dragon_vale_building(uint8_t building_type) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_create_dragon_vale_building_.begin();
            iter != this->task_accepted_create_dragon_vale_building_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_create_dragon_vale_building() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_create_dragon_vale_building(task_info, building_type);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_gain_dragon_vale_builiding_production(uint8_t building_type) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_gain_dragon_vale_builiding_production_.begin();
            iter != this->task_accepted_gain_dragon_vale_builiding_production_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_gain_dragon_vale_builiding_production() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_gain_dragon_vale_builiding_production(task_info, building_type);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_wake_up_hero_to_quality_level(uint32_t hero_id, uint32_t quality) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_wake_up_hero_to_quality_level_.begin();
            iter != this->task_accepted_wake_up_hero_to_quality_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_wake_up_hero_to_quality_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_wake_up_hero_to_quality_level(task_info, hero_id, quality);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_keep_hero_to_level(uint32_t hero_id, uint32_t hero_level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_keep_hero_to_level_.begin();
            iter != this->task_accepted_keep_hero_to_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_keep_hero_to_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_keep_hero_to_level(task_info, hero_id, hero_level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_compose_equip(uint32_t equip_id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_compose_equip_.begin();
            iter != this->task_accepted_compose_equip_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_compose_equip() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_compose_equip(task_info, equip_id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_hero_dream_scene(uint32_t id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_hero_dream_scene_.begin();
            iter != this->task_accepted_hero_dream_scene_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_hero_dream_scene() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_hero_dream_scene(task_info, id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_ganger_skill_level(uint32_t id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_ganger_skill_level_.begin();
            iter != this->task_accepted_ganger_skill_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_ganger_skill_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_ganger_skill_level(task_info, id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_war() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_war_.begin();
            iter != this->task_accepted_war_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_war() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_war(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_war_all() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_war_all_.begin();
            iter != this->task_accepted_war_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_war_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_war_all(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_kill_enemy_in_war(uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_kill_enemy_in_war_.begin();
            iter != this->task_accepted_kill_enemy_in_war_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_kill_enemy_in_war() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_kill_enemy_in_war(task_info, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_single_arena_all() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_single_arena_all_.begin();
            iter != this->task_accepted_single_arena_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_single_arena_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_single_arena_all(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_single_arena_win() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_single_arena_win_.begin();
            iter != this->task_accepted_single_arena_win_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_single_arena_win() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_single_arena_win(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_team_arena_all() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_team_arena_all_.begin();
            iter != this->task_accepted_team_arena_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_team_arena_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_team_arena_all(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_team_arena_win() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_team_arena_win_.begin();
            iter != this->task_accepted_team_arena_win_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_team_arena_win() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_team_arena_win(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_ganger_bless_buf() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_ganger_bless_buf_.begin();
            iter != this->task_accepted_ganger_bless_buf_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_ganger_bless_buf() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_ganger_bless_buf(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_rob_other_dragon_vale_all() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_rob_other_dragon_vale_all_.begin();
            iter != this->task_accepted_rob_other_dragon_vale_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_rob_other_dragon_vale_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_rob_other_dragon_vale_all(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_dragon_vale_step_foot_all() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_dragon_vale_step_foot_all_.begin();
            iter != this->task_accepted_dragon_vale_step_foot_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_loigc_dragon_vale_step_foot_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_dragon_vale_step_foot_all(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_activeness_of_today(uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_activeness_of_today_.begin();
            iter != this->task_accepted_activeness_of_today_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_activeness_of_today() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_activeness_of_today(task_info, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_dragon_vale_building_upgrade(uint8_t type, uint16_t level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_dragon_vale_building_upgrade_.begin();
            iter != this->task_accepted_dragon_vale_building_upgrade_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_dragon_vale_building_upgrade() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_dragon_vale_building_upgrade(task_info, type, level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_limit_time_kill_boss(uint32_t monster_id, uint32_t monster_num) {
	/*//FIXME
	MSG_DEBUG("限时击杀:  监听  role_name=%s, monster_id=%d, monster_num=%d",
			player_self()->role_name().c_str(), (int)monster_id, monster_num);
	*/
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_limit_time_kill_boss_.begin();
            iter != this->task_accepted_limit_time_kill_boss_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_limit_time_kill_boss() == false)
            continue;

        temp.insert(*iter);
    }

    bool has_finish_task = false; // 这个杀怪任务是否有完成了任务
    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		if (1 == process_listen_limit_time_kill_boss(task_info,  monster_id, monster_num)) {
			has_finish_task = true;
		}
    }

    temp.clear();
	return has_finish_task;
}

int Tasker::task_listen_dragon_keep_all() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_dragon_keep_all_.begin();
            iter != this->task_accepted_dragon_keep_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_dragon_keep_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_dragon_keep_all(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_dragon_keep_success() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_dragon_keep_success_.begin();
            iter != this->task_accepted_dragon_keep_success_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_dragon_keep_success() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_dragon_keep_success(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_donate_to_gang() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_donate_to_gang_.begin();
            iter != this->task_accepted_donate_to_gang_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_donate_to_gang() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_donate_to_gang(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_present_buf_to_other() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_present_buf_to_other_.begin();
            iter != this->task_accepted_present_buf_to_other_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_present_buf_to_other() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_present_buf_to_other(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_finish_reward_task_num() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_finish_reward_task_num_.begin();
            iter != this->task_accepted_finish_reward_task_num_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_finish_reward_task_num() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_finish_reward_task_num(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_keep_dragon_to_level(uint8_t keep_type, uint32_t keep_level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_keep_dragon_to_level_.begin();
            iter != this->task_accepted_keep_dragon_to_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_keep_dragon_to_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_keep_dragon_to_level(task_info, keep_type, keep_level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_knight_trial(uint8_t cur_points) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_knight_trial_.begin();
            iter != this->task_accepted_knight_trial_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_knight_trial() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_knight_trial(task_info, cur_points);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_role_pull_on_equip(uint32_t equip_id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_role_pull_on_equip_.begin();
            iter != this->task_accepted_role_pull_on_equip_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_role_pull_on_equip() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_role_pull_on_equip(task_info, equip_id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_hero_pull_on_equip(uint32_t equip_id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_hero_pull_on_equip_.begin();
            iter != this->task_accepted_hero_pull_on_equip_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_hero_pull_on_equip() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_hero_pull_on_equip(task_info, equip_id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_activate_wing() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_activate_wing_.begin();
            iter != this->task_accepted_activate_wing_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_activate_wing() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_activate_wing(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_get_quality_rune(uint32_t quality, uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_get_quality_rune_.begin();
            iter != this->task_accepted_get_quality_rune_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_get_quality_rune() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_get_quality_rune(task_info, quality, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_pass_kngiht_trials() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_pass_kngiht_trials_.begin();
            iter != this->task_accepted_pass_kngiht_trials_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_pass_kngiht_trials() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_pass_kngiht_trials(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_have_hero(uint32_t hero_num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_have_hero_.begin();
            iter != this->task_accepted_have_hero_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_have_hero() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_have_hero(task_info, hero_num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_have_hero_quality() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_have_hero_quality_.begin();
            iter != this->task_accepted_have_hero_quality_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_have_hero_quality() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}

    	std::map<int32_t, uint32_t> hero_quality_num_map;
    	hero_quality_num_map.clear();
    	player_self()->get_hero_quality_num_map(hero_quality_num_map);

    	uint32_t quality = 0;
    	uint32_t num = 0;
        const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
		   if(task_json) {
				for (size_t i = 0; i < task_json->execute.size(); ++i) {
				   if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HAVE_HERO_QUALITY) {
						for(std::map<int32_t, uint32_t>::const_iterator it = hero_quality_num_map.begin();
								it != hero_quality_num_map.end();
								++it) {
							if((it->first + 1) >= task_json->execute[i].id) {
								quality = task_json->execute[i].id;
								num += it->second;
							}
						}//end for

					  break;
				   }//end if
				}//end for

				process_listen_have_hero_quality(task_info, quality, num);
		   }//end if
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_first_pass_scene(uint32_t scene_id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_first_pass_scene_.begin();
            iter != this->task_accepted_first_pass_scene_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_first_pass_scene() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_first_pass_scene(task_info, scene_id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_dragon_born() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_dragon_born_.begin();
            iter != this->task_accepted_dragon_born_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_dragon_born() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_dragon_born(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_horse_star_level(uint32_t horse_id, uint32_t star_level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_horse_star_level_.begin();
            iter != this->task_accepted_horse_star_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_horse_star_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_horse_star_level(task_info, horse_id, star_level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_have_item_but_no_erase(uint32_t item_id, uint32_t item_num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_have_item_but_no_erase_.begin();
            iter != this->task_accepted_have_item_but_no_erase_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_have_item_but_no_erase() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_have_item_but_no_erase(task_info, item_id, item_num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_have_equip_but_no_erase(uint32_t item_id, uint32_t item_num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_have_equip_but_no_erase_.begin();
            iter != this->task_accepted_have_equip_but_no_erase_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_have_equip_but_no_erase() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_have_equip_but_no_erase(task_info, item_id, item_num);
    }

    temp.clear();
	return 0;
}

void Tasker::task_listen_have_equip_but_no_erase() {
	if(task_accepted_have_equip_but_no_erase_.empty()) {
		return;
	}

	TaskIdSet temp_task_set;
	temp_task_set.clear();
	temp_task_set.insert(this->task_accepted_have_equip_but_no_erase_.begin(), this->task_accepted_have_equip_but_no_erase_.end());

	for(TaskIdSet::const_iterator it = temp_task_set.begin(); it != temp_task_set.end(); ++it) {
		int task_id = *it;
		const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
		if(!task_cfg) {
			continue;
		}

		Int_Set temp_id_set;

		for (size_t i = 0; i < task_cfg->execute.size(); ++i) {
			if(task_cfg->execute[i].type == TaskExecType::TASK_EXEC_HAVE_EQUIP_BUT_NO_ERASE) {
				temp_id_set.insert(task_cfg->execute[i].id_list.begin(), task_cfg->execute[i].id_list.end());
				break;
			}
		}

		for(Int_Set::const_iterator set_it = temp_id_set.begin(); set_it != temp_id_set.end(); ++set_it) {
			const int item_id = *set_it;
			uint32_t item_num = 0;
	    	const Item_Detail *item = 0;

	    	//人物装备
	    	item_num += player_self()->pack_calc_item(PACK_T_EQUIP_INDEX, item_id);

	    	//人物背包
	    	item_num += player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, item_id);

	    	//英雄装备
	    	for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
	    			it != player_self()->heroer_detail().hero_map.end();
	    			++it) {
	    		int pack_type = PACK_T_HERO_EQUIP_INDEX + it->second.hero_index * ONE_HERO_EQUIP_GAP;
	    		uint cap = player_self()->pack_is_hero_type(pack_type) ?  player_self()->pack_get_hero_capacity_public(pack_type) :  player_self()->pack_get_capacity_public((Pack_Type)pack_type);
	    		int idx_start = pack_type + RS_INNER_IDX_BEGIN;
	    		int idx_end = pack_type + cap + 1;

	    		for (int idx = idx_start; idx < idx_end; ++idx) {
	    			item = player_self()->pack_get_const_item(idx);
	    			if (!item) {
	    				continue;
	    			}

	    			if(item->id_ != (uint32_t)item_id) {
	    				continue;
	    			}

	    			item_num += item->amount_;
	    		}
	    	}//end for

	    	task_listen_have_equip_but_no_erase(item_id, item_num);
		}//end for
	}//end for
}

int Tasker::task_listen_same_gang_challenge_all() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_same_gang_challenge_all_.begin();
            iter != this->task_accepted_same_gang_challenge_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_same_gang_challenge_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_same_gang_challenge_all(task_info);
    }

    temp.clear();

	return 0;
}

int Tasker::task_listen_main_scene_npc_battle(uint32_t monster_id, uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_main_scene_npc_battle_.begin();
            iter != this->task_accepted_main_scene_npc_battle_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_main_scene_npc_battle() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_main_scene_npc_battle(task_info, monster_id, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_equip_card_all(int num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_equip_card_all_.begin();
            iter != this->task_accepted_equip_card_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_equip_card_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_equip_card_all(task_info, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_equip_card() {
	if(task_accepted_equip_card_all_.empty()
			&& task_accepted_equip_card_by_position_.empty()
			&& task_accepted_card_upgrade_to_level_all_.empty()
			&& task_accepted_card_upgrade_to_level_by_position_.empty()) {
		return 0;
	}

	int num = 0;
	std::map<int, int> equip_card_position_num_map;
	int level = 0;
	std::map<int, int> equip_card_position_level_map;

   	const Item_Detail *equip = NULL;
	uint32_t equip_id = 0;
	int position = 0;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		equip = player_self()->pack_get_const_item(index);
		if (! equip) {
			continue;
		}

		equip_id = equip->id_;
		position = player_self()->get_part_by_id((int)equip->id_);

		for (int i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
			int card_idx = equip->addon.equip.card_hole_array_[i].card_index;
			if (0 == card_idx) {
				continue;
			}

			const Item_Detail *card = player_self()->pack_get_const_item(card_idx);
			if (0 == card) {
				continue;
			}

			if(card->type_ != Item_Detail::CARD) {
				continue;
			}

			++num;

			if(card->addon.card.lv_ >= level) {
				level = card->addon.card.lv_;
			}

			std::map<int, int>::iterator find_it = equip_card_position_num_map.find(position);
			if(find_it == equip_card_position_num_map.end()) {
				equip_card_position_num_map.insert(std::make_pair(position, 1));
			}
			else {
				find_it->second += 1;
			}

			std::map<int, int>::iterator level_it = equip_card_position_level_map.find(position);
			if(level_it == equip_card_position_level_map.end()) {
				equip_card_position_level_map.insert(std::make_pair(position, card->addon.card.lv_));
			}
			else {
				if(card->addon.card.lv_ >= level_it->second) {
					level_it->second = card->addon.card.lv_;
				}
			}
		}
	}//end for

	for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end();
			++it) {
		int pack_type = PACK_T_HERO_EQUIP_INDEX + it->second.hero_index * ONE_HERO_EQUIP_GAP;
		uint cap = player_self()->pack_is_hero_type(pack_type) ?  player_self()->pack_get_hero_capacity_public(pack_type) :  player_self()->pack_get_capacity_public((Pack_Type)pack_type);
		int idx_start = pack_type + RS_INNER_IDX_BEGIN;
		int idx_end = pack_type + cap + 1;
		for (int idx = idx_start; idx < idx_end; ++idx) {
			equip = player_self()->pack_get_const_item(idx);
			if (!equip) {
				continue;
			}

			equip_id = equip->id_;
			position = player_self()->get_part_by_id((int)equip->id_);

			for (int i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
				int card_idx = equip->addon.equip.card_hole_array_[i].card_index;
				if (0 == card_idx) {
					continue;
				}

				const Item_Detail *card = player_self()->pack_get_const_item(card_idx);
				if (0 == card) {
					continue;
				}

				if(card->type_ != Item_Detail::CARD) {
					continue;
				}

				++num;

				if(card->addon.card.lv_ >= level) {
					level = card->addon.card.lv_;
				}

				std::map<int, int>::iterator find_it = equip_card_position_num_map.find(position);
				if(find_it == equip_card_position_num_map.end()) {
					equip_card_position_num_map.insert(std::make_pair(position, 1));
				}
				else {
					find_it->second += 1;
				}

				std::map<int, int>::iterator level_it = equip_card_position_level_map.find(position);
				if(level_it == equip_card_position_level_map.end()) {
					equip_card_position_level_map.insert(std::make_pair(position, card->addon.card.lv_));
				}
				else {
					if(card->addon.card.lv_ >= level_it->second) {
						level_it->second = card->addon.card.lv_;
					}
				}
			}
		}
	}//end for

	if(!task_accepted_equip_card_all_.empty()) {
		task_listen_equip_card_all(num);
	}

	if(!task_accepted_equip_card_by_position_.empty()) {
		for(std::map<int, int>::const_iterator it = equip_card_position_num_map.begin();
				it != equip_card_position_num_map.end();
				++it) {
			task_listen_equip_card_by_position(it->first, it->second);
		}
	}

	if(!task_accepted_card_upgrade_to_level_all_.empty()) {
		task_listen_card_upgrade_to_level_all(level);
	}

	if(!task_accepted_card_upgrade_to_level_by_position_.empty()) {
		for(std::map<int, int>::const_iterator it = equip_card_position_level_map.begin();
				it != equip_card_position_level_map.end();
				++it) {
			task_listen_card_upgrade_to_level_by_position(it->first, it->second);
		}
	}

	equip_card_position_num_map.clear();
	equip_card_position_level_map.clear();
	return 0;
}

int Tasker::task_listen_equip_card_by_position(const uint32_t position, int num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_equip_card_by_position_.begin();
            iter != this->task_accepted_equip_card_by_position_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_equip_card_by_position() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_equip_card_by_position(task_info, position, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_card_upgrade_to_level_all(const uint32_t level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_card_upgrade_to_level_all_.begin();
            iter != this->task_accepted_card_upgrade_to_level_all_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_card_upgrade_to_level_all() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_card_upgrade_to_level_all(task_info, level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_card_upgrade_to_level_by_position(const uint32_t position, const uint32_t level) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_card_upgrade_to_level_by_position_.begin();
            iter != this->task_accepted_card_upgrade_to_level_by_position_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_card_upgrade_to_level_by_position() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_card_upgrade_to_level_by_position(task_info, position, level);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_lottery_draw(const uint32_t times) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_lottery_draw_.begin();
            iter != this->task_accepted_lottery_draw_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_lottery_draw() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_lottery_draw(task_info, times);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_equip_jewelry() {
	if(task_accepted_equip_jewelry_.empty()) {
		return 0;
	}

	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_equip_jewelry_.begin();
            iter != this->task_accepted_equip_jewelry_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_equip_jewelry() == false)
            continue;

        temp.insert(*iter);
    }

    std::map<int32_t, int32_t> jewelry_map;
    jewelry_map.clear();
    player_self()->get_jewelry_color_num(jewelry_map);

	for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end();
			++it) {
		int pack_type = PACK_T_HERO_EQUIP_INDEX + it->second.hero_index * ONE_HERO_EQUIP_GAP;
		uint cap = player_self()->pack_is_hero_type(pack_type) ?  player_self()->pack_get_hero_capacity_public(pack_type) :  player_self()->pack_get_capacity_public((Pack_Type)pack_type);
		int idx_start = pack_type + RS_INNER_IDX_BEGIN;
		int idx_end = pack_type + cap + 1;
		for (int idx = idx_start; idx < idx_end; ++idx) {
			const Item_Detail *equip = player_self()->pack_get_const_item(idx);
			if (!equip) {
				continue;
			}

			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
			if (!item_config) {
				continue;
			}

			int position = player_self()->get_part_by_id((int)equip->id_);
			if(position != EP_JEWELRY) {
				continue;
			}

			std::map<int32_t, int32_t>::iterator find_it = jewelry_map.find((int32_t)item_config->color);
			if(find_it == jewelry_map.end()) {
				jewelry_map.insert(std::make_pair((int32_t)item_config->color, equip->amount_));
			}
			else {
				find_it->second += equip->amount_;
			}
		}
	}//end for

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}

	    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	    if (NULL == task_json) {
	    	continue;
	    }

	    int32_t quality = 0;
	    int32_t num = 0;

		for (size_t i = 0; i < task_json->execute.size(); ++i) {
	        TaskCondition *task_cond = task_info->task_condition(i);
	        if (task_cond == 0)
	        {
	            continue;
	        }

	        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_EQUIP_JEWELRY) {
	        	quality = task_json->execute[i].id;
	        }
	        break;
		}

		for(std::map<int32_t, int32_t>::const_iterator it = jewelry_map.begin(); it != jewelry_map.end(); ++it) {
			if(it->first >= quality) {
				num += it->second;
			}
		}

		process_listen_equip_jewelry(task_info, quality, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_role_equip_n()
{
	if (task_accepted_role_equip_n_.empty()) {
		return 0;
	}

	//收集未完成任务
	TaskSet temp;
	TaskInfo *task_info = 0;
	for (TaskSet::iterator iter = this->task_accepted_role_equip_n_.begin();
			iter != this->task_accepted_role_equip_n_.end(); ++iter)
	{
		if (0 == (task_info = find_task(*iter))) {
			continue;
		}

		if (task_info->is_accepted() == false)
			continue;
		if (task_info->is_logic_role_equip_4() == false)
			continue;

		temp.insert(*iter);
	}

	std::map<int32_t, int32_t> equip_map, tempMap;
	equip_map.clear();

#define MAP_MERGE(dst, src) for(std::map<int32_t, int32_t>::iterator iter = src.begin(); \
				iter != src.end(); \
				++iter) { \
			if (dst.find(iter->first) == dst.end()) \
				dst.insert(*iter); \
			else \
				dst.find(iter->first)->second += iter->second; \
		}

	player_self()->get_equip_color_num(EP_WEAPONS, tempMap);
	MAP_MERGE(equip_map, tempMap);
	player_self()->get_equip_color_num(EP_HAT, tempMap);
	MAP_MERGE(equip_map, tempMap);
	player_self()->get_equip_color_num(EP_CLOTHING, tempMap);
	MAP_MERGE(equip_map, tempMap);
	player_self()->get_equip_color_num(EP_SHOES, tempMap);
	MAP_MERGE(equip_map, tempMap);
#undef MAP_MERGE

	//逐个任务取得信息，处理
	for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
			task_info = find_task(*it);
			if(!task_info) {
				continue;
			}

		    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
		    if (NULL == task_json) {
		    	continue;
		    }

		    int32_t quality = 0;
		    int32_t num = 0;

			for (size_t i = 0; i < task_json->execute.size(); ++i) {
		        TaskCondition *task_cond = task_info->task_condition(i);
		        if (task_cond == 0)
		        {
		            continue;
		        }

		        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_ROLE_EQUIP_4) {
		        	quality = task_json->execute[i].id;
		        }
		        break;
			}

			for(std::map<int32_t, int32_t>::const_iterator it = equip_map.begin(); it != equip_map.end(); ++it) {
				if(it->first >= quality) {
					num += it->second;
				}
			}

			process_listen_role_equip_4(task_info, quality, num);
	    }

	    temp.clear();
	return 0;
}

int Tasker::task_listen_hero_equip_n() {
	if (task_accepted_hero_equip_n_.empty()) {
		return 0;
	}

	//收集未完成任务
	TaskSet temp;
	TaskInfo *task_info = 0;
	for (TaskSet::iterator iter = this->task_accepted_hero_equip_n_.begin();
			iter != this->task_accepted_hero_equip_n_.end(); ++iter)
	{
		if (0 == (task_info = find_task(*iter))) {
			continue;
		}

		if (task_info->is_accepted() == false)
			continue;
		if (task_info->is_logic_hero_equip_4() == false)
			continue;

		temp.insert(*iter);
	}

//	TaskSet temp_done; //上一轮循环已完成的任务
	std::map<int32_t, int32_t> equip_map;
	for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
//		if (temp_done.find(*it) != temp_done.end())
//			continue;

		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}

		const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
		if (NULL == task_json) {
			continue;
		}

		int32_t quality = 0;
		int32_t num_req = 0;
		for (size_t i = 0; i < task_json->execute.size(); ++i) {
			TaskCondition *task_cond = task_info->task_condition(i);
			if (task_cond == 0)
			{
				continue;
			}

			if(task_json->execute[i].type == TaskExecType::TASK_EXEC_HERO_EQUIP_4) {
				quality = task_json->execute[i].id;
				num_req = task_json->execute[i].amount;
			}
			break;
		}

		int most_equip = 0; //穿最多的英雄穿了多少件
		for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
				it != player_self()->heroer_detail().hero_map.end();
				++it) {
			std::map<int32_t, int32_t> equip_map;

			int pack_type = PACK_T_HERO_EQUIP_INDEX + it->second.hero_index * ONE_HERO_EQUIP_GAP;
			uint cap = player_self()->pack_is_hero_type(pack_type) ?  player_self()->pack_get_hero_capacity_public(pack_type) :  player_self()->pack_get_capacity_public((Pack_Type)pack_type);
			int idx_start = pack_type + RS_INNER_IDX_BEGIN;
			int idx_end = pack_type + cap + 1;
			for (int idx = idx_start; idx < idx_end; ++idx) {
				const Item_Detail *equip = player_self()->pack_get_const_item(idx);
				if (!equip) {
					continue;
				}

				const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
				if (!item_config) {
					continue;
				}

				//			EP_WEAPONS = 1,		// 武器
				//			EP_HAT = 2,			// 帽子
				//			EP_CLOTHING = 3,	// 衣服
				//			EP_SHOES = 4,		// 鞋子
				int position = player_self()->get_part_by_id((int)equip->id_);
				if(position != EP_WEAPONS &&
						position != EP_HAT &&
						position != EP_CLOTHING &&
						position != EP_SHOES) {
					continue;
				}

				std::map<int32_t, int32_t>::iterator find_it = equip_map.find((int32_t)item_config->color);
				if(find_it == equip_map.end()) {
					equip_map.insert(std::make_pair((int32_t)item_config->color, equip->amount_));
				}
				else {
					find_it->second += equip->amount_;
				}
			}

			int32_t num_act = 0; //当前英雄穿了多少件
			for(std::map<int32_t, int32_t>::const_iterator it = equip_map.begin(); it != equip_map.end(); ++it) {
				if(it->first >= quality) {
					num_act += it->second;
				}
			}

			if (num_act > most_equip)
				most_equip = num_act;
		}

		process_listen_hero_equip_4(task_info, quality, most_equip);
	}

	temp.clear();
	return 0;
}

int Tasker::task_listen_invite_friend_step_foot() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_invite_friend_step_foot_.begin();
            iter != this->task_accepted_invite_friend_step_foot_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_invite_friend_step_foot() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_invite_friend_step_foot(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_kill_group_num(const uint32_t group_id) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_kill_group_num_.begin();
            iter != this->task_accepted_kill_group_num_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_kill_group_num() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_kill_group_num(task_info, group_id);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_did_reward_task_num(const uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_did_reward_task_num_.begin();
            iter != this->task_accepted_did_reward_task_num_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_did_reward_task_num() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_did_reward_task_num(task_info, num);
    }

    temp.clear();
	return 0;
}

 /*int Tasker::task_listen_use_skill(uint32_t skill_id) {
	    TaskInfo *task_info = 0;
	    for (TaskSet::iterator iter = this->task_accepted_use_skill_.begin();
	            iter != this->task_accepted_use_skill_.end(); ++iter)
	    {
	    	if (0 == (task_info = find_task(*iter))) {
	    		continue;
	    	}

	        if (task_info->is_accepted() == false)
	            continue;
	        if (task_info->is_logic_use_skill() == false)
	            continue;

	        process_listen_use_skill(task_info, skill_id);
	    }
	 return 0;
 }

int Tasker::task_listen_append_skill() {
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_append_skill_.begin();
            iter != this->task_accepted_append_skill_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_append_skill() == false)
            continue;

        process_listen_append_skill(task_info);
    }
	return 0;
}*/

bool Tasker::task_listen_item_update(const int item_id, const int num) {
	if (!is_task_listen_item(item_id)) {
		return false;
	}
	int amount = num;
	if (amount < 0) {
		amount = 0;
	}

	int ret_package = task_listen_package_item(item_id, amount);
	int ret_collect = task_listen_collect_item(item_id, amount);
	int ret_commit_consume_item = task_listen_commit_consume_item(item_id, amount);
	int ret_deliver_letter = task_listen_deliver_letter(item_id, amount);
	int ret_have_item_but_no_erase = task_listen_have_item_but_no_erase(item_id, amount);
	if (0 == ret_collect
			|| 0 == ret_package
			|| 0 == ret_commit_consume_item
			|| 0 == ret_deliver_letter
			|| 0 == ret_have_item_but_no_erase) {
		return true;
	}
	return false;
}

void Tasker::task_listen_item_update(void) {
	if(this->task_listen_item_TaskIdSet_map_.empty()) {
		return;
	}

	Item_TaskIdSet_Map temp;

	for (Item_TaskIdSet_Map::const_iterator item_id_it = this->task_listen_item_TaskIdSet_map_.begin();
			item_id_it != this->task_listen_item_TaskIdSet_map_.end();
			++item_id_it) {
		if(item_id_it->second.empty()) {
			continue;
		}

		for(TaskIdSet::const_iterator task_id_it = item_id_it->second.begin();
				task_id_it != item_id_it->second.end();
				++task_id_it) {

			if(temp.exist(item_id_it->first)) {
				temp[item_id_it->first].insert(*task_id_it);
			}
			else {
				TaskIdSet task_id_set;
				task_id_set.insert(*task_id_it);
				temp.bind(item_id_it->first, task_id_set);
			}
		}
	}

	for (Item_TaskIdSet_Map::iterator it = temp.begin();
			it != temp.end(); ++it) {
		int item_id = (*it).first;
		int pack_item_amount = this->player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, item_id);
		task_listen_item_update(item_id, pack_item_amount);
	}

	temp.clear();
}


int Tasker::process_accept(uint32_t task_id, bool is_auto_accept /*=true*/)
{
    const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(task_id);
    if (!task_config) {
    	return ERROR_TASK_NOT_EXIST;
    }

	TaskInfo *task_info = find_task(task_id);
	if (0 == task_info) {
    	return ERROR_TASK_NOT_EXIST;
	}

	//前置任务做完，当前任务不可接，给客户端发送不可接状态
	if(task_info->is_visible()
			&& task_config->precondition != 0
			&& this->tasker_detail_->task_submited_once_.find(task_config->precondition)
			  != this->tasker_detail_->task_submited_once_.end()) {
		notify_task_add(*task_info, *task_config);
		return 0;
	}

	if(task_info->is_accepted()) {
		return ERROR_TASK_ACCEPTED;
	}

	if(task_info->is_finish()) {
		return ERROR_TASK_FINISHED;
	}

	if(task_state(task_info->__task_id) == TaskInfo::TASK_STATUS_SUBMIT) {
		return ERROR_TASK_SUBMITED;
	}

    if (task_info->is_acceptable() != true) {
    	return ERROR_TASK_NOT_ACCEPT;
    }

/*	if(task_config->precondition == 0 && task_config->accept.level != 0
			&& player_self()->level() < task_config->accept.level) {
		MSG_USER("player level lower than task's min accept level, player_level:%d, task accept min level:%d, "
				"task_id=%d\n", player_self()->level(), task_config->accept.level, task_id);
		return ERROR_LEVEL_LOWER_ACCPET_TASK_MIN_LEVEL;
	}

	if(task_config->precondition == 0 && task_config->accept.level_max != 0
			&& player_self()->level() > task_config->accept.level_max) {
		MSG_USER("player level bigger than task's max accept level, player_level:%d, task accept max level:%d, "
				"task_id=%d\n", player_self()->level(), task_config->accept.level_max, task_id);
		return ERROR_LEVEL_BIGGER_ACCEPT_TASK_MAX_LEVEL;
	}*/

	const Reward_Task_Config&  reward_task_cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	uint32_t max_accept_count =	reward_task_cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ tasker_detail_->give_up_num
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);

	if(task_info->is_reward_task() && (tasker_detail_->used_accept_count >= max_accept_count)) {
		return ERROR_REWARD_TASK_ACCEPT_COUNT_BIGGER_MAX;
	}

	//背包空间不足，不能接悬赏任务
    if(task_info->is_reward_task()) {
    	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
		if(CONFIG_CACHE_ITEM->find_item(cfg.item_id)) {
			Item_Detail item(cfg.item_id,cfg.item_num, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
			if(player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX, item) != 0) {
				return ERROR_PACKAGE_FULL_NO_ACCEPT_REWARD_TASK;
			}
		}
    }

	//领取悬赏榜刷新出来的橙色品质悬赏任务，扣悬赏令
	if(task_info->is_reward_task() && task_config->quality == TASK_QUALITY_ORANGE
			&& task_config->accept.val3 && task_config->accept.val4) {
    	//判断物品是否足够
    	int pack_item_amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,
    			task_config->accept.val3);
    	if(pack_item_amount < (int)task_config->accept.val4) {
    		MSG_82100008 msg;
    		msg.item_ids.push_back((int)task_config->accept.val3);
    		THIS_SEND_TO_CLIENT(msg);
    		return 0;
    	}
    	//扣物品
		int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
				 Id_Amount(task_config->accept.val3, task_config->accept.val4),
				 Lose_Item_DM_Info(ITEM_LOST_ACCEPT_ORANGE_REWARD_TASK));
		if (0 != result) {
			MSG_USER("error :sub item failed, task_id:%d, item_id:%d, item_num:%d\n",
					task_info->__task_id, task_config->accept.val3, task_config->accept.val4);
			return ERROR_SUB_ITEM_FAIL;
		}
	}

    int ret = process_accept_imp(task_info->__task_id, is_auto_accept);
    if (0 == ret) {
		this->tasker_detail_->save_tick();
    }
    return ret;
}

int Tasker::process_abandon(uint32_t task_id)
{
	TaskInfo *task_info = find_task(task_id);
	if (0 == task_info) {
    	return ERROR_TASK_NOT_EXIST;
	}

    if (task_info->is_accepted() == false) {
    	return ERROR_TASK_ACCEPT_NOT_NOW;
    }
    if (task_info->is_main_task() == true) {
    	return ERROR_MAIN_TASK_NOT_ABANDON;
    }
    const Task_Mission_Detail_Config *tconfig = CONFIG_CACHE_TASK->find_task(task_id);
    if (!tconfig) {
    	return ERROR_TASK_NOT_EXIST;
    }
    if (!tconfig->giveuptask) {
    	return ERROR_TASK_NOT_ABANDON;
    }

    int ret = process_abandon_imp(task_info);
    if (0 == ret) {
		this->tasker_detail_->save_tick();
    }
    return ret;
}

int Tasker::process_submit(uint32_t task_id)
{
	TaskInfo *task_info = find_task(task_id);
	if (0 == task_info) {
    	return ERROR_TASK_NOT_EXIST;
	}

    if (task_info->is_finish() != true) {
    	return ERROR_TASK_NOT_FINISH;
    }

    int ret = process_submit_imp(task_info);
    if (0 == ret) {
		this->tasker_detail_->save_tick();

    	player_self()->title_property_refresh();
    }
    return ret;
}

int Tasker::process_fast_finish(uint32_t task_id, uint8_t type, uint8_t is_bind_diamond_first)
{
	TaskInfo *task_info = find_task(task_id);
	if (0 == task_info) {
    	return ERROR_TASK_NOT_EXIST;
	}

    if (task_info->is_accepted() != true) {
    	return ERROR_TASK_ACCEPT_NOT_NOW;
    }

    task_info->__item_money_flag = type;
    this->save_tick();
    return process_fast_finish_imp(task_info, is_bind_diamond_first);
}

int Tasker::notify_task_add(TaskInfo &task_info, const Task_Mission_Detail_Config &task_json)
{
	if (task_info.__task_id == 0) {
		return 0;
	}
    int task_status = task_state(task_info.__task_id);
    if (task_status <= 0)
        return 0;

	MSG_81110000 msg;
	msg.operate = 1;
	msg.task.id = task_info.__task_id;
	msg.task.type = task_info.__game_type;
	msg.task.status = task_state(task_info.__task_id);
	msg.task.time = task_info.__accept_tick.sec();

    if(TaskInfo::TASK_GTYPE_RING_TASK == task_info.__game_type) {
    	msg.task.ring_num = tasker_detail_->ring_num;
    }

	if(task_info.is_finish() && task_json.complete.is_empty) {
		msg.task.is_auto_submit = 1;
	}

	calc_remain_time_of_limit_time_task(msg.task, task_info, task_json);

    for (TaskInfo::TaskConditionList::iterator it_cond = task_info.__condition_list.begin();
    		it_cond != task_info.__condition_list.end(); ++it_cond) {
    	msg.task.value.push_back((*it_cond).__current_value);
    }

	THIS_SEND_TO_CLIENT(msg);

    //监听任务状态，如果满足任务状态,通知客户端开启图标
    if(TaskInfo::TASK_STATUS_ACCEPTED == msg.task.status
    		|| TaskInfo::TASK_STATUS_FINISH  == msg.task.status
    		|| TaskInfo::TASK_STATUS_SUBMIT == msg.task.status) {
    	player_self()->notify_open_icon_by_task_status();
    }

	if(TaskInfo::TASK_GTYPE_MAINLINE == task_info.__game_type) {
		player_self()->listen_task_status_change(task_info.__task_id, task_state(task_info.__task_id));
	}

    this->save_tick();

	return 0;
}

int Tasker::notify_task_del(TaskInfo &task_info)
{
	if (task_info.__task_id == 0) {
		return 0;
	}
	MSG_81110000 msg;
	msg.operate = 2;
	msg.task.id = task_info.__task_id;
	msg.task.type = task_info.__game_type;
	msg.task.status = task_state(task_info.__task_id);
	msg.task.time = task_info.__accept_tick.sec();

    if(TaskInfo::TASK_GTYPE_RING_TASK == task_info.__game_type) {
    	msg.task.ring_num = tasker_detail_->ring_num;
    }

    for (TaskInfo::TaskConditionList::iterator it_cond = task_info.__condition_list.begin();
    		it_cond != task_info.__condition_list.end(); ++it_cond) {
    	msg.task.value.push_back((*it_cond).__current_value);
    }

	THIS_SEND_TO_CLIENT(msg);

    //监听任务状态，如果满足任务状态,通知客户端开启图标
    if(TaskInfo::TASK_STATUS_ACCEPTED == msg.task.status
    		|| TaskInfo::TASK_STATUS_FINISH  == msg.task.status
    		|| TaskInfo::TASK_STATUS_SUBMIT == msg.task.status) {
    	player_self()->notify_open_icon_by_task_status();
    }

	if(TaskInfo::TASK_GTYPE_MAINLINE == task_info.__game_type) {
		player_self()->listen_task_status_change(task_info.__task_id, task_state(task_info.__task_id));
	}

    this->save_tick();
	return 0;
}

int Tasker::notify_task_update(TaskInfo &task_info, const Task_Mission_Detail_Config &task_json)
{
	if (task_info.__task_id == 0) {
		return 0;
	}
	MSG_81110000 msg;
	msg.operate = 3;
	msg.task.id = task_info.__task_id;
	msg.task.type = task_info.__game_type;
	msg.task.status = task_state(task_info.__task_id);
	msg.task.time = task_info.__accept_tick.sec();

    if(TaskInfo::TASK_GTYPE_RING_TASK == task_info.__game_type) {
    	msg.task.ring_num = tasker_detail_->ring_num;
    }

	if(task_info.is_finish() && task_json.complete.is_empty) {
		msg.task.is_auto_submit = 1;
	}

	calc_remain_time_of_limit_time_task(msg.task, task_info, task_json);

    for (TaskInfo::TaskConditionList::iterator it_cond = task_info.__condition_list.begin();
    		it_cond != task_info.__condition_list.end(); ++it_cond) {
    	msg.task.value.push_back((*it_cond).__current_value);
    }

	THIS_SEND_TO_CLIENT(msg);

    //监听任务状态，如果满足任务状态,通知客户端开启图标
    if(TaskInfo::TASK_STATUS_ACCEPTED == msg.task.status
    		|| TaskInfo::TASK_STATUS_FINISH  == msg.task.status
    		|| TaskInfo::TASK_STATUS_SUBMIT == msg.task.status) {
    	player_self()->notify_open_icon_by_task_status();
    }

	if(TaskInfo::TASK_GTYPE_MAINLINE == task_info.__game_type) {
		player_self()->listen_task_status_change(task_info.__task_id, task_state(task_info.__task_id));
	}

    this->save_tick();
	return 0;
}

/*int Tasker::notify_task_update_ready(TaskInfo &task_info, Block_Buffer &buf)
{
	if (task_info.__task_id == 0) {
		return -1;
	}
	MSG_81110000 res;
	res.operate = 3;
	res.task.id = task_info.__task_id;
	res.task.type = task_info.__game_type;
	res.task.status = task_info.task_status();
	res.task.time = task_info.__refresh_tick.sec();

    for (TaskInfo::TaskConditionList::iterator it_cond = task_info.__condition_list.begin();
    		it_cond != task_info.__condition_list.end(); ++it_cond) {
    	res.task.value.push_back((*it_cond).__current_value);
    }

    buf.reset();
    buf.make_message(res.msg_id);
    res.serialize(buf);
    buf.finish_message();
//	THIS_SEND_TO_CLIENT(res);
	return 0;
}*/

//用Scene_Tasker::load_detail解
int Tasker::serialize_task(Block_Buffer &buf)
{
    return 0;
}

int Tasker::unserialize_task(Block_Buffer &buf)
{
	return 0;
}

int Tasker::req_task_add_npc(int32_t npc_type, int8_t set, int64_t npc_id,
			std::string funcName, int32_t direction, std::string sceneId,
			std::string open, int32_t x, int32_t y, std::string npc_name) {
	if (set) {
		bool exist = false;
		MSG_81110001 msg;
		if (npc_type) {
			role_id_t id = this->tasker_detail_->npc_type_map_.size() + npc_type * 10 + 1;
			Task_Npc_Type task_npc_type;
			task_npc_type.npc_id = id;
			task_npc_type.npc_type = npc_type;
			task_npc_type.funcName = funcName;
			task_npc_type.direction = direction;
			task_npc_type.sceneId = sceneId;
			task_npc_type.open = open;
			task_npc_type.x = x;
			task_npc_type.y = y;
			task_npc_type.npc_name = npc_name;
			this->tasker_detail_->npc_type_map_.insert(Task_Npc_Type_Map::value_type(npc_type, task_npc_type));

			Location localtion = this->player_self()->location();
			NPC_Appear_Info_Task npc_info;
			if (x == 0 && y == 0) {
				npc_info.npc_info.coord = localtion.coord.pixel_coord();
			} else {
				npc_info.npc_info.coord.type = 1; // PIXEL_TYPE
				npc_info.npc_info.coord.x = x;
				npc_info.npc_info.coord.y = y;
			}
			npc_info.npc_info.hp_curr = 100;
			npc_info.npc_info.hp_max = 100;
			npc_info.npc_info.npc_type = npc_type;
			npc_info.npc_info.role_id = id;
			npc_info.npc_info.role_name = npc_name;
			npc_info.npc_info.speed = 180;
			npc_info.funcName = funcName;
			npc_info.direction = direction;
			npc_info.sceneId = sceneId;
			npc_info.open = open;
			msg.npc_info.push_back(npc_info);
			exist = true;
		}//end if: npc_type

		if (npc_id) {
			this->tasker_detail_->npc_id_.insert(Role_Id_Set::value_type(npc_id));
			msg.npc_ids.push_back(npc_id);
			exist = true;
		}

		if (exist) {
			THIS_SEND_TO_CLIENT(msg);
		}
	} // end if :set
	else {
		if (npc_type) {
			this->tasker_detail_->npc_type_map_.erase(npc_type);
		}

		if (npc_id) {
			this->tasker_detail_->npc_id_.erase(npc_id);
		}
	}
	this->save_tick();

	{
		MSG_50110011 res_msg;
		res_msg.npc_type = npc_type;
		res_msg.npc_id = npc_id;
		res_msg.set = set;
		res_msg.move_npc_id = npc_type * 10 + 1;
		res_msg.funcName = funcName;
		res_msg.direction = direction;
		res_msg.sceneId = sceneId;
		res_msg.open = open;
		THIS_SEND_TO_CLIENT(res_msg);
	}
	return 0;
}

int Tasker::task_state(uint32_t task_id) {
	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
	if(!task_cfg) {
		return TaskInfo::TASK_STATUS_NONE;
	}

	//悬赏任务特殊处理
	if(task_cfg->task_type == TaskInfo::TASK_GTYPE_REWARD_TASK) {
		RewardTaskMap::iterator it = tasker_detail_->reward_task_map.find(task_id);
		if(it == tasker_detail_->reward_task_map.end()) {
			return TaskInfo::TASK_STATUS_NONE;
		}

		if(it->second.is_submit())
			return TaskInfo::TASK_STATUS_SUBMIT;
		else if (it->second.is_finish())
	        return TaskInfo::TASK_STATUS_FINISH;
	    else if (it->second.is_accepted())
	        return TaskInfo::TASK_STATUS_ACCEPTED;
	    else if (it->second.is_acceptable())
	        return TaskInfo::TASK_STATUS_ACCEPTABLE;
	    else if (it->second.is_visible())
	        return TaskInfo::TASK_STATUS_VISIBLE;
	    else if (it->second.is_fail())
	    	return TaskInfo::TASK_STATUS_FAIL;
	    else
	        return TaskInfo::TASK_STATUS_NONE;
	}

	//其他任务
	if (this->tasker_detail_->task_submited_once_.count(task_id) > 0) {
		return TaskInfo::TASK_STATUS_SUBMIT;
	}

	TaskMap::iterator it = this->tasker_detail_->task_map_.find(task_id);
	if (it == this->tasker_detail_->task_map_.end()) {
		return TaskInfo::TASK_STATUS_NONE;
	}

    if (it->second.is_finish())
        return TaskInfo::TASK_STATUS_FINISH;
    else if (it->second.is_accepted())
        return TaskInfo::TASK_STATUS_ACCEPTED;
    else if (it->second.is_acceptable())
        return TaskInfo::TASK_STATUS_ACCEPTABLE;
    else if (it->second.is_visible())
        return TaskInfo::TASK_STATUS_VISIBLE;
    else if(it ->second.is_fail())
    	return TaskInfo::TASK_STATUS_FAIL;
    else if(it->second.is_visible())
    	return TaskInfo::TASK_STATUS_VISIBLE;
    else
        return TaskInfo::TASK_STATUS_NONE;
}

bool Tasker::task_execute_type_is_need_listen_item(int execute_type) {
	return (TaskExecType::TASK_EXEC_COLLECT == execute_type)
			|| (TaskExecType::TASK_EXEC_PACKAGE == execute_type)
			|| (TaskExecType::TASK_EXEC_COMMIT_CONSUME_ITEM == execute_type)
			|| (TaskExecType::TASK_EXEC_DELIVER_LETTER == execute_type);
}

void Tasker::sync_scene_special_process_first_task() {
	for(TaskMap::iterator it = tasker_detail_->task_map_.begin();
			it != tasker_detail_->task_map_.end(); ++it) {
		if(is_first_main_line_task(it->second) && it->second.is_accepted()) {
			//处理第一个任务回满怒气
			process_full_prop_when_task_accepted(it->second.__task_id);

			//处理第一个任务添加/删除英雄
		    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(it->second.__task_id);
		    if (NULL == task_json) {
		    	break;
		    }

		    add_or_del_hero_when_task_accepted(it->second, task_json->hero_op, task_json->hero_id);

		    //激活坐骑
		    process_activate_horse(task_json->task_id);

	    	//添加buf
	    	process_add_buf(task_json->task_id);

	    	process_transfer_point(task_json->task_id);

			break;
		}
	}
}

void Tasker::process_full_prop_when_task_accepted(uint32_t task_id) {
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
    if (NULL == task_json) {
    	return;
    }

    if(task_json->accept.full_prop_num) {
    	player_self()->restore_full_prop(PT_MORALE_CURRENT, task_json->accept.full_prop_num);
    }
}

void Tasker::process_clear_full_prop_when_task_commit(uint32_t task_id) {
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
    if (NULL == task_json) {
    	return;
    }

    if(task_json->accept.full_prop_num) {
    	player_self()->restore_full_prop(PT_MORALE_CURRENT, -1);
    }
}

void Tasker::add_limit_time_task_to_listen_set(TaskInfo *task_info) {
	if(!task_info) {
		return;
	}

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    if (!task_json) {
    	MSG_USER("task not exits, task_id=%d\n", task_info->__task_id);
    	return;
    }

    //主线任务不能限时
    if(task_info->is_main_task()) {
    	return;
    }

    if(!task_json->is_limit_time || task_json->limit_time <= 0) {
    	return;
    }

    this->task_accepted_limit_time_set().insert(task_info->__task_id);
}

void Tasker::add_limit_time_task_to_listen_set(uint32_t task_id) {
	add_limit_time_task_to_listen_set(find_task(task_id));
}

void Tasker::task_listen_limit_time(const Time_Value &now) {
	for(TaskSet::iterator it = task_accepted_limit_time_set().begin();
			it != task_accepted_limit_time_set().end(); ++it) {
	    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(*it);
	    if (!task_json) {
	    	MSG_USER("task not exits, task_id=%d\n", *it);
	    	continue;
	    }

	    if(!task_json->is_limit_time || task_json->limit_time <= 0) {
	    	continue;
	    }

	    TaskInfo *task_info = find_task(*it);
	    if(!task_info) {
	    	continue;
	    }

	    if(this->task_state(task_info->__task_id) != TaskInfo::TASK_STATUS_ACCEPTED) {
	    	continue;
	    }

	    //限时任务超时未完成
	    if(now.sec() > (task_info->__accept_tick.sec() + task_json->limit_time * Time_Value::ONE_MINUTE_IN_SECS)) {
	    	MSG_81110003 msg;
	    	msg.task_id = *it;
	    	msg.error_code = ERROR_LIMIT_TASK_TIMEOUT;
	    	THIS_SEND_TO_CLIENT(msg);

	        //悬赏限时任务,未完成,更改状态为失败状态
	        if(task_info->is_reward_task()) {
	        	tasker_detail_->give_up_num += 1;
	        	set_reward_task_list_show_status(task_info->__task_id, 4);
	        	send_to_client_reward_task_list_show_status();
	        	notify_task_update(*task_info, *task_json);

	        	//限时悬赏任务失败，扣除领取时获得的道具
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
	        else if(task_info->is_ring_task()) {//环任务:未完成限时任务,发送任务失败状态，任务链断开
	        	set_task_status(*task_info, TaskInfo::TASK_STATUS_FAIL);
	        	notify_task_update(*task_info, *task_json);
	        }
	        else {//策划需求： 未完成限时任务，手动接取的，可以重新可接，而自动接取的，任务列表不显示
	    		uint32_t task_id = task_info->__task_id;

	    		notify_task_del(*task_info);
	    		unbind_task(task_info->__task_id);

	    		if(!task_json->accept.is_empty && task_json->accept.auto_accept == 2) {
	    			insert_task(task_id, *task_json);
	    		}
	    	}

	        save_tick();
	    }
	}
}

void Tasker::calc_remain_time_of_limit_time_task(Task_Info &msg_task_info,
    		const TaskInfo &svr_task_info, const Task_Mission_Detail_Config &task_json) {
	if(!task_json.is_limit_time || task_json.limit_time <= 0) {
		return;
	}

	msg_task_info.is_limit_time = 1;

	if(svr_task_info.is_accepted()) {
		Time_Value now = Time_Value::gettimeofday();
		long total_time = svr_task_info.__accept_tick.sec() + task_json.limit_time * Time_Value::ONE_MINUTE_IN_SECS;
		if(now.sec() >= total_time) {
			msg_task_info.second = 0;
		} else {
			msg_task_info.second = total_time - now.sec();
		}
	}

	if(svr_task_info.is_acceptable() || svr_task_info.is_visible()) {
		msg_task_info.second = task_json.limit_time * Time_Value::ONE_MINUTE_IN_SECS;
	}
}

void Tasker::add_or_del_hero_when_task_accepted(TaskInfo &task_info, uint8_t hero_op, uint32_t hero_id) {
	if(0 == hero_op || 0 == hero_id) {
		return;
	}

	if(1 == hero_op && 0 == task_info.__hero_id) {
			int ret = player_self()->hero_add(hero_id);
			if(0 == ret) {
				task_info.__hero_id = hero_id;
				this->save_tick();
				int index = player_self()->heroer_get_hero_index(hero_id);

				Heroer_Detail::Hero_Map::iterator hero_it = player_self()->heroer_detail_no_const().hero_map.find(index);
				if(hero_it != player_self()->heroer_detail_no_const().hero_map.end()) {
					hero_it->second.is_task_tmp_hero = true;
					player_self()->heroer_detail_no_const().detail_change();
				}

				player_self()->req_hero_set_fight(index, 1, false);
			}
		return;
	}

	if(2 == hero_op) {
		int index = player_self()->heroer_get_hero_index(hero_id);
		player_self()->hero_erase(index);
		return;
	}
}

bool Tasker::is_first_main_line_task(const TaskInfo &task_info) const {
    if(task_info.is_main_task() && task_info.__prev_task == 0
    		&& (task_info.__post_task != 0 || task_info.__post_task2 != 0 || task_info.__post_task3 != 0)) {
    	return true;
    }

    return false;
}

bool Tasker::is_has_accepted_or_finished_main_line_task() const  {
	const TaskMap &map = this->tasker_detail().task_map_;
	for(TaskMap::const_iterator it = map.begin(); it != map.end(); ++it) {
		if(it->second.is_main_task() && (it->second.is_acceptable() || it->second.is_accepted() || it->second.is_finish())) {
			return true;
		}
	}
	return false;
}

bool Tasker::is_has_visible_main_line_task() const {
	const TaskMap &map = this->tasker_detail().task_map_;
	for(TaskMap::const_iterator it = map.begin(); it != map.end(); ++it) {
		if(it->second.is_main_task() && it->second.is_visible()) {
			return true;
		}
	}
	return false;
}

int Tasker::get_visiable_main_line_task_id() const {
	const TaskMap &map = this->tasker_detail().task_map_;
	for(TaskMap::const_iterator it = map.begin(); it != map.end(); ++it) {
		if(it->second.is_main_task() && it->second.is_visible()) {
			return it->second.__task_id;
		}
	}
	return 0;
}

bool Tasker::is_last_main_line_task_submit() const {
	for(TaskIdSet::const_iterator it = tasker_detail_->task_submited_once_.begin();
			it != tasker_detail_->task_submited_once_.end();
			++it) {
		const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(*it);
		if(!task_cfg) {
			continue;
		}

		if(task_cfg->task_type == TaskInfo::TASK_GTYPE_MAINLINE
				&& task_cfg->precondition != 0 && task_cfg->postcondition == 0
				&& task_cfg->postcondition2 == 0 && task_cfg->postcondition3 == 0) {
			return true;
		}
	}

	return false;
}

int Tasker::req_open_reward_task_list() {
	if(player_self()->level() < CONFIG_CACHE_REWARD_TASK->get_config().open_level) {
		return ERROR_PLAYER_LEVEL_LOWER_OPEN_LEVEL;
	}

	MSG_50110013 msg;
	msg.status = 1;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Tasker::req_reward_task_list(uint8_t is_refresh) {
	if(player_self()->level() < CONFIG_CACHE_REWARD_TASK->get_config().open_level) {
		return ERROR_PLAYER_LEVEL_LOWER_OPEN_LEVEL;
	}

	if(1 == is_refresh) {
		int ret = process_free_refresh_and_money_refresh();
		if(0 != ret) {
			return ret;
		}

		if(tasker_detail_->refresh_reward_task_num <= 2) {
			tasker_detail_->refresh_reward_task_num += 1;
		}

		if(!tasker_detail_->is_ever_open_reward_task_list) {
			tasker_detail_->is_ever_open_reward_task_list = true;
		}

		return process_refresh_new_reward_task_list();
	}

	if(0 == is_refresh && tasker_detail_->reward_task_map.empty()) {
		if(!tasker_detail_->is_ever_open_reward_task_list) {
			tasker_detail_->is_ever_open_reward_task_list = true;
		}

		return process_refresh_new_reward_task_list();
	}

	if(!tasker_detail_->is_ever_open_reward_task_list) {
		tasker_detail_->is_ever_open_reward_task_list = true;
	}

	process_old_reward_task_list();
	return 0;
}

int Tasker::reward_task_construct(const Task_Mission_Detail_Config &cfg) {
	if(cfg.task_type != TaskInfo::TASK_GTYPE_REWARD_TASK) {
		return 0;
	}

	if(cfg.accept.level != 0 && player_self()->level() < cfg.accept.level) {
		return 0;
	}

	if(cfg.accept.level_max != 0 && player_self()->level() > cfg.accept.level_max) {
		return 0;
	}

	if(cfg.quality != TASK_QUALITY_WHITE && cfg.quality != TASK_QUALITY_GREEN
			&& cfg.quality != TASK_QUALITY_BLUE && cfg.quality != TASK_QUALITY_PURPLE
			&& cfg.quality != TASK_QUALITY_ORANGE) {
		return 0;
	}

	tasker_detail_->quality_RewardTaskIdSet_map[cfg.quality].insert(cfg.task_id);

	return 0;
}

void Tasker::send_to_client_reward_task_list_show_status() {
	Time_Value now = Time_Value::gettimeofday();
	MSG_81110007 msg;
	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	if(tasker_detail_->is_open_countdown) {
		msg.auto_refresh_time = cfg.auto_refresh * Time_Value::ONE_MINUTE_IN_SECS;
		//更改开始倒计时时间
		tasker_detail_->begin_countdown_time.set(now);
		this->save_tick();
	}

	msg.is_open_countdown = tasker_detail_->is_open_countdown ? 1 : 0;
	msg.free_refresh_count =
			(cfg.free_refresh_max_count_per_day > tasker_detail_->used_free_refresh_count)
			? (cfg.free_refresh_max_count_per_day - tasker_detail_->used_free_refresh_count) : 0;

	uint32_t max_accept_count = cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ tasker_detail_->give_up_num
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);


	msg.normal_accept_count = (max_accept_count > tasker_detail_->used_accept_count)
			? (max_accept_count - tasker_detail_->used_accept_count) : 0;

	copy_reward_task_list_to_msg(ACTIVE_REWARD_TASK_LIST, 0, &msg);

	THIS_SEND_TO_CLIENT(msg);
}

int Tasker::process_old_reward_task_list() {
	Time_Value now = Time_Value::gettimeofday();
	MSG_50110012 msg;
	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	if(tasker_detail_->is_open_countdown) {
		long total_time = tasker_detail_->begin_countdown_time.sec() +
				cfg.auto_refresh * Time_Value::ONE_MINUTE_IN_SECS;
		if(total_time > now.sec()) {
			msg.auto_refresh_time = total_time - now.sec();
		} else {
			msg.auto_refresh_time = 0;
		}
	}

	msg.is_open_countdown = tasker_detail_->is_open_countdown ? 1 : 0;
	msg.free_refresh_count =
			(cfg.free_refresh_max_count_per_day > tasker_detail_->used_free_refresh_count)
			? (cfg.free_refresh_max_count_per_day - tasker_detail_->used_free_refresh_count) : 0;

	uint32_t max_accept_count = cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ tasker_detail_->give_up_num
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);

	msg.normal_accept_count = (max_accept_count > tasker_detail_->used_accept_count)
			? (max_accept_count - tasker_detail_->used_accept_count) : 0;

	copy_reward_task_list_to_msg(RES_REWARD_TASK_LIST, &msg, 0);

	THIS_SEND_TO_CLIENT(msg);

	task_listen_item_update();

	return 0;
}

int Tasker::process_refresh_new_reward_task_list() {
	if(is_exist_accepted_reward_task()) {
		return ERROR_EXIST_ACCEPTED_REWARD_TASK;
	}

	Time_Value now = Time_Value::gettimeofday();
	MSG_50110012 msg;

	//第一次打开悬赏榜、倒计时自动刷新、免费刷新、立即刷新开启倒计时, 更改开始倒计时时间
	if(!tasker_detail_->is_close_auto_refresh) {
		tasker_detail_->is_open_countdown = true;
		tasker_detail_->begin_countdown_time.set(now);
	}

	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	if(tasker_detail_->is_open_countdown) {
		msg.auto_refresh_time = cfg.auto_refresh * Time_Value::ONE_MINUTE_IN_SECS;
	}

	msg.is_open_countdown = tasker_detail_->is_open_countdown ? 1 : 0;
	msg.free_refresh_count =
			(cfg.free_refresh_max_count_per_day > tasker_detail_->used_free_refresh_count)
			? (cfg.free_refresh_max_count_per_day - tasker_detail_->used_free_refresh_count) : 0;

	uint32_t max_accept_count = cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ tasker_detail_->give_up_num
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);

	msg.normal_accept_count = (max_accept_count > tasker_detail_->used_accept_count)
			? (max_accept_count - tasker_detail_->used_accept_count) : 0;

	build_new_reward_task_list_of_server(true);

	if(1 == tasker_detail_->refresh_reward_task_num) {
		first_copy_reward_task_list_to_msg(RES_REWARD_TASK_LIST, &msg, 0);
	}
	else {
		copy_reward_task_list_to_msg(RES_REWARD_TASK_LIST, &msg, 0);
	}

	THIS_SEND_TO_CLIENT(msg);

	this->save_tick();

	return 0;
}

int Tasker::init_reward_task(uint32_t task_id, const Task_Mission_Detail_Config &task_cfg) {
	TaskInfo task_info;
    int status = TaskInfo::TASK_STATUS_VISIBLE;
    if (task_cfg.accept.level <= player_self()->level())
    {
        status = TaskInfo::TASK_STATUS_ACCEPTABLE;
    }

    task_info.__task_id = task_id;
    task_info.__game_type = task_cfg.task_type;
	task_info.__recruid_hero = task_cfg.recruid_hero;
	set_task_status(task_info, status);

    this->init_task_condition(task_info, task_cfg);

    if (this->tasker_detail_->reward_task_map.insert(std::make_pair(task_id, task_info)).second == false)
    {
        return -1;
    }

    this->save_tick();

    return 0;
}

TaskInfo* Tasker::find_reward_task(uint32_t task_id) {
	RewardTaskMap::iterator it = tasker_detail_->reward_task_map.find(task_id);
	if(it == tasker_detail_->reward_task_map.end()) {
		return 0;
	}

	return &(it->second);
}

void Tasker::set_reward_task_list_show_status(uint32_t task_id, uint8_t op) {
	//重新设置悬赏任务状态
	for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
			it != tasker_detail_->reward_task_map.end(); ++it) {
		switch(op) {
			case 1:
			{
				if(task_id == it->first) {
					set_task_status(it->second, TaskInfo::TASK_STATUS_ACCEPTED);
					continue;
				}
				if(it->second.is_acceptable()) {
					set_task_status(it->second, TaskInfo::TASK_STATUS_VISIBLE);
				}
				break;
			}
			case 2:
			{
				if(task_id == it->first) {
					set_task_status(it->second, TaskInfo::TASK_STATUS_ACCEPTABLE);

					//重置进度为0
					for (TaskInfo::TaskConditionList::iterator it_cond = it->second.__condition_list.begin();
							it_cond != it->second.__condition_list.end(); ++it_cond) {
						it_cond->__current_value = 0;
					}

					continue;
				}
				if(it->second.is_visible()) {
					set_task_status(it->second, TaskInfo::TASK_STATUS_ACCEPTABLE);
				}
				break;
			}
			case 3:
			{
				if(task_id == it->first) {
					set_task_status(it->second, TaskInfo::TASK_STATUS_SUBMIT);
					continue;
				}
				if(it->second.is_visible()) {
					set_task_status(it->second, TaskInfo::TASK_STATUS_ACCEPTABLE);
				}
				break;
			}
			case 4:
			{
				if(task_id == it->first) {
					set_task_status(it->second, TaskInfo::TASK_STATUS_FAIL);
					continue;
				}
				if(it->second.is_visible()) {
					set_task_status(it->second, TaskInfo::TASK_STATUS_ACCEPTABLE);
				}
				break;
			}
		}
	}

	//设置倒计时开关
	if(!tasker_detail_->is_close_auto_refresh) {
		switch(op) {
			case 1:
				tasker_detail_->is_open_countdown = false;
				break;
			case 2:
				tasker_detail_->is_open_countdown = true;
				break;
			case 3:
				tasker_detail_->is_open_countdown = true;
				break;
			case 4:
				tasker_detail_->is_open_countdown = true;
				break;
		}
	}

	this->save_tick();
}

void Tasker::listen_auto_refresh_reward_task_list(const Time_Value &now) {
	if(!tasker_detail_->is_ever_open_reward_task_list) {
		return;
	}

	//自动刷新倒计时关闭状态
	if(!tasker_detail_->is_open_countdown) {
		return;
	}

	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	long total = tasker_detail_->begin_countdown_time.sec() + cfg.auto_refresh * Time_Value::ONE_MINUTE_IN_SECS;
	if(total > now.sec()) {
		return;
	}

	process_refresh_new_reward_task_list();
}

int Tasker::process_free_refresh_and_money_refresh() {
	if(is_exist_accepted_reward_task()) {
		return ERROR_EXIST_ACCEPTED_REWARD_TASK;
	}

	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	if(tasker_detail_->used_free_refresh_count < cfg.free_refresh_max_count_per_day) {
		tasker_detail_->used_free_refresh_count += 1;
		return 0;
	}

	//判断是否有足够的货币
	int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(cfg.money_type),
			cfg.money_num, Money_DM_Info(MONEY_SUB_MONEY_REFRESH_REWARD_TASK_LIST)));
	if(return_status != 0) {
		switch(cfg.money_type) {
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
	if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(cfg.money_type),
			cfg.money_num, Money_DM_Info(MONEY_SUB_FAST_TASK_CONSUME))) != 0) {
		return ERROR_SUB_MONEY_FAIL;
	}

	return 0;
}

int Tasker::process_task_get_list(MSG_50110000 &msg, Task_Info &msg_task_info, const TaskInfo &svr_task_info,
								  TaskIdSet &award_set) {
    msg_task_info.type = svr_task_info.__game_type;
    msg_task_info.id = (int32_t)svr_task_info.__task_id;
    msg_task_info.status = task_state(svr_task_info.__task_id);
    msg_task_info.time = svr_task_info.__accept_tick.sec();

    if(svr_task_info.__game_type == TaskInfo::TASK_GTYPE_RING_TASK) {
    	msg_task_info.ring_num = tasker_detail_->ring_num;
    }

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(svr_task_info.__task_id);
    if (!task_json) {
    	MSG_USER("task not exits, task_id=%d\n", svr_task_info.__task_id);
    	return ERROR_TASK_NOT_EXIST;
    }

    if(svr_task_info.is_finish() && task_json->complete.is_empty && !svr_task_info.is_main_task()) {
    	if(svr_task_info.__game_type != TaskInfo::TASK_GTYPE_DAY ||
    			(svr_task_info.__game_type == TaskInfo::TASK_GTYPE_DAY && task_json->is_show_in_task_list)) {
    		award_set.insert(svr_task_info.__task_id);
    	}
    }

	if(svr_task_info.is_finish() && task_json->complete.is_empty) {
		msg_task_info.is_auto_submit = 1;
	}

    calc_remain_time_of_limit_time_task(msg_task_info, svr_task_info, *task_json);

    for (TaskInfo::TaskConditionList::const_iterator it_cond = svr_task_info.__condition_list.begin();
    		it_cond != svr_task_info.__condition_list.end(); ++it_cond) {
    	msg_task_info.value.push_back((*it_cond).__current_value);
    }

	if(svr_task_info.__game_type != TaskInfo::TASK_GTYPE_DAY ||
			(svr_task_info.__game_type == TaskInfo::TASK_GTYPE_DAY && task_json->is_show_in_task_list)) {
		msg.tasks.push_back(msg_task_info);
	}

	return 0;
}

int Tasker::process_get_day_task_list(MSG_50110018 &msg, Task_Info &msg_task_info, const TaskInfo &svr_task_info,
		   TaskIdSet &award_set) {
    msg_task_info.type = svr_task_info.__game_type;
    msg_task_info.id = (int32_t)svr_task_info.__task_id;
    msg_task_info.status = task_state(svr_task_info.__task_id);
    msg_task_info.time = svr_task_info.__accept_tick.sec();

    if(svr_task_info.__game_type == TaskInfo::TASK_GTYPE_RING_TASK) {
    	msg_task_info.ring_num = tasker_detail_->ring_num;
    }

    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(svr_task_info.__task_id);
    if (!task_json) {
    	MSG_USER("task not exits, task_id=%d\n", svr_task_info.__task_id);
    	return ERROR_TASK_NOT_EXIST;
    }

    if(svr_task_info.is_finish() && task_json->complete.is_empty && !svr_task_info.is_main_task()) {
    	award_set.insert(svr_task_info.__task_id);
    }

	if(svr_task_info.is_finish() && task_json->complete.is_empty) {
		msg_task_info.is_auto_submit = 1;
	}

    calc_remain_time_of_limit_time_task(msg_task_info, svr_task_info, *task_json);

    for (TaskInfo::TaskConditionList::const_iterator it_cond = svr_task_info.__condition_list.begin();
    		it_cond != svr_task_info.__condition_list.end(); ++it_cond) {
    	msg_task_info.value.push_back((*it_cond).__current_value);
    }

    msg.day_task_list.push_back(msg_task_info);
	return 0;
}

void Tasker::model_init_one_task(TaskInfo &task_info) {
	if (task_info.is_accepted())
	{
		if (task_info.is_logic_kill_monster()) {
			this->task_accepted_monster_set().insert(task_info.__task_id);
		}

		if (task_info.is_logic_fb()) {
			this->task_accepted_fb_set().insert(task_info.__task_id);
		}

		if (task_info.is_logic_npc_talk()) {
			this->task_accepted_npc_talk_set().insert(task_info.__task_id);
		}

		if (task_info.is_logic_arrive()) {
			this->task_accepted_arrive_set().insert(task_info.__task_id);
		}

		if (task_info.is_logic_level_up()) {
			this->task_accepted_lvl_set().insert(task_info.__task_id);
		}

		if (task_info.is_logic_collect()) {
			this->task_accepted_collect_set().insert(task_info.__task_id);
			this->init_task_listen_item(&task_info);
		}

		if (task_info.is_logic_kill_monster_group()) {
			this->task_accepted_monster_group_set().insert(task_info.__task_id);
		}

		if (task_info.is_logic_package()) {
			this->task_accepted_package_set().insert(task_info.__task_id);
			this->init_task_listen_item(&task_info);
		}

		if (task_info.is_logic_fb_type()) {
			this->task_accepted_fb_type_set().insert(task_info.__task_id);
		}

		if (task_info.is_logic_win_player()) {
			this->task_accepted_win_player_set().insert(task_info.__task_id);
		}

		if(task_info.is_logic_join_gang()) {
			this->task_accepted_join_gang_set().insert(task_info.__task_id);
		}

		if(task_info.is_logic_ganger_level()) {
			this->task_accepted_ganger_level_set().insert(task_info.__task_id);
		}

		if(task_info.is_logic_player_all_equip()) {
			this->task_accepted_player_all_equip_set().insert(task_info.__task_id);
		}

	    if(task_info.is_logic_body_equip()) {
	    	this->task_acccepted_body_equip_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_body_equip_level()) {
	    	this->task_accepted_body_equip_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_equip_level()) {
	    	this->task_accepted_equip_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_stone_level()) {
	    	this->task_accepted_stone_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_buy_goods()) {
	    	this->task_accepted_buy_goods_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_get_hero()) {
	    	this->task_accepted_get_hero_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_fight()) {
	    	this->task_accepted_fight_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_commit_consume_item()) {
	    	this->task_accepted_commit_consume_item_set().insert(task_info.__task_id);
	    	this->init_task_listen_item(&task_info);
	    }

	    if(task_info.is_logic_deliver_letter()) {
	    	this->task_accepted_deliver_letter_set().insert(task_info.__task_id);
	    	this->init_task_listen_item(&task_info);
	    }

	    if(task_info.is_logic_help_other_complete_team_scene()) {
	    	this->task_accepted_help_other_complete_team_scene_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_sell_item()) {
	    	this->task_accepted_sell_item_set().insert(task_info.__task_id);
	    }

/*		    if(task_info.is_logic_use_skill()) {
	    	this->task_accepted_use_skill_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_append_skill()) {
	    	this->task_accepted_append_skill_set().insert(task_info.__task_id);
	    }*/

	    if(task_info.is_logic_arena()) {
	    	this->task_accepted_arena_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_chat_with_friend()) {
	    	this->task_accepted_chat_with_friend_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_send_mail()) {
	    	this->task_accepted_send_mail_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_team_scene_or_pvp()) {
	    	this->task_accepted_team_scene_or_pvp_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_browse_other_player_info()) {
	    	this->task_accepted_browse_other_player_info_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_arena_level()) {
	    	this->task_accepted_arena_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_equip_crimson_level()) {
		   this->task_accepted_equip_crimson_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_arena_all()) {
	    	this->task_accepted_arena_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_create_dragon_vale_building()) {
	    	this->task_accepted_create_dragon_vale_building_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_gain_dragon_vale_builiding_production()) {
	    	this->task_accepted_gain_dragon_vale_builiding_production_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_wake_up_hero_to_quality_level()) {
	    	this->task_accepted_wake_up_hero_to_quality_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_keep_hero_to_level()) {
	    	this->task_accepted_keep_hero_to_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_compose_equip()) {
	    	this->task_accepted_compose_equip_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_hero_dream_scene()) {
	    	this->task_accepted_hero_dream_scene_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_ganger_skill_level()) {
	    	this->task_accepted_ganger_skill_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_war()) {
	    	this->task_accepted_war_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_war_all()) {
	    	this->task_accepted_war_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_kill_enemy_in_war()) {
	    	this->task_accepted_kill_enemy_in_war_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_single_arena_all()) {
	    	this->task_accepted_single_arena_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_single_arena_win()) {
	    	this->task_accepted_single_arena_win_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_team_arena_all()) {
	    	this->task_accepted_team_arena_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_team_arena_win()) {
	    	this->task_accepted_team_arena_win_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_ganger_bless_buf()) {
	    	this->task_accepted_ganger_bless_buf_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_rob_other_dragon_vale_all()) {
	    	this->task_accepted_rob_other_dragon_vale_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_loigc_dragon_vale_step_foot_all()) {
	    	this->task_accepted_dragon_vale_step_foot_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_activeness_of_today()) {
	    	this->task_accepted_activeness_of_today_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_dragon_vale_building_upgrade()) {
	    	this->task_accepted_dragon_vale_building_upgrade_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_limit_time_kill_boss()) {
	    	this->task_accepted_limit_time_kill_boss_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_dragon_keep_all()) {
	    	this->task_accepted_dragon_keep_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_dragon_keep_success()) {
	    	this->task_accepted_dragon_keep_success_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_donate_to_gang()) {
	    	this->task_accepted_donate_to_gang_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_present_buf_to_other()) {
	    	this->task_accepted_present_buf_to_other_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_finish_reward_task_num()) {
	    	task_accepted_finish_reward_task_num_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_keep_dragon_to_level()) {
	    	task_accepted_keep_dragon_to_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_knight_trial()) {
	    	task_accepted_knight_trial_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_suit()) {
	    	task_accepted_suit_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_role_pull_on_equip()) {
	    	task_accepted_role_pull_on_equip_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_hero_pull_on_equip()) {
	    	task_accepted_hero_pull_on_equip_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_activate_wing()) {
			task_accepted_activate_wing_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_get_quality_rune()) {
	    	task_accepted_get_quality_rune_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_pass_kngiht_trials()) {
	    	task_accepted_pass_kngiht_trials_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_have_hero()) {
	    	task_accepted_have_hero_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_have_hero_quality()) {
	    	task_accepted_have_hero_quality_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_first_pass_scene()) {
	    	task_accepted_first_pass_scene_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_dragon_born()) {
	    	task_accepted_dragon_born_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_horse_star_level()) {
	    	task_accepted_horse_star_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_have_item_but_no_erase()) {
	    	task_accepted_have_item_but_no_erase_set().insert(task_info.__task_id);
	    	this->init_task_listen_item(&task_info);
	    }

	    if(task_info.is_logic_have_equip_but_no_erase()) {
	    	task_accepted_have_equip_but_no_erase_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_same_gang_challenge_all()) {
	    	task_accepted_same_gang_challenge_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_main_scene_npc_battle()) {
	    	task_accepted_main_scene_npc_battle_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_equip_card_all()) {
	    	task_accepted_equip_card_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_equip_card_by_position()) {
	    	task_accepted_equip_card_by_position_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_card_upgrade_to_level_all()) {
	    	task_accepted_card_upgrade_to_level_all_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_card_upgrade_to_level_by_position()) {
	    	task_accepted_card_upgrade_to_level_by_position_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_lottery_draw()) {
	    	task_accepted_lottery_draw_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_equip_jewelry()) {
	    	task_accepted_equip_jewelry_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_role_equip_4()) {
	    	task_accepted_role_equip_n_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_hero_equip_4()) {
	    	task_accepted_hero_equip_n_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_invite_friend_step_foot()) {
	    	task_accepted_invite_friend_step_foot_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_kill_group_num()) {
	    	task_accepted_kill_group_num_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_did_reward_task_num()) {
	    	task_accepted_did_reward_task_num_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_alchemy_num()) {
	    	task_accepted_alchemy_num_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_did_reward_task_num_today()) {
	    	task_accepted_did_reward_task_num_today_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_remote_scene_any_pass()) {
	    	task_accepted_remote_scene_any_pass_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_knight_trial_any_pass()) {
	    	task_accepted_knight_trial_any_pass_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_compose_gemstone_level()) {
	    	task_accepted_compose_gemstone_level_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_inlay_position_gemstone()) {
	    	task_accepted_inlay_position_gemstone_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_inlay_any_gemstone()) {
	    	task_accepted_inlay_any_gemstone_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_world_boss()) {
	    	task_accepted_world_boss_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_world_boss_immediate_revive()) {
	    	task_accepted_world_boss_immediate_revive_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_enchant_any_equip()) {
	    	task_accepted_enchant_any_equip_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_inlay_id_gemstone()) {
	    	task_accepted_inlay_id_gemstone_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_gang_well_recover_spirit()) {
	    	task_accepted_gang_well_recover_spirit_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_equip_purple_ring()) {
	    	task_accepted_equip_purple_ring_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_inlay_level_gemstone()) {
	    	task_accepted_inlay_level_gemstone_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_upgrade_card()) {
	    	task_accepted_upgrade_card_set().insert(task_info.__task_id);
	    }

	    //添加限时任务到监听任务集
	    add_limit_time_task_to_listen_set(task_info.__task_id);


	} else if (task_info.is_finish()) {
		if (task_info.is_logic_collect()) {
			this->task_accepted_collect_set().insert(task_info.__task_id);
		}

		if (task_info.is_logic_package()) {
			this->task_accepted_package_set().insert(task_info.__task_id);
		}

		if(task_info.is_logic_commit_consume_item()) {
			this->task_accepted_commit_consume_item_set().insert(task_info.__task_id);
		}

		if(task_info.is_logic_deliver_letter()) {
			this->task_accepted_deliver_letter_set().insert(task_info.__task_id);
		}

	    if(task_info.is_logic_have_item_but_no_erase()) {
	    	task_accepted_have_item_but_no_erase_set().insert(task_info.__task_id);
	    }

	    if(task_info.is_logic_have_equip_but_no_erase()) {
	    	task_accepted_have_equip_but_no_erase_set().insert(task_info.__task_id);
	    }
	}
}

int Tasker::process_receive_normal_task_award(const Task_Mission_Detail_Config &cfg) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
    if (!cfg.award.item_list.empty())
    {
    	for (Task_Mission_Award_List_Cfg::const_iterator it = cfg.award.item_list.begin();
    			it != cfg.award.item_list.end(); ++it) {
    		if((it->type == 2 || it->type == 1) && it->job != 0 && player_self()->career() != it->job) {
    			continue;
    		}

			if ((*it).val4 > 0 && (*it).val4 < 100) {
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				if (random() % 100 < (*it).val4) {
					continue;
				}
			}

			Item_Vec item_list;
			make_item_vec(item_list, (*it).val1, (*it).val2, static_cast<Item_Detail::Bind_Type>((*it).val3));
//			Item_Detail item((*it).val1, (*it).val2, static_cast<Item_Detail::Bind_Type>((*it).val3));
			player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list,
					Gain_Item_DM_Info(Pack::ITEM_GAIN_TASK));
			for (Item_Vec::iterator item_it = item_list.begin(); item_it != item_list.end(); ++item_it) {
				Item_Basic_Info item_basic;
				(*item_it).set_item_basic_info(item_basic);
				acv_msg.item_info.push_back(item_basic);
				Card_Info card_info;
				if ((*item_it).set_item_card_info(card_info)) {
					acv_msg.card_info_vec.push_back(card_info);
				}
			}
    	}
    }//end if : item_list

    if (!cfg.award.other_list.empty())
    {
		Money_Add_List money_add_list;
    	for (Task_Mission_Award_List_Cfg::const_iterator it = cfg.award.other_list.begin();
    			it != cfg.award.other_list.end(); ++it) {
			if ((*it).val4 > 0 && (*it).val4 < 100) {
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				if (random() % 100 < (*it).val4) {
					continue;
				}
			}

			/*1，道具
			2，装备
			3，经验
			4，金币
			5，英灵
			6，声望
			7，龙魂
			8，钻石
			9，绑定钻石
			10，特殊物品
			11，hero*/
    		switch(it->type) {
    		case 3: {
    			if (it->val2 > 0) {
    				int exp =  (int)(it->val2 * player_self()->get_world_level_add_percent());
    				acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp));
    				acv_msg.world_level = player_self()->get_world_level_add();
    				player_self()->modify_experience(exp, O_ADD);
    			}

    			break;
    		}
    		case 4: {
    			if (it->val2 > 0) {
    				acv_msg.property.push_back(Property(COPPER, it->val2));
    				money_add_list.push_back(
    					Money_Add_Info(Pack::COPPER, it->val2, Money_DM_Info(Pack::MONEY_ADD_TASK_AWARD)));
    			}
    			break;
    		}
    		case 5: {
    			if (it->val2 > 0) {
    				acv_msg.property.push_back(Property(SOULS, it->val2));
    				money_add_list.push_back(
    					Money_Add_Info(Pack::SOULS, it->val2, Money_DM_Info(Pack::MONEY_ADD_TASK_AWARD)));
    			}
    			break;
    		}
    		case 6:
    			break;
    		case 7:
    			break;
    		case 8: {
    			if (it->val2 > 0) {
    				acv_msg.property.push_back(Property(GOLD, it->val2));
    				money_add_list.push_back(
    					Money_Add_Info(Pack::GOLD, it->val2, Money_DM_Info(Pack::MONEY_ADD_TASK_AWARD)));
    			}
    			break;
    		}
    		case 9: {
    			if (it->val2 > 0) {
    				acv_msg.property.push_back(Property(BIND_GOLD, it->val2));
    				money_add_list.push_back(
    					Money_Add_Info(Pack::BIND_GOLD, it->val2, Money_DM_Info(Pack::MONEY_ADD_TASK_AWARD)));
    			}
    			break;
    		}
    		case 10:
    			break;
    		case 11: {
    			player_self()->hero_add(it->val1);
    			if (it->val4) {
    				// 英雄跟随调整
    				//int index = player_self()->heroer_get_hero_index(it->val1);
    				//player_self()->req_hero_out(index, it->val4);
    				player_self()->heroer_get_hero_index(it->val1);
    			}
    			break;
    		}
    		case 12: {
    			if(it->val2 > 0) {
    				player_self()->profile_num_fb_pre_add(it->val2);
    			}
    			break;
    		}
    		case 13: {
    			if(it->val2 > 0) {
    				player_self()->add_contribute(it->val2);
    			}
    			break;
    		}
    		case 15: {
    			if(it->val2 > 0) {
    				player_self()->task_add_elite_challenge_num(it->val2);
    			}
    			break;
    		}
    		case 16: {
    			if(it->val2 > 0) {
    				player_self()->restor_phy_power(it->val2);
    			}
    			break;
    		}
    		default:
    			break;
    		}

		}//end for

    	if (!money_add_list.empty()) {
    		player_self()->pack_add_money(money_add_list, Pack::MONEY_OPT_REAL);
    	}
    }// end if : other_list

    if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
    	THIS_SEND_TO_CLIENT(acv_msg);
    }
	return 0;
}

int Tasker::process_receive_reward_task_award(const TaskInfo &task_info, const Task_Mission_Detail_Config &cfg) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	const Reward_Task_Config &reward_task_cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	for(White_Reward_List::const_iterator it = reward_task_cfg.white_reward_list.begin();
			it != reward_task_cfg.white_reward_list.end(); ++it) {
		if(player_self()->level() == it->accept_player_level) {
			uint32_t exp_award = it->exp;
			uint32_t coin_award = it->coin;
			switch (cfg.quality){
				case TASK_QUALITY_WHITE:
					exp_award *= 1;
					coin_award *= 1;
					break;
				case TASK_QUALITY_GREEN:
					exp_award *= reward_task_cfg.green;
					coin_award *= reward_task_cfg.green;
					break;
				case TASK_QUALITY_BLUE:
					exp_award *= reward_task_cfg.blue;
					coin_award *= reward_task_cfg.blue;
					break;
				case TASK_QUALITY_PURPLE:
					exp_award *= reward_task_cfg.purple;
					coin_award *= reward_task_cfg.purple;
					break;
				case TASK_QUALITY_ORANGE:
					exp_award *= reward_task_cfg.orange;
					coin_award *= reward_task_cfg.orange;
					break;
				default:
					MSG_USER("task quality is error:task_id=%u\n", task_info.__task_id);
					return -1;
			}//end switch

			if (exp_award > 0) {
				exp_award = (uint32_t)(exp_award * player_self()->get_world_level_add_percent());
				acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_award));
				acv_msg.world_level = player_self()->get_world_level_add();
				player_self()->modify_experience(exp_award, O_ADD);
			}
			if (coin_award > 0) {
				acv_msg.property.push_back(Property(COPPER, coin_award));

				//vip监听悬赏任务奖励货币
				player_self()->listen_vip_reward(VIP_WORLD_TREE_REWARD_TASK, COPPER, coin_award);

				Money_Add_Info money_add_info(Pack::COPPER, coin_award, Money_DM_Info(Pack::MONEY_ADD_REWARD_TASK));
				player_self()->pack_add_money(money_add_info, Pack::MONEY_OPT_REAL);
			}

			if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
				THIS_SEND_TO_CLIENT(acv_msg);
			}

			//悬赏任务提交，通知奖励找回
			UInt_UInt_Map award_map;
			award_map.insert(std::make_pair(Pack::COPPER, coin_award));
			award_map.insert(std::make_pair(PT_EXP_CURRENT, exp_award));
			player_self()->reward_back_listen_fulfill(REWARD_BACK_PRO_REWARD_TASK, award_map);
			return 0;
		}//end if
	}//end for

	MSG_USER("no reward task reward: player_level=%d\n", player_self()->level());
	return -1;
}

int Tasker::process_receive_day_week_task_award(const TaskInfo &task_info) {
	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_info.__task_id);
	if(!task_cfg) {
		return -1;
	}

	const Day_Week_Task_Award_Cfg *cfg = 0;
	if(task_info.is_day_task()) {
		cfg = CONFIG_CACHE_TASK->find_day_award_cfg(player_self()->level());
	}
	if(task_info.is_week_task()) {
		cfg = CONFIG_CACHE_TASK->find_week_award_cfg(player_self()->level());
	}

	if(!cfg) {
		return -1;
	}

	Money_DM_Info money_dm_info(Pack::MONEY_ADD_DAY_TASK);
	if(task_info.is_day_task()) {
		money_dm_info.type = Pack::MONEY_ADD_DAY_TASK;
	}
	if(task_info.is_week_task()) {
		money_dm_info.type = Pack::MONEY_ADD_WEEK_TASK;
	}

	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if(cfg->exp > 0 && task_cfg->exp_times > 0) {
		uint32_t exp = (uint32_t)ceil(cfg->exp * task_cfg->exp_times * 1.0 / 100);
		exp = (uint32_t)(exp * player_self()->get_world_level_add_percent());
		if(exp > 0) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp));
			acv_msg.world_level = player_self()->get_world_level_add();
			player_self()->modify_experience(exp, O_ADD);
		}
	}

	if(cfg->coin > 0 && task_cfg->coin_times > 0) {
		uint32_t coin = (uint32_t)ceil(cfg->coin * task_cfg->coin_times * 1.0 / 100);
		if(coin > 0) {
			acv_msg.property.push_back(Property(COPPER, coin));
			Money_Add_Info money_add_info_coin(Pack::COPPER, coin, money_dm_info);
			player_self()->pack_add_money(money_add_info_coin, Pack::MONEY_OPT_REAL);
		}
	}

	if(cfg->dragon_soul > 0 && task_cfg->dragon_soul_times > 0) {
		uint32_t dragon_soul = (uint32_t)ceil(cfg->dragon_soul * task_cfg->dragon_soul_times * 1.0 / 100);
		if(dragon_soul > 0) {
			acv_msg.property.push_back(Property(DRAGON_SOUL, dragon_soul));
			Money_Add_Info money_add_info_dragon_soul(Pack::DRAGON_SOUL, dragon_soul, money_dm_info);
			player_self()->pack_add_money(money_add_info_dragon_soul, Pack::MONEY_OPT_REAL);
		}
	}

	if(cfg->soul > 0 && task_cfg->soul_times > 0) {
		uint32_t soul = (uint32_t)ceil(cfg->soul * task_cfg->soul_times * 1.0 / 100);
		if(soul > 0) {
			acv_msg.property.push_back(Property(SOULS, soul));
			Money_Add_Info money_add_info_soul(Pack::SOULS, soul, money_dm_info);
			player_self()->pack_add_money(money_add_info_soul, Pack::MONEY_OPT_REAL);
		}
	}

	Item_Vec item_list;
	if(cfg->item1_id >  0 && cfg->item1_num > 0 && task_cfg->item1_times > 0) {
		Item_Detail::Bind_Type type = Item_Detail::UNDEFINED;
		type = cfg->is_item1_bind ? Item_Detail::BIND : Item_Detail::UNBIND;
		make_item_vec(item_list, cfg->item1_id, cfg->item1_num *task_cfg->item1_times, type);
//		Item_Detail item(cfg->item1_id, cfg->item1_num * task_cfg->item1_times, type);
//		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
//				Gain_Item_DM_Info(Pack::ITEM_GAIN_TASK));
//		Item_Basic_Info item_basic;
//		item.set_item_basic_info(item_basic);
//		acv_msg.item_info.push_back(item_basic);
	}

	if(cfg->item2_id >  0 && cfg->item2_num > 0 && task_cfg->item2_times > 0) {
		Item_Detail::Bind_Type type = Item_Detail::UNDEFINED;
		type = cfg->is_item2_bind ? Item_Detail::BIND : Item_Detail::UNBIND;
		make_item_vec(item_list, cfg->item2_id, cfg->item2_num *task_cfg->item2_times, type);
//		Item_Detail item(cfg->item2_id, cfg->item2_num * task_cfg->item2_times, type);
//		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
//				Gain_Item_DM_Info(Pack::ITEM_GAIN_TASK));
//		Item_Basic_Info item_basic;
//		item.set_item_basic_info(item_basic);
//		acv_msg.item_info.push_back(item_basic);
	}

	if(cfg->item3_id > 0 && cfg->item3_num > 0 && task_cfg->item3_times > 0) {
		Item_Detail::Bind_Type type = Item_Detail::UNDEFINED;
		type = cfg->is_item3_bind ? Item_Detail::BIND : Item_Detail::UNBIND;
		make_item_vec(item_list, cfg->item3_id, cfg->item3_num *task_cfg->item3_times, type);
//		Item_Detail item(cfg->item3_id, cfg->item3_num * task_cfg->item3_times, type);
//		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
//				Gain_Item_DM_Info(Pack::ITEM_GAIN_TASK));
//		Item_Basic_Info item_basic;
//		item.set_item_basic_info(item_basic);
//		acv_msg.item_info.push_back(item_basic);
	}

	if(cfg->item4_id > 0 && cfg->item4_num > 0 && task_cfg->item4_times > 0) {
		Item_Detail::Bind_Type type = Item_Detail::UNDEFINED;
		type = cfg->is_item4_bind ? Item_Detail::BIND : Item_Detail::UNBIND;
		make_item_vec(item_list, cfg->item4_id, cfg->item4_num *task_cfg->item4_times, type);
//		Item_Detail item(cfg->item4_id, cfg->item4_num * task_cfg->item4_times, type);
//		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
//				Gain_Item_DM_Info(Pack::ITEM_GAIN_TASK));
//		Item_Basic_Info item_basic;
//		item.set_item_basic_info(item_basic);
//		acv_msg.item_info.push_back(item_basic);
	}

	player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_TASK));

	Item_Basic_Info item_basic;
	Card_Info card_info;
	for (Item_Vec::iterator item_it = item_list.begin(); item_it != item_list.end(); ++item_it) {
		item_basic.reset();
		card_info.reset();
		(*item_it).set_item_basic_info(item_basic);
		acv_msg.item_info.push_back(item_basic);
		if ((*item_it).set_item_card_info(card_info)) {
			acv_msg.card_info_vec.push_back(card_info);
		}
	}

	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	return 0;
}

int Tasker::process_receive_ring_task_award(const uint32_t task_id) {
	const Task_Mission_Detail_Config *task_detail_cfg = CONFIG_CACHE_TASK->find_task((int)task_id);
	if(!task_detail_cfg) {
		return 0;
	}

	if(task_detail_cfg->task_type != TaskInfo::TASK_GTYPE_RING_TASK) {
		return 0;
	}

	const Ring_Task_All_Cfg &ring_task_all_cfg = CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	Ring_Task_Award_Cfg_Map::const_iterator ring_award_it =
			ring_task_all_cfg.ring_task_award_cfg_map.find(tasker_detail_->ring_num);
	if(ring_award_it == ring_task_all_cfg.ring_task_award_cfg_map.end()) {
		MSG_USER("ring_award cfg not exist, ring_num=%d", (int)tasker_detail_->ring_num);
		return 0;
	}

	Player_Level_Award_Cfg_Map::const_iterator player_level_it
		= ring_task_all_cfg.player_level_award_cfg_map.find((uint32_t)player_self()->level());
	if(player_level_it == ring_task_all_cfg.player_level_award_cfg_map.end()) {
		MSG_USER("player_level_award not exist, player_level=%d", player_self()->level());
		return 0;
	}

	Money_DM_Info money_dm_info(Pack::MONEY_ADD_RING_TASK);

	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if(ring_award_it->second.exp > 0 && player_level_it->second.exp_percent > 0) {
		uint32_t exp = (uint32_t)(ring_award_it->second.exp * player_level_it->second.exp_percent * 1.0 / 100);
		exp = (uint32_t)(exp * player_self()->get_world_level_add_percent());
		if(exp > 0) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp));
			acv_msg.world_level = player_self()->get_world_level_add();
			player_self()->modify_experience(exp, O_ADD);
		}
	}

	if(ring_award_it->second.coin > 0 && player_level_it->second.coin_percent > 0) {
		uint32_t coin = (uint32_t)ceil(ring_award_it->second.coin * player_level_it->second.coin_percent * 1.0 / 100);
		if(coin > 0) {
			acv_msg.property.push_back(Property(COPPER, coin));
			Money_Add_Info money_add_info_coin(Pack::COPPER, coin, money_dm_info);
			player_self()->pack_add_money(money_add_info_coin, Pack::MONEY_OPT_REAL);
			player_self()->listen_vip_reward(VIP_WORLD_TREE_RING_TASK, COPPER, coin);
		}
	}

	if(ring_award_it->second.soul > 0 && player_level_it->second.soul_percent > 0) {
		uint32_t soul = (uint32_t)ceil(ring_award_it->second.soul * player_level_it->second.soul_percent * 1.0 / 100);
		if(soul > 0) {
			acv_msg.property.push_back(Property(SOULS, soul));
			Money_Add_Info money_add_info_dragon_soul(Pack::SOULS, soul, money_dm_info);
			player_self()->pack_add_money(money_add_info_dragon_soul, Pack::MONEY_OPT_REAL);
			player_self()->listen_vip_reward(VIP_WORLD_TREE_RING_TASK, SOULS, soul);
		}
	}

	Item_Vec item_list;
	if(ring_award_it->second.item1_id >  0 && ring_award_it->second.item1_num > 0 && player_level_it->second.item1_percent > 0) {
		Item_Detail::Bind_Type type = Item_Detail::UNDEFINED;
		type = ring_award_it->second.is_item1_bind ? Item_Detail::BIND : Item_Detail::UNBIND;
		make_item_vec(item_list, ring_award_it->second.item1_id,
				ring_award_it->second.item1_num * player_level_it->second.item1_percent, type);
//		Item_Detail item(ring_award_it->second.item1_id, ring_award_it->second.item1_num * player_level_it->second.item1_percent, type);
//		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
//				Gain_Item_DM_Info(Pack::ITEM_GAIN_RING_TASK));
//		Item_Basic_Info item_basic;
//		item.set_item_basic_info(item_basic);
//		acv_msg.item_info.push_back(item_basic);
	}

	if(ring_award_it->second.item2_id >  0 && ring_award_it->second.item2_num > 0 && player_level_it->second.item2_percent > 0) {
		Item_Detail::Bind_Type type = Item_Detail::UNDEFINED;
		type = ring_award_it->second.is_item2_bind ? Item_Detail::BIND : Item_Detail::UNBIND;
		make_item_vec(item_list, ring_award_it->second.item2_id,
						ring_award_it->second.item2_num * player_level_it->second.item2_percent, type);
//		Item_Detail item(ring_award_it->second.item2_id, ring_award_it->second.item2_num * player_level_it->second.item2_percent, type);
//		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
//				Gain_Item_DM_Info(Pack::ITEM_GAIN_RING_TASK));
//		Item_Basic_Info item_basic;
//		item.set_item_basic_info(item_basic);
//		acv_msg.item_info.push_back(item_basic);
	}

	if(ring_award_it->second.item3_id > 0 && ring_award_it->second.item3_num > 0 && player_level_it->second.item3_percent > 0) {
		Item_Detail::Bind_Type type = Item_Detail::UNDEFINED;
		type = ring_award_it->second.is_item3_bind ? Item_Detail::BIND : Item_Detail::UNBIND;
		make_item_vec(item_list, ring_award_it->second.item3_id,
						ring_award_it->second.item3_num * player_level_it->second.item3_percent, type);
//		Item_Detail item(ring_award_it->second.item3_id, ring_award_it->second.item3_num * player_level_it->second.item3_percent, type);
//		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
//				Gain_Item_DM_Info(Pack::ITEM_GAIN_RING_TASK));
//		Item_Basic_Info item_basic;
//		item.set_item_basic_info(item_basic);
//		acv_msg.item_info.push_back(item_basic);
	}

	if(ring_award_it->second.item4_id > 0 && ring_award_it->second.item4_num > 0 && player_level_it->second.item4_percent > 0) {
		Item_Detail::Bind_Type type = Item_Detail::UNDEFINED;
		type = ring_award_it->second.is_item4_bind ? Item_Detail::BIND : Item_Detail::UNBIND;
		make_item_vec(item_list, ring_award_it->second.item4_id,
						ring_award_it->second.item4_num * player_level_it->second.item4_percent, type);
//		Item_Detail item(ring_award_it->second.item4_id, ring_award_it->second.item4_num * player_level_it->second.item4_percent, type);
//		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
//				Gain_Item_DM_Info(Pack::ITEM_GAIN_RING_TASK));
//		Item_Basic_Info item_basic;
//		item.set_item_basic_info(item_basic);
//		acv_msg.item_info.push_back(item_basic);
	}

	player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_RING_TASK));

	Item_Basic_Info item_basic;
	Card_Info card_info;
	for (Item_Vec::iterator item_it = item_list.begin(); item_it != item_list.end(); ++item_it) {
		item_basic.reset();
		card_info.reset();
		(*item_it).set_item_basic_info(item_basic);
		acv_msg.item_info.push_back(item_basic);
		if ((*item_it).set_item_card_info(card_info)) {
			acv_msg.card_info_vec.push_back(card_info);
		}
	}

	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	return 0;
}

int Tasker::is_consume_reward_token(uint32_t item_id, uint32_t item_num) {
	if(item_num == 0) {
		MSG_82100008 msg;
		msg.item_ids.push_back(item_id);
		THIS_SEND_TO_CLIENT(msg);
		return -1;//-1：不扣道具，背包不提示错误
	}

	RewardTokenSet &reward_token_set = CONFIG_CACHE_REWARD_TASK->get_config_no_const().reward_token_set;
	if(reward_token_set.find(item_id) == reward_token_set.end()) {
		return ERROR_PURPLE_REWARD_TOKEN_CAN_NOT_USE;
	}

	const Reward_Task_Config&  reward_task_cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	uint32_t max_accept_count = reward_task_cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ tasker_detail_->give_up_num
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);
	if(tasker_detail_->used_accept_count >= max_accept_count) {
		return ERROR_REWARD_TASK_ACCEPT_COUNT_BIGGER_MAX;
	}

	if(player_self()->level() < (int)reward_task_cfg.open_level || !tasker_detail_->is_ever_open_reward_task_list) {
		return ERROR_REWARD_TASK_LIST_IS_NOT_OPEN;
	}

	//删除在背包中用金色悬赏令领取的已提交悬赏任务
	{
		TaskSet temp_set;
		for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
				it != tasker_detail_->reward_task_map.end(); ++it) {
			if(it->second.__is_pack_accept && task_state(it->first) == TaskInfo::TASK_STATUS_SUBMIT) {
				temp_set.insert(it->second.__task_id);
			}
		}

		for(TaskSet::iterator it = temp_set.begin(); it != temp_set.end(); ++it) {
			tasker_detail_->reward_task_map.erase(*it);
		}
		temp_set.clear();
	}


	bool is_find_reward_task = false;
	std::vector<uint32_t> task_id_vec;

	for(Quality_RewardTaskIdSet_Map::const_iterator quality_it = tasker_detail_->quality_RewardTaskIdSet_map.begin();
			quality_it != tasker_detail_->quality_RewardTaskIdSet_map.end(); ++quality_it) {
		if(quality_it->first != TASK_QUALITY_ORANGE) {
			continue;
		}

		for(RewardTaskIdSet::const_iterator it = quality_it->second.begin();
				it != quality_it->second.end(); ++it) {
		    const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(*it);
		    if (!task_config) {
		    	continue;
		    }

			TaskInfo *task_info = find_task(*it);
			if(task_info) {
				if (task_info && task_info->is_accepted()) {
					return ERROR_REWARD_TASK_DO_COUNT_BIGGER_MAX;
				}
				//防止出现相同id的悬赏任务
				continue;
			}

			if(task_config->quality != TASK_QUALITY_ORANGE || task_config->accept.val1 == 0
					|| task_config->accept.val2  == 0) {
				continue;
			}

			if(task_config->accept.val1 == item_id && task_config->accept.val2 > item_num) {
				continue;
			}

			if(task_config->accept.level != 0 && player_self()->level() < task_config->accept.level) {
				MSG_USER("player level lower than task's min accept level, player_level:%d, task accept min level:%d, "
						"task_id=%d\n", player_self()->level(), task_config->accept.level, *it);
				continue;;
			}

			if(task_config->accept.level_max != 0 && player_self()->level() > task_config->accept.level_max) {
				MSG_USER("player level bigger than task's max accept level, player_level:%d, task accept max level:%d, "
						"task_id=%d\n", player_self()->level(), task_config->accept.level_max, *it);
				continue;
			}

			is_find_reward_task = true;
			task_id_vec.push_back(*it);
		}//end for
	}

	if(!is_find_reward_task || task_id_vec.empty()) {
		return ERROR_NO_SUITED_LEVEL_REWARD_TASK;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
    uint32_t index = random() % task_id_vec.size();
	uint32_t task_id = task_id_vec[index];
	if(!task_id) {
		return ERROR_USE_ORANGE_REWARD_TOKEN_ACCEPT_TASK_FAIL;
	}

	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);

	//背包空间不足，不能接悬赏任务
    if(task_cfg->task_type == TaskInfo::TASK_GTYPE_REWARD_TASK) {
    	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
		if(CONFIG_CACHE_ITEM->find_item(cfg.item_id)) {
			Item_Detail item(cfg.item_id,cfg.item_num, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
			if(player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX, item) != 0) {
				return ERROR_PACKAGE_FULL_NO_ACCEPT_REWARD_TASK;
			}
		}
    }

	init_reward_task(task_id, *task_cfg);

	TaskInfo *task_info = find_task(task_id);
	if(!task_info) {
		return ERROR_USE_ORANGE_REWARD_TOKEN_ACCEPT_TASK_FAIL;
	}

    int ret = process_accept_imp(task_info->__task_id);
    if (0 == ret) {
        //在背包中用金色悬赏令领取
    	task_info->__is_pack_accept = true;
	   this->tasker_detail_->save_tick();
    }

    return ret;
}

int Tasker::trigger_daily_zero(const Time_Value &now) {
	uint8_t time = 0;
	refresh_day_task(time);

	Date_Time date_time(now);
	uint8_t wday = (uint8_t)date_time.weekday();
	refresh_week_task(wday, time);
	return 0;
}

int Tasker::trigger_daily_six(const Time_Value &now) {
	daily_refresh_reward_task_list(now);
	uint8_t time = 6;
	refresh_day_task(time);

	Date_Time date_time(now);
	uint8_t wday = (uint8_t)date_time.weekday();
	refresh_week_task(wday, time);

	if(wday == 1) {
		refresh_open_ring_task();
	}

	return 0;
}

void Tasker::daily_refresh_reward_task_list(const Time_Value &now) {
	if(!tasker_detail_->is_ever_open_reward_task_list) {
		return;
	}

	tasker_detail_->did_reward_task_num_today = 0;
	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	tasker_detail_->used_free_refresh_count = 0;
	tasker_detail_->used_accept_count = 0;
	tasker_detail_->give_up_num = 0;
	tasker_detail_->is_close_auto_refresh = false;
	tasker_detail_->is_exist_cross_day = false;
	if(is_exist_accepted_reward_task()) {
		tasker_detail_->is_exist_cross_day = true;
	}

	//悬赏任务列表不为空，并且，不存在已领取未完成的悬赏任务， 刷新悬赏任务列表
	if(!tasker_detail_->reward_task_map.empty() && !is_exist_accepted_reward_task()) {
		tasker_detail_->is_open_countdown = true;
	}

	MSG_50110012 msg;
	if(tasker_detail_->is_open_countdown) {
		tasker_detail_->begin_countdown_time.set(now);
		msg.auto_refresh_time = cfg.auto_refresh * Time_Value::ONE_MINUTE_IN_SECS;
	}

	msg.is_open_countdown = tasker_detail_->is_open_countdown ? 1 : 0;
	msg.free_refresh_count = cfg.free_refresh_max_count_per_day;
	msg.normal_accept_count = cfg.accept_max_count_per_day + player_self()->achievement_reward_times()
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);

	//悬赏任务列表不为空，并且，不存在已领取未完成的悬赏任务， 刷新悬赏任务列表
	if(!tasker_detail_->reward_task_map.empty() && !is_exist_accepted_reward_task()) {
		build_new_reward_task_list_of_server(false);
	}

	copy_reward_task_list_to_msg(RES_REWARD_TASK_LIST, &msg, 0);

	THIS_SEND_TO_CLIENT(msg);

	this->save_tick();

	notify_remain_reward_task_num();
}

void Tasker::build_new_reward_task_list_of_server(bool is_process_first_hand_refresh/*=false*/) {
	if(is_process_first_hand_refresh && 1 == tasker_detail_->refresh_reward_task_num) {
		first_build_new_reward_task_list_of_server();
		return;
	}
	//如果旧列表中有失败状态，先通知主UI删除失败限时悬赏任务
	for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
			it != tasker_detail_->reward_task_map.end(); ++it) {
		if(task_state(it->second.__task_id) == TaskInfo::TASK_STATUS_FAIL) {
			notify_task_del(it->second);
		}
	}

	//刷新新列表前，清空旧列表 (注明：保留在背包中用金色悬赏令领取的悬赏任务)
	{
		TaskSet temp_set;
		for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
				it != tasker_detail_->reward_task_map.end(); ++it) {
			if(!it->second.__is_pack_accept) {
				temp_set.insert(it->second.__task_id);
			}

			//删除在背包中用金色悬赏令领取的已提交悬赏任务
			if(it->second.__is_pack_accept && task_state(it->first) == TaskInfo::TASK_STATUS_SUBMIT) {
				temp_set.insert(it->second.__task_id);
			}
		}

		for(TaskSet::iterator it = temp_set.begin(); it != temp_set.end(); ++it) {
			tasker_detail_->reward_task_map.erase(*it);
		}
		temp_set.clear();
	}

	for(int i = 0; i < 4; ) {
		RewardTaskIdSet temp_reward_task_set;
		temp_reward_task_set.clear();

	    uint8_t quality = TASK_QUALITY_END;
	    Quality_RewardTaskIdSet_Map::iterator quality_it = tasker_detail_->quality_RewardTaskIdSet_map.end();

	    while(1) {
	    	quality = get_reward_task_quality_by_chance();
	    	quality_it = tasker_detail_->quality_RewardTaskIdSet_map.find(quality);
	        if(quality_it != tasker_detail_->quality_RewardTaskIdSet_map.end()
	        		&& !quality_it->second.empty()) {
	        	break;
	        }
	    }

	    temp_reward_task_set.insert(quality_it->second.begin(), quality_it->second.end());
	    for(RewardTaskMap::const_iterator it = tasker_detail_->reward_task_map.begin();
	    		it != tasker_detail_->reward_task_map.end();
	    		++it) {
	    	temp_reward_task_set.erase(it->first);
	    }

	    if(temp_reward_task_set.empty()) {
	    	continue;
	    }

		//随机产生任务id
	    srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
		int index = random() % (temp_reward_task_set.size());
		if(index < 0) {
			continue;
		}

		uint32_t task_id = 0;
		int j = 0;
		for(RewardTaskIdSet::iterator it = temp_reward_task_set.begin();
				it != temp_reward_task_set.end(); ++it) {
			if(j == index) {
				task_id = *it;
				break;
			}
			++j;
		}

		//避免产生重复的任务id
		if(task_id == 0
				|| tasker_detail_->reward_task_map.find(task_id) !=  tasker_detail_->reward_task_map.end()) {
			continue;
		}

		const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
		if(!task_cfg) {
			continue;
		}

		if(task_cfg->accept.level != 0 && player_self()->level() < task_cfg->accept.level) {
			continue;
		}

		if(task_cfg->accept.level_max != 0 && player_self()->level() > task_cfg->accept.level_max) {
			continue;
		}

		init_reward_task(task_id, *task_cfg);
		++i;

		temp_reward_task_set.clear();
	}//end for
}

void Tasker::first_build_new_reward_task_list_of_server() {
	//如果旧列表中有失败状态，先通知主UI删除失败限时悬赏任务
	for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
			it != tasker_detail_->reward_task_map.end(); ++it) {
		if(task_state(it->second.__task_id) == TaskInfo::TASK_STATUS_FAIL) {
			notify_task_del(it->second);
		}
	}

	//刷新新列表前，清空旧列表 (注明：保留在背包中用金色悬赏令领取的悬赏任务)
	{
		TaskSet temp_set;
		for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
				it != tasker_detail_->reward_task_map.end(); ++it) {
			if(!it->second.__is_pack_accept) {
				temp_set.insert(it->second.__task_id);
			}

			//删除在背包中用金色悬赏令领取的已提交悬赏任务
			if(it->second.__is_pack_accept && task_state(it->first) == TaskInfo::TASK_STATUS_SUBMIT) {
				temp_set.insert(it->second.__task_id);
			}
		}

		for(TaskSet::iterator it = temp_set.begin(); it != temp_set.end(); ++it) {
			tasker_detail_->reward_task_map.erase(*it);
		}
		temp_set.clear();
	}

	bool is_task_id_useable = false;
	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(11001005);
	if(task_cfg) {
		if(TASK_QUALITY_ORANGE == task_cfg->quality
				&& tasker_detail_->reward_task_map.find(11001005) ==  tasker_detail_->reward_task_map.end()) {
			is_task_id_useable = true;
		}
	}

	if(is_task_id_useable) {
		if(task_cfg->accept.level != 0 && player_self()->level() < task_cfg->accept.level) {
			is_task_id_useable = false;
		}

		if(task_cfg->accept.level_max != 0 && player_self()->level() > task_cfg->accept.level_max) {
			is_task_id_useable = false;
		}

		if(is_task_id_useable) {
			init_reward_task(11001005, *task_cfg);
		}
	}

	if(!is_task_id_useable) {//第一次刷新悬赏任务列表，固定第一个产生橙色品质悬赏任务
		Quality_RewardTaskIdSet_Map::const_iterator first_quality_it =
				tasker_detail_->quality_RewardTaskIdSet_map.find(TASK_QUALITY_ORANGE);
		if(first_quality_it != tasker_detail_->quality_RewardTaskIdSet_map.end() && !first_quality_it->second.empty()) {
			while(1) {
				//随机产生任务id
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				int index = random() % (first_quality_it->second.size());
				if(index < 0) {
					continue;
				}

				uint32_t task_id = 0;
				int j = 0;
				for(RewardTaskIdSet::iterator it = first_quality_it->second.begin();
						it != first_quality_it->second.end(); ++it) {
					if(j == index) {
						task_id = *it;
						break;
					}
					++j;
				}

				//避免产生重复的任务id
				if(task_id == 0
						|| tasker_detail_->reward_task_map.find(task_id) !=  tasker_detail_->reward_task_map.end()) {
					continue;
				}

				const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
				if(!task_cfg) {
					continue;
				}

				if(task_cfg->accept.level != 0 && player_self()->level() < task_cfg->accept.level) {
					continue;
				}

				if(task_cfg->accept.level_max != 0 && player_self()->level() > task_cfg->accept.level_max) {
					continue;
				}

				init_reward_task(task_id, *task_cfg);

				break;
			}//end while
		}//end if
    }//end if

    //剩余三个悬赏任务，概率刷新
    {
    	for(int i = 0; i < 3; ) {
    		RewardTaskIdSet temp_reward_task_set;
    		temp_reward_task_set.clear();

    	    uint8_t quality = TASK_QUALITY_END;
    	    Quality_RewardTaskIdSet_Map::iterator quality_it = tasker_detail_->quality_RewardTaskIdSet_map.end();

    	    while(1) {
    	    	quality = get_reward_task_quality_by_chance();
    	    	quality_it = tasker_detail_->quality_RewardTaskIdSet_map.find(quality);
    	        if(quality_it != tasker_detail_->quality_RewardTaskIdSet_map.end()
    	        		&& !quality_it->second.empty()) {
    	        	break;
    	        }
    	    }

    	    temp_reward_task_set.insert(quality_it->second.begin(), quality_it->second.end());
    	    for(RewardTaskMap::const_iterator it = tasker_detail_->reward_task_map.begin();
    	    		it != tasker_detail_->reward_task_map.end();
    	    		++it) {
    	    	temp_reward_task_set.erase(it->first);
    	    }

    	    if(temp_reward_task_set.empty()) {
    	    	continue;
    	    }

    		//随机产生任务id
    	    srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
    		int index = random() % (temp_reward_task_set.size());
    		if(index < 0) {
    			continue;
    		}

    		uint32_t task_id = 0;
    		int j = 0;
    		for(RewardTaskIdSet::iterator it = temp_reward_task_set.begin();
    				it != temp_reward_task_set.end(); ++it) {
    			if(j == index) {
    				task_id = *it;
    				break;
    			}
    			++j;
    		}

    		//避免产生重复的任务id
    		if(task_id == 0
    				|| tasker_detail_->reward_task_map.find(task_id) !=  tasker_detail_->reward_task_map.end()) {
    			continue;
    		}

    		const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
    		if(!task_cfg) {
    			continue;
    		}

    		if(task_cfg->accept.level != 0 && player_self()->level() < task_cfg->accept.level) {
    			continue;
    		}

    		if(task_cfg->accept.level_max != 0 && player_self()->level() > task_cfg->accept.level_max) {
    			continue;
    		}

    		init_reward_task(task_id, *task_cfg);
    		++i;

    		temp_reward_task_set.clear();
    	}//end for
    }
}

void Tasker::copy_reward_task_list_to_msg(uint32_t msg_id, MSG_50110012* msg_50110012, MSG_81110007 *msg_81110007) {
	if(ACTIVE_REWARD_TASK_LIST != msg_id && RES_REWARD_TASK_LIST != msg_id) {
		MSG_USER("msg_id is invalid, msg_id=%ld\n", msg_id);
		return;
	}

	if(ACTIVE_REWARD_TASK_LIST == msg_id && !msg_81110007) {
		MSG_USER("pointer msg_81110007 is null, msg_id=%ld\n", msg_id);
		return;
	}

	if(RES_REWARD_TASK_LIST == msg_id && !msg_50110012) {
		MSG_USER("pointer msg_50110012 is null, msg_id=%ld\n", msg_id);
		return;
	}

	for(RewardTaskMap::const_iterator it = tasker_detail_->reward_task_map.begin();
			it != tasker_detail_->reward_task_map.end(); ++it) {
		if(it->second.__is_pack_accept) {
			continue;
		}

		if (it->second.is_submit() == false &&
				it->second.is_finish() == false &&
				it->second.is_accepted() == false &&
				it->second.is_acceptable() == false &&
				it->second.is_visible() == false &&
				it->second.is_fail() == false) {
			continue;
		}

		Task_Info task_info;
		task_info.reset();
		task_info.id = (int32_t)it->second.__task_id;
		task_info.type = it->second.__game_type;
		task_info.status = (uint8_t)task_state(it->second.__task_id);
		task_info.time = it->second.__accept_tick.sec();

	    if(TaskInfo::TASK_GTYPE_RING_TASK == task_info.type) {
	    	task_info.ring_num = tasker_detail_->ring_num;
	    }

		const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(it->second.__task_id);
		if (!task_json) {
			MSG_USER("task not exits, task_id=%d\n", it->second.__task_id);
			continue;
		}

	    calc_remain_time_of_limit_time_task(task_info, it->second, *task_json);

		for (TaskInfo::TaskConditionList::const_iterator it_cond = it->second.__condition_list.begin();
				it_cond != it->second.__condition_list.end(); ++it_cond) {
			task_info.value.push_back((*it_cond).__current_value);
		}

		if(RES_REWARD_TASK_LIST == msg_id) {
			msg_50110012->reaward_task_list.push_back(task_info);
			continue;
		}

		if(ACTIVE_REWARD_TASK_LIST == msg_id) {
			msg_81110007->reward_task_list.push_back(task_info);
		}
	}//end for
}

void Tasker::first_copy_reward_task_list_to_msg(uint32_t msg_id, MSG_50110012* msg_50110012,
											MSG_81110007 *msg_81110007) {
	if(ACTIVE_REWARD_TASK_LIST != msg_id && RES_REWARD_TASK_LIST != msg_id) {
		MSG_USER("msg_id is invalid, msg_id=%ld\n", msg_id);
		return;
	}

	if(ACTIVE_REWARD_TASK_LIST == msg_id && !msg_81110007) {
		MSG_USER("pointer msg_81110007 is null, msg_id=%ld\n", msg_id);
		return;
	}

	if(RES_REWARD_TASK_LIST == msg_id && !msg_50110012) {
		MSG_USER("pointer msg_50110012 is null, msg_id=%ld\n", msg_id);
		return;
	}

	bool is_task_id_useable = false;
	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(11001005);
	if(task_cfg) {
		TaskInfo *task_info = find_task(11001005);
		if(task_info && !task_info->__is_pack_accept) {
			is_task_id_useable = true;
		}
	}

	uint32_t first_orange_task_id = 0;

	//第一次刷新悬赏任务列表，固定第一个产生橙色品质悬赏任务
	for(RewardTaskMap::const_iterator it = tasker_detail_->reward_task_map.begin();
			it != tasker_detail_->reward_task_map.end(); ++it) {
		if(it->second.__is_pack_accept) {
			continue;
		}

		if (it->second.is_submit() == false &&
				it->second.is_finish() == false &&
				it->second.is_accepted() == false &&
				it->second.is_acceptable() == false &&
				it->second.is_visible() == false &&
				it->second.is_fail() == false) {
			continue;
		}

		const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(it->second.__task_id);
		if (!task_json) {
			MSG_USER("task not exits, task_id=%d\n", it->second.__task_id);
			continue;
		}

		if(TASK_QUALITY_ORANGE != task_json->quality) {
			continue;
		}

		if(is_task_id_useable && 11001005 != it->second.__task_id) {
			continue;
		}

		first_orange_task_id = it->second.__task_id;

		Task_Info task_info;
		task_info.reset();
		task_info.id = (int32_t)it->second.__task_id;
		task_info.type = it->second.__game_type;
		task_info.status = (uint8_t)task_state(it->second.__task_id);
		task_info.time = it->second.__accept_tick.sec();

	    if(TaskInfo::TASK_GTYPE_RING_TASK == task_info.type) {
	    	task_info.ring_num = tasker_detail_->ring_num;
	    }

	    calc_remain_time_of_limit_time_task(task_info, it->second, *task_json);

		for (TaskInfo::TaskConditionList::const_iterator it_cond = it->second.__condition_list.begin();
				it_cond != it->second.__condition_list.end(); ++it_cond) {
			task_info.value.push_back((*it_cond).__current_value);
		}

		if(RES_REWARD_TASK_LIST == msg_id) {
			msg_50110012->reaward_task_list.push_back(task_info);
			continue;
		}

		if(ACTIVE_REWARD_TASK_LIST == msg_id) {
			msg_81110007->reward_task_list.push_back(task_info);
		}

		break;
	}//end for

	//剩余三个悬赏任务
	for(RewardTaskMap::const_iterator it = tasker_detail_->reward_task_map.begin();
			it != tasker_detail_->reward_task_map.end(); ++it) {
		if(it->second.__is_pack_accept) {
			continue;
		}

		if (it->second.is_submit() == false &&
				it->second.is_finish() == false &&
				it->second.is_accepted() == false &&
				it->second.is_acceptable() == false &&
				it->second.is_visible() == false &&
				it->second.is_fail() == false) {
			continue;
		}

		const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(it->second.__task_id);
		if (!task_json) {
			MSG_USER("task not exits, task_id=%d\n", it->second.__task_id);
			continue;
		}

		if(it->second.__task_id == first_orange_task_id) {
			continue;
		}

		Task_Info task_info;
		task_info.reset();
		task_info.id = (int32_t)it->second.__task_id;
		task_info.type = it->second.__game_type;
		task_info.status = (uint8_t)task_state(it->second.__task_id);
		task_info.time = it->second.__accept_tick.sec();

	    if(TaskInfo::TASK_GTYPE_RING_TASK == task_info.type) {
	    	task_info.ring_num = tasker_detail_->ring_num;
	    }

	    calc_remain_time_of_limit_time_task(task_info, it->second, *task_json);

		for (TaskInfo::TaskConditionList::const_iterator it_cond = it->second.__condition_list.begin();
				it_cond != it->second.__condition_list.end(); ++it_cond) {
			task_info.value.push_back((*it_cond).__current_value);
		}

		if(RES_REWARD_TASK_LIST == msg_id) {
			msg_50110012->reaward_task_list.push_back(task_info);
			continue;
		}

		if(ACTIVE_REWARD_TASK_LIST == msg_id) {
			msg_81110007->reward_task_list.push_back(task_info);
		}

		break;
	}//end for

}

bool Tasker::is_exist_accepted_reward_task() {
	bool is_exist_accepted_task = false;

	for(RewardTaskMap::iterator it = tasker_detail_->reward_task_map.begin();
			it != tasker_detail_->reward_task_map.end(); ++it) {
        //存在已领取未完成的悬赏任务，不刷新悬赏任务列表
		if(it->second.is_accepted() && !it->second.__is_pack_accept) {
			is_exist_accepted_task = true;
			break;
		}
	}

	return is_exist_accepted_task;
}

uint8_t Tasker::get_reward_task_quality_by_chance() {
	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	int num = random() % 100;
	if(num >= 0 && num <= (cfg.white_chance - 1)) {
		return TASK_QUALITY_WHITE;
	}

	if(num >= cfg.white_chance && num <= (cfg.white_chance + cfg.green_chance - 1)) {
		return TASK_QUALITY_GREEN;
	}

	if(num >= (cfg.white_chance + cfg.green_chance)
			&& num <= (cfg.white_chance + cfg.green_chance + cfg.blue_chance - 1)) {
		return TASK_QUALITY_BLUE;
	}

	if(num >= (cfg.white_chance + cfg.green_chance + cfg.blue_chance)
			&& num <= (cfg.white_chance + cfg.green_chance + cfg.blue_chance + cfg.purple_chance - 1)) {
		return TASK_QUALITY_PURPLE;
	}

	if(num >= (cfg.white_chance + cfg.green_chance + cfg.blue_chance + cfg.purple_chance)
			&& num <= (cfg.white_chance + cfg.green_chance + cfg.blue_chance
					        + cfg.purple_chance + cfg.orange_chance - 1)) {
		return TASK_QUALITY_ORANGE;
	}

	return TASK_QUALITY_WHITE;
}

int Tasker::reqFinishNpcTalkTask(uint32_t npc_id) {
	task_listen_npc_talk(npc_id);
	return 0;
}

bool Tasker::is_can_do_reward_task() {
	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	uint32_t max_accept_count = cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ tasker_detail_->give_up_num
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);
	return tasker_detail_->used_accept_count < max_accept_count;
}

uint32_t Tasker::total_num_of_can_do_reward_task() {
	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	uint32_t max_accept_count = cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);
	if(tasker_detail_->is_exist_cross_day) {
		max_accept_count += 1;
	}
	return	max_accept_count;
}

uint32_t Tasker::num_of_can_do_reward_task() {
	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	uint32_t max_accept_count = cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ tasker_detail_->give_up_num
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);
	return (tasker_detail_->used_accept_count <= max_accept_count) ?
			(max_accept_count - tasker_detail_->used_accept_count) : 0;
}

void Tasker::refresh_day_task(uint8_t time) {
	const Day_Group_Set *day_group_set_ptr = CONFIG_CACHE_TASK->find_day_group_set(time);
	if(!day_group_set_ptr) {
		return;
	}

	for(Day_Group_Set::const_iterator group_it = day_group_set_ptr->begin();
			group_it != day_group_set_ptr->end();
			++group_it) {
		uint32_t group = *group_it;

		const Task_Config &task_cfg = CONFIG_CACHE_TASK->get_task_config();
		for(Task_Mission_Config::const_iterator task_cfg_it = task_cfg.missions.begin();
				task_cfg_it != task_cfg.missions.end();
				++task_cfg_it) {
			if(task_cfg_it->second.task_type != TaskInfo::TASK_GTYPE_DAY
					|| task_cfg_it->second.day_group != group) {
				continue;
			}

			TaskMap::iterator task_info_it = tasker_detail_->task_map_.find(task_cfg_it->first);
			if(task_info_it != tasker_detail_->task_map_.end()) {
				notify_task_del(task_info_it->second);
				unbind_task(task_cfg_it->first);
			}
			else {
				tasker_detail_->task_submited_once_.erase(task_cfg_it->first);
			}

			if(task_cfg_it->second.accept.level && player_self()->level() < task_cfg_it->second.accept.level) {
				continue;
			}

			if(task_cfg_it->second.accept.level_max && player_self()->level() > task_cfg_it->second.accept.level_max) {
				continue;
			}

			if (task_cfg_it->second.precondition > 0
					&& this->tasker_detail_->task_submited_once_.find( task_cfg_it->second.precondition ) == this->tasker_detail_->task_submited_once_.end()) {
				continue;
			}

			if(task_cfg_it->second.vip_accept_level > 0 && player_self()->vip() != task_cfg_it->second.vip_accept_level) {
				continue;
			}

		    if(task_cfg_it->first > 0 && task_submited_once_set().find(task_cfg_it->first) == task_submited_once_set().end()) {
		    	const Task_Mission_Detail_Config *task_mission_cfg = CONFIG_CACHE_TASK->find_task(task_cfg_it->first);
		    	if(task_mission_cfg) {
		    		insert_task(task_cfg_it->first, *task_mission_cfg);
		    	}
		    }

		}
	}//end for

	this->save_tick();
}

void Tasker::refresh_week_task(uint8_t wday, uint8_t time) {
	const Week_Group_Set *week_group_set_ptr = CONFIG_CACHE_TASK->find_week_group_set(wday, time);
	if(!week_group_set_ptr) {
		return;
	}

	for(Week_Group_Set::const_iterator group_it = week_group_set_ptr->begin();
			group_it != week_group_set_ptr->end();
			++group_it) {
		uint32_t group = *group_it;

		const Task_Config &task_cfg = CONFIG_CACHE_TASK->get_task_config();
		for(Task_Mission_Config::const_iterator task_cfg_it = task_cfg.missions.begin();
				task_cfg_it != task_cfg.missions.end();
				++task_cfg_it) {
			if(task_cfg_it->second.task_type != TaskInfo::TASK_GTYPE_WEEK
					|| task_cfg_it->second.day_group != group) {
				continue;
			}

			TaskMap::iterator task_info_it = tasker_detail_->task_map_.find(task_cfg_it->first);
			if(task_info_it != tasker_detail_->task_map_.end()) {
				notify_task_del(task_info_it->second);
				unbind_task(task_cfg_it->first);
			}
			else {
				tasker_detail_->task_submited_once_.erase(task_cfg_it->first);
			}

			if(task_cfg_it->second.accept.level && player_self()->level() < task_cfg_it->second.accept.level) {
				continue;
			}

			if(task_cfg_it->second.accept.level_max && player_self()->level() > task_cfg_it->second.accept.level_max) {
				continue;
			}

			if (task_cfg_it->second.precondition > 0
					&& this->tasker_detail_->task_submited_once_.find( task_cfg_it->second.precondition ) == this->tasker_detail_->task_submited_once_.end()) {
				continue;
			}

		    if(task_cfg_it->first > 0 && task_submited_once_set().find(task_cfg_it->first) == task_submited_once_set().end()) {
		    	const Task_Mission_Detail_Config *task_mission_cfg = CONFIG_CACHE_TASK->find_task(task_cfg_it->first);
		    	if(task_mission_cfg) {
		    		insert_task(task_cfg_it->first, *task_mission_cfg);
		    	}
		    }
		}
	}//end for

	this->save_tick();
}

void Tasker::process_activate_horse(uint32_t task_id) {
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
    if (NULL == task_json) {
    	return;
    }
    if(task_json->horse_id > 0) {
    	player_self()->mount_activate_by_item(task_json->horse_id);
    }
}

void Tasker::Log_Task_Stream(int64_t role_id, uint32_t task_id, uint8_t task_stream_status,
			uint32_t player_level, int64_t time_sec) {
	MSG_STREAM("`stream_task`(`player_id`, `task_id`, `status`, `level`, `time`) VALUES(%ld, %d, %d, %d, %ld)",
			   role_id, task_id, task_stream_status, player_level, time_sec);
}

int Tasker::process_task_listen_when_accepted(uint32_t task_id) {
	TaskInfo * task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}

    if (task_info->is_logic_level_up()) {
    	task_listen_lvl_up(player_self()->level());
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if (task_info->is_logic_collect() || task_info->is_logic_package()
    		|| task_info->is_logic_commit_consume_item()
    		|| task_info->is_logic_deliver_letter()
    		|| task_info->is_logic_have_item_but_no_erase()) {
    	task_listen_item_update();
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_join_gang()) {
    	task_listen_join_gang(player_self()->ganger_detail().gang_id);
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_ganger_level()) {
    	gang_id_t gang_id = player_self()->ganger_detail().gang_id;
    	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(gang_id);
    	if (gang_view) {
    		task_listen_gang_level(gang_id, gang_view->level);
    	}
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_body_equip_level()
    		|| task_info->is_logic_equip_level()
    		|| task_info->is_logic_role_pull_on_equip()) {
    	const Item_Detail *equip = NULL;
    	uint32_t equip_id = 0;
    	int position = 0;
    	int equip_level = 0;
    	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
    		equip = player_self()->pack_get_const_item(index);
    		if (! equip) {
    			continue;
    		}

    		equip_id = equip->id_;
    		position = player_self()->get_part_by_id((int)equip->id_);
    		equip_level = equip->addon.equip.unban_lvl;

    	    task_info = find_task(task_id);
    		if(!task_info) {
    			return 0;
    		}
    		if(task_info->is_logic_body_equip_level()) {
    			task_listen_body_equip_level(position, equip_level);
    		}

    	    task_info = find_task(task_id);
    		if(!task_info) {
    			return 0;
    		}
    	    if(task_info->is_logic_equip_level()) {
    	    	task_listen_equip_level(equip_id, equip_level);
    	    }

    	    task_info = find_task(task_id);
    	    if(!task_info) {
    	    	return 0;
    	    }

    	    if(task_info->is_logic_role_pull_on_equip()) {
    	    	task_listen_role_pull_on_equip(equip_id);
    	    }
    	}
    }//end if

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_arena_level()) {
    	int group_id = 0;
    	int level = 0;
    	player_self()->get_arena_level(group_id, level);
    	task_listen_arena_level(group_id, level);
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_keep_hero_to_level()) {
    	Int_Int_Map hero_id_level_map;
    	hero_id_level_map.clear();
    	player_self()->get_heros_lvl(hero_id_level_map);
    	for(Int_Int_Map::iterator it = hero_id_level_map.begin(); it != hero_id_level_map.end(); ++it) {
    		task_listen_keep_hero_to_level(it->first, it->second);
    	}
    	hero_id_level_map.clear();
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_wake_up_hero_to_quality_level()) {
    	Int_Int_Map hero_id_awake_level_map;
    	hero_id_awake_level_map.clear();
    	player_self()->get_heros_awake_lvl(hero_id_awake_level_map);
    	for(Int_Int_Map::iterator it = hero_id_awake_level_map.begin(); it != hero_id_awake_level_map.end(); ++it) {
    		task_listen_wake_up_hero_to_quality_level(it->first, it->second + 1);
    	}
    	hero_id_awake_level_map.clear();
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_equip_crimson_level()) {
    	Int_Int_Map crimson_id_level_map;
    	crimson_id_level_map.clear();
    	player_self()->get_rune_part_lvl(crimson_id_level_map);
    	for(Int_Int_Map::iterator it = crimson_id_level_map.begin(); it != crimson_id_level_map.end(); ++it) {
    		task_listen_equip_crimson_level(it->first, it->second);
    	}
    	crimson_id_level_map.clear();
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_ganger_skill_level()) {
    	const Ganger_Skill_Map &ganer_skill_map = player_self()->ganger_detail().skill_map;
    	for(Ganger_Skill_Map::const_iterator it = ganer_skill_map.begin(); it != ganer_skill_map.end(); ++it) {
    		task_listen_ganger_skill_level(it->first);
    	}
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_activeness_of_today()) {
    	task_listen_activeness_of_today(player_self()->daily_detail().today_activity);
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_dragon_vale_building_upgrade()) {
    	boost::unordered_multimap<uint8_t, uint16_t> building_map;
    	player_self()->get_dragon_vale_building_type_level_map(building_map);
    	for(boost::unordered_multimap<uint8_t, uint16_t>::const_iterator it = building_map.begin();
    			it != building_map.end();
    			++it) {
    		task_listen_dragon_vale_building_upgrade(it->first, it->second);
    	}
    	building_map.clear();
    }

    task_info = find_task(task_id);
	if(!task_info) {
		return 0;
	}
    if(task_info->is_logic_create_dragon_vale_building()) {
    	boost::unordered_multimap<uint8_t, uint16_t> building_map;
    	player_self()->get_dragon_vale_building_type_level_map(building_map);
    	for(boost::unordered_multimap<uint8_t, uint16_t>::const_iterator it = building_map.begin();
    			it != building_map.end();
    			++it) {
    		task_listen_create_dragon_vale_building(it->first);
    	}
    	building_map.clear();
    }

    task_info = find_task(task_id);
    if(!task_info) {
    	return 0;
    }

    if(task_info->is_logic_keep_dragon_to_level()) {
    	player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_POWER, player_self()->dragon_vale_detail().dragon_info.power_level);
    	player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_ARMOR, player_self()->dragon_vale_detail().dragon_info.armor_level);
		player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_STAMINA, player_self()->dragon_vale_detail().dragon_info.stamina_level);
		player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_HERO_POWER, player_self()->dragon_vale_detail().dragon_info.hero_power_level);
		player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_HERO_ARMOR, player_self()->dragon_vale_detail().dragon_info.hero_armor_level);
		player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_HERO_STAMINA, player_self()->dragon_vale_detail().dragon_info.hero_stamina_level);
		player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_CRIT, player_self()->dragon_vale_detail().dragon_info.crit_level);
		player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_PREVENT_CRIT, player_self()->dragon_vale_detail().dragon_info.prevent_crit_level);
		player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_HERO_CRIT, player_self()->dragon_vale_detail().dragon_info.hero_crit_level);
		player_self()->task_listen_keep_dragon_to_level(KEEP_DRAGON_HERO_PREVENT_CRIT, player_self()->dragon_vale_detail().dragon_info.hero_prevent_crit_level);
    }

    task_info = find_task(task_id);
    if(!task_info) {
    	return 0;
    }

    if(task_info->is_logic_suit()) {
    	const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
    	if(task_json) {
    		for (size_t i = 0; i < task_json->execute.size(); ++i) {
    			if(task_json->execute[i].type == TaskExecType::TASK_EXEC_SUIT) {

    				int most_put_on = 0; //穿最多的穿了几件
    				int suit_id = 0;
    				for (Int_Set::const_iterator it = task_json->execute[i].id_list.begin();
    						it != task_json->execute[i].id_list.end(); ++it) {
    					for(Heroer_Detail::Hero_Map::const_iterator it1 = player_self()->heroer_detail().hero_map.begin();
    							it1 != player_self()->heroer_detail().hero_map.end();
    							++it1) {
    						std::map<int, int>::const_iterator suit_iter = it1->second.suit_cnt_map.find(*it);
    						if (suit_iter != it1->second.suit_cnt_map.end() && suit_iter->second > most_put_on)
    						{
    							suit_id = *it;
    							most_put_on = suit_iter->second;
    						}
    					}

    					int cnt_ = player_self()->get_put_on_suit_count(*it, 0);
    					if (cnt_ > most_put_on)
    					{
    						suit_id = *it;
    						most_put_on = cnt_;
    					}
    				}

    				task_listen_suit(suit_id, most_put_on);
    			}
    		}
    	}
    }

    task_info = find_task(task_id);
    if(!task_info) {
    	return 0;
    }

    if(task_info->is_logic_hero_pull_on_equip()) {
    	const Item_Detail *item = 0;
    	for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
    			it != player_self()->heroer_detail().hero_map.end();
    			++it) {
    		int pack_type = PACK_T_HERO_EQUIP_INDEX + it->second.hero_index * ONE_HERO_EQUIP_GAP;
    		uint cap = player_self()->pack_is_hero_type(pack_type) ?  player_self()->pack_get_hero_capacity_public(pack_type) :  player_self()->pack_get_capacity_public((Pack_Type)pack_type);
    		int idx_start = pack_type + RS_INNER_IDX_BEGIN;
    		int idx_end = pack_type + cap + 1;

    		for (int idx = idx_start; idx < idx_end; ++idx) {
    			item = player_self()->pack_get_const_item(idx);
    			if (!item) {
    				continue;
    			}

    			task_listen_hero_pull_on_equip(item->id_);
    		}
    	}
    }

    task_info = find_task(task_id);
    if(!task_info) {
    	return 0;
    }

    if(task_info->is_logic_activate_wing()) {
    	if(player_self()->wing_detail().lv > 0) {
    		task_listen_activate_wing();
    	}
    }

    task_info = find_task(task_id);
    if(!task_info) {
    	return 0;
    }

    if(task_info->is_logic_get_quality_rune()) {
		std::map<int8_t, uint32_t> rune_quality_num_map;
		rune_quality_num_map.clear();
		player_self()->get_rune_quality_num_map(rune_quality_num_map);

		for(std::map<int8_t, uint32_t>::const_iterator it = rune_quality_num_map.begin();
				it != rune_quality_num_map.end();
				++it) {
			player_self()->task_listen_get_quality_rune(it->first, it->second);
		}
	}

    task_info = find_task(task_id);
    if(!task_info) {
    	return 0;
    }

    if(task_info->is_logic_have_hero()) {
    	player_self()->task_listen_have_hero(player_self()->heroer_detail().hero_map.size());
    }

    task_info = find_task(task_id);
    if(!task_info) {
    	return 0;
    }

    if(task_info->is_logic_have_hero_quality()) {
    	player_self()->task_listen_have_hero_quality();
    }

    task_info = find_task(task_id);
    if(!task_info) {
    	return 0;
    }

    if(task_info->is_logic_first_pass_scene()) {
    	uint32_t scene_id = 0;
        const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
        if(task_json) {
           	for (size_t i = 0; i < task_json->execute.size(); ++i) {
                if(task_json->execute[i].type == TaskExecType::TASK_EXEC_FIRST_PASS_SCENE) {
                	scene_id = task_json->execute[i].id;
                }
           	}
         }

        if(scene_id != 0) {
        	if(player_self()->is_sgl_dungeon_complete(scene_id) || player_self()->is_elite_dungeon_complete(scene_id)) {
        		task_listen_first_pass_scene(scene_id);
        	}
        }
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_dragon_born()) {
    	 if(player_self()->dragon_vale_detail().is_dragon_born) {
    		 task_listen_dragon_born();
    	 }
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_horse_star_level()) {
    	std::vector<Int_Int> horse_star_level_vec;
    	player_self()->get_mount_star_lv(horse_star_level_vec);
    	for(std::vector<Int_Int>::const_iterator it = horse_star_level_vec.begin(); it != horse_star_level_vec.end(); ++it) {
    		int32_t horse_id =  it->val_1;
    		int32_t star_level = it->val_2;
    		task_listen_horse_star_level((uint32_t)horse_id, (uint32_t)star_level);
    	}
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_have_equip_but_no_erase()) {
    	 task_listen_have_equip_but_no_erase();
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_equip_jewelry()) {
    	 task_listen_equip_jewelry();
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_equip_card_all()
    		 || task_info->is_logic_equip_card_by_position()
    		 || task_info->is_logic_card_upgrade_to_level_all()
    		 || task_info->is_logic_card_upgrade_to_level_by_position()) {
    	 task_listen_equip_card();
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_did_reward_task_num()) {
    	 task_listen_did_reward_task_num(tasker_detail_->did_reward_task_num);
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_did_reward_task_num_today()) {
    	 task_listen_did_reward_task_num_today(tasker_detail_->did_reward_task_num_today);
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_inlay_position_gemstone() || task_info->is_logic_inlay_any_gemstone()
    		 || task_info->is_logic_inlay_id_gemstone() || task_info->is_logic_inlay_level_gemstone()) {
    	 task_listen_inlay_gemstone();
     }

     task_info = find_task(task_id);
     if(!task_info) {
    	 return 0;
     }

     if(task_info->is_logic_equip_purple_ring()) {
    	 player_self()->task_listen_equip_purple_ring();
     }

	return 0;
}

void Tasker::process_add_buf(uint32_t task_id) {
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
    if (NULL == task_json) {
    	return;
    }

    if(task_json->accept.buf_op == 2 && task_json->accept.buff > 0) {
    	player_self()->logic_del_status(task_json->accept.buff);
    	MSG_81000071 acv_msg;
    	acv_msg.type = 0;
    	Status_Info_Msg sim;
    	sim.status_id = task_json->accept.buff;
    	sim.overlay = 0;
    	sim.status_owner = role_id();
    	acv_msg.buff_detail.push_back(sim);
    	THIS_SEND_TO_CLIENT(acv_msg);
    }

    if(task_json->accept.buf_op == 1 && task_json->accept.buff > 0) {
    	Status_Effect_Argv argv;
    	argv.caster_id = player_self()->role_id();
    	argv.status_id = task_json->accept.buff;
    	argv.last_time_ms = (task_json->accept.buf_ms != 0) ? task_json->accept.buf_ms : -1;
    	argv.status_from = 1;

    	Effect_Argv_Prop effect_prop;
    	argv.data.push_back(effect_prop);

    	player_self()->logic_insert_status(argv);
    }
}

void Tasker::process_transfer_point(uint32_t task_id) {
    const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
    if (!task_cfg) {
    	return;
    }

    if(task_cfg->scene_id) {
    	tasker_detail_->scene_id_set.insert(task_cfg->scene_id);
    	this->save_tick();
    	MSG_81110014 msg;
    	msg.scene_id = task_cfg->scene_id;
    	THIS_SEND_TO_CLIENT(msg);
    }
}

void Tasker::process_safe_guide(uint32_t task_id) {
	const Safe_Guide_Config_Map &safe_guide_config_map = CONFIG_CACHE_TASK->get_safe_guide_config_map();
	for (Safe_Guide_Config_Map::const_iterator it = safe_guide_config_map.begin(); it != safe_guide_config_map.end(); ++it) {
		{
			uint32_t safe_level = it->second.safe_level;
			uint32_t safe_task  = it->second.safe_task;
			if (safe_level != 0 && player_self()->level() >= (int)safe_level)
				goto pass;
			if (safe_task != 0 && task_id == safe_task)
				goto pass;
			continue;
		}
pass:
		switch(it->second.safe_type) {
			case SAFE_GUIDE_TYPE_LEARN_ROLE_SKILL:
			{
				break;
			}
			case SAFE_GUIDE_TYPE_HERO_TO_LEVEL:
			{
				if(!player_self()->has_hero_id((int)it->second.safe_id)) {
					int ret = player_self()->req_hero_recruit((int)it->second.safe_id, 0);
					if(0 == ret) {
						int index = player_self()->heroer_get_hero_index((int)it->second.safe_id);
						player_self()->req_hero_set_fight(index, 1, false);
						int level_up_flag = 0;
						while(level_up_flag == 0 && player_self()->heroer_detail().hero_base_detail_.hero_level < (int)it->second.param1) {
							level_up_flag = player_self()->req_hero_level_up(index);
						}
					}
				}
				else {
					int index = player_self()->heroer_get_hero_index((int)it->second.safe_id);
					int level_up_flag = 0;
					while(level_up_flag == 0 && player_self()->heroer_detail().hero_base_detail_.hero_level < (int)it->second.param1) {
						level_up_flag = player_self()->req_hero_level_up(index);
					}
				}
				break;
			}
			case SAFE_GUIDE_TYPE_GET_HERO:
			{
				if(!player_self()->has_hero_id((int)it->second.safe_id)) {
					int ret = player_self()->req_hero_recruit((int)it->second.safe_id, 0);
					if(0 == ret) {
						int index = player_self()->heroer_get_hero_index((int)it->second.safe_id);
						player_self()->req_hero_set_fight(index, 1, false);
					}
				}
				break;
			}
			case SAFE_GUIDE_TYPE_GET_ITEM:
			{
				const int pack_item_amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,
						(uint32_t)it->second.safe_id);
				if(pack_item_amount <= 0) {
					if(CONFIG_CACHE_ITEM->find_item((int)it->second.safe_id)) {
						Item_Detail item((uint32_t)it->second.safe_id, 1, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
						player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
								Gain_Item_DM_Info(Pack::ITEM_GAIN_SAFE_GUIDE));
						MSG_81000102 acv_msg;
						Item_Basic_Info item_basic;
						item.set_item_basic_info(item_basic);
						acv_msg.item_info.push_back(item_basic);
						Card_Info card_info;
						if (item.set_item_card_info(card_info)) {
							acv_msg.card_info_vec.push_back(card_info);
						}
					    if (!acv_msg.item_info.empty()) {
					    	THIS_SEND_TO_CLIENT(acv_msg);
					    }
					}
				}
				break;
			}
			case SAFE_GUIDE_TYPE_OPEN_NEW_FUNCTION_ICON:
			{
				if(!player_self()->is_icon_open((uint16_t)it->second.safe_id)) {
					player_self()->safe_guide_open_icon((uint16_t)it->second.safe_id);
				}
				break;
			}
			case SAFE_GUIDE_TYPE_HERO_SET_FIGHT: {
				if(!player_self()->has_hero_id((int)it->second.safe_id)) {
					int ret = player_self()->req_hero_recruit((int)it->second.safe_id, 0);
					if(0 == ret) {
						int index = player_self()->heroer_get_hero_index((int)it->second.safe_id);
						player_self()->req_hero_set_fight(index, 1, false);
					}
				}
				else {
					int index = player_self()->heroer_get_hero_index((int)it->second.safe_id);
					if(player_self()->heroer_detail().fighter_index != index) {
						player_self()->req_hero_set_fight(index, 1, false);
					}
				}

				break;
			}
			case SAFE_GUIDE_TYPE_WAPE_UP_HERO_TO_QUALITY: {
				if(!player_self()->has_hero_id((int)it->second.safe_id)) {
					int ret = player_self()->req_hero_recruit((int)it->second.safe_id, 0);
					if(0 == ret) {
						int index = player_self()->heroer_get_hero_index((int)it->second.safe_id);
						Heroer_Detail::Hero_Map::const_iterator it_hero = player_self()->heroer_detail().hero_map.find(index);
						if (it_hero != player_self()->heroer_detail().hero_map.end()) {
							int status = 0;
							while (!status && (it_hero->second.awake_lvl + 1) < (int32_t)it->second.param1) {
								status = player_self()->req_hero_awake(1, index);
							}
						}
					}
				}
				else {
					int index = player_self()->heroer_get_hero_index((int)it->second.safe_id);
					Heroer_Detail::Hero_Map::const_iterator it_hero = player_self()->heroer_detail().hero_map.find(index);
					if (it_hero != player_self()->heroer_detail().hero_map.end()) {
						int status = 0;
						while (!status && (it_hero->second.awake_lvl + 1) < (int32_t)it->second.param1) {
							status = player_self()->req_hero_awake(1, index);
						}
					}
				}
				break;
			}

			default:
				continue;
		}
	}
}


Int_Int_Vec Tasker::get_item_list(const int npc_id) {
	Int_Int_Vec item_list;
	Npc_Item_Chance_Set_Map::iterator it = this->scene_npc_ItemIdSet_map_.find(npc_id);
   if (it != this->scene_npc_ItemIdSet_map_.end()) {
	   const Item_Chance_Set &id_set = this->scene_npc_ItemIdSet_map_[npc_id];
//    	item_list.assign(id_set.begin(), id_set.end());
	   for(Item_Chance_Set::const_iterator set_it = id_set.begin(); set_it != id_set.end(); ++set_it) {
		   Int_Int temp_item_chance;
		   temp_item_chance.reset();
		   temp_item_chance.val_1 = set_it->val_1;
		   temp_item_chance.val_2 = set_it->val_2;
		   item_list.push_back(temp_item_chance);
	   }
    }
	return item_list;
}

int Tasker::test_process_finish_all_task_befor_id(uint32_t task_id) {
	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task((int)task_id);
	if(!task_cfg) {
		MSG_USER("task_cfg is nul, task=%d", (int)task_id);
		return ERROR_TASK_NOT_EXIST;
	}

	if(task_cfg->task_type != TaskInfo::TASK_GTYPE_MAINLINE) {
		MSG_USER("not main line task, task=%d", (int)task_id);
		return ERROR_TASK_NOT_MAINLINE;
	}

	std::list<uint32_t> test_task_id_list;
	test_task_id_list.clear();

	const Task_Mission_Detail_Config *prev_task_cfg = CONFIG_CACHE_TASK->find_task((int)task_cfg->precondition);
	while(prev_task_cfg) {
		if(prev_task_cfg->task_type != TaskInfo::TASK_GTYPE_MAINLINE) {
			MSG_USER("prev task not main line task, task=%d", prev_task_cfg->task_id);
			return ERROR_PERV_TASK_NOT_MAINLINE;
		}

		if(tasker_detail_->task_submited_once_.find(prev_task_cfg->task_id) != tasker_detail_->task_submited_once_.end()) {
			break;
		}

		test_task_id_list.push_back((uint32_t)prev_task_cfg->task_id);

		prev_task_cfg = CONFIG_CACHE_TASK->find_task((int)prev_task_cfg->precondition);
	}

	for(std::list<uint32_t>::iterator it = test_task_id_list.begin(); it != test_task_id_list.end(); ++it) {
		MSG_DEBUG("    -------: task_id=%d", (int)(*it));
	}

	std::list<uint32_t>::reverse_iterator it = test_task_id_list.rbegin();
	if(it == test_task_id_list.rend()) {
		MSG_USER("all perv task finished");
		return ERROR_ALL_PERV_TASK_FINISHED;
	}

	for(std::list<uint32_t>::reverse_iterator it = test_task_id_list.rbegin(); it != test_task_id_list.rend(); ++it) {
		uint32_t temp_task_id = *it;
    	task_cfg = CONFIG_CACHE_TASK->find_task(temp_task_id);
    	if(!task_cfg) {
    		MSG_USER("task_cfg is null");
    		return ERROR_TASK_NOT_EXIST;
    	}

		TaskInfo *task_info = find_task(temp_task_id);
		if(!task_info) {
			insert_task(temp_task_id, *task_cfg);
		}

		task_info = find_task(temp_task_id);
		if(!task_info) {
			return ERROR_INSERT_TASK_FAIL;
		}

		process_accept_imp(temp_task_id, true);

		if(task_id != temp_task_id) {
			task_info = find_task(task_id);
			if(task_info) {
				task_info->__is_req_fast_finish = true;

			   set_task_status(*task_info, TaskInfo::TASK_STATUS_FINISH);
			   save_tick();

			    Time_Value now = Time_Value::gettimeofday();
				Log_Task_Stream(player_self()->role_id(), task_info->__task_id,
												TASK_STREAM_FINISH, player_self()->level(),
												now.sec());

				//通知场景不能继续采集任务物品和收集打怪掉落物品
				erase_task_listen_item_source(task_info);

				this->process_submit(temp_task_id);
			}
		}
	}

	test_task_id_list.clear();
	return 0;
}

bool Tasker::is_last_magic_wing_task_submit() {
	const Task_Config &task_cfg = CONFIG_CACHE_TASK->get_task_config();
	for(Task_Mission_Config::const_iterator it = task_cfg.missions.begin(); it != task_cfg.missions.end(); ++it) {
		if(!it->second.is_magic_wing) {
			continue;
		}

		if(it->second.postcondition != 0 || it->second.postcondition2 != 0 || it->second.postcondition3 != 0) {
			continue;
		}

		int status = task_state(it->first);
		if(status == TaskInfo::TASK_STATUS_SUBMIT) {
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

void Tasker::task_listen_vip_level_change(uint32_t new_vip_level) {
	const Task_Config &task_cfg = CONFIG_CACHE_TASK->get_task_config();
	for(Task_Mission_Config::const_iterator task_cfg_it = task_cfg.missions.begin();
			task_cfg_it != task_cfg.missions.end();
			++task_cfg_it) {
		if(task_cfg_it->second.task_type != TaskInfo::TASK_GTYPE_DAY
				|| !task_cfg_it->second.day_group
				|| !task_cfg_it->second.vip_accept_level) {
			continue;
		}

		if(task_cfg_it->second.day_group != 0 && !is_day_group_exist(task_cfg_it->second.day_group)) {
			continue;
		}

		if(task_cfg_it->second.vip_accept_level > 0 && player_self()->vip() != task_cfg_it->second.vip_accept_level) {
			continue;
		}

		if(tasker_detail_->task_map_.find(task_cfg_it->first) != tasker_detail_->task_map_.end()
				|| tasker_detail_->task_submited_once_.find(task_cfg_it->first) != tasker_detail_->task_submited_once_.end()) {
			continue;
		}

		if(task_cfg_it->second.accept.level && player_self()->level() < task_cfg_it->second.accept.level) {
			continue;
		}

		if(task_cfg_it->second.accept.level_max && player_self()->level() > task_cfg_it->second.accept.level_max) {
			continue;
		}

		if (task_cfg_it->second.precondition > 0
				&& this->tasker_detail_->task_submited_once_.find( task_cfg_it->second.precondition ) == this->tasker_detail_->task_submited_once_.end()) {
			continue;
		}

		TaskInfo *task_info = this->find_task(task_cfg_it->second.task_id);
		if(task_info) {
			continue;
		}

		if(task_cfg_it->first > 0 && task_submited_once_set().find(task_cfg_it->first) == task_submited_once_set().end()) {
			const Task_Mission_Detail_Config *task_mission_cfg = CONFIG_CACHE_TASK->find_task(task_cfg_it->first);
			if(task_mission_cfg) {
				insert_task(task_cfg_it->first, *task_mission_cfg);
			}
		}
	}//end for
}

bool Tasker::is_day_group_exist(const uint32_t group_id) {
	const Day_Week_Task_Config &day_week_task_cfg = CONFIG_CACHE_TASK->get_day_task_config();
	for(Day_Group_Map::const_iterator map_it = day_week_task_cfg.day_group_map.begin();
			map_it != day_week_task_cfg.day_group_map.end();
			++map_it) {
		for(Day_Group_Set::const_iterator set_it = map_it->second.begin();
				set_it != map_it->second.end();
				++set_it) {
			if(group_id == *set_it) {
				return true;
			}
		}
	}
	return false;
}

int Tasker::req_commit_item(uint32_t task_id) {
	TaskInfo *task_info = find_task(task_id);
	if(!task_info) {
		return ERROR_TASK_NOT_EXIST;
	}

	this->process_finish_imp(task_info);

	return 0;
}

void Tasker::correct_task_cfg_execute_type() {
	const Task_Mission_Detail_Config *task_cfg = 0;
	//normal task
	{
		TaskIdSet temp_task_set;
		temp_task_set.clear();

		for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
			temp_task_set.insert(it->first);
		}

		for(TaskIdSet::const_iterator it = temp_task_set.begin(); it != temp_task_set.end(); ++it) {
			int task_id = *it;
			task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
			if(!task_cfg) {
				MSG_USER("delete task, becase task_cfg not exist, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
				//delete task becase it not exist in cfg,if not , errors will happen
				tasker_detail_->task_map_.erase(task_id);
				continue;
			}

			TaskInfo *task_info = find_task(task_id);
			if(!task_info) {
				MSG_USER("task_info not exist, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
				continue;
			}

			bool is_not_match = false;
		    if(task_cfg->execute.size() != task_info->__condition_list.size()) {
		    	MSG_USER("task execue'size not match, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
		    	is_not_match = true;
		    }

			for (size_t i = 0; i < task_cfg->execute.size(); ++i) {
		        const TaskCondition *task_cond = task_info->task_condition(i);
		        if (!task_cond)
		        {
		        	MSG_USER("task_cond is null, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
		            is_not_match = true;
		            break;
		        }

		        if(!is_task_execute_type_match(task_cfg->execute[i].type, task_cond->__type)) {
		        	MSG_USER("task_cond'type not match, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
		        	MSG_USER("task_cond->__type=%d, task_cfg->execute[i].type=%d", task_cond->__type, task_cfg->execute[i].type);
		        	is_not_match = true;
		        	break;
		        }
			}//end for

			if(is_not_match) {
				init_task_condition(*task_info, *task_cfg);
			}
		}// end for

		temp_task_set.clear();
	}//end normal task

	//reward task
	{
		TaskIdSet reward_task_set;
		reward_task_set.clear();
		for(RewardTaskMap::const_iterator it = tasker_detail_->reward_task_map.begin();
				it != tasker_detail_->reward_task_map.end();
				++it) {
			reward_task_set.insert(it->first);
		}

		for(TaskIdSet::const_iterator it = reward_task_set.begin();
				it != reward_task_set.end();
				++it) {
			int task_id = *it;
			task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
			if(!task_cfg) {
				MSG_USER("delete task, becase task_cfg not exist, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
				//delete task becase it not exist in cfg,if not , errors will happen
				tasker_detail_->reward_task_map.erase(task_id);
				continue;
			}

			TaskInfo *task_info = find_task(task_id);
			if(!task_info) {
				MSG_USER("task_info not exist, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
				continue;
			}

			bool is_not_match = false;
		    if(task_cfg->execute.size() != task_info->__condition_list.size()) {
		    	MSG_USER("task execue'size not match, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
		    	is_not_match = true;
		    }

			for (size_t i = 0; i < task_cfg->execute.size(); ++i) {
		        const TaskCondition *task_cond = task_info->task_condition(i);
		        if (!task_cond)
		        {
		        	MSG_USER("task_cond is null, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
		            is_not_match = true;
		            break;
		        }

		        if(!is_task_execute_type_match(task_cfg->execute[i].type, task_cond->__type)) {
		        	MSG_USER("task_cond'type not match, task_id=%d, role_name=%s", task_id, player_self()->role_name().c_str());
		        	MSG_USER("task_cond->__type=%d, task_cfg->execute[i].type=%d", task_cond->__type, task_cfg->execute[i].type);
		        	is_not_match = true;
		        	break;
		        }
			}//end for

			if(is_not_match) {
				init_task_condition(*task_info, *task_cfg);
			}

		}//end for

		reward_task_set.clear();
	}//end reward task

	this->save_tick();
}

bool Tasker::is_main_task_commit(int task_id) {
	if(tasker_detail_->task_submited_once_.find(task_id) != tasker_detail_->task_submited_once_.end()) {
		return true;
	}

	return false;
}

void Tasker::correct_task_status_and_execute_progress() {
	//normal task
	{
		TaskIdSet temp_task_set;
		temp_task_set.clear();

		for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
			temp_task_set.insert(it->first);
		}

		for(TaskIdSet::const_iterator it = temp_task_set.begin(); it != temp_task_set.end(); ++it) {
			int task_id = *it;
			process_task_listen_when_accepted(task_id);
		}

		temp_task_set.clear();
	}// end normal task

	//reward task
	{
		TaskIdSet reward_task_set;
		reward_task_set.clear();
		for(RewardTaskMap::const_iterator it = tasker_detail_->reward_task_map.begin();
				it != tasker_detail_->reward_task_map.end();
				++it) {
			reward_task_set.insert(it->first);
		}

		for(TaskIdSet::const_iterator it = reward_task_set.begin();
				it != reward_task_set.end();
				++it) {
			int task_id = *it;
			process_task_listen_when_accepted(task_id);
		}

		reward_task_set.clear();
	}// end reward task

	this->save_tick();
}

void Tasker::correct_ring_task_error_cfg() {
	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	bool is_have_ring_task = false;
	bool is_have_open_ring_task = false;
	bool is_have_close_ring_task = false;

	for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
		if(it->second.__game_type == TaskInfo::TASK_GTYPE_RING_TASK) {
			is_have_ring_task = true;
			continue;
		}

		if(it->second.__game_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK) {
			is_have_open_ring_task = true;
			continue;
		}

		if(it->second.__game_type == TaskInfo::TASK_GTYPE_CLOSE_RING_TASK) {
			is_have_close_ring_task = true;
			continue;
		}
	}

	if(tasker_detail_->ring_num > 0 && tasker_detail_->ring_num < ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		if(!is_have_ring_task) {
			--tasker_detail_->ring_num;
			refresh_ring_task();
		}
		return;
	}

	if(tasker_detail_->ring_num == 0
			&&  player_self()->level() >= (int)ring_task_all_cfg.ring_task_base_cfg.open_level) {
		if(!is_have_open_ring_task) {
			refresh_open_ring_task();
		}
		return;
	}

	if(tasker_detail_->ring_num >= ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		if(!tasker_detail_->is_submit_close_ring_task && !is_have_close_ring_task) {
			refresh_close_ring_task();
		}
		return;
	}
}

bool Tasker::is_task_execute_type_match(const int cfg_execute_type, const int db_cond_type) {
	int temp_cond_type = 0;

    switch(cfg_execute_type) {
    case TaskExecType::TASK_EXEC_KILL_MONSTER:
        temp_cond_type = TaskCondition::TASK_CT_KILL_MONSTER;
        break;

    case TaskExecType::TASK_EXEC_FB:
    	temp_cond_type = TaskCondition::TASK_CT_FB;
    	break;

    case TaskExecType::TASK_EXEC_NPC_TALK:
        temp_cond_type = TaskCondition::TASK_CT_NPC_TALK;
    	break;

    case TaskExecType::TASK_EXEC_ARRIVE:
        temp_cond_type = TaskCondition::TASK_CT_ARRIVE;
    	break;

    case TaskExecType::TASK_EXEC_LVL:
        temp_cond_type = TaskCondition::TASK_CT_LVL;
    	break;

    case TaskExecType::TASK_EXEC_COLLECT:
        temp_cond_type = TaskCondition::TASK_CT_COLLECT;
    	break;

    case TaskExecType::TASK_EXEC_KILL_MONSTER_GROUP:
        temp_cond_type = TaskCondition::TASK_CT_KILL_MONSTER_GROUP;
    	break;

    case TaskExecType::TASK_EXEC_PACKAGE:
    	temp_cond_type = TaskCondition::TASK_CT_PACKAGE;
    	break;

    case TaskExecType::TASK_EXEC_FB_TYPE:
    	temp_cond_type = TaskCondition::TASK_CT_FB_TYPE;
    	break;

    case TaskExecType::TASK_EXEC_WIN_PLAYER:
    	temp_cond_type = TaskCondition::TASK_CT_WIN_PLAYER;
    	break;

    case TaskExecType::TASK_EXEC_JOIN_GANG:
    	temp_cond_type = TaskCondition::TASK_CT_JOIN_GANG;
    	break;

    case TaskExecType::TASK_EXEC_GANGER_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_GANGER_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_PLAYER_ALL_EQUIP:
    	temp_cond_type = TaskCondition::TASK_CT_PLAYER_ALL_EQUIP;
    	break;

    case TaskExecType::TASK_EXEC_BODY_EQUIP:
    	temp_cond_type = TaskCondition::TASK_CT_BODY_EQUIP;
    	break;

    case TaskExecType::TASK_EXEC_BODY_EQUIP_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_BODY_EQUIP_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_EQUIP_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_EQUIP_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_STONE_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_STONE_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_BUY_GOODS:
    	temp_cond_type = TaskCondition::TASK_CT_BUY_GOODS;
    	break;

    case TaskExecType::TASK_EXEC_GET_HERO:
    	temp_cond_type = TaskCondition::TASK_CT_GET_HERO;
    	break;

    case TaskExecType::TASK_EXEC_FIGHT:
    	temp_cond_type = TaskCondition::TASK_CT_FIGHT;
    	break;

    case TaskExecType::TASK_EXEC_COMMIT_CONSUME_ITEM:
    	temp_cond_type = TaskCondition::TASK_CT_COMMIT_CONSUME_ITEM;
    	break;

    case TaskExecType::TASK_EXEC_DELIVER_LETTER:
    	temp_cond_type = TaskCondition::TASK_CT_DELIVER_LETTER;
    	break;

    case TaskExecType::TASK_EXEC_HELP_OTHER_COMPLETE_TEAM_SCENE:
    	temp_cond_type = TaskCondition::TASK_CT_HELP_OTHER_COMPLETE_TEAM_SCENE;
    	break;

    case TaskExecType::TASK_EXEC_SELL_ITEM:
    	temp_cond_type = TaskCondition::TASK_CT_SELL_ITEM;
    	break;

    case TaskExecType::TASK_EXEC_ARENA:
    	temp_cond_type = TaskCondition::TASK_CT_ARENA;
    	break;

    case TaskExecType::TASK_EXEC_CHAT_WITH_FRIEND:
    	temp_cond_type = TaskCondition::TASK_CT_CHAT_WITH_FRIEND;
    	break;

    case TaskExecType::TASK_EXEC_SEND_MAIL:
    	temp_cond_type = TaskCondition::TASK_CT_SEND_MAIL;
    	break;

    case TaskExecType::TASK_EXEC_TEAM_SCENE_OR_PVP:
    	temp_cond_type = TaskCondition::TASK_CT_TEAM_SCENE_OR_PVP;
    	break;

    case TaskExecType::TASK_EXEC_BROWSE_OTHER_PLAYER_INFO:
    	temp_cond_type = TaskCondition::TASK_CT_BROWSE_OTHER_PLAYER_INFO;
    	break;

    case TaskExecType::TASK_EXEC_ARENA_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_ARENA_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_EQUIP_CRIMSON_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_EQUIP_CRIMSON_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_ARENA_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_ARENA_ALL;
    	break;

    case TaskExecType::TASK_EXEC_CREATE_DRAGON_VALE_BUILDING:
    	temp_cond_type = TaskCondition::TASK_CT_CREATE_DRAGON_VALE_BUILDING;
    	break;

    case TaskExecType::TASK_EXEC_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION:
    	temp_cond_type = TaskCondition::TASK_CT_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION;
    	break;

    case TaskExecType::TASK_EXEC_WAKE_UP_HERO_TO_QUALITY_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_WAKE_UP_HERO_TO_QUALITY_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_KEEP_HERO_TO_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_KEEP_HERO_TO_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_COMPOSE_EQUIP:
    	temp_cond_type = TaskCondition::TASK_CT_COMPOSE_EQUIP;
    	break;

    case TaskExecType::TASK_EXEC_HERO_DREAM_SCENE:
    	temp_cond_type = TaskCondition::TASK_CT_HERO_DREAM_SCENE;
    	break;

    case TaskExecType::TASK_EXEC_GANGER_SKILL_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_GANGER_SKILL_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_WAR:
    	temp_cond_type = TaskCondition::TASK_CT_WAR;
    	break;

    case TaskExecType::TASK_EXEC_WAR_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_WAR_ALL;
    	break;

    case TaskExecType::TASK_EXEC_KILL_ENEMY_IN_WAR:
    	temp_cond_type = TaskCondition::TASK_CT_KILL_ENEMY_IN_WAR;
    	break;

    case TaskExecType::TASK_EXEC_SINGLE_ARENA_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_SINGLE_ARENA_ALL;
    	break;

    case TaskExecType::TASK_EXEC_SINGLE_ARENA_WIN:
    	temp_cond_type = TaskCondition::TASK_CT_SINGLE_ARENA_WIN;
    	break;

    case TaskExecType::TASK_EXEC_TEAM_ARENA_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_TEAM_ARENA_ALL;
    	break;

    case TaskExecType::TASK_EXEC_TEAM_ARENA_WIN:
    	temp_cond_type = TaskCondition::TASK_CT_TEAM_ARENA_WIN;
    	break;

    case TaskExecType::TASK_EXEC_GANGER_BLESS_BUF:
    	temp_cond_type = TaskCondition::TASK_CT_GANGER_BLESS_BUF;
    	break;

    case TaskExecType::TASK_EXEC_ROB_OTHER_DRAGON_VALE_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_ROB_OTHER_DRAGON_VALE_ALL;
    	break;

    case TaskExecType::TASK_EXEC_DRAGON_VALE_STEP_FOOT_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_DRAGON_VALE_STEP_FOOT_ALL;
    	break;

    case TaskExecType::TASK_EXEC_ACTIVENESS_OF_TODAY:
    	temp_cond_type =  TaskCondition::TASK_CT_ACTIVENESS_OF_TODAY;
    	break;

    case TaskExecType::TASK_EXEC_DRAGON_VALE_BUILDING_UPGRADE:
    	temp_cond_type = TaskCondition::TASK_CT_DRAGON_VALE_BUILDING_UPGRADE;
    	break;

    case TaskExecType::TASK_EXEC_LIMIT_TIME_KILL_BOSS:
    	temp_cond_type = TaskCondition::TASK_CT_LIMIT_TIME_KILL_BOSS;
    	break;

    case TaskExecType::TASK_EXEC_DRAGON_KEEP_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_DRAGON_KEEP_ALL;
    	break;

    case TaskExecType::TASK_EXEC_DRAGON_KEEP_SUCCESS:
    	temp_cond_type = TaskCondition::TASK_CT_DRAGON_KEEP_SUCCESS;
    	break;

    case TaskExecType::TASK_EXEC_DONATE_TO_GANG:
    	temp_cond_type = TaskCondition::TASK_CT_DONATE_TO_GANG;
    	break;

    case TaskExecType::TASK_EXEC_PRESENT_BUF_TO_OTHER:
    	temp_cond_type = TaskCondition::TASK_CT_PRESENT_BUF_TO_OTHER;
    	break;

    case TaskExecType::TASK_EXEC_FINISH_REWARD_TASK_NUM:
    	temp_cond_type = TaskCondition::TASK_CT_FINISH_REWARD_TASK_NUM;
    	break;

    case TaskExecType::TASK_EXEC_KEEP_DRAGON_TO_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_KEEP_DRAGON_TO_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_KNIGHT_TRIAL:
    	temp_cond_type = TaskCondition::TASK_CT_KNIGHT_TRIAL;
    	break;

    case TaskExecType::TASK_EXEC_SUIT:
    	temp_cond_type = TaskCondition::TASK_CT_SUIT;
    	break;

    case TaskExecType::TASK_EXEC_ROLE_PULL_ON_EQUIP:
    	temp_cond_type = TaskCondition::TASK_CT_ROLE_PULL_ON_EQUIP;
    	break;

    case TaskExecType::TASK_EXEC_HERO_PULL_ON_EQUIP:
    	temp_cond_type = TaskCondition::TASK_CT_HERO_PULL_ON_EQUIP;
    	break;

    case TaskExecType::TASK_EXEC_ACTIVATE_WING:
    	temp_cond_type = TaskCondition::TASK_CT_ACTIVATE_WING;
    	break;

    case TaskExecType::TASK_EXEC_GET_QUALITY_RUNE:
    	temp_cond_type = TaskCondition::TASK_CT_GET_QUALITY_RUNE;
    	break;

    case TaskExecType::TASK_EXEC_PASS_KNGIHT_TRIALS:
    	temp_cond_type = TaskCondition::TASK_CT_PASS_KNGIHT_TRIALS;
    	break;

    case TaskExecType::TASK_EXEC_HAVE_HERO:
    	temp_cond_type = TaskCondition::TASK_CT_HAVE_HERO;
    	break;

    case TaskExecType::TASK_EXEC_HAVE_HERO_QUALITY:
    	temp_cond_type = TaskCondition::TASK_CT_HAVE_HERO_QUALITY;
    	break;

    case TaskExecType::TASK_EXEC_FIRST_PASS_SCENE:
    	temp_cond_type = TaskCondition::TASK_CT_FIRST_PASS_SCENE;
    	break;

    case TaskExecType::TASK_EXEC_DRAGON_BORN:
    	temp_cond_type = TaskCondition::TASK_CT_DRAGON_BORN;
    	break;

    case TaskExecType::TASK_EXEC_HORSE_STAR_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_HORSE_STAR_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_HAVE_ITEM_BUT_NO_ERASE:
    	temp_cond_type = TaskCondition::TASK_CT_HAVE_ITEM_BUT_NO_ERASE;
    	break;

    case TaskExecType::TASK_EXEC_HAVE_EQUIP_BUT_NO_ERASE:
    	temp_cond_type = TaskCondition::TASK_CT_HAVE_EQUIP_BUT_NO_ERASE;
    	break;

    case TaskExecType::TASK_EXEC_SAME_GANG_CHALLENGE_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_SAME_GANG_CHALLENGE_ALL;
    	break;

    case TaskExecType::TASK_EXEC_MAIN_SCENE_NPC_BATTLE:
    	temp_cond_type = TaskCondition::TASK_CT_MAIN_SCENE_NPC_BATTLE;
    	break;

    case TaskExecType::TASK_EXEC_EQUIP_CARD_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_EQUIP_CARD_ALL;
    	break;

    case TaskExecType::TASK_EXEC_EQUIP_CARD_BY_POSITION:
    	temp_cond_type = TaskCondition::TASK_CT_EQUIP_CARD_BY_POSITION;
    	break;

    case TaskExecType::TASK_EXEC_CARD_UPGRADE_TO_LEVEL_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_CARD_UPGRADE_TO_LEVEL_ALL;
    	break;

    case TaskExecType::TASK_EXEC_CARD_UPGRADE_TO_LEVEL_BY_POSITION:
    	temp_cond_type = TaskCondition::TASK_CT_CARD_UPGRADE_TO_LEVEL_BY_POSITION;
    	break;

    case TaskExecType::TASK_EXEC_LOTTERY_DRAW:
    	temp_cond_type = TaskCondition::TASK_CT_LOTTERY_DRAW;
    	break;

    case TaskExecType::TASK_EXEC_EQUIP_JEWELRY:
    	temp_cond_type = TaskCondition::TASK_CT_EQUIP_JEWELRY;
    	break;

    case TaskExecType::TASK_EXEC_INVITE_FRIEND_STEP_FOOT:
    	temp_cond_type = TaskCondition::TASK_CT_INVITE_FRIEND_STEP_FOOT;
    	break;

    case TaskExecType::TASK_EXEC_KILL_GROUP_NUM:
    	temp_cond_type = TaskCondition::TASK_CT_KILL_GROUP_NUM;
    	break;

    case TaskExecType::TASK_EXEC_DID_REWARD_TASK_NUM:
    	temp_cond_type = TaskCondition::TASK_CT_DID_REWARD_TASK_NUM;
    	break;

    case TaskExecType::TASK_EXEC_ALCHEMY_NUM:
    	temp_cond_type = TaskCondition::TASK_CT_ALCHEMY_NUM;
    	break;

    case TaskExecType::TASK_EXEC_DID_REWARD_TASK_NUM_TODAY:
    	temp_cond_type = TaskCondition::TASK_CT_DID_REWARD_TASK_NUM_TODAY;
    	break;

    case TaskExecType::TASK_EXEC_REMOTE_SCENE_ANY_PASS:
    	temp_cond_type = TaskCondition::TASK_CT_REMOTE_SCENE_ANY_PASS;
    	break;

    case TaskExecType::TASK_EXEC_KNIGHT_TRIAL_ANY_PASS:
    	temp_cond_type = TaskCondition::TASK_CT_KNIGHT_TRIAL_ANY_PASS;
    	break;

    case TaskExecType::TASK_EXEC_COMPOSE_GEMSTONE_LEVEL:
    	temp_cond_type = TaskCondition::TASK_CT_COMPOSE_GEMSTONE_LEVEL;
    	break;

    case TaskExecType::TASK_EXEC_INLAY_POSITION_GEMSTONE:
    	temp_cond_type = TaskCondition::TASK_CT_INLAY_POSITION_GEMSTONE;
    	break;

    case TaskExecType::TASK_EXEC_INLAY_ANY_GEMSTONE:
    	temp_cond_type = TaskCondition::TASK_CT_INLAY_ANY_GEMSTONE;
    	break;

    case TaskExecType::TASK_EXEC_WORLD_BOSS:
    	temp_cond_type = TaskCondition::TASK_CT_WORLD_BOSS;
    	break;

    case TaskExecType::TASK_EXEC_WORLD_BOSS_IMMEDIATE_REVIVE:
    	temp_cond_type =  TaskCondition::TASK_CT_WORLD_BOSS_IMMEDIATE_REVIVE;
    	break;

    case TaskExecType::TASK_EXEC_ENCHANT_ANY_EQUIP:
    	temp_cond_type = TaskCondition::TASK_CT_ENCHANT_ANY_EQUIP;
    	break;

    case TaskExecType::TASK_EXEC_INLAY_ID_GEMSTONE:
    	temp_cond_type = TaskCondition::TASK_CT_INLAY_ID_GEMSTONE;
    	break;

    case TaskExecType::TASK_EXEC_GANG_WELL_RECOVER_SPIRIT:
    	temp_cond_type = TaskCondition::TASK_CT_GANG_WELL_RECOVER_SPIRIT;
    	break;

    case TaskExecType::TASK_EXEC_EQUIP_PURPLE_RING:
    	temp_cond_type = TaskCondition::TASK_CT_EQUIP_PURPLE_RING;
    	break;

    case TaskExecType::TASK_EXEC_INLAY_LEVEL_GEMSTONE:
    	temp_cond_type = TaskCondition::TASK_CT_INLAY_LEVEL_GEMSTONE;
    	break;

    case TaskExecType::TASK_EXEC_UPGRADE_CARD:
    	temp_cond_type = TaskCondition::TASK_CT_UPGRADE_CARD;
    	break;

    case TaskExecType::TASK_EXEC_DAYLY_SINGLE_ARENA_ALL:
    	temp_cond_type = TaskCondition::TASK_CT_DAYLY_SINGLE_ARENA_ALL;
    	break;

    default:
    	MSG_USER("error task condition type not register, %d", cfg_execute_type);
    	break;
    }

    if(db_cond_type == temp_cond_type) {
    	return true;
    }

    return false;
}

int Tasker::ring_task_construct(const Task_Mission_Detail_Config &cfg) {
	if(cfg.task_type != TaskInfo::TASK_GTYPE_RING_TASK) {
		return 0;
	}

	if(cfg.accept.level != 0 && player_self()->level() < cfg.accept.level) {
		return 0;
	}

	if(cfg.accept.level_max != 0 && player_self()->level() > cfg.accept.level_max) {
		return 0;
	}

	if(cfg.min_ring_num != 0 && tasker_detail_->ring_num < cfg.min_ring_num) {
		return 0;
	}

	if(cfg.max_ring_num != 0 && tasker_detail_->ring_num > cfg.max_ring_num) {
		return 0;
	}

	if(cfg.ring_type != RING_TASK_TYPE_NPC_TALK
			&& cfg.ring_type != RING_TASK_TYPE_COLLECT
			&& cfg.ring_type != RING_TASK_TYPE_LIMIT_TIME_KILL_BOSS
			&& cfg.ring_type != RING_TASK_TYPE_KILL_MONSTER
			&& cfg.ring_type != RING_TASK_TYPE_COMMIT_CONSUME_ITEM
			&& cfg.ring_type != RING_TASK_TYPE_DRAGON_VALE_OP) {
		return 0;
	}

	tasker_detail_->ring_type_RingTaskIdSet_map[cfg.ring_type].insert(cfg.task_id);
	return 0;
}

bool Tasker::is_exist_open_ring_task() {
	for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
		if(it->second.__game_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK) {
			return true;
		}
	}
	return false;
}

void Tasker::ring_task_listen_player_level_change(const int new_level) {
	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	if(new_level < (int)ring_task_all_cfg.ring_task_base_cfg.open_level) {
		return;
	}

	if(tasker_detail_->ring_num > 0) {
		return;
	}

	if(is_exist_open_ring_task()) {
		return;
	}

	refresh_open_ring_task();
}

void Tasker::refresh_close_ring_task() {
	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	if(tasker_detail_->ring_num < ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		return;
	}

	TaskIdSet temp_set;
	for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
		if(it->second.__game_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_CLOSE_RING_TASK) {
			temp_set.insert(it->second.__task_id);
		}
	}

	for(TaskIdSet::const_iterator  it =  temp_set.begin(); it != temp_set.end(); ++it) {
		TaskInfo *task_info = find_task(*it);
		if(task_info) {
			notify_task_del(*task_info);
		}
		tasker_detail_->task_map_.erase(*it);
	}

	int task_id = 0;
	const Task_Config &all_task_cfg =  CONFIG_CACHE_TASK->get_task_config();
	for(Task_Mission_Config::const_iterator it = all_task_cfg.missions.begin(); it != all_task_cfg.missions.end(); ++it) {
		if(it->second.task_type == TaskInfo::TASK_GTYPE_CLOSE_RING_TASK) {
			task_id = it->second.task_id;
			break;
		}
	}

	if(!task_id) {
		MSG_TRACE_ABORT("task not exist in task config, task_type= %d", TaskInfo::TASK_GTYPE_CLOSE_RING_TASK);
		this->save_tick();
		temp_set.clear();
		return;
	}

	const Task_Mission_Detail_Config *task_cfg_ptr = CONFIG_CACHE_TASK->find_task(task_id);
	if(!task_cfg_ptr) {
		MSG_TRACE_ABORT("task not exist in task config, task_id=%d", task_id);
		this->save_tick();
		temp_set.clear();
		return;
	}

	insert_task(task_cfg_ptr->task_id, *task_cfg_ptr);

	this->save_tick();
	temp_set.clear();
}

void Tasker::refresh_open_ring_task() {
	tasker_detail_->ring_num = 0;
	tasker_detail_->is_diamond_recover = false;
	tasker_detail_->did_ring_num = 0;
	tasker_detail_->is_submit_close_ring_task = false;

	TaskIdSet temp_set;
	for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
		if(it->second.__game_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_CLOSE_RING_TASK) {
			temp_set.insert(it->second.__task_id);
		}
	}

	for(TaskIdSet::const_iterator  it =  temp_set.begin(); it != temp_set.end(); ++it) {
		TaskInfo *task_info = find_task(*it);
		if(task_info) {
			notify_task_del(*task_info);
		}
		tasker_detail_->task_map_.erase(*it);
	}

	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	if(player_self()->level() < (int)ring_task_all_cfg.ring_task_base_cfg.open_level) {
		this->save_tick();
		temp_set.clear();
		return;
	}

	int task_id = 0;
	const Task_Config &all_task_cfg =  CONFIG_CACHE_TASK->get_task_config();
	for(Task_Mission_Config::const_iterator it = all_task_cfg.missions.begin(); it != all_task_cfg.missions.end(); ++it) {
		if(it->second.task_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK) {
			task_id = it->second.task_id;
			break;
		}
	}

	if(!task_id) {
		MSG_TRACE_ABORT("task not exist in task config, task_type= %d", TaskInfo::TASK_GTYPE_OPEN_RING_TASK);
		this->save_tick();
		temp_set.clear();
		return;
	}

	const Task_Mission_Detail_Config *task_cfg_ptr = CONFIG_CACHE_TASK->find_task(task_id);
	if(!task_cfg_ptr) {
		MSG_TRACE_ABORT("task not exist in task config, task_id=%d", task_id);
		this->save_tick();
		temp_set.clear();
		return;
	}

	insert_task(task_cfg_ptr->task_id, *task_cfg_ptr);

	this->save_tick();
	temp_set.clear();
}

void Tasker::refresh_ring_task() {
	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	if(tasker_detail_->ring_num >= ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		return;
	}

	TaskIdSet temp_set;
	for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
		if(it->second.__game_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_CLOSE_RING_TASK) {
			temp_set.insert(it->second.__task_id);
		}
	}

	for(TaskIdSet::const_iterator  it =  temp_set.begin(); it != temp_set.end(); ++it) {
		TaskInfo *task_info = find_task(*it);
		if(task_info) {
			notify_task_del(*task_info);
		}
		tasker_detail_->task_map_.erase(*it);
	}

    ++tasker_detail_->ring_num;
    if(tasker_detail_->ring_num > ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
    	return;
    }

	task_construct(false);

	if(tasker_detail_->ring_task_id) {
		const Task_Mission_Detail_Config *task_cfg =  CONFIG_CACHE_TASK->find_task(tasker_detail_->ring_task_id);
		if(task_cfg) {
			Ring_Type_RingTaskIdSet_Map::iterator find_it =  tasker_detail_->ring_type_RingTaskIdSet_map.find(task_cfg->ring_type);
			if(find_it != tasker_detail_->ring_type_RingTaskIdSet_map.end()) {
				find_it->second.erase(tasker_detail_->ring_task_id);
				if(find_it->second.empty()) {
					tasker_detail_->ring_type_RingTaskIdSet_map.erase(find_it);
				}
			}
		}
	}

	if(tasker_detail_->ring_type_RingTaskIdSet_map.empty()) {
		MSG_TRACE_ABORT("ring_type_RingTaskIdSet_map is empty");
	}

	uint32_t num = 0;

	uint8_t ring_type = 0;
	Ring_Type_RingTaskIdSet_Map::const_iterator find_it = tasker_detail_->ring_type_RingTaskIdSet_map.end();
    while(1) {
    	++num;
    	if(num >= 50) {
    		MSG_TRACE_ABORT("num >= 50");
    		return;
    	}

    	if(num <= 10) {
        	ring_type = get_ring_task_type_by_ring_num(tasker_detail_->ring_num);
        	if(!ring_type) {
        		const uint8_t must_not_ring_type = get_must_not_ring_task_type_by_ring_num(tasker_detail_->ring_num);
        		ring_type = get_ring_task_type_by_chance();
        		if(must_not_ring_type && must_not_ring_type == ring_type) {
        			continue;
        		}
        	}
    	}
    	else {
    		ring_type = get_ring_task_type_by_chance();
    	}

    	find_it = tasker_detail_->ring_type_RingTaskIdSet_map.find(ring_type);

    	if(find_it == tasker_detail_->ring_type_RingTaskIdSet_map.end()) {
    		MSG_USER("no ring task type, ring_type=%d, role_name=%s, level=%d, ring_num=%d",
    				ring_type, player_self()->role_name().c_str(), player_self()->level(), tasker_detail_->ring_num);
    		continue;
    	}

    	if(find_it->second.empty()) {
    		MSG_USER("no ring task, ring_type=%d, role_name=%s, level=%d, ring_num=%d",
    				ring_type, player_self()->role_name().c_str(), player_self()->level(), tasker_detail_->ring_num);
    		continue;
    	}

    	if(find_it != tasker_detail_->ring_type_RingTaskIdSet_map.end()
    			&& !find_it->second.empty()) {
    		break;
    	}
    }

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
    const int index = random() % (find_it->second.size());
    int task_id = 0;
    int i = 0;

    for(RingTaskIdSet::const_iterator it = find_it->second.begin(); it != find_it->second.end(); ++it) {
    	if(index == i) {
    		task_id = *it;
    		break;
    	}
    	++i;
    }

    const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
    if(!task_cfg) {
    	MSG_TRACE_ABORT("task_cfg is null, task_id=%d", task_id);
    	--tasker_detail_->ring_num;
    	this->save_tick();
    	return;
    }

    insert_task(task_cfg->task_id, *task_cfg);
    tasker_detail_->ring_task_id = task_cfg->task_id;

    this->save_tick();
    temp_set.clear();
}

uint8_t Tasker::get_ring_task_type_by_chance() {
	const Ring_Task_All_Cfg &cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	int num = random() % 100;

	int curr_left_border = 0;
	int next_left_border = cfg.ring_task_base_cfg.npc_talk_chance;

	if(num >= curr_left_border && num <= (next_left_border - 1)) {
		return (uint8_t)RING_TASK_TYPE_NPC_TALK;
	}

	curr_left_border = next_left_border;
	next_left_border += cfg.ring_task_base_cfg.collect_chance;
	if(num >= curr_left_border && num <= (next_left_border - 1)) {
		return (uint8_t)RING_TASK_TYPE_COLLECT;
	}

	curr_left_border = next_left_border;
	next_left_border += cfg.ring_task_base_cfg.limit_time_kill_chance;
	if(num >= curr_left_border && num <= next_left_border - 1) {
		return (uint8_t)RING_TASK_TYPE_LIMIT_TIME_KILL_BOSS;
	}

	curr_left_border = next_left_border;
	next_left_border += cfg.ring_task_base_cfg.kill_monster_chance;
	if(num >= curr_left_border && num <= next_left_border - 1) {
		return (uint8_t)RING_TASK_TYPE_KILL_MONSTER;
	}

	curr_left_border = next_left_border;
	next_left_border += cfg.ring_task_base_cfg.commit_consume_item_chance;
	if(num >= curr_left_border && num <= next_left_border - 1) {
		return (uint8_t)RING_TASK_TYPE_COMMIT_CONSUME_ITEM;
	}

	curr_left_border = next_left_border;
	next_left_border += cfg.ring_task_base_cfg.dragon_vale_op_chance;
	if(num >= curr_left_border && num <= next_left_border - 1) {
		return (uint8_t)RING_TASK_TYPE_DRAGON_VALE_OP;
	}

	return (uint8_t)RING_TASK_TYPE_NPC_TALK;
}

int Tasker::req_recover_task_chain(const uint8_t type) {
	if(1 != type && 2 != type) {
		return ERROR_RECOVER_RING_TASK_CONSUME_TYPE_INVALID;
	}

	if(!tasker_detail_->ring_num) {
		return ERROR_NO_OPEN_RING_TASK_CAN_NOT_RECOVER;
	}

	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();

	//友情值恢复
	if(1 == type) {
		int ret = consume_money_in_tasker(FRIENDSHIP_ONLY, ring_task_all_cfg.ring_task_base_cfg.friend_ship,
				MONEY_SUB_MONEY_RECOVER_TASK_CHAIN_BY_FRIENDSHIP);
		if(0 != ret) {
			return ret;
		}
	}

	//钻石恢复
	if(2 == type) {
		if(tasker_detail_->is_diamond_recover) {
			return ERROR_PLEASE_USE_FRIENDSHIP_TO_RECOVER;
		}

		int ret = consume_money_in_tasker(GOLD_ONLY, ring_task_all_cfg.ring_task_base_cfg.diamond,
				MONEY_SUB_MONEY_RECOVER_TASK_CHAIN_BY_DIAMOND);
		if(0 != ret) {
			return ret;
		}

		tasker_detail_->is_diamond_recover = true;
	}

	TaskIdSet temp_set;
	for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
		if(it->second.__game_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_CLOSE_RING_TASK) {
			temp_set.insert(it->second.__task_id);
		}
	}

	for(TaskIdSet::const_iterator  it =  temp_set.begin(); it != temp_set.end(); ++it) {
		TaskInfo *task_info = find_task(*it);
		if(task_info) {
			notify_task_del(*task_info);
		}
		tasker_detail_->task_map_.erase(*it);
	}

	if(tasker_detail_->ring_num >= ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		tasker_detail_->ring_num = ring_task_all_cfg.ring_task_base_cfg.max_ring_num - 1;
	}

	refresh_ring_task();

	this->save_tick();

	MSG_50110016 msg;
	THIS_SEND_TO_CLIENT(msg);

	temp_set.clear();
	return 0;
}

void Tasker::test_update_ring_num(uint32_t ring_num) {
	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	if(ring_num <= 0 || ring_num > ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		return;
	}

	if(ring_num == ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		ring_num = ring_task_all_cfg.ring_task_base_cfg.max_ring_num - 1;
	}

	tasker_detail_->ring_num = ring_num;
	this->save_tick();
}

void Tasker::test_refresh_ring_task(uint32_t task_id) {
	if(!task_id) {
		return;
	}

	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_id);
	if(!task_cfg) {
		return;
	}

	if(task_cfg->task_type != TaskInfo::TASK_GTYPE_RING_TASK) {
		return;
	}

	if(task_cfg->accept.level != 0 && player_self()->level() < task_cfg->accept.level) {
		return;
	}

	if(task_cfg->accept.level_max != 0 && player_self()->level() > task_cfg->accept.level_max) {
		return;
	}

	if(task_cfg->min_ring_num != 0 &&  tasker_detail_->ring_num < task_cfg->min_ring_num) {
		return;
	}

	if(task_cfg->max_ring_num != 0 && tasker_detail_->ring_num > task_cfg->max_ring_num) {
		return;
	}

	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	if(tasker_detail_->ring_num >= ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		return;
	}

	TaskIdSet temp_set;
	for(TaskMap::const_iterator it = tasker_detail_->task_map_.begin(); it != tasker_detail_->task_map_.end(); ++it) {
		if(it->second.__game_type == TaskInfo::TASK_GTYPE_OPEN_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_RING_TASK
				|| it->second.__game_type == TaskInfo::TASK_GTYPE_CLOSE_RING_TASK) {
			temp_set.insert(it->second.__task_id);
		}
	}

	for(TaskIdSet::const_iterator  it =  temp_set.begin(); it != temp_set.end(); ++it) {
		TaskInfo *task_info = find_task(*it);
		if(task_info) {
			notify_task_del(*task_info);
		}
		tasker_detail_->task_map_.erase(*it);
	}

    ++tasker_detail_->ring_num;
    if(tasker_detail_->ring_num > ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
    	MSG_USER("ring_num=%d", tasker_detail_->ring_num);
    	return;
    }

	task_construct(false);

    insert_task(task_cfg->task_id, *task_cfg);

    this->save_tick();
    temp_set.clear();
}

int Tasker::consume_money_in_tasker(uint8_t money_type, uint32_t money_num, uint32_t lose_money_type) {
	//判断是否有足够的货币
	int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(
			                   Money_Sub_Type(money_type),
			                   money_num, Money_DM_Info(lose_money_type)));
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
			money_num, Money_DM_Info(lose_money_type))) != 0) {
		return ERROR_SUB_MONEY_FAIL;
	}

	return 0;
}

void Tasker::set_task_status(TaskInfo &task_info, const int status) {
	task_info.set_task_status(status);
}

uint32_t Tasker::get_did_ring_task_num() {
	return tasker_detail_->did_ring_num;
}

uint32_t Tasker::get_total_ring_task_num() {
	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	return ring_task_all_cfg.ring_task_base_cfg.max_ring_num;
}

void Tasker::hook_ring_task_complete(uint32_t task_id) {
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_RING_TASK); //TOD助手日常监听
}

void Tasker::notify_remain_reward_task_num() {
	MSG_81110012 msg;
	const Reward_Task_Config&  cfg = CONFIG_CACHE_REWARD_TASK->get_config();
	uint32_t max_accept_count = cfg.accept_max_count_per_day
								+ player_self()->achievement_reward_times()
								+ tasker_detail_->give_up_num
								+ ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS);
	msg.num = (max_accept_count > tasker_detail_->used_accept_count)
			? (max_accept_count - tasker_detail_->used_accept_count) : 0;
	THIS_SEND_TO_CLIENT(msg);
}

void Tasker::process_scene_npc(const TaskInfo &task_info) {
	const Task_Mission_Detail_Config *task_cfg =  CONFIG_CACHE_TASK->find_task(task_info.__task_id);
	if(!task_cfg) {
		return;
	}

	if(!task_cfg->npc_op) {
		return;
	}

	if(task_cfg->npc_op == 1 && task_cfg->npc_scene_id && task_cfg->npc_id
			&& task_state(task_info.__task_id) == TaskInfo::TASK_STATUS_SUBMIT) {
		MSG_81110017 msg;
		msg.type = 1;
		msg.scene_id = task_cfg->npc_scene_id;
		msg.npc_id =  task_cfg->npc_id;
		THIS_SEND_TO_CLIENT(msg);

		Svr_Scene_Npc svr_scene_npc;
		svr_scene_npc.scene_id = task_cfg->npc_scene_id;
		svr_scene_npc.npc_id = task_cfg->npc_id;
		tasker_detail_->svr_scene_npc_set.insert(svr_scene_npc);
		this->save_tick();
		return;
	}

	if(task_cfg->npc_op == 2 && task_cfg->npc_scene_id && task_cfg->npc_id
			&& task_state(task_info.__task_id) == TaskInfo::TASK_STATUS_FINISH) {
		MSG_81110017 msg;
		msg.type = 2;
		msg.scene_id = task_cfg->npc_scene_id;
		msg.npc_id =  task_cfg->npc_id;
		THIS_SEND_TO_CLIENT(msg);

		Svr_Scene_Npc svr_scene_npc;
		svr_scene_npc.scene_id = task_cfg->npc_scene_id;
		svr_scene_npc.npc_id = task_cfg->npc_id;
		tasker_detail_->svr_scene_npc_set.erase(svr_scene_npc);
		this->save_tick();
		return;
	}
}

bool Tasker::is_finish_last_talent_task(uint32_t talent_id) {
	if(!talent_id) {
		return 0;
	}

	const Task_Config &task_all_cfg = CONFIG_CACHE_TASK->get_task_config();
	for(Task_Mission_Config::const_iterator it = task_all_cfg.missions.begin(); it != task_all_cfg.missions.end(); ++it) {
		if(it->second.task_type != TaskInfo::TASK_GTYPE_TALENT) {
			continue;
		}

		if(it->second.job && it->second.job != player_self()->career()) {
			continue;
		}

		if(it->second.talent_id != talent_id) {
			continue;
		}

		if(it->second.postcondition != 0 || it->second.postcondition2 != 0 || it->second.postcondition3 != 0) {
			continue;
		}

		int status = task_state(it->second.task_id);
		if(status == TaskInfo::TASK_STATUS_SUBMIT) {
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

uint8_t Tasker::get_ring_task_type_by_ring_num(const uint32_t ring_num) {
	const Ring_Task_All_Cfg &ring_task_all_cfg = CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	Ring_Num_Type_Cfg_Map::const_iterator find_it = ring_task_all_cfg.ring_num_type_cfg_map.find(ring_num);
	if(find_it == ring_task_all_cfg.ring_num_type_cfg_map.end()) {
		return 0;
	}

	if(find_it->second.op != 1) {
		return 0;
	}

	return find_it->second.ring_type;
}

int Tasker::task_listen_suit(int id, int curr_count) {
	TaskSet temp;
	TaskInfo *task_info = 0;
	const Suit_ID_Cfg *suit_cfg = CONFIG_CACHE_EQUIP->find_suit(id);
	if (!suit_cfg)
		return 0;
	for (TaskSet::iterator iter = this->task_accepted_suit_.begin();
			iter != this->task_accepted_suit_.end(); ++iter)
	{
		if (0 == (task_info = find_task(*iter))) {
			continue;
		}

		if (task_info->is_accepted() == false)
			continue;
		if (task_info->is_logic_suit() == false)
			continue;

		temp.insert(*iter);
	}

	for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_suit(task_info, id, curr_count);
	}

	temp.clear();
	return 0;
}

int Tasker::task_listen_role_hero_suit(int item_id, int hero_index) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_id);
	if (!item_config || !item_config->equip_cfg.suit_id) //是套装
		return -1;
	if (!hero_index) { //玩家
		std::map<int, int>::const_iterator suit_iter = player_self()->equiper_detail().suit_cnt_map.find(item_config->equip_cfg.suit_id);
		if (suit_iter != player_self()->equiper_detail().suit_cnt_map.end())
			return task_listen_suit(item_config->equip_cfg.suit_id, suit_iter->second);
		else
			return task_listen_suit(item_config->equip_cfg.suit_id, 0);
	} else { //英雄
		int most_put_on = 0;
		for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
				it != player_self()->heroer_detail().hero_map.end();
				++it) {
			std::map<int, int>::const_iterator suit_iter = it->second.suit_cnt_map.find(item_config->equip_cfg.suit_id);
			if (suit_iter != it->second.suit_cnt_map.end() && suit_iter->second > most_put_on)
				most_put_on = suit_iter->second;
		}
		return task_listen_suit(item_config->equip_cfg.suit_id, most_put_on);
	}
	return 0;
}

uint8_t Tasker::get_must_not_ring_task_type_by_ring_num(const uint32_t ring_num) {
	const Ring_Task_All_Cfg &ring_task_all_cfg = CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	Ring_Num_Type_Cfg_Map::const_iterator find_it = ring_task_all_cfg.ring_num_type_cfg_map.find(ring_num);
	if(find_it == ring_task_all_cfg.ring_num_type_cfg_map.end()) {
		return 0;
	}

	if(find_it->second.op != 2) {
		return 0;
	}

	return find_it->second.ring_type;
}

int Tasker::req_show_day_task_list() {
	MSG_50110018 msg;
	Task_Info msg_task_info;

    //未领取奖励的任务集
    TaskIdSet award_set;

    for (TaskMap::iterator it = this->tasker_detail_->task_map_.begin();
            it != this->tasker_detail_->task_map_.end(); ++it)
    {
    	msg_task_info.reset();

    	const TaskInfo &svr_task_info = it->second;

    	if(svr_task_info.__game_type != TaskInfo::TASK_GTYPE_DAY) {
    		continue;
    	}

    	const Task_Mission_Detail_Config *task_cfg =  CONFIG_CACHE_TASK->find_task(svr_task_info.__task_id);
    	if(!task_cfg) {
    		MSG_USER("task cfg not exist, role_name=%s, task_id=%d", player_self()->role_name().c_str(), svr_task_info.__task_id);
    		continue;
    	}

    	if(svr_task_info.__game_type == TaskInfo::TASK_GTYPE_DAY && task_cfg->is_show_in_task_list) {
    		continue;
    	}

        if (svr_task_info.is_finish() == false &&
        		svr_task_info.is_accepted() == false &&
        		svr_task_info.is_acceptable() == false &&
        		svr_task_info.is_visible() == false &&
        		svr_task_info.is_fail() == false)
            continue;

        process_get_day_task_list(msg, msg_task_info, svr_task_info, award_set);
    }

    for (TaskIdSet::iterator it = this->tasker_detail_->task_submited_once_.begin();
    		it != this->tasker_detail_->task_submited_once_.end(); ++it) {
    	const Task_Mission_Detail_Config *task_config = CONFIG_CACHE_TASK->find_task(*it);
    	if (task_config && task_config->task_type == TaskInfo::TASK_GTYPE_DAY && !task_config->is_show_in_task_list) {
    		msg_task_info.reset();
            msg_task_info.type = task_config->task_type;
            msg_task_info.id = *it;
            msg_task_info.status = TaskInfo::TASK_STATUS_SUBMIT;
            msg.day_task_list.push_back(msg_task_info);
    	}
    }

    THIS_SEND_TO_CLIENT(msg);

	award_set.clear();
	return 0;
}

bool Tasker::is_magic_wing_try_task_submit() {
	const Task_Config &task_config = CONFIG_CACHE_TASK->get_task_config();
	if(tasker_detail_->task_submited_once_.find((int)task_config.magic_wing_try_id) != tasker_detail_->task_submited_once_.end()) {
		return true;
	}
	return false;
}

int Tasker::task_listen_alchemy_num() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_alchemy_num_.begin();
            iter != this->task_accepted_alchemy_num_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_alchemy_num() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_alchemy_num(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_did_reward_task_num_today(const uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_did_reward_task_num_today_.begin();
            iter != this->task_accepted_did_reward_task_num_today_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_did_reward_task_num_today() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_did_reward_task_num_today(task_info, num);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_remote_scene_any_pass() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_remote_scene_any_pass_.begin();
            iter != this->task_accepted_remote_scene_any_pass_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_remote_scene_any_pass() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_remote_scene_any_pass(task_info);
    }

    temp.clear();
	return 0;
}

int Tasker::task_listen_knight_trial_any_pass() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_knight_trial_any_pass_.begin();
            iter != this->task_accepted_knight_trial_any_pass_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_knight_trial_any_pass() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_knight_trial_any_pass(task_info);
    }

    temp.clear();
	return 0;
}

void Tasker::task_listen_compose_gemstone_level(uint32_t stone_level, uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_compose_gemstone_level_.begin();
            iter != this->task_accepted_compose_gemstone_level_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_compose_gemstone_level() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_compose_gemstone_level(task_info, stone_level, num);
    }

    temp.clear();
}

void Tasker::task_listen_inlay_gemstone() {
	if(task_acceped_inlay_position_gemstone_.empty()
			&& task_accepted_inlay_any_gemstone_.empty()
			&& task_accepted_inlay_id_gemstone_.empty()
			&& task_accepted_inlay_level_gemstone_.empty()) {
		return;
	}

	int num = 0;
	std::map<int, int> equip_gemstone_position_num_map;
	equip_gemstone_position_num_map.clear();

	std::map<int, int> gemstone_id_num_map;
	gemstone_id_num_map.clear();

	std::map<int, int> gemstone_level_num_map;
	gemstone_level_num_map.clear();

   	const Item_Detail *equip = NULL;
	uint32_t equip_id = 0;
	int position = 0;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		equip = player_self()->pack_get_const_item(index);
		if (! equip) {
			continue;
		}

		equip_id = equip->id_;
		position = player_self()->get_part_by_id((int)equip->id_);

		for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
			GemStone gemstone = equip->addon.equip.gemstones[i];
			if (!gemstone.stone_id) {
				continue;
			}

			std::map<int, int>::iterator find_it = equip_gemstone_position_num_map.find(position);
			if(find_it == equip_gemstone_position_num_map.end()) {
				equip_gemstone_position_num_map.insert(std::make_pair(position, 1));
			}
			else {
				find_it->second += 1;
			}

			++num;

			std::map<int, int>::iterator gemstone_id_it = gemstone_id_num_map.find(gemstone.stone_id);
			if(gemstone_id_it == gemstone_id_num_map.end()) {
				gemstone_id_num_map.insert(std::make_pair(gemstone.stone_id, 1));
			}
			else {
				gemstone_id_it->second += 1;
			}

			const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(gemstone.stone_id);
			if(!item_cfg) {
				continue;
			}

			std::map<int, int>::iterator gemsteone_level_it = gemstone_level_num_map.find(item_cfg->gemstone_cfg.lv);
			if(gemsteone_level_it == gemstone_level_num_map.end()) {
				gemstone_level_num_map.insert(std::make_pair(item_cfg->gemstone_cfg.lv, 1));
			}
			else {
				gemsteone_level_it->second += 1;
			}
		}
	}//end for

	for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end();
			++it) {
		int pack_type = PACK_T_HERO_EQUIP_INDEX + it->second.hero_index * ONE_HERO_EQUIP_GAP;
		uint cap = player_self()->pack_is_hero_type(pack_type) ?  player_self()->pack_get_hero_capacity_public(pack_type) :  player_self()->pack_get_capacity_public((Pack_Type)pack_type);
		int idx_start = pack_type + RS_INNER_IDX_BEGIN;
		int idx_end = pack_type + cap + 1;
		for (int idx = idx_start; idx < idx_end; ++idx) {
			equip = player_self()->pack_get_const_item(idx);
			if (!equip) {
				continue;
			}

			equip_id = equip->id_;
			position = player_self()->get_part_by_id((int)equip->id_);

			for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
				GemStone gemstone = equip->addon.equip.gemstones[i];
				if (!gemstone.stone_id) {
					continue;
				}

				std::map<int, int>::iterator find_it = equip_gemstone_position_num_map.find(position);
				if(find_it == equip_gemstone_position_num_map.end()) {
					equip_gemstone_position_num_map.insert(std::make_pair(position, 1));
				}
				else {
					find_it->second += 1;
				}

				++num;

				std::map<int, int>::iterator gemstone_id_it = gemstone_id_num_map.find(gemstone.stone_id);
				if(gemstone_id_it == gemstone_id_num_map.end()) {
					gemstone_id_num_map.insert(std::make_pair(gemstone.stone_id, 1));
				}
				else {
					gemstone_id_it->second += 1;
				}

				const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(gemstone.stone_id);
				if(!item_cfg) {
					continue;
				}

				std::map<int, int>::iterator gemsteone_level_it = gemstone_level_num_map.find(item_cfg->gemstone_cfg.lv);
				if(gemsteone_level_it == gemstone_level_num_map.end()) {
					gemstone_level_num_map.insert(std::make_pair(item_cfg->gemstone_cfg.lv, 1));
				}
				else {
					gemsteone_level_it->second += 1;
				}
			}
		}
	}//end for

	if(!task_accepted_inlay_any_gemstone_.empty()) {
		task_listen_inlay_gemstone_any(num);
	}

	if(!task_acceped_inlay_position_gemstone_.empty()) {
		for(std::map<int, int>::const_iterator it = equip_gemstone_position_num_map.begin();
				it != equip_gemstone_position_num_map.end();
				++it) {
			task_listen_inlay_gemstone_position(it->first, it->second);
		}
	}

	if(!task_accepted_inlay_id_gemstone_.empty()) {
		for(std::map<int, int>::const_iterator it = gemstone_id_num_map.begin();
				it != gemstone_id_num_map.end();
				++it) {
			task_listen_inlay_gemstone_id(it->first, it->second);
		}
	}

	if(!task_accepted_inlay_level_gemstone_.empty()) {
		for(std::map<int, int>::const_iterator it = gemstone_level_num_map.begin();
				it != gemstone_level_num_map.end();
				++it) {
			task_listen_inlay_level_gemstone(it->first, it->second);
		}
	}
}

void Tasker::task_listen_inlay_gemstone_position(uint32_t position, uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_acceped_inlay_position_gemstone_.begin();
            iter != this->task_acceped_inlay_position_gemstone_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_inlay_position_gemstone() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_inlay_position_gemstone(task_info, position, num);
    }

    temp.clear();
}

void Tasker::task_listen_inlay_gemstone_any(uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_inlay_any_gemstone_.begin();
            iter != this->task_accepted_inlay_any_gemstone_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_inlay_any_gemstone() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_inlay_any_gemstone(task_info, (int)num);
    }

    temp.clear();
}

void Tasker::task_listen_world_boss() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_world_boss_.begin();
            iter != this->task_accepted_world_boss_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_world_boss() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_world_boss(task_info);
    }

    temp.clear();
}

void Tasker::task_listen_world_boss_immediate_revive() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_world_boss_immediate_revive_.begin();
            iter != this->task_accepted_world_boss_immediate_revive_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_world_boss_immediate_revive() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_world_boss_immediate_revive(task_info);
    }

    temp.clear();
}

void Tasker::task_listen_enchant_any_equip() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_enchant_any_equip_.begin();
            iter != this->task_accepted_enchant_any_equip_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_enchant_any_equip() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_enchant_any_equip(task_info);
    }

    temp.clear();
}

void Tasker::task_listen_inlay_gemstone_id(uint32_t stone_id, uint32_t num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_inlay_id_gemstone_.begin();
            iter != this->task_accepted_inlay_id_gemstone_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_inlay_id_gemstone() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_inlay_gemsteone_id(task_info, stone_id, num);
    }

    temp.clear();
}

void Tasker::task_listen_gang_well_recover_spirit() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_gang_well_recover_spirit_.begin();
            iter != this->task_accepted_gang_well_recover_spirit_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_gang_well_recover_spirit() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_gang_well_recover_spirit(task_info);
    }

    temp.clear();
}

void Tasker::task_listen_equip_purple_ring() {
	if(task_accepted_equip_purple_ring_.empty()) {
		return;
	}

	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_equip_purple_ring_.begin();
            iter != this->task_accepted_equip_purple_ring_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_equip_purple_ring() == false)
            continue;

        temp.insert(*iter);
    }

    std::map<int32_t, int32_t> ring_map;
    ring_map.clear();

	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		const Item_Detail *equip = player_self()->pack_get_const_item(index);
		if (! equip) {
			continue;
		}

		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (!item_config) {
			continue;
		}

		int position = player_self()->get_part_by_id((int)equip->id_);
		if(position != EP_RING) {
			continue;
		}

		std::map<int32_t, int32_t>::iterator find_it = ring_map.find((int32_t)item_config->color);
		if(find_it == ring_map.end()) {
			ring_map.insert(std::make_pair((int32_t)item_config->color, equip->amount_));
		}
		else {
			find_it->second += equip->amount_;
		}
	}

	for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end();
			++it) {
		int pack_type = PACK_T_HERO_EQUIP_INDEX + it->second.hero_index * ONE_HERO_EQUIP_GAP;
		uint cap = player_self()->pack_is_hero_type(pack_type) ?  player_self()->pack_get_hero_capacity_public(pack_type) :  player_self()->pack_get_capacity_public((Pack_Type)pack_type);
		int idx_start = pack_type + RS_INNER_IDX_BEGIN;
		int idx_end = pack_type + cap + 1;
		for (int idx = idx_start; idx < idx_end; ++idx) {
			const Item_Detail *equip = player_self()->pack_get_const_item(idx);
			if (!equip) {
				continue;
			}

			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
			if (!item_config) {
				continue;
			}

			int position = player_self()->get_part_by_id((int)equip->id_);
			if(position != EP_RING) {
				continue;
			}

			std::map<int32_t, int32_t>::iterator find_it = ring_map.find((int32_t)item_config->color);
			if(find_it == ring_map.end()) {
				ring_map.insert(std::make_pair((int32_t)item_config->color, equip->amount_));
			}
			else {
				find_it->second += equip->amount_;
			}
		}
	}//end for

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}

	    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_info->__task_id);
	    if (NULL == task_json) {
	    	continue;
	    }

	    int32_t quality = 0;
	    int32_t num = 0;

		for (size_t i = 0; i < task_json->execute.size(); ++i) {
	        TaskCondition *task_cond = task_info->task_condition(i);
	        if (task_cond == 0)
	        {
	            continue;
	        }

	        if(task_json->execute[i].type == TaskExecType::TASK_EXEC_EQUIP_PURPLE_RING) {
	        	quality = task_json->execute[i].id;
	        }
	        break;
		}

		for(std::map<int32_t, int32_t>::const_iterator it = ring_map.begin(); it != ring_map.end(); ++it) {
			if(it->first >= quality) {
				num += it->second;
			}
		}

		process_listen_equip_purple_ring(task_info, quality, num);
    }

    temp.clear();
}

void Tasker::task_listen_inlay_level_gemstone(uint32_t gemstone_level, uint32_t gemstone_num) {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_inlay_level_gemstone_.begin();
            iter != this->task_accepted_inlay_level_gemstone_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_inlay_level_gemstone() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_inlay_level_gemsteone(task_info, gemstone_level, gemstone_num);
    }

    temp.clear();
}

void Tasker::task_listen_upgrade_card() {
	TaskSet temp;
    TaskInfo *task_info = 0;
    for (TaskSet::iterator iter = this->task_accepted_upgrade_card_.begin();
            iter != this->task_accepted_upgrade_card_.end(); ++iter)
    {
    	if (0 == (task_info = find_task(*iter))) {
    		continue;
    	}

        if (task_info->is_accepted() == false)
            continue;
        if (task_info->is_logic_upgrade_card() == false)
            continue;

        temp.insert(*iter);
    }

    for(TaskSet::iterator it = temp.begin(); it != temp.end(); ++it) {
		task_info = find_task(*it);
		if(!task_info) {
			continue;
		}
		process_listen_upgrade_card(task_info);
    }

    temp.clear();
}

void Tasker::task_listen_magic_wing_active_to_clear_task() {
	bool flag = false;
	const Task_Config &task_all_cfg = CONFIG_CACHE_TASK->get_task_config();
	for(Task_Mission_Config::const_iterator it = task_all_cfg.missions.begin(); it != task_all_cfg.missions.end(); ++it) {
		if(it->second.is_magic_wing && tasker_detail_->task_submited_once_.find(it->first) == tasker_detail_->task_submited_once_.end()) {
			flag = true;

			TaskInfo *task_info = find_task(it->first);
			if(!task_info) {
				tasker_detail_->task_submited_once_.insert(it->first);
			}
			else {
				notify_task_del(*task_info);
				tasker_detail_->task_map_.erase(it->first);
				tasker_detail_->task_submited_once_.insert(it->first);
			}
		}
	}
	this->save_tick();
}

void Tasker::test_refresh_ring_task__ring_num(uint32_t ring_num) {
	const Ring_Task_All_Cfg &ring_task_all_cfg =  CONFIG_CACHE_TASK->get_ring_task_all_cfg();
	if(ring_num < 1 || tasker_detail_->ring_num > ring_task_all_cfg.ring_task_base_cfg.max_ring_num) {
		return;
	}

	if(player_self()->level() < (int)ring_task_all_cfg.ring_task_base_cfg.open_level) {
		return;
	}

	tasker_detail_->did_ring_num = ring_num - 1;
	tasker_detail_->ring_num = ring_num - 1;
	refresh_ring_task();
}
