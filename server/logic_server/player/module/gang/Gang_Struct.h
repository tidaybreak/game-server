/*
 * Gang_Struct.h
 *
 *  Created on: Apr 24, 2014
 *      Author: caijianwei
 */

#ifndef GANG_STRUCT_H_
#define GANG_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"
#include "Gang_Def.h"

/*
玩家公会技能信息
*/
struct Ganger_Skill_Detail {
	Ganger_Skill_Detail(void) { reset(); };
	void reset(void);
	void serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);

	int32_t skill_id;/*公会技能ID*/
};
typedef std::map<int, Ganger_Skill_Detail> Ganger_Skill_Map;

struct Ganger_Detail : public Detail {

	struct Affair{
		int8_t type;
		int8_t result;
		int64_t extra_id;
		Time_Value expire;
		std::string content;

		Affair() { reset(); }
		void reset() {
			type = 0;
			result = 0;
			extra_id = 0;
			expire = Time_Value::zero;
			content = "";
		}
	};

	typedef std::vector<Affair> Affair_Vec;
	typedef std::vector<Gang_Invite_Info> Invite_Info_Vec;

	Ganger_Detail();
	virtual ~Ganger_Detail();

	void reset(void);
	int serialize(Block_Buffer &buf) const;
	int deserialize(Block_Buffer &buf);
	int load(void);
	int save(void);
	void set_quit(gang_id_t quit_gang_id);
	void set_gang_war_award_get_times_by_day(uint16_t gw);
	void offline_set_quit();

	int gang_war_serialize(Block_Buffer &buf) const;
	int gang_war_deserialize(Block_Buffer &buf);

	gang_id_t gang_id;
	gang_id_t last_gang_id;		// 上次公会ID
	role_id_t role_id;
	int cur_contribute;	// 当前剩余贡献
	uint32_t total_contribute;
	uint32_t achi_contribute;
	uint32_t day_contribute;
	uint32_t contrib_of_week;
	int32_t day_gold_contrib_times;
	int32_t day_diam_contrib_times;
	int32_t last_rank;
	int32_t authority;
	Time_Value join_time;
	Time_Value leave_time;
	int8_t headship;
	int8_t receive_bless_times;
	//int32_t remain_draw_times;
	int today_has_draw_times;
	Affair_Vec affairs;
	Role_Vec invites;
	Role_Id_Set apply_ids;
	Ganger_Skill_Map skill_map;	// studied skills
	Int_Vec draw_awards;

	std::map<int, double> skill_pro_val;		// 模块加成的属性，用于外部接口，不存

	/*公会战*/
	int gang_war_kill_num;//击杀数
	int gang_war_hold_tower;//抢点
	uint32_t gang_war_contribute;//贡献获得
	int gang_war_merit;//功勋获得
	int gang_war_total_merit;//累计功勋
	int8_t gang_war_camp;//阵营：0红1蓝
	Time_Value gang_war_attend_time; // 参加公会战的时间
	int64_t gang_war_attend_gang_id; // 当天参加公会战的公会所在的对战图上的点索引
	int64_t gang_war_attend_gang_id2;
	uint16_t gang_war_award_get_times_by_day;

	// 公会修行
	int practice_times_;
	int practice_times_ttl_;
	Time_Value practice_start_time_;
};

struct Gang_Member_Detail {
	Gang_Member_Detail(void) { reset(); };
	~Gang_Member_Detail() {};
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	int64_t role_id;/**/
	uint32_t week_contribute;/*周贡献*/
	uint32_t total_contribute;/*总贡献*/
	uint32_t fight;/*战斗力*/
	int32_t amplify_times;/*附魔次数*/
	int32_t level;/**/
	int32_t authority;
	int8_t vip;/**/
	int8_t headship;/*公会职务*/
	int8_t career;/**/
	int8_t line_stat;/*在线状态*/
	int8_t gender;/**/
	std::string ganger_name;/*成员名称*/
};

/*
公会技能信息
*/
struct Gang_Skill_Detail {
	Gang_Skill_Detail(void) { reset(); };
	void reset(void);
	void serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);

	int32_t skill_id;/*公会技能ID*/
};
typedef std::map<int, Gang_Skill_Detail> Gang_Skill_Map;

/*
公会建筑信息
*/
struct Gang_Building_Detail {
	Gang_Building_Detail(void) { reset(); };
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	int32_t building_id;/*建筑ID*/
	int32_t building_lvl;/*建筑等级*/
	Time_Value cd_upgrade;
};
typedef std::map<int, Gang_Building_Detail> Gang_Building_Map;

struct Gang_DB_View {
	struct Affair {
		int8_t type;
		int8_t result;
		Time_Value expire;
		role_id_t applier;

		Affair() { memset( this, 0, sizeof(*this) ); }
	};

	enum GANG_STATUS{
		GANG_STAT_NON = 0,
		GANG_STAT_NORMAL,
		GANG_STAT_DISMISS,
		GANG_STAT_END
	};

	typedef std::vector<Affair> Affair_Vec;
	typedef std::vector<Gang_Passed_Event> Passed_Event_Vec;	// TODO
	typedef std::vector<Int_String> Simple_Event_Vec;
	typedef std::vector<Gang_Apply_Info> Apply_Info_Vec;
	typedef std::map<role_id_t, Gang_Member_Detail> Member_Map;

	Gang_DB_View();
	~Gang_DB_View();

	void reset(void);
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	int gang_war_serialize(Block_Buffer &w) const;
	int gang_war_deserialize(Block_Buffer &r);
	int save(void);
	bool is_change(){ return change;}
	void change_view(bool set=true) { change = set;}

	// gang passed event start
	void add_passed_event_101900(role_id_t role_act, role_id_t role_in);
	void add_passed_event_101901(role_id_t role_active, role_id_t role_passive, const int head_ship);
	void add_passed_event_101902(role_id_t role_id);
	void add_passed_event_101903(role_id_t role_act, role_id_t role_out);
	void add_passed_event_101904(role_id_t role_active, const int cost_fund);
	void add_passed_event_101906(role_id_t role_active, role_id_t role_passive);
	void add_passed_event_101907(role_id_t role_active, role_id_t role_passive);
	void add_passed_event_101908(void);
	void add_passed_event_101909(const int day);
	void add_passed_event_101910(role_id_t role_active);
	void send_last_passed_event();
	void get_last_n_events(int n, Passed_Event_Vec& out);
	// gang passed event end

	bool has_apply_info(role_id_t role_id) const;
	bool erase_apply_info(role_id_t role_id);
	bool can_direct_in(role_id_t role_id);
	bool has_member(role_id_t role_id);
	std::string headship_name(const int head_ship);
	boost::unordered_map<int64_t, Reward_Info_Gang_War> &gang_war_award_map(void);
	role_id_t make_gang_war_index(void);
	int add_contribute(const int donate_gain);

	gang_id_t gang_id;
	std::string name;
	role_id_t leader;
	int32_t fund;		// 公会贡献（财富）
	int32_t maintain_cost;
	int32_t fund_lack_times;
	int32_t level;
	int8_t boss_act;				// 每日公会boss是否打过
	int8_t boss_hour;				// 公会boss开启时间（小时）
	int8_t boss_minute;			// 公会boss开启时间（分钟）
	int boss_kill_num;			// 公会boss击杀数量
	int boss_lv;			// 公会boss level
	int32_t rank;
	int32_t max_members;
	int8_t  logo;
	int8_t  day_deficit_count;
	int8_t  status;
	int8_t  join_block;
	std::string announcement;
	int8_t can_seize_power;
	role_id_t last_leader_id;
	int8_t notified_last_leader;
	int force_total;

	Time_Value create_time;
	Time_Value dismiss_time;
	Time_Value cd_upgrade;
	Time_Value cd_annouce;
	Time_Value tv_leader_offline;

	Server_Key server_info;
	//Role_Hash_Set members;
	Member_Map member_map;
	Role_Hash_Set direct_in;
	Affair_Vec affairs;
	Passed_Event_Vec passed_events;
	Simple_Event_Vec simple_events;
	Apply_Info_Vec apply_infos;
	Gang_Skill_Map researched_skills;	// 已研究的公会技能
	Gang_Building_Map buildings;

	UInt_Set acitve_annon_ed;
	boost::unordered_map<int32_t, role_id_t> level_leader_id;

	boost::unordered_map<int64_t, Reward_Info_Gang_War> gang_war_award;
	role_id_t gang_war_award_index;

	// 不要存数据库的变量
	int8_t boss_act_state;					// 公会boss 0 1 close 2 ready 3 open
	Time_Value boss_act_time;			   // 公会boss开启时间
	Time_Value boss_act_next_time;		// 公会boss下一个状态时间

	bool change;
};

struct Gang_DB_View_Rank {
	Gang_DB_View_Rank(void) { reset(); };
	~Gang_DB_View_Rank() {};
	void reset(void);
	friend bool operator<(const Gang_DB_View_Rank &param1, const Gang_DB_View_Rank &param2);

	gang_id_t gang_id;
	int32_t level;
	int32_t member_size;
	int32_t fund;
	int force_total;
};

struct Gang_DB_View_War_Rank {
	Gang_DB_View_War_Rank(void) { reset(); };
	~Gang_DB_View_War_Rank() {};
	void reset(void);
	friend bool operator<(const Gang_DB_View_War_Rank &param1, const Gang_DB_View_War_Rank &param2);

	gang_id_t gang_id;
	int32_t level;
	int32_t member_size;
	int32_t fund;
	int force_total;
};

typedef std::pair<int64_t, int64_t> Matched_Gang_Pair;
struct Gang_War_Matched_Gangs_Pair {
	Gang_War_Matched_Gangs_Pair(void) {
		reset();
	}

	void reset(void) {
		match_time = Time_Value::zero;
		matched_pair.first = 0;
		matched_pair.second = 0;
		result = 0;
		matched_index = 0;
		scene_index = 0;
	}

	Time_Value match_time;
	Matched_Gang_Pair matched_pair;
	int8_t result;//0上面胜1下面胜2未打完
	int8_t matched_index;
	int64_t scene_index;
};
typedef std::map<int64_t, Gang_War_Matched_Gangs_Pair> SceneIndex_GangWarMatchedGangsPair_Map;
typedef std::vector<Gang_War_Matched_Gangs_Pair> GangWarMatchedGangsPair_Vec;
typedef std::deque<Gang_War_Matched_Gangs_Pair> GangWarMatchedGangsPair_Deque;

struct Gang_War_Matched_Gang_Info {
	Gang_War_Matched_Gang_Info(void) {
		reset();
	}
	void reset(void) {
		matched_index = 0;
		camp = 0;
		match_time = Time_Value::zero;
		scene_index = 0;
		zone.clear();
		gang_name.clear();
		btn_state = 0;
		rank = 0;
		force = 0;
	}
	int64_t matched_index;
	int8_t camp;
	Time_Value match_time;
	int64_t scene_index;
	std::string zone;
	std::string gang_name;
	int8_t btn_state; // 按钮状态0抽签1参战2领奖
	int8_t rank;
	int64_t force;
};
typedef std::map<int64_t, Gang_War_Matched_Gang_Info> GangWarMatchedGangInfo_Map;

struct Gang_War_Rank_Award_Detail {
	Gang_War_Rank_Award_Detail(void) { reset(); }
	void reset(void) {
		rank = 0;
		gang_id = 0;
		state = 0;
		gang_name.clear();
		zone.clear();
		score = 0;
		win_nums = 0;
		kill_nums = 0;
		force = 0;
	}
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	friend bool compress_gang_war_rank_award_detail(const Gang_War_Rank_Award_Detail &gwrad1, const Gang_War_Rank_Award_Detail &gwrad2);
	int8_t rank;
	role_id_t gang_id;
	int8_t state; // 0未领1已领
	std::string gang_name;
	std::string zone;
	int score;
	int8_t win_nums;
	int kill_nums;
	int force;
};
typedef std::map<int8_t, Gang_War_Rank_Award_Detail> Gang_War_Rank_Award;
typedef boost::unordered_map<role_id_t, Gang_War_Rank_Award_Detail> Gang_War_Gang_Id_Rank_Award;
struct Gang_War_Rank_Award_Detail_Less {
	bool operator() (const Gang_War_Rank_Award_Detail *gwrad1, const Gang_War_Rank_Award_Detail *gwrad2) const {
		return compress_gang_war_rank_award_detail(*gwrad1, *gwrad2);
	}
};

typedef boost::unordered_map<role_id_t, Gang_War_Rank_Info> Ganer_Rank_Info;
struct Gang_War_Gang_Rank_Info {
	Gang_War_Gang_Rank_Info(void) { reset(); }
	void reset(void) {
		gang_id = 0;
		gang_name.clear();
		zone.clear();
		score = 0;
		win_nums = 0;
		kill_nums = 0;
		hold_nums = 0;
		force = 0;
		ganger_rank_info.clear();
	}
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	int add_ganger_rank_info(const Gang_War_Rank_Info &ganger_rank_info);
	int set_gang_rank_info(Gang_War_Gang_Rank &gang_rank_info);
	friend bool compress_gang_war_rank_info(const Gang_War_Gang_Rank_Info &gwrad1, const Gang_War_Gang_Rank_Info &gwrad2);
	role_id_t gang_id;
	std::string gang_name;
	std::string zone;
	int score;
	int win_nums;
	int kill_nums;
	int hold_nums;
	int force;
	Ganer_Rank_Info ganger_rank_info;
};
typedef boost::unordered_map<role_id_t, Gang_War_Gang_Rank_Info> GangWarGangRankInfo_Map;
typedef std::vector<Gang_War_Gang_Rank_Info*> GangWarGangRankInfo_Vec;
struct Gang_War_Rank_Info_Less {
	bool operator() (const Gang_War_Gang_Rank_Info *gwrad1, const Gang_War_Gang_Rank_Info *gwrad2) const {
		return compress_gang_war_rank_info(*gwrad1, *gwrad2);
	}
};

struct Gang_War_Info {
	Gang_War_Info(void) {
		reset();
	}
	void reset(void) {
		CLEAR_VECTOR(GangWarMatchedGangsPair_Vec, gang_war_matched_gangs_pair_vec_);
		gang_war_matched_gang_map_.clear();
		gang_war_spot_rank_map_.clear();

		gang_war_result_map_.clear();
		stage_ = 0;
		switch_ = 0;
		activity_time_ = Time_Value::zero;
		draw_time_ = Time_Value::zero;
		open_time_for_each_match = Time_Value::zero;
		change = false;
		rank_award_.clear();
		gang_id_rank_award_.clear();
	}
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);
	int save(void);
	bool is_change(){ return change;}
	void save_tick(bool set=true) { change = set;}

	/*
	 * 公会战：指定周一为起点
	 * */
	void set_activity_from(void);

	std::map<int8_t, std::vector<Gang_War_Rank_Info> > gang_war_spot_rank_map_;
	std::map<int8_t, Gang_War_Spot> gang_war_result_map_;

	GangWarGangRankInfo_Map gang_rank_map;
	GangWarMatchedGangsPair_Vec gang_war_matched_gangs_pair_vec_;
	GangWarMatchedGangInfo_Map gang_war_matched_gang_map_;
	static const int FROM_WEEK;
	Gang_War_Rank_Award rank_award_;// 公会战排名奖励
	Gang_War_Gang_Id_Rank_Award gang_id_rank_award_;

	int8_t stage_;
	int8_t switch_;
	Time_Value activity_time_;
	Time_Value draw_time_;
	Time_Value open_time_for_each_match;

	bool change;
};

#endif /* GANG_STRUCT_H_ */
