/*
 * Expedition_Scene_Manager.cpp
 *
 *  Created on: 2014-10-14
 *      Author: xiaoliang
 */

#include "Expedition_Scene_Manager.h"
#include "Expedition_Scene.h"
#include "Base_Detail.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Expedition_Scene_Def.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Expedition.h"
#include "Config_Cache_NPC.h"
#include "expedition/Expedition_Def.h"
#include "Expedition_Global_Scene_Manager.h"
#include "Expedition_Scene_Def.h"
#include "Pool_Manager.h"

Expedition_Scene_Manager::Expedition_Scene_Manager() {
	reset();
}

Expedition_Scene_Manager::~Expedition_Scene_Manager() {

}

void Expedition_Scene_Manager::reset(void) {
	for (Expedition_Scene_Map::iterator it = scene_map_.begin(); it != scene_map_.end(); ++it) {
		scene_pool_.push((Expedition_Scene*)it->second);
	}
	scene_map_.clear();
	for (UUID_ExpeditionSceneMap_Map::iterator it_inter = uuid_scene_map_.begin();
			it_inter != uuid_scene_map_.end(); ++it_inter) {
		for (Expedition_Scene_Map::iterator it = it_inter->second.begin(); it != it_inter->second.end(); ++it) {
			scene_pool_.push((Expedition_Scene*)it->second);
		}
	}
	uuid_scene_map_.clear();

	mmuid_.reset();
	inter_ = 0;

	uuid_opentime_map_.clear();//服务器的开服时间
	opened_uuid_opentime_map_.clear();//服务器的开服时间
	uuid_gang_id_map_.clear();
	gang_id_uuid_map_.clear();
	first_listen_time_ = Time_Value::zero;
	uuid_first_listen_time_.clear();

	last_reset_time_ = Time_Value::zero;//最后一次重置地图时间
	first_gang_rank_.clear();//首次公会排名名次

	has_init_ = false;
	uuid_has_init_.clear();

	for (Expedition_Gang_Rank_Info_Map::iterator it = gang_rank_map_.begin(); it != gang_rank_map_.end(); ++it) {
		gang_rank_info_pool_.push(it->second);
	}
	gang_rank_.clear();//公会排名
	for (Role_ExpeditionGangRankInfoMap_Map::iterator it_igrm = uuid_gang_rank_map_.begin();
			it_igrm != uuid_gang_rank_map_.end(); ++it_igrm) {
		for (Expedition_Gang_Rank_Info_Map::iterator it = it_igrm->second.begin(); it != it_igrm->second.end(); ++it) {
			gang_rank_info_pool_.push(it->second);
		}
	}
	uuid_gang_rank_map_.clear();
	uuid_gang_rank_.clear();
	gang_rank_info_pool_.clear();

	gang_rank_sync_waiting_.clear();//公会排名获取等待服务器uuid列表
	gang_rank_sync_success_.clear();//公会排名获取成功服务器uuid列表
	gang_rank_sync_new_.clear();//公会排名的新公会列表; 这个必须要求要有顺序
	gang_rank_sync_new_vec_.clear();
	gang_id_point_id_.clear();
	gang_id_cur_line_.clear();
	uuid_gang_id_point_id_.clear();
	uuid_gang_id_cur_line_.clear();
	gang_ids_need_sync_to_logic_.clear();
	scene_pool_.clear();

	change_ = false;
}

int Expedition_Scene_Manager::init(const Monitor_Unique_ID &scene_uid, uint32_t groupid, Role_Id_Time_Map uuids) {
	mmuid_ = scene_uid;
	uuid_opentime_map_ = uuids;
	inter_ = groupid;
	//加载公用场景数据
	load_common_scene_data();
	//加载地图所有数据
	load_all_map_data(scene_map_);
	return 0;
}

int Expedition_Scene_Manager::init_new(const Monitor_Unique_ID &scene_uid, uint32_t groupid) {
	mmuid_ = scene_uid;
//	uuid_opentime_map_ = uuids;
	inter_ = groupid;

	return 0;
}

int Expedition_Scene_Manager::tick(Time_Value &now) {
	for (Expedition_Scene_Map::iterator scene_it = scene_map_.begin(); scene_it != scene_map_.end(); ++scene_it) {
		scene_it->second->tick(now);
		scene_it->second->flush_tick(now);
	}
	for (UUID_ExpeditionSceneMap_Map::iterator it = uuid_scene_map_.begin(); it != uuid_scene_map_.end(); ++it) {
		for (Expedition_Scene_Map::iterator scene_it = it->second.begin(); scene_it != it->second.end(); ++scene_it) {
			scene_it->second->tick(now);
			scene_it->second->flush_tick(now);
		}
	}

	tick_send_get_gang_rank_info_to_all_logic_server();
	tick_init(now);
	tick_add_init(now);

	//tick检测重置场景数据
	tick_check_reset_scene_map_data(now);
	//tick检测所有新公会拖入远征场景并生成城堡数据
	tick_check_all_new_gang_to_scene_make_data(now);
	tick_send_logic_gang_info(now);
	return 0;
}

Move_Scene *Expedition_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		UUID_ExpeditionSceneMap_Map::iterator it = uuid_scene_map_.find(csa.id);
		if (it != uuid_scene_map_.end()) {
			Expedition_Scene_Map::iterator scene_it_find_it = it->second.find(csa.int_value);
			if (scene_it_find_it != it->second.end()) {
				return scene_it_find_it->second;
			}
		}
		MSG_USER("ERROR: cant find expedition_scene for uuid(%ld) line(%d).", csa.id, csa.int_value);
	} else {
		Expedition_Scene_Map::iterator scene_it_find_it = scene_map_.find(csa.int_value);
		if (scene_it_find_it != scene_map_.end()) {
			return scene_it_find_it->second;
		}
		MSG_USER("ERROR: cant find expedition_scene for line(%d).", csa.int_value);
	}
	return 0;
}

Move_Scene *Expedition_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {

	return 0;
}

Move_Scene *Expedition_Scene_Manager::find_scene_by_cur_line(const int cur_line) {
	Expedition_Scene_Map::iterator scene_it_find_it = scene_map_.find(cur_line);
	if (scene_it_find_it != scene_map_.end()) {
		return scene_it_find_it->second;
	}
	return 0;
}

Move_Scene *Expedition_Scene_Manager::find_scene_by_cur_line(const int64_t uuid, const int cur_line) {
	UUID_ExpeditionSceneMap_Map::iterator it_usm = uuid_scene_map_.find(uuid);
	if (it_usm != uuid_scene_map_.end()) {
		Expedition_Scene_Map::iterator scene_it_find_it = it_usm->second.find(cur_line);
		if (scene_it_find_it != it_usm->second.end()) {
			return scene_it_find_it->second;
		}
	}
	return 0;
}

/*******************************/
int Expedition_Scene_Manager::data_channle(Block_Buffer& buf) {
	uint8_t type = 0;
	buf.read_uint8(type);
	switch(type) {
		case EXPEDITION_CHANNEL_SERVER_GANG_INFO: {
			int64_t uuid = 0;
			buf.read_int64(uuid);

			uint32_t size = 0;
			buf.read_uint32(size);
			Role_Vec gang_ids;
			for(uint32_t i = 0; i < size; ++i) {
				Expedition_Gang_Rank_Info *gi = gang_rank_info_pool_.pop();
				if(!gi) {
					LOG_USER("gang_rank_info_pool pop error");
					gang_rank_info_pool_.push(gi);
					continue;
				}
				gi->deserialize(buf);
				Expedition_Gang_Rank_Info_Map *map = 0;
				if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
					map = &(uuid_gang_rank_map_[uuid]);
				} else {
					map = &(gang_rank_map_);
				}
				Expedition_Gang_Rank_Info_Map::iterator it = map->find(gi->gang_id);
				if (it != map->end()) {
					*(it->second) = *gi;
					gang_rank_info_pool_.push(gi);
					continue;
				}
				map->insert(std::make_pair(gi->gang_id, gi));

				uuid_gang_id_map_[uuid].insert(gi->gang_id);
				gang_id_uuid_map_[gi->gang_id] = uuid;
				gang_ids.push_back(gi->gang_id);
			}
			sort_gang_rank_info_to_vec(uuid, gang_ids);
			make_ranking_num_info_on_vec_for_rank();

			gang_rank_sync_waiting_.erase(uuid);
			gang_rank_sync_success_.insert(uuid);
			if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
				if (!uuid_first_listen_time_.count(uuid) || uuid_first_listen_time_[uuid] == Time_Value::zero) {
					uuid_first_listen_time_[uuid] = Time_Value::gettimeofday();
				}
			} else if (first_listen_time_ == Time_Value::zero) {
				first_listen_time_ = Time_Value::gettimeofday();
			}
			save_tick_direct();
			break;
		}
		case EXPEDITION_CHANNEL_SERVER_GANG_INFO_ADD: {
			int64_t uuid = 0;
			buf.read_int64(uuid);
			int inter = EXPEDITION_GLOBAL_SCENE_MANAGER->find_inter_by_uuid(uuid);
			if (inter < 0) {
				MSG_TRACE_ABORT("ERROR: expedition cant find inter for uuid(%ld).", uuid);
				inter = 0;
			}

			uint32_t size = 0;
			buf.read_uint32(size);
			Role_Vec gang_ids;
			for(uint32_t i = 0; i < size; ++i) {
				Expedition_Gang_Rank_Info *gi = gang_rank_info_pool_.pop();
				if(!gi) {
					LOG_USER("gang_rank_info_pool pop error");
					gang_rank_info_pool_.push(gi);
					continue;
				}
				gi->deserialize(buf);
				Expedition_Gang_Rank_Info_Map *map = 0;
				if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
					map = &(uuid_gang_rank_map_[uuid]);
				} else {
					map = &(gang_rank_map_);
				}
				Expedition_Gang_Rank_Info_Map::iterator it = map->find(gi->gang_id);
				if (it != map->end()) {
					*(it->second) = *gi;
					gang_rank_info_pool_.push(gi);
					continue;
				}
				map->insert(std::make_pair(gi->gang_id, gi));
//				if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
//					uuid_gang_rank_map_[inter][gi->gang_id] = gi;
//				} else {
//					gang_rank_map_[gi->gang_id] = gi;
//				}
//				Expedition_Gang_Rank_Info_Map::iterator it = gang_rank_map_.find(gi->gang_id);
//				if (it != gang_rank_map_.end()) {
//					*(it->second) = *gi;
//					gang_rank_info_pool_.push(gi);
//					continue;
//				}
				uuid_gang_id_map_[uuid].insert(gi->gang_id);
				gang_id_uuid_map_[gi->gang_id] = uuid;
				gang_ids.push_back(gi->gang_id);
			}
			add_gang_rank_info(uuid, gang_ids);
			sort_gang_rank_info_to_vec();
			make_ranking_num_info_on_vec_for_rank();
			save_tick_direct();
			break;
		}
		case EXPEDITION_CHANNEL_SERVER_GANG_INFO_DEL: {
			int64_t uuid = 0;
			buf.read_int64(uuid);
			int inter = EXPEDITION_GLOBAL_SCENE_MANAGER->find_inter_by_uuid(uuid);
			if (inter < 0) {
				MSG_TRACE_ABORT("ERROR: expedition cant find inter for uuid(%ld).", uuid);
				inter = 0;
			}
			int64_t gang_id = 0;
			buf.read_int64(gang_id);
			del_gang_rank_info(uuid, gang_id);
			save_tick_direct();
			break;
		}
		case EXPEDITION_CHANNEL_CREATE_DAILY_AWARD: {
			int64_t uuid = 0;
			buf.read_int64(uuid);
			for (Expedition_Scene_Map::iterator it = this->scene_map_.begin();
					it != scene_map_.end(); ++it) {
				Expedition_Scene *es = (Expedition_Scene *) it->second;
				es->create_and_send_daily_award_to_logic();
			}
			for (UUID_ExpeditionSceneMap_Map::iterator it_uesm = uuid_scene_map_.begin(); it_uesm != uuid_scene_map_.end(); ++it_uesm) {
				for (Expedition_Scene_Map::iterator it = it_uesm->second.begin();
						it != it_uesm->second.end(); ++it) {
					Expedition_Scene *es = (Expedition_Scene *) it->second;
					es->create_and_send_daily_award_to_logic();
				}
			}
			break;
		}
		case EXPEDITION_CHANNEL_CREATE_DEMAGE_RANK_AWARD: {
			int64_t uuid = 0;
			buf.read_int64(uuid);
			for (Expedition_Scene_Map::iterator it = this->scene_map_.begin();
					it != scene_map_.end(); ++it) {
				Expedition_Scene *es = (Expedition_Scene *) it->second;
				es->settle_handle();
			}
			for (UUID_ExpeditionSceneMap_Map::iterator it_uesm = uuid_scene_map_.begin(); it_uesm != uuid_scene_map_.end(); ++it_uesm) {
				for (Expedition_Scene_Map::iterator it = it_uesm->second.begin();
						it != it_uesm->second.end(); ++it) {
					Expedition_Scene *es = (Expedition_Scene *) it->second;
					es->settle_handle();
				}
			}
			break;
		}
		case EXPEDITION_CHANGE_LEADER: {
			int64_t uuid = 0;
			buf.read_int64(uuid);
			int inter = EXPEDITION_GLOBAL_SCENE_MANAGER->find_inter_by_uuid(uuid);
			if (inter < 0) {
				MSG_TRACE_ABORT("ERROR: expedition cant find inter for uuid(%ld).", uuid);
				inter = 0;
			}
			Base_Detail base_detail;
			base_detail.deserialize(buf);
			Expedition_Gang_Rank_Info* egri = find_gang_rank_info(uuid, base_detail.gang_id);
			if (egri) {
				egri->gang_leader_id = base_detail.role_id;
				egri->gang_leader_name = base_detail.role_name;
				save_tick();
			}
			break;
		}
		default:
			break;
	}
	return 0;
}

int Expedition_Scene_Manager::add_uuid_opentime_map(const int64_t uuid, const Time_Value open_time) {
	uuid_opentime_map_[uuid] = open_time;
	return 0;
}

void Expedition_Scene_Manager::load_common_scene_data(void) {

}

void Expedition_Scene_Manager::load_all_map_data(Expedition_Scene_Map &expedition_scene_map) {
	expedition_scene_map.clear();
}

//tick检测重置场景数据
void Expedition_Scene_Manager::tick_check_reset_scene_map_data(Time_Value &now) {

}

//tick检测所有新公会拖入远征场景并生成城堡数据
void Expedition_Scene_Manager::tick_check_all_new_gang_to_scene_make_data(Time_Value &now) {
	if(gang_rank_sync_new_vec_.empty()) {
		return;
	}

	for(Role_Vec::iterator it = gang_rank_sync_new_vec_.begin(); it != gang_rank_sync_new_vec_.end(); ++it) {
		add_gang_to_scene(*it);
	}

	gang_rank_sync_new_vec_.clear();

//	last_reset_time_ = now;
	save_tick_direct();
}

void Expedition_Scene_Manager::tick_send_logic_gang_info(Time_Value &now) {
	Role_Hash_Set rhs;
	for (Role_Hash_Set::const_iterator it = gang_ids_need_sync_to_logic_.begin();
			it != gang_ids_need_sync_to_logic_.end(); ++it) {
		int64_t gang_id = *it;
		Expedition_Gang_Rank_Info *gi = find_gang_rank_info_by_gang_id(gang_id);
		if (0 == gi) {
			rhs.insert(gang_id);
			MSG_USER("ERROR: cant find gang_rank to gang_id(%ld).", gang_id);
			continue;
		}
		int point_id = this->find_point_id_by_gang_id(gang_id);
		if (point_id < 0) {
			rhs.insert(gang_id);
			MSG_TRACE_ABORT("ERROR: cant find point_id gang_id(%ld).", gang_id);
			continue;
		}
		int cur_line = find_cur_line_by_gang_id(gang_id);
		if (cur_line < 0) {
			rhs.insert(gang_id);
			MSG_USER("ERROR: cant find cur_line gang_id(%ld).", gang_id);
			continue;
		}

		//通知对应的逻辑服的公会所在线号 需要把基地ID(NPCid)传回去
		Coord coord;
		coord.type = Coord::PIXEL_TYPE;
		const Expedition_Map_Point_Cfg *pconf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
		if(pconf) {
			coord.x = pconf->x;
			coord.y = pconf->y;
		}
		if (0 == send_to_server_gang_on_line_no(gi->uuid, gi->gang_id, cur_line, coord)) {
			rhs.insert(gang_id);
		}
	}
	for (Role_Hash_Set::const_iterator it = rhs.begin();
				it != rhs.end(); ++it) {
		gang_ids_need_sync_to_logic_.erase(*it);
	}
}

int Expedition_Scene_Manager::send_to_server_gang_on_line_no(int64_t server_uuid, int64_t gang_id, uint32_t cur_line, Coord &coord) {
	Block_Buffer block_buf;
	block_buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	block_buf.write_uint8(EXPEDITION_CHANNEL_SCENE_NOTICE_SERVER_GANG_NO);
	block_buf.write_int64(gang_id);
	block_buf.write_uint32(cur_line);
	coord.serialize(block_buf);
	block_buf.finish_message();
	return SCENE_MONITOR->send_to_logic(server_uuid, block_buf);
}

int Expedition_Scene_Manager::add_gang_to_scene(const int64_t gang_id, const bool need_try_again) {
	if (gang_id <= 0) {
		return -1;
	}
	Expedition_Scene *scene = get_one_available_scene(gang_id);
	if(scene) {
//		++scene->map_gang_in_num();
		const Expedition_Gang_Home_Cfg *conf = scene->get_one_point_conf();
		if(conf) {
			Stronghold *sh = scene->find_map_data_by_point_id(conf->point_id);
			if (0 == sh) {
				scene->point_gang_id().insert(std::make_pair(conf->point_id, 0));
				MSG_TRACE_ABORT("ERROR: cant find map_data to point_id(%d).", conf->point_id);
				if (need_try_again) {
					return add_gang_to_scene(gang_id, false);
				}
				return -1;
			}

			Expedition_Gang_Rank_Info *gi = find_gang_rank_info_by_gang_id(gang_id);
			if (0 == gi) {
				MSG_TRACE_ABORT("ERROR: cant find gang_rank to gang_id(%ld).", gang_id);
				return -1;
			}
			sh->info.fight_status = EBFS_PEACE;
			sh->info.belong_status = EBBS_OCCUPY;
			sh->info.occupier.gang_id = gi->gang_id;//占领该据点的公会ID
			sh->info.occupier.server_uuid = gi->uuid;//占领该据点的服务器ID
			sh->info.occupier.server_name = gi->server_name;//占领该据点的服务器名字
			sh->info.occupier.gang_name = gi->gang_name;//占领该据点的公会名字

			//通知对应的逻辑服的公会所在线号 需要把基地ID(NPCid)传回去
			Coord coord;
			coord.type = Coord::PIXEL_TYPE;
			const Expedition_Map_Point_Cfg *pconf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(conf->point_id);
			if(pconf) {
				coord.x = pconf->x;
				coord.y = pconf->y;
			}

			if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
				int64_t uuid = 0;
				RoleID_RoleID_Map::const_iterator it = gang_id_uuid_map_.find(gang_id);
				if (it != gang_id_uuid_map_.end()) {
					uuid = it->second;
				} else {
					MSG_TRACE_ABORT("ERROR:expedition cant find inter for uuid(%ld).", uuid);
				}
				uuid_gang_id_point_id_[uuid][gi->gang_id] = conf->point_id;
				uuid_gang_id_cur_line_[uuid][gi->gang_id] = scene->cur_line();
			} else {
				gang_id_point_id_[gi->gang_id] = conf->point_id;
				gang_id_cur_line_[gi->gang_id] = scene->cur_line();
			}
			scene->init_gang_base_territory(conf->point_id, true);
			scene->active_map_all_data();
			gang_ids_need_sync_to_logic_.insert(gi->gang_id);
			gi->asset = scene->get_hold_nums_by_gang_id(gi->gang_id);
			sort_gang_rank_info_to_vec();
			make_ranking_num_info_on_vec_for_rank();
		}
		save_tick_direct();
	}
	return 0;
}

int Expedition_Scene_Manager::del_gang_to_scene(const int64_t gang_id) {
	if (gang_id <= 0) {
		return -1;
	}
	Expedition_Scene_Map *expedition_scene_map = 0;
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		int64_t uuid = 0;
		RoleID_RoleID_Map::const_iterator it = gang_id_uuid_map_.find(gang_id);
		if (it != gang_id_uuid_map_.end()) {
			uuid = it->second;
		} else {
			MSG_TRACE_ABORT("ERROR:expedition cant find uuid for gang_id(%ld).", gang_id);
		}

		if (0 == uuid_scene_map_.count(uuid)) {
			return -1;
		}
		expedition_scene_map = &(uuid_scene_map_[uuid]);

		if (0 == uuid_gang_id_cur_line_.count(uuid)) {
			return -1;
		}
		Role_Int_Map::iterator it_igicl = uuid_gang_id_cur_line_[uuid].find(gang_id);
		if (it_igicl != uuid_gang_id_cur_line_[uuid].end()) {
			Expedition_Scene_Map::iterator it_m = expedition_scene_map->find(it_igicl->second);
			if (it_m != expedition_scene_map->end()) {
				Expedition_Scene *scene = (Expedition_Scene *)it_m->second;
				scene->clear_gang_info(gang_id, true);
				scene->active_map_all_data();
			}
		}
	} else {
		expedition_scene_map = &scene_map_;
		Role_Int_Map::iterator it_igicl = gang_id_cur_line_.find(gang_id);
		if (it_igicl != gang_id_cur_line_.end()) {
			Expedition_Scene_Map::iterator it_m = expedition_scene_map->find(it_igicl->second);
			if (it_m != expedition_scene_map->end()) {
				Expedition_Scene *scene = (Expedition_Scene *)it_m->second;
				scene->clear_gang_info(gang_id, true);
				scene->active_map_all_data();
			}
		}
	}
	return 0;
}

int Expedition_Scene_Manager::find_point_id_by_gang_id(const int64_t gang_id) {
	if (gang_id <= 0) {
		return -1;
	}
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		int64_t uuid = 0;
		RoleID_RoleID_Map::const_iterator it = gang_id_uuid_map_.find(gang_id);
		if (it != gang_id_uuid_map_.end()) {
			uuid = it->second;
		} else {
			MSG_TRACE_ABORT("ERROR:expedition cant find uuid for gang_id(%ld).", gang_id);
		}
		Role_RoleUIntMap_Map::iterator it_igipi = uuid_gang_id_point_id_.find(uuid);
		if (it_igipi != uuid_gang_id_point_id_.end()) {
			Role_UInt_Map::iterator it_gipi = it_igipi->second.find(gang_id);
			if (it_gipi != it_igipi->second.end()) {
				return it_gipi->second;
			}
		}
	} else {
		Role_UInt_Map::iterator it = gang_id_point_id_.find(gang_id);
		if (it != gang_id_point_id_.end()) {
			return it->second;
		}
	}
	return -1;
}

int Expedition_Scene_Manager::find_cur_line_by_gang_id(const int64_t gang_id) {
	if (gang_id <= 0) {
		return -1;
	}
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		int64_t uuid = 0;
		RoleID_RoleID_Map::const_iterator it = gang_id_uuid_map_.find(gang_id);
		if (it != gang_id_uuid_map_.end()) {
			uuid = it->second;
		} else {
			MSG_TRACE_ABORT("ERROR:expedition cant find uuid for gang_id(%ld).", gang_id);
		}
		Role_RoleIntMap_Map::iterator it_igipi = uuid_gang_id_cur_line_.find(uuid);
		if (it_igipi != uuid_gang_id_cur_line_.end()) {
			Role_Int_Map::iterator it_gipi = it_igipi->second.find(gang_id);
			if (it_gipi != it_igipi->second.end()) {
				return it_gipi->second;
			}
		}
	} else {
		Role_Int_Map::iterator it = gang_id_cur_line_.find(gang_id);
		if (it != gang_id_cur_line_.end()) {
			return it->second;
		}
	}
	return -1;
}

Expedition_Gang_Rank_Info *Expedition_Scene_Manager::find_gang_rank_info_by_gang_id(const int64_t gang_id) {
	if (gang_id <= 0) {
		return 0;
	}
	Expedition_Gang_Rank_Info_Map *map = 0;
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		int64_t uuid = 0;
		RoleID_RoleID_Map::iterator it = gang_id_uuid_map_.find(gang_id);
		if (it != gang_id_uuid_map_.end()) {
			uuid = it->second;
		} else {
			MSG_TRACE_ABORT("ERROR:expedition cant find uuid for gang_id(%ld).", gang_id);
		}
		if (0 == uuid_gang_rank_map_.count(uuid)) {
			return 0;
		}
		map = &(uuid_gang_rank_map_[uuid]);
	} else {
		map = &(gang_rank_map_);
	}
	Expedition_Gang_Rank_Info_Map::iterator gi = map->find(gang_id);
	if (gi == map->end()) {
		MSG_TRACE_ABORT("ERROR:cant find gang(%ld) from gang_rank_map.", gang_id);
		return 0;
	}
	return gi->second;
}

void Expedition_Scene_Manager::save_tick(void) {
	change_ = true;
}

void Expedition_Scene_Manager::save_tick_direct(void) {
	change_ = true;
	EXPEDITION_GLOBAL_SCENE_MANAGER->save_direct_inter().insert(inter_);
}

int Expedition_Scene_Manager::line_nums(void) {
	return scene_map_.size();
}

const Expedition_Gang_Rank_Info_Vec *Expedition_Scene_Manager::gang_rank(const int64_t uuid) {
	Expedition_Gang_Rank_Info_Vec *vec = 0;
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		vec = &(uuid_gang_rank_[uuid]);
	} else {
		vec = &(gang_rank_);
	}
	return vec;
}

const Time_Value &Expedition_Scene_Manager::last_reset_time(void) {
	return last_reset_time_;
}

uint32_t &Expedition_Scene_Manager::get_inter(void) {
	return inter_;
}

Time_Value Expedition_Scene_Manager::get_day_zero_time(const Time_Value &date) {
	Date_Time date_(date);
	date_.microsec(0);
	date_.hour(0);
	date_.minute(0);
	date_.second(0);
	return Time_Value(date_.time_sec());
}

bool expedition_gang_rank_info_compare(Expedition_Gang_Rank_Info *gi1, Expedition_Gang_Rank_Info *gi2) {
	if(!gi1 || !gi2) {
		return false;
	}
	//gi1 和 gi2 比较大小; 大返回true; 小返回false
	//优先级：公会财富>公会总战力>公会人数>公会等级>公会创建时间
	if(gi1->asset > gi2->asset) {
		return true;
	} else if (gi1->asset < gi2->asset) {
		return false;
	}
	if(gi1->force > gi2->force) {//公会总战力
		return true;
	} else if(gi1->force < gi2->force) {
		return false;
	}
	if(gi1->num > gi2->num) {//公会人数
		return true;
	} else if(gi1->num < gi2->num) {
		return false;
	}
	if(gi1->level > gi2->level) {//公会等级
		return true;
	} else if(gi1->level < gi2->level) {
		return false;
	}
	if(gi1->create_time < gi2->create_time) {//公会创建时间 时间小的越早
		return true;
	} else if (gi1->create_time > gi2->create_time) {
		return false;
	}
	if (gi1->gang_id < gi2->gang_id) {
		return true;
	}
	return false;
}

void Expedition_Scene_Manager::sort_gang_rank_info_to_vec(const int64_t uuid, const Role_Vec &gang_ids) {
//	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
//		for (Role_Bool_Map::iterator it = uuid_has_init_.begin();
//				it != uuid_has_init_.end(); ++it) {
//			if (!it->second) {
//				sort_gang_rank_info_to_vec();
//			} else {
//				add_gang_rank_info(uuid, gang_ids);
//			}
//		}
//	} else {
		if (!has_init_) {
			sort_gang_rank_info_to_vec();
		} else {
			add_gang_rank_info(uuid, gang_ids);
		}
//	}
}

void Expedition_Scene_Manager::sort_gang_rank_info_to_vec(void) {
	gang_rank_.clear();
	for (Expedition_Gang_Rank_Info_Map::iterator it = gang_rank_map_.begin();
			it != gang_rank_map_.end(); ++it) {
		gang_rank_.push_back(it->second);
	}
	std::sort(gang_rank_.begin(), gang_rank_.end(), expedition_gang_rank_info_compare);

	uuid_gang_rank_.clear();
	for (Role_ExpeditionGangRankInfoMap_Map::iterator it = uuid_gang_rank_map_.begin();
			it != uuid_gang_rank_map_.end(); ++it) {
		for (Expedition_Gang_Rank_Info_Map::iterator it_egrim = it->second.begin();
				it_egrim != it->second.end(); ++it_egrim) {
			uuid_gang_rank_[it->first].push_back(it_egrim->second);
		}
		std::sort(uuid_gang_rank_[it->first].begin(), uuid_gang_rank_[it->first].end(), expedition_gang_rank_info_compare);
	}
}

int Expedition_Scene_Manager::add_gang_rank_info(const int64_t uuid, const Role_Vec &gang_ids) {
	Expedition_Gang_Rank_Info_Map *map = 0;
	Expedition_Gang_Rank_Info_Vec *vec = 0;
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		if (!uuid_gang_rank_map_.count(uuid)) {
			MSG_USER("ERROR: expedition cant find uuid(%ld).", uuid);
			return -1;
		}
		if (!uuid_gang_rank_.count(uuid)) {
			uuid_gang_rank_.insert(std::make_pair(uuid, Expedition_Gang_Rank_Info_Vec()));
		}
		map = &(uuid_gang_rank_map_[uuid]);
		vec = &(uuid_gang_rank_[uuid]);
	} else {
		map = &(gang_rank_map_);
		vec = &(gang_rank_);
	}
	for (Role_Vec::const_iterator it = gang_ids.begin(); it != gang_ids.end(); ++it) {
		Expedition_Gang_Rank_Info_Map::iterator it_m = map->find(*it);
		if (it_m == map->end()) {
			MSG_TRACE_ABORT("ERROR: expedition cant find gang_id(%ld) uuid(%ld).", *it, uuid);
			continue;
		}
		vec->push_back(it_m->second);
		this->add_gang_to_scene(*it);
	}
	save_tick_direct();
	return 0;
}

Expedition_Gang_Rank_Info* Expedition_Scene_Manager::find_gang_rank_info(const int64_t uuid, const int64_t gang_id) {
	Expedition_Gang_Rank_Info_Map *map = 0;
//	Expedition_Gang_Rank_Info_Vec *vec = 0;
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		map = &(uuid_gang_rank_map_[uuid]);
//		vec = &(uuid_gang_rank_[uuid]);
	} else {
		map = &(gang_rank_map_);
//		vec = &(gang_rank_);
	}
	Expedition_Gang_Rank_Info_Map::iterator it = map->find(gang_id);
	if (it != map->end()) {
		return (it->second);
	}
	return 0;
}

int Expedition_Scene_Manager::del_gang_rank_info(const int64_t uuid, const int64_t gang_id) {
	Expedition_Gang_Rank_Info_Map *map = 0;
	Expedition_Gang_Rank_Info_Vec *vec = 0;
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		map = &(uuid_gang_rank_map_[uuid]);
		vec = &(uuid_gang_rank_[uuid]);
	} else {
		map = &(gang_rank_map_);
		vec = &(gang_rank_);
	}
	Expedition_Gang_Rank_Info_Map::iterator it_m = map->find(gang_id);
	if (it_m == map->end()) {
		MSG_TRACE_ABORT("ERROR: expedition cant find gang_id(%ld) uuid(%ld).", gang_id, uuid);
		return -1;
	}
	for (Expedition_Gang_Rank_Info_Vec::iterator it_vec = vec->begin(); it_vec != vec->end(); ++it_vec) {
		if ((**it_vec).gang_id == gang_id) {
			vec->erase(it_vec);
			break;
		}
	}
	del_gang_to_scene(gang_id);
	gang_rank_info_pool_.push(it_m->second);
	map->erase(it_m);
	save_tick_direct();
	return 0;
}

void Expedition_Scene_Manager::make_ranking_num_info_on_vec(void) {
	for(uint32_t i = 0; i < gang_rank_.size(); ++i) {
		gang_rank_[i]->rank = i+1;
//		if(last_reset_time_ != Time_Value::zero) {
//			//不是第一次生成
//			gang_rank_[i]->expedition_rank = 0;
//			Role_UInt_Map::iterator it = first_gang_rank_.find(gang_rank_[i]->gang_id);
//			if(it != first_gang_rank_.end()) {
//				gang_rank_[i]->expedition_rank = it->second;
//			}
//		} else {
			//第一次生成
			gang_rank_[i]->expedition_rank = i+1;
//			first_gang_rank_[gang_rank_[i]->gang_id] = i+1;
//		}
		//记录新公会
		if(0 == gang_rank_sync_new_.count(gang_rank_[i]->gang_id)) {
			gang_rank_sync_new_.insert(gang_rank_[i]->gang_id);
			gang_rank_sync_new_vec_.push_back(gang_rank_[i]->gang_id);
		}
	}

	for (Role_ExpeditionGangRankInfoVec_Map::iterator it = uuid_gang_rank_.begin();
			it != uuid_gang_rank_.end(); ++it) {
		for(uint32_t i = 0; i < it->second.size(); ++i) {
			it->second[i]->rank = i+1;
			it->second[i]->expedition_rank = i+1;
			//记录新公会
			if(0 == gang_rank_sync_new_.count(gang_rank_[i]->gang_id)) {
				gang_rank_sync_new_.insert(gang_rank_[i]->gang_id);
				gang_rank_sync_new_vec_.push_back(gang_rank_[i]->gang_id);
			}
//			if(0 == uuid_gang_rank_sync_new_[it->first].count(it->second[i]->gang_id)) {
//				uuid_gang_rank_sync_new_[it->first].insert(it->second[i]->gang_id);
//				uuid_gang_rank_sync_new_vec_[it->first].push_back(it->second[i]->gang_id);
//			}
		}
	}
}

void Expedition_Scene_Manager::make_ranking_num_info_on_vec_for_rank(void) {
	for(uint32_t i = 0; i < gang_rank_.size(); ++i) {
		gang_rank_[i]->rank = i+1;
	}
	for (Role_ExpeditionGangRankInfoVec_Map::iterator it_igr = uuid_gang_rank_.begin();
			it_igr != uuid_gang_rank_.end(); ++it_igr) {
		for(uint32_t i = 0; i < it_igr->second.size(); ++i) {
			it_igr->second[i]->rank = i+1;
		}
	}
}

Time_Value *Expedition_Scene_Manager::find_open_time_by_uuid(const int64_t uuid) {
	Role_Id_Time_Map::iterator it = opened_uuid_opentime_map_.find(uuid);
	if (it != opened_uuid_opentime_map_.end()) {
		return &(it->second);
	}
	return 0;
}

void Expedition_Scene_Manager::tick_send_get_gang_rank_info_to_all_logic_server(void) {
	Role_Hash_Set uuids_;
	//领取所有公会信息
	for(Role_Id_Time_Map::iterator it = uuid_opentime_map_.begin(); it != uuid_opentime_map_.end(); ++it) {
		if(gang_rank_sync_waiting_.count(it->first) ||
			gang_rank_sync_success_.count(it->first)) {
			continue;//正在同步;或者已经同步成功
		}
		//发送获取所有公会信息的消息到跨服逻辑服
		Block_Buffer block_buf;
		block_buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
		block_buf.write_uint8(EXPEDITION_CHANNEL_SERVER_GANG_INFO);
		block_buf.finish_message();
		if (0 == SCENE_MONITOR->send_to_logic(it->first, block_buf)) {
			gang_rank_sync_waiting_.insert(it->first);
			uuids_.insert(it->first);
			opened_uuid_opentime_map_[it->first] = it->second;
		}
	}

	for (Role_Hash_Set::const_iterator it = uuids_.begin(); it != uuids_.end(); ++it) {
		uuid_opentime_map_.erase(*it);
	}
}

void Expedition_Scene_Manager::tick_init(Time_Value &now) {
	if (has_init_) {
		return ;
	}
//	MSG_USER("expedition: tick_init.");
	int manager_gang_init_gap = 300;
	const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(MANAGER_GANG_INIT_GAP);
	if(conf) {
		manager_gang_init_gap = conf->value;
	}


//	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
//		for (Role_Id_Time_Map::iterator it = uuid_first_listen_time_.begin();
//				it != uuid_first_listen_time_.end(); ++it) {
//			if (it->second > now - Time_Value(manager_gang_init_gap)) {
//				make_ranking_num_info_on_vec(it->first);
//				uuid_has_init_[it->first] = true;
//				MSG_USER("expedition: tick_init open uuid(%ld).", it->first);
//			}
//		}
//	} else if (!has_init_ && first_listen_time_ > now - Time_Value(manager_gang_init_gap)) {
		make_ranking_num_info_on_vec();
		has_init_ = true;
		MSG_USER("expedition: tick_init open.");
//	}

}

void Expedition_Scene_Manager::tick_add_init(Time_Value &now) {

}

Expedition_Scene*  Expedition_Scene_Manager::get_one_available_scene(const int64_t gang_id) {
	Expedition_Scene_Map *expedition_scene_map = 0;
	if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(inter_)) {
		int64_t uuid = 0;
		RoleID_RoleID_Map::const_iterator it = gang_id_uuid_map_.find(gang_id);
		if (it != gang_id_uuid_map_.end()) {
			uuid = it->second;
		} else {
			MSG_TRACE_ABORT("ERROR:expedition cant find uuid for gang_id(%ld).", gang_id);
		}
		expedition_scene_map = &(uuid_scene_map_[uuid]);
	} else {
		expedition_scene_map = &scene_map_;
	}

	for (Expedition_Scene_Map::iterator it = expedition_scene_map->begin();
			it != expedition_scene_map->end(); ++it) {
		Expedition_Scene *scene = (Expedition_Scene*)it->second;
		if(scene->point_gang_id().size() < scene->map_base_point_num()) {
			return scene;
		}
	}

	uint32_t cur_line = expedition_scene_map->size();
	++cur_line;
	Expedition_Scene *scene = scene_pool_.pop();
	if (0 == scene) {
		MSG_TRACE_ABORT("ERROR: cant pop Expedition_Scene");
		return 0;
	}
	scene->init_scene_normal(mmuid_.id, mmuid_.order);
	Create_Scene_Args csa;
	csa.int_value = cur_line;
	scene->set_create_scene_args(csa);
	scene->create_scene_map_data(cur_line, this);
	expedition_scene_map->insert(std::make_pair(csa.int_value, scene));
	return scene;
}

Expedition_Scene*  Expedition_Scene_Manager::get_scene_by_db(const int line, const int64_t gang_id, const bool is_first_month) {
	Expedition_Scene_Map *expedition_scene_map = 0;
	if (is_first_month) {
		int64_t uuid = 0;
		RoleID_RoleID_Map::const_iterator it = gang_id_uuid_map_.find(gang_id);
		if (it != gang_id_uuid_map_.end()) {
			uuid = it->second;
		} else {
			MSG_TRACE_ABORT("ERROR:expedition cant find uuid for gang_id(%ld).", gang_id);
		}
		expedition_scene_map = &(uuid_scene_map_[uuid]);
	} else {
		expedition_scene_map = &scene_map_;
	}

	Expedition_Scene_Map::iterator it = expedition_scene_map->find(line);
	if (it != expedition_scene_map->end()) {
		Expedition_Scene *scene = (Expedition_Scene*)it->second;
		return scene;
	}
	Expedition_Scene *scene = scene_pool_.pop();
	if (0 == scene) {
		MSG_TRACE_ABORT("ERROR: cant pop Expedition_Scene");
		return 0;
	}
	scene->init_scene_normal(mmuid_.id, mmuid_.order);
	Create_Scene_Args csa;
	csa.int_value = line;
	scene->set_create_scene_args(csa);
	scene->create_scene_map_data(line, this);
	expedition_scene_map->insert(std::make_pair(csa.int_value, scene));
	return scene;
}

int Expedition_Scene_Manager::set_data_by_db(const Expedition_Manager_Data &data) {
	Expedition_Scene_Map scene_map_;
	UUID_ExpeditionSceneMap_Map uuid_scene_map_;

	inter_ = data.inter_;

	opened_uuid_opentime_map_ = data.opened_uuid_opentime_map_;

	gang_id_uuid_map_ = data.gang_id_uuid_map_;
	for (RoleID_RoleID_Map::const_iterator it = gang_id_uuid_map_.begin();
			it != gang_id_uuid_map_.end(); ++it) {
		uuid_gang_id_map_[it->second].insert(it->first);
	}
//	Time_Value first_listen_time_;

	last_reset_time_ = data.last_reset_time_;
	first_gang_rank_ = data.first_gang_rank_;

//	has_init_ = data.has_init_;
//	uuid_has_init_ = data.uuid_has_init_;
	has_init_ = true;
	for (Role_Bool_Map::const_iterator it = data.uuid_has_init_.begin(); it != data.uuid_has_init_.end(); ++it) {
		uuid_has_init_[it->first] = true;
	}

//	Role_Hash_Set gang_rank_sync_waiting_;//公会排名获取等待服务器uuid列表
	gang_rank_sync_success_ = data.gang_rank_sync_success_;//公会排名获取成功服务器uuid列表
//	gang_rank_sync_new_ = data.gang_rank_sync_new_;//公会排名的新公会列表; 这个必须要求要有顺序
	gang_rank_sync_new_vec_ = data.gang_rank_sync_new_vec_;
	gang_id_point_id_ = data.gang_id_point_id_;
	gang_id_cur_line_ = data.gang_id_cur_line_;
	uuid_gang_id_point_id_ = data.uuid_gang_id_point_id_;
	uuid_gang_id_cur_line_ = data.uuid_gang_id_cur_line_;

	for (Gang_ExpeditionGangRankInfo_Map::const_iterator it = data.gang_rank_map_.begin();
			it != data.gang_rank_map_.end(); ++it) {
		Expedition_Gang_Rank_Info *gi = gang_rank_info_pool_.pop();
		if(!gi) {
			LOG_USER("gang_rank_info_pool pop error");
			gang_rank_info_pool_.push(gi);
			continue;
		}
		*gi = it->second;
		Expedition_Gang_Rank_Info_Map::iterator it = gang_rank_map_.find(gi->gang_id);
		if (it != gang_rank_map_.end()) {
			*(it->second) = *gi;
			gang_rank_info_pool_.push(gi);
			continue;
		}
		gang_rank_map_.insert(std::make_pair(gi->gang_id, gi));

		uuid_gang_id_map_[gi->uuid].insert(gi->gang_id);
		gang_id_uuid_map_[gi->gang_id] = gi->uuid;
	}
	for (Role_GangExpeditionGangRankInfoMap_Map::const_iterator it_ugrm = data.uuid_gang_rank_map_.begin();
			it_ugrm != data.uuid_gang_rank_map_.end(); ++it_ugrm) {
		for (Gang_ExpeditionGangRankInfo_Map::const_iterator it = it_ugrm->second.begin();
				it != it_ugrm->second.end(); ++it) {
			Expedition_Gang_Rank_Info *gi = gang_rank_info_pool_.pop();
			if(!gi) {
				LOG_USER("gang_rank_info_pool pop error");
				gang_rank_info_pool_.push(gi);
				continue;
			}
			*gi = it->second;
			Expedition_Gang_Rank_Info_Map::iterator it = uuid_gang_rank_map_[it_ugrm->first].find(gi->gang_id);
			if (it != uuid_gang_rank_map_[it_ugrm->first].end()) {
				*(it->second) = *gi;
				gang_rank_info_pool_.push(gi);
				continue;
			}
			uuid_gang_rank_map_[it_ugrm->first].insert(std::make_pair(gi->gang_id, gi));

			uuid_gang_id_map_[gi->uuid].insert(gi->gang_id);
			gang_id_uuid_map_[gi->gang_id] = gi->uuid;
		}
	}
	sort_gang_rank_info_to_vec();

	init_gang_rank_map();
	init_uuid_gang_rank_map();

	init_scene_map(data);
	init_uuid_scene_map(data);
	return 0;
}

void Expedition_Scene_Manager::init_gang_rank_map(void) {
	for (Expedition_Gang_Rank_Info_Map::const_iterator it = gang_rank_map_.begin();
			it != gang_rank_map_.end(); ++it) {
		int64_t gang_id = it->first;
		Role_Int_Map::const_iterator it_gicl = gang_id_cur_line_.find(gang_id);
		if (it_gicl != gang_id_cur_line_.end()) {
			Expedition_Scene* scene = get_scene_by_db(it_gicl->second, gang_id, false);
			if (scene) {
//				++scene->map_gang_in_num();
				int point_id = 0;
				Role_UInt_Map::iterator it_gipi = gang_id_point_id_.find(gang_id);
				if (it_gipi != gang_id_point_id_.end()) {
					point_id = it_gipi->second;
				} else {
					MSG_TRACE_ABORT("ERROR:cant find point_id for gang(%ld) from db.", gang_id);
					const Expedition_Gang_Home_Cfg *conf = scene->get_one_point_conf();
					if (conf) {
						point_id = conf->point_id;
					} else {
						MSG_TRACE_ABORT("ERROR: cant find point_id for gang(%ld) neigher db nor conf.", gang_id);
						continue;
					}
				}

				Stronghold *sh = scene->find_map_data_by_point_id(point_id);
				if (0 == sh) {
					scene->point_gang_id().insert(std::make_pair(point_id, 0));
					MSG_TRACE_ABORT("ERROR: cant find map_data to point_id(%d).", point_id);
					continue;
				}

				sh->info.fight_status = EBFS_PEACE;
				sh->info.belong_status = EBBS_OCCUPY;
				sh->info.occupier.gang_id = it->second->gang_id;//占领该据点的公会ID
				sh->info.occupier.server_uuid = it->second->uuid;//占领该据点的服务器ID
				sh->info.occupier.server_name = it->second->server_name;//占领该据点的服务器名字
				sh->info.occupier.gang_name = it->second->gang_name;//占领该据点的公会名字

				//通知对应的逻辑服的公会所在线号 需要把基地ID(NPCid)传回去
				Coord coord;
				coord.type = Coord::PIXEL_TYPE;
				const Expedition_Map_Point_Cfg *pconf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
				if(pconf) {
					coord.x = pconf->x;
					coord.y = pconf->y;
				}

				gang_id_point_id_[gang_id] = point_id;
				gang_id_cur_line_[gang_id] = scene->cur_line();

				scene->init_gang_base_territory(point_id, true);
//				scene->active_map_all_data();
//				send_to_server_gang_on_line_no(sh->info.occupier.server_uuid, sh->info.occupier.gang_id, scene->cur_line(), coord);
			}
		} else {
			MSG_TRACE_ABORT("ERROR: cant find line for gang_id(%ld) form db.", it->first);
		}
	}
}

void Expedition_Scene_Manager::init_uuid_gang_rank_map(void) {
	for (Role_ExpeditionGangRankInfoMap_Map::const_iterator it_ugrm = uuid_gang_rank_map_.begin();
			it_ugrm != uuid_gang_rank_map_.end(); ++it_ugrm) {
		for (Expedition_Gang_Rank_Info_Map::const_iterator it = it_ugrm->second.begin();
				it != it_ugrm->second.end(); ++it) {
			int64_t gang_id = it->first;
			if (uuid_gang_id_cur_line_.count(it_ugrm->first)) {
				Role_Int_Map::const_iterator it_gicl = uuid_gang_id_cur_line_[it_ugrm->first].find(gang_id);
				if (it_gicl != uuid_gang_id_cur_line_[it_ugrm->first].end()) {
					Expedition_Scene* scene = get_scene_by_db(it_gicl->second, gang_id, true);
					if (scene) {
	//					++scene->map_gang_in_num();
						int point_id = 0;
						Role_UInt_Map::iterator it_gipi = uuid_gang_id_point_id_[it_ugrm->first].find(gang_id);
						if (it_gipi != uuid_gang_id_point_id_[it_ugrm->first].end()) {
							point_id = it_gipi->second;
						} else {
							MSG_TRACE_ABORT("ERROR:cant find point_id for gang(%ld) from db.", gang_id);
							const Expedition_Gang_Home_Cfg *conf = scene->get_one_point_conf();
							if (conf) {
								point_id = conf->point_id;
							} else {
								MSG_TRACE_ABORT("ERROR: cant find point_id for gang(%ld) neigher db nor conf.", gang_id);
								continue;
							}
						}

						Stronghold *sh = scene->find_map_data_by_point_id(point_id);
						if (0 == sh) {
							scene->point_gang_id().insert(std::make_pair(point_id, 0));
							MSG_TRACE_ABORT("ERROR: cant find map_data to point_id(%d).", point_id);
							continue;
						}

						sh->info.fight_status = EBFS_PEACE;
						sh->info.belong_status = EBBS_OCCUPY;
						sh->info.occupier.gang_id = it->second->gang_id;//占领该据点的公会ID
						sh->info.occupier.server_uuid = it->second->uuid;//占领该据点的服务器ID
						sh->info.occupier.server_name = it->second->server_name;//占领该据点的服务器名字
						sh->info.occupier.gang_name = it->second->gang_name;//占领该据点的公会名字

						//通知对应的逻辑服的公会所在线号 需要把基地ID(NPCid)传回去
						Coord coord;
						coord.type = Coord::PIXEL_TYPE;
						const Expedition_Map_Point_Cfg *pconf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
						if(pconf) {
							coord.x = pconf->x;
							coord.y = pconf->y;
						}

						uuid_gang_id_point_id_[it_ugrm->first][gang_id] = point_id;
						uuid_gang_id_cur_line_[it_ugrm->first][gang_id] = scene->cur_line();

						scene->init_gang_base_territory(point_id, true);
	//					scene->active_map_all_data();
	//					send_to_server_gang_on_line_no(sh->info.occupier.server_uuid, sh->info.occupier.gang_id, scene->cur_line(), coord);
					}
				} else {
					MSG_TRACE_ABORT("ERROR: cant find line for gang_id(%ld) form db.", it->first);
				}

			} else {
				MSG_TRACE_ABORT("ERROR: cant find uuid(%ld) line for gang_id(%ld) form db.", it_ugrm->first, it->first);
			}

		}
	}

}


bool Expedition_Scene_Occupy_Castle_Rank_Greater(Expedition_Occupy_Castle_Data c1, Expedition_Occupy_Castle_Data c2);
void Expedition_Scene_Manager::init_scene_map(const Expedition_Manager_Data &data) {
	for (Line_ExpeditionSceneData_Map::const_iterator it = data.line_scene_map_.begin();
			it != data.line_scene_map_.end(); ++it) {
		Move_Scene *scene = find_scene_by_cur_line(it->first);
		if (!scene) {
			//MSG_TRACE_ABORT("ERROR: cant find scene from scene_map for cur_line(%d).", it->first);
			continue;
		}
		Expedition_Scene *es = (Expedition_Scene*) scene;
		const Expedition_Scene_Data *esd = &(it->second);

		es->map_base_point_num_ = esd->map_base_point_num_;
		es->point_gang_id_ = esd->point_gang_id_;
		es->award_time_ = esd->award_time_;
		es->settle_time_ = esd->settle_time_;
		es->occupy_castle_rank_ = esd->occupy_castle_rank_;
		es->occupy_castle_vec_.clear();
		for (Role_ExpeditionOccupyCastleData_Map::const_iterator it_eocd = es->occupy_castle_rank_.begin();
				it_eocd != es->occupy_castle_rank_.end(); ++it_eocd) {
			es->occupy_castle_vec_.push_back(it_eocd->second);
		}
		std::sort(es->occupy_castle_vec_.begin(), es->occupy_castle_vec_.end(), Expedition_Scene_Occupy_Castle_Rank_Greater);
		es->init_refreshed_monster(esd->refreshed_drop_or_material_);

		es->refreshed_monster_timing_ = esd->refreshed_monster_timing_;
		es->refreshed_monster_kill_gap_ = esd->refreshed_monster_kill_gap_;

		for (Stronghold_Map::iterator it_md = es->map_data_.begin();
				it_md != es->map_data_.end(); ++it_md) {
			Stronghold_Map::const_iterator it_esd_md = esd->map_data_.find(it_md->first);
			if (it_esd_md == esd->map_data_.end()
					|| EBT_CHEST == it_md->second.type
					|| EBT_BEAST == it_md->second.type) {
				if (EBBS_OCCUPY == it_md->second.info.belong_status) {
					it_md->second.info.fight_status = EBFS_PEACE;
					if (it_md->second.type != EBT_GANG_BASE) {
						it_md->second.info.belong_status = EBBS_NEUTRAL;
					} else {
						it_md->second.info.belong_status = EBBS_UNINIT;
					}
				}
			} else {
				it_md->second.type = it_esd_md->second.type;
				it_md->second.info.occupier = it_esd_md->second.info.occupier;
				it_md->second.info.belong_status = EBBS_OCCUPY;
				if (it_md->second.type != EBT_GANG_BASE) {
					int point_id = this->find_point_id_by_gang_id(it_md->second.info.occupier.gang_id);
					es->add_base_territorys(point_id, it_md->first);
				}
				Role_Vec rv_dead;
				for (Soldier_Map::const_iterator it_dm = it_md->second.info.defend_map.begin();
						it_dm != it_md->second.info.defend_map.end(); ++it_dm) {
					if (EBT_BEAST == it_md->second.type
							|| EBT_CHEST == it_md->second.type) {
						continue;
					}
					Role_Int_Map::const_iterator it_dim = it_esd_md->second.info.defend_id_map.find(it_dm->second->role_id);
					if (it_dim == it_esd_md->second.info.defend_id_map.end()) {
						rv_dead.push_back(it_dm->first);
						continue;
					}
					it_dm->second->nums = it_dim->second;
				}
				for (Role_Vec::const_iterator it_rv = rv_dead.begin(); it_rv != rv_dead.end(); ++it_rv) {
					Soldier_Map::iterator it_soldier = it_md->second.info.defend_map.find(*it_rv);
					if (it_soldier != it_md->second.info.defend_map.end()) {
						it_md->second.info.defend_list.remove(it_soldier->second);
						POOL_MANAGER->push_soldier_pool(it_soldier->second);
						it_md->second.info.defend_map.erase(*it_rv);
					}
				}
			}
		}
		es->demage_rank_ = esd->demage_rank_;
		es->reset_demage_vec();
		for (Role_Hash_Set::const_iterator it_gi = es->gang_ids_.begin();
				it_gi != es->gang_ids_.end(); ++it_gi) {
			int point_id = this->find_point_id_by_gang_id(*it_gi);
			es->init_gang_base_territory(point_id);
			Expedition_Gang_Rank_Info *gi = find_gang_rank_info_by_gang_id(*it_gi);
			if (0 == gi) {
				continue;
			}
			gi->asset = es->get_hold_nums_by_gang_id(gi->gang_id);
			sort_gang_rank_info_to_vec();
			this->make_ranking_num_info_on_vec_for_rank();
		}
	}
}

void Expedition_Scene_Manager::init_uuid_scene_map(const Expedition_Manager_Data &data) {
	for (UUID_LineExpeditionSceneData_Map_Map::const_iterator it_ulsm = data.uuid_line_scene_map_.begin();
			it_ulsm != data.uuid_line_scene_map_.end(); ++it_ulsm) {
		for (Line_ExpeditionSceneData_Map::const_iterator it = it_ulsm->second.begin();
				it != it_ulsm->second.end(); ++it) {
			Move_Scene *scene = find_scene_by_cur_line(it_ulsm->first, it->first);
			if (!scene) {
				MSG_TRACE_ABORT("ERROR: cant find scene from scene_map for cur_line(%d).", it->first);
				continue;
			}
			Expedition_Scene *es = (Expedition_Scene*) scene;
			const Expedition_Scene_Data *esd = &(it->second);

			es->map_base_point_num_ = esd->map_base_point_num_;
			es->point_gang_id_ = esd->point_gang_id_;
			for (Stronghold_Map::iterator it_md = es->map_data_.begin();
					it_md != es->map_data_.end(); ++it_md) {
				Stronghold_Map::const_iterator it_esd_md = esd->map_data_.find(it_md->first);
				if (it_esd_md == esd->map_data_.end()) {
					if (EBBS_OCCUPY == it_md->second.info.belong_status
							|| EBT_CHEST == it_md->second.type
							|| EBT_BEAST == it_md->second.type) {
						it_md->second.info.fight_status = EBFS_PEACE;
						if (it_md->second.type != EBT_GANG_BASE) {
							it_md->second.info.belong_status = EBBS_NEUTRAL;
						} else {
							it_md->second.info.belong_status = EBBS_UNINIT;
						}
					}
				} else {
					it_md->second.type = it_esd_md->second.type;
					it_md->second.info.occupier = it_esd_md->second.info.occupier;
					it_md->second.info.belong_status = EBBS_OCCUPY;
					if (it_md->second.type != EBT_GANG_BASE) {
						int point_id = this->find_point_id_by_gang_id(it_md->second.info.occupier.gang_id);
						es->add_base_territorys(point_id, it_md->first);
					}
					Role_Vec rv_dead;
					for (Soldier_Map::const_iterator it_dm = it_md->second.info.defend_map.begin();
							it_dm != it_md->second.info.defend_map.end(); ++it_dm) {
						Role_Int_Map::const_iterator it_dim = it_esd_md->second.info.defend_id_map.find(it_dm->second->role_id);
						if (it_dim == it_esd_md->second.info.defend_id_map.end()) {
							rv_dead.push_back(it_dm->first);
							continue;
						}
						it_dm->second->nums = it_dim->second;
					}
					for (Role_Vec::const_iterator it_rv = rv_dead.begin(); it_rv != rv_dead.end(); ++it_rv) {
						Soldier_Map::iterator it_soldier = it_md->second.info.defend_map.find(*it_rv);
						if (it_soldier != it_md->second.info.defend_map.end()) {
							it_md->second.info.defend_list.remove(it_soldier->second);
							POOL_MANAGER->push_soldier_pool(it_soldier->second);
							it_md->second.info.defend_map.erase(*it_rv);
						}
					}
				}
			}
			es->award_time_ = esd->award_time_;
			es->settle_time_ = esd->settle_time_;
			es->occupy_castle_rank_ = esd->occupy_castle_rank_;
			es->occupy_castle_vec_.clear();
			for (Role_ExpeditionOccupyCastleData_Map::const_iterator it_eocd = es->occupy_castle_rank_.begin();
					it_eocd != es->occupy_castle_rank_.end(); ++it_eocd) {
				es->occupy_castle_vec_.push_back(it_eocd->second);
			}
			std::sort(es->occupy_castle_vec_.begin(), es->occupy_castle_vec_.end(), Expedition_Scene_Occupy_Castle_Rank_Greater);
			es->init_refreshed_monster(esd->refreshed_drop_or_material_);

			es->refreshed_monster_timing_ = esd->refreshed_monster_timing_;
			es->refreshed_monster_kill_gap_ = esd->refreshed_monster_kill_gap_;

			es->demage_rank_ = esd->demage_rank_;
			es->reset_demage_vec();
			for (Role_Hash_Set::const_iterator it_gi = es->gang_ids_.begin();
					it_gi != es->gang_ids_.end(); ++it_gi) {
				int point_id = this->find_point_id_by_gang_id(*it_gi);
				es->init_gang_base_territory(point_id);
				Expedition_Gang_Rank_Info *gi = find_gang_rank_info_by_gang_id(*it_gi);
				if (0 == gi) {
					continue;
				}
				gi->asset = es->get_hold_nums_by_gang_id(gi->gang_id);
				sort_gang_rank_info_to_vec();
				this->make_ranking_num_info_on_vec_for_rank();
			}
		}
	}

}
