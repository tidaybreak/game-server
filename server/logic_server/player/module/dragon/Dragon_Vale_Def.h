/*
 * Dragon_Vale_Def.h
 *
 *  Created on: 2014年7月10日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_DEF_H_
#define DRAGON_VALE_DEF_H_


//建筑类型
enum Building_Type {
	BUILDING_GOLD_MINE = 1, //金矿
	BUILDING_MOON_WELL = 2, //月亮井
	BUILDING_TOTEM = 3,     //图腾
};

//建筑升级状态
enum Building_Upgrade_Status {
	BUILDING_UPGRADE_NO = 0,    //不是升级中，也不是升级完成
	BUILDING_UPGRADE_DOING = 1, //升级中
	BUILDING_UPGRADE_FINISH = 2,//升级完成
};

//龙培养的类型
enum Keep_Dragon_Type {
	KEEP_DRAGON_POWER = 1,   //主角斗力
	KEEP_DRAGON_ARMOR= 2,    //主角护甲
	KEEP_DRAGON_STAMINA = 3, //主角耐力
	KEEP_DRAGON_HERO_POWER = 4, //英雄斗力
	KEEP_DRAGON_HERO_ARMOR = 5, //英雄护甲
	KEEP_DRAGON_HERO_STAMINA = 6,    //英雄耐力
	KEEP_DRAGON_CRIT = 7, //主角暴击
	KEEP_DRAGON_PREVENT_CRIT = 8, //主角抗暴
	KEEP_DRAGON_HERO_CRIT = 9, //英雄暴击
	KEEP_DRAGON_HERO_PREVENT_CRIT = 10, //英雄抗暴
};

enum Building_Notice_Enum {
	BUILDING_NOTICE_OUTPUT_PRODUCTION_MAX = 1,
	BUILDING_NOTICE_UPGRADE_FINISH = 2,
};

struct Building_Notice {
	Building_Notice() {
		reset();
	}
	void reset() {

	}
	uint8_t gold_mine;
	uint8_t moon_well;
	uint8_t totem;
};

enum Dragon_Vale_Rob_Notice_Enum {
	DRAGON_VALE_ROB_NOTICE = 1,
	DRAGON_VALE_TARGET_NOTICE = 2,
};

struct Rob_Fight_Result {
	Rob_Fight_Result() {
		reset();
	}
	void reset() {
		server_name = "";
		player_name = "";
		result = 0;
	}
	std::string server_name;
	std::string player_name;
	uint8_t result;
};

struct Gain_Rob_Production {
	Gain_Rob_Production() {
		reset();
	}
	void reset() {
		server_name = "";
		player_name = "";
		gold_mine = 0;
		moon_well = 0;
		totem = 0;
	}
	std::string server_name;
	std::string player_name;
	uint32_t gold_mine;
	uint32_t moon_well;
	uint32_t totem;
};

inline std::size_t hash_value(const Gain_Rob_Production &gain_rob_production) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<uint32_t>(gain_rob_production.gold_mine));
    boost::hash_combine(seed, static_cast<uint32_t>(gain_rob_production.moon_well));
    boost::hash_combine(seed, static_cast<uint32_t>(gain_rob_production.totem));
    return seed;
}

inline bool operator==(const Gain_Rob_Production &a, const Gain_Rob_Production &b) {
	return a.gold_mine == b.gold_mine && a.moon_well == b.moon_well && a.totem == b.totem;
}

enum Thief_Quality {
	THIEF_QUALITY_WHITE = 1,
	THIEF_QUALITY_BLUE = 2,
	THIEF_QUALITY_ORANGE = 3,
	THIEF_QUALITY_END
};

enum Notice_Board_TYPE {
	NOTICE_BOARD_STEP_FOOT = 1,
	NOTICE_BOARD_DRIVE_THIEF = 2,
	NOTICE_BOARD_THIEF_APPEAR = 3,
	NOTICE_BOARD_ROB = 4,
	NOTICE_BOARD_PLAY_DRAGON = 5,
};

struct Logic_Rob_Production {
	Logic_Rob_Production() {
		reset();
	}
	void reset() {
		role_id = 0;
		id = 0;
		type = 0;
	}
	int64_t role_id;
	uint32_t id;
	uint8_t type;
};

inline std::size_t hash_value(const Logic_Rob_Production &logic_rob_production) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<uint32_t>(logic_rob_production.role_id));
    boost::hash_combine(seed, static_cast<uint32_t>(logic_rob_production.id));
    boost::hash_combine(seed, static_cast<uint32_t>(logic_rob_production.type));
    return seed;
}

inline bool operator==(const Logic_Rob_Production &a, const Logic_Rob_Production &b) {
	return a.role_id == b.role_id && a.id == b.id && a.type == b.type;
}

typedef boost::unordered_set<Logic_Rob_Production> Logic_Rob_Production_Set;
typedef boost::unordered_map<int64_t, Logic_Rob_Production_Set> Logic_Rob_Production_Map;

typedef boost::unordered_map<uint8_t, Building_Notice> Building_Notice_Map;
typedef boost::unordered_map<uint8_t, Rob_Fight_Result> Rob_Fight_Result_Map;
typedef boost::unordered_set<Gain_Rob_Production> Gain_Rob_Prodcutin_Set;
typedef boost::unordered_map<int64_t, int64_t> Thief_Id_Quality_Map;
typedef boost::unordered_set<int64_t> Foot_Id_Set;

#endif /* DRAGON_VALE_DEF_H_ */
