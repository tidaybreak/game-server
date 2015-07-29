/*
 * DB_Operator_Merge_Server.h
 *
 *  Created on: Feb 3, 2015
 *      Author: gaojianshan
 */

#ifndef DB_OPERATOR_MERGE_SERVER_H_
#define DB_OPERATOR_MERGE_SERVER_H_

#include "Game_Header.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

typedef std::map<sql::Connection*, Mysql_Conf> Conn_Dbname_Map;

class DB_Merge_Server_Manager {
public:
	DB_Merge_Server_Manager() {
		reset();
	}
	void reset() {
		is_merge_server = false;
	}

	mongo::DBClientConnection *connection_merge_server(std::string ip, uint32_t port);

	void merge_server();
	bool get_is_merge_server() const;
	void set_is_merge_server(bool flag);


	/*
	 * dest_conn:目标服的mongo连接
	 * source_iter_record:源mongo对应集合字段
	 * 说明:下面为合服时特殊表的处理,参考merge_mmo_role做法
	 */
	int merge_backstage_collection(DBClientConnection &dest_conn, DBClientConnection &source_conn, std::string table_name);
	int merge_mmo_role(DBClientConnection &dest_conn, DBClientConnection &source_conn);
	int merge_mmo_arena_season(DBClientConnection &dest_conn, DBClientConnection &source_conn);
	int merge_mmo_fb_info(DBClientConnection &dest_conn, DBClientConnection &source_conn);
	int merge_mmo_team_sports(DBClientConnection &dest_conn, DBClientConnection &source_conn);
	int merge_mmo_world_boss(DBClientConnection &dest_conn, DBClientConnection &source_conn);

	int merge_mmo_other(DBClientConnection &dest_conn, DBClientConnection &source_conn, std::vector<BSONObj> &source_iter_record, std::string table_name);
	void merge_database_mmo(DBClientConnection &dest_conn, std::vector<Int_String> &server_list);
	void merge_database_backstage(DBClientConnection &dest_conn, std::vector<Int_String> &server_list);

private:
	void merge_database();
	void merge_mysql_database(Conn_Dbname_Map &conn_dbname_map);

	void send_rename_mail(role_id_t role_id, DBClientConnection *dest_conn);
	int clean_old_role(DBClientConnection &dest_conn);

private:
	bool is_merge_server;
	Role_Vec old_role_;
};

typedef Singleton<DB_Merge_Server_Manager> DB_Merge_Server_Manager_Single;
#define DB_MERGE_SERVER_MANAGER    (DB_Merge_Server_Manager_Single::instance())

#endif /* DB_OPERATOR_MERGE_SERVER_H_ */


/*

activities_stores			??
activity      				合并
arena_player  				合并
arena_season  				特殊处理			@忆盛
auction_goods 				??
campfire	  				合并
chat_record					删除
common						??
expedition					删除
expedition_g_manager_data	删除
expedition_g_manager_data_b	删除
expedition_manager			删除
expedition_manager_bak		删除
expedition_manager_data		删除
fb_info 					特殊处理			@忆盛
gang		  				合并
gang_chat_record			删除
gang_war_info				删除
gang_war_info_bak			删除
invest_plan_history　		合并
mongo_repair 				合并
ranking						删除
team_sports      			特殊处理			@林其友
title_record				删除
world_boss					特殊处理			@忆盛


 */
