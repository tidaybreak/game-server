/*
 * Task_Struct.cpp
 *
 *  Created on: 2014年3月11日
 *      Author: root
 */


#include "Task_Struct.h"
#include "Pool_Manager.h"
#include "DB_Operator.h"
#include "Logic_Player.h"

void TaskCondition::reset(void)
{
    this->__type = TASK_CT_NONE;
    this->__current_value = 0;
    ext_set.clear();
}

/*bool TaskCondition::is_no_condtion(void)
{
    return this->__type == TASK_CT_NONE;
}

bool TaskCondition::is_kill_monster(void)
{
    return this->__type == TASK_CT_KILL_MONSTER;
}

bool TaskCondition::is_fb(void)
{
	return this->__type == TASK_CT_FB;
}

bool TaskCondition::is_npc_dialogue(void)
{
    return this->__type == TASK_CT_NPC_DIALOGUE;
}

bool TaskCondition::is_arrive(void)
{
    return this->__type == TASK_CT_ARRIVE;
}

bool TaskCondition::is_level(void)
{
    return this->__type == TASK_CT_LVL;
}

bool TaskCondition::is_collect_item(void)
{
    return this->__type == TASK_CT_COLLECT;
}

bool TaskCondition::is_kill_monster_group(void)
{
    return this->__type == TASK_CT_KILL_MONSTER_GROUP;
}

bool TaskCondition::is_package_item(void)
{
	return this->__type == TASK_CT_PACKAGE;
}

bool TaskCondition::is_fb_type(void)
{
    return this->__type == TASK_CT_FB_TYPE;
}

bool TaskCondition::is_win_player(void)
{
    return this->__type == TASK_CT_WIN_PLAYER;
}

bool TaskCondition::is_join_gang()
{
	return this->__type == TASK_CT_JOIN_GANG;
}

bool TaskCondition::is_gang_level()
{
	return this->__type == TASK_CT_GANG_LEVEL;
}

bool TaskCondition::is_player_all_equip()
{
	return this->__type == TASK_CT_PLAYER_ALL_EQUIP;
}

bool TaskCondition::is_body_equip()
{
	return this->__type == TASK_CT_BODY_EQUIP;
}

bool TaskCondition::is_equip_level()
{
	return this->__type ==  TASK_CT_EQUIP_LEVEL;
}

bool TaskCondition::is_body_equip_level()
{
	return this->__type == TASK_CT_BODY_EQUIP_LEVEL;
}

bool TaskCondition::is_stone_level()
{
	return this->__type == TASK_CT_STONE_LEVEL;
}

bool TaskCondition::is_buy_goods()
{
	return this->__type == TASK_CT_BUY_GOODS;
}

bool TaskCondition::is_get_hero()
{
	return this->__type == TASK_CT_GET_HERO;
}

bool TaskCondition::is_fight()
{
	return this->__type == TASK_CT_FIGHT;
}*/

TaskInfo::TaskInfo(void)
{
	reset();
}

void TaskInfo::reset(void)
{
    this->__task_id = 0;
    this->__game_type = TASK_GTYPE_NONE;

    this->__accept_tick = Time_Value::zero;
    this->__refresh_tick = Time_Value::zero;

    this->__prev_task = 0;
    this->__post_task = 0;
    this->__post_task2 = 0;
    this->__post_task3 = 0;

    this->__status.reset();
    this->__logic_type.reset();
//    CLEAR_VECTOR(TaskConditionList, __condition_list);
    this->__condition_list.clear();
    this->__item_money_flag = 0;
    this->__recruid_hero = false;
    this->__is_req_fast_finish = false;
    this->__hero_id = 0;
    this->__is_pack_accept = false;
}

bool TaskInfo::is_visible(void) const
{
    return this->__status.test(TASK_STATUS_VISIBLE);
}

bool TaskInfo::is_acceptable(void) const
{
    return this->__status.test(TASK_STATUS_ACCEPTABLE);
}

bool TaskInfo::is_accepted(void) const
{
    return this->__status.test(TASK_STATUS_ACCEPTED);
}

bool TaskInfo::is_finish(void) const
{
    return this->__status.test(TASK_STATUS_FINISH);
}

bool TaskInfo::is_submit(void) const
{
    return this->__status.test(TASK_STATUS_SUBMIT);
}

bool TaskInfo::is_fail() const {
	return this->__status.test(TASK_STATUS_FAIL);
}

void TaskInfo::set_task_status(const int status)
{
    if (status == TASK_STATUS_ACCEPTED)
    {
        this->__status.reset(TASK_STATUS_VISIBLE);
        this->__status.reset(TASK_STATUS_ACCEPTABLE);
        this->__status.reset(TASK_STATUS_FINISH);
        this->__status.reset(TASK_STATUS_SUBMIT);
        this->__status.reset(TASK_STATUS_FAIL);
    }
    else if (status == TASK_STATUS_FINISH)
    {
        this->__status.reset(TASK_STATUS_VISIBLE);
        this->__status.reset(TASK_STATUS_ACCEPTABLE);
        this->__status.reset(TASK_STATUS_ACCEPTED);
        this->__status.reset(TASK_STATUS_SUBMIT);
        this->__status.reset(TASK_STATUS_FAIL);
    }
    else if (status == TASK_STATUS_SUBMIT)
    {
        this->__status.reset(TASK_STATUS_VISIBLE);
        this->__status.reset(TASK_STATUS_ACCEPTABLE);
        this->__status.reset(TASK_STATUS_ACCEPTED);
        this->__status.reset(TASK_STATUS_FINISH);
        this->__status.reset(TASK_STATUS_FAIL);
    }
    else if (status == TASK_STATUS_ACCEPTABLE)
    {
        this->__status.reset(TASK_STATUS_VISIBLE);
        this->__status.reset(TASK_STATUS_ACCEPTED);
        this->__status.reset(TASK_STATUS_FINISH);
        this->__status.reset(TASK_STATUS_SUBMIT);
        this->__status.reset(TASK_STATUS_FAIL);
    }
    else if(status == TASK_STATUS_FAIL) {
    	this->__status.reset(TASK_STATUS_VISIBLE);
        this->__status.reset(TASK_STATUS_ACCEPTABLE);
        this->__status.reset(TASK_STATUS_ACCEPTED);
        this->__status.reset(TASK_STATUS_FINISH);
        this->__status.reset(TASK_STATUS_SUBMIT);
    }
    else if(status == TASK_STATUS_VISIBLE) {
        this->__status.reset(TASK_STATUS_ACCEPTABLE);
        this->__status.reset(TASK_STATUS_ACCEPTED);
        this->__status.reset(TASK_STATUS_FINISH);
        this->__status.reset(TASK_STATUS_SUBMIT);
    	this->__status.reset(TASK_STATUS_FAIL);
    }

    this->__status.set(status, true);
}

void TaskInfo::reset_task_status(const int status)
{
    this->__status.reset(status);
}

bool TaskInfo::is_main_task(void) const
{
    return this->__game_type == TASK_GTYPE_MAINLINE;
}

bool TaskInfo::is_branch_task(void) const
{
    return this->__game_type == TASK_GTYPE_BRANCH;
}

bool TaskInfo::is_day_task(void) const
{
    return this->__game_type == TASK_GTYPE_DAY;
}

bool TaskInfo::is_week_task(void) const
{
    return this->__game_type == TASK_GTYPE_WEEK;
}

bool TaskInfo::is_sociaty_task(void) const
{
    return this->__game_type == TASK_GTYPE_SOCIATY;
}

bool TaskInfo::is_activity_task(void) const
{
    return this->__game_type == TASK_GTYPE_ACTIVITY;
}

bool TaskInfo::is_elite_task(void) const
{
    return this->__game_type == TASK_GTYPE_ELITE;
}

bool TaskInfo::is_activity2_task(void) const
{
    return this->__game_type == TASK_GTYPE_ACTIVITY2;
}

bool TaskInfo::is_hero_task(void) const
{
    return this->__game_type == TASK_GTYPE_HERO;
}

bool TaskInfo::is_reward_task() const {
	return this->__game_type == TASK_GTYPE_REWARD_TASK;
}

bool TaskInfo::is_ring_task() const {
	return this->__game_type == TASK_GTYPE_RING_TASK;
}

bool TaskInfo::is_open_ring_task() const {
	return this->__game_type == TASK_GTYPE_OPEN_RING_TASK;
}

bool TaskInfo::is_close_ring_task() const {
	return this->__game_type == TASK_GTYPE_CLOSE_RING_TASK;
}

bool TaskInfo::is_talent_task() const {
	return this->__game_type == TASK_GTYPE_TALENT;
}

void TaskInfo::set_logic_type(const int type)
{
    this->__logic_type.set(type, true);
}

void TaskInfo::reset_logic_type(const int type)
{
    this->__logic_type.reset(type);
}

bool TaskInfo::is_logic_kill_monster(void)
{
    return this->__logic_type.test(TaskCondition::TASK_CT_KILL_MONSTER);
}

bool TaskInfo::is_logic_fb(void) {
	return this->__logic_type.test(TaskCondition::TASK_CT_FB);
}

bool TaskInfo::is_logic_npc_talk(void)
{
    return this->__logic_type.test(TaskCondition::TASK_CT_NPC_TALK);
}

bool TaskInfo::is_logic_arrive(void) {
	return this->__logic_type.test(TaskCondition::TASK_CT_ARRIVE);
}

bool TaskInfo::is_logic_level_up(void) {
    return this->__logic_type.test(TaskCondition::TASK_CT_LVL);
}

bool TaskInfo::is_logic_package(void) {
	return this->__logic_type.test(TaskCondition::TASK_CT_PACKAGE);
}

bool TaskInfo::is_logic_kill_monster_group(void)
{
    return this->__logic_type.test(TaskCondition::TASK_CT_KILL_MONSTER_GROUP);
}

bool TaskInfo::is_logic_collect(void) {
    return this->__logic_type.test(TaskCondition::TASK_CT_COLLECT);
}

bool TaskInfo::is_logic_fb_type(void)
{
    return this->__logic_type.test(TaskCondition::TASK_CT_FB_TYPE);
}

bool TaskInfo::is_logic_win_player(void) {
	return this->__logic_type.test(TaskCondition::TASK_CT_WIN_PLAYER);
}

bool TaskInfo::is_logic_join_gang() {
	return this->__logic_type.test(TaskCondition::TASK_CT_JOIN_GANG);
}

bool TaskInfo::is_logic_ganger_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_GANGER_LEVEL);
}

bool TaskInfo::is_logic_player_all_equip() {
	return this->__logic_type.test(TaskCondition::TASK_CT_PLAYER_ALL_EQUIP);
}

bool TaskInfo::is_logic_body_equip() {
	return this->__logic_type.test(TaskCondition::TASK_CT_BODY_EQUIP);
}

bool TaskInfo::is_logic_body_equip_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_BODY_EQUIP_LEVEL);
}

bool TaskInfo::is_logic_equip_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_EQUIP_LEVEL);
}

bool TaskInfo::is_logic_stone_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_STONE_LEVEL);
}

bool TaskInfo::is_logic_buy_goods() {
	return this->__logic_type.test(TaskCondition::TASK_CT_BUY_GOODS);
}

bool TaskInfo::is_logic_get_hero() {
	return this->__logic_type.test(TaskCondition::TASK_CT_GET_HERO);
}

bool TaskInfo::is_logic_fight() {
	return this->__logic_type.test(TaskCondition::TASK_CT_FIGHT);
}

bool TaskInfo::is_logic_commit_consume_item() {
	return this->__logic_type.test(TaskCondition::TASK_CT_COMMIT_CONSUME_ITEM);
}

bool TaskInfo::is_logic_deliver_letter() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DELIVER_LETTER);
}

bool TaskInfo::is_logic_help_other_complete_team_scene() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HELP_OTHER_COMPLETE_TEAM_SCENE);
}

bool TaskInfo::is_logic_sell_item() {
	return this->__logic_type.test(TaskCondition::TASK_CT_SELL_ITEM);
}

bool TaskInfo::is_logic_arena() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ARENA);
}

bool TaskInfo::is_logic_chat_with_friend() {
	return this->__logic_type.test(TaskCondition::TASK_CT_CHAT_WITH_FRIEND);
}

bool TaskInfo::is_logic_send_mail() {
	return this->__logic_type.test(TaskCondition::TASK_CT_SEND_MAIL);
}

bool TaskInfo::is_logic_team_scene_or_pvp() {
	return this->__logic_type.test(TaskCondition::TASK_CT_TEAM_SCENE_OR_PVP);
}

bool TaskInfo::is_logic_browse_other_player_info() {
	return this->__logic_type.test(TaskCondition::TASK_CT_BROWSE_OTHER_PLAYER_INFO);
}

bool TaskInfo::is_logic_arena_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ARENA_LEVEL);
}

bool TaskInfo::is_logic_equip_crimson_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_EQUIP_CRIMSON_LEVEL);
}

bool TaskInfo::is_logic_arena_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ARENA_ALL);
}

bool TaskInfo::is_logic_create_dragon_vale_building() {
	return this->__logic_type.test(TaskCondition::TASK_CT_CREATE_DRAGON_VALE_BUILDING);
}

bool TaskInfo::is_logic_gain_dragon_vale_builiding_production() {
	return this->__logic_type.test(TaskCondition::TASK_CT_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION);
}

bool TaskInfo::is_logic_wake_up_hero_to_quality_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_WAKE_UP_HERO_TO_QUALITY_LEVEL);
}

bool TaskInfo::is_logic_keep_hero_to_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_KEEP_HERO_TO_LEVEL);
}

bool TaskInfo::is_logic_compose_equip() {
	return this->__logic_type.test(TaskCondition::TASK_CT_COMPOSE_EQUIP);
}

bool TaskInfo::is_logic_hero_dream_scene() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HERO_DREAM_SCENE);
}

bool TaskInfo::is_logic_ganger_skill_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_GANGER_SKILL_LEVEL);
}

bool TaskInfo::is_logic_war() {
	return this->__logic_type.test(TaskCondition::TASK_CT_WAR);
}

bool TaskInfo::is_logic_war_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_WAR_ALL);
}

bool TaskInfo::is_logic_kill_enemy_in_war() {
	return this->__logic_type.test(TaskCondition::TASK_CT_KILL_ENEMY_IN_WAR);
}

bool TaskInfo::is_logic_single_arena_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_SINGLE_ARENA_ALL);
}

bool TaskInfo::is_logic_single_arena_win() {
	return this->__logic_type.test(TaskCondition::TASK_CT_SINGLE_ARENA_WIN);
}

bool TaskInfo::is_logic_team_arena_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_TEAM_ARENA_ALL);
}

bool TaskInfo::is_logic_team_arena_win() {
	return this->__logic_type.test(TaskCondition::TASK_CT_TEAM_ARENA_WIN);
}

bool TaskInfo::is_logic_ganger_bless_buf() {
	return this->__logic_type.test(TaskCondition::TASK_CT_GANGER_BLESS_BUF);
}

bool TaskInfo::is_logic_rob_other_dragon_vale_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ROB_OTHER_DRAGON_VALE_ALL);
}

bool TaskInfo::is_loigc_dragon_vale_step_foot_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DRAGON_VALE_STEP_FOOT_ALL);
}

bool TaskInfo::is_logic_activeness_of_today() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ACTIVENESS_OF_TODAY);
}


bool TaskInfo::is_logic_dragon_vale_building_upgrade() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DRAGON_VALE_BUILDING_UPGRADE);
}

bool TaskInfo::is_logic_limit_time_kill_boss() {
	return this->__logic_type.test(TaskCondition::TASK_CT_LIMIT_TIME_KILL_BOSS);
}

bool TaskInfo::is_logic_dragon_keep_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DRAGON_KEEP_ALL);
}

bool TaskInfo::is_logic_dragon_keep_success() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DRAGON_KEEP_SUCCESS);
}

bool TaskInfo::is_logic_donate_to_gang() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DONATE_TO_GANG);
}

bool TaskInfo::is_logic_present_buf_to_other() {
	return this->__logic_type.test(TaskCondition::TASK_CT_PRESENT_BUF_TO_OTHER);
}

bool TaskInfo::is_logic_finish_reward_task_num() {
	return this->__logic_type.test(TaskCondition::TASK_CT_FINISH_REWARD_TASK_NUM);
}

bool TaskInfo::is_logic_keep_dragon_to_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_KEEP_DRAGON_TO_LEVEL);
}

bool TaskInfo::is_logic_knight_trial() {
	return this->__logic_type.test(TaskCondition::TASK_CT_KNIGHT_TRIAL);
}

bool TaskInfo::is_logic_suit() {
	return this->__logic_type.test(TaskCondition::TASK_CT_SUIT);
}

bool TaskInfo::is_logic_role_pull_on_equip()  {
	return this->__logic_type.test(TaskCondition::TASK_CT_ROLE_PULL_ON_EQUIP);
}

bool TaskInfo::is_logic_hero_pull_on_equip() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HERO_PULL_ON_EQUIP);
}

bool TaskInfo::is_logic_activate_wing() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ACTIVATE_WING);
}

bool TaskInfo::is_logic_get_quality_rune() {
	return this->__logic_type.test(TaskCondition::TASK_CT_GET_QUALITY_RUNE);
}

bool TaskInfo::is_logic_pass_kngiht_trials() {
	return this->__logic_type.test(TaskCondition::TASK_CT_PASS_KNGIHT_TRIALS);
}

bool TaskInfo::is_logic_have_hero() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HAVE_HERO);
}

bool TaskInfo::is_logic_have_hero_quality() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HAVE_HERO_QUALITY);
}

bool TaskInfo::is_logic_first_pass_scene() {
	return this->__logic_type.test(TaskCondition::TASK_CT_FIRST_PASS_SCENE);
}

bool TaskInfo::is_logic_dragon_born() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DRAGON_BORN);
}

bool TaskInfo::is_logic_horse_star_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HORSE_STAR_LEVEL);
}

bool TaskInfo::is_logic_have_item_but_no_erase() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HAVE_ITEM_BUT_NO_ERASE);
}

bool TaskInfo::is_logic_have_equip_but_no_erase() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HAVE_EQUIP_BUT_NO_ERASE);
}

bool TaskInfo::is_logic_same_gang_challenge_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_SAME_GANG_CHALLENGE_ALL);
}

bool TaskInfo::is_logic_main_scene_npc_battle() {
	return this->__logic_type.test(TaskCondition::TASK_CT_MAIN_SCENE_NPC_BATTLE);
}

bool TaskInfo::is_logic_equip_card_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_EQUIP_CARD_ALL);
}

bool TaskInfo::is_logic_equip_card_by_position() {
	return this->__logic_type.test(TaskCondition::TASK_CT_EQUIP_CARD_BY_POSITION);
}

bool TaskInfo::is_logic_card_upgrade_to_level_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_CARD_UPGRADE_TO_LEVEL_ALL);
}

bool TaskInfo::is_logic_card_upgrade_to_level_by_position() {
	return this->__logic_type.test(TaskCondition::TASK_CT_CARD_UPGRADE_TO_LEVEL_BY_POSITION);
}

bool TaskInfo::is_logic_lottery_draw() {
	return this->__logic_type.test(TaskCondition::TASK_CT_LOTTERY_DRAW);
}

bool TaskInfo::is_logic_equip_jewelry() {
	return this->__logic_type.test(TaskCondition::TASK_CT_EQUIP_JEWELRY);
}

bool TaskInfo::is_logic_invite_friend_step_foot() {
	return this->__logic_type.test(TaskCondition::TASK_CT_INVITE_FRIEND_STEP_FOOT);
}

bool TaskInfo::is_logic_kill_group_num() {
	return this->__logic_type.test(TaskCondition::TASK_CT_KILL_GROUP_NUM);
}

bool TaskInfo::is_logic_did_reward_task_num() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DID_REWARD_TASK_NUM);
}

bool TaskInfo::is_logic_alchemy_num() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ALCHEMY_NUM);
}

bool TaskInfo::is_logic_did_reward_task_num_today() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DID_REWARD_TASK_NUM_TODAY);
}

bool TaskInfo::is_logic_remote_scene_any_pass() {
	return this->__logic_type.test(TaskCondition::TASK_CT_REMOTE_SCENE_ANY_PASS);
}

bool TaskInfo::is_logic_knight_trial_any_pass() {
	return this->__logic_type.test(TaskCondition::TASK_CT_KNIGHT_TRIAL_ANY_PASS);
}

bool TaskInfo::is_logic_compose_gemstone_level() {
	return this->__logic_type.test(TaskCondition::TASK_CT_COMPOSE_GEMSTONE_LEVEL);
}

bool TaskInfo::is_logic_inlay_position_gemstone() {
	return this->__logic_type.test(TaskCondition::TASK_CT_INLAY_POSITION_GEMSTONE);
}

bool TaskInfo::is_logic_inlay_any_gemstone() {
	return this->__logic_type.test(TaskCondition::TASK_CT_INLAY_ANY_GEMSTONE);
}

bool TaskInfo::is_logic_world_boss() {
	return this->__logic_type.test(TaskCondition::TASK_CT_WORLD_BOSS);
}

bool TaskInfo::is_logic_world_boss_immediate_revive() {
	return this->__logic_type.test(TaskCondition::TASK_CT_WORLD_BOSS_IMMEDIATE_REVIVE);
}

bool TaskInfo::is_logic_enchant_any_equip() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ENCHANT_ANY_EQUIP);
}

bool TaskInfo::is_logic_inlay_id_gemstone() {
	return this->__logic_type.test(TaskCondition::TASK_CT_INLAY_ID_GEMSTONE);
}

bool TaskInfo::is_logic_gang_well_recover_spirit() {
	return this->__logic_type.test(TaskCondition::TASK_CT_GANG_WELL_RECOVER_SPIRIT);
}

bool TaskInfo::is_logic_equip_purple_ring() {
	return this->__logic_type.test(TaskCondition::TASK_CT_EQUIP_PURPLE_RING);
}

bool TaskInfo::is_logic_inlay_level_gemstone() {
	return this->__logic_type.test(TaskCondition::TASK_CT_INLAY_LEVEL_GEMSTONE);
}

bool TaskInfo::is_logic_upgrade_card() {
	return this->__logic_type.test(TaskCondition::TASK_CT_UPGRADE_CARD);
}

bool TaskInfo::is_logic_role_equip_4() {
	return this->__logic_type.test(TaskCondition::TASK_CT_ROLE_EQUIP_4);
}

bool TaskInfo::is_logic_hero_equip_4() {
	return this->__logic_type.test(TaskCondition::TASK_CT_HERO_EQUIP_4);
}

bool TaskInfo::is_logic_dayly_single_arena_all() {
	return this->__logic_type.test(TaskCondition::TASK_CT_DAYLY_SINGLE_ARENA_ALL);
}

/*bool TaskInfo::is_logic_use_skill() {
	return this->__logic_type.test(TaskCondition::TASK_CT_USE_SKILL);
}

bool TaskInfo::is_logic_append_skill() {
	return this->__logic_type.test(TaskCondition::TASK_CT_APPEND_SKILL);
}*/


int TaskInfo::condition_size(void)
{
    return int(this->__condition_list.size());
}

TaskCondition *TaskInfo::task_condition(const int index)
{
    if (index < 0 || index >= int(this->__condition_list.size()))
        return 0;
    return &this->__condition_list[index];
}

/*
int TaskInfo::task_status(void) const
{
    if (this->is_submit()) //
        return TASK_STATUS_SUBMIT;
    else if (this->is_finish())
        return TASK_STATUS_FINISH;
    else if (this->is_accepted())
        return TASK_STATUS_ACCEPTED;
    else if (this->is_acceptable())
        return TASK_STATUS_ACCEPTABLE;
    else if (this->is_visible())
        return TASK_STATUS_VISIBLE;
    else
        return TASK_STATUS_NONE;
}
*/

int Task_Npc_Type::serialize(Block_Buffer &buf) const {
	buf.write_int32(npc_type);
	buf.write_int64(npc_id);
	buf.write_string(npc_name);
	buf.write_string(funcName);
	buf.write_int32(direction);
	buf.write_string(sceneId);
	buf.write_string(open);
	buf.write_int32(x);
	buf.write_int32(y);
	return 0;
}
int Task_Npc_Type::deserialize(Block_Buffer &buf) {
	buf.read_int32(npc_type);
	buf.read_int64(npc_id);
	buf.read_string(npc_name);
	buf.read_string(funcName);
	buf.read_int32(direction);
	buf.read_string(sceneId);
	buf.read_string(open);
	buf.read_int32(x);
	buf.read_int32(y);
	return 0;
}


Tasker_Detail::Tasker_Detail(void) {
	reset();
}

Tasker_Detail::~Tasker_Detail(void) {

}

int Tasker_Detail::serialize(Block_Buffer &buffer) const {
	return 0;
}

int Tasker_Detail::deserialize(Block_Buffer &buffer) {
//	buffer.read_int64(role_id);
//
//	uint16_t task_count = 0;
//	buffer.read_uint16(task_count);
//
//	for (int i = 0; i < task_count; ++i) {
//		TaskInfo *task_info = POOL_MANAGER->pop_task_pool();
//		buffer.read_int32(task_info->__task_id);
//        if (this->task_map_.insert(TaskMap::value_type(task_info->__task_id, task_info)).second == false)
//        {
//        	POOL_MANAGER->push_task_pool(task_info);
//            MSG_TRACE_ABORT("error pop_task_pool: %d", task_info->__task_id);
//        }
//
//		buffer.read_int32(task_info->__game_type);
//		int64_t sec = 0;
//		int64_t usec = 0;
//		buffer.read_int64(sec);
//		buffer.read_int64(usec);
//		task_info->__accept_tick(sec, usec);
//		buffer.read_int64(sec);
//		buffer.read_int64(usec);
//		task_info->__refresh_tick(sec, usec);
//		buffer.read_int64(sec);
//		buffer.read_int64(usec);
//		task_info->__refresh_tick(sec, usec);
//		buffer.read_int32(task_info->__prev_task);
//		buffer.read_int32(task_info->__post_task);
//
//		uint64_t status_flag = 0,tmp_flag = 0;
//		buffer.read_uint64(status_flag);
//		int uint64_len = sizeof(uint64_t) * 8;
//		for (int j = 0; j < TaskInfo::TASK_STATUS_END && j < uint64_len; ++j)
//		{
//			tmp_flag = 1 << j;
//			if ((status_flag & tmp_flag) == 1)
//				task_info->__status.set(j);
//		}
//		uint64_t logic_type_flag = 0;
//		buffer.read_uint64(logic_type_flag);
//		for (int j = 0; j < TaskCondition::TASK_CT_END && j < uint64_len; ++j)
//		{
//			tmp_flag = 1 << j;
//			if ((logic_type_flag & tmp_flag) == 1)
//				task_info->__logic_type.set(j);
//		}
//
//		uint16_t cond_list_size = 0;
//		buffer.read_uint16(cond_list_size);
//        for (int j = 0; j < cond_list_size; ++j)
//        {
//            TaskCondition *task_cond = POOL_MANAGER->pop_task_condition_pool();
//            buffer.read_int32(task_cond->__type);
//            buffer.read_int32(task_cond->__current_value);
//            task_info->__condition_list.push_back(task_cond);
//        }
//
//        task_info->__task_imp = this->pop_task_imp(task_info->__game_type);
//
//        if (task_info->is_logic_level_up())
//            this->task_accepted_lvl_set().insert(task_info);
//        if (task_info->is_logic_kill_monster())
//            this->task_accepted_monster_set().insert(task_info);
//        if (task_info->is_logic_collect_item())
//            this->task_accepted_collect_set().insert(task_info);
//        if (task_info->is_logic_attend())
//            this->task_accepted_attend_set().insert(task_info);
//        if (task_info->is_logic_package())
//        {
//        	this->init_task_listen_item(task_info);
//            this->task_accepted_package_set().insert(task_info);
//        }
//    }
//
//    int submited_task_size = request->submited_task_size();
//    for (int i = 0; i < submited_task_size; ++i)
//    {
//        this->task_player()->task_submited_once_set().insert(request->submited_task(i));
//    }

	return 0;
}

void Tasker_Detail::reset(void) {
	role_id = 0;
	task_map_.clear();
	task_submited_once_.clear();
	npc_id_.clear();
	npc_type_map_.clear();
	is_ever_open_reward_task_list = false;
	is_close_auto_refresh = false;
	refresh_reward_task_num = 0;
    is_open_countdown = true;
    begin_countdown_time.set(Time_Value::gettimeofday());
    used_free_refresh_count = 0;
    used_accept_count = 0;
    give_up_num = 0;
    reward_task_map.clear();
    quality_RewardTaskIdSet_map.clear();
    task_icon_hero_id_Map.clear();
    task_gift_level_set.clear();
    ring_num = 0;
    is_diamond_recover = false;
    ring_type_RingTaskIdSet_map.clear();
    target_player_id = 0;
    task_id = 0;
    scene_id_set.clear();
    did_ring_num = 0;
    did_reward_task_num = 0;
    svr_scene_npc_set.clear();
    ring_task_id  = 0;
    is_exist_cross_day = false;
    did_reward_task_num = 0;
    did_reward_task_num_today = 0;
    is_submit_close_ring_task = false;
}

int Tasker_Detail::load(void) {
	return MONGO_OPERATOR->load_tasker_detail(*this);
}

int Tasker_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_tasker_detail(*this);
	else
		return 0;
}

int Tasker_Detail::bind_task(const int task_id, TaskInfo &info)
{
	if(info.is_reward_task()) {
		return bind_reward_task(task_id, info);
	}

    TaskMap::iterator iter = this->task_map_.find(task_id);
    if (iter != this->task_map_.end())
        return -1;

    if (this->task_map_.insert(std::make_pair(task_id, info)).second == true)
        return 0;
    return -1;
}

int Tasker_Detail::bind_reward_task(int task_id, TaskInfo &info) {
	RewardTaskMap::iterator it = this->reward_task_map.find(task_id);
	if(it != this->reward_task_map.end()) {
		return -1;
	}

	if(this->reward_task_map.insert(std::make_pair(task_id, info)).second == true) {
		return 0;
	}
	return -1;
}

