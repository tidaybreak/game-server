/*
 * Expedition_Global_Scene_Manager.cpp
 *
 *  Created on: 2014-10-14
 *      Author: xiaoliang
 */

#include "Expedition_Global_Scene_Manager.h"
#include "Expedition_Scene.h"
#include "Base_Detail.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Expedition_Scene_Def.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Expedition.h"
#include "DB_Operator.h"
#include "Pool_Manager.h"
#include "DB_Manager.h"
#include "DB_Def.h"
#include "Scene_Public.h"

Expedition_Global_Scene_Manager::Expedition_Global_Scene_Manager() {
	reset();
}

Expedition_Global_Scene_Manager::~Expedition_Global_Scene_Manager() {

}

void Expedition_Global_Scene_Manager::reset(void) {
	save_data_change_ = false;
	uuid_opentime_map_.clear();
	opened_uuid_opentime_map_.clear();

	uuid_inter_map_.clear();
	inter_uuids_map_.clear();
	for (Inter_ExpeditionSceneManager_Map::iterator it = inter_manager_map_.begin();
			it != inter_manager_map_.end(); ++it) {
		manager_pool_.push(it->second);
	}
	inter_manager_map_.clear();
	uuid_manager_map_.clear();

	opentime_.clear();
	first_listen_time_.clear();
	is_first_month_.clear();
	has_open_.clear();

	mmuid_.reset();

	soldier_id_max_ = 0;
	manager_pool_.clear();

	save_time_ = Time_Value::zero;
	save_direct_inter_.clear();
	switch_ = false;

	uuid_had_ask_world_level_.clear();
}

void Expedition_Global_Scene_Manager::load(void) {
	int revive_time = 0;
	const Expedition_Common_Cfg *ecc = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(SWITCH);
	if (ecc) {
		revive_time = ecc->value;
	}
	switch_ = (bool)revive_time;
	if (!switch_) {
		return;
	}
	Expedition_Global_Manager_Data egmd;
	MONGO_OPERATOR->load_expedition_global_manager_data(egmd);
	module_init(egmd);
	Inter_ExpeditionManagerData_Map map;
	MONGO_OPERATOR->load_expedition_manager_data(map);
	module_init(map);
}

void Expedition_Global_Scene_Manager::save(void) {
	Expedition_Global_Manager_Data egmd;
	egmd.opened_uuid_opentime_map_ = opened_uuid_opentime_map_;//服务器的开服时间

	egmd.uuid_inter_map_ = uuid_inter_map_;
	//	Int_RoleHashSet_Map inter_uuids_map_;

	egmd.opentime_ = opentime_; // forver
	//	Time_Value first_listen_time_;
	//	bool is_first_month_;
	//	bool has_open_;

	egmd.mmuid_ = mmuid_;
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_EXPEDITION_MANAGER_DATA);
	egmd.serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);

	// -- global

//	for (Inter_ExpeditionSceneManager_Map::iterator it = inter_manager_map_.begin();
//			it != inter_manager_map_.end(); ++it) {
//		save_by_inter(it->first);
//	}
}

void Expedition_Global_Scene_Manager::save_by_inter(const int inter) {
	Expedition_Manager_Data *emd = POOL_MANAGER->pop_expedition_manager_data();
	if (!emd) {
		LOG_USER("pop expedition_scene_manager error");
		return ;
	}
	Expedition_Scene_Manager *manager = find_manager_by_inter(inter);
	if (!manager) {
		MSG_TRACE_ABORT("ERROR: cant find manager for inter(%d).", inter);
		return ;
	}
	emd->reset();
	emd->inter_ = manager->inter_;

	emd->opened_uuid_opentime_map_ = manager->opened_uuid_opentime_map_;
	emd->gang_id_uuid_map_ = manager->gang_id_uuid_map_;

	emd->last_reset_time_ = manager->last_reset_time_;
	emd->first_gang_rank_ = manager->first_gang_rank_;//首次公会排名名次

	emd->has_init_ = manager->has_init_;
	emd->uuid_has_init_ = manager->uuid_has_init_;

	for (Expedition_Gang_Rank_Info_Map::const_iterator it_egrim = manager->gang_rank_map_.begin();
			it_egrim != manager->gang_rank_map_.end(); ++it_egrim) {
		emd->gang_rank_map_.insert(std::make_pair(it_egrim->first, *it_egrim->second));
	}

	for (Role_ExpeditionGangRankInfoMap_Map::const_iterator it_i_egrim = manager->uuid_gang_rank_map_.begin();
			it_i_egrim != manager->uuid_gang_rank_map_.end(); ++it_i_egrim) {
		for (Expedition_Gang_Rank_Info_Map::const_iterator it_egrim = it_i_egrim->second.begin();
				it_egrim != it_i_egrim->second.end(); ++it_egrim) {
			emd->uuid_gang_rank_map_[it_i_egrim->first].insert(std::make_pair(it_egrim->first, *it_egrim->second));
		}
	}

	emd->gang_rank_sync_success_ = manager->gang_rank_sync_success_;//公会排名获取成功服务器uuid列表

	emd->gang_rank_sync_new_vec_ = manager->gang_rank_sync_new_vec_;
	emd->gang_id_point_id_ = manager->gang_id_point_id_;
	emd->gang_id_cur_line_ = manager->gang_id_cur_line_;
	emd->uuid_gang_id_point_id_ = manager->uuid_gang_id_point_id_;
	emd->uuid_gang_id_cur_line_ = manager->uuid_gang_id_cur_line_;

	// scene--
	{
		Expedition_Scene_Data esd;
		for (Expedition_Scene_Manager::Expedition_Scene_Map::const_iterator it_esm = manager->scene_map_.begin();
				it_esm != manager->scene_map_.end(); ++it_esm) {
			esd.reset();
			{
				Expedition_Scene *es = (Expedition_Scene*) it_esm->second;
				for (Stronghold_Map::iterator it_shm = es->map_data_.begin();
						it_shm != es->map_data_.end(); ++it_shm) {
					if (EBBS_OCCUPY != it_shm->second.info.belong_status) {
						continue;
					}
					Stronghold sh = it_shm->second;
					for (Soldier_Map::const_iterator it_dm = it_shm->second.info.defend_map.begin();
							it_dm != it_shm->second.info.defend_map.end(); ++it_dm) {
						if (it_dm->second->type == EST_PLAYER) {
							continue;
						}
						sh.info.defend_id_map.insert(std::make_pair(it_dm->second->role_id, it_dm->second->nums));
					}
					sh.info.defend_map.clear();
					esd.map_data_.insert(std::make_pair(sh.point_id, sh));
				}
				esd.point_gang_id_ = es->point_gang_id_;
				esd.map_base_point_num_ = es->map_base_point_num_;//地图数据中基地的总数
				esd.cur_line_ = es->cur_line_; //场景的当前线
				esd.award_time_ = es->award_time_;
				esd.settle_time_ = es->settle_time_;
				esd.occupy_castle_rank_ = es->occupy_castle_rank_;
				esd.demage_rank_ = es->demage_rank_;
				esd.refreshed_drop_or_material_ = es->refreshed_drop_or_material_;
				esd.refreshed_monster_timing_ = es->refreshed_monster_timing_;
				esd.refreshed_monster_kill_gap_ = es->refreshed_monster_kill_gap_;
			}

			emd->line_scene_map_.insert(std::make_pair(it_esm->first, esd));
		}

		for (Expedition_Scene_Manager::UUID_ExpeditionSceneMap_Map::const_iterator it_i_esm = manager->uuid_scene_map_.begin();
				it_i_esm != manager->uuid_scene_map_.end(); ++it_i_esm) {
			for (Expedition_Scene_Manager::Expedition_Scene_Map::const_iterator it_esm = it_i_esm->second.begin();
					it_esm != it_i_esm->second.end(); ++it_esm) {
				esd.reset();
				{
					Expedition_Scene *es = (Expedition_Scene*) it_esm->second;
					for (Stronghold_Map::iterator it_shm = es->map_data_.begin();
							it_shm != es->map_data_.end(); ++it_shm) {
						if (EBBS_OCCUPY != it_shm->second.info.belong_status ) {
							continue;
						}
						Stronghold sh = it_shm->second;
						for (Soldier_Map::const_iterator it_dm = it_shm->second.info.defend_map.begin();
								it_dm != it_shm->second.info.defend_map.end(); ++it_dm) {
							if (it_dm->second->type == EST_PLAYER) {
								continue;
							}
							sh.info.defend_id_map.insert(std::make_pair(it_dm->second->role_id, it_dm->second->nums));
						}
						sh.info.defend_map.clear();
						esd.map_data_.insert(std::make_pair(sh.point_id, sh));
					}
					esd.point_gang_id_ = es->point_gang_id_;
					esd.map_base_point_num_ = es->map_base_point_num_;//地图数据中基地的总数
					esd.cur_line_ = es->cur_line_; //场景的当前线
					esd.award_time_ = es->award_time_;
					esd.settle_time_ = es->settle_time_;
					esd.occupy_castle_rank_ = es->occupy_castle_rank_;
					esd.demage_rank_ = es->demage_rank_;
					esd.refreshed_drop_or_material_ = es->refreshed_drop_or_material_;
					esd.refreshed_monster_timing_ = es->refreshed_monster_timing_;
					esd.refreshed_monster_kill_gap_ = es->refreshed_monster_kill_gap_;
				}

				emd->uuid_line_scene_map_[it_i_esm->first].insert(std::make_pair(it_esm->first, esd));
			}
		}
	}
	// -- scene
	DB_MANAGER->push_expedition_manager_data(emd);
	return ;
}

int Expedition_Global_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	mmuid_ = scene_uid;
	load();
	return 0;
}

void Expedition_Global_Scene_Manager::module_init(const Expedition_Global_Manager_Data &data) {
	this->opened_uuid_opentime_map_ = data.opened_uuid_opentime_map_;
	this->uuid_inter_map_ = data.uuid_inter_map_;
	this->opentime_ = data.opentime_;

	for (Role_Int_Map::const_iterator it = uuid_inter_map_.begin();
			it != uuid_inter_map_.end(); ++it) {
		inter_uuids_map_[it->second].insert(it->first);
	}

	Time_Value now = Time_Value::gettimeofday();
	for (Int_Time_Map::const_iterator it = this->opentime_.begin();
			it != this->opentime_.end(); ++it) {
		first_listen_time_.insert(std::make_pair(it->first, Time_Value::zero));
		is_first_month_[it->first] = check_is_first_month(it->first);
		has_open_[it->first] = true;
	}
}

void Expedition_Global_Scene_Manager::module_init(const Inter_ExpeditionManagerData_Map &map) {
	for (Inter_ExpeditionManagerData_Map::const_iterator it = map.begin();
			it != map.end(); ++it) {
		const Expedition_Manager_Data * emd = &(it->second);
		// manager --
		Expedition_Scene_Manager *manager = get_manager_by_inter(emd->inter_);
		if (!manager) {
			MSG_TRACE_ABORT("ERROR: expedition cant find manager for inter(%d).", emd->inter_);
		}
		manager->set_data_by_db(*emd);
		Int_RoleHashSet_Map::const_iterator it_ium = inter_uuids_map_.find(it->first);
		if (it_ium != inter_uuids_map_.end()) {
			for (Role_Hash_Set::const_iterator it_um = it_ium->second.begin();
					it_um != it_ium->second.end(); ++it_um) {
				uuid_manager_map_[*it_um] = manager;//服务器ID对应管理器
			}
		}
		// -- manager
	}

}

int Expedition_Global_Scene_Manager::tick(Time_Value &now) {
	if (!switch_) {
		return 0;
	}
	//定时检测有没有新开的服务器
	tick_check_all_server_open_time(now);

	//保证分组里的manager的tick
	for(Inter_ExpeditionSceneManager_Map::iterator it = inter_manager_map_.begin(); it != inter_manager_map_.end(); ++it) {
		it->second->tick(now);
	}

	tick_check_for_open(now);
	tick_open(now);

	data_save_tick(now);
	data_save_tick_direct(now);

	ask_world_level_tick(now);
	return 0;
}

void Expedition_Global_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "expedition scene", manager_pool_.free_obj_list_size(), manager_pool_.used_obj_list_size(), inter_manager_map_.size());
}

Move_Scene *Expedition_Global_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	LOG_DEBUG("expedition find scene uuid:%ld, uuid_manager_map_.size():%d", csa.id, uuid_manager_map_.size());
	Muid_ExpeditionSceneManager_Map::iterator it = uuid_manager_map_.find(csa.id);
	if(it == uuid_manager_map_.end()) {
		MSG_USER("cant find manager for uuid:%ld", csa.id);
		return 0;
	}
	return it->second->find_scene(muid, role_id, csa);
}

Move_Scene *Expedition_Global_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	return 0;
}

Expedition_Scene_Manager *Expedition_Global_Scene_Manager::find_manager_by_inter(const int inter) {
	Inter_ExpeditionSceneManager_Map::iterator it = inter_manager_map_.find(inter);
	if(it == inter_manager_map_.end()) {
//		MSG_TRACE_ABORT("inter:%d", inter);
		return 0;
	}
	return it->second;
}

Expedition_Scene_Manager *Expedition_Global_Scene_Manager::get_manager_by_inter(const int inter) {
	Expedition_Scene_Manager *manager = find_manager_by_inter(inter);
	if (!manager) {
		manager = manager_pool_.pop();
		if (0 == manager) {
			MSG_TRACE_ABORT("ERROR: manager_pool_ cant pop!");
			manager_pool_.push(manager);
			return 0;
		}
		manager->init_new(mmuid_, inter);
		//建立关系
		inter_manager_map_[inter] = manager;//分组对应的管理器
	}
	return manager;
}

Expedition_Scene_Manager *Expedition_Global_Scene_Manager::find_manager_by_uuid(const int64_t uuid) {
	Muid_ExpeditionSceneManager_Map::iterator it = uuid_manager_map_.find(uuid);
	if(it == uuid_manager_map_.end()) {
		MSG_TRACE_ABORT("uuid:%ld", uuid);
		return 0;
	}
	return it->second;
}

int Expedition_Global_Scene_Manager::find_inter_by_uuid(const int64_t uuid) {
	Role_Int_Map::iterator it = uuid_inter_map_.find(uuid);
	if (it != uuid_inter_map_.end()) {
		return it->second;
	}
	return -1;
}

/*******************************/
int Expedition_Global_Scene_Manager::data_channle(Block_Buffer& buf) {
	int read_idx = buf.get_read_idx();
	uint8_t type = 0;
	buf.read_uint8(type);
	switch(type) {
		case EXPEDITION_CHANNEL_SERVER_OPEN_TIME: {
			int game_flag_inter = 0;
			int64_t uuid = 0;
			int64_t time = 0;
			buf.read_int32(game_flag_inter);
			buf.read_int64(uuid);
			buf.read_int64(time);
			Time_Value time_value(time);
			uuid_opentime_map_[uuid] = time_value;
			uuid_inter_map_[uuid] = game_flag_inter;
			inter_uuids_map_[game_flag_inter].insert(uuid);
			if (!opentime_.count(game_flag_inter) || opentime_[game_flag_inter] > time_value) {
				opentime_[game_flag_inter] = time_value;
				if (!first_listen_time_.count(game_flag_inter)) {
					first_listen_time_[game_flag_inter] = Time_Value::gettimeofday();
				}
			}
			break;
		}
		default : {
			int64_t uuid = 0;
			buf.read_int64(uuid);
			buf.set_read_idx(read_idx);
			Muid_ExpeditionSceneManager_Map::iterator it = uuid_manager_map_.find(uuid);
			if(it != uuid_manager_map_.end()) {
				it->second->data_channle(buf);
			}
			break;
		}
	}

	return 0;
}

//tick检测获取所有服开服时间
void Expedition_Global_Scene_Manager::tick_check_all_server_open_time(Time_Value &now) {
	const Server_Monitor::Monitor_Cid_Map &monitor_connect_cid_map = SCENE_MONITOR->get_monitor_connect_cid_map();
	for(Server_Monitor::Monitor_Cid_Map::const_iterator it = monitor_connect_cid_map.begin();
			it != monitor_connect_cid_map.end();
			++it) {
		if(Server_Enum::MONITOR_LOGIC == it->first.type && it->second > 0) {
			if(0 == uuid_opentime_map_.count(it->first.uuid)
					&& 0 == opened_uuid_opentime_map_.count(it->first.uuid)) {
				//发送获取开服时间的消息到跨服逻辑服
				Block_Buffer block_buf;
				block_buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
				block_buf.write_uint8(EXPEDITION_CHANNEL_SERVER_OPEN_TIME);
				block_buf.finish_message();
				if (0 == SCENE_MONITOR->send_to_logic(it->first, block_buf)) {
					uuid_opentime_map_[it->first.uuid] = Time_Value::zero;
				}
			}
		}
	}
}

void Expedition_Global_Scene_Manager::tick_check_for_open(Time_Value &now) {
	if (opentime_.empty() || has_open_.size() == opentime_.size()) {
		return ;
	}

	int open_gap = 10;
	const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(OPEN_SERVER_GAP);
	if(conf) {
		open_gap = conf->value;
	}
	for (Int_Time_Map::iterator it = opentime_.begin();
			it != opentime_.end(); ++it) {
		if (it->second == Time_Value::zero
				|| has_open_.count(it->first)) {
			continue;
		}
		if (!first_listen_time_.count(it->first)) {
			MSG_TRACE_ABORT("ERROR: cant find first_listen_time for inter(%d).", it->first);
			first_listen_time_.insert(std::make_pair(it->first, now));
		}

		if (first_listen_time_[it->first] > now - Time_Value(open_gap * Time_Value::ONE_MINUTE_IN_SECS)) {
			return ;
		}
		is_first_month_[it->first] = check_is_first_month(it->first);
		MSG_USER("expedition: inter(%d) tick_check_for_open open.", it->first);
		has_open_[it->first] = true;
		save();
	}
}

void Expedition_Global_Scene_Manager::tick_open(Time_Value &now) {
	if (has_open_.empty() || uuid_opentime_map_.empty()) {
		return ;
	}

	int open_gap_for_first_month = 129600;
	const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(OPEN_GAP_FOR_FIRST_MONTH);
	if(conf) {
		open_gap_for_first_month = conf->value;
	}
	Role_Hash_Set uuids_;
	for (Role_Id_Time_Map::iterator it = uuid_opentime_map_.begin();
			it != uuid_opentime_map_.end(); ++it) {
		if (it->second == Time_Value::zero) {
			continue;
		}

		// time check --
		int inter = find_inter_by_uuid(it->first);
		if (inter < 0) {
			MSG_TRACE_ABORT("ERROR: expedition cant find inter for uuid(%ld).", it->first);
			continue;
		}
		if (!has_open(inter)) {
			continue;
		}

		Time_Value open_time = get_day_zero_time(it->second);
		if (is_first_month(inter) && open_time > now - Time_Value(open_gap_for_first_month)) { // time not ready
			continue;
		}
		// -- time check

		// manager --
		Expedition_Scene_Manager *manager = get_manager_by_inter(inter);
		if (!manager) {
			MSG_TRACE_ABORT("ERROR: expedition cant find manager for inter(%d).", inter);
		}
		manager->add_uuid_opentime_map(it->first, it->second);
		uuid_manager_map_[it->first] = manager;//服务器ID对应管理器
		// -- manager

		uuids_.insert(it->first);
		opened_uuid_opentime_map_[it->first] = it->second;
		save();
	}
	for (Role_Hash_Set::const_iterator it = uuids_.begin();
			it != uuids_.end(); ++it) {
		uuid_opentime_map_.erase(*it);
	}
}

void Expedition_Global_Scene_Manager::data_save_tick(Time_Value &now) {
	if (save_time_ == Time_Value::zero) {
		save_time_ = now + Time_Value(30);
	}
	if (save_time_ > now) {
		return;
	}
	save_time_ = now + Time_Value(30);
	for(Inter_ExpeditionSceneManager_Map::iterator it = inter_manager_map_.begin(); it != inter_manager_map_.end(); ++it) {
		if (it->second->change_) {
			save_by_inter(it->first);
			it->second->change_ = false;
		}
	}
}

void Expedition_Global_Scene_Manager::data_save_tick_direct(Time_Value &now) {
	for (Int_Hash_Set::const_iterator it = save_direct_inter_.begin(); it != save_direct_inter_.end(); ++it) {
		Expedition_Scene_Manager *m = find_manager_by_inter(*it);
		if (m && m->change_) {
			save_by_inter(*it);
			m->change_ = false;
		}
	}
	save_direct_inter_.clear();
}

void Expedition_Global_Scene_Manager::ask_world_level_tick(const Time_Value &now) {
	const Server_Monitor::Monitor_Cid_Map &monitor_connect_cid_map = SCENE_MONITOR->get_monitor_connect_cid_map();
	for(Server_Monitor::Monitor_Cid_Map::const_iterator it = monitor_connect_cid_map.begin();
			it != monitor_connect_cid_map.end();
			++it) {
		if(Server_Enum::MONITOR_LOGIC == it->first.type && it->second > 0) {
			if(0 == uuid_had_ask_world_level_.count(it->first.uuid)) {
				Block_Buffer buf;
				buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
				buf.write_uint8(EXPEDITION_ASK_WORLD_LEVEL);
				buf.finish_message();
				if (0 == SCENE_MONITOR->send_to_logic(it->first, buf)) {
					uuid_had_ask_world_level_.insert(it->first.uuid);
				}
			}
		}
	}
}

Time_Value Expedition_Global_Scene_Manager::get_day_zero_time(const Time_Value &date) {
	Date_Time date_(date);
	date_.microsec(0);
	date_.hour(0);
	date_.minute(0);
	date_.second(0);
	return Time_Value(date_.time_sec());
}

role_id_t Expedition_Global_Scene_Manager::make_soldier_id_max(void) {
	return ++soldier_id_max_;
}

bool Expedition_Global_Scene_Manager::has_open(const int inter) {
	if (!has_open_.count(inter)) {
		return false;
	}
	return has_open_[inter];
}

bool Expedition_Global_Scene_Manager::is_first_month(const int inter) {
	return true;
	if (!is_first_month_.count(inter)) {
		MSG_TRACE_ABORT("ERROR: cant find is_first_month for inter(%d).", inter);
		return true;
	}
	return is_first_month_[inter];
}

Int_Hash_Set &Expedition_Global_Scene_Manager::save_direct_inter(void) {
	return save_direct_inter_;
}

bool Expedition_Global_Scene_Manager::check_is_first_month(const int inter) {
	if (!opentime_.count(inter)) {
		MSG_TRACE_ABORT("ERROR: cant find opentime for inter(%d).", inter);
		opentime_.insert(std::make_pair(inter, Time_Value::gettimeofday()));
	}
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt_now(now);
	Date_Time dt(opentime_[inter]);
	return dt.year() == dt_now.year() && dt.month() == dt_now.month();
}
