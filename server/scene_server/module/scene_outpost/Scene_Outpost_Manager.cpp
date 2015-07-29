/*
 * Scene_Outpost_Manager.cpp
 *
 *  Created on: 2015年4月8日
 *      Author: root
 */

#include "Scene_Outpost_Manager.h"
#include "Msg_Scene_Outpost_Struct.h"
#include "Scene_Player.h"
#include "Config_Cache_NPC.h"
#include "Move_Scene.h"
#include "tbay_scene/Tbay_Scene.h"
#include "NPC/NPC_Manager.h"
#include "Err_Code.h"
#include "Msg_Inner_Enum.h"

Scene_Outpost_Manager::Scene_Outpost_Manager(void){
	reset();
}

Scene_Outpost_Manager::~Scene_Outpost_Manager(void){

}

void Scene_Outpost_Manager::reset(void){
	vagrant_item_map_.clear();
}

int Scene_Outpost_Manager::add_new_monster_point(Move_Scene* scene, const int point, bool is_boss, Coord point_coord){
	vagrant_store_trigger(scene, point, is_boss, point_coord);
	tbay_trigger(scene, point, is_boss, point_coord);
	return 0;
}

int Scene_Outpost_Manager::clear_vagrant_store(const int scene_id){
	Vagrant_Store_Scene_Map::iterator it = vagrant_item_map_.find(scene_id);
	if(it != vagrant_item_map_.end()){
		vagrant_item_map_.erase(it);
	}
	return 0;
}

int Scene_Outpost_Manager::inner_sync_info(Scene_Player* player, Block_Buffer& buf){
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
		switch(sync_type){
		case 10:{// 购买流浪商人物品返回
			if(!player){
				return 0;
			}
			int scene_id = 0;
			int64_t monster_role_id = 0;
			int item_index = 0;
			int8_t result = 0;

			buf.read_int32(scene_id);
			buf.read_int64(monster_role_id);
			buf.read_int32(item_index);
			buf.read_int8(result);
			if(result == 1){
				Vagrant_Item_Info* item_info = get_vagrant_item_info(scene_id, monster_role_id, item_index);
				if(item_info){
					item_info->buy_item(player->role_id());
				}
				MSG_50211301 msg;
				msg.item_index = item_index;
				msg.result = 1;
				OBJ_SEND_TO_CLIENT(msg, (*player));
				req_vagrant_item_info(player, monster_role_id);
			}else{
				MSG_50211301 msg;
				msg.item_index = item_index;
				msg.result = 0;
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}
			return 0;
			break;
		}
		case 20:{
			if(!player){
				return 0;
			}
			int8_t bless_index = 0;
			int8_t result = 0;
			buf.read_int8(bless_index);
			buf.read_int8(result);
			if(result == 1){
				Move_Scene* scene = player->move_scene();
				if(scene){
					Tbay_Scene* tbay_scene = dynamic_cast<Tbay_Scene*>(scene);
					if(tbay_scene){
						tbay_scene->inner_tbay_bless(player, bless_index);
					}
				}
				MSG_50211312 msg;
				msg.bless_index = bless_index;
				msg.result = 1;
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}else{
				MSG_50211312 msg;
				msg.bless_index = bless_index;
				msg.result = 0;
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}
			return 0;
			break;
		}
		default:{
			return 0;
			break;
		}
	}
	return 0;
}

int Scene_Outpost_Manager::req_vagrant_item_info(Scene_Player* player, const int64_t monster_role_id){
	if(!player){
		return 0;
	}
	int scene_id = player->move_scene_id();

	MSG_50211300 msg;
	Vagrant_Store_Scene_Map::iterator it = vagrant_item_map_.find(scene_id);
	if(it != vagrant_item_map_.end()){
		Vagrant_Store_Map::iterator s_it = it->second.find(monster_role_id);
		if(s_it != it->second.end()){
			for(Vagrant_Item_Info_Vec::iterator t_it = s_it->second.begin(); t_it != s_it->second.end(); ++t_it){
				Secret_Item_Info info;
				info.item_id = t_it->cfg_key;
				info.remain_num = t_it->cfg_num - t_it->buy_num;
				info.buy_num = t_it->player_buy_num(player->role_id());
				msg.items.push_back(info);
			}
		}

	}
	OBJ_SEND_TO_CLIENT(msg, (*player));
	return 0;
}

int Scene_Outpost_Manager::req_vagrant_buy_item(Scene_Player* player, const int64_t monster_role_id, const int item_index){
	if(!player){
		return 0;
	}
	int scene_id = player->move_scene_id();
	Vagrant_Item_Info* item_info = get_vagrant_item_info(scene_id, monster_role_id, item_index);
	if(!item_info){
		return -1;
	}
	// 购买数量检测
	if(item_info->cfg_num != 0 &&
			item_info->buy_num >= item_info->cfg_num){
		//
		MSG_50211301 msg;
		msg.item_index = item_index;
		msg.result = 0;
		OBJ_SEND_TO_CLIENT(msg, (*player));
		//
		req_vagrant_item_info(player, monster_role_id);
		return ERROR_SECRET_SHOP_NUM_LIMIT;
	}
	// 限购检测
	if(item_info->cfg_limit_num != 0 &&
			item_info->player_buy_num(player->role_id()) >= item_info->cfg_limit_num){
		return ERROR_SECRET_SHOP_LIMIT_NUM_LIMIT;
	}
	{
		Block_Buffer buf;
		buf.make_message(INNER_SYNC_SCENE_OUTPOST);

		buf.write_int8(10);
		buf.write_int32(scene_id);
		buf.write_int64(monster_role_id);
		buf.write_int32(item_index);
		buf.write_int32(item_info->cfg_item_id);
		buf.write_int32(1);
		buf.write_int8(item_info->cfg_cost_type);
		buf.write_int32(item_info->cfg_cost);

		buf.finish_message();

		player->send_to_logic(buf);
	}
	return 0;
}

Vagrant_Item_Info* Scene_Outpost_Manager::get_vagrant_item_info(const int scene_id, const int64_t monster_role_id, const int item_index){
	Vagrant_Store_Scene_Map::iterator it = vagrant_item_map_.find(scene_id);
	if(it != vagrant_item_map_.end()){
		Vagrant_Store_Map::iterator s_it = it->second.find(monster_role_id);
		if(s_it != it->second.end()){
			for(Vagrant_Item_Info_Vec::iterator t_it = s_it->second.begin(); t_it != s_it->second.end(); ++t_it){
				if(t_it->cfg_key == item_index){
					return &(*t_it);
				}
			}
		}
	}
	return NULL;
}

int Scene_Outpost_Manager::vagrant_store_trigger(Move_Scene* scene, const int point, bool is_boss, Coord point_coord){
	int scene_id = scene->scene_id();
	int monster_id = CONFIG_CACHE_NPC->vagrant_item_trigger(scene_id, point);
	if(monster_id == 0){
		return 0;
	}
	// create monster
	NPC_Addition_Info npc_add;
	npc_add.birth_coord = point_coord;
	Monster* monster = NPC_MANAGER->create_monster(monster_id, 5, scene, NULL, npc_add);
	if(!monster){
		return 0;
	}
	int64_t monster_role_id = monster->role_id();
	// 生存流浪商店物品
	Vagrant_Item_Config_Map* vic_map = CONFIG_CACHE_NPC->get_vagrant_item_config_map();
	if(!vic_map){
		return 0;
	}
	Vagrant_Item_Info_Vec vi_vec;
	Vagrant_Item_Info vi;
	for(Vagrant_Item_Config_Map::iterator it = vic_map->begin(); it != vic_map->end();
			++ it){
		if(it->second.is_valid(monster_id)){
			vi.reset();
			vi.cfg_key = it->second.key;
			vi.cfg_item_id = it->second.item_id;
			vi.cfg_cost_type = it->second.sys_sell_type;
			vi.cfg_cost = it->second.cost;
			vi.cfg_num = it->second.num;
			vi.cfg_limit_num = it->second.limit_num;
			vi_vec.push_back(vi);
		}
	}
	Vagrant_Store_Scene_Map::iterator it = vagrant_item_map_.find(scene_id);
	if(it != vagrant_item_map_.end()){
		it->second[monster_role_id] = vi_vec;
	}else{
		Vagrant_Store_Map vagrant_stroe_map;
		vagrant_stroe_map[monster_role_id] = vi_vec;
		vagrant_item_map_[scene_id] = vagrant_stroe_map;
	}
	// 飘字
	{
		int err_id = ERROR_VAGRANT_STORE_OPEN;
		Block_Buffer buf;
		buf.make_message(80000001);
		buf.write_int32(err_id);
		buf.write_int32(0);
		buf.write_string("");
		buf.finish_message();
		scene->broadcast_all(0, buf);
	}
	return 0;
}

int Scene_Outpost_Manager::tbay_trigger(Move_Scene* scene, const int point, bool is_boss, Coord point_coord){
	int scene_id = scene->scene_id();
	int target_scene_id = 0;
	int target_scene_time = 0;
	int target_scene_type = 0;
	int monster_id = CONFIG_CACHE_NPC->tbay_trigger(scene_id, point, target_scene_id,
			target_scene_time, target_scene_type);
	if(monster_id == 0){
		return 0;
	}
	// create monster
	NPC_Addition_Info npc_add;
	npc_add.birth_coord = point_coord;
	Monster* monster = NPC_MANAGER->create_monster(monster_id, 5, scene, NULL, npc_add);
	if(!monster){
		return 0;
	}
	monster->set_ex_param(target_scene_id, target_scene_time, target_scene_type);
	return 0;
}


