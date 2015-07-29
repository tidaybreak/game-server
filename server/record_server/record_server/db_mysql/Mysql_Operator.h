// -*- C++ -*-
/*
 * Mysql_Operator.h
 *
 *  Created on: Aug 29, 2012
 *      Author: ChenLong
 */

#ifndef DB_RECORD_H_
#define DB_RECORD_H_

#include "Mysql_Fields.h"

class Block_Buffer;

namespace sql {
class Driver;
class Connection;
}

class Mysql_Operator {
public:
	const static int collector_max_num;
	const static Time_Value collector_timeout;
	const static int mysql_err_max_times;

	Mysql_Operator(void);
	virtual ~Mysql_Operator(void);

	int init(void);

	int tick(Time_Value &now);

	void mysql_err_handler(void);
	void generate_mysql_err_file(void);

	int execute_collector(Block_Buffer &buf);

	int commit(void);

private:
	bool connect_db(void);
	
	int process_mysql_errcode(int err_code);

	/// 直接执行collector中的SQL语句
	int execute_collector(std::string &sql_str);

	int tick_collector(Time_Value &now);

	/// collector中的数据条数num超过最大条数时执行该collector中的SQL语句
	int cond_execute_collector(Data_Collector &collector);

	void init_collector(void);

	void init_test_collector(void);

	String_Vec split(std::string str,std::string pattern);

private:
	bool mysql_on_off_;

	int mysql_err_times_; /// MySQL操作失败次数(包括connect, exec SQL操作等)

    sql::Driver *driver_;
    sql::Connection *conn_;

	Data_Collector test_collector_;
};

typedef Singleton<Mysql_Operator> Mysql_Operator_Single;
#define MYSQL_OPERATOR    (Mysql_Operator_Single::instance())

#endif /* DB_RECORD_H_ */
