// -*- C++ -*-
/*
 * Configurator.h
 *
 *  Created on: May 3, 2012
 *      Author: Ti
 */

#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include "Thread_Mutex.h"
#include "Json_Value.h"
#include "Singleton.h"

class Configurator {
public:
	typedef boost::unordered_map<std::string, int> Module_Update_Times;

	Configurator(void);
	virtual ~Configurator(void);

	int init_config(bool c_check = true);
	int update_config(String_Vec &files, std::string module = "");

	int load_json_file(const char *file_path, Json::Value &conf_value);
	int load_json_int_map(const std::string &dir, JsonKey_Func func, Config_Entry &config);

	int load_config(void);
	int load_open_solution(void);
	int load_all_config(void);
	int load_global_config(void);
	int load_all_config(String_Vec &file_path, std::string module);
	void load_all_config(Json::Value &json, std::string path);
	int load_module_config(std::string module, String_Vec &files);

	const Json::Value &config_json(void);
	const Json::Value &open_solution(void);

	//字符串分割函数
	String_Vec split(std::string str,std::string pattern);
private:
	Configurator(const Configurator &);
	const Configurator &operator=(const Configurator &);

private:
	Json::Reader reader_;
	Json::Value config_json_;
	Json::Value open_solution_;
	Module_Update_Times module_update_times;

public:
	Time_Value update_tick_;

};

typedef Singleton<Configurator> Configurator_Single;
#define CONFIG_INSTANCE    (Configurator_Single::instance())

#endif /* CONFIGURATOR_H_ */
