/*
 * Honor_Arena_Scene.h
 *
 *  Created on: 2015年3月16日
 *      Author: lys
 */

#ifndef HONOR_ARENA_SCENE_H_
#define HONOR_ARENA_SCENE_H_


#include "Move_Scene.h"

struct Honor_Arena_Player_Info;
class Item_Info;

class Honor_Arena_Scene: public Move_Scene{
public:
	Honor_Arena_Scene(void);
	~Honor_Arena_Scene(void);
public:
	void reset(void);
	void npc_init(int scene_id, int sort, int sub_sort);
	void set_logic_uuid(const int64_t uuid);

	virtual int tick(const Time_Value &now);

	virtual int enter_scene(Mover *mover);
	virtual int exit_scene(Mover *mover, bool offline);
	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int client_scene_complete(Fighter &fighter);

	virtual void hook_player_enter_battle(Scene_Player* player);
	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters);
public:
	int add_show_reward(const role_id_t role_id, const int type);
private:
	void kick_all_player(void);
	void act_over(void);
	void broadcast_boss_info(void);
	void broadcast_rank_info(void);
	void broadcast_act_time(void);
	void force_end_battle(void);
	void handle_rank_ann(Honor_Arena_Player_Info* first_info, Honor_Arena_Player_Info* player_info, const int src_rank);
	void broadcast_fight_report(const role_id_t role1, std::string name1, const role_id_t role2, std::string name2,
			const int type, const int value, std::vector<Item_Info> items);
	void add_cure_reduce_buffer(Scene_Player* player);
	void del_cure_reduce_buffer(Scene_Player* player);
public:
	void announce_to_all(const int ann_id, const role_id_t role_id1, std::string role_name1, const role_id_t role_id2, std::string role_name2,
			std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");
	void announce_to_guild(const int64_t gang_id, const int ann_id, const role_id_t role_id, std::string role_name, std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");
	void announce_to_scene(const int ann_id, const role_id_t role_id, std::string role_name, std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");
private:
	int64_t logic_uuid_;
	int act_state_;
	int act_result_;                  // 0：击杀boss失败， 1：普遍击杀boss 2：快速击杀boss
	Time_Value act_end_time_;
	Time_Value balance_time_;
	int cure_reduce_buff_id_;
};




#endif /* HONOR_ARENA_SCENE_H_ */
