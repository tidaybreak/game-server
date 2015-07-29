/*
 * Logic_Player_Detail.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *	只在逻辑服用
 */

#ifndef LOGIC_PLAYER_DETAIL_H_
#define LOGIC_PLAYER_DETAIL_H_

#include "Base_Detail.h"
#include "Fighter_Detail.h"
#include "pack/Pack_Struct.h"
#include "mail/Mail_Struct.h"
#include "friend/Friend_Struct.h"
#include "task/Task_Struct.h"
#include "item_user/Item_User_Struct.h"
#include "public/Public_Struct.h"
#include "example/Example_Struct.h"
#include "operating/Operating_Struct.h"
#include "dungeon/Single_Dungeon_Struct.h"
#include "equiper/Equiper_Struct.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "rune/Rune_Struct.h"
#include "rune_stone/Rune_Stone_Struct.h"
#include "team/Team_Struct.h"
#include "gang/Gang_Struct.h"
#include "alchemy/Alchemy_Struct.h"
#include "icon/Icon_Struct.h"
#include "arena/Arena_Struct.h"
#include "status/Logic_Status_Struct.h"
#include "exchange/Exchange_Struct.h"
#include "helper/Helper_Struct.h"
#include "covenant/Covenant_Struct.h"
#include "dragon/Dragon_Vale_Struct.h"
#include "achievement/Achievement_Struct.h"
#include "title/Title_Struct.h"
#include "vip/Vip_Struct.h"
#include "mount/Mount_Struct.h"
#include "reward/Reward_Struct.h"
#include "active/Active_Content_Struct.h"
#include "fashion/Fashion_Struct.h"
#include "auction/Auction_Struct.h"
#include "knight_trials/Knight_Trials_Struct.h"
#include "anci_artifact/Anci_Artifact_Struct.h"
#include "invest_plan/Invest_Plan_Struct.h"
#include "wing/Wing_Struct.h"
#include "expedition/Expedition_Struct.h"
#include "thorlottery/ThorLottery_Struct.h"
#include "activity/Activity_Struct.h"
#include "r2_invite/R2_Invite_Struct.h"
#include "activities_stores/Activities_Stores_Struct.h"
#include "life_skill/Life_Skill_Struct.h"
#include "oper_activity/Oper_Activity_Struct.h"
#include "pet/Pet_Struct.h"

//////////////////////////////////////////////////////////////////////////////

struct Logic_Player_Detail {
	enum LOAD_STATUS {
		NULL_STUB,
		SUCCESS_LOADED,		/// 加载成功
		ROLE_NOT_EXIST,		/// 角色不存在
	};

	Logic_Player_Detail(void);
	void reset(void);
	void set_logout(void);

	role_id_t role_id;
	int status;
	bool logout;				// 是否退出保存

	Base_Detail base_detail;

	Example_Detail example_detail;
	Icon_Detail icon_detail;
	Public_Detail public_detail;
	Operating_Detail operating_detail;
	Packer_Detail packer_detail;
	Tasker_Detail tasker_detail;
	Fighter_Detail fighter_detail;
	Mail_Detail mail_detail;
	Friend_Detail friend_detail;
	Item_User_Detail item_user_detail;
	Sgl_Dungeon_Detail item_sgl_dungeon_detail;
	Equiper_Detail equiper_detail;
	Heroer_Detail heroer_detail;
	Rune_Detail rune_detail;
	Rune_Stone_Detail rune_stone_detail;
	Teamer_Detail teamer_detail;
	Ganger_Detail ganger_detail;
	Alchemy_Detail alchemy_detail;
	Arenaer_Detail arenaer_detail;
	Logic_Status_Detail logic_status_detail;
	Exchange_Detail exchange_detail;
	Helper_Detail helper_detail;
	Covenant_Detail covenant_detail;
	Dragon_Vale_Detail dragon_vale_detail;
	Achievement_Detail achievement_detail;
	Title_Detail title_detail;
	Vip_Detail vip_deatil;
	Mount_Detail mount_detail;
	Reward_Detail reward_detail;
	Active_Content_Detail active_content_detail;
	Fashion_Detail fashion_detail;
	Auction_Detail auction_detail;
	Knight_Trials_Detail knight_trials_detail;
	Knight_Trials_Player_Detail knight_trials_player_detail;
	Anci_Artifact_Detail anci_artifact_detail;
	Invest_Plan_Detail invest_plan_detail;
	Wing_Detail wing_detail;
	Expedition_Detail expedition_detail;
	ThorLottery_Detail thorlottery_detail;
	Activity_Detail activity_detail;
	R2_Invite_Detail r2_invite_detail;
	Activities_Stores_Detail activities_stores_detail;
	Life_Skill_Detail life_skill_detail;
	Oper_Activity_Detail oper_activity_detail;
	Pet_Detail pet_detail;

	int login_player_load(void);
	int save_change_detail(void);

	int arena_fighter_load(void);
	int dragon_vale_fighter_load();
	int knight_trials_fighter_load();
};

#endif /* LOGIC_PLAYER_DETAIL_H_ */
