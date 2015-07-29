/*
 * Config_Cache.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_H_
#define CONFIG_CACHE_H_

#include "Singleton.h"
#include "Cache_Base.h"
#include "Server_Struct.h"

struct Int_String;

class Config_Cache : public Cache_Base {
public:
	Config_Cache(void);
	~Config_Cache(void);

	int64_t server_flag(void);

	const Server_List_Map &server_list_map() const;
	const Server_Maintainer_Cache &server_maintainer_cache(void) const;
	const Server_Solution_Cache &server_solution_cache(void) const;
	const Server_Id_IP_Map &server_ip_cache() const;
	const Monitor_Link_Map &monitor_link_cache() const;
	const String_Set &db_command() const;

	/////////////////////////////////////////////////////////////////////////////////////

	void refresh_all_config_cache(std::string module);
	void set_cross_info(int flag);
	void refresh_server_list_cache(void);
	void refresh_maintainer_cache(void);
	void refresh_server_config_cache(void);
	void refresh_server_solution_cache(void);
	void refresh_server_id_ip_cache(void);
	void refresh_monitor_link_cache(void);
	void refresh_merge_cache(void);
	void check_all_config_cache(void);
	void check_cache_solution(void);

	// ==== MYSQL config ====
	const String_String_Map &mysql_init_config() const;
	void refresh_mysql_init_config(void);

	std::vector<Int_String> &merge_server_list(void);
	std::vector<Mysql_Conf> &merge_mysql_list(void);

private:
	int64_t server_flag_;							// 根据第一个服编号跟代理编号生成全服唯一编号
	String_Set db_command_;
	Server_List_Map server_list_map_;
	Server_Maintainer_Cache server_maintainer_cache_;
	Server_Solution_Cache server_solution_cache_;
	Server_Id_IP_Map server_ip_ip_cache_;			// Run id  -> IP_Info
	Monitor_Link_Map monitor_link_cache_;			// Monitor -> IP Port
	String_String_Map mysql_init_sql_map_;			// MYSQL config ====

	std::vector<Int_String> merge_server_list_;
	std::vector<Mysql_Conf> merge_mysql_list_;

};
typedef Singleton<Config_Cache> Config_Cache_Single;
#define CONFIG_CACHE    (Config_Cache_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_H_ */
