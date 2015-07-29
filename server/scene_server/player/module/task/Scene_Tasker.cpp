/*
 * Scene_Tasker.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: Linqiyou
 */

#include "Scene_Tasker.h"
#include "task/Task_Def.h"
#include "Base_Struct.h"
#include "Scene_Player.h"

Scene_Tasker::Scene_Tasker() {
	this->__task_id_set.clear();
    this->__npc_ItemIdSet_map.unbind_all();
}

Scene_Tasker::~Scene_Tasker() {
}

/*void Scene_Tasker::load_detail(Block_Buffer &buf) {
	uint16_t size = 0;
	int32_t npc_type = 0;
	int64_t npc_id = 0;
	buf.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buf.read_int32(npc_type);
		buf.read_int64(npc_id);
		this->npc_type_.insert(Int_Uid::value_type(npc_type, npc_id));
	}
	buf.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buf.read_int64(npc_id);
		this->npc_id_.insert(Role_Id_Set::value_type(npc_id));
	}
}*/

// 解Tasker::serialize发过来的数据
void Scene_Tasker::load_scene_detail(Block_Buffer &buf) {
	int8_t flag = 0;
	TaskIdSet task_id_set;
//	Int_Int_Vec item_to_source; //val_1:npc_id   val_2:item_id

	Npc_Item_Chance_Map npc_item_chance_map;
	uint16_t size = 0;
	int32_t task_id = 0;

	buf.read_int8(flag);//1 flag
	buf.read_uint16(size);//2  : task_id的个数
	for (uint16_t i = 0; i < size; ++i) {
		buf.read_int32(task_id); //3...  task_id
		task_id_set.insert(task_id);
	}
	switch(flag) {
	case SYNC_TASK_SOURCE_DEL:
	case SYNC_TASK_SOURCE_ADD: {
		buf.read_uint16(size); //4  npc_id的个数
		for (uint16_t i = 0; i < size; ++i) {
			int npc_id = 0;
			Item_Chance item_chance;
			item_chance.reset();

			buf.read_int32(npc_id); //5...  npc_id
			buf.read_int32(item_chance.val_1); //6...  item_id
			buf.read_int32(item_chance.val_2); //7...  chance
//			item_to_source.push_back(i_t_s);
			npc_item_chance_map.insert(std::make_pair(npc_id, item_chance));
		}
		break;
	}
	case SYNC_TASK_SOURCE_TRANSMIT: {
		buf.read_uint16(size);//4     npc_id的个数
//		Int_Int i_t_s;

		uint16_t size2 = 0;
		for (uint16_t i = 0; i < size; ++i) {
			int npc_id = 0;
//			i_t_s.reset();
			buf.read_int32(npc_id);//5... npc_id

			buf.read_uint16(size2); //6...  item_id 的个数
			for (uint16_t j = 0; j < size2; ++j) {
				Item_Chance item_chance;
				item_chance.reset();
				buf.read_int32(item_chance.val_1); //7... item_id
				buf.read_int32(item_chance.val_2); //8... chance
				npc_item_chance_map.insert(std::make_pair(npc_id, item_chance));
			}
		}
		break;
	}
	}

	switch(flag) {
	case SYNC_TASK_SOURCE_ADD:
		for (TaskIdSet::const_iterator it = task_id_set.begin(); it != task_id_set.end(); ++it) {
			this->__task_id_set.insert(*it);
		}
		for (Npc_Item_Chance_Map::const_iterator it = npc_item_chance_map.begin(); it != npc_item_chance_map.end(); ++it ) {
			add_source_to_item(it->first, it->second);
		}
		break;
	case SYNC_TASK_SOURCE_TRANSMIT:
		__task_id_set.clear();
		__npc_ItemIdSet_map.clear();

		for (TaskIdSet::const_iterator it = task_id_set.begin(); it != task_id_set.end(); ++it) {
			this->__task_id_set.insert(*it);
		}
		for (Npc_Item_Chance_Map::const_iterator it = npc_item_chance_map.begin(); it != npc_item_chance_map.end(); ++it ) {
			add_source_to_item(it->first, it->second);
		}
		break;
	case SYNC_TASK_SOURCE_DEL:
		for (TaskIdSet::const_iterator it = task_id_set.begin(); it != task_id_set.end(); ++it) {
			this->__task_id_set.erase(*it);
		}
		for (Npc_Item_Chance_Map::const_iterator it = npc_item_chance_map.begin(); it != npc_item_chance_map.end(); ++it ) {
			erase_source_to_item(it->first, it->second);
		}
		break;
	default:
		break;
	}
}

/*void Scene_Tasker::module_init(void) {
}

void Scene_Tasker::sign_in(void) {
    return ;
}

void Scene_Tasker::sign_out(void) {
    return ;
}*/

void Scene_Tasker::reset(void) {
	__task_id_set.clear();
    this->__npc_ItemIdSet_map.unbind_all();
//    this->npc_type_.clear();
//    this->npc_id_.clear();
}

int Scene_Tasker::add_source_to_item(int npc_id, const Item_Chance &item_chance) {
	if(this->__npc_ItemIdSet_map.exist(npc_id)) {
		this->__npc_ItemIdSet_map[npc_id].insert(item_chance);
	}
	else {
		Item_Chance_Set item_chance_set;
		item_chance_set.insert(item_chance);
		this->__npc_ItemIdSet_map.bind(npc_id, item_chance_set);
	}

//	this->__npc_ItemIdSet_map[npc_id].insert(item_chance);

	return 0;
}

int Scene_Tasker::erase_source_to_item(int npc_id, const Item_Chance &item_chance) {
	Npc_Item_Chance_Set_Map::iterator it = this->__npc_ItemIdSet_map.find(npc_id);
    if (it == this->__npc_ItemIdSet_map.end())
        return 0;

    Item_Chance_Set &id_set = this->__npc_ItemIdSet_map[npc_id];
    id_set.erase(item_chance);
    if (id_set.size() <= 0) {
        this->__npc_ItemIdSet_map.unbind(npc_id);
    }
	return 0;
}

Int_Int_Vec Scene_Tasker::get_item_list(const int npc_id) {
	Int_Int_Vec item_list;
	Npc_Item_Chance_Set_Map::iterator it = this->__npc_ItemIdSet_map.find(npc_id);
    if (it != this->__npc_ItemIdSet_map.end()) {
//    	const Item_Chance_Set &id_set = this->__npc_ItemIdSet_map[npc_id];
//    	item_list.assign(id_set.begin(), id_set.end());
    	for(Item_Chance_Set::const_iterator set_it = it->second.begin(); set_it != it->second.end(); ++set_it) {
    		Int_Int temp_item_chance;
    		temp_item_chance.val_1 = set_it->val_1;
    		temp_item_chance.val_2 = set_it->val_2;
    		item_list.push_back(temp_item_chance);
    	}
    }
	return item_list;
}

bool Scene_Tasker::is_task_exist(Scene_Player *player, int task_id) {
	if(!player) {
		MSG_DEBUG("xxxxxx 采集道具： player is null, task_id = %d", task_id);
		return false;
	}

	bool flag = false;
	TaskIdSet::iterator it = this->__task_id_set.find(task_id);
	if(it == this->__task_id_set.end()) {
		flag = false;
	}
	else {
		flag = true;
	}

	MSG_DEBUG("xxxxxx 采集道具： role_name=%s, task_id=%d, is_task_exist=%s", player->role_name().c_str(), task_id, flag ? "yes" : "no");
	return flag;
}
