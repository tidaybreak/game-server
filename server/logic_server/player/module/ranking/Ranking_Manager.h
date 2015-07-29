/*
 * Ranking_Manager.h
 *
 *  Created on: 2014-05-22
 *      Author: xiaoliang
 */

#ifndef RANKING_MANAGER_H_
#define RANKING_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Ranking_Struct.h"
#include "title/Title_Struct.h"
#include "Ranking_Def.h"

struct Item_Detail;
struct Logic_Player;

class Ranking_Manager {

public:
	Ranking_Manager();
	virtual ~Ranking_Manager();
	void reset(void);

	int start(void);

	void test_refresh_ranking(uint32_t type);
	void tick(Time_Value &now);
	void trigger_daily_zero(const Time_Value &now);
	void trigger_daily_six(const Time_Value &now);
	void trigger_weekly_zero(const Time_Value &now);

	void sync_tick(const Time_Value &now);

	Ranking* get_rank_by_type(int32_t type);

	int process_refresh_ranking_data(Block_Buffer &buf);
	int process_refresh_ranking_data_p(Ranking *rank);

	int monitor_equipment_change(role_id_t role_id, const Item_Detail* item);
	int monitor_equipment_off(role_id_t role_id, int32_t item_id, int32_t item_index);

	//特殊处理的排行榜(不需要读数据库)
	int refresh_ranking(Ranking& rank_, int32_t type, uint32_t num);

	//接口处理
	int update_charts_title(const int career = 0);
	const int find_rank(const RankingDef::Ranking_Type type, const int64_t role_id);
	void find_rank_three(const RankingDef::Ranking_Type type, Role_Vec &role_id_set_);//前三名角色ID
	void find_rank_three(const RankingDef::Ranking_Type type, String_Vec &role_name_vec);//前三名角色名称
	void find_rank_role_id_name_by_career(const RankingDef::Ranking_Type type, role_id_t &role_id, std::string &role_name, const int career, const int rank = 1);//按职业返回第N名角色ID和角色名


	int world_level(void);
	void set_world_level(const int world_level);
	void sync_world_level_to_scene(void);
	void active_world_level(void);
	void active_world_level(Logic_Player *pl);
private:
	Ranking_Map rank_;
	int save_tick_sec;
	Time_Value save_tick_time;
	int world_level_;
	bool need_sync_world_level_;
};


typedef Singleton<Ranking_Manager> Ranking_Manager_Single;
#define RANKING_MANAGER    (Ranking_Manager_Single::instance())

#endif /* RANKING_H_ */
