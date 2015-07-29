/*
 * Title_Manager.cpp
 *
 *  Created on: 2014年7月25日
 *      Author: Linqiyou
 */

#include "Title_Manager.h"
#include "Logic_Monitor.h"
#include "Config_Cache_Title.h"
#include "Logic_Player.h"
#include "DB_Def.h"
#include "title/Title_Def.h"

Title_Manager::Title_Manager(void) { }

Title_Manager::~Title_Manager(void) { }

void Title_Manager::start(void) {
	reset();
	load_detail();

	title_record_detail_.title_role_map.clear();
	Title_Record_Detail::Charts_Title_Map::iterator m_iter;
	for(m_iter = title_record_detail_.charts_title_map.begin(); m_iter != title_record_detail_.charts_title_map.end(); m_iter++){
		Title_Record_Detail::Charts_Info_Vector::iterator v_iter;
		for(v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); v_iter++) {
			title_record_detail_.title_role_map[(*v_iter).title_id].insert((*v_iter).role_id);
		}
	}
}

int Title_Manager::load_detail(void) {
	return title_record_detail_.load();
}

int Title_Manager::save_detail(void) {
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_TITLE_INFO);
	title_record_detail_.serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

void Title_Manager::reset(void) {
	title_record_detail_.reset();
}

int Title_Manager::update_charts_title(Title_Record_Detail::Charts_Info_Vector &charts_info_vector, RankingDef::Ranking_Type charts_type) {
	Title_Record_Detail::Charts_Title_Map::iterator iter = title_record_detail_.charts_title_map.find(charts_type);
	if (iter != title_record_detail_.charts_title_map.end()) {
		charts_title_process(iter->second, 2);		//去除玩家旧的称号

		iter->second = charts_info_vector;
	} else {
		title_record_detail_.charts_title_map.insert(std::make_pair(charts_type, charts_info_vector));
	}

	charts_title_process(charts_info_vector, 1);	//添加新的称号

	save_detail();

	title_record_detail_.title_role_map.clear();
	Title_Record_Detail::Charts_Title_Map::iterator m_iter;
	for(m_iter = title_record_detail_.charts_title_map.begin(); m_iter != title_record_detail_.charts_title_map.end(); m_iter++){
		Title_Record_Detail::Charts_Info_Vector::iterator v_iter;
		for(v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); v_iter++) {
			title_record_detail_.title_role_map[(*v_iter).title_id].insert((*v_iter).role_id);
		}
	}
	return 0;
}

int Title_Manager::charts_title_process(Title_Record_Detail::Charts_Info_Vector &charts_info_vector, int type) {
	for (Title_Record_Detail::Charts_Info_Vector::iterator iter = charts_info_vector.begin();
			iter != charts_info_vector.end(); ++iter) {
		//if (iter->role_id == 0)
		//	continue;

		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(iter->role_id);
		if (player != 0) {
			if (type == 1) {
				player->add_rank_title(iter->title_id);
			} else if (type == 2) {
				player->erase_title(iter->title_id);
			}
		}
	}
	return 0;
}

int Title_Manager::get_charts_title_id(int charts_type, int rank, int career, int &title_id) {
	title_id = 0;

//	if (career == 0) {			//排行榜总榜，不按职业分
		switch (charts_type) {
		case RankingDef::LOCAL_PERSONAL_FORCE_RANKING:
			if (rank == 1) {
				title_id = TITLE_ROLE_COMBAT_FIRST;
			} else if (rank >=2 && rank <= 10) {
				title_id = TITLE_ROLE_COMBAT_SECOND;
			}
			break;
		case RankingDef::LOCAL_PERSONAL_LEVEL_RANKING:
			if (rank == 1) {
				title_id = TITLE_ROLE_LEVEL_FIRST;
			} else if (rank >=2 && rank <= 10) {
				title_id = TITLE_ROLE_LEVEL_SECOND;
			}
			break;
		case RankingDef::LOCAL_ARENA_SIGLE_RANKING:
			if (rank == 1) {
				title_id = TITLE_TEAM_SPORTS_RANK_FIRST;
			} else if (2 == rank) {
				title_id = TITLE_TEAM_SPORTS_RANK_SECOND;
			} else if (3 == rank) {
				title_id = TITLE_TEAM_SPORTS_RANK_THIRD;
			} else if (rank >=4 && rank <= 10) {
				title_id = TITLE_TEAM_SPORTS_RANK_FOUR;
			}
			break;
		case RankingDef::LOCAL_ARENA_TEAM_RANKING:
			if (rank == 1) {
				title_id = TITLE_ARENA_RANK_FIRST;
			} else if (rank >=2 && rank <= 10) {
				title_id = TITLE_ARENA_RANK_SECOND;
			}
			break;
		case RankingDef::LOCAL_PERSONAL_CHARM_RANKING:
				if (rank == 1) {
					title_id = TITLE_ROLE_GLAMOUR_FIRST;
				} else if (rank >=2 && rank <= 10) {
					title_id = TITLE_ROLE_GLAMOUR_SECOND;
				}
				break;
		case RankingDef::LOCAL_PERSONAL_FRIEND_VALUE_RANKING:
						if (rank == 1) {
							title_id = TITLE_ROLE_FRIENDLY_FIRST;
						} else if (rank >=2 && rank <= 10) {
							title_id = TITLE_ROLE_FRIENDLY_SECOND;
						}
						break;
		default:
			title_id = 0;
			break;
		}
//	}
	return 0;
}

int Title_Manager::add_change_career_title(role_id_t role_id, int career) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (player != 0) {
//		int title_id = 0;
//		switch (career) {
//		case 1:
//			title_id = TITLE_HUANGMAN_MENTU;
//			break;
//		case 2:
//			title_id = TITLE_XUANYU_MENTU;
//			break;
//		case 3:
//			title_id = TITLE_LIANHUA_MENTU;
//			break;
//		case 4:
//			title_id = TITLE_MIAOBING_MENTU;
//			break;
//		}
//		player->add_title(title_id);
	}
	return 0;
}

int Title_Manager::add_friend_title(role_id_t role_id, int friend_amount) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (player != 0) {
//		if (friend_amount >= 20) {
//			player->add_title(TITLE_FRIEND_20);
//		}
//		if (friend_amount >= 50) {
//			player->add_title(TITLE_FRIEND_50);
//		}
//		if (friend_amount >= 200) {
//			player->add_title(TITLE_FRIEND_200);
//		}
	}
	return 0;
}

int Title_Manager::add_gang_war_title(const role_id_t leader_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(title_record_detail_.gang_war_first_leader);
	if (player != 0) {
		player->erase_title(TITLE_GANG_WAR_FIRST);
	}
	title_record_detail_.gang_war_first_leader = leader_id;
	player = LOGIC_MONITOR->find_logic_player_by_role_id(leader_id);
	if (player != 0) {
		player->add_rank_title(TITLE_GANG_WAR_FIRST);
	}
	save_detail();
	return 0;
}

int Title_Manager::title_deal(role_id_t role_id, int title_id, int type, int equip) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (player) {
		if (type == 1) {
			return player->add_title(title_id);
		} else if (type == 2) {
			return player->erase_title(title_id);
		} else {
			return -1;
		}
	}

//	Block_Buffer buf;
//	buf.make_message(INNER_SAVE_TITLE_INFO);
//	MSG_700353 msg;
//	msg.reset();
//	msg.role_id = role_id;
//	msg.title_id = title_id;
//	msg.type = type;
//	msg.equip = equip;
//	msg.serialize(buf);
//	buf.finish_message();
//	return DB_MANAGER_INSTANCE->push_data_block(buf, role_id);
	return 0;
}

int Title_Manager::add_equip_title(int title_id, Int_Vec &equip_title) {
	int title_type = get_title_type(title_id);
	for (Int_Vec::iterator iter = equip_title.begin();
			iter != equip_title.end(); ++iter) {
		if (get_title_type(*iter) == title_type) {
			equip_title.erase(iter);
			break;
		}
	}
	equip_title.push_back(title_id);
	return 0;
}

int Title_Manager::erase_equip_title(int title_id, Int_Vec &equip_title) {
	for (Int_Vec::iterator iter = equip_title.begin();
			iter != equip_title.end(); ++iter) {
		if (*iter == title_id) {
			equip_title.erase(iter);
			break;
		}
	}
	return 0;
}

int Title_Manager::get_title_type(int title_id) {
	const Title_Cfg_Detail * title_info = CONFIG_CACHE_TITLE->find_title(title_id);
	if (!title_info)
		return 0;

	return 0;
}

bool Title_Manager::handle_title(role_id_t role_id, int title_id) {
	Title_Record_Detail::Title_Role_Map::iterator iter = title_record_detail_.title_role_map.find(title_id);
	if(iter == title_record_detail_.title_role_map.end()) {
		return true;
	}else {
		if(iter->second.count(role_id)) {
			return true;
		} else {
			return false;
		}
	}

	return true;
}

Title_Record_Detail &Title_Manager::title_record_detail(void) {
	return title_record_detail_;
}

