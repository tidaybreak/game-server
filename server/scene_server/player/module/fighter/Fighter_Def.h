/*
 * Fighter_Def.h
 *
 *  Created on: Mar 18, 2014
 *      Author: ti
 */

#ifndef FIGHTER_DEF_H_
#define FIGHTER_DEF_H_

class Fighter;

struct Attack_Param;

#define MAX_LEVEL		80

#define MAX_PHY_POWER 200

#define PHY_POWER_DAILY_RECOVER 100

#define PHY_POWER_RECOVER_VALUE 5

#define PHY_POWER_RECOVER_TIME_VALUE 1800


typedef void (*Attack_Calc_Func)(const Attack_Param &attack_param, Fighter &attacker, Fighter &peer, double &attack);

enum Prop_Change_Source {
	SOURCE_DEFAULT = 1001,
	SOURCE_HURT_SKILL = 1002,
	SOURCE_HURT_BUFF = 1003,
	SOURCE_HURT_STATUS = 1004,
};

enum Prop_Change_Effect {
	EFFECT_SHOW_ALL		= 1,	/// 是否飘字 1 显示 0不显示,
	EFFECT_SHOW_NO		= 2,	/// 是否飘字 0不显示,
	EFFECT_SHOW_SELF	= 3,	/// 2战斗时改变
};

enum Battle_Status {
	READY			= 0,
	BATTLING		= 2,	/// 战斗中
	DIE_OVER		= 3,	/// 死亡结束
};

enum Target_Select_Type {
	TARGET_EXIST = 1,		// 已经存在的目标
	TARGET_SELF	 = 2,		// 手动
	TARGET_AUTO	 = 3,		// 自动
	TARGET_BUFF	 = 4,		// buff目标
	TARGET_TEMP  = 5,		// 溅射目标
};

enum Target_Rela_Type {
	TYPE_ENEMY		= 1,			// 敌方
	TYPE_PARTNER	= 2,			// 己方
	TYPE_FRIEND		= 3,			// 友方
};

enum Skill_Action_Effect {
	EFFECT_PLAY_EFFECT		= 10001,			// 攻击 d_param1:攻击的百分比,附加d_param2固定伤害
	EFFECT_ATTACK_DEFAULT 	= 10002,			// 攻击 d_param1:攻击的百分比,附加d_param2固定伤害
	EFFECT_ATTACK_BUFF_SELF = 10003,			// 给自己加buff
	EFFECT_ATTACK_BUFF_OPP	= 10004, 			// 给技能目标加buff
	EFFECT_CURE				= 10005,			// 给目标治疗
	EFFECT_CALL_MONSTER		= 10006, 			// 召唤怪物
	EFFECT_PURE				= 10007,			// 净化
	EFFECT_DISPERSE			= 10008,			// 驱散
	EFFECT_SUB_ENEMY_MORALE = 10009,			// 减敌方怒气
	EFFECT_T_CALL_MONSTER	= 10010,			// 怪物变身召唤怪
	EFFECT_DESPERSE_ONE		= 10011,			// 驱散一个负面效果
	EFFECT_BUFF_SWITCH		= 10012,			// add/sub buff
	EFFECT_FLOAT_OVER		= 10020,			// 浮空技追加结束
};

enum Skill_Type {
	TYPE_NORMAL_SKILL 		= 1,				// 普通技
	TYPE_COMMON_SKILL 		= 2,				// 常规技
	TYPE_ADD_SKILL			= 3,				// 追加技
	TYPE_FLOAT_SKILL		= 4,				// 浮空技
	TYPE_CONT_SKILL			= 5,				// 连击技
	TYPE_UNIQUE_SKILL		= 6, 				// 必杀技
};

enum Attack_Extra_Bit {
	MUST_HIT = 0x01,
	MUST_CRIT = 0x02,
	IGNORE_DEF = 0x04,
	PERCENT_CRIT = 0x08,
	DONT_CRIT = 0x10,
};

enum Attack_Result {
	ATTACK_CRIT	= 1,	// 暴击
	ATTACK_AVOID,		// 闪避
	ATTACK_NORMAL,		// 正常
	ATTACK_BUFF,		// BUFF
	ATTACK_CURE, 		// 技能治疗
	ATTACK_BLD_COST,	// 消耗血使用技能
	ATTACK_POINTION_BUFF,//中毒伤血
};



/// 属性类型
enum Property_Type {
	PT_START  					= 500001,
	PT_CAREER 					= 500201,	/// 职业
	PT_GENDER                   = 500301,	/// 性别
	PT_LEVEL 					= 502403,	/// 等级
	PT_VIP_LEVEL				= 503303,	/// vip等级
	PT_EXP_CURRENT 				= 502503,	/// 当前经验
	PT_SKILL_POINT				= 502703,	/// 技能点
	PT_EXP_LEVEL_UP				= 503203,	/// 升级需要经验
	PT_CHARM					= 503503,	/// 魅力
	PT_ENERGY                   = 503504,	/// 精力

	//////////////////////////////////////////////////////////

	PT_FIGHT_PROP_START			= 504504,
	PT_FORCE 					= 504504,	/// 战力
	PT_POWER					= 504605, 	/// 斗力
	PT_AGILITY					= 504705,	/// 敏捷
	PT_DEXTERITY				= 504805,	/// 灵巧
	PT_ARMOR					= 504905,	/// 护甲
	PT_STAMINA					= 505005,	/// 耐力

	PT_PHY_ATTACK 				= 505605,	/// 物攻
	PT_SPELL_ATTACK				= 505705,	/// 法攻
	PT_PHY_DEFENSE				= 505805,	/// 物防
	PT_SPELL_DEFENSE			= 505905,	/// 法防
	PT_HIT						= 506005,	/// 命中
	PT_AVOID					= 506105,	/// 闪避
	PT_IGNORE_DEF				= 517007,	/// 无视防御
	PT_BLOOD_MAX				= 506205,	/// 生命
	PT_CURE						= 508506,	/// 治疗
	PT_CURE_RATE				= 514106,	/// 最终治疗乘法修正
	PT_SING_RATE				= 507905,	/// 吟唱修正
	PT_CRIT						= 506305,	/// 暴击
	PT_CRIT_DEF					= 506405,	/// 暴防
	PT_INIT_MORALE				= 506505,	/// 初始怒气
	PT_MORALE_CAPS				= 506605,	/// 士气值上限
	PT_CRIT_HURT				= 506705,	/// 暴击伤害加成1.5
	PT_CRIT_HURT_SUB			= 506706,	/// 暴击伤害减免
	PT_HIT_RATE_ADDITION 		= 506805,	/// 命中率加成
	PT_AVOID_RATE_ADDITION 		= 506905,	/// 闪避率加成
	PT_CRIT_RATE_ADDITION 		= 507005,	/// 暴击率加成
	PT_CRIT_DEF_RATE_ADDITION 	= 507105,	/// 暴击抵抗率加成
	PT_SPEED 					= 507205,	/// 移动速度
	PT_BLOOD_CURRENT 			= 507305,	/// 当前生命值
	PT_MORALE_CURRENT 			= 507405,	/// 当前怒气
	PT_STRENGTH_CURRENT			= 507505,	/// 当前体力
	PT_STRENGTH_MAX				= 507605,	/// 体力上限
	PT_BLOOD_POOL_CURRENT		= 507705,	/// 当前血池
	PT_BLOOD_POOL_MAX			= 507805,	/// 最大血池

	PT_IGNORE_DEF_RATE			= 508005,	/// 无视防御概率
	PT_STATUS_HIT_RATE			= 516207,	/// 状态命中率
	PT_STATUS_DEF_RATE			= 516307,	/// 状态命中率
	PT_BLOOD_STEAL				= 516507,	/// 吸血

	PT_PHY_HURT_FIX				= 513406,	/// 物理伤害加法修正
	PT_PHY_HURT_RATE			= 513506,	/// 物理伤害乘法修正
	PT_SPELL_HURT_FIX			= 513606,	/// 法术伤害加法修正
	PT_SPELL_HURT_RATE		= 513706,	/// 法术伤害乘法修正
	PT_HURT_FIX						= 513806,	/// 最终伤害加法修正
	PT_HURT_RATE					= 514006, 	/// 最终伤害修正
	PT_BE_HURT_FIX				= 514506,	/// 受到的伤害加法修正
	PT_BE_HURT_RATE				= 514406,	/// 受到的伤害乘法修正

	PT_PK_HURT_ADD					= 517207,	/// PK伤害加成
	PT_PK_HURT_SUB					= 517307,	/// PK伤害减免
	PT_PHY_HURT_RATE_SUB		= 514206,	/// 受到的物理伤害乘法修正
	PT_SPELL_HURT_RATE_SUB	= 514306,	/// 受到的法术伤害乘法修正
	PT_SKILL_HURT_RATE			= 514606,	/// 技能伤害修正

	PT_HUMAN_ADD				= 518007,	/// 人族伤害加成
	PT_ELF_ADD						= 518107,	/// 精灵族伤害加成
	PT_ORC_ADD						= 518207,	/// 兽族伤害加成
	PT_DRAGON_ADD				= 518307,	/// 龙族伤害加成
	PT_UNDEAD_ADD				= 518407,	/// 亡灵族伤害加成
	PT_DEMON_ADD					= 518507,	/// 恶魔族伤害加成
	PT_HUMAN_SUB				= 518008,	/// 人族伤害减免
	PT_ELF_SUB						= 518108,	/// 精灵族伤害减免
	PT_ORC_SUB						= 518208,	/// 兽族伤害减免
	PT_DRAGON_SUB				= 518308,	/// 龙族伤害减免
	PT_UNDEAD_SUB				= 518408,	/// 亡灵族伤害减免
	PT_DEMON_SUB					= 518508,	/// 恶魔族伤害减免

	PT_SUMMON_ADD				= 519117,  /// 召唤物伤害加成
	PT_SUMMON_SUB				= 519118,  /// 召唤物伤害减免

	PT_BE_CURE_RATE				= 519007,	/// 被治疗加成
	PT_EFFECT_RATE				= 519107,	/// 效果生效修正概率

	PT_PVE_HURT_ADD			= 514706,	/// PVE伤害加成
	PT_PVE_HURT_SUB			= 514806,	/// PVE伤害减免

	PT_DIZZ_STRENGTHEN		= 514906,	/// 晕眩强化
	PT_DIZZ_WEAKEN				= 515006, 	/// 晕眩弱化

	PT_FIGHT_PROP_END			= 530000,

	//////////////////////////////////////////////////////////
};

// 附加属性(顺序不可调)
enum Addition_Type {
    AT_BASIC       = 0,
    AT_EQUIP,              			// 装备(特殊)
    AT_CONVERSION,         	// 一二级属性转换所得加成(特殊, 一级无加成)
    AT_INITIATIVE_SKILL,   	// 主动技能(特殊, 战斗时有效,最后乘,加)
    AT_STATUS,             		// 状态
    AT_PASSIVE_SKILL,        // 被动技能
    AT_RUNE_STONE,			// 符石
//    AT_RUNE,						// 符文
    AT_HERO_ASSIST,			// 英雄助阵
    AT_GANG,						// 公会
    AT_ARTIFACTS,				// 神器
    AT_ACHIEVEMENT,		// 成就
    AT_TITLE,						// 称号
    AT_DRAGON_VALE,			//龙谷系统
    AT_ANCI_ARTIFACT,		// 上古神器
    AT_WING,						// wing
    AT_AWAKE,						// 觉醒
    AT_MOUNT,						// 坐骑
    AT_EXPEDITION,				// 远征
    AT_PET,							// pet
    AT_END
};

// 操作类型
enum Operate_Type {
	O_ADD,
	O_SUB,
	O_SET,
	OPERATE_TYPE_END
};

enum Prop_Setter_Type {
	PST_BASIC,
	PST_FIXED,
	PST_PERCENT,
	PST_END
};

enum Battle_Cost_Type {
	COST_MORALE 		= 1,
	COST_MORALE_PERCENT,
	COST_BUFF_SELF,
	COST_BUFF_ENEMY,
	COST_BLD,
	COST_BLD_PERCENT,
	COST_END
};

enum Morale_Add_Type {
	TYPE_SKILL_END_ADD = 10,
	TYPE_SKILL_HIT_ADD,
	TYPE_SKILL_CRIT_ADD,
	TYPE_SKILL_RETURN,
};

enum Select_Target_Priority_Type {
	PRIORITY_LEAST_BLOOD	= 1,
	PRIORITY_RANDOM			= 2,
	PRIORITY_BLOOD_PERCENT  = 3,
	PRIORITY_NGE_BUFF		= 4,
	PRIORITY_END,
};

enum Select_Target_Priority_Row {
	PRIORITY_FORE_ROW		= 1,		// 前排
	PRIORITY_REAR_ROW		= 2,		// 后排
};

enum Select_Target_Priority_Col {
	PRIORITY_UP_COL			= 1,		// 上排
	PRIORITY_DOWN_COL		= 2,		// 下排
	PRIORITY_SELF_COL		= 3,		// 攻击者当前排
	PRIORITY_RANDOM_COL		= 4,		// 随机列
};

enum Select_Role_Type {
	SELECT_TYPE_PLAYER		= 1,		// 主角
	SELECT_TYPE_HERO		= 2,		// 英雄
	SELECT_TYPE_MONSTER		= 3,		// 小怪
	SELECT_TYPE_BOSS		= 4,		// BOSS
};

enum Step_Fix_Factor {
	FCT_BLOOD_VALUE		= 1,		/// 1.固定血量
	FCT_BLOOD_PERCENT	= 2,		/// 2.血量百分比
	FCT_MORAL_VALUE		= 3,		/// 3.固定怒气
	FCT_MORAL_PERCENT	= 4,		///	4.怒气百分比
	FCT_BUFF_ID			= 5,		/// buff ID
	FCT_ENEMY_BLD_VAL	= 6,		/// 敌方血量值
	FCT_ENEMY_BLD_PER	= 7,		/// 敌方血量百分比
	FCT_STEP_MORALE		= 8,		/// 阶梯怒气
	FCT_END,						/// 大于1000的值特殊处理为buff层次计算
};

enum Step_Fix_Type {
	TYPE_TARGET_NUM		= 1,	/// 1.攻击数量
	TYPE_EFFECT_NUM		= 2,	/// 2.增加效果（数量）
	TYPE_HURT_RATE		= 3,	/// 3.伤害修正
	TYPE_EFFECT_TRIGGER = 4,	/// 4.修正效果触发概率
	TYPE_CRIT_MORAL		= 5,	/// 5.暴击加buff
	TYPE_END,
};

enum Skill_Sort {
	ACT_SKILL		= 1,		/// 主动技能
	BE_SKILL		= 2,		/// 被动技能
};

enum Battle_Err_Code {
	BTL_ERR_SUCCESS = 0,		/// 成功使用技能
	BTL_ERR_PRE_USE	= 1,		/// 预使用技能
	BTL_ADD_FAIL	= 2,		/// 使用追加技能失败
	BTL_NOT_ADD		= 3,		/// 非追加技能
};

enum Battle_Plot {
	PLOT_PAUSE = 1,
	PLOT_CONTINUE = 2,
};

enum Cost_Target {
	COST_SELF		= 1,
	COST_ENEMY		= 2,
};

enum Be_Skill_Add_Moral {
	ADD_TYPE_HIT	= 1,
	ADD_TYPE_CRIT	= 2,
};

enum CURE_TYPE {
	CURE_BY_VALUE	= 1,
	CURE_BY_PERCENT	= 2,
};

enum Notify_Passive_Type {
	NPT_ADD		= 1,
	NPT_REMOVE	= 2,
};

enum Be_Skill_Trigger_Type {
	TYPE_DYING		= 1,
	TYPE_BE_ATTACKED = 2,
	TYPE_BLOOD_VAL	= 3,
	TYPE_MORALE_VAL = 4,
	TYPE_NEG_BUFF = 5,
};

enum Skill_Aff_Type {

	TYPE_ROLE = 1,
	TYPE_HERO = 2,
};

#endif /* FIGHTER_DEF_H_ */
