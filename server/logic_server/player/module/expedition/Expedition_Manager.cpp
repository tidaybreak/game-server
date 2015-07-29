/*
 * Ranking.cpp
 *
 *  Created on: 2014-05-22
 *      Author: xiaoliang
 */

#include "Expedition_Manager.h"
#include "Config_Cache.h"
#include "Logic_Monitor.h"
#include "gang/Gang_Manager.h"
#include "Msg_Inner_Enum.h"
#include "Expedition_Def.h"
#include "DB_Manager.h"
#include "DB_Def.h"
#include "Config_Cache_Expedition.h"
#include "Logic_Player.h"
#include "gang/Gang_Manager.h"
#include "Logic_Public.h"
#include "ranking/Ranking_Manager.h"
#include "Config_Cache_Item.h"

Expedition_Manager::Expedition_Manager() {
	// TODO Auto-generated constructor stub
	reset();
}

Expedition_Manager::~Expedition_Manager() {
	// TODO Auto-generated destructor stub
}

void Expedition_Manager::reset(void) {
	expedition_manager_detail_.reset();
}

void Expedition_Manager::load(void) {
	expedition_manager_detail_.load();
}

void Expedition_Manager::save(void) {
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_EXPEDITION_MANAGER);
	expedition_manager_detail_.serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
}

int Expedition_Manager::start(void) {
	load();
	return 0;
}

int Expedition_Manager::data_channle(Block_Buffer& buf_) {
	uint8_t type = 0;
	buf_.read_uint8(type);
	switch(type) {
		case EXPEDITION_CHANNEL_SERVER_OPEN_TIME: {
			Block_Buffer buf;
			buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
			buf.write_uint8(EXPEDITION_CHANNEL_SERVER_OPEN_TIME);

			buf.write_int32(CONFIG_CACHE->server_maintainer_cache().game_inter_flag);
			buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器uuid
			buf.write_int64(CONFIG_CACHE->server_maintainer_cache().game_open_time.sec());//自己的服务器open_time

			buf.finish_message();
			LOGIC_MONITOR->send_to_scene(EXPEDITION_INNER_SCENE_ID, 0, buf);
			break;
		}
		case EXPEDITION_CHANNEL_SERVER_GANG_INFO: {
			send_all_gang_info_to_scene();
			break;
		}
		case EXPEDITION_CHANNEL_SCENE_NOTICE_SERVER_GANG_NO: {
			gang_id_t gang_id = 0;
			buf_.read_int64(gang_id);

			Gang_Data_Info gdi;
			gdi.reset();
			buf_.read_uint32(gdi.line);
			gdi.coord.deserialize(buf_);

			expedition_manager_detail_.gang_data_info_map[gang_id] = gdi;
			if (expedition_manager_detail_.first_listen_time == Time_Value::zero) {
				expedition_manager_detail_.first_listen_time = Time_Value::gettimeofday();

				int num = 5;
				const Expedition_Common_Cfg *conf =
						CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(Expedition_common_conf_type::START_ANN_NUM);
				if(conf && conf->value) {
					num = conf->val_2;
					if (num < 1) {
						num = 5;
					}
				}
				Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
				if (it != LOGIC_MONITOR->player_role_id_map().end()) {
					it->second->create_ann_expedition_start(num);
				}
				for (; it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
					it->second->active_expedition_status();
				}

			}
			save();
			break;
		}
		case EXPEDITION_CHANNEL_DAILY_AWARD: {
			int64_t gang_id = 0;
			int nums[5] = {1,0};

			buf_.read_int64(gang_id);
			for (int i = 0; i < 5; ++i) {
				buf_.read_int32(nums[i]);
			}

			expedition_manager_detail_.award.erase(gang_id);

			int nums_all = 0;
			Id_ItemInfo_Map iiim;
			Item_Info item_info;
			const Item_Info_Vec *iiv = 0;
			for (int i = 0; i < 5; ++i) {
				iiv = CONFIG_CACHE_EXPEDITION->find_settle_award(i + 1);
				if (iiv) {
					for (Item_Info_Vec::const_iterator it = iiv->begin();
							it != iiv->end(); ++it) {
						item_info.reset();
						item_info = *it;
						item_info.amount = it->amount * nums[i];
						if (item_info.amount <= 0) {
							continue;
						}
						if (iiim.count(it->id)) {
							iiim[it->id].amount += item_info.amount;
						} else {
							iiim.insert(std::make_pair(it->id, *it));
						}
						nums_all += item_info.amount;
					}
				}
			}
			const Expedition_Daily_Award_Big_Data *big_award = CONFIG_CACHE_EXPEDITION->get_big_settle_award_by_nums(nums_all);
			if (big_award) {
				for (Item_Info_Vec::const_iterator it = big_award->award.begin();
						it != big_award->award.end(); ++it) {
					item_info.reset();
					item_info = *it;
					item_info.amount = it->amount * big_award->amount;
					if (item_info.amount <= 0) {
						continue;
					}
					if (iiim.count(it->id)) {
						iiim[it->id].amount += item_info.amount;
					} else {
						iiim.insert(std::make_pair(it->id, *it));
					}
				}
			}
			for (Id_ItemInfo_Map::const_iterator it = iiim.begin(); it != iiim.end(); ++it) {
				expedition_manager_detail_.award[gang_id].push_back(it->second);
			}

			save();
			break;
		}
		case EXPEDITION_CHANNEL_DEFEND_NOTICE: {
			int64_t gang_id = 0;
			int point_id = 0;
			std::string gang_name;
			std::string castle_name;
			buf_.read_int64(gang_id);
			buf_.read_int32(point_id);
			buf_.read_string(gang_name);
			buf_.read_string(castle_name);
			Gang_DB_View* gdv = GANG_MANAGER->gang_view_by_id(gang_id);
			if (gdv) {
				for (Gang_DB_View::Member_Map::const_iterator iter = gdv->member_map.begin();
						iter != gdv->member_map.end(); ++iter) {
					Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(iter->first);
					if (! player || player->level() < 35) {
						continue;
					}
					player->create_ann_expedition_defend(player->ganger_detail().gang_id, point_id, gang_name, castle_name);
					break;
				}
			}
			break;
		}
		case EXPEDITION_CHANNEL_DEMAGE_RANK_AWARD: {
			role_id_t role_id = 0;
			int rank = 0;
			buf_.read_int64(role_id);
			buf_.read_int32(rank);

			Item_Vec iv;
			const Item_Info_Vec *iiv = CONFIG_CACHE_EXPEDITION->find_demage_rank_award(rank);
			if (iiv) {
				for (Item_Info_Vec::const_iterator it_iiv = iiv->begin(); it_iiv != iiv->end(); ++it_iiv) {
					iv.push_back(Item_Detail(it_iiv->id, it_iiv->amount, it_iiv->bind));
				}
				LOGIC_PUBLIC->send_mail(2013, role_id, iv);
			} else {
				MSG_USER("Error: expedition demage rank award cant find role(%ld) award(%d)", role_id, rank);
			}
			break;
		}
		case EXPEDITION_ASK_WORLD_LEVEL: {
			RANKING_MANAGER->sync_world_level_to_scene();
			break;
		}
		case EXPE_MONSTER_TIMING_REFRESH: {
			Time_Value now;
			now.deserialize(buf_);
			if (this->expedition_manager_detail_.broadcast_announce.count(EXPE_MONSTER_TIMING_REFRESH)) {
				Date_Time dt(now), dt_r(expedition_manager_detail_.broadcast_announce[EXPE_MONSTER_TIMING_REFRESH]);
				if (dt.year() == dt_r.year() && dt.month() == dt_r.month()
						&& dt.day() == dt_r.day() && dt.hour() == dt_r.hour()
						&& dt.minute() == dt_r.minute()) {
					break;
				}
			}
			Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
			if (it != LOGIC_MONITOR->player_role_id_map().end()) {
				uint16_t len = 0;
				int point_id = 0;
				buf_.read_uint16(len);
				for (uint16_t i = 0; i < len; ++i) {
					buf_.read_int32(point_id);
					const Expedition_Map_Point_Cfg *empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
					if (!empc) {
						continue;
					}
					it->second->create_ann_expedition_timing_monster_refresh(empc->name);
				}
			}
			expedition_manager_detail_.broadcast_announce[EXPE_MONSTER_TIMING_REFRESH] = now;
			break;
		}
		case EXPE_MONSTER_TIMING_KILLED: {
//			Time_Value now;
//			now.deserialize(buf_);
//			if (this->expedition_manager_detail_.broadcast_announce.count(EXPE_MONSTER_TIMING_KILLED)) {
//				Date_Time dt(now), dt_r(expedition_manager_detail_.broadcast_announce[EXPE_MONSTER_TIMING_KILLED]);
//				if (dt.year() == dt_r.year() && dt.month() == dt_r.month()
//						&& dt.day() == dt_r.day() && dt.hour() == dt_r.hour()
//						&& dt.minute() == dt_r.minute()) {
//					break;
//				}
//			}
			Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
			if (it != LOGIC_MONITOR->player_role_id_map().end()) {
				int point_id = 0;
				buf_.read_int32(point_id);
				const Expedition_Map_Point_Cfg *empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
				if (!empc) {
					break;
				}
				Role_Info role_info;
				role_info.deserialize(buf_);
				it->second->create_ann_expedition_timing_monster_killed(empc->name, role_info);
			}
//			expedition_manager_detail_.broadcast_announce[EXPE_MONSTER_TIMING_KILLED] = now;
			break;
		}
		case EXPE_BOX_OPEND: {
			Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
			if (it == LOGIC_MONITOR->player_role_id_map().end()) {
				break;
			}

			std::string box_name;
			Role_Info role_info;
			uint16_t len = 0;
			Int_Hash_Set items;
			int item_id = 0;

			buf_.read_string(box_name);
			role_info.deserialize(buf_);
			buf_.read_uint16(len);
			for (uint16_t i = 0; i < len; ++i) {
				buf_.read_int32(item_id);
				const Item_Detail_Config*idc = CONFIG_CACHE_ITEM->find_item(item_id);
				if (!idc) {
					continue;
				}
				it->second->create_ann_expedition_box_open(role_info, box_name, idc->name);
			}
			break;
		}
		default:
			break;
	}
	return 0;
}

void Expedition_Manager::send_all_gang_info_to_scene(void) {
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	buf.write_uint8(EXPEDITION_CHANNEL_SERVER_GANG_INFO);

	buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器uuid
	Gang_Manager::Gang_ID_Map &gang_map = GANG_MANAGER->gang_map_no_const();
	buf.write_uint32(gang_map.size());
	Expedition_Gang_Rank_Info gi;
	for(Gang_Manager::Gang_ID_Map::iterator it = gang_map.begin(); it != gang_map.end(); ++it) {
		GANG_MANAGER->refresh_force(it->second);	//TODO
		gi.reset();
		gi.uuid = CONFIG_CACHE->server_flag();
		gi.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
		gi.gang_id = it->second->gang_id;
		gi.asset = it->second->fund;
		gi.create_time = it->second->create_time.sec();
		gi.force = it->second->force_total;
		gi.gang_leader_id = it->second->leader;
		gi.gang_leader_name = LOGIC_MONITOR->get_role_name_by_id(it->second->leader);
		gi.gang_name = it->second->name;
		gi.level = it->second->level;
		gi.num = it->second->member_map.size();
		gi.serialize(buf);
	}

	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(EXPEDITION_INNER_SCENE_ID, 0, buf);
}

void Expedition_Manager::send_gang_info_to_scene(const int64_t gang_id) {
	if (gang_id <= 0) {
		return ;
	}
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	buf.write_uint8(EXPEDITION_CHANNEL_SERVER_GANG_INFO_ADD);

	buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器uuid

	Gang_DB_View* gdv = GANG_MANAGER->gang_view_by_id(gang_id);
	if (!gdv) {
		return ;
	}
	buf.write_uint32(1);
	Expedition_Gang_Rank_Info gi;
	gi.uuid = CONFIG_CACHE->server_flag();
	gi.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	gi.gang_id = gdv->gang_id;
	gi.asset = gdv->fund;
	gi.create_time = gdv->create_time.sec();
	gi.force = gdv->force_total;
	gi.gang_leader_id = gdv->leader;
	gi.gang_leader_name = LOGIC_MONITOR->get_role_name_by_id(gdv->leader);
	gi.gang_name = gdv->name;
	gi.level = gdv->level;
	gi.num = gdv->member_map.size();
	gi.serialize(buf);

	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(EXPEDITION_INNER_SCENE_ID, 0, buf);
}

void Expedition_Manager::send_del_gang_info_to_scene(const int64_t gang_id) {
	if (gang_id <= 0) {
		return ;
	}
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	buf.write_uint8(EXPEDITION_CHANNEL_SERVER_GANG_INFO_DEL);

	buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器uuid
	buf.write_int64(gang_id);

	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(EXPEDITION_INNER_SCENE_ID, 0, buf);
	expedition_manager_detail_.gang_data_info_map.erase(gang_id);
	save();
}

void Expedition_Manager::listen_gang_leader_change(const Base_Detail &detail) {
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	buf.write_uint8(EXPEDITION_CHANGE_LEADER);

	buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器uuid
	detail.serialize(buf);

	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(EXPEDITION_INNER_SCENE_ID, 0, buf);
}

uint32_t Expedition_Manager::get_line_by_gang_id(int64_t gang_id) {
	Int64_Gang_Data_Info_Map::iterator it = expedition_manager_detail_.gang_data_info_map.find(gang_id);
	if(it != expedition_manager_detail_.gang_data_info_map.end()) {
		return it->second.line;
	}
	return 0;
}

const Coord *Expedition_Manager::get_coord_by_gang_id(int64_t gang_id) {
	Int64_Gang_Data_Info_Map::iterator it = expedition_manager_detail_.gang_data_info_map.find(gang_id);
	if(it != expedition_manager_detail_.gang_data_info_map.end()) {
		return &it->second.coord;
	}
	return 0;
}
