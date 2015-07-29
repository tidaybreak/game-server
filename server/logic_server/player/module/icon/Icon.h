/*
 * Icon.h
 *
 *  Created on: 2014年5月22日
 *      Author: gaojianshan
 */

#ifndef ICON_H_
#define ICON_H_

#include "Logic_Player_Base.h"
#include "Msg_Struct.h"

struct Icon_Detail;
struct Base_Detail;
struct Icon_Base_Config;
struct Svr_Icon_Info;

class Icon : virtual public Logic_Player_Base {
public:
	Icon(void);
	virtual ~Icon();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Icon_Detail &iconDetail, const Base_Detail &baseDetail);

	// 登录时detail赋值
	void load_detail(Icon_Detail &detail);

	// 保存时detail赋值
	void save_detail(Icon_Detail &data);

	inline const Icon_Detail& icon_detail(void) const;
	inline Icon_Detail& icon_detail_no_const();

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

	//满足等级条件，推送开启图标消息
	void notify_open_icon_by_level(int new_level);

	//加入公会，通知客户端开启图标
	void notify_open_icon_by_join_gang();

	//满足任务状态,通知客户端开启图标
	void notify_open_icon_by_task_status();

	//使用道具，通知客户端开启图标
	void notify_open_icon_by_use_item(uint32_t item_id);

	//是否等级开启条件
	static bool is_level(const Icon_Base_Config &cfg);

	//是否任务状态开启条件
	static bool is_task(const Icon_Base_Config &cfg);

	//是否使用道具开启条件
	static bool is_use_item(const Icon_Base_Config &cfg);

	//是否加入公会开启条件
	static bool is_join_gang(const Icon_Base_Config &cfg);

	//检查是否满足任务状态开启条件
	bool is_all_task_pass(const Icon_Base_Config &cfg);

	//根据图标id查找玩家身上的图标开启/关闭信息
	Svr_Icon_Info* find_icon(uint16_t icon_id);

	//判断指定id的图标是否开启状态(给其他模块的接口)
	bool is_icon_open(uint16_t icon_id);

	//图标开启时，调用此接口，给其他模块监听
	void hook_icon_open(uint16_t icon_id);

	//其他模块通知图标去关闭图标(暂时只有英雄招募用到)
	void other_module_notify_icon_to_close_icon(uint16_t icon_id);

	//引导保底开启图标
	void safe_guide_open_icon(uint16_t icon_id);

private:
	Icon_Detail *icon_detail_;
};

const Icon_Detail& Icon::icon_detail(void) const{
	return *icon_detail_;
}

Icon_Detail& Icon::icon_detail_no_const() {
	return *icon_detail_;
}

#endif /* ICON_H_ */
