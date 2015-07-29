/*
 * Mysql_Operator.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: ChenLong
 */

#include "Mysql_Operator.h"
#include "Config_Cache.h"
#include "Block_Buffer.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Record_Struct.h"

Mysql_Operator::Mysql_Operator(void)
: mysql_on_off_(false),
  mysql_err_times_(0),
  driver_(0),
  conn_(0)
{ }

Mysql_Operator::~Mysql_Operator(void) { }


const int Mysql_Operator::collector_max_num = 1000;
const Time_Value Mysql_Operator::collector_timeout = Time_Value(5, 0);

const int Mysql_Operator::mysql_err_max_times = 20;

int Mysql_Operator::init(void) {

	const Server_Maintainer_Cache &server_maintainer = CONFIG_CACHE->server_maintainer_cache();

	mysql_on_off_ = server_maintainer.mysql_ip == "" ? false : true;

	connect_db();

	init_collector();

	return 0;
}

int Mysql_Operator::tick(Time_Value &now) {
	tick_collector(now);
	return 0;
}

void Mysql_Operator::mysql_err_handler(void) {
	if (++mysql_err_times_ > mysql_err_max_times) {
		generate_mysql_err_file();
		mysql_err_times_ = 0;
	}
}

void Mysql_Operator::generate_mysql_err_file(void) {
	FILE *fp = fopen("./mysql_err", "w");
	if (! fp) {
		LOG_SYS("fopen");
		return;
	}
	fclose(fp);
}

bool Mysql_Operator::connect_db(void) {
	if (! mysql_on_off_)
		return false;

	const Server_Maintainer_Cache &server_maintainer = CONFIG_CACHE->server_maintainer_cache();

	std::stringstream stream;
	stream << server_maintainer.mysql_port;
	std::string url = "tcp://" + server_maintainer.mysql_ip + ":" + stream.str();
	try {
		driver_ = get_driver_instance();
		conn_ = driver_->connect(url.c_str(), server_maintainer.mysql_user.c_str(), server_maintainer.mysql_pw.c_str());
		if (conn_->isClosed()) {
			LOG_USER("connect mysql error, url = [%s], user = [%s], pw = [%s]",
					 url.c_str(), server_maintainer.mysql_user.c_str(), server_maintainer.mysql_pw.c_str());
			mysql_err_handler();
			return false;
		}
	} catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LOG_ABORT("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		//mysql_err_handler();
		//process_mysql_errcode(err_code);
		return false;
	}

	return true;
}

int Mysql_Operator::execute_collector(Block_Buffer &buf) {
	Record_Info msg;
	msg.deserialize(buf);
	execute_collector(msg.log_str);


	return 0;
}

int Mysql_Operator::commit(void) {
	//PERF_MON("Mysql_Operator::commit");
	if (! mysql_on_off_) /// MySQL off
		return -1;

	if (! conn_) {
		return -1;
	}
	if (conn_->isClosed()) {
		return -1;
	}

	try {
		conn_->commit();
		conn_->setAutoCommit(true);
	} catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LOG_DEBUG("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		return -1;
	}
	return 0;
}

int Mysql_Operator::execute_collector(std::string &sql_str) {
	//PERF_MON("Mysql_Operator::execute_collector");
	if (! mysql_on_off_) {/// MySQL off
		if (CONFIG_CACHE->server_maintainer_cache().platform_des == "108") {
//			MSG_DEBUG("%s", sql_str.c_str());
			std::cout << sql_str;
		}
		return -1;
	}
	if (! conn_) {
		LOG_DEBUG("conn_ == 0");
		connect_db();
		return -1;
	}
	if (conn_->isClosed()) {
		LOG_DEBUG("conn_ isClosed");
		connect_db();
		return -1;
	}

	try {

		const Server_Maintainer_Cache &server_maintainer = CONFIG_CACHE->server_maintainer_cache();
		conn_->setSchema(server_maintainer.mysql_dbname);

		conn_->setAutoCommit(false);

		sql::Statement *stmt = conn_->createStatement();
		if (! stmt) {
			LOG_DEBUG("stmt == 0");
			return -1;
		}

		//LOG_DEBUG("execute SQL=[%s]", sql_str.c_str());

		stmt->execute(sql_str);

		delete stmt;
//
//		conn_->commit();
//		conn_->setAutoCommit(true);

	} catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LOG_DEBUG("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s], sql_str:[%s]", err_code, e.getSQLState().c_str(), e.what(), sql_str.c_str());
		mysql_err_handler();
		process_mysql_errcode(err_code);
		return -1;
	}

	return 0;
}

int Mysql_Operator::tick_collector(Time_Value &now) {

	if (test_collector_.is_timeout(now)) {
		execute_collector(test_collector_.sql_str_);
		test_collector_.reset_used();
	}

	/// other collector

	return 0;
}

int Mysql_Operator::cond_execute_collector(Data_Collector &collector) {
	if (collector.num_ > collector.max_num_) {
		execute_collector(collector.sql_str_);
		collector.reset_used();
	}

	return 0;
}

int Mysql_Operator::process_mysql_errcode(int err_code) {
	switch (err_code) {
	case 2003: { /// Can't connect to MySQL server
		connect_db();
		break;
	}
	default : {
		break;
	}
	}

	return 0;
}

void Mysql_Operator::init_collector(void) {
	init_test_collector();

	/// ... 其他 表collector
	//初始化表结构，如果表不存在就会生成
	String_String_Map mysql_init_sql_map_=CONFIG_CACHE->mysql_init_config();
	for(String_String_Map::iterator iter = mysql_init_sql_map_.begin(); iter != mysql_init_sql_map_.end(); iter++){
		std::string init_sql=iter->second;
		execute_collector(init_sql);
	}
	//初始化表结束
}

void Mysql_Operator::init_test_collector(void) {
	std::string insert_head("INSERT INTO log_test (role_id,role_name) VALUES");
	test_collector_.set(insert_head, collector_max_num, collector_timeout);
}

//字符串分割函数
String_Vec Mysql_Operator::split(std::string str,std::string pattern) {
    std::string::size_type pos;
    String_Vec result;
    str += pattern;//扩展字符串以方便操作
    uint size = str.size();

    for(uint i=0; i < size; i++) {
        pos=str.find(pattern,i);
        if(pos < size) {
            std::string s = str.substr(i,pos-i);
            if (s != "")
            	result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }

    return result;
}
