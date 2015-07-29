/*
 * Task_Config.h
 *
 *  Created on: 2014年3月26日
 *      Author: root
 */

#ifndef TASK_CONFIG_H_
#define TASK_CONFIG_H_

#include "Base_Struct.h"

// TASK CONFIG ==================================
/*struct Task_Type_Config {
	Task_Type_Config(void) { reset(); }
	void reset(void) {
		main = 0;
		branch = 0;
		routine = 0;
		league = 0;
	}

	int main;
	int branch;
	int routine;
	int league;
};*/
/*struct Task_Cond_Type_Config {
	Task_Cond_Type_Config(void) { reset(); }
	void reset(void) {
		kill = 0;
		fb = 0;
		talk = 0;
		arrive = 0;
		lvl_up = 0;
		arrive = 0;
		package = 0;
		kill_group = 0;
		collect = 0;
		recharge = 0;
		attend = 0;
	}

	int kill;
	int fb;
	int talk;
	int arrive;
	int lvl_up;
	int package;
	int kill_group;
	int collect;
	int recharge;
	int attend;
};*/
/*struct Task_Setting_Config {
	Task_Setting_Config(void) { reset(); }
	void reset(void) {
		task_type.reset();
		cond_type.reset();
		cond_enum.clear();
		daily_reset.clear();
	}

	Task_Type_Config task_type;
	Task_Cond_Type_Config cond_type;
	Int_Vec cond_enum;
	String_Vec daily_reset;
};*/

struct Scene_Monster_Point {
	Scene_Monster_Point() {
		reset();
	}
	void reset() {
		scene_id = 0;
		monster_point = 0;
	}
	uint32_t scene_id;
	uint32_t monster_point;
};

inline std::size_t hash_value(const Scene_Monster_Point &scene_monster_point) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<int32_t>(scene_monster_point.scene_id));
    boost::hash_combine(seed, static_cast<int32_t>(scene_monster_point.monster_point));
    return seed;
}

inline bool operator==(const Scene_Monster_Point &a, const Scene_Monster_Point &b) {
	return a.scene_id == b.scene_id && a.monster_point == b.monster_point;
}

typedef boost::unordered_set<Scene_Monster_Point> Scene_Monster_Point_Set;

struct Task_Mission_Award_Detail_Cfg {
	int32_t type;/**/
	int32_t val1;/**/
	int32_t val2;/**/
	int32_t val3;/**/
	int32_t val4;/**/
	int32_t val5;/**/
	int8_t job;
	Task_Mission_Award_Detail_Cfg(void) { reset(); };

	void reset(void) {
		type = 0;
		val1 = 0;
		val2 = 0;
		val3 = 0;
		val4 = 0;
		val5 = 0;
		job = 0;
	}
};
typedef std::vector<Task_Mission_Award_Detail_Cfg> Task_Mission_Award_List_Cfg;

struct Task_Mission_Action_Cfg {
	Task_Mission_Action_Cfg(void) { reset(); }
	void reset(void) {
		polt_id = 0;
		action_id = 0;
	}

	int polt_id;
	int action_id;
};
struct Task_Mission_Accept_Cfg {
	Task_Mission_Accept_Cfg(void) { reset(); }
	void reset(void) {
		is_empty = 0;
		talk = 0;
		camp = 0;
		career = 0;
		vip_level = 0;
		type = 0;
		level = 0;
		level_max = 0;
		team = 0;
		guild = 0;
		accept_times = 0;
		accept_times_all = 0;
		auto_accept = 0;
		receive.reset();
		paytype.reset();
		exp = 0;
		pet = 0;
		buf_op = 0;
		buff = 0;
		buf_ms = 0;
		fly = 0;
		polt_id = 0;
		action.reset();
		skill_id = 0;
		full_prop_num = 0;
		val1 = 0;
		val2 = 0;
		val3 = 0;
		val4 = 0;
//		pay = 0;
	}

	bool is_empty;//no config
	int talk; //接任务NPC
	int camp; //0 表示任何阵营的人都可以接，其他数字表示只有该阵营的人可接
	int career; // 0 表示任何职业的人都可以接，其他数字表示只有该职业的人可接
	int vip_level; // 只有VIP等级大于等于该数时可接
	int type; // 0:所有人可接，1：非vip可接，2：vip可接
	int level; // 接任务最低等级限制：0 表示所有, 等级 >=该等级可接
	int level_max; // 接任务最高等级限制：0 表示所有, 等级 <=该等级可接
	int team; //组队限制：0：表示不受该条款限制 ，其它表示需要组队才可接
	int guild; // 公会条件：0：表示不受该条款限制,其它表示需要加入公会才可接
	int accept_times; // （当天）次数限制：0：表示不受该条款限制 表示无限次 其他数据N表示N次
	int accept_times_all;// 总次数：0：表示不限次数，其他数据N表示N次，只对主线支线有效
	int auto_accept; // 接取方式，1：可自动接，2：需玩家手动接，
	Task_Mission_Award_Detail_Cfg receive; // 接受任务后，玩家获得的中间任务物品
	Task_Mission_Award_Detail_Cfg paytype; // 接受任务后，玩家获得的中间任务物品
	int exp; // 经验
	int pet; // 接取这个任务获取到的宠物id
	uint8_t buf_op;//1:加buf 2:清理buf
	int buff; // buff id
	int buf_ms;//buf持续时间(单位：毫秒)    -1：永久
	int fly; // 变成飞行状态，单位ms
	int polt_id; // 剧情id,0无效，若不为0 直接进入剧情播放，播放完后任务接取动作完成
	Task_Mission_Action_Cfg action; //接取任务后触发的动作组, [剧情id, 动作id]
	int skill_id; // 技能id，学习一个技能
	uint32_t full_prop_num; //接任务时，回怒气的比例  (范围：0 - 100  注意：必须为整数)
//	int pay; // 接取任务需要消耗的铜钱数量，优先绑定再非绑定
	uint32_t val1;//物品ID(直接消耗悬赏令领取随机橙色任务)
	uint32_t val2;//物品数量
	uint32_t val3; //物品ID(先刷新，点领取, 消耗悬赏令, 接取橙色品质任务)
	uint32_t val4; //物品数量
};
struct Task_Mission_Exec_Detail_Cfg {
	Task_Mission_Exec_Detail_Cfg(void) { reset(); }
	void reset(void) {
		step = 0;
		id = 0;
		id_list.clear();
		npc_id = 0;
		sceneid = 0;
		target_x = 0;
		target_y = 0;
		amount = 0;
		action.reset();
		scene_monster_point_set.clear();
		type = 0;
		param1 = 0;
		param2 = 0;
		talk = 0;
	}

	int step;
	int id;
	Int_Set id_list;
	int npc_id;
	int sceneid;
	int target_x;
	int target_y;
	int amount;
	Task_Mission_Action_Cfg action;
	Scene_Monster_Point_Set scene_monster_point_set;
	int type;/*111110,//npc谈话类型任务，此类任务时id填npc id， amoount填1
				111111,//杀怪类型任务，此类任务时id填怪物类型，， amoount填数量

				111209,//amount填4，param1填品质
				111210,//amount填4，id填英雄id，param1填品质*/

	/*
	 * =======================   execute 说明  (不写即为不填)============================
	 *   111112 完成副本任务         id:场景id                                         amount:填1
	 *   111114 到达目的地           scene_id:场景id               param1:半径        param2: 不填     amount:填1
	 *   111116 采集物品            id: 物品id                                         amount:采集物品数量
	 *   111118 收集打怪掉落物品      id: 物品id                     param1:怪物id        amount:收集物品数量
	 *   111119 完成某类型副本任务     id:场景类型的数组，形如：[801,802,803]                  amount:填1
	 *   111121 加入公会                                                              amount:填1
	 *   111122 公会等级到达N级                                                         amount:公会等级
	 *   111123 玩家强化N次                                                            amount:强化次数
	 *   111124 监听强化指定部位N次                   param1:身体部位                      amount:强化次数
	 *   111125 强化指定部位的装备到N级                param1:身体部位                      amount:强化等级
	 *   111126 强化指定装备到N级(有装备限制)           param1:装备id                       amount:强化等级
	 *   111127 宝石提升到N级                                                           amount:宝石等级
	 *   111128 在商城购买某件物品                    param1:物品id      param2:道具二级分类类型 (2者取其一)  amount:填1
	 *   111129 获得指定英雄                         param1:英雄id                               amount:填1
	 *   111130 挑战NPC(与NPC对话进入战斗)             (策划配怪点)     amount:填1  (服务端数据：monsterGroup.val_1:场景id    monsterGroup.val_2:怪物组id)
	 *	 111131 任务提交时消耗XX道具（ID）XX个（数量）    id:物品id   				         amount: 物品数量
	 *	 111132 送信任务                            id:物品id      param1:是否绑定    amount: 物品数量
	 *   111133 协助其他玩家完成多人副本N次(对副本无限制，策划只配置次数)                     amount:协助其他玩家完成多人副本次数
     *   111134 出售X件道具（道具无限制，只检测出售个数）                                  amount:出售道具个数
     *   //111135 使用某个技能XX次（每个职业技能不一样，支持多种技能ID配置） id:技能id的数组，形如：[201,202,203]   amount:技能使用次数
     *   //111136 释放XX次追加技                                                                     amount:释放追加技次数
	 * =========================================================
	 */
	uint32_t param1;//附加参数1
	uint32_t param2;//附加参数2
	uint32_t talk;  //对话的npc_id
};
typedef std::vector<Task_Mission_Exec_Detail_Cfg> Task_Mission_Exec_Cfg;
struct Task_Mission_Failed_Cfg {
	Task_Mission_Failed_Cfg(void) { reset(); }
	void reset(void) {
		duration = 0;
		item_id = 0;
		level_max = 0;
		state = 0;
		hero = 0;
	}

	int duration;
	int item_id;
	int level_max;
	int state;
	int hero;
};
struct Task_Mission_Cmp_Cfg {
	Task_Mission_Cmp_Cfg(void) { reset(); }
	void reset(void) {
		is_empty = false;
		polt_id = 0;
		action.reset();
		skill_id = 0;
		fashion1 = 0;
		fashion2 = 0;
		paytype.reset();
		npc_id = 0;
	}

	bool is_empty;
	int polt_id;
	Task_Mission_Action_Cfg action;
	int skill_id;
	int fashion1;
	int fashion2;
	Task_Mission_Award_Detail_Cfg paytype;
	int npc_id;
};

struct Task_Mission_Award_Cfg {
	Task_Mission_Award_Cfg(void) { reset(); }
	void reset(void) {
		CLEAR_VECTOR(Task_Mission_Award_List_Cfg, item_list);
		CLEAR_VECTOR(Task_Mission_Award_List_Cfg, other_list);
	}
/*
1，道具
2，装备
3，经验
4，金币
5，英灵
6，声望
7，龙魂
8，钻石
9，绑定钻石
10，特殊物品
11. 英雄
12.多人副本次数
13.公会贡献
14.卡片
15.上古副本次数
16.体力
*/
	Task_Mission_Award_List_Cfg item_list;
	Task_Mission_Award_List_Cfg other_list;
};
struct Task_Mission_Detail_Config {
	Task_Mission_Detail_Config(void) { reset(); }
	void reset(void) {
		task_id = 0;
		task_type = 0;
		isNew = 0;
		progress = 0;
		quality = 0;
		giveuptask = 0;
		precondition = 0;
		postcondition = 0;
		postcondition2 = 0;
		postcondition3 = 0;
		recruid_hero = false;
		accept.reset();
		execute.clear();
		failed.reset();
		complete.reset();
		award.reset();
		chapter.clear();
		is_limit_time = false;
		limit_time = 0;
		is_fast_complete = false;
		money_type = 0;
		amount = 0;
		item_id = 0;
		item_num = 0;
		hero_op = 0;
		hero_id = 0;
		icon_hero_id = 0;
		show_or_close_icon = 0;
		gift_level = 0;
		day_group = 0;
		week_group = 0;
		horse_id = 0;
		is_magic_wing = false;
		vip_accept_level = 0;
		exp_times = 0;
		coin_times = 0;
		dragon_soul_times = 0;
		soul_times = 0;
		item1_times = 0;
		item2_times = 0;
		item3_times = 0;
		item4_times = 0;
		ring_type = 0;
		min_ring_num = 0;
		max_ring_num = 0;
		scene_id = 0;
		npc_op = 0;
		npc_scene_id = 0;
		npc_id = 0;
		talent_id = 0;
		job = 0;
		task_version = 0;
		is_show_in_task_list = false;
	}

	int task_id;
	int task_type;	/*任务类型，数字,101:主线任务、102:支线任务、103:日常任务、104:周常任务、105:公会任务、106:活动任务、107:精英任务，108活动，109英雄任务*/
	int isNew;	/*新手引导特殊追踪框，有则填1，无则不填*/
	int progress;/*新手引导特殊进度值，有则填具体数字，无则不填*/
	int quality;/*任务品质,0：白，1：绿，2：蓝，3：紫，4：橙*/
	int giveuptask;/*显示放弃任务按钮： 0不显示 1显示*/
	int precondition;
	int postcondition;
	int postcondition2;
	int postcondition3;
	bool is_limit_time;      //是否限时任务
	int limit_time;          //限时(单位：分钟)
	bool is_fast_complete;   //是否可以快速完成
	int money_type;          //快速完成消耗货币类型  1:只扣金币  2:优先扣粉钻,粉钻不足，再扣钻石 3:只扣钻石  4:只扣粉钻  5:只扣英灵
	int amount;              //快速完成消耗货币数量
	int item_id;             //快速完成消耗道具id
	int item_num;            //快速完成消耗道具数量
	bool recruid_hero;       //解封英雄相关
	uint8_t hero_op;         //添加/删除英雄。 0:无操作  1：添加 2：删除
	uint32_t hero_id;        //英雄id
	uint32_t icon_hero_id;   //任务触发新英雄图标的英雄id
	uint8_t show_or_close_icon;  //图标显示/关闭(1:显示图标 2:1个星  3：2个星 4:3个星  5:4个星 6:5个星，并自动消失)
	uint16_t gift_level;     //任务开启礼包等级
	uint32_t day_group;      //日常组
	uint32_t week_group;     //周常组
	uint32_t horse_id;       //坐骑id
	bool is_magic_wing;     //是否魔翼相关
	uint32_t vip_accept_level; //领取vip等级(vip日常任务可配)
	uint32_t exp_times;           //经验倍数(日常周常任务)
	uint32_t coin_times;          //金币倍数(日常周常任务)
	uint32_t dragon_soul_times;   //龙魂倍数(日常周常任务)
	uint32_t soul_times;          //英灵倍数(日常周常任务)
	uint16_t item1_times;       //道具1倍数(日常周常任务)
	uint16_t item2_times;       //道具2倍数(日常周常任务)
	uint16_t item3_times;       //道具3倍数(日常周常任务)
	uint16_t item4_times;       //道具4倍数(日常周常任务)
	uint8_t ring_type; 			//环任务类型
	uint32_t min_ring_num;      //环下限
	uint32_t max_ring_num;      //环上限
	uint32_t scene_id;
	uint8_t npc_op;
	uint32_t npc_scene_id;
	uint32_t npc_id;
	uint32_t talent_id;
	uint8_t job;
	uint32_t task_version;
	bool is_show_in_task_list;

	Task_Mission_Accept_Cfg accept;
	Task_Mission_Exec_Cfg execute;
	Task_Mission_Failed_Cfg failed;
	Task_Mission_Cmp_Cfg complete;
	Task_Mission_Award_Cfg award;
	std::string chapter;
};
typedef boost::unordered_map<int, Task_Mission_Detail_Config> Task_Mission_Config;
struct Task_Config {
	typedef boost::unordered_set<int> TaskIdSet;
	typedef boost::unordered_set<int> IntSet;
	typedef boost::unordered_map<int, IntSet> IntSetMap;
	Task_Config(void) { reset(); }
	void reset(void) {
		/*task_setting.reset();*/
//		task_ids.clear();
//		npc_task_map.clear();
		magic_wing_try_id = 0;
		missions.clear();
	}

	/*Task_Setting_Config task_setting;*/
//	TaskIdSet task_ids;
//    IntSetMap npc_task_map;
	uint32_t magic_wing_try_id;
    Task_Mission_Config missions;
};
// TASK CONFIG ==================================

#endif /* TASK_CONFIG_H_ */
