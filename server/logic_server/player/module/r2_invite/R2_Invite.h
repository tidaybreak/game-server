/*
 * R2_Invite.h
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 *
 *      新模块例子
 */

#ifndef R2_INVITE_H_
#define R2_INVITE_H_

#include "Logic_Player_Base.h"

struct R2_Invite_Detail;

class R2_Invite : virtual public Logic_Player_Base {
public:
	R2_Invite(void);
	virtual ~R2_Invite();
	void reset(void);

	// 创建角色时初始化
	static int create_init(R2_Invite_Detail &detail);

	// 登录时detail赋值
	void load_detail(R2_Invite_Detail &detail);

	// 保存时detail赋值
	void save_detail(R2_Invite_Detail &data);

	R2_Invite_Detail const &r2_invite_detail(void) const;

	// 登录时module初始化,在这里发客户端消息无效
	void module_init(void);

	// 登录网关后后调用(只会的正常登录调用一次,顶号不会调)
	void sign_in(void);

	// 顶号时调用
	int sign_in_repeat(void);

	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	// 登出前调用
	void sign_out(void);

	// 场景加载完成
	int move_scene_completed(void);

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	//client
	int req_get_r2_invite_mail_info(void);
	int req_add_r2_invite_mail(std::vector<std::string> &mails);

	//listen
	void r2_invite_listen_level_up(const int level);
	void r2_listen_success_invite_levelup(std::string &mail, uint8_t status);

private:
	R2_Invite_Detail *r2_invite_detail_;
};

inline R2_Invite_Detail const &R2_Invite::r2_invite_detail(void) const{
	return *r2_invite_detail_;
}

#endif /* R2_INVITE_H_ */
