/*
 * Ranking.cpp
 *
 *  Created on: 2014-05-22
 *      Author: xiaoliang
 */

#include "Ranking_Manager.h"
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Msg_Struct.h"
#include "Global_Timer.h"
#include "Config_Cache_Ranking.h"
#include "ranking/Ranking_Def.h"
#include "DB_Def.h"
#include "pack/Pack_Struct.h"
#include "Config_Cache_Item.h"
#include "Logic_Monitor.h"
#include "Logic_Player_Struct.h"
#include "gang/Gang_Manager.h"
#include "gang/Gang_Struct.h"
#include "title/Title_Manager.h"
#include "Msg_Active_Struct.h"
#include "title/Title_Def.h"
#include "Config_Cache_Public.h"
#include "Pool_Manager.h"
#include "Msg_Inner_Enum.h"


Ranking_Manager::Ranking_Manager() {
	// TODO Auto-generated constructor stub
	reset();
}

Ranking_Manager::~Ranking_Manager() {
	// TODO Auto-generated destructor stub
}

void Ranking_Manager::reset(void) {
	rank_.clear();
	save_tick_sec = 30;
	save_tick_time = Time_Value::gettimeofday() + Time_Value(save_tick_sec);
	world_level_ = 0;
	need_sync_world_level_ = false;
}

int Ranking_Manager::start(void) {
	MONGO_OPERATOR->load_ranking(rank_);
	for(Ranking_Map::iterator iter = rank_.begin(); iter != rank_.end(); ++iter) {
		iter->second.tick_sec = CONFIG_CACHE_RANKING->get_time_by_ranking_type(iter->first);
		iter->second.tick_time = Time_Value::gettimeofday() + Time_Value(iter->second.tick_sec);
	}
	Ranking *ra = get_rank_by_type(RankingDef::LOCAL_PERSONAL_LEVEL_RANKING);
	if (ra) {
		int i = 1;
		for(Ranking_Sort_List::const_iterator iter = ra->ranking_.begin(); iter != ra->ranking_.end(); ++iter) {
			if (i == CONFIG_CACHE_PUBLIC->world_level_index()) {
				need_sync_world_level_ = true;
				set_world_level(iter->ext_3);
				break;
			}
			++i;
		}
	}
	return 0;
}

int Ranking_Manager::monitor_equipment_off(role_id_t role_id, int32_t item_id, int32_t item_index) {
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_RANKING_RECORD);
	buf.write_uint32(RankingDef::SAVE_EQUIPMENT_CHANGE);

	buf.write_int8(RankingDef::CHANGE_DELETE);
	buf.write_int64(role_id);
	buf.write_int32(item_id);
	buf.write_int32(item_index);

	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}
int Ranking_Manager::monitor_equipment_change(role_id_t role_id, const Item_Detail* item) {
	if(item->id_ < 10100000 || item->id_ > 19999999) {
		return 0;
	}
	monitor_equipment_off(role_id, item->id_, item->index_);
	if(item->index_ < 10000 || item->index_ >= 20000) {
		return 0;
	}
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		MSG_USER("player(role_id=%ld) is not in db_view", role_id);
		return -1;
	}
	const Item_Detail_Config* item_config = CONFIG_CACHE_ITEM->find_item(item->id_);
	if(!item_config) {
		MSG_USER("no item(item_id=%d) on config", item->id_);
		return -1;
	}
	std::string role_name = iter->second->role_name;
	std::string item_name = item_config->name;
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_RANKING_RECORD);
	buf.write_uint32(RankingDef::SAVE_EQUIPMENT_CHANGE);

	buf.write_int8(RankingDef::CHANGE_UPDATE);
	buf.write_int64(role_id);
	buf.write_int32(item->id_);
	buf.write_int32(item->index_);
	buf.write_string(role_name);
	buf.write_string(item_name);
	buf.write_int32(item->addon.equip.force);

	Item_Equip_Info item_info;
	item_info.reset();
	item->addon.equip.make_item_equip_info(item_info);
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(player) {
		for(int i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
			if(item->addon.equip.card_hole_array_[i].card_index) {
				const Item_Detail* card_item = player->pack_get_const_item(
						item->addon.equip.card_hole_array_[i].card_index);
				if(card_item && card_item->type_ == Item_Detail::CARD) {
					Card_Info card;
					card_item->addon.card.make_item_card_info(card_item->id_, card_item->index_, card);
					item_info.card_info_vec.push_back(card);
				}
			}
		}
	}
	item_info.serialize(buf);

	Item_Basic_Info item_base;
	item_base.reset();
	item->set_item_basic_info(item_base);
	item_base.serialize(buf);

	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Ranking_Manager::process_refresh_ranking_data_p(Ranking *rank) {
	if(!rank) {
		MSG_DEBUG("rank == 0");
		return -1;
	}
	Ranking_Map::iterator it = rank_.find(rank->type);
	if(it != rank_.end()) {
		it->second.ranking_ = rank->ranking_;
		it->second.role_info_ = rank->role_info_;
		it->second.item_info_ = rank->item_info_;
		it->second.heroer_info_ = rank->heroer_info_;
		it->second.tick_time = Time_Value::gettimeofday() + Time_Value(it->second.tick_sec);
		it->second.update_time = Time_Value::gettimeofday();
		it->second.is_change = true;

		if (rank->type == RankingDef::LOCAL_PERSONAL_LEVEL_RANKING) {
			int i = 1;
			for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
				if (i == CONFIG_CACHE_PUBLIC->world_level_index()) {
					set_world_level(iter->ext_3);
					break;
				}
				++i;
			}
		}
	}
	POOL_MANAGER->push_rank_data_pool(rank);
	return 0;
}

int Ranking_Manager::process_refresh_ranking_data(Block_Buffer &buf) {
	uint32_t type=0;
	buf.read_uint32(type);
	Ranking_Map::iterator it = rank_.find(type);
	if(it != rank_.end()) {
		Ranking rank_;
		rank_.deserialize(buf);
		it->second.ranking_ = rank_.ranking_;
		it->second.role_info_ = rank_.role_info_;
		it->second.item_info_ = rank_.item_info_;
		it->second.heroer_info_ = rank_.heroer_info_;
		it->second.tick_time = Time_Value::gettimeofday() + Time_Value(it->second.tick_sec);
		it->second.update_time = Time_Value::gettimeofday();
		it->second.is_change = true;

		if (type == RankingDef::LOCAL_PERSONAL_LEVEL_RANKING) {
			int i = 1;
			for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
				if (i == CONFIG_CACHE_PUBLIC->world_level_index()) {
					set_world_level(iter->ext_3);
					break;
				}
				++i;
			}
		}
	}
	return 0;
}

Ranking* Ranking_Manager::get_rank_by_type(int32_t type) {
	Ranking_Map::iterator it = rank_.find(type);
	if(it != rank_.end()) {
		return &it->second;
	} else {
		return 0;
	}
}

int Ranking_Manager::refresh_ranking(Ranking& rank_, int32_t type, uint32_t num) {
	switch(type){
		case RankingDef::LOCAL_GANG_RANKING: {
			Ranking_Sort_Map gang_sort_map;
			gang_sort_map.clear();
			rank_.ranking_.clear();
			Gang_Manager::Gang_ID_Map gang_id_map = GANG_MANAGER->gang_map();
			Ranking_Info ri;
			for(Gang_Manager::Gang_ID_Map::const_iterator it = gang_id_map.begin(); it != gang_id_map.end(); ++it) {
				ri.reset();
				ri.ext_2 = it->second->name;
				ri.ext_3 = it->second->level;
				ri.ext_4 = it->second->member_map.size();
				gang_sort_map.insert(std::make_pair(it->second->rank, ri));
				while(gang_sort_map.size() > num && gang_sort_map.size()) {
					gang_sort_map.erase(--gang_sort_map.end());
				}
			}

			for(Ranking_Sort_Map::iterator it = gang_sort_map.begin(); it != gang_sort_map.end(); ++it) {
				rank_.ranking_.push_back(it->second);
			}
			rank_.tick_time = Time_Value::gettimeofday() + Time_Value(rank_.tick_sec);
			rank_.update_time = Time_Value::gettimeofday();
			rank_.is_change = true;
			return 0;
			break;
		}
	}
	return -1;
}
int Ranking_Manager::update_charts_title(const int career) {
	Int_Vec type_vec;
	type_vec.push_back(RankingDef::LOCAL_PERSONAL_LEVEL_RANKING);
	type_vec.push_back(RankingDef::LOCAL_PERSONAL_FORCE_RANKING);
	type_vec.push_back(RankingDef::LOCAL_PERSONAL_CHARM_RANKING);
	type_vec.push_back(RankingDef::LOCAL_PERSONAL_FRIEND_VALUE_RANKING);
	type_vec.push_back(RankingDef::LOCAL_ARENA_SIGLE_RANKING);
	type_vec.push_back(RankingDef::LOCAL_ARENA_TEAM_RANKING);
	Title_Record_Detail::Charts_Info_Vector charts_info_vector;
	for (Int_Vec::const_iterator it = type_vec.begin(); it != type_vec.end(); ++it) {
		charts_info_vector.clear();
		int type = *it;
		Ranking_Map::iterator it = rank_.find(type);
		if(it == rank_.end()) {
			return -1;
		}
		switch (type) {
		case RankingDef::LOCAL_PERSONAL_LEVEL_RANKING:{
			int i = 1;
			Title_Record_Detail::Charts_Info charts_info;
			for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
				if (i == CONFIG_CACHE_PUBLIC->world_level_index()) {
					set_world_level(iter->ext_3);
				}
				charts_info.role_id = iter->role_id;
				TITLE_MANAGER->get_charts_title_id(type, i, career, charts_info.title_id);
				if (charts_info.title_id) {
					charts_info_vector.push_back(charts_info);
				} else {
					break;
				}
				++i;
			}
			break;
		}
		case RankingDef::LOCAL_PERSONAL_FORCE_RANKING: {
	//		Charts_Personal const &charts = charts_personal(type, career);
			int i = 1;
			Title_Record_Detail::Charts_Info charts_info;
			for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
				charts_info.role_id = iter->role_id;
				TITLE_MANAGER->get_charts_title_id(type, i, career, charts_info.title_id);
				if (charts_info.title_id) {
					charts_info_vector.push_back(charts_info);
				} else {
					break;
				}
				++i;
			}
			break;
		}
		case RankingDef::LOCAL_PERSONAL_CHARM_RANKING: {
		//		Charts_Personal const &charts = charts_personal(type, career);
				int i = 1;
				Title_Record_Detail::Charts_Info charts_info;
				for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
					charts_info.role_id = iter->role_id;
					TITLE_MANAGER->get_charts_title_id(type, i, career, charts_info.title_id);
					if (charts_info.title_id) {
						charts_info_vector.push_back(charts_info);
						if(charts_info.title_id == TITLE_ROLE_GLAMOUR_FIRST){
							charts_info.title_id = TITLE_ROLE_GLAMOUR_SECOND;
							charts_info_vector.push_back(charts_info);
						}
					} else {
						break;
					}
					++i;
				}
				break;
			}
		case RankingDef::LOCAL_PERSONAL_FRIEND_VALUE_RANKING: {
				//		Charts_Personal const &charts = charts_personal(type, career);
						int i = 1;
						Title_Record_Detail::Charts_Info charts_info;
						for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
							charts_info.role_id = iter->role_id;
							TITLE_MANAGER->get_charts_title_id(type, i, career, charts_info.title_id);
							if (charts_info.title_id) {
								charts_info_vector.push_back(charts_info);
								if(charts_info.title_id == TITLE_ROLE_FRIENDLY_FIRST){
									charts_info.title_id = TITLE_ROLE_FRIENDLY_SECOND;
									charts_info_vector.push_back(charts_info);
								}
							} else {
								break;
							}
							++i;
						}
						break;
					}
		case RankingDef::LOCAL_ARENA_SIGLE_RANKING: {
			int i = 1;
			Title_Record_Detail::Charts_Info charts_info;
			for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
				charts_info.role_id = iter->role_id;
				TITLE_MANAGER->get_charts_title_id(type, i, career, charts_info.title_id);
				if (charts_info.title_id) {
					charts_info_vector.push_back(charts_info);
				} else {
					break;
				}
				++i;
			}
			break;
		}
		case RankingDef::LOCAL_ARENA_TEAM_RANKING: {
			int i = 1;
			Title_Record_Detail::Charts_Info charts_info;
			for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
				charts_info.role_id = iter->role_id;
				TITLE_MANAGER->get_charts_title_id(type, i, career, charts_info.title_id);
				if (charts_info.title_id) {
					charts_info_vector.push_back(charts_info);
				} else {
					break;
				}
				++i;
			}
			break;
		}
		default: {
			break;
		}
		}

		TITLE_MANAGER->update_charts_title(charts_info_vector, RankingDef::Ranking_Type(type));
	}
	return 0;
}
const int Ranking_Manager::find_rank(const RankingDef::Ranking_Type type, const int64_t role_id) {
	Ranking_Map::iterator it = rank_.find(type);
	if(it != rank_.end()) {
		int i = 1;
		for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end(); ++iter) {
			if (role_id == iter->role_id) {
				return i;
			}
			++i;
		}
	}
	return -1;
}

void Ranking_Manager::find_rank_three(const RankingDef::Ranking_Type type, Role_Vec &role_id_set_) {
	role_id_set_.clear();
	Ranking_Map::iterator it = rank_.find(type);
	if(it != rank_.end()) {
		int i = 1;
		for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end() && i <= 3; ++iter) {
			role_id_set_.push_back(iter->role_id);
			++i;
		}
	}
}

void Ranking_Manager::find_rank_three(const RankingDef::Ranking_Type type, String_Vec &role_name_vec) {
	role_name_vec.clear();
	Ranking_Map::iterator it = rank_.find(type);
	if(it != rank_.end()) {
		int i = 1;
		for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end() && i <= 3; ++iter) {
			role_name_vec.push_back(iter->role_name);
			++i;
		}
	}
}

void Ranking_Manager::find_rank_role_id_name_by_career(const RankingDef::Ranking_Type type, role_id_t &role_id, std::string &role_name, const int career, const int rank) {
	role_id = 0; role_name.clear();
	Ranking_Map::iterator it = rank_.find(type);
	if(it != rank_.end()) {
		int i = 1;
		for(Ranking_Sort_List::const_iterator iter = it->second.ranking_.begin(); iter != it->second.ranking_.end() && i <= 3; ++iter) {
			if(iter->career == career) {
				if(rank == i) {
					role_id = iter->role_id;
					role_name = iter->role_name;
					break;
				}//end if rank
				++i;
			}//end if career
		}//end for
	}
}

int Ranking_Manager::world_level(void) {
	return world_level_;
}
void Ranking_Manager::set_world_level(const int world_level) {
	world_level_ = world_level;
	MSG_USER("world_level: (%d)", world_level);
	sync_world_level_to_scene();
	active_world_level();
}

void Ranking_Manager::sync_world_level_to_scene(void) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_SCENE_PUBLIC);
	buf.write_int8(0);
	buf.write_int32(world_level_);
	buf.finish_message();
	LOGIC_MONITOR->send_to_all_scene(buf);
	return;
}

void Ranking_Manager::active_world_level(void) {
	MSG_80100600 acv_msg;
	acv_msg.level = world_level_;
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
			it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
		OBJ_SEND_TO_CLIENT(acv_msg, (*it->second));
	}
}
void Ranking_Manager::active_world_level(Logic_Player *pl) {
	if (!pl) {
		return;
	}
	MSG_80100600 acv_msg;
	acv_msg.level = world_level_;
	OBJ_SEND_TO_CLIENT(acv_msg, (*pl));
}
void Ranking_Manager::trigger_daily_zero(const Time_Value &now) {
	Ranking_Config_Map &ranking_config_ = CONFIG_CACHE_RANKING->get_config();
	for(Ranking_Config_Map::iterator iter = ranking_config_.begin(); iter != ranking_config_.end(); ++iter) {
		if(iter->second.day == 0) {
			Ranking_Map::iterator it = rank_.find(iter->first);
			if(it != rank_.end()) {
				uint32_t num = iter->second.num;
				if(refresh_ranking(it->second, it->first, num)) {
					//生成排行榜
					Block_Buffer buf;
					buf.make_message(MONGO_REFRESH_RANKING_RECORD);
					buf.write_uint32(it->first);
					buf.write_uint32(num);
					buf.finish_message();
					DB_MANAGER->push_data_block(buf, 0);
				}
			} else {
				Ranking r;
				r.reset();
				rank_.insert(std::make_pair(iter->first, r));
			}
		}
	}
	update_charts_title();
}

void Ranking_Manager::trigger_daily_six(const Time_Value &now) {
	Ranking_Config_Map &ranking_config_ = CONFIG_CACHE_RANKING->get_config();
	for(Ranking_Config_Map::iterator iter = ranking_config_.begin(); iter != ranking_config_.end(); ++iter) {
		if(iter->second.day == 6) {
			Ranking_Map::iterator it = rank_.find(iter->first);
			if(it != rank_.end()) {
				uint32_t num = iter->second.num;
				if(refresh_ranking(it->second, it->first, num)) {
					//生成排行榜
					Block_Buffer buf;
					buf.make_message(MONGO_REFRESH_RANKING_RECORD);
					buf.write_uint32(it->first);
					buf.write_uint32(num);
					buf.finish_message();
					DB_MANAGER->push_data_block(buf, 0);
				}
			} else {
				Ranking r;
				r.reset();
				std::pair<Ranking_Map::iterator, bool> pair_ = rank_.insert(std::make_pair(iter->first, r));
				it = pair_.first;
				uint32_t num = iter->second.num;
				if(refresh_ranking(it->second, it->first, num)) {
					//生成排行榜
					Block_Buffer buf;
					buf.make_message(MONGO_REFRESH_RANKING_RECORD);
					buf.write_uint32(it->first);
					buf.write_uint32(num);
					buf.finish_message();
					DB_MANAGER->push_data_block(buf, 0);
				}//end if
			}
		}
	}
}

void Ranking_Manager::trigger_weekly_zero(const Time_Value &now) {
	Ranking_Config_Map &ranking_config_ = CONFIG_CACHE_RANKING->get_config();
	for(Ranking_Config_Map::iterator iter = ranking_config_.begin(); iter != ranking_config_.end(); ++iter) {
		if(iter->second.week == 1) {
			Ranking_Map::iterator it = rank_.find(iter->first);
			if(it != rank_.end()) {
				uint32_t num = iter->second.num;
				if(refresh_ranking(it->second, it->first, num)) {
					//生成排行榜
					Block_Buffer buf;
					buf.make_message(MONGO_REFRESH_RANKING_RECORD);
					buf.write_uint32(it->first);
					buf.write_uint32(num);
					buf.finish_message();
					DB_MANAGER->push_data_block(buf, 0);
				}
			} else {
				Ranking r;
				r.reset();
				std::pair<Ranking_Map::iterator, bool> pair_ = rank_.insert(std::make_pair(iter->first, r));
				it = pair_.first;
				uint32_t num = iter->second.num;
				if(refresh_ranking(it->second, it->first, num)) {
					//生成排行榜
					Block_Buffer buf;
					buf.make_message(MONGO_REFRESH_RANKING_RECORD);
					buf.write_uint32(it->first);
					buf.write_uint32(num);
					buf.finish_message();
					DB_MANAGER->push_data_block(buf, 0);
				}//end if
			}
		}
	}
}

void Ranking_Manager::sync_tick(const Time_Value &now) {
	if (!need_sync_world_level_) {
		return;
	}
//	Block_Buffer buf;
//	buf.make_message(INNER_SYNC_SCENE_PUBLIC);
//	buf.write_int8(0);
//	buf.write_int32(world_level_);
//	buf.finish_message();
//	if (0 == LOGIC_MONITOR->send_to_scene(80216001, 0, buf)) {
//		sync_world_level_to_scene();
//		need_sync_world_level_ = false;
//	}
}

void Ranking_Manager::test_refresh_ranking(uint32_t type) {
	Ranking_Config_Map &ranking_config_ = CONFIG_CACHE_RANKING->get_config();
	Ranking_Config_Map::iterator iter = ranking_config_.find(type);
	if(iter != ranking_config_.end()) {
		Ranking_Map::iterator it = rank_.find(iter->first);
		if(it != rank_.end()) {
			uint32_t num = iter->second.num;
			if(refresh_ranking(it->second, it->first, num)) {
				//生成排行榜
				Block_Buffer buf;
				buf.make_message(MONGO_REFRESH_RANKING_RECORD);
				buf.write_uint32(it->first);
				buf.write_uint32(num);
				buf.finish_message();
				DB_MANAGER->push_data_block(buf, 0);
				it->second.tick_time = Time_Value::max;
			}
		} else {
			Ranking r;
			r.reset();
			r.tick_sec = iter->second.time; //right
			r.tick_time = Time_Value::gettimeofday() + Time_Value(r.tick_sec);
			std::pair<Ranking_Map::iterator, bool> pair_ = rank_.insert(std::make_pair(iter->first, r));
			it = pair_.first;

			uint32_t num = iter->second.num;
			if(refresh_ranking(it->second, it->first, num)) {
				//生成排行榜
				Block_Buffer buf;
				buf.make_message(MONGO_REFRESH_RANKING_RECORD);
				buf.write_uint32(it->first);
				buf.write_uint32(num);
				buf.finish_message();
				DB_MANAGER->push_data_block(buf, 0);
				it->second.tick_time = Time_Value::max;
			}

		}
	}
}

void Ranking_Manager::tick(Time_Value &now) {
	Ranking_Config_Map &ranking_config_ = CONFIG_CACHE_RANKING->get_config();
	for(Ranking_Config_Map::iterator iter = ranking_config_.begin(); iter != ranking_config_.end(); ++iter) {
		if(iter->second.time) {
			Ranking_Map::iterator it = rank_.find(iter->first);
			if(it != rank_.end()) {
				if(now > it->second.tick_time) {
					uint32_t num = iter->second.num;
					if(refresh_ranking(it->second, it->first, num)) {
						//生成排行榜
						Block_Buffer buf;
						buf.make_message(MONGO_REFRESH_RANKING_RECORD);
						buf.write_uint32(it->first);
						buf.write_uint32(num);
						buf.finish_message();
						DB_MANAGER->push_data_block(buf, 0);
						it->second.tick_time = Time_Value::max;
					}
				}
			} else {
				Ranking r;
				r.reset();
				r.tick_sec = iter->second.time; //right
				r.tick_time = Time_Value::gettimeofday() + Time_Value(r.tick_sec);
				std::pair<Ranking_Map::iterator, bool> pair_ = rank_.insert(std::make_pair(iter->first, r));
				it = pair_.first;
				if(now > it->second.tick_time) {
					uint32_t num = iter->second.num;
					if(refresh_ranking(it->second, it->first, num)) {
						//生成排行榜
						Block_Buffer buf;
						buf.make_message(MONGO_REFRESH_RANKING_RECORD);
						buf.write_uint32(it->first);
						buf.write_uint32(num);
						buf.finish_message();
						DB_MANAGER->push_data_block(buf, 0);
						it->second.tick_time = Time_Value::max;
					}
				}
			}
		}
	}
	//save rank_ to DataBases
	if(now > save_tick_time) {
		for(Ranking_Map::iterator iter = rank_.begin(); iter != rank_.end(); ++iter) {
			if(iter->second.is_change) {
				iter->second.is_change = false;
//				Block_Buffer buf;
//				buf.make_message(MONGO_SAVE_RANKING_RECORD);
//				buf.write_uint32(RankingDef::SAVE_RANKING);
//				buf.write_uint8(1);
//				buf.write_int32(iter->first);
//				iter->second.serialize(buf);
//				buf.finish_message();
//				DB_MANAGER->push_data_block(buf, 0);
				Ranking *rank = POOL_MANAGER->rank_data_pool();
				if (!rank) {
					MSG_USER("POOL_MANAGER->rank_data_pool().pop() return 0");
				}
				*rank = iter->second;
				rank->type = iter->first;
				DB_MANAGER->push_save_rank(rank);
			}
		}
		save_tick_time = Time_Value::gettimeofday() + Time_Value(save_tick_sec);
	}

	sync_tick(now);
}


