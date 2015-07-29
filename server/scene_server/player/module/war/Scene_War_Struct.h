/*
 * Scene_War_Struct.h
 *
 *  Created on: 2014年7月3日
 *      Author: root
 */

#ifndef SCENE_WAR_STRUCT_H_
#define SCENE_WAR_STRUCT_H_

#include "Game_Header.h"
#include "Game_Typedef.h"

#define LEAGUE_COUNT 2
#define MAX_WAR_SCENE_PLAYER_NUM 30

#define WEIGHT_GANG  10000
#define WEIGHT_SIDE  1000
#define WEIGHT_FORCE 100

#define MAX_WAR_SCENE_MERIT 1200

#define WAR_SCENE_FORBID_TIME 300

#define WAR_SCENE_LEVEL_LIMIT 30

#define WAR_SCENE_ZONE_STAGE 5

enum HONOUR_CAMP{
	LIGHT_LEAGUE = 1,
	DARK_LEAGUE = 2,
};

enum War_Scene_Notice{
	WAR_SCENE_NOTICE_BE_KILL = 1,
	WAR_SCENE_NOTICE_KILL    = 2,
	WAR_SCENE_NOTICE_MINE    = 3,
	WAR_SCENE_NOTICE_WIN		 = 4,
	WAR_SCENE_NOTICE_LOSE    = 5,
	WAR_SCENE_NOTICE_ASSISTS = 6,
	WAR_SCENE_NOTICE_BLESS   = 7,
	WAR_SCENE_NOTICE_REDUCE  = 8,
};

enum War_Scene_Announce{
	WAR_SCENE_ANNOUNCE_END_STREAK_WIN  	= 1205,
	WAR_SCENE_ANNOUNCE_KILL_GUARD  	   = 1206,
	WAR_SCENE_ANNOUNCE_KILL_L_OFFICER   = 1207,
	WAR_SCENE_ANNOUNCE_KILL_D_OFFICER  	= 1208,
	WAR_SCENE_ANNOUNCE_WAR_CLOSE  	   = 1209,
	WAR_SCENE_ANNOUNCE_STREAK_WIN_3  	= 1211,
	WAR_SCENE_ANNOUNCE_STREAK_WIN_4  	= 1212,
	WAR_SCENE_ANNOUNCE_STREAK_WIN_5  	= 1213,
	WAR_SCENE_ANNOUNCE_STREAK_WIN_6  	= 1214,
	WAR_SCENE_ANNOUNCE_STREAK_WIN_7  	= 1215,
	WAR_SCENE_ANNOUNCE_STREAK_WIN_8  	= 1216,
	WAR_SCENE_ANNOUNCE_L_OFFICE_REVIVE  = 1217,
	WAR_SCENE_ANNOUNCE_D_OFFICE_REVIVE  = 1218,
	WAR_SCENE_ANNOUNCE_WAR_L_WIN  		= 1219,
	WAR_SCENE_ANNOUNCE_WAR_D_WIN  		= 1220,
	WAR_SCENE_ANNOUNCE_SPE_MINE_REFRESH = 1223,
};

struct Scene_War_Rank_Info{
	typedef boost::unordered_map<role_id_t, int> Hero_Hp_Map;
	role_id_t role_id;
	std::string role_name;
	std::string server_name;
	int league;
	int score;
	int total_score;
	double src_hp;
	double hp;
	Hero_Hp_Map hero_hp_map;
	int kill_num;
	int die_num;
	int assists_num;
	int streak_win_num;
	int max_streak_win_num;
	bool add_streak_lose_num;
	int streak_lose_num;
	int merit_coe;
	int merit;
	int total_merit;
	int chest_pieces_coe;
	int chest_pieces;
	int rep_coe;
	int rep;
	int arena_coin;
	int is_pull_mine;
	int heal_num;
	role_id_t last_attack_role_id;
	int revive;
	Time_Value revive_time;
	Time_Value safe_time;
	Time_Value offline_kick_time;
	Time_Value inactive_kick_time;
	bool notice_kick_time;
	Scene_War_Rank_Info(void);
	void reset(void);
	int get_hero_hp(role_id_t role_id);
	int add_score(int score);
	int dec_score(int score);
	int add_merit(int merit);
	int add_piece(int piece);
	int add_rep(int rep);
	int add_arean_coin(int value);
};

struct Honour_Player{
	role_id_t role_id;
	int level;
	int force;
	role_id_t gang_id;
	int total_merit;
	int remain_income_num;
	int income_rate;
	int league;
	int remain_time;
	int act_time;
	std::string server_name;
	role_id_t honor_field_id;

	Honour_Player(void);
	void reset(void);
};
typedef boost::unordered_map<role_id_t, Honour_Player> Honour_Player_Map;

struct Honour_Field{
	role_id_t id;
	int remain_time;
	int act_time;
	int zone_id;
	int field_player_num;
	int league_num[LEAGUE_COUNT];
	role_id_t league_force[LEAGUE_COUNT];
	boost::unordered_set<role_id_t> player_set;
	boost::unordered_set<role_id_t> league_gang[LEAGUE_COUNT];
	int enter_weight;
	int enter_league;
	Honour_Field(void);
	void reset(void);
	bool check_enter_weight(const Honour_Player* honour_player);
	int add_player(Honour_Player* honour_player);
	int del_player(Honour_Player* honour_player);
};
typedef boost::unordered_map<role_id_t, Honour_Field> Honour_Field_Map;
typedef boost::unordered_map<role_id_t, Honour_Field*> Honour_Field_P_Map;

struct Honour_Zone{
	int id;
	int zone_player_num;
	boost::unordered_set<role_id_t> honour_field_set;
	Honour_Field_P_Map honour_field_p_map;
	Honour_Zone(void);
	void reset(void);
	int add_player(Honour_Player* honour_player);
	int del_player(Honour_Player* honour_player);
	int add_field(Honour_Field* honour_field);
	int del_field(Honour_Field* honour_field);
	Honour_Field* get_best_honour_field(Honour_Player* honour_player);

};
typedef boost::unordered_map<int, Honour_Zone> Honour_Zone_Map;



#endif /* SCENE_WAR_STRUCT_H_ */
