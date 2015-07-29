/*
 * DB_Operator_M.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: gaojianshan
 */

#include "arena/Arena_Struct.h"
#include "campfire/Campfire_Struct.h"
#include "world_boss/World_Boss_Struct.h"
#include "DB_Operator.h"
#include "mongo/client/dbclient.h"
#include "Config_Cache.h"
#include "DB_Fields.h"
#include "DB_Merge_Server_Manager.h"
#include "Pool_Manager.h"
#include "mail/Mail_Struct.h"
#include "mail/Mail_Config.h"
#include "mail/Mail_Def.h"
#include "team/Team_Struct.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Mail.h"
#include "Logic_Player_Struct.h"
#include "Time_Value.h"
#include <sstream>

bool DB_Merge_Server_Manager::get_is_merge_server() const {
	return is_merge_server;
}

void DB_Merge_Server_Manager::set_is_merge_server(bool flag) {
	is_merge_server = flag;
}

void DB_Merge_Server_Manager::merge_database_mmo(DBClientConnection &dest_conn, std::vector<Int_String> &server_list) {
	// 遍历要合并的数据库
	for(std::vector<Int_String>::const_iterator it = server_list.begin(); it != server_list.end(); ++it) {
		DBClientConnection &source_conn = *connection_merge_server(it->str_value, it->int_value);

		// 找出所有mmo下面的表，然后把mmo.role表放到最后一个
		unsigned long long data_count = 0, save_name = 0;
		std::list<string> collection_list = source_conn.getCollectionNames("mmo");
		std::list<string> collection_list_tmp;
		collection_list_tmp.push_back("mmo.role");
		for (list<string>::iterator c_it = collection_list.begin(); c_it != collection_list.end(); ++c_it) {
			if ((*c_it) != "mmo.role" && (*c_it) != "mmo.system.indexes") {
				collection_list_tmp.push_back(*c_it);
			}
		}

		// 遍历mmo所有的集合
		for(list<string>::iterator collection_it = collection_list_tmp.begin(); collection_it != collection_list_tmp.end(); ++collection_it) {
			const std::string table_name = *collection_it;


			// 要清空的表
			if(table_name == "mmo.chat_record"
					|| table_name == "mmo.gang_chat_record"
					|| table_name == "mmo.gang_war_info"
					|| table_name == "mmo.gang_war_info_bak"
					|| table_name == "mmo.ranking"
					|| table_name == "mmo.title_record"
					|| table_name == "mmo.expedition"
					|| table_name == "mmo.expedition_g_manager_data"
					|| table_name == "mmo.expedition_g_manager_data_b"
					|| table_name == "mmo.expedition_manager"
					|| table_name == "mmo.expedition_manager_bak"
					|| table_name == "mmo.expedition_manager_data") {

				dest_conn.dropCollection(table_name);
				continue;
			}

			// 不处理的表
			if(table_name == "mmo.activities_stores"
					|| table_name == "mmo.auction_goods"
					|| table_name == "mmo.common") {
				continue;
			}

			data_count = source_conn.count(table_name);
			vector<BSONObj> source_iter_record;
			source_iter_record.reserve(data_count);
			source_conn.findN(source_iter_record, table_name, mongo::Query(), data_count, 0);

			if(table_name == "mmo.role") {
				save_name = merge_mmo_role(dest_conn, source_conn);
			} else if (table_name == "mmo.arena_season") {
				merge_mmo_arena_season(dest_conn, source_conn);
			} else if (table_name == "mmo.fb_info") {
				merge_mmo_fb_info(dest_conn, source_conn);
			}  else if (table_name == "mmo.team_sports") {
				merge_mmo_team_sports(dest_conn, source_conn);
			}  else if (table_name == "mmo.world_boss") {
				merge_mmo_world_boss(dest_conn, source_conn);
			} else {
				merge_mmo_other(dest_conn, source_conn, source_iter_record, table_name);
			}

		}

		cout << "merge server ==> source server: ip=" << it->str_value << ", port=" << it->int_value << " total:" << data_count<< " same name:" << save_name << endl;

		POOL_MANAGER->push_dbclientConnection_pool(&source_conn);
	}
}

void DB_Merge_Server_Manager::merge_database_backstage(DBClientConnection &dest_conn, std::vector<Int_String> &server_list) {
	// 遍历要合并的数据库
	for(std::vector<Int_String>::const_iterator it = server_list.begin(); it != server_list.end(); ++it) {
		DBClientConnection &source_conn = *connection_merge_server(it->str_value, it->int_value);

		// 找出所有backstage下面的表
		unsigned long long data_count = 0;
		std::list<string> collection_list_tmp = source_conn.getCollectionNames("backstage");

		// 遍历mmo所有的集合
		for(list<string>::iterator collection_it = collection_list_tmp.begin(); collection_it != collection_list_tmp.end(); ++collection_it) {
			const std::string table_name = *collection_it;


			// 要清空的表
			if(table_name == "backstage.pv_hour"
					|| table_name == "backstage.pv"
					|| table_name == "backstage.access_count"
					|| table_name == "backstage.variable"
					|| table_name == "backstage.flash_logs"
					|| table_name == "backstage.cross_area"
					|| table_name == "backstage.batch_mail"
					|| table_name == "backstage.gift_receive_id") {

				dest_conn.dropCollection(table_name);
				continue;
			}

			// 不处理的表
			if(table_name == "backstage.event"
					|| table_name == "backstage.exchange"
					|| table_name == "backstage.gift"
					|| table_name == "backstage.mall"
					|| table_name == "backstage.mall_banner"
					|| table_name == "backstage.mall_promo"
					|| table_name == "backstage.gm_annouce") {
				continue;
			}

			data_count = source_conn.count(table_name);
			vector<BSONObj> source_iter_record;
			source_iter_record.reserve(data_count);
			source_conn.findN(source_iter_record, table_name, mongo::Query(), data_count, 0);

			//
			if(table_name == "backstage.ban_account"
					|| table_name == "backstage.ban_gag"
					|| table_name == "backstage.ban_ip"
					|| table_name == "backstage.pack"
					|| table_name == "backstage.pay"
					|| table_name == "backstage.proposal"
					|| table_name == "backstage.torr"
					|| table_name == "backstage.system_mail"
					|| table_name == "backstage.questionnaite") {
				merge_backstage_collection(dest_conn, source_conn, table_name);
			}

		}

		cout << "merge server ==> source server: ip=" << it->str_value << ", port=" << it->int_value << " total:" << data_count << endl;

		POOL_MANAGER->push_dbclientConnection_pool(&source_conn);
	}
}

void DB_Merge_Server_Manager::merge_database() {
	std::vector<Int_String> server_list = CONFIG_CACHE->merge_server_list();
	Int_String dest_mongo = server_list.front();
	server_list.erase(server_list.begin());

	DBClientConnection &dest_conn = *connection_merge_server(dest_mongo.str_value, dest_mongo.int_value);
	cout << "merge server ==> dest server: ip=" << dest_mongo.str_value << ", port=" << dest_mongo.int_value << endl;

	// 合并mmo数据库
	merge_database_mmo(dest_conn, server_list);

	// 合并backstage数据库
	merge_database_backstage(dest_conn, server_list);

	// 清僵尸号
	clean_old_role(dest_conn);

	std::cout << "merge mongo database success!" << std::endl;
}

void DB_Merge_Server_Manager::merge_mysql_database(Conn_Dbname_Map &conn_dbname_map) {
	for(Conn_Dbname_Map::iterator it = conn_dbname_map.begin(); it != conn_dbname_map.end(); ++it) {
		if(it->first == 0) {
			std::cout << "connection of ip : " << it->second.mysql_ip
					<< " port : " << it->second.mysql_port
					<< " username : " << it->second.mysql_user
					<< " password : " << it->second.mysql_pw
					<< " database : " << it->second.mysql_dbname
					<< " loss efficacy , exit "
					<< std::endl;
			break;
		}
		it->first->setSchema(it->second.mysql_dbname);
		it->first->setAutoCommit(true);
		sql::Statement *statement = it->first->createStatement();
		if(statement == 0) {
			std::cout << "create statement of connection failed, exit " << std::endl;
			delete it->first;
			break;
		}
//		std::string use_db_sql = "use " + it->second.mysql_dbname + ";";
//		int use_db_result = statement->execute(use_db_sql);
//		if(use_db_result == 0) {
//			std::cout << "use database " << it->second.mysql_dbname << "failed, exit " << std::endl;
//			delete it->first;
//			break;
//		}

		std::map<std::string, std::string> table_key_map;
		table_key_map.insert(std::make_pair("api_log", "time"));
		table_key_map.insert(std::make_pair("flash_logs", "time"));
		table_key_map.insert(std::make_pair("item_stream", "is_op_time"));
		table_key_map.insert(std::make_pair("stat_depletion", "stat_time"));
		table_key_map.insert(std::make_pair("stat_depletion_item", "stat_time"));
		table_key_map.insert(std::make_pair("stat_expand", "stat_time"));
		table_key_map.insert(std::make_pair("stat_fortune", "stat_time"));
		table_key_map.insert(std::make_pair("stat_level", "stat_time"));
		table_key_map.insert(std::make_pair("stat_level_recharge", "stat_time"));
		table_key_map.insert(std::make_pair("stat_login_keep", "stat_time"));
		table_key_map.insert(std::make_pair("stat_login_loss", "stat_time"));
		table_key_map.insert(std::make_pair("stat_money_loss", "stat_time"));
		table_key_map.insert(std::make_pair("stat_new_account_process", "stat_time"));
		table_key_map.insert(std::make_pair("stat_online_day", "stat_time"));
		table_key_map.insert(std::make_pair("stat_online_ip", "stat_time"));
		table_key_map.insert(std::make_pair("stat_online_minute", "stat_time"));
		table_key_map.insert(std::make_pair("stat_online_player_time_day", "stat_time"));
		table_key_map.insert(std::make_pair("stat_online_time", "stat_time"));
		table_key_map.insert(std::make_pair("stat_player_npc", "stat_time"));
		table_key_map.insert(std::make_pair("stat_recharge", "stat_time"));
		table_key_map.insert(std::make_pair("stat_recharge_rank", "stat_time"));
		table_key_map.insert(std::make_pair("stat_register", "stat_time"));
		table_key_map.insert(std::make_pair("stat_retention", "stat_time"));
		table_key_map.insert(std::make_pair("stat_task", "stat_time"));
		table_key_map.insert(std::make_pair("stream_access", "time"));
		table_key_map.insert(std::make_pair("stream_crontab", "start_time"));
		table_key_map.insert(std::make_pair("stream_fb_click", "time"));
		table_key_map.insert(std::make_pair("stream_first_day_register", "player_id"));
		table_key_map.insert(std::make_pair("stream_level", "time"));
		table_key_map.insert(std::make_pair("stream_login", "login_time"));
		table_key_map.insert(std::make_pair("stream_login_logout", "logout_time"));
		table_key_map.insert(std::make_pair("stream_money", "time"));
		table_key_map.insert(std::make_pair("stream_physical", "time"));
		table_key_map.insert(std::make_pair("stream_player_npc", "time"));
		table_key_map.insert(std::make_pair("stream_register", "time"));
		table_key_map.insert(std::make_pair("stream_task", "time"));
		table_key_map.insert(std::make_pair("system_mail", "time"));

		std::string sql;
		int result;
		std::stringstream time_str;
		uint now_ = (uint)Time_Value::gettimeofday().sec();
		// 删除两周前的数据
		uint timestamp_ = now_ - 2*7*24*3600;
		time_str << timestamp_;
		for(std::map<std::string, std::string>::iterator itera = table_key_map.begin(); itera != table_key_map.end(); ++itera) {
			sql = "delete from " + itera->first + " where " + itera->second + " < " + time_str.str();
//			sql = "delete from " + itera->first + " where " + itera->second + " > " + "0";
			result = statement->execute(sql);
			if(result == 0) {
				std::cout << "execute " << sql << " in database " << it->second.mysql_dbname << " success " << std::endl;
			}else {
				std::cout << "execute " << sql << " in database " << it->second.mysql_dbname << " failed " << std::endl;
			}
		}

		delete statement;
		delete it->first;
		std::cout << "merge mysql database sucess after cleaning " << table_key_map.size() << " tables in " << it->second.mysql_dbname << std::endl;
		// 只需要操作被合服的数据库
		break;
	}
	std::cout << "merge mysql database sucess after cleaning all databases " << std::endl;
}

void DB_Merge_Server_Manager::send_rename_mail(role_id_t role_id, DBClientConnection *dest_conn) {
	int agent = role_id / 10000000000000;
	Mail_Base_Config mail_config;
	const Mail_Base_Config *mail_config_ptr = CONFIG_CACHE_MAIL->find_merge_config(agent);
	if (!mail_config_ptr) {
		const Mail_Base_Config *mail_config_ptr2 = CONFIG_CACHE_MAIL->find_merge_config(0);
		if (mail_config_ptr2) {
			mail_config = *mail_config_ptr2;
		}
	} else {
		mail_config = *mail_config_ptr;
	}

	Mail_Send_Info send_info;
	if (mail_config.sender_name != "") {
		send_info.sender_id = 0;
		send_info.sender_name = mail_config.sender_name;
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.receiver_id = role_id;
		send_info.title = mail_config.mail_title;
		send_info.content = mail_config.mail_content;
		send_info.show_send_time = Time_Value::gettimeofday();

		for (std::vector<Item_Basic_Info>::iterator it = mail_config.item_vector.begin(); it != mail_config.item_vector.end(); ++it) {
			Item_Detail item_detail;
			item_detail.id_ = (*it).id;
			item_detail.amount_ = (*it).amount;
			item_detail.bind_ = (*it).bind;
			send_info.item_vector.push_back(item_detail);
		}
	} else {
		send_info.sender_id = 0;
		send_info.sender_name = "系统";
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.receiver_id = role_id;
		send_info.title = "合服福利";
		send_info.content = "因合服数据互通处理，玩家角色名增加后缀，特补偿更名卡一张，快给自己取个炫酷霸气的新名字吧~";
		send_info.show_send_time = Time_Value::gettimeofday();

		Item_Detail item_detail;
		item_detail.id_ = 20601404;
		item_detail.amount_ = 1;
		item_detail.bind_ = 2;
		send_info.item_vector.push_back(item_detail);
	}

	MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info, dest_conn);

}


DBClientConnection *DB_Merge_Server_Manager::connection_merge_server(std::string ip, uint32_t port) {
	DBClientConnection *conn = 0;
	conn = POOL_MANAGER->pop_dbclientConnection_pool();
	if(!conn) {
		std::cout << "connection_merge_server error" << endl;
		return 0;
	}

	std::string err_msg;
	std::stringstream host_stream;
	host_stream << ip << ":" << port;
	conn->connect(host_stream.str().c_str(), err_msg);
    return conn;
}

void DB_Merge_Server_Manager::merge_server() {
	std::string err_msg;

	DBClientConnection *conn = 0;
	conn = POOL_MANAGER->pop_dbclientConnection_pool();
	std::vector<Int_String> &server_list = CONFIG_CACHE->merge_server_list();

	for (std::vector<Int_String>::iterator it = server_list.begin(); it != server_list.end(); ++it) {
		std::stringstream host_stream;
		host_stream << (*it).str_value;
		host_stream << ":" << (*it).int_value;

		if (!conn->connect(host_stream.str().c_str(), err_msg)) {
			std::cout << "连不上mongo, ip:" << (*it).str_value << " port:" << (*it).int_value << std::endl;
			exit(0);
		}
	}

	std::vector<Mysql_Conf> &mysql_conf_vec = CONFIG_CACHE->merge_mysql_list();
	Conn_Dbname_Map conn_dbname_map;
	sql::Driver* driver_ = get_driver_instance();
	if(driver_ == 0) {
		std::cout << "mysql dirver is null, exit" << std::endl;
		exit(0);
	}
	for(std::vector<Mysql_Conf>::iterator itera = mysql_conf_vec.begin(); itera != mysql_conf_vec.end(); ++itera) {
		std::stringstream port_str;
		port_str << (*itera).mysql_port;
		std::string conn_url = "tcp://" + (*itera).mysql_ip + ":" + port_str.str();
		sql::Connection* conn_ = driver_->connect(conn_url.c_str(), (*itera).mysql_user.c_str(), (*itera).mysql_pw.c_str());
		if(conn_ == 0 || conn_->isClosed()) {
			std::cout << "can not connect ip : " << (*itera).mysql_ip
					<< " port : " << port_str
					<< "user : " << (*itera).mysql_user
					<< " password : " << (*itera).mysql_pw
					<< ", exit "
					<< std::endl;
			exit(0);
		}
		conn_dbname_map.insert(std::make_pair(conn_, (*itera)));
		// 只需要操作被合服的数据库, 所以只需要取出第1个服的链接
		break;
	}

	std::cout << "merge mongo start " << std::endl;
	merge_database();
	std::cout << "merge mongo end " << std::endl;

	std::cout << "merge mysql start " << std::endl;
	merge_mysql_database(conn_dbname_map);
	std::cout << "merge mysql end " << std::endl;
}

int DB_Merge_Server_Manager::merge_backstage_collection(DBClientConnection &dest_conn, DBClientConnection &source_conn, std::string table_name) {
	std::string table = table_name;
	int data_count = source_conn.count(table);
	vector<BSONObj> source_iter_record;
	source_iter_record.reserve(data_count);
	source_conn.findN(source_iter_record, table, mongo::Query(), data_count, 0);

	for (vector<BSONObj>::iterator iter = source_iter_record.begin(); iter != source_iter_record.end(); ++iter) {
		dest_conn.insert(table, *iter);
	}

	return 1;
}

int DB_Merge_Server_Manager::merge_mmo_role(DBClientConnection &dest_conn, DBClientConnection &source_conn) {
	int data_count = source_conn.count("mmo.role");
	vector<BSONObj> source_iter_record;
	source_iter_record.reserve(data_count);
	source_conn.findN(source_iter_record, "mmo.role", mongo::Query(), data_count, 0);

	int save_name = 0;
	for (vector<BSONObj>::iterator iter = source_iter_record.begin(); iter != source_iter_record.end(); ++iter) {
		BSONObj &bson_obj = *iter;
		std::string role_name = bson_obj[Role_Fields::ROLE_NAME].str();
		int64_t role_id = (int64_t)(bson_obj[Role_Fields::ROLE_ID].numberLong());

		BSONObj res = dest_conn.findOne("mmo.role", QUERY(Role_Fields::ROLE_ID << (long long)role_id));
		if (!res.isEmpty()) {
			std::cout << "mmo.role角色id:" << role_id << "重复，检查是否重复合并,否则还原数据重新合并.! " << std::endl;
			exit(0);
		}

		res = dest_conn.findOne("mmo.role", QUERY(Role_Fields::ROLE_NAME << role_name));
    	if (!res.isEmpty()) {
    		save_name++;
			int agent_num = 0;
			int server_num = 0;
	    	if (bson_obj.hasField(Role_Fields::ACCOUNT_INFO.c_str())) {
	    		agent_num = bson_obj[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::AGENT_NUM].numberInt();
	    		server_num = bson_obj[Role_Fields::ACCOUNT_INFO][Role_Fields::Account_Info::SERVER_NUM].numberInt();
	    	}
    		std::ostringstream oss;
    		oss << server_num << "_" << agent_num << "_" << server_num;
    		role_name = oss.str();
    		bson_obj = bson_obj.removeField(Role_Fields::ROLE_NAME);
    		BSONObjBuilder builder;
    		builder.appendElements(bson_obj);
    		builder << Role_Fields::ROLE_NAME  << role_name;
    		bson_obj = builder.obj();
    		send_rename_mail(role_id, &dest_conn);
    	}

		dest_conn.insert("mmo.role", bson_obj);
	}

	return save_name;
}

int DB_Merge_Server_Manager::merge_mmo_other(DBClientConnection &dest_conn, DBClientConnection &source_conn, std::vector<BSONObj> &source_iter_record, std::string table_name) {
	for (vector<BSONObj>::iterator iter = source_iter_record.begin(); iter != source_iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		//if (obj.hasField(Role_Fields::ROLE_ID.c_str()) || obj.hasField(Role_Fields::GANG_ID.c_str()))
		dest_conn.insert(table_name, obj);
	}
	return 0;
}

int DB_Merge_Server_Manager::merge_mmo_arena_season(DBClientConnection &dest_conn, DBClientConnection &source_conn) {
	Arena_Season_Detail s_detail;
	MONGO_OPERATOR->set_inner_conn(&source_conn);
	MONGO_OPERATOR->load_arena_season_detail(s_detail);
	MONGO_OPERATOR->set_inner_conn(0);

	Arena_Season_Detail d_detail;
	MONGO_OPERATOR->set_inner_conn(&dest_conn);
	MONGO_OPERATOR->load_arena_season_detail(d_detail);
	MONGO_OPERATOR->set_inner_conn(0);

	Arena_Season_Result_Map::iterator it;
	for(it = s_detail.season_result_map.begin(); it != s_detail.season_result_map.end(); ++it){
		d_detail.season_result_map.insert(std::make_pair(it->first, it->second));
	}

	MONGO_OPERATOR->set_inner_conn(&dest_conn);
	MONGO_OPERATOR->save_arena_season_detail(d_detail);
	MONGO_OPERATOR->set_inner_conn(0);

	return 0;
}

int DB_Merge_Server_Manager::merge_mmo_fb_info(DBClientConnection &dest_conn, DBClientConnection &source_conn) {
	FB_Info_Detail_Map s_detail_map;
	MONGO_OPERATOR->set_inner_conn(&source_conn);
	MONGO_OPERATOR->load_fb_info(s_detail_map);
	MONGO_OPERATOR->set_inner_conn(0);

	FB_Info_Detail_Map d_detail_map;
	MONGO_OPERATOR->set_inner_conn(&dest_conn);
	MONGO_OPERATOR->load_fb_info(d_detail_map);
	MONGO_OPERATOR->set_inner_conn(0);

	MONGO_OPERATOR->set_inner_conn(&dest_conn);
	FB_Info_Detail_Map::iterator it;
	FB_Info_Detail_Map::iterator d_it;
	for(it = s_detail_map.begin(); it != s_detail_map.end(); ++it){
		d_it = d_detail_map.find(it->first);
		if(d_it != d_detail_map.end()){
			if(it->second.best_force >= d_it->second.best_force){
				continue;
			}
		}
		MONGO_OPERATOR->save_fb_info_detail(it->second);
	}
	MONGO_OPERATOR->set_inner_conn(0);

	return 0;
}

int DB_Merge_Server_Manager::merge_mmo_team_sports(DBClientConnection &dest_conn, DBClientConnection &source_conn) {
	int d_game_times = 0;
	RoleId_TeamSportsRankingDetail_Map d_rank_map;
	Team_Sports_Ranking_Info d_rank;
	Time_Value d_team_sports_start_time;
	First_Lvl_Detail d_first_lvl;
	MONGO_OPERATOR->set_inner_conn(&dest_conn);
	MONGO_OPERATOR->load_team_role(d_rank_map, d_rank, d_team_sports_start_time, d_first_lvl, d_game_times);
	MONGO_OPERATOR->set_inner_conn(0);

	int s_game_times = 0;
	RoleId_TeamSportsRankingDetail_Map s_rank_map;
	Team_Sports_Ranking_Info s_rank;
	Time_Value s_team_sports_start_time;
	First_Lvl_Detail s_first_lvl;
	MONGO_OPERATOR->set_inner_conn(&source_conn);
	MONGO_OPERATOR->load_team_role(s_rank_map, s_rank, s_team_sports_start_time, s_first_lvl, s_game_times);
	MONGO_OPERATOR->set_inner_conn(0);


	for (RoleId_TeamSportsRankingDetail_Map::iterator it = s_rank_map.begin(); it != s_rank_map.end(); ++it) {
		d_rank_map.insert(std::make_pair(it->first, it->second));
	}


	int d_max_level = 0;
	for (Int_Uid::iterator it = d_first_lvl.first_lvl_ck_his.begin(); it != d_first_lvl.first_lvl_ck_his.end(); ++it) {
		if (it->first > d_max_level) {
			d_max_level = it->first;
		}
	}
	int s_max_level = 0;
	for (Int_Uid::iterator it = s_first_lvl.first_lvl_ck_his.begin(); it != s_first_lvl.first_lvl_ck_his.end(); ++it) {
		if (it->first > s_max_level) {
			s_max_level = it->first;
		}
	}
	if (s_max_level > d_max_level) {
		d_first_lvl.first_lvl_ck_his = s_first_lvl.first_lvl_ck_his;
	}


	d_max_level = 0;
	for (Int_Uid::iterator it = d_first_lvl.first_lvl_role_id_his.begin(); it != d_first_lvl.first_lvl_role_id_his.end(); ++it) {
		if (it->first > d_max_level) {
			d_max_level = it->first;
		}
	}
	s_max_level = 0;
	for (Int_Uid::iterator it = s_first_lvl.first_lvl_role_id_his.begin(); it != s_first_lvl.first_lvl_role_id_his.end(); ++it) {
		if (it->first > s_max_level) {
			s_max_level = it->first;
		}
	}
	if (s_max_level > d_max_level) {
		d_first_lvl.first_lvl_role_id_his = s_first_lvl.first_lvl_role_id_his;
	}


	d_max_level = 0;
	for (Int_Uid::iterator it = d_first_lvl.first_lvl_yss_his.begin(); it != d_first_lvl.first_lvl_yss_his.end(); ++it) {
		if (it->first > d_max_level) {
			d_max_level = it->first;
		}
	}
	s_max_level = 0;
	for (Int_Uid::iterator it = s_first_lvl.first_lvl_yss_his.begin(); it != s_first_lvl.first_lvl_yss_his.end(); ++it) {
		if (it->first > s_max_level) {
			s_max_level = it->first;
		}
	}
	if (s_max_level > d_max_level) {
		d_first_lvl.first_lvl_yss_his = s_first_lvl.first_lvl_yss_his;
	}


	d_max_level = 0;
	for (Int_Uid::iterator it = d_first_lvl.first_lvl_zs_his.begin(); it != d_first_lvl.first_lvl_zs_his.end(); ++it) {
		if (it->first > d_max_level) {
			d_max_level = it->first;
		}
	}
	s_max_level = 0;
	for (Int_Uid::iterator it = s_first_lvl.first_lvl_zs_his.begin(); it != s_first_lvl.first_lvl_zs_his.end(); ++it) {
		if (it->first > s_max_level) {
			s_max_level = it->first;
		}
	}
	if (s_max_level > d_max_level) {
		d_first_lvl.first_lvl_zs_his = s_first_lvl.first_lvl_zs_his;
	}

	MONGO_OPERATOR->set_inner_conn(&dest_conn);
	MONGO_OPERATOR->save_team_role(d_rank_map, d_team_sports_start_time, d_first_lvl, d_game_times);
	MONGO_OPERATOR->set_inner_conn(0);
	return 0;
}

int DB_Merge_Server_Manager::merge_mmo_world_boss(DBClientConnection &dest_conn, DBClientConnection &source_conn) {
	World_Boss_Detail s_detail;
	MONGO_OPERATOR->set_inner_conn(&source_conn);
	MONGO_OPERATOR->load_world_boss_detail(s_detail);
	MONGO_OPERATOR->set_inner_conn(0);

	World_Boss_Detail d_detail;
	MONGO_OPERATOR->set_inner_conn(&dest_conn);
	MONGO_OPERATOR->load_world_boss_detail(d_detail);
	MONGO_OPERATOR->set_inner_conn(0);


	if(s_detail.kill_num > d_detail.kill_num){
		MONGO_OPERATOR->set_inner_conn(&dest_conn);
		MONGO_OPERATOR->save_world_boss_detail(s_detail);
		MONGO_OPERATOR->set_inner_conn(0);
	}

	return 0;
}

int DB_Merge_Server_Manager::clean_old_role(DBClientConnection &dest_conn) {
	if (CONFIG_CACHE->server_maintainer_cache().merge_level_lt_ == 0|| CONFIG_CACHE->server_maintainer_cache().merge_login_day_ == 0) {
		std::cout << "no clean old player" << std::endl;
		return 0;
	}

	// 从数据库加载所有玩家
	DB_View db_view;
	MONGO_OPERATOR->set_inner_conn(&dest_conn);
	MONGO_OPERATOR->load_db_view(&db_view, false);
	MONGO_OPERATOR->set_inner_conn(0);

	// 找出符合条件的玩家
	Role_Vec old_role;
	Time_Value now = Time_Value::gettimeofday();
	for (DB_View::ID_Player_View_Map::iterator it = db_view.id_player_view_map.begin(); it != db_view.id_player_view_map.end(); ++it) {
		if (it->second->level <= CONFIG_CACHE->server_maintainer_cache().merge_level_lt_) {
			Time_Value last_sign_out = now - it->second->last_sign_out_time;
			if (last_sign_out.sec() / 86400 >= CONFIG_CACHE->server_maintainer_cache().merge_login_day_) {
				// 没充过钱且不是会长
				BSONObj res = dest_conn.findOne("backstage.pay", QUERY(Role_Fields::ROLE_ID << (long long int)it->second->role_id));
				if (res.isEmpty()) {
					BSONObj res = dest_conn.findOne("mmo.gang", QUERY("leader" << (long long int)it->second->role_id));
					if (res.isEmpty()) {
						old_role.push_back(it->second->role_id);
					}
				}
			}
		}
	}

	// 移除所有表中相关玩家数据
	std::list<string> collection_list = dest_conn.getCollectionNames("mmo");
	std::list<string> collection_list_tmp;
	for (list<string>::iterator c_it = collection_list.begin(); c_it != collection_list.end(); ++c_it) {
		if ((*c_it) != "mmo.role" && (*c_it) != "mmo.system.indexes") {
			collection_list_tmp.push_back(*c_it);
		}
	}
	collection_list_tmp.push_back("mmo.role");
	for(list<string>::iterator collection_it = collection_list_tmp.begin(); collection_it != collection_list_tmp.end(); ++collection_it) {
		const std::string table_name = *collection_it;
		for (Role_Vec::iterator it = old_role.begin(); it != old_role.end(); ++it) {
			dest_conn.remove(table_name, QUERY(Role_Fields::ROLE_ID << (long long int)*it));
		}
	}

	// todo 还有一些特殊表要处理，比如如果加入公会，要从公会表那边移除相关role_id
	// 已要处理的表：公会(gang), 组队竞技(team_sports),副本表(fb_info),竞技场(arena_season)



	std::cout << "remove level less:" << CONFIG_CACHE->server_maintainer_cache().merge_level_lt_
			  << ", last login greater:" << CONFIG_CACHE->server_maintainer_cache().merge_login_day_
			  << " day, total:" << old_role.size() << std::endl;

	return 0;
}
