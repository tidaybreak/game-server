/*
 * Register_Client_Message.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: ti
 */

#include "Scene_Message_Unit.h"
#include "Scene_Client_Request.h"
#include "Head_Enum.h"
#include "Msg_Chat_Enum.h"

int Scene_Message_Unit::register_client_message(void) {
	register_client_process(REQ_SCENE_TEST_COMMAND, SCT_CALLBACK_2(Scene_Client_Request::process_10299999, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_ROLE_INFO_SCENE, SCT_CALLBACK_2(Scene_Client_Request::process_10200006, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_CLIENT_MOVE, SCT_CALLBACK_2(Scene_Client_Request::process_10200102, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_CLIENT_HERO_MOVE, SCT_CALLBACK_2(Scene_Client_Request::process_10200103, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_ROLE_PANEL, SCT_CALLBACK_2(Scene_Client_Request::process_10200032, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_BATTLE_READY, SCT_CALLBACK_2(Scene_Client_Request::process_10200105, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_PLAYER_DUEL, SCT_CALLBACK_2(Scene_Client_Request::process_10200150, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_TO_FIGHT, SCT_CALLBACK_2(Scene_Client_Request::process_10200156, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_RES_PLAYER_DUEL, SCT_CALLBACK_2(Scene_Client_Request::process_10200155, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_USER_SKILL, SCT_CALLBACK_2(Scene_Client_Request::process_10200110, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_FIGHT_PAUSE, SCT_CALLBACK_2(Scene_Client_Request::process_10200111, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_FIGHT_GIVE_UP, SCT_CALLBACK_2(Scene_Client_Request::process_10200113, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_MULTI_BATTLE_GIVE_UP, SCT_CALLBACK_2(Scene_Client_Request::process_10200117, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_FETCH_DROPS_LIST, SCT_CALLBACK_2(Scene_Client_Request::process_10210000, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_GET_DROPS_GOODS, SCT_CALLBACK_2(Scene_Client_Request::process_10210001, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_START_GATHER, SCT_CALLBACK_2(Scene_Client_Request::process_10210100, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_STOP_GATHER, SCT_CALLBACK_2(Scene_Client_Request::process_10210101, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_CREATE_BATTLE_MONSTER, SCT_CALLBACK_2(Scene_Client_Request::process_10210200, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_START_FLY, SCT_CALLBACK_2(Scene_Client_Request::process_10200005, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_FLY, SCT_CALLBACK_2(Scene_Client_Request::process_10200007, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_PLAYER_FIGHT_MONSTER, SCT_CALLBACK_2(Scene_Client_Request::process_10200149, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_PLAYER_FIGHT, SCT_CALLBACK_2(Scene_Client_Request::process_10200151, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_MACHINE, SCT_CALLBACK_2(Scene_Client_Request::process_10210103, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_BEAT_BACK, SCT_CALLBACK_2(Scene_Client_Request::process_10200008, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_TRIGGER_MACHINE, SCT_CALLBACK_2(Scene_Client_Request::process_10210104, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_PICK_UP_GOODS, SCT_CALLBACK_2(Scene_Client_Request::process_10210002, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_REFRESH_SCENE_POINT, SCT_CALLBACK_2(Scene_Client_Request::process_10210010, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_FIGHT_SCENE_POINT, SCT_CALLBACK_2(Scene_Client_Request::process_10210011, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_TAKE_LIFT_FINISH, SCT_CALLBACK_2(Scene_Client_Request::process_10200009, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_SCENE_COMPLETE, SCT_CALLBACK_2(Scene_Client_Request::process_10200030, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_WAR_SCENE_FETCH_RANK, SCT_CALLBACK_2(Scene_Client_Request::process_10211001, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_WAR_SCENE_HEAL, SCT_CALLBACK_2(Scene_Client_Request::process_10211002, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_DUNGEON_PANEL_CLOSE, SCT_CALLBACK_2(Scene_Client_Request::process_10200200, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_SINGLE_DUNGEON_REVIVE, SCT_CALLBACK_2(Scene_Client_Request::process_10200215, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_ON_SCENE_CHAT, SCT_CALLBACK_2(Scene_Client_Request::process_10200011, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_ATTCK_ROB_MATCH_MEMBER, SCT_CALLBACK_2(Scene_Client_Request::process_10200152, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_ACCEPT_FIGHT_WITH_ROBBER, SCT_CALLBACK_2(Scene_Client_Request::process_10200153, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_DRIVE_DRAGON_VALE_THIEF_SCENE, SCT_CALLBACK_2(Scene_Client_Request::process_10200154, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_CAMPFIRE_ADD_FIREWOOD, SCT_CALLBACK_2(Scene_Client_Request::process_10200701, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_CAMPFIRE_FIRE, SCT_CALLBACK_2(Scene_Client_Request::process_10200702, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_CAMPFIRE_RANK, SCT_CALLBACK_2(Scene_Client_Request::process_10200703, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_CAMPFIRE_ADDWOOD_AWARD, SCT_CALLBACK_2(Scene_Client_Request::process_10200704, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_CAMPFIRE_SPOT_AWARD, SCT_CALLBACK_2(Scene_Client_Request::process_10200705, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_WORLD_BOSS_USE_ABILITY, SCT_CALLBACK_2(Scene_Client_Request::process_10211100, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_WORLD_BOSS_ACT_GUILD_BLESS, SCT_CALLBACK_2(Scene_Client_Request::process_10211101, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_HONOR_ARENA_MATCH, SCT_CALLBACK_2(Scene_Client_Request::process_10211201, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_HONOR_ARENA_RECEIVE, SCT_CALLBACK_2(Scene_Client_Request::process_10211202, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_HONOR_ARENA_RANK, SCT_CALLBACK_2(Scene_Client_Request::process_10211205, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_HONOR_ARENA_PANEL_INFO, SCT_CALLBACK_2(Scene_Client_Request::process_10211207, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_HONOR_ARENA_SET_AUTO_MATCH, SCT_CALLBACK_2(Scene_Client_Request::process_10211208, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_VAGRANT_ITEM_INFO, SCT_CALLBACK_2(Scene_Client_Request::process_10211300, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_VAGRANT_BUY_ITEM, SCT_CALLBACK_2(Scene_Client_Request::process_10211301, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_FETCH_TBAY_INFO, SCT_CALLBACK_2(Scene_Client_Request::process_10211310, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_ENTER_SCENE_OUTPOST, SCT_CALLBACK_2(Scene_Client_Request::process_10211311, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_TBAY_BLESS, SCT_CALLBACK_2(Scene_Client_Request::process_10211312, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_GANG_WAR_HEAL, SCT_CALLBACK_2(Scene_Client_Request::process_10270001, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_DUEL, SCT_CALLBACK_2(Scene_Client_Request::process_10270002, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_AGREE_DUEL_S, SCT_CALLBACK_2(Scene_Client_Request::process_10270004, SCENE_CLIENT_REQUEST));

	register_client_process(REQ_USE_ANSWER_FREE, SCT_CALLBACK_2(Scene_Client_Request::process_10209001, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_USE_ANSWER_DOUBLE, SCT_CALLBACK_2(Scene_Client_Request::process_10209002, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_GET_ANSWER_FREE_DOUBLE_NUM, SCT_CALLBACK_2(Scene_Client_Request::process_10209003, SCENE_CLIENT_REQUEST));

	//  expedition start -- //
	register_client_process(REQ_EXPEDITION_DAILY_AWARD_GET, SCT_CALLBACK_2(Scene_Client_Request::process_10201300, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_DAILY_AWARD_INFO, SCT_CALLBACK_2(Scene_Client_Request::process_10201301, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_OTHER_BUILD_INFO, SCT_CALLBACK_2(Scene_Client_Request::process_10201302, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_GANG_RANK, SCT_CALLBACK_2(Scene_Client_Request::process_10201303, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_BATTLE_INFO, SCT_CALLBACK_2(Scene_Client_Request::process_10201304, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_GANG_BASE_BUILD_INFO, SCT_CALLBACK_2(Scene_Client_Request::process_10201305, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_FIGHTING, SCT_CALLBACK_2(Scene_Client_Request::process_10201306, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_HELP_INFO, SCT_CALLBACK_2(Scene_Client_Request::process_10201307, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_CHANGE_LINE, SCT_CALLBACK_2(Scene_Client_Request::process_10201308, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_DEMAGE_RANK, SCT_CALLBACK_2(Scene_Client_Request::process_10201309, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_LOOKING, SCT_CALLBACK_2(Scene_Client_Request::process_10201310, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_OCCUPY_CASTLE, SCT_CALLBACK_2(Scene_Client_Request::process_10201311, SCENE_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_OCCUPY_CASTLE_AWARD_GET, SCT_CALLBACK_2(Scene_Client_Request::process_10201312, SCENE_CLIENT_REQUEST));
	// -- expedition end  //
	return 0;
}
