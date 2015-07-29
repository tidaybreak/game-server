/*
 * Single_Dungeon.h
 *
 *  Created on: Mar 17, 2014
 *      Author: Donald Shaw
 */

#ifndef SINGLE_DUNGEON_H_
#define SINGLE_DUNGEON_H_

#include "Logic_Player_Base.h"

struct Sgl_Dungeon_Detail;
struct Sgl_Dungeon_Data;
struct Location;
struct Scene_Config;

class Single_Dungeon : virtual public Logic_Player_Base {
public:
	Single_Dungeon(void);
	virtual ~Single_Dungeon();

	// init data when create role
	static int create_init(Sgl_Dungeon_Detail &detail);
	static int a_new_single_dungeon(Sgl_Dungeon_Detail& detail, int DungeonId);
	static int a_new_elite_dungeon(Sgl_Dungeon_Detail& detail, int DungeonId, int difficulty);
	// load single dungeon's detail when role login
	void load_detail(Sgl_Dungeon_Detail &detail);
	// save single dungeon's detail
	void save_detail(Sgl_Dungeon_Detail &detail);

	const Sgl_Dungeon_Detail& sgl_dungeon_detail(void) const;

	//initial module when role login
	void module_init(void);
	//callback after role login
	void sign_in(void);
	//callback when role logout
	void sign_out(void);
	// 场景加载完成
	int move_scene_completed(void);

	// refresh data in the morning of everyday
	void daily_refresh(void);

	// 凌晨0点刷新
	int trigger_daily_zero(const Time_Value &now, bool logic_check = false);
	// 凌晨6点刷新
	int trigger_daily_six(const Time_Value &now, bool logic_check = false);

	void reset(void);

	int client_ready_now(void);		// 客户端初始化完成

	int check_enter_dungeon(int dungeon_id, int role_lv);
	int check_exit_dungeon(const Location& loc);
	// 单人副本通关
	void clear_single_dungeon(const Scene_Config* scene_config, bool is_no_profit = false);
	// 计算副本通关奖励
	int settle_reward(Sgl_Dungeon_Data &dungeon_data, int layer_id, bool is_no_profit = false);
	// 重置清除怪点数
	void reset_clear_monster_counter(int dungeon_id);
	// 精英副本通关
	void clear_elite_dungeon(const Scene_Config* scene_config);
	// 获取单人副本状态 0：完成 1：未完成
	int get_single_dungeon_status(const int scene_id);
	// 获取精英副本状态（status 0：未解锁 1：激活 2：未激活 3：未打（通关的） 4：已打（通关的））
	void get_elite_dungeon_status(const int scene_id, int& status);
	// 上古检测对应单人副本
	bool check_relation_single_dungeon(const int scene_id);
	// 上古检测对应上古副本
	bool check_relation_elite_dungeon(const int scene_id);
	// 单人副副本是否打通关
	bool is_single_dungeon_complete(const int scene_id);
	// 精英副本是否打通关
	bool is_elite_dungeon_complete(const int scene_id);
	// 副本是否可收获
	bool is_elite_dungeon_unreap(const int scene_id);

	//获取精英副本今日可以打的个数(含已经打过的,未打过的)
	int get_elite_dungeon_activation_num(void);
	// 获取上古副本挑战的副本id
	int get_elite_challenge_id(const int scene_id);
	//  检测上古副本是否可以打
	int check_elite_challenge(const int scene_id);
	//
	int check_elite_tran(const int scene_id);
	// 是否在扫荡中
	bool is_in_wipe_out(void);

public:
	int req_fetch_single_dungeon_info(void);
	int req_fetch_elite_dungeon_info(const int8_t type);
	int req_reset_elite_dungeon_times(int bind_gold_first);
	int req_receive_chapter_award(int chapter_id);
	int req_elite_buy_add_num(const int scene_id);
	int req_receive_elite_first_pass(bool to_client = true, bool hook_gain = true);
	void add_elite_reset_times(void);
	// 获取下一个激活的可以打普通精英副本(助手使用)
	int get_normal_elite_active_id(void);
	// 体力恢复
	int listen_strength_recover(const Time_Value& now);
	// 扫荡副本
	int listen_wipe_out_dungeon(const Time_Value& now);

	// 请求扫荡
	int req_wipe_out(const int wipe_type, const int src_wipe_num, const int src_scene_id);
	// 请求中断扫荡
	int req_interrupt_wipe_out(const int wipe_type);
	// 请求完成扫荡
	int req_complete_wipe_out(const int wipe_type);
	// 请求获得双倍扫荡收益
	int req_obtain_wipe_double_income(const int wipe_type);
	// 请求勾选双倍扫荡
	int req_select_wipe_double(const int wipe_type, const int is_selected);
	// 请求翻牌
	int req_open_single_card(int index);
	// 重新激活精英副本(测试使用)
	void act_first_elite_dungeon(void);
	void act_first_single_dungeon(void);
	bool is_sgl_dungeon_complete(int scene_id);
	int get_shanggu_complete_num(void);
	int get_lord_temple_complete_num(void);

	// 副本获得侦听
	void hook_fb_gain_item(const int scene_type, const int scene_id, const std::vector<Item_Info> items);
	// 任务增加上古副本挑战次数
	void task_add_elite_challenge_num(const int add_num);
	// 获取上古副本最后通关的boss层id
	int get_last_elite_boss_scene_id(void);

	void activate_lord_dungeon(void);
private:
	Sgl_Dungeon_Data* get_single_dungeon_data(int scene_id);
	Sgl_Dungeon_Data* get_elite_dungeon_data(int scene_id, int difficulty);
	int get_elite_nor_from_single(int scene_id);
	int get_elite_hor_from_single(int scene_id);
	int get_single_from_elite(int scene_id);
	// 获取单人副本通关数量
	int get_single_complete_num(void);
	// 获取上古副本通关数量
	int get_nor_elite_complete_num(void);
	//
	int get_hor_elite_complete_num(void);
	// 计算单人副本扫荡结果
	int dungeon_wipe_out(const int wipe_type, bool to_client = true);
	int calc_wipe_result(const int wipe_type, const int scene_id, std::vector<Item_Info>& items, int& cost_str);
	void add_wipe_items(const int item_id, const int num, std::vector<Item_Info>& items);

	void init_wipe_out(void);

	// 获取扫荡的精英副本
	int get_wipe_elite_id(const int wipe_type, std::vector<int>& wipe_scenes);
	// 普通精英扫荡开启
	bool is_nor_elite_wipe_out_active(const int scene_id);
	// 噩梦精英扫荡开启
	bool is_hor_elite_wipe_out_active(const int scene_id);
	// 单人副本通关侦听
	void hook_single_dungeon_first_complete(const int scene_id);
	// 单人副本章节通关通关侦听
	void hook_single_dungeon_chapter_complete(const int chapter_id);

	// 是否可以免费挑战上古副本
	bool is_elite_free_challenge(void);
	// 上古副本获得物品
	void elite_gain_items(const std::vector<Item_Info> items, const int diff);
	void send_elite_trail_info_to_client(const int diff);
	// //临时检测上古副本扫荡是否开启
	void check_elite_wipe_open(void);
	void activate_lord_temple(void);
private:
	Sgl_Dungeon_Detail *sgl_dungeon_detail_;
	// 离线扫荡次数
	int offline_wipe_num_;
	// 准备翻牌
	int is_opening_card_;
	int opening_card_scene_id_;
	//
	bool is_sg_first_pass_;
	int sg_first_pass_scene_id_;
};

inline const Sgl_Dungeon_Detail& Single_Dungeon::sgl_dungeon_detail(void) const {
	return *sgl_dungeon_detail_;
}

#endif /* SINGLE_DUNGEON_H_ */
