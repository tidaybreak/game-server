/*
 * DB_Manager.h
 *
 *  Created on: Sep 6, 2012
 *      Author: Ti
 */

#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include "Game_Typedef.h"

class DB_View;
class DB_Worker;
struct Logic_Player_Detail;
struct Ranking;
struct Gang_DB_View;
struct Expedition_Manager_Data;

class DB_Manager {
public:
	typedef std::vector<DB_Worker *> DB_Worker_Vector;

	DB_Manager(void);
	virtual ~DB_Manager(void);
	DB_Manager(const DB_Manager &);

	int init(int db_work_num);

	int start(void);

	int fini(void);

	int create_db_view(DB_View *&db_view);

	int db_role_size(void);

	/// Buffer数据操作消息
	int push_data_block(Block_Buffer &buf, int64_t key = 0, bool use_swap = true);
	/// 保存玩家全部数据
	int push_save_player_detail(Logic_Player_Detail *detail);
	/// 保存排行榜信息
	int push_save_rank(Ranking *rank);
	/// 保存公会信息
	int push_save_gang(Gang_DB_View *gang);
	/// 保存远征数据
	int push_expedition_manager_data(Expedition_Manager_Data *expedition_manager_data);

	bool db_worker_block_empty(void);

private:

	const DB_Manager &operator=(const DB_Manager &);

private:
	DB_Worker_Vector db_worker_vec_;
};
typedef Singleton<DB_Manager> DB_Manager_Single;
#define DB_MANAGER    (DB_Manager_Single::instance())

////////////////////////////////////////////////////////////////////////////////


#endif /* DB_MANAGER_H_ */
