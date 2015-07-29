/*
 * Reward_Manager.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef REWARD_MANAGER_H_
#define REWARD_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Reward_Config.h"

struct Activation_Code_Info;

class Reward_Manager {

public:
	Reward_Manager();
	virtual ~Reward_Manager();
	void reset(void);

	int start(void);

	void tick(const Time_Value &now);

	const Online_Gift_Config* get_online_gift_conf(uint32_t index) const;
	const uint32_t get_online_gift_time_by_index(uint32_t index) const;
	const uint32_t get_online_gift_id_by_index(uint32_t index) const;

	const uint32_t get_group_id(void);
	const Time_Value& get_refresh_time(void);

	/********************激活码相关****************************/
	void activation_code_decode(std::string &code, Activation_Code_Info &code_info);
	void activation_code_receive_save_id(long_id_t id);
	bool activation_code_has_receive_id(long_id_t id);
	int add_activation_key(std::vector<std::string> &key);


private:
	Online_Gift_Conf_Vec online_gif_conf_vec_;
	uint32_t group_id;
	Time_Value refresh_time;

	void refresh_online_group(void);

	/********************激活码相关****************************/
	String_Set activation_key;//激活码解码KEY
	Long_Id_Set activation_receive_id;//激活码已经领取的ID

	void load_all_activation_key(void);
	void load_all_activation_receive_id(void);
	int change_36_to_10(const std::string &str);
	int md5_string(const std::stringstream &str_src, std::string &str_res);
};


typedef Singleton<Reward_Manager> Reward_Manager_Single;
#define REWARD_MANAGER    (Reward_Manager_Single::instance())

#endif /* REWARD_MANAGER_H_ */
