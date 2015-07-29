/*
 * DB_Manager.cpp
 *
 *  Created on: Sep 6, 2012
 *      Author: Ti
 */

#include "DB_Manager.h"
#include "DB_Operator.h"
#include "Record_Client.h"
#include "DB_Worker.h"
#include "Pool_Manager.h"
#include "Config_Cache.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "ranking/Ranking_Struct.h"
#include "expedition_scene/Expedition_Scene_Struct.h"

DB_Manager::DB_Manager(void) {
}

DB_Manager::~DB_Manager(void) { }


int DB_Manager::init(int db_work_num) {
	DB_Worker *worker = 0;
	for (int i = 0; i < db_work_num; ++i) {
		if ((worker = POOL_MANAGER->pop_db_worker_pool()) == 0) {
			MSG_USER("db_worker_pool_.pop() return 0.");
			continue;
		}
		db_worker_vec_.push_back(worker);
	}

	return 0;
}

int DB_Manager::create_db_view(DB_View *&db_view) {
	LOG_DEBUG("db_view is created");
	PERF_MON("create_db_view");
	if ((db_view = new DB_View) == 0) {
		LOG_ABORT("new DB_View return 0.");
	}

	const Server_List_Map &list = CONFIG_CACHE->server_list_map();
	for (Server_List_Map::const_iterator it = list.begin(); it != list.end(); ++it) {
		int64_t agent_l = it->first.agent_num * 10000000000000L;
		int64_t server_l = it->first.server_num * 1000000000L;
		db_view->server_amount_map.insert(std::make_pair(it->first, agent_l + server_l + 0));
	}

	return MONGO_OPERATOR->load_db_view(db_view);
}

int DB_Manager::db_role_size(void) {
	return MONGO_OPERATOR->db_role_size();
}

int DB_Manager::start(void) {
	LOG_DEBUG("start db worker num:%d", db_worker_vec_.size());
	for (DB_Worker_Vector::iterator it = db_worker_vec_.begin(); it != db_worker_vec_.end(); ++it) {
		(*it)->thr_create();
	}

	return 0;
}

int DB_Manager::fini(void) {
	return 0;
}

int DB_Manager::push_data_block(Block_Buffer &buff, int64_t key, bool use_swap) {
	Block_Buffer *block_buffer = POOL_MANAGER->pop_db_oper_block_pool();
	if (! block_buffer) {
		//MSG_USER("block_pool_.pop() return 0.");
		return -1;
	}

	if (use_swap)
		block_buffer->swap(buff);
	else
		block_buffer->copy(&buff);

	db_worker_vec_[key % db_worker_vec_.size()]->push_data_block(block_buffer);

	return 0;
}

int DB_Manager::push_save_player_detail(Logic_Player_Detail *detail) {
	if (! detail) {
		MSG_USER("player_data == 0.");
		return -1;
	}
	return db_worker_vec_[detail->base_detail.role_id % db_worker_vec_.size()]->push_save_detail(detail);
}

int DB_Manager::push_save_rank(Ranking *rank) {
	if (! rank) {
		MSG_USER("rank == 0.");
		return -1;
	}
	return db_worker_vec_[rank->type % db_worker_vec_.size()]->push_save_rank(rank);
}

int DB_Manager::push_save_gang(Gang_DB_View *gang) {
	if (! gang) {
		MSG_USER("rank == 0.");
		return -1;
	}
	return db_worker_vec_[gang->gang_id % db_worker_vec_.size()]->push_save_gang(gang);
}

int DB_Manager::push_expedition_manager_data(Expedition_Manager_Data *expedition_manager_data) {
	if (! expedition_manager_data) {
		MSG_USER("rank == 0.");
		return -1;
	}
	return db_worker_vec_[expedition_manager_data->inter_ % db_worker_vec_.size()]->push_expedition_manager_data(expedition_manager_data);
}

bool DB_Manager::db_worker_block_empty(void) {
	bool is_empty = true;
	for (DB_Worker_Vector::iterator it = db_worker_vec_.begin(); it != db_worker_vec_.end(); ++it) {
		if (!(*it)->block_list_empty()) {
			is_empty = false;
			break;
		}
	}

	return is_empty;
}
