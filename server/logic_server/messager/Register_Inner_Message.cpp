/*
 * Register_Inner_Message.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: ti
 */

#include "Logic_Message_Unit.h"
#include "Logic_Server_Request.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Php_Enum.h"

using namespace Server_Enum;

int Logic_Message_Unit::register_inner_message(void) {
	register_server_process(SERVER_INNER_ABNORMAL, SCT_CALLBACK_4(Logic_Server_Request::abnormal_logout, LOGIC_SERVER_REQUEST));
	register_server_process(LOGIC_SCENE_LOGIN_READY, SCT_CALLBACK_4(Logic_Server_Request::scene_login_ready, LOGIC_SERVER_REQUEST));
	register_server_process(LOGIC_GATE_LOGIN_READY, SCT_CALLBACK_4(Logic_Server_Request::gate_login_ready, LOGIC_SERVER_REQUEST));
	register_server_process(LOGIC_CHAT_LOGIN_READY, SCT_CALLBACK_4(Logic_Server_Request::chat_login_ready, LOGIC_SERVER_REQUEST));
	register_server_process(GATE_LOGIC_CLIENT_DROP, SCT_CALLBACK_4(Logic_Server_Request::player_sign_out, LOGIC_SERVER_REQUEST));
	register_server_process(LOGIC_SCENE_LOGOUT_READY, SCT_CALLBACK_4(Logic_Server_Request::scene_logout_ready, LOGIC_SERVER_REQUEST));
	register_server_process(LOGIC_GATE_LOGOUT_READY, SCT_CALLBACK_4(Logic_Server_Request::gate_logout_ready, LOGIC_SERVER_REQUEST));
	register_server_process(LOGIC_CHAT_LOGOUT_READY, SCT_CALLBACK_4(Logic_Server_Request::chat_logout_ready, LOGIC_SERVER_REQUEST));
	register_server_process(GATE_LOGIC_CLIENT_LOGIN, SCT_CALLBACK_4(Logic_Server_Request::gate_client_login, LOGIC_SERVER_REQUEST));
	register_server_process(LOGIC_SYNC_LOGIN_SESSION, SCT_CALLBACK_4(Logic_Server_Request::process_login_session, LOGIC_SERVER_REQUEST));
	register_server_process(GATE_LOGIC_TRUSTEESHIP, SCT_CALLBACK_4(Logic_Server_Request::player_trusteeship, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_NOTICE_SIGN_SCENE, SCT_CALLBACK_4(Logic_Server_Request::process_20100012, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_NOTICE_ENTER_SCENE, SCT_CALLBACK_4(Logic_Server_Request::process_20100013, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_LEARN_SKILL_COST_ITEM, SCT_CALLBACK_4(Logic_Server_Request::process_20100016, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_AFTER_LEARN_SKILL, SCT_CALLBACK_4(Logic_Server_Request::process_20100017, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_FORCE_TO_LOGIC, SCT_CALLBACK_4(Logic_Server_Request::process_20100027, LOGIC_SERVER_REQUEST));
	register_server_process(SERVER_INNER_HEARTBEAT_BACK, SCT_CALLBACK_4(Logic_Server_Request::process_20000052, LOGIC_SERVER_REQUEST));
	register_server_process(SYSN_ENERGY_TO_LOGIC, SCT_CALLBACK_4(Logic_Server_Request::process_20100031, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_SYNC_MOVER_DETAIL, SCT_CALLBACK_4(Logic_Server_Request::process_20200010, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_GATE_ASK_LOGIC_TRAN, SCT_CALLBACK_4(Logic_Server_Request::process_20010050, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_TRAN_SCENE_SAME_PRO, SCT_CALLBACK_4(Logic_Server_Request::process_20200050, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ASK_SCENE_SIGN_OUT, SCT_CALLBACK_4(Logic_Server_Request::process_20200055, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ASK_SCENE_SIGN_IN, SCT_CALLBACK_4(Logic_Server_Request::process_20200056, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_FIGHTER_DETAIL, SCT_CALLBACK_4(Logic_Server_Request::process_20200013, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_BATTLE_OVER_TO_LOGIC, SCT_CALLBACK_4(Logic_Server_Request::process_20200200, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_BATTLE_END_TO_LOGIC, SCT_CALLBACK_4(Logic_Server_Request::process_20200202, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SCENE_PROP_CHANGE, SCT_CALLBACK_4(Logic_Server_Request::process_20200110, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_GET_DROPS_GOODS, SCT_CALLBACK_4(Logic_Server_Request::process_20300000, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_GATHER_GOODS, SCT_CALLBACK_4(Logic_Server_Request::process_20300010, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SCENE_COMPLETE, SCT_CALLBACK_4(Logic_Server_Request::process_20200080, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_TEAM_SCENE_COMPLETE, SCT_CALLBACK_4(Logic_Server_Request::process_20200081, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_HERO_DIE_TO_LOGIC, SCT_CALLBACK_4(Logic_Server_Request::process_20400004, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_HERO_INFO_TO_LOGIC, SCT_CALLBACK_4(Logic_Server_Request::process_20400005, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_HERO_OUT_FIGHT, SCT_CALLBACK_4(Logic_Server_Request::process_20400008, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_HERO_TMP_OUT_FIGHT, SCT_CALLBACK_4(Logic_Server_Request::process_20400009, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ADD_GOODS_TO_PACKAGE, SCT_CALLBACK_4(Logic_Server_Request::process_20300011, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SKILL_COST, SCT_CALLBACK_4(Logic_Server_Request::process_20100024, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ARENA_BATTLE_END, SCT_CALLBACK_4(Logic_Server_Request::process_20300031, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ARENA_FETCH_FIGHT_DATA, SCT_CALLBACK_4(Logic_Server_Request::process_20300033, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ARENA_MATCH_PLAYER, SCT_CALLBACK_4(Logic_Server_Request::process_20300034, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ARENA_FETCH_RANK_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20300035, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ARENA_SYNC_INNER_RANK, SCT_CALLBACK_4(Logic_Server_Request::process_20300036, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ARENA_PLAYER_DATA_COMPLETE, SCT_CALLBACK_4(Logic_Server_Request::process_20300037, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ARENA_KING_TIME_CHANGE, SCT_CALLBACK_4(Logic_Server_Request::process_20300039, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_GET_TEAM_SCORE, SCT_CALLBACK_4(Logic_Server_Request::process_20100310, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_TEAM_RANK, SCT_CALLBACK_4(Logic_Server_Request::process_20100312, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_SCENE_SYNC_TEAM_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20100304, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ADD_BLOOD_POOL, SCT_CALLBACK_4(Logic_Server_Request::process_20100305, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_TEAM_CUMULATE_AWARD, SCT_CALLBACK_4(Logic_Server_Request::process_20100307, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_PHY_POWER_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20100030, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_STATUS, SCT_CALLBACK_4(Logic_Server_Request::process_20100500, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_STATUS_HERO, SCT_CALLBACK_4(Logic_Server_Request::process_20100504, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_BATTLE_STATUS, SCT_CALLBACK_4(Logic_Server_Request::process_20100401, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_OPEN_ROLE_PANEL, SCT_CALLBACK_4(Logic_Server_Request::process_20410000, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SEND_BLESS_BUF, SCT_CALLBACK_4(Logic_Server_Request::process_20420000, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_WAR_SCENE_ENTER, SCT_CALLBACK_4(Logic_Server_Request::process_20300100, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_WAR_SCENE_SYNC_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20300101, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_SYNC_MOUNT_RIDE, SCT_CALLBACK_4(Logic_Server_Request::process_20100040, LOGIC_SERVER_REQUEST));


	register_server_process(INNER_SCENE_TO_LOGIC_ROB_MATCH, SCT_CALLBACK_4(Logic_Server_Request::process_20600001, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_REQ_DRAGON_VALE_ROB_FIGHT_LOAD, SCT_CALLBACK_4(Logic_Server_Request::process_20600003, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_NOTIFY_LOGIC_SAVE_ROB_RESULT, SCT_CALLBACK_4(Logic_Server_Request::process_20600005, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_NOTIFY_LOGIC_SAVE_TARGET_RESULT, SCT_CALLBACK_4(Logic_Server_Request::process_20600006, LOGIC_SERVER_REQUEST));
	register_server_process(INNNER_ROB_ENTER_DRAGON_VALE, SCT_CALLBACK_4(Logic_Server_Request::process_20600007, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_GAIN_ROB_PRODUCTION, SCT_CALLBACK_4(Logic_Server_Request::process_20600009, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_RES_GAIN_ROB_PRODUCTION, SCT_CALLBACK_4(Logic_Server_Request::process_20600010, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_NOTIFY_LOGIC_SAVE_TARGET_ROBBED_PRODUCTION, SCT_CALLBACK_4(Logic_Server_Request::process_20600011, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_PLAYER_EXIT_DRAGON_VALE, SCT_CALLBACK_4(Logic_Server_Request::process_20600013, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_CLEAR_DRAGON_VALE_MONSTER, SCT_CALLBACK_4(Logic_Server_Request::process_20600016, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SAVE_NOTICE_BOARD_DRIVE_ROBBER, SCT_CALLBACK_4(Logic_Server_Request::process_20600017, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SAVE_NOTICE_BOARD_ROB_PRODUCTION, SCT_CALLBACK_4(Logic_Server_Request::process_20600018, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ENTER_ROB_PROTECT_TIME, SCT_CALLBACK_4(Logic_Server_Request::process_20600019, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ROB_MATCH_FAIL_BACK_COIN, SCT_CALLBACK_4(Logic_Server_Request::process_20600020, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_CLEAR_DRAGON_VALE_ROB_PROTECT_STATUS, SCT_CALLBACK_4(Logic_Server_Request::process_20600021, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_CLEAR_ROB_PRODUCTION_PROCESS, SCT_CALLBACK_4(Logic_Server_Request::process_20600022, LOGIC_SERVER_REQUEST));
	register_server_process(INNNER_NOTIFY_TOD_ROB_PRODUCTION_FINISH, SCT_CALLBACK_4(Logic_Server_Request::process_20600023, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SET_FIRST_ENTER_AFTER_ROBBED, SCT_CALLBACK_4(Logic_Server_Request::process_20600024, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ROB_DRAGON_VALE_TASK_LISTEN, SCT_CALLBACK_4(Logic_Server_Request::process_20600025, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SUCCESS_PROTECT_DRAGON_VALE, SCT_CALLBACK_4(Logic_Server_Request::process_20600026, LOGIC_SERVER_REQUEST));
	register_server_process(INNNER_ROBBER_WIN_IN_ROB_FIGHT, SCT_CALLBACK_4(Logic_Server_Request::process_20600027, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ROBBER_GAIN_TOTAL_PRODUCTION, SCT_CALLBACK_4(Logic_Server_Request::process_20600028, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ROBBER_BEGIN_ATTCK_BATTLE, SCT_CALLBACK_4(Logic_Server_Request::process_20600029, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE, SCT_CALLBACK_4(Logic_Server_Request::process_20600031, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_NOTIFY_MASTER_OF_DRAGON_FIGHT_END, SCT_CALLBACK_4(Logic_Server_Request::process_20600032, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_DRAONG_VALE_NO_ENTER, SCT_CALLBACK_4(Logic_Server_Request::process_20600033, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_TARGET_IN_DEFENCE_DRAGON_VALE_FIGHT, SCT_CALLBACK_4(Logic_Server_Request::process_20600034, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_CHAT_LOGIC_WORLD_CHAT, SCT_CALLBACK_4(Logic_Server_Request::process_20200021, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_SYNC_GANG_WAR_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20170001, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_WAR_PLAYER_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20170002, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_WAR_RANK_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20170003, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_WAR_RESULT, SCT_CALLBACK_4(Logic_Server_Request::process_20170005, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_WAR_REWARD, SCT_CALLBACK_4(Logic_Server_Request::process_20170007, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_WAR_HELP, SCT_CALLBACK_4(Logic_Server_Request::process_20170008, LOGIC_SERVER_REQUEST));
//	register_server_process(INNER_SYNC_GANG_WAR_AWARD_SELF, SCT_CALLBACK_4(Logic_Server_Request::process_20170009, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_CAMPFIRE_SCENE_ENTER, SCT_CALLBACK_4(Logic_Server_Request::process_20300110, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_CAMPFIRE_SCENE_START, SCT_CALLBACK_4(Logic_Server_Request::process_20300111, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_CAMPFIRE_SCENE_SYNC_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20300112, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_LOGIC_ADD_ITEM_TO_PLAYER, SCT_CALLBACK_4(Logic_Server_Request::process_20300113, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_REQ_SYNC_CAMPFIRE_PLAYER_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20300115, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_RES_SYNC_CAMPFIRE_PLAYER_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20300116, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_REQ_MAIL_CAMPFIRE_AWARD, SCT_CALLBACK_4(Logic_Server_Request::process_20300117, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_ACT_SCENE_SYNC_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20300300, LOGIC_SERVER_REQUEST));
	//register_server_process(INNER_ACT_SCENE_PLAYER_ENTER, SCT_CALLBACK_4(Logic_Server_Request::process_20300301, LOGIC_SERVER_REQUEST));
	//register_server_process(INNER_ACT_SCENE_START, SCT_CALLBACK_4(Logic_Server_Request::process_20300302, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_ACT_SCENE_END, SCT_CALLBACK_4(Logic_Server_Request::process_20300303, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_SYNC_SCENE_OUTPOST, SCT_CALLBACK_4(Logic_Server_Request::process_20300400, LOGIC_SERVER_REQUEST));


	register_server_process(INNER_ACTIVITY_RESULT, SCT_CALLBACK_4(Logic_Server_Request::process_20200201, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_ANNOUNCE_SYSTEM, SCT_CALLBACK_4(Logic_Server_Request::process_20500000, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_KNIGHT_TRIALS_MATCH_PLAYER, SCT_CALLBACK_4(Logic_Server_Request::process_20610000, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_EXPEDITION_DATA_CHANNEL, SCT_CALLBACK_4(Logic_Server_Request::process_20610010, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_EXPEDITION_NOTE_LOGIC_PLAYER_INFO, SCT_CALLBACK_4(Logic_Server_Request::process_20610011, LOGIC_SERVER_REQUEST));
	register_server_process(INNER_EXPEDITION_PLAYER, SCT_CALLBACK_4(Logic_Server_Request::process_20610012, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_REQ_OPEN_PANEL, SCT_CALLBACK_4(Logic_Server_Request::process_20100200, LOGIC_SERVER_REQUEST));

	register_server_process(INNER_ANSWER_DATA_CHANNEL, SCT_CALLBACK_4(Logic_Server_Request::process_20610020, LOGIC_SERVER_REQUEST));

	/////////////////PHP内部消息//////////////////////
	register_server_process(FORBID_ACCOUNT_ROLE, SCT_CALLBACK_4(Logic_Server_Request::process_30100001, LOGIC_SERVER_REQUEST));
	register_server_process(FORBID_IP, SCT_CALLBACK_4(Logic_Server_Request::process_30100010, LOGIC_SERVER_REQUEST));
	register_server_process(MALL_GOODS_CHANGE, SCT_CALLBACK_4(Logic_Server_Request::process_30100002, LOGIC_SERVER_REQUEST));
	register_server_process(ONLINE_RECHARGE, SCT_CALLBACK_4(Logic_Server_Request::process_30100003, LOGIC_SERVER_REQUEST));
	register_server_process(PHP_INNER_ONLINE_RECHARGE, SCT_CALLBACK_4(Logic_Server_Request::process_30100009, LOGIC_SERVER_REQUEST));
	register_server_process(BACKSTAGE_SEND_MAIL, SCT_CALLBACK_4(Logic_Server_Request::process_30100004, LOGIC_SERVER_REQUEST));
	register_server_process(REQ_PHP_GET_ACCESS_COUNT, SCT_CALLBACK_4(Logic_Server_Request::process_30100006, LOGIC_SERVER_REQUEST));
	register_server_process(REQ_PHP_SEND_PROPOSAL_REPLY, SCT_CALLBACK_4(Logic_Server_Request::process_30100007, LOGIC_SERVER_REQUEST));
	register_server_process(PHP_SEND_GM_ANNOUNCE, SCT_CALLBACK_4(Logic_Server_Request::process_30100008, LOGIC_SERVER_REQUEST));
	register_server_process(REQ_PHP_ADD_ACTIVATION_KEY, SCT_CALLBACK_4(Logic_Server_Request::process_30100011, LOGIC_SERVER_REQUEST));
	register_server_process(REQ_PHP_FACEBOOK_SHARE_SUCCESS, SCT_CALLBACK_4(Logic_Server_Request::process_30100012, LOGIC_SERVER_REQUEST));
	register_server_process(GAG_ACCOUNT_ROLE, SCT_CALLBACK_4(Logic_Server_Request::process_30100005, LOGIC_SERVER_REQUEST));
	register_server_process(REQ_PHP_NOTICE_ACTIVE_OPEN, SCT_CALLBACK_4(Logic_Server_Request::process_30100013, LOGIC_SERVER_REQUEST));
	register_server_process(REQ_PHP_NOTICE_ACTIVE_CLOSE, SCT_CALLBACK_4(Logic_Server_Request::process_30100014, LOGIC_SERVER_REQUEST));
	register_server_process(PHP_SEND_ACTIVITY_EXCHANGE_MALL_GOODS_CHANGE, SCT_CALLBACK_4(Logic_Server_Request::process_30100015, LOGIC_SERVER_REQUEST));
	register_server_process(REQ_PHP_NOTICE_MOUNT_FUSE_TIME, SCT_CALLBACK_4(Logic_Server_Request::process_30100016, LOGIC_SERVER_REQUEST));



	return 0;
}

