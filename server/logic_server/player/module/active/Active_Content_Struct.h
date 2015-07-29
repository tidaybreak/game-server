/*
 * Active_Content_Struct.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef ACTIVE_CONTENT_STRUCT_H_
#define ACTIVE_CONTENT_STRUCT_H_

#include "Server_Struct.h"
#include "Game_Typedef.h"
#include "Active_Config.h"

struct Role_Info;
typedef boost::unordered_map<int, Time_Value> Int_Time_Map;
typedef boost::unordered_map<int, Int_Time_Map> Group_Int_Time_Map;
typedef boost::unordered_map<uint32_t, UInt_Set> Uint_Uint_Set;

struct Active_Content_Detail : public Detail{
	Active_Content_Detail();
	virtual ~Active_Content_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	UInt_Set vip_club_receive_id;
	UInt_Set level_ace_receive_level;
	UInt_Set suit_collect_receive_level;
	UInt_Set guild_level_receive_id;
	UInt_Set gem_levelup_receive_id;
	UInt_Set wing_levelup_receive_level;
	UInt_Set mount_levelup_receive_id;
	std::map<int, int> suit_cnt_map;
	UInt_Set is_open_icon_id;
	UInt_Set sg_pass_receive_id;
	UInt_Set mount_pass_receive_id;
	UInt_Set wing_pass_receive_id;
	UInt_Set arena_dan_receive_id;
	Group_Int_Time_Map arena_dan_pass_time;

	Uint_Uint_Set common_receive_id;
	uint32_t all_world_boss_hurt;
	uint32_t today_world_boss_hurt;
	uint32_t world_boos_max_rank;
	Uint_Uint_Set heroes_door_type_level;
	uint32_t team_arena_win_num;
	uint32_t warer_win_num;
	uint32_t reward_task_done_num;
	UInt_Set on_send_mail_act_id;

	uint32_t enchanting_num;//附魔次数
	uint32_t mount_lvup_num;//坐骑升阶次数
	uint32_t fusion_x_elite_card_num;//合成X张精英卡牌次数
	uint32_t alchemy_num;//炼金次数
	Int_Time_Map common_lstime;//活动ID对应最后一次开始时间
	UInt_UInt_Map common_type_act_id;//类型对应最近一次活动ID


	uint32_t dragon_prey_num;//龙谷掠夺次数
	bool ring_task_recharge;//天天跑环 环任务活动 - 充值
};


struct Icon_Time_Info {
	Icon_Time_Info(void) { reset(); }
	void reset(void) {
		icon_id = 0;
		over_time = Time_Value::zero;
		act_id = 0;
	}
	uint32_t icon_id;
	Time_Value over_time;
	uint32_t act_id;
};

struct Common_Active_Icon_Info {
	Common_Active_Icon_Info(void) { reset(); }
	void reset(void) {
		icon_id = 0;
		over_time = Time_Value::zero;
		level_limit = 0;
		down_time = Time_Value::zero;
		active = 0;
	}
	uint32_t icon_id;
	Time_Value over_time;
	uint32_t level_limit;
	int8_t active;
	Time_Value down_time;
};

struct Active_Content_Info {
	Active_Content_Info(void) { reset(); }
	void reset(void) {
		vip_announce_ed_level.clear();
		level_rank_count = false;
		level_rank_count_role_info.clear();
		arena_rank_count = false;
		arena_rank_count_role_info.clear();
		force_rank_count = false;
		force_rank_count_role_info.clear();
		lr_last_start_time = Time_Value::zero;
		fr_last_start_time = Time_Value::zero;
		ar_last_start_time = Time_Value::zero;

		sg_rank_count = false;
		sg_rank_count_role_info.clear();
		sgr_last_start_time = Time_Value::zero;

		mount_rank_count = false;
		mount_rank_count_role_info.clear();
		mount_last_start_time = Time_Value::zero;

		wing_rank_count = false;
		wing_rank_count_role_info.clear();
		wing_last_start_time = Time_Value::zero;

		arena_dan_start_time = Time_Value::zero;
		arena_dan_last_id = 0;
		common_lstime.clear();
	}

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	UInt_Set vip_announce_ed_level;
	bool level_rank_count;
	std::vector<Role_Info> level_rank_count_role_info;
	bool arena_rank_count;
	std::vector<Role_Info> arena_rank_count_role_info;
	bool force_rank_count;
	std::vector<Role_Info> force_rank_count_role_info;

	Time_Value lr_last_start_time;//zuijinyici
	Time_Value fr_last_start_time;//zuijinyici
	Time_Value ar_last_start_time;//zuijinyici
//shanggufuben
	Time_Value sgr_last_start_time;//zuijinyici
	bool sg_rank_count;
	std::vector<Role_Info> sg_rank_count_role_info;

	//mount
	Time_Value mount_last_start_time;//zuijinyici
	bool mount_rank_count;
	std::vector<Role_Info> mount_rank_count_role_info;

	//wing
	Time_Value wing_last_start_time;//zuijinyici
	bool wing_rank_count;
	std::vector<Role_Info> wing_rank_count_role_info;

	Time_Value arena_dan_start_time;

	uint32_t arena_dan_last_id;
	Int_Time_Map common_lstime;
};

typedef boost::unordered_map<uint32_t, Common_Active_Icon_Info> Common_Active_Icon_Info_Map;


struct Two_Recharge_Info {
	Two_Recharge_Info(void) { reset(); }
	void reset() {
		rec_num = 0;
		last_start_time = 0;
		change = false;
	}

	int serialize(Block_Buffer &buffer) const {
		buffer.write_uint32(rec_num);
		buffer.write_uint32(last_start_time);
		return 0;
	}

	int deserialize(Block_Buffer &buffer) {
		buffer.read_uint32(rec_num);
		buffer.read_uint32(last_start_time);
		return 0;
	}
	uint32_t rec_num;
	uint32_t last_start_time;
	bool change;
};

#endif /* ACTIVE_CONTENT_STRUCT_H_ */
