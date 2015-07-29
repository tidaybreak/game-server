/*
 * Gang_Boss_Manager.cpp
 *
 *  Created on: 2015年2月7日
 *      Author: root
 */

#include "Gang_Boss_Manager.h"
#include "World_Boss_Manager.h"
#include "Pool_Manager.h"
#include "Scene_Player.h"

Gang_Boss_Manager::Gang_Boss_Manager(void){
	reset();
}

Gang_Boss_Manager::~Gang_Boss_Manager(void){

}

void Gang_Boss_Manager::reset(void){

}

World_Boss_Manager* Gang_Boss_Manager::world_boss_manager(role_id_t gang_id){
	World_Boss_Manager_Map::iterator it =  world_boss_manager_map_.find(gang_id);
	if(it != world_boss_manager_map_.end()){
		return it->second;
	}
	return NULL;
}


World_Boss_Manager* Gang_Boss_Manager::get_world_boss_manager(role_id_t gang_id){
	World_Boss_Manager_Map::iterator it =  world_boss_manager_map_.find(gang_id);
	if(it != world_boss_manager_map_.end()){
		return it->second;
	}
	World_Boss_Manager* wb_manager = POOL_MANAGER->pop_world_boss_manager();
	if(!wb_manager){
		return 0;
	}
	wb_manager->set_icon_id(19);
	world_boss_manager_map_[gang_id] = wb_manager;
	return wb_manager;
}

role_id_t Gang_Boss_Manager::get_gang_id(const role_id_t role_id){
	World_Boss_Manager_Map::iterator it;
	for(it = world_boss_manager_map_.begin(); it != world_boss_manager_map_.end(); ++it){
		World_Boss_Player_Info* player_info = it->second->get_player_info(role_id);
		if(player_info){
			return player_info->gang_id;
		}
	}
	return 0;
}

int Gang_Boss_Manager::req_world_boss_act_guild_bless(Scene_Player* player){
	if(!player){
		return -1;
	}
	role_id_t gang_id = player->gang_id();
	World_Boss_Manager* wb_manager = world_boss_manager(gang_id);
	if(!wb_manager){
		return -1;
	}
	return wb_manager->req_world_boss_act_guild_bless(player);
}

int Gang_Boss_Manager::req_world_boss_use_ability(Scene_Player* player, int type){
	if(!player){
		return -1;
	}
	role_id_t gang_id = player->gang_id();
	World_Boss_Manager* wb_manager = world_boss_manager(gang_id);
	if(!wb_manager){
		return -1;
	}
	return wb_manager->req_world_boss_use_ability(player, type);
}

int Gang_Boss_Manager::req_fight_world_boss(Scene_Player* player){
	if(!player){
		return -1;
	}
	role_id_t gang_id = player->gang_id();
	World_Boss_Manager* wb_manager = world_boss_manager(gang_id);
	if(!wb_manager){
		return -1;
	}
	return wb_manager->req_fight_world_boss(player);
}

int Gang_Boss_Manager::inner_player_enter(const role_id_t gang_id, Block_Buffer& buf){
	World_Boss_Manager* wb_manager = world_boss_manager(gang_id);
	if(!wb_manager){
		return -1;
	}
	return wb_manager->inner_player_enter(gang_id, buf);
}

int Gang_Boss_Manager::inner_sync_info(Scene_Player* player, Block_Buffer& buf){
	if(!player){
		return -1;
	}
	role_id_t gang_id = player->gang_id();
	World_Boss_Manager* wb_manager = world_boss_manager(gang_id);
	if(!wb_manager){
		return -1;
	}
	return wb_manager->inner_sync_info(player, buf);
}

int Gang_Boss_Manager::inner_act_start(const role_id_t gang_id, Block_Buffer& buf){
	World_Boss_Manager_Map::iterator it =  world_boss_manager_map_.find(gang_id);
	if(it != world_boss_manager_map_.end()){
		it->second->set_icon_id(19);
		return it->second->inner_act_start(gang_id, buf);;
	}
	World_Boss_Manager* wb_manager = POOL_MANAGER->pop_world_boss_manager();
	if(!wb_manager){
		return 0;
	}
	wb_manager->set_icon_id(19);
	world_boss_manager_map_[gang_id] = wb_manager;
	return wb_manager->inner_act_start(gang_id, buf);
}

int Gang_Boss_Manager::inner_act_end(const role_id_t gang_id, Block_Buffer& buf){
	World_Boss_Manager* wb_manager = world_boss_manager(gang_id);
	if(!wb_manager){
		return -1;
	}
	return wb_manager->inner_act_end(gang_id, buf);
}

void Gang_Boss_Manager::listen_gang_info_sync(const gang_id_t src_gang_id, Scene_Player* player){
	if(!player){
		return;
	}
	World_Boss_Manager* wb_manager = world_boss_manager(src_gang_id);
	if(!wb_manager){
		return;
	}
	return wb_manager->listen_gang_info_sync(src_gang_id, player);
}

void Gang_Boss_Manager::listen_vip_info_change(Scene_Player* player){
	if(!player){
		return;
	}
	role_id_t gang_id = player->gang_id();
	World_Boss_Manager* wb_manager = world_boss_manager(gang_id);
	if(!wb_manager){
		return;
	}
	return wb_manager->listen_vip_info_change(player);
}

