/*
 * Arena_Struct.h
 *
 *  Created on: 2014年5月17日
 *      Author: lys
 */

#ifndef ARENA_STRUCT_H_
#define ARENA_STRUCT_H_

#include "Server_Struct.h"
#include "Fighter_Detail.h"
#include "Msg_Struct.h"
#include "Base_Detail.h"
#include "heroer/Logic_Heroer_Struct.h"

enum Arena_Announce_ID{
	ARENA_ANNOUNCE_ID_LOCAL_AWARD		= 1340,				// 竞技场奖励发布广播
	ARENA_ANNOUNCE_ID_LOCAL_RISE		= 1341,				// 竞技场晋级广播
	ARENA_ANNOUNCE_ID_ENTER_FIFTY 	= 1342,				// 排名杀进前50的广播
	ARENA_ANNOUNCE_ID_ENTER_THIRTY 	= 1343,				// 排名杀进前30的广播
	ARENA_ANNOUNCE_ID_ENTER_TEN 		= 1344,				// 排名杀进前10的广播
	ARENA_ANNOUNCE_ID_ENTER_THREE 	= 1345,				// 第3名广播
	ARENA_ANNOUNCE_ID_ENTER_TWO 		= 1346,				// 第3名广播
	ARENA_ANNOUNCE_ID_ENTER_ONE 		= 1347,				// 第3名广播
};

struct Arenaer_Detail:public Detail{

	Arenaer_Detail(void);
	virtual ~Arenaer_Detail(void);

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	int load_arena_fighter_detail(void);
	int group;
	int seg;
	int score;
	int rise_fail;
	int rise_win;
	int rise_lose;
	int rank;
	int inner_rank;
	int king_time;
	int enter_num;
	int pay_times;
	int buy_pay_times;
	int streak_win;								//连胜次数
	int streak_lose;
	int compensate_times;			//补偿剩余次数
	int three_day_rank;
	int seven_day_rank;
	Time_Value cd;
	std::vector<Arena_Battle_Report> report_vec;
	Time_Value match_time;
	role_id_t match_player_1;
	role_id_t match_player_2;
	role_id_t match_player_3;
	int8_t match_dlv_1;			//难度 （0 普通 1困难 2神级）
	int8_t match_dlv_2;
	int8_t match_dlv_3;
	int fight_times;
	int be_worship;             // 被膜拜次数

	friend bool operator>(const Arenaer_Detail &detail1 , const Arenaer_Detail &detail2);
};

struct Arena_Season_Result{
	Arena_Season_Result(void);
	~Arena_Season_Result(void);

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	role_id_t player_id;
	int level;
	int group_index;
	int rank;
	int inner_rank;
};
typedef boost::unordered_map<role_id_t, Arena_Season_Result> Arena_Season_Result_Map;

struct Arena_Season_Detail:public Detail{

	Arena_Season_Detail(void);
	virtual ~Arena_Season_Detail(void);

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	void clear_season_rank(const role_id_t role_id);

	int get_season_rank(const role_id_t role_id) const;

	void get_season_result(const role_id_t role_id, int &rank, int &group_index, int &level);

	int8_t season_status;
	Time_Value season_begin;
	Time_Value season_finish;
	Arena_Season_Result_Map season_result_map;
};


struct Player_Rank_Info{
	role_id_t role_id;
	std::string role_name;
	std::string server_name;
	int level;
	int career;
	int gender;
	uint32_t head_id;						//头像ID
	int fight;
	int score;
	int group;
	int seg;
	int rank;
	int inner_rank;
	int king_time;
	int fight_times;
	int be_worship;

	Player_Rank_Info(void);
	void reset(void);
	void season_reset(void);
	friend bool operator>(const Player_Rank_Info &detail1 , const Player_Rank_Info &detail2);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
};

struct Arena_Reward_Info{
	int rank;
	Time_Value rank_time;
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	Arena_Reward_Info(void);
	void reset(void);
};

//
struct Arena_Load_Status{
	Arena_Load_Status(void):role_id(0), complete(false), group(0), seg(0),
			score(0), rank(0), difficult_lv(0){

	}
	Arena_Load_Status(const role_id_t id):role_id(id), complete(false), group(0), seg(0),
			score(0), rank(0), difficult_lv(0){

	}
	void reset(void){
		role_id = 0;
		complete = false;
		group = 0;
		seg = 0;
		score = 0;
		role_name = "";
		server_name = "";
		rank = 0;
		difficult_lv = 0;
	}
	bool is_complete(void){
		return complete;
	}
	void load_complete(const role_id_t id){
		if(id == role_id){
			complete = true;
		}
	}
	role_id_t role_id;
	bool complete;
	int group;
	int seg;
	int score;
	std::string role_name;
	std::string server_name;
	int rank;
	int8_t difficult_lv;
};

struct Arena_Match_Status{
	Arena_Match_Status(void);
	Arena_Match_Status(Time_Value time, std::vector<role_id_t> match_id);
	Arena_Match_Status(Time_Value time, std::vector<Arena_Load_Status> match_status, int8_t ref = 0);
	void reset(void);
	bool is_timeout(const Time_Value &now);
	bool is_complete(void);
	void add_one_match(role_id_t player, bool complete = false);
	void set_timeout(Time_Value time);

	void load_complete(const role_id_t id);

	int get_match_pos(const role_id_t match_id);
	int8_t refresh;
	Time_Value match_time;
	std::vector<Arena_Load_Status> match_players;
};
typedef boost::unordered_map<role_id_t, Arena_Match_Status> Arena_Match_Status_Map;


typedef std::map<int, Role_Id_Set, std::less<int> > Arena_Force_Set_Map;
struct Arena_Seg{
	int seg_key;
	Role_Id_Set seg_set;
	Arena_Force_Set_Map force_set_map;

	Arena_Seg(void);
	void reset(void);
	void add_player(role_id_t role_id, int force_lv);
	void del_player(role_id_t role_id, int force_lv);
	void force_change(role_id_t role_id, int old_force_lv, int new_force_lv);
};
typedef std::map<int, Arena_Seg, std::less<int>> Arena_Seg_Map;

struct Arena_Match_Player_Data{
	Arena_Match_Player_Data(void);
	~Arena_Match_Player_Data(void);

	void reset(void);

	Base_Detail base_detail;
	Fighter_Detail fighter_detail;
	Heroer_Detail heroer_detail;
};

struct Arena_Match_P{
	Arena_Match_P(void);
	Arena_Match_P(const role_id_t r_id, const int8_t d_lv);
	~Arena_Match_P(void);

	void reset(void);

	role_id_t role_id;
	int8_t difficult_lv;
};

#endif /* ARENA_STRUCT_H_ */
