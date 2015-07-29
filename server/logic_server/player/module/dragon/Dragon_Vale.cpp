/*
 * Dragon_Vale.cpp
 *
 *  Created on: 2014年7月11日
 *      Author: gaojianshan
 */

#include <stdio.h>
#include <string.h>
#include "Dragon_Vale_Struct.h"
#include "Dragon_Vale_Building_Config.h"
#include "Dragon_Vale_Dragon_Config.h"
#include "Dragon_Vale.h"
#include "Config_Cache_Scene.h"
#include "Logic_Player.h"
#include "Config_Cache_Dragon_Vale.h"
#include "Logic_Monitor.h"
#include "Logic_Player_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Dragon_Vale_Struct.h"
#include "pack/Packer.h"
#include "Err_Code.h"
#include "gang/Gang_Manager.h"
#include "gang/Gang_Struct.h"
#include "fighter/Fighter_Def.h"
#include "Config_Cache_Scene.h"
#include "Fighter_Detail.h"
#include "Msg_Inner_Struct.h"
#include "Config_Cache_Role.h"
#include "Config_Cache.h"
#include "Dragon_Vale_Manager.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Item.h"
#include "Logic_Public.h"
#include "Config_Cache_Mail.h"
#include "DB_Def.h"
#include "Config_Cache_NPC.h"
#include "announce/Logic_Announce.h"
#include "public/Public_Config.h"
#include "helper/Helper_Def.h"
#include "vip/Vip_Def.h"
#include "icon/Icon_Def.h"

Dragon_Vale::Dragon_Vale(void) {
	reset();
}

Dragon_Vale::~Dragon_Vale() {
}

void Dragon_Vale::reset(void) {
	dragon_vale_detail_ = 0;
	building_notice_map.clear();
}

int Dragon_Vale::create_init(Dragon_Vale_Detail &dragonValeDetail, const Base_Detail &baseDetail) {
	//普通玩家的最大建筑队列数目，取vip等级为0的配置
	const Vip_Level_Cfg *vip_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Vip_Level_Cfg(0);
	if(vip_level_cfg) {
		dragonValeDetail.build_queue_num = vip_level_cfg->max_queue_num;
		dragonValeDetail.all_open_build_queue_num = vip_level_cfg->max_queue_num;
	}

	return 0;
}

void Dragon_Vale::load_detail(Dragon_Vale_Detail &detail) {
	dragon_vale_detail_ = &detail;
}

void Dragon_Vale::save_detail(Dragon_Vale_Detail &detail) {
	if (dragon_vale_detail_->is_change) {
		detail = dragon_vale_detail();
		dragon_vale_detail_->is_change = false;
	}
}

void Dragon_Vale::module_init(void) {
	correct_building_queue();

	// 防止场景配制删除
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
	if (scene_config) {
		//登录后不在龙谷内，重置target_player_id，按不在龙谷内处理
		if(Dragon_Vale_Type != scene_config->type) {
			dragon_vale_detail_->target_player_id = 0;
			dragon_vale_detail_->is_rob_enter_dragon_vale = false;
			dragon_vale_detail_->uuid = 0;
		} else {
			player_self()->set_create_scene_args(0, Time_Value::zero, dragon_vale_detail_->target_player_id,
							dragon_vale_detail_->is_rob_enter_dragon_vale ? 1 : 0, dragon_vale_detail_->uuid,
							0, true);
		}
	} else {
		MSG_USER("can not find scene:%d ", player_self()->location().scene_id);

		//找不到场景配置，按不在龙谷内处理
		dragon_vale_detail_->target_player_id = 0;
		dragon_vale_detail_->is_rob_enter_dragon_vale = false;
		dragon_vale_detail_->uuid = 0;
	}

	process_login_building_finish();
	process_login_building_output_production_by_one_minute();
	process_login_dragon_born_finish();
	process_login_play_dragon_interval_time();
	process_login_dragon_feeling_down_time();
	process_login_dragon_rob_protect_time();
	process_login_refresh_dragon_vale_monster();

	if(dragon_vale_detail_->is_dragon_born && dragon_vale_detail_->dragon_info.progress == 1) {
		const Dragon_Evolution_Cfg* dragon_evolution_cfg =
					CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(dragon_vale_detail_->dragon_info.progress);
		if(dragon_evolution_cfg) {
			double temp_property = 0.0;
			const Dragon_Keep_Cfg* dragon_keep_cfg = 0;
			//初始化主角斗力属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_POWER,
											dragon_vale_detail_->dragon_info.power_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.power == 0) {
				temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.power = (uint32_t)temp_property;
			}

			//初始化主角护甲属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_ARMOR,
											dragon_vale_detail_->dragon_info.armor_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.armor == 0) {
				temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.armor = (uint32_t)temp_property;
			}

			//初始化主角耐力属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_STAMINA,
											dragon_vale_detail_->dragon_info.stamina_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.stamina == 0) {
				temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
					dragon_vale_detail_->dragon_info.stamina = (uint32_t)temp_property;
			}

			//初始化英雄斗力属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_POWER,
											dragon_vale_detail_->dragon_info.hero_power_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.hero_power == 0) {
				temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_power = (uint32_t)temp_property;
			}

			//初始化英雄护甲属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_ARMOR,
											dragon_vale_detail_->dragon_info.hero_armor_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.hero_armor == 0) {
				temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_armor = (uint32_t)temp_property;
			}

			//初始化英雄耐力属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_STAMINA,
											dragon_vale_detail_->dragon_info.hero_stamina_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.hero_stamina == 0) {
				temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
					dragon_vale_detail_->dragon_info.hero_stamina = (uint32_t)temp_property;
			}

			//初始化主角暴击属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_CRIT,
											dragon_vale_detail_->dragon_info.crit_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.crit == 0) {
				temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
					dragon_vale_detail_->dragon_info.crit = (uint32_t)temp_property;
			}

			//初始化主角抗暴属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_PREVENT_CRIT,
											dragon_vale_detail_->dragon_info.prevent_crit_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.prevent_crit == 0) {
				temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
					dragon_vale_detail_->dragon_info.prevent_crit = (uint32_t)temp_property;
			}

			//初始化英雄暴击属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_CRIT,
											dragon_vale_detail_->dragon_info.hero_crit_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.hero_crit == 0) {
				temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
					dragon_vale_detail_->dragon_info.hero_crit = (uint32_t)temp_property;
			}

			//初始化英雄抗暴属性
			dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_PREVENT_CRIT,
											dragon_vale_detail_->dragon_info.hero_prevent_crit_level);
			if(dragon_keep_cfg && dragon_vale_detail_->dragon_info.hero_prevent_crit == 0) {
				temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
					dragon_vale_detail_->dragon_info.hero_prevent_crit = (uint32_t)temp_property;
			}
		}
	}//end if

	Prop_Setter_Vec prop_vec;
	Prop_Setter setter;
	if(dragon_vale_detail_->dragon_info.power > 0) {
		setter.reset();
		setter.prop_type = PT_POWER;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.power);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.armor > 0) {
		setter.reset();
		setter.prop_type = PT_ARMOR;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.armor);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.stamina > 0) {
		setter.reset();
		setter.prop_type = PT_STAMINA;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.stamina);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.crit > 0) {
		setter.reset();
		setter.prop_type = PT_CRIT;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.crit);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.prevent_crit > 0) {
		setter.reset();
		setter.prop_type = PT_CRIT_DEF;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.prevent_crit);
		prop_vec.push_back(setter);
	}

	if (prop_vec.size() > 0) player_self()->fight_prop_modify(prop_vec, 210);

	player_self()->heroer_prop_refresh();

	Time_Value now = Time_Value::gettimeofday();
	int num = get_time_subdays(player_self()->base_detail().last_sign_in_time, now);
	if(num == 0) {
		return;
	}
	if(num > 1) {
		dragon_vale_detail_->is_yesterday_login = false;
	}
	if(num == 1) {
		dragon_vale_detail_->is_yesterday_login = true;
	}

	((Dragon_Vale*)this)->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	// inter
	listen_lvl_when_module_init();
}

void Dragon_Vale::sign_in(void) {

}

int Dragon_Vale::sign_in_repeat(void) {
	return 0;
}

int Dragon_Vale::client_ready_now(void) {
	if(!building_notice_map.empty()) {
		Building_Notice_Map::iterator it = building_notice_map.find(BUILDING_NOTICE_OUTPUT_PRODUCTION_MAX);
		if(it != building_notice_map.end()) {
			MSG_82300008 msg;
			msg.is_gold_mine = it->second.gold_mine;
			msg.is_moon_weel = it->second.moon_well;
			msg.is_totem = it->second.totem;
			THIS_SEND_TO_CLIENT(msg);
		}

		it = building_notice_map.find(BUILDING_NOTICE_UPGRADE_FINISH);
		if(it != building_notice_map.end()) {
			MSG_82300009 msg;
			msg.gold_mine_flag = it->second.gold_mine;
			msg.moon_well_flag = it->second.moon_well;
			msg.totem_flag = it->second.totem;
			THIS_SEND_TO_CLIENT(msg);
		}
		building_notice_map.clear();
	}

	if(!player_self()->operation_detail_no_const().building_notice_map.empty()) {
		Building_Notice_Map::iterator it =
				player_self()->operation_detail_no_const().building_notice_map.find(BUILDING_NOTICE_OUTPUT_PRODUCTION_MAX);
		if(it != building_notice_map.end()) {
			MSG_82300008 msg;
			msg.is_gold_mine = it->second.gold_mine;
			msg.is_moon_weel = it->second.moon_well;
			msg.is_totem = it->second.totem;
			THIS_SEND_TO_CLIENT(msg);
		}

		it = player_self()->operation_detail_no_const().building_notice_map.find(BUILDING_NOTICE_UPGRADE_FINISH);
		if(it != building_notice_map.end()) {
			MSG_82300009 msg;
			msg.gold_mine_flag = it->second.gold_mine;
			msg.moon_well_flag = it->second.moon_well;
			msg.totem_flag = it->second.totem;
			THIS_SEND_TO_CLIENT(msg);
		}
		player_self()->operation_detail_no_const().building_notice_map.clear();
	}

	if(!player_self()->operation_detail_no_const().rob_fight_result_map.empty()) {
		Rob_Fight_Result_Map::iterator it =
				player_self()->operation_detail_no_const().rob_fight_result_map.find(DRAGON_VALE_ROB_NOTICE);
		if(it != player_self()->operation_detail_no_const().rob_fight_result_map.end()) {
			MSG_82300023 msg;
			msg.server_name = it->second.server_name;
			msg.player_name = it->second.player_name;
			msg.result = it->second.result;
			THIS_SEND_TO_CLIENT(msg);
		}

		//note:策划需求改动： 由发改为不发， 暂时不删除，留着以后再改回来时，不需重写(注：不同策划需求不同，其他策划可能要求改回来)
//		it = player_self()->operation_detail_no_const().rob_fight_result_map.find(DRAGON_VALE_TARGET_NOTICE);
//		if(it != player_self()->operation_detail_no_const().rob_fight_result_map.end()) {
//			MSG_82300024 msg;
//			msg.server_name = it->second.server_name;
//			msg.player_name = it->second.player_name;
//			msg.result = it->second.result;
//			THIS_SEND_TO_CLIENT(msg);
//		}
		player_self()->operation_detail_no_const().rob_fight_result_map.clear();
	}

	if(!player_self()->operation_detail_no_const().gain_rob_production_set.empty()) {
		for(Gain_Rob_Prodcutin_Set::iterator it = player_self()->operation_detail_no_const().gain_rob_production_set.begin();
				it != player_self()->operation_detail_no_const().gain_rob_production_set.end();
				++it) {
			MSG_82300025 msg;
			msg.server_name = it->server_name;
			msg.player_name = it->player_name;
			msg.total_gold_mine = it->gold_mine;
			msg.total_moon_weel = it->moon_well;
			msg.total_totem = it->totem;
			THIS_SEND_TO_CLIENT(msg);
		}
		player_self()->operation_detail_no_const().gain_rob_production_set.clear();
	}

	if(player_self()->operation_detail_no_const().is_dragon_vale_special) {
		MSG_82300028 msg;
		THIS_SEND_TO_CLIENT(msg);
		player_self()->operation_detail_no_const().is_dragon_vale_special = false;
	}

	notify_has_can_create_dragon_building_num();

	if(dragon_vale_detail_->is_special && player_self()->gang_id() > 0) {
		player_self()->create_ann_dragon_vale_specail_online();
	}

	Time_Value now = Time_Value::gettimeofday();
	if(dragon_vale_detail_->begin_no_enter_time.sec() + dragon_vale_detail_->left_time > now.sec()) {
		MSG_82300026 msg_82300026;
		msg_82300026.left_time = dragon_vale_detail_->begin_no_enter_time.sec() + dragon_vale_detail_->left_time - now.sec();
		THIS_SEND_TO_CLIENT(msg_82300026);
	}
	else {
		if(DRAGON_VALE_MANAGER->get_battle_role_id_set().find(player_self()->role_id())
				!= DRAGON_VALE_MANAGER->get_battle_role_id_set().end()) {
			const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
			MSG_82300026 msg_82300026;
			msg_82300026.left_time = main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;
			THIS_SEND_TO_CLIENT(msg_82300026);
		}
	}

	return 0;
}

void Dragon_Vale::sign_out(void) {

}

int Dragon_Vale::trigger_daily_six(const Time_Value &now) {

	return 0;
}

int Dragon_Vale::move_scene_completed(void) {
	const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
	if(cfg && cfg->type != Dragon_Vale_Type) {
		//主动通知龙谷信息(龙谷以外的场景接收)
		{
			MSG_82300033 msg;
			msg.build_queue_num = dragon_vale_detail_->build_queue_num;
			msg.all_open_build_queue_num = dragon_vale_detail_->all_open_build_queue_num;
			msg.progress = dragon_vale_detail_->dragon_info.progress;
			const Role_Level_Cfg *role_level_cfg = 0;
			for(DragonBuildingInfoMap::const_iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
					it != dragon_vale_detail_->dragon_building_info_map.end(); ++it) {
				Building_Info building_info;
				building_info.id = it->second.id;
				building_info.type = it->second.type;
				building_info.level = it->second.level;
				building_info.status = it->second.status;
				building_info.production = it->second.production;

				if(it->second.status == BUILDING_UPGRADE_DOING || !it->second.is_gain) {
					building_info.is_gain = 0;
				}
				else {
					building_info.is_gain = 1;
				}

				building_info.order = it->second.order;
				building_info.consume_type = it->second.consume_type;

				role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), it->second.type);
				if(!role_level_cfg) {
					continue;
				}

				if(it->second.level >= role_level_cfg->max_building_level) {
					msg.building_info_list.push_back(building_info);
					continue;
				}

				uint16_t new_level = it->second.level + 1;
				const Building_Level_Cfg* building_level_cfg =
						CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, new_level);
				if(!building_level_cfg) {
					continue;
				}

				Time_Value now = Time_Value::gettimeofday();
				long total_time = 0;

				if(0 == it->second.level) {
					const Create_Building_Cfg *create_building_cfg =
									CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
					if(!create_building_cfg) {
						continue;
					}
					total_time = it->second.begin_upgrade_time.sec()
							+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
							+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
							+ (long)(create_building_cfg->create_need_second);
				}
				else {
					total_time = it->second.begin_upgrade_time.sec()
							+ building_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
							+ building_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
							+ building_level_cfg->upgrade_need_second;
				}

				building_info.left_time = (total_time > now.sec()) ? total_time - now.sec() : 0;
				msg.building_info_list.push_back(building_info);
			}//end for
			THIS_SEND_TO_CLIENT(msg);
		}

		if(cfg && cfg->type == Main_Scene_Type) {
			if(!dragon_vale_detail_->is_dragon_born) {
				return 0;
			}

			if(dragon_vale_detail_->dragon_info.progress > 0) {
				return 0;
			}

			MSG_82300032 msg;
			Time_Value now = Time_Value::gettimeofday();
			const Dragon_Base_Cfg &dragon_base_cfg =  CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
			long total_time = dragon_vale_detail_->begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;
			if(total_time <= now.sec()) {
				return 0;
			}

			msg.left_time = total_time - now.sec();
			THIS_SEND_TO_CLIENT(msg);
		}//end if

		return 0;
	}//end if

	if(!dragon_vale_detail_->target_player_id) {
		return 0;
	}

	if (!cfg || Scene_Dragon_Vale != cfg->manager) {
		return 0;
	}

	//进自己龙谷
	if(player_self()->role_id() == dragon_vale_detail_->target_player_id) {
		Time_Value now = Time_Value::gettimeofday();
		if(dragon_vale_detail_->begin_no_enter_time.sec() + dragon_vale_detail_->left_time > now.sec()) {
			MSG_82300026 msg_82300026;
			msg_82300026.left_time = dragon_vale_detail_->begin_no_enter_time.sec() + dragon_vale_detail_->left_time - now.sec();
			THIS_SEND_TO_CLIENT(msg_82300026);
			return 0;
		}
		else {
			if(DRAGON_VALE_MANAGER->get_battle_role_id_set().find(player_self()->role_id())
					!= DRAGON_VALE_MANAGER->get_battle_role_id_set().end()) {
				const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
				MSG_82300026 msg_82300026;
				msg_82300026.left_time = main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;
				THIS_SEND_TO_CLIENT(msg_82300026);
				return 0;
			}
		}

		DRAGON_VALE_MANAGER->process_enter_dragon_vale(player_self()->role_id(), player_self()->role_id());
		enter_self_dragon_vale();
		return 0;
	}

	//进本服别人龙谷
	if(!dragon_vale_detail_->uuid && !dragon_vale_detail_->is_rob_enter_dragon_vale) {
		Time_Value now = Time_Value::gettimeofday();
		if(dragon_vale_detail_->begin_no_enter_time.sec() + dragon_vale_detail_->left_time > now.sec()) {
			MSG_82300026 msg_82300026;
			msg_82300026.left_time = dragon_vale_detail_->begin_no_enter_time.sec() + dragon_vale_detail_->left_time - now.sec();
			THIS_SEND_TO_CLIENT(msg_82300026);
			return 0;
		}
		else {
			if(DRAGON_VALE_MANAGER->get_battle_role_id_set().find(player_self()->role_id())
					!= DRAGON_VALE_MANAGER->get_battle_role_id_set().end()) {
				const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
				MSG_82300026 msg_82300026;
				msg_82300026.left_time = main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;
				THIS_SEND_TO_CLIENT(msg_82300026);
				return 0;
			}
		}

		DRAGON_VALE_MANAGER->process_enter_dragon_vale(dragon_vale_detail_->target_player_id, player_self()->role_id());
		enter_other_dragon_vale(dragon_vale_detail_->target_player_id);
		return 0;
	}

	//掠夺者进入跨服龙谷
	if(dragon_vale_detail_->uuid && dragon_vale_detail_->is_rob_enter_dragon_vale) {
		DRAGON_VALE_MANAGER->porocss_rob_enter_dragon_vale(*player_self());
		return 0;
	}

	MSG_USER("enter dragon vale error, target_player_id=%ld, uuid=%ld, is_rob_enter_dragon_vale=%d",
			dragon_vale_detail_->target_player_id, dragon_vale_detail_->uuid,
			dragon_vale_detail_->is_rob_enter_dragon_vale ? 1 : 0);
	return 0;
}

int Dragon_Vale::sync_scene_module_info(Block_Buffer &buf) {
	return 0;
}

void Dragon_Vale::sync_dragon_vale_to_db_view() {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(player_self()->role_id());
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	find_it->second->dragon_vale_view.target_player_id = dragon_vale_detail_->target_player_id;
	find_it->second->dragon_vale_view.is_rob_enter_dragon_vale = dragon_vale_detail_->is_rob_enter_dragon_vale;
	find_it->second->dragon_vale_view.uuid = dragon_vale_detail_->uuid;

	find_it->second->dragon_vale_view.is_gold_mine_max_storage = dragon_vale_detail_->is_gold_mine_max_storage;
	find_it->second->dragon_vale_view.is_moon_well_max_storage = dragon_vale_detail_->is_moon_well_max_storage;
	find_it->second->dragon_vale_view.is_totem_max_storage = dragon_vale_detail_->is_totem_max_storage;

	find_it->second->dragon_vale_view.build_gold_mine_num = dragon_vale_detail_->build_gold_mine_num;
	find_it->second->dragon_vale_view.build_moon_well_num = dragon_vale_detail_->build_moon_well_num;
	find_it->second->dragon_vale_view.build_totem_num = dragon_vale_detail_->build_totem_num;
	find_it->second->dragon_vale_view.build_queue_num = dragon_vale_detail_->build_queue_num;
	find_it->second->dragon_vale_view.all_open_build_queue_num = dragon_vale_detail_->all_open_build_queue_num;

	//building list
	find_it->second->dragon_vale_view.dragon_building_info_map.clear();

	find_it->second->dragon_vale_view.dragon_building_info_map.insert(
			    dragon_vale_detail_->dragon_building_info_map.begin(),
			    dragon_vale_detail_->dragon_building_info_map.end());

	//cancel create building set
	find_it->second->dragon_vale_view.cancel_create_building_set.clear();
	find_it->second->dragon_vale_view.cancel_create_building_set.insert(dragon_vale_detail_->cancel_create_building_set.begin(),
				dragon_vale_detail_->cancel_create_building_set.end());

	find_it->second->dragon_vale_view.is_dragon_born = dragon_vale_detail_->is_dragon_born;
	find_it->second->dragon_vale_view.begin_born_time.set(dragon_vale_detail_->begin_born_time);

	//dragon
	find_it->second->dragon_vale_view.dragon_info.progress = dragon_vale_detail_->dragon_info.progress;
	find_it->second->dragon_vale_view.dragon_info.feeling = dragon_vale_detail_->dragon_info.feeling;
	find_it->second->dragon_vale_view.dragon_info.power = dragon_vale_detail_->dragon_info.power;
	find_it->second->dragon_vale_view.dragon_info.armor = dragon_vale_detail_->dragon_info.armor;
	find_it->second->dragon_vale_view.dragon_info.stamina = dragon_vale_detail_->dragon_info.stamina;
	find_it->second->dragon_vale_view.dragon_info.power_level = dragon_vale_detail_->dragon_info.power_level;
	find_it->second->dragon_vale_view.dragon_info.armor_level = dragon_vale_detail_->dragon_info.armor_level;
	find_it->second->dragon_vale_view.dragon_info.stamina_level = dragon_vale_detail_->dragon_info.stamina_level;
	find_it->second->dragon_vale_view.dragon_info.hero_power = dragon_vale_detail_->dragon_info.hero_power;
	find_it->second->dragon_vale_view.dragon_info.hero_armor = dragon_vale_detail_->dragon_info.hero_armor;
	find_it->second->dragon_vale_view.dragon_info.hero_stamina = dragon_vale_detail_->dragon_info.hero_stamina;
	find_it->second->dragon_vale_view.dragon_info.hero_power_level = dragon_vale_detail_->dragon_info.hero_power_level;
	find_it->second->dragon_vale_view.dragon_info.hero_armor_level = dragon_vale_detail_->dragon_info.hero_armor_level;
	find_it->second->dragon_vale_view.dragon_info.hero_stamina_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
	find_it->second->dragon_vale_view.dragon_info.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon;
	find_it->second->dragon_vale_view.dragon_info.energy = dragon_vale_detail_->dragon_info.energy;
	find_it->second->dragon_vale_view.dragon_info.crit = dragon_vale_detail_->dragon_info.crit;
	find_it->second->dragon_vale_view.dragon_info.prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;
	find_it->second->dragon_vale_view.dragon_info.crit_level = dragon_vale_detail_->dragon_info.crit_level;
	find_it->second->dragon_vale_view.dragon_info.prevent_crit_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
	find_it->second->dragon_vale_view.dragon_info.hero_crit = dragon_vale_detail_->dragon_info.hero_crit;
	find_it->second->dragon_vale_view.dragon_info.hero_prevent_crit = dragon_vale_detail_->dragon_info.hero_prevent_crit;
	find_it->second->dragon_vale_view.dragon_info.hero_crit_level = dragon_vale_detail_->dragon_info.hero_crit_level;
	find_it->second->dragon_vale_view.dragon_info.hero_prevent_crit_level = dragon_vale_detail_->dragon_info.hero_prevent_crit_level;

	find_it->second->dragon_vale_view.begin_play_dragon_interval_time.set(dragon_vale_detail_->begin_play_dragon_interval_time);
	find_it->second->dragon_vale_view.begin_down_feeling_time.set(dragon_vale_detail_->begin_down_feeling_time);

	find_it->second->dragon_vale_view.is_vip_down_to_zero = dragon_vale_detail_->is_vip_down_to_zero;
	find_it->second->dragon_vale_view.matched_num = dragon_vale_detail_->matched_num;

	find_it->second->dragon_vale_view.thief_id_quality_map.clear();
	find_it->second->dragon_vale_view.thief_id_quality_map.insert(dragon_vale_detail_->thief_id_quality_map.begin(),
										dragon_vale_detail_->thief_id_quality_map.end());
	find_it->second->dragon_vale_view.begin_refresh_thief_time.set(dragon_vale_detail_->begin_refresh_thief_time);

	find_it->second->dragon_vale_view.star_num = dragon_vale_detail_->star_num;
	find_it->second->dragon_vale_view.award_foot_num = dragon_vale_detail_->award_foot_num;
	find_it->second->dragon_vale_view.is_special = dragon_vale_detail_->is_special;

	find_it->second->dragon_vale_view.foot_info_map.clear();
	find_it->second->dragon_vale_view.foot_info_map.insert(dragon_vale_detail_->foot_info_map.begin(),
											dragon_vale_detail_->foot_info_map.end());

	find_it->second->dragon_vale_view.foot_id_set.clear();
	find_it->second->dragon_vale_view.foot_id_set.insert(dragon_vale_detail_->foot_id_set.begin(),
											dragon_vale_detail_->foot_id_set.end());

	find_it->second->dragon_vale_view.is_yesterday_login = dragon_vale_detail_->is_yesterday_login;
	find_it->second->dragon_vale_view.dragon_heart_num = dragon_vale_detail_->dragon_heart_num;

	find_it->second->dragon_vale_view.notice_board_list.clear();
	find_it->second->dragon_vale_view.notice_board_list.assign(dragon_vale_detail_->notice_board_list.begin(),
															   dragon_vale_detail_->notice_board_list.end());

	find_it->second->dragon_vale_view.is_rob_protect = dragon_vale_detail_->is_rob_protect;
	find_it->second->dragon_vale_view.begin_rob_protect_time.set(dragon_vale_detail_->begin_rob_protect_time);
	find_it->second->dragon_vale_view.is_first_enter_after_robbed = dragon_vale_detail_->is_first_enter_after_robbed;
	find_it->second->dragon_vale_view.is_in_robbed_dragon_vale = dragon_vale_detail_->is_in_robbed_dragon_vale;
	find_it->second->dragon_vale_view.begin_in_robbed_dragon_vale_time.set(dragon_vale_detail_->begin_in_robbed_dragon_vale_time);
	find_it->second->dragon_vale_view.is_dragon_vale_open = dragon_vale_detail_->is_dragon_vale_open;
	find_it->second->dragon_vale_view.begin_no_enter_time.set(dragon_vale_detail_->begin_no_enter_time);
	find_it->second->dragon_vale_view.left_time = dragon_vale_detail_->left_time;
	find_it->second->dragon_vale_view.play_dragon_num = dragon_vale_detail_->play_dragon_num;
	find_it->second->dragon_vale_view.drive_thief_num = dragon_vale_detail_->drive_thief_num;
}

void Dragon_Vale::sync_dragon_vale_create_args_to_db_view() {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(player_self()->role_id());
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	find_it->second->dragon_vale_view.target_player_id = dragon_vale_detail_->target_player_id;
	find_it->second->dragon_vale_view.uuid = dragon_vale_detail_->uuid;
	find_it->second->dragon_vale_view.is_rob_enter_dragon_vale = dragon_vale_detail_->is_rob_enter_dragon_vale;
}

void Dragon_Vale::save_dragon_vale_tick() {
	dragon_vale_detail_->detail_change();
}

void Dragon_Vale::enter_self_dragon_vale() {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	Time_Value now = Time_Value::gettimeofday();
	MSG_82300000 msg;

	//建筑
	const Role_Level_Cfg *role_level_cfg = 0;
	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_GOLD_MINE);
	if(role_level_cfg) {
		msg.is_build_gold_mine =
				role_level_cfg->max_building_num > dragon_vale_detail_->build_gold_mine_num ? 1 : 0;
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_MOON_WELL);
	if(role_level_cfg) {
		msg.is_build_moon_well = role_level_cfg->max_building_num > dragon_vale_detail_->build_moon_well_num ? 1 : 0;
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_TOTEM);
	if(role_level_cfg) {
		msg.is_build_totem = role_level_cfg->max_building_num > dragon_vale_detail_->build_totem_num ? 1 : 0;
	}

	msg.build_queue_num = dragon_vale_detail_->build_queue_num;
	msg.all_open_build_queue_num = dragon_vale_detail_->all_open_build_queue_num;

	for(DragonBuildingInfoMap::const_iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
			it != dragon_vale_detail_->dragon_building_info_map.end(); ++it) {
		Building_Info building_info;
		building_info.id = it->second.id;
		building_info.type = it->second.type;
		building_info.level = it->second.level;
		building_info.status = it->second.status;
		building_info.production = it->second.production;

		if(it->second.status == BUILDING_UPGRADE_DOING || !it->second.is_gain) {
			building_info.is_gain = 0;
		}
		else {
			building_info.is_gain = 1;
		}

		building_info.order = it->second.order;
		building_info.consume_type = it->second.consume_type;

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), it->second.type);
		if(!role_level_cfg) {
			continue;
		}

		if(it->second.level >= role_level_cfg->max_building_level) {
			msg.building_info_list.push_back(building_info);
			continue;
		}

		uint16_t new_level = it->second.level + 1;
		const Building_Level_Cfg* building_level_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, new_level);
		if(!building_level_cfg) {
			continue;
		}

		long total_time = 0;

		if(0 == it->second.level) {
			const Create_Building_Cfg *create_building_cfg =
							CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
			if(!create_building_cfg) {
				continue;
			}
			total_time = it->second.begin_upgrade_time.sec()
					+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
					+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
					+ (long)(create_building_cfg->create_need_second);
		}
		else {
			total_time = it->second.begin_upgrade_time.sec()
					+ building_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
					+ building_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
					+ building_level_cfg->upgrade_need_second;
		}

		building_info.left_time = (total_time > now.sec()) ? total_time - now.sec() : 0;

		msg.building_info_list.push_back(building_info);
	}

	//头像区
	msg.head_info.role_id = player_self()->role_id();
	msg.head_info.role_name = player_self()->role_name();
	msg.head_info.star_num = dragon_vale_detail_->star_num;
	msg.head_info.head_photo_id = player_self()->friend_detail().head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(dragon_vale_detail_->star_num);
	if(!star_add_speed) {
		msg.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", dragon_vale_detail_->star_num);
	}
	else {
		msg.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg.head_info.thief_sub_speed =
		(uint16_t)(dragon_vale_detail_->thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = dragon_vale_detail_->foot_info_map.begin();
			it != dragon_vale_detail_->foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg.head_info.foot_list.push_back(msg_foot_info);
	}

	//龙
	msg.is_dragon_born = dragon_vale_detail_->is_dragon_born ? 1 : 0;

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = dragon_vale_detail_->begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;
	msg.born_time = (uint32_t)((total_time > now.sec()) ? (total_time - now.sec()) : 0);
	msg.progress = dragon_vale_detail_->dragon_info.progress;
	msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;

	//掠夺相关
	if(!dragon_vale_detail_->is_rob_protect) {
		msg.protect_time = 0;
	}
	else {
		total_time = dragon_vale_detail_->begin_rob_protect_time.sec()
						+ main_base_cfg.rob_time_hour * Time_Value::ONE_HOUR_IN_SECS
						+ main_base_cfg.rob_time_minute * Time_Value::ONE_MINUTE_IN_SECS;
		msg.protect_time = (total_time > now.sec()) ? (total_time - now.sec()) : 0;
	}

	msg.is_first_enter_after_robbed = dragon_vale_detail_->is_first_enter_after_robbed ? 1 : 0;

	if(dragon_vale_detail_->is_first_enter_after_robbed) {
		dragon_vale_detail_->is_first_enter_after_robbed = false;
		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();
	}

	THIS_SEND_TO_CLIENT(msg);

	{
		MSG_82300039 msg_82300039;
		for(Cancel_Create_Building_Set::const_iterator it = dragon_vale_detail_->cancel_create_building_set.begin();
				it != dragon_vale_detail_->cancel_create_building_set.end();
				++it) {
			msg_82300039.number_list.push_back(*it);
		}

		THIS_SEND_TO_CLIENT(msg_82300039);
	}
}

void Dragon_Vale::enter_other_dragon_vale(role_id_t role_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
		enter_other_offline_player_dragon_vale(role_id);
		return;
	}

	MSG_82300003 msg;
	Time_Value now = Time_Value::gettimeofday();
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	//建筑
	long total_time = 0;
	const Building_Level_Cfg *build_level_cfg = 0;

	DragonBuildingInfoMap temp;
	temp.clear();
	temp.insert(player->dragon_vale_detail().dragon_building_info_map.begin(),
				player->dragon_vale_detail().dragon_building_info_map.end());

	const Role_Level_Cfg *role_level_cfg = 0;

	for(DragonBuildingInfoMap::const_iterator it = temp.begin(); it != temp.end(); ++it) {
		Building_Info building_info;
		building_info.id = it->second.id;
		building_info.type = it->second.type;
		building_info.level = it->second.level;
		building_info.status  = it->second.status;
		building_info.production = it->second.production;

		if(it->second.status == BUILDING_UPGRADE_DOING || !it->second.is_gain) {
			building_info.is_gain = 0;
		}
		else {
			building_info.is_gain = 1;
		}

		building_info.order = it->second.order;
		building_info.consume_type = it->second.consume_type;

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), it->second.type);
		if(!role_level_cfg) {
			continue;
		}

		if(it->second.level >= role_level_cfg->max_building_level) {
			msg.building_info_list.push_back(building_info);
			continue;
		}

		uint16_t new_level = it->second.level + 1;
		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, new_level);
		if(!build_level_cfg) {
			continue;
		}

		if(0 == it->second.level) {
			const Create_Building_Cfg *create_building_cfg =
							CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
			if(!create_building_cfg) {
				continue;
			}
			total_time = it->second.begin_upgrade_time.sec()
					+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
					+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
					+ (long)(create_building_cfg->create_need_second);
		}
		else {
			total_time = it->second.begin_upgrade_time.sec()
					+ build_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
					+ build_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
					+ build_level_cfg->upgrade_need_second;
		}

		building_info.left_time = (total_time > now.sec()) ? (total_time - now.sec()) : 0;

		msg.building_info_list.push_back(building_info);
	}

	temp.clear();

	//头像区
	msg.head_info.role_id = player->role_id();
	msg.head_info.role_name = player->role_name();
	msg.head_info.star_num = player->dragon_vale_detail().star_num;
	msg.head_info.head_photo_id = player->friend_detail().head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(player->dragon_vale_detail().star_num);
	if(!star_add_speed) {
		msg.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", player->dragon_vale_detail().star_num);
	}
	else {
		msg.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg.head_info.thief_sub_speed =
		(uint16_t)(player->dragon_vale_detail().thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = player->dragon_vale_detail().foot_info_map.begin();
			it != player->dragon_vale_detail().foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg.head_info.foot_list.push_back(msg_foot_info);
	}

	//龙
	msg.is_dragon_born = player->dragon_vale_detail_no_const().is_dragon_born ? 1 : 0;

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	total_time = player->dragon_vale_detail_no_const().begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;
	msg.born_time = (uint32_t)((total_time > now.sec()) ? (total_time - now.sec()) : 0);
	msg.progress = player->dragon_vale_detail_no_const().dragon_info.progress;
	msg.is_play_dragon = player->dragon_vale_detail_no_const().dragon_info.is_play_dragon ? 1 : 0;

	if(dragon_vale_detail_->foot_id_set.find(role_id) == dragon_vale_detail_->foot_id_set.end()) {
		msg.is_can_step_foot = 1;
	}
	else {
		msg.is_can_step_foot = 0;
	}

	THIS_SEND_TO_CLIENT(msg);
	return;
}

void Dragon_Vale::enter_other_offline_player_dragon_vale(role_id_t role_id) {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();
	DRAGON_VALE_MANAGER->process_offline_tick(*(find_it->second), now);

	MSG_82300003 msg;
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	//建筑
	long total_time = 0;
	const Building_Level_Cfg *build_level_cfg = 0;

	DragonBuildingInfoMap temp;
	temp.clear();
	temp.insert(find_it->second->dragon_vale_view.dragon_building_info_map.begin(),
			find_it->second->dragon_vale_view.dragon_building_info_map.end());

	const Role_Level_Cfg *role_level_cfg = 0;

	for(DragonBuildingInfoMap::const_iterator it = temp.begin(); it != temp.end(); ++it) {
		Building_Info building_info;
		building_info.id = it->second.id;
		building_info.type = it->second.type;
		building_info.level = it->second.level;
		building_info.status  = it->second.status;
		building_info.production = it->second.production;

		if(it->second.status == BUILDING_UPGRADE_DOING || !it->second.is_gain) {
			building_info.is_gain = 0;
		}
		else {
			building_info.is_gain = 1;
		}

		building_info.order = it->second.order;
		building_info.consume_type = it->second.consume_type;

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), it->second.type);
		if(!role_level_cfg) {
			continue;
		}

		if(it->second.level >= role_level_cfg->max_building_level) {
			msg.building_info_list.push_back(building_info);
			continue;
		}

		uint16_t new_level = it->second.level + 1;
		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, new_level);
		if(!build_level_cfg) {
			continue;
		}

		if(0 == it->second.level) {
			const Create_Building_Cfg *create_building_cfg =
							CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
			if(!create_building_cfg) {
				continue;
			}
			total_time = it->second.begin_upgrade_time.sec()
					+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
					+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
					+ (long)(create_building_cfg->create_need_second);
		}
		else {
			total_time = it->second.begin_upgrade_time.sec()
					+ build_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
					+ build_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
					+ build_level_cfg->upgrade_need_second;
		}

		building_info.left_time = (total_time > now.sec()) ? (total_time - now.sec()) : 0;

		msg.building_info_list.push_back(building_info);
	}

	temp.clear();

	//头像区
	msg.head_info.role_id =  find_it->second->role_id;
	msg.head_info.role_name =  find_it->second->role_name;
	msg.head_info.star_num = find_it->second->dragon_vale_view.star_num;
	msg.head_info.head_photo_id = find_it->second->friend_.head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(find_it->second->dragon_vale_view.star_num);
	if(!star_add_speed) {
		msg.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", find_it->second->dragon_vale_view.star_num);
	}
	else {
		msg.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg.head_info.thief_sub_speed =
		(uint16_t)(find_it->second->dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = find_it->second->dragon_vale_view.foot_info_map.begin();
			it != find_it->second->dragon_vale_view.foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg.head_info.foot_list.push_back(msg_foot_info);
	}

	//龙
	msg.is_dragon_born = find_it->second->dragon_vale_view.is_dragon_born ? 1 : 0;

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	total_time = find_it->second->dragon_vale_view.begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;
	msg.born_time = (uint32_t)((total_time > now.sec()) ? (total_time - now.sec()) : 0);
	msg.progress = find_it->second->dragon_vale_view.dragon_info.progress;
	msg.is_play_dragon = find_it->second->dragon_vale_view.dragon_info.is_play_dragon ? 1 : 0;

	if(dragon_vale_detail_->foot_id_set.find(role_id) == dragon_vale_detail_->foot_id_set.end()) {
		msg.is_can_step_foot = 1;
	}
	else {
		msg.is_can_step_foot = 0;
	}

	THIS_SEND_TO_CLIENT(msg);
}

void Dragon_Vale::dragon_vale_listen_level_up(int new_level) {
	//不在龙谷里面不发消息
	if(!dragon_vale_detail_->target_player_id) {
		notify_has_can_create_dragon_building_num();
		return;
	}

	MSG_82300002 msg;
	msg.role_id = player_self()->role_id();

	const Role_Level_Cfg *role_level_cfg = 0;
	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(new_level, BUILDING_GOLD_MINE);
	if(role_level_cfg) {
		msg.is_build_gold_mine = (role_level_cfg->max_building_num > dragon_vale_detail_->build_gold_mine_num ? 1 : 0);
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(new_level, BUILDING_MOON_WELL);
	if(role_level_cfg) {
		msg.is_build_moon_well = (role_level_cfg->max_building_num > dragon_vale_detail_->build_moon_well_num ? 1 : 0);
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(new_level, BUILDING_TOTEM);
	if(role_level_cfg) {
		msg.is_build_totem = (role_level_cfg->max_building_num > dragon_vale_detail_->build_totem_num ? 1 : 0);
	}

	msg.build_queue_num = dragon_vale_detail_->build_queue_num;
	THIS_SEND_TO_CLIENT(msg);

	notify_has_can_create_dragon_building_num();
}

int Dragon_Vale::req_add_new_building(uint32_t building_id, uint8_t building_type, uint8_t consume_type) {
	if(1 != consume_type && 2 != consume_type) {
		return ERROR_CLIENT_PARAM;
	}

	const Building_Level_Cfg *build_level_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(building_type, (uint16_t)1);
	if(!build_level_cfg) {
		return ERROR_BUILDING_LEVEL_CFG_NOT_EXIST;
	}

	uint64_t key = create_key_of_Dragon_Building_Info(building_id, building_type);
	DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.find(key);
	if(it != dragon_vale_detail_->dragon_building_info_map.end()) {
		return ERROR_BUILDING_EXIST;
	}

	if(dragon_vale_detail_->build_queue_num < 1) {
		return ERROR_BUILDING_QUEUE_NOT_ENGOUGH;
	}

	const Role_Level_Cfg *role_level_cfg = 0;
	if(BUILDING_GOLD_MINE == building_type) {
		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_GOLD_MINE);
		if(!role_level_cfg) {
			return ERROR_BUILDING_ROLE_LEVEL_CFG_NOT_EXIST;
		}
		if(dragon_vale_detail_->build_gold_mine_num >= role_level_cfg->max_building_num) {
			return ERROR_GOLD_MINE_MAX_NUM_LIMIT;
		}
	}

	if(BUILDING_MOON_WELL == building_type) {
		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_MOON_WELL);
		if(!role_level_cfg) {
			return ERROR_BUILDING_ROLE_LEVEL_CFG_NOT_EXIST;
		}
		if(dragon_vale_detail_->build_moon_well_num >= role_level_cfg->max_building_num) {
			return ERROR_MOON_WEEL_MAX_NUM_LIMIT;
		}
	}

	if(BUILDING_TOTEM == building_type) {
		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_TOTEM);
		if(!role_level_cfg) {
			return ERROR_BUILDING_ROLE_LEVEL_CFG_NOT_EXIST;
		}
		if(dragon_vale_detail_->build_totem_num >= role_level_cfg->max_building_num) {
			return ERROR_TOTEM_MAX_NUM_LIMIT;
		}
	}

	uint8_t order = 0;

	if(dragon_vale_detail_->cancel_create_building_set.empty()) {
		uint8_t num = get_dragon_building_total_num();
		order = num + 1;
	}
	else {
		Cancel_Create_Building_Set::const_iterator cancel_it = dragon_vale_detail_->cancel_create_building_set.begin();
		if(cancel_it != dragon_vale_detail_->cancel_create_building_set.end()) {
			order = *cancel_it;
		}
		else {
			uint8_t num = get_dragon_building_total_num();
			order = num + 1;
		}
	}

	const Create_Building_Cfg *create_building_cfg =
					CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(order);
	if(!create_building_cfg) {
		return ERROR_NO_CREATE_BUILDING_CFG;
	}

	//消耗货币
	uint8_t money_type = 0;
	uint32_t money_num = 0;
	if(1 == consume_type) {
		money_type = create_building_cfg->create_money_type;
		money_num = create_building_cfg->create_money_num;
	}
	else {
		money_type = create_building_cfg->create_money_type2;
		money_num = create_building_cfg->create_money_num2;
	}

	int ret = 0;
	if(money_num > 0) {
		ret = consume_money_in_dragon_vale(money_type, money_num, MONEY_SUB_MONEY_CREATE_DRAGON_VALE_BUILDING);
	}

	if(ret != 0) {
		return ret;
	}

	Cancel_Create_Building_Set::const_iterator cancel_it =
			dragon_vale_detail_->cancel_create_building_set.find(order);
	if(cancel_it != dragon_vale_detail_->cancel_create_building_set.end()) {
		dragon_vale_detail_->cancel_create_building_set.erase(cancel_it);
	}

	DragonBuildingInfo dragon_building_info;
	dragon_building_info.id = building_id;
	dragon_building_info.type = building_type;
	dragon_building_info.level = 0; //建造完成后，等级才变为1
	dragon_building_info.status = BUILDING_UPGRADE_DOING;
	dragon_building_info.production  = 0;
	dragon_building_info.is_gain = false;
	Time_Value now = Time_Value::gettimeofday();
	dragon_building_info.begin_upgrade_time.set(now);
	dragon_building_info.last_output_production_time.set(now);
	dragon_building_info.order = order;
	dragon_building_info.consume_type = consume_type;

	dragon_vale_detail_->dragon_building_info_map.insert(std::make_pair(key, dragon_building_info));

	if(BUILDING_GOLD_MINE == building_type) {
		dragon_vale_detail_->build_gold_mine_num += 1;
	}

	if(BUILDING_MOON_WELL == building_type) {
		dragon_vale_detail_->build_moon_well_num += 1;
	}


	if(BUILDING_TOTEM == building_type) {
		dragon_vale_detail_->build_totem_num  += 1;
	}

	dragon_vale_detail_->build_queue_num =
			(dragon_vale_detail_->build_queue_num >= 1) ? (dragon_vale_detail_->build_queue_num - 1) : 0;

	this->save_dragon_vale_tick();
	this->sync_dragon_vale_to_db_view();

	MSG_50180000 msg;
	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_GOLD_MINE);
	if(role_level_cfg) {
		msg.is_build_gold_mine =
				role_level_cfg->max_building_num > dragon_vale_detail_->build_gold_mine_num ? 1 : 0;
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_MOON_WELL);
	if(role_level_cfg) {
		msg.is_build_moon_well = role_level_cfg->max_building_num > dragon_vale_detail_->build_moon_well_num ? 1 : 0;
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_TOTEM);
	if(role_level_cfg) {
		msg.is_build_totem = role_level_cfg->max_building_num > dragon_vale_detail_->build_totem_num ? 1 : 0;
	}

	msg.build_queue_num = dragon_vale_detail_->build_queue_num;
	msg.building_info.id = dragon_building_info.id;
	msg.building_info.type = dragon_building_info.type;
	msg.building_info.level = dragon_building_info.level;
	msg.building_info.status = dragon_building_info.status;
	msg.building_info.production = dragon_building_info.production;
	msg.building_info.is_gain = dragon_building_info.is_gain ? 1 : 0;
	msg.building_info.left_time = create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS
			            + create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS
			            + create_building_cfg->create_need_second;
	msg.building_info.order = dragon_building_info.order;
	msg.building_info.consume_type = dragon_building_info.consume_type;

	//boradcast to dragon vale scene
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	{
		MSG_82300039 msg_82300039;
		for(Cancel_Create_Building_Set::const_iterator it = dragon_vale_detail_->cancel_create_building_set.begin();
				it != dragon_vale_detail_->cancel_create_building_set.end();
				++it) {
			msg_82300039.number_list.push_back(*it);
		}
		THIS_SEND_TO_CLIENT(msg_82300039);
	}

	//同步到db view
	sync_dragon_vale_to_db_view();

	player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_DRAGON_VALE_BUILDING_CREATE, building_type);
	player_self()->task_listen_create_dragon_vale_building(building_type);

	if(dragon_vale_detail_->dragon_building_info_map.size() == 1) {
		DRAGON_VALE_MANAGER->refresh_dragon_vale_op(player_self()->role_id());
	}
	return 0;
}

int Dragon_Vale::req_upgrade_building(uint32_t building_id, uint8_t building_type) {
	const Role_Level_Cfg *role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(
			                                  player_self()->level(),
			                                  building_type);
	if(!role_level_cfg) {
		return ERROR_BUILDING_ROLE_LEVEL_CFG_NOT_EXIST;
	}

	uint64_t key = create_key_of_Dragon_Building_Info(building_id, building_type);
	DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.find(key);
	if(it == dragon_vale_detail_->dragon_building_info_map.end()) {
		return ERROR_BUILDING_NOT_EXIST;
	}

	if(BUILDING_UPGRADE_DOING == it->second.status) {
		if(it->second.level == 0) {
			return ERROR_BUILDING_IN_CREATEING_NO_UPGRADE;
		}
		else {
			return ERROR_BUILDING_IN_UPGRADEING_NO_UPGRADE;
		}
	}

	if(dragon_vale_detail_->build_queue_num < 1) {
		return ERROR_BUILDING_QUEUE_NOT_ENGOUGH;
	}

	if(it->second.level >= role_level_cfg->max_building_level) {
		return ERROR_BUILDING_MAX_LEVEL_LIMIT;
	}

	uint16_t new_level = it->second.level + 1;
	const Building_Level_Cfg *build_level_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(building_type, new_level);
	if(!build_level_cfg) {
		return ERROR_BUILDING_LEVEL_CFG_NOT_EXIST;
	}

	//消耗货币
	int ret = consume_money_in_dragon_vale(build_level_cfg->upgrade_money_type,
				build_level_cfg->upgrade_money_num,
				MONEY_SUB_MONEY_UPGRADE_DRAGON_VALE_BUILDING);
	if(ret != 0) {
		return ret;
	}

	//如果有未收获的资源，升级建筑前先收获资源
	if(it->second.production >= build_level_cfg->min_gain_limit) {
		//收获资源获得货币
		uint8_t money_type = get_production_type(building_type);
		gain_money_in_dragon_vale(money_type,
				it->second.production,
				Pack::MONEY_GAIN_PRODUCTION_IN_DRAGON_VALE);

		MSG_50180004 msg;
		msg.gain_production = it->second.production;

		it->second.production = 0;
		it->second.is_gain = false;

		msg.building_info.id = it->second.id;
		msg.building_info.type = it->second.type;
		msg.building_info.level = it->second.level;
		msg.building_info.status = it->second.status;
		msg.building_info.production = it->second.production;
		msg.building_info.is_gain = it->second.is_gain ? 1 : 0;
		msg.building_info.order = it->second.order;
		msg.building_info.consume_type = it->second.consume_type;

		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();

		//boradcast to dragon vale scene
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(player_self()->role_id());
		broad_cast_buf.copy(&buf);
		broad_cast_buf.finish_message();
	    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);
	}

	it->second.is_gain =  false;
	it->second.status = BUILDING_UPGRADE_DOING;
	Time_Value now = Time_Value::gettimeofday();
	it->second.begin_upgrade_time.set(now);

	dragon_vale_detail_->build_queue_num =
			(dragon_vale_detail_->build_queue_num >= 1) ? (dragon_vale_detail_->build_queue_num - 1) : 0;

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	MSG_50180001 msg;
	msg.build_queue_num = dragon_vale_detail_->build_queue_num;
	msg.building_info.id = it->second.id;
	msg.building_info.type = it->second.type;
	msg.building_info.level = it->second.level;
	msg.building_info.status = it->second.status;
	msg.building_info.production = it->second.production;
	msg.building_info.is_gain = it->second.is_gain ? 1 : 0;
	msg.building_info.left_time = build_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
			            + build_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
			            + build_level_cfg->upgrade_need_second;
	msg.building_info.order = it->second.order;
	msg.building_info.consume_type = it->second.consume_type;

	//boradcast to dragon vale scene
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	player_self()->task_listen_dragon_vale_building_upgrade(it->second.type, it->second.level + 1);
	return 0;
}

int Dragon_Vale::consume_money_in_dragon_vale(uint8_t money_type, uint32_t money_num, uint32_t lose_money_type) {
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

int Dragon_Vale::gain_money_in_dragon_vale(uint8_t money_type, uint32_t money_num,
				uint32_t gain_money_type, bool is_record /*= true*/) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;

	enum Money_Type add_money_type;
	switch(money_type) {
		case COPPER_ONLY:
			add_money_type = COPPER;
			break;
		case BIND_GOLD_FIRST:
			add_money_type = BIND_GOLD;
			break;
		case GOLD_ONLY:
			add_money_type = GOLD;
			break;
		case BIND_GOLD_ONLY:
			add_money_type = BIND_GOLD;
			break;
		case SOULS_ONLY:
			add_money_type = SOULS;
			break;
		case FRIENDSHIP_ONLY:
			add_money_type = FRIENDSHIP;
			break;
		case DRAGON_SOUL_ONLY:
			add_money_type = DRAGON_SOUL;
			break;
		default:
			add_money_type = MONEY_TYPE_END;
			break;
	}

	if (add_money_type != MONEY_TYPE_END && money_num > 0) {
		acv_msg.property.push_back(Property(add_money_type, money_num));
	}

	Money_Add_Info money_add_info(add_money_type,
			                      money_num,
			                      Money_DM_Info(gain_money_type));
	player_self()->pack_add_money(money_add_info, Pack::MONEY_OPT_REAL);

	if (!acv_msg.property.empty() && is_record) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return 0;
}

int Dragon_Vale::req_quick_finish_building(uint32_t building_id, uint8_t building_type) {
	uint64_t key = create_key_of_Dragon_Building_Info(building_id, building_type);
	DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.find(key);
	if(it == dragon_vale_detail_->dragon_building_info_map.end()) {
		return ERROR_BUILDING_NOT_EXIST;
	}

	if(BUILDING_UPGRADE_DOING != it->second.status) {
		return ERROR_NOT_UPGRADING_NO_QUICK_FINSH;
	}

	uint16_t new_level = it->second.level + 1;
	const Building_Level_Cfg *build_level_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(building_type, new_level);
	if(!build_level_cfg) {
		return ERROR_BUILDING_LEVEL_CFG_NOT_EXIST;
	}

	const Role_Level_Cfg *role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(
			                                  player_self()->level(),
			                                  building_type);
	if(!role_level_cfg) {
		return ERROR_BUILDING_ROLE_LEVEL_CFG_NOT_EXIST;
	}

	Time_Value now = Time_Value::gettimeofday();
	long total_time = 0;

	if(0 == it->second.level) {
		const Create_Building_Cfg *create_building_cfg =
						CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
		if(!create_building_cfg) {
			return ERROR_NO_CREATE_BUILDING_CFG;
		}
		total_time = it->second.begin_upgrade_time.sec()
				+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
				+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
				+ (long)(create_building_cfg->create_need_second);
	}
	else {
		total_time = it->second.begin_upgrade_time.sec()
				+ build_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
				+ build_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
				+ build_level_cfg->upgrade_need_second;
	}

	long left_time = (total_time > now.sec()) ? (total_time - now.sec()) : 0;
	if(left_time == 0) {
		return ERROR_BUIDING_FINISH_NO_QUICK_FINISH;
	}

	uint32_t minute_prev =  left_time / Time_Value::ONE_MINUTE_IN_SECS;
	uint32_t minute_post = left_time % Time_Value::ONE_MINUTE_IN_SECS;
	uint32_t minute = minute_prev + (minute_post > 0 ? 1 : 0);

	const Quick_Upgrade_Cfg *quick_upgrade_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Quick_Upgrade_Cfg(1);
	if(!quick_upgrade_cfg) {
		return ERROR_QUICK_FINISH_CFG_NOT_EXIST;
	}

	//消耗货币
	if(minute > 0) {
		int ret = consume_money_in_dragon_vale(quick_upgrade_cfg->quick_upgrade_money_type,
					quick_upgrade_cfg->quick_upgrade_money_ratio * minute,
					MONEY_SUB_MONEY_QUICK_FINISH_VALE_BUILDING);
		if(ret != 0) {
			return ret;
		}
	}

	it->second.level += 1;
	it->second.status = BUILDING_UPGRADE_FINISH;
	//加速建造或者升级完成，将上一次资源产出时间设置为当前时间(因为建造或者升级中，不能产出资源)
	it->second.last_output_production_time.set(now);
	dragon_vale_detail_->build_queue_num += 1;

	if(1 == it->second.level && dragon_vale_detail_->dragon_building_info_map.size() == 1) {
		dragon_vale_detail_->begin_refresh_thief_time.set(now);
	}

	this->save_dragon_vale_tick();

	MSG_82300002 msg;
	msg.role_id = player_self()->role_id();

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_GOLD_MINE);
	if(role_level_cfg) {
		msg.is_build_gold_mine =
				role_level_cfg->max_building_num > dragon_vale_detail_->build_gold_mine_num ? 1 : 0;
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_MOON_WELL);
	if(role_level_cfg) {
		msg.is_build_moon_well = role_level_cfg->max_building_num > dragon_vale_detail_->build_moon_well_num ? 1 : 0;
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_TOTEM);
	if(role_level_cfg) {
		msg.is_build_totem = role_level_cfg->max_building_num > dragon_vale_detail_->build_totem_num ? 1 : 0;
	}

	msg.build_queue_num = dragon_vale_detail_->build_queue_num;

	Building_Info  msg_building_info;
	msg_building_info.id = it->second.id;
	msg_building_info.type = it->second.type;
	msg_building_info.level = it->second.level;
	msg_building_info.status = it->second.status;
	msg_building_info.production = it->second.production;
	msg_building_info.is_gain = it->second.is_gain ? 1 : 0;
	msg_building_info.order = it->second.order;
	msg_building_info.consume_type = it->second.consume_type;

	msg.building_info_list.push_back(msg_building_info);

	//在龙谷里面才会发消息
//	if(dragon_vale_detail_->target_player_id != 0) {
//		THIS_SEND_TO_CLIENT(msg);
//	}

	//boradcast to dragon vale scene
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	sync_dragon_vale_to_db_view();

	switch(building_type) {
	case BUILDING_GOLD_MINE: {
		player_self()->achievement_listen_dragon_wale_gold_mine_lvl(it->second.level);
		break;
	}
	case BUILDING_MOON_WELL: {
		player_self()->achievement_listen_dragon_wale_moon_well_lvl(it->second.level);
		break;
	}
	case BUILDING_TOTEM: {
		player_self()->achievement_listen_dragon_wale_totem_lvl(it->second.level);
		break;
	}
	default:
		break;
	}
	return 0;
}

int Dragon_Vale::req_cancel_finish_building(uint32_t building_id, uint8_t building_type) {
	uint64_t key = create_key_of_Dragon_Building_Info(building_id, building_type);
	DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.find(key);
	if(it == dragon_vale_detail_->dragon_building_info_map.end()) {
		return ERROR_BUILDING_NOT_EXIST;
	}

	if(BUILDING_UPGRADE_DOING != it->second.status) {
		return ERROR_NOT_UPGRADING_NO_CANCEL;
	}

	uint16_t new_level = it->second.level + 1;
	const Building_Level_Cfg *build_level_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(building_type, new_level);
	if(!build_level_cfg) {
		return ERROR_BUILDING_LEVEL_CFG_NOT_EXIST;
	}

	const Role_Level_Cfg *role_level_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(),
					building_type);
	if(!role_level_cfg) {
		return ERROR_BUILDING_ROLE_LEVEL_CFG_NOT_EXIST;
	}


	if(0 == it->second.level) {
		const Create_Building_Cfg *create_building_cfg =
						CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
		if(!create_building_cfg) {
			return ERROR_NO_CREATE_BUILDING_CFG;
		}
		Time_Value now = Time_Value::gettimeofday();
		long total_time = it->second.begin_upgrade_time.sec()
				+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
				+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
				+ (long)(create_building_cfg->create_need_second);
		long left_time = (total_time > now.sec()) ? (total_time - now.sec()) : 0;
		if(left_time == 0) {
			return ERROR_BUILDING_FINISH_NO_CANCEL;
		}
		//取消建造返还货币
		uint8_t money_type = 0;
		uint32_t money_num = 0;
		if(1 == it->second.consume_type) {
			money_type = create_building_cfg->create_money_type;
			money_num = create_building_cfg->cancel_create_return_money_num;
		} else {
			money_type = create_building_cfg->create_money_type2;
			money_num = create_building_cfg->cancel_create_return_2;
		}

		if(money_num > 0) {
			gain_money_in_dragon_vale(money_type, money_num, Pack::MONEY_CANCEL_CREATE_DRAGON_VALE_BUILDING);
		}
	}
	else {
		Time_Value now = Time_Value::gettimeofday();
		long total_time = it->second.begin_upgrade_time.sec()
				+ (long)(build_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS)
				+ (long)(build_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS)
				+ (long)(build_level_cfg->upgrade_need_second);
		long left_time = (total_time > now.sec()) ? (total_time - now.sec()) : 0;
		if(left_time == 0) {
			return ERROR_BUILDING_FINISH_NO_CANCEL;
		}

		//取消升级返还货币
		if(build_level_cfg->cancel_upgrade_return_money_num > 0) {
			gain_money_in_dragon_vale(build_level_cfg->upgrade_money_type,
					build_level_cfg->cancel_upgrade_return_money_num,
					Pack::MONEY_CANCEL_UPGRADE_DRAGON_VALE_BUILDING);
		}
	}

	MSG_50180003 msg;

	//取消建造
	if(it->second.level == 0) {
		dragon_vale_detail_->build_gold_mine_num =
				(dragon_vale_detail_->build_gold_mine_num > 1) ? (dragon_vale_detail_->build_gold_mine_num - 1) : 0;
		dragon_vale_detail_->build_moon_well_num =
				(dragon_vale_detail_->build_moon_well_num > 1) ? (dragon_vale_detail_->build_moon_well_num - 1) : 0;
		dragon_vale_detail_->build_totem_num =
				(dragon_vale_detail_->build_totem_num > 1) ? (dragon_vale_detail_->build_totem_num - 1) : 0;
	}

	//取消建造、 取消升级都会执行
	dragon_vale_detail_->build_queue_num += 1;
	it->second.status = BUILDING_UPGRADE_NO;

	//取消建造或者升级，将上一次资源产出时间设置为当前时间(因为建造或者升级中，不能产出资源)
	Time_Value now = Time_Value::gettimeofday();
	it->second.last_output_production_time.set(now);

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_GOLD_MINE);
	if(role_level_cfg) {
		msg.is_build_gold_mine =
				role_level_cfg->max_building_num > dragon_vale_detail_->build_gold_mine_num ? 1 : 0;
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_MOON_WELL);
	if(role_level_cfg) {
		msg.is_build_moon_well = role_level_cfg->max_building_num > dragon_vale_detail_->build_moon_well_num ? 1 : 0;
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_TOTEM);
	if(role_level_cfg) {
		msg.is_build_totem = role_level_cfg->max_building_num > dragon_vale_detail_->build_totem_num ? 1 : 0;
	}

	msg.build_queue_num = dragon_vale_detail_->build_queue_num;
	msg.building_info.id = it->second.id;
	msg.building_info.type = it->second.type;
	msg.building_info.level = it->second.level;
	msg.building_info.status = it->second.status;
	msg.building_info.production = it->second.production;
	msg.building_info.is_gain = it->second.is_gain ? 1 : 0;
	msg.building_info.order = it->second.order;
	msg.building_info.consume_type = it->second.consume_type;

//	THIS_SEND_TO_CLIENT(msg);
	//boradcast to dragon vale scene
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	//取消建造,删除建筑
	if(it->second.level == 0) {
		dragon_vale_detail_->cancel_create_building_set.insert(it->second.order);
		dragon_vale_detail_->dragon_building_info_map.erase(key);
		{
			MSG_82300039 msg_82300039;
			for(Cancel_Create_Building_Set::const_iterator it = dragon_vale_detail_->cancel_create_building_set.begin();
					it != dragon_vale_detail_->cancel_create_building_set.end();
					++it) {
				msg_82300039.number_list.push_back(*it);
			}
			THIS_SEND_TO_CLIENT(msg_82300039);
		}
	}

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	return 0;
}

int Dragon_Vale::req_gain_production(uint32_t building_id, uint8_t building_type) {
	if(dragon_vale_detail().target_player_id != player_self()->role_id()) {
		return ERROR_NO_GAIN_OTHER_DRAGON_VALE_PRODUCTION;
	}

	uint64_t key = create_key_of_Dragon_Building_Info(building_id, building_type);
	DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.find(key);
	if(it == dragon_vale_detail_->dragon_building_info_map.end()) {
		return ERROR_BUILDING_NOT_EXIST;
	}

	const Building_Level_Cfg *build_level_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(building_type, it->second.level);
	if(!build_level_cfg) {
		return ERROR_BUILDING_LEVEL_CFG_NOT_EXIST;
	}

	if(BUILDING_UPGRADE_DOING == it->second.status) {
		return ERROR_UPGRADING_NO_GAIN_PRODUCTION;
	}

	if(it->second.production == 0 || it->second.production < build_level_cfg->min_gain_limit) {
		return ERROR_MIN_GAIN_PRODUCTION_LIMIT;
	}

	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_DRAGON_HARVEST);//日常助手监听龙谷收获

	//收获资源获得货币
	uint8_t money_type = get_production_type(building_type);
	gain_money_in_dragon_vale(money_type,
			it->second.production,
			Pack::MONEY_GAIN_PRODUCTION_IN_DRAGON_VALE);

	//重置上一次建筑资源产出时间
	Time_Value now = Time_Value::gettimeofday();
	it->second.last_output_production_time.set(now);

	MSG_50180004 msg;
	msg.gain_production = it->second.production;

	it->second.production = 0;
	it->second.is_gain = false;

	if(BUILDING_GOLD_MINE == it->second.type) {
		dragon_vale_detail_->is_gold_mine_max_storage = false;
		player_self()->listen_vip_reward(VIP_WORLD_TREE_GAIN_DRAGON, COPPER, msg.gain_production);
	}

	if(BUILDING_MOON_WELL == it->second.type) {
		dragon_vale_detail_->is_moon_well_max_storage = false;
		player_self()->listen_vip_reward(VIP_WORLD_TREE_GAIN_DRAGON, SOULS, msg.gain_production);
	}

	if(BUILDING_TOTEM == it->second.type) {
		dragon_vale_detail_->is_totem_max_storage = false;
		player_self()->listen_vip_reward(VIP_WORLD_TREE_GAIN_DRAGON, DRAGON_SOUL, msg.gain_production);
	}

	msg.building_info.id = it->second.id;
	msg.building_info.type = it->second.type;
	msg.building_info.level = it->second.level;
	msg.building_info.status = it->second.status;
	msg.building_info.production = it->second.production;
	msg.building_info.is_gain = it->second.is_gain ? 1 : 0;
	msg.building_info.order = it->second.order;
	msg.building_info.consume_type = it->second.consume_type;

//	THIS_SEND_TO_CLIENT(msg);

	//boradcast to dragon vale scene
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	player_self()->task_listen_gain_dragon_vale_builiding_production(building_type);

	return 0;
}

uint8_t Dragon_Vale::get_production_type(uint8_t building_type) {
	uint8_t production_type = 0;
	switch(building_type) {
	case BUILDING_GOLD_MINE:
		production_type = COPPER_ONLY;
		break;
	case BUILDING_MOON_WELL:
		production_type = SOULS_ONLY;
		break;
	case BUILDING_TOTEM:
		production_type = DRAGON_SOUL_ONLY;
		break;
	default:
		MSG_USER("building_type is error, building_type=%d\n", building_type);
		production_type = MONEY_SUB_TYPE_END;
		break;
	}

	return production_type;
}

void Dragon_Vale::listen_building_output_production_by_one_minute(const Time_Value &now) {
	MSG_82300001 msg;
	const Building_Level_Cfg *build_level_cfg = 0;
	long total_time = 0;

	uint32_t production_per_min = 0;
	uint32_t add_speed = 0;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(dragon_vale_detail_->star_num);
	if(!star_add_speed) {
		add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", dragon_vale_detail_->star_num);
	}
	else {
		add_speed = (uint16_t)(*star_add_speed);
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	uint32_t sub_speed =
				(uint32_t)(dragon_vale_detail_->thief_id_quality_map.size() * main_base_cfg.thief_down_production);

	for(DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
			it != dragon_vale_detail_->dragon_building_info_map.end();
			++it) {
		//升级状态，不产出
		if(BUILDING_UPGRADE_DOING == it->second.status) {
			continue;
		}

		//等级为0，说明还未建筑完成，不能产出
		if(0 == it->second.level) {
			continue;
		}

		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, it->second.level);
		if(!build_level_cfg) {
			continue;
		}

		if(it->second.production >= build_level_cfg->max_storage) {
			continue;
		}

		total_time = it->second.last_output_production_time.sec() + 1 * Time_Value::ONE_MINUTE_IN_SECS;
		if(total_time > now.sec()) {
			continue;
		}

		it->second.last_output_production_time.set(now);

		production_per_min = (uint32_t)((100 + add_speed - sub_speed) * build_level_cfg->production_per_min / 100.0);

		it->second.production += production_per_min;
		if(it->second.production > build_level_cfg->max_storage) {
			it->second.production = build_level_cfg->max_storage;
		}

		it->second.is_gain = (it->second.production >= build_level_cfg->min_gain_limit);

		Building_Info msg_building_info;
		msg_building_info.id = it->second.id;
		msg_building_info.type = it->second.type;
		msg_building_info.level = it->second.level;
		msg_building_info.status = it->second.status;
		msg_building_info.production = it->second.production;
		msg_building_info.production_per_minute = production_per_min;
		msg_building_info.is_gain = it->second.is_gain ? 1 : 0;
		msg_building_info.order = it->second.order;
		msg_building_info.consume_type = it->second.consume_type;

		msg.building_info_list.push_back(msg_building_info);
	}

	if(!msg.building_info_list.empty()) {
		//在龙谷里面才会发消息
//		if(dragon_vale_detail_->target_player_id != 0) {
//			THIS_SEND_TO_CLIENT(msg);
//		}

		//boradcast to dragon vale scene
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(player_self()->role_id());
		broad_cast_buf.copy(&buf);
		broad_cast_buf.finish_message();
	    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

		uint32_t gold_mine_max_storage = 0;
		uint32_t moon_weel_max_storage = 0;
	    uint32_t totem_max_storage = 0;
	    uint32_t new_gold_mine_production = 0;
	    uint32_t new_moon_weel_production = 0;
	    uint32_t new_totem_production = 0;

	    //在龙谷里面、外面都会发的消息，客户端弹窗用
	    MSG_82300008 max_msg;
		get_building_max_storage(gold_mine_max_storage, moon_weel_max_storage,
				totem_max_storage, new_gold_mine_production,
				new_moon_weel_production, new_totem_production, false);
		if(!dragon_vale_detail_->is_gold_mine_max_storage && gold_mine_max_storage != 0
				&& new_gold_mine_production != 0 && new_gold_mine_production >= gold_mine_max_storage) {
			max_msg.is_gold_mine = 1;
			dragon_vale_detail_->is_gold_mine_max_storage = true;
		}
		if(!dragon_vale_detail_->is_moon_well_max_storage && moon_weel_max_storage != 0
				&& new_moon_weel_production != 0 && new_moon_weel_production >= moon_weel_max_storage) {
			max_msg.is_moon_weel = 1;
			dragon_vale_detail_->is_moon_well_max_storage = true;
		}
		if(!dragon_vale_detail_->is_totem_max_storage && totem_max_storage != 0
				&& new_totem_production != 0 && new_totem_production >= totem_max_storage) {
			max_msg.is_totem = 1;
			dragon_vale_detail_->is_totem_max_storage = true;
		}

		if(max_msg.is_gold_mine || max_msg.is_moon_weel || max_msg.is_totem) {
			THIS_SEND_TO_CLIENT(max_msg);
		}

		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();
	}
}

void Dragon_Vale::listen_building_finish(const Time_Value &now) {
	MSG_82300002 msg;
	const Building_Level_Cfg *build_level_cfg = 0;
	long total_time = 0;

	//建筑是否建造、升级完成的标志
	uint8_t gold_mine_flag = 0;
	uint8_t moon_well_flag = 0;
	uint8_t totem_flag = 0;

	for(DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
				it != dragon_vale_detail_->dragon_building_info_map.end();
				++it) {
		//不是升级状态，不处理
		if(BUILDING_UPGRADE_DOING != it->second.status) {
			continue;
		}

		uint16_t new_level = it->second.level + 1;
		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, new_level);
		if(!build_level_cfg) {
			continue;
		}

		if(0 == it->second.level) {
			const Create_Building_Cfg *create_building_cfg =
							CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
			if(!create_building_cfg) {
				continue;
			}
			total_time = it->second.begin_upgrade_time.sec()
					+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
					+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
					+ (long)(create_building_cfg->create_need_second);
		}
		else {
			total_time = it->second.begin_upgrade_time.sec()
					+ build_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
					+ build_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
					+ build_level_cfg->upgrade_need_second;
		}

		if(total_time > now.sec()) {
			continue;
		}

		it->second.level += 1;
		it->second.status = BUILDING_UPGRADE_FINISH;
		//建造或者升级完成，将上一次资源产出时间设置为当前时间(因为建造或者升级中，不能产出资源)
		it->second.last_output_production_time.set(now);
		dragon_vale_detail_->build_queue_num += 1;

		if(1 == it->second.level && dragon_vale_detail_->dragon_building_info_map.size() == 1) {
			dragon_vale_detail_->begin_refresh_thief_time.set(now);
		}

		if(BUILDING_GOLD_MINE == it->second.type) {
			if(1 == it->second.level){
				gold_mine_flag = 1;
			}
//			else {
//				gold_mine_flag = 2;
//			}
			player_self()->achievement_listen_dragon_wale_gold_mine_lvl(it->second.level);
		}

		if(BUILDING_MOON_WELL == it->second.type) {
			if(1 == it->second.level) {
				moon_well_flag = 1;
			}
//			else {
//				moon_well_flag = 2;
//			}
			player_self()->achievement_listen_dragon_wale_moon_well_lvl(it->second.level);
		}

		if(BUILDING_TOTEM == it->second.type) {
			if(1 == it->second.level) {
				totem_flag = 1;
			}
//			else {
//				totem_flag = 2;
//			}
			player_self()->achievement_listen_dragon_wale_totem_lvl(it->second.level);
		}

		Building_Info msg_building_info;
		msg_building_info.id = it->second.id;
		msg_building_info.type = it->second.type;
		msg_building_info.level = it->second.level;
		msg_building_info.status = it->second.status;
		msg_building_info.production = it->second.production;
		msg_building_info.is_gain = it->second.is_gain ? 1 : 0;
		msg_building_info.order = it->second.order;
		msg_building_info.consume_type = it->second.consume_type;

		msg.building_info_list.push_back(msg_building_info);
	}//end for

	if(!msg.building_info_list.empty()) {
		msg.role_id = player_self()->role_id();
		const Role_Level_Cfg *role_level_cfg = 0;
		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_GOLD_MINE);
		if(role_level_cfg) {
			msg.is_build_gold_mine =
					role_level_cfg->max_building_num > dragon_vale_detail_->build_gold_mine_num ? 1 : 0;
		}

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_MOON_WELL);
		if(role_level_cfg) {
			msg.is_build_moon_well = role_level_cfg->max_building_num > dragon_vale_detail_->build_moon_well_num ? 1 : 0;
		}

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_TOTEM);
		if(role_level_cfg) {
			msg.is_build_totem = role_level_cfg->max_building_num > dragon_vale_detail_->build_totem_num ? 1 : 0;
		}

		msg.build_queue_num = dragon_vale_detail_->build_queue_num;

		//在龙谷里面才会发消息
//		if(dragon_vale_detail_->target_player_id != 0) {
//			THIS_SEND_TO_CLIENT(msg);
//		}

		//boradcast to dragon vale scene
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(player_self()->role_id());
		broad_cast_buf.copy(&buf);
		broad_cast_buf.finish_message();
	    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();

		//在龙谷里面、外面都会发的消息，客户端弹窗用
		if((gold_mine_flag == 1) || (moon_well_flag == 1) || (totem_flag == 1)) {
			MSG_82300009 msg;
			msg.gold_mine_flag = gold_mine_flag;
			msg.moon_well_flag = moon_well_flag;
			msg.totem_flag = totem_flag;
			THIS_SEND_TO_CLIENT(msg);
		}

		if(dragon_vale_detail_->is_vip_down_to_zero && !is_has_building_upgrading()) {
			const Vip_Level_Cfg *vip_normal_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Vip_Level_Cfg(0);
			if(!vip_normal_cfg) {
				MSG_USER("vip_level_cfg not exist, vip_level=0");
				return;
			}

			dragon_vale_detail_->all_open_build_queue_num = vip_normal_cfg->max_queue_num;
			dragon_vale_detail_->build_queue_num = vip_normal_cfg->max_queue_num;

			if(player_self()->role_id() == dragon_vale_detail_->target_player_id) {
				MSG_82300022 msg;
				msg.all_open_build_queue_num = dragon_vale_detail_->all_open_build_queue_num;
				msg.build_queue_num = dragon_vale_detail_->build_queue_num;
				THIS_SEND_TO_CLIENT(msg);
			}

			this->save_dragon_vale_tick();
			this->sync_dragon_vale_to_db_view();
		}

		const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
		if(cfg && cfg->type != Dragon_Vale_Type) {
			//主动通知龙谷信息(龙谷以外的场景接收)
			{
				MSG_82300033 msg;
				msg.build_queue_num = dragon_vale_detail_->build_queue_num;
				msg.all_open_build_queue_num = dragon_vale_detail_->all_open_build_queue_num;
				msg.progress = dragon_vale_detail_->dragon_info.progress;
				for(DragonBuildingInfoMap::const_iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
						it != dragon_vale_detail_->dragon_building_info_map.end(); ++it) {
					Building_Info building_info;
					building_info.id = it->second.id;
					building_info.type = it->second.type;
					building_info.level = it->second.level;
					building_info.status = it->second.status;
					building_info.production = it->second.production;
					building_info.is_gain = it->second.is_gain ? 1 : 0;
					building_info.order = it->second.order;
					building_info.consume_type = it->second.consume_type;

					role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), it->second.type);
					if(!role_level_cfg) {
						continue;
					}

					if(it->second.level >= role_level_cfg->max_building_level) {
						msg.building_info_list.push_back(building_info);
						continue;
					}

					uint16_t new_level = it->second.level + 1;
					const Building_Level_Cfg* building_level_cfg =
							CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, new_level);
					if(!building_level_cfg) {
						continue;
					}

					Time_Value now = Time_Value::gettimeofday();
					long total_time = 0;

					if(0 == it->second.level) {
						const Create_Building_Cfg *create_building_cfg =
										CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
						if(!create_building_cfg) {
							continue;
						}
						total_time = it->second.begin_upgrade_time.sec()
								+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
								+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
								+ (long)(create_building_cfg->create_need_second);
					}
					else {
						total_time = it->second.begin_upgrade_time.sec()
								+ building_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
								+ building_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
								+ building_level_cfg->upgrade_need_second;
					}

					building_info.left_time = (total_time > now.sec()) ? total_time - now.sec() : 0;
					msg.building_info_list.push_back(building_info);
				}//end for
				THIS_SEND_TO_CLIENT(msg);
			}
		}//end if

	}//end if
}

void Dragon_Vale::req_get_dragon_vale_friend_list() {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	MSG_50180007 msg;
	const Friend_Info_Map &freind_list_map = player_self()->friend_detail().firend_list_;

	DB_View::ID_Player_View_Map::const_iterator find_it  = LOGIC_MONITOR->db_view()->id_player_view_map.end();

	for(Friend_Info_Map::const_iterator it = freind_list_map.begin();
			it != freind_list_map.end(); ++it) {
		find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->second.role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end() || !find_it->second) {
			continue;
		}

		if(find_it->second->dragon_vale_view.dragon_building_info_map.empty()
				&& !find_it->second->dragon_vale_view.is_dragon_born) {
			continue;
		}

		if(player_self()->role_id() == it->second.role_id) {
			continue;
		}

		Dragon_Vale_Member member;
		member.role_id = it->second.role_id;
		member.head_photo_id = it->second.head_photo_id;
		member.vip_level = it->second.vip;
		member.name = it->second.role_name;
		member.is_special = find_it->second->dragon_vale_view.is_special;
		member.flag = 1;

		member.op = 0;
		if(!find_it->second->dragon_vale_view.thief_id_quality_map.empty()) {
			member.op  = 3;
		}

		if(find_it->second->dragon_vale_view.dragon_info.is_play_dragon) {
			member.op = 2;
		}

		if(dragon_vale_detail_->foot_id_set.find(find_it->second->role_id)
				== dragon_vale_detail_->foot_id_set.end()
				&& dragon_vale_detail_->award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
			member.op = 1;
		}

		msg.friend_list.push_back(member);
	}

	THIS_SEND_TO_CLIENT(msg);
}

void Dragon_Vale::req_get_dranon_vale_gang_member_list() {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	MSG_50180008 msg;
	const Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(player_self()->gang_id());
	if (!gang_view) {
		THIS_SEND_TO_CLIENT(msg);
		return;
	}

	DB_View::ID_Player_View_Map::const_iterator find_it  = LOGIC_MONITOR->db_view()->id_player_view_map.end();

	for (Gang_DB_View::Member_Map::const_iterator it = gang_view->member_map.begin();
			it != gang_view->member_map.end(); ++it) {
		find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->second.role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end() || !find_it->second) {
			continue;
		}

		if(find_it->second->dragon_vale_view.dragon_building_info_map.empty()
				&& !find_it->second->dragon_vale_view.is_dragon_born) {
			continue;
		}

		if(player_self()->role_id() == it->second.role_id) {
			continue;
		}

		Dragon_Vale_Member member;
		member.role_id = it->second.role_id;
		member.head_photo_id = find_it->second->friend_.head_photo_id;
		member.vip_level = find_it->second->vip_level;
		member.name = find_it->second->role_name;
		member.is_special = find_it->second->dragon_vale_view.is_special;
		member.headship = it->second.headship;
		member.flag = 2;

		member.op = 0;
		if(!find_it->second->dragon_vale_view.thief_id_quality_map.empty()) {
			member.op  = 3;
		}

		if(find_it->second->dragon_vale_view.dragon_info.is_play_dragon) {
			member.op = 2;
		}

		if(dragon_vale_detail_->foot_id_set.find(find_it->second->role_id)
				== dragon_vale_detail_->foot_id_set.end()
				&& dragon_vale_detail_->award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
			member.op = 1;
		}

		msg.gang_memeber_list.push_back(member);
	}

	THIS_SEND_TO_CLIENT(msg);
}

void Dragon_Vale::get_building_max_storage(uint32_t &gold_mine_max_storage, uint32_t &moon_weel_max_storage,
			uint32_t &totem_max_storage, uint32_t &gold_mine_production, uint32_t &moon_weel_production,
			uint32_t &totem_production, bool is_rob /*= false*/) {
	const Building_Level_Cfg *build_level_cfg = 0;

	for(DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
			it != dragon_vale_detail_->dragon_building_info_map.end();
			++it) {
		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, it->second.level);
		if(!build_level_cfg) {
			continue;
		}

		if(BUILDING_GOLD_MINE == it->second.type) {
			gold_mine_max_storage += build_level_cfg->max_storage;
			if(!is_rob || (is_rob && it->second.status != BUILDING_UPGRADE_DOING)) {
				gold_mine_production += it->second.production;
			}
			continue;
		}

		if(BUILDING_MOON_WELL == it->second.type) {
			moon_weel_max_storage += build_level_cfg->max_storage;
			if(!is_rob || (is_rob && it->second.status != BUILDING_UPGRADE_DOING)) {
				moon_weel_production += it->second.production;
			}
			continue;
		}

		if(BUILDING_TOTEM == it->second.type) {
			totem_max_storage += build_level_cfg->max_storage;
			if(!is_rob || (is_rob && it->second.status != BUILDING_UPGRADE_DOING)) {
				totem_production += it->second.production;
			}
			continue;
		}
	}
}

int Dragon_Vale::req_dragon_born() {
	Time_Value now = Time_Value::gettimeofday();
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = 0;

	//如果龙蛋已开始孵化
	if(dragon_vale_detail_->is_dragon_born) {
		total_time = dragon_vale_detail_->begin_born_time.sec()
								+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;
		if(total_time > now.sec()) {
			return ERROR_BEGIN_DRAONG_BORN;
		}
		else {
			return ERROR_DRAONG_BORN_FINISH;
		}
	}

	if(player_self()->level() < (int)dragon_base_cfg.dragon_born_level) {
		return ERROR_DRAGON_BORN_LEVEL_LIMIT;
	}

	dragon_vale_detail_->is_dragon_born = true;
	dragon_vale_detail_->begin_born_time.set(now);

	MSG_50180009 msg;
	msg.born_time = dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;
//	THIS_SEND_TO_CLIENT(msg);

	//boradcast to dragon vale scene
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_DRAGON_EGG_BORN);
	player_self()->task_listen_dragon_born();

	DRAGON_VALE_MANAGER->refresh_dragon_vale_op(player_self()->role_id());
	return 0;
}

void Dragon_Vale::listen_dragon_born_finish(const Time_Value &now) {
	//未开始孵化，不处理
	if(!dragon_vale_detail_->is_dragon_born) {
		return;
	}

	//龙蛋已孵化为龙，不处理
	if(dragon_vale_detail_->dragon_info.progress > 0) {
		return;
	}

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = dragon_vale_detail_->begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;

	//已开始孵化，但是，孵化未完成，不处理
	if(total_time > now.sec()) {
		return;
	}

	//孵化完成，进化程度变为1
	dragon_vale_detail_->dragon_info.progress = 1;

	//孵化完成，可以逗龙   注意：龙蛋孵化完成，马上就可以逗龙，不能重置逗龙冷却时间
	dragon_vale_detail_->dragon_info.is_play_dragon = true;

	//龙蛋孵化为龙，龙的心情值初始为最大值 - 1
	dragon_vale_detail_->dragon_info.feeling = dragon_base_cfg.max_feeling  - 1;

	//龙蛋孵化为龙，初始化龙的等级为0、 初始化属性
	dragon_vale_detail_->dragon_info.power_level = 0;
	dragon_vale_detail_->dragon_info.armor_level = 0;
	dragon_vale_detail_->dragon_info.stamina_level = 0;
	dragon_vale_detail_->dragon_info.hero_power_level = 0;
	dragon_vale_detail_->dragon_info.hero_armor_level = 0;
	dragon_vale_detail_->dragon_info.hero_stamina_level = 0;
	dragon_vale_detail_->dragon_info.crit_level = 0;
	dragon_vale_detail_->dragon_info.prevent_crit_level = 0;
	dragon_vale_detail_->dragon_info.hero_crit_level = 0;
	dragon_vale_detail_->dragon_info.hero_prevent_crit_level = 0;

	const Dragon_Evolution_Cfg* dragon_evolution_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(dragon_vale_detail_->dragon_info.progress);
	if(dragon_evolution_cfg) {
		double temp_property = 0.0;
		const Dragon_Keep_Cfg* dragon_keep_cfg = 0;
		//初始化主角斗力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_POWER,
										dragon_vale_detail_->dragon_info.power_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.power = (uint32_t)temp_property;
		}

		//初始化主角护甲属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_ARMOR,
										dragon_vale_detail_->dragon_info.armor_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.armor = (uint32_t)temp_property;
		}

		//初始化主角耐力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_STAMINA,
										dragon_vale_detail_->dragon_info.stamina_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.stamina = (uint32_t)temp_property;
		}

		//初始化英雄斗力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_POWER,
										dragon_vale_detail_->dragon_info.hero_power_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_power = (uint32_t)temp_property;
		}

		//初始化英雄护甲属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_ARMOR,
										dragon_vale_detail_->dragon_info.hero_armor_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_armor = (uint32_t)temp_property;
		}

		//初始化英雄耐力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_STAMINA,
										dragon_vale_detail_->dragon_info.hero_stamina_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_stamina = (uint32_t)temp_property;
		}

		//初始化主角暴击属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_CRIT,
				dragon_vale_detail_->dragon_info.crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.crit = (uint32_t)temp_property;
		}

		//初始化主角主角抗暴属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_PREVENT_CRIT,
				dragon_vale_detail_->dragon_info.prevent_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.prevent_crit = (uint32_t)temp_property;
		}

		//初始化英雄暴击属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_CRIT,
				dragon_vale_detail_->dragon_info.hero_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_crit = (uint32_t)temp_property;
		}

		//初始化英雄抗暴属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_PREVENT_CRIT,
				dragon_vale_detail_->dragon_info.hero_prevent_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_prevent_crit = (uint32_t)temp_property;
		}
	}

	//重置龙心情值下降的开始时间
	dragon_vale_detail_->begin_down_feeling_time.set(now);

	//在龙谷里面才会发消息
//	if(dragon_vale_detail_->target_player_id != 0) {
//		MSG_82300004 msg;
//		msg.progress = dragon_vale_detail_->dragon_info.progress;
//		msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
//		THIS_SEND_TO_CLIENT(msg);
//	}

	//boradcast to dragon vale scene
	MSG_82300004 msg;
	msg.progress = dragon_vale_detail_->dragon_info.progress;
	msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	Prop_Setter_Vec prop_vec;
	Prop_Setter setter;

	if(dragon_vale_detail_->dragon_info.power > 0) {
		setter.reset();
		setter.prop_type = PT_POWER;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = dragon_vale_detail_->dragon_info.power;
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.armor > 0) {
		setter.reset();
		setter.prop_type = PT_ARMOR;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = dragon_vale_detail_->dragon_info.armor;
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.stamina > 0) {
		setter.reset();
		setter.prop_type = PT_STAMINA;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = dragon_vale_detail_->dragon_info.stamina;
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.crit > 0) {
		setter.reset();
		setter.prop_type = PT_CRIT;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.crit);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.prevent_crit > 0) {
		setter.reset();
		setter.prop_type = PT_CRIT_DEF;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.prevent_crit);
		prop_vec.push_back(setter);
	}

	if (prop_vec.size() > 0) player_self()-> fight_prop_modify (prop_vec, 209);

	player_self()->heroer_prop_refresh();
}

int Dragon_Vale::req_play_dragon(role_id_t role_id) {
	if(player_self()->role_id() == role_id) {
		return play_self_dragon();
	}

	return play_other_dragon(role_id);
}

int Dragon_Vale::play_self_dragon() {
	Time_Value now = Time_Value::gettimeofday();
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = dragon_vale_detail_->begin_play_dragon_interval_time.sec()
							+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;

	//冷却时间未到，不可逗龙
	if(total_time > now.sec()) {
		return ERROR_PLAY_DRAONG_INTERVAL_TIME;
	}

	//龙的心情值满时，不可以逗龙
	if(dragon_base_cfg.max_feeling <= dragon_vale_detail_->dragon_info.feeling) {
		return ERROR_FULL_FEELING_NO_PLAY_DRAGON;
	}

	//当前不可逗龙    note: 冷却时间未到、龙心情值满不可逗龙，为后续增加其他情况考虑
	if(!dragon_vale_detail_->dragon_info.is_play_dragon) {
		return ERROR_NO_PLAY_DRAGON;
	}

	const Dragon_Play_Dragon_Cfg *dragon_play_dragon_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Dragon_Play_Dragon_Cfg(player_self()->level());
	if(!dragon_play_dragon_cfg) {
		return ERROR_PLAY_DRAGON_CFG_NOT_EXIST;
	}

	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_DRAGON_PLAY_DRAGON);//日常助手监听喂龙

	//每次逗龙，龙的心情值加1点,并且, 不超过龙的最大心情值
	dragon_vale_detail_->dragon_info.feeling += 1;
	if(dragon_vale_detail_->dragon_info.feeling > dragon_base_cfg.max_feeling) {
		dragon_vale_detail_->dragon_info.feeling = dragon_base_cfg.max_feeling;
	}

	//逗龙冷却
	dragon_vale_detail_->dragon_info.is_play_dragon = false;

	//重置逗龙冷却开始时间
	dragon_vale_detail_->begin_play_dragon_interval_time.set(now);

	//重置龙心情值下降的开始时间
	dragon_vale_detail_->begin_down_feeling_time.set(now);

	if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
		++dragon_vale_detail_->play_dragon_num;
	}

	//逗龙奖励
	if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
		Money_Sub_Type money_sub_type = Pack::MONEY_SUB_TYPE_END;
		Money_Gain_Type money_gain_type = MONEY_ADD_MAX;
		switch(dragon_base_cfg.dragon_vale_op_type) {
		case Pack::COPPER:
			money_sub_type = Pack::COPPER_ONLY;
			money_gain_type = Pack::MONEY_GAIN_COIN_BY_PLAY_DRAGON;
			break;

		case Pack::DRAGON_SOUL:
			money_sub_type = Pack::DRAGON_SOUL_ONLY;
			money_gain_type = Pack::MONEY_GAIN_DRAONG_SOUL_BY_PLAY_DRAGON;
			break;

		case Pack::SOULS:
			money_sub_type = Pack::SOULS_ONLY;
			money_gain_type = Pack::MONEY_ADD_SOUL_BY_PLAY_DRAGON;
			break;

		case Pack::FRIENDSHIP:
			money_sub_type = Pack::FRIENDSHIP_ONLY;
			money_gain_type = Pack::MONEY_ADD_FRIENDSHIP_BY_PLAY_DRAGON;
			break;
		}

		gain_money_in_dragon_vale(money_sub_type,
				dragon_play_dragon_cfg->play_dragon_award,
				money_gain_type);
	}

	MSG_50180010 msg;

	//逗龙概率获得龙心
	if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
		uint16_t heart_num = play_dragon_get_heart_num_by_chance();
		const Play_Dragon_Get_Heart_Cfg *play_dragon_get_heart_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Play_Dragon_Get_Heart_Cfg(heart_num);
		if(heart_num > 0 && play_dragon_get_heart_cfg) {
			if(CONFIG_CACHE_ITEM->find_item(play_dragon_get_heart_cfg->item_id)) {
				Item_Detail item(play_dragon_get_heart_cfg->item_id, heart_num, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
						Gain_Item_DM_Info(Pack::ITEM_GAIN_PLAY_SELF_DRAGON));
				//道具id 是合法的，才会通知客户端获得龙心
				msg.item_id = play_dragon_get_heart_cfg->item_id;
				msg.item_num = heart_num;

				MSG_81000102 acv_msg;
				Item_Basic_Info item_basic;
				item.set_item_basic_info(item_basic);
				acv_msg.item_info.push_back(item_basic);
			    if (!acv_msg.item_info.empty()) {
			    	THIS_SEND_TO_CLIENT(acv_msg);
			    }
			}
		}
	}

	msg.role_id = player_self()->role_id();
	msg.feeling = dragon_vale_detail_->dragon_info.feeling;

	if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
		msg.reward = dragon_play_dragon_cfg->play_dragon_award;
	}
	else {
		msg.reward = 0;
	}

	msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
	THIS_SEND_TO_CLIENT(msg);

	//boradcast to dragon vale scene
	{
		MSG_82300041 msg_82300041;
		msg_82300041.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
		Block_Buffer buf_82300041;
		buf_82300041.make_message(msg_82300041.msg_id);
		msg_82300041.serialize(buf_82300041);
		buf_82300041.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(player_self()->role_id());
		broad_cast_buf.copy(&buf_82300041);
		broad_cast_buf.finish_message();
	    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);
	}

	//refresh left time
	{
		MSG_82300042 msg_82300042;
		if(!dragon_vale_detail_->dragon_info.is_play_dragon) {
			const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
			long total_time = dragon_vale_detail_->begin_play_dragon_interval_time.sec()
									+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;
			Time_Value now = Time_Value::gettimeofday();
			if(total_time > now.sec()) {
				msg_82300042.left_time = total_time - now.sec();
			}
			else {
				MSG_USER("left time error");
				msg_82300042.left_time = 0;
			}
		}
		else {
			msg_82300042.left_time = 0;
		}
		Block_Buffer buf_82300042;
		buf_82300042.make_message(msg_82300042.msg_id);
		msg_82300042.serialize(buf_82300042);
		buf_82300042.finish_message();
		player_self()->send_to_client(buf_82300042);
	}

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
	player_self()->achievement_listen_dragon_wale_play_dragon_times();

	DRAGON_VALE_MANAGER->refresh_dragon_vale_op(player_self()->role_id());

	return 0;
}

int Dragon_Vale::play_other_dragon(role_id_t role_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(player) {
		Time_Value now = Time_Value::gettimeofday();
		const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
		long total_time = player->dragon_vale_detail_no_const().begin_play_dragon_interval_time.sec()
								+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;

		//冷却时间未到，不可逗龙
		if(total_time > now.sec()) {
			return ERROR_PLAY_DRAONG_INTERVAL_TIME;
		}

		//龙的心情值满时，不可以逗龙
		if(dragon_base_cfg.max_feeling <= player->dragon_vale_detail_no_const().dragon_info.feeling) {
			return ERROR_FULL_FEELING_NO_PLAY_DRAGON;
		}

		//当前不可逗龙    note: 冷却时间未到、龙心情值满不可逗龙，为后续增加其他情况考虑
		if(!player->dragon_vale_detail_no_const().dragon_info.is_play_dragon) {
			return ERROR_NO_PLAY_DRAGON;
		}

		const Dragon_Play_Dragon_Cfg *dragon_play_dragon_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Play_Dragon_Cfg(player_self()->level());
		if(!dragon_play_dragon_cfg) {
			return ERROR_PLAY_DRAGON_CFG_NOT_EXIST;
		}

		player->dragon_vale_detail_no_const().begin_play_dragon_interval_time.set(now);

		//每次逗龙，龙的心情值加1点,并且, 不超过龙的最大心情值
		player->dragon_vale_detail_no_const().dragon_info.feeling += 1;
		if(player->dragon_vale_detail_no_const().dragon_info.feeling > dragon_base_cfg.max_feeling) {
			player->dragon_vale_detail_no_const().dragon_info.feeling = dragon_base_cfg.max_feeling;
		}

		//逗龙冷却
		player->dragon_vale_detail_no_const().dragon_info.is_play_dragon = false;

		//重置逗龙冷却开始时间
		player->dragon_vale_detail_no_const().begin_play_dragon_interval_time.set(now);

		//重置龙心情值下降的开始时间
		player->dragon_vale_detail_no_const().begin_down_feeling_time.set(now);

		if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
			++dragon_vale_detail_->play_dragon_num;
		}

		//逗龙奖励
		if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
			Money_Sub_Type money_sub_type = Pack::MONEY_SUB_TYPE_END;
			Money_Gain_Type money_gain_type = MONEY_ADD_MAX;
			switch(dragon_base_cfg.dragon_vale_op_type) {
			case Pack::COPPER:
				money_sub_type = Pack::COPPER_ONLY;
				money_gain_type = Pack::MONEY_GAIN_COIN_BY_PLAY_DRAGON;
				break;

			case Pack::DRAGON_SOUL:
				money_sub_type = Pack::DRAGON_SOUL_ONLY;
				money_gain_type = Pack::MONEY_GAIN_DRAONG_SOUL_BY_PLAY_DRAGON;
				break;

			case Pack::SOULS:
				money_sub_type = Pack::SOULS_ONLY;
				money_gain_type = Pack::MONEY_ADD_SOUL_BY_PLAY_DRAGON;
				break;

			case Pack::FRIENDSHIP:
				money_sub_type = Pack::FRIENDSHIP_ONLY;
				money_gain_type = Pack::MONEY_ADD_FRIENDSHIP_BY_PLAY_DRAGON;
				break;
			}

			gain_money_in_dragon_vale(money_sub_type,
					dragon_play_dragon_cfg->play_dragon_award,
					money_gain_type);
		}

		MSG_50180010 msg;

		//逗龙概率获得龙心
		if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
			uint16_t heart_num = play_dragon_get_heart_num_by_chance();
			const Play_Dragon_Get_Heart_Cfg *play_dragon_get_heart_cfg =
					CONFIG_CACHE_DRAGON_VALE->find_Dragon_Play_Dragon_Get_Heart_Cfg(heart_num);
			if(heart_num > 0 && play_dragon_get_heart_cfg) {
				if(CONFIG_CACHE_ITEM->find_item(play_dragon_get_heart_cfg->item_id)) {
					Item_Detail item(play_dragon_get_heart_cfg->item_id, heart_num, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
					player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
							Gain_Item_DM_Info(Pack::ITEM_GAIN_PLAY_SELF_DRAGON));
					//道具id 是合法的，才会通知客户端获得龙心
					msg.item_id = play_dragon_get_heart_cfg->item_id;
					msg.item_num = heart_num;

					MSG_81000102 acv_msg;
					Item_Basic_Info item_basic;
					item.set_item_basic_info(item_basic);
					acv_msg.item_info.push_back(item_basic);
				    if (!acv_msg.item_info.empty()) {
				    	THIS_SEND_TO_CLIENT(acv_msg);
				    }
				}
			}
		}

		msg.role_id = role_id;
		msg.feeling = player->dragon_vale_detail_no_const().dragon_info.feeling;

		if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
			msg.reward = dragon_play_dragon_cfg->play_dragon_award;
		}
		else {
			msg.reward = 0;
		}

		msg.is_play_dragon = player->dragon_vale_detail_no_const().dragon_info.is_play_dragon ? 1 : 0;
		THIS_SEND_TO_CLIENT(msg);

		//boradcast to dragon vale scene
		{
			MSG_82300041 msg_82300041;
			msg_82300041.is_play_dragon = player->dragon_vale_detail_no_const().dragon_info.is_play_dragon ? 1 : 0;
			Block_Buffer buf_82300041;
			buf_82300041.make_message(msg_82300041.msg_id);
			msg_82300041.serialize(buf_82300041);
			buf_82300041.finish_message();

			Block_Buffer broad_cast_buf;
			broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
			broad_cast_buf.write_int64(role_id);
			broad_cast_buf.copy(&buf_82300041);
			broad_cast_buf.finish_message();
		    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
			LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);
		}

		//refresh left time
		{
			MSG_82300042 msg_82300042;
			if(!player->dragon_vale_detail().dragon_info.is_play_dragon) {
				const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
				long total_time = player->dragon_vale_detail().begin_play_dragon_interval_time.sec()
										+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;
				Time_Value now = Time_Value::gettimeofday();
				if(total_time > now.sec()) {
					msg_82300042.left_time = total_time - now.sec();
				}
				else {
					MSG_USER("left time error");
					msg_82300042.left_time = 0;
				}
			}
			else {
				msg_82300042.left_time = 0;
			}
			Block_Buffer buf_82300042;
			buf_82300042.make_message(msg_82300042.msg_id);
			msg_82300042.serialize(buf_82300042);
			buf_82300042.finish_message();
			player->send_to_client(buf_82300042);
		}

	    player->dragon_vale_detail_no_const().check_and_change_notice_board_num();
		if(player->dragon_vale_detail_no_const().notice_board_list.size() < 50) {
			Notice_Board notice_board;
			Time_Value now = Time_Value::gettimeofday();
			notice_board.type = NOTICE_BOARD_PLAY_DRAGON;
			notice_board.time = now.sec();
			notice_board.host = player_self()->role_name();
			notice_board.dragon_soul = dragon_play_dragon_cfg->play_dragon_award;
			notice_board.param1 = 1;
			player->dragon_vale_detail_no_const().notice_board_list.push_back(notice_board);
		}

		((Dragon_Vale*)player)->save_dragon_vale_tick();
		player->sync_dragon_vale_to_db_view();
		player_self()->achievement_listen_dragon_wale_play_dragon_times();

		DRAGON_VALE_MANAGER->refresh_dragon_vale_op(role_id);

		return 0;
	}//end if

	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return ERROR_PLAYER_NOT_EXIST;
	}

	if(!find_it->second) {
		return ERROR_PLAYER_NOT_EXIST;
	}

	Time_Value now = Time_Value::gettimeofday();
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = find_it->second->dragon_vale_view.begin_play_dragon_interval_time.sec()
							+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;

	//冷却时间未到，不可逗龙
	if(total_time > now.sec()) {
		return ERROR_PLAY_DRAONG_INTERVAL_TIME;
	}

	//龙的心情值满时，不可以逗龙
	if(dragon_base_cfg.max_feeling <= find_it->second->dragon_vale_view.dragon_info.feeling) {
		return ERROR_FULL_FEELING_NO_PLAY_DRAGON;
	}

	//当前不可逗龙    note: 冷却时间未到、龙心情值满不可逗龙，为后续增加其他情况考虑
	if(!find_it->second->dragon_vale_view.dragon_info.is_play_dragon) {
		return ERROR_NO_PLAY_DRAGON;
	}

	const Dragon_Play_Dragon_Cfg *dragon_play_dragon_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Dragon_Play_Dragon_Cfg(player_self()->level());
	if(!dragon_play_dragon_cfg) {
		return ERROR_PLAY_DRAGON_CFG_NOT_EXIST;
	}

	find_it->second->dragon_vale_view.begin_play_dragon_interval_time.set(now);

	//每次逗龙，龙的心情值加1点,并且, 不超过龙的最大心情值
	find_it->second->dragon_vale_view.dragon_info.feeling += 1;
	if(find_it->second->dragon_vale_view.dragon_info.feeling > dragon_base_cfg.max_feeling) {
		find_it->second->dragon_vale_view.dragon_info.feeling = dragon_base_cfg.max_feeling;
	}

	//逗龙冷却
	find_it->second->dragon_vale_view.dragon_info.is_play_dragon = false;

	//重置逗龙冷却开始时间
	find_it->second->dragon_vale_view.begin_play_dragon_interval_time.set(now);

	//重置龙心情值下降的开始时间
	find_it->second->dragon_vale_view.begin_down_feeling_time.set(now);

	if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
		++dragon_vale_detail_->play_dragon_num;
	}

	//逗龙奖励
	if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
		Money_Sub_Type money_sub_type = Pack::MONEY_SUB_TYPE_END;
		Money_Gain_Type money_gain_type = MONEY_ADD_MAX;
		switch(dragon_base_cfg.dragon_vale_op_type) {
		case Pack::COPPER:
			money_sub_type = Pack::COPPER_ONLY;
			money_gain_type = Pack::MONEY_GAIN_COIN_BY_PLAY_DRAGON;
			break;

		case Pack::DRAGON_SOUL:
			money_sub_type = Pack::DRAGON_SOUL_ONLY;
			money_gain_type = Pack::MONEY_GAIN_DRAONG_SOUL_BY_PLAY_DRAGON;
			break;

		case Pack::SOULS:
			money_sub_type = Pack::SOULS_ONLY;
			money_gain_type = Pack::MONEY_ADD_SOUL_BY_PLAY_DRAGON;
			break;

		case Pack::FRIENDSHIP:
			money_sub_type = Pack::FRIENDSHIP_ONLY;
			money_gain_type = Pack::MONEY_ADD_FRIENDSHIP_BY_PLAY_DRAGON;
			break;
		}

		gain_money_in_dragon_vale(money_sub_type,
				dragon_play_dragon_cfg->play_dragon_award,
				money_gain_type);
	}

	MSG_50180010 msg;

	//逗龙概率获得龙心
	if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
		uint16_t heart_num = play_dragon_get_heart_num_by_chance();
		const Play_Dragon_Get_Heart_Cfg *play_dragon_get_heart_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Play_Dragon_Get_Heart_Cfg(heart_num);
		if(heart_num > 0 && play_dragon_get_heart_cfg) {
			if(CONFIG_CACHE_ITEM->find_item(play_dragon_get_heart_cfg->item_id)) {
				Item_Detail item(play_dragon_get_heart_cfg->item_id, heart_num, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
						Gain_Item_DM_Info(Pack::ITEM_GAIN_PLAY_SELF_DRAGON));
				//道具id 是合法的，才会通知客户端获得龙心
				msg.item_id = play_dragon_get_heart_cfg->item_id;
				msg.item_num = heart_num;

				MSG_81000102 acv_msg;
				Item_Basic_Info item_basic;
				item.set_item_basic_info(item_basic);
				acv_msg.item_info.push_back(item_basic);
			    if (!acv_msg.item_info.empty()) {
			    	THIS_SEND_TO_CLIENT(acv_msg);
			    }
			}
		}
	}


	msg.role_id = role_id;
	msg.feeling = find_it->second->dragon_vale_view.dragon_info.feeling;

	if(dragon_vale_detail_->play_dragon_num <= dragon_base_cfg.max_play_dragon_award_num) {
		msg.reward = dragon_play_dragon_cfg->play_dragon_award;
	}
	else {
		msg.reward = 0;
	}

	msg.is_play_dragon = find_it->second->dragon_vale_view.dragon_info.is_play_dragon ? 1 : 0;
	THIS_SEND_TO_CLIENT(msg);

	//boradcast to dragon vale scene
	{
		MSG_82300041 msg_82300041;
		msg_82300041.is_play_dragon = find_it->second->dragon_vale_view.dragon_info.is_play_dragon ? 1 : 0;
		Block_Buffer buf_82300041;
		buf_82300041.make_message(msg_82300041.msg_id);
		msg_82300041.serialize(buf_82300041);
		buf_82300041.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(role_id);
		broad_cast_buf.copy(&buf_82300041);
		broad_cast_buf.finish_message();
	    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);
	}

	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	find_it->second->dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);

	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_DRAGON_PLAY_DRAGON);//日常助手监听喂龙

	DRAGON_VALE_MANAGER->refresh_dragon_vale_op(role_id);
	player_self()->achievement_listen_dragon_wale_play_dragon_times();
	return 0;
}

void Dragon_Vale::listen_play_dragon_interval_time(const Time_Value &now) {
	//未开始孵化或者孵化未完成，不处理
	if(!dragon_vale_detail_->is_dragon_born || dragon_vale_detail_->dragon_info.progress < 1) {
		return;
	}

	//当前可以逗龙，不需要处理冷却时间
	if(dragon_vale_detail_->dragon_info.is_play_dragon) {
		return;
	}

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();

	//龙的心情值满时，不可以逗龙,不处理逗龙冷却
	if(dragon_base_cfg.max_feeling <= dragon_vale_detail_->dragon_info.feeling) {
		return;
	}

	long total_time = dragon_vale_detail_->begin_play_dragon_interval_time.sec()
							+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;
	//冷却时间未到，不处理
	if(total_time > now.sec()) {
		return;
	}

	dragon_vale_detail_->dragon_info.is_play_dragon = true;

	//在龙谷里面才会发消息
//	if(dragon_vale_detail_->target_player_id != 0) {
//		MSG_82300007 msg;
//		msg.role_id = player_self()->role_id();
//		msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ?  1 : 0;
//		THIS_SEND_TO_CLIENT(msg);
//	}

	//boradcast to dragon vale scene
	MSG_82300007 msg;
	msg.role_id = player_self()->role_id();
	msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ?  1 : 0;

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}

void Dragon_Vale::listen_dragon_feeling_down_time(const Time_Value &now) {
	//未开始孵化或者孵化未完成，不处理
	if(!dragon_vale_detail_->is_dragon_born || dragon_vale_detail_->dragon_info.progress < 1) {
		return;
	}

	//龙的心情值已下降到0，不处理
	if(0 == dragon_vale_detail_->dragon_info.feeling) {
		return;
	}

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();

	long total_time = dragon_vale_detail_->begin_down_feeling_time.sec()
						+ dragon_base_cfg.down_feeling_time * Time_Value::ONE_MINUTE_IN_SECS;
	//时间未到,不处理
	if(total_time > now.sec()) {
		return;
	}

	//龙的心情值减1，最低为0
	dragon_vale_detail_->dragon_info.feeling =
			(dragon_vale_detail_->dragon_info.feeling > 0) ? dragon_vale_detail_->dragon_info.feeling - 1 : 0;

	//重置龙心情值下降的开始时间
	dragon_vale_detail_->begin_down_feeling_time.set(now);

	//在龙谷里面才会发消息
	if(dragon_vale_detail_->target_player_id != 0) {
		MSG_82300005 msg;
		msg.type = 1;
		msg.feeling = dragon_vale_detail_->dragon_info.feeling;
		THIS_SEND_TO_CLIENT(msg);
	}

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}

int Dragon_Vale::req_get_dragon_total_info(role_id_t role_id) {
	if(player_self()->role_id() == role_id) {
		return get_self_dragon_total_info();
	}

	return get_other_dragon_total_info(role_id);
}

int Dragon_Vale::get_self_dragon_total_info() {
	MSG_50180011 msg;
	msg.role_id = player_self()->role_id();
	msg.dragon_info.progress = dragon_vale_detail_->dragon_info.progress;
	msg.dragon_info.feeling = dragon_vale_detail_->dragon_info.feeling;
	msg.dragon_info.power = dragon_vale_detail_->dragon_info.power;
	msg.dragon_info.armor = dragon_vale_detail_->dragon_info.armor;
	msg.dragon_info.stamina = dragon_vale_detail_->dragon_info.stamina;
	msg.dragon_info.power_level = dragon_vale_detail_->dragon_info.power_level;
	msg.dragon_info.armor_level = dragon_vale_detail_->dragon_info.armor_level;
	msg.dragon_info.stamina_level = dragon_vale_detail_->dragon_info.stamina_level;
	msg.dragon_info.hero_power = dragon_vale_detail_->dragon_info.hero_power;
	msg.dragon_info.hero_armor = dragon_vale_detail_->dragon_info.hero_armor;
	msg.dragon_info.hero_stamina = dragon_vale_detail_->dragon_info.hero_stamina;
	msg.dragon_info.hero_power_level = dragon_vale_detail_->dragon_info.hero_power_level;
	msg.dragon_info.hero_armor_level = dragon_vale_detail_->dragon_info.hero_armor_level;
	msg.dragon_info.hero_stamina_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
	msg.dragon_info.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
	msg.dragon_info.energy = dragon_vale_detail_->dragon_info.energy;
	msg.dragon_info.crit = dragon_vale_detail_->dragon_info.crit;
	msg.dragon_info.prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;
	msg.dragon_info.crit_level = dragon_vale_detail_->dragon_info.crit_level;
	msg.dragon_info.prevent_crit_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
	msg.dragon_info.hero_crit = dragon_vale_detail_->dragon_info.hero_crit;
	msg.dragon_info.hero_prevent_crit = dragon_vale_detail_->dragon_info.hero_prevent_crit;
	msg.dragon_info.hero_crit_level = dragon_vale_detail_->dragon_info.hero_crit_level;
	msg.dragon_info.hero_prevent_crit_level = dragon_vale_detail_->dragon_info.hero_prevent_crit_level;

	if(!msg.dragon_info.is_play_dragon) {
		const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
		long total_time = dragon_vale_detail_->begin_play_dragon_interval_time.sec()
								+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;
		Time_Value now = Time_Value::gettimeofday();
		if(total_time > now.sec()) {
			msg.left_time = total_time - now.sec();
		}
		else {
			MSG_USER("left time error");
			msg.left_time = 0;
			msg.dragon_info.is_play_dragon = 1;
		}
	}
	else {
		msg.left_time = 0;
	}

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Dragon_Vale::get_other_dragon_total_info(role_id_t role_id) {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return 0;
	}

	if(!find_it->second) {
		return 0;
	}

	MSG_50180011 msg;
	msg.role_id = role_id;
	msg.dragon_info.progress = find_it->second->dragon_vale_view.dragon_info.progress;
	msg.dragon_info.feeling = find_it->second->dragon_vale_view.dragon_info.feeling;
	msg.dragon_info.power = find_it->second->dragon_vale_view.dragon_info.power;
	msg.dragon_info.armor = find_it->second->dragon_vale_view.dragon_info.armor;
	msg.dragon_info.stamina = find_it->second->dragon_vale_view.dragon_info.stamina;
	msg.dragon_info.power_level = find_it->second->dragon_vale_view.dragon_info.power_level;
	msg.dragon_info.armor_level = find_it->second->dragon_vale_view.dragon_info.armor_level;
	msg.dragon_info.stamina_level = find_it->second->dragon_vale_view.dragon_info.stamina_level;
	msg.dragon_info.hero_power = find_it->second->dragon_vale_view.dragon_info.hero_power;
	msg.dragon_info.hero_armor = find_it->second->dragon_vale_view.dragon_info.hero_armor;
	msg.dragon_info.hero_stamina = find_it->second->dragon_vale_view.dragon_info.hero_stamina;
	msg.dragon_info.hero_power_level = find_it->second->dragon_vale_view.dragon_info.hero_power_level;
	msg.dragon_info.hero_armor_level = find_it->second->dragon_vale_view.dragon_info.hero_armor_level;
	msg.dragon_info.hero_stamina_level = find_it->second->dragon_vale_view.dragon_info.hero_stamina_level;
	msg.dragon_info.is_play_dragon = find_it->second->dragon_vale_view.dragon_info.is_play_dragon ? 1 : 0;
	msg.dragon_info.energy = find_it->second->dragon_vale_view.dragon_info.energy;
	msg.dragon_info.crit = find_it->second->dragon_vale_view.dragon_info.crit;
	msg.dragon_info.prevent_crit = find_it->second->dragon_vale_view.dragon_info.prevent_crit;
	msg.dragon_info.crit_level = find_it->second->dragon_vale_view.dragon_info.crit_level;
	msg.dragon_info.prevent_crit_level = find_it->second->dragon_vale_view.dragon_info.prevent_crit_level;
	msg.dragon_info.hero_crit = find_it->second->dragon_vale_view.dragon_info.hero_crit;
	msg.dragon_info.hero_prevent_crit = find_it->second->dragon_vale_view.dragon_info.hero_prevent_crit;
	msg.dragon_info.hero_crit_level = find_it->second->dragon_vale_view.dragon_info.hero_crit_level;
	msg.dragon_info.hero_prevent_crit_level = find_it->second->dragon_vale_view.dragon_info.hero_prevent_crit_level;

	if(!msg.dragon_info.is_play_dragon) {
		const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
		long total_time = find_it->second->dragon_vale_view.begin_play_dragon_interval_time.sec()
								+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;
		Time_Value now = Time_Value::gettimeofday();
		if(total_time > now.sec()) {
			msg.left_time = total_time - now.sec();
		}
		else {
			MSG_USER("left time error");
			msg.left_time = 0;
			msg.dragon_info.is_play_dragon = 1;
		}
	}
	else {
		msg.left_time = 0;
	}

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Dragon_Vale::req_keep_dragon(uint8_t type) {
	const Dragon_Role_Level_Cfg *dragon_role_cfg =
			CONFIG_CACHE_DRAGON_VALE->find_Dragon_Role_Level_Cfg(player_self()->level(), type);
	if(!dragon_role_cfg) {
		return ERROR_DRAGON_ROLE_LEVEL_CFG_NOT_EXIST;
	}

	const Dragon_Feeling_Cfg *dragon_feeling_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Feeling_Cfg(dragon_vale_detail_->dragon_info.feeling);
	if(!dragon_feeling_cfg) {
		return ERROR_DRAGON_FEELING_CFG_NOT_EXIST;
	}

	const Dragon_Evolution_Cfg *dragon_evolution_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(dragon_vale_detail_->dragon_info.progress);
	if(!dragon_evolution_cfg) {
		return ERROR_DRAGON_EVOLUTION_NOT_EXIST;
	}

	uint32_t keep_level = 0;
	switch(type) {
		case KEEP_DRAGON_POWER:
			keep_level = dragon_vale_detail_->dragon_info.power_level;
			break;
		case KEEP_DRAGON_ARMOR:
			keep_level = dragon_vale_detail_->dragon_info.armor_level;
			break;
		case KEEP_DRAGON_STAMINA:
			keep_level = dragon_vale_detail_->dragon_info.stamina_level;
			break;
		case KEEP_DRAGON_HERO_POWER:
			keep_level = dragon_vale_detail_->dragon_info.hero_power_level;
			break;
		case KEEP_DRAGON_HERO_ARMOR:
			keep_level = dragon_vale_detail_->dragon_info.hero_armor_level;
			break;
		case KEEP_DRAGON_HERO_STAMINA:
			keep_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
			break;

		case KEEP_DRAGON_CRIT:
			keep_level = dragon_vale_detail_->dragon_info.crit_level;
			break;

		case KEEP_DRAGON_PREVENT_CRIT:
			keep_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
			break;

		case KEEP_DRAGON_HERO_CRIT:
			keep_level = dragon_vale_detail_->dragon_info.hero_crit_level;
			break;

		case KEEP_DRAGON_HERO_PREVENT_CRIT:
			keep_level =  dragon_vale_detail_->dragon_info.hero_prevent_crit_level;
			break;

		default:
			return ERROR_KEEP_DRAGON_TYPE_NOT_EXIST;
	}

	//已达到玩家等级对应的最大培养等级
	if(keep_level >= dragon_role_cfg->dragon_max_level) {
		return ERROR_KEEP_DRAONG_MAX_LEVEL;
	}

	//下一个等级
	uint32_t new_level = keep_level + 1;
	const Dragon_Keep_Cfg* dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg(type, new_level);
	if(!dragon_keep_cfg) {
			return ERROR_DRAGON_KEEP_CFG_NOT_EXIST;
	}

	//判断金币是否足够
	int ret = check_money_is_engough(Pack::COPPER_ONLY,
			dragon_keep_cfg->coin_num,
			MONEY_SUB_MONEY_COIN_BY_KEEP_DRAGON);
	if(ret != 0) {
		return ret;
	}

	//判断龙魂是否足够
	ret = check_money_is_engough(Pack::DRAGON_SOUL_ONLY,
			dragon_keep_cfg->dragon_soul_num,
			MONEY_SUB_MONEY_DRAGON_SOUL_BY_KEEP_DRAGON);
	if(ret != 0) {
		return ret;
	}

	//培养龙消耗金币
	ret = consume_money_in_dragon_vale(Pack::COPPER_ONLY,
				dragon_keep_cfg->coin_num,
				MONEY_SUB_MONEY_COIN_BY_KEEP_DRAGON);
	if(ret != 0) {
		return ret;
	}

	//培养龙消耗龙魂
	ret = consume_money_in_dragon_vale(Pack::DRAGON_SOUL_ONLY,
			dragon_keep_cfg->dragon_soul_num,
			MONEY_SUB_MONEY_DRAGON_SOUL_BY_KEEP_DRAGON);
	if(ret != 0) {
		return ret;
	}

	//培养是否成功的标志
	bool is_success = false;

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint32_t chance = random() % 10000;
	if(chance < dragon_keep_cfg->success_chance * dragon_feeling_cfg->chance) {
		is_success = true;
	}

	MSG_50180012 msg;
	//培养失败
	if(!is_success) {
		msg.type = type;
		msg.is_success = is_success ? 1 : 0;
		msg.dragon_info.progress = dragon_vale_detail_->dragon_info.progress;
		msg.dragon_info.feeling = dragon_vale_detail_->dragon_info.feeling;
		msg.dragon_info.power = dragon_vale_detail_->dragon_info.power;
		msg.dragon_info.armor = dragon_vale_detail_->dragon_info.armor;
		msg.dragon_info.stamina = dragon_vale_detail_->dragon_info.stamina;
		msg.dragon_info.power_level = dragon_vale_detail_->dragon_info.power_level;
		msg.dragon_info.armor_level = dragon_vale_detail_->dragon_info.armor_level;
		msg.dragon_info.stamina_level = dragon_vale_detail_->dragon_info.stamina_level;
		msg.dragon_info.hero_power = dragon_vale_detail_->dragon_info.hero_power;
		msg.dragon_info.hero_armor = dragon_vale_detail_->dragon_info.hero_armor;
		msg.dragon_info.hero_stamina = dragon_vale_detail_->dragon_info.hero_stamina;
		msg.dragon_info.hero_power_level = dragon_vale_detail_->dragon_info.hero_power_level;
		msg.dragon_info.hero_armor_level = dragon_vale_detail_->dragon_info.hero_armor_level;
		msg.dragon_info.hero_stamina_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
		msg.dragon_info.crit = dragon_vale_detail_->dragon_info.crit;
		msg.dragon_info.prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;
		msg.dragon_info.crit_level = dragon_vale_detail_->dragon_info.crit_level;
		msg.dragon_info.prevent_crit_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
		msg.dragon_info.hero_crit = dragon_vale_detail_->dragon_info.hero_crit;
		msg.dragon_info.hero_prevent_crit = dragon_vale_detail_->dragon_info.hero_prevent_crit;
		msg.dragon_info.hero_crit_level = dragon_vale_detail_->dragon_info.hero_crit_level;
		msg.dragon_info.hero_prevent_crit_level = dragon_vale_detail_->dragon_info.hero_prevent_crit_level;

		msg.dragon_info.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
		msg.dragon_info.energy = dragon_vale_detail_->dragon_info.energy;
		THIS_SEND_TO_CLIENT(msg);

		player_self()->task_listen_dragon_keep_all();
		return 0;
	}

	uint32_t last_power = dragon_vale_detail_->dragon_info.power;
	uint32_t last_armor = dragon_vale_detail_->dragon_info.armor;
	uint32_t last_stamina = dragon_vale_detail_->dragon_info.stamina;
	uint32_t last_crit = dragon_vale_detail_->dragon_info.crit;
	uint32_t last_prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;

	//培养成功,等级提升,属性提升
	double temp_property = 0.0;
	if(KEEP_DRAGON_POWER == type) {
		dragon_vale_detail_->dragon_info.power_level += 1;
		temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.power = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.power_level);
	}

	if(KEEP_DRAGON_ARMOR == type) {
		dragon_vale_detail_->dragon_info.armor_level += 1;
		temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.armor = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.armor_level);
	}

	if(KEEP_DRAGON_STAMINA == type) {
		dragon_vale_detail_->dragon_info.stamina_level += 1;
		temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.stamina = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.stamina_level);
	}

	if(KEEP_DRAGON_HERO_POWER == type) {
		dragon_vale_detail_->dragon_info.hero_power_level += 1;
		temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_power = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.hero_power_level);
	}

	if(KEEP_DRAGON_HERO_ARMOR == type) {
		dragon_vale_detail_->dragon_info.hero_armor_level += 1;
		temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_armor = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.hero_armor_level);
	}

	if(KEEP_DRAGON_HERO_STAMINA == type) {
		dragon_vale_detail_->dragon_info.hero_stamina_level += 1;
		temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_stamina = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.hero_stamina_level);
	}

	if(KEEP_DRAGON_CRIT == type) {
		dragon_vale_detail_->dragon_info.crit_level += 1;
		temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.crit = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.crit_level);
	}

	if(KEEP_DRAGON_PREVENT_CRIT == type) {
		dragon_vale_detail_->dragon_info.prevent_crit_level += 1;
		temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.prevent_crit = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.prevent_crit_level);
	}

	if(KEEP_DRAGON_HERO_CRIT == type) {
		dragon_vale_detail_->dragon_info.hero_crit_level += 1;
		temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_crit = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.hero_crit_level);
	}

	if(KEEP_DRAGON_HERO_PREVENT_CRIT == type) {
		dragon_vale_detail_->dragon_info.hero_prevent_crit_level += 1;
		temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_prevent_crit = (uint32_t)temp_property;
		player_self()->task_listen_keep_dragon_to_level(type, dragon_vale_detail_->dragon_info.hero_prevent_crit_level);
	}

	msg.type = type;
	msg.is_success = is_success ? 1 : 0;
	msg.dragon_info.progress = dragon_vale_detail_->dragon_info.progress;
	msg.dragon_info.feeling = dragon_vale_detail_->dragon_info.feeling;
	msg.dragon_info.power = dragon_vale_detail_->dragon_info.power;
	msg.dragon_info.armor = dragon_vale_detail_->dragon_info.armor;
	msg.dragon_info.stamina = dragon_vale_detail_->dragon_info.stamina;
	msg.dragon_info.power_level = dragon_vale_detail_->dragon_info.power_level;
	msg.dragon_info.armor_level = dragon_vale_detail_->dragon_info.armor_level;
	msg.dragon_info.stamina_level = dragon_vale_detail_->dragon_info.stamina_level;
	msg.dragon_info.hero_power = dragon_vale_detail_->dragon_info.hero_power;
	msg.dragon_info.hero_armor = dragon_vale_detail_->dragon_info.hero_armor;
	msg.dragon_info.hero_stamina = dragon_vale_detail_->dragon_info.hero_stamina;
	msg.dragon_info.hero_power_level = dragon_vale_detail_->dragon_info.hero_power_level;
	msg.dragon_info.hero_armor_level = dragon_vale_detail_->dragon_info.hero_armor_level;
	msg.dragon_info.hero_stamina_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
	msg.dragon_info.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
	msg.dragon_info.energy = dragon_vale_detail_->dragon_info.energy;
	msg.dragon_info.crit = dragon_vale_detail_->dragon_info.crit;
	msg.dragon_info.prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;
	msg.dragon_info.crit_level = dragon_vale_detail_->dragon_info.crit_level;
	msg.dragon_info.prevent_crit_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
	msg.dragon_info.hero_crit = dragon_vale_detail_->dragon_info.hero_crit;
	msg.dragon_info.hero_prevent_crit = dragon_vale_detail_->dragon_info.hero_prevent_crit;
	msg.dragon_info.hero_crit_level = dragon_vale_detail_->dragon_info.hero_crit_level;
	msg.dragon_info.hero_prevent_crit_level = dragon_vale_detail_->dragon_info.hero_prevent_crit_level;

	THIS_SEND_TO_CLIENT(msg);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	Prop_Setter_Vec prop_vec;
	Prop_Setter setter;
	if(dragon_vale_detail_->dragon_info.power > last_power){
		setter.reset();
		setter.prop_type = PT_POWER;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.power - last_power);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.armor > last_armor) {
		setter.reset();
		setter.prop_type = PT_ARMOR;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.armor - last_armor);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.stamina > last_stamina) {
		setter.reset();
		setter.prop_type = PT_STAMINA;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.stamina - last_stamina);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.crit > last_crit) {
		setter.reset();
		setter.prop_type = PT_CRIT;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.crit - last_crit);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.prevent_crit > last_prevent_crit) {
		setter.reset();
		setter.prop_type = PT_CRIT_DEF;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.prevent_crit - last_prevent_crit);
		prop_vec.push_back(setter);
	}

	if (prop_vec.size() > 0) player_self()->fight_prop_modify(prop_vec, 213);

	player_self()->heroer_prop_refresh();

	player_self()->task_listen_dragon_keep_all();
	player_self()->task_listen_dragon_keep_success();

	return 0;
}

int Dragon_Vale::check_money_is_engough(uint8_t money_type, uint32_t money_num, uint32_t lose_money_type) {
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

	return 0;
}

int Dragon_Vale::req_add_dragon_feeling() {
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	//龙心情值已是最大，不能增加
	if(dragon_base_cfg.max_feeling <= dragon_vale_detail_->dragon_info.feeling) {
		return ERROR_DRAGON_FEELING_MAX_NO_ADD;
	}

	//增加1点心情值消耗货币
	int ret = consume_money_in_dragon_vale(dragon_base_cfg.money_type,
				dragon_base_cfg.money_num,
				MONEY_SUB_MONEY_ADD_DRAGON_FEELING);
	if(ret != 0) {
		return ret;
	}

	//增加1点心情值,并且, 不超过龙的最大心情值
	dragon_vale_detail_->dragon_info.feeling += 1;
	if(dragon_vale_detail_->dragon_info.feeling >= dragon_base_cfg.max_feeling) {
		dragon_vale_detail_->dragon_info.feeling = dragon_base_cfg.max_feeling;
		dragon_vale_detail_->dragon_info.is_play_dragon = false;
	}

	MSG_50180013 msg;
	msg.feeling = dragon_vale_detail_->dragon_info.feeling;
	THIS_SEND_TO_CLIENT(msg);

	//boradcast to dragon vale scene
	{
		MSG_82300041 msg_82300041;
		msg_82300041.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
		Block_Buffer buf_82300041;
		buf_82300041.make_message(msg_82300041.msg_id);
		msg_82300041.serialize(buf_82300041);
		buf_82300041.finish_message();

		Block_Buffer broad_cast_buf;
		broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
		broad_cast_buf.write_int64(player_self()->role_id());
		broad_cast_buf.copy(&buf_82300041);
		broad_cast_buf.finish_message();
	    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);
	}

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	return 0;
}

int Dragon_Vale::req_evolute_dragon(uint8_t type) {
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	if(player_self()->level() < (int)dragon_base_cfg.evolution_open_level) {
		return ERROR_DRAGON_EVOLUTION_LEVEL_LIMIT;
	}

	if(dragon_vale_detail_->dragon_info.progress >= dragon_base_cfg.max_evolution) {
		return ERROR_DRAGON_EVOLUTION_MAX;
	}

	uint8_t new_progresss = dragon_vale_detail_->dragon_info.progress + 1;
	const Dragon_Evolution_Cfg *dragon_evolution_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(new_progresss);
	if(!dragon_evolution_cfg) {
		return ERROR_DRAGON_EVOLUTION_NOT_EXIST;
	}

	if(1 != type && 2 != type) {
		return ERROR_DRAGON_EVOLUTION_TYPE_NOT_EXIST;
	}

	if(1 == type) {
    	//判断物品是否足够
    	int pack_item_amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,
    			dragon_evolution_cfg->item_id);
    	if(pack_item_amount < (int)dragon_evolution_cfg->item_num) {
    		//道具不足，通知客户端进化龙所需道具不足，是否愿意消耗货币
    		MSG_82300006 msg;
    		msg.monye_type = dragon_evolution_cfg->money_type;
    		msg.money_num = dragon_evolution_cfg->money_num;
    		THIS_SEND_TO_CLIENT(msg);
    		return 0;
    	}

    	//扣物品
		int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
				 Id_Amount(dragon_evolution_cfg->item_id, dragon_evolution_cfg->item_num),
				 Lose_Item_DM_Info(ITEM_LOSE_DRAGON_EVOLUTION));
		if (0 != result) {
			MSG_USER("error :sub item failed, item_id:%d, item_num:%d\n",
					dragon_evolution_cfg->item_id, dragon_evolution_cfg->item_num);
			return ERROR_SUB_ITEM_FAIL;
		}

		return process_evolution_dragon(*dragon_evolution_cfg);
	}

	if(2 == type) {
		int ret = consume_money_in_dragon_vale(dragon_evolution_cfg->money_type,
				dragon_evolution_cfg->money_num,
				MONEY_SUB_MONEY_DRAGON_EVOLUTION);
		if(ret != 0) {
			return ret;
		}

		return process_evolution_dragon(*dragon_evolution_cfg);
	}

	return 0;
}

int Dragon_Vale::process_evolution_dragon(const Dragon_Evolution_Cfg &dragon_evolution_cfg) {
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	if(dragon_vale_detail_->dragon_info.progress >= dragon_base_cfg.max_evolution) {
		return ERROR_DRAGON_EVOLUTION_MAX;
	}


	//是否获得10倍能量
	bool is_ten_times = false;

	uint32_t energy = 0;

	//概率获得10倍能量
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint8_t chance = random() % 100;
	if(chance < dragon_base_cfg.chance) {
		energy = dragon_base_cfg.energy * 10;
		is_ten_times = true;
	}
	else {
		energy = dragon_base_cfg.energy;
	}

	//进化获得能量
	dragon_vale_detail_->dragon_info.energy += energy;

	MSG_50180014 msg;

	//没有进化出新的龙
	if(dragon_vale_detail_->dragon_info.energy < dragon_evolution_cfg.total_energy) {
		msg.is_ten_times = is_ten_times ? 1 : 0;
		msg.energy = dragon_vale_detail_->dragon_info.energy;
		msg.is_new_dragon = 0;
		msg.dragon_info.progress = dragon_vale_detail_->dragon_info.progress;
		msg.dragon_info.feeling = dragon_vale_detail_->dragon_info.feeling;
		msg.dragon_info.power = dragon_vale_detail_->dragon_info.power;
		msg.dragon_info.armor = dragon_vale_detail_->dragon_info.armor;
		msg.dragon_info.stamina = dragon_vale_detail_->dragon_info.stamina;
		msg.dragon_info.power_level = dragon_vale_detail_->dragon_info.power_level;
		msg.dragon_info.armor_level = dragon_vale_detail_->dragon_info.armor_level;
		msg.dragon_info.stamina_level = dragon_vale_detail_->dragon_info.stamina_level;
		msg.dragon_info.hero_power = dragon_vale_detail_->dragon_info.hero_power;
		msg.dragon_info.hero_armor = dragon_vale_detail_->dragon_info.hero_armor;
		msg.dragon_info.hero_stamina = dragon_vale_detail_->dragon_info.hero_stamina;
		msg.dragon_info.hero_power_level = dragon_vale_detail_->dragon_info.hero_power_level;
		msg.dragon_info.hero_armor_level = dragon_vale_detail_->dragon_info.hero_armor_level;
		msg.dragon_info.hero_stamina_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
		msg.dragon_info.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
		msg.dragon_info.energy = dragon_vale_detail_->dragon_info.energy;
		msg.dragon_info.crit = dragon_vale_detail_->dragon_info.crit;
		msg.dragon_info.prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;
		msg.dragon_info.crit_level = dragon_vale_detail_->dragon_info.crit_level;
		msg.dragon_info.prevent_crit_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
		msg.dragon_info.hero_crit = dragon_vale_detail_->dragon_info.hero_crit;
		msg.dragon_info.hero_prevent_crit = dragon_vale_detail_->dragon_info.hero_prevent_crit;
		msg.dragon_info.hero_crit_level = dragon_vale_detail_->dragon_info.hero_crit_level;
		msg.dragon_info.hero_prevent_crit_level = dragon_vale_detail_->dragon_info.hero_prevent_crit_level;

		THIS_SEND_TO_CLIENT(msg);

		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();
		return 0;
	}

	//进化出新的龙,进度加1，能量值重置为0
	dragon_vale_detail_->dragon_info.progress += 1;
	dragon_vale_detail_->dragon_info.energy = 0;

	uint32_t last_power = dragon_vale_detail_->dragon_info.power;
	uint32_t last_armor = dragon_vale_detail_->dragon_info.armor;
	uint32_t last_stamina = dragon_vale_detail_->dragon_info.stamina;
	uint32_t last_crit = dragon_vale_detail_->dragon_info.crit;
	uint32_t last_prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;

	//进化出新的龙,属性提升
	double temp_property = 0.0;
	const Dragon_Keep_Cfg *dragon_keep_cfg = 0;
	//主角斗力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_POWER,
									dragon_vale_detail_->dragon_info.power_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_power * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.power = (uint32_t)temp_property;
	}

	//主角护甲属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_ARMOR,
									dragon_vale_detail_->dragon_info.armor_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_armor * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.armor = (uint32_t)temp_property;
	}

	//主角耐力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_STAMINA,
									dragon_vale_detail_->dragon_info.stamina_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.stamina = (uint32_t)temp_property;
	}

	////////////////////////////////////////
	//英雄斗力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_POWER,
									dragon_vale_detail_->dragon_info.hero_power_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_power * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_power = (uint32_t)temp_property;
	}

	//英雄护甲属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_ARMOR,
									dragon_vale_detail_->dragon_info.hero_armor_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_armor * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_armor = (uint32_t)temp_property;
	}

	//英雄耐力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_STAMINA,
									dragon_vale_detail_->dragon_info.hero_stamina_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_stamina = (uint32_t)temp_property;
	}

	//主角暴击
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_CRIT,
									dragon_vale_detail_->dragon_info.crit_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.crit = (uint32_t)temp_property;
	}

	//主角抗暴
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_PREVENT_CRIT,
									dragon_vale_detail_->dragon_info.prevent_crit_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.prevent_crit = (uint32_t)temp_property;
	}

	//英雄暴击
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_CRIT,
									dragon_vale_detail_->dragon_info.hero_crit_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_crit = (uint32_t)temp_property;
	}

	//英雄抗暴
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_PREVENT_CRIT,
									dragon_vale_detail_->dragon_info.hero_crit_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_prevent_crit = (uint32_t)temp_property;
	}

	msg.is_ten_times = is_ten_times ? 1 : 0;
	msg.energy = dragon_vale_detail_->dragon_info.energy;
	msg.is_new_dragon = 1;
	msg.dragon_info.progress = dragon_vale_detail_->dragon_info.progress;
	msg.dragon_info.feeling = dragon_vale_detail_->dragon_info.feeling;
	msg.dragon_info.power = dragon_vale_detail_->dragon_info.power;
	msg.dragon_info.armor = dragon_vale_detail_->dragon_info.armor;
	msg.dragon_info.stamina = dragon_vale_detail_->dragon_info.stamina;
	msg.dragon_info.power_level = dragon_vale_detail_->dragon_info.power_level;
	msg.dragon_info.armor_level = dragon_vale_detail_->dragon_info.armor_level;
	msg.dragon_info.stamina_level = dragon_vale_detail_->dragon_info.stamina_level;
	msg.dragon_info.hero_power = dragon_vale_detail_->dragon_info.hero_power;
	msg.dragon_info.hero_armor = dragon_vale_detail_->dragon_info.hero_armor;
	msg.dragon_info.hero_stamina = dragon_vale_detail_->dragon_info.hero_stamina;
	msg.dragon_info.hero_power_level = dragon_vale_detail_->dragon_info.hero_power_level;
	msg.dragon_info.hero_armor_level = dragon_vale_detail_->dragon_info.hero_armor_level;
	msg.dragon_info.hero_stamina_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
	msg.dragon_info.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
	msg.dragon_info.energy = dragon_vale_detail_->dragon_info.energy;
	msg.dragon_info.crit = dragon_vale_detail_->dragon_info.crit;
	msg.dragon_info.prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;
	msg.dragon_info.crit_level = dragon_vale_detail_->dragon_info.crit_level;
	msg.dragon_info.prevent_crit_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
	msg.dragon_info.hero_crit = dragon_vale_detail_->dragon_info.hero_crit;
	msg.dragon_info.hero_prevent_crit = dragon_vale_detail_->dragon_info.hero_prevent_crit;
	msg.dragon_info.hero_crit_level = dragon_vale_detail_->dragon_info.hero_crit_level;
	msg.dragon_info.hero_prevent_crit_level = dragon_vale_detail_->dragon_info.hero_prevent_crit_level;

	//boradcast to dragon vale scene
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	Prop_Setter_Vec prop_vec;
	Prop_Setter setter;
	if(dragon_vale_detail_->dragon_info.power > last_power) {
		setter.reset();
		setter.prop_type = PT_POWER;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.power - last_power);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.armor > last_armor) {
		setter.reset();
		setter.prop_type = PT_ARMOR;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.armor - last_armor);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.stamina > last_stamina) {
		setter.reset();
		setter.prop_type = PT_STAMINA;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.stamina - last_stamina);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.crit > last_crit) {
		setter.reset();
		setter.prop_type = PT_CRIT;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.crit - last_crit);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.prevent_crit > last_prevent_crit) {
		setter.reset();
		setter.prop_type = PT_CRIT_DEF;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.prevent_crit - last_prevent_crit);
		prop_vec.push_back(setter);
	}

	if (prop_vec.size() > 0) player_self()->fight_prop_modify(prop_vec, 212);

	player_self()->heroer_prop_refresh();

	//系统公告
	player_self()->create_ann_dragon_evolution_success(player_self()->role_id(), dragon_evolution_cfg.dragon_name);
	player_self()->achievement_listen_dragon_wale_keep_level(dragon_vale_detail_->dragon_info.progress);
	return 0;
}

uint32_t Dragon_Vale::get_add_power_from_dragon_vale() {
	if(!dragon_vale_detail_->is_dragon_born) {
		return 0;
	}
	return dragon_vale_detail_->dragon_info.power;
}

uint32_t Dragon_Vale::get_add_armor_from_dragon_vale() {
	if(!dragon_vale_detail_->is_dragon_born) {
		return 0;
	}
	return dragon_vale_detail_->dragon_info.armor;
}

uint32_t Dragon_Vale::get_add_stamina_from_dragon_vale() {
	if(!dragon_vale_detail_->is_dragon_born) {
		return 0;
	}
	return dragon_vale_detail_->dragon_info.stamina;
}

uint32_t Dragon_Vale::get_add_hero_power_from_dragon_vale() {
	if(!dragon_vale_detail_->is_dragon_born) {
		return 0;
	}
	return dragon_vale_detail_->dragon_info.hero_power;
}

uint32_t Dragon_Vale::get_add_hero_armor_from_dragon_vale() {
	if(!dragon_vale_detail_->is_dragon_born) {
		return 0;
	}
	return dragon_vale_detail_->dragon_info.hero_armor;
}

uint32_t Dragon_Vale::get_add_hero_stamina_from_dragon_vale() {
	if(!dragon_vale_detail_->is_dragon_born) {
		return 0;
	}
	return dragon_vale_detail_->dragon_info.hero_stamina;
}

uint32_t Dragon_Vale::get_add_hero_crit_from_dragon_vale() {
	if(!dragon_vale_detail_->is_dragon_born) {
		return 0;
	}
	return dragon_vale_detail_->dragon_info.hero_crit;
}

uint32_t Dragon_Vale::get_add_hero_prevent_from_dragon_vale() {
	if(!dragon_vale_detail_->is_dragon_born) {
		return 0;
	}
	return dragon_vale_detail_->dragon_info.hero_prevent_crit;
}

void Dragon_Vale::get_add_fight_property(std::map<int, double> &add_property_map) {
	add_property_map.insert(std::make_pair(PT_POWER, get_add_power_from_dragon_vale()));
	add_property_map.insert(std::make_pair(PT_ARMOR, get_add_armor_from_dragon_vale()));
	add_property_map.insert(std::make_pair(PT_STAMINA, get_add_stamina_from_dragon_vale()));

	if(dragon_vale_detail_->is_dragon_born && dragon_vale_detail_->dragon_info.crit > 0) {
		add_property_map.insert(std::make_pair(PT_CRIT, dragon_vale_detail_->dragon_info.crit));
	}

	if(dragon_vale_detail_->is_dragon_born && dragon_vale_detail_->dragon_info.prevent_crit > 0) {
		add_property_map.insert(std::make_pair(PT_CRIT_DEF, dragon_vale_detail_->dragon_info.prevent_crit));
	}
}

bool Dragon_Vale::is_has_building_upgrading() {
	for(DragonBuildingInfoMap::const_iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
			it != dragon_vale_detail_->dragon_building_info_map.end();
			++it) {
		if(it->second.status == BUILDING_UPGRADE_DOING) {
			return true;
		}
	}

	return false;
}

void Dragon_Vale::dragon_vale_listen_vip_level_chance(uint32_t new_vip_level) {
	const Vip_Level_Cfg *vip_normal_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Vip_Level_Cfg(0);
	if(!vip_normal_cfg) {
		MSG_USER("vip_level_cfg not exist, vip_level=0");
		return;
	}

	const Vip_Level_Cfg *vip_new_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Vip_Level_Cfg(new_vip_level);
	if(!vip_new_level_cfg) {
		MSG_USER("vip_level_cfg not exist, vip_level=%d", new_vip_level);
		return;
	}

	//vip升级
	if(new_vip_level > 0) {
		MSG_DEBUG("role_name=%s, new_vip_level=%d, max_queue_num=%d",
				player_self()->role_name().c_str(), new_vip_level, vip_new_level_cfg->max_queue_num);
		if(vip_new_level_cfg->max_queue_num > dragon_vale_detail_->all_open_build_queue_num) {
			dragon_vale_detail_->build_queue_num +=
					(vip_new_level_cfg->max_queue_num - dragon_vale_detail_->all_open_build_queue_num);
			dragon_vale_detail_->all_open_build_queue_num = vip_new_level_cfg->max_queue_num;
		}
	}

	//vip过期
	if(0 == new_vip_level) {
		if(is_has_building_upgrading()) {
			dragon_vale_detail_->is_vip_down_to_zero = true;
			this->save_dragon_vale_tick();
			this->sync_dragon_vale_to_db_view();
			return;
		}
		dragon_vale_detail_->all_open_build_queue_num = vip_normal_cfg->max_queue_num;
		dragon_vale_detail_->build_queue_num = vip_normal_cfg->max_queue_num;
	}

	if(player_self()->role_id() == dragon_vale_detail_->target_player_id) {
		MSG_82300022 msg;
		msg.all_open_build_queue_num = dragon_vale_detail_->all_open_build_queue_num;
		msg.build_queue_num = dragon_vale_detail_->build_queue_num;
		THIS_SEND_TO_CLIENT(msg);
	}

	this->save_dragon_vale_tick();
	this->sync_dragon_vale_to_db_view();
}

int Dragon_Vale::req_rob_other_dragon_vale() {
	const Main_Base_Cfg& main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	if(player_self()->level() < (int)main_base_cfg.rob_level) {
		return ERROR_ROB_DRAGON_LEVEL_LIMIT;
	}

	uint32_t match_num = 0;
	if(dragon_vale_detail_->matched_num >= main_base_cfg.border_match_num_of_max_match_coin) {
		match_num = main_base_cfg.border_match_num_of_max_match_coin;
	}
	else {
		match_num = dragon_vale_detail_->matched_num;
	}

	const uint32_t *coin = CONFIG_CACHE_DRAGON_VALE->find_main_match_coin(match_num);
	if(!coin) {
		return ERROR_DRAGON_VALE_MATCH_COIN_CFG_NOT_EXIST;
	}

	MSG_50180015 msg;
	msg.coin = *coin;
	msg.is_rob_protect = dragon_vale_detail_->is_rob_protect ? 1 : 0;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Dragon_Vale::req_rob_other_dragon_vale_match() {
	const Main_Base_Cfg& main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	if(player_self()->level() < (int)main_base_cfg.rob_level) {
		return ERROR_ROB_DRAGON_LEVEL_LIMIT;
	}

	uint32_t match_num = 0;
	if(dragon_vale_detail_->matched_num >= main_base_cfg.border_match_num_of_max_match_coin) {
		match_num = main_base_cfg.border_match_num_of_max_match_coin;
	}
	else {
		match_num = dragon_vale_detail_->matched_num;
	}

	const uint32_t *coin = CONFIG_CACHE_DRAGON_VALE->find_main_match_coin(match_num);
	if(!coin) {
		return ERROR_DRAGON_VALE_MATCH_COIN_CFG_NOT_EXIST;
	}

	//消耗金币
	uint32_t cost_coin = *coin;
	int ret = consume_money_in_dragon_vale(Pack::COPPER_ONLY,
										   cost_coin,
										   MONEY_SUB_MONEY_ROB_MATCH);
	if(ret != 0) {
		return ret;
	}

	MSG_20600000 msg;
	msg.uuid = CONFIG_CACHE->server_flag();
	msg.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	msg.role_id = player_self()->role_id();

	if(dragon_vale_detail_->matched_num >= main_base_cfg.border_match_num_of_max_match_coin) {
		match_num = main_base_cfg.border_match_num_of_max_match_coin;
	}
	else {
		match_num = dragon_vale_detail_->matched_num;
	}

	coin = CONFIG_CACHE_DRAGON_VALE->find_main_match_coin(match_num);
	if(!coin) {
		return ERROR_DRAGON_VALE_MATCH_COIN_CFG_NOT_EXIST;
	}

	msg.coin = *coin;
	msg.last_coin = cost_coin;

	Block_Buffer buf;
	buf.make_message(INNER_LOGIC_TO_SCENE_ROB_MATCH);
    msg.serialize(buf);
    buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	dragon_vale_detail_->is_rob_protect = false;

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
	return 0;
}

int Dragon_Vale::req_gain_rob_dragon_vale_production(int64_t to_uuid, int64_t to_role_id,
									    int64_t uuid, int64_t role_id,
									    uint32_t building_id, uint8_t building_type) {
	if(dragon_vale_detail().uuid != uuid
	    || dragon_vale_detail().is_rob_enter_dragon_vale == false
	    || dragon_vale_detail().target_player_id != role_id) {
		return ERROR_NO_ROB_NO_GAIN_PRODUCTION;
	}

	MSG_20600009 msg;
	msg.to_uuid = to_uuid;
	msg.to_role_id = to_role_id;
	msg.to_server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	msg.to_role_name = player_self()->role_name();
	msg.uuid = uuid;
	msg.role_id = role_id;
	msg.building_id = building_id;
	msg.building_type = building_type;

	Block_Buffer buf;
	buf.make_message(INNER_GAIN_ROB_PRODUCTION);
	msg.serialize(buf);
    buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
    LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, buf);

	return 0;
}

void Dragon_Vale::process_login_building_output_production_by_one_minute() {
	if(player_self()->base_detail().last_sign_out_time.sec() == 0
			&& player_self()->base_detail().last_sign_out_time.usec() == 0)  {
		return;
	}

	MSG_82300001 msg;
	const Building_Level_Cfg *build_level_cfg = 0;
	int32_t minute = 0;
	Time_Value now = Time_Value::gettimeofday();
	uint64_t temp_production = 0;

	uint32_t production_per_min = 0;
	uint32_t add_speed = 0;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(dragon_vale_detail_->star_num);
	if(!star_add_speed) {
		add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", dragon_vale_detail_->star_num);
	}
	else {
		add_speed = (uint16_t)(*star_add_speed);
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	uint32_t sub_speed =
				(uint32_t)(dragon_vale_detail_->thief_id_quality_map.size() * main_base_cfg.thief_down_production);

	for(DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
			it != dragon_vale_detail_->dragon_building_info_map.end();
			++it) {
		//升级状态，不产出
		if(BUILDING_UPGRADE_DOING == it->second.status) {
			continue;
		}

		//等级为0，说明还未建筑完成，不能产出
		if(0 == it->second.level) {
			continue;
		}

		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, it->second.level);
		if(!build_level_cfg) {
			continue;
		}

		if(it->second.production >= build_level_cfg->max_storage) {
			continue;
		}

		minute = (int32_t)((now.sec() - it->second.last_output_production_time.sec()) / Time_Value::ONE_MINUTE_IN_SECS);
		if(minute <= 0) {
			continue;
		}

		it->second.last_output_production_time.set(now);

		production_per_min  = (uint32_t)((100 + add_speed - sub_speed) * build_level_cfg->production_per_min / 100.0);

		temp_production = it->second.production + production_per_min * minute;
		if(temp_production <= 0) {
			continue;
		}
		if(temp_production > build_level_cfg->max_storage) {
			temp_production = build_level_cfg->max_storage;
		}

		it->second.production = (uint32_t)temp_production;

		it->second.is_gain = (it->second.production >= build_level_cfg->min_gain_limit);

		Building_Info msg_building_info;
		msg_building_info.id = it->second.id;
		msg_building_info.type = it->second.type;
		msg_building_info.level = it->second.level;
		msg_building_info.status = it->second.status;
		msg_building_info.production = it->second.production;
		msg_building_info.production_per_minute = production_per_min;
		msg_building_info.is_gain = it->second.is_gain ? 1 : 0;
		msg_building_info.order = it->second.order;
		msg_building_info.consume_type = it->second.consume_type;

		msg.building_info_list.push_back(msg_building_info);
	}

	if(!msg.building_info_list.empty()) {
		//在龙谷里面才会发消息
//		if(dragon_vale_detail_->target_player_id != 0) {
//			THIS_SEND_TO_CLIENT(msg);
//		}



		uint32_t gold_mine_max_storage = 0;
		uint32_t moon_weel_max_storage = 0;
	    uint32_t totem_max_storage = 0;
	    uint32_t new_gold_mine_production = 0;
	    uint32_t new_moon_weel_production = 0;
	    uint32_t new_totem_production = 0;

	    //在龙谷里面、外面都会发的消息，客户端弹窗用
	    MSG_82300008 max_msg;
		get_building_max_storage(gold_mine_max_storage, moon_weel_max_storage,
				totem_max_storage, new_gold_mine_production,
				new_moon_weel_production, new_totem_production, false);
		if(!dragon_vale_detail_->is_gold_mine_max_storage && gold_mine_max_storage != 0
				&& new_gold_mine_production != 0 && new_gold_mine_production >= gold_mine_max_storage) {
			max_msg.is_gold_mine = 1;
			dragon_vale_detail_->is_gold_mine_max_storage = true;
		}
		if(!dragon_vale_detail_->is_moon_well_max_storage && moon_weel_max_storage != 0
				&& new_moon_weel_production != 0 && new_moon_weel_production >= moon_weel_max_storage) {
			max_msg.is_moon_weel = 1;
			dragon_vale_detail_->is_moon_well_max_storage = true;
		}
		if(!dragon_vale_detail_->is_totem_max_storage && totem_max_storage != 0
				&& new_totem_production != 0 && new_totem_production >= totem_max_storage) {
			max_msg.is_totem = 1;
			dragon_vale_detail_->is_totem_max_storage = true;
		}

		if(max_msg.is_gold_mine || max_msg.is_moon_weel || max_msg.is_totem) {
//			THIS_SEND_TO_CLIENT(max_msg);
			Building_Notice notice;
			notice.gold_mine = dragon_vale_detail_->is_gold_mine_max_storage ? 1 : 0;
			notice.moon_well = dragon_vale_detail_->is_moon_well_max_storage ? 1 : 0;
			notice.totem = dragon_vale_detail_->is_totem_max_storage ? 1 : 0 ;
			building_notice_map.insert(std::make_pair(BUILDING_NOTICE_OUTPUT_PRODUCTION_MAX, notice));
		}

		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();
	}
}

void Dragon_Vale::process_login_building_finish() {
	MSG_82300002 msg;
	const Building_Level_Cfg *build_level_cfg = 0;
	long total_time = 0;

	//建筑是否建造、升级完成的标志
	uint8_t gold_mine_flag = 0;
	uint8_t moon_well_flag = 0;
	uint8_t totem_flag = 0;

	Time_Value now = Time_Value::gettimeofday();

	for(DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
				it != dragon_vale_detail_->dragon_building_info_map.end();
				++it) {
		//不是升级状态，不处理
		if(BUILDING_UPGRADE_DOING != it->second.status) {
			continue;
		}

		uint16_t new_level = it->second.level + 1;
		build_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(it->second.type, new_level);
		if(!build_level_cfg) {
			continue;
		}

		if(0 == it->second.level) {
			const Create_Building_Cfg *create_building_cfg =
							CONFIG_CACHE_DRAGON_VALE->find_Building_Create_Cfg(it->second.order);
			if(!create_building_cfg) {
				continue;
			}
			total_time = it->second.begin_upgrade_time.sec()
					+ (long)(create_building_cfg->create_need_hour * Time_Value::ONE_HOUR_IN_SECS)
					+ (long)(create_building_cfg->create_need_min * Time_Value::ONE_MINUTE_IN_SECS)
					+ (long)(create_building_cfg->create_need_second);
		}
		else {
			total_time = it->second.begin_upgrade_time.sec()
					+ build_level_cfg->upgrade_need_hour * Time_Value::ONE_HOUR_IN_SECS
					+ build_level_cfg->upgrade_need_min * Time_Value::ONE_MINUTE_IN_SECS
					+ build_level_cfg->upgrade_need_second;
		}

		if(total_time > now.sec()) {
			continue;
		}

		Time_Value total_time_value(total_time);

		it->second.level += 1;
		it->second.status = BUILDING_UPGRADE_FINISH;
		//建造或者升级完成，将上一次资源产出时间设置为当前时间(因为建造或者升级中，不能产出资源)
		it->second.last_output_production_time.set(total_time_value);
		dragon_vale_detail_->build_queue_num += 1;

		if(1 == it->second.level && dragon_vale_detail_->dragon_building_info_map.size() == 1) {
			dragon_vale_detail_->begin_refresh_thief_time.set(total_time_value);
		}

		if(BUILDING_GOLD_MINE == it->second.type) {
			if(1 == it->second.level){
				gold_mine_flag = 1;
			}
//			else {
//				gold_mine_flag = 2;
//			}
			player_self()->achievement_listen_dragon_wale_gold_mine_lvl(it->second.level);
		}

		if(BUILDING_MOON_WELL == it->second.type) {
			if(1 == it->second.level) {
				moon_well_flag = 1;
			}
//			else {
//				moon_well_flag = 2;
//			}
			player_self()->achievement_listen_dragon_wale_moon_well_lvl(it->second.level);
		}

		if(BUILDING_TOTEM == it->second.type) {
			if(1 == it->second.level) {
				totem_flag = 1;
			}
//			else {
//				totem_flag = 2;
//			}
			player_self()->achievement_listen_dragon_wale_totem_lvl(it->second.level);
		}

		Building_Info msg_building_info;
		msg_building_info.id = it->second.id;
		msg_building_info.type = it->second.type;
		msg_building_info.level = it->second.level;
		msg_building_info.status = it->second.status;
		msg_building_info.production = it->second.production;
		msg_building_info.is_gain = it->second.is_gain ? 1 : 0;
		msg_building_info.order = it->second.order;
		msg_building_info.consume_type = it->second.consume_type;

		msg.building_info_list.push_back(msg_building_info);
	}//end for

	if(!msg.building_info_list.empty()) {
		msg.role_id = player_self()->role_id();
		const Role_Level_Cfg *role_level_cfg = 0;
		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_GOLD_MINE);
		if(role_level_cfg) {
			msg.is_build_gold_mine =
					role_level_cfg->max_building_num > dragon_vale_detail_->build_gold_mine_num ? 1 : 0;
		}

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_MOON_WELL);
		if(role_level_cfg) {
			msg.is_build_moon_well = role_level_cfg->max_building_num > dragon_vale_detail_->build_moon_well_num ? 1 : 0;
		}

		role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_TOTEM);
		if(role_level_cfg) {
			msg.is_build_totem = role_level_cfg->max_building_num > dragon_vale_detail_->build_totem_num ? 1 : 0;
		}

		msg.build_queue_num = dragon_vale_detail_->build_queue_num;

		//在龙谷里面才会发消息
//		if(dragon_vale_detail_->target_player_id != 0) {
//			THIS_SEND_TO_CLIENT(msg);
//		}

		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();

		//在龙谷里面、外面都会发的消息，客户端弹窗用
		if((gold_mine_flag == 1)  || (moon_well_flag == 1) || (totem_flag == 1)) {
			Building_Notice notice;
			notice.gold_mine = gold_mine_flag;
			notice.moon_well = moon_well_flag;
			notice.totem = totem_flag;
			building_notice_map.insert(std::make_pair(BUILDING_NOTICE_UPGRADE_FINISH, notice));
		}

		if(dragon_vale_detail_->is_vip_down_to_zero && !is_has_building_upgrading()) {
			const Vip_Level_Cfg *vip_normal_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Vip_Level_Cfg(0);
			if(!vip_normal_cfg) {
				MSG_USER("vip_level_cfg not exist, vip_level=0");
				return;
			}

			dragon_vale_detail_->all_open_build_queue_num = vip_normal_cfg->max_queue_num;
			dragon_vale_detail_->build_queue_num = vip_normal_cfg->max_queue_num;

/*			if(player_self()->role_id() == dragon_vale_detail_->target_player_id) {
				MSG_82300022 msg;
				msg.all_open_build_queue_num = dragon_vale_detail_->all_open_build_queue_num;
				msg.build_queue_num = dragon_vale_detail_->build_queue_num;
				THIS_SEND_TO_CLIENT(msg);
			}*/

			this->save_dragon_vale_tick();
			this->sync_dragon_vale_to_db_view();
		}
	}//end if
}

void Dragon_Vale::process_login_dragon_born_finish() {
	//未开始孵化，不处理
	if(!dragon_vale_detail_->is_dragon_born) {
		return;
	}

	//龙蛋已孵化为龙，不处理
	if(dragon_vale_detail_->dragon_info.progress > 0) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	long total_time = dragon_vale_detail_->begin_born_time.sec()
							+ dragon_base_cfg.dragon_born_time * Time_Value::ONE_MINUTE_IN_SECS;

	//已开始孵化，但是，孵化未完成，不处理
	if(total_time > now.sec()) {
		return;
	}

	//孵化完成，进化程度变为1
	dragon_vale_detail_->dragon_info.progress = 1;

	//孵化完成，可以逗龙   注意：龙蛋孵化完成，马上就可以逗龙，不能重置逗龙冷却时间
	dragon_vale_detail_->dragon_info.is_play_dragon = true;

	//龙蛋孵化为龙，龙的心情值初始为最大值 - 1
	dragon_vale_detail_->dragon_info.feeling = dragon_base_cfg.max_feeling  - 1;

	//龙蛋孵化为龙，初始化龙的等级为0、 初始化属性
	dragon_vale_detail_->dragon_info.power_level = 0;
	dragon_vale_detail_->dragon_info.armor_level = 0;
	dragon_vale_detail_->dragon_info.stamina_level = 0;
	dragon_vale_detail_->dragon_info.hero_power_level = 0;
	dragon_vale_detail_->dragon_info.hero_armor_level = 0;
	dragon_vale_detail_->dragon_info.hero_stamina_level = 0;
	dragon_vale_detail_->dragon_info.crit_level = 0;
	dragon_vale_detail_->dragon_info.prevent_crit_level  = 0;
	dragon_vale_detail_->dragon_info.hero_crit_level = 0;
	dragon_vale_detail_->dragon_info.hero_prevent_crit_level = 0;

	const Dragon_Evolution_Cfg* dragon_evolution_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(dragon_vale_detail_->dragon_info.progress);
	if(dragon_evolution_cfg) {
		double temp_property = 0.0;
		const Dragon_Keep_Cfg* dragon_keep_cfg = 0;
		//初始化主角斗力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_POWER,
										dragon_vale_detail_->dragon_info.power_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.power = (uint32_t)temp_property;
		}

		//初始化主角护甲属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_ARMOR,
										dragon_vale_detail_->dragon_info.armor_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.armor = (uint32_t)temp_property;
		}

		//初始化主角耐力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_STAMINA,
										dragon_vale_detail_->dragon_info.stamina_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.stamina = (uint32_t)temp_property;
		}

		//初始化英雄斗力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_POWER,
										dragon_vale_detail_->dragon_info.hero_power_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_power = (uint32_t)temp_property;
		}

		//初始化英雄护甲属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_ARMOR,
										dragon_vale_detail_->dragon_info.hero_armor_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_armor = (uint32_t)temp_property;
		}

		//初始化英雄耐力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_STAMINA,
										dragon_vale_detail_->dragon_info.hero_stamina_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_stamina = (uint32_t)temp_property;
		}

		//初始化主角暴击属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_CRIT,
										dragon_vale_detail_->dragon_info.crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.crit = (uint32_t)temp_property;
		}

		//初始化主角抗暴属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_PREVENT_CRIT,
										dragon_vale_detail_->dragon_info.prevent_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.prevent_crit = (uint32_t)temp_property;
		}

		//初始化英雄暴击属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_CRIT,
										dragon_vale_detail_->dragon_info.hero_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_crit = (uint32_t)temp_property;
		}

		//初始化英雄抗暴属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_PREVENT_CRIT,
										dragon_vale_detail_->dragon_info.hero_prevent_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_prevent_crit = (uint32_t)temp_property;
		}

	}

	//重置龙心情值下降的开始时间
	dragon_vale_detail_->begin_down_feeling_time.set(now);

	//在龙谷里面才会发消息
//	if(dragon_vale_detail_->target_player_id != 0) {
//		MSG_82300004 msg;
//		msg.progress = dragon_vale_detail_->dragon_info.progress;
//		msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
//		THIS_SEND_TO_CLIENT(msg);
//	}

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}

void Dragon_Vale::process_login_play_dragon_interval_time() {
	//未开始孵化或者孵化未完成，不处理
	if(!dragon_vale_detail_->is_dragon_born || dragon_vale_detail_->dragon_info.progress < 1) {
		return;
	}

	//当前可以逗龙，不需要处理冷却时间
	if(dragon_vale_detail_->dragon_info.is_play_dragon) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();

	//龙的心情值满时，不可以逗龙,不处理逗龙冷却
	if(dragon_base_cfg.max_feeling <= dragon_vale_detail_->dragon_info.feeling) {
		return;
	}

	long total_time = dragon_vale_detail_->begin_play_dragon_interval_time.sec()
							+ dragon_base_cfg.play_dragon_time * Time_Value::ONE_MINUTE_IN_SECS;
	//冷却时间未到，不处理
	if(total_time > now.sec()) {
		return;
	}

	dragon_vale_detail_->dragon_info.is_play_dragon = true;

	//在龙谷里面才会发消息
//	if(dragon_vale_detail_->target_player_id != 0) {
//		MSG_82300007 msg;
//		msg.role_id = player_self()->role_id();
//		msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ?  1 : 0;
//		THIS_SEND_TO_CLIENT(msg);
//	}

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}

void Dragon_Vale::process_login_dragon_feeling_down_time() {
	//未开始孵化或者孵化未完成，不处理
	if(!dragon_vale_detail_->is_dragon_born || dragon_vale_detail_->dragon_info.progress < 1) {
		return;
	}

	//龙的心情值已下降到0，不处理
	if(0 == dragon_vale_detail_->dragon_info.feeling) {
		return;
	}

	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();

	Time_Value now = Time_Value::gettimeofday();
	long total_time = dragon_vale_detail_->begin_down_feeling_time.sec()
						+ dragon_base_cfg.down_feeling_time * Time_Value::ONE_MINUTE_IN_SECS;
	//时间未到,不处理
	if(total_time > now.sec()) {
		return;
	}

	//龙的心情值减1，最低为0
	dragon_vale_detail_->dragon_info.feeling =
			(dragon_vale_detail_->dragon_info.feeling > 0) ? dragon_vale_detail_->dragon_info.feeling - 1 : 0;

	//重置龙心情值下降的开始时间
	dragon_vale_detail_->begin_down_feeling_time.set(now);

	//在龙谷里面才会发消息
//	if(dragon_vale_detail_->target_player_id != 0) {
//		MSG_82300005 msg;
//		msg.type = 1;
//		msg.feeling = dragon_vale_detail_->dragon_info.feeling;
//		THIS_SEND_TO_CLIENT(msg);
//	}

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}

void Dragon_Vale::process_login_dragon_rob_protect_time() {
	if(!dragon_vale_detail_->is_rob_protect) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	long total_time = dragon_vale_detail_->begin_rob_protect_time.sec()
			          + main_base_cfg.rob_time_hour * Time_Value::ONE_HOUR_IN_SECS
			          + main_base_cfg.rob_time_minute * Time_Value::ONE_MINUTE_IN_SECS;
	if(total_time > now.sec()) {
		return;
	}

	dragon_vale_detail_->is_rob_protect = false;
	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}

void Dragon_Vale::process_login_refresh_dragon_vale_monster() {
	if(dragon_vale_detail_->dragon_building_info_map.empty()) {
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	if(dragon_vale_detail_->thief_id_quality_map.size() >= main_base_cfg.max_thief_num) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();
	long total_time = dragon_vale_detail_->begin_refresh_thief_time.sec()
					+ main_base_cfg.refresh_thief_time * Time_Value::ONE_MINUTE_IN_SECS;
	if(total_time > now.sec()) {
		return;
	}

	bool is_has_building_finish = dragon_vale_detail_->is_has_building_finish();
	if(!is_has_building_finish) {
		return;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint8_t chance = random() % 100;
	if(chance > main_base_cfg.refresh_thief_chance) {
		return;
	}

	const Main_Thief_Id_Set &main_thief_id_set = CONFIG_CACHE_DRAGON_VALE->get_main_config().main_thief_id_set;
	std::vector<uint32_t> temp;
	for(Main_Thief_Id_Set::const_iterator it = main_thief_id_set.begin(); it != main_thief_id_set.end(); ++it) {
		if(dragon_vale_detail_->thief_id_quality_map.find((int64_t)*it)
				!= dragon_vale_detail_->thief_id_quality_map.end()) {
			continue;
		}
		temp.push_back(*it);
	}

	if(temp.empty()) {
		return;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint32_t index = random()%temp.size();
	uint32_t id = temp[index];
	uint8_t  quality = DRAGON_VALE_MANAGER->get_dragon_vale_refresh_monster_quality();

	dragon_vale_detail_->begin_refresh_thief_time.set(now);
	dragon_vale_detail_->thief_id_quality_map.insert(std::make_pair(id, quality));

	dragon_vale_detail_->check_and_change_notice_board_num();
	if(dragon_vale_detail_->notice_board_list.size() < 50) {
		Notice_Board notice_board;
		Time_Value now = Time_Value::gettimeofday();
		notice_board.type = NOTICE_BOARD_THIEF_APPEAR;
		notice_board.time = now.sec();

		const Monster_Section_Config *monster_section_cfg =
				CONFIG_CACHE_NPC->get_monster_section_config(main_base_cfg.dragon_vale_scene_id);
		if(monster_section_cfg) {
			Monster_Point_Config_Map::const_iterator it = monster_section_cfg->point_map.find(id);
			if(it != monster_section_cfg->point_map.end()) {
				const std::vector<Int_Int> &npc_vec = it->second.npc_vec;
				if(!npc_vec.empty()) {
					uint32_t monster_id = npc_vec.begin()->val_1;
					const NPC_Config_Cache *npc_config_cache =
							CONFIG_CACHE_NPC->find_npc_config_cache(monster_id);
					if(npc_config_cache) {
						notice_board.host = npc_config_cache->name;
						notice_board.param1 = quality;
						dragon_vale_detail_->notice_board_list.push_back(notice_board);
					}
				}
			}
		}
	}

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	DRAGON_VALE_MANAGER->refresh_dragon_vale_op(player_self()->role_id());
}

void Dragon_Vale::listen_refresh_dragon_vale_monster(const Time_Value &now) {
	if(dragon_vale_detail_->dragon_building_info_map.empty()) {
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	if(dragon_vale_detail_->thief_id_quality_map.size() >= main_base_cfg.max_thief_num) {
		return;
	}

	long total_time = dragon_vale_detail_->begin_refresh_thief_time.sec()
					+ main_base_cfg.refresh_thief_time * Time_Value::ONE_MINUTE_IN_SECS;
	if(total_time > now.sec()) {
		return;
	}

	bool is_has_building_finish = dragon_vale_detail_->is_has_building_finish();
	if(!is_has_building_finish) {
		return;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint8_t chance = random() % 100;
	if(chance > main_base_cfg.refresh_thief_chance) {
		return;
	}

	MSG_20600015 msg;
	msg.type = 1;
	msg.role_id = player_self()->role_id();

	const Main_Thief_Id_Set &main_thief_id_set = CONFIG_CACHE_DRAGON_VALE->get_main_config().main_thief_id_set;
	std::vector<uint32_t> temp;
	for(Main_Thief_Id_Set::const_iterator it = main_thief_id_set.begin(); it != main_thief_id_set.end(); ++it) {
		if(dragon_vale_detail_->thief_id_quality_map.find((int64_t)*it)
				!= dragon_vale_detail_->thief_id_quality_map.end()) {
			continue;
		}
		temp.push_back(*it);
	}

	if(temp.empty()) {
		return;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	uint32_t index = random()%temp.size();
	uint32_t id = temp[index];
	msg.id = id;
	uint8_t  quality = DRAGON_VALE_MANAGER->get_dragon_vale_refresh_monster_quality();
	msg.quality = quality;

	dragon_vale_detail_->begin_refresh_thief_time.set(now);
	bool flag = dragon_vale_detail_->thief_id_quality_map.insert(std::make_pair(id, quality)).second;
	if(!flag) {
		return;
	}


	Block_Buffer broadcast_buf;
	broadcast_buf.make_message(INNER_DRAGON_VALE_MONSTER);
	msg.serialize(broadcast_buf);
	broadcast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broadcast_buf);


	dragon_vale_detail_->check_and_change_notice_board_num();
	if(dragon_vale_detail_->notice_board_list.size() < 50) {
		Notice_Board notice_board;
		Time_Value now = Time_Value::gettimeofday();
		notice_board.type = NOTICE_BOARD_THIEF_APPEAR;
		notice_board.time = now.sec();

		const Monster_Section_Config *monster_section_cfg =
				CONFIG_CACHE_NPC->get_monster_section_config(main_base_cfg.dragon_vale_scene_id);
		if(monster_section_cfg) {
			Monster_Point_Config_Map::const_iterator it = monster_section_cfg->point_map.find(id);
			if(it != monster_section_cfg->point_map.end()) {
				const std::vector<Int_Int> &npc_vec = it->second.npc_vec;
				if(!npc_vec.empty()) {
					uint32_t monster_id = npc_vec.begin()->val_1;
					const NPC_Config_Cache *npc_config_cache =
							CONFIG_CACHE_NPC->find_npc_config_cache(monster_id);
					if(npc_config_cache) {
						notice_board.host = npc_config_cache->name;
						notice_board.param1 = quality;
						dragon_vale_detail_->notice_board_list.push_back(notice_board);
					}
				}
			}
		}
	}//end if

	MSG_82300030 msg_82300030;
	msg_82300030.head_info.role_id = player_self()->role_id();
	msg_82300030.head_info.role_name = player_self()->role_name();
	msg_82300030.head_info.star_num = dragon_vale_detail_->star_num;
	msg_82300030.head_info.head_photo_id = player_self()->friend_detail().head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(dragon_vale_detail_->star_num);
	if(!star_add_speed) {
		msg_82300030.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", dragon_vale_detail_->star_num);
	}
	else {
		msg_82300030.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_82300030.head_info.thief_sub_speed =
		(uint16_t)(dragon_vale_detail_->thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = dragon_vale_detail_->foot_info_map.begin();
			it != dragon_vale_detail_->foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_82300030.head_info.foot_list.push_back(msg_foot_info);
	}

	Block_Buffer buf;
	buf.make_message(msg_82300030.msg_id);
	msg_82300030.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	DRAGON_VALE_MANAGER->refresh_dragon_vale_op(player_self()->role_id());
}

bool Dragon_Vale::is_dragon_vale_special() {
	return dragon_vale_detail_->is_special;
}

void Dragon_Vale::hook_dragon_vale_special_change() {

}

int Dragon_Vale::req_step_foot(int64_t role_id) {
	if(player_self()->role_id() == role_id) {
		return ERROR_NOT_STEP_FOOT_ON_SELF_DRAGON_VALE;
	}

	if(dragon_vale_detail_->foot_id_set.find(role_id)
			!= dragon_vale_detail_->foot_id_set.end()) {
		return ERROR_STEP_FOOT_ONCE_IN_SAME_DRAGON_PERDAY;
	}

	player_self()->task_listen_dragon_vale_step_foot_all();
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_DRAGON_STEP_FOOT); //日常助手监听踩脚印

	Logic_Player *target = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!target) {
		return process_step_offline_player_foot(role_id);
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	uint32_t award = 0;
	if(dragon_vale_detail_->award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
		const uint32_t *foot_award = CONFIG_CACHE_DRAGON_VALE->find_main_foot_award(player_self()->level());
		if(!foot_award) {
			award = 0;
			MSG_USER("step foot award not exist, player level=%d", player_self()->level());
		}
		else {
			dragon_vale_detail_->award_foot_num += 1;
			award = *foot_award;

			const Dragon_Base_Cfg &dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->get_dragon_config().dragon_base_cfg;
			Money_Sub_Type money_sub_type = Pack::MONEY_SUB_TYPE_END;
			Money_Gain_Type money_gain_type = MONEY_ADD_MAX;
			switch(dragon_base_cfg.dragon_vale_op_type) {
			case Pack::COPPER:
				money_sub_type = Pack::COPPER_ONLY;
				money_gain_type = Pack::MONEY_ADD_STEP_FOOT_GET_COIN;
				break;

			case Pack::DRAGON_SOUL:
				money_sub_type = Pack::DRAGON_SOUL_ONLY;
				money_gain_type = Pack::MONEY_ADD_STEP_FOOT_GET_DRAGON_SOUL;
				break;

			case Pack::SOULS:
				money_sub_type = Pack::SOULS_ONLY;
				money_gain_type = Pack::MONEY_ADD_SOUL_BY_STEP_FOOT;
				break;

			case Pack::FRIENDSHIP:
				money_sub_type = Pack::FRIENDSHIP_ONLY;
				money_gain_type = Pack::MONEY_ADD_FRIENDSHIP_BY_STEP_FOOT;
				break;
			}

			player_self()->gain_money_in_dragon_vale(money_sub_type,
							*foot_award,
							money_gain_type);
		}
	}

	bool is_up_star = false;
	uint32_t self_dragon_heart_id = 0;
	uint32_t target_dragon_heart_id = 0;
	if(target->dragon_vale_detail().star_num < 5) {
		srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
		uint8_t chance = random() % 100;
		if(dragon_vale_detail_->is_special) {
			if(chance < main_base_cfg.special_player_chance) {
				is_up_star = true;
			}
		}
		else {
			if(chance < main_base_cfg.normal_player_chance) {
				is_up_star = true;
			}
		}
	}

	if(is_up_star) {
		target->dragon_vale_detail_no_const().star_num += 1;
		if(target->dragon_vale_detail().foot_info_map.size() < 5) {
			Time_Value now = Time_Value::gettimeofday();
			Foot_Info foot_info;
			foot_info.sex = player_self()->gender();
			foot_info.name = player_self()->role_name();
			target->dragon_vale_detail_no_const().foot_info_map.insert(std::make_pair(now.sec(), foot_info));
		}
		if(dragon_vale_detail_->dragon_heart_num < main_base_cfg.max_dragon_heart_num) {
			dragon_vale_detail_->dragon_heart_num += 1;
			if(CONFIG_CACHE_ITEM->find_item(main_base_cfg.dragon_heart_item_id)) {
				self_dragon_heart_id = main_base_cfg.dragon_heart_item_id;
				Item_Detail item(main_base_cfg.dragon_heart_item_id, 1, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
						Gain_Item_DM_Info(Pack::ITEM_GAIN_STEP_FOOT));
				MSG_81000102 acv_msg;
				acv_msg.type = 0;
				Item_Basic_Info ibi;
				item.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				THIS_SEND_TO_CLIENT(acv_msg);
			}
		}
		if(target->dragon_vale_detail().dragon_heart_num < main_base_cfg.max_dragon_heart_num) {
			target->dragon_vale_detail_no_const().dragon_heart_num += 1;
			if(CONFIG_CACHE_ITEM->find_item(main_base_cfg.dragon_heart_item_id)) {
				target_dragon_heart_id = main_base_cfg.dragon_heart_item_id;
				Item_Detail item(main_base_cfg.dragon_heart_item_id, 1, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
				target->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
						Gain_Item_DM_Info(Pack::ITEM_GAIN_OTHER_STEP_FOOT));
				MSG_81000102 acv_msg;
				acv_msg.type = 0;
				Item_Basic_Info ibi;
				item.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				OBJ_SEND_TO_CLIENT(acv_msg, (*target));
			}
		}
	}

	dragon_vale_detail_->foot_id_set.insert(target->role_id());

	target->dragon_vale_detail_no_const().check_and_change_notice_board_num();
	if(target->dragon_vale_detail_no_const().notice_board_list.size() < 50) {
		Notice_Board notice_board;
		Time_Value now = Time_Value::gettimeofday();
		notice_board.type = NOTICE_BOARD_STEP_FOOT;
		notice_board.time = now.sec();
		notice_board.host = player_self()->role_name();
		notice_board.dragon_soul = award;
		notice_board.is_get_dragon_heart = (self_dragon_heart_id != 0) ? 1 : 0;
		target->dragon_vale_detail_no_const().notice_board_list.push_back(notice_board);
	}

	MSG_50180005 msg_50180005;
	msg_50180005.is_up_star = is_up_star;
	msg_50180005.dragon_heart = self_dragon_heart_id;
	msg_50180005.dragon_soul_award = award;
	msg_50180005.head_info.role_id = target->role_id();
	msg_50180005.head_info.role_name = target->role_name();
	msg_50180005.head_info.star_num = target->dragon_vale_detail().star_num;
	msg_50180005.head_info.head_photo_id = target->friend_detail().head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(target->dragon_vale_detail().star_num);
	if(!star_add_speed) {
		msg_50180005.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", target->dragon_vale_detail().star_num);
	}
	else {
		msg_50180005.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_50180005.head_info.thief_sub_speed =
		(uint16_t)(target->dragon_vale_detail().thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = target->dragon_vale_detail().foot_info_map.begin();
			it != target->dragon_vale_detail().foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_50180005.head_info.foot_list.push_back(msg_foot_info);
	}
	THIS_SEND_TO_CLIENT(msg_50180005);

	if(is_up_star) {
		MSG_82300010 msg_82300010;
		msg_82300010.is_special = dragon_vale_detail_->is_special ? 1 : 0;
		msg_82300010.name = player_self()->role_name();
		msg_82300010.star = target->dragon_vale_detail().star_num;
		msg_82300010.dragon_heart = target_dragon_heart_id;
		msg_82300010.head_info.role_id = target->role_id();
		msg_82300010.head_info.role_name = target->role_name();
		msg_82300010.head_info.star_num = target->dragon_vale_detail().star_num;
		msg_82300010.head_info.head_photo_id = target->friend_detail().head_photo_id;
		star_add_speed =
					CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(target->dragon_vale_detail().star_num);
		if(!star_add_speed) {
			msg_82300010.head_info.star_add_speed = 0;
			MSG_USER("star up spped not exist, star=%d", target->dragon_vale_detail().star_num);
		}
		else {
			msg_82300010.head_info.star_add_speed = (uint16_t)(*star_add_speed);
		}

		msg_82300010.head_info.thief_sub_speed =
			(uint16_t)(target->dragon_vale_detail().thief_id_quality_map.size() * main_base_cfg.thief_down_production);
		for(Foot_Info_Map::const_iterator it = target->dragon_vale_detail().foot_info_map.begin();
				it != target->dragon_vale_detail().foot_info_map.end();
				++it) {
			Dragon_Vale_Foot_Info msg_foot_info;
			msg_foot_info.sex = it->second.sex;
			msg_foot_info.name = it->second.name;
			msg_82300010.head_info.foot_list.push_back(msg_foot_info);
		}

		OBJ_SEND_TO_CLIENT(msg_82300010, (*target));
	}

	MSG_82300030 msg_82300030;
	msg_82300030.head_info.role_id = target->role_id();
	msg_82300030.head_info.role_name = target->role_name();
	msg_82300030.head_info.star_num = target->dragon_vale_detail().star_num;
	msg_82300030.head_info.head_photo_id = target->friend_detail().head_photo_id;
	star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(target->dragon_vale_detail().star_num);
	if(!star_add_speed) {
		msg_82300030.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", target->dragon_vale_detail().star_num);
	}
	else {
		msg_82300030.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_82300030.head_info.thief_sub_speed =
		(uint16_t)(target->dragon_vale_detail().thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = target->dragon_vale_detail().foot_info_map.begin();
			it != target->dragon_vale_detail().foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_82300030.head_info.foot_list.push_back(msg_foot_info);
	}

	Block_Buffer buf;
	buf.make_message(msg_82300030.msg_id);
	msg_82300030.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(target->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	((Dragon_Vale*)target)->save_dragon_vale_tick();

	this->sync_dragon_vale_to_db_view();
	target->sync_dragon_vale_to_db_view();
	if (is_up_star) {
		player_self()->achievement_listen_dragon_wale_step_foot_times();
	}

	refresh_dragon_vale_op_by_step_foot(role_id);

	return 0;
}

void Dragon_Vale::sync_special_to_db_view() {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(player_self()->role_id());
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	find_it->second->dragon_vale_view.is_special = dragon_vale_detail_->is_special;
}

void Dragon_Vale::sync_dragon_vale_reset_to_db_view() {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(player_self()->role_id());
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	find_it->second->dragon_vale_view.matched_num = dragon_vale_detail_->matched_num;
	find_it->second->dragon_vale_view.star_num = dragon_vale_detail_->star_num;
	find_it->second->dragon_vale_view.award_foot_num = dragon_vale_detail_->award_foot_num;
	find_it->second->dragon_vale_view.foot_info_map.clear();
	find_it->second->dragon_vale_view.foot_id_set.clear();
	find_it->second->dragon_vale_view.dragon_heart_num = dragon_vale_detail_->dragon_heart_num;
}

int Dragon_Vale::process_step_offline_player_foot(int64_t role_id) {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return 0;
	}

	if(!find_it->second) {
		return 0;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	uint32_t award = 0;
	if(dragon_vale_detail_->award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
		const uint32_t *foot_award = CONFIG_CACHE_DRAGON_VALE->find_main_foot_award(player_self()->level());
		if(!foot_award) {
			award = 0;
			MSG_USER("step foot award not exist, player level=%d", player_self()->level());
		}
		else {
			dragon_vale_detail_->award_foot_num += 1;
			award = *foot_award;

			const Dragon_Base_Cfg &dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->get_dragon_config().dragon_base_cfg;
			Money_Sub_Type money_sub_type = Pack::MONEY_SUB_TYPE_END;
			Money_Gain_Type money_gain_type = MONEY_ADD_MAX;
			switch(dragon_base_cfg.dragon_vale_op_type) {
			case Pack::COPPER:
				money_sub_type = Pack::COPPER_ONLY;
				money_gain_type = Pack::MONEY_ADD_STEP_FOOT_GET_COIN;
				break;

			case Pack::DRAGON_SOUL:
				money_sub_type = Pack::DRAGON_SOUL_ONLY;
				money_gain_type = Pack::MONEY_ADD_STEP_FOOT_GET_DRAGON_SOUL;
				break;

			case Pack::SOULS:
				money_sub_type = Pack::SOULS_ONLY;
				money_gain_type = Pack::MONEY_ADD_SOUL_BY_STEP_FOOT;
				break;

			case Pack::FRIENDSHIP:
				money_sub_type = Pack::FRIENDSHIP_ONLY;
				money_gain_type = Pack::MONEY_ADD_FRIENDSHIP_BY_STEP_FOOT;
				break;
			}

			player_self()->gain_money_in_dragon_vale(money_sub_type,
							*foot_award,
							money_gain_type);
		}
	}

	bool is_up_star = false;
	uint32_t self_dragon_heart_id = 0;
	uint32_t target_dragon_heart_id = 0;
	if(find_it->second->dragon_vale_view.star_num < 5) {
		srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
		uint8_t chance = random() % 100;
		if(dragon_vale_detail_->is_special) {
			if(chance < main_base_cfg.special_player_chance) {
				is_up_star = true;
			}
		}
		else {
			if(chance < main_base_cfg.normal_player_chance) {
				is_up_star = true;
			}
		}
	}

	if(is_up_star) {
		find_it->second->dragon_vale_view.star_num += 1;
		if(find_it->second->dragon_vale_view.foot_info_map.size() < 5) {
			Time_Value now = Time_Value::gettimeofday();
			Foot_Info foot_info;
			foot_info.sex = player_self()->gender();
			foot_info.name = player_self()->role_name();
			find_it->second->dragon_vale_view.foot_info_map.insert(std::make_pair(now.sec(), foot_info));
		}
		if(dragon_vale_detail_->dragon_heart_num < main_base_cfg.max_dragon_heart_num) {
			dragon_vale_detail_->dragon_heart_num += 1;
			if(CONFIG_CACHE_ITEM->find_item(main_base_cfg.dragon_heart_item_id)) {
				self_dragon_heart_id = main_base_cfg.dragon_heart_item_id;
				Item_Detail item(main_base_cfg.dragon_heart_item_id, 1, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
						Gain_Item_DM_Info(Pack::ITEM_GAIN_STEP_FOOT));

				MSG_81000102 acv_msg;
				acv_msg.type = 0;
				Item_Basic_Info ibi;
				item.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				THIS_SEND_TO_CLIENT(acv_msg);
			}
		}
		if(find_it->second->dragon_vale_view.dragon_heart_num < main_base_cfg.max_dragon_heart_num) {
			find_it->second->dragon_vale_view.dragon_heart_num += 1;
			if(CONFIG_CACHE_ITEM->find_item(main_base_cfg.dragon_heart_item_id)) {
				target_dragon_heart_id = main_base_cfg.dragon_heart_item_id;
				Item_Detail item(main_base_cfg.dragon_heart_item_id, 1, static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));

				const Mail_Base_Config *mail_cfg = CONFIG_CACHE_MAIL->find_config(1030);
				if(mail_cfg) {
					Item_Vec item_vec;
					item_vec.push_back(item);

					size_t size = mail_cfg->mail_content.size() + player_self()->role_name().size() + 10;
					char mail_buf[size];
					memset(mail_buf, 0, size);
					snprintf(mail_buf, size, mail_cfg->mail_content.c_str(), player_self()->role_name().c_str());
					std::string mail_content(mail_buf);
					LOGIC_PUBLIC->send_mail(1030, find_it->second->role_id, item_vec,
											mail_cfg->mail_title, mail_content);
				}
			}
		}
	}

	dragon_vale_detail_->foot_id_set.insert(find_it->second->role_id);

	find_it->second->dragon_vale_view.check_and_change_notice_board_num();
	if(find_it->second->dragon_vale_view.notice_board_list.size() < 50) {
		Notice_Board notice_board;
		Time_Value now = Time_Value::gettimeofday();
		notice_board.type = NOTICE_BOARD_STEP_FOOT;
		notice_board.time = now.sec();
		notice_board.host = player_self()->role_name();
		notice_board.dragon_soul = award;
		notice_board.is_get_dragon_heart = (self_dragon_heart_id != 0) ? 1 : 0;
		find_it->second->dragon_vale_view.notice_board_list.push_back(notice_board);
	}

	MSG_50180005 msg_50180005;
	msg_50180005.is_up_star = is_up_star;
	msg_50180005.dragon_heart = self_dragon_heart_id;
	msg_50180005.dragon_soul_award = award;
	msg_50180005.head_info.role_id = find_it->second->role_id;
	msg_50180005.head_info.role_name = find_it->second->role_name;
	msg_50180005.head_info.star_num = find_it->second->dragon_vale_view.star_num;
	msg_50180005.head_info.head_photo_id = find_it->second->friend_.head_photo_id;
	const uint8_t *star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(find_it->second->dragon_vale_view.star_num);
	if(!star_add_speed) {
		msg_50180005.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", find_it->second->dragon_vale_view.star_num);
	}
	else {
		msg_50180005.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_50180005.head_info.thief_sub_speed =
		(uint16_t)(find_it->second->dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = find_it->second->dragon_vale_view.foot_info_map.begin();
			it != find_it->second->dragon_vale_view.foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_50180005.head_info.foot_list.push_back(msg_foot_info);
	}
	THIS_SEND_TO_CLIENT(msg_50180005);

	if(is_up_star) {
		MSG_82300010 msg_82300010;
		msg_82300010.is_special = dragon_vale_detail_->is_special ? 1 : 0;
		msg_82300010.name = player_self()->role_name();
		msg_82300010.star = find_it->second->dragon_vale_view.star_num;
		msg_82300010.dragon_heart = target_dragon_heart_id;
		msg_82300010.head_info.role_id =  find_it->second->role_id;
		msg_82300010.head_info.role_name =  find_it->second->role_name;
		msg_82300010.head_info.star_num = find_it->second->dragon_vale_view.star_num;
		msg_82300010.head_info.head_photo_id = find_it->second->friend_.head_photo_id;
		star_add_speed =
					CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(find_it->second->dragon_vale_view.star_num);
		if(!star_add_speed) {
			msg_82300010.head_info.star_add_speed = 0;
			MSG_USER("star up spped not exist, star=%d", find_it->second->dragon_vale_view.star_num);
		}
		else {
			msg_82300010.head_info.star_add_speed = (uint16_t)(*star_add_speed);
		}

		msg_82300010.head_info.thief_sub_speed =
			(uint16_t)(find_it->second->dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);
		for(Foot_Info_Map::const_iterator it = find_it->second->dragon_vale_view.foot_info_map.begin();
				it != find_it->second->dragon_vale_view.foot_info_map.end();
				++it) {
			Dragon_Vale_Foot_Info msg_foot_info;
			msg_foot_info.sex = it->second.sex;
			msg_foot_info.name = it->second.name;
			msg_82300010.head_info.foot_list.push_back(msg_foot_info);
		}

	//	OBJ_SEND_TO_CLIENT(msg_82300010, (*target));
		//TODO offline
	}

	MSG_82300030 msg_82300030;
	msg_82300030.head_info.role_id = find_it->second->role_id;
	msg_82300030.head_info.role_name = find_it->second->role_name;
	msg_82300030.head_info.star_num = find_it->second->dragon_vale_view.star_num;
	msg_82300030.head_info.head_photo_id = find_it->second->friend_.head_photo_id;
	star_add_speed =
				CONFIG_CACHE_DRAGON_VALE->find_Main_Star_Up_Spped(find_it->second->dragon_vale_view.star_num);
	if(!star_add_speed) {
		msg_82300030.head_info.star_add_speed = 0;
		MSG_USER("star up spped not exist, star=%d", find_it->second->dragon_vale_view.star_num);
	}
	else {
		msg_82300030.head_info.star_add_speed = (uint16_t)(*star_add_speed);
	}

	msg_82300030.head_info.thief_sub_speed =
		(uint16_t)(find_it->second->dragon_vale_view.thief_id_quality_map.size() * main_base_cfg.thief_down_production);
	for(Foot_Info_Map::const_iterator it = find_it->second->dragon_vale_view.foot_info_map.begin();
			it != find_it->second->dragon_vale_view.foot_info_map.end();
			++it) {
		Dragon_Vale_Foot_Info msg_foot_info;
		msg_foot_info.sex = it->second.sex;
		msg_foot_info.name = it->second.name;
		msg_82300030.head_info.foot_list.push_back(msg_foot_info);
	}

	Block_Buffer buf;
	buf.make_message(msg_82300030.msg_id);
	msg_82300030.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(find_it->second->role_id);
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	this->sync_dragon_vale_to_db_view();

	Block_Buffer db_buf;
	db_buf.make_message(MONGO_SAVE_DRAGON_VALE_DETAIL);
	find_it->second->dragon_vale_view.serialize(db_buf);
	db_buf.finish_message();
	DB_MANAGER->push_data_block(db_buf, 0);

	if (is_up_star) {
		player_self()->achievement_listen_dragon_wale_step_foot_times();
	}

	refresh_dragon_vale_op_by_step_foot(role_id);
	return 0;
}

int Dragon_Vale::req_get_dragon_vale_notice_board() {
	MSG_50180017 msg;
	for(Notice_Board_List::const_iterator it = dragon_vale_detail_->notice_board_list.begin();
			it != dragon_vale_detail_->notice_board_list.end();
			++it) {
			Dragon_Vale_Notice notice;
			notice.type = it->type;
			notice.time = it->time;
			notice.host = it->host;
			notice.target = it->target;
			notice.dragon_soul = it->dragon_soul;
			notice.coin = it->coin;
			notice.soul = it->soul;
			notice.is_get_dragon_heart = it->is_get_dragon_heart;
			notice.param1  = it->param1;
			msg.notice_list.push_back(notice);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Dragon_Vale::listen_dragon_rob_protect_time(const Time_Value &now) {
	if(!dragon_vale_detail_->is_rob_protect) {
		return;
	}

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	long total_time = dragon_vale_detail_->begin_rob_protect_time.sec()
			          + main_base_cfg.rob_time_hour * Time_Value::ONE_HOUR_IN_SECS
			          + main_base_cfg.rob_time_minute * Time_Value::ONE_MINUTE_IN_SECS;
	if(total_time > now.sec()) {
		return;
	}

	dragon_vale_detail_->is_rob_protect = false;

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}

int Dragon_Vale::rob_dragon_vale_task_listen() {
	player_self()->task_listen_rob_other_dragon_vale_all();
	return 0;
}

int Dragon_Vale::get_dragon_vale_building_type_level_map(boost::unordered_multimap<uint8_t, uint16_t> &building_map) {
	for(DragonBuildingInfoMap::const_iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
			it != dragon_vale_detail_->dragon_building_info_map.end();
			++it) {
		building_map.insert(std::make_pair(it->second.type, it->second.level));
	}
	return 0;
}

void Dragon_Vale::dragon_born_right_now_test() {
	//未开始孵化，不处理
	if(!dragon_vale_detail_->is_dragon_born) {
		return;
	}

	//龙蛋已孵化为龙，不处理
	if(dragon_vale_detail_->dragon_info.progress > 0) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();

	dragon_vale_detail_->begin_born_time.set(Time_Value::zero);

	//孵化完成，进化程度变为1
	dragon_vale_detail_->dragon_info.progress = 1;

	//孵化完成，可以逗龙   注意：龙蛋孵化完成，马上就可以逗龙，不能重置逗龙冷却时间
	dragon_vale_detail_->dragon_info.is_play_dragon = true;

	//龙蛋孵化为龙，龙的心情值初始为最大值 - 1
	dragon_vale_detail_->dragon_info.feeling = dragon_base_cfg.max_feeling  - 1;

	//龙蛋孵化为龙，初始化龙的等级为0、 初始化属性
	dragon_vale_detail_->dragon_info.power_level = 0;
	dragon_vale_detail_->dragon_info.armor_level = 0;
	dragon_vale_detail_->dragon_info.stamina_level = 0;
	dragon_vale_detail_->dragon_info.hero_power_level = 0;
	dragon_vale_detail_->dragon_info.hero_armor_level = 0;
	dragon_vale_detail_->dragon_info.hero_stamina_level = 0;
	dragon_vale_detail_->dragon_info.crit_level  = 0;
	dragon_vale_detail_->dragon_info.prevent_crit_level = 0;
	dragon_vale_detail_->dragon_info.hero_crit_level = 0;
	dragon_vale_detail_->dragon_info.hero_prevent_crit_level = 0;

	const Dragon_Evolution_Cfg* dragon_evolution_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(dragon_vale_detail_->dragon_info.progress);
	if(dragon_evolution_cfg) {
		double temp_property = 0.0;
		const Dragon_Keep_Cfg* dragon_keep_cfg = 0;
		//初始化主角斗力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_POWER,
										dragon_vale_detail_->dragon_info.power_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.power = (uint32_t)temp_property;
		}

		//初始化主角护甲属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_ARMOR,
										dragon_vale_detail_->dragon_info.armor_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.armor = (uint32_t)temp_property;
		}

		//初始化主角耐力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_STAMINA,
										dragon_vale_detail_->dragon_info.stamina_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.stamina = (uint32_t)temp_property;
		}

		//初始化英雄斗力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_POWER,
										dragon_vale_detail_->dragon_info.hero_power_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_power = (uint32_t)temp_property;
		}

		//初始化英雄护甲属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_ARMOR,
										dragon_vale_detail_->dragon_info.hero_armor_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_armor = (uint32_t)temp_property;
		}

		//初始化英雄耐力属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_STAMINA,
										dragon_vale_detail_->dragon_info.hero_stamina_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_stamina = (uint32_t)temp_property;
		}

		//初始化主角暴击属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_CRIT,
										dragon_vale_detail_->dragon_info.crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.crit = (uint32_t)temp_property;
		}

		//初始化主角抗暴属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_PREVENT_CRIT,
										dragon_vale_detail_->dragon_info.prevent_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.prevent_crit = (uint32_t)temp_property;
		}

		//初始化英雄暴击属性
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_CRIT,
										dragon_vale_detail_->dragon_info.hero_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_crit = (uint32_t)temp_property;
		}

		//初始化英雄抗暴
		dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_PREVENT_CRIT,
										dragon_vale_detail_->dragon_info.hero_prevent_crit_level);
		if(dragon_keep_cfg) {
			temp_property = dragon_evolution_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
				dragon_vale_detail_->dragon_info.hero_prevent_crit = (uint32_t)temp_property;
		}

	}

	//重置龙心情值下降的开始时间
	dragon_vale_detail_->begin_down_feeling_time.set(now);

	//boradcast to dragon vale scene
	MSG_82300004 msg;
	msg.progress = dragon_vale_detail_->dragon_info.progress;
	msg.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	Prop_Setter_Vec prop_vec;
	Prop_Setter setter;

	if(dragon_vale_detail_->dragon_info.power > 0) {
		setter.reset();
		setter.prop_type = PT_POWER;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = dragon_vale_detail_->dragon_info.power;
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.armor > 0) {
		setter.reset();
		setter.prop_type = PT_ARMOR;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = dragon_vale_detail_->dragon_info.armor;
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.stamina > 0) {
		setter.reset();
		setter.prop_type = PT_STAMINA;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = dragon_vale_detail_->dragon_info.stamina;
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.crit > 0) {
		setter.reset();
		setter.prop_type = PT_CRIT;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.crit);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.prevent_crit > 0) {
		setter.reset();
		setter.prop_type = PT_CRIT_DEF;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.prevent_crit);
		prop_vec.push_back(setter);
	}

	if (prop_vec.size() > 0) player_self()-> fight_prop_modify (prop_vec, 208);

	player_self()->heroer_prop_refresh();
}

int Dragon_Vale::req_batch_evolution_dragon(const uint8_t type, const uint16_t batch_num, const uint16_t ten_times) {
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	if(player_self()->level() < (int)dragon_base_cfg.evolution_open_level) {
		return ERROR_DRAGON_EVOLUTION_LEVEL_LIMIT;
	}

	if(dragon_vale_detail_->dragon_info.progress >= dragon_base_cfg.max_evolution) {
		return ERROR_DRAGON_EVOLUTION_MAX;
	}

	if(dragon_base_cfg.max_batch_evolution_num < 1) {
		return ERROR_BATCH_EVOLUTION_DRAGON_CFG_INVALID;
	}

	uint8_t new_progresss = dragon_vale_detail_->dragon_info.progress + 1;
	const Dragon_Evolution_Cfg *dragon_evolution_cfg =
				CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(new_progresss);
	if(!dragon_evolution_cfg) {
		return ERROR_DRAGON_EVOLUTION_NOT_EXIST;
	}

	if(type != 0 && type != 1 && type != 2 && type != 3) {
		return ERROR_BATCH_EVOLUTION_DRAGON_CLIENT_TYPE_INVALID;
	}

	if(type == 0) {
		return req_batch_evolution_dragon_type_0(*dragon_evolution_cfg);
	}

	//只扣道具
	if(type == 1) {
		int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
				 Id_Amount(dragon_evolution_cfg->item_id, dragon_evolution_cfg->item_num * batch_num),
				 Lose_Item_DM_Info(ITEM_LOSE_BATCH_EVOLUTION_DRAGON));
		if (0 != result) {
			MSG_USER("error :sub item failed, item_id:%ld, item_num:%ld\n",
					dragon_evolution_cfg->item_id, dragon_evolution_cfg->item_num * batch_num);
			return ERROR_SUB_ITEM_FAIL;
		}

		return process_batch_evolution_dragon(*dragon_evolution_cfg, batch_num, ten_times);
	}

	//只扣货币
	if(type == 2) {
		int ret = consume_money_in_dragon_vale(dragon_evolution_cfg->money_type,
				dragon_evolution_cfg->money_num * batch_num,
				MONEY_SUB_MONEY_BATCH_EVOLUTION_DRAGON);
		if(ret != 0) {
			return ret;
		}
		return process_batch_evolution_dragon(*dragon_evolution_cfg, batch_num, ten_times);
	}

	//道具扣x个，再扣货币
	if(type == 3) {
		const int pack_item_amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,
				dragon_evolution_cfg->item_id);
		if(pack_item_amount <= 0) {
			return ERROR_ITEM_NOT_ENOUGH;
		}

		if(pack_item_amount >= (int)dragon_evolution_cfg->item_num * batch_num) {
			//只扣道具
			int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
					 Id_Amount(dragon_evolution_cfg->item_id, dragon_evolution_cfg->item_num * batch_num),
					 Lose_Item_DM_Info(ITEM_LOSE_BATCH_EVOLUTION_DRAGON));
			if (0 != result) {
				MSG_USER("error :sub item failed, item_id:%ld, item_num:%ld\n",
						dragon_evolution_cfg->item_id, dragon_evolution_cfg->item_num * batch_num);
				return ERROR_SUB_ITEM_FAIL;
			}
		}
		else {
			int num = pack_item_amount / (int)dragon_evolution_cfg->item_num;
			if(num < 0) {
				return ERROR_ITEM_COUNT_INVALID;
			}
			if(num >= 0 && num < batch_num) {
				int ret = check_money_is_engough(dragon_evolution_cfg->money_type,
						dragon_evolution_cfg->money_num * (batch_num - num),
						MONEY_SUB_MONEY_BATCH_EVOLUTION_DRAGON);
				if(ret != 0) {
					return ret;
				}
			}

			//道具扣x个，再扣货币
			int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
					 Id_Amount(dragon_evolution_cfg->item_id, dragon_evolution_cfg->item_num * num),
					 Lose_Item_DM_Info(ITEM_LOSE_BATCH_EVOLUTION_DRAGON));
			if (0 != result) {
				MSG_USER("error :sub item failed, item_id:%ld, item_num:%ld\n",
						dragon_evolution_cfg->item_id, dragon_evolution_cfg->item_num * num);
				return ERROR_SUB_ITEM_FAIL;
			}

			result = consume_money_in_dragon_vale(dragon_evolution_cfg->money_type,
					dragon_evolution_cfg->money_num * batch_num,
					MONEY_SUB_MONEY_BATCH_EVOLUTION_DRAGON);
			if(result != 0) {
				return result;
			}
		}//end else

		return process_batch_evolution_dragon(*dragon_evolution_cfg, batch_num, ten_times);
	}

	return ERROR_BATCH_EVOLUTION_DRAGON_CLIENT_TYPE_INVALID;
}

int Dragon_Vale::req_batch_evolution_dragon_type_0(const Dragon_Evolution_Cfg &dragon_evolution_cfg) {
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	uint16_t batch_num = 0;//批量进化总次数
	uint16_t ten_times = 0;//获得10倍能量次数
	uint32_t energy = 0;   //获得总能量

	for(uint16_t i = 0; i < dragon_base_cfg.max_batch_evolution_num; ++i) {
		srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
		uint8_t chance = random() % 100;
		if(chance < dragon_base_cfg.chance) {
			energy += dragon_base_cfg.energy * 10;
			ten_times += 1;
		}
		else {
			energy += dragon_base_cfg.energy;
		}

		batch_num += 1;

		if(energy >= dragon_evolution_cfg.total_energy) {
			break;
		}
	}//end for

	const int pack_item_amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,
			dragon_evolution_cfg.item_id);
	//道具足够
	if(pack_item_amount >= (int)dragon_evolution_cfg.item_num * batch_num) {
		MSG_82300034 msg;
		msg.batch_num = batch_num;
		msg.ten_times = ten_times;
		msg.type = 1;
		msg.item_id = dragon_evolution_cfg.item_id;
		msg.item_num = (uint16_t)(dragon_evolution_cfg.item_num * batch_num);
		msg.money_type = 0;
		msg.money_num = 0;
		THIS_SEND_TO_CLIENT(msg);
		return 0;
	}

	//没有道具
	if(pack_item_amount == 0) {
		int ret = check_money_is_engough(dragon_evolution_cfg.money_type,
				dragon_evolution_cfg.money_num * batch_num,
				MONEY_SUB_MONEY_BATCH_EVOLUTION_DRAGON);
		if(ret != 0) {
			return ret;
		}

		MSG_82300034 msg;
		msg.batch_num = batch_num;
		msg.ten_times = ten_times;
		msg.type = 2;
		msg.item_id = 0;
		msg.item_num = 0;
		msg.money_type = dragon_evolution_cfg.money_type;
		msg.money_num = dragon_evolution_cfg.money_num * batch_num;
		THIS_SEND_TO_CLIENT(msg);
		return 0;
	}//end if

	//道具有一部分，但是不足
	if(pack_item_amount > 0 && pack_item_amount < (int)dragon_evolution_cfg.item_num * batch_num) {
		int num = pack_item_amount / (int)dragon_evolution_cfg.item_num;
		if(num >= 0 && num < batch_num) {
			int ret = check_money_is_engough(dragon_evolution_cfg.money_type,
					dragon_evolution_cfg.money_num * (batch_num - num),
					MONEY_SUB_MONEY_BATCH_EVOLUTION_DRAGON);
			if(ret != 0) {
				return ret;
			}

			MSG_82300034 msg;
			msg.batch_num = batch_num;
			msg.ten_times = ten_times;
			msg.type = (num != 0 ? 3 : 2);
			msg.item_id = (num != 0 ? dragon_evolution_cfg.item_id : 0);
			msg.item_num = num;
			msg.money_type = dragon_evolution_cfg.money_type;
			msg.money_num = dragon_evolution_cfg.money_num * (batch_num - num);
			THIS_SEND_TO_CLIENT(msg);
		}
		return 0;
	}//end if

	return ERROR_ITEM_COUNT_INVALID;
}

int Dragon_Vale::process_batch_evolution_dragon(const Dragon_Evolution_Cfg &dragon_evolution_cfg,
						const uint16_t batch_num, const uint16_t ten_times) {
	const Dragon_Base_Cfg& dragon_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Base_Cfg();
	if(dragon_vale_detail_->dragon_info.progress >= dragon_base_cfg.max_evolution) {
		return ERROR_DRAGON_EVOLUTION_MAX;
	}

	if(batch_num < ten_times) {
		return ERROR_BATCH_EVOLUTION_DRAOGN_TIMES_INVALID;
	}

	uint32_t energy = dragon_base_cfg.energy * 10 * ten_times +  dragon_base_cfg.energy * (batch_num - ten_times);

	//进化获得能量
	dragon_vale_detail_->dragon_info.energy += energy;

	MSG_50180020 msg;

	//没有进化出新的龙
	if(dragon_vale_detail_->dragon_info.energy < dragon_evolution_cfg.total_energy) {
		msg.batch_num = batch_num;
		msg.ten_times = ten_times;
		msg.get_energy = energy;
		msg.energy = dragon_vale_detail_->dragon_info.energy;
		msg.is_new_dragon = 0;
		msg.dragon_info.progress = dragon_vale_detail_->dragon_info.progress;
		msg.dragon_info.feeling = dragon_vale_detail_->dragon_info.feeling;
		msg.dragon_info.power = dragon_vale_detail_->dragon_info.power;
		msg.dragon_info.armor = dragon_vale_detail_->dragon_info.armor;
		msg.dragon_info.stamina = dragon_vale_detail_->dragon_info.stamina;
		msg.dragon_info.power_level = dragon_vale_detail_->dragon_info.power_level;
		msg.dragon_info.armor_level = dragon_vale_detail_->dragon_info.armor_level;
		msg.dragon_info.stamina_level = dragon_vale_detail_->dragon_info.stamina_level;
		msg.dragon_info.hero_power = dragon_vale_detail_->dragon_info.hero_power;
		msg.dragon_info.hero_armor = dragon_vale_detail_->dragon_info.hero_armor;
		msg.dragon_info.hero_stamina = dragon_vale_detail_->dragon_info.hero_stamina;
		msg.dragon_info.hero_power_level = dragon_vale_detail_->dragon_info.hero_power_level;
		msg.dragon_info.hero_armor_level = dragon_vale_detail_->dragon_info.hero_armor_level;
		msg.dragon_info.hero_stamina_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
		msg.dragon_info.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
		msg.dragon_info.energy = dragon_vale_detail_->dragon_info.energy;
		msg.dragon_info.crit = dragon_vale_detail_->dragon_info.crit;
		msg.dragon_info.prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;
		msg.dragon_info.crit_level = dragon_vale_detail_->dragon_info.crit_level;
		msg.dragon_info.prevent_crit_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
		msg.dragon_info.hero_crit = dragon_vale_detail_->dragon_info.hero_crit;
		msg.dragon_info.hero_prevent_crit = dragon_vale_detail_->dragon_info.hero_prevent_crit;
		msg.dragon_info.hero_crit_level = dragon_vale_detail_->dragon_info.hero_crit_level;
		msg.dragon_info.hero_prevent_crit_level = dragon_vale_detail_->dragon_info.hero_prevent_crit_level;

		THIS_SEND_TO_CLIENT(msg);

		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();
		return 0;
	}

	//进化出新的龙,进度加1，能量值重置为0
	dragon_vale_detail_->dragon_info.progress += 1;
	dragon_vale_detail_->dragon_info.energy = 0;

	uint32_t last_power = dragon_vale_detail_->dragon_info.power;
	uint32_t last_armor = dragon_vale_detail_->dragon_info.armor;
	uint32_t last_stamina = dragon_vale_detail_->dragon_info.stamina;
	uint32_t last_crit = dragon_vale_detail_->dragon_info.crit;
	uint32_t last_prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;

	//进化出新的龙,属性提升
	double temp_property = 0.0;
	const Dragon_Keep_Cfg *dragon_keep_cfg = 0;
	//主角斗力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_POWER,
									dragon_vale_detail_->dragon_info.power_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_power * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.power = (uint32_t)temp_property;
	}

	//主角护甲属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_ARMOR,
									dragon_vale_detail_->dragon_info.armor_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_armor * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.armor = (uint32_t)temp_property;
	}

	//主角耐力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_STAMINA,
									dragon_vale_detail_->dragon_info.stamina_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.stamina = (uint32_t)temp_property;
	}

	////////////////////////////////////////
	//英雄斗力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_POWER,
									dragon_vale_detail_->dragon_info.hero_power_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_power * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_power = (uint32_t)temp_property;
	}

	//英雄护甲属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_ARMOR,
									dragon_vale_detail_->dragon_info.hero_armor_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_armor * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_armor = (uint32_t)temp_property;
	}

	//英雄耐力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_STAMINA,
									dragon_vale_detail_->dragon_info.hero_stamina_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_stamina = (uint32_t)temp_property;
	}

	//初始化主角暴击属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_CRIT,
									dragon_vale_detail_->dragon_info.crit_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.crit = (uint32_t)temp_property;
	}

	//初始化主角抗暴属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_PREVENT_CRIT,
									dragon_vale_detail_->dragon_info.prevent_crit_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.prevent_crit = (uint32_t)temp_property;
	}

	//初始化英雄暴击属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_CRIT,
									dragon_vale_detail_->dragon_info.hero_crit_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_crit = (uint32_t)temp_property;
	}

	//初始化英雄抗暴
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_PREVENT_CRIT,
									dragon_vale_detail_->dragon_info.hero_prevent_crit_level);
	if(dragon_keep_cfg) {
		temp_property = dragon_evolution_cfg.base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_prevent_crit = (uint32_t)temp_property;
	}


	msg.batch_num = batch_num;
	msg.ten_times = ten_times;
	msg.get_energy = energy;
	msg.energy = dragon_vale_detail_->dragon_info.energy;
	msg.is_new_dragon = 1;
	msg.dragon_info.progress = dragon_vale_detail_->dragon_info.progress;
	msg.dragon_info.feeling = dragon_vale_detail_->dragon_info.feeling;
	msg.dragon_info.power = dragon_vale_detail_->dragon_info.power;
	msg.dragon_info.armor = dragon_vale_detail_->dragon_info.armor;
	msg.dragon_info.stamina = dragon_vale_detail_->dragon_info.stamina;
	msg.dragon_info.power_level = dragon_vale_detail_->dragon_info.power_level;
	msg.dragon_info.armor_level = dragon_vale_detail_->dragon_info.armor_level;
	msg.dragon_info.stamina_level = dragon_vale_detail_->dragon_info.stamina_level;
	msg.dragon_info.hero_power = dragon_vale_detail_->dragon_info.hero_power;
	msg.dragon_info.hero_armor = dragon_vale_detail_->dragon_info.hero_armor;
	msg.dragon_info.hero_stamina = dragon_vale_detail_->dragon_info.hero_stamina;
	msg.dragon_info.hero_power_level = dragon_vale_detail_->dragon_info.hero_power_level;
	msg.dragon_info.hero_armor_level = dragon_vale_detail_->dragon_info.hero_armor_level;
	msg.dragon_info.hero_stamina_level = dragon_vale_detail_->dragon_info.hero_stamina_level;
	msg.dragon_info.is_play_dragon = dragon_vale_detail_->dragon_info.is_play_dragon ? 1 : 0;
	msg.dragon_info.energy = dragon_vale_detail_->dragon_info.energy;
	msg.dragon_info.crit = dragon_vale_detail_->dragon_info.crit;
	msg.dragon_info.prevent_crit = dragon_vale_detail_->dragon_info.prevent_crit;
	msg.dragon_info.crit_level = dragon_vale_detail_->dragon_info.crit_level;
	msg.dragon_info.prevent_crit_level = dragon_vale_detail_->dragon_info.prevent_crit_level;
	msg.dragon_info.hero_crit = dragon_vale_detail_->dragon_info.hero_crit;
	msg.dragon_info.hero_prevent_crit = dragon_vale_detail_->dragon_info.hero_prevent_crit;
	msg.dragon_info.hero_crit_level = dragon_vale_detail_->dragon_info.hero_crit_level;
	msg.dragon_info.hero_prevent_crit_level = dragon_vale_detail_->dragon_info.hero_prevent_crit_level;

	//boradcast to dragon vale scene
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();

	Block_Buffer broad_cast_buf;
	broad_cast_buf.make_message(INNER_BORADCAST_DRAGON_VALE);
	broad_cast_buf.write_int64(player_self()->role_id());
	broad_cast_buf.copy(&buf);
	broad_cast_buf.finish_message();
    const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	LOGIC_MONITOR->send_to_scene(main_base_cfg.dragon_vale_scene_id , 0, broad_cast_buf);

	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();

	Prop_Setter_Vec prop_vec;
	Prop_Setter setter;
	if(dragon_vale_detail_->dragon_info.power > last_power) {
		setter.reset();
		setter.prop_type = PT_POWER;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.power - last_power);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.armor > last_armor) {
		setter.reset();
		setter.prop_type = PT_ARMOR;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.armor - last_armor);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.stamina > last_stamina) {
		setter.reset();
		setter.prop_type = PT_STAMINA;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.stamina - last_stamina);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.crit > last_crit) {
		setter.reset();
		setter.prop_type = PT_CRIT;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.crit - last_crit);
		prop_vec.push_back(setter);
	}

	if(dragon_vale_detail_->dragon_info.prevent_crit > last_prevent_crit) {
		setter.reset();
		setter.prop_type = PT_CRIT_DEF;
		setter.addi_type = AT_DRAGON_VALE;
		setter.operate = O_ADD;
		setter.fixed = (double)(dragon_vale_detail_->dragon_info.prevent_crit - last_prevent_crit);
		prop_vec.push_back(setter);
	}

	if (prop_vec.size() > 0) player_self()->fight_prop_modify(prop_vec, 211);

	player_self()->heroer_prop_refresh();

	//系统公告
	player_self()->create_ann_dragon_evolution_success(player_self()->role_id(), dragon_evolution_cfg.dragon_name);
	player_self()->achievement_listen_dragon_wale_keep_level(dragon_vale_detail_->dragon_info.progress);
	return 0;
}

uint16_t Dragon_Vale::play_dragon_get_heart_num_by_chance() {
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	int num = random() % 100;
	const Dragon_All_Cfg &dragon_all_cfg = CONFIG_CACHE_DRAGON_VALE->get_dragon_config();

	uint8_t last_total_border = 0;
	for(Dragon_Play_Dragon_Get_Heart_Cfg_Map::const_iterator it
				= dragon_all_cfg.dragon_play_dragon_get_heart_cfg_map.begin();
	    it != dragon_all_cfg.dragon_play_dragon_get_heart_cfg_map.end();
	    ++it) {
		if(num >= last_total_border && num <= last_total_border + it->second.chance - 1) {
			return it->first;
		}

		last_total_border += it->second.chance;
	}

	return 0;
}

void Dragon_Vale::dragon_vale_sync_player_level_change_to_db_view(int new_level) {
	DB_View::ID_Player_View_Map::iterator find_it =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(player_self()->role_id());
	if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	if(!find_it->second) {
		return;
	}

	find_it->second->level = new_level;
}

void Dragon_Vale::process_robber_win_in_rob_fight() {
	dragon_vale_detail_->is_in_robbed_dragon_vale = false;
	save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}

int Dragon_Vale::req_dragon_building_gain_production_left_time(uint32_t id, uint8_t type) {
	uint64_t key = create_key_of_Dragon_Building_Info(id, type);
	DragonBuildingInfoMap::const_iterator find_it = dragon_vale_detail_->dragon_building_info_map.find(key);
	if(find_it == dragon_vale_detail_->dragon_building_info_map.end()) {
		return ERROR_BUILDING_NOT_EXIST;
	}

	if(find_it->second.status == BUILDING_UPGRADE_DOING) {
		return 0;
	}

	const Building_Level_Cfg *building_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Level_Cfg(type, find_it->second.level);
	if(!building_level_cfg) {
		return ERROR_BUILDING_LEVEL_CFG_NOT_EXIST;
	}

	if(find_it->second.production >= building_level_cfg->min_gain_limit) {
		MSG_50180021 msg;
		msg.id = id;
		msg.type = type;
		msg.left_time = 0;
		THIS_SEND_TO_CLIENT(msg);
		return 0;
	}

	Time_Value now = Time_Value::gettimeofday();
	long total_time = find_it->second.last_output_production_time.sec() + Time_Value::ONE_MINUTE_IN_SECS;
	uint32_t left_time = 0;
	if(total_time > now.sec()) {
		left_time = (uint32_t)(total_time - now.sec());
	}
	else {
		left_time = 0;
	}

	MSG_50180021 msg;
	msg.id = id;
	msg.type = type;
	msg.left_time = (building_level_cfg->min_gain_limit - find_it->second.production - 1) / building_level_cfg->production_per_min * 60
				+ left_time;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

uint16_t Dragon_Vale::get_dragon_building_num_by_type(uint8_t type) {
	uint16_t num = 0;
	for(DragonBuildingInfoMap::const_iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
			it != dragon_vale_detail_->dragon_building_info_map.end();
			++it) {
		if(it->second.type == type) {
			++num;
		}
	}
	return num;
}

uint16_t Dragon_Vale::get_dragon_building_total_num() {
	uint16_t num = 0;
	for(DragonBuildingInfoMap::const_iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
			it != dragon_vale_detail_->dragon_building_info_map.end();
			++it) {
		++num;
	}
	return num;
}

void Dragon_Vale::notify_has_can_create_dragon_building_num() {
	MSG_82300038 msg_82300038;
	uint16_t total_num = 0;
	uint16_t num = 0;
	const Role_Level_Cfg *role_level_cfg =  0;
	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_GOLD_MINE);
	if(role_level_cfg) {
		num = get_dragon_building_num_by_type(BUILDING_GOLD_MINE);
		if(num < role_level_cfg->max_building_num) {
			total_num += (role_level_cfg->max_building_num - num);
		}
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_MOON_WELL);
	if(role_level_cfg) {
		num = get_dragon_building_num_by_type(BUILDING_MOON_WELL);
		if(num < role_level_cfg->max_building_num) {
			total_num += (role_level_cfg->max_building_num - num);
		}
	}

	role_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Role_Level_Cfg(player_self()->level(), BUILDING_TOTEM);
	if(role_level_cfg) {
		num = get_dragon_building_num_by_type(BUILDING_TOTEM);
		if(num < role_level_cfg->max_building_num) {
			total_num += (role_level_cfg->max_building_num - num);
		}
	}

	if(total_num > 0) {
		msg_82300038.num = total_num;
		THIS_SEND_TO_CLIENT(msg_82300038);
	}
}

void Dragon_Vale::process_robber_gain_total_production(uint32_t gold_mine, uint32_t moon_well, uint32_t totem) {
	player_self()->listen_vip_reward(VIP_WORLD_TREE_ROB_DRAGON, COPPER, gold_mine);
	player_self()->listen_vip_reward(VIP_WORLD_TREE_ROB_DRAGON, SOULS, moon_well);
	player_self()->listen_vip_reward(VIP_WORLD_TREE_ROB_DRAGON, DRAGON_SOUL, totem);
}

void Dragon_Vale::refresh_dragon_vale_op_by_step_foot(const int64_t target_id) {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();

	if(player_self()->role_id() == target_id) {
		return;
	}

	if(!dragon_vale_detail_->target_player_id) {
		return;
	}

	DB_View::ID_Player_View_Map::const_iterator target_it  =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(target_id);
	if(target_it == LOGIC_MONITOR->db_view()->id_player_view_map.end() || !target_it->second) {
		return;
	}

	uint8_t op = 0;
	if(!target_it->second->dragon_vale_view.thief_id_quality_map.empty()) {
		op  = 3;
	}

	if(target_it->second->dragon_vale_view.dragon_info.is_play_dragon) {
		op = 2;
	}

	if(dragon_vale_detail_->foot_id_set.find(target_id) == dragon_vale_detail_->foot_id_set.end()
			&& dragon_vale_detail_->award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
		op = 1;
	}

	MSG_82300037 msg;
	msg.target_id = target_id;
	msg.op = op;
	THIS_SEND_TO_CLIENT(msg);

//	req_get_dragon_vale_friend_list();
//	req_get_dranon_vale_gang_member_list();

	const Friend_Info_Map &freind_list_map = player_self()->friend_detail().firend_list_;

	DB_View::ID_Player_View_Map::const_iterator find_it  = LOGIC_MONITOR->db_view()->id_player_view_map.end();

	for(Friend_Info_Map::const_iterator it = freind_list_map.begin();
			it != freind_list_map.end(); ++it) {
		find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->second.role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end() || !find_it->second) {
			continue;
		}

		if(find_it->second->dragon_vale_view.dragon_building_info_map.empty()
				&& !find_it->second->dragon_vale_view.is_dragon_born) {
			continue;
		}

		if(player_self()->role_id() == it->second.role_id) {
			continue;
		}

		uint8_t op = 0;
		if(!find_it->second->dragon_vale_view.thief_id_quality_map.empty()) {
			op  = 3;
		}

		if(find_it->second->dragon_vale_view.dragon_info.is_play_dragon) {
			op = 2;
		}

		if(dragon_vale_detail_->foot_id_set.find(find_it->first) == dragon_vale_detail_->foot_id_set.end()
				&& dragon_vale_detail_->award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
			op = 1;
		}

		MSG_82300037 msg;
		msg.target_id = find_it->first;
		msg.op = op;
		THIS_SEND_TO_CLIENT(msg);
	}//end for

	const Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(player_self()->gang_id());
	if (!gang_view) {
		THIS_SEND_TO_CLIENT(msg);
		return;
	}

	for (Gang_DB_View::Member_Map::const_iterator it = gang_view->member_map.begin();
			it != gang_view->member_map.end(); ++it) {
		find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->second.role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end() || !find_it->second) {
			continue;
		}

		if(find_it->second->dragon_vale_view.dragon_building_info_map.empty()
				&& !find_it->second->dragon_vale_view.is_dragon_born) {
			continue;
		}

		if(player_self()->role_id() == it->second.role_id) {
			continue;
		}

		uint8_t op = 0;
		if(!find_it->second->dragon_vale_view.thief_id_quality_map.empty()) {
			op  = 3;
		}

		if(find_it->second->dragon_vale_view.dragon_info.is_play_dragon) {
			op = 2;
		}

		if(dragon_vale_detail_->foot_id_set.find(find_it->first) == dragon_vale_detail_->foot_id_set.end()
				&& dragon_vale_detail_->award_foot_num < main_base_cfg.step_foot_max_num_per_day) {
			op = 1;
		}

		MSG_82300037 msg;
		msg.target_id = find_it->first;
		msg.op = op;
		THIS_SEND_TO_CLIENT(msg);
	}//end for
}

void Dragon_Vale::process_robber_begin_attck() {
	const Main_Base_Cfg& main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	dragon_vale_detail_->matched_num += 1;

	if(dragon_vale_detail_->matched_num > main_base_cfg.border_match_num_of_max_match_coin) {
		dragon_vale_detail_->matched_num = main_base_cfg.border_match_num_of_max_match_coin;
	}

	uint32_t match_num = 0;
	if(dragon_vale_detail_->matched_num >= main_base_cfg.border_match_num_of_max_match_coin) {
		match_num = main_base_cfg.border_match_num_of_max_match_coin;
	}
	else {
		match_num = dragon_vale_detail_->matched_num;
	}

	const uint32_t *coin = CONFIG_CACHE_DRAGON_VALE->find_main_match_coin(match_num);
	if(!coin) {
		MSG_USER("cfg not exist");
		this->save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();
		return;
	}

	MSG_82300040 msg_82300040;
	msg_82300040.coin = *coin;
	THIS_SEND_TO_CLIENT(msg_82300040);

	player_self()->active_content_listen_dragon_prey_num();
}

void Dragon_Vale::dragon_vale_listen_icon_open(const uint16_t icon_id) {
	if(ICON_ID_DRAGON_VALE == icon_id) {
		dragon_vale_detail_->is_dragon_vale_open = true;
		save_dragon_vale_tick();
		sync_dragon_vale_to_db_view();
	}
}

int Dragon_Vale::req_get_invite_friend_step_foot_list() {
	Logic_Player *player = 0;
	MSG_50180022 msg;
	const Friend_Info_Map &freind_list_map = player_self()->friend_detail().firend_list_;

	for(Friend_Info_Map::const_iterator it = freind_list_map.begin();
			it != freind_list_map.end(); ++it) {
		if(player_self()->role_id() == it->second.role_id) {
			continue;
		}

		player = LOGIC_MONITOR->find_logic_player_by_role_id(it->second.role_id);
		if(!player) {
			continue;
		}

		if(!player->dragon_vale_detail().is_dragon_vale_open) {
			continue;
		}

		if(player->dragon_vale_detail().foot_id_set.find(player_self()->role_id())
				!= player->dragon_vale_detail().foot_id_set.end()) {
			continue;
		}

		Dragon_Vale_Invite_Step_Foot_Friend dragon_vale_invite_step_foot_friend;
		dragon_vale_invite_step_foot_friend.role_id = player->role_id();
		dragon_vale_invite_step_foot_friend.role_name = player->role_name();
		msg.friend_list.push_back(dragon_vale_invite_step_foot_friend);
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Dragon_Vale::req_invite_friend_step_foot(int64_t role_id) {
	player_self()->task_listen_invite_friend_step_foot();

	if(player_self()->role_id() == role_id) {
		return 0;
	}

	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) {
	    MSG_50180023 msg_50180023;
	    msg_50180023.error_code = ERROR_INVITED_FRIEND_IS_OFFLINE;
		Block_Buffer buf_50180023;
		buf_50180023.make_message(msg_50180023.msg_id);
	    msg_50180023.serialize(buf_50180023);
	    buf_50180023.finish_message();
	    player_self()->send_to_client(buf_50180023);

	    req_get_invite_friend_step_foot_list();
		return 0;
	}

	if(!player->dragon_vale_detail().is_dragon_vale_open) {
		return 0;
	}

	if(player->dragon_vale_detail().foot_id_set.find(player_self()->role_id())
			!= player->dragon_vale_detail().foot_id_set.end()) {
	    MSG_50180023 msg_50180023;
	    msg_50180023.error_code = ERROR_INVITED_FRIEND_STEP_FOOT_ALREADY;
		Block_Buffer buf_50180023;
		buf_50180023.make_message(msg_50180023.msg_id);
	    msg_50180023.serialize(buf_50180023);
	    buf_50180023.finish_message();
	    player_self()->send_to_client(buf_50180023);

	    req_get_invite_friend_step_foot_list();
		return 0;
	}

    MSG_50180023 msg_50180023;
    msg_50180023.error_code = 0;
	Block_Buffer buf_50180023;
	buf_50180023.make_message(msg_50180023.msg_id);
    msg_50180023.serialize(buf_50180023);
    buf_50180023.finish_message();
    player_self()->send_to_client(buf_50180023);

	MSG_82300043 msg;
	msg.role_id = player_self()->role_id();
	msg.role_name = player_self()->role_name();
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
    msg.serialize(buf);
    buf.finish_message();
    player->send_to_client(buf);
	return 0;
}

int Dragon_Vale::req_invite_all_friend_step_foot() {
	player_self()->task_listen_invite_friend_step_foot();

	Logic_Player *player = 0;
	const Friend_Info_Map &freind_list_map = player_self()->friend_detail().firend_list_;

	bool is_invite_success = false;

	for(Friend_Info_Map::const_iterator it = freind_list_map.begin();
			it != freind_list_map.end(); ++it) {
		if(player_self()->role_id() == it->second.role_id) {
			continue;
		}

		player = LOGIC_MONITOR->find_logic_player_by_role_id(it->second.role_id);
		if(!player) {
			continue;
		}

		if(!player->dragon_vale_detail().is_dragon_vale_open) {
			continue;
		}

		if(player->dragon_vale_detail().foot_id_set.find(player_self()->role_id())
				!= player->dragon_vale_detail().foot_id_set.end()) {
			continue;
		}

		MSG_82300043 msg;
		msg.role_id = player_self()->role_id();
		msg.role_name = player_self()->role_name();
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
	    msg.serialize(buf);
	    buf.finish_message();
	    player->send_to_client(buf);

	    is_invite_success = true;
	}

	if(is_invite_success) {
		MSG_50180024 msg;
		THIS_SEND_TO_CLIENT(msg);
	}

	return 0;
}

int Dragon_Vale::listen_lvl_when_module_init(void) {
	int gold_lvl = 0;
	int moon_lvl = 0;
	int soul_lvl = 0;
	for(DragonBuildingInfoMap::iterator it = dragon_vale_detail_->dragon_building_info_map.begin();
					it != dragon_vale_detail_->dragon_building_info_map.end();
					++it) {
		//不是升级状态，不处理
		if(BUILDING_UPGRADE_FINISH != it->second.status) {
			continue;
		}
		switch(it->second.type) {
		case BUILDING_GOLD_MINE: {//金矿
			if (gold_lvl < it->second.level) {
				gold_lvl = it->second.level;
			}
			break;
		}
		case BUILDING_MOON_WELL: {//月亮井
			if (moon_lvl < it->second.level) {
				moon_lvl = it->second.level;
			}
			break;
		}
		case BUILDING_TOTEM: {//图腾
			if (soul_lvl < it->second.level) {
				soul_lvl = it->second.level;
			}
			break;
		}
		default:
			break;
		}
	}
	if (gold_lvl > 0) {
		player_self()->achievement_listen_dragon_wale_gold_mine_lvl(gold_lvl);
	}
	if (moon_lvl > 0) {
		player_self()->achievement_listen_dragon_wale_moon_well_lvl(moon_lvl);
	}
	if (soul_lvl > 0) {
		player_self()->achievement_listen_dragon_wale_totem_lvl(soul_lvl);
	}
	return 0;
}

void Dragon_Vale::correct_building_queue() {
	if(player_self()->vip() <= (uint32_t)0) {
		return;
	}

	const Vip_Level_Cfg *vip_normal_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Vip_Level_Cfg(0);
	if(!vip_normal_cfg) {
		MSG_USER("vip_level_cfg not exist, vip_level=0");
		return;
	}

	const Vip_Level_Cfg *vip_new_level_cfg = CONFIG_CACHE_DRAGON_VALE->find_Building_Vip_Level_Cfg(player_self()->vip());
	if(!vip_new_level_cfg) {
		MSG_USER("vip_level_cfg not exist, vip_level=%d", player_self()->vip());
		return;
	}

	if(vip_new_level_cfg->max_queue_num > dragon_vale_detail_->all_open_build_queue_num) {
		dragon_vale_detail_->build_queue_num +=
				(vip_new_level_cfg->max_queue_num - dragon_vale_detail_->all_open_build_queue_num);
		dragon_vale_detail_->all_open_build_queue_num = vip_new_level_cfg->max_queue_num;
		this->save_dragon_vale_tick();
		this->sync_dragon_vale_to_db_view();
	}
}

void Dragon_Vale::correct_dragon_evolution_20150129() {
	if(dragon_vale_detail_->dragon_info.progress <= 1) {
		return;
	}

	const Dragon_Evolution_Cfg *normal_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Evolution_Cfg(1);
	if(!normal_cfg) {
		return;
	}

	double temp_property = 0.0;
	const Dragon_Keep_Cfg *dragon_keep_cfg = 0;
	//主角斗力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_POWER,
									dragon_vale_detail_->dragon_info.power_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.power = (uint32_t)temp_property;
	}

	//主角护甲属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_ARMOR,
									dragon_vale_detail_->dragon_info.armor_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.armor = (uint32_t)temp_property;
	}

	//主角耐力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_STAMINA,
									dragon_vale_detail_->dragon_info.stamina_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.stamina = (uint32_t)temp_property;
	}

	////////////////////////////////////////
	//英雄斗力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_POWER,
									dragon_vale_detail_->dragon_info.hero_power_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_power * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_power = (uint32_t)temp_property;
	}

	//英雄护甲属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_ARMOR,
									dragon_vale_detail_->dragon_info.hero_armor_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_armor * (100 + dragon_keep_cfg->add_property) / 100;
		dragon_vale_detail_->dragon_info.hero_armor = (uint32_t)temp_property;
	}

	//英雄耐力属性
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_STAMINA,
									dragon_vale_detail_->dragon_info.hero_stamina_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_stamina * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_stamina = (uint32_t)temp_property;
	}

	//主角暴击
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_CRIT,
									dragon_vale_detail_->dragon_info.crit_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.crit = (uint32_t)temp_property;
	}

	//主角抗暴
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_PREVENT_CRIT,
									dragon_vale_detail_->dragon_info.prevent_crit_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.prevent_crit = (uint32_t)temp_property;
	}

	//英雄暴击
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_CRIT,
									dragon_vale_detail_->dragon_info.hero_crit_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_crit = (uint32_t)temp_property;
	}

	//英雄抗暴
	dragon_keep_cfg = CONFIG_CACHE_DRAGON_VALE->find_Dragon_Keep_Cfg((uint8_t)KEEP_DRAGON_HERO_PREVENT_CRIT,
									dragon_vale_detail_->dragon_info.hero_crit_level);
	if(dragon_keep_cfg) {
		temp_property = normal_cfg->base_prevent_crit * (100 + dragon_keep_cfg->add_property) / 100;
			dragon_vale_detail_->dragon_info.hero_prevent_crit = (uint32_t)temp_property;
	}

	dragon_vale_detail_->dragon_info.progress = 1;
	this->save_dragon_vale_tick();
	sync_dragon_vale_to_db_view();
}
