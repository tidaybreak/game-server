/*
 * Oper_Activity.h
 *
 *  Created on: Jan 21, 2014
 *      Author: XIAOLIANG
 *
 *      新模块例子
 */

#ifndef OPER_ACTIVITY_H_
#define OPER_ACTIVITY_H_

#include "Logic_Player_Base.h"
#include "pack/Pack_Struct.h"

struct Oper_Activity_Detail;
struct Oper_Activity_Project_Config;
struct Oper_Activity_Item_Line_Config;
struct Oper_Activity_Append_Effect_Config;
struct Oper_Activity_Item_Info;
struct Uint_Uint;
struct Oper_Activity_Reward_Info_Config;
struct Gain_Item_DM_Info;
struct Money_DM_Info;

class Oper_Activity : virtual public Logic_Player_Base {
public:
	Oper_Activity(void);
	virtual ~Oper_Activity();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Oper_Activity_Detail &detail);

	// 登录时detail赋值
	void load_detail(Oper_Activity_Detail &detail);

	// 保存时detail赋值
	void save_detail(Oper_Activity_Detail &data);

	Oper_Activity_Detail const &oper_activity_detail(void) const;

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
	int trigger_daily_six(const Time_Value &now);

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	//处理客户端请求相关
	int req_oper_activity_content_info(void);
	int req_oper_activity_content_reward(uint32_t act_id, uint32_t rec_id);
	int req_oper_activity_content_icon_num(void);
	int req_oper_activity_content_item(uint32_t act_id);

	//活动开始到结束加特殊效果总入口
	void oper_activity_append_effect(const Oper_Activity_Project_Config &conf, const Oper_Activity_Append_Effect_Config &ae_config);
	//-----
	void oper_activity_close_append_effect_buff_type(const Oper_Activity_Project_Config &conf, const Oper_Activity_Append_Effect_Config &ae_config);
	//活动获取信息总入口
	uint32_t oper_activity_proect_content_info(const Oper_Activity_Project_Config &proect,  Oper_Activity_Item_Info &info);
	//活动领取总入口
	int oper_activity_proect_item_line_reward(const Oper_Activity_Item_Line_Config &item_line, const uint32_t act_id);
	//运营活动监听
	void oper_activity_listen_type_common(const uint32_t type, const uint32_t num = 1, const uint32_t num_type = 1);
	void oper_activity_listen_type_common_add(const uint32_t actid, const uint32_t type, const uint32_t num = 1, const uint32_t num_type = 1);
	void oper_activity_listen_type_common_re(const uint32_t type, const uint32_t num = 1, const uint32_t num_type = 1);

	//test cmd interface
	void sub_n_day_actid_recid_last_refresh_time(int n);
	void oper_activity_add_login_day_on_test_cmd(int n);


private:
	//活动开始到结束加特殊效果相关方法组
	void oper_activity_append_effect_buff_type(const Oper_Activity_Project_Config &conf, const Oper_Activity_Append_Effect_Config &ae_config);
	void oper_activity_close_append_effect_buff_type_status_id(const uint32_t status_id);

	//进度
	void oper_activity_item_line_uu_proc(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);
	void oper_activity_item_line_uu_proc_ext1(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);
	//领取次数
	void oper_activity_item_line_uu_mum(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);
	//领取状态
	uint32_t oper_activity_item_line_uu_status(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);


	//进度 - 荣耀竞技场胜利次数 类型
	void oper_activity_item_line_uu_proc_by_common(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);
	void oper_activity_item_line_uu_proc_by_common_ext1(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);
	void oper_activity_item_line_uu_proc_by_common_type_num_day(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);
	void oper_activity_item_line_uu_proc_by_wing_level(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);
	void oper_activity_item_line_uu_proc_by_mount_on_start(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);
	void oper_activity_item_line_uu_proc_by_exchange_item(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu);

	//领取状态 - 荣耀竞技场胜利次数 类型
	void oper_activity_item_line_uu_status_by_common(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u);
	void oper_activity_item_line_uu_status_by_common_ext1(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u);
	int oper_activity_item_line_uu_status_by_exchange_item(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u);
	void oper_activity_item_line_uu_status_by_common_type_num_day(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u);
	void oper_activity_item_line_uu_status_by_wing_level(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u);
	void oper_activity_item_line_uu_status_by_mount_on_start(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u);

	//解析类型条件参数扩展
	void oper_activity_item_line_parm_2arr_to_index_amount_vec(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Id_Amount_Vec &u);

	void oper_activity_daily_refresh(void);//检测按X天刷新的所有项目的奖励
	void oper_activity_check_reset_on_restart_act(void);

	//领取通用奖励
	int oper_activity_proect_item_line_receive_reward(const Oper_Activity_Reward_Info_Config &reward_info, const Gain_Item_DM_Info &item_dm_info, const Money_DM_Info &money_dm_info_);
	int oper_activity_proect_item_line_receive_reward_on_mail(const uint32_t mail_id, const Oper_Activity_Reward_Info_Config &reward_info);

	//登录天数相关的
	void oper_activity_login_day(void);
	void oper_activity_calc_login_day(const uint32_t type, const uint32_t num = 1, const uint32_t num_type = 1);

	//other interface
	uint32_t oper_activity_get_mount_star_level_by_mount_id(uint32_t mount_id);

	Oper_Activity_Detail *oper_activity_detail_;
};

inline Oper_Activity_Detail const &Oper_Activity::oper_activity_detail(void) const{
	return *oper_activity_detail_;
}

#endif /* ACTIVITY_H_ */
