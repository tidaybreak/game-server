/*
 * Register_Client_Message.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: ti
 */

#include "Logic_Message_Unit.h"
#include "Logic_Client_Request.h"
#include "Head_Enum.h"

int Logic_Message_Unit::register_client_message(void) {
	register_client_process(REQ_LOGIC_TEST_COMMAND, SCT_CALLBACK_2(Logic_Client_Request::process_10199999, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACCESS_DATE_COUNT, SCT_CALLBACK_2(Logic_Client_Request::process_10188888, LOGIC_CLIENT_REQUEST));

	// 	public ==============
	register_client_process(REQ_SET_PLOT_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100010, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CLIENT_READY, SCT_CALLBACK_2(Logic_Client_Request::process_10100015, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ROLE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100006, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RENAME, SCT_CALLBACK_2(Logic_Client_Request::process_10100008, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_ROLE_PANEL_LOGIC, SCT_CALLBACK_2(Logic_Client_Request::process_10100032, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SET_FORENOTICE_PLAYED_EFFECT, SCT_CALLBACK_2(Logic_Client_Request::process_10110017, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SEND_CHALLENGE_TO_GANG_CHAT, SCT_CALLBACK_2(Logic_Client_Request::process_10120096, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_RANK_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10100332, LOGIC_CLIENT_REQUEST));


	// 	public ==============

	// 	package item ========
	register_client_process(REQ_FETCH_PACKAGE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100100, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ADD_CAPACITY, SCT_CALLBACK_2(Logic_Client_Request::process_10100101, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_DISCARD_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10100102, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOVE_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10100103, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SPLIT_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10100104, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SORT_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10100105, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_USE_SPECINDEX_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10100106, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_USE_SPECIAL_GIFT, SCT_CALLBACK_2(Logic_Client_Request::process_10100107, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MERGE_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10100108, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SELL_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10100109, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOVE_ITEM_FROM_TOWER, SCT_CALLBACK_2(Logic_Client_Request::process_10100110, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOVE_ITEM_FROM_PACKAGE, SCT_CALLBACK_2(Logic_Client_Request::process_10100111, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_PACKAGE_INFO_OTHER, SCT_CALLBACK_2(Logic_Client_Request::process_10100112, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_BUY_AND_USE, SCT_CALLBACK_2(Logic_Client_Request::process_10100113, LOGIC_CLIENT_REQUEST));
	// 	package item ========

	//	mail ========
	register_client_process(REQ_FETCH_MAIL_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10150000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MAIL_CHECK, SCT_CALLBACK_2(Logic_Client_Request::process_10150001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MAIL_SEND, SCT_CALLBACK_2(Logic_Client_Request::process_10150002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PICKUP_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10150003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MUTIMAIL_DELET, SCT_CALLBACK_2(Logic_Client_Request::process_10150005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PICKUP_MAIL_ALL_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10150004, LOGIC_CLIENT_REQUEST));
	//	mail ========

	// 	task ========
	register_client_process(REQ_FETCH_TASK_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10110000, LOGIC_CLIENT_REQUEST));
//	register_client_process(REQ_FETCH_NPC_TASK_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10110001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_ACCETP, SCT_CALLBACK_2(Logic_Client_Request::process_10110002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_ABANDON, SCT_CALLBACK_2(Logic_Client_Request::process_10110003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_COMPLETE, SCT_CALLBACK_2(Logic_Client_Request::process_10110004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_QUICK_COMPLETE, SCT_CALLBACK_2(Logic_Client_Request::process_10110007, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_PLOT, SCT_CALLBACK_2(Logic_Client_Request::process_10110008, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_NPC_TALK, SCT_CALLBACK_2(Logic_Client_Request::process_10110009, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_CLEAR_PLOTS, SCT_CALLBACK_2(Logic_Client_Request::process_10110010, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_ADD_NPC, SCT_CALLBACK_2(Logic_Client_Request::process_10110011, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_REWARD_TASK_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10110012, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_REWARD_TASK_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10110013, LOGIC_CLIENT_REQUEST));
	register_client_process(TASK_LISTEN_CHAT_WITH_PLAYER, SCT_CALLBACK_2(Logic_Client_Request::process_10110014, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_COMMIT_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10110015, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECOVER_TASK_CHAIN, SCT_CALLBACK_2(Logic_Client_Request::process_10110016, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SHOW_DAY_TASK_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10110018, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PLAYER_READED_TIMEOUT_TRY_MESSAGE, SCT_CALLBACK_2(Logic_Client_Request::process_10110019, LOGIC_CLIENT_REQUEST));


	// 	task ========

	//friends system
	register_client_process(REQ_FETCH_FRIEND_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10160000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_FRIEND_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10160001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_FRIEND_APPLY_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10160002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_DELET_FRIEND, SCT_CALLBACK_2(Logic_Client_Request::process_10160003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ADD_FRIEND, SCT_CALLBACK_2(Logic_Client_Request::process_10160004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MULTI_ADD_FRIEND, SCT_CALLBACK_2(Logic_Client_Request::process_10160013, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PUSH_BLACK, SCT_CALLBACK_2(Logic_Client_Request::process_10160005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HANDEL_FRIEND_APPLY, SCT_CALLBACK_2(Logic_Client_Request::process_10160009, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_ROLE_ONLINE, SCT_CALLBACK_2(Logic_Client_Request::process_10160010, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FRIEND_SET_PHOTO, SCT_CALLBACK_2(Logic_Client_Request::process_10160011, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SET_HEAD_PHOTO, SCT_CALLBACK_2(Logic_Client_Request::process_10160012, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FRIEND_BASE_INFO_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10160014, LOGIC_CLIENT_REQUEST));

	//friends system

	//	single dungeeon
	register_client_process(REQ_PASTED_SGL_DUNGEON, SCT_CALLBACK_2(Logic_Client_Request::process_10100200, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_CARD, SCT_CALLBACK_2(Logic_Client_Request::process_10100201, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_ELITE_DUNGEON_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100202, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RESET_ELITE_DUNGEON_TIMES, SCT_CALLBACK_2(Logic_Client_Request::process_10100204, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_SINGLE_DUNGEON_AWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10100205, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_BUY_ELITE_ADD_NUM, SCT_CALLBACK_2(Logic_Client_Request::process_10100206, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_ELITE_FIRST_PASS, SCT_CALLBACK_2(Logic_Client_Request::process_10100208, LOGIC_CLIENT_REQUEST));
	//	single dungeon

	//-- equip start
	register_client_process(REQ_EQUIP_ON_OFF, SCT_CALLBACK_2(Logic_Client_Request::process_10101000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_UNBAN, SCT_CALLBACK_2(Logic_Client_Request::process_10101001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_INCREASE, SCT_CALLBACK_2(Logic_Client_Request::process_10101002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_LEVEL_UP, SCT_CALLBACK_2(Logic_Client_Request::process_10101003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_MASTER_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10101004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_ENCHANT, SCT_CALLBACK_2(Logic_Client_Request::process_10101005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_ENCHANT_REPLACE_PROP_GROUP, SCT_CALLBACK_2(Logic_Client_Request::process_10101006, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_EMBED_GEMSTONE, SCT_CALLBACK_2(Logic_Client_Request::process_10101007, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_UMEMBED_GEMSTONE, SCT_CALLBACK_2(Logic_Client_Request::process_10101008, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_GEMSTONE_HOLE, SCT_CALLBACK_2(Logic_Client_Request::process_10101009, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GEMSTONE_CONVERT, SCT_CALLBACK_2(Logic_Client_Request::process_10101010, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GEM_ENABLE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10101011, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_BREAKDOWN, SCT_CALLBACK_2(Logic_Client_Request::process_10101012, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_BREAKDOWN_GET_PROPS, SCT_CALLBACK_2(Logic_Client_Request::process_10101013, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GEM_CARVE, SCT_CALLBACK_2(Logic_Client_Request::process_10101014, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GEM_CARVE_LVL, SCT_CALLBACK_2(Logic_Client_Request::process_10101015, LOGIC_CLIENT_REQUEST));
	//-- equip end

	//-- rune start
	register_client_process(REQ_FETCH_RUNE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10103000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_ACTIVATE, SCT_CALLBACK_2(Logic_Client_Request::process_10103001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_FILL_ENERGY, SCT_CALLBACK_2(Logic_Client_Request::process_10103002, LOGIC_CLIENT_REQUEST));
	//-- rune end

	//-- rune_stone start
	register_client_process(REQ_RUNE_STONE_SYST_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10103200, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_SMELT, SCT_CALLBACK_2(Logic_Client_Request::process_10103201, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_SELL, SCT_CALLBACK_2(Logic_Client_Request::process_10103202, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_PICK, SCT_CALLBACK_2(Logic_Client_Request::process_10103203, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_LOCK, SCT_CALLBACK_2(Logic_Client_Request::process_10103204, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_MERGE, SCT_CALLBACK_2(Logic_Client_Request::process_10103205, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_MOVE, SCT_CALLBACK_2(Logic_Client_Request::process_10103206, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_EXCHANGE, SCT_CALLBACK_2(Logic_Client_Request::process_10103207, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_POINTS, SCT_CALLBACK_2(Logic_Client_Request::process_10103208, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_ON_OFF, SCT_CALLBACK_2(Logic_Client_Request::process_10103209, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ESSENCE, SCT_CALLBACK_2(Logic_Client_Request::process_10103210, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RUNE_STONE_ESSENCE_EXCHANGE, SCT_CALLBACK_2(Logic_Client_Request::process_10103211, LOGIC_CLIENT_REQUEST));
	//-- rune_stone end

	//learn skill
	register_client_process(REQ_SKILLS_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10100140, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_LEARN_SKILL, SCT_CALLBACK_2(Logic_Client_Request::process_10100141, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CLEAR_SKILL, SCT_CALLBACK_2(Logic_Client_Request::process_10100165, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOVE_SKILL, SCT_CALLBACK_2(Logic_Client_Request::process_10100166, LOGIC_CLIENT_REQUEST));
//	register_client_process(REQ_OPEN_TALENT, SCT_CALLBACK_2(Logic_Client_Request::process_10100142, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_TALENT_PAGE, SCT_CALLBACK_2(Logic_Client_Request::process_10100143, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_USE_TALENT_PAGE, SCT_CALLBACK_2(Logic_Client_Request::process_10100144, LOGIC_CLIENT_REQUEST));
	//learn_skill

	//-- hero start
	register_client_process(REQ_FETCH_HEROER_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10102000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HERO_LEVEL_UP, SCT_CALLBACK_2(Logic_Client_Request::process_10102001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HERO_EQUIP_ON_OFF, SCT_CALLBACK_2(Logic_Client_Request::process_10102002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HERO_OUT, SCT_CALLBACK_2(Logic_Client_Request::process_10102003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HERO_RECRUIT, SCT_CALLBACK_2(Logic_Client_Request::process_10102004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HERO_SET_FORMATION, SCT_CALLBACK_2(Logic_Client_Request::process_10102005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HERO_SET_FIGHT, SCT_CALLBACK_2(Logic_Client_Request::process_10102006, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HERO_AWAKE, SCT_CALLBACK_2(Logic_Client_Request::process_10102007, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ARTIFACT_UPGRADE, SCT_CALLBACK_2(Logic_Client_Request::process_10102020, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_NEW_HERO_CLICK, SCT_CALLBACK_2(Logic_Client_Request::process_10102021, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_CLICKED_NEW_HEROS, SCT_CALLBACK_2(Logic_Client_Request::process_10102022, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_HERO_ASSIST, SCT_CALLBACK_2(Logic_Client_Request::process_10102032, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HERO_INTERACTIVE, SCT_CALLBACK_2(Logic_Client_Request::process_10102033, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GIFT_FOR_ASSIST_HERO, SCT_CALLBACK_2(Logic_Client_Request::process_10102034, LOGIC_CLIENT_REQUEST));
	//-- hero end

	//-- mall start 商城
	register_client_process(REQ_FETCH_MALL_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10151000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_BUY_GOODS_IN_MALL, SCT_CALLBACK_2(Logic_Client_Request::process_10151001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ONE_KEY_BUY_GOODS_IN_MALL, SCT_CALLBACK_2(Logic_Client_Request::process_10151002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FAST_BUY_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10151003, LOGIC_CLIENT_REQUEST));
	//-- mall end 商城

	//-- team start
	register_client_process(REQ_TEAM_PLATFORM, SCT_CALLBACK_2(Logic_Client_Request::process_10100300, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CREATE_TEAM, SCT_CALLBACK_2(Logic_Client_Request::process_10100301, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_INVITE_ROLE, SCT_CALLBACK_2(Logic_Client_Request::process_10100302, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_APPLY_TEAM, SCT_CALLBACK_2(Logic_Client_Request::process_10100303, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MAKE_LEADER, SCT_CALLBACK_2(Logic_Client_Request::process_10100304, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_KICK_ROLE, SCT_CALLBACK_2(Logic_Client_Request::process_10100305, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_LEAVE_TEAM, SCT_CALLBACK_2(Logic_Client_Request::process_10100306, LOGIC_CLIENT_REQUEST));
//	register_client_process(REQ_DISMISS_TEAM, SCT_CALLBACK_2(Logic_Client_Request::process_10100307, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_SETTING, SCT_CALLBACK_2(Logic_Client_Request::process_10100308, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_AGREE_TEAM_INVITE, SCT_CALLBACK_2(Logic_Client_Request::process_10100309, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CHANGE_TEAM_SETTING, SCT_CALLBACK_2(Logic_Client_Request::process_10100310, LOGIC_CLIENT_REQUEST));
//	register_client_process(REQ_TEAM_HIRING, SCT_CALLBACK_2(Logic_Client_Request::process_10100311, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_BE_WAIT, SCT_CALLBACK_2(Logic_Client_Request::process_10100312, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_CANCEL_WAIT, SCT_CALLBACK_2(Logic_Client_Request::process_10100313, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_LISTEN_TEAM_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100315, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_REJECT_TEAM_INVITE, SCT_CALLBACK_2(Logic_Client_Request::process_10100316, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_APPLY_TEAM_QUICK, SCT_CALLBACK_2(Logic_Client_Request::process_10100317, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_SET_PASSWORD, SCT_CALLBACK_2(Logic_Client_Request::process_10100318, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_CANCEL_PASSWORD, SCT_CALLBACK_2(Logic_Client_Request::process_10100319, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_READY, SCT_CALLBACK_2(Logic_Client_Request::process_10100320, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_WAIT_AREA, SCT_CALLBACK_2(Logic_Client_Request::process_10100321, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_SET_POSITION_OPEN_OR_CLOSE, SCT_CALLBACK_2(Logic_Client_Request::process_10100322, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_SET_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100323, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_LINEUP_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100324, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_LINEUP_SET, SCT_CALLBACK_2(Logic_Client_Request::process_10100325, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_BACK_ROOM, SCT_CALLBACK_2(Logic_Client_Request::process_10100326, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_HERO_DREAM, SCT_CALLBACK_2(Logic_Client_Request::process_10100327, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_FOLLOW, SCT_CALLBACK_2(Logic_Client_Request::process_10100328, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100330, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_QUICK_INVITE, SCT_CALLBACK_2(Logic_Client_Request::process_10100331, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TEAM_SCORE_AWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10100333, LOGIC_CLIENT_REQUEST));

	//-- team end
	//-- gang start
	register_client_process(REQ_CREATE_GANG, SCT_CALLBACK_2(Logic_Client_Request::process_10170000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_JOIN_GANG, SCT_CALLBACK_2(Logic_Client_Request::process_10170001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10170002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_SELF_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10170003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10170004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANGER_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10170005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_QUIT, SCT_CALLBACK_2(Logic_Client_Request::process_10170006, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_RESIGN, SCT_CALLBACK_2(Logic_Client_Request::process_10170007, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_INFO_UPDATE, SCT_CALLBACK_2(Logic_Client_Request::process_10170008, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_KICK_OUT, SCT_CALLBACK_2(Logic_Client_Request::process_10170009, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_SET_ANNOUNCE, SCT_CALLBACK_2(Logic_Client_Request::process_10170010, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_INVITE_TO_JOIN, SCT_CALLBACK_2(Logic_Client_Request::process_10170011, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_PASSED_EVENTS, SCT_CALLBACK_2(Logic_Client_Request::process_10170012, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_SIMPLE_PASSED_EVENT, SCT_CALLBACK_2(Logic_Client_Request::process_10170013, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_GROUP_MAIL, SCT_CALLBACK_2(Logic_Client_Request::process_10170014, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_DONATE, SCT_CALLBACK_2(Logic_Client_Request::process_10170015, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_SET_JOIN_BLOCK, SCT_CALLBACK_2(Logic_Client_Request::process_10170016, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_INVITE_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10170017, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_APPLY_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10170018, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_LIST_ACTION, SCT_CALLBACK_2(Logic_Client_Request::process_10170019, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_INVITE_ACTION, SCT_CALLBACK_2(Logic_Client_Request::process_10170020, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_APPLY_ACTION, SCT_CALLBACK_2(Logic_Client_Request::process_10170021, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANGER_CHANGE_HEADSHIP_AUTH, SCT_CALLBACK_2(Logic_Client_Request::process_10170022, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_UPGRADE, SCT_CALLBACK_2(Logic_Client_Request::process_10170023, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_UPGRADE_SEND_GIFT, SCT_CALLBACK_2(Logic_Client_Request::process_10170024, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_UPGRADE_CD_CLEAR, SCT_CALLBACK_2(Logic_Client_Request::process_10170025, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WORLD_NOTICE, SCT_CALLBACK_2(Logic_Client_Request::process_10170026, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_LOGO_MODIFY, SCT_CALLBACK_2(Logic_Client_Request::process_10170027, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_SEIZE_POWER, SCT_CALLBACK_2(Logic_Client_Request::process_10170028, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SEND_BLESS_BUF, SCT_CALLBACK_2(Logic_Client_Request::process_10170029, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RESEARCH_GANG_SKILL, SCT_CALLBACK_2(Logic_Client_Request::process_10170030, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_LEARN_GANGER_SKILL, SCT_CALLBACK_2(Logic_Client_Request::process_10170031, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_DRAW, SCT_CALLBACK_2(Logic_Client_Request::process_10170032, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_TAKE_DRAW_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10170033, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_BUILDING_UPGRADE, SCT_CALLBACK_2(Logic_Client_Request::process_10170034, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_ENTER, SCT_CALLBACK_2(Logic_Client_Request::process_10170035, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_BOARD, SCT_CALLBACK_2(Logic_Client_Request::process_10170036, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_SPOT, SCT_CALLBACK_2(Logic_Client_Request::process_10170037, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_AWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10170038, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_AWARD_ASSIGN, SCT_CALLBACK_2(Logic_Client_Request::process_10170039, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_SHOP_BUY_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10170041, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_DRAW, SCT_CALLBACK_2(Logic_Client_Request::process_10170042, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_GET_RANK_AWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10170043, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_RANK_AWARD_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10170044, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GANG_WAR_GANG_RANK, SCT_CALLBACK_2(Logic_Client_Request::process_10170045, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PHY_POWER_PRACTICE_START, SCT_CALLBACK_2(Logic_Client_Request::process_10170047, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PHY_POWER_PRACTICE_STOP, SCT_CALLBACK_2(Logic_Client_Request::process_10170048, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SPEED_UP_PRACTICE, SCT_CALLBACK_2(Logic_Client_Request::process_10170049, LOGIC_CLIENT_REQUEST));
	//-- gang end
	//fcm start
	register_client_process(REQ_FCM_PASS, SCT_CALLBACK_2(Logic_Client_Request::process_10100090, LOGIC_CLIENT_REQUEST));
	//fcm end
	//Alchemy(炼金系统) start
	register_client_process(REQ_ALCHEMY_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10152000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ALCHEMY, SCT_CALLBACK_2(Logic_Client_Request::process_10152001, LOGIC_CLIENT_REQUEST));
	//alchemy end

	//arena start
	register_client_process(REQ_FETCH_LOCAL_ARENA_RANK_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100400, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_ARENA_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100405, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CLOSE_ARENA_PANEL, SCT_CALLBACK_2(Logic_Client_Request::process_10100406, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RESET_ARENA_CD, SCT_CALLBACK_2(Logic_Client_Request::process_10100407, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ARENA_PANEL_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100408, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MATCH_ARENA_BATTLE, SCT_CALLBACK_2(Logic_Client_Request::process_10100410, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ARENA_BUY_PAY_TIMES, SCT_CALLBACK_2(Logic_Client_Request::process_10100420, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ARENA_RECEIVE_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10100401, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_ARENA_AWARD_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100402, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_ARENA_MATCH, SCT_CALLBACK_2(Logic_Client_Request::process_10100412, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ARENA_FIGHT, SCT_CALLBACK_2(Logic_Client_Request::process_10100413, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ARENA_MATCH_REFRESH, SCT_CALLBACK_2(Logic_Client_Request::process_10100414, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ARENA_WORSHIP, SCT_CALLBACK_2(Logic_Client_Request::process_10100416, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_REFRESH_ARENA_PANEL, SCT_CALLBACK_2(Logic_Client_Request::process_10100417, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_OPEN_ARENA_ROLE_PANEL, SCT_CALLBACK_2(Logic_Client_Request::process_10100415, LOGIC_CLIENT_REQUEST));
	//arena end

	//guide start
	register_client_process(REQ_NEW_HAND_GUIDE, SCT_CALLBACK_2(Logic_Client_Request::process_10104000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_NEW_HAND_GUIDE_SET, SCT_CALLBACK_2(Logic_Client_Request::process_10104001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SAVE_PLAYED_PLOT, SCT_CALLBACK_2(Logic_Client_Request::process_10104002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GUIDE_ADD_ITEM_IN_PLOT, SCT_CALLBACK_2(Logic_Client_Request::process_10120095, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TASK_GUIDE, SCT_CALLBACK_2(Logic_Client_Request::process_10104003, LOGIC_CLIENT_REQUEST));


	//guide end

	//ranking start
	register_client_process(REQ_RANKING_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10171000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_RANK_ROLE_PANEL, SCT_CALLBACK_2(Logic_Client_Request::process_10171001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_RANK_ITEM_TIP, SCT_CALLBACK_2(Logic_Client_Request::process_10171002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_RANK_HEROER_PANEL, SCT_CALLBACK_2(Logic_Client_Request::process_10171003, LOGIC_CLIENT_REQUEST));
	//ranking end

	//phy power
	register_client_process(REQ_BUY_PHY_POWER, SCT_CALLBACK_2(Logic_Client_Request::process_10120000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_PHY_POWER_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10120001, LOGIC_CLIENT_REQUEST));
	//phy power

	//chat start
	register_client_process(REQ_CHAT_BUGLE, SCT_CALLBACK_2(Logic_Client_Request::process_10120002, LOGIC_CLIENT_REQUEST));
	//chat end

	//exchange start
	register_client_process(REQ_ITEM_EXCHANGE, SCT_CALLBACK_2(Logic_Client_Request::process_10100500, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_EXCHANGE_ITEM_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100501, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EXCHANGE_SKILL_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10100502, LOGIC_CLIENT_REQUEST));
	//exchange end

	//daily helper start
	register_client_process(REQ_DAILY_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVITY_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_ACTIVITY_AWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TODAY_OPEN_ACTIVE, SCT_CALLBACK_2(Logic_Client_Request::process_10106003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_UPGRADE_SCORE, SCT_CALLBACK_2(Logic_Client_Request::process_10106004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_UPGRADE_GET_STATUS, SCT_CALLBACK_2(Logic_Client_Request::process_10106005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HELPER_CHECK_IN, SCT_CALLBACK_2(Logic_Client_Request::process_10106006, LOGIC_CLIENT_REQUEST));
	//daily helper end

	// world boss start
	register_client_process(REQ_WORLD_BOSS_KILL_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10111102, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_WORLD_BOSS_SET_TIME, SCT_CALLBACK_2(Logic_Client_Request::process_10111103, LOGIC_CLIENT_REQUEST));
	// world boss end

	// war scene start
	register_client_process(REQ_WAR_SCENE_ENTER, SCT_CALLBACK_2(Logic_Client_Request::process_10111000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_WAR_SCENE_ENTER_NUM, SCT_CALLBACK_2(Logic_Client_Request::process_10111001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_WAR_SCENE_RECEIVE, SCT_CALLBACK_2(Logic_Client_Request::process_10111003, LOGIC_CLIENT_REQUEST));
	// war scene end

	//sys cfg start
	register_client_process(REQ_GET_SYS_CFG, SCT_CALLBACK_2(Logic_Client_Request::process_10120003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SET_SYS_CFG, SCT_CALLBACK_2(Logic_Client_Request::process_10120004, LOGIC_CLIENT_REQUEST));
	//sys cfg end

	// player awake begin
	register_client_process(REQ_FETCH_AWAKE_LVL, SCT_CALLBACK_2(Logic_Client_Request::process_10120020, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_AWAKE_LVL_UPGRADE, SCT_CALLBACK_2(Logic_Client_Request::process_10120021, LOGIC_CLIENT_REQUEST));
	// player awake end

	// player artifact begin
	register_client_process(REQ_FETCH_PLAYER_ARTIFACT_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10120030, LOGIC_CLIENT_REQUEST));
	// player artifact end

	// forenotice begin
	register_client_process(REQ_FORENOTICE_RECEIVE_GIFT, SCT_CALLBACK_2(Logic_Client_Request::process_10120040, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FORENOTICE_RECEIVED_GIFTS, SCT_CALLBACK_2(Logic_Client_Request::process_10120041, LOGIC_CLIENT_REQUEST));
	// forenotice end

	// exploit begin
	register_client_process(REQ_EXPLOIT_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10120080, LOGIC_CLIENT_REQUEST));
	// exploit_end

	//COVENANT begin
	register_client_process(REQ_BUY_COVENANT, SCT_CALLBACK_2(Logic_Client_Request::process_10106100, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_HANDSEL_COVENANT, SCT_CALLBACK_2(Logic_Client_Request::process_10106101, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_START_COVENANT_BECOME_TIME, SCT_CALLBACK_2(Logic_Client_Request::process_10106102, LOGIC_CLIENT_REQUEST));
	//covenant end

	//dragon vale begin
	register_client_process(REQ_CREATE_NEW_BUILDING, SCT_CALLBACK_2(Logic_Client_Request::process_10180000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_UPGRADE_BUILDING, SCT_CALLBACK_2(Logic_Client_Request::process_10180001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_QUICK_FINISH_BUILDING, SCT_CALLBACK_2(Logic_Client_Request::process_10180002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CANCEL_FINISH_BUILDING, SCT_CALLBACK_2(Logic_Client_Request::process_10180003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GAIN_PRODUCTION, SCT_CALLBACK_2(Logic_Client_Request::process_10180004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_STEP_FOOT, SCT_CALLBACK_2(Logic_Client_Request::process_10180005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ADD_BUILDING_QUEUE, SCT_CALLBACK_2(Logic_Client_Request::process_10180006, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_DRAGON_VALE_FIENDLIST, SCT_CALLBACK_2(Logic_Client_Request::process_10180007, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_DRANON_VALE_GANG_MEMBER_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10180008, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_DRAGON_BORN, SCT_CALLBACK_2(Logic_Client_Request::process_10180009, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PLAY_DRAGON, SCT_CALLBACK_2(Logic_Client_Request::process_10180010, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_DRAGON_TOTAL_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10180011, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_KEEP_DRAGON, SCT_CALLBACK_2(Logic_Client_Request::process_10180012, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ADD_DRAGON_FEELING, SCT_CALLBACK_2(Logic_Client_Request::process_10180013, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EVOLUTE_DRAGON, SCT_CALLBACK_2(Logic_Client_Request::process_10180014, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ROB_OTHER_DRAGON_VALE, SCT_CALLBACK_2(Logic_Client_Request::process_10180015, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ROB_OTHER_DRAGON_VALE_MATCH, SCT_CALLBACK_2(Logic_Client_Request::process_10180016, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_NOTICE, SCT_CALLBACK_2(Logic_Client_Request::process_10180017, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GAIN_ROB_PRODUCTION_TO_LOGIC, SCT_CALLBACK_2(Logic_Client_Request::process_10180018, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ROB_OTHER_DRAGON_VALE_TASK_LISTEN, SCT_CALLBACK_2(Logic_Client_Request::process_10180019, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_BATCH_EVOLUTE_DRAGON, SCT_CALLBACK_2(Logic_Client_Request::process_10180020, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_DRAGON_BUILDING_GAIN_PRODUCTION_LEFT_TIME, SCT_CALLBACK_2(Logic_Client_Request::process_10180021, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_INVITE_FRIEND_STEP_FOOT_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10180022, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_INVITE_FRIEND_STEP_FOOT, SCT_CALLBACK_2(Logic_Client_Request::process_10180023, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_INVITE_ALL_FRIEND_STEP_FOOT, SCT_CALLBACK_2(Logic_Client_Request::process_10180024, LOGIC_CLIENT_REQUEST));
	//dragon vale end

	//achievement start
	register_client_process(REQ_ACHIEVEMENT_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10100800, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACHIEVEMENT_GET, SCT_CALLBACK_2(Logic_Client_Request::process_10100801, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACHIEVEMENT_MEDAL_ONOFF, SCT_CALLBACK_2(Logic_Client_Request::process_10100802, LOGIC_CLIENT_REQUEST));
	//achievement end

	//title start
	register_client_process(REQ_FETCH_TITLE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100900, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_TITLE_ON_OFF, SCT_CALLBACK_2(Logic_Client_Request::process_10100901, LOGIC_CLIENT_REQUEST));
	//title end

	//vip start
	register_client_process(REQ_OPEN_VIP, SCT_CALLBACK_2(Logic_Client_Request::process_10106200, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_VIP_WORLD_TREE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106201, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_VIP_RECEIVE_WORLD_TREE, SCT_CALLBACK_2(Logic_Client_Request::process_10106202, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_VIP_TURNTABLE_OPEN, SCT_CALLBACK_2(Logic_Client_Request::process_10106203, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_VIP_TURNTABLE_LOTTERY, SCT_CALLBACK_2(Logic_Client_Request::process_10106204, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_VIP_TURNTABLE_RECEIVE, SCT_CALLBACK_2(Logic_Client_Request::process_10106205, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_VIP_DAILY_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106206, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_VIP_DAILY_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106207, LOGIC_CLIENT_REQUEST));
	//vip end

	// mount start
	register_client_process(REQ_FETCH_MOUNT_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100600, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FETCH_MOUNT_STABLE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10100601, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOUNT_RIDE, SCT_CALLBACK_2(Logic_Client_Request::process_10100602, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOUNT_ACTIVATE, SCT_CALLBACK_2(Logic_Client_Request::process_10100603, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOUNT_CLICK_NEW_GROUP, SCT_CALLBACK_2(Logic_Client_Request::process_10100604, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_MOUNT_TRAIN, SCT_CALLBACK_2(Logic_Client_Request::process_10100605, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOUNT_DEPLOY, SCT_CALLBACK_2(Logic_Client_Request::process_10100606, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOUNT_LEARN_SKILL, SCT_CALLBACK_2(Logic_Client_Request::process_10100608, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MOUNT_FUSE_TIME, SCT_CALLBACK_2(Logic_Client_Request::process_10100609, LOGIC_CLIENT_REQUEST));
	// mount end

	//flowers start
	register_client_process(REQ_HANDSEL_FLOWERS, SCT_CALLBACK_2(Logic_Client_Request::process_10120050, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_LOOK_FLOWERS, SCT_CALLBACK_2(Logic_Client_Request::process_10120051, LOGIC_CLIENT_REQUEST));
	//flowers end

	// secret_shop start
	register_client_process(REQ_FETCH_SECRET_SHOP_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10120010, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SECRET_SHOP_BUY_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10120011, LOGIC_CLIENT_REQUEST));
	// secret_shop end

	//reward start
	register_client_process(REQ_REWARD_TIP_NUM, SCT_CALLBACK_2(Logic_Client_Request::process_10106309, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_RECEIVE_ONLINE_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106300, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_REWARD_LOGIN_SEVEN_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106301, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_LOGIN_SEVEN_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106302, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_ACTIVATION_CODE_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106303, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_REWARD_BACK_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106304, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_REWARD_BACK, SCT_CALLBACK_2(Logic_Client_Request::process_10106305, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_HAVE_RECEIVE_KEEP_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106306, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_KEEP_REWARD_SUCCESS, SCT_CALLBACK_2(Logic_Client_Request::process_10106307, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_KEEP_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106308, LOGIC_CLIENT_REQUEST));
	//reward end

	// wipe out single dungeon start
	register_client_process(REQ_WIPE_OUT_SINGLE_DUNGEON, SCT_CALLBACK_2(Logic_Client_Request::process_10100210, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_INTERRUP_WIPE_OUT_SINGLE_DUNGEON, SCT_CALLBACK_2(Logic_Client_Request::process_10100211, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_COMPLETE_WIPE_OUT_SINGLE_DUNGEON, SCT_CALLBACK_2(Logic_Client_Request::process_10100212, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OBTAIN_WIPE_DOUBLE_INCOME, SCT_CALLBACK_2(Logic_Client_Request::process_10100213, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SELECT_WIPE_DOUBLE, SCT_CALLBACK_2(Logic_Client_Request::process_10100214, LOGIC_CLIENT_REQUEST));
	// wipe out single dungeon end

	// campfire start
	register_client_process(REQ_CAMPFIRE_ENTER, SCT_CALLBACK_2(Logic_Client_Request::process_10100700, LOGIC_CLIENT_REQUEST));
	// campfire end

	//
	register_client_process(REQ_FETCH_MONTH_HERO_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10120070, LOGIC_CLIENT_REQUEST));
	//

	register_client_process(REQ_ACT_ENTER_SCENE, SCT_CALLBACK_2(Logic_Client_Request::process_10100040, LOGIC_CLIENT_REQUEST));

	//active content start
	register_client_process(REQ_ACTIVE_CONTENT_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106400, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_VIP_CLUB_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106401, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_LEVEL_ACE_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106402, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_SUIT_COLLECT_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106403, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_GUILD_LEVEL_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106404, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_TIP_NUM, SCT_CALLBACK_2(Logic_Client_Request::process_10106405, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_GEM_LEVELUP_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106406, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_WING_LEVELUP_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106407, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_MOUNT_LEVELUP_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106408, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_SG_PASS_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106409, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_ARENA_DAN_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106410, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_ACTIVE_CONTENT_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106411, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_MOUNT_PASS_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106412, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_WING_PASS_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106413, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_ACTIVE_CONTENT_LIMIT_BUY_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106420, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_VIP_CLUB_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106421, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_LEVEL_ACE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106422, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_SUIT_COLLECT_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106423, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_GUILD_LEVEL_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106424, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_LEVEL_RANK_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106425, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_ARENA_RANK_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106426, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_FORCE_RANK_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106427, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_SG_RANK_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106431, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_MOUNT_RANK_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106448, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_WING_RANK_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106449, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_GEM_LEVELUP_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106428, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_WING_LEVELUP_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106429, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_MOUNT_LEVELUP_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106430, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_ARENA_DAN_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106432, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_WORLD_BOSS_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106433, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_SUIT_COLLECT_TWO_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106434, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_HEROES_DOOR_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106435, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_TEAM_ARENA_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106436, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_WARER_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106437, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_REWARD_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106438, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_SGFB_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106439, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_FM_X_CI, SCT_CALLBACK_2(Logic_Client_Request::process_10106440, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_X_X_QUALITY_HEROER_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10106441, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_MOUNT_LVUP_X, SCT_CALLBACK_2(Logic_Client_Request::process_10106442, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_FUSION_X_ELITE_CARD, SCT_CALLBACK_2(Logic_Client_Request::process_10106443, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_ALCHEMY_GIFT, SCT_CALLBACK_2(Logic_Client_Request::process_10106444, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_ACTIVITY_GIFT, SCT_CALLBACK_2(Logic_Client_Request::process_10106445, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_RING_TASK, SCT_CALLBACK_2(Logic_Client_Request::process_10106446, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_CONTENT_DRAGON_PREY, SCT_CALLBACK_2(Logic_Client_Request::process_10106447, LOGIC_CLIENT_REQUEST));
	//active content end

	//你提我改
	register_client_process(REQ_SEND_PROPOSAL, SCT_CALLBACK_2(Logic_Client_Request::process_10120052, LOGIC_CLIENT_REQUEST));
	//你提我改

	//recharge
	register_client_process(REQ_OPEN_FIRST_RECHARGE, SCT_CALLBACK_2(Logic_Client_Request::process_10120090, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_FIRST_RECHARGE, SCT_CALLBACK_2(Logic_Client_Request::process_10120091, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_RECHARGE_REBATE, SCT_CALLBACK_2(Logic_Client_Request::process_10120092, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_RECHARGE_REBATE, SCT_CALLBACK_2(Logic_Client_Request::process_10120093, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_REBATE_RECHARGE_STATUS, SCT_CALLBACK_2(Logic_Client_Request::process_10120094, LOGIC_CLIENT_REQUEST));
	//recharge

	// fashion
//	register_client_process(REQ_FASHION_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10105001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ALTER_FASHION_STATE, SCT_CALLBACK_2(Logic_Client_Request::process_10105002, LOGIC_CLIENT_REQUEST));
//	register_client_process(REQ_MERGE_FASHION, SCT_CALLBACK_2(Logic_Client_Request::process_10105003, LOGIC_CLIENT_REQUEST));
//	register_client_process(REQ_UPGRADE_FASHION, SCT_CALLBACK_2(Logic_Client_Request::process_10105004, LOGIC_CLIENT_REQUEST));
//	register_client_process(REQ_FASHION_INHERIT, SCT_CALLBACK_2(Logic_Client_Request::process_10105005, LOGIC_CLIENT_REQUEST));
//	register_client_process(REQ_PICK_UP_CARD, SCT_CALLBACK_2(Logic_Client_Request::process_10105006, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CHANG_FASHION_SHOW_STATE, SCT_CALLBACK_2(Logic_Client_Request::process_10105007, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_GET_FASHION_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10105020, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_USE_FASHION_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10105021, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_FASHION_PART_UPGARDE, SCT_CALLBACK_2(Logic_Client_Request::process_10105022, LOGIC_CLIENT_REQUEST));
	//wings
	register_client_process(REQ_GET_WING_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10105011, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_WING, SCT_CALLBACK_2(Logic_Client_Request::process_10105012, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_WING_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10105013, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_UPGRADE_WING, SCT_CALLBACK_2(Logic_Client_Request::process_10105014, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_AUTO_UPGRADE_WING, SCT_CALLBACK_2(Logic_Client_Request::process_10105015, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CHANG_WING_SHOWED, SCT_CALLBACK_2(Logic_Client_Request::process_10105016, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_RET_RWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10105017, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_WING_EXPERIENCE, SCT_CALLBACK_2(Logic_Client_Request::process_10105018, LOGIC_CLIENT_REQUEST));
	// fashion

	//auction
	register_client_process(REQ_AUCTION_GOODS_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10107000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ADD_AUCTION_GOODS, SCT_CALLBACK_2(Logic_Client_Request::process_10107001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_UNDER_AUCTION_GOODS, SCT_CALLBACK_2(Logic_Client_Request::process_10107002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ONES_PRICE_BUY_AUCTION_GOODS, SCT_CALLBACK_2(Logic_Client_Request::process_10107003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_UPDATE_AUCTION_GOODS, SCT_CALLBACK_2(Logic_Client_Request::process_10107004, LOGIC_CLIENT_REQUEST));
	//auction

	//knight
	register_client_process(REQ_OPEN_KNIGHT_TRIALS_PLAN, SCT_CALLBACK_2(Logic_Client_Request::process_10107100, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ENTER_FIGHTER_KNIGHT, SCT_CALLBACK_2(Logic_Client_Request::process_10107101, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_KNIGHT_TRIALS_BOX_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10107102, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_KNIGHT_TRIALS_BOX_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10107103, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RESET_KNIGHT_TRIALS_PLAYER_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10107104, LOGIC_CLIENT_REQUEST));
	//knight

	// anci_artifact
	register_client_process(REQ_ANCI_ARTIFACT_LIST, SCT_CALLBACK_2(Logic_Client_Request::process_10101100, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ANCI_ARTIFACT_UNBAN, SCT_CALLBACK_2(Logic_Client_Request::process_10101101, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ANCI_ARTIFACT_ACTIVE, SCT_CALLBACK_2(Logic_Client_Request::process_10101102, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ANCI_ARTIFACT_BOX_GET, SCT_CALLBACK_2(Logic_Client_Request::process_10101103, LOGIC_CLIENT_REQUEST));
	// anci_artifact

	//invest plan begin
	register_client_process(REQ_OPEN_INVEST_PLAN, SCT_CALLBACK_2(Logic_Client_Request::process_10101200, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_MONTH_CARD_PROFIT, SCT_CALLBACK_2(Logic_Client_Request::process_10101201, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MONTH_CARD_INVEST, SCT_CALLBACK_2(Logic_Client_Request::process_10101202, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_UPGRADE_INVEST, SCT_CALLBACK_2(Logic_Client_Request::process_10101203, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_UPGRADE_INVEST_PROFIT, SCT_CALLBACK_2(Logic_Client_Request::process_10101204, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_UPGRADE_INVEST_MONEY, SCT_CALLBACK_2(Logic_Client_Request::process_10101205, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_ALL_UPGRADE_INVEST_PROFIT, SCT_CALLBACK_2(Logic_Client_Request::process_10101206, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_INVEST_PLAN_HISTORY, SCT_CALLBACK_2(Logic_Client_Request::process_10101207, LOGIC_CLIENT_REQUEST));
	//invest plan end

	// expedition begin
	register_client_process(REQ_EXPEDITION_OPEN_CHEST, SCT_CALLBACK_2(Logic_Client_Request::process_10101300, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EXPEDITION_CHEST_COLLECT, SCT_CALLBACK_2(Logic_Client_Request::process_10101301, LOGIC_CLIENT_REQUEST));
	// expedition end

	// thorlottery
	register_client_process(REQ_GET_SEASON_ID, SCT_CALLBACK_2(Logic_Client_Request::process_10108001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_THOR_LOTTERY, SCT_CALLBACK_2(Logic_Client_Request::process_10108002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_THORLOTTERY_EXCHANGE_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10108003, LOGIC_CLIENT_REQUEST));

	// card
	register_client_process(REQ_EMBED_CARD, SCT_CALLBACK_2(Logic_Client_Request::process_10109002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SWALLOW_CARD, SCT_CALLBACK_2(Logic_Client_Request::process_10109003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SELL_CARD, SCT_CALLBACK_2(Logic_Client_Request::process_10109005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EQUIP_OPEN_CARD_HOLE, SCT_CALLBACK_2(Logic_Client_Request::process_10109006, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_MERGE_CARD, SCT_CALLBACK_2(Logic_Client_Request::process_10109007, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_VIP_UPGRADE_TIMES, SCT_CALLBACK_2(Logic_Client_Request::process_10109008, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_VIP_UPGRADE_CARD, SCT_CALLBACK_2(Logic_Client_Request::process_10109009, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CARD_POTEN_UPGRADE, SCT_CALLBACK_2(Logic_Client_Request::process_10109010, LOGIC_CLIENT_REQUEST));


	register_client_process(REQ_QUESTIONNAIRE_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10120097, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SBUMIT_QUESTIONNAIRE, SCT_CALLBACK_2(Logic_Client_Request::process_10120098, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_OPEN_INVITE_REWARD_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10120060, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_INVITE_REWARD_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10120061, LOGIC_CLIENT_REQUEST));


	register_client_process(REQ_OPEN_37VIP_LEVEL_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10112001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_37VIP_LEVEL_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10112000, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_OPEN_TWO_RECHARGE, SCT_CALLBACK_2(Logic_Client_Request::process_10120099, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_TWO_RECHARGE, SCT_CALLBACK_2(Logic_Client_Request::process_10120100, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPEN_LIMIT_TIME_RECHARGE, SCT_CALLBACK_2(Logic_Client_Request::process_10120101, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_RECEIVE_LIMIT_TIME_RECHARGE, SCT_CALLBACK_2(Logic_Client_Request::process_10120102, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_PLOT_ALL_ADD_ONE, SCT_CALLBACK_2(Logic_Client_Request::process_10120110, LOGIC_CLIENT_REQUEST));

	register_client_process(REQ_OPEN_ACTIVTIY_EXCHANGE_MALL, SCT_CALLBACK_2(Logic_Client_Request::process_10106900, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_EXCHANGE_ACTIVTIY_EXCHANGE_MALL, SCT_CALLBACK_2(Logic_Client_Request::process_10106901, LOGIC_CLIENT_REQUEST));

    // life skill
	register_client_process(REQ_OPEN_LFSKILL_PANEL, SCT_CALLBACK_2(Logic_Client_Request::process_10116000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_LEARN_LIFE_SKILL, SCT_CALLBACK_2(Logic_Client_Request::process_10116001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_LIFE_SKILL_UPGRADE, SCT_CALLBACK_2(Logic_Client_Request::process_10116002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SPAR_REFINE, SCT_CALLBACK_2(Logic_Client_Request::process_10116003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CARD_MAKE, SCT_CALLBACK_2(Logic_Client_Request::process_10116004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_SPAR_SMELT, SCT_CALLBACK_2(Logic_Client_Request::process_10116005, LOGIC_CLIENT_REQUEST));

	//oper activity
	register_client_process(REQ_OPER_ACTIVITY_CONTENT_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10117000, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPER_ACTIVITY_CONTENT_REWARD, SCT_CALLBACK_2(Logic_Client_Request::process_10117001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPER_ACTIVITY_CONTENT_ICON_NUM, SCT_CALLBACK_2(Logic_Client_Request::process_10117002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_OPER_ACTIVITY_CONTENT_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10117003, LOGIC_CLIENT_REQUEST));

	// pet
	register_client_process(REQ_GET_PET_INFO, SCT_CALLBACK_2(Logic_Client_Request::process_10113001, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PET_UPGRADE, SCT_CALLBACK_2(Logic_Client_Request::process_10113002, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PET_AUTO_UPGRADE, SCT_CALLBACK_2(Logic_Client_Request::process_10113003, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_PET, SCT_CALLBACK_2(Logic_Client_Request::process_10113004, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PET_ILLUSION, SCT_CALLBACK_2(Logic_Client_Request::process_10113005, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_ACTIVE_PET_ILLUSION, SCT_CALLBACK_2(Logic_Client_Request::process_10113006, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_GET_PET_ACTIVE_ITEM, SCT_CALLBACK_2(Logic_Client_Request::process_10113007, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PET_LEARN_SKILL_NORMAL, SCT_CALLBACK_2(Logic_Client_Request::process_10113008, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_PET_LEARN_SKILL_ADVANCE, SCT_CALLBACK_2(Logic_Client_Request::process_10113009, LOGIC_CLIENT_REQUEST));
	register_client_process(REQ_CHANGE_PET_STATE, SCT_CALLBACK_2(Logic_Client_Request::process_10113010, LOGIC_CLIENT_REQUEST));

	return 0;
}

