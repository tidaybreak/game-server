/*
 * DB_Operator_A.cpp
 *
 *  Created on: Jul 31, 2012
 *      Author: Ti
 */

#include "DB_Operator.h"
#include "DB_Fields.h"
#include "Configurator.h"
#include "Record_Client.h"
#include "Pool_Manager.h"
#include "Server_Struct.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "mongo/client/dbclient.h"
#include "knight_trials/Knight_Trials_Manager.h"
#include "gang/Gang_Manager.h"
#include "fashion/Fashion_Config.h"
#include "Config_Cache_Fashion.h"

using namespace mongo;

#include "DB_Manager.h"

DB_Operator::DB_Operator(void) : last_cross_inter_(0), inner_conn_(0) { }

DB_Operator::~DB_Operator(void) { }

void DB_Operator::set_inner_conn(DBClientConnection *inner_conn) {
	inner_conn_ = inner_conn;
}

DBClientConnection &DB_Operator::connection(void) {
	GUARD(MUTEX, mon, connection_map_lock_);

	if (inner_conn_) {
		return *inner_conn_;
	}

	const int64_t thread_key = pthread_self();

	DBClientConnection *conn = 0;
	Connection_Map::iterator it = connection_map_.find(thread_key);
	if (it == connection_map_.end()) {
		conn = POOL_MANAGER->pop_dbclientConnection_pool();

		std::string err_msg;
		std::stringstream host_stream;

		const Json::Value &mongodb_server = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_maintainer"]["mongodb_server"];
		if (mongodb_server == Json::Value::null) {
			abort();
		}
		host_stream << (mongodb_server["ip"].asString());
		if (mongodb_server.isMember("port")) {
			host_stream << ":" << (mongodb_server["port"].asInt());
		}

		conn->connect(host_stream.str().c_str(), err_msg);
		connection_map_[thread_key] = conn;
	} else {
		conn = it->second;
	}

    return *conn;
}

void DB_Operator::conn_check(void) {
	std::string err_msg;
	std::stringstream host_stream;

	DBClientConnection *conn = 0;
	conn = POOL_MANAGER->pop_dbclientConnection_pool();
	const Json::Value &mongodb_server = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_maintainer"]["mongodb_server"];
	if (mongodb_server == Json::Value::null) {
		abort();
	}
	host_stream << (mongodb_server["ip"].asString());
	if (mongodb_server.isMember("port")) {
		host_stream << ":" << (mongodb_server["port"].asInt());
	}

	if (!conn->connect(host_stream.str().c_str(), err_msg)) {
		std::cout << "ERROR: start fail, can not connect mongo db, ip:" << mongodb_server["ip"].asString() << " port:" << mongodb_server["port"].asInt() << std::endl;
		exit(0);
	}
}

int DB_Operator::init(void) {

	return 0;
}

int DB_Operator::ensure_all_index(void) {
	{
		MONGO_CONNECTION.ensureIndex(Inner_Check_Info_Fields::COLLECTION, BSON(Inner_Check_Info_Fields::SERVER << 1), false);
		MONGO_CONNECTION.ensureIndex(Inner_Check_Info_Fields::COLLECTION, BSON(Inner_Check_Info_Fields::CORE_NUM << 1), false);
		MONGO_CONNECTION.ensureIndex(Inner_Check_Info_Fields::COLLECTION, BSON(Inner_Check_Info_Fields::UPDATE_TIME << 1), false);
		MONGO_CONNECTION.ensureIndex(Inner_Check_Info_Fields::COLLECTION, BSON(Inner_Check_Info_Fields::MONGO_IP_PORT << 1), false);
	}

	{ 	// role index
		MONGO_CONNECTION.ensureIndex(Role_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
		MONGO_CONNECTION.ensureIndex(Role_Fields::COLLECTION, BSON(Role_Fields::ROLE_NAME << 1), true);
		MONGO_CONNECTION.ensureIndex(Role_Fields::COLLECTION, BSON(Role_Fields::Account_Info::ACCOUNT_NAME << 1), false);
		MONGO_CONNECTION.ensureIndex(Role_Fields::COLLECTION, BSON(Role_Fields::CHARM << 1)); //魅力值 排行榜
		MONGO_CONNECTION.ensureIndex(Role_Fields::COLLECTION, BSON(Role_Fields::ONLINE << 1));
	}
	{ 	// public index
		MONGO_CONNECTION.ensureIndex(Public_Fields::COLLECTION, BSON(Public_Fields::ROLE_ID << 1), true);
	}
	{ 	// example index
		MONGO_CONNECTION.ensureIndex(Example_Fields::COLLECTION, BSON(Example_Fields::ROLE_ID << 1), true);
	}
	{ 	// operating index
		MONGO_CONNECTION.ensureIndex(Operating_Fields::COLLECTION, BSON(Operating_Fields::ROLE_ID << 1), true);
	}
	{ 	// fighter index
		MONGO_CONNECTION.ensureIndex(Fighter_Fields::COLLECTION, BSON(Fighter_Fields::ROLE_ID << 1), true);
		MONGO_CONNECTION.ensureIndex(Fighter_Fields::COLLECTION, BSON(Fighter_Fields::LEVEL << 1), false);
	}
	{
		// mmo.package
		MONGO_CONNECTION.ensureIndex(Package_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.status
		MONGO_CONNECTION.ensureIndex(Status_Fields::COLLECTION, BSON(Status_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.task
		MONGO_CONNECTION.ensureIndex(Task_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
	}
	{	// mmo.heroer
		MONGO_CONNECTION.ensureIndex(Heroer_Fields::COLLECTION, BSON(Heroer_Fields::ROLE_ID << 1), true);
		MONGO_CONNECTION.ensureIndex(Heroer_Fields::STATUS_COLLECTION, BSON(Heroer_Fields::ROLE_ID << 1), true);
	}
	{	// mmo.rune
		MONGO_CONNECTION.ensureIndex(Rune_Fields::COLLECTION, BSON(Rune_Fields::ROLE_ID << 1), true);
	}
	{	// mmo.rune_stone
		MONGO_CONNECTION.ensureIndex(Rune_Stone_Fields::COLLECTION, BSON(Rune_Stone_Fields::ROLE_ID << 1), true);
	}
	{	// mmo.ganger
		MONGO_CONNECTION.ensureIndex(Ganger_Fields::COLLECTION, BSON(Ganger_Fields::ROLE_ID << 1), true);
	}
	{	// mmo.gang
		MONGO_CONNECTION.ensureIndex(Gang_Fields::COLLECTION, BSON(Gang_Fields::GANG_ID << 1), true);
		MONGO_CONNECTION.ensureIndex(Gang_War_Info_Fields::COLLECTION, BSON(Gang_War_Info_Fields::ACTIVITY_TIME << 1), true);
		MONGO_CONNECTION.ensureIndex(Gang_War_Info_Fields::HISTORY_COLLECTION, BSON(Gang_War_Info_Fields::ACTIVITY_TIME << 1), true);
	}
	{	// mmo.friend
		MONGO_CONNECTION.ensureIndex(Friend_Fields::COLLECTION, BSON(Friend_Fields::ROLE_ID << 1), true);
	}
	{	// mmo.alchemy
		MONGO_CONNECTION.ensureIndex(Alchemy_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
	}
//	{	// backstage.pay
//		MONGO_CONNECTION.ensureIndex(Pay_Fields::COLLECTION, BSON(Pay_Fields::ROLE_ID << 1), false);
//	}
	{	// mmo.mail
		MONGO_CONNECTION.ensureIndex(Mail_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1));
	}
	{	// mmo.chat_record
		MONGO_CONNECTION.ensureIndex(Chat_Operating_Fields::PRIVATE_CHAT_COLLECTION, BSON(Chat_Operating_Fields::ROLE_ID << 1
											<< Chat_Operating_Fields::TIME << 1
											<< Chat_Operating_Fields::TARGET << 1), false);
	}

	{
		// mmo.team_role
		MONGO_CONNECTION.ensureIndex(Team_Role_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
		MONGO_CONNECTION.ensureIndex(Team_Sports_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1
				<< Team_Sports_Fields::PROFILE_TIME_SEC << 1), true);
	}

	{
		// mmo.icon
		MONGO_CONNECTION.ensureIndex(Icon_Fields::COLLECTION, BSON(Icon_Fields::ROLE_ID << 1), true);
	}

	{
		// mmo.dragon_vale
		MONGO_CONNECTION.ensureIndex(Dragon_Vale_Fields::COLLECTION, BSON(Dragon_Vale_Fields::ROLE_ID << 1), true);
	}
	/*---------------------------------------------*/

	{
		// mmo.single_dungeon
		MONGO_CONNECTION.ensureIndex(Sgl_Dungeon_Fields::COLLECTION, BSON(Sgl_Dungeon_Fields::ROLE_ID << 1), true);
	}

	{
		// mmo.arena_player
		MONGO_CONNECTION.ensureIndex(Arenaer_Fields::COLLECTION, BSON(Arenaer_Fields::ROLE_ID << 1), true);
	}

	{	// mmo.daily
		MONGO_CONNECTION.ensureIndex(Daily_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
	}
	{	// mmo.gang_chat_record
		MONGO_CONNECTION.ensureIndex(Chat_Operating_Fields::GANG_CHAT_RECORD_COLLECTION, BSON(Chat_Operating_Fields::TARGET << 1), true);
	}
	{	// mmo.rank_equiper
		MONGO_CONNECTION.ensureIndex(Rank_Equiper_Fields::COLLECTION, BSON(Rank_Equiper_Fields::ROLE_ID << 1
											<< Rank_Equiper_Fields::SCORE << 1), false);
	}
	{
		// mmo.ranking
		MONGO_CONNECTION.ensureIndex(Ranking_Fields::COLLECTION, BSON(Ranking_Fields::RANKING_TYPE << 1), true);
	}
	{
		// mmo.covenant
		MONGO_CONNECTION.ensureIndex(Covenant_Fields::COLLECTION, BSON(Covenant_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.achievement
		MONGO_CONNECTION.ensureIndex(Achievement_Fields::COLLECTION, BSON(Achievement_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.title
		MONGO_CONNECTION.ensureIndex(Title_Fields::COLLECTION, BSON(Title_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.equiper
		MONGO_CONNECTION.ensureIndex(Equiper_Fields::COLLECTION, BSON(Equiper_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.title_record
		MONGO_CONNECTION.ensureIndex(Title_Record_Fields::COLLECTION, BSON(Title_Record_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.vip
		MONGO_CONNECTION.ensureIndex(Vip_Fields::COLLECTION, BSON(Vip_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.mount
		MONGO_CONNECTION.ensureIndex(Mount_Fields::COLLECTION, BSON(Mount_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.reward
		MONGO_CONNECTION.ensureIndex(Reward_Fields::COLLECTION, BSON(Reward_Fields::ROLE_ID << 1), true);
		MONGO_CONNECTION.ensureIndex(Activation_Receive_Id_Fields::COLLECTION, BSON(Activation_Receive_Id_Fields::ID << 1), true);
	}
	{
		// mmo.campfire
		MONGO_CONNECTION.ensureIndex(Campfire_Fields::COLLECTION, BSON(Campfire_Fields::GANG_ID << 1), true);
	}
	{
		// mmo.fb_info
		MONGO_CONNECTION.ensureIndex(FB_Info_Fields::COLLECTION, BSON(FB_Info_Fields::ROLE_ID << 1), true);
	}
	{
		// mmo.active_content
		MONGO_CONNECTION.ensureIndex(Active_Content_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
	}
	{
		MONGO_CONNECTION.ensureIndex(Access_Count_Fields::COLLECTION, BSON(Access_Count_Fields::TYPE << 1 << Access_Count_Fields::H_TIME << 1), true);
	}
	{
		MONGO_CONNECTION.ensureIndex(Fashion_Fields::COLLECTION, BSON(Fashion_Fields::ROLE_ID <<1 ), true);
	}
	{//拍卖行
		MONGO_CONNECTION.ensureIndex(Auction_Goods_Fields::COLLECTION, BSON(Auction_Goods_Fields::ID << 1 ), true);
	}
	{//骑士试炼
		MONGO_CONNECTION.ensureIndex(Knight_Trials_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1 ), true);
		MONGO_CONNECTION.ensureIndex(Knight_Trials_Player_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1 ), true);
	}
	{//翅膀
		MONGO_CONNECTION.ensureIndex(Wing_Fields::COLLECTION, BSON(Wing_Fields::ROLE_ID << 1), true);
	}
	{//mmo.anci_artifact
		MONGO_CONNECTION.ensureIndex(Anci_Artifact_Fields::COLLECTION, BSON(Anci_Artifact_Fields::ROLE_ID << 1), true);
	}

	{
		// mmo.invest_plan
		MONGO_CONNECTION.ensureIndex(Invest_Plan_Fields::COLLECTION, BSON(Invest_Plan_Fields::ROLE_ID << 1), true);
	}

	{
		// mmo.thorlottery
		MONGO_CONNECTION.ensureIndex(ThorLottery_Fields::COLLECTION, BSON(ThorLottery_Fields::ROLE_ID << 1), true);
	}

	{ // mmo.exchange
		MONGO_CONNECTION.ensureIndex(Exchange_Detail_Fields::COLLECTION, BSON(ThorLottery_Fields::ROLE_ID << 1), true);
	}

	{ // mmo.activity
		MONGO_CONNECTION.ensureIndex(Activity_Fields::COLLECTION, BSON(Activity_Fields::ROLE_ID << 1), true);
	}

	{ // mmo.expedition
		MONGO_CONNECTION.ensureIndex(Expedition_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
	}

	{ // mmo.activities_stores
		MONGO_CONNECTION.ensureIndex(Activities_Exchange_Stores_Detail_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
		MONGO_CONNECTION.ensureIndex(Activities_Exchange_Stores_Fields::COLLECTION, BSON(Activities_Exchange_Stores_Fields::ID << 1), true);
	}
	{
		// life_skill
		MONGO_CONNECTION.ensureIndex(Life_Skill_Data_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
	}
	{
		// oper_activity
		MONGO_CONNECTION.ensureIndex(Oper_Activity_Data_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1), true);
	}
	{
		MONGO_CONNECTION.ensureIndex(Pet_Fields::COLLECTION, BSON(Pet_Fields::ROLE_ID<<1), true);
	}
	return 0;
}


//int DB_Operator::load_db_view_base_info(Player_DB_View *player) {
////离线基础信息
//	{
//		BSONObj role_res = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)player->role_id));
//		if (! role_res.isEmpty()) {
//		/***/
//		player->prop_value.push_back(Property(PT_CAREER, (double)role_res[Role_Fields::CAREER].numberInt()));
//		player->prop_value.push_back(Property(PT_LEVEL, (double)role_res[Role_Fields::LEVEL].numberInt()));
//		/***/
//		}
//	}
//	{
//		BSONObj res = MONGO_CONNECTION.findOne(Package_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)player->role_id));
//		if (! res.isEmpty() && res.hasField(Package_Fields::PACKAGE.c_str())) {
//			BSONObj package = res.getObjectField(Package_Fields::PACKAGE.c_str());
//			BSONObjIterator iter(package.getObjectField(Package_Fields::ITEM.c_str()));
//			BSONObj obj;
//			while (iter.more()){
//				obj = iter.next().embeddedObject();
//				Item_Detail item;
//				if (load_item_detail(obj, item) == 0) {
//					if(item.index_ >= Pack::PACK_T_EQUIP_INDEX && item.index_ < Pack::PACK_T_PACKAGE_INDEX){
//						Item_Basic_Info ibi;
//						item.set_item_basic_info(ibi);
//						player->item_info.push_back(ibi);
//
//						Item_Equip_Info iei;
//						iei.index = item.addon.equip.index;
//						iei.force = item.addon.equip.force;
//						iei.score = item.addon.equip.score;
//						iei.unban_lvl = item.addon.equip.unban_lvl;
//						player->equip_info.push_back(iei);
//					}
//				}
//			}
//		}
//	}
//	{
//		BSONObj res = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Fighter_Fields::ROLE_ID << (long long int)player->role_id));
//		if (! res.isEmpty()) {
//			if (res.hasField(Fighter_Fields::ARENA_FIGHT.c_str())) {
//				player->prop_value.push_back(Property(PT_BLOOD_MAX, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_BLOOD_MAX].numberDouble()));
//				player->prop_value.push_back(Property(PT_PHY_ATTACK, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_PHY_ATTACK].numberDouble()));
//				player->prop_value.push_back(Property(PT_PHY_DEFENSE, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE].numberDouble()));
//				player->prop_value.push_back(Property(PT_SPELL_DEFENSE, (double)res[Fighter_Fields::ARENA_FIGHT][Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE].numberDouble()));
//			}
//		}
//	}
//	return 0;
//}

int DB_Operator::robot_db_view(std::vector<Account_Key> &account_vec) {
	unsigned long long res_count = MONGO_CONNECTION.count(Role_Fields::COLLECTION);
	BSONObjBuilder field_builder;
	field_builder 	<< Role_Fields::ROLE_ID << 1
					<< Role_Fields::ACCOUNT_INFO << 1
					<< Role_Fields::LOCATION << 1
					<< Role_Fields::ROLE_NAME << 1
					<< Role_Fields::LEVEL << 1
					<< Role_Fields::GENDER << 1
					<< Role_Fields::CAREER << 1
					<< Role_Fields::HEAD_ID << 1
					<< Role_Fields::TITLE_ID << 1
					<< Role_Fields::VIP << 1
					<< Role_Fields::ONLINE << 1
					<< Role_Fields::VIP_OVER_TIME << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);

	BSONObjBuilder sort_builder;
	sort_builder << "level"  << -1;
	BSONObj sort_bson;
	sort_bson = sort_builder.obj();
	MONGO_CONNECTION.findN(iter_record, Role_Fields::COLLECTION, mongo::Query().sort(sort_bson), res_count, 0, &field_bson);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		Account_Key account;
		account.account_name = obj[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::ACCOUNT_NAME].valuestrsafe();
		account.agent_num = obj[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::AGENT_NUM].numberInt();
		account.server_num = obj[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::SERVER_NUM].numberInt();
		account_vec.push_back(account);
	}

	return 0;
}

int DB_Operator::db_role_size(void) {
	return MONGO_CONNECTION.count(Role_Fields::COLLECTION);
}

int DB_Operator::load_db_view(DB_View *db_view, bool all_info) {
	unsigned long long res_count = MONGO_CONNECTION.count(Role_Fields::COLLECTION);
	BSONObjBuilder field_builder;
	field_builder 	<< Role_Fields::ROLE_ID << 1
					<< Role_Fields::ACCOUNT_INFO << 1
					<< Role_Fields::LOCATION << 1
					<< Role_Fields::ROLE_NAME << 1
					<< Role_Fields::LEVEL << 1
					<< Role_Fields::GENDER << 1
					<< Role_Fields::CAREER << 1
					<< Role_Fields::HEAD_ID << 1
					<< Role_Fields::TITLE_ID << 1
					<< Role_Fields::VIP << 1
					<< Role_Fields::ONLINE << 1
					<< Role_Fields::LAST_SIGN_IN_TIME << 1
					<< Role_Fields::LAST_SIGN_OUT_TIME << 1
					<< Role_Fields::VIP_OVER_TIME << 1
					<< Role_Fields::AVATAR << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);

	MONGO_CONNECTION.findN(iter_record, Role_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;

		Player_DB_View *player = POOL_MANAGER->pop_player_db_view();
		if (! player) {
			MSG_USER("pop_player_db_view().pop() return 0.");
			return -1;
		}
		player->role_id = obj[Role_Fields::ROLE_ID].numberLong();
		if (player->role_id == 0) {
			LOG_ABORT("error role id, check db");
		}

		player->account_info.account_name = obj[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::ACCOUNT_NAME].valuestrsafe();
		player->account_info.agent_num = obj[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::AGENT_NUM].numberInt();
		player->account_info.server_num = obj[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::SERVER_NUM].numberInt();

		if (player->account_info.agent_num == 0 || player->account_info.server_num == 0) {
			LOG_ABORT("error role id, check db, role:%ld", player->role_id);
		}

		player->location.scene_id = obj[Role_Fields::LOCATION][Role_Fields::Location::SCENE_ID].numberInt();
		player->location.scene_order = obj[Role_Fields::LOCATION][Role_Fields::Location::SCENE_ORDER].numberInt();
		player->location.scene_layer = obj[Role_Fields::LOCATION][Role_Fields::Location::SCENE_LAYER].numberInt();

		player->role_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
		player->level = obj[Role_Fields::LEVEL].numberInt();
		player->gender = obj[Role_Fields::GENDER].numberInt();
		player->career = obj[Role_Fields::CAREER].numberInt();
		player->head_id = obj[Role_Fields::HEAD_ID].numberInt();
		player->title_id = obj[Role_Fields::TITLE_ID].numberInt();
		player->vip_level = obj[Role_Fields::VIP].numberInt();
		player->vip_over_time.sec(obj[Role_Fields::VIP_OVER_TIME].numberLong());
		player->last_sign_in_time.sec(obj[Role_Fields::LAST_SIGN_IN_TIME].numberInt());
		player->last_sign_out_time.sec(obj[Role_Fields::LAST_SIGN_OUT_TIME].numberInt());

		if (all_info) {
			BSONObjIterator iter_set(obj.getObjectField(Role_Fields::AVATAR.c_str()));
			while (iter_set.more()) {
				player->avatar_vec.push_back(iter_set.next().numberInt());
			}

			//防沉迷数据
			BSONObj res = MONGO_CONNECTION.findOne(Public_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)player->role_id));
			if (! res.isEmpty()) {
				player->fcm = res[Public_Fields::FCM].numberInt();
			}
			//
			BSONObj res1 = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)player->role_id));
			if (! res1.isEmpty()) {
				player->force = res1[Fighter_Fields::FORCE].numberInt();
				player->ttl_force = res1[Fighter_Fields::TTL_FORCE].numberInt();
			}

			BSONObj res_fsh = MONGO_CONNECTION.findOne(Fashion_Fields::COLLECTION, QUERY(Fashion_Fields::ROLE_ID << (long long int)player->role_id));
			if (! res_fsh.isEmpty()) {
				const BSONObj &item_obj = res_fsh.getObjectField(Fashion_Fields::FASHION_PART.c_str());
				BSONObjIterator iter(item_obj);
				while(iter.more()) {
					const BSONObj &obj = iter.next().embeddedObject();
					int id = obj[Fashion_Fields::Fashion_Part::USED_ID].numberInt();
					const Fashion_Base_Cfg *fashion_base_cfg = CONFIG_CACHE_FASHION->get_fashion_base_cfg(id);
					if (0 == fashion_base_cfg) continue;
					Fashion_Avatar_Map::const_iterator find_it = fashion_base_cfg->fashion_avatar_map.find(player->career);
					if (fashion_base_cfg->fashion_avatar_map.end() == find_it) continue;
					Int_Int_Map::const_iterator avatar_it = find_it->second.find(player->gender);
					if (find_it->second.end() == avatar_it) continue;
					player->avatar_vec.push_back(avatar_it->second);
				}
			}
			//好友列表和好友头像
			{
				BSONObj res_ = MONGO_CONNECTION.findOne(Friend_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)player->role_id));
				if(! res_.isEmpty()) {
					player->friend_.head_photo_id = res_[Friend_Fields::HEAD_PHOTO_ID].numberInt();
					{//friend
						BSONObj friend_map = res_.getObjectField(Friend_Fields::FRIEND_LIST.c_str());
						BSONObjIterator iter_friend(friend_map);
						while (iter_friend.more()) {
							BSONObj obj = iter_friend.next().embeddedObject();

							role_id_t role_id = obj[Friend_Fields::Friend_Info::ROLE_ID].numberLong();
							uint32_t friend_type =  obj[Friend_Fields::Friend_Info::FRIEND_TYPE].numberLong();
							if(friend_type == 0) {
								player->friend_.firend_list_.insert(role_id);
							}
						}
					}
					{//apply
						BSONObj apply_map = res_.getObjectField(Friend_Fields::APPLY_LIST.c_str());
						BSONObjIterator iter_friend(apply_map);
						player->friend_.apply_list_size = 0;
						while (iter_friend.more()) {
							BSONObj obj = iter_friend.next().embeddedObject();
							++player->friend_.apply_list_size;
						}
					}
					{//invite_list_size
						BSONObj invite_map = res_.getObjectField(Friend_Fields::INVITE_LIST.c_str());
						BSONObjIterator iter_friend(invite_map);
						player->friend_.invite_list_size = 0;
						while (iter_friend.more()) {
							BSONObj obj = iter_friend.next().embeddedObject();
							++player->friend_.invite_list_size;
						}
					}
				}
			}

			//龙谷信息
			{
				player->dragon_vale_view.role_id = player->role_id;
				load_dragon_vale_detail(player->dragon_vale_view);
			}

			// 公会
			BSONObj res_ganger = MONGO_CONNECTION.findOne(Ganger_Fields::COLLECTION, QUERY(Ganger_Fields::ROLE_ID << (long long int)player->role_id));
			if (! res_ganger.isEmpty()) {
				gang_id_t gang_id = res_ganger[Ganger_Fields::GANG_ID].numberLong();
				if (gang_id) {
					player->gang_id = gang_id;
					player->gang_name = GANG_MANAGER->gang_name(gang_id);
				}
				player->gang_war_award_get_times_by_day = res_ganger[Ganger_Fields::GANG_WAR_AWARD_GET_TIMES_BY_DAY].numberInt();
			}
		}

		db_view->insert_new_player(player);

		KNIGHT_TRIALS_MANAGER->insert_force_role_id_match(player->ttl_force, player->role_id);
	}

	KNIGHT_TRIALS_MANAGER->make_force_role_vec_form_map();

	LOG_DEBUG("load db view size:%d", db_view->id_player_view_map.size());

	return 0;
}

int DB_Operator::has_role_by_id(role_id_t role_id) {
	BSONObj res = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
	if (res.isEmpty()) return 0;
	else return 1;
}

int DB_Operator::load_base_detail(Base_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (! res.isEmpty()) {
		detail.role_id = res[Role_Fields::ROLE_ID].numberLong();
    	if (res.hasField(Role_Fields::ACCOUNT_INFO.c_str())) {
    		detail.account_info.account_name = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::ACCOUNT_NAME].valuestrsafe();
    		detail.account_info.agent_num = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::AGENT_NUM].numberInt();
    		detail.account_info.server_num = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::SERVER_NUM].numberInt();
    	}

    	if (res.hasField(Role_Fields::LOCATION.c_str())) {
    		detail.location.scene_id = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_ID].numberInt();
    		detail.location.scene_order = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_ORDER].numberInt();
    		detail.location.scene_layer = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_LAYER].numberInt();
    		detail.location.coord.x = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_X].numberInt();
    		detail.location.coord.y = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_Y].numberInt();
    		detail.location.coord.type = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_TYPE].numberInt();
    	}


		detail.role_name = res[Role_Fields::ROLE_NAME].valuestrsafe();
		detail.level = res[Role_Fields::LEVEL].numberInt();
		detail.gender = res[Role_Fields::GENDER].numberInt();
		detail.career = res[Role_Fields::CAREER].numberInt();
		detail.vip = res[Role_Fields::VIP].numberInt();
		detail.vip_over_time.sec(res[Role_Fields::VIP_OVER_TIME].numberInt());
		detail.create_time.sec(res[Role_Fields::CREATE_TIME].numberInt());
		detail.last_sign_in_time.sec(res[Role_Fields::LAST_SIGN_IN_TIME].numberInt());
		detail.last_sign_out_time.sec(res[Role_Fields::LAST_SIGN_OUT_TIME].numberInt());
		detail.last_trigger_daily_zero.sec(res[Role_Fields::LAST_TRIGGER_DAILY_ZERO].numberInt());
		detail.last_trigger_daily_six.sec(res[Role_Fields::LAST_TRIGGER_DAILY_SIX].numberInt());
		detail.last_trigger_weekly_zero.sec(res[Role_Fields::LAST_TRIGGER_WEEKLY_ZERO].numberInt());
		detail.title_id = res[Role_Fields::TITLE_ID].numberInt();
		detail.head_id = res[Role_Fields::HEAD_ID].numberInt();
		detail.charm = res[Role_Fields::CHARM].numberInt();
		detail.awake_lvl = res[Role_Fields::AWAKE_LVL].numberInt();
		detail.mount_model_id = res[Role_Fields::MOUNT_ID].numberInt();
		detail.role_type = res[Role_Fields::ROLE_TYPE].numberInt();
		return 0;
	} else {
		return -1;
	}
}

int DB_Operator::load_base_arena_detail(Base_Detail &detail){
	BSONObj res = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (! res.isEmpty()) {
		detail.role_id = res[Role_Fields::ROLE_ID].numberLong();
    	if (res.hasField(Role_Fields::ACCOUNT_INFO.c_str())) {
    		detail.account_info.account_name = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::ACCOUNT_NAME].valuestrsafe();
    		detail.account_info.agent_num = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::AGENT_NUM].numberInt();
    		detail.account_info.server_num = res[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::SERVER_NUM].numberInt();
    	}

    	if (res.hasField(Role_Fields::LOCATION.c_str())) {
    		detail.location.scene_id = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_ID].numberInt();
    		detail.location.scene_order = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_ORDER].numberInt();
    		detail.location.scene_layer = res[Role_Fields::LOCATION][Role_Fields::Location::SCENE_LAYER].numberInt();
    		detail.location.coord.x = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_X].numberInt();
    		detail.location.coord.y = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_Y].numberInt();
    		detail.location.coord.type = res[Role_Fields::LOCATION][Role_Fields::Location::COORD_TYPE].numberInt();
    	}

		detail.role_name = res[Role_Fields::ROLE_NAME].valuestrsafe();
		detail.level = res[Role_Fields::LEVEL].numberInt();
		detail.gender = res[Role_Fields::GENDER].numberInt();
		detail.career = res[Role_Fields::CAREER].numberInt();
		detail.vip = res[Role_Fields::VIP].numberInt();
		detail.vip_over_time.sec(res[Role_Fields::VIP_OVER_TIME].numberInt());
		detail.create_time.sec(res[Role_Fields::CREATE_TIME].numberInt());
		detail.last_sign_in_time.sec(res[Role_Fields::LAST_SIGN_IN_TIME].numberInt());
		detail.last_sign_out_time.sec(res[Role_Fields::LAST_SIGN_OUT_TIME].numberInt());
		detail.last_trigger_daily_zero.sec(res[Role_Fields::LAST_TRIGGER_DAILY_ZERO].numberInt());
		detail.last_trigger_daily_six.sec(res[Role_Fields::LAST_TRIGGER_DAILY_SIX].numberInt());
		detail.last_trigger_weekly_zero.sec(res[Role_Fields::LAST_TRIGGER_WEEKLY_ZERO].numberInt());
		detail.title_id = res[Role_Fields::TITLE_ID].numberInt();
		detail.head_id = res[Role_Fields::HEAD_ID].numberInt();
		detail.charm = res[Role_Fields::CHARM].numberInt();
		detail.awake_lvl = res[Role_Fields::AWAKE_LVL].numberInt();
		detail.mount_model_id = res[Role_Fields::MOUNT_ID].numberInt();
		detail.role_type = res[Role_Fields::ROLE_TYPE].numberInt();
		BSONObjIterator iter_set(res.getObjectField(Role_Fields::AVATAR.c_str()));
		while (iter_set.more()) {
			detail.avatar_vec.push_back(iter_set.next().numberInt());
		}
		return 0;
	} else {
		return -1;
	}
}

int DB_Operator::save_base_detail(Base_Detail &detail) {
	BSONObjBuilder builder;
	builder << Role_Fields::ROLE_ID << (long long int)detail.role_id
			<< Role_Fields::ROLE_NAME << detail.role_name
			<< Role_Fields::LEVEL << detail.level
			<< Role_Fields::GENDER << detail.gender
			<< Role_Fields::CAREER << detail.career
			<< Role_Fields::VIP << detail.vip
			<< Role_Fields::VIP_OVER_TIME << (long long int)detail.vip_over_time.sec()
			<< Role_Fields::CREATE_TIME << (long long int)detail.create_time.sec()
			<< Role_Fields::LAST_SIGN_IN_TIME << (long long int)detail.last_sign_in_time.sec()
			<< Role_Fields::LAST_SIGN_OUT_TIME << (long long int)detail.last_sign_out_time.sec()
			<< Role_Fields::LAST_TRIGGER_DAILY_ZERO << (long long int)detail.last_trigger_daily_zero.sec()
			<< Role_Fields::LAST_TRIGGER_DAILY_SIX << (long long int)detail.last_trigger_daily_six.sec()
			<< Role_Fields::LAST_TRIGGER_WEEKLY_ZERO << (long long int)detail.last_trigger_weekly_zero.sec()
			<< Role_Fields::TITLE_ID << detail.title_id
			<< Role_Fields::ONLINE << detail.online
			<< Role_Fields::IP << detail.ip
			<< Role_Fields::HEAD_ID << detail.head_id
			<< Role_Fields::CHARM << detail.charm
			<< Role_Fields::GANG_ID << (long long int)detail.gang_id
			<< Role_Fields::GANG_NAME << detail.gang_name
			<< Role_Fields::AWAKE_LVL << detail.awake_lvl
			<< Role_Fields::MOUNT_ID << detail.mount_model_id
			<< Role_Fields::AVATAR << detail.avatar_vec
			;

	BSONObj account_obj = BSON( Role_Fields::Account_Info::ACCOUNT_NAME << detail.account_info.account_name
							<< Role_Fields::Account_Info::AGENT_NUM << detail.account_info.agent_num
							<< Role_Fields::Account_Info::SERVER_NUM << detail.account_info.server_num);
	builder.append(Role_Fields::ACCOUNT_INFO, account_obj);

	BSONObj location_obj = BSON( Role_Fields::Location::SCENE_ID << detail.location.scene_id
							<< Role_Fields::Location::SCENE_ORDER << detail.location.scene_order
							<< Role_Fields::Location::SCENE_LAYER << detail.location.scene_layer
							<< Role_Fields::Location::COORD_X << detail.location.coord.x
							<< Role_Fields::Location::COORD_Y << detail.location.coord.y
							<< Role_Fields::Location::COORD_TYPE << detail.location.coord.type
							<< Role_Fields::Location::TOWARD << detail.location.toward);
	builder.append(Role_Fields::LOCATION, location_obj);


	MONGO_CONNECTION.update(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::reset_online_state(void) {
	BSONObjBuilder builder;
	builder << Role_Fields::ONLINE << 0;
	MONGO_CONNECTION.update(Role_Fields::COLLECTION, mongo::Query(),
			BSON("$set" << builder.obj()), false, true);
	return 0;
}

int DB_Operator::load_fighter_detail(Fighter_Detail &detail) {

	BSONObj res = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Fighter_Fields::ROLE_ID << (long long int)detail.role_id));
	if (! res.isEmpty()) {
		detail.role_id = res[Fighter_Fields::ROLE_ID].numberLong();
		//detail.phy_power_cur = res[Fighter_Fields::PHY_POWER_CUR].numberLong();
		detail.phy_power_recover_time = res[Fighter_Fields::PHY_POWER_RECOVER_TIME].numberLong();
		detail.phy_power_buy_times = res[Fighter_Fields::PHY_POWER_BUY_TIMES].numberLong();
		detail.level = res[Fighter_Fields::LEVEL].numberLong();
		detail.force = res[Fighter_Fields::FORCE].numberInt();
		detail.highest_force = res[Fighter_Fields::HIGHEST_FORCE].numberInt();
		detail.ttl_force = res[Fighter_Fields::TTL_FORCE].numberInt();
		detail.default_skill = res[Fighter_Fields::DEFAULT_SKILL].numberInt();
		detail.experience = res[Fighter_Fields::EXPERIENCE].numberDouble();
		detail.elite_btl_morale = res[Fighter_Fields::ELITE_BTL_MORALE].numberDouble();
		detail.ttl_skill_points_ = res[Fighter_Fields::TTL_SKILL_POINTER].numberInt();
		detail.blood_current.basic.basic_value = res[Fighter_Fields::CURRENT_BLOOD].numberDouble();
		detail.morale_current.basic.basic_value = res[Fighter_Fields::CURRENT_MORALE].numberDouble();
		detail.modify_normal_property(PT_LEVEL, O_SET, res[Fighter_Fields::LEVEL].numberInt());
		detail.modify_normal_property(PT_EXP_CURRENT, O_SET, res[Fighter_Fields::EXPERIENCE].numberLong());
		detail.strength_current.basic.basic_value = res[Fighter_Fields::CURRENT_STRENGTH].numberDouble();
		detail.blood_pool_current.basic.basic_value = res[Fighter_Fields::CURRENT_BLOOD_POOL].numberDouble();
		detail.cur_used_talent_ = res[Fighter_Fields::CUR_USED_TALENT].numberInt();
		detail.reset_skill_times = res[Fighter_Fields::RESET_SKILL_TIMES].numberInt();
		detail.is_clear_moral_ = res[Fighter_Fields::IS_CLEAR_MORAL].booleanSafe();

		Talent talent;
		//Talent_Page page;
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

		return 0;
	} else {
		return -1;
	}
}

int DB_Operator::handle_save_fighter_detail(const Fighter_Detail &detail, mongo::BSONObj &obj) {

	std::vector<BSONObj> vec_skill;
	for (Skill_DB_Info_Vec::const_iterator it = detail.extra_skill_vec.begin(); it != detail.extra_skill_vec.end(); ++it) {
		vec_skill.push_back(BSON(Fighter_Fields::Arena_Fight::Extra_Skill::SKILL_ID << it->skill_id
				<< Fighter_Fields::Arena_Fight::Extra_Skill::SKILL_LV << it->skill_level));
	}

	BSONObjBuilder builder;
	builder << Fighter_Fields::Arena_Fight::PT_PHY_ATTACK << detail.physical_attack.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SPELL_ATTACK << detail.spell_attack.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE << detail.physical_defense.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE << detail.spell_defense.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_HIT << detail.hit_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_AVOID << detail.avoid.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_IGNORE_DEF << detail.ignore_def.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_BLOOD_MAX << detail.blood_max.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_CURE << detail.cure.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_CURE_RATE << detail.cure_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SING_RATE << detail.sing_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_CRIT << detail.crit.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_CRIT_DEF << detail.crit_def.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_INIT_MORALE << detail.init_morale.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_MORALE_CAPS << detail.morale_caps.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_CRIT_HURT << detail.crit_hurt.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_CRIT_HURT_SUB << detail.crit_hurt_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_HIT_RATE_ADDITION << detail.hit_rate_addition.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_AVOID_RATE_ADDITION << detail.avoid_rate_addition.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_CRIT_RATE_ADDITION << detail.crit_rate_addition.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_CRIT_DEF_RATE_ADDITION << detail.crit_def_rate_addition.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SPEED << detail.speed.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_BLOOD_CURRENT << detail.blood_current.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_MORALE_CURRENT << detail.morale_current.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_STRENGTH_CURRENT << detail.strength_current.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_STRENGTH_MAX << detail.strength_max.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_CURRENT << detail.blood_pool_current.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_MAX << detail.blood_pool_max.total(Prop_Value::ELEM_FORCE)

			<< Fighter_Fields::Arena_Fight::PT_IGNORE_DEF_RATE << detail.ignore_def_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_STATUS_HIT_RATE << detail.status_hit_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_STATUS_DEF_RATE << detail.status_def_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_BLOOD_STEAL << detail.blood_steal.total(Prop_Value::ELEM_FORCE)

			<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_FIX << detail.phy_hurt_fix.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE << detail.phy_hurt_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_FIX << detail.spell_hurt_fix.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE << detail.spell_hurt_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_HURT_FIX << detail.hurt_fix.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_HURT_RATE << detail.hurt_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_BE_HURT_FIX << detail.be_hurt_fix.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_BE_HURT_RATE << detail.be_hurt_rate.total(Prop_Value::ELEM_FORCE)

			<< Fighter_Fields::Arena_Fight::PT_PK_HURT_ADD << detail.pk_hurt_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_PK_HURT_SUB << detail.pk_hurt_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE_SUB << detail.phy_hurt_rate_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE_SUB << detail.spell_hurt_rate_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SKILL_HURT_RATE << detail.skill_hurt_rate.total(Prop_Value::ELEM_FORCE)

			<< Fighter_Fields::Arena_Fight::PT_HUMAN_ADD << detail.human_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_ELF_ADD << detail.elf_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_ORC_ADD << detail.orc_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_DRAGON_ADD << detail.dragon_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_UNDEAD_ADD << detail.undead_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_DEMON_ADD << detail.demon_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_HUMAN_SUB << detail.human_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_ELF_SUB << detail.elf_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_ORC_SUB << detail.orc_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_DRAGON_SUB << detail.dragon_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_UNDEAD_SUB << detail.undead_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_DEMON_SUB << detail.demon_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SUMMON_ADD << detail.summon_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_SUMMON_SUB << detail.summon_sub.total(Prop_Value::ELEM_FORCE)

			<< Fighter_Fields::Arena_Fight::PT_BE_CURE_RATE << detail.be_cure_rate.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_EFFECT_RATE << detail.effect_rate.total(Prop_Value::ELEM_FORCE)

			<< Fighter_Fields::Arena_Fight::PT_PVE_HURT_ADD << detail.pve_hurt_add.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_PVE_HURT_SUB << detail.pve_hurt_sub.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_DIZZ_STRENGTHEN << detail.dizz_strengthen.total(Prop_Value::ELEM_FORCE)
			<< Fighter_Fields::Arena_Fight::PT_DIZZ_WEAKEN << detail.dizz_weaken.total(Prop_Value::ELEM_FORCE)

			<< Fighter_Fields::Arena_Fight::FORCE << detail.force
			<< Fighter_Fields::Arena_Fight::EXTRA_SKILL << vec_skill
			;

	obj = builder.obj();
	return 0;
}

int DB_Operator::handle_load_fighter_detail(Fighter_Detail &detail, const mongo::BSONObj &obj) {
	if (obj.isEmpty()) {
		MSG_DEBUG("no fighter detail, role_id=%ld", detail.role_id);
		return -1;
	}

	detail.physical_attack.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PHY_ATTACK].numberDouble();
	detail.spell_attack.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SPELL_ATTACK].numberDouble();
	detail.physical_defense.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PHY_DEFENSE].numberDouble();
	detail.spell_defense.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SPELL_DEFENSE].numberDouble();
	detail.hit_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_HIT].numberDouble();
	detail.avoid.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_AVOID].numberDouble();
	detail.ignore_def.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_IGNORE_DEF].numberDouble();
	detail.blood_max.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_BLOOD_MAX].numberDouble();
	detail.cure.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_CURE].numberDouble();
	detail.cure_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_CURE_RATE].numberDouble();
	detail.sing_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SING_RATE].numberDouble();
	detail.crit.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_CRIT].numberDouble();
	detail.crit_def.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_CRIT_DEF].numberDouble();
	detail.init_morale.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_INIT_MORALE].numberDouble();
	detail.morale_caps.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_MORALE_CAPS].numberDouble();
	detail.crit_hurt.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_CRIT_HURT].numberDouble();
	detail.crit_hurt_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_CRIT_HURT_SUB].numberDouble();
	detail.hit_rate_addition.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_HIT_RATE_ADDITION].numberDouble();
	detail.avoid_rate_addition.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_AVOID_RATE_ADDITION].numberDouble();
	detail.crit_rate_addition.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_CRIT_RATE_ADDITION].numberDouble();
	detail.crit_def_rate_addition.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_CRIT_DEF_RATE_ADDITION].numberDouble();
	detail.speed.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SPEED].numberDouble();
	detail.blood_current.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_BLOOD_CURRENT].numberDouble();
	detail.morale_current.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_MORALE_CURRENT].numberDouble();
	detail.strength_current.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_STRENGTH_CURRENT].numberDouble();
	detail.strength_max.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_STRENGTH_MAX].numberDouble();
	detail.blood_pool_current.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_CURRENT].numberDouble();
	detail.blood_pool_max.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_BLOOD_POOL_MAX].numberDouble();

	detail.ignore_def_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_IGNORE_DEF_RATE].numberDouble();
	detail.status_hit_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_STATUS_HIT_RATE].numberDouble();
	detail.status_def_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_STATUS_DEF_RATE].numberDouble();
	detail.blood_steal.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_BLOOD_STEAL].numberDouble();

	detail.phy_hurt_fix.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PHY_HURT_FIX].numberDouble();
	detail.phy_hurt_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE].numberDouble();
	detail.spell_hurt_fix.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SPELL_HURT_FIX].numberDouble();
	detail.spell_hurt_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE].numberDouble();
	detail.hurt_fix.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_HURT_FIX].numberDouble();
	detail.hurt_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_HURT_RATE].numberDouble();
	detail.be_hurt_fix.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_BE_HURT_FIX].numberDouble();
	detail.be_hurt_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_BE_HURT_RATE].numberDouble();

	detail.pk_hurt_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PK_HURT_ADD].numberDouble();
	detail.pk_hurt_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PK_HURT_SUB].numberDouble();
	detail.phy_hurt_rate_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PHY_HURT_RATE_SUB].numberDouble();
	detail.spell_hurt_rate_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SPELL_HURT_RATE_SUB].numberDouble();
	detail.skill_hurt_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SKILL_HURT_RATE].numberDouble();

	detail.human_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_HUMAN_ADD].numberDouble();
	detail.elf_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_ELF_ADD].numberDouble();
	detail.orc_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_ORC_ADD].numberDouble();
	detail.dragon_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_DRAGON_ADD].numberDouble();
	detail.undead_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_UNDEAD_ADD].numberDouble();
	detail.demon_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_DEMON_ADD].numberDouble();
	detail.human_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_HUMAN_SUB].numberDouble();
	detail.elf_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_ELF_SUB].numberDouble();
	detail.orc_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_ORC_SUB].numberDouble();
	detail.dragon_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_DRAGON_SUB].numberDouble();
	detail.undead_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_UNDEAD_SUB].numberDouble();
	detail.demon_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_DEMON_SUB].numberDouble();
	detail.summon_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SUMMON_ADD].numberDouble();
	detail.summon_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_SUMMON_SUB].numberDouble();

	detail.be_cure_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_BE_CURE_RATE].numberDouble();
	detail.effect_rate.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_EFFECT_RATE].numberDouble();
	detail.pve_hurt_add.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PVE_HURT_ADD].numberDouble();
	detail.pve_hurt_sub.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_PVE_HURT_SUB].numberDouble();
	detail.dizz_strengthen.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_DIZZ_STRENGTHEN].numberDouble();
	detail.dizz_weaken.basic.basic_value = obj[Fighter_Fields::Arena_Fight::PT_DIZZ_WEAKEN].numberDouble();

	if(detail.force <= 0) {
		detail.force = obj[Fighter_Fields::Arena_Fight::FORCE].numberInt();
	}

	if (obj.hasField(Fighter_Fields::Arena_Fight::EXTRA_SKILL.c_str())) {
		Skill_DB_Info skill;
		BSONObjIterator it(obj.getObjectField(Fighter_Fields::Arena_Fight::EXTRA_SKILL.c_str()));
		while(it.more()) {
			skill.reset();

			BSONObj skill_obj = it.next().embeddedObject();
			skill.skill_id = skill_obj[Fighter_Fields::Arena_Fight::Extra_Skill::SKILL_ID].numberInt();
			skill.skill_level = skill_obj[Fighter_Fields::Arena_Fight::Extra_Skill::SKILL_LV].numberInt();
//			detail.extra_skill_map[skill.skill_id] = skill;
			detail.extra_skill_vec.push_back(skill);
		}
	}

	return 0;
}

int DB_Operator::load_arena_fighter_detail(Fighter_Detail &detail){
	BSONObj res = MONGO_CONNECTION.findOne(Fighter_Fields::COLLECTION, QUERY(Fighter_Fields::ROLE_ID << (long long int)detail.role_id));
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
		return 0;
	} else {
		return -1;
	}
}

int DB_Operator::save_fighter_detail(Fighter_Detail &detail, bool save_arena) {
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
			<< Fighter_Fields::ELITE_BTL_MORALE << detail.elite_btl_morale
			<< Fighter_Fields::FORCE << detail.force
			<< Fighter_Fields::HIGHEST_FORCE << detail.highest_force
			<< Fighter_Fields::TTL_FORCE << detail.ttl_force
			<< Fighter_Fields::DEFAULT_SKILL << detail.default_skill
			<< Fighter_Fields::IS_CLEAR_MORAL << detail.is_clear_moral_
			<< Fighter_Fields::CUR_USED_TALENT << detail.cur_used_talent_
			<< Fighter_Fields::RESET_SKILL_TIMES << detail.reset_skill_times
			<< Fighter_Fields::TALENT << vc_talent;

	// 为了减少对数据库操作,只有当下线并等级大于10级才保持属性
	if (detail.logout && detail.level > 10) {
		BSONObjBuilder attr_builder;
		save_fighter_attr(attr_builder, detail);
		builder.append(Fighter_Fields::ATTRIBUTE, attr_builder.obj());
	}

	// 只有当下线才保持属性
//	if (detail.logout && detail.level > 10) {
	if (save_arena) {
		BSONObj arena_fight_obj;
		handle_save_fighter_detail(detail, arena_fight_obj);
		builder.append(Fighter_Fields::ARENA_FIGHT, arena_fight_obj);
	}
//	}

	MONGO_CONNECTION.update(Fighter_Fields::COLLECTION, QUERY(Fighter_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::save_fighter_attr(BSONObjBuilder &builder, Fighter_Detail &detail) {
	std::stringstream str;
	std::string type = "";

	save_fighter_prop(builder, str, detail.power);
	save_fighter_prop(builder, str, detail.agility);
	save_fighter_prop(builder, str, detail.dexterity);
	save_fighter_prop(builder, str, detail.armor);
	save_fighter_prop(builder, str, detail.stamina);
	save_fighter_prop(builder, str, detail.physical_attack);
	save_fighter_prop(builder, str, detail.spell_attack);
	save_fighter_prop(builder, str, detail.physical_defense);
	save_fighter_prop(builder, str, detail.spell_defense);
	save_fighter_prop(builder, str, detail.hit_rate);
	save_fighter_prop(builder, str, detail.avoid);
	save_fighter_prop(builder, str, detail.ignore_def);
	save_fighter_prop(builder, str, detail.blood_max);
	save_fighter_prop(builder, str, detail.crit);
	save_fighter_prop(builder, str, detail.crit_def);
	save_fighter_prop(builder, str, detail.init_morale);
	save_fighter_prop(builder, str, detail.crit_hurt);
	save_fighter_prop(builder, str, detail.crit_hurt_sub);
	save_fighter_prop(builder, str, detail.hit_rate_addition);
	save_fighter_prop(builder, str, detail.avoid_rate_addition);
	save_fighter_prop(builder, str, detail.speed);
	save_fighter_prop(builder, str, detail.pk_hurt_add);
	save_fighter_prop(builder, str, detail.pk_hurt_sub);
	save_fighter_prop(builder, str, detail.sing_rate);
	save_fighter_prop(builder, str, detail.status_hit_rate);
	save_fighter_prop(builder, str, detail.status_def_rate);
	save_fighter_prop(builder, str, detail.cure_rate);
	return 0;
}


int DB_Operator::save_fighter_prop(BSONObjBuilder &builder, std::stringstream& str, Prop_Value& prop) {
	str << prop.type;
	std::string type = str.str();

	BSONObjBuilder prop_builder;

	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_BASIC, prop.basic);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_EQUIP, prop.equip);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_CONVERSION, prop.conversion);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_INITIATIVE_SKILL, prop.initiative_skill);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_STATUS, prop.status);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_PASSIVE_SKILL, prop.passive_skill);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_RUNE_STONE, prop.rune_stone);
//	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_RUNE, prop.rune);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_HERO_ASSIST, prop.hero_assist);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_GANG, prop.gang);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_ARTIFACTS, prop.artifacts);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_ACHIEVEMENT, prop.achievement);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_TITLE, prop.title);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_DRAGON_VALE, prop.dragon_vale);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_ANCI_ARTIFACT, prop.anci_artifact);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_WING, prop.wing);
	save_fighter_prop_value(prop_builder, Fighter_Fields::Attribute::AT_MOUNT, prop.mount);

	builder.append(type, prop_builder.obj());

	str.str("");
	str.clear();

	return 0;
}

int DB_Operator::save_fighter_prop_value(BSONObjBuilder &builder, const std::string& name, Prop_Base& prop) {

	BSONObj obj = BSON(Fighter_Fields::Attribute::Attr_Part::BASE << prop.basic_value
			<< Fighter_Fields::Attribute::Attr_Part::FIX << prop.fixed_value
			<< Fighter_Fields::Attribute::Attr_Part::PERCENT << prop.percent_value);
	builder.append(name, obj);
	return 0;
}

int DB_Operator::load_mail_detail(Mail_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Mail_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return 0;
	}
	detail.mail_all_count = res[Mail_Fields::MAIL_ALL_COUNT.c_str()].numberInt();
	detail.mail_page_count = res[Mail_Fields::MAIL_PAGE_COUNT.c_str()].numberInt();
	detail.mail_current_page = res[Mail_Fields::MAIL_CURRENT_PAGE.c_str()].numberInt();
	detail.last_read_sys_mail_time.sec(res[Mail_Fields::LAST_READ_SYS_MAIL_TIME.c_str()].numberInt());
	detail.max_gold = res[Mail_Fields::MAX_GOLD.c_str()].numberInt();

	BSONObj mail = res.getObjectField(Mail_Fields::MAIL_LIST.c_str());
	BSONObjIterator iter(mail);
	while (iter.more()) {
		BSONObj obj = iter.next().embeddedObject();
		Mail_Data mail_info_detal;
		mail_info_detal.reset();

		mail_info_detal.mail_id = obj[Mail_Fields::Mail_List::MAIL_ID].numberInt();
		mail_info_detal.have_read = obj[Mail_Fields::Mail_List::HAVE_READ].numberInt();
		mail_info_detal.gold = obj[Mail_Fields::Mail_List::COIN].numberInt();
		mail_info_detal.mail_type = obj[Mail_Fields::Mail_List::MAIL_TYPE].numberInt();
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
		if(mail_info_detal.mail_type) {
			detail.mail_map.insert(std::make_pair(mail_info_detal.mail_id, mail_info_detal));
		} else {
			detail.sys_mail_map.insert(std::make_pair(mail_info_detal.mail_id, mail_info_detal));
		}
	}

	return 0;
}

int DB_Operator::save_mail_detail(Mail_Detail &detail) {
	std::vector<BSONObj> mail_vector;
	for (Mail_Detail::Mail_Map::const_iterator iter = detail.mail_map.begin(); iter != detail.mail_map.end(); iter++) {
		std::vector<BSONObj> item_vector;
		//BSONObj obj;
		for(Item_Vec::const_iterator item_iter = iter->second.item_vector.begin(); item_iter != iter->second.item_vector.end(); item_iter++ ) {
			item_vector.push_back( BSON(Package_Fields::Item::INDEX << item_iter->index_ << Package_Fields::Item::ID
					<< item_iter->id_ << Package_Fields::Item::AMOUNT << item_iter->amount_
					<< Package_Fields::Item::BIND << item_iter->bind_
					<< Package_Fields::Item::OVERDUE_TIME << (long long int)item_iter->overdue_time.sec()));
		}
		mail_vector.push_back( BSON(Mail_Fields::Mail_List::MAIL_ID << (long long int)iter->second.mail_id
		<< Mail_Fields::Mail_List::HAVE_READ << iter->second.have_read
		<< Mail_Fields::Mail_List::COIN << iter->second.gold
		<< Mail_Fields::Mail_List::MAIL_TITLE << iter->second.mail_title
		<< Mail_Fields::Mail_List::MAIL_TYPE << iter->second.mail_type
		<< Mail_Fields::Mail_List::MAIL_CONTENT << iter->second.mail_content
		<< Mail_Fields::Mail_List::SENDER_NAME << iter->second.sender_name
		<< Mail_Fields::Mail_List::ITEM << item_vector
		<< Mail_Fields::Mail_List::SHOW_SEND_TIME << (long long int)iter->second.show_send_time.sec()
		<< Mail_Fields::Mail_List::REAL_SEND_TIME << (long long int)iter->second.real_send_time.sec()));
	}
	for (Mail_Detail::Mail_Map::const_iterator iter = detail.sys_mail_map.begin(); iter != detail.sys_mail_map.end(); iter++) {
		std::vector<BSONObj> item_vector;
		//BSONObj obj;
		for(Item_Vec::const_iterator item_iter = iter->second.item_vector.begin(); item_iter != iter->second.item_vector.end(); item_iter++ ) {
			item_vector.push_back( BSON(Package_Fields::Item::INDEX << item_iter->index_ << Package_Fields::Item::ID
					<< item_iter->id_ << Package_Fields::Item::AMOUNT << item_iter->amount_
					<< Package_Fields::Item::BIND << item_iter->bind_
					<< Package_Fields::Item::OVERDUE_TIME << (long long int)item_iter->overdue_time.sec()));
		}
		mail_vector.push_back( BSON(Mail_Fields::Mail_List::MAIL_ID << (long long int)iter->second.mail_id
		<< Mail_Fields::Mail_List::HAVE_READ << iter->second.have_read
		<< Mail_Fields::Mail_List::COIN << iter->second.gold
		<< Mail_Fields::Mail_List::MAIL_TITLE << iter->second.mail_title
		<< Mail_Fields::Mail_List::MAIL_TYPE << iter->second.mail_type
		<< Mail_Fields::Mail_List::MAIL_CONTENT << iter->second.mail_content
		<< Mail_Fields::Mail_List::SENDER_NAME << iter->second.sender_name
		<< Mail_Fields::Mail_List::ITEM << item_vector
		<< Mail_Fields::Mail_List::SHOW_SEND_TIME << (long long int)iter->second.show_send_time.sec()
		<< Mail_Fields::Mail_List::REAL_SEND_TIME << (long long int)iter->second.real_send_time.sec()));
	}

	BSONObjBuilder set_builder;
	set_builder << Mail_Fields::MAIL_ALL_COUNT << detail.mail_all_count
				<< Mail_Fields::MAIL_PAGE_COUNT << detail.mail_page_count
				<< Mail_Fields::MAIL_CURRENT_PAGE << detail.mail_current_page
				<< Mail_Fields::LAST_READ_SYS_MAIL_TIME << (long long int)detail.last_read_sys_mail_time.sec()
				<< Mail_Fields::MAX_GOLD << detail.max_gold
				<< Mail_Fields::MAIL_LIST << mail_vector;

	MONGO_CONNECTION.update(Mail_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << set_builder.obj() ), true);

	return 0;
}

int DB_Operator::load_alchemy_detail(Alchemy_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Alchemy_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	detail.role_id = res[Role_Fields::ROLE_ID].numberLong();
	//detail.current_hero_critical = res[Alchemy_Fields::CURRENT_HERO_CRITICAL.c_str()].numberInt();
	//detail.current_hero_gold = res[Alchemy_Fields::CUTTENT_HERO_GOLD.c_str()].numberInt();
	detail.used_free_bout = res[Alchemy_Fields::OVER_FREE_BOUT.c_str()].numberInt();
	detail.used_pay_bout = res[Alchemy_Fields::OVER_PAY_BOUT.c_str()].numberInt();
	detail.last_tabs = res[Alchemy_Fields::LAST_TABS.c_str()].numberInt();
	detail.six_critical_num = res[Alchemy_Fields::SIX_CRITICAL_NUM.c_str()].numberInt();
//souls
	detail.used_souls_free_bout = res[Alchemy_Fields::SOVER_FREE_BOUT.c_str()].numberInt();
	detail.used_souls_pay_bout = res[Alchemy_Fields::SOVER_PAY_BOUT.c_str()].numberInt();
	detail.last_souls_tabs = res[Alchemy_Fields::SLAST_TABS.c_str()].numberInt();
	detail.six_critical_souls_num = res[Alchemy_Fields::SSIX_CRITICAL_NUM.c_str()].numberInt();

	return 0;
}

int DB_Operator::save_alchemy_detail(Alchemy_Detail &detail) {
	BSONObjBuilder builder;
	builder //<< Alchemy_Fields::CURRENT_HERO_CRITICAL << detail.current_hero_critical
			//<< Alchemy_Fields::CUTTENT_HERO_GOLD << detail.current_hero_gold
			<< Alchemy_Fields::LAST_TABS << detail.last_tabs
			<< Alchemy_Fields::OVER_FREE_BOUT << detail.used_free_bout
			<< Alchemy_Fields::SIX_CRITICAL_NUM << detail.six_critical_num
			<< Alchemy_Fields::OVER_PAY_BOUT << detail.used_pay_bout
			<< Alchemy_Fields::SLAST_TABS << detail.last_souls_tabs
			<< Alchemy_Fields::SOVER_FREE_BOUT << detail.used_souls_free_bout
			<< Alchemy_Fields::SSIX_CRITICAL_NUM << detail.six_critical_souls_num
			<< Alchemy_Fields::SOVER_PAY_BOUT << detail.used_souls_pay_bout;

	MONGO_CONNECTION.update(Alchemy_Fields::COLLECTION,
			QUERY(Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_friend_detail(Friend_Detail &detail) {
	Time_Value now = Time_Value::gettimeofday();
	BSONObj res = MONGO_CONNECTION.findOne(Friend_Fields::COLLECTION,QUERY(Friend_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	detail.role_id = res[Friend_Fields::ROLE_ID].numberLong();
	detail.head_photo_id = res[Friend_Fields::HEAD_PHOTO_ID].numberInt();

	//friend
	BSONObj friend_map = res.getObjectField(Friend_Fields::FRIEND_LIST.c_str());
	BSONObjIterator iter_friend(friend_map);
	detail.firend_list_.clear();
	while (iter_friend.more()) {
		BSONObj obj = iter_friend.next().embeddedObject();
		Friend_Info fi;
		fi.reset();
		fi.role_id = obj[Friend_Fields::Friend_Info::ROLE_ID].numberLong();
		fi.friend_type =  obj[Friend_Fields::Friend_Info::FRIEND_TYPE].numberLong();
		fi.friendship_degree = obj[Friend_Fields::Friend_Info::FRIENDSHIP_DEGREE].numberInt();
		if(fi.friend_type == 0) {
			detail.firend_list_.insert(std::make_pair(fi.role_id, fi));
		}
	}
	//stranger
	BSONObj stranger_map = res.getObjectField(Friend_Fields::STRANGER_LIST.c_str());
	BSONObjIterator iter_stranger(stranger_map);
	detail.stranger_list_.clear();
	while (iter_stranger.more()) {
		BSONObj obj = iter_stranger.next().embeddedObject();
		Friend_Info fi;
		fi.reset();
		fi.role_id = obj[Friend_Fields::Friend_Info::ROLE_ID].numberLong();
		fi.friend_type =  obj[Friend_Fields::Friend_Info::FRIEND_TYPE].numberLong();
		if(fi.friend_type == 1) {
			detail.stranger_list_.insert(std::make_pair(fi.role_id, fi));
		}
	}
	//black
	BSONObj black_map = res.getObjectField(Friend_Fields::BLACK_LIST.c_str());
	BSONObjIterator iter_black(black_map);
	detail.black_list_.clear();
	while (iter_black.more()) {
		BSONObj obj = iter_black.next().embeddedObject();
		Friend_Info fi;
		fi.reset();
		fi.role_id = obj[Friend_Fields::Friend_Info::ROLE_ID].numberLong();
		fi.friend_type =  obj[Friend_Fields::Friend_Info::FRIEND_TYPE].numberLong();
		if(fi.friend_type == 2) {
			detail.black_list_.insert(std::make_pair(fi.role_id, fi));
		}
	}
	//apply
	BSONObj apply_map = res.getObjectField(Friend_Fields::APPLY_LIST.c_str());
	BSONObjIterator iter_apply(apply_map);
	detail.apply_list_.clear();
	while (iter_apply.more()) {
		BSONObj obj = iter_apply.next().embeddedObject();
		now.sec(obj[Friend_Fields::Apply::TIME].numberLong());
		detail.apply_list_.insert(std::make_pair(obj[Friend_Fields::Apply::ROLE_ID].numberLong(), now));
	}
	//invite
	BSONObj invite_map = res.getObjectField(Friend_Fields::INVITE_LIST.c_str());
	BSONObjIterator iter_invite(invite_map);
	detail.invite_list_.clear();
	while (iter_invite.more()) {
		BSONObj obj = iter_invite.next().embeddedObject();
		now.sec(obj[Friend_Fields::Apply::TIME].numberLong());
		detail.invite_list_.insert(std::make_pair(obj[Friend_Fields::Apply::ROLE_ID].numberLong(), now));
	}
	//del
	BSONObj del_map = res.getObjectField(Friend_Fields::DEL_LIST.c_str());
	BSONObjIterator iter_del(del_map);
	detail.del_firend_list_.clear();
	while (iter_del.more()) {
		BSONObj obj = iter_del.next().embeddedObject();
		detail.del_firend_list_.insert(std::make_pair( obj[Friend_Fields::Del::ROLE_ID].numberLong(),
				obj[Friend_Fields::Del::FRIENDSHIP_DEGREE].numberInt() ));
	}
	return 0;
}

int DB_Operator::save_friend_detail(Friend_Detail &detail) {
	//friend_list
	vector<BSONObj> vc_firend_list;
	for(Friend_Info_Map::iterator iter = detail.firend_list_.begin(); iter != detail.firend_list_.end(); ++iter) {
		BSONObjBuilder firend_builder;
		if((iter->second).friend_type == 0) {
			firend_builder << Friend_Fields::Friend_Info::ROLE_ID << (long long int)(iter->first)
				    << Friend_Fields::Friend_Info::FRIEND_TYPE << (int8_t)(iter->second).friend_type
					<< Friend_Fields::Friend_Info::FRIENDSHIP_DEGREE << (iter->second).friendship_degree;
			vc_firend_list.push_back(firend_builder.obj());
		}
	}
	//stranger_list
	vector<BSONObj> vc_stranger_list;
	for(Friend_Info_Map::iterator iter = detail.stranger_list_.begin(); iter != detail.stranger_list_.end(); ++iter) {
		BSONObjBuilder stranger_builder;
		if((iter->second).friend_type == 1) {
			stranger_builder << Friend_Fields::Friend_Info::ROLE_ID << (long long int)(iter->first)
						<< Friend_Fields::Friend_Info::FRIEND_TYPE << (int8_t)(iter->second).friend_type;
			vc_stranger_list.push_back(stranger_builder.obj());
		}
	}
	//black_list
	vector<BSONObj> vc_black_list;
	for(Friend_Info_Map::iterator iter = detail.black_list_.begin(); iter != detail.black_list_.end(); ++iter) {
		BSONObjBuilder black_builder;
		if((iter->second).friend_type == 2) {
			black_builder << Friend_Fields::Friend_Info::ROLE_ID << (long long int)(iter->first)
						<< Friend_Fields::Friend_Info::FRIEND_TYPE << (int8_t)(iter->second).friend_type;
			vc_black_list.push_back(black_builder.obj());
		}

	}
	//invite_list
	vector<BSONObj> vc_invite_list;
	for(Role_Id_Time_Map::iterator iter = detail.invite_list_.begin(); iter != detail.invite_list_.end(); ++iter) {
		BSONObjBuilder invite_builder;
		invite_builder << Friend_Fields::Invite::ROLE_ID << (long long int)(iter->first)
				    << Friend_Fields::Invite::TIME << (long long int)(iter->second).sec();
		vc_invite_list.push_back(invite_builder.obj());
	}
	//apply_list
	vector<BSONObj> vc_apply_list;
	for(Role_Id_Time_Map::iterator iter = detail.apply_list_.begin(); iter != detail.apply_list_.end(); ++iter) {
		BSONObjBuilder apply_builder;
		apply_builder << Friend_Fields::Apply::ROLE_ID << (long long int)(iter->first)
				    << Friend_Fields::Apply::TIME << (long long int)(iter->second).sec();
		vc_apply_list.push_back(apply_builder.obj());
	}

	//del_firend_list
	vector<BSONObj> vc_del_list;
	for(RoleID_Friend_Map::iterator iter = detail.del_firend_list_.begin(); iter != detail.del_firend_list_.end(); ++iter) {
		BSONObjBuilder del_builder;
		del_builder << Friend_Fields::Del::ROLE_ID << (long long int)(iter->first)
				    << Friend_Fields::Del::FRIENDSHIP_DEGREE << iter->second;
		vc_del_list.push_back(del_builder.obj());
	}

	BSONObjBuilder builder;
	builder << Friend_Fields::FRIEND_LIST << vc_firend_list <<
			Friend_Fields::STRANGER_LIST << vc_stranger_list <<
			Friend_Fields::BLACK_LIST << vc_black_list <<
			Friend_Fields::INVITE_LIST << vc_invite_list <<
			Friend_Fields::APPLY_LIST << vc_apply_list <<
			Friend_Fields::DEL_LIST << vc_del_list <<
			Friend_Fields::HEAD_PHOTO_ID << detail.head_photo_id;

	MONGO_CONNECTION.update(Friend_Fields::COLLECTION,
			QUERY(Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::save_life_skill_data(const Life_Skill_Detail &detail) {

	BSONObjBuilder builder;
	vector<BSONObj> skill_list;

	for ( Life_Skill_Map::const_iterator iter = detail.skill_map.begin();iter != detail.skill_map.end();++iter) {
		BSONObjBuilder tmp_builder;
		tmp_builder << Life_Skill_Data_Fields::Life_Skill_Data::SKILL_ID << iter->second.val_1;
		tmp_builder << Life_Skill_Data_Fields::Life_Skill_Data::DEGREE << iter->second.val_2;
		skill_list.push_back(tmp_builder.obj());
	}
	builder << Life_Skill_Data_Fields::ENERGY << detail.energy <<
			Life_Skill_Data_Fields::RE_TIME << detail.remain_time <<
			Life_Skill_Data_Fields::INIL << detail.inil <<
			Life_Skill_Data_Fields::LF_SKILL_LIST << skill_list;
	MONGO_CONNECTION.update(Life_Skill_Data_Fields::COLLECTION,
			QUERY(Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_life_skill_data(Life_Skill_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Life_Skill_Data_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	//detail.role_id = res[Life_Skill_Data_Fields::ROLE_ID].numberLong();
	detail.energy = res[Life_Skill_Data_Fields::ENERGY].numberInt();
	detail.inil = res[Life_Skill_Data_Fields::INIL].numberInt();
	detail.remain_time = res[Life_Skill_Data_Fields::RE_TIME].numberInt();
	BSONObj skill_list = res.getObjectField(Life_Skill_Data_Fields::LF_SKILL_LIST.c_str());
	BSONObjIterator iter_skill(skill_list);
	detail.skill_map.clear();
	while(iter_skill.more()) {
		Int_Int skill_unit;
		BSONObj obj = iter_skill.next().embeddedObject();
		skill_unit.val_1 = obj[Life_Skill_Data_Fields::Life_Skill_Data::SKILL_ID].numberInt();
		skill_unit.val_2 = obj[Life_Skill_Data_Fields::Life_Skill_Data::DEGREE].numberInt();
		detail.skill_map.insert(std::make_pair(skill_unit.val_1/1000,skill_unit));
	}
	return 0;
}

int DB_Operator::load_example_detail(Example_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Example_Fields::COLLECTION, QUERY(Example_Fields::ROLE_ID << (long long int)detail.role_id));
	if (! res.isEmpty()) {
		detail.role_id = res[Example_Fields::ROLE_ID].numberLong();

		return 0;
	} else {
		return -1;
	}
	return 0;
}

int DB_Operator::save_example_detail(Example_Detail &detail) {
	BSONObjBuilder builder;
	builder << Example_Fields::ROLE_ID << (long long int)detail.role_id;

	MONGO_CONNECTION.update(Example_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_icon_detail(Icon_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Icon_Fields::COLLECTION, QUERY(Icon_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Icon_Fields::ROLE_ID].numberLong();

	//icon list
	BSONObj icon_map = res.getObjectField(Icon_Fields::ICON.c_str());
	BSONObjIterator iter_icon(icon_map);
	while (iter_icon.more()) {
		BSONObj obj = iter_icon.next().embeddedObject();
		Svr_Icon_Info svr_icon_info;
		svr_icon_info.reset();
		svr_icon_info.id = (icon_id_t)obj[Icon_Fields::Icon::ICON_ID].numberInt();
		svr_icon_info.status = (uint8_t)obj[Icon_Fields::Icon::STATUS].numberInt();
		svr_icon_info.is_ever_opened = (obj[Icon_Fields::Icon::IS_EVER_OPENED].numberInt() == 1) ? true : false;
		svr_icon_info.is_used_item = (obj[Icon_Fields::Icon::IS_USED_ITEM].numberInt() == 1) ? true : false;
		detail.icon_map.insert(std::make_pair(svr_icon_info.id, svr_icon_info));
	}

	return 0;
}

int DB_Operator::save_icon_detail(Icon_Detail &detail) {
	//icon_list
    vector<BSONObj> vc_icon_list;
	for(Icon_Info_Map::iterator iter = detail.icon_map.begin(); iter != detail.icon_map.end(); ++iter) {
		BSONObjBuilder icon_builder;
		icon_builder << Icon_Fields::Icon::ICON_ID << (icon_id_t)(iter->first)
				    << Icon_Fields::Icon::STATUS << (int8_t)(iter->second).status
                    << Icon_Fields::Icon::IS_EVER_OPENED << (iter->second.is_ever_opened ? 1 : 0)
                    << Icon_Fields::Icon::IS_USED_ITEM << (iter->second.is_used_item ? 1 : 0);
			vc_icon_list.push_back(icon_builder.obj());
	}

	BSONObjBuilder builder;
	builder << Icon_Fields::ROLE_ID << (long long int)detail.role_id
	        << Icon_Fields::ICON << vc_icon_list;

	MONGO_CONNECTION.update(Icon_Fields::COLLECTION, QUERY(Icon_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_dragon_vale_detail(Dragon_Vale_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Dragon_Vale_Fields::COLLECTION, QUERY(Dragon_Vale_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Dragon_Vale_Fields::ROLE_ID].numberLong();
	detail.target_player_id = res[Dragon_Vale_Fields::TARGET_PLAYER_ID].numberLong();
	detail.is_rob_enter_dragon_vale = (res[Dragon_Vale_Fields::IS_ROB_ENTER_DRAGON_VALE].numberInt() == 1) ? true: false;
	detail.uuid = res[Dragon_Vale_Fields::UUID].numberLong();
	detail.is_gold_mine_max_storage = (res[Dragon_Vale_Fields::IS_GOLD_MINE_MAX_STORAGE].numberInt() == 1) ? true : false;
	detail.is_moon_well_max_storage = (res[Dragon_Vale_Fields::IS_MOON_WELL_MAX_STORAGE].numberInt() == 1) ? true : false;
	detail.is_totem_max_storage = (res[Dragon_Vale_Fields::IS_TOTEM_MAX_STORAGE].numberInt() == 1) ? true : false;
	detail.build_gold_mine_num = (uint8_t)res[Dragon_Vale_Fields::BUILD_GOLD_MINE_NUM].numberInt();
	detail.build_moon_well_num = (uint8_t)res[Dragon_Vale_Fields::BUILD_MOON_WELL_NUM].numberInt();
	detail.build_totem_num = (uint8_t)res[Dragon_Vale_Fields::BUILD_TOTEM_NUM].numberInt();
	detail.build_queue_num = (uint8_t)(res[Dragon_Vale_Fields::BUILD_QUEUE_NUM].numberInt()) ;
	detail.all_open_build_queue_num = (uint8_t)(res[Dragon_Vale_Fields::ALL_OPEN_BUILD_QUEUE_NUM].numberInt()) ;
	detail.is_dragon_born = (res[Dragon_Vale_Fields::IS_DRAGON_BORN].numberInt() == 1) ? true : false ;
	detail.begin_born_time.sec((long long int)res[Dragon_Vale_Fields::BEGIN_BORN_TIME_SEC].numberLong());
	detail.begin_play_dragon_interval_time.sec((long long int)res[Dragon_Vale_Fields::BEGIN_PLAY_DRAGON_INTERVAL_TIME_SEC].numberLong());
	detail.begin_down_feeling_time.sec((long long int)res[Dragon_Vale_Fields::BEGIN_DOWN_FEELING_TIME_SEC].numberLong());
	detail.is_vip_down_to_zero = (res[Dragon_Vale_Fields::IS_VIP_DOWN_TO_ZERO].numberInt() == 1) ? true : false;
	detail.matched_num = (uint32_t)res[Dragon_Vale_Fields::MATCHED_NUM].numberInt();
	detail.begin_refresh_thief_time.sec((long long int)res[Dragon_Vale_Fields::BEGIN_REFRESH_THIEF_TIME_SEC].numberLong());
	detail.star_num = (uint8_t)res[Dragon_Vale_Fields::STAR_NUM].numberInt();
	detail.award_foot_num = (uint8_t)res[Dragon_Vale_Fields::AWARD_FOOT_NUM].numberInt();
	detail.is_special = res[Dragon_Vale_Fields::IS_SPECIAL].numberInt() ? true : false;
	detail.is_yesterday_login = (res[Dragon_Vale_Fields::IS_YESTERDAY_LOGIN].numberInt() == 1) ? true : false;
	detail.dragon_heart_num = (uint8_t)res[Dragon_Vale_Fields::DRAGON_HEART_NUM].numberInt();
	detail.is_rob_protect = (res[Dragon_Vale_Fields::IS_ROB_PROTECT].numberInt() == 1) ? true : false;
	detail.begin_rob_protect_time.sec((long long int)res[Dragon_Vale_Fields::BEGIN_ROB_PROTECT_TIME_SEC].numberLong());
	detail.is_first_enter_after_robbed = (res[Dragon_Vale_Fields::IS_FIRST_ENTER_AFTER_ROBBED].numberInt() == 1) ? true : false;
	detail.is_in_robbed_dragon_vale = (res[Dragon_Vale_Fields::IS_IN_ROBBED_DRAGON_VALE].numberInt() == 1) ? true : false;
	detail.begin_in_robbed_dragon_vale_time.sec((long long int)res[Dragon_Vale_Fields::BEGIN_IN_ROBBED_DRAGON_VALE_TIME_SEC].numberLong());
	detail.is_dragon_vale_open = (res[Dragon_Vale_Fields::IS_DRAGON_VALE_OPEN].numberInt() == 1) ? true : false;
	detail.begin_no_enter_time.sec((long long int)res[Dragon_Vale_Fields::BEGIN_NO_ENTER_TIME].numberLong());
	detail.left_time = (uint32_t)res[Dragon_Vale_Fields::LEFT_TIME].numberInt();
	detail.play_dragon_num = (uint32_t)res[Dragon_Vale_Fields::PLAY_DRAGON_NUM].numberInt();
	detail.drive_thief_num = (uint32_t)res[Dragon_Vale_Fields::DRIVE_THIEF_NUM].numberInt();

	//building list
	BSONObj building_list = res.getObjectField(Dragon_Vale_Fields::BUILDING_LIST.c_str());
	BSONObjIterator iter_building(building_list);
	while (iter_building.more()) {
		BSONObj obj = iter_building.next().embeddedObject();
		DragonBuildingInfo dragon_building_info;
		dragon_building_info.reset();
		dragon_building_info.id = (uint32_t)obj[Dragon_Vale_Fields::Building::ID].numberInt();
		dragon_building_info.type = (uint8_t)obj[Dragon_Vale_Fields::Building::TYPE].numberInt();
		dragon_building_info.level = (uint16_t)obj[Dragon_Vale_Fields::Building::LEVEL].numberInt();
		dragon_building_info.status = (uint8_t)obj[Dragon_Vale_Fields::Building::STATUS].numberInt();
		dragon_building_info.production = (uint32_t)obj[Dragon_Vale_Fields::Building::PRODUCTION].numberInt();
		dragon_building_info.is_gain = (obj[Dragon_Vale_Fields::Building::IS_GAIN].numberInt()  == 1) ? true: false;
		dragon_building_info.begin_upgrade_time.sec((long long int)obj[Dragon_Vale_Fields::Building::BEGIN_UPGRADE_TIME_SEC].numberLong());
		dragon_building_info.begin_upgrade_time.usec((long long int)obj[Dragon_Vale_Fields::Building::BEGIN_UPGRADE_TIME_USEC].numberLong());
		dragon_building_info.last_output_production_time.sec((long long int)obj[Dragon_Vale_Fields::Building::LAST_OUTPUT_PRODUCTION_TIME_SEC].numberLong());
		dragon_building_info.last_output_production_time.usec((long long int)obj[Dragon_Vale_Fields::Building::LAST_OUTPUT_PRODUCTION_TIME_USEC].numberLong());
		dragon_building_info.order = (uint8_t)obj[Dragon_Vale_Fields::Building::ORDER].numberInt();
		dragon_building_info.consume_type = (uint8_t)obj[Dragon_Vale_Fields::Building::CONSUME_TYPE].numberInt();

		uint64_t key = create_key_of_Dragon_Building_Info(dragon_building_info.id, dragon_building_info.type);
		detail.dragon_building_info_map.insert(std::make_pair(key, dragon_building_info));
	}

	//cacel create building set
	{
		BSONObj cancel_create_building_list = res.getObjectField(Dragon_Vale_Fields::CANCEL_CREATE_BUILDING_LIST.c_str());
		BSONObjIterator iter_cancel_building(cancel_create_building_list);
		while(iter_cancel_building.more()) {
			BSONObj obj = iter_cancel_building.next().embeddedObject();
			uint8_t order = (uint8_t)obj[Dragon_Vale_Fields::Cancel_Create_Building::ORDER].numberInt();
			detail.cancel_create_building_set.insert(order);
		}
	}


	//dragon list
	BSONObj dragon_list = res.getObjectField(Dragon_Vale_Fields::DRAGON_LIST.c_str());
	BSONObjIterator iter_dragon(dragon_list);
	while (iter_dragon.more()) {
		BSONObj obj = iter_dragon.next().embeddedObject();
		detail.dragon_info.progress = (uint8_t)obj[Dragon_Vale_Fields::Dragon::PROGRESS].numberInt();
		detail.dragon_info.feeling = (uint8_t)obj[Dragon_Vale_Fields::Dragon::FEELING].numberInt();
		detail.dragon_info.power = (uint32_t)obj[Dragon_Vale_Fields::Dragon::POWER].numberInt();
		detail.dragon_info.armor = (uint32_t)obj[Dragon_Vale_Fields::Dragon::ARMOR].numberInt();
		detail.dragon_info.stamina = (uint32_t)obj[Dragon_Vale_Fields::Dragon::STAMINA].numberInt();
		detail.dragon_info.power_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::POWER_LEVEL].numberInt();
		detail.dragon_info.armor_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::ARMOR_LEVEL].numberInt();
		detail.dragon_info.stamina_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::STAMINA_LEVEL].numberInt();
		detail.dragon_info.hero_power = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_POWER].numberInt();
		detail.dragon_info.hero_armor = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_ARMOR].numberInt();
		detail.dragon_info.hero_stamina = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_STAMINA].numberInt();
		detail.dragon_info.hero_power_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_POWER_LEVEL].numberInt();
		detail.dragon_info.hero_armor_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_ARMOR_LEVEL].numberInt();
		detail.dragon_info.hero_stamina_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_STAMINA_LEVEL].numberInt();
		detail.dragon_info.is_play_dragon = (obj[Dragon_Vale_Fields::Dragon::IS_PLAY_DRAGON].numberInt() == 1) ? true : false;
		detail.dragon_info.energy = (uint32_t)obj[Dragon_Vale_Fields::Dragon::ENERGY].numberInt();
		detail.dragon_info.crit = (uint32_t)obj[Dragon_Vale_Fields::Dragon::CRIT].numberInt();
		detail.dragon_info.prevent_crit = (uint32_t)obj[Dragon_Vale_Fields::Dragon::PREVENT_CRIT].numberInt();
		detail.dragon_info.crit_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::CRIT_LEVEL].numberInt();
		detail.dragon_info.prevent_crit_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::PREVENT_CRIT_LEVEL].numberInt();
		detail.dragon_info.hero_crit = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_CRIT].numberInt();
		detail.dragon_info.hero_prevent_crit = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_PREVENT_CRIT].numberInt();
		detail.dragon_info.hero_crit_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_CRIT_LEVEL].numberInt();
		detail.dragon_info.hero_prevent_crit_level = (uint32_t)obj[Dragon_Vale_Fields::Dragon::HERO_PREVETN_CRIT_LEVEL].numberInt();
	}

	//thief list
	BSONObj thief_list = res.getObjectField(Dragon_Vale_Fields::THIEF_LIST.c_str());
	BSONObjIterator iter_thief(thief_list);
	while (iter_thief.more()) {
		BSONObj obj = iter_thief.next().embeddedObject();
		int64_t key = (int64_t)obj[Dragon_Vale_Fields::Thief::CREATER].numberLong();
		int64_t value = (int64_t)obj[Dragon_Vale_Fields::Thief::ID].numberLong();
		detail.thief_id_quality_map.insert(std::make_pair(key, value));
	}

	//foot info list
	BSONObj foot_info_list = res.getObjectField(Dragon_Vale_Fields::FOOT_INFO_LIST.c_str());
	BSONObjIterator iter_foot_info(foot_info_list);
	while(iter_foot_info.more()) {
		BSONObj obj = iter_foot_info.next().embeddedObject();
		Foot_Info foot_info;
		int64_t id = (int64_t)obj[Dragon_Vale_Fields::Foot_Info::ID].numberLong();
		foot_info.sex = (uint8_t)obj[Dragon_Vale_Fields::Foot_Info::SEX].numberInt();
		foot_info.name = obj[Dragon_Vale_Fields::Foot_Info::NAME].str();
		detail.foot_info_map.insert(std::make_pair(id, foot_info));
	}

	//foot id list
	BSONObj foot_id_list = res.getObjectField(Dragon_Vale_Fields::FOOT_ID_LIST.c_str());
	BSONObjIterator iter_foot_id(foot_id_list);
	while(iter_foot_id.more()) {
		BSONObj obj = iter_foot_id.next().embeddedObject();
		int64_t id = (int64_t)obj[Dragon_Vale_Fields::Foot_ID::ID].numberLong();
		detail.foot_id_set.insert(id);
	}

	//notice board list
	BSONObj notice_board_list = res.getObjectField(Dragon_Vale_Fields::NOTICE_BOARD_LIST.c_str());
	BSONObjIterator iter_notice_board(notice_board_list);
	while(iter_notice_board.more()) {
		BSONObj obj = iter_notice_board.next().embeddedObject();
		Notice_Board notice_board;
		notice_board.type = (uint8_t)obj[Dragon_Vale_Fields::Notice_Board::TYPE].numberInt();
		notice_board.time =  (int32_t)obj[Dragon_Vale_Fields::Notice_Board::TIME_SEC].numberLong();
		notice_board.host = obj[Dragon_Vale_Fields::Notice_Board::HOST].str();
		notice_board.target = obj[Dragon_Vale_Fields::Notice_Board::TARGET].str();
		notice_board.dragon_soul = (uint32_t)obj[Dragon_Vale_Fields::Notice_Board::DRAGON_SOUL].numberInt();
		notice_board.coin = (uint32_t)obj[Dragon_Vale_Fields::Notice_Board::COIN].numberInt();
		notice_board.soul = (uint32_t)obj[Dragon_Vale_Fields::Notice_Board::SOUL].numberInt();
		notice_board.is_get_dragon_heart = (uint8_t)obj[Dragon_Vale_Fields::Notice_Board::IS_GET_DRAGON_HEART].numberInt();
		notice_board.param1 = (uint8_t)obj[Dragon_Vale_Fields::Notice_Board::PARAM1].numberInt();
		detail.notice_board_list.push_back(notice_board);
	}

	return 0;
}

int DB_Operator::save_dragon_vale_detail(Dragon_Vale_Detail &detail) {
	//building_list
    vector<BSONObj> building_list;
	for(DragonBuildingInfoMap::iterator it = detail.dragon_building_info_map.begin();
			it != detail.dragon_building_info_map.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Dragon_Vale_Fields::Building::ID << (it->second.id)
			<< Dragon_Vale_Fields::Building::TYPE << (it->second.type)
			<< Dragon_Vale_Fields::Building::LEVEL << (it->second.level)
			<< Dragon_Vale_Fields::Building::STATUS << (it->second.status)
			<< Dragon_Vale_Fields::Building::PRODUCTION << (it->second.production)
			<< Dragon_Vale_Fields::Building::IS_GAIN << (it->second.is_gain ? 1 : 0)
			<< Dragon_Vale_Fields::Building::BEGIN_UPGRADE_TIME_SEC << (long long int)(it->second.begin_upgrade_time.sec())
			<< Dragon_Vale_Fields::Building::BEGIN_UPGRADE_TIME_USEC << (long long int)(it->second.begin_upgrade_time.usec())
			<< Dragon_Vale_Fields::Building::LAST_OUTPUT_PRODUCTION_TIME_SEC << (long long int)(it->second.last_output_production_time.sec())
			<< Dragon_Vale_Fields::Building::LAST_OUTPUT_PRODUCTION_TIME_USEC << (long long int)(it->second.last_output_production_time.usec())
			<< Dragon_Vale_Fields::Building::ORDER << (it->second.order)
			<< Dragon_Vale_Fields::Building::CONSUME_TYPE << (it->second.consume_type);

		building_list.push_back(building_builder.obj());
	}

	//cancel create building set
	vector<BSONObj> cancel_create_building_list;
	for(Cancel_Create_Building_Set::const_iterator it = detail.cancel_create_building_set.begin();
			it != detail.cancel_create_building_set.end();
			++it) {
		BSONObjBuilder cancel_create_builder;
		cancel_create_builder << Dragon_Vale_Fields::Cancel_Create_Building::ORDER << (*it);
		cancel_create_building_list.push_back(cancel_create_builder.obj());
	}

	//dragon list
	vector<BSONObj> dragon_list;
	BSONObjBuilder dragon_builder;
	dragon_builder << Dragon_Vale_Fields::Dragon::PROGRESS << (detail.dragon_info.progress)
			<< Dragon_Vale_Fields::Dragon::FEELING << (detail.dragon_info.feeling)
			<< Dragon_Vale_Fields::Dragon::POWER << (detail.dragon_info.power)
			<< Dragon_Vale_Fields::Dragon::ARMOR << (detail.dragon_info.armor)
			<< Dragon_Vale_Fields::Dragon::STAMINA << (detail.dragon_info.stamina)
			<< Dragon_Vale_Fields::Dragon::POWER_LEVEL << (detail.dragon_info.power_level)
			<< Dragon_Vale_Fields::Dragon::ARMOR_LEVEL << (detail.dragon_info.armor_level)
			<< Dragon_Vale_Fields::Dragon::STAMINA_LEVEL << (detail.dragon_info.stamina_level)
			<< Dragon_Vale_Fields::Dragon::HERO_POWER << (detail.dragon_info.hero_power)
			<< Dragon_Vale_Fields::Dragon::HERO_ARMOR << (detail.dragon_info.hero_armor)
			<< Dragon_Vale_Fields::Dragon::HERO_STAMINA << (detail.dragon_info.hero_stamina)
			<< Dragon_Vale_Fields::Dragon::HERO_POWER_LEVEL << (detail.dragon_info.hero_power_level)
			<< Dragon_Vale_Fields::Dragon::HERO_ARMOR_LEVEL << (detail.dragon_info.hero_armor_level)
			<< Dragon_Vale_Fields::Dragon::HERO_STAMINA_LEVEL << (detail.dragon_info.hero_stamina_level)
			<< Dragon_Vale_Fields::Dragon::IS_PLAY_DRAGON << (detail.dragon_info.is_play_dragon ? 1 : 0)
			<< Dragon_Vale_Fields::Dragon::ENERGY << (detail.dragon_info.energy)
			<< Dragon_Vale_Fields::Dragon::CRIT <<  (detail.dragon_info.crit)
			<< Dragon_Vale_Fields::Dragon::PREVENT_CRIT << (detail.dragon_info.prevent_crit)
			<< Dragon_Vale_Fields::Dragon::CRIT_LEVEL << (detail.dragon_info.crit_level)
			<< Dragon_Vale_Fields::Dragon::PREVENT_CRIT_LEVEL << (detail.dragon_info.prevent_crit_level)
			<< Dragon_Vale_Fields::Dragon::HERO_CRIT << (detail.dragon_info.hero_crit)
			<< Dragon_Vale_Fields::Dragon::HERO_PREVENT_CRIT << (detail.dragon_info.hero_prevent_crit)
			<< Dragon_Vale_Fields::Dragon::HERO_CRIT_LEVEL << (detail.dragon_info.hero_crit_level)
			<< Dragon_Vale_Fields::Dragon::HERO_PREVETN_CRIT_LEVEL << (detail.dragon_info.hero_prevent_crit_level);

	dragon_list.push_back(dragon_builder.obj());

	//thief list
	vector<BSONObj> thief_list;
	for(Thief_Id_Quality_Map::iterator it = detail.thief_id_quality_map.begin();
				it != detail.thief_id_quality_map.end();
				++it) {
		BSONObjBuilder thief_builder;
		thief_builder << Dragon_Vale_Fields::Thief::CREATER << (long long int)(it->first)
				      << Dragon_Vale_Fields::Thief::ID << (long long int)(it->second);
		thief_list.push_back(thief_builder.obj());
	}

	//foot info list
	vector<BSONObj> foot_info_list;
	for(Foot_Info_Map::const_iterator it = detail.foot_info_map.begin(); it != detail.foot_info_map.end(); ++it) {
		BSONObjBuilder foot_builder;
		foot_builder << Dragon_Vale_Fields::Foot_Info::ID << (long long int)(it->first)
				     << Dragon_Vale_Fields::Foot_Info::SEX << (it->second.sex)
				     << Dragon_Vale_Fields::Foot_Info::NAME << (it->second.name);
		foot_info_list.push_back(foot_builder.obj());
	}

	//foot id list
	vector<BSONObj> foot_id_list;
	for(Foot_Id_Set::const_iterator it = detail.foot_id_set.begin(); it != detail.foot_id_set.end(); ++it) {
		BSONObjBuilder foot_id_builder;
		foot_id_builder << Dragon_Vale_Fields::Foot_ID::ID << (long long int)(*it);
		foot_id_list.push_back(foot_id_builder.obj());
	}

	//notice board list
	vector<BSONObj> notice_board_list;
	for(Notice_Board_List::const_iterator it = detail.notice_board_list.begin();
			it != detail.notice_board_list.end();
			++it) {
		BSONObjBuilder notice_board_builder;
		notice_board_builder << Dragon_Vale_Fields::Notice_Board::TYPE << (it->type)
							 <<	Dragon_Vale_Fields::Notice_Board::TIME_SEC << (it->time)
							 << Dragon_Vale_Fields::Notice_Board::HOST << (it->host)
							 << Dragon_Vale_Fields::Notice_Board::TARGET << (it->target)
							 << Dragon_Vale_Fields::Notice_Board::DRAGON_SOUL << (it->dragon_soul)
							 << Dragon_Vale_Fields::Notice_Board::COIN << (it->coin)
							 << Dragon_Vale_Fields::Notice_Board::SOUL << (it->soul)
							 << Dragon_Vale_Fields::Notice_Board::IS_GET_DRAGON_HEART << (it->is_get_dragon_heart)
							 << Dragon_Vale_Fields::Notice_Board::PARAM1 << (it->param1);
		notice_board_list.push_back(notice_board_builder.obj());
	}

	//total
	BSONObjBuilder total_builder;
	total_builder << Dragon_Vale_Fields::ROLE_ID << (long long int)detail.role_id
			<< Dragon_Vale_Fields::TARGET_PLAYER_ID << (long long int)detail.target_player_id
			<< Dragon_Vale_Fields::IS_ROB_ENTER_DRAGON_VALE << (detail.is_rob_enter_dragon_vale ? 1 :0)
			<< Dragon_Vale_Fields::UUID << (long long int)detail.uuid
			<< Dragon_Vale_Fields::IS_GOLD_MINE_MAX_STORAGE << (detail.is_gold_mine_max_storage ? 1 : 0)
			<< Dragon_Vale_Fields::IS_MOON_WELL_MAX_STORAGE << (detail.is_moon_well_max_storage ? 1 : 0)
			<< Dragon_Vale_Fields::IS_TOTEM_MAX_STORAGE << (detail.is_totem_max_storage ? 1 : 0)
			<< Dragon_Vale_Fields::BUILD_GOLD_MINE_NUM << detail.build_gold_mine_num
			<< Dragon_Vale_Fields::BUILD_MOON_WELL_NUM << detail.build_moon_well_num
			<< Dragon_Vale_Fields::BUILD_TOTEM_NUM << detail.build_totem_num
			<< Dragon_Vale_Fields::BUILD_QUEUE_NUM << detail.build_queue_num
			<< Dragon_Vale_Fields::ALL_OPEN_BUILD_QUEUE_NUM << detail.all_open_build_queue_num
			<< Dragon_Vale_Fields::IS_VIP_DOWN_TO_ZERO << (detail.is_vip_down_to_zero ? 1 : 0)
			<< Dragon_Vale_Fields::MATCHED_NUM << (detail.matched_num)
			<< Dragon_Vale_Fields::IS_DRAGON_BORN << (detail.is_dragon_born ? 1 : 0)
			<< Dragon_Vale_Fields::BEGIN_BORN_TIME_SEC << (long long int)(detail.begin_born_time.sec())
			<< Dragon_Vale_Fields::BEGIN_PLAY_DRAGON_INTERVAL_TIME_SEC
						<< (long long int)(detail.begin_play_dragon_interval_time.sec())
			<< Dragon_Vale_Fields::BEGIN_DOWN_FEELING_TIME_SEC << (long long int)(detail.begin_down_feeling_time.sec())
			<< Dragon_Vale_Fields::BEGIN_REFRESH_THIEF_TIME_SEC << (long long int)(detail.begin_refresh_thief_time.sec())
			<< Dragon_Vale_Fields::STAR_NUM << (detail.star_num)
			<< Dragon_Vale_Fields::AWARD_FOOT_NUM << (detail.award_foot_num)
			<< Dragon_Vale_Fields::IS_SPECIAL << (detail.is_special ? 1 :0)
			<< Dragon_Vale_Fields::IS_YESTERDAY_LOGIN << (detail.is_yesterday_login ? 1 : 0)
			<< Dragon_Vale_Fields::DRAGON_HEART_NUM << (detail.dragon_heart_num)
			<< Dragon_Vale_Fields::IS_ROB_PROTECT << (detail.is_rob_protect ? 1 : 0)
			<< Dragon_Vale_Fields::BEGIN_ROB_PROTECT_TIME_SEC << (long long int)(detail.begin_rob_protect_time.sec())
			<< Dragon_Vale_Fields::IS_FIRST_ENTER_AFTER_ROBBED << (detail.is_first_enter_after_robbed ? 1 : 0)
			<< Dragon_Vale_Fields::IS_IN_ROBBED_DRAGON_VALE << (detail.is_in_robbed_dragon_vale ? 1 : 0)
			<< Dragon_Vale_Fields::BEGIN_IN_ROBBED_DRAGON_VALE_TIME_SEC << (long long int)(detail.begin_in_robbed_dragon_vale_time.sec())
			<< Dragon_Vale_Fields::IS_DRAGON_VALE_OPEN <<(detail.is_dragon_vale_open ? 1 : 0)
			<< Dragon_Vale_Fields::BEGIN_NO_ENTER_TIME << (long long int)(detail.begin_no_enter_time.sec())
			<< Dragon_Vale_Fields::LEFT_TIME << detail.left_time
			<< Dragon_Vale_Fields::PLAY_DRAGON_NUM << detail.play_dragon_num
			<< Dragon_Vale_Fields::DRIVE_THIEF_NUM << detail.drive_thief_num
	        << Dragon_Vale_Fields::BUILDING_LIST << building_list
	        << Dragon_Vale_Fields::CANCEL_CREATE_BUILDING_LIST << cancel_create_building_list
	        << Dragon_Vale_Fields::DRAGON_LIST << dragon_list
	        << Dragon_Vale_Fields::THIEF_LIST << thief_list
	        << Dragon_Vale_Fields::FOOT_INFO_LIST << foot_info_list
	        << Dragon_Vale_Fields::FOOT_ID_LIST << foot_id_list
	        << Dragon_Vale_Fields::NOTICE_BOARD_LIST << notice_board_list;

	MONGO_CONNECTION.update(Dragon_Vale_Fields::COLLECTION, QUERY(Dragon_Vale_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << total_builder.obj()), true);

	return 0;
}

int DB_Operator::save_dragon_vale_is_special(int64_t role_id, bool is_speical) {
	BSONObjBuilder builder;
	builder << Dragon_Vale_Fields::IS_SPECIAL << (is_speical ? 1 : 0);
	MONGO_CONNECTION.update(Dragon_Vale_Fields::COLLECTION, QUERY(Dragon_Vale_Fields::ROLE_ID << (long long int)role_id),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::save_dragon_vale_reset(int64_t role_id, uint32_t matched_num, uint8_t star_num, uint8_t award_foot_num,
		const Foot_Info_Map &foot_info_map, const Foot_Id_Set &foot_id_set, uint8_t dragon_heart_num,
		const uint32_t play_dragon_num, const uint32_t drive_thief_num) {
	vector<BSONObj> foot_info_list;
	for(Foot_Info_Map::const_iterator it = foot_info_map.begin(); it != foot_info_map.end(); ++it) {
		BSONObjBuilder foot_builder;
		foot_builder << Dragon_Vale_Fields::Foot_Info::ID << (long long int)(it->first)
				     << Dragon_Vale_Fields::Foot_Info::SEX << (it->second.sex)
				     << Dragon_Vale_Fields::Foot_Info::NAME << (it->second.name);
		foot_info_list.push_back(foot_builder.obj());
	}

	vector<BSONObj> foot_id_list;
	for(Foot_Id_Set::const_iterator it = foot_id_set.begin(); it != foot_id_set.end(); ++it) {
		BSONObjBuilder foot_id_builder;
		foot_id_builder << Dragon_Vale_Fields::Foot_ID::ID << (long long int)(*it);
		foot_id_list.push_back(foot_id_builder.obj());
	}

	BSONObjBuilder builder;
	builder << Dragon_Vale_Fields::MATCHED_NUM << matched_num
			<< Dragon_Vale_Fields::STAR_NUM << star_num
			<< Dragon_Vale_Fields::AWARD_FOOT_NUM << award_foot_num
			<< Dragon_Vale_Fields::FOOT_INFO_LIST << foot_info_list
			<< Dragon_Vale_Fields::FOOT_ID_LIST << foot_id_list
			<< Dragon_Vale_Fields::DRAGON_HEART_NUM << dragon_heart_num
			<< Dragon_Vale_Fields::PLAY_DRAGON_NUM << play_dragon_num
			<< Dragon_Vale_Fields::DRIVE_THIEF_NUM << drive_thief_num;

	MONGO_CONNECTION.update(Dragon_Vale_Fields::COLLECTION, mongo::Query(), BSON("$set" << builder.obj()), true, true);
	return 0;
}

int DB_Operator::load_achievement_detail(Achievement_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Achievement_Fields::COLLECTION, QUERY(Achievement_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return 0;
	}

	detail.role_id = res[Achievement_Fields::ROLE_ID].numberLong();
	detail.achievement_count = res[Achievement_Fields::ACHIEVEMENT_COUNT].numberLong();

	BSONObj list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_INFO_MAP.c_str());
	BSONObjIterator iter_building(list);
	while (iter_building.more()) {
		BSONObj obj = iter_building.next().embeddedObject();
		Achievement_Info info;
		info.reset();
		info.id = (uint32_t)obj[Achievement_Fields::Achievement_Info::ID].numberInt();
		info.data = (uint8_t)obj[Achievement_Fields::Achievement_Info::DATA].numberInt();
		info.state_count = (uint8_t)obj[Achievement_Fields::Achievement_Info::STATE_COUNT].numberInt();
		info.state_award = (uint16_t)obj[Achievement_Fields::Achievement_Info::STATE_AWARD].numberInt();

		detail.achievement_info_map.insert(std::make_pair(info.id, info));
	}

	int32_t id = 0;
	BSONObj complete_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_COMPLETE_IDS.c_str());
	BSONObjIterator iter_complete(complete_list);
	while (iter_complete.more()) {
		BSONObj obj = iter_complete.next().embeddedObject();
		id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
		detail.complete_ids.insert(id);
	}
	BSONObj suit_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_SUIT_PURPLE_IDS.c_str());
	BSONObjIterator iter_suit(suit_list);
	while (iter_suit.more()) {
		BSONObj obj = iter_suit.next().embeddedObject();
		id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
		detail.suit_purple_ids.insert(id);
	}
	BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_SUIT_GOLD_IDS.c_str());
	BSONObjIterator iter_gold_suit(suit_gold_list);
	while (iter_gold_suit.more()) {
		BSONObj obj = iter_gold_suit.next().embeddedObject();
		id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
		detail.suit_gold_ids.insert(id);
	}
	detail.rune_lvl_his = res[Achievement_Fields::ACHIEVEMENT_RUNE_LVL_HIS].numberInt();
	detail.unban_lvl_his = res[Achievement_Fields::ACHIEVEMENT_UNBAN_LVL_HIS].numberInt();
	detail.master_lvl_his = res[Achievement_Fields::ACHIEVEMENT_MASTER_LVL_HIS].numberInt();
	detail.force_lvl_his = res[Achievement_Fields::ACHIEVEMENT_FORCE_LVL_HIS].numberInt();
	detail.war_kill_nums_his = res[Achievement_Fields::ACHIEVEMENT_WAR_KILL_NUMS_HIS].numberInt();
	detail.arena_win_nums_his = res[Achievement_Fields::ACHIEVEMENT_ARENA_WIN_NUMS_HIS].numberInt();
	detail.gang_war_win_nums_his = res[Achievement_Fields::ACHIEVEMENT_GANG_WAR_WIN_NUMS_HIS].numberInt();
//	role_id_t uid = 0;
//	{
//		BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_FIRST_LVL_ROLE_ID_HIS.c_str());
//		BSONObjIterator iter_gold_suit(suit_gold_list);
//		while (iter_gold_suit.more()) {
//			BSONObj obj = iter_gold_suit.next().embeddedObject();
//			id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
//			uid = obj[Achievement_Fields::Achievement_Info::DATA].numberLong();
//			detail.first_lvl_role_id_his.insert(std::make_pair(id, uid));
//		}
//	}
//	{
//		BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_FIRST_LVL_CK_HIS.c_str());
//		BSONObjIterator iter_gold_suit(suit_gold_list);
//		while (iter_gold_suit.more()) {
//			BSONObj obj = iter_gold_suit.next().embeddedObject();
//			id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
//			uid = obj[Achievement_Fields::Achievement_Info::DATA].numberLong();
//			detail.first_lvl_ck_his.insert(std::make_pair(id, uid));
//		}
//	}
//	{
//		BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_FIRST_LVL_ZS_HIS.c_str());
//		BSONObjIterator iter_gold_suit(suit_gold_list);
//		while (iter_gold_suit.more()) {
//			BSONObj obj = iter_gold_suit.next().embeddedObject();
//			id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
//			uid = obj[Achievement_Fields::Achievement_Info::DATA].numberLong();
//			detail.first_lvl_zs_his.insert(std::make_pair(id, uid));
//		}
//	}
//	{
//		BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_FIRST_LVL_YSS_HIS.c_str());
//		BSONObjIterator iter_gold_suit(suit_gold_list);
//		while (iter_gold_suit.more()) {
//			BSONObj obj = iter_gold_suit.next().embeddedObject();
//			id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
//			uid = obj[Achievement_Fields::Achievement_Info::DATA].numberLong();
//			detail.first_lvl_yss_his.insert(std::make_pair(id, uid));
//		}
//	}
	detail.kill_monster_nums_his = res[Achievement_Fields::ACHIEVEMENT_KILL_MONSTER_NUMS_HIS].numberInt();
	{
		BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_FB_COMPLETE_NUMS_HIS.c_str());
		BSONObjIterator iter_gold_suit(suit_gold_list);
		while (iter_gold_suit.more()) {
			BSONObj obj = iter_gold_suit.next().embeddedObject();
			id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
			detail.team_fb_complete_nums_his.insert(id);
		}
	}
	{
		BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_HORSE_GET_HIS.c_str());
		BSONObjIterator iter_gold_suit(suit_gold_list);
		while (iter_gold_suit.more()) {
			BSONObj obj = iter_gold_suit.next().embeddedObject();
			id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
			detail.horse_get_his.insert(id);
		}
	}
	detail.secret_shop_buy_nums_his = res[Achievement_Fields::ACHIEVEMENT_SECRET_SHOP_BUY_NUMS_HIS].numberInt();
	detail.dragon_wale_rob_times_his = res[Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_ROB_TIMES_HIS].numberInt();
	detail.dragon_wale_step_foot_times_his = res[Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_STEP_FOOT_TIMES_HIS].numberInt();
	detail.dragon_wale_play_dragon_times_his = res[Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_PLAY_DRAGON_TIMES_HIS].numberInt();
	detail.hero_awake_lvl_his = res[Achievement_Fields::ACHIEVEMENT_HERO_AWAKE_LVL_HIS].numberInt();
	detail.task_complete_nums_his = res[Achievement_Fields::ACHIEVEMENT_TASK_COMPLETE_NUMS_HIS].numberInt();
	detail.dragon_wale_gold_mine_lvl_his = res[Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_GOLD_MINE_LVL_HIS].numberInt();
	detail.dragon_wale_moon_well_lvl_his = res[Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_MOON_WELL_LVL_HIS].numberInt();
	detail.dragon_wale_totem_lvl_his = res[Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_TOTEM_LVL_HIS].numberInt();
	detail.dragon_wale_drive_thief_times_his = res[Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_DRIVE_THIEF_TIMES_HIS].numberInt();
	detail.war_light_win_nums_his = res[Achievement_Fields::ACHIEVEMENT_WAR_LIGHT_WIN_NUMS_HIS].numberInt();
	detail.war_dark_win_nums_his = res[Achievement_Fields::ACHIEVEMENT_WAR_DRAK_WIN_NUMS_HIS].numberInt();
	if (res.hasField(Achievement_Fields::ACHIEVEMENT_TASK_LIMIT_TIME_HIS.c_str())) {
		detail.task_limit_time_his = res[Achievement_Fields::ACHIEVEMENT_TASK_LIMIT_TIME_HIS].numberInt();
	}
	{
		BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_KILL_BOSS_HIS.c_str());
		BSONObjIterator iter_gold_suit(suit_gold_list);
		while (iter_gold_suit.more()) {
			BSONObj obj = iter_gold_suit.next().embeddedObject();
			id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
			detail.kill_boss_his.insert(id);
		}
	}
	detail.dragon_wale_keep_level_his = res[Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_KEEP_LEVEL_HIS].numberInt();
	detail.rune_stone_lvl_his = res[Achievement_Fields::ACHIEVEMENT_RUNE_STONE_LVL_HIS].numberInt();
	detail.arena_lvl_his = res[Achievement_Fields::ACHIEVEMENT_ARENA_LVL_HIS].numberInt();
	detail.first_boss_against = res[Achievement_Fields::ACHIEVEMENT_FIRST_BOSS_AGAINST].numberInt();
	{
		BSONObj suit_gold_list = res.getObjectField(Achievement_Fields::ACHIEVEMENT_HORSE_GET_HIS.c_str());
		BSONObjIterator iter_gold_suit(suit_gold_list);
		while (iter_gold_suit.more()) {
			BSONObj obj = iter_gold_suit.next().embeddedObject();
			id = obj[Achievement_Fields::Achievement_Info::ID].numberInt();
			detail.complete_suit.insert(id);
		}
	}
	return 0;
}

int DB_Operator::save_achievement_detail(Achievement_Detail &detail) {
    vector<BSONObj> list;
    vector<BSONObj> complete_list;
    vector<BSONObj> suit_purple_list;
    vector<BSONObj> suit_gold_list;
//    vector<BSONObj> first_lvl_role_id_his;
//    vector<BSONObj> first_lvl_ck_his;
//    vector<BSONObj> first_lvl_zs_his;
//    vector<BSONObj> first_lvl_yss_his;
    vector<BSONObj> fb_complete_nums_his;
    vector<BSONObj> horse_get_his;
    vector<BSONObj> kill_boss_his;
    vector<BSONObj> complete_suit;
	for(Achievement_Info_Map::iterator it = detail.achievement_info_map.begin();
			it != detail.achievement_info_map.end(); ++it) {
		if (it->second.data == 0) {
			continue;
		}
		BSONObjBuilder building_builder;
		building_builder << Achievement_Fields::Achievement_Info::ID << (it->second.id)
			<< Achievement_Fields::Achievement_Info::DATA << (it->second.data)
			<< Achievement_Fields::Achievement_Info::STATE_COUNT << (it->second.state_count)
			<< Achievement_Fields::Achievement_Info::STATE_AWARD << (it->second.state_award);

		list.push_back(building_builder.obj());
	}
	for(Int_Hash_Set::iterator it = detail.complete_ids.begin();
			it != detail.complete_ids.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Achievement_Fields::Achievement_Info::ID << (*it);

		complete_list.push_back(building_builder.obj());
	}
	for(Int_Hash_Set::iterator it = detail.suit_purple_ids.begin();
			it != detail.suit_purple_ids.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Achievement_Fields::Achievement_Info::ID << (*it);

		suit_purple_list.push_back(building_builder.obj());
	}
	for(Int_Hash_Set::iterator it = detail.suit_gold_ids.begin();
			it != detail.suit_gold_ids.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Achievement_Fields::Achievement_Info::ID << (*it);

		suit_gold_list.push_back(building_builder.obj());
	}
//	for(Int_Uid::iterator it = detail.first_lvl_role_id_his.begin();
//			it != detail.first_lvl_role_id_his.end(); ++it) {
//		BSONObjBuilder building_builder;
//		building_builder << Achievement_Fields::Achievement_Info::ID << it->first
//				<< Achievement_Fields::Achievement_Info::DATA << (long long int)it->second;
//
//		first_lvl_role_id_his.push_back(building_builder.obj());
//	}
//	for(Int_Uid::iterator it = detail.first_lvl_ck_his.begin();
//			it != detail.first_lvl_ck_his.end(); ++it) {
//		BSONObjBuilder building_builder;
//		building_builder << Achievement_Fields::Achievement_Info::ID << it->first
//				<< Achievement_Fields::Achievement_Info::DATA << (long long int)it->second;
//
//		first_lvl_ck_his.push_back(building_builder.obj());
//	}
//	for(Int_Uid::iterator it = detail.first_lvl_zs_his.begin();
//			it != detail.first_lvl_zs_his.end(); ++it) {
//		BSONObjBuilder building_builder;
//		building_builder << Achievement_Fields::Achievement_Info::ID << it->first
//				<< Achievement_Fields::Achievement_Info::DATA << (long long int)it->second;
//
//		first_lvl_zs_his.push_back(building_builder.obj());
//	}
//	for(Int_Uid::iterator it = detail.first_lvl_yss_his.begin();
//			it != detail.first_lvl_yss_his.end(); ++it) {
//		BSONObjBuilder building_builder;
//		building_builder << Achievement_Fields::Achievement_Info::ID << it->first
//				<< Achievement_Fields::Achievement_Info::DATA << (long long int)it->second;
//
//		first_lvl_yss_his.push_back(building_builder.obj());
//	}
	for(Int_Hash_Set::iterator it = detail.team_fb_complete_nums_his.begin();
			it != detail.team_fb_complete_nums_his.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Achievement_Fields::Achievement_Info::ID << (*it);

		fb_complete_nums_his.push_back(building_builder.obj());
	}
	for(Int_Hash_Set::iterator it = detail.horse_get_his.begin();
			it != detail.horse_get_his.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Achievement_Fields::Achievement_Info::ID << (*it);

		horse_get_his.push_back(building_builder.obj());
	}
	for(Int_Hash_Set::iterator it = detail.kill_boss_his.begin();
			it != detail.kill_boss_his.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Achievement_Fields::Achievement_Info::ID << (*it);

		kill_boss_his.push_back(building_builder.obj());
	}
	for(Int_Hash_Set::iterator it = detail.complete_suit.begin();
			it != detail.complete_suit.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Achievement_Fields::Achievement_Info::ID << (*it);

		complete_suit.push_back(building_builder.obj());
	}

	BSONObjBuilder builder;
	builder << Achievement_Fields::ROLE_ID << (long long int)detail.role_id
			<< Achievement_Fields::ACHIEVEMENT_COUNT << (long long int)detail.achievement_count;
	if (!list.empty()) {
		builder << Achievement_Fields::ACHIEVEMENT_INFO_MAP << list;
	}
	if (!complete_list.empty()) {
		builder << Achievement_Fields::ACHIEVEMENT_COMPLETE_IDS << complete_list;
	}
	if (!suit_purple_list.empty()) {
		builder << Achievement_Fields::ACHIEVEMENT_SUIT_PURPLE_IDS << suit_purple_list;
	}
	if (!suit_gold_list.empty()) {
		builder << Achievement_Fields::ACHIEVEMENT_SUIT_GOLD_IDS << suit_gold_list;
	}
	if (detail.rune_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_RUNE_LVL_HIS << detail.rune_lvl_his;
	}
	if (detail.unban_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_UNBAN_LVL_HIS << detail.unban_lvl_his;
	}
	if (detail.master_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_MASTER_LVL_HIS << detail.master_lvl_his;
	}
	if (detail.force_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_FORCE_LVL_HIS << detail.force_lvl_his;
	}
	if (detail.war_kill_nums_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_WAR_KILL_NUMS_HIS << detail.war_kill_nums_his;
	}
	if (detail.arena_win_nums_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_ARENA_WIN_NUMS_HIS << detail.arena_win_nums_his;
	}
	if (detail.gang_war_win_nums_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_GANG_WAR_WIN_NUMS_HIS << detail.gang_war_win_nums_his;
	}
	if (detail.kill_monster_nums_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_KILL_MONSTER_NUMS_HIS << detail.kill_monster_nums_his;
	}
	if (!fb_complete_nums_his.empty()) {
		builder << Achievement_Fields::ACHIEVEMENT_FB_COMPLETE_NUMS_HIS << fb_complete_nums_his;
	}
	if (!horse_get_his.empty()) {
		builder << Achievement_Fields::ACHIEVEMENT_HORSE_GET_HIS << horse_get_his;
	}
	if (detail.secret_shop_buy_nums_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_SECRET_SHOP_BUY_NUMS_HIS << detail.secret_shop_buy_nums_his;
	}
	if (detail.dragon_wale_rob_times_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_ROB_TIMES_HIS << detail.dragon_wale_rob_times_his;
	}
	if (detail.dragon_wale_step_foot_times_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_STEP_FOOT_TIMES_HIS << detail.dragon_wale_step_foot_times_his;
	}
	if (detail.dragon_wale_play_dragon_times_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_PLAY_DRAGON_TIMES_HIS << detail.dragon_wale_play_dragon_times_his;
	}
	if (detail.hero_awake_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_HERO_AWAKE_LVL_HIS << detail.hero_awake_lvl_his;
	}
	if (detail.task_complete_nums_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_TASK_COMPLETE_NUMS_HIS << detail.task_complete_nums_his;
	}
	if (detail.dragon_wale_gold_mine_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_GOLD_MINE_LVL_HIS << detail.dragon_wale_gold_mine_lvl_his;
	}
	if (detail.dragon_wale_moon_well_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_MOON_WELL_LVL_HIS << detail.dragon_wale_moon_well_lvl_his;
	}
	if (detail.dragon_wale_totem_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_TOTEM_LVL_HIS << detail.dragon_wale_totem_lvl_his;
	}
	if (detail.dragon_wale_drive_thief_times_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_DRIVE_THIEF_TIMES_HIS << detail.dragon_wale_drive_thief_times_his;
	}
	if (detail.war_light_win_nums_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_WAR_LIGHT_WIN_NUMS_HIS << detail.war_light_win_nums_his;
	}
	if (detail.war_dark_win_nums_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_WAR_DRAK_WIN_NUMS_HIS << detail.war_dark_win_nums_his;
	}
	if (detail.task_limit_time_his != -1) {
		builder << Achievement_Fields::ACHIEVEMENT_TASK_LIMIT_TIME_HIS << detail.task_limit_time_his;
	}
	if (!kill_boss_his.empty()) {
		builder << Achievement_Fields::ACHIEVEMENT_KILL_BOSS_HIS << kill_boss_his;
	}
	if (detail.dragon_wale_keep_level_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_DRAGON_WALE_KEEP_LEVEL_HIS << detail.dragon_wale_keep_level_his;
	}
	if (detail.rune_stone_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_RUNE_STONE_LVL_HIS << detail.rune_stone_lvl_his;
	}
	if (detail.arena_lvl_his != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_ARENA_LVL_HIS << detail.arena_lvl_his;
	}
	if (detail.first_boss_against != 0) {
		builder << Achievement_Fields::ACHIEVEMENT_FIRST_BOSS_AGAINST << detail.first_boss_against;
	}
	if (!complete_suit.empty()) {
		builder << Achievement_Fields::ACHIEVEMENT_COMPLETE_SUIT << complete_suit;
	}

	MONGO_CONNECTION.update(Achievement_Fields::COLLECTION, QUERY(Achievement_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_title_detail(Title_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Title_Fields::COLLECTION,
				QUERY(Title_Fields::ROLE_ID << (long long int)detail.role_id));

	if (res.isEmpty()) {
		return -1;
	}

	Title_Detail_Info_Detail info;
	BSONObjIterator iter_title(res.getObjectField(Title_Fields::TITLE_INFO.c_str()));
	while(iter_title.more()) {
		BSONObj obj = iter_title.next().embeddedObject();
		info.reset();
		info.id = obj[Title_Fields::Title_Info::ID].numberInt();
		info.last_time = obj[Title_Fields::Title_Info::LAST_TIME].numberInt();
		info.start_time.sec(obj[Title_Fields::Title_Info::START_TIME].numberLong());
		info.last_end_time.sec(obj[Title_Fields::Title_Info::LAST_END_TIME].numberLong());
		info.id = obj[Title_Fields::Title_Info::ID].numberInt();
		detail.title_set.insert(std::make_pair(info.id, info));
	}

	detail.equip_title = res[Title_Fields::EQUIP_TITLE].numberInt();

	return 0;
}

int DB_Operator::save_title_detail(Title_Detail &detail) {
	std::vector<BSONObj> title_vector;
	for (Title_Detail_Info::const_iterator iter = detail.title_set.begin();
			iter != detail.title_set.end(); ++iter) {
		title_vector.push_back(BSON(Title_Fields::Title_Info::ID << iter->second.id
				<< Title_Fields::Title_Info::LAST_TIME << iter->second.last_time
				<< Title_Fields::Title_Info::START_TIME << (long long int)iter->second.start_time.sec()
				<< Title_Fields::Title_Info::LAST_END_TIME << (long long int)iter->second.last_end_time.sec()));
	}

	BSONObjBuilder builder;
	builder << Title_Fields::TITLE_INFO << title_vector
			<< Title_Fields::EQUIP_TITLE << detail.equip_title;

	MONGO_CONNECTION.update(Title_Fields::COLLECTION, QUERY(Title_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_equiper_detail(Equiper_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Equiper_Fields::COLLECTION,
				QUERY(Equiper_Fields::ROLE_ID << (long long int)detail.role_id));

	if (res.isEmpty()) {
		return -1;
	}

	Master_Part_Info_Detail info;
	BSONObjIterator iter_part(res.getObjectField(Equiper_Fields::MASTER_PART_INFO.c_str()));
	while(iter_part.more()) {
		BSONObj obj = iter_part.next().embeddedObject();
		info.reset();
		info.index = obj[Equiper_Fields::Master_Part_Info_Detail::INDEX].numberInt();
		info.level = obj[Equiper_Fields::Master_Part_Info_Detail::LEVEL].numberInt();
		info.luck_value = obj[Equiper_Fields::Master_Part_Info_Detail::LUCK_VALUE].numberLong();
		info.fail_cnt = obj[Equiper_Fields::Master_Part_Info_Detail::FAIL_CNT].numberLong();
		detail.master_part_info.insert(std::make_pair(info.index, info));

	}

	int32_t id = 0;
	BSONObj gem_data_list = res.getObjectField(Equiper_Fields::GEM_ENABLE_INFO.c_str());
	BSONObjIterator iter_gem_data(gem_data_list);
	while (iter_gem_data.more()) {
		BSONObj obj = iter_gem_data.next().embeddedObject();
		id = obj[Equiper_Fields::GEM_ID].numberInt();
		detail.gem_data.insert(id);
	}
	detail.carve_lvl = res[Equiper_Fields::CARVE_LVL].numberInt();
	detail.gem_progress = res[Equiper_Fields::GEM_PROGRESS].numberInt();

	return 0;
}

int DB_Operator::save_equiper_detail(Equiper_Detail &detail) {
	std::vector<BSONObj> part_vector;
	for (Master_Part_Info_Map::const_iterator iter = detail.master_part_info.begin();
			iter != detail.master_part_info.end(); ++iter) {
		part_vector.push_back(BSON(Equiper_Fields::Master_Part_Info_Detail::INDEX << iter->second.index
				<< Equiper_Fields::Master_Part_Info_Detail::LEVEL << iter->second.level
				<< Equiper_Fields::Master_Part_Info_Detail::LUCK_VALUE << iter->second.luck_value
				<< Equiper_Fields::Master_Part_Info_Detail::FAIL_CNT << iter->second.fail_cnt));
	}

	vector<BSONObj> gem_data_list;
	for(Int_Hash_Set::iterator it = detail.gem_data.begin();
			it != detail.gem_data.end(); ++it) {
		BSONObjBuilder building_builder;
		building_builder << Equiper_Fields::GEM_ID << (*it);

		gem_data_list.push_back(building_builder.obj());
	}


	BSONObjBuilder builder;
	builder << Equiper_Fields::MASTER_PART_INFO << part_vector
			<< Equiper_Fields::GEM_ENABLE_INFO << gem_data_list;

	builder << Equiper_Fields::CARVE_LVL << detail.carve_lvl;
	builder << Equiper_Fields::GEM_PROGRESS << detail.gem_progress;

	MONGO_CONNECTION.update(Equiper_Fields::COLLECTION, QUERY(Equiper_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_title_record(Title_Record_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Title_Record_Fields::COLLECTION, mongo::Query());
	if (res.isEmpty())
		return -1;
	detail.gang_war_first_leader = res[Title_Record_Fields::GANG_WAR_FIRST_LEADER].numberLong();

	BSONObjIterator iter_charts_title(res.getObjectField(Title_Record_Fields::CHARTS_TITLE_MAP.c_str()));
	while (iter_charts_title.more()) {
		BSONObj obj = iter_charts_title.next().embeddedObject();

		int charts_type = obj[Title_Record_Fields::CHARTS_TYPE].numberInt();

		Title_Record_Detail::Charts_Info_Vector charts_info_vector;
		charts_info_vector.clear();
		BSONObjIterator iter_charts_info(obj.getObjectField(Title_Record_Fields::CHARTS_INFO.c_str()));
		while(iter_charts_info.more()) {
			BSONObj obj_charts_info = iter_charts_info.next().embeddedObject();
			Title_Record_Detail::Charts_Info charts_info;
			charts_info.role_id = obj_charts_info[Title_Record_Fields::ROLE_ID].numberLong();
			charts_info.title_id = obj_charts_info[Title_Record_Fields::TITLE_ID].numberInt();
			charts_info_vector.push_back(charts_info);
		}
		detail.charts_title_map.insert(std::make_pair(charts_type, charts_info_vector));
	}

	return 0;
}

int DB_Operator::save_title_record(Title_Record_Detail &detail) {
	std::vector<BSONObj> charts_title_vector;
	for (Title_Record_Detail::Charts_Title_Map::iterator iter = detail.charts_title_map.begin();
			iter != detail.charts_title_map.end(); ++iter) {
		std::vector<BSONObj> charts_info_vector;
		for (Title_Record_Detail::Charts_Info_Vector::iterator it = iter->second.begin();
				it != iter->second.end(); ++it) {
			charts_info_vector.push_back(BSON(Title_Record_Fields::ROLE_ID << (long long int)(it->role_id)
				<< Title_Record_Fields::TITLE_ID << it->title_id));
		}
		charts_title_vector.push_back((BSON(Title_Record_Fields::CHARTS_TYPE << iter->first
				<< Title_Record_Fields::CHARTS_INFO << charts_info_vector)));
	}

	BSONObjBuilder builder;
	builder << Title_Record_Fields::CHARTS_TITLE_MAP << charts_title_vector
			<< Title_Record_Fields::GANG_WAR_FIRST_LEADER << (long long int)detail.gang_war_first_leader;

	MONGO_CONNECTION.update(Title_Record_Fields::COLLECTION, mongo::Query(),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_public_detail(Public_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Public_Fields::COLLECTION, QUERY(Public_Fields::ROLE_ID << (long long int)detail.role_id));
	if (! res.isEmpty()) {
		detail.role_id = res[Public_Fields::ROLE_ID].numberLong();
		detail.plot_id = res[Public_Fields::PLOT_ID].valuestrsafe();
		detail.plot_step = res[Public_Fields::PLOT_STEP].numberInt();
		detail.plot_switch = res[Public_Fields::PLOT_SWITCH].numberInt();
		detail.add_up_online_time = res[Public_Fields::ADD_UP_ONLINE_TIME].numberInt();
		detail.fcm = res[Public_Fields::FCM].numberInt();
		detail.war_enter_num = res[Public_Fields::WAR_ENTER_NUM].numberInt();
		detail.war_merit = res[Public_Fields::WAR_MERIT].numberInt();
		detail.merit = res[Public_Fields::MERIT].numberInt();
		detail.war_forbid_time.sec(res[Public_Fields::WAR_FORBID_TIME].numberInt());
		detail.add_campfire = res[Public_Fields::ADD_CAMPFIRE].numberInt();
		detail.campfire_id = res[Public_Fields::CAMPFIRE_ID].numberInt();
		detail.campfire_finish_time = res[Public_Fields::CAMPFIRE_FINISH_TIME].numberLong();
		detail.campfire_rank = res[Public_Fields::CAMPFIRE_RANK].numberInt();

		BSONObj building_list = res.getObjectField(Public_Fields::CAMPFIRE_KILLED_MATCH_MAN.c_str());
		BSONObjIterator iter1(building_list);
		while(iter1.more()) {
			detail.killed_match_man.insert(iter1.next().numberLong());
		}
		detail.campfire_lighter = res[Public_Fields::CAMPFIRE_LIGHTER].numberInt();
		detail.campfire_recieve_stat = res[Public_Fields::CAMPFIRE_RECEIVESTAT].numberInt();
		detail.campfire_rank_rev_stat = res[Public_Fields::CAMPFIRE_RANK_RCV_STAT].numberInt();

		detail.war_result = res[Public_Fields::WAR_RESULT].numberInt();
		detail.war_rank = res[Public_Fields::WAR_RANK].numberInt();
		detail.war_kill_num = res[Public_Fields::WAR_KILL_NUM].numberInt();
		detail.war_coe = res[Public_Fields::WAR_COE].numberInt();
		detail.arena_worship_times = res[Public_Fields::WORSHIPS].numberInt();
		detail.awake_lvl = res[Public_Fields::AWAKE_LVL].numberInt();
		detail.exploit_val = res[Public_Fields::EXPLOIT_VAL].numberInt();
		detail.exploit_lvl = res[Public_Fields::EXPLOIT_LVL].numberInt();
		detail.first_recharge = res[Public_Fields::FIRST_RECHARGE].numberInt();
		detail.two_recharge = res[Public_Fields::TWO_RECHARGE].numberInt();
		detail.limit_time_recharge = res[Public_Fields::LIMIT_TIME_RECHARGE].numberInt();
		detail.last_two_recharge_stime.sec(res[Public_Fields::LAST_TWO_RECHARGE_STIME].numberLong());
		detail.last_limit_time_recharge_stime.sec(res[Public_Fields::LAST_LIMIT_TIME_RECHARGE_STIME].numberLong());
		detail.recharge_money = res[Public_Fields::RECHARGE_MONEY].numberInt();
		detail.recharge_money_his = res[Public_Fields::RECHARGE_MONEY_HIS].numberInt();
		detail.last_recharge_rebate_stime.sec(res[Public_Fields::RECHARGE_LSTIME].numberLong());
		detail.start_frist_recharge_time.sec(res[Public_Fields::START_FIRST_RECHARGE_TIME].numberLong());
		detail.notice_id = (uint32_t)res[Public_Fields::NOTICE_ID].numberInt();
		detail.is_played_effect = (res[Public_Fields::IS_PLAYED_EFFECT].numberInt() == 1) ? true : false;
		detail.is_show =  (res[Public_Fields::IS_SHOW].numberInt() == 1) ? true : false;
		detail.is_receive_old_player_gift =  res[Public_Fields::IS_ROPG].booleanSafe();
		detail.is_receive_inv_ones =  res[Public_Fields::IS_RECEIVE_INV].booleanSafe();
		detail.is_receive_like_reward =  res[Public_Fields::IS_RECEIVE_LIKE].booleanSafe();
		detail.today_rec_bind_gold =  res[Public_Fields::TODAY_REC_BG].numberInt();
		detail.inv_time.sec(res[Public_Fields::INV_TIME].numberLong());
		detail.rec_inv_time.sec(res[Public_Fields::REC_INV_TIME].numberLong());
		detail.is_magic_wing_over_time = (res[Public_Fields::IS_MAGIC_WING_OVER_TIME].numberInt() == 1) ? true : false;
		detail.is_magic_wing_readed = (res[Public_Fields::IS_MAGIC_WING_READED].numberInt() == 1) ? true : false;
		detail.vip_over_time.sec(res[Public_Fields::VIP_OVER_TIME].numberLong());
		detail.is_vip_readed = (res[Public_Fields::IS_VIP_READED].numberInt() == 1) ? true : false;
		detail.genius_over_time.sec(res[Public_Fields::GENIUS_OVER_TIME].numberLong());
		detail.is_genius_readed = (res[Public_Fields::IS_GENIUS_READED].numberInt() == 1) ? true : false;
		detail.task_guide_id = (uint32_t)res[Public_Fields::TASK_GUIDE_ID].numberInt();
		detail.vip_upgrade_cards_ = res[Public_Fields::VIP_UPGRADE_CARDS].numberInt();
		detail.friendship_accumulate = (uint32_t)res[Package_Fields::Money::FRIENDSHIP].numberInt();

		if(res.hasField(Public_Fields::RECHARGE_FEEDBACK_LIST.c_str())) {
			BSONObjIterator iter(res.getObjectField(Public_Fields::RECHARGE_FEEDBACK_LIST.c_str()));
			while (iter.more()) {
				BSONElement obj = iter.next();
				detail.recharge_feedback_receive_list.insert(obj.numberInt());
			}
		}

		if(res.hasField(Public_Fields::ANS_QID.c_str())) {
			BSONObjIterator iter(res.getObjectField(Public_Fields::ANS_QID.c_str()));
			while (iter.more()) {
				BSONElement obj = iter.next();
				detail.ans_qid.insert(obj.numberInt());
			}
		}

    	if (res.hasField(Role_Fields::LOCAL_HIS.c_str())) {
			BSONObjIterator scene_trace_iter(res.getObjectField(Role_Fields::LOCAL_HIS.c_str()));
			Location location;
			while (scene_trace_iter.more()) {
				BSONElement trace_obj = scene_trace_iter.next();
				location.scene_id = trace_obj[Role_Fields::Location::SCENE_ID].numberInt();
				location.scene_order = trace_obj[Role_Fields::Location::SCENE_ORDER].numberInt();
				location.scene_layer = trace_obj[Role_Fields::Location::SCENE_LAYER].numberInt();
				location.coord.x = trace_obj[Role_Fields::Location::COORD_X].numberInt();
				location.coord.y = trace_obj[Role_Fields::Location::COORD_Y].numberInt();
				detail.local_his.push_back(location);
			}
    	}

		std::string plot_id = "";
		int num = 0;
		BSONObjIterator plot_info_iter(res.getObjectField(Public_Fields::PLOT_INFO.c_str()));
		while (plot_info_iter.more()) {
			BSONElement obj = plot_info_iter.next();
			plot_id = obj[Public_Fields::PLOT_ID].valuestrsafe();
			num = obj[Public_Fields::PLOT_NUM].numberInt();
			detail.plot_info.insert(String_Int_Map::value_type(plot_id, num));
		}

		//guide list
		const BSONObj &guide_map = res.getObjectField(Public_Fields::GUIDE_LIST.c_str());
		BSONObjIterator iter_guide(guide_map);
		while (iter_guide.more()) {
			BSONObj obj = iter_guide.next().embeddedObject();
			uint16_t guide_id = (uint16_t)obj[Public_Fields::Guide::GUIDE_ID].numberInt();
			uint8_t used_num = (uint8_t)obj[Public_Fields::Guide::USED_NUM].numberInt();
			detail.guide_num_map.insert(std::make_pair(guide_id, used_num));
		}

		//guide step list
		{
			const BSONObj &guide_step_map = res.getObjectField(Public_Fields::GUIDE_STEP_LIST.c_str());
			BSONObjIterator iter_guide_step(guide_step_map);
			while(iter_guide_step.more()) {
				BSONObj obj = iter_guide_step.next().embeddedObject();
				uint16_t guide_id = (uint16_t)obj[Public_Fields::Guide_Step::GUIDE_ID].numberInt();
				uint8_t step = (uint8_t)obj[Public_Fields::Guide_Step::STEP].numberInt();
				uint8_t num = (uint8_t)obj[Public_Fields::Guide_Step::NUM].numberInt();
				Guide_Step_Info guide_step_info;
				guide_step_info.step = step;
				guide_step_info.num = num;
				detail.guide_step_map.insert(std::make_pair(guide_id, guide_step_info));
			}
		}

		//played plot set
		const BSONObj &played_plot_set = res.getObjectField(Public_Fields::PLAYED_PLOT_LIST.c_str());
		BSONObjIterator iter_played_plot(played_plot_set);
		while(iter_played_plot.more()) {
			BSONObj obj = iter_played_plot.next().embeddedObject();
			uint32_t plot_id = (uint32_t)obj[Public_Fields::Played_Plot::PLOT_ID].numberInt();
			detail.played_plot_set.insert(plot_id);
		}

		//sys cfg
		const BSONObj &sys_cfg_list = res.getObjectField(Public_Fields::SYS_CFG.c_str());
		BSONObjIterator sys_cfg_iter(sys_cfg_list);
		while (sys_cfg_iter.more()) {
			BSONObj sys_cfg = sys_cfg_iter.next().embeddedObject();
			detail.is_total_open = (uint8_t)sys_cfg[Public_Fields::SysCfg::IS_TOTAL_OPEN].numberInt();
			detail.is_open_music = (uint8_t)sys_cfg[Public_Fields::SysCfg::IS_OPEN_MUSIC].numberInt();
			detail.music_num = (uint8_t)sys_cfg[Public_Fields::SysCfg::MUSIC_NUM].numberInt();
			detail.is_open_voice = (uint8_t)sys_cfg[Public_Fields::SysCfg::IS_OPEN_VOICE].numberInt();
			detail.voice_num = (uint8_t)sys_cfg[Public_Fields::SysCfg::VOICE_NUM].numberInt();
			detail.is_hide_role_model = (uint8_t)sys_cfg[Public_Fields::SysCfg::IS_HIDE_ROLE_MODEL].numberInt();
			detail.is_hide_hero_model = (uint8_t)sys_cfg[Public_Fields::SysCfg::IS_HIDE_HERO_MODEL].numberInt();
			detail.is_hide_title = (uint8_t)sys_cfg[Public_Fields::SysCfg::IS_HIDE_TITLE].numberInt();
			detail.player_count = (uint8_t)sys_cfg[Public_Fields::SysCfg::PLAYER_COUNT].numberInt();
			detail.is_skip_plot = (uint8_t)sys_cfg[Public_Fields::SysCfg::IS_SKIP_PLOT].numberInt();
		}

		Artifact_Detail artifact_detail;
		BSONObjIterator it_artifact(res.getObjectField(Public_Fields::ARTIFACTS.c_str()));
		while (it_artifact.more()) {
			BSONObj obj_artifact = it_artifact.next().embeddedObject();
			artifact_detail.reset();
			artifact_detail.artifact_id = obj_artifact[Public_Fields::Artifact::ARTIFACT_ID].numberInt();
			artifact_detail.artifact_lvl = obj_artifact[Public_Fields::Artifact::ARTIFACT_LVL].numberInt();
			detail.artifacts.insert(std::make_pair(artifact_detail.artifact_id, artifact_detail));
		}

		BSONObjIterator it_fornotice_gifts(res.getObjectField(Public_Fields::FORNOTICE_GIFTS_STATUS.c_str()));
		while (it_fornotice_gifts.more()) {
			BSONObj obj_gift = it_fornotice_gifts.next().embeddedObject();
			int gift_lvl = obj_gift[Public_Fields::Fornotice_Gifts_Status::GIFT_LVL].numberInt();
			int8_t status = obj_gift[Public_Fields::Fornotice_Gifts_Status::HAS_RECEIVED].numberInt();
			detail.fornotice_gifts_status.insert(std::make_pair(gift_lvl, status));
		}

		BSONObjIterator it_receive_flowers(res.getObjectField(Public_Fields::RECEIVE_FLOWERS_LIST.c_str()));
		Flowers_Info info;
		detail.flowers_info_vec.clear();
		while (it_receive_flowers.more()) {
			info.reset();
			BSONObj obj = it_receive_flowers.next().embeddedObject();
			info.role_id = obj[Operating_Fields::Receive_Flowers_List::ROLE_ID].numberLong();
			info.role_name = obj[Operating_Fields::Receive_Flowers_List::ROLE_NAME].valuestrsafe();
			info.item_id = obj[Operating_Fields::Receive_Flowers_List::ITEM_ID].numberInt();
			info.charm = obj[Operating_Fields::Receive_Flowers_List::CHARM].numberInt();
			info.friendship = obj[Operating_Fields::Receive_Flowers_List::FRIEND_NUM].numberInt();
			info.msg = obj[Operating_Fields::Receive_Flowers_List::MSG].valuestrsafe();
			detail.flowers_info_vec.push_back(info);
		}

		return 0;
	} else {
		return -1;
	}
	return 0;
}

int DB_Operator::save_public_detail(Public_Detail &detail) {
	BSONObjBuilder builder;

	std::vector<BSONObj> vc_plot;
	for (String_Int_Map::const_iterator iter = detail.plot_info.begin(); iter != detail.plot_info.end(); ++iter) {
		vc_plot.push_back(BSON(Public_Fields::PLOT_ID << iter->first
                << Public_Fields::PLOT_NUM << iter->second));
	}
	builder.append(Public_Fields::PLOT_INFO, vc_plot);

	std::vector<BSONObj> local_his;
	for (std::deque<Location>::iterator it = detail.local_his.begin(); it != detail.local_his.end(); ++it) {
		BSONObj scene_trace_obj = BSON( Role_Fields::Location::SCENE_ID << (*it).scene_id
								<< Role_Fields::Location::SCENE_ORDER << (*it).scene_order
								<< Role_Fields::Location::SCENE_LAYER << (*it).scene_layer
								<< Role_Fields::Location::COORD_X << (*it).coord.x
								<< Role_Fields::Location::COORD_Y << (*it).coord.y
								<< Role_Fields::Location::TOWARD << (*it).toward);
		local_his.push_back(scene_trace_obj);
	}
	builder.append(Role_Fields::LOCAL_HIS, local_his);

	//guide_list
    vector<BSONObj> vc_guide_list;
	for(Guide_NUM_Map::iterator iter = detail.guide_num_map.begin(); iter != detail.guide_num_map.end(); ++iter) {
		BSONObjBuilder guide_builder;
		guide_builder << Public_Fields::Guide::GUIDE_ID << (uint16_t)iter->first
				    << Public_Fields::Guide::USED_NUM << (int8_t)iter->second;
			vc_guide_list.push_back(guide_builder.obj());
	}
	builder.append(Public_Fields::GUIDE_LIST, vc_guide_list);

	//guide step list
	vector<BSONObj> vc_guide_step_list;
	for(Guide_Step_Map::const_iterator it = detail.guide_step_map.begin(); it != detail.guide_step_map.end(); ++it) {
		BSONObjBuilder guide_step_builder;
		guide_step_builder << Public_Fields::Guide_Step::GUIDE_ID << (uint16_t)it->first
					       << Public_Fields::Guide_Step::STEP << (uint8_t)it->second.step
					       << Public_Fields::Guide_Step::NUM << (uint8_t)it->second.num;
		vc_guide_step_list.push_back(guide_step_builder.obj());
	}
	builder.append(Public_Fields::GUIDE_STEP_LIST, vc_guide_step_list);

	//played plot set
	vector<BSONObj> vc_plot_list;
	for(Played_Plot_Set::iterator it = detail.played_plot_set.begin(); it != detail.played_plot_set.end(); ++it) {
		BSONObjBuilder played_plot_buider;
		played_plot_buider << Public_Fields::Played_Plot::PLOT_ID << (*it);
		vc_plot_list.push_back(played_plot_buider.obj());
	}
	builder.append(Public_Fields::PLAYED_PLOT_LIST, vc_plot_list);

	//sys cfg
    vector<BSONObj> sys_cfg_list;
	BSONObjBuilder sys_cfg_builder;
	sys_cfg_builder << Public_Fields::SysCfg::IS_TOTAL_OPEN << detail.is_total_open
				<< Public_Fields::SysCfg::IS_OPEN_MUSIC << detail.is_open_music
				<< Public_Fields::SysCfg::MUSIC_NUM << detail.music_num
				<< Public_Fields::SysCfg::IS_OPEN_VOICE << detail.is_open_voice
				<< Public_Fields::SysCfg::VOICE_NUM << detail.voice_num
				<< Public_Fields::SysCfg::IS_HIDE_ROLE_MODEL << detail.is_hide_role_model
				<< Public_Fields::SysCfg::IS_HIDE_HERO_MODEL << detail.is_hide_hero_model
				<< Public_Fields::SysCfg::IS_HIDE_TITLE << detail.is_hide_title
				<< Public_Fields::SysCfg::PLAYER_COUNT << detail.player_count
				<< Public_Fields::SysCfg::IS_SKIP_PLOT << detail.is_skip_plot;
	sys_cfg_list.push_back(sys_cfg_builder.obj());
	builder.append(Public_Fields::SYS_CFG, sys_cfg_list);

	std::vector<BSONObj> artifact_bson_vec;
	for (Artifact_Map::const_iterator it_artifact = detail.artifacts.begin();
			it_artifact != detail.artifacts.end(); ++it_artifact) {
		artifact_bson_vec.push_back(BSON(
				Public_Fields::Artifact::ARTIFACT_ID << it_artifact->second.artifact_id
				<< Public_Fields::Artifact::ARTIFACT_LVL << it_artifact->second.artifact_lvl
				));
	}

	std::vector<BSONObj> bvec_fornotice_gifts;
	for (Fornotice_Gifts_Status_Map::const_iterator it_gifts = detail.fornotice_gifts_status.begin();
			it_gifts != detail.fornotice_gifts_status.end(); ++it_gifts) {
		bvec_fornotice_gifts.push_back(BSON(
				Public_Fields::Fornotice_Gifts_Status::GIFT_LVL << it_gifts->first
				<< Public_Fields::Fornotice_Gifts_Status::HAS_RECEIVED << it_gifts->second
				));
	}

	//收到的鲜花列表
	std::vector<BSONObj> flowers_info;
	flowers_info.clear();
	for(std::vector<Flowers_Info>::iterator it = detail.flowers_info_vec.begin(); it != detail.flowers_info_vec.end(); ++it) {
		flowers_info.push_back(BSON(
				Public_Fields::Receive_Flowers_List::ROLE_ID << (long long int)it->role_id
				<< Public_Fields::Receive_Flowers_List::ROLE_NAME << it->role_name
				<< Public_Fields::Receive_Flowers_List::ITEM_ID << it->item_id
				<< Public_Fields::Receive_Flowers_List::CHARM << it->charm
				<< Public_Fields::Receive_Flowers_List::FRIEND_NUM << it->friendship
				<< Public_Fields::Receive_Flowers_List::MSG << it->msg
		));
	}

	std::vector<int32_t> killed_match_man_vec;
	for (boost::unordered_set<int32_t>::const_iterator set_iter = detail.killed_match_man.begin();
			set_iter != detail.killed_match_man.end(); ++set_iter) {
		killed_match_man_vec.push_back(*set_iter);
	}

	builder << Public_Fields::ROLE_ID << (long long int)detail.role_id
			<< Public_Fields::PLOT_ID << detail.plot_id
			<< Public_Fields::PLOT_STEP << detail.plot_step
			<< Public_Fields::PLOT_SWITCH << detail.plot_switch
			<< Public_Fields::ADD_UP_ONLINE_TIME << detail.add_up_online_time
			<< Public_Fields::FCM << detail.fcm
			<< Public_Fields::WAR_ENTER_NUM << detail.war_enter_num
			<< Public_Fields::WAR_MERIT << detail.war_merit
			<< Public_Fields::MERIT << detail.merit
			<< Public_Fields::WAR_FORBID_TIME << (unsigned int)detail.war_forbid_time.sec()
			<< Public_Fields::ADD_CAMPFIRE << detail.add_campfire
			<< Public_Fields::CAMPFIRE_ID << detail.campfire_id
			<< Public_Fields::CAMPFIRE_FINISH_TIME << detail.campfire_finish_time
			<< Public_Fields::CAMPFIRE_RANK << detail.campfire_rank
			<< Public_Fields::CAMPFIRE_KILLED_MATCH_MAN << killed_match_man_vec
			<< Public_Fields::CAMPFIRE_LIGHTER << detail.campfire_lighter
			<< Public_Fields::CAMPFIRE_RECEIVESTAT << detail.campfire_recieve_stat
			<< Public_Fields::CAMPFIRE_RANK_RCV_STAT << detail.campfire_rank_rev_stat

			<< Public_Fields::WAR_RESULT << detail.war_result
			<< Public_Fields::WAR_RANK << detail.war_rank
			<< Public_Fields::WAR_KILL_NUM << detail.war_kill_num
			<< Public_Fields::WAR_COE << detail.war_coe
			<< Public_Fields::WORSHIPS << detail.arena_worship_times
			<< Public_Fields::AWAKE_LVL << detail.awake_lvl
			<< Public_Fields::EXPLOIT_VAL << detail.exploit_val
			<< Public_Fields::EXPLOIT_LVL << detail.exploit_lvl
			<< Public_Fields::NOTICE_ID << detail.notice_id
			<< Public_Fields::IS_PLAYED_EFFECT << (detail.is_played_effect ? 1 : 0)
			<< Public_Fields::IS_SHOW << (detail.is_show ? 1 : 0)
			<< Public_Fields::IS_ROPG << detail.is_receive_old_player_gift
			<< Public_Fields::ARTIFACTS << artifact_bson_vec
			<< Public_Fields::FORNOTICE_GIFTS_STATUS << bvec_fornotice_gifts
			<< Public_Fields::RECEIVE_FLOWERS_LIST << flowers_info
	        << Public_Fields::FIRST_RECHARGE << detail.first_recharge
	        << Public_Fields::TWO_RECHARGE << detail.two_recharge
	        << Public_Fields::LIMIT_TIME_RECHARGE << detail.limit_time_recharge
	        << Public_Fields::LAST_TWO_RECHARGE_STIME << (long long int)detail.last_two_recharge_stime.sec()
	        << Public_Fields::LAST_LIMIT_TIME_RECHARGE_STIME << (long long int)detail.last_limit_time_recharge_stime.sec()
	        << Public_Fields::RECHARGE_MONEY << detail.recharge_money
	        << Public_Fields::RECHARGE_MONEY_HIS << detail.recharge_money_his
	        << Public_Fields::RECHARGE_FEEDBACK_LIST << detail.recharge_feedback_receive_list
	        << Public_Fields::ANS_QID << detail.ans_qid
	        << Public_Fields::RECHARGE_LSTIME << (long long int)detail.last_recharge_rebate_stime.sec()
	        << Public_Fields::INV_TIME << (long long int)detail.inv_time.sec()
	        << Public_Fields::REC_INV_TIME << (long long int)detail.rec_inv_time.sec()
	        << Public_Fields::IS_RECEIVE_INV << detail.is_receive_inv_ones
	        << Public_Fields::IS_RECEIVE_LIKE << detail.is_receive_like_reward
	        << Public_Fields::TODAY_REC_BG << detail.today_rec_bind_gold
			<< Public_Fields::IS_MAGIC_WING_OVER_TIME << (detail.is_magic_wing_over_time ? 1 : 0)
			<< Public_Fields::IS_MAGIC_WING_READED << (detail.is_magic_wing_readed ? 1 : 0)
			<< Public_Fields::VIP_OVER_TIME << (long long int)(detail.vip_over_time.sec())
			<< Public_Fields::IS_VIP_READED << (detail.is_vip_readed ? 1 : 0)
			<< Public_Fields::GENIUS_OVER_TIME << (long long int)(detail.genius_over_time.sec())
			<< Public_Fields::IS_GENIUS_READED << (detail.is_genius_readed ? 1 : 0)
			<< Public_Fields::TASK_GUIDE_ID << detail.task_guide_id
			<< Public_Fields::VIP_UPGRADE_CARDS << detail.vip_upgrade_cards_
			<< Public_Fields::START_FIRST_RECHARGE_TIME << (long long int)detail.start_frist_recharge_time.sec()
			<< Package_Fields::Money::FRIENDSHIP << detail.friendship_accumulate;


	MONGO_CONNECTION.update(Public_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}
