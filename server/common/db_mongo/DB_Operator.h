// -*- C++ -*-
/*
 * DB_Operator.h
 *
 *  Created on: May 3, 2012
 *      Author: Ti
 */

#ifndef DB_OPERATOR_H_
#define DB_OPERATOR_H_

#include "Object_Pool.h"
#include "Game_Typedef.h"

namespace mongo {
	class DBClientConnection;
	class BSONObj;
	class BSONObjBuilder;
}

using namespace mongo;

struct DB_View;
struct Base_Detail;
struct Fighter_Detail;
struct Prop_Value;
struct Prop_Base;
struct Example_Detail;
struct Icon_Detail;
struct Public_Detail;
struct Operating_Detail;
struct Equip_Addon_Detail;
struct Rune_Stone_Addon_Detail;
struct Item_Detail;
struct Packer_Detail;
struct Item_User_Detail;
struct Tasker_Detail;
struct Mail_Detail;
struct Friend_Detail;
struct Sgl_Dungeon_Detail;
struct Heroer_Detail;
struct Rune_Detail;
struct Rune_Stone_Detail;
struct Goods_Info;
struct Teamer_Detail;
struct Mall_Promo_Info;
struct Mail_Send_Info;
struct Gang_DB_View;
struct Ganger_Detail;
struct Mail_Data;
struct Alchemy_Detail;
struct Arenaer_Detail;
struct Guide_Detail;
struct TaskInfo;
struct Dragon_Vale_Detail;
struct Achievement_Detail;
struct Title_Detail;
struct Equiper_Detail;
struct Mount_Detail;
struct Wing_Detail;
struct Invest_Plan_Detail;
struct Invest_Plan_History_Record;
struct ThorLottery_Manager_Detail;
struct ThorLottery_Detail;
struct Card_Detail;
struct Life_Skill_Detail;
struct Pet_Detail;

typedef std::list<Invest_Plan_History_Record> Invest_Plan_History_Record_List;
typedef std::vector<Item_Detail> Item_Vec;
typedef std::vector<Goods_Info> Goods_Info_Vec;
typedef std::vector<Mall_Promo_Info> Promo_Goods_Info_Vec;
typedef std::vector<Mail_Data> Sys_Mail_Vec;
struct Chat_Content;
struct Chat_Record;
struct Gang_Chat_Record;
typedef boost::unordered_map<int64_t, Gang_Chat_Record> Gang_ID_Chat_Record_Map;
struct Team_Sports_Ranking_Detail;
typedef boost::unordered_map<role_id_t, Team_Sports_Ranking_Detail> RoleId_TeamSportsRankingDetail_Map;
struct Team_Sports_Ranking_Detail;
typedef std::vector<Team_Sports_Ranking_Detail*> Team_Sports_Ranking_Info;


struct Ranking_Info;
struct Rank_Role_Info;
struct Rank_Item_All_Info;
typedef std::list<Ranking_Info> Ranking_Sort_List;
struct Ranking;
typedef boost::unordered_map<int, Ranking> Ranking_Map;
struct Arena_Reward_Info;
struct Arena_Season_Detail;
struct Logic_Status_Detail;
struct Title_Record_Detail;

struct Helper_Detail;
struct Covenant_Detail;
struct Vip_Detail;
struct Reward_Detail;
struct Activation_Code_Info;
struct Player_DB_View;
struct Gang_War_Info;
struct Campfire_Detail;
struct World_Boss_Detail;
struct FB_Info_Detail;
struct Active_Content_Detail;
struct Foot_Info;
typedef std::multimap<int64_t, Foot_Info> Foot_Info_Map;
typedef boost::unordered_set<int64_t> Foot_Id_Set;
typedef boost::unordered_map<int64_t, Campfire_Detail> Campfire_Detail_Map;
typedef boost::unordered_map<int64_t, FB_Info_Detail> FB_Info_Detail_Map;

struct Item_Basic_Info;
struct Item_Equip_Info;
struct Rune_Part_Info;
struct Rank_Heroer_Info;
struct Fashion_Detail;
struct Item_Rune_Stone_Info;
struct Card_Addon_Detail;

struct GM_Ann_Info;
typedef boost::unordered_map<int32_t, GM_Ann_Info> GM_Ann_Info_Map;

typedef std::multiset<role_id_t> Role_Id_MSet;
typedef boost::unordered_map<int32_t, Role_Id_MSet> Item_Role_Set_Map;
typedef boost::unordered_map<int32_t, Item_Role_Set_Map> Tabs_Item_Role_Set_Map;

struct Auction_Goods_Data;
typedef boost::unordered_map<uint32_t, Auction_Goods_Data> Aution_Goods_Data_Map;

struct First_Lvl_Detail;

struct Knight_Trials_Detail;
struct Knight_Trials_Player_Detail;
struct Anci_Artifact_Detail;
struct Answer_All_Data_Info;
struct Questionnaire_Answer_Info;
struct Master_Part_Info;
struct Active_Content_Info;

struct MSG_20999999;
struct Chat_Control_Info;

struct Activity_Detail;
struct Knight_Trials_Player;
struct Knight_Trials_Role_Info;
typedef std::vector<Knight_Trials_Role_Info> Knight_Trials_Role_Info_Vec;
struct Account_Key;
struct DB_Active_Data_Info;
typedef boost::unordered_map<uint32_t, DB_Active_Data_Info> DB_Active_Data_Info_Map;
struct Two_Recharge_Info;
struct R2_Invite_Detail;
struct Activities_Stores_Detail;
struct Activity_Exchange_Mall_Goods_Info;
typedef boost::unordered_map<uint32_t, Activity_Exchange_Mall_Goods_Info> Activity_Exchange_Mall_Goods_Info_Map;
struct Active_Data_Stauts;
typedef boost::unordered_map<uint32_t, Active_Data_Stauts> Active_Data_Map;

struct Oper_Activity_Detail;

// expedition --
struct Exchange_Detail;
struct Expedition_Detail;
struct Expedition_Manager_Detail;
struct Expedition_Global_Manager_Data;
struct Expedition_Manager_Data;
typedef boost::unordered_map<uint32_t, Expedition_Manager_Data> Inter_ExpeditionManagerData_Map;
// -- expedition

class DB_Operator {
public:
	typedef boost::unordered_map<int64_t, DBClientConnection *> Connection_Map;

	DB_Operator(void);
	~DB_Operator(void);
	DB_Operator(const DB_Operator &);
	const DB_Operator &operator=(const DB_Operator &);

	mongo::DBClientConnection &connection(void);
	void set_inner_conn(DBClientConnection *inner_conn);		// 合服时方便

	void conn_check(void);

	int init(void);
	int ensure_all_index(void);
	int mongo_update_on_start(void);
	int update_db_cross_info(bool time_out = false);
	int change_solution_id(int solution_id);
	int drop_mmo_database(void);


	int db_role_size(void);
	int load_db_view(DB_View *db_view, bool all_info = true);
	int robot_db_view(std::vector<Account_Key> &account_vec);

	int has_role_by_id(role_id_t role_id);

	int load_base_detail(Base_Detail &detail);
	int save_base_detail(Base_Detail &detail);
	int reset_online_state(void);

	int load_base_arena_detail(Base_Detail &detail);

	int load_fighter_detail(Fighter_Detail &detail);
	int save_fighter_detail(Fighter_Detail &detail, bool save_arena = true);

	int save_fighter_attr(BSONObjBuilder &builder, Fighter_Detail &detail);
	int save_fighter_prop(BSONObjBuilder &builder, std::stringstream& str, Prop_Value& prop);
	int save_fighter_prop_value(BSONObjBuilder &builder, const std::string& name, Prop_Base& prop);

	//public
	int load_public_detail(Public_Detail &detail);
	int save_public_detail(Public_Detail &detail);

	int load_example_detail(Example_Detail &detail);
	int save_example_detail(Example_Detail &detail);

	//gang
	int load_gang_db_view();
	int save_gang_db_view(const Gang_DB_View& gang_view);
	int load_gang_war_info(Gang_War_Info& gang_war_info);
	int save_gang_war_info(const Gang_War_Info &gang_war_info);

	//icon
	int load_icon_detail(Icon_Detail &detail);
	int save_icon_detail(Icon_Detail &detail);

	int load_operating_detail(Operating_Detail &detail);
	int save_operating_offline_mail(role_id_t role_id, Mail_Send_Info &mail, DBClientConnection *dest_conn = 0);
	int save_operating_team_award(role_id_t role_id, int level_id);
	int save_operating_offline_friend_add(role_id_t role_id, role_id_t other_role_id, int64_t addtime);
	int save_operating_offline_apply_handel(role_id_t role_id, role_id_t other_role_id, int8_t handel);
	int save_operating_offline_ganger_info(role_id_t role_id, Ganger_Detail& detail);
	int save_operating_offline_arena_three_day_reward(role_id_t role_id, Arena_Reward_Info &info);
	int save_operating_offline_arena_seven_day_reward(role_id_t role_id, Arena_Reward_Info &info);
	int save_operating_offline_receive_flowers(role_id_t role_id, std::string &role_name, uint32_t item_id,  uint32_t charm, uint32_t friend_num, std::string &msg,  role_id_t receive_role_id);
	int save_operating_offline_dragon_vale_building(role_id_t role_id, uint8_t notice_type, uint8_t gold_mine,
											uint8_t moon_well, uint8_t totem);
	int save_operating_offline_dragon_vale_rob_fight_result(role_id_t role_id, uint8_t notice_type, std::string server_name,
											std::string player_name, uint8_t result);
	int save_operating_offline_dragon_vale_gain_rob_production(role_id_t role_id, std::string server_name,
											std::string player_name, uint32_t gold_mine, uint32_t moon_well,
											uint32_t totem);
	int save_operating_offline_dragon_vale_special(role_id_t role_id, bool is_specail);
	int save_operating_offline_invite_player(role_id_t role_id, int64_t time);
	int save_operating_offline_like_success(role_id_t role_id);
	int save_operating_offline_active_content_listen_world_boss_end_info(role_id_t role_id , uint32_t damage, uint32_t rank, uint32_t damage_rate);

	int save_chat_record(role_id_t role_id, int64_t target, uint8_t chat_type, std::vector<Chat_Content>& contents);
	int load_offline_private_chat_record(role_id_t role_id, Time_Value& time, std::vector<Chat_Record>& chat_record);
	int load_private_chat_record(role_id_t role_id, role_id_t target, std::vector<Chat_Record>& chat_record);
	int save_gang_chat_record(Gang_ID_Chat_Record_Map& gang_chat_record_map);
	int load_gang_chat_record(Gang_ID_Chat_Record_Map& gang_chat_record_map);


	//加载系统邮件
	int load_mail_detail(Mail_Detail &detail);
	int load_sys_mail_list(Sys_Mail_Vec &sys_mail_);
	int save_mail_detail(Mail_Detail &detail);
	int save_sys_mail_list(Mail_Data &mail_info_);

	//炼金
	int save_alchemy_detail(Alchemy_Detail &detail);
	int load_alchemy_detail(Alchemy_Detail &detail);

	int load_friend_detail(Friend_Detail &detail);
	int save_friend_detail(Friend_Detail &detail);

	int load_packer_detail(Packer_Detail &detail);
	int save_packer_detail(Packer_Detail &detail);

	int load_item_user_detail(Item_User_Detail &detail);
	int save_item_user_detail(Item_User_Detail &detail);

	//task
	int load_tasker_detail(Tasker_Detail &detail);
	int save_tasker_detail(Tasker_Detail &detail);
	void save_one_task_of_tasker_detail(std::vector<int> &status_vc,
			std::vector<int> &logic_type_vc,
			std::vector<BSONObj> &task_vc, std::vector<BSONObj> &cond_vc,
			const TaskInfo &task_info);

	int load_sgl_dungeon_detail(Sgl_Dungeon_Detail &detail);
	int save_sgl_dungeon_detail(Sgl_Dungeon_Detail &detail);

	int load_heroer_detail(Heroer_Detail &detail);
	int save_heroer_detail(Heroer_Detail &detail, bool save_arena = true);
	int load_fight_hero_detail(Heroer_Detail &detail);
	int load_heroer_status_detail(Heroer_Detail &detail);
	int save_heroer_status_detail(Heroer_Detail &detail);

	int load_teamer_detail(Teamer_Detail &detail);
	int save_teamer_detail(Teamer_Detail &detail);

	int load_ganger_detail(Ganger_Detail &detail);
	int save_ganger_detail(Ganger_Detail &detail);

	int load_rune_detail(Rune_Detail &detail);
	int save_rune_detail(const Rune_Detail &detail);

	int load_rune_stone_detail(Rune_Stone_Detail &detail);
	int save_rune_stone_detail(const Rune_Stone_Detail &detail);

	int load_status_detail(Logic_Status_Detail &detail);
	int save_status_detail(const Logic_Status_Detail &detail);

	//dragon vale
	int load_dragon_vale_detail(Dragon_Vale_Detail &detail);
	int save_dragon_vale_detail(Dragon_Vale_Detail &detail);
	int save_dragon_vale_is_special(int64_t role_id, bool is_speical);
	int save_dragon_vale_reset(int64_t role_id, uint32_t matched_num, uint8_t star_num, uint8_t award_foot_num,
			const Foot_Info_Map &foot_info_map, const Foot_Id_Set &foot_id_set, uint8_t dragon_heart_num,
			const uint32_t play_dragon_num, const uint32_t drive_thief_num);

	int load_achievement_detail(Achievement_Detail &detail);
	int save_achievement_detail(Achievement_Detail &detail);

	int load_title_detail(Title_Detail &detail);
	int save_title_detail(Title_Detail &detail);

	int load_equiper_detail(Equiper_Detail &detail);
	int save_equiper_detail(Equiper_Detail &detail);

	int load_title_record(Title_Record_Detail &detail);
	int save_title_record(Title_Record_Detail &detail);

	int load_exchange_detail(Exchange_Detail &detail);
	int save_exchange_detail(Exchange_Detail &detail);

	////////////////////////////////////////////////////////////////////

	//读取封禁帐号
	void load_forbid_list(String_Time_Map &account_list, Role_Id_Time_Map &role_id_list);
	//读取封禁IP
	void load_forbid_ip_list(String_Time_Map &ip_list);
	//读取商城物品列表
	void load_item_sales_num(Item_Role_Set_Map &item_sales_num, Tabs_Item_Role_Set_Map& tabs_item_sales_num);
	void save_item_sales_num(Item_Role_Set_Map &item_sales_num, Tabs_Item_Role_Set_Map& tabs_item_sales_num);
	void load_goods_list(Goods_Info_Vec &goods_list_);
	void load_promo_goods_list(Promo_Goods_Info_Vec &promo_goods_list_);
	void load_banner_goods_list(Promo_Goods_Info_Vec &banner_goods_list_);

	////////////////////////////////////////////////////////////////////
	//arena
	int load_arena_fighter_detail(Fighter_Detail &detail);

	int load_arenaer_detail(Arenaer_Detail &detail);
	int save_arenaer_detail(const Arenaer_Detail &detail);

	int load_arena_season_detail(Arena_Season_Detail &detail);
	int save_arena_season_detail(const Arena_Season_Detail &detail);
	int clear_arena_player_data(void);
	int save_arena_king_time(const role_id_t role_id, const int king_time);
	int save_arena_be_worship(const role_id_t role_id, const int be_worship);

	int load_mount_detail(Mount_Detail &detail);
	int save_mount_detail(const Mount_Detail &detail);

	int update_arenaer_fight(Arenaer_Detail* detail);
	int load_all_arena_player(bool reset = false);
	int init_all_arena_player(void);
	
	// ranking start
	int load_ranking(Ranking_Map& rank_);
	int save_ranking(Ranking_Map& rank_);
	int save_ranking_equipment_data(role_id_t role_id, int item_id, int item_index, std::string role_name, std::string item_name, int score, Item_Equip_Info &item_info, Item_Basic_Info &item_base);
	int del_ranking_equipment_data(role_id_t role_id, int item_id, int item_index);

	int refresh_ranking(Ranking& rank_, int32_t type, uint32_t num);
	int get_rank_role_info(Rank_Role_Info &role_info, role_id_t role_id);
	int get_rank_heroer_info(Rank_Heroer_Info &heroer_info, role_id_t role_id, int32_t heroer_id, int32_t heroer_index);
	int get_rank_item_all_info(Rank_Item_All_Info &item_info, const mongo::BSONObj &obj, role_id_t role_id);

	int save_ranking_item_base(Item_Basic_Info &item_base, mongo::BSONObj &obj);
	int save_ranking_item_equip(Item_Equip_Info &item_info, mongo::BSONObj &obj);
	int load_ranking_item_base(Item_Basic_Info &item_base, mongo::BSONObj &obj);
	int load_ranking_item_equip(Item_Equip_Info &item_info, mongo::BSONObj &obj);
	int save_ranking_rune_part(Rune_Part_Info &rune_info, mongo::BSONObj &obj);
	int load_ranking_rune_part(Rune_Part_Info &rune_info, mongo::BSONObj &obj);
	int save_ranking_rune_stone(Item_Rune_Stone_Info &rune_stone_info, mongo::BSONObj &obj);
	int load_ranking_rune_stone(Item_Rune_Stone_Info &rune_stone_info, mongo::BSONObj &obj);
	int save_ranking_master_part(Master_Part_Info &master_part_info, mongo::BSONObj &obj);
	int load_ranking_master_part(Master_Part_Info &master_part_info, mongo::BSONObj &obj);
	// ranking end
	
	// campfire start
	int load_campfire(Campfire_Detail_Map& campfire_info);
	int save_campfire_detail(const Campfire_Detail& detail);
	// campfire end

	// world boss start
	int load_world_boss_detail(World_Boss_Detail& detail);
	int save_world_boss_detail(const World_Boss_Detail& detail);
	// world boss end

	// fb_info
	int load_fb_info(FB_Info_Detail_Map& fb_info_map);
	int save_fb_info_detail(const FB_Info_Detail& detail);

	//daily start
	int save_daily_detail(Helper_Detail &detail);
	int load_daily_detail(Helper_Detail &detail);
	//daily end

	//team_sports_start
	int load_team_role(RoleId_TeamSportsRankingDetail_Map& rank_map_, Team_Sports_Ranking_Info &rank_,
			Time_Value &team_sports_start_time_, First_Lvl_Detail &first_lvl, int &game_times);
	int save_team_role(RoleId_TeamSportsRankingDetail_Map& rank_,
			Time_Value &team_sports_start_time_,
			First_Lvl_Detail &first_lvl, int game_times);
	//team_sports_end

	//covenant start
	int load_covenant_detail(Covenant_Detail &detail);
	int save_covenant_detail(Covenant_Detail &detail);
	//covenant end

	//vip start
	int load_vip_detail(Vip_Detail &detail);
	int save_vip_detail(Vip_Detail &detail);
	//vip end

	//fashion start
	int load_fashion_detail(Fashion_Detail &detail);
	int save_fashion_detail(Fashion_Detail &detail);
	//fashion end

	//reward start
	int save_reward_detail(Reward_Detail &detail);
	int load_reward_detail(Reward_Detail &detail);
//	int query_activation_code_info(Activation_Code_Info &code_info, const std::string &code);
//	int update_activation_code_info(const std::string &code);
	int load_all_activatiion_code_key(String_Set &keys);
	int load_all_activation_receive_id(Long_Id_Set &ids);
	int save_activation_code_receive_id(long_id_t id);
	//reward end
	int save_access_count(UInt_UInt_Map &access_count);

	//auction start
	int load_action_goods_data(Aution_Goods_Data_Map &goods_data_map, long long int &max_id);
	int save_action_goods_data(Auction_Goods_Data &goods_data);
	int delete_action_goods_data(uint32_t id);
	//auction end

	//active start
	int save_active_content_detail(Active_Content_Detail &detail);
	int load_active_content_detail(Active_Content_Detail &detail);
	int save_active_content(Active_Content_Info &c);
	int load_active_content(Active_Content_Info &c);
	int load_active_db_event(DB_Active_Data_Info_Map &event);
	//active end
	int save_proposal(role_id_t role_id, uint8_t type, std::string &contents, int64_t time);

	int load_ann_info(GM_Ann_Info_Map &ann_info_map);
	int load_chat_control_info(Chat_Control_Info &info);

	//knight_trials start
	int save_knight_trials_detail(const Knight_Trials_Detail &detail);
	int load_knight_trials_detail(Knight_Trials_Detail &detail);
//	int save_knight_trials_player_detail(Knight_Trials_Player_Detail &detail);
	int load_knight_trials_player_detail(Knight_Trials_Player_Detail &detail);
	int push_cur_knight_trials_player(role_id_t role_id, Knight_Trials_Player &player, bool clear);
	int push_hide_knight_trials_player(role_id_t role_id, Knight_Trials_Player &player, bool clear);
	int find_cur_kinght_trials_player(role_id_t role_id, role_id_t player_id, Knight_Trials_Player &player);
	int reset_kinght_trials_player(role_id_t role_id);
	void insert_knight_trials_player_role_info_vec(Knight_Trials_Role_Info_Vec &vec, Knight_Trials_Role_Info &player);
		/*save*/
	int save_knight_trials_base_detail(Base_Detail &detail, mongo::BSONObj &obj);
	int save_knight_trials_fighter_detail(Fighter_Detail &detail, mongo::BSONObj &obj);
	int save_knight_trials_heroer_detail(Heroer_Detail &detail, mongo::BSONObj &obj);
		/*load*/
	int load_knight_trials_base_detail(Base_Detail &detail, mongo::BSONObj &obj);
	int load_knight_trials_fighter_detail(Fighter_Detail &detail, mongo::BSONObj &obj);
	int load_knight_trials_heroer_detail(Heroer_Detail &detail, mongo::BSONObj &obj);
	//knight_trials end

	// 上古神器
	int load_anci_artifact_detail(Anci_Artifact_Detail &detail);
	int save_anci_artifact_detail(Anci_Artifact_Detail &detail);

	//翅膀
	int load_wing_detail(Wing_Detail &detail);
	int save_wing_detail(Wing_Detail &detail);

	//invest plan
	int load_invest_plan_detail(Invest_Plan_Detail &detail);
	int save_invest_plan_detail(Invest_Plan_Detail &detail);

	int save_invest_plan_history_record(const Invest_Plan_History_Record_List &invest_plan_history_list);
	int load_invest_plan_history_record(Invest_Plan_History_Record_List &invest_plan_history_list);

	//thorlottery
	int load_thorlottery_manager(ThorLottery_Manager_Detail &detail);
	int save_thorlottery_manager(const ThorLottery_Manager_Detail &detail);
	int load_thorlottery(ThorLottery_Detail &detail);
	int save_thorlottery(const ThorLottery_Detail &detail);

	//answer
	int save_answer_info_detail(const Answer_All_Data_Info &answer_info);
	int load_answer_info_detail(Answer_All_Data_Info &answer_info);

	//activity
	int load_activity_detail(Activity_Detail &detail);
	int save_activity_detail(Activity_Detail &detail);

	// expedition
	int load_expedition_detail(Expedition_Detail &detail);
	int save_expedition_detail(const Expedition_Detail &detail);

	int load_expedition_manager_detail(Expedition_Manager_Detail &detail);
	int save_expedition_manager_detail(const Expedition_Manager_Detail &detail);

	int load_expedition_global_manager_data(Expedition_Global_Manager_Data &data);
	int save_expedition_global_manager_data(const Expedition_Global_Manager_Data &data);

	int load_expedition_manager_data(Inter_ExpeditionManagerData_Map &map);
	int save_expedition_manager_data(const Expedition_Manager_Data &data);

	int save_questionnaire_info( role_id_t role_id, std::vector<Questionnaire_Answer_Info> &ans_info);

	int save_two_recharge_info(Two_Recharge_Info &info);
	int load_two_recharge_info(Two_Recharge_Info &info);

	int save_r2_invite_detail(const R2_Invite_Detail &detail);
	int load_r2_invite_detail(R2_Invite_Detail &detail);

	int save_activities_stores_detail(const Activities_Stores_Detail &detail);
	int load_activities_stores_detail(Activities_Stores_Detail &detail);
	int load_activities_stores_goods(Activity_Exchange_Mall_Goods_Info_Map &goods_vec);

	int save_active_data(Active_Data_Map &data_map);
	int load_active_data(Active_Data_Map &data_map);

	// life_skill
	int save_life_skill_data(const Life_Skill_Detail &detail);
	int load_life_skill_data(Life_Skill_Detail &detail);

	//oper activity
	int save_oper_activity_detail(const Oper_Activity_Detail &detail);
	int load_oper_activity_detail(Oper_Activity_Detail &detail);

	//center server
	int save_inner_check_info(MSG_20999999 msg, std::string &ip);
	int save_inner_check_info(Block_Buffer &buf);

	// pet
	int save_pet_data(const Pet_Detail &detail);
	int load_pet_data(Pet_Detail &detail);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 停服更新数据处理
	void package_add_copper(role_id_t role_id, int value);		// 加金币
	void package_add_bind_gold(role_id_t role_id, int value);	// 加粉钻
	void db_repair_20141219_handle_fix_wing_data();
	// 37玩坐骑补偿
	void db_repair_20150112_handle_mount_compensation(void);
	void db_repair_20141220_handle_fix_invest_data();
	void db_repair_20150112_handle_handle_fix_wing_state_data();

	void db_repair_20150113_handle_37wan_upgrade_invest_plan_data();
	//海外清空拍卖行
	void db_repair_20150114_handle_r2_auction(void);
	//37活跃度
	void db_repair_20150114_handle_37_helper_daily(void);
	// 补偿原符文系统
	void db_repair_20150122_hanle_repay_rune(void);
	// 修正装备卡牌数据
	void db_repair_20150205_handle_repay_equip_card_info(void);
	// 修正上古神奇装备强化
	void db_repair_20150202_handle_anci_artifact(void);
	// 英雄之门等级补偿
	void db_repair_20150213_handle_elite_dungeon_reimburse(void);
//	// 远征：逻辑服修复数据
//	void db_repair_20150311_handle_expedition_repair(void);
	// 更新：英雄及功勋更新
	void db_repair_20150321_handle_hero_and_feats(void);
	void db_repair_20150328_handle_hero_and_feats(void);
	// 更新：时装补偿
	void db_repair_20150321_handle_fashion(void);
	// 技能补偿
	void db_repair_20150321_handle_skill(void);

private:

	int handle_save_fighter_detail(const Fighter_Detail &detail, mongo::BSONObj &obj);
	int handle_load_fighter_detail(Fighter_Detail &detail, const mongo::BSONObj &obj);

	int load_item_detail(const mongo::BSONObj &obj, Item_Detail &item);
	int save_item_detail(const Item_Detail &item, mongo::BSONObj &obj, const Packer_Detail& packer_detail);

	int load_item_equip_detail(const mongo::BSONObj &obj, Equip_Addon_Detail &equip);
	int save_item_equip_detail(const Equip_Addon_Detail &equip, mongo::BSONObj &obj, const Packer_Detail& packer_detail);

	int load_item_rune_stone_detail(const mongo::BSONObj &obj, Rune_Stone_Addon_Detail &rune_stone);
	int save_item_rune_stone_detail(const Rune_Stone_Addon_Detail &rune_stone, mongo::BSONObj &obj);

	int load_item_card_detail(const mongo::BSONObj &obj, Card_Addon_Detail &card);
	int save_item_card_detail(const Card_Addon_Detail &card, mongo::BSONObj &obj);

private:
	static DB_Operator *instance_;
	Connection_Map connection_map_;
	Connection_Map center_conn_map_;
	MUTEX connection_map_lock_;
	int last_cross_inter_;

	DBClientConnection *inner_conn_;

};
typedef Singleton<DB_Operator> DB_Operator_Single;
#define MONGO_OPERATOR    (DB_Operator_Single::instance())
#define MONGO_CONNECTION		MONGO_OPERATOR->connection()

#endif /* DB_OPERATOR_H_ */
