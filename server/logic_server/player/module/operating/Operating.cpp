/*
 * Operating.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "friend/Friend.h"
#include "pack/Packer.h"
#include "mail/Mail.h"
#include "Operating.h"
#include "Operating_Struct.h"
#include "Logic_Module_Head.h"
#include "operating/Operating_Def.h"
#include "gang/Gang_Struct.h"
#include "Logic_Player.h"
#include "DB_Def.h"
#include "Msg_Active_Struct.h"
#include "public/Public_Struct.h"
#include "oper_activity/Oper_Activity_Def.h"

Operating::Operating(void) {
	reset();
}

Operating::~Operating() {
	// TODO Auto-generated destructor stub
}

void Operating::reset(void) {
	operating_detail_ = 0;
}

int Operating::create_init(Operating_Detail &detail) {

	return 0;
}

void Operating::load_detail(Operating_Detail &detail) {
	operating_detail_ = &detail;
}

void Operating::save_detail(Operating_Detail &detail) {
	if (operating_detail().is_change) {
		detail = operating_detail();
		operating_detail_->is_change = false;
	}
}

int Operating::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int Operating::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Operating::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

void Operating::module_init(void) {
	if (! operating_detail_->ganger_info.empty()) {
		player_self()->offline_operate_init(operating_detail_->ganger_info.back());
		operating_detail_->ganger_info.clear();
	}
}

void Operating::sign_in(void) {
	// 发离线邮件
	for (Operating_Detail::Offline_Mail::iterator it = operating_detail_->mail_list.begin(); it != operating_detail_->mail_list.end(); ++it) {
		send_mail_to_self(*it);
	}
	operating_detail_->mail_list.clear();

	// 处理离线好友请求
	offline_add_invite(operating_detail_->invite_list);
	operating_detail_->invite_list.clear();
	// 处理离线好友处理结果
	offline_handel_apply(operating_detail_->apply_list);
	operating_detail_->apply_list.clear();

	// 发放充值
	if(operating_detail_->recharge_gold.size()) {
		for(Operating_Recharge_Vec::iterator it = operating_detail_->recharge_gold.begin(); it != operating_detail_->recharge_gold.end(); ++it) {
			int res = pack_add_money(Money_Add_Info(GOLD, it->gold, Money_DM_Info(MONEY_ADD_RECHARGE,0,0,it->time)));
			if(res == 0) {
				player_self()->recharge_listen(it->gold);
			} else {
				MSG_USER("info:%s, pay_num:%d, role_id:%ld, error_no:%d, order_id:%s", "玩家离线充值补单(失败)", it->gold, player_self()->role_id(), res, it->order_id.c_str());
			}
		}
		operating_detail_->recharge_gold.clear();
	}

//	if ( !(operating_detail_->ganger_info.empty()) ) {
//		player_self()->update_offline_ganger_detail( operating_detail_->ganger_info.back() );
//		operating_detail_->ganger_info.clear();
//	}

	// 处理离线过程中的送花
	if(!operating_detail_->receive_flowers_list.empty()) {
		player_self()->offline_add_flowers(operating_detail_->receive_flowers_list);
		operating_detail_->receive_flowers_list.clear();
	}
	// 处理离线竞技场奖励
	{
		player_self()->update_offline_reward();
		operating_detail_->arena_three_day_reward.clear();
	}

	//离线成功邀请一个玩家
	if(player_self()->public_detail().inv_time.sec() < operating_detail_->inv_time
		&& operating_detail_->inv_time <= Time_Value::gettimeofday().sec()) {
		player_self()->set_inv_time(Time_Value(operating_detail_->inv_time));
	}
	//离线点赞成功
	if(operating_detail_->like_success) {
		player_self()->get_like_reward();
	}

	if(operating_detail_->wbrank) {
		player_self()->active_content_listen_world_boss_end_info(operating_detail_->damage, operating_detail_->wbrank, operating_detail_->damage_rate);
		operating_detail_->damage = 0;
		operating_detail_->wbrank = 0;
	}

	if(operating_detail_->damage_rate) {
		player_self()->oper_activity_listen_type_common_re(OPER_ACTIVITY_ITEM_WORLD_BOSS_PER, operating_detail_->damage_rate);
	}

	// 离线组队竞技奖励
	for (Int_Vec::iterator it = operating_detail_->team_award.begin(); it != operating_detail_->team_award.end(); ++it) {
		player_self()->notice_team_award(*it, false);
	}
	operating_detail_->team_award.clear();
}

void Operating::sign_out(void) {

}

int Operating::offline_mail(Mail_Send_Info &send_info) {
	if (!send_info.receiver_id) {
		MSG_USER("error receiver id");
		return -1;
	}
	MSG_DEBUG("offline_mail, role_id:%ld, mail_title:%s, mail_contents:%s", send_info.receiver_id, send_info.title.c_str(), send_info.content.c_str() );
	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	int type = OPERATING_OFFLINE_MAIL;
	buf.write_int32(type);
	send_info.serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);

	return 0;
}

int Operating::offline_team_award(role_id_t role_id, int level_id) {

	Block_Buffer buf;
	buf.make_message(OPERATING_OFFLINE_TEAM_AWARD);
	buf.write_int64(role_id);
	buf.write_int32(level_id);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);

	return 0;
}

int Operating::offline_firend_add(role_id_t role_id, role_id_t other_role_id, Time_Value time) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	int type = OPERATING_OFFLINE_FIREND_ADD;
	buf.write_int32(type);
	buf.write_int64(role_id);
	buf.write_int64(other_role_id);
	buf.write_int64(time.sec());
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);

	return 0;
}

int Operating::offline_firend_handel(role_id_t role_id, role_id_t other_role_id, int8_t handel) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	int type = OPERATING_OFFLINE_APPLY_HANDEL;
	buf.write_int32(type);
	buf.write_int64(role_id);
	buf.write_int64(other_role_id);
	buf.write_int8(handel);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_ganger_info_update(role_id_t role_id, Ganger_Detail& detail) {
	if ( role_id == 0 ) return 0;
	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	int type = OPERATING_OFFLINE_GANGER_INFO;
	buf.write_int32(type);
	buf.write_int64(role_id);
	detail.serialize( buf );
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_receive_flowers(role_id_t role_id, std::string &role_name, uint32_t item_id,  uint32_t charm, uint32_t friend_num, std::string &msg, role_id_t receive_role_id) {
	if ( role_id == 0 ) return 0;
	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	int type = OPERATING_OFFLINE_RECEIVE_FLOWERS;
	buf.write_int32(type);
	buf.write_int64(role_id);
	buf.write_string(role_name);
	buf.write_uint32(item_id);
	buf.write_uint32(charm);
	buf.write_uint32(friend_num);
	buf.write_string(msg);
	buf.write_int64(receive_role_id);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_arena_three_day_reward(role_id_t role_id, Arena_Reward_Info &info){
	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	int type = OPERATING_OFFLINE_ARENA_THREE_DAY_REWARD;
	buf.write_int32(type);
	buf.write_int64(role_id);
	info.serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_arena_seven_day_reward(role_id_t role_id, Arena_Reward_Info &info){
	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	int type = OPERATING_OFFLINE_ARENA_SEVEN_DAY_REWARD;
	buf.write_int32(type);
	buf.write_int64(role_id);
	info.serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_dragon_vale_building_notice(role_id_t role_id, uint8_t notice_type,
								uint8_t gold_mine, uint8_t moon_well, uint8_t totem) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	uint32_t op_type = OPERATING_OFFLINE_DRAGON_VALE_BUILDING;
	buf.write_uint32(op_type);
	buf.write_int64(role_id);
	buf.write_uint8(notice_type);
	buf.write_uint8(gold_mine);
	buf.write_uint8(moon_well);
	buf.write_uint8(totem);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_dragon_vale_rob_fight_result_notice(role_id_t role_id, uint8_t notice_type, std::string server_name,
								std::string player_name, uint8_t result) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	uint32_t op_type = OPERATING_OFFLINE_DRAGON_VALE_ROB_FIGHT_RESULT;
	buf.write_uint32(op_type);
	buf.write_int64(role_id);
	buf.write_uint8(notice_type);
	buf.write_string(server_name);
	buf.write_string(player_name);
	buf.write_uint8(result);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_dargon_vale_gain_rob_production_notice(role_id_t role_id, std::string server_name, std::string player_name,
								uint32_t gold_mine, uint32_t moon_well, uint32_t totem) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	uint32_t op_type = OPERATING_OFFLINE_DRAGON_VALE_GAIN_ROB_PRODUCTION;
	buf.write_uint32(op_type);
	buf.write_int64(role_id);
	buf.write_string(server_name);
	buf.write_string(player_name);
	buf.write_uint32(gold_mine);
	buf.write_uint32(moon_well);
	buf.write_uint32(totem);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_dragon_vale_special_notice(role_id_t role_id, bool is_special) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	uint32_t op_type = OPERATING_OFFLINE_DRAGON_VALE_SPECIAL;
	buf.write_uint32(op_type);
	buf.write_int64(role_id);
	buf.write_bool(is_special);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_invite_player(role_id_t role_id, const Time_Value &time) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	uint32_t op_type = OPERATING_OFFLINE_INVITE_PLAYER;
	buf.write_uint32(op_type);
	buf.write_int64(role_id);
	buf.write_int64(time.sec());
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_like_success(role_id_t role_id) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	uint32_t op_type = OPERATING_OFFLINE_LIKE_SUCCESS;
	buf.write_uint32(op_type);
	buf.write_int64(role_id);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

int Operating::offline_active_content_listen_world_boss_end_info(role_id_t role_id , uint32_t damage, uint32_t rank, uint32_t damage_rate) {
	if (!role_id) {
		MSG_USER("error role id");
		return -1;
	}

	Block_Buffer buf;
	buf.make_message(MONGO_OPERATING_OFFLINE_DATA);
	uint32_t op_type = OPERATING_OFFLINE_ACTIVE_CONTENT_LISTEN_WORLD_BOSS_END_INFO;
	buf.write_uint32(op_type);
	buf.write_int64(role_id);
	buf.write_uint32(damage);
	buf.write_uint32(rank);
	buf.write_uint32(damage_rate);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}
