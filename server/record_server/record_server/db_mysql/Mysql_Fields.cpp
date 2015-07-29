/*
 * Mysql_Fields.h
 *
 *  Created on: Nov 19, 2012
 *      Author: Ti
 */

#include "Mysql_Fields.h"

void Test_Table::append_insert_handler(Data_Collector &collector) {
	collector.sql_str_ += "(";

	std::stringstream stream;

	/// role_id
	if (role_id_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << (role_id_.data_);
		collector.sql_str_ += stream.str();
	}

	/// role_name
	collector.sql_str_ += ",";
	if (role_name_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << "\"" << role_name_.data_ << "\"";
		collector.sql_str_ += stream.str();
	}

	collector.sql_str_ += ")";
}

