/*
 * HotUpdate.h
 *
 *  Created on: Mar 21, 2013
 *      Author: ti
 */

#ifndef HOTUPDATE_H_
#define HOTUPDATE_H_

#include "Thread.h"
#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"
#include <vector>
#include <set>
#include "Singleton.h"

struct Config_File {
	Config_File();
	Config_File(const std::string name, const std::string path);
	void reset(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	std::string file_md5;
	std::string file_path;

	bool operator==(const Config_File &key) const {
		return file_md5 == key.file_md5;
	}

	bool operator<(const Config_File &key) const  {
		return file_md5 == key.file_md5;
	}
};

inline std::size_t hash_value(const Config_File &key) {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.file_md5);
    return seed;
}

class Hot_Update: public Thread {
public:
	typedef boost::unordered_set<Config_File> MD5_STR_SET;
	typedef boost::unordered_map<std::string, MD5_STR_SET> MD5_STR_MAP;

	Hot_Update();
	virtual ~Hot_Update();

	virtual void run_handler(void);

	int notice_update(const std::string module, MD5_STR_SET &files);

	std::string file_md5(std::string file_name);

	void check_config(std::string module);
	void init_all_module(void);
	void init_server_global_conf(void);
	void get_md5_str(std::string path, MD5_STR_SET &md5_str_set);

private:
	MD5_STR_MAP update_module_;
	Time_Value update_time_;
	static Hot_Update *instance_;
	MD5_STR_MAP md5_str_map_;

	Time_Value notify_interval_;
};
typedef Singleton<Hot_Update> Hot_Update_Single;
#define HOT_UPDATE_INSTANCE    (Hot_Update_Single::instance())

#endif /* HOTUPDATE_H_ */
