/*
 * Logic_Player.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef LOGIC_PLAYER_H_
#define LOGIC_PLAYER_H_

#include "Monitor_Struct.h"
#include "pack/Packer.h"
#include "mail/Mail.h"
#include "example/Example.h"
#include "public/Public.h"
#include "friend/Friend.h"
#include "operating/Operating.h"
#include "task/Tasker.h"
#include "fighter/Logic_Fighter.h"
#include "dungeon/Single_Dungeon.h"
#include "Scene_Def.h"
#include "item_user/Item_User.h"
#include "equiper/Equiper.h"
#include "heroer/Logic_Heroer.h"
#include "rune/Rune.h"
#include "rune_stone/Rune_Stone.h"
#include "Base_Detail.h"
#include "mall/Mall.h"
#include "team/Teamer.h"
#include "gang/Ganger.h"
#include "alchemy/Alchemy.h"
#include "announce/Logic_Announce.h"
#include "icon/Icon.h"
#include "arena/Logic_Arenaer.h"
#include "ranking/Rank.h"
#include "status/Logic_Status.h"
#include "exchange/Exchange.h"
#include "helper/Helper.h"
#include "covenant/Covenant.h"
#include "dragon/Dragon_Vale.h"
#include "achievement/Achievement.h"
#include "title/Title.h"
#include "vip/Vip.h"
#include "mount/Logic_Mount.h"
#include "reward/Reward.h"
#include "active/Active_Content.h"
#include "fashion/Fashion.h"
#include "auction/Auction.h"
#include "knight_trials/Knight_Trials.h"
#include "anci_artifact/Anci_Artifact.h"
#include "invest_plan/Invest_Plan.h"
#include "wing/Wing.h"
#include "expedition/Expedition.h"
#include "thorlottery/Logic_ThorLottery.h"
#include "card/Card.h"
#include "answer/Answer.h"
#include "activity/Activity.h"
#include "r2_invite/R2_Invite.h"
#include "activities_stores/Activities_Stores.h"
#include "life_skill/Life_Skill.h"
#include "oper_activity/Oper_Activity.h"
#include "pet/Logic_Pet.h"

class Logic_Monitor;

struct Location;
struct Base_Detail;
struct Logic_Player_Detail;

class Logic_Player :
					 virtual public Packer,
		 	 	 	 virtual public Tasker,
					 virtual public Friend,
					 virtual public Mail,
					 virtual public Teamer,
					 virtual public Equiper,
					 public Example,
					 public Public,
					 public Operating,
					 public Logic_Fighter,
					 public Item_User,
					 public Single_Dungeon,
					 public Logic_Heroer,
					 public Rune,
					 public Rune_Stone,
					 public Mall,
					 public Ganger,
					 public Logic_Announce,
					 public Alchemy,
					 public Icon,
					 public Rank,
					 public Logic_Arenaer,
					 public Logic_Status,
					 public Exchange,
					 public Helper,
					 public Covenant,
					 public Dragon_Vale,
					 public Achievement,
					 public Title,
					 public Vip,
					 public Logic_Mount,
					 public Reward,
					 public Active_Content,
					 public Fashion,
					 public Auction,
					 public Knight_Trials,
					 public Anci_Artifact,
					 public Invest_Plan,
					 public Wing,
					 public Expedition,
					 public Logic_ThorLottery,
					 public Card,
					 public Answer,
					 public Activity,
					 public R2_Invite,
					 public Activities_Stores,
					 public Life_Skill,
					 public Oper_Activity,
					 public Logic_Pet {
public:
	Logic_Player(void);
	virtual ~Logic_Player();

	void reset(void);

	Logic_Monitor *monitor(void);

	virtual Logic_Player *player_self(void);

	// 测试命令
	int test_command(std::string cmd_string);
	int echo_test_result(std::string result, role_id_t value = -1);

	////////////////////////////////////////////////////////////////

	virtual int send_to_client(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_gate(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_scene(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_local_chat(Block_Buffer &buf, bool use_swap = false);

	virtual int send_err_to_client(int error_id, int source);
	virtual int send_tip_to_client(int error_id, int arg1);
	virtual int send_tip_to_client(int error_id, std::string &str1);

	////////////////////////////////////////////////////////////////

	// 等级改变监听
	virtual void hook_fighter_level(const int new_level, const int src_level);

	// 总战斗力变化侦听
	virtual void hook_fighter_ttl_force(int new_force);

	void update_ttl_force(void);

	////////////////////////////////////////////////////////////////

	static int create_init_player(Logic_Player_Detail &detail);

	int login_logic(Logic_Player_Detail &detail);

	void sign_in(void);

	void sign_out(int out_reason = 0);

	void trusteeship_sign_out(void);

	void load_detail(Logic_Player_Detail &detail);

	void save_detail(Logic_Player_Detail &detail);

	void set_detail_change(bool change);

	void set_online_flag(int online);

	void set_logout_time(const Time_Value &time);

	void save_player_data(void);

	int module_init(void);
	//	每天0点触发
	int trigger_daily_zero(const Time_Value &now, bool logic_check = false);
	// 每天6点触发
	int trigger_daily_six(const Time_Value& now, bool logic_check = false);

	int trigger_weekly_zero(const Time_Value &now, bool logic_check = false);

	int exit_diff_scence(void);

	int bad_scene_protect(void);

	int monitor_link_init(void);

	int notice_gate_login_ready(void);

	int notice_scene_login_ready(int msg_id);

	int notice_chat_login_ready(void);

	int sign_in_repeat(bool is_trustership);	// 顶号调用

	int client_ready_now(void);		// 客户端初始化完成

	int move_scene_completed(void);	// 场景加载完成

	int enter_move_scene(void);	// 进入场景

	int player_login_gate(int cid);	// 通知登录网关

	int player_login_info(void);	// 登录网关返回

	////////////////////////////////////////////////////////////////

	Base_Detail const &base_detail(void) const;
	Base_Detail& base_detail_no_const();
	Logic_Player_Detail* logic_player_detail(void);

	Location const &location(void) const;

	Player_Monitor_Link const &monitor_link(void);

	virtual role_id_t role_id(void);

	virtual std::string role_name(void);

	virtual int level(void);

	virtual int career(void);

	virtual int gender(void);

	virtual bool player_has_init(void);

	virtual int scene_id(void);

	virtual role_id_t gang_id(void);

	uint32_t head_id(void);

	////////////////////////////////////////////////////////////////

	// 刷新坐标
	void refresh_mover_detail(Location &location);
	void void_coord_change(Coord &src_coord, Coord &des_coord);

	// 场景改变
	int set_scene_muid(Monitor_Unique_ID &muid);
	int hook_scene_change(int src_scene, int des_scene);
	void set_grid_coord(uint16_t x, uint16_t y);

	// 场景转换(内部调用) --------
	int inner_transmit(int &des_scene, int scene_order, int64_t arg3, std::string arg4 = "");
	int inner_transmit(int &des_scene, int scene_order, Coord coord, int64_t arg3, std::string arg4 = "");
	int scene_transmit(int &des_scene, int scene_order, int64_t arg3, Coord des_coord, std::string arg4, int32_t sort, int32_t sub_sort);
	int scene_rollback(int &des_scene, int &scene_order, Coord &coord, bool set = false,
					   int exist_des_scene = 0, int exist_des_scene_order = 0, Scene_Type rollback_type = Main_Scene_Type);
	int allow_transfer(const Monitor_Unique_ID &src_muid, const Monitor_Unique_ID &des_muid, int &des_scene, int64_t arg3, std::string arg4);
	int transfer_coord(const Monitor_Unique_ID &src_muid, const Monitor_Unique_ID &des_muid, int &des_scene, int64_t arg3, std::string arg4, Coord &coord);
	int set_create_scene_args(int type, Time_Value time, role_id_t id, int v1, int v2, int v3, bool v4 = false);
	int set_create_scene_args(bool v4 = false);
	int set_create_scene_args(const Create_Scene_Args &csa);
	// ------------------------

	int set_level(int level);
	int set_role_name(const std::string& new_name);

	int set_gang_id(const role_id_t gang_id);
	int set_gang_name(const std::string &gang_name);

	int modify_gender();

	//刷新最后登出时间
	int set_last_sign_out_time(Time_Value& time);

	///////////////////////////////////////////////////////////////
	// 其它接口尽量加到Public

	int load_base_detail(Base_Detail& detail);

	uint32_t vip(void);
	int set_vip_level(uint32_t new_vip_level);
	int set_vip_over_time(const Time_Value &new_time);

	// 称号系统
	int set_title(const int title_id);

	//送花系统配置
	void add_charm(uint32_t num);

	// 组队多层副本转送
	int multi_team_scene_transmit(const Scene_Config* dest_cfg);
	// 设置头像
	void set_head_id(uint32_t id);

	// 清空时装
	void reset_fashion_id(void);

	// 设置时装
	void set_fashion_id(int32_t id);
	// 觉醒等级
	int awake_lvl(void) const;
	// 增加觉醒等级
	void add_awake_lvl(void);

	// 刷新avatar
	void refresh_avatar(void);

	const bool get_player_in_trans_all() const;
private:
	int special_transmit(int des_scene, int scene_order, int64_t arg3, Coord &des_coord,
						std::string &arg4, int32_t sort, int32_t sub_sort);
	int team_manor_transmit(const int des_scene, int type, Time_Value time, role_id_t id,
					        int v1, int v2, int v3, bool v4 = false);
	int gang_manor_transmit(void);
	int war_scene_transmit(void);

private:
	Logic_Monitor *monitor_;
	Player_Monitor_Link monitor_link_;

	Base_Detail *base_detail_;
	Logic_Player_Detail *detail_ptr_;

	bool player_in_trans_;				// 跨进程转场景时不发到场景服
	bool player_has_init_;				// 是否同步到场景完成,初始化属性判断用
	bool player_in_trans_all_;          //正在转场景的标志(包含跨进程、同进程)
};

inline Logic_Monitor *Logic_Player::monitor(void) {
	return monitor_;
}

inline Logic_Player *Logic_Player::player_self(void) {
	return this;
}

inline Base_Detail const &Logic_Player::base_detail(void) const {
	return *base_detail_;
}

inline Base_Detail& Logic_Player::base_detail_no_const() {
	return *base_detail_;
}

inline Logic_Player_Detail* Logic_Player::logic_player_detail(void){
	return detail_ptr_;
}


#endif /* LOGIC_PLAYER_H_ */
