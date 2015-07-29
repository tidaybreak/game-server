/*
 * Logic_Player_Detail.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ti
 */

#include "Pool_Manager.h"
#include "Logic_Player_Detail.h"
#include "DB_Operator.h"
#include "Record_Client.h"

//////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////

Logic_Player_Detail::Logic_Player_Detail(void) {
	reset();
}

void Logic_Player_Detail::set_logout(void) {
	logout = true;
}

void Logic_Player_Detail::reset(void) {
	role_id = 0;
	status = 0;
	logout = false;

	base_detail.reset();

	example_detail.reset();
	public_detail.reset();
	operating_detail.reset();
	packer_detail.reset();
	tasker_detail.reset();
	mail_detail.reset();
	friend_detail.reset();
	fighter_detail.reset();
	item_user_detail.reset();
	item_sgl_dungeon_detail.reset();
	equiper_detail.reset();
	heroer_detail.reset();
	rune_detail.reset();
	rune_stone_detail.reset();
	teamer_detail.reset();
	ganger_detail.reset();
	alchemy_detail.reset();
	icon_detail.reset();
	arenaer_detail.reset();
	logic_status_detail.reset();
	exchange_detail.reset();
	helper_detail.reset();
	covenant_detail.reset();
	dragon_vale_detail.reset();
	achievement_detail.reset();
	title_detail.reset();
	vip_deatil.reset();
	mount_detail.reset();
	reward_detail.reset();
	active_content_detail.reset();
	fashion_detail.reset();
	knight_trials_player_detail.reset();
	knight_trials_detail.reset();
	anci_artifact_detail.reset();
	invest_plan_detail.reset();
	wing_detail.reset();
	expedition_detail.reset();
	thorlottery_detail.reset();
	activity_detail.reset();
	r2_invite_detail.reset();
	activities_stores_detail.reset();
	life_skill_detail.reset();
	oper_activity_detail.reset();
	pet_detail.reset();
}

int Logic_Player_Detail::login_player_load(void) {
	if (!MONGO_OPERATOR->has_role_by_id(role_id)) {
		status = ROLE_NOT_EXIST;
		return 0;
	}

	status = SUCCESS_LOADED;

	base_detail.role_id = role_id;
	base_detail.load();

	fighter_detail.role_id = role_id;
	fighter_detail.load();

	example_detail.role_id = role_id;
	example_detail.load();

	public_detail.role_id = role_id;
	public_detail.load();

	operating_detail.role_id = role_id;
	operating_detail.load();

	packer_detail.role_id = role_id;
	packer_detail.load();

	tasker_detail.role_id = role_id;
	tasker_detail.load();

	mail_detail.role_id = role_id;
	mail_detail.load();

	friend_detail.role_id = role_id;
	friend_detail.load();

	item_user_detail.role_id = role_id;
	item_user_detail.load();

	item_sgl_dungeon_detail.role_id = role_id;
	item_sgl_dungeon_detail.load();

	equiper_detail.role_id = role_id;
	equiper_detail.load();

	heroer_detail.role_id = role_id;
	heroer_detail.load();

	rune_detail.role_id = role_id;
	rune_detail.load();

	rune_stone_detail.role_id = role_id;
	rune_stone_detail.load();

	teamer_detail.role_id = role_id;
	teamer_detail.load();

	ganger_detail.role_id = role_id;
	ganger_detail.load();

	alchemy_detail.role_id = role_id;
	alchemy_detail.load();

	icon_detail.role_id = role_id;
	icon_detail.load();

	arenaer_detail.role_id = role_id;
	arenaer_detail.load();

	logic_status_detail.role_id = role_id;
	logic_status_detail.load();

	exchange_detail.role_id = role_id;
	exchange_detail.load();

	helper_detail.role_id = role_id;
	helper_detail.load();

	covenant_detail.role_id = role_id;
	covenant_detail.load();

	dragon_vale_detail.role_id = role_id;
	dragon_vale_detail.load();

	achievement_detail.role_id = role_id;
	achievement_detail.load();

	title_detail.role_id = role_id;
	title_detail.load();

	vip_deatil.role_id = role_id;
	vip_deatil.load();

	mount_detail.role_id = role_id;
	mount_detail.load();

	reward_detail.role_id = role_id;
	reward_detail.load();

	active_content_detail.role_id = role_id;
	active_content_detail.load();

	fashion_detail.role_id = role_id;
	fashion_detail.load();

	knight_trials_player_detail.role_id = role_id;
	knight_trials_player_detail.load();

	knight_trials_detail.role_id = role_id;
	knight_trials_detail.load();

	anci_artifact_detail.role_id = role_id;
	anci_artifact_detail.load();

	invest_plan_detail.role_id = role_id;
	invest_plan_detail.load();

	wing_detail.role_id = role_id;
	wing_detail.load();

	expedition_detail.role_id = role_id;
	expedition_detail.load();

	thorlottery_detail.role_id = role_id;
	thorlottery_detail.load();

	activity_detail.role_id = role_id;
	activity_detail.load();

	r2_invite_detail.role_id = role_id;
	r2_invite_detail.load();

	activities_stores_detail.role_id = role_id;
	activities_stores_detail.load();

	life_skill_detail.role_id = role_id;
	life_skill_detail.load();

	oper_activity_detail.role_id = role_id;
	oper_activity_detail.load();

	pet_detail.role_id = role_id;
	pet_detail.load();

	return 0;
}

int Logic_Player_Detail::save_change_detail(void) {
	if (logout) {
		fighter_detail.logout = true;
	}

	base_detail.save();
	fighter_detail.save();
	example_detail.save();
	public_detail.save();
	operating_detail.save();
	packer_detail.save();
	tasker_detail.save();
	mail_detail.save();
	friend_detail.save();
	item_user_detail.save();
	item_sgl_dungeon_detail.save();
	equiper_detail.save();
	heroer_detail.save();
	rune_detail.save();
	rune_stone_detail.save();
	teamer_detail.save();
	ganger_detail.save();
	alchemy_detail.save();
	icon_detail.save();
	arenaer_detail.save();
	logic_status_detail.save();
	exchange_detail.save();
	helper_detail.save();
	covenant_detail.save();
	dragon_vale_detail.save();
	achievement_detail.save();
	title_detail.save();
	vip_deatil.save();
	mount_detail.save();
	reward_detail.save();
	active_content_detail.save();
	fashion_detail.save();
	knight_trials_player_detail.save();
	knight_trials_detail.save();
	anci_artifact_detail.save();
	invest_plan_detail.save();
	wing_detail.save();
	expedition_detail.save();
	thorlottery_detail.save();
	activity_detail.save();
	r2_invite_detail.save();
	activities_stores_detail.save();
	life_skill_detail.save();
	oper_activity_detail.save();
	pet_detail.save();
	return 0;
}

int Logic_Player_Detail::arena_fighter_load(void){
	if (!MONGO_OPERATOR->has_role_by_id(role_id)) {
		status = ROLE_NOT_EXIST;
		return 0;
	}

	status = SUCCESS_LOADED;

	base_detail.role_id = role_id;
	base_detail.arena_load();

	fighter_detail.role_id = role_id;
	fighter_detail.arena_load();

	heroer_detail.role_id = role_id;
	heroer_detail.load_fight_hero();

	return 0;
}

int Logic_Player_Detail::knight_trials_fighter_load() {
	if (!MONGO_OPERATOR->has_role_by_id(role_id)) {
		status = ROLE_NOT_EXIST;
		return 0;
	}

	status = SUCCESS_LOADED;

	base_detail.role_id = role_id;
	base_detail.arena_load();

	fighter_detail.role_id = role_id;
	fighter_detail.arena_load();

	heroer_detail.role_id = role_id;
	heroer_detail.load_fight_hero();

	return 0;
}

int Logic_Player_Detail::dragon_vale_fighter_load() {
	if (!MONGO_OPERATOR->has_role_by_id(role_id)) {
		status = ROLE_NOT_EXIST;
		return 0;
	}

	status = SUCCESS_LOADED;

	base_detail.role_id = role_id;
	base_detail.load();

	fighter_detail.role_id = role_id;
	fighter_detail.arena_load();

	heroer_detail.role_id = role_id;
	heroer_detail.load_fight_hero();

	dragon_vale_detail.role_id = role_id;
	dragon_vale_detail.load();

	return 0;
}
