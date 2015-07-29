/*
 * Invest_Plan.h
 *
 *  Created on: Oct 14, 2014
 *      Author: gaojianshan
 */

#ifndef INVEST_PLAN_H_
#define INVEST_PLAN_H_

#include "Logic_Player_Base.h"

struct Base_Detail;
struct Invest_Plan_Detail;

class Invest_Plan : virtual public Logic_Player_Base {
public:
	Invest_Plan(void);
	virtual ~Invest_Plan();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Invest_Plan_Detail &detail, const Base_Detail &baseDetail);

	// 登录时detail赋值
	void load_detail(Invest_Plan_Detail &detail);

	// 保存时detail赋值
	void save_detail(Invest_Plan_Detail &data);

	inline const Invest_Plan_Detail& invest_plan_detail(void) const;

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

	//每天凌晨6点触发
	int trigger_daily_six(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	void save_invest_plan_tick();

	//请求打开月卡投资页面
	int req_open_month_card_invest_plan();

	//请求领取月卡投资的收益
	int req_get_month_card_profit(uint8_t day);

	//请求月卡投资
	int req_month_card_invest();

	//请求打开升级投资页面
	int req_open_upgrade_invest();

	//请求领取升级投资的收益
	int req_get_upgrade_invest_profit(uint32_t level);

	//请求升级投资
	int req_upgrade_invest_money(uint32_t money, uint8_t type);

	//一键领取所有升级投资收益
	int req_get_all_upgrade_invest_profit();

	//请求获取投资计划的历史记录
	int req_get_invest_plan_history();

	//投资计划监听玩家等级变化
	void invest_plan_listen_level_up(const int new_level);

private:
	//月卡投资合计尚有多少粉钻可领
	uint32_t get_month_card_invest_plan_left_monty();

	//升级投资总共已投资钻石数目
	uint32_t get_upgrade_invest_total_invested_money();

	//升级投资可合计尚有多少粉钻可领(等级够的、等级不够的都算)
	uint32_t get_upgrade_invest_plan_left_money();

	//升级投资达到等级，但是，总共没领取的收益
	uint32_t get_upgrade_invest_can_get_lef_money();

	//是否升级投资额度已满
	bool is_upgrade_invest_full();

	//获取某个等级已领取的所有额度之和
	uint32_t get_level_invested_money_sum(uint32_t level);

	//在投资计划内消耗货币
	int consume_money_in_invest_plan(uint8_t money_type, uint32_t money_num, uint32_t lose_money_type);

	//在投资计划内获得货币
	int gain_money_in_invest_plan(uint32_t money_type, uint32_t money_num, uint32_t gain_money_type);

	//领取升级投资等级收益，放入已经领取收益的等级对应的投资额度列表
	void getted_level_profit_to_getted_map(uint32_t level, uint32_t invest_money);

	//升级投资的马上投资
	int process_upgrade_invest_money_first(uint32_t money);

	//升级投资的追加投资
	int req_upgrade_invest_money_add_money(uint32_t money);

	//是否存在等级足够的按钮
	bool is_exist_little_level_button();

	//重置投资计划数据
	void reset_invest_plan_data();

private:
	Invest_Plan_Detail *invest_plan_detail_;
};

Invest_Plan_Detail const &Invest_Plan::invest_plan_detail(void) const{
	return *invest_plan_detail_;
}



#endif /* INVEST_PLAN_H_ */
