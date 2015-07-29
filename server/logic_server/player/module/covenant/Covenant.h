/*
 * Covenant.h
 *
 *  Created on: 2014-07-15
 *      Author: xiaoliang
 *
 *      新模块例子
 */

#ifndef COVENANT_H_
#define COVENANT_H_

#include "Logic_Player_Base.h"
#include "Config_Cache_Covenant.h"

struct Covenant_Detail;
struct Covenant_Data;

class Covenant : virtual public Logic_Player_Base {
public:
	Covenant(void);
	virtual ~Covenant();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Covenant_Detail &detail);

	// 登录时detail赋值
	void load_detail(Covenant_Detail &detail);

	// 保存时detail赋值
	void save_detail(Covenant_Detail &data);

	Covenant_Detail const &covenant_detail(void) const;

	void test_covenant(uint32_t id, int time, int type = 0);


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

	//定时监听是否已经过期
	void listen_covenant_expired(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	//客户端请求
	int req_buy_covenant(uint8_t type, uint32_t id);
	int req_handsel_covenant(uint8_t type, uint32_t id, std::string &role_name);
	int req_start_covenant_become_time(uint32_t id);

	//开通公用操作
	void open_covenant(uint32_t id, uint32_t days);
	void open_covenant_hours(uint32_t id, uint32_t hours, uint32_t type = 0);
	void open_covenant_by_card(uint32_t id, uint32_t time);


	//interface  获取开通状态接口
	bool get_covenant_status(uint32_t id);
	void get_covenant_status_effect(uint32_t id, std::vector<Status_Effect_Argv> status);
	void covenant_expired_listen(const uint32_t id, const Time_Value &over_time, const Time_Value &now);
//	void covenant_hook_hero_fight(const int32_t fight_index, const int32_t last_fight_index);
//	void covenant_hero_fight_prop_refresh(std::map<int, double> &pro_val_map);

	//检查系统模块是否开启使用
	int check_covenant_use(Logic_Player *player);



private:
	Covenant_Detail *covenant_detail_;
	bool status_has_effect_;


	void send_expired_mail(uint32_t mail_id);
	void send_active_msg(Logic_Player *player);
	void set_status(Covenant_Data &cd); //战斗buf

};

inline Covenant_Detail const &Covenant::covenant_detail(void) const{
	return *covenant_detail_;
}

#endif /* COVENANT_H_ */
