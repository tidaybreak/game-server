/*  
* Generate by devtool
*/

#ifndef _MSG_ACTIVE_H_ENUM_
#define _MSG_ACTIVE_H_ENUM_

enum Msg_Active_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		ACTIVE_PLAYER_APPEAR = 81000001,				//玩家出现         

		ACTIVE_MOVER_DISAPPEAR = 81000010,				//对象消失(玩家/NPC)         

		ACTIVE_MOVER_MOVE = 81000020,				//Mover(玩家/NPC/宠物)移动         

		ACTIVE_NPC_APPEAR = 81000005,				//NPC出现         

		SERVER_ERROR_MSG_TO_CLIENT = 80000001,				//错误提示         

		ACTIVE_LINK_CLOSE = 80000005,				//通知关闭连接         

		ACTIVE_PLAYER_LOGIN = 81000000,				//登录或创建角色成功时返回         

		ACTIVE_SCENE_TRANSFER = 81000030,				//转场景通知         

		ACTIVE_PACK_ITEM_UPDATE = 82100000,				//背包物品更新         

		ACTIVE_PACK_CAPACITY_UPDATE = 82100001,				//背包容量改变         

		ACTIVE_PACK_MONEY_UPDATE = 82100002,				//金钱更新         

		ACTIVE_PACK_ITEM_ERASE_UPDATE = 82100003,				//背包物品删除         

		ACTIVE_ITEM_ADDIT_INFO_UPDATE = 82100004,				//物品附加信息更新         

		ACTIVE_TRADE_LIMIT_INFO = 82100005,				//通知交易额度信息         

		ACTIVE_ROLE_BASE_INFO = 81000100,				//场景服登录通知-基础信息         

		ACTIVE_TRANSMIT_POINT = 81000040,				//更新地图传送点数据         

		ACTIVE_OPEN_ITEM_GET_LIST = 82100006,				//使用物品得到物品列表         

		ACTIVE_TASK_LIST_UPDATE = 81110000,				//任务信息更新         

		ACTIVE_CLIENT_FIND_PATH = 81000041,				//寻路传送点数据通知         

		ACTIVE_INTO_BATTLE = 81000050,				//进入战斗模式         

		ACTIVE_BATTLE_OVER = 81000052,				//结束战斗通知         

		ACTIVE_BATTLE_EXIT = 81000055,				//退出战斗模式         

		ACTIVE_FIGHTER_APPEAR = 81000008,				//战斗单位出现         

		ACTIVE_FIGHTER_PROP_CHANGE = 81000060,				//属性信息更新(战斗属性)         

		ACTIVE_SKILL_ACTION = 81000070,				//技能动作效果         

		ACTIVE_RECEIVE_MAIL = 82200000,				//收到邮件通知         

		ACTIVE_RESET_COORD = 81000015,				//重置坐标         

		ACTIVE_PLAYER_DIE = 81000045,				//通知玩家死亡         

		ACTIVE_CHAT_BOARD = 83000000,				//聊天广播         

		ACTIVE_ROLE_INFO = 81000101,				//更新人物属性信息         

		ACTIVE_FRIEND_APPLY = 82200001,				//好友申请列表推送         

		ACTIVE_GATHER_END = 81000110,				//采集结束通知         

		ACTIVE_HERO_ADD = 81001000,				//增加英雄信息更新         

		ACTIVE_HERO_INFO = 81001001,				//英雄信息更新         

		ACTIVE_BTL_BUFF_UPDATE = 81000071,				//战斗buff添修改         

		ACTIVE_NPC_TRIGGER_SPOT = 81000201,				//怪物触发剧情         

		ACTIVE_TEAM_LIST = 80100300,				//广播房间列表         

		ACTIVE_TEAM_WAITER = 80100301,				//广播等待区信息         

		ACTIVE_TEAM_INFO = 80100302,				//通知小队信息         

		ACTIVE_TEAMER_ONLINE = 80100303,				//通知队员上线         

		ACTIVE_TEAMER_OFFLINE = 80100304,				//通知队员下线         

		ACTIVE_TEAMER_JOIN = 80100305,				//通知队员入队         

		ACTIVE_TEAMMER_LEAVE = 80100306,				//通知队员离队         

		ACTIVE_TEAM_DISSMISSION = 80100307,				//通知小队解散         

		ACTIVE_ENQUEUE_INVITATION = 80100308,				//通知入队邀请         

		ACTIVE_ENQUEUE_APPLY = 80100309,				//通知入队申请         

		ACTIVE_REJECT_ENQUEUE_INVITATION = 80100310,				//通知拒绝入队邀请         

		ACTIVE_TEAM_LEADER_CHANGE = 80100312,				//通知小队队长改变         

		ACTIVE_TEAMER_STATUS_CHANGE = 80100313,				//通知队员状态改变         

		ACTIVE_TEAMMER_UNREADY = 80100317,				//通知队员未准备         

		ACTIVE_FRIEND_ADD = 82200002,				//好友同意添加         

		ACTIVE_TEAM_SET = 80100318,				//广播房间信息         

		ACTIVE_MALL_GOODS = 82151000,				//通知商城列表变化         

		ACTIVE_BATTLE_PLOT_STOP = 81000202,				//战斗剧情结束通知         

		ACTIVE_BATTLE_PAUSE = 81000203,				//战斗挂起通知         

		ACTIVE_BATTLE_RESUME = 81000204,				//战斗恢复通知         

		ACTIVE_LOGIC_ROLE_BASE_INFO = 81000099,				//逻辑服登录通知-基础信息         

		ACTIVE_NPC_TALK = 81000205,				//npc对话通知         

		ACTIVE_NPC_APPEAR_TASK = 81110001,				//任务随身怪出现         

		ACTIVE_TASK_LINEUP_INFO = 80100319,				//广播组队布阵信息         

		ACTIVE_FLY = 81000025,				//请求飞行(坐电梯)         

		ACTIVE_GANG_ROW_INFO = 81070000,				//公会列表信息更新         

		ACTIVE_GANGER_ROW_INFO = 81070001,				//公会成员列表更新信息         

		ACTIVE_MOVER_SPEED_CHANGE = 81000021,				//移动速度更改通知         

		ACTIVE_UPDATE_TIME = 80000010,				//同步时间         

		ACTIVE_MOVEABLE_INFO = 81000022,				//地图可走信息通知         

		FCM_TIME_TIP = 82200100,				//防沉迷时间提示         

		FCM_LOGIN_TIME_TIP = 82200101,				//防沉迷登陆提示         

		ACTIVE_MOVE_SCENE_BEAK_BACK = 81000023,				//移动场景击退通知         

		ACTIVE_GANG_PASSED_EVENT = 81070002,				//公会动态信息更新         

		ACTIVE_GANG_INFO_UPDATE = 81070003,				//公会信息更新         

		ACTIVE_GANGER_INFO_UPDATE = 81070004,				//玩家公会信息更新         

		ACTIVE_MOVE_SCENE_BEAK_BACK_START = 81000024,				//移动场景击退开始通知         

		ACTIVE_NPC_PATH_GUIDE = 81000016,				//副本npc路径点指引         

		ACTIVE_MOVE_UNLINK_INFO = 81000017,				//地图不可联通点通知         

		ACTIVE_MONSTER_START_CHASE = 81000018,				//怪物开始追击         

		ACTIVE_RUNE_HOLE_INFO = 81002000,				//符文孔信息更新         

		ACTIVE_ARENA_LOCAL_RANK_CHANGE = 81000300,				//竞技场本服排名变化通知         

		ACTIVE_ARENA_INTER_RANK_CHANGE = 81000301,				//竞技场跨服排名变化通知         

		ACTIVE_MOVER_PLAY_ACTION = 81000014,				//对象播放动画         

		ACTIVE_BUFF_PROP_CHANGE = 81000061,				//属性信息更新(buff属性)         

		ACTIVE_OFFLINE_CHAT_BOARD = 83000001,				//离线聊天记录广播         

		ACTIVE_OPEN_CARD = 81000080,				//多人副本翻牌结果         

		ACTIVE_BUFF_CHANGE = 81000062,				//buff效果广播         

		ACTIVE_ICON_LIST = 80100400,				//发送功能图标开启信息列表         

		ACTIVE_ICON = 80100401,				//发送开启/关闭功能图标         

		ACTIVE_START_OPEN_CARD = 81000081,				//通知开始翻牌         

		ACTIVE_ARENA_PLAYER_INFO_CHANGE = 81000302,				//竞技场个人信息变化通知         

		ACTIVE_NEW_HAND_DISABLE = 80100500,				//主动推送不能引导的列表         

		ACTIVE_PACK_ITEM_EFFECT = 82100007,				//获得物品飘特效         

		ACTIVE_PACK_MODIFY = 81000102,				//背包获得客户端特效         

		ACTIVE_UPDATE_ROLE_STATE = 81000026,				//更新人物在场景的状态         

		ACTIVE_NPC_APPEAR_TASK_SECOND = 81110002,				//任务随身怪出现         

		ACTIVE_GANG_NOTIFY = 81070005,				//公会主动通知         

		ACTIVE_PHY_POWER_CHANGE = 81000063,				//体力信息更新         

		ACTIVE_PACK_GATHER_TIP = 81000103,				//途径获得提示         

		ACTIVE_DUNGEON_COST = 81000064,				//副本消耗通知         

		ACTIVE_BATTLE_NEXT_WAVE = 81000051,				//战斗下一波怪开始通知         

		ACTIVE_HEROER_INFO = 82210000,				//主动通知人物面板英雄信息         

		ACTIVE_TAKE_LIFT_START = 81000027,				//电梯升降开始通知         

		ACTIVE_TAKE_LIFT_END = 81000028,				//电梯升降结束通知         

		ACTIVE_LIMIT_TASK_TIMEOUT = 81110003,				//主动发送限时任务超时未完成消息         

		ACTIVE_FRIEND_RECOMMEND = 82200003,				//通知推荐好友         

		ACTIVE_FAST_FINISH_ASK_SUB_MONEY = 81110005,				//快速完成任务主动发送咨询是否愿意消耗货币的消息         

		ACTIVE_AUTO_SUBMIT_AWARD = 81110006,				//自动提交的任务，通知客户端弹出奖励提示框         

		ACTIVE_REWARD_TASK_LIST = 81110007,				//主动推送悬赏任务列表         

		ACTIVE_TEAM_MATCH_INFO = 80100320,				//广播匹配双方信息         

		ACTIVE_TEAM_IN_MATCH = 80100321,				//匹配中         

		ACTIVE_ARENA_THREE_DAY_AWARD_INFO = 81000303,				//竞技场玩家奖励信息         

		ACTIVE_TEAM_SPORTS_RANKING = 80100322,				//广播组队竞技积分         

		ACTIVE_WAR_SCENE_INFO = 81000400,				//战场信息         

		ACTIVE_WAR_SCENE_PLAYER_INFO = 81000401,				//战场玩家信息更新         

		ACTIVE_DAILY_INFO = 82200200,				//通知日常活跃度完成次数改变         

		ACTIVE_WAR_SCENE_BALANCE = 81000402,				//战场结算通知         

		ACTIVE_WAR_SCENE_HEAL = 81000403,				//战斗治疗成功通知         

		ACTIVE_WAR_SCENE_INFO_CHANGE = 81000404,				//战场信息更新         

		ACTIVE_WAR_SCENE_PLAYER_REVIVE = 81000405,				//战场玩家复活通知         

		ACTIVE_TEAM_HERO_LIST = 80100323,				//英雄梦境开启列表         

		ACTIVE_SELF_DRAGON_VALE_ALL_INFO = 82300000,				//进入自己龙谷，主动发送龙谷内的所有信息         

		ACTIVE_OUTPUT_PRODUCTION = 82300001,				//主动推送建筑的资源产出         

		ACTIVE_BUILDING_STATUS_CHANGE = 82300002,				//主动通知龙谷内的状态发生改变的建筑列表         

		ACTIVE_PLAYER_APPEAR_ADD_INFO = 81000002,				//玩家附加信息更新         

		ACTIVE_TEAM_HERO_DREAM_AWARD = 81000082,				//广播英雄梦境奖励         

		ACTIVE_TOD_HELPER_FORCE_CHANGE = 82200210,				//TOD助手通知战斗力提升达到条件产于模块         

		ACTIVE_ITEM_LACK = 82100008,				//不足的物品列表         

		ACTIVE_OTHER_DRAGON_VALE_ALL_INFO = 82300003,				//进入别人龙谷，主动推送龙谷内的信息         

		ACTIVE_CHAT_TO_FAST = 83000002,				//聊天广播过快         

		ACTIVE_DRAGON_BORN_FINISH = 82300004,				//孵化龙蛋完成，主动通知         

		ACTIVE_DRAGON_FEELING_DOWN = 82300005,				//时间消耗、被掠夺，龙的心情值下降，主动通知         

		ACTIVE_COVENANT_STATUS = 82200220,				//主动推送精灵盟约状态         

		ACTIVE_EVOLUTE_DRAGON_ITEM_NOT_ENGOUGH = 82300006,				//进化龙不足，提示是否愿意消耗多少货币         

		ACTIVE_WAR_SCENE_NOTICE = 81000406,				//战场飘字通知         

		ACTIVE_CAN_PLAY_DRAGON = 82300007,				//倒计时结束，由不可逗龙变为可以逗龙，主动通知         

		ACTIVE_WAR_SCENE_KICK_PLAYER = 81000407,				//战场踢人通知         

		ACTIVE_REWARD_COVENANT_FINISH_REWARD_TASK = 81110008,				//通知客户端，使用悬赏精灵完成悬赏任务         

		ACTIVE_COVENANT_HANDSEL = 82200221,				// 通知被赠送精灵盟约         

		ACTIVE_COVENANT_EXPIRED = 82200222,				//通知精灵盟约过期         

		ACTIVE_DRAGON_VALE_BUILDING_MAX_STORAGE = 82300008,				//龙谷系统资源存储达到上限，主动通知不再产出         

		ACTIVE_DRAGON_VALE_BUILDING_FINISH_WORD = 82300009,				//建筑建造或者升级完成提示         

		ACTIVE_ACT_ITEM_NOTIFY = 81000408,				//活动图标变更信息通知         

		ACTIVE_ACHIEVEMENT_COMPLETE = 81100800,				//成就完成         

		ACTIVE_MASTER_OF_STEP_FOOT_DRAGON_VALE = 82300010,				//主动通知龙谷主人被踩脚印         

		ACTIVE_VIP_STATUS_INFO = 82200230,				//通知VIP状态信息         

		ACTIVE_PANAL_HERO_TAGS = 82210001,				//主动通知人物面板英雄标签更新         

		ACTIVE_BATTER_STATISTICS = 81000065,				//战斗连击统计广播         

		ACTIVE_ASK_MASTER_OF_DRAGON_IS_FIGHT = 82300012,				//有人来掠夺龙谷，给龙谷主人提示，是否应战         

		ACTIVE_NOTIFY_ROBBER_FIGHT_LIMIT_TIME = 82300013,				//通知龙谷掠夺者进入战斗倒计时         

		ACTIVE_NOTIFY_ROBBER_FIGHT_END = 82300014,				//通知掠夺者掠夺战斗结束         

		ACTIVE_NOTIFY_MASTER_OF_DRAGON_FIGHT_END = 82300015,				//通知被掠夺龙谷的主人，战斗结束         

		ACTIVE_ROBBER_GAIN_PRODUCTION = 82300017,				//主动通知掠夺者在被掠夺者的跨服龙谷内收获了多少资源         

		ACTIVE_MASTER_OTHER_ROB_TOTAL_PRODUCTION = 82300018,				//主动通知龙谷主人被掠夺了多少资源         

		ACTIVE_ACHIEVEMENT_INFO = 81100801,				//更新成就状态         

		ACTIVE_SYNC_ROBBED_BUILDING = 82300019,				//龙谷主人的建筑资源被掠夺者收获，更新建筑信息(预留 一键收获掠夺资源的返回))         

		ACTIVE_AVATAR_CHANGE = 81000003,				//avatar更改通知         

		ACTIVE_GET_TITLE = 81000900,				//玩家获得称号         

		ACTIVE_LOSE_TITLE = 81000901,				//玩家失去称号         

		ACTIVE_TITLE_UPDATE = 81000902,				//玩家称号更新         

		ACTIVE_ROBBER_ENTER_OTHER_DRAGON_VALE = 82300020,				//掠夺者进入被掠夺者的龙谷         

		ACTIVE_BUILIDNG_QUEUE_CHANGE = 82300022,				//VIP等级改变，主动通知建筑队列数目改变         

		ACTIVE_RECEIVE_FLOWERS = 82200240,				//通知收到鲜花         

		ACTIVE_FLOWERS_EFFECT = 82200241,				//通知全屏飘花效果         

		ACTIVE_GANG_END_TIME = 81170000,				//公会战倒计时         

		ACTIVE_GANG_WAR_INFO_SELF = 81170001,				//公会战个人信息         

		ACTIVE_GANG_WAR_INFO = 81170002,				//公会战信息         

		ACTIVE_GANG_WAR_GATHER = 81170003,				//公会战采集主动信息         

		ACTIVE_GANG_WAR_AWARD = 81170004,				//公会战奖励         

		ACTIVE_POINTS = 81080000,				//主动更新积分         

		ACTIVE_TEXT_PROMPT = 81003000,				//服务器请求飘字         

		ACTIVE_CHECK_IN_INFO = 82200201,				//通知签到信息         

		ACTIVE_ONLINE_REWARD_INFO = 82200202,				//通知在线奖励状态信息         

		ACTIVE_GANG_WAR_TOWER_INFO = 81170005,				//公会战资源塔信息         

		ACTIVE_NOTIFY_ROB_RESULT_ONLINE_LATER = 82300023,				//掠夺者玩家上线，通知掠夺战斗结果         

		ACTIVE_NOTIFY_TARGET_RESULT_ONLINE_LATER = 82300024,				//被掠夺者玩家上线，通知掠夺战斗结果         

		ACTIVE_GATHER_CHANGE_CD = 81000111,				//更改采集时间通知         

		ACTIVE_PLOT_INFO = 81110009,				//广播剧情信息         

		ACTIVE_OFFLINE_TARGET_TOTAL_ROBBED_PRODUCTION = 82300025,				//被掠夺者上线，通知龙谷资源被掠夺了多少         

		ACTIVE_CAMPFIRE_INFO_UPDATE = 81000700,				//篝火信息变更通知         

		ACTIVE_DRAONG_VALE_NO_ENTER = 82300026,				//龙谷正在被掠夺，不可进入         

		ACTIVE_GANG_WAR_HEAL = 81170006,				//公会战战斗治疗成功通知         

		ACTIVE_GANG_WAR_NOTICE = 81170007,				//公会战飘字通知         

		ACTIVE_ROB_MATCH_FAIL = 82300027,				//掠夺匹配失败         

		ACTIVE_GANG_WAR_BOARD_NOTICE = 81170008,				//公会战弹框提示         

		ACTIVE_GANG_WAR_DUEL = 81170009,				//提示公会战决斗         

		ACTIVE_NOTIFY_DRAGON_VALE_SPECIAL = 82300028,				//恭喜你成为成为至尊宝         

		ACTIVE_NOTFIY_ACCEPT_OR_SUBMIT_TASK = 82300029,				//通知客户端弹出接任务、提交任务特效         

		ACTIVE_DRAGON_VALE_HEAD_INFO_CHANGE = 82300030,				//主动推送龙谷头像区信息改变通知         

		ACTIVE_NOTIFY_DRAGON_VALE_STAR_UP = 82300031,				//被踩脚印的人，上线后，提示被踩并且星级提升         

		ACTIVE_STRING_INFO_CHANGE = 81000066,				//字符串信息改变更新         

		ACTIVE_DRAGON_BORN_LEFT_TIME = 82300032,				//玩家在主城中，显示龙谷龙蛋孵化剩余时间         

		ACTIVE_WIPE_OUT_SCENE_ID = 81000409,				//重新登录扫荡副本id通知         

		ACTIVE_INT_INFO_CHANGE = 81000067,				//整型信息改变更新         

		ACTIVE_COPY_CHAPTER_AWARD_CMD = 82100009,				//副本章节奖励领取通知         

		ACTIVE_RUNE_INFO = 82210002,				//主动通知人物面板符文信息         

		ACTIVE_EQUIP_MASTER_INFO = 82210003,				//主动推精通数据         

		ACTIVE_NOTIFY_DRAONG_VALE_INFO = 82300033,				//主动通知龙谷信息(龙谷以外的场景接收)         

		ACTIVE_GM_ANNOUNCE = 83000003,				//GM广播公告         

		ACTIVE_COPY_STATUS_CMD = 81000019,				//副本状态更改通知         

		ACTIVE_BATCH_EVOLUTE_DRAGON_ITEM_NOT_ENGOUGH = 82300034,				//批量进化龙， 提示是否愿意消耗多少道具、多少货币         

		ACTIVE_RECEIVE_REBATE_RECHARGE_STATUS = 82200203,				//充值返利可领取通知         

		ACTIVE_COVENANT_CRAD_NEED = 82200223,				//通知精灵盟约卡使用         

		ACTIVE_TEAM_IS_FIRST = 80100324,				//广播组队副本是否为第一次且未开始战斗         

		MSG_UPDATE_ROB_PROTECT_TIME = 82300035,				//更新被掠夺保护时间         

		ACTIVE_NOTIFY_PLAYED_PLOT_ID_LIST = 82300036,				//主动通知已播放的剧情列表         

		ACTIVE_ARENA_PERSONAL_INFO_CHANGE = 81000304,				//竞技场个人信息变化通知         

		ACTIVE_ARENA_FIRST_THREE_CHANGE = 81000305,				//竞技场前三名变化通知         

		ACTIVE_REFRESH_DRAGON_VALE_OPERATOR = 82300037,				//通知客户端更新龙谷操作区         

		ACTIVE_ANCI_ARTIFACT_COMPLETE = 81101100,				//上古神器条伯完成         

		ACTIVE_REFRESH_UPGRADE_INVEST_PAGE = 82310000,				//主动通知更新升级投资的页面信息         

		ACTIVE_WAR_SCENE_KICK_PLAYER_NOTICE = 81000410,				//战场3分钟没有积分变化提示         

		ACTIVE_ITEM_USE_GAIN = 81000104,				//使用道具获得效果飘字         

		ACTIVE_UPGRADE_INVEST_PROFIT_BUTTON_LIST = 82310001,				//主动通知升级投资的领取按钮列表信息         

		ACTIVE_NOTIFY_CAN_CREATE_DRAONG_BUILDING_NUM = 82300038,				//主动通知龙谷可建造的建筑数量         

		ACTIVE_CLOSE_REWARD_TASK_AUTO_REFRESH = 81110010,				//主动通知客户端关闭悬赏榜自动刷新倒计时         

		ACTIVE_MONTH_CARD_INVEST_PROFIT_BUTTON_STATUS = 82310002,				//主动通知客户端月卡投资领取收益按钮变为可领取         

		ACTIVE_EXPEDITION_CASTLE_INFO = 81101300,				//远征建筑信息列表         

		ACTIVE_NOTIFY_CANCEL_CREATE_BUILDING_NUMBER_LIST = 82300039,				//主动通知客户端龙谷已取消建造建筑的编号列表         

		ACTIVE_EXPEDITION_BROADCAST = 81101301,				//远征相应通知         

		ACTIVE_EXPEDITION_REVIVE = 81101302,				//远征倒计时通知         

		ACTIVE_INVEST_PLAN_HISTORY_CHANGE = 82310003,				//主动通知客户端投资历史更新         

		ACTIVE_ACTION_CLIENT_OPEN_NOTICE = 82200211,				//活动倒计时和开启活动预告通知         

		ACTIVE_TEAM_FB_AWARD_CUMULATE = 80100325,				//广播组队累计奖励         

		ACTIVE_UPDATE_NEXT_MATCH_CONSUME = 82300040,				//通知客户端更新下一次匹配消耗数量         

		ACTIVE_NOTIFY_COMMIT_ITEM = 81110011,				//通知客户端显示/关闭提交道具链接         

		ACTIVE_NOTFIY_IS_PLAY_DRAGON = 82300041,				//通知是否可以逗龙         

		ACTIVE_CAN_GET_PROFIT_BUTTON_NUM = 82310004,				//主动通知客户端所有可领取的按钮数量         

		ACTIVE_GUIDE_STEP_LIST = 80100501,				//主动推送引导步骤列表         

		ACTIVE_REFRESH_PLAY_DRAGON_LEFT_TIME = 82300042,				//主动通知更新龙信息面板的逗龙冷却时间         

		REQ_YOUR_FRIEND_INVITE_YOU_TO_STEP_FOOT = 82300043,				//你的好友邀请去踩脚印         

		ACTIVE_YOU_NEED_TO_ENTER_DRAGON_VALE_SCENE = 82310005,				//别人来进攻掠夺龙谷，需要转龙谷场景         

		ACTIVE_WORLD_LEVEL = 80100600,				//广播世界等级         

		ACTIVE_REWARD_TASK_CAN_ACCEPT_NUM = 81110012,				//主动通知悬赏任务剩余领取次数         

		ACTIVE_ERROR_EQUIP_PUT_ON = 81001002,				//装备穿上错误提示         

		ACTIVE_NOTE_KNIGHT_TRIALS_NUM = 82200250,				//通知骑士试练剩余次数         

		ACTIVE_CURRENT_FORENOTICE = 81110013,				//主动通知当前预告         

		ACTIVE_SEND_ANSWER_QUESTION_INDO = 82330000,				//通知智慧试练题目更新         

		ACTIVE_SEND_ANSWER_RANK_INFO = 82330001,				//通知刷新智慧试练排行榜信息         

		ACTIVE_ANSWER_OUT_NOTE = 82330002,				//通知智慧试练退出倒计时通知         

		ACTIVE_OPEN_TRANSPOINT = 81110014,				//开启传送点         

		ACTIVE_ALL_TRANSPOINT = 81110015,				//通知全部传送点         

		ACTIVE_ELITE_TRAIL_INFO = 81000090,				//上古副本获得追踪信息         

		ACTIVE_TEAM_DEF_BOSS_INFO = 80100326,				//广播boss信息         

		ACTIVE_TEAM_DEF_TIME = 80100327,				//守卫帝都剩余时间         

		ACTIVE_TEAM_DEF_APPEAR_TIME = 80100328,				//怪物出现倒计时         

		ACTIVE_BATTLE_AI_FORESHOW = 81000056,				//战斗怪物ai预告         

		ACTIVE_TEAM_DUNGEON_REWARD = 81000500,				//多人副本通关结算         

		ACTIVE_SERVER_CLOSE_TIP = 80000006,				//服务器即将关闭提示         

		ACTIVE_SCENE_NPC_SHOW = 81110016,				//要显示的场景npc         

		ACTIVE_SCENE_NPC_OP = 81110017,				//添加/删除场景npc         

		ACTIVE_TEAM_DEF_FAIL = 80100329,				//广播失败——守卫帝都         

		ACTIVE_OPEN_TALENT_SKILL_MODE = 81110018,				//打开天赋技能面板         

		ACTIVE_NOTIFY_TRY_TIMEOUT = 81110019,				//主动通知体验过期         

		ACTIVE_NOTIFY_TRY_TIMEOUT_LIST = 81110020,				//主动通知体验过期列表         

		ACTIVE_COPY_FIRST_COMPLETE = 81000029,				//副本首次通关通知         

		ACTIVE_EXPEDITION_BUILD_INFO_CHANGE = 81101303,				//远征建筑状态变化         

		ACTIVE_TODAY_REC_BIND_GOLD_NUM = 82200204,				//通知最新的今日获得粉钻数         

		ACTIVE_NOTIFY_TASK_GUIDE = 81110021,				//主动通知任务引导         

		ACTIVE_WORLD_BOSS_INFO = 81000600,				//世界boss信息更新         

		ACTIVE_WORLD_BOSS_ACT_GUILD_BLESS = 81000601,				//世界boss公会激活通知         

		ACTIVE_WORLD_BOSS_AWARD = 81000602,				//世界boss奖励通知         

		ACTIVE_WORLD_BOSS_BLOOD_UPDATE = 81000603,				//世界boss血量更新通知         

		ACTIVE_WORLD_BOSS_RANK_UPDATE = 81000604,				//世界boss伤害排名更新通知         

		ACTIVE_WORLD_BOSS_ACT_TIME = 81000605,				//世界boss活动时间更新         

		ACTIVE_EXPEDITION_BUILD_INFO = 81101304,				//远征切线建筑信息列表         

		ACTIVE_ELITE_BUY_ADD_NUM = 81000800,				//上古副本弹购买额外挑战次数框         

		ACTIVE_TEAM_SPORTS_RESULT = 80100331,				//广播组队竞技战斗结果         

		ACTICE_TEAM_AWARD = 80100332,				//通知组队积分奖励         

		ACTIVE_GEM_FORMULA = 80100316,				//发送宝石合成开启状态         

		ACTIVE_VALENTINE_MONSTER_RESULT = 81000610,				//情人节杀怪活动结果通知         

		ACTIVE_ESSENCE = 81090000,				//主动更新精华         

		ACTIVE_CAMPFIRE_SPOT_INFO = 81000701,				//请求篝火结点面板         

		ACTIVE_CAMPFIRE_FIRE = 81000702,				//篝火个人信息更新         

		ACTIVE_ARENA_PANEL_FIRST_INFO = 81000306,				//竞技场战斗结束后更新竞技大界面第一名玩家信息         

		ACTIVE_MOVER_STATE_CHANGE = 81000013,				//对象状态改变通知         

		EXPEDITION_ACTIVE_REFRESH_MONSTER = 81101305,				//远征刷boss         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_ACTIVE_H_ENUM_ */
