/*
 * Answer_Scene_Manager.cpp
 *
 *  Created on: 2014年8月13日
 *      Author: xiaoliang
 */


#include "Answer_Scene_Manager.h"
#include "Answer_Scene.h"


Answer_Scene_Manager::Answer_Scene_Manager() {
	// TODO Auto-generated constructor stub

}

Answer_Scene_Manager::~Answer_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int Answer_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	mid = scene_uid;
	return 0;
}

int Answer_Scene_Manager::tick(Time_Value &now) {
	for (Scene_Map::iterator it = scene_map_.begin(); it != scene_map_.end();) {
		if(it->second->is_recover_status() && it->second->is_recover_time(now)){
			Move_Scene* scene = it->second;
			scene->release_scene_normal();
			scene_pool_.push((Answer_Scene*)scene);
			it = scene_map_.erase(it);
		}else{
			it->second->tick(now);
			it->second->flush_tick(now);
			++it;
		}
	}
	return 0;
}

void Answer_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "compfire scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Answer_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Scene_Map::iterator find_it = scene_map_.find(muid);
	if (find_it != scene_map_.end()) {
		return find_it->second;
	} /*else {
		return 0;
		Answer_Scene *scene = scene_pool_.pop();
		scene->set_create_scene_args(csa);
		scene->init_scene_normal(muid.id, muid.order);
		scene_map_.insert(std::make_pair(muid, scene));
		return scene;
	}*/
	return 0;
}

Move_Scene *Answer_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Scene_Map::iterator find_it = scene_map_.find(muid);
	if (find_it != scene_map_.end()) {
			return find_it->second;
	} else {
		return 0;
	}
}

int Answer_Scene_Manager::data_channle(Block_Buffer& buf) {
	int read_idx = buf.get_read_idx();
	uint8_t type = 0;
	buf.read_uint8(type);
	switch(type) {
		case ANSWER_DATA_CHANNLE_TYPE_LS_OPEN_ACTIVE: {
			open_active(buf);
			break;
		}
		case ANSWER_DATA_CHANNLE_TYPE_LS_CLOSE_ACTIVE: {
			close_active(buf);
			break;
		}
		default : {
			int64_t uuid = 0;
			buf.read_int64(uuid);
			buf.set_read_idx(read_idx);

			break;
		}
	}
	return 0;
}

void Answer_Scene_Manager::open_active(Block_Buffer& buf) {
	int64_t time = 0;
	buf.read_int64(time);
	Time_Value start_time = Time_Value(time);

	Answer_All_Data_Info answer_info;
	answer_info.deserialize(buf);
	//创建
	Answer_Scene *scene = scene_pool_.pop();
	scene->set_scene_status(Move_Scene::SCENE_NORMAL_STATUS);
	scene->init_scene_normal(mid.id, mid.order);
	scene->init_scene_data(start_time, answer_info);
	scene_map_.insert(std::make_pair(mid, scene));
}

void Answer_Scene_Manager::close_active(Block_Buffer& buf) {
	int64_t time = 0;
	buf.read_int64(time);
	Time_Value end_time = Time_Value(time);
	for (Scene_Map::iterator it = scene_map_.begin(); it != scene_map_.end(); ++it) {
		Answer_Scene *scene = (Answer_Scene*)(it->second);
		scene->scene_close_note(end_time);
	}
}
