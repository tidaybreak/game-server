/*
 * DB_Worker.h
 *
 *  Created on: Sep 6, 2012
 *      Author: Ti
 */

#ifndef DB_WORKER_H_
#define DB_WORKER_H_

#include "Thread.h"
#include "Record_Client.h"
#include "Block_List.h"
#include "List.h"

class Block_Buffer;
struct Logic_Player_Detail;
struct Ranking;
struct Gang_DB_View;
struct Expedition_Manager_Data;

class DB_Worker: public Thread {
public:
	typedef Block_List<Thread_Mutex> Buffer_List;
	typedef List<Logic_Player_Detail *, Thread_Mutex> Detail_List;
	typedef List<Ranking *, Thread_Mutex> Ranking_List;
	typedef List<Gang_DB_View *, Thread_Mutex> Gang_List;
	typedef List<Expedition_Manager_Data *, Thread_Mutex> Expedition_Manager_Data_List;

	DB_Worker(void);
	virtual ~DB_Worker(void);

	void run_handler(void);

	bool block_list_empty(void);

	void process_list(void);

	/// 通用Block消息
	int push_data_block(Block_Buffer *buf);

	/// 保存玩家全部数据
	int push_save_detail(Logic_Player_Detail *detail);
	/// 保存排行榜信息
	int push_save_rank(Ranking *rank);
	/// 保存公会信息
	int push_save_gang(Gang_DB_View *rank);
	/// 保存远征数据
	int push_expedition_manager_data(Expedition_Manager_Data *expedition_manager_data);

	int process_block(Block_Buffer *buff);
	int process_rank_block(Ranking *buff);

private:
	Buffer_List block_list_;
	Detail_List save_detail_list_;  /// 等待保存的玩家数据列表
	Ranking_List save_rank_list_;   /// 等待保存排行榜信息数据列表
	Gang_List save_gang_list_;  	/// 等待保存公会信息数据列表
	Expedition_Manager_Data_List save_expedition_manager_data_list_; // wait for 保存远征数据列表
};

////////////////////////////////////////////////////////////////////////////////

inline int DB_Worker::push_data_block(Block_Buffer *buf) {
	if (! buf) {
		MSG_USER("buf == 0");
		return -1;
	}
	block_list_.push_back(buf);
	return 0;
}

inline int DB_Worker::push_save_rank(Ranking *rank) {
	if (! rank) {
		MSG_USER("rank == 0");
		return -1;
	}
	save_rank_list_.push_back(rank);
	return 0;
}

inline int DB_Worker::push_save_gang(Gang_DB_View *gang) {
	if (! gang) {
		MSG_USER("rank == 0");
		return -1;
	}
	save_gang_list_.push_back(gang);
	return 0;
}

inline int DB_Worker::push_expedition_manager_data(Expedition_Manager_Data *expedition_manager_data) {
	if (! expedition_manager_data) {
		MSG_USER("rank == 0");
		return -1;
	}
	save_expedition_manager_data_list_.push_back(expedition_manager_data);
	return 0;
}

inline int DB_Worker::push_save_detail(Logic_Player_Detail *detail) {
	if (! detail) {
		MSG_USER("player_data == 0");
		return -1;
	}

	save_detail_list_.push_back(detail);

	return 0;
}

#endif /* DB_WORKER_H_ */
