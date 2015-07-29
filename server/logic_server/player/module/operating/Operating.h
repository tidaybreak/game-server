/*
 * Operating.h
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 *
 *      离线操作
 */

#ifndef OPERATING_H_
#define OPERATING_H_

#include "Logic_Player_Base.h"

class Mail;
class Friend;

struct Operating_Detail;
struct Mail_Send_Info;
struct Ganger_Detail;
struct Arena_Reward_Info;

class Operating : virtual public Mail,
				  virtual public Friend {
public:
	Operating(void);
	virtual ~Operating();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Operating_Detail &detail);
	// 登录时detail赋值
	void load_detail(Operating_Detail &detail);
	// 保存时detail赋值
	void save_detail(Operating_Detail &data);

	Operating_Detail const &operating_detail(void) const;
	Operating_Detail& operation_detail_no_const();

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);

	// 登录时module初始化,在这里发客户端消息无效
	void module_init(void);
	// 登录网关后后调用,通知客户端模块信息
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

	/////////////////////////////////////////////////////////////////////////////////////

	// 离线邮件
	static int offline_mail(Mail_Send_Info &send_info);

	// 离线组队竞技奖励
	static int offline_team_award(role_id_t role_id, int level_id);

	//离线好友
	static int offline_firend_add(role_id_t role_id, role_id_t other_role_id, Time_Value time);
	static int offline_firend_handel(role_id_t role_id, role_id_t other_role_id, int8_t handel);

	// 离线arena_reward
	static int offline_arena_three_day_reward(role_id_t role_id, Arena_Reward_Info &info);
	static int offline_arena_seven_day_reward(role_id_t role_id, Arena_Reward_Info &info);
	//
	static int offline_ganger_info_update(role_id_t role_id, Ganger_Detail& detail);

	//离线收到鲜花
	static int offline_receive_flowers(role_id_t role_id, std::string &role_name, uint32_t item_id,  uint32_t charm, uint32_t friend_num, std::string &msg,  role_id_t receive_role_id);

	//龙谷建筑离线通知
	static int offline_dragon_vale_building_notice(role_id_t role_id, uint8_t notice_type,
									uint8_t gold_mine, uint8_t moon_well, uint8_t totem);
	//龙谷掠夺战斗离线结果通知
	static int offline_dragon_vale_rob_fight_result_notice(role_id_t role_id, uint8_t notice_type, std::string server_name,
									std::string player_name, uint8_t result);
	//龙谷资源被掠夺离线通知
	static int offline_dargon_vale_gain_rob_production_notice(role_id_t role_id, std::string server_name, std::string player_name,
									uint32_t gold_mine, uint32_t moon_well, uint32_t totem);
	//龙谷至尊宝离线通知
	static int offline_dragon_vale_special_notice(role_id_t role_id, bool is_special);

	//离线成功邀请一个玩家
	static int offline_invite_player(role_id_t role_id, const Time_Value &time);
	static int offline_like_success(role_id_t role_id);//点赞

	static int offline_active_content_listen_world_boss_end_info(role_id_t role_id , uint32_t damage, uint32_t rank, uint32_t damage_rate);
private:
	Operating_Detail *operating_detail_;
};

inline Operating_Detail const &Operating::operating_detail(void) const{
	return *operating_detail_;
}

inline Operating_Detail& Operating::operation_detail_no_const() {
	return *operating_detail_;
}

#endif /* OPERATING_H_ */
