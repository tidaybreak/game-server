/*
 * R2_Invite.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include <r2_invite/R2_Invite.h>
#include <r2_invite/R2_Invite_Struct.h>
#include "Logic_Module_Head.h"
#include "Config_Cache_R2_Invite.h"
#include "Default_Struct.h"
#include "R2_Invite_Struct.h"
#include "R2_Invite_Def.h"
#include "Logic_Player.h"

R2_Invite::R2_Invite(void) {
	reset();
}

R2_Invite::~R2_Invite() {
	// TODO Auto-generated destructor stub
}

void R2_Invite::reset(void) {
	r2_invite_detail_ = 0;
}

int R2_Invite::create_init(R2_Invite_Detail &detail) {

	return 0;
}

void R2_Invite::load_detail(R2_Invite_Detail &detail) {
	r2_invite_detail_ = &detail;
}

void R2_Invite::save_detail(R2_Invite_Detail &detail) {
	if (r2_invite_detail().is_change) {
		detail = r2_invite_detail();
		r2_invite_detail_->is_change = false;
	}
}

int R2_Invite::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int R2_Invite::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int R2_Invite::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int R2_Invite::move_scene_completed(void) {

	return 0;
}

int R2_Invite::client_ready_now(void) {

	return 0;
}

void R2_Invite::module_init(void) {

}

void R2_Invite::sign_in(void) {

}

void R2_Invite::sign_out(void) {

}

void R2_Invite::trusteeship_sign_out(void) {

}

int R2_Invite::sign_in_repeat(void) {

	return 0;
}


int R2_Invite::req_get_r2_invite_mail_info(void) {
	MSG_50106801 msg;
	for(R2_Mail_Invite_Info_Map::iterator mit =  r2_invite_detail_->invite_mail.begin(); mit != r2_invite_detail_->invite_mail.end(); ++mit) {
		msg.info.push_back(mit->second);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int R2_Invite::req_add_r2_invite_mail(std::vector<std::string> &mails) {
	MSG_50106800 msg;
	for(std::vector<std::string>::iterator it = mails.begin(); it != mails.end(); ++it) {
		R2_Mail_Invite_Info_Map::iterator mit = r2_invite_detail_->invite_mail.find(*it);
		if(mit == r2_invite_detail_->invite_mail.end()) {
			R2_Mail_Invite_Info info;
			info.mail = *it;
			info.time = Time_Value::gettimeofday().sec();
			r2_invite_detail_->invite_mail.insert(std::make_pair(info.mail, info));
			r2_invite_detail_->detail_change();
			msg.mails.push_back(*it);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void R2_Invite::r2_invite_listen_level_up(const int level) {
	if(r2_invite_detail_->invite_user &&  r2_invite_detail_->note_level.find(level) == r2_invite_detail_->note_level.end() && r2_invite_detail_->my_mail.size()) {
		r2_invite_detail_->note_level.insert(level);
		r2_invite_detail_->detail_change();
		//得到当前等级对应状态和是否达到等级要求
		uint8_t status = R2_INVITE_MAIL_STATUS_DEFAULT;
		uint32_t val = CONFIG_CACHE_R2_INVITE->get_common_config_val(R2_INVITE_COMMON_CFG_TYPE_LVUPING);
		if((uint32_t)level >= val && val != 0) {
			status = R2_INVITE_MAIL_STATUS_UPLVING;
		}
		val = CONFIG_CACHE_R2_INVITE->get_common_config_val(R2_INVITE_COMMON_CFG_TYPE_LVSECCESS);
		if((uint32_t)level >= val && val != 0) {
			status = R2_INVITE_MAIL_STATUS_SUCCESS;
		}
		if(status == R2_INVITE_MAIL_STATUS_DEFAULT) {
			return;
		}
		//查找玩家;
		Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(r2_invite_detail_->invite_user);
		if (receiver) {//online
			receiver->r2_listen_success_invite_levelup(r2_invite_detail_->my_mail, status);
		} else {//offline
			//todo
		}
	}
}

void R2_Invite::r2_listen_success_invite_levelup(std::string &mail, uint8_t status) {
	R2_Mail_Invite_Info_Map::iterator it = r2_invite_detail_->invite_mail.find(mail);
	if(it != r2_invite_detail_->invite_mail.end()) {
		it->second.status = status;
		r2_invite_detail_->detail_change();
	}
}
