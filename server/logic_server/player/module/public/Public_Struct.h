/*
 * Public_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef PUBLIC_STRUCT_H_
#define PUBLIC_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Struct.h"
#include "heroer/Logic_Heroer_Struct.h"

/*
 *key: guide_id
 *value:已经引导的次数
 */
typedef boost::unordered_map<uint16_t, uint8_t> Guide_NUM_Map;
typedef std::map<int, int8_t> Fornotice_Gifts_Status_Map;
typedef boost::unordered_set<uint32_t> Played_Plot_Set;


struct Guide_Step_Info {
	Guide_Step_Info() {
		reset();
	}
	void reset() {
		step = 0;
		num = 0;
	}
	uint8_t step;
	uint8_t num;
};

//key:新手引导id
typedef boost::unordered_map<uint16_t, Guide_Step_Info> Guide_Step_Map;

struct Public_Detail: public Detail {
	Public_Detail(void);
	virtual ~Public_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	typedef std::deque<Location> Location_History;

	std::string plot_id;				/// 剧情id
	int plot_step;						/// 剧情进度
	String_Int_Map plot_info;
	bool plot_switch;					// 剧情switch0:true不播 1：false播
	Location_History local_his;			/// 场景历史

	int add_up_online_time;				/// 累计在线时间(秒)
	int fcm;							///防沉迷,0.通过防沉迷验证,1.未登记信息,2.登记了信息但没成年

	Guide_NUM_Map guide_num_map;        //引导次数
	Played_Plot_Set played_plot_set;    //已播放过的剧情集
	Guide_Step_Map guide_step_map;		//引导步骤列表

	uint8_t is_total_open;          //总开关是否打开(1:开启，0:关闭)
	uint8_t is_open_music;			//是否开启音乐. 1:开启,0:关闭
	uint8_t music_num; 				//音乐音量
	uint8_t is_open_voice; 			//是否开启音效. 1:开启,0:关闭
	uint8_t voice_num;              //音效音量
	uint8_t is_hide_role_model;     //是否屏蔽角色模型. 1:是,0:否
	uint8_t is_hide_hero_model;     //是否屏蔽英雄模型. 1:是,0:否
	uint8_t is_hide_title;          //是否屏蔽称号.    1:是,0:否
	uint32_t player_count;          //同屏显示玩家数
	uint8_t is_skip_plot;           //是否跳过剧情 1:是,0:否

	uint8_t war_enter_num;				// 战场当天进入次数
	uint32_t war_merit;					// 战场当天功勋
	uint32_t merit; 						// 功勋
	Time_Value war_forbid_time;		// 禁止进入战场时间

	uint8_t add_campfire;				// 公会篝火（0：今日还没有添加过柴火；1：今日已经添加过柴火)
	int campfire_id;						// 篝火晚会id
	uint32_t campfire_finish_time; //上一次篝火完成时间
	uint32_t campfire_rank;			//篝火晚会名次
	boost::unordered_set<int32_t> killed_match_man;
	int campfire_lighter; // 0添柴奖励 1点燃奖励
	int campfire_recieve_stat;//篝火奖励 0:不可领 1:可领 2:已领
	int campfire_rank_rev_stat;//篝火排名奖励 0:不可领 1:可领 2:已领

	// 战场额外奖励
	int8_t war_result;
	int8_t war_rank;
	uint16_t war_kill_num;
	uint16_t war_coe;

	int awake_lvl;									// 觉醒等级
	Artifact_Map artifacts;							// 神器信息集

	Fornotice_Gifts_Status_Map fornotice_gifts_status;	// 预告系统礼包领取状态

	int exploit_val;									// 功勋值
	int exploit_lvl;									// 功勋等级

	std::vector<Flowers_Info> flowers_info_vec;

	uint8_t first_recharge; //首充状态; 0,未充值; 1.首充过但未领取礼包; 2已经首充并领取礼包
	uint8_t two_recharge; //二次首充状态; 0,未充值; 1.首充过但未领取礼包; 2已经首充并领取礼包
	uint8_t limit_time_recharge; //限时充值状态; 0,未充值; 1.首充过但未领取礼包; 2已经首充并领取礼包
	Time_Value last_two_recharge_stime;//上一次全民充值活动开始时间
	Time_Value last_limit_time_recharge_stime;
	uint32_t recharge_money; //累计充值钻石
	UInt_Set recharge_feedback_receive_list;//充值反馈已经领取列表
	Time_Value last_recharge_rebate_stime;//上一次的充值返利活动开始时间
	uint32_t recharge_money_his; //累计充值钻石

	Time_Value recharge_activities_last_time;//充值活动最近一次开始时间
	uint32_t recharge_activities_money;//充值活动过程累计充值
	UInt_Set recharge_activities_receive_id;//充值活动这次的领取ID

	uint32_t notice_id;
	bool is_played_effect;
	bool is_show;

	bool is_receive_old_player_gift;

	uint32_t today_rec_bind_gold;

	//调查问卷
	Int_Set ans_qid;//已经回答的ID
	//分享邀请奖励
	Time_Value inv_time; //最后一次分享成功时间
	Time_Value rec_inv_time; //最后一次领取分享时间
	bool is_receive_inv_ones;//是否已经领取过一次性奖励
	bool is_receive_like_reward;//是否已经领取点赞

	bool is_magic_wing_over_time;
	bool is_magic_wing_readed;
	Time_Value vip_over_time;
	bool is_vip_readed;
	Time_Value genius_over_time;
	bool is_genius_readed;

	uint32_t task_guide_id;

	int friendship_accumulate;

	int vip_upgrade_cards_;

	Time_Value start_frist_recharge_time;
	int8_t arena_worship_times;       // 玩家已膜拜次数
};


#endif /* PUBLIC_STRUCT_H_ */
