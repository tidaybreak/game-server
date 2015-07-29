/*
 * Public_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Public_Struct.h"
#include "DB_Operator.h"

Public_Detail::Public_Detail(void) {
	reset();
}

Public_Detail::~Public_Detail(void) {

}

void Public_Detail::reset(void) {
	role_id = 0;

	local_his.clear();
	plot_id.clear();
	plot_step = 0;
	plot_info.clear();
	plot_switch = false;

	add_up_online_time = 0;
	fcm = 1;
	war_enter_num = 0;
	war_merit = 0;
	merit = 0;
	war_forbid_time = Time_Value::zero;
	add_campfire = 0;
	campfire_id = 0;
	campfire_finish_time = 0;
	campfire_rank = 0;
	killed_match_man.clear();
	campfire_lighter = 0;
	campfire_recieve_stat = 0;
	campfire_rank_rev_stat = 0;

	war_result = 0;
	war_rank = 0;
	war_kill_num = 0;
	war_coe = 0;

	guide_num_map.clear();
	played_plot_set.clear();
	guide_step_map.clear();

	is_total_open = 0;
	is_open_music = 0;
	music_num = 0;
	is_open_voice = 0;
	voice_num = 0;
	is_hide_role_model = 0;
	is_hide_hero_model = 0;
	is_hide_title = 0;
	player_count = 0;
	is_skip_plot = 0;

	awake_lvl = 0;
	artifacts.clear();
	fornotice_gifts_status.clear();

	exploit_val = 0;
	exploit_lvl = 0;

	CLEAR_VECTOR(std::vector<Flowers_Info>, flowers_info_vec);

	first_recharge = 0;
	two_recharge = 0;
	limit_time_recharge = 0;
	last_two_recharge_stime = Time_Value::zero;
	last_limit_time_recharge_stime = Time_Value::zero;
	recharge_feedback_receive_list.clear();
	recharge_money = 0;
	last_recharge_rebate_stime = Time_Value::zero;
	recharge_money_his = 0;

	recharge_activities_last_time = Time_Value::zero;
	recharge_activities_money = 0;
	recharge_activities_receive_id.clear();

	notice_id = 0;
	is_played_effect = false;
	is_show = false;

	is_receive_old_player_gift = false;
	today_rec_bind_gold = 0;
	is_receive_like_reward = false;

	ans_qid.clear();

	inv_time = Time_Value::zero; //最后一次分享成功时间
	rec_inv_time  = Time_Value::zero; //最后一次领取分享时间
	is_receive_inv_ones = false;//是否已经领取过一次性奖励

	is_magic_wing_over_time = false;
	is_magic_wing_readed = false;
	vip_over_time = Time_Value::zero;
	is_vip_readed = false;
	genius_over_time = Time_Value::zero;
	is_genius_readed = false;
	task_guide_id =  0;

	friendship_accumulate = 0;

	vip_upgrade_cards_ = 0;

	start_frist_recharge_time = Time_Value::gettimeofday();
	arena_worship_times = 0;
}

int Public_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_string(plot_id);
	buffer.write_int32(plot_step);
	buffer.write_bool(plot_switch);

	buffer.write_uint16(plot_info.size());
	for (String_Int_Map::const_iterator it = plot_info.begin(); it != plot_info.end(); ++it) {
		buffer.write_string(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_int32(awake_lvl);

	buffer.write_uint16(artifacts.size());
	for (Artifact_Map::const_iterator it = artifacts.begin(); it != artifacts.end(); ++it) {
		it->second.serialize(buffer);
	}

	buffer.write_uint16(fornotice_gifts_status.size());
	for (Fornotice_Gifts_Status_Map::const_iterator it = fornotice_gifts_status.begin();
			it != fornotice_gifts_status.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int8(it->second);
	}

	buffer.write_int32(exploit_val);
	buffer.write_int32(exploit_lvl);

	return 0;
}

int Public_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_string(plot_id);
	buffer.read_int32(plot_step);
	buffer.read_bool(plot_switch);

	uint16_t size = 0;
	buffer.read_uint16(size);
	std::string str = "";
	int num = 0;
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_string(str);
		buffer.read_int32(num);
		plot_info.insert(String_Int_Map::value_type(str, num));
	}

	buffer.read_int32(awake_lvl);

	Artifact_Detail artifact_detail;
	buffer.read_uint16(size);
	for(uint16_t i = 0; i < size; ++i) {
		artifact_detail.reset();
		if (artifact_detail.deserialize(buffer)) {
			continue;
		};
		artifacts.insert(std::make_pair(artifact_detail.artifact_id, artifact_detail));
	}

	int32_t gift_lvl = 0;
	int8_t status = 0;
	buffer.read_uint16(size);
	for(uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(gift_lvl);
		buffer.read_int8(status);
		fornotice_gifts_status.insert(std::make_pair(gift_lvl, status));
	}

	buffer.read_int32(exploit_val);
	buffer.read_int32(exploit_lvl);

	return 0;
}

int Public_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_public_detail(*this);
	return 0;
}

int Public_Detail::load(void) {
	return MONGO_OPERATOR->load_public_detail(*this);
}
