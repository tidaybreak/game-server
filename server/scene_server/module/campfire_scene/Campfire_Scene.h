/*
 * Campfire_Scene.h
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */

#ifndef CAMPFIRE_SCENE_H_
#define CAMPFIRE_SCENE_H_


#include "Move_Scene.h"
#include "campfire/Scene_Campfire_Struct.h"
#include "Msg_Inner_Struct.h"

struct Campfire_Detail;

class Campfire_Scene: public Move_Scene {
	static const int AWARD_MAIL_ID = 1055;
	typedef boost::unordered_map<role_id_t, Scene_Campfire_Gain_Info> Fire_Player_Gain_Map;
	typedef boost::unordered_map<role_id_t, Time_Value> Dancing_Player_Map;
public:
	Campfire_Scene();
	virtual ~Campfire_Scene();
public:
	void reset(void);
	virtual int tick(const Time_Value &now);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	virtual int enter_scene(Mover *mover);
	virtual int exit_scene(Mover *mover, bool offline);

	virtual int client_scene_complete(Fighter &fighter);

	virtual void hook_gather_success(Mover* mover, int point_id, int material_type);
	virtual void hook_player_move(Scene_Player* player);
	void npc_init(int scene_id, int sort, int sub_sort);
	void section_init(int scene_id, int sort, int sub_sort);

	int player_req_battle(Scene_Player* player, role_id_t monster_id);
	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters);

public:
	int req_campfire_fire(Scene_Player* player, Material* material);
	int req_campfire_fire_old(Scene_Player* player, Material* material);
	int req_campfire_add_firewood(Scene_Player* player, Material* material);
	int req_campfire_add_firewood_old(Scene_Player* player, Material* material);
	int finish_campfire(void);
	void listen_gang_info_sync(Scene_Player* player);
	void req_rank_list_info(Scene_Player* player);

	void send_campfire_personal_info(Scene_Player* player);
public:
	void reset_fire(Scene_Player* player);
	void reset_firewood(Scene_Player* player);
private:
	void kick_all_player(void);
	void init_meat(void);
	void init_matchman(void);
	void init_goddess(void);
	void refresh_goddess(int point_id);
	void refresh_fire(void);
	void refresh_meat(int point_id, Coord& src_coord);
	void reset_meat(int point_id);
	int click_meat(Scene_Player* player, const int point_id);
	Material* find_material_from_point(const int point);
	// 篝火晚会采集烤肉加公会贡献
	void add_player_contribute(Scene_Player* player, const int value);
	void get_belong_fire_point_id(Coord& src_coord, int& point_id, gang_id_t& gang_id);
	void add_fire_gain_player(Scene_Player* player, int point_id, gang_id_t gang_id);
	void sync_campfire_info_to_logic(Scene_Player* player, Campfire_Detail* detail);
	void check_fire_gain(const Time_Value& now);
	void check_dancing(const Time_Value& now);
	int player_fire_gain(role_id_t role_id, bool is_gang_gain, int fire_level, const Time_Value& now);
	void refresh_fire_gain(void);
	void del_fire_buf(Scene_Player* player);
	void clear_fire_buf(Scene_Player* player);
	void add_fire_buf(Scene_Player* player, const double add_exp, const double add_fs);

	void system_announce_guide_old(Scene_Player* player, const int ann_id, std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");

private:
	void send_gang_campfire_info(Scene_Player* player);
	void send_gang_campfire_info_to_all(const gang_id_t gang_id);
	void send_campfire_personal_info_to_all();

	void system_announce(const int chat_type, const int type, std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");
	void system_announce_guide(Scene_Player* player, const int ann_id, const int district_id, std::string str1 = "", std::string str2 = "", std::string str3 = "", std::string str4 = "");
	void send_annouce_fire_lit(Scene_Player* player, Material *material, Campfire_Detail *detail, const int district_id);
	void send_annouce_fire_add(Scene_Player* player, uint32_t rank, Campfire_Detail *detail, const int district_id);
	void notice_player(Scene_Player* player, int type, std::string text);
	// 添加跳舞状态
	void add_dancing_act(Scene_Player* player);
	// 删除跳舞状态
	void del_dancing_act(Scene_Player* player);
	// 添加跳舞状态检测
	void add_dancing_player_check(Scene_Player* player);
	// 删除跳舞状态检测
	void del_dancing_player_check(Scene_Player* player);

private:
	Monster_Point_Config meat_cfg_;
	Monster_Point_Config goddess_cfg_;
	Monster_Point_Config_Map matchman_point_cfg_;
	Time_Value over_time_;
	std::vector<int> fire_point_vec_;
	Fire_Player_Gain_Map fire_gain_player_map_;
	Dancing_Player_Map dancing_player_map_;
	int64_t act_key_;
	int exp_time_;
	int dancing_time_;
	Campfire_Matchman_Map matchman_map_;
	boost::unordered_set<role_id_t> goddess_killer_set_;

	std::vector<role_id_t> finish_list_; //按照完成顺序
};



#endif /* CAMPFIRE_SCENE_H_ */
