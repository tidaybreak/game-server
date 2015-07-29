/*
 * DB_Field.cpp
 *
 *  Created on: May 8, 2012
 *      Author: Ti
 */

#include "DB_Fields.h"

const std::string Global_Fields::COLLECTION = "mmo.global";
const std::string Global_Fields::ID = "id";
const std::string Global_Fields::KEY = "key";
const std::string Global_Fields::ROLE = "role";
const std::string Global_Fields::TIME = "time";

/// ROLE START ///////////////////////////////////////////////////
const std::string Role_Fields::COLLECTION = "mmo.role";
const std::string Role_Fields::ROLE_ID = "role_id";

const std::string Role_Fields::ACCOUNT_INFO = "account_info";
const std::string Role_Fields::Account_Info::ACCOUNT_NAME = "account_name";
const std::string Role_Fields::Account_Info::AGENT_NUM = "agent_num";
const std::string Role_Fields::Account_Info::SERVER_NUM = "server_num";

const std::string Role_Fields::LOCAL_HIS = "local_his";

const std::string Role_Fields::LOCATION = "location";
const std::string Role_Fields::Location::SCENE_ID = "scene_id";
const std::string Role_Fields::Location::SCENE_SORT = "scene_sort";
const std::string Role_Fields::Location::SCENE_SUB_SORT = "scene_sub_sort";
const std::string Role_Fields::Location::SCENE_ORDER = "scene_order";
const std::string Role_Fields::Location::SCENE_LAYER = "scene_layer";
const std::string Role_Fields::Location::COORD_X = "coord_x";
const std::string Role_Fields::Location::COORD_Y = "coord_y";
const std::string Role_Fields::Location::COORD_TYPE = "coord_type";
const std::string Role_Fields::Location::TOWARD = "toward";

const std::string Role_Fields::ROLE_NAME = "role_name";
const std::string Role_Fields::LEVEL = "level";
const std::string Role_Fields::GENDER = "gender";
const std::string Role_Fields::CAREER = "career";
const std::string Role_Fields::VIP = "vip";
const std::string Role_Fields::VIP_OVER_TIME = "vip_over_time";
const std::string Role_Fields::CREATE_TIME = "create_time";
const std::string Role_Fields::LAST_SIGN_IN_TIME = "last_sign_in_time";
const std::string Role_Fields::LAST_SIGN_OUT_TIME = "last_sign_out_time";
const std::string Role_Fields::LAST_TRIGGER_DAILY_ZERO = "last_trigger_daily_zero";
const std::string Role_Fields::LAST_TRIGGER_DAILY_SIX = "last_trigger_daily_six";
const std::string Role_Fields::LAST_TRIGGER_WEEKLY_ZERO = "last_trigger_weekly_zero";
const std::string Role_Fields::RIDE = "ride";
const std::string Role_Fields::MOUNT_MODEL_ID = "mount_model_id";
const std::string Role_Fields::TITLE_ID = "title_id";
const std::string Role_Fields::ONLINE = "online";
const std::string Role_Fields::IP = "ip";
const std::string Role_Fields::HEAD_ID = "head_id";
const std::string Role_Fields::CHARM = "charm";
const std::string Role_Fields::GANG_ID = "gang_id";
const std::string Role_Fields::GANG_NAME = "gang_name";
const std::string Role_Fields::AWAKE_LVL = "awake_lvl";
const std::string Role_Fields::AVATAR = "avatar";
const std::string Role_Fields::MOUNT_ID = "mount_id";
const std::string Role_Fields::ROLE_TYPE = "role_type";

/// EXAMPLE START ///////////////////////////////////////////////////
const std::string Example_Fields::COLLECTION = "mmo.example";
const std::string Example_Fields::ROLE_ID = "role_id";


/// ICON START/////////////////////////////////////////////////////
const std::string Icon_Fields::COLLECTION = "mmo.icon";
const std::string Icon_Fields::ROLE_ID = "role_id";
const std::string Icon_Fields::ICON = "icon";
const std::string Icon_Fields::Icon::ICON_ID = "icon_id";
const std::string Icon_Fields::Icon::STATUS = "status";
const std::string Icon_Fields::Icon::IS_EVER_OPENED = "is_ever_opened";
const std::string Icon_Fields::Icon::IS_USED_ITEM = "is_used_item";

/// DRAGON VALE START///////////////////////////////////////////////
const std::string Dragon_Vale_Fields::COLLECTION = "mmo.dragon_vale";
const std::string Dragon_Vale_Fields::ROLE_ID = "role_id";
const std::string Dragon_Vale_Fields::TARGET_PLAYER_ID = "target_player_id";
const std::string Dragon_Vale_Fields::IS_ROB_ENTER_DRAGON_VALE = "is_rob_enter_dragon_vale";
const std::string Dragon_Vale_Fields::UUID = "uuid";
const std::string Dragon_Vale_Fields::IS_GOLD_MINE_MAX_STORAGE = "is_gold_mine_max_storage";
const std::string Dragon_Vale_Fields::IS_MOON_WELL_MAX_STORAGE = "is_moon_well_max_storage";
const std::string Dragon_Vale_Fields::IS_TOTEM_MAX_STORAGE = "is_totem_max_storage";
const std::string Dragon_Vale_Fields::BUILD_GOLD_MINE_NUM = "build_gold_mine_num";
const std::string Dragon_Vale_Fields::BUILD_MOON_WELL_NUM = "build_moon_well_num";
const std::string Dragon_Vale_Fields::BUILD_TOTEM_NUM = "build_totem_num";
const std::string Dragon_Vale_Fields::BUILD_QUEUE_NUM = "build_queue_num";
const std::string Dragon_Vale_Fields::ALL_OPEN_BUILD_QUEUE_NUM = "all_open_build_queue_num";
const std::string Dragon_Vale_Fields::IS_DRAGON_BORN = "is_dragon_born";
const std::string Dragon_Vale_Fields::BEGIN_BORN_TIME_SEC = "begin_born_time_sec";
const std::string Dragon_Vale_Fields::BEGIN_PLAY_DRAGON_INTERVAL_TIME_SEC = "begin_play_dragon_interval_time_sec";
const std::string Dragon_Vale_Fields::BEGIN_DOWN_FEELING_TIME_SEC = "begin_down_feeling_time_sec";
const std::string Dragon_Vale_Fields::IS_VIP_DOWN_TO_ZERO = "is_vip_down_to_zero";
const std::string Dragon_Vale_Fields::MATCHED_NUM = "matched_num";
const std::string Dragon_Vale_Fields::BEGIN_REFRESH_THIEF_TIME_SEC = "begin_refresh_thief_time_sec";
const std::string Dragon_Vale_Fields::STAR_NUM = "star_num";
const std::string Dragon_Vale_Fields::AWARD_FOOT_NUM = "award_foot_num";
const std::string Dragon_Vale_Fields::IS_SPECIAL = "is_special";
const std::string Dragon_Vale_Fields::IS_YESTERDAY_LOGIN = "is_yesterday_login";
const std::string Dragon_Vale_Fields::DRAGON_HEART_NUM = "DRAGON_HEART_NUM";
const std::string Dragon_Vale_Fields::IS_ROB_PROTECT = "is_rob_protect";
const std::string Dragon_Vale_Fields::BEGIN_ROB_PROTECT_TIME_SEC = "begin_rob_protect_time_sec";
const std::string Dragon_Vale_Fields::IS_FIRST_ENTER_AFTER_ROBBED = "is_first_enter_after_robbed";
const std::string Dragon_Vale_Fields::IS_IN_ROBBED_DRAGON_VALE = "is_in_robbed_dragon_vale";
const std::string Dragon_Vale_Fields::BEGIN_IN_ROBBED_DRAGON_VALE_TIME_SEC = "begin_in_robbed_dragon_vale_time_sec";
const std::string Dragon_Vale_Fields::IS_DRAGON_VALE_OPEN = "is_dragon_vale_open";
const std::string Dragon_Vale_Fields::BEGIN_NO_ENTER_TIME = "begin_no_enter_time";
const std::string Dragon_Vale_Fields::LEFT_TIME = "left_time";
const std::string Dragon_Vale_Fields::PLAY_DRAGON_NUM = "play_dragon_num";
const std::string Dragon_Vale_Fields::DRIVE_THIEF_NUM = "drive_thief_num";


const std::string Dragon_Vale_Fields::BUILDING_LIST = "building_list";
const std::string Dragon_Vale_Fields::Building::ID = "id";
const std::string Dragon_Vale_Fields::Building::TYPE = "type";
const std::string Dragon_Vale_Fields::Building::LEVEL = "level";
const std::string Dragon_Vale_Fields::Building::STATUS = "status";
const std::string Dragon_Vale_Fields::Building::PRODUCTION = "production";
const std::string Dragon_Vale_Fields::Building::IS_GAIN = "is_gain";
const std::string Dragon_Vale_Fields::Building::BEGIN_UPGRADE_TIME_SEC = "begin_upgrade_time_sec";
const std::string Dragon_Vale_Fields::Building::BEGIN_UPGRADE_TIME_USEC = "begin_upgrade_time_usec";
const std::string Dragon_Vale_Fields::Building::LAST_OUTPUT_PRODUCTION_TIME_SEC = "last_output_production_time_sec";
const std::string Dragon_Vale_Fields::Building::LAST_OUTPUT_PRODUCTION_TIME_USEC = "last_output_production_time_usec";
const std::string Dragon_Vale_Fields::Building::ORDER = "order";
const std::string Dragon_Vale_Fields::Building::CONSUME_TYPE = "consume_type";

const std::string Dragon_Vale_Fields::CANCEL_CREATE_BUILDING_LIST = "cancel_create_building_list";
const std::string Dragon_Vale_Fields::Cancel_Create_Building::ORDER = "order";

const std::string Dragon_Vale_Fields::DRAGON_LIST = "dragon_list";
const std::string Dragon_Vale_Fields::Dragon::PROGRESS = "progress";
const std::string Dragon_Vale_Fields::Dragon::FEELING = "feeling";
const std::string Dragon_Vale_Fields::Dragon::POWER = "power";
const std::string Dragon_Vale_Fields::Dragon::ARMOR = "armor";
const std::string Dragon_Vale_Fields::Dragon::STAMINA = "stamina";
const std::string Dragon_Vale_Fields::Dragon::POWER_LEVEL = "power_level";
const std::string Dragon_Vale_Fields::Dragon::ARMOR_LEVEL = "armor_level";
const std::string Dragon_Vale_Fields::Dragon::STAMINA_LEVEL = "stamina_level";
const std::string Dragon_Vale_Fields::Dragon::HERO_POWER = "hero_power";
const std::string Dragon_Vale_Fields::Dragon::HERO_ARMOR = "hero_armor";
const std::string Dragon_Vale_Fields::Dragon::HERO_STAMINA = "hero_stamina";
const std::string Dragon_Vale_Fields::Dragon::HERO_POWER_LEVEL = "hero_power_level";
const std::string Dragon_Vale_Fields::Dragon::HERO_ARMOR_LEVEL = "hero_armor_level";
const std::string Dragon_Vale_Fields::Dragon::HERO_STAMINA_LEVEL = "hero_stamina_level";
const std::string Dragon_Vale_Fields::Dragon::IS_PLAY_DRAGON = "is_play_dragon";
const std::string Dragon_Vale_Fields::Dragon::ENERGY = "energy";
const std::string Dragon_Vale_Fields::Dragon::CRIT = "crit";
const std::string Dragon_Vale_Fields::Dragon::PREVENT_CRIT = "prevent_crit";
const std::string Dragon_Vale_Fields::Dragon::CRIT_LEVEL = "crit_level";
const std::string Dragon_Vale_Fields::Dragon::PREVENT_CRIT_LEVEL = "prevent_crit_level";
const std::string Dragon_Vale_Fields::Dragon::HERO_CRIT = "hero_crit";
const std::string Dragon_Vale_Fields::Dragon::HERO_PREVENT_CRIT = "hero_prevent_crit";
const std::string Dragon_Vale_Fields::Dragon::HERO_CRIT_LEVEL = "hero_crit_level";
const std::string Dragon_Vale_Fields::Dragon::HERO_PREVETN_CRIT_LEVEL = "hero_prevent_crit_level";

const std::string Dragon_Vale_Fields::THIEF_LIST = "thief_list";
const std::string Dragon_Vale_Fields::Thief::CREATER = "creater";
const std::string Dragon_Vale_Fields::Thief::ID = "id";

const std::string Dragon_Vale_Fields::FOOT_INFO_LIST = "foot_info_list";
const std::string Dragon_Vale_Fields::Foot_Info::ID = "id";
const std::string Dragon_Vale_Fields::Foot_Info::SEX = "sex";
const std::string Dragon_Vale_Fields::Foot_Info::NAME = "name";

const std::string Dragon_Vale_Fields::FOOT_ID_LIST = "foot_id_list";
const std::string Dragon_Vale_Fields::Foot_ID::ID = "id";

const std::string Dragon_Vale_Fields::NOTICE_BOARD_LIST = "notice_board_list";
const std::string Dragon_Vale_Fields::Notice_Board::TYPE = "type";
const std::string Dragon_Vale_Fields::Notice_Board::TIME_SEC = "time_sec";
const std::string Dragon_Vale_Fields::Notice_Board::HOST = "host";
const std::string Dragon_Vale_Fields::Notice_Board::TARGET = "target";
const std::string Dragon_Vale_Fields::Notice_Board::DRAGON_SOUL = "dragon_soul";
const std::string Dragon_Vale_Fields::Notice_Board::COIN = "coin";
const std::string Dragon_Vale_Fields::Notice_Board::SOUL = "soul";
const std::string Dragon_Vale_Fields::Notice_Board::IS_GET_DRAGON_HEART  = "is_get_dragon_heart";
const std::string Dragon_Vale_Fields::Notice_Board::PARAM1 = "param1";


/// ACHIEVEMENT START //////////////////////////////////////////////
const std::string Achievement_Fields::COLLECTION = "mmo.achievement";
const std::string Achievement_Fields::ROLE_ID = "role_id";
const std::string Achievement_Fields::ACHIEVEMENT_COUNT = "achievement_count";
const std::string Achievement_Fields::ACHIEVEMENT_COMPLETE_IDS = "complete_ids";
const std::string Achievement_Fields::ACHIEVEMENT_SUIT_PURPLE_IDS = "suit_purple_ids";
const std::string Achievement_Fields::ACHIEVEMENT_SUIT_GOLD_IDS = "suit_gold_ids";
const std::string Achievement_Fields::ACHIEVEMENT_RUNE_LVL_HIS = "rune_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_UNBAN_LVL_HIS = "unban_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_MASTER_LVL_HIS = "master_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_FORCE_LVL_HIS = "force_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_WAR_KILL_NUMS_HIS = "war_kill_nums_his";
const std::string Achievement_Fields::ACHIEVEMENT_ARENA_WIN_NUMS_HIS = "arena_win_nums_his";
const std::string Achievement_Fields::ACHIEVEMENT_GANG_WAR_WIN_NUMS_HIS = "gang_war_win_nums_his";
//const std::string Achievement_Fields::ACHIEVEMENT_FIRST_LVL_ROLE_ID_HIS = "first_lvl_role_id_his";
//const std::string Achievement_Fields::ACHIEVEMENT_FIRST_LVL_CK_HIS = "first_lvl_ck_his";
//const std::string Achievement_Fields::ACHIEVEMENT_FIRST_LVL_ZS_HIS = "first_lvl_zs_his";
//const std::string Achievement_Fields::ACHIEVEMENT_FIRST_LVL_YSS_HIS = "first_lvl_yss_his";
const std::string Achievement_Fields::ACHIEVEMENT_KILL_MONSTER_NUMS_HIS = "kill_monster_nums_his";
const std::string Achievement_Fields::ACHIEVEMENT_FB_COMPLETE_NUMS_HIS = "fb_complete_nums_his";
const std::string Achievement_Fields::ACHIEVEMENT_HORSE_GET_HIS = "horse_get_his";
const std::string Achievement_Fields::ACHIEVEMENT_SECRET_SHOP_BUY_NUMS_HIS = "secret_shop_buy_nums_his";
const std::string Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_ROB_TIMES_HIS = "dragon_wale_rob_times_his";
const std::string Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_STEP_FOOT_TIMES_HIS = "dragon_wale_step_foot_times_his";
const std::string Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_PLAY_DRAGON_TIMES_HIS = "dragon_wale_play_dragon_times_his";
const std::string Achievement_Fields::ACHIEVEMENT_HERO_AWAKE_LVL_HIS = "hero_awake_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_TASK_COMPLETE_NUMS_HIS = "task_complete_nums_his";
const std::string Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_GOLD_MINE_LVL_HIS = "dragon_wale_gold_mine_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_MOON_WELL_LVL_HIS = "dragon_wale_moon_well_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_TOTEM_LVL_HIS = "dragon_wale_totem_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_DRIVE_THIEF_TIMES_HIS = "dragon_wale_drive_thief_times_his";
const std::string Achievement_Fields::ACHIEVEMENT_WAR_LIGHT_WIN_NUMS_HIS = "war_light_win_nums_his";
const std::string Achievement_Fields::ACHIEVEMENT_WAR_DRAK_WIN_NUMS_HIS = "war_drak_win_nums_his";
const std::string Achievement_Fields::ACHIEVEMENT_TASK_LIMIT_TIME_HIS = "task_limit_time_his";
const std::string Achievement_Fields::ACHIEVEMENT_KILL_BOSS_HIS = "kill_boss_his";
const std::string Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_KEEP_LEVEL_HIS = "dragon_wale_keep_level_his";
const std::string Achievement_Fields::ACHIEVEMENT_RUNE_STONE_LVL_HIS = "rune_stone_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_ARENA_LVL_HIS = "arena_lvl_his";
const std::string Achievement_Fields::ACHIEVEMENT_FIRST_BOSS_AGAINST = "first_boss_against";
const std::string Achievement_Fields::ACHIEVEMENT_COMPLETE_SUIT = "complete_suit";

const std::string Achievement_Fields::ACHIEVEMENT_INFO_MAP = "achievement_info";
const std::string Achievement_Fields::Achievement_Info::ID = "id";
const std::string Achievement_Fields::Achievement_Info::DATA = "data";
const std::string Achievement_Fields::Achievement_Info::STATE_COUNT = "state_count";
const std::string Achievement_Fields::Achievement_Info::STATE_AWARD = "state_award";
// Title Start
const std::string Title_Fields::COLLECTION = "mmo.title";
const std::string Title_Fields::ROLE_ID = "role_id";
const std::string Title_Fields::TITLE_INFO = "title_info";
const std::string Title_Fields::Title_Info::ID = "id";
const std::string Title_Fields::Title_Info::LAST_TIME = "last_time";
const std::string Title_Fields::Title_Info::START_TIME = "start_time";
const std::string Title_Fields::Title_Info::LAST_END_TIME = "last_end_time";
const std::string Title_Fields::EQUIP_TITLE = "equip_title_info";
// Title End
// Equiper Start
const std::string Equiper_Fields::COLLECTION = "mmo.equiper";
const std::string Equiper_Fields::ROLE_ID = "role_id";
const std::string Equiper_Fields::GEM_ID = "id";
const std::string Equiper_Fields::GEM_ENABLE_INFO = "gem_enable_info";
const std::string Equiper_Fields::MASTER_PART_INFO = "master_part_info";
const std::string Equiper_Fields::Master_Part_Info_Detail::INDEX = "index";
const std::string Equiper_Fields::Master_Part_Info_Detail::LEVEL = "level";
const std::string Equiper_Fields::Master_Part_Info_Detail::LUCK_VALUE = "luck_value";
const std::string Equiper_Fields::Master_Part_Info_Detail::FAIL_CNT = "fail_cnt";
const std::string Equiper_Fields::CARVE_LVL = "carve_lvl";
const std::string Equiper_Fields::GEM_PROGRESS = "gem_progress";
// Equiper End

// Title_Record Start
const std::string Title_Record_Fields::COLLECTION = "mmo.title_record";
const std::string Title_Record_Fields::GANG_WAR_FIRST_LEADER = "gang_war_first_leader";
const std::string Title_Record_Fields::CHARTS_TITLE_MAP = "charts_title_map";

const std::string Title_Record_Fields::CHARTS_INFO = "charts_info";
const std::string Title_Record_Fields::ROLE_ID = "role_id";
const std::string Title_Record_Fields::CHARTS_TYPE = "charts_type";
const std::string Title_Record_Fields::TITLE_ID = "title_id";
// Title_Record End

// Exchange_Detail_Fields Start
const std::string Exchange_Detail_Fields::COLLECTION = "mmo.exchange";
const std::string Exchange_Detail_Fields::EX_DATA = "ex_data";
const std::string Exchange_Detail_Fields::EX_DATA_EDAY = "ex_data_eday";
const std::string Exchange_Detail_Fields::KEY = "k";
const std::string Exchange_Detail_Fields::VALUE = "v";
// Exchange_Detail_Fields End

/// PUBLIC START ///////////////////////////////////////////////////
const std::string Public_Fields::COLLECTION = "mmo.public";
const std::string Public_Fields::ROLE_ID = "role_id";
const std::string Public_Fields::PLOT_INFO = "plot_info";
const std::string Public_Fields::PLOT_ID = "plot_id";
const std::string Public_Fields::PLOT_STEP = "plot_step";
const std::string Public_Fields::PLOT_SWITCH = "plot_switch";
const std::string Public_Fields::PLOT_NUM = "plot_num";
const std::string Public_Fields::ADD_UP_ONLINE_TIME = "add_up_online_time";
const std::string Public_Fields::FCM = "fcm";
const std::string Public_Fields::WAR_ENTER_NUM = "war_enter_num";
const std::string Public_Fields::WAR_MERIT = "war_merit";
const std::string Public_Fields::MERIT = "merit";
const std::string Public_Fields::WAR_FORBID_TIME = "war_forbid_time";
const std::string Public_Fields::ADD_CAMPFIRE = "add_campfire";
const std::string Public_Fields::CAMPFIRE_ID = "campfire_id";
const std::string Public_Fields::CAMPFIRE_FINISH_TIME = "campfire_finish_time";
const std::string Public_Fields::CAMPFIRE_RANK = "campfire_rank";
const std::string Public_Fields::CAMPFIRE_KILLED_MATCH_MAN = "killed_match_man";
const std::string Public_Fields::CAMPFIRE_LIGHTER = "campfire_lighter";
const std::string Public_Fields::CAMPFIRE_RECEIVESTAT = "campfire_recieve_stat";
const std::string Public_Fields::CAMPFIRE_RANK_RCV_STAT = "campfire_rank_rev_stat";

const std::string Public_Fields::WAR_RESULT = "war_result";
const std::string Public_Fields::WAR_RANK = "war_rank";
const std::string Public_Fields::WAR_KILL_NUM = "war_kill_num";
const std::string Public_Fields::WAR_COE = "war_coe";
const std::string Public_Fields::WORSHIPS = "worships";

const std::string Public_Fields::NOTICE_ID = "notice_id";
const std::string Public_Fields::IS_PLAYED_EFFECT = "is_played_effect";
const std::string Public_Fields::IS_SHOW = "is_show";
const std::string Public_Fields::IS_ROPG = "is_ropg";

const std::string Public_Fields::GUIDE_LIST = "guide_list";
const std::string Public_Fields::Guide::GUIDE_ID = "guide_id";
const std::string Public_Fields::Guide::USED_NUM = "used_num";
const std::string Public_Fields::GUIDE_STEP_LIST = "guide_step_List";
const std::string Public_Fields::Guide_Step::GUIDE_ID  = "guide_id";
const std::string Public_Fields::Guide_Step::STEP = "step";
const std::string Public_Fields::Guide_Step::NUM = "num";
const std::string Public_Fields::PLAYED_PLOT_LIST = "played_plot_list";
const std::string Public_Fields::Played_Plot::PLOT_ID = "plot_id";
const std::string Public_Fields::SYS_CFG = "sys_cfg";
const std::string Public_Fields::SysCfg::IS_TOTAL_OPEN = "is_total_open";
const std::string Public_Fields::SysCfg::IS_OPEN_MUSIC = "is_open_music";
const std::string Public_Fields::SysCfg::MUSIC_NUM = "music_num";
const std::string Public_Fields::SysCfg::IS_OPEN_VOICE = "is_open_voice";
const std::string Public_Fields::SysCfg::VOICE_NUM = "voice_num";
const std::string Public_Fields::SysCfg::IS_HIDE_ROLE_MODEL = "is_hide_role_model";
const std::string Public_Fields::SysCfg::IS_HIDE_HERO_MODEL = "is_hide_hero_model";
const std::string Public_Fields::SysCfg::IS_HIDE_TITLE = "is_hide_title";
const std::string Public_Fields::SysCfg::PLAYER_COUNT = "player_count";
const std::string Public_Fields::SysCfg::IS_SKIP_PLOT = "is_skip_plot";
const std::string Public_Fields::AWAKE_LVL = "awake_lvl";
const std::string Public_Fields::ARTIFACTS = "artifacts";
const std::string Public_Fields::Artifact::ARTIFACT_ID = "artifact_id";
const std::string Public_Fields::Artifact::ARTIFACT_LVL = "artifact_lvl";
const std::string Public_Fields::FORNOTICE_GIFTS_STATUS = "fornotice_gifts_status";
const std::string Public_Fields::Fornotice_Gifts_Status::GIFT_LVL = "gift_lvl";
const std::string Public_Fields::Fornotice_Gifts_Status::HAS_RECEIVED = "has_received";
const std::string Public_Fields::RECEIVE_FLOWERS_LIST = "receive_flowers_list";
const std::string Public_Fields::Receive_Flowers_List::ROLE_ID = "role_id";
const std::string Public_Fields::Receive_Flowers_List::ROLE_NAME = "role_name";
const std::string Public_Fields::Receive_Flowers_List::ITEM_ID = "item_id";
const std::string Public_Fields::Receive_Flowers_List::CHARM = "charm";
const std::string Public_Fields::Receive_Flowers_List::FRIEND_NUM = "friend_num";
const std::string Public_Fields::Receive_Flowers_List::MSG = "msg";
const std::string Public_Fields::EXPLOIT_VAL = "exploit_val";
const std::string Public_Fields::EXPLOIT_LVL = "exploit_lvl";
const std::string Public_Fields::FIRST_RECHARGE = "first_recharge";
const std::string Public_Fields::TWO_RECHARGE = "two_recharge";
const std::string Public_Fields::LIMIT_TIME_RECHARGE = "limit_time_recharge";
const std::string Public_Fields::LAST_TWO_RECHARGE_STIME = "last_two_recharge_stime";
const std::string Public_Fields::LAST_LIMIT_TIME_RECHARGE_STIME = "last_limit_time_recharge_stime";
const std::string Public_Fields::RECHARGE_MONEY = "recharge_money";
const std::string Public_Fields::RECHARGE_MONEY_HIS = "recharge_money_his";
const std::string Public_Fields::RECHARGE_LSTIME = "recharge_lstime";
const std::string Public_Fields::RECHARGE_FEEDBACK_LIST = "rechare_feedback_list";
const std::string Public_Fields::ANS_QID = "ans_qid";
const std::string Public_Fields::INV_TIME = "inv_time";
const std::string Public_Fields::REC_INV_TIME = "rec_inv_time";
const std::string Public_Fields::IS_RECEIVE_INV = "is_receive_inv";
const std::string Public_Fields::IS_RECEIVE_LIKE = "is_receive_like";
const std::string Public_Fields::TODAY_REC_BG = "today_rec_bg";

const std::string Public_Fields::IS_MAGIC_WING_OVER_TIME = "is_magic_wing_over_time";
const std::string Public_Fields::IS_MAGIC_WING_READED = "is_magic_wing_readed";
const std::string Public_Fields::VIP_OVER_TIME = "vip_over_time";
const std::string Public_Fields::IS_VIP_READED = "is_vip_readed";
const std::string Public_Fields::GENIUS_OVER_TIME = "genius_over_time";
const std::string Public_Fields::IS_GENIUS_READED = "is_genius_readed";
const std::string Public_Fields::TASK_GUIDE_ID = "task_guide_id";
const std::string Public_Fields::VIP_UPGRADE_CARDS = "vip_upgrade_cards";
const std::string Public_Fields::START_FIRST_RECHARGE_TIME = "start_first_recharge_time";

/// CHAT START    ///////////////////////////////////////////////////
const std::string Chat_Operating_Fields::PRIVATE_CHAT_COLLECTION = "mmo.chat_record";
const std::string Chat_Operating_Fields::GANG_CHAT_RECORD_COLLECTION = "mmo.gang_chat_record";
const std::string Chat_Operating_Fields::ROLE_ID = "role_id";
const std::string Chat_Operating_Fields::RECORDS = "records";
const std::string Chat_Operating_Fields::CHAT_TYPE = "chat_type";
const std::string Chat_Operating_Fields::TIME = "time";
const std::string Chat_Operating_Fields::RECORD = "record";
const std::string Chat_Operating_Fields::Record::CHAT_TYPE = "chat_type";
const std::string Chat_Operating_Fields::Record::CONTENT = "content";
const std::string Chat_Operating_Fields::Record::TYPE = "type";
const std::string Chat_Operating_Fields::Record::ROLE = "role";
const std::string Chat_Operating_Fields::Record::ROLE_ROLE_ID = "role_role_id";
const std::string Chat_Operating_Fields::Record::ROLE_ROLE_NAME = "role_role_name";
const std::string Chat_Operating_Fields::Record::ROLE_GENDER = "role_gender";
const std::string Chat_Operating_Fields::Record::ROLE_GUILD_NAME = "role_guild_name";
const std::string Chat_Operating_Fields::Record::ROLE_CAREER = "role_cateer";
const std::string Chat_Operating_Fields::Record::ROLE_LEVEL = "role_level";
const std::string Chat_Operating_Fields::Record::ROLE_VIP_LEVEL = "role_vip_level";
const std::string Chat_Operating_Fields::Record::ROLE_COMBAT = "role_combat";
const std::string Chat_Operating_Fields::Record::ITEM = "item";
const std::string Chat_Operating_Fields::Record::ITEM_ID = "item_id";
const std::string Chat_Operating_Fields::Record::ITEM_AMOUNT = "item_amount";
const std::string Chat_Operating_Fields::Record::ITEM_BIND = "item_bind";
const std::string Chat_Operating_Fields::TARGET = "target";


/// EXAMPLE START ///////////////////////////////////////////////////
const std::string Operating_Fields::COLLECTION = "mmo.operating";
const std::string Operating_Fields::ROLE_ID = "role_id";
const std::string Operating_Fields::MAIL_LIST = "mail_list";
const std::string Operating_Fields::Mail_List::RECEIVER_ID = "receiver_id";
const std::string Operating_Fields::Mail_List::SENDER_ID = "sender_id";
const std::string Operating_Fields::Mail_List::SEND_TYPE = "send_type";
const std::string Operating_Fields::Mail_List::SENDER_NAME = "send_name";
const std::string Operating_Fields::Mail_List::GOLD = "gold";
const std::string Operating_Fields::Mail_List::TITLE = "title";
const std::string Operating_Fields::Mail_List::CONTENT = "content";
const std::string Operating_Fields::Mail_List::ITEM = "item";
const std::string Operating_Fields::Mail_List::SHOW_SEND_TIME = "show_send_time";
const std::string Operating_Fields::ADD_F_INVITE_LIST = "add_f_invite_list";
const std::string Operating_Fields::HANDEL_F_APPLY_LIST = "handel_f_apply_list";
const std::string Operating_Fields::Friend_List::ROLE_ID = "role_id";
const std::string Operating_Fields::Friend_List::TIME = "time";
const std::string Operating_Fields::Friend_List::HANDEL = "handel";
const std::string Operating_Fields::GANGER_INFO = "ganger_info";
const std::string Operating_Fields::Ganger_Info::GANG_ID = "gang_id";
const std::string Operating_Fields::Ganger_Info::LAST_GANG_ID = "last_gang_id";
const std::string Operating_Fields::Ganger_Info::DAY_CONTRIBUTE = "day_contribute";
const std::string Operating_Fields::Ganger_Info::WEEK_CONTRIBUTE = "week_contribute";
const std::string Operating_Fields::Ganger_Info::DAY_GOLD_CONTRIB_TIMES = "day_gold_contrib_times";
const std::string Operating_Fields::Ganger_Info::DAY_DIAM_CONTRIB_TIMES = "day_diam_contrib_times";
const std::string Operating_Fields::Ganger_Info::AUTHORITY = "authority";
const std::string Operating_Fields::Ganger_Info::JOIN_TIME = "join_time";
const std::string Operating_Fields::Ganger_Info::LEAVE_TIME = "leave_time";
const std::string Operating_Fields::Ganger_Info::HEADSHIP = "headship";
const std::string Operating_Fields::ARENA_THREE_DAY_REWARD = "arena_three_day_reward";
const std::string Operating_Fields::ARENA_SEVEN_DAY_REWARD = "arena_seven_day_reward";
const std::string Operating_Fields::Arena_Reward::RANK = "rank";
const std::string Operating_Fields::Arena_Reward::RANK_TIME = "rank_time";

const std::string Operating_Fields::RECEIVE_FLOWERS_LIST = "receive_flowers_list";
const std::string Operating_Fields::Receive_Flowers_List::ROLE_ID = "role_id";
const std::string Operating_Fields::Receive_Flowers_List::ROLE_NAME = "role_name";
const std::string Operating_Fields::Receive_Flowers_List::ITEM_ID = "item_id";
const std::string Operating_Fields::Receive_Flowers_List::CHARM = "charm";
const std::string Operating_Fields::Receive_Flowers_List::FRIEND_NUM = "friend_num";
const std::string Operating_Fields::Receive_Flowers_List::MSG = "msg";

const std::string Operating_Fields::DRAGON_VALE_BUILDING_NOTICE_LIST = "dragon_vale_building_notice_list";
const std::string Operating_Fields::Dragon_Vale_Building_Notice::NOTICE_TYPE = "notice_type";
const std::string Operating_Fields::Dragon_Vale_Building_Notice::GOLD_MINE = "gold_mine";
const std::string Operating_Fields::Dragon_Vale_Building_Notice::MOON_WELL = "moon_well";
const std::string Operating_Fields::Dragon_Vale_Building_Notice::TOTEM = "totem";

const std::string Operating_Fields::DRAGON_VALE_ROB_FIGHT_RESULT_LIST = "dragon_vale_rob_fight_result_list";
const std::string Operating_Fields::Dragon_Vale_Rob_Fight_Result::NOTICE_TYPE = "notice_type";
const std::string Operating_Fields::Dragon_Vale_Rob_Fight_Result::SERVER_NAME = "server_name";
const std::string Operating_Fields::Dragon_Vale_Rob_Fight_Result::PLAYER_NAME = "player_name";
const std::string Operating_Fields::Dragon_Vale_Rob_Fight_Result::RESULT = "result";

const std::string Operating_Fields::DRAGON_VALE_GAIN_ROB_PRODUCTION_LIST = "dragon_vale_gain_rob_production_list";
const std::string Operating_Fields::Dragon_Vale_Gain_Rob_Production::SERVER_NAME = "server_name";
const std::string Operating_Fields::Dragon_Vale_Gain_Rob_Production::PLAYER_NAME = "player_name";
const std::string Operating_Fields::Dragon_Vale_Gain_Rob_Production::GOLD_MINE = "gold_mine";
const std::string Operating_Fields::Dragon_Vale_Gain_Rob_Production::MOON_WELL = "moon_well";
const std::string Operating_Fields::Dragon_Vale_Gain_Rob_Production::TOTEM = "totem";

const std::string Operating_Fields::IS_DRAGON_VALE_SPECIAL = "is_dragon_vale_special";
const std::string Operating_Fields::INV_TIME = "inv_time";
const std::string Operating_Fields::LIKE_SUCCESS = "like_success";
const std::string Operating_Fields::DAMAGE = "damage";
const std::string Operating_Fields::DAMAGE_RATE = "damage_rate";
const std::string Operating_Fields::WBRANK = "wbrank";

/// FIGHTER START ///////////////////////////////////////////////////
const std::string Fighter_Fields::COLLECTION = "mmo.fighter";
const std::string Fighter_Fields::ROLE_ID = "role_id";
const std::string Fighter_Fields::PHY_POWER_CUR = "phy_power_cur";
const std::string Fighter_Fields::PHY_POWER_RECOVER_TIME = "phy_power_recover_time";
const std::string Fighter_Fields::PHY_POWER_BUY_TIMES = "phy_power_buy_times";
const std::string Fighter_Fields::LEVEL = "level";
const std::string Fighter_Fields::TTL_SKILL_POINTER = "ttl_skill_pointer";
const std::string Fighter_Fields::CURRENT_BLOOD = "current_blood";
const std::string Fighter_Fields::CURRENT_MORALE = "current_morale";
const std::string Fighter_Fields::CURRENT_STRENGTH = "current_strength";
const std::string Fighter_Fields::EXPERIENCE = "experience";
const std::string Fighter_Fields::ELITE_BTL_MORALE = "elite_btl_morale";
const std::string Fighter_Fields::CURRENT_BLOOD_POOL = "current_blood_pool";
const std::string Fighter_Fields::FORCE = "force";
const std::string Fighter_Fields::HIGHEST_FORCE = "highest_force";
const std::string Fighter_Fields::TTL_FORCE = "ttl_force";
const std::string Fighter_Fields::DEFAULT_SKILL = "default_skill";
const std::string Fighter_Fields::CUR_USED_TALENT = "cur_used_talent";
const std::string Fighter_Fields::RESET_SKILL_TIMES = "reset_skill_times";
const std::string Fighter_Fields::IS_CLEAR_MORAL = "is_clear_moral";
const std::string Fighter_Fields::TALENT = "talent";

const std::string Fighter_Fields::Talent::TALENT_ID = "talent_id";
const std::string Fighter_Fields::Talent::SKILL_POINTER = "skill_pointer";
const std::string Fighter_Fields::Talent::SKILL = "skill";
const std::string Fighter_Fields::Talent::Skill::SKILL_ID = "skill_id";
const std::string Fighter_Fields::Talent::Skill::SKILL_LEVEL = "skill_level";
const std::string Fighter_Fields::Talent::Skill::SkILL_LOC = "skill_loc";

//arena
const std::string Fighter_Fields::ATTRIBUTE = "attribute";

const std::string Fighter_Fields::Attribute::AT_BASIC = "basic";
const std::string Fighter_Fields::Attribute::AT_EQUIP = "equip";
const std::string Fighter_Fields::Attribute::AT_CONVERSION = "conversion";
const std::string Fighter_Fields::Attribute::AT_INITIATIVE_SKILL = "initiative_skill";
const std::string Fighter_Fields::Attribute::AT_STATUS = "status";
const std::string Fighter_Fields::Attribute::AT_PASSIVE_SKILL = "passive_skill";
const std::string Fighter_Fields::Attribute::AT_RUNE_STONE = "rune_stone";
//const std::string Fighter_Fields::Attribute::AT_RUNE = "rune";
const std::string Fighter_Fields::Attribute::AT_HERO_ASSIST = "hero_assist";
const std::string Fighter_Fields::Attribute::AT_GANG = "gang";
const std::string Fighter_Fields::Attribute::AT_ARTIFACTS = "artifacts";
const std::string Fighter_Fields::Attribute::AT_ACHIEVEMENT = "achievement";
const std::string Fighter_Fields::Attribute::AT_TITLE = "title";
const std::string Fighter_Fields::Attribute::AT_DRAGON_VALE = "dragon_vale";
const std::string Fighter_Fields::Attribute::AT_ANCI_ARTIFACT = "anci_artifact";
const std::string Fighter_Fields::Attribute::AT_WING = "wing";
const std::string Fighter_Fields::Attribute::AT_MOUNT = "mount";

const std::string Fighter_Fields::Attribute::Attr_Part::BASE = "base";
const std::string Fighter_Fields::Attribute::Attr_Part::FIX = "fix";
const std::string Fighter_Fields::Attribute::Attr_Part::PERCENT = "percent";

const std::string Fighter_Fields::ARENA_FIGHT = "arena_fight";


const std::string Fighter_Fields::Arena_Fight::PT_PHY_ATTACK = "physical_attack";
const std::string Fighter_Fields::Arena_Fight::PT_SPELL_ATTACK = "spell_attack";
const std::string Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE = "physical_defense";
const std::string Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE = "spell_defense";
const std::string Fighter_Fields::Arena_Fight::PT_HIT = "hit_rate";
const std::string Fighter_Fields::Arena_Fight::PT_AVOID = "avoid";
const std::string Fighter_Fields::Arena_Fight::PT_IGNORE_DEF = "ignore_def";
const std::string Fighter_Fields::Arena_Fight::PT_BLOOD_MAX = "blood_max";
const std::string Fighter_Fields::Arena_Fight::PT_CRIT = "crit";
const std::string Fighter_Fields::Arena_Fight::PT_CRIT_DEF = "crit_def";
const std::string Fighter_Fields::Arena_Fight::PT_CURE = "cure";
const std::string Fighter_Fields::Arena_Fight::PT_CURE_RATE = "cure_rate";
const std::string Fighter_Fields::Arena_Fight::PT_SING_RATE = "sing_rate";
const std::string Fighter_Fields::Arena_Fight::PT_INIT_MORALE = "init_morale";
const std::string Fighter_Fields::Arena_Fight::PT_MORALE_CAPS = "morale_caps";
const std::string Fighter_Fields::Arena_Fight::PT_CRIT_HURT = "crit_hurt";
const std::string Fighter_Fields::Arena_Fight::PT_CRIT_HURT_SUB = "crit_hurt_sub";
const std::string Fighter_Fields::Arena_Fight::PT_HIT_RATE_ADDITION = "hit_rate_addition";
const std::string Fighter_Fields::Arena_Fight::PT_AVOID_RATE_ADDITION = "avoid_rate_addition";
const std::string Fighter_Fields::Arena_Fight::PT_CRIT_RATE_ADDITION = "crit_rate_addition";
const std::string Fighter_Fields::Arena_Fight::PT_CRIT_DEF_RATE_ADDITION = "crit_def_rate_addition";
const std::string Fighter_Fields::Arena_Fight::PT_SPEED = "speed";
const std::string Fighter_Fields::Arena_Fight::PT_BLOOD_CURRENT = "blood_current";
const std::string Fighter_Fields::Arena_Fight::PT_MORALE_CURRENT = "morale_current";
const std::string Fighter_Fields::Arena_Fight::PT_STRENGTH_CURRENT = "strength_current";
const std::string Fighter_Fields::Arena_Fight::PT_STRENGTH_MAX = "strength_max";
const std::string Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_CURRENT = "blood_pool_current";
const std::string Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_MAX = "blood_pool_max";
const std::string Fighter_Fields::Arena_Fight::PT_IGNORE_DEF_RATE = "ignore_def_rate";
const std::string Fighter_Fields::Arena_Fight::PT_STATUS_HIT_RATE = "status_hit_rate";
const std::string Fighter_Fields::Arena_Fight::PT_STATUS_DEF_RATE = "status_def_rate";
const std::string Fighter_Fields::Arena_Fight::PT_BLOOD_STEAL = "blood_steal";
const std::string Fighter_Fields::Arena_Fight::PT_BE_CURE_RATE = "be_cure_rate";

const std::string Fighter_Fields::Arena_Fight::PT_PHY_HURT_FIX = "phy_hurt_fix";
const std::string Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE = "phy_hurt_rate";
const std::string Fighter_Fields::Arena_Fight::PT_SPELL_HURT_FIX = "spell_hurt_fix";
const std::string Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE = "spell_hurt_rate";
const std::string Fighter_Fields::Arena_Fight::PT_HURT_FIX = "hurt_fix";
const std::string Fighter_Fields::Arena_Fight::PT_HURT_RATE = "hurt_rate";
const std::string Fighter_Fields::Arena_Fight::PT_BE_HURT_FIX = "be_hurt_fix";
const std::string Fighter_Fields::Arena_Fight::PT_BE_HURT_RATE = "be_hurt_rate";
const std::string Fighter_Fields::Arena_Fight::PT_EFFECT_RATE = "effect_rate";
const std::string Fighter_Fields::Arena_Fight::PT_PVE_HURT_ADD = "pve_hurt_add";
const std::string Fighter_Fields::Arena_Fight::PT_PVE_HURT_SUB = "pve_hurt_sub";
const std::string Fighter_Fields::Arena_Fight::PT_DIZZ_STRENGTHEN = "dizz_strengthen";
const std::string Fighter_Fields::Arena_Fight::PT_DIZZ_WEAKEN = "dizz_weaken";

const std::string Fighter_Fields::Arena_Fight::PT_PK_HURT_ADD = "pk_hurt_add";
const std::string Fighter_Fields::Arena_Fight::PT_PK_HURT_SUB = "pk_hurt_sub";
const std::string Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE_SUB = "phy_hurt_rate_sub";
const std::string Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE_SUB = "spell_hurt_rate_sub";
const std::string Fighter_Fields::Arena_Fight::PT_SKILL_HURT_RATE = "skill_hurt_rate";
const std::string Fighter_Fields::Arena_Fight::PT_HUMAN_ADD = "human_add";
const std::string Fighter_Fields::Arena_Fight::PT_ELF_ADD = "elf_add";
const std::string Fighter_Fields::Arena_Fight::PT_ORC_ADD = "orc_add";
const std::string Fighter_Fields::Arena_Fight::PT_DRAGON_ADD = "dragon_add";
const std::string Fighter_Fields::Arena_Fight::PT_UNDEAD_ADD = "undead_add";
const std::string Fighter_Fields::Arena_Fight::PT_DEMON_ADD = "demon_add";
const std::string Fighter_Fields::Arena_Fight::PT_HUMAN_SUB = "human_sub";
const std::string Fighter_Fields::Arena_Fight::PT_ELF_SUB = "elf_sub";
const std::string Fighter_Fields::Arena_Fight::PT_ORC_SUB = "orc_sub";
const std::string Fighter_Fields::Arena_Fight::PT_DRAGON_SUB = "dragon_sub";
const std::string Fighter_Fields::Arena_Fight::PT_UNDEAD_SUB = "undead_sub";
const std::string Fighter_Fields::Arena_Fight::PT_DEMON_SUB = "demon_sub";
const std::string Fighter_Fields::Arena_Fight::PT_SUMMON_ADD = "summon_add";
const std::string Fighter_Fields::Arena_Fight::PT_SUMMON_SUB = "summon_sub";
const std::string Fighter_Fields::Arena_Fight::FORCE = "force";

const std::string Fighter_Fields::Arena_Fight::EXTRA_SKILL = "extra_skill";
const std::string Fighter_Fields::Arena_Fight::Extra_Skill::SKILL_ID = "skill_id";
const std::string Fighter_Fields::Arena_Fight::Extra_Skill::SKILL_LV = "skill_lv";

/// MAIL START /////////////////////////////
const std::string Mail_Fields::COLLECTION = "mmo.mail";
const std::string Mail_Fields::MAIL_ALL_COUNT = "mail_all_count";
const std::string Mail_Fields::MAX_GOLD = "max_gold";
const std::string Mail_Fields::MAIL_PAGE_COUNT = "mail_page_count";
const std::string Mail_Fields::MAIL_CURRENT_PAGE = "mail_current_page";
const std::string Mail_Fields::LAST_READ_SYS_MAIL_TIME = "last_read_sys_mail_time";

const std::string Mail_Fields::MAIL_LIST = "mail_map";
const std::string Mail_Fields::Mail_List::MAIL_ID = "mail_id";
const std::string Mail_Fields::Mail_List::HAVE_READ = "have_read";
const std::string Mail_Fields::Mail_List::COIN = "coin";
const std::string Mail_Fields::Mail_List::MAIL_TITLE = "mail_title";
const std::string Mail_Fields::Mail_List::MAIL_TYPE = "mail_type";
const std::string Mail_Fields::Mail_List::MAIL_CONTENT = "mail_content";
const std::string Mail_Fields::Mail_List::SENDER_NAME = "sender_name";
const std::string Mail_Fields::Mail_List::ITEM = "item";
const std::string Mail_Fields::Mail_List::SHOW_SEND_TIME = "show_send_time";
const std::string Mail_Fields::Mail_List::REAL_SEND_TIME = "real_send_time";

//ALCHEMY START ////////////////////////
const std::string Alchemy_Fields::COLLECTION = "mmo.alchemy";
const std::string Alchemy_Fields::CURRENT_HERO_CRITICAL = "current_hero_critical";
const std::string Alchemy_Fields::CUTTENT_HERO_GOLD = "current_hero_gold";
const std::string Alchemy_Fields::OVER_FREE_BOUT = "over_free_bout";
const std::string Alchemy_Fields::OVER_PAY_BOUT = "over_pay_bout";
const std::string Alchemy_Fields::SIX_CRITICAL_NUM = "six_critical_num";
const std::string Alchemy_Fields::LAST_TABS = "last_tabs";
const std::string Alchemy_Fields::SCURRENT_HERO_CRITICAL = "current_souls_hero_critical";
const std::string Alchemy_Fields::SCUTTENT_HERO_GOLD = "current_souls_hero_gold";
const std::string Alchemy_Fields::SOVER_FREE_BOUT = "over_sous_free_bout";
const std::string Alchemy_Fields::SOVER_PAY_BOUT = "over_sous_pay_bout";
const std::string Alchemy_Fields::SSIX_CRITICAL_NUM = "six_critical_souls_num";
const std::string Alchemy_Fields::SLAST_TABS = "last_souls_tabs";

//FRIEND START //////////////////////////
const std::string Friend_Fields::COLLECTION = "mmo.friend";
const std::string Friend_Fields::ROLE_ID = "role_id";
const std::string Friend_Fields::FRIEND_LIST = "friend_list";
const std::string Friend_Fields::STRANGER_LIST = "stranger_list";
const std::string Friend_Fields::BLACK_LIST = "black_list";
const std::string Friend_Fields::Friend_Info::ROLE_ID = "role_id";
const std::string Friend_Fields::Friend_Info::FRIEND_TYPE = "friend_type";
const std::string Friend_Fields::Friend_Info::FRIENDSHIP_DEGREE = "degree";
const std::string Friend_Fields::APPLY_LIST = "apply_list";
const std::string Friend_Fields::HEAD_PHOTO_ID = "head_photo_id";
const std::string Friend_Fields::Apply::ROLE_ID = "role_id";
const std::string Friend_Fields::Apply::TIME = "time";
const std::string Friend_Fields::INVITE_LIST = "invite_list";
const std::string Friend_Fields::Invite::ROLE_ID = "role_id";
const std::string Friend_Fields::Invite::TIME = "time";
const std::string Friend_Fields::DEL_LIST = "del_list";
const std::string Friend_Fields::Del::ROLE_ID = "role_id";
const std::string Friend_Fields::Del::FRIENDSHIP_DEGREE = "degree";

// PACKAGE START ////////////////////////////////////////////////////
const std::string Package_Fields::COLLECTION = "mmo.package";
const std::string Package_Fields::PACKAGE = "package";
const std::string Package_Fields::PACKAGE_MONEY_COPPER = "package.money.copper";

const std::string Package_Fields::PACK_CAPACITY = "pack_capacity";
const std::string Package_Fields::STORAGE_CAPACITY = "storage_capacity";
const std::string Package_Fields::SPIRIT_EQUIP_CAPACITY = "spirit_equip_capacity";
const std::string Package_Fields::SPIRIT_PACKAGE_CAPACITY = "spirit_package_capacity";
const std::string Package_Fields::RUNE_STONE_EQUIP_CAP = "rune_stone_equip_cap";
const std::string Package_Fields::RUNE_STONE_PACK_CAP = "rune_stone_pack_cap";
const std::string Package_Fields::THORLOTTERY_PACK_CAP = "thorlottery_pack_cap";

const std::string Package_Fields::LAST_WIPE_TIME = "last_wipe_time";

const std::string Package_Fields::MONEY = "money";
const std::string Package_Fields::Money::COPPER = "copper";
const std::string Package_Fields::Money::BIND_GOLD = "bind_gold";
const std::string Package_Fields::Money::GOLD = "gold";
const std::string Package_Fields::Money::DRAGON_SOUL = "dragon_soul";
const std::string Package_Fields::Money::SOULS = "souls";
const std::string Package_Fields::Money::FRIENDSHIP = "friendship";
const std::string Package_Fields::Money::REPUTATION = "reputation";

const std::string Package_Fields::Money::TODAY_GOLD_PAY = "today_gold_pay";
const std::string Package_Fields::Money::TODAY_GOLD_GET = "today_gold_get";
const std::string Package_Fields::Money::TODAY_COPPER_PAY = "today_copper_pay";
const std::string Package_Fields::Money::TODAY_COPPER_GET = "today_copper_get";

const std::string Package_Fields::ITEM = "item";
const std::string Package_Fields::Item::INDEX = "index";
const std::string Package_Fields::Item::ID = "id";
const std::string Package_Fields::Item::AMOUNT = "amount";
const std::string Package_Fields::Item::BIND = "bind";
const std::string Package_Fields::Item::OVERDUE_TIME = "overdue_time";

const std::string Package_Fields::Item::UNBAN_LVL = "unban_lvl";
const std::string Package_Fields::Item::SCORE = "score";
const std::string Package_Fields::Item::FORCE = "force";
const std::string Package_Fields::Item::INCR_LEVEL = "incr_level";
const std::string Package_Fields::Item::INCR_LUCK_VALUE = "incr_luck_value";
const std::string Package_Fields::Item::INCR_LVL_CONTI_FAIL_CNT = "incr_lvl_conti_fail_cnt";
const std::string Package_Fields::Item::COMBAT_POWER = "combat_power";
const std::string Package_Fields::Item::DURABILITY = "durability";
const std::string Package_Fields::Item::LEVEL = "level";
const std::string Package_Fields::Item::PERFECTION = "perfection";
const std::string Package_Fields::Item::COLOR = "color";
const std::string Package_Fields::Item::HOLE = "hole";
const std::string Package_Fields::Item::Hole::GEM_ID = "gem_id";
const std::string Package_Fields::Item::Hole::GEM_STEP = "gem_step";
const std::string Package_Fields::Item::Hole::GEM_PURTITY = "gem_purity";
const std::string Package_Fields::Item::ADDIT_SPIRIT = "addit_spirit";
const std::string Package_Fields::Item::ADDIT_SPIRITS = "addit_spirits";
const std::string Package_Fields::Item::SCORE_BASE = "score_base";
const std::string Package_Fields::Item::LIGHTEN_TIME = "lighten_time";
const std::string Package_Fields::Item::DISPLAY_REMAIN_TIME = "display_remain_time";
const std::string Package_Fields::Item::TIME_LIMIT = "time_limit";
const std::string Package_Fields::Item::INCR_ENERGY_LEVEL = "incr_energy_level";
const std::string Package_Fields::Item::INCR_BLESS_LEVEL = "incr_bless_level";
const std::string Package_Fields::Item::INCR_BLESS_VALUE = "incr_bless_value";
const std::string Package_Fields::Item::INCR_LEVEL_CONTI_CNT = "incr_level_conti_cnt";
const std::string Package_Fields::Item::ENCHANT_TIMES = "enchant_times";
const std::string Package_Fields::Item::CNT_ENCHANT_GROUP = "cnt_enchant_group";
const std::string Package_Fields::Item::ENCHANT_GROUPS = "enchant_groups";
const std::string Package_Fields::Item::Enchant_Groups::CNT_ENCHANT_PROP = "cnt_enchant_prop";
const std::string Package_Fields::Item::Enchant_Groups::ENCHANT_PROPS = "enchant_props";
const std::string Package_Fields::Item::Enchant_Groups::SCORE = "score";
const std::string Package_Fields::Item::Enchant_Groups::Enchant_Props::PROP_TYPE = "prop_type";
const std::string Package_Fields::Item::Enchant_Groups::Enchant_Props::PROP_VAL = "prop_val";
const std::string Package_Fields::Item::Enchant_Groups::Enchant_Props::STAR = "star";
const std::string Package_Fields::Item::Enchant_Groups::Enchant_Props::IS_LOCK = "is_lock";
const std::string Package_Fields::Item::ENCHANT_LOCK_PROP_FAIL = "enchant_lock_prop_fail";
const std::string Package_Fields::Item::Enchant_Lock_Prop_Fail::PROP = "prop";
const std::string Package_Fields::Item::Enchant_Lock_Prop_Fail::FAIL_CNT = "fail_cnt";
const std::string Package_Fields::Item::CARD_HOLE = "card_hole";
const std::string Package_Fields::Item::Card_Hole::LOC = "loc";
const std::string Package_Fields::Item::Card_Hole::STATE = "state";
const std::string Package_Fields::Item::Card_Hole::CARD_INDEX = "card_index";
const std::string Package_Fields::Item::STONE = "stone";
const std::string Package_Fields::Item::Stone_Hole::STONE_HOLE_INDEX = "stone_hole_index";
const std::string Package_Fields::Item::Stone_Hole::STONE_HOLE_STATE = "stone_hole_state";
const std::string Package_Fields::Item::Stone_Hole::STONE_ID = "stone_id";

const std::string Package_Fields::Item::EXP_LVL = "exp_lvl";
const std::string Package_Fields::Item::EXP = "exp";
const std::string Package_Fields::Item::IS_LOCK = "is_lock";

const std::string Package_Fields::Item::CARD_PART = "card_part";
const std::string Package_Fields::Item::CARD_POTEN = "card_poten";
const std::string Package_Fields::Item::CARD_STAR = "card_star";
const std::string Package_Fields::Item::CARD_LV = "card_lv";
const std::string Package_Fields::Item::CARD_EXP = "card_exp";
const std::string Package_Fields::Item::CARD_EQUIP_INDEX = "card_equip_index";
const std::string Package_Fields::Item::CARD_EQUIP_LOC = "card_equip_loc";
const std::string Package_Fields::Item::CARD_MAIN_ATTR = "card_main_attr";
const std::string Package_Fields::Item::CARD_SUB_ATTR = "card_sub_attr";

// ITEM_USER START	 ////////////////////////////////////////////////////
const std::string Item_User_Fields::COLLECTION = "mmo.item_user";
const std::string Item_User_Fields::USE_TIME = "use_time";
const std::string Item_User_Fields::ITEM_USE_SEC = "sec";
const std::string Item_User_Fields::ITEM_PROP = "item_prop";
const std::string Item_User_Fields::ITEM_PROP_TYPE = "item_prop_type";
const std::string Item_User_Fields::ITEM_PROP_VALUE = "item_prop_value";
const std::string Item_User_Fields::ITEM_HAD_USED_TIMES = "item_had_used_times";
const std::string Item_User_Fields::ITEM_HAD_USED_TIMES_TYPE = "item_had_used_times_type";
const std::string Item_User_Fields::ITEM_HAD_USED_TIMES_VALUE = "item_had_used_times_value";


// TASKER START	 ////////////////////////////////////////////////////
const std::string Task_Fields::COLLECTION = "mmo.task";
const std::string Task_Fields::ID = "id";
const std::string Task_Fields::SUBMITED_TASK = "submited_task";
const std::string Task_Fields::TASK = "task";
const std::string Task_Fields::NPC_ID = "npc_id";
const std::string Task_Fields::NPC_TYPE_MAP = "npc_type_map";
const std::string Task_Fields::Npc_Type_Map::NPC_ID = "npc_id";
const std::string Task_Fields::Npc_Type_Map::NPC_TYPE = "npc_type";
const std::string Task_Fields::Npc_Type_Map::FUNCNAME = "funcName";
const std::string Task_Fields::Npc_Type_Map::DIRECTION = "direction";
const std::string Task_Fields::Npc_Type_Map::SCENEID = "sceneId";
const std::string Task_Fields::Npc_Type_Map::OPEN = "open";
const std::string Task_Fields::Npc_Type_Map::X = "x";
const std::string Task_Fields::Npc_Type_Map::Y = "y";
const std::string Task_Fields::Npc_Type_Map::NPC_NAME = "ncp_name";

const std::string Task_Fields::Task::TASK_ID = "task_id";
const std::string Task_Fields::Task::GAME_TYPE = "game_type";
const std::string Task_Fields::Task::ACCEPT_TICK_SEC = "accept_tick_sec";
const std::string Task_Fields::Task::ACCEPT_TICK_USEC = "accept_tick_usec";
const std::string Task_Fields::Task::REFRESH_TICK_SEC = "refresh_tick_sec";
const std::string Task_Fields::Task::REFRESH_TICK_USEC = "refresh_tick_usec";
const std::string Task_Fields::Task::PREV_TASK = "prev_task";
const std::string Task_Fields::Task::POST_TASK = "post_task";
const std::string Task_Fields::Task::POST_TASK2 = "post_task2";
const std::string Task_Fields::Task::POST_TASK3 = "post_task3";
const std::string Task_Fields::Task::STATUS = "status";
const std::string Task_Fields::Task::LOGIC_TYPE = "logic_type";
const std::string Task_Fields::Task::ITEM_MONEY_FLAG = "item_money_flag";
const std::string Task_Fields::Task::IS_REQ_FAST_FINISH = "is_req_fast_finish";
const std::string Task_Fields::Task::RECRUID_HERO = "recruid_hero";
const std::string Task_Fields::Task::HERO_ID = "hero_id";
const std::string Task_Fields::Task::IS_PACK_ACCEPT = "is_pack_accept";

const std::string Task_Fields::Task::COND_LIST = "cond_list";
const std::string Task_Fields::Task::CondList::TYPE = "type";
const std::string Task_Fields::Task::CondList::CURRENT_VALUE = "current_value";

const std::string Task_Fields::Task::CondList::EXT_LIST = "ext_list";
const std::string Task_Fields::Task::CondList::ExtList::EXT = "ext";

const std::string Task_Fields::IS_EVER_OPEN_REWARD_TASK_LIST = "is_ever_open_reward_task_list";
const std::string Task_Fields::IS_CLOSE_AUTO_REFRESH = "is_close_auto_refresh";
const std::string Task_Fields::REFRESH_REWARD_TASK_NUM = "refresh_reward_task_num";
const std::string Task_Fields::IS_OPEN_COUNTDOWN = "is_open_countdown";
const std::string Task_Fields::BEGIN_COUNTDOWN_TIME_SEC = "begin_countdown_time_sec";
const std::string Task_Fields::BEGIN_COUNTDOWN_TIME_USEC = "begin_countdown_time_usec";
const std::string Task_Fields::USED_FREE_REFRESH_COUNT = "used_free_refresh_count";
const std::string Task_Fields::USED_ACCEPT_COUNT = "used_accept_count";
const std::string Task_Fields::GIVE_UP_NUM = "give_up_num";
const std::string Task_Fields::TARGET_PLAYER_ID = "target_player_id";
const std::string Task_Fields::TASK_ID = "task_id";
const std::string Task_Fields::RING_NUM = "ring_num";
const std::string Task_Fields::IS_DIAMOND_RECOVER = "is_diamond_recover";

const std::string Task_Fields::TASK_ICON_HERO_LIST = "task_icon_hero_list";
const std::string Task_Fields::Task_Icon_Hero::TASK_ID = "task_id";
const std::string Task_Fields::Task_Icon_Hero::HERO_ID = "hero_id";
const std::string Task_Fields::TASK_GIFT_LEVEL = "task_gift_level";
const std::string Task_Fields::SCENE_ID_LIST = "scene_id_list";
const std::string Task_Fields::Scene_Id::SCENE_ID = "scene_id";
const std::string Task_Fields::DID_RING_NUM = "did_ring_num";
const std::string Task_Fields::DID_REWARD_TASK_NUM = "did_reward_task_num";
const std::string Task_Fields::SCENE_NPC_LIST = "scene_npc_list";
const std::string Task_Fields::Scene_Npc::SCENE_ID = "scene_id";
const std::string Task_Fields::Scene_Npc::NPC_ID = "npc_id";
const std::string Task_Fields::RING_TASK_ID = "ring_task_id";
const std::string Task_Fields::IS_EXIST_CROSS_DAY = "is_exist_cross_day";
const std::string Task_Fields::DID_REWARD_TASK_NUM_TODAY = "did_reward_task_num_today";
const std::string Task_Fields::IS_SUBMIT_CLOSE_RING_TASK = "is_submit_close_ring_task";

/*=== SGL_DUNGEON START ============================================================================*/
const std::string Sgl_Dungeon_Fields::COLLECTION = "mmo.single_dungeon";
const std::string Sgl_Dungeon_Fields::ROLE_ID = "role_id";

const std::string Sgl_Dungeon_Fields::LAST_LOCATION = "last_location";
const std::string Sgl_Dungeon_Fields::SCENE_ID = "scene_id";
const std::string Sgl_Dungeon_Fields::SCENE_ORDER = "scene_order";
const std::string Sgl_Dungeon_Fields::SCENE_LAYER = "scene_layer";
const std::string Sgl_Dungeon_Fields::COORD_X = "coord_x";
const std::string Sgl_Dungeon_Fields::COORD_Y = "coord_y";
const std::string Sgl_Dungeon_Fields::TOWARD = "toward";

const std::string Sgl_Dungeon_Fields::SINGLE_VER = "single_ver";
const std::string Sgl_Dungeon_Fields::ELITE_VER = "elite_ver";
const std::string Sgl_Dungeon_Fields::LORD_VER = "lord_ver";
const std::string Sgl_Dungeon_Fields::SINGLE_COMPLETE_NUM = "single_complete_num";
const std::string Sgl_Dungeon_Fields::NOR_ELITE_COMPLETE_NUM = "nor_elite_complete_num";
const std::string Sgl_Dungeon_Fields::HOR_ELITE_COMPLETE_NUM = "lord_temple_complete_num";
const std::string Sgl_Dungeon_Fields::ELITE_RESET_TIMES = "elite_reset_times";
const std::string Sgl_Dungeon_Fields::ELITE_CH_NUM = "elite_ch_num";
const std::string Sgl_Dungeon_Fields::ELITE_ADD_NUM = "elite_add_num";
const std::string Sgl_Dungeon_Fields::ELITE_ADD_BUY_NUM = "elite_add_buy_num";
const std::string Sgl_Dungeon_Fields::ELITE_TASK_ADD_NUM = "elite_task_add_num";
const std::string Sgl_Dungeon_Fields::ACT_NOR_ELITE_WIPE = "act_nor_elite_wipe";
const std::string Sgl_Dungeon_Fields::SGLDUNGEONITEM = "sgl_dungeon_item";
const std::string Sgl_Dungeon_Fields::NOR_ELITE_ITEM = "nor_elite_item";
const std::string Sgl_Dungeon_Fields::HOR_ELITE_ITEM = "lord_temple_item";
const std::string Sgl_Dungeon_Fields::CHAPTER_AWARD_LIST = "chapter_award_list";
const std::string Sgl_Dungeon_Fields::TRAIL_INFO = "Trail_Info";
const std::string Sgl_Dungeon_Fields::LORD_TRAIL_INFO = "lord_trail_Info";

const std::string Sgl_Dungeon_Fields::WIPE_TYPE = "wipe_type";
const std::string Sgl_Dungeon_Fields::WIPE_NUM = "wipe_num";
const std::string Sgl_Dungeon_Fields::WIPE_ELITE_DOUBLE = "wipe_elite_double";
const std::string Sgl_Dungeon_Fields::WIPE_ELITE_DOUBLE_NOT_SELECTED = "wipe_elite_double_not_selected";
const std::string Sgl_Dungeon_Fields::WIPE_BEGIN_TIME = "wipe_begin_time";
const std::string Sgl_Dungeon_Fields::WIPE_SCENES = "wipe_scenes";
const std::string Sgl_Dungeon_Fields::WIPE_INFOS = "wipe_infos";

const std::string Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DUNGEON_ID = "dungeon_id";
const std::string Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TYPE = "type";
const std::string Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::PROGRESS = "progress";
const std::string Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TTL_COUNTER = "ttl_counter";
const std::string Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DAILY_COUNTER = "daily_counter";
const std::string Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_GOLD_NUM = "add_gold_num";
const std::string Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_PROP_NUM = "add_prop_num";
const std::string Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::CLEAR_MONSTER = "clear_monster";


const std::string Sgl_Dungeon_Fields::Chapter_Award::CHAPTER_ID = "chapter_id";
const std::string Sgl_Dungeon_Fields::Chapter_Award::AWARD_STATUS = "award_status";

const std::string Sgl_Dungeon_Fields::Trail_Info::ELITE_ITEMS = "elite_items";

const std::string Sgl_Dungeon_Fields::Wipe_Scene::SCENE_ID = "scene_id";

const std::string Sgl_Dungeon_Fields::Wipe_Info::INDEX = "index";
const std::string Sgl_Dungeon_Fields::Wipe_Info::SCENE_ID = "scene_id";
const std::string Sgl_Dungeon_Fields::Wipe_Info::EXP = "exp";
const std::string Sgl_Dungeon_Fields::Wipe_Info::COPPER = "copper";
const std::string Sgl_Dungeon_Fields::Wipe_Info::ITEMS = "items";

const std::string Sgl_Dungeon_Fields::Item_Info::ID = "id";
const std::string Sgl_Dungeon_Fields::Item_Info::AMOUNT = "amount";
const std::string Sgl_Dungeon_Fields::Item_Info::BIND = "bind";





/*=== SGL_DUNGEON END ===============================================================================*/

/*=== HEROER START ==============================================================================*/
const std::string Heroer_Fields::COLLECTION = "mmo.heroer";
const std::string Heroer_Fields::STATUS_COLLECTION = "mmo.heroer_status";
const std::string Heroer_Fields::ROLE_ID = "role_id";
const std::string Heroer_Fields::OUTER_INDEX = "outer_index";
const std::string Heroer_Fields::FIGHTER_INDEX = "fighter_index";
const std::string Heroer_Fields::LAST_FIGHTER_IDX = "last_fighter_idx";
const std::string Heroer_Fields::CLICKED_NEW_HEROS = "clicked_new_heros";
const std::string Heroer_Fields::ASSIST_HEROS = "assist_heros";
const std::string Heroer_Fields::Assist_Hero::LOC = "loc";
const std::string Heroer_Fields::Assist_Hero::HERO_IDX = "hero_idx";
const std::string Heroer_Fields::HERO_INFOS = "hero_infos";
const std::string Heroer_Fields::Hero_Info::HERO_ID = "hero_id";
const std::string Heroer_Fields::Hero_Info::FIGHT_DATE = "fight_date";
const std::string Heroer_Fields::Hero_Info::HERO_INDEX = "hero_index";
const std::string Heroer_Fields::Hero_Info::HERO_FACADE_ID = "hero_facade_id";
const std::string Heroer_Fields::Hero_Info::HERO_NAME = "hero_name";
const std::string Heroer_Fields::Hero_Info::LEVEL = "level";
const std::string Heroer_Fields::Hero_Info::EXP = "exp";
const std::string Heroer_Fields::Hero_Info::IS_OUTER = "is_outer";
const std::string Heroer_Fields::Hero_Info::IS_FIGHTER = "is_fighter"; //
const std::string Heroer_Fields::Hero_Info::FORMATION = "formation";
const std::string Heroer_Fields::Hero_Info::AWAKE_LVL = "awake_lvl";
const std::string Heroer_Fields::Hero_Info::SKILL_INFOS = "skill_infos";
const std::string Heroer_Fields::Hero_Info::COMBAT_POWER = "combat_power";
const std::string Heroer_Fields::Hero_Info::ARTIFACTS = "artifacts";
const std::string Heroer_Fields::Hero_Info::Artifact::ARTIFACT_ID = "artifact_id";
const std::string Heroer_Fields::Hero_Info::Artifact::ARTIFACT_LVL = "artifact_lvl";
const std::string Heroer_Fields::Hero_Info::IS_TMP_HERO = "is_tmp_hero";
const std::string Heroer_Fields::Hero_Info::INTIMACY_LV = "intimacy_lv";
const std::string Heroer_Fields::Hero_Info::INTIMACY_EXP = "intimacy_exp";
const std::string Heroer_Fields::Hero_Info::COMMUNITY_TIMES = "community_times";
const std::string Heroer_Fields::Hero_Info::GIFT_TIMES = "gift_times";
/*=== HEROER END ===============================================================================*/

/*=== RUNE START ==============================================================================*/
const std::string Rune_Fields::COLLECTION = "mmo.rune";
const std::string Rune_Fields::ROLE_ID = "role_id";
const std::string Rune_Fields::PARTS = "parts";
const std::string Rune_Fields::Part::PART = "rune_part";
const std::string Rune_Fields::Part::HOLES = "holes";
const std::string Rune_Fields::Part::Hole::INDEX = "index";
const std::string Rune_Fields::Part::Hole::LEVEL = "level";
const std::string Rune_Fields::Part::Hole::ENERGY = "energy";
/*=== RUNE END ================================================================================*/

/*=== rune_stone start ==============================================================================*/
const std::string Rune_Stone_Fields::COLLECTION = "mmo.rune_stone";
const std::string Rune_Stone_Fields::ROLE_ID = "role_id";
const std::string Rune_Stone_Fields::POINTS = "points";
const std::string Rune_Stone_Fields::REMAIN_DOUBLE_TIMES = "remain_double_times";
const std::string Rune_Stone_Fields::SMELTER_DETAILS = "smelter_details";
const std::string Rune_Stone_Fields::Smelter_Detail::SMELTER_IDX = "smelter_idx";
const std::string Rune_Stone_Fields::Smelter_Detail::IS_OPEN = "is_open";
/*=== rune_stone end ================================================================================*/

/*=== Time_Value START ==============================================================================*/
const std::string Time_Value_Fields::SEC = "sec";
const std::string Time_Value_Fields::USEC = "usec";
/*=== Time_Value END ================================================================================*/

/*=== Status START ==============================================================================*/
const std::string Status_Fields::COLLECTION = "mmo.status";
const std::string Status_Fields::ROLE_ID = "role_id";
const std::string Status_Fields::STATUS_MAP = "status_map";
const std::string Status_Fields::Status_Map::STATUS_ID					= "status_id";
const std::string Status_Fields::Status_Map::OVERLAY					= "overlay";
const std::string Status_Fields::Status_Map::ACTION_TIMES				= "action_times";
const std::string Status_Fields::Status_Map::TRIGGER_TIMES				= "trigger_times";
const std::string Status_Fields::Status_Map::LAST_STATUS_FROM			= "last_status_from";
const std::string Status_Fields::Status_Map::CFG_ACTION_TIMES			= "cfg_action_times";
const std::string Status_Fields::Status_Map::CFG_TRIGGER_TIMES			= "cfg_trigger_times";
const std::string Status_Fields::Status_Map::LAST_CASTER_ROLE_ID		= "last_caster_role_id";
const std::string Status_Fields::Status_Map::LAST_CASTER_MASTER_ID	= "last_caster_master_id";
const std::string Status_Fields::Status_Map::IS_FIGHT_BUFF				= "is_fight_buff";
const std::string Status_Fields::Status_Map::LAST_TIME_MS				= "last_time_ms";
const std::string Status_Fields::Status_Map::LAST_START_TIME			= "last_start_time";
const std::string Status_Fields::Status_Map::LAST_END_TIME				= "last_end_time";
const std::string Status_Fields::Status_Map::BUFF_SCENE				= "buff_scene";
const std::string Status_Fields::Status_Map::SCENE_VISIBLE				= "scene_visible";

const std::string Status_Fields::Status_Map::STATUS_FIFO				= "status_fifo";

const std::string Status_Fields::Status_Map::STATUS_INFO_MAP			= "status_info_map";
const std::string Status_Fields::Status_Map::Status_Info_Map::STATUS_ID			= "status_id";
const std::string Status_Fields::Status_Map::Status_Info_Map::STATUS_FROM		= "status_from";
const std::string Status_Fields::Status_Map::Status_Info_Map::CASTER_ROLE_ID		= "caster_role_id";
const std::string Status_Fields::Status_Map::Status_Info_Map::CASTER_MASTER_ID	= "caster_master_id";
const std::string Status_Fields::Status_Map::Status_Info_Map::OVERLAY			= "overlay";

const std::string Status_Fields::Status_Map::Status_Info_Map::EFFECT_MAP			= "effect_map";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_ID			= "effect_id";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_VALUE		= "effect_value";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE1	= "i_effect_value1";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE2	= "i_effect_value2";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE3	= "i_effect_value3";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE1	= "d_effect_value1";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE2	= "d_effect_value2";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE3	= "d_effect_value3";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE4	= "d_effect_value4";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE5	= "d_effect_value5";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE6	= "d_effect_value6";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE7	= "d_effect_value7";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE8	= "d_effect_value8";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE9	= "d_effect_value9";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TYPE		= "effect_type";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::STATUS_ID			= "status_id";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::HEAT_TYPE			= "heat_type";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TIMES		= "effect_times";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::OVERLAY			= "overlay";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_OVERLAY		= "is_overlay";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_HIT			= "is_hit";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::SKILL_ID			= "skill_id";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::TRIGGER_BUFF			= "trigger_buff";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_ID = "status_id";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::LAST_TIME_MS = "last_time_ms";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_ACTION_GAP = "cfg_action_gap";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_TRIGGER_GAP = "cfg_trigger_gap";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_FROM = "status_from";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CASTER_ID = "caster_id";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::OVERLAY = "overlay";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DISAPPEAR_GAP = "disappear_gap";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DATA 	= "data";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::EFFECT_VALUE	= "effect_value";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL1	= "i_effect_val1";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL2	= "i_effect_val2";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL3	= "i_effect_val3";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL1	= "d_effect_val1";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL2	= "d_effect_val2";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL3	= "d_effect_val3";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL4	= "d_effect_val4";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL5	= "d_effect_val5";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL6	= "d_effect_val6";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL7	= "d_effect_val7";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL8	= "d_effect_val8";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL9	= "d_effect_val9";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::IS_FIGHT_BUFF	= "is_out_buff";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::BUFF_CHANCE				= "buff_chance";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::CASTER_ID				= "caster_id";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TIMES			= "action_times";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TICK			= "action_tick";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TIMES		= "passive_times";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TICK		= "passive_tick";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::NEXT_OPT				= "next_opt";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::INTERVAL				= "interval";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_DELAY				= "is_delay";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIMES			= "delay_times";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_MS			= "delay_times_ms";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TIMES	= "delay_action_times";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TIMES	= "delay_passive_times";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_TICK		= "delay_time_tick";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TICK		= "delay_action_tiCK";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TICK	= "delay_passive_tiCK";

const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_VALUE		= "last_cure_value";
const std::string Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_RATE		= "last_cure_rate";
/*=== Status END ================================================================================*/

/*==========封号(帐号,角色ID)表START====================================================*/
const std::string BAN_ACCOUNT_Fields::COLLECTION = "backstage.ban_account";
const std::string BAN_ACCOUNT_Fields::BAN_TYPE = "btype";
const std::string BAN_ACCOUNT_Fields::BAN_NAME = "bname";
const std::string BAN_ACCOUNT_Fields::ACCOUNT = "account";
const std::string BAN_ACCOUNT_Fields::ROLE_ID = "role_id";
const std::string BAN_ACCOUNT_Fields::BAN_REMARK = "ban_remark";
const std::string BAN_ACCOUNT_Fields::BAN_TIME = "ban_time";
const std::string BAN_ACCOUNT_Fields::UNBAN_TIME = "unban_time";
/*==========封号(帐号,角色ID)表END====================================================*/

/*==========封IP表START====================================================*/
const std::string BAN_IP_Fields::COLLECTION = "backstage.ban_ip";
const std::string BAN_IP_Fields::IP_ADDRESS = "ip_address";
const std::string BAN_IP_Fields::BAN_REMARK = "ban_remark";
const std::string BAN_IP_Fields::BAN_TIME = "ban_time";
const std::string BAN_IP_Fields::UNBAN_TIME = "unban_time";
/*==========封IP表END====================================================*/

/*==========商城物品表START====================================================*/
const std::string MALL_LIST_Fields::COLLECTION = "backstage.mall";
const std::string MALL_LIST_Fields::ID = "id";
const std::string MALL_LIST_Fields::TABS = "tabs";
const std::string MALL_LIST_Fields::PAY_TYPE = "pay_type";
const std::string MALL_LIST_Fields::PRICE = "price";
const std::string MALL_LIST_Fields::VIP_DISCOUNTS = "vip_discounts";
const std::string MALL_LIST_Fields::PROMO_PRICE = "promo_price";
const std::string MALL_LIST_Fields::ALL_NUM = "all_num";
const std::string MALL_LIST_Fields::PER_NUM = "per_num";
const std::string MALL_LIST_Fields::SORT = "sort";
/*==========商城物品表END====================================================*/
/*==========商城PROMO表START====================================================*/
const std::string MALL_PROMO_LIST_Fields::COLLECTION = "backstage.mall_promo";
const std::string MALL_PROMO_LIST_Fields::START_TIME = "start_time";
const std::string MALL_PROMO_LIST_Fields::END_TIME = "end_time";
const std::string MALL_PROMO_LIST_Fields::DAY = "day";
const std::string MALL_PROMO_LIST_Fields::RANK = "rank";
const std::string MALL_PROMO_LIST_Fields::GOODS_LIST = "goods_list";
/*==========商城PROMO表END====================================================*/
/*==========商城MALL_BANNER_LIST_Fields表START====================================================*/
const std::string MALL_BANNER_LIST_Fields::COLLECTION = "backstage.mall_banner";
const std::string MALL_BANNER_LIST_Fields::START_TIME = "start_time";
const std::string MALL_BANNER_LIST_Fields::END_TIME = "end_time";
const std::string MALL_BANNER_LIST_Fields::DAY = "day";
const std::string MALL_BANNER_LIST_Fields::RANK = "rank";
const std::string MALL_BANNER_LIST_Fields::GOODS_LIST = "goods_list";
/*==========商城MALL_BANNER_LIST_Fields表END====================================================*/
/*==========Team END====================================================*/
const std::string Team_Role_Fields::COLLECTION = "mmo.team_role";
const std::string Team_Role_Fields::ROLE_ID = "role_id";
const std::string Team_Role_Fields::TEAM_ID = "team_id";
const std::string Team_Role_Fields::READY = "ready";
const std::string Team_Role_Fields::PROFILE_NUM_FB = "profile_num_fb";
const std::string Team_Role_Fields::PROFILE_NUM_FB_PRE = "profile_num_fb_pre";
const std::string Team_Role_Fields::PROFILE_NUM_SPORTS = "profile_num_sports";
const std::string Team_Role_Fields::PROFILE_NUM_HERO_DREAM = "profile_num_hero_dream";
const std::string Team_Role_Fields::PROFILE_NUM_DEFEND = "profile_num_defend";
const std::string Team_Role_Fields::PROFILE_TIME_SEC = "sec";
const std::string Team_Role_Fields::IS_NOT_FIRST_TEAM_FB = "is_not_first_team_fb";
const std::string Team_Role_Fields::INTEGRATE_MAP = "integrate_map";
const std::string Team_Role_Fields::TEAM_SPORTS_START_TIME = "team_sports_start_time";
const std::string Team_Sports_Fields::COLLECTION = "mmo.team_sports";
const std::string Team_Sports_Fields::ROLE_ID = "role_id";
const std::string Team_Sports_Fields::TEAM_ID = "team_id";
const std::string Team_Sports_Fields::READY = "ready";
const std::string Team_Sports_Fields::PROFILE_NUM_FB = "profile_num_fb";
const std::string Team_Sports_Fields::PROFILE_NUM_FB_PRE = "profile_num_fb_pre";
const std::string Team_Sports_Fields::PROFILE_NUM_SPORTS = "profile_num_sports";
const std::string Team_Sports_Fields::PROFILE_NUM_HERO_DREAM = "profile_num_hero_dream";
const std::string Team_Sports_Fields::PROFILE_TIME_SEC = "sec";
const std::string Team_Sports_Fields::INTEGRATE = "integrate";
const std::string Team_Sports_Fields::INTEGRATE_MAP = "integrate_map";
const std::string Team_Sports_Fields::TEAM_SPORTS_START_TIME = "team_sports_start_time";
const std::string Team_Sports_Fields::TEAM_SPORTS_START_TIMES = "team_sports_start_times";
const std::string Team_Sports_Fields::FIRST_LVL = "first_lvl";
const std::string Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID_HIS = "first_lvl_role_id_his";
const std::string Team_Sports_Fields::First_Lvl::FIRST_LVL_ZS_HIS = "first_lvl_zs_his";
const std::string Team_Sports_Fields::First_Lvl::FIRST_LVL_CK_HIS = "first_lvl_ck_his";
const std::string Team_Sports_Fields::First_Lvl::FIRST_LVL_YSS_HIS = "first_lvl_yss_his";
const std::string Team_Sports_Fields::First_Lvl::FIRST_LVL_INT = "first_lvl_int";
const std::string Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID = "first_lvl_role_id";
/*==========Team END====================================================*/
/*==========Gang START==================================================*/
const std::string Gang_Fields::COLLECTION = "mmo.gang";
const std::string Gang_Fields::GANG_ID = "gang_id";
const std::string Gang_Fields::LEADER = "leader";
const std::string Gang_Fields::LAST_LEADER_ID = "last_leader_id";
const std::string Gang_Fields::NOTIFIED_LAST_LEADER = "notified_last_leader";
const std::string Gang_Fields::FUND = "fund";
const std::string Gang_Fields::MAINTAIN_COST = "maintain_cost";
const std::string Gang_Fields::FUND_LACK_TIMES = "fund_lack_times";
const std::string Gang_Fields::LEVEL = "level";
const std::string Gang_Fields::BOSS_ACT = "boss_act";
const std::string Gang_Fields::BOSS_HOUR = "boss_hour";
const std::string Gang_Fields::BOSS_MINUTE = "boss_minute";
const std::string Gang_Fields::BOSS_KILL_NUM = "boss_kill_num";
const std::string Gang_Fields::BOSS_LV = "boss_lv";
const std::string Gang_Fields::RANK = "rank";
const std::string Gang_Fields::MAX_MEMBERS = "max_members";
const std::string Gang_Fields::LOGO = "logo";
const std::string Gang_Fields::DAY_DEFICIT_COUNT = "day_deficit_count";
const std::string Gang_Fields::STATUS = "status";
const std::string Gang_Fields::JOIN_BLOCK = "join_block";
const std::string Gang_Fields::CD_UPGRADE = "cd_upgrade";
const std::string Gang_Fields::CREATE_TIME = "create_time";
const std::string Gang_Fields::DISMISS_TIME = "dismiss_time";
const std::string Gang_Fields::TV_LEADER_OFFLINE = "tv_leader_offline";
const std::string Gang_Fields::ANNOUNCEMENT = "announcement";
const std::string Gang_Fields::NAME = "name";
const std::string Gang_Fields::SERVER_INFO = "server_info";
const std::string Gang_Fields::Server_Info::AGENT_NUM = "agent_num";
const std::string Gang_Fields::Server_Info::SERVER_NUM = "server_num";
const std::string Gang_Fields::MEMBERS = "members";
const std::string Gang_Fields::Member::ROLE_ID = "role_id";
const std::string Gang_Fields::Member::WEEK_CONTRIBUTE = "week_contribute";
const std::string Gang_Fields::Member::TOTAL_CONTRIBUTE = "total_contribute";
const std::string Gang_Fields::Member::AMPLIFY_TIMES = "amplify_times";
const std::string Gang_Fields::Member::HEADSHIP = "headship";
const std::string Gang_Fields::Member::AUTHORITY = "authority";
const std::string Gang_Fields::MEMBER_SIZE = "member_size";
const std::string Gang_Fields::AFFAIRS = "affairs";
const std::string Gang_Fields::Affairs::TYPE = "type";
const std::string Gang_Fields::Affairs::RESULT = "result";
const std::string Gang_Fields::Affairs::EXPIRE = "expire";
const std::string Gang_Fields::Affairs::APPLIER = "applier";
const std::string Gang_Fields::RESEARCHED_SKILLS = "researched_skills";
const std::string Gang_Fields::Researched_Skills::SKILL_ID = "skill_id";
const std::string Gang_Fields::BUILDINGS = "buildings";
const std::string Gang_Fields::Building::BUILDING_ID = "building_id";
const std::string Gang_Fields::Building::BUILDING_LVL = "building_lvl";
const std::string Gang_Fields::Building::CD_UPGRADE = "cd_upgrade";
const std::string Gang_Fields::REWARD_INFO_GANG_WAR = "reward_info_gang_war";
const std::string Gang_Fields::LEVEL_LEADER_ID = "lev_leader_id";
const std::string Gang_Fields::ACTIVE_ANNON_ED = "active_annon_ed";
const std::string Gang_Fields::Reward_Info_Gang_War::INDEX = "index";
const std::string Gang_Fields::Reward_Info_Gang_War::REWARD_ID = "reward_id";
const std::string Gang_Fields::Reward_Info_Gang_War::QUANTITY = "quantity";
const std::string Gang_Fields::Reward_Info_Gang_War::VIP_ADD = "vip_add";
/*==========Gang END====================================================*/
/*==========Gang War Start==============================================*/
const std::string Gang_War_Info_Fields::COLLECTION = "mmo.gang_war_info";
const std::string Gang_War_Info_Fields::HISTORY_COLLECTION = "mmo.gang_war_info_bak";
const std::string Gang_War_Info_Fields::GANG_WAR_MATCHED_GANGS_PAIR_VEC = "gang_war_matched_gangs_pair_vec";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCH_TIME = "match_time";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_PAIR_FIRST = "matched_pair_first";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_PAIR_SECOND = "matched_pair_second";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::RESULT = "result";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_INDEX = "matched_index";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::SCENE_INDEX = "scene_index";

const std::string Gang_War_Info_Fields::GANG_WAR_MATCHED_GANG_MAP = "gang_war_matched_gang_map";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::KEY = "key";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::MATCHED_INDEX = "matched_index";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::SCENE_INDEX = "scene_index";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::CAMP = "camp";
const std::string Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::MATCH_TIME = "match_time";

const std::string Gang_War_Info_Fields::GANG_WAR_SPOT_RANK_MAP = "gang_war_spot_rank_map";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::KEY = "key";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::VALUE = "value";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::KILL_NUM = "kill_num";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::DIE_NUM = "die_num";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::HOLD_SPOT = "hold_spot";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::HEADSHIP = "headship";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::FORCE = "force";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::GANG_NAME = "gang_name";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::CAMP = "camp";
const std::string Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::BTN_STATE = "btn_state";

const std::string Gang_War_Info_Fields::GANG_WAR_RESULT_MAP = "gang_war_result_map";
const std::string Gang_War_Info_Fields::Gang_War_Result_Map::INDEX = "index";
const std::string Gang_War_Info_Fields::Gang_War_Result_Map::GANG_VEC = "gang_vec";
const std::string Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_ID = "gang_id";
const std::string Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME = "gang_name";
const std::string Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::INDEX = "index";
const std::string Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::ZONE = "zone";
const std::string Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::RANK = "rank";
const std::string Gang_War_Info_Fields::Gang_War_Result_Map::RESULT = "result";

const std::string Gang_War_Info_Fields::GANG_RANK_MAP = "gang_rank_map";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::GANG_ID = "gang_id";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::GANG_NAME = "gang_name";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::ZONE = "zone";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::SCORE = "score";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::WIN_NUMS = "win_nums";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::KILL_NUMS = "kill_nums";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::HOLD_NUMS = "hold_nums";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::FORCE = "force";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::GANGER_RANK_INFO = "ganger_rank_info";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::KILL_NUM = "kill_num";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::DIE_NUM = "die_num";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::HOLD_SPOT = "hold_spot";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::HEADSHIP = "headship";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::FORCE = "force";
const std::string Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::CAMP = "camp";
const std::string Gang_War_Info_Fields::OPEN_TIME_FOR_EACH_MATCH = "open_time_for_each_match";

const std::string Gang_War_Info_Fields::GANG_WAR_RANK_AWARD = "gang_war_rank_award";
const std::string Gang_War_Info_Fields::Gang_War_Rank_Award::SCORE = "score";
const std::string Gang_War_Info_Fields::Gang_War_Rank_Award::WIN_NUMS = "win_nums";
const std::string Gang_War_Info_Fields::Gang_War_Rank_Award::KILL_NUMS = "kill_nums";
const std::string Gang_War_Info_Fields::Gang_War_Rank_Award::FORCE = "force";
const std::string Gang_War_Info_Fields::Gang_War_Rank_Award::STATE = "state";

const std::string Gang_War_Info_Fields::STAGE = "stage";
const std::string Gang_War_Info_Fields::SWITCH = "switch";
const std::string Gang_War_Info_Fields::ACTIVITY_TIME = "activity_time";
/*==========Gang War END====================================================*/
/*==========Ganger START================================================*/
const std::string Ganger_Fields::COLLECTION = "mmo.ganger";
const std::string Ganger_Fields::GANG_ID = "gang_id";
const std::string Ganger_Fields::LAST_GANG_ID = "last_gang_id";
const std::string Ganger_Fields::ROLE_ID = "role_id";
const std::string Ganger_Fields::CUR_CONTRIBUTE = "cur_contribute";
const std::string Ganger_Fields::TOTAL_CONTRIBUTE = "total_contribute";
const std::string Ganger_Fields::ACHI_CONTRIBUTE = "achi_contribute";
const std::string Ganger_Fields::DAY_CONTRIBUTE = "day_contribute";
const std::string Ganger_Fields::CONTRIB_OF_WEEK = "contrib_of_week";
const std::string Ganger_Fields::DAY_GOLD_CONTRIB_TIMES = "day_gold_contrib_times";
const std::string Ganger_Fields::DAY_DIAM_CONTRIB_TIMES = "day_diam_contrib_times";
const std::string Ganger_Fields::LAST_RANK = "last_rank";
const std::string Ganger_Fields::AUTHORITY = "authority";
const std::string Ganger_Fields::RECEIVE_BLESS_TIMES = "receive_bless_times";
//const std::string Ganger_Fields::REMAIN_DRAW_TIMES = "remain_draw_times";
const std::string Ganger_Fields::TODAY_HAS_DRAW_TIMES = "today_has_draw_times";
const std::string Ganger_Fields::JOIN_TIME = "join_time";
const std::string Ganger_Fields::LEAVE_TIME= "leave_time";
const std::string Ganger_Fields::HEADSHIP  = "headship";
const std::string Ganger_Fields::PRACTICE_TIMES_TTL = "practice_times_ttl";
const std::string Ganger_Fields::AFFAIRS = "affairs";
const std::string Ganger_Fields::Affairs::TYPE = "type";
const std::string Ganger_Fields::Affairs::RESULT = "result";
const std::string Ganger_Fields::Affairs::EXTRA_ID = "extra_id";
const std::string Ganger_Fields::Affairs::EXPIRE = "expire";
const std::string Ganger_Fields::Affairs::CONTENT = "content";
const std::string Ganger_Fields::INVITES = "invites";
//const std::string Ganger_Fields::Invites::GANG_ID = "gang_id";
const std::string Ganger_Fields::APPLY_IDS = "apply_ids";
const std::string Ganger_Fields::SKILL_DETAILS = "skill_details";
const std::string Ganger_Fields::Skill_Details::SKILL_ID = "skill_id";
const std::string Ganger_Fields::DRAW_AWARDS = "draw_awards";
const std::string Ganger_Fields::GANG_WAR_ATTEND_TIME = "gang_war_attend_time";
const std::string Ganger_Fields::GANG_WAR_SPOT_INDEX = "gang_war_spot_index";
const std::string Ganger_Fields::GANG_WAR_ATTEND_GANG_ID = "gang_war_attend_gang_id";
const std::string Ganger_Fields::GANG_WAR_ATTEND_GANG_ID2 = "gang_war_attend_gang_id2";
const std::string Ganger_Fields::GANG_WAR_AWARD_GET_TIMES_BY_DAY = "gang_war_award_get_times_by_day";
/*==========Ganger END==================================================*/
/*==========pay START====================================================*/
const std::string Pay_Fields::COLLECTION = "backstage.pay";
const std::string Pay_Fields::ROLE_ID = "role_id";
const std::string Pay_Fields::PAYTICKET = "payticket";
const std::string Pay_Fields::TIME = "time";
const std::string Pay_Fields::ORDER_ID = "order_id";
const std::string Pay_Fields::IN_ACCOUNT = "in_account";
/*==========pay END====================================================*/

/*==========arena_battle START====================================================*/
const std::string Arena_Battle_Fields::COLLECTION = "mmo.arean_batlle";
const std::string Arena_Battle_Fields::ARENA_ID = "arena_id";
const std::string Arena_Battle_Fields::STATUS = "status";
const std::string Arena_Battle_Fields::FREE_MATCH_TIMES = "free_match_times";
const std::string Arena_Battle_Fields::PAY_MATCH_TIMES = "pay_match_times";
const std::string Arena_Battle_Fields::START_TIME = "start_time";
const std::string Arena_Battle_Fields::END_TIME = "end_time";
const std::string Arena_Battle_Fields::REWARD_TIME1 = "reward_time1";
const std::string Arena_Battle_Fields::REWARD_TIME2 = "reward_time2";
/*==========arena_battle END====================================================*/

/*==========arenaer START====================================================*/
const std::string Arenaer_Fields::COLLECTION = "mmo.arena_player";
const std::string Arenaer_Fields::ROLE_ID = "role_id";
const std::string Arenaer_Fields::ARENA_ID = "arena_id";
const std::string Arenaer_Fields::GROUP = "group";
const std::string Arenaer_Fields::SEG = "seg";
const std::string Arenaer_Fields::SCORE = "score";
const std::string Arenaer_Fields::RISE_FAIL = "rise_fail";
const std::string Arenaer_Fields::RISE_WIN = "rise_win";
const std::string Arenaer_Fields::RISE_LOSE = "rise_lose";
const std::string Arenaer_Fields::RANK = "rank";
const std::string Arenaer_Fields::KING_TIME = "king_time";
const std::string Arenaer_Fields::ENTER_NUM = "enter_num";
const std::string Arenaer_Fields::PAY_TIMES = "pay_times";
const std::string Arenaer_Fields::BUY_PAY_TIMES = "buy_pay_times";
const std::string Arenaer_Fields::STREAK_WIN = "streak_win";
const std::string Arenaer_Fields::STREAK_LOSE = "streak_lose";
const std::string Arenaer_Fields::COMPENSATE_TIMES = "compensate_times";
const std::string Arenaer_Fields::THREE_DAY_RANK = "three_day_rank";
const std::string Arenaer_Fields::SEVEN_DAY_RANK = "seven_day_rank";
const std::string Arenaer_Fields::CD = "cd";
const std::string Arenaer_Fields::MATCH_TIME = "match_time";
const std::string Arenaer_Fields::MATCH_PLAYER_1 = "match_player_1";
const std::string Arenaer_Fields::MATCH_PLAYER_2 = "match_player_2";
const std::string Arenaer_Fields::MATCH_PLAYER_3 = "match_player_3";
const std::string Arenaer_Fields::MATCH_DLV_1 = "match_dlv_1";
const std::string Arenaer_Fields::MATCH_DLV_2 = "match_dlv_2";
const std::string Arenaer_Fields::MATCH_DLV_3 = "match_dlv_3";
const std::string Arenaer_Fields::FIGHT_TIMES = "fight_times";
const std::string Arenaer_Fields::ARENA_WORSHIP = "arena_worship";
const std::string Arenaer_Fields::BE_WORSHIP = "be_worship";
const std::string Arenaer_Fields::BATTLE_REPORT = "battle_report";
const std::string Arenaer_Fields::Battle_Report::ROLE_NAME = "role_name";
const std::string Arenaer_Fields::Battle_Report::RESULT = "result";
const std::string Arenaer_Fields::Battle_Report::SCORE = "score";
const std::string Arenaer_Fields::Battle_Report::GOLD = "gold";
const std::string Arenaer_Fields::Battle_Report::VAL1 = "val1";
const std::string Arenaer_Fields::Battle_Report::VAL2 = "val2";
const std::string Arenaer_Fields::Battle_Report::ITEM = "item";
const std::string Arenaer_Fields::Battle_Report::Item::ITEM = "item";
const std::string Arenaer_Fields::Battle_Report::Item::COUNT = "count";
const std::string Arenaer_Fields::FIGHT = "fight";
const std::string Arenaer_Fields::ROLE_NAME = "role_name";
const std::string Arenaer_Fields::LEVEL = "level";
const std::string Arenaer_Fields::CAREER = "career";
const std::string Arenaer_Fields::GENDER = "gender";

/*==========arena_battle END====================================================*/

/*==========arena_season START====================================================*/
const std::string Arena_Season_Fields::COLLECTION = "mmo.arena_season";
const std::string Arena_Season_Fields::SEASON_STATUS = "season_status";
const std::string Arena_Season_Fields::SEASON_BEGIN = "season_begin";
const std::string Arena_Season_Fields::SEASON_FINISH = "season_finish";
const std::string Arena_Season_Fields::SEASON_RESULT = "season_result";
const std::string Arena_Season_Fields::Season_Result::ROLE_ID = "role_id";
const std::string Arena_Season_Fields::Season_Result::LEVEL = "level";
const std::string Arena_Season_Fields::Season_Result::GROUP_INDEX = "group_index";
const std::string Arena_Season_Fields::Season_Result::RANK = "rank";
const std::string Arena_Season_Fields::Season_Result::INNER_RANK = "inner_rank";
/*==========arena_season END====================================================*/

/*==========ranking START====================================================*/
const std::string Ranking_Fields::COLLECTION = "mmo.ranking";
const std::string Ranking_Fields::RANKING_TYPE = "type";
const std::string Ranking_Fields::RANKING = "ranking";
const std::string Ranking_Fields::ROLE_INFO = "role_info";
const std::string Ranking_Fields::HEROER_INFO = "heroer_info";
const std::string Ranking_Fields::UPDATE_TIME = "update_time";
const std::string Ranking_Fields::KEY = "key";
const std::string Ranking_Fields::VALUE = "value";

const std::string Ranking_Fields::Rank_Info::ROLE_ID = "role_id";
const std::string Ranking_Fields::Rank_Info::ROLE_NAME = "role_name";
const std::string Ranking_Fields::Rank_Info::CAREER = "career";
const std::string Ranking_Fields::Rank_Info::SERVER = "server";

const std::string Ranking_Fields::Rank_Info::EXT_1 = "ext_1";
const std::string Ranking_Fields::Rank_Info::EXT_2 = "ext_2";
const std::string Ranking_Fields::Rank_Info::EXT_3 = "ext_3";
const std::string Ranking_Fields::Rank_Info::EXT_4 = "ext_4";
const std::string Ranking_Fields::Rank_Info::EXT_5 = "ext_5";
const std::string Ranking_Fields::Rank_Info::EXT_6 = "ext_6";
const std::string Ranking_Fields::Rank_Info::EXT_7 = "ext_7";
const std::string Ranking_Fields::Rank_Info::EXT_8 = "ext_8";

const std::string Ranking_Fields::Role_Info::ROLE_ID = "role_id";
const std::string Ranking_Fields::Role_Info::ROLE_NAME = "role_name";
const std::string Ranking_Fields::Role_Info::GENDER = "gender";
const std::string Ranking_Fields::Role_Info::GANG_ID = "gang_id";
const std::string Ranking_Fields::Role_Info::GANG_NAME = "gang_name";
const std::string Ranking_Fields::Role_Info::HERO_ID = "hero_id";
const std::string Ranking_Fields::Role_Info::HERO_INDEX = "hero_index";
const std::string Ranking_Fields::Role_Info::EXPLOIT_LVL = "exploit_lvl";
const std::string Ranking_Fields::Role_Info::EXPLOIT_VAL = "exploit_val";
const std::string Ranking_Fields::Role_Info::AVATAR = "avatar";
const std::string Ranking_Fields::Role_Info::HERO_ITEM_INFO = "hero_item_info";
const std::string Ranking_Fields::Role_Info::HERO_ITEM_EQUIP_INFO = "hero_item_equip_info";
const std::string Ranking_Fields::Role_Info::HERO_RUNE_PART_INFO = "hero_rune_part_info";
const std::string Ranking_Fields::Role_Info::HERO_ITEM_RUNE_STONE_INFO = "hero_rune_stone_info";

const std::string Ranking_Fields::Role_Info::HERO_PROP_VALUE = "hero_prop_value";
const std::string Ranking_Fields::Role_Info::PROP_VALUE = "prop_value";
const std::string Ranking_Fields::Role_Info::Prop_Value::TYPE = "type";
const std::string Ranking_Fields::Role_Info::Prop_Value::VALUE = "value";

const std::string Ranking_Fields::ITEM_INFO = "item_info";
const std::string Ranking_Fields::Item_Info::INDEX = "index";
const std::string Ranking_Fields::Item_Info::SEQ = "seq";
const std::string Ranking_Fields::Item_Info::ID = "id";
const std::string Ranking_Fields::Item_Info::AMOUNT = "amount";
const std::string Ranking_Fields::Item_Info::BIND = "bind";
const std::string Ranking_Fields::Item_Info::OVERDUE_TIME = "overdue_time";

const std::string Ranking_Fields::ITEM_EQUIP_INFO = "item_equip_info";
const std::string Ranking_Fields::Item_Equip_Info::INDEX = "index";
const std::string Ranking_Fields::Item_Equip_Info::UNBAN_LVL = "unban_lvl";
const std::string Ranking_Fields::Item_Equip_Info::FORCE = "force";
const std::string Ranking_Fields::Item_Equip_Info::SCORE = "score";
const std::string Ranking_Fields::Item_Equip_Info::INCR_LEVEL = "incr_level";
const std::string Ranking_Fields::Item_Equip_Info::INCR_LUCK_VALUE = "incr_luck_value";
const std::string Ranking_Fields::Item_Equip_Info::INCR_FAIL_ADD_RATE = "incr_fail_add_rate";
const std::string Ranking_Fields::Item_Equip_Info::CARD_INFO = "card_info";
const std::string Ranking_Fields::Item_Equip_Info::ENCHANT_PROP_GROUPS = "enchant_prop_groups";
const std::string Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::GROUP_INFO = "group_info";
const std::string Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::PROP_TYPE = "prop_type";
const std::string Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::PROP_VAL = "prop_val";
const std::string Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::STAR = "star";
const std::string Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::IS_LOCK = "is_lock";
const std::string Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::SCORE = "score";

const std::string Ranking_Fields::Item_Equip_Info::Card_Info::INDEX = "index";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::LV = "lv";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::ID = "id";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::CUR_EXP = "cur_exp";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::POTEN = "poten";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR = "main_attr";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR = "sub_attr";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::STAR = "star";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::PART = "part";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::ITEM_ID = "item_id";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::EQUIP_LOC = "equip_loc";
const std::string Ranking_Fields::Item_Equip_Info::Card_Info::EMBED_STATE = "embed_state";

const std::string Ranking_Fields::RUNE_PART_INFO = "rune_part_info";
const std::string Ranking_Fields::Rune_Part_Info::PART = "part";
const std::string Ranking_Fields::Rune_Part_Info::RUNE_HOLE_INFO = "rune_hole_info";
const std::string Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::INDEX = "index";
const std::string Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::LEVEL = "level";
const std::string Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::ENERGY = "energy";

const std::string Ranking_Fields::ITEM_RUNE_STONE_INFO = "item_rune_stone_info";
const std::string Ranking_Fields::Item_Rune_Stone_Info::INDEX = "index";
const std::string Ranking_Fields::Item_Rune_Stone_Info::EXP_LVL = "exp_lvl";
const std::string Ranking_Fields::Item_Rune_Stone_Info::EXP = "exp";
const std::string Ranking_Fields::Item_Rune_Stone_Info::IS_LOCK = "is_lock";

const std::string Ranking_Fields::ITEM_MASTER_PART_INFO = "master_part_info";
const std::string Ranking_Fields::Item_Master_Part_Info::INDEX = "index";
const std::string Ranking_Fields::Item_Master_Part_Info::LEVEL = "level";
const std::string Ranking_Fields::Item_Master_Part_Info::LUCK_VALUE = "luck_value";
const std::string Ranking_Fields::Item_Master_Part_Info::FAIL_ADD_RATE = "fail_add_rate";
//***

const std::string Rank_Equiper_Fields::COLLECTION = "mmo.rank_equiper";
const std::string Rank_Equiper_Fields::ROLE_ID = "role_id";
const std::string Rank_Equiper_Fields::ITEM_ID = "item_id";
const std::string Rank_Equiper_Fields::ITEM_INDEX = "item_index";
const std::string Rank_Equiper_Fields::ROLE_NAME = "role_name";
const std::string Rank_Equiper_Fields::ITEM_NAME = "item_name";
const std::string Rank_Equiper_Fields::SCORE = "score";
const std::string Rank_Equiper_Fields::ITEM_INFO = "item_info";
const std::string Rank_Equiper_Fields::ITEM_BASE = "item_base";

/*==========ranking END====================================================*/

const std::string Daily_Fields::COLLECTION = "mmo.daily";
const std::string Daily_Fields::DAILY_LIST = "daily_list";
const std::string Daily_Fields::TODAY_ACTIVITY = "today_activity";
const std::string Daily_Fields::ALL_ACTIVITY = "all_activity";
const std::string Daily_Fields::Daily_Info::ID = "id";
const std::string Daily_Fields::Daily_Info::NUM = "num";
const std::string Daily_Fields::ACTIVITY_LIST = "activity_list";
const std::string Daily_Fields::Activity_Info::ID = "id";
const std::string Daily_Fields::Activity_Info::NUM = "num";
const std::string Daily_Fields::TODAY_RECEIVE = "today_receive";
const std::string Daily_Fields::ALL_RECEIVE = "all_receive";
const std::string Daily_Fields::LAST_ARENA_FORCE = "last_arena_force";
const std::string Daily_Fields::NJOIN_ARENA_TIP = "njoin_arena_tip";
const std::string Daily_Fields::RECEIVE_VIP_ACTIVITY = "receive_vip_activity";
const std::string Daily_Fields::CHECK_IN_DAY = "check_in_day";
const std::string Daily_Fields::LAST_CHECK_IN_REFRESH_TIME = "last_check_in_refresh";
const std::string Daily_Fields::TODAY_IS_CHECK_IN = "today_is_check_in";
const std::string Daily_Fields::LAST_TIP_FORCE = "last_tip_force";

/*==========Covenant start====================================================*/
const std::string Covenant_Fields::COLLECTION = "mmo.covenant";
const std::string Covenant_Fields::ROLE_ID = "role_id";
const std::string Covenant_Fields::COVENANT_DATA_LIST = "covenant_data_list";
const std::string Covenant_Fields::Covenant_Data::ID = "id";
const std::string Covenant_Fields::Covenant_Data::INIT_STATUS = "init_status";
const std::string Covenant_Fields::Covenant_Data::OVER_TIME = "over_time";
const std::string Covenant_Fields::Covenant_Data::TODAY_BECOME_TIME = "today_become_time";
/*==========Covenant END====================================================*/

/*==========Vip start====================================================*/
const std::string Vip_Fields::COLLECTION = "mmo.vip";
const std::string Vip_Fields::ROLE_ID = "role_id";
const std::string Vip_Fields::GROW_VAL = "grow_val";
const std::string Vip_Fields::LAST_SETTLE_GROW = "last_settle_grow";
const std::string Vip_Fields::EXP_OVERTIME = "exp_overtime";
const std::string Vip_Fields::COST_MONEY = "cost_money";
const std::string Vip_Fields::EXPIRED_PROMPT = "expired_prompt";
const std::string Vip_Fields::HAS_REC_DAILY = "has_rec_daily";
const std::string Vip_Fields::WORLD_TREE_LIST = "world_tree_list";
const std::string Vip_Fields::World_Tree_Data::ID = "id";
const std::string Vip_Fields::World_Tree_Data::COPPER = "copper";
const std::string Vip_Fields::World_Tree_Data::SOULS = "souls";
const std::string Vip_Fields::World_Tree_Data::DRAGON_SOUL = "dragon_soul";
const std::string Vip_Fields::World_Tree_Data::RECEIVE_COPPER = "rec_copper";
const std::string Vip_Fields::World_Tree_Data::RECEIVE_SOULS = "rec_souls";
const std::string Vip_Fields::World_Tree_Data::RECEIVE_DRAGON_SOUL = "rec_dragon_soul";
/*==========Vip END====================================================*/

/*==========Mount start====================================================*/
const std::string Mount_Fields::COLLECTION = "mmo.mount";
const std::string Mount_Fields::ROLE_ID = "role_id";
const std::string Mount_Fields::MOUNT_ID = "mount_id";
const std::string Mount_Fields::RIDE = "ride";
const std::string Mount_Fields::HORSE_LIST = "horse_list";
const std::string Mount_Fields::GROUP_LIST = "group_list";
const std::string Mount_Fields::SUM_TRAIN_LV = "sum_train_lv";
const std::string Mount_Fields::Horse_Info::HORSE_ID = "horse_id";
const std::string Mount_Fields::Horse_Info::INVALID = "invalid";
const std::string Mount_Fields::Horse_Info::DEADLINE = "deadline";
const std::string Mount_Fields::Horse_Info::ACTIVATE_TIME = "activate_time";
const std::string Mount_Fields::Horse_Info::TRAIN_LV = "train_lv";
const std::string Mount_Fields::Horse_Info::TRAIN_NUM = "train_num";
const std::string Mount_Fields::Horse_Info::TRAIN_FAIL_NUM = "train_fail_num";
const std::string Mount_Fields::Horse_Info::DEPLOY = "deploy";
const std::string Mount_Fields::Horse_Info::DEPLOY_SKILL_1 = "deploy_skill_1";
const std::string Mount_Fields::Horse_Info::DEPLOY_SKILL_2 = "deploy_skill_2";
const std::string Mount_Fields::Horse_Info::DEPLOY_SKILL_3 = "deploy_skill_3";
const std::string Mount_Fields::Group_Info::GROUP_ID = "group_id";
const std::string Mount_Fields::Group_Info::STATUS = "status";
/*==========Mount END====================================================*/

/*==========Reward start====================================================*/
const std::string Reward_Fields::COLLECTION = "mmo.reward";
const std::string Reward_Fields::ROLE_ID = "role_id";
const std::string Reward_Fields::ONLINE_REWAED_NUM = "online_reward_num";
const std::string Reward_Fields::ONLINE_REWARD_TIME = "online_reward_time";
const std::string Reward_Fields::ONLINE_REFRESH_REWARD_TIME = "online_refresh_reward_time";
const std::string Reward_Fields::ONLINE_REWAED_OVER = "online_reward_over";
const std::string Reward_Fields::LOGIN_DAYS = "login_days";
const std::string Reward_Fields::RECEIVE_SEVEN_DAYS = "receive_seven_days";
const std::string Reward_Fields::ACTIVATION_CODE_RECEIVE_INFO = "activation_code_receive_info";
const std::string Reward_Fields::RB_ALL_NUM = "rb_all_num";
const std::string Reward_Fields::RB_ITEM_VALUE = "rb_item_value";
const std::string Reward_Fields::RB_ITEM_VALUE_TMP = "rb_item_value_tmp";
const std::string Reward_Fields::RB_RECEIVE_TIME = "re_receive_time";
const std::string Reward_Fields::RB_FIRST_TIME = "re_first_time";
const std::string Reward_Fields::KEEP_REWARD_STATUS = "keep_reward_status";
const std::string Reward_Fields::KEY = "key";
const std::string Reward_Fields::VALUE = "value";
const std::string Reward_Fields::Activation_Code_Receive_Info::TYPE = "type";
const std::string Reward_Fields::Activation_Code_Receive_Info::NUM = "num";
/*==========Reward END====================================================*/

/*==========Activation_Code start====================================================*/
const std::string Activation_Code_Fields::COLLECTION = "backstage.gift";
const std::string Activation_Code_Fields::KEY = "key";

const std::string Activation_Receive_Id_Fields::COLLECTION = "backstage.gift_receive_id";
const std::string Activation_Receive_Id_Fields::ID = "id";
/*==========Activation_Code END====================================================*/

/*==========Campfire start====================================================*/
const std::string Campfire_Fields::COLLECTION = "mmo.campfire";
const std::string Campfire_Fields::ID = "id";
const std::string Campfire_Fields::LEVEL = "level";
const std::string Campfire_Fields::GANG_ID = "gang_id";
const std::string Campfire_Fields::GANG_NAME = "gang_name";
const std::string Campfire_Fields::FIREWOOD_NUM = "firewood_num";
const std::string Campfire_Fields::BURNING = "burning";
const std::string Campfire_Fields::LEVEL_FIREWOOD_NUM = "level_firewood_num";
const std::string Campfire_Fields::LEVEL_MAX_FIREWOOD_NUM = "level_max_firewood_num";

/*==========Campfire end====================================================*/


/*==========world boss start====================================================*/
const std::string World_Boss_Fields::COLLECTION = "mmo.world_boss";
const std::string World_Boss_Fields::ACT_ID = "act_id";
const std::string World_Boss_Fields::ACT_STATE = "act_state";
const std::string World_Boss_Fields::OPEN_NUM = "open_num";
const std::string World_Boss_Fields::KILL_NUM = "kill_num";
const std::string World_Boss_Fields::BOSS_LV = "boss_lv";
/*==========world boss end====================================================*/

/*==========FB_info start====================================================*/
const std::string FB_Info_Fields::COLLECTION = "mmo.fb_info";
const std::string FB_Info_Fields::ROLE_ID = "role_id";
const std::string FB_Info_Fields::BEST_FORCE = "best_force";
const std::string FB_Info_Fields::BEST_ROLE_ID = "best_role_id";
const std::string FB_Info_Fields::BEST_ROLE_NAME = "best_role_name";
/*==========FB_info end====================================================*/

/*==========active content start====================================================*/
const std::string Active_Content_Fields::COLLECTION = "mmo.active_content";
const std::string Active_Content_Fields::VIP_CLUB = "vip_club";
const std::string Active_Content_Fields::LEVEL_ACE = "level_ace";
const std::string Active_Content_Fields::SUIT_COLLECT = "suit_collect";
const std::string Active_Content_Fields::GUILD_LEVEL = "guild_level";
const std::string Active_Content_Fields::GEM_LEVELUP = "gem_levelup";
const std::string Active_Content_Fields::WING_LEVELUP = "wing_levelup";
const std::string Active_Content_Fields::MOUNT_LEVELUP = "mount_levelup";
const std::string Active_Content_Fields::SG_PASS = "sg_pass";
const std::string Active_Content_Fields::MOUNT_PASS = "mount_pass";
const std::string Active_Content_Fields::WING_PASS = "wing_pass";
const std::string Active_Content_Fields::ARENA_DAN = "arena_dan";
const std::string Active_Content_Fields::ARENA_DAN_TIME = "arena_dan_t";
const std::string Active_Content_Fields::SUIT_CNT = "suit_cnt";
const std::string Active_Content_Fields::COM_REC_ID = "com_rec_id";
const std::string Active_Content_Fields::ALL_WORLD_BOSS_HURT = "all_world_boss_hurt";
const std::string Active_Content_Fields::TODAY_WORLD_BOSS_HURT = "today_world_boss_hurt";
const std::string Active_Content_Fields::WORLD_BOSS_MAX_RANK = "world_boss_max_rank";
const std::string Active_Content_Fields::HEROES_DOOR_TYPE_LV = "heroes_door_type_lv";
const std::string Active_Content_Fields::TEAM_ARENA_WNUM = "team_arena_wnum";
const std::string Active_Content_Fields::WARER_WNUM = "warer_wnum";
const std::string Active_Content_Fields::REW_TASK_DONE_NUM = "rew_task_done_num";
const std::string Active_Content_Fields::ON_SEND_MAIL_ACT_ID = "on_send_mail_act_id";
const std::string Active_Content_Fields::ENCHANTING_NUM = "enchanting_num";
const std::string Active_Content_Fields::DRAGON_PREY_NUM = "dragon_prey_num";
const std::string Active_Content_Fields::RING_TASK_RECHARGE = "ring_task_recharge";
const std::string Active_Content_Fields::MOUNT_LVUP_NUM = "mount_lvup_num";
const std::string Active_Content_Fields::FUSION_X_ELITE_CARD_NUM = "fusion_x_elite_card_num";
const std::string Active_Content_Fields::ALCHEMY_NUM = "alchemy_num";
const std::string Active_Content_Fields::COMMON_LSTIME = "common_lstime";
const std::string Active_Content_Fields::COMMON_TAI = "common_tai";
const std::string Active_Content_Fields::KEY = "k";
const std::string Active_Content_Fields::VAL = "v";

const std::string Active_Content_Info_Fields::VIP_AN_LV = "vip_an_lv";
const std::string Active_Content_Info_Fields::LRC = "lrc";
const std::string Active_Content_Info_Fields::ARC = "arc";
const std::string Active_Content_Info_Fields::FRC = "frc";
const std::string Active_Content_Info_Fields::SGRC = "sgrc";
const std::string Active_Content_Info_Fields::MOUNT_COUNT = "mount_count";
const std::string Active_Content_Info_Fields::WING_COUNT = "wing_count";
const std::string Active_Content_Info_Fields::LRT = "lrt";
const std::string Active_Content_Info_Fields::FRT = "frt";
const std::string Active_Content_Info_Fields::SGRT = "sgrt";
const std::string Active_Content_Info_Fields::MOUNT_TIME = "mount_time";
const std::string Active_Content_Info_Fields::WING_TIME = "wing_time";
const std::string Active_Content_Info_Fields::ADST = "adst";
const std::string Active_Content_Info_Fields::ART = "art";
const std::string Active_Content_Info_Fields::ADLI = "adli";
const std::string Active_Content_Info_Fields::COM_LSTIME = "com_lstime";
const std::string Active_Content_Info_Fields::LR_ROLE = "lr_role";
const std::string Active_Content_Info_Fields::AR_ROLE = "ar_role";
const std::string Active_Content_Info_Fields::SGR_ROLE = "sgr_role";
const std::string Active_Content_Info_Fields::MOUNT_ROLE = "mount_role";
const std::string Active_Content_Info_Fields::WING_ROLE = "wing_role";
const std::string Active_Content_Info_Fields::FR_ROLE = "fr_role";
const std::string Active_Content_Info_Fields::Role_Info::ROLE_ID = "role_id";
const std::string Active_Content_Info_Fields::Role_Info::ROLE_NAME = "role_name";
const std::string Active_Content_Info_Fields::Role_Info::HPI = "hpi";
const std::string Active_Content_Info_Fields::Role_Info::GENDER = "gender";
const std::string Active_Content_Info_Fields::Role_Info::GUILD_NAME = "guild_name";
const std::string Active_Content_Info_Fields::Role_Info::CAREER = "career";
const std::string Active_Content_Info_Fields::Role_Info::LEVEL = "level";
const std::string Active_Content_Info_Fields::Role_Info::VIP_LEVEL = "vip_level";
const std::string Active_Content_Info_Fields::Role_Info::COMBAT = "combat";
const std::string Active_Content_Info_Fields::Role_Info::TITLE_ID = "title_id";

/*==========active content end====================================================*/

/*==========Common start====================================================*/
const std::string Common_Fields::COLLECTION = "mmo.common";
const std::string Common_Fields::TYPE = "type";
const std::string Common_Fields::VALUE = "value";
const std::string Common_Fields::VALUE_1 = "value1";
const std::string Common_Fields::VALUE_2 = "value2";
const std::string Common_Fields::VALUE_3 = "value3";
/*==========Common end====================================================*/

const std::string Access_Count_Fields::COLLECTION = "backstage.access_count";
const std::string Access_Count_Fields::TYPE = "type";
const std::string Access_Count_Fields::H_TIME = "h_time";
const std::string Access_Count_Fields::NUM = "num";

const std::string Proposal_Fields::COLLECTION = "backstage.proposal";
const std::string Proposal_Fields::TYPE = "type";
const std::string Proposal_Fields::ROLE_ID = "role_id";
const std::string Proposal_Fields::CONTENTS = "contents";
const std::string Proposal_Fields::TIME = "time";
const std::string Proposal_Fields::REPLY = "reply";

/*=================Fashion start=====================================================*/
const std::string Fashion_Fields::COLLECTION = "mmo.fashion2";
const std::string Fashion_Fields::ROLE_ID = "role_id";
const std::string Fashion_Fields::SHOW_STATE = "show_state";
const std::string Fashion_Fields::FASHION_PART = "parts";
const std::string Fashion_Fields::Fashion_Part::ID = "part_id";
const std::string Fashion_Fields::Fashion_Part::LV = "lv";
const std::string Fashion_Fields::Fashion_Part::EXP = "exp";
const std::string Fashion_Fields::Fashion_Part::USED_ID = "used_id";
const std::string Fashion_Fields::Fashion_Part::IDS = "ids";
/*=================Fashion end=====================================================*/

const std::string Gm_Ann_Fields::COLLECTION = "backstage.gm_announce";
const std::string Gm_Ann_Fields::TYPE = "type";
const std::string Gm_Ann_Fields::ID = "id";
const std::string Gm_Ann_Fields::STIME = "stime";
const std::string Gm_Ann_Fields::ENDTIME = "etime";
const std::string Gm_Ann_Fields::STEP = "step";
const std::string Gm_Ann_Fields::CONTENTS = "contents";
const std::string Gm_Ann_Fields::STATUS = "status";

const std::string Ban_Gag_Fields::COLLECTION = "backstage.ban_gag";
const std::string Ban_Gag_Fields::ACCOUNT = "account";
const std::string Ban_Gag_Fields::ROLE_ID = "role_id";
const std::string Ban_Gag_Fields::UNBAN_TIME = "unban_time";

/*=================Auction start=====================================================*/
const std::string Auction_Goods_Fields::COLLECTION = "mmo.auction_goods";
const std::string Auction_Goods_Fields::ID = "id";
const std::string Auction_Goods_Fields::ITEM_ID = "item_id";
const std::string Auction_Goods_Fields::NUM = "num";
const std::string Auction_Goods_Fields::AUCTION_PRICE = "auction_price";
const std::string Auction_Goods_Fields::ONES_PRICE = "ones_price";
const std::string Auction_Goods_Fields::OVER_TIME = "over_time";
const std::string Auction_Goods_Fields::START_TIME = "start_time";
const std::string Auction_Goods_Fields::CONTINUE_TIME = "continue_time";
const std::string Auction_Goods_Fields::ISSUE_ROLE_ID = "issue_role_id";
const std::string Auction_Goods_Fields::AUCTION_ROLE_ID = "auction_role_id";
const std::string Auction_Goods_Fields::L_AUCTION_PRICE = "l_auction_price";
const std::string Auction_Goods_Fields::AUCTION_FAIL_ROLE_ID = "auction_fail_role_id";
/*=================Auction end=====================================================*/

/*=================Knight_Trials_Fields start=====================================================*/
const std::string Knight_Trials_Fields::COLLECTION = "mmo.knight_trials";
const std::string Knight_Trials_Fields::CUR_POINTS = "cur_points";
const std::string Knight_Trials_Fields::ROLE_ID_SET = "role_id_set";
const std::string Knight_Trials_Fields::MATCH_DAY = "match_day";
const std::string Knight_Trials_Fields::CUR_RECEIVE = "cur_receive";
const std::string Knight_Trials_Fields::RESET_STATUS = "reset_status";
const std::string Knight_Trials_Fields::REWARD = "reward";
const std::string Knight_Trials_Fields::RECODE_REWARD = "recode_reward";
const std::string Knight_Trials_Fields::Reward_Info::ID = "id";
const std::string Knight_Trials_Fields::Reward_Info::VAL = "val";
const std::string Knight_Trials_Fields::Reward_Info::TYPE = "type";

const std::string Knight_Trials_Player_Fields::COLLECTION = "mmo.knight_trials_player";
const std::string Knight_Trials_Player_Fields::CUR_PLAYER = "cur_player";
const std::string Knight_Trials_Player_Fields::HIDE_PLAYER = "hide_player";
const std::string Knight_Trials_Player_Fields::Player_Detail::BASE = "base";
const std::string Knight_Trials_Player_Fields::Player_Detail::FIGHTER = "fighter";
const std::string Knight_Trials_Player_Fields::Player_Detail::HEROER = "heroer";
const std::string Knight_Trials_Player_Fields::Player_Detail::UUID = "uuid";
const std::string Knight_Trials_Player_Fields::Player_Detail::SERVER_NAME = "server_name";
/*=================Knight_Trials_Fields end=====================================================*/

/// ANCIARTIFACT START //////////////////////////////////////////////
const std::string Anci_Artifact_Fields::COLLECTION = "mmo.anci_artifact";
const std::string Anci_Artifact_Fields::ROLE_ID = "role_id";
const std::string Anci_Artifact_Fields::PERFECTPOINT = "perfectPoint";
const std::string Anci_Artifact_Fields::AA_COMPLETE_IDS = "complete_ids";

const std::string Anci_Artifact_Fields::AA_INFO_MAP = "aa_info";
const std::string Anci_Artifact_Fields::AA_Info::ID = "id";
const std::string Anci_Artifact_Fields::AA_Info::DATA = "data";
const std::string Anci_Artifact_Fields::AA_Info::STATE_COUNT = "state_count";

const std::string Anci_Artifact_Fields::AA_UNBAN_INFO = "aa_unban_info";
const std::string Anci_Artifact_Fields::AA_Unban_Info::ID = "id";
const std::string Anci_Artifact_Fields::AA_Unban_Info::STATE = "state";
const std::string Anci_Artifact_Fields::AA_Unban_Info::BOX_STATE = "box_state";
/// ANCIARTIFACT START //////////////////////////////////////////////

/*=================Wing_Fields start============================================================*/
const std::string Wing_Fields::COLLECTION 	= "mmo.wing";
const std::string Wing_Fields::ROLE_ID 		= "role_id";
const std::string Wing_Fields::LV			= "lv";
const std::string Wing_Fields::RATE 		= "rate";
const std::string Wing_Fields::PROGRESS		= "progress";
const std::string Wing_Fields::TIMES		= "times";
const std::string Wing_Fields::LAST_TIME	= "last_time";
const std::string Wing_Fields::SHOW_STATE	= "show_state";
const std::string Wing_Fields::STATE = "state";
const std::string Wing_Fields::GOT_REWARD_STAGE = "got_reward_stage";
const std::string Wing_Fields::ACTIVE_TIME = "active_time";
/*=================Wing_Fields end===============================================================*/

/*============invest plan begin=================================================================*/
const std::string Invest_Plan_Fields::COLLECTION = "mmo.invest_plan";
const std::string Invest_Plan_Fields::ROLE_ID = "role_id";
const std::string Invest_Plan_Fields::IS_MONTH_CARD_INVEST = "is_month_card_invest";
const std::string Invest_Plan_Fields::LEFT_DAY = "left_day";
const std::string Invest_Plan_Fields::DAYS = "days";
const std::string Invest_Plan_Fields::MONTH_CARD_PROFIT_BUTTON_LIST = "month_card_profit_button_list";
const std::string Invest_Plan_Fields::Month_Card_Profit_Button::DAY = "day";
const std::string Invest_Plan_Fields::Month_Card_Profit_Button::STATUS = "status";
const std::string Invest_Plan_Fields::INVESTED_MONEY = "invested_money";
const std::string Invest_Plan_Fields::INVEST_BUTTON = "invest_button";
const std::string Invest_Plan_Fields::UPGRADE_INVEST_PROFIT_BUTTON_LIST = "upgrade_invest_profit_button_list";
const std::string Invest_Plan_Fields::Upgrade_Invest_Profit_Button::LEVEL = "level";
const std::string Invest_Plan_Fields::Upgrade_Invest_Profit_Button::STATUS = "status";

const std::string Invest_Plan_Fields::CAN_INVEST_MONEY_LIST = "can_invest_money_list";
const std::string Invest_Plan_Fields::Can_Invest_Money::MONEY = "money";

const std::string Invest_Plan_Fields::INVESTED_MONEY_LIST = "invested_money_list";
const std::string Invest_Plan_Fields::Invested_Money::MONEY = "money";

const std::string Invest_Plan_Fields::GETTED_UPGRADE_INVEST_PROFIT_MAP = "getted_upgrade_invest_profit_map";
const std::string Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::LEVEL = "level";
const std::string Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::GETTED_UPGRADE_INVEST_PROFIT_LIST = "getted_upgrade_invest_profit_list";
const std::string Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::Getted_Upgrade_Invest_Profit::MONEY = "money";

/*============invest plan end=================================================================*/

/*============invest plan history begin=================================================================*/
const std::string Invest_Plan_History_Fields::COLLECTION = "mmo.invest_plan_history";
const std::string Invest_Plan_History_Fields::HISTORY_LIST = "history_list";
const std::string Invest_Plan_History_Fields::History_Record::INVEST_TYPE = "invest_type";
const std::string Invest_Plan_History_Fields::History_Record::HISTORY_TYPE = "history_type";
const std::string Invest_Plan_History_Fields::History_Record::MONEY = "money";
const std::string Invest_Plan_History_Fields::History_Record::PLAYER_NAME = "player_name";
/*============invest plan history end=================================================================*/

/*============ThorLottery begin===========================================================================*/
const std::string ThorLottery_Manager_Fields::SEASON_ID = "season_id";
const std::string ThorLottery_Manager_Fields::STATE = "state";
const std::string ThorLottery_Manager_Fields::ICON_ID = "icon_id";
const std::string ThorLottery_Manager_Fields::LV_LIMIT = "lv_limit";
const std::string ThorLottery_Manager_Fields::THORLOTTERY_LOG = "thorlottery_log";
const std::string ThorLottery_Manager_Fields::ThorLottery_Log::TIME = "time";
const std::string ThorLottery_Manager_Fields::ThorLottery_Log::ROLE_NAME = "role_name";
const std::string ThorLottery_Manager_Fields::ThorLottery_Log::ROLE_ID = "role_id";
const std::string ThorLottery_Manager_Fields::ThorLottery_Log::ITEM_ID = "item_id";
const std::string ThorLottery_Manager_Fields::ThorLottery_Log::ITEM_NUM = "item_num";
const std::string ThorLottery_Manager_Fields::ThorLottery_Log::BIND_STATE = "bind_state";
const std::string ThorLottery_Manager_Fields::THORLOTTERR_ITEM = "thorlottery_item";
const std::string ThorLottery_Manager_Fields::ThorLottery_Item::INDEX = "index";
const std::string ThorLottery_Manager_Fields::ThorLottery_Item::NUM = "num";

const std::string ThorLottery_Fields::COLLECTION = "mmo.thorlottery";
const std::string ThorLottery_Fields::ROLE_ID = "role_id";
const std::string ThorLottery_Fields::SEASON_ID = "season_id";
const std::string ThorLottery_Fields::TIME = "time";
const std::string ThorLottery_Fields::SCORE = "score";
const std::string ThorLottery_Fields::DAILY_LOTTERY_TIMES = "daily_lottery_times";
const std::string ThorLottery_Fields::THORLOTTERY_ITEM = "thorlottery_item";
const std::string ThorLottery_Fields::ThorLottery_Item::INDEX = "index";
const std::string ThorLottery_Fields::ThorLottery_Item::NUM = "num";
/*============ThorLottery end=============================================================================*/


/*============Answer strat=============================================================================*/
const std::string Answer_Info_Fields::EXT_NUM = "ext_num";
const std::string Answer_Info_Fields::CUR_QNO = "cur_qno";
const std::string Answer_Info_Fields::QID_LIST = "qid_list";
const std::string Answer_Info_Fields::ROLE_SORCE_VEC = "role_sorce";
const std::string Answer_Info_Fields::ROLE_DATA_VEC = "role_data";
const std::string Answer_Info_Fields::KEY = "key";
const std::string Answer_Info_Fields::VALUE = "value";
const std::string Answer_Info_Fields::LAST_TIME = "last_time";
const std::string Answer_Info_Fields::FORCE_RANK = "force_rank";
const std::string Answer_Info_Fields::ARENA_RANK = "arena_rank";
const std::string Answer_Info_Fields::GANG_RANK = "gang_rank";
const std::string Answer_Info_Fields::Rank_Role_Data::ROLE_ID = "role_id";
const std::string Answer_Info_Fields::Rank_Role_Data::NAME = "name";
const std::string Answer_Info_Fields::Rank_Role_Data::LEVEL = "level";
const std::string Answer_Info_Fields::Rank_Role_Data::SORCE = "sorce";
const std::string Answer_Info_Fields::Rank_Role_Data::TRUE_NUM = "true_num";
const std::string Answer_Info_Fields::Rank_Role_Data::EXP = "exp";
const std::string Answer_Info_Fields::Role_Info_Data::FREE_NUM = "free_num";
const std::string Answer_Info_Fields::Role_Info_Data::DOUBLE_NUM = "double_num";
const std::string Answer_Info_Fields::Role_Info_Data::CUR_FREE_QID = "cur_f_qid";
const std::string Answer_Info_Fields::Role_Info_Data::CUR_DOUBLE_QID = "cur_d_qid";
/*============Answer end=============================================================================*/


const std::string Activity_Fields::COLLECTION = "mmo.activity";
const std::string Activity_Fields::ROLE_ID = "role_id";
const std::string Activity_Fields::VIP37_RIDS = "vip37_rids";
const std::string Activity_Fields::IS_REC_37PHONE = "is_rec_37phone";
const std::string Activity_Fields::IS_REC_R2_LOVE_GIFT = "is_rec_r2_love_gift";

const std::string Questionnaite_Info_Fields::COLLECTION = "backstage.questionnaite";
const std::string Questionnaite_Info_Fields::LIST = "list";
const std::string Questionnaite_Info_Fields::List::QNO = "qno";
const std::string Questionnaite_Info_Fields::List::RADIO = "radio";
const std::string Questionnaite_Info_Fields::List::MULT = "mult";
const std::string Questionnaite_Info_Fields::List::CONTENTS = "contents";

const std::string Active_Event_Info_Fields::COLLECTION = "backstage.event";
const std::string Active_Event_Info_Fields::ID = "event_id";
const std::string Active_Event_Info_Fields::START_TIME = "start_time";
const std::string Active_Event_Info_Fields::END_TIME = "end_time";
const std::string Active_Event_Info_Fields::MID_END_TIME = "mid_end_time";

const std::string Two_Recharge_Info_Fields::REC_NUM = "rec_num";
const std::string Two_Recharge_Info_Fields::LAST_TIME = "last_time";


/*============center server begin===========================================================================*/
const std::string Inner_Check_Info_Fields::COLLECTION = "mmo.inner_check_info";
const std::string Inner_Check_Info_Fields::SERVER = "server";
const std::string Inner_Check_Info_Fields::CORE_NUM = "core_num";
const std::string Inner_Check_Info_Fields::UPDATE_TIME = "time";
const std::string Inner_Check_Info_Fields::MONGO_IP_PORT = "mongo_ip_port";
/*============center server end=============================================================================*/

/*===============expedition begin ==============================*/
const std::string Expedition_Fields::COLLECTION = "mmo.expedition";
const std::string Expedition_Fields::KILL_BEAST = "kill_beast";
const std::string Expedition_Fields::KEY = "key";
const std::string Expedition_Fields::VAL = "val";
const std::string Expedition_Fields::COLLECT_CHEST = "collect_chest";
const std::string Expedition_Fields::COLLECT_BEAST = "collect_beast";
const std::string Expedition_Fields::BOX_COLLECT = "box_collect";
const std::string Expedition_Fields::ID = "id";

const std::string Expedition_Manager_Detail_Fields::COLLECTION = "mmo.expedition_manager";
const std::string Expedition_Manager_Detail_Fields::BAK_COLLECTION = "mmo.expedition_manager_bak";
const std::string Expedition_Manager_Detail_Fields::GANG_DATA_INFO_MAP = "gang_data_info_map";
const std::string Expedition_Manager_Detail_Fields::AWARD = "award";
const std::string Expedition_Manager_Detail_Fields::Gang_Data_Info_Map::LINE = "line";
const std::string Expedition_Manager_Detail_Fields::FLAG_TIME = "flag_time";
const std::string Expedition_Manager_Detail_Fields::FIRST_LISTEN_TIME = "first_listen_time";

const std::string Expedition_Global_Manager_Data_Fields::COLLECTION = "mmo.expedition_g_manager_data";
const std::string Expedition_Global_Manager_Data_Fields::BAK_COLLECTION = "mmo.expedition_g_manager_data_b";
const std::string Expedition_Global_Manager_Data_Fields::OPENED_UUID_OPENTIME_MAP = "opened_uuid_opentime_map";
const std::string Expedition_Global_Manager_Data_Fields::UUID = "uuid";
const std::string Expedition_Global_Manager_Data_Fields::UUID_INTER_MAP = "uuid_inter_map";
const std::string Expedition_Global_Manager_Data_Fields::INTER = "inter";
const std::string Expedition_Global_Manager_Data_Fields::OPENTIME = "opentime";

const std::string Stronghold_Info_Fields::OCCUPIER = "occupier";
const std::string Stronghold_Info_Fields::SOLDIER_MAP = "soldier_map";
const std::string Stronghold_Info_Fields::NUMS = "nums";

const std::string Stronghold_Fields::TYPE = "type";
const std::string Stronghold_Fields::INFO = "info";

const std::string Expedition_Scene_Data_Fields::MAP_DATA = "map_data";
const std::string Expedition_Scene_Data_Fields::POINT_GANG_ID = "point_gang_id";
const std::string Expedition_Scene_Data_Fields::MAP_BASE_POINT_NUM = "map_base_point_num";
const std::string Expedition_Scene_Data_Fields::DEMAGE_RANK = "demage_rank";
const std::string Expedition_Scene_Data_Fields::SETTLE_TIME = "settle_time";
const std::string Expedition_Scene_Data_Fields::OCCUPY_CASTLE_RANK = "occupy_castle_rank";
const std::string Expedition_Scene_Data_Fields::OCCUPY_CASTLE_NUMS = "occupy_castle_nums";
const std::string Expedition_Scene_Data_Fields::DEMAGE = "demage";
const std::string Expedition_Scene_Data_Fields::GET_NUMS = "get_nums";
const std::string Expedition_Scene_Data_Fields::SERVER_NAME = "server_name";
const std::string Expedition_Scene_Data_Fields::DROP_OR_MATERIAL = "drop_or_material";
const std::string Expedition_Scene_Data_Fields::HAD_COLLECTED = "had_collected";
const std::string Expedition_Scene_Data_Fields::REFRESHED_MONSTER_TIMING = "refreshed_monster_timing";
const std::string Expedition_Scene_Data_Fields::REFRESHED_MONSTER_KILL_GAP = "refreshed_monster_kill_gap";

const std::string Expedition_Manager_Data_Fields::COLLECTION = "mmo.expedition_manager_data";
const std::string Expedition_Manager_Data_Fields::INTER = "inter";
const std::string Expedition_Manager_Data_Fields::OPENED_UUID_OPENTIME_MAP = "opened_uuid_opetime_map";
const std::string Expedition_Manager_Data_Fields::GANG_ID_UUID_MAP = "gang_id_uuid_map";
const std::string Expedition_Manager_Data_Fields::LAST_RESET_TIME_SEC = "last_reset_time_sec";
const std::string Expedition_Manager_Data_Fields::LAST_RESET_TIME_USEC = "last_reset_time_usec";
const std::string Expedition_Manager_Data_Fields::FIRST_GANG_RANK = "first_gang_rank";
const std::string Expedition_Manager_Data_Fields::RANK = "rank";
const std::string Expedition_Manager_Data_Fields::HAS_INIT = "has_init";
const std::string Expedition_Manager_Data_Fields::UUID_HAS_INIT = "uuid_has_init";
const std::string Expedition_Manager_Data_Fields::GANG_RANK_MAP = "gang_rank_map";
const std::string Expedition_Manager_Data_Fields::GANG_LEADER_NAME = "gang_leader_name";
const std::string Expedition_Manager_Data_Fields::NUM = "num";
const std::string Expedition_Manager_Data_Fields::ASSET = "asset";
const std::string Expedition_Manager_Data_Fields::CREATE_TIME = "create_time";
const std::string Expedition_Manager_Data_Fields::EXPEDITION_RANK = "expedition_rank";

const std::string Expedition_Manager_Data_Fields::UUID_GANG_RANK_MAP = "uuid_gang_rank_map";
const std::string Expedition_Manager_Data_Fields::GANG_RANK_SYNC_SUCCESS = "gang_rank_sync_success";
const std::string Expedition_Manager_Data_Fields::GANG_RANK_SYNC_NEW_VEC = "gang_rank_sync_new_vec";
const std::string Expedition_Manager_Data_Fields::UUID_GANG_RANK_SYNC_NEW_VEC = "uuid_gang_rank_sync_new_vec";
const std::string Expedition_Manager_Data_Fields::GANG_ID_POINT_ID = "gang_id_point_id";
const std::string Expedition_Manager_Data_Fields::POINT_ID = "point_id";
const std::string Expedition_Manager_Data_Fields::GANG_ID_CUR_LINE = "gang_id_cur_line";
const std::string Expedition_Manager_Data_Fields::CUR_LINE = "cur_line";

const std::string Expedition_Manager_Data_Fields::UUID_GANG_ID_POINT_ID = "uuid_gang_id_point_id";
const std::string Expedition_Manager_Data_Fields::UUID_GANG_ID_CUR_LINE = "uuid_gang_id_cur_line";

const std::string Expedition_Manager_Data_Fields::LINE_SCENE_MAP = "line_scene_map";
const std::string Expedition_Manager_Data_Fields::UUID_LINE_SCENE_MAP = "uuid_line_scene_map";
/*===============expedition end ================================*/

/*==========活动兑换商店物品信息START====================================================*/
const std::string Activities_Exchange_Stores_Fields::COLLECTION = "backstage.exchange";
const std::string Activities_Exchange_Stores_Fields::ID = "id";
const std::string Activities_Exchange_Stores_Fields::ITEM_ID = "item_id";//物品ID
const std::string Activities_Exchange_Stores_Fields::ITEM_NUM ="item_num";//
const std::string Activities_Exchange_Stores_Fields::EXCHANGE_ID = "exchange_id";//物品ID
const std::string Activities_Exchange_Stores_Fields::EXCHANGE_NUM = "exchange_num";//
const std::string Activities_Exchange_Stores_Fields::LIMIT_LV = "limit_lv";//
const std::string Activities_Exchange_Stores_Fields::LIMIT_EXCHAGNGE = "limit_exchange";//
const std::string Activities_Exchange_Stores_Fields::CLASSIFY = "classify";//
const std::string Activities_Exchange_Stores_Fields::SUBCLASS = "subclass";//
const std::string Activities_Exchange_Stores_Fields::ACT_ID = "act_id";//
const std::string Activities_Exchange_Stores_Fields::SORT = "sort";
//detail
const std::string Activities_Exchange_Stores_Detail_Fields::COLLECTION="mmo.activities_stores";//集合名
const std::string Activities_Exchange_Stores_Detail_Fields::ID_NUM_BUY = "id_num_buy";
/*==========活动兑换商店物品信息END====================================================*/

const std::string Active_Data_Fields::ID = "id";
const std::string Active_Data_Fields::NUM = "num";
const std::string Active_Data_Fields::BEGIN_TIME = "begin_time";
const std::string Active_Data_Fields::END_TIME = "end_time";

//
const std::string Life_Skill_Data_Fields::COLLECTION = "mmo.life_skill";
const std::string Life_Skill_Data_Fields::ROLE_ID = "role_id";
const std::string Life_Skill_Data_Fields::ENERGY = "energy";
const std::string Life_Skill_Data_Fields::RE_TIME = "re_time";
const std::string Life_Skill_Data_Fields::INIL = "inil";
const std::string Life_Skill_Data_Fields::LF_SKILL_LIST = "lf_skill_list";
const std::string Life_Skill_Data_Fields::Life_Skill_Data::SKILL_ID = "skill_id";
const std::string Life_Skill_Data_Fields::Life_Skill_Data::DEGREE = "degree";

//oper activity
const std::string Oper_Activity_Data_Fields::COLLECTION = "mmo.oper_activity";
const std::string Oper_Activity_Data_Fields::LAST_ITEM_START_TIME = "last_item_start_time";//每一项活动ID对应最后一次开始时间
const std::string Oper_Activity_Data_Fields::ACTID_TYPE_RECORD_VAL = "actid_type_record_val";//活动ID->类型->完成记录(次数,类型->值)
const std::string Oper_Activity_Data_Fields::ACTID_RECEIVE_ID = "actid_receive_id";//活动ID对应已经领取的ID集合
const std::string Oper_Activity_Data_Fields::ACTID_RECEIVE_ID_NUM = "actid_receive_id_num";//活动ID->(项目)领取ID->领取次数
const std::string Oper_Activity_Data_Fields::ACTID_APPEND_EFFECT = "actid_append_effect";//活动ID执行过的附加效果
const std::string Oper_Activity_Data_Fields::ACTID_RECID_LAST_REFRESH_TIME = "actid_recid_last_refresh_time";//活动ID->(项目)领取ID->最后一次刷新时间
const std::string Oper_Activity_Data_Fields::ACTID_LOGIN_DAY = "actid_login_day";//活动ID->login day 时间
const std::string Oper_Activity_Data_Fields::Record::NUM = "num";
const std::string Oper_Activity_Data_Fields::Record::EXT1 = "ext1";
const std::string Oper_Activity_Data_Fields::Record::EXT2 = "ext2";
const std::string Oper_Activity_Data_Fields::Record::EXT3 = "ext3";
const std::string Oper_Activity_Data_Fields::Record::TYPE_NUM = "type_num";
const std::string Oper_Activity_Data_Fields::Common::KEY = "k";
const std::string Oper_Activity_Data_Fields::Common::VAL = "v";

// pet
const std::string Pet_Fields::COLLECTION = "mmo.pet";
const std::string Pet_Fields::ROLE_ID = "role_id";
const std::string Pet_Fields::LV = "lv";
const std::string Pet_Fields::STATE = "state";
const std::string Pet_Fields::UPGRADE_VAL = "upgrade_val";
const std::string Pet_Fields::ACTIVE_ILLUSION_ID = "active_illusion_id";
const std::string Pet_Fields::SKILLS = "skills";
const std::string Pet_Fields::Pet_Skill::ID = "id";
const std::string Pet_Fields::Pet_Skill::LOC = "loc";
const std::string Pet_Fields::ILLUSIONS = "illusions";
