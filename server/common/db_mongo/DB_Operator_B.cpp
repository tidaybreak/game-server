/*
 * DB_Operator_C.cpp
 *
 *  Created on: 2014年9月11日
 *      Author: Linqiyou
 */

#include "DB_Operator.h"
#include "DB_Fields.h"
#include "Configurator.h"
#include "Config_Cache_Task.h"
#include "Config_Cache_World_Boss.h"
#include "Record_Client.h"
#include "Pool_Manager.h"
#include "Server_Struct.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "mongo/client/dbclient.h"
#include "gang/Gang_Manager.h"
#include "arena/Arena_Manager.h"
#include "ranking/Ranking_Def.h"
#include "ranking/Ranking_Struct.h"
#include "Config_Cache_Ranking.h"
#include "DB_Def.h"
#include "Config_Cache_Item.h"
#include "equiper/Equiper_Def.h"
#include "rune_stone/Rune_Stone_Def.h"

using namespace mongo;

#include "DB_Manager.h"

//读取封禁帐号
void DB_Operator::load_forbid_list(String_Time_Map &account_list, Role_Id_Time_Map &role_id_list){
	unsigned long long res_count = MONGO_CONNECTION.count(BAN_ACCOUNT_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder 	<< BAN_ACCOUNT_Fields::BAN_TYPE << 1
					<< BAN_ACCOUNT_Fields::ACCOUNT << 1
					<< BAN_ACCOUNT_Fields::ROLE_ID << 1
					<< BAN_ACCOUNT_Fields::UNBAN_TIME << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	account_list.empty();role_id_list.empty(); //清空两个列表
	Time_Value now = Time_Value::gettimeofday();//获取当前时间
	MONGO_CONNECTION.findN(iter_record, BAN_ACCOUNT_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		//判断封号时间是否已经过了
		if(obj[BAN_ACCOUNT_Fields::UNBAN_TIME].numberLong() < now.sec()) continue;
		if(0 == obj[BAN_ACCOUNT_Fields::BAN_TYPE].numberLong()){
			//帐号
			account_list.insert(std::make_pair(obj[BAN_ACCOUNT_Fields::ACCOUNT].valuestrsafe(), Time_Value(obj[BAN_ACCOUNT_Fields::UNBAN_TIME].numberLong(),0)));
		}else{
			//角色ID
			role_id_list.insert(std::make_pair(obj[BAN_ACCOUNT_Fields::ROLE_ID].numberLong(), Time_Value(obj[BAN_ACCOUNT_Fields::UNBAN_TIME].numberLong(),0)));
		}
	}
}

//读取封禁IP
void DB_Operator::load_forbid_ip_list(String_Time_Map &ip_list){
	unsigned long long res_count = MONGO_CONNECTION.count(BAN_IP_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder 	<< BAN_IP_Fields::IP_ADDRESS << 1
					<< BAN_IP_Fields::UNBAN_TIME << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	ip_list.empty(); //清空列表
	Time_Value now = Time_Value::gettimeofday();//获取当前时间
	MONGO_CONNECTION.findN(iter_record, BAN_IP_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		//判断禁止时间是否已经过了
		if(obj[BAN_ACCOUNT_Fields::UNBAN_TIME].numberLong() < now.sec()) continue;
		//加载封禁的IP
		ip_list.insert(std::make_pair(obj[BAN_IP_Fields::IP_ADDRESS].valuestrsafe(), Time_Value(obj[BAN_IP_Fields::UNBAN_TIME].numberLong(),0)));

	}
}

//读取商城物品列表
void DB_Operator::save_item_sales_num(Item_Role_Set_Map &item_sales_num, Tabs_Item_Role_Set_Map& tabs_item_sales_num) {
	std::vector<BSONObj> obj_vec;
	obj_vec.clear();
	for ( Item_Role_Set_Map::iterator iter = item_sales_num.begin();
			iter != item_sales_num.end(); ++iter ) {
		std::vector<long long int> rid_vc;
		rid_vc.clear();
		for(Role_Id_MSet::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			rid_vc.push_back(*it);
		}
		obj_vec.push_back( BSON(Common_Fields::TYPE << iter->first
				<< Common_Fields::VALUE << rid_vc) );
	}
//tabs_item_sales_num
	std::vector<BSONObj> tabs_obj_vec;
	tabs_obj_vec.clear();
	for(Tabs_Item_Role_Set_Map::iterator itt = tabs_item_sales_num.begin(); itt != tabs_item_sales_num.end(); ++itt) {
		std::vector<BSONObj> t_obj_vec;
		t_obj_vec.clear();
		for ( Item_Role_Set_Map::iterator iter = itt->second.begin();
				iter != itt->second.end(); ++iter ) {
			std::vector<long long int> rid_vc;
			rid_vc.clear();
			for(Role_Id_MSet::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
				rid_vc.push_back(*it);
			}
			t_obj_vec.push_back( BSON(Common_Fields::TYPE << iter->first
					<< Common_Fields::VALUE << rid_vc) );
		}
		tabs_obj_vec.push_back( BSON(Common_Fields::TYPE << itt->first
				<< Common_Fields::VALUE << t_obj_vec) );
	}
	BSONObjBuilder builder;
	builder	<< Common_Fields::VALUE << obj_vec
			<< Common_Fields::VALUE_1 << tabs_obj_vec;
	MONGO_CONNECTION.update(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_MALL),
							BSON("$set" << builder.obj() ), true);
}

void DB_Operator::load_item_sales_num(Item_Role_Set_Map &item_sales_num, Tabs_Item_Role_Set_Map& tabs_item_sales_num) {
	item_sales_num.clear();
	BSONObj res = MONGO_CONNECTION.findOne(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_MALL));
	if (res.isEmpty()) {
		return;
	}
	{
		BSONObjIterator iter(res.getObjectField(Common_Fields::VALUE.c_str()));
		while (iter.more())	{
			const BSONObj& obj = iter.next().embeddedObject();
			BSONObjIterator iter_set(obj.getObjectField(Common_Fields::VALUE.c_str()));
			Role_Id_MSet rmset;
			rmset.clear();
			while (iter_set.more()) {
				rmset.insert(iter_set.next().numberLong());
			}
			item_sales_num.insert(std::make_pair(obj[Common_Fields::TYPE].numberInt(), rmset));
		}
	}
	{
		BSONObjIterator titer(res.getObjectField(Common_Fields::VALUE_1.c_str()));
		while (titer.more())	{
			const BSONObj& tobj = titer.next().embeddedObject();
			BSONObjIterator iter(tobj.getObjectField(Common_Fields::VALUE.c_str()));
			Item_Role_Set_Map isn;isn.clear();
			while (iter.more())	{
				const BSONObj& obj = iter.next().embeddedObject();
				BSONObjIterator iter_set(obj.getObjectField(Common_Fields::VALUE.c_str()));
				Role_Id_MSet rmset;
				rmset.clear();
				while (iter_set.more()) {
					rmset.insert(iter_set.next().numberLong());
				}
				isn.insert(std::make_pair(obj[Common_Fields::TYPE].numberInt(), rmset));
			}
			tabs_item_sales_num.insert(std::make_pair(tobj[Common_Fields::TYPE].numberInt(), isn));
		}
	}
}

void DB_Operator::load_goods_list(Goods_Info_Vec &goods_list_){
	unsigned long long res_count = MONGO_CONNECTION.count(MALL_LIST_Fields::COLLECTION);
	vector<BSONObj> res;
		BSONObjBuilder field_builder;
		field_builder 	<< MALL_LIST_Fields::ID << 1
						<< MALL_LIST_Fields::TABS << 1
						<< MALL_LIST_Fields::PAY_TYPE << 1
						<< MALL_LIST_Fields::PRICE << 1
						<< MALL_LIST_Fields::VIP_DISCOUNTS << 1
						<< MALL_LIST_Fields::PROMO_PRICE << 1
						<< MALL_LIST_Fields::ALL_NUM << 1
						<< MALL_LIST_Fields::PER_NUM << 1
						<< MALL_LIST_Fields::SORT << 1;
		BSONObj field_bson = field_builder.obj();
		vector<BSONObj> iter_record;
		iter_record.reserve(res_count);
		goods_list_.clear();//清空列表
		Goods_Info gi;
		MONGO_CONNECTION.findN(iter_record, MALL_LIST_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
			BSONObj &obj = *iter;
			gi.reset();
			gi.id = obj[MALL_LIST_Fields::ID].numberInt();
			gi.tabs=obj[MALL_LIST_Fields::TABS].numberInt();
			gi.pay_type=obj[MALL_LIST_Fields::PAY_TYPE].numberInt();
			gi.price=obj[MALL_LIST_Fields::PRICE].numberInt();
			gi.vip_discounts=obj[MALL_LIST_Fields::VIP_DISCOUNTS].numberInt();
			gi.promo_price=obj[MALL_LIST_Fields::PROMO_PRICE].numberInt();
			gi.all_num=obj[MALL_LIST_Fields::ALL_NUM].numberInt();
			gi.per_num=obj[MALL_LIST_Fields::PER_NUM].numberInt();
			gi.sort=obj[MALL_LIST_Fields::SORT].numberInt();
			//插入数据
			goods_list_.push_back(gi);
		}

}

void DB_Operator::load_promo_goods_list(Promo_Goods_Info_Vec &promo_goods_list_){
	Time_Value time_=Time_Value::gettimeofday();
//	BSONObj query = BSONObj();
	unsigned long long res_count = MONGO_CONNECTION.count(MALL_PROMO_LIST_Fields::COLLECTION);
	vector<BSONObj> res;
		BSONObjBuilder field_builder;
		field_builder 	<< MALL_PROMO_LIST_Fields::START_TIME << 1
						<< MALL_PROMO_LIST_Fields::END_TIME << 1
						<< MALL_PROMO_LIST_Fields::END_TIME << 1
						<< MALL_PROMO_LIST_Fields::DAY << 1
						<< MALL_PROMO_LIST_Fields::RANK << 1
						<< MALL_PROMO_LIST_Fields::GOODS_LIST << 1;
		BSONObj field_bson = field_builder.obj();
		vector<BSONObj> iter_record;
		iter_record.reserve(res_count);
		promo_goods_list_.clear();//清空列表
		Mall_Promo_Info pd;
		Goods_Info gi;
		MONGO_CONNECTION.findN(iter_record, MALL_PROMO_LIST_Fields::COLLECTION, mongo::Query().sort( BSON( MALL_PROMO_LIST_Fields::RANK << 1) ), res_count, 0, &field_bson);
		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
			pd.reset();
			BSONObj &obj = *iter;
			pd.start_time = obj[MALL_PROMO_LIST_Fields::START_TIME].numberInt();
			pd.end_time = obj[MALL_PROMO_LIST_Fields::END_TIME].numberInt();
			pd.day = obj[MALL_PROMO_LIST_Fields::DAY].numberInt();

			BSONObjIterator goods_list_iter(obj.getObjectField(MALL_PROMO_LIST_Fields::GOODS_LIST.c_str()));
			while (goods_list_iter.more())	{
				gi.reset();
				const BSONObj& obj = goods_list_iter.next().embeddedObject();
				gi.id = obj[MALL_LIST_Fields::ID].numberInt();
				gi.tabs=obj[MALL_LIST_Fields::TABS].numberInt();
				gi.pay_type=obj[MALL_LIST_Fields::PAY_TYPE].numberInt();
				gi.price=obj[MALL_LIST_Fields::PRICE].numberInt();
				gi.vip_discounts=obj[MALL_LIST_Fields::VIP_DISCOUNTS].numberInt();
				gi.promo_price=obj[MALL_LIST_Fields::PROMO_PRICE].numberInt();
				gi.all_num=obj[MALL_LIST_Fields::ALL_NUM].numberInt();
				gi.per_num=obj[MALL_LIST_Fields::PER_NUM].numberInt();
				gi.sort=obj[MALL_LIST_Fields::SORT].numberInt();
				pd.goods_list.push_back(gi);
			}

			promo_goods_list_.push_back(pd);
//			if(pd.start_time <= time_.sec() && pd.end_time >= time_.sec()){
//				promo_goods_=pd;
//			}
		}
}

void DB_Operator::load_banner_goods_list(Promo_Goods_Info_Vec &banner_goods_list_){
	Time_Value time_=Time_Value::gettimeofday();
//	BSONObj query = BSONObj();
	unsigned long long res_count = MONGO_CONNECTION.count(MALL_BANNER_LIST_Fields::COLLECTION);
	vector<BSONObj> res;
		BSONObjBuilder field_builder;
		field_builder 	<< MALL_BANNER_LIST_Fields::START_TIME << 1
						<< MALL_BANNER_LIST_Fields::END_TIME << 1
						<< MALL_BANNER_LIST_Fields::DAY << 1
						<< MALL_BANNER_LIST_Fields::RANK << 1
						<< MALL_BANNER_LIST_Fields::GOODS_LIST << 1;
		BSONObj field_bson = field_builder.obj();
		vector<BSONObj> iter_record;
		iter_record.reserve(res_count);
		banner_goods_list_.clear();//清空列表
		Mall_Promo_Info pd;
		Goods_Info gi;
		MONGO_CONNECTION.findN(iter_record, MALL_BANNER_LIST_Fields::COLLECTION,  mongo::Query().sort( BSON( MALL_BANNER_LIST_Fields::RANK << 1) ), res_count, 0, &field_bson);
		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
			pd.reset();
			BSONObj &obj = *iter;
			pd.start_time = obj[MALL_BANNER_LIST_Fields::START_TIME].numberInt();
			pd.end_time = obj[MALL_BANNER_LIST_Fields::END_TIME].numberInt();
			pd.day = obj[MALL_BANNER_LIST_Fields::DAY].numberInt();

			BSONObjIterator goods_list_iter(obj.getObjectField(MALL_BANNER_LIST_Fields::GOODS_LIST.c_str()));
			while (goods_list_iter.more())	{
				gi.reset();
				const BSONObj& obj = goods_list_iter.next().embeddedObject();
				gi.id = obj[MALL_LIST_Fields::ID].numberInt();
				gi.tabs=obj[MALL_LIST_Fields::TABS].numberInt();
				gi.pay_type=obj[MALL_LIST_Fields::PAY_TYPE].numberInt();
				gi.price=obj[MALL_LIST_Fields::PRICE].numberInt();
				gi.vip_discounts=obj[MALL_LIST_Fields::VIP_DISCOUNTS].numberInt();
				gi.promo_price=obj[MALL_LIST_Fields::PROMO_PRICE].numberInt();
				gi.all_num=obj[MALL_LIST_Fields::ALL_NUM].numberInt();
				gi.per_num=obj[MALL_LIST_Fields::PER_NUM].numberInt();
				gi.sort=obj[MALL_LIST_Fields::SORT].numberInt();
				pd.goods_list.push_back(gi);
			}
			banner_goods_list_.push_back(pd);
//			if(pd.start_time <= time_.sec() && pd.end_time >= time_.sec()){
//				banner_goods_=pd;
//			}
		}
}

int DB_Operator::load_teamer_detail(Teamer_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Team_Role_Fields::COLLECTION,
			QUERY(Team_Role_Fields::ROLE_ID << (long long int)detail.role_id));

	if (res.isEmpty()) {
		return -1;
	}

	detail.team_id = res[Team_Role_Fields::TEAM_ID].numberLong();
	detail.profile_num_fb = res[Team_Role_Fields::PROFILE_NUM_FB].numberInt();
	detail.profile_num_fb_pre = res[Team_Role_Fields::PROFILE_NUM_FB_PRE].numberInt();
	detail.profile_num_sports = res[Team_Role_Fields::PROFILE_NUM_SPORTS].numberInt();
	detail.profile_num_hero_dream = res[Team_Role_Fields::PROFILE_NUM_HERO_DREAM].numberInt();
	detail.profile_num_defend = res[Team_Role_Fields::PROFILE_NUM_DEFEND].numberInt();
	int sec = res[Team_Role_Fields::PROFILE_TIME_SEC].numberInt();
	detail.profile_time.sec(sec);
	detail.is_not_first_team_fb = res[Team_Role_Fields::IS_NOT_FIRST_TEAM_FB].booleanSafe();

	if (res.hasField("team_award")) {
		std::vector<BSONElement> v = res.getField("team_award").Array();
		for(std::vector<BSONElement>::iterator it = v.begin(); it != v.end(); ++it)
			detail.team_award.push_back(it->numberInt());
	}

	detail.ready = res[Team_Role_Fields::READY].booleanSafe();

	return 0;
}

int DB_Operator::save_teamer_detail(Teamer_Detail &detail) {
	BSONObjBuilder builder;
	builder << Team_Role_Fields::ROLE_ID << (long long int)(detail.role_id)
		<< Team_Role_Fields::TEAM_ID <<  (long long int)detail.team_id
		<< Team_Role_Fields::PROFILE_NUM_FB << detail.profile_num_fb
		<< Team_Role_Fields::PROFILE_NUM_FB_PRE << detail.profile_num_fb_pre
		<< Team_Role_Fields::PROFILE_NUM_SPORTS << detail.profile_num_sports
		<< Team_Role_Fields::PROFILE_NUM_HERO_DREAM << detail.profile_num_hero_dream
		<< Team_Role_Fields::PROFILE_NUM_DEFEND << detail.profile_num_defend
		<< Team_Role_Fields::PROFILE_TIME_SEC << (long long int)(detail.profile_time.sec())
		<< Team_Role_Fields::IS_NOT_FIRST_TEAM_FB << detail.is_not_first_team_fb
		<< "team_award" << detail.team_award
		<< Team_Role_Fields::READY << detail.ready;

	MONGO_CONNECTION.update(Team_Role_Fields::COLLECTION, QUERY(Team_Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
		BSON("$set" << builder.obj()), true);


	return 0;
}

//int DB_Operator::load_item_detail(const std::vector<mongo::BSONObj> &obj_vec, Item_Vec &item_vec) {
//	for (std::vector<mongo::BSONObj>::const_iterator it =  obj_vec.begin(); it != obj_vec.end(); ++it) {
//		Item_Detail item;
//		load_item_detail(*it, item);
//		item_vec.push_back(item);
//	}
//	return 0;
//}
//
//int DB_Operator::save_item_detail(const Item_Vec &item_vec, std::vector<mongo::BSONObj> &obj_vec) {
//	for (Item_Vec::const_iterator it =  item_vec.begin(); it != item_vec.end(); ++it) {
//		mongo::BSONObj obj;
//		save_item_detail(*it, obj);
//		obj_vec.push_back(obj);
//	}
//	return 0;
//}

int DB_Operator::load_gang_db_view() {
	unsigned long long row_count = MONGO_CONNECTION.count( Gang_Fields::COLLECTION );
	BSONObjBuilder field_builder;
	field_builder 	<< Gang_Fields::GANG_ID << 1
					<< Gang_Fields::LEADER << 1
					<< Gang_Fields::FUND << 1
					<< Gang_Fields::MAINTAIN_COST << 1
					<< Gang_Fields::LEVEL << 1
					<< Gang_Fields::BOSS_ACT << 1
					<< Gang_Fields::BOSS_HOUR << 1
					<< Gang_Fields::BOSS_MINUTE << 1
					<< Gang_Fields::BOSS_KILL_NUM << 1
					<< Gang_Fields::BOSS_LV << 1
					<< Gang_Fields::RANK << 1
					<< Gang_Fields::MAX_MEMBERS << 1
					<< Gang_Fields::LOGO << 1
					<< Gang_Fields::DAY_DEFICIT_COUNT << 1
					<< Gang_Fields::STATUS << 1
					<< Gang_Fields::JOIN_BLOCK << 1
					<< Gang_Fields::CD_UPGRADE << 1
					<< Gang_Fields::CREATE_TIME << 1
					<< Gang_Fields::DISMISS_TIME << 1
					<< Gang_Fields::ANNOUNCEMENT << 1
					<< Gang_Fields::NAME << 1
					<< Gang_Fields::SERVER_INFO << 1
					<< Gang_Fields::MEMBERS << 1
					<< Gang_Fields::AFFAIRS << 1
					<< Gang_Fields::AFFAIRS << 1
					<< Gang_Fields::LEVEL_LEADER_ID << 1
					;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve( row_count );

	//MONGO_CONNECTION.findN(iter_record, Gang_Fields::COLLECTION, QUERY(Gang_Fields::STATUS << Gang_DB_View::GANG_STAT_NORMAL), row_count, 0, &field_bson);
	//MONGO_CONNECTION.findN(iter_record, Gang_Fields::COLLECTION, mongo::Query(), row_count, 0, &field_bson);
	MONGO_CONNECTION.findN(iter_record, Gang_Fields::COLLECTION, mongo::Query(), row_count);
	int count = 0;
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj& obj = *iter;
		Gang_DB_View* gang_db_view = POOL_MANAGER->pop_gang_db_view();
		if ( !gang_db_view ) {
			MSG_USER("load_gang_db_view, pop_gang_db_view() return 0.");
			return -1;
		}
	    gang_db_view->gang_id = obj[Gang_Fields::GANG_ID].numberLong();
		if ( gang_db_view->gang_id == 0 ) {
			LOG_ABORT("load_gang_db_view, gang id == 0!!");
		}
		gang_db_view->leader = obj[Gang_Fields::LEADER].numberLong();
		gang_db_view->last_leader_id = obj[Gang_Fields::LAST_LEADER_ID].numberLong();
		gang_db_view->notified_last_leader = obj[Gang_Fields::NOTIFIED_LAST_LEADER].numberInt();
		gang_db_view->fund	 = obj[Gang_Fields::FUND].numberInt();
		gang_db_view->maintain_cost = obj[Gang_Fields::MAINTAIN_COST].numberInt();
		gang_db_view->fund_lack_times = obj[Gang_Fields::FUND_LACK_TIMES].numberInt();
		gang_db_view->level  = obj[Gang_Fields::LEVEL].numberInt();
		gang_db_view->boss_act  = obj[Gang_Fields::BOSS_ACT].numberInt();
		gang_db_view->boss_hour  = obj[Gang_Fields::BOSS_HOUR].numberInt();
		gang_db_view->boss_minute  = obj[Gang_Fields::BOSS_MINUTE].numberInt();
		gang_db_view->boss_kill_num  = obj[Gang_Fields::BOSS_KILL_NUM].numberInt();
		gang_db_view->boss_lv  = obj[Gang_Fields::BOSS_LV].numberInt();
		if(gang_db_view->boss_lv == 0){
			int gang_boss_act_icon_id = 19;
			gang_db_view->boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(
							gang_db_view->boss_kill_num, gang_boss_act_icon_id);
		}
		gang_db_view->rank = obj[Gang_Fields::RANK].numberInt();
		gang_db_view->max_members = obj[Gang_Fields::MAX_MEMBERS].numberInt();
		gang_db_view->logo		= obj[Gang_Fields::LOGO].numberInt();
		gang_db_view->day_deficit_count = obj[Gang_Fields::DAY_DEFICIT_COUNT].numberInt();
		gang_db_view->status	= obj[Gang_Fields::STATUS].numberInt();
		gang_db_view->join_block= obj[Gang_Fields::JOIN_BLOCK].numberInt();
		gang_db_view->cd_upgrade.sec( obj[Gang_Fields::CD_UPGRADE].numberLong() );
		gang_db_view->create_time.sec( obj[Gang_Fields::CREATE_TIME].numberLong() );
		gang_db_view->dismiss_time.sec( obj[Gang_Fields::DISMISS_TIME].numberLong() );
		gang_db_view->tv_leader_offline.sec(obj[Gang_Fields::TV_LEADER_OFFLINE].numberInt());
		gang_db_view->announcement = obj[Gang_Fields::ANNOUNCEMENT].str();
		gang_db_view->name 		= obj[Gang_Fields::NAME].str();
		gang_db_view->server_info.agent_num = obj[Gang_Fields::SERVER_INFO][Gang_Fields::Server_Info::AGENT_NUM].numberInt();
		gang_db_view->server_info.server_num = obj[Gang_Fields::SERVER_INFO][Gang_Fields::Server_Info::SERVER_NUM].numberInt();

		BSONObj bson_members = obj.getObjectField( Gang_Fields::MEMBERS.c_str() );
		Gang_Member_Detail member_detail;
		BSONObjIterator member_iter( bson_members );
		while ( member_iter.more() ) {
			member_detail.reset();
			const BSONElement &member_element = member_iter.next();
			if (member_element.isNumber()) {
				member_detail.role_id = member_element.numberLong();
				gang_db_view->member_map.insert(std::make_pair(member_detail.role_id, member_detail));
			} else if (member_element.isABSONObj()) {
				const BSONObj& obj_member = member_element.embeddedObject();
				member_detail.role_id = obj_member[Gang_Fields::Member::ROLE_ID].numberLong();

				// repair member_map after core, to check ganger if gang_id exist or not
				BSONObj res_ganger = MONGO_CONNECTION.findOne(Ganger_Fields::COLLECTION,
						QUERY(Ganger_Fields::ROLE_ID << (long long int)member_detail.role_id));
				if (!res_ganger.isEmpty() && res_ganger[Ganger_Fields::GANG_ID].numberLong() != (long long)gang_db_view->gang_id) {
					gang_db_view->change_view();
					continue;
				}

				member_detail.week_contribute = obj_member[Gang_Fields::Member::WEEK_CONTRIBUTE].numberInt();
				member_detail.total_contribute = obj_member[Gang_Fields::Member::TOTAL_CONTRIBUTE].numberInt();
				member_detail.amplify_times = obj_member[Gang_Fields::Member::AMPLIFY_TIMES].numberInt();
				member_detail.headship = obj_member[Gang_Fields::Member::HEADSHIP].numberInt();
				member_detail.authority = obj_member[Gang_Fields::Member::AUTHORITY].numberInt();

				gang_db_view->member_map.insert(std::make_pair(member_detail.role_id, member_detail));
			}
		}

		BSONObj bson_affairs = obj.getObjectField( Gang_Fields::AFFAIRS.c_str() );
		BSONObjIterator affair_iter( bson_affairs );
		while ( affair_iter.more() ) {
			const BSONObj& obj_affair = affair_iter.next().embeddedObject();
			Gang_DB_View::Affair new_affair;
			new_affair.type 	= obj_affair[Gang_Fields::Affairs::TYPE].numberInt();
			new_affair.result 	= obj_affair[Gang_Fields::Affairs::RESULT].numberInt();
			new_affair.expire.sec( obj_affair[Gang_Fields::Affairs::EXPIRE].numberLong() );
			new_affair.applier	= obj_affair[Gang_Fields::Affairs::APPLIER].numberLong();
			gang_db_view->affairs.push_back( new_affair );
		}

		BSONObj bson_skills = obj.getObjectField(Gang_Fields::RESEARCHED_SKILLS.c_str());
		Gang_Skill_Detail skill_detail;
		BSONObjIterator it_skill(bson_skills);
		while (it_skill.more()) {
			const BSONObj& obj_skill = it_skill.next().embeddedObject();
			skill_detail.reset();
			skill_detail.skill_id = obj_skill[Gang_Fields::Researched_Skills::SKILL_ID].numberInt();
			gang_db_view->researched_skills.insert(std::make_pair(skill_detail.skill_id, skill_detail));
		}

		BSONObj bson_buildings = obj.getObjectField(Gang_Fields::BUILDINGS.c_str());
		Gang_Building_Detail building_detail;
		BSONObjIterator it_building(bson_buildings);
		while (it_building.more()) {
			const BSONObj& obj_building = it_building.next().embeddedObject();
			building_detail.reset();
			building_detail.building_id = obj_building[Gang_Fields::Building::BUILDING_ID].numberInt();
			building_detail.building_lvl = obj_building[Gang_Fields::Building::BUILDING_LVL].numberInt();
			building_detail.cd_upgrade.sec(obj_building[Gang_Fields::Building::CD_UPGRADE].numberLong());

			gang_db_view->buildings.insert(std::make_pair(building_detail.building_id, building_detail));
		}

		{
			BSONObj bson_buildings = obj.getObjectField(Gang_Fields::REWARD_INFO_GANG_WAR.c_str());
			Reward_Info_Gang_War rigw;
			BSONObjIterator it_building(bson_buildings);
			while (it_building.more()) {
				const BSONObj& obj_building = it_building.next().embeddedObject();
				rigw.reset();
				rigw.role_id = obj_building[Role_Fields::ROLE_ID].numberLong();
				rigw.role_name = obj_building[Role_Fields::ROLE_NAME].valuestrsafe();
				rigw.index = obj_building[Gang_Fields::Reward_Info_Gang_War::INDEX].numberLong();
				rigw.award.reward_id = obj_building[Gang_Fields::Reward_Info_Gang_War::REWARD_ID].numberInt();
				rigw.award.quantity = obj_building[Gang_Fields::Reward_Info_Gang_War::QUANTITY].numberInt();
				rigw.award.vip_add = obj_building[Gang_Fields::Reward_Info_Gang_War::VIP_ADD].numberInt();

				gang_db_view->gang_war_award.insert(std::make_pair(rigw.index, rigw));
				if (gang_db_view->gang_war_award_index < rigw.index) {
					gang_db_view->gang_war_award_index = rigw.index;
				}
			}
		}

		{
			BSONObj bson_buildings = obj.getObjectField(Gang_Fields::ACTIVE_ANNON_ED.c_str());
			BSONObjIterator it_building(bson_buildings);
			while (it_building.more()) {
				gang_db_view->acitve_annon_ed.insert(it_building.next().numberInt());
			}
		}

		{
			BSONObj bson_buildings = obj.getObjectField(Gang_Fields::LEVEL_LEADER_ID.c_str());
			BSONObjIterator it_building(bson_buildings);
			while (it_building.more()) {
				const BSONObj& obj_building = it_building.next().embeddedObject();
				int32_t level = obj_building[Common_Fields::TYPE].numberInt();
				role_id_t role_id = obj_building[Common_Fields::VALUE].numberLong();
				gang_db_view->level_leader_id[level] = role_id;
			}
		}

		GANG_MANAGER->add_gang_db_view( gang_db_view );
		++count;
	}
	LOG_DEBUG("load_gang_db_view, normal gang size:%d", count);
	return 0;
}

int DB_Operator::save_gang_db_view(const Gang_DB_View& gang_view) {
	if ( gang_view.gang_id == 0 ) {
		//LOG_DEBUG("gang id == 0");
		return 1;
	}
	std::vector<BSONObj> affair_vec;
	for ( Gang_DB_View::Affair_Vec::const_iterator iter = gang_view.affairs.begin();
			iter != gang_view.affairs.end(); ++iter ) {
		affair_vec.push_back( BSON(Gang_Fields::Affairs::TYPE << (*iter).type
				<< Gang_Fields::Affairs::RESULT << (*iter).result
				<< Gang_Fields::Affairs::EXPIRE << (long long int)(*iter).expire.sec()
				<< Gang_Fields::Affairs::APPLIER << (long long int)(*iter).applier) );
	}

	//std::vector<long long int> member_vec;
	std::vector<BSONObj> member_vec;
	for (Gang_DB_View::Member_Map::const_iterator iter = gang_view.member_map.begin();
			iter != gang_view.member_map.end(); ++iter) {
		member_vec.push_back(BSON(Gang_Fields::Member::ROLE_ID << (long long int)iter->second.role_id
				<< Gang_Fields::Member::WEEK_CONTRIBUTE << iter->second.week_contribute
				<< Gang_Fields::Member::TOTAL_CONTRIBUTE << iter->second.total_contribute
				<< Gang_Fields::Member::AMPLIFY_TIMES << iter->second.amplify_times
				<< Gang_Fields::Member::HEADSHIP << iter->second.headship
				<< Gang_Fields::Member::AUTHORITY << iter->second.authority
				));
	}

	std::vector<BSONObj> skill_vec;
	for (Gang_Skill_Map::const_iterator iter = gang_view.researched_skills.begin();
			iter != gang_view.researched_skills.end(); ++iter) {
		skill_vec.push_back(BSON(
				Gang_Fields::Researched_Skills::SKILL_ID << iter->second.skill_id
				));
	}

	std::vector<BSONObj> building_vec;
	for (Gang_Building_Map::const_iterator iter = gang_view.buildings.begin();
			iter != gang_view.buildings.end(); ++iter) {
		building_vec.push_back(BSON(
				Gang_Fields::Building::BUILDING_ID << iter->second.building_id
				<< Gang_Fields::Building::BUILDING_LVL << iter->second.building_lvl
				<< Gang_Fields::Building::CD_UPGRADE << (unsigned int)iter->second.cd_upgrade.sec()
				));
	}

	std::vector<BSONObj> gang_war_award_vec;
	for (boost::unordered_map<int64_t, Reward_Info_Gang_War>::const_iterator iter = gang_view.gang_war_award.begin();
			iter != gang_view.gang_war_award.end(); ++iter) {
		gang_war_award_vec.push_back(BSON(
				Role_Fields::ROLE_ID << (long long int)iter->second.role_id
				<< Role_Fields::ROLE_NAME << iter->second.role_name
				<< Gang_Fields::Reward_Info_Gang_War::INDEX << (long long int)iter->second.index
				<< Gang_Fields::Reward_Info_Gang_War::REWARD_ID << iter->second.award.reward_id
				<< Gang_Fields::Reward_Info_Gang_War::QUANTITY << iter->second.award.quantity
				<< Gang_Fields::Reward_Info_Gang_War::VIP_ADD << iter->second.award.vip_add
				));
	}
	std::vector<BSONObj> level_leader_id_vec;
	for(boost::unordered_map<int32_t, role_id_t>::const_iterator iter = gang_view.level_leader_id.begin(); iter != gang_view.level_leader_id.end(); ++iter) {
		level_leader_id_vec.push_back(BSON(
				Common_Fields::TYPE << iter->first
				<< Common_Fields::VALUE << (long long int)iter->second
		));
	}

	BSONObjBuilder set_builder;
	set_builder << Gang_Fields::GANG_ID << (long long int)gang_view.gang_id
				<< Gang_Fields::LEADER << (long long int)gang_view.leader
				<< Gang_Fields::LAST_LEADER_ID << (long long int)gang_view.last_leader_id
				<< Gang_Fields::NOTIFIED_LAST_LEADER << gang_view.notified_last_leader
				<< Gang_Fields::FUND << gang_view.fund
				<< Gang_Fields::MAINTAIN_COST << gang_view.maintain_cost
				<< Gang_Fields::FUND_LACK_TIMES << gang_view.fund_lack_times
				<< Gang_Fields::LEVEL << gang_view.level
				<< Gang_Fields::BOSS_ACT << gang_view.boss_act
				<< Gang_Fields::BOSS_HOUR << gang_view.boss_hour
				<< Gang_Fields::BOSS_MINUTE << gang_view.boss_minute
				<< Gang_Fields::BOSS_KILL_NUM << gang_view.boss_kill_num
				<< Gang_Fields::BOSS_LV << gang_view.boss_lv
				<< Gang_Fields::RANK << gang_view.rank
				<< Gang_Fields::MAX_MEMBERS << gang_view.max_members
				<< Gang_Fields::LOGO << gang_view.logo
				<< Gang_Fields::DAY_DEFICIT_COUNT << gang_view.day_deficit_count
				<< Gang_Fields::STATUS << gang_view.status
				<< Gang_Fields::JOIN_BLOCK << gang_view.join_block
				<< Gang_Fields::CD_UPGRADE << (unsigned int)gang_view.cd_upgrade.sec()
				<< Gang_Fields::CREATE_TIME << (unsigned int)gang_view.create_time.sec()
				<< Gang_Fields::DISMISS_TIME << (unsigned int)gang_view.dismiss_time.sec()
				<< Gang_Fields::TV_LEADER_OFFLINE << (unsigned int)gang_view.tv_leader_offline.sec()
				<< Gang_Fields::ANNOUNCEMENT << gang_view.announcement
				<< Gang_Fields::NAME << gang_view.name
				<< Gang_Fields::SERVER_INFO << (BSON(Gang_Fields::Server_Info::AGENT_NUM << gang_view.server_info.agent_num
													<< Gang_Fields::Server_Info::SERVER_NUM << gang_view.server_info.server_num ))
				<< Gang_Fields::MEMBERS << member_vec
				<< Gang_Fields::MEMBER_SIZE << (unsigned int)member_vec.size()
				<< Gang_Fields::AFFAIRS << affair_vec
				<< Gang_Fields::RESEARCHED_SKILLS << skill_vec
				<< Gang_Fields::BUILDINGS << building_vec
				<< Gang_Fields::REWARD_INFO_GANG_WAR << gang_war_award_vec
				<< Gang_Fields::LEVEL_LEADER_ID << level_leader_id_vec
				<< Gang_Fields::ACTIVE_ANNON_ED << gang_view.acitve_annon_ed;
				;

	MONGO_CONNECTION.update(Gang_Fields::COLLECTION, QUERY(Gang_Fields::GANG_ID << (long long int)gang_view.gang_id),
			BSON("$set" << set_builder.obj() ), true);
	return 0;
}

int DB_Operator::load_gang_war_info(Gang_War_Info& gang_war_info) {
	unsigned long long row_count = MONGO_CONNECTION.count( Gang_War_Info_Fields::COLLECTION );
	BSONObjBuilder field_builder;
	field_builder 	<< Gang_War_Info_Fields::ACTIVITY_TIME << 1
					<< Gang_War_Info_Fields::STAGE << 1
					<< Gang_War_Info_Fields::SWITCH << 1
					<< Gang_War_Info_Fields::GANG_WAR_MATCHED_GANGS_PAIR_VEC << 1
					<< Gang_War_Info_Fields::GANG_WAR_MATCHED_GANG_MAP << 1
					<< Gang_War_Info_Fields::GANG_WAR_SPOT_RANK_MAP << 1
					<< Gang_War_Info_Fields::GANG_WAR_RESULT_MAP << 1
					<< Gang_War_Info_Fields::GANG_WAR_RANK_AWARD << 1
					;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve( row_count );

//	MONGO_CONNECTION.findN(iter_record, Gang_War_Info_Fields::COLLECTION, mongo::Query(), row_count, 0, &field_bson);
	MONGO_CONNECTION.findN(iter_record, Gang_War_Info_Fields::COLLECTION,
			mongo::Query(BSON(Gang_War_Info_Fields::ACTIVITY_TIME << (long long int) gang_war_info.activity_time_.sec())),
			row_count);
	int count = 0;
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj& obj = *iter;

//		gang_war_info.activity_time_.sec(obj[Gang_War_Info_Fields::ACTIVITY_TIME].numberLong());
		gang_war_info.stage_ = obj[Gang_War_Info_Fields::STAGE].numberInt();
		gang_war_info.switch_ = obj[Gang_War_Info_Fields::SWITCH].numberInt();
		gang_war_info.open_time_for_each_match.sec(obj[Gang_War_Info_Fields::Gang_War_Info_Fields::OPEN_TIME_FOR_EACH_MATCH].numberLong());

		{
			BSONObj gang_war_matched_gangs_pair_vec = obj.getObjectField(Gang_War_Info_Fields::GANG_WAR_MATCHED_GANGS_PAIR_VEC.c_str());
			Gang_War_Matched_Gangs_Pair gwmgp;
			BSONObjIterator it_gang_war_matched_gangs_pair_vec(gang_war_matched_gangs_pair_vec);
			while (it_gang_war_matched_gangs_pair_vec.more()) {
				const BSONObj& obj_skill = it_gang_war_matched_gangs_pair_vec.next().embeddedObject();
				gwmgp.reset();
				gwmgp.matched_index = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_INDEX].numberInt();
				gwmgp.scene_index = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::SCENE_INDEX].numberLong();
				gwmgp.match_time.sec(obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCH_TIME].numberLong());
				gwmgp.result = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::RESULT].numberInt();
				gwmgp.matched_pair.first = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_PAIR_FIRST].numberLong();
				gwmgp.matched_pair.second = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_PAIR_SECOND].numberLong();
				gang_war_info.gang_war_matched_gangs_pair_vec_.push_back(gwmgp);
			}
		}
		{
			BSONObj gang_war_matched_gang_map = obj.getObjectField(Gang_War_Info_Fields::GANG_WAR_MATCHED_GANGS_PAIR_VEC.c_str());
			Gang_War_Matched_Gang_Info gwmgi;
			BSONObjIterator iter_gwmgm(gang_war_matched_gang_map);
			while (iter_gwmgm.more()) {
				const BSONObj& obj_skill = iter_gwmgm.next().embeddedObject();
				gwmgi.reset();
				int64_t gang_id = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::KEY].numberLong();
				gwmgi.matched_index = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::MATCHED_INDEX].numberLong();
				gwmgi.scene_index = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::SCENE_INDEX].numberLong();
				gwmgi.camp = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::CAMP].numberInt();
				gwmgi.match_time.sec(obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::MATCH_TIME].numberLong());
				gwmgi.zone = obj_skill[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::ZONE].valuestrsafe();
				gwmgi.gang_name = obj_skill[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME].valuestrsafe();
				gwmgi.rank = obj_skill[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::RANK].numberInt();
				gwmgi.force = obj_skill[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::FORCE].numberLong();
				gwmgi.btn_state = obj_skill[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::BTN_STATE].numberInt();
				gang_war_info.gang_war_matched_gang_map_.insert(std::make_pair(gang_id, gwmgi));
			}
		}
		{
			BSONObj gang_war_spot_rank_map = obj.getObjectField(Gang_War_Info_Fields::GANG_WAR_SPOT_RANK_MAP.c_str());
			std::vector<Gang_War_Rank_Info> gwri_vec;
			Gang_War_Rank_Info gwri;
			BSONObjIterator it_gwsrm(gang_war_spot_rank_map);
			while (it_gwsrm.more()) {
				const BSONObj& obj_skill = it_gwsrm.next().embeddedObject();
				int8_t key = obj_skill[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::KEY].numberInt();
				gwri_vec.clear();
				BSONObj value = obj_skill.getObjectField(Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::VALUE.c_str());
				BSONObjIterator it_value(value);
				while (it_value.more()) {
					const BSONObj &obj_value = it_value.next().embeddedObject();
					gwri.reset();
					gwri.role_id = obj_value[Role_Fields::ROLE_ID].numberLong();
					gwri.role_name = obj_value[Role_Fields::ROLE_NAME].valuestrsafe();
					gwri.kill_num = obj_value[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::KILL_NUM].numberInt();
					gwri.die_num = obj_value[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::DIE_NUM].numberInt();
					gwri.hold_spot = obj_value[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::HOLD_SPOT].numberInt();
					gwri.headship = obj_value[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::HEADSHIP].numberInt();
					gwri.force = obj_value[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::FORCE].numberInt();
					gwri.gang_name = obj_value[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::GANG_NAME].valuestrsafe();
					gwri.camp = obj_value[Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::CAMP].numberInt();
					gwri_vec.push_back(gwri);
				}
				gang_war_info.gang_war_spot_rank_map_.insert(std::make_pair(key, gwri_vec));
			}
		}
		{
			BSONObj gang_war_result_map = obj.getObjectField(Gang_War_Info_Fields::GANG_WAR_RESULT_MAP.c_str());
			Gang_War_Spot gws;
			Gang_Basic_Detail gbd;
			BSONObjIterator it_gwrm(gang_war_result_map);
			while (it_gwrm.more()) {
				const BSONObj& obj_skill = it_gwrm.next().embeddedObject();
				gws.reset();
				gws.index = obj_skill[Gang_War_Info_Fields::Gang_War_Result_Map::INDEX].numberInt();
				gws.result = obj_skill[Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::RESULT].numberInt();
				BSONObj gang_vec = obj_skill.getObjectField(Gang_War_Info_Fields::Gang_War_Result_Map::GANG_VEC.c_str());
				BSONObjIterator it_gang_vec(gang_vec);
				while (it_gang_vec.more()) {
					const BSONObj &obj_value = it_gang_vec.next().embeddedObject();
					gbd.reset();
					gbd.gang_id = obj_value[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_ID].numberLong();
					gbd.gang_name = obj_value[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME].valuestrsafe();
					gbd.index = obj_value[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::INDEX].numberInt();
					gbd.zone = obj_value[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::ZONE].valuestrsafe();
					gbd.rank = obj_value[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::RANK].numberInt();
					gws.gang_vec.push_back(gbd);
				}
				gang_war_info.gang_war_result_map_.insert(std::make_pair(gws.index, gws));
			}
		}
		{
			BSONObj gang_war_result_map = obj.getObjectField(Gang_War_Info_Fields::GANG_RANK_MAP.c_str());
			Gang_War_Gang_Rank_Info gws;
			BSONObjIterator it_gwrm(gang_war_result_map);
			while (it_gwrm.more()) {
				const BSONObj& obj_skill = it_gwrm.next().embeddedObject();
				gws.reset();
				gws.gang_id = obj_skill[Gang_War_Info_Fields::Gang_Rank_Map::GANG_ID].numberLong();
				gws.gang_name = obj_skill[Gang_War_Info_Fields::Gang_Rank_Map::GANG_NAME].valuestrsafe();
				gws.zone = obj_skill[Gang_War_Info_Fields::Gang_Rank_Map::ZONE].valuestrsafe();
				gws.score = obj_skill[Gang_War_Info_Fields::Gang_Rank_Map::SCORE].numberInt();
				gws.win_nums = obj_skill[Gang_War_Info_Fields::Gang_Rank_Map::WIN_NUMS].numberInt();
				gws.kill_nums = obj_skill[Gang_War_Info_Fields::Gang_Rank_Map::KILL_NUMS].numberInt();
				gws.hold_nums = obj_skill[Gang_War_Info_Fields::Gang_Rank_Map::HOLD_NUMS].numberInt();
				gws.force = obj_skill[Gang_War_Info_Fields::Gang_Rank_Map::FORCE].numberInt();
				BSONObj gang_vec = obj_skill.getObjectField(Gang_War_Info_Fields::Gang_Rank_Map::GANGER_RANK_INFO.c_str());
				BSONObjIterator it_gang_vec(gang_vec);
				Gang_War_Rank_Info gwri;
				while (it_gang_vec.more()) {
					const BSONObj &obj_value = it_gang_vec.next().embeddedObject();
					gwri.reset();
					gwri.role_id = obj_value[Role_Fields::ROLE_ID].numberLong();
					gwri.gang_id = obj_value[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_ID].numberLong();

					if(!GANG_MANAGER->is_in_gang(gwri.role_id, gwri.gang_id)){
						continue;
					}

					gwri.gang_name = obj_value[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME].valuestrsafe();
					gwri.role_name = obj_value[Role_Fields::ROLE_NAME].valuestrsafe();
					gwri.kill_num = obj_value[Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::KILL_NUM].numberInt();
					gwri.die_num = obj_value[Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::DIE_NUM].numberInt();
					gwri.hold_spot = obj_value[Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::HOLD_SPOT].numberInt();
					gwri.headship = obj_value[Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::HEADSHIP].numberInt();
					gwri.force = obj_value[Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::FORCE].numberInt();
					gwri.camp = obj_value[Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::CAMP].numberInt();
					gws.ganger_rank_info.insert(std::make_pair(gwri.role_id, gwri));
				}
				gang_war_info.gang_rank_map.insert(std::make_pair(gws.gang_id, gws));
			}
		}
		{
			BSONObj gang_war_result_map = obj.getObjectField(Gang_War_Info_Fields::GANG_WAR_RANK_AWARD.c_str());
			Gang_War_Rank_Award_Detail gwrad;
			BSONObjIterator it_gwrm(gang_war_result_map);
			while (it_gwrm.more()) {
				const BSONObj& obj_skill = it_gwrm.next().embeddedObject();
				gwrad.reset();
				gwrad.rank = obj_skill[Gang_War_Info_Fields::Gang_War_Result_Map::INDEX].numberInt();
				gwrad.gang_id = obj_skill[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_ID].numberLong();
				gwrad.state = obj_skill[Gang_War_Info_Fields::Gang_War_Rank_Award::STATE].numberInt();
				gwrad.score = obj_skill[Gang_War_Info_Fields::Gang_War_Rank_Award::SCORE].numberInt();
				gwrad.win_nums = obj_skill[Gang_War_Info_Fields::Gang_War_Rank_Award::WIN_NUMS].numberInt();
				gwrad.kill_nums = obj_skill[Gang_War_Info_Fields::Gang_War_Rank_Award::KILL_NUMS].numberInt();
				gwrad.force = obj_skill[Gang_War_Info_Fields::Gang_War_Rank_Award::FORCE].numberInt();
				gwrad.gang_name = obj_skill[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME].valuestrsafe();
				gwrad.zone = obj_skill[Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::ZONE].valuestrsafe();
				gang_war_info.rank_award_.insert(std::make_pair(gwrad.rank, gwrad));
				gang_war_info.gang_id_rank_award_.insert(std::make_pair(gwrad.gang_id, gwrad));
			}
		}
	}
	LOG_DEBUG("load_gang_war_info, size:%d", count);
	return 0;
}

int DB_Operator::save_gang_war_info(const Gang_War_Info& gang_war_info) {
	std::vector<BSONObj> gang_war_matched_gangs_pair_vec;
	for (GangWarMatchedGangsPair_Vec::const_iterator it = gang_war_info.gang_war_matched_gangs_pair_vec_.begin();
			it != gang_war_info.gang_war_matched_gangs_pair_vec_.end(); ++it) {
		gang_war_matched_gangs_pair_vec.push_back(BSON(
				Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_INDEX << it->matched_index
				<< Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::SCENE_INDEX << (long long int)it->scene_index
				<< Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCH_TIME << (long long int)it->match_time.sec()
				<< Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::RESULT << it->result
				<< Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_PAIR_FIRST << (long long int)it->matched_pair.first
				<< Gang_War_Info_Fields::Gang_War_Matched_Gangs_Pair_Vec::MATCHED_PAIR_SECOND << (long long int)it->matched_pair.second
				));
	}
	std::vector<BSONObj> gang_war_matched_gang_map;
	for (GangWarMatchedGangInfo_Map::const_iterator it = gang_war_info.gang_war_matched_gang_map_.begin();
			it != gang_war_info.gang_war_matched_gang_map_.end(); ++it) {
		gang_war_matched_gang_map.push_back(BSON(
				Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::KEY << (long long int)it->first
				<< Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::MATCHED_INDEX << (long long int)it->second.matched_index
				<< Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::SCENE_INDEX << (long long int)it->second.scene_index
				<< Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::MATCH_TIME << (long long int)it->second.match_time.sec()
				<< Gang_War_Info_Fields::Gang_War_Matched_Gang_Map::CAMP << it->second.camp
				<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::ZONE << it->second.zone
				<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME << it->second.gang_name
				<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::RANK << it->second.rank
				<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::FORCE << (long long int)it->second.force
				<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::BTN_STATE << it->second.btn_state
				));
	}
	std::vector<BSONObj> gang_war_spot_rank_map;
	for (std::map<int8_t, std::vector<Gang_War_Rank_Info> >::const_iterator it = gang_war_info.gang_war_spot_rank_map_.begin();
			it != gang_war_info.gang_war_spot_rank_map_.end(); ++it) {
		std::vector<BSONObj> gang_war_rank_info;
		for (std::vector<Gang_War_Rank_Info>::const_iterator it_rank = it->second.begin();
				it_rank != it->second.end(); ++it_rank) {
			gang_war_rank_info.push_back(BSON(
					Role_Fields::ROLE_ID << (long long int)it_rank->role_id
					<< Role_Fields::ROLE_NAME << it_rank->role_name
					<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::KILL_NUM << it_rank->kill_num
					<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::DIE_NUM << it_rank->die_num
					<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::HOLD_SPOT << it_rank->hold_spot
					<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::HEADSHIP << it_rank-> headship
					<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::FORCE << it_rank->force
					<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::GANG_NAME << it_rank->gang_name
					<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::CAMP << it_rank->camp
					));
		}
		gang_war_spot_rank_map.push_back(BSON(
				Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::KEY << it->first
				<< Gang_War_Info_Fields::Gang_War_Spot_Rank_Map::VALUE << gang_war_rank_info
				));
	}

	std::vector<BSONObj> gang_war_result_map;
	for (std::map<int8_t, Gang_War_Spot>::const_iterator it = gang_war_info.gang_war_result_map_.begin();
			it != gang_war_info.gang_war_result_map_.end(); ++it) {
		std::vector<BSONObj> gbd;
		for (std::vector<Gang_Basic_Detail>::const_iterator it_gbd = it->second.gang_vec.begin();
				it_gbd != it->second.gang_vec.end(); ++it_gbd) {
			gbd.push_back(BSON(
					Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_ID << (long long int)it_gbd->gang_id
					<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME << it_gbd->gang_name
					<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::INDEX << it_gbd->index
					<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::ZONE << it_gbd->zone
					<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::RANK << it_gbd->rank
			));
		}
		gang_war_result_map.push_back(BSON(
				Gang_War_Info_Fields::Gang_War_Result_Map::INDEX << it->second.index
				<< Gang_War_Info_Fields::Gang_War_Result_Map::RESULT << it->second.result
				<< Gang_War_Info_Fields::Gang_War_Result_Map::GANG_VEC << gbd
				));
	}

	std::vector<BSONObj> gang_rank_map;
	{
		for (GangWarGangRankInfo_Map::const_iterator it = gang_war_info.gang_rank_map.begin();
				it != gang_war_info.gang_rank_map.end(); ++it) {
			std::vector<BSONObj> gbd;
			for (Ganer_Rank_Info::const_iterator it_gbd = it->second.ganger_rank_info.begin();
					it_gbd != it->second.ganger_rank_info.end(); ++it_gbd) {
				gbd.push_back(BSON(
						Ganger_Fields::ROLE_ID << (long long int) it_gbd->second.role_id
						<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_ID << (long long int)it_gbd->second.gang_id
						<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME << it_gbd->second.gang_name
						<< Role_Fields::ROLE_NAME << it_gbd->second.role_name
						<< Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::KILL_NUM << it_gbd->second.kill_num
						<< Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::DIE_NUM << it_gbd->second.die_num
						<< Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::HOLD_SPOT << it_gbd->second.hold_spot
						<< Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::HEADSHIP << it_gbd->second.headship
						<< Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::FORCE << it_gbd->second.force
						<< Gang_War_Info_Fields::Gang_Rank_Map::Ganger_Rank_Info::CAMP << it_gbd->second.camp
				));
			}
			gang_rank_map.push_back(BSON(
					Gang_War_Info_Fields::Gang_Rank_Map::GANG_ID << (long long int)it->second.gang_id
					<< Gang_War_Info_Fields::Gang_Rank_Map::GANG_NAME << it->second.gang_name
					<< Gang_War_Info_Fields::Gang_Rank_Map::ZONE << it->second.zone
					<< Gang_War_Info_Fields::Gang_Rank_Map::SCORE << it->second.score
					<< Gang_War_Info_Fields::Gang_Rank_Map::WIN_NUMS << it->second.win_nums
					<< Gang_War_Info_Fields::Gang_Rank_Map::KILL_NUMS << it->second.kill_nums
					<< Gang_War_Info_Fields::Gang_Rank_Map::HOLD_NUMS << it->second.hold_nums
					<< Gang_War_Info_Fields::Gang_Rank_Map::FORCE << it->second.force
					<< Gang_War_Info_Fields::Gang_Rank_Map::GANGER_RANK_INFO << gbd
					));
		}
	}

	std::vector<BSONObj> gang_war_rank_award_map;
	for (Gang_War_Rank_Award::const_iterator it = gang_war_info.rank_award_.begin();
			it != gang_war_info.rank_award_.end(); ++it) {
		gang_war_rank_award_map.push_back(BSON(
				Gang_War_Info_Fields::Gang_War_Result_Map::INDEX << it->second.rank
				<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_ID << (long long int)it->second.gang_id
				<< Gang_War_Info_Fields::Gang_War_Rank_Award::STATE << it->second.state
				<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::GANG_NAME << it->second.gang_name
				<< Gang_War_Info_Fields::Gang_War_Result_Map::Gang_Vec::ZONE << it->second.zone
				<< Gang_War_Info_Fields::Gang_War_Rank_Award::SCORE << it->second.score
				<< Gang_War_Info_Fields::Gang_War_Rank_Award::WIN_NUMS << it->second.win_nums
				<< Gang_War_Info_Fields::Gang_War_Rank_Award::KILL_NUMS << it->second.kill_nums
				<< Gang_War_Info_Fields::Gang_War_Rank_Award::FORCE << it->second.force
				));
	}

	{
		BSONObjBuilder set_builder;
		set_builder << Gang_War_Info_Fields::ACTIVITY_TIME << (long long int)gang_war_info.activity_time_.sec()
					<< Gang_War_Info_Fields::STAGE << gang_war_info.stage_
					<< Gang_War_Info_Fields::SWITCH << gang_war_info.switch_
					<< Gang_War_Info_Fields::Gang_War_Info_Fields::OPEN_TIME_FOR_EACH_MATCH << (long long int)gang_war_info.open_time_for_each_match.sec()
					<< Gang_War_Info_Fields::GANG_WAR_MATCHED_GANGS_PAIR_VEC << gang_war_matched_gangs_pair_vec
					<< Gang_War_Info_Fields::GANG_WAR_MATCHED_GANG_MAP << gang_war_matched_gang_map
					<< Gang_War_Info_Fields::GANG_WAR_SPOT_RANK_MAP << gang_war_spot_rank_map
					<< Gang_War_Info_Fields::GANG_WAR_RESULT_MAP << gang_war_result_map
					<< Gang_War_Info_Fields::GANG_WAR_RANK_AWARD << gang_war_rank_award_map
					<< Gang_War_Info_Fields::GANG_RANK_MAP << gang_rank_map
					;

		MONGO_CONNECTION.update(Gang_War_Info_Fields::COLLECTION, QUERY(Gang_War_Info_Fields::ACTIVITY_TIME << (long long int)gang_war_info.activity_time_.sec()),
				BSON("$set" << set_builder.obj() ), true);
	}

	{
		Time_Value now(Time_Value::gettimeofday());
		Date_Time date_now(now);
		Date_Time date_des(date_now);
		date_des.hour(0);
		date_des.minute(0);
		date_des.second(0);
		date_des.microsec(0);

		long this_week = date_des.weekday(); // date_des是星期几 , date_des.weekday() = [0-6];
		if (this_week == 0) {
			this_week = 7;
		}
		int split_day = 0; /*隔多少天*/
		if (this_week > 1) {//指定星期在本周已经过去
			split_day = - abs(this_week - 1);//Time_Value::ONE_WEEK_DAYS - (this_week - from_week);
		} else if (this_week < 1) {
			split_day = abs(this_week - 1);
			MSG_USER("it must set problem");
		}
		if(split_day != 0) {
			date_des = Date_Time(Time_Value(date_des.time_sec() + split_day * Time_Value::ONE_DAY_IN_SECS));
		}
		BSONObjBuilder set_builder;
			set_builder << Gang_War_Info_Fields::ACTIVITY_TIME << (long long int)date_des.time_sec()
						<< Gang_War_Info_Fields::STAGE << gang_war_info.stage_
						<< Gang_War_Info_Fields::SWITCH << gang_war_info.switch_
						<< Gang_War_Info_Fields::GANG_WAR_MATCHED_GANGS_PAIR_VEC << gang_war_matched_gangs_pair_vec
						<< Gang_War_Info_Fields::GANG_WAR_MATCHED_GANG_MAP << gang_war_matched_gang_map
						<< Gang_War_Info_Fields::GANG_WAR_SPOT_RANK_MAP << gang_war_spot_rank_map
						<< Gang_War_Info_Fields::GANG_WAR_RESULT_MAP << gang_war_result_map
						<< Gang_War_Info_Fields::GANG_WAR_RANK_AWARD << gang_war_rank_award_map
						<< Gang_War_Info_Fields::GANG_RANK_MAP << gang_rank_map
						;
		MONGO_CONNECTION.update(Gang_War_Info_Fields::HISTORY_COLLECTION, QUERY(Gang_War_Info_Fields::ACTIVITY_TIME << (long long int)date_des.time_sec()),
					BSON("$set" << set_builder.obj() ), true);
	}
	return 0;
}

int DB_Operator::load_ganger_detail(Ganger_Detail& detail) {
	BSONObj find_it = MONGO_CONNECTION.findOne(Ganger_Fields::COLLECTION, QUERY(Ganger_Fields::ROLE_ID << (long long int)detail.role_id));
	if ( find_it.isEmpty() ) {
		LOG_DEBUG("No role %ld ganger info", detail.role_id);
		return 0;
	}
	detail.gang_id = find_it[Ganger_Fields::GANG_ID].numberLong();
	detail.last_gang_id = find_it[Ganger_Fields::LAST_GANG_ID].numberLong();
	detail.cur_contribute = find_it[Ganger_Fields::CUR_CONTRIBUTE].numberInt();
	detail.total_contribute = find_it[Ganger_Fields::TOTAL_CONTRIBUTE].numberInt();
	detail.achi_contribute = find_it[Ganger_Fields::ACHI_CONTRIBUTE].numberInt();
	detail.day_contribute = find_it[Ganger_Fields::DAY_CONTRIBUTE].numberInt();
	detail.contrib_of_week = find_it[Ganger_Fields::CONTRIB_OF_WEEK].numberInt();
	detail.day_gold_contrib_times = find_it[Ganger_Fields::DAY_GOLD_CONTRIB_TIMES].numberInt();
	detail.day_diam_contrib_times = find_it[Ganger_Fields::DAY_DIAM_CONTRIB_TIMES].numberInt();
	detail.last_rank = find_it[Ganger_Fields::LAST_RANK].numberInt();
	detail.authority = find_it[Ganger_Fields::AUTHORITY].numberInt();
	detail.receive_bless_times = find_it[Ganger_Fields::RECEIVE_BLESS_TIMES].numberInt();
	//detail.remain_draw_times = find_it[Ganger_Fields::REMAIN_DRAW_TIMES].numberInt();
	detail.today_has_draw_times = find_it[Ganger_Fields::TODAY_HAS_DRAW_TIMES].numberInt();
	detail.join_time.sec( find_it[Ganger_Fields::JOIN_TIME].numberInt() );
	detail.leave_time.sec( find_it[Ganger_Fields::LEAVE_TIME].numberInt() );
	detail.headship  = find_it[Ganger_Fields::HEADSHIP].numberInt();
	detail.practice_times_ttl_ = find_it[Ganger_Fields::PRACTICE_TIMES_TTL].numberInt();
	detail.gang_war_attend_time.sec(find_it[Ganger_Fields::GANG_WAR_ATTEND_TIME].numberLong());
	detail.gang_war_attend_gang_id = find_it[Ganger_Fields::GANG_WAR_ATTEND_GANG_ID].numberLong();
	detail.gang_war_attend_gang_id2 = find_it[Ganger_Fields::GANG_WAR_ATTEND_GANG_ID2].numberLong();
	detail.gang_war_award_get_times_by_day = find_it[Ganger_Fields::GANG_WAR_AWARD_GET_TIMES_BY_DAY].numberInt();

	BSONObj obj_affair = find_it.getObjectField( Ganger_Fields::AFFAIRS.c_str() );
	BSONObjIterator affair_iter( obj_affair );
	while ( affair_iter.more() ) {
		BSONObj db_affair = affair_iter.next().embeddedObject();
		Ganger_Detail::Affair new_affair;
		new_affair.type = db_affair[Ganger_Fields::Affairs::TYPE].numberInt();
		new_affair.result = db_affair[Ganger_Fields::Affairs::RESULT].numberInt();
		new_affair.extra_id = db_affair[Ganger_Fields::Affairs::EXTRA_ID].numberLong();
		new_affair.expire.sec( db_affair[Ganger_Fields::Affairs::EXPIRE].numberInt() );
		new_affair.content = db_affair[Ganger_Fields::Affairs::CONTENT].str();
		detail.affairs.push_back( new_affair );
	}

	BSONObjIterator it_invite(find_it.getObjectField(Ganger_Fields::INVITES.c_str()));
	while (it_invite.more()) {
		BSONElement invite_obj = it_invite.next();
		detail.invites.push_back(invite_obj.numberLong());
	}

	BSONObjIterator it_apply_id(find_it.getObjectField(Ganger_Fields::APPLY_IDS.c_str()));
	while (it_apply_id.more()) {
		BSONElement apply_id_obj = it_apply_id.next();
		detail.apply_ids.insert(apply_id_obj.numberLong());
	}

	Ganger_Skill_Detail skill_detail;
	BSONObjIterator it_skill(find_it.getObjectField(Ganger_Fields::SKILL_DETAILS.c_str()));
	while (it_skill.more()) {
		BSONObj obj_skill = it_skill.next().embeddedObject();
		skill_detail.reset();
		skill_detail.skill_id = obj_skill[Ganger_Fields::Skill_Details::SKILL_ID].numberInt();
		detail.skill_map.insert(std::make_pair(skill_detail.skill_id, skill_detail));
	}

	BSONObjIterator it_draw_awards(find_it.getObjectField(Ganger_Fields::DRAW_AWARDS.c_str()));
	while (it_draw_awards.more()) {
		BSONElement obj_draw_awards = it_draw_awards.next();
		detail.draw_awards.push_back(obj_draw_awards.numberInt());
	}

	return 0;
}

int DB_Operator::save_ganger_detail(Ganger_Detail& detail) {
	if ( detail.role_id == 0 ) {
		//LOG_DEBUG("ganger role id == 0");
		return 1;
	}
	std::vector<BSONObj> affair_vec;
	for (Ganger_Detail::Affair_Vec::const_iterator iter = detail.affairs.begin();
			iter != detail.affairs.end(); iter++) {
		affair_vec.push_back( BSON(Ganger_Fields::Affairs::TYPE << (*iter).type
				<< Ganger_Fields::Affairs::RESULT << (*iter).result
				<< Ganger_Fields::Affairs::EXTRA_ID << (long long int)(*iter).extra_id
				<< Ganger_Fields::Affairs::EXPIRE << (long long int)(*iter).expire.sec()
				<< Ganger_Fields::Affairs::CONTENT << (*iter).content) );
	}

	std::vector<long long int> invite_id_vec;
	for (Role_Vec::const_iterator iter = detail.invites.begin();
			iter != detail.invites.end(); ++iter) {
		invite_id_vec.push_back(*iter);
	}

	std::vector<long long int> apply_id_vec;
	for (Role_Id_Set::const_iterator iter = detail.apply_ids.begin();
			iter != detail.apply_ids.end(); ++iter) {
		apply_id_vec.push_back(*iter);
	}

	std::vector<BSONObj> skill_vec;
	for (Ganger_Skill_Map::const_iterator iter = detail.skill_map.begin();
			iter != detail.skill_map.end(); ++iter) {
		skill_vec.push_back(BSON(
				Ganger_Fields::Skill_Details::SKILL_ID << iter->second.skill_id
				));
	}

	BSONObjBuilder set_builder;
	set_builder << Ganger_Fields::GANG_ID << (long long int)detail.gang_id
				<< Ganger_Fields::LAST_GANG_ID << (long long int)detail.last_gang_id
				<< Ganger_Fields::ROLE_ID << (long long int)detail.role_id
				<< Ganger_Fields::CUR_CONTRIBUTE << detail.cur_contribute
				<< Ganger_Fields::TOTAL_CONTRIBUTE << detail.total_contribute
				<< Ganger_Fields::ACHI_CONTRIBUTE << detail.achi_contribute
				<< Ganger_Fields::DAY_CONTRIBUTE << detail.day_contribute
				<< Ganger_Fields::CONTRIB_OF_WEEK << detail.contrib_of_week
				<< Ganger_Fields::DAY_GOLD_CONTRIB_TIMES << detail.day_gold_contrib_times
				<< Ganger_Fields::DAY_DIAM_CONTRIB_TIMES << detail.day_diam_contrib_times
				<< Ganger_Fields::LAST_RANK << detail.last_rank
				<< Ganger_Fields::AUTHORITY << detail.authority
				<< Ganger_Fields::RECEIVE_BLESS_TIMES << detail.receive_bless_times
				//<< Ganger_Fields::REMAIN_DRAW_TIMES << detail.remain_draw_times
				<< Ganger_Fields::TODAY_HAS_DRAW_TIMES << detail.today_has_draw_times
				<< Ganger_Fields::JOIN_TIME << (unsigned int)detail.join_time.sec()
				<< Ganger_Fields::LEAVE_TIME << (unsigned int)detail.leave_time.sec()
				<< Ganger_Fields::HEADSHIP << detail.headship
				<< Ganger_Fields::PRACTICE_TIMES_TTL << detail.practice_times_ttl_
				<< Ganger_Fields::AFFAIRS << affair_vec
				<< Ganger_Fields::INVITES << invite_id_vec
				<< Ganger_Fields::APPLY_IDS << apply_id_vec
				<< Ganger_Fields::SKILL_DETAILS << skill_vec
				<< Ganger_Fields::DRAW_AWARDS << detail.draw_awards
				<< Ganger_Fields::GANG_WAR_ATTEND_TIME << (long long int) detail.gang_war_attend_time.sec()
				<< Ganger_Fields::GANG_WAR_ATTEND_GANG_ID << (long long int)detail.gang_war_attend_gang_id
				<< Ganger_Fields::GANG_WAR_ATTEND_GANG_ID2 << (long long int)detail.gang_war_attend_gang_id2
				<< Ganger_Fields::GANG_WAR_AWARD_GET_TIMES_BY_DAY << detail.gang_war_award_get_times_by_day
				;

	MONGO_CONNECTION.update(Ganger_Fields::COLLECTION, QUERY(Ganger_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << set_builder.obj() ), true);
	return 0;
}

int DB_Operator::load_sys_mail_list(Sys_Mail_Vec &sys_mail_) {
	BSONObj res = MONGO_CONNECTION.findOne(Mail_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)0));
	if (res.isEmpty()) {
		return 0;
	}
	BSONObj mail = res.getObjectField(Mail_Fields::MAIL_LIST.c_str());
	BSONObjIterator iter(mail);
	while (iter.more()) {
		BSONObj obj = iter.next().embeddedObject();
		Mail_Data mail_info_detal;
		mail_info_detal.reset();
		mail_info_detal.mail_id = obj[Mail_Fields::Mail_List::MAIL_ID].numberInt();
		mail_info_detal.have_read = obj[Mail_Fields::Mail_List::HAVE_READ].numberInt();
		mail_info_detal.gold = obj[Mail_Fields::Mail_List::COIN].numberInt();
		mail_info_detal.mail_title = obj[Mail_Fields::Mail_List::MAIL_TITLE].valuestrsafe();
		mail_info_detal.mail_content = obj[Mail_Fields::Mail_List::MAIL_CONTENT].valuestrsafe();
		mail_info_detal.sender_name = obj[Mail_Fields::Mail_List::SENDER_NAME].valuestrsafe();
		mail_info_detal.show_send_time.sec(obj[Mail_Fields::Mail_List::SHOW_SEND_TIME].numberInt());
		mail_info_detal.real_send_time.sec(obj[Mail_Fields::Mail_List::REAL_SEND_TIME].numberInt());
			//加载附件信息
		BSONObjIterator item_iter(obj.getObjectField(Mail_Fields::Mail_List::ITEM.c_str()));
		while (item_iter.more()) {
			BSONObj item_obj = item_iter.next().embeddedObject();
			Item_Detail item;
			int result = load_item_detail(item_obj, item);
			if (result != 0)    continue;

			mail_info_detal.item_vector.push_back(item);
		}
		/////////////////////////
		sys_mail_.push_back(mail_info_detal);
	}

	return 0;
}

int DB_Operator::save_sys_mail_list(Mail_Data &mail_info_) {
	BSONObjBuilder builder;
	std::vector<BSONObj> item_vector;
	//BSONObj obj;
	for(Item_Vec::const_iterator item_iter = mail_info_.item_vector.begin(); item_iter != mail_info_.item_vector.end(); item_iter++ ) {
		item_vector.push_back( BSON(Package_Fields::Item::INDEX << item_iter->index_ << Package_Fields::Item::ID
				<< item_iter->id_ << Package_Fields::Item::AMOUNT << item_iter->amount_
				<< Package_Fields::Item::BIND << item_iter->bind_
				<< Package_Fields::Item::OVERDUE_TIME << (long long int)item_iter->overdue_time.sec()));
	}
	BSONObj mail_obj = BSON( Mail_Fields::Mail_List::MAIL_ID << (int)mail_info_.mail_id
								<< Mail_Fields::Mail_List::HAVE_READ << (int)mail_info_.have_read
								<< Mail_Fields::Mail_List::COIN << mail_info_.gold
								<< Mail_Fields::Mail_List::MAIL_TITLE << mail_info_.mail_title
								<< Mail_Fields::Mail_List::MAIL_CONTENT << mail_info_.mail_content
								<< Mail_Fields::Mail_List::SENDER_NAME << mail_info_.sender_name
								<< Mail_Fields::Mail_List::ITEM << item_vector
								<< Mail_Fields::Mail_List::SHOW_SEND_TIME << (long long int)mail_info_.show_send_time.sec()
								<< Mail_Fields::Mail_List::REAL_SEND_TIME << (long long int)mail_info_.real_send_time.sec());
	builder.append(Mail_Fields::MAIL_LIST, mail_obj);

	MONGO_CONNECTION.update(Mail_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)0),
			BSON("$push" << builder.obj() ), true);

	return 0;
}

int DB_Operator::load_arenaer_detail(Arenaer_Detail &detail){
	BSONObj find_it = MONGO_CONNECTION.findOne(Arenaer_Fields::COLLECTION, QUERY(Arenaer_Fields::ROLE_ID << (long long int)detail.role_id));
	if ( find_it.isEmpty() ) {
		return 0;
	}
	detail.role_id = find_it[Arenaer_Fields::ROLE_ID].numberLong();
	detail.group = find_it[Arenaer_Fields::GROUP].numberInt();
	detail.seg = find_it[Arenaer_Fields::SEG].numberInt();
	detail.score = find_it[Arenaer_Fields::SCORE].numberInt();
	detail.rise_fail = find_it[Arenaer_Fields::RISE_FAIL].numberInt();
	detail.rise_win = find_it[Arenaer_Fields::RISE_WIN].numberInt();
	detail.rise_lose = find_it[Arenaer_Fields::RISE_LOSE].numberInt();
	detail.rank = find_it[Arenaer_Fields::RANK].numberInt();
	detail.king_time = find_it[Arenaer_Fields::KING_TIME].numberInt();
	detail.enter_num = find_it[Arenaer_Fields::ENTER_NUM].numberInt();
	detail.pay_times = find_it[Arenaer_Fields::PAY_TIMES].numberInt();
	detail.buy_pay_times = find_it[Arenaer_Fields::BUY_PAY_TIMES].numberInt();
	detail.streak_win = find_it[Arenaer_Fields::STREAK_WIN].numberInt();
	detail.streak_lose = find_it[Arenaer_Fields::STREAK_LOSE].numberInt();
	detail.compensate_times = find_it[Arenaer_Fields::COMPENSATE_TIMES].numberInt();
	detail.three_day_rank = find_it[Arenaer_Fields::THREE_DAY_RANK].numberInt();
	detail.seven_day_rank = find_it[Arenaer_Fields::SEVEN_DAY_RANK].numberInt();
	detail.cd.sec(find_it[Arenaer_Fields::CD].numberInt());
	detail.match_time.sec(find_it[Arenaer_Fields::MATCH_TIME].numberInt());
	detail.match_player_1 = find_it[Arenaer_Fields::MATCH_PLAYER_1].numberLong();
	detail.match_player_2 = find_it[Arenaer_Fields::MATCH_PLAYER_2].numberLong();
	detail.match_player_3 = find_it[Arenaer_Fields::MATCH_PLAYER_3].numberLong();
	detail.match_dlv_1 = find_it[Arenaer_Fields::MATCH_DLV_1].numberInt();
	detail.match_dlv_2 = find_it[Arenaer_Fields::MATCH_DLV_2].numberInt();
	detail.match_dlv_3 = find_it[Arenaer_Fields::MATCH_DLV_3].numberInt();
	detail.fight_times = find_it[Arenaer_Fields::FIGHT_TIMES].numberInt();
//	detail.arena_worship = find_it[Arenaer_Fields::ARENA_WORSHIP].numberInt();
	detail.be_worship = find_it[Arenaer_Fields::BE_WORSHIP].numberInt();
	if (find_it.hasField(Arenaer_Fields::BATTLE_REPORT.c_str())) {
		BSONObjIterator battle_info_iter(find_it.getObjectField(Arenaer_Fields::BATTLE_REPORT.c_str()));
		Arena_Battle_Report battle_info;
		while (battle_info_iter.more()){
			battle_info.reset();
			BSONElement battle_info_obj = battle_info_iter.next();
			battle_info.role_name = battle_info_obj[Arenaer_Fields::Battle_Report::ROLE_NAME].valuestrsafe();
			battle_info.score = battle_info_obj[Arenaer_Fields::Battle_Report::SCORE].numberInt();
			battle_info.gold = battle_info_obj[Arenaer_Fields::Battle_Report::GOLD].numberInt();
			battle_info.val1 = battle_info_obj[Arenaer_Fields::Battle_Report::VAL1].numberInt();
			battle_info.val2 = battle_info_obj[Arenaer_Fields::Battle_Report::VAL2].numberInt();
			battle_info.result = battle_info_obj[Arenaer_Fields::Battle_Report::RESULT].numberInt();
			if(battle_info_obj.embeddedObject().hasField(Arenaer_Fields::Battle_Report::ITEM.c_str())){
				BSONObjIterator item_info_iter(battle_info_obj.embeddedObject().getObjectField(Arenaer_Fields::Battle_Report::ITEM.c_str()));
				Reward_Info item;
				while (item_info_iter.more()){
					BSONElement item_info_obj = item_info_iter.next();
					item.reset();
					item.reward_id = item_info_obj[Arenaer_Fields::Battle_Report::Item::ITEM].numberInt();
					item.quantity = item_info_obj[Arenaer_Fields::Battle_Report::Item::COUNT].numberInt();
					battle_info.item_vec.push_back(item);
				}
			}
			detail.report_vec.push_back(battle_info);
		}
    }

	return 0;
}

int DB_Operator::save_arenaer_detail(const Arenaer_Detail &detail){
	std::vector<BSONObj> report_vector;
	std::vector<Arena_Battle_Report>::const_iterator it;
	for (it = detail.report_vec.begin(); it != detail.report_vec.end(); it++) {
		std::vector<BSONObj> item_vector;
		std::vector<Reward_Info>::const_iterator item_iter;
		for(item_iter = it->item_vec.begin(); item_iter != it->item_vec.end(); item_iter++ ) {
			item_vector.push_back(
					BSON(Arenaer_Fields::Battle_Report::Item::ITEM << (int)item_iter->reward_id
					     << Arenaer_Fields::Battle_Report::Item::COUNT << (int)item_iter->quantity
					)
			);
		}
		report_vector.push_back(
			BSON(Arenaer_Fields::Battle_Report::ROLE_NAME << it->role_name
					<< Arenaer_Fields::Battle_Report::RESULT << (int)it->result
					<< Arenaer_Fields::Battle_Report::SCORE << (int)it->score
					<< Arenaer_Fields::Battle_Report::GOLD << (int)it->gold
					<< Arenaer_Fields::Battle_Report::RESULT << (int)it->result
					<< Arenaer_Fields::Battle_Report::VAL1 << (int)it->val1
					<< Arenaer_Fields::Battle_Report::VAL2 << (int)it->val2
					<< Arenaer_Fields::Battle_Report::ITEM << item_vector
			)
		);

	}
	BSONObjBuilder set_builder;
	set_builder << Arenaer_Fields::ROLE_ID << (long long int)detail.role_id
			   << Arenaer_Fields::GROUP << (int)detail.group
			   << Arenaer_Fields::SEG << (int)detail.seg
			   << Arenaer_Fields::SCORE << (int)detail.score
			   << Arenaer_Fields::RISE_FAIL << (int)detail.rise_fail
			   << Arenaer_Fields::RISE_WIN << (int)detail.rise_win
			   << Arenaer_Fields::RISE_LOSE << (int)detail.rise_lose
			   << Arenaer_Fields::RANK << (int)detail.rank
			   << Arenaer_Fields::KING_TIME << (int)detail.king_time
			   << Arenaer_Fields::ENTER_NUM << (int)detail.enter_num
			   << Arenaer_Fields::PAY_TIMES << (int)detail.pay_times
			   << Arenaer_Fields::BUY_PAY_TIMES << (int)detail.buy_pay_times
			   << Arenaer_Fields::STREAK_WIN << (int)detail.streak_win
			   << Arenaer_Fields::STREAK_LOSE << (int)detail.streak_lose
			   << Arenaer_Fields::COMPENSATE_TIMES << (int)detail.compensate_times
			   << Arenaer_Fields::THREE_DAY_RANK << (int)detail.three_day_rank
			   << Arenaer_Fields::SEVEN_DAY_RANK << (int)detail.seven_day_rank
			   << Arenaer_Fields::CD << (int)detail.cd.sec()
			   << Arenaer_Fields::MATCH_TIME << (int)detail.match_time.sec()
			   << Arenaer_Fields::MATCH_PLAYER_1 << (long long int)detail.match_player_1
			   << Arenaer_Fields::MATCH_PLAYER_2 << (long long int)detail.match_player_2
			   << Arenaer_Fields::MATCH_PLAYER_3 << (long long int)detail.match_player_3
			   << Arenaer_Fields::MATCH_DLV_1 << detail.match_dlv_1
			   << Arenaer_Fields::MATCH_DLV_2 << detail.match_dlv_2
			   << Arenaer_Fields::MATCH_DLV_3 << detail.match_dlv_3
			   << Arenaer_Fields::FIGHT_TIMES << (int)detail.fight_times
//			   << Arenaer_Fields::ARENA_WORSHIP << detail.arena_worship
			   << Arenaer_Fields::BE_WORSHIP << (int)detail.be_worship
			   << Arenaer_Fields::BATTLE_REPORT << report_vector;
	MONGO_CONNECTION.update(Arenaer_Fields::COLLECTION, QUERY(Arenaer_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << set_builder.obj() ), true);
	return 0;
}

int DB_Operator::load_arena_season_detail(Arena_Season_Detail &detail){
	BSONObj find_it = MONGO_CONNECTION.findOne(Arena_Season_Fields::COLLECTION, Query());
	if ( find_it.isEmpty() ) {
		return 0;
	}
	detail.season_status = find_it[Arena_Season_Fields::SEASON_STATUS].numberInt();
	int begin_time = find_it[Arena_Season_Fields::SEASON_BEGIN].numberInt();
	int finish_time = find_it[Arena_Season_Fields::SEASON_FINISH].numberInt();
	detail.season_begin.sec(begin_time);
	detail.season_finish.sec(finish_time);

	if (find_it.hasField(Arena_Season_Fields::SEASON_RESULT.c_str())) {
		BSONObjIterator battle_info_iter(find_it.getObjectField(Arena_Season_Fields::SEASON_RESULT.c_str()));
		Arena_Season_Result battle_info;
		while (battle_info_iter.more()){
			battle_info.reset();
			BSONElement battle_info_obj = battle_info_iter.next();
			battle_info.player_id = battle_info_obj[Arena_Season_Fields::Season_Result::ROLE_ID].numberLong();
			battle_info.level = battle_info_obj[Arena_Season_Fields::Season_Result::LEVEL].numberInt();
			battle_info.group_index = battle_info_obj[Arena_Season_Fields::Season_Result::GROUP_INDEX].numberInt();
			battle_info.rank = battle_info_obj[Arena_Season_Fields::Season_Result::RANK].numberInt();
			battle_info.inner_rank = battle_info_obj[Arena_Season_Fields::Season_Result::INNER_RANK].numberInt();
			detail.season_result_map.insert(std::make_pair(battle_info.player_id, battle_info));
		}
    }
	return 0;
}

int DB_Operator::save_arena_season_detail(const Arena_Season_Detail &detail){
	std::vector<BSONObj> result_vector;
	Arena_Season_Result_Map::const_iterator it;
	for (it = detail.season_result_map.begin(); it != detail.season_result_map.end(); it++) {
		result_vector.push_back(
			BSON(Arena_Season_Fields::Season_Result::ROLE_ID << (long long int)it->second.player_id
					<< Arena_Season_Fields::Season_Result::LEVEL << (int)it->second.level
					<< Arena_Season_Fields::Season_Result::GROUP_INDEX << (int)it->second.group_index
					<< Arena_Season_Fields::Season_Result::RANK << (int)it->second.rank
					<< Arena_Season_Fields::Season_Result::INNER_RANK << (int)it->second.inner_rank
			)
		);

	}
	int begin_time = detail.season_begin.sec();
	int finish_time = detail.season_finish.sec();
	BSONObjBuilder set_builder;
	set_builder << Arena_Season_Fields::SEASON_STATUS << detail.season_status
			<< Arena_Season_Fields::SEASON_BEGIN << begin_time
			<< Arena_Season_Fields::SEASON_FINISH << finish_time
			<<Arena_Season_Fields::SEASON_RESULT << result_vector;
	MONGO_CONNECTION.update(Arena_Season_Fields::COLLECTION, Query(),
			BSON("$set" << set_builder.obj() ), true);
	return 0;
}

int DB_Operator::clear_arena_player_data(void){
	MONGO_CONNECTION.remove(Arenaer_Fields::COLLECTION, Query());
	return 0;
}

int DB_Operator::save_arena_king_time(const role_id_t role_id, const int king_time){
	if(role_id == 0){
		return -1;
	}
	BSONObjBuilder set_builder;
	set_builder << Arenaer_Fields::KING_TIME << (int)king_time;
	MONGO_CONNECTION.update(Arenaer_Fields::COLLECTION, QUERY(Arenaer_Fields::ROLE_ID << (long long int)role_id),
			BSON("$set" << set_builder.obj() ), true);
	return 0;
}

int DB_Operator::save_arena_be_worship(const role_id_t role_id, const int be_worship) {
	if(role_id == 0){
		return -1;
	}
	BSONObjBuilder set_builder;
	set_builder << Arenaer_Fields::BE_WORSHIP << (int)be_worship;
	MONGO_CONNECTION.update(Arenaer_Fields::COLLECTION, QUERY(Arenaer_Fields::ROLE_ID << (long long int)role_id),
		BSON("$set" << set_builder.obj() ), true);
	return 0;
}

int DB_Operator::load_mount_detail(Mount_Detail &detail){
	BSONObj find_it = MONGO_CONNECTION.findOne(Mount_Fields::COLLECTION, QUERY(Mount_Fields::ROLE_ID << (long long int)detail.role_id));
	if ( find_it.isEmpty() ) {
		return 0;
	}
	detail.role_id = find_it[Mount_Fields::ROLE_ID].numberLong();
	detail.mount_id = find_it[Mount_Fields::MOUNT_ID].numberInt();
	detail.ride = find_it[Mount_Fields::RIDE].numberInt();
	if (find_it.hasField(Mount_Fields::HORSE_LIST.c_str())) {
		BSONObjIterator info_iter(find_it.getObjectField(Mount_Fields::HORSE_LIST.c_str()));
		while (info_iter.more()){
			Horse_Info info;
			BSONElement info_obj = info_iter.next();
			info.horse_id = info_obj[Mount_Fields::Horse_Info::HORSE_ID].numberInt();
			info.invalid = info_obj[Mount_Fields::Horse_Info::INVALID].numberInt();
			info.deadline.sec(info_obj[Mount_Fields::Horse_Info::DEADLINE].numberInt());
			info.activate_time.sec(info_obj[Mount_Fields::Horse_Info::ACTIVATE_TIME].numberInt());
			info.train_lv = info_obj[Mount_Fields::Horse_Info::TRAIN_LV].numberInt();
			info.train_num = info_obj[Mount_Fields::Horse_Info::TRAIN_NUM].numberInt();
			info.train_fail_num = info_obj[Mount_Fields::Horse_Info::TRAIN_FAIL_NUM].numberInt();
			info.deploy = info_obj[Mount_Fields::Horse_Info::DEPLOY].numberInt();
			info.deploy_skill_1 = info_obj[Mount_Fields::Horse_Info::DEPLOY_SKILL_1].numberInt();
			info.deploy_skill_2 = info_obj[Mount_Fields::Horse_Info::DEPLOY_SKILL_2].numberInt();
			info.deploy_skill_3 = info_obj[Mount_Fields::Horse_Info::DEPLOY_SKILL_3].numberInt();
			detail.horse_list.push_back(info);
		}
    }
	if (find_it.hasField(Mount_Fields::GROUP_LIST.c_str())) {
		BSONObjIterator info_iter(find_it.getObjectField(Mount_Fields::GROUP_LIST.c_str()));
		while (info_iter.more()){
			Horse_Group_Info info;
			BSONElement info_obj = info_iter.next();
			info.group_id = info_obj[Mount_Fields::Group_Info::GROUP_ID].numberInt();
			info.status = info_obj[Mount_Fields::Group_Info::STATUS].numberInt();
			detail.group_list.push_back(info);
		}
    }
	return 0;
}
int DB_Operator::save_mount_detail(const Mount_Detail &detail){
	std::vector<BSONObj> horse_vector;
	std::vector<Horse_Info>::const_iterator it;
	int sum_train_lv = 0;
	for (it = detail.horse_list.begin(); it != detail.horse_list.end(); it++) {
		horse_vector.push_back(BSON(
				      Mount_Fields::Horse_Info::HORSE_ID << (int)it->horse_id
				   << Mount_Fields::Horse_Info::INVALID << (int)it->invalid
					<< Mount_Fields::Horse_Info::DEADLINE << (int)it->deadline.sec()
					<< Mount_Fields::Horse_Info::ACTIVATE_TIME << (int)it->activate_time.sec()
					<< Mount_Fields::Horse_Info::TRAIN_LV << (int)it->train_lv
					<< Mount_Fields::Horse_Info::TRAIN_NUM << (int)it->train_num
					<< Mount_Fields::Horse_Info::TRAIN_FAIL_NUM << (int)it->train_fail_num
					<< Mount_Fields::Horse_Info::DEPLOY << (int)it->deploy
					<< Mount_Fields::Horse_Info::DEPLOY_SKILL_1 << (int)it->deploy_skill_1
					<< Mount_Fields::Horse_Info::DEPLOY_SKILL_2 << (int)it->deploy_skill_2
					<< Mount_Fields::Horse_Info::DEPLOY_SKILL_3 << (int)it->deploy_skill_3
					)
		);
		sum_train_lv += (int)it->train_lv;
	}

	std::vector<BSONObj> group_vector;
	std::vector<Horse_Group_Info>::const_iterator group_it;
	for (group_it = detail.group_list.begin(); group_it != detail.group_list.end(); group_it++) {
		group_vector.push_back(BSON(
				Mount_Fields::Group_Info::GROUP_ID << (int)group_it->group_id
				<<Mount_Fields::Group_Info::STATUS << (int)group_it->status
				)
		);
	}
	BSONObjBuilder set_builder;
	set_builder << Mount_Fields::ROLE_ID << (long long int)detail.role_id
			   << Mount_Fields::MOUNT_ID << (int)detail.mount_id
			   << Mount_Fields::RIDE << (int)detail.ride
			   << Mount_Fields::HORSE_LIST << horse_vector
				<< Mount_Fields::GROUP_LIST << group_vector
				<< Mount_Fields::SUM_TRAIN_LV << sum_train_lv
				;
	MONGO_CONNECTION.update(Mount_Fields::COLLECTION, QUERY(Mount_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << set_builder.obj() ), true);
	return 0;
}


int DB_Operator::update_arenaer_fight(Arenaer_Detail* detail){
	BSONObjBuilder field_builder;
		field_builder 	<< Fighter_Fields::FORCE << 1;
		BSONObj field_bson = field_builder.obj();
	BSONObj find_it = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Arenaer_Fields::ROLE_ID << (long long int)detail->role_id),&field_bson);
	if ( find_it.isEmpty() ) {
		LOG_DEBUG("No role %ld update arenaer_fight info", detail->role_id);
		return 0;
	}
	//detail->fight = find_it[Fighter_Fields::FORCE].numberInt();
	return 0;
}

int DB_Operator::load_all_arena_player(bool reset/* = false*/){
	unsigned long long row_count = MONGO_CONNECTION.count( Arenaer_Fields::COLLECTION );
	BSONObjBuilder field_builder;
	field_builder 	<< Arenaer_Fields::ROLE_ID << 1
					<< Arenaer_Fields::GROUP << 1
					<< Arenaer_Fields::SEG << 1
					<< Arenaer_Fields::SCORE << 1
					//<< Arenaer_Fields::RANK << 1
					<< Arenaer_Fields::KING_TIME << 1
					<< Arenaer_Fields::FIGHT_TIMES << 1
					<< Arenaer_Fields::BE_WORSHIP << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve( row_count );
	MONGO_CONNECTION.findN(iter_record, Arenaer_Fields::COLLECTION, mongo::Query(), row_count, 0, &field_bson);
	int count = 0;
	for(vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj& obj = *iter;

		role_id_t role_id = obj[Arenaer_Fields::ROLE_ID].numberLong();
		Arena_Rank_Player* rank_player = ARENA_MANAGER->get_rank_player(role_id);
		if(rank_player){
			rank_player->player_rank_info()->group	 = obj[Arenaer_Fields::GROUP].numberInt();
			rank_player->player_rank_info()->seg = obj[Arenaer_Fields::SEG].numberInt();
			rank_player->player_rank_info()->score  = obj[Arenaer_Fields::SCORE].numberInt();
			//rank_player->player_rank_info()->rank = obj[Arenaer_Fields::RANK].numberInt();
			rank_player->player_rank_info()->king_time = obj[Arenaer_Fields::KING_TIME].numberInt();
			rank_player->player_rank_info()->fight_times = obj[Arenaer_Fields::FIGHT_TIMES].numberInt();
			rank_player->player_rank_info()->be_worship = obj[Arenaer_Fields::BE_WORSHIP].numberInt();
		}
		++count;
	}
	LOG_DEBUG("load_all_arena_player, size:%d", count);
	return 0;
}

int DB_Operator::init_all_arena_player(void){
	unsigned long long row_count = MONGO_CONNECTION.count( Role_Fields::COLLECTION );
	BSONObjBuilder field_builder;
	field_builder 	<< Role_Fields::ROLE_ID << 1
					<< Role_Fields::ROLE_NAME << 1
					<< Role_Fields::LEVEL << 1
					<< Role_Fields::CAREER << 1
					<< Role_Fields::GENDER << 1;

	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve( row_count );
	MONGO_CONNECTION.findN(iter_record, Arenaer_Fields::COLLECTION, mongo::Query(), row_count, 0, &field_bson);
	int count = 0;
	for(vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
//		BSONObj& obj = *iter;

//		role_id_t role_id = obj[Role_Fields::ROLE_ID].numberLong();;
//		Arena_Rank_Player* rank_player = ARENA_MANAGER->get_rank_player(role_id);
//	   rank_player->role_id() = obj[Arenaer_Fields::ROLE_ID].numberLong();
//		if ( rank_player->role_id() == 0 ) {
//			LOG_ABORT("load_all_arena_player, role_id == 0!!");
//		}
//		rank_player->role_name()	 = obj[Role_Fields::ROLE_NAME].String();
//		rank_player->level() = obj[Role_Fields::LEVEL].numberInt();
//		rank_player->career()  = obj[Role_Fields::CAREER].numberInt();
//		rank_player->gender() = obj[Role_Fields::GENDER].numberInt();
		//rank_player->free_times = 10;
		++count;
	}
	LOG_DEBUG("init_all_arena_player, size:%d", count);
	return 0;
}

int DB_Operator::load_ranking(Ranking_Map& rank_) {
	rank_.clear();
	unsigned long long res_count = MONGO_CONNECTION.count(Ranking_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder 	<< Ranking_Fields::RANKING_TYPE << 1
					<< Ranking_Fields::RANKING << 1
					<< Ranking_Fields::ROLE_INFO << 1
					<< Ranking_Fields::ITEM_INFO << 1
					<< Ranking_Fields::HEROER_INFO << 1
					<< Ranking_Fields::UPDATE_TIME << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, Ranking_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	Ranking r;
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		r.reset();
		int rank_type = obj[Ranking_Fields::RANKING_TYPE].numberInt();
		{
			BSONObjIterator rsit(iter->getObjectField(Ranking_Fields::RANKING.c_str()));
			BSONObj rsobj;
			Ranking_Info rs;
			while (rsit.more()){
				rs.reset();
				rsobj = rsit.next().embeddedObject();

				rs.reset();
				rs.role_name = rsobj[Ranking_Fields::Rank_Info::ROLE_NAME].valuestrsafe();
				rs.role_id = rsobj[Ranking_Fields::Rank_Info::ROLE_ID].numberLong();
				rs.career = rsobj[Ranking_Fields::Rank_Info::CAREER].numberInt();
				rs.server = rsobj[Ranking_Fields::Rank_Info::SERVER].valuestrsafe();
				rs.ext_1 = rsobj[Ranking_Fields::Rank_Info::EXT_1].numberDouble();
				rs.ext_2 = rsobj[Ranking_Fields::Rank_Info::EXT_2].valuestrsafe();
				rs.ext_3 = rsobj[Ranking_Fields::Rank_Info::EXT_3].numberInt();
				rs.ext_4 = rsobj[Ranking_Fields::Rank_Info::EXT_4].numberInt();
				rs.ext_5 = rsobj[Ranking_Fields::Rank_Info::EXT_5].numberInt();
				rs.ext_6 = rsobj[Ranking_Fields::Rank_Info::EXT_6].numberInt();
				rs.ext_7 = rsobj[Ranking_Fields::Rank_Info::EXT_7].numberInt();
				rs.ext_8 = rsobj[Ranking_Fields::Rank_Info::EXT_8].numberLong();
				r.ranking_.push_back(rs);
			}
		}
		//role_info
		{
			BSONObjIterator rsit(iter->getObjectField(Ranking_Fields::ROLE_INFO.c_str()));
			BSONObj rsobj;
			Rank_Role_Info rri;
			while (rsit.more()){
				rri.reset();
				rsobj = rsit.next().embeddedObject();

				rri.role_id = rsobj[Ranking_Fields::Role_Info::ROLE_ID].numberLong();
				rri.role_name = rsobj[Ranking_Fields::Role_Info::ROLE_NAME].valuestrsafe();
				rri.gang_id = rsobj[Ranking_Fields::Role_Info::GANG_ID].numberLong();
				rri.gang_name = rsobj[Ranking_Fields::Role_Info::GANG_NAME].valuestrsafe();
				rri.gender = rsobj[Ranking_Fields::Role_Info::GENDER].numberInt();
				rri.exploit_lvl = rsobj[Ranking_Fields::Role_Info::EXPLOIT_LVL].numberInt();
				rri.exploit_val = rsobj[Ranking_Fields::Role_Info::EXPLOIT_VAL].numberInt();
				{//AVATAR
					BSONObjIterator iter_set(rsobj.getObjectField(Ranking_Fields::Role_Info::AVATAR.c_str()));
					while (iter_set.more()) {
						rri.avatar.push_back(iter_set.next().numberInt());
					}
				}
				rri.hero_id = rsobj[Ranking_Fields::Role_Info::HERO_ID].numberInt();
				rri.hero_index = rsobj[Ranking_Fields::Role_Info::HERO_INDEX].numberInt();
				//prop_value
				BSONObjIterator prop_rsit(rsobj.getObjectField(Ranking_Fields::Role_Info::PROP_VALUE.c_str()));
				BSONObj prop_rsobj;
				Property pro;
				while(prop_rsit.more()){
					pro.reset();
					prop_rsobj = prop_rsit.next().embeddedObject();

					pro.type = prop_rsobj[Ranking_Fields::Role_Info::Prop_Value::TYPE].numberInt();
					pro.value = prop_rsobj[Ranking_Fields::Role_Info::Prop_Value::VALUE].numberDouble();

					rri.prop_value.push_back(pro);
				}

				//item_info
				BSONObjIterator item_info_rsit(rsobj.getObjectField(Ranking_Fields::ITEM_INFO.c_str()));
				BSONObj item_info_rsobj;
				Item_Basic_Info item_info;
				while(item_info_rsit.more()){
					item_info.reset();
					item_info_rsobj = item_info_rsit.next().embeddedObject();

					load_ranking_item_base(item_info, item_info_rsobj);

					rri.item_info.push_back(item_info);
				}

				//equip_info
				BSONObjIterator equip_info_rsit(rsobj.getObjectField(Ranking_Fields::ITEM_EQUIP_INFO.c_str()));
				BSONObj equip_info_rsobj;
				Item_Equip_Info equip_info;
				while(equip_info_rsit.more()){
					equip_info.reset();
					equip_info_rsobj = equip_info_rsit.next().embeddedObject();

					load_ranking_item_equip(equip_info, equip_info_rsobj);

					rri.equip_info.push_back(equip_info);
				}
				//rune_part_info
				BSONObjIterator rp_info_rsit(rsobj.getObjectField(Ranking_Fields::RUNE_PART_INFO.c_str()));
				BSONObj rp_info_rsobj;
				Rune_Part_Info rpi;
				while(rp_info_rsit.more()){
					rpi.reset();
					rp_info_rsobj = rp_info_rsit.next().embeddedObject();

					load_ranking_rune_part(rpi, rp_info_rsobj);

					rri.rune_part_info.push_back(rpi);
				}
				//rune_stone_info
				BSONObjIterator rs_info_rsit(rsobj.getObjectField(Ranking_Fields::ITEM_RUNE_STONE_INFO.c_str()));
				BSONObj rs_info_rsobj;
				Item_Rune_Stone_Info rsi;
				while(rs_info_rsit.more()){
					rsi.reset();
					rs_info_rsobj = rs_info_rsit.next().embeddedObject();

					load_ranking_rune_stone(rsi, rs_info_rsobj);

					rri.rune_stone_info.push_back(rsi);
				}
				//master_part_info
				BSONObjIterator mp_info_rsit(rsobj.getObjectField(Ranking_Fields::ITEM_MASTER_PART_INFO.c_str()));
				BSONObj mp_info_rsobj;
				Master_Part_Info mpi;
				while(mp_info_rsit.more()){
					mpi.reset();
					mp_info_rsobj = mp_info_rsit.next().embeddedObject();

					load_ranking_master_part(mpi, mp_info_rsobj);

					rri.master_part_info.push_back(mpi);
				}
				/*********************************/
				//hero_prop_value
				BSONObjIterator hero_prop_rsit(rsobj.getObjectField(Ranking_Fields::Role_Info::HERO_PROP_VALUE.c_str()));
				BSONObj hero_prop_rsobj;
				Property hero_pro;
				while(hero_prop_rsit.more()){
					hero_pro.reset();
					hero_prop_rsobj = hero_prop_rsit.next().embeddedObject();

					hero_pro.type = hero_prop_rsobj[Ranking_Fields::Role_Info::Prop_Value::TYPE].numberInt();
					hero_pro.value = hero_prop_rsobj[Ranking_Fields::Role_Info::Prop_Value::VALUE].numberDouble();

					rri.hero_prop_value.push_back(hero_pro);
				}
				//hero_item_info
				BSONObjIterator hero_item_info_rsit(rsobj.getObjectField(Ranking_Fields::Role_Info::HERO_ITEM_INFO.c_str()));
				BSONObj hero_item_info_rsobj;
				Item_Basic_Info hero_item_info;
				while(hero_item_info_rsit.more()){
					hero_item_info.reset();
					hero_item_info_rsobj = hero_item_info_rsit.next().embeddedObject();

					load_ranking_item_base(hero_item_info, hero_item_info_rsobj);

					rri.hero_item_info.push_back(hero_item_info);
				}

				//hero_equip_info
				BSONObjIterator hero_equip_info_rsit(rsobj.getObjectField(Ranking_Fields::Role_Info::HERO_ITEM_EQUIP_INFO.c_str()));
				BSONObj hero_equip_info_rsobj;
				Item_Equip_Info hero_equip_info;
				while(hero_equip_info_rsit.more()){
					hero_equip_info.reset();
					hero_equip_info_rsobj = hero_equip_info_rsit.next().embeddedObject();

					load_ranking_item_equip(hero_equip_info, hero_equip_info_rsobj);

					rri.hero_equip_info.push_back(hero_equip_info);
				}
				//hero_rune_part_info
				BSONObjIterator hero_rp_info_rsit(rsobj.getObjectField(Ranking_Fields::Role_Info::HERO_RUNE_PART_INFO.c_str()));
				BSONObj hero_rp_info_rsobj;
				Rune_Part_Info hero_rpi;
				while(hero_rp_info_rsit.more()){
					hero_rpi.reset();
					hero_rp_info_rsobj = hero_rp_info_rsit.next().embeddedObject();

					load_ranking_rune_part(hero_rpi, hero_rp_info_rsobj);

					rri.hero_rune_part_info.push_back(hero_rpi);
				}
				//hero_rune_stone_info
				BSONObjIterator hero_rs_info_rsit(rsobj.getObjectField(Ranking_Fields::Role_Info::HERO_ITEM_RUNE_STONE_INFO.c_str()));
				BSONObj hero_rs_info_rsobj;
				Item_Rune_Stone_Info hero_rsi;
				while(hero_rs_info_rsit.more()){
					hero_rsi.reset();
					hero_rs_info_rsobj = hero_rs_info_rsit.next().embeddedObject();

					load_ranking_rune_stone(hero_rsi, hero_rs_info_rsobj);

					rri.hero_rune_stone_info.push_back(hero_rsi);
				}

				r.role_info_.push_back(rri);
			}
		}
		//heroer_info
		{
			BSONObjIterator rsit(iter->getObjectField(Ranking_Fields::HEROER_INFO.c_str()));
			BSONObj rsobj;
			Rank_Heroer_Info rri;
			while (rsit.more()){
				rri.reset();
				rsobj = rsit.next().embeddedObject();

				//item_info
				BSONObjIterator item_info_rsit(rsobj.getObjectField(Ranking_Fields::ITEM_INFO.c_str()));
				BSONObj item_info_rsobj;
				Item_Basic_Info item_info;
				while(item_info_rsit.more()){
					item_info.reset();
					item_info_rsobj = item_info_rsit.next().embeddedObject();

					load_ranking_item_base(item_info, item_info_rsobj);

					rri.item_info.push_back(item_info);
				}

				//equip_info
				BSONObjIterator equip_info_rsit(rsobj.getObjectField(Ranking_Fields::ITEM_EQUIP_INFO.c_str()));
				BSONObj equip_info_rsobj;
				Item_Equip_Info equip_info;
				while(equip_info_rsit.more()){
					equip_info.reset();
					equip_info_rsobj = equip_info_rsit.next().embeddedObject();

					load_ranking_item_equip(equip_info, equip_info_rsobj);

					rri.equip_info.push_back(equip_info);
				}
				//rune_part_info
				BSONObjIterator rp_info_rsit(rsobj.getObjectField(Ranking_Fields::RUNE_PART_INFO.c_str()));
				BSONObj rp_info_rsobj;
				Rune_Part_Info rpi;
				while(rp_info_rsit.more()){
					rpi.reset();
					rp_info_rsobj = rp_info_rsit.next().embeddedObject();

					load_ranking_rune_part(rpi, rp_info_rsobj);

					rri.rune_part_info.push_back(rpi);
				}
				//rune_stone_info
				BSONObjIterator rs_info_rsit(rsobj.getObjectField(Ranking_Fields::ITEM_RUNE_STONE_INFO.c_str()));
				BSONObj rs_info_rsobj;
				Item_Rune_Stone_Info rsi;
				while(rs_info_rsit.more()){
					rsi.reset();
					rs_info_rsobj = rs_info_rsit.next().embeddedObject();

					load_ranking_rune_stone(rsi, rs_info_rsobj);

					rri.rune_stone_info.push_back(rsi);
				}
				//master_part_info
				BSONObjIterator mp_info_rsit(rsobj.getObjectField(Ranking_Fields::ITEM_MASTER_PART_INFO.c_str()));
				BSONObj mp_info_rsobj;
				Master_Part_Info mpi;
				while(mp_info_rsit.more()){
					mpi.reset();
					mp_info_rsobj = mp_info_rsit.next().embeddedObject();

					load_ranking_master_part(mpi, mp_info_rsobj);

					rri.master_part_info.push_back(mpi);
				}
				r.heroer_info_.push_back(rri);
			}
		}
		{//item info
			BSONObjIterator rsit(iter->getObjectField(Ranking_Fields::ITEM_INFO.c_str()));
			BSONObj rsobj;
			Rank_Item_All_Info rri;
			while (rsit.more()){
				rri.reset();
				rsobj = rsit.next().embeddedObject();

				//item_info
				BSONObj item_info_rsobj = rsobj.getObjectField(Ranking_Fields::ITEM_INFO.c_str());
				load_ranking_item_base(rri.item_info, item_info_rsobj);

				//equip_info
				BSONObj equip_info_rsobj = rsobj.getObjectField(Ranking_Fields::ITEM_EQUIP_INFO.c_str());
				load_ranking_item_equip(rri.equip_info, equip_info_rsobj);

				//rune_part_info
				BSONObj rp_info_rsobj = rsobj.getObjectField(Ranking_Fields::RUNE_PART_INFO.c_str());
				load_ranking_rune_part(rri.rune_part_info, rp_info_rsobj);

				//master_part_info
				BSONObj mp_info_rsobj = rsobj.getObjectField(Ranking_Fields::ITEM_MASTER_PART_INFO.c_str());
				load_ranking_master_part(rri.master_part_info, mp_info_rsobj);

				r.item_info_.push_back(rri);
			}
		}
		r.tick_sec = CONFIG_CACHE_RANKING->get_time_by_ranking_type(rank_type);
		r.tick_time = Time_Value::gettimeofday() + Time_Value(r.tick_sec);
		r.update_time.sec(obj[Ranking_Fields::UPDATE_TIME].numberLong());
		rank_.insert(std::make_pair(rank_type, r));
	}

	return 0;
}

int DB_Operator::save_ranking(Ranking_Map& rank_) {
	for(Ranking_Map::iterator iter = rank_.begin(); iter != rank_.end(); ++iter) {
			BSONObjBuilder builder;
			{
				std::vector<BSONObj> cr_vector;
				cr_vector.clear();
				for(Ranking_Sort_List::iterator cr_iter = iter->second.ranking_.begin(); cr_iter != iter->second.ranking_.end(); cr_iter++){
					cr_vector.push_back( BSON( Ranking_Fields::Rank_Info::ROLE_NAME << cr_iter->role_name
											<< Ranking_Fields::Rank_Info::ROLE_ID << (long long int)cr_iter->role_id
											<< Ranking_Fields::Rank_Info::CAREER << cr_iter->career
											<< Ranking_Fields::Rank_Info::SERVER << cr_iter->server
											<< Ranking_Fields::Rank_Info::EXT_1 << cr_iter->ext_1
											<< Ranking_Fields::Rank_Info::EXT_2 << cr_iter->ext_2
											<< Ranking_Fields::Rank_Info::EXT_3 << cr_iter->ext_3
											<< Ranking_Fields::Rank_Info::EXT_4 << cr_iter->ext_4
											<< Ranking_Fields::Rank_Info::EXT_5 << cr_iter->ext_5
											<< Ranking_Fields::Rank_Info::EXT_6 << cr_iter->ext_6
											<< Ranking_Fields::Rank_Info::EXT_7 << cr_iter->ext_7
											<< Ranking_Fields::Rank_Info::EXT_8 << (long long int)cr_iter->ext_8
											) );
				}
				builder << Ranking_Fields::RANKING << cr_vector;
			}
			{//role_info
				std::vector<BSONObj> cr_vector;
				cr_vector.clear();
				for(Rank_Role_Info_Vec::iterator cr_iter = iter->second.role_info_.begin(); cr_iter != iter->second.role_info_.end(); cr_iter++){
					//prop_value
					std::vector<BSONObj> prop_vector;
					prop_vector.clear();
					for(std::vector<Property>::iterator prop_iter = cr_iter->prop_value.begin(); prop_iter != cr_iter->prop_value.end(); ++prop_iter) {
						prop_vector.push_back( BSON(  Ranking_Fields::Role_Info::Prop_Value::TYPE << prop_iter->type
													<< Ranking_Fields::Role_Info::Prop_Value::VALUE << prop_iter->value) );
					}

					//item_info
					std::vector<BSONObj> item_info_vector;
					item_info_vector.clear();
					for(std::vector<Item_Basic_Info>::iterator item_info_iter = cr_iter->item_info.begin(); item_info_iter != cr_iter->item_info.end(); ++item_info_iter) {
						mongo::BSONObj item_base_obj;
						save_ranking_item_base((*item_info_iter), item_base_obj);
						item_info_vector.push_back( item_base_obj );
					}
					//equip_info
					std::vector<BSONObj> equip_info_vector;
					equip_info_vector.clear();
					for(std::vector<Item_Equip_Info>::iterator equip_info_iter = cr_iter->equip_info.begin(); equip_info_iter != cr_iter->equip_info.end(); ++equip_info_iter) {
						mongo::BSONObj item_info_obj;
						save_ranking_item_equip((*equip_info_iter), item_info_obj);
						equip_info_vector.push_back( item_info_obj );
					}
					//rune_part_info
					std::vector<BSONObj> rune_part_info;
					rune_part_info.clear();
					for(std::vector<Rune_Part_Info>::iterator rp_info_iter = cr_iter->rune_part_info.begin(); rp_info_iter != cr_iter->rune_part_info.end(); ++rp_info_iter) {
						mongo::BSONObj rune_part_obj;
						save_ranking_rune_part((*rp_info_iter), rune_part_obj);
						rune_part_info.push_back( rune_part_obj );
					}
					//rune_stone_info
					std::vector<BSONObj> rune_stone_info;
					rune_stone_info.clear();
					for(std::vector<Item_Rune_Stone_Info>::iterator rp_info_iter = cr_iter->rune_stone_info.begin(); rp_info_iter != cr_iter->rune_stone_info.end(); ++rp_info_iter) {
						mongo::BSONObj rune_stone_obj;
						save_ranking_rune_stone((*rp_info_iter), rune_stone_obj);
						rune_stone_info.push_back( rune_stone_obj );
					}
					//master_part_info
					std::vector<BSONObj> master_part_info;
					master_part_info.clear();
					for(std::vector<Master_Part_Info>::iterator rp_info_iter = cr_iter->master_part_info.begin(); rp_info_iter != cr_iter->master_part_info.end(); ++rp_info_iter) {
						mongo::BSONObj master_part_obj;
						save_ranking_master_part((*rp_info_iter), master_part_obj);
						master_part_info.push_back( master_part_obj );
					}
					/******************************************/
					//hero_prop_value
					std::vector<BSONObj> hero_prop_vector;
					hero_prop_vector.clear();
					for(std::vector<Property>::iterator prop_iter = cr_iter->hero_prop_value.begin(); prop_iter != cr_iter->hero_prop_value.end(); ++prop_iter) {
						hero_prop_vector.push_back( BSON(  Ranking_Fields::Role_Info::Prop_Value::TYPE << prop_iter->type
													<< Ranking_Fields::Role_Info::Prop_Value::VALUE << prop_iter->value) );
					}
					//hero_item_info
					std::vector<BSONObj> hero_item_info_vector;
					hero_item_info_vector.clear();
					for(std::vector<Item_Basic_Info>::iterator item_info_iter = cr_iter->hero_item_info.begin(); item_info_iter != cr_iter->hero_item_info.end(); ++item_info_iter) {
						mongo::BSONObj item_base_obj;
						save_ranking_item_base((*item_info_iter), item_base_obj);
						hero_item_info_vector.push_back( item_base_obj );
					}
					//hero_equip_info
					std::vector<BSONObj> hero_equip_info_vector;
					hero_equip_info_vector.clear();
					for(std::vector<Item_Equip_Info>::iterator equip_info_iter = cr_iter->hero_equip_info.begin(); equip_info_iter != cr_iter->hero_equip_info.end(); ++equip_info_iter) {
						mongo::BSONObj item_info_obj;
						save_ranking_item_equip((*equip_info_iter), item_info_obj);
						hero_equip_info_vector.push_back( item_info_obj );
					}
					//hero_rune_part_info
					std::vector<BSONObj> hero_rune_part_info;
					hero_rune_part_info.clear();
					for(std::vector<Rune_Part_Info>::iterator rp_info_iter = cr_iter->hero_rune_part_info.begin(); rp_info_iter != cr_iter->hero_rune_part_info.end(); ++rp_info_iter) {
						mongo::BSONObj rune_part_obj;
						save_ranking_rune_part((*rp_info_iter), rune_part_obj);
						hero_rune_part_info.push_back( rune_part_obj );
					}
					//hero_rune_stone_info
					std::vector<BSONObj> hero_rune_stone_info;
					hero_rune_stone_info.clear();
					for(std::vector<Item_Rune_Stone_Info>::iterator rp_info_iter = cr_iter->hero_rune_stone_info.begin(); rp_info_iter != cr_iter->hero_rune_stone_info.end(); ++rp_info_iter) {
						mongo::BSONObj rune_stone_obj;
						save_ranking_rune_stone((*rp_info_iter), rune_stone_obj);
						hero_rune_stone_info.push_back( rune_stone_obj );
					}
					cr_vector.push_back( BSON( Ranking_Fields::Role_Info::ROLE_ID << (long long int)cr_iter->role_id
											<< Ranking_Fields::Role_Info::ROLE_NAME << cr_iter->role_name
											<< Ranking_Fields::Role_Info::GANG_ID << (long long int)cr_iter->gang_id
											<< Ranking_Fields::Role_Info::GANG_NAME << cr_iter->gang_name
											<< Ranking_Fields::Role_Info::HERO_ID << cr_iter->hero_id
											<< Ranking_Fields::Role_Info::HERO_INDEX << cr_iter->hero_index
											<< Ranking_Fields::Role_Info::GENDER << cr_iter->gender
											<< Ranking_Fields::Role_Info::EXPLOIT_LVL << cr_iter->exploit_lvl
											<< Ranking_Fields::Role_Info::EXPLOIT_VAL << cr_iter->exploit_val
											<< Ranking_Fields::Role_Info::AVATAR << cr_iter->avatar
											<< Ranking_Fields::Role_Info::PROP_VALUE << prop_vector
											<< Ranking_Fields::ITEM_INFO << item_info_vector
											<< Ranking_Fields::ITEM_EQUIP_INFO << equip_info_vector
											<< Ranking_Fields::RUNE_PART_INFO << rune_part_info
											<< Ranking_Fields::ITEM_RUNE_STONE_INFO << rune_stone_info
											<< Ranking_Fields::ITEM_MASTER_PART_INFO << master_part_info
											<< Ranking_Fields::Role_Info::HERO_PROP_VALUE << hero_prop_vector
											<< Ranking_Fields::Role_Info::HERO_ITEM_INFO << hero_item_info_vector
											<< Ranking_Fields::Role_Info::HERO_ITEM_EQUIP_INFO << hero_equip_info_vector
											<< Ranking_Fields::Role_Info::HERO_ITEM_RUNE_STONE_INFO << hero_rune_stone_info
											<< Ranking_Fields::Role_Info::HERO_RUNE_PART_INFO << hero_rune_part_info) );
				}
				builder << Ranking_Fields::ROLE_INFO << cr_vector;
			}
			{//heroer_info
				std::vector<BSONObj> cr_vector;
				cr_vector.clear();
				for(Rank_Heroer_Info_Vec::iterator cr_iter = iter->second.heroer_info_.begin(); cr_iter != iter->second.heroer_info_.end(); cr_iter++){
					//item_info
					std::vector<BSONObj> item_info_vector;
					item_info_vector.clear();
					for(std::vector<Item_Basic_Info>::iterator item_info_iter = cr_iter->item_info.begin(); item_info_iter != cr_iter->item_info.end(); ++item_info_iter) {
						mongo::BSONObj item_base_obj;
						save_ranking_item_base((*item_info_iter), item_base_obj);
						item_info_vector.push_back( item_base_obj );
					}
					//equip_info
					std::vector<BSONObj> equip_info_vector;
					equip_info_vector.clear();
					for(std::vector<Item_Equip_Info>::iterator equip_info_iter = cr_iter->equip_info.begin(); equip_info_iter != cr_iter->equip_info.end(); ++equip_info_iter) {
						mongo::BSONObj item_info_obj;
						save_ranking_item_equip((*equip_info_iter), item_info_obj);
						equip_info_vector.push_back( item_info_obj );
					}
					//rune_part_info
					std::vector<BSONObj> rune_part_info;
					rune_part_info.clear();
					for(std::vector<Rune_Part_Info>::iterator rp_info_iter = cr_iter->rune_part_info.begin(); rp_info_iter != cr_iter->rune_part_info.end(); ++rp_info_iter) {
						mongo::BSONObj rune_part_obj;
						save_ranking_rune_part((*rp_info_iter), rune_part_obj);
						rune_part_info.push_back( rune_part_obj );
					}
					//rune_part_info
					std::vector<BSONObj> rune_stone_info;
					rune_stone_info.clear();
					for(std::vector<Item_Rune_Stone_Info>::iterator rp_info_iter = cr_iter->rune_stone_info.begin(); rp_info_iter != cr_iter->rune_stone_info.end(); ++rp_info_iter) {
						mongo::BSONObj rune_stone_obj;
						save_ranking_rune_stone((*rp_info_iter), rune_stone_obj);
						rune_stone_info.push_back( rune_stone_obj );
					}
					//master_part_info
					std::vector<BSONObj> master_part_info;
					rune_stone_info.clear();
					for(std::vector<Master_Part_Info>::iterator rp_info_iter = cr_iter->master_part_info.begin(); rp_info_iter != cr_iter->master_part_info.end(); ++rp_info_iter) {
						mongo::BSONObj master_part_obj;
						save_ranking_master_part((*rp_info_iter), master_part_obj);
						master_part_info.push_back( master_part_obj );
					}
					cr_vector.push_back( BSON( Ranking_Fields::ITEM_INFO << item_info_vector
											<< Ranking_Fields::ITEM_EQUIP_INFO << equip_info_vector
											<< Ranking_Fields::RUNE_PART_INFO << rune_part_info
											<< Ranking_Fields::ITEM_MASTER_PART_INFO << master_part_info
											<< Ranking_Fields::ITEM_RUNE_STONE_INFO << rune_stone_info) );
				}
				builder << Ranking_Fields::HEROER_INFO << cr_vector;
			}
			{//item info
				std::vector<BSONObj> cr_vector;
				cr_vector.clear();
				for(Rank_Item_All_Info_Vec::iterator cr_iter = iter->second.item_info_.begin(); cr_iter != iter->second.item_info_.end(); cr_iter++){
					mongo::BSONObj item_base_obj;
					save_ranking_item_base(cr_iter->item_info, item_base_obj);
					mongo::BSONObj item_info_obj;
					save_ranking_item_equip(cr_iter->equip_info, item_info_obj);
					mongo::BSONObj rune_part_obj;
					save_ranking_rune_part(cr_iter->rune_part_info, rune_part_obj);
					mongo::BSONObj master_part_obj;
					save_ranking_master_part(cr_iter->master_part_info, master_part_obj);

					cr_vector.push_back( BSON(
							Ranking_Fields::ITEM_INFO << item_base_obj
							<< Ranking_Fields::ITEM_EQUIP_INFO << item_info_obj
							<< Ranking_Fields::RUNE_PART_INFO << rune_part_obj
							<< Ranking_Fields::ITEM_MASTER_PART_INFO << master_part_obj
					));
				}
				builder << Ranking_Fields::ITEM_INFO << cr_vector;
			}

			builder << Ranking_Fields::UPDATE_TIME << (long long int)iter->second.update_time.sec();

			MONGO_CONNECTION.update(Ranking_Fields::COLLECTION, QUERY(Ranking_Fields::RANKING_TYPE << iter->first),
					BSON("$set" << builder.obj() ), true);

	}
	return 0;
}

int DB_Operator::save_ranking_equipment_data(role_id_t role_id, int item_id, int item_index, std::string role_name, std::string item_name, int score, Item_Equip_Info &item_info, Item_Basic_Info &item_base) {

	mongo::BSONObj item_base_obj;
	save_ranking_item_base(item_base, item_base_obj);

	mongo::BSONObj item_info_obj;
	save_ranking_item_equip(item_info, item_info_obj);

	BSONObjBuilder builder;
	builder << Rank_Equiper_Fields::ROLE_ID << (long long int)role_id
			<< Rank_Equiper_Fields::ITEM_ID << item_id
			<< Rank_Equiper_Fields::ITEM_INDEX << item_index
			<< Rank_Equiper_Fields::ROLE_NAME << role_name
			<< Rank_Equiper_Fields::ITEM_NAME << item_name
			<< Rank_Equiper_Fields::ITEM_INFO << item_info_obj
			<< Rank_Equiper_Fields::ITEM_BASE << item_base_obj
			<< Rank_Equiper_Fields::SCORE << score;
	MONGO_CONNECTION.update(Rank_Equiper_Fields::COLLECTION, QUERY(Rank_Equiper_Fields::ROLE_ID << (long long int)role_id << Rank_Equiper_Fields::ITEM_ID << item_id << Rank_Equiper_Fields::ITEM_INDEX << item_index),
						BSON("$set" << builder.obj() ), true);
	return 0;
}

int DB_Operator::del_ranking_equipment_data(role_id_t role_id, int item_id, int item_index) {
	MONGO_CONNECTION.remove(Rank_Equiper_Fields::COLLECTION, QUERY(Rank_Equiper_Fields::ROLE_ID << (long long int)role_id << Rank_Equiper_Fields::ITEM_ID << item_id << Rank_Equiper_Fields::ITEM_INDEX << item_index));
	return 0;
}

int DB_Operator::load_ranking_rune_part(Rune_Part_Info &rune_info, mongo::BSONObj &obj) {
	rune_info.part = obj[Ranking_Fields::Rune_Part_Info::PART].numberInt();
	BSONObjIterator rh_rsit(obj.getObjectField(Ranking_Fields::Rune_Part_Info::RUNE_HOLE_INFO.c_str()));
	BSONObj rh_rsobj;
	Rune_Hole_Info rhi;
	while(rh_rsit.more()){
		rhi.reset();
		rh_rsobj = rh_rsit.next().embeddedObject();

		rhi.index = rh_rsobj[Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::INDEX].numberInt();
		rhi.level = rh_rsobj[Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::LEVEL].numberInt();
		rhi.energy = rh_rsobj[Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::ENERGY].numberInt();

		rune_info.hole_infos.push_back(rhi);
	}
	return 0;
}

int DB_Operator::save_ranking_rune_stone(Item_Rune_Stone_Info &rune_stone_info, mongo::BSONObj &obj) {
	BSONObjBuilder builder;
	builder << Ranking_Fields::Item_Rune_Stone_Info::INDEX << rune_stone_info.index
			<< Ranking_Fields::Item_Rune_Stone_Info::EXP_LVL << rune_stone_info.exp_lvl
			<< Ranking_Fields::Item_Rune_Stone_Info::EXP << rune_stone_info.exp
			<< Ranking_Fields::Item_Rune_Stone_Info::IS_LOCK << rune_stone_info.is_lock;
	obj = builder.obj();
	return 0;
}

int DB_Operator::load_ranking_rune_stone(Item_Rune_Stone_Info &rune_stone_info, mongo::BSONObj &obj) {
	rune_stone_info.index = obj[Ranking_Fields::Item_Rune_Stone_Info::INDEX].numberInt();
	rune_stone_info.exp_lvl = obj[Ranking_Fields::Item_Rune_Stone_Info::EXP_LVL].numberInt();
	rune_stone_info.exp = obj[Ranking_Fields::Item_Rune_Stone_Info::EXP].numberInt();
	rune_stone_info.is_lock = obj[Ranking_Fields::Item_Rune_Stone_Info::IS_LOCK].numberInt();
	return 0;
}

int DB_Operator::save_ranking_master_part(Master_Part_Info &master_part_info, mongo::BSONObj &obj) {
	BSONObjBuilder builder;
	builder << Ranking_Fields::Item_Master_Part_Info::INDEX << master_part_info.index
			<< Ranking_Fields::Item_Master_Part_Info::LEVEL << master_part_info.level
			<< Ranking_Fields::Item_Master_Part_Info::LUCK_VALUE << master_part_info.luck_value
			<< Ranking_Fields::Item_Master_Part_Info::FAIL_ADD_RATE << master_part_info.fail_add_rate;
	obj = builder.obj();
	return 0;
}

int DB_Operator::load_ranking_master_part(Master_Part_Info &master_part_info, mongo::BSONObj &obj) {
	master_part_info.index = obj[Ranking_Fields::Item_Master_Part_Info::INDEX].numberInt();
	master_part_info.level = obj[Ranking_Fields::Item_Master_Part_Info::LEVEL].numberInt();
	master_part_info.luck_value = obj[Ranking_Fields::Item_Master_Part_Info::LUCK_VALUE].numberInt();
	master_part_info.fail_add_rate = obj[Ranking_Fields::Item_Master_Part_Info::FAIL_ADD_RATE].numberInt();
	return 0;
}

int DB_Operator::save_ranking_rune_part(Rune_Part_Info &rune_info, mongo::BSONObj &obj) {
	BSONObjBuilder builder;
	std::vector<BSONObj> rune_hole_info;
	rune_hole_info.clear();
	for(std::vector<Rune_Hole_Info>::iterator rh_iter = rune_info.hole_infos.begin(); rh_iter != rune_info.hole_infos.end(); ++rh_iter) {
		rune_hole_info.push_back( BSON(Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::INDEX << rh_iter->index
							<< Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::LEVEL << rh_iter->level
							<< Ranking_Fields::Rune_Part_Info::Rune_Hole_Info::ENERGY << rh_iter->energy));
	}
	builder << Ranking_Fields::Rune_Part_Info::PART << rune_info.part
										<< Ranking_Fields::Rune_Part_Info::RUNE_HOLE_INFO << rune_hole_info;
	obj = builder.obj();
	return 0;
}

int DB_Operator::load_ranking_item_base(Item_Basic_Info &item_base, mongo::BSONObj &obj) {
	item_base.index = obj[Ranking_Fields::Item_Info::INDEX].numberInt();
	item_base.seq = obj[Ranking_Fields::Item_Info::SEQ].numberInt();
	item_base.id = obj[Ranking_Fields::Item_Info::ID].numberInt();
	item_base.amount = obj[Ranking_Fields::Item_Info::AMOUNT].numberInt();
	item_base.bind = obj[Ranking_Fields::Item_Info::BIND].numberInt();
	item_base.overdue_time = obj[Ranking_Fields::Item_Info::OVERDUE_TIME].numberInt();
	return 0;
}

int DB_Operator::save_ranking_item_base(Item_Basic_Info &item_base, mongo::BSONObj &obj) {
	BSONObjBuilder item_base_builder;
	{//item base info start
		item_base_builder << Ranking_Fields::Item_Info::INDEX << item_base.index
							<< Ranking_Fields::Item_Info::SEQ << item_base.seq
							<< Ranking_Fields::Item_Info::ID << item_base.id
							<< Ranking_Fields::Item_Info::AMOUNT << item_base.amount
							<< Ranking_Fields::Item_Info::BIND << item_base.bind
							<< Ranking_Fields::Item_Info::OVERDUE_TIME << item_base.overdue_time;
	}//item base info end
	obj = item_base_builder.obj();
	return 0;
}

int DB_Operator::load_ranking_item_equip(Item_Equip_Info &item_info, mongo::BSONObj &obj) {
	item_info.index = obj[Ranking_Fields::Item_Equip_Info::INDEX].numberInt();
	item_info.unban_lvl = obj[Ranking_Fields::Item_Equip_Info::UNBAN_LVL].numberInt();
	item_info.force = obj[Ranking_Fields::Item_Equip_Info::FORCE].numberInt();
	item_info.score = obj[Ranking_Fields::Item_Equip_Info::SCORE].numberInt();
	item_info.incr_level = obj[Ranking_Fields::Item_Equip_Info::INCR_LEVEL].numberInt();
	item_info.incr_luck_value = obj[Ranking_Fields::Item_Equip_Info::INCR_LUCK_VALUE].numberInt();
	item_info.incr_fail_add_rate = obj[Ranking_Fields::Item_Equip_Info::INCR_FAIL_ADD_RATE].numberInt();

	BSONObjIterator epg_rsit(obj.getObjectField(Ranking_Fields::Item_Equip_Info::ENCHANT_PROP_GROUPS.c_str()));
	BSONObj epg_rsobj;
	Enchant_Prop_Group_Info epgi;
	while(epg_rsit.more()){
		epgi.reset();
		epg_rsobj = epg_rsit.next().embeddedObject();
		epgi.score = epg_rsobj[Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::SCORE].numberInt();

		BSONObjIterator gi_rsit(epg_rsobj.getObjectField(Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::GROUP_INFO.c_str()));
		BSONObj gi_rsobj;
		Enchant_Prop_Info epi;
		while(gi_rsit.more()){
			epi.reset();
			gi_rsobj = gi_rsit.next().embeddedObject();
			epi.prop_type = gi_rsobj[Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::PROP_TYPE].numberInt();
			epi.prop_val = gi_rsobj[Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::PROP_VAL].numberInt();
			epi.star = gi_rsobj[Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::STAR].numberInt();
			epi.is_lock = gi_rsobj[Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::IS_LOCK].numberInt();
			epgi.group_info.push_back(epi);
		}

		item_info.enchant_prop_groups.push_back(epgi);
	}
	BSONObjIterator card_rsit(obj.getObjectField(Ranking_Fields::Item_Equip_Info::CARD_INFO.c_str()));
	BSONObj card_rsobj;
	Card_Info card;
	while(card_rsit.more()){
		card.reset();
		card_rsobj = card_rsit.next().embeddedObject();

		card.index = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::INDEX].numberInt();
		card.lv = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::LV].numberInt();
		card.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::ID].numberInt();
		card.cur_exp = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::CUR_EXP].numberInt();
		card.poten = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::POTEN].numberInt();
		card.star = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::STAR].numberInt();
		card.part = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::PART].numberInt();
		card.item_id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::ITEM_ID].numberInt();
		card.equip_loc = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::EQUIP_LOC].numberInt();
		card.embed_state = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::EMBED_STATE].numberInt();

		card.main_attr.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR][Ranking_Fields::KEY].numberInt();
		card.main_attr.value = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR][Ranking_Fields::VALUE].numberDouble();

		card.sub_attr.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR][Ranking_Fields::KEY].numberInt();
		card.sub_attr.value = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR][Ranking_Fields::VALUE].numberDouble();

		item_info.card_info_vec.push_back(card);
	}
	return 0;
}

int DB_Operator::save_ranking_item_equip(Item_Equip_Info &item_info, mongo::BSONObj &obj) {
	BSONObjBuilder item_builder;
	{//item equip info start
		std::vector<BSONObj> enchant_prop_groups;
		enchant_prop_groups.clear();
		for(std::vector<Enchant_Prop_Group_Info>::iterator epgi_iter = item_info.enchant_prop_groups.begin(); epgi_iter != item_info.enchant_prop_groups.end(); ++epgi_iter) {
			std::vector<BSONObj> group_info;
			group_info.clear();
			for(std::vector<Enchant_Prop_Info>::iterator epi_iter = epgi_iter->group_info.begin(); epi_iter != epgi_iter->group_info.end(); ++epi_iter) {
				group_info.push_back( BSON( Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::PROP_TYPE << epi_iter->prop_type
						<< Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::PROP_VAL << epi_iter->prop_val
						<< Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::STAR << epi_iter->star
						<< Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::Enchant_Prop_Info::IS_LOCK << epi_iter->is_lock ));
			}
			enchant_prop_groups.push_back( BSON( Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::GROUP_INFO << group_info
					<< Ranking_Fields::Item_Equip_Info::Enchant_Prop_Groups_Info::SCORE << epgi_iter->score ));
		}
		std::vector<BSONObj> car_info_vec;
		car_info_vec.clear();
		for(std::vector<Card_Info>::iterator citer = item_info.card_info_vec.begin(); citer != item_info.card_info_vec.end(); ++citer) {
			car_info_vec.push_back( BSON(
					Ranking_Fields::Item_Equip_Info::Card_Info::INDEX << citer->index
					<< Ranking_Fields::Item_Equip_Info::Card_Info::LV << citer->lv
					<< Ranking_Fields::Item_Equip_Info::Card_Info::CUR_EXP << citer->cur_exp
					<< Ranking_Fields::Item_Equip_Info::Card_Info::ID << citer->id
					<< Ranking_Fields::Item_Equip_Info::Card_Info::POTEN << citer->poten
					<< Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR << BSON(Ranking_Fields::KEY << citer->main_attr.id << Ranking_Fields::VALUE << citer->main_attr.value)
					<< Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR << BSON(Ranking_Fields::KEY << citer->sub_attr.id << Ranking_Fields::VALUE << citer->sub_attr.value)
					<< Ranking_Fields::Item_Equip_Info::Card_Info::STAR << citer->star
					<< Ranking_Fields::Item_Equip_Info::Card_Info::PART << citer->part
					<< Ranking_Fields::Item_Equip_Info::Card_Info::ITEM_ID << citer->item_id
					<< Ranking_Fields::Item_Equip_Info::Card_Info::EQUIP_LOC << citer->equip_loc
					<< Ranking_Fields::Item_Equip_Info::Card_Info::EMBED_STATE << citer->embed_state
			 ));
		}
		item_builder <<  Ranking_Fields::Item_Equip_Info::INDEX << item_info.index
					<< Ranking_Fields::Item_Equip_Info::UNBAN_LVL << item_info.unban_lvl
					<< Ranking_Fields::Item_Equip_Info::FORCE << item_info.force
					<< Ranking_Fields::Item_Equip_Info::SCORE << item_info.score
					<< Ranking_Fields::Item_Equip_Info::INCR_LEVEL << item_info.incr_level
					<< Ranking_Fields::Item_Equip_Info::INCR_LUCK_VALUE << item_info.incr_luck_value
					<< Ranking_Fields::Item_Equip_Info::INCR_FAIL_ADD_RATE << item_info.incr_fail_add_rate
					<< Ranking_Fields::Item_Equip_Info::CARD_INFO << car_info_vec
					<< Ranking_Fields::Item_Equip_Info::ENCHANT_PROP_GROUPS << enchant_prop_groups;
	}//item equip info end
	obj = item_builder.obj();
	return 0;
}

int DB_Operator::get_rank_item_all_info(Rank_Item_All_Info &item_info, const mongo::BSONObj &obj, role_id_t role_id) {
	BSONObj item_base =  obj.getObjectField(Rank_Equiper_Fields::ITEM_BASE.c_str());
	load_ranking_item_base(item_info.item_info, item_base);
	BSONObj item_equip =  obj.getObjectField(Rank_Equiper_Fields::ITEM_INFO.c_str());
	load_ranking_item_equip(item_info.equip_info, item_equip);
	{//符文信息
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_info.item_info.id);
		if (item_config) {
			{
				Rune_Detail de;
				de.reset();
				de.role_id = role_id;
				load_rune_detail(de);
				de.get_rune_part_info(item_config->equip_cfg.part + PACK_T_EQUIP_INDEX, item_info.rune_part_info);
			}
			{//精通信息
				Equiper_Detail de;
				de.reset();
				de.role_id = role_id;
				load_equiper_detail(de);
				int part = item_config->equip_cfg.part + PACK_T_EQUIP_INDEX;
				Master_Part_Info_Map::iterator it_part = de.master_part_info.find(part);
				item_info.master_part_info.reset();
				if(it_part != de.master_part_info.end()) {
					item_info.master_part_info.index = it_part->second.index;
					item_info.master_part_info.level = it_part->second.level;
					item_info.master_part_info.luck_value = it_part->second.luck_value;
					item_info.master_part_info.fail_add_rate = it_part->second.fail_cnt;
				}

			}//end 精通信息
		}// end if item config
	}
	return 0;
}

int DB_Operator::get_rank_heroer_info(Rank_Heroer_Info &heroer_info, role_id_t role_id, int32_t heroer_id, int32_t heroer_index) {
	{//装备信息
		BSONObj res = MONGO_CONNECTION.findOne(Package_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
		if (! res.isEmpty() && res.hasField(Package_Fields::PACKAGE.c_str())) {
			BSONObj package = res.getObjectField(Package_Fields::PACKAGE.c_str());
			BSONObjIterator iter(package.getObjectField(Package_Fields::ITEM.c_str()));
			BSONObj obj;
			while (iter.more()){
				obj = iter.next().embeddedObject();
				Item_Detail item;
				if (load_item_detail(obj, item) == 0) {
					{//装备
						uint32_t hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + heroer_index * ONE_HERO_EQUIP_GAP;
						uint32_t start = hero_pack_idx_gap + EQUIP_BEGIN;
						uint32_t end = hero_pack_idx_gap + EQUIP_END;
						if(item.index_ >= start && item.index_ <= end){
							Item_Basic_Info ibi;
							item.set_item_basic_info(ibi);
							heroer_info.item_info.push_back(ibi);

							Item_Equip_Info iei;
							item.addon.equip.make_item_equip_info(iei);
							iei.index = ibi.index;

							BSONObjIterator card_rsit(obj.getObjectField(Ranking_Fields::Item_Equip_Info::CARD_INFO.c_str()));
							BSONObj card_rsobj;
							Card_Info card;
							while(card_rsit.more()){
								card.reset();
								card_rsobj = card_rsit.next().embeddedObject();

								card.index = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::INDEX].numberInt();
								card.lv = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::LV].numberInt();
								card.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::ID].numberInt();
								card.cur_exp = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::CUR_EXP].numberInt();
								card.poten = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::POTEN].numberInt();
								card.star = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::STAR].numberInt();
								card.part = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::PART].numberInt();
								card.item_id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::ITEM_ID].numberInt();
								card.equip_loc = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::EQUIP_LOC].numberInt();
								card.embed_state = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::EMBED_STATE].numberInt();

								card.main_attr.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR][Ranking_Fields::KEY].numberInt();
								card.main_attr.value = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR][Ranking_Fields::VALUE].numberDouble();

								card.sub_attr.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR][Ranking_Fields::KEY].numberInt();
								card.sub_attr.value = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR][Ranking_Fields::VALUE].numberDouble();

								iei.card_info_vec.push_back(card);
							}


							heroer_info.equip_info.push_back(iei);
						}
					}
					{//符石
						uint32_t hero_pack_idx_gap = PACK_T_HERO_RUNE_STONE_EQUIP_INDEX + heroer_index * ONE_HERO_EQUIP_GAP;
						uint32_t start = hero_pack_idx_gap + RS_INNER_IDX_BEGIN;
						uint32_t end = hero_pack_idx_gap + RS_INNER_IDX_END;
						if(item.index_ >= start && item.index_ <= end){
							Item_Basic_Info ibi;
							item.set_item_basic_info(ibi);
							heroer_info.item_info.push_back(ibi);

							Item_Rune_Stone_Info iei;
							item.addon.rune_stone.make_item_rune_stone_info(iei);
							iei.index = ibi.index;
							heroer_info.rune_stone_info.push_back(iei);
						}
					}
				}
			}//end while
		}//end if (! res.isEmpty() && res.hasField(Package_Fields::PACKAGE.c_str())) {
	}
	{//符文信息
		Rune_Detail de;
		de.reset();
		de.role_id = role_id;
		load_rune_detail(de);
		Rune_Part_Info rpi;
		uint32_t hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + 1 * ONE_HERO_EQUIP_GAP;
		for (Rune_Part_Detail_Map::iterator it_part = de.part_detail_map.begin(); it_part != de.part_detail_map.end(); ++it_part) {
			if((uint32_t)it_part->first >= hero_pack_idx_gap + EQUIP_BEGIN && (uint32_t)it_part->first < hero_pack_idx_gap + EQUIP_END) {
				rpi.reset();
				de.get_rune_part_info(it_part->first,rpi);
				heroer_info.rune_part_info.push_back(rpi);
			}
		}
	}
	{//精通信息
		Equiper_Detail de;
		de.reset();
		de.role_id = role_id;
		load_equiper_detail(de);
		uint32_t hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + heroer_index * ONE_HERO_EQUIP_GAP;
		for (Master_Part_Info_Map::iterator it_part = de.master_part_info.begin(); it_part != de.master_part_info.end(); ++it_part) {
			if((uint32_t)it_part->first >= hero_pack_idx_gap + EQUIP_BEGIN && (uint32_t)it_part->first < hero_pack_idx_gap + EQUIP_END) {
				Master_Part_Info info;
				info.reset();
				info.index = it_part->second.index;
				info.level = it_part->second.level;
				info.luck_value = it_part->second.luck_value;
				info.fail_add_rate = it_part->second.fail_cnt;
				heroer_info.master_part_info.push_back(info);
			}
		}
	}
	{
		BSONObj res = MONGO_CONNECTION.findOne(Heroer_Fields::COLLECTION, QUERY(Heroer_Fields::ROLE_ID << (long long int)role_id));
		if (! res.isEmpty()) {
			BSONObjIterator iter(res.getObjectField(Heroer_Fields::HERO_INFOS.c_str()));
			while (iter.more()) {
				BSONObj obj = iter.next().embeddedObject();
				if (obj.hasField(Fighter_Fields::ARENA_FIGHT.c_str()) && obj[Heroer_Fields::Hero_Info::HERO_ID].numberInt() == heroer_id) {
					heroer_info.prop_value.push_back(Property(PT_BLOOD_MAX, (double)obj[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_BLOOD_MAX].numberDouble()));
					heroer_info.prop_value.push_back(Property(PT_PHY_ATTACK, (double)obj[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_PHY_ATTACK].numberDouble()));
					heroer_info.prop_value.push_back(Property(PT_PHY_DEFENSE, (double)obj[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE].numberDouble()));
					heroer_info.prop_value.push_back(Property(PT_SPELL_DEFENSE, (double)obj[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE].numberDouble()));
					heroer_info.prop_value.push_back(Property(PT_LEVEL, (double)obj[Heroer_Fields::Hero_Info::LEVEL].numberInt()));
					heroer_info.prop_value.push_back(Property(PT_FORCE, (double)obj[Heroer_Fields::Hero_Info::COMBAT_POWER].numberInt()));
					break;
				}
			}
		}

//		BSONObj res = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Fighter_Fields::ROLE_ID << (long long int)role_id));
//		if (! res.isEmpty()) {
//			if (res.hasField(Fighter_Fields::ARENA_FIGHT.c_str())) {
//				role_info.prop_value.push_back(Property(PT_BLOOD_MAX, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_BLOOD_MAX].numberDouble()));
//				role_info.prop_value.push_back(Property(PT_PHY_ATTACK, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_PHY_ATTACK].numberDouble()));
//				role_info.prop_value.push_back(Property(PT_PHY_DEFENSE, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE].numberDouble()));
//				role_info.prop_value.push_back(Property(PT_SPELL_DEFENSE, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE].numberDouble()));
//			}
//		}
	}
	return 0;
}

int DB_Operator::get_rank_role_info(Rank_Role_Info &role_info, role_id_t role_id) {
	{
		BSONObj role_res = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
		if (! role_res.isEmpty()) {
			role_info.role_id = role_id;
			role_info.role_name = role_res[Role_Fields::ROLE_NAME].valuestrsafe();
			role_info.gender = role_res[Role_Fields::GENDER].numberInt();
			role_info.title_id = role_res[Role_Fields::TITLE_ID].numberInt();
			role_info.gang_id = role_res[Role_Fields::GANG_ID].numberInt();
			role_info.gang_name = role_res[Role_Fields::GANG_NAME].valuestrsafe();
			/***/
			role_info.prop_value.push_back(Property(PT_CAREER, (double)role_res[Role_Fields::CAREER].numberInt()));
			role_info.prop_value.push_back(Property(PT_LEVEL, (double)role_res[Role_Fields::LEVEL].numberInt()));
			/***/
			BSONObjIterator iter_set(role_res.getObjectField(Role_Fields::AVATAR.c_str()));
			while (iter_set.more()) {
				role_info.avatar.push_back(iter_set.next().numberInt());
			}
		} else {
			LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
		}
	}
	{//public
		BSONObj role_res = MONGO_CONNECTION.findOne(Public_Fields::COLLECTION, QUERY(Public_Fields::ROLE_ID << (long long int)role_id));
		if (! role_res.isEmpty()) {
			role_info.exploit_lvl = role_res[Public_Fields::EXPLOIT_LVL].numberInt();
			role_info.exploit_val = role_res[Public_Fields::EXPLOIT_VAL].numberInt();
		} else {
			LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
		}
	}
	{//符文信息
		Rune_Detail de;
		de.reset();
		de.role_id = role_id;
		load_rune_detail(de);
		Rune_Part_Info rpi;
		for (Rune_Part_Detail_Map::iterator it_part = de.part_detail_map.begin(); it_part != de.part_detail_map.end(); ++it_part) {
			if(it_part->first >= PACK_T_EQUIP_INDEX + EQUIP_BEGIN && it_part->first < PACK_T_EQUIP_INDEX + EQUIP_END) {
				rpi.reset();
				de.get_rune_part_info(it_part->first,rpi);
				role_info.rune_part_info.push_back(rpi);
			}
		}
	}
	{//精通信息
		Equiper_Detail de;
		de.reset();
		de.role_id = role_id;
		load_equiper_detail(de);
		for (Master_Part_Info_Map::iterator it_part = de.master_part_info.begin(); it_part != de.master_part_info.end(); ++it_part) {
			if(it_part->first >= PACK_T_EQUIP_INDEX + EQUIP_BEGIN && it_part->first < PACK_T_EQUIP_INDEX + EQUIP_END) {
				Master_Part_Info info;
				info.reset();
				info.index = it_part->second.index;
				info.level = it_part->second.level;
				info.luck_value = it_part->second.luck_value;
				info.fail_add_rate = it_part->second.fail_cnt;
				role_info.master_part_info.push_back(info);
			}
		}
	}
	{
		BSONObj res = MONGO_CONNECTION.findOne(Package_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
		if (! res.isEmpty() && res.hasField(Package_Fields::PACKAGE.c_str())) {
			BSONObj package = res.getObjectField(Package_Fields::PACKAGE.c_str());
			BSONObjIterator iter(package.getObjectField(Package_Fields::ITEM.c_str()));
			BSONObj obj;
			while (iter.more()){
				obj = iter.next().embeddedObject();
				Item_Detail item;
				if (load_item_detail(obj, item) == 0) {
					{//装备
						if(item.index_ >= Pack::PACK_T_EQUIP_INDEX && item.index_ < Pack::PACK_T_PACKAGE_INDEX){
							Item_Basic_Info ibi;
							item.set_item_basic_info(ibi);
							role_info.item_info.push_back(ibi);

							Item_Equip_Info iei;
							item.addon.equip.make_item_equip_info(iei);
							iei.index = ibi.index;

							BSONObjIterator card_rsit(obj.getObjectField(Ranking_Fields::Item_Equip_Info::CARD_INFO.c_str()));
							BSONObj card_rsobj;
							Card_Info card;
							while(card_rsit.more()){
								card.reset();
								card_rsobj = card_rsit.next().embeddedObject();

								card.index = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::INDEX].numberInt();
								card.lv = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::LV].numberInt();
								card.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::ID].numberInt();
								card.cur_exp = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::CUR_EXP].numberInt();
								card.poten = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::POTEN].numberInt();
								card.star = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::STAR].numberInt();
								card.part = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::PART].numberInt();
								card.item_id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::ITEM_ID].numberInt();
								card.equip_loc = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::EQUIP_LOC].numberInt();
								card.embed_state = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::EMBED_STATE].numberInt();

								card.main_attr.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR][Ranking_Fields::KEY].numberInt();
								card.main_attr.value = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR][Ranking_Fields::VALUE].numberDouble();

								card.sub_attr.id = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR][Ranking_Fields::KEY].numberInt();
								card.sub_attr.value = card_rsobj[Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR][Ranking_Fields::VALUE].numberDouble();

								iei.card_info_vec.push_back(card);
							}

							role_info.equip_info.push_back(iei);
						}
					}
					{//符石头
						if(item.index_ >= Pack::PACK_T_RUNE_STONE_EQUIP_INDEX && item.index_ < Pack::PACK_T_RUNE_STONE_BOX_INDEX){
							Item_Basic_Info ibi;
							item.set_item_basic_info(ibi);
							role_info.item_info.push_back(ibi);

							Item_Rune_Stone_Info iei;
							item.addon.rune_stone.make_item_rune_stone_info(iei);
							iei.index = ibi.index;
							role_info.rune_stone_info.push_back(iei);
						}
					}
				}
			}
		}
	}
//	{//加载英雄信息
//		BSONObj res = MONGO_CONNECTION.findOne(Heroer_Fields::COLLECTION, QUERY(Heroer_Fields::ROLE_ID << (long long int)role_id));
//		if (! res.isEmpty()) {
//			int f_index = res[Heroer_Fields::FIGHTER_INDEX].numberInt();
//			if(f_index) {
//				BSONObjIterator iter(res.getObjectField(Heroer_Fields::HERO_INFOS.c_str()));
//				while (iter.more()) {
//					BSONObj obj = iter.next().embeddedObject();
//					if (obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt() == f_index) {
//						uint32_t hero_id = obj[Heroer_Fields::Hero_Info::HERO_ID].numberInt();
//						Rank_Heroer_Info rhi;
//						rhi.reset();
//						get_rank_heroer_info(rhi, role_id, hero_id, f_index);
//						role_info.hero_index = obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt();
//						role_info.hero_id = hero_id;
//						role_info.hero_equip_info = rhi.equip_info;
//						role_info.hero_item_info = rhi.item_info;
//						role_info.hero_rune_part_info = rhi.rune_part_info;
//						role_info.hero_rune_stone_info = rhi.rune_stone_info;
//						role_info.hero_prop_value = rhi.prop_value;
//						break;
//					}
//				}
//			}
//		}
//	}
	{
		BSONObj res = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Fighter_Fields::ROLE_ID << (long long int)role_id));
		if (! res.isEmpty()) {
			if (res.hasField(Fighter_Fields::ARENA_FIGHT.c_str())) {
				role_info.prop_value.push_back(Property(PT_BLOOD_MAX, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_BLOOD_MAX].numberDouble()));
				role_info.prop_value.push_back(Property(PT_PHY_ATTACK, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_PHY_ATTACK].numberDouble()));
				role_info.prop_value.push_back(Property(PT_PHY_DEFENSE, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE].numberDouble()));
				role_info.prop_value.push_back(Property(PT_SPELL_DEFENSE, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE].numberDouble()));
				role_info.prop_value.push_back(Property(PT_SPELL_ATTACK, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_SPELL_ATTACK].numberDouble()));
				role_info.prop_value.push_back(Property(PT_FORCE, (double)res[Fighter_Fields::TTL_FORCE].numberInt()));
			}
		}
	}
	return 0;
}

int DB_Operator::refresh_ranking(Ranking& rank_, int32_t type, uint32_t num) {
	rank_.reset();
	switch(type) {
		//start 个人排行(个人等级)本服
		case RankingDef::LOCAL_PERSONAL_LEVEL_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Fighter_Fields::ROLE_ID << 1
							<< Fighter_Fields::LEVEL << 1
							<< Fighter_Fields::EXPERIENCE << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Fighter_Fields::COLLECTION, mongo::Query().sort( BSON( Fighter_Fields::LEVEL << -1 << Fighter_Fields::EXPERIENCE << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//角色等级
				ris.ext_3 = obj[Fighter_Fields::LEVEL].numberInt();
				//经验
				ris.ext_1 = obj[Fighter_Fields::EXPERIENCE].numberLong();

				//READ career START
				int64_t role_id = obj[Fighter_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
				} else {
					ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
					ris.career = find_it[Role_Fields::CAREER].numberInt();
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}
				//READ career END
				rank_.ranking_.push_back(ris);
				//start get_role_info
				Rank_Role_Info role_info;
				role_info.reset();
				get_rank_role_info(role_info, role_id);
				rank_.role_info_.push_back(role_info);
				//end get_role_info
			}
			break;
		}
		//end local level ranking
		//start 个人排行(个人战斗力)本服
		case RankingDef::LOCAL_PERSONAL_FORCE_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Fighter_Fields::ROLE_ID << 1
							//<< Fighter_Fields::ROLE_NAME << 1
							<< Fighter_Fields::TTL_FORCE << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Fighter_Fields::COLLECTION, mongo::Query().sort( BSON( Fighter_Fields::TTL_FORCE << -1 << Fighter_Fields::LEVEL << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//战斗力
				ris.ext_4 = obj[Fighter_Fields::TTL_FORCE].numberLong();

				//READ career START
				int64_t role_id = obj[Fighter_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
				} else {
					ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
					ris.career = find_it[Role_Fields::CAREER].numberInt();
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}
				//READ career END
				rank_.ranking_.push_back(ris);
				//start get_role_info
				Rank_Role_Info role_info;
				role_info.reset();
				get_rank_role_info(role_info, role_id);
				rank_.role_info_.push_back(role_info);
				//end get_role_info
			}
			break;
		}
		//end local force ranking
		//start 上古副本 本服
		case RankingDef::LOCAL_DUNGEON_SG_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Sgl_Dungeon_Fields::ROLE_ID << 1
							<< Sgl_Dungeon_Fields::NOR_ELITE_COMPLETE_NUM << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Sgl_Dungeon_Fields::COLLECTION, mongo::Query().sort( BSON( Sgl_Dungeon_Fields::NOR_ELITE_COMPLETE_NUM << -1 /*<< Sgl_Dungeon_Fields::LEVEL << -1*/ ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//战斗力
				ris.ext_4 = obj[Sgl_Dungeon_Fields::NOR_ELITE_COMPLETE_NUM].numberLong();

				//READ career START
				int64_t role_id = obj[Sgl_Dungeon_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
				} else {
					ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
					ris.career = find_it[Role_Fields::CAREER].numberInt();
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}
				//READ career END
				rank_.ranking_.push_back(ris);
				//start get_role_info
				Rank_Role_Info role_info;
				role_info.reset();
				get_rank_role_info(role_info, role_id);
				rank_.role_info_.push_back(role_info);
				//end get_role_info
			}
			break;
		}
		//end上古副本 本服
		//start  魔神殿 本服
		case RankingDef::LOCAL_DUNGEON_SMD_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Sgl_Dungeon_Fields::ROLE_ID << 1
							<< Sgl_Dungeon_Fields::HOR_ELITE_COMPLETE_NUM << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Sgl_Dungeon_Fields::COLLECTION, mongo::Query().sort( BSON( Sgl_Dungeon_Fields::HOR_ELITE_COMPLETE_NUM << -1 /*<< Sgl_Dungeon_Fields::LEVEL << -1*/ ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//战斗力
				ris.ext_4 = obj[Sgl_Dungeon_Fields::HOR_ELITE_COMPLETE_NUM].numberLong();

				//READ career START
				int64_t role_id = obj[Sgl_Dungeon_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
				} else {
					ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
					ris.career = find_it[Role_Fields::CAREER].numberInt();
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}
				//READ career END
				rank_.ranking_.push_back(ris);
				//start get_role_info
				Rank_Role_Info role_info;
				role_info.reset();
				get_rank_role_info(role_info, role_id);
				rank_.role_info_.push_back(role_info);
				//end get_role_info
			}
			break;
		}
		//end 魔神殿 本服
		//start 个人排行(魅力榜)本服
		case RankingDef::LOCAL_PERSONAL_CHARM_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Role_Fields::ROLE_ID << 1
							<< Role_Fields::ROLE_NAME << 1
							<< Role_Fields::CAREER << 1
							<< Role_Fields::GENDER << 1
							<< Role_Fields::CHARM << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Role_Fields::COLLECTION, mongo::Query( BSON(
					Role_Fields::CHARM << BSON("$gt" << 0) ) ).sort( BSON( Role_Fields::CHARM << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;
				ris.reset();

				int64_t role_id = obj[Role_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				ris.ext_4 = obj[Role_Fields::CHARM].numberLong();//魅力值
				ris.role_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
				ris.career = obj[Role_Fields::CAREER].numberInt();
				ris.ext_3 = obj[Role_Fields::GENDER].numberInt();//性别

				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)ris.role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", ris.role_id);
				} else {
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}

				rank_.ranking_.push_back(ris);
				//start get_role_info
				Rank_Role_Info role_info;
				role_info.reset();
				get_rank_role_info(role_info, role_id);
				rank_.role_info_.push_back(role_info);
				//end get_role_info
			}
			break;
		}
		//end 个人排行(魅力榜)本服
		//start 个人排行(武将排行)本服
		case RankingDef::LOCAL_PERSONAL_HEROER_RANKING: {
			unsigned long long res_count = MONGO_CONNECTION.count(Heroer_Fields::COLLECTION);
			BSONObjBuilder field_builder;
			field_builder 	<< Heroer_Fields::ROLE_ID << 1
							<< Heroer_Fields::HERO_INFOS << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			rank_.ranking_.clear();
			Ranking_Info ris;
			MONGO_CONNECTION.findN(iter_record, Heroer_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				int64_t role_id = obj[Fighter_Fields::ROLE_ID].numberLong();
				BSONObjIterator iter(obj.getObjectField(Heroer_Fields::HERO_INFOS.c_str()));
				while (iter.more()) {
					ris.reset();
					BSONObj obj = iter.next().embeddedObject();
					ris.role_id = role_id;
					ris.ext_3=obj[Heroer_Fields::Hero_Info::LEVEL].numberInt();
					ris.ext_4=obj[Heroer_Fields::Hero_Info::COMBAT_POWER].numberInt();
					ris.ext_2=obj[Heroer_Fields::Hero_Info::HERO_NAME].valuestrsafe();
					ris.ext_6=obj[Heroer_Fields::Hero_Info::HERO_ID].numberInt();
					ris.ext_7=obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt();
					std::list<Ranking_Info>::reverse_iterator rit = rank_.ranking_.rbegin();
					if(rit != rank_.ranking_.rend()) {
						for(int i=0; rit != rank_.ranking_.rend(); ++rit) {
							if((rit->ext_4 > ris.ext_4) || (rit->ext_4 == ris.ext_4 && rit->ext_3 > ris.ext_3)) {
								if(!(i == 0 && rank_.ranking_.size() >= num)) {
									BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
									if (find_it.isEmpty() ) {
										LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
									} else {
										ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
									}
									if(i == 0) {
										rank_.ranking_.push_back(ris);
									} else {
										rank_.ranking_.insert(rit.base(),ris);
									}
								}
								break;
							}
							++i;
						}
						if(rit == rank_.ranking_.rend()){
							BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
							if (find_it.isEmpty() ) {
								LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
							} else {
								ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
							}
							rank_.ranking_.insert(rit.base(),ris);
						}
					} else {
						BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
						if (find_it.isEmpty() ) {
								LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
						} else {
							ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
							if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
								ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
							}
						}
						rank_.ranking_.push_back(ris);
					}
					while(rank_.ranking_.size() > num) {
						rank_.ranking_.erase(--rank_.ranking_.end());
					}
				}
			}
			Rank_Heroer_Info heroer_info;
			for(Ranking_Sort_List::iterator it = rank_.ranking_.begin(); it != rank_.ranking_.end(); ++it) {
				//得到英雄信息
				heroer_info.reset();
				get_rank_heroer_info(heroer_info, it->role_id, it->ext_6, it->ext_7);
				rank_.heroer_info_.push_back(heroer_info);
			}
			break;
		}
		//end local heroer ranking
		//start 财富排行(个人财富榜)本服
		case RankingDef::LOCAL_ASSET_SIGLE_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Role_Fields::ROLE_ID << 1
							<< Package_Fields::PACKAGE_MONEY_COPPER << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Package_Fields::COLLECTION, mongo::Query().sort( BSON( Package_Fields::PACKAGE_MONEY_COPPER << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//金币
				ris.ext_4 = ((obj[Package_Fields::PACKAGE].embeddedObject())[Package_Fields::MONEY].embeddedObject())[Package_Fields::Money::COPPER].numberInt();

				//READ career START
				int64_t role_id = obj[Fighter_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
				} else {
					ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
					ris.career = find_it[Role_Fields::CAREER].numberInt();
					//角色等级
					ris.ext_3 = find_it[Role_Fields::LEVEL].numberInt();
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}
				//READ career END
				rank_.ranking_.push_back(ris);
			}
			break;
		}
		//end loval asset sigle ranking
		case RankingDef::LOCAL_PERSONAL_FRIEND_VALUE_RANKING: {//友情值
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Role_Fields::ROLE_ID << 1
							<< Package_Fields::Money::FRIENDSHIP << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Public_Fields::COLLECTION, mongo::Query().sort( BSON( Package_Fields::Money::FRIENDSHIP << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//友情值
				ris.ext_4 = obj[Package_Fields::Money::FRIENDSHIP].numberInt();

				//READ career START
				int64_t role_id = obj[Fighter_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
				} else {
					ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
					ris.career = find_it[Role_Fields::CAREER].numberInt();
					ris.ext_2 = find_it[Role_Fields::GANG_NAME].valuestrsafe();
					//角色等级
					ris.ext_3 = find_it[Role_Fields::LEVEL].numberInt();
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}
				//READ career END
				rank_.ranking_.push_back(ris);
				//start get_role_info
				Rank_Role_Info role_info;
				role_info.reset();
				get_rank_role_info(role_info, role_id);
				rank_.role_info_.push_back(role_info);
				//end get_role_info
			}
			break;
		}
		case RankingDef::LOCAL_PERSONAL_MOUNT_RANKING: {//坐骑排行
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Role_Fields::ROLE_ID << 1
							<< Mount_Fields::SUM_TRAIN_LV << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Mount_Fields::COLLECTION, mongo::Query().sort( BSON( Mount_Fields::SUM_TRAIN_LV << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//坐骑总星数
				ris.ext_6 = obj[Mount_Fields::SUM_TRAIN_LV].numberInt();

				//READ career START
				int64_t role_id = obj[Fighter_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				{
					BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
					if (find_it.isEmpty() ) {
							LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
					} else {
						ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
						ris.career = find_it[Role_Fields::CAREER].numberInt();
						ris.ext_2 = find_it[Role_Fields::GANG_NAME].valuestrsafe();
						//角色等级
						ris.ext_3 = find_it[Role_Fields::LEVEL].numberInt();
						if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
							ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
						}
					}
				}
				//READ career END
				//read 战斗力
				{
					BSONObj find_it = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
					if (find_it.isEmpty() ) {
							LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
					} else {
						//战斗力
						ris.ext_4 = find_it[Fighter_Fields::TTL_FORCE].numberInt();
					}
				}
				//end read
				rank_.ranking_.push_back(ris);
				//start get_role_info
				Rank_Role_Info role_info;
				role_info.reset();
				get_rank_role_info(role_info, role_id);
				rank_.role_info_.push_back(role_info);
				//end get_role_info

				//adv
				for(Ranking_Sort_List::iterator it = rank_.ranking_.begin(); it != rank_.ranking_.end(); ++it) {
					for(Ranking_Sort_List::iterator iter = it; iter != rank_.ranking_.end(); ++iter) {
						if(it->ext_6 != iter->ext_6) break;
						if(it->ext_4 < iter->ext_4) {
							Ranking_Info tmp = *it;
							*it = *iter;
							*iter = tmp;
						}
					}
				}

			}
			break;
		}
		case RankingDef::LOCAL_PERSONAL_WING_RANKING: {//翅膀排行
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Role_Fields::ROLE_ID << 1
							<< Wing_Fields::PROGRESS << 1
							<< Wing_Fields::LV << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Wing_Fields::COLLECTION, mongo::Query(BSON(
					Wing_Fields::LV << BSON("$gt" << 0) ) ).sort( BSON( Wing_Fields::LV << -1 << Wing_Fields::PROGRESS << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//翅膀阶数 翅膀等级
				ris.ext_6 = obj[Wing_Fields::LV].numberInt();
				ris.ext_7 = obj[Wing_Fields::PROGRESS].numberInt();

				//READ career START
				int64_t role_id = obj[Fighter_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				{
					BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
					if (find_it.isEmpty() ) {
							LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
					} else {
						ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
						ris.career = find_it[Role_Fields::CAREER].numberInt();
						ris.ext_2 = find_it[Role_Fields::GANG_NAME].valuestrsafe();
						//角色等级
						ris.ext_3 = find_it[Role_Fields::LEVEL].numberInt();
						if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
							ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
						}
					}
				}
				//READ career END
				//read 战斗力
				{
					BSONObj find_it = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
					if (find_it.isEmpty() ) {
							LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
					} else {
						//战斗力
						ris.ext_4 = find_it[Fighter_Fields::TTL_FORCE].numberInt();
					}
				}
				//end read
				rank_.ranking_.push_back(ris);
				//start get_role_info
				Rank_Role_Info role_info;
				role_info.reset();
				get_rank_role_info(role_info, role_id);
				rank_.role_info_.push_back(role_info);
				//end get_role_info
			}
			break;
		}
		//start 财富排行(公会财富榜)本服
		case RankingDef::LOCAL_ASSET_GANG_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Gang_Fields::NAME << 1
							<< Gang_Fields::FUND << 1
							<< Gang_Fields::LEVEL << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Gang_Fields::COLLECTION, mongo::Query().sort( BSON( Gang_Fields::FUND << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//金币
				ris.ext_4 = obj[Gang_Fields::FUND].numberInt();
				//公会名
				ris.ext_2 = obj[Gang_Fields::NAME].valuestrsafe();
				//公会等级
				ris.ext_3 = obj[Gang_Fields::LEVEL].numberInt();

				rank_.ranking_.push_back(ris);
			}
			break;
		}
		//end local asset gang ranking
		//start LOCAL_ARENA_SIGLE_RANKING
/*		case RankingDef::LOCAL_ARENA_SIGLE_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Arenaer_Fields::ROLE_ID << 1
							<< Arenaer_Fields::SCORE << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Arenaer_Fields::COLLECTION, mongo::Query().sort( BSON( Arenaer_Fields::SCORE << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;
				ris.reset();
				//积分
				ris.ext_3 = obj[Arenaer_Fields::SCORE].numberInt();
				//READ career START
				int64_t role_id = obj[Arenaer_Fields::ROLE_ID].numberLong();
				ris.role_id = role_id;
				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
				if (find_it.isEmpty() ) {
					LOG_DEBUG("No role %ld info in refresh_ranking (local level)", role_id);
				} else {
					ris.role_name = find_it[Role_Fields::ROLE_NAME].valuestrsafe();
					ris.career = find_it[Role_Fields::CAREER].numberInt();
					//角色等级
					ris.ext_3 = find_it[Role_Fields::LEVEL].numberInt();
				}
				//READ career END
				rank_.ranking_.push_back(ris);
			}
			break;
		}*/
		//end LOCAL_ARENA_SIGLE_RANKING
		// start 装备排行(武器排行)本服
		case RankingDef::LOCAL_EQUIPAGE_ARMS_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Rank_Equiper_Fields::ROLE_ID << 1
							<< Rank_Equiper_Fields::ROLE_NAME << 1
							<< Rank_Equiper_Fields::ITEM_NAME << 1
							<< Rank_Equiper_Fields::ITEM_INDEX << 1
							<< Rank_Equiper_Fields::SCORE << 1
							<< Rank_Equiper_Fields::ITEM_BASE << 1
							<< Rank_Equiper_Fields::ITEM_INFO << 1
							<< Rank_Equiper_Fields::ITEM_ID << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Rank_Equiper_Fields::COLLECTION, mongo::Query( BSON(
						Rank_Equiper_Fields::ITEM_ID << BSON("$gte" << 10100000) << Rank_Equiper_Fields::ITEM_ID << BSON("$lte" << 10199999)
					)).sort( BSON( Rank_Equiper_Fields::SCORE << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				ris.role_id = obj[Rank_Equiper_Fields::ROLE_ID].numberLong();
				ris.role_name = obj[Rank_Equiper_Fields::ROLE_NAME].valuestrsafe();
				//装备ID
				ris.ext_4 = obj[Rank_Equiper_Fields::ITEM_ID].numberInt();
				//装备名
				ris.ext_2 = obj[Rank_Equiper_Fields::ITEM_NAME].valuestrsafe();
				//装备评分
				ris.ext_3 = obj[Rank_Equiper_Fields::SCORE].numberInt();
				//装备INDEX
				//ris.ext_6 = obj[Rank_Equiper_Fields::ITEM_INDEX].numberInt();

				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)ris.role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", ris.role_id);
				} else {
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}

				rank_.ranking_.push_back(ris);
				Rank_Item_All_Info item_info;
				item_info.reset();
				get_rank_item_all_info(item_info, obj, ris.role_id);
				rank_.item_info_.push_back(item_info);
			}
			break;
		}
		//end 装备排行(武器排行)本服
		// start 装备排行(饰品排行)本服
		case RankingDef::LOCAL_EQUIPAGE_ACCESSORIES_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Rank_Equiper_Fields::ROLE_ID << 1
							<< Rank_Equiper_Fields::ROLE_NAME << 1
							<< Rank_Equiper_Fields::ITEM_NAME << 1
							<< Rank_Equiper_Fields::ITEM_INDEX << 1
							<< Rank_Equiper_Fields::SCORE << 1
							<< Rank_Equiper_Fields::ITEM_BASE << 1
							<< Rank_Equiper_Fields::ITEM_INFO << 1
							<< Rank_Equiper_Fields::ITEM_ID << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Rank_Equiper_Fields::COLLECTION, mongo::Query( BSON(
						Rank_Equiper_Fields::ITEM_ID << BSON("$gte" << 10500000) << Rank_Equiper_Fields::ITEM_ID << BSON("$lte" << 10699999)
					)).sort( BSON( Rank_Equiper_Fields::SCORE << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				ris.role_id = obj[Rank_Equiper_Fields::ROLE_ID].numberLong();
				ris.role_name = obj[Rank_Equiper_Fields::ROLE_NAME].valuestrsafe();
				//装备ID
				ris.ext_4 = obj[Rank_Equiper_Fields::ITEM_ID].numberInt();
				//装备名
				ris.ext_2 = obj[Rank_Equiper_Fields::ITEM_NAME].valuestrsafe();
				//装备评分
				ris.ext_3 = obj[Rank_Equiper_Fields::SCORE].numberInt();
				//装备INDEX
				//ris.ext_6 = obj[Rank_Equiper_Fields::ITEM_INDEX].numberInt();

				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)ris.role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", ris.role_id);
				} else {
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}

				rank_.ranking_.push_back(ris);
				Rank_Item_All_Info item_info;
				item_info.reset();
				get_rank_item_all_info(item_info, obj, ris.role_id);
				rank_.item_info_.push_back(item_info);
			}
			break;
		}
		//end LOCAL_EQUIPAGE_ACCESSORIES_RANKING
		// start 装备排行(防具排行)本服
		case RankingDef::LOCAL_EQUIPAGE_ARMOR_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Rank_Equiper_Fields::ROLE_ID << 1
							<< Rank_Equiper_Fields::ROLE_NAME << 1
							<< Rank_Equiper_Fields::ITEM_NAME << 1
							<< Rank_Equiper_Fields::ITEM_INDEX << 1
							<< Rank_Equiper_Fields::SCORE << 1
							<< Rank_Equiper_Fields::ITEM_BASE << 1
							<< Rank_Equiper_Fields::ITEM_INFO << 1
							<< Rank_Equiper_Fields::ITEM_ID << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Rank_Equiper_Fields::COLLECTION, mongo::Query( BSON(
						Rank_Equiper_Fields::ITEM_ID << BSON("$gte" << 10200000) << Rank_Equiper_Fields::ITEM_ID << BSON("$lte" << 10499999)
					)).sort( BSON( Rank_Equiper_Fields::SCORE << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				ris.role_id = obj[Rank_Equiper_Fields::ROLE_ID].numberLong();
				ris.role_name = obj[Rank_Equiper_Fields::ROLE_NAME].valuestrsafe();
				//装备ID
				ris.ext_4 = obj[Rank_Equiper_Fields::ITEM_ID].numberInt();
				//装备名
				ris.ext_2 = obj[Rank_Equiper_Fields::ITEM_NAME].valuestrsafe();
				//装备评分
				ris.ext_3 = obj[Rank_Equiper_Fields::SCORE].numberInt();
				//装备INDEX
				//ris.ext_6 = obj[Rank_Equiper_Fields::ITEM_INDEX].numberInt();

				BSONObj find_it = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)ris.role_id));
				if (find_it.isEmpty() ) {
						LOG_DEBUG("No role %ld info in refresh_ranking (local level)", ris.role_id);
				} else {
					if(Time_Value::gettimeofday().sec() < find_it[Role_Fields::VIP_OVER_TIME].numberLong()) {
						ris.ext_5 = find_it[Role_Fields::VIP].numberInt(); //VIP
					}
				}

				rank_.ranking_.push_back(ris);
				Rank_Item_All_Info item_info;
				item_info.reset();
				get_rank_item_all_info(item_info, obj, ris.role_id);
				rank_.item_info_.push_back(item_info);
			}
			break;
		}
		//end LOCAL_EQUIPAGE_ARMOR_RANKING
		//start LOCAL_GANG_RANKING //本地公会排行,按公会排行取于内存
		/*case RankingDef::LOCAL_GANG_RANKING: {
			unsigned long long res_count = num;
			BSONObjBuilder field_builder;
			field_builder 	<< Gang_Fields::NAME << 1
							<< Gang_Fields::FUND << 1
							<< Gang_Fields::MEMBER_SIZE << 1
							<< Gang_Fields::LEVEL << 1;
			BSONObj field_bson = field_builder.obj();
			vector<BSONObj> iter_record;
			iter_record.reserve(res_count);
			MONGO_CONNECTION.findN(iter_record, Gang_Fields::COLLECTION, mongo::Query().sort( BSON( Gang_Fields::LEVEL << -1 << Gang_Fields::MEMBER_SIZE << -1 ) ), res_count, 0, &field_bson);
			Ranking_Info ris;
			for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
				BSONObj &obj = *iter;

				ris.reset();
				//公会人数
				ris.ext_4 = obj[Gang_Fields::MEMBER_SIZE].numberInt();
				//公会名
				ris.ext_2 = obj[Gang_Fields::NAME].valuestrsafe();
				//公会等级
				ris.ext_3 = obj[Gang_Fields::LEVEL].numberInt();

				rank_.ranking_.push_back(ris);
			}
			break;
		}*/
		//end LOCAL_GANG_RANKING
	}
	return 0;
}
