/*
 * Register_Inner_Message.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: ti
 */

#include "Scene_Message_Unit.h"
#include "Scene_Server_Request.h"
#include "Msg_Inner_Enum.h"

using namespace Server_Enum;

int Scene_Message_Unit::register_inner_message(void) {

	register_server_process(LOGIC_SCENE_LOGIN_READY, SCT_CALLBACK_4(Scene_Server_Request::logic_login_ready, SCENE_SERVER_REQUEST));
	register_server_process(LOGIC_SCENE_LOGOUT_READY, SCT_CALLBACK_4(Scene_Server_Request::logic_logout_ready, SCENE_SERVER_REQUEST));
	register_server_process(SYNC_TASK_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20110001, SCENE_SERVER_REQUEST));
	register_server_process(INNER_TRAN_SCENE_SAME_PRO, SCT_CALLBACK_4(Scene_Server_Request::process_20200050, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ASK_SCENE_SIGN_OUT, SCT_CALLBACK_4(Scene_Server_Request::process_20200055, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ASK_SCENE_SIGN_IN, SCT_CALLBACK_4(Scene_Server_Request::process_20200056, SCENE_SERVER_REQUEST));
	register_server_process(INNER_CHANGE_FIGHT_PROP, SCT_CALLBACK_4(Scene_Server_Request::process_20200100, SCENE_SERVER_REQUEST));
	register_server_process(INNER_LEARN_SKILL_SUCCESS, SCT_CALLBACK_4(Scene_Server_Request::process_20100018, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ERASE_SKILL, SCT_CALLBACK_4(Scene_Server_Request::process_20100019, SCENE_SERVER_REQUEST));
	register_server_process(INNER_MOVE_SKILL, SCT_CALLBACK_4(Scene_Server_Request::process_20100020, SCENE_SERVER_REQUEST));
	register_server_process(INNER_OPEN_TALENT_PAGE, SCT_CALLBACK_4(Scene_Server_Request::process_20100022, SCENE_SERVER_REQUEST));
	register_server_process(INNER_USE_TALENT_PAGE, SCT_CALLBACK_4(Scene_Server_Request::process_20100023, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SKILL_COST_SUCCESS, SCT_CALLBACK_4(Scene_Server_Request::process_20100025, SCENE_SERVER_REQUEST));
	register_server_process(INNER_GET_DROPS_GOODS, SCT_CALLBACK_4(Scene_Server_Request::process_20300000, SCENE_SERVER_REQUEST));
	register_server_process(INNER_NPC_TRIGGER_SPOT_END, SCT_CALLBACK_4(Scene_Server_Request::process_20300020, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ARENA_BATTLE_START, SCT_CALLBACK_4(Scene_Server_Request::process_20300030, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ARENA_SYNC_RANK_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20300032, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ARENA_FETCH_FIGHT_DATA, SCT_CALLBACK_4(Scene_Server_Request::process_20300033, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ARENA_MATCH_PLAYER, SCT_CALLBACK_4(Scene_Server_Request::process_20300034, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ARENA_FETCH_RANK_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20300035, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ARENA_BATTLE_RESULT, SCT_CALLBACK_4(Scene_Server_Request::process_20300038, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ARENA_SEASON_FINISH, SCT_CALLBACK_4(Scene_Server_Request::process_20300040, SCENE_SERVER_REQUEST));
	register_server_process(SYSN_ENERGY_TO_LOGIC, SCT_CALLBACK_4(Scene_Server_Request::process_20100031, SCENE_SERVER_REQUEST));
	register_server_process(SYNS_LIFE_SKILL_TO_SCENE, SCT_CALLBACK_4(Scene_Server_Request::process_20100032, SCENE_SERVER_REQUEST));

	register_server_process(INNER_SYNC_HERO_BASE_DETAIL, SCT_CALLBACK_4(Scene_Server_Request::process_20400000, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_HERO_FIGHTER_DETAIL, SCT_CALLBACK_4(Scene_Server_Request::process_20400001, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_HERO_PROPERTY, SCT_CALLBACK_4(Scene_Server_Request::process_20400002, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_HERO_OUT, SCT_CALLBACK_4(Scene_Server_Request::process_20400003, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_HERO_FIGHT, SCT_CALLBACK_4(Scene_Server_Request::process_20400007, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SCENE_SYNC_TEAMER_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20100303, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SCENE_SYNC_TEAM_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20100304, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ADD_BLOOD_POOL, SCT_CALLBACK_4(Scene_Server_Request::process_20100305, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_TEAM_SPORTS, SCT_CALLBACK_4(Scene_Server_Request::process_20100306, SCENE_SERVER_REQUEST));
	register_server_process(INNER_FIGHTER_RESTORE_FULL_PROP, SCT_CALLBACK_4(Scene_Server_Request::process_20100400, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_STATUS, SCT_CALLBACK_4(Scene_Server_Request::process_20100500, SCENE_SERVER_REQUEST));
	register_server_process(INNER_STATUS_INSERT, SCT_CALLBACK_4(Scene_Server_Request::process_20100501, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_STATUS_DEL, SCT_CALLBACK_4(Scene_Server_Request::process_20100502, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_STATUS_HERO_INSERT, SCT_CALLBACK_4(Scene_Server_Request::process_20100503, SCENE_SERVER_REQUEST));
	register_server_process(INNER_GATHER_GOODS_RESULT, SCT_CALLBACK_4(Scene_Server_Request::process_20300012, SCENE_SERVER_REQUEST));
	register_server_process(INNER_BATTLE_STATUS, SCT_CALLBACK_4(Scene_Server_Request::process_20100401, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SEND_BLESS_BUF, SCT_CALLBACK_4(Scene_Server_Request::process_20420000, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20170000, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_WAR_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20170001, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_WAR_PLAYER_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20170002, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_ASK_GANG_WAR_RANK_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20170004, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_GANG_WAR_STATE, SCT_CALLBACK_4(Scene_Server_Request::process_20170006, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_TEAM_INTEGRATE, SCT_CALLBACK_4(Scene_Server_Request::process_20100309, SCENE_SERVER_REQUEST));
	register_server_process(INNER_GET_TEAM_SCORE, SCT_CALLBACK_4(Scene_Server_Request::process_20100310, SCENE_SERVER_REQUEST));


	register_server_process(INNER_SYNC_MOUNT_RIDE, SCT_CALLBACK_4(Scene_Server_Request::process_20100040, SCENE_SERVER_REQUEST));

	register_server_process(INNER_SYNC_PHY_POWER_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20100030, SCENE_SERVER_REQUEST));
	register_server_process(INNER_WAR_SCENE_ENTER, SCT_CALLBACK_4(Scene_Server_Request::process_20300100, SCENE_SERVER_REQUEST));
	register_server_process(INNER_WAR_SCENE_SYNC_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20300101, SCENE_SERVER_REQUEST));
	register_server_process(INNER_NOTIFY_SCENE_PASSIVE, SCT_CALLBACK_4(Scene_Server_Request::process_20100403, SCENE_SERVER_REQUEST));
	register_server_process(INNER_LOGIC_ADD_SKILL_POINT, SCT_CALLBACK_4(Scene_Server_Request::process_20100026, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_TITLE_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20100600, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ASK_SCENE_VIP_CHANGE, SCT_CALLBACK_4(Scene_Server_Request::process_20200020, SCENE_SERVER_REQUEST));

	register_server_process(INNER_LOGIC_TO_SCENE_ROB_MATCH, SCT_CALLBACK_4(Scene_Server_Request::process_20600000, SCENE_SERVER_REQUEST));
	register_server_process(INNER_RES_ROB_MATCH, SCT_CALLBACK_4(Scene_Server_Request::process_20600002, SCENE_SERVER_REQUEST));
	register_server_process(INNER_RES_DRAGON_VALE_ROB_FIGHT_LOAD, SCT_CALLBACK_4(Scene_Server_Request::process_20600004, SCENE_SERVER_REQUEST));
	register_server_process(INNNER_ROB_ENTER_DRAGON_VALE, SCT_CALLBACK_4(Scene_Server_Request::process_20600007, SCENE_SERVER_REQUEST));
	register_server_process(INNNER_ROB_ENTER_DRAGON_VALE_RES, SCT_CALLBACK_4(Scene_Server_Request::process_20600008, SCENE_SERVER_REQUEST));
	register_server_process(INNER_GAIN_ROB_PRODUCTION, SCT_CALLBACK_4(Scene_Server_Request::process_20600009, SCENE_SERVER_REQUEST));
	register_server_process(INNER_RES_GAIN_ROB_PRODUCTION, SCT_CALLBACK_4(Scene_Server_Request::process_20600010, SCENE_SERVER_REQUEST));
	register_server_process(INNER_BORADCAST_DRAGON_VALE, SCT_CALLBACK_4(Scene_Server_Request::process_20600012, SCENE_SERVER_REQUEST));
	register_server_process(INNER_DRAGON_VALE_ERROR_NOTICE, SCT_CALLBACK_4(Scene_Server_Request::process_20600014, SCENE_SERVER_REQUEST));
	register_server_process(INNER_DRAGON_VALE_MONSTER, SCT_CALLBACK_4(Scene_Server_Request::process_20600015, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ROB_MATCH_FAIL_BACK_COIN, SCT_CALLBACK_4(Scene_Server_Request::process_20600020, SCENE_SERVER_REQUEST));
	register_server_process(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE, SCT_CALLBACK_4(Scene_Server_Request::process_20600031, SCENE_SERVER_REQUEST));

	register_server_process(INNER_CAMPFIRE_SCENE_ENTER, SCT_CALLBACK_4(Scene_Server_Request::process_20300110, SCENE_SERVER_REQUEST));
	register_server_process(INNER_CAMPFIRE_SCENE_START, SCT_CALLBACK_4(Scene_Server_Request::process_20300111, SCENE_SERVER_REQUEST));
	register_server_process(INNER_CAMPFIRE_SCENE_SYNC_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20300112, SCENE_SERVER_REQUEST));
	register_server_process(INNER_RES_LOGIC_ADD_ITEM_TO_PLAYER, SCT_CALLBACK_4(Scene_Server_Request::process_20300114, SCENE_SERVER_REQUEST));
	register_server_process(INNER_RES_SYNC_CAMPFIRE_PLAYER_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20300116, SCENE_SERVER_REQUEST));

	register_server_process(INNER_ACT_SCENE_SYNC_INFO, SCT_CALLBACK_4(Scene_Server_Request::process_20300300, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ACT_SCENE_PLAYER_ENTER, SCT_CALLBACK_4(Scene_Server_Request::process_20300301, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ACT_SCENE_START, SCT_CALLBACK_4(Scene_Server_Request::process_20300302, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ACT_SCENE_END, SCT_CALLBACK_4(Scene_Server_Request::process_20300303, SCENE_SERVER_REQUEST));

	register_server_process(INNER_SYNC_SCENE_OUTPOST, SCT_CALLBACK_4(Scene_Server_Request::process_20300400, SCENE_SERVER_REQUEST));

	register_server_process(INNER_SYNC_SECRET_SHOP, SCT_CALLBACK_4(Scene_Server_Request::process_20300200, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ASK_SCENE_CHARM_CHANGE, SCT_CALLBACK_4(Scene_Server_Request::process_20200022, SCENE_SERVER_REQUEST));
	register_server_process(INNER_ASK_SCENE_HEAD_ID_CHANGE, SCT_CALLBACK_4(Scene_Server_Request::process_20200023, SCENE_SERVER_REQUEST));

	register_server_process(INNER_SYNC_PLAYER_NAME, SCT_CALLBACK_4(Scene_Server_Request::process_20100028, SCENE_SERVER_REQUEST));
	register_server_process(INNER_SYNC_PLAYER_GENDER, SCT_CALLBACK_4(Scene_Server_Request::process_20100029, SCENE_SERVER_REQUEST));

	register_server_process(INNER_STRING_INFO_CHANGE, SCT_CALLBACK_4(Scene_Server_Request::process_20100050, SCENE_SERVER_REQUEST));
	register_server_process(INNER_INT_INFO_CHANGE, SCT_CALLBACK_4(Scene_Server_Request::process_20100051, SCENE_SERVER_REQUEST));

	register_server_process(GATE_SCENE_LGOUT_IN_BATTLE, SCT_CALLBACK_4(Scene_Server_Request::process_20010061, SCENE_SERVER_REQUEST));

	register_server_process(INNER_SYNC_AVATAR, SCT_CALLBACK_4(Scene_Server_Request::process_20100060, SCENE_SERVER_REQUEST));

	register_server_process(INNER_KNIGHT_TRIALS_MATCH_PLAYER, SCT_CALLBACK_4(Scene_Server_Request::process_20610000, SCENE_SERVER_REQUEST));
	register_server_process(INNER_KNIGHT_TRIALS_ENTER_FIGHTER, SCT_CALLBACK_4(Scene_Server_Request::process_20610001, SCENE_SERVER_REQUEST));

	register_server_process(INNER_EXPEDITION_DATA_CHANNEL, SCT_CALLBACK_4(Scene_Server_Request::process_20610010, SCENE_SERVER_REQUEST));
	register_server_process(INNER_EXPEDITION_PLAYER, SCT_CALLBACK_4(Scene_Server_Request::process_20610012, SCENE_SERVER_REQUEST));

	register_server_process(INNER_ANSWER_DATA_CHANNEL, SCT_CALLBACK_4(Scene_Server_Request::process_20610020, SCENE_SERVER_REQUEST));

	register_server_process(INNER_REFRESH_ROLE_PANEL_PROP, SCT_CALLBACK_4(Scene_Server_Request::process_20410001, SCENE_SERVER_REQUEST));
	register_server_process(INNER_REQ_OPEN_PANEL, SCT_CALLBACK_4(Scene_Server_Request::process_20100200, SCENE_SERVER_REQUEST));


	register_server_process(INNER_SYNC_SCENE_PUBLIC, SCT_CALLBACK_4(Scene_Server_Request::process_20020001, SCENE_SERVER_REQUEST));

	///////////////////////////////////////////中央服务器///////////////////////////////////
	register_server_process(INNER_CHECK_INFO_TO_CENTER_SERVER, SCT_CALLBACK_4(Scene_Server_Request::process_20999999, SCENE_SERVER_REQUEST));
	return 0;
}
