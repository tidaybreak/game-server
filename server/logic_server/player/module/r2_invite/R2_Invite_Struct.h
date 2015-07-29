/*
 * R2_Invite_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef R2_INVITE_STRUCT_H_
#define R2_INVITE_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"

typedef boost::unordered_map<std::string, R2_Mail_Invite_Info> R2_Mail_Invite_Info_Map;

struct R2_Invite_Detail: public Detail {
	R2_Invite_Detail(void);
	virtual ~R2_Invite_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	role_id_t invite_user;//邀请我的人
	std::string my_mail;//邀请我创号的邮箱;我自己的;我通过这个邮箱点进创号的
/*********************************/
	uint32_t today_success_num;//今日邀请成功人数
	uint32_t all_success_num;//共邀请成功人数
	R2_Mail_Invite_Info_Map invite_mail;//邀请过的邮箱
	UInt_Set note_level;//已经通知的等级(通知是代表升级到了指定等级已经让对方知道并处理一次邀请成功)
};

#endif /* R2_INVITE_STRUCT_H_ */
