/*
 * Scene_Campfire.h
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */

#ifndef SCENE_CAMPFIRE_H_
#define SCENE_CAMPFIRE_H_


#include "Scene_Move_Base.h"
#include "Scene_Campfire_Struct.h"

extern const int WAR_DIE_REVIVE_TIME;

struct MSG_20300114;
struct MSG_20300116;

class Scene_Campfire: virtual public Scene_Move_Base {
	static const int AWARD_MAIL_ID = 1055; //篝火奖励找回邮件ID
public:
	Scene_Campfire(void);
	virtual ~Scene_Campfire(void);
public:
	void reset(void);
	// 地图通知登录完成时调用
	void sign_in(void);
	// 登出时调用
	void sign_out(void);

	int time_up(const Time_Value& now);

public:
	Scene_Campfire_Info* campfire_info(void);
	void set_campfire_info(Scene_Campfire_Info* info);
	void listen_gang_info_sync(void);
public:
	int req_campfire_add_firewood(role_id_t fire_role_id);
	int req_campfire_fire(role_id_t fire_role_id);
	int req_campfire_rank_list();

	int send_campfire_spot_info();
	int send_campfire_personal_info();
	int save_campfire_player_info();

	int get_campfire_award();
	int get_campfire_award_result(const MSG_20300114 &msg);
	int get_campfire_rank_award();

	int load_campfire_player_info(const MSG_20300116 &msg);
	void req_campfire_player_info();

	void clear_campfire_award_record();
private:
	Scene_Campfire_Info campfire_info_;
};



#endif /* SCENE_CAMPFIRE_H_ */
