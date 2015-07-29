/*
 * Expedition.h
 *
 *  Created on: 2014-10-16
 *      Author: xiaoliang
 *
 *      新模块例子
 */

#ifndef EXPEDITION_H_
#define EXPEDITION_H_

#include "Logic_Player_Base.h"

struct Expedition_Detail;
struct MSG_10101300;
struct MSG_10101301;

class Expedition : virtual public Logic_Player_Base {
public:
	Expedition(void);
	virtual ~Expedition();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Expedition_Detail &detail);

	// 登录时detail赋值
	void load_detail(Expedition_Detail &detail);

	// 保存时detail赋值
	void save_detail(Expedition_Detail &data);

	Expedition_Detail const &expedition_detail(void) const;

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

	// msg --
	//记录玩家坐标
	int set_expedition_coord(Coord &coord);
	int expedition_data_channle(Block_Buffer &buf);
	int req_expedition_open_chest(const MSG_10101300 &msg);
	int req_expedition_chest_collect(const MSG_10101301 &msg);
	// -- msg

	void active_expedition_status(void);

	// inner --
	int sync_to_scene(const int point_id = 0);
	int sync_to_scene_chest_point(const int point_id);
	int expedition_listen_quit_gang(void);
	int handle_drops_by_point_id(const int point_id);
	int active_daily_award_info(void);
	void sync_repeat_login_to_scene(void);
	// -- inner

	// test
	int reset_award_time(void);
private:
	Expedition_Detail *expedition_detail_;
};

inline Expedition_Detail const &Expedition::expedition_detail(void) const{
	return *expedition_detail_;
}

#endif /* EXPEDITION_H_ */
