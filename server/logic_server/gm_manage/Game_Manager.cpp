/*
 * Game_Manager.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: ti
 */

#include "Game_Manager.h"
#include "DB_Operator.h"

Game_Manager::Game_Manager() {
	// TODO Auto-generated constructor stub

}

Game_Manager::~Game_Manager() {
	// TODO Auto-generated destructor stub
}

void Game_Manager::start(void) {
	init_forbid_set();
}

void Game_Manager::init_forbid_set(){
	MONGO_OPERATOR->load_forbid_list(forbid_account_list_, forbid_role_id_list_);//封号
	MONGO_OPERATOR->load_forbid_ip_list(forbid_ip_set_);//封IP
}

bool Game_Manager::check_forbid_ip_set(std::string &ip) {
	//return forbid_ip_set_.count(ip);
	String_Time_Map::iterator find_it = forbid_ip_set_.find(ip);
	if (find_it != forbid_ip_set_.end()) {
		Time_Value now = Time_Value::gettimeofday();//获取当前时间
		if(find_it->second.sec()<now.sec()){
			forbid_ip_set_.erase(find_it);
			return false;
		}else{
			return true;
		}
	}
	return false;
}

bool Game_Manager::check_forbid_account_list(std::string &account) {
	//return forbid_account_list_.count(account);
	String_Time_Map::iterator find_it = forbid_account_list_.find(account);
	if (find_it != forbid_account_list_.end()) {
		Time_Value now = Time_Value::gettimeofday();//获取当前时间
		if(find_it->second.sec()<now.sec()){
			forbid_account_list_.erase(find_it);
			return false;
		}else{
			return true;
		}
	}
	return false;
}

bool Game_Manager::check_forbid_role_id_list(role_id_t role_id) {
	//return forbid_role_id_list_.count(role_id);
	Role_Id_Time_Map::iterator find_it = forbid_role_id_list_.find(role_id);
	if (find_it != forbid_role_id_list_.end()) {
		Time_Value now = Time_Value::gettimeofday();//获取当前时间
		if(find_it->second.sec()<now.sec()){
			forbid_role_id_list_.erase(find_it);
			return false;
		}else{
			return true;
		}
	}
	return false;

}
//PHP通知封帐号处理
void Game_Manager::set_forbid_account(std::string &account, Time_Value time){
	String_Time_Map::iterator find_it = forbid_account_list_.find(account);
	if (find_it != forbid_account_list_.end()) {
		//已经存在list里面
		find_it->second = time;
	}else{
		//不存在list里面
		forbid_account_list_.insert(std::make_pair(account, time));
	}
}
//PHP通知封角色ID处理
void Game_Manager::set_forbid_role_id(role_id_t role_id, Time_Value time){
	Role_Id_Time_Map::iterator find_it = forbid_role_id_list_.find(role_id);
	if (find_it != forbid_role_id_list_.end()) {
		//已经存在list里面
		find_it->second=time;
	}else{
		//不存在list里面
		forbid_role_id_list_.insert(std::make_pair(role_id, time));
	}
}
//PHP通知解封帐号处理
void Game_Manager::erase_forbid_account(std::string &account){
	String_Time_Map::iterator find_it = forbid_account_list_.find(account);
	if (find_it != forbid_account_list_.end()) {
		//存在list里面
		forbid_account_list_.erase(find_it);
	}
}
//PHP通知解封角色ID处理
void Game_Manager::erase_forbid_role_id(role_id_t role_id){
	Role_Id_Time_Map::iterator find_it = forbid_role_id_list_.find(role_id);
	if (find_it != forbid_role_id_list_.end()) {
		//在list里面
		forbid_role_id_list_.erase(find_it);
	}
}

//PHP通知封IP处理
void Game_Manager::set_forbid_ip(std::string &ip_address, Time_Value time){
	String_Time_Map::iterator find_it = forbid_ip_set_.find(ip_address);
	if (find_it != forbid_ip_set_.end()) {
		//已经存在list里面
		find_it->second = time;
	}else{
		//不存在list里面
		forbid_ip_set_.insert(std::make_pair(ip_address, time));
	}
}

//PHP通知解封IP处理
void Game_Manager::erase_forbid_ip(std::string &ip_address){
	String_Time_Map::iterator find_it = forbid_ip_set_.find(ip_address);
	if (find_it != forbid_ip_set_.end()) {
		//存在list里面
		forbid_ip_set_.erase(find_it);
	}
}
