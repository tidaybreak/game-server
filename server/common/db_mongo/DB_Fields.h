/*
 * DB_Fields.h
 *
 *  Created on: May 8, 2012
 *      Author: Ti
 */

#ifndef DB_FIELDS_H_
#define DB_FIELDS_H_

#include <string>

struct Global_Fields {
    static const std::string COLLECTION;

    static const std::string ID;
    static const std::string KEY;
    static const std::string ROLE;
    static const std::string TIME;
};

struct Role_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;

	static const std::string ACCOUNT_INFO;
    struct Account_Info {
		static const std::string ACCOUNT_NAME;
		static const std::string AGENT_NUM;
		static const std::string SERVER_NUM;
    };

    static const std::string LOCAL_HIS;

	static const std::string LOCATION;
    struct Location {
        static const std::string SCENE_ID;
        static const std::string SCENE_SORT;
        static const std::string SCENE_SUB_SORT;
        static const std::string SCENE_ORDER;
        static const std::string SCENE_LAYER;
        static const std::string COORD_X;
        static const std::string COORD_Y;
        static const std::string COORD_TYPE;
        static const std::string TOWARD;
    };

	static const std::string ROLE_NAME;
	static const std::string LEVEL;
	static const std::string GENDER; /// man 1, 0 female, you konw ...
	static const std::string CAREER;
	static const std::string VIP;
	static const std::string VIP_OVER_TIME;
	static const std::string CREATE_TIME;
	static const std::string LAST_SIGN_IN_TIME;
	static const std::string LAST_SIGN_OUT_TIME;
	static const std::string LAST_TRIGGER_DAILY_ZERO;
	static const std::string LAST_TRIGGER_DAILY_SIX;
	static const std::string LAST_TRIGGER_WEEKLY_ZERO;
	static const std::string RIDE;
	static const std::string MOUNT_MODEL_ID;
	static const std::string TITLE_ID;
	static const std::string ONLINE;
	static const std::string IP;
	static const std::string HEAD_ID;
	static const std::string CHARM;
	static const std::string GANG_ID;
	static const std::string GANG_NAME;
	static const std::string AWAKE_LVL;
	static const std::string AVATAR;
	static const std::string MOUNT_ID;
	static const std::string ROLE_TYPE;

};

struct Example_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
};

struct Icon_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string ICON;
	struct Icon {
		static const std::string ICON_ID;
		static const std::string STATUS;
		static const std::string IS_EVER_OPENED;
		static const std::string IS_USED_ITEM;
	};
};

struct Dragon_Vale_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string TARGET_PLAYER_ID;
	static const std::string IS_ROB_ENTER_DRAGON_VALE;
	static const std::string UUID;
	static const std::string IS_GOLD_MINE_MAX_STORAGE;
	static const std::string IS_MOON_WELL_MAX_STORAGE;
	static const std::string IS_TOTEM_MAX_STORAGE;
	static const std::string BUILD_GOLD_MINE_NUM;
	static const std::string BUILD_MOON_WELL_NUM;
	static const std::string BUILD_TOTEM_NUM;
	static const std::string BUILD_QUEUE_NUM;
	static const std::string ALL_OPEN_BUILD_QUEUE_NUM;
	static const std::string IS_DRAGON_BORN;
	static const std::string BEGIN_BORN_TIME_SEC;
	static const std::string BEGIN_PLAY_DRAGON_INTERVAL_TIME_SEC;
	static const std::string BEGIN_DOWN_FEELING_TIME_SEC;
	static const std::string MATCHED_NUM;
	static const std::string IS_VIP_DOWN_TO_ZERO;
	static const std::string BEGIN_REFRESH_THIEF_TIME_SEC;
	static const std::string STAR_NUM;
	static const std::string AWARD_FOOT_NUM;
	static const std::string IS_SPECIAL;
	static const std::string IS_YESTERDAY_LOGIN;
	static const std::string DRAGON_HEART_NUM;
	static const std::string IS_ROB_PROTECT;
	static const std::string BEGIN_ROB_PROTECT_TIME_SEC;
	static const std::string IS_FIRST_ENTER_AFTER_ROBBED;
	static const std::string IS_IN_ROBBED_DRAGON_VALE;
	static const std::string BEGIN_IN_ROBBED_DRAGON_VALE_TIME_SEC;
	static const std::string IS_DRAGON_VALE_OPEN;
	static const std::string BEGIN_NO_ENTER_TIME;
	static const std::string LEFT_TIME;
	static const std::string PLAY_DRAGON_NUM;
	static const std::string DRIVE_THIEF_NUM;

	static const std::string BUILDING_LIST;
	struct Building {
		static const std::string ID;
		static const std::string TYPE;
		static const std::string LEVEL;
		static const std::string STATUS;
		static const std::string PRODUCTION;
		static const std::string IS_GAIN;
		static const std::string BEGIN_UPGRADE_TIME_SEC;
		static const std::string BEGIN_UPGRADE_TIME_USEC;
		static const std::string LAST_OUTPUT_PRODUCTION_TIME_SEC;
		static const std::string LAST_OUTPUT_PRODUCTION_TIME_USEC;
		static const std::string ORDER;
		static const std::string CONSUME_TYPE;
	};

	static const std::string CANCEL_CREATE_BUILDING_LIST;
	struct Cancel_Create_Building {
		static const std::string ORDER;
	};

	static const std::string DRAGON_LIST;
	struct Dragon {
		static const std::string PROGRESS;
		static const std::string FEELING;
		static const std::string POWER;
		static const std::string ARMOR;
		static const std::string STAMINA;
		static const std::string POWER_LEVEL;
		static const std::string ARMOR_LEVEL;
		static const std::string STAMINA_LEVEL;
		static const std::string HERO_POWER;
		static const std::string HERO_ARMOR;
		static const std::string HERO_STAMINA;
		static const std::string HERO_POWER_LEVEL;
		static const std::string HERO_ARMOR_LEVEL;
		static const std::string HERO_STAMINA_LEVEL;
		static const std::string IS_PLAY_DRAGON;
		static const std::string ENERGY;
		static const std::string CRIT;
		static const std::string PREVENT_CRIT;
		static const std::string CRIT_LEVEL;
		static const std::string PREVENT_CRIT_LEVEL;
		static const std::string HERO_CRIT;
		static const std::string HERO_PREVENT_CRIT;
		static const std::string HERO_CRIT_LEVEL;
		static const std::string HERO_PREVETN_CRIT_LEVEL;
	};

	static const std::string THIEF_LIST;
	struct Thief {
		static const std::string CREATER;
		static const std::string ID;
	};

	static const std::string FOOT_INFO_LIST;
	struct Foot_Info {
		static const std::string ID;
		static const std::string SEX;
		static const std::string NAME;
	};

	static const std::string FOOT_ID_LIST;
	struct Foot_ID {
		static const std::string ID;
	};

	static const std::string NOTICE_BOARD_LIST;
	struct Notice_Board {
		static const std::string TYPE;
		static const std::string TIME_SEC;
		static const std::string HOST;
		static const std::string TARGET;
		static const std::string DRAGON_SOUL;
		static const std::string COIN;
		static const std::string SOUL;
		static const std::string IS_GET_DRAGON_HEART;
		static const std::string PARAM1;
	};
};

struct Achievement_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string ACHIEVEMENT_COUNT;
	static const std::string ACHIEVEMENT_COMPLETE_IDS;
	static const std::string ACHIEVEMENT_SUIT_PURPLE_IDS;
	static const std::string ACHIEVEMENT_SUIT_GOLD_IDS;
	static const std::string ACHIEVEMENT_RUNE_LVL_HIS;
	static const std::string ACHIEVEMENT_UNBAN_LVL_HIS;
	static const std::string ACHIEVEMENT_MASTER_LVL_HIS;
	static const std::string ACHIEVEMENT_FORCE_LVL_HIS;
	static const std::string ACHIEVEMENT_WAR_KILL_NUMS_HIS;
	static const std::string ACHIEVEMENT_ARENA_WIN_NUMS_HIS;
	static const std::string ACHIEVEMENT_GANG_WAR_WIN_NUMS_HIS;
//	static const std::string ACHIEVEMENT_FIRST_LVL_ROLE_ID_HIS;
//	static const std::string ACHIEVEMENT_FIRST_LVL_CK_HIS;
//	static const std::string ACHIEVEMENT_FIRST_LVL_ZS_HIS;
//	static const std::string ACHIEVEMENT_FIRST_LVL_YSS_HIS;
	static const std::string ACHIEVEMENT_KILL_MONSTER_NUMS_HIS;
	static const std::string ACHIEVEMENT_FB_COMPLETE_NUMS_HIS;
	static const std::string ACHIEVEMENT_HORSE_GET_HIS;
	static const std::string ACHIEVEMENT_SECRET_SHOP_BUY_NUMS_HIS;
	static const std::string ACHIEVEMENT_DRAGON_WALE_ROB_TIMES_HIS;
	static const std::string ACHIEVEMENT_DRAGON_WALE_STEP_FOOT_TIMES_HIS;
	static const std::string ACHIEVEMENT_DRAGON_WALE_PLAY_DRAGON_TIMES_HIS;
	static const std::string ACHIEVEMENT_HERO_AWAKE_LVL_HIS;
	static const std::string ACHIEVEMENT_TASK_COMPLETE_NUMS_HIS;
	static const std::string ACHIEVEMENT_DRAGON_WALE_GOLD_MINE_LVL_HIS;
	static const std::string ACHIEVEMENT_DRAGON_WALE_MOON_WELL_LVL_HIS;
	static const std::string ACHIEVEMENT_DRAGON_WALE_TOTEM_LVL_HIS;
	static const std::string ACHIEVEMENT_DRAGON_WALE_DRIVE_THIEF_TIMES_HIS;
	static const std::string ACHIEVEMENT_WAR_LIGHT_WIN_NUMS_HIS;
	static const std::string ACHIEVEMENT_WAR_DRAK_WIN_NUMS_HIS;
	static const std::string ACHIEVEMENT_TASK_LIMIT_TIME_HIS;
	static const std::string ACHIEVEMENT_KILL_BOSS_HIS;
	static const std::string ACHIEVEMENT_DRAGON_WALE_KEEP_LEVEL_HIS;
	static const std::string ACHIEVEMENT_RUNE_STONE_LVL_HIS;
	static const std::string ACHIEVEMENT_ARENA_LVL_HIS;
	static const std::string ACHIEVEMENT_FIRST_BOSS_AGAINST;
	static const std::string ACHIEVEMENT_COMPLETE_SUIT;

	static const std::string ACHIEVEMENT_INFO_MAP;
	struct Achievement_Info {
		static const std::string ID;
		static const std::string DATA;
		static const std::string STATE_COUNT;
		static const std::string STATE_AWARD;
	};
};

struct Title_Fields {
    static const std::string COLLECTION;
    static const std::string ROLE_ID;

    static const std::string TITLE_INFO;
    struct Title_Info {
    	static const std::string ID;
    	static const std::string LAST_TIME;
    	static const std::string START_TIME;
    	static const std::string LAST_END_TIME;
    };
    static const std::string EQUIP_TITLE;
};

struct Equiper_Fields {
    static const std::string COLLECTION;
    static const std::string ROLE_ID;
    static const std::string GEM_ID;
    static const std::string GEM_ENABLE_INFO;
    static const std::string MASTER_PART_INFO;
	static const std::string CARVE_LVL;//雕刻等级
	static const std::string GEM_PROGRESS;//雕刻进度

    struct Master_Part_Info_Detail {
    	static const std::string INDEX;/*部位索引主角10001-10008 英雄1-8*/
    	static const std::string LEVEL;/*等级*/
    	static const std::string LUCK_VALUE;/*当前幸运值*/
    	static const std::string FAIL_CNT;//失败次数
    };
};

struct Title_Record_Fields {
	static const std::string COLLECTION;
	static const std::string GANG_WAR_FIRST_LEADER;

	static const std::string CHARTS_TITLE_MAP;

	static const std::string CHARTS_INFO;
	static const std::string ROLE_ID;
	static const std::string CHARTS_TYPE;
	static const std::string TITLE_ID;
};

struct Exchange_Detail_Fields {
	static const std::string COLLECTION;
	static const std::string EX_DATA;
	static const std::string EX_DATA_EDAY;
	static const std::string KEY;
	static const std::string VALUE;
};

struct Public_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string PLOT_INFO;
	static const std::string PLOT_ID;
	static const std::string PLOT_NUM;
	static const std::string PLOT_STEP;
	static const std::string PLOT_SWITCH;
	static const std::string ADD_UP_ONLINE_TIME;
	static const std::string FCM;
	static const std::string WAR_ENTER_NUM;
	static const std::string WAR_MERIT;
	static const std::string MERIT;
	static const std::string WAR_FORBID_TIME;
	static const std::string ADD_CAMPFIRE;
	static const std::string CAMPFIRE_ID;
	static const std::string CAMPFIRE_FINISH_TIME;
	static const std::string CAMPFIRE_RANK;
	static const std::string CAMPFIRE_KILLED_MATCH_MAN;
	static const std::string CAMPFIRE_LIGHTER;
	static const std::string CAMPFIRE_RECEIVESTAT;
	static const std::string CAMPFIRE_RANK_RCV_STAT;

	static const std::string WAR_RESULT;
	static const std::string WAR_RANK;
	static const std::string WAR_KILL_NUM;
	static const std::string WAR_COE;
	static const std::string WORSHIPS;

	static const std::string NOTICE_ID;
	static const std::string IS_PLAYED_EFFECT;
	static const std::string IS_SHOW;
	static const std::string IS_ROPG;

	static const std::string GUIDE_LIST;
	struct Guide {
		static const std::string GUIDE_ID;
		static const std::string USED_NUM;
	};
	static const std::string  GUIDE_STEP_LIST;
	struct Guide_Step {
		static const std::string GUIDE_ID;
		static const std::string STEP;
		static const std::string NUM;
	};
	static const std::string PLAYED_PLOT_LIST;
	struct Played_Plot {
		static const std::string PLOT_ID;
	};
	static const std::string SYS_CFG;
	struct SysCfg {
		static const std::string IS_TOTAL_OPEN;
		static const std::string IS_OPEN_MUSIC;
		static const std::string MUSIC_NUM;
		static const std::string IS_OPEN_VOICE;
		static const std::string VOICE_NUM;
		static const std::string IS_HIDE_ROLE_MODEL;
		static const std::string IS_HIDE_HERO_MODEL;
		static const std::string IS_HIDE_TITLE;
		static const std::string PLAYER_COUNT;
		static const std::string IS_SKIP_PLOT;
	};
	static const std::string AWAKE_LVL;
	static const std::string ARTIFACTS;
	struct Artifact {
		static const std::string ARTIFACT_ID;
		static const std::string ARTIFACT_LVL;
	};
	static const std::string FORNOTICE_GIFTS_STATUS;
	struct Fornotice_Gifts_Status {
		static const std::string GIFT_LVL;
		static const std::string HAS_RECEIVED;
	};
	static const std::string RECEIVE_FLOWERS_LIST;
	struct Receive_Flowers_List {
		static const std::string ROLE_ID;
		static const std::string ROLE_NAME;
	    static const std::string ITEM_ID;
	    static const std::string CHARM;
	    static const std::string FRIEND_NUM;
	    static const std::string MSG;
	};

	static const std::string EXPLOIT_VAL;
	static const std::string EXPLOIT_LVL;
	static const std::string FIRST_RECHARGE;
	static const std::string TWO_RECHARGE;
	static const std::string LIMIT_TIME_RECHARGE;
	static const std::string LAST_TWO_RECHARGE_STIME;
	static const std::string LAST_LIMIT_TIME_RECHARGE_STIME;
	static const std::string RECHARGE_MONEY;
	static const std::string RECHARGE_MONEY_HIS;
	static const std::string RECHARGE_LSTIME;
	static const std::string RECHARGE_FEEDBACK_LIST;
	static const std::string ANS_QID;
	static const std::string INV_TIME;
	static const std::string REC_INV_TIME;
	static const std::string IS_RECEIVE_INV;
	static const std::string IS_RECEIVE_LIKE;
	static const std::string TODAY_REC_BG;
	static const std::string IS_MAGIC_WING_OVER_TIME;
	static const std::string IS_MAGIC_WING_READED;
	static const std::string VIP_OVER_TIME;
	static const std::string IS_VIP_READED;
	static const std::string GENIUS_OVER_TIME;
	static const std::string IS_GENIUS_READED;
	static const std::string TASK_GUIDE_ID;

	static const std::string VIP_UPGRADE_CARDS;
	static const std::string START_FIRST_RECHARGE_TIME;
};

struct Chat_Operating_Fields {
	static const std::string PRIVATE_CHAT_COLLECTION;
	static const std::string GANG_CHAT_RECORD_COLLECTION;
	static const std::string ROLE_ID;
	static const std::string TIME;
	static const std::string CHAT_TYPE;
	static const std::string RECORDS;
	static const std::string RECORD;
	struct Record {
		static const std::string CHAT_TYPE;
		static const std::string CONTENT;
		static const std::string TYPE;
		static const std::string ROLE;
		static const std::string ROLE_ROLE_ID;
		static const std::string ROLE_ROLE_NAME;
		static const std::string ROLE_GENDER;
		static const std::string ROLE_GUILD_NAME;
		static const std::string ROLE_CAREER;
		static const std::string ROLE_LEVEL;
		static const std::string ROLE_VIP_LEVEL;
		static const std::string ROLE_COMBAT;
		static const std::string ITEM;
		static const std::string ITEM_ID;
		static const std::string ITEM_AMOUNT;
		static const std::string ITEM_BIND;
	};
	static const std::string TARGET;
};

struct Operating_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string MAIL_LIST;
    struct Mail_List {
        static const std::string RECEIVER_ID;
        static const std::string SENDER_ID;
        static const std::string SEND_TYPE;
        static const std::string SENDER_NAME;
        static const std::string GOLD;
        static const std::string TITLE;
        static const std::string CONTENT;
        static const std::string ITEM;
        static const std::string SHOW_SEND_TIME;
    };
    /**********/
    static const std::string ADD_F_INVITE_LIST;
    static const std::string HANDEL_F_APPLY_LIST;
    struct Friend_List {
    	static const std::string ROLE_ID;
    	static const std::string TIME;
    	static const std::string HANDEL;
    };
    static const std::string GANGER_INFO;
    struct Ganger_Info {
    	static const std::string GANG_ID;
    	static const std::string LAST_GANG_ID;
    	static const std::string DAY_CONTRIBUTE;
    	static const std::string WEEK_CONTRIBUTE;
    	static const std::string DAY_GOLD_CONTRIB_TIMES;
    	static const std::string DAY_DIAM_CONTRIB_TIMES;
    	static const std::string AUTHORITY;
    	static const std::string JOIN_TIME;
    	static const std::string LEAVE_TIME;
    	static const std::string HEADSHIP;
    };

	static const std::string ARENA_THREE_DAY_REWARD;
	static const std::string ARENA_SEVEN_DAY_REWARD;
   struct Arena_Reward {
       static const std::string RANK;
       static const std::string RANK_TIME;
    };

   static const std::string RECEIVE_FLOWERS_LIST;
   struct Receive_Flowers_List {
	   static const std::string ROLE_ID;
	   static const std::string ROLE_NAME;
	   static const std::string ITEM_ID;
	   static const std::string CHARM;
	   static const std::string FRIEND_NUM;
	   static const std::string MSG;
   };

   static const std::string DRAGON_VALE_BUILDING_NOTICE_LIST;
   struct Dragon_Vale_Building_Notice {
	   static const std::string NOTICE_TYPE;
	   static const std::string GOLD_MINE;
	   static const std::string MOON_WELL;
	   static const std::string TOTEM;
   };

   static const std::string DRAGON_VALE_ROB_FIGHT_RESULT_LIST;
   struct Dragon_Vale_Rob_Fight_Result {
	   static const std::string NOTICE_TYPE;
	   static const std::string SERVER_NAME;
	   static const std::string PLAYER_NAME;
	   static const std::string RESULT;
   };

   static const std::string DRAGON_VALE_GAIN_ROB_PRODUCTION_LIST;
   struct Dragon_Vale_Gain_Rob_Production {
	   static const std::string SERVER_NAME;
	   static const std::string PLAYER_NAME;
	   static const std::string GOLD_MINE;
	   static const std::string MOON_WELL;
	   static const std::string TOTEM;
   };

   static const std::string IS_DRAGON_VALE_SPECIAL;
   static const std::string INV_TIME;
   static const std::string LIKE_SUCCESS;
   static const std::string DAMAGE;
   static const std::string DAMAGE_RATE;
   static const std::string WBRANK;

};

struct Fighter_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string PHY_POWER_CUR;
	static const std::string PHY_POWER_RECOVER_TIME;
	static const std::string PHY_POWER_BUY_TIMES;
	static const std::string LEVEL;
	static const std::string TTL_SKILL_POINTER;
	static const std::string CURRENT_BLOOD;
	static const std::string CURRENT_MORALE;
	static const std::string CURRENT_STRENGTH;
	static const std::string EXPERIENCE;
	static const std::string ELITE_BTL_MORALE;
	static const std::string CURRENT_BLOOD_POOL;
	static const std::string FORCE;
	static const std::string HIGHEST_FORCE;
	static const std::string TTL_FORCE;
	static const std::string DEFAULT_SKILL;
	static const std::string CUR_USED_TALENT;
	static const std::string RESET_SKILL_TIMES;
	static const std::string IS_CLEAR_MORAL;
	static const std::string TALENT;
	struct Talent {
		static const std::string TALENT_ID;
		static const std::string SKILL_POINTER;
		static const std::string SKILL;
		struct Skill {
			static const std::string SKILL_ID;
			static const std::string SKILL_LEVEL;
			static const std::string SkILL_LOC;
		};
	};
	static const std::string ATTRIBUTE;
	struct Attribute {
		static const std::string AT_BASIC;
		static const std::string AT_EQUIP;
		static const std::string AT_CONVERSION;
		static const std::string AT_INITIATIVE_SKILL;
		static const std::string AT_STATUS;
		static const std::string AT_PASSIVE_SKILL;
		static const std::string AT_RUNE_STONE;
//		static const std::string AT_RUNE;
		static const std::string AT_HERO_ASSIST;
		static const std::string AT_GANG;
		static const std::string AT_ARTIFACTS;
		static const std::string AT_ACHIEVEMENT;
		static const std::string AT_TITLE;
		static const std::string AT_DRAGON_VALE;
		static const std::string AT_ANCI_ARTIFACT;
		static const std::string AT_WING;
		static const std::string AT_MOUNT;

		struct Attr_Part {
			static const std::string BASE;
			static const std::string FIX;
			static const std::string PERCENT;
		};
	};

	static const std::string ARENA_FIGHT;
	struct Arena_Fight{
		static const std::string PT_PHY_ATTACK;
		static const std::string PT_SPELL_ATTACK;
		static const std::string PT_PHY_DEFENSE;
		static const std::string PT_SPELL_DEFENSE;
		static const std::string PT_HIT;
		static const std::string PT_AVOID;
		static const std::string PT_IGNORE_DEF;
		static const std::string PT_BLOOD_MAX;
		static const std::string PT_CRIT;
		static const std::string PT_CRIT_DEF;
		static const std::string PT_CURE;
		static const std::string PT_CURE_RATE;
		static const std::string PT_SING_RATE;
		static const std::string PT_INIT_MORALE;
		static const std::string PT_MORALE_CAPS;
		static const std::string PT_CRIT_HURT;
		static const std::string PT_CRIT_HURT_SUB;
		static const std::string PT_HIT_RATE_ADDITION;
		static const std::string PT_AVOID_RATE_ADDITION;
		static const std::string PT_CRIT_RATE_ADDITION;
		static const std::string PT_CRIT_DEF_RATE_ADDITION;
		static const std::string PT_SPEED;
		static const std::string PT_BLOOD_CURRENT;
		static const std::string PT_MORALE_CURRENT;
		static const std::string PT_STRENGTH_CURRENT;
		static const std::string PT_STRENGTH_MAX;
		static const std::string PT_BLOOD_POOL_CURRENT;
		static const std::string PT_BLOOD_POOL_MAX;
		static const std::string PT_IGNORE_DEF_RATE;
		static const std::string PT_STATUS_HIT_RATE;
		static const std::string PT_STATUS_DEF_RATE;
		static const std::string PT_BLOOD_STEAL;
		static const std::string PT_BE_CURE_RATE;

		static const std::string PT_PHY_HURT_FIX;
		static const std::string PT_PHY_HURT_RATE;
		static const std::string PT_SPELL_HURT_FIX;
		static const std::string PT_SPELL_HURT_RATE;
		static const std::string PT_HURT_FIX;
		static const std::string PT_HURT_RATE;
		static const std::string PT_BE_HURT_FIX;
		static const std::string PT_BE_HURT_RATE;
		static const std::string PT_EFFECT_RATE;
		static const std::string PT_PVE_HURT_ADD;
		static const std::string PT_PVE_HURT_SUB;

		static const std::string PT_DIZZ_STRENGTHEN;
		static const std::string PT_DIZZ_WEAKEN;

		static const std::string PT_PK_HURT_ADD;
		static const std::string PT_PK_HURT_SUB;
		static const std::string PT_PHY_HURT_RATE_SUB;
		static const std::string PT_SPELL_HURT_RATE_SUB;
		static const std::string PT_SKILL_HURT_RATE;
		static const std::string PT_HUMAN_ADD;
		static const std::string PT_ELF_ADD;
		static const std::string PT_ORC_ADD;
		static const std::string PT_DRAGON_ADD;
		static const std::string PT_UNDEAD_ADD;
		static const std::string PT_DEMON_ADD;
		static const std::string PT_HUMAN_SUB;
		static const std::string PT_ELF_SUB;
		static const std::string PT_ORC_SUB;
		static const std::string PT_DRAGON_SUB;
		static const std::string PT_UNDEAD_SUB;
		static const std::string PT_DEMON_SUB;
		static const std::string PT_SUMMON_ADD;
		static const std::string PT_SUMMON_SUB;
		static const std::string FORCE;

		static const std::string EXTRA_SKILL;
		struct Extra_Skill {
			static const std::string SKILL_ID;
			static const std::string SKILL_LV;
		};
	};
};

struct Mail_Fields {
    static const std::string COLLECTION;
    static const std::string MAIL_ALL_COUNT;
    static const std::string MAX_GOLD;
    static const std::string MAIL_PAGE_COUNT;
    static const std::string MAIL_CURRENT_PAGE;
    static const std::string LAST_READ_SYS_MAIL_TIME;
    static const std::string MAIL_LIST;
    struct Mail_List{
        static const std::string MAIL_ID;
        static const std::string HAVE_READ;
        static const std::string COIN;
        static const std::string MAIL_TITLE;
        static const std::string MAIL_TYPE;
        static const std::string MAIL_CONTENT;
        static const std::string SENDER_NAME;
        static const std::string ITEM;
        static const std::string SHOW_SEND_TIME;
        static const std::string REAL_SEND_TIME;

    };
};

struct Alchemy_Fields {
	static const std::string COLLECTION;
	static const std::string CURRENT_HERO_CRITICAL;
	static const std::string CUTTENT_HERO_GOLD;
	static const std::string OVER_FREE_BOUT;
	static const std::string OVER_PAY_BOUT;
	static const std::string SIX_CRITICAL_NUM;
	static const std::string LAST_TABS;
	static const std::string SCURRENT_HERO_CRITICAL;
	static const std::string SCUTTENT_HERO_GOLD;
	static const std::string SOVER_FREE_BOUT;
	static const std::string SOVER_PAY_BOUT;
	static const std::string SSIX_CRITICAL_NUM;
	static const std::string SLAST_TABS;
};

struct Friend_Fields {
	static const std::string COLLECTION;
	static const std::string FRIEND_LIST;
	static const std::string STRANGER_LIST;
	static const std::string BLACK_LIST;
	struct Friend_Info {
		static const std::string ROLE_ID;
		static const std::string FRIEND_TYPE;
		static const std::string FRIENDSHIP_DEGREE;
	};
	static const std::string APPLY_LIST;
	struct Apply {
		static const std::string ROLE_ID;
		static const std::string TIME;
	};
	static const std::string INVITE_LIST;
	struct Invite {
		static const std::string ROLE_ID;
		static const std::string TIME;
	};
	static const std::string ROLE_ID;
	static const std::string HEAD_PHOTO_ID;
	static const std::string DEL_LIST;
	struct Del {
		static const std::string ROLE_ID;
		static const std::string FRIENDSHIP_DEGREE;
	};
};

struct Package_Fields {
    static const std::string COLLECTION;
    static const std::string PACKAGE;
    static const std::string PACKAGE_MONEY_COPPER;

    static const std::string PACK_CAPACITY;
    static const std::string STORAGE_CAPACITY;
    static const std::string SPIRIT_EQUIP_CAPACITY;
    static const std::string SPIRIT_PACKAGE_CAPACITY;
    static const std::string RUNE_STONE_EQUIP_CAP;
    static const std::string RUNE_STONE_PACK_CAP;
    static const std::string THORLOTTERY_PACK_CAP;

    static const std::string MONEY;
	struct Money {
		static const std::string COPPER;
    	static const std::string BIND_GOLD;
		static const std::string GOLD;
		static const std::string DRAGON_SOUL;
		static const std::string SOULS;
		static const std::string FRIENDSHIP;
		static const std::string REPUTATION;

		static const std::string TODAY_GOLD_PAY;
		static const std::string TODAY_GOLD_GET;
		static const std::string TODAY_COPPER_PAY;
		static const std::string TODAY_COPPER_GET;
	};

	static const std::string LAST_WIPE_TIME;

	static const std::string ITEM;
	struct Item {
		// 共有属性
		static const std::string INDEX;
		static const std::string ID;
		static const std::string AMOUNT;
		static const std::string BIND;
		static const std::string OVERDUE_TIME;

		// 装备属性
		static const std::string UNBAN_LVL;
		static const std::string SCORE;
		static const std::string FORCE;
		static const std::string INCR_LEVEL;
		static const std::string INCR_LUCK_VALUE;
		static const std::string INCR_LVL_CONTI_FAIL_CNT;
		static const std::string COMBAT_POWER;
		static const std::string DURABILITY;
		static const std::string LEVEL;
		static const std::string PERFECTION;
		static const std::string COLOR;
		static const std::string HOLE;
		struct Hole {
			static const std::string GEM_ID;
			static const std::string GEM_STEP;
			static const std::string GEM_PURTITY;
		};
		static const std::string ADDIT_SPIRIT;
		static const std::string ADDIT_SPIRITS;
		static const std::string SCORE_BASE;
		static const std::string LIGHTEN_TIME;
		static const std::string DISPLAY_REMAIN_TIME;
		static const std::string TIME_LIMIT;
		static const std::string INCR_ENERGY_LEVEL;
		static const std::string INCR_BLESS_LEVEL;
		static const std::string INCR_BLESS_VALUE;
		static const std::string INCR_LEVEL_CONTI_CNT;
		static const std::string ENCHANT_TIMES;
		static const std::string CNT_ENCHANT_GROUP;
		static const std::string ENCHANT_GROUPS;
		struct Enchant_Groups {
			static const std::string CNT_ENCHANT_PROP;
			static const std::string ENCHANT_PROPS;
			struct Enchant_Props {
				static const std::string PROP_TYPE;
				static const std::string PROP_VAL;
				static const std::string STAR;
				static const std::string IS_LOCK;
			};
			static const std::string SCORE;
		};
		static const std::string ENCHANT_LOCK_PROP_FAIL;
		struct Enchant_Lock_Prop_Fail {
			static const std::string PROP;
			static const std::string FAIL_CNT;
		};
		static const std::string CARD_HOLE;
		struct Card_Hole {
			static const std::string LOC;
			static const std::string STATE;
			static const std::string CARD_INDEX;
		};
		static const std::string STONE;
		struct Stone_Hole {
			static const std::string STONE_HOLE_INDEX;
			static const std::string STONE_HOLE_STATE;
			static const std::string STONE_ID;
		};

		// 符石属性
		static const std::string EXP_LVL;
		static const std::string EXP;
		static const std::string IS_LOCK;

		// 卡牌属性
		static const std::string CARD_PART;
		static const std::string CARD_POTEN;
		static const std::string CARD_STAR;
		static const std::string CARD_LV;
		static const std::string CARD_EXP;
		static const std::string CARD_EQUIP_INDEX;
		static const std::string CARD_EQUIP_LOC;
		static const std::string CARD_MAIN_ATTR;
		static const std::string CARD_SUB_ATTR;
	};
};

struct Item_User_Fields {
	static const std::string COLLECTION;

	static const std::string USE_TIME;
	static const std::string ITEM_USE_SEC;

	static const std::string ITEM_PROP;
	static const std::string ITEM_PROP_TYPE;
	static const std::string ITEM_PROP_VALUE;
	static const std::string ITEM_HAD_USED_TIMES;
	static const std::string ITEM_HAD_USED_TIMES_TYPE;
	static const std::string ITEM_HAD_USED_TIMES_VALUE;
};

struct Task_Fields
{
	static const std::string COLLECTION;
	static const std::string ID;
    static const std::string SUBMITED_TASK;
    static const std::string NPC_ID;
    static const std::string IS_EVER_OPEN_REWARD_TASK_LIST;
    static const std::string IS_CLOSE_AUTO_REFRESH;
    static const std::string REFRESH_REWARD_TASK_NUM;
	static const std::string IS_OPEN_COUNTDOWN;
    static const std::string BEGIN_COUNTDOWN_TIME_SEC;
    static const std::string BEGIN_COUNTDOWN_TIME_USEC;
    static const std::string USED_FREE_REFRESH_COUNT;
    static const std::string USED_ACCEPT_COUNT;
    static const std::string GIVE_UP_NUM;
    static const std::string TARGET_PLAYER_ID;
    static const std::string TASK_ID;
    static const std::string RING_NUM;
    static const std::string IS_DIAMOND_RECOVER;

    static const std::string NPC_TYPE_MAP;
    struct Npc_Type_Map {
    	static const std::string NPC_ID;
    	static const std::string NPC_TYPE;
    	static const std::string FUNCNAME;
    	static const std::string DIRECTION;
    	static const std::string SCENEID;
    	static const std::string OPEN;
    	static const std::string X;
    	static const std::string Y;
    	static const std::string NPC_NAME;
    };
    static const std::string TASK;
	struct Task
	{
		static const std::string TASK_ID;
        static const std::string GAME_TYPE;
        static const std::string ACCEPT_TICK_SEC;
        static const std::string ACCEPT_TICK_USEC;
        static const std::string REFRESH_TICK_SEC;
        static const std::string REFRESH_TICK_USEC;
        static const std::string PREV_TASK;
        static const std::string POST_TASK;
        static const std::string POST_TASK2;
        static const std::string POST_TASK3;
        static const std::string STATUS;
        static const std::string LOGIC_TYPE;
        static const std::string ITEM_MONEY_FLAG;
        static const std::string IS_REQ_FAST_FINISH;
        static const std::string RECRUID_HERO;
        static const std::string HERO_ID;
        static const std::string IS_PACK_ACCEPT;

        static const std::string COND_LIST;
        struct CondList
        {
            static const std::string TYPE;
            static const std::string CURRENT_VALUE;

            static const std::string EXT_LIST;
            struct ExtList {
            	static const std::string EXT;
            };
        };
	};

    static const std::string TASK_ICON_HERO_LIST;
    struct	Task_Icon_Hero {
    	static const std::string TASK_ID;
    	static const std::string HERO_ID;
    };

    static const std::string TASK_GIFT_LEVEL;

    static const std::string SCENE_ID_LIST;
    struct Scene_Id {
    	static const std::string SCENE_ID;
    };

    static const std::string DID_RING_NUM;
    static const std::string DID_REWARD_TASK_NUM;

    static const std::string SCENE_NPC_LIST;
    struct Scene_Npc {
    	static const std::string SCENE_ID;
    	static const std::string NPC_ID;
    };

    static const std::string RING_TASK_ID;
    static const std::string IS_EXIST_CROSS_DAY;
    static const std::string DID_REWARD_TASK_NUM_TODAY;
    static const std::string IS_SUBMIT_CLOSE_RING_TASK;
};

struct Sgl_Dungeon_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string LAST_LOCATION;
	static const std::string SCENE_ID;
	static const std::string SCENE_ORDER;
	static const std::string SCENE_LAYER;
	static const std::string COORD_X;
	static const std::string COORD_Y;
	static const std::string TOWARD;
	static const std::string SINGLE_VER;
	static const std::string ELITE_VER;
	static const std::string LORD_VER;
	static const std::string SINGLE_COMPLETE_NUM;
	static const std::string NOR_ELITE_COMPLETE_NUM;
	static const std::string HOR_ELITE_COMPLETE_NUM;
	static const std::string ELITE_RESET_TIMES;
	static const std::string ELITE_CH_NUM;
	static const std::string ELITE_ADD_NUM;
	static const std::string ELITE_ADD_BUY_NUM;
	static const std::string ELITE_TASK_ADD_NUM;
	static const std::string ACT_NOR_ELITE_WIPE;
	static const std::string SGLDUNGEONITEM;
	static const std::string NOR_ELITE_ITEM;
	static const std::string HOR_ELITE_ITEM;
	static const std::string CHAPTER_AWARD_LIST;
	static const std::string TRAIL_INFO;
	static const std::string LORD_TRAIL_INFO;

	static const std::string WIPE_TYPE;
	static const std::string WIPE_NUM;
	static const std::string WIPE_ELITE_DOUBLE;
	static const std::string WIPE_ELITE_DOUBLE_NOT_SELECTED;
	static const std::string WIPE_BEGIN_TIME;
	static const std::string WIPE_SCENES;
	static const std::string WIPE_INFOS;

	struct Item_Info{
		static const std::string ID;
		static const std::string AMOUNT;
		static const std::string BIND;
	};

	struct Sgl_Dungeon_Fields_Item {
		static const std::string DUNGEON_ID;
		static const std::string TYPE;
		static const std::string PROGRESS;
		static const std::string TTL_COUNTER;
		static const std::string DAILY_COUNTER;
		static const std::string ADD_GOLD_NUM;
		static const std::string ADD_PROP_NUM;
		static const std::string CLEAR_MONSTER;
	};
	struct Chapter_Award{
		static const std::string CHAPTER_ID;
		static const std::string AWARD_STATUS;
	};
	struct Trail_Info{
		static const std::string ELITE_ITEMS;
	};
	struct Wipe_Scene{
		static const std::string SCENE_ID;
	};
	struct Wipe_Info{
		static const std::string INDEX;
		static const std::string SCENE_ID;
		static const std::string EXP;
		static const std::string COPPER;
		static const std::string ITEMS;
	};
};

struct Heroer_Fields {
	static const std::string COLLECTION;
	static const std::string STATUS_COLLECTION;
	static const std::string ROLE_ID;

	static const std::string OUTER_INDEX;
	static const std::string FIGHTER_INDEX;
	static const std::string LAST_FIGHTER_IDX;
	static const std::string CLICKED_NEW_HEROS;
	static const std::string ASSIST_HEROS;
	struct Assist_Hero {
		static const std::string LOC;
		static const std::string HERO_IDX;
	};
	static const std::string HERO_INFOS;
	struct Hero_Info {
		static const std::string HERO_ID;
		static const std::string FIGHT_DATE;
		static const std::string HERO_INDEX;
		static const std::string HERO_NAME;
		static const std::string HERO_FACADE_ID;
		static const std::string LEVEL;
		static const std::string EXP;
		static const std::string IS_OUTER;
		static const std::string IS_FIGHTER; //
		static const std::string FORMATION;
		static const std::string AWAKE_LVL;
		static const std::string SKILL_INFOS;
		static const std::string COMBAT_POWER;	// 此字段专门存储给排行榜
		static const std::string IS_TMP_HERO;

		static const std::string ARTIFACTS;
		struct Artifact {
			static const std::string ARTIFACT_ID;
			static const std::string ARTIFACT_LVL;
		};

		static const std::string INTIMACY_LV;
		static const std::string INTIMACY_EXP;
		static const std::string COMMUNITY_TIMES;
		static const std::string GIFT_TIMES;
	};
};

struct Rune_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;

	static const std::string PARTS;
	struct Part {
		static const std::string PART;
		static const std::string HOLES;
		struct Hole {
			static const std::string INDEX;
			static const std::string LEVEL;
			static const std::string ENERGY;
		};
	};
};

struct Rune_Stone_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;

	static const std::string POINTS;
	static const std::string REMAIN_DOUBLE_TIMES;
	static const std::string SMELTER_DETAILS;
	struct Smelter_Detail {
		static const std::string SMELTER_IDX;
		static const std::string IS_OPEN;
	};
};

struct Time_Value_Fields {
	static const std::string SEC;
	static const std::string USEC;
};
struct Status_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;

	static const std::string STATUS_MAP;
	struct Status_Map {
		static const std::string STATUS_ID;
		static const std::string OVERLAY;
		static const std::string ACTION_TIMES;
		static const std::string TRIGGER_TIMES;
		static const std::string CFG_ACTION_TIMES;
		static const std::string CFG_TRIGGER_TIMES;
		static const std::string LAST_STATUS_FROM;
		static const std::string LAST_CASTER_ROLE_ID;
		static const std::string LAST_CASTER_MASTER_ID;
		static const std::string IS_FIGHT_BUFF;
		static const std::string LAST_TIME_MS;
		static const std::string LAST_START_TIME;
		static const std::string LAST_END_TIME;
		static const std::string BUFF_SCENE;
		static const std::string SCENE_VISIBLE;

		static const std::string STATUS_FIFO;

		static const std::string STATUS_INFO_MAP;
		struct Status_Info_Map {
			static const std::string STATUS_ID;
			static const std::string STATUS_FROM;
			static const std::string CASTER_ROLE_ID;
			static const std::string CASTER_MASTER_ID;
			static const std::string OVERLAY;

			static const std::string EFFECT_MAP;
			struct Effect_Map {
				static const std::string EFFECT_ID;
				static const std::string EFFECT_VALUE;

				static const std::string I_EFFECT_VALUE1;
				static const std::string I_EFFECT_VALUE2;
				static const std::string I_EFFECT_VALUE3;
				static const std::string D_EFFECT_VALUE1;
				static const std::string D_EFFECT_VALUE2;
				static const std::string D_EFFECT_VALUE3;
				static const std::string D_EFFECT_VALUE4;
				static const std::string D_EFFECT_VALUE5;
				static const std::string D_EFFECT_VALUE6;
				static const std::string D_EFFECT_VALUE7;
				static const std::string D_EFFECT_VALUE8;
				static const std::string D_EFFECT_VALUE9;

				static const std::string EFFECT_TYPE;
				static const std::string STATUS_ID;
				static const std::string HEAT_TYPE;
				static const std::string EFFECT_TIMES;
				static const std::string OVERLAY;
				static const std::string IS_OVERLAY;
				static const std::string IS_HIT;
				static const std::string SKILL_ID;

				static const std::string TRIGGER_BUFF;
				struct Trigger_Buff {
					static const std::string STATUS_ID;
					static const std::string LAST_TIME_MS;
					static const std::string CFG_ACTION_GAP;
					static const std::string CFG_TRIGGER_GAP;
					static const std::string STATUS_FROM;
					static const std::string CASTER_ID;
					static const std::string OVERLAY;

					static const std::string DISAPPEAR_GAP;

					static const std::string DATA;
					struct Data {
						static const std::string EFFECT_VALUE;
						static const std::string I_EFFECT_VAL1;
						static const std::string I_EFFECT_VAL2;
						static const std::string I_EFFECT_VAL3;
						static const std::string D_EFFECT_VAL1;
						static const std::string D_EFFECT_VAL2;
						static const std::string D_EFFECT_VAL3;
						static const std::string D_EFFECT_VAL4;
						static const std::string D_EFFECT_VAL5;
						static const std::string D_EFFECT_VAL6;
						static const std::string D_EFFECT_VAL7;
						static const std::string D_EFFECT_VAL8;
						static const std::string D_EFFECT_VAL9;
					};
					static const std::string IS_FIGHT_BUFF;
				};

				static const std::string BUFF_CHANCE;
				static const std::string CASTER_ID;

				static const std::string ACTION_TIMES;
				static const std::string ACTION_TICK;
				static const std::string PASSIVE_TIMES;
				static const std::string PASSIVE_TICK;
				static const std::string NEXT_OPT;
				static const std::string INTERVAL;

				static const std::string IS_DELAY;
				static const std::string DELAY_TIMES;
				static const std::string DELAY_TIME_MS;
				static const std::string DELAY_ACTION_TIMES;
				static const std::string DELAY_PASSIVE_TIMES;
				static const std::string DELAY_TIME_TICK;
				static const std::string DELAY_ACTION_TICK;
				static const std::string DELAY_PASSIVE_TICK;

				static const std::string LAST_CURE_VALUE;
				static const std::string LAST_CURE_RATE;
			};
		};
	};
};


//封号（帐号，角色）表字段
struct BAN_ACCOUNT_Fields {
	static const std::string COLLECTION;//集合名
	static const std::string BAN_TYPE; //类型，0帐号 1角色名 2角色ID

	static const std::string BAN_NAME; //相应类型的名
	static const std::string ACCOUNT;   //帐号
	static const std::string ROLE_ID;   //角色名

  	static const std::string BAN_REMARK; //封号原因
	static const std::string BAN_TIME;  //封号时间
	static const std::string UNBAN_TIME; //解封时间
};
//封号IP表字段
struct BAN_IP_Fields {
	static const std::string COLLECTION;//集合名
	static const std::string IP_ADDRESS;//ip地址

  	static const std::string BAN_REMARK; //封号原因
	static const std::string BAN_TIME;  //封号时间
	static const std::string UNBAN_TIME; //解封时间
};
//商城列表字段
struct MALL_LIST_Fields {
	static const std::string COLLECTION;//集合名
	static const std::string ID;//物品ID
	static const std::string TABS;//标签
	static const std::string PAY_TYPE;//支付类型
	static const std::string PRICE;//售价
	static const std::string VIP_DISCOUNTS;//是否享受VIP打折
	static const std::string PROMO_PRICE;
	static const std::string ALL_NUM;
	static const std::string PER_NUM;
	static const std::string SORT;//
};
struct MALL_PROMO_LIST_Fields {
	static const std::string COLLECTION;//集合名
	static const std::string START_TIME;//
	static const std::string END_TIME;//
	static const std::string DAY;//
	static const std::string RANK;//
	static const std::string GOODS_LIST;//
};
struct MALL_BANNER_LIST_Fields {
	static const std::string COLLECTION;//集合名
	static const std::string START_TIME;//
	static const std::string END_TIME;//
	static const std::string DAY;//
	static const std::string RANK;//
	static const std::string GOODS_LIST;//
};
//Team
struct Team_Role_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string TEAM_ID;
	static const std::string READY;
	static const std::string PROFILE_NUM_FB;
	static const std::string PROFILE_NUM_FB_PRE;
	static const std::string PROFILE_NUM_SPORTS;
	static const std::string PROFILE_NUM_HERO_DREAM;
	static const std::string PROFILE_NUM_DEFEND;
	static const std::string PROFILE_TIME_SEC;
	static const std::string IS_NOT_FIRST_TEAM_FB;
	static const std::string INTEGRATE_MAP;
	static const std::string TEAM_SPORTS_START_TIME;
};
struct Team_Sports_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string TEAM_ID;
	static const std::string READY;
	static const std::string PROFILE_NUM_FB;
	static const std::string PROFILE_NUM_FB_PRE;
	static const std::string PROFILE_NUM_SPORTS;
	static const std::string PROFILE_NUM_HERO_DREAM;
	static const std::string PROFILE_TIME_SEC;
	static const std::string INTEGRATE;
	static const std::string INTEGRATE_MAP;
	static const std::string TEAM_SPORTS_START_TIME;
	static const std::string TEAM_SPORTS_START_TIMES;

	static const std::string FIRST_LVL;
	struct First_Lvl {
		static const std::string FIRST_LVL_ROLE_ID_HIS;
		static const std::string FIRST_LVL_ZS_HIS;
		static const std::string FIRST_LVL_CK_HIS;
		static const std::string FIRST_LVL_YSS_HIS;
		static const std::string FIRST_LVL_INT;
		static const std::string FIRST_LVL_ROLE_ID;
	};
};
// Gang
struct Gang_Fields {
	static const std::string COLLECTION;
	static const std::string GANG_ID;
	static const std::string LEADER;
	static const std::string LAST_LEADER_ID;
	static const std::string NOTIFIED_LAST_LEADER;
	static const std::string FUND;
	static const std::string MAINTAIN_COST;
	static const std::string FUND_LACK_TIMES;
	static const std::string LEVEL;
	static const std::string BOSS_ACT;
	static const std::string BOSS_HOUR;
	static const std::string BOSS_MINUTE;
	static const std::string BOSS_KILL_NUM;
	static const std::string BOSS_LV;
	static const std::string RANK;
	static const std::string MAX_MEMBERS;
	static const std::string LOGO;
	static const std::string DAY_DEFICIT_COUNT;
	static const std::string STATUS;
	static const std::string JOIN_BLOCK;
	static const std::string CD_UPGRADE;
	static const std::string CREATE_TIME;
	static const std::string DISMISS_TIME;
	static const std::string TV_LEADER_OFFLINE;
	static const std::string ANNOUNCEMENT;
	static const std::string NAME;
	static const std::string SERVER_INFO;
	struct Server_Info {
		static const std::string AGENT_NUM;
		static const std::string SERVER_NUM;
	};
	static const std::string MEMBERS;
	struct Member {
		static const std::string ROLE_ID;
		static const std::string WEEK_CONTRIBUTE;
		static const std::string TOTAL_CONTRIBUTE;
		//static const std::string FIGHT;
		static const std::string AMPLIFY_TIMES;
		//static const std::string LEVEL;
		//static const std::string VIP;
		static const std::string HEADSHIP;
		static const std::string AUTHORITY;
		//static const std::string CAREER;
		//static const std::string LINE_STAT;
		//static const std::string GENDER;
		//static const std::string GANGER_NAME;
	};
	static const std::string MEMBER_SIZE;
	static const std::string AFFAIRS;
	struct Affairs {
		static const std::string TYPE;
		static const std::string RESULT;
		static const std::string EXPIRE;
		static const std::string APPLIER;
	};
	static const std::string RESEARCHED_SKILLS;
	struct Researched_Skills {
		static const std::string SKILL_ID;
	};
	static const std::string BUILDINGS;
	struct Building {
		static const std::string BUILDING_ID;
		static const std::string BUILDING_LVL;
		static const std::string CD_UPGRADE;
	};
	static const std::string REWARD_INFO_GANG_WAR;
	struct Reward_Info_Gang_War {
		static const std::string INDEX;
		static const std::string REWARD_ID;
		static const std::string QUANTITY;
		static const std::string VIP_ADD;
	};
	static const std::string ACTIVE_ANNON_ED;
	static const std::string LEVEL_LEADER_ID;
};
struct Gang_War_Info_Fields {
	static const std::string COLLECTION;
	static const std::string HISTORY_COLLECTION;
	static const std::string GANG_WAR_MATCHED_GANGS_PAIR_VEC;
	struct Gang_War_Matched_Gangs_Pair_Vec {
		static const std::string MATCH_TIME;
		static const std::string MATCHED_PAIR_FIRST;
		static const std::string MATCHED_PAIR_SECOND;
		static const std::string RESULT;
		static const std::string MATCHED_INDEX;
		static const std::string SCENE_INDEX;
	};

	static const std::string GANG_WAR_MATCHED_GANG_MAP;
	struct Gang_War_Matched_Gang_Map {
		static const std::string KEY;
		static const std::string MATCHED_INDEX;
		static const std::string SCENE_INDEX;
		static const std::string CAMP;
		static const std::string MATCH_TIME;
	};

	static const std::string GANG_WAR_SPOT_RANK_MAP;
	struct Gang_War_Spot_Rank_Map {
		static const std::string KEY;
		static const std::string VALUE;
		static const std::string KILL_NUM;
		static const std::string DIE_NUM;
		static const std::string HOLD_SPOT;
		static const std::string HEADSHIP;
		static const std::string FORCE;
		static const std::string GANG_NAME;
		static const std::string CAMP;
		static const std::string BTN_STATE;
	};

	static const std::string GANG_WAR_RESULT_MAP;
	struct Gang_War_Result_Map {
		static const std::string INDEX;
		static const std::string GANG_VEC;
		struct Gang_Vec {
			static const std::string GANG_ID;
			static const std::string GANG_NAME;
			static const std::string INDEX;
			static const std::string ZONE;
			static const std::string RANK;
		};
		static const std::string RESULT;
	};

	static const std::string GANG_RANK_MAP;
	struct Gang_Rank_Map {
		static const std::string GANG_ID;
		static const std::string GANG_NAME;
		static const std::string ZONE;
		static const std::string SCORE;
		static const std::string WIN_NUMS;
		static const std::string KILL_NUMS;
		static const std::string HOLD_NUMS;
		static const std::string FORCE;
		static const std::string GANGER_RANK_INFO;
		struct Ganger_Rank_Info {
			static const std::string KILL_NUM;
			static const std::string DIE_NUM;
			static const std::string HOLD_SPOT;
			static const std::string HEADSHIP;
			static const std::string FORCE;
			static const std::string CAMP;
		};
	};

	static const std::string OPEN_TIME_FOR_EACH_MATCH;

	static const std::string GANG_WAR_RANK_AWARD;
	struct Gang_War_Rank_Award {
		static const std::string SCORE;
		static const std::string WIN_NUMS;
		static const std::string KILL_NUMS;
		static const std::string FORCE;
		static const std::string STATE;
	};

	static const std::string STAGE;
	static const std::string SWITCH;
	static const std::string ACTIVITY_TIME;
};
// ganger
struct Ganger_Fields {
	static const std::string COLLECTION;
	static const std::string GANG_ID;
	static const std::string LAST_GANG_ID;
	static const std::string ROLE_ID;
	static const std::string CUR_CONTRIBUTE;
	static const std::string TOTAL_CONTRIBUTE;
	static const std::string ACHI_CONTRIBUTE;
	static const std::string DAY_CONTRIBUTE;
	static const std::string CONTRIB_OF_WEEK;
	static const std::string DAY_GOLD_CONTRIB_TIMES;
	static const std::string DAY_DIAM_CONTRIB_TIMES;
	static const std::string LAST_RANK;
	static const std::string AUTHORITY;
	static const std::string RECEIVE_BLESS_TIMES;
	//static const std::string REMAIN_DRAW_TIMES;
	static const std::string TODAY_HAS_DRAW_TIMES;
	static const std::string JOIN_TIME;
	static const std::string LEAVE_TIME;
	static const std::string HEADSHIP;
	static const std::string PRACTICE_TIMES_TTL;
	static const std::string AFFAIRS;
	struct Affairs {
		static const std::string TYPE;
		static const std::string RESULT;
		static const std::string EXTRA_ID;
		static const std::string EXPIRE;
		static const std::string CONTENT;
	};
	static const std::string INVITES;
//	struct Invites {
//		static const std::string GANG_ID;
//	};
	static const std::string APPLY_IDS;
	static const std::string SKILL_DETAILS;
	struct Skill_Details {
		static const std::string SKILL_ID;
	};
	static const std::string DRAW_AWARDS;
	static const std::string GANG_WAR_ATTEND_TIME;
	static const std::string GANG_WAR_SPOT_INDEX;
	static const std::string GANG_WAR_ATTEND_GANG_ID;
	static const std::string GANG_WAR_ATTEND_GANG_ID2;
	static const std::string GANG_WAR_AWARD_GET_TIMES_BY_DAY;
};
//充值表字段
struct Pay_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string PAYTICKET;
	static const std::string TIME;
	static const std::string ORDER_ID;
	static const std::string IN_ACCOUNT;
};

//arena
struct Arena_Battle_Fields{
	static const std::string COLLECTION;
	static const std::string ARENA_ID;
	static const std::string STATUS;
	static const std::string FREE_MATCH_TIMES;
	static const std::string PAY_MATCH_TIMES;
	static const std::string START_TIME;
	static const std::string END_TIME;
	static const std::string REWARD_TIME1;
	static const std::string REWARD_TIME2;
};

struct Arenaer_Fields{
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string ARENA_ID;
	static const std::string GROUP;
	static const std::string SEG;
	static const std::string SCORE;
	static const std::string RISE_FAIL;
	static const std::string RISE_WIN;
	static const std::string RISE_LOSE;
	static const std::string RANK;
	static const std::string KING_TIME;
	static const std::string ENTER_NUM;
	static const std::string PAY_TIMES;
	static const std::string BUY_PAY_TIMES;
	static const std::string STREAK_WIN;
	static const std::string STREAK_LOSE;
	static const std::string COMPENSATE_TIMES;
	static const std::string THREE_DAY_RANK;
	static const std::string SEVEN_DAY_RANK;
	static const std::string CD;
	static const std::string MATCH_TIME;
	static const std::string MATCH_PLAYER_1;
	static const std::string MATCH_PLAYER_2;
	static const std::string MATCH_PLAYER_3;
	static const std::string MATCH_DLV_1;
	static const std::string MATCH_DLV_2;
	static const std::string MATCH_DLV_3;
	static const std::string FIGHT_TIMES;
	static const std::string ARENA_WORSHIP;
	static const std::string BATTLE_REPORT;
	static const std::string BE_WORSHIP;
	struct Battle_Report {
		static const std::string ROLE_NAME;
		static const std::string RESULT;
		static const std::string SCORE;
		static const std::string GOLD;
		static const std::string VAL1;
		static const std::string VAL2;
		static const std::string ITEM;
		struct Item {
			static const std::string ITEM;
			static const std::string COUNT;
		};
	};
	static const std::string FIGHT;
	static const std::string ROLE_NAME;
	static const std::string LEVEL;
	static const std::string CAREER;
	static const std::string GENDER;

};

// arena_season
struct Arena_Season_Fields{
	static const std::string COLLECTION;
	static const std::string SEASON_STATUS;
	static const std::string SEASON_BEGIN;
	static const std::string SEASON_FINISH;
	static const std::string SEASON_RESULT;
	struct Season_Result{
		static const std::string ROLE_ID;
		static const std::string LEVEL;
		static const std::string GROUP_INDEX;
		static const std::string RANK;
		static const std::string INNER_RANK;
	};
};

//start ranking
struct Ranking_Fields{
	static const std::string COLLECTION;
	static const std::string RANKING_TYPE;
	static const std::string RANKING;
	static const std::string ROLE_INFO;
	static const std::string HEROER_INFO;
	static const std::string UPDATE_TIME;
	static const std::string KEY;
	static const std::string VALUE;
	struct Rank_Info {
		static const std::string ROLE_ID;
		static const std::string ROLE_NAME;
		static const std::string CAREER;
		static const std::string SERVER;

		static const std::string EXT_1;
		static const std::string EXT_2;
		static const std::string EXT_3;
		static const std::string EXT_4;
		static const std::string EXT_5;
		static const std::string EXT_6;
		static const std::string EXT_7;
		static const std::string EXT_8;
	};
	struct Role_Info {
		static const std::string ROLE_ID;
		static const std::string ROLE_NAME;
		static const std::string GENDER;
		static const std::string GANG_ID;
		static const std::string GANG_NAME;
		static const std::string HERO_ID;
		static const std::string HERO_INDEX;
		static const std::string EXPLOIT_LVL;
		static const std::string EXPLOIT_VAL;
		static const std::string AVATAR;

		static const std::string HERO_ITEM_INFO;
		static const std::string HERO_ITEM_EQUIP_INFO;
		static const std::string HERO_RUNE_PART_INFO;
		static const std::string HERO_ITEM_RUNE_STONE_INFO;
		static const std::string HERO_PROP_VALUE;

		static const std::string PROP_VALUE;
		struct Prop_Value {
			static const std::string TYPE;
			static const std::string VALUE;
		};
	};
	static const std::string ITEM_INFO;
	struct Item_Info {
		static const std::string INDEX;
		static const std::string SEQ;
		static const std::string ID;
		static const std::string AMOUNT;
		static const std::string BIND;
		static const std::string OVERDUE_TIME;
	};
	static const std::string ITEM_EQUIP_INFO;
	struct Item_Equip_Info {
		static const std::string INDEX;
		static const std::string UNBAN_LVL;
		static const std::string FORCE;
		static const std::string SCORE;
		static const std::string INCR_LEVEL;
		static const std::string INCR_LUCK_VALUE;
		static const std::string INCR_FAIL_ADD_RATE;
		static const std::string CARD_INFO;
		static const std::string ENCHANT_PROP_GROUPS;
		struct Enchant_Prop_Groups_Info {
			static const std::string GROUP_INFO;
			struct Enchant_Prop_Info {
				static const std::string PROP_TYPE;
				static const std::string PROP_VAL;
				static const std::string STAR;
				static const std::string IS_LOCK;
			};
			static const std::string SCORE;
		};
		struct Card_Info {
			static const std::string INDEX;
			static const std::string LV;
			static const std::string ID;
			static const std::string CUR_EXP;
			static const std::string POTEN;
			static const std::string MAIN_ATTR;
			static const std::string SUB_ATTR;
			static const std::string STAR;
			static const std::string PART;
			static const std::string ITEM_ID;
			static const std::string EQUIP_LOC;
			static const std::string EMBED_STATE;
		};
	};

	static const std::string RUNE_PART_INFO;
	struct Rune_Part_Info {
		static const std::string PART;
		static const std::string RUNE_HOLE_INFO;
		struct Rune_Hole_Info {
			static const std::string INDEX;
			static const std::string LEVEL;
			static const std::string ENERGY;
		};
	};

	static const std::string ITEM_RUNE_STONE_INFO;
	struct Item_Rune_Stone_Info {
		static const std::string INDEX;
		static const std::string EXP_LVL;
		static const std::string EXP;
		static const std::string IS_LOCK;
	};

	static const std::string ITEM_MASTER_PART_INFO;
	struct Item_Master_Part_Info {
		static const std::string INDEX;
		static const std::string LEVEL;
		static const std::string LUCK_VALUE;
		static const std::string FAIL_ADD_RATE;
	};
};

struct Rank_Equiper_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string ITEM_ID;
	static const std::string ITEM_INDEX;
	static const std::string ROLE_NAME;
	static const std::string ITEM_NAME;
	static const std::string SCORE;
	static const std::string ITEM_INFO;
	static const std::string ITEM_BASE;
};
//end ranking

struct Daily_Fields {
	static const std::string COLLECTION;
	static const std::string DAILY_LIST;
	static const std::string ACTIVITY_LIST;
	struct Daily_Info {
		static const std::string ID;
		static const std::string NUM;
	};
	struct Activity_Info {
		static const std::string ID;
		static const std::string NUM;
	};
	static const std::string TODAY_ACTIVITY;
	static const std::string ALL_ACTIVITY;
	static const std::string TODAY_RECEIVE;
	static const std::string ALL_RECEIVE;
	static const std::string LAST_ARENA_FORCE;
	static const std::string NJOIN_ARENA_TIP;
	static const std::string RECEIVE_VIP_ACTIVITY;
	static const std::string CHECK_IN_DAY;
	static const std::string LAST_CHECK_IN_REFRESH_TIME;
	static const std::string TODAY_IS_CHECK_IN;
	static const std::string LAST_TIP_FORCE;
};

struct Covenant_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string COVENANT_DATA_LIST;
	struct Covenant_Data {
		static const std::string ID;
		static const std::string INIT_STATUS;
		static const std::string OVER_TIME;
		static const std::string TODAY_BECOME_TIME;
	};
};

struct Vip_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string GROW_VAL;
	static const std::string LAST_SETTLE_GROW;
	static const std::string EXP_OVERTIME;
	static const std::string COST_MONEY;
	static const std::string EXPIRED_PROMPT;
	static const std::string HAS_REC_DAILY;
	static const std::string WORLD_TREE_LIST;
	struct World_Tree_Data {
		static const std::string ID;
		static const std::string COPPER;
		static const std::string SOULS;
		static const std::string DRAGON_SOUL;
		static const std::string RECEIVE_COPPER;
		static const std::string RECEIVE_SOULS;
		static const std::string RECEIVE_DRAGON_SOUL;
	};
};


struct Mount_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string MOUNT_ID;
	static const std::string RIDE;
	static const std::string HORSE_LIST;
	static const std::string GROUP_LIST;
	static const std::string SUM_TRAIN_LV;
	struct Horse_Info {
		static const std::string HORSE_ID;
		static const std::string INVALID;
		static const std::string DEADLINE;
		static const std::string ACTIVATE_TIME;
		static const std::string TRAIN_LV;
		static const std::string TRAIN_NUM;
		static const std::string TRAIN_FAIL_NUM;
		static const std::string DEPLOY;
		static const std::string DEPLOY_SKILL_1;
		static const std::string DEPLOY_SKILL_2;
		static const std::string DEPLOY_SKILL_3;
	};
	struct Group_Info{
		static const std::string GROUP_ID;
		static const std::string STATUS;
	};
};

struct Reward_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string ONLINE_REWAED_NUM;
	static const std::string ONLINE_REWARD_TIME;
	static const std::string ONLINE_REFRESH_REWARD_TIME;
	static const std::string ONLINE_REWAED_OVER;
	static const std::string LOGIN_DAYS;
	static const std::string RECEIVE_SEVEN_DAYS;
	static const std::string ACTIVATION_CODE_RECEIVE_INFO;
	static const std::string RB_ALL_NUM;
	static const std::string RB_ITEM_VALUE;
	static const std::string RB_ITEM_VALUE_TMP;
	static const std::string RB_RECEIVE_TIME;
	static const std::string RB_FIRST_TIME;
	static const std::string KEEP_REWARD_STATUS;
	struct Activation_Code_Receive_Info {
		static const std::string TYPE;
		static const std::string NUM;
	};
	static const std::string KEY;
	static const std::string VALUE;

};

struct Activation_Code_Fields {
	static const std::string COLLECTION;
	static const std::string KEY;
};

struct Activation_Receive_Id_Fields {
	static const std::string COLLECTION;
	static const std::string ID;
};

struct Campfire_Fields{
	static const std::string COLLECTION;
	static const std::string ID;
	static const std::string LEVEL;
	static const std::string GANG_ID;
	static const std::string GANG_NAME;
	static const std::string FIREWOOD_NUM;
	static const std::string BURNING;
	static const std::string LEVEL_FIREWOOD_NUM;
	static const std::string LEVEL_MAX_FIREWOOD_NUM;
};


struct World_Boss_Fields{
	static const std::string COLLECTION;
	static const std::string ACT_ID;
	static const std::string ACT_STATE;
	static const std::string OPEN_NUM;
	static const std::string KILL_NUM;
	static const std::string BOSS_LV;
};

struct FB_Info_Fields{
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string BEST_FORCE;
	static const std::string BEST_ROLE_ID;
	static const std::string BEST_ROLE_NAME;
};

struct Access_Count_Fields{
	static const std::string COLLECTION;
	static const std::string TYPE;
	static const std::string H_TIME;
	static const std::string NUM;
};

struct Active_Content_Fields{
	static const std::string COLLECTION;
	static const std::string VIP_CLUB;
	static const std::string LEVEL_ACE;
	static const std::string SUIT_COLLECT;
	static const std::string GUILD_LEVEL;
	static const std::string GEM_LEVELUP;
	static const std::string WING_LEVELUP;
	static const std::string MOUNT_LEVELUP;
	static const std::string SG_PASS;
	static const std::string MOUNT_PASS;
	static const std::string WING_PASS;
	static const std::string ARENA_DAN;
	static const std::string ARENA_DAN_TIME;
	static const std::string SUIT_CNT;

	static const std::string COM_REC_ID;
	static const std::string ALL_WORLD_BOSS_HURT;
	static const std::string TODAY_WORLD_BOSS_HURT;
	static const std::string WORLD_BOSS_MAX_RANK;
	static const std::string HEROES_DOOR_TYPE_LV;
	static const std::string TEAM_ARENA_WNUM;
	static const std::string WARER_WNUM;
	static const std::string REW_TASK_DONE_NUM;
	static const std::string ON_SEND_MAIL_ACT_ID;
	static const std::string ENCHANTING_NUM;
	static const std::string DRAGON_PREY_NUM;
	static const std::string RING_TASK_RECHARGE;
	static const std::string MOUNT_LVUP_NUM;
	static const std::string FUSION_X_ELITE_CARD_NUM;
	static const std::string ALCHEMY_NUM;
	static const std::string COMMON_LSTIME;
	static const std::string COMMON_TAI;

	//COMMON

	static const std::string KEY;
	static const std::string VAL;
};

struct Active_Content_Info_Fields{
	static const std::string VIP_AN_LV;
	static const std::string LRC;
	static const std::string ARC;
	static const std::string FRC;
	static const std::string SGRC;
	static const std::string MOUNT_COUNT;
	static const std::string WING_COUNT;
	static const std::string LRT;
	static const std::string FRT;
	static const std::string SGRT;
	static const std::string MOUNT_TIME;
	static const std::string WING_TIME;
	static const std::string ADST;
	static const std::string ART;
	static const std::string ADLI;
	static const std::string COM_LSTIME;
	static const std::string LR_ROLE;
	static const std::string AR_ROLE;
	static const std::string FR_ROLE;
	static const std::string SGR_ROLE;
	static const std::string MOUNT_ROLE;
	static const std::string WING_ROLE;
	struct Role_Info {
		static const std::string ROLE_ID;
		static const std::string ROLE_NAME;
		static const std::string HPI;
		static const std::string GENDER;
		static const std::string GUILD_NAME;
		static const std::string CAREER;
		static const std::string LEVEL;
		static const std::string VIP_LEVEL;
		static const std::string COMBAT;
		static const std::string TITLE_ID;
	};
};

struct Common_Fields{
	static const std::string COLLECTION;
	static const std::string TYPE;
	static const std::string VALUE;
	static const std::string VALUE_1;
	static const std::string VALUE_2;
	static const std::string VALUE_3;
};

struct Proposal_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string TYPE;
	static const std::string CONTENTS;
	static const std::string TIME;
	static const std::string REPLY;
};

struct Fashion_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string SHOW_STATE;
	static const std::string FASHION_PART;
	struct Fashion_Part {
		static const std::string ID;
		static const std::string LV;
		static const std::string EXP;
		static const std::string USED_ID;
		static const std::string IDS;
	};
};

struct Gm_Ann_Fields {
	static const std::string COLLECTION;
	static const std::string TYPE;
	static const std::string STIME;
	static const std::string ID;
	static const std::string ENDTIME;
	static const std::string STEP;
	static const std::string CONTENTS;
	static const std::string STATUS;
};

struct Ban_Gag_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string ACCOUNT;
	static const std::string UNBAN_TIME;
};

struct Auction_Goods_Fields {
	static const std::string COLLECTION;
	static const std::string ID;
	static const std::string ITEM_ID;
	static const std::string NUM;
	static const std::string AUCTION_PRICE;
	static const std::string ONES_PRICE;
	static const std::string OVER_TIME;
	static const std::string START_TIME;
	static const std::string CONTINUE_TIME;
	static const std::string ISSUE_ROLE_ID;
	static const std::string AUCTION_ROLE_ID;
	static const std::string L_AUCTION_PRICE;
	static const std::string AUCTION_FAIL_ROLE_ID;
};

struct Knight_Trials_Fields {
	static const std::string COLLECTION;
	static const std::string CUR_POINTS;
	static const std::string ROLE_ID_SET;
	static const std::string MATCH_DAY;
	static const std::string CUR_RECEIVE;
	static const std::string RESET_STATUS;
	static const std::string REWARD;
	static const std::string RECODE_REWARD;
	struct Reward_Info {
		static const std::string ID;
		static const std::string VAL;
		static const std::string TYPE;
	};
};

struct Knight_Trials_Player_Fields {
	static const std::string COLLECTION;
	static const std::string CUR_PLAYER;
	static const std::string HIDE_PLAYER;
	struct Player_Detail {
		static const std::string BASE;
		static const std::string FIGHTER;
		static const std::string HEROER;
		static const std::string UUID;
		static const std::string SERVER_NAME;
	};
};

struct Anci_Artifact_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string PERFECTPOINT;
	static const std::string AA_COMPLETE_IDS;

	static const std::string AA_INFO_MAP;
	struct AA_Info {
		static const std::string ID;
		static const std::string DATA;
		static const std::string STATE_COUNT;
	};

	static const std::string AA_UNBAN_INFO;
	struct AA_Unban_Info {
		static const std::string ID;
		static const std::string STATE;
		static const std::string BOX_STATE;
	};
};

struct Wing_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string LV;
	static const std::string RATE;
	static const std::string PROGRESS;
	static const std::string TIMES;
	static const std::string LAST_TIME;
	static const std::string SHOW_STATE;
	static const std::string STATE;
	static const std::string GOT_REWARD_STAGE;
	static const std::string ACTIVE_TIME;
};

struct Invest_Plan_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string IS_MONTH_CARD_INVEST;
	static const std::string LEFT_DAY;
	static const std::string DAYS;

	static const std::string MONTH_CARD_PROFIT_BUTTON_LIST;
	struct Month_Card_Profit_Button {
		static const std::string  DAY;
		static const std::string STATUS;
	};

	static const std::string INVESTED_MONEY;
	static const std::string INVEST_BUTTON;

	static const std::string UPGRADE_INVEST_PROFIT_BUTTON_LIST;
	struct Upgrade_Invest_Profit_Button {
		static const std::string LEVEL;
		static const std::string STATUS;
	};

	static const std::string CAN_INVEST_MONEY_LIST;
	struct Can_Invest_Money {
		static const std::string MONEY;
	};

	static const std::string INVESTED_MONEY_LIST;
	struct Invested_Money {
		static const std::string MONEY;
	};

	static const std::string GETTED_UPGRADE_INVEST_PROFIT_MAP;
	struct Getted_Upgrade_Invest_Profit_Map {
		static const std::string LEVEL;
		static const std::string GETTED_UPGRADE_INVEST_PROFIT_LIST;
		struct Getted_Upgrade_Invest_Profit {
			static const std::string MONEY;
		};
	};
};

struct Invest_Plan_History_Fields {
	static const std::string COLLECTION;
	static const std::string HISTORY_LIST;
	struct History_Record {
		static const std::string INVEST_TYPE;
		static const std::string HISTORY_TYPE;
		static const std::string MONEY;
		static const std::string PLAYER_NAME;
	};
};

struct ThorLottery_Manager_Fields {
	static const std::string SEASON_ID;
	static const std::string STATE;
	static const std::string ICON_ID;
	static const std::string LV_LIMIT;
	static const std::string THORLOTTERY_LOG;
	struct ThorLottery_Log {
		static const std::string TIME;
		static const std::string ROLE_NAME;
		static const std::string ROLE_ID;
		static const std::string ITEM_ID;
		static const std::string ITEM_NUM;
		static const std::string BIND_STATE;
	};
	static const std::string THORLOTTERR_ITEM;
	struct ThorLottery_Item {
		static const std::string INDEX;
		static const std::string ITEM_ID;
		static const std::string NUM;
	};
};

struct Answer_Info_Fields {
	static const std::string EXT_NUM;
	static const std::string CUR_QNO;
	static const std::string QID_LIST;
	static const std::string ROLE_SORCE_VEC;
	static const std::string ROLE_DATA_VEC;
	static const std::string KEY;
	static const std::string VALUE;
	static const std::string LAST_TIME;
	static const std::string FORCE_RANK;
	static const std::string ARENA_RANK;
	static const std::string GANG_RANK;
	struct Rank_Role_Data {
		static const std::string ROLE_ID;
		static const std::string NAME;
		static const std::string LEVEL;
		static const std::string SORCE;
		static const std::string TRUE_NUM;
		static const std::string EXP;
	};
	struct Role_Info_Data {
		static const std::string FREE_NUM;
		static const std::string DOUBLE_NUM;
		static const std::string CUR_FREE_QID;
		static const std::string CUR_DOUBLE_QID;
	};
};

struct ThorLottery_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string SEASON_ID;
	static const std::string TIME;
	static const std::string SCORE;
	static const std::string DAILY_LOTTERY_TIMES;
	static const std::string THORLOTTERY_ITEM;
	struct ThorLottery_Item {
		static const std::string INDEX;
		static const std::string ITEM_ID;
		static const std::string NUM;
	};
};

struct Activity_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string VIP37_RIDS;
	static const std::string IS_REC_37PHONE;
	static const std::string IS_REC_R2_LOVE_GIFT;
};

struct Questionnaite_Info_Fields {
	static const std::string COLLECTION;
	static const std::string LIST;
	struct List {
		static const std::string QNO;
		static const std::string RADIO;
		static const std::string MULT;
		static const std::string CONTENTS;
	};
};

struct Active_Event_Info_Fields {
	static const std::string COLLECTION;
	static const std::string ID;
	static const std::string START_TIME;
	static const std::string END_TIME;
	static const std::string MID_END_TIME;
};

struct Two_Recharge_Info_Fields {
	static const std::string REC_NUM;
	static const std::string LAST_TIME;
};

struct Inner_Check_Info_Fields {
	static const std::string COLLECTION;
	static const std::string SERVER;
	static const std::string CORE_NUM;
	static const std::string UPDATE_TIME;
	static const std::string MONGO_IP_PORT;
};

struct Expedition_Fields {
	static const std::string COLLECTION;
	static const std::string KILL_BEAST;
	static const std::string KEY;
	static const std::string VAL;
	static const std::string COLLECT_CHEST;
	static const std::string COLLECT_BEAST;
	static const std::string BOX_COLLECT;
	static const std::string ID;
};

struct Expedition_Manager_Detail_Fields {
	static const std::string COLLECTION;
	static const std::string BAK_COLLECTION;
	static const std::string AWARD;
	static const std::string GANG_DATA_INFO_MAP;
	struct Gang_Data_Info_Map {
		static const std::string LINE;
	};
	static const std::string FLAG_TIME;
	static const std::string FIRST_LISTEN_TIME;
};

struct Expedition_Global_Manager_Data_Fields {
	static const std::string COLLECTION;
	static const std::string BAK_COLLECTION;
	static const std::string OPENED_UUID_OPENTIME_MAP;
	static const std::string UUID;
	static const std::string UUID_INTER_MAP;
	static const std::string INTER;
	static const std::string OPENTIME;
};
struct Stronghold_Info_Fields {
	static const std::string OCCUPIER;
	static const std::string SOLDIER_MAP;
	static const std::string NUMS;
};
struct Stronghold_Fields {
	static const std::string TYPE;
	static const std::string INFO;
};
struct Expedition_Scene_Data_Fields {
	static const std::string MAP_DATA;
	static const std::string POINT_GANG_ID;
	static const std::string MAP_BASE_POINT_NUM;
	static const std::string DEMAGE_RANK;
	static const std::string SETTLE_TIME;
	static const std::string OCCUPY_CASTLE_RANK;
	static const std::string OCCUPY_CASTLE_NUMS;
	static const std::string DEMAGE;
	static const std::string GET_NUMS;
	static const std::string SERVER_NAME;
	static const std::string DROP_OR_MATERIAL;
	static const std::string HAD_COLLECTED;
	static const std::string REFRESHED_MONSTER_TIMING;
	static const std::string REFRESHED_MONSTER_KILL_GAP;
};

struct Expedition_Manager_Data_Fields {
	static const std::string COLLECTION;
	static const std::string INTER;
	static const std::string OPENED_UUID_OPENTIME_MAP;
	static const std::string GANG_ID_UUID_MAP;

	static const std::string LAST_RESET_TIME_SEC;
	static const std::string LAST_RESET_TIME_USEC;

	static const std::string FIRST_GANG_RANK;
	static const std::string RANK;

	static const std::string HAS_INIT;
	static const std::string UUID_HAS_INIT;

	static const std::string GANG_RANK_MAP;
	static const std::string GANG_LEADER_NAME;
	static const std::string NUM;
	static const std::string ASSET;
	static const std::string CREATE_TIME;
	static const std::string EXPEDITION_RANK;

	static const std::string UUID_GANG_RANK_MAP;

	static const std::string GANG_RANK_SYNC_SUCCESS;
	static const std::string GANG_RANK_SYNC_NEW_VEC;
	static const std::string UUID_GANG_RANK_SYNC_NEW_VEC;

	static const std::string GANG_ID_POINT_ID;
	static const std::string POINT_ID;

	static const std::string GANG_ID_CUR_LINE;
	static const std::string CUR_LINE;

	static const std::string UUID_GANG_ID_POINT_ID;
	static const std::string UUID_GANG_ID_CUR_LINE;

	static const std::string LINE_SCENE_MAP;
	static const std::string UUID_LINE_SCENE_MAP;
};

struct Activities_Exchange_Stores_Fields {
	static const std::string COLLECTION;//集合名
	static const std::string ID;//ID
	static const std::string ITEM_ID;//物品ID
	static const std::string ITEM_NUM;//
	static const std::string EXCHANGE_ID;//物品ID
	static const std::string EXCHANGE_NUM;//
	static const std::string LIMIT_LV;//
	static const std::string LIMIT_EXCHAGNGE;//
	static const std::string CLASSIFY;//
	static const std::string SUBCLASS;//
	static const std::string ACT_ID;
	static const std::string SORT;//显示顺序
};

struct Activities_Exchange_Stores_Detail_Fields {
	static const std::string COLLECTION;//集合名
	static const std::string ID_NUM_BUY;
};

struct Active_Data_Fields {
	static const std::string  ID;
	static const std::string  NUM;
	static const std::string  BEGIN_TIME;
	static const std::string  END_TIME;
};

struct Life_Skill_Data_Fields {
	static const std::string COLLECTION;
    static const std::string ROLE_ID;
	static const std::string ENERGY;
	static const std::string RE_TIME;
	static const std::string INIL;
	static const std::string LF_SKILL_LIST;
	struct Life_Skill_Data {
		static const std::string SKILL_ID;
		static const std::string DEGREE;
	};
};

struct Oper_Activity_Data_Fields {
	static const std::string COLLECTION;
	static const std::string  LAST_ITEM_START_TIME;//每一项活动ID对应最后一次开始时间
	static const std::string  ACTID_TYPE_RECORD_VAL;//活动ID->类型->完成记录(次数,类型->值)
	static const std::string  ACTID_RECEIVE_ID;//活动ID对应已经领取的ID集合
	static const std::string  ACTID_RECEIVE_ID_NUM;//活动ID->(项目)领取ID->领取次数
	static const std::string  ACTID_APPEND_EFFECT;//活动ID执行过的附加效果
	static const std::string  ACTID_RECID_LAST_REFRESH_TIME;//活动ID->(项目)领取ID->最后一次刷新时间
	static const std::string  ACTID_LOGIN_DAY;//活动ID->login day 时间
	struct Record {
		static const std::string NUM;
		static const std::string EXT1;
		static const std::string EXT2;
		static const std::string EXT3;
		static const std::string EXT4;
		static const std::string TYPE_NUM;
	};
	struct Common {
		static const std::string KEY;
		static const std::string VAL;
	};
};

struct Pet_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string LV;
	static const std::string STATE;
	static const std::string UPGRADE_VAL;
	static const std::string ACTIVE_ILLUSION_ID;
	static const std::string SKILLS;
	struct Pet_Skill {
		static const std::string ID;
		static const std::string LOC;
	};
	static const std::string ILLUSIONS;
};

#endif /* DB_FIELDS_H_ */
