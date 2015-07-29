/*
 * Dragon_Vale_Struct.h
 *
 *  Created on: 2014年7月11日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_STRUCT_H_
#define DRAGON_VALE_STRUCT_H_

#include "Server_Struct.h"
#include "boost/unordered_map.hpp"
#include "Dragon_Vale_Def.h"

//龙谷建筑信息
struct DragonBuildingInfo  {
	DragonBuildingInfo() {
		reset();
	}
	DragonBuildingInfo(const DragonBuildingInfo &that) :
		id(0), type(0), level(0), status(BUILDING_UPGRADE_NO), production(0), is_gain(false), order(0), consume_type(0) {
		this->id = that.id;
		this->type = that.type;
		this->level = that.level;
		this->status = that.status;
		this->production = that.production;
		this->is_gain = that.is_gain;
		this->begin_upgrade_time.sec(that.begin_upgrade_time.sec());
		this->begin_upgrade_time.usec(that.begin_upgrade_time.usec());
		this->last_output_production_time.sec(that.last_output_production_time.sec());
		this->last_output_production_time.usec(that.last_output_production_time.usec());
		this->order = that.order;
		this->consume_type = that.consume_type;
	}
	DragonBuildingInfo& operator=(const DragonBuildingInfo &that) {
		if(&that == this) {
			return *this;
		}
		this->id = that.id;
		this->type = that.type;
		this->level = that.level;
		this->status = that.status;
		this->production = that.production;
		this->is_gain = that.is_gain;
		this->begin_upgrade_time.sec(that.begin_upgrade_time.sec());
		this->begin_upgrade_time.usec(that.begin_upgrade_time.usec());
		this->last_output_production_time.sec(that.last_output_production_time.sec());
		this->last_output_production_time.usec(that.last_output_production_time.usec());
		this->order = that.order;
		this->consume_type = that.consume_type;
		return *this;
	}
	void reset() {
		id = 0;
		type = 0;
		level = 0;
		status = BUILDING_UPGRADE_NO;
		production = 0;
		is_gain = false;
		order = 0;
		consume_type = 0;
	}

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	uint32_t id;                   //建筑id
	uint8_t type;                  //建筑类型
	uint16_t level;                //建筑等级
	uint8_t status;                //建筑升级状态
	uint32_t production;           //当前资源产出量
	bool is_gain;                  //是否可收获资源
	Time_Value begin_upgrade_time; //开始升级时间
	Time_Value last_output_production_time;//上一次资源产出时间
	uint8_t order;                 //建筑建造次序
	uint8_t consume_type;
};

//key: ((uint64_t)id << 32) | (uint64_t)type   value:DragonBuildingInfo
typedef boost::unordered_map<uint64_t, DragonBuildingInfo> DragonBuildingInfoMap;

//生成 DragonBuildingInfo 的key   building_id:建筑id    building_type:建筑类型
inline uint64_t create_key_of_Dragon_Building_Info(uint32_t building_id, uint8_t building_type) {
	return ((uint64_t)building_id << 32) | (uint64_t)building_type;
}

//龙信息
struct DragonInfo {
	DragonInfo() {
		reset();
	}
	void reset() {
		progress = 0;
		feeling = 0;
		power = 0;
		armor = 0;
		stamina = 0;
		power_level = 0;
		armor_level = 0;
		stamina_level = 0;
		hero_power = 0;
		hero_armor = 0;
		hero_stamina = 0;
		hero_power_level = 0;
		hero_armor_level = 0;
		hero_stamina_level = 0;
		is_play_dragon = false;
		energy = 0;
		crit = 0;
		prevent_crit = 0;
		crit_level = 0;
		prevent_crit_level = 0;
		hero_crit = 0;
		hero_prevent_crit = 0;
		hero_crit_level = 0;
		hero_prevent_crit_level = 0;
	}

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	uint8_t progress;		//进化程度
	uint8_t feeling;		//当前心情值 (不超过255,这里用uint8_t足够)
	uint32_t power;			//当前主角斗力值
	uint32_t armor;			//当前主角护甲值
	uint32_t stamina;		//当前主角耐力值
	uint32_t power_level;	//当前主角斗力等级
	uint32_t armor_level;   //当前主角护甲等级
	uint32_t stamina_level; //当前主角耐力等级
	uint32_t hero_power;		 //当前英雄斗力值
	uint32_t hero_armor;		 //当前英雄护甲值
	uint32_t hero_stamina;		 //当前英雄耐力值
	uint32_t hero_power_level;	 //当前英雄斗力等级
	uint32_t hero_armor_level;   //当前英雄护甲等级
	uint32_t hero_stamina_level; //当前英雄耐力等级
	bool is_play_dragon; 	//是否可以逗龙
	uint32_t energy;		//当前能量值
	uint32_t crit;          //主角暴击值
	uint32_t prevent_crit;  //主角抗暴值
	uint32_t crit_level;    //主角暴击等级
	uint32_t prevent_crit_level;//主角抗暴等级
	uint32_t hero_crit;     //英雄暴击值
	uint32_t hero_prevent_crit;//英雄抗暴值
	uint32_t hero_crit_level; //英雄暴击等级
	uint32_t hero_prevent_crit_level; //英雄抗暴等级
};

struct Foot_Info {
	Foot_Info() {
		reset();
	}
	void reset() {
		sex = 0;
		name = "";
	}
	uint8_t sex; 	 //1:male 2:female
	std::string name;//角色名
};

typedef std::multimap<int64_t, Foot_Info> Foot_Info_Map;

struct Notice_Board {
	Notice_Board(void) {
		reset();
	};

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_int32(time);
		w.write_string(host);
		w.write_string(target);
		w.write_uint32(dragon_soul);
		w.write_uint32(coin);
		w.write_uint32(soul);
		w.write_uint8(is_get_dragon_heart);
		w.write_uint8(param1);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_int32(time)  ||  r.read_string(host)  ||  r.read_string(target)  ||  r.read_uint32(dragon_soul)  ||  r.read_uint32(coin)  ||  r.read_uint32(soul)  ||  r.read_uint8(is_get_dragon_heart)  ||  r.read_uint8(param1)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		time = 0;
		host.clear();
		target.clear();
		dragon_soul = 0;
		coin = 0;
		soul = 0;
		is_get_dragon_heart = 0;
		param1 = 0;
	}

	uint8_t type;/*类型*/
	int32_t time;/*时间*/
	std::string host;/*主语*/
	std::string target;/*宾语*/
	uint32_t dragon_soul;/*龙魂*/
	uint32_t coin;/*金币*/
	uint32_t soul;/*英灵*/
	uint8_t is_get_dragon_heart;/*是否获得龙心  1：是， 0：否*/
	uint8_t param1;/*附加参数1*/
};

typedef std::list<Notice_Board> Notice_Board_List;

//key:order
typedef std::set<uint8_t> Cancel_Create_Building_Set;

struct Dragon_Vale_Detail: public Detail {
	Dragon_Vale_Detail(void);
	virtual ~Dragon_Vale_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	//获取各种建筑类型所有建筑的最大资源存储上限及当前产出量
	void get_building_max_storage(uint32_t &gold_mine_max_storage, uint32_t &moon_weel_max_storage,
			uint32_t &totem_max_storage, uint32_t &gold_mine_production, uint32_t &moon_weel_production,
			uint32_t &totem_production, bool is_rob = false);
	bool is_has_building_upgrading();
	void check_and_change_notice_board_num();
	bool is_has_building_finish();

	role_id_t target_player_id;  //当前玩家进入的目标龙谷的主人角色id
	bool is_rob_enter_dragon_vale;
	int64_t uuid;

	bool is_gold_mine_max_storage;//是否达到所有金矿建筑的最大存储
	bool is_moon_well_max_storage;//是否达到所有月亮井建筑的最大存储
	bool is_totem_max_storage;    //是否达到所有图腾建筑的最大存储

	uint8_t build_gold_mine_num; //已建造金矿数量
	uint8_t build_moon_well_num; //已建造月亮井数量
	uint8_t build_totem_num;     //已建造图腾数量
	uint8_t build_queue_num;     //当前可用建筑队列数目(如果为0，不可建造或升级建筑)
	uint8_t all_open_build_queue_num;//所有已开启的建筑队列数目
	DragonBuildingInfoMap dragon_building_info_map; //龙谷内已经建造出来的建筑列表

	Cancel_Create_Building_Set cancel_create_building_set;  //取消建造龙谷建筑的编号集合

	bool is_dragon_born;		 //是否已开始孵化龙蛋
	Time_Value begin_born_time;  //开始孵化龙蛋时间
	DragonInfo dragon_info;      //龙信息

	Time_Value begin_play_dragon_interval_time;  //逗龙冷却开始时间
	Time_Value begin_down_feeling_time;  		 //龙的心情值下降的开始时间

	bool is_vip_down_to_zero;	//是否vip过期

	uint32_t matched_num;

	Thief_Id_Quality_Map thief_id_quality_map;
	Time_Value begin_refresh_thief_time;

	uint8_t star_num;
	uint8_t award_foot_num;
	bool is_special;
	Foot_Info_Map foot_info_map;
	Foot_Id_Set foot_id_set;
	bool is_yesterday_login;
	uint8_t dragon_heart_num;

	Notice_Board_List notice_board_list;

	bool is_rob_protect;
	Time_Value begin_rob_protect_time;

	bool is_first_enter_after_robbed;

	bool is_in_robbed_dragon_vale;
	Time_Value begin_in_robbed_dragon_vale_time;

	bool is_dragon_vale_open;

	Time_Value begin_no_enter_time;
	uint32_t left_time;

	uint32_t  play_dragon_num;
	uint32_t drive_thief_num;

	int64_t to_uuid;
	int64_t to_role_id;
	std::string to_server_name;
	std::string to_role_name;
	uint32_t coin;
};

typedef Dragon_Vale_Detail Dragon_Vale_DB_View;

#endif /* DRAGON_VALE_STRUCT_H_ */
