/*
 * Robot_Config.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: Linqiyou
 */

#include "Config_Cache_Robot.h"
#include "Configurator.h"
#include "Json_Utility.h"


const Robot_Config &Config_Cache_Robot::robot_config(void) const {
	return robot_config_;
}

void Config_Cache_Robot::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Robot");

	{
		const Json::Value& robot_config = CONFIG_INSTANCE->config_json()["robot"]["robot"];

		robot_config_.ip = robot_config["control"]["ip"].asString();
		robot_config_.port = robot_config["control"]["port"].asInt();
		if (robot_config_.ip == "" || !robot_config_.port)
			LOG_ABORT("");

		const Json::Value& robot_json = robot_config["control"]["robot"];
		robot_config_.name_prefix = robot_json["name_prefix"].asString();

		robot_config_.total_count = robot_json["total_count"].asInt();
		robot_config_.online_max = robot_json["online_max"].asInt();
		Json_Utility::assign_time_value(robot_json["batch_login_interval"], robot_config_.batch_login_interval);

		Json_Utility::assign_time_value(robot_config["control"]["command_interval"], robot_config_.command_interval);
		Json_Utility::assign_time_value(robot_config["control"]["run_time"], robot_config_.run_time);
		Json_Utility::assign_time_value(robot_config["control"]["chat_interval"], robot_config_.chat_time);
		Json_Utility::assign_time_value(robot_config["control"]["send_msg_interval"], robot_config_.send_msg_time);
		robot_config_.send_mode = robot_json["SendMode"].asInt();
		robot_config_.move_skip_range = robot_config["control"]["move_skip_range"].asInt();
		robot_config_.robot_def = robot_config["control"]["robot_def"].asInt();
	}
}
