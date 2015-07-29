/*
 * Scene_Dragon_Vale_Def.h
 *
 *  Created on: 2014年8月5日
 *      Author: root
 */

#ifndef SCENE_DRAGON_VALE_DEF_H_
#define SCENE_DRAGON_VALE_DEF_H_

#include <string>
#include "boost/unordered_map.hpp"

struct Dragon_Vale_Rob_Fight {
	Dragon_Vale_Rob_Fight() {
		reset();
	}
	Dragon_Vale_Rob_Fight(const Dragon_Vale_Rob_Fight &that) :uuid(0), role_id(0) {
		uuid = that.uuid;
		role_id = that.role_id;
		server_name = that.server_name;
		role_name = that.role_name;
	}
	Dragon_Vale_Rob_Fight& operator=(const Dragon_Vale_Rob_Fight &that) {
		if(&that == this) {
			return *this;
		}
		uuid = that.uuid;
		role_id = that.role_id;
		server_name = that.server_name;
		role_name = that.role_name;
		return *this;
	}
	void reset() {
		uuid = 0;
		role_id = 0;
		server_name = "";
		role_name = "";
	}
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
};

inline std::size_t hash_value(const Dragon_Vale_Rob_Fight &dragon_vale_rob_fight) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<int64_t>(dragon_vale_rob_fight.uuid));
    boost::hash_combine(seed, static_cast<int64_t>(dragon_vale_rob_fight.role_id));
    return seed;
}

inline bool operator==(const Dragon_Vale_Rob_Fight &a, const Dragon_Vale_Rob_Fight &b) {
	return a.uuid == b.uuid && a.role_id == b.role_id;
}

struct Scene_Complete {
	Scene_Complete() {
		reset();
	}
	Scene_Complete(const Scene_Complete &that) :uuid(0), role_id(0) {
		uuid = that.uuid;
		role_id = that.role_id;
		server_name = that.server_name;
		role_name = that.role_name;
		result = that.result;
	}
	Scene_Complete& operator=(const Scene_Complete &that) {
		if(&that == this) {
			return *this;
		}
		uuid = that.uuid;
		role_id = that.role_id;
		server_name = that.server_name;
		role_name = that.role_name;
		result = that.result;
		return *this;
	}
	void reset() {
		uuid = 0;
		role_id = 0;
		server_name = "";
		role_name = "";
		result = 0;
	}
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
	uint8_t result; //战斗结果。 1：胜利 2：失败
};

inline std::size_t hash_value(const Scene_Complete &scene_complete) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<int64_t>(scene_complete.uuid));
    boost::hash_combine(seed, static_cast<int64_t>(scene_complete.role_id));
    return seed;
}

inline bool operator==(const Scene_Complete &a, const Scene_Complete &b) {
	return a.uuid == b.uuid && a.role_id == b.role_id;
}

typedef boost::unordered_map<uint8_t, uint32_t> Rob_Production_Map;

struct Total_Rob_Production_Info {
	Total_Rob_Production_Info() {
		reset();
	}
	Total_Rob_Production_Info(const Total_Rob_Production_Info &that) :
		to_uuid(0), to_role_id(0), uuid(0), role_id(0){
		this->to_uuid = that.to_uuid;
		this->to_role_id = that.to_role_id;
		this->to_server_name = that.to_server_name;
		this->to_role_name = that.to_role_name;
		this->uuid = that.uuid;
		this->role_id = that.role_id;
		this->server_name = that.server_name;
		this->role_name = that.role_name;
		this->rob_production_map.clear();
		this->rob_production_map.insert(that.rob_production_map.begin(), that.rob_production_map.end());
	}
	Total_Rob_Production_Info& operator=(const Total_Rob_Production_Info &that) {
		if(&that == this) {
			return *this;
		}
		this->to_uuid = that.to_uuid;
		this->to_role_id = that.to_role_id;
		this->to_server_name = that.to_server_name;
		this->to_role_name = that.to_role_name;
		this->uuid = that.uuid;
		this->role_id = that.role_id;
		this->server_name = that.server_name;
		this->role_name = that.role_name;
		this->rob_production_map.clear();
		this->rob_production_map.insert(that.rob_production_map.begin(), that.rob_production_map.end());
		return *this;
	}
	void reset() {
		to_uuid = 0;
		to_role_id = 0;
		to_server_name = "";
		to_role_name = "";
		uuid = 0;
		role_id = 0;
		server_name = "";
		role_name = "";
		rob_production_map.clear();
	}
	int64_t to_uuid;
	int64_t to_role_id;
	std::string to_server_name;
	std::string to_role_name;
	int64_t uuid;
	int64_t role_id;
	std::string server_name;
	std::string role_name;
	Rob_Production_Map rob_production_map;
};

typedef boost::unordered_map<Dragon_Vale_Rob_Fight, Dragon_Vale_Rob_Fight> Dragon_Vale_Rob_Fight_Map;
typedef boost::unordered_map<Dragon_Vale_Rob_Fight, Scene_Complete> Scene_Complete_Map;
typedef boost::unordered_map<int64_t, Total_Rob_Production_Info> Total_Rob_Production_Info_Map;

#endif /* SCENE_DRAGON_VALE_DEF_H_ */
