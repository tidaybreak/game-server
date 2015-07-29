/*
 * Scene_Gang_Global_Manager.cpp
 *
 *  Created on: Jan 13, 2015
 *      Author: linqiyou
 */

#include "Scene_Gang_Global_Manager.h"

Scene_Gang_Global_Manager::Scene_Gang_Global_Manager(void) {
	reset();
}
Scene_Gang_Global_Manager::~Scene_Gang_Global_Manager(void) {
}

void Scene_Gang_Global_Manager::reset(void) {
	gang_id_manager_map_.clear();
	scene_index_ = 0;
}

void Scene_Gang_Global_Manager::tick(Time_Value &now) {
	for(Area_SceneGangManager_Map::iterator it = manager_map_.begin(); it != manager_map_.end(); ++it) {
		it->second->tick(now);
	}
}

Gang_DB_View* Scene_Gang_Global_Manager::gang_view_by_id(int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 0;
	}
	return manager->gang_view_by_id(gang_id);
}

std::string Scene_Gang_Global_Manager::gang_name(int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return "";
	}
	return manager->gang_name(gang_id);
}
role_id_t Scene_Gang_Global_Manager::gang_leader_id(int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 0;
	}
	return manager->gang_leader_id(gang_id);
}
//bool Scene_Gang_Global_Manager::has_member(int64_t gang_id, role_id_t role_id) {
//	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
//	if (!manager) {
//		return false;
//	}
//	return manager->has_member(gang_id, role_id);
//}
int Scene_Gang_Global_Manager::get_gang_level(int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 0;
	}
	return manager->get_gang_level(gang_id);
}

//void process_matched_gangs_pair(void);
int Scene_Gang_Global_Manager::gang_war_load_source_gangs_from_logic(Block_Buffer &buf) {
	int game_inter_flag = 0;
	buf.read_int32(game_inter_flag);

	Scene_Gang_Manager *manager = find_manager(game_inter_flag);
	if (0 == manager) {
		MSG_TRACE_ABORT("ERROR: manager cant find!");
		return -1;
	}
	manager->gang_war_load_source_gangs_from_logic(buf);
	return 0;
}
int64_t Scene_Gang_Global_Manager::find_server_id(const int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 0;
	}
	return manager->find_server_id(gang_id);
}
//int sync_to_logic(Block_Buffer &buf);
int Scene_Gang_Global_Manager::sync_to_logic(Block_Buffer &buf, const int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 0;
	}
	return manager->sync_to_logic(buf, gang_id);
}
int Scene_Gang_Global_Manager::sync_to_logic_by_area(Block_Buffer &buf, const int area) {
	Scene_Gang_Manager* manager = find_manager(area);
	if (!manager) {
		return 0;
	}
	return manager->sync_to_logic(buf);
}
std::string *Scene_Gang_Global_Manager::find_zone(const int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 0;
	}
	return manager->find_zone(gang_id);
}

int Scene_Gang_Global_Manager::set_activity_state(Block_Buffer &buf) {
	int game_inter_flag = 0;
	buf.read_int32(game_inter_flag);

	Scene_Gang_Manager *manager = find_manager(game_inter_flag);
	if (0 == manager) {
		MSG_TRACE_ABORT("ERROR: manager cant find!");
		return -1;
	}
	manager->set_activity_state(buf);
	return 0;
}
//int gang_war_create_16(void);
//int ready_activity(void); // 开始
//int open_activity(void); // 开始
//int close_activity(void); // 结束
//bool is_open(void);
//int set_start_time(Time_Value &now);
int8_t Scene_Gang_Global_Manager::stage(const int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 1;
	}
	return manager->stage();
}

const Gang_War_Matched_Gang_Info *Scene_Gang_Global_Manager::find_matched_gang_info(const int64_t gang_id) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 0;
	}
	return manager->find_matched_gang_info(gang_id);
}
const Gang_War_Matched_Gangs_Pair *Scene_Gang_Global_Manager::find_matched_gangs_pair(const int64_t scene_index, const int area) {
	Scene_Gang_Manager* manager = find_manager(area);
	if (!manager) {
		return 0;
	}
	return manager->find_matched_gangs_pair(scene_index);
}
const Gang_War_Matched_Gangs_Pair *Scene_Gang_Global_Manager::find_matched_gangs_pair_by_gang_id(const int64_t gang_id, const int64_t scene_index) {
	Scene_Gang_Manager* manager = find_manager_by_gang_id(gang_id);
	if (!manager) {
		return 0;
	}
	return manager->find_matched_gangs_pair(scene_index);
}
//int matched_handle(int64_t team_id1, int64_t team_id2); // 记录、广播匹配的两信息
//void matched_handle(Gang_War_Spot &spot);
//GangWarMatchedGangInfo_Map &gang_index_map(void);

Scene_Gang_Manager *Scene_Gang_Global_Manager::find_manager(const int area) {
	Area_SceneGangManager_Map::iterator it = manager_map_.find(area);
	if (it != manager_map_.end()) {
		return it->second;
	} else {
		Scene_Gang_Manager *manager = manager_pool_.pop();
		if (0 == manager) {
			MSG_TRACE_ABORT("ERROR: manager cant pop!");
			return 0;
		}
		manager_map_.insert(std::make_pair(area, manager));
		return manager;
	}
	return 0;
}
bool Scene_Gang_Global_Manager::add_gang(const int64_t gang_id, Scene_Gang_Manager* manager) {
	if (!manager) {
		return false;
	}
	return gang_id_manager_map_.insert(std::make_pair(gang_id, manager)).second;
}
int Scene_Gang_Global_Manager::erase_gang(const int64_t gang_id) {
	return gang_id_manager_map_.erase(gang_id);
}
Scene_Gang_Manager *Scene_Gang_Global_Manager::find_manager_by_gang_id(const int64_t gang_id) {
	Int64_SceneGangManager_Map::iterator it = gang_id_manager_map_.find(gang_id);
	if (it != gang_id_manager_map_.end()) {
		return it->second;
	}
	return 0;
}

int64_t Scene_Gang_Global_Manager::make_scene_index(void) {
	return ++scene_index_;
}
