/*
 * Reward_Manager.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include "Reward_Manager.h"
#include "DB_Operator.h"
#include "Config_Cache_Reward.h"
#include "Config_Cache.h"
#include "Reward_Struct.h"
#include <openssl/md5.h>
#include "DB_Manager.h"
#include "Reward_Def.h"
#include "DB_Def.h"

Reward_Manager::Reward_Manager() {
	// TODO Auto-generated constructor stub
	reset();
}

Reward_Manager::~Reward_Manager() {
	// TODO Auto-generated destructor stub
}

void Reward_Manager::reset(void) {
	online_gif_conf_vec_.clear();
	group_id = 0;
	refresh_time = Time_Value::zero;
	activation_key.clear();
	activation_receive_id.clear();
}

int Reward_Manager::start(void) {
	refresh_online_group();
	load_all_activation_key();
	return 0;
}

void Reward_Manager::tick(const Time_Value &now) {
	if(refresh_time.sec() <= now.sec()) {
		refresh_online_group();
	}
}

const uint32_t Reward_Manager::get_online_gift_time_by_index(uint32_t index) const {
	if(online_gif_conf_vec_.size() <= index)
		return 0;
	return online_gif_conf_vec_[index].online_time;
}

const uint32_t Reward_Manager::get_online_gift_id_by_index(uint32_t index) const {
	if(online_gif_conf_vec_.size() <= index)
		return 0;
	return online_gif_conf_vec_[index].id;
}

const Online_Gift_Config* Reward_Manager::get_online_gift_conf(uint32_t index) const {
	if(online_gif_conf_vec_.size() <= index)
		return 0;
	return &online_gif_conf_vec_[index];
}

const uint32_t Reward_Manager::get_group_id(void) {
	return group_id;
}

void Reward_Manager::refresh_online_group(void) {
	const Server_Maintainer_Cache server_maintainer = CONFIG_CACHE->server_maintainer_cache();
	int days = get_time_subdays(server_maintainer.game_open_time, Time_Value::gettimeofday());
	uint32_t week = days/7 + 1;
	group_id = CONFIG_CACHE_REWARD->get_group_id_by_week(week);
	const Online_Gift_Conf_Vec *gconf = CONFIG_CACHE_REWARD->get_online_gif_conf_vec_by_group_id(group_id);
	if(gconf) {
		online_gif_conf_vec_ = *gconf;
	}

	//得出下一次刷新时间
	const Online_Group_Conf *conf = CONFIG_CACHE_REWARD->get_online_group_conf_by_group_id(group_id);
	if(conf) {
		Time_Value game_open_day_zero_time = Time_Value::zero;
		get_zero_time(server_maintainer.game_open_time, game_open_day_zero_time);
		refresh_time.sec(game_open_day_zero_time.sec() + conf->end * Time_Value::ONE_WEEK_IN_SECS);
	} else {
		refresh_time.sec(Time_Value::gettimeofday().sec() + Time_Value::ONE_WEEK_IN_SECS);
	}
}



const Time_Value& Reward_Manager::get_refresh_time(void) {
	return refresh_time;
}

void Reward_Manager::load_all_activation_key(void) {
	MONGO_OPERATOR->load_all_activatiion_code_key(activation_key);
}

void Reward_Manager::load_all_activation_receive_id(void) {
	MONGO_OPERATOR->load_all_activation_receive_id(activation_receive_id);
}

bool Reward_Manager::activation_code_has_receive_id(long_id_t id) {
	return activation_receive_id.find(id) != activation_receive_id.end();
}

int Reward_Manager::add_activation_key(std::vector<std::string> &key) {
	activation_key.insert(key.begin(), key.end());
	return 0;
}

void Reward_Manager::activation_code_receive_save_id(long_id_t id) {
	activation_receive_id.insert(id);
	//同步到DB
	Block_Buffer buf;
	buf.make_message(MONGO_ACTIVATION_CODE_OP);
	buf.write_uint32(ACTIVATION_CODE_RECEIVE_ID);
	buf.write_int64(id);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
}

void Reward_Manager::activation_code_decode(std::string &code, Activation_Code_Info &code_info) {
	code_info.reset();
	if(code.size() != 18) {
		return;
	}
	// 0.将字符串转为小写
	std::transform(code.begin(), code.end(), code.begin(), ::tolower);
	// 1.第一个字母为随机数，转为10进制
	std::string first_code = code.substr(0, 1);
	uint32_t rand = change_36_to_10(first_code);
	// 2.计算随机数的余数
	uint32_t randMod = rand % 17;
	// 3.除第一位以外的其他字符串信息
	code.erase(code.begin());
	// 4.字符位移还原
	code = code.substr(randMod) + code.substr(0,randMod);
	// 5.验证flag
	std::string keyId = code.substr(3, 6);
	std::string keyType = code.substr(0, 3);
	std::string keyFlag = code.substr(9, 8);
	std::string key;
	key.clear();
	for(String_Set::iterator it = activation_key.begin(); it != activation_key.end(); ++it) {
		std::stringstream str_source;
		str_source.str("");
		str_source << keyId << '#' << keyType << '@' << (*it) << '#' << rand;
		std::string md5_flag;
		md5_flag.clear();
		md5_string(str_source, md5_flag);
		std::string flag =	md5_flag.substr(randMod, 8);
		if(flag == keyFlag) {
			key = *it;
			break;
		}
	}
	if(key.empty()) {
		return;
	}
	// 6.type计算，右移随机数的余数
	uint32_t type = 0;
	{
		std::stringstream type_str;
		type_str.str("");
		uint32_t num = randMod % keyType.size();
		for(uint32_t i = 0; i < keyType.size(); ++i) {
			if(i < num) {
				type_str << keyType[keyType.size() - num + i];
			} else {
				type_str << keyType[i - num];
			}
		}
		type = change_36_to_10(type_str.str());
	}
	// 7.id计算，右移随机数的余数
	uint32_t id = 0;
	{
		std::stringstream id_str;
		id_str.str("");
		uint32_t num = randMod % keyId.size();
		for(uint32_t i = 0; i < keyId.size(); ++i) {
			if(i < num) {
				id_str << keyId[keyId.size() - num + i];
			} else {
				id_str << keyId[i - num];
			}
		}
		id = change_36_to_10(id_str.str());
	}

	code_info.type = type;
	code_info.id = id;
	code_info.rand = rand;

	return;
}

int Reward_Manager::change_36_to_10(const std::string &str) {
	int result = 0;
	int size = str.size();
	for (std::string::const_iterator iter = str.begin(); iter != str.end(); ++iter,--size) {
		if ('0' <= *iter && *iter <= '9') {
			result += (*iter - '0') * pow(36, size-1);
		} else if ('a' <= *iter && *iter <= 'z') {
			if (size == 1) {
				result += (*iter - 'a' + 10);
			} else {
				result += (*iter - 'a' + 10) * pow(36, size-1);
			}
		} else {
			result = 0;
			break;
		}
	}
	return result;
}

int Reward_Manager::md5_string(const std::stringstream &str_src, std::string &str_res) {
	const unsigned char *result_md5 = MD5((const unsigned char *) str_src.str().c_str(), str_src.str().length(), 0);
	char char_str_res[50];
	memset(char_str_res, 0, sizeof(char_str_res));
	for (uint i = 0; i < 16; i++) {
		snprintf(&char_str_res[i * 2], sizeof(char_str_res), "%.2x", result_md5[i]);
	}
	str_res.assign(char_str_res);
//	str_res = (*result_md5);
	return 0;
}
