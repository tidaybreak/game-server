/*
 * Teamer.h
 *
 *  Created on: 2014年4月15日
 *      Author: Linqiyou
 */

#ifndef TEAMER_H_
#define TEAMER_H_

#include "Logic_Player_Base.h"
#include "Team_Struct.h"

struct Teamer_Detail;
class Team;
struct MSG_10100309;
struct MSG_10100303;
struct MSG_10100330;
struct MSG_10100302;
struct MSG_10100331;

class Teamer : virtual public Logic_Player_Base {
public :
	enum Profile_Set {
		PROFILE_UNUSE = 0,
		PROFILE_USE = 1,

		CANCEL_MATCH = 0,
		IN_MATCH = 1,
	};

	Teamer(void);
	virtual ~Teamer(void);

	void reset(void);

	void load_detail(Teamer_Detail &teamer_detail);
	void save_detail(Teamer_Detail &teamer_detail);

	Teamer_Detail const &teamer_detail(void);
	int32_t &integrate(void);
	Time_Value &integrate_time(void);

	void save_tick(void);

	static int create_init(Teamer_Detail &detail);
	void module_init(void);

	void sign_in(void);
	int sign_in_repeat(bool is_trustership);
	void sign_out(void);
	void trusteeship_sign_out(void);

	int client_ready_now(void);	// 场景加载完成

	int move_scene_completed(void);

	int trigger_daily_zero(const Time_Value &now);
	int trigger_daily_six(const Time_Value& now, bool logic_check = false);

	//-- 处理操作 ///////////////////////////////////////////////////////////////////////////
	int fetch_team_info(int8_t type);						//请求组队设置 区分是组队(0)还是竞技场(1)的或者是挂机(2)

	int create_team(int scene_id, std::string password, int8_t type, int8_t degree);//创建队伍
	int dismiss_team(void);									//解散队伍
	int profile_info(const int8_t type = -1);
//	int team_setting(void);									//请求队伍设置
	int invite_role(const MSG_10100302 &msg);						//邀请玩家入队
	int apply_team(const MSG_10100303 &msg);//申请入队
	int make_leader(role_id_t new_id);						//设为队长
	int kick_role(role_id_t role_id);						//踢出队伍
	int leave_team(void);									//离开队伍
	int change_team_setting(int8_t profile_set);			//修改组队设置-个人
	int agree_team_invite(const MSG_10100309 &msg);				//同意组队邀请

	int listen_team_info(const int8_t & type, const int8_t &listened);						//开始/结束监听副本
	int team_enter_wait_area(role_id_t role_id, int8_t type);			//玩家进入待战区
	int team_exit_wait_area(int8_t type);							//玩家退出待战区

	int reject_team_invite(role_id_t role_id);				//拒绝组队邀请

	int req_apply_team_quick(int8_t type);					// 快速加入
	int join_team_in_rule(const int8_t type);				// 按组队快速加入规则加入队伍
	int req_set_ready_state(bool ready);					//0 未准备好 1 已准备

	int req_back_room(void); // 返回房间

	int active_wait_area(int8_t type); // 请求等待区信息

	int set_password(std::string password_new); // 设置密码
	int cancel_password(std::string password); // 取消密码

	int add_inviter(Logic_Player &inviter);		// 增加邀请者（邀请自己入队的）
	int req_battle_status(Block_Buffer &buf);
	int add_applicant(Logic_Player &applicant, std::string password, int8_t profile_set);	// 增加申请者（申请入自己小队的）

	void rejected_invite_by(Logic_Player *rejecter);	// 邀请被拒绝（玩家拒绝进入小队）
//	void rejected_apply_by(Logic_Player *rejecter);		// 申请被拒绝（玩家不被允许进入小队）

	int req_team_set_position(const int8_t type, const int8_t position);	//设置队伍位置开启关闭(0关闭1开启)
	int req_team_set_info(const int8_t type, const int32_t scene_id, const int32_t degree); //修改队伍设置

	// 布阵
	int req_team_lineup_info(void);
	int req_team_lineup_set(const Int_Role_Id_Vec &info);

	/**多人副本*/
	int req_open_card(int index);//请求翻牌

	int req_team_hero_dream(void);
	int req_team_follow(const int8_t follow);
	int req_team_info(const MSG_10100330 &msg);
	int req_team_quick_invite(const MSG_10100331 &msg);
	//////////////////////////////////////////////////////////////////////////

	// hook
	int team_hook_enter_fb(const int scene_id, const int order); // 监听进副本
	int team_hook_finish_fb(const int scene_id, const int order); // 监听完成副本
	int team_hook_finish_fb_non_profile_set(const int scene_id, const int order);// 监听进副本无收益次数
	int team_hook_finish_fb_profile_set(const int scene_id, const int order);// 监听进副本收益次数
	void hook_fighter_force(const int new_force);
	// hook

	//创建限时击杀队伍
	int create_team_timelimit(const int32_t &scene_id, int type, Time_Value time, role_id_t id,
							  int v1, int v2, int v3, bool v4 = false);

	// 检测创建队伍的条件
	int check_create_team_condition(const int8_t type, const int32_t scene_id);
	int check_team_condition(const int8_t type, const int32_t scene_id);
	// 检测组队副本相关限制
	int check_fb_condition(const int32_t scene_id);
	// 检测组队竞技相关限制
	int check_sports_condition(void);
	// 检测组队竞技相关限制
	int check_hero_dream_condition(const int32_t scene_id);
	int check_defend_condition(const int scene_id);
	// 检测加入队伍限制
	int check_join_team_condition(const role_id_t &role_id, const int64_t index = -1, const std::string &password = "", const int8_t& is_world = 0);

	int team_hook_lineup_change(const int32_t pos);
	int team_hook_heroer_change(const int32_t fight_index);
	void hook_fighter_level(const int new_level, const int src_level);

	// 通知队员状态改变
	int active_other_member(void);
	// 广播组队布阵信息
	int active_member_lineup(void);
	// 广播是否第一次组队副本
	int active_is_first_team_fb(void);

	inline bool has_team(void);
	bool is_leader(void);

	int get_online_team_member_nums(void);
	int get_all_team_member_nums(void);
	int team_member_set(Role_Hash_Set &role_set);
	Team *get_team(void);

	void set_team_id(int64_t team_id);
	role_id_t leader_id(void);

	void sync_teamer_detail_to_chat(void);
	void sync_teamer_detail_to_scene(void);
	void sync_teamer_detail_to_scene(Block_Buffer &buf);
	void get_team_match_info(void);
	int team_load_info(Block_Buffer &buf); // 处理地图服的轮空次数
	int team_load_cumulate_award(Block_Buffer &buf);

	void set_no_team(void);						//设置为无队伍状态
	void set_is_not_first_team_fb(const bool is_not_first_fb_team);

	bool &team_get_ready_status(void);

	int team_sub_profile_num(void); // -1 fail
	int get_profile_num_by_type(const int8_t type);
	int profile_set(void);
	int8_t &follow(void);
//	int &profile_num_fb(void);
	int profile_num_fb_all_cfg(void); // 配置的收益次数
	int profile_num_fb_all(void); // 当前剩余收益次数
	int profile_num_fb_sub(const int num = 1);
	int profile_num_fb_pre_add(const int num);
	int &profile_num_sports(void);
	int &profile_num_hero_dream(void);
	int profile_num_sports_all(void);
	int profile_num_defend_all(void);
	int profile_num_defend_sub(const int num = 1);
	int64_t team_start_time(void);

	// 组队竞技奖励
	int team_sports_get_award(const uint8_t  result, const int team_sports_battle_level, const int score_gap);
//	int team_client_ready_broadcast(const bool &flag);

	int get_team_award(int level);

	void notice_team_award(int level_id = 0, bool notice = true);

	void send_score_list_to_client(Block_Buffer &buf);

private:
	Teamer_Detail *teamer_detail_;
	Role_Hash_Set inviter_map_;			// 邀请自己的玩家列表
	int profile_set_;					// 收益设置
	int8_t follow_;						// 英雄梦境：跟随
	bool client_ready_broadcast_;		// 客户端准备完成时广播
	int64_t start_time_;
};

#endif /* TEAMER_H_ */
