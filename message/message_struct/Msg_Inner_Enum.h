/*  
* Generate by devtool
*/

#ifndef _MSG_INNER_H_ENUM_
#define _MSG_INNER_H_ENUM_

enum Msg_Inner_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		INNER_SYNC_SCENE_DETAIL = 20100010,				//逻辑服同步场景Detail到场景服         

		INNER_SYNC_MOVER_DETAIL = 20200010,				//地图同步Mover到逻辑服         

		INNER_TRAN_SCENE_SAME_PRO = 20200050,				//同进程改变场景         

		INNER_ASK_SCENE_SIGN_OUT = 20200055,				//逻辑通知场景退出         

		INNER_DIFF_PROCESS_SCENE = 20200060,				//跨进程改变场景         

		INNER_SYNC_MONITOR_LINK = 20100015,				//同步Monitor_Link         

		INNER_ASK_SCENE_SIGN_IN = 20200056,				//逻辑通知场景登录         

		INNER_SYNC_FIGHTER_DETAIL = 20200013,				//地图同步Fighter到逻辑服         

		INNER_CHANGE_FIGHT_PROP = 20200100,				//逻辑服战斗属性改变         

		INNER_BATTLE_OVER_TO_LOGIC = 20200200,				//战斗结束通知逻辑服         

		INNER_SCENE_PROP_CHANGE = 20200110,				//场景服属性改变通知         

		INNER_GET_DROPS_GOODS = 20300000,				//拾取掉落物品         

		INNER_SCENE_COMPLETE = 20200080,				//场景完成通知(所有场景)         

		INNER_LEARN_SKILL_SUCCESS = 20100018,				//同步成功学习技能         

		INNER_GATHER_GOODS = 20300010,				//添加采集物到背包         

		SYNC_TASK_INFO = 20110001,				//同步任务数据         

		INNER_NPC_TRIGGER_SPOT_END = 20300020,				//NPC触发的剧情结束通知         

		INNER_SYNC_HERO_BASE_DETAIL = 20400000,				//同步英雄基本信息         

		INNER_SYNC_HERO_FIGHTER_DETAIL = 20400001,				//同步英雄战斗信息         

		INNER_SYNC_HERO_PROPERTY = 20400002,				//通知场景服修改英雄属性         

		INNER_SYNC_HERO_OUT = 20400003,				//通知场景服英雄跟随或收回         

		INNER_SYNC_HERO_DIE_TO_LOGIC = 20400004,				//通知逻辑服英雄死亡         

		INNER_SYNC_HERO_INFO_TO_LOGIC = 20400005,				//同步英雄信息到逻辑服         

		INNER_NOTICE_SIGN_SCENE = 20100012,				//通知逻辑服玩家登录         

		INNER_ERASE_SKILL = 20100019,				//同步洗点技能         

		INNER_MOVE_SKILL = 20100020,				//同步移动技能位置         

		INNER_ADD_GOODS_TO_PACKAGE = 20300011,				//添加物品到背包（包括金钱、英灵等）         

		INNER_OPEN_TALENT = 20100021,				//同步开启天赋         

		INNER_OPEN_TALENT_PAGE = 20100022,				//同步开启天赋页         

		INNER_USE_TALENT_PAGE = 20100023,				//同步使用天赋页         

		INNER_SYNC_GANG_INFO = 20170000,				//同步公会信息         

		INNER_SKILL_COST = 20100024,				//技能逻辑服消耗         

		INNER_SKILL_COST_SUCCESS = 20100025,				//技能逻辑服成功消耗         

		INNER_SYNC_BLACK_LIST = 20160000,				//同步黑名单列表         

		INNER_ANNOUNCE_SYSTEM = 20500000,				//发布系统公告         

		LOGIC_CHAT_CLIENT_READY_NOW = 20010085,				//通知聊天服客户端初始化完成         

		INNER_ARENA_BATTLE_START = 20300030,				//逻辑服通知场景服竞技场战斗开始         

		INNER_ARENA_BATTLE_END = 20300031,				//场景服通知逻辑服竞技场战斗结束         

		INNER_TEAM_SCENE_COMPLETE = 20200081,				//多人场景完成通知(所有场景)         

		INNER_CHAT_SYNC_TEAMER_INFO = 20100301,				//同步个人队伍信息到聊天服         

		INNER_CHAT_SYNC_TEAM_INFO = 20100302,				//同步队伍信息到聊天服         

		INNER_SYNC_HERO_FIGHT = 20400007,				//逻辑服通知地图服英雄战斗         

		INNER_SYNC_PHY_POWER_INFO = 20100030,				//同步体力信息         

		INNER_SCENE_SYNC_TEAMER_INFO = 20100303,				//同步个人队伍信息到地图服         

		INNER_SCENE_SYNC_TEAM_INFO = 20100304,				//同步队伍信息到地图服         

		INNER_ADD_BLOOD_POOL = 20100305,				//使用物品需要地图服处理的道具         

		INNER_HERO_OUT_FIGHT = 20400008,				//地图服通知英雄出战         

		INNER_GATHER_GOODS_RESULT = 20300012,				//添加采集物到背包结果         

		INNER_FIGHTER_RESTORE_FULL_PROP = 20100400,				//逻辑服通知地图服回满指定属性         

		INNER_SYNC_STATUS = 20100500,				//传递状态数据         

		LOGIC_CHAT_CHAT_BUGLE = 20100099,				//通知聊天服小喇叭聊天         

		INNER_BATTLE_STATUS = 20100401,				//检测战斗状态         

		INNER_SYNC_LOGIC_BATTLE_OVER = 20100402,				//地图服通知逻辑服战斗结果         

		INNER_OPEN_ROLE_PANEL = 20410000,				//获取玩家人物属性         

		INNER_SEND_BLESS_BUF = 20420000,				//发送公会祝福         

		INNER_STATUS_INSERT = 20100501,				//逻辑服插入buff接口         

		INNER_WAR_SCENE_ENTER = 20300100,				//进入战场         

		INNER_WAR_SCENE_SYNC_INFO = 20300101,				//同步战场信息         

		INNER_NOTIFY_SCENE_PASSIVE = 20100403,				//通知地图服被动技能         

		INNER_SYNC_MOUNT_RIDE = 20100040,				//同步坐骑骑乘         

		INNER_SYNC_TITLE_INFO = 20100600,				//传送称号数据         

		INNER_LOGIC_TO_SCENE_ROB_MATCH = 20600000,				//掠夺匹配         

		INNER_SCENE_TO_LOGIC_ROB_MATCH = 20600001,				//掠夺匹配         

		LOGIC_CHAT_BROADCAST_BUFF = 20100098,				//通知聊天服广播全服buff接口         

		INNER_RES_ROB_MATCH = 20600002,				//掠夺匹配返回         

		INNER_ASK_SCENE_VIP_CHANGE = 20200020,				//逻辑服通知场景服VIP信息改变         

		INNER_CHAT_LOGIC_WORLD_CHAT = 20200021,				//聊天服通知逻辑服进行了一次世界聊天         

		INNER_SYNC_GANG_WAR_INFO = 20170001,				//同步公会战公会信息         

		INNER_SYNC_GANG_WAR_PLAYER_INFO = 20170002,				//同步公会战玩家信息         

		INNER_SYNC_SECRET_SHOP = 20300200,				//同步神秘商店         

		INNER_REQ_DRAGON_VALE_ROB_FIGHT_LOAD = 20600003,				//掠夺战斗加载玩家数据         

		INNER_RES_DRAGON_VALE_ROB_FIGHT_LOAD = 20600004,				//掠夺战斗加载玩家数据(返回)         

		INNER_ASK_SCENE_CHARM_CHANGE = 20200022,				//逻辑服通知场景服魅力值改变         

		INNER_NOTIFY_LOGIC_SAVE_ROB_RESULT = 20600005,				//掠夺战斗结束，场景通知逻辑服保存离线掠夺者的战斗结果         

		INNER_NOTIFY_LOGIC_SAVE_TARGET_RESULT = 20600006,				//掠夺战斗结束，场景通知逻辑服保存离线被掠夺者的战斗结果         

		INNER_SYNC_GANG_WAR_RANK_INFO = 20170003,				//同步公会战排行榜数据         

		INNER_SYNC_ASK_GANG_WAR_RANK_INFO = 20170004,				//请求公会战排行榜数据         

		INNNER_ROB_ENTER_DRAGON_VALE = 20600007,				//掠夺者进入被掠夺者的跨服龙谷         

		INNNER_ROB_ENTER_DRAGON_VALE_RES = 20600008,				//掠夺者进入被掠夺者的跨服龙谷(返回)         

		INNER_GAIN_ROB_PRODUCTION = 20600009,				//掠夺者请求收获被掠者的跨服龙谷资源         

		INNER_RES_GAIN_ROB_PRODUCTION = 20600010,				//掠夺者请求收获被掠夺者的跨服龙谷资源(返回)         

		INNER_SYNC_GANG_WAR_RESULT = 20170005,				//同步公会战结果         

		INNER_NOTIFY_LOGIC_SAVE_TARGET_ROBBED_PRODUCTION = 20600011,				//掠夺资源结束，场景通知逻辑服保存离线被掠夺者的资源被掠夺通知         

		INNER_BORADCAST_DRAGON_VALE = 20600012,				//通知场景广播龙谷内的变化         

		INNER_PLAYER_EXIT_DRAGON_VALE = 20600013,				//场景通知逻辑服玩家退出龙谷         

		INNER_SYNC_GANG_WAR_STATE = 20170006,				//公会战活动状态         

		INNER_LOGIC_ADD_SKILL_POINT = 20100026,				//逻辑服加技能点         

		INNER_CAMPFIRE_SCENE_ENTER = 20300110,				//进入篝火活动         

		INNER_CAMPFIRE_SCENE_START = 20300111,				//篝火活动开始         

		INNER_CAMPFIRE_SCENE_SYNC_INFO = 20300112,				//同步篝火信息         

		INNER_DRAGON_VALE_ERROR_NOTICE = 20600014,				//龙谷场景错误提示         

		INNER_DRAGON_VALE_MONSTER = 20600015,				//龙谷刷怪         

		INNER_SYNC_GANG_WAR_REWARD = 20170007,				//同步公会战奖励         

		INNER_SYNC_GANG_WAR_HELP = 20170008,				//同步公会战协助请求         

		INNER_CLEAR_DRAGON_VALE_MONSTER = 20600016,				//龙谷清理怪         

		INNER_SYNC_GANG_WAR_AWARD_SELF = 20170009,				//同步公会战奖励         

		INNER_SYNC_GANG_WAR_DUEL_REQUEST = 20170010,				//公会战请求决斗         

		INNER_SAVE_NOTICE_BOARD_DRIVE_ROBBER = 20600017,				//通知逻辑服记录被掠夺者的布告栏，已经将掠夺者赶跑         

		INNER_SAVE_NOTICE_BOARD_ROB_PRODUCTION = 20600018,				//通知逻辑服记录被掠夺者的布告栏，被掠夺了多少资源         

		INNER_ENTER_ROB_PROTECT_TIME = 20600019,				//通知逻辑服被掠夺者进入掠夺保护时间         

		INNER_ROB_MATCH_FAIL_BACK_COIN = 20600020,				//掠夺匹配失败，返还金币         

		INNER_CLEAR_DRAGON_VALE_ROB_PROTECT_STATUS = 20600021,				//龙谷被掠夺者战斗胜利，通知逻辑服清除掠夺保护状态         

		INNER_CLEAR_ROB_PRODUCTION_PROCESS = 20600022,				//通知逻辑服，清除掠夺资源的进度         

		INNNER_NOTIFY_TOD_ROB_PRODUCTION_FINISH = 20600023,				//通知逻辑服，记录TOD助手，掠夺者收获掠夺资源完成         

		INNER_SET_FIRST_ENTER_AFTER_ROBBED = 20600024,				//通知逻辑服，设置龙谷主人被掠夺资源后第一次进入龙谷的标志         

		INNER_STRING_INFO_CHANGE = 20100050,				//内部通知字符串信息改变更新         

		INNER_ACTIVITY_RESULT = 20200201,				//活动结果通知         

		INNER_SYNC_STATUS_DEL = 20100502,				//逻辑服删除状态接口         

		INNER_ASK_SCENE_HEAD_ID_CHANGE = 20200023,				//逻辑服同步头像到场景服         

		INNER_INT_INFO_CHANGE = 20100051,				//内部通知整型信息改变更新         

		INNER_SYNC_STATUS_HERO_INSERT = 20100503,				//英雄添加状态         

		INNER_ROB_DRAGON_VALE_TASK_LISTEN = 20600025,				//通知逻辑服，掠夺任务监听         

		INNER_SUCCESS_PROTECT_DRAGON_VALE = 20600026,				//被掠夺者战斗胜利，通知被掠夺者的服务器全服广播         

		INNER_SYNC_TEAM_SPORTS = 20100306,				//组队竞技传参         

		INNER_ARENA_SYNC_RANK_INFO = 20300032,				//竞技场同步排名相关信息         

		INNER_ARENA_FETCH_FIGHT_DATA = 20300033,				//竞技场请求战斗数据         

		INNER_ARENA_MATCH_PLAYER = 20300034,				//竞技场请求匹配跨服玩家         

		INNER_ARENA_FETCH_RANK_INFO = 20300035,				//竞技场请求跨服排行信息         

		INNER_ARENA_SYNC_INNER_RANK = 20300036,				//竞技场同步跨服排名         

		INNER_SYNC_AVATAR = 20100060,				//同步avatar时装到场景服         

		INNER_AFTER_LEARN_SKILL = 20100017,				//同步场景服学习技能到逻辑服         

		INNER_KNIGHT_TRIALS_MATCH_PLAYER = 20610000,				//骑士试炼请求匹配跨服玩家         

		INNER_ARENA_PLAYER_DATA_COMPLETE = 20300037,				//竞技场战斗数据同步         

		INNER_ARENA_BATTLE_RESULT = 20300038,				//竞技场战斗结果同步跨服         

		INNER_SYNC_STATUS_HERO = 20100504,				//同步英雄状态属性         

		INNER_SYNC_FORCE_TO_LOGIC = 20100027,				//同步战力到逻辑服         

		INNER_KNIGHT_TRIALS_ENTER_FIGHTER = 20610001,				//骑士试炼通知场景进入战斗         

		INNER_ARENA_KING_TIME_CHANGE = 20300039,				//竞技场王者名称参数变化通知         

		INNER_ARENA_SEASON_FINISH = 20300040,				//竞技场赛季结束通知         

		INNNER_ROBBER_WIN_IN_ROB_FIGHT = 20600027,				//掠夺者在掠夺战斗中胜利，通知逻辑服准备相关数据         

		INNER_EXPEDITION_DATA_CHANNEL = 20610010,				//内部远征逻辑服于场景服之间的数据传输通道         

		INNER_EXPEDITION_NOTE_LOGIC_PLAYER_INFO = 20610011,				//远征系统玩家退出场景通知玩家记录最新信息         

		INNER_REFRESH_ROLE_PANEL_PROP = 20410001,				//刷新人物面板属性         

		INNER_SYNC_PLAYER_NAME = 20100028,				//同步玩家角色名         

		INNER_ROBBER_GAIN_TOTAL_PRODUCTION = 20600028,				//通知逻辑服，掠夺者总共掠夺了多少资源         

		INNER_ROBBER_BEGIN_ATTCK_BATTLE = 20600029,				//通知逻辑服，掠夺者开始进攻         

		INNER_SYNC_TEAM_CUMULATE_AWARD = 20100307,				//组队副本、英雄梦镜         

		INNER_ASYNC_STATUS = 20100100,				//异步状态操作         

		INNER_REQ_OPEN_PANEL = 20100200,				//内部打开人物面板         

		INNER_ASK_PLAYER_FIGHT_STATUS = 20100308,				//内部查询战斗状态         

		INNER_CHECK_INFO_TO_CENTER_SERVER = 20999999,				//检测信息传到中央服务器         

		INNER_ROBBED_MASTER_CAN_ENTER_DRAGON_VALE = 20600030,				//通知被掠夺者可以进入龙谷了         

		INNER_SEND_VIP_INFO_TO_CHAT = 20200024,				//通知聊天服VIP信息改变         

		INNER_SYNC_CHALLENGE_TWO_PLAYER_IFNO_TO_LOGIC = 20100070,				//同步擂台双方玩家到逻辑服         

		INNER_ANSWER_DATA_CHANNEL = 20610020,				//内部智慧试练逻辑与场景间数据通道         

		INNER_HERO_TMP_OUT_FIGHT = 20400009,				//场景服请求临时英雄         

		INNER_GANG_WAR_PRODUCE_MATCHED_INFO = 20170011,				//公会战产生16名单并随机匹配         

		INNER_SEND_LEVEL_INFO_TO_CHAT = 20200025,				//通知聊天服等级改变         

		CHAT_GET_FILTER_LIST = 20200026,				//聊天黑/白名单         

		INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE = 20600031,				//被掠夺者逻辑服/掠夺者龙谷场景服/被掠夺者主城场景服处理掠夺         

		INNER_NOTIFY_MASTER_OF_DRAGON_FIGHT_END = 20600032,				//通知被掠夺者战斗后的结果         

		INNER_DRAONG_VALE_NO_ENTER = 20600033,				//龙谷正在被掠夺，不可进入         

		INNER_TARGET_IN_DEFENCE_DRAGON_VALE_FIGHT = 20600034,				//开始/结束被入侵龙谷的战斗中         

		INNER_ACT_SCENE_SYNC_INFO = 20300300,				//同步活动场景信息         

		INNER_ACT_SCENE_PLAYER_ENTER = 20300301,				//同步活动场景玩家进入         

		INNER_ACT_SCENE_START = 20300302,				//活动场景开启         

		INNER_ACT_SCENE_END = 20300303,				//同步活动场景结束         

		INNER_BATTLE_END_TO_LOGIC = 20200202,				//战斗结束通知逻辑服         

		INNER_EXPEDITION_PLAYER = 20610012,				//远征玩家数据通道         

		INNER_NOTICE_ENTER_SCENE = 20100013,				//通知逻辑服玩家进入场景         

		INNER_SYNC_TEAM_INTEGRATE = 20100309,				//同步组队竞技积分         

		INNER_GET_TEAM_SCORE = 20100310,				//获取组队积分榜         

		INNER_SYNC_TEAM_RANK = 20100312,				//同步组队竞技排名         

		INNER_LEARN_SKILL_COST_ITEM = 20100016,				//同步逻辑学习技能扣除物品         

		INNER_ASYNC_BE_SKILL_CALL_ACTIVE_SKILL = 20100101,				//异常操作被动技能调用主动技能         

		INNER_SYNC_PLAYER_GENDER = 20100029,				//同步性别到地图         

		SYSN_ENERGY_TO_LOGIC = 20100031,				//同步精力值到逻辑         

		SYNS_LIFE_SKILL_TO_SCENE = 20100032,				//同步生活技能收集技能到场景         

		INNER_SYNC_SCENE_PUBLIC = 20020001,				//同步公共信息         

		INNER_SYNC_SCENE_OUTPOST = 20300400,				//内部同步关卡信息         

		INNER_LOGIC_ADD_ITEM_TO_PLAYER = 20300113,				//通知逻辑服领取篝火奖励         

		INNER_RES_LOGIC_ADD_ITEM_TO_PLAYER = 20300114,				//通知逻辑服领取篝火奖励(返回)         

		INNER_REQ_SYNC_CAMPFIRE_PLAYER_INFO = 20300115,				//请求读取玩家篝火信息         

		INNER_RES_SYNC_CAMPFIRE_PLAYER_INFO = 20300116,				//请求同步玩家篝火信息(返回)         

		INNER_REQ_MAIL_CAMPFIRE_AWARD = 20300117,				//请求邮件结算玩家篝火奖励         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_INNER_H_ENUM_ */
