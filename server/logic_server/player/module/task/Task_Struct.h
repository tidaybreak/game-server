/*
 * Task_Struct.h
 *
 *  Created on: 2014年3月11日
 *      Author: root
 */

#ifndef TASK_STRUCT_H_
#define TASK_STRUCT_H_

#include "Server_Struct.h"
#include "Task_Def.h"

typedef boost::unordered_set<role_id_t> EXT_SET;

struct TaskCondition
{
    enum TASK_COND_TYPE
    {
        TASK_CT_NONE = 0,
        TASK_CT_KILL_MONSTER = 1,   		// 杀怪
        TASK_CT_FB = 2,         			// 完成副本任务
        TASK_CT_NPC_TALK = 3,   		// NPC对话
        TASK_CT_ARRIVE = 4,         		// 到达目的地
        TASK_CT_LVL = 5,            		// 等级
        TASK_CT_COLLECT = 6,				// 采集
        TASK_CT_KILL_MONSTER_GROUP = 7,   	// 杀指定场景指定组怪
        TASK_CT_PACKAGE = 8,   				// 收集
        TASK_CT_FB_TYPE = 9,       			// 完成某类型副本任务
        TASK_CT_WIN_PLAYER = 10,    		// 击败玩家
        TASK_CT_JOIN_GANG = 11,          	//加入公会
        TASK_CT_GANGER_LEVEL = 12,         	//公会等级到达N级
        TASK_CT_PLAYER_ALL_EQUIP = 13,  	//玩家强化N次(没有装备、部位的限制)
        TASK_CT_BODY_EQUIP = 14,         	//强化指定部位N次(没有装备限制,只要该部位强化N次即可)
        TASK_CT_BODY_EQUIP_LEVEL = 15,   	//强化指定部位的装备到N级(没有装备限制,只要该部位有装备到N级即可)
        TASK_CT_EQUIP_LEVEL = 16,       	//强化指定装备到N级(有装备限制)
        TASK_CT_STONE_LEVEL = 17,        	//宝石提升到N级
        TASK_CT_BUY_GOODS = 18,          	//在商城购买某件物品  param1:物品id      param2:道具二级分类类型 (2者取其一)
        TASK_CT_GET_HERO = 19,           	//获得指定英雄
        TASK_CT_FIGHT = 20,              	//挑战NPC(与NPC对话进入战斗)
        TASK_CT_COMMIT_CONSUME_ITEM = 21,	//任务提交时消耗XX道具（ID）XX个（数量）
        TASK_CT_DELIVER_LETTER = 22,        //送信任务-领取任务时获得特定道具（ID），提交时需要扣除该道具（ID）
        TASK_CT_HELP_OTHER_COMPLETE_TEAM_SCENE = 23, //协助其他玩家完成多人副本N次(对副本无限制，策划只配置次数)
        TASK_CT_SELL_ITEM = 24,                      //出售X件道具（道具无限制，只检测出售个数）
        TASK_CT_ARENA = 27,                     //竞技场战斗胜利
        TASK_CT_CHAT_WITH_FRIEND = 28,         //与X个好友聊天(每个好友只要1次聊天即可)
        TASK_CT_SEND_MAIL = 29,                //发送XXX封邮件(只限制封数，不限制给谁发)
        TASK_CT_TEAM_SCENE_OR_PVP = 30,        //XXX次多人战斗胜利. 即:多人副本或者多人组队竞技胜利  注明： (1)多人副本只要胜利即可, 有收益、无收益均可. (2)多人组队竞技，只要胜利即可)
        TASK_CT_BROWSE_OTHER_PLAYER_INFO = 31, //查看XXX个玩家的信息
        TASK_CT_ARENA_LEVEL = 32,							//指定组id的竞技场到XX段位(包括以上)
        TASK_CT_EQUIP_CRIMSON_LEVEL = 33,					//装备符文充能到XX级
        TASK_CT_ARENA_ALL = 34,								//参与XX次竞技场(不论胜负，单人竞技、组队竞技都可以)
        TASK_CT_CREATE_DRAGON_VALE_BUILDING = 35,			//指定类型的龙谷建筑建造XX个
        TASK_CT_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION = 36,	//收获XX次指定类型的龙谷建筑资源
        TASK_CT_WAKE_UP_HERO_TO_QUALITY_LEVEL = 37,				//将指定英雄觉醒至指定的觉醒品质(如：蓝色、绿色)
        TASK_CT_KEEP_HERO_TO_LEVEL = 38,					//将指定英雄培养至指定的等级
        TASK_CT_COMPOSE_EQUIP = 39, 						//合成指定装备
        TASK_CT_HERO_DREAM_SCENE = 40,						//参与XX次英雄梦境
        TASK_CT_GANGER_SKILL_LEVEL = 41,							//指定公会技能提升到XX级(玩家学习的公会技能)
        TASK_CT_WAR = 42,                                   //参与并打赢战场
        TASK_CT_WAR_ALL = 43,         //参与战场X次(打赢、打输都可以)
        TASK_CT_KILL_ENEMY_IN_WAR = 44,//在战场中击杀X个敌人
        TASK_CT_SINGLE_ARENA_ALL = 45,//单人竞技参与X次(打赢、打输都可以)
        TASK_CT_SINGLE_ARENA_WIN = 46,//单人竞技参与并打赢X次
        TASK_CT_TEAM_ARENA_ALL = 47,  //组队竞技参与X次(打赢、打输都可以)
        TASK_CT_TEAM_ARENA_WIN = 48,  //组队竞技参与并打赢X次
        TASK_CT_GANGER_BLESS_BUF = 49,//公会抽奖X次
        TASK_CT_ROB_OTHER_DRAGON_VALE_ALL = 50,//龙谷掠夺X次(只要参与掠夺别人的龙谷战斗即可，不管输赢，不管是否掠夺资源)
        TASK_CT_DRAGON_VALE_STEP_FOOT_ALL = 51,//龙谷踩脚印X次(只要踩脚印即可，不管是否提升星级)
        TASK_CT_ACTIVENESS_OF_TODAY = 52,//当天活跃度达到X
        TASK_CT_DRAGON_VALE_BUILDING_UPGRADE = 53,//龙谷建筑升级到X级
        TASK_CT_LIMIT_TIME_KILL_BOSS = 54,//限时击杀boss
        TASK_CT_DRAGON_KEEP_ALL = 55,//龙培养X次(只要花钱或者道具培养了，不管是否培养成功)
        TASK_CT_DRAGON_KEEP_SUCCESS = 56,//龙培养X次(龙培养成功，培养到下一阶才算)
        TASK_CT_DONATE_TO_GANG = 57,//公会捐献x次
        TASK_CT_PRESENT_BUF_TO_OTHER = 58,//公会成员赠送buf给他人x次
        TASK_CT_FINISH_REWARD_TASK_NUM = 59,//完成悬赏任务数量
        TASK_CT_KEEP_DRAGON_TO_LEVEL = 60,  //将龙培养到指定阶数
        TASK_CT_KNIGHT_TRIAL = 61,          //骑士试炼通关xx关
        TASK_CT_SUIT = 62,                  //某个套装的对应的部件全部穿在身上(玩家职业对应的可以穿的全部)
        TASK_CT_ROLE_PULL_ON_EQUIP = 63,//主角穿指定的装备
        TASK_CT_HERO_PULL_ON_EQUIP = 64,//英雄穿指定的装备
        TASK_CT_ACTIVATE_WING = 65,     //激活翅膀
        TASK_CT_GET_QUALITY_RUNE = 66,  //获得x品质的符石xx个
        TASK_CT_PASS_KNGIHT_TRIALS = 67, //通关骑士试炼x次(从第一关打到最后一关，所有关都打完才算通关一次)
        TASK_CT_HAVE_HERO = 68,          //拥有多少个英雄
        TASK_CT_HAVE_HERO_QUALITY = 69,  //拥有多少个x品质的英雄
        TASK_CT_FIRST_PASS_SCENE = 70,   //首通单人副本、精英副本(接任务前通关过、接任务后通关都算) (不限制是第几次通关)
        TASK_CT_DRAGON_BORN = 71,        //龙谷龙蛋孵化
        TASK_CT_HORSE_STAR_LEVEL = 72,   //指定id的激活坐骑升到xx星级
        TASK_CT_HAVE_ITEM_BUT_NO_ERASE = 73, //拥有某个道具(不包括符石)，仅限于人物背包段中，不扣道具
        TASK_CT_HAVE_EQUIP_BUT_NO_ERASE = 74,//拥有某个装备(不包括符石)，主角穿戴的装备、英雄穿戴的装备、人物背包段中的未穿戴的装备，不扣除
        TASK_CT_SAME_GANG_CHALLENGE_ALL = 75,//前往擂台与同公会玩家决斗X次(不论输赢，都算)
        TASK_CT_MAIN_SCENE_NPC_BATTLE = 76,  //主城npc战斗任务
        TASK_CT_EQUIP_CARD_ALL = 77,          //向任意装备镶嵌兵魂
        TASK_CT_EQUIP_CARD_BY_POSITION = 78,  //向指定部位镶嵌兵魂
        TASK_CT_CARD_UPGRADE_TO_LEVEL_ALL = 79,   //将任意兵魂升级到XX级
        TASK_CT_CARD_UPGRADE_TO_LEVEL_BY_POSITION = 80, //将某一个部位兵魂升级到XX级
        TASK_CT_LOTTERY_DRAW = 81, //博彩N次
        TASK_CT_EQUIP_JEWELRY = 82, //穿戴4件紫色或紫色以上（金色）的首饰
        TASK_CT_INVITE_FRIEND_STEP_FOOT = 83,
        TASK_CT_KILL_GROUP_NUM = 84,
        TASK_CT_DID_REWARD_TASK_NUM = 85,
        TASK_CT_ALCHEMY_NUM = 86,
        TASK_CT_DID_REWARD_TASK_NUM_TODAY = 87,
        TASK_CT_REMOTE_SCENE_ANY_PASS = 88,
        TASK_CT_KNIGHT_TRIAL_ANY_PASS = 89,
        TASK_CT_COMPOSE_GEMSTONE_LEVEL = 90,
        TASK_CT_INLAY_POSITION_GEMSTONE = 91,
        TASK_CT_INLAY_ANY_GEMSTONE = 92,
        TASK_CT_WORLD_BOSS = 93,
        TASK_CT_WORLD_BOSS_IMMEDIATE_REVIVE = 94,
        TASK_CT_ENCHANT_ANY_EQUIP = 95,
        TASK_CT_INLAY_ID_GEMSTONE = 96,
        TASK_CT_GANG_WELL_RECOVER_SPIRIT = 97,
        TASK_CT_EQUIP_PURPLE_RING = 98,
        TASK_CT_INLAY_LEVEL_GEMSTONE = 99,
        TASK_CT_UPGRADE_CARD = 100,
        TASK_CT_ROLE_EQUIP_4 = 101,
        TASK_CT_HERO_EQUIP_4 = 102,
        TASK_CT_DAYLY_SINGLE_ARENA_ALL = 103,//当天内单人竞技参与X次(打赢、打输都可以，接任务前的次数也算)

        //        TASK_CT_USE_SKILL = 99, 					 //使用某个技能XX次（每个职业技能不一样，支持多种技能ID配置）(只限制次数，只要是数组中的任意一个技能即可)
        //        TASK_CT_APPEND_SKILL = 100,                   //释放XX次追加技
        TASK_CT_END
    };

    int __type;     // 类型: TASK_COND_TYPE
    int __current_value;
    EXT_SET ext_set;  //拓展字段

    void reset(void);
/*    bool is_no_condtion(void);
    bool is_kill_monster(void);
    bool is_fb(void);
    bool is_npc_dialogue(void);
    bool is_arrive(void);
    bool is_level(void);
    bool is_collect_item(void);
    bool is_kill_monster_group(void);
    bool is_package_item(void);
    bool is_fb_type(void);
    bool is_win_player(void);
    bool is_join_gang();
    bool is_gang_level();
    bool is_player_all_equip();
    bool is_body_equip();
    bool is_body_equip_level();
    bool is_equip_level();
    bool is_stone_level();
    bool is_buy_goods();
    bool is_get_hero();
    bool is_fight();*/
};

class Task_Implement;
struct TaskInfo
{
    enum TASK_STATUS
    {
        TASK_STATUS_NONE = 0,
        TASK_STATUS_VISIBLE = 1,    // 任务可见(不可接)
        TASK_STATUS_ACCEPTABLE = 2, // 任务可接
        TASK_STATUS_ACCEPTED = 3,   // 任务已接
        TASK_STATUS_FINISH = 4,     // 任务已完成
        TASK_STATUS_SUBMIT = 5,     // 任务已提交
        TASK_STATUS_FAIL = 6,       // 限时任务失败

        TASK_STATUS_END
    };
    /*任务类型*/
    enum TASK_GAME_TYPE
    {
        TASK_GTYPE_NONE 		= 0,
        TASK_GTYPE_MAINLINE 	= 101,	// 主线任务
        TASK_GTYPE_BRANCH 		= 102,	// 支线任务
        TASK_GTYPE_DAY 		    = 103,	// 日常任务
        TASK_GTYPE_WEEK         = 104,	// 周常任务
        TASK_GTYPE_SOCIATY 		= 105,	// 公会任务
        TASK_GTYPE_ACTIVITY 	= 106,	// 活动任务
        TASK_GTYPE_ELITE		= 107,	// 精英任务
        TASK_GTYPE_ACTIVITY2	= 108,	// 活动
		TASK_GTYPE_HERO			= 109,  // 英雄任务
        TASK_GTYPE_REWARD_TASK  = 110,  // 悬赏任务
        TASK_GTYPE_RING_TASK = 120,     //环任务
        TASK_GTYPE_OPEN_RING_TASK = 121,//开启环任务类型(只能有一个任务配置)
        TASK_GTYPE_CLOSE_RING_TASK = 122,//结束环任务(只能有一个任务配置)
        TASK_GTYPE_TALENT = 130, //天赋任务

        TASK_GTYPE_END
    };

	enum Task_Index {
		TASK_START			= 10000000,
		TASK_END			= 99999999,
		MAIN_START 			= 10100000,
		MAIN_END 			= 10200000,
		BRANCH_START 		= 10200001,//支线
		BRANCH_END			= 10300000,
		ROUTINE_START		= 10300001,//日常
		ROUTINE_END			= 10400000,
		ROUTINE_WEEK_START	= 10400001,//周常
		ROUTINE_WEEK_END	= 10500000,
		SOCIATY_START		= 10500001,//公会
		SOCIATY_END			= 10600000,
		ACTIVITY_START		= 10600001,//活动
		ACTIVITY_END		= 10700000,
		ELITE_START			= 10700001,//精英
		ELITE_END			= 10800000,
		ACTIVITY2_START		= 10800001,//活动
		ACTIVITY2_END		= 10900000,
		HERO_START			= 10900001,//英雄任务
		HERO_END			= 11000000
	};
    typedef TaskCondition::TASK_COND_TYPE TASK_LOGIC_TYPE;

    uint32_t __task_id;
    int __game_type;            // 类型: TASK_GAME_TYPE
    Time_Value __accept_tick;   // 接受时间戳
    Time_Value __refresh_tick;  // 刷新时间戳(日常等定期刷新的任务)

    typedef std::vector<TaskCondition> TaskConditionList;
    TaskConditionList __condition_list;     // 任务的条件集

    int __prev_task;    // 前置任务,若为0则从配置文件中读取
    int __post_task;    // 后置任务,若为0则从配置文件中读取
    int __post_task2;
    int __post_task3;

    bool __recruid_hero; // 配置中取得，不用每次完成任务，都去查询

    typedef std::bitset<TASK_STATUS_END> TaskStatusFlag;
    TaskStatusFlag __status;    // 任务状态集合
    typedef std::bitset<TaskCondition::TASK_CT_END> TaskLogicType;
    TaskLogicType __logic_type;

    uint8_t __item_money_flag;   //0：默认值。 1：道具不足，扣货币(只用于快速完成任务)
    bool __is_req_fast_finish;   //是否请求快速完成任务
    uint32_t __hero_id;            //策划配置通过接任务添加的英雄的id
    bool __is_pack_accept;         //是否是在背包中用金色悬赏令领取

    TaskInfo(void);
    void reset(void);
    bool is_visible(void) const;      // 可见
    bool is_acceptable(void) const;   // 可授受
    bool is_accepted(void) const;     // 已接受
    bool is_finish(void) const;       // 已完成未提交
    bool is_submit(void) const;       // 已提交
    bool is_fail() const;             // 失败
    void set_task_status(const int status);
    void reset_task_status(const int status);

    bool is_main_task(void) const;
    bool is_branch_task(void) const;
    bool is_day_task(void) const;
    bool is_week_task(void) const;
    bool is_sociaty_task(void) const;
    bool is_activity_task(void) const;
    bool is_elite_task(void) const;
    bool is_activity2_task(void) const;
    bool is_hero_task(void) const;
    bool is_reward_task() const;
    bool is_ring_task() const;
    bool is_open_ring_task() const;
    bool is_close_ring_task() const;
    bool is_talent_task() const;

    void set_logic_type(const int type);
    void reset_logic_type(const int type);
    bool is_logic_kill_monster(void);
    bool is_logic_fb(void);
    bool is_logic_npc_talk(void);
    bool is_logic_arrive(void);
    bool is_logic_level_up(void);
    bool is_logic_package(void);
    bool is_logic_kill_monster_group(void);
    bool is_logic_collect(void);
    bool is_logic_fb_type(void);
    bool is_logic_win_player(void);
    bool is_logic_join_gang();
    bool is_logic_ganger_level();
    bool is_logic_player_all_equip();
    bool is_logic_body_equip();
    bool is_logic_body_equip_level();
    bool is_logic_equip_level();
    bool is_logic_stone_level();
    bool is_logic_buy_goods();
    bool is_logic_get_hero();
    bool is_logic_fight();
    bool is_logic_commit_consume_item();
    bool is_logic_deliver_letter();
    bool is_logic_help_other_complete_team_scene();
    bool is_logic_sell_item();
//    bool is_logic_use_skill();
//    bool is_logic_append_skill();
    bool is_logic_arena();
    bool is_logic_chat_with_friend();
    bool is_logic_send_mail();
    bool is_logic_team_scene_or_pvp();
    bool is_logic_browse_other_player_info();
    bool is_logic_arena_level();
    bool is_logic_equip_crimson_level();
    bool is_logic_arena_all();
    bool is_logic_create_dragon_vale_building();
    bool is_logic_gain_dragon_vale_builiding_production();
    bool is_logic_wake_up_hero_to_quality_level();
    bool is_logic_keep_hero_to_level();
    bool is_logic_compose_equip();
    bool is_logic_hero_dream_scene();
    bool is_logic_ganger_skill_level();
    bool is_logic_war();
    bool is_logic_war_all();
    bool is_logic_kill_enemy_in_war();
    bool is_logic_single_arena_all();
    bool is_logic_single_arena_win();
    bool is_logic_team_arena_all();
    bool is_logic_team_arena_win();
    bool is_logic_ganger_bless_buf();
    bool is_logic_rob_other_dragon_vale_all();
    bool is_loigc_dragon_vale_step_foot_all();
    bool is_logic_activeness_of_today();
    bool is_logic_dragon_vale_building_upgrade();
    bool is_logic_limit_time_kill_boss();
    bool is_logic_dragon_keep_all();
    bool is_logic_dragon_keep_success();
    bool is_logic_donate_to_gang();
    bool is_logic_present_buf_to_other();
    bool is_logic_finish_reward_task_num();
    bool is_logic_keep_dragon_to_level();
    bool is_logic_knight_trial();
    bool is_logic_suit();
    bool is_logic_role_pull_on_equip();
    bool is_logic_hero_pull_on_equip();
    bool is_logic_activate_wing();
    bool is_logic_get_quality_rune();
    bool is_logic_pass_kngiht_trials();
    bool is_logic_have_hero();
    bool is_logic_have_hero_quality();
    bool is_logic_first_pass_scene();
    bool is_logic_dragon_born();
    bool is_logic_horse_star_level();
    bool is_logic_have_item_but_no_erase();
    bool is_logic_have_equip_but_no_erase();
    bool is_logic_same_gang_challenge_all();
    bool is_logic_main_scene_npc_battle();
    bool is_logic_equip_card_all();
    bool is_logic_equip_card_by_position();
    bool is_logic_card_upgrade_to_level_all();
    bool is_logic_card_upgrade_to_level_by_position();
    bool is_logic_lottery_draw();
    bool is_logic_equip_jewelry();
    bool is_logic_invite_friend_step_foot();
    bool is_logic_kill_group_num();
    bool is_logic_did_reward_task_num();
    bool is_logic_alchemy_num();
    bool is_logic_did_reward_task_num_today();
    bool is_logic_remote_scene_any_pass();
    bool is_logic_knight_trial_any_pass();
    bool is_logic_compose_gemstone_level();
    bool is_logic_inlay_position_gemstone();
    bool is_logic_inlay_any_gemstone();
    bool is_logic_world_boss();
    bool is_logic_world_boss_immediate_revive();
    bool is_logic_enchant_any_equip();
    bool is_logic_inlay_id_gemstone();
    bool is_logic_gang_well_recover_spirit();
    bool is_logic_equip_purple_ring();
    bool is_logic_inlay_level_gemstone();
    bool is_logic_upgrade_card();
    bool is_logic_role_equip_4();
    bool is_logic_hero_equip_4();
    bool is_logic_dayly_single_arena_all();

    int condition_size(void);
    TaskCondition *task_condition(const int index = 0);
//    int task_status(void) const;
};

typedef boost::unordered_map<int, TaskInfo> TaskMap;
typedef boost::unordered_set<int> TaskSet;
typedef boost::unordered_set<int> TaskIdSet;
typedef Hash_Map<int, TaskIdSet, NULL_MUTEX> ItemTaskMap;

//typedef boost::unordered_map<uint32_t, TaskInfo> RewardTaskMap;
typedef std::map<uint32_t, TaskInfo> RewardTaskMap;

typedef boost::unordered_set<uint32_t> RewardTaskIdSet;
//key:悬赏任务品质 value:RewardTaskIdSet
typedef boost::unordered_map<uint8_t, RewardTaskIdSet> Quality_RewardTaskIdSet_Map;

typedef boost::unordered_set<uint32_t> RingTaskIdSet;
//key:环类型
typedef boost::unordered_map<uint8_t, RingTaskIdSet> Ring_Type_RingTaskIdSet_Map;

typedef boost::unordered_set<uint32_t> DayTaskIdSet;

/* 随身怪 */
struct Task_Npc_Type {
	Task_Npc_Type(void) { reset(); }
	void reset(void) {
		npc_type = 0;
		npc_id = 0;
		npc_name.clear();
		funcName.clear();
		direction = 0;
		sceneId.clear();
		open.clear();
		x = 0;
		y = 0;
	}
	int serialize(Block_Buffer &buf) const;
	int deserialize(Block_Buffer &buf);
	int npc_type;
	role_id_t npc_id;
	std::string npc_name;
	std::string funcName;
	int direction;
	std::string sceneId;
	std::string open;
	int32_t x;
	int32_t y;
};
typedef boost::unordered_map<int32_t, Task_Npc_Type> Task_Npc_Type_Map;

//key: task_id   value:hero_id
typedef boost::unordered_map<uint32_t, uint32_t> Task_Icon_Hero_Id_Map;

//key: task_id
typedef boost::unordered_set<uint32_t> Task_Gift_Level_Set;

typedef boost::unordered_set<uint32_t> Scene_Id_Set;

struct Tasker_Detail : public Detail {
	Tasker_Detail();
	virtual ~Tasker_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	inline void save_tick(void) { is_change = true; };

	int bind_task(int task_id, TaskInfo &info);
	int bind_reward_task(int task_id, TaskInfo &info);

	role_id_t role_id;
	TaskMap task_map_;
    TaskIdSet task_submited_once_;  // 只提交一次
    Role_Id_Set npc_id_; // 屏蔽怪
    Task_Npc_Type_Map npc_type_map_; // 随身怪

    bool is_ever_open_reward_task_list;         //是否曾经打开过悬赏榜
    bool is_close_auto_refresh;                 //是否关闭自动刷新
    uint8_t refresh_reward_task_num;            //刷新悬赏任务次数
    bool is_open_countdown;                     //是否开启了倒计时
    Time_Value begin_countdown_time;            //开始倒计时时间
    uint32_t used_free_refresh_count;           //今日已经使用的免费刷新次数
    uint32_t used_accept_count;                 //今日已经领取次数
    uint32_t give_up_num;                       //今日放弃悬赏任务次数
    RewardTaskMap reward_task_map;              //悬赏榜刷新出来的任务列表
    Quality_RewardTaskIdSet_Map quality_RewardTaskIdSet_map; //可以刷新出来的悬赏任务

    Task_Icon_Hero_Id_Map task_icon_hero_id_Map;			//任务触发新英雄图标的map
    Task_Gift_Level_Set task_gift_level_set;            //任务开启礼包的set

    uint32_t ring_num;  //环数
    bool is_diamond_recover;//是否钻石恢复过任务链
    Ring_Type_RingTaskIdSet_Map ring_type_RingTaskIdSet_map;//可以刷新出来的环任务

    role_id_t target_player_id;
    uint32_t task_id;

	Scene_Id_Set scene_id_set;
    uint32_t did_ring_num;
    uint32_t did_reward_task_num;
    Svr_Scene_Npc_Set svr_scene_npc_set;
    uint32_t ring_task_id;
    bool is_exist_cross_day;
    uint32_t did_reward_task_num_today;
    bool is_submit_close_ring_task;
};

#endif /* TASK_STRUCT_H_ */
