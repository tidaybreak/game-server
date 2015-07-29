/*
 * Gang_Config.h
 *
 *  Created on: Apr 24, 2014
 *      Author: caijianwei
 */

#ifndef GANG_CONFIG_H_
#define GANG_CONFIG_H_

struct Gang_Config {
	Gang_Config() { reset(); }
	void reset();

	uint32_t create_cost;
	uint32_t create_cost_copper;
	uint32_t change_chief;
	uint32_t cost_notice;
	uint32_t cost_friendset;
	uint32_t diam_donate;
	uint32_t diam_donate_plus;
	uint32_t gold_donate;
	uint32_t gold_donate_plus;
	int32_t limit_level;
	int32_t upgrade_cd_clear_rate;
	int32_t diam_upgrade_gift;
	int32_t group_mail_cost;
	int32_t min_donate_copper;
	int32_t min_donate_diam;
	int32_t max_gang_lvl;
	double copper_to_contrib;
	double diam_to_contrib;
	Time_Value cd_notice;
	Time_Value cd_friendset;
	Time_Value join_cd;
	Time_Value change_noticecd;
	Time_Value cd_seize_power;
};

inline void Gang_Config::reset() {
	create_cost = 0;
	create_cost_copper = 0;
	change_chief = 0;
	cost_notice = 0;
	cost_friendset = 0;
	limit_level = 0;
	diam_donate = 0;
	diam_donate_plus = 0;
	gold_donate = 0;
	gold_donate_plus = 0;
	upgrade_cd_clear_rate = 0;
	diam_upgrade_gift = 0;
	group_mail_cost = 0;
	min_donate_copper = 0;
	min_donate_diam = 0;
	max_gang_lvl = 0;
	copper_to_contrib = 0.0;
	diam_to_contrib = 0.0;
	cd_notice = Time_Value::zero;
	cd_friendset = Time_Value::zero;
	join_cd = Time_Value::zero;
	change_noticecd = Time_Value::zero;
	cd_seize_power = Time_Value::zero;
}

struct Gang_Config_Level {
	Gang_Config_Level() { reset(); }
	void reset () {
		level = 0;
		population = 0;
		cost = 0;
		money = 0;
		donate_gain = 0;
		upgrade_cd = Time_Value::zero;
		tower_upgrade_cd = Time_Value::zero;
		altar_upgrade_cd = Time_Value::zero;
		shop_upgrade_cd = Time_Value::zero;
		draw_box.clear();
		daily_draw_times = 0;
		draw_cost_contrib = 0;
		tower_up_cost_contrib = 0;
		altar_up_cost_contrib = 0;
		shop_upgrade_contrib = 0;
		upgrade_gift.clear();
	}

	uint32_t level;
	uint32_t population;
	uint32_t cost;
	uint32_t money;
	uint32_t donate_gain;
	Time_Value upgrade_cd;
	Time_Value tower_upgrade_cd;
	Time_Value altar_upgrade_cd;
	Time_Value shop_upgrade_cd;
	Int_Vec draw_box;
	int daily_draw_times;
	int draw_cost_contrib;
	int tower_up_cost_contrib;
	int altar_up_cost_contrib;
	int shop_upgrade_contrib;
	Item_Info_Vec upgrade_gift;
};

struct Gang_Config_Base {
	typedef std::map<uint32_t, Gang_Config_Level> Gang_Level_Map;

	Gang_Config_Base() { reset(); }
	void reset();
	void add_level(const Gang_Config_Level& level);
	Gang_Config_Level const& get_level(uint32_t level);

	Gang_Level_Map base_levels;
};

inline void Gang_Config_Base::reset() {
	base_levels.clear();
}

inline void Gang_Config_Base::add_level(const Gang_Config_Level& level) {
	base_levels[level.level] = level;
}

inline const Gang_Config_Level& Gang_Config_Base::get_level( uint32_t level) {
	return (base_levels[level]);
}

struct Gang_Config_Authority {
	Gang_Config_Authority() { reset(); }
	void reset() {
		headship = 0;
		max_count = 0;
		name.clear();
		default_auth.clear();
		can_granted.clear();
	}

	int32_t auth_to_bit() const {
		return Gang_Config_Authority::vec_auth_to_bit(default_auth);
	}

	int32_t granted_to_bit() const {
		return Gang_Config_Authority::vec_auth_to_bit(can_granted);
	}

	static int32_t vec_auth_to_bit(Int_Vec const& vec) {
		int32_t bit_auth = 0;
		for(Int_Vec::const_iterator iter=vec.begin(); iter!=vec.end(); ++iter) {
			bit_auth |= (0x1 << (*iter -1));
		}
		return bit_auth;
	}

	static int32_t vec_auth_to_bit(Int8_Vec const& vec) {
		int32_t bit_auth = 0;
		for(Int8_Vec::const_iterator iter=vec.begin(); iter!=vec.end(); ++iter) {
			bit_auth |= (0x1 << (*iter -1));
		}
		return bit_auth;
	}

//	template<class Auth_Vec>
//	static int32_t vec_auth_to_bit(Auth_Vec const& vec) {
//		int32_t bit_auth = 0;
//		for(Auth_Vec::const_iterator iter = vec.begin(); iter != vec.end(); ++iter) {
//			bit_auth |= (0x1 << (*iter -1));
//		}
//		return bit_auth;
//	}

	static void bit_auth_to_vec(int32_t auth, Int8_Vec& vec) {
		for( int i=0; i<32; ++i ) {
			if ( auth & (0x1<<i) ) {
				vec.push_back(i+1);
			}
		}
	}

	int8_t headship;
	int32_t max_count;
	std::string name;
	Int_Vec default_auth;
	Int_Vec can_granted;
};

struct Gang_Config_Power {
	typedef std::map<int8_t, Gang_Config_Authority> Gang_Authority_Map;
	Gang_Config_Power() { reset(); }
	void reset() {
		authorities.clear();
	}

	void add_authority(const Gang_Config_Authority& auth);
	const Gang_Config_Authority& get_authority(int8_t headship);

	Gang_Authority_Map authorities;
};

inline void Gang_Config_Power::add_authority(const Gang_Config_Authority& auth) {
	authorities[auth.headship] = auth;
}

inline const Gang_Config_Authority& Gang_Config_Power::get_authority(int8_t headship) {
	return authorities[headship];
}


struct Gang_Cfg_Event {
	Gang_Cfg_Event(void) { reset(); }
	void reset(void) {
		event_id = 0;
		event_name.clear();
		event_content.clear();
	}

	uint32_t event_id;
	std::string event_name;
	std::string event_content;
};
typedef std::map<uint32_t, Gang_Cfg_Event> Gang_Event_Cfg_Map;

struct Gang_Cfg_Skill_Id {
	Gang_Cfg_Skill_Id(void) { reset(); }
	void reset(void) {
		skill_id = 0;
		skill_lvl = 0;
		skill_type = 0;
		need_player_lvl = 0;
		need_tower_lvl = 0;
		research_cost_fund = 0;
		learn_cost_fund = 0;
		prop = 0;
		prop_val = 0.0;
	}

	uint32_t skill_id;
	int skill_lvl;
	int skill_type;
	int need_player_lvl;
	int need_tower_lvl;
	int research_cost_fund;
	int learn_cost_fund;
	int prop;
	double prop_val;
};
typedef std::map<uint32_t, Gang_Cfg_Skill_Id> Gang_Cfg_Skill_Id_Map;

struct Gang_Cfg_Bless_Buf_Id {
	Gang_Cfg_Bless_Buf_Id(void) { reset(); }
	void reset(void) {
		bless_buf_id = 0;
		need_gold = 0;
		effect_id = 0;
		effect_time = Time_Value::zero;
		effect_prop = 0;
		effect_val = 0.0;
	}

	uint32_t bless_buf_id;
	int need_gold;
	int effect_id;
	Time_Value effect_time;
	int effect_prop;
	double effect_val;
};
typedef std::map<uint32_t, Gang_Cfg_Bless_Buf_Id> Gang_Cfg_Bless_Buf_Id_Map;

struct Gang_Cfg_Role_Bless {
	Gang_Cfg_Role_Bless(void) { reset(); }
	void reset(void) {
		role_level = 0;
		bless_buf_1 = 0;
		bless_buf_2 = 0;
		bless_buf_3 = 0;
		need_gold = 0;
	}

	uint32_t role_level;
	int bless_buf_1;
	int bless_buf_2;
	int bless_buf_3;
	int need_gold;
};
typedef std::map<uint32_t, Gang_Cfg_Role_Bless> Gang_Cfg_Role_Bless_Map;

struct Gang_Cfg_Draw_Card {
	Gang_Cfg_Draw_Card(void) { reset(); }
	void reset(void) {
		draw_card_id = 0;
		item_id = 0;
		item_amount = 0;
		rate = 0;
		is_rare = false;
	}

	uint32_t draw_card_id;
	int item_id;
	int item_amount;
	int rate;
	bool is_rare;
};
typedef std::map<uint32_t, Gang_Cfg_Draw_Card> Gang_Cfg_Draw_Card_Map;

struct Gang_Cfg_Shop_Item {
	Gang_Cfg_Shop_Item(void) { reset(); }
	void reset(void) {
		item_id = 0;
		cost_contrib = 0;
		need_shop_lvl = 0;
	}

	int item_id;
	int cost_contrib;
	int need_shop_lvl;
};
typedef std::map<int, Gang_Cfg_Shop_Item> Gang_Cfg_Shop_Item_Map;

#endif /* GANG_CONFIG_H_ */
