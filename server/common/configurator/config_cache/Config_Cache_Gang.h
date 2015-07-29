/*
 * ConfigCacheGang.h
 *
 *  Created on: Apr 29, 2014
 *      Author: caijianwei
 */

#ifndef CONFIGCACHEGANG_H_
#define CONFIGCACHEGANG_H_

#include "Cache_Base.h"
#include "gang/Gang_Config.h"
#include "gang/Gang_War_Config.h"

class Config_Cache_Gang: public Cache_Base {
public:
	void refresh_config_cache(void);

	Gang_Config const& limit() const;
	Gang_Config_Level const& base(uint32_t level);
	Gang_Config_Authority const& power(int8_t headship);
	const Gang_Cfg_Event *find_event(const int event_id) const;
	const Gang_Cfg_Skill_Id *find_skill(const int skill_id) const;
	const Gang_Cfg_Skill_Id_Map *skill_map(void) const;
	const Gang_Cfg_Bless_Buf_Id *find_bless_buf(const int bless_buf_id) const;
	std::string team_defend_boss_name(void) const;
	const Gang_Cfg_Bless_Buf_Id_Map& bless_buf_id_map(void) const;
	const Gang_Cfg_Role_Bless *find_role_bless(const int role_lvl) const;
	const Gang_Cfg_Draw_Card *find_draw_card(const int draw_card_id) const;
	const Gang_Cfg_Shop_Item *find_shop_item(const int item_id) const;
	const Gang_War_Setting_Cfg &gang_war_setting_cfg(void) const;
	const Gang_War_Rank_Detail_Cfg *find_gang_war_rank(const int8_t index) const;
	const Gang_War_Rank_Award_Detail_Cfg *find_gang_war_rank_award(const int8_t rank) const;
	const std::string *find_weekday_string(const int weekday) const;
	const std::string *find_part_string(const int weekday) const;

public:
	int get_open_gang_boss_lv(void);
private:
	void refresh_gang_boss(void);
	void refresh_limit();
	void refresh_base();
	void refresh_power();
	void refresh_event(void);
	void refresh_skill(void);
	void refresh_bless_buf(void);
	void refresh_role_bless(void);
	void refresh_draw_card(void);
	void refresh_shop_items(void);
	void refresh_gang_war(void);

	Gang_Config gang_config_limit_;
	Gang_Config_Base gang_config_base_;
	Gang_Config_Power gang_config_power_;
	Gang_Event_Cfg_Map event_cfg_map_;
	Gang_Cfg_Skill_Id_Map skill_id_map_;
	Gang_Cfg_Bless_Buf_Id_Map bless_buf_id_map_;
	Gang_Cfg_Role_Bless_Map role_bless_map_;
	Gang_Cfg_Draw_Card_Map draw_card_map_;
	Gang_Cfg_Shop_Item_Map shop_item_map_;

	//公会战
	Gang_War_Rank_Cfg gang_war_rank_cfg_;
	Gang_War_Setting_Cfg gang_war_setting_cfg_;
	Gang_War_Rank_Award_Cfg gang_war_rank_award_cfg_;
	Int_String_Map weekday_string_;
	Int_String_Map part_string_;
	std::string team_defend_boss_name_; // 守卫帝都boss未出现时
	int gang_boss_open_lv_;
};

typedef Singleton<Config_Cache_Gang> Config_Cache_Gang_Single;
#define CONFIG_CACHE_GANG    (Config_Cache_Gang_Single::instance())

#endif /* CONFIGCACHEGANG_H_ */
