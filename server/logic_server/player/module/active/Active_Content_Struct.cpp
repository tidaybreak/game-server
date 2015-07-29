/*
 * Active_Content_Struct.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include "Active_Content_Struct.h"
#include "DB_Operator.h"
#include "Msg_Struct.h"

Active_Content_Detail::Active_Content_Detail(void) {
	reset();
}

Active_Content_Detail::~Active_Content_Detail(void) {

}

void Active_Content_Detail::reset(void) {
	role_id = 0;
	vip_club_receive_id.clear();
	level_ace_receive_level.clear();
	suit_collect_receive_level.clear();
	guild_level_receive_id.clear();
	suit_cnt_map.clear();
	gem_levelup_receive_id.clear();
	wing_levelup_receive_level.clear();
	mount_levelup_receive_id.clear();
	sg_pass_receive_id.clear();
	mount_pass_receive_id.clear();
	wing_pass_receive_id.clear();
	arena_dan_receive_id.clear();
	arena_dan_pass_time.clear();
	is_open_icon_id.clear();

	common_receive_id.clear();
	all_world_boss_hurt = 0;
	today_world_boss_hurt = 0;
	world_boos_max_rank = 0;
	heroes_door_type_level.clear();
	team_arena_win_num = 0;
	warer_win_num = 0;
	reward_task_done_num = 0;
	on_send_mail_act_id.clear();

	enchanting_num = 0;//附魔次数
	mount_lvup_num = 0;//坐骑升阶次数
	fusion_x_elite_card_num = 0;
	alchemy_num = 0;
	common_lstime.clear();//活动ID对应最后一次开始时间
	common_type_act_id.clear();

	dragon_prey_num = 0;
	ring_task_recharge = false;
}

int Active_Content_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Active_Content_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Active_Content_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_active_content_detail(*this);
	return 0;
}

int Active_Content_Detail::load(void) {
	return MONGO_OPERATOR->load_active_content_detail(*this);
}


int Active_Content_Info::serialize(Block_Buffer &buf) const {
	buf.write_uint8(vip_announce_ed_level.size());
	for(UInt_Set::iterator iter = vip_announce_ed_level.begin(); iter != vip_announce_ed_level.end(); ++iter) {
		buf.write_int32(*iter);
	}
	buf.write_bool(level_rank_count);
	buf.write_bool(arena_rank_count);
	buf.write_bool(force_rank_count);
	buf.write_bool(sg_rank_count);
	buf.write_bool(mount_rank_count);
	buf.write_bool(wing_rank_count);
	buf.write_int64(lr_last_start_time.sec());
	buf.write_int64(ar_last_start_time.sec());
	buf.write_int64(fr_last_start_time.sec());
	buf.write_int64(sgr_last_start_time.sec());
	buf.write_int64(mount_last_start_time.sec());
	buf.write_int64(wing_last_start_time.sec());
	buf.write_int64(arena_dan_start_time.sec());

	buf.write_uint16(level_rank_count_role_info.size());
	for(std::vector<Role_Info>::const_iterator it =  level_rank_count_role_info.begin(); it != level_rank_count_role_info.end(); ++it) {
		it->serialize(buf);
	}
	buf.write_uint16(force_rank_count_role_info.size());
	for(std::vector<Role_Info>::const_iterator it =  force_rank_count_role_info.begin(); it != force_rank_count_role_info.end(); ++it) {
		it->serialize(buf);
	}
	buf.write_uint16(arena_rank_count_role_info.size());
	for(std::vector<Role_Info>::const_iterator it =  arena_rank_count_role_info.begin(); it != arena_rank_count_role_info.end(); ++it) {
		it->serialize(buf);
	}
	buf.write_uint16(sg_rank_count_role_info.size());
	for(std::vector<Role_Info>::const_iterator it =  sg_rank_count_role_info.begin(); it != sg_rank_count_role_info.end(); ++it) {
		it->serialize(buf);
	}
	buf.write_uint16(mount_rank_count_role_info.size());
	for(std::vector<Role_Info>::const_iterator it =  mount_rank_count_role_info.begin(); it != mount_rank_count_role_info.end(); ++it) {
		it->serialize(buf);
	}
	buf.write_uint16(wing_rank_count_role_info.size());
	for(std::vector<Role_Info>::const_iterator it =  wing_rank_count_role_info.begin(); it != wing_rank_count_role_info.end(); ++it) {
		it->serialize(buf);
	}

	return 0;
}

int Active_Content_Info::deserialize(Block_Buffer &buff) {
	vip_announce_ed_level.clear();
	uint8_t size = 0;
	buff.read_uint8(size);
	for(; size > 0; --size) {
		int32_t val = 0;
		buff.read_int32(val);
		vip_announce_ed_level.insert(val);
	}

	buff.read_bool(level_rank_count);
	buff.read_bool(arena_rank_count);
	buff.read_bool(force_rank_count);
	buff.read_bool(sg_rank_count);
	buff.read_bool(mount_rank_count);
	buff.read_bool(wing_rank_count);

	int64_t time = 0;
	buff.read_int64(time);
	lr_last_start_time = Time_Value(time);
	time = 0;
	buff.read_int64(time);
	ar_last_start_time = Time_Value(time);
	time = 0;
	buff.read_int64(time);
	fr_last_start_time = Time_Value(time);
	time = 0;
	buff.read_int64(time);
	sgr_last_start_time = Time_Value(time);
	time = 0;
	buff.read_int64(time);
	mount_last_start_time = Time_Value(time);
	time = 0;
	buff.read_int64(time);
	wing_last_start_time = Time_Value(time);
	time = 0;
	buff.read_int64(time);
	arena_dan_start_time = Time_Value(time);

	uint16_t rsize = 0;
	buff.read_uint16(rsize);
	for(; rsize > 0; --rsize) {
		Role_Info role;
		role.deserialize(buff);
		level_rank_count_role_info.push_back(role);
	}
	buff.read_uint16(rsize);
	for(; rsize > 0; --rsize) {
		Role_Info role;
		role.deserialize(buff);
		force_rank_count_role_info.push_back(role);
	}
	buff.read_uint16(rsize);
	for(; rsize > 0; --rsize) {
		Role_Info role;
		role.deserialize(buff);
		arena_rank_count_role_info.push_back(role);
	}
	buff.read_uint16(rsize);
	for(; rsize > 0; --rsize) {
		Role_Info role;
		role.deserialize(buff);
		sg_rank_count_role_info.push_back(role);
	}
	buff.read_uint16(rsize);
	for(; rsize > 0; --rsize) {
		Role_Info role;
		role.deserialize(buff);
		mount_rank_count_role_info.push_back(role);
	}
	buff.read_uint16(rsize);
	for(; rsize > 0; --rsize) {
		Role_Info role;
		role.deserialize(buff);
		wing_rank_count_role_info.push_back(role);
	}
	return 0;
}
