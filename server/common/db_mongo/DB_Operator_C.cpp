/*
 * DB_Operator_D.cpp
 *
 *  Created on: 2014年9月11日
 *      Author: Linqiyou
 */

#include "Logic_Monitor.h"
#include "DB_Operator.h"
#include "DB_Fields.h"
#include "Configurator.h"
#include "Config_Cache_Task.h"
#include "Record_Client.h"
#include "Pool_Manager.h"
#include "Server_Struct.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "mongo/client/dbclient.h"
#include "Msg_Struct.h"
#include "team/Team_Def.h"
#include "campfire/Campfire_Struct.h"
#include "campfire/Scene_Campfire_Struct.h"
#include "world_boss/World_Boss_Struct.h"
#include "DB_Def.h"
#include "equiper/Equiper_Def.h"
#include "gm_ann/GM_Ann_Def.h"
#include "gm_ann/GM_Ann_Struct.h"
#include "auction/Auction_Struct.h"
#include "team/Team_Struct.h"
#include "anci_artifact/Anci_Artifact_Struct.h"
#include "wing/Wing_Struct.h"
#include "invest_plan/Invest_Plan_Def.h"
#include "thorlottery/ThorLottery_Struct.h"
#include "active/Active_Content_Struct.h"
#include "chat_control/Chat_Control_Manager.h"
#include "active/Active_Struct.h"

using namespace mongo;

#include "DB_Manager.h"

int DB_Operator::load_daily_detail(Helper_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Daily_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (! res.isEmpty()) {
		detail.role_id = res[Role_Fields::ROLE_ID].numberLong();
		//daily
		BSONObj daily_map = res.getObjectField(Daily_Fields::DAILY_LIST.c_str());
		BSONObjIterator iter_daily(daily_map);
		detail.daily_num_map_.clear();
		while (iter_daily.more()) {
			BSONObj obj = iter_daily.next().embeddedObject();

			uint32_t num = obj[Daily_Fields::Daily_Info::NUM].numberInt();
			uint32_t id = obj[Daily_Fields::Daily_Info::ID].numberInt();
			if(id) {
				detail.daily_num_map_.insert(std::make_pair(id, num));
			}
		}
		//activity
		BSONObj activity_map = res.getObjectField(Daily_Fields::ACTIVITY_LIST.c_str());
		BSONObjIterator iter_activity(activity_map);
		detail.activity_num_map_.clear();
		while (iter_activity.more()) {
			BSONObj obj = iter_activity.next().embeddedObject();

			uint32_t num = obj[Daily_Fields::Activity_Info::NUM].numberInt();
			uint32_t id = obj[Daily_Fields::Activity_Info::ID].numberInt();
			if(id) {
				detail.activity_num_map_.insert(std::make_pair(id, num));
			}
		}

		uint32_t rec_id = 0;
		//今日领取的物品
		{
			detail.today_award_receive.clear();
			BSONObjIterator iter(res.getObjectField(Daily_Fields::TODAY_RECEIVE.c_str()));
			while (iter.more())	{
				rec_id = iter.next().numberInt();
				detail.today_award_receive.insert(rec_id);
			}
		}
		//累计活跃度领取的物品
		{
			detail.all_award_receive.clear();
			BSONObjIterator iter(res.getObjectField(Daily_Fields::ALL_RECEIVE.c_str()));
			while (iter.more())	{
				rec_id = iter.next().numberInt();
				detail.all_award_receive.push_back(rec_id);
			}
		}

		detail.today_activity = res[Daily_Fields::TODAY_ACTIVITY].numberInt();
		detail.all_activity = res[Daily_Fields::ALL_ACTIVITY].numberInt();
		detail.last_join_arena_force = res[Daily_Fields::LAST_ARENA_FORCE].numberInt();
		detail.last_tip_force = res[Daily_Fields::LAST_TIP_FORCE].numberInt();
		detail.receive_vip_activity = res[Daily_Fields::RECEIVE_VIP_ACTIVITY].numberInt();
		detail.no_join_areana_tip = res[Daily_Fields::NJOIN_ARENA_TIP].booleanSafe();
		detail.check_in_day = res[Daily_Fields::CHECK_IN_DAY].numberInt();
		detail.last_check_in_refresh_time.sec(res[Daily_Fields::LAST_CHECK_IN_REFRESH_TIME].numberLong());
		detail.today_is_check_in = res[Daily_Fields::TODAY_IS_CHECK_IN].numberInt();
	}
	return 0;
}

int DB_Operator::save_daily_detail(Helper_Detail &detail) {
	vector<BSONObj> daily_list;
	for(UInt_UInt_Map::iterator iter = detail.daily_num_map_.begin(); iter != detail.daily_num_map_.end(); ++iter) {
		BSONObjBuilder daily_builder;
		daily_builder << Daily_Fields::Daily_Info::ID << iter->first
			    << Daily_Fields::Daily_Info::NUM << iter->second;
		daily_list.push_back(daily_builder.obj());
	}

	vector<BSONObj> activity_list;
	for(UInt_UInt_Map::iterator iter = detail.activity_num_map_.begin(); iter != detail.activity_num_map_.end(); ++iter) {
		BSONObjBuilder activity_builder;
		activity_builder << Daily_Fields::Activity_Info::ID << iter->first
			    << Daily_Fields::Activity_Info::NUM << iter->second;
		activity_list.push_back(activity_builder.obj());
	}

	//today_award_receive
	UInt_Vec today_award_receive;
	today_award_receive.clear();
	for(UInt_Set::iterator it = detail.today_award_receive.begin(); it != detail.today_award_receive.end(); ++it) {
		today_award_receive.push_back(*it);
	}

	BSONObjBuilder builder;
	builder << Daily_Fields::DAILY_LIST << daily_list
			<< Daily_Fields::ACTIVITY_LIST << activity_list
			<< Daily_Fields::TODAY_ACTIVITY << detail.today_activity
			<< Daily_Fields::ALL_ACTIVITY << detail.all_activity
			<< Daily_Fields::TODAY_RECEIVE << today_award_receive
			<< Daily_Fields::ALL_RECEIVE << detail.all_award_receive
			<< Daily_Fields::LAST_ARENA_FORCE << detail.last_join_arena_force
			<< Daily_Fields::NJOIN_ARENA_TIP << detail.no_join_areana_tip
			<< Daily_Fields::LAST_TIP_FORCE << detail.last_tip_force
			<< Daily_Fields::RECEIVE_VIP_ACTIVITY << detail.receive_vip_activity
			<< Daily_Fields::CHECK_IN_DAY << detail.check_in_day
			<< Daily_Fields::LAST_CHECK_IN_REFRESH_TIME << (long long int)detail.last_check_in_refresh_time.sec()
			<< Daily_Fields::TODAY_IS_CHECK_IN << detail.today_is_check_in;

	MONGO_CONNECTION.update(Daily_Fields::COLLECTION,
			QUERY(Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_team_role(RoleId_TeamSportsRankingDetail_Map &rank_map_, Team_Sports_Ranking_Info &rank_,
		Time_Value &team_sports_start_time_, First_Lvl_Detail &first_lvl, int &game_times) {
	BSONObj res = MONGO_CONNECTION.findOne(Team_Sports_Fields::COLLECTION, QUERY(Team_Sports_Fields::PROFILE_TIME_SEC << (long long int)0));
	if (! res.isEmpty()) {
		//daily
		BSONObj daily_map = res.getObjectField(Team_Sports_Fields::INTEGRATE_MAP.c_str());
		BSONObjIterator iter_daily(daily_map);
		Team_Sports_Ranking_Detail r;
		while (iter_daily.more()) {
			BSONObj obj = iter_daily.next().embeddedObject();
			r.reset();
			r.role_id_ = obj[Team_Sports_Fields::ROLE_ID].numberLong();
			r.integrate_ = obj[Team_Sports_Fields::INTEGRATE].numberInt();
			r.win_times = obj["win"].numberInt();
			r.fail_times = obj["fail"].numberInt();

			Player_DB_View *target = LOGIC_MONITOR->find_player_db_view_by_role_id(r.role_id_);
			if (target) {
				r.name_ = target->role_name;
				r.force = target->ttl_force;
			} else {
				LOG_DEBUG("ERROR: role_id: %ld", r.role_id_);
			}
			rank_map_.insert(std::make_pair(r.role_id_, r));
			RoleId_TeamSportsRankingDetail_Map::iterator it = rank_map_.find(r.role_id_);
			if (it != rank_map_.end()) {
				rank_.push_back(&it->second);
			}
		}
		{
//			BSONObj daily_map = res.getObjectField(Team_Sports_Fields::FIRST_LVL.c_str());
			int32_t int_id = 0;
			role_id_t role_id = 0;
			{
				BSONObj fl_r = res.getObjectField(Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID_HIS.c_str());
				BSONObjIterator iter_daily(fl_r);
				while (iter_daily.more()) {
					BSONObj obj = iter_daily.next().embeddedObject();
					int_id = obj[Team_Sports_Fields::First_Lvl::FIRST_LVL_INT].numberLong();
					role_id = obj[Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID].numberLong();
					first_lvl.first_lvl_role_id_his.insert(std::make_pair(int_id, role_id));
				}
			}
			{
				BSONObj fl_r = res.getObjectField(Team_Sports_Fields::First_Lvl::FIRST_LVL_ZS_HIS.c_str());
				BSONObjIterator iter_daily(fl_r);
				while (iter_daily.more()) {
					BSONObj obj = iter_daily.next().embeddedObject();
					int_id = obj[Team_Sports_Fields::First_Lvl::FIRST_LVL_INT].numberLong();
					role_id = obj[Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID].numberLong();
					first_lvl.first_lvl_zs_his.insert(std::make_pair(int_id, role_id));
				}
			}
			{
				BSONObj fl_r = res.getObjectField(Team_Sports_Fields::First_Lvl::FIRST_LVL_CK_HIS.c_str());
				BSONObjIterator iter_daily(fl_r);
				while (iter_daily.more()) {
					BSONObj obj = iter_daily.next().embeddedObject();
					int_id = obj[Team_Sports_Fields::First_Lvl::FIRST_LVL_INT].numberLong();
					role_id = obj[Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID].numberLong();
					first_lvl.first_lvl_ck_his.insert(std::make_pair(int_id, role_id));
				}
			}
			{
				BSONObj fl_r = res.getObjectField(Team_Sports_Fields::First_Lvl::FIRST_LVL_YSS_HIS.c_str());
				BSONObjIterator iter_daily(fl_r);
				while (iter_daily.more()) {
					BSONObj obj = iter_daily.next().embeddedObject();
					int_id = obj[Team_Sports_Fields::First_Lvl::FIRST_LVL_INT].numberLong();
					role_id = obj[Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID].numberLong();
					first_lvl.first_lvl_yss_his.insert(std::make_pair(int_id, role_id));
				}
			}
		}
	}
	res = MONGO_CONNECTION.findOne(Team_Sports_Fields::COLLECTION, QUERY(Team_Sports_Fields::ROLE_ID << (long long int)0));
	if (! res.isEmpty()) {
		team_sports_start_time_.sec(res[Team_Sports_Fields::TEAM_SPORTS_START_TIME].numberLong());
		game_times = res[Team_Sports_Fields::TEAM_SPORTS_START_TIMES].numberLong();
	}
	return 0;
}

int DB_Operator::save_team_role(RoleId_TeamSportsRankingDetail_Map &detail,
		Time_Value &team_sports_start_time_,
		First_Lvl_Detail &first_lvl, int game_times) {
	vector<BSONObj> fl_r;
	for(Int_Uid::iterator iter = first_lvl.first_lvl_role_id_his.begin();
			iter != first_lvl.first_lvl_role_id_his.end(); ++iter) {
		BSONObjBuilder daily_builder;
		daily_builder << Team_Sports_Fields::First_Lvl::FIRST_LVL_INT << iter->first
				<< Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID << (long long int)iter->second;
		fl_r.push_back(daily_builder.obj());
	}
	vector<BSONObj> fl_zs;
	for(Int_Uid::iterator iter = first_lvl.first_lvl_zs_his.begin();
			iter != first_lvl.first_lvl_zs_his.end(); ++iter) {
		BSONObjBuilder daily_builder;
		daily_builder << Team_Sports_Fields::First_Lvl::FIRST_LVL_INT << iter->first
				<< Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID << (long long int)iter->second;
		fl_zs.push_back(daily_builder.obj());
	}
	vector<BSONObj> fl_ck;
	for(Int_Uid::iterator iter = first_lvl.first_lvl_ck_his.begin();
			iter != first_lvl.first_lvl_ck_his.end(); ++iter) {
		BSONObjBuilder daily_builder;
		daily_builder << Team_Sports_Fields::First_Lvl::FIRST_LVL_INT << iter->first
				<< Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID << (long long int)iter->second;
		fl_ck.push_back(daily_builder.obj());
	}
	vector<BSONObj> fl_yss;
	for(Int_Uid::iterator iter = first_lvl.first_lvl_yss_his.begin();
			iter != first_lvl.first_lvl_yss_his.end(); ++iter) {
		BSONObjBuilder daily_builder;
		daily_builder << Team_Sports_Fields::First_Lvl::FIRST_LVL_INT << iter->first
				<< Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID << (long long int)iter->second;
		fl_yss.push_back(daily_builder.obj());
	}

	if (detail.empty()) {
		BSONObjBuilder builder;
		vector<BSONObj> data_list;
		builder << Team_Sports_Fields::PROFILE_TIME_SEC << (long long int)0
				<< Team_Sports_Fields::INTEGRATE_MAP << data_list
				<< Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID_HIS << fl_r
				<< Team_Sports_Fields::First_Lvl::FIRST_LVL_ZS_HIS << fl_zs
				<< Team_Sports_Fields::First_Lvl::FIRST_LVL_CK_HIS << fl_ck
				<< Team_Sports_Fields::First_Lvl::FIRST_LVL_YSS_HIS << fl_yss
				;
		MONGO_CONNECTION.update(Team_Sports_Fields::COLLECTION,
				QUERY(Team_Sports_Fields::PROFILE_TIME_SEC << (long long int)(0)),
				BSON("$set" << builder.obj()), true);
	} else {
		vector<BSONObj> data_list;
		for(RoleId_TeamSportsRankingDetail_Map::iterator iter = detail.begin(); iter != detail.end(); ++iter) {
			BSONObjBuilder builder;
			builder << Team_Sports_Fields::ROLE_ID << (long long int)iter->second.role_id_
					<< Team_Sports_Fields::INTEGRATE << iter->second.integrate_
					<< Role_Fields::ROLE_NAME << iter->second.name_
					<< "win" << iter->second.win_times
					<< "fail" << iter->second.fail_times;
			data_list.push_back(builder.obj());
		}
		BSONObjBuilder builder;
			builder << Team_Sports_Fields::PROFILE_TIME_SEC << (long long int)0
					<< Team_Sports_Fields::INTEGRATE_MAP << data_list
					<< Team_Sports_Fields::First_Lvl::FIRST_LVL_ROLE_ID_HIS << fl_r
					<< Team_Sports_Fields::First_Lvl::FIRST_LVL_ZS_HIS << fl_zs
					<< Team_Sports_Fields::First_Lvl::FIRST_LVL_CK_HIS << fl_ck
					<< Team_Sports_Fields::First_Lvl::FIRST_LVL_YSS_HIS << fl_yss
					;
		MONGO_CONNECTION.update(Team_Sports_Fields::COLLECTION,
				QUERY(Team_Sports_Fields::PROFILE_TIME_SEC << (long long int)(0)),
				BSON("$set" << builder.obj()), true);
	}
	{
		BSONObjBuilder builder;
		builder << Team_Sports_Fields::ROLE_ID << (long long int)0
				<< Team_Sports_Fields::TEAM_SPORTS_START_TIME << (long long int)team_sports_start_time_.sec()
				<< Team_Sports_Fields::TEAM_SPORTS_START_TIMES << game_times;
		MONGO_CONNECTION.update(Team_Sports_Fields::COLLECTION,
						QUERY(Team_Sports_Fields::ROLE_ID << 0),
						BSON("$set" << builder.obj()), true);
	}
	{
		BSONObjBuilder builder;
		builder << Team_Sports_Fields::ROLE_ID << (long long int)0
				<< Team_Sports_Fields::TEAM_SPORTS_START_TIME << (long long int)team_sports_start_time_.sec();
		MONGO_CONNECTION.update(Team_Sports_Fields::COLLECTION,
						QUERY(Team_Sports_Fields::ROLE_ID << 0),
						BSON("$set" << builder.obj()), true);
	}

	return 0;
}

int DB_Operator::save_covenant_detail(Covenant_Detail &detail) {
	vector<BSONObj> data_list;
	data_list.clear();
	for(Covenant_Data_Map::iterator iter = detail.covenant_data_map.begin(); iter != detail.covenant_data_map.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Covenant_Fields::Covenant_Data::ID << iter->second.id
				<< Covenant_Fields::Covenant_Data::INIT_STATUS << iter->second.init_status
				<< Covenant_Fields::Covenant_Data::OVER_TIME << (long long int)iter->second.over_time.sec()
				<< Covenant_Fields::Covenant_Data::TODAY_BECOME_TIME << (long long int)iter->second.today_become_time.sec();
		data_list.push_back(builder.obj());
	}

	BSONObjBuilder builder;
	builder << Covenant_Fields::COVENANT_DATA_LIST << data_list;

	MONGO_CONNECTION.update(Covenant_Fields::COLLECTION,
			QUERY(Covenant_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_covenant_detail(Covenant_Detail &detail) {
	detail.covenant_data_map.clear();
	BSONObj res = MONGO_CONNECTION.findOne(Covenant_Fields::COLLECTION,QUERY(Covenant_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	BSONObj data_map = res.getObjectField(Covenant_Fields::COVENANT_DATA_LIST.c_str());
	BSONObjIterator iter_data(data_map);
	while (iter_data.more()) {
		BSONObj obj = iter_data.next().embeddedObject();
		Covenant_Data cd;
		cd.reset();

		cd.id = obj[Covenant_Fields::Covenant_Data::ID].numberInt();
		cd.init_status =  obj[Covenant_Fields::Covenant_Data::INIT_STATUS].numberInt();
		cd.over_time.sec(obj[Covenant_Fields::Covenant_Data::OVER_TIME].numberLong());
		cd.today_become_time.sec(obj[Covenant_Fields::Covenant_Data::TODAY_BECOME_TIME].numberLong());

		detail.covenant_data_map.insert(std::make_pair(cd.id, cd));
	}
	return 0;
}

int DB_Operator::load_vip_detail(Vip_Detail &detail) {
	detail.world_tree_val.clear();
	BSONObj res = MONGO_CONNECTION.findOne(Vip_Fields::COLLECTION,QUERY(Vip_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	detail.role_id = res[Vip_Fields::ROLE_ID].numberLong();
	detail.grow_val = res[Vip_Fields::GROW_VAL].numberInt();
	detail.last_settle_grow.sec(res[Vip_Fields::LAST_SETTLE_GROW].numberLong());
	detail.exp_over_time.sec(res[Vip_Fields::EXP_OVERTIME].numberLong());
	detail.cost_money = res[Vip_Fields::COST_MONEY].numberInt();
	detail.expired_prompt = res[Vip_Fields::EXPIRED_PROMPT].booleanSafe();
	detail.has_receive_daily_reward = res[Vip_Fields::HAS_REC_DAILY].booleanSafe();


	BSONObj tree_map = res.getObjectField(Vip_Fields::WORLD_TREE_LIST.c_str());
	BSONObjIterator iter_tree(tree_map);
	while (iter_tree.more()) {
		BSONObj obj = iter_tree.next().embeddedObject();
		World_Tree_Flock wtf;
		wtf.reset();

		wtf.id = obj[Vip_Fields::World_Tree_Data::ID].numberInt();
		wtf.copper =  obj[Vip_Fields::World_Tree_Data::COPPER].numberInt();
		wtf.souls =  obj[Vip_Fields::World_Tree_Data::SOULS].numberInt();
		wtf.dragon_soul =  obj[Vip_Fields::World_Tree_Data::DRAGON_SOUL].numberInt();
		wtf.receive_copper =  obj[Vip_Fields::World_Tree_Data::RECEIVE_COPPER].numberInt();
		wtf.receive_souls =  obj[Vip_Fields::World_Tree_Data::RECEIVE_SOULS].numberInt();
		wtf.receive_dragon_soul =  obj[Vip_Fields::World_Tree_Data::RECEIVE_DRAGON_SOUL].numberInt();

		detail.world_tree_val.insert(std::make_pair(wtf.id, wtf));
	}

	return 0;
}

int DB_Operator::save_vip_detail(Vip_Detail &detail) {
	BSONObjBuilder builder;

	vector<BSONObj> tree_list;
	tree_list.clear();
	for(World_Tree_Flock_Map::iterator iter = detail.world_tree_val.begin(); iter != detail.world_tree_val.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Vip_Fields::World_Tree_Data::ID << iter->second.id
				<< Vip_Fields::World_Tree_Data::COPPER << iter->second.copper
				<< Vip_Fields::World_Tree_Data::SOULS << iter->second.souls
				<< Vip_Fields::World_Tree_Data::DRAGON_SOUL << iter->second.dragon_soul
				<< Vip_Fields::World_Tree_Data::RECEIVE_COPPER << iter->second.receive_copper
				<< Vip_Fields::World_Tree_Data::RECEIVE_SOULS << iter->second.receive_souls
				<< Vip_Fields::World_Tree_Data::RECEIVE_DRAGON_SOUL << iter->second.receive_dragon_soul;
		tree_list.push_back(builder.obj());
	}

	builder << Vip_Fields::GROW_VAL << detail.grow_val
			<< Vip_Fields::LAST_SETTLE_GROW << (long long int)detail.last_settle_grow.sec()
			<< Vip_Fields::EXP_OVERTIME << (long long int)detail.exp_over_time.sec()
			<< Vip_Fields::COST_MONEY << detail.cost_money
			<< Vip_Fields::EXPIRED_PROMPT << detail.expired_prompt
			<< Vip_Fields::HAS_REC_DAILY << detail.has_receive_daily_reward
			<< Vip_Fields::WORLD_TREE_LIST << tree_list;

	MONGO_CONNECTION.update(Vip_Fields::COLLECTION,
			QUERY(Vip_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::save_reward_detail(Reward_Detail &detail) {
	BSONObjBuilder builder;
	//激活码领取信息
	vector<BSONObj> activation_code_receive_info_list;
	activation_code_receive_info_list.clear();
	for(UInt_UInt_Map::iterator iter = detail.activation_code_receive_info.begin(); iter != detail.activation_code_receive_info.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Reward_Fields::Activation_Code_Receive_Info::TYPE << iter->first
				<< Reward_Fields::Activation_Code_Receive_Info::NUM << iter->second;
		activation_code_receive_info_list.push_back(builder.obj());
	}

	////累计每天奖励找回项目对应次数
	vector<BSONObj> reward_back_all_num_list;
	reward_back_all_num_list.clear();
	for(Timesec_UInt_UInt_Map::iterator iter = detail.reward_back_all_num.begin(); iter != detail.reward_back_all_num.end(); ++iter) {
		BSONObjBuilder builder;

		vector<BSONObj> all_num_list;
		all_num_list.clear();
		for(UInt_UInt_Map::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			BSONObjBuilder tmp_builder;
			tmp_builder << Reward_Fields::KEY << it->first
					<< Reward_Fields::VALUE << it->second;
			all_num_list.push_back(tmp_builder.obj());
		}

		builder << Reward_Fields::KEY << (long long int)iter->first
				<< Reward_Fields::VALUE << all_num_list;
		reward_back_all_num_list.push_back(builder.obj());
	}

	//奖励找回项目类型 奖励值
	vector<BSONObj> reward_back_item_value_list;
	reward_back_item_value_list.clear();
	for(UInt_UInt_UInt_Map::iterator iter = detail.reward_back_item_value.begin(); iter != detail.reward_back_item_value.end(); ++iter) {
		BSONObjBuilder builder;

		vector<BSONObj> second_list;
		second_list.clear();
		for(UInt_UInt_Map::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			BSONObjBuilder tmp_builder;
			tmp_builder << Reward_Fields::KEY << it->first
					<< Reward_Fields::VALUE << it->second;
			second_list.push_back(tmp_builder.obj());
		}

		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << second_list;
		reward_back_item_value_list.push_back(builder.obj());
	}

	////奖励找回项目类型 奖励值,分批分次发送调用接口,刷历史最高来获取奖励找回
	vector<BSONObj> reward_back_item_value_tmp_list;
	reward_back_item_value_tmp_list.clear();
	for(UInt_UInt_UInt_Map::iterator iter = detail.reward_back_item_value_tmp.begin(); iter != detail.reward_back_item_value_tmp.end(); ++iter) {
		BSONObjBuilder builder;

		vector<BSONObj> second_list;
		second_list.clear();
		for(UInt_UInt_Map::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			BSONObjBuilder tmp_builder;
			tmp_builder << Reward_Fields::KEY << it->first
					<< Reward_Fields::VALUE << it->second;
			second_list.push_back(tmp_builder.obj());
		}

		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << second_list;
		reward_back_item_value_tmp_list.push_back(builder.obj());
	}

	//最近一次奖励找回领取时间
	vector<BSONObj> reward_back_receive_time_list;
	reward_back_receive_time_list.clear();
	for(UInt_Time_Map::iterator iter = detail.reward_back_receive_time.begin(); iter != detail.reward_back_receive_time.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << (long long int)iter->second.sec();
		reward_back_receive_time_list.push_back(builder.obj());
	}

	//项目首次完成时间
	vector<BSONObj> reward_back_first_time_list;
	reward_back_first_time_list.clear();
	for(UInt_Time_Map::iterator iter = detail.reward_back_first_time.begin(); iter != detail.reward_back_first_time.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << (long long int)iter->second.sec();
		reward_back_first_time_list.push_back(builder.obj());
	}

	builder << Reward_Fields::ONLINE_REWAED_NUM << detail.online_reward_num
			<< Reward_Fields::ONLINE_REWARD_TIME << (long long int)detail.online_reward_time.sec()
			<< Reward_Fields::ONLINE_REFRESH_REWARD_TIME << (long long int)detail.last_online_refresh_time.sec()
			<< Reward_Fields::ONLINE_REWAED_OVER << detail.online_reward_over
			//<< Reward_Fields::LOGIN_SEVEN_NUM << detail.login_seven_num
			//<< Reward_Fields::LOGIN_SEVEN_IS_RECEIVE << detail.login_seven_is_receive
			<< Reward_Fields::LOGIN_DAYS << detail.login_days
			<< Reward_Fields::RECEIVE_SEVEN_DAYS << detail.receive_seven_days
			<< Reward_Fields::ACTIVATION_CODE_RECEIVE_INFO << activation_code_receive_info_list
			<< Reward_Fields::RB_ALL_NUM << reward_back_all_num_list
			<< Reward_Fields::RB_ITEM_VALUE << reward_back_item_value_list
			<< Reward_Fields::RB_ITEM_VALUE_TMP << reward_back_item_value_tmp_list
			<< Reward_Fields::RB_RECEIVE_TIME << reward_back_receive_time_list
			<< Reward_Fields::RB_FIRST_TIME << reward_back_first_time_list
			<< Reward_Fields::KEEP_REWARD_STATUS << detail.keep_reward_status;

	MONGO_CONNECTION.update(Reward_Fields::COLLECTION,
			QUERY(Reward_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_reward_detail(Reward_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Reward_Fields::COLLECTION,QUERY(Reward_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.online_reward_num = res[Reward_Fields::ONLINE_REWAED_NUM].numberInt();
	detail.online_reward_over = res[Reward_Fields::ONLINE_REWAED_OVER].numberInt();
	detail.online_reward_time.sec(res[Reward_Fields::ONLINE_REWARD_TIME].numberLong());
	detail.last_online_refresh_time.sec(res[Reward_Fields::ONLINE_REFRESH_REWARD_TIME].numberLong());
	detail.login_days = res[Reward_Fields::LOGIN_DAYS].numberInt();
	{
		BSONObjIterator iter(res.getObjectField(Reward_Fields::RECEIVE_SEVEN_DAYS.c_str()));
		while (iter.more())	{
			detail.receive_seven_days.insert(iter.next().numberInt());
		}
	}
	detail.keep_reward_status = res[Reward_Fields::KEEP_REWARD_STATUS].numberInt();

	BSONObj act_code_info_map = res.getObjectField(Reward_Fields::ACTIVATION_CODE_RECEIVE_INFO.c_str());
	BSONObjIterator iter_act_info(act_code_info_map);
	while (iter_act_info.more()) {
		BSONObj obj = iter_act_info.next().embeddedObject();

		uint32_t type = obj[Reward_Fields::Activation_Code_Receive_Info::TYPE].numberInt();
		uint32_t num = obj[Reward_Fields::Activation_Code_Receive_Info::NUM].numberInt();

		detail.activation_code_receive_info.insert(std::make_pair(type, num));
	}

	{
		// Timesec_UInt_UInt_Map reward_back_all_num;//累计每天奖励找回项目对应次数
		BSONObj rb_all_num_map = res.getObjectField(Reward_Fields::RB_ALL_NUM.c_str());
		BSONObjIterator iter(rb_all_num_map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			long long int key = obj[Reward_Fields::KEY].numberLong();
			UInt_UInt_Map value;
			value.clear();

			BSONObj second_map = obj.getObjectField(Reward_Fields::VALUE.c_str());
			BSONObjIterator second_iter(second_map);
			while (second_iter.more()) {
				BSONObj second_obj = second_iter.next().embeddedObject();
				uint32_t second_key = second_obj[Reward_Fields::KEY].numberInt();
				uint32_t second_value = second_obj[Reward_Fields::VALUE].numberInt();
				value.insert(std::make_pair(second_key, second_value));
			}

			detail.reward_back_all_num.insert(std::make_pair(key, value));
		}
	}
	{
		// UInt_UInt_UInt_Map reward_back_item_value;//奖励找回项目类型 奖励值
		BSONObj map = res.getObjectField(Reward_Fields::RB_ITEM_VALUE.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			UInt_UInt_Map value;
			value.clear();

			BSONObj second_map = obj.getObjectField(Reward_Fields::VALUE.c_str());
			BSONObjIterator second_iter(second_map);
			while (second_iter.more()) {
				BSONObj second_obj = second_iter.next().embeddedObject();
				uint32_t second_key = second_obj[Reward_Fields::KEY].numberInt();
				uint32_t second_value = second_obj[Reward_Fields::VALUE].numberInt();
				value.insert(std::make_pair(second_key, second_value));
			}

			detail.reward_back_item_value.insert(std::make_pair(key, value));
		}
	}
	{
		// UInt_UInt_UInt_Map reward_back_item_value;//奖励找回项目类型 奖励值
		BSONObj map = res.getObjectField(Reward_Fields::RB_ITEM_VALUE_TMP.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			UInt_UInt_Map value;
			value.clear();

			BSONObj second_map = obj.getObjectField(Reward_Fields::VALUE.c_str());
			BSONObjIterator second_iter(second_map);
			while (second_iter.more()) {
				BSONObj second_obj = second_iter.next().embeddedObject();
				uint32_t second_key = second_obj[Reward_Fields::KEY].numberInt();
				uint32_t second_value = second_obj[Reward_Fields::VALUE].numberInt();
				value.insert(std::make_pair(second_key, second_value));
			}

			detail.reward_back_item_value_tmp.insert(std::make_pair(key, value));
		}
	}
	{
		// UInt_Time_Map reward_back_receive_time; //最近一次奖励找回领取时间
		BSONObj map = res.getObjectField(Reward_Fields::RB_RECEIVE_TIME.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			Time_Value value = Time_Value(obj[Reward_Fields::VALUE].numberLong());

			detail.reward_back_receive_time.insert(std::make_pair(key, value));
		}
	}
	{
		// UInt_Time_Map reward_back_receive_time; //项目首次完成时间
		BSONObj map = res.getObjectField(Reward_Fields::RB_FIRST_TIME.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			Time_Value value = Time_Value(obj[Reward_Fields::VALUE].numberLong());

			detail.reward_back_first_time.insert(std::make_pair(key, value));
		}
	}



	return 0;
}

int DB_Operator::load_campfire(Campfire_Detail_Map& campfire_info){
	unsigned long long res_count = MONGO_CONNECTION.count(Campfire_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder << Campfire_Fields::ID << 1
			<< Campfire_Fields::LEVEL << 1
			<< Campfire_Fields::GANG_ID << 1
			<< Campfire_Fields::GANG_NAME << 1
			<< Campfire_Fields::FIREWOOD_NUM << 1
			<< Campfire_Fields::BURNING << 1
			<< Campfire_Fields::LEVEL_FIREWOOD_NUM << 1
			<< Campfire_Fields::LEVEL_MAX_FIREWOOD_NUM << 1
			;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, Campfire_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	Campfire_Detail r;
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		r.reset();
		//r.id = obj[Campfire_Fields::ID].numberInt();
		r.level = obj[Campfire_Fields::LEVEL].numberInt();
		r.gang_id = obj[Campfire_Fields::GANG_ID].numberLong();
		r.gang_name = obj[Campfire_Fields::GANG_NAME].str();
		r.firewood_num = obj[Campfire_Fields::FIREWOOD_NUM].numberInt();
		r.burning = obj[Campfire_Fields::BURNING].numberInt();
		r.level_firewood_num = obj[Campfire_Fields::LEVEL_FIREWOOD_NUM].numberInt();
		r.level_max_firewood_num = obj[Campfire_Fields::LEVEL_MAX_FIREWOOD_NUM].numberInt();
		campfire_info.insert(std::make_pair(r.gang_id, r));
	}
	return 0;
}

int DB_Operator::save_campfire_detail(const Campfire_Detail& detail){
	BSONObjBuilder builder;
	//builder << Campfire_Fields::ID << detail.id
	builder << Campfire_Fields::LEVEL << detail.level
			<< Campfire_Fields::GANG_ID << (long long int)detail.gang_id
			<< Campfire_Fields::GANG_NAME << detail.gang_name
			<< Campfire_Fields::FIREWOOD_NUM << detail.firewood_num
			<< Campfire_Fields::BURNING << detail.burning
			<< Campfire_Fields::LEVEL_FIREWOOD_NUM << detail.level_firewood_num
			<< Campfire_Fields::LEVEL_MAX_FIREWOOD_NUM << detail.level_max_firewood_num
			;
	MONGO_CONNECTION.update(Campfire_Fields::COLLECTION, QUERY(Campfire_Fields::GANG_ID << (long long int)(detail.gang_id)),
						BSON("$set" << builder.obj() ), true);
	return 0;
}

int DB_Operator::load_world_boss_detail(World_Boss_Detail& detail){
	BSONObj find_it = MONGO_CONNECTION.findOne(World_Boss_Fields::COLLECTION, Query());
	if ( find_it.isEmpty() ) {
		return 0;
	}
	detail.act_id = find_it[World_Boss_Fields::ACT_ID].numberLong();
	detail.act_state = find_it[World_Boss_Fields::ACT_STATE].numberInt();
	detail.open_num = find_it[World_Boss_Fields::OPEN_NUM].numberInt();
	detail.kill_num = find_it[World_Boss_Fields::KILL_NUM].numberInt();
	detail.boss_lv = find_it[World_Boss_Fields::BOSS_LV].numberInt();
	return 0;
}

int DB_Operator::save_world_boss_detail(const World_Boss_Detail& detail){
	BSONObjBuilder set_builder;
	set_builder << World_Boss_Fields::ACT_ID << (long long int)detail.act_id
					<< World_Boss_Fields::ACT_STATE << detail.act_state
					<< World_Boss_Fields::OPEN_NUM << detail.open_num
					<< World_Boss_Fields::KILL_NUM << detail.kill_num
					<< World_Boss_Fields::BOSS_LV << detail.boss_lv
			        ;
	MONGO_CONNECTION.update(World_Boss_Fields::COLLECTION, Query(), BSON("$set" << set_builder.obj() ), true);
	return 0;
}

int DB_Operator::load_fb_info(FB_Info_Detail_Map& fb_info_map){
	unsigned long long res_count = MONGO_CONNECTION.count(FB_Info_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder << FB_Info_Fields::ROLE_ID << 1
			<< FB_Info_Fields::BEST_FORCE << 1
			<< FB_Info_Fields::BEST_ROLE_ID << 1
			<< FB_Info_Fields::BEST_ROLE_NAME << 1
			;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, FB_Info_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	FB_Info_Detail r;
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		r.reset();
		r.role_id = obj[FB_Info_Fields::ROLE_ID].numberLong();
		r.best_force = obj[FB_Info_Fields::BEST_FORCE].numberInt();
		r.best_role_id = obj[FB_Info_Fields::BEST_ROLE_ID].numberLong();
		r.best_role_name = obj[FB_Info_Fields::BEST_ROLE_NAME].str();
		fb_info_map.insert(std::make_pair(r.role_id, r));
	}
	return 0;
}

int DB_Operator::save_fb_info_detail(const FB_Info_Detail& detail){
	BSONObjBuilder builder;
	builder << FB_Info_Fields::ROLE_ID << (long long int)detail.role_id
			<< FB_Info_Fields::BEST_FORCE << detail.best_force
			<< FB_Info_Fields::BEST_ROLE_ID << (long long int)detail.best_role_id
			<< FB_Info_Fields::BEST_ROLE_NAME << detail.best_role_name
			;
	MONGO_CONNECTION.update(FB_Info_Fields::COLLECTION, QUERY(FB_Info_Fields::ROLE_ID << (long long int)(detail.role_id)),
						BSON("$set" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_access_count(UInt_UInt_Map &access_count) {
	Date_Time htime = Date_Time(Time_Value::gettimeofday());
	htime.minute(0);
	htime.second(0);
	htime.microsec(0);
	for(UInt_UInt_Map::iterator iter = access_count.begin(); iter != access_count.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Access_Count_Fields::NUM << iter->second;
		MONGO_CONNECTION.update(Access_Count_Fields::COLLECTION, QUERY(Access_Count_Fields::TYPE << iter->first << Access_Count_Fields::H_TIME << (long long int)htime.time_sec()),
				BSON("$inc" << builder.obj() ), true);
	}
	return 0;
}

int DB_Operator::load_active_db_event(DB_Active_Data_Info_Map &event) {
	event.clear();
	unsigned long long res_count = MONGO_CONNECTION.count(Active_Event_Info_Fields::COLLECTION);
	vector<BSONObj> res;
		BSONObjBuilder field_builder;
		field_builder 	<< Active_Event_Info_Fields::ID << 1
						<< Active_Event_Info_Fields::START_TIME << 1
						<< Active_Event_Info_Fields::END_TIME << 1
						<< Active_Event_Info_Fields::MID_END_TIME << 1;
		BSONObj field_bson = field_builder.obj();
		vector<BSONObj> iter_record;
		iter_record.reserve(res_count);
		DB_Active_Data_Info gi;
		MONGO_CONNECTION.findN(iter_record, Active_Event_Info_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
			BSONObj &obj = *iter;
			gi.reset();
			gi.id = obj[Active_Event_Info_Fields::ID].numberInt();
			gi.start_time.sec(obj[Active_Event_Info_Fields::START_TIME].numberInt());
			gi.end_time.sec(obj[Active_Event_Info_Fields::END_TIME].numberInt());
			gi.midway_end_time.sec(obj[Active_Event_Info_Fields::MID_END_TIME].numberInt());
			//插入数据
			event.insert(std::make_pair(gi.id, gi));
		}
	return 0;
}

int DB_Operator::save_active_content_detail(Active_Content_Detail &detail) {
	BSONObjBuilder builder;
	std::vector<int> suit_cnt;
	suit_cnt.clear();
	for(std::map<int, int>::iterator it = detail.suit_cnt_map.begin(); it != detail.suit_cnt_map.end(); ++it) {
		suit_cnt.push_back(it->first);
	}

	vector<BSONObj> arena_dan_pass_time_list;
	arena_dan_pass_time_list.clear();
	for(Group_Int_Time_Map::iterator iter = detail.arena_dan_pass_time.begin(); iter != detail.arena_dan_pass_time.end(); ++iter) {
		BSONObjBuilder builder;

		vector<BSONObj> second_list;
		second_list.clear();
		for(Int_Time_Map::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			BSONObjBuilder tmp_builder;
			tmp_builder << Reward_Fields::KEY << it->first
					<< Reward_Fields::VALUE << (long long int)it->second.sec();
			second_list.push_back(tmp_builder.obj());
		}

		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << second_list;
		arena_dan_pass_time_list.push_back(builder.obj());
	}

	vector<BSONObj> common_receive_id;
	common_receive_id.clear();
	for(Uint_Uint_Set::iterator iter = detail.common_receive_id.begin(); iter != detail.common_receive_id.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << iter->second;
		common_receive_id.push_back(builder.obj());
	}

	vector<BSONObj> heroes_door_type_level;
	heroes_door_type_level.clear();
	for(Uint_Uint_Set::iterator iter = detail.heroes_door_type_level.begin(); iter != detail.heroes_door_type_level.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << iter->second;
		heroes_door_type_level.push_back(builder.obj());
	}

	vector<BSONObj> comm_lstime;
	comm_lstime.clear();
	for(Int_Time_Map::iterator iter = detail.common_lstime.begin(); iter != detail.common_lstime.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << (long long int)iter->second.sec();
		comm_lstime.push_back(builder.obj());
	}

	vector<BSONObj> comm_tai;
	comm_tai.clear();
	for(UInt_UInt_Map::iterator iter = detail.common_type_act_id.begin(); iter != detail.common_type_act_id.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << iter->second;
		comm_tai.push_back(builder.obj());
	}

	builder << Active_Content_Fields::VIP_CLUB << detail.vip_club_receive_id
			<< Active_Content_Fields::LEVEL_ACE << detail.level_ace_receive_level
			<< Active_Content_Fields::SUIT_COLLECT << detail.suit_collect_receive_level
			<< Active_Content_Fields::GUILD_LEVEL << detail.guild_level_receive_id
			<< Active_Content_Fields::GEM_LEVELUP << detail.gem_levelup_receive_id
			<< Active_Content_Fields::WING_LEVELUP << detail.wing_levelup_receive_level
			<< Active_Content_Fields::MOUNT_LEVELUP << detail.mount_levelup_receive_id
			<< Active_Content_Fields::SG_PASS << detail.sg_pass_receive_id
			<< Active_Content_Fields::MOUNT_PASS << detail.mount_pass_receive_id
			<< Active_Content_Fields::WING_PASS << detail.wing_pass_receive_id
			<< Active_Content_Fields::ARENA_DAN << detail.arena_dan_receive_id
			<< Active_Content_Fields::ARENA_DAN_TIME << arena_dan_pass_time_list
			<< Active_Content_Fields::SUIT_CNT << suit_cnt
			<< Active_Content_Fields::COM_REC_ID << common_receive_id
			<< Active_Content_Fields::ALL_WORLD_BOSS_HURT << detail.all_world_boss_hurt
			<< Active_Content_Fields::TODAY_WORLD_BOSS_HURT << detail.today_world_boss_hurt
			<< Active_Content_Fields::WORLD_BOSS_MAX_RANK << detail.world_boos_max_rank
			<< Active_Content_Fields::HEROES_DOOR_TYPE_LV << heroes_door_type_level
			<< Active_Content_Fields::TEAM_ARENA_WNUM << detail.team_arena_win_num
			<< Active_Content_Fields::WARER_WNUM << detail.warer_win_num
			<< Active_Content_Fields::REW_TASK_DONE_NUM << detail.reward_task_done_num
			<< Active_Content_Fields::ON_SEND_MAIL_ACT_ID << detail.on_send_mail_act_id
			<< Active_Content_Fields::ENCHANTING_NUM << detail.enchanting_num
			<< Active_Content_Fields::DRAGON_PREY_NUM << detail.dragon_prey_num
			<< Active_Content_Fields::RING_TASK_RECHARGE << detail.ring_task_recharge
			<< Active_Content_Fields::MOUNT_LVUP_NUM << detail.mount_lvup_num
			<< Active_Content_Fields::FUSION_X_ELITE_CARD_NUM << detail.fusion_x_elite_card_num
			<< Active_Content_Fields::ALCHEMY_NUM << detail.alchemy_num
			<< Active_Content_Fields::COMMON_LSTIME << comm_lstime
			<< Active_Content_Fields::COMMON_TAI << comm_tai
			;
	MONGO_CONNECTION.update(Active_Content_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
						BSON("$set" << builder.obj() ), true);
	return 0;
}

int DB_Operator::load_active_content(Active_Content_Info &c) {
	c.reset();
	BSONObj res = MONGO_CONNECTION.findOne(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_ACTIVE_CONTENT));
	if (res.isEmpty()) {
		return 0;
	}
	BSONObj obj = res.getObjectField(Common_Fields::VALUE.c_str());
	if (obj.isEmpty()) return -1;
	{
		BSONObjIterator iter(obj.getObjectField(Active_Content_Info_Fields::VIP_AN_LV.c_str()));
		while (iter.more())	{
			c.vip_announce_ed_level.insert(iter.next().numberInt());
		}
	}
	c.level_rank_count = obj[Active_Content_Info_Fields::LRC].booleanSafe();
	c.arena_rank_count = obj[Active_Content_Info_Fields::ARC].booleanSafe();
	c.force_rank_count = obj[Active_Content_Info_Fields::FRC].booleanSafe();
	c.sg_rank_count = obj[Active_Content_Info_Fields::SGRC].booleanSafe();
	c.mount_rank_count = obj[Active_Content_Info_Fields::MOUNT_COUNT].booleanSafe();
	c.wing_rank_count = obj[Active_Content_Info_Fields::WING_COUNT].booleanSafe();
	c.lr_last_start_time.sec(obj[Active_Content_Info_Fields::LRT].numberLong());
	c.fr_last_start_time.sec(obj[Active_Content_Info_Fields::FRT].numberLong());
	c.ar_last_start_time.sec(obj[Active_Content_Info_Fields::ART].numberLong());
	c.sgr_last_start_time.sec(obj[Active_Content_Info_Fields::SGRT].numberLong());
	c.mount_last_start_time.sec(obj[Active_Content_Info_Fields::MOUNT_TIME].numberLong());
	c.wing_last_start_time.sec(obj[Active_Content_Info_Fields::WING_TIME].numberLong());
	c.arena_dan_start_time.sec(obj[Active_Content_Info_Fields::ADST].numberLong());
	c.arena_dan_last_id = obj[Active_Content_Info_Fields::ADLI].numberInt();

	{
		BSONObj rsv_obj = obj.getObjectField(Active_Content_Info_Fields::LR_ROLE.c_str());
		BSONObjIterator iter(rsv_obj);
		Role_Info data;
		while(iter.more()) {
			data.reset();
			BSONObj log_obj = iter.next().embeddedObject();
			data.role_id  = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_ID].numberLong();
			data.role_name = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_NAME].valuestrsafe();
			data.head_photo_id = log_obj[Active_Content_Info_Fields::Role_Info::HPI].numberInt();
			data.gender = log_obj[Active_Content_Info_Fields::Role_Info::GENDER].numberInt();
			data.level = log_obj[Active_Content_Info_Fields::Role_Info::LEVEL].numberInt();
			data.guild_name = log_obj[Active_Content_Info_Fields::Role_Info::GUILD_NAME].valuestrsafe();
			data.career = log_obj[Active_Content_Info_Fields::Role_Info::CAREER].numberInt();
			data.vip_level = log_obj[Active_Content_Info_Fields::Role_Info::VIP_LEVEL].numberInt();
			data.combat = log_obj[Active_Content_Info_Fields::Role_Info::COMBAT].numberInt();
			data.title_id = log_obj[Active_Content_Info_Fields::Role_Info::TITLE_ID].numberInt();
			c.level_rank_count_role_info.push_back(data);
		}
	}
	{
		BSONObj rsv_obj = obj.getObjectField(Active_Content_Info_Fields::FR_ROLE.c_str());
		BSONObjIterator iter(rsv_obj);
		Role_Info data;
		while(iter.more()) {
			data.reset();
			BSONObj log_obj = iter.next().embeddedObject();
			data.role_id  = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_ID].numberLong();
			data.role_name = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_NAME].valuestrsafe();
			data.head_photo_id = log_obj[Active_Content_Info_Fields::Role_Info::HPI].numberInt();
			data.gender = log_obj[Active_Content_Info_Fields::Role_Info::GENDER].numberInt();
			data.level = log_obj[Active_Content_Info_Fields::Role_Info::LEVEL].numberInt();
			data.guild_name = log_obj[Active_Content_Info_Fields::Role_Info::GUILD_NAME].valuestrsafe();
			data.career = log_obj[Active_Content_Info_Fields::Role_Info::CAREER].numberInt();
			data.vip_level = log_obj[Active_Content_Info_Fields::Role_Info::VIP_LEVEL].numberInt();
			data.combat = log_obj[Active_Content_Info_Fields::Role_Info::COMBAT].numberInt();
			data.title_id = log_obj[Active_Content_Info_Fields::Role_Info::TITLE_ID].numberInt();
			c.force_rank_count_role_info.push_back(data);
		}
	}
	{
		BSONObj rsv_obj = obj.getObjectField(Active_Content_Info_Fields::SGR_ROLE.c_str());
		BSONObjIterator iter(rsv_obj);
		Role_Info data;
		while(iter.more()) {
			data.reset();
			BSONObj log_obj = iter.next().embeddedObject();
			data.role_id  = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_ID].numberLong();
			data.role_name = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_NAME].valuestrsafe();
			data.head_photo_id = log_obj[Active_Content_Info_Fields::Role_Info::HPI].numberInt();
			data.gender = log_obj[Active_Content_Info_Fields::Role_Info::GENDER].numberInt();
			data.level = log_obj[Active_Content_Info_Fields::Role_Info::LEVEL].numberInt();
			data.guild_name = log_obj[Active_Content_Info_Fields::Role_Info::GUILD_NAME].valuestrsafe();
			data.career = log_obj[Active_Content_Info_Fields::Role_Info::CAREER].numberInt();
			data.vip_level = log_obj[Active_Content_Info_Fields::Role_Info::VIP_LEVEL].numberInt();
			data.combat = log_obj[Active_Content_Info_Fields::Role_Info::COMBAT].numberInt();
			data.title_id = log_obj[Active_Content_Info_Fields::Role_Info::TITLE_ID].numberInt();
			c.sg_rank_count_role_info.push_back(data);
		}
	}
	{
		BSONObj rsv_obj = obj.getObjectField(Active_Content_Info_Fields::MOUNT_ROLE.c_str());
		BSONObjIterator iter(rsv_obj);
		Role_Info data;
		while(iter.more()) {
			data.reset();
			BSONObj log_obj = iter.next().embeddedObject();
			data.role_id  = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_ID].numberLong();
			data.role_name = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_NAME].valuestrsafe();
			data.head_photo_id = log_obj[Active_Content_Info_Fields::Role_Info::HPI].numberInt();
			data.gender = log_obj[Active_Content_Info_Fields::Role_Info::GENDER].numberInt();
			data.level = log_obj[Active_Content_Info_Fields::Role_Info::LEVEL].numberInt();
			data.guild_name = log_obj[Active_Content_Info_Fields::Role_Info::GUILD_NAME].valuestrsafe();
			data.career = log_obj[Active_Content_Info_Fields::Role_Info::CAREER].numberInt();
			data.vip_level = log_obj[Active_Content_Info_Fields::Role_Info::VIP_LEVEL].numberInt();
			data.combat = log_obj[Active_Content_Info_Fields::Role_Info::COMBAT].numberInt();
			data.title_id = log_obj[Active_Content_Info_Fields::Role_Info::TITLE_ID].numberInt();
			c.mount_rank_count_role_info.push_back(data);
		}
	}
	{
		BSONObj rsv_obj = obj.getObjectField(Active_Content_Info_Fields::WING_ROLE.c_str());
		BSONObjIterator iter(rsv_obj);
		Role_Info data;
		while(iter.more()) {
			data.reset();
			BSONObj log_obj = iter.next().embeddedObject();
			data.role_id  = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_ID].numberLong();
			data.role_name = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_NAME].valuestrsafe();
			data.head_photo_id = log_obj[Active_Content_Info_Fields::Role_Info::HPI].numberInt();
			data.gender = log_obj[Active_Content_Info_Fields::Role_Info::GENDER].numberInt();
			data.level = log_obj[Active_Content_Info_Fields::Role_Info::LEVEL].numberInt();
			data.guild_name = log_obj[Active_Content_Info_Fields::Role_Info::GUILD_NAME].valuestrsafe();
			data.career = log_obj[Active_Content_Info_Fields::Role_Info::CAREER].numberInt();
			data.vip_level = log_obj[Active_Content_Info_Fields::Role_Info::VIP_LEVEL].numberInt();
			data.combat = log_obj[Active_Content_Info_Fields::Role_Info::COMBAT].numberInt();
			data.title_id = log_obj[Active_Content_Info_Fields::Role_Info::TITLE_ID].numberInt();
			c.wing_rank_count_role_info.push_back(data);
		}
	}
	{
		BSONObj rsv_obj = obj.getObjectField(Active_Content_Info_Fields::AR_ROLE.c_str());
		BSONObjIterator iter(rsv_obj);
		Role_Info data;
		while(iter.more()) {
			data.reset();
			BSONObj log_obj = iter.next().embeddedObject();
			data.role_id  = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_ID].numberLong();
			data.role_name = log_obj[Active_Content_Info_Fields::Role_Info::ROLE_NAME].valuestrsafe();
			data.head_photo_id = log_obj[Active_Content_Info_Fields::Role_Info::HPI].numberInt();
			data.gender = log_obj[Active_Content_Info_Fields::Role_Info::GENDER].numberInt();
			data.level = log_obj[Active_Content_Info_Fields::Role_Info::LEVEL].numberInt();
			data.guild_name = log_obj[Active_Content_Info_Fields::Role_Info::GUILD_NAME].valuestrsafe();
			data.career = log_obj[Active_Content_Info_Fields::Role_Info::CAREER].numberInt();
			data.vip_level = log_obj[Active_Content_Info_Fields::Role_Info::VIP_LEVEL].numberInt();
			data.combat = log_obj[Active_Content_Info_Fields::Role_Info::COMBAT].numberInt();
			data.title_id = log_obj[Active_Content_Info_Fields::Role_Info::TITLE_ID].numberInt();
			c.arena_rank_count_role_info.push_back(data);
		}
	}
	{
		BSONObj map = res.getObjectField(Active_Content_Info_Fields::COM_LSTIME.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			Time_Value value;
			value.sec(obj[Reward_Fields::VALUE].numberLong());

			c.common_lstime.insert(std::make_pair(key, value));
		}
	}
	return 0;
}

int DB_Operator::save_active_content(Active_Content_Info &c) {
	std::vector<BSONObj> lr;
	{
		for (std::vector<Role_Info>::iterator it = c.level_rank_count_role_info.begin();
				it != c.level_rank_count_role_info.end(); ++it) {
			BSONObjBuilder ibuilder;
			ibuilder << Active_Content_Info_Fields::Role_Info::ROLE_ID << (long long int)it->role_id
					<< Active_Content_Info_Fields::Role_Info::ROLE_NAME << it->role_name
					<< Active_Content_Info_Fields::Role_Info::HPI << it->head_photo_id
					<< Active_Content_Info_Fields::Role_Info::GENDER << it->gender
					<< Active_Content_Info_Fields::Role_Info::GUILD_NAME << it->guild_name
					<< Active_Content_Info_Fields::Role_Info::CAREER << it->career
					<< Active_Content_Info_Fields::Role_Info::LEVEL << it->level
					<< Active_Content_Info_Fields::Role_Info::VIP_LEVEL << it->vip_level
					<< Active_Content_Info_Fields::Role_Info::COMBAT << it->combat
					<< Active_Content_Info_Fields::Role_Info::TITLE_ID << it->title_id;
			lr.push_back(ibuilder.obj());
		}
	}
	std::vector<BSONObj> fr;
	{
		for (std::vector<Role_Info>::iterator it = c.force_rank_count_role_info.begin();
				it != c.force_rank_count_role_info.end(); ++it) {
			BSONObjBuilder ibuilder;
			ibuilder << Active_Content_Info_Fields::Role_Info::ROLE_ID << (long long int)it->role_id
					<< Active_Content_Info_Fields::Role_Info::ROLE_NAME << it->role_name
					<< Active_Content_Info_Fields::Role_Info::HPI << it->head_photo_id
					<< Active_Content_Info_Fields::Role_Info::GENDER << it->gender
					<< Active_Content_Info_Fields::Role_Info::GUILD_NAME << it->guild_name
					<< Active_Content_Info_Fields::Role_Info::CAREER << it->career
					<< Active_Content_Info_Fields::Role_Info::LEVEL << it->level
					<< Active_Content_Info_Fields::Role_Info::VIP_LEVEL << it->vip_level
					<< Active_Content_Info_Fields::Role_Info::COMBAT << it->combat
					<< Active_Content_Info_Fields::Role_Info::TITLE_ID << it->title_id;
			fr.push_back(ibuilder.obj());
		}
	}
	std::vector<BSONObj> sgr;
	{
		for (std::vector<Role_Info>::iterator it = c.sg_rank_count_role_info.begin();
				it != c.sg_rank_count_role_info.end(); ++it) {
			BSONObjBuilder ibuilder;
			ibuilder << Active_Content_Info_Fields::Role_Info::ROLE_ID << (long long int)it->role_id
					<< Active_Content_Info_Fields::Role_Info::ROLE_NAME << it->role_name
					<< Active_Content_Info_Fields::Role_Info::HPI << it->head_photo_id
					<< Active_Content_Info_Fields::Role_Info::GENDER << it->gender
					<< Active_Content_Info_Fields::Role_Info::GUILD_NAME << it->guild_name
					<< Active_Content_Info_Fields::Role_Info::CAREER << it->career
					<< Active_Content_Info_Fields::Role_Info::LEVEL << it->level
					<< Active_Content_Info_Fields::Role_Info::VIP_LEVEL << it->vip_level
					<< Active_Content_Info_Fields::Role_Info::COMBAT << it->combat
					<< Active_Content_Info_Fields::Role_Info::TITLE_ID << it->title_id;
			sgr.push_back(ibuilder.obj());
		}
	}
	std::vector<BSONObj> mount_role;
	{
		for (std::vector<Role_Info>::iterator it = c.mount_rank_count_role_info.begin();
				it != c.mount_rank_count_role_info.end(); ++it) {
			BSONObjBuilder ibuilder;
			ibuilder << Active_Content_Info_Fields::Role_Info::ROLE_ID << (long long int)it->role_id
					<< Active_Content_Info_Fields::Role_Info::ROLE_NAME << it->role_name
					<< Active_Content_Info_Fields::Role_Info::HPI << it->head_photo_id
					<< Active_Content_Info_Fields::Role_Info::GENDER << it->gender
					<< Active_Content_Info_Fields::Role_Info::GUILD_NAME << it->guild_name
					<< Active_Content_Info_Fields::Role_Info::CAREER << it->career
					<< Active_Content_Info_Fields::Role_Info::LEVEL << it->level
					<< Active_Content_Info_Fields::Role_Info::VIP_LEVEL << it->vip_level
					<< Active_Content_Info_Fields::Role_Info::COMBAT << it->combat
					<< Active_Content_Info_Fields::Role_Info::TITLE_ID << it->title_id;
			mount_role.push_back(ibuilder.obj());
		}
	}
	std::vector<BSONObj> wing_role;
	{
		for (std::vector<Role_Info>::iterator it = c.wing_rank_count_role_info.begin();
				it != c.wing_rank_count_role_info.end(); ++it) {
			BSONObjBuilder ibuilder;
			ibuilder << Active_Content_Info_Fields::Role_Info::ROLE_ID << (long long int)it->role_id
					<< Active_Content_Info_Fields::Role_Info::ROLE_NAME << it->role_name
					<< Active_Content_Info_Fields::Role_Info::HPI << it->head_photo_id
					<< Active_Content_Info_Fields::Role_Info::GENDER << it->gender
					<< Active_Content_Info_Fields::Role_Info::GUILD_NAME << it->guild_name
					<< Active_Content_Info_Fields::Role_Info::CAREER << it->career
					<< Active_Content_Info_Fields::Role_Info::LEVEL << it->level
					<< Active_Content_Info_Fields::Role_Info::VIP_LEVEL << it->vip_level
					<< Active_Content_Info_Fields::Role_Info::COMBAT << it->combat
					<< Active_Content_Info_Fields::Role_Info::TITLE_ID << it->title_id;
			wing_role.push_back(ibuilder.obj());
		}
	}
	std::vector<BSONObj> ar;
	{
		for (std::vector<Role_Info>::iterator it = c.arena_rank_count_role_info.begin();
				it != c.arena_rank_count_role_info.end(); ++it) {
			BSONObjBuilder ibuilder;
			ibuilder << Active_Content_Info_Fields::Role_Info::ROLE_ID << (long long int)it->role_id
					<< Active_Content_Info_Fields::Role_Info::ROLE_NAME << it->role_name
					<< Active_Content_Info_Fields::Role_Info::HPI << it->head_photo_id
					<< Active_Content_Info_Fields::Role_Info::GENDER << it->gender
					<< Active_Content_Info_Fields::Role_Info::GUILD_NAME << it->guild_name
					<< Active_Content_Info_Fields::Role_Info::CAREER << it->career
					<< Active_Content_Info_Fields::Role_Info::LEVEL << it->level
					<< Active_Content_Info_Fields::Role_Info::VIP_LEVEL << it->vip_level
					<< Active_Content_Info_Fields::Role_Info::COMBAT << it->combat
					<< Active_Content_Info_Fields::Role_Info::TITLE_ID << it->title_id;
			ar.push_back(ibuilder.obj());
		}
	}

	vector<BSONObj> common_lstime;
	common_lstime.clear();
	for(Int_Time_Map::iterator iter = c.common_lstime.begin(); iter != c.common_lstime.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << (long long int)iter->second.sec();
		common_lstime.push_back(builder.obj());
	}

	BSONObjBuilder builder;
	builder << Common_Fields::TYPE << DB_COMMON_TYPE_ACTIVE_CONTENT
			<< Common_Fields::VALUE << BSON(
					 	 	 Active_Content_Info_Fields::VIP_AN_LV << c.vip_announce_ed_level
							<< Active_Content_Info_Fields::LRC << c.level_rank_count
							<< Active_Content_Info_Fields::ARC << c.arena_rank_count
							<< Active_Content_Info_Fields::FRC << c.force_rank_count
							<< Active_Content_Info_Fields::SGRC << c.sg_rank_count
							<< Active_Content_Info_Fields::MOUNT_COUNT << c.mount_rank_count
							<< Active_Content_Info_Fields::WING_COUNT << c.wing_rank_count
							<< Active_Content_Info_Fields::LRT << (long long int)c.lr_last_start_time.sec()
							<< Active_Content_Info_Fields::FRT << (long long int)c.fr_last_start_time.sec()
							<< Active_Content_Info_Fields::SGRT << (long long int)c.sgr_last_start_time.sec()
							<< Active_Content_Info_Fields::MOUNT_TIME << (long long int)c.mount_last_start_time.sec()
							<< Active_Content_Info_Fields::WING_TIME << (long long int)c.wing_last_start_time.sec()
							<< Active_Content_Info_Fields::ADST << (long long int)c.arena_dan_start_time.sec()
							<< Active_Content_Info_Fields::LR_ROLE << lr
							<< Active_Content_Info_Fields::FR_ROLE << fr
							<< Active_Content_Info_Fields::AR_ROLE << ar
							<< Active_Content_Info_Fields::SGR_ROLE << sgr
							<< Active_Content_Info_Fields::WING_ROLE << wing_role
							<< Active_Content_Info_Fields::MOUNT_ROLE << mount_role
							<< Active_Content_Info_Fields::ART << (long long int)c.ar_last_start_time.sec()
							<< Active_Content_Info_Fields::ADLI << c.arena_dan_last_id
							<< Active_Content_Info_Fields::COM_LSTIME << common_lstime
			);
	MONGO_CONNECTION.update(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_ACTIVE_CONTENT),
							BSON("$set" << builder.obj() ), true);
	return 0;
}

int DB_Operator::load_active_content_detail(Active_Content_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Active_Content_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	detail.guild_level_receive_id.clear();
	detail.level_ace_receive_level.clear();
	detail.vip_club_receive_id.clear();
	detail.suit_collect_receive_level.clear();
	detail.gem_levelup_receive_id.clear();
	detail.wing_levelup_receive_level.clear();
	detail.mount_levelup_receive_id.clear();
	detail.sg_pass_receive_id.clear();
	detail.mount_pass_receive_id.clear();
	detail.wing_pass_receive_id.clear();
	detail.arena_dan_receive_id.clear();
	detail.arena_dan_pass_time.clear();
	if (res.isEmpty()) {
		return 0;
	}
	detail.all_world_boss_hurt = res[Active_Content_Fields::ALL_WORLD_BOSS_HURT].numberInt();
	detail.today_world_boss_hurt = res[Active_Content_Fields::TODAY_WORLD_BOSS_HURT].numberInt();
	detail.world_boos_max_rank = res[Active_Content_Fields::WORLD_BOSS_MAX_RANK].numberInt();
	detail.team_arena_win_num = res[Active_Content_Fields::TEAM_ARENA_WNUM].numberInt();
	detail.warer_win_num = res[Active_Content_Fields::WARER_WNUM].numberInt();
	detail.reward_task_done_num = res[Active_Content_Fields::REW_TASK_DONE_NUM].numberInt();
	detail.enchanting_num = res[Active_Content_Fields::ENCHANTING_NUM].numberInt();
	detail.dragon_prey_num = res[Active_Content_Fields::DRAGON_PREY_NUM].numberInt();
	detail.ring_task_recharge = res[Active_Content_Fields::RING_TASK_RECHARGE].booleanSafe();
	detail.mount_lvup_num = res[Active_Content_Fields::MOUNT_LVUP_NUM].numberInt();
	detail.fusion_x_elite_card_num = res[Active_Content_Fields::FUSION_X_ELITE_CARD_NUM].numberInt();
	detail.alchemy_num = res[Active_Content_Fields::ALCHEMY_NUM].numberInt();
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::VIP_CLUB.c_str()));
		while (iter.more())	{
			detail.vip_club_receive_id.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::LEVEL_ACE.c_str()));
		while (iter.more())	{
			detail.level_ace_receive_level.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::SUIT_COLLECT.c_str()));
		while (iter.more())	{
			detail.suit_collect_receive_level.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::GUILD_LEVEL.c_str()));
		while (iter.more())	{
			detail.guild_level_receive_id.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::GEM_LEVELUP.c_str()));
		while (iter.more())	{
			detail.gem_levelup_receive_id.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::WING_LEVELUP.c_str()));
		while (iter.more())	{
			detail.wing_levelup_receive_level.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::MOUNT_LEVELUP.c_str()));
		while (iter.more())	{
			detail.mount_levelup_receive_id.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::SG_PASS.c_str()));
		while (iter.more())	{
			detail.sg_pass_receive_id.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::MOUNT_PASS.c_str()));
		while (iter.more())	{
			detail.mount_pass_receive_id.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::WING_PASS.c_str()));
		while (iter.more())	{
			detail.wing_pass_receive_id.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::SUIT_CNT.c_str()));
		while (iter.more())	{
			detail.suit_cnt_map[iter.next().numberInt()] = 1;
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::ARENA_DAN.c_str()));
		while (iter.more())	{
			detail.arena_dan_receive_id.insert(iter.next().numberInt());
		}
	}
	{
		BSONObj map = res.getObjectField(Active_Content_Fields::ARENA_DAN_TIME.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			Int_Time_Map value;
			value.clear();

			BSONObj second_map = obj.getObjectField(Reward_Fields::VALUE.c_str());
			BSONObjIterator second_iter(second_map);
			while (second_iter.more()) {
				BSONObj second_obj = second_iter.next().embeddedObject();
				uint32_t second_key = second_obj[Reward_Fields::KEY].numberInt();
				uint32_t second_value = second_obj[Reward_Fields::VALUE].numberInt();
				value.insert(std::make_pair(second_key, Time_Value(second_value)));
			}

			detail.arena_dan_pass_time.insert(std::make_pair(key, value));
		}
	}
	{
		BSONObj map = res.getObjectField(Active_Content_Fields::COM_REC_ID.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			UInt_Set value;
			value.clear();

			BSONObjIterator second_iter(obj.getObjectField(Reward_Fields::VALUE.c_str()));
			while (second_iter.more())	{
				value.insert(second_iter.next().numberInt());
			}

			detail.common_receive_id.insert(std::make_pair(key, value));
		}
	}
	{
		BSONObj map = res.getObjectField(Active_Content_Fields::HEROES_DOOR_TYPE_LV.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			UInt_Set value;
			value.clear();

			BSONObjIterator second_iter(obj.getObjectField(Reward_Fields::VALUE.c_str()));
			while (second_iter.more())	{
				value.insert(second_iter.next().numberInt());
			}

			detail.heroes_door_type_level.insert(std::make_pair(key, value));
		}
	}
	{
		BSONObj map = res.getObjectField(Active_Content_Fields::COMMON_LSTIME.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			Time_Value time = Time_Value(obj[Reward_Fields::VALUE].numberLong());

			detail.common_lstime.insert(std::make_pair(key, time));
		}
	}
	{
		BSONObj map = res.getObjectField(Active_Content_Fields::COMMON_TAI.c_str());
		BSONObjIterator iter(map);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();

			uint32_t key = obj[Reward_Fields::KEY].numberInt();
			uint32_t val = obj[Reward_Fields::VALUE].numberInt();

			detail.common_type_act_id.insert(std::make_pair(key, val));
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Active_Content_Fields::ON_SEND_MAIL_ACT_ID.c_str()));
		while (iter.more())	{
			detail.on_send_mail_act_id.insert(iter.next().numberInt());
		}
	}
	return 0;
}

int DB_Operator::save_proposal(role_id_t role_id, uint8_t type, std::string &contents, int64_t time) {
	vector<BSONObj> reply_vec;
	reply_vec.clear();
	BSONObjBuilder builder;
		builder	<< Proposal_Fields::ROLE_ID << (long long int)role_id
				<< Proposal_Fields::TYPE << type
				<< Proposal_Fields::CONTENTS << contents
				<< Proposal_Fields::TIME << (long long int)time
				<< Proposal_Fields::REPLY << reply_vec;
	MONGO_CONNECTION.insert(Proposal_Fields::COLLECTION, builder.obj());
	return 0;
}

int DB_Operator::load_fashion_detail(Fashion_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Fashion_Fields::COLLECTION, QUERY(Fashion_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) return -1;

	detail.is_show_ = res[Fashion_Fields::SHOW_STATE].numberInt();

	const BSONObj &item_obj = res.getObjectField(Fashion_Fields::FASHION_PART.c_str());
	BSONObjIterator iter(item_obj);
	Fashion_Part part;
	while(iter.more()) {
		part.reset();
		const BSONObj &obj = iter.next().embeddedObject();
		part.part_id_ = obj[Fashion_Fields::Fashion_Part::ID].numberInt();
		part.lv_ = obj[Fashion_Fields::Fashion_Part::LV].numberInt();
		part.exp_ = obj[Fashion_Fields::Fashion_Part::EXP].numberInt();
		part.used_fashion_id_ = obj[Fashion_Fields::Fashion_Part::USED_ID].numberInt();

		const BSONObj &ids_obj = obj.getObjectField(Fashion_Fields::Fashion_Part::IDS.c_str());
		BSONObjIterator ids_iter(ids_obj);
		while(ids_iter.more()) {
			part.fashion_id_vec_.push_back(ids_iter.next().numberInt());
		}

		detail.fashion_part_map_[part.part_id_] = part;
	}

	return 0;
}

int DB_Operator::save_fashion_detail(Fashion_Detail &detail) {
	BSONObjBuilder builder;
	std::vector<BSONObj> item_bson;
	for (Fashion_Part_Map::iterator it = detail.fashion_part_map_.begin();
			it != detail.fashion_part_map_.end(); ++it) {
		BSONObjBuilder partbuilder;
		partbuilder << Fashion_Fields::Fashion_Part::ID << it->second.part_id_
				<< Fashion_Fields::Fashion_Part::LV << it->second.lv_
				<< Fashion_Fields::Fashion_Part::EXP << it->second.exp_
				<< Fashion_Fields::Fashion_Part::USED_ID << it->second.used_fashion_id_
				<< Fashion_Fields::Fashion_Part::IDS << it->second.fashion_id_vec_;
		item_bson.push_back(partbuilder.obj());
	}
	builder << Fashion_Fields::ROLE_ID << (long long int)detail.role_id
			<< Fashion_Fields::SHOW_STATE << detail.is_show_
			<< Fashion_Fields::FASHION_PART << item_bson;

	MONGO_CONNECTION.update(Fashion_Fields::COLLECTION,
			QUERY(Fashion_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_chat_control_info(Chat_Control_Info &info) {
	info.reset();
	BSONObj query_obj = BSON(Ban_Gag_Fields::UNBAN_TIME << BSON("$gt" << (long long int)Time_Value::gettimeofday().sec()));
		unsigned long long res_count = MONGO_CONNECTION.count(Ban_Gag_Fields::COLLECTION, query_obj);
		BSONObjBuilder field_builder;
		field_builder 	<< Ban_Gag_Fields::ACCOUNT << 1
						<< Ban_Gag_Fields::ROLE_ID << 1
						<< Ban_Gag_Fields::UNBAN_TIME << 1;
		BSONObj field_bson = field_builder.obj();
		vector<BSONObj> iter_record;
		iter_record.reserve(res_count);
		MONGO_CONNECTION.findN(iter_record, Ban_Gag_Fields::COLLECTION, mongo::Query(query_obj), res_count, 0, &field_bson);
		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
			BSONObj &obj = *iter;
			long long int time = obj[Ban_Gag_Fields::UNBAN_TIME].numberLong();
			std::string account =  obj[Ban_Gag_Fields::ACCOUNT].valuestrsafe();
			role_id_t role_id = obj[Ban_Gag_Fields::ROLE_ID].numberLong();
			if(role_id) {
				info.role_id_gag[role_id] = Time_Value(time);
			}
			if(!account.empty()) {
				info.account_gag[account] = Time_Value(time);
			}
		}
	return 0;
}

int DB_Operator::load_ann_info(GM_Ann_Info_Map &ann_info_map) {
	BSONObj query_obj = BSON( Gm_Ann_Fields::TYPE << GM_ANN_SET_STEP << Gm_Ann_Fields::STATUS << 0
			<< Gm_Ann_Fields::ENDTIME << BSON("$gt" << (long long int)Time_Value::gettimeofday().sec()));
		unsigned long long res_count = MONGO_CONNECTION.count(Gm_Ann_Fields::COLLECTION, query_obj);
		BSONObjBuilder field_builder;
		field_builder 	<< Gm_Ann_Fields::ID << 1
						<< Gm_Ann_Fields::STIME << 1
						<< Gm_Ann_Fields::ENDTIME << 1
						<< Gm_Ann_Fields::STEP << 1
						<< Gm_Ann_Fields::CONTENTS << 1;
		BSONObj field_bson = field_builder.obj();
		vector<BSONObj> iter_record;
		iter_record.reserve(res_count);
		GM_Ann_Info info;
		MONGO_CONNECTION.findN(iter_record, Gm_Ann_Fields::COLLECTION, mongo::Query(query_obj), res_count, 0, &field_bson);
		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
			BSONObj &obj = *iter;
			info.reset();
			info.id = obj[Gm_Ann_Fields::ID].numberInt();
			info.stime.sec(obj[Gm_Ann_Fields::STIME].numberLong());
			info.etime.sec(obj[Gm_Ann_Fields::ENDTIME].numberLong());
			info.step = obj[Gm_Ann_Fields::STEP].numberInt();
			info.contents = obj[Gm_Ann_Fields::CONTENTS].valuestrsafe();
			ann_info_map.insert(std::make_pair(info.id, info));
		}
	return 0;
}

int DB_Operator::load_action_goods_data(Aution_Goods_Data_Map &goods_data_map, long long int &max_id) {
	goods_data_map.clear(); max_id = 0;
	unsigned long long res_count = MONGO_CONNECTION.count(Auction_Goods_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder 	<< Auction_Goods_Fields::ID << 1
					<< Auction_Goods_Fields::ITEM_ID << 1
					<< Auction_Goods_Fields::NUM << 1
					<< Auction_Goods_Fields::AUCTION_PRICE << 1
					<< Auction_Goods_Fields::ONES_PRICE << 1
					<< Auction_Goods_Fields::OVER_TIME << 1
					<< Auction_Goods_Fields::START_TIME << 1
					<< Auction_Goods_Fields::CONTINUE_TIME << 1
					<< Auction_Goods_Fields::ISSUE_ROLE_ID << 1
					<< Auction_Goods_Fields::AUCTION_ROLE_ID << 1
					<< Auction_Goods_Fields::L_AUCTION_PRICE << 1
					<< Auction_Goods_Fields::AUCTION_FAIL_ROLE_ID << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, Auction_Goods_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	Auction_Goods_Data value;
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		value.id = obj[Auction_Goods_Fields::ID].numberInt();
		value.item_id = obj[Auction_Goods_Fields::ITEM_ID].numberInt();
		value.num = obj[Auction_Goods_Fields::NUM].numberInt();
		value.auction_price = obj[Auction_Goods_Fields::AUCTION_PRICE].numberInt();
		value.ones_price = obj[Auction_Goods_Fields::ONES_PRICE].numberInt();
		value.over_time.sec(obj[Auction_Goods_Fields::OVER_TIME].numberLong());
		value.start_time.sec(obj[Auction_Goods_Fields::START_TIME].numberLong());
		value.continue_time = obj[Auction_Goods_Fields::CONTINUE_TIME].numberInt();
		value.issue_role_id = obj[Auction_Goods_Fields::ISSUE_ROLE_ID].numberLong();
		value.auction_role_id = obj[Auction_Goods_Fields::AUCTION_ROLE_ID].numberLong();
		value.l_auction_price = obj[Auction_Goods_Fields::L_AUCTION_PRICE].numberInt();
		//value.id = obj[Auction_Goods_Fields::AUCTION_FAIL_ROLE_ID].numberInt();
		BSONObjIterator iter_set(obj.getObjectField(Auction_Goods_Fields::AUCTION_FAIL_ROLE_ID.c_str()));
		while (iter_set.more()) {
			value.auction_fail_role_id.insert(iter_set.next().numberLong());
		}

		goods_data_map.insert(std::make_pair(value.id, value));
		if(value.id > max_id) {
			max_id = value.id;
		}
	}
	return 0;
}

int DB_Operator::save_action_goods_data(Auction_Goods_Data &goods_data) {
	BSONObjBuilder builder;
	std::vector<long long int> auction_fail_role_id_vec;
	auction_fail_role_id_vec.clear();
	for(Role_Id_Set::iterator it = goods_data.auction_fail_role_id.begin(); it != goods_data.auction_fail_role_id.end(); ++it) {
		auction_fail_role_id_vec.push_back(*it);
	}
	builder << Auction_Goods_Fields::ID << goods_data.id
			<< Auction_Goods_Fields::ITEM_ID << goods_data.item_id
			<< Auction_Goods_Fields::NUM << goods_data.num
			<< Auction_Goods_Fields::AUCTION_PRICE << goods_data.auction_price
			<< Auction_Goods_Fields::ONES_PRICE << goods_data.ones_price
			<< Auction_Goods_Fields::OVER_TIME << (long long int)goods_data.over_time.sec()
			<< Auction_Goods_Fields::START_TIME << (long long int)goods_data.start_time.sec()
			<< Auction_Goods_Fields::CONTINUE_TIME << goods_data.continue_time
			<< Auction_Goods_Fields::ISSUE_ROLE_ID << (long long int)goods_data.issue_role_id
			<< Auction_Goods_Fields::AUCTION_ROLE_ID << (long long int)goods_data.auction_role_id
			<< Auction_Goods_Fields::L_AUCTION_PRICE << goods_data.l_auction_price
			<< Auction_Goods_Fields::AUCTION_FAIL_ROLE_ID << auction_fail_role_id_vec
			;
	MONGO_CONNECTION.update(Auction_Goods_Fields::COLLECTION,
				QUERY(Auction_Goods_Fields::ID << (long long int)(goods_data.id)),
				BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::delete_action_goods_data(uint32_t id) {
	MONGO_CONNECTION.remove(Auction_Goods_Fields::COLLECTION, QUERY(Auction_Goods_Fields::ID << id));
	return 0;
}

int DB_Operator::save_knight_trials_detail(const Knight_Trials_Detail &detail) {
	BSONObjBuilder builder;
	std::vector<long long int> role_id_vec;
	role_id_vec.clear();
	for(Role_Id_Set::iterator it = detail.role_id_set.begin(); it != detail.role_id_set.end(); ++it) {
		role_id_vec.push_back(*it);
	}

	std::vector<BSONObj> reward_vec;
	for (std::vector<Uint_Uint>::const_iterator it = detail.reward.begin();
			it != detail.reward.end(); ++it) {
		reward_vec.push_back(BSON(Knight_Trials_Fields::Reward_Info::ID << it->val_1
				<< Knight_Trials_Fields::Reward_Info::VAL << it->val_2));
	}

	std::vector<BSONObj> recode_box_reward;
	recode_box_reward.clear();
	for (Record_Box_Reward_Conf_Map::const_iterator it = detail.recode_box_reward.begin();
			it != detail.recode_box_reward.end(); ++it) {
		std::vector<BSONObj> recode_reward;
		recode_reward.clear();
		for(Record_Box_Reward_List::const_iterator iter = it->second.begin(); iter != it->second.end(); ++iter) {
			recode_reward.push_back(BSON(Knight_Trials_Fields::Reward_Info::ID << iter->id
						<< Knight_Trials_Fields::Reward_Info::VAL << iter->num
						<< Knight_Trials_Fields::Reward_Info::TYPE << iter->type));
		}
		recode_box_reward.push_back(BSON(Knight_Trials_Fields::Reward_Info::ID << it->first
				<< Knight_Trials_Fields::Reward_Info::VAL << recode_reward));
	}

	builder << Knight_Trials_Fields::CUR_POINTS << detail.cur_points
			<< Knight_Trials_Fields::ROLE_ID_SET << role_id_vec
			<< Knight_Trials_Fields::MATCH_DAY << (long long int)detail.match_day
			<< Knight_Trials_Fields::CUR_RECEIVE << detail.cur_receive
			<< Knight_Trials_Fields::RESET_STATUS << detail.reset_status
			<< Knight_Trials_Fields::REWARD << reward_vec
			<< Knight_Trials_Fields::RECODE_REWARD << recode_box_reward
			;
	MONGO_CONNECTION.update(Knight_Trials_Fields::COLLECTION,
				QUERY(Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
				BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_knight_trials_detail(Knight_Trials_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Knight_Trials_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.cur_points = res[Knight_Trials_Fields::CUR_POINTS].numberInt();
	detail.match_day = res[Knight_Trials_Fields::MATCH_DAY].numberLong();
	detail.cur_receive = res[Knight_Trials_Fields::CUR_RECEIVE].numberInt();
	detail.reset_status = res[Knight_Trials_Fields::RESET_STATUS].booleanSafe();

	BSONObjIterator iter_set(res.getObjectField(Knight_Trials_Fields::ROLE_ID_SET.c_str()));
	while (iter_set.more()) {
		detail.role_id_set.insert(iter_set.next().numberLong());
	}
	{
		BSONObjIterator iter(res.getObjectField(Knight_Trials_Fields::REWARD.c_str()));
		Uint_Uint uu;
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uu.reset();
			uu.val_1 = obj[Knight_Trials_Fields::Reward_Info::ID].numberInt();
			uu.val_2 = obj[Knight_Trials_Fields::Reward_Info::VAL].numberInt();
			detail.reward.push_back(uu);
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Knight_Trials_Fields::RECODE_REWARD.c_str()));
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uint8_t points = obj[Knight_Trials_Fields::Reward_Info::ID].numberInt();

			Record_Box_Reward_List list;
			list.clear();
			BSONObjIterator iter_list(obj.getObjectField(Knight_Trials_Fields::Reward_Info::VAL.c_str()));
			Record_Box_Reward rbr;
			while (iter_list.more()) {
				rbr.reset();
				BSONObj obj_list = iter_list.next().embeddedObject();
				rbr.id = obj_list[Knight_Trials_Fields::Reward_Info::ID].numberInt();
				rbr.num = obj_list[Knight_Trials_Fields::Reward_Info::VAL].numberInt();
				rbr.type = obj_list[Knight_Trials_Fields::Reward_Info::TYPE].numberInt();
				list.push_back(rbr);
			}

			detail.recode_box_reward.insert(std::make_pair(points,list));
		}
	}

	return 0;
}

int DB_Operator::save_knight_trials_heroer_detail(Heroer_Detail &detail, mongo::BSONObj &obj) {
	Int_Vec clicked_new_heros(detail.clicked_new_heros.begin(), detail.clicked_new_heros.end());

	std::vector<BSONObj> hero_infos;
	for (Heroer_Detail::Hero_Map::iterator it = detail.hero_map.begin();
			it != detail.hero_map.end(); ++it) {

		std::vector<BSONObj> artifact_bson_vec;
		for (Artifact_Map::const_iterator it_artifact = it->second.artifacts.begin();
				it_artifact != it->second.artifacts.end(); ++it_artifact) {
			artifact_bson_vec.push_back(BSON(
					Heroer_Fields::Hero_Info::Artifact::ARTIFACT_ID << it_artifact->second.artifact_id
					<< Heroer_Fields::Hero_Info::Artifact::ARTIFACT_LVL << it_artifact->second.artifact_lvl
					));
		}

		std::vector<BSONObj> vc_talent;
		for (Talent_Map::iterator tit = it->second.hero_fighter_detail.talent_map.begin();
				tit != it->second.hero_fighter_detail.talent_map.end(); ++tit) {
		    vector<BSONObj> vc_skill;	// skill
		    for (Skill_DB_Info_Map::iterator it = tit->second.skill_db_info_map.begin(); it != tit->second.skill_db_info_map.end(); ++it) {
		    	vc_skill.push_back(BSON(Fighter_Fields::Talent::Skill::SKILL_ID << it->second.skill_id
				<< Fighter_Fields::Talent::Skill::SKILL_LEVEL << it->second.skill_level
				<< Fighter_Fields::Talent::Skill::SkILL_LOC << it->second.skill_loc));
		    }
		    vc_talent.push_back(BSON(Fighter_Fields::Talent::TALENT_ID << tit->second.talent_id
					<< Fighter_Fields::Talent::SKILL_POINTER << tit->second.skill_point
					<< Fighter_Fields::Talent::SKILL << vc_skill));
		}

		BSONObjBuilder hero_info_builder;
		hero_info_builder << Heroer_Fields::Hero_Info::HERO_ID << it->second.hero_id
				<< Heroer_Fields::Hero_Info::HERO_INDEX << it->second.hero_index
				<< Heroer_Fields::Hero_Info::HERO_FACADE_ID << it->second.hero_facade_id
				<< Heroer_Fields::Hero_Info::HERO_NAME << it->second.hero_name
				<< Heroer_Fields::Hero_Info::IS_OUTER << it->second.is_outer
				<< Heroer_Fields::Hero_Info::IS_FIGHTER << it->second.is_fighter
				<< Heroer_Fields::Hero_Info::LEVEL << it->second.level
				<< Heroer_Fields::Hero_Info::EXP << it->second.exp
				<< Heroer_Fields::Hero_Info::FORMATION << it->second.formation
				<< Heroer_Fields::Hero_Info::AWAKE_LVL << it->second.awake_lvl
				<< Heroer_Fields::Hero_Info::IS_TMP_HERO << it->second.is_task_tmp_hero
				<< Heroer_Fields::Hero_Info::COMBAT_POWER << it->second.hero_fighter_detail.fetch_fight_property(PT_FORCE)
				<< Heroer_Fields::Hero_Info::ARTIFACTS << artifact_bson_vec
				<< Fighter_Fields::TALENT << vc_talent
				;

		BSONObj arena_fight_obj;
		handle_save_fighter_detail(it->second.hero_fighter_detail, arena_fight_obj);
		// ------------------- arena start ----------------------
//		BSONObj arena_fight_obj = BSON(Fighter_Fields::Arena_Fight::PT_PHY_ATTACK << it->second.hero_fighter_detail.physical_attack.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_SPELL_ATTACK << it->second.hero_fighter_detail.spell_attack.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE << it->second.hero_fighter_detail.physical_defense.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE << it->second.hero_fighter_detail.spell_defense.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_HIT << it->second.hero_fighter_detail.hit_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_AVOID << it->second.hero_fighter_detail.avoid.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_IGNORE_DEF << it->second.hero_fighter_detail.avoid.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_BLOOD_MAX << it->second.hero_fighter_detail.blood_max.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_CRIT << it->second.hero_fighter_detail.crit.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_CRIT_DEF << it->second.hero_fighter_detail.crit_def.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_CURE << it->second.hero_fighter_detail.cure.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_CURE_RATE << it->second.hero_fighter_detail.cure_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_SING_RATE << it->second.hero_fighter_detail.sing_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_INIT_MORALE << it->second.hero_fighter_detail.init_morale.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_MORALE_CAPS << it->second.hero_fighter_detail.morale_caps.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_CRIT_HURT << it->second.hero_fighter_detail.crit_hurt.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_CRIT_HURT_SUB << it->second.hero_fighter_detail.crit_hurt_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_HIT_RATE_ADDITION << it->second.hero_fighter_detail.hit_rate_addition.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_AVOID_RATE_ADDITION << it->second.hero_fighter_detail.avoid_rate_addition.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_CRIT_RATE_ADDITION << it->second.hero_fighter_detail.crit_rate_addition.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_CRIT_DEF_RATE_ADDITION << it->second.hero_fighter_detail.crit_def_rate_addition.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_SPEED << it->second.hero_fighter_detail.speed.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_BLOOD_CURRENT << it->second.hero_fighter_detail.blood_current.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_MORALE_CURRENT << it->second.hero_fighter_detail.morale_current.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_STRENGTH_CURRENT << it->second.hero_fighter_detail.strength_current.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_STRENGTH_MAX << it->second.hero_fighter_detail.strength_max.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_CURRENT << it->second.hero_fighter_detail.blood_pool_current.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_MAX << it->second.hero_fighter_detail.blood_pool_max.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_IGNORE_DEF_RATE << it->second.hero_fighter_detail.ignore_def_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_STATUS_HIT_RATE << it->second.hero_fighter_detail.status_hit_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_STATUS_DEF_RATE << it->second.hero_fighter_detail.status_def_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_BE_CURE_RATE << it->second.hero_fighter_detail.be_cure_rate.total(Prop_Value::ELEM_NORMAL)
//
//				<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_FIX << it->second.hero_fighter_detail.phy_hurt_fix.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE << it->second.hero_fighter_detail.phy_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_FIX << it->second.hero_fighter_detail.spell_hurt_fix.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_BE_HURT_RATE << it->second.hero_fighter_detail.spell_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE << it->second.hero_fighter_detail.hurt_fix.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_HURT_FIX << it->second.hero_fighter_detail.hurt_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_HURT_RATE << it->second.hero_fighter_detail.be_hurt_fix.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_BE_HURT_FIX << it->second.hero_fighter_detail.be_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_BE_HURT_RATE << it->second.hero_fighter_detail.spell_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//
//				<< Fighter_Fields::Arena_Fight::PT_EFFECT_RATE << it->second.hero_fighter_detail.effect_rate.total(Prop_Value::ELEM_NORMAL)
//
//				<< Fighter_Fields::Arena_Fight::PT_PK_HURT_ADD << it->second.hero_fighter_detail.pk_hurt_add.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_PK_HURT_SUB << it->second.hero_fighter_detail.pk_hurt_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE_SUB << it->second.hero_fighter_detail.phy_hurt_rate_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE_SUB << it->second.hero_fighter_detail.spell_hurt_rate_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_SKILL_HURT_RATE << it->second.hero_fighter_detail.skill_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_HUMAN_ADD << it->second.hero_fighter_detail.human_add.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_ELF_ADD << it->second.hero_fighter_detail.elf_add.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_ORC_ADD << it->second.hero_fighter_detail.orc_add.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_DRAGON_ADD << it->second.hero_fighter_detail.dragon_add.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_UNDEAD_ADD << it->second.hero_fighter_detail.undead_add.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_DEMON_ADD << it->second.hero_fighter_detail.demon_add.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_HUMAN_SUB << it->second.hero_fighter_detail.human_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_ELF_SUB << it->second.hero_fighter_detail.elf_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_ORC_SUB << it->second.hero_fighter_detail.orc_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_DRAGON_SUB << it->second.hero_fighter_detail.dragon_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_UNDEAD_SUB << it->second.hero_fighter_detail.undead_sub.total(Prop_Value::ELEM_NORMAL)
//				<< Fighter_Fields::Arena_Fight::PT_DEMON_SUB << it->second.hero_fighter_detail.demon_sub.total(Prop_Value::ELEM_NORMAL)
//
//				<< Fighter_Fields::Arena_Fight::FORCE << it->second.hero_fighter_detail.force
//				);
		hero_info_builder.append(Fighter_Fields::ARENA_FIGHT, arena_fight_obj);
		// ------------------- arena end ----------------------

		hero_infos.push_back(hero_info_builder.obj());
	}

	BSONObjBuilder builder;
	builder << Heroer_Fields::ROLE_ID << (long long int)detail.role_id
			<< Heroer_Fields::OUTER_INDEX << detail.outer_index
			<< Heroer_Fields::FIGHTER_INDEX << detail.fighter_index
			<< Heroer_Fields::LAST_FIGHTER_IDX << detail.last_fighter_idx
			<< Heroer_Fields::CLICKED_NEW_HEROS << clicked_new_heros
			<< Heroer_Fields::HERO_INFOS << hero_infos;

	obj = builder.obj();
	return 0;
}

int DB_Operator::save_knight_trials_fighter_detail(Fighter_Detail &detail, mongo::BSONObj &obj) {
	std::vector<BSONObj> vc_talent;
	for (Talent_Map::iterator tit = detail.talent_map.begin(); tit != detail.talent_map.end(); ++tit) {
	    // skill
	    vector<BSONObj> vc_skill;
	    for (Skill_DB_Info_Map::iterator it = tit->second.skill_db_info_map.begin(); it != tit->second.skill_db_info_map.end(); ++it) {
	    	vc_skill.push_back(BSON(Fighter_Fields::Talent::Skill::SKILL_ID << it->second.skill_id
			<< Fighter_Fields::Talent::Skill::SKILL_LEVEL << it->second.skill_level
			<< Fighter_Fields::Talent::Skill::SkILL_LOC << it->second.skill_loc));
	    }
	    vc_talent.push_back(BSON(Fighter_Fields::Talent::TALENT_ID << tit->second.talent_id
				<< Fighter_Fields::Talent::SKILL_POINTER << tit->second.skill_point
				<< Fighter_Fields::Talent::SKILL << vc_skill));
	}

	BSONObjBuilder builder;
	builder << Fighter_Fields::ROLE_ID << (long long int)detail.role_id
			<< Fighter_Fields::PHY_POWER_RECOVER_TIME << detail.phy_power_recover_time
			<< Fighter_Fields::PHY_POWER_BUY_TIMES << detail.phy_power_buy_times
			<< Fighter_Fields::LEVEL << detail.level
			<< Fighter_Fields::TTL_SKILL_POINTER << detail.ttl_skill_points_
			<< Fighter_Fields::CURRENT_BLOOD << detail.current_blood()
			<< Fighter_Fields::CURRENT_MORALE << detail.current_morale()
			<< Fighter_Fields::CURRENT_STRENGTH << detail.current_strength()
			<< Fighter_Fields::CURRENT_BLOOD_POOL << detail.current_blood_pool()
			<< Fighter_Fields::EXPERIENCE << detail.experience
			<< Fighter_Fields::FORCE << detail.force
			<< Fighter_Fields::HIGHEST_FORCE << detail.highest_force
			<< Fighter_Fields::TTL_FORCE << detail.ttl_force
			<< Fighter_Fields::DEFAULT_SKILL << detail.default_skill
			<< Fighter_Fields::IS_CLEAR_MORAL << detail.is_clear_moral_
			<< Fighter_Fields::CUR_USED_TALENT << detail.cur_used_talent_
			<< Fighter_Fields::RESET_SKILL_TIMES << detail.reset_skill_times
			<< Fighter_Fields::TALENT << vc_talent;

//	BSONObjBuilder attr_builder;
//	save_fighter_attr(attr_builder, detail);
//
//	builder.append(Fighter_Fields::ATTRIBUTE, attr_builder.obj());

	BSONObj arena_fight_obj;
	handle_save_fighter_detail(detail, arena_fight_obj);
//	BSONObj arena_fight_obj = BSON(
//								Fighter_Fields::Arena_Fight::PT_PHY_ATTACK << detail.physical_attack.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_SPELL_ATTACK << detail.spell_attack.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE << detail.physical_defense.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE << detail.spell_defense.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_HIT << detail.hit_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_AVOID << detail.avoid.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_IGNORE_DEF << detail.ignore_def.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_BLOOD_MAX << detail.blood_max.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_CRIT << detail.crit.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_CRIT_DEF << detail.crit_def.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_CURE << detail.cure.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_CURE_RATE << detail.cure_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_SING_RATE << detail.sing_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_INIT_MORALE << detail.init_morale.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_MORALE_CAPS << detail.morale_caps.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_CRIT_HURT << detail.crit_hurt.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_CRIT_HURT_SUB << detail.crit_hurt_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_HIT_RATE_ADDITION << detail.hit_rate_addition.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_AVOID_RATE_ADDITION << detail.avoid_rate_addition.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_CRIT_RATE_ADDITION << detail.crit_rate_addition.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_CRIT_DEF_RATE_ADDITION << detail.crit_def_rate_addition.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_SPEED << detail.speed.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_MORALE_CURRENT << detail.morale_current.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_STRENGTH_CURRENT << detail.strength_current.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_STRENGTH_MAX << detail.strength_max.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_CURRENT << detail.blood_pool_current.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_MAX << detail.blood_pool_max.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_IGNORE_DEF_RATE << detail.ignore_def_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_STATUS_HIT_RATE << detail.status_hit_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_STATUS_DEF_RATE << detail.status_def_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_BE_CURE_RATE << detail.be_cure_rate.total(Prop_Value::ELEM_NORMAL)
//
//							<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_FIX << detail.phy_hurt_fix.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE << detail.phy_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_FIX << detail.spell_hurt_fix.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_BE_HURT_RATE << detail.spell_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE << detail.hurt_fix.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_HURT_FIX << detail.hurt_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_HURT_RATE << detail.be_hurt_fix.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_BE_HURT_FIX << detail.be_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_BE_HURT_RATE << detail.spell_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//
//							<< Fighter_Fields::Arena_Fight::PT_EFFECT_RATE << detail.effect_rate.total(Prop_Value::ELEM_NORMAL)
//
//							<< Fighter_Fields::Arena_Fight::PT_PK_HURT_ADD << detail.pk_hurt_add.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_PK_HURT_SUB << detail.pk_hurt_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE_SUB << detail.phy_hurt_rate_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE_SUB << detail.spell_hurt_rate_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_SKILL_HURT_RATE << detail.skill_hurt_rate.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_HUMAN_ADD << detail.human_add.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_ELF_ADD << detail.elf_add.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_ORC_ADD << detail.orc_add.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_DRAGON_ADD << detail.dragon_add.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_UNDEAD_ADD << detail.undead_add.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_DEMON_ADD << detail.demon_add.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_HUMAN_SUB << detail.human_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_ELF_SUB << detail.elf_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_ORC_SUB << detail.orc_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_DRAGON_SUB << detail.dragon_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_UNDEAD_SUB << detail.undead_sub.total(Prop_Value::ELEM_NORMAL)
//							<< Fighter_Fields::Arena_Fight::PT_DEMON_SUB << detail.demon_sub.total(Prop_Value::ELEM_NORMAL)
//	);
	builder.append(Fighter_Fields::ARENA_FIGHT, arena_fight_obj);
	obj = builder.obj();
	return 0;
}

int DB_Operator::save_knight_trials_base_detail(Base_Detail &detail, mongo::BSONObj &obj) {
	BSONObjBuilder builder;
	builder << Role_Fields::ROLE_ID << (long long int)detail.role_id
			<< Role_Fields::ROLE_NAME << detail.role_name
			<< Role_Fields::LEVEL << detail.level
			<< Role_Fields::GENDER << detail.gender
			<< Role_Fields::CAREER << detail.career
			<< Role_Fields::VIP << detail.vip
			<< Role_Fields::VIP_OVER_TIME << (long long int)detail.vip_over_time.sec()
//			<< Role_Fields::CREATE_TIME << (long long int)detail.create_time.sec()
//			<< Role_Fields::LAST_SIGN_IN_TIME << (long long int)detail.last_sign_in_time.sec()
//			<< Role_Fields::LAST_SIGN_OUT_TIME << (long long int)detail.last_sign_out_time.sec()
//			<< Role_Fields::LAST_TRIGGER_DAILY_ZERO << (long long int)detail.last_trigger_daily_zero.sec()
//			<< Role_Fields::LAST_TRIGGER_DAILY_SIX << (long long int)detail.last_trigger_daily_six.sec()
//			<< Role_Fields::LAST_TRIGGER_WEEKLY_ZERO << (long long int)detail.last_trigger_weekly_zero.sec()
			<< Role_Fields::TITLE_ID << detail.title_id
			<< Role_Fields::ONLINE << detail.online
			<< Role_Fields::IP << detail.ip
			<< Role_Fields::HEAD_ID << detail.head_id
			<< Role_Fields::CHARM << detail.charm
			<< Role_Fields::GANG_ID << (long long int)detail.gang_id
			<< Role_Fields::GANG_NAME << detail.gang_name
			<< Role_Fields::AWAKE_LVL << detail.awake_lvl
			<< Role_Fields::AVATAR << detail.avatar_vec
			<< Role_Fields::MOUNT_ID << detail.mount_model_id
			;

	BSONObj account_obj = BSON( Role_Fields::Account_Info::ACCOUNT_NAME << detail.account_info.account_name
							<< Role_Fields::Account_Info::AGENT_NUM << detail.account_info.agent_num
							<< Role_Fields::Account_Info::SERVER_NUM << detail.account_info.server_num);
	builder.append(Role_Fields::ACCOUNT_INFO, account_obj);

//	BSONObj location_obj = BSON( Role_Fields::Location::SCENE_ID << detail.location.scene_id
//							<< Role_Fields::Location::SCENE_ORDER << detail.location.scene_order
//							<< Role_Fields::Location::SCENE_LAYER << detail.location.scene_layer
//							<< Role_Fields::Location::COORD_X << detail.location.coord.x
//							<< Role_Fields::Location::COORD_Y << detail.location.coord.y
//							<< Role_Fields::Location::COORD_TYPE << detail.location.coord.type
//							<< Role_Fields::Location::TOWARD << detail.location.toward);
//	builder.append(Role_Fields::LOCATION, location_obj);
	obj = builder.obj();
	return 0;
}

//int DB_Operator::save_knight_trials_player_detail(Knight_Trials_Player_Detail &detail) {
//	BSONObjBuilder all_builder;
//
//	vector<BSONObj> cur_list;
//	cur_list.clear();
//	for(Player_Fighter_Info_Vec::iterator iter = detail.cur_player_fighter_vec.begin(); iter != detail.cur_player_fighter_vec.end(); ++iter) {
//		BSONObjBuilder builder;
//
//		mongo::BSONObj base_obj;
//		save_knight_trials_base_detail(iter->base, base_obj);
//
//		mongo::BSONObj fighter_obj;
//		save_knight_trials_fighter_detail(iter->fighter, fighter_obj);
//
//		mongo::BSONObj heroer_obj;
//		save_knight_trials_heroer_detail(iter->heroer, heroer_obj);
//
//		builder << Knight_Trials_Player_Fields::Player_Detail::BASE << base_obj
//				<< Knight_Trials_Player_Fields::Player_Detail::FIGHTER << fighter_obj
//				<< Knight_Trials_Player_Fields::Player_Detail::HEROER << heroer_obj
//				<< Knight_Trials_Player_Fields::Player_Detail::UUID << (long long int)iter->uuid
//				<< Knight_Trials_Player_Fields::Player_Detail::SERVER_NAME << iter->server_name
//			    ;
//		cur_list.push_back(builder.obj());
//	}
//
//	vector<BSONObj> hide_list;
//	hide_list.clear();
//	for(Player_Fighter_Info_Vec::iterator iter = detail.hide_player_fighter_vec.begin(); iter != detail.hide_player_fighter_vec.end(); ++iter) {
//		BSONObjBuilder builder;
//
//		mongo::BSONObj base_obj;
//		save_knight_trials_base_detail(iter->base, base_obj);
//
//		mongo::BSONObj fighter_obj;
//		save_knight_trials_fighter_detail(iter->fighter, fighter_obj);
//
//		mongo::BSONObj heroer_obj;
//		save_knight_trials_heroer_detail(iter->heroer, heroer_obj);
//
//		builder << Knight_Trials_Player_Fields::Player_Detail::BASE << base_obj
//				<< Knight_Trials_Player_Fields::Player_Detail::FIGHTER << fighter_obj
//				<< Knight_Trials_Player_Fields::Player_Detail::HEROER << heroer_obj
//				<< Knight_Trials_Player_Fields::Player_Detail::UUID << (long long int)iter->uuid
//				<< Knight_Trials_Player_Fields::Player_Detail::SERVER_NAME << iter->server_name
//			    ;
//		hide_list.push_back(builder.obj());
//	}
//
//	all_builder << Knight_Trials_Player_Fields::CUR_PLAYER << cur_list
//				<< Knight_Trials_Player_Fields::HIDE_PLAYER << hide_list;
//
//	MONGO_CONNECTION.update(Knight_Trials_Player_Fields::COLLECTION,
//					QUERY(Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
//					BSON("$set" << all_builder.obj()), true);
//	return 0;
//}

int DB_Operator::push_cur_knight_trials_player(role_id_t role_id, Knight_Trials_Player &player, bool clear) {
		if(clear) {
			BSONObjBuilder all_builder;
			vector<BSONObj> cur_list;
			cur_list.clear();
			all_builder << Knight_Trials_Player_Fields::CUR_PLAYER << cur_list;

			MONGO_CONNECTION.update(Knight_Trials_Player_Fields::COLLECTION,
							QUERY(Role_Fields::ROLE_ID << (long long int)(role_id)),
							BSON("$set" << all_builder.obj()), true);
		}
		BSONObjBuilder builder;

		mongo::BSONObj base_obj;
		save_knight_trials_base_detail(player.base, base_obj);

		mongo::BSONObj fighter_obj;
		save_knight_trials_fighter_detail(player.fighter, fighter_obj);

		mongo::BSONObj heroer_obj;
		save_knight_trials_heroer_detail(player.heroer, heroer_obj);

		builder << Knight_Trials_Player_Fields::Player_Detail::BASE << base_obj
				<< Knight_Trials_Player_Fields::Player_Detail::FIGHTER << fighter_obj
				<< Knight_Trials_Player_Fields::Player_Detail::HEROER << heroer_obj
				<< Knight_Trials_Player_Fields::Player_Detail::UUID << (long long int)player.uuid
				<< Knight_Trials_Player_Fields::Player_Detail::SERVER_NAME << player.server_name
			    ;
	BSONObjBuilder all_builder;
	all_builder << Knight_Trials_Player_Fields::CUR_PLAYER << builder.obj();

	MONGO_CONNECTION.update(Knight_Trials_Player_Fields::COLLECTION,
					QUERY(Role_Fields::ROLE_ID << (long long int)(role_id)),
					BSON("$push" << all_builder.obj()), true);
	return 0;
}

int DB_Operator::push_hide_knight_trials_player(role_id_t role_id, Knight_Trials_Player &player, bool clear) {
	if(clear) {
		BSONObjBuilder all_builder;
		vector<BSONObj> cur_list;
		cur_list.clear();
		all_builder << Knight_Trials_Player_Fields::HIDE_PLAYER << cur_list;

		MONGO_CONNECTION.update(Knight_Trials_Player_Fields::COLLECTION,
						QUERY(Role_Fields::ROLE_ID << (long long int)(role_id)),
						BSON("$set" << all_builder.obj()), true);
	}

	BSONObjBuilder builder;

	mongo::BSONObj base_obj;
	save_knight_trials_base_detail(player.base, base_obj);

	mongo::BSONObj fighter_obj;
	save_knight_trials_fighter_detail(player.fighter, fighter_obj);

	mongo::BSONObj heroer_obj;
	save_knight_trials_heroer_detail(player.heroer, heroer_obj);

	builder << Knight_Trials_Player_Fields::Player_Detail::BASE << base_obj
			<< Knight_Trials_Player_Fields::Player_Detail::FIGHTER << fighter_obj
			<< Knight_Trials_Player_Fields::Player_Detail::HEROER << heroer_obj
			<< Knight_Trials_Player_Fields::Player_Detail::UUID << (long long int)player.uuid
			<< Knight_Trials_Player_Fields::Player_Detail::SERVER_NAME << player.server_name
			;
	BSONObjBuilder all_builder;
	all_builder << Knight_Trials_Player_Fields::HIDE_PLAYER << builder.obj();

	MONGO_CONNECTION.update(Knight_Trials_Player_Fields::COLLECTION,
					QUERY(Role_Fields::ROLE_ID << (long long int)(role_id)),
					BSON("$push" << all_builder.obj()), true);
	return 0;
}

void DB_Operator::insert_knight_trials_player_role_info_vec(Knight_Trials_Role_Info_Vec &vec, Knight_Trials_Role_Info &value) {
		bool not_find = true;
		for(Knight_Trials_Role_Info_Vec::iterator it = vec.begin();
				it != vec.end(); ++it) {
			if((it->force+it->hero_force) > (value.force+value.hero_force)) {
				vec.insert(it, value);
				not_find = false;
				break;
			}
		}
		if(vec.size() == 0 || not_find) {
			vec.push_back(value);
		}
}

int DB_Operator::reset_kinght_trials_player(role_id_t role_id) {
	BSONObj res = MONGO_CONNECTION.findOne(Knight_Trials_Player_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
	if (res.isEmpty()) {
		return -1;
	}
	{//隐藏列表提到当前列表
		BSONObjBuilder all_builder;
		BSONObj hide_list = res.getObjectField(Knight_Trials_Player_Fields::HIDE_PLAYER.c_str());
		all_builder << Knight_Trials_Player_Fields::CUR_PLAYER << hide_list;
		MONGO_CONNECTION.update(Knight_Trials_Player_Fields::COLLECTION,
						QUERY(Role_Fields::ROLE_ID << (long long int)(role_id)),
						BSON("$set" << all_builder.obj()), true);
	}
	{//清空隐藏列表
		BSONObjBuilder all_builder;
		vector<BSONObj> cur_list;
		cur_list.clear();
		all_builder << Knight_Trials_Player_Fields::HIDE_PLAYER << cur_list;

		MONGO_CONNECTION.update(Knight_Trials_Player_Fields::COLLECTION,
						QUERY(Role_Fields::ROLE_ID << (long long int)(role_id)),
						BSON("$set" << all_builder.obj()), true);
	}
	return 0;
}

int DB_Operator::find_cur_kinght_trials_player(role_id_t role_id, role_id_t player_id, Knight_Trials_Player &player) {
	BSONObj res = MONGO_CONNECTION.findOne(Knight_Trials_Player_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
	if (res.isEmpty()) {
		return -1;
	}
	{
		BSONObj cur_list = res.getObjectField(Knight_Trials_Player_Fields::CUR_PLAYER.c_str());
		BSONObjIterator iter(cur_list);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			player.reset();
			player.uuid = obj[Knight_Trials_Player_Fields::Player_Detail::UUID].numberLong();
			player.server_name = obj[Knight_Trials_Player_Fields::Player_Detail::SERVER_NAME].valuestrsafe();

			BSONObj base =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::BASE.c_str());
			load_knight_trials_base_detail(player.base, base);

			if(player_id != player.base.role_id) {
				player.reset();
				continue;
			}

			BSONObj fighter =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::FIGHTER.c_str());
			load_knight_trials_fighter_detail(player.fighter, fighter);

			BSONObj heroer =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::HEROER.c_str());
			load_knight_trials_heroer_detail(player.heroer, heroer);
			if(player_id == player.base.role_id) {
				return 0;
			}
		}
	}
	return -1;
}

int DB_Operator::load_knight_trials_player_detail(Knight_Trials_Player_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Knight_Trials_Player_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	{
		BSONObj cur_list = res.getObjectField(Knight_Trials_Player_Fields::CUR_PLAYER.c_str());
		BSONObjIterator iter(cur_list);
		detail.cur_player_fighter_vec.clear();
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			Knight_Trials_Player player;
			player.reset();

			player.uuid = obj[Knight_Trials_Player_Fields::Player_Detail::UUID].numberLong();
			player.server_name = obj[Knight_Trials_Player_Fields::Player_Detail::SERVER_NAME].valuestrsafe();

			BSONObj base =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::BASE.c_str());
			load_knight_trials_base_detail(player.base, base);

			BSONObj fighter =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::FIGHTER.c_str());
			load_knight_trials_fighter_detail(player.fighter, fighter);

			BSONObj heroer =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::HEROER.c_str());
			load_knight_trials_heroer_detail(player.heroer, heroer);

			Knight_Trials_Role_Info value;
			value.role_id = player.base.role_id;
			value.role_name = player.base.role_name;
			value.head_id = player.base.head_id;
			value.gender = player.base.gender;
			value.career = player.base.career;
			value.level = player.base.level;
			{//vip
				Time_Value now = Time_Value::gettimeofday();
				if(now.sec() > player.base.vip_over_time.sec()) {
					value.vip = player.base.vip;
				} else {
					value.vip = 0;
				}
			}
			value.server_name = player.server_name;
			value.force = player.fighter.force;
			{//hero
				if(player.heroer.fighter_index) {
					Heroer_Detail::Hero_Map::iterator hero_it = player.heroer.hero_map.find(player.heroer.fighter_index);
					if(hero_it != player.heroer.hero_map.end()) {
						value.hero_id = hero_it->second.hero_id;
						value.hero_force = hero_it->second.hero_fighter_detail.force;
						value.hero_level = hero_it->second.level;
						value.hero_name = hero_it->second.hero_name;
					}
				}
			}
			value.mount_id = player.base.mount_model_id;
			value.fashion = player.base.avatar_vec;
			value.gang_name =player.base.gang_name;
			//detail.cur_player_fighter_vec.push_back(value);
			insert_knight_trials_player_role_info_vec(detail.cur_player_fighter_vec, value);

		}
	}
	{
		BSONObj hide_list = res.getObjectField(Knight_Trials_Player_Fields::HIDE_PLAYER.c_str());
		BSONObjIterator iter(hide_list);
		detail.hide_player_fighter_vec.clear();
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			Knight_Trials_Player player;
			player.reset();

			player.uuid = obj[Knight_Trials_Player_Fields::Player_Detail::UUID].numberLong();
			player.server_name = obj[Knight_Trials_Player_Fields::Player_Detail::SERVER_NAME].valuestrsafe();

			BSONObj base =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::BASE.c_str());
			load_knight_trials_base_detail(player.base, base);

			BSONObj fighter =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::FIGHTER.c_str());
			load_knight_trials_fighter_detail(player.fighter, fighter);

			BSONObj heroer =  obj.getObjectField(Knight_Trials_Player_Fields::Player_Detail::HEROER.c_str());
			load_knight_trials_heroer_detail(player.heroer, heroer);

			Knight_Trials_Role_Info value;
			value.role_id = player.base.role_id;
			value.role_name = player.base.role_name;
			value.head_id = player.base.head_id;
			value.gender = player.base.gender;
			value.career = player.base.career;
			value.level = player.base.level;
			{//vip
				Time_Value now = Time_Value::gettimeofday();
				if(now.sec() > player.base.vip_over_time.sec()) {
					value.vip = player.base.vip;
				} else {
					value.vip = 0;
				}
			}
			value.server_name = player.server_name;
			value.force = player.fighter.force;
			{//hero
				if(player.heroer.fighter_index) {
					Heroer_Detail::Hero_Map::iterator hero_it = player.heroer.hero_map.find(player.heroer.fighter_index);
					if(hero_it != player.heroer.hero_map.end()) {
						value.hero_id = hero_it->second.hero_id;
						value.hero_force = hero_it->second.hero_fighter_detail.force;
						value.hero_level = hero_it->second.level;
						value.hero_name = hero_it->second.hero_name;
					}
				}
			}
			value.mount_id = player.base.mount_model_id;
			value.fashion = player.base.avatar_vec;
			value.gang_name =player.base.gang_name;
			//detail.hide_player_fighter_vec.push_back(value);
			insert_knight_trials_player_role_info_vec(detail.hide_player_fighter_vec, value);
		}
	}
	return 0;
}

int DB_Operator::load_knight_trials_base_detail(Base_Detail &detail, mongo::BSONObj &res) {
	if (! res.isEmpty()) {
		detail.role_id = res[Role_Fields::ROLE_ID].numberLong();
    	if (res.hasField(Role_Fields::ACCOUNT_INFO.c_str())) {
    		detail.account_info.account_name = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::ACCOUNT_NAME].valuestrsafe();
    		detail.account_info.agent_num = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::AGENT_NUM].numberInt();
    		detail.account_info.server_num = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::SERVER_NUM].numberInt();
    	}

//    	if (res.hasField(Role_Fields::LOCATION.c_str())) {
//    		detail.location.scene_id = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_ID].numberInt();
//    		detail.location.scene_order = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_ORDER].numberInt();
//    		detail.location.scene_layer = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_LAYER].numberInt();
//    		detail.location.coord.x = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_X].numberInt();
//    		detail.location.coord.y = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_Y].numberInt();
//    		detail.location.coord.type = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_TYPE].numberInt();
//    	}


		detail.role_name = res[Role_Fields::ROLE_NAME].valuestrsafe();
		detail.level = res[Role_Fields::LEVEL].numberInt();
		detail.gender = res[Role_Fields::GENDER].numberInt();
		detail.career = res[Role_Fields::CAREER].numberInt();
		detail.vip = res[Role_Fields::VIP].numberInt();
		detail.vip_over_time.sec(res[Role_Fields::VIP_OVER_TIME].numberInt());
//		detail.create_time.sec(res[Role_Fields::CREATE_TIME].numberInt());
//		detail.last_sign_in_time.sec(res[Role_Fields::LAST_SIGN_IN_TIME].numberInt());
//		detail.last_sign_out_time.sec(res[Role_Fields::LAST_SIGN_OUT_TIME].numberInt());
//		detail.last_trigger_daily_zero.sec(res[Role_Fields::LAST_TRIGGER_DAILY_ZERO].numberInt());
//		detail.last_trigger_daily_six.sec(res[Role_Fields::LAST_TRIGGER_DAILY_SIX].numberInt());
//		detail.last_trigger_weekly_zero.sec(res[Role_Fields::LAST_TRIGGER_WEEKLY_ZERO].numberInt());
		detail.title_id = res[Role_Fields::TITLE_ID].numberInt();
		detail.head_id = res[Role_Fields::HEAD_ID].numberInt();
		detail.charm = res[Role_Fields::CHARM].numberInt();
		detail.awake_lvl = res[Role_Fields::AWAKE_LVL].numberInt();
		detail.mount_model_id = res[Role_Fields::MOUNT_ID].numberInt();

		BSONObjIterator iter_set(res.getObjectField(Role_Fields::AVATAR.c_str()));
		while (iter_set.more()) {
			detail.avatar_vec.push_back(iter_set.next().numberInt());
		}

		return 0;
	} else {
		return -1;
	}
}

int DB_Operator::load_knight_trials_fighter_detail(Fighter_Detail &detail, mongo::BSONObj &res) {
	if (! res.isEmpty()) {
		detail.force = 0;
		detail.role_id = res[Fighter_Fields::ROLE_ID].numberLong();
		detail.level = res[Fighter_Fields::LEVEL].numberLong();
		detail.force = res[Fighter_Fields::FORCE].numberInt();
		detail.ttl_force = res[Fighter_Fields::TTL_FORCE].numberInt();
		detail.default_skill = res[Fighter_Fields::DEFAULT_SKILL].numberInt();
		detail.ttl_skill_points_ = res[Fighter_Fields::TTL_SKILL_POINTER].numberInt();
		detail.blood_current.basic.basic_value = res[Fighter_Fields::CURRENT_BLOOD].numberDouble();
		detail.morale_current.basic.basic_value = res[Fighter_Fields::CURRENT_MORALE].numberDouble();
		detail.modify_normal_property(PT_LEVEL, O_SET, res[Fighter_Fields::LEVEL].numberInt());
		detail.modify_normal_property(PT_EXP_CURRENT, O_SET, res[Fighter_Fields::EXPERIENCE].numberLong());
		detail.strength_current.basic.basic_value = res[Fighter_Fields::CURRENT_STRENGTH].numberDouble();
		detail.blood_pool_current.basic.basic_value = res[Fighter_Fields::CURRENT_BLOOD_POOL].numberDouble();
		detail.cur_used_talent_ = res[Fighter_Fields::CUR_USED_TALENT].numberInt();

		Talent talent;
		BSONObjIterator talent_it(res.getObjectField(Fighter_Fields::TALENT.c_str()));
		while (talent_it.more()) {
			talent.reset();
			BSONObj tp = talent_it.next().embeddedObject();
			talent.talent_id = tp[Fighter_Fields::Talent::TALENT_ID].numberInt();
			talent.skill_point = tp[Fighter_Fields::Talent::SKILL_POINTER].numberInt();

			/// skills
			Skill_DB_Info skill;
			BSONObjIterator skill_bson_it(tp.getObjectField(Fighter_Fields::Talent::SKILL.c_str()));
			while (skill_bson_it.more()) {
				BSONObj obj = skill_bson_it.next().embeddedObject();

				skill.reset();
				skill.skill_id = obj[Fighter_Fields::Talent::Skill::SKILL_ID].numberInt();
				skill.skill_level = obj[Fighter_Fields::Talent::Skill::SKILL_LEVEL].numberInt();
				skill.skill_loc = obj[Fighter_Fields::Talent::Skill::SkILL_LOC].numberInt();
				talent.skill_db_info_map.insert(std::make_pair(skill.skill_id, skill));
			}
			detail.talent_map.insert(std::make_pair(talent.talent_id, talent));
		}

		//arena
		if (res.hasField(Fighter_Fields::ARENA_FIGHT.c_str())) {
			handle_load_fighter_detail(detail, res[Fighter_Fields::ARENA_FIGHT].embeddedObject());
		}
		//arena
		return 0;
	} else {
		return -1;
	}
}

int DB_Operator::load_knight_trials_heroer_detail(Heroer_Detail &detail, mongo::BSONObj &res) {
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Heroer_Fields::ROLE_ID].numberLong();
	detail.outer_index = res[Heroer_Fields::OUTER_INDEX].numberInt();
	detail.fighter_index = res[Heroer_Fields::FIGHTER_INDEX].numberInt();
	detail.last_fighter_idx = res[Heroer_Fields::LAST_FIGHTER_IDX].numberInt();

	Hero_Info_Detail hero_info_detail;
	BSONObjIterator iter(res.getObjectField(Heroer_Fields::HERO_INFOS.c_str()));
	while (iter.more()) {
		BSONObj obj = iter.next().embeddedObject();
		//if (detail.fighter_index == obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt()) {
		//if (detail.outer_index == obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt()) {
		if (detail.fighter_index == obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt()) {
			hero_info_detail.reset();
			hero_info_detail.hero_id = obj[Heroer_Fields::Hero_Info::HERO_ID].numberInt();
			hero_info_detail.hero_index = obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt();
			hero_info_detail.hero_facade_id = obj[Heroer_Fields::Hero_Info::HERO_FACADE_ID].numberInt();
			hero_info_detail.hero_name = obj[Heroer_Fields::Hero_Info::HERO_NAME].valuestrsafe();
			hero_info_detail.is_outer = obj[Heroer_Fields::Hero_Info::IS_OUTER].numberInt();
			hero_info_detail.is_fighter = obj[Heroer_Fields::Hero_Info::IS_FIGHTER].numberInt(); //
			hero_info_detail.level = obj[Heroer_Fields::Hero_Info::LEVEL].numberInt();
			hero_info_detail.exp = obj[Heroer_Fields::Hero_Info::EXP].numberInt();
			hero_info_detail.formation = obj[Heroer_Fields::Hero_Info::FORMATION].numberInt();
			hero_info_detail.awake_lvl = obj[Heroer_Fields::Hero_Info::AWAKE_LVL].numberInt();
			hero_info_detail.is_task_tmp_hero = obj[Heroer_Fields::Hero_Info::IS_TMP_HERO].numberInt();

			Artifact_Detail artifact_detail;
			BSONObjIterator it_artifact(obj.getObjectField(Heroer_Fields::Hero_Info::ARTIFACTS.c_str()));
			while (it_artifact.more()) {
				BSONObj obj_artifact = it_artifact.next().embeddedObject();
				artifact_detail.reset();
				artifact_detail.artifact_id = obj_artifact[Heroer_Fields::Hero_Info::Artifact::ARTIFACT_ID].numberInt();
				artifact_detail.artifact_lvl = obj_artifact[Heroer_Fields::Hero_Info::Artifact::ARTIFACT_LVL].numberInt();
				hero_info_detail.artifacts.insert(std::make_pair(artifact_detail.artifact_id, artifact_detail));
			}

			Talent talent;
			BSONObjIterator talent_it(obj.getObjectField(Fighter_Fields::TALENT.c_str()));
			while (talent_it.more()) {
				talent.reset();
				BSONObj tp = talent_it.next().embeddedObject();
				talent.talent_id = tp[Fighter_Fields::Talent::TALENT_ID].numberInt();
				talent.skill_point = tp[Fighter_Fields::Talent::SKILL_POINTER].numberInt();

				Skill_DB_Info skill;
				BSONObjIterator skill_bson_it(tp.getObjectField(Fighter_Fields::Talent::SKILL.c_str()));
				while (skill_bson_it.more()) {
					BSONObj obj = skill_bson_it.next().embeddedObject();

					skill.reset();
					skill.skill_id = obj[Fighter_Fields::Talent::Skill::SKILL_ID].numberInt();
					skill.skill_level = obj[Fighter_Fields::Talent::Skill::SKILL_LEVEL].numberInt();
					skill.skill_loc = obj[Fighter_Fields::Talent::Skill::SkILL_LOC].numberInt();
					talent.skill_db_info_map.insert(std::make_pair(skill.skill_id, skill));
				}
				hero_info_detail.hero_fighter_detail.talent_map.insert(std::make_pair(talent.talent_id, talent));
				hero_info_detail.hero_fighter_detail.cur_used_talent_ = talent.talent_id;
			}

			//arena
			if (obj.hasField(Fighter_Fields::ARENA_FIGHT.c_str())) {
				hero_info_detail.hero_fighter_detail.force = 0;
				handle_load_fighter_detail(hero_info_detail.hero_fighter_detail, obj[Fighter_Fields::ARENA_FIGHT].embeddedObject());

			}
			//arena

			detail.hero_map.insert(std::make_pair(hero_info_detail.hero_index,hero_info_detail));
		}
	}

	return 0;
}

int DB_Operator::load_anci_artifact_detail(Anci_Artifact_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Anci_Artifact_Fields::COLLECTION, QUERY(Anci_Artifact_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return 0;
	}

	detail.role_id = res[Anci_Artifact_Fields::ROLE_ID].numberLong();
//	detail.perfectPoint = res[Anci_Artifact_Fields::PERFECTPOINT].numberLong();

	BSONObj list = res.getObjectField(Anci_Artifact_Fields::AA_INFO_MAP.c_str());
	BSONObjIterator iter_building(list);
	while (iter_building.more()) {
		BSONObj obj = iter_building.next().embeddedObject();
		Anci_Artifact_Cond_Info info;
		info.reset();
		info.id = (uint32_t)obj[Anci_Artifact_Fields::AA_Info::ID].numberInt();
		info.data = (uint8_t)obj[Anci_Artifact_Fields::AA_Info::DATA].numberInt();
		info.state_count = (uint8_t)obj[Anci_Artifact_Fields::AA_Info::STATE_COUNT].numberInt();

		detail.aa_info_map.insert(std::make_pair(info.id, info));
	}

	int32_t id = 0;
	BSONObj complete_list = res.getObjectField(Anci_Artifact_Fields::AA_COMPLETE_IDS.c_str());
	BSONObjIterator iter_complete(complete_list);
	while (iter_complete.more()) {
		BSONObj obj = iter_complete.next().embeddedObject();
		id = obj[Anci_Artifact_Fields::AA_Info::ID].numberInt();
		detail.complete_ids.insert(id);
	}

	{
		BSONObj list = res.getObjectField(Anci_Artifact_Fields::AA_UNBAN_INFO.c_str());
		BSONObjIterator iter_building(list);
		while (iter_building.more()) {
			BSONObj obj = iter_building.next().embeddedObject();
			Anci_Artifact_Unban_Info info;
			info.reset();
			info.id = (uint32_t)obj[Anci_Artifact_Fields::AA_Unban_Info::ID].numberInt();
			info.state = (uint8_t)obj[Anci_Artifact_Fields::AA_Unban_Info::STATE].numberInt();
			info.box_state = (uint8_t)obj[Anci_Artifact_Fields::AA_Unban_Info::BOX_STATE].numberInt();

			detail.aa_unban_info.insert(std::make_pair(info.id, info));
		}
	}
	return 0;
}

int DB_Operator::save_anci_artifact_detail(Anci_Artifact_Detail &detail) {
    vector<BSONObj> list;
    vector<BSONObj> complete_list;
    vector<BSONObj> unban_info_list;
	for(AA_Info_Map::iterator it = detail.aa_info_map.begin();
			it != detail.aa_info_map.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Anci_Artifact_Fields::AA_Info::ID << (it->second.id)
			<< Anci_Artifact_Fields::AA_Info::DATA << (it->second.data)
			<< Anci_Artifact_Fields::AA_Info::STATE_COUNT << (it->second.state_count);

		list.push_back(building_builder.obj());
	}
	for(Int_Hash_Set::iterator it = detail.complete_ids.begin();
			it != detail.complete_ids.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Anci_Artifact_Fields::AA_Info::ID << (*it);

		complete_list.push_back(building_builder.obj());
	}
	for(AA_Unban_Info::iterator it = detail.aa_unban_info.begin();
			it != detail.aa_unban_info.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Anci_Artifact_Fields::AA_Unban_Info::ID << (it->second.id)
			<< Anci_Artifact_Fields::AA_Unban_Info::STATE << (it->second.state)
			<< Anci_Artifact_Fields::AA_Unban_Info::BOX_STATE << (it->second.box_state);

		unban_info_list.push_back(building_builder.obj());
	}

	BSONObjBuilder builder;
	builder << Anci_Artifact_Fields::ROLE_ID << (long long int)detail.role_id
//			<< Anci_Artifact_Fields::PERFECTPOINT << (long long int)detail.perfectPoint
	        << Anci_Artifact_Fields::AA_INFO_MAP << list
	        << Anci_Artifact_Fields::AA_COMPLETE_IDS << complete_list
	        << Anci_Artifact_Fields::AA_UNBAN_INFO << unban_info_list
	        ;

	MONGO_CONNECTION.update(Anci_Artifact_Fields::COLLECTION, QUERY(Anci_Artifact_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_wing_detail(Wing_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Wing_Fields::COLLECTION, QUERY(Wing_Fields::ROLE_ID <<  (long long int)detail.role_id));
	if (! res.isEmpty()) {
		detail.lv = res[Wing_Fields::LV].numberInt();
		detail.rate = res[Wing_Fields::RATE].numberInt();
		detail.progress = res[Wing_Fields::PROGRESS].numberInt();
		detail.times = res[Wing_Fields::TIMES].numberInt();
		detail.last_time = Time_Value(res[Wing_Fields::LAST_TIME].numberInt());
		detail.is_show = res[Wing_Fields::SHOW_STATE].numberInt();
		detail.state = res[Wing_Fields::STATE].numberInt();
		detail.got_reward_stage = res[Wing_Fields::GOT_REWARD_STAGE].numberInt();
		detail.active_time = Time_Value(res[Wing_Fields::ACTIVE_TIME].numberInt());
	}
	return 0;
}

int DB_Operator::save_wing_detail(Wing_Detail &detail) {
	BSONObjBuilder builder;
	int last_time_sec = detail.last_time.sec();
	int active_time_sec = detail.active_time.sec();
	builder << Wing_Fields::ROLE_ID << (long long int)detail.role_id
			<< Wing_Fields::LV << detail.lv
			<< Wing_Fields::RATE << detail.rate
			<< Wing_Fields::PROGRESS << detail.progress
			<< Wing_Fields::TIMES << detail.times
			<< Wing_Fields::LAST_TIME << last_time_sec
			<< Wing_Fields::SHOW_STATE << detail.is_show
			<< Wing_Fields::STATE << detail.state
			<< Wing_Fields::GOT_REWARD_STAGE << detail.got_reward_stage
			<< Wing_Fields::ACTIVE_TIME << active_time_sec
			;
	MONGO_CONNECTION.update(Wing_Fields::COLLECTION, QUERY(Wing_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_invest_plan_detail(Invest_Plan_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Invest_Plan_Fields::COLLECTION, QUERY(Icon_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Invest_Plan_Fields::ROLE_ID].numberLong();

	detail.is_month_card_invest = (res[Invest_Plan_Fields::IS_MONTH_CARD_INVEST].numberInt() == 1) ? true : false;
	detail.left_day = (uint8_t)res[Invest_Plan_Fields::LEFT_DAY].numberInt();
	detail.days = (uint8_t)res[Invest_Plan_Fields::DAYS].numberInt();

	{
		BSONObj month_card_profit_button_map = res.getObjectField(Invest_Plan_Fields::MONTH_CARD_PROFIT_BUTTON_LIST.c_str());
		BSONObjIterator it(month_card_profit_button_map);
		while (it.more()) {
			BSONObj obj = it.next().embeddedObject();
			Month_Card_Profit_Button month_card_profit_buuton;
			month_card_profit_buuton.day = (uint8_t)obj[Invest_Plan_Fields::Month_Card_Profit_Button::DAY].numberInt();
			month_card_profit_buuton.status = (uint8_t)obj[Invest_Plan_Fields::Month_Card_Profit_Button::STATUS].numberInt();
			detail.month_card_profit_button_map.insert(std::make_pair(month_card_profit_buuton.day, month_card_profit_buuton));
		}
	}

	detail.invested_money = (uint32_t)res[Invest_Plan_Fields::INVESTED_MONEY].numberInt();
	detail.invest_button = (uint8_t)res[Invest_Plan_Fields::INVEST_BUTTON].numberInt();

	{
		BSONObj upgrade_invest_profit_button_map = res.getObjectField(Invest_Plan_Fields::UPGRADE_INVEST_PROFIT_BUTTON_LIST.c_str());
		BSONObjIterator it(upgrade_invest_profit_button_map);
		while(it.more()) {
			BSONObj obj = it.next().embeddedObject();
			Upgrade_Invest_Profit_Button upgrade_invest_profit_button;
			upgrade_invest_profit_button.level = (uint8_t)obj[Invest_Plan_Fields::Upgrade_Invest_Profit_Button::LEVEL].numberInt();
			upgrade_invest_profit_button.status = (uint8_t)obj[Invest_Plan_Fields::Upgrade_Invest_Profit_Button::STATUS].numberInt();
			detail.upgrade_invest_profit_button_map.insert(std::make_pair(upgrade_invest_profit_button.level, upgrade_invest_profit_button));
		}
	}

	{
		BSONObj can_invest_money_set = res.getObjectField(Invest_Plan_Fields::CAN_INVEST_MONEY_LIST.c_str());
		BSONObjIterator it(can_invest_money_set);
		while(it.more()) {
			BSONObj obj = it.next().embeddedObject();
			uint32_t money = (uint32_t)obj[Invest_Plan_Fields::Can_Invest_Money::MONEY].numberInt();
			detail.can_invest_money_set.insert(money);
		}
	}

	{
		BSONObj invested_money_list = res.getObjectField(Invest_Plan_Fields::INVESTED_MONEY_LIST.c_str());
		BSONObjIterator it(invested_money_list);
		while(it.more()) {
			BSONObj obj = it.next().embeddedObject();
			uint32_t money = (uint32_t)obj[Invest_Plan_Fields::Invested_Money::MONEY].numberInt();
			detail.invested_money_list.push_back(money);
		}
	}

	{
		BSONObj getted_upgrade_invest_profit_map = res.getObjectField(Invest_Plan_Fields::GETTED_UPGRADE_INVEST_PROFIT_MAP.c_str());
		BSONObjIterator level_it(getted_upgrade_invest_profit_map);
		while(level_it.more()) {
			BSONObj level_list_obj = level_it.next().embeddedObject();
			uint32_t level = (uint32_t)level_list_obj[Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::LEVEL].numberInt();

			Invested_Money_List invested_money_list;
			invested_money_list.clear();
			BSONObjIterator money_it(level_list_obj.getObjectField(Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::GETTED_UPGRADE_INVEST_PROFIT_LIST.c_str()));
			while(money_it.more()) {
				BSONObj money_obj = money_it.next().embeddedObject();
				uint32_t money =
						(uint32_t)money_obj[Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::Getted_Upgrade_Invest_Profit::MONEY].numberInt();
				invested_money_list.push_back(money);
			}

			if(!invested_money_list.empty()) {
				detail.getted_upgrade_invest_profit_map.insert(std::make_pair(level, invested_money_list));
			}
		}
	}

	return 0;
}

int DB_Operator::save_invest_plan_detail(Invest_Plan_Detail &detail) {
	//月卡投资领取按钮列表
    vector<BSONObj> vc_month_button_list;
	for(Month_Card_Profit_Button_Map::const_iterator it = detail.month_card_profit_button_map.begin(); it != detail.month_card_profit_button_map.end(); ++it) {
		BSONObjBuilder month_builder;
		month_builder << Invest_Plan_Fields::Month_Card_Profit_Button::DAY << it->second.day
				    << Invest_Plan_Fields::Month_Card_Profit_Button::STATUS << (uint8_t)(it->second).status;
		vc_month_button_list.push_back(month_builder.obj());
	}

	vector<BSONObj> vc_upgrade_invest_profit_button_list;
	for(Upgrade_Invest_Profit_Button_Map::const_iterator it = detail.upgrade_invest_profit_button_map.begin();
			it != detail.upgrade_invest_profit_button_map.end();
			++it) {
		BSONObjBuilder upgrade_profit_button_builder;
		upgrade_profit_button_builder << Invest_Plan_Fields::Upgrade_Invest_Profit_Button::LEVEL << it->second.level
				                      << Invest_Plan_Fields::Upgrade_Invest_Profit_Button::STATUS << it->second.status;
		vc_upgrade_invest_profit_button_list.push_back(upgrade_profit_button_builder.obj());
	}

	vector<BSONObj> vc_can_invest_money_set;
	for(Invested_Money_Set::const_iterator it = detail.can_invest_money_set.begin();
			it != detail.can_invest_money_set.end();
			++it) {
		BSONObjBuilder builder;
		builder << Invest_Plan_Fields::Can_Invest_Money::MONEY << (*it);
		vc_can_invest_money_set.push_back(builder.obj());
	}

	vector<BSONObj> vc_invested_money_list;
	for(Invested_Money_List::const_iterator it = detail.invested_money_list.begin();
			it != detail.invested_money_list.end();
			++it) {
		BSONObjBuilder builder;
		builder << Invest_Plan_Fields::Invested_Money::MONEY << (*it);
		vc_invested_money_list.push_back(builder.obj());
	}

	vector<BSONObj> vc_getted_upgrade_invest_profit_map;
	for(Getted_Upgrade_Invest_Profit_Map::const_iterator level_it = detail.getted_upgrade_invest_profit_map.begin();
			level_it != detail.getted_upgrade_invest_profit_map.end();
			++level_it) {
		vector<BSONObj> vc_money_list;
		for(Invested_Money_List::const_iterator money_it = level_it->second.begin();
				money_it != level_it->second.end();
				++money_it) {
			BSONObjBuilder money_builder;
			money_builder << Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::Getted_Upgrade_Invest_Profit::MONEY << (*money_it);
			vc_money_list.push_back(money_builder.obj());
		}

		if(!vc_money_list.empty()) {
			BSONObjBuilder level_builder;
			level_builder << Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::LEVEL << level_it->first
					      << Invest_Plan_Fields::Getted_Upgrade_Invest_Profit_Map::GETTED_UPGRADE_INVEST_PROFIT_LIST << vc_money_list;
			vc_getted_upgrade_invest_profit_map.push_back(level_builder.obj());
		}
	}


	BSONObjBuilder builder;
	builder << Invest_Plan_Fields::ROLE_ID << (long long int)detail.role_id
			<< Invest_Plan_Fields::IS_MONTH_CARD_INVEST << (detail.is_month_card_invest ? 1 : 0)
			<< Invest_Plan_Fields::LEFT_DAY << (detail.left_day)
			<< Invest_Plan_Fields::DAYS << (detail.days)
			<< Invest_Plan_Fields::MONTH_CARD_PROFIT_BUTTON_LIST << vc_month_button_list
			<< Invest_Plan_Fields::INVESTED_MONEY << detail.invested_money
			<< Invest_Plan_Fields::INVEST_BUTTON << detail.invest_button
			<< Invest_Plan_Fields::UPGRADE_INVEST_PROFIT_BUTTON_LIST << vc_upgrade_invest_profit_button_list
			<< Invest_Plan_Fields::CAN_INVEST_MONEY_LIST << vc_can_invest_money_set
			<< Invest_Plan_Fields::INVESTED_MONEY_LIST << vc_invested_money_list
			<< Invest_Plan_Fields::GETTED_UPGRADE_INVEST_PROFIT_MAP << vc_getted_upgrade_invest_profit_map;
	MONGO_CONNECTION.update(Invest_Plan_Fields::COLLECTION, QUERY(Invest_Plan_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::save_invest_plan_history_record(const Invest_Plan_History_Record_List &invest_plan_history_list) {
	MONGO_CONNECTION.remove(Invest_Plan_History_Fields::COLLECTION, mongo::Query());
	for(Invest_Plan_History_Record_List::const_iterator it = invest_plan_history_list.begin();
			it != invest_plan_history_list.end();
			++it) {
		BSONObjBuilder invest_plan_history_builder;
		invest_plan_history_builder << Invest_Plan_History_Fields::History_Record::INVEST_TYPE << it->invest_type
				<< Invest_Plan_History_Fields::History_Record::HISTORY_TYPE << it->history_type
				<< Invest_Plan_History_Fields::History_Record::MONEY << it->money
				<< Invest_Plan_History_Fields::History_Record::PLAYER_NAME << it->player_name;
		MONGO_CONNECTION.insert(Invest_Plan_History_Fields::COLLECTION, invest_plan_history_builder.obj());
	}

	return 0;
}

int DB_Operator::load_invest_plan_history_record(Invest_Plan_History_Record_List &invest_plan_history_list) {
	unsigned long long res_count = MONGO_CONNECTION.count(Invest_Plan_History_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder 	<< Invest_Plan_History_Fields::History_Record::INVEST_TYPE << 1
					<< Invest_Plan_History_Fields::History_Record::HISTORY_TYPE << 1
					<< Invest_Plan_History_Fields::History_Record::MONEY << 1
					<< Invest_Plan_History_Fields::History_Record::PLAYER_NAME << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	MONGO_CONNECTION.findN(iter_record, Invest_Plan_History_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);

	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj total_obj = *iter;
		Invest_Plan_History_Record history_record;
		history_record.invest_type = (uint8_t)total_obj[Invest_Plan_History_Fields::History_Record::INVEST_TYPE].numberInt();
		history_record.history_type = (uint8_t)total_obj[Invest_Plan_History_Fields::History_Record::HISTORY_TYPE].numberInt();
		history_record.money = (uint32_t)total_obj[Invest_Plan_History_Fields::History_Record::MONEY].numberInt();
		history_record.player_name = total_obj[Invest_Plan_History_Fields::History_Record::PLAYER_NAME].str();
		invest_plan_history_list.push_back(history_record);
	}

	iter_record.clear();
	return 0;
}

int DB_Operator::load_thorlottery_manager(ThorLottery_Manager_Detail &detail) {

	typedef ThorLottery_Manager_Fields FIELDS;
	BSONObj res = MONGO_CONNECTION.findOne(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_THORLOTTERY));
	if (res.isEmpty()) return -1;

	BSONObj obj = res.getObjectField(Common_Fields::VALUE.c_str());
	if (obj.isEmpty()) return -1;

	detail.season_id_ = obj[FIELDS::SEASON_ID].numberInt();
	detail.state_ = obj[FIELDS::STATE].numberInt();
	detail.icon_id_ = obj[FIELDS::ICON_ID].numberInt();
	detail.lv_limit = obj[FIELDS::LV_LIMIT].numberInt();

	BSONObj thorlottery_log_obj = obj.getObjectField(FIELDS::THORLOTTERY_LOG.c_str());
	BSONObjIterator iter(thorlottery_log_obj);
	ThorLottery_Item_Log log;
	while(iter.more()) {
		BSONObj log_obj = iter.next().embeddedObject();
		int64_t time = log_obj[FIELDS::ThorLottery_Log::TIME].numberLong();
		log.time_ = Time_Value(time / 1000, time % 1000 * 1000);
		log.role_id_ = log_obj[FIELDS::ThorLottery_Log::ROLE_ID].numberInt();
		log.role_name_ = log_obj[FIELDS::ThorLottery_Log::ROLE_NAME].valuestrsafe();
		log.item_id_ = log_obj[FIELDS::ThorLottery_Log::ITEM_ID].numberInt();
		log.item_num_ = log_obj[FIELDS::ThorLottery_Log::ITEM_NUM].numberInt();
		log.bind_state_ = log_obj[FIELDS::ThorLottery_Log::BIND_STATE].numberInt();
		detail.lottery_log_vec_.push_back(log);
	}
	BSONObj thorlottery_item_obj = obj.getObjectField(FIELDS::THORLOTTERR_ITEM.c_str());
	BSONObjIterator item_iter(thorlottery_item_obj);
	ThorLottery_Item item;
	while(item_iter.more()) {
		item.reset();
		BSONObj item_obj = item_iter.next().embeddedObject();
		item.index_ = item_obj[FIELDS::ThorLottery_Item::INDEX].numberInt();
		item.counter_ = item_obj[FIELDS::ThorLottery_Item::NUM].numberInt();
		detail.thorlottery_item_map_[item.index_] = item;
	}

	return 0;
}

int DB_Operator::save_thorlottery_manager(const ThorLottery_Manager_Detail &detail) {

	typedef ThorLottery_Manager_Fields FIELDS;
	std::vector<BSONObj> log_vec;
	for (ThorLottery_Log_Vec::const_iterator it = detail.lottery_log_vec_.begin();
			it != detail.lottery_log_vec_.end(); ++it) {

		int64_t msec = static_cast<int64_t>(it->time_.sec()) * 1000 + it->time_.usec() / 1000;
		log_vec.push_back(BSON(FIELDS::ThorLottery_Log::TIME << (long long int)msec
				<< FIELDS::ThorLottery_Log::ROLE_NAME << it->role_name_
				<< FIELDS::ThorLottery_Log::ROLE_ID << (long long int)(it->role_id_)
				<< FIELDS::ThorLottery_Log::ITEM_ID << it->item_id_
				<< FIELDS::ThorLottery_Log::ITEM_NUM << it->item_num_
				<< FIELDS::ThorLottery_Log::BIND_STATE<< it->bind_state_));
	}

	std::vector<BSONObj> obj_vec;
	for (ThorLottery_Item_Map::const_iterator it = detail.thorlottery_item_map_.begin();
			it != detail.thorlottery_item_map_.end(); ++it) {
		BSONObj obj = BSON(
				FIELDS::ThorLottery_Item::INDEX << it->second.index_ <<
				FIELDS::ThorLottery_Item::NUM << it->second.counter_);
		obj_vec.push_back(obj);
	}

	BSONObj obj =  BSON(
			FIELDS::SEASON_ID << detail.season_id_ <<
			FIELDS::STATE << detail.state_ <<
			FIELDS::ICON_ID << detail.icon_id_ <<
			FIELDS::LV_LIMIT << detail.lv_limit <<
			FIELDS::THORLOTTERY_LOG << log_vec <<
			FIELDS::THORLOTTERR_ITEM << obj_vec);

	BSONObjBuilder builder;
	builder << Common_Fields::TYPE << DB_THORLOTTERY
			<< Common_Fields::VALUE << obj;

	MONGO_CONNECTION.update(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_THORLOTTERY),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_thorlottery(ThorLottery_Detail &detail) {

	typedef ThorLottery_Fields FIELDS;
	BSONObj res = MONGO_CONNECTION.findOne(ThorLottery_Fields::COLLECTION, QUERY(ThorLottery_Fields::ROLE_ID << (long long int) detail.role_id));
	if (res.isEmpty()) return -1;

	int64_t time = res[FIELDS::TIME].numberLong();
	detail.lottery_time_ = Time_Value(time / 1000, (time % 1000) * 1000);
	detail.season_id = res[FIELDS::SEASON_ID].numberInt();
	detail.score = res[FIELDS::SCORE].numberInt();
	detail.daily_lottery_times_ = res[FIELDS::DAILY_LOTTERY_TIMES].numberInt();
	detail.thorlottery_item_map_.clear();

	BSONObjIterator iter = res.getObjectField(FIELDS::THORLOTTERY_ITEM.c_str());
	while (iter.more()) {
		BSONObj obj  = iter.next().embeddedObject();

		ThorLottery_Item item;
		item.index_ = obj[FIELDS::ThorLottery_Item::INDEX].numberInt();
		item.counter_ = obj[FIELDS::ThorLottery_Item::NUM].numberInt();
		detail.thorlottery_item_map_[item.index_] = item;
	}

	return 0;
}

int DB_Operator::save_thorlottery(const ThorLottery_Detail &detail) {

	typedef ThorLottery_Fields FIELDS;
	vector<BSONObj> obj_vec;
	for (ThorLottery_Item_Map::const_iterator it = detail.thorlottery_item_map_.begin();
			it != detail.thorlottery_item_map_.end(); ++it) {

		BSONObj obj = BSON(
				FIELDS::ThorLottery_Item::INDEX << it->second.index_ <<
				FIELDS::ThorLottery_Item::NUM << it->second.counter_);
		obj_vec.push_back(obj);
	}
	int msec = static_cast<int64_t>(detail.lottery_time_.sec()) * 1000 + detail.lottery_time_.usec() / 1000;
	BSONObjBuilder builder;
	builder << FIELDS::ROLE_ID << (long long int)(detail.role_id)
			<< FIELDS::SEASON_ID << detail.season_id
			<< FIELDS::TIME << (long long int)msec
			<< FIELDS::SCORE << detail.score
			<< FIELDS::DAILY_LOTTERY_TIMES << detail.daily_lottery_times_
			<< FIELDS::THORLOTTERY_ITEM << obj_vec;

	MONGO_CONNECTION.update(FIELDS::COLLECTION, QUERY(FIELDS::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);

	return 0;
}
