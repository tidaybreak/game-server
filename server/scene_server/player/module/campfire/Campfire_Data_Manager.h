/*
 * Campfire_Data_Manager.h
 *
 *  Created on: 2014年8月14日
 *      Author: root
 */

#ifndef CAMPFIRE_DATA_MANAGER_H_
#define CAMPFIRE_DATA_MANAGER_H_


#include "Game_Typedef.h"
#include "Singleton.h"
#include "campfire/Campfire_Struct.h"
#include "campfire/Scene_Campfire_Struct.h"
#include "Msg_Inner_Struct.h"

struct Campfire_Detail;
struct Scene_Campfire_Info;

class Campfire_Data_Manager{
public:
	Campfire_Data_Manager(void);
	~Campfire_Data_Manager(void);
public:
	void load_campfire(Block_Buffer& buf);
	Campfire_Detail* get_campfire_detail_from_point(const int point_id);
	Campfire_Detail* get_campfire_detail_from_gang(const int64_t gang_id);
	int add_campfire_detail(Campfire_Detail& detail);
	Scene_Campfire_Info* get_player_campfire_info(role_id_t role_id);
	Player_Campfire_Map get_campfire_finished_player(void);
	void max_rank_plus_plus();
	void max_rank_minus_one(); //只有测试使用到
	uint32_t max_rank();
	void check_if_came_in(Scene_Player *player);
public:
	// 篝火晚会开始
	int cmd_campfire_activate(Block_Buffer& buf);
	// 篝火晚会结束
	int cmd_campfire_finish(void);
	// 同步玩家
	int save_campfire_player_info(role_id_t player_id);
	void add_player(const Scene_Campfire_Info &info);
public:
	// 篝火晚会剩余时间
	int get_campfire_remain_time(void);
private:
	Campfire_Detail_Map campfire_info_;
	Player_Campfire_Map player_campfire_map_;
	Time_Value campfire_over_time_;
	uint32_t max_rank_; //当前有多少名完成了
	boost::unordered_set<role_id_t> players_came_in;
};
typedef Singleton<Campfire_Data_Manager> Campfire_Data_Manager_Single;
#define CAMPFIRE_DATA_MANAGER (Campfire_Data_Manager_Single::instance())



#endif /* CAMPFIRE_DATA_MANAGER_H_ */
