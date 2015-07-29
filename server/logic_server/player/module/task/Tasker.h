/*
 * Tasker.h
 *
 *  Created on: Nov 26, 2013
 *      Author: ti
 */

#ifndef TASKER_H_
#define TASKER_H_

#include "Logic_Player_Base.h"
#include "pack/Pack_Struct.h"
#include "task/Task_Def.h"

class Task_Implement;

struct TaskInfo;
struct Tasker_Detail;
struct Task_Mission_Detail_Config;
struct Task_Info;
struct MSG_50110000;
struct Reward_Task_Config;
struct MSG_50110012;
struct MSG_81110007;
struct MSG_50110018;

class Tasker : virtual public Logic_Player_Base {
public:
	typedef boost::unordered_map<int, TaskInfo> TaskMap;
	typedef boost::unordered_set<int> TaskSet;
	typedef boost::unordered_set<int> TaskIdSet;
	typedef Hash_Map<int, TaskIdSet, NULL_MUTEX> Item_TaskIdSet_Map; //key:item_id value:TaskIdSet

	Tasker();
	virtual ~Tasker();

	// 创建角色时初始化
	static int create_init(Tasker_Detail &detail);
	// 登录时detail赋值
	void load_detail(Tasker_Detail &detail);
	// 保存时detail赋值
	void save_detail(Tasker_Detail &detail);

	inline const Tasker_Detail& tasker_detail(void) const;
	inline Tasker_Detail& tasker_detail_no_const();

	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	// 场景加载完成
	int move_scene_completed(void);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

	//每天凌晨6点触发
	int trigger_daily_six(const Time_Value &now);

	//每天凌晨6点自动刷新悬赏榜，重置每日免费刷新次数、每日可领取次数
    void daily_refresh_reward_task_list(const Time_Value &now);

    void reset(void);

    void save_tick(void);

    int sync_task_module_info(Block_Buffer &buf);

    int task_construct(bool is_level_up = false);
    virtual int init_task(const uint32_t task_id, const Task_Mission_Detail_Config &json, TaskInfo &taskinfo);
    virtual int insert_task(uint32_t task_id, const Task_Mission_Detail_Config &json);
    int task_auto_accept(uint32_t task_id);

    virtual int task_accept(uint32_t task_id);
    virtual int task_abandon(uint32_t task_id);
    virtual int task_submit(uint32_t task_id);

    virtual int task_get_list(void);    // 获取任务列表
//    virtual int task_npc_list(int npc_id, uint8_t source);    // 获取npc列表
    virtual int task_fast_finish(const uint32_t task_id, const uint8_t type, const uint8_t is_bind_diamond_first); // 快速完成任务
//    virtual int task_plot(uint32_t task_id, int8_t action);
    virtual int task_after_check_npc(uint32_t npc_id);

    virtual bool is_task_submited(uint32_t task_id);
    virtual bool is_nearby_npc(uint32_t task_id);

//    virtual int bind_task(uint32_t task_id, TaskInfo &info);
    virtual int unbind_task(uint32_t task_id);
    virtual TaskInfo *find_task(uint32_t task_id);

//    virtual Tasker_Detail &task_map(void);
    TaskSet& task_accepted_monster_set(void);
    TaskSet& task_accepted_fb_set(void);
    TaskSet& task_accepted_arrive_set(void);
    TaskSet& task_accepted_lvl_set(void);
    TaskSet& task_accepted_package_set(void);
    TaskSet& task_accepted_monster_group_set(void);
    TaskSet& task_accepted_collect_set(void);
    TaskSet& task_accepted_fb_type_set(void);
    TaskSet& task_accepted_win_player_set(void);
    TaskSet& task_accepted_join_gang_set();
    TaskSet& task_accepted_ganger_level_set();
    TaskSet& task_accepted_player_all_equip_set();
    TaskSet& task_acccepted_body_equip_set();
    TaskSet& task_accepted_body_equip_level_set();
    TaskSet& task_accepted_equip_level_set();
    TaskSet& task_accepted_stone_level_set();
    TaskSet& task_accepted_buy_goods_set();
    TaskSet& task_accepted_get_hero_set();
    TaskSet& task_accepted_fight_set();
    TaskSet& task_accepted_commit_consume_item_set();
    TaskSet& task_accepted_deliver_letter_set();
    TaskSet& task_accepted_help_other_complete_team_scene_set();
    TaskSet& task_accepted_sell_item_set();
//    TaskSet& task_accepted_use_skill_set();
//    TaskSet& task_accepted_append_skill_set();
    TaskSet& task_accepted_limit_time_set();
    TaskSet& task_accepted_arena_set();
    TaskSet& task_accepted_chat_with_friend_set();
    TaskSet& task_accepted_send_mail_set();
    TaskSet& task_accepted_team_scene_or_pvp_set();
    TaskSet& task_accepted_browse_other_player_info_set();
    TaskSet& task_accepted_npc_talk_set();
    TaskSet& task_accepted_arena_level_set();
    TaskSet& task_accepted_equip_crimson_level_set();
    TaskSet& task_accepted_arena_all_set();
    TaskSet& task_accepted_create_dragon_vale_building_set();
    TaskSet& task_accepted_gain_dragon_vale_builiding_production_set();
    TaskSet& task_accepted_wake_up_hero_to_quality_level_set();
    TaskSet& task_accepted_keep_hero_to_level_set();
    TaskSet& task_accepted_compose_equip_set();
    TaskSet& task_accepted_hero_dream_scene_set();
    TaskSet& task_accepted_ganger_skill_level_set();
    TaskSet& task_accepted_war_set();
    TaskSet& task_accepted_war_all_set();
    TaskSet& task_accepted_kill_enemy_in_war_set();
    TaskSet& task_accepted_single_arena_all_set();
    TaskSet& task_accepted_single_arena_win_set();
    TaskSet& task_accepted_team_arena_all_set();
    TaskSet& task_accepted_team_arena_win_set();
    TaskSet& task_accepted_ganger_bless_buf_set();
    TaskSet& task_accepted_rob_other_dragon_vale_all_set();
    TaskSet& task_accepted_dragon_vale_step_foot_all_set();
    TaskSet& task_accepted_activeness_of_today_set();
    TaskSet& task_accepted_dragon_vale_building_upgrade_set();
    TaskSet& task_accepted_limit_time_kill_boss_set();
    TaskSet& task_accepted_dragon_keep_all_set();
    TaskSet& task_accepted_dragon_keep_success_set();
    TaskSet& task_accepted_donate_to_gang_set();
    TaskSet& task_accepted_present_buf_to_other_set();
    TaskSet& task_accepted_finish_reward_task_num_set();
    TaskSet& task_accepted_keep_dragon_to_level_set();
    TaskSet& task_accepted_knight_trial_set();
    TaskSet& task_accepted_suit_set();
    TaskSet& task_accepted_role_pull_on_equip_set();
    TaskSet& task_accepted_hero_pull_on_equip_set();
    TaskSet& task_accepted_activate_wing_set();
    TaskSet& task_accepted_get_quality_rune_set();
    TaskSet& task_accepted_pass_kngiht_trials_set();
    TaskSet& task_accepted_have_hero_set();
    TaskSet& task_accepted_have_hero_quality_set();
    TaskSet& task_accepted_first_pass_scene_set();
    TaskSet& task_accepted_dragon_born_set();
    TaskSet& task_accepted_horse_star_level_set();
    TaskSet& task_accepted_have_item_but_no_erase_set();
    TaskSet& task_accepted_have_equip_but_no_erase_set();
    TaskSet& task_accepted_same_gang_challenge_all_set();
    TaskSet& task_accepted_main_scene_npc_battle_set();
    TaskSet& task_accepted_equip_card_all_set();
    TaskSet& task_accepted_equip_card_by_position_set();
    TaskSet& task_accepted_card_upgrade_to_level_all_set();
    TaskSet& task_accepted_card_upgrade_to_level_by_position_set();
    TaskSet& task_accepted_lottery_draw_set();
    TaskSet& task_accepted_equip_jewelry_set();
    TaskSet& task_accepted_role_equip_n_set();
    TaskSet& task_accepted_hero_equip_n_set();
    TaskSet& task_accepted_invite_friend_step_foot_set();
    TaskSet& task_accepted_kill_group_num_set();
    TaskSet& task_accepted_did_reward_task_num_set();
    TaskSet& task_accepted_alchemy_num_set();
    TaskSet& task_accepted_did_reward_task_num_today_set();
    TaskSet& task_accepted_remote_scene_any_pass_set();
    TaskSet& task_accepted_knight_trial_any_pass_set();
    TaskSet& task_accepted_compose_gemstone_level_set();
    TaskSet& task_accepted_inlay_position_gemstone_set();
    TaskSet& task_accepted_inlay_any_gemstone_set();
    TaskSet& task_accepted_world_boss_set();
    TaskSet& task_accepted_world_boss_immediate_revive_set();
    TaskSet& task_accepted_enchant_any_equip_set();
    TaskSet& task_accepted_inlay_id_gemstone_set();
    TaskSet& task_accepted_gang_well_recover_spirit_set();
    TaskSet& task_accepted_equip_purple_ring_set();
    TaskSet& task_accepted_inlay_level_gemstone_set();
    TaskSet& task_accepted_upgrade_card_set();

    TaskIdSet& task_submited_once_set(void);
    Item_TaskIdSet_Map& task_listen_item_map();
    Npc_Item_Chance_Set_Map& task_listen_item_source_map();

    virtual int init_task_listen_item(TaskInfo *task_info);
    virtual int erase_task_listen_item(TaskInfo *task_info, bool is_test = false);
    virtual bool is_task_listen_item(const int item_id);

    //通知场景可以继续采集任务物品或者收集打怪掉落物品
    virtual int init_task_listen_item_source(TaskInfo *task_info);
    //通知场景不能继续采集任务物品或者收集打怪掉落物品
    virtual int erase_task_listen_item_source(TaskInfo *task_info);
    virtual int sync_item_source(const int8_t type, const Npc_Item_Chance_Map &npc_item_chance_map, const TaskIdSet &task_id_set);
    virtual int sync_item_source();

    //监听杀怪任务
    int task_listen_kill_monster(const int monster_id, const int monster_num);

    //监听完成副本任务
    int task_listen_fb(const int sceneid);

    //监听到达目的地的任务
    int task_listen_arrive(const int sceneid, const int target_x, const int target_y);

    //监听等级的任务
    int task_listen_lvl_up(const int target_level);

    //监听采集物品的任务
    int task_listen_collect_item(const int item_id, const int item_num, const int bind = -1);

    //杀指定场景指定组怪    scene_id:场景id  monster_group_id:怪物组id
    int task_listen_kill_monster_group(const int sceneid, const int monster_group_id);

    //监听收集打怪掉落物品
    int task_listen_package_item(const int item_id, const int item_num, const int bind = -1);

    //监听完成某类型副本任务
    int task_listen_fb_type(const int type);

    //监听击败玩家
    int task_listen_win_player(const role_id_t role_id);

    //监听加入公会的任务  gang_id : 公会id
    int task_listen_join_gang(gang_id_t gang_id);

    //监听公会等级的任务  gang_level : 公会等级
    int task_listen_gang_level(gang_id_t gang_id, const int gang_level);

    //监听玩家强化N次的任务(没有装备或部位的限制)  equip_num:对所有部位的累计强化次数
    int task_listen_player_all_equip(const int equip_num);

    //监听强化指定部位N次的任务(有部位的限制)  position: 身体部位   equip_num:该部位累计强化次数
    int task_listen_body_equip(int position, int equip_num);

    //监听强化指定部位的装备到N级的任务(有部位的限制)  position: 身体部位  equip_level:装备等级
    int task_listen_body_equip_level(int position, int equip_level);

    //监听强化指定装备到N级的任务  equip_id:装备id   equip_level:装备等级
    int task_listen_equip_level(uint32_t equip_id, const int equip_level);

    //监听宝石提升到N级的任务  stone_level:宝石等级
    int task_listen_stone_level(const int stone_level);

    //监听在商城购买某件物品的任务
    int task_listen_buy_goods(Item_Vec::const_iterator item_begin, Item_Vec::const_iterator item_end);

    //监听获得指定英雄的任务   hero_id: 英雄id
    int task_listen_get_hero(int32_t hero_id);

    //监听挑战NPC(与NPC对话进入战斗)的任务  scene_id:场景id  monster_group_id:怪物组id
    int task_listen_fight(const int scene_id, const int monster_group_id);

    //监听提交时消耗XX道具（ID）XX个（数量）的任务    item_id: 消耗道具ID    item_num:   消耗道具数量
    int task_listen_commit_consume_item(uint32_t item_id, uint32_t item_num);

    //监听送信任务   item_id: 领取任务时获得特定道具（ID），提交时需要扣除该道具（ID）
    int task_listen_deliver_letter(uint32_t item_id, uint32_t item_num);

    //监听协助其他玩家完成多人副本N次的任务(对副本无限制，策划只配置次数)   num:  协助其他玩家完成多人副本的次数
    int task_listen_help_other_complete_team_scene(uint32_t num);

    //监听出售X件道具的任务（道具无限制，只检测出售个数）   num:  出售道具的个数
    int task_listen_sell_item(uint32_t num);

    //监听竞技场战斗胜利(给竞技场模块的接口)
    int task_listen_arena();

    //任务监听与X个好友聊天(每个好友只要1次聊天即可)
    int task_listen_chat_with_friend(role_id_t friend_id);

    //任务监听发送XXX封邮件(只限制封数，不限制给谁发)
    int task_listen_send_mail();

    //任务监听XXX次多人战斗胜利. 即:多人副本或者多人组队竞技胜利
    //注明： (1)多人副本只要胜利即可, 有收益、无收益均可. (2)多人组队竞技，只要胜利即可)
    int task_listen_team_scene_or_pvp();

    //任务监听查看XXX个玩家的信息
    int task_listen_browse_other_player_info(role_id_t role_id);

    //任务监听NPC对话任务
    int task_listen_npc_talk(uint32_t npc_id);

    //任务监听指定组id的竞技场到XX段位(包括以上)  group_id:竞技场group id     level:竞技场段位
    int task_listen_arena_level(uint32_t group_id, uint32_t level);

    //任务监听装备符文充能到XX级  position:部位   level:符文等级
    int task_listen_equip_crimson_level(uint32_t position, uint32_t level);

    //任务监听参与XX次竞技场(不论胜负，单人竞技、组队竞技都可以)
    int task_listen_arena_all();

    //任务监听指定类型的龙谷建筑建造XX个
    int task_listen_create_dragon_vale_building(uint8_t building_type);

    //任务监听收获XX次指定类型的龙谷建筑资源  building_type:建筑类型
    int task_listen_gain_dragon_vale_builiding_production(uint8_t building_type);

    //任务监听将指定英雄觉醒至指定的觉醒品质(如：蓝色、绿色)   hero_id:英雄id  quality:觉醒品质
    int task_listen_wake_up_hero_to_quality_level(uint32_t hero_id, uint32_t quality);

    //任务监听将指定英雄培养至指定的等级  hero_id:英雄id   hero_level:英雄等级
    int task_listen_keep_hero_to_level(uint32_t hero_id, uint32_t hero_level);

    //任务监听合成指定装备  equip_id:装备id
    int task_listen_compose_equip(uint32_t equip_id);

    //任务监听参与XX次英雄梦境    id:英雄梦境副本id
    int task_listen_hero_dream_scene(uint32_t id);

    //任务监听指定公会技能提升到XX级(玩家学习的公会技能)    id:公会技能id
    int task_listen_ganger_skill_level(uint32_t id);

    //任务监听参与并打赢战场
    int task_listen_war();

    //参与战场X次(打赢、打输都可以)
    int task_listen_war_all();

    //在战场中击杀X个敌人      num:击杀敌人数目
    int task_listen_kill_enemy_in_war(uint32_t num);

    //单人竞技参与X次(打赢、打输都可以)
    int task_listen_single_arena_all();

    //单人竞技参与并打赢X次
    int task_listen_single_arena_win();

    //组队竞技参与X次(打赢、打输都可以)
    int task_listen_team_arena_all();

    //组队竞技参与并打赢X次
    int task_listen_team_arena_win();

    //公会抽奖X次
    int task_listen_ganger_bless_buf();

    //龙谷掠夺X次(只要参与掠夺别人的龙谷战斗即可，不管输赢，不管是否掠夺资源)
    int task_listen_rob_other_dragon_vale_all();

    //龙谷踩脚印X次(只要踩脚印即可，不管是否提升星级)
    int task_listen_dragon_vale_step_foot_all();

    //当天活跃度达到X     num:当天活跃度
    int task_listen_activeness_of_today(uint32_t num);

    //龙谷建筑升级到X级
    int task_listen_dragon_vale_building_upgrade(uint8_t type, uint16_t level);

    //限时击杀boss
    int task_listen_limit_time_kill_boss(uint32_t monster_id, uint32_t monster_num);

    //龙培养X次(只要花钱或者道具培养了，不管是否培养成功)
    int task_listen_dragon_keep_all();

    //龙培养X次(龙培养成功，培养到下一阶才算)
    int task_listen_dragon_keep_success();

    //公会捐献x次
    int task_listen_donate_to_gang();

    //公会成员赠送buf给他人 x次
    int task_listen_present_buf_to_other();

    //完成悬赏任务数量
    int task_listen_finish_reward_task_num();

    //将龙培养到指定阶数
    int task_listen_keep_dragon_to_level(uint8_t keep_type, uint32_t keep_level);

    //骑士试炼通关xx关    cur_points:当前通关到第几关
    int task_listen_knight_trial(uint8_t cur_points);

    //某个套装的对应的部件全部穿在身上(玩家职业对应的可以穿的全部) id:套装id
    int task_listen_suit(int id, int curr_count);
    int task_listen_role_hero_suit(int item_id, int hero_index = 0);

    //主角穿指定的装备 equip_id:装备id
    int task_listen_role_pull_on_equip(uint32_t equip_id);

    //英雄穿指定的装备  equip_id:装备id
    int task_listen_hero_pull_on_equip(uint32_t equip_id);

    //激活翅膀
    int task_listen_activate_wing();

    //获得x品质的符石xx个
    int task_listen_get_quality_rune(uint32_t quality, uint32_t num);

    //通关骑士试炼x次(从第一关打到最后一关，所有关都打完才算通关一次)
    int task_listen_pass_kngiht_trials();

    //拥有多少个英雄   hero_num:拥有的所有英雄总数量
    int task_listen_have_hero(uint32_t hero_num);

    //拥有多少个x品质的英雄  quality:英雄品质  hero_num:品质对应的英雄总数量
    int task_listen_have_hero_quality();

    //首通单人副本、精英副本(接任务前通关过、接任务后通关都算) (不限制是第几次通关)
    int task_listen_first_pass_scene(uint32_t scene_id);

    //龙谷龙蛋孵化
    int task_listen_dragon_born();

    //指定id的坐骑升到xx星级
    int task_listen_horse_star_level(uint32_t horse_id, uint32_t star_level);

    //拥有某个道具(不包括符石)，仅限于人物背包段中，不扣道具
    int task_listen_have_item_but_no_erase(uint32_t item_id, uint32_t item_num);

    //拥有某个装备(不包括符石)，主角穿戴的装备、英雄穿戴的装备、人物背包段中的未穿戴的装备，不扣除
    int task_listen_have_equip_but_no_erase(uint32_t item_id, uint32_t item_num);
    void task_listen_have_equip_but_no_erase();

    //前往擂台与同公会玩家决斗X次(不论输赢，都算)
    int task_listen_same_gang_challenge_all();

    //主城npc战斗任务
    int task_listen_main_scene_npc_battle(uint32_t monster_id, uint32_t num);

    //向任意装备镶嵌兵魂
    int task_listen_equip_card_all(int num);
    int task_listen_equip_card();

    //向指定部位镶嵌兵魂
    int task_listen_equip_card_by_position(const uint32_t position, int num);

    //将任意兵魂升级到XX级
    int task_listen_card_upgrade_to_level_all(const uint32_t level);

    //将某一个部位兵魂升级到XX级
    int task_listen_card_upgrade_to_level_by_position(const uint32_t position, const uint32_t level);

    //博彩N次
    int task_listen_lottery_draw(const uint32_t times);

    //穿戴4件紫色或紫色以上（金色）的首饰
    int task_listen_equip_jewelry();

    //主角穿戴n件某色或某色以上的装备
    int task_listen_role_equip_n();

    //英雄穿戴n件某色或某色以上的装备
    int task_listen_hero_equip_n();

    //邀请好友踩脚印N次
    int task_listen_invite_friend_step_foot();

    //击杀怪物组
    int task_listen_kill_group_num(const uint32_t group_id);

    int task_listen_did_reward_task_num(const uint32_t num);

    int task_listen_alchemy_num();
    int task_listen_did_reward_task_num_today(const uint32_t num);
    int task_listen_remote_scene_any_pass();
    int task_listen_knight_trial_any_pass();
    void task_listen_compose_gemstone_level(uint32_t stone_level, uint32_t num);
    void task_listen_inlay_gemstone();
    void task_listen_inlay_gemstone_position(uint32_t position, uint32_t num);
    void task_listen_inlay_gemstone_any(uint32_t num);
    void task_listen_world_boss();
    void task_listen_world_boss_immediate_revive();
    void task_listen_enchant_any_equip();
    void task_listen_inlay_gemstone_id(uint32_t stone_id, uint32_t num);
    void task_listen_gang_well_recover_spirit();
    void task_listen_equip_purple_ring();
    void task_listen_inlay_level_gemstone(uint32_t gemstone_level, uint32_t gemstone_num);
    void task_listen_upgrade_card();
/*
     *  监听使用某个技能XX次的任务
     *  skill_id: 技能id
    int task_listen_use_skill(uint32_t skill_id);

     *  监听释放XX次追加技的任务
    int task_listen_append_skill();*/

	bool task_listen_item_update(const int item_id, const int num);
	void task_listen_item_update(void);

    virtual int notify_task_add(TaskInfo &task_info, const Task_Mission_Detail_Config &task_json);
    virtual int notify_task_del(TaskInfo &task_info);
    virtual int notify_task_update(TaskInfo &task_info, const Task_Mission_Detail_Config &task_json);
//    virtual int notify_task_update_ready(TaskInfo &task_info, Block_Buffer &buf);

    virtual int serialize_task(Block_Buffer &buf);
    virtual int unserialize_task(Block_Buffer &buf);

    int req_task_add_npc(int32_t npc_type, int8_t set, int64_t npc_id,
			std::string funcName, int32_t direction, std::string sceneId,
			std::string open, int32_t x, int32_t y, std::string npc_name);

    //获取任务状态
    int task_state(uint32_t task_id);

    //判断指定任务完成条件是否需要监听物品    execute_type: 任务完成条件类型
    bool task_execute_type_is_need_listen_item(int execute_type);

    //处理接任务时，回满怒气
    void process_full_prop_when_task_accepted(uint32_t task_id);

    //处理交任务时，清除怒气
    void process_clear_full_prop_when_task_commit(uint32_t task_id);

    //第一个主线任务特殊处理
    void sync_scene_special_process_first_task();

    //添加限时任务到监听任务集
    void add_limit_time_task_to_listen_set(TaskInfo *task_info);
    void add_limit_time_task_to_listen_set(uint32_t task_id);

    //监听限时任务
    void task_listen_limit_time(const Time_Value &now);

    //计算限时任务剩余时间
    void calc_remain_time_of_limit_time_task(Task_Info &msg_task_info,
    		const TaskInfo &svr_task_info, const Task_Mission_Detail_Config &task_json);

    //接任务时，添加/删除英雄
    void add_or_del_hero_when_task_accepted(TaskInfo &task_info, uint8_t hero_op, uint32_t hero_id);

    //判断是不是第一个主线任务
    bool is_first_main_line_task(const TaskInfo &task_info) const;

    //判断是否已有主线任务(主线任务在任务列表中只能有一个)
    bool is_has_accepted_or_finished_main_line_task() const;
    bool is_has_visible_main_line_task() const;
    int get_visiable_main_line_task_id() const;
    bool is_last_main_line_task_submit() const;

    //请求打开悬赏榜
    int req_open_reward_task_list();

    //请求悬赏任务列表  is_refresh: 是否请求刷新。0:否， 1：是。
    int req_reward_task_list(uint8_t is_refresh);

    //构造可接悬赏任务集合
    int reward_task_construct(const Task_Mission_Detail_Config &cfg);

    //更新悬赏榜显示状态:  接受任务、 放弃任务、 提交任务、 限时任务失败
    void send_to_client_reward_task_list_show_status();

    //设置悬赏榜显示状态  op: 1:接受任务  2:放弃任务  3:提交任务 4:限时任务失败
    void set_reward_task_list_show_status(uint32_t task_id, uint8_t op);

    //自动刷新悬赏任务列表
    void listen_auto_refresh_reward_task_list(const Time_Value &now);

    //处理非悬赏任务奖励
    int process_receive_normal_task_award(const Task_Mission_Detail_Config &cfg);

    //处理悬赏任务奖励
    int process_receive_reward_task_award(const TaskInfo &task_info, const Task_Mission_Detail_Config &cfg);

    //处理日常周常任务奖励
    int process_receive_day_week_task_award(const TaskInfo &task_info);

    //处理环任务奖励
    int process_receive_ring_task_award(const uint32_t task_id);

    //判断在背包中是否可以使用并扣除悬赏令(给背包模块的接口)   成功返回0，失败返回错误码
    int is_consume_reward_token(uint32_t item_id, uint32_t item_num);

    //NPC对话任务，完成对话后，请求完成任务
    int reqFinishNpcTalkTask(uint32_t npc_id);

    //今日当前是否可以做悬赏任务(提供给TOD助手的接口)
    bool is_can_do_reward_task();

    //每日可做悬赏任务总次数(提供给TOD助手的接口)
    uint32_t total_num_of_can_do_reward_task();

    //今日当前还可以做悬赏任务的次数(提供给TOD助手的接口)
    uint32_t num_of_can_do_reward_task();

    //日常任务刷新
    void refresh_day_task(uint8_t time);

    //周常任务刷新
    void refresh_week_task(uint8_t wday, uint8_t time);

    //激活坐骑
    void process_activate_horse(uint32_t task_id);

    //记录任务流水
    void Log_Task_Stream(int64_t role_id, uint32_t task_id, uint8_t task_stream_status,
    			uint32_t player_level, int64_t time_sec);

    //处理接任务时的任务监听
    int process_task_listen_when_accepted(uint32_t task_id);

    //添加buf
    void process_add_buf(uint32_t task_id);

    void process_transfer_point(uint32_t task_id);

    /////////////////////////////////////////////////////////////////////////////////////
    int process_accept_imp(uint32_t task_id, bool is_auto_accept = true);
    int process_abandon_imp(TaskInfo *task_info);
    int process_submit_imp(TaskInfo *task_info, bool is_test = false);
    int process_fast_finish_imp(TaskInfo *task_info, const uint8_t is_bind_diamond_first);
    int process_finish_imp(TaskInfo *task_info, bool is_test = false);
    /////////////////////////////////////////////////////////////////////////////////////

    //引导保底  if level up triggers this, task_id could be 0
    void process_safe_guide(uint32_t task_id = 0);

    // 获取任务掉落物品
    Int_Int_Vec get_item_list(const int npc_id);

   //完成在某个id之前的所有任务
   int test_process_finish_all_task_befor_id(uint32_t task_id);

   //判断魔翼相关任务的最后一个任务是否提交
   bool is_last_magic_wing_task_submit();

   //任务监听vip等级变化
   void task_listen_vip_level_change(uint32_t new_vip_level);

   //请求提交道具
   int req_commit_item(uint32_t task_id);

   //判断主线任务是否提交
   bool is_main_task_commit(int task_id);

   void ring_task_listen_player_level_change(const int new_level);
   void refresh_open_ring_task();
   void refresh_close_ring_task();

   //请求恢复任务链
   int req_recover_task_chain(const uint8_t type);

   void test_refresh_ring_task(uint32_t task_id);
   void test_update_ring_num(uint32_t ring_num);
   void test_refresh_ring_task__ring_num(uint32_t ring_num);

   void set_task_status(TaskInfo &task_info, const int status);
   uint32_t get_did_ring_task_num();
   uint32_t get_total_ring_task_num();
   void hook_ring_task_complete(uint32_t task_id);
   void notify_remain_reward_task_num();
   bool is_finish_last_talent_task(uint32_t talent_id);

   int req_show_day_task_list();
   bool is_magic_wing_try_task_submit();
   void task_listen_magic_wing_active_to_clear_task();

protected:
    int process_accept(uint32_t task_id, bool is_auto_accept = true);
    int process_abandon(uint32_t task_id);
    int process_submit(uint32_t task_id);
    int process_fast_finish(uint32_t task_id, uint8_t type, uint8_t is_bind_diamond_first);

    int init_main_task(uint32_t task_id, const Task_Mission_Detail_Config &task_json, TaskInfo &taskinfo);
    int init_task_condition(TaskInfo &task_info, const Task_Mission_Detail_Config &task_json);

private:
   //构造并初始化悬赏任务
   int init_reward_task(uint32_t task_id, const Task_Mission_Detail_Config &task_json);

   //显示上一次刷新出来的悬赏任务列表
   int process_old_reward_task_list();

   //刷新悬赏任务列表:  第一次打开悬赏榜、倒计时自动刷新、免费刷新、立即刷新
   int process_refresh_new_reward_task_list();

   //获取悬赏任务
   TaskInfo* find_reward_task(uint32_t task_id);

   //处理免费刷新次数、立即刷新扣货币
   int process_free_refresh_and_money_refresh();

   //处理获取任务列表
   int process_task_get_list(MSG_50110000 &msg, Task_Info &msg_task_info, const TaskInfo &svr_task_info,
		   	   	   	   	   	 TaskIdSet &award_set);

   int process_get_day_task_list(MSG_50110018 &msg, Task_Info &msg_task_info, const TaskInfo &svr_task_info,
		   TaskIdSet &award_set);

   //登录初始化单个任务
   void model_init_one_task(TaskInfo &task_info);

   //生成新的悬赏任务列表(把旧的删除)
   void build_new_reward_task_list_of_server(bool is_process_first_hand_refresh = false);

   //第一次点刷新按钮手动刷新悬赏任务列表
   void first_build_new_reward_task_list_of_server();

   //拷贝服务器中玩家的悬赏任务列表到消息结构中(此函数不发消息，由调用者发消息)
   void copy_reward_task_list_to_msg(uint32_t msg_id, MSG_50110012* msg_50110012, MSG_81110007 *msg_81110007);

   //第一次点刷新按钮手动刷新悬赏任务列表:特殊处理(此函数不发消息，由调用者发消息)
   void first_copy_reward_task_list_to_msg(uint32_t msg_id, MSG_50110012* msg_50110012, MSG_81110007 *msg_81110007);

   //是否存在已领取未完成的悬赏任务
   bool is_exist_accepted_reward_task();

   //概率获取悬赏任务品质
   uint8_t get_reward_task_quality_by_chance();

   //判断日常组号是否存在
   bool is_day_group_exist(const uint32_t group_id);

   void correct_task_cfg_execute_type();
   void correct_task_status_and_execute_progress();
   bool is_task_execute_type_match(const int cfg_execute_type, const int db_cond_type);
   void correct_ring_task_error_cfg();

   int ring_task_construct(const Task_Mission_Detail_Config &cfg);
   bool is_exist_open_ring_task();
   void refresh_ring_task();
   uint8_t get_ring_task_type_by_chance();
   int consume_money_in_tasker(uint8_t money_type, uint32_t money_num, uint32_t lose_money_type);
   void process_scene_npc(const TaskInfo &task_info);
   uint8_t get_ring_task_type_by_ring_num(const uint32_t ring_num);
   uint8_t get_must_not_ring_task_type_by_ring_num(const uint32_t ring_num);

private:
	int process_listen_kill_monster(TaskInfo *task_info, const int monster_id, const int monster_num);
	int process_listen_fb(TaskInfo *task_info, const int sceneid);
	int process_listen_arrive(TaskInfo *task_info, const int sceneid, const int target_x, const int target_y);
	int process_listen_lvl_up(TaskInfo *task_info, const int target_level);
	int process_listen_collect_item(TaskInfo *task_info, const int item_id, const int item_num, const int bind = -1);
	int process_listen_kill_monster_group(TaskInfo *task_info, const int sceneid, const int monster_point);
	int process_listen_package_item(TaskInfo *task_info, const int item_id, const int item_num, const int bind = -1);
	int process_listen_fb_type(TaskInfo *task_info, const int &type);
	int process_listen_win_player(TaskInfo *task_info, const role_id_t role_id);
	int process_listen_join_gang(TaskInfo *task_info);
	int process_listen_gang_level(TaskInfo *task_info, int gang_level);
	int process_listen_player_all_equip_num(TaskInfo *task_info, int equip_num);
	int process_listen_body_equip_num(TaskInfo *task_info, int position, int equip_num);
	int process_listen_body_equip_level(TaskInfo *task_info, int position,int equip_level);
	int process_listen_equip_level(TaskInfo *task_info, uint32_t equip_id, int equip_level);
	int process_listen_stone_level(TaskInfo *task_info, int stone_level);
	int process_listen_buy_goods(TaskInfo *task_info, uint32_t item_id, int item_type, int32_t item_count);
	int process_listen_get_hero(TaskInfo *task_info, int32_t hero_id);
	int process_listen_fight(TaskInfo *task_info, const int scene_id, const int monster_group_id);
	int process_listen_commit_consume_item(TaskInfo *task_info, uint32_t item_id, uint32_t item_num);
	int process_listen_deliver_letter(TaskInfo *task_info, uint32_t item_id, uint32_t item_num);
	int process_listen_help_other_complete_team_scene(TaskInfo *task_info, uint32_t num);
	int process_listen_sell_item(TaskInfo *task_info, uint32_t num);
	//    int process_listen_use_skill(TaskInfo *task_info, uint32_t skill_id);
	//    int process_listen_append_skill(TaskInfo *task_info);
	int process_listen_arena(TaskInfo *task_info);
	int process_listen_chat_with_friend(TaskInfo *task_info, role_id_t friend_id);
	int process_listen_send_mail(TaskInfo *task_info);
	int process_listen_team_scene_or_pvp(TaskInfo *task_info);
	int process_listen_browse_other_player_info(TaskInfo *task_info, role_id_t role_id);
	int process_listen_npc_talk(TaskInfo *task_info, uint32_t npc_id);
	int process_listen_arena_level(TaskInfo *task_info, uint32_t group_id, uint32_t level);
	int process_listen_equip_crimson_level(TaskInfo *task_info, uint32_t position, uint32_t level);
	int process_listen_arena_all(TaskInfo *task_info);
	int process_listen_create_dragon_vale_building(TaskInfo *task_info, uint8_t building_type);
	int process_listen_gain_dragon_vale_builiding_production(TaskInfo *task_info, uint8_t building_type);
	int process_listen_wake_up_hero_to_quality_level(TaskInfo *task_info, uint32_t hero_id, uint32_t quality);
	int process_listen_keep_hero_to_level(TaskInfo *task_info, uint32_t hero_id, uint32_t hero_level);
	int process_listen_compose_equip(TaskInfo *task_info, uint32_t equip_id);
	int process_listen_hero_dream_scene(TaskInfo *task_info, uint32_t id);
	int process_listen_ganger_skill_level(TaskInfo *task_info, uint32_t id);
	int process_listen_war(TaskInfo *task_info);
	int process_listen_war_all(TaskInfo *task_info);
	int process_listen_kill_enemy_in_war(TaskInfo *task_info, uint32_t num);
	int process_listen_single_arena_all(TaskInfo *task_info);
	int process_listen_single_arena_win(TaskInfo *task_info);
	int process_listen_team_arena_all(TaskInfo *task_info);
	int process_listen_team_arena_win(TaskInfo *task_info);
	int process_listen_ganger_bless_buf(TaskInfo *task_info);
	int process_listen_rob_other_dragon_vale_all(TaskInfo *task_info);
	int process_listen_dragon_vale_step_foot_all(TaskInfo *task_info);
	int process_listen_activeness_of_today(TaskInfo *task_info, uint32_t num);
	int process_listen_dragon_vale_building_upgrade(TaskInfo *task_info, uint8_t type, uint16_t level);
	int process_listen_limit_time_kill_boss(TaskInfo *task_info, uint32_t monster_id, uint32_t monster_num);
	int process_listen_dragon_keep_all(TaskInfo *task_info);
	int process_listen_dragon_keep_success(TaskInfo *task_info);
	int process_listen_donate_to_gang(TaskInfo *task_info);
	int process_listen_present_buf_to_other(TaskInfo *task_info);
	int process_listen_finish_reward_task_num(TaskInfo *task_info);
	int process_listen_keep_dragon_to_level(TaskInfo *task_info, uint8_t keep_type, uint32_t keep_level);
	int process_listen_knight_trial(TaskInfo *task_info, uint8_t cur_points);
	int process_listen_suit(TaskInfo *task_info, int id, int curr_cnt);
	int process_listen_role_pull_on_equip(TaskInfo *task_info, uint32_t equip_id);
	int process_listen_hero_pull_on_equip(TaskInfo *task_info, uint32_t equip_id);
	int process_listen_activate_wing(TaskInfo *task_info);
	int process_listen_get_quality_rune(TaskInfo *task_info, uint32_t quality, uint32_t num);
	int process_listen_pass_kngiht_trials(TaskInfo *task_info);
	int process_listen_have_hero(TaskInfo *task_info, uint32_t hero_num);
	int process_listen_have_hero_quality(TaskInfo *task_info, uint32_t quality, uint32_t hero_num);
	int process_listen_first_pass_scene(TaskInfo *task_info, uint32_t scene_id);
	int process_listen_dragon_born(TaskInfo *task_info);
	int process_listen_horse_star_level(TaskInfo *task_info, uint32_t horse_id, uint32_t star_level);
	int process_listen_have_item_but_no_erase(TaskInfo *task_info, uint32_t item_id, uint32_t item_num);
	int process_listen_have_equip_but_no_erase(TaskInfo *task_info, uint32_t item_id, uint32_t item_num);
	int process_listen_same_gang_challenge_all(TaskInfo *task_info);
	int process_listen_main_scene_npc_battle(TaskInfo *task_info, uint32_t monster_id, uint32_t num);
	int process_listen_equip_card_all(TaskInfo *task_info, int num);
	int process_listen_equip_card_by_position(TaskInfo *task_info, const uint32_t position, int num);
	int process_listen_card_upgrade_to_level_all(TaskInfo *task_info, const uint32_t level);
	int process_listen_card_upgrade_to_level_by_position(TaskInfo *task_info, const uint32_t position, const uint32_t level);
	int process_listen_lottery_draw(TaskInfo *task_info, const uint32_t times);
	int process_listen_equip_jewelry(TaskInfo *task_info, const uint32_t quality, const uint32_t num);
	int process_listen_invite_friend_step_foot(TaskInfo *task_info);
	int process_listen_kill_group_num(TaskInfo *task_info, const uint32_t group_id);
	int process_listen_did_reward_task_num(TaskInfo *task_info, const uint32_t num);
	int process_listen_alchemy_num(TaskInfo *task_info);
	int process_listen_did_reward_task_num_today(TaskInfo *task_info, const uint32_t num);
	int process_listen_remote_scene_any_pass(TaskInfo *task_info);
	int process_listen_knight_trial_any_pass(TaskInfo *task_info);
	int process_listen_compose_gemstone_level(TaskInfo *task_info, uint32_t stone_level, uint32_t num);
	int process_listen_inlay_any_gemstone(TaskInfo *task_info, int num);
	int process_listen_inlay_position_gemstone(TaskInfo *task_info, const uint32_t position, const uint32_t num);
	int process_listen_world_boss(TaskInfo *task_info);
	int process_listen_world_boss_immediate_revive(TaskInfo *task_info);
	int process_listen_enchant_any_equip(TaskInfo *task_info);
	int process_listen_inlay_gemsteone_id(TaskInfo *task_info, uint32_t stone_id, uint32_t num);
	int process_listen_gang_well_recover_spirit(TaskInfo *task_info);
	int process_listen_equip_purple_ring(TaskInfo *task_info, uint32_t quality, uint32_t num);
	int process_listen_inlay_level_gemsteone(TaskInfo *task_info, uint32_t gemstone_level, uint32_t gemstone_num);
	int process_listen_upgrade_card(TaskInfo *task_info);
	int process_listen_role_equip_4(TaskInfo *task_info, const uint32_t quality, const uint32_t num_act);
	int process_listen_hero_equip_4(TaskInfo *task_info, const uint32_t quality, const uint32_t num_act);
	int process_listen_dayly_single_arena_all(TaskInfo *task_info);

protected:
    Tasker_Detail *tasker_detail_;				  // 所有任务

    TaskSet task_accepted_monster_; 			  // 监听杀怪的任务集
    TaskSet task_accepted_fb_;					  // 监听完成副本的任务集
    TaskSet task_accepted_arrive_;				  // 监听到达坐标的任务集
    TaskSet task_accepted_lvl_;     			  // 监听升级的任务集
    TaskSet task_accepted_collect_; 			  // 监听采集任务物品的任务集
    TaskSet task_accepted_monster_group_; 		  // 监听杀一组怪的任务集
    TaskSet task_accepted_package_; 			  // 监听收集打怪掉落物品的任务集
    TaskSet task_accepted_fb_type_;  			  // 监听参加活动的任务集
    TaskSet task_accepted_win_player_; 			  // 监听击杀玩家的任务集
    TaskSet task_accepted_join_gang_;             //监听加入公会的任务集
    TaskSet task_accepted_ganger_level_;            //监听公会等级的任务集
    TaskSet task_accepted_player_all_equip_;      //监听玩家强化N次的任务集(没有装备或部位的限制)
    TaskSet task_accepted_body_equip_;            //监听强化指定部位N次的任务集(有部位的限制)
    TaskSet task_accepted_body_equip_level_;      //监听强化指定部位的装备到N级的任务集(没有装备限制)
    TaskSet task_accepted_equip_level_;           //监听强化指定装备到N级的任务集(有装备限制)
    TaskSet task_accepted_stone_level_;           //监听宝石提升到N级的任务集
    TaskSet task_accepted_buy_goods_;             //监听在商城购买某件物品的任务集
    TaskSet task_accepted_get_hero_;              //监听获得某个英雄的任务集
    TaskSet task_accepted_fight_;                 //监听挑战NPC的任务集
    TaskSet task_accepted_commit_consume_item_;   //监听任务提交时消耗XX道具（ID）XX个（数量）的任务集
    TaskSet task_accepted_deliver_letter_;        //监听送信任务的任务集
    TaskSet task_accepted_help_other_complete_team_scene_;//监听协助其他玩家完成多人副本N次的任务集(对副本无限制，策划只配置次数)
    TaskSet task_accepted_sell_item_;                     //监听出售X件道具的任务集（道具无限制，只检测出售个数）
//    TaskSet task_accepted_use_skill_;                     //使用某个技能XX次（每个职业技能不一样，支持多种技能ID配置）
//    TaskSet task_accepted_append_skill_;                  //释放XX次追加技
    TaskSet task_accepted_limit_time_;                      //监听限时任务的任务集
    TaskSet task_accepted_arena_;                            //竞技场战斗胜利
    TaskSet task_accepted_chat_with_friend_;                //与X个好友聊天(每个好友只要1次聊天即可)
    TaskSet task_accepted_send_mail_;                       //发送XXX封邮件(只限制封数，不限制给谁发)
    TaskSet task_accepted_team_scene_or_pvp_;               //XXX次多人战斗胜利. 即:多人副本或者多人组队竞技胜利  注明： (1)多人副本只要胜利即可, 有收益、无收益均可. (2)多人组队竞技，只要胜利即可)
    TaskSet task_accepted_browse_other_player_info_;        //查看XXX个玩家的信息
    TaskSet task_accepted_npc_talk_;                        //NPC对话任务
    TaskSet task_accepted_arena_level_;                          //指定组id的竞技场到XX段位(包括以上)
    TaskSet task_accepted_equip_crimson_level_;                  //装备符文充能到XX级
    TaskSet task_accepted_arena_all_;					         //参与XX次竞技场(不论胜负，单人竞技、组队竞技都可以)
    TaskSet task_accepted_create_dragon_vale_building_;          //指定类型的龙谷建筑建造XX个
    TaskSet task_accepted_gain_dragon_vale_builiding_production_;//收获XX次指定类型的龙谷建筑资源
    TaskSet task_accepted_wake_up_hero_to_quality_level_;        //将指定英雄觉醒至指定的觉醒等级(如：蓝色、绿色)
    TaskSet task_accepted_keep_hero_to_level_;					 //将指定英雄培养至指定的等级
    TaskSet task_accepted_compose_equip_;                        //合成指定装备
    TaskSet task_accepted_hero_dream_scene_;					 //参与XX次英雄梦境
    TaskSet task_accepted_ganger_skill_level_;				     //指定公会技能提升到XX级(玩家学习的公会技能)
    TaskSet task_accepted_war_;                                  //参与并打赢战场
    TaskSet task_accepted_war_all_;           			//参与战场X次(打赢、打输都可以)
    TaskSet task_accepted_kill_enemy_in_war_; 			//在战场中击杀X个敌人
    TaskSet task_accepted_single_arena_all_;  			//单人竞技参与X次(打赢、打输都可以)
    TaskSet task_accepted_single_arena_win_;  			//单人竞技参与并打赢X次
    TaskSet task_accepted_team_arena_all_;    			//组队竞技参与X次(打赢、打输都可以)
    TaskSet task_accepted_team_arena_win_;    			//组队竞技参与并打赢X次
    TaskSet task_accepted_ganger_bless_buf_;  			//公会抽奖X次
    TaskSet task_accepted_rob_other_dragon_vale_all_;	//龙谷掠夺X次(只要参与掠夺别人的龙谷战斗即可，不管输赢，不管是否掠夺资源)
    TaskSet task_accepted_dragon_vale_step_foot_all_;	//龙谷踩脚印X次(只要踩脚印即可，不管是否提升星级)
    TaskSet task_accepted_activeness_of_today_;			//当天活跃度达到X
    TaskSet task_accepted_dragon_vale_building_upgrade_;//龙谷建筑升级到X级
	TaskSet task_accepted_limit_time_kill_boss_;	    //限时击杀boss
    TaskSet task_accepted_dragon_keep_all_;//龙培养X次(只要花钱或者道具培养了，不管是否培养成功)
    TaskSet task_accepted_dragon_keep_success_;//龙培养X次(龙培养成功，培养到下一阶才算)
    TaskSet task_accepted_donate_to_gang_;// 公会捐献x次
    TaskSet task_accepted_present_buf_to_other_;//公会成员赠送buf给他人 x次
    TaskSet task_accepted_finish_reward_task_num_;
    TaskSet task_accepted_keep_dragon_to_level_;
    TaskSet task_accepted_knight_trial_;
    TaskSet task_accepted_suit_;
    TaskSet task_accepted_role_pull_on_equip_;
    TaskSet task_accepted_hero_pull_on_equip_;
    TaskSet task_accepted_activate_wing_;
    TaskSet task_accepted_get_quality_rune_;
    TaskSet task_accepted_pass_kngiht_trials_;
    TaskSet task_accepted_have_hero_;
    TaskSet task_accepted_have_hero_quality_;
    TaskSet task_accepted_first_pass_scene_;
    TaskSet task_accepted_dragon_born_;
    TaskSet task_accepted_horse_star_level_;
    TaskSet task_accepted_have_item_but_no_erase_;
    TaskSet task_accepted_have_equip_but_no_erase_;
    TaskSet task_accepted_same_gang_challenge_all_;
    TaskSet task_accepted_main_scene_npc_battle_;
    TaskSet task_accepted_equip_card_all_;
    TaskSet task_accepted_equip_card_by_position_;
    TaskSet task_accepted_card_upgrade_to_level_all_;
    TaskSet task_accepted_card_upgrade_to_level_by_position_;
    TaskSet task_accepted_lottery_draw_;
    TaskSet task_accepted_equip_jewelry_;
    TaskSet task_accepted_hero_equip_n_;
    TaskSet task_accepted_role_equip_n_;
    TaskSet task_accepted_invite_friend_step_foot_;
    TaskSet task_accepted_kill_group_num_;
    TaskSet task_accepted_did_reward_task_num_;
    TaskSet task_accepted_alchemy_num_;
    TaskSet task_accepted_did_reward_task_num_today_;
    TaskSet task_accepted_remote_scene_any_pass_;
	TaskSet task_accepted_knight_trial_any_pass_;
    TaskSet task_accepted_compose_gemstone_level_;
    TaskSet task_acceped_inlay_position_gemstone_;
    TaskSet task_accepted_inlay_any_gemstone_;
    TaskSet task_accepted_world_boss_;
    TaskSet task_accepted_world_boss_immediate_revive_;
    TaskSet task_accepted_enchant_any_equip_;
    TaskSet task_accepted_inlay_id_gemstone_;
    TaskSet task_accepted_gang_well_recover_spirit_;
    TaskSet task_accepted_equip_purple_ring_;
    TaskSet task_accepted_inlay_level_gemstone_;
    TaskSet task_accepted_upgrade_card_;

    Item_TaskIdSet_Map task_listen_item_TaskIdSet_map_;     //任务提交需要扣除物品的 Item_TaskIdSet_Map

    TaskIdSet scene_task_id_set_;				      //采集物品的任务的id集合(已接任务)       注： 用于同步给场景服
    Npc_Item_Chance_Set_Map scene_npc_ItemIdSet_map_;	      //收集打怪掉落物品的Npc_ItemIdSet_Map  注：用于同步给场景服

//    ItemTaskMap task_listen_source_to_item_;	// 任务需要收集的物品集合的source
};

const Tasker_Detail& Tasker::tasker_detail(void) const{
	return *tasker_detail_;
}

Tasker_Detail& Tasker::tasker_detail_no_const() {
	return *tasker_detail_;
}

#endif /* TASKER_H_ */
